// Dialog_Select.cpp : implementation file
//

#include "stdafx.h"
#include "Handler.h"
#include "Dialog_Select.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialog_Select dialog


CDialog_Select::CDialog_Select(CWnd* pParent /*=NULL*/)
	: StandardDialog(CDialog_Select::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialog_Select)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_caption_main = "User Select Dialog Box"; 
//	m_is_shape_3d = YES;
}


void CDialog_Select::DoDataExchange(CDataExchange* pDX)
{
	StandardDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialog_Select)
	DDX_Control(pDX, IDC_SELECT_MESSAGE, m_select_message);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialog_Select, StandardDialog)
	//{{AFX_MSG_MAP(CDialog_Select)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDEXIT, OnExit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialog_Select message ICTs

BOOL CDialog_Select::OnInitDialog() 
{
	StandardDialog::OnInitDialog();
	
	// Start Switch의 동작을 막는다.
	if (st_handler.nSwitchLock == FALSE)
	{
		st_handler.nSwitchLock = TRUE;
		mn_dialog = 1;
	}
	else	mn_dialog = 0;
	
	// **************************************************************************
	// 컨트롤에 설정할 폰트 생성한다                                             
	// **************************************************************************
	mp_select_font = NULL;
	mp_select_font = new CFont;
	mp_select_font->CreateFont(15,0,0,0,900,0,0,0,0,0,0,ANTIALIASED_QUALITY,0,"MS Sans Serif");
	// **************************************************************************

	OnSelect_Button_Set();	// 대화 상자 칼라 버튼 생성 함수
	OnSelect_Lable_Set();	// 칼라 텍스트 박스 생성 함수
		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialog_Select::OnDestroy() 
{
	// **************************************************************************
	// 생성한 폰트 정보 삭제한다                                                 
	// **************************************************************************
	delete mp_select_font;
	mp_select_font = NULL;
	// **************************************************************************

	delete mp_select_exit;	// OK 버튼에 설정된 정보 삭제 
	delete mp_select_yes;	// OK 버튼에 설정된 정보 삭제 
	delete mp_select_no;	// Cancel 버튼에 설정된 정보 삭제 

	if (mn_dialog == 1)
	{
		st_handler.nSwitchLock = FALSE;
	}

	if (st_handler.nMenuLock != FALSE)
	{
		st_handler.nMenuLock = FALSE;  // 메뉴 사용 가능
	}

	st_handler.mn_board_onoff_dialog = CTL_NO;

	StandardDialog::OnDestroy();	
}

void CDialog_Select::OnSelect_Lable_Set()
{
	m_select_message.SetFont(mp_select_font);
	m_select_message.SetWindowText(_T(st_msg.mstr_confirm_msg));
	m_select_message.SetCenterText();
	m_select_message.SetColor(RGB(0,0,255));
	m_select_message.SetGradientColor(RGB(0,0,0));
	m_select_message.SetTextColor(RGB(255,255,255));
}

void CDialog_Select::OnSelect_Button_Set()
{
	if (st_handler.mn_board_onoff_dialog == CTL_YES)
	{
		SetDlgItemText(IDOK, "ON");
		SetDlgItemText(IDCANCEL, "OFF");
	}
	
	mp_select_yes = CreateOkButton(IDOK, this);
	mp_select_no = CreateCancelButton(IDCANCEL, this);
	mp_select_exit = CreateCancelButton(IDEXIT, this);

	if (st_handler.mn_board_onoff_dialog == CTL_YES)
	{
		mp_select_exit->ShowWindow(SW_SHOW);
	}
	else
	{
		mp_select_exit->ShowWindow(SW_HIDE);
	}
}

void CDialog_Select::OnExit() 
{
	StandardDialog::EndDialog(5);
}

BOOL CDialog_Select::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_ESCAPE)
		{
			StandardDialog::EndDialog(5);
		}
	}

	return StandardDialog::PreTranslateMessage(pMsg);
}
