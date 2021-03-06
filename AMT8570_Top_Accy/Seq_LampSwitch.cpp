// Seq_LampSwitch.cpp: implementation of the CSeq_LampSwitch class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Seq_LampSwitch.h"

#include "MyJamData.h"
#include "FastechPublic_IO.h"
#include "Dialog_Message.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CSeq_LampSwitch	seqSwitch;

CSeq_LampSwitch::CSeq_LampSwitch()
{

}

CSeq_LampSwitch::~CSeq_LampSwitch()
{

}

//==================================================================//
// 쓰레드 동작
//==================================================================//
void CSeq_LampSwitch::OnSeq_RUN(void) 
{
	OnSeq_Init();     // 쓰레드 관련 변수 초기화
	OnSeq_Execute();  // 쓰레드 제어
}

//==================================================================//
// 쓰레드 관련 변수 초기화
//==================================================================//
void CSeq_LampSwitch::OnSeq_Init(void) 
{
	if (m_bFlagINIT == true)	{
		return;
	}
	m_bFlagINIT = true;

	m_nStep_Start	= 0;
	m_nStep_Stop	= 0;
	m_nStep_Reset	= 0;
	m_nStep_Buzz	= 0;

	m_nStep_LoadingStart = 0;
	m_nStep_ManualLoading = 0;

	m_nStep_Auto = 0;
	m_nStep_Manual = 0;

	m_nStep_EqpStatus	= 0;
	m_nStep_StatusAlarm	= 0;

	// 규이리 추가 [2014.12.16]
	m_nLockStep = 0;
	m_nDoorOpenTry = FALSE;
}

//==================================================================//
// 타워 랩프 시컨스 제어
//==================================================================//
void CSeq_LampSwitch::OnSeq_Execute(void) 
{
	OnPush_Start();	// START 스위치 눌림 처리
	OnPush_Stop();	// STOP 스위치 눌림 처리
	OnPush_AlarmClear();// 알람 해제 스위치 눌림 처리
	OnPush_BuzzerOff();	// BUZZER OFF 스위치 눌림 처리
	OnPush_LoadingStart();	// ManualLoading 스위치 눌림 처리
	OnPush_ManualLoading();	// LoadingStart 스위치 눌림 처리
	OnReport_2MainStatusAlarm();	// 설비 상태 보고 [ALARM]
	OnTurn_Auto();		// [Auto] 상태로 스위치 돌린 경우 처리
	OnTurn_Manual();	// [Manual] 상태로 스위치 돌린 경우 처리

	// 규이리 추가 [2014.12.17]
	OnRun_DoorLock();	// 모든 DOOR 닫힌 경우 강제로 DOOR LOCK 처리
}

//==================================================================//
// START 스위치 눌림 처리
//==================================================================//
void CSeq_LampSwitch::OnPush_Start() 
{
	// 강제 리턴 조건 검사
	if (st_handler.nSysLock != FALSE)
	{
		return;
	}
	if (st_handler.nSwitchLock != FALSE)
	{
		return;
	}
	if (st_work.nEqpStatus == dRUN)
	{
		return;
	}
	// yangkyu [2015.04.11]
	if (st_work.nForceDischarge == FORCEDISCHARGE_ON)
	{
		return;
	}
	int nRet = VAR_INIT_;

	switch(m_nStep_Start)
	{
	case 0:
		// [STOP] 상태에서만 [START] 동작하도록 제약조건 추가
		if (st_work.nEqpStatus == dSTOP)
		{
			// 메인 화면에서 [START] 버튼이 눌렸는지 확인
			if ((st_work.nReq_StrtProc) == REQ_YES)
			{
				// 메인 화면에서 [START 버튼] 선택된 경우 처리
				// : 이 경우에는 I/O 입력이 발생하지 않기 때문에 플래그로 처리
				st_work.nReq_StrtProc = REQ_NONE;

				m_nStep_Start = 600;
				break;
			}

			if (g_ioMgr.get_in_bit(stIO.i_Chk_StartSwitch) == IO_ON)
			{
				// 만일 STOP 버튼이 함께 눌린 경우에는 무시
				if (g_ioMgr.get_in_bit(stIO.i_Chk_StopSwitch) == IO_ON)
				{
					break;
				}
				OnRest_SwitchStep(0);	// 버튼 눌림 처리 스텝 정보 초기화

				m_lWaitTimeSW[0] = GetCurrentTime();
				m_nStep_Start = 200;
			}
		}
		break;

	case 200:
		m_lWaitTimeSW[1] = GetCurrentTime();
		m_lWaitTimeSW[2] = m_lWaitTimeSW[1] - m_lWaitTimeSW[0];
		if (m_lWaitTimeSW[2] < 0)
		{
			m_lWaitTimeSW[0] = GetCurrentTime();
			break;
		}

		// [50ms] 동안 버튼이 계속 눌린 경우에만 받아들임
		// - 시그널이 튀는 경우에 대해서는 버튼 눌림으로 처리하지 않음
		if (m_lWaitTimeSW[2] > 50 &&
			g_ioMgr.get_in_bit(stIO.i_Chk_StartSwitch) == IO_OFF)
		{
			m_lWaitTimeSW[0] = GetCurrentTime();
			m_nStep_Start = 300;
		}
		else if (g_ioMgr.get_in_bit(stIO.i_Chk_StartSwitch) == IO_OFF)
		{
			// 순간적으로 입력이 튀는 경우에는 무시
			m_nStep_Start = 0;
		}
		break;

	case 300:
		m_lWaitTimeSW[1] = GetCurrentTime();
		m_lWaitTimeSW[2] = m_lWaitTimeSW[1] - m_lWaitTimeSW[0];
		if (m_lWaitTimeSW[2] < 0)
		{
			m_lWaitTimeSW[0] = GetCurrentTime();
			break;
		}

		if (m_lWaitTimeSW[2] > 10)
		{
			if (g_ioMgr.get_in_bit(stIO.i_Chk_StartSwitch) == IO_OFF)
			{
				m_nStep_Start = 600;
			}
			else
			{
				m_nStep_Start = 0;
			}
		}
		break;

	case 600:
		// 스위치 버튼 동작 조건 검사
		if (OnCheckSwitchRunCondition() == TRUE)
		{
			m_nStep_Start = 700;
		}
		else
		{
			m_nStep_Start = 0;
		}
		break;

	case 700:
		// 초기화 작업 완료 후에 자동으로 [RUN] 상태로 변경
		if (st_work.nEqpStatus != dSTOP &&
			st_work.nEqpStatus != dINIT)
		{
			break;
		}

		// 복구 동작을 우선 처리함
		//nRet = Func.Robot_Reinstatement_Position(1);
		// baeksh [test]
		nRet = CTLBD_RET_GOOD;
		if (nRet != CTLBD_RET_GOOD)
		{
			// 위 조건을 주석 처리하는 경우 대비해서 [STOP] 조건 추가함
			if (st_work.nEqpStatus != dSTOP)
			{
				m_nStep_Start = 0;
			}
			break;
		}

		// 알람 화면이 출력된 상태에서 [START] 버튼 누른 경우 : 알람 화면 우선 종료시킴
		if (st_handler.nShowAlarmDlg != FALSE)
		{
			::PostMessage(st_handler.hWnd, WM_FORM_CHANGE, 7, 2);
		}
		// 메인 화면으로 전환 요청
		if (st_handler.nSelectedMenuNum != 101)
		{
			::PostMessage(st_handler.hWnd, WM_FORM_CHANGE, 1, 1);
		}

		Func.OnSet_EqpStatusRun();	// [RUN] 설비 상태 변경
		//m_nStep_Start = 0;
		// 규이리 수정 [2014.07.31]
		m_nStep_Start = 2000;
		break;

		//==========================================================//
		// 규이리 추가 [2014.07.31]
		//==========================================================//
	case 2000:
		// 다른 상태보고 함수에서 이미 OnReport_2CimEqpStatus() 함수 호출했다면 처리중에 있을 것임
		// - 이런 경우는 이전 작업이 완료되기를 기다림
		if (m_nStep_EqpStatus != 0)
		{
			break;
		}
		//m_nStep_EqpStatus = 0;

		m_nStep_Start = 2010;
		break;

	case 2010:
		m_nStep_Start = 0;
		break;
		//----------------------------------------------------------//
	}
}

//==================================================================//
// STOP 스위치 눌림 처리
//==================================================================//
void CSeq_LampSwitch::OnPush_Stop() 
{
	// 강제 리턴 조건 검사
	if (st_handler.nSysLock != FALSE)
	{
		return;
	}
	int nRet;

	switch(m_nStep_Stop)
	{
	case 0:
		// [STOP] 상태가 아닌 경우에만 [STOP] 시컨스 실행시킴
		if (st_work.nEqpStatus != dSTOP)
		{
			if (g_ioMgr.get_in_bit(stIO.i_Chk_StopSwitch) == IO_ON)
			{
				// 만일 START 버튼이 함께 눌린 경우에는 무시
				if (g_ioMgr.get_in_bit(stIO.i_Chk_StartSwitch) == IO_ON)
				{
					break;
				}
				OnRest_SwitchStep(1);	// 버튼 눌림 처리 스텝 정보 초기화

				// 규이리 수정 [2014.07.29]
				m_lWaitTimeSW[0] = GetCurrentTime();
				m_nStep_Stop = 200;
			}
		}
		break;

	case 200:
		m_lWaitTimeSW[1] = GetCurrentTime();
		m_lWaitTimeSW[2] = m_lWaitTimeSW[1] - m_lWaitTimeSW[0];
		if (m_lWaitTimeSW[2] < 0)
		{
			m_lWaitTimeSW[0] = GetCurrentTime();
			break;
		}

		// [50ms] 동안 버튼이 계속 눌린 경우에만 받아들임
		// - 시그널이 튀는 경우에 대해서는 버튼 눌림으로 처리하지 않음
		if (m_lWaitTimeSW[2] > 50 &&
			g_ioMgr.get_in_bit(stIO.i_Chk_StopSwitch) == IO_OFF)
		{
			m_lWaitTimeSW[0] = GetCurrentTime();
			m_nStep_Stop = 300;
		}
		else if (g_ioMgr.get_in_bit(stIO.i_Chk_StopSwitch) == IO_OFF)
		{
			m_nStep_Stop = 0;
		}
		break;

	case 300:
		m_lWaitTimeSW[1] = GetCurrentTime();
		m_lWaitTimeSW[2] = m_lWaitTimeSW[1] - m_lWaitTimeSW[0];
		if (m_lWaitTimeSW[2] < 0)
		{
			m_lWaitTimeSW[0] = GetCurrentTime();
			break;
		}

		if (m_lWaitTimeSW[2] > 10)
		{
			if (g_ioMgr.get_in_bit(stIO.i_Chk_StopSwitch) == IO_OFF)
			{
				m_nStep_Stop = 600;
			}
			else
			{
				m_nStep_Stop = 0;
			}
		}
		break;

	case 600:
		if (g_ioMgr.get_in_bit(stIO.i_Chk_StopSwitch) == IO_OFF)
		{
			// 초기화 작업 중 STOP 버튼 눌러서 초기화 취소됨
			// - 초기화 작업 중 에러 발생 상태 설정 [에러발생]
			// - 초기화 화면의 프로그레스바 리셋 처리함
			if (st_work.nEqpStatus == dINIT)
			{
				// 초기화 작업에서 에러 반환하도록 설정
				st_handler.nStopPushInInitial = TRUE;
				break;
			}

			// 메인 화면으로 전환 요청
			if (st_handler.nSelectedMenuNum != 101)
			{
				::PostMessage(st_handler.hWnd, WM_FORM_CHANGE, 1, 1);
			}

			Func.OnSet_EqpStatusStop();	// [STOP] 설비 상태 변경 
			//m_nStep_Stop = 0;
			// 규이리 수정 [2014.07.31]
			m_nStep_Stop = 2000;
		}
		break;

		//==========================================================//
		// 규이리 추가 [2014.07.31]
		//==========================================================//
	case 2000:
		// 다른 상태보고 함수에서 이미 OnReport_2CimEqpStatus() 함수 호출했다면 처리중에 있을 것임
		// - 이런 경우는 이전 작업이 완료되기를 기다림
		if (m_nStep_EqpStatus != 0)
		{
			break;
		}
		//m_nStep_EqpStatus = 0;

		m_nStep_Stop = 2010;
		break;
		
	case 2010:
		nRet = OnReport_2MainEqpStatus(dSTOP);	// [EQP<->HOST] 설비 상태 보고
		if (nRet == RET_GOOD)
		{
			m_nStep_Stop = 0;
		}
		else if (nRet == RET_ERROR)
		{
			m_nStep_Stop = 0;
		}
		break;
		//----------------------------------------------------------//
	}
}

//==================================================================//
// 알람 해제 스위치 눌림 처리
//==================================================================//
void CSeq_LampSwitch::OnPush_AlarmClear() 
{
	// 강제 리턴 조건 검사
	if (st_handler.nSysLock != FALSE)
	{
		return;
	}

	switch(m_nStep_Reset)
	{
	case 0:
		if (g_ioMgr.get_in_bit(stIO.i_Chk_JamClearSwitch) == IO_ON)
		{
			OnRest_SwitchStep(2);	// 버튼 눌림 처리 스텝 정보 초기화

			m_lWaitTimeSW[0] = GetCurrentTime();
			m_nStep_Reset = 200;
		}
		break;

	case 200:
		m_lWaitTimeSW[1] = GetCurrentTime();
		m_lWaitTimeSW[2] = m_lWaitTimeSW[1] - m_lWaitTimeSW[0];
		if (m_lWaitTimeSW[2] < 0)
		{
			m_lWaitTimeSW[0] = GetCurrentTime();
			break;
		}

		// [50ms] 동안 버튼이 계속 눌린 경우에만 받아들임
		// - 시그널이 튀는 경우에 대해서는 버튼 눌림으로 처리하지 않음
		if (m_lWaitTimeSW[2] > 50 &&
			g_ioMgr.get_in_bit(stIO.i_Chk_JamClearSwitch) == IO_OFF)
		{
			m_lWaitTimeSW[0] = GetCurrentTime();
			m_nStep_Reset = 300;
		}
		else if (g_ioMgr.get_in_bit(stIO.i_Chk_JamClearSwitch) == IO_OFF)
		{
			m_nStep_Reset = 0;
		}
		break;

	case 300:
		m_lWaitTimeSW[1] = GetCurrentTime();
		m_lWaitTimeSW[2] = m_lWaitTimeSW[1] - m_lWaitTimeSW[0];
		if (m_lWaitTimeSW[2] < 0)
		{
			m_lWaitTimeSW[0] = GetCurrentTime();
			break;
		}

		if (m_lWaitTimeSW[2] > 10)
		{
			if (g_ioMgr.get_in_bit(stIO.i_Chk_JamClearSwitch) == IO_OFF)
			{
				m_nStep_Reset = 400;
			}
			else
			{
				m_nStep_Reset = 0;
			}
		}
		break;

	case 400:
		if (g_ioMgr.get_in_bit(stIO.i_Chk_JamClearSwitch) == IO_OFF)
		{
			m_nStep_Reset = 600;
		}
		break;

	case 600:
		// 알람 리셋 작업이 필요한 경우인지 확인
		// [RUN 상태]     : 사운드만 종료
		// [RUN 상태 아님] : [STOP] 상태로 변경
		alarm.nReset_AlarmStatus = CTL_NO;	// 알람 리셋 작업 필요 없음

		if (st_handler.cwnd_list != NULL)
		{
			sprintf(st_msg.c_normal_msg, "[ALARM CLEAR] JAM RESET.");
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
		}

		if (st_work.nEqpStatus != dRUN)
		{
			Func.OnSet_EqpStatusStop();	// [STOP] 설비 상태 변경 
		}
		Func.OnSet_BuzzerSound(IO_OFF, 0);	// 부저 사운드 ON/OFF
		g_ioMgr.set_out_bit(stIO.o_LampJamClear, IO_OFF);

		m_nStep_Reset = 0;
		break;
	}
}

//==================================================================//
// BUZZER OFF 스위치 눌림 처리
//==================================================================//
void CSeq_LampSwitch::OnPush_BuzzerOff() 
{
	// 강제 리턴 조건 검사
	if (st_handler.nSysLock != FALSE)
	{
		return;
	}

	switch(m_nStep_Buzz)
	{
	case 0:
		if (g_ioMgr.get_in_bit(stIO.i_Chk_BuzzStopSwitch) == IO_ON)
		{
			OnRest_SwitchStep(3);	// 버튼 눌림 처리 스텝 정보 초기화

			m_lWaitTimeSW[0] = GetCurrentTime();
			m_nStep_Buzz = 200;
		}
		break;

	case 200:
		m_lWaitTimeSW[1] = GetCurrentTime();
		m_lWaitTimeSW[2] = m_lWaitTimeSW[1] - m_lWaitTimeSW[0];
		if (m_lWaitTimeSW[2] < 0)
		{
			m_lWaitTimeSW[0] = GetCurrentTime();
			break;
		}

		// [50ms] 동안 버튼이 계속 눌린 경우에만 받아들임
		// - 시그널이 튀는 경우에 대해서는 버튼 눌림으로 처리하지 않음
		if (m_lWaitTimeSW[2] > 50 &&
			g_ioMgr.get_in_bit(stIO.i_Chk_BuzzStopSwitch) == IO_OFF)
		{
			m_lWaitTimeSW[0] = GetCurrentTime();
			m_nStep_Buzz = 300;
		}
		else if (g_ioMgr.get_in_bit(stIO.i_Chk_BuzzStopSwitch) == IO_OFF)
		{
			m_nStep_Buzz = 0;
		}
		break;

	case 300:
		m_lWaitTimeSW[1] = GetCurrentTime();
		m_lWaitTimeSW[2] = m_lWaitTimeSW[1] - m_lWaitTimeSW[0];
		if (m_lWaitTimeSW[2] < 0)
		{
			m_lWaitTimeSW[0] = GetCurrentTime();
			break;
		}

		if (m_lWaitTimeSW[2] > 10)
		{
			if (g_ioMgr.get_in_bit(stIO.i_Chk_BuzzStopSwitch) == IO_OFF)
			{
				m_nStep_Buzz = 400;
			}
			else
			{
				m_nStep_Buzz = 0;
			}
		}
		break;

	case 400:
		if (g_ioMgr.get_in_bit(stIO.i_Chk_BuzzStopSwitch) == IO_OFF)
		{
			m_nStep_Buzz = 600;
		}
		break;

	case 600:
		if (st_handler.cwnd_list != NULL)
		{
			sprintf(st_msg.c_normal_msg, "[SOUND] OFF.");
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
		}

		Func.OnSet_BuzzerSound(IO_OFF, 0);	// 부저 사운드 ON/OFF
		m_nStep_Buzz = 0;
		break;
	}
}

//==================================================================//
// ManualLoading 스위치 눌림 처리
//==================================================================//
void CSeq_LampSwitch::OnPush_ManualLoading() 
{
	// 강제 리턴 조건 검사
	if (st_handler.nSysLock != FALSE)
	{
		return;
	}

	switch(m_nStep_ManualLoading)
	{
	case 0:
		if (g_ioMgr.get_in_bit(stIO.i_Chk_ManualLoadingSwitch) == IO_ON)
		{
			OnRest_SwitchStep(4);	// 버튼 눌림 처리 스텝 정보 초기화

			m_lWaitTimeSW[0] = GetCurrentTime();
			m_nStep_ManualLoading = 200;
		}
		break;

	case 200:
		m_lWaitTimeSW[1] = GetCurrentTime();
		m_lWaitTimeSW[2] = m_lWaitTimeSW[1] - m_lWaitTimeSW[0];
		if (m_lWaitTimeSW[2] < 0)
		{
			m_lWaitTimeSW[0] = GetCurrentTime();
			break;
		}

		// [50ms] 동안 버튼이 계속 눌린 경우에만 받아들임
		// - 시그널이 튀는 경우에 대해서는 버튼 눌림으로 처리하지 않음
		if (m_lWaitTimeSW[2] > 50 &&
			g_ioMgr.get_in_bit(stIO.i_Chk_ManualLoadingSwitch) == IO_OFF)
		{
			m_lWaitTimeSW[0] = GetCurrentTime();
			m_nStep_ManualLoading = 300;
		}
		else if (g_ioMgr.get_in_bit(stIO.i_Chk_ManualLoadingSwitch) == IO_OFF)
		{
			m_nStep_ManualLoading = 0;
		}
		break;

	case 300:
		m_lWaitTimeSW[1] = GetCurrentTime();
		m_lWaitTimeSW[2] = m_lWaitTimeSW[1] - m_lWaitTimeSW[0];
		if (m_lWaitTimeSW[2] < 0)
		{
			m_lWaitTimeSW[0] = GetCurrentTime();
			break;
		}

		if (m_lWaitTimeSW[2] > 10)
		{
			if (g_ioMgr.get_in_bit(stIO.i_Chk_ManualLoadingSwitch) == IO_OFF)
			{
				m_nStep_ManualLoading = 400;
			}
			else
			{
				m_nStep_ManualLoading = 0;
			}
		}
		break;

	case 400:
		if (g_ioMgr.get_in_bit(stIO.i_Chk_ManualLoadingSwitch) == IO_OFF)
		{
			m_nStep_ManualLoading = 600;
		}
		break;

	case 600:
		if (st_handler.cwnd_list != NULL)
		{
			sprintf(st_msg.c_normal_msg, "[Manual Loading] ON");
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
		}

		// [Manual Loading] 상태 변경
		Func.OnSet_EqpLoadingManual();
		m_nStep_ManualLoading = 0;
		break;
	}
}

//==================================================================//
// Loading Start 스위치 눌림 처리
//==================================================================//
void CSeq_LampSwitch::OnPush_LoadingStart() 
{
	// 강제 리턴 조건 검사
	if (st_handler.nSysLock != FALSE)
	{
		return;
	}
	int nRet;
	switch(m_nStep_LoadingStart)
	{
	case 0:
		if (g_ioMgr.get_in_bit(stIO.i_Chk_LoadingStartSwitch) == IO_ON)
		{
			OnRest_SwitchStep(5);	// 버튼 눌림 처리 스텝 정보 초기화

			m_lWaitTimeSW[0] = GetCurrentTime();
			m_nStep_LoadingStart = 200;
		}
		break;

	case 200:
		m_lWaitTimeSW[1] = GetCurrentTime();
		m_lWaitTimeSW[2] = m_lWaitTimeSW[1] - m_lWaitTimeSW[0];
		if (m_lWaitTimeSW[2] < 0)
		{
			m_lWaitTimeSW[0] = GetCurrentTime();
			break;
		}

		// [50ms] 동안 버튼이 계속 눌린 경우에만 받아들임
		// - 시그널이 튀는 경우에 대해서는 버튼 눌림으로 처리하지 않음
		if (m_lWaitTimeSW[2] > 50 &&
			g_ioMgr.get_in_bit(stIO.i_Chk_LoadingStartSwitch) == IO_OFF)
		
		{
			m_lWaitTimeSW[0] = GetCurrentTime();
			m_nStep_LoadingStart = 300;
		}
		else if (g_ioMgr.get_in_bit(stIO.i_Chk_LoadingStartSwitch) == IO_OFF)
		{
			m_nStep_LoadingStart = 0;
		}
		break;

	case 300:
		m_lWaitTimeSW[1] = GetCurrentTime();
		m_lWaitTimeSW[2] = m_lWaitTimeSW[1] - m_lWaitTimeSW[0];
		if (m_lWaitTimeSW[2] < 0)
		{
			m_lWaitTimeSW[0] = GetCurrentTime();
			break;
		}

		if (m_lWaitTimeSW[2] > 10)
		{
			if (g_ioMgr.get_in_bit(stIO.i_Chk_LoadingStartSwitch) == IO_OFF)
			{
				m_nStep_LoadingStart = 400;
			}
			else
			{
				m_nStep_LoadingStart = 0;
			}
		}
		break;

	case 400:
		if (g_ioMgr.get_in_bit(stIO.i_Chk_LoadingStartSwitch) == IO_OFF)
		{
			m_nStep_LoadingStart = 600;
		}
		break;

	case 600:
		if (st_handler.cwnd_list != NULL)
		{
			sprintf(st_msg.c_normal_msg, "[Loading Start] ON.");
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
		}

		// [Loading] 상태 변경
		Func.OnSet_EqpLoadingStart();
		m_nStep_LoadingStart = 0;
		break;
	}
}

//==================================================================//
// 스위치 버튼 동작 조건 검사
//==================================================================//
int CSeq_LampSwitch::OnCheckSwitchRunCondition() 
{
	// yangkyu [2015.02.28]
	// Grip, Vac 사용 모드 추가
	if (st_basic.nMode_VacUsing == VAC_NOT_USING && st_basic.nMode_GripUsing == GRIP_NOT_USING ||
		st_basic.nMode_VacUsing == VAC_USING && st_basic.nMode_GripUsing == GRIP_USING)
	{
		CDialog_Message dlgMSG;
		
		st_msg.str_fallacy_msg = _T("You have to Using Any Of Grip and Vac.");
		dlgMSG.DoModal();
		return FALSE;
	}
	// 초기화 작업이 완료된 경우에만 [START] 버튼 누를 수 있도록 함
	// - 초기화 작업 없이 시컨스 시작된 경우 문제가 발생할 수 있음
	if (st_handler.nIsAllSiteInitEnd != CTL_YES)
	{
		//Func.OnSet_BuzzerSound(IO_ON, 0);	// 부저 사운드 ON

		if (st_handler.cwnd_list != NULL)
		{
			sprintf(st_msg.c_abnormal_msg, "[MOTOR INIT CHECK] Motor Initial..., become Run.");
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
		}

		// 알람을 출력할 필요 없는 경우에는 주석 처리
		// 900001 0 16 "EQUIPMENT_INITIALIZE_ERR."
		alarm.mstr_code		= "900000";
		alarm.mn_count_mode	= 0;
		alarm.mn_type_mode	= eWARNING;
		st_work.nEqpStatus	= dWARNING;

		return FALSE;
	}

	// [알람 리셋] 상태인지 확인
	// - 알람 발생 시 [알람 리셋]->[START] 순으로 동작시키기 위함
	if (alarm.nReset_AlarmStatus != CTL_NO)
	{
		if (st_handler.cwnd_list != NULL)
		{
			sprintf(st_msg.c_abnormal_msg, "[JAM CONFIRM] If do to do Run, do first Jam Reset.");
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
		}
		return FALSE;
	}

	// 매뉴얼 모드에서는 버튼 눌림 스킵
	if (st_handler.nModeManual == YES)
	{
		if (st_handler.cwnd_list != NULL)
		{
			sprintf(st_msg.c_abnormal_msg, "[JAM CONFIRM] If do to do Run, do first turn auto mode.");
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
		}
		return FALSE;
	}

	// 도어 열림 상태 조사 [0:모두 닫힘, 0이상:열린 도어 존재]
	if (Func.OnIsDoorOpen() != 0)
	{
		Func.OnSet_BuzzerSound(IO_ON);

		if (st_handler.cwnd_list != NULL)
		{
			sprintf(st_msg.c_abnormal_msg, "[DOOR CHECK] Door Open being done Run become.");
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
		}
		return FALSE;
	}

	// 알람 코드 에디터 호출 되어진 경우이면 알람 정보 다시 로딩
	// - 알람 코드를 수정했을 확률이 높기 때문
	if (st_handler.b_CalledAlarmEditor == TRUE)
	{
		st_handler.b_CalledAlarmEditor = FALSE;

		MyJamData.OnLoad_AlarmInfoFronFile();	// [파일->구조체] 모든 알람 정보 로딩
	}

	return TRUE;
}

//==================================================================//
// 버튼 눌림 처리 스텝 정보 초기화
// - 버튼은 한번에 하나만 눌러야 한다는 가정에서 추가함
//==================================================================//
void CSeq_LampSwitch::OnRest_SwitchStep(int nzStatus) 
{
	switch(nzStatus)
	{
	case 0:	// START 버튼 눌림
		m_nStep_Stop	= 0;
		m_nStep_Reset	= 0;
		m_nStep_Buzz	= 0;
		break;
		
	case 1:	// STOP 버튼 눌림
		m_nStep_Start	= 0;
		m_nStep_Reset	= 0;
		m_nStep_Buzz	= 0;
		break;

	case 2:	// 알람 리셋 버튼 눌림
		m_nStep_Start	= 0;
		m_nStep_Stop	= 0;
		m_nStep_Buzz	= 0;
		break;

	case 3:	// 부저 사운드 OFF 버튼 눌림
		m_nStep_Start	= 0;
		m_nStep_Stop	= 0;
		m_nStep_Reset	= 0;
		m_nStep_Buzz	= 0;
		break;
	case 4:	// Manual Loading 버튼 눌림
		m_nStep_LoadingStart = 0;
		break;
	case 5:	// Loading Start 버튼 눌림
		m_nStep_ManualLoading = 0;
		break;
	}
}

//==================================================================//
// [EQP<->MAIN] 설비 상태 보고
//==================================================================//
int CSeq_LampSwitch::OnReport_2MainEqpStatus(int nzStatus)
{
	int nFuncRet = RET_PROCEED;	// [초기화] 함수 리턴 플래그 (동작중)
	CString sSnd;
	switch(m_nStep_EqpStatus)
	{
	case 0:
		g_client_accy.OnCreate_SendFormat(MAIN_EQP_STATE);
		g_client_accy.OnReq_CommStart();
		m_nStep_EqpStatus = 0;
		nFuncRet = RET_GOOD;
		break;
	}
	return nFuncRet;
}

//==================================================================//
// 설비 상태 보고 [ALARM]
// - 알람 코드 전송 후 설비 상태 보고를 위한 함수
// : 설비 상태 보고 시 응답 확인이 필요하게 될 경우를 대비해 이런 식으로 처리함
//==================================================================//
void CSeq_LampSwitch::OnReport_2MainStatusAlarm() 
{
	int nRet;

	switch(m_nStep_StatusAlarm)
	{
	case 0:
		// 설비 알람 보고 후 설비 상태 (알람) 보고가 발생했는지 확인
		if (st_work.nReq_AlarmProc == CTL_YES)
		{
			// 다른 상태보고 함수에서 이미 OnReport_2CimEqpStatus() 함수 호출했다면 처리중에 있을 것임
			// - 이런 경우는 이전 작업이 완료되기를 기다림
			if (m_nStep_EqpStatus != 0)
			{
				break;
			}
			//m_nStep_EqpStatus = 0;

			// [해제] 설비 상태 (알람) 요청 해제함
			st_work.nReq_AlarmProc = CTL_NO;
			m_nStep_StatusAlarm = 100;
		}
		break;

	case 100:
		nRet = OnReport_2MainEqpStatus(dJAM);	// [EQP<->HOST] 설비 상태 보고
		if (nRet == RET_GOOD)
		{
			m_nStep_StatusAlarm = 0;
		}
		else if (nRet == RET_ERROR)
		{
			m_nStep_StatusAlarm = 0;
		}
		break;
	}
}

//==================================================================//
// [Auto] 상태로 스위치 돌린 경우 처리
//==================================================================//
void CSeq_LampSwitch::OnTurn_Auto() 
{
	int nReadSEN = g_ioMgr.get_in_bit(stIO.i_Chk_AutoSwitch);

	switch(m_nStep_Auto)
	{
	case 0:
		if (nReadSEN == IO_ON && st_handler.nModeManual == CTL_YES)
		{
			m_lWait_Auto[0] = GetCurrentTime();
			m_nStep_Auto = 100;
		}
		break;

	case 100:
		m_lWait_Auto[1] = GetCurrentTime();
		m_lWait_Auto[2] = m_lWait_Auto[1] - m_lWait_Auto[0];
		if (m_lWait_Auto[2] < 0)
		{
			m_lWait_Auto[0] = GetCurrentTime();
			break;
		}

		// [50ms] 동안 유지되는 경우에만 처리 [시그널 튀는 경우 방지]
		if (m_lWait_Auto[2] > 50 && nReadSEN == IO_ON)
		{
			m_nStep_Auto = 600;
		}
		else if (nReadSEN == IO_OFF)
		{
			m_nStep_Auto = 0;
		}
		break;

	case 600:
		Func.OnSet_EqpAuto();	// [오토 모드] 설정 및 스위치 램프 변경
		// 와치독 대화상자 화면 출력 요청
		//::PostMessage(st_handler.hWnd, WM_FORM_CHANGE, 40, DLG_SHOW_);
		m_nStep_Auto = 0;
		break;
	}
}

//==================================================================//
// [Manual] 상태로 스위치 돌린 경우 처리
//==================================================================//
void CSeq_LampSwitch::OnTurn_Manual() 
{
	int nReadSEN = g_ioMgr.get_in_bit(stIO.i_Chk_ManualSwitch);

	switch(m_nStep_Manual)
	{
	case 0:
		if (nReadSEN == IO_ON && st_handler.nModeManual == CTL_NO)
		{
			m_lWait_Manual[0] = GetCurrentTime();
			m_nStep_Manual = 100;
		}
		break;

	case 100:
		m_lWait_Manual[1] = GetCurrentTime();
		m_lWait_Manual[2] = m_lWait_Manual[1] - m_lWait_Manual[0];
		if (m_lWait_Manual[2] < 0)
		{
			m_lWait_Manual[0] = GetCurrentTime();
			break;
		}
		
		// [50ms] 동안 유지되는 경우에만 처리 [시그널 튀는 경우 방지]
		if (m_lWait_Manual[2] > 50 && nReadSEN == IO_ON)
		{
			m_nStep_Manual = 600;
		}
		else if (nReadSEN == IO_OFF)
		{
			m_nStep_Manual = 0;
		}
		break;

	case 600:
		Func.OnSet_EqpManual();	// [매뉴얼 모드] 설정 및 스위치 램프 변경
		Func.OnSet_EqpStatusStop();
		Func.OnOpenDoor();	// 도어 열기
		m_nStep_Manual = 0;
		break;
	}
}

//==================================================================//
// 모든 DOOR 닫힌 경우 강제로 DOOR LOCK 처리
//==================================================================//
void CSeq_LampSwitch::OnRun_DoorLock() 
{
	int nRet = -1;
	int nSensor = -1;

	switch(m_nLockStep)
	{
	case 0:
		// (IDW_SCREEN_IO) 화면이 출력중인 경우에는 강제 리턴
		// - 강제로 도어를 열어놓았는데 다시 닫으면 안되기 때문임
		if (st_handler.nSelectedMenuNum == 701)
		{
			break;
		}
		// 설비 상태가 매뉴얼 모드가 아니면 [오토 모드인 경우]
		// - 매뉴얼 모드에서만 도어 닫힘 상태 검사
		// - 오토 모드에서는 도어가 열리면 이미 모터 전언이 떨어져서 알람이 발생한 상황일것임
		if (st_handler.nModeManual != YES)
		{
			break;
		}

		nRet = Func.OnWatchdogDoor();	// 도어 열림 상태 조사 [0:모두 닫힘, 0이상:열린 도어 존재]
		if (nRet > 0)
		{
			m_nDoorOpenTry = TRUE;	// 도어 열기 동작을 진해한 상태임
		}
		else
		{
			// 매뉴얼 모드로 전한되어 도어 잠금 상태를 해제했는데 도어를 열지 않고 있음
			// - 일정 시간 경과후에 도어를 강제로 잠금 상태로 변경함
			if (m_nDoorOpenTry != TRUE)
			{
				// DOOR 열린적이 없음
				// 하지만 도어락 스위치는 해제된 경우 [도어락 설정]
				nSensor = g_ioMgr.get_out_bit(stIO.o_DoorLock, IO_OFF);
				// 도어 잠금 해제 상태 [Release 상태]
				if (nSensor == IO_OFF)
				{
					m_lLockWaitTime[0] = GetCurrentTime();
					m_nLockStep = 1000;
				}
				else
				{
					break;
				}
			}
			// 도어가 [열림->닫힘] 상태로 변경된 경우
			else
			{
				m_nLockStep = 100;
			}
		}
		break;

	case 100:
		// 도어 잠금 상태 확인
		// - 도어 잠금이 해제되어 있으면 도어 잠금 신호 출력 [모든 도어 잠금]
		nSensor = g_ioMgr.get_out_bit(stIO.o_DoorLock, IO_OFF);
		// 도어 잠금 해제함
		if (nSensor == IO_OFF)
		{
			// 모든 도어가 닫혔으므로 도어 잠금을 설정함
			g_ioMgr.set_out_bit(stIO.o_DoorLock, IO_ON);

			// 도어 잠금 신호 출력 후 곧바로 도어가 잠기는 것이 아니기 때문에 일정 시간 경과후에 동작 완료시킴
			m_lLockWaitTime[0] = GetCurrentTime();
			m_nLockStep = 200;
		}
		// 도어 잠금이 설정된 경우 [도어 열림부터 다시 검사 재시작]
		else
		{
			m_nDoorOpenTry = FALSE;	// [리셋] 도어 여는 동작 진행 여부
			m_nLockStep = 0;
		}
		break;

	case 200:
		// 도어가 완전히 잠길때까지 일정 시간 기다림 [0.5초 대기]
		m_lLockWaitTime[1] = GetCurrentTime();
		m_lLockWaitTime[2] = m_lLockWaitTime[1] - m_lLockWaitTime[0];
		if (m_lLockWaitTime[2] < 0)
		{
			m_lLockWaitTime[0] = GetCurrentTime();
			break;
		}

		if (m_lLockWaitTime[2] > 500)
		{
			if (st_handler.cwnd_list != NULL)
			{
				sprintf(st_msg.c_normal_msg, "Door [Locking]");
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
			}

			m_nDoorOpenTry = FALSE;	// [리셋] 도어 여는 동작 진행 여부
			m_nLockStep = 0;
		}
		break;

		// 도어 잠금 해제 후 [30초] 이내에 도어를 여는 동작이 수반되지 않으면 도어를 열 의향이 없다고 간주
		// 도어 잠금 해제를 취소하고 도어 잠금 상태로 변경
	case 1000:
		nRet = Func.OnWatchdogDoor();	// 도어 열림 상태 조사 [0:모두 닫힘, 0이상:열린 도어 존재]
		if (nRet > 0)
		{
			// 도어가 하나 이상 열렸으므로 정상적으로 모든 도어 닫을 때까지 기다림
			m_nDoorOpenTry = FALSE;	// [리셋] 도어 여는 동작 진행 여부
			m_nLockStep = 0;
		}
		else
		{
			nSensor = g_ioMgr.get_out_bit(stIO.o_DoorLock, IO_OFF);
			// 도어 잠금 해제 상태 [Release 상태]
			if (nSensor == IO_OFF)
			{
				m_lLockWaitTime[1] = GetCurrentTime();
				m_lLockWaitTime[2] = m_lLockWaitTime[1] - m_lLockWaitTime[0];
				if (m_lLockWaitTime[2] < 0)
				{
					m_lLockWaitTime[0] = GetCurrentTime();
					break;
				}
				
				if (m_lLockWaitTime[2] > (1000*30))
				{
					m_nLockStep = 100;
				}
			}
			else
			{
				m_nDoorOpenTry = FALSE;	// [리셋] 도어 여는 동작 진행 여부
				m_nLockStep = 0;
			}
		}
		break;
	}
}
