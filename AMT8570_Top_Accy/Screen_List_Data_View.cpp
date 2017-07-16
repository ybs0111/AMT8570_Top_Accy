// Screen_List_Data_View.cpp : implementation file
//

#include "stdafx.h"
#include "Handler.h"
#include "Screen_List_Data_View.h"
#include "Dialog_Infor.h"

#include "io.h" 
#include "FileDialogST.h"
#include "Dialog_KeyPad.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScreen_List_Data_View

IMPLEMENT_DYNCREATE(CScreen_List_Data_View, CFormView)

CScreen_List_Data_View::CScreen_List_Data_View()
	: CFormView(CScreen_List_Data_View::IDD)
{
	//{{AFX_DATA_INIT(CScreen_List_Data_View)
	//}}AFX_DATA_INIT
}

CScreen_List_Data_View::~CScreen_List_Data_View()
{
}

void CScreen_List_Data_View::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CScreen_List_Data_View)
	DDX_Control(pDX, IDC_BTN_SCARP_SEARCH, m_btn_scrap_search);
	DDX_Control(pDX, IDC_BTN_BUFFER_SEARCH, m_btn_buffer_search);
	DDX_Control(pDX, IDC_BTN_START_TIME, m_btn_start_time);
	DDX_Control(pDX, IDC_BTN_END_TIME, m_btn_end_time);
	DDX_Control(pDX, IDC_BTN_DAY_TODAY, m_btn_day_today);
	DDX_Control(pDX, IDC_BTN_DAY_PREV, m_btn_day_prev);
	DDX_Control(pDX, IDC_BTN_DAY_NEXT, m_btn_day_next);
	DDX_Control(pDX, IDC_RADIO_BOARD, m_radio_board);
	DDX_Control(pDX, IDC_RADIO_LOT, m_radio_lot);
	DDX_Control(pDX, IDC_RADIO_ALL, m_radio_all);
	DDX_Control(pDX, IDC_GROUP_START_DATE, m_group_start_date);
	DDX_Control(pDX, IDC_GROUP_END_DATE, m_group_end_date);
	DDX_Control(pDX, IDC_GROUP_DATA, m_group_data);
	DDX_Control(pDX, IDC_BTN_VIEW_RETEST, m_btn_view_retest);
	DDX_Control(pDX, IDC_BTN_VIEW_PRIME, m_btn_view_prime);
	DDX_Control(pDX, IDC_BTN_VIEW, m_btn_view);
	DDX_Control(pDX, IDC_BTN_SEARCH_TIME, m_btn_serach_time);
	DDX_Control(pDX, IDC_BTN_SEARCH_BOARDOFF, m_btn_search_boardoff);
	DDX_Control(pDX, IDC_BTN_SEARCH_GROUND, m_btn_search_ground);
	DDX_Control(pDX, IDC_BTN_SEARCH_ALARM, m_btn_search_alarm);
	DDX_Control(pDX, IDC_BTN_SAVE, m_btn_save);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CScreen_List_Data_View, CFormView)
	//{{AFX_MSG_MAP(CScreen_List_Data_View)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_SEARCH_TIME, OnBtnSearchTime)
	ON_BN_CLICKED(IDC_BTN_SEARCH_ALARM, OnBtnSearchAlarm)
	ON_BN_CLICKED(IDC_RADIO_ALL, OnRadioAll)
	ON_BN_CLICKED(IDC_RADIO_LOT, OnRadioLot)
	ON_BN_CLICKED(IDC_RADIO_BOARD, OnRadioBoard)
	ON_BN_CLICKED(IDC_BTN_SEARCH_GROUND, OnBtnSearchGround)
	ON_BN_CLICKED(IDC_BTN_SAVE, OnBtnSave)
	ON_BN_CLICKED(IDC_BTN_DAY_PREV, OnBtnDayPrev)
	ON_BN_CLICKED(IDC_BTN_DAY_NEXT, OnBtnDayNext)
	ON_BN_CLICKED(IDC_BTN_DAY_TODAY, OnBtnDayToday)
	ON_BN_CLICKED(IDC_BTN_VIEW, OnBtnView)
	ON_BN_CLICKED(IDC_BTN_VIEW_PRIME, OnBtnViewPrime)
	ON_BN_CLICKED(IDC_BTN_VIEW_RETEST, OnBtnViewRetest)
	ON_BN_CLICKED(IDC_BTN_START_TIME, OnBtnStartTime)
	ON_BN_CLICKED(IDC_BTN_END_TIME, OnBtnEndTime)
	ON_BN_CLICKED(IDC_BTN_BUFFER_SEARCH, OnBtnBufferSearch)
	ON_BN_CLICKED(IDC_BTN_SCARP_SEARCH, OnBtnScarpSearch)
	//}}AFX_MSG_MAP
	ON_MESSAGE(SSM_CLICK  , OnCellClick)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScreen_List_Data_View diagnostics

#ifdef _DEBUG
void CScreen_List_Data_View::AssertValid() const
{
	CFormView::AssertValid();
}

void CScreen_List_Data_View::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CScreen_List_Data_View message handlers
void CScreen_List_Data_View::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	mp_mainfont = NULL;
	mp_mainfont = new CFont;
	mp_mainfont->CreateFont(24,0,0,0,900,0,0,0,0,0,0,ANTIALIASED_QUALITY,0,"MS Sans Serif");
	
	mn_view_type = 0;				// 0은 기본값.
	nSearchMethod = 2;				// 2은 ALL
	mstr_file_full_name = "";
	mn_socket_view_mode = 0;		// 0은 ALL, 1은 Prime, 2는 Retest
	
	Init_Group();
	Init_Date();
	//Init_Lot();
	Init_Data();
	Init_Device();
	
	OnData_Search_Change();
	
}

void CScreen_List_Data_View::OnDestroy() 
{
	CFormView::OnDestroy();
}

void CScreen_List_Data_View::Init_Group()
{
	CSxLogFont main_font(15,FW_SEMIBOLD,false,"Arial");
	
	m_group_start_date.SetFont(main_font);
	m_group_start_date.SetCatptionTextColor(RGB(145,25,0));
	m_group_start_date.SetFontBold(TRUE);
	
	m_group_end_date.SetFont(main_font);
	m_group_end_date.SetCatptionTextColor(RGB(145,25,0));
	m_group_end_date.SetFontBold(TRUE);
	
	m_group_data.SetFont(main_font);
	m_group_data.SetCatptionTextColor(RGB(145,25,0));
	m_group_data.SetFontBold(TRUE);
}

void CScreen_List_Data_View::Init_Date()
{
	COleDateTime time;
	COleDateTime Stime;
	COleDateTime Etime;
	
	time = COleDateTime::GetCurrentTime();
	
	m_pstart_date.SetShowPickerButton(TRUE);
	m_pstart_date.AttachEdit(this, IDC_START_DATE);
	m_pstart_time.AttachEdit(this, IDC_START_TIME);
	
	m_pend_date.SetShowPickerButton(TRUE);
	m_pend_date.AttachEdit(this, IDC_END_DATE);
	m_pend_time.AttachEdit(this, IDC_END_TIME);
	
	// set the display format for the time and date controls
	m_pstart_date.SetDisplayFormat("%Y/%m/%d");
	m_pstart_time.SetDisplayFormat("%H:%M:%S");
	
	m_pend_date.SetDisplayFormat("%Y/%m/%d");
	m_pend_time.SetDisplayFormat("%H:%M:%S");
	
	// set appropriate parser options
	m_pstart_date.SetParserOption(VAR_DATEVALUEONLY);
	m_pstart_time.SetParserOption(VAR_TIMEVALUEONLY);
	
	m_pend_date.SetParserOption(VAR_DATEVALUEONLY);
	m_pend_time.SetParserOption(VAR_TIMEVALUEONLY);
	
	m_pstart_date.SetFont(mp_mainfont);
	m_pstart_time.SetFont(mp_mainfont);
	
	m_pend_date.SetFont(mp_mainfont);	
	m_pend_time.SetFont(mp_mainfont);	
	
	if (time.GetHour() >= 22)			// 현재 시간이 22보다 크면...
	{
		Stime = time + COleDateTimeSpan(0, 0, 0, 0);				// 오늘
		Etime = time + COleDateTimeSpan(+1, 0, 0, 0);				// 내일
	}
	else
	{
		Stime = time + COleDateTimeSpan(-1, 0, 0, 0);				// 어제
		Etime = time + COleDateTimeSpan(0, 0, 0, 0);				// 오늘
	}
	
	m_pstart_date.SetDate(Stime);
	m_pstart_time.SetDate(Stime);
	
	m_pend_date.SetDate(Etime);
	m_pend_time.SetDate(Etime);
	
	((CEdit*)GetDlgItem(IDC_START_TIME))->SetWindowText("22:00:00");
	((CEdit*)GetDlgItem(IDC_END_TIME))->SetWindowText("22:00:00");
}

void CScreen_List_Data_View::Init_Lot()
{
	TSpread *Grid; 
	int row;
	Grid = (TSpread*)GetDlgItem(IDC_CUSTOM_LOT);
	
	m_pGrid.GridReset(Grid);
	// 대문자 
	m_pGrid.GridCellSelectDisplay(Grid, TRUE);
	m_pGrid.GridRowHeader(Grid, FALSE);
	m_pGrid.GridColHeader(Grid, FALSE);
	m_pGrid.GridHorizontal(Grid, FALSE);
	m_pGrid.GridVertical(Grid, TRUE);
	m_pGrid.GridAutoSize(Grid, FALSE);
	
	m_pGrid.GridCellRows(Grid, 1);
	m_pGrid.GridCellCols(Grid, 2);
	
	m_pGrid.GridCellHeight(Grid, 1, 20);
	m_pGrid.GridCellWidth(Grid, 1, 10);
	m_pGrid.GridCellWidth(Grid, 2, 34);
	
	row =1;
	m_pGrid.GridCellControlStatic(Grid, row, 1);
	m_pGrid.GridCellFont(Grid, row, 1, "MS Sans Serif", 20);
	m_pGrid.GridCellColor(Grid, row, 1, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 1, "No.");
	
	m_pGrid.GridCellControlStatic(Grid, row, 2);
	m_pGrid.GridCellFont(Grid, row, 2, "MS Sans Serif", 12);
	m_pGrid.GridCellColor(Grid, row, 2, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 2, "Lot No");

	if (nSearchMethod == 1)
	{
		m_pGrid.GridCellText(Grid, row, 2, "Scrap Code");
	}
}

void CScreen_List_Data_View::Init_Device()
{
	TSpread *Grid; 
	int row;
	int i;
	Grid = (TSpread*)GetDlgItem(IDC_CUSTOM_DEVICE);
	
	m_pGrid.GridReset(Grid);
	// 대문자 
	m_pGrid.GridCellSelectDisplay(Grid, TRUE);
	m_pGrid.GridRowHeader(Grid, FALSE);
	m_pGrid.GridColHeader(Grid, TRUE);
	m_pGrid.GridHorizontal(Grid, TRUE);
	m_pGrid.GridVertical(Grid, TRUE);
	m_pGrid.GridAutoSize(Grid, FALSE);
	
	m_pGrid.GridCellRows(Grid, 1);
	m_pGrid.GridCellCols(Grid, 51);
	

	m_pGrid.GridCellHeight(Grid, 0, 20);
	
	for (i = 0; i<51; i++)
	{
		m_pGrid.GridCellWidth(Grid, i+1, 25);

		m_pGrid.GridCellControlStatic(Grid, 0, i+1);
		m_pGrid.GridCellFont(Grid, 0, i+1, "MS Sans Serif", 18);
	}
	
	row =0;
	m_pGrid.GridCellColor(Grid, row, 1, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 1, "LOT No");
	
	m_pGrid.GridCellColor(Grid, row, 2, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 2, "LOT Type");

	m_pGrid.GridCellColor(Grid, row, 3, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 3, "Part No");

	m_pGrid.GridCellColor(Grid, row, 4, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 4, "Start Time");

	m_pGrid.GridCellColor(Grid, row, 5, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 5, "End Time");

	m_pGrid.GridCellColor(Grid, row, 6, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 6, "Array Y");

	m_pGrid.GridCellColor(Grid, row, 7, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 7, "Array X");

	m_pGrid.GridCellColor(Grid, row, 8, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 8, "Lot Qty");

	m_pGrid.GridCellColor(Grid, row, 9, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 9, "PCB Dir");

	m_pGrid.GridCellColor(Grid, row, 10, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 10, "Result");

	m_pGrid.GridCellColor(Grid, row, 11, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 11, "PCB1 Result");
	
	m_pGrid.GridCellColor(Grid, row, 12, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 12, "PCB2 Result");
	
	m_pGrid.GridCellColor(Grid, row, 13, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 13, "PCB3 Result");
	
	m_pGrid.GridCellColor(Grid, row, 14, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 14, "PCB4 Result");
	
	m_pGrid.GridCellColor(Grid, row, 15, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 15, "PCB5 Result");
	
	m_pGrid.GridCellColor(Grid, row, 16, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 16, "PCB6 Result");
	
	m_pGrid.GridCellColor(Grid, row, 17, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 17, "PCB7 Result");
	
	m_pGrid.GridCellColor(Grid, row, 18, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 18, "PCB8 Result");
	
	m_pGrid.GridCellColor(Grid, row, 19, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 19, "PCB9 Result");
	
	m_pGrid.GridCellColor(Grid, row, 20, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 20, "PCB10 Result");
	
	m_pGrid.GridCellColor(Grid, row, 21, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 21, "PCB11 Result");
	
	m_pGrid.GridCellColor(Grid, row, 22, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 22, "PCB12 Result");
	
	m_pGrid.GridCellColor(Grid, row, 23, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 23, "PCB13 Result");
	
	m_pGrid.GridCellColor(Grid, row, 24, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 24, "PCB14 Result");
	
	m_pGrid.GridCellColor(Grid, row, 25, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 25, "PCB15 Result");
	
	m_pGrid.GridCellColor(Grid, row, 26, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 26, "PCB16 Result");
	
	m_pGrid.GridCellColor(Grid, row, 27, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 27, "PCB17 Result");
	
	m_pGrid.GridCellColor(Grid, row, 28, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 28, "PCB18 Result");
	
	m_pGrid.GridCellColor(Grid, row, 29, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 29, "PCB19 Result");
	
	m_pGrid.GridCellColor(Grid, row, 30, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 30, "PCB20 Result");

	m_pGrid.GridCellColor(Grid, row, 31, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 31, "PCB1 Serial");

	m_pGrid.GridCellColor(Grid, row, 32, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 32, "PCB2 Serial");

	m_pGrid.GridCellColor(Grid, row, 33, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 33, "PCB3 Serial");

	m_pGrid.GridCellColor(Grid, row, 34, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 34, "PCB4 Serial");

	m_pGrid.GridCellColor(Grid, row, 35, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 35, "PCB5 Serial");

	m_pGrid.GridCellColor(Grid, row, 36, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 36, "PCB6 Serial");

	m_pGrid.GridCellColor(Grid, row, 37, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 37, "PCB7 Serial");

	m_pGrid.GridCellColor(Grid, row, 38, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 38, "PCB8 Serial");

	m_pGrid.GridCellColor(Grid, row, 39, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 39, "PCB9 Serial");

	m_pGrid.GridCellColor(Grid, row, 40, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 40, "PCB10 Serial");

	m_pGrid.GridCellColor(Grid, row, 41, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 41, "PCB11 Serial");

	m_pGrid.GridCellColor(Grid, row, 42, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 42, "PCB12 Serial");

	m_pGrid.GridCellColor(Grid, row, 43, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 43, "PCB13 Serial");

	m_pGrid.GridCellColor(Grid, row, 44, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 44, "PCB14 Serial");

	m_pGrid.GridCellColor(Grid, row, 45, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 45, "PCB15 Serial");

	m_pGrid.GridCellColor(Grid, row, 46, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 46, "PCB16 Serial");

	m_pGrid.GridCellColor(Grid, row, 47, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 47, "PCB17 Serial");

	m_pGrid.GridCellColor(Grid, row, 48, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 48, "PCB18 Serial");

	m_pGrid.GridCellColor(Grid, row, 49, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 49, "PCB19 Serial");

	m_pGrid.GridCellColor(Grid, row, 50, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 50, "PCB20 Serial");

	m_pGrid.GridCellColor(Grid, row, 51, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 51, "Note");
}

void CScreen_List_Data_View::Display_LotNo(int count, CString lot_no)
{
	TSpread *Grid; 
	CString strTmp;
	
	Grid = (TSpread*)GetDlgItem(IDC_CUSTOM_LOT);
	
	m_pGrid.GridCellRows(Grid, count+2);
	m_pGrid.GridCellCols(Grid, 2);
	
	m_pGrid.GridCellHeight(Grid, count+2, 20);
	m_pGrid.GridCellWidth(Grid, 1, 10);
	m_pGrid.GridCellWidth(Grid, 2, 34);
	
	m_pGrid.GridCellControlStatic(Grid, count+2, 1);
	m_pGrid.GridCellFont(Grid, count+2, 1, "MS Sans Serif", 20);
	m_pGrid.GridCellColor(Grid, count+2, 1, YELLOW_L, BLACK_C);	
	strTmp.Format("%d", count+1);
	m_pGrid.GridCellText(Grid, count+2, 1, strTmp);
	
	m_pGrid.GridCellControlStatic(Grid, count+2, 2);
	m_pGrid.GridCellFont(Grid, count+2, 2, "MS Sans Serif", 12);
	m_pGrid.GridCellColor(Grid, count+2, 2, WHITE_C, BLUE_D);	
	m_pGrid.GridCellText(Grid, count+2, 2, lot_no);
}

void CScreen_List_Data_View::Display_Data()
{
	TSpread *Grid;
	Grid = (TSpread*)GetDlgItem(IDC_CUSTOM_DATA);
	
	// Total
	m_pGrid.GridCellColor(Grid, 1, 1, BLUE_D, WHITE_C);
	m_pGrid.GridCellText(Grid, 1, 1, "TOTAL" );

	// Load
	m_pGrid.GridCellColor(Grid, 2, 1, BLUE_C, WHITE_C);
	m_pGrid.GridCellText(Grid, 2, 1, "Load" );

	// Pass
	m_pGrid.GridCellColor(Grid, 3, 1, BLUE_L, WHITE_C);
	m_pGrid.GridCellText(Grid, 3, 1, "Pass" );

	// NTO
	m_pGrid.GridCellColor(Grid, 4, 1, GREEN_D, WHITE_C);
	m_pGrid.GridCellText(Grid, 4, 1, "NTO" );

	// BTO
	m_pGrid.GridCellColor(Grid, 5, 1, GREEN_D, WHITE_C);
	m_pGrid.GridCellText(Grid, 5, 1, "BTO" );

	// HTO
	m_pGrid.GridCellColor(Grid, 6, 1, GREEN_D, WHITE_C);
	m_pGrid.GridCellText(Grid, 6, 1, "HTO" );

	// FAIL
	m_pGrid.GridCellColor(Grid, 7, 1, GREEN_D, WHITE_C);
	m_pGrid.GridCellText(Grid, 7, 1, "FAIL" );

	// FAIL and NBH
	m_pGrid.GridCellColor(Grid, 7, 3, GREEN_D, WHITE_C);
	m_pGrid.GridCellText(Grid, 7, 3, "FAIL and NBH" );

	// 기타 등등
	m_pGrid.GridCellColor(Grid, 1, 3, GRAY_C, WHITE_C);
	m_pGrid.GridCellText(Grid, 1, 3, "" );

	for( int i=2; i<=6; i++ )
	{
		for( int j=3; j<=4; j++ )
		{
			m_pGrid.GridCellColor(Grid, i, j, GRAY_C, WHITE_C);
			m_pGrid.GridCellText(Grid, i, j, "" );
		}
	}


	// =-=-=-=-=-=-=-=-=-=- data =-=-=-=-=-=-=-=-=-==-
	CString strTmp;
	int iLoad = (int)md_board_result_count_t;
	
	// Load
	strTmp.Format("%d", iLoad );
	m_pGrid.GridCellColor(Grid, 2, 2, YELLOW_L, BLACK_C);
	m_pGrid.GridCellText(Grid, 2, 2, strTmp );

	if( iLoad == 0 )
		iLoad = 1;

	// Pass
	strTmp.Format("%d ( %.03f %% )", (int)md_board_good_t, (float)md_board_good_t / (float)iLoad * 100.f );
	m_pGrid.GridCellColor(Grid, 3, 2, YELLOW_C, BLACK_C);
	m_pGrid.GridCellText(Grid, 3, 2, strTmp );

	// NTO
	strTmp.Format("%d ( %.03f %% )", (int)md_board_nto, (float)md_board_nto / (float)iLoad * 100.f );
	m_pGrid.GridCellColor(Grid, 4, 2, RED_C, BLACK_C);
	m_pGrid.GridCellText(Grid, 4, 2, strTmp );

	// BTO
	strTmp.Format("%d ( %.03f %% )", (int)md_board_bto, (float)md_board_bto / (float)iLoad * 100.f );
	m_pGrid.GridCellColor(Grid, 5, 2, RED_C, BLACK_C);
	m_pGrid.GridCellText(Grid, 5, 2, strTmp );

	// HTO
	strTmp.Format("%d ( %.03f %% )", (int)md_board_hto, (float)md_board_hto / (float)iLoad * 100.f );
	m_pGrid.GridCellColor(Grid, 6, 2, RED_C, BLACK_C);
	m_pGrid.GridCellText(Grid, 6, 2, strTmp );

	// FAIL
	strTmp.Format("%d ( %.03f %% )", (int)md_board_fail, (float)md_board_fail / (float)iLoad * 100.f );
	m_pGrid.GridCellColor(Grid, 7, 2, RED_C, BLACK_C);
	m_pGrid.GridCellText(Grid, 7, 2, strTmp );

	// FAIL and NBH
	strTmp.Format("%d ( %.03f %% )", (int)md_board_fail_t, (float)md_board_fail_t / (float)iLoad * 100.f );
	m_pGrid.GridCellColor(Grid, 7, 4, RED_C, BLACK_C);
	m_pGrid.GridCellText(Grid, 7, 4, strTmp );
}

void CScreen_List_Data_View::Init_Chart()
{

}

void CScreen_List_Data_View::OnData_Search_Change()
{
	if (nSearchMethod < 0 || nSearchMethod > 2)	nSearchMethod = 2;
	
	switch(nSearchMethod)
	{
	case 0:			//LOT
		m_radio_lot.SetCheck(TRUE);
		m_radio_board.SetCheck(FALSE);
		m_radio_all.SetCheck(FALSE);
		break;

	case 1:			//BOARD
		m_radio_lot.SetCheck(FALSE);
		m_radio_board.SetCheck(TRUE);
		m_radio_all.SetCheck(FALSE);
		break;
		
	case 2:			//ALL
		m_radio_lot.SetCheck(FALSE);
		m_radio_board.SetCheck(FALSE);
		m_radio_all.SetCheck(TRUE);
		break;
	}
}

void CScreen_List_Data_View::Init_Data()
{
	TSpread *Grid; 
	int row;
	int i, j;
	Grid = (TSpread*)GetDlgItem(IDC_CUSTOM_DATA);
	
	m_pGrid.GridReset(Grid);
	// 대문자 
	m_pGrid.GridCellSelectDisplay(Grid, TRUE);
	m_pGrid.GridRowHeader(Grid, FALSE);
	m_pGrid.GridColHeader(Grid, FALSE);
	m_pGrid.GridHorizontal(Grid, FALSE);
	m_pGrid.GridVertical(Grid, FALSE);
	m_pGrid.GridAutoSize(Grid, FALSE);
	
	m_pGrid.GridCellRows(Grid, 7);
	m_pGrid.GridCellCols(Grid, 4);
	
	for (i = 0; i<7; i++)
	{
		m_pGrid.GridCellHeight(Grid, i+1, 19);

		for(j=0; j<4; j++)
		{
			m_pGrid.GridCellWidth(Grid, j+1, 21.5);
			m_pGrid.GridCellControlStatic(Grid, i+1, j+1);
			m_pGrid.GridCellFont(Grid, i+1, j+1, "MS Sans Serif", 10);
		}
	}
		
	row =1;
	m_pGrid.GridCellMerge(Grid, row, 1, 1, 2);
	m_pGrid.GridCellColor(Grid, row, 1, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 1, "Lot No");
	
	m_pGrid.GridCellMerge(Grid, row, 3, 1, 2);
	m_pGrid.GridCellFont(Grid, row, 3, "MS Sans Serif", 20);
	m_pGrid.GridCellColor(Grid, row, 3, TEXT_BC, RED_C);	
	m_pGrid.GridCellText(Grid, row, 3, "-");

	row =2;
	m_pGrid.GridCellColor(Grid, row, 1, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 1, "LOT Type");

	m_pGrid.GridCellColor(Grid, row, 2, RED_L, BLACK_C);	
	m_pGrid.GridCellText(Grid, row, 2, "-");

	m_pGrid.GridCellColor(Grid, row, 3, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 3, "Part Number");
	
	m_pGrid.GridCellColor(Grid, row, 4, GREEN_L, BLACK_C);	
	m_pGrid.GridCellText(Grid, row, 4, "-");

	row =3;
	m_pGrid.GridCellColor(Grid, row, 1, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 1, "Start Time");
	
	m_pGrid.GridCellColor(Grid, row, 2, TEXT_BC, BLACK_C);	
	m_pGrid.GridCellText(Grid, row, 2, "-");
	
	m_pGrid.GridCellColor(Grid, row, 3, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 3, "End Time");
	
	m_pGrid.GridCellColor(Grid, row, 4, GREEN_C, BLACK_C);	
	m_pGrid.GridCellText(Grid, row, 4, "-");

	row =4;
	m_pGrid.GridCellColor(Grid, row, 1, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 1, "PCB Array Row");
	
	m_pGrid.GridCellColor(Grid, row, 2, GREEN_L, BLACK_C);	
	m_pGrid.GridCellText(Grid, row, 2, "-");
	
	m_pGrid.GridCellColor(Grid, row, 3, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 3, "PCB Array Col");
	
	m_pGrid.GridCellColor(Grid, row, 4, GREEN_L, BLACK_C);	
	m_pGrid.GridCellText(Grid, row, 4, "-");

	row =5;
	m_pGrid.GridCellColor(Grid, row, 1, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 1, "Lot QTY");
	
	m_pGrid.GridCellColor(Grid, row, 2, RED_L, BLACK_C);	
	m_pGrid.GridCellText(Grid, row, 2, "-");
	
	m_pGrid.GridCellColor(Grid, row, 3, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 3, "PCB Count");
	
	m_pGrid.GridCellColor(Grid, row, 4, TEXT_BC, BLACK_C);	
	m_pGrid.GridCellText(Grid, row, 4, "-");

	row = 6;
	m_pGrid.GridCellColor(Grid, row, 1, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 1, "Test QTY");
	
	m_pGrid.GridCellColor(Grid, row, 2, RED_L, BLACK_C);	
	m_pGrid.GridCellText(Grid, row, 2, "-");
	
	m_pGrid.GridCellColor(Grid, row, 3, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 3, "Cum Yield");
	
	m_pGrid.GridCellColor(Grid, row, 4, TEXT_BC, BLACK_C);	
	m_pGrid.GridCellText(Grid, row, 4, "-");

	row = 7;
	m_pGrid.GridCellColor(Grid, row, 1, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 1, "Good Count");
	
	m_pGrid.GridCellColor(Grid, row, 2, RED_L, BLACK_C);	
	m_pGrid.GridCellText(Grid, row, 2, "-");
	
	m_pGrid.GridCellColor(Grid, row, 3, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 3, "Fail Count");
	
	m_pGrid.GridCellColor(Grid, row, 4, TEXT_BC, BLACK_C);	
	m_pGrid.GridCellText(Grid, row, 4, "-");
}

void CScreen_List_Data_View::OnBtnSearchTime() 
{
	int year_s, year_e, month_s, month_e, day_s, day_e;
	int hour_s, hour_e, min_s, min_e, sec_s, sec_e;
	long count = 0;
	int i, j;
	CString temp;
	CString strTmp, strDate;
	CTime time;

// 	st_timedb.mnTime_Section	= st_timedb.mnSectionBuffer;
// 	st_timedb.mole_date			= COleDateTime::GetCurrentTime();
// // 	st_timedb.mstrlot_no		= st_pcb[1].strLotNumber;
// // 	st_timedb.mstrpart_no		= st_pcb[1].strPartNumber;
// 	Func.DB_Write_Time(st_timedb);

	TSpread *Grid; 
	int row;

	mn_view_type = 6;				// 6은 시간.
	
	m_oledate[0] = m_pstart_date.GetDate();
	m_oledate[1] = m_pend_date.GetDate();

	m_oletime[0] = m_pstart_time.GetDate();
	m_oletime[1] = m_pend_time.GetDate();

	year_s		= m_oledate[0].GetYear();
	month_s		= m_oledate[0].GetMonth();
	day_s		= m_oledate[0].GetDay();

	hour_s		= m_oletime[0].GetHour();
	min_s		= m_oletime[0].GetMinute();
	sec_s		= m_oletime[0].GetSecond();

	year_e		= m_oledate[1].GetYear();
	month_e		= m_oledate[1].GetMonth();
	day_e		= m_oledate[1].GetDay();
	
	hour_e		= m_oletime[1].GetHour();
	min_e		= m_oletime[1].GetMinute();
	sec_e		= m_oletime[1].GetSecond();

	Init_Data();
	
	for (i = 0; i < 6; i++)
	{
		for (j = 0; j < 2; j++)
		{
			dTime[i][j] = 0;
		}
	}

	CString strFuncRet = OnDateCheck(year_s, month_s, day_s, hour_s, min_s, sec_s, year_e, month_e, day_e, hour_e, min_e, sec_e);
	
	if (strFuncRet != "NONE")
	{
		Func.MsgLog(strFuncRet);
		return;
	}

	m_oledate[0].SetDateTime(year_s, month_s, day_s, hour_s, 0, 0);
	m_oledate[1].SetDateTime(year_e, month_e, day_e, hour_e, 0, 0);

	if (Mysql.MySql_Open("localhost", "root", "1111", 3306))
	{
		if (Mysql.Database_Select("AMT8530"))
		{	
			count = Mysql.Record_Date_Search("TIME_DATA", "DATE", "<>", m_oledate);
			
			if (count > 0)
			{
				Init_Lot();
				Init_Time();

				for (i = 0; i<count; i++)
				{
					Display_Time(i);
				}

				// 해당 결과에 대한 정리를 한다.
				Grid = (TSpread*)GetDlgItem(IDC_CUSTOM_DATA);
				
				row = 2;
				m_pGrid.GridCellColor(Grid, row, 1, BLUE_D, WHITE_C);	
				m_pGrid.GridCellText(Grid, row, 1, "Run Time");
				
				strTmp.Format("%3.2f (Sec)", (float)((dTime[1][0] + dTime[1][1]) / (long)1000));
				m_pGrid.GridCellColor(Grid, row, 2, RED_L, BLACK_C);	
				m_pGrid.GridCellText(Grid, row, 2, strTmp);
				
				m_pGrid.GridCellColor(Grid, row, 3, BLUE_D, WHITE_C);	
				m_pGrid.GridCellText(Grid, row, 3, "");
				
				m_pGrid.GridCellColor(Grid, row, 4, TEXT_BC, BLACK_C);	
				m_pGrid.GridCellText(Grid, row, 4, "");
				
				row = 3;
				m_pGrid.GridCellColor(Grid, row, 1, BLUE_D, WHITE_C);	
				m_pGrid.GridCellText(Grid, row, 1, "Stop Time");
				
				strTmp.Format("%3.2f (Sec)", (float)((dTime[0][0] + dTime[0][1]) / (long)1000));
				m_pGrid.GridCellColor(Grid, row, 2, RED_L, BLACK_C);	
				m_pGrid.GridCellText(Grid, row, 2, strTmp);
				
				m_pGrid.GridCellColor(Grid, row, 3, BLUE_D, WHITE_C);	
				m_pGrid.GridCellText(Grid, row, 3, "");
				
				m_pGrid.GridCellColor(Grid, row, 4, TEXT_BC, BLACK_C);	
				m_pGrid.GridCellText(Grid, row, 4, "");
				
				row = 4;
				m_pGrid.GridCellColor(Grid, row, 1, BLUE_D, WHITE_C);	
				m_pGrid.GridCellText(Grid, row, 1, "Alarm Time");
				
				strTmp.Format("%3.2f (Sec)", (float)((dTime[5][0] + dTime[5][1]) / (long)1000));
				m_pGrid.GridCellColor(Grid, row, 2, RED_L, BLACK_C);	
				m_pGrid.GridCellText(Grid, row, 2, strTmp);
				
				m_pGrid.GridCellColor(Grid, row, 3, BLUE_D, WHITE_C);	
				m_pGrid.GridCellText(Grid, row, 3, "");
				
				m_pGrid.GridCellColor(Grid, row, 4, TEXT_BC, BLACK_C);	
				m_pGrid.GridCellText(Grid, row, 4, "");

				row = 5;
				m_pGrid.GridCellColor(Grid, row, 1, BLUE_D, WHITE_C);	
				m_pGrid.GridCellText(Grid, row, 1, "Work Run Time");
				m_pGrid.GridCellText(Grid, row, 1, "");
				
				strTmp.Format("%3.2f (Sec)", (float)(dTime[1][0] / (long)1000));
				m_pGrid.GridCellColor(Grid, row, 2, RED_L, BLACK_C);	
				m_pGrid.GridCellText(Grid, row, 2, strTmp);
				m_pGrid.GridCellText(Grid, row, 2, "");
				
				m_pGrid.GridCellColor(Grid, row, 3, BLUE_D, WHITE_C);	
				m_pGrid.GridCellText(Grid, row, 3, "ByPass Run Time");
				m_pGrid.GridCellText(Grid, row, 3, "");
				
				strTmp.Format("%3.2f (Sec)", (float)(dTime[1][1] / (long)1000));
				m_pGrid.GridCellColor(Grid, row, 4, TEXT_BC, BLACK_C);	
				m_pGrid.GridCellText(Grid, row, 4, strTmp);
				m_pGrid.GridCellText(Grid, row, 4, "");
				
				row = 6;
				m_pGrid.GridCellColor(Grid, row, 1, BLUE_D, WHITE_C);	
				m_pGrid.GridCellText(Grid, row, 1, "Work Stop Time");
				m_pGrid.GridCellText(Grid, row, 1, "");
				
				strTmp.Format("%3.2f (Sec)", (float)(dTime[0][0] / (long)1000));
				m_pGrid.GridCellColor(Grid, row, 2, RED_L, BLACK_C);	
				m_pGrid.GridCellText(Grid, row, 2, strTmp);
				m_pGrid.GridCellText(Grid, row, 2, "");
				
				m_pGrid.GridCellColor(Grid, row, 3, BLUE_D, WHITE_C);	
				m_pGrid.GridCellText(Grid, row, 3, "ByPass Stop Time");
				m_pGrid.GridCellText(Grid, row, 3, "");
				
				strTmp.Format("%3.2f (Sec)", (float)(dTime[0][1] / (long)1000));
				m_pGrid.GridCellColor(Grid, row, 4, TEXT_BC, BLACK_C);	
				m_pGrid.GridCellText(Grid, row, 4, strTmp);
				m_pGrid.GridCellText(Grid, row, 4, "");
				
				row = 7;
				m_pGrid.GridCellColor(Grid, row, 1, BLUE_D, WHITE_C);	
				m_pGrid.GridCellText(Grid, row, 1, "Work Alarm Time");
				m_pGrid.GridCellText(Grid, row, 1, "");
				
				strTmp.Format("%3.2f (Sec)", (float)(dTime[5][0] / (long)1000));
				m_pGrid.GridCellColor(Grid, row, 2, RED_L, BLACK_C);	
				m_pGrid.GridCellText(Grid, row, 2, strTmp);
				m_pGrid.GridCellText(Grid, row, 2, "");
				
				m_pGrid.GridCellColor(Grid, row, 3, BLUE_D, WHITE_C);	
				m_pGrid.GridCellText(Grid, row, 3, "ByPass Alarm Time");
				m_pGrid.GridCellText(Grid, row, 3, "");
				
				strTmp.Format("%3.2f (Sec)", (float)(dTime[5][1] / (long)1000));
				m_pGrid.GridCellColor(Grid, row, 4, TEXT_BC, BLACK_C);	
				m_pGrid.GridCellText(Grid, row, 4, strTmp);
				m_pGrid.GridCellText(Grid, row, 4, "");
			}	
		}
	}
	Mysql.MySql_Close();
}

void CScreen_List_Data_View::Init_Time()
{
	TSpread *Grid; 
	int row;
	int i;
	Grid = (TSpread*)GetDlgItem(IDC_CUSTOM_DEVICE);
	
	m_pGrid.GridReset(Grid);
	// 대문자 
	m_pGrid.GridCellSelectDisplay(Grid, TRUE);
	m_pGrid.GridRowHeader(Grid, FALSE);
	m_pGrid.GridColHeader(Grid, TRUE);
	m_pGrid.GridHorizontal(Grid, TRUE);
	m_pGrid.GridVertical(Grid, TRUE);
	m_pGrid.GridAutoSize(Grid, FALSE);
	
	m_pGrid.GridCellRows(Grid, 1);
	m_pGrid.GridCellCols(Grid, 6);
	
	
	m_pGrid.GridCellHeight(Grid, 0, 20);
	
	for (i = 0; i<6; i++)
	{
		m_pGrid.GridCellWidth(Grid, i+1, 25);
		
		m_pGrid.GridCellControlStatic(Grid, 0, i+1);
		m_pGrid.GridCellFont(Grid, 0, i+1, "MS Sans Serif", 18);
	}
	
	row =0;
	m_pGrid.GridCellColor(Grid, row, 1, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 1, "Date");
	
	m_pGrid.GridCellColor(Grid, row, 2, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 2, "Lot no");
	
	m_pGrid.GridCellColor(Grid, row, 3, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 3, "Part No");
	
	m_pGrid.GridCellColor(Grid, row, 4, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 4, "Time Section");
	
	m_pGrid.GridCellColor(Grid, row, 5, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 5, "Time");
	
	m_pGrid.GridCellColor(Grid, row, 6, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 6, "Mode");
}

void CScreen_List_Data_View::Display_Time(int count)
{
	TSpread *Grid; 
	int i;
	int length;
	int pos[2];
	
	CString strData, strTmp[7];
	
	Grid = (TSpread*)GetDlgItem(IDC_CUSTOM_DEVICE);
	
	m_pGrid.GridCellRows(Grid, count+1);
	m_pGrid.GridCellCols(Grid, 6);
	
	m_pGrid.GridCellHeight(Grid, count+1, 20);
	
	for (i = 0; i < 6; i++)
	{
		m_pGrid.GridCellWidth(Grid, i+1, 25);
		
		m_pGrid.GridCellControlStatic(Grid, count+1, i+1);
		m_pGrid.GridCellFont(Grid, count+1, i+1, "MS Sans Serif", 18);
	}
	
	strData = Mysql.Record_Read();
	// lot_no
	length = strData.GetLength();
	pos[0] = strData.Find(",", 0);
	
	for (i = 0; i < Mysql.mn_time_table_cnt; i++)
	{	
		pos[1] = strData.Find(",", pos[0]+1);
		
		if (pos[1] > 0)
		{
			strTmp[i]= strData.Mid(pos[0]+1, (pos[1] - pos[0])-1);
			pos[0] = pos[1];
		}
		else
		{
			strTmp[i] = strData.Mid(pos[0]+1, (length - pos[1])-1);
		}
	}
	
	m_pGrid.GridCellText(Grid, count+1, 1, strTmp[0]);
	m_pGrid.GridCellText(Grid, count+1, 2, strTmp[1]);
	m_pGrid.GridCellText(Grid, count+1, 3, strTmp[2]);
	m_pGrid.GridCellText(Grid, count+1, 4, strTmp[3]);
	m_pGrid.GridCellText(Grid, count+1, 5, strTmp[4]);
	m_pGrid.GridCellText(Grid, count+1, 6, strTmp[5]);

	// 3는 타임구분, 4는 타임, 5은 Work냐 ByPass냐.
	dTime[atoi(strTmp[3])][atoi(strTmp[5])] += atol(strTmp[4]);	
}

void CScreen_List_Data_View::Display_Ground_Date(int count, CString strtime)
{
	TSpread *Grid; 
	CString strTmp;
	
	Grid = (TSpread*)GetDlgItem(IDC_CUSTOM_LOT);
	
	m_pGrid.GridCellRows(Grid, count+2);
	m_pGrid.GridCellCols(Grid, 2);
	
	m_pGrid.GridCellHeight(Grid, count+2, 20);
	m_pGrid.GridCellWidth(Grid, 1, 10);
	m_pGrid.GridCellWidth(Grid, 2, 34);
	
	m_pGrid.GridCellControlStatic(Grid, count+2, 1);
	m_pGrid.GridCellFont(Grid, count+2, 1, "MS Sans Serif", 20);
	m_pGrid.GridCellColor(Grid, count+2, 1, YELLOW_L, BLACK_C);	
	strTmp.Format("%d", count+1);
	m_pGrid.GridCellText(Grid, count+2, 1, strTmp);
	
	m_pGrid.GridCellControlStatic(Grid, count+2, 2);
	m_pGrid.GridCellFont(Grid, count+2, 2, "MS Sans Serif", 12);
	m_pGrid.GridCellColor(Grid, count+2, 2, WHITE_C, BLUE_D);	
	m_pGrid.GridCellText(Grid, count+2, 2, strtime);
}

void CScreen_List_Data_View::OnBtnSearchAlarm() 
{
	int year_s, year_e, month_s, month_e, day_s, day_e;
	int hour_s, hour_e, min_s, min_e, sec_s, sec_e;
	long count = 0;
	int i;
	int pos[10] = {0,};
	CString temp;
	CString strTmp, strDate, strCode;
	CTime time;
//	TSpread *Grid;
//	int row;

	mn_view_type = 4;				// 4는 Alarm
	
	m_oledate[0] = m_pstart_date.GetDate();
	m_oledate[1] = m_pend_date.GetDate();

	m_oletime[0] = m_pstart_time.GetDate();
	m_oletime[1] = m_pend_time.GetDate();

	year_s		= m_oledate[0].GetYear();
	month_s		= m_oledate[0].GetMonth();
	day_s		= m_oledate[0].GetDay();

	hour_s		= m_oletime[0].GetHour();
	min_s		= m_oletime[0].GetMinute();
	sec_s		= m_oletime[0].GetSecond();

	year_e		= m_oledate[1].GetYear();
	month_e		= m_oledate[1].GetMonth();
	day_e		= m_oledate[1].GetDay();
	
	hour_e		= m_oletime[1].GetHour();
	min_e		= m_oletime[1].GetMinute();
	sec_e		= m_oletime[1].GetSecond();

	Init_Data();	
	Init_Alarm();

	CString strFuncRet = OnDateCheck(year_s, month_s, day_s, hour_s, min_s, sec_s, year_e, month_e, day_e, hour_e, min_e, sec_e);
	
	if (strFuncRet != "NONE")
	{
		Func.MsgLog(strFuncRet);
		return;
	}

	m_oledate[0].SetDateTime(year_s, month_s, day_s, hour_s, 0, 0);
	m_oledate[1].SetDateTime(year_e, month_e, day_e, hour_e, 0, 0);	

	if (Mysql.MySql_Open("localhost", "root", "1111", 3306))
	{
		if (Mysql.Database_Select("AMT8530"))
		{	
//						Mysql.Table_Delete("ALARM_DATA");				// 테이블 리셋용

			count = Mysql.Record_Date_Search("ALARM_DATA", "DATE", "<>", m_oledate);
			
			if (count > 0)
			{
				Init_Lot();
				Init_Alarm();

				for (i = 0; i<count; i++)
				{
					Display_Alarm(i);
				}
			}	
		}
	}
	Mysql.MySql_Close();
}

void CScreen_List_Data_View::Init_Alarm()
{
	TSpread *Grid; 
	int row;
	int i;
	Grid = (TSpread*)GetDlgItem(IDC_CUSTOM_DEVICE);
	
	m_pGrid.GridReset(Grid);
	// 대문자 
	m_pGrid.GridCellSelectDisplay(Grid, TRUE);
	m_pGrid.GridRowHeader(Grid, FALSE);
	m_pGrid.GridColHeader(Grid, TRUE);
	m_pGrid.GridHorizontal(Grid, TRUE);
	m_pGrid.GridVertical(Grid, TRUE);
	m_pGrid.GridAutoSize(Grid, FALSE);
	
	m_pGrid.GridCellRows(Grid, 1);
	m_pGrid.GridCellCols(Grid, 3);
	
	
	m_pGrid.GridCellHeight(Grid, 0, 20);
	
	for (i = 0; i<5; i++)
	{
		if (i == 0)
		{
			m_pGrid.GridCellWidth(Grid, i+1, 30);
		} 
		else
		{
			m_pGrid.GridCellWidth(Grid, i+1, 60);
		}
		
		m_pGrid.GridCellControlStatic(Grid, 0, i+1);
		m_pGrid.GridCellFont(Grid, 0, i+1, "MS Sans Serif", 18);
	}
	
	
	row =0;
	m_pGrid.GridCellColor(Grid, row, 1, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 1, "Date");
	
	m_pGrid.GridCellColor(Grid, row, 2, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 2, "Alarm Code");
	
	m_pGrid.GridCellColor(Grid, row, 3, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 3, "Alarm Msg");
}

void CScreen_List_Data_View::Display_Alarm(int count)
{
	TSpread *Grid; 
	int i;
	int length;
	int pos[2];
	
	CString strData, strTmp[9];
	
	Grid = (TSpread*)GetDlgItem(IDC_CUSTOM_DEVICE);
	
	m_pGrid.GridCellRows(Grid, count+1);
	m_pGrid.GridCellCols(Grid, 3);
	
	m_pGrid.GridCellHeight(Grid, count+1, 20);
	
	for (i = 0; i < 5; i++)
	{
		if (i == 0)
		{
			m_pGrid.GridCellWidth(Grid, i+1, 30);
		} 
		else if (i == 1 || i == 2)
		{
			m_pGrid.GridCellWidth(Grid, i+1, 30);
		}
		else if (i == 3)
		{
			m_pGrid.GridCellWidth(Grid, i+1, 17);
		}
		else
		{
			m_pGrid.GridCellWidth(Grid, i+1, 100);
		}
		
		m_pGrid.GridCellControlStatic(Grid, count+1, i+1);
		m_pGrid.GridCellFont(Grid, count+1, i+1, "MS Sans Serif", 18);
	}
	
	strData = Mysql.Record_Read();
	// lot_no
	length = strData.GetLength();
	pos[0] = strData.Find(",", 0);
	
	for (i = 0; i < Mysql.mn_alarm_table_cnt; i++)
	{	
		pos[1] = strData.Find(",", pos[0]+1);
		
		if (pos[1] > 0)
		{
			strTmp[i]= strData.Mid(pos[0]+1, (pos[1] - pos[0])-1);
			pos[0] = pos[1];
		}
		else
		{
			strTmp[i] = strData.Mid(pos[0]+1, (length - pos[1])-1);
		}
	}
	
	m_pGrid.GridCellText(Grid, count+1, 1, strTmp[0]);
	m_pGrid.GridCellText(Grid, count+1, 2, strTmp[3]);
	m_pGrid.GridCellText(Grid, count+1, 3, strTmp[4]);
}

void CScreen_List_Data_View::OnRadioAll() 
{
	nSearchMethod = 2;
	OnData_Search_Change();
}

void CScreen_List_Data_View::OnRadioBoard() 
{
	nSearchMethod = 1;	
	OnData_Search_Change();
}

void CScreen_List_Data_View::OnRadioLot() 
{
	nSearchMethod = 0;	
	OnData_Search_Change();
}

void CScreen_List_Data_View::Init_BoardOff()
{
	TSpread *Grid; 
	int row;
	int i;
	CString strTemp;
	Grid = (TSpread*)GetDlgItem(IDC_CUSTOM_DEVICE);
	
	m_pGrid.GridReset(Grid);
	// 대문자 
	m_pGrid.GridCellSelectDisplay(Grid, TRUE);
	m_pGrid.GridRowHeader(Grid, FALSE);
	m_pGrid.GridColHeader(Grid, TRUE);
	m_pGrid.GridHorizontal(Grid, TRUE);
	m_pGrid.GridVertical(Grid, TRUE);
	m_pGrid.GridAutoSize(Grid, FALSE);
	
	m_pGrid.GridCellRows(Grid, 1);
	m_pGrid.GridCellCols(Grid, 4);
	
	
	m_pGrid.GridCellHeight(Grid, 0, 20);
	
	for (i = 0; i<4; i++)
	{
		m_pGrid.GridCellWidth(Grid, i+1, 25);
		
		m_pGrid.GridCellControlStatic(Grid, 0, i+1);
		m_pGrid.GridCellFont(Grid, 0, i+1, "MS Sans Serif", 18);
	}
	
	row =0;
	m_pGrid.GridCellColor(Grid, row, 1, BLUE_D, WHITE_C);
	m_pGrid.GridCellText(Grid, row, 1, "Date");
	
	m_pGrid.GridCellColor(Grid, row, 1, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 1, "Date");
	
	m_pGrid.GridCellColor(Grid, row, 2, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 2, "Board No");
	
	m_pGrid.GridCellColor(Grid, row, 3, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 3, "Board Pos");
	
	m_pGrid.GridCellColor(Grid, row, 4, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 4, "Board Live Info");
}

void CScreen_List_Data_View::OnCellClick(WPARAM wParam, LPARAM lParam)
{
	LPSS_CELLCOORD lpcc = (LPSS_CELLCOORD)lParam;

	TSpread *Grid;
	CString strTmp, strData;
	int count, i, row;

	BOOL bShowDetailList = IsDlgButtonChecked(IDC_CHECK_SHOW_DETAIL_LIST);

	if (wParam==IDC_CUSTOM_LOT)
	{	
		if (lpcc->Col == 1 || lpcc->Row == 1) return;

		md_pcb_test_count = 0;			// PCB Base Test Count
		md_pcb_good_count = 0;			// PCB Base Good Count
		md_pcb_yield = 0;				// PCB Base Yield
		
		Grid = (TSpread*)GetDlgItem(IDC_CUSTOM_LOT);

		strTmp = m_pGrid.GridCellText(Grid, lpcc->Row, lpcc->Col);

		if (mn_view_type == 7)		// Lot 정보는 PCB일때만 보여준다.
		{
			Init_Device();

			if (Mysql.MySql_Open("localhost", "root", "1111", 3306))
			{
				if (Mysql.Database_Select("AMT8530"))
				{	
					if (nSearchMethod == 0)
					{
						count = Mysql.Record_Text_Search("SCRAP_DATA", "LOT_NO", strTmp);
					}
					else if (nSearchMethod == 1)
					{
						count = Mysql.Record_Text_Search("SCRAP_DATA", "SCRAP_CODE", strTmp);
					}

					if (count > 0)
					{
//						Display_Count();
					}	
				}
			}
			Mysql.MySql_Close();

			if (Mysql.MySql_Open("localhost", "root", "1111", 3306))
			{
				if (Mysql.Database_Select("AMT8530"))
				{	
					if (nSearchMethod == 0)
					{
						count = Mysql.Record_Text_Search("SCRAP_DATA", "LOT_NO", strTmp);
					}
					else if (nSearchMethod == 1)
					{
						count = Mysql.Record_Text_Search("SCRAP_DATA", "SCRAP_CODE", strTmp);
					}
					
					if (count > 0)
					{
						count = Mysql.Record_DateLot_Search(strTmp, "SCRAP_DATA", "DATE", "<>", m_oledate, nSearchMethod);

						if (count > 0)
						{
							for (i = 0; i<count; i++)
							{
								Display_Scrap(i);
							}

							// 해당 결과에 대한 정리를 한다.
							Grid = (TSpread*)GetDlgItem(IDC_CUSTOM_DATA);
							
							row = 6;
// 							m_pGrid.GridCellColor(Grid, row, 1, BLUE_D, WHITE_C);	
// 							m_pGrid.GridCellText(Grid, row, 1, "Test QTY");
				
							strTmp.Format("%d", (int)md_pcb_test_count);
							m_pGrid.GridCellColor(Grid, row, 2, RED_L, BLACK_C);	
							m_pGrid.GridCellText(Grid, row, 2, strTmp);
							
// 							m_pGrid.GridCellColor(Grid, row, 3, BLUE_D, WHITE_C);	
// 							m_pGrid.GridCellText(Grid, row, 3, "Test Yield");
							
							md_pcb_yield = (md_pcb_good_count / md_pcb_test_count) * 100;
							strTmp.Format("%3.2f %%", (float)md_pcb_yield);
							m_pGrid.GridCellColor(Grid, row, 4, TEXT_BC, BLACK_C);	
							m_pGrid.GridCellText(Grid, row, 4, strTmp);
							
							row = 7;
// 							m_pGrid.GridCellColor(Grid, row, 1, BLUE_D, WHITE_C);	
// 							m_pGrid.GridCellText(Grid, row, 1, "Good Count");
							
							strTmp.Format("%d", (int)md_pcb_good_count);
							m_pGrid.GridCellColor(Grid, row, 2, RED_L, BLACK_C);	
							m_pGrid.GridCellText(Grid, row, 2, strTmp);
							
// 							m_pGrid.GridCellColor(Grid, row, 3, BLUE_D, WHITE_C);	
// 							m_pGrid.GridCellText(Grid, row, 3, "Fail Count");
							
							strTmp.Format("%d", (int)(md_pcb_test_count - md_pcb_good_count));
							m_pGrid.GridCellColor(Grid, row, 4, TEXT_BC, BLACK_C);	
							m_pGrid.GridCellText(Grid, row, 4, strTmp);

						}	
					}
				}
			}
			Mysql.MySql_Close();
		}
		else if (mn_view_type == 2)		// 그라운드 일때는 시간대별 그라운드 상황을 보여준다.
		{
			if (Mysql.MySql_Open("localhost", "root", "1111", 3306))
			{
				if (Mysql.Database_Select("AMT8530"))
				{	
					count = Mysql.Record_Text_Search("GROUND_DATA", "DATE", strTmp);
					
					if (count > 0)
					{
						for (i = 0; i<count; i++)
						{
//							Display_Ground(i);
						}
					}
				}
			}
			Mysql.MySql_Close();
		}
	}
}

void CScreen_List_Data_View::OnBtnSearchGround() 
{
	int year_s, year_e, month_s, month_e, day_s, day_e;
	int hour_s, hour_e, min_s, min_e, sec_s, sec_e;
	long count = 0;
	int i;
	CString temp;
	CString strTmp, strDate;
	CTime time;

	mn_view_type = 2;				// 2는 그라운드.

	for (i = 0; i < 5; i++)
	{
		md_high_val_data[i] = -1;
		mn_high_val_pos[i] = -1;
	}
	
	m_oledate[0] = m_pstart_date.GetDate();
	m_oledate[1] = m_pend_date.GetDate();

	m_oletime[0] = m_pstart_time.GetDate();
	m_oletime[1] = m_pend_time.GetDate();

	year_s		= m_oledate[0].GetYear();
	month_s		= m_oledate[0].GetMonth();
	day_s		= m_oledate[0].GetDay();

	hour_s		= m_oletime[0].GetHour();
	min_s		= m_oletime[0].GetMinute();
	sec_s		= m_oletime[0].GetSecond();

	year_e		= m_oledate[1].GetYear();
	month_e		= m_oledate[1].GetMonth();
	day_e		= m_oledate[1].GetDay();
	
	hour_e		= m_oletime[1].GetHour();
	min_e		= m_oletime[1].GetMinute();
	sec_e		= m_oletime[1].GetSecond();

	Init_Data();
	Init_Ground();

	CString strFuncRet = OnDateCheck(year_s, month_s, day_s, hour_s, min_s, sec_s, year_e, month_e, day_e, hour_e, min_e, sec_e);
	
	if (strFuncRet != "NONE")
	{
		Func.MsgLog(strFuncRet);
		return;
	}

	m_oledate[0].SetDateTime(year_s, month_s, day_s, hour_s, 0, 0);
	m_oledate[1].SetDateTime(year_e, month_e, day_e, hour_e, 0, 0);

	if (Mysql.MySql_Open("localhost", "root", "1111", 3306))
	{
		if (Mysql.Database_Select("AMT8530"))
		{	
			count = Mysql.Record_Date_Search("GROUND_DATA", "DATE", "<>", m_oledate);
			
			if (count > 0)
			{
				Init_Lot();
				Init_Ground();

				for (i = 0; i<count; i++)
				{
					Display_Ground(i);
				}
			}	
		}
	}
	Mysql.MySql_Close();

	// 써치가 끝나면 가장 큰수(최근) 자리에 색을 칠한다.
	TSpread *Grid;
	Grid = (TSpread*)GetDlgItem(IDC_CUSTOM_DEVICE);

	for (i = 0; i < 5; i++)
	{
		if (md_high_val_data[i] > st_basic.md_electrostatic_value)
		{
			m_pGrid.GridCellColor(Grid, mn_high_val_pos[i]+1, i+2, RED_C, YELLOW_C);
		}
		else
		{
			m_pGrid.GridCellColor(Grid, mn_high_val_pos[i]+1, i+2, YELLOW_C, BLACK_C);
		}
	}	
}

void CScreen_List_Data_View::Init_Ground()
{
	TSpread *Grid; 
	int row;
	int i;
	Grid = (TSpread*)GetDlgItem(IDC_CUSTOM_DEVICE);
	
	m_pGrid.GridReset(Grid);
	// 대문자 
	m_pGrid.GridCellSelectDisplay(Grid, TRUE);
	m_pGrid.GridRowHeader(Grid, FALSE);
	m_pGrid.GridColHeader(Grid, TRUE);
	m_pGrid.GridHorizontal(Grid, TRUE);
	m_pGrid.GridVertical(Grid, TRUE);
	m_pGrid.GridAutoSize(Grid, FALSE);
	
	m_pGrid.GridCellRows(Grid, 1);
	m_pGrid.GridCellCols(Grid, 6);
	
	
	m_pGrid.GridCellHeight(Grid, 0, 20);
	
	for (i = 0; i<6; i++)
	{
		m_pGrid.GridCellWidth(Grid, i+1, 25);
		
		m_pGrid.GridCellControlStatic(Grid, 0, i+1);
		m_pGrid.GridCellFont(Grid, 0, i+1, "MS Sans Serif", 18);
	}
	
	
	row =0;
	m_pGrid.GridCellColor(Grid, row, 1, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 1, "Date");
	
	m_pGrid.GridCellColor(Grid, row, 2, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 2, "Ch 1 (Robot Z)");
	
	m_pGrid.GridCellColor(Grid, row, 3, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 3, "Ch 2 (LOB Buffer 1)");
	
	m_pGrid.GridCellColor(Grid, row, 4, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 4, "Ch 3 (LOB Buffer 2)");
	
	m_pGrid.GridCellColor(Grid, row, 5, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 5, "Ch 4 (Reject Buffer)");
	
	m_pGrid.GridCellColor(Grid, row, 6, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 6, "Ch 5 (Retest Buffer)");
}

void CScreen_List_Data_View::Display_Ground(int count)
{
	TSpread *Grid; 
	int i;
	int length;
	int pos[2];
	
	CString strData, strTmp[7];
	
	//	Init_Ground();
	
	Grid = (TSpread*)GetDlgItem(IDC_CUSTOM_DEVICE);
	
	m_pGrid.GridCellRows(Grid, count+1);
	m_pGrid.GridCellCols(Grid, 6);
	
	m_pGrid.GridCellHeight(Grid, count+1, 20);
	
	for (i = 0; i < 6; i++)
	{
		m_pGrid.GridCellWidth(Grid, i+1, 25);
		
		m_pGrid.GridCellControlStatic(Grid, count+1, i+1);
		m_pGrid.GridCellFont(Grid, count+1, i+1, "MS Sans Serif", 18);
	}
	
	strData = Mysql.Record_Read();
	// lot_no
	length = strData.GetLength();
	pos[0] = strData.Find(",", 0);
	
	for (i = 0; i < Mysql.mn_ground_table_cnt; i++)
	{	
		pos[1] = strData.Find(",", pos[0]+1);
		
		if (pos[1] > 0)
		{
			strTmp[i]= strData.Mid(pos[0]+1, (pos[1] - pos[0])-1);
			pos[0] = pos[1];
		}
		else
		{
			strTmp[i] = strData.Mid(pos[0]+1, (length - pos[1])-1);
		}
	}
	
	// 가장 큰 수를 저장한다.
	for (i = 0; i < 5; i ++)
	{
		if (md_high_val_data[i] <= atof(strTmp[i+1]))
		{
			md_high_val_data[i] = atof(strTmp[i+1]);
			mn_high_val_pos[i] = count;
		}
	}
	
	m_pGrid.GridCellText(Grid, count+1, 1, strTmp[0]);
	m_pGrid.GridCellText(Grid, count+1, 2, strTmp[1]);
	m_pGrid.GridCellText(Grid, count+1, 3, strTmp[2]);
	m_pGrid.GridCellText(Grid, count+1, 4, strTmp[3]);
	m_pGrid.GridCellText(Grid, count+1, 5, strTmp[4]);
	m_pGrid.GridCellText(Grid, count+1, 6, strTmp[5]);
	
	for (i = 1; i < 6; i ++)
	{
		if (st_basic.md_electrostatic_value <= atof(strTmp[i]))			// 기준값보다 크다면?
		{
			m_pGrid.GridCellColor(Grid, count+1, i, RED_C, WHITE_C);
		}
	}
}

void CScreen_List_Data_View::Init_Board()
{
	TSpread *Grid; 
	int row;
	int i;
	Grid = (TSpread*)GetDlgItem(IDC_CUSTOM_DEVICE);
	
	m_pGrid.GridReset(Grid);
	// 대문자 
	m_pGrid.GridCellSelectDisplay(Grid, TRUE);
	m_pGrid.GridRowHeader(Grid, FALSE);
	m_pGrid.GridColHeader(Grid, TRUE);
	m_pGrid.GridHorizontal(Grid, TRUE);
	m_pGrid.GridVertical(Grid, TRUE);
	m_pGrid.GridAutoSize(Grid, FALSE);
	
	m_pGrid.GridCellRows(Grid, 1);
	m_pGrid.GridCellCols(Grid, 14);
	
	
	m_pGrid.GridCellHeight(Grid, 0, 20);
	
	for (i = 0; i < 14; i++)
	{
		if (i == 0)
		{
			m_pGrid.GridCellWidth(Grid, i+1, 30);
		}
		else
		{
			m_pGrid.GridCellWidth(Grid, i+1, 17);
		}
		
		m_pGrid.GridCellControlStatic(Grid, 0, i+1);
		m_pGrid.GridCellFont(Grid, 0, i+1, "MS Sans Serif", 18);
	}
	
	
	row = 0;
	m_pGrid.GridCellColor(Grid, row, 1, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 1, "LOT_NO");
	
	m_pGrid.GridCellColor(Grid, row, 2, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 2, "BOARD_NO");
	
	m_pGrid.GridCellColor(Grid, row, 3, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 3, "BOARD_POS");
	
	m_pGrid.GridCellColor(Grid, row, 4, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 4, "START_DATE");
	
	m_pGrid.GridCellColor(Grid, row, 5, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 5, "END_DATE");
	
	m_pGrid.GridCellColor(Grid, row, 6, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 6, "TEST_TIME");
	
	m_pGrid.GridCellColor(Grid, row, 7, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 7, "#1 Serial");
	
	m_pGrid.GridCellColor(Grid, row, 8, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 8, "#2 Serial");
	
	m_pGrid.GridCellColor(Grid, row, 9, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 9, "#1 Arrary Info");

	m_pGrid.GridCellColor(Grid, row, 10, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 10, "#2 Arrary Info");
	
	m_pGrid.GridCellColor(Grid, row, 11, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 11, "#1 Result");
	
	m_pGrid.GridCellColor(Grid, row, 12, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 12, "#2 Result");
	
	m_pGrid.GridCellColor(Grid, row, 13, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 13, "#1 Test Count");
	
	m_pGrid.GridCellColor(Grid, row, 14, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 14, "#2 Test Count");
}

CString CScreen_List_Data_View::OnDateCheck(int nYearS, int nMonthS, int nDayS, int nHourS, int nMinS, int nSecS, int nYearE, int nMonthE, int nDayE, int nHourE, int nMinE, int nSecE)
{
	CString strRetFunc = "NONE";

	if (nYearS > nYearE)
	{
		strRetFunc = "시작 날짜, 종료 날짜를 다시 설정하여 주십시요.";
		if (st_handler.mn_language == LANGUAGE_ENGLISH) strRetFunc = "Start date, end date, please re-set.";	// 20130926 kjh english
		return strRetFunc;
	}
	
	if (nYearS == nYearE)
	{
		if (nMonthS > nMonthE)
		{
			strRetFunc = "시작 날짜, 종료 날짜를 다시 설정하여 주십시요.";
			if (st_handler.mn_language == LANGUAGE_ENGLISH) strRetFunc = "Start date, end date, please re-set.";	// 20130926 kjh english
			return strRetFunc;
		}
		
		if (nMonthS == nMonthE)
		{
			if (nDayS > nDayE)
			{
				strRetFunc = "시작 날짜, 종료 날짜를 다시 설정하여 주십시요.";
				if (st_handler.mn_language == LANGUAGE_ENGLISH) strRetFunc = "Start date, end date, please re-set.";	// 20130926 kjh english
				return strRetFunc;
			}
			
			if (nDayS == nDayE)
			{
				if (nHourS >= nHourE)
				{
					strRetFunc = "시작 날짜, 종료 날짜를 다시 설정하여 주십시요.";
					if (st_handler.mn_language == LANGUAGE_ENGLISH) strRetFunc = "Start date, end date, please re-set.";	// 20130926 kjh english
					return strRetFunc;
				}
			}
		}
	}
	
	return strRetFunc;
}

void CScreen_List_Data_View::OnBtnSave() 
{
	char path[MAX_PATH] = {0};
	//현재의 작업경로를 얻어와 저장 한다.
	GetCurrentDirectory(MAX_PATH, path);
	
	CString strSheetName;
	
	CFileDialogST	dlg(FALSE, NULL, mstr_file_name, OFN_FILEMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST, _T("All files\0*.*\0"), this);
	CString			sPathName, str_temp_file, str_ext, str_chk_ext, sTxtName;
	int				nRetValue, n_pos;
	
	TSpread *Grid; 
	
	Grid = (TSpread*)GetDlgItem(IDC_CUSTOM_DEVICE);
	
	switch (mn_view_type)
	{
	case 1:
		strSheetName = "PCB";
		break;
	case 2:
		strSheetName = "Ground";
		break;
	case 3:
		strSheetName = "BOARD";
		break;
	case 4:
		strSheetName = "ALARM";
		break;
	case 5:
		strSheetName = "Board Off";
		break;
	case 6:
		strSheetName = "Time";
		Grid = NULL;

		Grid = (TSpread*)GetDlgItem(IDC_CUSTOM_DATA);
		break;
	default:
		return;
		break;
	}
	
	nRetValue = dlg.DoModal();
	
	if (nRetValue == IDOK)
	{
		sPathName = dlg.GetPathName();
		
		n_pos = mstr_file_full_name.Find(".");			// 원본 파일의 확장자 위치 검사
		
		if (n_pos == -1)								// 복사본 확장자 미존재
		{
			str_ext = ".xls";
		}
		else 
		{		
			str_ext = mstr_file_full_name.Mid(n_pos);	// 원본 파일 확장자 설정
		}
		
		n_pos = sPathName.Find(".");					// 복사본 확장자 위치 검사
		
		if (n_pos == -1)								// 복사본 확장자 미존재
		{
			sTxtName = sPathName + ".txt";
			sPathName += _T(str_ext);					// 복사본 확장자 추가
		}
		
		Grid->ExportToTextFile(sTxtName, "", "\t", "\r\n", 8 || 1, "");
		Grid->ExportToExcel(sPathName, strSheetName, "default.log");
		
		SetCurrentDirectory(path);						// 작업 경로를 원래 경로로 복구 한다.
	}

	Grid = NULL;
	delete Grid;
}

void CScreen_List_Data_View::OnBtnDayPrev() 
{
	COleDateTime Stime;
	COleDateTime Etime;

	Stime = m_pstart_date.GetDate();
	Etime = m_pend_date.GetDate();

	Stime = Stime + COleDateTimeSpan(-1, 0, 0, 0);
	Etime = Etime + COleDateTimeSpan(-1, 0, 0, 0);
	
	m_pstart_date.SetDate(Stime);
	m_pend_date.SetDate(Etime);
}

void CScreen_List_Data_View::OnBtnDayNext() 
{
	COleDateTime Stime;
	COleDateTime Etime;

	Stime = m_pstart_date.GetDate();
	Etime = m_pend_date.GetDate();

	Stime = Stime + COleDateTimeSpan(1, 0, 0, 0);
	Etime = Etime + COleDateTimeSpan(1, 0, 0, 0);
	
	m_pstart_date.SetDate(Stime);
	m_pend_date.SetDate(Etime);
}

void CScreen_List_Data_View::OnBtnDayToday() 
{
	COleDateTime time;
	COleDateTime Stime;
	COleDateTime Etime;
	
	time = COleDateTime::GetCurrentTime();
	
	if (time.GetHour() >= 22)			// 현재 시간이 22보다 크면...
	{
		Stime = time + COleDateTimeSpan(0, 0, 0, 0);				// 오늘
		Etime = time + COleDateTimeSpan(+1, 0, 0, 0);				// 내일
	}
	else
	{
		Stime = time + COleDateTimeSpan(-1, 0, 0, 0);				// 어제
		Etime = time + COleDateTimeSpan(0, 0, 0, 0);				// 오늘
	}
	
	m_pstart_date.SetDate(Stime);
	m_pend_date.SetDate(Etime);
}

void CScreen_List_Data_View::OnBtnView() 
{
	mn_socket_view_mode = 0;		// All
	
	switch (mn_view_type)
	{
	case 1:
		break;
		
	case 3:
		Display_Socket_Detail();
		break;
		
	case 4:
		break;
		
	case 5:
		break;
	}
}

void CScreen_List_Data_View::OnBtnViewPrime() 
{
	mn_socket_view_mode = 1;			// Prime
	
	switch (mn_view_type)
	{
	case 1:
		break;
		
	case 3:
		Display_Socket_Detail();
		break;
		
	case 4:
		break;
		
	case 5:
		break;
	}
}

void CScreen_List_Data_View::OnBtnViewRetest() 
{
	mn_socket_view_mode = 2;			// Retest
	
	switch (mn_view_type)
	{
	case 1:
		break;
		
	case 3:
		Display_Socket_Detail();
		break;
		
	case 4:
		break;
		
	case 5:
		break;
	}
}

void CScreen_List_Data_View::Display_Socket_Detail()
{
	TSpread *Grid; 
	int row = 82;
	int i, j, nTotalCount = 0;
	int nBinCount[5] = {0,};
	int nBoardCount[80] = {0,};
	float fBinYield[5] = {0,};
	float fBoardYield[80] = {0,};

	int nTotalGood = 0;
	float fTotalYield = 0;

	int nBinTotal = 0;
	int nSocketTotal[80] = {0,};
	int nSocketGood[80] = {0,};

	float fYield = 0;
	CString strTemp;
	Grid = (TSpread*)GetDlgItem(IDC_CUSTOM_DEVICE);
	
 	m_pGrid.GridReset(Grid);
	// 대문자 
	m_pGrid.GridCellSelectDisplay(Grid, TRUE);
	m_pGrid.GridRowHeader(Grid, FALSE);
	m_pGrid.GridColHeader(Grid, TRUE);
	m_pGrid.GridHorizontal(Grid, TRUE);
	m_pGrid.GridVertical(Grid, TRUE);
	m_pGrid.GridAutoSize(Grid, FALSE);
	
	m_pGrid.GridCellRows(Grid, 14);
	m_pGrid.GridCellCols(Grid, row);
	

	m_pGrid.GridCellHeight(Grid, 0, 20);
	
	for (i = 0; i<row; i++)
	{
		m_pGrid.GridCellWidth(Grid, i+1, 18.65);

		m_pGrid.GridCellControlStatic(Grid, 0, i+1);
		m_pGrid.GridCellFont(Grid, 0, i+1, "MS Sans Serif", 18);

		m_pGrid.GridCellHeight(Grid, i+1, 23);
	}

	for (i = 0; i<row; i++)
	{
		for (j = 0; j < 35; j++)
		{
			m_pGrid.GridCellControlStatic(Grid, j, i+1);
			m_pGrid.GridCellFont(Grid, j, i+1, "MS Sans Serif", 18);
		}
	}
	
	row = 0;
	m_pGrid.GridCellColor(Grid, row, 1, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 1, "Section");
	m_pGrid.GridCellFont(Grid, row, 1, "MS Sans Serif", 18);

	for (i = 0; i < 80; i++)
	{
		strTemp.Format("Socket #%02d", i + 1);
		m_pGrid.GridCellColor(Grid, row, 2 + i, BLUE_D, WHITE_C);	
		m_pGrid.GridCellText(Grid, row, 2 + i, strTemp);
		m_pGrid.GridCellFont(Grid, row, 2 + i, "MS Sans Serif", 18);
	}

	m_pGrid.GridCellColor(Grid, row, 82, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 82, "Total");
	m_pGrid.GridCellFont(Grid, row, 82, "MS Sans Serif", 18);

	row++;		// 1														// Save할때 맨 윗줄이 날라가기 때문에 한번 더 출력 시킨다.
	m_pGrid.GridCellColor(Grid, row, 1, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 1, "Section");
	m_pGrid.GridCellFont(Grid, row, 1, "MS Sans Serif", 18);
	
	for (i = 0; i < 80; i++)
	{
		strTemp.Format("Socket #%02d", i + 1);
		m_pGrid.GridCellColor(Grid, row, 2 + i, BLUE_D, WHITE_C);	
		m_pGrid.GridCellText(Grid, row, 2 + i, strTemp);
		m_pGrid.GridCellFont(Grid, row, 2 + i, "MS Sans Serif", 18);
	}

	m_pGrid.GridCellColor(Grid, row, 82, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 82, "Total");
	m_pGrid.GridCellFont(Grid, row, 82, "MS Sans Serif", 18);

	row++;		// 2
	m_pGrid.GridCellColor(Grid, row, 1, BLUE_D, WHITE_C);
	m_pGrid.GridCellText(Grid, row, 1, "PASS");
	m_pGrid.GridCellFont(Grid, row, 1, "MS Sans Serif", 18);

	row++;		// 3
	m_pGrid.GridCellColor(Grid, row, 1, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 1, "Yield");
	m_pGrid.GridCellFont(Grid, row, 1, "MS Sans Serif", 18);

	row++;		// 4
	m_pGrid.GridCellColor(Grid, row, 1, RED_C, WHITE_C);
	m_pGrid.GridCellText(Grid, row, 1, "NTO");
	m_pGrid.GridCellFont(Grid, row, 1, "MS Sans Serif", 18);
	
	row++;		// 5
	m_pGrid.GridCellColor(Grid, row, 1, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 1, "Yield");
	m_pGrid.GridCellFont(Grid, row, 1, "MS Sans Serif", 18);

	row++;		// 6
	m_pGrid.GridCellColor(Grid, row, 1, RED_C, WHITE_C);
	m_pGrid.GridCellText(Grid, row, 1, "BTO");
	m_pGrid.GridCellFont(Grid, row, 1, "MS Sans Serif", 18);
	
	row++;		// 7
	m_pGrid.GridCellColor(Grid, row, 1, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 1, "Yield");
	m_pGrid.GridCellFont(Grid, row, 1, "MS Sans Serif", 18);

	row++;		// 8
	m_pGrid.GridCellColor(Grid, row, 1, RED_C, WHITE_C);
	m_pGrid.GridCellText(Grid, row, 1, "HTO");
	m_pGrid.GridCellFont(Grid, row, 1, "MS Sans Serif", 18);
	
	row++;		// 9
	m_pGrid.GridCellColor(Grid, row, 1, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 1, "Yield");
	m_pGrid.GridCellFont(Grid, row, 1, "MS Sans Serif", 18);

	row++;		// 10
	m_pGrid.GridCellColor(Grid, row, 1, RED_C, WHITE_C);
	m_pGrid.GridCellText(Grid, row, 1, "FAIL");
	m_pGrid.GridCellFont(Grid, row, 1, "MS Sans Serif", 18);
	
	row++;		// 11
	m_pGrid.GridCellColor(Grid, row, 1, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 1, "Yield");
	m_pGrid.GridCellFont(Grid, row, 1, "MS Sans Serif", 18);

	row++;		// 12
	m_pGrid.GridCellColor(Grid, row, 1, RED_C, WHITE_C);
	m_pGrid.GridCellText(Grid, row, 1, "FAIL & NBH");
	m_pGrid.GridCellFont(Grid, row, 1, "MS Sans Serif", 18);
	
// 	row++;		// 13
// 	m_pGrid.GridCellColor(Grid, row, 1, BLUE_D, WHITE_C);	
// 	m_pGrid.GridCellText(Grid, row, 1, "Yield");
// 	m_pGrid.GridCellFont(Grid, row, 1, "MS Sans Serif", 18);

	row++;		// 13
	m_pGrid.GridCellColor(Grid, row, 1, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 1, "Total");
	m_pGrid.GridCellFont(Grid, row, 1, "MS Sans Serif", 18);

	row++;		// 14
	m_pGrid.GridCellColor(Grid, row, 1, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 1, "Yield");
	m_pGrid.GridCellFont(Grid, row, 1, "MS Sans Serif", 18);

// 	int mn_board_view_p[80][5];			// 보드 View에서 쓸 변수. [보드][빈] Prime  : 0-Pass, 1-NTO, 2-BTO, 3-HTO, 4-FAIL
// 	int mn_board_view_r[80][5];			// 보드 View에서 쓸 변수. [보드][빈] Retest
	// 보드 Display
	nTotalCount = 0;
	for (i = 0; i < 5; i++)
	{
		for (j = 0; j < 80; j++)
		{
			row = 2 + i * 2;

			switch(mn_socket_view_mode)
			{
			case 0:
				strTemp.Format("%d", mn_board_view_p[j][i] + mn_board_view_r[j][i]);			// Detail View용.
				m_pGrid.GridCellColor(Grid, row, j + 2, WHITE_C, BLUE_D);
				m_pGrid.GridCellText(Grid, row, j + 2, strTemp);
				nTotalCount += mn_board_view_p[j][i] + mn_board_view_r[j][i];
				break;
			case 1:
				strTemp.Format("%d", mn_board_view_p[j][i]);									// Detail View용.
				m_pGrid.GridCellColor(Grid, row, j + 2, WHITE_C, BLUE_D);
				m_pGrid.GridCellText(Grid, row, j + 2, strTemp);
				nTotalCount += mn_board_view_p[j][i];
				break;
			case 2:
				strTemp.Format("%d", mn_board_view_r[j][i]);									// Detail View용.
				m_pGrid.GridCellColor(Grid, row, j + 2, WHITE_C, BLUE_D);
				m_pGrid.GridCellText(Grid, row, j + 2, strTemp);
				nTotalCount += mn_board_view_r[j][i];
				break;
			}
		}
	}

	for (j = 0; j < 80; j++)		// Fail + NTO + BTO + HTO의 카운트
	{
		row = 2 + 5 * 2;
		
		switch(mn_socket_view_mode)
		{
		case 0:
			for (i = 1; i < 5; i++)
			{
				strTemp.Format("%d", mn_board_view_p[j][i] + mn_board_view_r[j][i]);			// Detail View용.
			}
			
			m_pGrid.GridCellColor(Grid, row, j + 2, WHITE_C, BLUE_D);
			m_pGrid.GridCellText(Grid, row, j + 2, strTemp);
			break;
		case 1:
			for (i = 1; i < 5; i++)
			{
				strTemp.Format("%d", mn_board_view_p[j][i]);			// Detail View용.
			}
			m_pGrid.GridCellColor(Grid, row, j + 2, WHITE_C, BLUE_D);
			m_pGrid.GridCellText(Grid, row, j + 2, strTemp);
			break;
		case 2:
			for (i = 1; i < 5; i++)
			{
				strTemp.Format("%d", mn_board_view_r[j][i]);			// Detail View용.
			}
			m_pGrid.GridCellColor(Grid, row, j + 2, WHITE_C, BLUE_D);
			m_pGrid.GridCellText(Grid, row, j + 2, strTemp);
			break;
		}
	}

	// 보드별 Count
	for (i = 0; i < 5; i++)
	{
		for (j = 0; j < 80; j++)
		{
			switch(mn_socket_view_mode)
			{
			case 0:
				nBoardCount[j] += mn_board_view_p[j][i];
				nBoardCount[j] += mn_board_view_r[j][i];
				break;
			case 1:
				nBoardCount[j] += mn_board_view_p[j][i];
				break;
			case 2:
				nBoardCount[j] += mn_board_view_r[j][i];
				break;
			}
		}
	}
	
	for (i = 0; i < 80; i++)
	{
		strTemp.Format("%d", nBoardCount[i]);	
		m_pGrid.GridCellColor(Grid, 13, i + 2, WHITE_C, BLUE_C);	
		m_pGrid.GridCellText(Grid, 13, i + 2, strTemp);
	}

	// Bin별 Count
	for (i = 0; i < 5; i++)
	{
		for (j = 0; j < 80; j++)
		{
			switch(mn_socket_view_mode)
			{
			case 0:
				nBinCount[i] += mn_board_view_p[j][i];
				nBinCount[i] += mn_board_view_r[j][i];
				break;
			case 1:
				nBinCount[i] += mn_board_view_p[j][i];
				break;
			case 2:
				nBinCount[i] += mn_board_view_r[j][i];
				break;
			}
		}
	}

	for (i = 0; i < 5; i++)
	{
		strTemp.Format("%d", nBinCount[i]);	
		m_pGrid.GridCellColor(Grid, 2 + i * 2, 82, WHITE_C, BLUE_C);	
		m_pGrid.GridCellText(Grid, 2 + i * 2, 82, strTemp);
	}

	for (i = 1; i < 5; i++)			// Fail + NBH의 토탈 
	{
		strTemp.Format("%d", nBinCount[i]);	
	}

	m_pGrid.GridCellColor(Grid, 12, 82, WHITE_C, BLUE_C);	
	m_pGrid.GridCellText(Grid, 12, 82, strTemp);

	// Bin별 Yield
	nBinTotal = 0;
	for (i = 0; i < 5; i++)
	{
		nBinTotal += nBinCount[i];
	}

	for (i = 0; i < 5; i++)
	{
		if (nBinTotal > 0)
		{
			fBinYield[i] = float((float)nBinCount[i] / (float)nBinTotal * 100);
		}
		else
		{
			fBinYield[i] = 0;
		}
	}

	for (i = 0; i < 5; i++)
	{
		strTemp.Format("%.3f %%", fBinYield[i]);	
		m_pGrid.GridCellColor(Grid, 3 + i * 2, 82, WHITE_C, BLUE_C);	
		m_pGrid.GridCellText(Grid, 3 + i * 2, 82, strTemp);
	}

	// Socket별 Yield
	for (i = 0; i < 5; i++)
	{
		for (j = 0; j < 80; j++)
		{
			switch(mn_socket_view_mode)
			{
			case 0:
				nSocketTotal[j] += mn_board_view_p[j][i];
				nSocketTotal[j] += mn_board_view_r[j][i];
				break;
			case 1:
				nSocketTotal[j] += mn_board_view_p[j][i];
				break;
			case 2:
				nSocketTotal[j] += mn_board_view_r[j][i];
				break;
			}
		}
	}

	for (i = 0; i < 5; i++)
	{
		for (j = 0; j < 80; j++)
		{
			if (i == 0)					// 0번 배열이 PASS.
			{
				switch(mn_socket_view_mode)
				{
				case 0:
					nSocketGood[j] += mn_board_view_p[j][i];
					nSocketGood[j] += mn_board_view_r[j][i];
					break;
				case 1:
					nSocketGood[j] += mn_board_view_p[j][i];
					break;
				case 2:
					nSocketGood[j] += mn_board_view_r[j][i];
					break;
				}
			}
		}
	}

	for (i = 0; i < 80; i++)
	{
		if (nSocketTotal[i] > 0)
		{
			fBoardYield[i] = float(float((float)nSocketGood[i] / (float)nSocketTotal[i]) * 100);
		}
		else
		{
			fBoardYield[i] = 0;
		}
	}
	
	for (i = 0; i < 80; i++)
	{
		strTemp.Format("%.3f %%", fBoardYield[i]);	
		m_pGrid.GridCellColor(Grid, 14, i + 2, WHITE_C, BLUE_C);	
		m_pGrid.GridCellText(Grid, 14, i + 2, strTemp);

		strTemp.Format("%d", nSocketTotal[i] - nSocketGood[i]);
		m_pGrid.GridCellColor(Grid, 12, i + 2, WHITE_C, RED_C);	
		m_pGrid.GridCellText(Grid, 12, i + 2, strTemp);
	}

	// 총 수량
	strTemp.Format("%d", nTotalCount);	
	m_pGrid.GridCellColor(Grid, 13, 82, BLUE_C, WHITE_C);	
	m_pGrid.GridCellText(Grid, 13, 82, strTemp);

	for (j = 0; j < 80; j++)
	{
		nTotalGood += nSocketGood[j];
	}

	if (nTotalCount > 0)
	{
		fTotalYield = float(float((float)nTotalGood / (float)nTotalCount) * 100);
	}
	else
	{
		fTotalYield = 0;
	}

	// 전체 Yield
	strTemp.Format("%.3f %%", fTotalYield);	
	m_pGrid.GridCellColor(Grid, 14, 82, BLUE_C, WHITE_C);	
	m_pGrid.GridCellText(Grid, 14, 82, strTemp);

	// Fail & NBH
	strTemp.Format("%d", nTotalCount - nTotalGood);
	m_pGrid.GridCellColor(Grid, 12, 82, RED_C, WHITE_C);	
	m_pGrid.GridCellText(Grid, 12, 82, strTemp);

	// 개별 Yield
	nTotalCount = 0;
	for (i = 0; i < 5; i++)
	{
		for (j = 0; j < 80; j++)
		{
			row = 3 + i * 2;
			if (nSocketTotal[j] > 0)
			{
				switch(mn_socket_view_mode)
				{
				case 0:
					fYield = float(float((float)(mn_board_view_p[j][i] + mn_board_view_r[j][i]) / (float)nSocketTotal[j]) * 100);
					break;
				case 1:
					fYield = float(float((float)(mn_board_view_p[j][i]) / (float)nSocketTotal[j]) * 100);
					break;
				case 2:
					fYield = float(float((float)(mn_board_view_r[j][i]) / (float)nSocketTotal[j]) * 100);
					break;
				}
			}
			else
			{
				fYield = 0;
			}
			strTemp.Format("%.3f %%", fYield);						// Detail View용.
			m_pGrid.GridCellColor(Grid, row, j + 2, WHITE_C, BLUE_D);
			m_pGrid.GridCellText(Grid, row, j + 2, strTemp);
		}
	}
}

void CScreen_List_Data_View::OnBtnStartTime() 
{
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	char chr_buf[20];
	int nHour;
	
	m_oletime[0] = m_pstart_time.GetDate();
	nHour		= m_oletime[0].GetHour();
	
	mstr_temp = LPCTSTR(_itoa(nHour, chr_buf, 10));
	
	st_msg.mstr_keypad_msg = _T("시작 시간.");
	if (st_handler.mn_language == LANGUAGE_ENGLISH)	st_msg.mstr_keypad_msg = _T("Start Time");
    
	st_msg.mstr_keypad_val = mstr_temp;

	mstr_temp = KeyPad.GetNumEditString_I(0, 24, atoi(st_msg.mstr_keypad_val), st_msg.mstr_keypad_msg);

	mstr_temp.Format("%02s:00:00", mstr_temp);
	((CEdit*)GetDlgItem(IDC_START_TIME))->SetWindowText(mstr_temp);
}

void CScreen_List_Data_View::OnBtnEndTime() 
{
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	char chr_buf[20];
	int nHour;
	
	m_oletime[1] = m_pend_time.GetDate();
	nHour		= m_oletime[1].GetHour();
	
	mstr_temp = LPCTSTR(_itoa(nHour, chr_buf, 10));
	
	st_msg.mstr_keypad_msg = _T("끝 시간.");
	if (st_handler.mn_language  == LANGUAGE_ENGLISH)	st_msg.mstr_keypad_msg = _T("End Time");
    
	st_msg.mstr_keypad_val = mstr_temp;
	
	mstr_temp = KeyPad.GetNumEditString_I(0, 24, atoi(st_msg.mstr_keypad_val), st_msg.mstr_keypad_msg);
	mstr_temp.Format("%02s:00:00", mstr_temp);
	((CEdit*)GetDlgItem(IDC_END_TIME))->SetWindowText(mstr_temp);
}

void CScreen_List_Data_View::OnBtnBufferSearch() 
{
	int year_s, year_e, month_s, month_e, day_s, day_e;
	int hour_s, hour_e, min_s, min_e, sec_s, sec_e;
	long count = 0;
	int i;
	int pos[10] = {0,};
	CString temp;
	CString strTmp, strDate, strCode;
	CTime time;
	
	mn_view_type = 5;				// 5는 Buffer
	
	m_oledate[0] = m_pstart_date.GetDate();
	m_oledate[1] = m_pend_date.GetDate();
	
	m_oletime[0] = m_pstart_time.GetDate();
	m_oletime[1] = m_pend_time.GetDate();
	
	year_s		= m_oledate[0].GetYear();
	month_s		= m_oledate[0].GetMonth();
	day_s		= m_oledate[0].GetDay();
	
	hour_s		= m_oletime[0].GetHour();
	min_s		= m_oletime[0].GetMinute();
	sec_s		= m_oletime[0].GetSecond();
	
	year_e		= m_oledate[1].GetYear();
	month_e		= m_oledate[1].GetMonth();
	day_e		= m_oledate[1].GetDay();
	
	hour_e		= m_oletime[1].GetHour();
	min_e		= m_oletime[1].GetMinute();
	sec_e		= m_oletime[1].GetSecond();
	
	Init_Data();	
	Init_Buffer();
	
	CString strFuncRet = OnDateCheck(year_s, month_s, day_s, hour_s, min_s, sec_s, year_e, month_e, day_e, hour_e, min_e, sec_e);
	
	if (strFuncRet != "NONE")
	{
		Func.MsgLog(strFuncRet);
		return;
	}
	
	m_oledate[0].SetDateTime(year_s, month_s, day_s, hour_s, 0, 0);
	m_oledate[1].SetDateTime(year_e, month_e, day_e, hour_e, 0, 0);
	
	if (Mysql.MySql_Open("localhost", "root", "1111", 3306))
	{
		if (Mysql.Database_Select("AMT8530"))
		{	
			count = Mysql.Record_Date_Search("BUFFER_DATA", "DATE", "<>", m_oledate);
			
			if (count > 0)
			{
				Init_Lot();
				Init_Buffer();
				
				for (i = 0; i<count; i++)
				{
					Display_Buffer(i);
				}
			}	
		}
	}
	Mysql.MySql_Close();
}

void CScreen_List_Data_View::Init_Buffer()
{
	TSpread *Grid; 
	int row;
	int i;
	Grid = (TSpread*)GetDlgItem(IDC_CUSTOM_DEVICE);
	
	m_pGrid.GridReset(Grid);
	// 대문자 
	m_pGrid.GridCellSelectDisplay(Grid, TRUE);
	m_pGrid.GridRowHeader(Grid, FALSE);
	m_pGrid.GridColHeader(Grid, TRUE);
	m_pGrid.GridHorizontal(Grid, TRUE);
	m_pGrid.GridVertical(Grid, TRUE);
	m_pGrid.GridAutoSize(Grid, FALSE);
	
	m_pGrid.GridCellRows(Grid, 1);
	m_pGrid.GridCellCols(Grid, 8);
	
	
	m_pGrid.GridCellHeight(Grid, 0, 20);
	
	for (i = 0; i<8; i++)
	{
		if (i == 0)
		{
			m_pGrid.GridCellWidth(Grid, i+1, 30);
		} 
		else if (i == 1 || i == 2)
		{
			m_pGrid.GridCellWidth(Grid, i+1, 30);
		}
		else
		{
			m_pGrid.GridCellWidth(Grid, i+1, 15);
		}
		
		m_pGrid.GridCellControlStatic(Grid, 0, i+1);
		m_pGrid.GridCellFont(Grid, 0, i+1, "MS Sans Serif", 18);
	}
	
	row =0;
	m_pGrid.GridCellColor(Grid, row, 1, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 1, "Date");
	
	m_pGrid.GridCellColor(Grid, row, 2, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 2, "Lot No");
	
	m_pGrid.GridCellColor(Grid, row, 3, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 3, "Part No");
	
	m_pGrid.GridCellColor(Grid, row, 4, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 4, "Buffer Pos");
	
	m_pGrid.GridCellColor(Grid, row, 5, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 5, "Last Module");

	m_pGrid.GridCellColor(Grid, row, 6, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 6, "ByPass");

	m_pGrid.GridCellColor(Grid, row, 7, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 7, "Module Cnt");

	m_pGrid.GridCellColor(Grid, row, 8, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 8, "Retest");
}

void CScreen_List_Data_View::Display_Buffer(int count)
{
	TSpread *Grid; 
	int i;
	int length;
	int pos[2];
	
	CString strData, strTmp[9];
	
	Grid = (TSpread*)GetDlgItem(IDC_CUSTOM_DEVICE);
	
	m_pGrid.GridCellRows(Grid, count+1);
	m_pGrid.GridCellCols(Grid, 8);
	
	m_pGrid.GridCellHeight(Grid, count+1, 20);
	
	for (i = 0; i < 8; i++)
	{
		if (i == 0)
		{
			m_pGrid.GridCellWidth(Grid, i+1, 30);
		} 
		else if (i == 1 || i == 2)
		{
			m_pGrid.GridCellWidth(Grid, i+1, 30);
		}
		else
		{
			m_pGrid.GridCellWidth(Grid, i+1, 15);
		}
		
		m_pGrid.GridCellControlStatic(Grid, count+1, i+1);
		m_pGrid.GridCellFont(Grid, count+1, i+1, "MS Sans Serif", 18);
	}
	
	strData = Mysql.Record_Read();
	// lot_no
	length = strData.GetLength();
	pos[0] = strData.Find(",", 0);
	
	for (i = 0; i < Mysql.mn_buffer_table_cnt; i++)
	{	
		pos[1] = strData.Find(",", pos[0]+1);
		
		if (pos[1] > 0)
		{
			strTmp[i]= strData.Mid(pos[0]+1, (pos[1] - pos[0])-1);
			pos[0] = pos[1];
		}
		else
		{
			strTmp[i] = strData.Mid(pos[0]+1, (length - pos[1])-1);
		}
	}
	
	m_pGrid.GridCellText(Grid, count+1, 1, strTmp[0]);
	m_pGrid.GridCellText(Grid, count+1, 2, strTmp[1]);
	m_pGrid.GridCellText(Grid, count+1, 3, strTmp[2]);
	m_pGrid.GridCellText(Grid, count+1, 4, strTmp[3]);
	m_pGrid.GridCellText(Grid, count+1, 5, strTmp[4]);
	m_pGrid.GridCellText(Grid, count+1, 6, strTmp[5]);
	m_pGrid.GridCellText(Grid, count+1, 7, strTmp[6]);
	m_pGrid.GridCellText(Grid, count+1, 8, strTmp[7]);
}

void CScreen_List_Data_View::OnBtnScarpSearch() 
{
	int year_s, year_e, month_s, month_e, day_s, day_e;
	int hour_s, hour_e, min_s, min_e, sec_s, sec_e;
	long count = 0;
	int i;
	int pos[10] = {0,};
	CString temp;
	CString strTmp, strDate, strCode;
	CTime time;
	
	CString strData, strTemp[10];
	int length = 0;

	


	mn_view_type = 7;				// 7은 Scrap
	
	m_oledate[0] = m_pstart_date.GetDate();
	m_oledate[1] = m_pend_date.GetDate();
	
	m_oletime[0] = m_pstart_time.GetDate();
	m_oletime[1] = m_pend_time.GetDate();
	
	year_s		= m_oledate[0].GetYear();
	month_s		= m_oledate[0].GetMonth();
	day_s		= m_oledate[0].GetDay();
	
	hour_s		= m_oletime[0].GetHour();
	min_s		= m_oletime[0].GetMinute();
	sec_s		= m_oletime[0].GetSecond();
	
	year_e		= m_oledate[1].GetYear();
	month_e		= m_oledate[1].GetMonth();
	day_e		= m_oledate[1].GetDay();
	
	hour_e		= m_oletime[1].GetHour();
	min_e		= m_oletime[1].GetMinute();
	sec_e		= m_oletime[1].GetSecond();
	
	Init_Data();	
	Init_Scrap();
	
	CString strFuncRet = OnDateCheck(year_s, month_s, day_s, hour_s, min_s, sec_s, year_e, month_e, day_e, hour_e, min_e, sec_e);
	
	if (strFuncRet != "NONE")
	{
		Func.MsgLog(strFuncRet);
		return;
	}
	
	m_oledate[0].SetDateTime(year_s, month_s, day_s, hour_s, 0, 0);
	m_oledate[1].SetDateTime(year_e, month_e, day_e, hour_e, 0, 0);
	
	if (Mysql.MySql_Open("localhost", "root", "1111", 3306))
	{
		if (Mysql.Database_Select("AMT8530"))
		{	
			if ( nSearchMethod == 2 )
			{
				count = Mysql.Record_Date_Search("SCRAP_DATA", "DATE", "<>", m_oledate, nSearchMethod);
				
				if (count > 0)
				{
					Init_Lot();
					Init_Scrap();
					
					for (i = 0; i<count; i++)
					{
						Display_Scrap(i);
					}
				}	
			}
			else
			{
				count = Mysql.Record_Date_Search("SCRAP_DATA", "DATE", "<!>", m_oledate, nSearchMethod);
				
				if (count > 0)
				{
					Init_Lot();
					Init_Scrap();
					
					for (i = 0; i<count; i++)
					{
						strData = Mysql.Record_Read();
						
						length = strData.GetLength();

						if (strData.Mid(length - 1, 1) == ",")
						{
							strData = strData.Mid(0, length - 1);
						}

						Display_LotNo(i, strData);
					}
				}	
			}
		}
	}
	Mysql.MySql_Close();
}

void CScreen_List_Data_View::Init_Scrap()
{
	TSpread *Grid; 
	int row;
	int i;
	Grid = (TSpread*)GetDlgItem(IDC_CUSTOM_DEVICE);
	
	m_pGrid.GridReset(Grid);
	// 대문자 
	m_pGrid.GridCellSelectDisplay(Grid, TRUE);
	m_pGrid.GridRowHeader(Grid, FALSE);
	m_pGrid.GridColHeader(Grid, TRUE);
	m_pGrid.GridHorizontal(Grid, TRUE);
	m_pGrid.GridVertical(Grid, TRUE);
	m_pGrid.GridAutoSize(Grid, FALSE);
	
	m_pGrid.GridCellRows(Grid, 1);
	m_pGrid.GridCellCols(Grid, 5);
	
	
	m_pGrid.GridCellHeight(Grid, 0, 20);
	
	for (i = 0; i<5; i++)
	{
		if (i == 0)
		{
			m_pGrid.GridCellWidth(Grid, i+1, 30);
		} 
		else if (i == 1 || i == 2)
		{
			m_pGrid.GridCellWidth(Grid, i+1, 30);
		}
		else if (i == 3)
		{
			m_pGrid.GridCellWidth(Grid, i+1, 30);
		}
		else
		{
			m_pGrid.GridCellWidth(Grid, i+1, 30);
		}
		
		m_pGrid.GridCellControlStatic(Grid, 0, i+1);
		m_pGrid.GridCellFont(Grid, 0, i+1, "MS Sans Serif", 18);
	}
	
	
	row =0;
	m_pGrid.GridCellColor(Grid, row, 1, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 1, "Date");
	
	m_pGrid.GridCellColor(Grid, row, 2, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 2, "Lot No");
	
	m_pGrid.GridCellColor(Grid, row, 3, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 3, "Part No");
	
	m_pGrid.GridCellColor(Grid, row, 4, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 4, "Serial No");
	
	m_pGrid.GridCellColor(Grid, row, 5, BLUE_D, WHITE_C);	
	m_pGrid.GridCellText(Grid, row, 5, "Scrap Code");
}

void CScreen_List_Data_View::Display_Scrap(int count)
{
	TSpread *Grid; 
	int i;
	int length;
	int pos[2];
	
	CString strData, strTmp[9];
	
	Grid = (TSpread*)GetDlgItem(IDC_CUSTOM_DEVICE);
	
	m_pGrid.GridCellRows(Grid, count+1);
	m_pGrid.GridCellCols(Grid, 5);
	
	m_pGrid.GridCellHeight(Grid, count+1, 20);
	
	for (i = 0; i < 5; i++)
	{
		if (i == 0)
		{
			m_pGrid.GridCellWidth(Grid, i+1, 30);
		} 
		else if (i == 1 || i == 2)
		{
			m_pGrid.GridCellWidth(Grid, i+1, 30);
		}
		else if (i == 3)
		{
			m_pGrid.GridCellWidth(Grid, i+1, 30);
		}
		else
		{
			m_pGrid.GridCellWidth(Grid, i+1, 30);
		}
		
		m_pGrid.GridCellControlStatic(Grid, count+1, i+1);
		m_pGrid.GridCellFont(Grid, count+1, i+1, "MS Sans Serif", 18);
	}
	
	strData = Mysql.Record_Read();
	// lot_no
	length = strData.GetLength();
	pos[0] = strData.Find(",", 0);
	
	for (i = 0; i < Mysql.mn_scrap_table_cnt; i++)
	{	
		pos[1] = strData.Find(",", pos[0]+1);
		
		if (pos[1] > 0)
		{
			strTmp[i]= strData.Mid(pos[0]+1, (pos[1] - pos[0])-1);
			pos[0] = pos[1];
		}
		else
		{
			strTmp[i] = strData.Mid(pos[0]+1, (length - pos[1])-1);
		}
	}
	
	m_pGrid.GridCellText(Grid, count+1, 1, strTmp[0]);
	m_pGrid.GridCellText(Grid, count+1, 2, strTmp[1]);
	m_pGrid.GridCellText(Grid, count+1, 3, strTmp[2]);
	m_pGrid.GridCellText(Grid, count+1, 4, strTmp[3]);
	m_pGrid.GridCellText(Grid, count+1, 5, strTmp[4]);
}
