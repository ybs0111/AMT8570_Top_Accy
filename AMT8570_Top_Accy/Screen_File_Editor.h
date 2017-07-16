#if !defined(AFX_SCREEN_FILE_EDITOR_H__84660736_DA32_43BE_8FD7_CE0F848B861A__INCLUDED_)
#define AFX_SCREEN_FILE_EDITOR_H__84660736_DA32_43BE_8FD7_CE0F848B861A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Screen_File_Editor.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CScreen_File_Editor form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "Variable.h"		// 전역 변수 정의 클래스 추가 
#include "Public_Function.h"
#include "BtnST.h"          // 칼라 버튼 생성 클래스 추가 
#include "XPGroupBox.h"		// 칼라 그룹 박스 생성 클래스 추가 
#include "sxlogfont.h"
#include "MacButtons.h"
#include "GridControlAlg.h"
#include "Picture.h"


class CScreen_File_Editor : public CFormView
{
public:
	CScreen_File_Editor();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CScreen_File_Editor)

// Form Data
public:
	//{{AFX_DATA(CScreen_File_Editor)
	enum { IDD = IDD_SCREEN_FILE_EDITOR };
	CMacButton	m_btn_time_save;
	CMacButton	m_btn_time_open;
	CMacButton	m_btn_motor_pos_save;
	CMacButton	m_btn_motor_pos_open;
	CMacButton	m_btn_motor_part_save;
	CMacButton	m_btn_motor_part_open;
	CMacButton	m_btn_io_save;
	CMacButton	m_btn_io_part_save;
	CMacButton	m_btn_io_part_open;
	CMacButton	m_btn_io_open;
	CXPGroupBox	m_group_time;
	CXPGroupBox	m_group_motor_pos;
	CXPGroupBox	m_group_motor_part;
	CXPGroupBox	m_group_io_part;
	CXPGroupBox	m_group_io;
	CXPGroupBox	m_group_file_info;
	//}}AFX_DATA

// Attributes
public:
	CFont			*m_p_font;
	GridControlAlg	m_p_grid;
	TSpread			*m_grid_info;

// Operations
public:
	void OnFileEditor_GroupBox_Set();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScreen_File_Editor)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CScreen_File_Editor();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CScreen_File_Editor)
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBtnIoOpen();
	afx_msg void OnBtnIoSave();
	afx_msg void OnBtnIoPartOpen();
	afx_msg void OnBtnIoPartSave();
	afx_msg void OnBtnMotorPartOpen();
	afx_msg void OnBtnMotorPartSave();
	afx_msg void OnBtnMotorPosOpen();
	afx_msg void OnBtnMotorPosSave();
	afx_msg void OnBtnTimeOpen();
	afx_msg void OnBtnTimeSave();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCREEN_FILE_EDITOR_H__84660736_DA32_43BE_8FD7_CE0F848B861A__INCLUDED_)
