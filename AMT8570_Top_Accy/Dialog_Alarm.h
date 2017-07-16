#if !defined(AFX_DIALOG_ALARM_H__6FB58774_188C_4576_B791_966B99A73E75__INCLUDED_)
#define AFX_DIALOG_ALARM_H__6FB58774_188C_4576_B791_966B99A73E75__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dialog_Alarm.h : header file
//

#include "Variable.h"
#include "Public_Function.h"

#include "LedButton.h"		// LED 버튼 클래스
#include "BtnST.h"          //칼라 버튼 클래스
#include "XpButton.h"
#include "MacButtons.h"
#include "NewLabel.h"		// 칼라 라벨 클래스
#include "EditEx.h"			// 칼라 에디트 박스 클래스
#include "XPGroupBox.h"		// XP 형태 그룹 박스 클래스
#include "GradientStatic.h" // 그라데이션 라벨 클래스
#include "PictureEx.h"		// GIF 로딩 클래스
#include "FloatST.h"		// 그림자 라벨 클래스
#include "MatrixStatic.h"	// LCD 문자 생성 클래스
#include "BmpImageST.h"		// 알람 비트맵 로딩 클래스

#include "MyJamData.h"		// 알람 출력 정보 로딩 및 알람 메시지 저장 클래스

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
	
	CMyJamData mcls_alarm_load;	// 알람 메시지 로딩 클래스 접근자
	
	// 칼라 에디트 박스 클래스 변수
	CEditEx	m_edit_alarm_title;
	CEditEx	m_edit_alarm_repare;
	
	CString m_sfile_JamData;	// 알람 출력 유형 정보 로딩 파일명 설정
	CString m_sfile_JamAct;		// 알람 조치 사항 정보 로딩 파일명 설정
	
	CString mp_sJamCode[10];

// Operation
public:
	void OnSet_GrpBoxCtrl();// 칼라 그룹박스 생성
	void OnSet_LblCtrl();	// 칼라 라벨 생성
	void OnSet_EditCtrl();	// 칼라 에디트박스 생성
	void OnSet_GifCtrl();	// 이미지[GIF] 로딩
	void OnSet_DigitalCtrl();	// LCD 문자열 출력 컨트롤 생성
	
	void OnSet_AlarmFileName();	// 알람 정보가 저장된 파일명 설정
	int  OnDisp_HappenedAlarm();	// 발생한 알람 정보 화면 표시
	
	void OnSet_JamDispStyle(CString szPath);	// 발생한 알람 표시 형식 설정
	void OnDisp_JamTypeInfo(int nzAlarmState);	// 발생한 알람 종류 화면 표시
	void OnShow_LotEndMsg(int nzState);	// [LOT END] 메시지 화면 출력
	
	void OnReady_SearchPrevJamBtn();	// 알람 조회용 버튼 준비
	void OnToggle_PrevJamBtn(int nzCount);	// 이전 알람 조회용 버튼 보여주기
	void OnDisp_PrevJamBtnCaption(int nzCount);	// 알람 조회용 버튼의 캡션 변경
	CString OnSearch_PrevJamCode(int nzCount);	// 사용자가 선택한 이전 알람 코드 찾기
	void OnDisp_SelectedPrevJam(int nzSelectedPos);	// 선택된 알람 정보 화면 출력
	void OnDisp_JamHappenedDate(int nzHappenedPos);	// 알람 발생 시간 정보 출력

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialog_Alarm)
	public:
	virtual BOOL Create(CWnd* pParentWnd);				// 모달리스 대화 상자 생성 함수 변경 
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
	virtual void OnCancel();  // 알람 화면 ESC 키 누른 경우 대비하여 함수 추가
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
