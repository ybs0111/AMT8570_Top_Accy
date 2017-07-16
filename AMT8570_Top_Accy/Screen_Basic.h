#if !defined(AFX_SCREEN_BASIC_H__08E76C42_8480_4E17_95BE_F37439F994FF__INCLUDED_)
#define AFX_SCREEN_BASIC_H__08E76C42_8480_4E17_95BE_F37439F994FF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Screen_Basic.h : header file
//
#include "Variable.h"		// 전역 변수 정의 클래스 추가 
#include "Public_Function.h"
#include "NewLabel.h"		// 칼라 텍스트 박스 생성 클래스 추가 
#include "editex.h"			// 칼라 에디트 박스 생성 클래스 추가 
#include "BtnST.h"          // 칼라 버튼 생성 클래스 추가 
#include "MacButtons.h"
#include "XPGroupBox.h"		// 칼라 그룹 박스 생성 클래스 추가 
#include "ListBoxST.h"      // 칼라 아이콘 리스트 박스 클래스

//#define _GRID_ITEM_CNT		18
// 규이리 수정 [2015.03.25]
#define GRID_ITEM_CNT_			20

/////////////////////////////////////////////////////////////////////////////
// CScreen_Basic form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CScreen_Basic : public CFormView
{
//protected:  // 수정함 [2014.08.26]
public:
	CScreen_Basic();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CScreen_Basic)

// Form Data
public:
	//{{AFX_DATA(CScreen_Basic)
	enum { IDD = IDD_SCREEN_BASIC };
	CButtonST	m_chk_slop_variable;
	CXPGroupBox	m_group_slop_variable;
	CMacButton	m_btn_basic_apply;
	CListBoxST	m_list_device_type;
	CMacButton	m_btn_dvc_create;
	CMacButton	m_btn_dvc_delete;
	CXPGroupBox	m_group_set_device;
	CXPGroupBox	m_group_basic;
	//}}AFX_DATA

// Attributes
public:
	CFont* mp_basic_font;			// 폰트 정보 설정 변수
	
	CMyBasicData    mcls_basic;		// 기본 셋팅 정보 로딩 및 저장 클래스
	
	int mn_count;            		// 파일 목록 갯수 저장 변수
	// **************************************************************************
	// 칼라 에디트 박스 클래스 변수 선언                                         
	// **************************************************************************
	CEditEx	 m_edit_device_type;
	
	int m_nModeDevice[2];						// With, With Out, No Tray
	int m_nModeNetwork[2];						// Online, Offline
	
	// **************************************************************************
	// 리스트 박스 컨트롤 정보 저장 클래스 변수 선언                             
	// **************************************************************************
	CImageList mp_device_image_list;	// 이미지 리스트 컨트롤 선언
	
	CString m_sDeviceName[2];		// 선택된 디바이스 종류 저장 변수
	CString mstr_temp_device;			// 선택된 디바이스 종류 임시 저장 변수
	CString mstr_load_file;				// 로딩할 파일에 대한 [폴더]+[파일명] 저장 변수
	int mn_device_name;					// 선택된 디바이스의 리스트 위치 저장 변수
	
	CString mstr_new_device;			// 새로 생성할 디바이스의 이름.
	CString str_select_device_name;
	// **************************************************************************
	
	CString m_sEquipNo[2];
	CString m_sEquipModel[2];
	
	// 백상현 추가 [2014.08.19]
	GridControlAlg m_p_grid;
	TSpread *m_grid_basic_setting;
	
	int m_nMode_SupplyMode[2];		// Master Slave 모드

	int m_nMode_SupplyDevice[2];		// SupplyDevice 모드
	int m_nMode_SMEMAUsing[2];		// SMEMA 모드
	int m_nMode_GripRetryCNT[2];

	// Grip, Vac 사용 모드 추가
	int m_nMode_VacUsing[2];		// VacUsing 모드
	int m_nMode_GripUsing[2];		// GripUsing 모드
	int m_nMode_PlaceType[2];

	int m_nMode_Slop_Variable[2];

// Operations
public:
	void OnBasic_Button_Set();
	void OnBasic_GroupBox_Set();
	void OnBasic_EditBox_Set();
	void OnBasic_Data_Set();
	
	void OnBasic_Data_Apply();
	void OnBasic_Data_Display();
	void OnBasic_Data_BackUp();	
	int OnBasic_Data_Comparison();
	void OnBasic_LogFile_Create();
	int OnBasic_Input_Device_Check(CString str_temp_device);
	void OnBasic_Device_Display(CString str_dvc);
	BOOL AddItem(int nIndex, WIN32_FIND_DATA *pfd);
	void OnBasic_Device_Focus_Set();
	int OnBasic_Init_List(LPCTSTR pszPath);
	void OnBasic_Create_ImageList();
	void OnBasic_DeviceData_Apply();
	int OnBasic_New_Device_Check();
	void OnBasic_Create_Device();
	int OnBasic_Input_Data_Check();
	void OnBasic_DeviceDataChange();
	
	void OnSave_TotalData();	// [저장] 디바이스 변경에 따른 각종 설정 정보
	
	void OnBasicDataGridInit();  // [초기화] 셋팅 데이터 표시용 그리드
	void OnBasicDataGridView();  // 그리드에 셋팅 정보 표시
	
	void OnWithModeGridView();		// WihtMode 그리드 정보 표시 
	void OnSupplyModeModeGridView();	// SupplyMode Mode 그리드 정보 표시
	void OnSupplyDeviceModeGridView();	// SupplyDevice Mode 그리드 정보 표시
	void OnSMEMAModeGridView();
	void OnNetworkModeGridView();

	void OnVacUsingModeGridView();
	void OnGripUsingModeGridView();
	void OnGridView_PlaceType();	// 자재 내려놓는 모드 [1:4개씩, 0:2개씩]

	void OnBasic_Slop_Pitch_Change();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScreen_Basic)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CScreen_Basic();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CScreen_Basic)
	afx_msg void OnDestroy();
	afx_msg void OnBtnDvcDelete();
	afx_msg void OnBtnDvcCreate();
	afx_msg void OnSelchangeListDeviceType();
	afx_msg void OnBtnBasicApply();
	afx_msg void OnChkSlopVariable();
	//}}AFX_MSG
	afx_msg LRESULT OnBasic_Info_Apply(WPARAM wParam, LPARAM lParam);  // BASIC 화면 정보 갱신 요청 
	afx_msg void OnCell_L_Click(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCREEN_BASIC_H__08E76C42_8480_4E17_95BE_F37439F994FF__INCLUDED_)
