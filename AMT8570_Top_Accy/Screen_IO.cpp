// Screen_IO.cpp : implementation file
//

#include "stdafx.h"
#include "Handler.h"
#include "Screen_IO.h"
#include "Variable.h"
#include "Dialog_KeyPad.h"
#include "Dialog_Message.h"
#include "FastechPublic_IO.h"
#include "math.h"

#include "IO_Manager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScreen_IO

IMPLEMENT_DYNCREATE(CScreen_IO, CFormView)

CScreen_IO::CScreen_IO()
	: CFormView(CScreen_IO::IDD)
{
	//{{AFX_DATA_INIT(CScreen_IO)
	//}}AFX_DATA_INIT
	mn_master = 0;
	mn_port = 0;
	mn_slave = 0;
}

CScreen_IO::~CScreen_IO()
{
}

void CScreen_IO::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CScreen_IO)
	DDX_Control(pDX, IDC_RADIO_IO_CNT_50, m_radio_io_cnt_50);
	DDX_Control(pDX, IDC_RADIO_IO_CNT_100, m_radio_io_cnt_100);
	DDX_Control(pDX, IDC_RADIO_IO_CNT_10, m_radio_io_cnt_10);
	DDX_Control(pDX, IDC_RADIO_IO_CNT_1, m_radio_io_cnt_1);
	DDX_Control(pDX, IDC_MSG_RETRY_WAIT, m_msg_retry_wait);
	DDX_Control(pDX, IDC_GROUP_CONTI_OP, m_group_conti_op);
	DDX_Control(pDX, IDC_BTN_IO_E_STOP, m_btn_io_e_stop);
	DDX_Control(pDX, IDC_DGT_RETRY_WAIT, m_dgt_retry_wait);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CScreen_IO, CFormView)
	//{{AFX_MSG_MAP(CScreen_IO)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_RADIO_IO_CNT_1, OnRadioIoCnt1)
	ON_BN_CLICKED(IDC_RADIO_IO_CNT_10, OnRadioIoCnt10)
	ON_BN_CLICKED(IDC_RADIO_IO_CNT_50, OnRadioIoCnt50)
	ON_BN_CLICKED(IDC_RADIO_IO_CNT_100, OnRadioIoCnt100)
	ON_BN_CLICKED(IDC_DGT_RETRY_WAIT, OnDgtRetryWait)
	ON_BN_CLICKED(IDC_BTN_IO_E_STOP, OnBtnIoEStop)
	//}}AFX_MSG_MAP
	ON_MESSAGE(SSM_CLICK  , OnCellClick)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScreen_IO diagnostics

#ifdef _DEBUG
void CScreen_IO::AssertValid() const
{
	CFormView::AssertValid();
}

void CScreen_IO::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CScreen_IO message handlers

void CScreen_IO::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	// **************************************************************************
	mp_io_font = NULL;
	mp_io_font = new CFont;
	mp_io_font->CreateFont(14,0,0,0,900,0,0,0,0,0,0,ANTIALIASED_QUALITY,0,"MS Sans Serif");
	// **************************************************************************

	mn_action_cnt_set = 1;
	mn_conti_wait = 1000;
	
	mstr_io_name = "";
	mstr_io_act = "";
	
	OnIO_Init_Map();
	OnIO_Init_List();
	
	OnIO_GroupBox_Set();
	OnIO_Operation_Count_Set();
	OnIO_Name_Label_Set();
	OnIO_Digital_Count_Set();

	OnIO_Slave_Change();
	
	SetTimer(TMR_IO_READ, 50, NULL);	
}

void CScreen_IO::OnDestroy() 
{
	// **************************************************************************
	// 생성한 폰트 정보 삭제한다                                                 
	// **************************************************************************
	delete mp_io_font;
	mp_io_font = NULL;

	CFormView::OnDestroy();
}

void CScreen_IO::OnTimer(UINT nIDEvent) 
{
	int n_check;
	
	switch(nIDEvent)
	{
	case TMR_IO_READ:
		OnIO_OnOutputCheck(mn_port, mn_slave);
		OnIO_OnInputCheck(mn_port, mn_slave);
		break;

	case TMR_IO_CONTINUE:
		n_check = OnIO_Cylinder_Excution();
		
		if (n_check == RET_GOOD)
		{
			if (st_handler.nMenuLock != FALSE)
				st_handler.nMenuLock = FALSE;
			
			KillTimer(TMR_IO_CONTINUE);		// 타이머 삭제
			OnIO_Controls_Enable(true);		// 모든 컨트롤 화면 출력 제어 함수
		}
		else if (n_check == RET_ERROR)
		{
			if (st_handler.nMenuLock != FALSE)
				st_handler.nMenuLock = FALSE;
			
			KillTimer(TMR_IO_CONTINUE);		// 타이머 삭제
			OnIO_Controls_Enable(true);		// 모든 컨트롤 화면 출력 제어 함수
		}
		break;
	}
	
	CFormView::OnTimer(nIDEvent);
}

void CScreen_IO::GridColor(UINT nID, int row, int col, COLORREF bk, COLORREF tk)
{
	TSpread *Grid = (TSpread*)GetDlgItem(nID);
	
	Grid->SetColor(col, row, bk, tk);
	Grid->SetBackColorStyle(SS_BACKCOLORSTYLE_UNDERGRID);

	// 규이리 추가 [2015.02.16]
	if (Grid != NULL)
	{
		Grid = NULL;
		delete Grid;
	}
}

void CScreen_IO::GridControl(UINT nID, int type, int row, int col, int pos)
{
	TSpread *Grid = (TSpread*)GetDlgItem(nID);
	
	SS_CELLTYPE CellType;
	SS_DATEFORMAT dateFormat = { TRUE, '/', IDF_DDMMYY, FALSE };
	SS_TIMEFORMAT tmFormat;
	CString strList;
	
	strList = "combo1\tcombo2\tcombo3\tcombo4\tcombo5\tcombo6";
	
	double a = 0.5;
	
	
	switch(type)
	{
	case DATE:			
		Grid->SetTypeDate(&CellType, 0 , &dateFormat, NULL, NULL);
		break;
	case EDIT:
		Grid->SetTypeEdit(&CellType,ES_CENTER,60,SS_CHRSET_CHR,SS_CASE_NOCASE);
		break;
	case COMBO:
		// SetValue(col, row, data); data = ""0"(1선택) data = "1"(2선택) data = "2"(3선택)
		Grid->SetTypeComboBox(&CellType, 0, strList);
		break;
	case BUTTON:
		Grid->SetTypeButton(&CellType,SBS_TEXTLEFT_PICTRIGHT, "Press Me",NULL, SUPERBTN_PICT_NONE,
			NULL, SUPERBTN_PICT_NONE,SUPERBTN_TYPE_NORMAL,2, NULL);
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

	// 규이리 추가 [2015.02.16]
	if (Grid != NULL)
	{
		Grid = NULL;
		delete Grid;
	}
}

void CScreen_IO::GridData(UINT nID, int row, int col, CString data)
{	
	TSpread *Grid = (TSpread*)GetDlgItem(nID);
	
	Grid->SetValue(col, row, data);
	
	// 규이리 추가 [2015.02.16]
	if (Grid != NULL)
	{
		Grid = NULL;
		delete Grid;
	}
}

void CScreen_IO::GridFont(UINT nID, int row, int col, int size)
{
	HFONT font;
	CString strTemp;
	TSpread *Grid = (TSpread*)GetDlgItem(nID);
	
	font = CreateFont(size, 0, 0, 0, FW_BOLD, 0, 0, 0, 0, 0, 0, 0, 0, "MS Sans Serif");
	
	Grid->SetFont(col, row, font, TRUE);

	// 규이리 추가 [2015.02.16]
	if (Grid != NULL)
	{
		Grid = NULL;
		delete Grid;
	}
}

void CScreen_IO::GridMerge(UINT nID, int srow, int scol, int nrow, int ncol)
{	
	TSpread *Grid = (TSpread*)GetDlgItem(nID);
	
	Grid->AddCellSpan(scol, srow, ncol, nrow);

	// 규이리 추가 [2015.02.16]
	if (Grid != NULL)
	{
		Grid = NULL;
		delete Grid;
	}
}

void CScreen_IO::OnIO_Init_Map()
{
	TSpread *Grid = (TSpread*)GetDlgItem(IDC_CUSTOM_IO);	
	m_p_grid.GridFileOpen(Grid, st_path.sIOMap, "sheet1");
	
	OnIO_OnSearch_Data();
	
	OnIO_OnDrawGrid();
	OnIO_OnChangeSheet(mn_port, mn_slave);
	OnIO_OnOutputCheck(mn_port, mn_slave);

	// 규이리 추가 [2015.02.16]
	if (Grid != NULL)
	{
		Grid = NULL;
		delete Grid;
	}
}

void CScreen_IO::OnIO_Init_List()
{
	TSpread *Grid = (TSpread*)GetDlgItem(IDC_CUSTOM_LIST);
	int i, j;
	int count;
	CString strTmp;
	
	Grid->SetBool(SSB_EDITMODEPERMANENT, TRUE); // cell선택시 박스 제거
	Grid->SetBool(SSB_SHOWCOLHEADERS, TRUE); // col header 삭제
	Grid->SetBool(SSB_SHOWROWHEADERS, FALSE); // row header 삭제
	Grid->SetBool(SSB_HORZSCROLLBAR, FALSE); // 좌우 스크롤바

	if (mn_list_count > 17)
	{
		Grid->SetBool(SSB_VERTSCROLLBAR, TRUE); // 상하 스크롤바
	}
	else
	{
		Grid->SetBool(SSB_VERTSCROLLBAR, FALSE); // 상하 스크롤바
	}
	
	GridColor(IDC_CUSTOM_LIST, 0, 1, BLUE_D, WHITE_C);
	GridFont(IDC_CUSTOM_LIST, 0, 1, 14);
	Grid->SetData(1, 0, "Module No.");
	
	GridColor(IDC_CUSTOM_LIST, 0, 2, BLUE_D, WHITE_C);
	GridFont(IDC_CUSTOM_LIST, 0, 2, 14);
	Grid->SetData(2, 0, "ASS`Y");
	
	Grid->SetMaxCols(2);
	Grid->SetMaxRows(mn_list_count);
	
	for (i = 0; i < mn_list_count+1; i++)
	{
		for (j = 0; j < 3; j++)
		{
			Grid->SetRowHeight(i, 20);
			
			if(j==1) Grid->SetColWidth(j, 10);
			else if(j==2) 
			{
				if (mn_list_count > 17)
				{
					Grid->SetColWidth(j, 25.5);
				}
				else
				{
					Grid->SetColWidth(j, 27.7);
				}
			}
			
			GridControl(IDC_CUSTOM_LIST, STATIC, i, j, 0);
		}
	}
	
	count = 0;
	
	for (i = 0; i < 10; i++)
	{
		for (j = 0; j < 8; j++)
		{
			if (mstr_io_module[i][j] != "")
			{
				strTmp.Format("%d", mn_io_module[i][j]);
				GridFont(IDC_CUSTOM_LIST, count+1, 1, 14);
			
				if (i == 0 && j == 0)
				{
					GridColor(IDC_CUSTOM_LIST, count+1, 1, SKY_C, BLACK_C);
				}
				else
				{
					GridColor(IDC_CUSTOM_LIST, count+1, 1, WHITE_C, BLACK_C);
				}
				GridData(IDC_CUSTOM_LIST, count+1, 1, strTmp);
				
				GridFont(IDC_CUSTOM_LIST, count+1, 2, 14);
				
				if (i == 0 && j == 0)
				{
					GridColor(IDC_CUSTOM_LIST, count+1, 2, SKY_C, BLACK_C);
				}
				else
				{
					GridColor(IDC_CUSTOM_LIST, count+1, 2, WHITE_C, BLACK_C);
				}
				GridData(IDC_CUSTOM_LIST, count+1, 2, mstr_io_module[i][j]);
				
				count++;
			}	
		}
	}

	// 규이리 추가 [2015.02.16]
	if (Grid != NULL)
	{
		Grid = NULL;
		delete Grid;
	}
}

void CScreen_IO::OnIO_OnSearch_Data()
{
	TSpread *Grid = (TSpread*)GetDlgItem(IDC_CUSTOM_IO);
	int i, j, k;
	int row_m, col_m;
	int row_s, col_s;
	int pos;
	CString strFind;
	CString strTmp;
	char chvalue[100];
	
	mn_list_count = 0;
	for (i = 0; i < 10; i++)
	{
		for (j = 0; j < 8; j++)
		{
			strFind.Format("Module No : %1d%1d", i, j);
			
			row_m = Grid->GetMaxRows();
			col_m = Grid->GetMaxCols();
			
			row_s = Grid->SearchCol(1, 0, row_m, strFind, SS_SEARCHFLAGS_NONE);
			col_s = Grid->SearchRow(row_s, 0, col_m, strFind, SS_SEARCHFLAGS_NONE);
			
			if (row_s > 0 && col_s > 0)
			{
				Grid->GetValue(col_s, row_s, chvalue);
				strTmp.Format("%s", &chvalue[12]);
				
				mn_io_module[i][j] = atoi(strTmp);
				
				Grid->GetValue(col_s+4, row_s-1, chvalue);
				strTmp.Format("%s", chvalue);
				pos = strTmp.Find(":");
				mstr_io_module[i][j] = strTmp.Mid(pos+1);
				
				for (k = 0; k < 16; k++)
				{
					Grid->GetValue(col_s, row_s+2+k, chvalue);
					strTmp.Format("%s", chvalue);
					mn_io_num[i][j][k] = atoi(strTmp);
					
					Grid->GetValue(col_s+1, row_s+2+k, chvalue);
					strTmp.Format("%s", chvalue);
					mstr_io_label[i][j][k] = strTmp;
					
					Grid->GetValue(col_s+1, row_s+2+k, chvalue);
					strTmp.Format("%c", chvalue[0]);
					
					if(strTmp == "S")
					{
						mn_io_type[i][j][k] = IO_OUT;
					}
					else if(strTmp == "P")
					{
						mn_io_type[i][j][k] = IO_IN;
					}
					else
					{
						mn_io_type[i][j][k] = IO_NONE;
					}
					
					Grid->GetValue(col_s+3, row_s+2+k, chvalue);
					strTmp.Format("%s", chvalue);
					mstr_io_description[i][j][k] = strTmp;
				}
				
				mn_list_count++;
			}	
		}
	}

	// 규이리 추가 [2015.02.16]
	if (Grid != NULL)
	{
		Grid = NULL;
		delete Grid;
	}
}

void CScreen_IO::OnIO_OnChangeSheet(int port, int num)
{
	int row;
	int i;
	CString strTmp;
	TSpread *Grid = (TSpread*)GetDlgItem(IDC_CUSTOM_IO);

	row = 2; 

	if (mn_io_module[port][num]/40)
	{
		strTmp.Format("Master : [1] Port : [%d] Slave : [%d]", mn_io_module[port][num]/10-4, mn_io_module[port][num]%10);
	}
	else
	{
		strTmp.Format("Master : [0] Port : [%d] Slave : [%d]", mn_io_module[port][num]/10, mn_io_module[port][num]%10);
	}

	m_p_grid.GridCellText(Grid, row, 1, strTmp);

	m_p_grid.GridCellMerge(Grid, row, 5, 1, 2);
	m_p_grid.GridCellFont(Grid, row, 5, "MS Sans Serif", 24);
	m_p_grid.GridCellColor(Grid, row, 5, WHITE_C, BLACK_C);
	m_p_grid.GridCellText(Grid, row, 5, "I / O Map Design Sheet");

	for (i = 0; i < 16; i++)
	{
		m_p_grid.GridCellText(Grid, i+4, 4, "");

		if(mstr_io_description[port][num][i] != "")
		{
			strTmp.Format("%d", i+1);
			m_p_grid.GridCellFont(Grid, i+4, 1, "MS Sans Serif", 18);

			if(mn_io_type[port][num][i] == IO_IN)
			{
				m_p_grid.GridCellColor(Grid, i+4, 1, RGB(200,255,200), RGB(25,25,85));
				m_p_grid.GridCellColor(Grid, i+4, 2, RGB(200,255,200), RGB(25,25,85));
				m_p_grid.GridCellColor(Grid, i+4, 3, RGB(200,255,200), RGB(25,25,85));

				m_p_grid.GridCellControlStatic(Grid, i+4, 4);
				m_p_grid.GridCellColor(Grid, i+4, 4, RGB(200,255,200), RGB(25,25,85));

				m_p_grid.GridCellColor(Grid, i+4, 6, RGB(200,255,200), RGB(25,25,85));
				m_p_grid.GridCellText(Grid, i+4, 3, "Input");
			}
			else if(mn_io_type[port][num][i] == IO_OUT)
			{
				m_p_grid.GridCellColor(Grid, i+4, 1, RGB(255,200,200), RGB(25,25,85));
				m_p_grid.GridCellColor(Grid, i+4, 2, RGB(255,200,200), RGB(25,25,85));
				m_p_grid.GridCellColor(Grid, i+4, 3, RGB(255,200,200), RGB(25,25,85));

				m_p_grid.GridCellText(Grid, i+4, 4, "N");
				m_p_grid.GridCellColor(Grid, i+4, 4, RGB(255,200,200), RGB(25,25,85));

				m_p_grid.GridCellColor(Grid, i+4, 6, RGB(255,200,200), RGB(25,25,85));
				m_p_grid.GridCellText(Grid, i+4, 3, "Output");
			}

			m_p_grid.GridCellText(Grid, i+4, 1, strTmp);
			
			m_p_grid.GridCellFont(Grid, i+4, 2, "MS Sans Serif", 18);
			m_p_grid.GridCellText(Grid, i+4, 2, mstr_io_label[port][num][i]);
			
			m_p_grid.GridCellFont(Grid, i+4, 3, "MS Sans Serif", 18);

			m_p_grid.GridCellFont(Grid, i+4, 6, "MS Sans Serif", 18);
			m_p_grid.GridCellText(Grid, i+4, 6, mstr_io_description[port][num][i]);
		}
		else
		{
			m_p_grid.GridCellFont(Grid, i+4, 1, "MS Sans Serif", 18);
			m_p_grid.GridCellColor(Grid, i+4, 1, BLACK_L, WHITE_C);
			m_p_grid.GridCellText(Grid, i+4, 1, "-");

			m_p_grid.GridCellFont(Grid, i+4, 2, "MS Sans Serif", 18);
			m_p_grid.GridCellColor(Grid, i+4, 2, BLACK_L, WHITE_C);
			m_p_grid.GridCellText(Grid, i+4, 2, "-");

			m_p_grid.GridCellFont(Grid, i+4, 3, "MS Sans Serif", 18);
			m_p_grid.GridCellColor(Grid, i+4, 3, BLACK_L, WHITE_C);
			m_p_grid.GridCellText(Grid, i+4, 3, "-");

			m_p_grid.GridCellControlStatic(Grid, i+4, 4);
			m_p_grid.GridCellFont(Grid, i+4, 4, "MS Sans Serif", 18);
			m_p_grid.GridCellColor(Grid, i+4, 4, BLACK_L, WHITE_C);

			m_p_grid.GridCellText(Grid, i+4, 4, "");

			m_p_grid.GridCellFont(Grid, i+4, 5, "MS Sans Serif", 18);
			m_p_grid.GridCellColor(Grid, i+4, 5, BLACK_L, WHITE_C);
			m_p_grid.GridCellText(Grid, i+4, 5, "-");

			m_p_grid.GridCellFont(Grid, i+4, 6, "MS Sans Serif", 18);
			m_p_grid.GridCellColor(Grid, i+4, 6, BLACK_L, WHITE_C);
			m_p_grid.GridCellText(Grid, i+4, 6, "-");
		}
	}

	// 규이리 추가 [2015.02.16]
	if (Grid != NULL)
	{
		Grid = NULL;
		delete Grid;
	}
}

void CScreen_IO::OnIO_OnOutputCheck(int port, int slave)
{
	int i;
	int output;
	int data;

	TSpread *Grid = (TSpread*)GetDlgItem(IDC_CUSTOM_IO);

	output = g_ioMgr.Get_Out_Word(mn_io_module[port][slave]);

	for(i=0; i<16; i++)
	{
		if(mn_io_type[port][slave][i] == IO_OUT && mstr_io_description[port][slave][i] != "")
		{
			data = (output>>i) & 0x01;

			if(data == IO_ON)
			{
				m_p_grid.GridCellColor(Grid, i+4, 5, RGB(255,128,64), WHITE_C);
				m_p_grid.GridCellFont(Grid, i+4, 5, "MS Sans Serif", 14);
				m_p_grid.GridCellText(Grid, i+4, 5, "ON");
			}
			else
			{
				m_p_grid.GridCellColor(Grid, i+4, 5, RGB(128,64,0), BLACK_C);
				m_p_grid.GridCellFont(Grid, i+4, 5, "MS Sans Serif", 14);
				m_p_grid.GridCellText(Grid, i+4, 5, "OFF");
			}
		}
	}

	// 규이리 추가 [2015.02.16]
	if (Grid != NULL)
	{
		Grid = NULL;
		delete Grid;
	}
}

void CScreen_IO::OnIO_OnDrawGrid()
{
	TSpread *Grid = (TSpread*)GetDlgItem(IDC_CUSTOM_IO);
	int i, j;
	int row;

	m_p_grid.GridReset(Grid);

	m_p_grid.GridCellSelectDisplay(Grid, TRUE);
	m_p_grid.GridRowHeader(Grid, FALSE);
	m_p_grid.GridColHeader(Grid, FALSE);
	m_p_grid.GridHorizontal(Grid, FALSE);
	m_p_grid.GridVertical(Grid, FALSE);
	m_p_grid.GridAutoSize(Grid, FALSE);
	m_p_grid.GridAutoSize(Grid, FALSE);
	m_p_grid.GridCellRows(Grid, 19);
	m_p_grid.GridCellCols(Grid, 6);

	for (i = 1; i < 20; i++)
	{
		for (j = 1; j < 7; j++)
		{
			m_p_grid.GridCellHeight_L(Grid, i, 28);

			if (j == 1)	
			{
				m_p_grid.GridCellWidth_L(Grid, j, 10);
			}
			else if (j == 2)	
			{
				m_p_grid.GridCellWidth_L(Grid, j, 20);
			}
			else if (j == 3)	
			{
				m_p_grid.GridCellWidth_L(Grid, j, 10);
			}
			else if (j == 4)	
			{
				m_p_grid.GridCellWidth_L(Grid, j, 5);
			}
			else if (j == 5)	
			{
				m_p_grid.GridCellWidth_L(Grid, j, 20);
			}
			else if (j == 6)	
			{
				m_p_grid.GridCellWidth_L(Grid, j, 50);
			}

			m_p_grid.GridCellControlStatic(Grid, i, j);
			m_p_grid.GridCellFont(Grid, i, j, "MS Sans Serif", 18);
		}
	}

	row = 1;

	m_p_grid.GridCellMerge(Grid, row, 1, 1, 6);
	m_p_grid.GridCellFont(Grid, row, 1, "MS Sans Serif", 32);
	m_p_grid.GridCellColor(Grid, row, 1, BLUE_D, WHITE_C);
	m_p_grid.GridCellText(Grid, row, 1, "I / O Map Design Sheet");

	row = 2; 
	m_p_grid.GridCellMerge(Grid, row, 1, 1, 3);
	m_p_grid.GridCellFont(Grid, row, 1, "MS Sans Serif", 24);

	m_p_grid.GridCellMerge(Grid, row, 4, 1, 2);
	m_p_grid.GridCellFont(Grid, row, 4, "MS Sans Serif", 24);
	m_p_grid.GridCellColor(Grid, row, 4, SKY_C, BLACK_C);
	m_p_grid.GridCellText(Grid, row, 4, "ASS`Y");

	row = 3; 
	m_p_grid.GridCellFont(Grid, row, 1, "MS Sans Serif", 24);
	m_p_grid.GridCellColor(Grid, row, 1, SKY_C, BLACK_C);
	m_p_grid.GridCellText(Grid, row, 1, "No");

	m_p_grid.GridCellFont(Grid, row, 2, "MS Sans Serif", 24);
	m_p_grid.GridCellColor(Grid, row, 2, SKY_C, BLACK_C);
	m_p_grid.GridCellText(Grid, row, 2, "Label");

	m_p_grid.GridCellFont(Grid, row, 3, "MS Sans Serif", 24);
	m_p_grid.GridCellColor(Grid, row, 3, SKY_C, BLACK_C);
	m_p_grid.GridCellText(Grid, row, 3, "I/O");

	m_p_grid.GridCellMerge(Grid, row, 4, 1, 2);
	m_p_grid.GridCellFont(Grid, row, 4, "MS Sans Serif", 24);
	m_p_grid.GridCellColor(Grid, row, 4, SKY_C, BLACK_C);
	m_p_grid.GridCellText(Grid, row, 4, "Status");

	m_p_grid.GridCellFont(Grid, row, 6, "MS Sans Serif", 24);
	m_p_grid.GridCellColor(Grid, row, 6, SKY_C, BLACK_C);
	m_p_grid.GridCellText(Grid, row, 6, "Description");

	// 규이리 추가 [2015.02.16]
	if (Grid != NULL)
	{
		Grid = NULL;
		delete Grid;
	}
}

void CScreen_IO::OnIO_GroupBox_Set()
{
	CSxLogFont io_font(15,FW_SEMIBOLD,false,"MS Sans Serif");
	
	m_group_conti_op.SetFont(io_font);
	m_group_conti_op.SetCatptionTextColor(RGB(145,25,0));
	m_group_conti_op.SetFontBold(TRUE);
}

void CScreen_IO::OnIO_Operation_Count_Set()
{
	m_radio_io_cnt_1.SetCheck(CTL_NO);
	m_radio_io_cnt_10.SetCheck(CTL_NO);
	m_radio_io_cnt_50.SetCheck(CTL_NO);
	m_radio_io_cnt_100.SetCheck(CTL_NO);
	
	switch(mn_action_cnt_set)
	{
	case 1:
		m_radio_io_cnt_1.SetCheck(CTL_YES);
		break;
		
	case 10:
		m_radio_io_cnt_10.SetCheck(CTL_YES);
		break;
		
	case 50:
		m_radio_io_cnt_50.SetCheck(CTL_YES);
		break;
		
	case 100:
		m_radio_io_cnt_100.SetCheck(CTL_YES);
		break;

	default:
		mn_action_cnt_set = 1;
		m_radio_io_cnt_1.SetCheck(CTL_YES);
		break;
	}
}

void CScreen_IO::OnIO_Name_Label_Set()
{
	m_msg_retry_wait.SetTextFont(mp_io_font);
	m_msg_retry_wait.SetColor(RGB(0, 0, 0), RGB(255, 255, 255), 3, 2);
	m_msg_retry_wait.SetTextAlign(CFloatST::STA_CENTER, 10);
}

void CScreen_IO::OnIO_Digital_Count_Set()
{
	m_dgt_retry_wait.SetStyle(CDigit::DS_INT, 4, CDigit::DC_BGREEN);
	m_dgt_retry_wait.SetVal(mn_conti_wait);
}

void CScreen_IO::OnRadioIoCnt1() 
{
	mn_action_cnt_set = 1;
	OnIO_Operation_Count_Set();
}

void CScreen_IO::OnRadioIoCnt10() 
{
	mn_action_cnt_set = 10;
	OnIO_Operation_Count_Set();
}

void CScreen_IO::OnRadioIoCnt50() 
{
	mn_action_cnt_set = 50;
	OnIO_Operation_Count_Set();
}

void CScreen_IO::OnRadioIoCnt100() 
{
	mn_action_cnt_set = 100;
	OnIO_Operation_Count_Set();
}

void CScreen_IO::OnDgtRetryWait() 
{
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	char chr_buf[20];
	
	if(st_handler.nMenuLock != FALSE)	return;
	
	mn_conti_wait = (int)m_dgt_retry_wait.GetVal();
	mstr_temp = LPCTSTR(_itoa(mn_conti_wait, chr_buf, 10));
	
	st_msg.mstr_keypad_msg = _T("Retry Wait Setting");
    
	st_msg.mstr_keypad_val = mstr_temp;

	mstr_temp = KeyPad.GetNumEditString_I(100, 5000, atoi(st_msg.mstr_keypad_val), st_msg.mstr_keypad_msg);

	mn_conti_wait = atoi(mstr_temp);	
	m_dgt_retry_wait.SetVal(mn_conti_wait);
}

void CScreen_IO::OnBtnIoEStop() 
{
	mn_stop_req = TRUE;
	mn_action_cnt_chk = 0;	
}

void CScreen_IO::OnIO_Slave_Change()
{
	mn_check_slave[mn_master * 1000 + mn_port * 100 + mn_slave * 10] = FAS_IO.Check_IO_Slave(mn_master, mn_port, mn_slave);
	
	if (mn_check_slave[mn_master * 1000 + mn_port * 100 + mn_slave * 10] == TRUE)		// 살아 있다면... 죽일수 있는 버튼.
	{
		SetDlgItemText(IDC_BTN_SLAVE_ENABLE, "Disable");
	}
	else							// 죽어 있다면... 살릴수 있는 버튼.
	{
		SetDlgItemText(IDC_BTN_SLAVE_ENABLE, "Enable");
	}
}

void CScreen_IO::OnCellClick(WPARAM wParam, LPARAM lParam)
{
	TSpread *Grid_List, *Grid_Map;
	LPSS_CELLCOORD lpcc = (LPSS_CELLCOORD)lParam;
	int master, port, slave, io;
	int i, nRet = 0;
	char chvalue[100];
	CString strTmp;
	int n_response = 0;
	int count;
	
	CDialog_Message msg_dlg;
	
	if (wParam==IDC_CUSTOM_IO)
	{
		if (st_handler.nMenuLock == TRUE) return;
		if (lpcc->Row<4) return;
		
		Grid_Map = (TSpread*)GetDlgItem(IDC_CUSTOM_IO);

		if (lpcc->Col == 4)
		{
			strTmp = m_p_grid.GridCellText(Grid_Map, lpcc->Row, 1);
			
			if (strTmp == "-")
			{
				return;
			}

			io = atoi(strTmp) - 1;

			if (mn_io_type[mn_port][mn_slave][io] == IO_OUT)
			{
				strTmp = m_p_grid.GridCellText(Grid_Map, lpcc->Row, 4);

				if (strTmp == "N")
				{
					m_p_grid.GridCellColor(Grid_Map, lpcc->Row, 4, YELLOW_C, BLACK_C);
					m_p_grid.GridCellText(Grid_Map,  lpcc->Row, 4, "Y");
				}
				else
				{
					m_p_grid.GridCellColor(Grid_Map, lpcc->Row, 4, RGB(255,200,200), RGB(25,25,85));
					m_p_grid.GridCellText(Grid_Map,  lpcc->Row, 4, "N");
				}
			}
		}
		else if (lpcc->Col == 5)
		{
			strTmp = m_p_grid.GridCellText(Grid_Map, lpcc->Row, 1);
			
			if (strTmp == "-")	
			{
				return;
			}
			
			io = atoi(strTmp) - 1;

			strTmp = m_p_grid.GridCellText(Grid_Map, lpcc->Row, 6);
			mstr_io_name = strTmp;

			if (mn_action_cnt_set > 1)
			{
				count = 0;
				
				for (i = 0; i < 16; i++)
				{
					if (mn_io_type[mn_port][mn_slave][i] == IO_IN)
					{
						mn_output_info[i] = CTL_NO;
					}
					else if (mn_io_type[mn_port][mn_slave][i] == IO_OUT)
					{
						strTmp = m_p_grid.GridCellText(Grid_Map, i+4, 4);
						
						if (strTmp == "Y")
						{
							count++;
							mn_output_info[i] = CTL_YES;
						}
						else	
						{
							mn_output_info[i] = CTL_NO;
						}
					}
				}
				
				if (count == 0)
				{
					if (mn_io_type[mn_port][mn_slave][io] == IO_OUT)
					{
						mn_output_info[io] = CTL_YES;
						m_p_grid.GridCellText(Grid_Map, lpcc->Row, 4, "1");
					}
					else
					{
						return;
					}
				}
			}

			strTmp.Format("[IO] Port(%d) Slave(%d) IO(%d)", mn_port, mn_slave, io);
			cLOG.OnLogEvent( LOG_PGM_, strTmp );

			OnIO_OnOutputData(mn_port, mn_slave, io);
		}

		// 규이리 추가 [2015.02.16]
		if (Grid_Map != NULL)
		{
			Grid_Map = NULL;
			delete Grid_Map;
		}
	}
	else if(wParam==IDC_CUSTOM_LIST)
	{
		Grid_List = (TSpread*)GetDlgItem(IDC_CUSTOM_LIST);
		
		Grid_List->GetValue(1, lpcc->Row, chvalue);
		strTmp.Format("%s", chvalue);
		
		master = atoi(strTmp) / 40;
		port = atoi(strTmp) / 10;
		slave = atoi(strTmp) % 10;
		
		if(port == mn_port && slave == mn_slave) return;
		
		mn_master = master;
		mn_port = port;
		mn_slave = slave;
		
		for(i=0; i<mn_list_count; i++)
		{
			GridColor(IDC_CUSTOM_LIST, i+1, 1, WHITE_C, BLACK_C);
			GridColor(IDC_CUSTOM_LIST, i+1, 2, WHITE_C, BLACK_C);
		}
		
		GridColor(IDC_CUSTOM_LIST, lpcc->Row, 1, SKY_C, BLACK_C);
		GridColor(IDC_CUSTOM_LIST, lpcc->Row, 2, SKY_C, BLACK_C);
		
		OnIO_OnChangeSheet(mn_port, mn_slave);
		
		// 규이리 추가 [2015.02.16]
		if (Grid_List != NULL)
		{
			Grid_List = NULL;
			delete Grid_List;
		}
	}
}

void CScreen_IO::OnIO_OnOutputData(int port, int slave, int io)
{
	int pos;
	int output;
	int nChk[2] = {0,};
	double dMotorPosition = 0;

	if(st_handler.nMenuLock == TRUE) return;
	
	if(mn_io_type[port][slave][io] == IO_OUT)
	{
		pos = (mn_io_module[port][slave]*100)+io;
		
		output = g_ioMgr.get_out_bit( pos, 0 );
		
		if (mn_action_cnt_set > 1)
		{
			mn_stop_req = FALSE;  // ESTOP 요청 플래그 초기화
			mn_action_step = 0;
			mn_action_cnt_chk = 1;
			
			mn_conti_io_no = pos;
			mn_conti_io_flag = !output;

			OnIO_Controls_Enable(false);			// 모든 컨트롤 화면 출력 제어 함수
			
			if(st_handler.nMenuLock == FALSE)
			{
				st_handler.nMenuLock = TRUE;
			}
			
			SetTimer(TMR_IO_CONTINUE, 50, NULL);	// I/O 상태 출력 타이머 생성
		}
		else
		{	
			g_ioMgr.set_out_bit(pos, !output);
			
			if (!output == IO_ON)
			{
				mstr_io_act = "On";
			}
			else
			{
				mstr_io_act = "Off";
			}
			
			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
			{
				sprintf(st_msg.c_normal_msg, "%s did  %s", mstr_io_name, mstr_io_act);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
			}
		}
		
	}
}

void CScreen_IO::OnIO_Controls_Enable(bool b_state)
{

}

void CScreen_IO::OnIO_OnInputCheck(int port, int slave)
{
	int i;
	int input;
	int data;

	TSpread *Grid = (TSpread*)GetDlgItem(IDC_CUSTOM_IO);

	input = g_ioMgr.Get_In_Word(mn_io_module[port][slave]);
	
	for(i=0; i<16; i++)
	{
		if(mn_io_type[port][slave][i] == IO_IN && mstr_io_description[port][slave][i] != "")
		{
			data = (input>>i) & 0x01;
			
			if(data == IO_ON)
			{
				m_p_grid.GridCellColor(Grid, i+4, 5, RGB(0,128,0), WHITE_C);
				m_p_grid.GridCellFont(Grid, i+4, 5, "MS Sans Serif", 14);
				m_p_grid.GridCellText(Grid, i+4, 5, "ON");
			}
			else
			{
				m_p_grid.GridCellColor(Grid, i+4, 5, RGB(128,255,128), BLACK_L);
				m_p_grid.GridCellFont(Grid, i+4, 5, "MS Sans Serif", 14);
				m_p_grid.GridCellText(Grid, i+4, 5, "OFF");
			}
		}
	}

	// 규이리 추가 [2015.02.16]
	if (Grid != NULL)
	{
		Grid = NULL;
		delete Grid;
	}
}

int CScreen_IO::OnIO_Cylinder_Excution()
{
	int mn_action_flag = RET_PROCEED;
	CString str_temp, sTempMSG;
	char chr_buf[20];
	int out_data;
	int val = 0;
	int i = 0;
	int port = 0;
	int nAction;

	// **************************************************************************
	// 모터 동작 중 ESTOP 요청 발생했는지 검사한다                               
	// -> ESTOP 요청 발생한 경우 긴급 정지로 모터 멈춘다                         
	// **************************************************************************
	if (mn_stop_req == TRUE)
	{
		if (mn_action_step < 1000)
			mn_action_step = 1000;
	}
	// **************************************************************************

	switch(mn_action_step)
	{
	case 0:
		port = (mn_port * 1000) + (mn_slave * 100);
		out_data = g_ioMgr.Get_Out_Word(port);
		
		for (i = 0; i < 16; i++)
		{
			if (mn_output_info[i] == CTL_YES)
			{
				nAction = g_ioMgr.get_out_bit(port + i, IO_OFF);
				g_ioMgr.Set_Out_Bit(port + i, !nAction);	
			}
		}
		
		if (st_handler.cwnd_list != NULL)
		{
			sTempMSG.Format("[%d][%d][%d][%d][%d][%d][%d][%d][%d][%d][%d][%d][%d][%d][%d][%d] Word Output",
				mn_output_info[0], mn_output_info[1], mn_output_info[2], mn_output_info[3],
				mn_output_info[4], mn_output_info[5], mn_output_info[6], mn_output_info[7],
				mn_output_info[8], mn_output_info[9], mn_output_info[10], mn_output_info[11],
				mn_output_info[12], mn_output_info[13], mn_output_info[14], mn_output_info[15]);
			
			sprintf(st_msg.c_normal_msg, "%s", sTempMSG);
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
		}
		
		mn_conti_io_flag = !mn_conti_io_flag;
		mn_conti_wait_time[0] = GetCurrentTime();
		mn_action_step = 100;
		break;
		
	case 100:
		// *************************************************************
		//  잠시 대기후 Retry 진행                                      
		// *************************************************************
		mn_conti_wait_time[1] = GetCurrentTime();
		mn_conti_wait_time[2] = mn_conti_wait_time[1] - mn_conti_wait_time[0];
		
		if (mn_conti_wait_time[2] > mn_conti_wait)
		{
			mn_action_step = 200;
		}
		break;
		
	case 200:
		port = (mn_port * 1000) + (mn_slave * 100);
		out_data = g_ioMgr.Get_Out_Word(port);
		
		for (i = 0; i < 16; i++)
		{
			if (mn_output_info[i] == CTL_YES)
			{
				nAction = g_ioMgr.get_out_bit(port + i, IO_ON);
				g_ioMgr.set_out_bit(port + i, !nAction );
			}
		}
		
		port = (mn_port*10) + mn_slave;
		
		if (st_handler.cwnd_list != NULL)
		{
			sTempMSG.Format("[%d][%d][%d][%d][%d][%d][%d][%d][%d][%d][%d][%d][%d][%d][%d][%d] Word Output", 
				mn_output_info[0], mn_output_info[1], mn_output_info[2], mn_output_info[3],
				mn_output_info[4], mn_output_info[5], mn_output_info[6], mn_output_info[7],
				mn_output_info[8], mn_output_info[9], mn_output_info[10], mn_output_info[11],
				mn_output_info[12], mn_output_info[13], mn_output_info[14], mn_output_info[15]);
			
			sprintf(st_msg.c_normal_msg, "%s", sTempMSG);
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
		}
		
		mn_conti_io_flag = !mn_conti_io_flag;
		mn_conti_wait_time[0] = GetCurrentTime();
		mn_action_step = 300;
		break;
		
	case 300:
		// *************************************************************
		//  잠시 대기후 Retry 진행                                      
		// *************************************************************
		mn_conti_wait_time[1] = GetCurrentTime();
		
		mn_conti_wait_time[2] = mn_conti_wait_time[1] - mn_conti_wait_time[0];
		
		if (mn_conti_wait_time[2] > mn_conti_wait)
		{
			mn_action_step = 400;
		}
		break;
		
	case 400:
		str_temp = LPCTSTR(_itoa(mn_action_cnt_chk, chr_buf, 10));
		
		if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
		{
			sprintf(st_msg.c_normal_msg, str_temp + "th repeat action.");
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG); 
		}
		
		mn_action_cnt_chk++;		
		
		if(mn_action_cnt_chk > mn_action_cnt_set)	mn_action_step = 500;
		else	mn_action_step = 0;
		
		break;
		
	case 500:
		sprintf(st_msg.c_normal_msg, "[Repeat action] The action was completed.!");
		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG); 
		mn_action_flag = RET_GOOD;
		break;
		
	case 1000:
		sprintf(st_msg.c_abnormal_msg, "A Repeat action became the cancel.");
		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG); 
		mn_action_flag = RET_GOOD;
		
		if (mn_stop_req != FALSE)
			mn_stop_req = FALSE;  // ESTOP 요청 플래그 초기화
		break;
	}
	return mn_action_flag;
}
