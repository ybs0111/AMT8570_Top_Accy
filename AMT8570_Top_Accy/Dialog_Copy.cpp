// Dialog_Copy.cpp : implementation file
//

#include "stdafx.h"
#include "handler.h"
#include "Dialog_Copy.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialog_Copy dialog


CDialog_Copy::CDialog_Copy(CWnd* pParent /*=NULL*/)
	: StandardDialog(CDialog_Copy::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialog_Copy)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_caption_main = "Input New Device Name";
}


void CDialog_Copy::DoDataExchange(CDataExchange* pDX)
{
	StandardDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialog_Copy)
	DDX_Control(pDX, IDC_MSG_DC_ORG_NAME, m_msg_org_name);
	DDX_Control(pDX, IDC_BTN_DC_OK, m_btn_ok);
	DDX_Control(pDX, IDC_BTN_DC_CANCEL, m_btn_cancel);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialog_Copy, StandardDialog)
	//{{AFX_MSG_MAP(CDialog_Copy)
	ON_BN_CLICKED(IDC_BTN_DC_OK, OnBtnDcOk)
	ON_BN_CLICKED(IDC_BTN_DC_CANCEL, OnBtnDcCancel)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialog_Copy message handlers
BOOL CDialog_Copy::OnInitDialog() 
{
	StandardDialog::OnInitDialog();	
	
	m_msg_org_name.SetTextColor(RGB(255, 255, 255));
	m_msg_org_name.SetBkColor(RGB(50, 100, 150));
	m_msg_org_name.SetFontBold(TRUE);
	m_msg_org_name.SetFontName("Arial");
	m_msg_org_name.SetFontSize(12);	
	m_msg_org_name.SetText("  Source file Name : " + mstr_selected_device_name);


	m_edit_new_name.SubclassDlgItem(IDC_EDIT_DC_NEW_NAME, this );
	m_edit_new_name.bkColor( RGB(50, 100, 150) );
	m_edit_new_name.textColor( RGB(255, 255,255) );
    m_edit_new_name.setFont(-16, FW_BOLD, DEFAULT_PITCH | FF_DONTCARE, "Arial");


	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CDialog_Copy::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message == WM_KEYDOWN) //
	{
		if(pMsg->wParam == VK_RETURN)
		{
			OnBtnDcOk();
			return true;
		}
	}
	return StandardDialog::PreTranslateMessage(pMsg);
}

void CDialog_Copy::OnBtnDcOk() 
{
	CFileFind find;
	BOOL bResult = false;
	int n_pos;

	CString str_org_file = st_path.mstr_path_dvc;
	CString str_file       = _T("");
	CString str_check_file;


	((CEdit*)GetDlgItem(IDC_EDIT_DC_NEW_NAME))->GetWindowText(str_check_file);
	str_check_file.MakeUpper();

	if(str_check_file.GetLength() < 3)		//적어도 3자 이상은 되야한다!
	{
		if(st_handler.cwnd_list != NULL)
		{
			sprintf(st_msg.c_abnormal_msg, _T("File Name is too short!!"));
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
		}
		return;
	}

	n_pos = str_check_file.Find(".");  // 확장자 위치 검사
	if(n_pos == -1)
	{
		str_check_file += _T(".TXT");  // 확장자 추가
	}

	str_file = st_path.mstr_path_dvc + str_check_file;

	bResult = find.FindFile(str_file);
	find.Close();

	if(bResult != 0)
	{
		//이미 파일이 존재함
		if(st_handler.cwnd_list != NULL)
		{
			sprintf(st_msg.c_abnormal_msg, _T("[Error] Device Existence!"));
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
		}
		return;
	}
	else
	{
		str_org_file += mstr_selected_device_name;
		//파일이 없음!! 선택한 파일을 다른 이름으로 복사하자!!
		CopyFile(str_org_file, str_file, FALSE);
		StandardDialog::OnOK();
	}
}

void CDialog_Copy::OnBtnDcCancel() 
{
	StandardDialog::OnOK();
}



void CDialog_Copy::OnDestroy() 
{
	if(iDialogPopup == TRUE) //lock이 걸린 대화상자라면.. switch lock을 해제하기 위함
	{
	}

	StandardDialog::OnDestroy();

	if (st_handler.nMenuLock != FALSE)
	{
		st_handler.nMenuLock = FALSE;  // 메뉴 사용 가능
	}

	st_handler.nSwitchLock = FALSE;
}
