// Seq_XYZRobot.h: interface for the CSeq_XYZRobot class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SEQ_XYZROBOT_H__D730D533_007E_477C_AD42_11431E3697F1__INCLUDED_)
#define AFX_SEQ_XYZROBOT_H__D730D533_007E_477C_AD42_11431E3697F1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// 규이리 추가 [2015.03.21]
#define MAX_OFFSET_			6

class CSeq_XYZRobot  
{
public:
	CSeq_XYZRobot();
	virtual ~CSeq_XYZRobot();

// Attributes
public:
	bool m_bflag_INIT;
	
	int m_nStep_Init;
	int m_nStep_Run;
	int m_nStep_Rcvy;
	int	m_nStep_BCRRead;
	int	m_nStep_NG_Buff;
	int	m_nStep_NGAccyOut;
	int	m_nStep_ForceDischarge;
	int	m_nStep_BufferConvClear;
	int m_nStep_ReadMove;
	int	m_nStep_WorkSiteChange;
	int m_nStep_AccyVacPick;
	int m_nStep_AccyPlace;
	int m_nStep_BCRBad;
	int m_nStep_BufferConvPlace;
	int	m_nStep_NGBufferPick;
	int m_nStep_NGBufferPlace;


	int m_nStep_GripRetry;

	int  m_nResp_Move;
	int  m_nGripRetryCnt;


	double m_dTargetPos;
	double m_dMaxOffset;
	double m_dPos_Target[3];

	int  m_nGirpSum;
	int  m_nPickRetryCnt;
	int  m_nBCRReadAllBadCount;

	int m_nNGBufferPickSite;



// Attributes
private:
	int  m_nMotX;
	int  m_nMotY;
	int  m_nMotZ;

	int  m_nMotZ0;
	int  m_nMotZ1;
	int  m_nMotZ2;
	int  m_nMotZ3;
	
	int  m_nRetryCount;	// 재시도 횟수
	int  m_nNearby;		// 바코드 읽기 실패 시 [좌/우] 이동하여 재시도 작업 진행 상태

	// [0:체크할 자재유무, 1:실재 자재존재]
	int  m_nPickerUnDn[DATA_TYPE_][MAX_PICKER_];
	int  m_nPicker[DATA_TYPE_][MAX_PICKER_];
	// kilee [2015.03.17]
	int m_nSensor[4];

	bool m_bflag_Picker;
	long m_lWait_Picker[3];

	bool m_bflag_Vac;
	long m_lWait_Vac[3];

	bool m_bflag_Eject;
	long m_lWait_Eject[3];

	long m_lTime_GoesBy[3];
	int  m_nRepickCount;	// 악세사리 집기 작업 재시도 횟수

	bool  m_bPickerPitchFlag;
	DWORD m_lWaitPickerPitch[3];

	long m_nMotflag[3];

	double dAccyLiftPos[4];
	int m_nResp_Lift[4];



// Operations
public:
	void OnSeq_RUN(void);		// 쓰레드 동작
	void OnSeq_INIT(void);		// 쓰레드 관련 변수 초기화
	void OnSeq_Execute(void);	// 쓰레드 제어

// Operations
private:
	void OnRun_Initial();
	void OnRun_Move();
	int OnBCRRead(int nzMode);
	int OnProc_BCRStateBad();



	int OnCheck_NGBufferAccy(int nzOnOff, int nzMode=CHK_EXIST_, int nzSite=SIDE_BOTH_);
	int OnCheck_FrontAccySupply(int nzOnOff, int nzMode, int nzSite); 
	int OnCheck_RearAccySupply(int nzOnOff, int nzMode, int nzSite);
	int OnCheck_MiddleAccySupply(int nzOnOff, int nzMode, int nzSite); 
	int OnCheck_OutFrontAccySupply(int nzOnOff, int nzMode, int nzSite);
	int OnCheck_OutRearAccySupply(int nzOnOff, int nzMode, int nzSite);

	int OnProc_ReadMove();
	int OnProc_WorkSiteChange();
	int OnProc_NGBufferClear();
	int OnProc_AccyOut(int nzNGAccyInfo[4], int nzSite);
	int OnProc_BufferConvClear();
	int OnProc_AccyVacPick();
	int OnProc_AccyPlace();

	int OnProc_BufferPlace();
	int OnProc_4BufferPlace();

	int OnProc_NGBufferPick(int nzSite);
	int OnProc_NGBufferPlace(int nzSite);

	int OnProc_GripRetry();

	void OnSet_PickerPitch( int nOzOff, int nzMode=0);
	int OnGet_PickerPitch( int nOzOff, int nzMode=0);

	
	void OnSet_PickerUpDn(int nzOnOff, int nzPickerInfo[MAX_PICKER_], int nzMode=0);	
	int OnGet_PickerUpDn(int nzOnOff, int nzPickerInfo[MAX_PICKER_], int nzUnDnStatus[MAX_PICKER_], int nzMode=0);

	void OnSet_SolVacuum(int nzOnOff, int nzPickerInfo[MAX_PICKER_], int nzMode=0);	// Vacuum [ON:진공설정, OFF:진공해제]
	int  OnGet_SolVacuum(int nzOnOff, int nzPickerInfo[MAX_PICKER_], int nzVacStatus[MAX_PICKER_], int nzMode=0);

	void OnSet_EjectOnOff(int nzOnOff, int nzPickerInfo[MAX_PICKER_], int nzMode=0);
	int  OnGet_EjectOnOff(int nzOnOff, int nzPickerInfo[MAX_PICKER_], int nzMode=0);

	void OnSet_PickerPitchOpenClose( int nOzOff, int nzMode=0);
	int OnGet_PickerPitchOPenClose( int nOzOff, int nzMode=0);

	int OnRun_ForceDischarge();



};
extern CSeq_XYZRobot	SeqXYZRobot;

#endif // !defined(AFX_SEQ_XYZROBOT_H__D730D533_007E_477C_AD42_11431E3697F1__INCLUDED_)
