// Screen_List_Operation.cpp : implementation file
//

#include "stdafx.h"
#include "Handler.h"
#include "Screen_List_Operation.h"

#include "io.h" 
#include "FileDialogST.h"

/* ****************************************************************************** */
/* 대화 상자 클래스 추가                                                          */
/* ****************************************************************************** */
#include "Dialog_Infor.h"
#include "Dialog_Select.h"
#include "Dialog_Message.h"
/* ****************************************************************************** */

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScreen_List_Operation

IMPLEMENT_DYNCREATE(CScreen_List_Operation, CFormView)

CScreen_List_Operation::CScreen_List_Operation()
	: CFormView(CScreen_List_Operation::IDD)
{
	//{{AFX_DATA_INIT(CScreen_List_Operation)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CScreen_List_Operation::~CScreen_List_Operation()
{
}

void CScreen_List_Operation::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CScreen_List_Operation)
	DDX_Control(pDX, IDC_RADIO_WORK, m_radio_work);
	DDX_Control(pDX, IDC_RADIO_ALARM, m_radio_alarm);
	DDX_Control(pDX, IDC_RADIO_OPERATION, m_radio_operation);
	DDX_Control(pDX, IDC_RADIO_MACHINE, m_radio_machine);
	DDX_Control(pDX, IDC_RADIO_INTERFACE, m_radio_interface);
	DDX_Control(pDX, IDC_MSG_OPERATION_FILE_NAME, m_msg_operation_file_name);
	DDX_Control(pDX, IDC_MSG_OPERATION_FILE, m_msg_operation_file);
	DDX_Control(pDX, IDC_MONTHCAL_OPERATION, m_monthcal_operation);
	DDX_Control(pDX, IDC_LIST_OPERATION_FILE, m_list_operation_file);
	DDX_Control(pDX, IDC_GROUP_OPERATION_TYPE, m_group_operation_type);
	DDX_Control(pDX, IDC_GROUP_OPERATION_LIST, m_group_operation_list);
	DDX_Control(pDX, IDC_BTN_OPERATION_FILE_REFRESH, m_btn_operation_file_refresh);
	DDX_Control(pDX, IDC_BTN_OPERATION_FILE_DEL, m_btn_operation_file_del);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CScreen_List_Operation, CFormView)
	//{{AFX_MSG_MAP(CScreen_List_Operation)
	ON_WM_DESTROY()
	ON_LBN_SELCHANGE(IDC_LIST_OPERATION_FILE, OnSelchangeListOperationFile)
	ON_NOTIFY(MCN_SELECT, IDC_MONTHCAL_OPERATION, OnSelectMonthcalOperation)
	ON_BN_CLICKED(IDC_RADIO_OPERATION, OnRadioOperation)
	ON_BN_CLICKED(IDC_RADIO_INTERFACE, OnRadioInterface)
	ON_BN_CLICKED(IDC_RADIO_MACHINE, OnRadioMachine)
	ON_BN_CLICKED(IDC_BTN_OPERATION_FILE_DEL, OnBtnOperationFileDel)
	ON_BN_CLICKED(IDC_BTN_OPERATION_FILE_REFRESH, OnBtnOperationFileRefresh)
	ON_BN_CLICKED(IDC_RADIO_ALARM, OnRadioAlarm)
	ON_BN_CLICKED(IDC_RADIO_WORK, OnRadioWork)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScreen_List_Operation diagnostics

#ifdef _DEBUG
void CScreen_List_Operation::AssertValid() const
{
	CFormView::AssertValid();
}

void CScreen_List_Operation::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CScreen_List_Operation message ICTs

void CScreen_List_Operation::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	/* ************************************************************************** */
	/* 컨트롤에 설정할 폰트 생성한다                                              */
	/* ************************************************************************** */
	mp_operation_list_font = NULL;
	mp_operation_list_font = new CFont;
	mp_operation_list_font->CreateFont(15,0,0,0,900,0,0,0,0,0,0,ANTIALIASED_QUALITY,0,"MS Sans Serif");
	/* ************************************************************************** */

	/* ************************************************************************** */
    /* 출력할 알람 BIN 결과 저장 파일 정보 설정                                 */
    /* ************************************************************************** */
	mn_type_select = 1;					// 초기 상태 정보 설정 

	mstr_selected_Date.Empty();			// 선택된 날짜 정보 저장 변수 초기화 
	mstr_operation_file_name.Empty();	// 선택된 파일 이름을 저장하기 위한 변수 선언

	mstr_default_path = CONFIG_PATH_BASE_;

	OnOperation_List_Full_Path_Set(mn_type_select);			// 파일 정보 로딩할 전체 폴더 정보 설정 함수 */
	/* ************************************************************************** */

	mstr_file_full_name = "";								// Save as용 Reset~ 2K4/08/16/ViboX

	OnOperation_List_GroupBox_Set();
	OnOperation_List_Lable_Set();
	OnOperation_List_EditBox_Set();
	OnOperation_List_RadioButton_Set();
	OnOperation_List_State_Change(mn_type_select);

	OnOperation_List_Create_ImageList();	
	m_list_operation_file.SetImageList(&mp_operation_image_list);

	OnOperation_List_Init_List(LPCTSTR(mstr_full_path));	// 알람 결과 출력 화면에 출력할 파일 경로 초기화 작업 함수

	m_monthcal_operation.SetMaxSelCount(2);
}

void CScreen_List_Operation::OnDestroy() 
{
	mp_operation_image_list.DeleteImageList();

	/* ************************************************************************** */
	/* 생성한 폰트 정보 삭제한다                                                  */
	/* ************************************************************************** */
	delete mp_operation_list_font;
	mp_operation_list_font = NULL;
	/* ************************************************************************** */

	CFormView::OnDestroy();
}

void CScreen_List_Operation::OnOperation_List_GroupBox_Set()
{
	CSxLogFont operation_list_font(15,FW_SEMIBOLD,false,"MS Sans Serif");

//	m_group_operation_list.SetXPGroupStyle(CXPGroupBox::XPGB_WINDOW);
//	m_group_operation_list.SetBackgroundColor(RGB(201, 201, 151), RGB(255, 255, 204));
	m_group_operation_list.SetFont(operation_list_font);
	m_group_operation_list.SetCatptionTextColor(RGB(145,25,0));
	m_group_operation_list.SetFontBold(TRUE);

//	m_group_operation_type.SetXPGroupStyle(CXPGroupBox::XPGB_WINDOW);
//	m_group_operation_type.SetBackgroundColor(RGB(201, 201, 151), RGB(255, 255, 204));
	m_group_operation_type.SetFont(operation_list_font);
	m_group_operation_type.SetCatptionTextColor(RGB(145,25,0));
	m_group_operation_type.SetFontBold(TRUE);
}

void CScreen_List_Operation::OnOperation_List_Lable_Set()
{	
	m_msg_operation_file.SetFont(mp_operation_list_font);
	m_msg_operation_file.SetWindowText(_T("Select Display File"));
	m_msg_operation_file.SetCenterText();
	m_msg_operation_file.SetColor(RGB(0,0,255));
	m_msg_operation_file.SetGradientColor(RGB(255,0,0));
	m_msg_operation_file.SetTextColor(RGB(255,255,255));

	m_msg_operation_file_name.SetFont(mp_operation_list_font);
	m_msg_operation_file_name.SetWindowText(_T(""));
	m_msg_operation_file_name.SetCenterText();
	m_msg_operation_file_name.SetColor(RGB(0,0,255));
	m_msg_operation_file_name.SetGradientColor(RGB(0,0,0));
	m_msg_operation_file_name.SetTextColor(RGB(255,255,255));
}

void CScreen_List_Operation::OnOperation_List_EditBox_Set()
{	
	m_edit_operation_content.SubclassDlgItem(IDC_EDIT_OPERATION_DISPLAY, this);
	m_edit_operation_content.bkColor( RGB(50, 100, 150) );
	m_edit_operation_content.textColor( RGB(255, 255,255) );
	m_edit_operation_content.setFont(-14, FW_BOLD, DEFAULT_PITCH | FF_DONTCARE, "MS Mincho");
}

void CScreen_List_Operation::OnOperation_List_RadioButton_Set()
{
	//Set images
    m_radio_operation.SetImage(IDB_REDBUTTON, 15);
	m_radio_interface.SetImage(IDB_REDBUTTON, 15);
    m_radio_machine.SetImage(IDB_REDBUTTON, 15);
	m_radio_alarm.SetImage(IDB_REDBUTTON, 15);
	m_radio_work.SetImage(IDB_REDBUTTON, 15);
}

void CScreen_List_Operation::OnOperation_List_State_Change(int n_state)
{
	switch(n_state)
	{
	case 1 :
		m_radio_operation.Depress(true);
		m_radio_interface.Depress(false);
		m_radio_machine.Depress(false);
		m_radio_alarm.Depress(false);
		m_radio_work.Depress(false);
		break;

	case 2 :
		m_radio_operation.Depress(false);
		m_radio_interface.Depress(true);
		m_radio_machine.Depress(false);
		m_radio_alarm.Depress(false);
		m_radio_work.Depress(false);
		break;

	case 3 :
		m_radio_operation.Depress(false);
		m_radio_interface.Depress(false);
		m_radio_machine.Depress(true);
		m_radio_alarm.Depress(false);
		m_radio_work.Depress(false);
		break;

	case 4 :
		m_radio_operation.Depress(false);
		m_radio_interface.Depress(false);
		m_radio_machine.Depress(false);
		m_radio_alarm.Depress(true);
		m_radio_work.Depress(false);
		break;

	case 5 :
		m_radio_operation.Depress(false);
		m_radio_interface.Depress(false);
		m_radio_machine.Depress(false);
		m_radio_alarm.Depress(false);
		m_radio_work.Depress(true);
		break;

	default:
		break;
	}
}

void CScreen_List_Operation::OnOperation_List_Create_ImageList()
{
	BOOL bRetValue = FALSE;

	// Create image list
	bRetValue = mp_operation_image_list.Create(16, 16, ILC_COLOR32 | ILC_MASK, 5, 1);
	ASSERT(bRetValue == TRUE);
}

int CScreen_List_Operation::OnOperation_List_Init_List(LPCTSTR pszPath)
{
	int mn_chk;					// 함수 리턴 플래그 설정 변수 
    CString str_path = pszPath; // 폴더 정보 설정 

	HANDLE hFind;
    WIN32_FIND_DATA fd;
	int mn_count = 0;			// 리스트 박스 컨트롤에 추가한 파일 갯수 정보 저장 변수 

	if (str_path.Right (1) != "\\")
        str_path += "\\";

	str_path += "*.*";

	// 파일이 총 몇개가 있는지 찾아보자. 2K4/08/31/ViboX
/*	if ((hFind = ::FindFirstFile ((LPCTSTR) str_path, &fd)) != INVALID_HANDLE_VALUE) 
	{
        if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) 
		{
			mn_count++;
        }

        while (::FindNextFile (hFind, &fd)) 
		{
            mn_count++;
        }
		::FindClose(hFind);
    }
*/  
    if ((hFind = ::FindFirstFile ((LPCTSTR) str_path, &fd)) != INVALID_HANDLE_VALUE) 
	{
        if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) 
		{
			mn_chk = AddItem (mn_count, &fd);
			if (mn_chk == TRUE)  mn_count++;
        }

        while (::FindNextFile (hFind, &fd)) 
		{
            if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				mn_chk = AddItem (mn_count, &fd);
				if (mn_chk == FALSE) 
				{
					break;
				}
				else if (mn_chk == RET_ERROR)  mn_count--;
			}
            mn_count++;
        }
		::FindClose(hFind);
    }

	m_list_operation_file.SetCurSel(mn_count - 2);
	
	return mn_count;
}

void CScreen_List_Operation::OnOperation_List_Full_Path_Set(int n_selected)
{
	SYSTEMTIME  systime;
	GetLocalTime(&systime);

	switch(n_selected)
	{
	case 1 :  // SOCKET 경로 설정 
		mstr_full_path.Format("%s\\SOCKET\\%i\\%02i\\", mstr_default_path, systime.wYear, systime.wMonth);
		break;
	case 2 :  // COMM 경로 설정 
		mstr_full_path.Format("%s\\COMM\\%i\\%02i\\", mstr_default_path, systime.wYear, systime.wMonth);
		break;
	case 3 :  // PGM 경로 설정 
		mstr_full_path.Format("%s\\COMM\\%i\\%02i\\", mstr_default_path, systime.wYear, systime.wMonth);
		break;
	case 4 :  // Alarm 경로 설정 
		mstr_full_path = mstr_default_path + "\\ALARM\\daily\\";
		break;
	case 5 :  // SEQ 경로 설정 
		mstr_full_path.Format("%s\\SEQ\\%i\\%02i\\", mstr_default_path, systime.wYear, systime.wMonth);
		break;
	default :
		break;
	}
}

BOOL CScreen_List_Operation::AddItem(int n_index, WIN32_FIND_DATA *pfd)
{
	int mn_pos;

	HICON	hIcon = NULL;

	/* ************************************************************************** */
    /* 해당 파일 리스트 컨트롤에 추가할지 여부 검사한다                           */
    /* ************************************************************************** */
	if (!mstr_selected_Date.IsEmpty()) 
	{
		mstr_temp_file = (pfd->cFileName);
		mn_pos = mstr_temp_file.Find(mstr_selected_Date);  // 달력에서 설정한 날짜 정보가 파일 이름에 존재하는지 검사 
		if (mn_pos == -1)  // 설정된 날짜 정보가 파일 이름에 존재하지 않는 경우 
		{
			return RET_ERROR;
		}
	}
	/* ************************************************************************** */

	hIcon = AfxGetApp()->LoadIcon(IDI_OPEN_FOLDER);
	mp_operation_image_list.Add(hIcon);

	if (m_list_operation_file.AddString(_T(pfd->cFileName), n_index - 1) == -1)  return FALSE;

    return TRUE;
}

void CScreen_List_Operation::OnOperation_List_Selected_File_Load(CString str_folder, CString str_file)
{
	int ndx;					// 에디트 박스에 출력된 문자열 전체 길이 정보 저장 변수 
	int mn_existence;			// 파일 존재 여부 설정 변수 
	CString mstr_chk_file;		// 출력할 파일에 대한 [폴더]+[파일명] 저장 변수 
	CString mstr_description;	// 선택된 파일 내용 임시 저장 변수 
	CString mstr_temp_file;		// 현재 선택된 파일명 임시 저장 변수 
	char fileName[256];			// 출력할 파일 정보 저장 변수 
	char buffer[256001];	// 파일 내용 중 한 라인 정보 임시 저장 변수 

	m_msg_operation_file.SetWindowText(_T("Select Display File"));

	/* ************************************************************************** */
    /* 현재 선택된 파일이 이미 출력된 파일인지 검사한다                           */
	/* -> 파일 이름 출력 에디트 박스에서 읽어서 비교해야 한다                     */
    /* ************************************************************************** */
	mstr_temp_file = m_msg_operation_file_name.GetWindowText();
	mstr_temp_file.MakeUpper();
	mstr_temp_file.TrimLeft(' ');               
	mstr_temp_file.TrimRight(' ');

	if (mstr_operation_file_name == mstr_temp_file)  return ;  // 선택된 파일이 현재 출력되어 있는 파일과 동일한 경우 리턴 
	/* ************************************************************************** */

	mstr_chk_file = str_folder + str_file;  // 선택된 파일에 대한 [폴더]+[파일명] 설정 

	sprintf(fileName, "%s", mstr_chk_file);
	mn_existence = access(fileName,0) ;
	if(mn_existence != -1)  
	{
		FILE *fp = fopen ( fileName, "r" );

		if ( fp == (FILE *)NULL ) 
		{
			m_msg_operation_file.SetWindowText(_T("File Open Error~!!"));
			return ;
		}

		mstr_description.Empty();  // 파일 내용 저장 변수 초기화 

		while ( !feof(fp) )
		{
			if ( fgets ( buffer, 256000, fp ) == NULL )  break;

			if ( strlen(buffer) )   buffer[strlen(buffer)-1] = 0;
			strcat ( buffer, "\r\n" );

			mstr_description += buffer;
		}
		fclose(fp);

		((CEdit*)GetDlgItem(IDC_EDIT_OPERATION_DISPLAY))->SetWindowText(_T("")); 
		((CEdit*)GetDlgItem(IDC_EDIT_OPERATION_DISPLAY))->SetFocus();

		ndx = ((CEdit*)GetDlgItem(IDC_EDIT_OPERATION_DISPLAY))->GetWindowTextLength();

		((CEdit*)GetDlgItem(IDC_EDIT_OPERATION_DISPLAY))->SendMessage(EM_SETSEL, ndx, ndx);
//		((CEdit*)GetDlgItem(IDC_EDIT_OPERATION_DISPLAY))->SendMessage(EM_REPLACESEL, 0, (LPARAM)(LPCSTR)mstr_description);
		((CEdit*)GetDlgItem(IDC_EDIT_OPERATION_DISPLAY))->SetWindowText(_T(mstr_description));
	}
	else   
	{
		m_msg_operation_file.SetWindowText(_T("Not Exist File~!"));
		return ;
	}
}

int CScreen_List_Operation::OnOperation_List_Selected_File_Name_Check()
{
	int mn_pos;  // 위치 정보 저장 변수 
	CString mstr_chk_file;  // 선택된 파일에 대한 [폴더]+[파일명] 저장 변수 
	CString mstr_chk_ext;  // 파일 정보 중 확장자 정보만을 저장하기 위한 변수 

	m_msg_operation_file.SetWindowText(_T("Select Display File"));

	mstr_operation_file_name.MakeUpper();
	mstr_operation_file_name.TrimLeft(' ');
	mstr_operation_file_name.TrimRight(' ');

	if (mstr_operation_file_name.IsEmpty()) 
	{
		m_msg_operation_file.SetWindowText(_T("File name error~!"));
		return FALSE;
	}

	mn_pos = mstr_operation_file_name.Find(" ");		// 공백 존재 여부 검사 
	if (mn_pos != -1)
	{
		m_msg_operation_file.SetWindowText(_T("File name in space error~!"));
		return FALSE;
	}

	mstr_chk_file = mstr_full_path + mstr_operation_file_name;  // 파일에 대한 [폴더]+[파일명] 정보 설정 
	mn_pos = mstr_chk_file.Find(".");						// 확장자 위치 검사 
	if (mn_pos == -1) mstr_chk_file += ".TXT";
	else 
	{
		mstr_chk_ext = mstr_chk_file.Mid(mn_pos);	// 선택된 파일 정보 중 파일 확장자 정보만을 설정 
		if (mstr_chk_ext != ".TXT")					// .TXT 이외에는 에러로 처리
		{
			m_msg_operation_file.SetWindowText(_T("An extension input error occurrence."));
			return FALSE;
		}
	}

	return TRUE;
}

void CScreen_List_Operation::OnOperation_List_Reset_AllItem()
{	
	mstr_selected_Date.Empty();						// 선택된 날짜 정보 저장 변수 초기화 
	mstr_operation_file_name.Empty();					// 선택된 파일 이름 정보 저장 변수 초기화 

	OnOperation_List_Full_Path_Set(mn_type_select);		// 파일 정보 로딩할 전체 폴더 정보 설정 함수
	OnOperation_List_State_Change(mn_type_select);		// 파일 종류 선택 라디오 버튼 상태 변경 함수

	/* ************************************************************************** */
	/* 해당 폴더 안에 존재하는 파일 정보 새롭게 리스트 박스 컨트롤에 추가         */
	/* ************************************************************************** */
	m_list_operation_file.ResetContent();				// 리스트 박스 컨트롤에 추가된 모든 정보 삭제 
	mp_operation_image_list.DeleteImageList();			// 이미지 리스트에 설정된 모든 파일 정보 삭제 

	OnOperation_List_Create_ImageList();				// 테스트 결과 출력 화면에 사용할 이미지 생성 함수

	// Associate image list to list box
	m_list_operation_file.SetImageList(&mp_operation_image_list);

	OnOperation_List_Init_List(LPCTSTR(mstr_full_path)); // 테스트 결과 출력 화면에 출력할 파일 경로 초기화 작업 함수

	mstr_file_full_name = "";							// Save as용 Reset~ 2K4/08/16/ViboX
	m_msg_operation_file_name.SetWindowText(_T(mstr_file_full_name));

	m_msg_operation_file.SetWindowText(_T("Select Display File"));
	/* ************************************************************************** */
}

void CScreen_List_Operation::OnOperation_List_Display_Selected_Day()
{
	COleDateTime ctMinRange, ctMaxRange;
	CString mstr_date_msg, mstr_chk_file;

	if (m_monthcal_operation.GetSelRange(ctMinRange, ctMaxRange))
	{
		/* ********************************************************************** */
		/* 출력할 파일 정보 설정한다                                              */
		/* ********************************************************************** */
		switch(mn_type_select)
		{
		case 1 :  // SOCKET 출력 파일 이름 정보 설정 
			mstr_date_msg = ctMinRange.Format(_T("%Y%m%d"));
			mstr_chk_file = "LOG_SOCKET_" + mstr_date_msg;
			break;
		case 2 :  // COMM 출력 파일 이름 정보 설정 
			mstr_date_msg = ctMinRange.Format(_T("%Y%m%d"));
			mstr_chk_file = "LOG_COMM_" + mstr_date_msg;
			break;
		case 3 :  // PGM 출력 파일 이름 정보 설정 
			mstr_date_msg = ctMinRange.Format(_T("%Y%m"));
			mstr_chk_file = "LOG_PGM_" + mstr_date_msg;
			break;
		case 4 :  // Alarm 출력 파일 이름 정보 설정 
			mstr_date_msg = ctMinRange.Format(_T("%Y%m%d"));
			mstr_chk_file = "DY" + mstr_date_msg;
			break;
		default :
			break;
		}
		/* ********************************************************************** */

		if (mstr_date_msg.IsEmpty()) return;		// 선택된 날짜 정보가 공백인 경우 리턴 

		if (mstr_selected_Date != mstr_chk_file)	// 동일한 날짜에 대한 파일 출력인지 검사 
		{
			mstr_selected_Date = mstr_chk_file;		// 새로 설정된 날짜에 대한 파일 정보 설정 

			/* ****************************************************************** */
			/* 해당 폴더안에 존재하는 파일 정보 새롭게 리스트 박스에 추가         */
			/* ****************************************************************** */
			m_list_operation_file.ResetContent();		// 리스트 박스 컨트롤에 설정된 모든 정보 삭제 
			mp_operation_image_list.DeleteImageList();  // 이미지 리스트에 설정된 모든 정보 삭제 

			OnOperation_List_Create_ImageList();		// 테스트 결과 출력 화면에 사용할 이미지 생성 함수

			// Associate image list to list box
			m_list_operation_file.SetImageList(&mp_operation_image_list);

			OnOperation_List_Init_List(LPCTSTR(mstr_full_path)); // 테스트 결과 출력 화면에 출력할 파일 경로 초기화 작업 함수
			/* ****************************************************************** */
		}
	}
}

void CScreen_List_Operation::OnSelchangeListOperationFile() 
{
	int mn_index = m_list_operation_file.GetCurSel();  // 현재 선택된 리스트 컨트롤 위치 정보 설정 

	if (mn_index != LB_ERR)  // 아이템 선택에 에러 발생했는지 검사 
	{
		m_list_operation_file.GetText(mn_index, mstr_operation_file_name);				// 현재 선택된 파일 정보 얻는다 

		OnOperation_List_Selected_File_Load(mstr_full_path, mstr_operation_file_name);	// 선택된 파일 내용 출력 함수

		mstr_file_full_name = mstr_full_path + mstr_operation_file_name;				// Save as 시킬 때, 사용한다. 2K4/08/16/ViboX

		m_msg_operation_file_name.SetWindowText(_T(mstr_operation_file_name));			// 선택된 파일의 이름을 출력 시킨다.		
	}
}

void CScreen_List_Operation::OnSelectMonthcalOperation(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnOperation_List_Display_Selected_Day();			// 현재 선택된 날짜 정보 기반으로 출력할 파일 정보 설정 함수
	
	*pResult = 0;
}

void CScreen_List_Operation::OnRadioOperation() 
{
	if (mn_type_select != 1)  
	{
		mn_type_select = 1;								// Operation 폴더 설정 
		OnOperation_List_State_Change(mn_type_select);	// 파일 종류 선택 라디오 버튼 상태 변경 함수

		OnOperation_List_Reset_AllItem();				// 리스트 컨트롤에 설정된 모든 아이템 정보 해제 후 재 설정 함수
	}
}

void CScreen_List_Operation::OnRadioInterface() 
{
	if (mn_type_select != 2)  
	{
		mn_type_select = 2;								// Interface 폴더 설정 
		OnOperation_List_State_Change(mn_type_select);	// 파일 종류 선택 라디오 버튼 상태 변경 함수

		OnOperation_List_Reset_AllItem();				// 리스트 컨트롤에 설정된 모든 아이템 정보 해제 후 재 설정 함수
	}
}

void CScreen_List_Operation::OnRadioMachine() 
{
	if (mn_type_select != 3)  
	{
		mn_type_select = 3;								// Machine 폴더 설정 
		OnOperation_List_State_Change(mn_type_select);	// 파일 종류 선택 라디오 버튼 상태 변경 함수

		OnOperation_List_Reset_AllItem();				// 리스트 컨트롤에 설정된 모든 아이템 정보 해제 후 재 설정 함수
	}
}

void CScreen_List_Operation::OnBtnOperationFileDel() 
{
	char path[MAX_PATH] = {0};
	//현재의 작업경로를 얻어와 저장 한다.
	GetCurrentDirectory(MAX_PATH, path);

	if (mstr_file_full_name == "")	return;			// 원본 파일이 없을 경우에는 리턴...

	CFileDialogST	dlg(FALSE, NULL, mstr_operation_file_name, OFN_FILEMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST, _T("All files\0*.*\0"), this);
	CString			sPathName, str_temp_file, str_ext, str_chk_ext;
	int				nRetValue, n_pos;

	nRetValue = dlg.DoModal();
	
	if (nRetValue == IDOK)
	{
		sPathName = dlg.GetPathName();

		n_pos = mstr_file_full_name.Find(".");			// 원본 파일의 확장자 위치 검사

		if (n_pos == -1)								// 복사본 확장자 미존재
		{
			str_ext = ".txt";
		}
		else 
		{		
			str_ext = mstr_file_full_name.Mid(n_pos);	// 원본 파일 확장자 설정
		}

		n_pos = sPathName.Find(".");					// 복사본 확장자 위치 검사
	
		if (n_pos == -1)								// 복사본 확장자 미존재
		{
			sPathName += _T(str_ext);					// 복사본 확장자 추가
		}

		CopyFile(mstr_file_full_name, sPathName, TRUE);
		
		SetCurrentDirectory(path);						// 작업 경로를 원래 경로로 복구 한다.
	}
}

void CScreen_List_Operation::OnBtnOperationFileRefresh() 
{
	if (mn_type_select != 1)
	{
		mn_type_select = 1;								// 일별 알람 리스트 폴더 설정 
		OnOperation_List_State_Change(mn_type_select);	// 파일 종류 선택 라디오 버튼 상태 변경 함수
	}

	OnOperation_List_Reset_AllItem();					// 리스트 컨트롤에 설정된 모든 아이템 정보 해제 후 재 설정 함수
}

void CScreen_List_Operation::OnRadioAlarm() 
{
	if (mn_type_select != 4)
	{
		mn_type_select = 4;								// Barcode 폴더 설정 
		OnOperation_List_State_Change(mn_type_select);	// 파일 종류 선택 라디오 버튼 상태 변경 함수
		
		OnOperation_List_Reset_AllItem();				// 리스트 컨트롤에 설정된 모든 아이템 정보 해제 후 재 설정 함수
	}
}

void CScreen_List_Operation::OnRadioWork() 
{
	if (mn_type_select != 5)
	{
		mn_type_select = 5;								// Barcode 폴더 설정 
		OnOperation_List_State_Change(mn_type_select);	// 파일 종류 선택 라디오 버튼 상태 변경 함수
		
		OnOperation_List_Reset_AllItem();				// 리스트 컨트롤에 설정된 모든 아이템 정보 해제 후 재 설정 함수
	}
}
