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
// ������ ����
//==================================================================//
void CSeq_LampSwitch::OnSeq_RUN(void) 
{
	OnSeq_Init();     // ������ ���� ���� �ʱ�ȭ
	OnSeq_Execute();  // ������ ����
}

//==================================================================//
// ������ ���� ���� �ʱ�ȭ
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

	// ���̸� �߰� [2014.12.16]
	m_nLockStep = 0;
	m_nDoorOpenTry = FALSE;
}

//==================================================================//
// Ÿ�� ���� ������ ����
//==================================================================//
void CSeq_LampSwitch::OnSeq_Execute(void) 
{
	OnPush_Start();	// START ����ġ ���� ó��
	OnPush_Stop();	// STOP ����ġ ���� ó��
	OnPush_AlarmClear();// �˶� ���� ����ġ ���� ó��
	OnPush_BuzzerOff();	// BUZZER OFF ����ġ ���� ó��
	OnPush_LoadingStart();	// ManualLoading ����ġ ���� ó��
	OnPush_ManualLoading();	// LoadingStart ����ġ ���� ó��
	OnReport_2MainStatusAlarm();	// ���� ���� ���� [ALARM]
	OnTurn_Auto();		// [Auto] ���·� ����ġ ���� ��� ó��
	OnTurn_Manual();	// [Manual] ���·� ����ġ ���� ��� ó��

	// ���̸� �߰� [2014.12.17]
	OnRun_DoorLock();	// ��� DOOR ���� ��� ������ DOOR LOCK ó��
}

//==================================================================//
// START ����ġ ���� ó��
//==================================================================//
void CSeq_LampSwitch::OnPush_Start() 
{
	// ���� ���� ���� �˻�
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
		// [STOP] ���¿����� [START] �����ϵ��� �������� �߰�
		if (st_work.nEqpStatus == dSTOP)
		{
			// ���� ȭ�鿡�� [START] ��ư�� ���ȴ��� Ȯ��
			if ((st_work.nReq_StrtProc) == REQ_YES)
			{
				// ���� ȭ�鿡�� [START ��ư] ���õ� ��� ó��
				// : �� ��쿡�� I/O �Է��� �߻����� �ʱ� ������ �÷��׷� ó��
				st_work.nReq_StrtProc = REQ_NONE;

				m_nStep_Start = 600;
				break;
			}

			if (g_ioMgr.get_in_bit(stIO.i_Chk_StartSwitch) == IO_ON)
			{
				// ���� STOP ��ư�� �Բ� ���� ��쿡�� ����
				if (g_ioMgr.get_in_bit(stIO.i_Chk_StopSwitch) == IO_ON)
				{
					break;
				}
				OnRest_SwitchStep(0);	// ��ư ���� ó�� ���� ���� �ʱ�ȭ

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

		// [50ms] ���� ��ư�� ��� ���� ��쿡�� �޾Ƶ���
		// - �ñ׳��� Ƣ�� ��쿡 ���ؼ��� ��ư �������� ó������ ����
		if (m_lWaitTimeSW[2] > 50 &&
			g_ioMgr.get_in_bit(stIO.i_Chk_StartSwitch) == IO_OFF)
		{
			m_lWaitTimeSW[0] = GetCurrentTime();
			m_nStep_Start = 300;
		}
		else if (g_ioMgr.get_in_bit(stIO.i_Chk_StartSwitch) == IO_OFF)
		{
			// ���������� �Է��� Ƣ�� ��쿡�� ����
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
		// ����ġ ��ư ���� ���� �˻�
		if (OnCheckSwitchRunCondition() == TRUE)
		{
			m_nStep_Start = 700;
		}
		else	{
			m_nStep_Start = 0;
		}
		break;

	case 700:
		// �ʱ�ȭ �۾� �Ϸ� �Ŀ� �ڵ����� [RUN] ���·� ����
		if (st_work.nEqpStatus != dSTOP &&
			st_work.nEqpStatus != dINIT)
		{
			break;
		}

		// ���� ������ �켱 ó����
		//nRet = Func.Robot_Reinstatement_Position(1);
		// baeksh [test]
		nRet = CTLBD_RET_GOOD;
		if (nRet != CTLBD_RET_GOOD)
		{
			// �� ������ �ּ� ó���ϴ� ��� ����ؼ� [STOP] ���� �߰���
			if (st_work.nEqpStatus != dSTOP)
			{
				m_nStep_Start = 0;
			}
			break;
		}

		// �˶� ȭ���� ��µ� ���¿��� [START] ��ư ���� ��� : �˶� ȭ�� �켱 �����Ŵ
		if (st_handler.nShowAlarmDlg != FALSE)
		{
			::PostMessage(st_handler.hWnd, WM_FORM_CHANGE, 7, 2);
		}
		// ���� ȭ������ ��ȯ ��û
		if (st_handler.nSelectedMenuNum != 101)
		{
			::PostMessage(st_handler.hWnd, WM_FORM_CHANGE, 1, 1);
		}

		Func.OnSet_EqpStatusRun();	// [RUN] ���� ���� ����
		//m_nStep_Start = 0;
		// ���̸� ���� [2014.07.31]
		m_nStep_Start = 2000;
		break;

		//==========================================================//
		// ���̸� �߰� [2014.07.31]
		//==========================================================//
	case 2000:
		// �ٸ� ���º��� �Լ����� �̹� OnReport_2CimEqpStatus() �Լ� ȣ���ߴٸ� ó���߿� ���� ����
		// - �̷� ���� ���� �۾��� �Ϸ�Ǳ⸦ ��ٸ�
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
// STOP ����ġ ���� ó��
//==================================================================//
void CSeq_LampSwitch::OnPush_Stop() 
{
	// ���� ���� ���� �˻�
	if (st_handler.nSysLock != FALSE)
	{
		return;
	}
	int nRet;

	switch(m_nStep_Stop)
	{
	case 0:
		// [STOP] ���°� �ƴ� ��쿡�� [STOP] ������ �����Ŵ
		if (st_work.nEqpStatus != dSTOP)
		{
			if (g_ioMgr.get_in_bit(stIO.i_Chk_StopSwitch) == IO_ON)
			{
				// ���� START ��ư�� �Բ� ���� ��쿡�� ����
				if (g_ioMgr.get_in_bit(stIO.i_Chk_StartSwitch) == IO_ON)
				{
					break;
				}
				OnRest_SwitchStep(1);	// ��ư ���� ó�� ���� ���� �ʱ�ȭ

				// ���̸� ���� [2014.07.29]
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

		// [50ms] ���� ��ư�� ��� ���� ��쿡�� �޾Ƶ���
		// - �ñ׳��� Ƣ�� ��쿡 ���ؼ��� ��ư �������� ó������ ����
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
			// �ʱ�ȭ �۾� �� STOP ��ư ������ �ʱ�ȭ ��ҵ�
			// - �ʱ�ȭ �۾� �� ���� �߻� ���� ���� [�����߻�]
			// - �ʱ�ȭ ȭ���� ���α׷����� ���� ó����
			if (st_work.nEqpStatus == dINIT)
			{
				// �ʱ�ȭ �۾����� ���� ��ȯ�ϵ��� ����
				st_handler.nStopPushInInitial = TRUE;
				break;
			}

			// ���� ȭ������ ��ȯ ��û
			if (st_handler.nSelectedMenuNum != 101)
			{
				::PostMessage(st_handler.hWnd, WM_FORM_CHANGE, 1, 1);
			}

			Func.OnSet_EqpStatusStop();	// [STOP] ���� ���� ���� 
			//m_nStep_Stop = 0;
			// ���̸� ���� [2014.07.31]
			m_nStep_Stop = 2000;
		}
		break;

		//==========================================================//
		// ���̸� �߰� [2014.07.31]
		//==========================================================//
	case 2000:
		// �ٸ� ���º��� �Լ����� �̹� OnReport_2CimEqpStatus() �Լ� ȣ���ߴٸ� ó���߿� ���� ����
		// - �̷� ���� ���� �۾��� �Ϸ�Ǳ⸦ ��ٸ�
		if (m_nStep_EqpStatus != 0)
		{
			break;
		}
		//m_nStep_EqpStatus = 0;

		m_nStep_Stop = 2010;
		break;
		
	case 2010:
		nRet = OnReport_2MainEqpStatus(dSTOP);	// [EQP<->HOST] ���� ���� ����
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
// �˶� ���� ����ġ ���� ó��
//==================================================================//
void CSeq_LampSwitch::OnPush_AlarmClear() 
{
	// ���� ���� ���� �˻�
	if (st_handler.nSysLock != FALSE)
	{
		return;
	}

	switch(m_nStep_Reset)
	{
	case 0:
		if (g_ioMgr.get_in_bit(stIO.i_Chk_JamClearSwitch) == IO_ON)
		{
			OnRest_SwitchStep(2);	// ��ư ���� ó�� ���� ���� �ʱ�ȭ

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

		// [50ms] ���� ��ư�� ��� ���� ��쿡�� �޾Ƶ���
		// - �ñ׳��� Ƣ�� ��쿡 ���ؼ��� ��ư �������� ó������ ����
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
		// �˶� ���� �۾��� �ʿ��� ������� Ȯ��
		// [RUN ����]     : ���常 ����
		// [RUN ���� �ƴ�] : [STOP] ���·� ����
		alarm.nReset_AlarmStatus = CTL_NO;	// �˶� ���� �۾� �ʿ� ����

		if (st_handler.cwnd_list != NULL)
		{
			sprintf(st_msg.c_normal_msg, "[ALARM CLEAR] JAM RESET.");
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
		}

		if (st_work.nEqpStatus != dRUN)
		{
			Func.OnSet_EqpStatusStop();	// [STOP] ���� ���� ���� 
		}
		Func.OnSet_BuzzerSound(IO_OFF, 0);	// ���� ���� ON/OFF
		g_ioMgr.set_out_bit(stIO.o_LampJamClear, IO_OFF);

		m_nStep_Reset = 0;
		break;
	}
}

//==================================================================//
// BUZZER OFF ����ġ ���� ó��
//==================================================================//
void CSeq_LampSwitch::OnPush_BuzzerOff() 
{
	// ���� ���� ���� �˻�
	if (st_handler.nSysLock != FALSE)
	{
		return;
	}

	switch(m_nStep_Buzz)
	{
	case 0:
		if (g_ioMgr.get_in_bit(stIO.i_Chk_BuzzStopSwitch) == IO_ON)
		{
			OnRest_SwitchStep(3);	// ��ư ���� ó�� ���� ���� �ʱ�ȭ

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

		// [50ms] ���� ��ư�� ��� ���� ��쿡�� �޾Ƶ���
		// - �ñ׳��� Ƣ�� ��쿡 ���ؼ��� ��ư �������� ó������ ����
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

		Func.OnSet_BuzzerSound(IO_OFF, 0);	// ���� ���� ON/OFF
		m_nStep_Buzz = 0;
		break;
	}
}

//==================================================================//
// ManualLoading ����ġ ���� ó��
//==================================================================//
void CSeq_LampSwitch::OnPush_ManualLoading() 
{
	// ���� ���� ���� �˻�
	if (st_handler.nSysLock != FALSE)
	{
		return;
	}

	switch(m_nStep_ManualLoading)
	{
	case 0:
		if (g_ioMgr.get_in_bit(stIO.i_Chk_ManualLoadingSwitch) == IO_ON)
		{
			OnRest_SwitchStep(4);	// ��ư ���� ó�� ���� ���� �ʱ�ȭ

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

		// [50ms] ���� ��ư�� ��� ���� ��쿡�� �޾Ƶ���
		// - �ñ׳��� Ƣ�� ��쿡 ���ؼ��� ��ư �������� ó������ ����
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

		// [Manual Loading] ���� ����
		Func.OnSet_EqpLoadingManual();
		m_nStep_ManualLoading = 0;
		break;
	}
}

//==================================================================//
// Loading Start ����ġ ���� ó��
//==================================================================//
void CSeq_LampSwitch::OnPush_LoadingStart() 
{
	// ���� ���� ���� �˻�
	if (st_handler.nSysLock != FALSE)
	{
		return;
	}

	switch(m_nStep_LoadingStart)
	{
	case 0:
		if (g_ioMgr.get_in_bit(stIO.i_Chk_LoadingStartSwitch) == IO_ON)
		{
			OnRest_SwitchStep(5);	// ��ư ���� ó�� ���� ���� �ʱ�ȭ

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

		// [50ms] ���� ��ư�� ��� ���� ��쿡�� �޾Ƶ���
		// - �ñ׳��� Ƣ�� ��쿡 ���ؼ��� ��ư �������� ó������ ����
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

		// [Loading] ���� ����
		Func.OnSet_EqpLoadingStart();
		m_nStep_LoadingStart = 0;
		break;
	}
}

//==================================================================//
// ����ġ ��ư ���� ���� �˻�
//==================================================================//
int CSeq_LampSwitch::OnCheckSwitchRunCondition() 
{
	// yangkyu [2015.02.28]
	// Grip, Vac ��� ��� �߰�
	if (st_basic.nMode_VacUsing == VAC_NOT_USING && st_basic.nMode_GripUsing == GRIP_NOT_USING ||
		st_basic.nMode_VacUsing == VAC_USING && st_basic.nMode_GripUsing == GRIP_USING)
	{
		CDialog_Message dlgMSG;
		
		st_msg.str_fallacy_msg = _T("You have to Using Any Of Grip and Vac.");
		dlgMSG.DoModal();
		return FALSE;
	}
	// �ʱ�ȭ �۾��� �Ϸ�� ��쿡�� [START] ��ư ���� �� �ֵ��� ��
	// - �ʱ�ȭ �۾� ���� ������ ���۵� ��� ������ �߻��� �� ����
	if (st_handler.nIsAllSiteInitEnd != CTL_YES)
	{
		//Func.OnSet_BuzzerSound(IO_ON, 0);	// ���� ���� ON

		if (st_handler.cwnd_list != NULL)
		{
			sprintf(st_msg.c_abnormal_msg, "[MOTOR INIT CHECK] Motor Initial..., become Run.");
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
		}

		// �˶��� ����� �ʿ� ���� ��쿡�� �ּ� ó��
		// 900001 0 16 "EQUIPMENT_INITIALIZE_ERR."
		alarm.mstr_code		= "900000";
		alarm.mn_count_mode	= 0;
		alarm.mn_type_mode	= eWARNING;
		st_work.nEqpStatus	= dWARNING;

		return FALSE;
	}

	// [�˶� ����] �������� Ȯ��
	// - �˶� �߻� �� [�˶� ����]->[START] ������ ���۽�Ű�� ����
	if (alarm.nReset_AlarmStatus != CTL_NO)
	{
		if (st_handler.cwnd_list != NULL)
		{
			sprintf(st_msg.c_abnormal_msg, "[JAM CONFIRM] If do to do Run, do first Jam Reset.");
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
		}
		return FALSE;
	}

	// �Ŵ��� ��忡���� ��ư ���� ��ŵ
	if (st_handler.nModeManual == YES)
	{
		if (st_handler.cwnd_list != NULL)
		{
			sprintf(st_msg.c_abnormal_msg, "[JAM CONFIRM] If do to do Run, do first turn auto mode.");
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
		}
		return FALSE;
	}

	// ���� ���� ���� ���� [0:��� ����, 0�̻�:���� ���� ����]
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

	// �˶� �ڵ� ������ ȣ�� �Ǿ��� ����̸� �˶� ���� �ٽ� �ε�
	// - �˶� �ڵ带 �������� Ȯ���� ���� ����
	if (st_handler.b_CalledAlarmEditor == TRUE)
	{
		st_handler.b_CalledAlarmEditor = FALSE;

		MyJamData.OnLoad_AlarmInfoFronFile();	// [����->����ü] ��� �˶� ���� �ε�
	}

	return TRUE;
}

//==================================================================//
// ��ư ���� ó�� ���� ���� �ʱ�ȭ
// - ��ư�� �ѹ��� �ϳ��� ������ �Ѵٴ� �������� �߰���
//==================================================================//
void CSeq_LampSwitch::OnRest_SwitchStep(int nzStatus) 
{
	switch(nzStatus)
	{
	case 0:	// START ��ư ����
		m_nStep_Stop	= 0;
		m_nStep_Reset	= 0;
		m_nStep_Buzz	= 0;
		break;
		
	case 1:	// STOP ��ư ����
		m_nStep_Start	= 0;
		m_nStep_Reset	= 0;
		m_nStep_Buzz	= 0;
		break;

	case 2:	// �˶� ���� ��ư ����
		m_nStep_Start	= 0;
		m_nStep_Stop	= 0;
		m_nStep_Buzz	= 0;
		break;

	case 3:	// ���� ���� OFF ��ư ����
		m_nStep_Start	= 0;
		m_nStep_Stop	= 0;
		m_nStep_Reset	= 0;
		m_nStep_Buzz	= 0;
		break;
	case 4:	// Manual Loading ��ư ����
		m_nStep_LoadingStart = 0;
		break;
	case 5:	// Loading Start ��ư ����
		m_nStep_ManualLoading = 0;
		break;
	}
}

//==================================================================//
// [EQP<->MAIN] ���� ���� ����
//==================================================================//
int CSeq_LampSwitch::OnReport_2MainEqpStatus(int nzStatus)
{
	int nFuncRet = RET_PROCEED;	// [�ʱ�ȭ] �Լ� ���� �÷��� (������)
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
// ���� ���� ���� [ALARM]
// - �˶� �ڵ� ���� �� ���� ���� ������ ���� �Լ�
// : ���� ���� ���� �� ���� Ȯ���� �ʿ��ϰ� �� ��츦 ����� �̷� ������ ó����
//==================================================================//
void CSeq_LampSwitch::OnReport_2MainStatusAlarm() 
{
	int nRet;

	switch(m_nStep_StatusAlarm)
	{
	case 0:
		// ���� �˶� ���� �� ���� ���� (�˶�) ������ �߻��ߴ��� Ȯ��
		if (st_work.nReq_AlarmProc == CTL_YES)
		{
			// �ٸ� ���º��� �Լ����� �̹� OnReport_2CimEqpStatus() �Լ� ȣ���ߴٸ� ó���߿� ���� ����
			// - �̷� ���� ���� �۾��� �Ϸ�Ǳ⸦ ��ٸ�
			if (m_nStep_EqpStatus != 0)
			{
				break;
			}
			//m_nStep_EqpStatus = 0;

			// [����] ���� ���� (�˶�) ��û ������
			st_work.nReq_AlarmProc = CTL_NO;
			m_nStep_StatusAlarm = 100;
		}
		break;

	case 100:
		nRet = OnReport_2MainEqpStatus(dJAM);	// [EQP<->HOST] ���� ���� ����
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
// [Auto] ���·� ����ġ ���� ��� ó��
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

		// [50ms] ���� �����Ǵ� ��쿡�� ó�� [�ñ׳� Ƣ�� ��� ����]
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
		Func.OnSet_EqpAuto();	// [���� ���] ���� �� ����ġ ���� ����
		// ��ġ�� ��ȭ���� ȭ�� ��� ��û
		//::PostMessage(st_handler.hWnd, WM_FORM_CHANGE, 40, DLG_SHOW_);
		m_nStep_Auto = 0;
		break;
	}
}

//==================================================================//
// [Manual] ���·� ����ġ ���� ��� ó��
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
		
		// [50ms] ���� �����Ǵ� ��쿡�� ó�� [�ñ׳� Ƣ�� ��� ����]
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
		Func.OnSet_EqpManual();	// [�Ŵ��� ���] ���� �� ����ġ ���� ����
		Func.OnSet_EqpStatusStop();
		Func.OnOpenDoor();	// ���� ����
		m_nStep_Manual = 0;
		break;
	}
}

//==================================================================//
// ��� DOOR ���� ��� ������ DOOR LOCK ó��
//==================================================================//
void CSeq_LampSwitch::OnRun_DoorLock() 
{
	int nRet = -1;
	int nSensor = -1;

	switch(m_nLockStep)
	{
	case 0:
		// (IDW_SCREEN_IO) ȭ���� ������� ��쿡�� ���� ����
		// - ������ ��� ������Ҵµ� �ٽ� ������ �ȵǱ� ������
		if (st_handler.nSelectedMenuNum == 701)
		{
			break;
		}
		// ���� ���°� �Ŵ��� ��尡 �ƴϸ� [���� ����� ���]
		// - �Ŵ��� ��忡���� ���� ���� ���� �˻�
		// - ���� ��忡���� ��� ������ �̹� ���� ������ �������� �˶��� �߻��� ��Ȳ�ϰ���
		if (st_handler.nModeManual != YES)
		{
			break;
		}

		nRet = Func.OnWatchdogDoor();	// ���� ���� ���� ���� [0:��� ����, 0�̻�:���� ���� ����]
		if (nRet > 0)
		{
			m_nDoorOpenTry = TRUE;	// ���� ���� ������ ������ ������
		}
		else
		{
			// �Ŵ��� ���� ���ѵǾ� ���� ��� ���¸� �����ߴµ� ��� ���� �ʰ� ����
			// - ���� �ð� ����Ŀ� ��� ������ ��� ���·� ������
			if (m_nDoorOpenTry != TRUE)
			{
				// DOOR �������� ����
				// ������ ����� ����ġ�� ������ ��� [����� ����]
				nSensor = g_ioMgr.get_out_bit(stIO.o_DoorLock, IO_OFF);
				// ���� ��� ���� ���� [Release ����]
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
			// ��� [����->����] ���·� ����� ���
			else
			{
				m_nLockStep = 100;
			}
		}
		break;

	case 100:
		// ���� ��� ���� Ȯ��
		// - ���� ����� �����Ǿ� ������ ���� ��� ��ȣ ��� [��� ���� ���]
		nSensor = g_ioMgr.get_out_bit(stIO.o_DoorLock, IO_OFF);
		// ���� ��� ������
		if (nSensor == IO_OFF)
		{
			// ��� ��� �������Ƿ� ���� ����� ������
			g_ioMgr.set_out_bit(stIO.o_DoorLock, IO_ON);

			// ���� ��� ��ȣ ��� �� ��ٷ� ��� ���� ���� �ƴϱ� ������ ���� �ð� ����Ŀ� ���� �Ϸ��Ŵ
			m_lLockWaitTime[0] = GetCurrentTime();
			m_nLockStep = 200;
		}
		// ���� ����� ������ ��� [���� �������� �ٽ� �˻� �����]
		else
		{
			m_nDoorOpenTry = FALSE;	// [����] ���� ���� ���� ���� ����
			m_nLockStep = 0;
		}
		break;

	case 200:
		// ��� ������ ��涧���� ���� �ð� ��ٸ� [0.5�� ���]
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

			m_nDoorOpenTry = FALSE;	// [����] ���� ���� ���� ���� ����
			m_nLockStep = 0;
		}
		break;

		// ���� ��� ���� �� [30��] �̳��� ��� ���� ������ ���ݵ��� ������ ��� �� ������ ���ٰ� ����
		// ���� ��� ������ ����ϰ� ���� ��� ���·� ����
	case 1000:
		nRet = Func.OnWatchdogDoor();	// ���� ���� ���� ���� [0:��� ����, 0�̻�:���� ���� ����]
		if (nRet > 0)
		{
			// ��� �ϳ� �̻� �������Ƿ� ���������� ��� ���� ���� ������ ��ٸ�
			m_nDoorOpenTry = FALSE;	// [����] ���� ���� ���� ���� ����
			m_nLockStep = 0;
		}
		else
		{
			nSensor = g_ioMgr.get_out_bit(stIO.o_DoorLock, IO_OFF);
			// ���� ��� ���� ���� [Release ����]
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
				m_nDoorOpenTry = FALSE;	// [����] ���� ���� ���� ���� ����
				m_nLockStep = 0;
			}
		}
		break;
	}
}