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
	
	st_map.nLiftWorkSite = SIDE_REAR_;		// Accy 작업 영역 설정 (작업 순서 : Rear -> Front)
	m_nBoxLift_Step = 0; //kwlee 2017.0711

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
	m_nBoxLift_Step = 0;
	m_nStep_Rcvy = 0;
	
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
		// 초기화 작업이 완료되지 않으면 [AUTO] 작업 시작 불가능
		// - st_handler.nIsAllSiteInitEnd : 초기화 화면에서 설정
		if( st_handler.nIsAllSiteInitEnd == CTL_YES )
		{
			OnRun_Move();
		}
		break;
		
	default:
		OnRun_ForceDischarge();
		break;
	}
	
}

void CSeq_BoxLifter::OnRun_Initial() 
{

	int nRet[5] = {0,};
	int nCnt,i;
	if( st_handler.mn_init_state[INIT_BOX_LIFT] != CTL_NO )	
	return;

	switch(m_nStep_Init)
	{
	case 0:
		m_nStep_Init = 100;
		break;

	case 100:
		//m_nStep_Init = 200;
		nRet[0] = g_comiMgr.HomeCheck_Mot( m_nMotZ0, 0, MOT_TIMEOUT );
		if (nRet[0] == CTLBD_RET_GOOD )
		{
			m_nStep_Init = 110;
		}
		else if (nRet[0] == CTLBD_RET_ERROR)
		{
			alarm.mstr_code		= "000003";
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
			CTL_Lib.Alarm_Error_Occurrence( 2000, CTL_dWARNING, alarm.mstr_code);
		}
		break;

	case 110:
		nRet[0] = g_comiMgr.HomeCheck_Mot( m_nMotZ1, 0, MOT_TIMEOUT );
		if (nRet[0] == CTLBD_RET_GOOD )
		{
			m_nStep_Init = 120;
		}
		else if (nRet[0] == CTLBD_RET_ERROR)
		{
			alarm.mstr_code		= "010003";
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
			CTL_Lib.Alarm_Error_Occurrence( 2000, CTL_dWARNING, alarm.mstr_code);
		}
		break;

	case 120:
		nRet[0] = g_comiMgr.HomeCheck_Mot( m_nMotZ2, 0, MOT_TIMEOUT );
		if (nRet[0] == CTLBD_RET_GOOD )
		{
			m_nStep_Init = 130;
		}
		else if (nRet[0] == CTLBD_RET_ERROR)
		{
			alarm.mstr_code		= "020003";
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
			CTL_Lib.Alarm_Error_Occurrence( 2000, CTL_dWARNING, alarm.mstr_code);
		}
		break;

	case 130:
		nRet[0] = g_comiMgr.HomeCheck_Mot( m_nMotZ3, 0, MOT_TIMEOUT );
		if (nRet[0] == CTLBD_RET_GOOD )
		{
			m_nStep_Init = 200;
		}
		else if (nRet[0] == CTLBD_RET_ERROR)
		{
			alarm.mstr_code		= "030003";
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
			CTL_Lib.Alarm_Error_Occurrence( 2000, CTL_dWARNING, alarm.mstr_code);
		}
		break;

// 	case 200:
// 		//nRet[0] = g_comiMgr.HomeCheck_Mot( m_nMotZ0, 0, MOT_TIMEOUT );
// // 		nRet[1] = g_comiMgr.HomeCheck_Mot( m_nMotZ1, 0, MOT_TIMEOUT );
// // 		nRet[2] = g_comiMgr.HomeCheck_Mot( m_nMotZ2, 0, MOT_TIMEOUT );
// // 		nRet[3] = g_comiMgr.HomeCheck_Mot( m_nMotZ3, 0, MOT_TIMEOUT );
// 
// 		//if( nRet[0] == CTLBD_RET_GOOD && nRet[1] == CTLBD_RET_GOOD && nRet[2] == CTLBD_RET_GOOD && nRet[3] == CTLBD_RET_GOOD)
// 		if( nRet[0] == CTLBD_RET_GOOD)
// 		{
// 			m_nStep_Init = 210;
// 		}
// 		else if (nRet[0] == CTLBD_RET_ERROR || nRet[1] == CTLBD_RET_ERROR || nRet[2] == CTLBD_RET_ERROR || nRet[3] == CTLBD_RET_ERROR)
// 		{
// 			if (nRet[0] == CTLBD_RET_ERROR)
// 			{
// 				// 000003 0 00 "LIFTER_UP_DOWN_SHIFT1_HOMING_ERR."
// 				alarm.mstr_code		= "000003";
// 			}
// 			else if (nRet[1] == CTLBD_RET_ERROR)
// 			{
// 				//010003 0 00 "LIFTER_UP_DOWN_SHIFT2_HOMING_ERR."
// 				alarm.mstr_code		= "010003";
// 			}
// 			else if (nRet[2] == CTLBD_RET_ERROR)
// 			{
// 				// 020003 0 00 "LIFTER_UP_DOWN_SHIFT3_HOMING_ERR."
// 				alarm.mstr_code		= "020003";
// 			}
// 			else if (nRet[3] == CTLBD_RET_ERROR)
// 			{
// 				// 030003 0 00 "LIFTER_UP_DOWN_SHIFT4_HOMING_ERR."
// 				alarm.mstr_code		= "030003";
// 			}
// 			alarm.mn_count_mode	= 0;
// 			alarm.mn_type_mode	= eWARNING;
// 			st_work.nEqpStatus	= dWARNING;
// 			CTL_Lib.Alarm_Error_Occurrence( 2000, CTL_dWARNING, alarm.mstr_code);
// 		}
// 		break;

	case 200:
		nRet[0] = CTL_Lib.OnSingleMove( m_nMotZ0, st_motor[m_nMotZ0].d_pos[Z_ACC_LIFT_SAFETY_], (int)st_handler.md_run_speed );
		nRet[1] = CTL_Lib.OnSingleMove( m_nMotZ1, st_motor[m_nMotZ1].d_pos[Z_ACC_LIFT_SAFETY_], (int)st_handler.md_run_speed );
		nRet[2] = CTL_Lib.OnSingleMove( m_nMotZ2, st_motor[m_nMotZ2].d_pos[Z_ACC_LIFT_SAFETY_], (int)st_handler.md_run_speed );
		nRet[3] = CTL_Lib.OnSingleMove( m_nMotZ3, st_motor[m_nMotZ3].d_pos[Z_ACC_LIFT_SAFETY_], (int)st_handler.md_run_speed );

		if( nRet[0] == CTLBD_RET_GOOD && nRet[1] == CTLBD_RET_GOOD && nRet[2] == CTLBD_RET_GOOD && nRet[3] == CTLBD_RET_GOOD)
		{
			st_work.nLiftDownComplete = CTL_YES;
			m_nStep_Init = 210;
		}
		else if (nRet[0] == CTLBD_RET_ERROR || nRet[1] == CTLBD_RET_ERROR || nRet[2] == CTLBD_RET_ERROR || nRet[3] == CTLBD_RET_ERROR)
		{
			if( nRet[0] == CTLBD_RET_ERROR || nRet[0] == CTLBD_RET_RETRY )
			{
				//001002 0 00 "LIFTER_UP_DOWN_SHIFT1_Z_ACC_LIFT_SAFETY_MOVE_ERR."
				alarm.mstr_code		= "001002";
			}
			else if( nRet[1] == CTLBD_RET_ERROR || nRet[1] == CTLBD_RET_RETRY )
			{
				//011002 0 00 "LIFTER_UP_DOWN_SHIFT2_Z_ACC_LIFT_SAFETY_MOVE_ERR."
				alarm.mstr_code		= "011002";
			}
			else if( nRet[2] == CTLBD_RET_ERROR || nRet[2] == CTLBD_RET_RETRY )
			{
				//021002 0 00 "LIFTER_UP_DOWN_SHIFT3_Z_ACC_LIFT_SAFETY_MOVE_ERR."
				alarm.mstr_code		= "021002";
			}
			else if( nRet[3] == CTLBD_RET_ERROR || nRet[3] == CTLBD_RET_RETRY )
			{
				//031002 0 00 "LIFTER_UP_DOWN_SHIFT4_Z_ACC_LIFT_SAFETY_MOVE_ERR."
				alarm.mstr_code		= "031002";
			}
			
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
			CTL_Lib.Alarm_Error_Occurrence( 2001, CTL_dWARNING, alarm.mstr_code);
		}
		break;

	case 210:
		nCnt =0;
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
			m_nStep_Init = 300;
		}
		else
		{
			//150000 0 00 "ACCY_UNLOADER_FULL_ERR."
			CTL_Lib.Alarm_Error_Occurrence( 2002, CTL_dWARNING, "150000");
		}
		break;

	case 300:
		nRet[0] = g_comiMgr.HomeCheck_Mot( m_nMotY, 0, MOT_TIMEOUT );
		if( nRet[0] == CTLBD_RET_GOOD )
		{
			m_nStep_Init = 400;
		}
		else if (nRet[0] == CTLBD_RET_ERROR)
		{
			//040003 0 00 "LIFTER_Y1_HOMING_ERR."
			alarm.mstr_code		= "040003";
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
			CTL_Lib.Alarm_Error_Occurrence( 2003, CTL_dWARNING, alarm.mstr_code);
		}
		break;

	case 400:
		nRet[0] = CTL_Lib.OnSingleMove( m_nMotY, st_motor[m_nMotY].d_pos[Y_ACC_BOX_AVOID_], (int)st_handler.md_run_speed );
		if( nRet[0] == CTLBD_RET_GOOD)
		{
			stSync.nResp_Lifter2Clamp_Work = SYNC_RESP_WORK_COMPLETE_;
			m_nStep_Init = 500;
		}
		else if (nRet[0] == CTLBD_RET_ERROR)
		{
			//001002 0 00 "LIFTER_UP_DOWN_SHIFT1_Z_ACC_LIFT_SAFETY_MOVE_ERR."
			alarm.mstr_code		= "001002";
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
			CTL_Lib.Alarm_Error_Occurrence( 2004, CTL_dWARNING, alarm.mstr_code);
		}
		break;

		//Lift Up 되었다고 신호 받았으면.
	case 500:
		if (stSync.nReq_Clamp2Lifter_Work == SYNC_REQ_RESET_)
		{
			m_nStep_Init = 600;		
		}
		break;

	case 600:
		nRet[0] = CTL_Lib.OnSingleMove( m_nMotY, st_motor[m_nMotY].d_pos[Y_ACC_REAR_], (int)st_handler.md_run_speed );
		if( nRet[0] == CTLBD_RET_GOOD)
		{
			m_nStep_Init = 1000;
		}
		else if (nRet[0] == CTLBD_RET_ERROR)
		{
			//042000 0 00 "LIFTER_Y1_WORK_AREA_MOVE_ERR."
			alarm.mstr_code		= "042000";
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
			CTL_Lib.Alarm_Error_Occurrence( 2005, CTL_dWARNING, alarm.mstr_code);
		}
		break;

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
	
	switch(m_nBoxLift_Step)
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
			
			st_map.nLiftWorkSite = SIDE_REAR_;		// Accy 작업 영역 설정 (작업 순서 : Rear -> Front)
			//m_nStep_Run = 200;
			//kwlee 2017.0721
			m_nBoxLift_Step = 100;
		}
		break;

	case 100:
		// Lifter 유닉을 Rear 작업 영역으로 이동
		nRet_1 = CTL_Lib.OnSingleMove(m_nMotY, st_motor[m_nMotY].d_pos[Y_ACC_REAR_], (int)st_handler.md_run_speed);
		if (nRet_1 == CTLBD_RET_GOOD)
		{
			if (st_basic.n_mode_device == WITHOUT_DVC_)
			{
				memset(st_work.nAccyCNT, 0, sizeof(st_work.nAccyCNT));
			}
			//m_nStep_Run = 300;
			//kwlee 2017.0721
			m_nBoxLift_Step = 200;

		}
		else if (nRet_1 == CTLBD_RET_ERROR || nRet_1 == CTLBD_RET_RETRY)
		{
			// 041001 0 00 "LIFTER_Y_ACC_REAR_MOVE_ERR."
			alarm.mstr_code		= "041001";
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1400, CTL_dWARNING, alarm.mstr_code);
		}
		break;

	case 200:
		m_nStandByOK[0] = CTL_NO;
		m_nStandByOK[1] = CTL_NO;
		m_nStandByOK[2] = CTL_NO;
		m_nStandByOK[3] = CTL_NO;

		dCurrPos[0] = COMI.Get_MotCurrentPos(m_nMotZ0);
		dCurrPos[1] = COMI.Get_MotCurrentPos(m_nMotZ1);
		dCurrPos[2] = COMI.Get_MotCurrentPos(m_nMotZ2);	
		dCurrPos[3] = COMI.Get_MotCurrentPos(m_nMotZ3);
		//m_nStep_Run = 300;
		//kwlee 2017.0721
		m_nBoxLift_Step = 300;
		break;

	case 300:
		if (m_nStandByOK[0] == CTL_NO)
		{
			if (dCurrPos[0] > st_motor[m_nMotZ0].d_pos[Z_ACC_LIFT_SAFETY_] + 10)
			{
				nRetData[0] = OnSupplyStandBy0(2, st_map.nLiftWorkSite);		// Rear -> Front
			}
			else
			{
				// UP Until Sensor is Sensing
				nRetData[0] = OnSupplyStandBy0(0, st_map.nLiftWorkSite);		// Rear -> Front
			}

			if (nRetData[0] == CTLBD_RET_GOOD)	
			{
				m_nStandByOK[0] = CTL_YES;
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
			// 2015.07.12 yangkyu
			//if (st_basic.n_mode_device == WITHOUT_DVC_)
			if (dCurrPos[1] > st_motor[m_nMotZ1].d_pos[Z_ACC_LIFT_SAFETY_] + 10)
			{
				// WithOut모드에서는 Offset 간격으로 UP
				// - int nzMode : [0:악세사리 공급 요청, 1:안전 위치 이동, 2:Offset 만큼 이동]
				nRetData[1] = OnSupplyStandBy1(2, st_map.nLiftWorkSite);		// Accy 작업 영역 설정 (작업 순서 : Rear -> Front)
			}
			else
			{
				// 센서가 감지 될때까지 Accy를 UP
				nRetData[1] = OnSupplyStandBy1(0, st_map.nLiftWorkSite);		// Accy 작업 영역 설정 (작업 순서 : Rear -> Front)
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
				// 자재가 감지되지 않아 DOWN 위치로 이동시켜야 할 조건
				m_nStandByOK[1] = CTL_DVC_SKIP;
			}
		}
		if (m_nStandByOK[2] == CTL_NO)
		{
			// 2015.07.12 yangkyu
			//if (st_basic.n_mode_device == WITHOUT_DVC_)
			if (dCurrPos[2] > st_motor[m_nMotZ2].d_pos[Z_ACC_LIFT_SAFETY_] + 10)
			{
				// WithOut모드에서는 Offset 간격으로 UP
				nRetData[2] = OnSupplyStandBy2(2, st_map.nLiftWorkSite);		// Accy 작업 영역 설정 (작업 순서 : Rear -> Front)
			}
			else
			{
				// 센서가 감지 될때까지 Accy를 UP
				nRetData[2] = OnSupplyStandBy2(0, st_map.nLiftWorkSite);		// Accy 작업 영역 설정 (작업 순서 : Rear -> Front)
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
				// 자재가 감지되지 않아 DOWN 위치로 이동시켜야 할 조건
				m_nStandByOK[2] = CTL_DVC_SKIP;
			}
		}
		if (m_nStandByOK[3] == CTL_NO)
		{
			// 2015.07.12 yangkyu
			//if (st_basic.n_mode_device == WITHOUT_DVC_)
			if(dCurrPos[3] > st_motor[m_nMotZ3].d_pos[Z_ACC_LIFT_SAFETY_] + 10)
			{
				// WithOut모드에서는 Offset 간격으로 UP
				nRetData[3] = OnSupplyStandBy3(2, st_map.nLiftWorkSite);		// Accy 작업 영역 설정 (작업 순서 : Rear -> Front)
			}
			else
			{
				// 센서가 감지 될때까지 Accy를 UP
				nRetData[3] = OnSupplyStandBy3(0, st_map.nLiftWorkSite);		// Accy 작업 영역 설정 (작업 순서 : Rear -> Front)
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
		if (m_nStandByOK[0] == CTL_NO ||
			m_nStandByOK[1] == CTL_NO ||
			m_nStandByOK[2] == CTL_NO ||
			m_nStandByOK[3] == CTL_NO )
		{
			break;
		}

		// Lifter Z(0,1,2,3)에 대해서 모든 리프터가 정상적으로 자재 공급 완료했는지 검사
		if (m_nStandByOK[0] == CTL_YES &&
			m_nStandByOK[1] == CTL_YES &&
			m_nStandByOK[2] == CTL_YES &&
			m_nStandByOK[3] == CTL_YES )
		{
			// [XYZ Robot <- BoxLifter] 작업 완료 했다고 응답
			stSync.nReq_Lifter2XYZRbt_Work = SYNC_REQ_ACC_LIFT_COMPLETE_;
			
			//m_nStep_Run = 1300;
			//kwlee 2017.0721
			//여기서 자재 없을 때까지 밀어 준다.
			//
			m_nBoxLift_Step = 1000;
		}
		// Lifter Z(0,1,2,3)에 대해서 모든 리프터가 정상적으로 자재 공급을 하지 못하였기 때문에 Down Step 진행
		else
		{
			m_nBoxLift_Step = 2600;
		}
		break;
		//m_nBoxLift_Step = 100;
	//	break;

	case 1000:
		//XYZ Robot에게서 작업 완료, 배출 신호 받음.
		if (stSync.nReq_Lifter2XYZRbt_Work == SYNC_REQ_ACC_LIFT_COMPLETE_)
		{
			m_nBoxLift_Step = 1100;
		}
		break;
	
	case 1100:
		//안전한 대기 위치로 빠진다.
		nRet_1 = CTL_Lib.OnSingleMove( m_nMotY, st_motor[m_nMotY].d_pos[Y_ACC_BOX_AVOID_], (int)st_handler.md_run_speed );
		if( nRet_1 == CTLBD_RET_GOOD )
		{
			m_nBoxLift_Step = 1200;
		}
		else if( nRet_1 == CTLBD_RET_ERROR || nRet_1 == CTLBD_RET_RETRY )
		{
			//041002 0 00 "LIFTER_Y1_X_ACC_BOX_AVOID_MOVE_ERR."
			alarm.mstr_code		= "041002";
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
			CTL_Lib.Alarm_Error_Occurrence( 2006, CTL_dWARNING, alarm.mstr_code);
		}
		break;

		//BoxLift -> Box Clamp에게 배출 신호 준다.
		//Box Clamp는 실린더 해제 한다.
		//Robot Safety Pos Check
// 	case 200:
// 		nRet_1 = g_ioMgr.get_in_bit(stIO.i_Chk_GripperSafetyPos,IO_OFF);
// 		if (nRet_1 == IO_OFF)
// 		{
// 			m_nBoxLift_Step = 300;
// 		}
// 		else
// 		{
// 			//040000 0 00 "LIFTER_Y1_SAFETY_NOT_EXIST_ERR."
// 			CTL_Lib.Alarm_Error_Occurrence( 1401, CTL_dWARNING, "040000");
// 		}
// 		break;
	
	//Box Lift -> ox Clamp Cylinder Off 하라고 명령.
	case 1200:
		stSync.nResp_Lifter2Clamp_Work = SYNC_RESP_WORK_COMPLETE_;
		m_nBoxLift_Step = 1300;
		break;

		//Box Clamp Clamp 해제하고 Lift 까지 Down
	case 1300:
		if (stSync.nReq_Clamp2Lifter_Work == SYNC_REQ_WORK_)
		{
			m_nBoxLift_Step = 1400;
		}
		break;

		//Out Conv Check Sensor B 접점.
		//1,2,3 Check 하여 모두 감지 시 Alarm
		//하나라도 비어 있으면 Push.
	case 1400:
		nCnt = 0;
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
			m_nBoxLift_Step = 1500;
		}
		else
		{
			//150000 0 00 "ACCY_UNLOADER_FULL_ERR."
			CTL_Lib.Alarm_Error_Occurrence( 2007, CTL_dWARNING, "150000");
		}
		break;

		//Box Out Conv로 민다.
	case 1500:
		nRet_1 = CTL_Lib.OnSingleMove( m_nMotY, st_motor[m_nMotY].d_pos[Y_ACC_FRONT_], (int)st_handler.md_run_speed );
		if( nRet_1 == CTLBD_RET_GOOD )
		{
			m_nBoxLift_Step = 1600;
		}
		else if( nRet_1 == CTLBD_RET_ERROR || nRet_1 == CTLBD_RET_RETRY )
		{
			//041002 0 00 "LIFTER_Y1_X_ACC_BOX_AVOID_MOVE_ERR."
			alarm.mstr_code		= "041002";
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
			CTL_Lib.Alarm_Error_Occurrence( 2008, CTL_dWARNING, alarm.mstr_code);
		}
		break;

		//Y Robot Avoid 위치 이동 하였다. Lift 올리자.
	case 1600:
		nRet_1 = CTL_Lib.OnSingleMove( m_nMotY, st_motor[m_nMotY].d_pos[Y_ACC_BOX_AVOID_], (int)st_handler.md_run_speed );
		if( nRet_1 == CTLBD_RET_GOOD )
		{
			stSync.nResp_Lifter2Clamp_Work = SYNC_RESP_BOX_REMOVE_COMPLETE;
			m_nBoxLift_Step = 1700;
		}
		else if( nRet_1 == CTLBD_RET_ERROR || nRet_1 == CTLBD_RET_RETRY )
		{
			//041002 0 00 "LIFTER_Y1_X_ACC_BOX_AVOID_MOVE_ERR."
			alarm.mstr_code		= "041002";
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
			CTL_Lib.Alarm_Error_Occurrence( 2009, CTL_dWARNING, alarm.mstr_code);
		}
		break;

	case 1700:
// 		nRet_1 = g_ioMgr.get_in_bit(stIO.i_Chk_GripperSafetyPos,IO_OFF);
// 		if (nRet_1 == IO_OFF)
// 		{
// 			m_nBoxLift_Step = 300;
// 		}
// 		else
// 		{
// 			//040000 0 00 "LIFTER_Y1_SAFETY_NOT_EXIST_ERR."
// 			CTL_Lib.Alarm_Error_Occurrence( 1300, CTL_dWARNING, "040000");
// 		}
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
			m_nBoxLift_Step = 1800;
		}
		else
		{
			//150000 0 00 "ACCY_UNLOADER_FULL_ERR."
			CTL_Lib.Alarm_Error_Occurrence( 2010, CTL_dWARNING, "150000");
		}
		break;
	//Avoid 위치로 빠진 상태. Lift Up 상태 확인  Elevator Front 위치 이동 
	case 1800:
		if (stSync.nReq_Clamp2Lifter_Work == SYNC_REQ_RESET_)
		{
			m_nBoxLift_Step = 2000;
		}
		break;

	//아무 것도 없는 것 확인 하고 없으면 Y Robot 이동.
// 	case 1000:
// 		nRet_1 = g_ioMgr.get_in_bit(stIO.i_Chk_GripperSafetyPos,IO_OFF);
// 		if (nRet_1 == IO_OFF)
// 		{
// 			m_nBoxLift_Step = 2000;
// 		}
// 		else
// 		{
// 		 	//040000 0 00 "LIFTER_Y1_SAFETY_NOT_EXIST_ERR."
// 		 	CTL_Lib.Alarm_Error_Occurrence( 1300, CTL_dWARNING, "040000");
// 		}
// 		break;
	
		//작업 위치 이동.
	case 2000:
		nRet_1 = CTL_Lib.OnSingleMove( m_nMotY, st_motor[m_nMotY].d_pos[Y_ACC_FRONT_], (int)st_handler.md_run_speed );
		if( nRet_1 == CTLBD_RET_GOOD )
		{
				m_nBoxLift_Step = 3000;
		}
		else if( nRet_1 == CTLBD_RET_ERROR || nRet_1 == CTLBD_RET_RETRY )
		{
			//042000 0 00 "LIFTER_Y1_WORK_AREA_MOVE_ERR."
			alarm.mstr_code		= "042000";
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
			CTL_Lib.Alarm_Error_Occurrence( 2011, CTL_dWARNING, alarm.mstr_code);
		}
		break;

	case 3000:
		stSync.nResp_Lifter2Clamp_Work = SYNC_REQ_RESET_;
		m_nBoxLift_Step = 0;
		break;
	}
}

bool CSeq_BoxLifter::OnRun_Remove() 
{

	return true;

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
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
		}
		else if (nRetData[1] == CTLBD_RET_ERROR || nRetData[1] == CTLBD_RET_SAFETY)
		{
			// 011001 0 00 "LIFTER_UP_DOWN_SHIFT2_Z_ACC_LIFT_SAFETY_MOVE_ERR."
			alarm.mstr_code		= "011001";
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
		}
		else if (nRetData[2] == CTLBD_RET_ERROR || nRetData[2] == CTLBD_RET_SAFETY)
		{
			// 021001 0 00 "LIFTER_UP_DOWN_SHIFT3_Z_ACC_LIFT_SAFETY_MOVE_ERR."
			alarm.mstr_code		= "021001";
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
		}
		else if (nRetData[3] == CTLBD_RET_ERROR || nRetData[3] == CTLBD_RET_SAFETY)
		{
			// 031001 0 00 "LIFTER_UP_DOWN_SHIFT4_Z_ACC_LIFT_SAFETY_MOVE_ERR."
			alarm.mstr_code		= "031001";
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
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

	// yangkyu 2015.02.11
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
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
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
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
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

	
	return true;
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
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
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
	return true;

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
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
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
	return true;
}

// yangkyu 2015.02.12
// 악세사리 재정렬을 위해 추가
int CSeq_BoxLifter::OnProc_ReAlign() 
{

	return true;
}