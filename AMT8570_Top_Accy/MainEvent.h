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
	void OnSeq_RUN(void);      // 쓰레드 동작
	void OnSeq_Init(void);     // 쓰레드 관련 변수 초기화
	void OnSeq_Execute(void);  // 쓰레드 제어

	int  OnRun_MainWork();	// [EQP<->RFID] RFID 정보 읽기/쓰기 작업

	int OnDivide_FromMain(int nzSocketNo, CString szCommand);	// [Main->Accy] 수신된 메시지 구분하여 처리
	int OnAnalysis_MesData(CString szRcvData);	// 수신된 메시지 분석 작업
	CString OnAnalysis_MesBody(int nzMode, CString szFind, CString szBody);	// Body 정보 분석

	void OnReq_CommStart()	{ m_nReqStart  = REQ_CALL_; }
	int  OnGet_CommEnd()	{ return m_nReqStart; }

	int  PushSendMsg(CString szMsg);	// [EQP->MES] 메시지 전송
	void OnCreate_SendFormat(int nzIdx); 

	CString OnFindInRcvMsg(CString szFind, CString szOrg, CString szDelimiter);	// 특정한 문자열 사이의 데이터 반환

};
extern CMainEvent	g_client_accy;

#endif // !defined(AFX_MAINEVENT_H__9D4F96BF_AB8A_4549_8853_3AAF057CEB2D__INCLUDED_)
