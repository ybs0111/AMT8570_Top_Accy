// Seq_BoxLifter.h: interface for the CSeq_BoxLifter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SEQ_BOXLIFTER_H__3D5B48F6_1BB9_4E19_B5E7_664ABBF41F68__INCLUDED_)
#define AFX_SEQ_BOXLIFTER_H__3D5B48F6_1BB9_4E19_B5E7_664ABBF41F68__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WITHOUT_SUPPLY_CNT		10

class CSeq_BoxLifter  
{
public:
	CSeq_BoxLifter();
	virtual ~CSeq_BoxLifter();

// Attributes
public:
	bool m_bflag_INIT;
	
	int  m_nStep_Init;			// BoxLifter �ʱ�ȭ Step
	int  m_nStep_Run;			// BoxLifter Run Step
	int  m_nStep_Remove;			// Box ���� Step
	int  m_nStep_ForceDischarge;	// Box ���� Step
	//kwlee 2017.0093
	long m_lLiftUpCheckTime[4][3];
	// �Ǽ��縮 �������� ���� �߰�
	int  m_nStep_ReAlig;	// Accy ����

	int  m_nStep_Rcvy;			// BoxLifter ���� Step

// Attributes
private:
	double m_dTargetPos[4];

	int  m_nMotY;
	int  m_nMotZ0;
	int  m_nMotZ1;
	int  m_nMotZ2;
	int  m_nMotZ3;

	//kwlee 2017.0825
	int m_WorkSite;

	int  m_nMotZStep[4];
	int  m_nMotZInitStep[4];

	int  m_nInitOK[4];
	int  m_nStandByOK[4];

	int  m_nReq_Rcvy[5];	// ���ͺ� ���� ���� �ʿ� ����
	double m_dPos_TmpRcvyZ[4];	// Z�� ���� ��ġ �ӽ� ����

	double dAccyLiftPos[4];

	double  dCurrPos[4];

// Operations
public:
	void OnSeq_RUN(void);		// ������ ����
	void OnSeq_INIT(void);		// ������ ���� ���� �ʱ�ȭ
	void OnSeq_Execute(void);	// ������ ����

// Operations
private:
	void OnRun_Initial();			// Seq_LoadingConv�� �ʱ�ȭ �۾�
	void OnRun_Move();			// Seq_LoadingConv�� ������ ����
	bool OnRun_Remove();
	int OnRun_ForceDischarge();

	int  OnRcvy_RobotPos();	// [�Ǽ��縮] ���� ������ ��ġ ���� ����

	int OnSupplyStandBy0(int nzMode, int nzSite);
	int OnSupplyInitial0();
	int OnSupplyStandBy1(int nzMode, int nzSite);
	int OnSupplyInitial1();
	int OnSupplyStandBy2(int nzMode, int nzSite);
	int OnSupplyInitial2();
	int OnSupplyStandBy3(int nzMode, int nzSite);
	int OnSupplyInitial3();

	// yangkyu 2015.02.12
	// �Ǽ��縮 �������� ���� �߰�
	int OnProc_ReAlign();		// �Ǽ��縮 ������ �Լ�
};
extern CSeq_BoxLifter	SeqBoxLifter;

#endif // !defined(AFX_SEQ_BOXLIFTER_H__3D5B48F6_1BB9_4E19_B5E7_664ABBF41F68__INCLUDED_)
