// ExternFunctions.cpp: implementation of the CExternFunctions class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ExternFunctions.h"

// 파일 존재 유무 검사 함수 호출하기 위해서는 반드시 INCLUDE 필요
#include "io.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//==================================================================//
// 입력 시간만큼 대기 후 리턴
//==================================================================//
BOOL _Wait(int nzTime) 
{
	if (nzTime<0)	{
		nzTime = 0;
	}

	long lTime10	= nzTime/10;
	long lTime1		= nzTime%10;

	int nCNT=0;
	for(nCNT=0; nCNT<lTime10; nCNT++)
	{
		Sleep(10);
		_DoEvents();
	}
	Sleep(lTime1);

	return TRUE;
}

//==================================================================//
// 현재 시스템 시간 리턴
//==================================================================//
int _TimeGet() 
{
	return (int)(GetTickCount()&0x7FFFFFFF);
}

//==================================================================//
// 입력 시간만큼 대기 후 리턴
//==================================================================//
void _EventSleep(DWORD dwzMilliSec) 
{
	DWORD dwWaitSec = dwzMilliSec;
	if (dwWaitSec<0)	{
		dwWaitSec = 0;
	}

	DWORD dwTimeDiff = 0L;
	DWORD dwStart = GetTickCount();
	MSG msg;

	while(dwTimeDiff<dwWaitSec)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else	{
			Sleep(10);
		}

		DWORD dwCurr = GetTickCount();
		if (dwCurr<dwStart)
		{
			dwTimeDiff = ULONG_MAX - (dwStart+dwCurr);  // ULONG_MAX 상수값은 = 4294967295
		}
		else
		{
			dwTimeDiff = dwCurr-dwStart;
		}
	}
}

//==================================================================//
// 윈도우 메시지 처리
//==================================================================//
void _DoEvents() 
{
	MSG msg;

	// 윈도우 메시지 처리
	// - PeekMessage() : 메시지를 위한 쓰레드의 메시지 큐를 점검
	//                 : 메시지가 있다면 NonZero 반환
	//                 : 메시지가 없다면 Zero를 반환
	// : PM_REMOVE = 처리후에 큐에서 메시지 제거
	while(PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
	{
		// 메시지 존재 시 처리
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}



//==================================================================//
// 시스템 폴더 정보 리턴
//==================================================================//
BOOL _GetSystemPath(CString &spPath) 
{
	char chPath[256] = {0,};
	
	UINT unRet = GetSystemDirectory(chPath, 256);
	spPath.Format("%s\\", chPath);
	
	return (unRet>0) ? TRUE:FALSE;
}

//==================================================================//
// 윈도우 폴더 정보 리턴
//==================================================================//
BOOL _GetWindowPath(CString &spPath) 
{
	char chPath[256] = {0,};
	
	UINT unRet = GetWindowsDirectory(chPath, 256);
	spPath.Format("%s\\", chPath);
	
	return (unRet>0) ? TRUE:FALSE;
}

//==================================================================//
// 응용 프로그램의 폴더 정보 리턴
//==================================================================//
void _GetAppPath(CString& spAppDir) 
{
	TCHAR tcFullPath[MAX_PATH];
	TCHAR tcDir[_MAX_DIR];
	TCHAR tcDrv[_MAX_DRIVE];
	
	// App 전체 경로 정보 얻음
	::GetModuleFileName(NULL, tcFullPath, MAX_PATH);
	
	// 드라이브, 폴더 등의 정보로 세분화
	_splitpath(tcFullPath, tcDrv, tcDir, NULL, NULL);
	spAppDir.Format(_T("%s%s"), tcDrv, tcDir);
}



//==================================================================//
// 현재 작업 폴더 정보 리턴
//==================================================================//
CString _GetCurrentFolder() 
{
	/*
	char path[MAX_PATH] = {0};
	GetCurrentDirectory(MAX_PATH, path);  // 현재 작업 폴더
	*/
	char chBuff[256];
	TCHAR drive[_MAX_DRIVE], dir[_MAX_DIR], fname[_MAX_FNAME], ext[_MAX_EXT];
	GetModuleFileName(NULL, chBuff, 255);
	_tsplitpath(chBuff, drive, dir, fname, ext);

	CString sPath = drive;
	sPath += dir;
	int nPos = sPath.Find("Debug");
	if (nPos >= 0)	{
		sPath = sPath.Mid(0,nPos);
	}

	if (sPath.Right(1) != "\\")
	{
		sPath += "\\";
	}
	return sPath;
}

//==================================================================//
// 폴더 생성
//==================================================================//
BOOL _CreateFolder(LPCTSTR szPath) 
{
	int iRet = 0;
	CString sPath = szPath;
	CString sTemp;

	do{
		iRet = sPath.Find('\\', iRet+1);
		if (iRet>0)
		{
			sTemp = sPath.Left(iRet+1);
			if (_access(sTemp.operator LPCTSTR(), 00) == -1)
			{
				CreateDirectory(sTemp, NULL);
			}
		}
    }while(iRet != -1);

	return iRet;
}

//==================================================================//
// [폴더+파일] 삭제
// - 맨 상위 폴더는 삭제되지 않음
//==================================================================//
void _RecursiveDelete(CString szPath) 
{
	CFileFind fFile;

	int nPos=0;
	CString sPath = szPath;
	if (sPath.Right(1) != "\\")
	{
		sPath += "\\";
	}
	sPath += "*.*";

	BOOL bRes = fFile.FindFile(sPath);
	while(bRes)
	{
		bRes = fFile.FindNextFile();
		if (!fFile.IsDots() &&
			!fFile.IsDirectory())
		{
			DeleteFile(fFile.GetFilePath());
		}
		else if (fFile.IsDirectory() &&
			!fFile.IsDots())
		{
			sPath = fFile.GetFilePath();
			_RecursiveDelete(sPath);

			RemoveDirectory(sPath);
		}
		else if (fFile.IsDirectory())
		{
			sPath = fFile.GetFilePath();
			nPos = sPath.ReverseFind('\\');
			if (nPos != -1)	{
				sPath = sPath.Mid(0,nPos);
			}
			RemoveDirectory(sPath);
		}
	}
}

//==================================================================//
// [폴더+파일] 삭제
//==================================================================//
BOOL _RemoveDir(CString szPath) 
{
	char chFolder[256] = {0,};
	sprintf(chFolder, "%s", szPath);

	SHFILEOPSTRUCT FileOp = {0};

	char chTemp[MAX_PATH];
	strcpy(chTemp, chFolder);
	chTemp[strlen(chFolder)+1] = NULL;

	FileOp.hwnd		= NULL;
	FileOp.wFunc	= FO_DELETE;  // Property Delete
	FileOp.pTo		= NULL;
	FileOp.fFlags	= FOF_SILENT | FOF_NOCONFIRMATION;  // FOF_NOCONFIRMATION | FOF_NOERRORUI;  // 확인 메시지 미출력
	FileOp.fAnyOperationsAborted = false;
	FileOp.hNameMappings		= NULL;
	FileOp.lpszProgressTitle	= chTemp;
	FileOp.pFrom	= chTemp;

	// 삭제 완료
	if (SHFileOperation(&FileOp) == 0)
	{
		return TRUE;
	}
	return FALSE;
}

//==================================================================//
// 파일 존재 여부 체크
//==================================================================//
BOOL _IsFileExist(LPCTSTR lpszFileName, BOOL bIsDirCheck) 
{
	DWORD dwAttributes = GetFileAttributes(lpszFileName);
	if (dwAttributes == 0xFFFFFFFF)
	{
		return FALSE;
	}

	if ((dwAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY)
	{
		if (bIsDirCheck)
		{
			return TRUE;
		}
		else	{
			return FALSE;
		}
	}
	else
	{
		if (!bIsDirCheck)
		{
			return TRUE;
		}
		else	{
			return FALSE;
		}
	}
}

//==================================================================//
// 파일 존재 여부 체크
//==================================================================//
int _IsFileExist(CString szFile) 
{
	char chFile[256];
	sprintf(chFile, "%s", szFile);
	
	int nExist = access(chFile, 0);
	if (nExist == -1)
	{
		return -1;
	}
	return 1;
}

//==================================================================//
// 파일이 일정 크기 이상인지 파일의 사이즈 검사
// long lzSize : 파일 크기 정보 [예) 1000000]
//==================================================================//
int _FileSizeCheck(CString szFile, long lzSize, int nzCheck) 
{
	long lLength=0, lFlag=0;
	BOOL bContinue;

	CFileFind finder;

	if (bContinue = finder.FindFile(szFile))
	{
		if (nzCheck == 1)
		{
			finder.FindFile(szFile);
			finder.FindNextFile();
			finder.GetFileName();

			lLength = finder.GetLength();
			if (lLength > lzSize)
			{
				lFlag=1;
			}

			finder.Close();
		}
		else
		{
			finder.FindFile(szFile);
			finder.FindNextFile();
			finder.GetFileName();

			lLength	= finder.GetLength();
			lFlag	= lLength;

			finder.Close();
		}
	}
	return lFlag;
}

//==================================================================//
// 파일 생성 일자 리턴
//==================================================================//
CString _GetFileDate(CFileFind &find) 
{
	CString sResult = "";
	CTime time = CTime::GetCurrentTime();

	find.GetLastWriteTime(time);

	CTimeSpan timeSpan = CTime::GetCurrentTime() - time;
	if (timeSpan.GetDays() > 356)
	{
		sResult = time.Format(" %b %d %Y ");
	}
	else	{
		sResult.Format(" %s %02d:%02d ", time.Format("%b %d"), time.GetHour(), time.GetMinute());
	}
	return sResult;
}

//==================================================================//
// 특정 위치 BIT 상태 검사
//==================================================================//
BOOL _GetBitVal(long nzValue, int nzBitPos) 
{
	// 마스크 BIT 설정
	unsigned long lMask = 0x0001;
	// 변경할 BIT 위치로 이동 [SHIFT]
	lMask <<= nzBitPos;

	return(nzValue & lMask);
}

//==================================================================//
// 특정 위치 BIT 값 변경
//==================================================================//
void _SetBitVal(long &npValue, int nzBit, BOOL bzOn) 
{
	unsigned long lMask = 0x0001;
	lMask <<= nzBit;

	if (bzOn)
	{
		npValue |= lMask;
	}
	else	{
		npValue &= ~lMask;
	}
}

//==================================================================//
// 원하는 위치의 값 반환
// 규이리 추가 [2013.12.11]
//==================================================================//
CString _FTMid(CString szData, int nzStartPos, int nzLen) 
{
	CString sTmp = szData;
	// 입력된 문자열의 전체 길이
	int nLen = sTmp.GetLength();
	// 파싱하기 위해 필요한 최소 문자열 길이
	int nWantedLen = nzStartPos + nzLen;
	
	// 원본 문자열이 잘라낼 위치보다 큰 경우
	CString sRetVal;
	sRetVal.Empty();
	
	// 문자열 찾을 시작 위치가 전체 문자열 길이보다 큰 경우
	// 찾을 수 있는 조건이 아니므로 강제로 공백 리턴시킴
	if (nzStartPos > nLen)
	{
		return sRetVal;
	}
	
	if (nLen >= nWantedLen)
	{
		sRetVal = sTmp.Mid(nzStartPos,nzLen);
	}
	else	{
		sRetVal = sTmp.Mid(nzStartPos);
	}
	sRetVal.TrimLeft(' ');
	sRetVal.TrimRight(' ');
	
	return sRetVal;
}
