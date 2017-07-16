#if !defined(AFX_FILEMANAGEMENT_H__055FDD08_FE46_42DF_87A8_44C6E1F9DD05__INCLUDED_)
#define AFX_FILEMANAGEMENT_H__055FDD08_FE46_42DF_87A8_44C6E1F9DD05__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FileManagement.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFileManagement window
#define MAX_SIZE 50000
#define MAX_LINE 5000

class CFileManagement : public CMultiThreadSync<CFileManagement>
{
// Construction
public:
	CFileManagement();

// Attributes
public:
	CString strLineInfo[MAX_LINE];
	CString strItemValue[MAX_LINE];
	CString strItemName[MAX_LINE];
	int nItemLength;

	//CMutex mutex_File;
	HANDLE m_hMutex;

// Operations
public:
	int FileWrite(CString, CString);
	int FileRead(CString);
	void LineCut(BYTE* buffer);
	void LineAnalysis();
	int SaveValue(CString strFileName, CString itemName, CString itemValue);
	void FileAppend(CString, CString);
	CString FindValue(CString);
	CString	FindValueRev(CString);
	void DeleteAllFolderFile(LPCTSTR path, LPCTSTR name);

	void WriteFile(const CString &Filename, const CString &FileContents) ;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFileManagement)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CFileManagement();
/*
	// Generated message map functions
protected:
	//{{AFX_MSG(CFileManagement)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	*/
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILEMANAGEMENT_H__055FDD08_FE46_42DF_87A8_44C6E1F9DD05__INCLUDED_)
