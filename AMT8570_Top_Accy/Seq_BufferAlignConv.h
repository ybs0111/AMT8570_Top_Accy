// Seq_BufferAlignConv.h: interface for the CSeq_BufferAlignConv class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SEQ_BUFFERALIGNCONV_H__1320F1F3_E097_4F21_978C_E9E1796E2559__INCLUDED_)
#define AFX_SEQ_BUFFERALIGNCONV_H__1320F1F3_E097_4F21_978C_E9E1796E2559__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSeq_BufferAlignConv  
{
public:
	CSeq_BufferAlignConv();
	virtual ~CSeq_BufferAlignConv();

// Attributes
public:
	bool m_bflag_INIT;
	
	int  m_nStep_Init;
	int  m_nStep_Run;
	int  m_nStep_RunReady;
	int  m_nStep_RunMiddle;

	int  m_nStep_ForceDischarge;

// Attributes
private:
	int  m_nReq_AcMotCtrl;//Req AC action.
	long m_lTime_GoesBy[3][3];

	bool m_bflag_BufferTrayPathAlign;
	long m_lWait_BufferTrayPathAlign[3];

	bool m_bflag_BufferFrontTrayPitch;
	long m_lWait_BufferFrontTrayPitch[3];

	bool m_bflag_BufferRearTrayPitch;
	long m_lWait_BufferRearTrayPitch[3];

	bool m_bflag_Stopper[5];
	long m_lWait_Stopper[5][3];

	int  m_nReq_Completemsg;

	int m_nTestSupply;			// 0 : 기본 모드, 1 : 테스트 모드
	int m_nAccySupply;
	int m_nReqMidConv;
	int m_nReqOutConv;  //OutConv->BuffConv

// Operations
public:
	void OnSeq_RUN(void);		// 쓰레드 동작
	void OnSeq_INIT(void);		// 쓰레드 관련 변수 초기화
	void OnSeq_Execute(void);	// 쓰레드 제어

	void OnRun_Initial();		// Seq_BufferAlignConv의 초기화 작업
	void OnRun_Move();			// Seq_BufferAlignConv의 시컨스 동작
	void OnRun_ReadyMove();
	void OnRun_MiddleMove();
	int OnRun_ForceDischarge(); 

	void OnMove_AcMotor(int nzMoveType);

	int OnCheck_FrontAccySupply(int nzOnOff, int nzMode=CHK_EXIST_, int nzSite=SIDE_BOTH_);
	int OnCheck_RearAccySupply(int nzOnOff, int nzMode=CHK_EXIST_, int nzSite=SIDE_BOTH_);
	int OnCheck_MiddleAccySupply(int nzOnOff, int nzMode=CHK_EXIST_, int nzSite=SIDE_BOTH_);
	int OnCheck_OutFrontAccySupply(int nzOnOff, int nzMode=CHK_EXIST_, int nzSite=SIDE_BOTH_);
	int OnCheck_OutRearAccySupply(int nzOnOff, int nzMode=CHK_EXIST_, int nzSite=SIDE_BOTH_);
	int OnCheck_OutAccySupply(int nzOnOff, int nzMode=CHK_EXIST_, int nzSite=SIDE_BOTH_);
	///////////////////////////////////////////////////////////////////////////////////
	void OnSet_CylBufferFrontTrayPitch(int nzOnOff);
	int OnGet_CylBufferFrontTrayPitch(int nzOnOff, int nzSite=SIDE_BOTH_);

	void OnSet_CylBufferRearTrayPitch(int nzOnOff);
	int OnGet_CylBufferRearTrayPitch(int nzOnOff, int nzSite=SIDE_BOTH_);

	///////////////////////////////////////////////////////////////////////////////////
	
	void OnSet_CylBufferTrayPathAlign(int nzOnOff);
	int OnGet_CylBufferTrayPathAlign(int nzOnOff, int nzSite=SIDE_BOTH_);

	void OnSet_CylStopper(int nzOnOff, int nPos, int nzSite=SIDE_BOTH_);
	int OnGet_CylStopper(int nzOnOff, int nPos, int nzSite=SIDE_BOTH_);
};
extern CSeq_BufferAlignConv	SeqBufferAlignConv;

#endif // !defined(AFX_SEQ_BUFFERALIGNCONV_H__1320F1F3_E097_4F21_978C_E9E1796E2559__INCLUDED_)
