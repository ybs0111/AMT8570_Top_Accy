// SerialCommBase.h: interface for the CSerialCommBase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SERIALCOMMBASE_H__F723B6CC_738F_45AC_B02C_79AEFBA09B85__INCLUDED_)
#define AFX_SERIALCOMMBASE_H__F723B6CC_738F_45AC_B02C_79AEFBA09B85__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSerialCommBase  
{
public:
	CSerialCommBase();
	virtual ~CSerialCommBase();

// Attributes
public:
	int m_nPortNum;
	
// Attributes
private:
	int  m_nStep_Bcr;
	long m_lWait_Bcr[3];
	
// Operations
public:
	void OnRun_BarCode();	// [������] ���ڵ� �б� �۾�
	void OnSet_SerialPort(int nzPortNum);	// �ø��� ��Ʈ ��ȣ �Ҵ�

};

#endif // !defined(AFX_SERIALCOMMBASE_H__F723B6CC_738F_45AC_B02C_79AEFBA09B85__INCLUDED_)
