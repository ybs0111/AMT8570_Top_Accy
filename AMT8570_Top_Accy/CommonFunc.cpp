// CommonFunc.cpp: implementation of the CCommonFunc class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CommonFunc.h"

//==================================================================//
// 외부 클래스 추가
//==================================================================//
#include "Variable.h"

#include <math.h>
#include "io.h"
//------------------------------------------------------------------//

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


//==================================================================//
// 원하는 타이틀명을 갖는 파일의 핸들 검색
//==================================================================//
HWND OnGet_ProgramHwnd() 
{
	CWnd* hFindWnd = AfxGetMainWnd()->FindWindow(NULL, NULL);
	
	while(hFindWnd != NULL)
	{
		if (hFindWnd->GetParent() == NULL)
		{
			CString sWindowName =_T("");
			hFindWnd->GetWindowText(sWindowName);
			
			if (sWindowName.Find("[AMT] CIM") != -1)
			{
				// 프로세스 핸들 정보 리턴
				return hFindWnd->m_hWnd;
			}
		}
		hFindWnd = hFindWnd->GetNextWindow();
	}
	return NULL;
}

//==================================================================//
// 현재 작업 폴더 반환
//==================================================================//
CString OnGet_CurrentFolder() 
{
	/*
	char path[MAX_PATH] = {0};
	GetCurrentDirectory(MAX_PATH, path);  // 현재 작업 폴더
	*/
	char buf[256];
	TCHAR drive[_MAX_DRIVE], dir[_MAX_DIR], fname[_MAX_FNAME], ext[_MAX_EXT];
	GetModuleFileName(NULL, buf, 255);
	_tsplitpath(buf, drive, dir, fname, ext);

	CString sPath = drive;
	sPath += dir;
	int nPos = sPath.Find("Debug");
	if (nPos >= 0)	{
		sPath = sPath.Mid(0,nPos);
	}
	if (sPath.Right(1) != _T("\\"))	{
		sPath += _T("\\");
	}
	return sPath;
}

//==================================================================//
// 폴더 생성
//==================================================================//
BOOL OnMake_Directories(LPCTSTR szPath) 
{
	int iRet = 0;
	CString sPath = szPath;
	CString sTemp;
	
	do{
		iRet = sPath.Find('\\', iRet+1);
		if (iRet > 0)
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
// 폴더 생성
//==================================================================//
BOOL OnCreate_Directory(LPCTSTR szPath) 
{
	BOOL bRet = FALSE;
	CString sDirectory = szPath;

	int nIndex = sDirectory.ReverseFind('\\');
	if (nIndex != -1)	{
		sDirectory = sDirectory.Left(nIndex+1) + "*.*";
	}

	CFileFind finder;
	if (!finder.FindFile(sDirectory))
	{
		int nPos=0;
		nPos = sDirectory.Find(TCHAR(':'));
		if (nPos>0)	{
			nPos += 2;
		}
		else	{
			nPos=0;
		}

		do{
			nPos = sDirectory.Find(TCHAR('\\'), nPos);
			if (nPos>0)
			{
				CString sCurrent = sDirectory.Left(nPos);
				CreateDirectory(sCurrent, NULL);
				nPos = sCurrent.GetLength() + 1;
			}
		}while(nPos != -1);
		bRet = TRUE;
	}
	return bRet;
}

//==================================================================//
// [폴더+파일] 삭제
// : 맨 상위 폴더는 삭제되지 않는다
//==================================================================//
void OnRecursiveDelete(CString szPath) 
{
	int nPos=0;
	CString sPath = szPath;

	if (sPath.Right(1) != _T("\\"))	{
		sPath += _T("\\");
	}
	sPath += _T("*.*");

	CFileFind fFile;
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
			OnRecursiveDelete(sPath);
			RemoveDirectory(sPath);
		}
		else if (fFile.IsDirectory())
		{
			sPath = fFile.GetFilePath();
			nPos = sPath.ReverseFind('\\');
			if (nPos != -1)	{
				sPath = sPath.Mid(0, nPos);
			}
			RemoveDirectory(sPath);
		}
	}
}

//==================================================================//
// [폴더+파일] 삭제
//==================================================================//
BOOL OnRemoveDir(CString szPath) 
{
	char chFolder[256] = {0,};
	sprintf(chFolder, "%s", szPath);

	SHFILEOPSTRUCT  FileOp = {0};

	char chTemp[MAX_PATH];
	strcpy(chTemp, chFolder);
	chTemp[strlen(chFolder)+1] = NULL;

	FileOp.hwnd   = NULL;
	FileOp.wFunc  = FO_DELETE;  // Property Delete 
	FileOp.pTo    = NULL;
	FileOp.fFlags = FOF_SILENT | FOF_NOCONFIRMATION;//FOF_NOCONFIRMATION | FOF_NOERRORUI;  // 확인 메시지 미출력 
	FileOp.fAnyOperationsAborted = false;
	FileOp.hNameMappings     = NULL;
	FileOp.lpszProgressTitle = chTemp;
	FileOp.pFrom  = chTemp;

	if (SHFileOperation(&FileOp)==0)
	{
		// Delete OK
		return TRUE;
	}
	return FALSE;
}

//==================================================================//
// 응용 프로그램 폴더 정보 반환
//==================================================================//
void OnGet_AppDir(CString& szAppDir) 
{
	TCHAR szFullPath[MAX_PATH];
	TCHAR szDir[_MAX_DIR];
	TCHAR szDrive[_MAX_DRIVE];

	// App 전체 경로 정보 얻는다
	::GetModuleFileName(NULL, szFullPath, MAX_PATH);

	// 드라이브, 폴더 등의 정보로 세분화
	_splitpath(szFullPath, szDrive, szDir, NULL, NULL);

	szAppDir.Format(_T("%s%s"), szDrive, szDir);
}

//==================================================================//
// 폴더 구분자 추가
//==================================================================//
CString OnAdd_DirSeparator(CString szFolder) 
{
	CString sDIR = szFolder;
	if (sDIR.Right(1) != _T("\\"))
	{
		sDIR += _T("\\");
	}
	return sDIR;
}

//==================================================================//
// 파일 존재 유무 반환
//==================================================================//
BOOL OnIsFileExist(LPCTSTR lpszFile, BOOL bIsDirCheck) 
{
	DWORD dwAttributes = GetFileAttributes(lpszFile);
	if (dwAttributes == 0xFFFFFFFF)	{
		return FALSE;
	}

	if ((dwAttributes &
		FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY)
	{
		if (bIsDirCheck)	{
			return TRUE;
		}
		else	{
			return FALSE;
		}
	}
	else
	{
		if (!bIsDirCheck)	{
			return TRUE;
		}
		else	{
			return FALSE;
		}
	}
}

//==================================================================//
// 파일 존재 유무 반환
//==================================================================//
int OnIsFileExist(CString szFile) 
{
	char chFile[256];
	sprintf(chFile, "%s", szFile);
	int nExist = access(chFile, 0);
	if (nExist == -1)	{
		return -1;
	}
	return 1;
}

//==================================================================//
// 파일 크기가 일정크기 이상인지 검사
//==================================================================//
int OnCheck_FileSize(CString szFile, long lzSize, int nzCheck) 
{
	long lLength=0, lFlag=0;
	BOOL bContinue;

	CFileFind  finder;
	if (bContinue = finder.FindFile(szFile))
	{
		if (nzCheck == 1)
		{
			finder.FindFile(szFile);
			finder.FindNextFile();
			finder.GetFileName();

			lLength = finder.GetLength();
			if (lLength > lzSize)	{
				lFlag=1;
			}

			finder.Close();
		}
		else
		{
			finder.FindFile(szFile);
			finder.FindNextFile();
			finder.GetFileName();

			lLength = finder.GetLength();
			lFlag = lLength;

			finder.Close();
		}
	}
	return lFlag;
}

//==================================================================//
// 파일 생성 일자 반환
//==================================================================//
CString OnGet_FileDate(CFileFind &find) 
{
	CString sResult = _T("");
	CTime time = CTime::GetCurrentTime();
	find.GetLastWriteTime(time);

	CTimeSpan timeSpan = CTime::GetCurrentTime() - time;
	if (timeSpan.GetDays() > 356)	{
		sResult = time.Format(" %b %d %Y ");
	}
	else	{
		sResult.Format(" %s %02d:%02d ", time.Format("%b %d"), time.GetHour(), time.GetMinute());
	}
	return sResult;
}

//==================================================================//
// 폴더에 존재하는 파일 갯수 반환
//==================================================================//
int OnGet_FileCount(LPCTSTR szPath) 
{
	int nCount=0;
	CString sPath = szPath;

	HANDLE hFind;
	WIN32_FIND_DATA fd;

	if (sPath.Right(1) != _T("\\"))
	{
		sPath += _T("\\");
	}
	sPath += _T("*.*");

	if ((hFind = ::FindFirstFile((LPCTSTR)sPath, &fd)) != INVALID_HANDLE_VALUE)
	{
		if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			nCount++;
		}
		
		while(::FindNextFile(hFind, &fd))
		{
			if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				nCount++;
			}
		}
		::FindClose(hFind);
	}
	return nCount;
}

//==================================================================//
// 파일 확장자 변경
// : szFileName : [폴더+파일명]로 구성되어 있다
// : 확장자는 없음 [Format상 미존재함]
//==================================================================//
void OnChangeExtension(CString szFileName) 
{
	CTime tTime = CTime::GetCurrentTime();

	CString sExtension;
	sExtension.Format("%04d%02d%02d%02d%02d%02d", tTime.GetYear(), tTime.GetMonth(), tTime.GetDay(),
		tTime.GetHour(), tTime.GetMinute(), tTime.GetSecond());

	CString sRenameFile = szFileName;
	sRenameFile += _T(".");
	sRenameFile += sExtension;

	rename(szFileName, sRenameFile);
}

//==================================================================//
// 파일명에 확장자[.TXT] 추가
//==================================================================//
CString OnAdd_Extension(CString szFileName) 
{
	CString sCheckData = szFileName;
	int nPos = sCheckData.Find(".TXT");
	if (nPos == -1)	{
		sCheckData += _T(".TXT");
	}
	return sCheckData;
}

//==================================================================//
// 프로세스의 권한 설정
//==================================================================//
char OnGet_NtPrivilege() 
{
	HANDLE hToken;
	TOKEN_PRIVILEGES tPrivilege;

	// 현재 프로세스의 권한과 관련된 정보를 변경하기 위해 토큰 정보를 연다
	if (!OpenProcessToken(GetCurrentProcess(),
		TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
	{
		// 권한과 관련된 정보 접근에 실패함
		return 0;
	}

	// 현재 프로세스가 SE_SHUTDOWN_NAME 권한을 사용할수 있도록 설정한다
	LookupPrivilegeValue(NULL, SE_SYSTEMTIME_NAME, &tPrivilege.Privileges[0].Luid);
	tPrivilege.PrivilegeCount = 1;
	tPrivilege.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	// 지정한 값으로 권한을 조정한다
	AdjustTokenPrivileges(hToken, FALSE, &tPrivilege, 0, (PTOKEN_PRIVILEGES)NULL, 0);
	if (GetLastError() != ERROR_SUCCESS)
	{
		// 권한 조정에 실패한 경우
		return 0;
	}
	return 1;
}

//==================================================================//
// 운영 체제 버전 정보 얻기
//==================================================================//
char OnGet_OSVersionType() 
{
	char chVersion = -1;   // 계산된 윈도우즈 버전 저장
	OSVERSIONINFOEX osvi;  // 윈도우즈 버전에 관련된 정보

	// OSVERSIONINFOEX 구조체 값을 얻었는지 OSVERSIONINFO 구조체값을 얻었는지 구분
	// : [0: OSVERSIONINFO 구조체 정보를 얻었다는 의미]
	BOOL bVersionExFlag = 0;

	ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));

	// 현재 운영체제의 버전을 알수 없기 때문에 OSVERSIONINFOEX 속성으로 버전정보를 얻는다
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

	// GetVersionEx 함수를 사용하여 현재의 운영체제 정보를 얻는다
	// OSVERSIONINFOEX 값을 설정하고 호출했기 대문에 윈도우 NT 4.0 SP6 이상 및 그 이후
	// 버전의 윈도우가 아니라면 이 함수는 실패할 것이다
	if (!(bVersionExFlag = GetVersionEx((OSVERSIONINFO *)&osvi)))
	{
		// 윈도우즈 버전이 낮아서 OSVERSIONINFOEX 형식으로 값을 얻을수 없는 경우..
		// OSVERSIONINFO 형식으로 재설정하고 값을 얻는다
		osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
		if (!GetVersionEx((OSVERSIONINFO *)&osvi))	{
			return -1;
		}
	}

	// 얻어온 OSVERSIONINFOEX 구조체 변수의 값을 이용하여 버전을 구별한다
	switch(osvi.dwPlatformId)
	{
	// NT 기술 기반의 운영체제인 경우
	case VER_PLATFORM_WIN32_NT:
		if (osvi.dwMajorVersion <= 4)	{
			chVersion = 4;
		}
		else if (osvi.dwMajorVersion == 5 &&
			osvi.dwMinorVersion == 0)
		{
			chVersion = 5;
		}
		else if (bVersionExFlag)
		{
			if (osvi.dwMajorVersion == 5)	{
				chVersion = 6;
			}
			else if (osvi.dwMajorVersion == 6)	{
				// 윈도우즈 비스트와 윈도우즈 7은 osvi.dwMajorVersion이 6이다
				chVersion = 7;
			}
		}
		break;

	// 윈도우즈 9x 기반의 운영체제인 경우
	case VER_PLATFORM_WIN32_WINDOWS:
		if (osvi.dwMajorVersion == 4 &&
			osvi.dwMinorVersion == 0)
		{
			chVersion = 1;
		}
		else if (osvi.dwMajorVersion == 4 &&
			osvi.dwMinorVersion == 10)
		{
			chVersion = 2;
		}
		else if (osvi.dwMajorVersion == 4 &&
			osvi.dwMinorVersion == 90)
		{
			chVersion = 3;
		}
		break;
	}
	return chVersion;
}

//==================================================================//
// 특정 위치 BIT 상태 검사
//==================================================================//
BOOL OnBitCheck(long nzValue, int nzBitPos) 
{
	unsigned long nMask = 0x0001;  // 마스크 BIT 설정
	nMask <<= nzBitPos;  // 변경할 BIT 위치로 이동 [SHIFT]
	
	return(nzValue & nMask);
}

//==================================================================//
// 특정 위치 BIT Value 변경
//==================================================================//
void OnSetBitVal(long &npValue, int nzBit, BOOL bzOn) 
{
	unsigned long nMask = 0x0001;
	nMask <<= nzBit;

	if (bzOn)	{
		npValue |= nMask;
	}
	else	{
		npValue &= ~nMask;
	}
}

//==================================================================//
// [Binary -> String] 변환
//==================================================================//
CString OnCvt_Binary2String(int nzType, int nzSize, CString szData, int& npDataKind) 
{
	int nTolSize=0, nLoop=0;
	CString sTemp, sTemp2;
	CString sRet = _T(""), sWork = _T("");

	if (nzType == TYPE_WORD)
	{
		nTolSize=4;

		for(nLoop=0; nLoop < nzSize; nLoop++)
		{
			sWork = szData.Mid(nLoop, nTolSize);
			sTemp = OnCvt_Hex2Decimal(sWork);  // [Hex -> Decimal] 변환

			sRet += sTemp;
			
			nLoop += (nTolSize-1);
			if ((nLoop+1) < nzSize)	{
				sRet += _T(",");
			}
			npDataKind++;
		}
	}
	else if (nzType == TYPE_BYTE)
	{
		nTolSize=4;

		for(nLoop=0; nLoop < nzSize; nLoop++)
		{
			if ((nLoop + nTolSize) > nzSize)	{
				break;
			}

			sWork = szData.Mid(nLoop, 2);
			sTemp = OnCvt_Hex2Decimal(sWork);

			sWork  = szData.Mid(nLoop+2, 2);
			sTemp2 = OnCvt_Hex2Decimal(sWork);

			sRet += sTemp2;
			npDataKind++;
			sRet += _T(",");
			sRet += sTemp;
			
			nLoop += (nTolSize-1);
			if ((nLoop+1) < nzSize)	{
				sRet += _T(",");
			}
			npDataKind++;
		}
	}
	return sRet;
}

//==================================================================//
// [DWORD -> BIT] 변환
//==================================================================//
CString OnCvt_DWord2Bit(CString szDW) 
{
	CString sRet = _T("");
	int nlen, nCvt;
	char chBuf[4], chResult[4], *pStop;
	
	nlen = szDW.GetLength();
	for(int nLoop=0; nLoop < nlen; nLoop++)
	{
		nCvt = strtol(szDW.Mid(nLoop, 1), &pStop, 16);
		itoa(nCvt, chResult, 2);
		sprintf(chBuf, "%4s", chResult);
		sRet += chBuf;
	}
	sRet.Replace(" ", "0");
	return sRet;
}

//==================================================================//
// 2WORD HEX 값을 LONG으로 변환
//==================================================================//
CString OnCvt_Hex2DWORD(CString szHex) 
{
	CString sRet, sUpperWord, sLowerWord;
	sUpperWord = szHex.Mid(4, 4);
	sLowerWord = szHex.Mid(0, 4);

	CString sNewData = sUpperWord + sLowerWord;

	char *pchNewData;
	DWORD dwData = strtoul(sNewData, &pchNewData, 16);
	sRet.Format("%ld", dwData);
	return sRet;
}

//==================================================================//
// [Hex -> Decimal] 변환
//==================================================================//
CString OnCvt_Hex2Decimal(CString szHex) 
{
	CString sSort = _T(""), sRet = _T("");
	int nResult=0, nTemp, nFactor, nLoop, nIndex, nHexLen;

	nHexLen = szHex.GetLength();
	for(nLoop=nHexLen; nLoop>=0; nLoop--)
	{
		if (fmod(nLoop, 4) == 0 && nLoop < nHexLen)
		{
			sSort += szHex.Mid(nLoop, 4);
		}
	}

	for(nIndex = (nHexLen-1), nFactor=1; nIndex >= 0; nIndex--)
	{
		nTemp=0;

		if (szHex[nIndex] >= '0' && sSort[nIndex] <= '9')	{
			nTemp = (sSort[nIndex] - '0');
		}
		else if (sSort[nIndex] >= 'a' && sSort[nIndex] <= 'f')	{
			nTemp = (sSort[nIndex] - 'a') + 10;
		}
		else if (sSort[nIndex] >= 'A' && sSort[nIndex] <= 'F')	{
			nTemp = (sSort[nIndex] - 'A') + 10;
		}
		nResult += (nTemp * nFactor);
		nFactor *= 16;
	}
	sRet.Format("%d", nResult);

	return sRet;
}

//==================================================================//
// [Hex -> Ascii] 변환
//==================================================================//
CString OnCvt_Hex2Ascii(CString szHex) 
{
	ASSERT(szHex.GetLength()%2 == 0);
	
	CString sAscii, sTemp;
	int nLoop;
	long lValue;
	char *pchData;
	
	int nHexLen = szHex.GetLength();
	for(nLoop=0; nLoop < (nHexLen/2); nLoop++)
	{
		sTemp = szHex.Mid(nLoop*2, 2);
		lValue = (char)strtol(sTemp, &pchData, 16);
		
		sTemp = _T(" ");
		if (lValue>0 && lValue<128)	{
			sTemp.SetAt(0, (char)lValue);
		}
		else	{
			sTemp =_T("?");
		}
		sAscii += sTemp;
	}
	
	int nAsciiLen = sAscii.GetLength();
	if ((nAsciiLen%2) != 0)	{
		sAscii +=_T(" ");
	}
	
	char chTemp;
	for(nLoop=0; nLoop < nAsciiLen; nLoop++)
	{
		if ((nLoop%2) == 0)
		{
			chTemp = sAscii.GetAt(nLoop);
			sAscii.SetAt(nLoop, sAscii.GetAt(nLoop+1));
			sAscii.SetAt(nLoop+1, chTemp);
		}
	}
	
	sAscii.TrimRight();
	sAscii.Replace("?", "");  // 규이리 추가 [2011.05.27]
	return sAscii;
}

//==================================================================//
// [Hex -> Long] 변환
//==================================================================//
CString OnCvt_Hex2Long(CString szHex) 
{
	CString sRet = _T(""), sWork, sTemp;
	long lValue;
	char *pchNewData;
	
	sWork = szHex;
	int nHexLen = sWork.GetLength();
	for(int nLoop=0; nLoop < (nHexLen/4); nLoop++)
	{
		sTemp = sWork.Mid(nLoop*4, 4);
		lValue = strtol(sTemp, &pchNewData, 16);
		sRet.Format("%d", lValue);
	}
	sRet.Replace(" ", "0");  // 규이리 추가 [2011.05.28]
	return sRet;
}

//==================================================================//
// [Ascii -> Hex] 변환
//==================================================================//
CString OnCvt_Ascii2Hex(CString szAscii) 
{
	CString sWork, sTemp, sHex;
	int nLoop=0;
	char chTemp;

	sWork = szAscii;
	int nAsciiLen = sWork.GetLength();
	if ((nAsciiLen)%2 != 0)	{
		sWork += _T(" ");
	}

	// Word Order Change
	for(nLoop=0; nLoop < nAsciiLen; nLoop++)
	{
		if ((nLoop%2) == 0)
		{
			chTemp = sWork.GetAt(nLoop);
			sWork.SetAt(nLoop, sWork.GetAt(nLoop+1));
			sWork.SetAt(nLoop+1, chTemp);
		}
	}

	for(nLoop=0; nLoop < nAsciiLen; nLoop++)
	{
		chTemp = sWork[nLoop];
		sTemp.Format("%02X", chTemp);
		sHex += sTemp;
	}
	return sHex;
}

//==================================================================//
// [Long -> Hex] 변환
//==================================================================//
CString OnCvt_Long2Hex(CString szValue, int nzLen) 
{
	CString sCvtData = _T(""), sRet = _T("");
	char chTemp[33], *pStop;

	long lValue = strtoul(szValue, &pStop, 10);
	sprintf(chTemp, "%32X", lValue);
	sCvtData = chTemp;
	sCvtData.Replace(" ", "0");
	sCvtData = sCvtData.Right(nzLen);

	if (nzLen == 8)	{
		sRet = sCvtData.Right(4) + sCvtData.Left(4);
	}
	else if (nzLen == 4)	{
		sRet = sCvtData;
	}
	else if (nzLen == 2)	{
		sRet = sCvtData;
	}
	return sRet;
}

//==================================================================//
// [Long -> HexStr] 변환
//==================================================================//
CString OnCvt_Long2HexStr(LONG lzValue, int nzLen) 
{
	CString sRet = _T("");
	char chTemp[33];

	sprintf(chTemp, "%32X", lzValue);
	sRet = chTemp;
	sRet.Replace(" ", "0");
	sRet = sRet.Right(nzLen);
	return sRet;
}

//==================================================================//
// [10진수 -> 16진수] 변환
//==================================================================//
CString OnCvt_Dec2Hex(float fzDecData) 
{
	int nFloatToInt = (int)(fzDecData*10);

	CString sResult =_T("");
	sResult.Format("%04x", nFloatToInt);
	sResult.MakeUpper();
	return sResult;
}

//==================================================================//
// [10진수 -> 16진수] 변환
//==================================================================//
CString OnCvt_Dec2Hex(int nzDecData) 
{
	int nFloatToInt = nzDecData;

	CString sResult =_T("");
	sResult.Format("%04x", nFloatToInt);
	sResult.MakeUpper();
	return sResult;
}
