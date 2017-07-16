// CommonFunc.h: interface for the CCommonFunc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMMONFUNC_H__36766F8C_3923_4524_9AAA_D2A7F9C423BA__INCLUDED_)
#define AFX_COMMONFUNC_H__36766F8C_3923_4524_9AAA_D2A7F9C423BA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

extern HWND OnGet_ProgramHwnd();  // 원하는 타이틀명을 갖는 파일의 핸들 검색

extern CString OnGet_CurrentFolder();  // 현재 작업 폴더 반환
extern BOOL OnMake_Directories(LPCTSTR szPath);  // 폴더 생성
extern BOOL OnCreate_Directory(LPCTSTR szPath);  // 폴더 생성
extern void OnRecursiveDelete(CString szPath);  // [폴더+파일] 삭제
extern BOOL OnRemoveDir(CString szPath);  // [폴더+파일] 삭제
extern void OnGet_AppDir(CString& szAppDir);  // 응용 프로그램 폴더 정보 반환
extern CString OnAdd_DirSeparator(CString szFolder);  // 폴더 구분자 추가
extern BOOL OnIsFileExist(LPCTSTR lpszFile, BOOL bIsDirCheck);  // 파일 존재 유무 반환
extern int OnIsFileExist(CString szFile);  // 파일 존재 유무 반환
extern int OnCheck_FileSize(CString szFile, long lzSize, int nzCheck);  // 파일 크기가 일정크기 이상인지 검사
extern CString OnGet_FileDate(CFileFind &find);  // 파일 생성 일자 반환
extern int OnGet_FileCount(LPCTSTR szPath);  // 폴더에 존재하는 파일 갯수 반환
extern void OnChangeExtension(CString szFileName);  // 파일 확장자 변경
extern CString OnAdd_Extension(CString szFileName);  // 파일명에 확장자[.TXT] 추가

// 규이리 추가 [2012.02.22]
extern char OnGet_NtPrivilege();   // 프로세스의 권한 설정
extern char OnGet_OSVersionType();  // 운영 체제 버전 정보 얻기

extern BOOL OnBitCheck(long nzValue, int nzBitPos);  // 특정 위치 BIT 상태 검사
extern void OnSetBitVal(long &npValue, int nzBit, BOOL bzOn);  // 특정 위치 BIT Value 변경
extern CString OnCvt_Binary2String(int nzType, int nzSize, CString szData, int& npDataKind);  // [Binary -> String] 변환
extern CString OnCvt_DWord2Bit(CString szDW);   // [DWORD -> BIT] 변환
extern CString OnCvt_Hex2DWORD(CString szHex);  // 2WORD HEX 값을 LONG으로 변환
extern CString OnCvt_Hex2Decimal(CString szHex);  // [Hex -> Decimal] 변환
extern CString OnCvt_Hex2Ascii(CString szHex);    // [Hex -> Ascii] 변환
extern CString OnCvt_Hex2Long(CString szHex);     // [Hex -> Long] 변환
extern CString OnCvt_Ascii2Hex(CString szAscii);  // [Ascii -> Hex] 변환
extern CString OnCvt_Long2Hex(CString szValue, int nzLen);  // [Long -> Hex] 변환
extern CString OnCvt_Long2HexStr(LONG lzValue, int nzLen);  // [Long -> HexStr] 변환
extern CString OnCvt_Dec2Hex(float fzDecData);  // [10진수 -> 16진수] 변환
extern CString OnCvt_Dec2Hex(int nzDecData);    // [10진수 -> 16진수] 변환

#endif // !defined(AFX_COMMONFUNC_H__36766F8C_3923_4524_9AAA_D2A7F9C423BA__INCLUDED_)
