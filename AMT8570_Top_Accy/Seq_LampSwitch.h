// Seq_LampSwitch.h: interface for the CSeq_LampSwitch class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SEQ_LAMPSWITCH_H__69A1716A_2404_4A3C_98E9_B16D3F602CA7__INCLUDED_)
#define AFX_SEQ_LAMPSWITCH_H__69A1716A_2404_4A3C_98E9_B16D3F602CA7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Variable.h"
#include "Public_Function.h"
#include "FastechPublic_IO.h"

class CSeq_LampSwitch  
{
public:
	CSeq_LampSwitch();
	virtual ~CSeq_LampSwitch();

// Attributes
public:
	bool m_bFlagINIT;

	int  m_nStep_Start;
	int  m_nStep_Stop;
	int  m_nStep_Reset;
	int  m_nStep_Buzz;
	int	 m_nStep_LoadingStart;
	int  m_nStep_ManualLoading;

	long m_lWaitTimeSW[3];

	int  m_nStep_EqpStatus;
	long m_lWait_Response[3];	// ���� ��� ��� �ð�

	int  m_nStep_StatusAlarm;

	int  m_nStep_Auto;
	long m_lWait_Auto[3];
	int  m_nStep_Manual;
	long m_lWait_Manual[3];

	// ���̸� �߰� [2014.12.16]
	int  m_nLockStep;
	long m_lLockWaitTime[3];
	int  m_nDoorOpenTry;
	
// Operations
public:
	void OnSeq_RUN(void);      // ������ ����
	void OnSeq_Init(void);     // ������ ���� ���� �ʱ�ȭ
	void OnSeq_Execute(void);  // ������ ����

	void OnPush_Start();	// START ����ġ ���� ó��
	void OnPush_Stop();		// STOP ����ġ ���� ó��
	void OnPush_AlarmClear();	// �˶� ���� ����ġ ���� ó��
	void OnPush_BuzzerOff();	// BUZZER OFF ����ġ ���� ó��
	void OnPush_ManualLoading();	// ManualLoading ����ġ ���� ó��
	void OnPush_LoadingStart();	// LoadingStart ����ġ ���� ó��

	int  OnCheckSwitchRunCondition();  // ����ġ ��ư ���� ���� �˻�
	void OnRest_SwitchStep(int nzStatus);	// ��ư ���� ó�� ���� ���� �ʱ�ȭ

	int OnReport_2MainEqpStatus(int nzStatus);	// [EQP<->MAIN] ���� ���� ����
	void OnReport_2MainStatusAlarm();	// ���� ���� ���� [ALARM]
	
	void OnTurn_Auto();		// [Auto] ���·� ����ġ ���� ��� ó��
	void OnTurn_Manual();	// [Manual] ���·� ����ġ ���� ��� ó��

	// ���̸� �߰� [2014.12.17]
	void OnRun_DoorLock();	// ��� DOOR ���� ��� ������ DOOR LOCK ó��

};
extern CSeq_LampSwitch	seqSwitch;

#endif // !defined(AFX_SEQ_LAMPSWITCH_H__69A1716A_2404_4A3C_98E9_B16D3F602CA7__INCLUDED_)
