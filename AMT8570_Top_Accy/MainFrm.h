// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__2C6D2CE2_883C_498B_9324_12F2734B9EC3__INCLUDED_)
#define AFX_MAINFRM_H__2C6D2CE2_883C_498B_9324_12F2734B9EC3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TrueColorToolBar.h"  // ���� Ŭ����

#include "Variable.h"
#include "MyBasicData.h"

#include "Dialog_Title.h"	// Ÿ��Ʋ�� Ŭ����
#include "Dialog_List.h"	// ����Ʈ�� Ŭ����

#include "Thread.h"	// ������ Ŭ����

#include "Logger.h"	// LOG Ŭ����
#include "CommonFunc.h"
#include "ExternFunctions.h"

#include "SerialPort.h"	// �ø��� ��� Ŭ����
//==================================================================//
// ���̸� �߰� [2014.08.26]
//==================================================================//
#include "ServerSocket.h"
#include "ClientSocket.h"
//------------------------------------------------------------------//
#include "BcrEvent.h"
#include "MainEvent.h"

//==================================================================//
// Modelless ��ȭ���� ����
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

// ���̸� �߰� [2013.11.23]
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
	CDialog_Title	m_wndTitleBar;	// Ÿ��Ʋ�� Ŭ����
	CDialog_List	m_wndListBar;	// ����Ʈ�� Ŭ����

	//==============================================================//
	// Modelless ��ȭ���� ������ ����
	//==============================================================//
	CDialog_Alarm*	mp_cAlarm;
	CDialog_Event_Msg*	mp_msg_dlg;

	CDialog_Network *mp_cNetwork;
	CDialog_Smema *mp_cSmema;
	//--------------------------------------------------------------//

	//==============================================================//
	// �ø��� ��� ���� �κ�
	//==============================================================//
	CSerialPort	m_ports[MAX_PORT];		// �ø��� ��Ʈ �ν��Ͻ�
	CString mstr_received[MAX_PORT];	// ���� ������ �ӽ� ����
	//--------------------------------------------------------------//

	CMyBasicData	m_cBasic;

	//==============================================================//
	// ���̸� �߰� [2013.11.12]
	//==============================================================//
	CString m_sLogMsg;
	
	CString m_rcvData[10];
	
	// TCP/IP Ŭ���� �ν��Ͻ� ����
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
	CString mstr_received_calpella[MAX_PORT];    // ���� ������ �ӽ� ���� ����

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
	

	void OnSave_TotalDataBeforeExit();	// [��ü] ���� ������ ����

	void OnCreateMainThread();	// ���� ������ ����
	void OnKillMainThread();	// ���� ������ ����

	// �ʱ⿡ �ѹ��� ȣ���
	void OnReSet_StructVar();
	void OnSet_PathInfo();	// ��� ���� ����ü�� ���� �� ���� ����
	int  OnIsExistDvcFie();	// �۾��� �𵨸��� �����ϴ��� Ȯ��





	void OnSwitchToForm(int nForm);
	int  OnIsMenuUsable();	// �޴� ��� ���� ����
	
	// ���̸� �߰� [2013.11.12]
	int  OnCommandPut2Queue(CString szCommand);  // ���� �޽����� �� ��ɾ�� �����Ͽ� ť�� ����
	
	void OnSend_BcrRead(int nzPort);	// [EQP->BCR] ���ڵ� �б� ��� ����
	void OnPortOpen(int nzPort);	// ��� ��Ʈ [RS-232C] ����
	void OnPortClose(int nzMode=1000);	// �ø��� ��� ��Ʈ [RS-232C] �ݱ�

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
	CTrueColorToolBar	m_wndToolBar;  // ���� Ŭ����

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
	afx_msg LONG OnCTSDetected(UINT, LONG);    // RS-232C ��Ʈ �̺�Ʈ ���� �޽���
	afx_msg LRESULT OnDataSend(WPARAM wParam, LPARAM lParam);  // RS-232C ������ �۽� �޽���
	afx_msg LONG OnCommunication(UINT, LONG);  // RS-232C ���� �޽���
	afx_msg LRESULT OnSerialPort(WPARAM wParam, LPARAM lParam);  // �ø��� ��Ʈ ����/���� �޽���
	afx_msg LRESULT OnAccept(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCommand_Client_1(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__2C6D2CE2_883C_498B_9324_12F2734B9EC3__INCLUDED_)
