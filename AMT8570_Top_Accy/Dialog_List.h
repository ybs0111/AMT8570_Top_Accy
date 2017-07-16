#if !defined(AFX_DIALOG_LIST_H__8F57858E_4E20_4848_B58B_76FBBF404CCC__INCLUDED_)
#define AFX_DIALOG_LIST_H__8F57858E_4E20_4848_B58B_76FBBF404CCC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dialog_List.h : header file
//

#include <string>

#include "Variable.h"

#include "InitDialogBar.h"	// Initial Dialog Bar 생성 클래스
#include "ColorListBox.h"	// 칼라 리스트 박스 컨트롤 클래스
#include "ClockST.h"		// 디지털 시계 컨트롤 클래스
#include "NewLabel.h"		// 칼라 라벨 컨트롤
#include "GradientStatic.h"	// 그라데이션 라벨 컨트롤 클래스

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
	void OnSet_LblCtrl();	// 칼라 라벨 생성
	void OnSet_ClockCtrl();	// 디지털 시계 컨트롤 생성
	void OnSet_TabStops(int nzTabStops);	// 리스트 컨트롤에 Tab Stop 기능 설정
	
	void OnReset_ListContent();	// 리스트 컨트롤에 출력된 문자열 리셋 [500개 기준]
	void OnAdd_NormalMsg();		// [->리스트컨트롤] 정상적인 메시지 추가
	void OnAdd_AbnormalMsg();	// [->리스트컨트롤] 비정상적인 메시지 추가
	void OnAdd_CommSendMsg(int nzPort);	// [->리스트컨트롤] 통신 전송 메시지 추가
	void OnAdd_CommRcvMsg(int nzPort);	// [->리스트컨트롤] 통신 수신 메시지 추가
	
	void OnDisp_EqpInfo();	// 디바이스명 및 EQPID 정보 화면 출력

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
