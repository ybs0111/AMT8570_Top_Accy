#if !defined(AFX_SCREEN_SET_MAINTENANCE_H__1A368EE5_799F_4A80_B357_D7E095518DED__INCLUDED_)
#define AFX_SCREEN_SET_MAINTENANCE_H__1A368EE5_799F_4A80_B357_D7E095518DED__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Screen_Set_Maintenance.h : header file
//
#include "Variable.h"		// ���� ���� ���� Ŭ���� �߰�
#include "Public_Function.h"
#include "BtnST.h"          // Į�� ��ư ���� Ŭ���� �߰�  
#include "FloatST.h"
#include "GradientStatic.h" // �׶��̼� Į�� �ؽ�Ʈ �ڽ� ���� Ŭ���� �߰� 
#include "EditEx.h"			// Į�� ����Ʈ �ڽ� ���� Ŭ���� �߰� 
#include "XPGroupBox.h"		// Į�� �׷� �ڽ� ���� Ŭ���� �߰� 
#include "SxLogFont.h"
#include "MacButtons.h"
#include "XpButton.h"		// XP Į�� ��ư Ŭ����
#include "UniButtonEx.h"	// ���� ��ư Ŭ����
#include "EXDigitST.h"		// ������ ī���� ���� Ŭ���� �߰� 
#include "LedButton.h"				// LED ��ư ���� Ŭ���� �߰� 

// ******************************************************************************
// ���� ���� �ε� �� ���� Ŭ����                                                 
// ******************************************************************************
#include "MyBasicData.h"
// ******************************************************************************

// ******************************************************************************
// Ÿ�� ���� ON/OFF Į�� ����                                                    
// ******************************************************************************
#define R_ON	(RGB(255, 0, 0))
#define R_OFF	(RGB(162, 0, 0))
#define Y_ON	(RGB(255, 255, 0))
#define Y_OFF	(RGB(162, 162, 0))
#define G_ON	(RGB(0, 255, 0))
#define G_OFF	(RGB(0, 162, 0))

#define FLICK	(RGB(190, 220, 255))
// ******************************************************************************
/////////////////////////////////////////////////////////////////////////////
// CScreen_Set_Maintenance form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif
#include "Ctlbd_Variable.h"	// Added by ClassView

class CScreen_Set_Maintenance : public CFormView
{
public:
	CScreen_Set_Maintenance();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CScreen_Set_Maintenance)

// Form Data
public:
	//{{AFX_DATA(CScreen_Set_Maintenance)
	enum { IDD = IDD_SCREEN_SET_MAINTENANCE };
	CXPGroupBox	m_group_pitch;
	CXPGroupBox	m_group_network_addr;
	CMacButton	m_btn_network_load;
	CMacButton	m_btn_network_modify;
	CMacButton	m_btn_password_change_level2;
	CMacButton	m_btn_password_change_level1;
	CMacButton	m_btn_data_backup;
	CMacButton	m_btn_step_save;
	CXPGroupBox	m_group_lamp_change_time;
	CStatic	m_lamp_stop;
	CStatic	m_lamp_selfcheck;
	CStatic	m_lamp_warning;
	CStatic	m_lamp_run;
	CStatic	m_lamp_lotend;
	CStatic	m_lamp_lock;
	CStatic	m_lamp_initial;
	CStatic	m_lamp_alarm;
	CXPGroupBox	m_group_towerlamp;
	CXPGroupBox	m_group_password_change;
	CXPGroupBox	m_group_lamp_warning;
	CXPGroupBox	m_group_lamp_stop;
	CXPGroupBox	m_group_lamp_selfcheck;
	CXPGroupBox	m_group_lamp_run;
	CXPGroupBox	m_group_lamp_lotend;
	CXPGroupBox	m_group_lamp_lock;
	CXPGroupBox	m_group_lamp_initial;
	CXPGroupBox	m_group_lamp_flicker_time;
	CXPGroupBox	m_group_lamp_alarm;
	CXPGroupBox	m_group_data_backup;
	CXPGroupBox	m_group_buzzer;
	CButtonST	m_chk_buzzer;
	CXPButton	m_btn_maintenance_apply;
	//}}AFX_DATA

// Attributes
public:
	enum { IDC_LAMP_BASE	= 3000, 
		   IDC_LAMP_ROOF	= 3010, 
		   IDC_LAMP_POLE	= 3020, 
		   IDC_LAMP_LOWER	= 3030, 
		   IDC_LAMP_UPPER	= 3040, 
		   IDC_LAMP_SEP1	= 3050, 
		   IDC_LAMP_SEP2	= 3060, 
		   IDC_LAMP_SEP3	= 3070, 
		   ID_LAMP_RLAMP	= 3080, 
		   ID_LAMP_RFLICKER	= 3090, 
		   ID_LAMP_YLAMP	= 3100, 
		   ID_LAMP_YFLICKER	= 3110, 
		   ID_LAMP_GLAMP	= 3120, 
		   ID_LAMP_GFLICKER	= 3130, 
	};

	enum { STOP			= 0,
		   RUN			= 1,
		   ALARM		= 2,
		   LOTEND		= 3,
		   INIT			= 4,
		   WARNING		= 5,
		   LOCK			= 6,
		   SELFCHECK	= 7
	};

	CFont* mp_maintenance_font;			// ��Ʈ ���� ���� ����
	CFont* mp_combo_font;

	CMyBasicData     mcls_maintenance;	// �⺻ ���� ���� �ε� �� ���� Ŭ����

	GridControlAlg m_pGrid;

	// **************************************************************************
	// ������ ī���� Ŭ���� ���� ����                                            
	// **************************************************************************
	CEXDigitST  m_dgt_flicker_time;
	CEXDigitST  m_dgt_change_time;

	int	mn_buzzer_mode[2];

	int mn_lamp_flicker_time[2];
	int mn_lamp_change_time[2];

	double md_pitch_data[15][2];
	
	CMyBasicData	m_cBasic;

private:
	BOOL mb_first;     // Ÿ�� ���� ó�� ���� ���� �÷���
	CRect mr_area[8];  // ��Ʈ�� ���� ũ�� ���� ����

	int mn_r_lamp[2][8];  // RED ���� ���� �÷���		[0:OFF    1:ON    2:FLICKER]
	int mn_y_lamp[2][8];  // YELLOW ���� ���� �÷���	[0:OFF    1:ON    2:FLICKER]
	int mn_g_lamp[2][8];  // GREEN ���� ���� �÷���		[0:OFF    1:ON    2:FLICKER]
	int mn_pos_x[8][2], mn_pos_y[8][2];  // ��Ʈ�� ��ġ ���� ����
	int mn_y_diff[8], mn_y_space[8];     // ��� ���� ���� ����

	CUniButtonEx mp_btn_base[8], mp_btn_roof[8], mp_btn_pole[8], mp_btn_lower[8], mp_btn_upper[8];
	CUniButtonEx mp_btn_step1[8], mp_btn_step2[8], mp_btn_step3[8], mp_btn_r_lamp[8], mp_btn_y_lamp[8], mp_btn_g_lamp[8];

// Operations
public:
	void OnMaintenance_Init_Pitch();
	int ExCopyFile(CString strFrom, CString strTo);
	BOOL CopyFolder(CString& strFrom, CString& strTo);
	void OnMaintenance_Step_Save();
	void OnMaintenance_LogFile_Create();
	void OnMaintenance_Digital_Count_Set();
	void OnMaintenance_File_Backup(CString strTargetFile, CString strCreateFile, CString strBackupFolder, CString strBackupFile, BOOL b_over_write);
	void OnMaintenance_Lamp_Change();
	void OnMaintenance_Label_Set();
	void OnMaintenance_Data_Recovery();
	int OnMaintenance_Data_Comparison();
	void OnMaintenance_Data_Display();
	void OnMaintenance_ListBar_Info_Send();
	void OnMaintenance_Data_BackUp();
	void OnMaintenance_Data_Apply();
	void OnMaintenance_EditBox_Set();
	void OnMaintenance_Buzzer_Change();
	void OnMaintenance_Button_Set();
	void OnMaintenance_Data_Set();
	void OnMaintenance_Green_State_Change(int idx);
	void OnMaintenance_Yellow_State_Change(int idx);
	void OnMaintenance_Red_State_Change(int idx);
	void DrawButton(int i);
	void OnMaintenance_GroupBox_Set();
	void GridMerge(UINT nID, int srow, int scol, int nrow, int ncol);
	void GridFont(UINT nID, int row, int col, int size);
	void GridData(UINT nID, int row, int col, CString data);
	void GridControl(UINT nID, int type, int row, int col, int pos);
	void GridColor(UINT nID, int row, int col, COLORREF bk, COLORREF tk);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScreen_Set_Maintenance)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CScreen_Set_Maintenance();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CScreen_Set_Maintenance)
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnDgtFlickerTime();
	afx_msg void OnChkBuzzer();
	afx_msg void OnBtnDataBackup();
	afx_msg void OnBtnPasswordChangeLevle1();
	afx_msg void OnBtnPasswordChangeLevle2();
	afx_msg void OnBtnMaintenanceApply();
	afx_msg void OnDgtChangeTime();
	afx_msg void OnBtnStepSave();
	afx_msg void OnSetfocusEditEcEquipId();
	afx_msg void OnBtnNetworkAddrLoad();
	afx_msg void OnBtnNetworkAddrModify();
	//}}AFX_MSG
	afx_msg void OnRLampClick(UINT);
	afx_msg void OnYLampClick(UINT);
	afx_msg void OnGLampClick(UINT);
	afx_msg void OnCellClick(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMaintenance_Info_Apply(WPARAM wParam, LPARAM lParam);  // Tower Lamp ȭ�� ���� ���� ��û �޽���
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCREEN_SET_MAINTENANCE_H__1A368EE5_799F_4A80_B357_D7E095518DED__INCLUDED_)
