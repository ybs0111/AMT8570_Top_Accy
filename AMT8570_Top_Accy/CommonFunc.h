// CommonFunc.h: interface for the CCommonFunc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMMONFUNC_H__36766F8C_3923_4524_9AAA_D2A7F9C423BA__INCLUDED_)
#define AFX_COMMONFUNC_H__36766F8C_3923_4524_9AAA_D2A7F9C423BA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

extern HWND OnGet_ProgramHwnd();  // ���ϴ� Ÿ��Ʋ���� ���� ������ �ڵ� �˻�

extern CString OnGet_CurrentFolder();  // ���� �۾� ���� ��ȯ
extern BOOL OnMake_Directories(LPCTSTR szPath);  // ���� ����
extern BOOL OnCreate_Directory(LPCTSTR szPath);  // ���� ����
extern void OnRecursiveDelete(CString szPath);  // [����+����] ����
extern BOOL OnRemoveDir(CString szPath);  // [����+����] ����
extern void OnGet_AppDir(CString& szAppDir);  // ���� ���α׷� ���� ���� ��ȯ
extern CString OnAdd_DirSeparator(CString szFolder);  // ���� ������ �߰�
extern BOOL OnIsFileExist(LPCTSTR lpszFile, BOOL bIsDirCheck);  // ���� ���� ���� ��ȯ
extern int OnIsFileExist(CString szFile);  // ���� ���� ���� ��ȯ
extern int OnCheck_FileSize(CString szFile, long lzSize, int nzCheck);  // ���� ũ�Ⱑ ����ũ�� �̻����� �˻�
extern CString OnGet_FileDate(CFileFind &find);  // ���� ���� ���� ��ȯ
extern int OnGet_FileCount(LPCTSTR szPath);  // ������ �����ϴ� ���� ���� ��ȯ
extern void OnChangeExtension(CString szFileName);  // ���� Ȯ���� ����
extern CString OnAdd_Extension(CString szFileName);  // ���ϸ� Ȯ����[.TXT] �߰�

// ���̸� �߰� [2012.02.22]
extern char OnGet_NtPrivilege();   // ���μ����� ���� ����
extern char OnGet_OSVersionType();  // � ü�� ���� ���� ���

extern BOOL OnBitCheck(long nzValue, int nzBitPos);  // Ư�� ��ġ BIT ���� �˻�
extern void OnSetBitVal(long &npValue, int nzBit, BOOL bzOn);  // Ư�� ��ġ BIT Value ����
extern CString OnCvt_Binary2String(int nzType, int nzSize, CString szData, int& npDataKind);  // [Binary -> String] ��ȯ
extern CString OnCvt_DWord2Bit(CString szDW);   // [DWORD -> BIT] ��ȯ
extern CString OnCvt_Hex2DWORD(CString szHex);  // 2WORD HEX ���� LONG���� ��ȯ
extern CString OnCvt_Hex2Decimal(CString szHex);  // [Hex -> Decimal] ��ȯ
extern CString OnCvt_Hex2Ascii(CString szHex);    // [Hex -> Ascii] ��ȯ
extern CString OnCvt_Hex2Long(CString szHex);     // [Hex -> Long] ��ȯ
extern CString OnCvt_Ascii2Hex(CString szAscii);  // [Ascii -> Hex] ��ȯ
extern CString OnCvt_Long2Hex(CString szValue, int nzLen);  // [Long -> Hex] ��ȯ
extern CString OnCvt_Long2HexStr(LONG lzValue, int nzLen);  // [Long -> HexStr] ��ȯ
extern CString OnCvt_Dec2Hex(float fzDecData);  // [10���� -> 16����] ��ȯ
extern CString OnCvt_Dec2Hex(int nzDecData);    // [10���� -> 16����] ��ȯ

#endif // !defined(AFX_COMMONFUNC_H__36766F8C_3923_4524_9AAA_D2A7F9C423BA__INCLUDED_)
