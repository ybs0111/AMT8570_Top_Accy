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
	long m_lWait_Response[3];	// 소켓 통신 대기 시간

	int  m_nStep_StatusAlarm;

	int  m_nStep_Auto;
	long m_lWait_Auto[3];
	int  m_nStep_Manual;
	long m_lWait_Manual[3];

	// 규이리 추가 [2014.12.16]
	int  m_nLockStep;
	long m_lLockWaitTime[3];
	int  m_nDoorOpenTry;
	
// Operations
public:
	void OnSeq_RUN(void);      // 쓰레드 동작
	void OnSeq_Init(void);     // 쓰레드 관련 변수 초기화
	void OnSeq_Execute(void);  // 쓰레드 제어

	void OnPush_Start();	// START 스위치 눌림 처리
	void OnPush_Stop();		// STOP 스위치 눌림 처리
	void OnPush_AlarmClear();	// 알람 해제 스위치 눌림 처리
	void OnPush_BuzzerOff();	// BUZZER OFF 스위치 눌림 처리
	void OnPush_ManualLoading();	// ManualLoading 스위치 눌림 처리
	void OnPush_LoadingStart();	// LoadingStart 스위치 눌림 처리

	int  OnCheckSwitchRunCondition();  // 스위치 버튼 동작 조건 검사
	void OnRest_SwitchStep(int nzStatus);	// 버튼 눌림 처리 스텝 정보 초기화

	int OnReport_2MainEqpStatus(int nzStatus);	// [EQP<->MAIN] 설비 상태 보고
	void OnReport_2MainStatusAlarm();	// 설비 상태 보고 [ALARM]
	
	void OnTurn_Auto();		// [Auto] 상태로 스위치 돌린 경우 처리
	void OnTurn_Manual();	// [Manual] 상태로 스위치 돌린 경우 처리

	// 규이리 추가 [2014.12.17]
	void OnRun_DoorLock();	// 모든 DOOR 닫힌 경우 강제로 DOOR LOCK 처리

};
extern CSeq_LampSwitch	seqSwitch;

#endif // !defined(AFX_SEQ_LAMPSWITCH_H__69A1716A_2404_4A3C_98E9_B16D3F602CA7__INCLUDED_)
