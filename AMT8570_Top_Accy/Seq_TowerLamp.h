// Seq_TowerLamp.h: interface for the CSeq_TowerLamp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SEQ_TOWERLAMP_H__BAB44F24_7FC6_47FB_AC76_11111950AEF5__INCLUDED_)
#define AFX_SEQ_TOWERLAMP_H__BAB44F24_7FC6_47FB_AC76_11111950AEF5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Variable.h"
#include "Public_Function.h"
#include "FastechPublic_IO.h"

class CSeq_TowerLamp  
{
public:
	CSeq_TowerLamp();
	virtual ~CSeq_TowerLamp();


// Attributes
public:
	bool m_bFlagINIT;

	int  m_nLampType;	// 현재 설비 상태 정보 저장
	int  m_nLoopType;	// 현재 루프 상태 정보 [0:짝수번째, 1:홀수번째]
	int  m_nWaitTime;	// 타워 램프의 반짝이는 시간 간격 정보

	// 타워 램프 제어는 변경할 상태에 관계없이 동일한 변수 사용함
	// - 다른 상태로 변경되었어도 기존 상태를 모두 처리한 후 변경된 상태 처리해도 되기 때문임
	int  m_nStep_TLamp;
	long m_lTime_TLamp[3];

	// 화면 변경 시 사용할 변수들
	int  m_nStep_Form;
	long m_lTime_Form[3];
	
// Operations
public:
	void OnSeq_RUN(void);      // 쓰레드 동작
	void OnSeq_INIT(void);     // 쓰레드 관련 변수 초기화
	void OnSeq_Execute(void);  // 쓰레드 제어

	void OnRun_Lamp();  // 타워 램프 상태 제어
	void OnRun_FormChange();

};
extern CSeq_TowerLamp	seqTowerLamp;

#endif // !defined(AFX_SEQ_TOWERLAMP_H__BAB44F24_7FC6_47FB_AC76_11111950AEF5__INCLUDED_)
