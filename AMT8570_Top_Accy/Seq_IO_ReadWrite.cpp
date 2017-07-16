// Seq_IO_ReadWrite.cpp: implementation of the CSeq_IO_ReadWrite class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Seq_IO_ReadWrite.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CSeq_IO_ReadWrite	seqIO_ReadWrite;

CSeq_IO_ReadWrite::CSeq_IO_ReadWrite()
{

}

CSeq_IO_ReadWrite::~CSeq_IO_ReadWrite()
{

}

//==================================================================//
// 쓰레드 동작
//==================================================================//
void CSeq_IO_ReadWrite::OnSeq_RUN(void) 
{
	// I/O 보드가 열리지 않는 경우 강제 리턴
	if (FAS_IO.m_nflag_IOBrdInit != BD_YES)
	{
		return;
	}

	OnSeq_Init();     // 쓰레드 관련 변수 초기화
	OnSeq_Execute();  // 쓰레드 제어
}

//==================================================================//
// 쓰레드 관련 변수 초기화
//==================================================================//
void CSeq_IO_ReadWrite::OnSeq_Init(void) 
{
	if (m_bflag_INIT == true)	{
		return;
	}
	m_bflag_INIT = true;

	OnInit_TotalBoard();	// IO 보드 초기화
}

//==================================================================//
// 쓰레드 제어
// - IO 상태를 읽어서 특정 변수에 IO 상태 설정함
// : 셋팅 시에는 이전 상태와 현재 입력 상태 비교해서 다르면 출력을 내보내게 됨
// - 설비별로 이 부분은 수정해야 함
//==================================================================//
void CSeq_IO_ReadWrite::OnSeq_Execute(void) 
{
	// 규이리 위치 이동함 [2014.08.27]
	/*
	// I/O 보드가 열리지 않는 경우 강제 리턴
	if (FAS_IO.m_nflag_IOBrdInit != BD_YES)
	{
		return;
	}
	*/

	// 아래 부분은 설비별로 수정이 필요
	// - 설비별로 사용하는 IO 보드 갯수가 다르기 때문임
	FAS_IO.IO_ReadWrite(0, 0, 0, 1);
	FAS_IO.IO_ReadWrite(0, 0, 1, 1);
	FAS_IO.IO_ReadWrite(0, 0, 2, 1);
	FAS_IO.IO_ReadWrite(0, 0, 3, 1);
	FAS_IO.IO_ReadWrite(0, 0, 4, 1);
	FAS_IO.IO_ReadWrite(0, 0, 5, 1);
	FAS_IO.IO_ReadWrite(0, 0, 6, 1);
	FAS_IO.IO_ReadWrite(0, 0, 7, 1);
	
	FAS_IO.IO_ReadWrite(0, 1, 0, 1);
	FAS_IO.IO_ReadWrite(0, 1, 1, 1);
	FAS_IO.IO_ReadWrite(0, 1, 2, 1);
	FAS_IO.IO_ReadWrite(0, 1, 3, 1);
	FAS_IO.IO_ReadWrite(0, 1, 4, 1);
	FAS_IO.IO_ReadWrite(0, 1, 5, 1);
	FAS_IO.IO_ReadWrite(0, 1, 6, 1);
	FAS_IO.IO_ReadWrite(0, 1, 7, 1);
	
	FAS_IO.IO_ReadWrite(0, 2, 0, 1);
	FAS_IO.IO_ReadWrite(0, 2, 1, 1);
	FAS_IO.IO_ReadWrite(0, 2, 2, 1);
	FAS_IO.IO_ReadWrite(0, 2, 3, 1);
	FAS_IO.IO_ReadWrite(0, 2, 4, 1);
	FAS_IO.IO_ReadWrite(0, 2, 5, 1);
	FAS_IO.IO_ReadWrite(0, 2, 6, 1);
	FAS_IO.IO_ReadWrite(0, 2, 7, 1);
	
	FAS_IO.IO_ReadWrite(0, 3, 0, 1);
	FAS_IO.IO_ReadWrite(0, 3, 1, 1);
}

//==================================================================//
// IO 보드 초기화
//==================================================================//
void CSeq_IO_ReadWrite::OnInit_TotalBoard() 
{
	//FAS_IO.IO_ReadWrite(0, 0, 0, -1);	// 0번 보드 초기화

	FAS_IO.IO_ReadWrite(0, 0, 0, 0);
	FAS_IO.IO_ReadWrite(0, 0, 1, 0);
	FAS_IO.IO_ReadWrite(0, 0, 2, 0);
	FAS_IO.IO_ReadWrite(0, 0, 3, 0);
	FAS_IO.IO_ReadWrite(0, 0, 4, 0);
	FAS_IO.IO_ReadWrite(0, 0, 5, 0);
	FAS_IO.IO_ReadWrite(0, 0, 6, 0);
	FAS_IO.IO_ReadWrite(0, 0, 7, 0);
	
	FAS_IO.IO_ReadWrite(0, 1, 0, 0);
	FAS_IO.IO_ReadWrite(0, 1, 1, 0);
	FAS_IO.IO_ReadWrite(0, 1, 2, 0);
	FAS_IO.IO_ReadWrite(0, 1, 3, 0);
	FAS_IO.IO_ReadWrite(0, 1, 4, 0);
	FAS_IO.IO_ReadWrite(0, 1, 5, 0);
	FAS_IO.IO_ReadWrite(0, 1, 6, 0);
	FAS_IO.IO_ReadWrite(0, 1, 7, 0);
	
	FAS_IO.IO_ReadWrite(0, 2, 0, 0);
	FAS_IO.IO_ReadWrite(0, 2, 1, 0);
	FAS_IO.IO_ReadWrite(0, 2, 2, 0);
	FAS_IO.IO_ReadWrite(0, 2, 3, 0);
	FAS_IO.IO_ReadWrite(0, 2, 4, 0);
	FAS_IO.IO_ReadWrite(0, 2, 5, 0);
	FAS_IO.IO_ReadWrite(0, 2, 6, 0);
	FAS_IO.IO_ReadWrite(0, 2, 7, 0);
	
	FAS_IO.IO_ReadWrite(0, 3, 0, 0);
	FAS_IO.IO_ReadWrite(0, 3, 1, 0);
}
