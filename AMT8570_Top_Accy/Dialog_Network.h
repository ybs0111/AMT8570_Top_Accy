#if !defined(AFX_DIALOG_NETWORK_H__7D029612_1BC7_4A61_912A_434F33B93868__INCLUDED_)
#define AFX_DIALOG_NETWORK_H__7D029612_1BC7_4A61_912A_434F33B93868__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dialog_Network.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialog_Network dialog

class CDialog_Network : public StandardDialog
{
// Construction
public:
	CDialog_Network(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialog_Network)
	enum { IDD = IDD_DIALOG_NETWORK };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialog_Network)
	public:
	virtual BOOL Create(CWnd* pParentWnd);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialog_Network)
	afx_msg void OnSUPPLU_COMPLETE();
	afx_msg void OnSUPPLY_TYPE();
	afx_msg void OnALARM();
	afx_msg void OnDestroy();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOG_NETWORK_H__7D029612_1BC7_4A61_912A_434F33B93868__INCLUDED_)
