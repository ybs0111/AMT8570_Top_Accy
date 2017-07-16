// ExternFunctions.h: interface for the CExternFunctions class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EXTERNFUNCTIONS_H__EDD38D02_41AD_4BFA_ABFD_BE2D12A1C615__INCLUDED_)
#define AFX_EXTERNFUNCTIONS_H__EDD38D02_41AD_4BFA_ABFD_BE2D12A1C615__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

extern BOOL _Wait(int nzTime);	// �Է� �ð���ŭ ��� �� ����
extern int _TimeGet();	// ���� �ý��� �ð� ����
extern void _EventSleep(DWORD dwzMilliSec);	// �Է� �ð���ŭ ��� �� ����
extern void _DoEvents();	// ������ �޽��� ó��

extern BOOL _GetSystemPath(CString &spPath);	// �ý��� ���� ���� ����
extern BOOL _GetWindowPath(CString &spPath);	// ������ ���� ���� ����
extern void _GetAppPath(CString& spAppDir);	// ���� ���α׷��� ���� ���� ����

extern CString _GetCurrentFolder();	// ���� �۾� ���� ���� ����
extern BOOL _CreateFolder(LPCTSTR szPath);	// ���� ����
extern void _RecursiveDelete(CString szPath);	// [����+����] ����
extern BOOL _RemoveDir(CString szPath);	// [����+����] ����
extern BOOL _IsFileExist(LPCTSTR lpszFileName, BOOL bIsDirCheck);	// ���� ���� ���� üũ
extern int _IsFileExist(CString szFile);	// ���� ���� ���� üũ
extern int _FileSizeCheck(CString szFile, long lzSize, int nzCheck);	// ������ ���� ũ�� �̻����� ������ ������ �˻�
extern CString _GetFileDate(CFileFind &find);	// ���� ���� ���� ����

extern BOOL _GetBitVal(long nzValue, int nzBitPos);			// Ư�� ��ġ BIT ���� �˻�
extern void _SetBitVal(long &npValue, int nzBit, BOOL bzOn);	// Ư�� ��ġ BIT �� ����

// ���̸� �߰� [2013.12.11]
extern CString _FTMid(CString szData, int nzStartPos, int nzLen);	// ���ϴ� ��ġ�� �� ��ȯ

#endif // !defined(AFX_EXTERNFUNCTIONS_H__EDD38D02_41AD_4BFA_ABFD_BE2D12A1C615__INCLUDED_)
