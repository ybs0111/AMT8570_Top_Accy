// Dialog_Multi_Msg.cpp : implementation file
//

#include "stdafx.h"
#include "Handler.h"
#include "Dialog_Multi_Msg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialog_Multi_Msg dialog


CDialog_Multi_Msg::CDialog_Multi_Msg(CWnd* pParent /*=NULL*/)
	: StandardDialog(CDialog_Multi_Msg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialog_Multi_Msg)
	//}}AFX_DATA_INIT
	m_caption_main = "User Message Dialog Box";
}


void CDialog_Multi_Msg::DoDataExchange(CDataExchange* pDX)
{
	StandardDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialog_Multi_Msg)
	DDX_Control(pDX, IDC_MSG_M_MESSAGE, m_msg_m_message);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialog_Multi_Msg, StandardDialog)
	//{{AFX_MSG_MAP(CDialog_Multi_Msg)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialog_Multi_Msg message ICTs

BOOL CDialog_Multi_Msg::OnInitDialog() 
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
	mp_m_msg_font = NULL;
	mp_m_msg_font = new CFont;
	mp_m_msg_font->CreateFont(15,0,0,0,900,0,0,0,0,0,0,ANTIALIASED_QUALITY,0,"MS Sans Serif");
	// **************************************************************************
	
	OnMMsg_Button_Set(); // 대화 상자 칼라 버튼 생성 함수
	OnMMsg_Lable_Set();  // 칼라 텍스트 박스 생성 함수
		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialog_Multi_Msg::OnDestroy() 
{
	// **************************************************************************
	// 생성한 폰트 정보 삭제한다                                                 
	// **************************************************************************
	delete mp_m_msg_font;
	mp_m_msg_font = NULL;
	// **************************************************************************

	delete mp_m_msg_ok;  // OK 버튼에 설정된 정보 삭제 

	if (mn_dialog == 1)
	{
		st_handler.nSwitchLock = FALSE;
	}

	StandardDialog::OnDestroy();
}

void CDialog_Multi_Msg::OnMMsg_Lable_Set()
{
	m_msg_m_message.SetAlignment(CNewLabel::HORZ_CENTER | CNewLabel::VERT_CENTER);
	m_msg_m_message.SetTextColor(RGB(0, 0, 0));
	m_msg_m_message.SetBkColor(RGB(181, 182, 140));
	m_msg_m_message.SetFontBold(TRUE);
	m_msg_m_message.SetFontName("MS Sans Serif");
	m_msg_m_message.SetFontSize(20);

	m_msg_m_message.SetText(_T(st_msg.str_fallacy_msg));
}

void CDialog_Multi_Msg::OnMMsg_Button_Set()
{
	mp_m_msg_ok = CreateOkButton(IDOK, this);
}
