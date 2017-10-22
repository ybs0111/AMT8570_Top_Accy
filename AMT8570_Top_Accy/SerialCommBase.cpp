// SerialCommBase.cpp: implementation of the CSerialCommBase class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SerialCommBase.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSerialCommBase::CSerialCommBase()
{
	m_nPortNum	= -1;
	m_nStep_Bcr	= 0;

}

CSerialCommBase::~CSerialCommBase()
{

}

//==================================================================//
// [시컨스] 바코드 읽기 작업
//==================================================================//
void CSerialCommBase::OnRun_BarCode() 
{
	CString sTemp;

	switch(m_nStep_Bcr)
	{
	case 0:
		// 메시지 전송 요청 발생
		if (st_serial.nReq_CommSend[PORT_BCR_ACCY_ -1] == REQ_CALL_)
		{
			m_nStep_Bcr = 1000;
		}
		break;

	case 1000:
		// 바코드 읽기 요청
		::PostMessage(st_handler.hWnd, WM_DATA_SEND, PORT_BCR_ACCY_, 0);
		st_serial.n_rec_chk[PORT_BCR_ACCY_ -1] = FALSE;

		m_lWait_Bcr[0] = GetCurrentTime();
		m_nStep_Bcr = 1100;
		break;

	case 1100:
		// 응답 기다림
		if (st_serial.n_rec_chk[PORT_BCR_ACCY_ -1] == YES)
		{
			sTemp = st_serial.comm_rec[PORT_BCR_ACCY_ -1];
			cLOG.OnLogEvent(LOG_SEQ_, sTemp);
			// 바코드 읽기 작업 실패했는지 검사
			if (sTemp == "No-Read")
			{
				m_nStep_Bcr = 5000;	// 에러 반환
			}
			else
			{
				st_serial.nReq_CommSend[PORT_BCR_ACCY_ -1] = REPLY_CORRECT_;
				m_nStep_Bcr = 0;
			}
		}
		else
		{
			m_lWait_Bcr[1] = GetCurrentTime();
			m_lWait_Bcr[2] = m_lWait_Bcr[1] - m_lWait_Bcr[0];
			if (m_lWait_Bcr[2] < 0)
			{
				m_lWait_Bcr[0] = GetCurrentTime();
				break;
			}

			// 1초 동안 기다림
			if (m_lWait_Bcr[2] > 500)
			{
				m_nStep_Bcr = 5000;	// 에러 반환
			}
		}
		break;

	case 5000:
		st_serial.nReq_CommSend[PORT_BCR_ACCY_ -1] = REPLY_ERROR_;
		m_nStep_Bcr = 0;
		break;
	}
}

//==================================================================//
// 시리얼 포트 번호 할당
//==================================================================//
void CSerialCommBase::OnSet_SerialPort(int nzPortNum) 
{
	m_nPortNum = nzPortNum;
}
