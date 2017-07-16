#if !defined(AFX_SCREEN_ADMIN_H__AB02B8FF_25A7_4103_B294_E0123393C722__INCLUDED_)
#define AFX_SCREEN_ADMIN_H__AB02B8FF_25A7_4103_B294_E0123393C722__INCLUDED_

#include "Variable.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Screen_Admin.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CScreen_Admin form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CScreen_Admin : public CFormView
{
//protected:  // ¼öÁ¤ÇÔ [2014.08.26]
public:
	CScreen_Admin();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CScreen_Admin)

// Form Data
public:
	CString m_sLogMsg;
	//{{AFX_DATA(CScreen_Admin)
	enum { IDD = IDD_SCREEN_ADMIN };
	//}}AFX_DATA

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScreen_Admin)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CScreen_Admin();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CScreen_Admin)
	afx_msg void OnDestroy();
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCREEN_ADMIN_H__AB02B8FF_25A7_4103_B294_E0123393C722__INCLUDED_)
