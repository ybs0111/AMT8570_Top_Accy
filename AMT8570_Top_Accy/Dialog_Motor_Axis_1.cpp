// Dialog_Motor_Axis_1.cpp : implementation file
//

#include "stdafx.h"
#include "handler.h"
#include "Dialog_Motor_Axis_1.h"
#include "Public_Function.h"
#include "Cmmsdk.h"
#include "Dialog_KeyPad.h"
#include "Dialog_Select.h"

#include "CtlBd_Function.h"
#include "CtlBd_Library.h"

#include "Seq_MotMove.h"
#include "COMI_Manager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialog_Motor_Axis_1 dialog
#define TM_STATUS_CHECK		100
#define TM_POSITION_CHECK	200
#define TM_HOME_CHECK		300
#define TM_MOVE_CHECK		400

#define AXIS_NUM			0
CDialog_Motor_Axis_1::CDialog_Motor_Axis_1(CWnd* pParent /*=NULL*/)
	: CDialog(CDialog_Motor_Axis_1::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialog_Motor_Axis_1)
	//}}AFX_DATA_INIT
	m_n_move_mode = 0;
}


void CDialog_Motor_Axis_1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialog_Motor_Axis_1)
	DDX_Control(pDX, IDC_BTN_APPLY, m_btn_apply);
	DDX_Control(pDX, IDC_BTN_RIGHT, m_btn_right);
	DDX_Control(pDX, IDC_BTN_LEFT, m_btn_left);
	DDX_Control(pDX, IDC_BTN_HOME, m_btn_home);
	DDX_Control(pDX, IDC_GROUP_AXIS_INFO, m_group_axis_info);
	DDX_Control(pDX, IDC_GROUP_MOTOR, m_group_motor);
	DDX_Control(pDX, IDC_GROUP_MOVE, m_group_move);
	DDX_Control(pDX, IDC_BTN_EMG, m_btn_emg);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialog_Motor_Axis_1, CDialog)
	//{{AFX_MSG_MAP(CDialog_Motor_Axis_1)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_EMG, OnBtnEmg)
	ON_BN_CLICKED(IDC_BTN_HOME, OnBtnHome)
	ON_BN_CLICKED(IDC_BTN_APPLY, OnBtnApply)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
	ON_MESSAGE(SSM_CLICK  , OnCellClick)
	ON_MESSAGE(WMU_CHILD_LBUTTON_DOWN, OnUserLButtonDown)
	ON_MESSAGE(WMU_CHILD_LBUTTON_UP, OnUserLButtonUp)
	ON_MESSAGE( WM_MOTOR_COMPLETION, OnMotorCompletion)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialog_Motor_Axis_1 message handlers

BOOL CDialog_Motor_Axis_1::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_p_font = NULL;
	m_p_font = new CFont;
	m_p_font->CreateFont(14,0,0,0,900,0,0,0,0,0,0,ANTIALIASED_QUALITY,0,"MS Sans Serif");

	m_d_data = 0;
	
// 	Data_Init();
// 
// 	Init_Grid_Pos();
// 	Init_Label();
	Init_Group();
// 	Init_Digit();
// 	Init_Radio();
// 	Init_Button();

	if (COMI.mn_motorbd_init_end)
	{
		SetTimer(TM_STATUS_CHECK, 100, NULL);
		SetTimer(TM_POSITION_CHECK, 100, NULL);
	}

	st_handler.cwnd_motorAxis = this;
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialog_Motor_Axis_1::Init_Group()
{
	CSxLogFont main_font(15,FW_SEMIBOLD,false,"Arial");
	
	m_group_motor.SetFont(main_font);
	m_group_motor.SetCatptionTextColor(RGB(145,25,0));
	m_group_motor.SetFontBold(TRUE);
	m_group_motor.SetWindowText(m_str_axis_name);

	m_group_move.SetFont(main_font);
	m_group_move.SetCatptionTextColor(RGB(145,25,0));
	m_group_move.SetFontBold(TRUE);
	m_group_move.SetWindowText("Move");

	m_group_axis_info.SetFont(main_font);
	m_group_axis_info.SetCatptionTextColor(RGB(145,25,0));
	m_group_axis_info.SetFontBold(TRUE);
	m_group_axis_info.SetWindowText("Position Information");
}

//==================================================================//
// 칼라 버튼 생성
//==================================================================//
void CDialog_Motor_Axis_1::Init_Button() 
{
	CString strTmp;
	short shBtnColor = 30;

	m_btn_emg.SetFont(m_p_font);
	m_btn_emg.SetIcon(IDI_EMG_STOP);
	m_btn_emg.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btn_emg.SetButtonColor(1);
	strTmp.Format("%s Emergency Step", m_str_axis_name);
	m_btn_emg.SetTooltipText(strTmp);

	m_btn_home.SetTag(IDC_BTN_HOME);
	m_btn_home.SetFont(m_p_font);
	m_btn_home.SetIcon(IDI_HOME);
	m_btn_home.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btn_home.SetButtonColor(1);

	/*
	if (m_str_axis_name == "Loader_Tray_Transfer_X" || m_str_axis_name == "Loader_Transfer_X")
	{
		m_btn_left.SetIcon(IDI_LEFT);
	}
	else
	{
		m_btn_left.SetIcon(IDI_DN);
	}
	m_btn_left.SetTag(IDC_BTN_LEFT);
	m_btn_left.SetFont(m_p_font);
	m_btn_left.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btn_left.SetButtonColor(1);

	if (m_str_axis_name == "Loader_Tray_Transfer_X" || m_str_axis_name == "Loader_Transfer_X")
	{
		m_btn_right.SetIcon(IDI_RIGHT);
	}
	else
	{
		m_btn_right.SetIcon(IDI_UP);
	}
	m_btn_right.SetTag(IDC_BTN_RIGHT);
	m_btn_right.SetFont(m_p_font);
	m_btn_right.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btn_right.SetButtonColor(1);
	*/

	// 규이리 수정 [2015.02.12]
	switch(m_n_direction)
	{
	case 0:
		m_btn_left.SetIcon(IDI_LEFT);
		m_btn_right.SetIcon(IDI_RIGHT);
		break;
	case 1:
		m_btn_left.SetIcon(IDI_RIGHT);
		m_btn_right.SetIcon(IDI_LEFT);
		break;
	case 2:
		m_btn_left.SetIcon(IDI_UP);
		m_btn_right.SetIcon(IDI_DN);
		break;
	case 3:
		m_btn_left.SetIcon(IDI_DN);
		m_btn_right.SetIcon(IDI_UP);
		break;
	}

	m_btn_left.SetTag(IDC_BTN_LEFT);
	m_btn_left.SetFont(m_p_font);
	m_btn_left.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btn_left.SetButtonColor(1);

	m_btn_right.SetTag(IDC_BTN_RIGHT);
	m_btn_right.SetFont(m_p_font);
	m_btn_right.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btn_right.SetButtonColor(1);
}

void CDialog_Motor_Axis_1::Init_Label()
{

}

void CDialog_Motor_Axis_1::Init_Radio()
{

}

void CDialog_Motor_Axis_1::Init_Digit()
{

}

void CDialog_Motor_Axis_1::Init_Grid_Pos()
{
	int   i, j;
	CString str_tmp;
	m_grid_motor_pos = (TSpread*)GetDlgItem(IDC_CUSTOM_MOTOR_POS);

//	m_p_grid.GridReset(m_grid_motor_pos);
	// 대문자 
	m_p_grid.GridCellSelectDisplay(m_grid_motor_pos, TRUE);
	
	m_p_grid.GridRowHeader(m_grid_motor_pos, FALSE);
	m_p_grid.GridColHeader(m_grid_motor_pos, TRUE);
	m_p_grid.GridHorizontal(m_grid_motor_pos, FALSE);
	m_p_grid.GridVertical(m_grid_motor_pos, TRUE);
	m_p_grid.GridAutoSize(m_grid_motor_pos, FALSE);
	m_p_grid.GridAutoSize(m_grid_motor_pos, FALSE);
	m_p_grid.GridCellRows(m_grid_motor_pos, m_n_position_num);
	m_p_grid.GridCellCols(m_grid_motor_pos, 4);
	
	for(i=0; i<m_n_position_num+1; i++)
	{
		m_p_grid.GridCellHeight_L(m_grid_motor_pos, i, 30);
		for(j=0; j<4; j++)
		{
			
			switch(j)
			{
				case 0:
					m_p_grid.GridCellWidth_L(m_grid_motor_pos, j+1, 25);
					break;
				case 1:
					m_p_grid.GridCellWidth_L(m_grid_motor_pos, j+1, 15);
					break;
				case 2:
					m_p_grid.GridCellWidth_L(m_grid_motor_pos, j+1, 11);
					break;
				case 3:
					m_p_grid.GridCellWidth_L(m_grid_motor_pos, j+1, 11);
					break;
			}
			m_p_grid.GridCellControlStatic(m_grid_motor_pos, i, j+1);
		}
	}
	
	m_p_grid.GridCellFont(m_grid_motor_pos, 0, 1, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(m_grid_motor_pos, 0, 1, BLACK_C, YELLOW_C);
	m_p_grid.GridCellText(m_grid_motor_pos, 0, 1, "-");
	
	m_p_grid.GridCellFont(m_grid_motor_pos, 0, 2, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(m_grid_motor_pos, 0, 2, BLACK_C, YELLOW_C);
	m_p_grid.GridCellText(m_grid_motor_pos, 0, 2, "Positon");
	
	m_p_grid.GridCellFont(m_grid_motor_pos, 0, 3, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(m_grid_motor_pos, 0, 3, BLACK_C, YELLOW_C);
	m_p_grid.GridCellText(m_grid_motor_pos, 0, 3, "Move");
	
	m_p_grid.GridCellFont(m_grid_motor_pos, 0, 4, "MS Sans Serif", 15);
	m_p_grid.GridCellColor(m_grid_motor_pos, 0, 4, BLACK_C, YELLOW_C);
	m_p_grid.GridCellText(m_grid_motor_pos, 0, 4, "Read");
	
	for(i=0; i<m_n_position_num; i++)
	{
		m_p_grid.GridCellColor(m_grid_motor_pos, i+1, 1, BLACK_C, YELLOW_C);
		m_p_grid.GridCellText(m_grid_motor_pos, i+1, 1, m_str_position_name[i]);
		
		m_p_grid.GridCellFont(m_grid_motor_pos, i+1, 2, "MS Sans Serif", 10);
		m_p_grid.GridCellColor(m_grid_motor_pos, i+1, 2, GREEN_L, BLACK_C);
		str_tmp.Format("%.3f", m_d_position[i][1]);
		m_p_grid.GridCellText(m_grid_motor_pos, i+1, 2, str_tmp);
		
		m_p_grid.GridCellFont(m_grid_motor_pos, i+1, 3, "MS Sans Serif", 15);
		m_p_grid.GridCellColor(m_grid_motor_pos, i+1, 3, TEXT_BC, BLUE_C);
		m_p_grid.GridCellText(m_grid_motor_pos, i+1, 3, "Go.");
		
		m_p_grid.GridCellFont(m_grid_motor_pos, i+1, 4, "MS Sans Serif", 15);
		m_p_grid.GridCellColor(m_grid_motor_pos, i+1, 4, TEXT_BC, RED_C);
		m_p_grid.GridCellText(m_grid_motor_pos, i+1, 4, "Read.");
	}

	// 규이리 추가 [2015.02.16]
	if (m_grid_motor_pos != NULL)
	{
		m_grid_motor_pos = NULL;
		delete m_grid_motor_pos;
	}
}

void CDialog_Motor_Axis_1::Data_Apply()
{
	int i;
	
	for(i=0; i<m_n_position_num; i++)
	{
		st_motor[m_n_axis].d_pos[i]		= m_d_position[i][1];
	}
}

void CDialog_Motor_Axis_1::Data_Backup()
{
	int i;
	
	for(i=0; i<m_n_position_num; i++)
	{
		m_d_position[i][0]	= m_d_position[i][1];
	}
}

int CDialog_Motor_Axis_1::Data_Comparison()
{
	int i;
	int Ret = CTLBD_RET_GOOD;
	
	for(i=0; i<m_n_position_num; i++)
	{
		if (m_d_position[i][0] != m_d_position[i][1])
		{
			return CTLBD_RET_ERROR;
		}
	}
	
	return CTLBD_RET_GOOD;
}

void CDialog_Motor_Axis_1::Data_HistoryLog()
{
	int i;
	CString str_msg;
	
	for(i=0; i<m_n_position_num; i++)
	{
		if (m_d_position[i][0] != m_d_position[i][1])
		{
			str_msg.Format( "[%s] Axis Change %.3f -> %.3f", m_str_axis_name, m_d_position[i][0], m_d_position[i][1]);
			cLOG.OnLogEvent( LOG_PGM_, str_msg );
		}
	}
}

void CDialog_Motor_Axis_1::Data_Recovery()
{
	int i;
	
	for(i=0; i<m_n_position_num; i++)
	{
		m_d_position[i][1]	= m_d_position[i][0];
	}

}

//==================================================================//
// 구조체값을 클래스 변수로 복사
//==================================================================//
void CDialog_Motor_Axis_1::Data_Init(int nSelected) 
{
	m_n_move_mode		= 0;
	m_n_axis			= st_motor_info.n_part_axis[m_n_part][nSelected];
	m_n_position_num	= st_motor_info.n_axis_pos_num[m_n_part][nSelected];

	int i=0;
	for(i=0; i<m_n_position_num; i++)
	{
		m_str_position_name[i]	= st_motor_info.str_axis_pos_name[m_n_part][nSelected][i];
		m_str_position_bmp[i]	= st_motor_info.str_axis_pos_info[m_n_part][nSelected][i];
	}

	m_str_axis_name	= st_motor_info.str_part_axis_name[m_n_part][nSelected];
	m_n_minus_el	= st_motor_info.n_axis_minus_el[m_n_part][nSelected];
	m_n_plus_el		= st_motor_info.n_axis_plus_el[m_n_part][nSelected];
	m_n_home		= st_motor_info.n_axis_home[m_n_part][nSelected];
	m_n_sd			= st_motor_info.n_axis_sd[m_n_part][nSelected];
	m_n_direction	= st_motor_info.n_axis_direction[m_n_part][nSelected];
	m_n_account		= st_motor_info.n_axis_account[m_n_part][nSelected];
	
	for(i=0; i<m_n_position_num; i++)
	{
		m_d_position[i][1]	= st_motor[m_n_axis].d_pos[i];
	}

	Data_Backup();  // 현재 위치값 백업 [배열0: 백업 데이터, 배열1: 현재 데이터]
}

//==================================================================//
// 모터 상태를 스플래드시트에 표시
//==================================================================//
void CDialog_Motor_Axis_1::OnDisp_MotorStatus() 
{
	// 규이리 수정 [2015.03.05]
	// 타이머에서 계속 호출되므로 변수를 함수 변수로 변경함
	TSpread *gridMotInfo;
	gridMotInfo = (TSpread*)GetDlgItem(IDC_CUSTOM_MOTOR_INFO);

	if (m_n_home == 0)  // HOME 센서 미사용
	{
		if (m_n_minus_el == 1)  // -LIMIT 센서를 HOME 센서로 사용
		{
			if (COMI.Get_MotIOSensor(m_n_axis, MOT_SENS_ELM) == CTLBD_RET_GOOD)
			{
				m_p_grid.GridCellColor(gridMotInfo, 2, 1, GREEN_C, WHITE_C);
			}
			else
			{
				m_p_grid.GridCellColor(gridMotInfo, 2, 1, BLACK_L, WHITE_C);
			}
		}

		if (m_n_plus_el == 1)  // +LIMIT 센서 존재
		{
			if (COMI.Get_MotIOSensor(m_n_axis, MOT_SENS_ELP) == CTLBD_RET_GOOD)
			{
				m_p_grid.GridCellColor(gridMotInfo, 2, 4, GREEN_C, WHITE_C);
			}
			else
			{
				m_p_grid.GridCellColor(gridMotInfo, 2, 4, BLACK_L, WHITE_C);
			}
		}
	}
	else  // HOME 센서 존재
	{
		if (m_n_minus_el == 1)
		{
			if (COMI.Get_MotIOSensor(m_n_axis, MOT_SENS_ELM) == CTLBD_RET_GOOD)
			{
				m_p_grid.GridCellColor(gridMotInfo, 2, 1, GREEN_C, WHITE_C);
			}
			else
			{
				m_p_grid.GridCellColor(gridMotInfo, 2, 1, BLACK_L, WHITE_C);
			}
		}

		if (COMI.Get_MotIOSensor(m_n_axis, MOT_SENS_HOME) == CTLBD_RET_GOOD)
		{
			m_p_grid.GridCellColor(gridMotInfo, 2, 3, GREEN_C, WHITE_C);
		}
		else
		{
			m_p_grid.GridCellColor(gridMotInfo, 2, 3, BLACK_L, WHITE_C);
		}

		if (m_n_plus_el == 1)
		{
			if (COMI.Get_MotIOSensor(m_n_axis, MOT_SENS_ELP) == CTLBD_RET_GOOD)
			{
				m_p_grid.GridCellColor(gridMotInfo, 2, 5, GREEN_C, WHITE_C);
			}
			else
			{
				m_p_grid.GridCellColor(gridMotInfo, 2, 5, BLACK_L, WHITE_C);
			}
		}
	}

	if (COMI.Get_MotPower(m_n_axis) == CTLBD_RET_GOOD)
	{
		m_p_grid.GridCellColor(gridMotInfo, 4, 1, GREEN_C, WHITE_C);
	}
	else
	{
		m_p_grid.GridCellColor(gridMotInfo, 4, 1, BLACK_L, WHITE_C);
	}

	if (COMI.Get_MotAlarmStatus(m_n_axis) == CTLBD_RET_GOOD)
	{
		m_p_grid.GridCellColor(gridMotInfo, 6, 1, BLACK_L, WHITE_C);
	}
	else
	{
		m_p_grid.GridCellColor(gridMotInfo, 6, 1, RED_C, WHITE_C);
	}

	// 규이리 추가 [2015.02.16]
	if (gridMotInfo != NULL)
	{
		gridMotInfo = NULL;
		delete gridMotInfo;
	}
}

void CDialog_Motor_Axis_1::MotorPositionCheck()
{
	// 규이리 수정 [2015.03.05]
	// 타이머에서 계속 호출되므로 변수를 함수 변수로 변경함
	TSpread *gridMotInfo;
	gridMotInfo = (TSpread*)GetDlgItem(IDC_CUSTOM_MOTOR_INFO);

	CString str_tmp;
	str_tmp.Format("%.3f", g_comiMgr.Get_MotCurrentPos(m_n_axis));
	m_p_grid.GridCellText(gridMotInfo, 8, 3, str_tmp);

	// 규이리 추가 [2015.02.16]
	if (gridMotInfo != NULL)
	{
		gridMotInfo = NULL;
		delete gridMotInfo;
	}
}

void CDialog_Motor_Axis_1::OnCellClick(WPARAM wParam, LPARAM lParam)
{
	LPSS_CELLCOORD lpcc = (LPSS_CELLCOORD)lParam;
	CDialog_Message msg_dlg;

	CString strTmp, strChange, str_file;
	CString str_tmp, str_data;
	double value;
	int nResponse;

	CString strText;
	
	CDialog_KeyPad	pad_dlg;
	
	if (st_handler.nMenuLock) return;

	// 규이리 추가 [2014.08.27]
	CString sPath;

	if (wParam == IDC_CUSTOM_MOTOR_POS)
	{
		if (lpcc->Row == 0) return;

		m_grid_motor_pos = (TSpread*)GetDlgItem(IDC_CUSTOM_MOTOR_POS);
		
		m_n_cmd_row	= lpcc->Row;
		m_n_cmd_col	= lpcc->Col;

		switch(lpcc->Col)
		{
		case 1:
			sPath = BMP_PATH_BASE_;
			str_file.Format("%s%s", sPath, m_str_position_bmp[lpcc->Row-1]);
			m_p_bmp_view.Load(str_file);
			
			Invalidate(FALSE);
			break;

		case 2:				
			strText = m_p_grid.GridCellText(m_grid_motor_pos, lpcc->Row, lpcc->Col - 1);
			st_msg.mstr_keypad_msg.Format("[%s Motor] %s set", m_str_axis_name, strText);

			strTmp = m_p_grid.GridCellText(m_grid_motor_pos, lpcc->Row, lpcc->Col);
			st_msg.mstr_keypad_val = strTmp;
			
			strTmp = KeyPad.GetNumEditString_D(COMI.md_limit_position[m_n_axis][0], COMI.md_limit_position[m_n_axis][1], double( atof(st_msg.mstr_keypad_val) ), st_msg.mstr_keypad_msg);
			
			m_d_position[lpcc->Row-1][1] = atof(strTmp);
			m_p_grid.GridCellText(m_grid_motor_pos, lpcc->Row, lpcc->Col, strTmp);
			break;

		case 3:
			{
				//if (st_handler.mn_motor_init_check != CTL_YES)	// 모터 초기화를 하지 않았다면 동작하지 않는다. 2K4/11/23/ViboX
				if (COMI.mn_motorbd_init_end != TRUE) 
				{
					st_msg.str_fallacy_msg = _T("First, you may initialize motion board.");
					
					nResponse = msg_dlg.DoModal();
					if (nResponse == IDOK) return ;
				}

				m_p_grid.GridCellText(m_grid_motor_pos, lpcc->Row, lpcc->Col, "ing...");
				m_p_grid.GridCellColor(m_grid_motor_pos, lpcc->Row, lpcc->Col, NOR_C, BLACK_C);

				double dMotGotoPos = m_d_position[lpcc->Row-1][1];
				GETMOTOR( (ENUM_MOTOR)m_n_axis ).SetMotGotoPos( dMotGotoPos );
				
				nResponse = Func.OnMotor_Go_Check(m_n_axis, dMotGotoPos);
				
				if (nResponse != IDOK)
				{
					MotorCompletion(m_n_cmd_row, m_n_cmd_col);
					return;
				}
				
				if (dMotGotoPos < 0)
				{
					MotorCompletion(m_n_cmd_row, m_n_cmd_col);
					return;
				}
				
				if (st_handler.nMenuLock != TRUE)
				{
					st_handler.nMenuLock = TRUE;
				}
				// =============================================================================
				
				GETMOTOR( (ENUM_MOTOR)m_n_axis ).MotorStepClear();
				GETMOTOR( (ENUM_MOTOR)m_n_axis ).MotorMoveStart( dMotGotoPos, st_handler.md_manual_speed, MMMODE_MANUAL );
			}
			break;
				
		case 4:
			//if (st_handler.mn_motor_init_check != CTL_YES)
			if (COMI.mn_motorbd_init_end != TRUE) 
			{
				st_msg.str_fallacy_msg = _T("First, you may initialize motion board.");
				
				nResponse = msg_dlg.DoModal();
				if (nResponse == IDOK) return ;
			}
		
//			value = COMI.Get_MotCurrentPos(m_n_axis);  // FASTECH 모터 특정 축의 커맨트 위치 리턴 함수 //
			value = g_comiMgr.Get_MotCurrentPos(m_n_axis);  // FASTECH 모터 특정 축의 커맨트 위치 리턴 함수 //
			
			nResponse = Func.On_Encoder_Check(value);

			if (nResponse == CTL_YES)
			{
				m_d_position[lpcc->Row-1][1] = value;
				strTmp.Format("%.3f", m_d_position[lpcc->Row-1][1]);
				m_p_grid.GridCellText(m_grid_motor_pos, lpcc->Row, 2, strTmp);	
			}
			MotorCompletion(m_n_cmd_row, m_n_cmd_col);
			break;
		}

		// 규이리 추가 [2015.02.16]
		if (m_grid_motor_pos != NULL)
		{
			m_grid_motor_pos = NULL;
			delete m_grid_motor_pos;
		}
	}
	else
	{
		m_grid_motor_info = (TSpread*)GetDlgItem(IDC_CUSTOM_MOTOR_INFO);

		switch(lpcc->Row)
		{
		case 3:
		case 4:
			switch(lpcc->Col)
			{
			case 3:
			case 4:
				if (COMI.mn_motorbd_init_end != TRUE)  
				{
					st_msg.str_fallacy_msg = _T("First, you may initialize motion board.");
					
					nResponse = msg_dlg.DoModal();
					if (nResponse == IDOK) return ;
				}
				strTmp.Format("[Motor] (%s)_Axis Power Off", m_str_axis_name);
				cLOG.OnLogEvent( LOG_PGM_, strTmp );
				
				COMI.Set_MotPower(m_n_axis, FALSE);
				break;
				
			case 5:
			case 6:
				if (COMI.mn_motorbd_init_end != TRUE)  
				{
					st_msg.str_fallacy_msg = _T("First, you may initialize motion board.");
					
					nResponse = msg_dlg.DoModal();
					if (nResponse == IDOK) return ;
				}
				strTmp.Format("[Motor] (%s)_Axis Power On", m_str_axis_name);
				cLOG.OnLogEvent( LOG_PGM_, strTmp );
				
				COMI.Set_MotPower(m_n_axis, TRUE);
				break;
			}
			break;
			
			case 5:
			case 6:
				if (lpcc->Col == 3 || lpcc->Col == 4 || lpcc->Col == 5 || lpcc->Col == 6)
				{
					if (COMI.mn_motorbd_init_end != TRUE)  
					{
						st_msg.str_fallacy_msg = _T("First, you may initialize motion board.");
						
						nResponse = msg_dlg.DoModal();
						if (nResponse == IDOK) return ;
					}
					
					strTmp.Format("[Motor] (%s)_Axis Alarm Clear", m_str_axis_name);
					cLOG.OnLogEvent( LOG_PGM_, strTmp );
					
					COMI.Set_MotAlarmClear(m_n_axis);
				}
				break;
				
			case 8:
				switch(lpcc->Col)
				{
				case 1:
					if (m_n_move_mode != 0)
					{
						m_n_move_mode = 0;
						
						m_p_grid.GridCellColor(m_grid_motor_info, 8, 1, RED_C, BLACK_C);
						m_p_grid.GridCellColor(m_grid_motor_info, 8, 2, BLACK_L, YELLOW_C);
					}
					break;
					
				case 2:
					if (m_n_move_mode != 1)
					{
						m_n_move_mode = 1;
						
						m_p_grid.GridCellColor(m_grid_motor_info, 8, 1, BLACK_L, YELLOW_C);
						m_p_grid.GridCellColor(m_grid_motor_info, 8, 2, RED_C, BLACK_C);
					}
					break;
					
				case 5:
				case 6:
					strTmp.Format("%.3f", m_d_data);
					st_msg.mstr_keypad_msg.Format("[Jog Data]  set");
					st_msg.mstr_keypad_val = strTmp;

					strTmp = KeyPad.GetNumEditString_D((double)0.001, (double)10000, double(atoi(st_msg.mstr_keypad_val)), st_msg.mstr_keypad_msg);
					
					m_d_data = atof(strTmp);
					m_p_grid.GridCellText(m_grid_motor_info, 8, 5, strTmp);
					break;
					
				}
				break;
		}

		// 규이리 추가 [2015.02.16]
		if (m_grid_motor_info != NULL)
		{
			m_grid_motor_info = NULL;
			delete m_grid_motor_info;
		}
	}
}

void CDialog_Motor_Axis_1::MotorMoveStepClear()
{
	m_b_stop_req = FALSE;	// ESTOP 요청 플래그 초기화
	
	m_n_cmd_no = 0;			// 실행 명령 번호 저장 변수 초기화
	m_n_cmd_motor_no = -1;	// 모터 번호 저장 변수 초기화
	m_str_cmd_motor = _T("Motor NULL");
	
	m_n_home_step = 0;		// 모터 HOMING 처리 스텝 저장 변수 초기화
	m_n_move_step = 0;		// 모터 이동 처리 스텝 저장 변수 초기화
}

void CDialog_Motor_Axis_1::MotorCompletion(int nrow, int ncol)
{
	m_grid_motor_pos = (TSpread*)GetDlgItem(IDC_CUSTOM_MOTOR_POS);
	
	if (ncol == 3)
	{
		m_p_grid.GridCellText(m_grid_motor_pos, nrow, ncol, "Go.");
		m_p_grid.GridCellColor(m_grid_motor_pos, nrow, ncol, TEXT_BC, BLUE_C);
	}
	else
	{
		m_p_grid.GridCellText(m_grid_motor_pos, nrow, ncol, "Read.");
		m_p_grid.GridCellColor(m_grid_motor_pos, nrow, ncol, TEXT_BC, RED_C);
	}

	// 규이리 추가 [2015.02.16]
	if (m_grid_motor_pos != NULL)
	{
		m_grid_motor_pos = NULL;
		delete m_grid_motor_pos;
	}
}

void CDialog_Motor_Axis_1::OnButtonControl(BOOL b_flag)
{

}

void CDialog_Motor_Axis_1::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CRect rect;
	CDC *pDC;

	// TODO: Add your message handler code here

	pDC = GetDlgItem(IDC_BMP_VIEW)->GetDC();
	GetDlgItem(IDC_BMP_VIEW)->GetWindowRect(rect);
	
	m_p_bmp_view.Show(pDC, CPoint(0, 0), CPoint(rect.Width(), rect.Height()), 0,0);
	
	ReleaseDC(pDC);
	// Do not call CDialog::OnPaint() for painting messages
}

//==================================================================//
// 모터 제어용 그리드 컨트롤 생성
//==================================================================//
void CDialog_Motor_Axis_1::OnSet_GridMotorCtrl() 
{
	m_grid_motor_info = (TSpread*)GetDlgItem(IDC_CUSTOM_MOTOR_INFO);

	// 규이리 추가 [2015.02.16]
	m_p_grid.GridReset(m_grid_motor_info);  // 그리드 리셋
	// 대문자
	m_p_grid.GridCellSelectDisplay(m_grid_motor_info,	TRUE);
	m_p_grid.GridRowHeader(m_grid_motor_info,	FALSE);	//헤더 미사용
	m_p_grid.GridColHeader(m_grid_motor_info,	FALSE);
	m_p_grid.GridHorizontal(m_grid_motor_info,	FALSE);	// 스크롤 미사용
	m_p_grid.GridVertical(m_grid_motor_info,	FALSE);
	m_p_grid.GridAutoSize(m_grid_motor_info,	FALSE);	// 입력값에 따른 자동 셀 사이즈 기능 해제
	m_p_grid.GridAutoSize(m_grid_motor_info,	FALSE);
	m_p_grid.GridCellRows(m_grid_motor_info,	8);		// 가로*세로 길이 설정
	m_p_grid.GridCellCols(m_grid_motor_info,	6);

	int i=0, j=0;
	for(i=0; i<8; i++)
	{
		m_p_grid.GridCellHeight_L(m_grid_motor_info, i+1, 20);  // 셀 높이 설정

		for(j=0; j<6; j++)
		{
			m_p_grid.GridCellWidth_L(m_grid_motor_info, j+1, 9.5);
			m_p_grid.GridCellControlStatic(m_grid_motor_info, i+1, j+1);
		}
	}

	if (m_n_home == 0)  // HOME 센서 미사용
	{
		if (m_n_minus_el == 1)  // -LIMIT 센서를 HOME 센서로 사용
		{
			m_p_grid.GridCellMerge(m_grid_motor_info, 1, 1, 1, 3);  // 3칸을 Merge 시킴
			m_p_grid.GridCellFont( m_grid_motor_info, 1, 1, "MS Sans Serif", 12);
			m_p_grid.GridCellColor(m_grid_motor_info, 1, 1, BLUE_D, WHITE_C);
			m_p_grid.GridCellText( m_grid_motor_info, 1, 1, "HOME");

			m_p_grid.GridCellMerge(m_grid_motor_info, 2, 1, 1, 3);
			m_p_grid.GridCellFont( m_grid_motor_info, 2, 1, "MS Sans Serif", 12);
			m_p_grid.GridCellColor(m_grid_motor_info, 2, 1, BLACK_L, WHITE_C);
			m_p_grid.GridCellText( m_grid_motor_info, 2, 1, "");  // 센서 표시 영역
		}
		else
		{
			// 규이리 추가 [2012.02.11]
			// 리미트 센서를 사용하지 않는 경우에는 그리드가 초기 상태로 있게 되어 추가함
			m_p_grid.GridCellMerge(m_grid_motor_info, 1, 1, 1, 3);  // 3칸을 Merge 시킴
			m_p_grid.GridCellFont( m_grid_motor_info, 1, 1, "MS Sans Serif", 12);
			m_p_grid.GridCellColor(m_grid_motor_info, 1, 1, BLUE_D, WHITE_C);
			m_p_grid.GridCellText( m_grid_motor_info, 1, 1, "");
			
			m_p_grid.GridCellMerge(m_grid_motor_info, 2, 1, 1, 3);
			m_p_grid.GridCellFont( m_grid_motor_info, 2, 1, "MS Sans Serif", 12);
			m_p_grid.GridCellColor(m_grid_motor_info, 2, 1, BLACK_L, WHITE_C);
			m_p_grid.GridCellText( m_grid_motor_info, 2, 1, "");  // 센서 표시 영역
		}

		if (m_n_plus_el == 1)  // +LIMIT 센서 존재
		{
			m_p_grid.GridCellMerge(m_grid_motor_info, 1, 4, 1, 3);
			m_p_grid.GridCellFont( m_grid_motor_info, 1, 4, "MS Sans Serif", 12);
			m_p_grid.GridCellColor(m_grid_motor_info, 1, 4, BLUE_D, WHITE_C);
			m_p_grid.GridCellText( m_grid_motor_info, 1, 4, "＋ PL");

			m_p_grid.GridCellMerge(m_grid_motor_info, 2, 4, 1, 3);
			m_p_grid.GridCellFont( m_grid_motor_info, 2, 4, "MS Sans Serif", 12);
			m_p_grid.GridCellColor(m_grid_motor_info, 2, 4, BLACK_L, WHITE_C);
			m_p_grid.GridCellText( m_grid_motor_info, 2, 4, "");
		}
		else
		{
			// 규이리 추가 [2012.02.11]
			// 리미트 센서를 사용하지 않는 경우에는 그리드가 초기 상태로 있게 되어 추가함
			m_p_grid.GridCellMerge(m_grid_motor_info, 1, 4, 1, 3);
			m_p_grid.GridCellFont( m_grid_motor_info, 1, 4, "MS Sans Serif", 12);
			m_p_grid.GridCellColor(m_grid_motor_info, 1, 4, BLUE_D, WHITE_C);
			m_p_grid.GridCellText( m_grid_motor_info, 1, 4, "");
			
			m_p_grid.GridCellMerge(m_grid_motor_info, 2, 4, 1, 3);
			m_p_grid.GridCellFont( m_grid_motor_info, 2, 4, "MS Sans Serif", 12);
			m_p_grid.GridCellColor(m_grid_motor_info, 2, 4, BLACK_L, WHITE_C);
			m_p_grid.GridCellText( m_grid_motor_info, 2, 4, "");
		}
	}
	else  // HOME 센서 존재
	{
		if (m_n_minus_el == 1)
		{
			m_p_grid.GridCellMerge(m_grid_motor_info, 1, 1, 1, 2);
			m_p_grid.GridCellFont( m_grid_motor_info, 1, 1, "MS Sans Serif", 12);
			m_p_grid.GridCellColor(m_grid_motor_info, 1, 1, BLUE_D, WHITE_C);
			m_p_grid.GridCellText( m_grid_motor_info, 1, 1, "－ ML");

			m_p_grid.GridCellMerge(m_grid_motor_info, 2, 1, 1, 2);
			m_p_grid.GridCellFont( m_grid_motor_info, 2, 1, "MS Sans Serif", 12);
			m_p_grid.GridCellColor(m_grid_motor_info, 2, 1, BLACK_L, WHITE_C);
			m_p_grid.GridCellText( m_grid_motor_info, 2, 1, "");
		}
		else
		{
			// 규이리 추가 [2012.02.11]
			// 리미트 센서를 사용하지 않는 경우에는 그리드가 초기 상태로 있게 되어 추가함
			m_p_grid.GridCellMerge(m_grid_motor_info, 1, 1, 1, 2);
			m_p_grid.GridCellFont( m_grid_motor_info, 1, 1, "MS Sans Serif", 12);
			m_p_grid.GridCellColor(m_grid_motor_info, 1, 1, BLUE_D, WHITE_C);
			m_p_grid.GridCellText( m_grid_motor_info, 1, 1, "");
			
			m_p_grid.GridCellMerge(m_grid_motor_info, 2, 1, 1, 2);
			m_p_grid.GridCellFont( m_grid_motor_info, 2, 1, "MS Sans Serif", 12);
			m_p_grid.GridCellColor(m_grid_motor_info, 2, 1, BLACK_L, WHITE_C);
			m_p_grid.GridCellText( m_grid_motor_info, 2, 1, "");
		}

		m_p_grid.GridCellMerge(m_grid_motor_info, 1, 3, 1, 2);
		m_p_grid.GridCellFont( m_grid_motor_info, 1, 3, "MS Sans Serif", 12);
		m_p_grid.GridCellColor(m_grid_motor_info, 1, 3, BLUE_D, WHITE_C);
		m_p_grid.GridCellText( m_grid_motor_info, 1, 3, "HOME");

		m_p_grid.GridCellMerge(m_grid_motor_info, 2, 3, 1, 2);
		m_p_grid.GridCellFont( m_grid_motor_info, 2, 3, "MS Sans Serif", 12);
		m_p_grid.GridCellColor(m_grid_motor_info, 2, 3, BLACK_L, WHITE_C);
		m_p_grid.GridCellText( m_grid_motor_info, 2, 3, "");

		if (m_n_plus_el == 1)
		{
			m_p_grid.GridCellMerge(m_grid_motor_info, 1, 5, 1, 2);
			m_p_grid.GridCellFont( m_grid_motor_info, 1, 5, "MS Sans Serif", 12);
			m_p_grid.GridCellColor(m_grid_motor_info, 1, 5, BLUE_D, WHITE_C);
			m_p_grid.GridCellText( m_grid_motor_info, 1, 5, "＋ PL");

			m_p_grid.GridCellMerge(m_grid_motor_info, 2, 5, 1, 2);
			m_p_grid.GridCellFont( m_grid_motor_info, 2, 5, "MS Sans Serif", 12);
			m_p_grid.GridCellColor(m_grid_motor_info, 2, 5, BLACK_L, WHITE_C);
			m_p_grid.GridCellText( m_grid_motor_info, 2, 5, "");
		}
		else
		{
			// 규이리 추가 [2012.02.11]
			// 리미트 센서를 사용하지 않는 경우에는 그리드가 초기 상태로 있게 되어 추가함
			m_p_grid.GridCellMerge(m_grid_motor_info, 1, 5, 1, 2);
			m_p_grid.GridCellFont( m_grid_motor_info, 1, 5, "MS Sans Serif", 12);
			m_p_grid.GridCellColor(m_grid_motor_info, 1, 5, BLUE_D, WHITE_C);
			m_p_grid.GridCellText( m_grid_motor_info, 1, 5, "");
			
			m_p_grid.GridCellMerge(m_grid_motor_info, 2, 5, 1, 2);
			m_p_grid.GridCellFont( m_grid_motor_info, 2, 5, "MS Sans Serif", 12);
			m_p_grid.GridCellColor(m_grid_motor_info, 2, 5, BLACK_L, WHITE_C);
			m_p_grid.GridCellText( m_grid_motor_info, 2, 5, "");
		}
	}

	m_p_grid.GridCellMerge(m_grid_motor_info, 3, 1, 1, 2);
	m_p_grid.GridCellFont( m_grid_motor_info, 3, 1, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(m_grid_motor_info, 3, 1, BLUE_D, WHITE_C);
	m_p_grid.GridCellText( m_grid_motor_info, 3, 1, "Motor Power");

	m_p_grid.GridCellMerge(m_grid_motor_info, 4, 1, 1, 2);
	m_p_grid.GridCellFont( m_grid_motor_info, 4, 1, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(m_grid_motor_info, 4, 1, BLACK_L, WHITE_C);
	m_p_grid.GridCellText( m_grid_motor_info, 4, 1, "");

	m_p_grid.GridCellMerge(m_grid_motor_info, 3, 3, 2, 2);
	m_p_grid.GridCellFont( m_grid_motor_info, 3, 3, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(m_grid_motor_info, 3, 3, TEXT_BC, RED_C);
	m_p_grid.GridCellText( m_grid_motor_info, 3, 3, "Power Off");

	m_p_grid.GridCellMerge(m_grid_motor_info, 3, 5, 2, 2);
	m_p_grid.GridCellFont( m_grid_motor_info, 3, 5, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(m_grid_motor_info, 3, 5, TEXT_BC, RED_C);
	m_p_grid.GridCellText( m_grid_motor_info, 3, 5, "Power On");

	
	m_p_grid.GridCellMerge(m_grid_motor_info, 5, 1, 1, 2);
	m_p_grid.GridCellFont( m_grid_motor_info, 5, 1, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(m_grid_motor_info, 5, 1, BLUE_D, WHITE_C);
	m_p_grid.GridCellText( m_grid_motor_info, 5, 1, "Motor Alarm");

	m_p_grid.GridCellMerge(m_grid_motor_info, 6, 1, 1, 2);
	m_p_grid.GridCellFont( m_grid_motor_info, 6, 1, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(m_grid_motor_info, 6, 1, BLACK_L, WHITE_C);
	m_p_grid.GridCellText( m_grid_motor_info, 6, 1, "");

	m_p_grid.GridCellMerge(m_grid_motor_info, 5, 3, 2, 4);
	m_p_grid.GridCellFont( m_grid_motor_info, 5, 3, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(m_grid_motor_info, 5, 3, TEXT_BC, RED_C);
	m_p_grid.GridCellText( m_grid_motor_info, 5, 3, "Alarm Reset");

	m_p_grid.GridCellMerge(m_grid_motor_info, 7, 1, 1, 2);
	m_p_grid.GridCellFont( m_grid_motor_info, 7, 1, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(m_grid_motor_info, 7, 1, BLUE_D, WHITE_C);
	m_p_grid.GridCellText( m_grid_motor_info, 7, 1, "Motor Move");

	m_p_grid.GridCellFont( m_grid_motor_info, 8, 1, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(m_grid_motor_info, 8, 1, GREEN_C, BLACK_C);
	m_p_grid.GridCellText( m_grid_motor_info, 8, 1, "Jog");

	m_p_grid.GridCellFont( m_grid_motor_info, 8, 2, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(m_grid_motor_info, 8, 2, BLACK_L, YELLOW_C);
	m_p_grid.GridCellText( m_grid_motor_info, 8, 2, "Distance");

	m_p_grid.GridCellMerge(m_grid_motor_info, 7, 3, 1, 2);
	m_p_grid.GridCellFont( m_grid_motor_info, 7, 3, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(m_grid_motor_info, 7, 3, BLUE_D, WHITE_C);
	m_p_grid.GridCellText( m_grid_motor_info, 7, 3, "Current Position");

	m_p_grid.GridCellMerge(m_grid_motor_info, 8, 3, 1, 2);
	m_p_grid.GridCellFont( m_grid_motor_info, 8, 3, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(m_grid_motor_info, 8, 3, WHITE_C, BLACK_C);
	m_p_grid.GridCellText( m_grid_motor_info, 8, 3, "0");

	m_p_grid.GridCellMerge(m_grid_motor_info, 7, 5, 1, 2);
	m_p_grid.GridCellFont( m_grid_motor_info, 7, 5, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(m_grid_motor_info, 7, 5, BLUE_D, WHITE_C);
	m_p_grid.GridCellText( m_grid_motor_info, 7, 5, "Move Distance (mm)");

	m_p_grid.GridCellMerge(m_grid_motor_info, 8, 5, 1, 2);
	m_p_grid.GridCellFont( m_grid_motor_info, 8, 5, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(m_grid_motor_info, 8, 5, WHITE_C, BLACK_C);
	m_p_grid.GridCellText( m_grid_motor_info, 8, 5, "10.0");

	// 규이리 추가 [2015.02.16]
	if (m_grid_motor_info != NULL)
	{
		m_grid_motor_info = NULL;
		delete m_grid_motor_info;
	}
}

void CDialog_Motor_Axis_1::OnTimer(UINT nIDEvent) 
{
	switch(nIDEvent)
	{
		case TM_STATUS_CHECK:
			OnDisp_MotorStatus();	// 모터 상태를 스플래드시트에 표시
			break;

		case TM_POSITION_CHECK:
			MotorPositionCheck();
			break;
	}
	CDialog::OnTimer(nIDEvent);
}

void CDialog_Motor_Axis_1::OnDestroy() 
{
	st_handler.cwnd_motorAxis = NULL;

	Data_HistoryLog();
	Data_Apply();
	
	if (m_p_font != NULL)
	{
		delete m_p_font;
		m_p_font = NULL;
	}
	
	KillTimer(TM_STATUS_CHECK);
	KillTimer(TM_POSITION_CHECK);
	KillTimer(TM_HOME_CHECK);
	KillTimer(TM_MOVE_CHECK);

	CDialog::OnDestroy();
}

void CDialog_Motor_Axis_1::OnBtnEmg() 
{
	GETMOTOR( (ENUM_MOTOR)m_n_axis ).SetStopReq( true );
}

// **********************************************************************
// OnUserLButtonDown() : 버튼의 WM_LBUTTONDOWN 이벤트 핸들러입니다.
// CButton 클래스는 WM_LBUTTONDOWN 이벤트를 지원하지 않으므로 CButtonST
// 이라는 클래스를 생성하여 WM_LBUTTONDOWN 이벤트를 받은후에 User-event로
// CSxMotionDlg 클래스에 전달하는 방식을 취하였습니다.
// **********************************************************************
LONG CDialog_Motor_Axis_1::OnUserLButtonDown(WPARAM wParam, LPARAM lParam)
{
	CDialog_Message msg_dlg;
	CString strTmp;
	int		nRet;
	
	nRet = Func.DoorOpenCheckSpot();
	
	if (nRet == CTLBD_RET_ERROR)
	{
// 		st_msg.str_fallacy_msg = _T("If the Open Door, does not work.");
// 		::PostMessage(st_handler.hWnd, WM_MAINFRAME_WORK, 1000, 0);
		return 0;
	}

	// Velocity-Move(Jog) 모드일때만 WM_LBUTTONDOWN 이벤트를 처리한다. //
	// Rel/Abs Position Mode에서는 WM_LBUTTONUP 이벤트에서 이송 명령을 //
	// 처리한다.                                                       //
	CButtonST *pButton = (CButtonST *)lParam;

	nRet = pButton->GetTag();

	if (pButton->GetTag() == IDC_BTN_LEFT)
	{ // "Jog (-)" 버튼이 눌린 경우...
		if (m_n_move_mode == 0)
		{
			//nRet = CTL_Lib.Motor_SafetyCheck(0, m_n_axis, 0);
			// 규이리 수정 [2014.12.03]
			nRet = CTL_Lib.Motor_SafetyCheck(REQ_JOG_MOVE_, m_n_axis, 0);
			if (nRet == CTLBD_RET_ERROR)
			{
				return 0;
			}
			
			OnButtonControl(FALSE);	
			
			strTmp.Format("[Motor] (%s)_Axis - Jog Move", m_str_axis_name);
			cLOG.OnLogEvent( LOG_PGM_, strTmp );
			
			EMOTRET motRet = GETMOTOR( (ENUM_MOTOR)m_n_axis ).JogMoveStart( MINUS );
			
			if( motRet == MOTRET_ERROR )		cmmErrShowLast(GetSafeHwnd());
			else								st_handler.nMenuLock = FALSE;
		}
		else
		{
			// 차후 이동가능 여부 체크 추가 해야됨......
			OnButtonControl(FALSE);							// 버튼 비활성화
			
			strTmp.Format("[Motor] (%s)_Axis (%.3f) - Data Move", m_str_axis_name, m_d_data);
			cLOG.OnLogEvent( LOG_PGM_, strTmp );
			
			double dMotGotoPos		= g_comiMgr.Get_MotCurrentPos(m_n_axis) - m_d_data;
			double dMotGotoSpeed	= (COMI.md_spd_jog[m_n_axis] / COMI.md_spd_vel[m_n_axis][0]) * 100;
			
			GETMOTOR( (ENUM_MOTOR)m_n_axis ).MotorStepClear();
			GETMOTOR( (ENUM_MOTOR)m_n_axis ).MotorMoveStart( dMotGotoPos, st_handler.md_manual_speed, MMMODE_DISTANCE );
		}
	}
	else if (pButton->GetTag() == IDC_BTN_RIGHT)
	{ 
		if (m_n_move_mode == 0)
		{
			//nRet = CTL_Lib.Motor_SafetyCheck(0, m_n_axis, 0);
			// 규이리 수정 [2014.12.03]
			nRet = CTL_Lib.Motor_SafetyCheck(REQ_JOG_MOVE_, m_n_axis, 0);
			if (nRet == CTLBD_RET_ERROR)
			{
				return 0;
			}
			
			OnButtonControl(FALSE);						// 버튼 비활성화
			
			strTmp.Format("[Motor] (%s)_Axis + Jog Move", m_str_axis_name);
			cLOG.OnLogEvent( LOG_PGM_, strTmp );
			
			EMOTRET motRet = GETMOTOR( (ENUM_MOTOR)m_n_axis ).JogMoveStart( PLUS );
			
			if( motRet == MOTRET_ERROR )		cmmErrShowLast(GetSafeHwnd());
			else								st_handler.nMenuLock = FALSE;
		}
		else
		{
			// 차후 이동가능 여부 체크 추가 해야됨......				
			OnButtonControl(FALSE);						// 버튼 비활성화
			
			strTmp.Format("[Motor] (%s)_Axis (%.3f) + Data Move", m_str_axis_name, m_d_data);
			cLOG.OnLogEvent( LOG_PGM_, strTmp );
			
			double dMotGotoPos		= g_comiMgr.Get_MotCurrentPos(m_n_axis) + m_d_data;
			double dMotGotoSpeed	= (COMI.md_spd_jog[m_n_axis] / COMI.md_spd_vel[m_n_axis][0]) * 100;
			
			GETMOTOR( (ENUM_MOTOR)m_n_axis ).MotorStepClear();
			GETMOTOR( (ENUM_MOTOR)m_n_axis ).MotorMoveStart( dMotGotoPos, st_handler.md_manual_speed, MMMODE_DISTANCE );
		}
	}

	return 0;
}

LONG CDialog_Motor_Axis_1::OnUserLButtonUp(WPARAM wParam, LPARAM lParam)
{	
	CDialog_Message msg_dlg;
	CString strTmp;
	int		nRet;

	nRet = Func.DoorOpenCheckSpot();
	
	if (nRet == CTLBD_RET_ERROR)
	{
// 		st_msg.str_fallacy_msg = _T("If the Open Door, does not work.");
// 		::PostMessage(st_handler.hWnd, WM_MAINFRAME_WORK, 1000, 0);
		return 0;
	}

	// Velocity-Move(Jog) 모드일때만 WM_LBUTTONDOWN 이벤트를 처리한다. //
	// Rel/Abs Position Mode에서는 WM_LBUTTONUP 이벤트에서 이송 명령을 //
	// 처리한다.                                                       //
	CButtonST *pButton = (CButtonST *)lParam;

	nRet = pButton->GetTag();

	if (pButton->GetTag() == IDC_BTN_LEFT)
	{ // "Jog (-)" 버튼이 눌린 경우...
		if (m_n_move_mode == 0)
		{
			strTmp.Format("[Motor] (%s)_Axis - Jog Stop", m_str_axis_name);
			cLOG.OnLogEvent( LOG_PGM_, strTmp );
			
			cmmSxStop(m_n_axis, FALSE, TRUE);  // 모터 이동 정지 함수 
			
			OnButtonControl(TRUE);	
		}
	}
	else if (pButton->GetTag() == IDC_BTN_RIGHT)
	{ 
		if (m_n_move_mode == 0)
		{
			strTmp.Format("[Motor] (%s)_Axis + Jog Stop", m_str_axis_name);
			cLOG.OnLogEvent( LOG_PGM_, strTmp );
			
			cmmSxStop(m_n_axis, FALSE, TRUE);  // 모터 이동 정지 함수 
			
			OnButtonControl(TRUE);	
		}
	}
	return 0;
}

void CDialog_Motor_Axis_1::OnBtnHome() 
{
	CString strTmp;

	int mn_response = 0;  // 대화 상자에 대한 리턴 값 저장 변수

	int nRet;	// 20130925 kjh

	CDialog_Select select_dlg;

	st_msg.mstr_confirm_msg = _T("Home Check를 하시겠습니까?");	
	if (st_handler.mn_language == LANGUAGE_ENGLISH) st_msg.mstr_confirm_msg = _T("Home checked Would you like to?");	 // 20130926 kjh english
	
	mn_response = select_dlg.DoModal();
	
	if (mn_response == IDOK)
	{
		
	}
	else
	{
		return;
	}
	
	nRet = CTL_Lib.Motor_SafetyCheck(REQ_HOMING_, m_n_axis, 0);	// 20130925 kjh
	
	if (nRet == BD_SAFETY)	// error -> safety
	{
		st_msg.mstr_confirm_msg = _T("Z Axis Motor Safty Please");	
		if (st_handler.mn_language == LANGUAGE_ENGLISH) st_msg.mstr_confirm_msg = _T("Z Axis Motor Safty Please");	 // 20130926 kjh english
		
		mn_response = select_dlg.DoModal();
		
		if (mn_response == IDOK)
		{
			return;
		}
		else
		{
			return;
		}
	}
	// 규이리 추가 [2015.02.13]
	else if (nRet == BD_ERROR)
	{
		return;
	}
	OnButtonControl(FALSE);  // 모든 컨트롤 화면 출력 제어 함수
	
	// ==============================================================================
	// 메뉴 사용 불가능하도록 플래그 설정한다                                    
	// -> 현재 모터 동작 중에 다른 메뉴 모터 동작시키면 오동작할 수 있기 때문이다
	// -> st_handler.i_menu_lock   : 메뉴 사용 가능 상태 플래그                  
	//  : 메인 프레임의 메뉴 전환 부분에서 이 플래그 검사하게 된다               
	// ==============================================================================
	if (st_handler.nMenuLock != TRUE)
	{
		st_handler.nMenuLock = TRUE;
	}

	// ==============================================================================
	
	// ==============================================================================
	// -> HOMING 작업 처리 함수는 CPublic_Function 클래스에 존재                 
	// ==============================================================================
	st_handler.mn_home_state[m_n_axis] = CTL_READY;	// 모터 HOMING 완료 플래그
	// ==============================================================================
	
	strTmp.Format("[Motor] (%s)_Axis Home Move", m_str_axis_name);
	cLOG.OnLogEvent( LOG_PGM_, strTmp );
	
	GETMOTOR( (ENUM_MOTOR)m_n_axis ).MotorStepClear();
	GETMOTOR( (ENUM_MOTOR)m_n_axis ).MotorHomeStart();
}

void CDialog_Motor_Axis_1::OnBtnApply() 
{
	Data_HistoryLog();
	Data_Apply();
}

LRESULT CDialog_Motor_Axis_1::OnMotorCompletion(WPARAM wParam,LPARAM lParam)
{
	if (st_handler.nMenuLock != FALSE)
	{
		st_handler.nMenuLock = FALSE;
	}
	// ==============================================================================
	
	if( wParam != MMMODE_MANUAL )
		return 0;
	
	OnButtonControl(TRUE);  // 모든 컨트롤 화면 출력 제어 함수 //
	MotorCompletion(m_n_cmd_row, m_n_cmd_col);
	
	return 0;
}

void CDialog_Motor_Axis_1::SetDataForSelected(int nSelected)
{
	Data_Init(nSelected);
	
	Init_Grid_Pos();
	OnSet_GridMotorCtrl();	// 모터 제어용 그리드 컨트롤 생성
	Init_Label();
	Init_Digit();
	Init_Radio();
	Init_Button();
}
