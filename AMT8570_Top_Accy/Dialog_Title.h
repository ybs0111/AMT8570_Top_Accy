#if !defined(AFX_DIALOG_TITLE_H__82EC6C02_86F2_4355_8A81_612CDAB12FE2__INCLUDED_)
#define AFX_DIALOG_TITLE_H__82EC6C02_86F2_4355_8A81_612CDAB12FE2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dialog_Title.h : header file
//

#include "InitDialogBar.h"	// Initial Dialog Bar 클래스

#include "Variable.h"
#include "MenuHead.h"

#include "NewLabel.h"		// 칼라 텍스트 박스 클래스
#include "GradientStatic.h"	// 그라데이션 칼라 텍스트 박스 클래스
#include "PictureEx.h"		// GIF 로딩 클래스
#include "VLabel.h"			// 칼라 텍스트 박스 클래스

#include <process.h>
#include <afxmt.h>
// CCriticalSecton으로 CSingleLock을 사용하기 위해 추가
//#include "Afxmt.h"

/////////////////////////////////////////////////////////////////////////////
// CDialog_Title dialog

class CDialog_Title : public CInitDialogBar
{
// Construction
public:
	//CDialog_Title(CWnd* pParent = NULL);   // standard constructor
	CDialog_Title();	// Initial Dialog Bar 클래스에서 상속
	
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
	CFont* mp_title_font;	// 폰트 변수

	//==============================================================//
	// 쓰레드 관련 변수 선언
	//==============================================================//
	CCriticalSection m_csMotRead;  // 동기화 객체



	BOOL m_bDispThreadFlag;		// 쓰레드 상태 플래그
	HANDLE m_hDispThreadHandle;	// 쓰레드 핸들
	UINT m_unDispThreadID;		// 쓰레드 ID
	//--------------------------------------------------------------//

	// 백상현 추가 [2014.08.29]
	int m_nEQP_Status;
	int m_nDVC_Mode;
	int m_nLevel_Mode;
	int m_nBCR_Mode;
	int m_nState_Eqp2Main;
	
	// Operations
public:
	void OnSetLabel();		// 칼라 라벨 생성
	void OnSetPicture();	// 이미지[GIF] 로딩
	
	void OnDisplayFormName(int nzViewID);	// 출력된 화면 명칭 표시
	void OnDisplayStatus(int nzStatus);		// 설비 상태 표시
	void OnDisplayLevelStatus();			// 현재 Level 정보 표시

	void OnBuildEQPStatus();				// 설비 상태 표시
	void OnBuildDeviceMode();				// 작업중인 디바이스 정보 표시
	void OnBuildBCRMode();					// 작업중인 BCR 정보 표시
	void OnBuildMainConnect();				// Main 통신 상태 표시

	//==============================================================//
	// 쓰레드 관련 함수들
	//==============================================================//
	BOOL OnCreateThread();	// 쓰레드 생성
	static unsigned int __stdcall DispThreadProc(void *pParam);
	void OnDispInfo();
	void OnCloseThread();	// 생성한 쓰레드 종료
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
	afx_msg  LRESULT OnStatusDataUpdate(WPARAM wParam,LPARAM lParam);  // UI 정보 갱신 요청 사용자 정의 메시지
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOG_TITLE_H__82EC6C02_86F2_4355_8A81_612CDAB12FE2__INCLUDED_)
