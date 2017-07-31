// Seq_MotMove.h: interface for the CSeq_MotMove class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SEQ_MOTMOVE_H__521CF8B9_F29C_4D24_8428_B4E829FFD95C__INCLUDED_)
#define AFX_SEQ_MOTMOVE_H__521CF8B9_F29C_4D24_8428_B4E829FFD95C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Variable.h"
#include "SrcBase/AMotor.h"

#define GETMOTOR( motorID )			seqMotMove.GetMotor( motorID )
#define GETMOTORMAP( motormapID )	seqMotMove.GetMotorMap( motormapID )

// �� �κ��� ���� ���� �κа� ��ġ�ؾ� ��
enum ENUM_MOTOR
{
	MOTOR_LIFTER_Z1 =	0,
	MOTOR_LIFTER_Z2,
	MOTOR_LIFTER_Z3,
	MOTOR_LIFTER_Z4,
	MOTOR_LIFTER_Y,
	MOTOR_BCRREAD_ROBOT_Y,
	MOTOR_BCRREAD_ROBOT_Z,
	MOTOR_BCRREAD_ROBOT_X,
	MOTOR_COUNT,
};

class CSeq_MotMove  
{
public:
	CSeq_MotMove();
	virtual ~CSeq_MotMove();

// Attributes
public:
	bool m_bflag_INIT;

// Attributes
protected:
	AMotor	m_motor[MOT_MAXMOTOR];
	AMotor	m_motormap[MOT_MAXMOTOR];
	
// Operations
public:
	void OnSeq_RUN(void);      // ������ ����
	void OnSeq_INIT(void);     // ������ ���� ���� �ʱ�ȭ
	void OnSeq_Execute(void);  // ������ ����

	AMotor& GetMotor( ENUM_MOTOR motorID )	{ return m_motor[motorID]; }
	AMotor& GetMotorMap( int motormapID )	{ return m_motormap[motormapID]; }

};
extern CSeq_MotMove	seqMotMove;

#endif // !defined(AFX_SEQ_MOTMOVE_H__521CF8B9_F29C_4D24_8428_B4E829FFD95C__INCLUDED_)
