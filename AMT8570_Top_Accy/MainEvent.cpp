// MainEvent.cpp: implementation of the CMainEvent class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "handler.h"
#include "MainEvent.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMainEvent	g_client_accy;

CMainEvent::CMainEvent()
{
	m_nArrayPos = 0;
	//st_client[m_nArrayPos].str_ip = "192.168.1.10";
// 	st_client[m_nArrayPos].str_ip = "192.168.1.10";
// 	st_client[m_nArrayPos].n_port = 2000;
}

CMainEvent::~CMainEvent()
{

}

//==================================================================//
// ������ ����
//==================================================================//
void CMainEvent::OnSeq_RUN(void) 
{
	OnSeq_Init();     // ������ ���� ���� �ʱ�ȭ
	OnSeq_Execute();  // ������ ����
}

//==================================================================//
// ������ ���� ���� �ʱ�ȭ
//==================================================================//
void CMainEvent::OnSeq_Init(void) 
{
	if (m_bflag_INIT == true)	{
		return;
	}
	m_bflag_INIT = true;
	
	// ���� ���� �ʱ�ȭ
	m_nStep_RUN	= 0;
	
	m_sRcvData.Empty();
}

//==================================================================//
// ������ ����
//==================================================================//
void CMainEvent::OnSeq_Execute(void) 
{
	if (m_nReqStart == REQ_CALL_)
	{
		OnRun_MainWork();		// [MAIN->ACCY] ������ ���� �۾�
	}
}

//==================================================================//
// [MAIN<->ACCY] MAIN �� �б�/���� �۾�
//==================================================================//
int CMainEvent::OnRun_MainWork() 
{
		// [�ʱ�ȭ] �Լ� ���� �÷��� (�۾���)
	int FuncRet = RET_PROCEED;

	CString sMsg;
	switch(m_nStep_RUN)
	{
	case 0:
		// [MAIN<->ACCY] ����Ǿ� �ִ��� Ȯ��
		if (st_client[m_nArrayPos].n_connect == CTL_YES)
		{
			m_nStep_RUN = 1000;
		}
		// ����Ǿ� ���� ������ ���� �۾� �õ�
		else
		{
			::PostMessage(st_handler.hWnd, WM_CLIENT_MSG_1, CLIENT_CONNECT, m_nArrayPos);

			// ���� ���� �ð� ����
			m_lWaitResp[0] = GetCurrentTime();
			m_nStep_RUN = 100;
		}
		break;

	case 100:
		m_lWaitResp[1] = GetCurrentTime();
		m_lWaitResp[2] = m_lWaitResp[1] - m_lWaitResp[0];
		if (m_lWaitResp[2] < 0)
		{
			m_lWaitResp[0] = GetCurrentTime();
			break;
		}

		if (st_client[m_nArrayPos].n_connect == CTL_YES)
		{
			m_nStep_RUN = 1000;
		}
		else
		{
			if (m_lWaitResp[2] > 5000)
			{
				m_nStep_RUN = 5000;	// ���� ��ȯ ����
			}
		}
		break;

	// ������ �޽��� ���� OnReq_MesWork() �Լ� ȣ��
	case 1000:
		sMsg.Format("%s%s", st_client[m_nArrayPos].chSendHead, st_client[m_nArrayPos].chSendBody);
		PushSendMsg(sMsg);	// [EQP->LowerStream] �޽��� ����
		
		m_nIdx = st_client[m_nArrayPos].nIndex;
		// ������ �޽����� ���� ���� ��ٸ� ���� �� ���� ����
		// - [0:default, 1:����, -1:����, 10:��������(��ٸ�), 11:�������]
		if ((st_client[m_nArrayPos].stComm[m_nIdx]).nResponse == 10)
		{
			// Ÿ�Ӿƿ� �˻����� ���� [0:�˻���ʿ�, 1:�˻��ʿ�]
			if ((st_client[m_nArrayPos].stComm[m_nIdx]).nTimeOut == 1)
			{
				m_nStep_RUN = 1500;
			}
			else
			{
				m_nStep_RUN = 1100;
			}
		}
		else
		{
			m_nStep_RUN = 4000;
		}
		break;

	case 1100:
		// ������ �޽����� ���� ���� üũ ���� �ð� ����
		m_lWaitResp[0] = GetCurrentTime();
		m_nStep_RUN = 1200;
		break;
		
	case 1200:
		m_lWaitResp[1] = GetCurrentTime();
		m_lWaitResp[2] = m_lWaitResp[1] - m_lWaitResp[0];
		if (m_lWaitResp[2] < 0)
		{
			m_lWaitResp[0] = GetCurrentTime();
			break;
		}
		
		// [���->�Ϸ�] ���� �޽��� �Ľ� �Լ����� ������
		
		// ������ �޽����� ���� ���� ��ٸ� ���� �� ���� ����
		// - [0:default, 1:����, -1:����, 10:��������(��ٸ�), 11:�������]
		if ((st_client[m_nArrayPos].stComm[m_nIdx]).nResponse == CTL_GOOD)
		{
			m_nStep_RUN = 4000;
		}
		else if ((st_client[m_nArrayPos].stComm[m_nIdx]).nResponse == CTL_ERROR)
		{
			m_nStep_RUN = 5000;
		}
		else
		{
			// ��� Ÿ�� �ƿ� �߻�
			if (m_lWaitResp[2] > COMM_TIME_OUT)
			{
				m_nStep_RUN = 5000;
			}
		}
		break;
		
	case 1500:
		// [���->�Ϸ�] ���� �޽��� �Ľ� �Լ����� ������
		
		// ������ �޽����� ���� ���� ��ٸ� ���� �� ���� ����
		// - [0:default, 1:����, -1:����, 10:��������(��ٸ�), 11:�������]
		if ((st_client[m_nArrayPos].stComm[m_nIdx]).nResponse == CTL_GOOD)
		{
			m_nStep_RUN = 4000;
		}
		else if ((st_client[m_nArrayPos].stComm[m_nIdx]).nResponse == CTL_ERROR)
		{
			m_nStep_RUN = 5000;
		}
		break;

	// ���� �Ϸ� ����
	case 4000:
		m_nReqStart	= REPLY_CORRECT_;

		m_nStep_RUN = 0;
		FuncRet = RET_GOOD;
		break;

		// ���� ��ȯ ����
	case 5000:
		m_nReqStart	= REPLY_ERROR_;

		m_nStep_RUN = 0;
		FuncRet = RET_ERROR;
		break;
	}

	return FuncRet;
}

//==================================================================//
// [Main->Accy] ���ŵ� �޽��� �����Ͽ� ó��
//==================================================================//
int CMainEvent::OnDivide_FromMain(int nzSocketNo, CString szCommand) 
{
	int nRet = 0;
	
	// ���ŵ� �����Ͱ� ������ ���� ����
	if (szCommand == "")
	{
		return -1;
	}
	// kilee [2015.03.20]
	/*
	// ���ŵ� �޽��� �α׿� ����
	m_sLogMsg.Format("Client_[%02d] Data Receive - %s", nzSocketNo, szCommand);
	cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
	*/

	CString sPart_Head, sPart_Len, sPart_Body, sData_Process;
	int nPart_Len, nPart_Body, nLen_TotRcv;

	// ������ ��ɾ��� �ε��� ���� ����
	// - ������ ��ɾ ���� �������� Ȯ���ϱ� ����
	int nIdx = st_client[m_nArrayPos].nIndex;

	m_sRcvData += szCommand;	// ���� �޽��� ������Ŵ
	
	do{
		nLen_TotRcv = m_sRcvData.GetLength();
		// �ּ��� ���̺�+������(1����) �̻��̾�� ��
		if (nLen_TotRcv > 20)
		{
			// �������� ����� ��� [ ���� ��� (HD=) ]
			sPart_Head	= m_sRcvData.Mid(0,3);
			// �������� ���̺� ��� [���� ��� (000025) ]
			sPart_Len	= m_sRcvData.Mid(3,16);
			// ������ �������� ���� ���� ����
			nPart_Len	= atoi((LPCSTR)sPart_Len);
			// ���� �����ͺ� ��� [ ���� ��� (FUNC=INTERFACE,CONNECT=0,) ]
			sPart_Body	= m_sRcvData.Mid(19);
			// Body(������) ���� ����
			nPart_Body	= sPart_Body.GetLength();
			
			// �������� ���ռ� �˻�
			// - ���̺��� ���ڰ� ���������� �˻�
			// : ���̺��� ù���ڸ� ����
			// : �� �κ��� ���������� �����Ǿ� �־�� ��
			if (sPart_Head == "HD=" &&
				m_sRcvData.GetAt(3) >= '0' && m_sRcvData.GetAt(3) <= '9')
			{
				sData_Process = m_sRcvData.Mid(0, nPart_Len +19);
				// ���� ���ŵ� ���ڿ� ���� ����
				nLen_TotRcv = sData_Process.GetLength();
				// ó���ϱ� ���� �̾Ƴ� �����ʹ� ������
				// - ���ŵ� �����Ϳ��� ó���� ���ڿ���ŭ ������Ŵ
				// - ó���ϰ� ���� �����͸� ���� ������ ���Ҵ�
				m_sRcvData = m_sRcvData.Mid(nLen_TotRcv);
				
				// ������ 2�� ���޾� (�پ) ������ ��� �̰����� ��ٷ� �Ľ��ϴ� �Լ� ȣ���ϸ� ������ �߻���
				// - ó���� ���ŵ� ��ɾ�� ó���Ǵµ� �ڿ� �پ ���ŵ� ��ɾ�� ó������ ���ϰ� ���� ��ɾ� ���ŵ� ��쿡 ó���ǰ� ��
				// - ��, ��ɾ �з��� ó���� �� ����
				// - ������ ���� �޽��� �ǰ�δ� [��ɾ�<->����] �����̹Ƿ� 2�� ��ɾ ���޾� ���ŵ��� �ʾ� ������ ����
				int nResp = OnAnalysis_MesData(sData_Process);
				if (nResp == RET_ERROR)
				{
					//m_sRcvData = "";
					
					// ���� ������ �Ľ� �۾� �� ���� �߻���
					// ������ �޽����� ���� ���� ��ٸ� ���� �� ���� ����
					// - [0:default, 1:����, -1:����, 10:��������(��ٸ�), 11:�������]
					(st_client[m_nArrayPos].stComm[nIdx]).nResponse = CTL_ERROR;
				}
				else if (nResp == RET_GOOD)
				{
					m_sRcvData = "";
					
					//PushSendMsg(sData_Process);
					
					// ���� ������ ���� �Ľ̵�
					// ������ �޽����� ���� ���� ��ٸ� ���� �� ���� ����
					// - [0:default, 1:����, -1:����, 10:��������(��ٸ�), 11:�������]
					(st_client[m_nArrayPos].stComm[nIdx]).nResponse = CTL_GOOD;
				}
			}
			else
			{
				// ó���ؾ� �� �Ѱ��� ��ɾ� ���̺��� ���� �������� ���̰� ũ�鼭 ��ɾ� ���信 ������ �ִ� ���
				// - ������ ���۸� ��� ���� ���� �����ͺ��� ó���ǵ��� ��
				if (nPart_Body > nPart_Len)
				{
					// ���ݱ��� ���ŵ� �����ʹ� �����
					// - ���Ŀ� ���ŵǴ� �����ͺ��� ó���ǵ��� ��
					m_sRcvData.Empty();
				}
				// ���ŵ� �����Ͱ� ó�� �Ұ����� ������
				// - ���� ���� �޽����� ó���� ���� ������ ���Ͻ�Ŵ
				nRet = -1;
			}
		}
		// ��ɾ� �ּ� ���� ������ �������� ����
		// - �����͸� �� �����ؾ� ��
		else
		{
			nRet = -1;
		}
    }while (nRet != -1);
	return 0;
}

//==================================================================//
// ���ŵ� �޽��� �м� �۾�
//==================================================================//
int CMainEvent::OnAnalysis_MesData(CString szRcvData) 
{
	/*
	// ��� ������ �����س�
	// - ���� �Ǵ� ã�� ���ڿ��� ��ȯ��
	CString sHeader = OnAnalysis_MesBody(0, "FUNCTION=", szRcvData);
	CString sBody[5];

	// ������ ��ɾ��� �ε��� ���� ����
	// - ������ ��ɾ ���� �������� Ȯ���ϱ� ����
	int nIdx = st_client[m_nArrayPos].nIndex;

	if (sHeader == "ACCY_MATH_CODE")	
	{
		st_work.m_sAccyMathCode = OnAnalysis_MesBody(0, "VALUE=", szRcvData);
	}
	else
	{
		return RET_ERROR;
	}
	*/

	CString sHeader = OnFindInRcvMsg("FUNCTION=", szRcvData, ";");
	CString sBody[5];
	
	// ������ ��ɾ��� �ε��� ���� ����
	// - ������ ��ɾ ���� �������� Ȯ���ϱ� ����
	int nIdx = st_client[m_nArrayPos].nIndex;
	
	if (sHeader == "ACCY_MATH_CODE")	
	{
		st_work.m_sRcvAccyMathCode = OnFindInRcvMsg("VALUE=", szRcvData, ";");
	}
	else
	{
		return RET_ERROR;
	}

	return RET_GOOD;
}

//==================================================================//
// Body ���� �м�
//==================================================================//
CString CMainEvent::OnAnalysis_MesBody(int nzMode, CString szFind, CString szBody) 
{
	CString sFindData = "";

	int nLen;
	int nPos[4];

	// ���ڿ� ���� ���
	nLen = szBody.GetLength();

	nPos[0] = -1;
	// ã���� �ϴ� ���ڿ��� ��ġ ����
	nPos[0] = szBody.Find(szFind, 0);
	// ã���� �ϴ� ���ڿ��� �������� ������ ���� ���� ��ȯ
	if (nPos[0] < 0)
	{
		return sFindData;
	}

	nPos[1] = -1;
	nPos[1] = szBody.Find("=", nPos[0] +1);
	if (nPos[1] < 0)
	{
		return sFindData;
	}

	switch(nzMode)
	{
	case 0:
		nPos[2] = -1;
		nPos[2] = szBody.Find(";", nPos[0] +1);

		if (nPos[2] == -1)
		{
			sFindData = szBody.Mid(nPos[1] +1, nLen - (nPos[1] +1));
		}
		else	{
			sFindData = szBody.Mid(nPos[1] +1, nPos[2] - (nPos[1] +1));
		}
		break;

	case 1:
		nPos[2] = -1;
		nPos[2] = szBody.Find(")", nPos[1] +1);
		
		sFindData = szBody.Mid(nPos[1] +1, nPos[2] - (nPos[1]));
		break;

	case 2:
		nPos[2] = -1;
		nPos[2] = szBody.Find("^XZ", nPos[1] +1);

		sFindData = szBody.Mid(nPos[1] +1, nPos[2] - (nPos[1] +1));
		break;

	case 3:
		nPos[2] = -1;
		nPos[2] = szBody.Find("\"", nPos[1] +1);

		nPos[3] = -1;
		nPos[3] = szBody.Find("\"", nPos[2] +1);

		sFindData = szBody.Mid(nPos[2] +1, nPos[3] - (nPos[2] +1));
		break;

	case 4:
		nPos[2] = -1;
		nPos[2] = szBody.GetLength();

		sFindData = szBody.Mid(nPos[1] +1, nPos[2] - (nPos[1]));
		break;
	}

	return sFindData;
}

//==================================================================//
// [EQP->MES] �޽��� ����
//==================================================================//
int CMainEvent::PushSendMsg(CString szMsg) 
{
	// ������ ���ڿ� ����
	sprintf(st_client[m_nArrayPos].ch_send, szMsg);
	//::SendMessage(st_handler.hWnd, WM_CLIENT_MSG_1, CLIENT_SEND, m_nArrayPos);
	::PostMessage(st_handler.hWnd, WM_CLIENT_MSG + m_nArrayPos, CLIENT_SEND, m_nArrayPos);
	
	return 0;
}

//==================================================================//
// [MAIN->ACCY] ������ �޽��� ����
//==================================================================//
void CMainEvent::OnCreate_SendFormat(int nzIdx) 
{
	CString sFunction;
	CString sHead, sBody, sTemp, sBuff;
	int nReadIO[3] = {0,};

	switch(nzIdx)
	{
	case MAIN_BCR_STATE:
		sTemp.Empty();
		sBody.Format("FUNCTION_RPY=BCR_STATE;EQP_ID=%d;", st_map.nSupplyTypeMath);
		sHead.Format("HD=%016d;", sBody.GetLength());
		
		sprintf(st_client[m_nArrayPos].chSendHead, sHead);
		sprintf(st_client[m_nArrayPos].chSendBody, sBody);
		
		sFunction = "BCR_STATE";
		sprintf((st_client[m_nArrayPos].stComm[nzIdx]).chSendCmd, sFunction);
		sFunction = "BCR_STATE";
		sprintf((st_client[m_nArrayPos].stComm[nzIdx]).chResponse, sFunction);
		break;
		
	case MAIN_ALARM:
		sTemp.Empty();
		sBody.Format("FUNCTION_RPY=ALARM;ALARM_CODE=%s;ALARM_TITLE=%s;", alarm.mstr_code, st_alarm.mstr_cur_msg);
		sHead.Format("HD=%016d;", sBody.GetLength());
		
		sprintf(st_client[m_nArrayPos].chSendHead, sHead);
		sprintf(st_client[m_nArrayPos].chSendBody, sBody);
		
		sFunction = "ALARM";
		sprintf((st_client[m_nArrayPos].stComm[nzIdx]).chSendCmd, sFunction);
		sFunction = "ALARM";
		sprintf((st_client[m_nArrayPos].stComm[nzIdx]).chResponse, sFunction);
		break;
		
	case MAIN_ACCY_SUPPLY_COMPLETE:
		sTemp.Empty();
		sBody.Format("FUNCTION_RPY=COMPLETE;");
		sHead.Format("HD=%016d;", sBody.GetLength());
		
		sprintf(st_client[m_nArrayPos].chSendHead, sHead);
		sprintf(st_client[m_nArrayPos].chSendBody, sBody);
		
		sFunction = "COMPLETE";
		sprintf((st_client[m_nArrayPos].stComm[nzIdx]).chSendCmd, sFunction);
		sFunction = "COMPLETE";
		sprintf((st_client[m_nArrayPos].stComm[nzIdx]).chResponse, sFunction);
		break;
		
	case MAIN_EQP_STATE:
		sTemp.Empty();
		sBody.Format("FUNCTION_RPY=EQP_STATE;EQP_STATE=%d;", st_work.nEqpStatus);
		sHead.Format("HD=%016d;", sBody.GetLength());
		
		sprintf(st_client[m_nArrayPos].chSendHead, sHead);
		sprintf(st_client[m_nArrayPos].chSendBody, sBody);
		
		sFunction = "EQP_STATE";
		sprintf((st_client[m_nArrayPos].stComm[nzIdx]).chSendCmd, sFunction);
		sFunction = "EQP_STATE";
		sprintf((st_client[m_nArrayPos].stComm[nzIdx]).chResponse, sFunction);
		break;
		
	case MAIN_SUPPLY_TYPE:
		sTemp.Empty();
		sBody.Format("FUNCTION_RPY=SUPPLY_TYPE;LOT_TYPE=%d;", st_basic.nMode_SupplyDevice);
		sHead.Format("HD=%016d;", sBody.GetLength());
		
		sprintf(st_client[m_nArrayPos].chSendHead, sHead);
		sprintf(st_client[m_nArrayPos].chSendBody, sBody);
		
		sFunction = "SUPPLY_TYPE";
		sprintf((st_client[m_nArrayPos].stComm[nzIdx]).chSendCmd, sFunction);
		sFunction = "SUPPLY_TYPE";
		sprintf((st_client[m_nArrayPos].stComm[nzIdx]).chResponse, sFunction);
		break;

	case MAIN_SUPPLY_READY_CNT:
		sTemp.Empty();
		
		// yangkyu [2015.02.26]
		// ������ B���̶� ���� ���� Ȯ�� �� ���� ���
		nReadIO[0] = !g_ioMgr.get_in_bit(stIO.i_Chk_AccyBoxConvDetection[SIDE_THIRD_]);
		nReadIO[1] = !g_ioMgr.get_in_bit(stIO.i_Chk_AccyBoxConvDetection[SIDE_SECOND_]);
		nReadIO[2] = !g_ioMgr.get_in_bit(stIO.i_Chk_AccyBoxConvDetection[SIDE_FIRST_]);
		
		sBody.Format("FUNCTION_RPY=SUPPLY_CNT;ACCY_CNT=%d;", st_map.nResp_AccyReadyCNT + (nReadIO[0] * 160) + (nReadIO[1] * 160) + (nReadIO[2] * 160));
		sHead.Format("HD=%016d;", sBody.GetLength());
		
		sprintf(st_client[m_nArrayPos].chSendHead, sHead);
		sprintf(st_client[m_nArrayPos].chSendBody, sBody);
		
		sFunction = "SUPPLY_CNT";
		sprintf((st_client[m_nArrayPos].stComm[nzIdx]).chSendCmd, sFunction);
		sFunction = "SUPPLY_CNT";
		sprintf((st_client[m_nArrayPos].stComm[nzIdx]).chResponse, sFunction);
		break;

		// yangkyu [2015.03.19]
		// ��� �������� �߰�
	case MAIN_ACCY_MATH_CODE:
		sTemp.Empty();
		sBody.Format("FUNCTION_RPY=REQ_ACCY_MATH_CODE;");
		sHead.Format("HD=%016d;", sBody.GetLength());
		
		sprintf(st_client[m_nArrayPos].chSendHead, sHead);
		sprintf(st_client[m_nArrayPos].chSendBody, sBody);
		
		sFunction = "REQ_ACCY_MATH_CODE";
		sprintf((st_client[m_nArrayPos].stComm[nzIdx]).chSendCmd, sFunction);
		sFunction = "REQ_ACCY_MATH_CODE";
		sprintf((st_client[m_nArrayPos].stComm[nzIdx]).chResponse, sFunction);
		break;
	}
}



//==================================================================//
// Ư���� ���ڿ� ������ ������ ��ȯ
//==================================================================//
CString CMainEvent::OnFindInRcvMsg(CString szFind, CString szOrg, CString szDelimiter) 
{
	CString sRetChar;	// ������ ���ڿ� ���� ����
	
	int nOrgLen		= szOrg.GetLength();	// �Էµ� ���ڿ��� ��ü ����
	// ��ü ���ڿ� �߿��� ã���� �ϴ� ���ڿ��� ���� ��ġ �˻�
	int nFindPos	= szOrg.Find(szFind);
	int nFindLen	= szFind.GetLength();	// ã�� ���ڿ��� �� ���� ���
	// ������ ������ ������ ��ġ �˻�
	// - ã���� �ϴ� ���ڿ��� ���� ��ġ���� ã�� ������
	//int nEndPos	= szOrg.Find(szDelimiter, nFindPos+1);
	int nEndPos	= szOrg.Find(szDelimiter, (nFindPos+nFindLen));
	// ���� ���� Ȯ��
	//if ((nFindPos<0 || nEndPos<0) || (nEndPos <= (nFindPos+nFindLen)))
	// ���̸� ���� [2014.11.24]
	// ������ ��ġ�� ������ �����ڰ� �������� �ʾ� ����
	if (nFindPos<0 ||
		(nEndPos >= 0 && (nEndPos < (nFindPos+nFindLen)+1)))
	{
		sRetChar = "";
	}
	// ������ ������ �����ڰ� ����
	else if (nEndPos<0)
	{
		sRetChar = szOrg.Mid(nFindPos+nFindLen, nOrgLen-(nFindPos+nFindLen));
	}
	else
	{
		// [�˻����ڿ�~������] ������ ���� ����
		sRetChar = szOrg.Mid(nFindPos+nFindLen, nEndPos-(nFindPos+nFindLen));
	}
	return sRetChar;
}
