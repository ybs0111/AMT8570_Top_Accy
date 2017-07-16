// Dialog_SetReset.cpp : implementation file
//

#include "stdafx.h"
#include "handler.h"
#include "Dialog_SetReset.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialog_SetReset dialog


CDialog_SetReset::CDialog_SetReset(CWnd* pParent /*=NULL*/)
	: StandardDialog(CDialog_SetReset::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialog_SetReset)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDialog_SetReset::DoDataExchange(CDataExchange* pDX)
{
	StandardDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialog_SetReset)
	DDX_Control(pDX, IDC_INFO_MESSAGE, m_info_message);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialog_SetReset, StandardDialog)
	//{{AFX_MSG_MAP(CDialog_SetReset)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_SET, OnButtonSet)
	ON_BN_CLICKED(IDC_BUTTON_RESET, OnButtonReset)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialog_SetReset message handlers

BOOL CDialog_SetReset::OnInitDialog() 
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
	mp_info_font = NULL;
	mp_info_font = new CFont;
	mp_info_font->CreateFont(15,0,0,0,900,0,0,0,0,0,0,ANTIALIASED_QUALITY,0,"MS Sans Serif");
	// **************************************************************************
	
	OnInfo_Button_Set();	// 대화 상자 칼라 버튼 생성 함수
	OnInfo_Lable_Set();		// 칼라 텍스트 박스 생성 함수
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialog_SetReset::OnDestroy() 
{
	StandardDialog::OnDestroy();
	
	if (st_handler.nMenuLock != FALSE)
	{
		st_handler.nMenuLock = FALSE;  // 메뉴 사용 가능
	}

	st_handler.nSwitchLock = FALSE;
	
}

void CDialog_SetReset::OnInfo_Button_Set()
{
}

void CDialog_SetReset::OnInfo_Lable_Set()
{
	m_info_message.SetFont(mp_info_font);
	m_info_message.SetWindowText(_T(st_msg.mstr_confirm_msg));
	m_info_message.SetCenterText();
	m_info_message.SetColor(RGB(0,0,255));
	m_info_message.SetGradientColor(RGB(0,0,0));
	m_info_message.SetTextColor(RGB(255,255,255));
}


void CDialog_SetReset::OnButtonSet() 
{
	mn_Result = 1;
	OnOK();
}

void CDialog_SetReset::OnButtonReset() 
{
	mn_Result = 0;
	OnOK();
}
