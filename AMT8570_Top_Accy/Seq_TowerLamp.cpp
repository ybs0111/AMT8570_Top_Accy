// Seq_TowerLamp.cpp: implementation of the CSeq_TowerLamp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Seq_TowerLamp.h"

#include "FastechPublic_IO.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CSeq_TowerLamp	seqTowerLamp;

CSeq_TowerLamp::CSeq_TowerLamp()
{

}

CSeq_TowerLamp::~CSeq_TowerLamp()
{

}

//==================================================================//
// ������ ����
//==================================================================//
void CSeq_TowerLamp::OnSeq_RUN(void) 
{
	OnSeq_INIT();     // ������ ���� ���� �ʱ�ȭ
	OnSeq_Execute();  // ������ ����
}

//==================================================================//
// ������ ���� ���� �ʱ�ȭ
//==================================================================//
void CSeq_TowerLamp::OnSeq_INIT(void) 
{
	if (m_bFlagINIT == true)	{
		return;
	}
	m_bFlagINIT = true;

	m_nStep_TLamp	= 0;	// [�ʱ�ȭ] ������ ���� ����
	m_nLoopType		= 1;	// ���� Ÿ�� ���� [Ȧ����°]

	// ȭ�� ������ �̷������ �Ϸ��� �� ���� 100���� �����ؾ� ��
	m_nStep_Form = 0;
}

//==================================================================//
// Ÿ�� ���� ������ ����
//==================================================================//
void CSeq_TowerLamp::OnSeq_Execute(void) 
{
	// I/O ����� ȭ�� ��µ� ��� ����
	// : �������� ������ I/O ȭ�鿡�� Ÿ�� ���� ��Ʈ���� �Ұ���
	// : �� �����忡�� ����ؼ� Ÿ�� ���� ��� �������� ����
	if (st_handler.nSelectedMenuNum == 701)	{
		return;
	}
	// ����ġ ���� ����� OnSet_BtnLampStop() �Լ��� ���� ����� ����
	// - �̰����� �����ϸ� ��� ��� ��Ʈ�� ȣ���ؼ� ��ġ �̵���Ŵ

	m_nLampType = st_work.nEqpStatus;	// ���� ���� ���� ���� ����
	m_nWaitTime = stLAMP.nTwinkle;		// Ÿ������ ���� �ð� ���� ����

	OnRun_Lamp();  // Ÿ�� ���� ���� ����
}

//==================================================================//
// Ÿ�� ���� ���� ����
//==================================================================//
void CSeq_TowerLamp::OnRun_Lamp() 
{
	switch(m_nStep_TLamp)
	{
	case 0:
		if (m_nLampType == dINIT)
		{
			if (m_nLoopType == 1)	// ����Ÿ�� [Ȧ����°]
			{
				g_ioMgr.set_out_bit(stIO.o_LampStart,		IO_ON);
				g_ioMgr.set_out_bit(stIO.o_LampStop,		IO_OFF);
				g_ioMgr.set_out_bit(stIO.o_LampJamClear,	IO_OFF);
			}
			else	// ����Ÿ�� [¦����°]
			{
				g_ioMgr.set_out_bit(stIO.o_LampStart,		IO_OFF);
				g_ioMgr.set_out_bit(stIO.o_LampStop,		IO_OFF);
				g_ioMgr.set_out_bit(stIO.o_LampJamClear,	IO_ON);
			}
		}
		if (st_handler.nRecoveryReq == CTL_YES)  // ���� ����
		{
			g_ioMgr.set_out_bit(stIO.o_LampStart,	IO_ON);
		}

		// ���� ���� ��� ����
		if (stLAMP.nLampR[m_nLampType] == LAMP_ON)
		{
			g_ioMgr.set_out_bit(stIO.o_TLampRed,	IO_ON);
		}
		else if (stLAMP.nLampR[m_nLampType] == LAMP_OFF)
		{
			g_ioMgr.set_out_bit(stIO.o_TLampRed,	IO_OFF);
		}
		else if (stLAMP.nLampR[m_nLampType] == LAMP_FLICK)
		{
			g_ioMgr.set_out_bit(stIO.o_TLampRed,	IO_ON);
		}

		// Ȳ�� ���� ��� ����
		if (stLAMP.nLampY[m_nLampType] == LAMP_ON)
		{
			g_ioMgr.set_out_bit(stIO.o_TLampYellow,	IO_ON);
		}
		else if (stLAMP.nLampY[m_nLampType] == LAMP_OFF)
		{
			g_ioMgr.set_out_bit(stIO.o_TLampYellow,	IO_OFF);
		}
		else if (stLAMP.nLampY[m_nLampType] == LAMP_FLICK)
		{
			g_ioMgr.set_out_bit(stIO.o_TLampYellow,	IO_ON);
		}

		// �ʷ� ���� ��� ����
		if (stLAMP.nLampG[m_nLampType] == LAMP_ON)
		{
			g_ioMgr.set_out_bit(stIO.o_TLampGreen,	IO_ON);
		}
		else if (stLAMP.nLampG[m_nLampType] == LAMP_OFF)
		{
			g_ioMgr.set_out_bit(stIO.o_TLampGreen,	IO_OFF);
		}
		else if (stLAMP.nLampG[m_nLampType] == LAMP_FLICK)
		{
			g_ioMgr.set_out_bit(stIO.o_TLampGreen,	IO_ON);
		}

		// ON/OFF ���� ���� ���Ͽ� ���� �ð� ����
		// : ȭ�� ���� �ð���ŭ ON/OFF �ð� ����
		// : [�迭 2] - [�迭 1] ���� [-]�̸� �������� �ٽ� ����
		m_lTime_TLamp[0] = GetCurrentTime();
		m_nStep_TLamp = 10;
		break;

	case 10:
		m_lTime_TLamp[1] = GetCurrentTime();
		m_lTime_TLamp[2] = m_lTime_TLamp[1] - m_lTime_TLamp[0];
		if (m_lTime_TLamp[2] > m_nWaitTime)
		{
			m_nStep_TLamp = 20;
		}
		else if (m_lTime_TLamp[2] < 0)
		{
			m_lTime_TLamp[0] = GetCurrentTime();
		}
		break;

	case 20:
		if (m_nLampType == dINIT)
		{
			if (m_nLoopType == 1)	// ����Ÿ�� [Ȧ����°]
			{
				g_ioMgr.set_out_bit(stIO.o_LampStart,		IO_OFF);
				g_ioMgr.set_out_bit(stIO.o_LampStop,		IO_ON);
				g_ioMgr.set_out_bit(stIO.o_LampJamClear,	IO_OFF);
			}
			else	// ����Ÿ�� [¦����°]
			{
				g_ioMgr.set_out_bit(stIO.o_LampStart,		IO_OFF);
				g_ioMgr.set_out_bit(stIO.o_LampStop,		IO_ON);
				g_ioMgr.set_out_bit(stIO.o_LampJamClear,	IO_OFF);
			}
		}
		if (st_handler.nRecoveryReq == CTL_YES)
		{
			g_ioMgr.set_out_bit(stIO.o_LampStart,	IO_OFF);
		}

		if (stLAMP.nLampR[m_nLampType] == LAMP_ON)
		{
			g_ioMgr.set_out_bit(stIO.o_TLampRed,	IO_ON);
		}
		else if (stLAMP.nLampR[m_nLampType] == LAMP_OFF)
		{
			g_ioMgr.set_out_bit(stIO.o_TLampRed,	IO_OFF);
		}
		else if (stLAMP.nLampR[m_nLampType] == LAMP_FLICK)
		{
			g_ioMgr.set_out_bit(stIO.o_TLampRed,	IO_OFF);
		}

		if (stLAMP.nLampY[m_nLampType] == LAMP_ON)
		{
			g_ioMgr.set_out_bit(stIO.o_TLampYellow,	IO_ON);
		}
		else if (stLAMP.nLampY[m_nLampType] == LAMP_OFF)
		{
			g_ioMgr.set_out_bit(stIO.o_TLampYellow,	IO_OFF);
		}
		else if (stLAMP.nLampY[m_nLampType] == LAMP_FLICK)
		{
			g_ioMgr.set_out_bit(stIO.o_TLampYellow,	IO_OFF);
		}

		if (stLAMP.nLampG[m_nLampType] == LAMP_ON)
		{
			g_ioMgr.set_out_bit(stIO.o_TLampGreen,	IO_ON);
		}
		else if (stLAMP.nLampG[m_nLampType] == LAMP_OFF)
		{
			g_ioMgr.set_out_bit(stIO.o_TLampGreen,	IO_OFF);
		}
		else if (stLAMP.nLampG[m_nLampType] == LAMP_FLICK)
		{
			g_ioMgr.set_out_bit(stIO.o_TLampGreen,	IO_OFF);
		}

		m_lTime_TLamp[0] = GetCurrentTime();
		m_nStep_TLamp = 30;
		break;

	case 30:
		m_lTime_TLamp[1] = GetCurrentTime();
		m_lTime_TLamp[2] = m_lTime_TLamp[1] - m_lTime_TLamp[0];
		if (m_lTime_TLamp[2] > m_nWaitTime)
		{
			m_nStep_TLamp = 0;

			if (m_nLampType == dINIT)
			{
				if (m_nLoopType == 1)
				{
					m_nLoopType = 0;
				}
				else if (m_nLoopType == 0)
				{
					m_nLoopType = 1;
				}
			}
		}
		else if (m_lTime_TLamp[2] < 0)
		{
			m_lTime_TLamp[0] = GetCurrentTime();
		}
		break;
	}
}

void CSeq_TowerLamp::OnRun_FormChange() 
{
	switch(m_nStep_Form)
	{
	case 100:
		::PostMessage(st_handler.hWnd, WM_FORM_CHANGE, 9, 1);	// IO

		m_lTime_Form[0] = GetCurrentTime();
		m_nStep_Form = 200;
		break;

	case 200:
		m_lTime_Form[1] = GetCurrentTime();
		m_lTime_Form[2] = m_lTime_Form[1] - m_lTime_Form[0];
		if (m_lTime_Form[2] > 100)
		{
			m_nStep_Form = 300;
		}
		else if (m_lTime_Form[2] < 0)
		{
			m_lTime_Form[0] = GetCurrentTime();
		}
		break;

	case 300:
		::PostMessage(st_handler.hWnd, WM_FORM_CHANGE, 5, 1);	// XYZ

		m_lTime_Form[0] = GetCurrentTime();
		m_nStep_Form = 400;
		break;

	case 400:
		m_lTime_Form[1] = GetCurrentTime();
		m_lTime_Form[2] = m_lTime_Form[1] - m_lTime_Form[0];
		if (m_lTime_Form[2] > 100)
		{
			m_nStep_Form = 100;
		}
		else if (m_lTime_Form[2] < 0)
		{
			m_lTime_Form[0] = GetCurrentTime();
		}
		break;
	}
}
