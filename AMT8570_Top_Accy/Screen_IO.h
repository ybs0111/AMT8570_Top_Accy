#if !defined(AFX_SCREEN_IO_H__9BADC855_B2E6_4CAE_B48F_40A5CC344BCB__INCLUDED_)
#define AFX_SCREEN_IO_H__9BADC855_B2E6_4CAE_B48F_40A5CC344BCB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Screen_IO.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CScreen_IO form view

#include "Variable.h"			// 전역 변수 정의 클래스 추가 
#include "Public_Function.h"	// 모터 보드 및 I/O 보드 관련 클래스
#include "XPGroupBox.h"			// 칼라 그룹 박스 생성 클래스 추가 
#include "sxlogfont.h"
#include "MacButtons.h"
#include "FloatST.h"
#include "EXDigitST.h"			// 디지털 카운터 생성 클래스 추가 
#include "BtnST.h"				// 칼라 버튼 생성 클래스 추가 
#include "Digit.h"

#define TMR_IO_READ				940
#define TMR_IO_CONTINUE			941

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CScreen_IO : public CFormView
{
public:
	CScreen_IO();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CScreen_IO)

// Form Data
public:
	//{{AFX_DATA(CScreen_IO)
	enum { IDD = IDD_SCREEN_IO };
	CMacRadioButton	m_radio_io_cnt_50;
	CMacRadioButton	m_radio_io_cnt_100;
	CMacRadioButton	m_radio_io_cnt_10;
	CMacRadioButton	m_radio_io_cnt_1;
	CFloatST	m_msg_retry_wait;
	CXPGroupBox	m_group_conti_op;
	CMacButton	m_btn_io_e_stop;
	//}}AFX_DATA

// Attributes
public:
	int mn_conti_wait_time[3];
	CDigit	m_dgt_retry_wait;
	
	CFont* mp_io_font;			// 폰트 정보 설정 변수
	
	CString mstr_io_name;		// Logging할 IO의 이름...
	CString mstr_io_act;		// Logging할 IO의 동작상태...
	
	int mn_action_cnt_chk;
	int mn_action_cnt_set;
	int mn_conti_wait;
	int mn_conti_io_no;
	int mn_conti_io_flag;
	int mn_action_step;
	int mn_stop_req;
	int mn_continue;
	int mn_check_slave[50];
	
	int		mn_master;
	int     mn_port;
	int     mn_slave;
	int     mn_list_count;
	CString mstr_io_model;
	CString mstr_io_description[10][8][16];
	CString mstr_io_label[10][8][16];
	CString mstr_io_module[10][8];
	
	int		mn_io_type[10][8][16];
	int		mn_io_num[10][8][16];
	int		mn_io_module[10][8];
	
	TSpread *m_pIoMap;
	TSpread *m_pIoList;

	GridControlAlg m_p_grid;

	int mn_output_info[16];
	int mn_output_onoff[16];

// Operations
public:
	int OnIO_Cylinder_Excution();
	void OnIO_OnInputCheck(int port, int slave);
	void OnIO_Controls_Enable(bool b_state);
	void OnIO_OnOutputData(int port, int slave, int io);
	void OnIO_Slave_Change();
	void OnIO_Digital_Count_Set();
	void OnIO_Name_Label_Set();
	void OnIO_Operation_Count_Set();
	void OnIO_GroupBox_Set();
	void OnIO_OnDrawGrid();
	void OnIO_OnOutputCheck(int port, int slave);
	void OnIO_OnChangeSheet(int port, int num);
	void OnIO_OnSearch_Data();
	void OnIO_Init_List();
	void OnIO_Init_Map();
	void GridMerge(UINT nID, int srow, int scol, int nrow, int ncol);
	void GridFont(UINT nID, int row, int col, int size);
	void GridData(UINT nID, int row, int col, CString data);
	void GridControl(UINT nID, int type, int row, int col, int pos);
	void GridColor(UINT nID, int row, int col, COLORREF bk, COLORREF tk);
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScreen_IO)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CScreen_IO();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CScreen_IO)
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnRadioIoCnt1();
	afx_msg void OnRadioIoCnt10();
	afx_msg void OnRadioIoCnt50();
	afx_msg void OnRadioIoCnt100();
	afx_msg void OnDgtRetryWait();
	afx_msg void OnBtnIoEStop();
	//}}AFX_MSG
	afx_msg void OnCellClick(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCREEN_IO_H__9BADC855_B2E6_4CAE_B48F_40A5CC344BCB__INCLUDED_)
