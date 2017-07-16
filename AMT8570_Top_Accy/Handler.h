// Handler.h : main header file for the HANDLER application
//

#if !defined(AFX_HANDLER_H__E8946B3B_9EC3_4BF1_9098_99EB0781B347__INCLUDED_)
#define AFX_HANDLER_H__E8946B3B_9EC3_4BF1_9098_99EB0781B347__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"			// main symbols
#include "SingleInstanceApp.h"	// Single Instance 생성 클래스 추가 
#include "Variable.h"			// 전역 변수 정의 클래스 추가 
/////////////////////////////////////////////////////////////////////////////
// CHandlerApp:
// See Handler.cpp for the implementation of this class
//

class CHandlerApp : public CSingleInstanceApp
{
public:
	CHandlerApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHandlerApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CHandlerApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HANDLER_H__E8946B3B_9EC3_4BF1_9098_99EB0781B347__INCLUDED_)
