// Seq_BoxLifter.cpp: implementation of the CSeq_BoxLifter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "handler.h"
#include "Seq_BoxLifter.h"
#include "Seq_BoxClamp.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CSeq_BoxLifter	SeqBoxLifter;

CSeq_BoxLifter::CSeq_BoxLifter()
{
	// 모터 번호 할당
	m_nMotY = M_LIFTER_Y;
	
	// yangkyu [2015.02.23]
	// 번호는 메인 설비 정면 기준으로 0(앞쪽 좌), 1(앞쪽 우), 2(뒤쪽 좌), 3(뒤쪽 우)
	//m_nMotZ0 = M_LIFTER_Z2;
	//m_nMotZ1 = M_LIFTER_Z4;
	//m_nMotZ2 = M_LIFTER_Z1;
	//m_nMotZ3 = M_LIFTER_Z3;
	// yangkyu [2015.03.25]
	// 모터 값 수정
	m_nMotZ0 = M_LIFTER_Z1;
	m_nMotZ1 = M_LIFTER_Z2;
	m_nMotZ2 = M_LIFTER_Z3;
	m_nMotZ3 = M_LIFTER_Z4;

	st_work.nLiftDownComplete = CTL_NO;
	stSync.nRcvyComplete[SITE_BOX_LIFT_] = CTL_YES;
	m_WorkSite = SITE_REAR; //kwlee 2017.0825
	//st_map.nLiftWorkSite = SIDE_REAR_;		// Accy 작업 영역 설정 (작업 순서 : Rear -> Front)
	m_nStep_Run = 0; //kwlee 2017.0711

}

CSeq_BoxLifter::~CSeq_BoxLifter()
{

}

//==================================================================//
// 쓰레드 동작
//==================================================================//
void CSeq_BoxLifter::OnSeq_RUN(void) 
{
	OnSeq_INIT();
	OnSeq_Execute();
}

//==================================================================//
// 쓰레드 관련 변수 초기화
//==================================================================//
void CSeq_BoxLifter::OnSeq_INIT(void) 
{
	if( m_bflag_INIT == true )
	{
		return;
	}
	m_bflag_INIT = true;
	
	m_nStep_Init = 0;
	m_nStep_Run		= 0;
	m_nStep_Rcvy = 0;
	m_nStep_ForceDischarge = 0;


	for ( int i = 0; i < 4; i++ )
	{
		m_nInitOK[i] = CTL_NO;
	}
	
}

//==================================================================//
// 쓰레드 제어
//==================================================================//
void CSeq_BoxLifter::OnSeq_Execute(void) 
{
	if( Func.OnIsInterruptCondition() == 1 )
	{
		return;
	}
	
	int nRet = 0;
	
	switch( st_work.nEqpStatus )
	{
	case dINIT:
		// 초기화 진행 대화상자에서 [YES] 선택했는지 여부
		if( st_handler.nInitTryREQ != YES )
		{
			break;
		}
		OnRun_Initial();
		break;
		
	case dRUN:
		if (stSync.nRcvyComplete[SITE_BOX_LIFT_] == CTL_NO)
		{
			nRet = OnRcvy_RobotPos();
			if (nRet == CTL_GOOD)
			{
				stSync.nRcvyComplete[SITE_BOX_LIFT_] = CTL_YES;
			}
		}
		else
		{
			if( st_handler.nIsAllSiteInitEnd == CTL_YES )
			{
				if(OnRun_Remove())		// Box remove
				{
					OnRun_Move();
 				}
			}
		}
		break;
		
	default:
		OnRun_ForceDischarge();
		break;
	}
	
}

void CSeq_BoxLifter::OnRun_Initial() 
{
	if( st_handler.mn_init_state[INIT_BOX_LIFT] != CTL_NO )	
	{
		return;
	}

	int nRet = VAR_INIT_;
	int nRetData[4];
	int i, nCnt = 0;
	CString sAlarmCode;

	switch(m_nStep_Init)
	{
	case 0:
		//st_map.nLiftWorkSite = SIDE_REAR_;	// Accy setting  Back(Rear) -> Middle -> Front
		m_nStep_Init = 10;
		break;

	case 10:
		for ( i = 0; i < 4; i++ )
		{
			if(m_nInitOK[i] == CTL_NO )
			{
				if( i == 0 )
				{//Z0 AXIS Init
					nRet = OnSupplyInitial0();
				}
				else if( i == 1 )
				{//Z1 AXIS Init
					nRet = OnSupplyInitial1();
				}
				else if( i == 2 )
				{//Z2 AXIS Init
					nRet = OnSupplyInitial2();
				}
				else if( i == 3 )// if( i == 3 )
				{//Z3 AXIS Init
					nRet = OnSupplyInitial3();
				}

				if( nRet == CTLBD_RET_GOOD )
				{
					m_nInitOK[i] = CTL_YES;
				}
				else if( nRet != CTLBD_RET_PROCEED )
				{
					// 000003 0 00 "LIFTER_UP_DOWN_SHIFT0_HOMING_ERR."
					// 010003 0 00 "LIFTER_UP_DOWN_SHIFT1_HOMING_ERR."
					// 020003 0 00 "LIFTER_UP_DOWN_SHIFT2_HOMING_ERR."
					// 030003 0 00 "LIFTER_UP_DOWN_SHIFT3_HOMING_ERR."
					alarm.mstr_code.Format("%02d0003", i);
					CTL_Lib.Alarm_Error_Occurrence( 2001, CTL_dWARNING, alarm.mstr_code );
					break;
				}
			}
		}

		if( m_nInitOK[0] == CTL_YES && m_nInitOK[1] == CTL_YES &&
			m_nInitOK[2] == CTL_YES && m_nInitOK[3] == CTL_YES )
		{
			st_work.nLiftDownComplete = CTL_YES;
			m_nStep_Init = 100;
		}
		break;

	case 100:
		nRet = g_comiMgr.HomeCheck_Mot(m_nMotY, 0, MOT_TIMEOUT);
		if( nRet == CTLBD_RET_GOOD )
		{
			stSync.nResp_Lifter2Clamp_Work = SYNC_RESP_WORK_COMPLETE_; 
			m_nStep_Init = 110;
		}
		else if( nRet == CTLBD_RET_ERROR )
		{
			//	040003 0 00 "LIFTER_Y_HOMING_ERR."
			alarm.mstr_code		= "040003";
			CTL_Lib.Alarm_Error_Occurrence( 2001, CTL_dWARNING, alarm.mstr_code);
		}
		break;

	case 110:
		//Box Clamp -> Lifter complete req.
		if( stSync.nReq_Clamp2Lifter_Work == SYNC_REQ_WORK_COMPLETE_)
		{
			stSync.nResp_Lifter2Clamp_Work = SYNC_RESP_RESET_;
			m_nStep_Init = 150;
		}
		break;

	case 150:
		//Lifter Y Axis -> Rear Pos, BOX Out
		nRet = CTL_Lib.OnSingleMove(m_nMotY, st_motor[m_nMotY].d_pos[Y_ACC_REAR_], (int)st_handler.md_run_speed);
		if (nRet == CTLBD_RET_GOOD)
		{
			//배출이 완료 된 후 BoxClamp에게 작업 완료 응답
			stSync.nResp_Lifter2Clamp_Work = SYNC_RESP_WORK_COMPLETE_; 
			m_nStep_Init = 200;
		}
		else if (nRet == CTLBD_RET_ERROR || nRet == CTLBD_RET_RETRY)
		{
			//  041001 0 00 "LIFTER_Y_ACC_REAR_MOVE_ERR."
			alarm.mstr_code		= "041001";
			CTL_Lib.Alarm_Error_Occurrence( 2002, CTL_dWARNING, alarm.mstr_code);
		}
		break;

	case 200:
		nRetData[0] = CTL_Lib.OnSingleMove( m_nMotZ0, st_motor[m_nMotZ0].d_pos[Z_ACC_LIFT_SAFETY_], (int)st_handler.md_run_speed );
		nRetData[1] = CTL_Lib.OnSingleMove( m_nMotZ1, st_motor[m_nMotZ1].d_pos[Z_ACC_LIFT_SAFETY_], (int)st_handler.md_run_speed );
		nRetData[2] = CTL_Lib.OnSingleMove( m_nMotZ2, st_motor[m_nMotZ2].d_pos[Z_ACC_LIFT_SAFETY_], (int)st_handler.md_run_speed );
		nRetData[3] = CTL_Lib.OnSingleMove( m_nMotZ3, st_motor[m_nMotZ3].d_pos[Z_ACC_LIFT_SAFETY_], (int)st_handler.md_run_speed );

		if( nRetData[0] == CTLBD_RET_GOOD && nRetData[1] == CTLBD_RET_GOOD && nRetData[2] == CTLBD_RET_GOOD && nRetData[3] == CTLBD_RET_GOOD)
		{
			
			m_nStep_Init = 220;
		}
		else 
		{
			if( nRetData[0] == CTLBD_RET_ERROR || nRetData[0] == CTLBD_RET_RETRY )
			{
				//001002 0 00 "LIFTER_UP_DOWN_SHIFT1_Z_ACC_LIFT_SAFETY_MOVE_ERR."
				alarm.mstr_code		= "001002";
				CTL_Lib.Alarm_Error_Occurrence( 2003, CTL_dWARNING, alarm.mstr_code);
			}
			else if( nRetData[1] == CTLBD_RET_ERROR || nRetData[1] == CTLBD_RET_RETRY )
			{
				//011002 0 00 "LIFTER_UP_DOWN_SHIFT2_Z_ACC_LIFT_SAFETY_MOVE_ERR."
				alarm.mstr_code		= "011002";
				CTL_Lib.Alarm_Error_Occurrence( 2003, CTL_dWARNING, alarm.mstr_code);
			}
			else if( nRetData[2] == CTLBD_RET_ERROR || nRetData[2] == CTLBD_RET_RETRY )
			{
				//021002 0 00 "LIFTER_UP_DOWN_SHIFT3_Z_ACC_LIFT_SAFETY_MOVE_ERR."
				alarm.mstr_code		= "021002";
				CTL_Lib.Alarm_Error_Occurrence( 2003, CTL_dWARNING, alarm.mstr_code);
			}
			else if( nRetData[3] == CTLBD_RET_ERROR || nRetData[3] == CTLBD_RET_RETRY )
			{
				//031002 0 00 "LIFTER_UP_DOWN_SHIFT4_Z_ACC_LIFT_SAFETY_MOVE_ERR."
				alarm.mstr_code		= "031002";
				CTL_Lib.Alarm_Error_Occurrence( 2003, CTL_dWARNING, alarm.mstr_code);
			}			
			/*CTL_Lib.Alarm_Error_Occurrence( 2003, CTL_dWARNING, alarm.mstr_code);*/
		}
		break;

	case 220:
		nCnt =0;
		for (i =0; i<3; i++)
		{
			nRetData[i] = !g_ioMgr.get_in_bit(stIO.i_Chk_UnloaderFull[i],IO_OFF);
			
			if (nRetData[i] == IO_ON)
			{
				nCnt++;
			}
		}
		
		if (nCnt > 0)
		{
			m_nStep_Init = 300;
		}
		else
		{
			//150000 0 00 "ACCY_UNLOADER_FULL_ERR."
			CTL_Lib.Alarm_Error_Occurrence( 2004, CTL_dWARNING, "150000");
		}
		break;

	case 300:
		if (stSync.nReq_Clamp2Lifter_Work == SYNC_REQ_RESET_)
		{
			m_nStep_Init = 1000;
		}
		break;

		
// 	case 400:
// 		nRet[0] = CTL_Lib.OnSingleMove( m_nMotY, st_motor[m_nMotY].d_pos[Y_ACC_BOX_AVOID_], (int)st_handler.md_run_speed );
// 		if( nRet[0] == CTLBD_RET_GOOD)
// 		{
// 			m_nStep_Init = 500;
// 		}
// 		else 
// 		{
// 			//001002 0 00 "LIFTER_UP_DOWN_SHIFT1_Z_ACC_LIFT_SAFETY_MOVE_ERR."
// 			alarm.mstr_code		= "001002";
// 			alarm.mn_count_mode	= 0;
// 			alarm.mn_type_mode	= eWARNING;
// 			st_work.nEqpStatus	= dWARNING;
// 			CTL_Lib.Alarm_Error_Occurrence( 2004, CTL_dWARNING, alarm.mstr_code);
// 		}
// 		break;

	case 1000:
		// 초기화 화면에서 [초기화] 완료 표시 가능
		st_handler.mn_init_state[INIT_BOX_LIFT] = CTL_YES;
		stSync.nResp_Lifter2Clamp_Work = SYNC_REQ_RESET_;
	
		m_nStep_Init = 0;
		break;
	}
}

void CSeq_BoxLifter::OnRun_Move() 
{
	//Up Complete
	int nRet_1/*,nRet_2*/;
	int nRet[3] = {0,};
	int nCnt,i;
	int nRetData[4] = {0,};
	double dTarget_Pos;
	Func.OnTrace_ThreadStep(2, m_nStep_Run);

	switch(m_nStep_Run)
	{
	case 0:
		//2017.0713
		//if BoxClamp tutns LotEnd, We make BoxLifter Lotend status and wait
		if (stSync.nLotEnd_BoxClamp == TRUE)
		{
			stSync.nLotEnd_BoxLifter = TRUE;
			break;
		}
		else
		{
			stSync.nLotEnd_BoxLifter = FALSE;
		}
		
		// BoxClamp가 Box를 잡은 상태가 되기를 대기
		if(stSync.nReq_Clamp2Lifter_Work == SYNC_REQ_ACCY_BOX_CLAMPING_COMPLETE_)
		{
			stSync.nResp_Lifter2Clamp_Work = SYNC_RESP_WORK_;
			stSync.nReq_Lifter2XYZRbt_Work = SYNC_REQ_RESET_;
			//st_map.nLiftWorkSite = SIDE_REAR_;		// Accy 작업 영역 설정 (작업 순서 : Rear -> Front)
			//kwlee 2017.0825
			//st_map.nLiftWorkSite = m_WorkSite;		// Accy 작업 영역 설정 (작업 순서 : Rear -> Front)
			m_nStep_Run = 100;
		}
		break;

	case 100:
		// Lifter 유닉을 Rear 작업 영역으로 이동
		//nRet_1 = CTL_Lib.OnSingleMove(m_nMotY, st_motor[m_nMotY].d_pos[Y_ACC_REAR_], (int)st_handler.md_run_speed);
		//kwlee 2017.0831
		st_map.nLiftWorkSite = m_WorkSite;		// Accy 작업 영역 설정 (작업 순서 : Rear -> Front)

		switch(m_WorkSite)
		{
			case SITE_REAR: dTarget_Pos = st_motor[m_nMotY].d_pos[Y_ACC_REAR_]; break;
			case SITE_MID: dTarget_Pos = st_motor[m_nMotY].d_pos[Y_ACC_MIDDLE_]; break;
			case SITE_FRONT: dTarget_Pos = st_motor[m_nMotY].d_pos[Y_ACC_FRONT_]; break;
		}
		nRet_1 = CTL_Lib.OnSingleMove(m_nMotY,dTarget_Pos, (int)st_handler.md_run_speed);

		if (nRet_1 == CTLBD_RET_GOOD)
		{
			if (st_basic.n_mode_device == WITHOUT_DVC_)
			{
				memset(st_work.nAccyCNT, 0, sizeof(st_work.nAccyCNT));
			}
			//m_nStep_Run = 200;
			//kwlee 2017.0910
			m_nStep_Run = 110;

		}
		else if (nRet_1 == CTLBD_RET_ERROR || nRet_1 == CTLBD_RET_RETRY)
		{
			// 041001 0 00 "LIFTER_Y_ACC_REAR_MOVE_ERR."
			alarm.mstr_code		= "041001";
			CTL_Lib.Alarm_Error_Occurrence(2400, CTL_dWARNING, alarm.mstr_code);
		}
		break;

		case 110:
			nRetData[0] = !g_ioMgr.get_in_bit(stIO.i_Chk_BoxClampAccyDetection[SIDE_LEFT_][ SIDE_FW_ + m_WorkSite * 2 ]);
			nRetData[1] = !g_ioMgr.get_in_bit(stIO.i_Chk_BoxClampAccyDetection[SIDE_RIGHT_][SIDE_FW_ + m_WorkSite * 2]);
			nRetData[2] = !g_ioMgr.get_in_bit(stIO.i_Chk_BoxClampAccyDetection[SIDE_LEFT_][SIDE_BW_ + m_WorkSite * 2]);
			nRetData[3] = !g_ioMgr.get_in_bit(stIO.i_Chk_BoxClampAccyDetection[SIDE_RIGHT_][SIDE_BW_ + m_WorkSite * 2]);
			
			if (nRetData[0] == IO_OFF && nRetData[1] == IO_OFF && nRetData[2] == IO_OFF && nRetData[3] == IO_OFF)
			{
				m_nStep_Run = 200;
			}
			else
			{
				if (nRetData[0] == IO_ON)
				{
					// 002000 0 00 "LIFTER_UP_DOWN_SHIFT1_LOAD_ACCY_CHK_ERR."
					alarm.mstr_code		= "002000";
				}
				else if (nRetData[1] == IO_ON)
				{
					// 012000 0 00 "LIFTER_UP_DOWN_SHIFT2_LOAD_ACCY_CHK_ERR."
					alarm.mstr_code		= "012000";
				}
				else if (nRetData[2] == IO_ON)
				{
					// 022000 0 00 "LIFTER_UP_DOWN_SHIFT3_LOAD_ACCY_CHK_ERR."
					alarm.mstr_code		= "022000";
				}
				else if (nRetData[3] == IO_ON)
				{
					// 032000 0 00 "LIFTER_UP_DOWN_SHIFT4_LOAD_ACCY_CHK_ERR."
					alarm.mstr_code		= "032000";
				}
				CTL_Lib.Alarm_Error_Occurrence(2401, CTL_dWARNING, alarm.mstr_code);
				
			}
			break;
		//TRAY, MANUA Supply
	case 200:
			m_nStandByOK[0] = CTL_NO;
			m_nStandByOK[1] = CTL_NO;
			m_nStandByOK[2] = CTL_NO;
			m_nStandByOK[3] = CTL_NO;

			dCurrPos[0] = COMI.Get_MotCurrentPos(m_nMotZ0);
			dCurrPos[1] = COMI.Get_MotCurrentPos(m_nMotZ1);
			dCurrPos[2] = COMI.Get_MotCurrentPos(m_nMotZ2);	
			dCurrPos[3] = COMI.Get_MotCurrentPos(m_nMotZ3);
			m_nStep_Run = 300;
	
		break;

		//Lift Up
	case 300:
		if (m_nStandByOK[0] == CTL_NO)
		{
			if (dCurrPos[0] > st_motor[m_nMotZ0].d_pos[Z_ACC_LIFT_SAFETY_] + 10)
			{
				// WithOut모드에서는 Offset 간격으로 UP
				//nRetData[0] = OnSupplyStandBy0(2, st_map.nLiftWorkSite);		// Rear -> Front
				//kwlee 2017.0825
				nRetData[0] = OnSupplyStandBy0(2, SIDE_LEFT_);		// Rear -> Front
			}
			else
			{
				// UP Until Sensor is Sensing
				// 0: Accy supply req, 1:safety pos move, 2: + Offset move
				//nRetData[0] = OnSupplyStandBy0(0, st_map.nLiftWorkSite);		// Rear -> Front
				//kwlee 2017.0825
				nRetData[0] = OnSupplyStandBy0(0, SIDE_LEFT_ );		// Rear -> Front
			}

			if (nRetData[0] == CTLBD_RET_GOOD)	
			{
				m_nStandByOK[0] = CTL_YES;
			}
// 			else if (nRetData[0] == CTLBD_RET_SKIP)
// 			{
// 				m_nStandByOK[0] = CTL_END;
// 			}
			//kwlee 2017 0913
			else if (nRetData[0] == CTLBD_RET_NO_ACC)
			{
				// AUAc°¡ °¨AoμCAo ¾E¾Æ DOWN A§A¡·I AIμ¿½AAN¾ß CO A¶°C
				m_nStandByOK[0] = CTL_DVC_SKIP;
			}
		}
		//kwlee 2017.0831
		else if (m_nStandByOK[0] == CTL_DVC_SKIP)
		{
			nRetData[0] = OnSupplyStandBy0(1, SIDE_LEFT_ );		// Rear -> Front

			if (nRetData[0] == CTLBD_RET_SKIP)
			{
				dCurrPos[0] = COMI.Get_MotCurrentPos(m_nMotZ0);
				if (dCurrPos[0] < st_motor[m_nMotZ0].d_pos[Z_ACC_LIFT_READY_])
				{
					m_nStandByOK[0] = CTL_END;
				}
			}
		}

		if (m_nStandByOK[1] == CTL_NO)
		{
			if (dCurrPos[1] > st_motor[m_nMotZ1].d_pos[Z_ACC_LIFT_SAFETY_] + 10)
			{
				//nRetData[1] = OnSupplyStandBy1(2, st_map.nLiftWorkSite);
				//kwlee 2017.0825
				nRetData[1] = OnSupplyStandBy1(2, SIDE_RIGHT_);
			}
			else
			{
				//nRetData[1] = OnSupplyStandBy1(0, st_map.nLiftWorkSite);
				//kwlee 2017.0825
				nRetData[1] = OnSupplyStandBy1(0, SIDE_RIGHT_);
			}
			if (nRetData[1] == CTLBD_RET_GOOD)	
			{
				m_nStandByOK[1] = CTL_YES;
			}
		
// 			else if (nRetData[1] == CTLBD_RET_SKIP)	
// 			{
// 				m_nStandByOK[1] = CTL_END;
// 			}
			else if (nRetData[1] == CTLBD_RET_NO_ACC)
			{
				m_nStandByOK[1] = CTL_DVC_SKIP;
			}
		}
		//kwlee 2017.0831
		else if (m_nStandByOK[1] == CTL_DVC_SKIP)
		{
			nRetData[1] = OnSupplyStandBy1(1, SIDE_RIGHT_);

			if (nRetData[1] == CTLBD_RET_SKIP)	
			{
				dCurrPos[1] = COMI.Get_MotCurrentPos(m_nMotZ1);

				if (dCurrPos[1] < st_motor[m_nMotZ1].d_pos[Z_ACC_LIFT_READY_])
				{
					m_nStandByOK[1] = CTL_END;
				}
				
			}
		}

		if (m_nStandByOK[2] == CTL_NO)
		{
			if (dCurrPos[2] > st_motor[m_nMotZ2].d_pos[Z_ACC_LIFT_SAFETY_] + 10)
			{
				//nRetData[2] = OnSupplyStandBy2(2, st_map.nLiftWorkSite);
				//kwlee 2017.825
				nRetData[2] = OnSupplyStandBy2(2, SIDE_LEFT_);
			}
			else
			{
				// UP Until Sensor is Sensing
				//nRetData[2] = OnSupplyStandBy2(0, st_map.nLiftWorkSite);
				//kwlee 2017.0825
				nRetData[2] = OnSupplyStandBy2(0, SIDE_LEFT_);
			}
			if (nRetData[2] == CTLBD_RET_GOOD)	
			{
				m_nStandByOK[2] = CTL_YES;
			}
// 			else if (nRetData[2] == CTLBD_RET_SKIP)	
// 			{
// 				//m_nStandByOK[2] = CTL_DVC_SKIP;
// 				//kwlee 2017.0831
// 				m_nStandByOK[2] = CTL_END;
// 			}
			//kwlee 20170913
			else if (nRetData[2] == CTLBD_RET_NO_ACC)
			{
				m_nStandByOK[2] = CTL_DVC_SKIP;
			}
		}
		//kwlee 2017.0831
		else if (m_nStandByOK[2] == CTL_DVC_SKIP)
		{
			nRetData[2] = OnSupplyStandBy2(1, SIDE_LEFT_);

			if (nRetData[2] == CTLBD_RET_SKIP)	
			{
				//m_nStandByOK[2] = CTL_DVC_SKIP;
				//kwlee 2017.0831
				dCurrPos[2] = COMI.Get_MotCurrentPos(m_nMotZ2);
				
				if (dCurrPos[2] < st_motor[m_nMotZ2].d_pos[Z_ACC_LIFT_READY_])
				{
					m_nStandByOK[2] = CTL_END;
				}
			}			
		}

		if (m_nStandByOK[3] == CTL_NO)
		{
			if(dCurrPos[3] > st_motor[m_nMotZ3].d_pos[Z_ACC_LIFT_SAFETY_] + 10)
			{
				//nRetData[3] = OnSupplyStandBy3(2, st_map.nLiftWorkSite);
				//kwlee 2017.0825
				nRetData[3] = OnSupplyStandBy3(2, SIDE_RIGHT_);
			}
			else
			{
				// UP Until Sensor is Sensing
				//nRetData[3] = OnSupplyStandBy3(0, st_map.nLiftWorkSite);
				//kwle 2017.0825
				nRetData[3] = OnSupplyStandBy3(0, SIDE_RIGHT_);
			}
			if (nRetData[3] == CTLBD_RET_GOOD)	
			{
				m_nStandByOK[3] = CTL_YES;
			}
			//kwlee 2017.0831
// 			else if (nRetData[3] == CTLBD_RET_SKIP)	
// 			{
// 				m_nStandByOK[3] = CTL_END;
// 			}
			//kwlee 2017.0913
			else if (nRetData[3] == CTLBD_RET_NO_ACC)
			{
				m_nStandByOK[3] = CTL_DVC_SKIP;
				
			}
		}
		//kwlee 2017.0831
		else if (m_nStandByOK[3] == CTL_DVC_SKIP)
		{
			nRetData[3] = OnSupplyStandBy3(1, SIDE_RIGHT_);

			if (nRetData[3] == CTLBD_RET_SKIP)	
			{
				dCurrPos[3] = COMI.Get_MotCurrentPos(m_nMotZ3);

				if (dCurrPos[3] < st_motor[m_nMotZ3].d_pos[Z_ACC_LIFT_READY_])
				{
					m_nStandByOK[3] = CTL_END;
				}
			}
		}

		// Lifter Z(0,1,2,3)에 대해서 모두 작업을 완료할때까지 기다린다
		// 2017.0910 추가 예정.
		//1. SenSor 미감지시 바로 다운 하지 말고Pitch 만� 올린후  로봇에 요청 하여 한번 집어 보자.
		//2. 로봇에 자재 감지 안되었을 경우Lift 내린다.
		if (m_nStandByOK[0] == CTL_NO || m_nStandByOK[1] == CTL_NO || m_nStandByOK[2] == CTL_NO || m_nStandByOK[3] == CTL_NO )
		{
			break;
		}

		nRetData[0] = !g_ioMgr.get_in_bit(stIO.i_Chk_BoxClampAccyDetection[SIDE_LEFT_][ SIDE_FW_ + m_WorkSite * 2 ]);
		nRetData[1] = !g_ioMgr.get_in_bit(stIO.i_Chk_BoxClampAccyDetection[SIDE_RIGHT_][SIDE_FW_ + m_WorkSite * 2]);
		nRetData[2] = !g_ioMgr.get_in_bit(stIO.i_Chk_BoxClampAccyDetection[SIDE_LEFT_][SIDE_BW_ + m_WorkSite * 2]);
		nRetData[3] = !g_ioMgr.get_in_bit(stIO.i_Chk_BoxClampAccyDetection[SIDE_RIGHT_][SIDE_BW_ + m_WorkSite * 2]);

		//  Did it complete to supply all DVC in Lifter Z(0,1,2,3)?
		if (m_nStandByOK[0] == CTL_YES && m_nStandByOK[1] == CTL_YES &&	m_nStandByOK[2] == CTL_YES && m_nStandByOK[3] == CTL_YES )
		{
			// [XYZ Robot <- BoxLifter] Complete send.	
			/*stSync.nReq_Lifter2XYZRbt_Work = SYNC_REQ_ACC_LIFT_COMPLETE_;*/
			//kwlee 2017.0830
// 			nRetData[0] = !g_ioMgr.get_in_bit(stIO.i_Chk_BoxClampAccyDetection[SIDE_LEFT_][ SIDE_FW_ + m_WorkSite * 2 ]);
// 			nRetData[1] = !g_ioMgr.get_in_bit(stIO.i_Chk_BoxClampAccyDetection[SIDE_RIGHT_][SIDE_FW_ + m_WorkSite * 2]);
// 			nRetData[2] = !g_ioMgr.get_in_bit(stIO.i_Chk_BoxClampAccyDetection[SIDE_LEFT_][SIDE_BW_ + m_WorkSite * 2]);
// 			nRetData[3] = !g_ioMgr.get_in_bit(stIO.i_Chk_BoxClampAccyDetection[SIDE_RIGHT_][SIDE_BW_ + m_WorkSite * 2]);

			if (nRetData[0] == IO_OFF || nRetData[1] == IO_OFF || nRetData[2] == IO_OFF || nRetData[3] == IO_OFF)
			{
				m_nStep_Run = 200;
			}
			else 
			{
				dCurrPos[0] = COMI.Get_MotCurrentPos(m_nMotZ0);
				dCurrPos[1] = COMI.Get_MotCurrentPos(m_nMotZ1);
				dCurrPos[2] = COMI.Get_MotCurrentPos(m_nMotZ2);	
				dCurrPos[3] = COMI.Get_MotCurrentPos(m_nMotZ3);

				if (dCurrPos[0] > st_motor[m_nMotZ0].d_pos[Z_ACC_LIFT_READY_] &&  dCurrPos[1] > st_motor[m_nMotZ1].d_pos[Z_ACC_LIFT_READY_] &&
					dCurrPos[2] > st_motor[m_nMotZ2].d_pos[Z_ACC_LIFT_READY_] &&  dCurrPos[3] > st_motor[m_nMotZ3].d_pos[Z_ACC_LIFT_READY_])
				{
					//Lift Device Up 준비 완료.
					stSync.nReq_Lifter2XYZRbt_Work = SYNC_REQ_ACC_LIFT_COMPLETE_;
					m_nStep_Run = 1000;
				}
				else
				{

					if (dCurrPos[0] <= st_motor[m_nMotZ0].d_pos[Z_ACC_LIFT_READY_])
					{
						// 002000 0 00 "LIFTER_UP_DOWN_SHIFT1_LOAD_ACCY_CHK_ERR."
						alarm.mstr_code		= "002000";
						CTL_Lib.Alarm_Error_Occurrence(2410, CTL_dWARNING, alarm.mstr_code);
					}
					else if (dCurrPos[1] <= st_motor[m_nMotZ1].d_pos[Z_ACC_LIFT_READY_])
					{
						// 012000 0 00 "LIFTER_UP_DOWN_SHIFT2_LOAD_ACCY_CHK_ERR."
						alarm.mstr_code		= "012000";
						CTL_Lib.Alarm_Error_Occurrence(2410, CTL_dWARNING, alarm.mstr_code);
					}
					else if (dCurrPos[2] <= st_motor[m_nMotZ2].d_pos[Z_ACC_LIFT_READY_])
					{
						// 022000 0 00 "LIFTER_UP_DOWN_SHIFT3_LOAD_ACCY_CHK_ERR."
						alarm.mstr_code		= "022000";
						CTL_Lib.Alarm_Error_Occurrence(2410, CTL_dWARNING, alarm.mstr_code);
					}
					else if (dCurrPos[3] <= st_motor[m_nMotZ3].d_pos[Z_ACC_LIFT_READY_])
					{
						// 032000 0 00 "LIFTER_UP_DOWN_SHIFT4_LOAD_ACCY_CHK_ERR."
						alarm.mstr_code		= "032000";
						CTL_Lib.Alarm_Error_Occurrence(2410, CTL_dWARNING, alarm.mstr_code);
					}
				}
			}
		}
		//kwlee 2017.08
		else if (m_nStandByOK[0] == CTL_DVC_SKIP && m_nStandByOK[1] == CTL_DVC_SKIP &&	m_nStandByOK[2] == CTL_DVC_SKIP && m_nStandByOK[3] == CTL_DVC_SKIP )
		{
// 			nRetData[0] = !g_ioMgr.get_in_bit(stIO.i_Chk_BoxClampAccyDetection[SIDE_LEFT_][m_WorkSite + SIDE_FW_]);
// 			nRetData[1] = !g_ioMgr.get_in_bit(stIO.i_Chk_BoxClampAccyDetection[SIDE_RIGHT_][m_WorkSite + SIDE_FW_]);
// 			nRetData[2] = !g_ioMgr.get_in_bit(stIO.i_Chk_BoxClampAccyDetection[SIDE_LEFT_][m_WorkSite + SIDE_BW_]);
// 			nRetData[3] = !g_ioMgr.get_in_bit(stIO.i_Chk_BoxClampAccyDetection[SIDE_RIGHT_][m_WorkSite + SIDE_BW_]);
			
			if (nRetData[0] == IO_OFF && nRetData[1] == IO_OFF && nRetData[2] == IO_OFF && nRetData[3] == IO_OFF)
			{
				stSync.nReq_Lifter2XYZRbt_Work = SYNC_REQ_RESET_; //kwlee 2017.0901
				m_nStep_Run = 300;
			}
// 			else if (nRetData[0] == IO_ON || nRetData[1] == IO_ON || nRetData[2] == IO_ON || nRetData[3] == IO_ON)
// 			{
// 				m_nStep_Run = 200;
// 			}
		}
		//모두 내려와야 Lift 동작 한다.
		else if (m_nStandByOK[0] == CTL_END && m_nStandByOK[1] == CTL_END && m_nStandByOK[2] == CTL_END && m_nStandByOK[3] == CTL_END )
		{
			m_WorkSite++;
			stSync.nReq_Lifter2XYZRbt_Work = SYNC_REQ_RESET_; //kwlee 2017.0901
			if (m_WorkSite < 3)
			{
				m_nStep_Run = 100;
			}
			else
			{
				//배출..
				m_WorkSite = SITE_REAR;
				m_nStep_Run = 1100;
			}
		}
		//kwlee 2017.09
		else
		{
			nCnt = 0;
			for (i = 0; i<MAX_PICKER_; i++)
			{
				if (nRetData[i] == IO_ON)
				{
					nCnt++;
				}
			}

			if (nCnt > 0)
			{
				if (dCurrPos[0] <= st_motor[m_nMotZ0].d_pos[Z_ACC_LIFT_READY_])
				{
					// 002000 0 00 "LIFTER_UP_DOWN_SHIFT1_LOAD_ACCY_CHK_ERR."
					alarm.mstr_code		= "002000";
					CTL_Lib.Alarm_Error_Occurrence(2410, CTL_dWARNING, alarm.mstr_code);
				}
				else if (dCurrPos[1] <= st_motor[m_nMotZ1].d_pos[Z_ACC_LIFT_READY_])
				{
					// 012000 0 00 "LIFTER_UP_DOWN_SHIFT2_LOAD_ACCY_CHK_ERR."
					alarm.mstr_code		= "012000";
					CTL_Lib.Alarm_Error_Occurrence(2410, CTL_dWARNING, alarm.mstr_code);
				}
				else if (dCurrPos[2] <= st_motor[m_nMotZ2].d_pos[Z_ACC_LIFT_READY_])
				{
					// 022000 0 00 "LIFTER_UP_DOWN_SHIFT3_LOAD_ACCY_CHK_ERR."
					alarm.mstr_code		= "022000";
					CTL_Lib.Alarm_Error_Occurrence(2410, CTL_dWARNING, alarm.mstr_code);
				}
				else if (dCurrPos[3] <= st_motor[m_nMotZ3].d_pos[Z_ACC_LIFT_READY_])
				{
					// 032000 0 00 "LIFTER_UP_DOWN_SHIFT4_LOAD_ACCY_CHK_ERR."
					alarm.mstr_code		= "032000";
					CTL_Lib.Alarm_Error_Occurrence(2410, CTL_dWARNING, alarm.mstr_code);
				}
				m_nStep_Run = 200;
			}
		}
		//else// Lifter Z(0,1,2,3)에 대해서 모든 리프터가 정상적으로 자재 공급을 하지 못하였기 때문에 Down Step 진행
// 		else 
// 		{
// 			//Lift Down
// 			m_nStep_Run = 2600;
// 		}
		break;

	case 1000:
		//if (stSync.nReq_Lifter2XYZRbt_Work == SYNC_REQ_ACC_LIFT_COMPLETE_)
		//kwlee 2017.0825
		if (stSync.nResp_XYZRbt2Lifter_Work == SYNC_RESP_WORK_COMPLETE_)
		{
			//m_nStep_Run = 1100; //kwlee 2017.0825
			//stSync.nReq_Lifter2XYZRbt_Work = SYNC_REQ_RESET_; //kwlee 2017.0825
			m_nStep_Run = 200; //kwlee 2017.0825
		}
		break;
	
	case 1100:
		//안전한 대기 위치로 빠진다.
		nRet_1 = CTL_Lib.OnSingleMove( m_nMotY, st_motor[m_nMotY].d_pos[Y_ACC_BOX_AVOID_], (int)st_handler.md_run_speed );
		if( nRet_1 == CTLBD_RET_GOOD )
		{
			m_nStep_Run = 1200;
		}
		else if( nRet_1 == CTLBD_RET_ERROR || nRet_1 == CTLBD_RET_RETRY )
		{
			//041002 0 00 "LIFTER_Y1_X_ACC_BOX_AVOID_MOVE_ERR."
			alarm.mstr_code		= "041002";
			CTL_Lib.Alarm_Error_Occurrence( 2005, CTL_dWARNING, alarm.mstr_code);
		}
		break;

	//Box Lift -> ox Clamp Cylinder Off
	case 1200:
		stSync.nResp_Lifter2Clamp_Work = SYNC_RESP_WORK_COMPLETE_;
		m_nStep_Run = 1300;
		break;

		//Box Clamp Clamp 해제하고 Lift 까지 Down
	case 1300:
		if (stSync.nReq_Clamp2Lifter_Work == SYNC_REQ_WORK_)
		{
			m_nStep_Run = 1400;
		}
		break;

	case 1400:
		nCnt = 0;
		for (i =0; i<3; i++)
		{
			nRet[i] = !g_ioMgr.get_in_bit(stIO.i_Chk_UnloaderFull[i],IO_OFF);

			if (nRet[i] == IO_ON) 
			{
				nCnt++;
			}
		}

		if (nCnt > 0)
		{
			m_nStep_Run = 1500;
		}
		else
		{
			//150000 0 00 "ACCY_UNLOADER_FULL_ERR."
			alarm.mstr_code		= "150000";
			CTL_Lib.Alarm_Error_Occurrence( 2006, CTL_dWARNING, "150000");
		}
		break;

		//Box Out Conv로 민다.
	case 1500:
		//nRet_1 = CTL_Lib.OnSingleMove( m_nMotY, st_motor[m_nMotY].d_pos[Y_ACC_FRONT_], (int)st_handler.md_run_speed );
		//kwlee 2017.0823
		nRet_1 = CTL_Lib.OnSingleMove( m_nMotY, st_motor[m_nMotY].d_pos[Y_ACC_REAR_], (int)st_handler.md_run_speed );
		if( nRet_1 == CTLBD_RET_GOOD )
		{
			m_nStep_Run = 1600;
		}
		else if( nRet_1 == CTLBD_RET_ERROR || nRet_1 == CTLBD_RET_RETRY )
		{
			//041002 0 00 "LIFTER_Y1_X_ACC_BOX_AVOID_MOVE_ERR."
			alarm.mstr_code		= "041002";
			CTL_Lib.Alarm_Error_Occurrence( 2008, CTL_dWARNING, alarm.mstr_code);
		}
		break;

		//Y Robot Avoid 위치 이동 하였다. Lift 올리자.
	case 1600:
		nRet_1 = CTL_Lib.OnSingleMove( m_nMotY, st_motor[m_nMotY].d_pos[Y_ACC_BOX_AVOID_], (int)st_handler.md_run_speed );
		if( nRet_1 == CTLBD_RET_GOOD )
		{
			stSync.nResp_Lifter2Clamp_Work = SYNC_RESP_BOX_REMOVE_COMPLETE;
			m_nStep_Run = 1700;
		}
		else if( nRet_1 == CTLBD_RET_ERROR || nRet_1 == CTLBD_RET_RETRY )
		{
			//041002 0 00 "LIFTER_Y1_X_ACC_BOX_AVOID_MOVE_ERR."
			alarm.mstr_code		= "041002";
			CTL_Lib.Alarm_Error_Occurrence( 2009, CTL_dWARNING, alarm.mstr_code);
		}
		break;

	case 1700:
		nCnt =0;
		for (i =0; i<3; i++)
		{
			nRet[i] = !g_ioMgr.get_in_bit(stIO.i_Chk_UnloaderFull[i],IO_OFF);
			
			if (nRet[i] == IO_OFF)
			{
				nCnt++;
			}
		}

		if (nCnt > 0)
		{
			m_nStep_Run = 1800;
		}
		else
		{
			//150000 0 00 "ACCY_UNLOADER_FULL_ERR."
			CTL_Lib.Alarm_Error_Occurrence( 2010, CTL_dWARNING, "150000");
		}
		break;
	//Avoid 위치로 빠진 상태. Lift Up 상태 확인  Elevator Rear 위치 이동 
	case 1800:
		if (stSync.nReq_Clamp2Lifter_Work == SYNC_REQ_RESET_)
		{
			m_nStep_Run = 2000;
		}
		break;

		//작업 위치 이동.
	case 2000:
		//nRet_1 = CTL_Lib.OnSingleMove( m_nMotY, st_motor[m_nMotY].d_pos[Y_ACC_FRONT_], (int)st_handler.md_run_speed );
		//kwlee 2017.0823
		nRet_1 = CTL_Lib.OnSingleMove( m_nMotY, st_motor[m_nMotY].d_pos[Y_ACC_REAR_], (int)st_handler.md_run_speed );
		if( nRet_1 == CTLBD_RET_GOOD )
		{
				m_nStep_Run = 3000;
		}
		else if( nRet_1 == CTLBD_RET_ERROR || nRet_1 == CTLBD_RET_RETRY )
		{
			//042000 0 00 "LIFTER_Y1_WORK_AREA_MOVE_ERR."
			alarm.mstr_code		= "042000";
			CTL_Lib.Alarm_Error_Occurrence( 2011, CTL_dWARNING, alarm.mstr_code);
		}
		break;


	case 3000:
		stSync.nResp_Lifter2Clamp_Work = SYNC_REQ_RESET_;
		m_nStep_Run = 0;
		break;
	}
}

bool CSeq_BoxLifter::OnRun_Remove() 
{
	if (Func.OnIsAllRcvyComplete() != CTL_YES)
	{
		return false;
	}

	bool bRet = true;
	int nRet, nReadIO[2] = {0,};
	
	Func.OnTrace_ThreadStep(3, m_nStep_Remove);	
	
	switch(m_nStep_Remove)
	{
	case 0:	
		// [BoxClamp -> BoxLifter]
		if(stSync.nReq_Clamp2Lifter_Work == SYNC_REQ_BOX_REMOVE_)
		{
			m_nStep_Remove = 50;
			bRet = false;
		}
		break;
		
	case 50:
		bRet = false;
		nReadIO[0] = !g_ioMgr.get_in_bit(stIO.i_Chk_UnloaderFull[0]);
		nReadIO[1] = !g_ioMgr.get_in_bit(stIO.i_Chk_UnloaderFull[1]);
		if (nReadIO[0] == IO_ON && nReadIO[1] == IO_ON)
		{
			// 150000 0 00 "PS1200_PS1201_ACCY_UNLOADER_FULL_ERR."
			alarm.mstr_code		= "150000";
			CTL_Lib.Alarm_Error_Occurrence( 2012, CTL_dWARNING, alarm.mstr_code);
		}
		else
		{
			m_nStep_Remove = 100;
		}
		break;
		
	case 100:
		// Lifter X축을 첫번째 공급 영역으로 이동 하여 Box 제거
		nRet = CTL_Lib.OnSingleMove(m_nMotY, st_motor[m_nMotY].d_pos[Y_ACC_REAR_], (int)st_handler.md_run_speed);
		if (nRet == CTLBD_RET_GOOD)
		{
			stSync.nResp_Lifter2Clamp_Work = SYNC_RESP_BOX_REMOVE_COMPLETE;
			m_nStep_Remove = 200;
			bRet = false;
		}
		else if (nRet == CTLBD_RET_ERROR || nRet == CTLBD_RET_RETRY)
		{
			// 041001 0 00 "LIFTER_Y_ACC_REAR_MOVE_ERR."
			alarm.mstr_code		= "041001";
			CTL_Lib.Alarm_Error_Occurrence( 2013, CTL_dWARNING, alarm.mstr_code);
		}
		break;
		
	case 200:
		if (stSync.nReq_Clamp2Lifter_Work == SYNC_REQ_WORK_COMPLETE_)
		{
			stSync.nResp_Lifter2Clamp_Work = SYNC_RESP_RESET_;
			m_nStep_Remove = 0;
		}
		break;
	}
	return bRet;
}

int CSeq_BoxLifter::OnRun_ForceDischarge() 
{
	int nFuncRet = true;
	int nRet;
	int nRetData[4] = {0,};

	Func.OnTrace_ThreadStep(12, m_nStep_ForceDischarge);	// 쓰레드 스텝 정보 TRACE

	// Box 배출 대기
	if(st_work.nForceDischarge != FORCEDISCHARGE_ON)
	{
		return true;
	}
	
	switch(m_nStep_ForceDischarge)
	{
	case 0:
		if(stSync.nForceDischargeComplete[SITE_BOX_LIFT_] != CTL_YES)
		{
			stSync.nResp_Lifter2Clamp_Work = SYNC_RESP_WORK_;
			m_nStep_ForceDischarge = 100;
		}
		break;

	case 100:
		// Accy Box 배출을 위해 Z축 안전위치로 이동
		nRetData[0] = CTL_Lib.OnSingleMove(m_nMotZ0, st_motor[m_nMotZ0].d_pos[Z_ACC_LIFT_SAFETY_], (int)st_handler.md_run_speed * 3);
		nRetData[1] = CTL_Lib.OnSingleMove(m_nMotZ1, st_motor[m_nMotZ1].d_pos[Z_ACC_LIFT_SAFETY_], (int)st_handler.md_run_speed * 3);
		nRetData[2] = CTL_Lib.OnSingleMove(m_nMotZ2, st_motor[m_nMotZ2].d_pos[Z_ACC_LIFT_SAFETY_], (int)st_handler.md_run_speed * 3);
		nRetData[3] = CTL_Lib.OnSingleMove(m_nMotZ3, st_motor[m_nMotZ3].d_pos[Z_ACC_LIFT_SAFETY_], (int)st_handler.md_run_speed * 3);
		if (nRetData[0] == CTLBD_RET_GOOD && nRetData[1] == CTLBD_RET_GOOD &&
			nRetData[2] == CTLBD_RET_GOOD && nRetData[3] == CTLBD_RET_GOOD)	
		{
			m_nStep_ForceDischarge = 300;
		}
		else if (nRetData[0] == CTLBD_RET_ERROR || nRetData[0] == CTLBD_RET_SAFETY)
		{
			// 001001 0 00 "LIFTER_UP_DOWN_SHIFT1_Z_ACC_LIFT_SAFETY_MOVE_ERR."
			alarm.mstr_code		= "001001";
			CTL_Lib.Alarm_Error_Occurrence( 2306, CTL_dWARNING, alarm.mstr_code );
		}
		else if (nRetData[1] == CTLBD_RET_ERROR || nRetData[1] == CTLBD_RET_SAFETY)
		{
			// 011001 0 00 "LIFTER_UP_DOWN_SHIFT2_Z_ACC_LIFT_SAFETY_MOVE_ERR."
			alarm.mstr_code		= "011001";
			CTL_Lib.Alarm_Error_Occurrence( 2306, CTL_dWARNING, alarm.mstr_code );
		}
		else if (nRetData[2] == CTLBD_RET_ERROR || nRetData[2] == CTLBD_RET_SAFETY)
		{
			// 021001 0 00 "LIFTER_UP_DOWN_SHIFT3_Z_ACC_LIFT_SAFETY_MOVE_ERR."
			alarm.mstr_code		= "021001";
			CTL_Lib.Alarm_Error_Occurrence( 2306, CTL_dWARNING, alarm.mstr_code );
		}
		else if (nRetData[3] == CTLBD_RET_ERROR || nRetData[3] == CTLBD_RET_SAFETY)
		{
			// 031001 0 00 "LIFTER_UP_DOWN_SHIFT4_Z_ACC_LIFT_SAFETY_MOVE_ERR."
			alarm.mstr_code		= "031001";
			CTL_Lib.Alarm_Error_Occurrence( 2306, CTL_dWARNING, alarm.mstr_code );
		}
		break;

	case 300:
		// Accy Box 배출을 위해 Lift 유닛을 회피 구간으로 이동
		nRet = CTL_Lib.OnSingleMove(m_nMotY, st_motor[m_nMotY].d_pos[Y_ACC_BOX_AVOID_], (int)st_handler.md_run_speed);
		if (nRet == CTLBD_RET_GOOD)
		{
			stSync.nResp_Lifter2Clamp_Work = SYNC_RESP_BOX_REMOVE_READY;
			m_nStep_ForceDischarge = 400;
		}
		else if (nRet == CTLBD_RET_ERROR || nRet == CTLBD_RET_RETRY)
		{
			//  041002 0 00 "LIFTER_Y_X_ACC_BOX_AVOID_MOVE_ERR."
			alarm.mstr_code		= "041002";
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
		}
		break;

	case 400:
		// [BoxClamp -> BoxLifter] 응답 초기화 대기
		if(stSync.nReq_Clamp2Lifter_Work == SYNC_REQ_RESET_)
		{
			m_nStep_ForceDischarge = 500;
		}
		break;

	case 500:
		// Accy Box 배출을 위해 Lift 유닛을 회피 구간으로 이동
		nRet = CTL_Lib.OnSingleMove(m_nMotY, st_motor[m_nMotY].d_pos[Y_ACC_REAR_], (int)st_handler.md_run_speed);
		if (nRet == CTLBD_RET_GOOD)
		{
			m_nStep_ForceDischarge = 600;
		}
		else if (nRet == CTLBD_RET_ERROR || nRet == CTLBD_RET_RETRY)
		{
			//  041002 0 00 "LIFTER_Y_X_ACC_BOX_AVOID_MOVE_ERR."
			alarm.mstr_code		= "041002";
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
		}
		break;

	case 600:
		m_nStep_Run = 0;
		m_nStep_ForceDischarge = 0;
		stSync.nForceDischargeComplete[SITE_BOX_LIFT_] = CTL_YES;
		stSync.nReq_Lifter2XYZRbt_Work = SYNC_REQ_RESET_;
// 		stSync.nResp_Lifter2Clamp_Work = SYNC_RESP_RESET_;
		//2015.0602
// 		if(SeqBoxClamp.m_nStep_Run <= 4260)
		if(SeqBoxClamp.m_nStep_Run <= 4800)
			stSync.nResp_Lifter2Clamp_Work = SYNC_RESP_BOX_REMOVE_COMPLETE;
		else
			stSync.nResp_Lifter2Clamp_Work = SYNC_RESP_RESET_;
		st_map.nLiftWorkSite = SIDE_REAR_;  				// Accy 작업 영역 설정 (작업 순서 : Rear -> Front)
		break;
	}

	return nFuncRet;
}

int CSeq_BoxLifter::OnRcvy_RobotPos()
{
	int nFuncRet = CTL_PROCEED;

	/*

	int nRet, nResp;
	int i, nRcvyCount;

	switch(m_nStep_Rcvy)
	{
	case 0:
		nRet[0] = COMI.Check_MotPosRange(m_nMotZ0, m_dPos_TmpRcvyZ[0], st_motor[m_nMotZ0].mn_allow);
		nRet[1] = COMI.Check_MotPosRange(m_nMotZ1, m_dPos_TmpRcvyZ[1], st_motor[m_nMotZ1].mn_allow);
		nRet[2] = COMI.Check_MotPosRange(m_nMotZ2, m_dPos_TmpRcvyZ[2], st_motor[m_nMotZ2].mn_allow);
		nRet[3] = COMI.Check_MotPosRange(m_nMotZ3, m_dPos_TmpRcvyZ[3], st_motor[m_nMotZ3].mn_allow);
		nRet[4] = COMI.Check_MotPosRange(m_nMotY, COMI.md_cmdpos_backup[m_nMotY], st_motor[m_nMotY].mn_allow);
		
		nRcvyCount = 0;	// [리셋] 복구할 모터 갯수 정보
		for(i=0; i<5; i++)
		{
			m_nReq_Rcvy[i] = CTL_NO;
			// 모터 위치가 변경됨 [복구 동작 필요]
			if (nRet[i] == BD_ERROR)
			{
				// yangkyu 2015.02.11
				if (i == 4)
				{
					m_nReq_Rcvy[i] = CTL_YES;
					nRcvyCount++;
				}
			}
		}
		// 복구 동작 불필요
		if (nRcvyCount == 0)
		{
			m_nStep_Rcvy = 10000;
		}
		// 복구 동작 필요
		else
		{
			m_nStep_Rcvy = 100;
		}
		break;

	case 100:
		// Z축 0모터 안전 위치로 이동 [충돌 방지]
		nResp = COMI.Start_SingleMove(m_nMotZ0, st_motor[m_nMotZ0].d_pos[Z_ACC_LIFT_SAFETY_], (int)st_handler.md_run_speed * 2);
		if (nResp == CTLBD_RET_GOOD)
		{
			m_nStep_Rcvy = 110;
		}
		else if (nResp == CTLBD_RET_ERROR || nResp == CTLBD_RET_SAFETY)
		{
			// 001001 0 00 "LIFTER_UP_DOWN_SHIFT1_Z_ACC_LIFT_SAFETY_MOVE_ERR."
			alarm.mstr_code		= "001001";
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
		}
		break;

	case 110:
		nResp = COMI.Check_SingleMove(m_nMotZ0, st_motor[m_nMotZ0].d_pos[Z_ACC_LIFT_SAFETY_]);
		if (nResp == CTLBD_RET_GOOD)
		{
			m_nStep_Rcvy = 200;
		}
		else if (nResp == CTLBD_RET_ERROR || nResp == CTLBD_RET_SAFETY)
		{
			m_nStep_Rcvy = 100;

			// 001001 0 00 "LIFTER_UP_DOWN_SHIFT1_Z_ACC_LIFT_SAFETY_MOVE_ERR."
			alarm.mstr_code		= "001001";
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
		}
		else if (nResp == CTLBD_RET_RETRY)
		{
			m_nStep_Rcvy = 100;
		}
		break;

	case 200:
		// Z축 1모터 안전 위치로 이동 [충돌 방지]
		nResp = COMI.Start_SingleMove(m_nMotZ1, st_motor[m_nMotZ1].d_pos[Z_ACC_LIFT_SAFETY_], (int)st_handler.md_run_speed * 2);
		if (nResp == CTLBD_RET_GOOD)
		{
			m_nStep_Rcvy = 210;
		}
		else if (nResp == CTLBD_RET_ERROR || nResp == CTLBD_RET_SAFETY)
		{
			// 011001 0 00 "LIFTER_UP_DOWN_SHIFT2_Z_ACC_LIFT_SAFETY_MOVE_ERR."
			alarm.mstr_code		= "011001";
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
		}
		break;

	case 210:
		nResp = COMI.Check_SingleMove(m_nMotZ1, st_motor[m_nMotZ1].d_pos[Z_ACC_LIFT_SAFETY_]);
		if (nResp == CTLBD_RET_GOOD)
		{
			m_nStep_Rcvy = 300;
		}
		else if (nResp == CTLBD_RET_ERROR || nResp == CTLBD_RET_SAFETY)
		{
			m_nStep_Rcvy = 200;

			// 011001 0 00 "LIFTER_UP_DOWN_SHIFT2_Z_ACC_LIFT_SAFETY_MOVE_ERR."
			alarm.mstr_code		= "011001";
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
		}
		else if (nResp == CTLBD_RET_RETRY)
		{
			m_nStep_Rcvy = 200;
		}
		break;

	case 300:
		// Z축 2모터 안전 위치로 이동 [충돌 방지]
		nResp = COMI.Start_SingleMove(m_nMotZ2, st_motor[m_nMotZ2].d_pos[Z_ACC_LIFT_SAFETY_], (int)st_handler.md_run_speed * 2);
		if (nResp == CTLBD_RET_GOOD)
		{
			m_nStep_Rcvy = 310;
		}
		else if (nResp == CTLBD_RET_ERROR || nResp == CTLBD_RET_SAFETY)
		{
			// 021001 0 00 "LIFTER_UP_DOWN_SHIFT3_Z_ACC_LIFT_SAFETY_MOVE_ERR."
			alarm.mstr_code		= "021001";
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
		}
		break;

	case 310:
		nResp = COMI.Check_SingleMove(m_nMotZ2, st_motor[m_nMotZ2].d_pos[Z_ACC_LIFT_SAFETY_]);
		if (nResp == CTLBD_RET_GOOD)
		{
			m_nStep_Rcvy = 400;
		}
		else if (nResp == CTLBD_RET_ERROR || nResp == CTLBD_RET_SAFETY)
		{
			m_nStep_Rcvy = 300;

			// 021001 0 00 "LIFTER_UP_DOWN_SHIFT3_Z_ACC_LIFT_SAFETY_MOVE_ERR."
			alarm.mstr_code		= "021001";
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
		}
		else if (nResp == CTLBD_RET_RETRY)
		{
			m_nStep_Rcvy = 300;
		}
		break;

	case 400:
		// Z축 3모터 안전 위치로 이동 [충돌 방지]
		nResp = COMI.Start_SingleMove(m_nMotZ3, st_motor[m_nMotZ3].d_pos[Z_ACC_LIFT_SAFETY_], (int)st_handler.md_run_speed * 2);
		if (nResp == CTLBD_RET_GOOD)
		{
			m_nStep_Rcvy = 410;
		}
		else if (nResp == CTLBD_RET_ERROR || nResp == CTLBD_RET_SAFETY)
		{
			// 031001 0 00 "LIFTER_UP_DOWN_SHIFT4_Z_ACC_LIFT_SAFETY_MOVE_ERR."
			alarm.mstr_code		= "031001";
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
		}
		break;

	case 410:
		nResp = COMI.Check_SingleMove(m_nMotZ3, st_motor[m_nMotZ3].d_pos[Z_ACC_LIFT_SAFETY_]);
		if (nResp == CTLBD_RET_GOOD)
		{
			m_nStep_Rcvy = 1000;
		}
		else if (nResp == CTLBD_RET_ERROR || nResp == CTLBD_RET_SAFETY)
		{
			m_nStep_Rcvy = 400;

			// 031001 0 00 "LIFTER_UP_DOWN_SHIFT4_Z_ACC_LIFT_SAFETY_MOVE_ERR."
			alarm.mstr_code		= "031001";
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
		}
		else if (nResp == CTLBD_RET_RETRY)
		{
			m_nStep_Rcvy = 400;
		}
		break;

	case 1000:
		if(m_nReq_Rcvy[4] == CTL_NO)
		{
			m_nStep_Rcvy = 5000;
			break;
		}

		// X축 모터 복구 동작
		nResp = COMI.Start_SingleMove(m_nMotY, COMI.md_cmdpos_backup[m_nMotY], (int)st_handler.md_run_speed);
		if (nResp == CTLBD_RET_GOOD)
		{
			m_nStep_Rcvy = 1010;
		}
		else if (nResp == CTLBD_RET_ERROR || nResp == CTLBD_RET_SAFETY)
		{
			// 040008 0 00 "LIFTER_Y_RECOVERY_POS_MOVE_ERR."
			alarm.mstr_code		= "040008";
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
		}
		break;

	case 1010:
		nResp = COMI.Check_SingleMove(m_nMotY, COMI.md_cmdpos_backup[m_nMotY]);
		if (nResp == CTLBD_RET_GOOD)
		{
			m_nStep_Rcvy = 5000;
		}
		else if (nResp == CTLBD_RET_ERROR || nResp == CTLBD_RET_SAFETY)
		{
			m_nStep_Rcvy = 1000;

			// 040008 0 00 "LIFTER_Y_RECOVERY_POS_MOVE_ERR."
			alarm.mstr_code		= "040008";
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
		}
		else if (nResp == CTLBD_RET_RETRY)
		{
			m_nStep_Rcvy = 1000;
		}
		break;

	case 5000:
		if (m_dPos_TmpRcvyZ[0] == st_motor[m_nMotZ0].d_pos[Z_ACC_LIFT_SAFETY_] &&
			m_dPos_TmpRcvyZ[1] == st_motor[m_nMotZ1].d_pos[Z_ACC_LIFT_SAFETY_] &&
			m_dPos_TmpRcvyZ[2] == st_motor[m_nMotZ2].d_pos[Z_ACC_LIFT_SAFETY_] &&
			m_dPos_TmpRcvyZ[3] == st_motor[m_nMotZ3].d_pos[Z_ACC_LIFT_SAFETY_])
		{
			m_nStep_Rcvy = 10000;
		}
		else
		{
			if
		}
		break;

	case 10000:
		nFuncRet = CTL_GOOD;
		m_nStep_Rcvy = 0;
		break;
	}
	*/
	nFuncRet = CTL_GOOD;
	return nFuncRet;
}

int CSeq_BoxLifter::OnSupplyInitial0() 
{
	int nFuncRet = CTLBD_RET_PROCEED;
	int nRet = VAR_INIT_;
	
	switch(m_nMotZInitStep[0])
	{
	case 0:
		nRet = COMI.HomeCheck_Mot(m_nMotZ0, 0, MOT_TIMEOUT);
		if (nRet == CTLBD_RET_GOOD)	
		{
			m_nMotZInitStep[0] = 5000;
		}
		else if (nRet == CTLBD_RET_ERROR)
		{
			//  000007 0 00 "LIFTER_UP_DOWN_SHIFT1_NOT_READY_HOME_CHK_ERR."
			alarm.mstr_code		= "000007";
			CTL_Lib.Alarm_Error_Occurrence( 2100, CTL_dWARNING, alarm.mstr_code );
		}
		break;
		
	case 5000:
		nFuncRet = CTLBD_RET_GOOD;  // HOMING 완료
		m_nMotZInitStep[0] = 0;
		break;
	}
	return nFuncRet;
}

//==================================================================//
// Lift 유닉 Z0 대기 위치로 이동
// nFuncRet : CTLBD_RET_PROCEED, CTLBD_RET_SKIP, CTLBD_RET_GOOD
// - int nzMode : [0:악세사리 공급 요청, 1:안전 위치 이동, 2:Offset 만큼 이동]
// - int nzSite : 작업 영역
//==================================================================//
int CSeq_BoxLifter::OnSupplyStandBy0(int nzMode, int nzSite) 
{
	int nFuncRet = CTLBD_RET_PROCEED;	

	int m_Mot = m_nMotZ0;
	int nRet = VAR_INIT_;
	double dNowPos;

	Func.OnTrace_ThreadStep(5, m_nMotZStep[0]);

	switch(m_nMotZStep[0])
	{
	case 0:
		if( nzMode == 1 )
		{
			m_nMotZStep[0] = 3000;
		}
		else
		{
			m_nMotZStep[0] = 10;
		}
		break;

	case 10:
		if( nzMode == 2 )
		{
			m_nMotZStep[0] = 5000;
		
		}
		else
		{
			st_work.nAccyCNT[0] = 0;
			m_nMotZStep[0] = 100;
		}
		break;

	case 100:
		nRet = COMI.Start_SingleMove(m_Mot, st_motor[m_Mot].d_pos[Z_ACC_LIFT_READY_], (int)st_handler.md_run_speed);
		
		if (nRet == CTLBD_RET_GOOD)
		{
			m_nMotZStep[0] = 110;
		}
		else if (nRet == CTLBD_RET_ERROR || nRet == CTLBD_RET_SAFETY)
		{
			// yangkyu edit
			// 001000 0 00 "LIFTER_UP_DOWN_SHIFT1_Z_ACC_LIFT_READY__MOVE_ERR."
			alarm.mstr_code		= "001000";
			CTL_Lib.Alarm_Error_Occurrence( 2200, CTL_dWARNING, alarm.mstr_code );
		}
		break;

	case 110:
		nRet = COMI.Check_SingleMove(m_Mot, st_motor[m_Mot].d_pos[Z_ACC_LIFT_READY_]);
		
		if (nRet == CTLBD_RET_GOOD)
		{
			m_nMotZStep[0] = 200;
		}
		else if (nRet == CTLBD_RET_ERROR || nRet == CTLBD_RET_SAFETY)
		{
			m_nMotZStep[0] = 100;
			// 001000 0 00 "LIFTER_UP_DOWN_SHIFT1_Z_ACC_LIFT_READY__MOVE_ERR."
			alarm.mstr_code		= "001000";
			CTL_Lib.Alarm_Error_Occurrence( 2201, CTL_dWARNING, alarm.mstr_code );
		}
		else if (nRet == CTLBD_RET_RETRY)	
		{
			m_nMotZStep[0] = 100;
		}
		break;

	case 200:
		//nRet = !g_ioMgr.get_in_bit(stIO.i_Chk_BoxClampAccyDetection[nzSite][0]);
		//kwlee 2017.0825
		nRet = !g_ioMgr.get_in_bit(stIO.i_Chk_BoxClampAccyDetection[nzSite][SIDE_BW_ + m_WorkSite * 2]);
		
		if (nRet == IO_ON)	
		{
			m_nMotZStep[0] = 2000;
		}
		else 
		{
			m_dTargetPos[0] = COMI.Get_MotCurrentPos(m_Mot);
			if (m_dTargetPos[0] >= st_motor[m_Mot].d_pos[Z_ACC_LIFT_LIMIT_])
			{
				m_nMotZStep[0] = 0;
				nFuncRet = CTLBD_RET_NO_ACC;  // 자재 미존재함 [DOWN 위치로 내려달라고 요청]
				break;
			}
			m_nMotZStep[0] = 1000;
		}
		break;

	case 1000:
		dNowPos = COMI.Get_MotCurrentPos(m_Mot);
		
		m_dTargetPos[0] = dNowPos + st_motor[m_Mot].d_pos[Z_ACC_LIFT_OFFSET_] / 2;
		
		if (m_dTargetPos[0] >= st_motor[m_Mot].d_pos[Z_ACC_LIFT_LIMIT_])
		{
			m_dTargetPos[0] = st_motor[m_Mot].d_pos[Z_ACC_LIFT_LIMIT_];
		}
		
		m_nMotZStep[0] = 1100;
		break;

	case 1100:
		nRet = COMI.Start_SingleMove(m_Mot, m_dTargetPos[0], (int)st_handler.md_run_speed);
		
		if (nRet == CTLBD_RET_GOOD)
		{
			//m_nMotZStep[0] = 200;
			//kwlee 2017.0902
			m_nMotZStep[0] = 1200;
		}
		else if (nRet == CTLBD_RET_ERROR || nRet == CTLBD_RET_SAFETY)
		{
			// yangkyu edit
			// 001000 0 00 "LIFTER_UP_DOWN_SHIFT1_Z_ACC_LIFT_READY__MOVE_ERR."
			alarm.mstr_code		= "001000";
			CTL_Lib.Alarm_Error_Occurrence( 2202, CTL_dWARNING, alarm.mstr_code );
		}
		break;

	//kwlee 2017.0902
	case 1200:
		nRet = COMI.Check_SingleMove(m_Mot, m_dTargetPos[0]);
		if (nRet == CTLBD_RET_GOOD)
		{
			m_nMotZStep[0] = 200;
		}
		else if (nRet == CTLBD_RET_RETRY)	
		{
			m_nMotZStep[0] = 1100;
		}
		else if (nRet == CTLBD_RET_ERROR || nRet == CTLBD_RET_SAFETY)
		{
			m_nMotZStep[0] = 1100;
			// 001002 0 00 "LIFTER_UP_DOWN_SHIFT1_Z_ACC_LIFT_OFFSET_MOVE_ERR."
			alarm.mstr_code		= "001002";
			CTL_Lib.Alarm_Error_Occurrence( 2306, CTL_dWARNING, alarm.mstr_code );
		}
		break;
	//

	case 2000:
		m_nMotZStep[0] = 0;
	
		nFuncRet = CTLBD_RET_GOOD; 
		break;


	case 3000:
		nRet = COMI.Start_SingleMove( m_Mot, st_motor[m_Mot].d_pos[Z_ACC_LIFT_SAFETY_], (int)st_handler.md_run_speed);
		if( nRet == CTLBD_RET_GOOD )
		{
			m_nMotZStep[0] = 3100;
		}
		else if( nRet == CTLBD_RET_ERROR || nRet == CTLBD_RET_SAFETY )
		{
			// 001001 0 00 "LIFTER_UP_DOWN_SHIFT1_Z_ACC_LIFT_SAFETY_MOVE_ERR."
			alarm.mstr_code		= "001001";
			CTL_Lib.Alarm_Error_Occurrence( 2203, CTL_dWARNING, alarm.mstr_code );
		}
		break;

	case 3100:
		nRet = COMI.Check_SingleMove(m_Mot, st_motor[m_Mot].d_pos[Z_ACC_LIFT_SAFETY_]);
		if (nRet == CTLBD_RET_GOOD)
		{
			m_nMotZStep[0] = 0;
			nFuncRet = CTLBD_RET_SKIP;
		}
		else if (nRet == CTLBD_RET_RETRY)
		{
			m_nMotZStep[0] = 3000;
		}
		else if (nRet == CTLBD_RET_ERROR || nRet == CTLBD_RET_SAFETY)
		{
			m_nMotZStep[0] = 3000;
			
			// 001001 0 00 "LIFTER_UP_DOWN_SHIFT1_Z_ACC_LIFT_SAFETY_MOVE_ERR."
			alarm.mstr_code		= "001001";
			CTL_Lib.Alarm_Error_Occurrence( 2204, CTL_dWARNING, alarm.mstr_code );
		}
		break;

		

	case 5000:
		if (st_basic.n_mode_device == WITHOUT_DVC_)
		{
			(st_work.nAccyCNT[0])++;
			
			if ((st_work.nAccyCNT[0]) > WITHOUT_SUPPLY_CNT)
			{
				m_nMotZStep[0] = 0;
				nFuncRet = CTLBD_RET_NO_ACC;  // DVC No [DOWN 위치로 내려달라고 요청]
			}
			else
			{
				// 현재 위치에서 공급이 완료되었다고 설정한다
				m_dTargetPos[0] = COMI.Get_MotCurrentPos(m_Mot) + st_motor[m_Mot].d_pos[Z_ACC_LIFT_OFFSET_];  // 자재 두께 값만큼 더한다
				nRet = !g_ioMgr.get_in_bit(stIO.i_Chk_BoxClampAccyDetection[nzSite][SIDE_BW_ + m_WorkSite * 2]); //kwlee 2017.0825

				//if (m_dTargetPos[0] > st_motor[m_Mot].d_pos[Z_ACC_LIFT_LIMIT_] )
				//kwlee 2017.0825
				if ((m_dTargetPos[0] > st_motor[m_Mot].d_pos[Z_ACC_LIFT_LIMIT_] && nRet == IO_OFF) )
				{
					m_nMotZStep[0] = 0;
					m_dTargetPos[0] = st_motor[m_Mot].d_pos[Z_ACC_LIFT_LIMIT_]; //kwlee 2017.0830
					nFuncRet = CTLBD_RET_NO_ACC;  // 자재 미존재함 [DOWN 위치로 내려달라고 요청]
					break;
				}
				m_nMotZStep[0] = 5100;
			}
		}
		else
		{
			//kwlee 2017.0902
			nRet = !g_ioMgr.get_in_bit(stIO.i_Chk_BoxClampAccyDetection[nzSite][SIDE_BW_ + m_WorkSite * 2]);
			if (nRet == IO_ON)	
			{
				m_nMotZStep[0] = 6000;
				break;
			}
			// 현재 모터 위치로부터 자재 두께 값만큼 위로 올려서 자재 공급 위치를 설정한다
			dNowPos = COMI.Get_MotCurrentPos(m_Mot);
			m_dTargetPos[0] = dNowPos + st_motor[m_Mot].d_pos[Z_ACC_LIFT_OFFSET_] / 2;  // 자재 두께 값만큼 더한다
			//kwlee 2017.0914
// 			nRet = !g_ioMgr.get_in_bit(stIO.i_Chk_BoxClampAccyDetection[nzSite][SIDE_BW_ + m_WorkSite * 2]); //kwlee 2017.0825
// 			//if (m_dTargetPos[0] > st_motor[m_Mot].d_pos[Z_ACC_LIFT_LIMIT_])
// 			//kwlee 2017.0825
// 			if (m_dTargetPos[0] > st_motor[m_Mot].d_pos[Z_ACC_LIFT_LIMIT_] && nRet == IO_OFF)
// 			{
// 				m_nMotZStep[0] = 0;
// 				nFuncRet = CTLBD_RET_NO_ACC;  // AUAc ¹IA¸AcCO [DOWN A§A¡·I ³≫·A´Þ¶o°i ¿aA≫]
// 				break;
// 			}
			//

			//kwlee 2017.0830
			 if (m_dTargetPos[0] >= st_motor[m_Mot].d_pos[Z_ACC_LIFT_LIMIT_])
			{
				m_dTargetPos[0] = st_motor[m_Mot].d_pos[Z_ACC_LIFT_LIMIT_];
			}
			
			m_nMotZStep[0] = 5100;
		}

		break;

	case 5100:
		nRet = COMI.Start_SingleMove(m_Mot, m_dTargetPos[0], (int)st_handler.md_run_speed);
		if (nRet == CTLBD_RET_GOOD)	
		{
			
			m_nMotZStep[0] = 5110;
		}
		else if (nRet == CTLBD_RET_ERROR || nRet == CTLBD_RET_SAFETY)
		{
			// 001002 0 00 "LIFTER_UP_DOWN_SHIFT1_Z_ACC_LIFT_OFFSET_MOVE_ERR."
			alarm.mstr_code		= "001002";
			CTL_Lib.Alarm_Error_Occurrence( 2205, CTL_dWARNING, alarm.mstr_code );
		}
		break;

	case 5110:
		nRet = COMI.Check_SingleMove(m_Mot, m_dTargetPos[0]);
		if (nRet == CTLBD_RET_GOOD)
		{
			//m_lLiftUpCheckTime[0][0] = GetCurrentTime(); //kwlee 2017.0902
			nRet = !g_ioMgr.get_in_bit(stIO.i_Chk_BoxClampAccyDetection[nzSite][SIDE_BW_ + m_WorkSite * 2]); //kwlee 2017.0825
			if (m_dTargetPos[0] >= st_motor[m_Mot].d_pos[Z_ACC_LIFT_LIMIT_] && nRet == IO_OFF)
			{
				m_nMotZStep[0] = 0;
				nFuncRet = CTLBD_RET_NO_ACC;  // 자재 미존재함 [DOWN 위치로 내려달라고 요청]
				break;
			}
			m_nMotZStep[0] = 5200;
		}
		else if (nRet == CTLBD_RET_RETRY)	
		{
			m_nMotZStep[0] = 5100;
		}
		else if (nRet == CTLBD_RET_ERROR || nRet == CTLBD_RET_SAFETY)
		{
			m_nMotZStep[0] = 5100;
			
			// 001002 0 00 "LIFTER_UP_DOWN_SHIFT1_Z_ACC_LIFT_OFFSET_MOVE_ERR."
			alarm.mstr_code		= "001002";
			CTL_Lib.Alarm_Error_Occurrence( 2206, CTL_dWARNING, alarm.mstr_code );
		}
		break;

	case 5200:
		//nRet = !g_ioMgr.get_in_bit(stIO.i_Chk_BoxClampAccyDetection[nzSite][0]);
		//kwlee 2017.0825
		nRet = !g_ioMgr.get_in_bit(stIO.i_Chk_BoxClampAccyDetection[nzSite][SIDE_BW_ + m_WorkSite * 2]);
		dNowPos = COMI.Get_MotCurrentPos(m_Mot);// kwlee 2017.0902
		//if (nRet == IO_ON)
		//kwlee 2017.0903
		if (nRet == IO_ON ||dNowPos >= m_dTargetPos[0] )	
		{
			m_nMotZStep[0] = 6000;
		}
		else
		{
			m_nMotZStep[0] = 5000;
		}
		break;

	case 6000:
		m_nMotZStep[0] = 0;
		nFuncRet = CTLBD_RET_GOOD;  // 자재 공급 완료
		break;

	}
	
	return nFuncRet;
}

int CSeq_BoxLifter::OnSupplyInitial1() 
{
	//kwlee 2017.0721
	int nFuncRet = CTLBD_RET_PROCEED;
	int nRet = VAR_INIT_;
	
	switch(m_nMotZInitStep[1])
	{
	case 0:
		nRet = COMI.HomeCheck_Mot(m_nMotZ1, 0, MOT_TIMEOUT);
		if (nRet == CTLBD_RET_GOOD)	
		{
			m_nMotZInitStep[1] = 5000;
		}
		else if (nRet == CTLBD_RET_ERROR)
		{
			//  000007 0 00 "LIFTER_UP_DOWN_SHIFT2_NOT_READY_HOME_CHK_ERR."
			alarm.mstr_code		= "010007";
			CTL_Lib.Alarm_Error_Occurrence( 2306, CTL_dWARNING, alarm.mstr_code );
		}
		break;
		
	case 5000:
		nFuncRet = CTLBD_RET_GOOD;  // HOMING 완료
		m_nMotZInitStep[1] = 0;
		break;
	}
	return nFuncRet;
}

//==================================================================//
// Lift 유닉 Z0 대기 위치로 이동
// nFuncRet : CTLBD_RET_PROCEED, CTLBD_RET_SKIP, CTLBD_RET_GOOD
// - int nzMode : [0:악세사리 공급 요청, 1:안전 위치 이동, 2:Offset 만큼 이동]
// - int nzSite : 작업 영역
//==================================================================//
int CSeq_BoxLifter::OnSupplyStandBy1(int nzMode, int nzSite) 
{
	int nFuncRet = CTLBD_RET_PROCEED;	

	int m_Mot = m_nMotZ1;
	int nRet = VAR_INIT_;
	double dNowPos;

	Func.OnTrace_ThreadStep(6, m_nMotZStep[1]);
	switch(m_nMotZStep[1])
	{
	case 0:
		if( nzMode == 1 )
		{
			m_nMotZStep[1] = 3000;
		}
		else
		{
			m_nMotZStep[1] = 10;
		}
		break;

	case 10:
		if( nzMode == 2 )
		{
			m_nMotZStep[1] = 5000;
		}
		else
		{
			st_work.nAccyCNT[1] = 0;
			m_nMotZStep[1] = 100;
		}
		break;
		
	case 100:
		nRet = COMI.Start_SingleMove(m_Mot, st_motor[m_Mot].d_pos[Z_ACC_LIFT_READY_], (int)st_handler.md_run_speed);
		
		if (nRet == CTLBD_RET_GOOD)
		{
			m_nMotZStep[1] = 110;
		}
		else if (nRet == CTLBD_RET_ERROR || nRet == CTLBD_RET_SAFETY)
		{
			// yangkyu edit
			// 001000 0 00 "LIFTER_UP_DOWN_SHIFT1_Z_ACC_LIFT_READY__MOVE_ERR."
			alarm.mstr_code		= "001000";
			CTL_Lib.Alarm_Error_Occurrence( 2300, CTL_dWARNING, alarm.mstr_code );
		}
		break;

	case 110:
		nRet = COMI.Check_SingleMove(m_Mot, st_motor[m_Mot].d_pos[Z_ACC_LIFT_READY_]);
		
		if (nRet == CTLBD_RET_GOOD)
		{
			m_nMotZStep[1] = 200;
		}
		else if (nRet == CTLBD_RET_ERROR || nRet == CTLBD_RET_SAFETY)
		{
			m_nMotZStep[1] = 100;
			// 001000 0 00 "LIFTER_UP_DOWN_SHIFT1_Z_ACC_LIFT_READY__MOVE_ERR."
			alarm.mstr_code		= "001000";
			CTL_Lib.Alarm_Error_Occurrence( 2301, CTL_dWARNING, alarm.mstr_code );
		}
		else if (nRet == CTLBD_RET_RETRY)	
		{
			m_nMotZStep[1] = 100;
		}
		break;

	case 200:
		//nRet = !g_ioMgr.get_in_bit(stIO.i_Chk_BoxClampAccyDetection[nzSite][1]);
		//kwlee 2017.0825
		nRet = !g_ioMgr.get_in_bit(stIO.i_Chk_BoxClampAccyDetection[nzSite][SIDE_BW_ + m_WorkSite * 2]);
		
		if (nRet == IO_ON)	
		{
			m_nMotZStep[1] = 2000;
		}
		else 
		{
// 			m_dTargetPos[0] = COMI.Get_MotCurrentPos(m_Mot);
// 			if (m_dTargetPos[0] >= st_motor[m_Mot].d_pos[Z_ACC_LIFT_LIMIT_])
			//kwlee 2017.0825
			m_dTargetPos[1] = COMI.Get_MotCurrentPos(m_Mot);
			if (m_dTargetPos[1] >= st_motor[m_Mot].d_pos[Z_ACC_LIFT_LIMIT_])
			{
				m_nMotZStep[1] = 0;
				nFuncRet = CTLBD_RET_NO_ACC;  // 자재 미존재함 [DOWN 위치로 내려달라고 요청]
				break;
			}
			m_nMotZStep[1] = 1000;
		}
		break;

	case 1000:
		dNowPos = COMI.Get_MotCurrentPos(m_Mot);
		
// 		m_dTargetPos[0] = dNowPos + st_motor[m_Mot].d_pos[Z_ACC_LIFT_OFFSET_] / 2;
// 		
// 		if (m_dTargetPos[0] > st_motor[m_Mot].d_pos[Z_ACC_LIFT_LIMIT_])
// 		{
// 			m_dTargetPos[0] = st_motor[m_Mot].d_pos[Z_ACC_LIFT_LIMIT_];
// 		}
		//kwlee 2017.0825
		m_dTargetPos[1] = dNowPos + st_motor[m_Mot].d_pos[Z_ACC_LIFT_OFFSET_] / 2;
		
		if (m_dTargetPos[1] >= st_motor[m_Mot].d_pos[Z_ACC_LIFT_LIMIT_])
		{
			m_dTargetPos[1] = st_motor[m_Mot].d_pos[Z_ACC_LIFT_LIMIT_];
		}
		
		m_nMotZStep[1] = 1100;
		break;

	case 1100:
		//nRet = COMI.Start_SingleMove(m_Mot, m_dTargetPos[0], (int)st_handler.md_run_speed);
		//kwlee 2017.0825
		nRet = COMI.Start_SingleMove(m_Mot, m_dTargetPos[1], (int)st_handler.md_run_speed);
		
		if (nRet == CTLBD_RET_GOOD)
		{
			//m_nMotZStep[1] = 200;
			//kwlee 2017.0902
			m_nMotZStep[1] = 1200;
		}
		else if (nRet == CTLBD_RET_ERROR || nRet == CTLBD_RET_SAFETY)
		{
			// yangkyu edit
			// 001000 0 00 "LIFTER_UP_DOWN_SHIFT1_Z_ACC_LIFT_READY__MOVE_ERR."
			alarm.mstr_code		= "001000";
			CTL_Lib.Alarm_Error_Occurrence( 2302, CTL_dWARNING, alarm.mstr_code );
		}
		break;

 //kwlee 2017.0902
	case 1200:
		nRet = COMI.Check_SingleMove(m_Mot, m_dTargetPos[1]);
		if (nRet == CTLBD_RET_GOOD)
		{
			m_nMotZStep[1] = 200;
		}
		else if (nRet == CTLBD_RET_RETRY)	
		{
			m_nMotZStep[1] = 1100;
		}
		else if (nRet == CTLBD_RET_ERROR || nRet == CTLBD_RET_SAFETY)
		{
			m_nMotZStep[1] = 1100;
			// 001002 0 00 "LIFTER_UP_DOWN_SHIFT1_Z_ACC_LIFT_OFFSET_MOVE_ERR."
			alarm.mstr_code		= "001002";
			CTL_Lib.Alarm_Error_Occurrence( 2306, CTL_dWARNING, alarm.mstr_code );
		}
		break;
	//

	case 2000:
		m_nMotZStep[1] = 0;
		nFuncRet = CTLBD_RET_GOOD; 
		break;


	case 3000:
		nRet = COMI.Start_SingleMove( m_Mot, st_motor[m_Mot].d_pos[Z_ACC_LIFT_SAFETY_], (int)st_handler.md_run_speed);
		if( nRet == CTLBD_RET_GOOD )
		{
			m_nMotZStep[1] = 3100;
		}
		else if( nRet == CTLBD_RET_ERROR || nRet == CTLBD_RET_SAFETY )
		{
			// 001001 0 00 "LIFTER_UP_DOWN_SHIFT1_Z_ACC_LIFT_SAFETY_MOVE_ERR."
			alarm.mstr_code		= "001001";
			CTL_Lib.Alarm_Error_Occurrence( 2303, CTL_dWARNING, alarm.mstr_code );
		}
		break;

	case 3100:
		nRet = COMI.Check_SingleMove(m_Mot, st_motor[m_Mot].d_pos[Z_ACC_LIFT_SAFETY_]);
		if (nRet == CTLBD_RET_GOOD)
		{
			m_nMotZStep[1] = 0;
			nFuncRet = CTLBD_RET_SKIP;
		}
		else if (nRet == CTLBD_RET_RETRY)
		{
			m_nMotZStep[1] = 3000;
		}
		else if (nRet == CTLBD_RET_ERROR || nRet == CTLBD_RET_SAFETY)
		{
			m_nMotZStep[1] = 3000;
			
			// 001001 0 00 "LIFTER_UP_DOWN_SHIFT1_Z_ACC_LIFT_SAFETY_MOVE_ERR."
			alarm.mstr_code		= "001001";
			CTL_Lib.Alarm_Error_Occurrence( 2304, CTL_dWARNING, alarm.mstr_code );
		}
		break;

	case 5000:
		if (st_basic.n_mode_device == WITHOUT_DVC_)
		{
			(st_work.nAccyCNT[1])++;
			
			if ((st_work.nAccyCNT[1]) > WITHOUT_SUPPLY_CNT)
			{
				m_nMotZStep[1] = 0;
				nFuncRet = CTLBD_RET_NO_ACC;  // DVC No [DOWN 위치로 내려달라고 요청]
			}
			else
			{
			
				// 현재 위치에서 공급이 완료되었다고 설정한다
// 				m_dTargetPos[0] = COMI.Get_MotCurrentPos(m_Mot) + st_motor[m_Mot].d_pos[Z_ACC_LIFT_OFFSET_];  // AUAc μI²² °ª¸¸A­ ´oCN´U
// 				if (m_dTargetPos[0] > st_motor[m_Mot].d_pos[Z_ACC_LIFT_LIMIT_])
				//kwlee 2017.0825
				m_dTargetPos[1] = COMI.Get_MotCurrentPos(m_Mot) + st_motor[m_Mot].d_pos[Z_ACC_LIFT_OFFSET_];  // 자재 두께 값만큼 더한다
				if (m_dTargetPos[1] > st_motor[m_Mot].d_pos[Z_ACC_LIFT_LIMIT_])
				{
					m_nMotZStep[1] = 0;
					nFuncRet = CTLBD_RET_NO_ACC;  // 자재 미존재함 [DOWN 위치로 내려달라고 요청]
					break;
				}
				m_nMotZStep[1] = 5100;
			}
		}
		else
		{
			//kwlee 2017.0902
			nRet = !g_ioMgr.get_in_bit(stIO.i_Chk_BoxClampAccyDetection[nzSite][SIDE_BW_ + m_WorkSite * 2]);
			
			if (nRet == IO_ON)	
			{
				m_nMotZStep[1] = 6000;
				break;
			}

			// 현재 모터 위치로부터 자재 두께 값만큼 위로 올려서 자재 공급 위치를 설정한다
			
// 			m_dTargetPos[0] = dNowPos + st_motor[m_Mot].d_pos[Z_ACC_LIFT_OFFSET_] / 2;  // AUAc μI²² °ª¸¸A­ ´oCN´U
// 			if (m_dTargetPos[0] > st_motor[m_Mot].d_pos[Z_ACC_LIFT_LIMIT_])
			//kwlee 2017.0825
			dNowPos = COMI.Get_MotCurrentPos(m_Mot);
			m_dTargetPos[1] = dNowPos + st_motor[m_Mot].d_pos[Z_ACC_LIFT_OFFSET_] / 2;  // 자재 두께 값만큼 더한다
// 			nRet = !g_ioMgr.get_in_bit(stIO.i_Chk_BoxClampAccyDetection[nzSite][SIDE_BW_ + m_WorkSite * 2]); //kwlee 2017.0825
// 			//if (m_dTargetPos[1] > st_motor[m_Mot].d_pos[Z_ACC_LIFT_LIMIT_])
// 			//kwlee 2017.0825
// 			 if (m_dTargetPos[1] > st_motor[m_Mot].d_pos[Z_ACC_LIFT_LIMIT_] && nRet ==IO_OFF)
// 			{
// 				m_nMotZStep[1] = 0;
// 				nFuncRet = CTLBD_RET_NO_ACC;  // AUAc ¹IA¸AcCO [DOWN A§A¡·I ³≫·A´Þ¶o°i ¿aA≫]
// 				break;
// 			}
			 //kwlee 2017.0830
			if (m_dTargetPos[1] >= st_motor[m_Mot].d_pos[Z_ACC_LIFT_LIMIT_])
			{
				m_dTargetPos[1] = st_motor[m_Mot].d_pos[Z_ACC_LIFT_LIMIT_];
			}
			m_nMotZStep[1] = 5100;
		}

		break;

	case 5100:
		//nRet = COMI.Start_SingleMove(m_Mot, m_dTargetPos[0], (int)st_handler.md_run_speed);
		//kwlee 2017.0825
		nRet = COMI.Start_SingleMove(m_Mot, m_dTargetPos[1], (int)st_handler.md_run_speed);
		if (nRet == CTLBD_RET_GOOD)	
		{
			m_nMotZStep[1] = 5110;
		}
		else if (nRet == CTLBD_RET_ERROR || nRet == CTLBD_RET_SAFETY)
		{
			// 001002 0 00 "LIFTER_UP_DOWN_SHIFT1_Z_ACC_LIFT_OFFSET_MOVE_ERR."
			alarm.mstr_code		= "001002";
			CTL_Lib.Alarm_Error_Occurrence( 2305, CTL_dWARNING, alarm.mstr_code );
		}
		break;

	case 5110:
		//nRet = COMI.Check_SingleMove(m_Mot, m_dTargetPos[0]);
		//kwlee 2017.0825
		nRet = COMI.Check_SingleMove(m_Mot, m_dTargetPos[1]);
		if (nRet == CTLBD_RET_GOOD)
		{	
			///kwlee 2017.0914
			nRet = !g_ioMgr.get_in_bit(stIO.i_Chk_BoxClampAccyDetection[nzSite][SIDE_BW_ + m_WorkSite * 2]); //kwlee 2017.0825
			if (m_dTargetPos[1] >= st_motor[m_Mot].d_pos[Z_ACC_LIFT_LIMIT_] && nRet ==IO_OFF)
			{
				m_nMotZStep[1] = 0;
				nFuncRet = CTLBD_RET_NO_ACC;  // 자재 미존재함 [DOWN 위치로 내려달라고 요청]
				break;
			}

			m_nMotZStep[1] = 5200;
		}
		else if (nRet == CTLBD_RET_RETRY)	
		{
			m_nMotZStep[1] = 5100;
		}
		else if (nRet == CTLBD_RET_ERROR || nRet == CTLBD_RET_SAFETY)
		{
			m_nMotZStep[1] = 5100;
			
			// 001002 0 00 "LIFTER_UP_DOWN_SHIFT1_Z_ACC_LIFT_OFFSET_MOVE_ERR."
			alarm.mstr_code		= "001002";
			CTL_Lib.Alarm_Error_Occurrence( 2306, CTL_dWARNING, alarm.mstr_code );
		}
		break;

	case 5200:
		//nRet = !g_ioMgr.get_in_bit(stIO.i_Chk_BoxClampAccyDetection[nzSite][1]);
		//kwlee 2017.0825
		nRet = !g_ioMgr.get_in_bit(stIO.i_Chk_BoxClampAccyDetection[nzSite][SIDE_BW_ + m_WorkSite * 2]);
		dNowPos = COMI.Get_MotCurrentPos(m_Mot);

		//if (nRet == IO_ON)
		//kwlee 2017.902
		if (nRet == IO_ON || dNowPos>= m_dTargetPos[1])	
		{
			m_nMotZStep[1] = 6000;
		}
		else
		{
			m_nMotZStep[1] = 5000;
		}
		break;

	case 6000:
		m_nMotZStep[1] = 0;
		nFuncRet = CTLBD_RET_GOOD;  // 자재 공급 완료
		break;

	}
	
	return nFuncRet;
}

int CSeq_BoxLifter::OnSupplyInitial2() 
{
	//kwlee 2017.0721
	int nFuncRet = CTLBD_RET_PROCEED;
	int nRet = VAR_INIT_;
	
	switch(m_nMotZInitStep[2])
	{
	case 0:
		nRet = COMI.HomeCheck_Mot(m_nMotZ2, 0, MOT_TIMEOUT);
		if (nRet == CTLBD_RET_GOOD)	
		{
			m_nMotZInitStep[2] = 5000;
		}
		else if (nRet == CTLBD_RET_ERROR)
		{
			//  000007 0 00 "LIFTER_UP_DOWN_SHIFT2_NOT_READY_HOME_CHK_ERR."
			alarm.mstr_code		= "020007";
			CTL_Lib.Alarm_Error_Occurrence( 2306, CTL_dWARNING, alarm.mstr_code );
		}
		break;
		
	case 5000:
		nFuncRet = CTLBD_RET_GOOD;  // HOMING 완료
		m_nMotZInitStep[2] = 0;
		break;
	}
	return nFuncRet;

}

//==================================================================//
// Lift 유닉 Z0 대기 위치로 이동
// nFuncRet : CTLBD_RET_PROCEED, CTLBD_RET_SKIP, CTLBD_RET_GOOD
// - int nzMode : [0:악세사리 공급 요청, 1:안전 위치 이동, 2:Offset 만큼 이동]
// - int nzSite : 작업 영역
//==================================================================//
int CSeq_BoxLifter::OnSupplyStandBy2(int nzMode, int nzSite) 
{
	int nFuncRet = CTLBD_RET_PROCEED;	

	int m_Mot = m_nMotZ2;
	int nRet = VAR_INIT_;
	double dNowPos;

	Func.OnTrace_ThreadStep(7, m_nMotZStep[2]);
	switch(m_nMotZStep[2])
	{
	case 0:
		if( nzMode == 1 )
		{
			m_nMotZStep[2] = 3000;
		}
		else
		{
			m_nMotZStep[2] = 10;
		}
		break;

	case 10:
		if( nzMode == 2 )
		{
			m_nMotZStep[2] = 5000;
		}
		else
		{
			st_work.nAccyCNT[1] = 0;
			m_nMotZStep[2] = 100;
		}
		break;

	case 100:
		nRet = COMI.Start_SingleMove(m_Mot, st_motor[m_Mot].d_pos[Z_ACC_LIFT_READY_], (int)st_handler.md_run_speed);
		
		if (nRet == CTLBD_RET_GOOD)
		{
			m_nMotZStep[2] = 110;
		}
		else if (nRet == CTLBD_RET_ERROR || nRet == CTLBD_RET_SAFETY)
		{
			// yangkyu edit
			// 001000 0 00 "LIFTER_UP_DOWN_SHIFT1_Z_ACC_LIFT_READY__MOVE_ERR."
			alarm.mstr_code		= "001000";
			CTL_Lib.Alarm_Error_Occurrence( 2300, CTL_dWARNING, alarm.mstr_code );
		}
		break;

	case 110:
		nRet = COMI.Check_SingleMove(m_Mot, st_motor[m_Mot].d_pos[Z_ACC_LIFT_READY_]);
		
		if (nRet == CTLBD_RET_GOOD)
		{
			m_nMotZStep[2] = 200;
		}
		else if (nRet == CTLBD_RET_ERROR || nRet == CTLBD_RET_SAFETY)
		{
			m_nMotZStep[2] = 100;
			// 001000 0 00 "LIFTER_UP_DOWN_SHIFT1_Z_ACC_LIFT_READY__MOVE_ERR."
			alarm.mstr_code		= "001000";
			CTL_Lib.Alarm_Error_Occurrence( 2301, CTL_dWARNING, alarm.mstr_code );
		}
		else if (nRet == CTLBD_RET_RETRY)	
		{
			m_nMotZStep[2] = 100;
		}
		break;

	case 200:
		//nRet = !g_ioMgr.get_in_bit(stIO.i_Chk_BoxClampAccyDetection[nzSite][2]);
		//kwlee 2017.0825
		nRet = !g_ioMgr.get_in_bit(stIO.i_Chk_BoxClampAccyDetection[nzSite][SIDE_FW_ + m_WorkSite * 2]);
		
		if (nRet == IO_ON)	
		{
			m_nMotZStep[2] = 2000;
		}
		else 
		{
// 			m_dTargetPos[0] = COMI.Get_MotCurrentPos(m_Mot);
// 			if (m_dTargetPos[0] >= st_motor[m_Mot].d_pos[Z_ACC_LIFT_LIMIT_])
			//kwlee 2017.0825
			m_dTargetPos[2] = COMI.Get_MotCurrentPos(m_Mot);
			if (m_dTargetPos[2] >= st_motor[m_Mot].d_pos[Z_ACC_LIFT_LIMIT_])
			{
				m_nMotZStep[2] = 0;
				nFuncRet = CTLBD_RET_NO_ACC;  // 자재 미존재함 [DOWN 위치로 내려달라고 요청]
				break;
			}
			m_nMotZStep[2] = 1000;
		}
		break;

	case 1000:
		dNowPos = COMI.Get_MotCurrentPos(m_Mot);
		
// 		m_dTargetPos[0] = dNowPos + st_motor[m_Mot].d_pos[Z_ACC_LIFT_OFFSET_] / 2;
// 		
// 		if (m_dTargetPos[0] > st_motor[m_Mot].d_pos[Z_ACC_LIFT_LIMIT_])
// 		{
// 			m_dTargetPos[0] = st_motor[m_Mot].d_pos[Z_ACC_LIFT_LIMIT_];
// 		}
		//kwlee 2017.0825
		m_dTargetPos[2] = dNowPos + st_motor[m_Mot].d_pos[Z_ACC_LIFT_OFFSET_] / 2;
		
		if (m_dTargetPos[2] >= st_motor[m_Mot].d_pos[Z_ACC_LIFT_LIMIT_])
		{
			m_dTargetPos[2] = st_motor[m_Mot].d_pos[Z_ACC_LIFT_LIMIT_];
		}
		
		m_nMotZStep[2] = 1100;
		break;

	case 1100:
		nRet = COMI.Start_SingleMove(m_Mot, m_dTargetPos[2], (int)st_handler.md_run_speed); //kwlee 2017.0825 m_dTargetPos[0] -> m_dTargetPos[2]
		
		if (nRet == CTLBD_RET_GOOD)
		{
			//m_nMotZStep[2] = 200;
			//kwlee 2017.0902
			m_nMotZStep[2] = 1200;
		}
		else if (nRet == CTLBD_RET_ERROR || nRet == CTLBD_RET_SAFETY)
		{
			// yangkyu edit
			// 001000 0 00 "LIFTER_UP_DOWN_SHIFT1_Z_ACC_LIFT_READY__MOVE_ERR."
			alarm.mstr_code		= "001000";
			CTL_Lib.Alarm_Error_Occurrence( 2302, CTL_dWARNING, alarm.mstr_code );
		}
		break;


		//kwlee 2017.0902
	case 1200:
		nRet = COMI.Check_SingleMove(m_Mot, m_dTargetPos[2]);
		if (nRet == CTLBD_RET_GOOD)
		{
			m_nMotZStep[2] = 200;
		}
		else if (nRet == CTLBD_RET_RETRY)	
		{
			m_nMotZStep[2] = 1100;
		}
		else if (nRet == CTLBD_RET_ERROR || nRet == CTLBD_RET_SAFETY)
		{
			m_nMotZStep[2] = 1100;
			// 001002 0 00 "LIFTER_UP_DOWN_SHIFT1_Z_ACC_LIFT_OFFSET_MOVE_ERR."
			alarm.mstr_code		= "001002";
			CTL_Lib.Alarm_Error_Occurrence( 2306, CTL_dWARNING, alarm.mstr_code );
		}
		break;
	//

	case 2000:
		m_nMotZStep[2] = 0;
		nFuncRet = CTLBD_RET_GOOD; 
		break;


	case 3000:
		nRet = COMI.Start_SingleMove( m_Mot, st_motor[m_Mot].d_pos[Z_ACC_LIFT_SAFETY_], (int)st_handler.md_run_speed);
		if( nRet == CTLBD_RET_GOOD )
		{
			m_nMotZStep[2] = 3100;
		}
		else if( nRet == CTLBD_RET_ERROR || nRet == CTLBD_RET_SAFETY )
		{
			// 001001 0 00 "LIFTER_UP_DOWN_SHIFT1_Z_ACC_LIFT_SAFETY_MOVE_ERR."
			alarm.mstr_code		= "001001";
			CTL_Lib.Alarm_Error_Occurrence( 2303, CTL_dWARNING, alarm.mstr_code );
		}
		break;

	case 3100:
		nRet = COMI.Check_SingleMove(m_Mot, st_motor[m_Mot].d_pos[Z_ACC_LIFT_SAFETY_]);
		if (nRet == CTLBD_RET_GOOD)
		{
			m_nMotZStep[2] = 0;
			nFuncRet = CTLBD_RET_SKIP;
		}
		else if (nRet == CTLBD_RET_RETRY)
		{
			m_nMotZStep[2] = 3000;
		}
		else if (nRet == CTLBD_RET_ERROR || nRet == CTLBD_RET_SAFETY)
		{
			m_nMotZStep[2] = 3000;
			
			// 001001 0 00 "LIFTER_UP_DOWN_SHIFT1_Z_ACC_LIFT_SAFETY_MOVE_ERR."
			alarm.mstr_code		= "001001";
			CTL_Lib.Alarm_Error_Occurrence( 2304, CTL_dWARNING, alarm.mstr_code );
		}
		break;

	case 5000:
		if (st_basic.n_mode_device == WITHOUT_DVC_)
		{
			(st_work.nAccyCNT[1])++;
			
			if ((st_work.nAccyCNT[1]) > WITHOUT_SUPPLY_CNT)
			{
				m_nMotZStep[2] = 0;
				nFuncRet = CTLBD_RET_NO_ACC;  // DVC No [DOWN 위치로 내려달라고 요청]
			}
			else
			{
				
				// 현재 위치에서 공급이 완료되었다고 설정한다
				//kwlee 2017.0825 m_dTargetPos[0] -> m_dTargetPos[2]
				dNowPos = COMI.Get_MotCurrentPos(m_Mot);
				m_dTargetPos[2] = dNowPos + st_motor[m_Mot].d_pos[Z_ACC_LIFT_OFFSET_];  // 자재 두께 값만큼 더한다
				nRet = !g_ioMgr.get_in_bit(stIO.i_Chk_BoxClampAccyDetection[nzSite][SIDE_FW_ + m_WorkSite * 2]);
				//if (m_dTargetPos[2] > st_motor[m_Mot].d_pos[Z_ACC_LIFT_LIMIT_]) //kwlee 2017.0825 m_dTargetPos[0] -> m_dTargetPos[2]

				//if (m_dTargetPos[2] > st_motor[m_Mot].d_pos[Z_ACC_LIFT_LIMIT_] && nRet == IO_OFF) 
				//kwlee 2017.0901
				if (m_dTargetPos[2] >= st_motor[m_Mot].d_pos[Z_ACC_LIFT_LIMIT_] && nRet == IO_OFF) 
				{
					m_nMotZStep[2] = 0;
					nFuncRet = CTLBD_RET_NO_ACC;  // 자재 미존재함 [DOWN 위치로 내려달라고 요청]
					break;
				}
				m_nMotZStep[2] = 5100;
			}
		}
		else
		{
			//kwlee 2017.0902
			nRet = !g_ioMgr.get_in_bit(stIO.i_Chk_BoxClampAccyDetection[nzSite][SIDE_FW_ + m_WorkSite * 2]);
			
			if (nRet == IO_ON)	
			{
				m_nMotZStep[2] = 6000;
				break;
			}

			// 현재 모터 위치로부터 자재 두께 값만큼 위로 올려서 자재 공급 위치를 설정한다
			dNowPos = COMI.Get_MotCurrentPos(m_Mot);
			m_dTargetPos[2] = dNowPos + st_motor[m_Mot].d_pos[Z_ACC_LIFT_OFFSET_] / 2;  // 자재 두께 값만큼 더한다
			
// 			nRet = !g_ioMgr.get_in_bit(stIO.i_Chk_BoxClampAccyDetection[nzSite][SIDE_FW_ + m_WorkSite * 2]);
// 			if (m_dTargetPos[2] > st_motor[m_Mot].d_pos[Z_ACC_LIFT_LIMIT_] && nRet == IO_OFF) //kwlee 2017.0825 m_dTargetPos[0] -> m_dTargetPos[2]
// 			{
// 				m_nMotZStep[2] = 0;
// 
// 				nFuncRet = CTLBD_RET_NO_ACC;  // AUAc ¹IA¸AcCO [DOWN A§A¡·I ³≫·A´Þ¶o°i ¿aA≫]
// 				break;
// 			}
			
			 if (m_dTargetPos[2] >= st_motor[m_Mot].d_pos[Z_ACC_LIFT_LIMIT_])
			{
				m_dTargetPos[2] = st_motor[m_Mot].d_pos[Z_ACC_LIFT_LIMIT_];
			}
			m_nMotZStep[2] = 5100;
		}
		break;

	case 5100:
		nRet = COMI.Start_SingleMove(m_Mot, m_dTargetPos[2], (int)st_handler.md_run_speed); //kwlee 2017.0825 m_dTargetPos[0] -> m_dTargetPos[2]
		if (nRet == CTLBD_RET_GOOD)	
		{
			m_nMotZStep[2] = 5110;
		}
		else if (nRet == CTLBD_RET_ERROR || nRet == CTLBD_RET_SAFETY)
		{
			// 001002 0 00 "LIFTER_UP_DOWN_SHIFT1_Z_ACC_LIFT_OFFSET_MOVE_ERR."
			alarm.mstr_code		= "001002";
			CTL_Lib.Alarm_Error_Occurrence( 2305, CTL_dWARNING, alarm.mstr_code );
		}
		break;

	case 5110:
		nRet = COMI.Check_SingleMove(m_Mot, m_dTargetPos[2]);//kwlee 2017.0825 m_dTargetPos[0] -> m_dTargetPos[2]
		if (nRet == CTLBD_RET_GOOD)
		{
			//kwlee 2017.0914
			nRet = !g_ioMgr.get_in_bit(stIO.i_Chk_BoxClampAccyDetection[nzSite][SIDE_FW_ + m_WorkSite * 2]);
			
			if (m_dTargetPos[2] >= st_motor[m_Mot].d_pos[Z_ACC_LIFT_LIMIT_] && nRet == IO_OFF) //kwlee 2017.0825 m_dTargetPos[0] -> m_dTargetPos[2]
			{
				m_nMotZStep[2] = 0;
				
				nFuncRet = CTLBD_RET_NO_ACC;  // 자재 미존재함 [DOWN 위치로 내려달라고 요청]
				break;
			}
			m_nMotZStep[2] = 5200;
		}
		else if (nRet == CTLBD_RET_RETRY)	
		{
			m_nMotZStep[2] = 5100;
		}
		else if (nRet == CTLBD_RET_ERROR || nRet == CTLBD_RET_SAFETY)
		{
			m_nMotZStep[2] = 5100;
			
			// 001002 0 00 "LIFTER_UP_DOWN_SHIFT1_Z_ACC_LIFT_OFFSET_MOVE_ERR."
			alarm.mstr_code		= "001002";
			CTL_Lib.Alarm_Error_Occurrence( 2306, CTL_dWARNING, alarm.mstr_code );
		}
		break;

	case 5200:
		//nRet = !g_ioMgr.get_in_bit(stIO.i_Chk_BoxClampAccyDetection[nzSite][2]);
		//kwlee 2017.0825
		nRet = !g_ioMgr.get_in_bit(stIO.i_Chk_BoxClampAccyDetection[nzSite][SIDE_FW_ + m_WorkSite * 2 ]);
		dNowPos = COMI.Get_MotCurrentPos(m_Mot);

		//if (nRet == IO_ON)
		//kwlee 2017.902
		if (nRet == IO_ON || dNowPos >= m_dTargetPos[2])	
		{
			m_nMotZStep[2] = 6000;
		}
		else
		{
			m_nMotZStep[2] = 5000;
		}
		break;

	case 6000:
		m_nMotZStep[2] = 0;
		nFuncRet = CTLBD_RET_GOOD;  // 자재 공급 완료
		break;

	}
	
	return nFuncRet;
}


int CSeq_BoxLifter::OnSupplyInitial3() 
{
	//kwlee 2017.0721
	int nFuncRet = CTLBD_RET_PROCEED;
	int nRet = VAR_INIT_;
	
	switch(m_nMotZInitStep[3])
	{
	case 0:
		nRet = COMI.HomeCheck_Mot(m_nMotZ3, 0, MOT_TIMEOUT);
		if (nRet == CTLBD_RET_GOOD)	
		{
			m_nMotZInitStep[3] = 5000;
		}
		else if (nRet == CTLBD_RET_ERROR)
		{
			//  000007 0 00 "LIFTER_UP_DOWN_SHIFT3_NOT_READY_HOME_CHK_ERR."
			alarm.mstr_code		= "030007";
			CTL_Lib.Alarm_Error_Occurrence( 2306, CTL_dWARNING, alarm.mstr_code );
		}
		break;
		
	case 5000:
		nFuncRet = CTLBD_RET_GOOD;  // HOMING 완료
		m_nMotZInitStep[3] = 0;
		break;
	}
	return nFuncRet;

}

//==================================================================//
// Lift 유닉 Z0 대기 위치로 이동
// nFuncRet : CTLBD_RET_PROCEED, CTLBD_RET_SKIP, CTLBD_RET_GOOD
// - int nzMode : [0:악세사리 공급 요청, 1:안전 위치 이동, 2:Offset 만큼 이동]
// - int nzSite : 작업 영역
//==================================================================//
int CSeq_BoxLifter::OnSupplyStandBy3(int nzMode, int nzSite) 
{
	int nFuncRet = CTLBD_RET_PROCEED;	

	int m_Mot = m_nMotZ3;
	int nRet = VAR_INIT_;
	double dNowPos;

	Func.OnTrace_ThreadStep(8, m_nMotZStep[3]);
	switch(m_nMotZStep[3])
	{
	case 0:
		if( nzMode == 1 )
		{
			m_nMotZStep[3] = 3000;
		}
		else
		{
			m_nMotZStep[3] = 10;
		}
		break;

	case 10:
		if( nzMode == 2 )
		{
			m_nMotZStep[3] = 5000;
		}
		else
		{
			st_work.nAccyCNT[1] = 0;
			m_nMotZStep[3] = 100;
		}
		break;

	case 100:
		nRet = COMI.Start_SingleMove(m_Mot, st_motor[m_Mot].d_pos[Z_ACC_LIFT_READY_], (int)st_handler.md_run_speed);
		
		if (nRet == CTLBD_RET_GOOD)
		{
			m_nMotZStep[3] = 110;
		}
		else if (nRet == CTLBD_RET_ERROR || nRet == CTLBD_RET_SAFETY)
		{
			// yangkyu edit
			// 001000 0 00 "LIFTER_UP_DOWN_SHIFT1_Z_ACC_LIFT_READY__MOVE_ERR."
			alarm.mstr_code		= "001000";
			CTL_Lib.Alarm_Error_Occurrence( 2300, CTL_dWARNING, alarm.mstr_code );
		}
		break;

	case 110:
		nRet = COMI.Check_SingleMove(m_Mot, st_motor[m_Mot].d_pos[Z_ACC_LIFT_READY_]);
		
		if (nRet == CTLBD_RET_GOOD)
		{
			m_nMotZStep[3] = 200;
		}
		else if (nRet == CTLBD_RET_ERROR || nRet == CTLBD_RET_SAFETY)
		{
			m_nMotZStep[3] = 100;
			// 001000 0 00 "LIFTER_UP_DOWN_SHIFT1_Z_ACC_LIFT_READY__MOVE_ERR."
			alarm.mstr_code		= "001000";
			CTL_Lib.Alarm_Error_Occurrence( 2301, CTL_dWARNING, alarm.mstr_code );
		}
		else if (nRet == CTLBD_RET_RETRY)	
		{
			m_nMotZStep[3] = 100;
		}
		break;

	case 200:
		//nRet = !g_ioMgr.get_in_bit(stIO.i_Chk_BoxClampAccyDetection[nzSite][3]);
		//kwlee 2017.0825
		nRet = !g_ioMgr.get_in_bit(stIO.i_Chk_BoxClampAccyDetection[nzSite][SIDE_FW_ + m_WorkSite * 2]);	
		if (nRet == IO_ON)	
		{
			m_nMotZStep[3] = 2000;
		}
		else 
		{
			m_dTargetPos[3] = COMI.Get_MotCurrentPos(m_Mot); //kwlee 2017.0825 m_dTargetPos[0] -> m_dTargetPos[3]
			if (m_dTargetPos[3] >= st_motor[m_Mot].d_pos[Z_ACC_LIFT_LIMIT_]) //kwlee 2017.0825 m_dTargetPos[0] -> m_dTargetPos[3]
			{
				m_nMotZStep[3] = 0;
				nFuncRet = CTLBD_RET_NO_ACC;  // 자재 미존재함 [DOWN 위치로 내려달라고 요청]
				break;
			}
			m_nMotZStep[3] = 1000;
		}
		break;

	case 1000:
		dNowPos = COMI.Get_MotCurrentPos(m_Mot);
		
		m_dTargetPos[3] = dNowPos + st_motor[m_Mot].d_pos[Z_ACC_LIFT_OFFSET_] / 2; //kwlee 2017.0825 m_dTargetPos[0] -> m_dTargetPos[3]
		
		if (m_dTargetPos[3] >= st_motor[m_Mot].d_pos[Z_ACC_LIFT_LIMIT_])
		{
			m_dTargetPos[3] = st_motor[m_Mot].d_pos[Z_ACC_LIFT_LIMIT_];
		}
		
		m_nMotZStep[3] = 1100;
		break;

	case 1100:
		nRet = COMI.Start_SingleMove(m_Mot, m_dTargetPos[3], (int)st_handler.md_run_speed); //kwlee 2017.0825 m_dTargetPos[0] -> m_dTargetPos[3]
		
		if (nRet == CTLBD_RET_GOOD)
		{
			//m_nMotZStep[3] = 200;
			//kwlee 2017.0902
			m_nMotZStep[3] = 1200;
		}
		else if (nRet == CTLBD_RET_ERROR || nRet == CTLBD_RET_SAFETY)
		{
			// yangkyu edit
			// 001000 0 00 "LIFTER_UP_DOWN_SHIFT1_Z_ACC_LIFT_READY__MOVE_ERR."
			alarm.mstr_code		= "001000";
			CTL_Lib.Alarm_Error_Occurrence( 2302, CTL_dWARNING, alarm.mstr_code );
		}
		break;
		//kwlee 2017.0902
	case 1200:
		nRet = COMI.Check_SingleMove(m_Mot, m_dTargetPos[3]);
		if (nRet == CTLBD_RET_GOOD)
		{
			m_nMotZStep[3] = 200;
		}
		else if (nRet == CTLBD_RET_RETRY)	
		{
			m_nMotZStep[3] = 1100;
		}
		else if (nRet == CTLBD_RET_ERROR || nRet == CTLBD_RET_SAFETY)
		{
			m_nMotZStep[3] = 1100;
			// 001002 0 00 "LIFTER_UP_DOWN_SHIFT1_Z_ACC_LIFT_OFFSET_MOVE_ERR."
			alarm.mstr_code		= "001002";
			CTL_Lib.Alarm_Error_Occurrence( 2306, CTL_dWARNING, alarm.mstr_code );
		}
		break;
	//

	case 2000:
		m_nMotZStep[3] = 0;
		nFuncRet = CTLBD_RET_GOOD; 
		break;


	case 3000:
		nRet = COMI.Start_SingleMove( m_Mot, st_motor[m_Mot].d_pos[Z_ACC_LIFT_SAFETY_], (int)st_handler.md_run_speed);
		if( nRet == CTLBD_RET_GOOD )
		{
			m_nMotZStep[3] = 3100;
		}
		else if( nRet == CTLBD_RET_ERROR || nRet == CTLBD_RET_SAFETY )
		{
			// 001001 0 00 "LIFTER_UP_DOWN_SHIFT1_Z_ACC_LIFT_SAFETY_MOVE_ERR."
			alarm.mstr_code		= "001001";
			CTL_Lib.Alarm_Error_Occurrence( 2303, CTL_dWARNING, alarm.mstr_code );
		}
		break;

	case 3100:
		nRet = COMI.Check_SingleMove(m_Mot, st_motor[m_Mot].d_pos[Z_ACC_LIFT_SAFETY_]);
		if (nRet == CTLBD_RET_GOOD)
		{
			m_nMotZStep[3] = 0;
			nFuncRet = CTLBD_RET_SKIP;
		}
		else if (nRet == CTLBD_RET_RETRY)
		{
			m_nMotZStep[3] = 3000;
		}
		else if (nRet == CTLBD_RET_ERROR || nRet == CTLBD_RET_SAFETY)
		{
			m_nMotZStep[3] = 3000;
			
			// 001001 0 00 "LIFTER_UP_DOWN_SHIFT1_Z_ACC_LIFT_SAFETY_MOVE_ERR."
			alarm.mstr_code		= "001001";
			CTL_Lib.Alarm_Error_Occurrence( 2304, CTL_dWARNING, alarm.mstr_code );
		}
		break;

	case 5000:
		if (st_basic.n_mode_device == WITHOUT_DVC_)
		{
			(st_work.nAccyCNT[1])++;
			
			if ((st_work.nAccyCNT[1]) > WITHOUT_SUPPLY_CNT)
			{
				m_nMotZStep[3] = 0;
				nFuncRet = CTLBD_RET_NO_ACC;  // DVC No [DOWN 위치로 내려달라고 요청]
			}
			else
			{
				
				// 현재 위치에서 공급이 완료되었다고 설정한다
// 				m_dTargetPos[0] = COMI.Get_MotCurrentPos(m_Mot) + st_motor[m_Mot].d_pos[Z_ACC_LIFT_OFFSET_];  // AUAc μI²² °ª¸¸A­ ´oCN´U
// 				if (m_dTargetPos[0] > st_motor[m_Mot].d_pos[Z_ACC_LIFT_LIMIT_])
				//kwlee 2017.0825
				m_dTargetPos[3] = COMI.Get_MotCurrentPos(m_Mot) + st_motor[m_Mot].d_pos[Z_ACC_LIFT_OFFSET_];  // 자재 두께 값만큼 더한다
				if (m_dTargetPos[3] > st_motor[m_Mot].d_pos[Z_ACC_LIFT_LIMIT_])
				{
					m_nMotZStep[3] = 0;
					nFuncRet = CTLBD_RET_NO_ACC;  // 자재 미존재함 [DOWN 위치로 내려달라고 요청]
					break;
				}
				m_nMotZStep[3] = 5100;
			}
		}
		else
		{
			//kwlee 2017.0902
			nRet = !g_ioMgr.get_in_bit(stIO.i_Chk_BoxClampAccyDetection[nzSite][SIDE_FW_ + m_WorkSite * 2]);	
			if (nRet == IO_ON)	
			{
				m_nMotZStep[3] = 6000;
				break;
			}

			// 현재 모터 위치로부터 자재 두께 값만큼 위로 올려서 자재 공급 위치를 설정한다
			dNowPos = COMI.Get_MotCurrentPos(m_Mot);
			m_dTargetPos[3] = dNowPos + st_motor[m_Mot].d_pos[Z_ACC_LIFT_OFFSET_] / 2;  // 자재 두께 값만큼 더한다
// 			nRet = !g_ioMgr.get_in_bit(stIO.i_Chk_BoxClampAccyDetection[nzSite][SIDE_FW_ + m_WorkSite * 2]);
// 
// 			//if (m_dTargetPos[3] > st_motor[m_Mot].d_pos[Z_ACC_LIFT_LIMIT_])
// 			//kwlee 2017.0825
// 			if (m_dTargetPos[3] > st_motor[m_Mot].d_pos[Z_ACC_LIFT_LIMIT_] && nRet == IO_OFF)
// 			{
// 				m_nMotZStep[3] = 0;
// 				nFuncRet = CTLBD_RET_NO_ACC;  // AUAc ¹IA¸AcCO [DOWN A§A¡·I ³≫·A´Þ¶o°i ¿aA≫]
// 				break;
// 			}
			//kwlee 2017.0830
			 if (m_dTargetPos[3] >= st_motor[m_Mot].d_pos[Z_ACC_LIFT_LIMIT_])
			{
				m_dTargetPos[3] = st_motor[m_Mot].d_pos[Z_ACC_LIFT_LIMIT_];
			}
			m_nMotZStep[3] = 5100;
		}

		break;

	case 5100:
		//nRet = COMI.Start_SingleMove(m_Mot, m_dTargetPos[0], (int)st_handler.md_run_speed);
		//kwlee 2017.0825
		nRet = COMI.Start_SingleMove(m_Mot, m_dTargetPos[3], (int)st_handler.md_run_speed);
		if (nRet == CTLBD_RET_GOOD)	
		{
			m_nMotZStep[3] = 5110;
		}
		else if (nRet == CTLBD_RET_ERROR || nRet == CTLBD_RET_SAFETY)
		{
			// 001002 0 00 "LIFTER_UP_DOWN_SHIFT1_Z_ACC_LIFT_OFFSET_MOVE_ERR."
			alarm.mstr_code		= "001002";
			CTL_Lib.Alarm_Error_Occurrence( 2305, CTL_dWARNING, alarm.mstr_code );
		}
		break;

	case 5110:
		//nRet = COMI.Check_SingleMove(m_Mot, m_dTargetPos[0]);
		//kwlee 2017.0825
		nRet = COMI.Check_SingleMove(m_Mot, m_dTargetPos[3]);
		if (nRet == CTLBD_RET_GOOD)
		{
			//kwlee 2017.0914
			nRet = !g_ioMgr.get_in_bit(stIO.i_Chk_BoxClampAccyDetection[nzSite][SIDE_FW_ + m_WorkSite * 2]);
			if (m_dTargetPos[3] >= st_motor[m_Mot].d_pos[Z_ACC_LIFT_LIMIT_] && nRet == IO_OFF)
			{
				m_nMotZStep[3] = 0;
				nFuncRet = CTLBD_RET_NO_ACC;  // 자재 미존재함 [DOWN 위치로 내려달라고 요청]
				break;
			}
			m_nMotZStep[3] = 5200;
		}
		else if (nRet == CTLBD_RET_RETRY)	
		{
			m_nMotZStep[3] = 5100;
		}
		else if (nRet == CTLBD_RET_ERROR || nRet == CTLBD_RET_SAFETY)
		{
			m_nMotZStep[3] = 5100;
			
			// 001002 0 00 "LIFTER_UP_DOWN_SHIFT1_Z_ACC_LIFT_OFFSET_MOVE_ERR."
			alarm.mstr_code		= "001002";
			CTL_Lib.Alarm_Error_Occurrence( 2306, CTL_dWARNING, alarm.mstr_code );
		}
		break;

	case 5200:
		//nRet = !g_ioMgr.get_in_bit(stIO.i_Chk_BoxClampAccyDetection[nzSite][3]);
		//kwlee 2017.0825
		nRet = !g_ioMgr.get_in_bit(stIO.i_Chk_BoxClampAccyDetection[nzSite][SIDE_FW_ + m_WorkSite * 2]);
		dNowPos = COMI.Get_MotCurrentPos(m_Mot);

		//if (nRet == IO_ON)
		//kwlee 2017.0902
		if (nRet == IO_ON || dNowPos >= m_dTargetPos[3])	
		{
			m_nMotZStep[3] = 6000;
		}
		else
		{
			m_nMotZStep[3] = 5000;
		}
		break;

	case 6000:
		m_nMotZStep[3] = 0;
		nFuncRet = CTLBD_RET_GOOD;  // 자재 공급 완료
		break;

	}
	
	return nFuncRet;
}

int CSeq_BoxLifter::OnProc_ReAlign() 
{
	// 동작 스텝 정보를 파일에 저장한다 [디버깅용]
	Func.OnTrace_ThreadStep(21, m_nStep_ReAlig);	// 쓰레드 스텝 정보 TRACE
	
	int nFuncRet = CTLBD_RET_PROCEED;
	
	int nRet = VAR_INIT_;
	int nRetData[4];
	
	switch(m_nStep_ReAlig)
	{
	case 0:
		if (st_map.m_nReq_ReAlign == SYNC_REQ_ACCY_REALIGN)
		{
			m_nStep_ReAlig = 100;	
		}
		break;

	case 100:// 자재 공급 완료 여부 초기화
		m_nStandByOK[0] = CTL_NO;  	m_nStandByOK[1] = CTL_NO; 
		m_nStandByOK[2] = CTL_NO;  	m_nStandByOK[3] = CTL_NO; 
		
		m_nStep_ReAlig = 200;
		break;
		
	case 200:
		if (m_nStandByOK[0] == CTL_NO)
		{
			// DOWN 위치로 이동 요청
			//nRetData[0] = OnSupplyStandBy0(1, st_map.nLiftWorkSite);  				// Accy (Rear -> Front)
			//kwlee 2017.0825
			nRetData[0] = OnSupplyStandBy0(1, SIDE_LEFT_);  				// Accy (Rear -> Front)
			if (nRetData[0] == CTLBD_RET_SKIP)	
			{
				m_nStandByOK[0] = CTL_DVC_SKIP;  // DOWN 위치 이동 완료
			}
		}
		if (m_nStandByOK[1] == CTL_NO)
		{
			//nRetData[1] = OnSupplyStandBy1(1, st_map.nLiftWorkSite);		
			//kwlee  2017.0825
			nRetData[1] = OnSupplyStandBy1(1, SIDE_RIGHT_);		
			if (nRetData[1] == CTLBD_RET_SKIP)	
			{
				m_nStandByOK[1] = CTL_DVC_SKIP;
			}
		}
		if (m_nStandByOK[2] == CTL_NO)
		{
			//nRetData[2] = OnSupplyStandBy2(1, st_map.nLiftWorkSite); 		
			//kwlee 2017.0825
			nRetData[2] = OnSupplyStandBy2(1, SIDE_LEFT_); 		
			if (nRetData[2] == CTLBD_RET_SKIP)	
			{
				m_nStandByOK[2] = CTL_DVC_SKIP;
			}
		}
		if (m_nStandByOK[3] == CTL_NO)
		{
			//nRetData[3] = OnSupplyStandBy3(1, st_map.nLiftWorkSite);
			//kwlee 2017.0825
			nRetData[3] = OnSupplyStandBy3(1, SIDE_RIGHT_);
			if (nRetData[3] == CTLBD_RET_SKIP)	
			{
				m_nStandByOK[3] = CTL_DVC_SKIP;
			}
		}
		
		if (m_nStandByOK[0] == CTL_DVC_SKIP && m_nStandByOK[1] == CTL_DVC_SKIP &&
			m_nStandByOK[2] == CTL_DVC_SKIP && m_nStandByOK[3] == CTL_DVC_SKIP )
		{
			m_nStep_ReAlig = 500;
		}
		break;

	case 500:
		m_nStandByOK[0] = CTL_NO;  m_nStandByOK[1] = CTL_NO;
		m_nStandByOK[2] = CTL_NO;  m_nStandByOK[3] = CTL_NO;

		m_nStep_ReAlig = 600;
		break;

	case 600:
		if (m_nStandByOK[0] == CTL_NO)
		{
			if (st_basic.n_mode_device == WITHOUT_DVC_)
			{
				// WithOut모드에서는 Offset 간격으로 UP
				//nRetData[0] = OnSupplyStandBy0(2, st_map.nLiftWorkSite);				// Accy(Rear -> Front)
				//kwlee 2017.0825
				nRetData[0] = OnSupplyStandBy0(2, SIDE_LEFT_);				// Accy(Rear -> Front)
			}
			else
			{
				// 센서가 감지 될때까지 Accy를 UP
				//nRetData[0] = OnSupplyStandBy0(0, st_map.nLiftWorkSite);
				//kwlee 2017.0825
				nRetData[0] = OnSupplyStandBy0(0, SIDE_LEFT_);
			}
			if (nRetData[0] == CTLBD_RET_GOOD)	
			{
				m_nStandByOK[0] = CTL_YES;  // 작업 준비 완료
			}
			else if (nRetData[0] == CTLBD_RET_SKIP)
			{
				m_nStandByOK[0] = CTL_DVC_SKIP;
			}
			else if (nRetData[0] == CTLBD_RET_NO_ACC)
			{
				// 자재가 감지되지 않아 DOWN 위치로 이동시켜야 할 조건
				m_nStandByOK[0] = CTL_DVC_SKIP;
			}
		}
		if (m_nStandByOK[1] == CTL_NO)
		{
			if (st_basic.n_mode_device == WITHOUT_DVC_)
			{
				//nRetData[1] = OnSupplyStandBy1(2, st_map.nLiftWorkSite);
				//kwlee 2017.0825
				nRetData[1] = OnSupplyStandBy1(2, SIDE_RIGHT_);
			}
			else
			{
				//nRetData[1] = OnSupplyStandBy1(0, st_map.nLiftWorkSite);
				//kwlee 2017.0825
				nRetData[1] = OnSupplyStandBy1(0, SIDE_RIGHT_);
			}
			if (nRetData[1] == CTLBD_RET_GOOD)	
			{
				m_nStandByOK[1] = CTL_YES;
			}
			else if (nRetData[1] == CTLBD_RET_SKIP)	
			{
				m_nStandByOK[1] = CTL_DVC_SKIP;
			}
			else if (nRetData[1] == CTLBD_RET_NO_ACC)
			{
				m_nStandByOK[1] = CTL_DVC_SKIP;
			}
		}
		if (m_nStandByOK[2] == CTL_NO)
		{
			if (st_basic.n_mode_device == WITHOUT_DVC_)
			{
				//nRetData[2] = OnSupplyStandBy2(2, st_map.nLiftWorkSite);
				//kwlee 2017.0825
				nRetData[2] = OnSupplyStandBy2(2, SIDE_LEFT_);
			}
			else
			{
				nRetData[2] = OnSupplyStandBy2(0, SIDE_LEFT_);
			}
			if (nRetData[2] == CTLBD_RET_GOOD)	
			{
				m_nStandByOK[2] = CTL_YES;
			}
			else if (nRetData[2] == CTLBD_RET_SKIP)	
			{
				m_nStandByOK[2] = CTL_DVC_SKIP;
			}
			else if (nRetData[2] == CTLBD_RET_NO_ACC)
			{
				m_nStandByOK[2] = CTL_DVC_SKIP;
			}
		}
		if (m_nStandByOK[3] == CTL_NO)
		{
			if (st_basic.n_mode_device == WITHOUT_DVC_)
			{
				//nRetData[3] = OnSupplyStandBy3(2, st_map.nLiftWorkSite);
				//kwlee 2017.0825
				nRetData[3] = OnSupplyStandBy3(2, SIDE_RIGHT_);
			}
			else
			{
				//nRetData[3] = OnSupplyStandBy3(0, st_map.nLiftWorkSite);
				//kwlee 2017.0825
				nRetData[3] = OnSupplyStandBy3(0, SIDE_RIGHT_);
			}
			if (nRetData[3] == CTLBD_RET_GOOD)	
			{
				m_nStandByOK[3] = CTL_YES;
			}
			else if (nRetData[3] == CTLBD_RET_SKIP)	
			{
				m_nStandByOK[3] = CTL_DVC_SKIP;
			}
			else if (nRetData[3] == CTLBD_RET_NO_ACC)
			{
				// 자재가 감지되지 않아 DOWN 위치로 이동시켜야 할 조건
				m_nStandByOK[3] = CTL_DVC_SKIP;
			}
		}
		
		// Lifter Z(0,1,2,3)에 대해서 모두 작업을 완료할때까지 기다린다
		if (m_nStandByOK[0] == CTL_NO || m_nStandByOK[1] == CTL_NO || m_nStandByOK[2] == CTL_NO || m_nStandByOK[3] == CTL_NO )
		{
			break;
		}

		// Lifter Z(0,1,2,3)에 대해서 모든 리프터가 정상적으로 자재 공급 완료했는지 검사
		if (m_nStandByOK[0] == CTL_YES && m_nStandByOK[1] == CTL_YES && m_nStandByOK[2] == CTL_YES && m_nStandByOK[3] == CTL_YES )
		{
			m_nStep_ReAlig = 10000;
		}
		break;

	case 10000:
		m_nStep_ReAlig = 0;
		st_map.m_nReq_ReAlign = SYNC_REQ_RESET_;
		nFuncRet = CTLBD_RET_GOOD;  // 자재 정렬 완료
		break;
	}


	return nFuncRet;
}