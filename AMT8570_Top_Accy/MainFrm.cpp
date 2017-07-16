// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "Handler.h"

#include "MainFrm.h"


#include "MenuHead.h"	// �� ȭ�鿡 ���� ID ����

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
// ���� ����ü ����
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

// �Ҽ� ��� ���� ����ü �߰�
st_client_param	st_client[10];
st_server_param	st_server[10];

tagIO_PARAM		stIO;			// I/O ��Ī ���� ����ü ���� 
st_other_param			st_other;			// ��Ÿ ���� ���� ����ü ����
tagLAMP_PARAM			stLAMP;			// Tower Lamp ȭ�鿡 ���� ���� ���� ����ü ���� ����
tagSYNC_PARAM	stSync;
tagWITHOUT_PARAM	stWithoutData;

st_Eqp_Map  st_map;
//==================================================================//

// ���̸� �߰� [2014.08.12]
extern CHandlerApp theApp;

// ����� �߰� [2014.0920]
// ���� ���� ����ü
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
	ON_MESSAGE(WM_DATA_SEND, OnDataSend)	// RS-232C �ø��� ��Ʈ ���� ������ �۽� �޽���
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
	// ���̸� �߰� [2014.09.01]
	// �ʱ� ���� ���� ���� ���� [MANUL]
	// - ���� �����尡 ���� ���ϵ��� �ϱ� ����
	// - �ʱ�ȭ �Ŀ� �Ŵ��� ���� ��ȯ������ �ϴ� �̰����� �÷��� ���� �ʱ�ȭ
	st_handler.nModeManual = YES;

	// [�ʱ�ȭ �۾� ����, ��� �� IO �ʱ�ȭ ����]
	Func.OnReset_FlagStructure();	// ����ü�� �÷��� ���� �ʱ�ȭ
	Func.OnReset_FlagInitialize();	// [�ʱ�ȭ] ���� ����
	Func.OnReset_FlagInterLock();	// [�ʱ�ȭ] �ʱ�ȭ �۾����� ���Ͷ� ����
	Func.OnReset_FlagSyncVar();		// [�ʱ�ȭ] SYNC �÷���
	Func.OnReset_FlagLotVar();		// [�ʱ�ȭ] LOT ���� �÷���
	Func.OnReset_StructMapInfo();	// [�ʱ�ȭ] ����ü�� ���� �� ����

	//==============================================================//
	// ���̸� �߰� [2014.08.26]
	// [�ʱ�ȭ] ���� ���� ����
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

	// �ø��� ��� ��Ʈ ����
	for(i=0; i<MAX_PORT; i++)
	{
		st_serial.n_connect[i] = NO;
		OnPortOpen(i);	// ��� ��Ʈ [RS-232C] ����
	}

	// ���� ���� �ð� ����
	st_handler.lTimeTable[0]	= GetCurrentTime();
	st_timedb.mnSectionBuffer	= dSTOP;
	m_timeLastDay				= AMTRegistry::RegReadTime();

	// ��ǰ� IO�� �ùķ��̼� ��� ����
	FAS_IO.mn_simulation_mode	= 0;
	COMI.mn_simulation_mode		= 0;

	OnReSet_StructVar();
}

CMainFrame::~CMainFrame()
{
	// ���̸� �߰� [2014.09.01]
	if (mp_cAlarm != NULL)
	{
		delete mp_cAlarm;
	}

}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	// ������ ȭ�� �ػ� ����
	/*
	if (::GetSystemMetrics(SM_CXSCREEN) != 1280 &&
		::GetSystemMetrics(SM_CYSCREEN) != 1024)
	{
		DEVMODE mode;
		mode.dmBitsPerPel	= 32;  // 32��Ʈ Į��� ����
		mode.dmPelsWidth	= 1280;
		mode.dmPelsHeight	= 1024;
		mode.dmFields		= DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
		ChangeDisplaySettings(&mode, CDS_FULLSCREEN);
	}
	*/

	// LOG �޽����� ������ ���� ����
	cLOG.OnSetPathLogEvent();  // Log Event ������ ���� ����

	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// ���̸� ��ġ �̵� [2014.08.26]
	// ������ ���� ���̶� �ڵ��� �������� ���ϴ� ��� �߻��� �� �־� �̵���
	st_handler.hWnd = GetSafeHwnd();  // ���� �����ӿ� ���� �ڵ� ���� ����

	//==============================================================//
	// Ʈ��Į�� ���� ����
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
	// ���� ��¿� Ÿ��Ʋ�� ����
	//==============================================================//
	if (!m_wndTitleBar.Create(this, IDD_DIALOG_TITLE, CBRS_ALIGN_TOP, IDD_DIALOG_TITLE))
	{
		TRACE0("Failed to create my title bar\n");
		return -1;      // fail to create
	}
	//m_wndTitleBar.EnableDocking(CBRS_ALIGN_ANY);
	//DockControlBar(&m_wndTitleBar);
	st_handler.cwnd_title = &m_wndTitleBar;  // �ڵ� ����
	//--------------------------------------------------------------//

	//==============================================================//
	// �޽��� ��¿� ����Ʈ�� ����
	// : ���� �� �ؿ� ��ġ�ϵ��� �Ϸ��� �� �κ��� ���� �� ���� �κ� ���� �ø��� ��
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
	// ���̸� �߰� [2014.02.17]
	// ���� ���� ���α׷��� �׻� �ֻ����� ��ġ��Ŵ
	// : �� �κ��� �� �ʿ� ���� �ּ� ó�� ��
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
	// Ÿ��Ʋ ���� ���
	//==============================================================//
	st_handler.sEqpVersion = "[AMT8570 Accy] v1.13 (2015.09.10)";
	SetWindowText(st_handler.sEqpVersion);

	CenterWindow();	// ���� ���� ���α׷��� �� �߾ӿ� ��ġ
	//--------------------------------------------------------------//

	Func.OnSet_MappingIO();	// I/O ����
	Func.OnInit_IOBoard();	// I/O ���� �̴ϼ� �۾�
	Func.OnClear_OutPort();	// [�ʱ�ȭ] ��� ��Ʈ ����

	// [cme2] ���� �ʱ�ȭ �۾� ����
	// ���̸� �߰� [2014.09.01]
	int iRet1 = -1;
	iRet1 = COMI.Initialize_MotBoard(st_path.mstr_motor);	// ��� ���� �ʱ�ȭ
	if (iRet1 == BD_GOOD)
	{
		st_handler.mn_mot_brd_initial = TRUE;

		Func.OnSet_MotorUnitDist();	// ���� ȯ�� �Ķ���� ����
		// st_teach[i].d_spd_acc ���� ����ü ����
		Func.OnSet_MotorSpeed();	// ���� ���� �ӵ� ����
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

	// Ÿ��Ʋ�ٿ� ����ϴ� �޽����� ���Ŀ��� �ּ� ó���϶�
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

	OnCreateMainThread();	// ���� ������ ����
	
	// ���̸� ��ġ �̵� [2014.02.15]
	/*
	// �̴ϼ� ȭ������ ��ȯ ��û
	::PostMessage(st_handler.hWnd, WM_FORM_CHANGE, 1, 3);
	*/

	// �ʿ��ϸ� Ÿ�̸� �����Ͽ� ����϶�
	/*
	SetTimer(TMR_MAIN_REFRESH,	500,	NULL);
	SetTimer(TMR_FILE_CREATE,	1000,	NULL);
	*/

	// �ڵ����� �о ó���ϵ��� ������
	/*
	st_handler.mn_level_teach = TRUE;
	st_handler.cwnd_title->PostMessage(WM_STATUS_CHANGE, LEVEL_MODE, 0);
	*/

	// ���̸� ��ġ �̵�
	::PostMessage(st_handler.hWnd, WM_FORM_CHANGE, 1, 3);
	// kilee [2015.03.20]
	::PostMessage(st_handler.hWnd, WM_CLIENT_MSG_1, CLIENT_CONNECT, 0);

	// �ʱ⿡ Ŭ���̾�Ʈ �����ϱ� ���Ͽ� �߰�
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
	// Ÿ��Ʋ ��� ��� ���� �� �޴� ����
	//==============================================================//
	cs.style  =  WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE | WS_SYSMENU | WS_MINIMIZEBOX;
	cs.style &= ~FWS_ADDTOTITLE;  // Ÿ��Ʋ ��� �����ϵ��� ��Ÿ�� ����
	//cs.style &= ~WS_MAXIMIZEBOX;
	
	// ���� ���� ���α׷��� ���� Ŭ���� �̸��� ��´�(Need for Single Instance)
	cs.lpszClass=((CHandlerApp*)AfxGetApp())->GetClassName();

	// ���̸� �߰� [2014.08.26]
	// �޴� ����
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
		// ���ٰ� �÷��õ� ��� ȭ�� ��ܿ� ���� ����
		// ���ٰ� �̵��Ǹ� ȭ�� ũ�Ⱑ ����Ǳ� �����̴� 
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
	// Ÿ��Ʋ�� ���� Ŭ�� �� �۾����� �ʵ��� ���� ����
	if (nHitTest == HTCAPTION)
	{
		return;
	}
	
	CFrameWnd::OnNcLButtonDblClk(nHitTest, point);
}

void CMainFrame::OnNcLButtonDown(UINT nHitTest, CPoint point) 
{
	// Ÿ��Ʋ�� Ŭ�� �Ǿ��� �� ���� ����
	if (nHitTest == HTCAPTION)
	{
		return;
	}
	
	CFrameWnd::OnNcLButtonDown(nHitTest, point);
}

void CMainFrame::OnNcRButtonDown(UINT nHitTest, CPoint point) 
{
	// Ÿ��Ʋ�ٿ� ���� ��ư �� ��� ���� ����
	if (nHitTest == HTCAPTION)
	{
		return;
	}
	
	CFrameWnd::OnNcRButtonDown(nHitTest, point);
}

void CMainFrame::OnNcRButtonDblClk(UINT nHitTest, CPoint point) 
{
	// Ÿ��Ʋ�ٿ� ���� ��ư �� ��� ���� ����
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
	int nCheck = OnIsMenuUsable();	// �޴� ��� ���� ����
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
	int nCheck = OnIsMenuUsable();	// �޴� ��� ���� ����
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
	int nCheck = OnIsMenuUsable();	// �޴� ��� ���� ����
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
	int nCheck = OnIsMenuUsable();	// �޴� ��� ���� ����
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
	int nCheck = OnIsMenuUsable();	// �޴� ��� ���� ����
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
	int nCheck = OnIsMenuUsable();	// �޴� ��� ���� ����
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
		OnSave_TotalDataBeforeExit();	// [��ü] ���� ������ ����

		g_client_accy.OnCreate_SendFormat(MAIN_SUPPLY_TYPE);
 		g_client_accy.OnReq_CommStart();
		
		// SLAVE_MODE(Bottom)�� ��� Accy Math �ڵ带 ���� ����.
		if (st_basic.nMode_SupplyMode == MASTER_MODE)
		{
			// yangkyu [2015.03.19]
			// ��� ����� Accy Math Code ��û
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
	int nCheck = OnIsMenuUsable();	// �޴� ��� ���� ����
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
	int nCheck = OnIsMenuUsable();	// �޴� ��� ���� ����
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
	int nCheck = OnIsMenuUsable();	// �޴� ��� ���� ����
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
	int nCheck = OnIsMenuUsable();	// �޴� ��� ���� ����
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
	int nCheck = OnIsMenuUsable();	// �޴� ��� ���� ����
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
	int nCheck = OnIsMenuUsable();	// �޴� ��� ���� ����
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
	int nCheck = OnIsMenuUsable();	// �޴� ��� ���� ����
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
				// ������ �߸� ���ǰ� �־���.
				sprintf(st_msg.c_abnormal_msg, "[�ʱ�ȭ] �޴� LOCK ���¿����� �Ұ����� !!");
			}
			else	{
				// yangkyu [2015.03.24]
				// ������ �߸� ���ǰ� �־���.
				sprintf(st_msg.c_abnormal_msg, "[�ʱ�ȭ] �޴� LOCK ���¿����� �Ұ����� !!");
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
				// ������ �߸� ���ǰ� �־���.
				sprintf(st_msg.c_abnormal_msg, "[�ʱ�ȭ] LOCK ��忡���� �Ұ����� !!");
			}
			else	{
				// yangkyu [2015.03.24]
				// ������ �߸� ���ǰ� �־���.
				sprintf(st_msg.c_abnormal_msg, "[�ʱ�ȭ] LOCK ��忡���� �Ұ����� !!");
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
				// ������ �߸� ���ǰ� �־���.
				sprintf(st_msg.c_abnormal_msg, "[�ʱ�ȭ] ���� STOP ���°� �ƴϸ� �Ұ����� !!");
			}
			else	{
				// yangkyu [2015.03.24]
				// ������ �߸� ���ǰ� �־���.
				sprintf(st_msg.c_abnormal_msg, "[�ʱ�ȭ] ���� STOP ���°� �ƴϸ� �Ұ����� !!");
			}
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
		}
		return;
	}
	//==============================================================//
	// - ��� ���� �÷��״� �ʱ�ȭ ��Ű�� ����
	// : ��� ���� �ʱ�ȭ �۾��� ���α׷� ���� �� �ѹ��� �����ϱ� ����
	// - �浹 ���� �÷��� ������ �ʱ�ȭ ��Ű�� ����
	// - I/O ���� �ʱ�ȭ �۾� �� ���� ���� ���� �۾��� �����ϱ� ����
	//==============================================================//
	
	// ���ͺ��� �ʱ�ȭ �۾� �Ϸ�� ��쿡�� ���� ������Ŵ
	// - �ʱ�ȭ ���� ���� �Լ� ȣ���ϸ� ���α׷� �ٿ��
	if (st_handler.mn_mot_brd_initial == TRUE)
	{
		Func.OnStop_AxisAll();	// ��ü ������ ��� ����
	}
	//--------------------------------------------------------------//
	
	Func.OnReset_FlagInitialize();	// [�ʱ�ȭ] ���� ����

	// �ʱ�ȭ ȭ������ ��ȯ ��û
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
		// Lock ȭ������ ��ȯ ��û
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

		extStopAll = 1;	// ������ ���� �÷��� ����
		OnKillMainThread();	// ���� ������ ����

		OnSave_TotalDataBeforeExit();	// [��ü] ���� ������ ����

		CView* pOldActiveView = GetActiveView();	// ���� ȭ�鿡 ��µ� �� ���� ���� (save old view)
		pOldActiveView->ShowWindow(SW_HIDE);	// ������ ��µ� �並 ����� (hide the old)
		pOldActiveView->DestroyWindow();		// ������ ��µ� �並 �ı���Ų�� (Old View Destroy ��Ű�� ���Ͽ� �߰�)
		pOldActiveView = NULL;					// ������ ��µ� �信 ���� ���� ���� ���� �ʱ�ȭ
		delete pOldActiveView;	// ������ ��µ� �� ������ �����Ѵ� (kill old view)


		// ���̸� �߰� [2014.01.15]
		// �ܺ� ���� ���α׷��� �����Ű�� ���� �߰���
		// - xgem ó�� ���α׷� ������
		HWND hWndManage = OnGet_ProgramHwnd();	// ���ϴ� Ÿ��Ʋ ������ ���� ���μ����� �ڵ� �˻�
		// ���μ��� ã��
		if (hWndManage != NULL)
		{
			// �ش� ���μ��� �����Ŵ
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
	// ������� ��� ��Ʈ [TCP/IP] �����ϱ� ���Ͽ� �߰���
	if (st_client[0].n_connect == CTL_YES)
	{
		::SendMessage(st_handler.hWnd, WM_CLIENT_MSG_1, CLIENT_CLOSE, 0);
	}
	// ������� ��� ��Ʈ [TCP/IP] �����ϱ� ���Ͽ� �߰���
	if (st_client[1].n_connect == CTL_YES)
	{
		::SendMessage(st_handler.hWnd, WM_CLIENT_MSG_2, CLIENT_CLOSE, 0);
	}

	// ���̸� �߰� [2014.08.12]
	// ���� ���� ���� ����
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
	
	OnPortClose();	// �ø��� ��� ��Ʈ [RS-232C] �ݱ�
	
	// Ÿ�̸� ����
	KillTimer(TMR_MAIN_REFRESH);
	KillTimer(TMR_FILE_CREATE);
	KillTimer(TMR_CIM_);
	// ���̸� �߰� [2013.11.23]
	KillTimer(TMR_TAMS_);

	if (mp_cAlarm)
	{
		OnAlarm_Destroy();
		delete this;
	}

	CFrameWnd::OnDestroy();
}

//==================================================================//
// [��ü] ���� ������ ����
//==================================================================//
void CMainFrame::OnSave_TotalDataBeforeExit() 
{
	m_cBasic.OnBasic_Data_Save();				// ���� ������ ����� Basic ���� ������ ���Ͽ� �����ϴ� �Լ�
	m_cBasic.OnSaveWaitTimeFile();			// ���� ������ ����� Wait Time ���� ������ ���Ͽ� �����ϴ� �Լ�
	m_cBasic.OnSaveMotorSpeedFile();		// ���� ������ ����� Motor Speed ���� ������ ���Ͽ� �����ϴ� �Լ�.
	m_cBasic.On_Teach_Data_Save();
	m_cBasic.OnMaintenance_Data_Save();
}

//==================================================================//
// �޴� ������ ���� ȭ�� ��ȯ
//==================================================================//
void CMainFrame::OnSwitchToForm(int nzFormNo) 
{
	// ���� ���� ���� �˻�
	if (st_handler.nMenuLock != FALSE)
	{
		return;
	}
	if (st_handler.nSysLock != FALSE)
	{
		return;
	}
	
	CView* pOldActiveView = GetActiveView();	// ���� ȭ�鿡 ��µ� �� ���� ���� (save old view)
	CView* pNewActiveView = (CView*)GetDlgItem(nzFormNo);	// ���� ����� �� ���� ���� (get new view)
	
	// ���� ����� �䰡 ���� ��µ��� �ʾҴ��� �˻� (if it hasn't been created yet)
	if (pNewActiveView == NULL)
	{
		// �信 ������ ID ���� �� (these IDs are the dialog IDs of the view but can use anything)
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

		CCreateContext context;	// ���ο� �信 ��ť��Ʈ Attach ( attach the document to the new view)
		context.m_pCurrentDoc = pOldActiveView->GetDocument();
		pNewActiveView->Create(NULL, NULL, 0L, CFrameWnd::rectDefault, this, nzFormNo, &context);
		pNewActiveView->OnInitialUpdate();
	}
	
	SetActiveView(pNewActiveView);			// ���� ��µ� �並 ��Ƽ�� ��� ���� (change the active view)
	pNewActiveView->ShowWindow(SW_SHOW);	// ���ο� �並 ��� (show the new window)
	
	pOldActiveView->ShowWindow(SW_HIDE);	// ������ ��µ� �並 ����� (hide the old)
	pOldActiveView->DestroyWindow();		// ������ ��µ� �並 �ı���Ų�� (Old View Destroy ��Ű�� ���Ͽ� �߰�)
	pOldActiveView = NULL;					// ������ ��µ� �信 ���� ���� ���� ���� �ʱ�ȭ
	
	::SetWindowLong(pNewActiveView->m_hWnd, GWL_ID, AFX_IDW_PANE_FIRST);	// gotta have it
	RecalcLayout();	// �������� �����Ѵ� (adjust frame)
	delete pOldActiveView;	// ������ ��µ� �� ������ �����Ѵ� (kill old view)
	
	st_handler.nSelectedMenuNum = nzFormNo;
	
	if (st_handler.cwnd_title != NULL)
	{
		st_handler.cwnd_title->PostMessage(WM_STATUS_CHANGE, FORM_NAME, nzFormNo);
	}
}

//==================================================================//
// ����� ���� �޽����� ���� ȭ�� ��ȯ
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
	// ���̸� ���� [2015.02.13]
	if (nCommand != 9 && nCommand != 7)
	{
		// ���� ���� ���� �˻�
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

		// ����� �߰� [2014.10.10]
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
			OnShow_DlgNetwork();	// KeyIn-Material ��ȭ���� SHOW
		}
		else if (nRequire == DLG_HIDE_)
		{
			OnKill_DlgNetwork();	// KeyIn-Material ��ȭ���� �ı�
		}
		break;

	case 61:
		if (nRequire == DLG_SHOW_)
		{
			OnShow_DlgSmema();	// KeyIn-Material ��ȭ���� SHOW
		}
		else if (nRequire == DLG_HIDE_)
		{
			OnKill_DlgSmema();	// KeyIn-Material ��ȭ���� �ı�
		}
		break;

	}
	
	return 0;
}

void CMainFrame::OnReSet_StructVar() 
{
	//==============================================================//
	// �ڵ� �ʱ�ȭ
	// - �ڵ� �ʱ�ȭ �۾��� OnCreate() �Լ� ������ ȣ��Ǿ����� ��
	//==============================================================//
	st_handler.cwnd_title	= NULL;
	st_handler.cwnd_list	= NULL;

	st_handler.cwnd_main		= NULL;
	st_handler.cwnd_basic		= NULL;
	st_handler.cwnd_motor		= NULL;
	st_handler.cwnd_motorAxis	= NULL;
	//--------------------------------------------------------------//

	OnSet_PathInfo();	// ��� ���� ����ü�� ���� �� ���� ����
	OnIsExistDvcFie();	// �۾��� �𵨸��� �����ϴ��� Ȯ��

	// ���̸� �߰� [2014.08.26]
	st_handler.nShowAlarmDlg = FALSE;
	mp_cAlarm	= NULL;
	mp_msg_dlg	= NULL;

	mp_cNetwork = NULL;
	mp_cSmema = NULL;

	// ������ �߻��� �˶� ���� �ʱ�ȭ
	st_handler.mstr_last_alarm = "";

	// [�ʱ�ȭ] �˶� �ڵ� ������ ���α׷� �ε� ����
	st_handler.b_CalledAlarmEditor = FALSE;

	MyJamData.OnLoad_AlarmInfoFronFile();	// [����->����ü] ��� �˶� ���� �ε�

	m_cBasic.OnLoad_WaitTimeData();
	m_cBasic.OnLoad_MotSpeedData();
	m_cBasic.OnLoad_BasicData();
	m_cBasic.OnLoad_MaintData();
	// ����� �߰� [2014.08.27]
	m_cBasic.On_Teach_Data_Load();

	m_cBasic.On_Network_Data_Load();
}	

//==================================================================//
// ��� ���� ����ü�� ���� �� ���� ����
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

	// ���̸� �߰� [2015.02.12]
	st_path.mstr_path_alarm = BMP_PATH_BASE_;
	
	char chr_data[100];
	// ������ ���� ���� ����
	::GetPrivateProfileString("Folder_Data", "Main", "Default", chr_data, 100, ".\\PathInfo.TXT");
	CString sMainPath = chr_data;
	sMainPath.TrimLeft(' ');
	sMainPath.TrimRight(' ');
	
	// ���̸� �ּ� ó�� [2015.02.14]
	/*
	// ���� �߰� [2014.12.09]
	// �˶� ��� �������� ���� �̹��� ����� ���� �ʾ� ��� �߰�
	st_path.mstr_path_alarm =	PGM_BASE_ + _T("Bmp\\");						// ��� �˶� ���� ���� ���� ����
	Func.CreateFolder(st_path.mstr_path_alarm);
	*/

	st_path.mstr_basic = sMainPath + "Setting\\BasicTerm.TXT";
	CString sFolder;
	sFolder = TEXT(st_path.mstr_basic);
	_CreateFolder((LPCTSTR)sFolder);  // ���� ����
	
	// ���� �߰� [2014.12.09]
	// ��Ʈ��ũ ������ ���Ͽ��� �о���� ���� NetworkAddr.txt ���� ��� �߰�
	st_path.mstr_network = SETTING_PATH_BASE_ + "NetworkAddr.TXT";
	sFolder = TEXT(st_path.mstr_network);
	_CreateFolder((LPCTSTR)sFolder);  // ���� ����

	st_path.mstr_path_dvc = PGM_BASE_ + "Device\\";
	sFolder = TEXT(st_path.mstr_path_dvc);
	_CreateFolder((LPCTSTR)sFolder);  // ���� ����

	st_path.mstr_basic_folder	= sMainPath + "Setting\\";
	st_path.mstr_motor			= sMainPath + "Setting\\AMT8570A.cme2";
	
	st_path.mstr_file_basic	= "BasicTerm";
	st_path.mstr_file_wait	= "WaitTime";
	
	st_path.mstr_path_file	= sMainPath;
	st_path.mstr_path_back	= "d:\\BackUp\\";
}

//==================================================================//
// �۾��� �𵨸��� �����ϴ��� Ȯ��
//==================================================================//
int CMainFrame::OnIsExistDvcFie() 
{
	// [�ʱ�ȭ] �Լ� ���� �÷��� (�۾���)
	int FuncRet = RET_PROCEED;

	//==============================================================//
	// ����̽� ������ ����� ������ �����ϴ��� �˻�
	// - ƼĪ ���� ����Ǿ� ����
	//==============================================================//
	CString sLoadDvc;	// �ε��� ����̽��� ����
	CString sExtension;	// ���� Ȯ���� ����
	int nPos;
	char chData[50];

	// �ε��� ����̽� ������ �������� �˻�
	if (st_basic.sDvcName == "")
	{
		::GetPrivateProfileString("BasicData", "DeviceType", "-1", chData, 50, st_path.mstr_basic);
		sLoadDvc = chData;
		sLoadDvc.TrimLeft(' ');
		sLoadDvc.TrimRight(' ');

		// Ȯ���� ��ġ �˻�
		nPos = sLoadDvc.Find(".TXT");
		if (nPos == -1)
		{
			st_basic.sDvcName = sLoadDvc + ".TXT";
		}
		else
		{
			sExtension = sLoadDvc.Mid(nPos);  // Ȯ���� ���� ����
			// Ȯ���ڰ� Ʋ���� �⺻ ����̽����� ����
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
	// ����̽� ������ �����ϴ��� Ȯ��
	//==============================================================//
	CFileFind finder;
	
	BOOL bFlag = finder.FindFile(st_path.mstr_path_dvc + st_basic.sDvcName);
	finder.Close();

	if (bFlag == 1)
	{
		// ���������� ����̽� ������ ������
	}
	else
	{
		if (st_handler.cwnd_list != NULL)
		{
			sprintf(st_msg.c_abnormal_msg, "[Device Type File Not Exist] The error happened at a file Exist-1.");
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
		}

		// ������ �������� �ʴ´ٰ� ��� �˶��� ��½�Ŵ
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
	int n_temp;  // �� �ð� [��] ���� ����
	int n_min_temp;	// �� �ð� [��] ���� ���� 
	int n_year, n_month, n_day;				// ��, ��, �� ���� ����

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
		// ���� ������ �ʿ��� �ð� ���� ���
		//==========================================================//
		cur_time = COleDateTime::GetCurrentTime();  // ���� �ð� ���� ���
		
		n_hour		= cur_time.GetHour();
		n_minute	= cur_time.GetMinute();
		n_second	= cur_time.GetSecond();
		n_temp		= n_hour*3600 + n_minute*60 + n_second;
		
		n_year	= cur_time.GetYear();
		n_month	= cur_time.GetMonth();
		n_day	= cur_time.GetDay();
		//----------------------------------------------------------//

		COleDateTimeSpan span(0, 22, 0, 0);	// �Ϸ�
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
			// [����<->Main] ���� �۾� �õ�
			OnCommand_Client_1(CLIENT_CONNECT, nArray);
		}
		else
		{
			// [����<->Main] ���� ����Ǹ� Ÿ�̸� ����
			KillTimer(TMR_MAIN);
		}
	}

	CFrameWnd::OnTimer(nIDEvent);
}

//==================================================================//
// ���� ������ ����
//==================================================================//
void CMainFrame::OnCreateMainThread() 
{
	// ���̸� �߰� [22011.11.09]
	extStopAll = 0;  // ������ ���� �÷��� [���� RUN]

	int i=0;
	for(i=0; i<25; i++)
	{
		// ������ �ڵ� �ʱ�ȭ
		m_thread[i] = NULL;
	}

	//==============================================================//
	// ���̸� �߰� [2013.06.18]
	// ������ ���� ������ ����
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
// ���� ������ ����
//==================================================================//
void CMainFrame::OnKillMainThread() 
{
	// ���̸� ���� [2011.11.09]
	int i=0;
	for(i=0; i<22; i++)
	{
		if (hThrHandle[i])  // ������ ���� 
		{
			/*
			if (WaitForSingleObject(hThrHandle[i], 2000L) == WAIT_TIMEOUT)
			{
				TerminateThread(hThrHandle[i], 1L);  // ������ ���� 
			}
			
			//CloseHandle(hThrHandle[i]);  // �����忡 ���� �ڵ� ���� 
			hThrHandle[i] = NULL;          // �ڵ� ���� �ʱ�ȭ
			*/

			// ���̸� ���� [2013.06.15]
			if (WaitForSingleObject(hThrHandle[i], 2000L) == WAIT_TIMEOUT)
			{
				if (TerminateThread(hThrHandle[i], 1L))
				{
					//CloseHandle(hThrHandle[i]);  // �����忡 ���� �ڵ� ���� 
					hThrHandle[i] = NULL;          // �ڵ� ���� �ʱ�ȭ
				}
			}
		}
	}
}

//==================================================================//
// �޴� ��� ���� ����
//==================================================================//
int CMainFrame::OnIsMenuUsable() 
{
	int nFuncRet = TRUE;	// �ʱ� ���� �� ����
	
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
// RS-232C ���� �κ�
//==================================================================//
//////////////////////////////////////////////////////////////////////

//==================================================================//
// ����� ���� �޽��� [�޽��� �Է� ����]
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
// RS-232C �ø��� ��Ʈ�� ���� �޽��� ����
// - wParam : �޽��� ������ ��Ʈ ��ȣ
//==================================================================//
LRESULT CMainFrame::OnDataSend(WPARAM wParam, LPARAM lParam)
{
	int nPort		= (int)wParam;
	int nCommand	= (int)lParam;

	if (nPort == PORT_BCR_ACCY_)
	{
		OnSend_BcrRead(nPort);	// [EQP->BCR] ���ڵ� �б� ��� ����
	}
	return 0;
}

LRESULT CMainFrame::OnSerialPort(WPARAM wParam,LPARAM lParam)
{
	int nStatus	= (int)wParam;
	int nPort	= (int)lParam;
	
	// [OPEN] ��û
	if (nStatus == 1)
	{
		// �÷��� ������ �ʱ�ȭ���� ����
		// - OnPortOpen() �Լ� �ȿ��� ��Ʈ�� �����ִ� ��쿡�� �׳� ���Ͻ�Ű�� ����
		OnPortOpen(nPort);	// ��� ��Ʈ [RS-232C] ����
	}
	// [CLOSE] ��û
	else if (nStatus == 0)
	{
		OnPortClose(nPort);	// �ø��� ��� ��Ʈ [RS-232C] �ݱ�
	}
	return 0;
}

//==================================================================//
// [EQP->BCR] ���ڵ� �б� ��� ����
//==================================================================//
void CMainFrame::OnSend_BcrRead(int nzPort) 
{
	// �ø��� ��Ʈ�� ������ ���� ��� ���� ����
	if (st_serial.n_connect[nzPort -1] != YES)
	{
		st_serial.n_snd_chk[nzPort -1] = COM_ERROR;	// �޽��� ���� ���� ����
		st_work.nModeBCR_Test = FALSE;
		return;
	}
	st_serial.str_snd[nzPort -1].Format("%cON%c", 0x02, 0x03);
	
	// ������ �޽����� ������ �ƴ��� Ȯ��
	// - �����̸� ���� ó����
	if (st_serial.str_snd[nzPort -1] == _T(""))
	{
		if (st_handler.cwnd_list != NULL)
		{
			// yangkyu [2015.03.24]
			// ������ �߸� ���ǰ� �־���.
 			//st_serial.comm_err[nzPort -1] = "[Send Data] Data Format ERR [contain SPACE]";
 			//st_handler.cwnd_list->PostMessage(WM_LIST_DATA, nzPort, SEND_MSG);
			sprintf(st_msg.c_abnormal_msg, "[Send Data] Data Format ERR [contain SPACE]");
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
		}
		st_serial.n_snd_chk[nzPort -1] = COM_ERROR;	// �޽��� ���� ���� ����
		return;
	}

	// ������ ���ڿ� ���� ���
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
// ����� ���� �޽��� [�޽��� ����]
//==================================================================//
LONG CMainFrame::OnCommunication(WPARAM ch, LPARAM port)
{
	// ��Ʈ ���� ����
	int nPort = (int)port;

	// ������ ���ŵǴ� ��Ʈ�� ���� ��Ʈ���� Ȯ��
	if (nPort <= 0 ||
		nPort > MAX_PORT)
	{
		return -1;	// ���� ��ȯ
	}
	
	// �б� ���� �߻��� ���
	if (ch == -1)
	{
		if (st_handler.cwnd_list != NULL)
		{
			// yangkyu [2015.03.24]
			// ������ �߸� ���ǰ� �־���.
			//st_serial.comm_err[nPort -1] = "[RCV Data] Read ERR [from Serial Port]";
			//st_handler.cwnd_list->PostMessage(WM_LIST_DATA, nPort, RECEIVE_MSG);
			sprintf(st_msg.c_abnormal_msg, "[RCV Data] Read ERR [from Serial Port]");
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
		}
		st_serial.n_rec_chk[nPort -1] = COM_ERROR;	// ���� ���� �÷��� ����
		return 0;
	}
	// ���� ���� �߻��� ���
	else if (ch == -2)
	{
		if (st_handler.cwnd_list != NULL)
		{
			// yangkyu [2015.03.24]
			// ������ �߸� ���ǰ� �־���.
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
		evBCR.OnRcvDataFromBCR(ch, port);	// [EQP<-BCR] ���ڵ� ������κ��� �޽��� ����
	}
	return 0;
}

//==================================================================//
// ��� ��Ʈ [RS-232C] ����
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

	// �̹� ��� ��Ʈ�� ���� �ִٸ�..
	if (st_serial.n_connect[nPortNo -1] == YES)
	{
		return;
	}

	// ��� ��Ʈ�� ���� �̺�Ʈ ����
	DWORD dwCommEvents = 0;
	dwCommEvents |= EV_CTS;
	dwCommEvents |= EV_RXCHAR;

	CString sTemp;
	// �ø��� ��� ��Ʈ ����
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
		st_serial.n_connect[nPortNo -1] = YES;	// PORT ���� ����
	}
	else
	{
		sTemp.Format("Can not open [PORT : %02d]", nPortNo);
		AfxMessageBox(sTemp);
		st_serial.n_connect[nPortNo -1] = NO;	// PORT ���� ����
	}
}

//==================================================================//
// �ø��� ��� ��Ʈ [RS-232C] �ݱ�
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
				m_ports[i].ClosePort();  // ��� ��Ʈ Close
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
// [MAIN<->ACCY] ��� ���
//==================================================================//
LRESULT CMainFrame::OnCommand_Client_1(WPARAM wParam, LPARAM lParam)
{
	CString sSnd, sRcv, sTmp, sPart_Head;

	int nStatus	= (int)wParam;
	int nAddr	= (int)lParam;

	switch(nStatus)
	{
	case CLIENT_CONNECT:
		// DISCONNECT �ÿ� ���� �۾� �õ�
		if (st_client[nAddr].n_connect == CTL_NO)
		{
			m_pClient[nAddr] = new CClientSocket;  // ���� ����

			// - Create(int n_address, CString str_ip, int n_port, int n_timeout);
			if (m_pClient[nAddr]->Create(nAddr, st_client[nAddr].str_ip, st_client[nAddr].n_port, 1))
			{
				st_work.nLink_Eqp2Main = CTL_YES;
				st_client[nAddr].n_connect = CTL_YES;
				sTmp.Format("Client_[%02d] Connect Success.. IP = %s , Port = %d", nAddr, st_client[nAddr].str_ip, st_client[nAddr].n_port);

				if (st_handler.cwnd_list != NULL)
				{
					sprintf(st_msg.c_normal_msg, sTmp);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // �޽��� ��� ��û
				}
				m_sLogMsg = sTmp;
				cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);

				g_client_accy.OnCreate_SendFormat(MAIN_SUPPLY_TYPE);
				g_client_accy.OnReq_CommStart();
				
				// SLAVE_MODE(Bottom)�� ��� Accy Math �ڵ带 ���� ����.
				if (st_basic.nMode_SupplyMode == MASTER_MODE)
				{
					// yangkyu [2015.03.19]
					// ��� ����� Accy Math Code ��û
					g_client_accy.OnCreate_SendFormat(MAIN_ACCY_MATH_CODE);
					g_client_accy.OnReq_CommStart();
				}
			}
			else
			{
				// ������ ���� �ν��Ͻ� ����
				delete m_pClient[nAddr];
				m_pClient[nAddr] = NULL;

				st_work.nLink_Eqp2Main = CTL_NO;
				st_client[nAddr].n_connect = CTL_NO;
				sTmp.Format("Client_[%02d] Connect Fail.. IP = %s , Port = %d", nAddr, st_client[nAddr].str_ip, st_client[nAddr].n_port);

				if (st_handler.cwnd_list != NULL)
				{
					sprintf(st_msg.c_abnormal_msg, sTmp);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // �޽��� ��� ��û
				}
				m_sLogMsg = sTmp;
				cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
			}
		}
		break;

	case CLIENT_CLOSE:
		// ������ �����Ŵ
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
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // �޽��� ��� ��û
			}
			m_sLogMsg = sTmp;
			cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);

			// ���̸� �߰� [2014.11.03]
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
		// ���ŵ� �����͸� ���̸�ŭ �޾Ƶ���
		sTmp.Format("%s", st_client[nAddr].ch_rev);
		sRcv = sTmp.Mid(0, st_client[nAddr].n_rev_length);

		m_sLogMsg.Format("Client_[%02d] Data Receive - %s", nAddr, sRcv);
		cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);

		g_client_accy.OnDivide_FromMain(nAddr, sRcv);	// [MES->CIM] ���ŵ� �޽��� �����Ͽ� ó��
		break;

	}
	return 0;
}

//==================================================================//
// Network ��ȭ���� SHOW
//==================================================================//
void CMainFrame::OnShow_DlgNetwork()
{
	if (mp_cNetwork != NULL)
	{
		// ������ ������ ���� �ּ� ó����
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
// Network ��ȭ���� �ı�
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
// SMEMA ��ȭ���� SHOW
//==================================================================//
void CMainFrame::OnShow_DlgSmema()
{
	if (mp_cSmema != NULL)
	{
		// ������ ������ ���� �ּ� ó����
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
// SMEMA ��ȭ���� �ı�
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
