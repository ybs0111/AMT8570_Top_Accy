#if !defined(AFX_DIALOG_LIST_H__8F57858E_4E20_4848_B58B_76FBBF404CCC__INCLUDED_)
#define AFX_DIALOG_LIST_H__8F57858E_4E20_4848_B58B_76FBBF404CCC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dialog_List.h : header file
//

#include <string>

#include "Variable.h"

#include "InitDialogBar.h"	// Initial Dialog Bar ���� Ŭ����
#include "ColorListBox.h"	// Į�� ����Ʈ �ڽ� ��Ʈ�� Ŭ����
#include "ClockST.h"		// ������ �ð� ��Ʈ�� Ŭ����
#include "NewLabel.h"		// Į�� �� ��Ʈ��
#include "GradientStatic.h"	// �׶��̼� �� ��Ʈ�� Ŭ����

#define MSG_TERM_		10000

/////////////////////////////////////////////////////////////////////////////
// CDialog_List dialog

class CDialog_List : public CInitDialogBar
{
// Construction
public:
	CDialog_List();   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialog_List)
	enum { IDD = IDD_DIALOG_LIST };
	CGradientStatic	m_msg_eqp_id;
	CGradientStatic	m_msg_device_name;
	CNewLabel	m_label_eqp_id;
	CNewLabel	m_label_device_name;
	CColorListBox	m_list_data;
	CClockST	m_list_clock;
	//}}AFX_DATA

// Attributes
private:
	CFont* mp_fontList;

	long m_lTimeNormal[3];
	long m_lTimeAbnormal[3];
	long m_lTimeCommSend[3];
	long m_lTimeCommRcv[3];
	
	std::string m_sOldMsgNormal;
	std::string mm_sOldMsgAbnormal;
	
	CString m_sOldMsgCommSend;
	CString m_sOldMsgCommRcv;
	
// Operations
private:
	void OnSet_LblCtrl();	// Į�� �� ����
	void OnSet_ClockCtrl();	// ������ �ð� ��Ʈ�� ����
	void OnSet_TabStops(int nzTabStops);	// ����Ʈ ��Ʈ�ѿ� Tab Stop ��� ����
	
	void OnReset_ListContent();	// ����Ʈ ��Ʈ�ѿ� ��µ� ���ڿ� ���� [500�� ����]
	void OnAdd_NormalMsg();		// [->����Ʈ��Ʈ��] �������� �޽��� �߰�
	void OnAdd_AbnormalMsg();	// [->����Ʈ��Ʈ��] ���������� �޽��� �߰�
	void OnAdd_CommSendMsg(int nzPort);	// [->����Ʈ��Ʈ��] ��� ���� �޽��� �߰�
	void OnAdd_CommRcvMsg(int nzPort);	// [->����Ʈ��Ʈ��] ��� ���� �޽��� �߰�
	
	void OnDisp_EqpInfo();	// ����̽��� �� EQPID ���� ȭ�� ���

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialog_List)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialog_List)
	virtual BOOL OnInitDialogBar();
	afx_msg void OnDestroy();
	afx_msg void OnDblclkListData();
	//}}AFX_MSG
	afx_msg LRESULT OnListDataUpdate(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOG_LIST_H__8F57858E_4E20_4848_B58B_76FBBF404CCC__INCLUDED_)
