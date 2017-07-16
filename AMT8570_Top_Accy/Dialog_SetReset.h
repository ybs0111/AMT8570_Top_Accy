#if !defined(AFX_DIALOG_SETRESET_H__80E30BCE_9163_40F1_AC44_412762DCC9FB__INCLUDED_)
#define AFX_DIALOG_SETRESET_H__80E30BCE_9163_40F1_AC44_412762DCC9FB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dialog_SetReset.h : header file
//
#include "Variable.h"		// 전역 변수 정의 클래스 추가 
#include "GradientStatic.h" // 그라데이션 칼라 텍스트 박스 생성 클래스 추가 
/////////////////////////////////////////////////////////////////////////////
// CDialog_SetReset dialog

class CDialog_SetReset : public StandardDialog
{
// Construction
public:
	void OnInfo_Lable_Set();
	void OnInfo_Button_Set();
	CDialog_SetReset(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialog_SetReset)
	enum { IDD = IDD_DIALOG_SETRESET };
	CGradientStatic	m_info_message;
	//}}AFX_DATA

// Attributes
private:
	CFont* mp_info_font;					// 폰트 설정 변수 선언 
	
	int mn_dialog;

public:
	int mn_Result;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialog_SetReset)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialog_SetReset)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnButtonSet();
	afx_msg void OnButtonReset();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOG_SETRESET_H__80E30BCE_9163_40F1_AC44_412762DCC9FB__INCLUDED_)
