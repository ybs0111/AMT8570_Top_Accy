// BcrEvent.h: interface for the CBcrEvent class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BCREVENT_H__E680F49C_DABB_449D_B535_4524DED94AB5__INCLUDED_)
#define AFX_BCREVENT_H__E680F49C_DABB_449D_B535_4524DED94AB5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Variable.h"
#include "SerialCommBase.h"

class CBcrEvent  
{
public:
	CBcrEvent();
	virtual ~CBcrEvent();

// Attributes
private:
	CString m_sLogMsg;
	CString m_sRcvedData[MAX_PORT];  // 수신 데이터 임시 저장
	
// Attributes
public:
	bool m_bflag_INIT;
	
	// 시리얼 통신 클래스 접근자 선언
	CSerialCommBase	m_cComm_ACCY;
	
// Operations
public:
	void OnSeq_RUN(void);      // 쓰레드 동작
	void OnSeq_Init(void);     // 쓰레드 관련 변수 초기화
	void OnSeq_Execute(void);  // 쓰레드 제어

	void OnRcvDataFromBCR(WPARAM ch, LPARAM port);	// [EQP<-BCR] 바코드 리더기로부터 메시지 수신
	int  OnSet_2Struct(int nzPort);	// 구조체에 수신된 정보 설정

	void OnReq_ACCY()	{ st_serial.nReq_CommSend[PORT_BCR_ACCY_  -1]	= REQ_CALL_; }

	int  OnGet_ACCY()	{ return st_serial.nReq_CommSend[PORT_BCR_ACCY_  -1]; }

};
extern CBcrEvent	evBCR;

#endif // !defined(AFX_BCREVENT_H__E680F49C_DABB_449D_B535_4524DED94AB5__INCLUDED_)
