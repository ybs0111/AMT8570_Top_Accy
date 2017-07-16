#if !defined(AFX_SCREEN_BASIC_H__08E76C42_8480_4E17_95BE_F37439F994FF__INCLUDED_)
#define AFX_SCREEN_BASIC_H__08E76C42_8480_4E17_95BE_F37439F994FF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Screen_Basic.h : header file
//
#include "Variable.h"		// ���� ���� ���� Ŭ���� �߰� 
#include "Public_Function.h"
#include "NewLabel.h"		// Į�� �ؽ�Ʈ �ڽ� ���� Ŭ���� �߰� 
#include "editex.h"			// Į�� ����Ʈ �ڽ� ���� Ŭ���� �߰� 
#include "BtnST.h"          // Į�� ��ư ���� Ŭ���� �߰� 
#include "MacButtons.h"
#include "XPGroupBox.h"		// Į�� �׷� �ڽ� ���� Ŭ���� �߰� 
#include "ListBoxST.h"      // Į�� ������ ����Ʈ �ڽ� Ŭ����

//#define _GRID_ITEM_CNT		18
// ���̸� ���� [2015.03.25]
#define GRID_ITEM_CNT_			20

/////////////////////////////////////////////////////////////////////////////
// CScreen_Basic form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CScreen_Basic : public CFormView
{
//protected:  // ������ [2014.08.26]
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
	CFont* mp_basic_font;			// ��Ʈ ���� ���� ����
	
	CMyBasicData    mcls_basic;		// �⺻ ���� ���� �ε� �� ���� Ŭ����
	
	int mn_count;            		// ���� ��� ���� ���� ����
	// **************************************************************************
	// Į�� ����Ʈ �ڽ� Ŭ���� ���� ����                                         
	// **************************************************************************
	CEditEx	 m_edit_device_type;
	
	int m_nModeDevice[2];						// With, With Out, No Tray
	int m_nModeNetwork[2];						// Online, Offline
	
	// **************************************************************************
	// ����Ʈ �ڽ� ��Ʈ�� ���� ���� Ŭ���� ���� ����                             
	// **************************************************************************
	CImageList mp_device_image_list;	// �̹��� ����Ʈ ��Ʈ�� ����
	
	CString m_sDeviceName[2];		// ���õ� ����̽� ���� ���� ����
	CString mstr_temp_device;			// ���õ� ����̽� ���� �ӽ� ���� ����
	CString mstr_load_file;				// �ε��� ���Ͽ� ���� [����]+[���ϸ�] ���� ����
	int mn_device_name;					// ���õ� ����̽��� ����Ʈ ��ġ ���� ����
	
	CString mstr_new_device;			// ���� ������ ����̽��� �̸�.
	CString str_select_device_name;
	// **************************************************************************
	
	CString m_sEquipNo[2];
	CString m_sEquipModel[2];
	
	// ����� �߰� [2014.08.19]
	GridControlAlg m_p_grid;
	TSpread *m_grid_basic_setting;
	
	int m_nMode_SupplyMode[2];		// Master Slave ���

	int m_nMode_SupplyDevice[2];		// SupplyDevice ���
	int m_nMode_SMEMAUsing[2];		// SMEMA ���
	int m_nMode_GripRetryCNT[2];

	// Grip, Vac ��� ��� �߰�
	int m_nMode_VacUsing[2];		// VacUsing ���
	int m_nMode_GripUsing[2];		// GripUsing ���
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
	
	void OnSave_TotalData();	// [����] ����̽� ���濡 ���� ���� ���� ����
	
	void OnBasicDataGridInit();  // [�ʱ�ȭ] ���� ������ ǥ�ÿ� �׸���
	void OnBasicDataGridView();  // �׸��忡 ���� ���� ǥ��
	
	void OnWithModeGridView();		// WihtMode �׸��� ���� ǥ�� 
	void OnSupplyModeModeGridView();	// SupplyMode Mode �׸��� ���� ǥ��
	void OnSupplyDeviceModeGridView();	// SupplyDevice Mode �׸��� ���� ǥ��
	void OnSMEMAModeGridView();
	void OnNetworkModeGridView();

	void OnVacUsingModeGridView();
	void OnGripUsingModeGridView();
	void OnGridView_PlaceType();	// ���� �������� ��� [1:4����, 0:2����]

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
	afx_msg LRESULT OnBasic_Info_Apply(WPARAM wParam, LPARAM lParam);  // BASIC ȭ�� ���� ���� ��û 
	afx_msg void OnCell_L_Click(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCREEN_BASIC_H__08E76C42_8480_4E17_95BE_F37439F994FF__INCLUDED_)
