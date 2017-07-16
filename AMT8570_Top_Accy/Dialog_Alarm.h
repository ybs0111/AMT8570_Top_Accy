#if !defined(AFX_DIALOG_ALARM_H__6FB58774_188C_4576_B791_966B99A73E75__INCLUDED_)
#define AFX_DIALOG_ALARM_H__6FB58774_188C_4576_B791_966B99A73E75__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dialog_Alarm.h : header file
//

#include "Variable.h"
#include "Public_Function.h"

#include "LedButton.h"		// LED ��ư Ŭ����
#include "BtnST.h"          //Į�� ��ư Ŭ����
#include "XpButton.h"
#include "MacButtons.h"
#include "NewLabel.h"		// Į�� �� Ŭ����
#include "EditEx.h"			// Į�� ����Ʈ �ڽ� Ŭ����
#include "XPGroupBox.h"		// XP ���� �׷� �ڽ� Ŭ����
#include "GradientStatic.h" // �׶��̼� �� Ŭ����
#include "PictureEx.h"		// GIF �ε� Ŭ����
#include "FloatST.h"		// �׸��� �� Ŭ����
#include "MatrixStatic.h"	// LCD ���� ���� Ŭ����
#include "BmpImageST.h"		// �˶� ��Ʈ�� �ε� Ŭ����

#include "MyJamData.h"		// �˶� ��� ���� �ε� �� �˶� �޽��� ���� Ŭ����

/////////////////////////////////////////////////////////////////////////////
// CDialog_Alarm dialog

class CDialog_Alarm : public CDialog
{
// Construction
public:
	CDialog_Alarm(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialog_Alarm)
	enum { IDD = IDD_DIALOG_ALARM };
	CFloatST	m_msg_alarm_time;
	CNewLabel	m_data_alarm_time;
	CMatrixStatic	m_msg_lot_end;
	CFloatST	m_msg_alarm_type;
	CFloatST	m_msg_alarm_title;
	CFloatST	m_msg_alarm_repare;
	CFloatST	m_msg_alarm_code;
	CBmpImageST	m_image_alarm;
	CXPGroupBox	m_group_prev_alarm;
	CXPGroupBox	m_group_alarm_info;
	CPictureEx	m_gif_alarm;
	CNewLabel	m_data_alarm_type;
	CNewLabel	m_data_alarm_code;
	CMacButton	m_btn_prev_alarm_10;
	CMacButton	m_btn_prev_alarm_9;
	CMacButton	m_btn_prev_alarm_8;
	CMacButton	m_btn_prev_alarm_7;
	CMacButton	m_btn_prev_alarm_6;
	CMacButton	m_btn_prev_alarm_5;
	CMacButton	m_btn_prev_alarm_4;
	CMacButton	m_btn_prev_alarm_3;
	CMacButton	m_btn_prev_alarm_2;
	CMacButton	m_btn_prev_alarm_1;
	CMacButton	m_btn_buzzer_off;
	CXPButton	m_btn_alarm_ok;
	//}}AFX_DATA

// Attributes
private:
	CFont* mp_fontAlarm;
	
	CMyJamData mcls_alarm_load;	// �˶� �޽��� �ε� Ŭ���� ������
	
	// Į�� ����Ʈ �ڽ� Ŭ���� ����
	CEditEx	m_edit_alarm_title;
	CEditEx	m_edit_alarm_repare;
	
	CString m_sfile_JamData;	// �˶� ��� ���� ���� �ε� ���ϸ� ����
	CString m_sfile_JamAct;		// �˶� ��ġ ���� ���� �ε� ���ϸ� ����
	
	CString mp_sJamCode[10];

// Operation
public:
	void OnSet_GrpBoxCtrl();// Į�� �׷�ڽ� ����
	void OnSet_LblCtrl();	// Į�� �� ����
	void OnSet_EditCtrl();	// Į�� ����Ʈ�ڽ� ����
	void OnSet_GifCtrl();	// �̹���[GIF] �ε�
	void OnSet_DigitalCtrl();	// LCD ���ڿ� ��� ��Ʈ�� ����
	
	void OnSet_AlarmFileName();	// �˶� ������ ����� ���ϸ� ����
	int  OnDisp_HappenedAlarm();	// �߻��� �˶� ���� ȭ�� ǥ��
	
	void OnSet_JamDispStyle(CString szPath);	// �߻��� �˶� ǥ�� ���� ����
	void OnDisp_JamTypeInfo(int nzAlarmState);	// �߻��� �˶� ���� ȭ�� ǥ��
	void OnShow_LotEndMsg(int nzState);	// [LOT END] �޽��� ȭ�� ���
	
	void OnReady_SearchPrevJamBtn();	// �˶� ��ȸ�� ��ư �غ�
	void OnToggle_PrevJamBtn(int nzCount);	// ���� �˶� ��ȸ�� ��ư �����ֱ�
	void OnDisp_PrevJamBtnCaption(int nzCount);	// �˶� ��ȸ�� ��ư�� ĸ�� ����
	CString OnSearch_PrevJamCode(int nzCount);	// ����ڰ� ������ ���� �˶� �ڵ� ã��
	void OnDisp_SelectedPrevJam(int nzSelectedPos);	// ���õ� �˶� ���� ȭ�� ���
	void OnDisp_JamHappenedDate(int nzHappenedPos);	// �˶� �߻� �ð� ���� ���

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialog_Alarm)
	public:
	virtual BOOL Create(CWnd* pParentWnd);				// ��޸��� ��ȭ ���� ���� �Լ� ���� 
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialog_Alarm)
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	virtual void OnCancel();  // �˶� ȭ�� ESC Ű ���� ��� ����Ͽ� �Լ� �߰�
	afx_msg void OnBtnBuzzerOff();
	afx_msg void OnBtnPrevAlarm1();
	afx_msg void OnBtnPrevAlarm2();
	afx_msg void OnBtnPrevAlarm3();
	afx_msg void OnBtnPrevAlarm4();
	afx_msg void OnBtnPrevAlarm5();
	afx_msg void OnBtnPrevAlarm6();
	afx_msg void OnBtnPrevAlarm7();
	afx_msg void OnBtnPrevAlarm8();
	afx_msg void OnBtnPrevAlarm9();
	afx_msg void OnBtnPrevAlarm10();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOG_ALARM_H__6FB58774_188C_4576_B791_966B99A73E75__INCLUDED_)
