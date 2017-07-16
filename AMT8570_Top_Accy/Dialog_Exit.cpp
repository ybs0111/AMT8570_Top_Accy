// Dialog_Exit.cpp : implementation file
//

#include "stdafx.h"
#include "Handler.h"
#include "Dialog_Exit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialog_Exit dialog


CDialog_Exit::CDialog_Exit(CWnd* pParent /*=NULL*/)
	: StandardDialog(CDialog_Exit::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialog_Exit)
	//}}AFX_DATA_INIT
	m_caption_main = "Program Exit Dialog Box";
//	m_is_shape_3d = YES;
}


void CDialog_Exit::DoDataExchange(CDataExchange* pDX)
{
	StandardDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialog_Exit)
	DDX_Control(pDX, IDC_MSG_EXIT, m_msg_exit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialog_Exit, StandardDialog)
	//{{AFX_MSG_MAP(CDialog_Exit)
	ON_BN_CLICKED(IDSAVEOK, OnSaveok)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialog_Exit message ICTs

BOOL CDialog_Exit::OnInitDialog() 
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
	mp_exit_font = NULL;
	mp_exit_font = new CFont;
	mp_exit_font->CreateFont(15,0,0,0,900,0,0,0,0,0,0,ANTIALIASED_QUALITY,0,"MS Sans Serif");
	// **************************************************************************

	OnExit_Button_Set();	// 대화 상자 칼라 버튼 생성 함수
	OnExit_Lable_Set();		// 칼라 텍스트 박스 생성 함수
		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialog_Exit::OnDestroy() 
{
	// **************************************************************************
	// 생성한 폰트 정보 삭제한다
	// **************************************************************************
	delete mp_exit_font;
	mp_exit_font = NULL;
	// **************************************************************************

	delete mp_btn_ok;		// OK 버튼에 대한 정보 삭제 
	delete mp_btn_cancel;	// Cancel 버튼에 대한 정보 삭제 
	delete mp_btn_saveok;	// Save & OK 버튼에 대한 정보 삭제 

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

void CDialog_Exit::OnSaveok() 
{
	StandardDialog::EndDialog(5);	
}

void CDialog_Exit::OnExit_Button_Set()
{
	mp_btn_ok =		CreateOkButton(IDOK, this);
	mp_btn_cancel = CreateCancelButton(IDCANCEL, this);
	mp_btn_saveok = CreateSaveOkButton(IDSAVEOK, this);
}

void CDialog_Exit::OnExit_Lable_Set()
{
	m_msg_exit.SetFont(mp_exit_font);
	m_msg_exit.SetWindowText(_T("AMT8570 SSD Packing Accy Program Exit"));
	m_msg_exit.SetCenterText();
	m_msg_exit.SetColor(RGB(0,0,0));
	m_msg_exit.SetGradientColor(RGB(0,0,255));
	m_msg_exit.SetTextColor(RGB(255,255,255));
}
