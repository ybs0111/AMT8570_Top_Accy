// Dialog_Menu.cpp : implementation file
//

#include "stdafx.h"
#include "Handler.h"
#include "Dialog_Menu.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialog_Menu dialog


CDialog_Menu::CDialog_Menu(CWnd* pParent /*=NULL*/)
	: StandardDialog(CDialog_Menu::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialog_Menu)
	//}}AFX_DATA_INIT

	// Start Switch의 동작을 막는다.
	m_caption_main = "Select Menu";
//	m_is_shape_3d = YES;
}


void CDialog_Menu::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialog_Menu)
	DDX_Control(pDX, IDC_MSG_MENU, m_msg_menu);
	DDX_Control(pDX, IDC_LIST_MENU, m_list_menu);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialog_Menu, StandardDialog)
	//{{AFX_MSG_MAP(CDialog_Menu)
	ON_WM_DESTROY()
	ON_LBN_SELCHANGE(IDC_LIST_MENU, OnSelchangeListMenu)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialog_Menu message ICTs

BOOL CDialog_Menu::OnInitDialog() 
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
	mp_menu_font = NULL;
	mp_menu_font = new CFont;
	mp_menu_font->CreateFont(18,0,0,0,900,0,0,0,0,0,0,ANTIALIASED_QUALITY,0,"MS Sans Serif");
	// **************************************************************************
	
	OnMenu_Button_Set();
	OnMenu_Lable_Set();
	
	OnCreate_Menu_ListBox();	// 리스트 박스 컨트롤에 메뉴 텍스트 정보 추가 함수
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialog_Menu::OnDestroy() 
{
	// **************************************************************************
	// 생성한 폰트 정보 삭제한다                                                 
	// **************************************************************************
	delete mp_menu_font;
	mp_menu_font = NULL;
	// **************************************************************************
	
	delete mp_menu_ok;  // OK 버튼에 설정된 정보 삭제

	mp_menu_image_list.DeleteImageList();  // 이미지 리스트 정보 삭제

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

void CDialog_Menu::OnMenu_Button_Set()
{
	mp_menu_ok = CreateOkButton(IDOK, this);
}

void CDialog_Menu::OnMenu_Lable_Set()
{
	m_msg_menu.SetFont(mp_menu_font);
	m_msg_menu.SetWindowText(_T("Select Menu"));
	m_msg_menu.SetCenterText();
	m_msg_menu.SetColor(RGB(0,0,255));
	m_msg_menu.SetGradientColor(RGB(0,0,0));
	m_msg_menu.SetTextColor(RGB(255,255,255));
}

void CDialog_Menu::OnCreate_Menu_ListBox()
{
	OnCreate_Menu_Image_List(); // 리스트 박스 컨트롤에 아이콘 설정 함수

	// Associate image list to list box
	m_list_menu.SetImageList(&mp_menu_image_list);

	if (st_msg.mstr_parent_menu == "Setting Screen")		// Setting 메뉴에 대한 서브 메뉴 구성 
	{
		m_list_menu.AddString(_T("Maintenance"), 0);
		m_list_menu.AddString(_T("Test Reference"), 1);
//		m_list_menu.AddString(_T("Interface"), 2);
	}
	else if (st_msg.mstr_parent_menu == "Motor Screen")	// Robot 모터 메뉴에 대한 서브 메뉴 구성 
	{
		m_list_menu.AddString(_T("Teach"), 0);
		m_list_menu.AddString(_T("Speed"), 1);
	}
	else if (st_msg.mstr_parent_menu == "List Screen")		// 리스트 메뉴에 대한 서브 메뉴 구성 
	{
		m_list_menu.AddString(_T("Operation"), 0);
		//m_list_menu.AddString(_T("DataMan"), 1);
	}
}

void CDialog_Menu::OnCreate_Menu_Image_List()
{
	BOOL bRetValue = FALSE;
	HICON hIcon = NULL;

	// **************************************************************************
	// 리스트 박스 컨트롤에 적용할 이미지 생성한다.                              
	// **************************************************************************
	bRetValue = mp_menu_image_list.Create(32, 32, ILC_COLOR32 | ILC_MASK, 5, 1);
	ASSERT(bRetValue == TRUE);
	// **************************************************************************

	// **************************************************************************
	// 리스트 박스 컨트롤에 생성한 아이콘을 추가한다.                            
	// **************************************************************************
	if (st_msg.mstr_parent_menu == "Setting Screen")  // Setting 메뉴에 대한 서브 메뉴 구성 
	{
		hIcon = AfxGetApp()->LoadIcon(IDI_MAINTENANCE);
		mp_menu_image_list.Add(hIcon);

		hIcon = AfxGetApp()->LoadIcon(IDI_TRAY);
		mp_menu_image_list.Add(hIcon);

// 		hIcon = AfxGetApp()->LoadIcon(IDI_MAINTENANCE);
// 		mp_menu_image_list.Add(hIcon);
	}
	else if (st_msg.mstr_parent_menu == "Motor Screen")  // Robot 모터 메뉴에 대한 서브 메뉴 구성 
	{
		hIcon = AfxGetApp()->LoadIcon(IDI_MOT_POS);
		mp_menu_image_list.Add(hIcon);

		hIcon = AfxGetApp()->LoadIcon(IDI_MOT_SPD);
		mp_menu_image_list.Add(hIcon);
	}
	else if (st_msg.mstr_parent_menu == "List Screen")  // 리스트 메뉴에 대한 서브 메뉴 구성 
	{
		hIcon = AfxGetApp()->LoadIcon(IDI_LIST_ARM);
		mp_menu_image_list.Add(hIcon);

		hIcon = AfxGetApp()->LoadIcon(IDI_LIST_WORK);
		mp_menu_image_list.Add(hIcon);
	}
}

void CDialog_Menu::OnMenu_ID_Sending(CString str_selected_menu)
{
	if (str_selected_menu == "Maintenance")
	{
		::PostMessage(st_handler.hWnd, WM_FORM_CHANGE, 3, 1);
	}
	else if (str_selected_menu == "Interface")
	{
		::PostMessage(st_handler.hWnd, WM_FORM_CHANGE, 3, 2);
	}
	else if (str_selected_menu == "Test Reference")
	{
		::PostMessage(st_handler.hWnd, WM_FORM_CHANGE, 3, 3);
	}
	else if (str_selected_menu == "Teach")
	{
		::PostMessage(st_handler.hWnd, WM_FORM_CHANGE, 5, 1);
	}
	else if (str_selected_menu == "Speed")
	{
		::PostMessage(st_handler.hWnd, WM_FORM_CHANGE, 5, 2);
	}
	else if (str_selected_menu == "Operation")
	{
		::PostMessage(st_handler.hWnd, WM_FORM_CHANGE, 6, 1);
	}
	else if (str_selected_menu == "DataMan")
	{
		::PostMessage(st_handler.hWnd, WM_FORM_CHANGE, 6, 2);
	}

	StandardDialog::OnOK();
}

void CDialog_Menu::OnSelchangeListMenu() 
{
	int mn_index = m_list_menu.GetCurSel();				// 현재 선택된 셀 정보를 얻는다. 

	if (mn_index != LB_ERR)
	{
		m_list_menu.GetText(mn_index, mp_menu_name);	// 선택된 셀에 대한 문자열 정보를 얻는다. 

		OnMenu_ID_Sending(mp_menu_name);				// 리스트 박스 컨트롤에서 선택된 메뉴에 대한 메시지 전송 함수
	}
}