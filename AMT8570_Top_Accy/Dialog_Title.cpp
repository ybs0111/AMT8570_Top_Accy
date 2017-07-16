// Dialog_Title.cpp : implementation file
//

#include "stdafx.h"
#include "Handler.h"
#include "Dialog_Title.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialog_Title dialog


CDialog_Title::CDialog_Title()
{
	//{{AFX_DATA_INIT(CDialog_Title)
	//}}AFX_DATA_INIT
	m_bDispThreadFlag	= FALSE;	// 쓰레드 생성되지 않았음
	m_hDispThreadHandle	= NULL;		// 쓰레드 핸들 정보 초기화
	m_unDispThreadID	= NULL;		// 쓰레드 ID 정보 초기화
}


void CDialog_Title::DoDataExchange(CDataExchange* pDX)
{
	CInitDialogBar::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialog_Title)
	DDX_Control(pDX, IDC_TITLE_BCR_MODE, m_title_bcr_mode);
	DDX_Control(pDX, IDC_TITLE_CNNT_EQP2MAIN, m_title_cnnt_eqp2main);
	DDX_Control(pDX, IDC_TITLE_LEVEL_MODE, m_title_level_mode);
	DDX_Control(pDX, IDC_TITLE_DEVICE_MODE, m_title_device_mode);
	DDX_Control(pDX, IDC_MSG_STATUS_INFO, m_msg_status_info);
	DDX_Control(pDX, IDC_MSG_FORM_NAME, m_msg_form_name);
	DDX_Control(pDX, IDC_GIF_TITLE_LOGO, m_gif_title_logo);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialog_Title, CInitDialogBar)
	//{{AFX_MSG_MAP(CDialog_Title)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_STATUS_CHANGE,OnStatusDataUpdate)  // 타이틀 바 화면으로 전송된 메시지 출력시키기 위한 사용자 정의 메시지 추가 
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialog_Title message handlers

BOOL CDialog_Title::OnInitDialogBar() 
{
	CInitDialogBar::OnInitDialogBar();
	
	// **************************************************************************
	// 컨트롤에 설정할 폰트 생성한다
	// **************************************************************************
	mp_title_font = NULL;
	mp_title_font = new CFont;
	mp_title_font->CreateFont(15,0,0,0,900,0,0,0,0,0,0,ANTIALIASED_QUALITY,0,"MS Sans Serif");
	// **************************************************************************
	
	OnSetLabel();
	OnSetPicture(); // 타이틀 바 화면에 대한 GIF 그림 파일 로딩 함수



	OnCreateThread ();

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialog_Title::OnDestroy() 
{
	OnCloseThread();	// 생성한 쓰레드 종료

	// **************************************************************************
	// 생성한 폰트 정보 삭제한다
	// **************************************************************************
	delete mp_title_font;
	mp_title_font = NULL;
	// **************************************************************************

	CInitDialogBar::OnDestroy();
}

LRESULT CDialog_Title::OnStatusDataUpdate(WPARAM wParam,LPARAM lParam)
{
	int mn_change;			// PostMessage를 통해 수신된 메시지 ID 저장 변수 
	
	mn_change = lParam;						// 명령어 ID 정보 저장 
	
	if(wParam == FORM_NAME)					// 화면 뷰 전환인 경우 
	{
		OnDisplayFormName(mn_change);// 현재 출력된 뷰 화면 이름 정보 출력 함수
	}
	else if(wParam == MACHINE_STATUS)		// 장비 상태 정보 변경인 경우 
	{
		// 백상현 주석 [2014.08.29]
		// 쓰레드에서 자체적으로 출력하도록 수정함
		//OnDisplayStatus(mn_change);
	}
	else if(wParam == DEVICE_MODE)			// Device Mode
	{

	}
	else if(wParam == LEVEL_MODE)
	{
		OnDisplayLevelStatus();
	}
	else if(wParam == FRONT_KEY_MODE)
	{

	}
	else if(wParam == REAR_KEY_MODE)
	{

	}
	else if(wParam == RETEST_MODE)
	{

	}
	else if(wParam == MACIHNE_MODE)	// 20130822 kjh
	{

	}
	
	return 0;
}

void CDialog_Title::OnSetPicture()
{
	if (m_gif_title_logo.Load(MAKEINTRESOURCE(IDR_GIF_N_LOGO),_T("GIF")))
		m_gif_title_logo.Draw();
}

void CDialog_Title::OnSetLabel()
{
	m_msg_form_name.SetAlignment(CNewLabel::HORZ_LEFT | CNewLabel::VERT_CENTER);
	m_msg_form_name.SetTextColor(RGB(0, 0, 0));
	m_msg_form_name.SetBkColor(RGB(181, 182, 140));
	m_msg_form_name.SetFontBold(TRUE);
	m_msg_form_name.SetFontName("MS Sans Serif");
	m_msg_form_name.SetFontSize(12);
	
	m_msg_status_info.SetFont(mp_title_font);
	m_msg_status_info.SetCenterText();
	m_msg_status_info.SetColor(RGB(255,255,255));
	m_msg_status_info.SetGradientColor(RGB(0,255,0));
	m_msg_status_info.SetTextColor(RGB(255,255,255));

	m_title_device_mode.SetFont(mp_title_font);
	m_title_device_mode.SetCenterText();
	m_title_device_mode.SetColor(RGB(0,0,255));
	m_title_device_mode.SetGradientColor(RGB(0,0,0));
	m_title_device_mode.SetTextColor(RGB(255,255,255));
	
	m_title_level_mode.SetFont(mp_title_font);
	m_title_level_mode.SetCenterText();
	m_title_level_mode.SetColor(RGB(0,0,255));
	m_title_level_mode.SetGradientColor(RGB(0,0,0));
	m_title_level_mode.SetTextColor(RGB(255,255,255));

	m_title_bcr_mode.SetFont(mp_title_font);
	m_title_bcr_mode.SetCenterText();
	m_title_bcr_mode.SetColor(RGB(0,0,255));
	m_title_bcr_mode.SetGradientColor(RGB(0,0,0));
	m_title_bcr_mode.SetTextColor(RGB(255,255,255));

	m_title_cnnt_eqp2main.SetFont(mp_title_font);
	m_title_cnnt_eqp2main.SetCenterText();
	m_title_cnnt_eqp2main.SetColor(RGB(0,0,255));
	m_title_cnnt_eqp2main.SetGradientColor(RGB(0,0,0));
	m_title_cnnt_eqp2main.SetTextColor(RGB(255,255,255));
	
}

void CDialog_Title::OnDisplayFormName(int nzViewID)
{
	CString mstr_form_name;  // 현재 설정된 화면 정보 저장 변수 
	
	switch(nzViewID)
	{
	case IDW_SCREEN_MAIN:
		mstr_form_name = _T("\n");
		mstr_form_name += _T(" Screen: Main");
		break;
	case IDW_SCREEN_LOCK:
		mstr_form_name = _T("\n");
		mstr_form_name += _T(" Screen: System Lock");
		break;
	case IDW_SCREEN_INIT:
		mstr_form_name = _T("\n");
		mstr_form_name += _T(" Screen: Initial");
		break;
	case IDW_SCREEN_BASIC :
		mstr_form_name = _T("\n");
		mstr_form_name += _T(" Screen: Basic");
		break;
	case IDW_SCREEN_SET_MAINTENANCE :
		mstr_form_name = _T(" Menu: Setting");
		mstr_form_name += _T("\n");
		mstr_form_name += _T(" Screen: Maintenance");
		break;
	case IDW_SCREEN_WAIT_TIME :
		mstr_form_name += _T("\n");
		mstr_form_name += _T(" Screen: Wait Time");
		break;
	case IDW_SCREEN_MOTOR_TEACH:
		mstr_form_name = _T(" Menu: Motor");
		mstr_form_name += _T("\n");
		mstr_form_name += _T(" Screen: Teach");
		break;
	case IDW_SCREEN_MOTOR_SPEED :
		mstr_form_name = _T(" Menu: Motor");
		mstr_form_name += _T("\n");
		mstr_form_name += _T(" Screen: Speed");
		break;
	case IDW_SCREEN_LIST_OPERATION :
		mstr_form_name = _T(" Menu: List");
		mstr_form_name += _T("\n");
		mstr_form_name += _T(" Screen: Operation");
		break;
	case IDW_SCREEN_LIST_DATA_VIEW:
		mstr_form_name = _T(" Menu: List");
		mstr_form_name += _T("\n");
		mstr_form_name += _T(" Screen: Data View");
		break;
	case IDW_SCREEN_IO :
		mstr_form_name = _T("\n");
		mstr_form_name += _T(" Screen: I/O");
		break;
	case IDW_SCREEN_ADMINISTRATOR :
		mstr_form_name = _T("\n");
		mstr_form_name += _T(" Screen: ADMINISTRATOR");
		break;
	case IDW_SCREEN_FILE_EDITOR :
		mstr_form_name = _T("\n");
		mstr_form_name += _T(" Screen: File Editor");
		break;

	default:
		mstr_form_name = _T(" ERROR: SCREEN VIEW ID");
		break;
	}
	
	m_msg_form_name.SetText(_T(mstr_form_name));
}

void CDialog_Title::OnDisplayStatus(int nzStatus)
{
	switch(nzStatus)
	{
	case dSTOP :		// STOP 상태
		m_msg_status_info.SetWindowText(_T("STOP"));
		m_msg_status_info.SetTextColor(RGB(255,0,0));
		break;
	case dRUN :			// RUN 상태
		m_msg_status_info.SetWindowText(_T("RUN"));
		m_msg_status_info.SetTextColor(RGB(0, 0, 255));
		break;
	case dJAM :			// ALARM 상태
		m_msg_status_info.SetWindowText(_T("ALARM"));
		m_msg_status_info.SetTextColor(RGB(255,0,0));
		break;
	case dWARNING :		// ALARM 상태
		m_msg_status_info.SetWindowText(_T("ALARM"));
		m_msg_status_info.SetTextColor(RGB(255,0,0));
		break;
	case dMAINT :		// MAINT 상태
		m_msg_status_info.SetWindowText(_T("MAINT"));
		m_msg_status_info.SetTextColor(RGB(0,0,200));
		break;
	case dINIT :		// INIT 상태
		m_msg_status_info.SetWindowText(_T("INIT"));
		m_msg_status_info.SetTextColor(RGB(50,100,150));
		break;
	case dLOTEND :		// LOT END 상태
		m_msg_status_info.SetWindowText(_T("LOTEND"));
		m_msg_status_info.SetTextColor(RGB(50,100,150));
		break;
	case dLOCK :		// LOCK 상태
		m_msg_status_info.SetWindowText(_T("LOCK"));
		m_msg_status_info.SetTextColor(RGB(50,100,150));
		break;
	default :
		break;
	}
	
	m_msg_status_info.SetColor(RGB(255,255,255));
	m_msg_status_info.SetGradientColor(RGB(0,255,0));
}

void CDialog_Title::OnDisplayLevelStatus()
{
	CString str_level;   // 임시 저장 변수
	
	if (st_handler.mn_level_teach == TRUE)
	{
		str_level = _T("LEVEL 2 (Teaching Mode)");
		m_title_level_mode.SetColor(RGB(255,0,0));
		m_title_level_mode.SetWindowText(_T(str_level));
	}
	else if (st_handler.mn_level_maint == TRUE)
	{
		str_level = _T("LEVEL 1 (Maint Mode)");
		m_title_level_mode.SetColor(RGB(255,0,0));
		m_title_level_mode.SetWindowText(_T(str_level));
	}
	else
	{
		str_level = _T("LEVEL 0 (Operation Mode)");
		m_title_level_mode.SetColor(RGB(255,0,0));
		m_title_level_mode.SetWindowText(_T(str_level));
	}
}

void CDialog_Title::OnBuildEQPStatus()
{
	if (m_nEQP_Status == st_work.nEqpStatus)
	{
		return;
	}
	m_nEQP_Status = st_work.nEqpStatus;

	switch(st_work.nEqpStatus)
	{
	case dSTOP :		// STOP 상태
		m_msg_status_info.SetWindowText(_T("STOP"));
		m_msg_status_info.SetTextColor(RGB(255,0,0));
		break;
	case dRUN :			// RUN 상태
		m_msg_status_info.SetWindowText(_T("RUN"));
		m_msg_status_info.SetTextColor(RGB(0, 0, 255));
		break;
	case dJAM :			// ALARM 상태
		m_msg_status_info.SetWindowText(_T("ALARM"));
		m_msg_status_info.SetTextColor(RGB(255,0,0));
		break;
	case dWARNING :		// ALARM 상태
		m_msg_status_info.SetWindowText(_T("ALARM"));
		m_msg_status_info.SetTextColor(RGB(255,0,0));
		break;
	case dMAINT :		// MAINT 상태
		m_msg_status_info.SetWindowText(_T("MAINT"));
		m_msg_status_info.SetTextColor(RGB(0,0,200));
		break;
	case dINIT :		// INIT 상태
		m_msg_status_info.SetWindowText(_T("INIT"));
		m_msg_status_info.SetTextColor(RGB(50,100,150));
		break;
	case dLOTEND :		// LOT END 상태
		m_msg_status_info.SetWindowText(_T("LOTEND"));
		m_msg_status_info.SetTextColor(RGB(50,100,150));
		break;
	case dLOCK :		// LOCK 상태
		m_msg_status_info.SetWindowText(_T("LOCK"));
		m_msg_status_info.SetTextColor(RGB(50,100,150));
		break;
	default :
		break;
	}
	
	m_msg_status_info.SetColor(RGB(255,255,255));
	m_msg_status_info.SetGradientColor(RGB(0,255,0));
}

void CDialog_Title::OnBuildDeviceMode()
{
	CString str_device;   // 임시 저장 변수.

	if (m_nDVC_Mode == st_basic.n_mode_device)
	{
		return;
	}
	m_nDVC_Mode = st_basic.n_mode_device;

	switch(st_basic.n_mode_device)
	{
	case WITHOUT_DVC_:
		str_device = _T("WITHOUT DEVICE");
		m_title_device_mode.SetColor(RGB(255,0,0));
		break;
	case WITH_DVC_:
		str_device = _T("WITH DEVICE");
		m_title_device_mode.SetColor(RGB(0,0,255));
		break;
	default:
		str_device = _T("ERROR");
		m_title_device_mode.SetColor(RGB(255,0,0));
		break;
	}

	m_title_device_mode.SetWindowText(_T(str_device));
}

void CDialog_Title::OnBuildBCRMode()
{
	CString SBCRMode;

	if (m_nBCR_Mode == st_basic.nMode_BCR)
	{
		return;
	}
	m_nBCR_Mode = st_basic.nMode_BCR;
	
	switch(st_basic.nMode_BCR)
	{
	case 0:
		SBCRMode = _T("Continuity");
		m_title_bcr_mode.SetColor(RGB(0,0, 255));
		m_title_bcr_mode.SetWindowText(_T(SBCRMode));
		break;

	case 1:
		SBCRMode = _T("Step by Step");
		m_title_bcr_mode.SetColor(RGB(255,0,0));
		m_title_bcr_mode.SetWindowText(_T(SBCRMode));
		break;
	}
}

void CDialog_Title::OnBuildMainConnect()
{
	if (m_nState_Eqp2Main == st_work.nLink_Eqp2Main)
	{
		return;
	}
	m_nState_Eqp2Main = st_work.nLink_Eqp2Main;
	CString sUpdateData;

	switch(m_nState_Eqp2Main)
	{
	case 0:
		sUpdateData  = _T("MAIN : DISCONNECT");
		m_title_cnnt_eqp2main.SetColor(RGB(255,0,0));		
		m_title_cnnt_eqp2main.SetWindowText(_T(sUpdateData));
		break;

	case 1:
		sUpdateData  = _T("MAIN : CONNECT");
		m_title_cnnt_eqp2main.SetColor(RGB(0,0, 255));
		m_title_cnnt_eqp2main.SetWindowText(_T(sUpdateData));
		break;
	}
}

//==================================================================//
// 쓰레드 생성
//==================================================================//
BOOL CDialog_Title::OnCreateThread() 
{
	// 쓰레드가 생성되지 않은 경우 생성함
	if (m_bDispThreadFlag == FALSE)
	{
		m_bDispThreadFlag = TRUE;

		m_hDispThreadHandle = (HANDLE)_beginthreadex(NULL, 0, DispThreadProc, this, 0, &m_unDispThreadID);
		if (m_hDispThreadHandle)
		{
			return TRUE;
		}
		else	{
			return FALSE;
		}
	}
	return TRUE;
}

unsigned int __stdcall CDialog_Title::DispThreadProc(void *pParam) 
{
	// 클래스 포인터 캐스트
	CDialog_Title *pSelf = static_cast<CDialog_Title*>(pParam);

	while(pSelf->m_bDispThreadFlag)
	{
		pSelf->OnDispInfo();
	}

	CloseHandle(pSelf->m_hDispThreadHandle);	// 쓰레드 핸들 종료
	pSelf->m_hDispThreadHandle	= NULL;		// 쓰레드 핸들 초기화
	pSelf->m_bDispThreadFlag	= FALSE;	// 쓰레드 종료 처리

	return (0);
}

void CDialog_Title::OnDispInfo() 
{
	// 규이리 주석 처리 [2014.08.28]
	// 필요없을 듯 하여 주석 처리함
	/*
	CSingleLock sl(&m_csMotRead);
	sl.Lock();
	*/
	{

	}
	/*
	Sleep(100);
	sl.Unlock();
	*/

	// 백상현 추가 [2014.08.29]
	OnBuildEQPStatus();		// 설비 상태 표시
	OnBuildDeviceMode();	// 작업중인 디바이스 정보 표시
	OnBuildBCRMode();		// 작업중인 BCR 정보 표시
	OnBuildMainConnect();	// Main과의 접속 상태 표시
}

//==================================================================//
// 생성한 쓰레드 종료
//==================================================================//
void CDialog_Title::OnCloseThread() 
{
	if (m_bDispThreadFlag == TRUE)
	{
		m_bDispThreadFlag = FALSE;

		//::WaitForSingleObject((void *)m_unDispThreadID, 3000);
		::WaitForSingleObject(m_hDispThreadHandle, 3000);
	}
}
