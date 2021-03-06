#if !defined(AFX_SCREEN_MAIN_H__4124683A_789C_4F3E_BBCA_278E31E66B20__INCLUDED_)
#define AFX_SCREEN_MAIN_H__4124683A_789C_4F3E_BBCA_278E31E66B20__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Screen_Main.h : header file
//
#include "Variable.h"		// 전역 변수 정의 클래스 추가 
#include "Public_Function.h"

#include "NewLabel.h"		// 칼라 텍스트 박스 생성 클래스 추가 
#include "editex.h"			// 칼라 에디트 박스 생성 클래스 추가 

#include "BtnST.h"          // 칼라 버튼 생성 클래스 추가 

#include "XPGroupBox.h"		// 칼라 그룹 박스 생성 클래스 추가 
#include "sxlogfont.h"

#include "LedButton.h"      // LED 버튼 클래스

#include "EXDigitST.h"		// 디지털 카운터 생성 클래스 추가 
#include "MacButtons.h"
#include "Digit.h"

#include "GradientStatic.h" // 그라데이션 칼라 텍스트 박스 생성 클래스 추가 

#include "Picture.h"	

/////////////////////////////////////////////////////////////////////////////
// CScreen_Main form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#define TMR_STATE		1
#define ACCY_DISPLAY	2

class CScreen_Main : public CFormView
{
//protected:  // 수정함 [2014.08.26]
public:
	CScreen_Main();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CScreen_Main)

// Form Data
public:
	//{{AFX_DATA(CScreen_Main)
	enum { IDD = IDD_SCREEN_MAIN };
	//}}AFX_DATA

// Attributes
public:
	//////////////////////////////////////////////////////////////////////////
	// BMP 이미지 Display 관련 변수 
	//////////////////////////////////////////////////////////////////////////
	CPicture		m_p_bmp_view;
	CPicture		m_p_bmp_AccyBox[3];
	CPicture		m_p_bmp_EmptyAccyBox[3];
	CPicture		m_p_bmp_ClampAccyBox;
	CPicture		m_p_bmp_NGOutAccy;
	CPicture		m_p_bmp_NGBuffer[4];
	CPicture		m_p_bmp_BufferConv[4];
	
	int m_nCurrent_AccyBoxExist[3];
	int m_nCurrent_NGOutAccy;
	int m_nCurrent_NGBufferExist[4];
	int m_nCurrent_EmptyAccyBoxExist[2];
	int m_nCurrent_ClampAccyBoxAccyExist;
	int m_nCurrent_BufferCovAccyExist[8];

	//////////////////////////////////////////////////////////////////////////
	// Grid Info 관련 변수 
	//////////////////////////////////////////////////////////////////////////
	int m_nCurrent_Supply_LotType;
	int m_nCurrent_Supply_Count;
	int m_nCurrent_LoadingConv_State[3];
	int m_nCurrent_NG_Count;

	int m_nCurrent_Gripper[4];

	int m_nCurrent_Door[MAX_DOOR_];
	
	int m_nCurrent_RearReady;
	int m_nCurrent_RearInterlock;
	int m_nCurrent_RearRequest;
	int m_nCurrent_RearTransfer;

	//////////////////////////////////////////////////////////////////////////
	// EQP State Display 관련 변수
	//////////////////////////////////////////////////////////////////////////
	CXPGroupBox	m_group_forcedischarge_state;
	//CXPGroupBox	m_group_bcr_state;
	CXPGroupBox	m_group_supply_info;
	CXPGroupBox	m_group_smema_info;
	CXPGroupBox	m_group_gripper_info;

	CButtonST	m_chk_forcedischarge;
	CNewLabel	m_msg_forcedischarge;
	
	//int m_nSupplyDevice;
	int m_nForceDischarge;

	//////////////////////////////////////////////////////////////////////////
	// 시간 관련 변수
	//////////////////////////////////////////////////////////////////////////
	CXPGroupBox	m_group_time_info;
	
	CDigit	m_dgt_time_run;
	CDigit	m_dgt_time_stop;
	CDigit	m_dgt_time_alarm;
	CDigit	m_dgt_time_mtbi;
	
	CGradientStatic	m_label_time_run;
	CGradientStatic	m_label_time_stop;
	CGradientStatic	m_label_time_alarm;
	CGradientStatic	m_label_time_mtbi;

	CNewLabel	m_lblMatCode;

	int mn_time_select;

	// Attributes
private:
	CFont* mp_main_font;				// 폰트 정보 설정 변수

	// 규이리 추가 [2015.03.20]
	CString m_sAcceMatCode;


// Operations
public:
	void OnMain_Display_Main();
	void OnMain_Display_AccyBox();
	void OnMain_Display_EmptyAccyBox();
	void OnMain_Display_ClampAccyBox_Accy();
	void OnMain_Display_NGOut_Accy();
	void OnMain_Display_NGBuffer();
	void OnMain_Display_BufferConv();

	void OnSupplyInfoInit();
	void OnDisplaySupplyInfo();
	void OnDoorInfoInit();
	void OnDisplayDoorInfo();

	void OnSMEMAInfoInit();
	void OnDisplaySMEMAInfo();

	void OnGripperInfoInit();
	void OnDisplayGripperInfo();
	
	void OnMain_Button_Set();
	void OnMain_GroupBox_Set();
	void OnMain_Digital_Count_Set();
	void OnMain_Label_Set();
	void OnMain_Picture_Set();

	void OnMain_TimeInfo_Display();
	void OnMain_Time_Display(int n_state);

	void OnBuildForceDischargeState();

	LRESULT OnMainUpdateReq(WPARAM wParam, LPARAM lParam);

	void GridMerge(UINT nID, int srow, int scol, int nrow, int ncol);
	void GridFont(UINT nID, int row, int col, int size);
	void GridData(UINT nID, int row, int col, CString data);
	void GridControl(UINT nID, int type, int row, int col, int pos);
	void GridColor(UINT nID, int row, int col, COLORREF bk, COLORREF tk);

	// 규이리 추가 [2015.03.20]
	void OnDisp_AcceMatCode();	// 악세사리 매칭 코드 화면 출력

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScreen_Main)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CScreen_Main();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CScreen_Main)
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnChkForceDischarge();
	afx_msg void OnPaint();
	afx_msg void OnButtonNetwork();
	afx_msg void OnButtonSmema();
	afx_msg void OnBmpAccyBoxView();
	afx_msg void OnButtonRealign();
	afx_msg void OnBmpNgBufferAccy1();
	afx_msg void OnBmpNgBufferAccy2();
	afx_msg void OnBmpNgBufferAccy3();
	afx_msg void OnBmpNgBufferAccy4();
	afx_msg void OnBmpBufferConvAccyFront();
	afx_msg void OnBmpBufferConvAccyRear();
	afx_msg void OnBtnBcrReadTest();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCREEN_MAIN_H__4124683A_789C_4F3E_BBCA_278E31E66B20__INCLUDED_)
