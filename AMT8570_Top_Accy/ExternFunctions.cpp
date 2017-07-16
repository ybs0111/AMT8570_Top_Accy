// ExternFunctions.cpp: implementation of the CExternFunctions class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ExternFunctions.h"

// ���� ���� ���� �˻� �Լ� ȣ���ϱ� ���ؼ��� �ݵ�� INCLUDE �ʿ�
#include "io.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//==================================================================//
// �Է� �ð���ŭ ��� �� ����
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
// ���� �ý��� �ð� ����
//==================================================================//
int _TimeGet() 
{
	return (int)(GetTickCount()&0x7FFFFFFF);
}

//==================================================================//
// �Է� �ð���ŭ ��� �� ����
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
			dwTimeDiff = ULONG_MAX - (dwStart+dwCurr);  // ULONG_MAX ������� = 4294967295
		}
		else
		{
			dwTimeDiff = dwCurr-dwStart;
		}
	}
}

//==================================================================//
// ������ �޽��� ó��
//==================================================================//
void _DoEvents() 
{
	MSG msg;

	// ������ �޽��� ó��
	// - PeekMessage() : �޽����� ���� �������� �޽��� ť�� ����
	//                 : �޽����� �ִٸ� NonZero ��ȯ
	//                 : �޽����� ���ٸ� Zero�� ��ȯ
	// : PM_REMOVE = ó���Ŀ� ť���� �޽��� ����
	while(PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
	{
		// �޽��� ���� �� ó��
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}



//==================================================================//
// �ý��� ���� ���� ����
//==================================================================//
BOOL _GetSystemPath(CString &spPath) 
{
	char chPath[256] = {0,};
	
	UINT unRet = GetSystemDirectory(chPath, 256);
	spPath.Format("%s\\", chPath);
	
	return (unRet>0) ? TRUE:FALSE;
}

//==================================================================//
// ������ ���� ���� ����
//==================================================================//
BOOL _GetWindowPath(CString &spPath) 
{
	char chPath[256] = {0,};
	
	UINT unRet = GetWindowsDirectory(chPath, 256);
	spPath.Format("%s\\", chPath);
	
	return (unRet>0) ? TRUE:FALSE;
}

//==================================================================//
// ���� ���α׷��� ���� ���� ����
//==================================================================//
void _GetAppPath(CString& spAppDir) 
{
	TCHAR tcFullPath[MAX_PATH];
	TCHAR tcDir[_MAX_DIR];
	TCHAR tcDrv[_MAX_DRIVE];
	
	// App ��ü ��� ���� ����
	::GetModuleFileName(NULL, tcFullPath, MAX_PATH);
	
	// ����̺�, ���� ���� ������ ����ȭ
	_splitpath(tcFullPath, tcDrv, tcDir, NULL, NULL);
	spAppDir.Format(_T("%s%s"), tcDrv, tcDir);
}



//==================================================================//
// ���� �۾� ���� ���� ����
//==================================================================//
CString _GetCurrentFolder() 
{
	/*
	char path[MAX_PATH] = {0};
	GetCurrentDirectory(MAX_PATH, path);  // ���� �۾� ����
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
// ���� ����
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
// [����+����] ����
// - �� ���� ������ �������� ����
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
// [����+����] ����
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
	FileOp.fFlags	= FOF_SILENT | FOF_NOCONFIRMATION;  // FOF_NOCONFIRMATION | FOF_NOERRORUI;  // Ȯ�� �޽��� �����
	FileOp.fAnyOperationsAborted = false;
	FileOp.hNameMappings		= NULL;
	FileOp.lpszProgressTitle	= chTemp;
	FileOp.pFrom	= chTemp;

	// ���� �Ϸ�
	if (SHFileOperation(&FileOp) == 0)
	{
		return TRUE;
	}
	return FALSE;
}

//==================================================================//
// ���� ���� ���� üũ
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
// ���� ���� ���� üũ
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
// ������ ���� ũ�� �̻����� ������ ������ �˻�
// long lzSize : ���� ũ�� ���� [��) 1000000]
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
// ���� ���� ���� ����
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
// Ư�� ��ġ BIT ���� �˻�
//==================================================================//
BOOL _GetBitVal(long nzValue, int nzBitPos) 
{
	// ����ũ BIT ����
	unsigned long lMask = 0x0001;
	// ������ BIT ��ġ�� �̵� [SHIFT]
	lMask <<= nzBitPos;

	return(nzValue & lMask);
}

//==================================================================//
// Ư�� ��ġ BIT �� ����
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
// ���ϴ� ��ġ�� �� ��ȯ
// ���̸� �߰� [2013.12.11]
//==================================================================//
CString _FTMid(CString szData, int nzStartPos, int nzLen) 
{
	CString sTmp = szData;
	// �Էµ� ���ڿ��� ��ü ����
	int nLen = sTmp.GetLength();
	// �Ľ��ϱ� ���� �ʿ��� �ּ� ���ڿ� ����
	int nWantedLen = nzStartPos + nzLen;
	
	// ���� ���ڿ��� �߶� ��ġ���� ū ���
	CString sRetVal;
	sRetVal.Empty();
	
	// ���ڿ� ã�� ���� ��ġ�� ��ü ���ڿ� ���̺��� ū ���
	// ã�� �� �ִ� ������ �ƴϹǷ� ������ ���� ���Ͻ�Ŵ
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
