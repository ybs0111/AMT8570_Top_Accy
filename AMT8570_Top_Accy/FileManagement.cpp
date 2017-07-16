// FileManagement.cpp : implementation file
//

#include "stdafx.h"
#include "Handler.h"
#include "FileManagement.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int const MAX_CHAR = 0x40000;
/////////////////////////////////////////////////////////////////////////////
// CFileManagement

CFileManagement::CFileManagement()
{
}



CFileManagement::~CFileManagement()
{
}


/*
BEGIN_MESSAGE_MAP(CFileManagement, CWnd)
	//{{AFX_MSG_MAP(CFileManagement)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
*/

/////////////////////////////////////////////////////////////////////////////
// CFileManagement message handlers

int CFileManagement::FileWrite(CString strFileName, CString strContent)
{
	int nResult;
//	::WaitForSingleObject(m_hMutex,100);

	CThreadSync Sync;

	if(strFileName.IsEmpty())
	{
		nResult = -1;
	}

	else
	{
		CFile file;
		
		TRY
		{
			if(file.Open(strFileName, CFile::modeWrite | CFile::modeCreate|CFile::shareDenyNone) == TRUE)
			{
				file.Write(strContent, strContent.GetLength());
				file.Close();
				nResult = 1;

			}
		}

		CATCH (CFileException, e)
		{
			if (st_handler.mn_language == LANGUAGE_ENGLISH)
			{
				AfxMessageBox("Failed to save file");	// 20130926 kjh
			}
			else
			{
				AfxMessageBox("화일 저장 실패");
			}
			file.Close();
			nResult = -2;
		}
		END_CATCH
	}

//	::ReleaseMutex(m_hMutex);
	return nResult;
}

int CFileManagement::FileRead(CString strFileName)
{
	CThreadSync Sync;

	int nResult;

	// CString의 배열값 초기화.
	
	for(int i = 0; i < MAX_LINE; i++)  
	{
		strLineInfo[i].Empty();
		strItemName[i].Empty();
		strItemValue[i].Empty();
	}
	

	if(strFileName.IsEmpty())
	{
		nResult = -1;
	}
	else
	{
		TRY
		{
			BYTE buffer[MAX_SIZE];

			for(int i=0; i<MAX_SIZE; i++)
				buffer[i] = NULL;

			CFile file(_T(strFileName), CFile::modeReadWrite|CFile::shareDenyNone);
			DWORD dwBytesRemaining = file.GetLength();
			
			while(dwBytesRemaining)
			{
				UINT nBytesRead = file.Read(buffer, sizeof(buffer));
				dwBytesRemaining -= nBytesRead;
			}

			file.Close();
			LineCut(&buffer[0]);
			nResult =1;
		}
		CATCH(CFileException, e)
		{
			return -2;
		}
		END_CATCH
	}

	return nResult;
}

void CFileManagement::LineCut(BYTE* buffer)
{
	CString strBuffer, strGetchar, strTmp;
	int length;
	unsigned short getAscii = 0;

	nItemLength = 0;
	strBuffer = buffer;
	length = strBuffer.GetLength();

	for(int i = 0 ; i < length ; i++)
	{
		getAscii = strBuffer.GetAt(i);
		strGetchar.Format("%c", getAscii);
		strTmp = strTmp + strGetchar;
		if(getAscii == 10)
		{
			strLineInfo[nItemLength++] = strTmp;
			strTmp = "";
		}
	}
	LineAnalysis();
}

void CFileManagement::LineAnalysis()
{
	unsigned short getAscii = 0;
	CString strTmp;
	int length, find;
	 
	for(int i = 0 ; i < nItemLength ; i++)
	{
		length = strLineInfo[i].GetLength();
		find = strLineInfo[i].Find('=', 0);
		strItemName[i] = strLineInfo[i].Left(find);
		strItemName[i].TrimRight();
		strItemValue[i] = strLineInfo[i].Right(length - (find + 1));
		strItemValue[i].TrimLeft();
		strItemValue[i].TrimRight();
	}
}

CString CFileManagement::FindValue(CString itemName)
{
	for(int i = 0 ; i < nItemLength ; i++)
	{
		if(strItemName[i] == itemName)
		{
			return strItemValue[i];
		}
	}
	return "X";
}

CString CFileManagement::FindValueRev(CString itemName)
{
	for(int i = nItemLength ; i > 0 ; i--)
	{
		if(strItemName[i] == itemName)
			return strItemValue[i];
	}
	return "X";
}

int CFileManagement::SaveValue(CString strFileName, CString itemName, CString itemValue)
{
	FileRead(strFileName);

	CString strFind, strContent, strTmp;
	int chk = 0;

	for(int uuu = 0 ; uuu < nItemLength ; uuu++)
	{
		if(strItemName[uuu] == itemName)
		{
			chk = 1;
			strItemValue[uuu] = itemValue;
			strLineInfo[uuu].Format("%s=%s\r\n", itemName, itemValue);
		}
	
		strContent += strLineInfo[uuu];
	}

	if(chk)
	{
		FileWrite(strFileName, strContent);
		return 1;
	}
	else
		return -1;
}

void CFileManagement::FileAppend(CString strFileName, CString strContent)
{
	CThreadSync Sync;

	if(strFileName.IsEmpty())
	{
		if (st_handler.mn_language == LANGUAGE_ENGLISH)
		{
			AfxMessageBox("Filename is not defined.");
		}
		else
		{
			AfxMessageBox("Filename이 정의 되지 않았습니다.");	// 20130926 kjh english
		}
	}
	else
	{
		BYTE buffer[MAX_SIZE];

		for(int i = 0; i < MAX_SIZE ; i++)
			buffer[i] = NULL;

		TRY
		{
			CFile file(_T (strFileName), CFile::modeReadWrite|CFile::shareDenyNone);
			DWORD dwBytesRemaining = file.GetLength();

			while(dwBytesRemaining)
			{
				UINT nBytesRead = file.Read(buffer, sizeof(buffer));
				dwBytesRemaining -= nBytesRead;
			}

			file.Write(strContent, strContent.GetLength());
			file.Close();
		}
		CATCH( CFileException, e )
		{
			CString tmp;
			tmp.Format("(%s)\nFile could not be opened!!\n\n"
				"해당 File을 Access하지 못했습니다.\n"
				"해당 경로에 File이 존재하는지 확인해 보십시오.\n\n"
				"프로그램에 심각한 영향을 미칠수 있습니다!!!!", strFileName);

			AfxMessageBox(tmp);
		}
		END_CATCH
	}
}

void CFileManagement::DeleteAllFolderFile(LPCTSTR path, LPCTSTR name)
{
   	CFileFind   find;
    CString     fupath      =   _T("");
    BOOL        is_found    =   FALSE;

	CString strTemp;

    if ( is_found = find.FindFile(fupath) )
    {
        while( is_found )
        {
            is_found    =   find.FindNextFile();
			strTemp = find.GetFileName();

            if ( find.IsDots())
                continue;
            if ( find.IsDirectory() )
            {
                DeleteAllFolderFile( find.GetFilePath(), name );
				strTemp = find.GetFilePath();
				::RemoveDirectory(strTemp);
            }
            else
            {
				strTemp = find.GetFilePath();
				::DeleteFile(strTemp);
            }
       }
  		is_found = find.FindFile(path);
		if(	is_found)
		{
			::RemoveDirectory(path);
		}
   }
    else
	{
		::RemoveDirectory(path);
	}
    find.Close();
}

void CFileManagement::WriteFile(const CString &Filename, const CString &FileContents)
{  
	char buffer[MAX_CHAR];
	CThreadSync Sync;
	strcpy((LPTSTR)buffer, FileContents);

	int nResult;

  //  ::WaitForSingleObject(m_hMutex, 30);
	if(Filename.IsEmpty())
	{
		nResult = -1;
	}
	else
	{
		CFile file;
		try{
			if (file.Open(_T (Filename), CFile::modeWrite|CFile::modeCreate |CFile::shareDenyNone) == TRUE) //0506, Just Write-only
			{
				DWORD dwBytesRemaining = FileContents.GetLength();
				DWORD dwPosition = file.GetPosition();
				file.Seek(dwPosition, CFile::begin);
				file.Write(buffer,dwBytesRemaining);
				file.Close();
			}
		}	
 		catch(CFileException* e){
			e->ReportError();
			e->Delete();	
		}
	}
	// 버퍼를 비워 주자.
//	::ReleaseMutex(m_hMutex);
	CString strClear;
	strClear = "";
	strcpy((LPTSTR)buffer,strClear);
}