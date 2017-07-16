// Seq_IO_ReadWrite.h: interface for the CSeq_IO_ReadWrite class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SEQ_IO_READWRITE_H__2A0B2912_3AFE_482F_AD66_7C44C49F487E__INCLUDED_)
#define AFX_SEQ_IO_READWRITE_H__2A0B2912_3AFE_482F_AD66_7C44C49F487E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Variable.h"
#include "FastechPublic_IO.h"

class CSeq_IO_ReadWrite  
{
public:
	CSeq_IO_ReadWrite();
	virtual ~CSeq_IO_ReadWrite();

// Attributes
public:
	bool m_bflag_INIT;
	
// Operations
public:
	void OnSeq_RUN(void);      // ������ ����
	void OnSeq_Init(void);     // ������ ���� ���� �ʱ�ȭ
	void OnSeq_Execute(void);  // ������ ����

	void OnInit_TotalBoard();	// IO ���� �ʱ�ȭ

};
extern CSeq_IO_ReadWrite	seqIO_ReadWrite;

#endif // !defined(AFX_SEQ_IO_READWRITE_H__2A0B2912_3AFE_482F_AD66_7C44C49F487E__INCLUDED_)
