// Seq_BoxClamp.h: interface for the CSeq_BoxClamp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SEQ_BOXCLAMP_H__97F8C636_98CE_4684_97CE_B8F124A23722__INCLUDED_)
#define AFX_SEQ_BOXCLAMP_H__97F8C636_98CE_4684_97CE_B8F124A23722__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSeq_BoxClamp  
{
public:
	CSeq_BoxClamp();
	virtual ~CSeq_BoxClamp();

// Attributes
public:
	bool m_bflag_INIT;
	
	int  m_nStep_Init;
	int  m_nStep_Run;

	int  m_nRcvyStep;

	int  m_nStep_ForceDischarge;	// Box 배출 Step

// Attributes
private:
	bool m_bflag_ConvClamp;
	long m_lWait_ConvClamp[3];

	bool m_bflag_ConvPusher;
	long m_lWait_ConvPusher[3];

	bool m_bflag_Clamp;
	long m_lWait_Clamp[3];

	bool m_bflag_Stopper;
	long m_lWait_Stopper[3];

	bool m_bflag_Supply;
	long m_lWait_Supply[3];

	bool m_bflag_UDGripper;
	long m_lWait_UDGripper[3];

	bool m_bflag_Lifter;
	long m_lWait_Lifter[3];

	// 규이리 추가 [2015.03.10]
	int  m_nResp_Move[5];

	long m_lTime_GoesBy[3];

	int  m_nPusher_Retry;
// Operations
public:
	void OnSeq_RUN(void);		// 쓰레드 동작
	void OnSeq_INIT(void);		// 쓰레드 관련 변수 초기화
	void OnSeq_Execute(void);	// 쓰레드 제어

	void OnRun_Initial();		// Seq_BoxClamp의 초기화 작업
	void OnRun_Move();			// Seq_BoxClamp의 시컨스 동작
	int OnRun_ForceDischarge();

	bool OnLoadingConvRun_StateCheck();

	void OnSet_ConvBoxClamp(int nzOnOff);
	int OnGet_ConvBoxClamp(int nzOnOff);
	
	void OnSet_ConvBoxPusher(int nzOnOff);
	int OnGet_ConvBoxPusher(int nzOnOff);

	void OnSet_CylClamp(int nzOnOff);
	int OnGet_CylClamp(int nzOnOff, int nzSite = SIDE_BOTH_);

	void OnSet_CylStopper(int nzOnOff);
	int OnGet_CylStopper(int nzOnOff, int nzSite = SIDE_BOTH_);

	void OnSet_CylSupply(int nzOnOff);
	int OnGet_CylSupply(int nzOnOff, int nzSite = SIDE_BOTH_);

	void OnSet_CylUDGripper(int nzOnOff);
	int OnGet_CylUDGripper(int nzOnOff, int nzSite = SIDE_BOTH_);

	void OnSet_CylLifter(int nzOnOff);
	int OnGet_CylLifter(int nzOnOff, int nzSite = SIDE_BOTH_);

};
extern CSeq_BoxClamp	SeqBoxClamp;

#endif // !defined(AFX_SEQ_BOXCLAMP_H__97F8C636_98CE_4684_97CE_B8F124A23722__INCLUDED_)
