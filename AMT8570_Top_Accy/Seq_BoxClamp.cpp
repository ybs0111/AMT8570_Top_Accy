// Seq_BoxClamp.cpp: implementation of the CSeq_BoxClamp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "handler.h"
#include "Seq_BoxClamp.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

long m_lTime_Without[3];

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CSeq_BoxClamp	SeqBoxClamp;

CSeq_BoxClamp::CSeq_BoxClamp()
{

}

CSeq_BoxClamp::~CSeq_BoxClamp()
{

}

//==================================================================//
// 쓰레드 동작
//==================================================================//
void CSeq_BoxClamp::OnSeq_RUN(void) 
{
	OnSeq_INIT();
	OnSeq_Execute();
}

//==================================================================//
// 쓰레드 관련 변수 초기화
//==================================================================//
void CSeq_BoxClamp::OnSeq_INIT(void) 
{
	if( m_bflag_INIT == true )
	{
		return;
	}
	m_bflag_INIT = true;

	m_nStep_Init = 0;
	m_nStep_Run = 0;
	m_nRcvyStep = 0;
}

//==================================================================//
// 쓰레드 제어
//==================================================================//
void CSeq_BoxClamp::OnSeq_Execute(void) 
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

void CSeq_BoxClamp::OnRun_Initial() 
{
	// 초기화 작업이 완료된 경우 강제 리턴함
	if (st_handler.mn_init_state[INIT_BOX_CLAMP] != CTL_NO)
	{
		return;
	}
	
	int nRet = VAR_INIT_;
	int nRetData[4] = {0,};

	switch(m_nStep_Init)
	{
	case 0:
		m_nStep_Init = 10;
		break;

	case 10://have to check...
		// XYZ Robot이 안전 영역에 없을때 Box Clamp를 초기화
		// 센서가 B접이라 센서 상태 확인 시 반전 사용
		nRet = !g_ioMgr.get_in_bit(stIO.i_Chk_GripperSafetyPos);
		if (nRet == IO_OFF)
		{
			m_nStep_Init = 100;
		}
		break;

	case 100:
// 		OnSet_CylClamp(IO_OFF);			// Align
// 		OnSet_CylUDGripper(IO_OFF);		// 악세사리 박스 윗부분을 정렬 하는 실린더 [ON:정렬, OFF:해제]
//		m_nStep_Init = 110;

		//kwlee 2017.0712
		if( st_work.nLiftDownComplete == CTL_YES )
		{
			OnSet_CylClamp(IO_OFF);			// Align
			OnSet_CylUDGripper(IO_OFF);		// 악세사리 박스 윗부분을 정렬 하는 실린더 [ON:정렬, OFF:해제]
			m_nStep_Init = 110;
		}
		break;

	case 110:
		nRetData[0] = OnGet_CylClamp(IO_OFF);
		nRetData[1] = OnGet_CylUDGripper(IO_OFF);
		if(nRetData[0] == CTL_GOOD && nRetData[1] == CTL_GOOD)
		{
			stSync.nResp_Clamp2LoadingConv_Work = SYNC_RESP_RESET_;
			m_nStep_Init = 200;
		}
		else if (nRetData[0] == CTL_ERROR || nRetData[1] == CTL_ERROR)
		{
			// 알람 코드는 OnGet_CylLRGripper() 함수 안에서 설정함
			m_nStep_Init = 100;
			CTL_Lib.Alarm_Error_Occurrence( 1000, CTL_dWARNING, alarm.mstr_code );
		}
		break;

		
	case 200:
		OnSet_ConvBoxPusher(IO_OFF);			// pusher backward
		m_nStep_Init = 210;
		break;

	case 210:
		nRet = OnGet_ConvBoxPusher( IO_OFF );
		if( nRet == CTL_GOOD )
		{
			m_nStep_Init = 300;
		}
		else if( nRet == CTL_ERROR )
		{
			m_nStep_Init = 200;
			CTL_Lib.Alarm_Error_Occurrence( 1001, CTL_dWARNING, alarm.mstr_code );
		}
		break;

	case 300:
		OnSet_ConvBoxClamp( IO_OFF );
		m_nStep_Init = 310;
		break;

	case 310:
		nRet = OnGet_ConvBoxClamp( IO_OFF );
		if( nRet == CTL_GOOD )
		{
			m_nStep_Init = 400;
		}
		else if( nRet == CTL_ERROR )
		{
			m_nStep_Init = 300;
			CTL_Lib.Alarm_Error_Occurrence( 1002, CTL_dWARNING, alarm.mstr_code );
		}
		break;

	case 400:
		// [ BoxLift -> BoxClamp ] 작업 완료 대기
		if (stSync.nResp_Lifter2Clamp_Work == SYNC_RESP_WORK_COMPLETE_)
		{
			m_nStep_Init = 500;
		}
		break;

	case 500:
		OnSet_CylLifter( IO_OFF );
		m_nStep_Init = 510;
		break;

	case 510:
		nRet = OnGet_CylLifter( IO_OFF );
		if( nRet == CTL_GOOD )
		{
			m_nStep_Init = 520;
		}
		else if( nRet == CTL_ERROR )
		{
			m_nStep_Init = 500;
			CTL_Lib.Alarm_Error_Occurrence( 1003, CTL_dWARNING, alarm.mstr_code );
		}
		break;

	case 520:
		// Lift 실린더가 내려가면 Lifter에게 SYNC_REQ_WORK_COMPLETE_ 신호 전달
		stSync.nReq_Clamp2Lifter_Work = SYNC_REQ_WORK_COMPLETE_;
		m_nStep_Init = 530;
		break;

	case 530:
		// Lifter의 응답 신호가  SYNC_RESP_WORK_COMPLETE_으로 변경되면 
		// Lifter X축이 첫번째 공급 위치로 이동하면서 Accy Box를 제거 할때 까지 대기
		if( stSync.nResp_Lifter2Clamp_Work == SYNC_RESP_WORK_COMPLETE_ )
		{
			stSync.nReq_Clamp2Lifter_Work = SYNC_REQ_RESET_;
			m_nStep_Init = 600;
		}
		break;

	case 600:
		OnSet_CylLifter( IO_ON );
		m_nStep_Init = 610;
		break;

	case 610:
		nRet = OnGet_CylLifter( IO_ON );
		if( nRet == CTL_GOOD )
		{
			m_nStep_Init = 1000;
		}
		else if( nRet == CTL_ERROR )
		{
			m_nStep_Init = 600;
			CTL_Lib.Alarm_Error_Occurrence( 1004, CTL_dWARNING, alarm.mstr_code );
		}
		break;

	case 1000:
		// 초기화 화면에서 [초기화] 완료 표시 가능
		st_handler.mn_init_state[INIT_BOX_CLAMP] = CTL_YES;
		stSync.nReq_Clamp2Lifter_Work = SYNC_REQ_RESET_;
		m_nStep_Init = 0;
		break;

	}
}

void CSeq_BoxClamp::OnRun_Move() 
{
	// 전체 사이트 복구 동작 완료 여부 확인
	// - 모든 부분의 복구 동작이 완료된 후에만 시컨스 동작하도록 함
	// : 복구 동작 중에 시컨스 구동하면 복구하는 영역과 충돌이 발생할 수 있음
	if( Func.OnIsAllRcvyComplete() != CTL_YES )
	{
		return;
	}
	Func.OnTrace_ThreadStep(2, m_nStep_Run);

	int nRet = VAR_INIT_;
	int nRetData[4] = {0,};
	int i = 0;

	switch( m_nStep_Run )
	{
	case 0:
		// Loading Cov가 LotEnd 상태이면 Box Clamp를 LotEnd 처리 이며 대기
		if( stSync.nLotEnd_LoadingConv == TRUE )
		{
			stSync.nLotEnd_BoxClamp = TRUE;
			break;
		}
		else
		{
			stSync.nLotEnd_BoxClamp = FALSE;
		}

		if( st_basic.n_mode_device == WITHOUT_DVC_ )
		{
			if( st_map.nLoadingAccyBoxExist[0] == 0 &&
				st_map.nLoadingAccyBoxExist[1] == 0 &&
				st_map.nLoadingAccyBoxExist[2] == 0 )
			{
				stSync.nResp_Clamp2LoadingConv_Work = SYNC_RESP_RESET_;
				break;
			}
		}

		stSync.nResp_Clamp2LoadingConv_Work = SYNC_RESP_RESET_;
		m_nStep_Run = 100;
		//kwlee 2017.0711
		//m_nStep_Run = 10;
		break;

	case 100:
		// Load Conv에서 Accy Box 공급이 완료 될때까지 대기
		if( stSync.nReq_LoadingConv2Clamp_Work == SYNC_REQ_ACCY_BOX_LOADING_COMPLETE_)
		{
			stSync.nResp_Clamp2LoadingConv_Work = SYNC_RESP_WORK_;
			// 박스가 없을 경우 알람 소리를 듣고 박스 공급을 하기 위해 알람을 울리도록 해놓음.
			Func.OnSet_BuzzerSound(IO_OFF, 0);

			m_nPusher_Retry = 0;
			m_nStep_Run = 900;
		}
		break;

	//////////////////////////////////////////////////////////////////////////
	// Accy Box Gripping과 Lift 유닛에 Accy Box를 공급 하는 작업
	//////////////////////////////////////////////////////////////////////////
	case 900:
		{
			int ReadIO[4] = {0,};
 			ReadIO[0] = g_ioMgr.get_in_bit( stIO.i_Chk_Load_BoxClampOnCheck, IO_OFF);
 			ReadIO[1] = g_ioMgr.get_in_bit( stIO.i_Chk_Load_BoxClampOffCheck, IO_ON);
 			ReadIO[2] = g_ioMgr.get_in_bit( stIO.i_Chk_Load_BoxPusherCylPusherCheck, IO_OFF);
 			ReadIO[3] = g_ioMgr.get_in_bit( stIO.i_Chk_Load_BoxPusherCylPullCheck, IO_ON);

			if( ReadIO[0] == IO_OFF && ReadIO[1] == IO_ON && ReadIO[2] == IO_OFF && ReadIO[3] == IO_ON )
			{
				m_nStep_Run = 2000;
			}
			else
			{
				m_nStep_Run = 1000;
			}
		}
		break;

	case 1000:
		OnSet_ConvBoxPusher(IO_OFF);			// Align
		m_nStep_Run = 1100;
		break;

	case 1100:
		nRet = OnGet_ConvBoxPusher( IO_OFF );
		if( nRet == CTL_GOOD )
		{
			m_nStep_Run = 1200;
		}
		else if( nRet == CTL_ERROR )
		{
			m_nStep_Run = 1000;
			CTL_Lib.Alarm_Error_Occurrence( 1005, CTL_dWARNING, alarm.mstr_code );
		}
		break;

	case  1200:
		OnSet_ConvBoxClamp( IO_OFF );
		m_nStep_Run = 1300;
		break;
		
	case 1300:
		nRet = OnGet_ConvBoxClamp( IO_OFF );
		if( nRet == CTL_GOOD )
		{
			m_nStep_Run = 1400;
		}
		else if( nRet == CTL_ERROR )
		{
			m_nStep_Run = 1200;
			CTL_Lib.Alarm_Error_Occurrence( 1006, CTL_dWARNING, alarm.mstr_code );
		}
		break;


		//Conv Box Pusher
	case  2000:
		OnSet_ConvBoxClamp( IO_ON );
		m_nStep_Run = 2100;
		break;
		
	case 2100:
		nRet = OnGet_ConvBoxClamp( IO_ON );
		if( nRet == CTL_GOOD )
		{
			m_nStep_Run = 2200;
		}
		else if( nRet == CTL_ERROR )
		{
			m_nStep_Run = 2000;
			CTL_Lib.Alarm_Error_Occurrence( 1007, CTL_dWARNING, alarm.mstr_code );
		}
		break;

	case 2200:
		OnSet_ConvBoxPusher(IO_ON);			// Align
		m_nStep_Run = 2300;
		break;
		
	case 2300:
		nRet = OnGet_ConvBoxPusher( IO_ON );
		if( nRet == CTL_GOOD )
		{
			m_lTime_GoesBy[0] = GetCurrentTime();
			m_nStep_Run = 2400;
		}
		else if( nRet == CTL_ERROR )
		{
			m_nStep_Run = 2200;
			CTL_Lib.Alarm_Error_Occurrence( 1008, CTL_dWARNING, alarm.mstr_code );
		}
		break;

	case 2400:
		nRet = g_ioMgr.get_in_bit( stIO.i_Chk_AccyBox_Arrive_End, IO_ON );
		if( nRet == IO_ON )
		{
			m_nStep_Run = 2500;
		}
		else
		{
			m_lTime_GoesBy[1] = GetCurrentTime();
			m_lTime_GoesBy[2] = m_lTime_GoesBy[1] - m_lTime_GoesBy[0];
			if( m_lTime_GoesBy[2] < 0) m_lTime_GoesBy[0] = GetCurrentTime();
			if( m_lTime_GoesBy[2] < 100 ) break;
			//132000 0 00 "PS0407_LOAD_BOX_ARRIVE_END_OFF_CHK_ERR."
			CTL_Lib.Alarm_Error_Occurrence( 1009, CTL_dWARNING, "132000" );
		}
		break;

	case 2500://alig
		OnSet_CylClamp(IO_ON);
		m_nStep_Run = 2510;
		break;

	case 2510:
		nRet = OnGet_CylClamp(IO_ON);
		if( nRet == CTL_GOOD )
		{
			m_nStep_Run = 2600;
		}
		else
		{
			CTL_Lib.Alarm_Error_Occurrence( 1010, CTL_dWARNING, alarm.mstr_code );
			m_nStep_Run = 2500;
		}
		break;

	case 2600://IO_OFF = DOWN
		OnSet_CylUDGripper(IO_OFF);
	//	OnSet_ConvBoxPusher(IO_OFF);
		m_nStep_Run = 2610;
		break;

	case 2610:
		nRetData[0] = OnGet_CylUDGripper( IO_OFF );
 	//	nRetData[1] = OnGet_ConvBoxPusher( IO_OFF ); 
 	//	if ( nRetData[0] == CTL_GOOD && nRetData[1] == CTL_GOOD )
		//kwlee 2017.0712 
		if ( nRetData[0] == CTL_GOOD)
		{
			m_nStep_Run = 2700;
			
		}
		else if ( nRetData[0] == CTL_ERROR || nRetData[1] == CTL_ERROR )
		{
			CTL_Lib.Alarm_Error_Occurrence( 1011, CTL_dWARNING, alarm.mstr_code );
			m_nStep_Run = 2600;
		}
		break;

		//Clamp & Pusher 동시 OFF
	case 2700:
		OnSet_ConvBoxClamp( IO_OFF );
		OnSet_ConvBoxPusher(IO_OFF); //kwlee 2017.0712
		m_nStep_Run = 2710;
		break;
		
	case 2710:
		//nRet = OnGet_ConvBoxClamp( IO_OFF );
		//if( nRet == CTL_GOOD )
		//kwlee 2017.0712
		nRetData[0] = OnGet_ConvBoxClamp( IO_OFF );
		nRetData[1] = OnGet_ConvBoxPusher( IO_OFF ); 

		if ( nRetData[0] == CTL_GOOD && nRetData[1] == CTL_GOOD )
		{
			// [WithOut] 모드인지 확인
			if (st_basic.n_mode_device == WITHOUT_DVC_)
			{
				st_map.nLoadingAccyBoxExist[0] = 0;
				
				if (stWithoutData.nWorkTestBox != 0)
				{
					stWithoutData.nWorkTestBox--;
				}
			}
			// BoxClamp가 작업 영역에 도착 했다고 Loading Conv에 알림
			stSync.nResp_Clamp2LoadingConv_Work = SYNC_RESP_RESET_;
			
			m_nStep_Run = 2800;
		}
		else if( nRet == CTL_ERROR )
		{
			m_nStep_Run = 2700;
			CTL_Lib.Alarm_Error_Occurrence( 1012, CTL_dWARNING, alarm.mstr_code );
		}
		break;

	case 2800:
		st_map.nBoxChange = TRUE;
		st_map.nResp_AccyReadyCNT = 240;
		// Lifter에게 Accy Box Clamping 완료 알림(누구에게?? 리프트에게??)(그럼 무슨일 하지?)
		stSync.nReq_Clamp2Lifter_Work = SYNC_REQ_ACCY_BOX_CLAMPING_COMPLETE_;
		
		st_map.nClampAccyBoxAccy = 1;
		
		if (st_handler.cwnd_main != NULL)
		{
			// Box Clamp 자재 존재 상태 표시 요청
			st_handler.cwnd_main->PostMessage(WM_UPDATE_MAIN, PLACE_BOX_CLAMP_);
		}
		m_nStep_Run = 3000;
		break;


		//////////////////////////////////////////////////////////////////////////
		// Accy Box 배출 작업
		//////////////////////////////////////////////////////////////////////////
	case 3000:
		// [BoxLifter -> BoxClamp] 작업 완료 대기
		if( stSync.nResp_Lifter2Clamp_Work == SYNC_RESP_WORK_COMPLETE_ )
		{		   
			//stSync.nReq_Clamp2Lifter_Work = SYNC_REQ_WORK_; //kwlee 2017.0711
			m_nStep_Run = 3100;
		}
		break;

	case 3100:
		OnSet_CylClamp(IO_OFF);	            // align
		//OnSet_CylUDGripper(IO_OFF);			// 악세사리 박스 윗부분을 정렬 하는 실린더 [ON:정렬, OFF:해제]
		//kwlee 2017.0712
		OnSet_CylUDGripper(IO_ON);			// 악세사리 박스 윗부분을 정렬 하는 실린더 [ON:정렬, OFF:해제]
		m_nStep_Run = 3110;
		break;

	case 3110:
		nRetData[0] = OnGet_CylClamp( IO_OFF );
		//nRetData[1] = OnGet_CylUDGripper( IO_OFF );
		//kwlee 2017.0712 IO_ON = UP
		nRetData[1] = OnGet_CylUDGripper( IO_ON );
		if( nRetData[0] == CTL_GOOD && nRetData[1] == CTL_GOOD )
		{
			m_lTime_GoesBy[01] = GetCurrentTime();
			m_nStep_Run = 4000;
			//stSync.nReq_Clamp2Lifter_Work = SYNC_REQ_RESET_;
			//
		}
		else if( nRetData[0] == CTL_ERROR || nRetData[1] == CTL_ERROR )
		{
			CTL_Lib.Alarm_Error_Occurrence( 1013, CTL_dWARNING, alarm.mstr_code );
			m_nStep_Run = 3100;
		}
		break;

	case 4000:
		//kwlee 2017.0707
 		nRetData[0] = g_ioMgr.get_in_bit( stIO.i_Chk_Load_BoxPusherCylPusherCheck, IO_OFF);
 		nRetData[1] = g_ioMgr.get_in_bit( stIO.i_Chk_Load_BoxPusherCylPullCheck, IO_ON);
 		if( nRetData[0] == IO_OFF && nRetData[1] == IO_ON )
 		{
 			m_nStep_Run = 4100;
 		}
 		else
 		{
 			m_lTime_GoesBy[1] = GetCurrentTime();
 			m_lTime_GoesBy[2] = m_lTime_GoesBy[1] - m_lTime_GoesBy[0];
 			if( m_lTime_GoesBy[2] < 0) m_lTime_GoesBy[0] = GetCurrentTime();
 			if( m_lTime_GoesBy[2] < 100 ) break;
 			//131001 0 00 "PS0702_PS0703_LOAD_BOX_PUSHER_FORWARD_CHK_ERR."
 			CTL_Lib.Alarm_Error_Occurrence( 1014, CTL_dWARNING, "131001" );
 		}
		break;

	case 4100:
		OnSet_CylLifter(IO_OFF);		// 악세사리 박스 밑부분을 받쳐주는 실린더 [ON:Up, OFF:Down]
		m_nStep_Run = 4110;
		break;


	case 4110:
		nRet = OnGet_CylLifter( IO_OFF );
		if( nRet == CTL_GOOD )
		{
			//stSync.nReq_Clamp2Lifter_Work = SYNC_REQ_RESET_; 
			//kwlee 2017.0711
			stSync.nReq_Clamp2Lifter_Work = SYNC_REQ_WORK_;
			m_nStep_Run = 4200;
		}
		else if( nRet == CTL_ERROR )
		{
			CTL_Lib.Alarm_Error_Occurrence( 1015, CTL_dWARNING, alarm.mstr_code );
			m_nStep_Run = 4100;
		}
		break;

	case 4200:
		// Box Remove 완료 대기 
		
		if( stSync.nResp_Lifter2Clamp_Work == SYNC_RESP_BOX_REMOVE_COMPLETE )
		{
			//stSync.nReq_Clamp2Lifter_Work = SYNC_REQ_WORK_COMPLETE_; //kwlee 2017.0711
			m_nStep_Run = 4300;
		}
		else if( stSync.nReq_LoadingConv2Clamp_Work == SYNC_REQ_ACCY_BOX_LOADING_COMPLETE_ )
		{
			stSync.nResp_Clamp2LoadingConv_Work = SYNC_RESP_WORK_;
			m_nStep_Run = 4300;
		}
		break;

	case 4300:
		// Box Remove 완료 대기 
		if(stSync.nResp_Lifter2Clamp_Work == SYNC_RESP_BOX_REMOVE_COMPLETE)
		{
			//stSync.nReq_Clamp2Lifter_Work = SYNC_REQ_WORK_COMPLETE_; //kwlee 2017.0712
			m_nStep_Run = 4500;
		}
		break;

	case 4500:
		// 다음 Accy Box를 공급 받기 위해 Lift를 올림
		OnSet_CylLifter(IO_ON);		// 악세사리 박스 밑부분을 받쳐주는 실린더 [ON:Up, OFF:Down]
		m_nStep_Run = 4510;
		break;

	case 4510:
		nRet = OnGet_CylLifter(IO_ON);
		if (nRet == CTL_GOOD)
		{
			//stSync.nReq_Clamp2Lifter_Work = SYNC_REQ_WORK_COMPLETE_; //kwlee 2017.0711 올렸다고 알려라.
			m_nStep_Run = 5000;
		}
		else if (nRet == CTL_ERROR)
		{
			CTL_Lib.Alarm_Error_Occurrence( 1016, CTL_dWARNING, alarm.mstr_code );
			m_nStep_Run = 4500;
		}
		break;

	case 5000:
		stSync.nReq_Clamp2Lifter_Work = SYNC_REQ_RESET_;
		m_nStep_Run = 0;
		break;

	}

}

//==================================================================//
// 현재 작업 중인 Box를 제거 하는 함수
//==================================================================//
int CSeq_BoxClamp::OnRun_ForceDischarge() 
{
	int nFuncRet = CTL_PROCEED;
//	int nRet;
	int nRetData[4] = {0,};
	
	Func.OnTrace_ThreadStep(4, m_nStep_ForceDischarge);	// 쓰레드 스텝 정보 TRACE
	
	// Box 배출 대기
	if(st_work.nForceDischarge != FORCEDISCHARGE_ON)
	{
		return true;
	}
	
// 	switch(m_nStep_ForceDischarge)
// 	{
// 	case 0:
// 		if(stSync.nForceDischargeComplete[SITE_BOX_CLAMP_] != CTL_YES)
// 		{
// 			stSync.nReq_Clamp2Lifter_Work = SYNC_REQ_WORK_;
// 			m_nStep_ForceDischarge = 10;
// 		}
// 		break;
// 
// 	case 10:
// 		if(stSync.nResp_Lifter2Clamp_Work == SYNC_RESP_BOX_REMOVE_READY)
// 		{
// 			m_nStep_ForceDischarge = 100;
// 		}
// 		break;
// 
// 	case 100:
// // 		OnSet_CylPusher(IO_OFF);		// 악세사리 박스 뒷부분을 정렬 하는 푸셔 [ON:정렬, OFF:해제]
// 		OnSet_CylClamp(IO_OFF);		// 악세사리 박스 윗부분을 정렬 하는 실린더 [ON:정렬, OFF:해제]
// // 		OnSet_CylLRGripper(IO_OFF);		// Box LR Gripper [ON:Grip, OFF:UnGrip]
// 		m_nStep_ForceDischarge = 110;
// 		break;
// 
// 	case 110:
// // 		nRetData[0] = OnGet_CylPusher(IO_OFF);
// 		nRetData[1] = OnGet_CylClamp(IO_OFF);
// // 		nRetData[2] = OnGet_CylLRGripper(IO_OFF);
// 		if (nRetData[0] == CTL_GOOD && nRetData[1] == CTL_GOOD && nRetData[2] == CTL_GOOD)
// 		{
// 			m_nStep_ForceDischarge = 200;
// 		}
// 		else if (nRetData[0] == CTL_ERROR || nRetData[1] == CTL_ERROR || nRetData[2] == CTL_ERROR)
// 		{
// 			// 알람 코드는 OnGet_CylPusher() 함수 안에서 설정함
// 			// 알람 코드는 OnGet_CylClamp() 함수 안에서 설정함
// 			// 알람 코드는 OnGet_CylLRGripper() 함수 안에서 설정함
// 			m_nStep_ForceDischarge = 100;
// 		}
// 		break;
// 
// 	case 200:
// 		OnSet_CylSupply(IO_OFF);		// 악세사리 박스 공급 실린더[ON:작업 영역, OFF:짚는 영역]
// 		m_nStep_ForceDischarge = 210;
// 		break;
// 
// 	case 210:
// 		nRet = g_ioMgr.get_in_bit(stIO.i_Chk_AccyBoxSupplyFw);
// 		if (nRet == IO_OFF)
// 		{
// 			m_nStep_ForceDischarge = 300;
// 		}
// 		break;
// 
// 	case 300:
// 		OnSet_CylLifter(IO_OFF);		// 악세사리 박스 밑부분을 받쳐주는 실린더 [ON:Up, OFF:Down]
// 		m_nStep_ForceDischarge = 310;
// 		break;
// 
// 	case 310:
// 		nRetData[0] = OnGet_CylLifter(IO_OFF);
// 		nRetData[1] = OnGet_CylSupply(IO_OFF);
// 		if (nRetData[0] == CTL_GOOD && nRetData[1] == CTL_GOOD)
// 		{
// 			stSync.nReq_Clamp2Lifter_Work = SYNC_REQ_RESET_;
// 
// 			st_map.nClampAccyBoxAccy = 0;
// 			
// 			if (st_handler.cwnd_main != NULL)
// 			{
// 				// Box Clamp 자재 존재 상태 표시 요청
// 				st_handler.cwnd_main->PostMessage(WM_UPDATE_MAIN, PLACE_BOX_CLAMP_);
// 			}
// 			m_nStep_ForceDischarge = 400;
// 		}
// 		else if (nRetData[0] == CTL_ERROR)
// 		{
// 			// 알람 코드는 OnGet_CylLifter() 함수 안에서 설정함
// 			m_nStep_ForceDischarge = 300;
// 		}
// 		else if (nRetData[1] == CTL_ERROR)
// 		{
// 			// 알람 코드는 OnGet_CylSupply() 함수 안에서 설정함
// 			m_nStep_ForceDischarge = 200;
// 		}
// 		break;
// 
// 	case 400:
// 		// 김양규 추가 [2015.06.24]
// 		// Lift 쪽에서 싱크 변수 타입을 2개로 주고 있어 추가 함.
// 		if(stSync.nResp_Lifter2Clamp_Work == SYNC_RESP_RESET_ || stSync.nResp_Lifter2Clamp_Work == SYNC_RESP_BOX_REMOVE_COMPLETE)
// 		{
// 			m_nStep_ForceDischarge = 500;
// 		}
// 		break;
// 
// 	case 500:
// 		OnSet_CylLifter(IO_ON);		// 악세사리 박스 밑부분을 받쳐주는 실린더 [ON:Up, OFF:Down]
// 		m_nStep_ForceDischarge = 510;
// 		break;
// 
// 	case 510:
// 		nRet = OnGet_CylLifter(IO_ON);
// 		if (nRet == CTL_GOOD)
// 		{
// 			m_nStep_ForceDischarge = 600;
// 		}
// 		else if (nRet == CTL_ERROR)
// 		{
// 			// 알람 코드는 OnGet_CylLifter() 함수 안에서 설정함
// 			m_nStep_ForceDischarge = 500;
// 		}
// 		break;
// 
// 	case 600:
// 		m_nStep_Run = 0;
// 		m_nStep_ForceDischarge = 0;
// 		stSync.nForceDischargeComplete[SITE_BOX_CLAMP_] = CTL_YES;
// 		stSync.nResp_Clamp2LoadingConv_Work = SYNC_RESP_RESET_;
// 		stSync.nReq_Clamp2Lifter_Work = SYNC_REQ_RESET_;
// 		break;
// 	}

	return nFuncRet;
}

//==================================================================//
// Load Conv Box Clamp
//==================================================================//
void CSeq_BoxClamp::OnSet_ConvBoxClamp(int nzOnOff) 
{
	g_ioMgr.set_out_bit(stIO.o_Load_BoxClampOnOff, nzOnOff);

	m_bflag_ConvClamp = true;
	m_lWait_ConvClamp[0] = GetCurrentTime();
}

int CSeq_BoxClamp::OnGet_ConvBoxClamp(int nzOnOff) 
{
	int nFunRet = CTL_PROCEED;

	int nReadIO[2] = {0,};
	nReadIO[0] = g_ioMgr.get_in_bit(stIO.i_Chk_Load_BoxClampOnCheck);
	nReadIO[1] = g_ioMgr.get_in_bit(stIO.i_Chk_Load_BoxClampOffCheck);

	m_lWait_ConvClamp[1] = GetCurrentTime();
	m_lWait_ConvClamp[2] = m_lWait_ConvClamp[1] - m_lWait_ConvClamp[0];

	if (m_lWait_ConvClamp[2] < 0)
	{
		m_lWait_ConvClamp[0] = GetCurrentTime();
	}

	if (nzOnOff == IO_OFF)		// 박스를 놓은 상태
	{
		if (m_bflag_ConvClamp == true &&
			nReadIO[0] == IO_OFF && nReadIO[1] == IO_ON)
		{
			m_lWait_ConvClamp[0] = GetCurrentTime();
			m_bflag_ConvClamp = false;
		}
		else if (m_bflag_ConvClamp == false &&
			nReadIO[0] == IO_OFF && nReadIO[1] == IO_ON )
		{
			if (m_lWait_ConvClamp[2] > st_time.nWait_Off[CYL_ACCY_CONV_BOX_CLAMP])
			{
				nFunRet = CTL_GOOD;
			}
		}
		else
		{
			if (m_lWait_ConvClamp[2] > st_time.nWait_Limit[CYL_ACCY_CONV_BOX_CLAMP])
			{
				//  130000 0 00 "PS0701_PS0703_LOAD_BOX_CLAMP_OFF_CHK_ERR."
				alarm.mstr_code		= "130000";

				alarm.mn_count_mode	= 0;
				alarm.mn_type_mode	= eWARNING;
				st_work.nEqpStatus	= dWARNING;

				nFunRet = CTL_ERROR;
			}
		}
	}
	else		// 박스를 잡은 상태
	{
		if (m_bflag_ConvClamp == true &&
			nReadIO[0] == IO_ON && nReadIO[1] == IO_OFF)
		{
			m_lWait_ConvClamp[0] = GetCurrentTime();
			m_bflag_ConvClamp = false;
		}
		else if (m_bflag_ConvClamp == false &&
			nReadIO[0] == IO_ON && nReadIO[1] == IO_OFF)
		{
			if (m_lWait_ConvClamp[2] > st_time.nWait_On[CYL_ACCY_CONV_BOX_CLAMP])
			{
				nFunRet = CTL_GOOD;
			}
		}
		else
		{
			if (m_lWait_ConvClamp[2] > st_time.nWait_Limit[CYL_ACCY_CONV_BOX_CLAMP])
			{
				// 130001 0 00 "PS0701_PS0703_LOAD_BOX_CLAMP_ON_CHK_ERR."
				alarm.mstr_code		= "130001";

				alarm.mn_count_mode	= 0;
				alarm.mn_type_mode	= eWARNING;
				st_work.nEqpStatus	= dWARNING;

				nFunRet = CTL_ERROR;
			}
		}
	}
	return nFunRet;
}

//==================================================================//
// Load Conv Box Pusher
//==================================================================//
void CSeq_BoxClamp::OnSet_ConvBoxPusher(int nzOnOff) 
{
	g_ioMgr.set_out_bit(stIO.o_Load_BoxPusherCylPusher, nzOnOff);
	g_ioMgr.set_out_bit(stIO.o_Load_BoxPusherCylPull, !nzOnOff);
	
	m_bflag_ConvClamp = true;
	m_lWait_ConvClamp[0] = GetCurrentTime();
}

int CSeq_BoxClamp::OnGet_ConvBoxPusher(int nzOnOff) 
{
	int nFunRet = CTL_PROCEED;
	
	int nReadIO[2] = {0,};
	//kwlee 2017.0707
// 	nReadIO[0] = g_ioMgr.get_in_bit(stIO.i_Chk_Load_BoxPusherFwCheck);
// 	nReadIO[1] = g_ioMgr.get_in_bit(stIO.i_Chk_Load_BoxPusherBwCheck);
	
	m_lWait_ConvClamp[1] = GetCurrentTime();
	m_lWait_ConvClamp[2] = m_lWait_ConvClamp[1] - m_lWait_ConvClamp[0];
	
	if (m_lWait_ConvClamp[2] < 0)
	{
		m_lWait_ConvClamp[0] = GetCurrentTime();
	}
	
	if (nzOnOff == IO_OFF)		// 박스를 놓은 상태
	{
		if (m_bflag_ConvClamp == true &&
			nReadIO[0] == IO_OFF && nReadIO[1] == IO_ON)
		{
			m_lWait_ConvClamp[0] = GetCurrentTime();
			m_bflag_ConvClamp = false;
		}
		else if (m_bflag_ConvClamp == false &&
			nReadIO[0] == IO_OFF && nReadIO[1] == IO_ON )
		{
			if (m_lWait_ConvClamp[2] > st_time.nWait_Off[CYL_ACCY_CONV_BOX_CLAMP])
			{
				nFunRet = CTL_GOOD;
			}
		}
		else
		{
			if (m_lWait_ConvClamp[2] > st_time.nWait_Limit[CYL_ACCY_CONV_BOX_CLAMP])
			{
				//  131000 0 00 "PS0702_PS0703_LOAD_BOX_PUSHER_BACKWARD_CHK_ERR."
				alarm.mstr_code		= "131000";
				
				alarm.mn_count_mode	= 0;
				alarm.mn_type_mode	= eWARNING;
				st_work.nEqpStatus	= dWARNING;
				
				nFunRet = CTL_ERROR;
			}
		}
	}
	else		// 박스를 잡은 상태
	{
		if (m_bflag_ConvClamp == true &&
			nReadIO[0] == IO_ON && nReadIO[1] == IO_OFF)
		{
			m_lWait_ConvClamp[0] = GetCurrentTime();
			m_bflag_ConvClamp = false;
		}
		else if (m_bflag_ConvClamp == false &&
			nReadIO[0] == IO_ON && nReadIO[1] == IO_OFF)
		{
			if (m_lWait_ConvClamp[2] > st_time.nWait_On[CYL_ACCY_CONV_BOX_CLAMP])
			{
				nFunRet = CTL_GOOD;
			}
		}
		else
		{
			if (m_lWait_ConvClamp[2] > st_time.nWait_Limit[CYL_ACCY_CONV_BOX_CLAMP])
			{
				// 130001 0 00 "PS0701_PS0703_LOAD_BOX_CLAMP_ON_CHK_ERR."
				alarm.mstr_code		= "130001";
				
				alarm.mn_count_mode	= 0;
				alarm.mn_type_mode	= eWARNING;
				st_work.nEqpStatus	= dWARNING;
				
				nFunRet = CTL_ERROR;
			}
		}
	}
	return nFunRet;
}

//==================================================================//
// 악세사리 박스의 상단을 왼쪽 오른 쪽 잡는 실린더
// - IO_ON = Clamp 상태이며, 상단 박스를 잡음
// - IO_OFF = Unclamp 상태이며, 상단 박스를 놓음
//==================================================================//
void CSeq_BoxClamp::OnSet_CylClamp(int nzOnOff) 
{
	g_ioMgr.set_out_bit(stIO.o_Cyl_AccyBoxClamp, nzOnOff);
	g_ioMgr.set_out_bit(stIO.o_Cyl_AccyBoxUnclamp, !nzOnOff);

	m_bflag_Clamp = true;
	m_lWait_Clamp[0] = GetCurrentTime();
}

int CSeq_BoxClamp::OnGet_CylClamp(int nzOnOff, int nzSite) 
{
	int nFunRet = CTL_PROCEED;

	int nReadIO[4] = {0,};
	nReadIO[0] = g_ioMgr.get_in_bit(stIO.i_Chk_AccyBoxClamp[SIDE_LEFT_]);
	nReadIO[1] = g_ioMgr.get_in_bit(stIO.i_Chk_AccyBoxUnclamp[SIDE_LEFT_]);
	nReadIO[2] = g_ioMgr.get_in_bit(stIO.i_Chk_AccyBoxClamp[SIDE_RIGHT_]);
	nReadIO[3] = g_ioMgr.get_in_bit(stIO.i_Chk_AccyBoxUnclamp[SIDE_RIGHT_]);

	if (nzSite == SIDE_LEFT_)
	{
		// [Right] 센서 입력은 강제로 설정
		nReadIO[2] =  nzOnOff;
		nReadIO[3] = !nzOnOff;
	}
	else if (nzSite == SIDE_RIGHT_)
	{
		// [Left] 센서 입력은 강제로 설정
		nReadIO[0] =  nzOnOff;
		nReadIO[1] = !nzOnOff;
	}

	m_lWait_Clamp[1] = GetCurrentTime();
	m_lWait_Clamp[2] = m_lWait_Clamp[1] - m_lWait_Clamp[0];

	if (m_lWait_Clamp[2] < 0)
	{
		m_lWait_Clamp[0] = GetCurrentTime();
	}

	if (nzOnOff == IO_OFF)		// 박스 상단을 놓은 상태
	{
		if (m_bflag_Clamp == true &&
			nReadIO[0] == IO_OFF && nReadIO[1] == IO_ON &&
			nReadIO[2] == IO_OFF && nReadIO[3] == IO_ON)
		{
			m_lWait_Clamp[0] = GetCurrentTime();
			m_bflag_Clamp = false;
		}
		else if (m_bflag_Clamp == false &&
			nReadIO[0] == IO_OFF && nReadIO[1] == IO_ON &&
			nReadIO[2] == IO_OFF && nReadIO[3] == IO_ON)
		{
			if (m_lWait_Clamp[2] > st_time.nWait_Off[CYL_ACCY_BOX_CLAMP])
			{
				nFunRet = CTL_GOOD;
			}
		}
		else
		{
			if (m_lWait_Clamp[2] > st_time.nWait_Limit[CYL_ACCY_BOX_CLAMP])
			{
				// 133000 0 00 "PS0505_PS0507_BOX_CLAMP_UNCLAMP_CHK_ERR."
				alarm.mstr_code		= "133000";

				alarm.mn_count_mode	= 0;
				alarm.mn_type_mode	= eWARNING;
				st_work.nEqpStatus	= dWARNING;

				nFunRet = CTL_ERROR;
			}
		}
	}
	else		// 박스 상단을 잡은 상태
	{
		if (m_bflag_Clamp == true &&
			nReadIO[0] == IO_ON && nReadIO[1] == IO_OFF &&
			nReadIO[2] == IO_ON && nReadIO[3] == IO_OFF)
		{
			m_lWait_Clamp[0] = GetCurrentTime();
			m_bflag_Clamp = false;
		}
		else if (m_bflag_Clamp == false &&
			nReadIO[0] == IO_ON && nReadIO[1] == IO_OFF &&
			nReadIO[2] == IO_ON && nReadIO[3] == IO_OFF)
		{
			if (m_lWait_Clamp[2] > st_time.nWait_On[CYL_ACCY_BOX_CLAMP])
			{
				nFunRet = CTL_GOOD;	// [OFF:DOWN]
			}
		}
		else
		{
			if (m_lWait_Clamp[2] > st_time.nWait_Limit[CYL_ACCY_BOX_CLAMP])
			{
				//  133001 0 00 "PS0504_PS0506_BOX_CLAMP_CLAMP_CHK_ERR."
				alarm.mstr_code		= "133001";

				alarm.mn_count_mode	= 0;
				alarm.mn_type_mode	= eWARNING;
				st_work.nEqpStatus	= dWARNING;

				nFunRet = CTL_ERROR;
			}
		}
	}
	return nFunRet;
}

//==================================================================//
// 악세사리 박스를 작업 영역로 가져가기 전 스토퍼
// - IO_ON = UP 상태에서 작업 영역에 악세사리 박스를 가져다 놓은 상태
// - IO_OFF = Down 상태에서 작업 영역으로 악세사리 박스를 가져가기 위한 상태
//==================================================================//
void CSeq_BoxClamp::OnSet_CylStopper(int nzOnOff) 
{
	g_ioMgr.set_out_bit(stIO.o_Cyl_AccyBoxConvStopperUp, nzOnOff);
	g_ioMgr.set_out_bit(stIO.o_Cyl_AccyBoxConvStopperDn, !nzOnOff);

	m_bflag_Stopper = true;
	m_lWait_Stopper[0] = GetCurrentTime();
}

int CSeq_BoxClamp::OnGet_CylStopper(int nzOnOff, int nzSite) 
{
	int nFunRet = CTL_PROCEED;

	int nReadIO[2] = {0,};
	nReadIO[0] = g_ioMgr.get_in_bit(stIO.i_Chk_AccyBoxConvStopperUp);
	nReadIO[1] = g_ioMgr.get_in_bit(stIO.i_Chk_AccyBoxConvStopperDn);

	if (nzSite == SIDE_UP_)
	{
		// [Down] 센서 입력은 강제로 설정
		nReadIO[1] = !nzOnOff;
	}
	else if (nzSite == SIDE_DOWN_)
	{
		// [UP] 센서 입력은 강제로 설정
		nReadIO[0] =  !nzOnOff;
	}

	m_lWait_Stopper[1] = GetCurrentTime();
	m_lWait_Stopper[2] = m_lWait_Stopper[1] - m_lWait_Stopper[0];

	if (m_lWait_Stopper[2] < 0)
	{
		m_lWait_Stopper[0] = GetCurrentTime();
	}

	if (nzOnOff == IO_OFF)		// 스토퍼 내려간 상태
	{
		if (m_bflag_Stopper == true &&
			nReadIO[0] == IO_OFF && nReadIO[1] == IO_ON)
		{
			m_lWait_Stopper[0] = GetCurrentTime();
			m_bflag_Stopper = false;
		}
		else if (m_bflag_Stopper == false &&
			nReadIO[0] == IO_OFF && nReadIO[1] == IO_ON)
		{
			if (m_lWait_Stopper[2] > st_time.nWait_Off[CYL_ACCY_BOX_STOPPER])
			{
				nFunRet = CTL_GOOD;	
			}
		}
		else
		{
			if (m_lWait_Stopper[2] > st_time.nWait_Limit[CYL_ACCY_BOX_STOPPER])
			{
				// 120100 0 00 "PS0303_LOADING_CONV_STOPPER_DOWN_CHK_ERR."
				alarm.mstr_code		= "120100";

				alarm.mn_count_mode	= 0;
				alarm.mn_type_mode	= eWARNING;
				st_work.nEqpStatus	= dWARNING;

				nFunRet = CTL_ERROR;
			}
		}
	}
	else		// 스토퍼 올라온 상태
	{
		if (m_bflag_Stopper == true &&
			nReadIO[0] == IO_ON && nReadIO[1] == IO_OFF)
		{
			m_lWait_Stopper[0] = GetCurrentTime();
			m_bflag_Stopper = false;
		}
		else if (m_bflag_Stopper == false &&
			nReadIO[0] == IO_ON && nReadIO[1] == IO_OFF)
		{
			if (m_lWait_Stopper[2] > st_time.nWait_On[CYL_ACCY_BOX_STOPPER])
			{
				nFunRet = CTL_GOOD;	
			}
		}
		else
		{
			if (m_lWait_Stopper[2] > st_time.nWait_Limit[CYL_ACCY_BOX_STOPPER])
			{
				// 120101 0 00 "PS0302_LOADING_CONV_STOPPER_UP_CHK_ERR."
				alarm.mstr_code		= "120101";
				nFunRet = CTL_ERROR;
			}
		}
	}
	return nFunRet;
}

//==================================================================//
// 악세사리 박스를 작업 영역까지 옴기는 실린더
// - IO_ON = Fw 상태이며, 박스의 위치가 작업 영역
// - IO_OFF = Bw 상태이며, 짚는 영역
//==================================================================//
void CSeq_BoxClamp::OnSet_CylSupply(int nzOnOff) 
{
// 	g_ioMgr.set_out_bit(stIO.o_Cyl_AccyBoxSupplyFw, nzOnOff);
// 	g_ioMgr.set_out_bit(stIO.o_Cyl_AccyBoxSupplyBw, !nzOnOff);

	m_bflag_Supply = true;
	m_lWait_Supply[0] = GetCurrentTime();
}

int CSeq_BoxClamp::OnGet_CylSupply(int nzOnOff, int nzSite)  
{
	int nFunRet = CTL_PROCEED;

	int nReadIO[2] = {0,};
// 	nReadIO[0] = g_ioMgr.get_in_bit(stIO.i_Chk_AccyBoxSupplyFw);
// 	nReadIO[1] = g_ioMgr.get_in_bit(stIO.i_Chk_AccyBoxSupplyBw);

	if (nzSite == SIDE_FW_)
	{
		// [Bw] 센서 입력은 강제로 설정
		nReadIO[1] = !nzOnOff;
	}
	else if (nzSite == SIDE_BW_)
	{
		// [Fw] 센서 입력은 강제로 설정
		nReadIO[0] =  !nzOnOff;
	}

	m_lWait_Supply[1] = GetCurrentTime();
	m_lWait_Supply[2] = m_lWait_Supply[1] - m_lWait_Supply[0];

	if (m_lWait_Supply[2] < 0)
	{
		m_lWait_Supply[0] = GetCurrentTime();
	}

	if (nzOnOff == IO_OFF)		// 박스를 짚는 영역 상태
	{
		if (m_bflag_Supply == true &&
			nReadIO[0] == IO_OFF && nReadIO[1] == IO_ON )
		{
			m_lWait_Supply[0] = GetCurrentTime();
			m_bflag_Supply = false;
		}
		else if (m_bflag_Supply == false &&
			nReadIO[0] == IO_OFF && nReadIO[1] == IO_ON)
		{
			if (m_lWait_Supply[2] > st_time.nWait_Off[CYL_ACCY_BOX_SUPPLY])
			{
				nFunRet = CTL_GOOD;
			}
		}
		else
		{
			if (m_lWait_Supply[2] > st_time.nWait_Limit[CYL_ACCY_BOX_SUPPLY])
			{
				
				// 133001 0 00 "PS0707_BOX_CLAMP_BOX_SUPPLY_BW_CHK_ERR."
				alarm.mstr_code		= "133001";
				
				alarm.mn_count_mode	= 0;
				alarm.mn_type_mode	= eWARNING;
				st_work.nEqpStatus	= dWARNING;
				
				nFunRet = CTL_ERROR;
			}
		}
	}
	else		// 박스를 작업 하는 상태
	{
		if (m_bflag_Supply == true &&
			nReadIO[0] == IO_ON && nReadIO[1] == IO_OFF)
		{
			m_lWait_Supply[0] = GetCurrentTime();
			m_bflag_Supply = false;
		}
		else if (m_bflag_Supply == false &&
			nReadIO[0] == IO_ON && nReadIO[1] == IO_OFF)
		{
			if (m_lWait_Supply[2] > st_time.nWait_On[CYL_ACCY_BOX_SUPPLY])
			{
				nFunRet = CTL_GOOD;	// [OFF:DOWN]
			}
		}
		else
		{
			if (m_lWait_Supply[2] > st_time.nWait_Limit[CYL_ACCY_BOX_SUPPLY])
			{
				// 133000 0 00 "PS0706_BOX_CLAMP_BOX_SUPPLY_FW_CHK_ERR."
				alarm.mstr_code		= "133000";
				
				alarm.mn_count_mode	= 0;
				alarm.mn_type_mode	= eWARNING;
				st_work.nEqpStatus	= dWARNING;

				nFunRet = CTL_ERROR;
			}
		}
	}
	return nFunRet;
}

//==================================================================//
// 악세사리 박스를 위, 아래로 움직이는 실린더
// - IO_ON = UP 상태이다
// - IO_OFF = DOWN 상태이다
//==================================================================//
void CSeq_BoxClamp::OnSet_CylUDGripper(int nzOnOff) 
{
	g_ioMgr.set_out_bit(stIO.o_Cyl_AccyBoxGripperUp, nzOnOff);
	g_ioMgr.set_out_bit(stIO.o_Cyl_AccyBoxGripperDn, !nzOnOff);

	m_bflag_UDGripper = true;
	m_lWait_UDGripper[0] = GetCurrentTime();
}

int CSeq_BoxClamp::OnGet_CylUDGripper(int nzOnOff, int nzSite) 
{
	int nFunRet = CTL_PROCEED;

	int nReadIO[4] = {0,};
	nReadIO[0] = g_ioMgr.get_in_bit(stIO.i_Chk_AccyBoxGripperUp[SIDE_LEFT_]);
	nReadIO[1] = g_ioMgr.get_in_bit(stIO.i_Chk_AccyBoxGripperDn[SIDE_LEFT_]);
	nReadIO[2] = g_ioMgr.get_in_bit(stIO.i_Chk_AccyBoxGripperUp[SIDE_RIGHT_]);
	nReadIO[3] = g_ioMgr.get_in_bit(stIO.i_Chk_AccyBoxGripperDn[SIDE_RIGHT_]);

	if (nzSite == SIDE_LEFT_)
	{
		// [Right] 센서 입력은 강제로 설정
		nReadIO[2] =  nzOnOff;
		nReadIO[3] = !nzOnOff;
	}
	else if (nzSite == SIDE_RIGHT_)
	{
		// [Left] 센서 입력은 강제로 설정
		nReadIO[0] =  nzOnOff;
		nReadIO[1] = !nzOnOff;
	}

	m_lWait_UDGripper[1] = GetCurrentTime();
	m_lWait_UDGripper[2] = m_lWait_UDGripper[1] - m_lWait_UDGripper[0];

	if (m_lWait_UDGripper[2] < 0)
	{
		m_lWait_UDGripper[0] = GetCurrentTime();
	}

	if (nzOnOff == IO_OFF)		// 박스를 Down 한 상태
	{
		if (m_bflag_UDGripper == true &&
			nReadIO[0] == IO_OFF && nReadIO[1] == IO_ON &&
			nReadIO[2] == IO_OFF && nReadIO[3] == IO_ON)
		{
			m_lWait_UDGripper[0] = GetCurrentTime();
			m_bflag_UDGripper = false;
		}
		else if (m_bflag_UDGripper == false &&
			nReadIO[0] == IO_OFF && nReadIO[1] == IO_ON &&
			nReadIO[2] == IO_OFF && nReadIO[3] == IO_ON)
		{
			if (m_lWait_UDGripper[2] > st_time.nWait_Off[CYL_ACCY_BOX_UD_GRIPPER])
			{
				nFunRet = CTL_GOOD;	// [OFF:DOWN]
			}
		}
		else
		{
			if (m_lWait_UDGripper[2] > st_time.nWait_Limit[CYL_ACCY_BOX_UD_GRIPPER])
			{
				// 134000 0 00 "PS0501_BOX_CLAMP_GRIPPER_DOWN_CHK_ERR."
				alarm.mstr_code		= "134000";

				alarm.mn_count_mode	= 0;
				alarm.mn_type_mode	= eWARNING;
				st_work.nEqpStatus	= dWARNING;

				nFunRet = CTL_ERROR;
			}
		}
	}
	else		// 박스를 UP 한 상태
	{
		if (m_bflag_UDGripper == true &&
			nReadIO[0] == IO_ON && nReadIO[1] == IO_OFF &&
			nReadIO[2] == IO_ON && nReadIO[3] == IO_OFF)
		{
			m_lWait_UDGripper[0] = GetCurrentTime();
			m_bflag_UDGripper = false;
		}
		else if (m_bflag_UDGripper == false &&
			nReadIO[0] == IO_ON && nReadIO[1] == IO_OFF &&
			nReadIO[2] == IO_ON && nReadIO[3] == IO_OFF)
		{
			if (m_lWait_UDGripper[2] > st_time.nWait_On[CYL_ACCY_BOX_UD_GRIPPER])
			{
				nFunRet = CTL_GOOD;	// [OFF:DOWN]
			}
		}
		else
		{
			if (m_lWait_UDGripper[2] > st_time.nWait_Limit[CYL_ACCY_BOX_UD_GRIPPER])
			{
				// 134001 0 00 "PS0500_BOX_CLAMP_GRIPPER_UP_CHK_ERR."
				alarm.mstr_code		= "134001";

				alarm.mn_count_mode	= 0;
				alarm.mn_type_mode	= eWARNING;
				st_work.nEqpStatus	= dWARNING;

				nFunRet = CTL_ERROR;
			}
		}
	}
	return nFunRet;
}

//==================================================================//
// 악세사리 박스를 받쳐주고, 배출을 위해 내려주는 실린더
// - IO_ON = UP 상태이며, 박스를 받침
// - IO_OFF = Down 상태이며, 박스를 내림
//==================================================================//
void CSeq_BoxClamp::OnSet_CylLifter(int nzOnOff) 
{
	g_ioMgr.set_out_bit(stIO.o_Cyl_AccyBoxLifterUp, nzOnOff);
	g_ioMgr.set_out_bit(stIO.o_Cyl_AccyBoxLifterDn, !nzOnOff);

	m_bflag_Lifter = true;
	m_lWait_Lifter[0] = GetCurrentTime();
}

int CSeq_BoxClamp::OnGet_CylLifter(int nzOnOff, int nzSite) 
{
	int nFunRet = CTL_PROCEED;

	int nReadIO[4] = {0,};
	nReadIO[0] = g_ioMgr.get_in_bit(stIO.i_Chk_AccyBoxLifterUp[SIDE_LEFT_]);
	nReadIO[1] = g_ioMgr.get_in_bit(stIO.i_Chk_AccyBoxLifterDn[SIDE_LEFT_]);
	nReadIO[2] = g_ioMgr.get_in_bit(stIO.i_Chk_AccyBoxLifterUp[SIDE_RIGHT_]);
	nReadIO[3] = g_ioMgr.get_in_bit(stIO.i_Chk_AccyBoxLifterDn[SIDE_RIGHT_]);

	if (nzSite == SIDE_LEFT_)
	{
		// [Right] 센서 입력은 강제로 설정
		nReadIO[2] =  nzOnOff;
		nReadIO[3] = !nzOnOff;
	}
	else if (nzSite == SIDE_RIGHT_)
	{
		// [Left] 센서 입력은 강제로 설정
		nReadIO[0] =  nzOnOff;
		nReadIO[1] = !nzOnOff;
	}

	m_lWait_Lifter[1] = GetCurrentTime();
	m_lWait_Lifter[2] = m_lWait_Lifter[1] - m_lWait_Lifter[0];

	if (m_lWait_Lifter[2] < 0)
	{
		m_lWait_Lifter[0] = GetCurrentTime();
	}

	if (nzOnOff == IO_OFF)		// 박스 배출 위치
	{
		if (m_bflag_Lifter == true &&
			nReadIO[0] == IO_OFF && nReadIO[1] == IO_ON &&
			nReadIO[2] == IO_OFF && nReadIO[3] == IO_ON)
		{
			m_lWait_Lifter[0] = GetCurrentTime();
			m_bflag_Lifter = false;
		}
		else if (m_bflag_Lifter == false &&
			nReadIO[0] == IO_OFF && nReadIO[1] == IO_ON &&
			nReadIO[2] == IO_OFF && nReadIO[3] == IO_ON)
		{
			if (m_lWait_Lifter[2] > st_time.nWait_Off[CYL_ACCY_BOX_LIFTER])
			{
				nFunRet = CTL_GOOD;	
			}
		}
		else
		{
			if (m_lWait_Lifter[2] > st_time.nWait_Limit[CYL_ACCY_BOX_LIFTER])
			{
				// 135000 0 00 "PS0401_PS0403_BOX_LIFTER_DOWN_CHK_ERR."
				alarm.mstr_code		= "135000";

				alarm.mn_count_mode	= 0;
				alarm.mn_type_mode	= eWARNING;
				st_work.nEqpStatus	= dWARNING;

				nFunRet = CTL_ERROR;
			}
		}
	}
	else		// 박스 하단 고정 위치
	{
		if (m_bflag_Lifter == true &&
			nReadIO[0] == IO_ON && nReadIO[1] == IO_OFF &&
			nReadIO[2] == IO_ON && nReadIO[3] == IO_OFF)
		{
			m_lWait_Lifter[0] = GetCurrentTime();
			m_bflag_Lifter = false;
		}
		else if (m_bflag_Lifter == false &&
			nReadIO[0] == IO_ON && nReadIO[1] == IO_OFF &&
			nReadIO[2] == IO_ON && nReadIO[3] == IO_OFF)
		{
			if (m_lWait_Lifter[2] > st_time.nWait_On[CYL_ACCY_BOX_LIFTER])
			{
				nFunRet = CTL_GOOD;	
			}
		}
		else
		{
			if (m_lWait_Lifter[2] > st_time.nWait_Limit[CYL_ACCY_BOX_LIFTER])
			{
				// 135001 0 00 "PS0400_PS0402_BOX_LIFTER_UP_CHK_ERR."
				alarm.mstr_code		= "135001";

				alarm.mn_count_mode	= 0;
				alarm.mn_type_mode	= eWARNING;
				st_work.nEqpStatus	= dWARNING;

				nFunRet = CTL_ERROR;
			}
		}
	}
	return nFunRet;
}



