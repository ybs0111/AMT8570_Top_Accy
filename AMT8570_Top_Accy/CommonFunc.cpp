// CommonFunc.cpp: implementation of the CCommonFunc class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CommonFunc.h"

//==================================================================//
// �ܺ� Ŭ���� �߰�
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
// ���ϴ� Ÿ��Ʋ���� ���� ������ �ڵ� �˻�
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
				// ���μ��� �ڵ� ���� ����
				return hFindWnd->m_hWnd;
			}
		}
		hFindWnd = hFindWnd->GetNextWindow();
	}
	return NULL;
}

//==================================================================//
// ���� �۾� ���� ��ȯ
//==================================================================//
CString OnGet_CurrentFolder() 
{
	/*
	char path[MAX_PATH] = {0};
	GetCurrentDirectory(MAX_PATH, path);  // ���� �۾� ����
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
// ���� ����
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
// ���� ����
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
// [����+����] ����
// : �� ���� ������ �������� �ʴ´�
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
// [����+����] ����
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
	FileOp.fFlags = FOF_SILENT | FOF_NOCONFIRMATION;//FOF_NOCONFIRMATION | FOF_NOERRORUI;  // Ȯ�� �޽��� ����� 
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
// ���� ���α׷� ���� ���� ��ȯ
//==================================================================//
void OnGet_AppDir(CString& szAppDir) 
{
	TCHAR szFullPath[MAX_PATH];
	TCHAR szDir[_MAX_DIR];
	TCHAR szDrive[_MAX_DRIVE];

	// App ��ü ��� ���� ��´�
	::GetModuleFileName(NULL, szFullPath, MAX_PATH);

	// ����̺�, ���� ���� ������ ����ȭ
	_splitpath(szFullPath, szDrive, szDir, NULL, NULL);

	szAppDir.Format(_T("%s%s"), szDrive, szDir);
}

//==================================================================//
// ���� ������ �߰�
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
// ���� ���� ���� ��ȯ
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
// ���� ���� ���� ��ȯ
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
// ���� ũ�Ⱑ ����ũ�� �̻����� �˻�
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
// ���� ���� ���� ��ȯ
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
// ������ �����ϴ� ���� ���� ��ȯ
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
// ���� Ȯ���� ����
// : szFileName : [����+���ϸ�]�� �����Ǿ� �ִ�
// : Ȯ���ڴ� ���� [Format�� ��������]
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
// ���ϸ� Ȯ����[.TXT] �߰�
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
// ���μ����� ���� ����
//==================================================================//
char OnGet_NtPrivilege() 
{
	HANDLE hToken;
	TOKEN_PRIVILEGES tPrivilege;

	// ���� ���μ����� ���Ѱ� ���õ� ������ �����ϱ� ���� ��ū ������ ����
	if (!OpenProcessToken(GetCurrentProcess(),
		TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
	{
		// ���Ѱ� ���õ� ���� ���ٿ� ������
		return 0;
	}

	// ���� ���μ����� SE_SHUTDOWN_NAME ������ ����Ҽ� �ֵ��� �����Ѵ�
	LookupPrivilegeValue(NULL, SE_SYSTEMTIME_NAME, &tPrivilege.Privileges[0].Luid);
	tPrivilege.PrivilegeCount = 1;
	tPrivilege.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	// ������ ������ ������ �����Ѵ�
	AdjustTokenPrivileges(hToken, FALSE, &tPrivilege, 0, (PTOKEN_PRIVILEGES)NULL, 0);
	if (GetLastError() != ERROR_SUCCESS)
	{
		// ���� ������ ������ ���
		return 0;
	}
	return 1;
}

//==================================================================//
// � ü�� ���� ���� ���
//==================================================================//
char OnGet_OSVersionType() 
{
	char chVersion = -1;   // ���� �������� ���� ����
	OSVERSIONINFOEX osvi;  // �������� ������ ���õ� ����

	// OSVERSIONINFOEX ����ü ���� ������� OSVERSIONINFO ����ü���� ������� ����
	// : [0: OSVERSIONINFO ����ü ������ ����ٴ� �ǹ�]
	BOOL bVersionExFlag = 0;

	ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));

	// ���� �ü���� ������ �˼� ���� ������ OSVERSIONINFOEX �Ӽ����� ���������� ��´�
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

	// GetVersionEx �Լ��� ����Ͽ� ������ �ü�� ������ ��´�
	// OSVERSIONINFOEX ���� �����ϰ� ȣ���߱� �빮�� ������ NT 4.0 SP6 �̻� �� �� ����
	// ������ �����찡 �ƴ϶�� �� �Լ��� ������ ���̴�
	if (!(bVersionExFlag = GetVersionEx((OSVERSIONINFO *)&osvi)))
	{
		// �������� ������ ���Ƽ� OSVERSIONINFOEX �������� ���� ������ ���� ���..
		// OSVERSIONINFO �������� �缳���ϰ� ���� ��´�
		osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
		if (!GetVersionEx((OSVERSIONINFO *)&osvi))	{
			return -1;
		}
	}

	// ���� OSVERSIONINFOEX ����ü ������ ���� �̿��Ͽ� ������ �����Ѵ�
	switch(osvi.dwPlatformId)
	{
	// NT ��� ����� �ü���� ���
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
				// �������� ��Ʈ�� �������� 7�� osvi.dwMajorVersion�� 6�̴�
				chVersion = 7;
			}
		}
		break;

	// �������� 9x ����� �ü���� ���
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
// Ư�� ��ġ BIT ���� �˻�
//==================================================================//
BOOL OnBitCheck(long nzValue, int nzBitPos) 
{
	unsigned long nMask = 0x0001;  // ����ũ BIT ����
	nMask <<= nzBitPos;  // ������ BIT ��ġ�� �̵� [SHIFT]
	
	return(nzValue & nMask);
}

//==================================================================//
// Ư�� ��ġ BIT Value ����
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
// [Binary -> String] ��ȯ
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
			sTemp = OnCvt_Hex2Decimal(sWork);  // [Hex -> Decimal] ��ȯ

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
// [DWORD -> BIT] ��ȯ
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
// 2WORD HEX ���� LONG���� ��ȯ
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
// [Hex -> Decimal] ��ȯ
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
// [Hex -> Ascii] ��ȯ
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
	sAscii.Replace("?", "");  // ���̸� �߰� [2011.05.27]
	return sAscii;
}

//==================================================================//
// [Hex -> Long] ��ȯ
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
	sRet.Replace(" ", "0");  // ���̸� �߰� [2011.05.28]
	return sRet;
}

//==================================================================//
// [Ascii -> Hex] ��ȯ
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
// [Long -> Hex] ��ȯ
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
// [Long -> HexStr] ��ȯ
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
// [10���� -> 16����] ��ȯ
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
// [10���� -> 16����] ��ȯ
//==================================================================//
CString OnCvt_Dec2Hex(int nzDecData) 
{
	int nFloatToInt = nzDecData;

	CString sResult =_T("");
	sResult.Format("%04x", nFloatToInt);
	sResult.MakeUpper();
	return sResult;
}
