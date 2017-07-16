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
// 쓰레드 동작
//==================================================================//
void CSeq_TowerLamp::OnSeq_RUN(void) 
{
	OnSeq_INIT();     // 쓰레드 관련 변수 초기화
	OnSeq_Execute();  // 쓰레드 제어
}

//==================================================================//
// 쓰레드 관련 변수 초기화
//==================================================================//
void CSeq_TowerLamp::OnSeq_INIT(void) 
{
	if (m_bFlagINIT == true)	{
		return;
	}
	m_bFlagINIT = true;

	m_nStep_TLamp	= 0;	// [초기화] 시컨스 스텝 정보
	m_nLoopType		= 1;	// 루프 타입 정보 [홀수번째]

	// 화면 변경이 이루어지게 하려면 이 값을 100으로 설정해야 함
	m_nStep_Form = 0;
}

//==================================================================//
// 타워 랩프 시컨스 제어
//==================================================================//
void CSeq_TowerLamp::OnSeq_Execute(void) 
{
	// I/O 디버깅 화면 출력된 경우 리턴
	// : 리턴하지 않으면 I/O 화면에서 타워 팸프 컨트롤이 불가능
	// : 이 쓰레드에서 계속해서 타워 램프 출력 내보내기 때문
	if (st_handler.nSelectedMenuNum == 701)	{
		return;
	}
	// 스위치 램프 제어는 OnSet_BtnLampStop() 함수로 따로 만들어 놓음
	// - 이곳에서 제어하면 계속 출력 포트를 호출해서 위치 이동시킴

	m_nLampType = st_work.nEqpStatus;	// 현재 설비 상태 정보 설정
	m_nWaitTime = stLAMP.nTwinkle;		// 타워램프 제어 시간 간격 설정

	OnRun_Lamp();  // 타워 램프 상태 제어
}

//==================================================================//
// 타워 램프 상태 제어
//==================================================================//
void CSeq_TowerLamp::OnRun_Lamp() 
{
	switch(m_nStep_TLamp)
	{
	case 0:
		if (m_nLampType == dINIT)
		{
			if (m_nLoopType == 1)	// 루프타입 [홀수번째]
			{
				g_ioMgr.set_out_bit(stIO.o_LampStart,		IO_ON);
				g_ioMgr.set_out_bit(stIO.o_LampStop,		IO_OFF);
				g_ioMgr.set_out_bit(stIO.o_LampJamClear,	IO_OFF);
			}
			else	// 루프타입 [짝수번째]
			{
				g_ioMgr.set_out_bit(stIO.o_LampStart,		IO_OFF);
				g_ioMgr.set_out_bit(stIO.o_LampStop,		IO_OFF);
				g_ioMgr.set_out_bit(stIO.o_LampJamClear,	IO_ON);
			}
		}
		if (st_handler.nRecoveryReq == CTL_YES)  // 복구 동작
		{
			g_ioMgr.set_out_bit(stIO.o_LampStart,	IO_ON);
		}

		// 적색 램프 출력 제어
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

		// 황색 램프 출력 제어
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

		// 초록 램프 출력 제어
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

		// ON/OFF 간격 제어 위하여 현재 시간 설정
		// : 화면 설정 시간만큼 ON/OFF 시간 유지
		// : [배열 2] - [배열 1] 값이 [-]이면 설정부터 다시 시작
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
			if (m_nLoopType == 1)	// 루프타입 [홀수번째]
			{
				g_ioMgr.set_out_bit(stIO.o_LampStart,		IO_OFF);
				g_ioMgr.set_out_bit(stIO.o_LampStop,		IO_ON);
				g_ioMgr.set_out_bit(stIO.o_LampJamClear,	IO_OFF);
			}
			else	// 루프타입 [짝수번째]
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
