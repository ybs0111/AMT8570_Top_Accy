#if !defined(AFX_SCREEN_INITIAL_H__C8338AC3_B7DD_4866_93A2_DF84D57A5B3E__INCLUDED_)
#define AFX_SCREEN_INITIAL_H__C8338AC3_B7DD_4866_93A2_DF84D57A5B3E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Screen_Initial.h : header file
//

#include "Variable.h"
#include "Public_Function.h"

#include "PictureEx.h"			// GIF ��� Ŭ����
#include "GradientStatic.h"		// �׶��̼� Į�� �� Ŭ����
#include "XPButton.h"			// XP ������ Į�� ��ư Ŭ����
#include "LedButton.h"			// LED ��ư Ŭ����
#include "MacButtons.h"			// MAC ������ ��ư Ŭ����
#include "SuperProgressCtrl.h"	// ���α׷��� Ŭ����
#include "MacButtons.h"

// �ʱ�ȭ ó�� Ÿ�̸�
#define TMR_INIT		710

/////////////////////////////////////////////////////////////////////////////
// CScreen_Initial form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CScreen_Initial : public CFormView
{
public:
	CScreen_Initial();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CScreen_Initial)

// Form Data
public:
	//{{AFX_DATA(CScreen_Initial)
	enum { IDD = IDD_SCREEN_INITIAL };
	
	CLedButton	m_radio_XYZrobot;
	CLedButton	m_radio_loading_conveyor;
	CLedButton	m_radio_buffer_align_conveyor;
	CLedButton	m_radio_box_lift;
	CLedButton	m_radio_box_clamp;
	CLedButton	m_radio_mot_bd;
	CLedButton	m_radio_io_bd;
	CPictureEx	m_gif_initializing;
	CMacButton	m_btn_init_skip;
	CMacButton	m_btn_init_retry;
	CGradientStatic	m_msg_initial;
	//}}AFX_DATA


// Attributes
private:
	CSuperProgressCtrl m_ctrlProgress;
	CSuperProgressCtrl m_ctrlProgress1;
	
// Attributes
public:
	CFont* mp_fontInit;
	
	int  m_nStep_Process;
	
	int  m_nStep_Init;
	int  m_nFlag_Init;
	
// Operations
public:
	void OnSet_LblCtrl();	// Į�� Label ����
	void OnSet_BtnCtrl();	// Į�� ��ư ����
	void OnSet_GifCtrl();	// �̹���[GIF] �ε�
	void OnSet_ProgCtrl();	// ���α׷����� ����
	
	void OnChangeInitState(int nzStatus);	// �ʱ�ȭ ���� ���� ǥ�ÿ� ���� ��ư üũ ���� ����
	
	void OnSet_DefaultVal();	// [�ʱ�ȭ] �̴ϼ� ���� �� ���� ����
	void OnReadyINIT();	// �ʱ�ȭ �۾� �غ� [���� üũ �� �ʱ�ȭ Ÿ�̸� ����]
	
	int  OnRun_Initial();	// ������ �ʱ�ȭ �۾� ����
	void OnInitReady();	// [�غ�] �̴ϼ� �۾�

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScreen_Initial)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CScreen_Initial();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CScreen_Initial)
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBtnInitSkip();
	afx_msg void OnBtnInitRetry();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCREEN_INITIAL_H__C8338AC3_B7DD_4866_93A2_DF84D57A5B3E__INCLUDED_)
