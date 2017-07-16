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
// ������ ����
//==================================================================//
void CSeq_IO_ReadWrite::OnSeq_RUN(void) 
{
	// I/O ���尡 ������ �ʴ� ��� ���� ����
	if (FAS_IO.m_nflag_IOBrdInit != BD_YES)
	{
		return;
	}

	OnSeq_Init();     // ������ ���� ���� �ʱ�ȭ
	OnSeq_Execute();  // ������ ����
}

//==================================================================//
// ������ ���� ���� �ʱ�ȭ
//==================================================================//
void CSeq_IO_ReadWrite::OnSeq_Init(void) 
{
	if (m_bflag_INIT == true)	{
		return;
	}
	m_bflag_INIT = true;

	OnInit_TotalBoard();	// IO ���� �ʱ�ȭ
}

//==================================================================//
// ������ ����
// - IO ���¸� �о Ư�� ������ IO ���� ������
// : ���� �ÿ��� ���� ���¿� ���� �Է� ���� ���ؼ� �ٸ��� ����� �������� ��
// - ���񺰷� �� �κ��� �����ؾ� ��
//==================================================================//
void CSeq_IO_ReadWrite::OnSeq_Execute(void) 
{
	// ���̸� ��ġ �̵��� [2014.08.27]
	/*
	// I/O ���尡 ������ �ʴ� ��� ���� ����
	if (FAS_IO.m_nflag_IOBrdInit != BD_YES)
	{
		return;
	}
	*/

	// �Ʒ� �κ��� ���񺰷� ������ �ʿ�
	// - ���񺰷� ����ϴ� IO ���� ������ �ٸ��� ������
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
// IO ���� �ʱ�ȭ
//==================================================================//
void CSeq_IO_ReadWrite::OnInit_TotalBoard() 
{
	//FAS_IO.IO_ReadWrite(0, 0, 0, -1);	// 0�� ���� �ʱ�ȭ

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
