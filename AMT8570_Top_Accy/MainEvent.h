// MainEvent.h: interface for the CMainEvent class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINEVENT_H__9D4F96BF_AB8A_4549_8853_3AAF057CEB2D__INCLUDED_)
#define AFX_MAINEVENT_H__9D4F96BF_AB8A_4549_8853_3AAF057CEB2D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMainEvent  
{
public:
	CMainEvent();
	virtual ~CMainEvent();

// Attributes
private:
	CString m_sLogMsg;
	CString m_sRcvData;

	int  m_nDataParse;
	long m_lWaitResp[3];

	int m_nArrayPos;

	int  m_nReqStart;
	int  m_nIdx;

	int  m_nReqMainWork;

// Attributes
public:
	bool m_bflag_INIT;
	int  m_nStep_RUN;

// Operations
public:
	void OnSeq_RUN(void);      // ������ ����
	void OnSeq_Init(void);     // ������ ���� ���� �ʱ�ȭ
	void OnSeq_Execute(void);  // ������ ����

	int  OnRun_MainWork();	// [EQP<->RFID] RFID ���� �б�/���� �۾�

	int OnDivide_FromMain(int nzSocketNo, CString szCommand);	// [Main->Accy] ���ŵ� �޽��� �����Ͽ� ó��
	int OnAnalysis_MesData(CString szRcvData);	// ���ŵ� �޽��� �м� �۾�
	CString OnAnalysis_MesBody(int nzMode, CString szFind, CString szBody);	// Body ���� �м�

	void OnReq_CommStart()	{ m_nReqStart  = REQ_CALL_; }
	int  OnGet_CommEnd()	{ return m_nReqStart; }

	int  PushSendMsg(CString szMsg);	// [EQP->MES] �޽��� ����
	void OnCreate_SendFormat(int nzIdx); 

	CString OnFindInRcvMsg(CString szFind, CString szOrg, CString szDelimiter);	// Ư���� ���ڿ� ������ ������ ��ȯ

};
extern CMainEvent	g_client_accy;

#endif // !defined(AFX_MAINEVENT_H__9D4F96BF_AB8A_4549_8853_3AAF057CEB2D__INCLUDED_)
