// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__2C6D2CE2_883C_498B_9324_12F2734B9EC3__INCLUDED_)
#define AFX_MAINFRM_H__2C6D2CE2_883C_498B_9324_12F2734B9EC3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TrueColorToolBar.h"  // 툴바 클래스

#include "Variable.h"
#include "MyBasicData.h"

#include "Dialog_Title.h"	// 타이틀바 클래스
#include "Dialog_List.h"	// 리스트바 클래스

#include "Thread.h"	// 쓰레드 클래스

#include "Logger.h"	// LOG 클래스
#include "CommonFunc.h"
#include "ExternFunctions.h"

#include "SerialPort.h"	// 시리얼 통신 클래스
//==================================================================//
// 규이리 추가 [2014.08.26]
//==================================================================//
#include "ServerSocket.h"
#include "ClientSocket.h"
//------------------------------------------------------------------//
#include "BcrEvent.h"
#include "MainEvent.h"

//==================================================================//
// Modelless 대화상자 정의
//==================================================================//
#include "Dialog_Alarm.h"
class CDialog_Alarm;

#include "Dialog_Event_Msg.h"

#include "Dialog_Network.h"
#include "Dialog_Smema.h"

//------------------------------------------------------------------//


#define TMR_FILE_CREATE			861
#define TMR_MAIN_REFRESH		862
#define TMR_MAIN_ELEC_CHK		863
#define TMR_NEW_LOT_IN			864
#define TMR_BOARD_LIVE_CHK		865
#define TMR_BCR_SETTING			867
#define TMR_MAIN_CIM_START		868
#define TMR_MAIN				799

// 규이리 추가 [2013.11.23]
#define TMR_CIM_			864
#define TMR_MES_			865

#define TMR_TAMS_			865

#define TOOLBAR_DRAW_BUTTON_WIDTH 32

class CClientSocket;
class CServerSocket;

class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:
	CDialog_Title	m_wndTitleBar;	// 타이틀바 클래스
	CDialog_List	m_wndListBar;	// 리스트바 클래스

	//==============================================================//
	// Modelless 대화상자 포인터 정의
	//==============================================================//
	CDialog_Alarm*	mp_cAlarm;
	CDialog_Event_Msg*	mp_msg_dlg;

	CDialog_Network *mp_cNetwork;
	CDialog_Smema *mp_cSmema;
	//--------------------------------------------------------------//

	//==============================================================//
	// 시리얼 통신 관련 부분
	//==============================================================//
	CSerialPort	m_ports[MAX_PORT];		// 시리얼 포트 인스턴스
	CString mstr_received[MAX_PORT];	// 수신 데이터 임시 저장
	//--------------------------------------------------------------//

	CMyBasicData	m_cBasic;

	//==============================================================//
	// 규이리 추가 [2013.11.12]
	//==============================================================//
	CString m_sLogMsg;
	
	CString m_rcvData[10];
	
	// TCP/IP 클래스 인스턴스 선언
	CServerSocket *m_pServer[10];
	CClientSocket *m_pClient[10];
	//--------------------------------------------------------------//

	CWnd* m_hFindWnd;
	int n_summary_flag;
	int nElectrostaticCheckStep;
	CString strElectCommand;
	int nElectChannel;
	long lElectSerialTime[3];

	int mn_calpella_pos;
	int mn_part;
	int mn_board;
	int mn_power_on_pos;
	int mn_power_on_step;
	int mn_power_off_pos;
	int mn_power_off_step;
	int mn_a_station_step;
	int mn_calpella_display_error;
	long mn_Calpella_Wait[3];
	int mn_calpella_display_step;
	int mn_calpella_display_old_pos;
	int mn_power_off_chk_step;
	int mn_power_off_chk_pos;
	int mn_video_chk_step;
	int mn_video_chk_pos;
	CString mstr_received_calpella[MAX_PORT];    // 수신 데이터 임시 저장 변수

	int mn_heater_write_step;
	CString mstrHeaterWCommand;
	long lTempSerialTime[3];

	int mn_new_lot_in_step;
	long ml_wait_time[3];
	long ml_retry_wait[3];

	int mn_bd_live_chk_step;
	int mn_bd_live_chk_no;

	int mn_lot_name_step;

	int mnQueryStep;

	SYSTEMTIME m_timeLastDay;

	int mn_BarcodeSetValueChangeStep;
	DWORD dw_BarcodeWaitTime;

// Operations
public:
	void OnAlarm_Destroy();
	void OnMain_Thread_Destroy();
	

	void OnSave_TotalDataBeforeExit();	// [전체] 설비 데이터 저장

	void OnCreateMainThread();	// 메인 쓰레드 생성
	void OnKillMainThread();	// 메인 쓰레드 종료

	// 초기에 한번만 호출됨
	void OnReSet_StructVar();
	void OnSet_PathInfo();	// 경로 정보 구조체에 설정 및 폴더 생성
	int  OnIsExistDvcFie();	// 작업할 모델명이 존재하는지 확인





	void OnSwitchToForm(int nForm);
	int  OnIsMenuUsable();	// 메뉴 사용 가능 여부
	
	// 규이리 추가 [2013.11.12]
	int  OnCommandPut2Queue(CString szCommand);  // 수신 메시지를 한 명령어로 구분하여 큐에 저장
	
	void OnSend_BcrRead(int nzPort);	// [EQP->BCR] 바코드 읽기 명령 전송
	void OnPortOpen(int nzPort);	// 통신 포트 [RS-232C] 열기
	void OnPortClose(int nzMode=1000);	// 시리얼 통신 포트 [RS-232C] 닫기

	void OnShow_DlgNetwork();
	void OnKill_DlgNetwork();

	void OnShow_DlgSmema();
	void OnKill_DlgSmema();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CTrueColorToolBar	m_wndToolBar;  // 툴바 클래스

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnNcLButtonDblClk(UINT nHitTest, CPoint point);
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnNcRButtonDblClk(UINT nHitTest, CPoint point);
	afx_msg void OnNcRButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnMain();
	afx_msg void OnBasic();
	afx_msg void OnSetting();
	afx_msg void OnWait();
	afx_msg void OnMotor();
	afx_msg void OnIo();
	afx_msg void OnList();
	afx_msg void OnLevel();
	afx_msg void OnSave();
	afx_msg void OnAlarm();
	afx_msg void OnAbout();
	afx_msg void OnReset();
	afx_msg void OnLock();
	afx_msg void OnExit();
	afx_msg void OnMotTeach();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnMotSpeed();
	afx_msg void OnSetInterface();
	afx_msg void OnSetMaintenance();
	afx_msg void OnAdmin();
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnListOper();
	afx_msg void OnListDataview();
	afx_msg void OnFileEditor();
	//}}AFX_MSG
	afx_msg LRESULT OnViewChangeMode(WPARAM wParam, LPARAM lParam);
	afx_msg void OnDataChangeManagement(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMainMessageEvent(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMainframe_Work(WPARAM wParam, LPARAM lParam);
	afx_msg LONG OnCTSDetected(UINT, LONG);    // RS-232C 포트 이벤트 설정 메시지
	afx_msg LRESULT OnDataSend(WPARAM wParam, LPARAM lParam);  // RS-232C 데이터 송신 메시지
	afx_msg LONG OnCommunication(UINT, LONG);  // RS-232C 수신 메시지
	afx_msg LRESULT OnSerialPort(WPARAM wParam, LPARAM lParam);  // 시리얼 포트 생성/삭제 메시지
	afx_msg LRESULT OnAccept(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCommand_Client_1(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__2C6D2CE2_883C_498B_9324_12F2734B9EC3__INCLUDED_)
