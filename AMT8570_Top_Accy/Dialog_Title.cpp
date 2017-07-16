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
	m_bDispThreadFlag	= FALSE;	// ������ �������� �ʾ���
	m_hDispThreadHandle	= NULL;		// ������ �ڵ� ���� �ʱ�ȭ
	m_unDispThreadID	= NULL;		// ������ ID ���� �ʱ�ȭ
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
	ON_MESSAGE(WM_STATUS_CHANGE,OnStatusDataUpdate)  // Ÿ��Ʋ �� ȭ������ ���۵� �޽��� ��½�Ű�� ���� ����� ���� �޽��� �߰� 
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialog_Title message handlers

BOOL CDialog_Title::OnInitDialogBar() 
{
	CInitDialogBar::OnInitDialogBar();
	
	// **************************************************************************
	// ��Ʈ�ѿ� ������ ��Ʈ �����Ѵ�
	// **************************************************************************
	mp_title_font = NULL;
	mp_title_font = new CFont;
	mp_title_font->CreateFont(15,0,0,0,900,0,0,0,0,0,0,ANTIALIASED_QUALITY,0,"MS Sans Serif");
	// **************************************************************************
	
	OnSetLabel();
	OnSetPicture(); // Ÿ��Ʋ �� ȭ�鿡 ���� GIF �׸� ���� �ε� �Լ�



	OnCreateThread ();

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialog_Title::OnDestroy() 
{
	OnCloseThread();	// ������ ������ ����

	// **************************************************************************
	// ������ ��Ʈ ���� �����Ѵ�
	// **************************************************************************
	delete mp_title_font;
	mp_title_font = NULL;
	// **************************************************************************

	CInitDialogBar::OnDestroy();
}

LRESULT CDialog_Title::OnStatusDataUpdate(WPARAM wParam,LPARAM lParam)
{
	int mn_change;			// PostMessage�� ���� ���ŵ� �޽��� ID ���� ���� 
	
	mn_change = lParam;						// ��ɾ� ID ���� ���� 
	
	if(wParam == FORM_NAME)					// ȭ�� �� ��ȯ�� ��� 
	{
		OnDisplayFormName(mn_change);// ���� ��µ� �� ȭ�� �̸� ���� ��� �Լ�
	}
	else if(wParam == MACHINE_STATUS)		// ��� ���� ���� ������ ��� 
	{
		// ����� �ּ� [2014.08.29]
		// �����忡�� ��ü������ ����ϵ��� ������
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
	CString mstr_form_name;  // ���� ������ ȭ�� ���� ���� ���� 
	
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
	case dSTOP :		// STOP ����
		m_msg_status_info.SetWindowText(_T("STOP"));
		m_msg_status_info.SetTextColor(RGB(255,0,0));
		break;
	case dRUN :			// RUN ����
		m_msg_status_info.SetWindowText(_T("RUN"));
		m_msg_status_info.SetTextColor(RGB(0, 0, 255));
		break;
	case dJAM :			// ALARM ����
		m_msg_status_info.SetWindowText(_T("ALARM"));
		m_msg_status_info.SetTextColor(RGB(255,0,0));
		break;
	case dWARNING :		// ALARM ����
		m_msg_status_info.SetWindowText(_T("ALARM"));
		m_msg_status_info.SetTextColor(RGB(255,0,0));
		break;
	case dMAINT :		// MAINT ����
		m_msg_status_info.SetWindowText(_T("MAINT"));
		m_msg_status_info.SetTextColor(RGB(0,0,200));
		break;
	case dINIT :		// INIT ����
		m_msg_status_info.SetWindowText(_T("INIT"));
		m_msg_status_info.SetTextColor(RGB(50,100,150));
		break;
	case dLOTEND :		// LOT END ����
		m_msg_status_info.SetWindowText(_T("LOTEND"));
		m_msg_status_info.SetTextColor(RGB(50,100,150));
		break;
	case dLOCK :		// LOCK ����
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
	CString str_level;   // �ӽ� ���� ����
	
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
	case dSTOP :		// STOP ����
		m_msg_status_info.SetWindowText(_T("STOP"));
		m_msg_status_info.SetTextColor(RGB(255,0,0));
		break;
	case dRUN :			// RUN ����
		m_msg_status_info.SetWindowText(_T("RUN"));
		m_msg_status_info.SetTextColor(RGB(0, 0, 255));
		break;
	case dJAM :			// ALARM ����
		m_msg_status_info.SetWindowText(_T("ALARM"));
		m_msg_status_info.SetTextColor(RGB(255,0,0));
		break;
	case dWARNING :		// ALARM ����
		m_msg_status_info.SetWindowText(_T("ALARM"));
		m_msg_status_info.SetTextColor(RGB(255,0,0));
		break;
	case dMAINT :		// MAINT ����
		m_msg_status_info.SetWindowText(_T("MAINT"));
		m_msg_status_info.SetTextColor(RGB(0,0,200));
		break;
	case dINIT :		// INIT ����
		m_msg_status_info.SetWindowText(_T("INIT"));
		m_msg_status_info.SetTextColor(RGB(50,100,150));
		break;
	case dLOTEND :		// LOT END ����
		m_msg_status_info.SetWindowText(_T("LOTEND"));
		m_msg_status_info.SetTextColor(RGB(50,100,150));
		break;
	case dLOCK :		// LOCK ����
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
	CString str_device;   // �ӽ� ���� ����.

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
// ������ ����
//==================================================================//
BOOL CDialog_Title::OnCreateThread() 
{
	// �����尡 �������� ���� ��� ������
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
	// Ŭ���� ������ ĳ��Ʈ
	CDialog_Title *pSelf = static_cast<CDialog_Title*>(pParam);

	while(pSelf->m_bDispThreadFlag)
	{
		pSelf->OnDispInfo();
	}

	CloseHandle(pSelf->m_hDispThreadHandle);	// ������ �ڵ� ����
	pSelf->m_hDispThreadHandle	= NULL;		// ������ �ڵ� �ʱ�ȭ
	pSelf->m_bDispThreadFlag	= FALSE;	// ������ ���� ó��

	return (0);
}

void CDialog_Title::OnDispInfo() 
{
	// ���̸� �ּ� ó�� [2014.08.28]
	// �ʿ���� �� �Ͽ� �ּ� ó����
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

	// ����� �߰� [2014.08.29]
	OnBuildEQPStatus();		// ���� ���� ǥ��
	OnBuildDeviceMode();	// �۾����� ����̽� ���� ǥ��
	OnBuildBCRMode();		// �۾����� BCR ���� ǥ��
	OnBuildMainConnect();	// Main���� ���� ���� ǥ��
}

//==================================================================//
// ������ ������ ����
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
