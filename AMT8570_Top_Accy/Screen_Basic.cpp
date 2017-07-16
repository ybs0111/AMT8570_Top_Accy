// Screen_Basic.cpp : implementation file
//

#include "stdafx.h"
#include "handler.h"
#include "Screen_Basic.h"

#include "io.h"  // ���� ���� ���� �˻� �Լ� ȣ���ϱ� ���ؼ��� �ݵ�� INCLUDE �ʿ�

// ******************************************************************************
// ��ȭ ���� Ŭ���� �߰�                                                         
// ******************************************************************************
#include "Dialog_Select.h"
#include "Dialog_Message.h"
#include "Dialog_KeyBoard.h"

#include "Dialog_KeyPad.h"
#include "Dialog_Infor.h"
#include "Dialog_Copy.h"
// ******************************************************************************

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScreen_Basic

IMPLEMENT_DYNCREATE(CScreen_Basic, CFormView)

CScreen_Basic::CScreen_Basic()
	: CFormView(CScreen_Basic::IDD)
{
	//{{AFX_DATA_INIT(CScreen_Basic)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CScreen_Basic::~CScreen_Basic()
{
}

void CScreen_Basic::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CScreen_Basic)
	DDX_Control(pDX, IDC_CHK_SLOP_VARIABLE, m_chk_slop_variable);
	DDX_Control(pDX, IDC_GROUP_VARIABLE, m_group_slop_variable);
	DDX_Control(pDX, IDC_BTN_BASIC_APPLY, m_btn_basic_apply);
	DDX_Control(pDX, IDC_LIST_DEVICE_TYPE, m_list_device_type);
	DDX_Control(pDX, IDC_BTN_DVC_CREATE, m_btn_dvc_create);
	DDX_Control(pDX, IDC_BTN_DVC_DELETE, m_btn_dvc_delete);
	DDX_Control(pDX, IDC_GROUP_SET_DEVICE, m_group_set_device);
	DDX_Control(pDX, IDC_GROUP_BASIC, m_group_basic);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CScreen_Basic, CFormView)
	//{{AFX_MSG_MAP(CScreen_Basic)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_DVC_DELETE, OnBtnDvcDelete)
	ON_BN_CLICKED(IDC_BTN_DVC_CREATE, OnBtnDvcCreate)
	ON_LBN_SELCHANGE(IDC_LIST_DEVICE_TYPE, OnSelchangeListDeviceType)
	ON_BN_CLICKED(IDC_BTN_BASIC_APPLY, OnBtnBasicApply)
	ON_BN_CLICKED(IDC_CHK_SLOP_VARIABLE, OnChkSlopVariable)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_BASIC_APPLY, OnBasic_Info_Apply)		// BASIC DATA�� APPLY ��Ű�� ���� ����� ���� �޽��� ���� WM_BASIC_DUMP_MODE
	ON_MESSAGE(SSM_CLICK, OnCell_L_Click)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScreen_Basic diagnostics

#ifdef _DEBUG
void CScreen_Basic::AssertValid() const
{
	CFormView::AssertValid();
}

void CScreen_Basic::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CScreen_Basic message handlers

void CScreen_Basic::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	st_handler.cwnd_basic = this;  // BASIC ȭ�鿡 ���� �ڵ� ���� ����
	
	// **************************************************************************
	// ��Ʈ�ѿ� ������ ��Ʈ �����Ѵ�                                             
	// **************************************************************************
	
	mp_basic_font = NULL;
	mp_basic_font = new CFont;
	//mp_basic_font->CreateFont(15,8,0,0,700,0,0,0,0,0,0,DEFAULT_QUALITY,0,"MS Sans Serif"); //"Arial");
	mp_basic_font->CreateFont(0,0,0,0,700,0,0,0,0,0,0,NONANTIALIASED_QUALITY,0,"MS Sans Serif"); //"Arial");
	// **************************************************************************
	
	// Ŭ���� �����͸� ����ϱ� ���� [��������->Ŭ��������] �����͸� ������ �־�� ��
	OnBasic_Data_Set();
	
	mn_device_name = -1;
	str_select_device_name.Empty();
	
	OnBasic_GroupBox_Set();
	OnBasic_Button_Set();
	OnBasic_EditBox_Set();
	
	//OnBasic_Data_Set();				// ���� ������ Data�� �޾ƿ´�.
	OnBasic_Data_BackUp();
	
	OnBasic_Data_Display();

	OnBasic_Slop_Pitch_Change();
	
	// **************************************************************************
    // Į�� ����Ʈ �ڽ� ��Ʈ�� �����Ѵ�                                          
    // **************************************************************************
	OnBasic_Create_ImageList();		// �̹��� ����Ʈ ���� �Լ�
	
	// Associate image list to list box
	m_list_device_type.SetImageList(&mp_device_image_list);
	
	OnBasic_Init_List(st_path.mstr_path_dvc);  // ���� ���� �ʱ�ȭ �Լ�
	// **************************************************************************
	
	OnBasic_Device_Focus_Set();  // ���õ� ����̽��� ��Ŀ�� ���� �Լ�	
	
	// ����� �߰� [2014.08.19]
	OnBasicDataGridInit();
}

void CScreen_Basic::OnDestroy() 
{
	st_handler.cwnd_basic = NULL;  // BASIC ȭ�� �ڵ� �ʱ�ȭ
	
	int n_response;  // ��ȭ ���ڿ� ���� ���� �� ���� ����
	int Ret;
	
	CDialog_Select select_dlg;
	
	// ���α׷��� ���������鼭 Save ���� �ʰ�, Form�� Destroy�Ǵ°��̶�� Setting�� �����Ѵٴ� ���̴�.
	// �񱳵� ���� ����.
	if (st_handler.b_program_exit == false)
	{
		Ret = OnBasic_Data_Comparison();
		
		if (Ret == RET_ERROR)
		{
			st_msg.mstr_confirm_msg = _T("Changed Data! Apply?");
			
			n_response = select_dlg.DoModal();
			
			if (n_response == IDOK)
			{
				OnBasic_Data_Apply();
				
				// ���̸� �ּ� ó�� [2013.01.21]
				// ������ �����ϴ� ��ġ�� ���� �ʾ� �ּ� ó����
				// - OnBasic_Data_Apply() �Լ� �ȿ��� �����ϵ��� ������
				//james 20100610 ����
				//::PostMessage(st_handler.hWnd, WM_DATA_INIT_SAVE, 0, 0);  //�����͸� ���Ͽ� �����϶�� �޼���
			}
			else if (n_response == IDCANCEL)
			{
				OnBasic_Data_Display();
				
				OnBasic_Data_Apply();
			}	
		}
	}
	
	mp_device_image_list.DeleteImageList();
	
	// **************************************************************************
	// ������ ��Ʈ ���� �����Ѵ�                                                 
	// **************************************************************************
	delete mp_basic_font;
	mp_basic_font = NULL;
	// **************************************************************************

	CFormView::OnDestroy();
}


void CScreen_Basic::OnBtnBasicApply() 
{
	int n_response;  // ��ȭ ���� ���� �÷���
	

	// yangkyu [2015.02.28]
	// Grip, Vac ��� ��� �߰�
	if (m_nMode_VacUsing[1] == VAC_NOT_USING && m_nMode_GripUsing[1] == GRIP_NOT_USING)
	{
		CDialog_Message dlgMSG;
		
		st_msg.str_fallacy_msg = _T("You have to Using Any Of Grip and Vac.");
		dlgMSG.DoModal();
		return;
	}

	CDialog_Select  select_dlg;
	
	st_msg.mstr_confirm_msg = _T("Basic : Setting Data Apply?");
	
	n_response = select_dlg.DoModal();
	
	if (n_response == IDOK)
	{
		OnBasic_Data_Apply();			// ���̽� ȭ�� ���� ���� ���� ������ ���� �Լ�
		
		OnBasic_LogFile_Create();		// Log File ����.
		
		OnBasic_Data_BackUp();
		if (st_handler.cwnd_list != NULL)  // ����Ʈ �� ȭ�� ����
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, DEVICE_INFO);  // ����̽��� ��� ��û
	}
	else if (n_response == IDCANCEL)
	{
		
	}
}

void CScreen_Basic::OnBtnDvcDelete() 
{
	int iResponse,iExistence;				// ��ȭ ���� ���� �÷���
	CString strFile_FullName;
	CString str_file_name;
	CString str_selected_dvc;

	char chr_file_name[256];

	CDialog_Select  select_dlg;
	CDialog_Infor  info_dlg;
	CDialog_Message msg_dlg;

	if(str_select_device_name == _T(""))
	{
		//st_other.str_fallacy_msg = _T("������ ����̽��� ������ �ּ���!");
		//if(st_inspect.n_language == LANGUAGE_ENGLISH) 
		st_other.str_fallacy_msg = _T("First Select Device!!.");
		msg_dlg.DoModal();
		return ;
	}	
	else if(str_select_device_name == st_basic.sDvcName)
	{
		//st_other.str_fallacy_msg = _T("���� ������� ���縦 ����� �Ѵ�(������ ����~!!!)");
		//if(st_inspect.n_language == LANGUAGE_ENGLISH) 
		st_other.str_fallacy_msg = _T("Can't delete, Select device is Active");
		msg_dlg.DoModal();
		return ;
	}

	st_msg.mstr_confirm_msg = _T(str_select_device_name + " Device Type Delete?");
	iResponse = info_dlg.DoModal();

	if(iResponse == IDOK)
	{
		strFile_FullName = st_path.mstr_path_dvc + str_select_device_name;
		sprintf(chr_file_name, "%s", strFile_FullName);
		
		iExistence = access(chr_file_name, 0) ;
		
		if(iExistence != -1)			// ���� ����
		{
			DeleteFile(chr_file_name);  // �ش� ���� ����

			m_list_device_type.ResetContent();         // ����Ʈ �ڽ��� ��� ��� ���� //
			mp_device_image_list.DeleteImageList();  // �̹��� ����Ʈ�� ������ ���� ���� //

			OnBasic_Create_ImageList();  // �̹��� ����Ʈ ���� �Լ� //

			// Associate image list to list box
			m_list_device_type.SetImageList(&mp_device_image_list);

			OnBasic_Init_List(st_path.mstr_path_dvc);  // ���� ���� �ʱ�ȭ �Լ� //
			OnBasic_Device_Focus_Set();  // ���õ� ����̽��� ��Ŀ�� ���� �Լ� //
		}
	}
}

void CScreen_Basic::OnBtnDvcCreate() 
{
	int iResponse;  // ��ȭ ���� ���� �÷��� //
	CString str_msg;
	
	CDialog_Copy  copy_dlg;
	CDialog_Message msg_dlg;
	
	if(str_select_device_name == _T(""))
	{
		//st_other.str_fallacy_msg = _T("������ ����̽��� ������ �ּ���!");
		//if(st_inspect.n_language == LANGUAGE_ENGLISH) 
		//st_other.str_fallacy_msg = _T("First Select Device!!.");
		st_msg.str_fallacy_msg = _T("First Select Device!!.");
		msg_dlg.DoModal();
		return ;
	}
	
 	copy_dlg.mstr_selected_device_name = str_select_device_name;
 	iResponse = copy_dlg.DoModal();
	
	if(iResponse == IDOK)
	{
		m_list_device_type.ResetContent();         // ����Ʈ �ڽ��� ��� ��� ���� //
		mp_device_image_list.DeleteImageList();  // �̹��� ����Ʈ�� ������ ���� ���� //
		
		OnBasic_Create_ImageList();  // �̹��� ����Ʈ ���� �Լ� //
		
		// Associate image list to list box
		m_list_device_type.SetImageList(&mp_device_image_list);
		
		OnBasic_Init_List(st_path.mstr_path_dvc);  // ���� ���� �ʱ�ȭ �Լ� //
		
		OnBasic_Device_Focus_Set();  // ���õ� ����̽��� ��Ŀ�� ���� �Լ� //
	}
	
	str_select_device_name == _T("");
}

void CScreen_Basic::OnSelchangeListDeviceType() 
{
	CDialog_Message dlgMSG;

	int n_index = m_list_device_type.GetCurSel();  // ���õ� ������ ��ġ ���
	if (n_index != LB_ERR)
	{
		// ���õ� �������� ���ڿ� ���� ���
		m_list_device_type.GetText(n_index, str_select_device_name);
	}
	else
	{
		str_select_device_name = _T("");
	}

	if (OnBasic_Input_Device_Check(str_select_device_name))
	{
		m_sDeviceName[1] = str_select_device_name;
	}
	else
	{
		if (st_handler.mn_language == LANGUAGE_KOREA)	{
			st_other.str_fallacy_msg = _T("�����̸��� ���鹮�ڰ� �ֽ��ϴ�. ������ ������ �ּ���.");
		}
		else	{
			st_other.str_fallacy_msg = _T("file name space error. please space erase.");
		}
		dlgMSG.DoModal();
	}

	m_edit_device_type.SetWindowText(str_select_device_name);
}

void CScreen_Basic::OnBasic_Button_Set()
{
	if (st_work.nEqpStatus != dSTOP)		// Stop ���°� �ƴϸ� Apply Button�� ��Ȱ��ȭ�� �ȴ�.
	{
		m_btn_basic_apply.EnableWindow(false);
	}

	short	shBtnColor = 30;
	
	// *************************************************************
	//  CHECK BUTTON		                                        
	// *************************************************************
	m_chk_slop_variable.SetFont(mp_basic_font);

	m_chk_slop_variable.SetIcon(IDI_SOUND_ON, IDI_SOUND_OFF);
	m_chk_slop_variable.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_chk_slop_variable.SetButtonColor(1);
}

void CScreen_Basic::OnBasic_GroupBox_Set()
{
	CSxLogFont basic_font(15,FW_SEMIBOLD,false,"Arial");
	
	m_group_set_device.SetFont(basic_font);
	m_group_set_device.SetCatptionTextColor(RGB(145,25,0));
	m_group_set_device.SetFontBold(TRUE);
	
	// ����� �߰� [2014.08.19]
	m_group_basic.SetFont(basic_font);
	m_group_basic.SetCatptionTextColor(RGB(145,25,0));
	m_group_basic.SetFontBold(TRUE);

	m_group_slop_variable.SetFont(basic_font);
	m_group_slop_variable.SetCatptionTextColor(RGB(145,25,0));
	m_group_slop_variable.SetFontBold(TRUE);

}

void CScreen_Basic::OnBasic_EditBox_Set()
{
	m_edit_device_type.SubclassDlgItem(IDC_EDIT_DEVICE_TYPE, this);
	m_edit_device_type.bkColor(RGB(50, 100, 150));
	m_edit_device_type.textColor(RGB(255, 255,255));
	m_edit_device_type.setFont(-16, FW_BOLD, DEFAULT_PITCH | FF_DONTCARE, "Arial");
}

void CScreen_Basic::OnBasic_Data_Set()
{
	CString str_temp;  // ������ ���� �ӽ� ���� ���� 
	
	m_nModeDevice[1] = st_basic.n_mode_device;			// With, With Out, No Tray

	m_nModeNetwork[1] = st_basic.n_mode_Network;
	
	m_sDeviceName[1] = st_basic.sDvcName;				// ���õ� ����̽� ���� ���� ����
	
	m_sEquipNo[1] = st_basic.sEqpNo;
	m_sEquipModel[1] = st_basic.sEqpModel;
	
	// ����� �߰� [2014.08.20]
	m_nMode_SupplyMode[1] = st_basic.nMode_SupplyMode;
	m_nMode_SupplyDevice[1] = st_basic.nMode_SupplyDevice;
	m_nMode_SMEMAUsing[1] = st_basic.nMode_SMEMAUsing;

	m_nMode_GripRetryCNT[1] = st_basic.nMode_GripRetryCNT;

	// yangkyu [2015.02.28]
	// Grip, Vac ��� ��� �߰�
	m_nMode_VacUsing[1] = st_basic.nMode_VacUsing;
	m_nMode_GripUsing[1] = st_basic.nMode_GripUsing;
	m_nMode_PlaceType[1] = st_basic.nMode_PlaceType;

	m_nMode_Slop_Variable[1] = st_basic.nMode_Slop_Variable;
}

void CScreen_Basic::OnBasic_Data_Apply()
{
	st_basic.n_mode_device = m_nModeDevice[1];			// With, With Out, No Tray

	st_basic.sDvcName = m_sDeviceName[1];

	st_basic.n_mode_Network = m_nModeNetwork[1];

	st_basic.sEqpNo = m_sEquipNo[1];	
	st_basic.sEqpModel = m_sEquipModel[1];

	st_basic.nMode_SupplyMode = m_nMode_SupplyMode[1];

	st_basic.nMode_SupplyDevice = m_nMode_SupplyDevice[1];
	
	st_basic.nMode_SMEMAUsing = m_nMode_SMEMAUsing[1];

	st_basic.nMode_GripRetryCNT = m_nMode_GripRetryCNT[1];

	// yangkyu [2015.02.28]
	// Grip, Vac ��� ��� �߰�
	st_basic.nMode_VacUsing = m_nMode_VacUsing[1];
	st_basic.nMode_GripUsing = m_nMode_GripUsing[1];
	// ���̸� �߰� [2015.03.25]
	st_basic.nMode_PlaceType = m_nMode_PlaceType[1];

	st_basic.nMode_Slop_Variable = m_nMode_Slop_Variable[1];

	// ���̸� �߰� [2013.01.21]
	OnSave_TotalData();	// [����] ����̽� ���濡 ���� ���� ���� ����

	if(m_sDeviceName[0] != m_sDeviceName[1])
	{
		OnBasic_DeviceDataChange();

		OnBasic_Data_Set();
		OnBasic_Data_Display();

		OnBasicDataGridView();

		//st_handler.cwnd_title->PostMessage(WM_STATUS_CHANGE, DEVICE_NAME, 0);								// Device Name	

	}
}

void CScreen_Basic::OnBasic_Data_Display()
{
	((CEdit*)GetDlgItem(IDC_EDIT_DEVICE_TYPE))->SetWindowText(m_sDeviceName[1]);
}

void CScreen_Basic::OnBasic_Data_BackUp()
{
	m_nModeDevice[0] = m_nModeDevice[1];			// With, With Out, No Tray

	m_nModeNetwork[0] = m_nModeNetwork[1];			// Online, Offline
	
	m_sDeviceName[0] = m_sDeviceName[1];		// ���õ� ����̽� ���� ���� ����
	
	m_sEquipNo[0] = m_sEquipNo[1];
	m_sEquipModel[0] = m_sEquipModel[1];
	
	m_nMode_SupplyMode[0] = m_nMode_SupplyMode[1];

	m_nMode_SupplyDevice[0] = m_nMode_SupplyDevice[1];

	m_nMode_SMEMAUsing[0] = m_nMode_SMEMAUsing[1] ;

	m_nMode_GripRetryCNT[0] = m_nMode_GripRetryCNT[1];

	// yangkyu [2015.02.28]
	// Grip, Vac ��� ��� �߰�
	m_nMode_VacUsing[0] = m_nMode_VacUsing[1];
	m_nMode_GripUsing[0] = m_nMode_GripUsing[1];
	// ���̸� �߰� [2015.03.25]
	m_nMode_PlaceType[0] = m_nMode_PlaceType[1];
	m_nMode_Slop_Variable[0] = m_nMode_Slop_Variable[1];
}

int CScreen_Basic::OnBasic_Data_Comparison()
{
	int Ret = RET_GOOD;
	
	if (m_nModeDevice[0] != m_nModeDevice[1])
	{
		return RET_ERROR;		// With, With Out, No Tray
	}
	if (m_nModeNetwork[0] != m_nModeNetwork[1])
	{
		return RET_ERROR;		// Offline, OnLine
	}
	if(m_sDeviceName[0] != m_sDeviceName[1])
	{
		return RET_ERROR;
	}
	if (m_sEquipNo[0] != m_sEquipNo[1])
	{
		return RET_ERROR;
	}
	if (m_sEquipModel[0] != m_sEquipModel[1])
	{
		return RET_ERROR;
	}
	if (m_nMode_SupplyMode[0] != m_nMode_SupplyMode[1])
	{
		return RET_ERROR;
	}
	if (m_nMode_SupplyDevice[0] != m_nMode_SupplyDevice[1])
	{
		return RET_ERROR;
	}
	if (m_nMode_SMEMAUsing[0] != m_nMode_SMEMAUsing[1])
	{
		return RET_ERROR;
	}
	if (m_nMode_GripRetryCNT[0] != m_nMode_GripRetryCNT[1])
	{
		return RET_ERROR;
	}

	// yangkyu [2015.02.28]
	// Grip, Vac ��� ��� �߰�
	if (m_nMode_VacUsing[0] != m_nMode_VacUsing[1])
	{
		return RET_ERROR;
	}
	if (m_nMode_GripUsing[0] != m_nMode_GripUsing[1])
	{
		return RET_ERROR;
	}
	// ���̸� �߰� [2015.03.25]
	if (m_nMode_PlaceType[0] != m_nMode_PlaceType[1])
	{
		return RET_ERROR;
	}

	if( m_nMode_Slop_Variable[0] != m_nMode_Slop_Variable[1])
	{
		return RET_ERROR;
	}

	return Ret;
}

void CScreen_Basic::OnBasic_LogFile_Create()
{
	CString str_msg;
	CString str_data;
	CString str_old_data;
	
	if (m_nModeDevice[0] != m_nModeDevice[1])
	{
		switch (m_nModeDevice[1])
		{
		case 0:
			str_msg = "[Device Mode] Was changed by With Device.";
			break;
		case 1:
			str_msg = "[Device Mode] Was changed by With Out Device.";
			break;
		case 2:
			str_msg = "[Device Mode] Was changed by No Tray.";
			break;
		}
		
		cLOG.OnLogEvent(LOG_PGM_, str_msg);
	}
	
	if(m_sDeviceName[0] != m_sDeviceName[1])
	{
		str_msg = "[Basic] [Test Device Type] Was changed by" + m_sDeviceName[0] + " To " + m_sDeviceName[1];
		cLOG.OnLogEvent(LOG_PGM_, str_msg);
	}
}

int CScreen_Basic::OnBasic_Input_Device_Check(CString str_temp_device)
{
	CString str_chk_file;  // ������ [����]+[���ϸ�] ���� ����
	CString str_chk_ext;   // Ȯ���� ���� ����
	int n_pos;
	
	// **************************************************************************
    // �Էµ� ����̽��� �����Ѵ�                                                
    // **************************************************************************
	str_temp_device.MakeUpper();
	str_temp_device.TrimLeft(' ');               
	str_temp_device.TrimRight(' ');
	
	if (str_temp_device.IsEmpty())  
	{
		if (st_handler.cwnd_list != NULL)  // ����Ʈ �� ȭ�� ����
		{
			st_other.str_abnormal_msg = _T("[DEVICE] A name input error occurrence.");
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // ���� ���� ��� ��û
		}
		return FALSE;
	}
	// **************************************************************************
	
	n_pos = str_temp_device.Find(" ");  // ���� �˻� //
	if(n_pos != -1)  
	{
		return FALSE;
	}
	str_chk_file = st_path.mstr_path_dvc + str_temp_device;  // ������ [����]+[���ϸ�] ���� //
	n_pos = str_chk_file.Find(".TXT");
	if(n_pos == -1) 
	{
		str_chk_file += _T(".TXT");
	}
	else 
	{
		str_chk_ext = str_chk_file.Mid(n_pos);  // Ȯ���� ���� ���� //
		if(str_chk_ext != _T(".TXT"))  
		{
			return FALSE;
		}
	}

	CString str_temp, str_msg, str_LoadFile;       // �ӽ� ���� ����
	CString sReadFileName;
	int n_chk;
	char chr_data[20];
	
	:: GetPrivateProfileString("Supply_Type", "MasterSlave_Mode", "0", chr_data, 10, str_chk_file);
	n_chk = atoi(chr_data);
	if (n_chk < 0 || n_chk > 1)
	{
		st_basic.nMode_SupplyMode = 1;
		str_temp.Format("%d", st_basic.nMode_SupplyMode);
		:: WritePrivateProfileString("Supply_Type", "MasterSlave_Mode", LPCTSTR(str_temp), str_chk_file);
	}
	else  st_basic.nMode_SupplyMode = n_chk;
	
	:: GetPrivateProfileString("Supply_Type", "Supply_Device", "0", chr_data, 10, str_chk_file);
	n_chk = atoi(chr_data);
	if (n_chk < 0 || n_chk > 14)
	{
		st_basic.nMode_SupplyDevice = 0;
		str_temp.Format("%d", st_basic.nMode_SupplyDevice);
		:: WritePrivateProfileString("Supply_Type", "Supply_Device", LPCTSTR(str_temp), str_chk_file);
	}
	else  st_basic.nMode_SupplyDevice = n_chk;

	/*
	// yangkyu [2015.02.28]
	// Grip, Vac ��� ��� �߰�
	:: GetPrivateProfileString("Supply_Type", "VacUsingMode", "1", chr_data, 10, str_chk_file);
	n_chk = atoi(chr_data);
	if (n_chk < 0 || n_chk > 1)
	{
		st_basic.nMode_VacUsing  = VAC_NOT_USING;
		str_temp.Format("%d", st_basic.nMode_VacUsing);
		:: WritePrivateProfileString("Supply_Type", "VacUsingMode", LPCTSTR(str_temp), str_chk_file);
	}
	else  st_basic.nMode_VacUsing = n_chk;
	
	:: GetPrivateProfileString("Supply_Type", "GripUsingMode", "1", chr_data, 10, str_chk_file);
	n_chk = atoi(chr_data);
	if (n_chk < 0 || n_chk > 1)
	{
		st_basic.nMode_GripUsing = GRIP_NOT_USING;
		str_temp.Format("%d", st_basic.nMode_GripUsing);
		:: WritePrivateProfileString("Supply_Type", "GripUsingMode", LPCTSTR(str_temp), str_chk_file);
	}
	else  st_basic.nMode_GripUsing = n_chk;
	*/
	// ���̸� �ּ� ó��
	// ����̽����� �� ������ �����Ϸ��� �Ѵٸ� ������ �ε� �κк��� ���� �ʿ�
	// - �ʱ⿡�� ������ ���Ͽ��� �ε��ϱ� ������

	OnBasic_Data_Set();

	OnBasicDataGridView();

	return TRUE;
}

void CScreen_Basic::OnBasic_Device_Display(CString str_dvc)
{
	((CEdit*)GetDlgItem(IDC_EDIT_DEVICE_TYPE))->SetWindowText(_T(str_dvc));
}

BOOL CScreen_Basic::AddItem(int nIndex, WIN32_FIND_DATA *pfd)
{
	CString str_temp_dvc;
	CString str_device;
	CString str_expansion;
	int n_pos;
	
	HICON	hIcon = NULL;
	
	str_temp_dvc = (pfd->cFileName);	// ���� �̸� ����
	
	n_pos = str_temp_dvc.Find("DEFAULT.TXT");		// �⺻���� ƼĪ �����Ͷ��..
	if(n_pos != -1)
	{
		
	}
	// **************************************************************************
	// ���� �̸� �� Ȯ���� Ȯ��                                                  
	// **************************************************************************
	n_pos = str_temp_dvc.Find(".TXT");
	
	if (n_pos == -1)					// Ȯ���� ������
		str_expansion = "";
	else								// Ȯ���� ����
		str_expansion = str_temp_dvc.Mid(n_pos + 1);
	// **************************************************************************
	
	if (str_expansion != "TXT")
	{
		return READY;	
	}
	
	str_device = str_temp_dvc;			// ���ϸ� ��ü ȭ�� ���
	
	if (str_device == m_sDeviceName[1])
		mn_device_name = nIndex-1;
	// **************************************************************************
	
	hIcon = AfxGetApp()->LoadIcon(IDI_MODULE);
	mp_device_image_list.Add(hIcon);
	
    if (m_list_device_type.AddString(_T(str_device), nIndex - 1) == -1)
		return FALSE;
	
    return TRUE;
}

void CScreen_Basic::OnBasic_Device_Focus_Set()
{
	if (mn_device_name == -1)
		return;
	
	m_list_device_type.SetCurSel(mn_device_name);
}

int CScreen_Basic::OnBasic_Init_List(LPCTSTR pszPath)
{
	int n_chk;						// �Լ� ���� �÷���
    CString str_path = pszPath;		// ���� ����
	
	HANDLE hFind;
    WIN32_FIND_DATA fd;
	int n_count = 0;				// ����Ʈ �ڽ��� �߰��� ��� ���� ���� ����
    
	//2010,0610
	CString str_select_device = _T("");
	str_select_device = _T("*.TXT");
	
	if (str_path.Right (1) != "\\")
        str_path += "\\";
	
   	str_path += str_select_device; 
	n_count++;
	
    if ((hFind = ::FindFirstFile ((LPCTSTR) str_path, &fd)) != INVALID_HANDLE_VALUE) 
	{
        if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) 
		{
			n_chk = AddItem(n_count, &fd);
			if (n_chk == TRUE)
				n_count++;
        }
		
        while (::FindNextFile (hFind, &fd)) 
		{
            if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				n_chk = AddItem(n_count, &fd);
				if (n_chk == FALSE) 
					break;
			}
			if (n_chk != READY)
				n_count++;
        }
		::FindClose(hFind);
    }
	
	return n_count;
}

void CScreen_Basic::OnBasic_Create_ImageList()
{
	BOOL bRetValue = FALSE;
	
	// Create image list
	bRetValue = mp_device_image_list.Create(32, 32, ILC_COLOR32 | ILC_MASK, 5, 1);
	ASSERT(bRetValue == TRUE);
}

void CScreen_Basic::OnBasic_DeviceData_Apply()
{
	int n_check;
	CString str_msg;
	
	CDialog_Message msg_dlg;
	
	n_check = OnBasic_Input_Data_Check();  // �Է� ���� ���� ���� �˻� �Լ�
	
	if (n_check == TRUE)  
	{
		if (st_handler.cwnd_list != NULL)  // ����Ʈ �� ȭ�� ����
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, DEVICE_INFO);  // ����̽��� ��� ��û
		
		str_msg = "[Test Device] Was changed by " + m_sDeviceName[1];
		cLOG.OnLogEvent(LOG_PGM_, str_msg);
	}
	else if (n_check == FALSE)  
	{
		if (st_handler.cwnd_list != NULL)  // ����Ʈ �� ȭ�� ����
		{
			sprintf(st_msg.c_abnormal_msg, "[LOT INFORMATION] An input device setting up error.");
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // ���� ���� ��� ��û
		}
		return ;
	}
	
	st_basic.sDvcName =	m_sDeviceName[1];
	m_sDeviceName[0] =		m_sDeviceName[1];
	
	// **************************************************************************
	// ������ �����ϴ� ���� ��� ����Ʈ �ڽ��� �缳���Ѵ�                        
	// **************************************************************************
	m_list_device_type.ResetContent();			// ����Ʈ �ڽ��� ��� ��� ����
	mp_device_image_list.DeleteImageList();		// �̹��� ����Ʈ�� ������ ���� ����
	
	OnBasic_Create_ImageList();					// �̹��� ����Ʈ ���� �Լ�
	
	// Associate image list to list box
	m_list_device_type.SetImageList(&mp_device_image_list);
	
	OnBasic_Init_List(st_path.mstr_path_dvc);	// ���� ���� �ʱ�ȭ �Լ�
	
	OnBasic_Device_Focus_Set();					// ���õ� ����̽��� ��Ŀ�� ���� �Լ�
	// **************************************************************************
	
	if (st_handler.cwnd_list != NULL)			// ����Ʈ �� ȭ�� ����
		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, DEVICE_INFO);  // ����̽� ���� ��� ��û
}

int CScreen_Basic::OnBasic_New_Device_Check()
{
	CString str_chk_file;  // ������ [����]+[���ϸ�] ���� ����
	CString str_chk_ext;   // Ȯ���� ���� ����
	int n_pos;
	
	// **************************************************************************
    // �Էµ� ����̽��� �����Ѵ�                                                
    // **************************************************************************
	mstr_new_device.MakeUpper();
	mstr_new_device.TrimLeft(' ');               
	mstr_new_device.TrimRight(' ');
	
	if (mstr_new_device.IsEmpty())  
	{
		if (st_handler.cwnd_list != NULL)  // ����Ʈ �� ȭ�� ����
		{
			sprintf(st_msg.c_abnormal_msg, "[DEVICE] A name input error occurrence.");
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // ���� ���� ��� ��û
		}
		return FALSE;
	}
	// **************************************************************************
	
	str_chk_file = st_path.mstr_path_dvc + mstr_new_device;  // ������ [����]+[���ϸ�] ����
	n_pos = str_chk_file.Find(".");  // Ȯ���� �˻�
	if (n_pos == -1) 
		str_chk_file += _T(".TXT");
	else 
	{
		str_chk_ext = str_chk_file.Mid(n_pos);  // Ȯ���� ���� ����
		if (str_chk_ext != _T(".TXT"))  
		{
			if (st_handler.cwnd_list != NULL)  // ����Ʈ �� ȭ�� ����
			{
				sprintf(st_msg.c_abnormal_msg, "[DEVICE] A name extension input error occurrence.");
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // ���� ���� ��� ��û
			}
			return FALSE;
		}
	}
	
	mstr_load_file = str_chk_file;  // ������ [����]+[���ϸ�]+[Ȯ����] ����
	
	return TRUE;
}

int CScreen_Basic::OnBasic_Input_Data_Check()
{
	CString str_chk_ext;   // Ȯ���� ���� ����
	int n_pos;
	
	// **************************************************************************
    // �Էµ� ����̽��� �����Ѵ�                                                
    // **************************************************************************
	((CEdit*)GetDlgItem(IDC_EDIT_DEVICE_TYPE))->GetWindowText(mstr_temp_device) ;
	mstr_temp_device.MakeUpper();
	mstr_temp_device.TrimLeft(' ');               
	mstr_temp_device.TrimRight(' ');
	
	if (mstr_temp_device.IsEmpty())  
	{
		if (st_handler.cwnd_list != NULL)			// ����Ʈ �� ȭ�� ����
		{
			sprintf(st_msg.c_abnormal_msg, "[DEVICE] A name input error occurrence.");
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // ���� ���� ��� ��û
		}
		return FALSE;
	}
	// **************************************************************************
	
	n_pos = mstr_temp_device.Find(".");				// Ȯ���� �˻�
	if (n_pos == -1) 
		mstr_temp_device += _T(".TXT");
	else 
	{
		str_chk_ext = mstr_temp_device.Mid(n_pos);  // Ȯ���� ���� ����
		if (str_chk_ext != _T(".TXT"))  
		{
			if (st_handler.cwnd_list != NULL)		// ����Ʈ �� ȭ�� ����
			{
				sprintf(st_msg.c_abnormal_msg, "[DEVICE] A name extension input error occurrence.");
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // ���� ���� ��� ��û
			}
			return FALSE;
		}
	}
	
	m_sDeviceName[1] = mstr_temp_device;			// ����̽� ���ϸ� ����
	// **************************************************************************
	
	return TRUE;
}

//==================================================================//
// [����] ����̽� ���濡 ���� ���� ���� ����
//==================================================================//
void CScreen_Basic::OnSave_TotalData() 
{
	mcls_basic.OnBasic_Data_Save();
	
	mcls_basic.On_Teach_Data_Save();

	CString mstr_temp;  // ������ ���� �ӽ� ���� ����
	char chr_buf[20];

	CString str_temp, str_SaveFile;			// �ӽ� ���� ����
	CString sSaveFileName;
	
	sSaveFileName	= st_basic.sDvcName;
	str_SaveFile	= st_path.mstr_path_dvc + sSaveFileName;

	mstr_temp = LPCTSTR(_itoa(st_basic.nMode_SupplyMode, chr_buf, 10));
	:: WritePrivateProfileString("Supply_Type", "MasterSlave_Mode", LPCTSTR(mstr_temp), str_SaveFile);
	
	mstr_temp = LPCTSTR(_itoa(st_basic.nMode_SupplyDevice, chr_buf, 10));
	:: WritePrivateProfileString("Supply_Type", "Supply_Device", LPCTSTR(mstr_temp), str_SaveFile);

	// yangkyu [2015.02.28]
	// Grip, Vac ��� ��� �߰�
	/*
	mstr_temp = LPCTSTR(_itoa(st_basic.nMode_VacUsing, chr_buf, 10));
	:: WritePrivateProfileString("Supply_Type", "VacUsingMode", LPCTSTR(mstr_temp), str_SaveFile);
	
	mstr_temp = LPCTSTR(_itoa(st_basic.nMode_GripUsing, chr_buf, 10));
	:: WritePrivateProfileString("Supply_Type", "GripUsingMode", LPCTSTR(mstr_temp), str_SaveFile);
	*/
	// ���̸� �ּ� ó��
	// ����̽����� �� ������ �����Ϸ��� �Ѵٸ� ������ ���� �κк��� ���� �ʿ�
	// - �ʱ⿡�� ������ ���Ͽ� �����ϱ� ������
 }

void CScreen_Basic::OnBasic_Create_Device()
{
	int n_existence;								// ���� ���� ���� �÷���
	char chr_file_name[256];
	CString str_device_temp;
	CString str_original_path;
	CString str_original_file;
	CString str_copy_file;
	CString str_chk_ext;
	CString str_copy_info;
	
	sprintf(chr_file_name, "%s", mstr_load_file);	// [����]+[���ϸ�] ����
	
	n_existence = access(chr_file_name, 0) ;
	
	if (n_existence != -1)							// ���� ����
		return ;
	else  
	{
		str_device_temp = st_basic.sDvcName;
		st_basic.sDvcName = mstr_new_device;
		
		:: WritePrivateProfileString("Device", "DeviceFile", LPCTSTR(mstr_new_device), mstr_load_file); 
		
		// 		mcls_basic.OnRobot_Teach_Data_Save();
		// 		mcls_basic.OnUnloaderSite_Teach_Data_Save();
		// 		mcls_basic.OnLoaderSite_Teach_Data_Save();
		
		st_basic.sDvcName = str_device_temp;
	}
}

void CScreen_Basic::OnBasic_DeviceDataChange()//james 20100610 ���� //�߰� 
{	
	mcls_basic.On_Teach_Data_Load();

	CString str_temp, str_msg, str_LoadFile;       // �ӽ� ���� ����
	CString sReadFileName;
	int n_chk;
	char chr_data[20];
	
	sReadFileName	= st_basic.sDvcName;
	str_LoadFile	= st_path.mstr_path_dvc + sReadFileName;

	:: GetPrivateProfileString("Supply_Type", "MasterSlave_Mode", "0", chr_data, 10, str_LoadFile);
	n_chk = atoi(chr_data);
	if (n_chk < 0 || n_chk > 1)
	{
		st_basic.nMode_SupplyMode = 1;
		str_temp.Format("%d", st_basic.nMode_SupplyMode);
		:: WritePrivateProfileString("Supply_Type", "MasterSlave_Mode", LPCTSTR(str_temp), str_LoadFile);
	}
	else  st_basic.nMode_SupplyMode = n_chk;
	
	:: GetPrivateProfileString("Supply_Type", "Supply_Device", "0", chr_data, 10, str_LoadFile);
	n_chk = atoi(chr_data);
	if (n_chk < 0 || n_chk > 14)
	{
		st_basic.nMode_SupplyDevice = 0;
		str_temp.Format("%d", st_basic.nMode_SupplyDevice);
		:: WritePrivateProfileString("Supply_Type", "Supply_Device", LPCTSTR(str_temp), str_LoadFile);
	}
	else  st_basic.nMode_SupplyDevice = n_chk;

	// yangkyu [2015.02.28]
	// Grip, Vac ��� ��� �߰�
	/*
	:: GetPrivateProfileString("Supply_Type", "VacUsingMode", "1", chr_data, 10, str_LoadFile);
	n_chk = atoi(chr_data);
	if (n_chk < 0 || n_chk > 1)	
	{
		st_basic.nMode_VacUsing  = VAC_NOT_USING;
		str_temp.Format("%d", st_basic.nMode_VacUsing);
		:: WritePrivateProfileString("Supply_Type", "VacUsingMode", LPCTSTR(str_temp), str_LoadFile);
	}
	else  st_basic.nMode_VacUsing = n_chk;
	
	:: GetPrivateProfileString("Supply_Type", "GripUsingMode", "1", chr_data, 10, str_LoadFile);
	n_chk = atoi(chr_data);
	if (n_chk < 0 || n_chk > 1)
	{
		st_basic.nMode_GripUsing = GRIP_NOT_USING;
		str_temp.Format("%d", st_basic.nMode_GripUsing);
		:: WritePrivateProfileString("Supply_Type", "GripUsingMode", LPCTSTR(str_temp), str_LoadFile);
	}
	else  st_basic.nMode_GripUsing = n_chk;
	*/
	// ���̸� �ּ� ó��
	// ����̽����� �� ������ �����Ϸ��� �Ѵٸ� ������ �ε� �κк��� ���� �ʿ�
	// - �ʱ⿡�� ������ ���Ͽ��� �ε��ϱ� ������
}

// ******************************************************************************
//  BASIC ȭ�� ���� ���� ��û �޽���                                             
// ******************************************************************************
LRESULT CScreen_Basic::OnBasic_Info_Apply(WPARAM wParam, LPARAM lParam)  
{
	OnBasic_Data_Apply();			// ���̽� ȭ�� ���� ���� ���� ������ ���� �Լ�
	
	OnBasic_LogFile_Create();		// Log File ����.
	
	OnBasic_Data_BackUp();
	
	return 0;
}

void CScreen_Basic::OnCell_L_Click( WPARAM wParam, LPARAM lParam )
{
	LPSS_CELLCOORD lpcc = (LPSS_CELLCOORD)lParam;
	
	CString sTmp, sText, sMSG;
	int nResponse, i=0;
	int nRowPos, nColPos;
	
	CString strText;
	
	CString str_tmp, str_data;

	CDialog_KeyPad	pad_dlg;

	CDialog_Keyboard  keyboard_dlg;
	
	if ((int)wParam == IDC_CUSTOM_BASIC)
	{
		// Ÿ��Ʋ ���� Ŭ���� ���� ����
		if (lpcc->Col == 1)
		{
			return;
		}
		m_grid_basic_setting = (TSpread*)GetDlgItem(IDC_CUSTOM_BASIC);
		nRowPos = lpcc->Row;  // Ŭ���� ROW ��ġ ����
		nColPos = lpcc->Col;  // Ŭ���� COL ��ġ ����
		
		switch(lpcc->Row)
		{
		case 1:
			st_msg.mstr_typing_msg = m_sEquipModel[1];
			
			nResponse = keyboard_dlg.DoModal();
			if (nResponse == IDOK)
			{
				m_sEquipModel[1] = st_msg.mstr_typing_msg;  // Ű���� ���� �Է� ���� Ŭ���� ������ ����
				
				m_sEquipModel[1].MakeUpper();
				m_sEquipModel[1].TrimLeft(' ');               
				m_sEquipModel[1].TrimRight(' ');
				
				sTmp.Format("%s", m_sEquipModel[1]);
				m_p_grid.GridCellText(m_grid_basic_setting, 1, 2, sTmp);
			}
			break;
			
		case 2:
			st_msg.mstr_typing_msg = m_sEquipNo[1];
			
			nResponse = keyboard_dlg.DoModal();
			if (nResponse == IDOK)
			{
				m_sEquipNo[1] = st_msg.mstr_typing_msg;  // Ű���� ���� �Է� ���� Ŭ���� ������ ����
				
				m_sEquipNo[1].MakeUpper();
				m_sEquipNo[1].TrimLeft(' ');               
				m_sEquipNo[1].TrimRight(' ');
				
				sTmp.Format("%s", m_sEquipNo[1]);
				m_p_grid.GridCellText(m_grid_basic_setting, 2, 2, sTmp);
			}
			break;
			
			// With
		case 3:
			m_nModeDevice[1] = WITH_DVC_;			
			OnWithModeGridView();			
			break;
			
			// With Out
		case 4:
			m_nModeDevice[1] = WITHOUT_DVC_;			
			OnWithModeGridView();		
			break;
			
			// MASTER_MODE
		case 5:
			m_nMode_SupplyMode[1] = MASTER_MODE;
			OnSupplyModeModeGridView();

			m_nMode_SupplyDevice[1] = DEVICE_2_5INCH_BTM;
			OnSupplyDeviceModeGridView();
			break;

			// SLAVE_MODE
		case 6:
			m_nMode_SupplyMode[1] = SLAVE_MODE;
			OnSupplyModeModeGridView();

			m_nMode_SupplyDevice[1] = DEVICE_DESKTOP_TOP;
			OnSupplyDeviceModeGridView();
			break;

			// 2_5INCH
		case 7:
			if(m_nMode_SupplyMode[1] == MASTER_MODE)
			{
				m_nMode_SupplyDevice[1] = DEVICE_2_5INCH_BTM;
			}
			OnSupplyDeviceModeGridView();
			break;

			// DESKTOP
		case 8:
			m_nMode_SupplyDevice[1] = DEVICE_DESKTOP_BTM + ( m_nMode_SupplyMode[1] * 10 );
			OnSupplyDeviceModeGridView();
			break;

			// MSATA
		case 9:
			m_nMode_SupplyDevice[1] = DEVICE_MSATA_BTM + ( m_nMode_SupplyMode[1] * 10 );
			OnSupplyDeviceModeGridView();
			break;

			// SMEMA Using
		case 10:
			m_nMode_SMEMAUsing[1] = SMEMA_USING;
			OnSMEMAModeGridView();
			break;

			// SMEMA Not Using
		case 11:
			m_nMode_SMEMAUsing[1] = SMEMA_NOT_USING;
			OnSMEMAModeGridView();
			break;

		case 12:
			m_nModeNetwork[1] = ON_LINE;
			OnNetworkModeGridView();
			break;

		case 13:
			m_nModeNetwork[1] = OFF_LINE;
			OnNetworkModeGridView();
			break;

		case 14:
			strText = m_p_grid.GridCellText(m_grid_basic_setting, lpcc->Row, lpcc->Col - 1);
			st_msg.mstr_keypad_msg.Format("%s set", strText);
			
			str_tmp = m_p_grid.GridCellText(m_grid_basic_setting, lpcc->Row, lpcc->Col);
			st_msg.mstr_keypad_val = str_tmp;
			
			str_tmp = KeyPad.GetNumEditString_D(0, 3, atof(st_msg.mstr_keypad_val), st_msg.mstr_keypad_msg);
			
			m_nMode_GripRetryCNT[1] = atof(str_tmp);
			m_p_grid.GridCellText(m_grid_basic_setting, lpcc->Row, lpcc->Col, str_tmp);
			break;

		// yangkyu [2015.02.28]
		// Grip, Vac ��� ��� �߰�
		case 15:
			m_nMode_GripUsing[1] = GRIP_USING;
			OnGripUsingModeGridView();
			break;
			
		case 16:
			m_nMode_GripUsing[1] = GRIP_NOT_USING;
			OnGripUsingModeGridView();
			break;

		case 17:
			m_nMode_VacUsing[1] = VAC_USING;
			OnVacUsingModeGridView();
			break;
			
		case 18:
			m_nMode_VacUsing[1] = VAC_NOT_USING;
			OnVacUsingModeGridView();
			break;

			// ���̸� �߰� [2015.03.24]
		case 19:
			m_nMode_PlaceType[1] = 0;
			OnGridView_PlaceType();	// ���� �������� ��� [1:4����, 0:2����]
			break;

		case 20:
			m_nMode_PlaceType[1] = 1;
			OnGridView_PlaceType();	// ���� �������� ��� [1:4����, 0:2����]
			break;
		}
	}
}

void CScreen_Basic::OnBasicDataGridInit()
{
	/*
	CString sTitle[GRID_ITEM_CNT_] = { "EQP Model", "EQP No", "With Mode", "", "Supply Mode", "", 
		"Supply Device", "", "", "SMEMA Mode", "", "Network Mode", "", "Grip Retry Count", "Grip Using Mode", "", "Vac Using Mode", ""};
	*/
	// ���̸� ���� [2015.03.25]
	CString sTitle[GRID_ITEM_CNT_] = { "EQP Model", "EQP No", "With Mode", "", "Supply Mode", "", "Supply Device", "", "", "SMEMA Mode", "",
		"Network Mode", "", "Grip Retry Count", "Grip Using Mode", "", "Vac Using Mode", "", "Place Type", "" };

	int nMaxROW = GRID_ITEM_CNT_;
	int nMaxCOL = 2;

	m_grid_basic_setting = (TSpread*)GetDlgItem(IDC_CUSTOM_BASIC);
	m_p_grid.GridReset(m_grid_basic_setting);
	
	m_p_grid.GridCellSelectDisplay(m_grid_basic_setting, TRUE);
	// �׸��� ��� �̻��
	m_p_grid.GridRowHeader(m_grid_basic_setting, FALSE);
	m_p_grid.GridColHeader(m_grid_basic_setting, FALSE);
	// ��ũ�� �̻���
	m_p_grid.GridHorizontal(m_grid_basic_setting, FALSE);
	m_p_grid.GridVertical(  m_grid_basic_setting, FALSE);
	// �Է� ���ڼ��� �°� �ڵ� ���� �׸��� ũ�� ���� ��� �̻��
	m_p_grid.GridAutoSize(m_grid_basic_setting, FALSE);
	// �׸��� ���� ����
	m_p_grid.GridCellRows(m_grid_basic_setting, nMaxROW);
	m_p_grid.GridCellCols(m_grid_basic_setting, nMaxCOL);

	int i, j;
	for(i=0; i<nMaxROW; i++)
	{
		//m_p_grid.GridCellHeight_L(m_grid_basic_setting, i+1, 28);
		// ���̸� ���� [2015.03.25]
		m_p_grid.GridCellHeight_L(m_grid_basic_setting, i+1, 24);
		
		for(j=0; j<nMaxCOL; j++)
		{
			switch(j)
			{
			case 0:
				m_p_grid.GridCellWidth_L(m_grid_basic_setting, j+1, 18);
				m_p_grid.GridCellColor(  m_grid_basic_setting, i+1, j+1, BLUE_D, WHITE_C);
				break;
			case 1:
				m_p_grid.GridCellWidth_L(m_grid_basic_setting, j+1, 20);
				m_p_grid.GridCellColor(  m_grid_basic_setting, i+1, j+1, RGB(50, 100, 150), WHITE_C);
				break;
			}
			m_p_grid.GridCellControlStatic(m_grid_basic_setting, i+1, j+1);
		}
		
		m_p_grid.GridCellFont(m_grid_basic_setting, i+1, 1, "Tahoma", 14);
		m_p_grid.GridCellText(m_grid_basic_setting, i+1, 1, sTitle[i]);
		m_p_grid.GridCellFont(m_grid_basic_setting, i+1, 2, "Tahoma", 20);

		// cell Merge
		m_p_grid.GridCellMerge(m_grid_basic_setting, 3, 1, 2, 1);	// With Mode
		m_p_grid.GridCellMerge(m_grid_basic_setting, 5, 1, 2, 1);	// Mode Supplu Device
		m_p_grid.GridCellMerge(m_grid_basic_setting, 7, 1, 3, 1);	// Supply Device
		m_p_grid.GridCellMerge(m_grid_basic_setting, 10, 1, 2, 1);	// SMEMA Using
		m_p_grid.GridCellMerge(m_grid_basic_setting, 12, 1, 2, 1);	// Network Mode
		// yangkyu [2015.02.28]
		// Grip, Vac ��� ��� �߰�
		m_p_grid.GridCellMerge(m_grid_basic_setting, 15, 1, 2, 1);	// Grip Using Mode
		m_p_grid.GridCellMerge(m_grid_basic_setting, 17, 1, 2, 1);	// Vac Using Mode
		// ���̸� �߰� [2015.03.25]
		m_p_grid.GridCellMerge(m_grid_basic_setting, 19, 1, 2, 1);
	}
	OnBasicDataGridView();
}

void CScreen_Basic::OnBasicDataGridView()
{
	m_grid_basic_setting = (TSpread*)GetDlgItem(IDC_CUSTOM_BASIC);
	
	CString sTmp;
	sTmp.Format("%s", m_sEquipModel[1]);
	m_p_grid.GridCellText(m_grid_basic_setting, 1, 2, sTmp);

	sTmp.Format("%s", m_sEquipNo[1]);
	m_p_grid.GridCellText(m_grid_basic_setting, 2, 2, sTmp);

	m_p_grid.GridCellText(m_grid_basic_setting, 3, 2, "With");
	m_p_grid.GridCellText(m_grid_basic_setting, 4, 2, "With Out");
	OnWithModeGridView();

	m_p_grid.GridCellText(m_grid_basic_setting, 5, 2, "Master");
	m_p_grid.GridCellText(m_grid_basic_setting, 6, 2, "Slave");
	OnSupplyModeModeGridView();

	OnSupplyDeviceModeGridView();

	m_p_grid.GridCellText(m_grid_basic_setting, 10, 2, "Using");
	m_p_grid.GridCellText(m_grid_basic_setting, 11, 2, "Not Using");
	OnSMEMAModeGridView();

	m_p_grid.GridCellText(m_grid_basic_setting, 12, 2, "Online");
	m_p_grid.GridCellText(m_grid_basic_setting, 13, 2, "Offline");
	OnNetworkModeGridView();

	sTmp.Format("%d", m_nMode_GripRetryCNT[1]);
	m_p_grid.GridCellText(m_grid_basic_setting, 14, 2, sTmp);

	// yangkyu [2015.02.28]
	// Grip, Vac ��� ��� �߰�
	m_p_grid.GridCellText(m_grid_basic_setting, 15, 2, "Using");
	m_p_grid.GridCellText(m_grid_basic_setting, 16, 2, "Not Using");
	OnGripUsingModeGridView();

	m_p_grid.GridCellText(m_grid_basic_setting, 17, 2, "Using");
	m_p_grid.GridCellText(m_grid_basic_setting, 18, 2, "Not Using");
	OnVacUsingModeGridView();
	
	// ���̸� �߰� [2015.03.25]
	m_p_grid.GridCellText(m_grid_basic_setting, 19, 2, "2 Picker");
	m_p_grid.GridCellText(m_grid_basic_setting, 20, 2, "4 Picker");
	OnGridView_PlaceType();	// ���� �������� ��� [1:4����, 0:2����]
}

void CScreen_Basic::OnWithModeGridView()
{
	// With
	if (m_nModeDevice[1] == WITH_DVC_)
	{
		m_p_grid.GridCellColor(m_grid_basic_setting, 3, 2, GREEN_C, BLACK_C);
		m_p_grid.GridCellColor(m_grid_basic_setting, 4, 2, BLACK_L, WHITE_C);
	}
	// With Out
	else
	{
		m_p_grid.GridCellColor(m_grid_basic_setting, 3, 2, BLACK_L, WHITE_C);
		m_p_grid.GridCellColor(m_grid_basic_setting, 4, 2, GREEN_C, BLACK_C);		
	}
}

void CScreen_Basic::OnNetworkModeGridView()
{
	// Online
	if (m_nModeNetwork[1] == ON_LINE)
	{
		m_p_grid.GridCellColor(m_grid_basic_setting, 12, 2, GREEN_C, BLACK_C);
		m_p_grid.GridCellColor(m_grid_basic_setting, 13, 2, BLACK_L, WHITE_C);
	}
	// Offline
	else
	{
		m_p_grid.GridCellColor(m_grid_basic_setting, 12, 2, BLACK_L, WHITE_C);
		m_p_grid.GridCellColor(m_grid_basic_setting, 13, 2, GREEN_C, BLACK_C);		
	}
}

void CScreen_Basic::OnSupplyModeModeGridView()
{
	// MASTER_MODE
	if (m_nMode_SupplyMode[1] == MASTER_MODE)
	{
		m_p_grid.GridCellColor(m_grid_basic_setting, 5, 2, GREEN_C, BLACK_C);
		m_p_grid.GridCellColor(m_grid_basic_setting, 6, 2, BLACK_L, WHITE_C);

		m_p_grid.GridCellText(m_grid_basic_setting, 7, 2, "2.5 Inch Bottom");
		m_p_grid.GridCellText(m_grid_basic_setting, 8, 2, "Desktop Bottom");
		m_p_grid.GridCellText(m_grid_basic_setting, 9, 2, "M-SATA Bottom");
	}
	// SLAVE_MODE
	else
	{
		m_p_grid.GridCellColor(m_grid_basic_setting, 5, 2, BLACK_L, WHITE_C);
		m_p_grid.GridCellColor(m_grid_basic_setting, 6, 2, GREEN_C, BLACK_C);	
		
		m_p_grid.GridCellText(m_grid_basic_setting, 7, 2, "");
		m_p_grid.GridCellText(m_grid_basic_setting, 8, 2, "Desktop Top");
		m_p_grid.GridCellText(m_grid_basic_setting, 9, 2, "M-SATA Top");
	}
}

void CScreen_Basic::OnSupplyDeviceModeGridView()
{
	// DEVICE_2_5INCH
	if (m_nMode_SupplyDevice[1] == DEVICE_2_5INCH_BTM)
	{
		m_p_grid.GridCellColor(m_grid_basic_setting, 7, 2, GREEN_C, BLACK_C);
		m_p_grid.GridCellColor(m_grid_basic_setting, 8, 2, BLACK_L, WHITE_C);
		m_p_grid.GridCellColor(m_grid_basic_setting, 9, 2, BLACK_L, WHITE_C);
	}
	// DEVICE_DESKTOP
	else if(m_nMode_SupplyDevice[1] == DEVICE_DESKTOP_BTM || m_nMode_SupplyDevice[1] == DEVICE_DESKTOP_TOP)
	{
		m_p_grid.GridCellColor(m_grid_basic_setting, 7, 2, BLACK_L, WHITE_C);
		m_p_grid.GridCellColor(m_grid_basic_setting, 8, 2, GREEN_C, BLACK_C);
		m_p_grid.GridCellColor(m_grid_basic_setting, 9, 2, BLACK_L, WHITE_C);
	}
	// DEVICE_MSATA
	else if (m_nMode_SupplyDevice[1] == DEVICE_MSATA_BTM || m_nMode_SupplyDevice[1] == DEVICE_MSATA_TOP)
	{
		m_p_grid.GridCellColor(m_grid_basic_setting, 7, 2, BLACK_L, WHITE_C);
		m_p_grid.GridCellColor(m_grid_basic_setting, 8, 2, BLACK_L, WHITE_C);
		m_p_grid.GridCellColor(m_grid_basic_setting, 9, 2, GREEN_C, BLACK_C);
	}
}

void CScreen_Basic::OnSMEMAModeGridView()
{
	if(m_nMode_SMEMAUsing[1] == SMEMA_USING)
	{
		m_p_grid.GridCellColor(m_grid_basic_setting, 10, 2, GREEN_C, BLACK_C);
		m_p_grid.GridCellColor(m_grid_basic_setting, 11, 2, BLACK_L, WHITE_C);
	}
	else if(m_nMode_SMEMAUsing[1] == SMEMA_NOT_USING)
	{
		m_p_grid.GridCellColor(m_grid_basic_setting, 10, 2, BLACK_L, WHITE_C);
		m_p_grid.GridCellColor(m_grid_basic_setting, 11, 2, GREEN_C, BLACK_C);
	}
}

// yangkyu [2015.02.28]
// Grip, Vac ��� ��� �߰�
void CScreen_Basic::OnGripUsingModeGridView()
{
	if(m_nMode_GripUsing[1] == GRIP_USING)
	{
		m_p_grid.GridCellColor(m_grid_basic_setting, 15, 2, GREEN_C, BLACK_C);
		m_p_grid.GridCellColor(m_grid_basic_setting, 16, 2, BLACK_L, WHITE_C);
	}
	else if(m_nMode_GripUsing[1] == GRIP_NOT_USING)
	{
		m_p_grid.GridCellColor(m_grid_basic_setting, 15, 2, BLACK_L, WHITE_C);
		m_p_grid.GridCellColor(m_grid_basic_setting, 16, 2, GREEN_C, BLACK_C);
	}
}

// yangkyu [2015.02.28]
// Grip, Vac ��� ��� �߰�
void CScreen_Basic::OnVacUsingModeGridView()
{
	if(m_nMode_VacUsing[1] == VAC_USING)
	{
		m_p_grid.GridCellColor(m_grid_basic_setting, 17, 2, GREEN_C, BLACK_C);
		m_p_grid.GridCellColor(m_grid_basic_setting, 18, 2, BLACK_L, WHITE_C);
	}
	else if(m_nMode_VacUsing[1] == VAC_NOT_USING)
	{
		m_p_grid.GridCellColor(m_grid_basic_setting, 17, 2, BLACK_L, WHITE_C);
		m_p_grid.GridCellColor(m_grid_basic_setting, 18, 2, GREEN_C, BLACK_C);
	}
}

//==================================================================//
// ���� �������� ��� [1:4����, 0:2����]
// ���̸� �߰� [2015.03.25]
//==================================================================//
void CScreen_Basic::OnGridView_PlaceType() 
{
	if (m_nMode_PlaceType[1] == 0)
	{
		m_p_grid.GridCellColor(m_grid_basic_setting, 19, 2, GREEN_C, BLACK_C);
		m_p_grid.GridCellColor(m_grid_basic_setting, 20, 2, BLACK_L, WHITE_C);
	}
	else
	{
		m_p_grid.GridCellColor(m_grid_basic_setting, 19, 2, BLACK_L, WHITE_C);
		m_p_grid.GridCellColor(m_grid_basic_setting, 20, 2, GREEN_C, BLACK_C);
	}
}

void CScreen_Basic::OnChkSlopVariable() 
{
	if (m_chk_slop_variable.GetCheck() == TRUE)	m_nMode_Slop_Variable[1] = TRUE;
	else										m_nMode_Slop_Variable[1] = FALSE;
		
	OnBasic_Slop_Pitch_Change();	
}

void CScreen_Basic::OnBasic_Slop_Pitch_Change()
{
	if (m_nMode_Slop_Variable[1] < 0 || m_nMode_Slop_Variable[1] > 1)	m_nMode_Slop_Variable[1] = TRUE;
	
	m_chk_slop_variable.SetCheck(m_nMode_Slop_Variable[1]);
	
	if (m_nMode_Slop_Variable[1] == TRUE)	SetDlgItemText(IDC_CHK_SLOP_VARIABLE, "Slop Pitch Use");
	else								SetDlgItemText(IDC_CHK_SLOP_VARIABLE, "Slop Pitch Not Use");
}

