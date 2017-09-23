// Seq_LoadingConv.h: interface for the CSeq_LoadingConv class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SEQ_LOADINGCONV_H__0FEE0A6A_7D0C_4D5D_A432_C52C962B1BE9__INCLUDED_)
#define AFX_SEQ_LOADINGCONV_H__0FEE0A6A_7D0C_4D5D_A432_C52C962B1BE9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSeq_LoadingConv  
{
public:
	CSeq_LoadingConv();
	virtual ~CSeq_LoadingConv();

// Attributes
public:
	bool m_bflag_INIT;
	
	int  m_nStep_Init;
 	int  m_nStep_Run;
 	int  m_nStep_Run2;
	int  m_nRetryCnt;
	
	int  m_nRcvyStep;
	int  m_nThirdBoxYesNo;
	CString m_strAlarmCode;

// Attributes
private:
	int  m_nReq_AcMotCtrl[2]; //[0]OnMove_AcMotor[1]OnMove_AcMotor2
	long m_lTime_GoesBy[2][3];
	
	bool m_bflag_Stopper[2];
	long m_lWait_Stopper[2][3];

	//kwlee 2017.0707
	bool m_bflag_Cylinder[2];
	long m_lWait_Cylinder[2][3];

	bool m_bflag_Clamp[2];
	long m_lWait_Clamp[2][3];

	long m_lWaitTime[2][3]; //kwlee 2017.0824

// Operations
public:
	void OnSeq_RUN(void);		// 쓰레드 동작
	void OnSeq_INIT(void);		// 쓰레드 관련 변수 초기화
	void OnSeq_Execute(void);	// 쓰레드 제어\

	void OnRun_Initial();			// Seq_LoadingConv의 초기화 작업
	void OnRun_Move_Front();			// Seq_LoadingConv의 시컨스 동작
	void OnRun_Move_Rear();         //kwlee 2017.0706
	
	int OnCheck_AccyBoxConvInFirst(int nzOnOff, int nzMode=CHK_EXIST_);
	int OnCheck_AccyBoxConvInSecond(int nzOnOff, int nzMode=CHK_EXIST_);
	int OnCheck_AccyBoxConvInThird(int nzOnOff, int nzMode=CHK_EXIST_);

	void OnMove_AcMotor(int nzMoveType);
	void OnMove_AcMotor2(int nzMoveType);

	void OnSet_CylStopper(int nzOnOff);
	int OnGet_CylStopper(int nzOnOff);

	void OnSet_CylStopper2(int nzOnOff);
	int OnGet_CylStopper2(int nzOnOff);

	//kwlee 2017.0706
	void OnSet_CylBoxClamp(int nzOnOff);
	int  OnGet_CylBoxClamp(int nzOnOff);

	//kwlee 2017.0707
	void OnSet_CylBoxPusher(int nzOnOff);
	int  OnGet_CylBoxPusher(int nzOnOff);




};
extern CSeq_LoadingConv	SeqLoadingConv;

#endif // !defined(AFX_SEQ_LOADINGCONV_H__0FEE0A6A_7D0C_4D5D_A432_C52C962B1BE9__INCLUDED_)
