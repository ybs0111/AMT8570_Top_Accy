// Dialog_Alarm.cpp : implementation file
//

#include "stdafx.h"
#include "Handler.h"
#include "Dialog_Alarm.h"

// MainFrm�� ������ �˶� Ŭ���� ������ ���� ��� ���Ͽ� �߰�
#include "MainFrm.h"
#include "io.h" 

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialog_Alarm dialog


CDialog_Alarm::CDialog_Alarm(CWnd* pParent /*=NULL*/)
	: CDialog(CDialog_Alarm::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialog_Alarm)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDialog_Alarm::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialog_Alarm)
	DDX_Control(pDX, IDC_MSG_ALARM_TIME, m_msg_alarm_time);
	DDX_Control(pDX, IDC_DATA_ALARM_TIME, m_data_alarm_time);
	DDX_Control(pDX, IDC_MSG_LOT_END, m_msg_lot_end);
	DDX_Control(pDX, IDC_MSG_ALARM_TYPE, m_msg_alarm_type);
	DDX_Control(pDX, IDC_MSG_ALARM_TITLE, m_msg_alarm_title);
	DDX_Control(pDX, IDC_MSG_ALARM_REPARE, m_msg_alarm_repare);
	DDX_Control(pDX, IDC_MSG_ALARM_CODE, m_msg_alarm_code);
	DDX_Control(pDX, IDC_IMAGE_ALARM, m_image_alarm);
	DDX_Control(pDX, IDC_GROUP_PREV_ALARM, m_group_prev_alarm);
	DDX_Control(pDX, IDC_GROUP_ALARM_INFO, m_group_alarm_info);
	DDX_Control(pDX, IDC_GIF_ALARM, m_gif_alarm);
	DDX_Control(pDX, IDC_DATA_ALARM_TYPE, m_data_alarm_type);
	DDX_Control(pDX, IDC_DATA_ALARM_CODE, m_data_alarm_code);
	DDX_Control(pDX, IDC_BTN_PREV_ALARM_10, m_btn_prev_alarm_10);
	DDX_Control(pDX, IDC_BTN_PREV_ALARM_9, m_btn_prev_alarm_9);
	DDX_Control(pDX, IDC_BTN_PREV_ALARM_8, m_btn_prev_alarm_8);
	DDX_Control(pDX, IDC_BTN_PREV_ALARM_7, m_btn_prev_alarm_7);
	DDX_Control(pDX, IDC_BTN_PREV_ALARM_6, m_btn_prev_alarm_6);
	DDX_Control(pDX, IDC_BTN_PREV_ALARM_5, m_btn_prev_alarm_5);
	DDX_Control(pDX, IDC_BTN_PREV_ALARM_4, m_btn_prev_alarm_4);
	DDX_Control(pDX, IDC_BTN_PREV_ALARM_3, m_btn_prev_alarm_3);
	DDX_Control(pDX, IDC_BTN_PREV_ALARM_2, m_btn_prev_alarm_2);
	DDX_Control(pDX, IDC_BTN_PREV_ALARM_1, m_btn_prev_alarm_1);
	DDX_Control(pDX, IDC_BTN_BUZZER_OFF, m_btn_buzzer_off);
	DDX_Control(pDX, IDOK, m_btn_alarm_ok);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialog_Alarm, CDialog)
	//{{AFX_MSG_MAP(CDialog_Alarm)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_BUZZER_OFF, OnBtnBuzzerOff)
	ON_BN_CLICKED(IDC_BTN_PREV_ALARM_1, OnBtnPrevAlarm1)
	ON_BN_CLICKED(IDC_BTN_PREV_ALARM_2, OnBtnPrevAlarm2)
	ON_BN_CLICKED(IDC_BTN_PREV_ALARM_3, OnBtnPrevAlarm3)
	ON_BN_CLICKED(IDC_BTN_PREV_ALARM_4, OnBtnPrevAlarm4)
	ON_BN_CLICKED(IDC_BTN_PREV_ALARM_5, OnBtnPrevAlarm5)
	ON_BN_CLICKED(IDC_BTN_PREV_ALARM_6, OnBtnPrevAlarm6)
	ON_BN_CLICKED(IDC_BTN_PREV_ALARM_7, OnBtnPrevAlarm7)
	ON_BN_CLICKED(IDC_BTN_PREV_ALARM_8, OnBtnPrevAlarm8)
	ON_BN_CLICKED(IDC_BTN_PREV_ALARM_9, OnBtnPrevAlarm9)
	ON_BN_CLICKED(IDC_BTN_PREV_ALARM_10, OnBtnPrevAlarm10)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialog_Alarm message handlers

BOOL CDialog_Alarm::Create(CWnd* pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

void CDialog_Alarm::PostNcDestroy() 
{
	((CMainFrame*)AfxGetMainWnd())->mp_cAlarm = NULL; // �˶� ȭ�� �ڵ� ���� �ʱ�ȭ
	delete this;
	
	CDialog::PostNcDestroy();  // Destroy �۾��� �̷������ ������ �� �κ� �ּ� ó��
}

BOOL CDialog_Alarm::OnInitDialog() 
{
	CDialog::OnInitDialog();

	mp_fontAlarm = NULL;
	mp_fontAlarm = new CFont;
	mp_fontAlarm->CreateFont(15,0,0,0,900,0,0,0,0,0,0,ANTIALIASED_QUALITY,0,"MS Sans Serif");

	int nLastAlarm;
	nLastAlarm = 0;

	CString sHappenedDate = "";
	CTime time;
	time = CTime::GetCurrentTime();

	// �߻��� �˶��� �����ϴ��� Ȯ��
	if (alarm.mstr_code != "")
	{
		sHappenedDate.Format("%4d-%02d-%02d (%02d:%02d:%02d)",
			time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());

		// �˶� ��Ʈ�� ���� �߻��� �˶� �ڵ� ����
		(alarm.mstr_list_jam).AddHead(alarm.mstr_code);
		(alarm.mstr_list_jam_time).AddTail(sHappenedDate);

		// �˶� ������ �ֱ� �������� �ִ� 10�� ������ ������
		// - ȭ�鿡 �˶� ��ȸ�� ��ư�� 10���ۿ� ���� ������
		nLastAlarm = (alarm.mstr_list_jam).GetCount();
		if (nLastAlarm > 10)
		{
			(alarm.mstr_list_jam).RemoveTail();
			(alarm.mstr_list_jam_time).RemoveTail();
		}
	}
	// �߻��� �˶��� �������� ����
	// - ����ڰ� ������ �߻��� �˶� ��ȸ �������� �˶� ȭ�� ��½�Ŵ
	else if (alarm.mstr_code == "")
	{
		// �˶� ����Ʈ�� �����ϴ� �� �˶� ���� Ȯ��
		nLastAlarm = (alarm.mstr_list_jam).GetCount();

		// �˶� �߻��� �̷��� ������ �⺻���� ȭ�� ������
		if (nLastAlarm == 0)
		{
			alarm.mstr_code = _T("999999");
			sprintf(alarm.mc_code, "999999");

			sHappenedDate.Format("%4d-%02d-%02d (%02d:%02d:%02d)",
				time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());

			(alarm.mstr_list_jam).AddTail(alarm.mstr_code);
			(alarm.mstr_list_jam_time).AddTail(sHappenedDate);
		}
		else
		{
			alarm.mstr_code = (alarm.mstr_list_jam).GetAt((alarm.mstr_list_jam).FindIndex(0));

			if (alarm.mstr_code == "")
			{
				alarm.mstr_code = _T("999999");
				sprintf(alarm.mc_code, "999999");

				sHappenedDate.Format("%4d-%02d-%02d (%02d:%02d:%02d)",
					time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());

				(alarm.mstr_list_jam).AddTail(alarm.mstr_code);
				(alarm.mstr_list_jam_time).AddTail(sHappenedDate);
			}
		}
	}

	// �߻��� �˶� �޽����� ���� ������ ���� ����ü�� �Ҵ���
	mcls_alarm_load.OnSet_Info2StrctAlarm(alarm.mstr_code);	// �߻��� �˶� ������ ����ü�� �Ҵ�
	
	OnSet_AlarmFileName();	// �˶� ������ ����� ���ϸ� ����
	OnReady_SearchPrevJamBtn();	// �˶� ��ȸ�� ��ư �غ�
	
	OnSet_GrpBoxCtrl();	// Į�� �׷�ڽ� ����
	OnSet_LblCtrl();	// Į�� �� ����
	OnSet_EditCtrl();	// Į�� ����Ʈ�ڽ� ����
	OnSet_GifCtrl();	// �̹���[GIF] �ε�
	OnSet_DigitalCtrl();	// LCD ���ڿ� ��� ��Ʈ�� ����
	
	OnShow_LotEndMsg(FALSE);	// [LOT END] �޽��� ȭ�� ���
	
	OnDisp_JamHappenedDate(1);	// �˶� �߻� �ð� ���� ���
	OnDisp_HappenedAlarm();	// �߻��� �˶� ���� ȭ�� ǥ��

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialog_Alarm::OnDestroy() 
{
	// ���̸� �߰� [2014.08.28]
	Func.OnSet_BuzzerSound(IO_OFF, 0);
	
	// LCD ������ ���� ��ũ�� ���� ����
	m_msg_lot_end.StopScroll();
	
	// ������ ��Ʈ ����
	delete mp_fontAlarm;
	mp_fontAlarm = NULL;
	
	// ���̸� �߰� [2014.08.28]
	m_image_alarm.ClearDrawing();
	m_image_alarm.Delete();
	
	alarm.mstr_code = "";
	// ���� �˶� ȭ���� ��µ��� �ʾҴٰ� �ش� �÷��� ����
	st_handler.nShowAlarmDlg = FALSE;

	if (st_handler.nMenuLock != FALSE)
	{
		st_handler.nMenuLock = FALSE;  // �޴� ��� ����
	}
	st_handler.nSwitchLock = FALSE;
	
	CDialog::OnDestroy();
}

//==================================================================//
// Į�� �׷�ڽ� ����
//==================================================================//
void CDialog_Alarm::OnSet_GrpBoxCtrl() 
{
	CSxLogFont logfAlarm(15, FW_SEMIBOLD, false, "Arial");

	m_group_alarm_info.SetFont(logfAlarm);
	m_group_alarm_info.SetCatptionTextColor(RGB(145,25,0));
	m_group_alarm_info.SetFontBold(TRUE);

	m_group_prev_alarm.SetFont(logfAlarm);
	m_group_prev_alarm.SetCatptionTextColor(RGB(145,25,0));
	m_group_prev_alarm.SetFontBold(TRUE);
}

//==================================================================//
// Į�� �� ����
//==================================================================//
void CDialog_Alarm::OnSet_LblCtrl() 
{
	m_msg_alarm_code.SetTextFont(mp_fontAlarm);
	m_msg_alarm_code.SetColor(RGB(0,0,0), RGB(255,255,255), 6, 4);
	m_msg_alarm_code.SetTextAlign(CFloatST::STA_CENTER, 10);

	m_data_alarm_code.SetTextColor(RGB(25,25,85));
	m_data_alarm_code.SetBkColor(RGB(180,230,250));
	m_data_alarm_code.SetFontBold(TRUE);
	m_data_alarm_code.SetFontName("MS Sans Serif");
	m_data_alarm_code.SetFontSize(12);

	m_msg_alarm_type.SetTextFont(mp_fontAlarm);
	m_msg_alarm_type.SetColor(RGB(0,0,0), RGB(255,255,255), 6, 4);
	m_msg_alarm_type.SetTextAlign(CFloatST::STA_CENTER, 10);

	m_data_alarm_type.SetTextColor(RGB(25,25,85));
	m_data_alarm_type.SetBkColor(RGB(180,230,250));
	m_data_alarm_type.SetFontBold(TRUE);
	m_data_alarm_type.SetFontName("MS Sans Serif");
	m_data_alarm_type.SetFontSize(12);

	m_msg_alarm_repare.SetTextFont(mp_fontAlarm);
	m_msg_alarm_repare.SetColor(RGB(0,0,0), RGB(255,255,255), 6, 4);
	m_msg_alarm_repare.SetTextAlign(CFloatST::STA_CENTER, 10);

	m_msg_alarm_title.SetTextFont(mp_fontAlarm);
	m_msg_alarm_title.SetColor(RGB(0,0,0), RGB(255,255,255), 6, 4);
	m_msg_alarm_title.SetTextAlign(CFloatST::STA_CENTER, 10);

	m_msg_alarm_time.SetTextFont(mp_fontAlarm);
	m_msg_alarm_time.SetColor(RGB(0,0,0), RGB(255,255,255), 6, 4);
	m_msg_alarm_time.SetTextAlign(CFloatST::STA_CENTER, 10);

	m_data_alarm_time.SetTextColor(RGB(25,25,85));
	m_data_alarm_time.SetBkColor(RGB(180,230,250));
	m_data_alarm_time.SetFontBold(TRUE);
	m_data_alarm_time.SetFontName("MS Sans Serif");
	m_data_alarm_time.SetFontSize(12);
}

//==================================================================//
// Į�� ����Ʈ�ڽ� ����
//==================================================================//
void CDialog_Alarm::OnSet_EditCtrl() 
{
	m_edit_alarm_title.SubclassDlgItem(IDC_EDIT_ALARM_TITLE, this);
	m_edit_alarm_title.bkColor(RGB(60,140,10));
	m_edit_alarm_title.textColor(RGB(255,255,255));
	m_edit_alarm_title.setFont(-12, FW_NORMAL, DEFAULT_PITCH | FF_DONTCARE, "MS Sans Serif");

	m_edit_alarm_repare.SubclassDlgItem(IDC_EDIT_ALARM_REPARE, this);
	m_edit_alarm_repare.bkColor(RGB(60,140,10));
	m_edit_alarm_repare.textColor(RGB(255,255,255));
	m_edit_alarm_repare.setFont(-12, FW_NORMAL, DEFAULT_PITCH | FF_DONTCARE, "MS Sans Serif");
}

//==================================================================//
// Į�� ����Ʈ�ڽ� ����
//==================================================================//
void CDialog_Alarm::OnSet_GifCtrl() 
{
	if (m_gif_alarm.Load(MAKEINTRESOURCE(IDR_GIF_ALARM), _T("GIF")))
	{
		m_gif_alarm.Draw();
	}
}

//==================================================================//
// LCD ���ڿ� ��� ��Ʈ�� ����
//==================================================================//
void CDialog_Alarm::OnSet_DigitalCtrl() 
{
	m_msg_lot_end.SetNumberOfLines(1);
	m_msg_lot_end.SetXCharsPerLine(16);
	m_msg_lot_end.SetSize(CMatrixStatic::LARGE);
	m_msg_lot_end.SetDisplayColors(RGB(0,0,0), RGB(255,60,0), RGB(103,30,0));
	m_msg_lot_end.AdjustClientXToSize(16);
	m_msg_lot_end.AdjustClientYToSize(1);
	m_msg_lot_end.SetAutoPadding(true, '!');
	m_msg_lot_end.DoScroll(300, CMatrixStatic::LEFT);
}

//==================================================================//
// �˶� ������ ����� ���ϸ� ����
//==================================================================//
void CDialog_Alarm::OnSet_AlarmFileName() 
{
	m_sfile_JamData	= "JamData.TXT";	// �˶� ��� ���� ���� �ε� ���ϸ� ����
	m_sfile_JamAct	= "JamAction.TXT";	// �˶� ��ġ ���� ���� �ε� ���ϸ� ����
}

//==================================================================//
// �߻��� �˶� ���� ȭ�� ǥ��
//==================================================================//
int CDialog_Alarm::OnDisp_HappenedAlarm() 
{
	CString sCheckData, sLoadImgFileName, sCurrentMsg;
	int nPos;
	
	if (alarm.mstr_code == "999999")
	{
		m_data_alarm_code.SetText(_T(alarm.mstr_pcode));	// �˶� �ڵ� ȭ�� ���
	}
	else	{
		m_data_alarm_code.SetText(_T(alarm.mstr_code));
	}
	
	sCurrentMsg  = "\r\n";
	sCurrentMsg += " " + st_alarm.mstr_cur_msg;
	((CEdit*)GetDlgItem(IDC_EDIT_ALARM_TITLE))->SetWindowText(_T(sCurrentMsg));	// �˶� Ÿ��Ʋ ����
	
	// �߻��� �˶��� ���� ���� �ε�
	sCheckData = mcls_alarm_load.OnLoad_AlarmDispInfo(st_path.mstr_path_alarm, m_sfile_JamData, m_sfile_JamAct, alarm.mstr_code);	// �߻��� �˶��� ���� ��� ���� ����
	nPos = sCheckData.Find("Trouble");
	if (nPos >= 0)	// �ε� �� ���� �߻�
	{
		((CEdit*)GetDlgItem(IDC_EDIT_ALARM_REPARE))->SetWindowText(sCheckData);
		return FALSE;
	}
	
	// ȭ�鿡 ����� �׸� ���ϸ� ����
	sLoadImgFileName = st_path.mstr_path_alarm + st_alarm.mstr_bmp_file;
	
	// ���̸� �߰� [2014.08.28]
	int nExistFile = 0;
	nExistFile = access(sLoadImgFileName, 0);
	if (nExistFile)
	{
		((CEdit*)GetDlgItem(IDC_EDIT_ALARM_REPARE))->SetWindowText(sLoadImgFileName);
		
		if (st_handler.cwnd_list != NULL)
		{
			printf(st_msg.c_abnormal_msg, "[%s] file is not exist.", sLoadImgFileName);
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
		}
		return FALSE;
	}
	
	m_image_alarm.ClearDrawing();	// ���� ��� �׸� ���� ���� ����
	if (!m_image_alarm.LoadImage(sLoadImgFileName))  // �߻��� �˶��� ���� �׸� �ε�
	{
		sCheckData  = "Picture File Name : " + sLoadImgFileName;
		sCheckData += "\r\n";
		sCheckData += "Loading Fail";
		((CEdit*)GetDlgItem(IDC_EDIT_ALARM_REPARE))->SetWindowText(sCheckData);
		
		return FALSE;
	}
	
	m_image_alarm.Invalidate();	// ȭ�� ��� ����

	OnSet_JamDispStyle(st_path.mstr_path_alarm);	// �߻��� �˶� ���� ǥ�� ��� ���� �Լ�
	OnDisp_JamTypeInfo(alarm.mn_type_mode);	// �߻��� �˶� ���� ȭ�� ǥ��
	
	return TRUE;
}

//==================================================================//
// �߻��� �˶� ǥ�� ���� ����
//==================================================================//
void CDialog_Alarm::OnSet_JamDispStyle(CString szPath) 
{
	if (st_alarm.mstr_style == "NONE")
	{
		return;
	}
	else if (st_alarm.mstr_style == "CIRCLE")
	{
		m_image_alarm.SetCircleStyle(st_alarm.mn_Cx, st_alarm.mn_Cy, st_alarm.mn_Offset);
		m_image_alarm.SetLineStyle(st_alarm.mn_LineWidth, RGB(st_alarm.mn_R, st_alarm.mn_G, st_alarm.mn_B));
		return;
	}
	else if (st_alarm.mstr_style == "ARROW_LEFT")
	{
		m_image_alarm.SetArrowStyle(st_alarm.mn_Cx, st_alarm.mn_Cy, CBmpImageST::JS_ARROW_LEFT, szPath, st_alarm.mn_Offset);
		return;
	}
	else if (st_alarm.mstr_style == "ARROW_RIGHT")
	{
		m_image_alarm.SetArrowStyle(st_alarm.mn_Cx, st_alarm.mn_Cy, CBmpImageST::JS_ARROW_RIGHT, szPath, st_alarm.mn_Offset);
		return;
	}
	else if (st_alarm.mstr_style == "ARROW_UP")
	{
		m_image_alarm.SetArrowStyle(st_alarm.mn_Cx, st_alarm.mn_Cy, CBmpImageST::JS_ARROW_UP, szPath, st_alarm.mn_Offset);
		return;
	}
	else if (st_alarm.mstr_style == "ARROW_DOWN")
	{
		m_image_alarm.SetArrowStyle(st_alarm.mn_Cx, st_alarm.mn_Cy, CBmpImageST::JS_ARROW_DOWN, szPath, st_alarm.mn_Offset);
		return;
	}
	else if (st_alarm.mstr_style == "ARROW_LEFTUP")
	{
		m_image_alarm.SetArrowStyle(st_alarm.mn_Cx, st_alarm.mn_Cy, CBmpImageST::JS_ARROW_LEFTUP, szPath, st_alarm.mn_Offset);
		return;
	}
	else if (st_alarm.mstr_style == "ARROW_LEFTDOWN")
	{
		m_image_alarm.SetArrowStyle(st_alarm.mn_Cx, st_alarm.mn_Cy, CBmpImageST::JS_ARROW_LEFTDOWN, szPath, st_alarm.mn_Offset);
		return;
	}
	else if (st_alarm.mstr_style == "ARROW_RIGHTUP")
	{
		m_image_alarm.SetArrowStyle(st_alarm.mn_Cx, st_alarm.mn_Cy, CBmpImageST::JS_ARROW_RIGHTUP, szPath, st_alarm.mn_Offset);
		return;
	}
	else if (st_alarm.mstr_style == "ARROW_RIGHTDOWN")
	{
		m_image_alarm.SetArrowStyle(st_alarm.mn_Cx, st_alarm.mn_Cy, CBmpImageST::JS_ARROW_RIGHTDOWN, szPath, st_alarm.mn_Offset);
		return;
	}
}

//==================================================================//
// �߻��� �˶� ���� ȭ�� ǥ��
//==================================================================//
void CDialog_Alarm::OnDisp_JamTypeInfo(int nzAlarmState) 
{
	CString sState;
	
	switch(nzAlarmState)
	{
	case 0:
		sState = "Warning";
		break;
		
	case 1:
		sState = "Running";
		break;
		
	case 2:
		sState = "Stop";
		break;
		
	default:
		sState = "Error";
		break;
	}
	m_data_alarm_type.SetText(_T(sState));
	
	// �˶� ��ġ ���� ���
	((CEdit*)GetDlgItem(IDC_EDIT_ALARM_REPARE))->SetWindowText(_T(st_alarm.mstr_repair_msg));
	
	if (st_work.nEqpStatus == dLOTEND)
	{
		OnShow_LotEndMsg(TRUE);	// [LOT END] �޽��� ȭ�� ���
	}
}

//==================================================================//
// [LOT END] �޽��� ȭ�� ���
//==================================================================//
void CDialog_Alarm::OnShow_LotEndMsg(int nzState) 
{
	switch(nzState)
	{
	case 0:
		m_msg_lot_end.ShowWindow(SW_HIDE);
		break;
		
	case 1:
		m_msg_lot_end.ShowWindow(SW_SHOW);
		m_msg_lot_end.SetText(_T("Lot End"));
		break;
	}
}

//==================================================================//
// �˶� ��ȸ�� ��ư �غ�
// - ������ �� �ִ� �˶� �ڵ� ������ ��ư ĸ�� ����
//==================================================================//
void CDialog_Alarm::OnReady_SearchPrevJamBtn() 
{
	int i;
	// ������� �߻��� �˶� ���� Ȯ��
	int nCount = (alarm.mstr_list_jam).GetCount();
	OnToggle_PrevJamBtn(nCount);	// ���� �˶� ��ȸ�� ��ư �����ֱ�

	if (nCount > 10)	{
		nCount = 10;
	}

	for(i=0; i<nCount; i++)
	{
		mp_sJamCode[i] = (alarm.mstr_list_jam).GetAt((alarm.mstr_list_jam).FindIndex(i));
	}

	OnDisp_PrevJamBtnCaption(nCount);	// �˶� ��ȸ�� ��ư�� ĸ�� ����
}

//==================================================================//
// ���� �˶� ��ȸ�� ��ư �����ֱ�
//==================================================================//
void CDialog_Alarm::OnToggle_PrevJamBtn(int nzCount) 
{
	if (nzCount > 10)	{
		nzCount = 10;
	}

	m_btn_prev_alarm_1.ShowWindow(FALSE);
	m_btn_prev_alarm_2.ShowWindow(FALSE);
	m_btn_prev_alarm_3.ShowWindow(FALSE);
	m_btn_prev_alarm_4.ShowWindow(FALSE);
	m_btn_prev_alarm_5.ShowWindow(FALSE);
	m_btn_prev_alarm_6.ShowWindow(FALSE);
	m_btn_prev_alarm_7.ShowWindow(FALSE);
	m_btn_prev_alarm_8.ShowWindow(FALSE);
	m_btn_prev_alarm_9.ShowWindow(FALSE);
	m_btn_prev_alarm_10.ShowWindow(FALSE);

	switch(nzCount)
	{
	case 0:
		break;
	case 1:
		m_btn_prev_alarm_1.ShowWindow(TRUE);
		break;
	case 2:
		m_btn_prev_alarm_1.ShowWindow(TRUE);
		m_btn_prev_alarm_2.ShowWindow(TRUE);
		break;
	case 3:
		m_btn_prev_alarm_1.ShowWindow(TRUE);
		m_btn_prev_alarm_2.ShowWindow(TRUE);
		m_btn_prev_alarm_3.ShowWindow(TRUE);
		break;
	case 4:
		m_btn_prev_alarm_1.ShowWindow(TRUE);
		m_btn_prev_alarm_2.ShowWindow(TRUE);
		m_btn_prev_alarm_3.ShowWindow(TRUE);
		m_btn_prev_alarm_4.ShowWindow(TRUE);
		break;
	case 5:
		m_btn_prev_alarm_1.ShowWindow(TRUE);
		m_btn_prev_alarm_2.ShowWindow(TRUE);
		m_btn_prev_alarm_3.ShowWindow(TRUE);
		m_btn_prev_alarm_4.ShowWindow(TRUE);
		m_btn_prev_alarm_5.ShowWindow(TRUE);
		break;
	case 6:
		m_btn_prev_alarm_1.ShowWindow(TRUE);
		m_btn_prev_alarm_2.ShowWindow(TRUE);
		m_btn_prev_alarm_3.ShowWindow(TRUE);
		m_btn_prev_alarm_4.ShowWindow(TRUE);
		m_btn_prev_alarm_5.ShowWindow(TRUE);
		m_btn_prev_alarm_6.ShowWindow(TRUE);
		break;
	case 7:
		m_btn_prev_alarm_1.ShowWindow(TRUE);
		m_btn_prev_alarm_2.ShowWindow(TRUE);
		m_btn_prev_alarm_3.ShowWindow(TRUE);
		m_btn_prev_alarm_4.ShowWindow(TRUE);
		m_btn_prev_alarm_5.ShowWindow(TRUE);
		m_btn_prev_alarm_6.ShowWindow(TRUE);
		m_btn_prev_alarm_7.ShowWindow(TRUE);
		break;
	case 8:
		m_btn_prev_alarm_1.ShowWindow(TRUE);
		m_btn_prev_alarm_2.ShowWindow(TRUE);
		m_btn_prev_alarm_3.ShowWindow(TRUE);
		m_btn_prev_alarm_4.ShowWindow(TRUE);
		m_btn_prev_alarm_5.ShowWindow(TRUE);
		m_btn_prev_alarm_6.ShowWindow(TRUE);
		m_btn_prev_alarm_7.ShowWindow(TRUE);
		m_btn_prev_alarm_8.ShowWindow(TRUE);
		break;
	case 9:
		m_btn_prev_alarm_1.ShowWindow(TRUE);
		m_btn_prev_alarm_2.ShowWindow(TRUE);
		m_btn_prev_alarm_3.ShowWindow(TRUE);
		m_btn_prev_alarm_4.ShowWindow(TRUE);
		m_btn_prev_alarm_5.ShowWindow(TRUE);
		m_btn_prev_alarm_6.ShowWindow(TRUE);
		m_btn_prev_alarm_7.ShowWindow(TRUE);
		m_btn_prev_alarm_8.ShowWindow(TRUE);
		m_btn_prev_alarm_9.ShowWindow(TRUE);
		break;
	case 10:
		m_btn_prev_alarm_1.ShowWindow(TRUE);
		m_btn_prev_alarm_2.ShowWindow(TRUE);
		m_btn_prev_alarm_3.ShowWindow(TRUE);
		m_btn_prev_alarm_4.ShowWindow(TRUE);
		m_btn_prev_alarm_5.ShowWindow(TRUE);
		m_btn_prev_alarm_6.ShowWindow(TRUE);
		m_btn_prev_alarm_7.ShowWindow(TRUE);
		m_btn_prev_alarm_8.ShowWindow(TRUE);
		m_btn_prev_alarm_9.ShowWindow(TRUE);
		m_btn_prev_alarm_10.ShowWindow(TRUE);
		break;
	}
}

//==================================================================//
// �˶� ��ȸ�� ��ư�� ĸ�� ����
//==================================================================//
void CDialog_Alarm::OnDisp_PrevJamBtnCaption(int nzCount) 
{
	/*
	SetDlgItemText(IDC_BTN_PREV_ALARM_1,	_T(mp_sJamCode[nzCount -1]));
	SetDlgItemText(IDC_BTN_PREV_ALARM_2,	_T(mp_sJamCode[nzCount -2]));
	SetDlgItemText(IDC_BTN_PREV_ALARM_3,	_T(mp_sJamCode[nzCount -3]));
	SetDlgItemText(IDC_BTN_PREV_ALARM_4,	_T(mp_sJamCode[nzCount -4]));
	SetDlgItemText(IDC_BTN_PREV_ALARM_5,	_T(mp_sJamCode[nzCount -5]));
	SetDlgItemText(IDC_BTN_PREV_ALARM_6,	_T(mp_sJamCode[nzCount -6]));
	SetDlgItemText(IDC_BTN_PREV_ALARM_7,	_T(mp_sJamCode[nzCount -7]));
	SetDlgItemText(IDC_BTN_PREV_ALARM_8,	_T(mp_sJamCode[nzCount -8]));
	SetDlgItemText(IDC_BTN_PREV_ALARM_9,	_T(mp_sJamCode[nzCount -9]));
	SetDlgItemText(IDC_BTN_PREV_ALARM_10,	_T(mp_sJamCode[nzCount -10]));
	*/
	SetDlgItemText(IDC_BTN_PREV_ALARM_1,	_T(mp_sJamCode[0]));
	SetDlgItemText(IDC_BTN_PREV_ALARM_2,	_T(mp_sJamCode[1]));
	SetDlgItemText(IDC_BTN_PREV_ALARM_3,	_T(mp_sJamCode[2]));
	SetDlgItemText(IDC_BTN_PREV_ALARM_4,	_T(mp_sJamCode[3]));
	SetDlgItemText(IDC_BTN_PREV_ALARM_5,	_T(mp_sJamCode[4]));
	SetDlgItemText(IDC_BTN_PREV_ALARM_6,	_T(mp_sJamCode[5]));
	SetDlgItemText(IDC_BTN_PREV_ALARM_7,	_T(mp_sJamCode[6]));
	SetDlgItemText(IDC_BTN_PREV_ALARM_8,	_T(mp_sJamCode[7]));
	SetDlgItemText(IDC_BTN_PREV_ALARM_9,	_T(mp_sJamCode[8]));
	SetDlgItemText(IDC_BTN_PREV_ALARM_10,	_T(mp_sJamCode[9]));
}

//==================================================================//
// ����ڰ� ������ ���� �˶� �ڵ� ã��
//==================================================================//
CString CDialog_Alarm::OnSearch_PrevJamCode(int nzCount) 
{
	int i;
	// �߻��ߴ� �˶� ���� Ȯ��
	int nCount = (alarm.mstr_list_jam).GetCount();
	return mp_sJamCode[nzCount -1];


	if (nzCount > 10)	{
		nzCount = 10;
	}

	for(i=0; i<nCount; i++)
	{
		mp_sJamCode[i] = (alarm.mstr_list_jam).GetAt((alarm.mstr_list_jam).FindIndex(i));
	}

	return mp_sJamCode[nCount - nzCount];
}

//==================================================================//
// ���õ� �˶� ���� ȭ�� ���
//==================================================================//
void CDialog_Alarm::OnDisp_SelectedPrevJam(int nzSelectedPos) 
{
	OnShow_LotEndMsg(FALSE);	// [LOT END] �޽��� ȭ�� ���

	// ����ڰ� ������ ���� �˶� �ڵ� ã��
	alarm.mstr_code = OnSearch_PrevJamCode(nzSelectedPos);
	
	((CEdit*)GetDlgItem(IDC_EDIT_ALARM_REPARE))->SetWindowText("");
	m_image_alarm.ClearDrawing();
	m_data_alarm_type.SetText("");
	m_data_alarm_code.SetText("");
	((CEdit*)GetDlgItem(IDC_EDIT_ALARM_TITLE))->SetWindowText("");
	((CEdit*)GetDlgItem(IDC_EDIT_ALARM_REPARE))->SetWindowText("");
	
	mcls_alarm_load.OnSet_Info2StrctAlarm(alarm.mstr_code);	// �߻��� �˶� ������ ����ü�� �Ҵ�
	OnDisp_HappenedAlarm();	// �߻��� �˶� ���� ȭ�� ǥ��
	OnDisp_JamHappenedDate(nzSelectedPos);	// �˶� �߻� �ð� ���� ���
}

//==================================================================//
// �˶� �߻� �ð� ���� ���
//==================================================================//
void CDialog_Alarm::OnDisp_JamHappenedDate(int nzHappenedPos) 
{
	// ���簡�� �߻��� �˶� ���� ���� ����
	// - �ִ�� �ֱ� �߻��� �˶� �������� 10������ ������
	int nIdx = (alarm.mstr_list_jam_time).GetCount();
	// �� �������� �߰��� �˶��� ���� �߻� �ð� ���� ����
	// - �� �������� ����Ʈ�� �߰��� ������ ���� �ֽ� ������
	CString sHappenedDate = (alarm.mstr_list_jam_time).GetAt((alarm.mstr_list_jam_time).FindIndex(nIdx - nzHappenedPos));
	m_data_alarm_time.SetText(_T(sHappenedDate));
}

void CDialog_Alarm::OnBtnBuzzerOff() 
{
	// ���̸� �߰� [2014.08.29]
	Func.OnSet_BuzzerSound(IO_OFF, 0);	// ���� ���� ON/OFF

}

void CDialog_Alarm::OnBtnPrevAlarm1() 
{
	OnDisp_SelectedPrevJam(1);	// ����ڰ� ������ ���� �˶� ���� ȭ�� ���
}

void CDialog_Alarm::OnBtnPrevAlarm2() 
{
	OnDisp_SelectedPrevJam(2);
}

void CDialog_Alarm::OnBtnPrevAlarm3() 
{
	OnDisp_SelectedPrevJam(3);
}

void CDialog_Alarm::OnBtnPrevAlarm4() 
{
	OnDisp_SelectedPrevJam(4);
}

void CDialog_Alarm::OnBtnPrevAlarm5() 
{
	OnDisp_SelectedPrevJam(5);
}

void CDialog_Alarm::OnBtnPrevAlarm6() 
{
	OnDisp_SelectedPrevJam(6);
}

void CDialog_Alarm::OnBtnPrevAlarm7() 
{
	OnDisp_SelectedPrevJam(7);
}

void CDialog_Alarm::OnBtnPrevAlarm8() 
{
	OnDisp_SelectedPrevJam(8);
}

void CDialog_Alarm::OnBtnPrevAlarm9() 
{
	OnDisp_SelectedPrevJam(9);
}

void CDialog_Alarm::OnBtnPrevAlarm10() 
{
	OnDisp_SelectedPrevJam(10);
}

void CDialog_Alarm::OnOK() 
{
	// ���̸� �߰� [2014.08.26]
	if (st_handler.cwnd_list != NULL)
	{
		sprintf(st_msg.c_normal_msg, "[JAM RESET] JAM RESET.");
		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
	}
	Func.OnSet_BuzzerSound(IO_OFF, 0);
	
	if (st_work.nEqpStatus == dWARNING ||
		st_work.nEqpStatus == dJAM ||
		st_work.nEqpStatus == dLOTEND)
	{
		Func.OnSet_EqpStatusStop();	// [STOP] ���� ���� ����
	}
	
	DestroyWindow();
}

BOOL CDialog_Alarm::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN ||
		pMsg->message == WM_SYSKEYDOWN)
	{
		switch(pMsg->wParam)
		{
		case VK_RETURN:
			return FALSE;
			break;  // Exit�� Return ���� ������� �ʵ��� ó��
		case VK_ESCAPE:
			return FALSE;
			break;
		case VK_SPACE:
			pMsg->wParam = NULL;
			return FALSE;
			break;
		case VK_F4:
			pMsg->wParam = NULL;
			return FALSE;
			break;
		case VK_F9:
			SendMessage(WM_KEYDOWN, pMsg->wParam, pMsg->lParam);
			return TRUE;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

//==================================================================//
// ESC Ű ������ ȭ�� ����� ����
//==================================================================//
void CDialog_Alarm::OnCancel() 
{
	return;
}
