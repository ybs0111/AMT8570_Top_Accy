// Screen_Set_Maintenance.cpp : implementation file
//

#include "stdafx.h"
#include "Handler.h"
#include "Screen_Set_Maintenance.h"

#include "io.h"  // ���� ���� ���� �˻� �Լ� ȣ���ϱ� ���ؼ��� �ݵ�� INCLUDE �ʿ�
#include "FileDialogST.h"
// ******************************************************************************
// ��ȭ ���� Ŭ���� �߰�                                                         
// ******************************************************************************
//#include "Dialog_Infor.h"
#include "Dialog_Select.h"
#include "Dialog_Message.h"

#include "Dialog_KeyPad.h"
#include "Dialog_Keyboard.h"

#include "Dialog_Pass_Change.h"
// ******************************************************************************

#include "AMTLotManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScreen_Set_Maintenance

IMPLEMENT_DYNCREATE(CScreen_Set_Maintenance, CFormView)

CScreen_Set_Maintenance::CScreen_Set_Maintenance()
	: CFormView(CScreen_Set_Maintenance::IDD)
{
	//{{AFX_DATA_INIT(CScreen_Set_Maintenance)
	//}}AFX_DATA_INIT
}

CScreen_Set_Maintenance::~CScreen_Set_Maintenance()
{
}

void CScreen_Set_Maintenance::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CScreen_Set_Maintenance)
	DDX_Control(pDX, IDC_GROUP_PITCH, m_group_pitch);
	DDX_Control(pDX, IDC_GROUP_NETWORK_ADDR, m_group_network_addr);
	DDX_Control(pDX, IDC_BTN_NETWORK_ADDR_LOAD, m_btn_network_load);
	DDX_Control(pDX, IDC_BTN_NETWORK_ADDR_MODIFY, m_btn_network_modify);
	DDX_Control(pDX, IDC_BTN_PASSWORD_CHANGE_LEVLE2, m_btn_password_change_level2);
	DDX_Control(pDX, IDC_BTN_PASSWORD_CHANGE_LEVLE1, m_btn_password_change_level1);
	DDX_Control(pDX, IDC_BTN_DATA_BACKUP, m_btn_data_backup);
	DDX_Control(pDX, IDC_BTN_STEP_SAVE, m_btn_step_save);
	DDX_Control(pDX, IDC_GROUP_LAMP_CHANGE_TIME, m_group_lamp_change_time);
	DDX_Control(pDX, IDC_LAMP_STOP, m_lamp_stop);
	DDX_Control(pDX, IDC_LAMP_SELFCHECK, m_lamp_selfcheck);
	DDX_Control(pDX, IDC_LAMP_WARNING, m_lamp_warning);
	DDX_Control(pDX, IDC_LAMP_RUN, m_lamp_run);
	DDX_Control(pDX, IDC_LAMP_LOTEND, m_lamp_lotend);
	DDX_Control(pDX, IDC_LAMP_LOCK, m_lamp_lock);
	DDX_Control(pDX, IDC_LAMP_INITIAL, m_lamp_initial);
	DDX_Control(pDX, IDC_LAMP_ALARM, m_lamp_alarm);
	DDX_Control(pDX, IDC_GROUP_TOWERLAMP, m_group_towerlamp);
	DDX_Control(pDX, IDC_GROUP_PASSWORD_CHANGE, m_group_password_change);
	DDX_Control(pDX, IDC_GROUP_LAMP_WARNING, m_group_lamp_warning);
	DDX_Control(pDX, IDC_GROUP_LAMP_STOP, m_group_lamp_stop);
	DDX_Control(pDX, IDC_GROUP_LAMP_SELFCHECK, m_group_lamp_selfcheck);
	DDX_Control(pDX, IDC_GROUP_LAMP_RUN, m_group_lamp_run);
	DDX_Control(pDX, IDC_GROUP_LAMP_LOTEND, m_group_lamp_lotend);
	DDX_Control(pDX, IDC_GROUP_LAMP_LOCK, m_group_lamp_lock);
	DDX_Control(pDX, IDC_GROUP_LAMP_INITIAL, m_group_lamp_initial);
	DDX_Control(pDX, IDC_GROUP_LAMP_FLICKER_TIME, m_group_lamp_flicker_time);
	DDX_Control(pDX, IDC_GROUP_LAMP_ALARM, m_group_lamp_alarm);
	DDX_Control(pDX, IDC_GROUP_DATA_BACKUP, m_group_data_backup);
	DDX_Control(pDX, IDC_GROUP_BUZZER, m_group_buzzer);
	DDX_Control(pDX, IDC_CHK_BUZZER, m_chk_buzzer);
	DDX_Control(pDX, IDC_BTN_MAINTENANCE_APPLY, m_btn_maintenance_apply);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CScreen_Set_Maintenance, CFormView)
	//{{AFX_MSG_MAP(CScreen_Set_Maintenance)
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_DGT_FLICKER_TIME, OnDgtFlickerTime)
	ON_BN_CLICKED(IDC_CHK_BUZZER, OnChkBuzzer)
	ON_BN_CLICKED(IDC_BTN_DATA_BACKUP, OnBtnDataBackup)
	ON_BN_CLICKED(IDC_BTN_PASSWORD_CHANGE_LEVLE1, OnBtnPasswordChangeLevle1)
	ON_BN_CLICKED(IDC_BTN_PASSWORD_CHANGE_LEVLE2, OnBtnPasswordChangeLevle2)
	ON_BN_CLICKED(IDC_BTN_MAINTENANCE_APPLY, OnBtnMaintenanceApply)
	ON_BN_CLICKED(IDC_DGT_CHANGE_TIME, OnDgtChangeTime)
	ON_BN_CLICKED(IDC_BTN_STEP_SAVE, OnBtnStepSave)
	ON_BN_CLICKED(IDC_BTN_NETWORK_ADDR_LOAD, OnBtnNetworkAddrLoad)
	ON_BN_CLICKED(IDC_BTN_NETWORK_ADDR_MODIFY, OnBtnNetworkAddrModify)
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(ID_LAMP_RLAMP, ID_LAMP_RLAMP+SELFCHECK, OnRLampClick)
	ON_COMMAND_RANGE(ID_LAMP_YLAMP, ID_LAMP_YLAMP+SELFCHECK, OnYLampClick)
	ON_COMMAND_RANGE(ID_LAMP_GLAMP, ID_LAMP_GLAMP+SELFCHECK, OnGLampClick)
	ON_MESSAGE(SSM_CLICK  , OnCellClick)
	ON_MESSAGE(WM_MAINTENANCE_APPLY, OnMaintenance_Info_Apply)				// Tower Lamp DATA�� APPLY ��Ű�� ���� ����� ���� �޽��� ����
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScreen_Set_Maintenance diagnostics

#ifdef _DEBUG
void CScreen_Set_Maintenance::AssertValid() const
{
	CFormView::AssertValid();
}

void CScreen_Set_Maintenance::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CScreen_Set_Maintenance message ICTs

void CScreen_Set_Maintenance::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();

	// **************************************************************************
	// ��Ʈ�ѿ� ������ ��Ʈ �����Ѵ�                                             
	// **************************************************************************
	mp_maintenance_font = NULL;
	mp_maintenance_font = new CFont;
	mp_maintenance_font->CreateFont(15,0,0,0,900,0,0,0,0,0,0,DEFAULT_QUALITY,0,"MS Sans Serif");
	// **************************************************************************

	mb_first = TRUE;

	OnMaintenance_GroupBox_Set();
	OnMaintenance_Button_Set();
	OnMaintenance_EditBox_Set();
	OnMaintenance_Label_Set();
	OnMaintenance_Digital_Count_Set();

	OnMaintenance_Data_Set();
	OnMaintenance_Data_BackUp();
	OnMaintenance_Data_Display();
	OnMaintenance_Buzzer_Change();

	OnMaintenance_Init_Pitch();
}

void CScreen_Set_Maintenance::OnDestroy() 
{
	int mn_response;  // ��ȭ ���ڿ� ���� ���� �� ���� ����
	int Ret;

	CDialog_Select select_dlg;

	//���α׷��� ���������鼭 Save ���� �ʰ�, Form�� Destroy�Ǵ°��̶�� Setting�� �����Ѵٴ� ���̴�.
	//�񱳵� ���� ����.
	if (st_handler.b_program_exit == false)
	{
		Ret = OnMaintenance_Data_Comparison();

		if (Ret == RET_ERROR)
		{
			st_msg.mstr_confirm_msg = _T("Changed Data! Apply?");
			mn_response = select_dlg.DoModal();
			
			if (mn_response == IDOK)
			{
				OnMaintenance_Data_Apply();

				OnMaintenance_LogFile_Create();

				OnMaintenance_ListBar_Info_Send();
			}
			else if (mn_response == IDCANCEL)
			{
				OnMaintenance_Data_Recovery();

				OnMaintenance_Data_Display();

				OnMaintenance_Data_Apply();

				OnMaintenance_ListBar_Info_Send();
			}	
		}
	}

	// **************************************************************************
	// ������ ��Ʈ ���� �����Ѵ�                                                 
	// **************************************************************************
	delete mp_maintenance_font;
	mp_maintenance_font = NULL;

	// **************************************************************************


	CFormView::OnDestroy();	
}

void CScreen_Set_Maintenance::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	if (mb_first)          // Ÿ�� ���� ó�� ���� ���� �˻�
	{
		mb_first = FALSE;  // Ÿ�� ���� ó�� ���� ���� �÷��� ����

		// **********************************************************************
		// Ÿ�� ���� ����� ��Ʈ�� ���� ũ�� ��´�                              
		// -> Ÿ�� ���� ��� ���� ���� �Ŀ��� �ش� ��Ʈ�� ������ �ʰ� �����     
		// -> Ÿ�� ���� ���������� ����ϴ� ���� ����̱� �����̴�               
		// -> GetWindowRect() : �����츦 �ѷ��δ� �簢���� ũ�⸦ ����           
		//    lprc�� ����Ű�� RECT ����ü�� �� ���� �����Ѵ�                     
		//  : ����� ��ǥ�� �ȼ� �����̸� ȭ���� ���� ����� �������� ����Ѵ�   
		//  : ���� �������� ũ��� ��ġ�� ȭ�� ��� ��ǥ�� �˰� ���� �� ����Ѵ� 
		// **********************************************************************
		CRect r;
		GetWindowRect(&r);

		m_lamp_run.GetWindowRect(&mr_area[RUN]);
		m_lamp_stop.GetWindowRect(&mr_area[STOP]);
		m_lamp_alarm.GetWindowRect(&mr_area[ALARM]);
		m_lamp_lock.GetWindowRect(&mr_area[LOCK]);
		m_lamp_lotend.GetWindowRect(&mr_area[LOTEND]);
		m_lamp_initial.GetWindowRect(&mr_area[INIT]);
		m_lamp_warning.GetWindowRect(&mr_area[WARNING]);
		m_lamp_selfcheck.GetWindowRect(&mr_area[SELFCHECK]);

		for(int i=0; i<=SELFCHECK; i++)
		{
			mr_area[i].left -= r.left;
			mr_area[i].right -= r.left;
			mr_area[i].top -= r.top;
			mr_area[i].bottom -= r.top;
		}

		m_lamp_run.ShowWindow(SW_HIDE);
		m_lamp_stop.ShowWindow(SW_HIDE);
		m_lamp_alarm.ShowWindow(SW_HIDE);
		m_lamp_lock.ShowWindow(SW_HIDE);
		m_lamp_lotend.ShowWindow(SW_HIDE);
		m_lamp_initial.ShowWindow(SW_HIDE);
		m_lamp_warning.ShowWindow(SW_HIDE);
		m_lamp_selfcheck.ShowWindow(SW_HIDE);
		// **********************************************************************

		for(int j=0; j<=SELFCHECK; j++)
			DrawButton(j);							// Ÿ�� ���� ��ư ���� �Լ�

		for(int k=0; k<=SELFCHECK; k++)
		{
			OnMaintenance_Red_State_Change(k);		// RED Ÿ�� ���� ��ư ���� ��ȯ �Լ�
			OnMaintenance_Yellow_State_Change(k);	// YELLOW Ÿ�� ���� ��ư ���� ��ȯ �Լ�
			OnMaintenance_Green_State_Change(k);	// GREEN Ÿ�� ���� ��ư ���� ��ȯ �Լ�
		}
	}
}

void CScreen_Set_Maintenance::OnMaintenance_GroupBox_Set()
{
	CSxLogFont maintenance_font(15,FW_SEMIBOLD,false,"MS Sans Serif");

	m_group_towerlamp.SetFont(maintenance_font);
	m_group_towerlamp.SetCatptionTextColor(RGB(145,25,0));
	m_group_towerlamp.SetFontBold(TRUE);

	m_group_lamp_run.SetFont(maintenance_font);
	m_group_lamp_run.SetCatptionTextColor(RGB(145,25,0));
	m_group_lamp_run.SetFontBold(TRUE);

	m_group_lamp_stop.SetFont(maintenance_font);
	m_group_lamp_stop.SetCatptionTextColor(RGB(145,25,0));
	m_group_lamp_stop.SetFontBold(TRUE);

	m_group_lamp_alarm.SetFont(maintenance_font);
	m_group_lamp_alarm.SetCatptionTextColor(RGB(145,25,0));
	m_group_lamp_alarm.SetFontBold(TRUE);

	m_group_lamp_lock.SetFont(maintenance_font);
	m_group_lamp_lock.SetCatptionTextColor(RGB(145,25,0));
	m_group_lamp_lock.SetFontBold(TRUE);

	m_group_lamp_lotend.SetFont(maintenance_font);
	m_group_lamp_lotend.SetCatptionTextColor(RGB(145,25,0));
	m_group_lamp_lotend.SetFontBold(TRUE);

	m_group_lamp_initial.SetFont(maintenance_font);
	m_group_lamp_initial.SetCatptionTextColor(RGB(145,25,0));
	m_group_lamp_initial.SetFontBold(TRUE);
	m_group_lamp_initial.ShowWindow(FALSE);

	m_group_lamp_warning.SetFont(maintenance_font);
	m_group_lamp_warning.SetCatptionTextColor(RGB(145,25,0));
	m_group_lamp_warning.SetFontBold(TRUE);

	m_group_lamp_selfcheck.SetFont(maintenance_font);
	m_group_lamp_selfcheck.SetCatptionTextColor(RGB(145,25,0));
	m_group_lamp_selfcheck.SetFontBold(TRUE);

	m_group_lamp_flicker_time.SetFont(maintenance_font);
	m_group_lamp_flicker_time.SetCatptionTextColor(RGB(145,25,0));
	m_group_lamp_flicker_time.SetFontBold(TRUE);

	m_group_buzzer.SetFont(maintenance_font);
	m_group_buzzer.SetCatptionTextColor(RGB(145,25,0));
	m_group_buzzer.SetFontBold(TRUE);

	m_group_network_addr.SetFont(maintenance_font);
	m_group_network_addr.SetCatptionTextColor(RGB(145,25,0));
	m_group_network_addr.SetFontBold(TRUE);

	m_group_password_change.SetFont(maintenance_font);
	m_group_password_change.SetCatptionTextColor(RGB(145,25,0));
	m_group_password_change.SetFontBold(TRUE);

	m_group_data_backup.SetFont(maintenance_font);
	m_group_data_backup.SetCatptionTextColor(RGB(145,25,0));
	m_group_data_backup.SetFontBold(TRUE);

	m_group_lamp_change_time.SetFont(maintenance_font);
	m_group_lamp_change_time.SetCatptionTextColor(RGB(145,25,0));
	m_group_lamp_change_time.SetFontBold(TRUE);
}

void CScreen_Set_Maintenance::DrawButton(int i)
{
	// **************************************************************************
    // Ÿ�� ���� ����� ��Ʈ�ѿ� ���� ���� ��´�                                
    // **************************************************************************
	CRect r;
	r = mr_area[i];  // ��Ʈ�� ���� ũ�� ����

	//Warning�ϰ�, Self Check�� �����.
//	if (i == dSELFCHECK || i == dINIT)	return;
	if (i == dINIT)	return;

	mn_pos_x[i][0] = r.left;
	mn_pos_x[i][1] = r.left + ((int)r.Width()-4);
	mn_pos_y[i][0] = r.top;
	mn_pos_y[i][1] = r.bottom-4;
	mn_y_diff[i] = (int) (mn_pos_y[i][1] - mn_pos_y[i][0])/7;
	mn_y_space[i] = (int) mn_y_diff[i]/6;
	// **************************************************************************

	// **************************************************************************
    // Ÿ�� ���� ��� �̹��� ���� ������ ���� ���� �����Ѵ�                      
    // **************************************************************************
	CRgn b1, b2, e1, e2;
	COLORREF cr;
	int x1=0, x2, y1=0, y2, w=mn_pos_x[i][1] - mn_pos_x[i][0];
	CPoint p;
	// **************************************************************************

	// **************************************************************************
    // Ÿ�� ���� ���� ���� �ձ� ���� ����� ���� ����Ͽ� ��Ʈ�ѿ� ����Ѵ�      
	// -> CreateEllipticRgn() : �־��� �簢�� �������� Ÿ�� ������ �����Ѵ�      
	//  : ���������� ������ �����Ǿ����� ������ �ڵ� ���� ����                   
	//  : ������ �������� �ʾ����� NULL�� ����                                   
    // **************************************************************************
	p.x = mn_pos_x[i][0];
	p.y = mn_pos_y[i][1] - mn_y_space[i] * 4;
	x2 = w;	
	y2 = mn_y_space[i] * 4;
	e1.CreateEllipticRgn(x1, y1, x2, y2);

	mp_btn_base[i].Create("", WS_CHILD|WS_VISIBLE, p, e1, this, IDC_LAMP_BASE+i, 1, RGB(150, 159, 183), RGB(150, 159, 183), RGB(150, 159, 183), RGB(150, 159, 183));
	mp_btn_base[i].EnableWindow(FALSE);

	e1.DeleteObject();
	// **************************************************************************

	// **************************************************************************
    // Ÿ�� ������ �ϴ� ���� �̹��� ��� ��ġ ����Ͽ� ��Ʈ�ѿ� ����Ѵ�         
	// -> CreateRectRgn() : �־��� ��ǥ�� �簢 ������ �׸���                     
	// -> OffsetRgn() : ������ �簢���� ��ǥ�� �̵��Ѵ�                          
	// -> CombineRgn() : �����ϴ� �� ���� �������� ���ļ� �� ��° ������ �����  
	//  : �� ��° ������ �ڵ��� �� �Լ��� ���̱� ���� �ݵ�� ��������߸�        
	//    ���ο� �������� ��ü�ȴ�                                               
	// -> GetRgnBox() : �־��� ������ ������ �ѷ��δ� ���� ���� ���簢�� �׸���  
    // **************************************************************************
	x2 = (int) w / 3;
	y2 = (int) mn_y_space[i] * 4 / 3;
	e1.CreateEllipticRgn(x1, y1, x2, y2);

	y2 = mn_y_diff[i] * 2 - mn_y_space[i] * 2;
	b1.CreateRectRgn(x1, y1, x2, y2);

	x2 = w;
	y2 = mn_y_space[i] * 4;
	e2.CreateEllipticRgn(x1, y1, x2, y2);

	e1.OffsetRgn(0, mn_y_diff[i]*2 - mn_y_space[i]*2 - (int)mn_y_space[i]*2/3);
	e2.OffsetRgn(-(int)w/3, -mn_y_space[i]);
	b1.CombineRgn(&e1, &b1, RGN_OR);
	b1.CombineRgn(&b1, &e2, RGN_DIFF);

	CRect rr;
	b1.GetRgnBox(&rr);

	p.y = rr.top;
	p.x = mn_pos_x[i][0] + (int) w/3;
	p.y += (mn_pos_y[i][1] - mn_y_diff[i] * 2);
	mp_btn_pole[i].Create("", WS_CHILD|WS_VISIBLE, p, b1, this, IDC_LAMP_POLE+i, 1, RGB(203, 185, 156), RGB(203, 185, 156), RGB(203, 185, 156), RGB(203, 185, 156));
	mp_btn_pole[i].EnableWindow(FALSE);

	e1.DeleteObject();
	e2.DeleteObject();
	b1.DeleteObject();
	// **************************************************************************

	// **************************************************************************
    // GREEN ���� �ؿ� �����ϴ� �ö�ƽ ���� ����Ͽ� ��Ʈ�ѿ� ����Ѵ�         
    // **************************************************************************
	x2 = w;
	y2 = mn_y_diff[i];
	b1.CreateRectRgn(x1, y1, x2, y2);

	y2 = mn_y_space[i] * 4;
	e1.CreateEllipticRgn(x1, y1, x2, y2);

	y2 = mn_y_space[i] * 4;
	e2.CreateEllipticRgn(x1, y1, x2, y2);

	e1.OffsetRgn(0, -mn_y_space[i]*2);
	b1.CombineRgn(&b1, &e1, RGN_DIFF);
	e2.OffsetRgn(0, mn_y_diff[i] - mn_y_space[i]*2);
	b1.CombineRgn(&b1, &e2, RGN_OR);

	p.x = mn_pos_x[i][0];
	p.y = mn_pos_y[i][1] - mn_y_diff[i] * 3 + mn_y_space[i];
	mp_btn_lower[i].Create("", WS_CHILD|WS_VISIBLE, p, b1, this, IDC_LAMP_LOWER+i, 1, RGB(217, 217, 217), RGB(217, 217, 217), RGB(217, 217, 217), RGB(217, 217, 217));
	mp_btn_lower[i].EnableWindow(FALSE);

	e1.OffsetRgn(0, mn_y_space[i]*2);
	e2.OffsetRgn(0, -(mn_y_diff[i] - mn_y_space[i]*2));

	b1.DeleteObject();
	// **************************************************************************

	// **************************************************************************
    // GREEN ���� �ٷ� �ؿ� �����ϴ� �׵θ� ���� ����Ͽ� ��Ʈ�ѿ� ����Ѵ�      
    // **************************************************************************
	y2 = mn_y_space[i];
	b1.CreateRectRgn(x1, y1, x2, y2);

	e1.OffsetRgn(0, -mn_y_space[i]);
	b1.CombineRgn(&b1, &e1, RGN_OR);
	e2.OffsetRgn(0, -mn_y_space[i]*2);
	b1.CombineRgn(&b1, &e2, RGN_DIFF);

	p.y -= mn_y_space[i];
	mp_btn_step1[i].Create("", WS_CHILD|WS_VISIBLE, p, b1, this, IDC_LAMP_SEP1+i, 1, RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0));
	mp_btn_step1[i].EnableWindow(FALSE);
	e1.OffsetRgn(0, mn_y_space[i]);
	// **************************************************************************

	// **************************************************************************
    // Ÿ�� GREEN ���� ��� ��ġ ����Ͽ� ��Ʈ�ѿ� ����Ѵ�                      
    // **************************************************************************
	y2 = mn_y_diff[i] - mn_y_space[i];
	b2.CreateRectRgn(x1, y1, x2, y2);

	e1.OffsetRgn(0, mn_y_diff[i] - mn_y_space[i]*3);
	b2.CombineRgn(&b2, &e1, RGN_OR);
	b2.CombineRgn(&b2, &e2, RGN_DIFF);

	CString title;  // ���� ���� ���� ����

	p.y -= (mn_y_diff[i] - mn_y_space[i]);
	if (mn_g_lamp[1][i]==0) 
	{
		cr = G_OFF;
		title = "OFF";
	}
	else if (mn_g_lamp[1][i]==1)
	{
		cr = G_ON;
		title = "ON";
	}
	else if (mn_g_lamp[1][i]==2)
	{
		cr = G_OFF;
		title = "Flicker";
	}

	mp_btn_g_lamp[i].Create(title, WS_CHILD|WS_VISIBLE, p, b2, this, ID_LAMP_GLAMP+i, 1, cr, G_ON, cr, cr);
	e1.OffsetRgn(0, -(mn_y_diff[i] - mn_y_space[i]*3));
	// **************************************************************************

	// **************************************************************************
    // GREEN ������ YELLOW ���� ������ �׵θ� ���� ����Ͽ� ��Ʈ�ѿ� ����Ѵ�    
    // **************************************************************************
	p.y -= mn_y_space[i];

	mp_btn_step2[i].Create("", WS_CHILD|WS_VISIBLE, p, b1, this, IDC_LAMP_SEP2+i, 1, RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0));
	mp_btn_step2[i].EnableWindow(FALSE);
	// **************************************************************************

	// **************************************************************************
    // Ÿ�� YELLOW ���� ��� ��ġ ����Ͽ� ��Ʈ�ѿ� ����Ѵ�                     
    // **************************************************************************
	p.y -= (mn_y_diff[i] - mn_y_space[i]);
	if (mn_y_lamp[1][i]==0) 
	{
		cr = Y_OFF;
		title = "OFF";
	}
	else if (mn_y_lamp[1][i]==1)
	{
		cr = Y_ON;
		title = "ON";
	}
	else if (mn_y_lamp[1][i]==2)
	{
		cr = Y_OFF;
		title = "Flicker";
	}

	mp_btn_y_lamp[i].Create(title, WS_CHILD|WS_VISIBLE, p, b2, this, ID_LAMP_YLAMP+i, 1, cr, Y_ON, cr, cr);
	// **************************************************************************

	// **************************************************************************
    // YELLOW ������ RED ���� ������ �׵θ� ���� ����Ͽ� ��Ʈ�ѿ� ����Ѵ�      
    // **************************************************************************
	p.y -= mn_y_space[i];

	 mp_btn_step3[i].Create("", WS_CHILD|WS_VISIBLE, p, b1, this, IDC_LAMP_SEP3+i, 1, RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0));
	 mp_btn_step3[i].EnableWindow(FALSE);
	// **************************************************************************

	// **************************************************************************
    // Ÿ�� RED ���� ��� ��ġ ����Ͽ� ��Ʈ�ѿ� ����Ѵ�                        
    // **************************************************************************
	p.y -= (mn_y_diff[i] - mn_y_space[i]);
	if (mn_r_lamp[1][i]==0) 
	{
		cr = R_OFF;
		title = "OFF";
	}
	else if (mn_r_lamp[1][i]==1)
	{
		cr = R_ON;
		title = "ON";
	}
	else if (mn_r_lamp[1][i]==2)
	{
		cr = R_OFF;
		title = "Flicker";
	}

	mp_btn_r_lamp[i].Create(title, WS_CHILD|WS_VISIBLE, p, b2, this, ID_LAMP_RLAMP+i, 1, cr, R_ON, cr, cr);
	// **************************************************************************

	// **************************************************************************
    // RED ���� ���� �����ϴ� �ö�ƽ ���� ����Ͽ� ��Ʈ�ѿ� ����Ѵ�           
    // **************************************************************************
	p.y -= (mn_y_diff[i]-mn_y_space[i]);

	mp_btn_upper[i].Create("", WS_CHILD|WS_VISIBLE, p, b2, this, IDC_LAMP_UPPER+i, 1, RGB(217, 217, 217), RGB(217, 217, 217), RGB(217, 217, 217), RGB(217, 217, 217));
	mp_btn_upper[i].EnableWindow(FALSE);
	// **************************************************************************

	// **************************************************************************
    // Ÿ�� ���� �� �� �ձ� �ö�ƽ ���� ����Ͽ� ��Ʈ�ѿ� ����Ѵ�             
    // **************************************************************************
	p.y -= mn_y_space[i]*2;

	mp_btn_roof[i].Create("", WS_CHILD|WS_VISIBLE, p, e1, this, IDC_LAMP_ROOF+i, 1, RGB(217, 217, 217), RGB(217, 217, 217), RGB(217, 217, 217), RGB(217, 217, 217));
	mp_btn_roof[i].EnableWindow(FALSE);
	// **************************************************************************
}

void CScreen_Set_Maintenance::OnMaintenance_Red_State_Change(int idx)
{
	//Wanring�ϰ�, Self Check�� �����.
//	if (idx == dSELFCHECK || idx == dINIT)	return;
	if (idx == dINIT)	return;

	int mn_red = mn_r_lamp[1][idx];  // RED ���� ����

	switch(mn_red)
	{
	case 0 :  // OFF
		mp_btn_r_lamp[idx].SetWindowText("OFF");
		mp_btn_r_lamp[idx].ChangeColor(R_OFF, R_ON, R_OFF, R_OFF);
		break;
	case 1 :  // ON
		mp_btn_r_lamp[idx].SetWindowText("ON");
		mp_btn_r_lamp[idx].ChangeColor(R_ON, R_ON, R_ON, R_ON);
		break;
	case 2 :  // FLICKER
		mp_btn_r_lamp[idx].SetWindowText("FLICK");
		mp_btn_r_lamp[idx].ChangeColor(FLICK, R_ON, FLICK, R_ON);
		break;
	default :
		break;
	}
}

void CScreen_Set_Maintenance::OnMaintenance_Yellow_State_Change(int idx)
{
	//Wanring�ϰ�, Self Check�� �����.
//	if (idx == dSELFCHECK || idx == dINIT)	return;
	if (idx == dINIT)	return;

	int mn_yellow = mn_y_lamp[1][idx];  // YELLOW ���� ����

	switch(mn_yellow)
	{
	case 0 :  // OFF
		mp_btn_y_lamp[idx].SetWindowText("OFF");
		mp_btn_y_lamp[idx].ChangeColor(Y_OFF, Y_ON, Y_OFF, Y_OFF);
		break;
	case 1 :  // ON
		mp_btn_y_lamp[idx].SetWindowText("ON");
		mp_btn_y_lamp[idx].ChangeColor(Y_ON, Y_ON, Y_ON, Y_ON);
		break;
	case 2 :  // FLICKER
		mp_btn_y_lamp[idx].SetWindowText("FLICK");
		mp_btn_y_lamp[idx].ChangeColor(FLICK, Y_ON, FLICK, Y_OFF);
		break;
	default :
		break;
	}
}

void CScreen_Set_Maintenance::OnMaintenance_Green_State_Change(int idx)
{
	//Wanring�ϰ�, Self Check�� �����.
//	if (idx == dSELFCHECK || idx == dINIT)	return;
	if (idx == dINIT)	return;

	int mn_green = mn_g_lamp[1][idx];  // GREEN ���� ����

	switch(mn_green)
	{
	case 0 :  // OFF
		mp_btn_g_lamp[idx].SetWindowText("OFF");
		mp_btn_g_lamp[idx].ChangeColor(G_OFF, G_ON, G_OFF, G_OFF);
		break;
	case 1 :  // ON
		mp_btn_g_lamp[idx].SetWindowText("ON");
		mp_btn_g_lamp[idx].ChangeColor(G_ON, G_ON, G_ON, G_ON);
		break;
	case 2 :  // FLICKER
		mp_btn_g_lamp[idx].SetWindowText("FLICK");
		mp_btn_g_lamp[idx].ChangeColor(FLICK, G_ON, FLICK, G_OFF);
		break;
	default :
		break;
	}
}

void CScreen_Set_Maintenance::OnMaintenance_Data_Set()
{
	int i;
	
	for (i = 0; i <= SELFCHECK; i++)
	{
		mn_r_lamp[1][i] = stLAMP.nLampR[i];
		mn_y_lamp[1][i] = stLAMP.nLampY[i];
		mn_g_lamp[1][i] = stLAMP.nLampG[i];
	}

	mn_buzzer_mode[1] = stLAMP.nModeBuzzer;

	mn_lamp_flicker_time[1] = stLAMP.nTwinkle;
	mn_lamp_change_time[1] = st_lamp.mn_lamp_change_time;

	md_pitch_data[0][1] = st_accy.d_X_Gripper_Size;
	md_pitch_data[1][1] = st_accy.d_Y_Gripper_Size;
	md_pitch_data[2][1] = st_accy.d_Y_BufferConv_Size;
//	mb_first = TRUE;
}

void CScreen_Set_Maintenance::OnMaintenance_Button_Set()
{
	short	shBtnColor = 30;

	// *************************************************************
	//  CHECK BUTTON		                                        
	// *************************************************************
	m_chk_buzzer.SetFont(mp_maintenance_font);
	m_chk_buzzer.SetIcon(IDI_SOUND_ON, IDI_SOUND_OFF);
	m_chk_buzzer.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_chk_buzzer.SetButtonColor(1);
}

void CScreen_Set_Maintenance::OnMaintenance_Buzzer_Change()
{
	if (mn_buzzer_mode[1] < 0 || mn_buzzer_mode[1] > 1)	mn_buzzer_mode[1] = TRUE;

	m_chk_buzzer.SetCheck(mn_buzzer_mode[1]);
	
	if (mn_buzzer_mode[1] == TRUE)	SetDlgItemText(IDC_CHK_BUZZER, "Buzzer On");
	else							SetDlgItemText(IDC_CHK_BUZZER, "Buzzer Off");
}

void CScreen_Set_Maintenance::OnMaintenance_EditBox_Set()
{	
}

void CScreen_Set_Maintenance::OnMaintenance_Data_Apply()
{
	int i;

	for(i = 0; i <= SELFCHECK; i++)
	{
		// **********************************************************************
		// RED Ÿ�� ���� ���� ���� ������ �����Ѵ�                               
		// **********************************************************************
		if (mn_r_lamp[1][i]<0 || mn_r_lamp[1][i]>2) 
		{
			mn_r_lamp[1][i] = 0;
			OnMaintenance_Red_State_Change(i);     // RED Ÿ�� ���� ��ư ���� ��ȯ �Լ�
		}
		stLAMP.nLampR[i] = mn_r_lamp[1][i];
		// **********************************************************************

		// **********************************************************************
		// YELLOW Ÿ�� ���� ���� ���� ������ �����Ѵ�                            
		// **********************************************************************
		if (mn_y_lamp[1][i]<0 || mn_y_lamp[1][i]>2) 
		{
			mn_y_lamp[1][i] = 0;
			OnMaintenance_Yellow_State_Change(i);  // YELLOW Ÿ�� ���� ��ư ���� ��ȯ �Լ�
		}
		stLAMP.nLampY[i] = mn_y_lamp[1][i];
		// **********************************************************************

		// **********************************************************************
		// GREEN Ÿ�� ���� ���� ���� ������ �����Ѵ�                             
		// **********************************************************************
		if (mn_g_lamp[1][i]<0 || mn_g_lamp[1][i]>2) 
		{
			mn_g_lamp[1][i] = 0;
			OnMaintenance_Green_State_Change(i);   // GREEN Ÿ�� ���� ��ư ���� ��ȯ �Լ�
		}
		stLAMP.nLampG[i] = mn_g_lamp[1][i];
		// **********************************************************************
	}

	stLAMP.nModeBuzzer = mn_buzzer_mode[1];

	stLAMP.nTwinkle = mn_lamp_flicker_time[1];
	st_lamp.mn_lamp_change_time = mn_lamp_change_time[1];

	st_accy.d_X_Gripper_Size = md_pitch_data[0][1];
	st_accy.d_Y_Gripper_Size = md_pitch_data[1][1];
	st_accy.d_Y_BufferConv_Size = md_pitch_data[2][1];

	if (st_handler.cwnd_list != NULL)
	{
		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, MACHINE_INFO);
	}
}

void CScreen_Set_Maintenance::OnMaintenance_Data_BackUp()
{
	int i;

	for(i=0; i<=SELFCHECK; i++)
	{
		// **********************************************************************
		// RED Ÿ�� ���� ���¸� ����Ѵ�                                         
		// **********************************************************************
		mn_r_lamp[0][i] = mn_r_lamp[1][i];
		// **********************************************************************

		// **********************************************************************
		// YELLOW Ÿ�� ���� ���¸� ����Ѵ�                                      
		// **********************************************************************
		mn_y_lamp[0][i] = mn_y_lamp[1][i];
		// **********************************************************************

		// **********************************************************************
		// GREEN Ÿ�� ���� ���¸� ����Ѵ�                                       
		// **********************************************************************
		mn_g_lamp[0][i] = mn_g_lamp[1][i];
		// **********************************************************************
	}

	mn_buzzer_mode[0] = mn_buzzer_mode[1];

	mn_lamp_flicker_time[0] = mn_lamp_flicker_time[1];
	mn_lamp_change_time[0] = mn_lamp_change_time[1];

	for (i = 0; i < 3; i++)
	{
		md_pitch_data[i][0]	= md_pitch_data[i][1];
	}
}

void CScreen_Set_Maintenance::OnMaintenance_ListBar_Info_Send()
{
	if (st_handler.cwnd_list != NULL)
	{
		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, MACHINE_INFO);
	}
}

void CScreen_Set_Maintenance::OnMaintenance_Data_Display()
{
	m_dgt_flicker_time.SetValue(mn_lamp_flicker_time[1]);
	m_dgt_change_time.SetValue(mn_lamp_change_time[1]);
}

int CScreen_Set_Maintenance::OnMaintenance_Data_Comparison()
{
	int i;
	int Ret = RET_GOOD;

	for(i=0; i<=SELFCHECK; i++)
	{
		// **********************************************************************
		// RED Ÿ�� ���� ���¸� Ȯ���Ѵ�                                         
		// **********************************************************************
		if (mn_r_lamp[0][i] != mn_r_lamp[1][i])					Ret = RET_ERROR;
		// **********************************************************************

		// **********************************************************************
		// YELLOW Ÿ�� ���� ���¸� Ȯ���Ѵ�                                      
		// **********************************************************************
		if (mn_y_lamp[0][i] != mn_y_lamp[1][i])					Ret = RET_ERROR;
		// **********************************************************************

		// **********************************************************************
		// GREEN Ÿ�� ���� ���¸� Ȯ���Ѵ�                                       
		// **********************************************************************
		if (mn_g_lamp[0][i] != mn_g_lamp[1][i])					Ret = RET_ERROR;
		// **********************************************************************
	}

	if (mn_buzzer_mode[0] != mn_buzzer_mode[1])					Ret = RET_ERROR;
	if (mn_lamp_flicker_time[0] != mn_lamp_flicker_time[1])		Ret = RET_ERROR;
	if (mn_lamp_change_time[0] != mn_lamp_change_time[1])		Ret = RET_ERROR;

	return Ret;
}

void CScreen_Set_Maintenance::OnMaintenance_Data_Recovery()
{
	int i;

	for (i = 0; i <= SELFCHECK; i++)
	{
		// **********************************************************************
		// RED Ÿ�� ���� ���¸� �����Ѵ�                                         
		// **********************************************************************
		mn_r_lamp[1][i] = mn_r_lamp[0][i];
		// **********************************************************************

		// **********************************************************************
		// YELLOW Ÿ�� ���� ���¸� �����Ѵ�                                      
		// **********************************************************************
		mn_y_lamp[1][i] = mn_y_lamp[0][i];
		// **********************************************************************

		// **********************************************************************
		// GREEN Ÿ�� ���� ���¸� �����Ѵ�                                       
		// **********************************************************************
		mn_g_lamp[1][i] = mn_g_lamp[0][i];
		// **********************************************************************
	}

	mn_buzzer_mode[1] = mn_buzzer_mode[0];

	mn_lamp_flicker_time[1] = mn_lamp_flicker_time[0];
	mn_lamp_change_time[1] = mn_lamp_change_time[0];

	for (i = 0; i < 4; i++)
	{
		md_pitch_data[i][1]	= md_pitch_data[i][0];
	}
}

void CScreen_Set_Maintenance::OnMaintenance_Label_Set()
{

}

void CScreen_Set_Maintenance::OnMaintenance_Digital_Count_Set()
{
	m_dgt_flicker_time.SubclassDlgItem(IDC_DGT_FLICKER_TIME, this);
	m_dgt_flicker_time.SetStyle(IDB_BIG3, 5);

	m_dgt_change_time.SubclassDlgItem(IDC_DGT_CHANGE_TIME, this);
	m_dgt_change_time.SetStyle(IDB_BIG3, 5);
}

void CScreen_Set_Maintenance::OnMaintenance_Lamp_Change()
{
	int i;

	for(i = 0; i <= SELFCHECK; i++)
	{
		OnMaintenance_Red_State_Change(i);      // RED Ÿ�� ���� ��ư ���� ��ȯ �Լ�
		OnMaintenance_Yellow_State_Change(i);   // YELLOW Ÿ�� ���� ��ư ���� ��ȯ �Լ�
		OnMaintenance_Green_State_Change(i);    // GREEN Ÿ�� ���� ��ư ���� ��ȯ �Լ�
	}
}

void CScreen_Set_Maintenance::OnMaintenance_File_Backup(CString strTargetFile, CString strCreateFile, CString strBackupFolder, CString strBackupFile, BOOL b_over_write)
{
	CString str_backup_file;	// ������ ��� ������ [���+���ϸ�+Ȯ����] ���� ����
	int n_existence;			// ���� ���� ���� �÷���
	int n_response;				// ��ȭ ���� ���� �÷���
	
	char chr_file[256];
	char szDrive[4] = {'\0',};
	char szDir[MAX_PATH] = {'\0',};
	char szName[MAX_PATH] = {'\0',};
	char szExt[MAX_PATH] = {'\0',};

	CDialog_Select  select_dlg;

	// **************************************************************************
	// ����� ������ �̹� �����ϴ��� �˻��Ѵ�                                    
	// -> �̹� �����ϴ� ��쿡�� ����ڿ��� ����� ���� ���� �ȴ�              
	// **************************************************************************
	sprintf(chr_file, "%s", strCreateFile);

	n_existence = access(chr_file,0) ;

	if (n_existence != -1)  // ���� ����
	{
		st_msg.mstr_confirm_msg = _T("Backup File Exist~! Overwrite??");

		n_response = select_dlg.DoModal();
		if (n_response == IDCANCEL)
			return ; 
	}
	// **************************************************************************

	// **************************************************************************
	// ��� ���� �����Ѵ�                                                        
	// **************************************************************************
	_tsplitpath(strTargetFile, szDrive, szDir, szName, szExt); 
	str_backup_file.Format("%s\\%s%s", strBackupFolder, strBackupFile, szExt);
	
	CopyFile(strTargetFile, str_backup_file, !b_over_write);
	// **************************************************************************
}

// ******************************************************************************
//  Tower Lamp ȭ�� ���� ���� ��û �޽���                                        
// ******************************************************************************
LRESULT CScreen_Set_Maintenance::OnMaintenance_Info_Apply(WPARAM wParam, LPARAM lParam)  
{
	OnMaintenance_Data_Apply();			// Tower Lamp ȭ�� ���� ���� ���� ������ ���� �Լ�

	OnMaintenance_LogFile_Create();

	OnMaintenance_Data_BackUp();

	OnMaintenance_ListBar_Info_Send();   // Ÿ��Ʋ �� ȭ�鿡 ���� ��� ��û�ϴ� �Լ�

	return 0;
}

void CScreen_Set_Maintenance::OnRLampClick(UINT nID)
{
	int idx = nID - ID_LAMP_RLAMP;		// �迭 ��ġ ����

	mn_r_lamp[1][idx]++;
	if (mn_r_lamp[1][idx]<0 || mn_r_lamp[1][idx]>2)
		mn_r_lamp[1][idx] = 0;

	OnMaintenance_Red_State_Change(idx);		// RED Ÿ�� ���� ��ư ���� ��ȯ �Լ�
}

void CScreen_Set_Maintenance::OnYLampClick(UINT nID)
{
	int idx = nID - ID_LAMP_YLAMP;		// �迭 ��ġ ����
	
	mn_y_lamp[1][idx]++;
	if (mn_y_lamp[1][idx]<0 || mn_y_lamp[1][idx]>2)
		mn_y_lamp[1][idx] = 0;

	OnMaintenance_Yellow_State_Change(idx);	// YELLOW Ÿ�� ���� ��ư ���� ��ȯ �Լ�
}

void CScreen_Set_Maintenance::OnGLampClick(UINT nID)
{
	int idx = nID - ID_LAMP_GLAMP;		// �迭 ��ġ ����
	
	mn_g_lamp[1][idx]++;
	if (mn_g_lamp[1][idx]<0 || mn_g_lamp[1][idx]>2)
		mn_g_lamp[1][idx] = 0;

	OnMaintenance_Green_State_Change(idx);		// GREEN Ÿ�� ���� ��ư ���� ��ȯ �Լ�
}

void CScreen_Set_Maintenance::OnDgtFlickerTime() 
{
	CString mstr_temp;  // ������ ���� �ӽ� ���� ���� 
	char chr_buf[20] ;

	mn_lamp_flicker_time[1] = m_dgt_flicker_time.GetValue();
	mstr_temp = LPCTSTR(_itoa(mn_lamp_flicker_time[1], chr_buf, 10));

	st_msg.mstr_keypad_msg = _T("Lamp Flicker Time Setting");
    
	st_msg.mstr_keypad_val = mstr_temp;

	mstr_temp = KeyPad.GetNumEditString_I(10, 10000, atoi(st_msg.mstr_keypad_val), st_msg.mstr_keypad_msg);

	mn_lamp_flicker_time[1] = atoi(mstr_temp);
	m_dgt_flicker_time.SetValue(mn_lamp_flicker_time[1]);
}

void CScreen_Set_Maintenance::OnChkBuzzer() 
{
	if (m_chk_buzzer.GetCheck() == TRUE)	mn_buzzer_mode[1] = TRUE;
	else									mn_buzzer_mode[1] = FALSE;

	OnMaintenance_Buzzer_Change();
}

void CScreen_Set_Maintenance::OnBtnDataBackup() 
{
	CString str_year, str_month, str_day;	// ������ ��, ��, �� ���� ����
	CString str_back_day, str_back_time;
	CString str_back_file;					// ��� ���ϸ� ���� ����
	CString str_back_info;					// ����� ���� ���� ���� ���� [����+���ϸ�+Ȯ����]
	CString str_temp_file;					// ���ϸ� �ӽ� ���� ����
	CString str_chk_ext;					// ���� Ȯ���� ���� ����
	CString str_device;
	int n_response;							// ��ȭ ���� ���� �÷���
	int n_year, n_month, n_day;				// ��, ��, �� ���� ����
	CString str_cur_hour, str_cur_min, str_cur_sec;
	
	char chr_buf[20];

	CDialog_Select   select_dlg;
	CDialog_Message  msg_dlg;

	st_msg.mstr_confirm_msg = _T("Do you back up environment Setting information?");

	n_response = select_dlg.DoModal();

	if (n_response == IDOK)
	{
		// ******************************************************************************
		// ��� ���ϸ����� ����� ��¥ ���� �����Ѵ�                                     
		// ******************************************************************************
		COleDateTime cur_time = COleDateTime::GetCurrentTime();
		n_year = cur_time.GetYear();  
		n_month = cur_time.GetMonth();  
		n_day = cur_time.GetDay();  

		str_year = LPCTSTR(_itoa( n_year, chr_buf, 10 ));  
		str_month.Format("%02d", n_month);
		str_day.Format("%02d", n_day);

		str_back_day = str_year;
		str_back_day += str_month;
		str_back_day += str_day;
		// ******************************************************************************

		// **************************************************************************
		// ���� �ð� ���� ��´�                                                     
		// **************************************************************************
		str_cur_hour.Format("%02d",cur_time.GetHour());	// ���� �ð� ������ �����Ѵ�.
		str_cur_min.Format("%02d",cur_time.GetMinute());	// ���� �� ������ �����Ѵ�. 
		str_cur_sec.Format("%02d",cur_time.GetSecond());	// ���� �� ������ �����Ѵ�. 

		str_back_time = str_cur_hour;
		str_back_time += str_cur_min;
		str_back_time += str_cur_sec;
		// **************************************************************************

		// ******************************************************************************
		// ������ ���� ���� ���� ���� ����Ѵ�                                           
		// -> ����� ���� ������ �����ϴ��� �˻��ϰ� �ȴ�                                
		// ******************************************************************************
		str_back_file = "c:\\Backup\\" + str_back_day + "_" + str_back_time;  // ��� ���ϸ� ���� [��¥+���ϸ�]

		CopyFolder(st_path.mstr_path_dvc, str_back_file);
		CopyFolder(st_path.mstr_basic_folder, str_back_file);
	}
}

void CScreen_Set_Maintenance::OnBtnPasswordChangeLevle1() 
{
	int n_response;  // ��ȭ ���� ���� �÷���

	CDialog_Pass_Change  pass_cng_dlg;

	st_handler.mstr_pass_level=_T("Level1");  // ��ȣ ���� : ����Ʈ ����
	n_response = pass_cng_dlg.DoModal();
	
	if (n_response == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (n_response == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}
}

void CScreen_Set_Maintenance::OnBtnPasswordChangeLevle2() 
{
	int n_response;  // ��ȭ ���� ���� �÷���

	CDialog_Pass_Change  pass_cng_dlg;

	st_handler.mstr_pass_level=_T("Level2");  // ��ȣ ���� : ƼĪ ����
	n_response = pass_cng_dlg.DoModal();
	
	if (n_response == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (n_response == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}
}

void CScreen_Set_Maintenance::OnBtnMaintenanceApply() 
{
	int n_response;		// ��ȭ ���� ���� �÷���

	CDialog_Select  select_dlg;

	st_msg.mstr_confirm_msg = _T("Maintenance : Setting Data Apply?");

	n_response = select_dlg.DoModal();

	if (n_response == IDOK)
	{
		OnMaintenance_Data_Apply();			// Tower Lamp ȭ�� ���� ���� ���� ������ ���� �Լ�

		OnMaintenance_LogFile_Create();

		OnMaintenance_Data_BackUp();

		OnMaintenance_ListBar_Info_Send();	// ����Ʈ �� ȭ�鿡 ���� ��� ��û�ϴ� �Լ�
	}
	else if (n_response == IDCANCEL)
	{

	}
}

void CScreen_Set_Maintenance::OnMaintenance_LogFile_Create()
{
	int i;
	CString str_msg;
	CString str_data;
	CString str_mode;
	int Ret = RET_GOOD;

	for(i=0; i<=SELFCHECK; i++)
	{
		if (i == 0)			str_mode = "STOP";
		else if (i == 1)	str_mode = "RUN";
		else if (i == 2)	str_mode = "JAM";
		else if (i == 3)	str_mode = "LOT END";
		else if (i == 4)	str_mode = "INIT";
		else if (i == 5)	str_mode = "WARNING";
		else if (i == 6)	str_mode = "LOCK";
//		else if (i == 7)	str_mode = "SELF CHECK";
		else if (i == 7)	str_mode = "EMPTY RUN";
		
		// **********************************************************************
		// RED Ÿ�� ���� ���¸� Ȯ���Ѵ�                                         
		// **********************************************************************
		if (mn_r_lamp[0][i] != mn_r_lamp[1][i])
		{
			if (mn_r_lamp[1][i] == 0)		str_data = "OFF";
			else if (mn_r_lamp[1][i] == 1)	str_data = "ON";
			else if (mn_r_lamp[1][i] == 2)	str_data = "FLICKER";

			str_msg.Format("[Tower Lamp] %s Status : RED LAMP was changed by %s", str_mode, str_data);
			cLOG.OnLogEvent( LOG_PGM_, str_msg );
		}
		// **********************************************************************

		// **********************************************************************
		// YELLOW Ÿ�� ���� ���¸� Ȯ���Ѵ�                                      
		// **********************************************************************
		if (mn_y_lamp[0][i] != mn_y_lamp[1][i])
		{
			if (mn_y_lamp[1][i] == 0)		str_data = "OFF";
			else if (mn_y_lamp[1][i] == 1)	str_data = "ON";
			else if (mn_y_lamp[1][i] == 2)	str_data = "FLICKER";

			str_msg.Format("[Tower Lamp] %s Status : YELLOW LAMP was changed by %s", str_mode, str_data);
			cLOG.OnLogEvent( LOG_PGM_, str_msg );
		}
		// **********************************************************************

		// **********************************************************************
		// GREEN Ÿ�� ���� ���¸� Ȯ���Ѵ�                                       
		// **********************************************************************
		if (mn_g_lamp[0][i] != mn_g_lamp[1][i])
		{
			if (mn_g_lamp[1][i] == 0)		str_data = "OFF";
			else if (mn_g_lamp[1][i] == 1)	str_data = "ON";
			else if (mn_g_lamp[1][i] == 2)	str_data = "FLICKER";

			str_msg.Format("[Tower Lamp] %s Status : GREEN LAMP was changed by %s", str_mode, str_data);
			cLOG.OnLogEvent( LOG_PGM_, str_msg );
		}
		// **********************************************************************
	}

	if (mn_buzzer_mode[0] != mn_buzzer_mode[1])
	{
		if (mn_buzzer_mode[1] == 0)
		{
			str_data = _T("On");
		}
		else
		{
			str_data = _T("Off");
		}
		
		str_msg = "[TowerLamp] Buzzer Mode was changed by " + str_data;
		cLOG.OnLogEvent( LOG_PGM_, str_msg );
	}

	if (mn_lamp_flicker_time[0] != mn_lamp_flicker_time[1])
	{
		str_data.Format("%d", (int)mn_lamp_flicker_time[1]);
		str_msg = "[TowerLamp] Lamp Flicker Time was changed by " + str_data + "(msec)";
		cLOG.OnLogEvent( LOG_PGM_, str_msg );
	}

	if (mn_lamp_change_time[0] != mn_lamp_change_time[1])
	{
		str_data.Format("%d", (int)mn_lamp_change_time[1]);
		str_msg = "[TowerLamp] Lamp Change Time was changed by " + str_data + "(Sec)";
		cLOG.OnLogEvent( LOG_PGM_, str_msg );
	}
}

void CScreen_Set_Maintenance::OnDgtChangeTime() 
{
	CString mstr_temp;  // ������ ���� �ӽ� ���� ���� 
	char chr_buf[20] ;

	mn_lamp_change_time[1] = m_dgt_change_time.GetValue();
	mstr_temp = LPCTSTR(_itoa(mn_lamp_change_time[1], chr_buf, 10));

	st_msg.mstr_keypad_msg = _T("Lamp Change Time Setting");
    
	st_msg.mstr_keypad_val = mstr_temp;

	mstr_temp = KeyPad.GetNumEditString_I(0, 100, atoi(st_msg.mstr_keypad_val), st_msg.mstr_keypad_msg);
	mn_lamp_change_time[1] = atoi(mstr_temp);
	m_dgt_change_time.SetValue(mn_lamp_change_time[1]);
}

void CScreen_Set_Maintenance::OnBtnStepSave() 
{
	char path[MAX_PATH] = {0};
	//������ �۾���θ� ���� ���� �Ѵ�.
	GetCurrentDirectory(MAX_PATH, path);
	
	CFileDialogST	dlg(FALSE, NULL, "Step_back", OFN_FILEMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST, _T("Text File(*.txt)\0*.txt\0"), this);
	CString			sPathName, str_temp_file, str_ext, str_chk_ext;
	int				nRetValue, n_pos;
	
	BOOL			bResult; // = find.FindFile(str_file);
	
	CFileFind find;
	
	nRetValue = dlg.DoModal();
	
	if (nRetValue == IDOK)
	{
		sPathName = dlg.GetPathName();   // ȭ�鿡 �������� ������ ������ ��� �� ������ �̸��� �����!!
		
		n_pos = sPathName.Find(".txt");
		if(n_pos == -1)
			n_pos = sPathName.Find(".TXT");
		if(n_pos == -1)
			sPathName += ".txt";
		
		
		bResult = find.FindFile(sPathName);
		find.Close();
		
		if(bResult != 0)
		{
			DeleteFile(sPathName);
		}
		
		(st_msg.str_step_savefile_name).Format("%s", dlg.GetFileName());
		(st_msg.str_step_savefile_path).Format("%s", dlg.GetFileDir());

		OnMaintenance_Step_Save();
		
		SetCurrentDirectory(path);						// �۾� ��θ� ���� ��η� ���� �Ѵ�.
	}
	else if (nRetValue == IDCANCEL)
	{
		
	}
}

void CScreen_Set_Maintenance::OnMaintenance_Step_Save()
{
	CString mstr_file_name;    // ���������� ������ ���� �̸� ���� ���� 
	CString mstr_create_file;  // �˶� ���� ������ ���Ͽ� ���� [����]+[���ϸ�]+[Ȯ����] ���� ���� 
	CString mstr_list_name, mstr_temp_data;  // �� �κк� �˶� �߻� Ƚ�� ���� ������ �α� ���� ���� ���� ���� 
	CString mstr_content;  // �� ���ο� ����� ���ڿ� ���� ���� ���� 
	int mn_existence, i;      // ���� ���� ���� ���� �÷��� 
	char fileName[256];    // �˻��� ���� ���� ���� �Լ� 
	FILE  *fp ;            // ���Ͽ� ���� ������ ���� ���� 
	
	CString str_title_time, str_file_time, str_new_file;		// ���Ͽ� ����� Ÿ��Ʋ �ð� ���� ����
	int n_cur_year, n_cur_month, n_cur_day;		// ��, ��, �� ���� ����
	int n_cur_hour, n_cur_minute, n_cur_second;		// ��, ��, �� ���� ����
	
	COleDateTime mtime_cur;						// ���� �ð� ���� ����
	CTime mp_time_cur;							// Ÿ��Ʋ �ð� ���� ����

	CString str_temp_data;
	int n_length;

	CString strData, str_time;
	
	/* ************************************************************************** */
	/* ���� �̸����� ����� ��¥ �����Ѵ�                                         */
	/* ************************************************************************** */
	mtime_cur = COleDateTime::GetCurrentTime();
	
	n_cur_year = mtime_cur.GetYear();  
	n_cur_month = mtime_cur.GetMonth();  
    n_cur_day = mtime_cur.GetDay();  

	n_cur_hour = mtime_cur.GetHour();
	n_cur_minute = mtime_cur.GetMinute();
	n_cur_second = mtime_cur.GetSecond();
	
	str_new_file.Format("DATA%04d%02d%02d", n_cur_year, n_cur_month, n_cur_day);
	str_time.Format("%04d/%02d/%02d %02d:%02d:%02d", n_cur_year, n_cur_month, n_cur_day, n_cur_hour, n_cur_minute, n_cur_second);
		
	mstr_create_file = st_msg.str_step_savefile_path + st_msg.str_step_savefile_name;
	mstr_create_file += ".TXT";
	
	sprintf(fileName, "%s", mstr_create_file);
	mn_existence = access(fileName,0);
	
	if (mn_existence == -1)  /* ���� ������ */
	{
		mstr_create_file = st_msg.str_step_savefile_path + st_msg.str_step_savefile_name;
		mstr_create_file += ".TXT";
		/* ********************************************************************** */
	}
	
	/* ************************************************************************** */
	/* �˶� �߻� Ƚ�� ���� ���� ���Ͽ� �߰� ������ ���� ���Ϸ� ����               */
	/* ************************************************************************** */
	if ((fp=fopen(mstr_create_file,"a+"))==NULL)  
	{
		if (st_handler.mn_language == LANGUAGE_ENGLISH)
		{
			Func.MsgLog("File Open Fail!.");	// 20130926 kjh english
		}
		else
		{
			Func.MsgLog("���� ���� ����!.");
		}
		return;
	}
	/* ************************************************************************** */

	/* ************************************************************************** */
	/* ���Ͽ� Ÿ��Ʋ ���� ����Ѵ�                                                */
	/* ************************************************************************** */
	mstr_content = "********************************************************************************************************";
	fprintf(fp,"%s\r\n", mstr_content) ;
	
	mstr_content.Format("| %-100s |", "AMT8570A [SSD Final Sorter] : " + st_handler.mstr_handler_date);
	fprintf(fp,"%s\r\n", mstr_content) ;
	
	mstr_content.Format("| %-100s |", "Thread Step Info File");
	fprintf(fp,"%s\r\n", mstr_content) ;
	
	mstr_content.Format("| %-100s |", "DIVISION  : AMT Coporation");
	fprintf(fp,"%s\n", mstr_content) ;

	mstr_content.Format("| %-100s |", "File Name : " + st_msg.str_step_savefile_path + st_msg.str_step_savefile_name);
	fprintf(fp,"%s\r\n", mstr_content) ;

	mstr_content.Format("| %-100s |", "Save Time : " + str_time);
	fprintf(fp,"%s\r\n", mstr_content) ;

	mstr_content = "********************************************************************************************************";
	fprintf(fp,"%s\r\n", mstr_content) ;
	
	mstr_content = "+==========================+=============================================================================================================================+";
	fprintf(fp,"%s\r\n",mstr_content);
	
	mstr_content = "|  Step Trace              |                                                    History                                                                  |";
	fprintf(fp,"%s\n", mstr_content );
	
	mstr_content = "+==========================+=============================================================================================================================+";
	fprintf(fp,"%s\r\n",mstr_content) ;
	
	str_temp_data = "";
	
	for (i = 0; i < 87; i++)
	{
		str_temp_data = st_work.mstr_last_change_date[i];
		mstr_content.Format("|  Step %02d Change LastDate |", i);
		Func.TextAndLine(130, mstr_content, str_temp_data, fp);
		
		if (st_work.mstr_history_step_back_up[i] != "")
		{
			str_temp_data = st_work.mstr_history_step_back_up[i];
			n_length = str_temp_data.GetLength();
			mstr_content.Format("|  Step %02d before History  |", i);
			mstr_content += str_temp_data;
			fprintf(fp,"%s\r\n", mstr_content);
			mstr_content =      "+--------------------------+-----------------------------------------------------------------------------------------------------------------------------+";
			fprintf(fp,"%s\r\n",mstr_content);
		}
		str_temp_data = st_work.mstr_history_step[i];
		n_length = str_temp_data.GetLength();
		mstr_content.Format("|  Step %02d History         |", i);

		if (str_temp_data == "")
		{
			strData.Format("%-124s |", "");
			mstr_content += strData;
			fprintf(fp,"%s\r\n", mstr_content) ;
		}
		else
		{
			mstr_content += str_temp_data;
			fprintf(fp,"%s\r\n", mstr_content);
		}
		mstr_content =      "+--------------------------+-----------------------------------------------------------------------------------------------------------------------------+";
		fprintf(fp,"%s\r\n",mstr_content);
	}

	mstr_content =      "+--------------------------+-----------------------------------------------------------------------------------------------------------------------------+";	
	fprintf(fp,"%s\r\n",mstr_content);
	
	if (ferror(fp))  
	{
		if (st_handler.mn_language == LANGUAGE_ENGLISH)
		{
			Func.MsgLog("File Open Fail!.");	// 20130926 kjh english
		}
		else
		{
			Func.MsgLog("���� ���� ����!.");
		}
		return ;
	}
	
	fclose(fp);  /* ���� ���� */
}

BOOL CScreen_Set_Maintenance::CopyFolder(CString &strFrom, CString &strTo)
{	
	CFileFind finder;
	
    CString sWildCard = strFrom + "\\*.*";
	
    BOOL bFound;
    BOOL bWorking = finder.FindFile(sWildCard);
	
    bFound = bWorking;
	
    ::CreateDirectory(strTo, NULL);
	
    while(bWorking)
    {
        bWorking = finder.FindNextFile();
		
        // skip . and .. files; otherwise, we"d
        // recur infinitely!
        if(finder.IsDots()) continue;
		
        // ���丮�̸� ��� ȣ��
        if(finder.IsDirectory())
        {
            CString sFrom2 = finder.GetFilePath();
            CString sTo2 = strTo + "\\" + finder.GetFileName();
            ::CreateDirectory(sTo2, NULL);
            
            CopyFolder(sFrom2, sTo2);
            continue;
        }
        ExCopyFile(finder.GetFilePath(), strTo);
    }
    return bFound;

}

int CScreen_Set_Maintenance::ExCopyFile(CString strFrom, CString strTo)
{
	SHFILEOPSTRUCT shfo;
	
    char pszFrom[1024] = {0};
    char pszTo[1024] = {0};
	
    WORD wFlag = 0;
	
    strcpy(pszFrom, (LPCTSTR)strFrom);
    strcpy(pszTo, (LPCTSTR)strTo);
	
    pszFrom[lstrlen(pszFrom) + 1] = 0;
	
    shfo.hwnd = m_hWnd;
    shfo.wFunc = FO_COPY;
    shfo.pFrom = pszFrom;
    shfo.pTo = pszTo;
    shfo.fFlags = 0;
    shfo.lpszProgressTitle = "File Copy..";
	
    return SHFileOperation(&shfo);

}

void CScreen_Set_Maintenance::GridColor(UINT nID, int row, int col, COLORREF bk, COLORREF tk)
{
	TSpread *Grid = (TSpread*)GetDlgItem(nID);
	Grid->SetColor(col, row, bk, tk);
	Grid->SetBackColorStyle(SS_BACKCOLORSTYLE_UNDERGRID);
	
	// ���̸� �߰� [2015.02.16]
	if (Grid != NULL)
	{
		Grid = NULL;
		delete Grid;
	}
}

void CScreen_Set_Maintenance::GridControl(UINT nID, int type, int row, int col, int pos)
{
	TSpread *Grid = (TSpread*)GetDlgItem(nID);
	
	SS_CELLTYPE CellType;
	SS_DATEFORMAT dateFormat = { TRUE, '/', IDF_DDMMYY, FALSE };
	SS_TIMEFORMAT tmFormat;
	CString strList;
	strList = "combo1\tcombo2\tcombo3\tcombo4\tcombo5\tcombo6";
	double a = 0.5;

	switch (type)
	{
		case DATE:			
			Grid->SetTypeDate(&CellType, 0 , &dateFormat, NULL, NULL);
			break;

		case EDIT:
			Grid->SetTypeEdit(&CellType,ES_CENTER,60,SS_CHRSET_CHR,SS_CASE_NOCASE);
			break;

		case COMBO:
			// SetValue(col, row, data); data = ""0"(1����) data = "1"(2����) data = "2"(3����)
			Grid->SetTypeComboBox(&CellType, 0, strList);
			break;

		case BUTTON:
			if (pos == 0)
			{
				Grid->SetTypeButton(&CellType, SBS_DEFPUSHBUTTON, "Go", NULL, SUPERBTN_PICT_NONE,
					NULL, SUPERBTN_PICT_NONE, SUPERBTN_TYPE_NORMAL, 2, NULL);
			}
			else
			{
				Grid->SetTypeButton(&CellType, SBS_DEFPUSHBUTTON, "Read", NULL, SUPERBTN_PICT_NONE,
					NULL, SUPERBTN_PICT_NONE,SUPERBTN_TYPE_NORMAL, 2, NULL);
			}
			break;

		case STATIC:
			Grid->SetTypeStaticText(&CellType, SS_TEXT_CENTER | SS_TEXT_VCENTER);
			break;

		case TIME:
			tmFormat.b24Hour=TRUE;
			tmFormat.bSpin=FALSE;
			tmFormat.bSeconds=FALSE;
			tmFormat.cSeparator=':';
			Grid->SetTypeTime(&CellType, 0, &tmFormat, NULL, NULL);
			break;

		case PERCENT:
			Grid->SetTypePercentEx(&CellType,ES_LEFTALIGN,2,.01,100,".",0,0,0,0,1.001);
			break;

		case CHECK1:
			Grid->SetTypeCheckBox(&CellType,BS_CENTER, "", 
				"CheckUp", BT_BITMAP,
				"CheckDown", BT_BITMAP,
				"CheckFocusUp", BT_BITMAP,
				"CheckFocusDown", BT_BITMAP,
				"CheckDisabledUp", BT_BITMAP,
				"CheckDisabledDown", BT_BITMAP);
			break;

		case NUMBER:
			Grid->SetTypeNumberEx(&CellType,0,pos, 0, 10000,0,".",",",0,0,1,0,1.001);
			break;
	}

	Grid->SetCellType(col,row,&CellType);

	// ���̸� �߰� [2015.02.16]
	if (Grid != NULL)
	{
		Grid = NULL;
		delete Grid;
	}
}

void CScreen_Set_Maintenance::GridData(UINT nID, int row, int col, CString data)
{
	TSpread *Grid = (TSpread*)GetDlgItem(nID);
	
	Grid->SetValue(col, row, data);

	// ���̸� �߰� [2015.02.16]
	if (Grid != NULL)
	{
		Grid = NULL;
		delete Grid;
	}
}

void CScreen_Set_Maintenance::GridFont(UINT nID, int row, int col, int size)
{
	TSpread *Grid = (TSpread *)GetDlgItem(nID);
	HFONT font;
	CString strTemp;
	
	font = CreateFont(size, 0, 0, 0, FW_BOLD, 0, 0, 0, 0, 0, 0, 0, 0, "����ü");
	Grid->SetFont(col, row, font, TRUE);

	Grid = NULL;
	delete Grid;
}

void CScreen_Set_Maintenance::GridMerge(UINT nID, int srow, int scol, int nrow, int ncol)
{
	TSpread *Grid = (TSpread*)GetDlgItem(nID);
	
	Grid->AddCellSpan(scol, srow, ncol, nrow);

	// ���̸� �߰� [2015.02.16]
	if (Grid != NULL)
	{
		Grid = NULL;
		delete Grid;
	}
}

void CScreen_Set_Maintenance::OnCellClick(WPARAM wParam, LPARAM lParam)
{
	LPSS_CELLCOORD lpcc = (LPSS_CELLCOORD)lParam;
	
	TSpread *Grid; 
	
	CString strTmp, strChange, str_file;
	CString str_tmp, str_data;
	
	CString strText;
	
	CDialog_KeyPad	pad_dlg;
	
	if (st_handler.nMenuLock) return;
	
	if (wParam == IDC_CUSTOM_PITCH)
	{
		Grid = (TSpread*)GetDlgItem(IDC_CUSTOM_PITCH);
		
		if (lpcc->Col == 1)
			return;	
		
		strText = m_pGrid.GridCellText(Grid, lpcc->Row, lpcc->Col - 1);
		st_msg.mstr_keypad_msg.Format("[Pitch] %s set", strText);
		
		strTmp = m_pGrid.GridCellText(Grid, lpcc->Row, lpcc->Col);
		st_msg.mstr_keypad_val = strTmp;
		
		strTmp = KeyPad.GetNumEditString_D(0, 250, atof(st_msg.mstr_keypad_val), st_msg.mstr_keypad_msg);
		
		md_pitch_data[lpcc->Row - 1][1] = atof(strTmp);
		m_pGrid.GridCellText(Grid, lpcc->Row, lpcc->Col, strTmp);
		
		// ���̸� �߰� [2015.02.16]
		if (Grid != NULL)
		{
			Grid = NULL;
			delete Grid;
		}
	}
}

void CScreen_Set_Maintenance::OnMaintenance_Init_Pitch()
{
	TSpread *Grid;
	CString str_pos;
	int row = 3;
	int col = 2;
	int i, j;
	CString strData;
	
	CString str_tmp[3] = {"Gripper Pitch X", "Gripper Pitch Y", "BufferConv Pitch Y"};
	
	Grid = (TSpread*)GetDlgItem(IDC_CUSTOM_PITCH);
	Grid -> SetBool(SSB_EDITMODEPERMANENT, TRUE);
	Grid -> SetBool(SSB_SHOWCOLHEADERS, FALSE);
	Grid -> SetBool(SSB_SHOWROWHEADERS, FALSE);
	Grid -> SetBool(SSB_HORZSCROLLBAR, FALSE);
	Grid -> SetBool(SSB_VERTSCROLLBAR, FALSE);
	Grid -> SetMaxRows(row);
	Grid -> SetMaxCols(col);
	
	for (i = 0; i <= row; i++)
	{
		for (j = 0; j <= col; j++)
		{
			Grid -> SetRowHeightInPixels(i, 24);
			
			if (j == 1)
			{
				Grid -> SetColWidthInPixels(j, 140);
				GridColor(IDC_CUSTOM_PITCH, i, j, YELLOW_L, BLACK_C);
				
				GridControl(IDC_CUSTOM_PITCH, STATIC, i, j, 0);
				GridFont(IDC_CUSTOM_PITCH, i, j, 15);
			}
			else
			{
				Grid -> SetColWidthInPixels(j, 155);
				GridColor(IDC_CUSTOM_PITCH, i, j, BLACK_C, WHITE_C);
				
				GridControl(IDC_CUSTOM_PITCH, STATIC, i, j, 0);
				
				GridFont(IDC_CUSTOM_PITCH, i, j, 15);
			}
		}
	}
	
	for (i = 1; i < row + 1; i++)
	{
		strData.Format("%3.2f", md_pitch_data[i - 1][1]);
		GridData(IDC_CUSTOM_PITCH, i, 1, str_tmp[i - 1]);
		GridData(IDC_CUSTOM_PITCH, i, 2, strData);
	}
	
	// ���̸� �߰� [2015.02.16]
	if (Grid != NULL)
	{
		Grid = NULL;
		delete Grid;
	}
}

void CScreen_Set_Maintenance::OnBtnNetworkAddrLoad() 
{
	m_cBasic.On_Network_Data_Load();
}

void CScreen_Set_Maintenance::OnBtnNetworkAddrModify() 
{
	ShellExecute(NULL, NULL, TEXT(st_path.mstr_network), NULL, TEXT("ReadMe"), SW_SHOW);
}
