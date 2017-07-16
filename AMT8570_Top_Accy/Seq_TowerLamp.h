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

	int  m_nLampType;	// ���� ���� ���� ���� ����
	int  m_nLoopType;	// ���� ���� ���� ���� [0:¦����°, 1:Ȧ����°]
	int  m_nWaitTime;	// Ÿ�� ������ ��¦�̴� �ð� ���� ����

	// Ÿ�� ���� ����� ������ ���¿� ������� ������ ���� �����
	// - �ٸ� ���·� ����Ǿ�� ���� ���¸� ��� ó���� �� ����� ���� ó���ص� �Ǳ� ������
	int  m_nStep_TLamp;
	long m_lTime_TLamp[3];

	// ȭ�� ���� �� ����� ������
	int  m_nStep_Form;
	long m_lTime_Form[3];
	
// Operations
public:
	void OnSeq_RUN(void);      // ������ ����
	void OnSeq_INIT(void);     // ������ ���� ���� �ʱ�ȭ
	void OnSeq_Execute(void);  // ������ ����

	void OnRun_Lamp();  // Ÿ�� ���� ���� ����
	void OnRun_FormChange();

};
extern CSeq_TowerLamp	seqTowerLamp;

#endif // !defined(AFX_SEQ_TOWERLAMP_H__BAB44F24_7FC6_47FB_AC76_11111950AEF5__INCLUDED_)
