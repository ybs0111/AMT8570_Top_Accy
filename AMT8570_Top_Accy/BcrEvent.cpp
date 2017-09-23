// BcrEvent.cpp: implementation of the CBcrEvent class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BcrEvent.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CBcrEvent	evBCR;

CBcrEvent::CBcrEvent()
{

}

CBcrEvent::~CBcrEvent()
{

}

//==================================================================//
// ������ ����
//==================================================================//
void CBcrEvent::OnSeq_RUN(void) 
{
}

//==================================================================//
// ������ ���� ���� �ʱ�ȭ
//==================================================================//
void CBcrEvent::OnSeq_Init(void) 
{

}

//==================================================================//
// ������ ����
//==================================================================//
void CBcrEvent::OnSeq_Execute(void) 
{
}

//==================================================================//
// [EQP<-BCR] �޽��� ����
//==================================================================//
void CBcrEvent::OnRcvDataFromBCR(WPARAM ch, LPARAM port) 
{
	// ��Ʈ ���� ����
	int nPort = (int)port;
	int nLen;
	int FuncRet = RET_PROCEED;

	// ������ ���� �Է� [CR] �Է����� Ȯ��
	if (ch == 0x0A)
	{
		m_sRcvedData[nPort -1] += (char)ch;
		//m_sRcvedData[nPort-1].TrimLeft(' ');
		//m_sRcvedData[nPort-1].TrimRight(' ');
		st_serial.comm_rec[nPort -1] = m_sRcvedData[nPort -1];
		(m_sRcvedData[nPort -1]).Empty();

		m_sLogMsg.Format( "[RCV]BCR:%s", st_serial.comm_rec[nPort -1]);
		cLOG.OnLogEvent(LOG_COMM_, m_sLogMsg);

		// ���� �߰� [2015.05.19]
		// BCR Test Mode �ϰ�쿡�� ȭ�鿡 ��� �� �������� ��.
		if (st_work.nModeBCR_Test == TRUE)
		{
			st_work.nModeBCR_Test = FALSE;
			// ȭ�鿡 ���� �޽��� ��� ��û
			if (st_handler.cwnd_list != NULL)
			{
				st_other.str_op_msg = st_serial.comm_rec[nPort -1];
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, BCR_PORT, RECEIVE_MSG);
			}
			return;
		}

		// ���� �޽����� ��ü ���� ���
		// �ּ� 3���� �̻� ���ŵǾ�� �������� �޽����� ����
		nLen = (st_serial.comm_rec[nPort -1]).GetLength();
		if (nLen > 2)
		{
			// CR+LF ������ ���ڿ��� ������
			st_serial.comm_rec[nPort -1] = (st_serial.comm_rec[nPort-1]).Mid(0, nLen-2);

			/*
			OnSet_2Struct(nPort);	// ����ü�� ���ŵ� ���� ����
			st_serial.n_rec_chk[nPort -1] = TRUE;
			*/
			// ���̸� ���� [2015.01.03]
			if (st_serial.comm_rec[nPort -1] != "No-Read")
			{
				FuncRet = OnSet_2Struct(nPort);	// ����ü�� ���ŵ� ���� ����
				if (FuncRet == CTL_YES)
				{
					st_serial.n_rec_chk[nPort -1] = TRUE;
				}
				else
				{
					st_serial.n_rec_chk[nPort -1] = FALSE;
				}
			}
			else
			{
				st_serial.n_rec_chk[nPort -1] = COM_ERROR;
			}
		}
		else
		{
			st_serial.n_rec_chk[nPort -1] = COM_ERROR;
		}

		// ȭ�鿡 ���� �޽��� ��� ��û
		if (st_handler.cwnd_list != NULL)
		{
			st_other.str_op_msg = st_serial.comm_rec[nPort -1];
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, BCR_PORT, RECEIVE_MSG);
		}
	}
	else
	{
		
		m_sRcvedData[port -1] += (char)ch;
		//kwlee 2017.0825
		if (ch == 0x03)
		{
			st_serial.comm_rec[nPort -1] = m_sRcvedData[nPort -1];
			(m_sRcvedData[nPort -1]).Empty();
		}
	}
}

//==================================================================//
// ����ü�� ���ŵ� ���� ����
//==================================================================//
int CBcrEvent::OnSet_2Struct(int nzPort) 
{
	int nRet = CTL_NO;	// ��ȯ ���� ����
	CString sDataBcr = st_serial.comm_rec[nzPort -1];

	//==============================================================//
	// ���̸� �߰� [2015.03.20]
	// �޽����� 2���� �����Ǿ� ���� (a;b;)
	// - �տ� ������ �Ǽ��縮 ��Ī �ڵ��̰� ���� ������ �Ǽ��縮 �ڽ� �ڵ���
	//==============================================================//
	CString sAcceMatCode;
	int iStart=0, iEnd;

	iEnd = sDataBcr.Find(';', iStart);
	if (iEnd == -1)
	{
		sAcceMatCode = sDataBcr;
	}
	else
	{
		sAcceMatCode = sDataBcr.Mid(iStart, (iEnd - iStart));
	}
	//==============================================================//

	//if (st_work.sBCR_Data[0] == NULL && st_map.nBCR_State[0] != BCR_READ_BAD_)
	// ���̸� ���� [2015.03.20]
	if (st_work.sRbtBCR_Data[0] == "" && st_map.nBCR_State[0] != BCR_READ_BAD_)
	{
		//st_work.sBCR_Data[0] = sDataBcr;
		// ���̸� ���� [2015.03.20]
		st_work.sRbtBCR_Data[0] = sAcceMatCode;
		nRet = CTL_YES;
		// kilee [2015.03.23]
		return nRet;
	}
	//else if (st_work.sBCR_Data[1] == NULL && st_map.nBCR_State[1] != BCR_READ_BAD_)
	// ���̸� ���� [2015.03.20]
	else if (st_work.sRbtBCR_Data[1] == "" && st_map.nBCR_State[1] != BCR_READ_BAD_)
	{
		//st_work.sBCR_Data[1] = sDataBcr;
		// ���̸� ���� [2015.03.20]
		st_work.sRbtBCR_Data[1] = sAcceMatCode;
		nRet = CTL_YES;
		// kilee [2015.03.23]
		return nRet;
	}
	//else if (st_work.sBCR_Data[3] == NULL && st_map.nBCR_State[3] != BCR_READ_BAD_)
	// ���̸� ���� [2015.03.20]
	else if (st_work.sRbtBCR_Data[3] == "" && st_map.nBCR_State[3] != BCR_READ_BAD_)
	{
		//st_work.sBCR_Data[3] = sDataBcr;
		// ���̸� ���� [2015.03.20]
		st_work.sRbtBCR_Data[3] = sAcceMatCode;
		nRet = CTL_YES;
		// kilee [2015.03.23]
		return nRet;
	}
	//else if (st_work.sBCR_Data[2] == NULL && st_map.nBCR_State[2] != BCR_READ_BAD_)
	// ���̸� ���� [2015.03.20]
	else if (st_work.sRbtBCR_Data[2] == "" && st_map.nBCR_State[2] != BCR_READ_BAD_)
	{
		//st_work.sBCR_Data[2] = sDataBcr;
		// ���̸� ���� [2015.03.20]
		st_work.sRbtBCR_Data[2] = sAcceMatCode;
		nRet = CTL_YES;
	}
	return nRet;
}