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
// [������] ���ڵ� �б� �۾�
//==================================================================//
void CSerialCommBase::OnRun_BarCode() 
{
	CString sTemp;

	switch(m_nStep_Bcr)
	{
	case 0:
		// �޽��� ���� ��û �߻�
		if (st_serial.nReq_CommSend[PORT_BCR_ACCY_ -1] == REQ_CALL_)
		{
			m_nStep_Bcr = 1000;
		}
		break;

	case 1000:
		// ���ڵ� �б� ��û
		::PostMessage(st_handler.hWnd, WM_DATA_SEND, PORT_BCR_ACCY_, 0);
		st_serial.n_rec_chk[PORT_BCR_ACCY_ -1] = FALSE;

		m_lWait_Bcr[0] = GetCurrentTime();
		m_nStep_Bcr = 1100;
		break;

	case 1100:
		// ���� ��ٸ�
		if (st_serial.n_rec_chk[PORT_BCR_ACCY_ -1] == YES)
		{
			sTemp = st_serial.comm_rec[PORT_BCR_ACCY_ -1];
			cLOG.OnLogEvent(LOG_SEQ_, sTemp);
			// ���ڵ� �б� �۾� �����ߴ��� �˻�
			if (sTemp == "No-Read")
			{
				m_nStep_Bcr = 5000;	// ���� ��ȯ
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

			// 1�� ���� ��ٸ�
			if (m_lWait_Bcr[2] > 500)
			{
				m_nStep_Bcr = 5000;	// ���� ��ȯ
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
// �ø��� ��Ʈ ��ȣ �Ҵ�
//==================================================================//
void CSerialCommBase::OnSet_SerialPort(int nzPortNum) 
{
	m_nPortNum = nzPortNum;
}
