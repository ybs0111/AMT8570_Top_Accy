// Screen_File_Editor.cpp : implementation file
//

#include "stdafx.h"
#include "handler.h"
#include "Screen_File_Editor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScreen_File_Editor

IMPLEMENT_DYNCREATE(CScreen_File_Editor, CFormView)

CScreen_File_Editor::CScreen_File_Editor()
	: CFormView(CScreen_File_Editor::IDD)
{
	//{{AFX_DATA_INIT(CScreen_File_Editor)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CScreen_File_Editor::~CScreen_File_Editor()
{
}

void CScreen_File_Editor::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CScreen_File_Editor)
	DDX_Control(pDX, IDC_BTN_TIME_SAVE, m_btn_time_save);
	DDX_Control(pDX, IDC_BTN_TIME_OPEN, m_btn_time_open);
	DDX_Control(pDX, IDC_BTN_MOTOR_POS_SAVE, m_btn_motor_pos_save);
	DDX_Control(pDX, IDC_BTN_MOTOR_POS_OPEN, m_btn_motor_pos_open);
	DDX_Control(pDX, IDC_BTN_MOTOR_PART_SAVE, m_btn_motor_part_save);
	DDX_Control(pDX, IDC_BTN_MOTOR_PART_OPEN, m_btn_motor_part_open);
	DDX_Control(pDX, IDC_BTN_IO_SAVE, m_btn_io_save);
	DDX_Control(pDX, IDC_BTN_IO_PART_SAVE, m_btn_io_part_save);
	DDX_Control(pDX, IDC_BTN_IO_PART_OPEN, m_btn_io_part_open);
	DDX_Control(pDX, IDC_BTN_IO_OPEN, m_btn_io_open);
	DDX_Control(pDX, IDC_GROUP_TIME, m_group_time);
	DDX_Control(pDX, IDC_GROUP_MOTOR_POS, m_group_motor_pos);
	DDX_Control(pDX, IDC_GROUP_MOTOR_PART, m_group_motor_part);
	DDX_Control(pDX, IDC_GROUP_IO_PART, m_group_io_part);
	DDX_Control(pDX, IDC_GROUP_IO, m_group_io);
	DDX_Control(pDX, IDC_GROUP_FILE_INFO, m_group_file_info);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CScreen_File_Editor, CFormView)
	//{{AFX_MSG_MAP(CScreen_File_Editor)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_IO_OPEN, OnBtnIoOpen)
	ON_BN_CLICKED(IDC_BTN_IO_SAVE, OnBtnIoSave)
	ON_BN_CLICKED(IDC_BTN_IO_PART_OPEN, OnBtnIoPartOpen)
	ON_BN_CLICKED(IDC_BTN_IO_PART_SAVE, OnBtnIoPartSave)
	ON_BN_CLICKED(IDC_BTN_MOTOR_PART_OPEN, OnBtnMotorPartOpen)
	ON_BN_CLICKED(IDC_BTN_MOTOR_PART_SAVE, OnBtnMotorPartSave)
	ON_BN_CLICKED(IDC_BTN_MOTOR_POS_OPEN, OnBtnMotorPosOpen)
	ON_BN_CLICKED(IDC_BTN_MOTOR_POS_SAVE, OnBtnMotorPosSave)
	ON_BN_CLICKED(IDC_BTN_TIME_OPEN, OnBtnTimeOpen)
	ON_BN_CLICKED(IDC_BTN_TIME_SAVE, OnBtnTimeSave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScreen_File_Editor diagnostics

#ifdef _DEBUG
void CScreen_File_Editor::AssertValid() const
{
	CFormView::AssertValid();
}

void CScreen_File_Editor::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CScreen_File_Editor message handlers

void CScreen_File_Editor::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	m_p_font = NULL;
	m_p_font = new CFont;
	m_p_font->CreateFont(24,0,0,0,900,0,0,0,0,0,0,ANTIALIASED_QUALITY,0,"MS Sans Serif");

	OnFileEditor_GroupBox_Set();	
}

void CScreen_File_Editor::OnDestroy() 
{
	if (m_p_font != NULL)
	{
		delete m_p_font;
		m_p_font = NULL;
	}

	CFormView::OnDestroy();
}

void CScreen_File_Editor::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	
	CFormView::OnTimer(nIDEvent);
}

void CScreen_File_Editor::OnBtnIoOpen() 
{
	m_grid_info = (TSpread*)GetDlgItem(IDC_CUSTOM_FILE_INFO);
	m_p_grid.GridFileOpen(m_grid_info, st_path.sIOMap, "Sheet1");
	
	m_btn_io_save.EnableWindow(TRUE);
	m_btn_io_part_save.EnableWindow(FALSE);
	
	m_btn_motor_part_save.EnableWindow(FALSE);
	m_btn_motor_pos_save.EnableWindow(FALSE);
	
	m_btn_time_save.EnableWindow(FALSE);
}

void CScreen_File_Editor::OnBtnIoSave() 
{
	m_grid_info = (TSpread*)GetDlgItem(IDC_CUSTOM_FILE_INFO);	
	m_p_grid.GridFileSave(m_grid_info, st_path.sIOMap, "Sheet1");
	
	m_btn_io_save.EnableWindow(FALSE);
}

void CScreen_File_Editor::OnBtnIoPartOpen() 
{
	m_grid_info = (TSpread*)GetDlgItem(IDC_CUSTOM_FILE_INFO);
	m_p_grid.GridFileOpen(m_grid_info, st_path.sIOPartMap, "Sheet1");
	
	m_btn_io_save.EnableWindow(FALSE);
	m_btn_io_part_save.EnableWindow(TRUE);
	
	m_btn_motor_part_save.EnableWindow(FALSE);
	m_btn_motor_pos_save.EnableWindow(FALSE);
	
	m_btn_time_save.EnableWindow(FALSE);
}

void CScreen_File_Editor::OnBtnIoPartSave() 
{
	m_grid_info = (TSpread*)GetDlgItem(IDC_CUSTOM_FILE_INFO);
	m_p_grid.GridFileSave(m_grid_info, st_path.sIOPartMap, "Sheet1");
	
	m_btn_io_part_save.EnableWindow(FALSE);
}

void CScreen_File_Editor::OnBtnMotorPartOpen() 
{
	m_grid_info = (TSpread*)GetDlgItem(IDC_CUSTOM_FILE_INFO);
	m_p_grid.GridFileOpen(m_grid_info, st_path.sMotPartMap, "Sheet1");
	
	m_btn_io_save.EnableWindow(FALSE);
	m_btn_io_part_save.EnableWindow(FALSE);
	
	m_btn_motor_part_save.EnableWindow(TRUE);
	m_btn_motor_pos_save.EnableWindow(FALSE);
	
	m_btn_time_save.EnableWindow(FALSE);
}

void CScreen_File_Editor::OnBtnMotorPartSave() 
{
	m_grid_info = (TSpread*)GetDlgItem(IDC_CUSTOM_FILE_INFO);
	m_p_grid.GridFileSave(m_grid_info, st_path.sMotPartMap, "Sheet1");
	
	m_btn_motor_part_save.EnableWindow(FALSE);
}

void CScreen_File_Editor::OnBtnMotorPosOpen() 
{
	m_grid_info = (TSpread*)GetDlgItem(IDC_CUSTOM_FILE_INFO);
	m_p_grid.GridFileOpen(m_grid_info, st_path.sMotAxisMap, "Sheet1");
	
	m_btn_io_save.EnableWindow(FALSE);
	m_btn_io_part_save.EnableWindow(FALSE);
	
	m_btn_motor_part_save.EnableWindow(FALSE);
	m_btn_motor_pos_save.EnableWindow(TRUE);
	
	m_btn_time_save.EnableWindow(FALSE);
}

void CScreen_File_Editor::OnBtnMotorPosSave() 
{
	m_grid_info = (TSpread*)GetDlgItem(IDC_CUSTOM_FILE_INFO);	
	m_p_grid.GridFileSave(m_grid_info, st_path.sMotAxisMap, "Sheet1");
	
	m_btn_motor_pos_save.EnableWindow(FALSE);
}

void CScreen_File_Editor::OnBtnTimeOpen() 
{
	m_grid_info = (TSpread*)GetDlgItem(IDC_CUSTOM_FILE_INFO);
	m_p_grid.GridFileOpen(m_grid_info, st_path.sWaitTimeMap, "Sheet1");
	
	m_btn_io_save.EnableWindow(FALSE);
	m_btn_io_part_save.EnableWindow(FALSE);
	
	m_btn_motor_part_save.EnableWindow(FALSE);
	m_btn_motor_pos_save.EnableWindow(FALSE);
	
	m_btn_time_save.EnableWindow(TRUE);
}

void CScreen_File_Editor::OnBtnTimeSave() 
{
	m_grid_info = (TSpread*)GetDlgItem(IDC_CUSTOM_FILE_INFO);
	m_p_grid.GridFileSave(m_grid_info, st_path.sWaitTimeMap, "Sheet1");
	
	m_btn_time_save.EnableWindow(FALSE);
}

void CScreen_File_Editor::OnFileEditor_GroupBox_Set()
{
	CSxLogFont fileeditor_font(15,FW_SEMIBOLD,false,"Arial");
	
	m_group_time.SetFont(fileeditor_font);
	m_group_time.SetCatptionTextColor(RGB(145,25,0));
	m_group_time.SetFontBold(TRUE);

	m_group_motor_pos.SetFont(fileeditor_font);
	m_group_motor_pos.SetCatptionTextColor(RGB(145,25,0));
	m_group_motor_pos.SetFontBold(TRUE);

	m_group_motor_part.SetFont(fileeditor_font);
	m_group_motor_part.SetCatptionTextColor(RGB(145,25,0));
	m_group_motor_part.SetFontBold(TRUE);

	m_group_io_part.SetFont(fileeditor_font);
	m_group_io_part.SetCatptionTextColor(RGB(145,25,0));
	m_group_io_part.SetFontBold(TRUE);

	m_group_io.SetFont(fileeditor_font);
	m_group_io.SetCatptionTextColor(RGB(145,25,0));
	m_group_io.SetFontBold(TRUE);

	m_group_file_info.SetFont(fileeditor_font);
	m_group_file_info.SetCatptionTextColor(RGB(145,25,0));
	m_group_file_info.SetFontBold(TRUE);
}
