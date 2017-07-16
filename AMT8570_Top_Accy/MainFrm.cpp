// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "Handler.h"

#include "MainFrm.h"


#include "MenuHead.h"	// 뷰 화면에 대한 ID 정의

#include "Dialog_Menu.h"
#include "Dialog_About.h"
#include "Dialog_Exit.h"
#include "Dialog_Pass_Check.h"
#include "Dialog_Infor.h"
#include "Dialog_Select.h"
#include "Dialog_Message.h"
#include "Dialog_KeyBoard.h"
#include "Dialog_Pass_Check.h"

#include "Public_Function.h"
#include "ComizoaPublic.h"
#include "Cmmsdk.h"
#include "FastechPublic_IO.h"
#include "io.h"
#include "FAS_HSSI.h"

#include "AMTRegistry.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//==================================================================//
// 전역 구조체 선언
//==================================================================//
st_handler_param			st_handler;

st_message_param			st_msg;
st_time_param				st_time;
st_lamp_param				st_lamp;
st_basic_param				st_basic;
st_alarm_param				alarm;
st_dsp_alarm_param			st_alarm;
st_work_param				st_work;
st_motor_param				st_motor[MOT_MAXMOTOR];
st_path_param				st_path;
st_ctlbd_param				st_ctlbd;
st_ctlbd_param				st_reco_ctlbd;  
st_serial_param				st_serial;
st_linearmotion_param		st_linearmot[8];
st_coordmotion_param		st_coordmot[16];

st_time_database			st_timedb;
st_alarm_database			st_alarmdb;
st_part_motor_info			st_motor_info;
st_part_info				st_part;

// yangkyu [2015.03.11]
st_Accy_data				st_accy;

// 소셋 통신 관련 구조체 추가
st_client_param	st_client[10];
st_server_param	st_server[10];

tagIO_PARAM		stIO;			// I/O 명칭 저장 구조체 선언 
st_other_param			st_other;			// 기타 정보 저장 구조체 선언
tagLAMP_PARAM			stLAMP;			// Tower Lamp 화면에 대한 정보 저장 구조체 변수 선언
tagSYNC_PARAM	stSync;
tagWITHOUT_PARAM	stWithoutData;

st_Eqp_Map  st_map;
//==================================================================//

// 규이리 추가 [2014.08.12]
extern CHandlerApp theApp;

// 백상현 추가 [2014.0920]
// 복구 관련 구조체
st_SeqBoxClamp_param			reco_SeqBoxClamp;
st_SeqBoxLifter_param			reco_SeqBoxLifter;
st_SeqBufferAlignConv_param		reco_SeqBuffAlignCV;
st_SeqLoadingConv_param			reco_SeqLoadingCV;
st_SeqXYZRobot_param			reco_SeqXYZRBT;

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_NCLBUTTONDBLCLK()
	ON_WM_NCLBUTTONDOWN()
	ON_WM_NCRBUTTONDBLCLK()
	ON_WM_NCRBUTTONDOWN()
	ON_COMMAND(ID_MAIN, OnMain)
	ON_COMMAND(ID_BASIC, OnBasic)
	ON_COMMAND(ID_SETTING, OnSetting)
	ON_COMMAND(ID_WAIT, OnWait)
	ON_COMMAND(ID_MOTOR, OnMotor)
	ON_COMMAND(ID_IO, OnIo)
	ON_COMMAND(ID_LIST, OnList)
	ON_COMMAND(ID_LEVEL, OnLevel)
	ON_COMMAND(ID_SAVE, OnSave)
	ON_COMMAND(ID_ALARM, OnAlarm)
	ON_COMMAND(ID_ABOUT, OnAbout)
	ON_COMMAND(ID_RESET, OnReset)
	ON_COMMAND(ID_LOCK, OnLock)
	ON_COMMAND(ID_EXIT, OnExit)
	ON_COMMAND(ID_MOT_TEACH, OnMotTeach)
	ON_WM_TIMER()
	ON_COMMAND(ID_MOT_SPEED, OnMotSpeed)
	ON_COMMAND(ID_SET_INTERFACE, OnSetInterface)
	ON_COMMAND(ID_SET_MAINTENANCE, OnSetMaintenance)
	ON_COMMAND(ID_ADMIN, OnAdmin)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_COMMAND(ID_LIST_OPER, OnListOper)
	ON_COMMAND(ID_LIST_DATAVIEW, OnListDataview)
	ON_COMMAND(ID_FILE_EDITOR, OnFileEditor)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_FORM_CHANGE, OnViewChangeMode)
	ON_MESSAGE(WM_MAIN_EVENT, OnMainMessageEvent)
	ON_MESSAGE(WM_MAINFRAME_WORK, OnMainframe_Work)
	ON_MESSAGE(WM_COMM_CTS_DETECTED, OnCTSDetected)
	ON_MESSAGE(WM_DATA_SEND, OnDataSend)	// RS-232C 시리얼 포트 통한 데이터 송신 메시지
	ON_MESSAGE(WM_COMM_RXCHAR, OnCommunication)
	ON_MESSAGE(WM_SERIAL_PORT, OnSerialPort)
	ON_MESSAGE(WM_CLIENT_MSG_1, OnCommand_Client_1)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// 규이리 추가 [2014.09.01]
	// 초기 설비 상태 정보 설정 [MANUL]
	// - 동작 쓰레드가 돌지 못하도록 하기 위함
	// - 초기화 후에 매뉴얼 모드로 전환하지만 일단 이곳에서 플래그 정보 초기화
	st_handler.nModeManual = YES;

	// [초기화 작업 상태, 모션 및 IO 초기화 상태]
	Func.OnReset_FlagStructure();	// 구조체의 플래그 정보 초기화
	Func.OnReset_FlagInitialize();	// [초기화] 각종 변수
	Func.OnReset_FlagInterLock();	// [초기화] 초기화 작업시의 인터락 변수
	Func.OnReset_FlagSyncVar();		// [초기화] SYNC 플래그
	Func.OnReset_FlagLotVar();		// [초기화] LOT 관련 플래그
	Func.OnReset_StructMapInfo();	// [초기화] 구조체의 각종 맵 정보

	//==============================================================//
	// 규이리 추가 [2014.08.26]
	// [초기화] 소켓 관련 변수
	//==============================================================//
	int i=0;
	for(i=0; i<10; i++)
	{
		m_pClient[i] = NULL;
		m_pServer[i] = NULL;
		
		st_client[i].n_connect = NO;
		st_server[i].n_connect = NO;
	}
	//--------------------------------------------------------------//

	// 시리얼 통신 포트 열기
	for(i=0; i<MAX_PORT; i++)
	{
		st_serial.n_connect[i] = NO;
		OnPortOpen(i);	// 통신 포트 [RS-232C] 열기
	}

	// 설비 동작 시간 설정
	st_handler.lTimeTable[0]	= GetCurrentTime();
	st_timedb.mnSectionBuffer	= dSTOP;
	m_timeLastDay				= AMTRegistry::RegReadTime();

	// 모션과 IO의 시뮬레이션 모드 해제
	FAS_IO.mn_simulation_mode	= 0;
	COMI.mn_simulation_mode		= 0;

	OnReSet_StructVar();
}

CMainFrame::~CMainFrame()
{
	// 규이리 추가 [2014.09.01]
	if (mp_cAlarm != NULL)
	{
		delete mp_cAlarm;
	}

}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	// 강제로 화면 해상도 변경
	/*
	if (::GetSystemMetrics(SM_CXSCREEN) != 1280 &&
		::GetSystemMetrics(SM_CYSCREEN) != 1024)
	{
		DEVMODE mode;
		mode.dmBitsPerPel	= 32;  // 32비트 칼라로 변경
		mode.dmPelsWidth	= 1280;
		mode.dmPelsHeight	= 1024;
		mode.dmFields		= DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
		ChangeDisplaySettings(&mode, CDS_FULLSCREEN);
	}
	*/

	// LOG 메시지를 저장할 폴더 생성
	cLOG.OnSetPathLogEvent();  // Log Event 저장할 폴더 생성

	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// 규이리 위치 이동 [2014.08.26]
	// 프레임 생성 전이라 핸들을 설정하지 못하는 경우 발생할 수 있어 이동함
	st_handler.hWnd = GetSafeHwnd();  // 메인 프레임에 대한 핸들 정보 설정

	//==============================================================//
	// 트루칼라 툴바 생성
	//==============================================================//
	m_wndToolBar.SetBorders(1, 1, 1, 1);
	
	if (!m_wndToolBar.Create(this, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	
	m_wndToolBar.ModifyStyle(0, TBSTYLE_FLAT);
	m_wndToolBar.LoadTrueColorToolBar(TOOLBAR_DRAW_BUTTON_WIDTH,
		IDB_TOOLBAR_DRAW, IDB_TOOLBAR_DRAW_HOT, IDB_TOOLBAR_DRAW_DISABLED);
	
	m_wndToolBar.EnableDocking(CBRS_ALIGN_TOP);
	EnableDocking(CBRS_ALIGN_TOP);
	//--------------------------------------------------------------//

	//==============================================================//
	// 정보 출력용 타이틀바 생성
	//==============================================================//
	if (!m_wndTitleBar.Create(this, IDD_DIALOG_TITLE, CBRS_ALIGN_TOP, IDD_DIALOG_TITLE))
	{
		TRACE0("Failed to create my title bar\n");
		return -1;      // fail to create
	}
	//m_wndTitleBar.EnableDocking(CBRS_ALIGN_ANY);
	//DockControlBar(&m_wndTitleBar);
	st_handler.cwnd_title = &m_wndTitleBar;  // 핸들 설정
	//--------------------------------------------------------------//

	//==============================================================//
	// 메시지 출력용 리스트바 생성
	// : 상태 바 밑에 위치하도록 하려면 이 부분을 상태 바 생성 부분 위로 올리면 됨
	//==============================================================//
	if (!m_wndListBar.Create(this, IDD_DIALOG_LIST, CBRS_ALIGN_BOTTOM, IDD_DIALOG_LIST))
	{
		TRACE0("Failed to create my list bar\n");
		return -1;      // fail to create
	}
	//m_wndListBar.EnableDocking(CBRS_ALIGN_ANY);
	//DockControlBar(&m_wndListBar);
	st_handler.cwnd_list = &m_wndListBar;
	//--------------------------------------------------------------//

	//==============================================================//
	// 규이리 추가 [2014.02.17]
	// 현재 응용 프로그램을 항상 최상위에 위치시킴
	// : 이 부분은 별 필요 없어 주석 처리 함
	//==============================================================//
	//SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | WS_EX_TOPMOST);

	/*
	CRect rect;
	GetWindowRect(rect);    // get the current window size and position

	// now change the size, position, and Z order of the window.
	::SetWindowPos(
		m_hWnd ,        // handle to window
		HWND_TOPMOST,   // placement-order handle
		rect.left,      // horizontal position
		rect.top,       // vertical position
		rect.Width(),	// width
		rect.Height(),  // height
		SWP_SHOWWINDOW  // window-positioning options
	);
	*/
	//--------------------------------------------------------------//

	//==============================================================//
	// 타이틀 정보 출력
	//==============================================================//
	st_handler.sEqpVersion = "[AMT8570 Accy] v1.13 (2015.09.10)";
	SetWindowText(st_handler.sEqpVersion);

	CenterWindow();	// 현재 응용 프로그램을 정 중앙에 배치
	//--------------------------------------------------------------//

	Func.OnSet_MappingIO();	// I/O 매핑
	Func.OnInit_IOBoard();	// I/O 보드 이니셜 작업
	Func.OnClear_OutPort();	// [초기화] 출력 포트 상태

	// [cme2] 파일 초기화 작업 진행
	// 규이리 추가 [2014.09.01]
	int iRet1 = -1;
	iRet1 = COMI.Initialize_MotBoard(st_path.mstr_motor);	// 모션 보드 초기화
	if (iRet1 == BD_GOOD)
	{
		st_handler.mn_mot_brd_initial = TRUE;

		Func.OnSet_MotorUnitDist();	// 모터 환경 파라메터 설정
		// st_teach[i].d_spd_acc 등의 구조체 값임
		Func.OnSet_MotorSpeed();	// 모터 구동 속도 설정
	}
	else
	{
		AfxMessageBox("Motion Board Init Failure!..");
	}
	if (COMI.mn_simulation_mode == 1 ||
		st_handler.mn_virtual_mode == 1)
	{
		st_handler.mn_mot_brd_initial = TRUE;
	}
	/*

	// 타이틀바에 출력하는 메시지는 추후에는 주석 처리하라
	if (st_handler.cwnd_list != NULL)
	{
		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, MACHINE_INFO);
	}

	if (st_handler.cwnd_title != NULL)
	{
		st_handler.cwnd_title->PostMessage(WM_STATUS_CHANGE, MACHINE_STATUS, 0);
		st_handler.cwnd_title->PostMessage(WM_STATUS_CHANGE, DEVICE_MODE, 0);
		st_handler.cwnd_title->PostMessage(WM_STATUS_CHANGE, LEVEL_MODE, 0);
	}*/

	OnCreateMainThread();	// 메인 쓰레드 생성
	
	// 규이리 위치 이동 [2014.02.15]
	/*
	// 이니셜 화면으로 전환 요청
	::PostMessage(st_handler.hWnd, WM_FORM_CHANGE, 1, 3);
	*/

	// 필요하면 타이머 생성하여 사용하라
	/*
	SetTimer(TMR_MAIN_REFRESH,	500,	NULL);
	SetTimer(TMR_FILE_CREATE,	1000,	NULL);
	*/

	// 자동으로 읽어서 처리하도록 수정함
	/*
	st_handler.mn_level_teach = TRUE;
	st_handler.cwnd_title->PostMessage(WM_STATUS_CHANGE, LEVEL_MODE, 0);
	*/

	// 규이리 위치 이동
	::PostMessage(st_handler.hWnd, WM_FORM_CHANGE, 1, 3);
	// kilee [2015.03.20]
	::PostMessage(st_handler.hWnd, WM_CLIENT_MSG_1, CLIENT_CONNECT, 0);

	// 초기에 클라이언트 연결하기 위하여 추가
	SetTimer(TMR_MAIN, 20000, NULL);
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	
	//==============================================================//
	// 타이틀 출력 모드 설정 및 메뉴 삭제
	//==============================================================//
	cs.style  =  WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE | WS_SYSMENU | WS_MINIMIZEBOX;
	cs.style &= ~FWS_ADDTOTITLE;  // 타이틀 출력 가능하도록 스타일 설정
	//cs.style &= ~WS_MAXIMIZEBOX;
	
	// 현재 응용 프로그램에 대한 클래스 이름을 얻는다(Need for Single Instance)
	cs.lpszClass=((CHandlerApp*)AfxGetApp())->GetClassName();

	// 규이리 추가 [2014.08.26]
	// 메뉴 삭제
	if (cs.hMenu != NULL)
	{
		::DestroyMenu(cs.hMenu);
		cs.hMenu = NULL;
	}
	//--------------------------------------------------------------//
	
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


BOOL CMainFrame::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->hwnd == m_wndToolBar.m_hWnd)
	{
		// 툴바가 플로팅된 경우 화면 상단에 강제 정렬
		// 툴바가 이동되면 화면 크기가 변경되기 때문이다 
		if (m_wndToolBar.IsFloating())
		{
			m_wndToolBar.EnableDocking(CBRS_ALIGN_TOP);
			EnableDocking(CBRS_ALIGN_TOP);
			DockControlBar(&m_wndToolBar);
		}
	}
	
	return CFrameWnd::PreTranslateMessage(pMsg);
}

void CMainFrame::OnNcLButtonDblClk(UINT nHitTest, CPoint point) 
{
	// 타이틀바 더블 클릭 시 작아지지 않도록 강제 리턴
	if (nHitTest == HTCAPTION)
	{
		return;
	}
	
	CFrameWnd::OnNcLButtonDblClk(nHitTest, point);
}

void CMainFrame::OnNcLButtonDown(UINT nHitTest, CPoint point) 
{
	// 타이틀바 클릭 되었을 때 강제 리턴
	if (nHitTest == HTCAPTION)
	{
		return;
	}
	
	CFrameWnd::OnNcLButtonDown(nHitTest, point);
}

void CMainFrame::OnNcRButtonDown(UINT nHitTest, CPoint point) 
{
	// 타이틀바에 좌측 버튼 쿨린 경우 강제 리턴
	if (nHitTest == HTCAPTION)
	{
		return;
	}
	
	CFrameWnd::OnNcRButtonDown(nHitTest, point);
}

void CMainFrame::OnNcRButtonDblClk(UINT nHitTest, CPoint point) 
{
	// 타이틀바에 좌측 버튼 쿨린 경우 강제 리턴
	if (nHitTest == HTCAPTION)
	{
		return;
	}

	CFrameWnd::OnNcRButtonDblClk(nHitTest, point);
}

LRESULT CMainFrame::OnMainMessageEvent(WPARAM wParam, LPARAM lParam)
{
	int i=0;

	if (wParam == CTL_YES)
	{
		if (mp_msg_dlg != NULL && IsWindow(mp_msg_dlg->m_hWnd))
		{
			mp_msg_dlg->SetFocus();
			mp_msg_dlg->OnEventMsg_Text_Set();
		}
		else
		{
			mp_msg_dlg = new CDialog_Event_Msg;
			mp_msg_dlg->Create();
			mp_msg_dlg->ShowWindow(SW_SHOW);
		}
	}
	else if (wParam == CTL_NO)
	{
		st_msg.mn_event_msg_list = 0;

		for(i=0; i<3; i++)
		{
			st_msg.mstr_event_msg[i] = "";
		}

		if (mp_msg_dlg != NULL && IsWindow(mp_msg_dlg->m_hWnd))
		{
			mp_msg_dlg->ShowWindow(SW_HIDE);

			delete mp_msg_dlg;
			mp_msg_dlg = NULL;
		}

		FAS_IO.set_out_bit(stIO.o_BuzzerSound1, IO_OFF);
	}

	return 0;
}

void CMainFrame::OnMain() 
{
	if (GetActiveView()->IsKindOf(RUNTIME_CLASS(CScreen_Main)))
	{
		return;
	}
	OnSwitchToForm(IDW_SCREEN_MAIN);
}

void CMainFrame::OnBasic() 
{
	int nCheck = OnIsMenuUsable();	// 메뉴 사용 가능 여부
	if (nCheck != TRUE)
	{
		return;
	}

	if (st_handler.mn_level_teach != TRUE)
	{
		if (st_handler.mn_level_maint != TRUE)
		{
			OnLevel();

			if (st_handler.mn_level_teach != TRUE)
			{
				if (st_handler.mn_level_maint != TRUE)
				{
					return;
				}
			}
		}
	}

	if (GetActiveView()->IsKindOf(RUNTIME_CLASS(CScreen_Basic)))
	{
		return;
	}
	OnSwitchToForm(IDW_SCREEN_BASIC);
}

void CMainFrame::OnSetting() 
{
	int nCheck = OnIsMenuUsable();	// 메뉴 사용 가능 여부
	if (nCheck != TRUE)
	{
		return;
	}
	if (st_handler.nModeManual != CTL_YES)
	{
		return;
	}
	
	if (st_handler.mn_level_teach != TRUE)
	{
		if (st_handler.mn_level_maint != TRUE)
		{
			OnLevel();
			
			if (st_handler.mn_level_teach != TRUE)
			{
				if (st_handler.mn_level_maint != TRUE)
				{
					return;
				}
			}
		}
	}

	if (GetActiveView()->IsKindOf(RUNTIME_CLASS(CScreen_Set_Maintenance)))
	{
		return;
	}
	OnSwitchToForm(IDW_SCREEN_SET_MAINTENANCE);
}

void CMainFrame::OnWait() 
{
	int nCheck = OnIsMenuUsable();	// 메뉴 사용 가능 여부
	if (nCheck != TRUE)
	{
		return;
	}
	if (st_handler.nModeManual != CTL_YES)
	{
		return;
	}
	
	if (st_handler.mn_level_teach != TRUE)
	{
		if (st_handler.mn_level_maint != TRUE)
		{
			OnLevel();
			
			if (st_handler.mn_level_teach != TRUE)
			{
				if (st_handler.mn_level_maint != TRUE)
				{
					return;
				}
			}
		}
	}

	if (GetActiveView()->IsKindOf(RUNTIME_CLASS(CScreen_Wait_Time)))
	{
		return;
	}
	OnSwitchToForm(IDW_SCREEN_WAIT_TIME);
}

void CMainFrame::OnMotor() 
{
	int nCheck = OnIsMenuUsable();	// 메뉴 사용 가능 여부
	if (nCheck != TRUE)
	{
		return;
	}
	if (st_handler.nModeManual != CTL_YES)
	{
		return;
	}
	
	if (st_handler.mn_level_teach != TRUE)
	{
		if (st_handler.mn_level_maint != TRUE)
		{
			OnLevel();
			
			if (st_handler.mn_level_teach != TRUE)
			{
				if (st_handler.mn_level_maint != TRUE)
				{
					return;
				}
			}
		}
	}

	st_msg.mstr_parent_menu = "Motor Screen";
	CDialog_Menu dlgMenu;
	int nResponse = dlgMenu.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
}

void CMainFrame::OnIo() 
{
	int nCheck = OnIsMenuUsable();	// 메뉴 사용 가능 여부
	if (nCheck != TRUE)
	{
		return;
	}
	if (st_handler.nModeManual != CTL_YES)
	{
		return;
	}
	
	if (st_handler.mn_level_teach != TRUE)
	{
		if (st_handler.mn_level_maint != TRUE)
		{
			OnLevel();
			
			if (st_handler.mn_level_teach != TRUE)
			{
				if (st_handler.mn_level_maint != TRUE)
				{
					return;
				}
			}
		}
	}

	if (GetActiveView()->IsKindOf(RUNTIME_CLASS(CScreen_IO)))
	{
		return;
	}
	OnSwitchToForm(IDW_SCREEN_IO);
}

void CMainFrame::OnList() 
{
	int nCheck = OnIsMenuUsable();	// 메뉴 사용 가능 여부
	if (nCheck != TRUE)
	{
		return;
	}

	
	if (st_handler.mn_level_teach != TRUE)
	{
		if (st_handler.mn_level_maint != TRUE)
		{
			OnLevel();
			
			if (st_handler.mn_level_teach != TRUE)
			{
				if (st_handler.mn_level_maint != TRUE)
				{
					return;
				}
			}
		}
	}

	st_msg.mstr_parent_menu = "List Screen";
	CDialog_Menu dlgMenu;
	int nResponse = dlgMenu.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
}

void CMainFrame::OnLevel() 
{
	if (st_handler.nMenuLock != FALSE)
	{
		return;
	}
	if (st_handler.nSysLock != FALSE)
	{
		return;
	}

	st_handler.mstr_pass_level = "LevelChange";
	CDialog_Pass_Check dlgPWord;
	int nResponse = dlgPWord.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{

	}
}

void CMainFrame::OnSave() 
{
	if (st_handler.nMenuLock != FALSE)
	{
		return;
	}
	if (st_handler.nSysLock != FALSE)
	{
		return;
	}

	CDialog_Select dlgSelect;
	CString sSnd;
	st_msg.mstr_confirm_msg = _T("Main : All Setting Data Save?");

	int n_response = dlgSelect.DoModal();
	if (n_response == IDOK)
	{
		OnSave_TotalDataBeforeExit();	// [전체] 설비 데이터 저장

		g_client_accy.OnCreate_SendFormat(MAIN_SUPPLY_TYPE);
 		g_client_accy.OnReq_CommStart();
		
		// SLAVE_MODE(Bottom)의 경우 Accy Math 코드를 하지 않음.
		if (st_basic.nMode_SupplyMode == MASTER_MODE)
		{
			// yangkyu [2015.03.19]
			// 통신 연결시 Accy Math Code 요청
			g_client_accy.OnCreate_SendFormat(MAIN_ACCY_MATH_CODE);
			g_client_accy.OnReq_CommStart();
		}

	}
	else if (n_response == IDCANCEL)
	{

	}
}

void CMainFrame::OnAlarm() 
{
	if (mp_cAlarm != NULL)
	{
		mp_cAlarm->SetFocus();
	}
	else
	{
		mp_cAlarm = new CDialog_Alarm;
		mp_cAlarm->Create(this);
		mp_cAlarm->ShowWindow(SW_SHOW);
	}
}

void CMainFrame::OnAbout() 
{
	if (st_handler.nSysLock != FALSE)
	{
		return;
	}

	st_handler.nSysLock = FALSE;
	st_handler.nSwitchLock = FALSE;

	CDialog_About dlgAbout;
	int nResponse = dlgAbout.DoModal();
	if (nResponse == IDOK)
	{
		ShellExecute(NULL, NULL, TEXT("UpdateList.txt"), NULL, TEXT("ReadMe"), SW_SHOW);
	}
}

void CMainFrame::OnMotTeach() 
{
	int nCheck = OnIsMenuUsable();	// 메뉴 사용 가능 여부
	if (nCheck != TRUE)
	{
		return;
	}
	if (st_handler.nModeManual != CTL_YES)
	{
		return;
	}
	
	if (st_handler.mn_level_teach != TRUE)
	{
		if (st_handler.mn_level_maint != TRUE)
		{
			OnLevel();
			
			if (st_handler.mn_level_teach != TRUE)
			{
				if (st_handler.mn_level_maint != TRUE)
				{
					return;
				}
			}
		}
	}

	if (GetActiveView()->IsKindOf(RUNTIME_CLASS(CScreen_Motor)))
	{
		return;
	}
	OnSwitchToForm(IDW_SCREEN_MOTOR_TEACH);
}

void CMainFrame::OnMotSpeed() 
{
	int nCheck = OnIsMenuUsable();	// 메뉴 사용 가능 여부
	if (nCheck != TRUE)
	{
		return;
	}
	if (st_handler.nModeManual != CTL_YES)
	{
		return;
	}
	
	if (st_handler.mn_level_teach != TRUE)
	{
		if (st_handler.mn_level_maint != TRUE)
		{
			OnLevel();
			
			if (st_handler.mn_level_teach != TRUE)
			{
				if (st_handler.mn_level_maint != TRUE)
				{
					return;
				}
			}
		}
	}

	if (GetActiveView()->IsKindOf(RUNTIME_CLASS(CScreen_Motor_Speed)))
	{
		return;
	}
	OnSwitchToForm(IDW_SCREEN_MOTOR_SPEED);
}

void CMainFrame::OnSetInterface() 
{

}

void CMainFrame::OnSetMaintenance() 
{
	int nCheck = OnIsMenuUsable();	// 메뉴 사용 가능 여부
	if (nCheck != TRUE)
	{
		return;
	}
	if (st_handler.nModeManual != CTL_YES)
	{
		return;
	}
	
	if (st_handler.mn_level_teach != TRUE)
	{
		if (st_handler.mn_level_maint != TRUE)
		{
			OnLevel();
			
			if (st_handler.mn_level_teach != TRUE)
			{
				if (st_handler.mn_level_maint != TRUE)
				{
					return;
				}
			}
		}
	}

	if (GetActiveView()->IsKindOf(RUNTIME_CLASS(CScreen_Set_Maintenance)))
	{
		return;
	}
	OnSwitchToForm(IDW_SCREEN_SET_MAINTENANCE);
}

void CMainFrame::OnAdmin() 
{
	int nCheck = OnIsMenuUsable();	// 메뉴 사용 가능 여부
	if (nCheck != TRUE)
	{
		return;
	}
	if (st_handler.nModeManual != CTL_YES)
	{
		return;
	}
	
	if (st_handler.mn_level_teach != TRUE)
	{
		if (st_handler.mn_level_maint != TRUE)
		{
			OnLevel();
			
			if (st_handler.mn_level_teach != TRUE)
			{
				if (st_handler.mn_level_maint != TRUE)
				{
					return;
				}
			}
		}
	}

	if (GetActiveView()->IsKindOf(RUNTIME_CLASS(CScreen_Admin)))
	{
		return;
	}
	OnSwitchToForm(IDW_SCREEN_ADMINISTRATOR);
}

void CMainFrame::OnAlarm_Destroy() 
{
	if (mp_cAlarm)
	{
		if (mp_cAlarm->IsWindowVisible())
		{
			mp_cAlarm->DestroyWindow();
		}

		if (st_handler.nShowAlarmDlg != FALSE)
		{
			st_handler.nShowAlarmDlg = FALSE;
		}
	}
}

void CMainFrame::OnListOper() 
{
	int nCheck = OnIsMenuUsable();	// 메뉴 사용 가능 여부
	if (nCheck != TRUE)
	{
		return;
	}

	if (GetActiveView()->IsKindOf(RUNTIME_CLASS(CScreen_List_Operation)))
	{
		return;
	}
	OnSwitchToForm(IDW_SCREEN_LIST_OPERATION);
}

void CMainFrame::OnListDataview() 
{
	int nCheck = OnIsMenuUsable();	// 메뉴 사용 가능 여부
	if (nCheck != TRUE)
	{
		return;
	}

	if (GetActiveView()->IsKindOf(RUNTIME_CLASS(CScreen_List_Data_View)))
	{
		return;
	}
	OnSwitchToForm(IDW_SCREEN_LIST_DATA_VIEW);
}

void CMainFrame::OnFileEditor() 
{
	int nCheck = OnIsMenuUsable();	// 메뉴 사용 가능 여부
	if (nCheck != TRUE)
	{
		return;
	}
	
	if (st_handler.mn_level_teach != TRUE)
	{
		if (st_handler.mn_level_maint != TRUE)
		{
			OnLevel();
			
			if (st_handler.mn_level_teach != TRUE)
			{
				if (st_handler.mn_level_maint != TRUE)
				{
					return;
				}
			}
		}
	}

	if (GetActiveView()->IsKindOf(RUNTIME_CLASS(CScreen_File_Editor)))
	{
		return;
	}
	OnSwitchToForm(IDW_SCREEN_FILE_EDITOR);
}

void CMainFrame::OnReset() 
{
	if (st_handler.nMenuLock != FALSE)
	{
		if (st_handler.cwnd_list != NULL)
		{
			if (st_handler.mn_language == LANGUAGE_KOREA)
			{
				// yangkyu [2015.03.24]
				// 변수가 잘못 사용되고 있었음.
				sprintf(st_msg.c_abnormal_msg, "[초기화] 메뉴 LOCK 상태에서는 불가능함 !!");
			}
			else	{
				// yangkyu [2015.03.24]
				// 변수가 잘못 사용되고 있었음.
				sprintf(st_msg.c_abnormal_msg, "[초기화] 메뉴 LOCK 상태에서는 불가능함 !!");
			}
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
		}
		return;
	}
	if (st_handler.nSysLock!= FALSE)
	{
		if (st_handler.cwnd_list != NULL)
		{
			if (st_handler.mn_language == LANGUAGE_KOREA)
			{
				// yangkyu [2015.03.24]
				// 변수가 잘못 사용되고 있었음.
				sprintf(st_msg.c_abnormal_msg, "[초기화] LOCK 모드에서는 불가능함 !!");
			}
			else	{
				// yangkyu [2015.03.24]
				// 변수가 잘못 사용되고 있었음.
				sprintf(st_msg.c_abnormal_msg, "[초기화] LOCK 모드에서는 불가능함 !!");
			}
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
		}
		return;
	}
	if (st_work.nEqpStatus != dSTOP)
	{
		if (st_handler.cwnd_list != NULL)
		{
			if (st_handler.mn_language == LANGUAGE_KOREA)
			{
				// yangkyu [2015.03.24]
				// 변수가 잘못 사용되고 있었음.
				sprintf(st_msg.c_abnormal_msg, "[초기화] 설비 STOP 상태가 아니면 불가능함 !!");
			}
			else	{
				// yangkyu [2015.03.24]
				// 변수가 잘못 사용되고 있었음.
				sprintf(st_msg.c_abnormal_msg, "[초기화] 설비 STOP 상태가 아니면 불가능함 !!");
			}
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
		}
		return;
	}
	//==============================================================//
	// - 모션 보드 플래그는 초기화 시키지 않음
	// : 모션 보드 초기화 작업은 프로그램 시작 시 한번만 진행하기 위함
	// - 충돌 관련 플래그 정보도 초기화 시키지 않음
	// - I/O 보드 초기화 작업 및 매핑 정보 설정 작업도 진행하기 않음
	//==============================================================//
	
	// 모터보드 초기화 작업 완료된 경우에만 모터 정지시킴
	// - 초기화 전에 모터 함수 호출하면 프로그램 다운됨
	if (st_handler.mn_mot_brd_initial == TRUE)
	{
		Func.OnStop_AxisAll();	// 전체 모터축 긴급 정지
	}
	//--------------------------------------------------------------//
	
	Func.OnReset_FlagInitialize();	// [초기화] 각종 변수

	// 초기화 화면으로 전환 요청
	::PostMessage(st_handler.hWnd, WM_FORM_CHANGE, 1, 3);
}

void CMainFrame::OnLock() 
{
	if (st_handler.mn_level_teach != TRUE)
	{
		if (st_handler.mn_level_maint != TRUE)
		{
			OnLevel();
			
			if (st_handler.mn_level_teach != TRUE)
			{
				if (st_handler.mn_level_maint != TRUE)
				{
					return;
				}
			}
		}
	}

	CDialog_Pass_Check pass_chk_dlg;
	st_handler.mstr_pass_level=_T("SystemLock");

	int n_response = pass_chk_dlg.DoModal();
	if (n_response == IDOK)
	{
		// Lock 화면으로 전환 요청
		::PostMessage(st_handler.hWnd, WM_FORM_CHANGE, FORM_NAME, 2);

		if (st_handler.cwnd_list != NULL)
		{
			sprintf(st_msg.c_normal_msg, "System Locking.");
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
		}
	}
	else if (n_response == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}
}

void CMainFrame::OnExit() 
{
	if (st_handler.nMenuLock != FALSE)
	{
		return;
	}
	if (st_handler.nSysLock != FALSE)
	{
		return;
	}


	int nResp;
	CDialog_Message dlgSelect;
	if (st_work.nEqpStatus != dSTOP)
	{
		st_msg.str_fallacy_msg = _T("SSD Auto Sorter is active.");

		nResp = dlgSelect.DoModal();
		if (nResp == IDOK)
		{
			return;
		}
	}

	st_handler.mn_level_teach = FALSE;
	st_handler.mn_level_maint = FALSE;

	if (st_handler.mn_level_teach != TRUE)
	{
		if (st_handler.mn_level_maint != TRUE)
		{
			OnLevel();
			
			if (st_handler.mn_level_teach != TRUE)
			{
				if (st_handler.mn_level_maint != TRUE)
				{
					return;
				}
			}
		}
	}

	CDialog_Exit dlgExit;
	nResp = dlgExit.DoModal();
	if (nResp == IDOK)
	{
		/*
		CWnd* hWndFind = CWnd::FindWindow(NULL, "XgemInterface");
		if (hWndFind != NULL)
		{
			hWndFind->SendMessage(WM_CLOSE, 0, 0);
		}
		*/

		st_timedb.mnTime_Section	= dSTOP;
		st_timedb.mole_date			= COleDateTime::GetCurrentTime();
		Func.DB_Write_Time(st_timedb);

		extStopAll = 1;	// 쓰레드 종료 플래그 설정
		OnKillMainThread();	// 메인 쓰레드 종료

		OnSave_TotalDataBeforeExit();	// [전체] 설비 데이터 저장

		CView* pOldActiveView = GetActiveView();	// 현재 화면에 출력된 뷰 정보 설정 (save old view)
		pOldActiveView->ShowWindow(SW_HIDE);	// 기존에 출력된 뷰를 감춘다 (hide the old)
		pOldActiveView->DestroyWindow();		// 기존에 출력된 뷰를 파괴시킨다 (Old View Destroy 시키기 위하여 추가)
		pOldActiveView = NULL;					// 기존에 출력된 뷰에 대한 정보 저장 변수 초기화
		delete pOldActiveView;	// 기존에 출력된 뷰 정보를 제거한다 (kill old view)


		// 규이리 추가 [2014.01.15]
		// 외부 응용 프로그램을 종료시키기 위해 추가함
		// - xgem 처리 프로그램 종료함
		HWND hWndManage = OnGet_ProgramHwnd();	// 원하는 타이틀 정보가 갖는 프로세스의 핸들 검색
		// 프로세스 찾음
		if (hWndManage != NULL)
		{
			// 해당 프로세스 종료시킴
			m_hFindWnd->SendMessage(WM_CLOSE, 0, 0);
		}

		CFrameWnd::OnClose();
	}
}

void CMainFrame::OnClose() 
{
	OnExit();
}

void CMainFrame::OnDestroy() 
{
	// 열어놓은 통신 포트 [TCP/IP] 종료하기 위하여 추가함
	if (st_client[0].n_connect == CTL_YES)
	{
		::SendMessage(st_handler.hWnd, WM_CLIENT_MSG_1, CLIENT_CLOSE, 0);
	}
	// 열어놓은 통신 포트 [TCP/IP] 종료하기 위하여 추가함
	if (st_client[1].n_connect == CTL_YES)
	{
		::SendMessage(st_handler.hWnd, WM_CLIENT_MSG_2, CLIENT_CLOSE, 0);
	}

	// 규이리 추가 [2014.08.12]
	// 소켓 관련 정보 삭제
	int i;
	for(i=0; i<10; i++)
	{
		if (m_pServer[i] != NULL)
		{
			delete m_pServer[i];
			m_pServer[i] = NULL;
		}
		
		if (m_pClient[i] != NULL)
		{
			delete m_pClient[i];
			m_pClient[i] = NULL;
		}
	}
	
	OnPortClose();	// 시리얼 통신 포트 [RS-232C] 닫기
	
	// 타이머 삭제
	KillTimer(TMR_MAIN_REFRESH);
	KillTimer(TMR_FILE_CREATE);
	KillTimer(TMR_CIM_);
	// 규이리 추가 [2013.11.23]
	KillTimer(TMR_TAMS_);

	if (mp_cAlarm)
	{
		OnAlarm_Destroy();
		delete this;
	}

	CFrameWnd::OnDestroy();
}

//==================================================================//
// [전체] 설비 데이터 저장
//==================================================================//
void CMainFrame::OnSave_TotalDataBeforeExit() 
{
	m_cBasic.OnBasic_Data_Save();				// 전역 변수에 저장된 Basic 셋팅 정보를 파일에 저장하는 함수
	m_cBasic.OnSaveWaitTimeFile();			// 전역 변수에 저장된 Wait Time 셋팅 정보를 파일에 저장하는 함수
	m_cBasic.OnSaveMotorSpeedFile();		// 전역 변수에 저장된 Motor Speed 셋팅 정보를 파일에 저장하는 함수.
	m_cBasic.On_Teach_Data_Save();
	m_cBasic.OnMaintenance_Data_Save();
}

//==================================================================//
// 메뉴 선택을 통한 화면 전환
//==================================================================//
void CMainFrame::OnSwitchToForm(int nzFormNo) 
{
	// 강제 리턴 조건 검사
	if (st_handler.nMenuLock != FALSE)
	{
		return;
	}
	if (st_handler.nSysLock != FALSE)
	{
		return;
	}
	
	CView* pOldActiveView = GetActiveView();	// 현재 화면에 출력된 뷰 정보 설정 (save old view)
	CView* pNewActiveView = (CView*)GetDlgItem(nzFormNo);	// 새로 출력할 뷰 정보 설정 (get new view)
	
	// 새로 출력할 뷰가 아직 출력되지 않았는지 검사 (if it hasn't been created yet)
	if (pNewActiveView == NULL)
	{
		// 뷰에 설정된 ID 정보 비교 (these IDs are the dialog IDs of the view but can use anything)
		switch(nzFormNo)
		{
		case IDW_SCREEN_MAIN:
			pNewActiveView = (CView*)new CScreen_Main;
			break;

		case IDW_SCREEN_LOCK:
			pNewActiveView = (CView*)new CScreen_Lock;
			break;

		case IDW_SCREEN_INIT:
			pNewActiveView = (CView*)new CScreen_Initial;
			break;

		case IDW_SCREEN_BASIC:
			pNewActiveView = (CView*)new CScreen_Basic;
			break;

		case IDW_SCREEN_SET_MAINTENANCE:
			pNewActiveView = (CView*)new CScreen_Set_Maintenance;
			break;

		case IDW_SCREEN_WAIT_TIME:
			pNewActiveView = (CView*)new CScreen_Wait_Time;
			break;

		case IDW_SCREEN_MOTOR_TEACH:
			pNewActiveView = (CView*)new CScreen_Motor;
			break;

		case IDW_SCREEN_MOTOR_SPEED:
			pNewActiveView = (CView*)new CScreen_Motor_Speed;
			break;

		case IDW_SCREEN_LIST_OPERATION:
			pNewActiveView = (CView*)new CScreen_List_Operation;
			break;

		case IDW_SCREEN_LIST_DATA_VIEW:
			pNewActiveView = (CView*)new CScreen_List_Data_View;
			break;

		case IDW_SCREEN_IO:
			pNewActiveView = (CView*)new CScreen_IO;
			break;

		case IDW_SCREEN_ADMINISTRATOR:
			pNewActiveView = (CView*)new CScreen_Admin;
			break;

		case IDW_SCREEN_FILE_EDITOR:
			pNewActiveView = (CView*)new CScreen_File_Editor;
			break;
		}

		CCreateContext context;	// 새로운 뷰에 도큐먼트 Attach ( attach the document to the new view)
		context.m_pCurrentDoc = pOldActiveView->GetDocument();
		pNewActiveView->Create(NULL, NULL, 0L, CFrameWnd::rectDefault, this, nzFormNo, &context);
		pNewActiveView->OnInitialUpdate();
	}
	
	SetActiveView(pNewActiveView);			// 새로 출력된 뷰를 액티브 뷰로 설정 (change the active view)
	pNewActiveView->ShowWindow(SW_SHOW);	// 새로운 뷰를 출력 (show the new window)
	
	pOldActiveView->ShowWindow(SW_HIDE);	// 기존에 출력된 뷰를 감춘다 (hide the old)
	pOldActiveView->DestroyWindow();		// 기존에 출력된 뷰를 파괴시킨다 (Old View Destroy 시키기 위하여 추가)
	pOldActiveView = NULL;					// 기존에 출력된 뷰에 대한 정보 저장 변수 초기화
	
	::SetWindowLong(pNewActiveView->m_hWnd, GWL_ID, AFX_IDW_PANE_FIRST);	// gotta have it
	RecalcLayout();	// 프레임을 조율한다 (adjust frame)
	delete pOldActiveView;	// 기존에 출력된 뷰 정보를 제거한다 (kill old view)
	
	st_handler.nSelectedMenuNum = nzFormNo;
	
	if (st_handler.cwnd_title != NULL)
	{
		st_handler.cwnd_title->PostMessage(WM_STATUS_CHANGE, FORM_NAME, nzFormNo);
	}
}

//==================================================================//
// 사용자 정의 메시지에 따른 화면 전환
//==================================================================//
LRESULT CMainFrame::OnViewChangeMode(WPARAM wParam, LPARAM lParam) 
{
	if (GetActiveView() == NULL)
	{
		return 0;
	}

	int nCommand = (int)wParam;
	int nRequire = (int)lParam;

	//if (nCommand != 9)
	// 규이리 수정 [2015.02.13]
	if (nCommand != 9 && nCommand != 7)
	{
		// 강제 리턴 조건 검사
		if (st_handler.nMenuLock != FALSE)
		{
			return 0;
		}
		if (st_handler.nSysLock != FALSE)
		{
			return 0;
		}
	}

	switch(nCommand)
	{
	case 1:
		if (nRequire == 1)
		{
			OnMain();
		}
		else if (nRequire == 2)
		{
			if(GetActiveView()->IsKindOf(RUNTIME_CLASS(CScreen_Lock)))   return 0;
			OnSwitchToForm(IDW_SCREEN_LOCK) ;
		}
		else if (nRequire == 3)
		{
			if (GetActiveView()->IsKindOf(RUNTIME_CLASS(CScreen_Initial)))
			{
				return 0;
			}
			OnSwitchToForm(IDW_SCREEN_INIT);
		}
		break;
	case 2:
		OnBasic();
		break;
	case 3:
		if (nRequire == 1)
		{
		}
		else if (nRequire == 2)
		{
		}
		break;
	case 4:
		break;
	case 5:
		if (nRequire == 1)
		{
			OnMotTeach();
		}
		if (nRequire == 2)
		{
			OnMotSpeed();
		}
		break;
	case 6:
		if (nRequire == 1)
		{
			OnListOper();
		}
		else if (nRequire == 2)
		{
			//OnListDataview();
		}
		else if (nRequire == 3)
		{
		}
		else if (nRequire == 4)
		{
		}
		else if (nRequire == 6)
		{
		}
		break;
	case 7:
		if (nRequire == 1)
		{
			OnAlarm();
		}
		else if (nRequire == 2)
		{
			OnAlarm_Destroy();
		}
		break;

		// 백상현 추가 [2014.10.10]
	case 8:
		if (nRequire == 1)
		{
			OnAdmin();
		}
		break;

	case 9:
		if (nRequire == 1)
		{
			OnIo();
		}
		break;

	case 60:
		if (nRequire == DLG_SHOW_)
		{
			OnShow_DlgNetwork();	// KeyIn-Material 대화상자 SHOW
		}
		else if (nRequire == DLG_HIDE_)
		{
			OnKill_DlgNetwork();	// KeyIn-Material 대화상자 파괴
		}
		break;

	case 61:
		if (nRequire == DLG_SHOW_)
		{
			OnShow_DlgSmema();	// KeyIn-Material 대화상자 SHOW
		}
		else if (nRequire == DLG_HIDE_)
		{
			OnKill_DlgSmema();	// KeyIn-Material 대화상자 파괴
		}
		break;

	}
	
	return 0;
}

void CMainFrame::OnReSet_StructVar() 
{
	//==============================================================//
	// 핸들 초기화
	// - 핸들 초기화 작업은 OnCreate() 함수 이전에 호출되어져야 함
	//==============================================================//
	st_handler.cwnd_title	= NULL;
	st_handler.cwnd_list	= NULL;

	st_handler.cwnd_main		= NULL;
	st_handler.cwnd_basic		= NULL;
	st_handler.cwnd_motor		= NULL;
	st_handler.cwnd_motorAxis	= NULL;
	//--------------------------------------------------------------//

	OnSet_PathInfo();	// 경로 정보 구조체에 설정 및 폴더 생성
	OnIsExistDvcFie();	// 작업할 모델명이 존재하는지 확인

	// 규이리 추가 [2014.08.26]
	st_handler.nShowAlarmDlg = FALSE;
	mp_cAlarm	= NULL;
	mp_msg_dlg	= NULL;

	mp_cNetwork = NULL;
	mp_cSmema = NULL;

	// 마지막 발생한 알람 정보 초기화
	st_handler.mstr_last_alarm = "";

	// [초기화] 알람 코드 편집용 프로그램 로딩 여부
	st_handler.b_CalledAlarmEditor = FALSE;

	MyJamData.OnLoad_AlarmInfoFronFile();	// [파일->구조체] 모든 알람 정보 로딩

	m_cBasic.OnLoad_WaitTimeData();
	m_cBasic.OnLoad_MotSpeedData();
	m_cBasic.OnLoad_BasicData();
	m_cBasic.OnLoad_MaintData();
	// 백상현 추가 [2014.08.27]
	m_cBasic.On_Teach_Data_Load();

	m_cBasic.On_Network_Data_Load();
}	

//==================================================================//
// 경로 정보 구조체에 설정 및 폴더 생성
//==================================================================//
void CMainFrame::OnSet_PathInfo() 
{
	st_path.sIOMap		= EXCEL_PATH_BASE_ + FILE_IOMAP_;
	st_path.sIOPartMap	= EXCEL_PATH_BASE_ + FILE_IO_PART_;
	st_path.sMotAxisMap	= EXCEL_PATH_BASE_ + FILE_MOT_AXIS_;
	st_path.sMotPartMap	= EXCEL_PATH_BASE_ + FILE_MOT_PART_;
	
	st_path.sWaitTimeMap	= EXCEL_PATH_BASE_ + FILE_WAITTIME_MAP_;
	st_path.sMotSpeed	= SETTING_PATH_BASE_ + FILE_MOT_SPEED_;
	st_path.sWaitTime	= SETTING_PATH_BASE_ + FILE_WAITTIME_;

	// 규이리 추가 [2015.02.12]
	st_path.mstr_path_alarm = BMP_PATH_BASE_;
	
	char chr_data[100];
	// 생성할 폴더 정보 얻음
	::GetPrivateProfileString("Folder_Data", "Main", "Default", chr_data, 100, ".\\PathInfo.TXT");
	CString sMainPath = chr_data;
	sMainPath.TrimLeft(' ');
	sMainPath.TrimRight(' ');
	
	// 규이리 주석 처리 [2015.02.14]
	/*
	// 김양규 추가 [2014.12.09]
	// 알람 경로 누락으로 인해 이미지 출력이 되지 않아 경로 추가
	st_path.mstr_path_alarm =	PGM_BASE_ + _T("Bmp\\");						// 출력 알람 정보 존재 폴더 설정
	Func.CreateFolder(st_path.mstr_path_alarm);
	*/

	st_path.mstr_basic = sMainPath + "Setting\\BasicTerm.TXT";
	CString sFolder;
	sFolder = TEXT(st_path.mstr_basic);
	_CreateFolder((LPCTSTR)sFolder);  // 폴더 생성
	
	// 김양규 추가 [2014.12.09]
	// 네트워크 정보를 파일에서 읽어오기 위해 NetworkAddr.txt 파일 경로 추가
	st_path.mstr_network = SETTING_PATH_BASE_ + "NetworkAddr.TXT";
	sFolder = TEXT(st_path.mstr_network);
	_CreateFolder((LPCTSTR)sFolder);  // 폴더 생성

	st_path.mstr_path_dvc = PGM_BASE_ + "Device\\";
	sFolder = TEXT(st_path.mstr_path_dvc);
	_CreateFolder((LPCTSTR)sFolder);  // 폴더 생성

	st_path.mstr_basic_folder	= sMainPath + "Setting\\";
	st_path.mstr_motor			= sMainPath + "Setting\\AMT8570A.cme2";
	
	st_path.mstr_file_basic	= "BasicTerm";
	st_path.mstr_file_wait	= "WaitTime";
	
	st_path.mstr_path_file	= sMainPath;
	st_path.mstr_path_back	= "d:\\BackUp\\";
}

//==================================================================//
// 작업할 모델명이 존재하는지 확인
//==================================================================//
int CMainFrame::OnIsExistDvcFie() 
{
	// [초기화] 함수 리턴 플래그 (작업중)
	int FuncRet = RET_PROCEED;

	//==============================================================//
	// 디바이스 정보가 저장된 파일이 존재하는지 검사
	// - 티칭 정보 저장되어 있음
	//==============================================================//
	CString sLoadDvc;	// 로딩할 디바이스명 저장
	CString sExtension;	// 파일 확장자 저장
	int nPos;
	char chData[50];

	// 로딩한 디바이스 정보가 공백인지 검사
	if (st_basic.sDvcName == "")
	{
		::GetPrivateProfileString("BasicData", "DeviceType", "-1", chData, 50, st_path.mstr_basic);
		sLoadDvc = chData;
		sLoadDvc.TrimLeft(' ');
		sLoadDvc.TrimRight(' ');

		// 확장자 위치 검사
		nPos = sLoadDvc.Find(".TXT");
		if (nPos == -1)
		{
			st_basic.sDvcName = sLoadDvc + ".TXT";
		}
		else
		{
			sExtension = sLoadDvc.Mid(nPos);  // 확장자 정보 얻음
			// 확장자가 틀리면 기본 디바이스명을 설정
			if (sExtension != ".TXT")
			{
				st_basic.sDvcName = "DEFAULT.TXT";

				if (st_handler.cwnd_list != NULL)
				{
					sprintf(st_msg.c_abnormal_msg, "[DEVICE FILE[DEFAULT.DAT]] The error happened at a file Exist-1.");
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
				}
			}
			else
			{
				st_basic.sDvcName = sLoadDvc;
			}
		}
	}
	//--------------------------------------------------------------//

	//==============================================================//
	// 디바이스 파일이 존재하는지 확인
	//==============================================================//
	CFileFind finder;
	
	BOOL bFlag = finder.FindFile(st_path.mstr_path_dvc + st_basic.sDvcName);
	finder.Close();

	if (bFlag == 1)
	{
		// 정상적으로 디바이스 파일이 존재함
	}
	else
	{
		if (st_handler.cwnd_list != NULL)
		{
			sprintf(st_msg.c_abnormal_msg, "[Device Type File Not Exist] The error happened at a file Exist-1.");
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
		}

		// 파일이 존재하지 않는다고 경고 알람을 출력시킴
		g_ioMgr.set_out_bit(stIO.o_BuzzerSound1, IO_ON);

		// 900005 0 21 "SETTING_OF_THE_DEVICE_TYPE_ERR."
		alarm.mstr_code		= "900005";
		alarm.mn_count_mode	= 0;
		alarm.mn_type_mode	= eWARNING;
		st_work.nEqpStatus	= dWARNING;

		FuncRet = RET_ERROR;
	}
	//--------------------------------------------------------------//

	return FuncRet;
}

LRESULT CMainFrame::OnMainframe_Work(WPARAM wParam, LPARAM lParam)
{
	int mn_command_num = (int)wParam;


	return 0;
}

void CMainFrame::OnTimer(UINT nIDEvent) 
{
	CString str_display_time;
	int n_hour, n_minute, n_second;
	int n_temp;  // 총 시간 [초] 저장 변수
	int n_min_temp;	// 총 시간 [분] 저장 변수 
	int n_year, n_month, n_day;				// 년, 월, 일 저장 변수

	int nIndexMin = 0, nIndexSec = 0;
	int nCyclexMin = 0, nCycleSec = 0;

	CString strTimeLog;
	CString mstr_temp;

	CString mStartTime, mEndTime; 

	COleDateTime cur_time;
	CTime bin_time;

	int nArray;


	if (nIDEvent == TMR_MAIN_REFRESH)
	{
		CTime cur = CTime::GetCurrentTime();
		CTimeSpan diff = cur - st_handler.m_tRef;
		st_handler.m_tRef = cur;

		switch(st_work.nEqpStatus)
		{
		case dRUN:
			break;

		case dWARNING:
		case dJAM:
			st_handler.m_tJ		= st_handler.m_tJ + diff;
			st_handler.m_tDJ	= st_handler.m_tDJ + diff;
			break;

		case dLOCK:
			st_handler.m_tM		= st_handler.m_tM + diff;
			st_handler.m_tDM	= st_handler.m_tDM + diff;
			break;

		case dLOTEND:
		case dSTOP:
			st_handler.m_tS		= st_handler.m_tS + diff;
			st_handler.m_tDS	= st_handler.m_tDS + diff;
			break;
		}

		if (st_handler.cwnd_main != NULL)
		{
			st_handler.cwnd_main->PostMessage(WM_WORK_END, MAIN_TIMEINFO, st_work.nEqpStatus);
		}
	}
	else if (nIDEvent == TMR_FILE_CREATE)
	{
		//==========================================================//
		// 파일 생성에 필요한 시간 정보 얻기
		//==========================================================//
		cur_time = COleDateTime::GetCurrentTime();  // 현재 시간 정보 얻기
		
		n_hour		= cur_time.GetHour();
		n_minute	= cur_time.GetMinute();
		n_second	= cur_time.GetSecond();
		n_temp		= n_hour*3600 + n_minute*60 + n_second;
		
		n_year	= cur_time.GetYear();
		n_month	= cur_time.GetMonth();
		n_day	= cur_time.GetDay();
		//----------------------------------------------------------//

		COleDateTimeSpan span(0, 22, 0, 0);	// 하루
		COleDateTime timeLastDay(m_timeLastDay);
		COleDateTime timeSpan = COleDateTime::GetCurrentTime() - span;

		if (timeSpan >= timeLastDay)
		{
			Func.OnDailyDataSave();
			Func.OnDayDataReset();

			COleDateTimeSpan span2(1, 0, 0, 0);
			timeSpan += span2;
			timeSpan.GetAsSystemTime(m_timeLastDay);
			m_timeLastDay.wHour		= 0;
			m_timeLastDay.wMinute	= 0;
			m_timeLastDay.wSecond	= 0;
			m_timeLastDay.wMilliseconds	= 0;

			AMTRegistry::RegWriteTime(m_timeLastDay);
		}

		if (n_minute == 59 && n_second == 59)
		{
			st_work.nHourPassCount[1]  = st_work.nHourPassCount[0];
			st_work.nHourPassCount[1] -= st_work.nHourPassCount[2];
			Func.OnHour_Pass_Count();
			st_work.nHourPassCount[2] += st_work.nHourPassCount[1];
			st_work.iLastTime = n_hour;

			if (n_hour == 21)
			{
				Func.OnHour_Pass_Count_Init();
			}

			if (st_handler.cwnd_main != NULL)
			{
				st_handler.cwnd_main->PostMessage(WM_WORK_END, MAIN_DSP_UPH, 0);
			}
		}

		if (n_hour == st_basic.n_day_start_time &&
			n_minute == 0 && n_second < 2)
		{

		}
		if (n_hour == 22 && n_summary_flag == 0)
		{

		}
		else if (n_hour<21 && n_summary_flag == 1)
		{
			n_summary_flag = 0;
		}

		if ((n_hour == 14 || n_hour == 22) && n_minute == 0 && n_second<2)
		{

		}

		n_min_temp = n_hour*60 + n_minute;

		if (n_second == 0)
		{
			if (n_min_temp % st_basic.mn_electrostatic_check_gap == 0)
			{
				SetTimer(TMR_MAIN_ELEC_CHK, 200, NULL);
			}
		}
	}
	else  if (nIDEvent == TMR_MAIN)
	{
		nArray = 0;
		
		if (st_client[nArray].n_connect == CTL_NO)
		{
			// [설비<->Main] 연결 작업 시도
			OnCommand_Client_1(CLIENT_CONNECT, nArray);
		}
		else
		{
			// [설비<->Main] 정상 연결되면 타이머 삭제
			KillTimer(TMR_MAIN);
		}
	}

	CFrameWnd::OnTimer(nIDEvent);
}

//==================================================================//
// 메인 쓰레드 생성
//==================================================================//
void CMainFrame::OnCreateMainThread() 
{
	// 규이리 추가 [22011.11.09]
	extStopAll = 0;  // 쓰레드 시작 플래그 [현재 RUN]

	int i=0;
	for(i=0; i<25; i++)
	{
		// 쓰레드 핸들 초기화
		m_thread[i] = NULL;
	}

	//==============================================================//
	// 규이리 추가 [2013.06.18]
	// 시컨스 관련 쓰레드 생성
	//==============================================================//
	m_thread[0] = AfxBeginThread(OnThread_ReadIO, this);
	if (m_thread[0] != NULL)
	{
		hThrHandle[0] = m_thread[0]->m_hThread;
	}
	else	{
		AfxMessageBox("Cannot begin the thread [OnThread_ReadIO]");
	}

	m_thread[1] = AfxBeginThread(OnThread_MotMove, this);
	if (m_thread[1] != NULL)
	{
		hThrHandle[1] = m_thread[1]->m_hThread;
	}
	else	{
		AfxMessageBox("Cannot begin the thread [OnThread_MotMove]");
	}

	m_thread[2] = AfxBeginThread(OnThread_TowerLamp, this);
	if (m_thread[2] != NULL)
	{
		hThrHandle[2] = m_thread[2]->m_hThread;
	}
	else	{
		AfxMessageBox("Cannot begin the thread [OnThread_TowerLamp]");
	}

	m_thread[3] = AfxBeginThread(OnThread_LampSwitch, this);
	if (m_thread[3] != NULL)
	{
		hThrHandle[3] = m_thread[3]->m_hThread;
	}
	else	{
		AfxMessageBox("Cannot begin the thread [OnThread_LampSwitch]");
	}

	m_thread[4] = AfxBeginThread(OnThread_RunCondition, this);
	if (m_thread[4] != NULL)
	{
		hThrHandle[4] = m_thread[4]->m_hThread;
	}
	else	{
		AfxMessageBox("Cannot begin the thread [OnThread_RunCondition]");
	}

	m_thread[5] = AfxBeginThread(OnThread_AlarmDisplay, this);
	if (m_thread[5] != NULL)
	{
		hThrHandle[5] = m_thread[5]->m_hThread;
	}
	else	{
		AfxMessageBox("Cannot begin the thread [OnThread_AlarmDisplay]");
	}

	m_thread[6] = AfxBeginThread(OnThread_NetWork, this);
	if (m_thread[6] != NULL)
	{
		hThrHandle[6] = m_thread[6]->m_hThread;
	}
	else	{
		AfxMessageBox("Cannot begin the thread [OnThread_NetWork]");
	}

	m_thread[7] = AfxBeginThread(OnThread_Serial, this);
	if (m_thread[7] != NULL)
	{
		hThrHandle[7] = m_thread[7]->m_hThread;
	}
	else	{
		AfxMessageBox("Cannot begin the thread [OnThread_Serial]");
	}	
	
	m_thread[8] = AfxBeginThread(OnThread_LoadingConv, this);
	if (m_thread[8] != NULL)
	{
		hThrHandle[8] = m_thread[8]->m_hThread;
	}
	else	{
		AfxMessageBox("Cannot begin the thread [OnThread_LoadingConv]");
	}	

	m_thread[9] = AfxBeginThread(OnThread_BoxClamp, this);
	if (m_thread[9] != NULL)
	{
		hThrHandle[9] = m_thread[9]->m_hThread;
	}
	else	{
		AfxMessageBox("Cannot begin the thread [OnThread_BoxClamp]");
	}	

	m_thread[10] = AfxBeginThread(OnThread_BoxLifter, this);
	if (m_thread[10] != NULL)
	{
		hThrHandle[10] = m_thread[10]->m_hThread;
	}
	else	{
		AfxMessageBox("Cannot begin the thread [OnThread_BoxLifter]");
	}	

	m_thread[11] = AfxBeginThread(OnThread_XYZRobot, this);
	if (m_thread[11] != NULL)
	{
		hThrHandle[11] = m_thread[11]->m_hThread;
	}
	else	{
		AfxMessageBox("Cannot begin the thread [OnThread_XYZRobot]");
	}
	
	m_thread[12] = AfxBeginThread(OnThread_BufferAlignConv, this);
	if (m_thread[12] != NULL)
	{
		hThrHandle[12] = m_thread[12]->m_hThread;
	}
	else	{
		AfxMessageBox("Cannot begin the thread [OnThread_BufferAlignConv]");
	}

}

//==================================================================//
// 메인 쓰레드 종료
//==================================================================//
void CMainFrame::OnKillMainThread() 
{
	// 규이리 수정 [2011.11.09]
	int i=0;
	for(i=0; i<22; i++)
	{
		if (hThrHandle[i])  // 쓰레드 존재 
		{
			/*
			if (WaitForSingleObject(hThrHandle[i], 2000L) == WAIT_TIMEOUT)
			{
				TerminateThread(hThrHandle[i], 1L);  // 쓰레드 종료 
			}
			
			//CloseHandle(hThrHandle[i]);  // 쓰레드에 대한 핸들 해제 
			hThrHandle[i] = NULL;          // 핸들 변수 초기화
			*/

			// 규이리 수정 [2013.06.15]
			if (WaitForSingleObject(hThrHandle[i], 2000L) == WAIT_TIMEOUT)
			{
				if (TerminateThread(hThrHandle[i], 1L))
				{
					//CloseHandle(hThrHandle[i]);  // 쓰레드에 대한 핸들 해제 
					hThrHandle[i] = NULL;          // 핸들 변수 초기화
				}
			}
		}
	}
}

//==================================================================//
// 메뉴 사용 가능 여부
//==================================================================//
int CMainFrame::OnIsMenuUsable() 
{
	int nFuncRet = TRUE;	// 초기 리턴 값 설정
	
	if (st_handler.nMenuLock != FALSE)
	{
		nFuncRet = FALSE;
		
		if (st_handler.cwnd_list != NULL)
		{
			sprintf(st_msg.c_normal_msg, "[Handler Menu LOCK] after UNLOCK use");
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
		}
	}
	
	if (st_handler.nSysLock != FALSE)
	{
		nFuncRet = FALSE;
		
		if (st_handler.cwnd_list != NULL)
		{
			sprintf(st_msg.c_normal_msg, "[Handler System LOCK] after UNLOCK use");
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
		}
	}
	
	if (st_work.nEqpStatus != dSTOP)
	{
		nFuncRet = FALSE;
	}
	
	if (st_work.nEqpStatus == dLOCK)
	{
		nFuncRet = FALSE;
		
		if (st_handler.cwnd_list != NULL)
		{
			sprintf(st_msg.c_normal_msg, "[Handler LOCK] after UNLOCK use");
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
		}
	}
	
	return nFuncRet;
}


//////////////////////////////////////////////////////////////////////
//==================================================================//
// RS-232C 관련 부분
//==================================================================//
//////////////////////////////////////////////////////////////////////

//==================================================================//
// 사용자 정의 메시지 [메시지 입력 감지]
//==================================================================//
LONG CMainFrame::OnCTSDetected(WPARAM, LPARAM port)
{
	if (port <= 0 ||
		port > MAX_PORT)
	{
		return -1;
	}
	return 0;
}

//==================================================================//
// RS-232C 시리얼 포트를 통한 메시지 전송
// - wParam : 메시지 전송할 포트 번호
//==================================================================//
LRESULT CMainFrame::OnDataSend(WPARAM wParam, LPARAM lParam)
{
	int nPort		= (int)wParam;
	int nCommand	= (int)lParam;

	if (nPort == PORT_BCR_ACCY_)
	{
		OnSend_BcrRead(nPort);	// [EQP->BCR] 바코드 읽기 명령 전송
	}
	return 0;
}

LRESULT CMainFrame::OnSerialPort(WPARAM wParam,LPARAM lParam)
{
	int nStatus	= (int)wParam;
	int nPort	= (int)lParam;
	
	// [OPEN] 요청
	if (nStatus == 1)
	{
		// 플래그 정보는 초기화하지 않음
		// - OnPortOpen() 함수 안에서 포트가 열려있는 경우에는 그냥 리턴시키기 때문
		OnPortOpen(nPort);	// 통신 포트 [RS-232C] 열기
	}
	// [CLOSE] 요청
	else if (nStatus == 0)
	{
		OnPortClose(nPort);	// 시리얼 통신 포트 [RS-232C] 닫기
	}
	return 0;
}

//==================================================================//
// [EQP->BCR] 바코드 읽기 명령 전송
//==================================================================//
void CMainFrame::OnSend_BcrRead(int nzPort) 
{
	// 시리얼 포트가 열리지 않은 경우 강제 리턴
	if (st_serial.n_connect[nzPort -1] != YES)
	{
		st_serial.n_snd_chk[nzPort -1] = COM_ERROR;	// 메시지 전송 에러 설정
		st_work.nModeBCR_Test = FALSE;
		return;
	}
	st_serial.str_snd[nzPort -1].Format("%cON%c", 0x02, 0x03);
	
	// 전송할 메시지가 공백이 아닌지 확인
	// - 공백이면 에러 처리함
	if (st_serial.str_snd[nzPort -1] == _T(""))
	{
		if (st_handler.cwnd_list != NULL)
		{
			// yangkyu [2015.03.24]
			// 변수가 잘못 사용되고 있었음.
 			//st_serial.comm_err[nzPort -1] = "[Send Data] Data Format ERR [contain SPACE]";
 			//st_handler.cwnd_list->PostMessage(WM_LIST_DATA, nzPort, SEND_MSG);
			sprintf(st_msg.c_abnormal_msg, "[Send Data] Data Format ERR [contain SPACE]");
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
		}
		st_serial.n_snd_chk[nzPort -1] = COM_ERROR;	// 메시지 전송 에러 설정
		return;
	}

	// 전송할 문자열 길이 계산
	int nLen = (st_serial.str_snd[nzPort -1]).GetLength();
	st_serial.comm_snd[nzPort -1] = st_serial.str_snd[nzPort -1];

	char buf[1024];
	//buf[0] = '\0';
	//strcpy(buf, st_serial.str_snd[nzPort -1]);
	memset(&buf, 0, sizeof(buf));
	sprintf(buf, st_serial.str_snd[nzPort -1]);
	buf[nLen]		= 0x0D;
	buf[nLen +1]	= 0x0A;

	(m_ports[nzPort -1]).WriteToPort(buf);

	st_serial.n_rec_chk[nzPort -1]	= FALSE;
	st_serial.n_snd_chk[nzPort -1]	= 1;
}

//==================================================================//
// 사용자 정의 메시지 [메시지 수신]
//==================================================================//
LONG CMainFrame::OnCommunication(WPARAM ch, LPARAM port)
{
	// 포트 정보 얻음
	int nPort = (int)port;

	// 정보가 수신되는 포트가 허용된 포트인지 확인
	if (nPort <= 0 ||
		nPort > MAX_PORT)
	{
		return -1;	// 오류 반환
	}
	
	// 읽기 에러 발생한 경우
	if (ch == -1)
	{
		if (st_handler.cwnd_list != NULL)
		{
			// yangkyu [2015.03.24]
			// 변수가 잘못 사용되고 있었음.
			//st_serial.comm_err[nPort -1] = "[RCV Data] Read ERR [from Serial Port]";
			//st_handler.cwnd_list->PostMessage(WM_LIST_DATA, nPort, RECEIVE_MSG);
			sprintf(st_msg.c_abnormal_msg, "[RCV Data] Read ERR [from Serial Port]");
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
		}
		st_serial.n_rec_chk[nPort -1] = COM_ERROR;	// 수신 에러 플래그 설정
		return 0;
	}
	// 쓰기 에러 발생한 경우
	else if (ch == -2)
	{
		if (st_handler.cwnd_list != NULL)
		{
			// yangkyu [2015.03.24]
			// 변수가 잘못 사용되고 있었음.
			//st_serial.comm_err[nPort -1] = "[SEND Data] Write ERR [to Serial Port]";
			//st_handler.cwnd_list->PostMessage(WM_LIST_DATA, nPort, SEND_MSG);
			sprintf(st_msg.c_abnormal_msg, "[SEND Data] Write ERR [to Serial Port]");
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
		}
		st_serial.n_rec_chk[nPort -1] = COM_ERROR;
		return 0;
	}

	if (nPort == PORT_BCR_ACCY_)
	{
		evBCR.OnRcvDataFromBCR(ch, port);	// [EQP<-BCR] 바코드 리더기로부터 메시지 수신
	}
	return 0;
}

//==================================================================//
// 통신 포트 [RS-232C] 열기
//==================================================================//
void CMainFrame::OnPortOpen(int nzPort) 
{
	if (nzPort < 0)	{
		return;
	}

	int nPortNo;
	int nBaudRate;
	CString sParity;
	int nDataBits;
	int nStopBits;

	switch(nzPort)
	{
	case 0:
		nPortNo		= PORT_BCR_ACCY_;
		nBaudRate	= 115200;
		sParity		= "None";
		nDataBits	= 8;
		nStopBits	= 1;
		break;
	}

	// 이미 통신 포트가 열려 있다면..
	if (st_serial.n_connect[nPortNo -1] == YES)
	{
		return;
	}

	// 통신 포트에 대한 이벤트 설정
	DWORD dwCommEvents = 0;
	dwCommEvents |= EV_CTS;
	dwCommEvents |= EV_RXCHAR;

	CString sTemp;
	// 시리얼 통신 포트 열기
	if (m_ports[nPortNo -1].InitPort(this,
		nPortNo,
		nBaudRate,
		sParity[0],
		nDataBits,
		nStopBits,
		dwCommEvents,
		1024))
	{
		m_ports[nPortNo -1].StartMonitoring();
		st_serial.n_connect[nPortNo -1] = YES;	// PORT 정상 열림
	}
	else
	{
		sTemp.Format("Can not open [PORT : %02d]", nPortNo);
		AfxMessageBox(sTemp);
		st_serial.n_connect[nPortNo -1] = NO;	// PORT 열기 실패
	}
}

//==================================================================//
// 시리얼 통신 포트 [RS-232C] 닫기
//==================================================================//
void CMainFrame::OnPortClose(int nzMode) 
{
	int i=0;

	if (nzMode == 1000)
	{
		for(i=0; i<MAX_PORT; i++)
		{
			// [NO:Close  YES:Open]
			if (st_serial.n_connect[i] == YES)
			{
				m_ports[i].ClosePort();  // 통신 포트 Close
				st_serial.n_connect[i] = NO;
			}
		}
	}
	else
	{
		// [NO:Close  YES:Open]
		if (st_serial.n_connect[i] == YES)
		{
			m_ports[i].ClosePort();
			st_serial.n_connect[i] = NO;
		}
	}
}

//==================================================================//
// [MAIN<->ACCY] 통신 담당
//==================================================================//
LRESULT CMainFrame::OnCommand_Client_1(WPARAM wParam, LPARAM lParam)
{
	CString sSnd, sRcv, sTmp, sPart_Head;

	int nStatus	= (int)wParam;
	int nAddr	= (int)lParam;

	switch(nStatus)
	{
	case CLIENT_CONNECT:
		// DISCONNECT 시에 연결 작업 시도
		if (st_client[nAddr].n_connect == CTL_NO)
		{
			m_pClient[nAddr] = new CClientSocket;  // 소켓 생성

			// - Create(int n_address, CString str_ip, int n_port, int n_timeout);
			if (m_pClient[nAddr]->Create(nAddr, st_client[nAddr].str_ip, st_client[nAddr].n_port, 1))
			{
				st_work.nLink_Eqp2Main = CTL_YES;
				st_client[nAddr].n_connect = CTL_YES;
				sTmp.Format("Client_[%02d] Connect Success.. IP = %s , Port = %d", nAddr, st_client[nAddr].str_ip, st_client[nAddr].n_port);

				if (st_handler.cwnd_list != NULL)
				{
					sprintf(st_msg.c_normal_msg, sTmp);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
				}
				m_sLogMsg = sTmp;
				cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);

				g_client_accy.OnCreate_SendFormat(MAIN_SUPPLY_TYPE);
				g_client_accy.OnReq_CommStart();
				
				// SLAVE_MODE(Bottom)의 경우 Accy Math 코드를 하지 않음.
				if (st_basic.nMode_SupplyMode == MASTER_MODE)
				{
					// yangkyu [2015.03.19]
					// 통신 연결시 Accy Math Code 요청
					g_client_accy.OnCreate_SendFormat(MAIN_ACCY_MATH_CODE);
					g_client_accy.OnReq_CommStart();
				}
			}
			else
			{
				// 생성한 소켓 인스턴스 해제
				delete m_pClient[nAddr];
				m_pClient[nAddr] = NULL;

				st_work.nLink_Eqp2Main = CTL_NO;
				st_client[nAddr].n_connect = CTL_NO;
				sTmp.Format("Client_[%02d] Connect Fail.. IP = %s , Port = %d", nAddr, st_client[nAddr].str_ip, st_client[nAddr].n_port);

				if (st_handler.cwnd_list != NULL)
				{
					sprintf(st_msg.c_abnormal_msg, sTmp);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 메시지 출력 요청
				}
				m_sLogMsg = sTmp;
				cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
			}
		}
		break;

	case CLIENT_CLOSE:
		// 연결을 종료시킴
		if (st_client[nAddr].n_connect == CTL_YES)
		{
			delete m_pClient[nAddr];
			m_pClient[nAddr] = NULL;

			st_work.nLink_Eqp2Main = CTL_NO;
			st_client[nAddr].n_connect = CTL_NO;
			sTmp.Format("Client_[%02d] Close.. IP = %s , Port = %d", nAddr, st_client[nAddr].str_ip, st_client[nAddr].n_port);

			if (st_handler.cwnd_list != NULL)
			{
				sprintf(st_msg.c_normal_msg, sTmp);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 메시지 출력 요청
			}
			m_sLogMsg = sTmp;
			cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);

			// 규이리 추가 [2014.11.03]
			SetTimer(TMR_MAIN, 5000, NULL);
		}
		break;

	case CLIENT_SEND:
		sSnd.Format("%s", st_client[nAddr].ch_send);

		if (st_client[nAddr].n_connect == CTL_YES)
		{
			m_pClient[nAddr]->Send(sSnd, sSnd.GetLength(), 0);
			sTmp.Format("Client_[%02d] Data Send    - %s", nAddr, sSnd);

			m_sLogMsg = sTmp;
			cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
		}
		else
		{
			m_sLogMsg.Format("Client_[%02d] Not ConnectData Send - %s", nAddr, sSnd);
			cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
		}
		break;

	case CLIENT_REV:
		// 수신된 데이터를 길이만큼 받아들임
		sTmp.Format("%s", st_client[nAddr].ch_rev);
		sRcv = sTmp.Mid(0, st_client[nAddr].n_rev_length);

		m_sLogMsg.Format("Client_[%02d] Data Receive - %s", nAddr, sRcv);
		cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);

		g_client_accy.OnDivide_FromMain(nAddr, sRcv);	// [MES->CIM] 수신된 메시지 구분하여 처리
		break;

	}
	return 0;
}

//==================================================================//
// Network 대화상자 SHOW
//==================================================================//
void CMainFrame::OnShow_DlgNetwork()
{
	if (mp_cNetwork != NULL)
	{
		// 시점상 문제로 인해 주석 처리함
		//mp_cCarrIdInput->SetFocus();
	}
	else
	{
		mp_cNetwork = new CDialog_Network;
		mp_cNetwork->Create(this);
		mp_cNetwork->ShowWindow(SW_SHOW);
	}
}

//==================================================================//
// Network 대화상자 파괴
//==================================================================//
void CMainFrame::OnKill_DlgNetwork()
{
	if (mp_cNetwork)
	{
		if (mp_cNetwork->IsWindowVisible())
		{
			mp_cNetwork->ShowWindow(SW_HIDE);
			mp_cNetwork->DestroyWindow();
		}
	}
}

//==================================================================//
// SMEMA 대화상자 SHOW
//==================================================================//
void CMainFrame::OnShow_DlgSmema()
{
	if (mp_cSmema != NULL)
	{
		// 시점상 문제로 인해 주석 처리함
		//mp_cCarrIdInput->SetFocus();
	}
	else
	{
		mp_cSmema = new CDialog_Smema;
		mp_cSmema->Create(this);
		mp_cSmema->ShowWindow(SW_SHOW);
	}
}

//==================================================================//
// SMEMA 대화상자 파괴
//==================================================================//
void CMainFrame::OnKill_DlgSmema()
{
	if (mp_cSmema)
	{
		if (mp_cSmema->IsWindowVisible())
		{
			mp_cSmema->ShowWindow(SW_HIDE);
			mp_cSmema->DestroyWindow();
		}
	}
}
