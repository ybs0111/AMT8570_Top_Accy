// Seq_MotMove.cpp: implementation of the CSeq_MotMove class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Seq_MotMove.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CSeq_MotMove	seqMotMove;

CSeq_MotMove::CSeq_MotMove()
{

}

CSeq_MotMove::~CSeq_MotMove()
{

}

//==================================================================//
// 쓰레드 동작
//==================================================================//
void CSeq_MotMove::OnSeq_RUN(void) 
{
}

//==================================================================//
// 쓰레드 관련 변수 초기화
//==================================================================//
void CSeq_MotMove::OnSeq_INIT(void) 
{

}

//==================================================================//
// 쓰레드 제어
//==================================================================//
void CSeq_MotMove::OnSeq_Execute(void) 
{
	int i=0;
	for(i=0; i<MOT_MAXMOTOR; i++)
	{
		m_motor[i].MotorMoving_repeat();
		m_motormap[i].MotorLMoving_repeat();
	}
}
