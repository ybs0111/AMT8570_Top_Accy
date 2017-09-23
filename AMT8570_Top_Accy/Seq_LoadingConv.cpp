// Seq_LoadingConv.cpp: implementation of the CSeq_LoadingConv class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "handler.h"
#include "Seq_LoadingConv.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CSeq_LoadingConv	SeqLoadingConv;

CSeq_LoadingConv::CSeq_LoadingConv()
{
	m_nThirdBoxYesNo = CTL_NO;

	//kwlee 2017.0711 test
	st_handler.nSimulation = 0;
	stSync.nReq_LoadingConv2Clamp_Work = -1;

}

CSeq_LoadingConv::~CSeq_LoadingConv()
{
	OnMove_AcMotor(AC_MV_STOP_);	// AC 모터 구동/정지

}

//==================================================================//
// 쓰레드 동작
//==================================================================//
void CSeq_LoadingConv::OnSeq_RUN(void) 
{
	//kwlee 2017.0711 test 
	//OnSeq_INIT();
	
	OnSeq_Execute();
}

//==================================================================//
// 쓰레드 관련 변수 초기화
//==================================================================//
void CSeq_LoadingConv::OnSeq_INIT(void) 
{
	if( m_bflag_INIT == true )
	{
		return;
	}
	m_bflag_INIT = true;

	m_nStep_Init = 0;
	m_nStep_Run = 0;
	m_nStep_Run2 = 0;

	m_nReq_AcMotCtrl[0] = m_nReq_AcMotCtrl[1] = REQ_MV_RESET_;

	//stSync.nRcvyComplete[SITE_BTM_ACCY_CONV_] = CTL_YES;

}

//==================================================================//
// 쓰레드 제어
//==================================================================//
void CSeq_LoadingConv::OnSeq_Execute(void) 
{
	
	//kwlee 2017.0711
	if (st_handler.nSimulation == 1)
	{
		st_basic.n_mode_device = WITH_DVC_;
		st_handler.nIsAllSiteInitEnd = CTL_YES;
		st_work.nEqpStatus = dRUN;
	}
	else
	{
		// 시컨스 인터럽트 조건 확인
		if (Func.OnIsInterruptCondition() == 1)
		{
			return;
		}
	}

	int nRet = CTL_NO;

	if( st_work.nEqpStatus != dRUN )
	{
		if ( m_nReq_AcMotCtrl[1] != REQ_MV_RESET_ &&
			 m_nReq_AcMotCtrl[1] != REQ_MV_INIT_ )
		{
			// [RUN] 상태로 변경되기 전의 맨 마지막 시간 정보 설정
			// - 이 정소는 [RUN] 상태로 변경되기까지 계속 갱신될 것임
			//m_lTime_GoesBy[0] = GetCurrentTime();
			//kwlee 2017.0706
			m_lTime_GoesBy[0][0] = GetCurrentTime();
		}
	}

	if( m_nReq_AcMotCtrl[1] == REQ_MV_THIRD_ )
	{
		nRet = OnCheck_AccyBoxConvInThird( IO_ON );
		if( nRet == IO_ON )
		{
			m_lTime_GoesBy[1][0] = GetCurrentTime();
			m_nReq_AcMotCtrl[1] = REQ_MV_STABILITY_;
		}
	}
	else if( m_nReq_AcMotCtrl[1] == REQ_MV_STABILITY_ )// After checking  if thers is accy box, load conv stop
	{
		m_lTime_GoesBy[1][1] = GetCurrentTime();
		m_lTime_GoesBy[1][2] = m_lTime_GoesBy[1][1] - m_lTime_GoesBy[1][0];
		if( m_lTime_GoesBy[1][2] <= 0 ) m_lTime_GoesBy[1][0] = GetCurrentTime();

		//if( m_lTime_GoesBy[1][2] >= st_time.nWait_On[WAIT_RAIL_AC_OFF])
		//kwlee 2017.00824
		if( m_lTime_GoesBy[1][2] >= st_time.nWait_Limit[WAIT_RAIL_AC_OFF])
		{
			OnMove_AcMotor2( AC_MV_STOP_ );
		}
	}
	
	if( m_nReq_AcMotCtrl[0] == REQ_MV_SECOND_ )
	{
		nRet = OnCheck_AccyBoxConvInSecond(IO_ON);
		if( nRet == IO_ON)
		{
			m_lTime_GoesBy[0][0] = GetCurrentTime();
			m_nReq_AcMotCtrl[0] = REQ_MV_STABILITY_;
		}
	}
	else if( m_nReq_AcMotCtrl[0] == REQ_MV_STABILITY_ )// After checking  if thers is accy box, load conv stop
	{
		m_lTime_GoesBy[0][1] = GetCurrentTime();
		m_lTime_GoesBy[0][2] = m_lTime_GoesBy[0][1] - m_lTime_GoesBy[0][0];
		if( m_lTime_GoesBy[0][2] <= 0 ) m_lTime_GoesBy[0][0] = GetCurrentTime();
		
		//if( m_lTime_GoesBy[0][2] >= st_time.nWait_On[WAIT_RAIL_AC_OFF])
		//kwlee 2017.0824
		if( m_lTime_GoesBy[0][2] >= st_time.nWait_Limit[WAIT_RAIL_AC_OFF])
		{
			OnMove_AcMotor( AC_MV_STOP_ );
		}
	}

	if( m_nReq_AcMotCtrl[1] == REQ_MV_INIT_ )
	{
		m_lTime_GoesBy[1][1] = GetCurrentTime();
		m_lTime_GoesBy[1][2] = m_lTime_GoesBy[1][1] - m_lTime_GoesBy[1][0];
		if( m_lTime_GoesBy[1][2] <= 0 ) m_lTime_GoesBy[1][0] = GetCurrentTime();
		if( m_lTime_GoesBy[1][2] > 5000 )
		{
			OnMove_AcMotor2( AC_MV_STOP_ );
		}
	}

	if( m_nReq_AcMotCtrl[0] == REQ_MV_INIT_ )
	{
		m_lTime_GoesBy[0][1] = GetCurrentTime();
		m_lTime_GoesBy[0][2] = m_lTime_GoesBy[0][1] - m_lTime_GoesBy[0][0];
		if( m_lTime_GoesBy[0][2] <= 0 ) m_lTime_GoesBy[0][0] = GetCurrentTime();
		if( m_lTime_GoesBy[0][2] > 5000 )
		{
			OnMove_AcMotor( AC_MV_STOP_ );
		}
	}

	switch( st_work.nEqpStatus )
	{

	case dINIT:
		if( st_handler.nInitTryREQ != YES )
		{
			break;
		}
		OnRun_Initial();
		break;

	case dRUN:
		if( st_handler.nIsAllSiteInitEnd == CTL_YES )
		{
// 			OnRun_Move2();
// 			OnRun_Move();
			
			OnRun_Move_Front();
			OnRun_Move_Rear();
		}
		break;

	default:
		if( st_work.nForceDischarge == FORCEDISCHARGE_ON )
		{
			m_nStep_Run = 0;
			//stSync.nReq_LoadingConv2Clamp_Work = SYNC_REQ_RESET_;
			//kwlee 2017.0706
			stSync.nReq_LoadingConv2Clamp_Work = SYNC_REQ_RESET_;
		}
		break;

	}

}

void CSeq_LoadingConv::OnRun_Initial() 
{

	int nRet[3] = {0,};
	
	if( st_handler.mn_init_state[INIT_LOAD_CONV] != CTL_NO )
	{
		return;
	}
	
	int nRet_1 = CTL_NO, nRet_2 = CTL_NO;
	
	switch( m_nStep_Init )
	{
	case 0:
		memset(&st_map, 0, sizeof(st_map));
		m_nStep_Init = 100;
		break;

	case 100:
		OnSet_CylStopper2( IO_ON );
		OnSet_CylStopper( IO_OFF );
		m_nStep_Init = 110;
		break;

	case 110:
		nRet_2 = OnGet_CylStopper2( IO_ON );
		nRet_1 = OnGet_CylStopper( IO_OFF );
		if( nRet_1 == CTL_GOOD && nRet_2 == CTL_GOOD )
		{
			m_nStep_Init = 200;
		}
		else if( nRet_1 == CTL_ERROR || nRet_2 == CTL_ERROR )
		{
			CTL_Lib.Alarm_Error_Occurrence( 5000, CTL_dWARNING, m_strAlarmCode );
			m_nStep_Init = 100;
		}
		break;

	case 200:
		OnMove_AcMotor2(AC_MV_STOP_);
		OnMove_AcMotor(AC_MV_STOP_);
		//m_lTime_GoesBy[0][0] = GetCurrentTime(); kwlee 2017.0707
		m_nStep_Init = 400;
		break;

		
	case 400:
		// [Manual Loading] 상태 변경
		//Func.OnSet_EqpLoadingManual();
		//kwlee 2017.0706
		nRet[0] = g_ioMgr.get_in_bit(stIO.i_Chk_AccyBoxConvDetection_First);
		nRet[1] = g_ioMgr.get_in_bit(stIO.i_Chk_AccyBoxConvDetection_Second);
		nRet[2] = g_ioMgr.get_in_bit(stIO.i_Chk_AccyBoxConvDetection_Third);

		if (nRet[0] == IO_ON && nRet[1] == IO_ON && nRet[2] == IO_ON)
		{
			//m_nStep_Init = 500;
			//kwlee 2017.0712
			m_nStep_Init = 1000;
		}
		else
		{
			if (nRet[0] == IO_OFF)
			{
				m_strAlarmCode = "120010";
				
			}
			else if (nRet[1] == IO_OFF)
			{
				m_strAlarmCode = "120011";
			}
			else if (nRet[2] == IO_OFF)
			{
				m_strAlarmCode = "120012";
			}

			CTL_Lib.Alarm_Error_Occurrence( 5001, CTL_dWARNING, m_strAlarmCode );
		}		
		break;

	//kwlee 2017.0707
// 	case 500:
// 		OnSet_CylBoxClamp(IO_OFF);
// 		m_nStep_Init = 600;
// 		break;
// 
// 	case 600:
// 		nRet_1 = OnGet_CylBoxClamp(IO_OFF);
// 		if (nRet_1 == CTL_GOOD)
// 		{
// 			m_nStep_Init = 700;
// 		}
// 		else if (nRet_1 == CTL_ERROR)
// 		{
// 			CTL_Lib.Alarm_Error_Occurrence( 1020, CTL_dWARNING, m_strAlarmCode );
// 		}
// 		break;
// 
// 	case 700:
// 		OnSet_CylBoxPusher(IO_OFF);
// 		m_nStep_Init = 800;
// 		break;
// 
// 	case 800:
// 		nRet_1 = OnGet_CylBoxPusher(IO_OFF);
// 		
// 		if (nRet_1 == CTL_GOOD && nRet_2 == CTL_GOOD)
// 		{
// 			m_lTime_GoesBy[0][0] = GetCurrentTime();
// 			m_nStep_Init = 1000;
// 		}
// 		else if (nRet_1 == CTL_ERROR || nRet_2 == CTL_ERROR)
// 		{
// 			CTL_Lib.Alarm_Error_Occurrence( 1030, CTL_dWARNING, m_strAlarmCode );
// 			m_nStep_Init = 700;
// 		}
// 		break;

	case 1000:
		m_lTime_GoesBy[0][1] = GetCurrentTime();
		m_lTime_GoesBy[0][2] = m_lTime_GoesBy[0][1] - m_lTime_GoesBy[0][0];
		if( m_lTime_GoesBy[0][2] <= 0 ) m_lTime_GoesBy[0][0] = GetCurrentTime(); 
		if( m_lTime_GoesBy[0][2] < 2000) break;

		stSync.nReq_LoadingConv2Clamp_Work = SYNC_REQ_RESET_;
		//stSync.nReq_LoadingConv2Clamp_Work[1] = SYNC_REQ_RESET_;
		st_handler.mn_init_state[INIT_LOAD_CONV] = CTL_READY;
		Func.OnSet_EqpManual();
		m_nStep_Init = 0;
		break;
	
	}
}

//LOOP : stopper2는 up된 상태이고 stopper1은 down상태에서 conv2, conv1이동
//       3번째 conv에서 자재 감지되면 stopper1을 up한다 자재 자재 준비됐다고 BoxClamp이 알림
//       conv에서는 자재를 2,1이 자 준지될때까지 받는다. conv에서는 자재를 2,1이 자 준지될때까지 받는다.(이 부분이 다른 부분과 상충된다 stopper 때문)
//		 BOXCLAMP가 complete상태에서는 다른 쓰레드함수를 동작하는 낫다
//       BoxClamp가 자재를 다 가지고 가면 다시 위로 Loop한다.

void CSeq_LoadingConv::OnRun_Move_Front()
{

	int nRetData = CTL_NO;
	int nRet_1 = CTL_NO, nRet_2 = CTL_NO;

	if (st_handler.nSimulation == 1)
	{
		//st_work.nEqpStatus = dRUN;
	}
	else 
	{
		if (Func.OnIsAllRcvyComplete() != CTL_YES)
		{
			return;
		}

		if (st_work.nLoadingStatus == dMANUAL)
		{
			st_work.nLoadingFlag = TRUE;
			return;
		}
		else
		{
			if (st_basic.n_mode_device == WITHOUT_DVC_)
			{
				if (stWithoutData.nLoadingTestBox <= 0)
				{
					st_map.nLoadingAccyBoxExist[0] = 1;//Third
					st_map.nLoadingAccyBoxExist[1] = 1;//Second
					st_map.nLoadingAccyBoxExist[2] = 1;//First
					stWithoutData.nLoadingTestBox = 3;
				}
			}
		}
	}
	Func.OnTrace_ThreadStep(1, m_nStep_Run2 );

	switch( m_nStep_Run2 )
	{
	case 0:
		m_nStep_Run2 = 100;
		break;

	case 100:
		OnSet_CylStopper2( IO_ON );
		m_nStep_Run2 = 200;
		break;

	case 200:
		nRet_1 = OnGet_CylStopper2( IO_ON );
		
		if( nRet_1 == CTL_GOOD )
		{
			//m_nStep_Run2 = 300;
			//kwlee 2017.0712
			m_nStep_Run2 = 400;
		}
		//

		else if( nRet_1 == CTL_ERROR )
		{
			CTL_Lib.Alarm_Error_Occurrence( 5002, CTL_dWARNING, m_strAlarmCode );
			m_nStep_Run2 = 100;
		}
		break;

	case 400:
		if( st_basic.n_mode_device == WITHOUT_DVC_ )
		{
			if( stWithoutData.nLoadingTestBox == 0 )
			{
				if( st_map.nLoadingAccyBoxExist[0] == 0 )// Accy Box Conv에 있는 자재 확인 변수
				{
					break;
				}
			}
			else
			{
				m_nStep_Run2 = 200;
				break;
			}
			m_nStep_Run2 = 100;
			break;
		}
		else //WITH MODE 일때 
		{
			//kwlee 2017.0711
			if (st_handler.nSimulation == 1)
			{
				cLOG.OnLogEvent(LOG_SEQ_, "BoxConv Third Sensor Check : IO_OFF ----Ready----- ");
				nRetData = IO_OFF;
			}
			else
			{
				nRetData = OnCheck_AccyBoxConvInThird( IO_ON ); 
			}
			
			//Right Conv(Cov_2) 센서에 Box 감지됨.
			//있으면 대기..
			if( nRetData == IO_ON ) 
			{
				m_lWaitTime[0][0] = GetCurrentTime();
				m_nStep_Run2 = 500;
			}
			else //Box 감지 안됨.  Box 받는다.
			{
				if( nRetData == IO_OFF )
				{
// 					if( stSync.nLotEnd_LoadingConv != TRUE )
// 					{
// 						stSync.nLotEnd_LoadingConv = TRUE;
					//kwlee 2017.0706
					if( stSync.nLotEnd_LoadingConv != TRUE )
					{
						stSync.nLotEnd_LoadingConv = TRUE;
						OnMove_AcMotor2( AC_MV_CW_ );
						//m_nReq_AcMotCtrl[1] = REQ_MV_THIRD_;//일정한시간후에 stop..그전에 각단계에서 센서 감지되면..
						//kwlee 2017.0830
						//m_lTime_GoesBy[1][1] = GetCurrentTime();
						m_nReq_AcMotCtrl[1] = REQ_MV_THIRD_;
						stSync.nReq_LoadingConv2Clamp_Work = SYNC_REQ_RESET_;	
					}
					else
					{
						stSync.nReq_LoadingConv2Clamp_Work = SYNC_REQ_RESET_;
					}
					st_map.nLotStart = CTL_YES;
					stSync.nLotEnd_LoadingConv = FALSE;
					m_lWaitTime[0][0] = GetCurrentTime();
					//m_nStep_Run2 = 200;
					//kwlee 2017.0709
					stSync.nReq_LoadingConv2Conv1_Work = CTL_REQ; //뒷단 Conv에 투입 요청.
					//kwlee 2017.0711
					cLOG.OnLogEvent(LOG_SEQ_, "1. Front Conv(Conv_2)가 Rear Conv_1에게  투입 요청(REQ) 보냄. ----Start----  ");
					m_nStep_Run2 = 900;
				//kwlee 2017.0707 
// 				stSync.nReq_LoadingConv2Conv1_Work = CTL_REQ;
// 				
				}
// 				st_map.nLotStart = CTL_YES;
// 				//stSync.nLotEnd_LoadingConv = FALSE;
// 				//m_nStep_Run2 = 200;
// 				//kwlee 2017.0706
// 				stSync.nLotEnd_LoadingConv[1] = FALSE;	
// 				m_nStep_Run2 = 100;
			}
		}
		break;

		//Box Check 센서 감지 되었을 때.
	case 500:
		m_lWaitTime[0][1] = GetCurrentTime();
		m_lWaitTime[0][2] = m_lWaitTime[0][1] - m_lWaitTime[0][0];
		if( m_lWaitTime[0][2] <= 0 ) m_lWaitTime[0][0] = GetCurrentTime();

		if( st_basic.n_mode_device == WITHOUT_DVC_ )
		{
			if( stWithoutData.nLoadingTestBox == 0 )
			{
				if( st_map.nLoadingAccyBoxExist[0] == 0 )
				{
					break;
				}
			}
			else
			{
				m_nStep_Run2 = 200;
				break;
			}
			m_nStep_Run2 = 100;
			break;
		}
		else //With Mode 일때
		{
			nRetData = OnCheck_AccyBoxConvInThird( IO_ON );
			if( nRetData == IO_ON )
			{
				if( m_lWaitTime[0][2] < 1000 ) break;

			//	OnSet_CylStopper( IO_ON ); 
				//m_nStep_Run2 = 70;
				//kwlee 2017.0709
				m_nStep_Run2 = 1020;
			}	
			else
			{
// 				if( stSync.nLotEnd_LoadingConv != TRUE )
// 				{
// 					stSync.nLotEnd_LoadingConv = TRUE;
				//kwlee 2017.0706
				if( stSync.nLotEnd_LoadingConv != TRUE )
				{
					stSync.nLotEnd_LoadingConv = TRUE;
					OnMove_AcMotor2( AC_MV_CW_ );
					//m_nReq_AcMotCtrl[1] = REQ_MV_THIRD_;
					//kwlee 2017.0830
					m_lTime_GoesBy[1][1] = GetCurrentTime();
					m_nReq_AcMotCtrl[1] = REQ_MV_STABILITY_;
					stSync.nReq_LoadingConv2Clamp_Work = SYNC_REQ_RESET_;
				}
				else
				{
					stSync.nReq_LoadingConv2Clamp_Work = SYNC_REQ_RESET_;
				}
				st_map.nLotStart = CTL_YES;
				stSync.nLotEnd_LoadingConv = FALSE;
				m_nStep_Run2 = 800;
			}
		}
		break;


	//case 100:
	//kwlee 2017.0711
	case 800:
		if (m_nReq_AcMotCtrl[1] == REQ_MV_RESET_)
		{		
			nRetData = OnCheck_AccyBoxConvInThird( IO_ON );
			if( st_basic.n_mode_device == WITHOUT_DVC_ )
			{
				nRetData = st_map.nLoadingAccyBoxExist[0];
			}

			if( nRetData == IO_OFF )
			{
				stSync.nReq_LoadingConv2Clamp_Work = SYNC_REQ_RESET_;
				m_nStep_Run2 = 0;
			}
			else
			{
				//m_nStep_Run2 = 1000;
				m_lWaitTime[0][0] = GetCurrentTime(); //kwlee 2017.0711
				m_nStep_Run2 = 900;
			}
		}
		break;

	case 900:
		//stSync.nReq_LoadingConv2Conv2_Work = CTL_REQ;
		//kwlee 2017.0707
	//	stSync.nReq_LoadingConv2Conv1_Work = CTL_REQ;
		if (stSync.nReq_LoadingConv2Conv1_Work == CTL_READY)
		{
			OnMove_AcMotor2( AC_MV_CW_ );
			//kwlee 2017.0830
			m_lTime_GoesBy[1][0] = GetCurrentTime();
 			m_nReq_AcMotCtrl[1] = REQ_MV_STABILITY_;

			cLOG.OnLogEvent(LOG_SEQ_, "4.Left Conv(Conv_1)이 Right Conv(Conv_2)에게 Ready 받음.   ---ING------  ");
			m_nStep_Run2 = 1000;
		}
		//kwlee 2017.0712
		//else if (stSync.nReq_LoadingConv2Clamp_Work == SYNC_REQ_RESET_)
		//kwlee 2017.0823
		else /*if (stSync.nReq_LoadingConv2Conv1_Work == SYNC_REQ_RESET_)*/
		{
			//Retry
			OnMove_AcMotor2( AC_MV_STOP_ ); //kwlee 2017.0917

			m_lWaitTime[0][1] = GetCurrentTime();
			m_lWaitTime[0][2] = m_lWaitTime[0][1] - m_lWaitTime[0][0];
			if( m_lWaitTime[0][2] <= 0 ) m_lWaitTime[0][0] = GetCurrentTime();
			if (m_lWaitTime[0][2] > MAX_WAIT_ACCY_BOX_CONV_ * 100)
			{
				m_nRetryCnt++;
				if (m_nRetryCnt > 3)
				{
					CTL_Lib.Alarm_Error_Occurrence( 5003, CTL_dWARNING, "900004" );
					m_lWaitTime[0][0] = GetCurrentTime();
					m_nRetryCnt = 0;
				}
				
			}
			
		}
		break;

	case 1000:
		//센서 감지시 멈춤.
		//OnMove_AcMotor2( AC_MV_CW_ );
		m_lWaitTime[0][0] = GetCurrentTime();
		//m_nReq_AcMotCtrl[1] = REQ_MV_THIRD_;
		
		
		cLOG.OnLogEvent(LOG_SEQ_, "5.Right Conv(Conv_2) 모터 온. ---ING------  ");
		m_nStep_Run2 = 1010;
		break;

		//Front Conv가 Rear Conv에게 Complete신호 준다.
	case 1010:
		nRetData = OnCheck_AccyBoxConvInThird( IO_ON );
		if (nRetData == IO_ON)
		{
			cLOG.OnLogEvent(LOG_SEQ_, "6. Right Conv(Conv_2)이 Left Conv(Conv_1)에게 Complete 보냄.. ---ING------  ");
			stSync.nReq_LoadingConv2Conv1_Work = CTL_COMPLETE;
			m_nStep_Run2 = 1020;
		}
		break;

	case 1020:
	
		if( m_nReq_AcMotCtrl[1] == REQ_MV_RESET_ )
		{
			if( st_map.nLoadingAccyBoxExist[0] == 0 )
			{
				st_map.nLoadingAccyBoxExist[0] = 1;
			}
			else
			{
				st_map.nLoadingAccyBoxExist[0] = 1;
			}

			//stSync.nReq_LoadingConv2Clamp_Work[1] = SYNC_REQ_ACCY_BOX_LOADING_COMPLETE_;

			//m_nStep_Run2 = 3000;
			
			//kwlee 2017.0709
			//감지시 COMPLETE, Conv_1,Conv_2  MOTOR STOP

			//stSync.nReq_LoadingConv2Conv1_Work = CTL_COMPLETE;
			stSync.nReq_LoadingConv2Clamp_Work = SYNC_REQ_ACCY_BOX_LOADING_COMPLETE_;			

			//m_nStep_Run2 = 1020;
			m_nStep_Run2 = 1100;
		}
		else
		{
			if( st_work.nSkipReq_ShiftTray == CTL_YES )
			{
				OnMove_AcMotor2( AC_MV_CW_ );
				m_nStep_Run2 = 3000;
			}
			m_lWaitTime[0][1] = GetCurrentTime();
			m_lWaitTime[0][2] = m_lWaitTime[0][1] - m_lWaitTime[0][0];
			if( m_lWaitTime[0][2] <= 0 ) m_lWaitTime[0][0] = GetCurrentTime();

			if( st_basic.n_mode_device == WITHOUT_DVC_ )
			{
				if( m_lWaitTime[0][2] >= ( MAX_WAIT_ACCY_BOX_CONV_ / 2 ) )
				{
					OnMove_AcMotor2(AC_MV_STOP_);
					if( st_map.nLoadingAccyBoxExist[0] == 0 )
					{
						st_map.nLoadingAccyBoxExist[0] = 1;
					}
					else
					{
						st_map.nLoadingAccyBoxExist[0] = 1;
					}
				}
			}
			else
			{
				if ( m_lWaitTime[0][2] >= (MAX_WAIT_ACCY_BOX_CONV_ * 10) )
				{
					OnMove_AcMotor2( AC_MV_STOP_ );
					m_nStep_Run2 = 1000;
					
					// 120012 0 00 "PS0308_LOADING_CONV_THIRD_CHK_ERR."
					CTL_Lib.Alarm_Error_Occurrence( 5004, CTL_dWARNING, "120012" );
				}
			}
		}
		break;

	//kwlee 2017.0709
	case 1100:
		//Box Clamp가 투입 요청 
		nRetData = OnCheck_AccyBoxConvInThird( IO_ON );
		if( nRetData == IO_OFF )
		{
			m_nStep_Run2 = 1020;
		}
		else
		{
			//Conv_1 Stopper Up 후 Conv_1 Motor On
			//Box Clamp 작업 요청 
			stSync.nReq_LoadingConv2Conv1_Work = CTL_SORT; 

		//	cLOG.OnLogEvent(LOG_SEQ_, "9. Conv_2 Motor On 하여 Box 앞으로 붙임. Sort 신호 보냄.   ---ING------  ");

			if (stSync.nResp_Clamp2LoadingConv_Work == SYNC_RESP_WORK_)
			{
				m_nStep_Run2 = 1200;
			}
		}
		break;

	case 1200:
		OnSet_CylStopper2( IO_OFF );
		m_nStep_Run2 = 1300;
		break;
		
	case 1300:
		nRet_1 = OnGet_CylStopper2( IO_OFF );

		if( nRet_1 == CTL_GOOD )
		{
			//m_nStep_Run2 = 1400;
			//kwlee 2017.0712
			//m_nStep_Run2 = 3000;
			stSync.nResp_Clamp2LoadingConv_Work = SYNC_RESP_BOX_LIFT_CHANGE_COMPLETE_; //kwlee 2017.0824
			m_nStep_Run2 = 2000;

		}
		else if( nRet_1 == CTL_ERROR )
		{
			CTL_Lib.Alarm_Error_Occurrence(5005, CTL_dWARNING, m_strAlarmCode);
			m_nStep_Run2 = 1200;
		}
		break;

	//kwlee 2017.0712 BoxClamp Clamp  완료.
	case  2000:
		if (stSync.nResp_Clamp2LoadingConv_Work == SYNC_RESP_RESET_)
		{
			m_nStep_Run2 = 3000;
		}
		break;

	case 3000:
		if( st_work.nSkipReq_ShiftTray == CTL_YES )
		{
			st_work.nSkipReq_ShiftTray = CTL_NO;
		}
		m_nStep_Run2 = 0;
		break;

	}

}

void CSeq_LoadingConv::OnRun_Move_Rear() 
{
	if (Func.OnIsAllRcvyComplete() != CTL_YES)
	{
		return;
	}

	if (st_work.nLoadingStatus == dMANUAL)
	{
		st_work.nLoadingFlag = TRUE;
		return;
	}
	

	Func.OnTrace_ThreadStep(0, m_nStep_Run );

	int nRet_1 = CTL_NO, nRet_2 = CTL_NO;
	int nRetData[4] = {0,};

	switch( m_nStep_Run )
	{
	case 0:
		m_nStep_Run = 100;
		break;

	case 100:
		//Stopper Up 상태.
		st_work.nSkipReq_ShiftTray = CTL_NO;
		//m_nStep_Run = 20;
		//kwlee 2017.07110
		OnSet_CylStopper(IO_ON);
		m_nStep_Run = 110;
		break;

	case 110:
		nRet_1 = OnGet_CylStopper(IO_ON);
		
		if (nRet_1 == CTL_GOOD)
		{
			m_nStep_Run = 200;
		}
		else if (nRet_1 == CTL_ERROR)
		{
			CTL_Lib.Alarm_Error_Occurrence( 5006, CTL_dWARNING, m_strAlarmCode );
		}
		break;


	case 200:
		if( st_basic.n_mode_device == WITHOUT_DVC_)
		{
			if( stWithoutData.nLoadingTestBox == 0 )
			{
				if( st_map.nLoadingAccyBoxExist[1] == 0 && // Accy Box Conv에 있는 자재 확인 변수
					st_map.nLoadingAccyBoxExist[2] == 0 )
				{
					break;
				}
			}
			else
			{
				//m_nStep_Run = 200;
				m_nStep_Run = 900;
				break;
			}
			//m_nStep_Run = 100;
			m_nStep_Run = 800;
			break;
		}
		else
		{
			nRetData[1] = OnCheck_AccyBoxConvInSecond( IO_ON );
			nRetData[2] = OnCheck_AccyBoxConvInFirst( IO_ON );
			if( nRetData[1] == IO_ON )
			{
				m_lWaitTime[1][0] = GetCurrentTime();
				//kwlee 2017.0710 
				OnMove_AcMotor( AC_MV_CW_ );
				m_nReq_AcMotCtrl[0] = REQ_MV_STABILITY_;
				m_lTime_GoesBy[0][0] = GetCurrentTime();
				m_nStep_Run = 500;
			}
			else
			{
				//Conv_1자재 없으면 모터 온.
// 				if( stSync.nLotEnd_LoadingConv != TRUE )
// 				{
// 					stSync.nLotEnd_LoadingConv = TRUE;
				//kwlee 2017.0706
				if( stSync.nLotEnd_LoadingConv != TRUE )
				{
					stSync.nLotEnd_LoadingConv = TRUE;
					OnMove_AcMotor( AC_MV_CW_ );
					//kwlee 2017.0824
					m_nReq_AcMotCtrl[0] = REQ_MV_STABILITY_;
					m_lTime_GoesBy[0][0] = GetCurrentTime();
					stSync.nReq_LoadingConv2Clamp_Work = SYNC_REQ_RESET_;
				}
				else
				{
					stSync.nReq_LoadingConv2Clamp_Work = SYNC_REQ_RESET_;
				}
				st_map.nLotStart = CTL_YES;
				//stSync.nLotEnd_LoadingConv = FALSE;
				//kwlee 2017.0706
				stSync.nLotEnd_LoadingConv = FALSE;
				//kwlee 2017.0824
				m_nStep_Run = 600;
			}
		}
		break;

	case 500:
		m_lWaitTime[1][1] = GetCurrentTime();
		m_lWaitTime[1][2] = m_lWaitTime[1][1] - m_lWaitTime[1][0];
		if (m_lTime_GoesBy[0][2] <= 0) m_lTime_GoesBy[0][0] = GetCurrentTime();
		
		if( st_basic.n_mode_device == WITHOUT_DVC_ )
		{
			if( stWithoutData.nLoadingTestBox == 0 )
			{
				if( st_map.nLoadingAccyBoxExist[1] == 0 &&
					st_map.nLoadingAccyBoxExist[2] == 0 )
				{
					break;
				}
			}
			else
			{
				m_nStep_Run = 900;
				break;
			}
			m_nStep_Run = 800;
			break;
		}
		else
		{
			if (m_nReq_AcMotCtrl[0] == REQ_MV_RESET_) //kwlee 2017.0711
			{
				nRetData[1] = OnCheck_AccyBoxConvInSecond( IO_ON );
				nRetData[2] = OnCheck_AccyBoxConvInFirst( IO_ON );
				
				if( nRetData[1] == IO_ON )
				{
					if( m_lWaitTime[1][2] < 500) break;//stick box at third
					m_lWaitTime[1][0] = GetCurrentTime();
					//m_nStep_Run = 70;
					//kwlee 2017.0710
					m_nStep_Run = 600;
				}
				else
				{
	// 				if( stSync.nLotEnd_LoadingConv != TRUE )
	// 				{
	// 					stSync.nLotEnd_LoadingConv = TRUE;
					//kwlee 2017.0706
					if( stSync.nLotEnd_LoadingConv != TRUE )
					{
						stSync.nLotEnd_LoadingConv = TRUE;
						OnMove_AcMotor( AC_MV_CW_ );
						m_nReq_AcMotCtrl[0] = REQ_MV_SECOND_;
						stSync.nReq_LoadingConv2Clamp_Work = SYNC_REQ_RESET_;
					}
					else
					{
						stSync.nReq_LoadingConv2Clamp_Work = SYNC_REQ_RESET_;
					}
					st_map.nLotStart = CTL_YES;
					//stSync.nLotEnd_LoadingConv = FALSE;
					//kwlee 2017.0706
					stSync.nLotEnd_LoadingConv = FALSE;

					m_nStep_Run = 900;
				}
			}
		}
		break;
	
		//kwlee 2017.0710
	case 600:
		if( stSync.nReq_LoadingConv2Conv1_Work == CTL_REQ )
		{
			//kwlee 2017.0711
			cLOG.OnLogEvent(LOG_SEQ_, "2. Rear Conv가  Front Conv에게 투입 요청 (REQ) 받음. ---ING------  ");
			//OnSet_CylStopper(IO_OFF);
			//m_nStep_Run = 700;
			m_nStep_Run = 610;
		}
		break;

	case 610:
		OnSet_CylStopper(IO_OFF);
		m_nStep_Run = 700;
		break;

	case 700:
		nRet_1 = OnGet_CylStopper(IO_OFF);
	
		if (nRet_1 == CTL_GOOD)
		{
			m_nStep_Run = 710;
		}
		else if (nRet_1 == CTL_ERROR)
		{
			CTL_Lib.Alarm_Error_Occurrence( 5007, CTL_dWARNING, m_strAlarmCode );
			m_nStep_Run = 610;
		}
		break;

	case 710:
		if( st_basic.n_mode_device == WITHOUT_DVC_ )
		{
			if( stWithoutData.nLoadingTestBox == 0 )
			{
				if( st_map.nLoadingAccyBoxExist[1] == 0 &&
					st_map.nLoadingAccyBoxExist[2] == 0 )
				{
					break;
				}
			}
			else
			{
				m_nStep_Run = 900;
				break;
			}
			m_nStep_Run = 800;
		}
		else
		{
			nRetData[1] = OnCheck_AccyBoxConvInSecond( IO_ON );
			nRetData[2] = OnCheck_AccyBoxConvInFirst( IO_ON );	

			//if( nRetData[1] == IO_ON)
			//kwlee 2017.0915
			if( nRetData[1] == IO_ON || nRetData[2] == IO_ON)
			{
// 				if( stSync.nReq_LoadingConv2Conv2_Work == CTL_REQ )
// 				{
// 					stSync.nReq_LoadingConv2Conv2_Work = CTL_READY;
				//kwlee 2017.0707
// 				if( stSync.nReq_LoadingConv2Conv1_Work == CTL_REQ )
// 				{
				OnMove_AcMotor( AC_MV_CW_ );
				//kwlee 2017.0824
				//m_nReq_AcMotCtrl[0] = REQ_MV_SECOND_; '
				m_lTime_GoesBy[0][0] = GetCurrentTime();
				m_nReq_AcMotCtrl[0] = REQ_MV_STABILITY_;
				
				stSync.nReq_LoadingConv2Conv1_Work = CTL_READY;

				//kwlee 2017.0711 test
				cLOG.OnLogEvent(LOG_SEQ_, "3. Left Conv(Conv_1)이 Right Conv(Conv_2)에게 Ready 보냄  ---ING------  ");
				m_nStep_Run = 720;
	//			}
				//stSync.nReq_LoadingConv2Clamp_Work[0] = SYNC_REQ_ACCY_BOX_LOADING_COMPLETE_;
			}
			else
			{
				if( m_nReq_AcMotCtrl[0] == REQ_MV_RESET_)
				{
					if( stSync.nLotEnd_LoadingConv != TRUE )
					{
						stSync.nLotEnd_LoadingConv = TRUE;
						//kwlee 2017.0830
// 						OnMove_AcMotor( AC_MV_CW_ );
// 						//m_nReq_AcMotCtrl[0] = REQ_MV_SECOND_;
// 						//kwlee 2017.0830
// 						m_lTime_GoesBy[0][0] = GetCurrentTime();
// 						m_nReq_AcMotCtrl[0] = REQ_MV_STABILITY_;
						stSync.nReq_LoadingConv2Clamp_Work = SYNC_REQ_RESET_;
					}
					else
					{
						st_map.nLotStart = CTL_YES;
						stSync.nLotEnd_LoadingConv = FALSE;
						stSync.nReq_LoadingConv2Clamp_Work = SYNC_REQ_RESET_;
					}
					
					//stSync.nLotEnd_LoadingConv = FALSE;
					//kwlee 2017.0706
// 					st_map.nLotStart = CTL_YES;
// 					stSync.nLotEnd_LoadingConv = FALSE;
					//m_nStep_Run = 200;
					//kwlee 2017.0823
				//	m_nStep_Run = 0;

					//kwlee 20170917
// 					st_map.nLotStart = CTL_YES;
// 					stSync.nLotEnd_LoadingConv = FALSE;
				}
			}			
		}
		break;
		
		//Box 붙어서 들어올지도 몰라서 Conv_2 감지시 모터 정지후 Conv_1 Stopper Up 후 conv_2 진행.
	case 720:
			if (m_nReq_AcMotCtrl[0] == REQ_MV_RESET_) //kwlee 2017.0711
			{
				if (stSync.nReq_LoadingConv2Conv1_Work == CTL_COMPLETE)
				{
					cLOG.OnLogEvent(LOG_SEQ_, "7.Left Conv(Conv_1)이 Right Conv(Conv_2)에게 Complete 받음..Motor Stop   ---ING------  ");
					m_lWaitTime[1][0] = GetCurrentTime();

					//OnMove_AcMotor( AC_MV_STOP_ ); 
					//kwlee 2017.0824
					nRetData[1] = OnCheck_AccyBoxConvInThird( IO_ON );
					if (nRetData[1] == IO_ON)
					{
						OnMove_AcMotor( AC_MV_STOP_ );
					}
					//
					m_nStep_Run = 900;
				}
			}
			break;

	//case 100:
	//kwlee 2017.0711
	case 800:
		if (m_nReq_AcMotCtrl[0] == REQ_MV_RESET_) //kwlee 2017.0711
		{
			nRetData[1] = OnCheck_AccyBoxConvInSecond( IO_ON );
			nRetData[2] = OnCheck_AccyBoxConvInFirst( IO_ON );
			if( st_basic.n_mode_device == WITHOUT_DVC_ )
			{
				nRetData[1] = st_map.nLoadingAccyBoxExist[1];
				nRetData[2] = st_map.nLoadingAccyBoxExist[2];
			}
			
			if( nRetData[1] == IO_OFF )
			{
				if( nRetData[2] == IO_OFF )
				{
					OnMove_AcMotor( AC_MV_CW_);
					//m_nReq_AcMotCtrl[0] = REQ_MV_SECOND_;
					//kwlee 2017.0830
					m_lTime_GoesBy[0][0] = GetCurrentTime();
						m_nReq_AcMotCtrl[0] = REQ_MV_STABILITY_;
					stSync.nReq_LoadingConv2Clamp_Work = SYNC_REQ_RESET_;
					m_nStep_Run = 0;
					break;
				}
				stSync.nReq_LoadingConv2Clamp_Work = SYNC_REQ_RESET_;
				m_nStep_Run = 1000;
			}
			else
			{
				m_nStep_Run = 1000;
			}
		}
		
		break;

	//case 200://3Pos check
	//kwlee 2017.0711
	case 900://3Pos check
// 		OnSet_CylStopper( IO_ON );
		//BOx 붙어서 들올지 몰라서 Conv_2 먼저 모터 On하고 Conv_1 모터 ON
		if (m_nReq_AcMotCtrl[0] == REQ_MV_RESET_) //kwlee 2017.0711
		{		
			if (stSync.nReq_LoadingConv2Conv1_Work == CTL_SORT)
			{
				cLOG.OnLogEvent(LOG_SEQ_, "10. Right Conv(Conv_2)이 Left Conv(Conv_1)에게 Sort 신호 받음.. Left Conv_1 Stopper Up  ---ING------  ");
				OnSet_CylStopper(IO_ON);
				m_nStep_Run = 910;
			}
		}
		break;

	//case 210:
	//kwlee 2017.0711
	case 910:
		nRet_1 = OnGet_CylStopper( IO_ON );

		if( nRet_1 == CTL_GOOD )
		{
			cLOG.OnLogEvent(LOG_SEQ_, "11. Right Conv(Conv_2)이 Left Conv(Conv_1)에게 Sort 신호 받음..   ---END------  ");
			stSync.nReq_LoadingConv2Conv1_Work = CTL_CLEAR;
			m_nStep_Run = 1000;
		}
		else if( nRet_1 == CTL_ERROR )
		{
			CTL_Lib.Alarm_Error_Occurrence(5008, CTL_dWARNING, m_strAlarmCode);
			m_nStep_Run = 900;
		}
		break;

	case 1000:
		nRetData[1] = OnCheck_AccyBoxConvInSecond( IO_ON );
		nRetData[2] = OnCheck_AccyBoxConvInFirst( IO_ON );

		if( nRetData[1] == IO_ON /*&& nRetData[2] == IO_ON*/ )
		{
			m_nStep_Run = 3000;
		}
		else
		{
			OnMove_AcMotor( AC_MV_CW_);	
			m_nReq_AcMotCtrl[0] = REQ_MV_STABILITY_;
			m_lTime_GoesBy[1][0] = GetCurrentTime();
			m_nStep_Run = 1010;
		}
		break;

		
		//Conv의 구동 상태를 확인 후 악세사리 박스가 로딩이 완료 됬다는것을 Clamp에 요청
	case 1010:
		if( m_nReq_AcMotCtrl[0] == REQ_MV_RESET_ )
		{
			if( st_map.nLoadingAccyBoxExist[2] == 1 &&
				st_map.nLoadingAccyBoxExist[1] == 1 )
			{
				st_map.nLoadingAccyBoxExist[2] = 0;
				st_map.nLoadingAccyBoxExist[1] = 1;
			}
			else if( st_map.nLoadingAccyBoxExist[2] == 0 &&
				st_map.nLoadingAccyBoxExist[1] == 1 )
			{
				st_map.nLoadingAccyBoxExist[1] = 0;
			}
			else
			{
				st_map.nLoadingAccyBoxExist[1] = 1;
			}
			m_nStep_Run = 3000;
		}
		else
		{
			//conv box sensor error
			if( st_work.nSkipReq_ShiftTray == CTL_YES )
			{
				OnMove_AcMotor( AC_MV_CW_ );
				m_nStep_Run = 3000;
			}
			m_lWaitTime[1][1] = GetCurrentTime();
			m_lWaitTime[1][2] = m_lWaitTime[1][1] - m_lWaitTime[1][0];
			if( m_lWaitTime[1][2] <= 0 ) m_lWaitTime[1][0] = GetCurrentTime();

			if( st_basic.n_mode_device == WITHOUT_DVC_ )
			{
				if ( m_lWaitTime[1][2] >= ( MAX_WAIT_ACCY_BOX_CONV_ / 2 ) )
				{
					OnMove_AcMotor(AC_MV_STOP_);
					if( st_map.nLoadingAccyBoxExist[2] == 1 &&
						st_map.nLoadingAccyBoxExist[1] == 1 )
					{
						st_map.nLoadingAccyBoxExist[2] = 0;
						st_map.nLoadingAccyBoxExist[1] = 1;
					}
					else if( st_map.nLoadingAccyBoxExist[2] == 0 &&
						st_map.nLoadingAccyBoxExist[1] == 1 )
					{
						st_map.nLoadingAccyBoxExist[1] = 0;
					}
					else if( st_map.nLoadingAccyBoxExist[2] == 0 &&
						st_map.nLoadingAccyBoxExist[1] == 0 )
					{
					}
					else
					{
						st_map.nLoadingAccyBoxExist[1] = 1;
					}
					if( stWithoutData.nLoadingTestBox != 0 )
					{
						stWithoutData.nLoadingTestBox--;
					}
					m_nStep_Run = 3000;				
				}
			}
			else
			{
				if( m_lWaitTime[1][2] >= (MAX_WAIT_ACCY_BOX_CONV_ * 5))
				{
					OnMove_AcMotor( AC_MV_STOP_);
					m_nStep_Run = 1000;

					// 120010 0 00 "PS0306_LOADING_CONV_FIRST_CHK_ERR."
					CTL_Lib.Alarm_Error_Occurrence( 5009, CTL_dWARNING, "120010" );
				}
			}
		}
		break;

	case 3000:
		if (st_work.nSkipReq_ShiftTray == CTL_YES)//In case sensor error happened, user skip box
		{
			st_work.nSkipReq_ShiftTray = CTL_NO;
		}
		m_nStep_Run = 0;
		break;

	}
	
}

//==================================================================//
// AC 모터 구동/정지
//==================================================================//
void CSeq_LoadingConv::OnMove_AcMotor(int nzMoveType) 
{
	if (nzMoveType == AC_MV_CW_)
	{
		g_ioMgr.set_out_bit(stIO.o_AccyBoxConvMotor, IO_ON);
	}
	else if (nzMoveType == AC_MV_STOP_)
	{
		g_ioMgr.set_out_bit(stIO.o_AccyBoxConvMotor, IO_OFF);

		m_nReq_AcMotCtrl[0] = REQ_MV_RESET_;	// AC 모터 구동 요청 없음
	}
}

void CSeq_LoadingConv::OnMove_AcMotor2( int nzMoveType )
{
	if ( nzMoveType == AC_MV_CW_)
	{
		g_ioMgr.set_out_bit( stIO.o_AccyBoxConvMotor2, IO_ON );
	}
	else if( nzMoveType == AC_MV_STOP_ )
	{
		g_ioMgr.set_out_bit( stIO.o_AccyBoxConvMotor2, IO_OFF );
		m_nReq_AcMotCtrl[1] = REQ_MV_RESET_;
	}
}


//==================================================================//
// [Accy Box Conv First] 악세사리 박스가 존재하는지 확인
//==================================================================//
int CSeq_LoadingConv::OnCheck_AccyBoxConvInFirst(int nzOnOff, int nzMode) 
{
	int nReadIO = 0;

	// 센서가 B접이라 센서 상태 확인 시 반전 사용
	nReadIO = !g_ioMgr.get_in_bit(stIO.i_Chk_AccyBoxConvDetection[SIDE_FIRST_]);

	// 자재 존재하는 경우에 대한 체크인지 확인
	if (nzMode == 0)
	{
		// 현재 WithOut 모드인지 확인
		// - 자재없이 돌리고 있으므로 감지되지 않기 때문에 강제로 설정함
		if (st_basic.n_mode_device == WITHOUT_DVC_)
		{
			nReadIO = nzOnOff;
		}
	}
	else if (nzMode == 1)
	{
		// 현재 WithOut 모드인지 확인
		// - 자재없이 돌리고 있으므로 감지되지 않기 때문에 강제로 설정함
		if (st_basic.n_mode_device == WITHOUT_DVC_)
		{
			nReadIO = st_map.nLoadingAccyBoxExist[2];
		}
	}
	
	if (nReadIO == IO_ON)
	{
		return IO_ON;	// 센서 감지
	}
	else	
	{
		return IO_OFF;	// 센서 미감지
	} 
}

//==================================================================//
// [Accy Box Conv Second] 악세사리 박스가 존재하는지 확인
//==================================================================//
int CSeq_LoadingConv::OnCheck_AccyBoxConvInSecond(int nzOnOff, int nzMode) 
{
	int nReadIO;
	
	// 센서가 B접이라 센서 상태 확인 시 반전 사용
	nReadIO = !g_ioMgr.get_in_bit( stIO.i_Chk_AccyBoxConvDetection[SIDE_SECOND_] );
	
	// 자재 존재하는 경우에 대한 체크인지 확인
	if( nzMode == 0 )
	{
		// 현재 WithOut 모드인지 확인
		// - 자재없이 돌리고 있으므로 감지되지 않기 때문에 강제로 설정함
		if( st_basic.n_mode_device == WITHOUT_DVC_)
		{
			nReadIO = nzOnOff;
		}
	}
	else if ( nzMode == 1 )
	{
		if( st_basic.n_mode_device == WITHOUT_DVC_)
		{
			nReadIO = st_map.nLoadingAccyBoxExist[1];
		}
	}
	if( nReadIO == IO_ON )
	{
		return IO_ON; // 센서 감지
	}
	else
	{
		return IO_OFF;// 센서 미감지
	}
}

//==================================================================//
// [Accy Box Conv Third] 악세사리 박스가 존재하는지 확인
//==================================================================//
int CSeq_LoadingConv::OnCheck_AccyBoxConvInThird(int nzOnOff, int nzMode) 
{
	int nReadIO;

	// 센서가 B접이라 센서 상태 확인 시 반전 사용
	nReadIO = !g_ioMgr.get_in_bit( stIO.i_Chk_AccyBoxConvDetection[SIDE_THIRD_] );

	// 자재 존재하는 경우에 대한 체크인지 확인
	if( nzMode == 0 )
	{
		// 현재 WithOut 모드인지 확인
		// - 자재없이 돌리고 있으므로 감지되지 않기 때문에 강제로 설정함
		if( st_basic.n_mode_device == WITHOUT_DVC_)
		{
			nReadIO = nzOnOff;
		}
	}
	else if ( nzMode == 1 )
	{
		if( st_basic.n_mode_device == WITHOUT_DVC_)
		{
			nReadIO = st_map.nLoadingAccyBoxExist[0];
		}
	}
	if( nReadIO == IO_ON )
	{
		return IO_ON; // 센서 감지
	}
	else
	{
		return IO_OFF;// 센서 미감지
	}
}

//==================================================================//
// 악세사리 박스를 BoxClamp 영역로 고정하게 하는 스토터
// - IO_ON = UP 상태에서 Conv에 3개의 Accy Box를 공급받음
// - IO_OFF = Down 상태에서 BoxClamp에 적되어 Supply 영역으로 공금 받음
//==================================================================//
void CSeq_LoadingConv::OnSet_CylStopper(int nzOnOff) 
{
	g_ioMgr.set_out_bit(stIO.o_Cyl_AccyBoxConvStopperUp, nzOnOff);
	g_ioMgr.set_out_bit(stIO.o_Cyl_AccyBoxConvStopperDn, !nzOnOff);

	m_bflag_Stopper[0] = true;
	m_lWait_Stopper[0][0] = GetCurrentTime();
}

int CSeq_LoadingConv::OnGet_CylStopper(int nzOnOff) 
{
	int nFunRet = CTL_PROCEED;

	int nReadIO[2] = {0,};
	nReadIO[0] = g_ioMgr.get_in_bit(stIO.i_Chk_AccyBoxConvStopperUp);
	nReadIO[1] = g_ioMgr.get_in_bit(stIO.i_Chk_AccyBoxConvStopperDn);


	m_lWait_Stopper[0][1] = GetCurrentTime();
	m_lWait_Stopper[0][2] = m_lWait_Stopper[0][1] - m_lWait_Stopper[0][0];

	if (m_lWait_Stopper[0][2] < 0)
	{
		m_lWait_Stopper[0][0] = GetCurrentTime();
	}
	
	if (nzOnOff == IO_OFF)		// 스토퍼 내려간 상태
	{
		if (m_bflag_Stopper[0] == true &&
			nReadIO[0] == IO_OFF && nReadIO[1] == IO_ON)
		{
			m_lWait_Stopper[0][0] = GetCurrentTime();
			m_bflag_Stopper[0] = false;
		}
		else if (m_bflag_Stopper[0] == false &&
			nReadIO[0] == IO_OFF && nReadIO[1] == IO_ON)
		{
			if (m_lWait_Stopper[0][2] > st_time.nWait_Off[CYL_ACCY_BOX_STOPPER])
			{
				nFunRet = CTL_GOOD;	// [OFF:DOWN]
			}
		}
		else
		{
			if (m_lWait_Stopper[0][2] > st_time.nWait_Limit[CYL_ACCY_BOX_STOPPER])
			{
				// 120200 0 00 "PS0404_LOADING_CONV_STOPPER_DOWN1_CHK_ERR."
				m_strAlarmCode = "120200";
				nFunRet = CTL_ERROR;
			}
		}
	}
	else		// 스토퍼 올라온 상태
	{
		if (m_bflag_Stopper[0] == true &&
			nReadIO[0] == IO_ON && nReadIO[1] == IO_OFF)
		{
			m_lWait_Stopper[0][0] = GetCurrentTime();
			m_bflag_Stopper[0] = false;
		}
		else if (m_bflag_Stopper[0] == false &&
			nReadIO[0] == IO_ON && nReadIO[1] == IO_OFF)
		{
			if (m_lWait_Stopper[0][2] > st_time.nWait_On[CYL_ACCY_BOX_STOPPER])
			{
				nFunRet = CTL_GOOD;	// [OFF:DOWN]
			}
		}
		else
		{
			if (m_lWait_Stopper[0][2] > st_time.nWait_Limit[CYL_ACCY_BOX_STOPPER])
			{
				// 120201 0 00 "PS0403_LOADING_CONV_STOPPER_UP2_CHK_ERR."
				m_strAlarmCode = "120101";
				nFunRet = CTL_ERROR;
			}
		}
	}
	return nFunRet;
}


void CSeq_LoadingConv::OnSet_CylStopper2(int nzOnOff) 
{
	g_ioMgr.set_out_bit(stIO.o_Cyl_AccyBoxConvStopperUp2, nzOnOff);
	g_ioMgr.set_out_bit(stIO.o_Cyl_AccyBoxConvStopperDn2, !nzOnOff);
	
	m_bflag_Stopper[1] = true;
	m_lWait_Stopper[1][0] = GetCurrentTime();
}

int CSeq_LoadingConv::OnGet_CylStopper2(int nzOnOff) 
{
	int nFunRet = CTL_PROCEED;
	
	int nReadIO[2] = {0,};

	nReadIO[0] = g_ioMgr.get_in_bit(stIO.i_Chk_AccyBoxConvStopperUp2);
	nReadIO[1] = g_ioMgr.get_in_bit(stIO.i_Chk_AccyBoxConvStopperDn2);
	
	
	m_lWait_Stopper[1][1] = GetCurrentTime();
	m_lWait_Stopper[1][2] = m_lWait_Stopper[1][1] - m_lWait_Stopper[1][0];
	
	if (m_lWait_Stopper[1][2] < 0)
	{
		m_lWait_Stopper[1][0] = GetCurrentTime();
	}

	if (nzOnOff == IO_OFF)		// 스토퍼 내려간 상태
	{
		if (m_bflag_Stopper[1] == true &&
			nReadIO[0] == IO_OFF && nReadIO[1] == IO_ON)
		{
			m_lWait_Stopper[1][0] = GetCurrentTime();
			m_bflag_Stopper[1] = false;
		}
		else if (m_bflag_Stopper[1] == false &&
			nReadIO[0] == IO_OFF && nReadIO[1] == IO_ON)
		{
			if (m_lWait_Stopper[1][2] > st_time.nWait_Off[CYL_ACCY_BOX_STOPPER])
			{
				nFunRet = CTL_GOOD;	// [OFF:DOWN]
			}
		}
		else
		{
			if (m_lWait_Stopper[1][2] > st_time.nWait_Limit[CYL_ACCY_BOX_STOPPER])
			{
				// 120100 0 00 "PS0303_LOADING_CONV_STOPPER_DOWN_CHK_ERR."
				m_strAlarmCode = "120100";
				nFunRet = CTL_ERROR;
				
			}
		}
	}
	else		// 스토퍼 올라온 상태
	{
		if (m_bflag_Stopper[1] == true &&
			nReadIO[0] == IO_ON && nReadIO[1] == IO_OFF)
		{
			m_lWait_Stopper[1][0] = GetCurrentTime();
			m_bflag_Stopper[1] = false;
		}
		else if (m_bflag_Stopper[1] == false &&
			nReadIO[0] == IO_ON && nReadIO[1] == IO_OFF)
		{
			if (m_lWait_Stopper[1][2] > st_time.nWait_On[CYL_ACCY_BOX_STOPPER])
			{
				nFunRet = CTL_GOOD;	// [OFF:DOWN]
			}
		}
		else
		{
			if (m_lWait_Stopper[1][2] > st_time.nWait_Limit[CYL_ACCY_BOX_STOPPER])
			{
				// 120101 0 00 "PS0302_LOADING_CONV_STOPPER_UP_CHK_ERR."
				m_strAlarmCode = "120101";				
				nFunRet = CTL_ERROR;
			}
		}
	}
	return nFunRet;
}

void CSeq_LoadingConv::OnSet_CylBoxClamp(int nzOnOff) 
{
	g_ioMgr.set_out_bit(stIO.o_Load_BoxClampOnOff, nzOnOff);
	
	m_bflag_Clamp[0] = true;
	m_lWait_Clamp[0][0] = GetCurrentTime();
}

int CSeq_LoadingConv::OnGet_CylBoxClamp(int nzOnOff) 
{
	int nFunRet = CTL_PROCEED;
	
	int nReadIO[2] = {0,};
	nReadIO[0] = g_ioMgr.get_in_bit(stIO.i_Chk_Load_BoxClampOnCheck);
	nReadIO[1] = g_ioMgr.get_in_bit(stIO.i_Chk_Load_BoxClampOffCheck);
	
	m_lWait_Clamp[0][1] = GetCurrentTime();
	m_lWait_Clamp[0][2] = m_lWait_Clamp[1] - m_lWait_Clamp[0];
	
	if (m_lWait_Clamp[0][2] < 0)
	{
		m_lWait_Clamp[0][0] = GetCurrentTime();
	}
	
	if (nzOnOff == IO_OFF)		// 스토퍼 내려간 상태
	{
		if (m_bflag_Clamp[0] == true &&
			nReadIO[0] == IO_OFF && nReadIO[1] == IO_ON)
		{
			m_lWait_Clamp[0][0] = GetCurrentTime();
			m_bflag_Clamp[0] = false;
		}
		else if (m_bflag_Clamp[0] == false &&
			nReadIO[0] == IO_OFF && nReadIO[1] == IO_ON)
		{
			if (m_lWait_Clamp[0][2] > st_time.nWait_Off[CYL_ACCY_BOX_STOPPER])
			{
				nFunRet = CTL_GOOD;	// [OFF:DOWN]
			}
		}
		else
		{
			if (m_lWait_Clamp[0][2] > st_time.nWait_Limit[CYL_ACCY_BOX_STOPPER])
			{
				// 130000 0 00 "PS0700_PS0701_LOAD_BOX_CLAMP_OFF_CHK_ERR."
				m_strAlarmCode = "130000";
				nFunRet = CTL_ERROR;	
			}
		}
	}
	else		// 스토퍼 올라온 상태
	{
		if (m_bflag_Clamp[0] == true &&
			nReadIO[0] == IO_ON && nReadIO[1] == IO_OFF)
		{
			m_lWait_Clamp[0][0] = GetCurrentTime();
			m_bflag_Clamp[0] = false;
		}
		else if (m_bflag_Clamp[0] == false &&
			nReadIO[0] == IO_ON && nReadIO[1] == IO_OFF)
		{
			if (m_lWait_Clamp[0][2] > st_time.nWait_On[CYL_ACCY_BOX_STOPPER])
			{
				nFunRet = CTL_GOOD;	// [OFF:DOWN]
			}
		}
		else
		{
			if (m_lWait_Clamp[0][2] > st_time.nWait_Limit[CYL_ACCY_BOX_STOPPER])
			{
				// 130001 0 00 "PS0700_PS0701_LOAD_BOX_CLAMP_ON_CHK_ERR."
				m_strAlarmCode = "130001 ";
				nFunRet = CTL_ERROR;
			}
		}
	}
	return nFunRet;
}

//kwlee 2017.0707
void CSeq_LoadingConv::OnSet_CylBoxPusher(int nzOnOff) 
{
	g_ioMgr.set_out_bit(stIO.o_Load_BoxPusherCylPusher, nzOnOff);
	g_ioMgr.set_out_bit(stIO.o_Load_BoxPusherCylPull, !nzOnOff);
	m_bflag_Cylinder[0] = true;
	m_lWait_Cylinder[0][0] = GetCurrentTime();
}

int CSeq_LoadingConv::OnGet_CylBoxPusher(int nzOnOff) 
{
	int nFunRet = CTL_PROCEED;
	
	int nReadIO[2] = {0,};
	nReadIO[0] = g_ioMgr.get_in_bit(stIO.i_Chk_Load_BoxPusherCylPusherCheck);
	nReadIO[1] = g_ioMgr.get_in_bit(stIO.i_Chk_Load_BoxPusherCylPullCheck);

	m_lWait_Cylinder[0][1] = GetCurrentTime();
	m_lWait_Cylinder[0][2] = m_lWait_Cylinder[1] - m_lWait_Cylinder[0];
	
	if (m_lWait_Cylinder[0][2] < 0)
	{
		m_lWait_Cylinder[0][0] = GetCurrentTime();
	}
	
	if (nzOnOff == IO_OFF)	
	{
		if (m_bflag_Cylinder[0] == true &&
			nReadIO[0] == IO_OFF && nReadIO[1] == IO_ON)
		{
			m_lWait_Cylinder[0][0] = GetCurrentTime();
			m_bflag_Cylinder[0] = false;
		}
		else if (m_bflag_Cylinder[0] == false && 
				nReadIO[0] == IO_OFF && nReadIO[1] == IO_ON )
		{
			if (m_lWait_Cylinder[0][2] > st_time.nWait_Off[CYL_ACCY_BOX_STOPPER])
			{
				nFunRet = CTL_GOOD;	// [OFF:DOWN]
			}
		}
		else
		{
			if (m_lWait_Cylinder[0][2] > st_time.nWait_Limit[CYL_ACCY_BOX_STOPPER])
			{
				//131000 0 00 "PS0702_LEFT_LOAD_BOX_PUSHER_FWDBWD_OFF_CHK_ERR."
				m_strAlarmCode = "131000";
				nFunRet = CTL_ERROR;	
			}
		}
	}
	else		// 스토퍼 올라온 상태
	{
 		if (m_bflag_Cylinder[0] == true &&
 			nReadIO[0] == IO_ON && nReadIO[1] == IO_OFF)
		
		
		{
			m_lWait_Cylinder[0][0] = GetCurrentTime();
			m_bflag_Cylinder[0] = false;
		}
 		else if (m_bflag_Cylinder[0] == false &&
 			nReadIO[0] == IO_ON && nReadIO[1] == IO_OFF)
		{
			if (m_lWait_Cylinder[0][2] > st_time.nWait_On[CYL_ACCY_BOX_STOPPER])
			{
				nFunRet = CTL_GOOD;	// [OFF:DOWN]
			}
		}
		else
		{
			if (m_lWait_Cylinder[0][2] > st_time.nWait_Limit[CYL_ACCY_BOX_STOPPER])
			{
				//131002 0 00 "PS0702_LEFT_LOAD_BOX_PUSHER_FWDBWD_ON_CHK_ERR."
				m_strAlarmCode = "131002";
				nFunRet = CTL_ERROR;
			}
		}
	}
	return nFunRet;
}

