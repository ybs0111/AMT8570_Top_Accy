#if !defined(AFX_DIALOG_SMEMA_H__21522E63_F83C_415E_AF50_71926445C284__INCLUDED_)
#define AFX_DIALOG_SMEMA_H__21522E63_F83C_415E_AF50_71926445C284__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dialog_Smema.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialog_Smema dialog

class CDialog_Smema : public StandardDialog
{
// Construction
public:
	CDialog_Smema(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialog_Smema)
	enum { IDD = IDD_DIALOG_SMEMA };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialog_Smema)
	public:
	virtual BOOL Create(CWnd* pParentWnd);  // 모달리스 대화상자 생성
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialog_Smema)
	afx_msg void OnButton1();
	afx_msg void OnRearReadySignalON();
	afx_msg void OnRearReadySignalOFF();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOG_SMEMA_H__21522E63_F83C_415E_AF50_71926445C284__INCLUDED_)
