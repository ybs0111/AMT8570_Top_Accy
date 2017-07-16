// Dialog_Message.cpp : implementation file
//

#include "stdafx.h"
#include "Handler.h"
#include "Dialog_Message.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialog_Message dialog

CDialog_Message::CDialog_Message(CWnd* pParent /*=NULL*/)
	: StandardDialog(CDialog_Message::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialog_Message)
	//}}AFX_DATA_INIT
	m_caption_main = "User Message Dialog Box";
//	m_is_shape_3d = YES;
}

void CDialog_Message::DoDataExchange(CDataExchange* pDX)
{
	StandardDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialog_Message)
	DDX_Control(pDX, IDC_MSG_MESSAGE, m_msg_message);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDialog_Message, StandardDialog)
	//{{AFX_MSG_MAP(CDialog_Message)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialog_Message message ICTs

BOOL CDialog_Message::OnInitDialog() 
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
	mp_msg_font = NULL;
	mp_msg_font = new CFont;
	mp_msg_font->CreateFont(15,0,0,0,900,0,0,0,0,0,0,ANTIALIASED_QUALITY,0,"MS Sans Serif");
	// **************************************************************************
	
	OnMsg_Button_Set(); // 대화 상자 칼라 버튼 생성 함수
	OnMsg_Lable_Set();  // 칼라 텍스트 박스 생성 함수
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialog_Message::OnDestroy() 
{
	// **************************************************************************
	// 생성한 폰트 정보 삭제한다                                                 
	// **************************************************************************
	delete mp_msg_font;
	mp_msg_font = NULL;
	// **************************************************************************

	delete mp_msg_ok;  // OK 버튼에 설정된 정보 삭제 

	if (mn_dialog == 1)
	{
		st_handler.nSwitchLock = FALSE;
	}

	if (st_handler.nMenuLock != FALSE)
	{
		st_handler.nMenuLock = FALSE;  // 메뉴 사용 가능
	}

	StandardDialog::OnDestroy();	
}

void CDialog_Message::OnMsg_Button_Set()
{
	mp_msg_ok = CreateOkButton(IDOK, this);
}

void CDialog_Message::OnMsg_Lable_Set()
{
	m_msg_message.SetFont(mp_msg_font);
	m_msg_message.SetWindowText(_T(st_msg.str_fallacy_msg));
	m_msg_message.SetCenterText();
	m_msg_message.SetColor(RGB(0,0,255));
	m_msg_message.SetGradientColor(RGB(0,0,0));
	m_msg_message.SetTextColor(RGB(255,255,255));
}