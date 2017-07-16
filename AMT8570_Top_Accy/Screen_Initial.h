#if !defined(AFX_SCREEN_INITIAL_H__C8338AC3_B7DD_4866_93A2_DF84D57A5B3E__INCLUDED_)
#define AFX_SCREEN_INITIAL_H__C8338AC3_B7DD_4866_93A2_DF84D57A5B3E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Screen_Initial.h : header file
//

#include "Variable.h"
#include "Public_Function.h"

#include "PictureEx.h"			// GIF 출력 클래스
#include "GradientStatic.h"		// 그라데이션 칼라 라벨 클래스
#include "XPButton.h"			// XP 형태의 칼라 버튼 클래스
#include "LedButton.h"			// LED 버튼 클래스
#include "MacButtons.h"			// MAC 형태의 버튼 클래스
#include "SuperProgressCtrl.h"	// 프로그레스 클래스
#include "MacButtons.h"

// 초기화 처리 타이머
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
	void OnSet_LblCtrl();	// 칼라 Label 생성
	void OnSet_BtnCtrl();	// 칼라 버튼 생성
	void OnSet_GifCtrl();	// 이미지[GIF] 로딩
	void OnSet_ProgCtrl();	// 프로그레스바 생성
	
	void OnChangeInitState(int nzStatus);	// 초기화 진행 상태 표시용 라디오 버튼 체크 상태 변경
	
	void OnSet_DefaultVal();	// [초기화] 이니셜 상태 및 스텝 정보
	void OnReadyINIT();	// 초기화 작업 준비 [조건 체크 및 초기화 타이머 생성]
	
	int  OnRun_Initial();	// 시컨스 초기화 작업 진행
	void OnInitReady();	// [준비] 이니셜 작업

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
