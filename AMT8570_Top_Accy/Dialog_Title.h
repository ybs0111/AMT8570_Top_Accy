#if !defined(AFX_DIALOG_TITLE_H__82EC6C02_86F2_4355_8A81_612CDAB12FE2__INCLUDED_)
#define AFX_DIALOG_TITLE_H__82EC6C02_86F2_4355_8A81_612CDAB12FE2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dialog_Title.h : header file
//

#include "InitDialogBar.h"	// Initial Dialog Bar Ŭ����

#include "Variable.h"
#include "MenuHead.h"

#include "NewLabel.h"		// Į�� �ؽ�Ʈ �ڽ� Ŭ����
#include "GradientStatic.h"	// �׶��̼� Į�� �ؽ�Ʈ �ڽ� Ŭ����
#include "PictureEx.h"		// GIF �ε� Ŭ����
#include "VLabel.h"			// Į�� �ؽ�Ʈ �ڽ� Ŭ����

#include <process.h>
#include <afxmt.h>
// CCriticalSecton���� CSingleLock�� ����ϱ� ���� �߰�
//#include "Afxmt.h"

/////////////////////////////////////////////////////////////////////////////
// CDialog_Title dialog

class CDialog_Title : public CInitDialogBar
{
// Construction
public:
	//CDialog_Title(CWnd* pParent = NULL);   // standard constructor
	CDialog_Title();	// Initial Dialog Bar Ŭ�������� ���
	
// Dialog Data
	//{{AFX_DATA(CDialog_Title)
	enum { IDD = IDD_DIALOG_TITLE };
	CGradientStatic	m_title_bcr_mode;
	CGradientStatic	m_title_cnnt_eqp2main;
	CGradientStatic	m_title_level_mode;
	CGradientStatic	m_title_device_mode;
	CGradientStatic	m_msg_status_info;
	CNewLabel	m_msg_form_name;
	CPictureEx	m_gif_title_logo;
	//}}AFX_DATA

	// Attributes
private:
	CFont* mp_title_font;	// ��Ʈ ����

	//==============================================================//
	// ������ ���� ���� ����
	//==============================================================//
	CCriticalSection m_csMotRead;  // ����ȭ ��ü



	BOOL m_bDispThreadFlag;		// ������ ���� �÷���
	HANDLE m_hDispThreadHandle;	// ������ �ڵ�
	UINT m_unDispThreadID;		// ������ ID
	//--------------------------------------------------------------//

	// ����� �߰� [2014.08.29]
	int m_nEQP_Status;
	int m_nDVC_Mode;
	int m_nLevel_Mode;
	int m_nBCR_Mode;
	int m_nState_Eqp2Main;
	
	// Operations
public:
	void OnSetLabel();		// Į�� �� ����
	void OnSetPicture();	// �̹���[GIF] �ε�
	
	void OnDisplayFormName(int nzViewID);	// ��µ� ȭ�� ��Ī ǥ��
	void OnDisplayStatus(int nzStatus);		// ���� ���� ǥ��
	void OnDisplayLevelStatus();			// ���� Level ���� ǥ��

	void OnBuildEQPStatus();				// ���� ���� ǥ��
	void OnBuildDeviceMode();				// �۾����� ����̽� ���� ǥ��
	void OnBuildBCRMode();					// �۾����� BCR ���� ǥ��
	void OnBuildMainConnect();				// Main ��� ���� ǥ��

	//==============================================================//
	// ������ ���� �Լ���
	//==============================================================//
	BOOL OnCreateThread();	// ������ ����
	static unsigned int __stdcall DispThreadProc(void *pParam);
	void OnDispInfo();
	void OnCloseThread();	// ������ ������ ����
	//--------------------------------------------------------------//



// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialog_Title)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialog_Title)
	virtual BOOL OnInitDialogBar();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	afx_msg  LRESULT OnStatusDataUpdate(WPARAM wParam,LPARAM lParam);  // UI ���� ���� ��û ����� ���� �޽���
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOG_TITLE_H__82EC6C02_86F2_4355_8A81_612CDAB12FE2__INCLUDED_)
