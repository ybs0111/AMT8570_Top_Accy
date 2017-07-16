// ExternFunctions.h: interface for the CExternFunctions class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EXTERNFUNCTIONS_H__EDD38D02_41AD_4BFA_ABFD_BE2D12A1C615__INCLUDED_)
#define AFX_EXTERNFUNCTIONS_H__EDD38D02_41AD_4BFA_ABFD_BE2D12A1C615__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

extern BOOL _Wait(int nzTime);	// 입력 시간만큼 대기 후 리턴
extern int _TimeGet();	// 현재 시스템 시간 리턴
extern void _EventSleep(DWORD dwzMilliSec);	// 입력 시간만큼 대기 후 리턴
extern void _DoEvents();	// 윈도우 메시지 처리

extern BOOL _GetSystemPath(CString &spPath);	// 시스템 폴더 정보 리턴
extern BOOL _GetWindowPath(CString &spPath);	// 윈도우 폴더 정보 리턴
extern void _GetAppPath(CString& spAppDir);	// 응용 프로그램의 폴더 정보 리턴

extern CString _GetCurrentFolder();	// 현재 작업 폴더 정보 리턴
extern BOOL _CreateFolder(LPCTSTR szPath);	// 폴더 생성
extern void _RecursiveDelete(CString szPath);	// [폴더+파일] 삭제
extern BOOL _RemoveDir(CString szPath);	// [폴더+파일] 삭제
extern BOOL _IsFileExist(LPCTSTR lpszFileName, BOOL bIsDirCheck);	// 파일 존재 여부 체크
extern int _IsFileExist(CString szFile);	// 파일 존재 여부 체크
extern int _FileSizeCheck(CString szFile, long lzSize, int nzCheck);	// 파일이 일정 크기 이상인지 파일의 사이즈 검사
extern CString _GetFileDate(CFileFind &find);	// 파일 생성 일자 리턴

extern BOOL _GetBitVal(long nzValue, int nzBitPos);			// 특정 위치 BIT 상태 검사
extern void _SetBitVal(long &npValue, int nzBit, BOOL bzOn);	// 특정 위치 BIT 값 변경

// 규이리 추가 [2013.12.11]
extern CString _FTMid(CString szData, int nzStartPos, int nzLen);	// 원하는 위치의 값 반환

#endif // !defined(AFX_EXTERNFUNCTIONS_H__EDD38D02_41AD_4BFA_ABFD_BE2D12A1C615__INCLUDED_)
