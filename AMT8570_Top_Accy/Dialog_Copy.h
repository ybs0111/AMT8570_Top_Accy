#if !defined(AFX_DIALOG_COPY_H__218BCA02_E33B_410F_9EB6_4540B15529AF__INCLUDED_)
#define AFX_DIALOG_COPY_H__218BCA02_E33B_410F_9EB6_4540B15529AF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dialog_Copy.h : header file
//

#include "Variable.h"

#include "NewLabel.h"
#include "EditEx.h"			// 칼라 에디트 박스 생성 클래스 추가 
#include "MacButtons.h"

/////////////////////////////////////////////////////////////////////////////
// CDialog_Copy dialog

class CDialog_Copy : public StandardDialog
{
// Construction
public:
	int	 iDialogPopup;
	CEditEx  m_edit_new_name;
	CString mstr_selected_device_name;
	CDialog_Copy(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialog_Copy)
	enum { IDD = IDD_DIALOG_COPY };
	CNewLabel	m_msg_org_name;
	CMacButton	m_btn_ok;
	CMacButton	m_btn_cancel;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialog_Copy)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialog_Copy)
	afx_msg void OnBtnDcOk();
	afx_msg void OnBtnDcCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOG_COPY_H__218BCA02_E33B_410F_9EB6_4540B15529AF__INCLUDED_)
