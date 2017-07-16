// Screen_Initial.cpp : implementation file
//

#include "stdafx.h"
#include "Handler.h"
#include "Screen_Initial.h"

//==================================================================//
// 대화상자 클래스 추가
//==================================================================//
#include "Dialog_Infor.h"
#include "Dialog_Select.h"
#include "Dialog_Message.h"
//------------------------------------------------------------------//
#include "FastechPublic_IO.h"
#include "CtlBd_Library.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScreen_Initial

IMPLEMENT_DYNCREATE(CScreen_Initial, CFormView)

CScreen_Initial::CScreen_Initial()
	: CFormView(CScreen_Initial::IDD)
{
	//{{AFX_DATA_INIT(CScreen_Initial)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CScreen_Initial::~CScreen_Initial()
{

}

void CScreen_Initial::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CScreen_Initial)
	DDX_Control(pDX, IDC_RADIO_INIT_XYZROBOT, m_radio_XYZrobot);
	DDX_Control(pDX, IDC_RADIO_INIT_LOADING_CONVEYOR, m_radio_loading_conveyor);
	DDX_Control(pDX, IDC_RADIO_INIT_BUFFER_ALIGN_CONVEYOR, m_radio_buffer_align_conveyor);
	DDX_Control(pDX, IDC_RADIO_INIT_BOX_LIFT, m_radio_box_lift);
	DDX_Control(pDX, IDC_RADIO_INIT_BOX_CLAMP, m_radio_box_clamp);
	DDX_Control(pDX, IDC_RADIO_MOT_BD, m_radio_mot_bd);
	DDX_Control(pDX, IDC_RADIO_IO_BD, m_radio_io_bd);
	DDX_Control(pDX, IDC_GIF_INITIALIZING, m_gif_initializing);
	DDX_Control(pDX, IDC_BTN_INIT_SKIP, m_btn_init_skip);
	DDX_Control(pDX, IDC_BTN_INIT_RETRY, m_btn_init_retry);
	DDX_Control(pDX, IDC_MSG_INITIAL, m_msg_initial);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CScreen_Initial, CFormView)
	//{{AFX_MSG_MAP(CScreen_Initial)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_INIT_SKIP, OnBtnInitSkip)
	ON_BN_CLICKED(IDC_BTN_INIT_RETRY, OnBtnInitRetry)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScreen_Initial diagnostics

#ifdef _DEBUG
void CScreen_Initial::AssertValid() const
{
	CFormView::AssertValid();
}

void CScreen_Initial::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CScreen_Initial message handlers

void CScreen_Initial::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();

	Func.OnReset_FlagInitialize();	// [초기화] 각종 변수

	// 컨트롤에 적용할 폰트 생성
	mp_fontInit = NULL;
	mp_fontInit = new CFont;
	mp_fontInit->CreateFont(35, 20, 0, 0, 900, 0, 0, 0, 0, 0, 0, DEFAULT_QUALITY, 0, "Arial Black Italic");
	
	OnSet_LblCtrl();    // 칼라 Label 생성
	OnSet_BtnCtrl();    // 칼라 버튼 생성
	OnSet_GifCtrl();	// 이미지[GIF] 로딩
	OnSet_ProgCtrl();	// 프로그레스바 생성
	
	// [체크 상태 변경] 초기화 진행 상태 표시용 라디오 버튼
	OnChangeInitState(0);
	
	OnSet_DefaultVal();	// [초기화] 이니셜 상태 및 스텝 정보
	OnReadyINIT();	// 초기화 작업 준비 [조건 체크 및 초기화 타이머 생성]
	
	// 현재 초기화 작업 중이라고 설정
	// - 초기화 작업 중에는 프로그램 종료 못하도록 제약하기 위한 인터락 조건
	st_handler.nFlag_InitComplete = NO;
}

void CScreen_Initial::OnDestroy() 
{
	// 생성한 폰트 삭제
	if (mp_fontInit != NULL)
	{
		delete mp_fontInit;
		mp_fontInit = NULL;
	}
	
	KillTimer(TMR_INIT);
	
	// [초기화] 이니셜 선택 대화상자의 사용자 선택 [YES/NO]
	st_handler.nInitTryREQ = READY;
	
	CFormView::OnDestroy();
	
	// TODO: Add your message handler code here
}

void CScreen_Initial::OnTimer(UINT nIDEvent) 
{
	int nRunCHK;
	int i=0;
	
	CDialog_Message dlgMSG;
	
	if (nIDEvent == TMR_INIT)
	{
		if (st_handler.nMenuLock != TRUE)
		{
			// 메뉴 사용 못하도록 설정
			st_handler.nMenuLock = TRUE;
		}
		
		nRunCHK = OnRun_Initial();  // 시컨스 초기화 작업 진행
		if (nRunCHK == RET_GOOD)
		{
			KillTimer(TMR_INIT);  // INIT 수행 타이머 삭제

			if (st_handler.mn_motor_init_check == CTL_YES)
			{
				if (st_handler.cwnd_list != NULL)
				{
					sprintf(st_msg.c_normal_msg, "[Initialize] check end");
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
				}
			}
			else
			{
				if (st_handler.cwnd_list != NULL)
				{
					// yangkyu [2015.03.24]
					// 변수가 잘못 사용되고 있었음.
					sprintf(st_msg.c_abnormal_msg, "[Initialize] check not end");
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
				}
			}

			if (st_work.nEqpStatus != dSTOP)
			{
				alarm.nReset_AlarmStatus = NO;
				
				Func.OnSet_BuzzerSound(IO_OFF, 0);	// 부저 사운드 OFF
				Func.OnSet_EqpStatusStop();	// [STOP] 설비 상태 변경
			}
			
			if (st_handler.nMenuLock != FALSE)
			{
				st_handler.nMenuLock = FALSE;	// 메뉴 사용 가능하도록 설정
			}

			g_ioMgr.set_out_bit(stIO.o_LampJamClear, IO_OFF);

			st_msg.str_fallacy_msg = _T("Initialization completion");
			dlgMSG.DoModal();
			
			// 초기화 작업 완료되었다고 설정
			// - 초기화 작업 중에는 프로그램 종료 못하도록 제약하기 위한 인터락 조건
			st_handler.nFlag_InitComplete	= YES;
			st_handler.nIsAllSiteInitEnd	= CTL_YES;	// 모든 초기화 작업 완료 여부 설정 [초기화 완료]
			
			::PostMessage(st_handler.hWnd, WM_FORM_CHANGE, 1, 1);  // 메인 화면으로 전환 요청
		}
		else if (nRunCHK == RET_ERROR)
		{
			// 초기화 작업 미완료 상태라고 설정
			// - 초기화 작업 중에는 프로그램 종료 못하도록 제약하기 위한 인터락 조건
			st_handler.nFlag_InitComplete = NO;
			
			KillTimer(TMR_INIT);  // INIT 수행 타이머 삭제
			
			if (st_handler.cwnd_list != NULL)
			{
				sprintf(st_msg.c_abnormal_msg, "[initialize] handler initialize check error");
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
			}
			
			// 김양규 수정 [2014.12.08]
			// 장비의 상태가 Stop상태가 되어 알람 화면이 뜨지 않음
// 			if (st_work.nEqpStatus != dSTOP)
// 			{
// 				Func.OnSet_EqpStatusStop();	// [STOP] 설비 상태 변경
// 			}
			
			if (st_handler.nMenuLock != FALSE)
			{
				st_handler.nMenuLock = FALSE;	// 메뉴 사용 가능하도록 설정
			}
		}
	}
	
	CFormView::OnTimer(nIDEvent);
}

//==================================================================//
// 칼라 라벨 생성
//==================================================================//
void CScreen_Initial::OnSet_LblCtrl() 
{
	m_msg_initial.SetFont(mp_fontInit);
	m_msg_initial.SetWindowText(_T("initializing~!! Wait a moment~"));
	m_msg_initial.SetCenterText();
	m_msg_initial.SetColor(RGB(0,0,0));
	m_msg_initial.SetGradientColor(RGB(0,0,255));
	m_msg_initial.SetTextColor(RGB(255,255,255));
}

//==================================================================//
// 칼라 버튼 생성
//==================================================================//
void CScreen_Initial::OnSet_BtnCtrl() 
{
	m_radio_io_bd.SetIcons(IDI_GRAY_LED_ICON, IDI_RED_LED_ICON);
	m_radio_mot_bd.SetIcons(IDI_GRAY_LED_ICON, IDI_RED_LED_ICON);

	m_radio_XYZrobot.SetIcons(IDI_GRAY_LED_ICON, IDI_RED_LED_ICON);
	m_radio_loading_conveyor.SetIcons(IDI_GRAY_LED_ICON, IDI_RED_LED_ICON);
	m_radio_buffer_align_conveyor.SetIcons(IDI_GRAY_LED_ICON, IDI_RED_LED_ICON);
	m_radio_box_lift.SetIcons(IDI_GRAY_LED_ICON, IDI_RED_LED_ICON);
	m_radio_box_clamp.SetIcons(IDI_GRAY_LED_ICON, IDI_RED_LED_ICON);
}

//==================================================================//
// 이미지[GIF] 로딩
//==================================================================//
void CScreen_Initial::OnSet_GifCtrl() 
{
	if (m_gif_initializing.Load(MAKEINTRESOURCE(IDR_GIF_INITIALIZING), _T("GIF")))
	{
		m_gif_initializing.Draw();
	}
}

//==================================================================//
// 프로그레스바 생성
// : 프로그레스바 2개를 생성하여 [50%]씩 진행사항 나타내게 된다
//==================================================================//
void CScreen_Initial::OnSet_ProgCtrl() 
{
	CSuperProgressCtrl::RegisterClass();
	
	m_ctrlProgress.Create(this, 4, 250, LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_INITIAL_STR_1)), 0);
	m_ctrlProgress.SetFillStyle(SP_FILL_HORZGRAD);
	m_ctrlProgress.SetColours(RGB(255,0,0), RGB(0,0,0));
	m_ctrlProgress.SetRange(0, 50);
	m_ctrlProgress.SetStep(1);
	
	m_ctrlProgress1.Create(this, 4, 400, LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_INITIAL_STR_2)), 0);
	m_ctrlProgress1.SetFillStyle(SP_FILL_HORZGRAD);
	m_ctrlProgress1.SetColours(RGB(0,0,255), RGB(0,0,0));
	m_ctrlProgress1.SetRange(50, 100);
	m_ctrlProgress1.SetStep(1);
}

//==================================================================//
// [체크 상태 변경] 초기화 진행 상태 표시용 라디오 버튼
//==================================================================//
void CScreen_Initial::OnChangeInitState(int nzStatus) 
{
	switch(nzStatus)
	{
		// [초기화] 모든 라디오 버튼 상태
	case 0:
		m_radio_io_bd.SetLedState(FALSE);
		m_radio_mot_bd.SetLedState(FALSE);

		m_radio_XYZrobot.SetLedState(FALSE);
		m_radio_loading_conveyor.SetLedState(FALSE);
		m_radio_buffer_align_conveyor.SetLedState(FALSE);
		m_radio_box_lift.SetLedState(FALSE);
		m_radio_box_clamp.SetLedState(FALSE);
		break;

	case 1:
		m_radio_io_bd.SetLedState(TRUE);
		break;

	case 2:
		m_radio_mot_bd.SetLedState(TRUE);
		break;

	case 3:
		m_radio_loading_conveyor.SetLedState(TRUE);
		break;

	case 4:
		m_radio_box_clamp.SetLedState(TRUE);		
		break;

	case 5:
		m_radio_box_lift.SetLedState(TRUE);
		break;

	case 6:
		m_radio_XYZrobot.SetLedState(TRUE);		
		break;

	case 7:
		m_radio_buffer_align_conveyor.SetLedState(TRUE);
		break;
	default:
		break;
	}
}

//==================================================================//
// [초기화] 이니셜 상태 및 스텝 정보
//==================================================================//
void CScreen_Initial::OnSet_DefaultVal() 
{
	m_nStep_Init	= 0;	// [초기화] 이니셜 작업 진행 스텝 정보
	m_nStep_Process	= 0;	// [초기화] 프로그레스바 위치 정보

	int i=0;
	for(i=0; i<MAXSITE; i++)
	{
		st_handler.mn_init_state[i] = CTL_NO;
	}
}

//==================================================================//
// 초기화 작업 준비 [조건 체크 및 초기화 타이머 생성]
//==================================================================//
void CScreen_Initial::OnReadyINIT() 
{
	// 초기화 작업 중 에러 발생 상태 설정 [미발생함]
	st_handler.nStopPushInInitial = FALSE;

	//==============================================================//
	// [LOCK] 상태에서 강제로 종료된 조건인지 확인
	// - [LOCK] 화면 전환 후 초기화 작업 취소됨
	//==============================================================//
	char chData[25];
	CString sLockCHK;
	::GetPrivateProfileString("Password", "SysLockStatus", "NO", chData, 20, st_path.mstr_basic);
	sLockCHK = chData;

	// [LOCK] 상태에서 강제 종료된 조건
	if (sLockCHK == "YES")
	{
		if (st_handler.cwnd_list != NULL)
		{
			sprintf(st_msg.c_normal_msg, "System Locking.");
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
		}

		// LOCK 화면으로 전환시킴
		// - 메인 프레임에 SYSTEM LOCK 화면 출력 요청
		::PostMessage(st_handler.hWnd, WM_FORM_CHANGE, FORM_NAME, 2);
		return;
	}
	//--------------------------------------------------------------//

	// 기존에 이곳에 추가되어 있던 INIT 상태로의 변경 작업은 OnInitReady() 함수로 이동시킴

	CDialog_Select dlgDECIDE;
	int nResponse;
	//==============================================================//
	// 모터 초기화 작업이 필요한지 검사
	// - OnInitialUpdate() 함수에서 [READY]로 설정
	//==============================================================//
	if (st_handler.nInitTryREQ == READY)
	{
		st_msg.mstr_confirm_msg = "Handler Motor Init?";
		nResponse = dlgDECIDE.DoModal();
		if (nResponse == IDOK)
		{
			st_handler.mn_motor_init_check = CTL_YES;

			// 실재로 초기화 작업 선택 여부는 복구 모드 선택 후에 설정해야 하므로 위치 이동시킴
			//st_handler.nInitTryREQ = YES;	// 초기화 작업 진행

			OnInitReady();  // [준비] 이니셜 작업
		}
		else
		{
			st_handler.mn_motor_init_check = CTL_NO;

			// [NO] 선택 시 이전 상태를 유지해야 한다면 아래 부분 모두 주석 처리하면 됨
			/*
			st_handler.nInitTryREQ = NO;	// 초기화 작업 SKIP

			// [취소] 선택한 경우에는 메인 화면으로 전환시킴
			::PostMessage(st_handler.hWnd, WM_FORM_CHANGE, 1, 1);  // 메인 화면으로 전환 요청
			return;
			*/

			// SKIP 버튼 동작과 동일하게 진행함
			// - 구동하는 모터가 없고 타이머 구동전이라 모터 STOP 및 타이머 제거 동작 필요없지만
			//   위 동작을 진행한다고 문제가 되는 것은 아니기에 이렇게 처리함
			OnBtnInitSkip();
			return;
		}
	}
	//--------------------------------------------------------------//

	/////////////////////////////////////////////////////////////////////////////////
	// 복구 모드 확인 작업
	/////////////////////////////////////////////////////////////////////////////////

	// 복구 동작이 필요한 상태인지에 대한 정보 로딩
	::GetPrivateProfileString("SaveMode", "RecoveryMode", "0", chData, 20, st_path.mstr_basic);
	st_basic.nFlag_EqpRecovery[0] = atoi(chData);

	// [0]=>복구모드 저장 모드(0:저장되지 않은 상태, 1:복구모드가 저장 상태(이때만 초기화시 복구여부를 물어볼 수 있음)
	// [1]=>초기화시 복구모드 선택 유/무 확인 (0:복구모드 선택안함, 1:복구모드 선택)

	int iReadCancel;
	int i=0;
	int iFuncRet;
	// 복구할 상태 정보가 존재함
	// - 사용자의 선택에 따라 복구진행/복구취소 작업 진행하게 됨
	if (st_basic.nFlag_EqpRecovery[0] == YES)
	{
		st_msg.mstr_confirm_msg = "Handler Recovery Mode(YES), Are You sure ?";
		nResponse = dlgDECIDE.DoModal();
		if (nResponse == IDOK)
		{
			// [초기화] 복구 정보 읽기 작업 SKIP 여부
			iReadCancel = NO;

			for(i=0; i<HANDLER_BACKUP_DATA_RW; i++)
			{
				iFuncRet = Func.Handler_Recovery_Data(0, i, DATA_READ);
				if (iFuncRet == RET_ERROR)
				{
					iReadCancel = YES;
				}
			}

			if (iReadCancel == YES)
			{
				// 사용자의 복구 동작 선택을 해제함
				// - 복구 동작 정보 로딩에 문제가 있으므로 복구 동작 선택을 강제로 해제함
				st_basic.nFlag_EqpRecovery[1] = NO;

				if (st_handler.cwnd_list != NULL)
				{
					sprintf(st_msg.c_abnormal_msg, "[skip] recovery data save-init[read]");
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
				}
			}
			else
			{
				// st_basic.nFlag_EqpRecovery[1] = YES; 설정은 Handler_Recovery_Data() 함수에서 진행함
				// - 정상적으로 복구 정보가 로딩된 경우에는 해당 플래그를 이곳에서 설정할 필요 없음

				if (st_handler.cwnd_list != NULL)
				{
					sprintf(st_msg.c_normal_msg, "[ok]recovery data save-init[read]");
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
				}
			}
		}
		else if (nResponse == IDCANCEL)
		{
			// 사용자의 복구 동작 선택을 해제함
			// - 복구 동작 정보 로딩에 문제가 있으므로 복구 동작 선택을 강제로 해제함
			st_basic.nFlag_EqpRecovery[1] = NO;

			if (st_handler.cwnd_list != NULL)
			{
				sprintf(st_msg.c_normal_msg, "[Init_Menu_SKIP]recovery data save-init[read]");
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
			}
		}
	}
	/////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////
	st_handler.nInitTryREQ = YES;		// 초기화 작업 진행
	st_handler.nStart_INIT = CTL_YES;	// 초기화 시작되었음

	SetTimer(TMR_INIT, 250, NULL);  // 초기화 작업 타이머 생성
}

//==================================================================//
// 시컨스 초기화 작업 진행
// - 각종 데이터 로딩은 메인 프레임의 OnCreate() 함수에서 이미 호출됨
//==================================================================//
int CScreen_Initial::OnRun_Initial() 
{
	m_nFlag_Init = RET_PROCEED;

	if (st_work.nEqpStatus != dINIT)
	{
		return m_nFlag_Init;
	}

	int nRet;
	int i, j=0;
	int iRet1 = -1, iRet2 = -1;

	switch(m_nStep_Init)
	{
	case 0:
		// 각종 설비 데이터 로딩 작업은 OnMain_Var_Default_Set() 함수에서 했으므로 SKIP
		// - 중간에 데이터 변경 후 초기화 작업을 하는 경우에도 해당 구조체 값은 변경 되었을 것이기에
		//   굳이 설비 데이터를 다시 로딩할 필요는 없음
		m_nStep_Process	= 9;
		m_nStep_Init	= 5;
		break;

	case 5:
		// st_handler.n_io_board_initial : I/O 보드 초기화 여부
		// - IO_Board_Initialize() 함수 안에서 TRUE 설정함
		if (FAS_IO.m_nflag_IOBrdInit == BD_ERROR)
		{
			// 900003 0 16 "IO_BOARD_INITIALIZE_ERR."
			alarm.mstr_code		= "900003";
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;

			// 스텝 정보 초기화
			m_nStep_Process	= 0;
			m_nStep_Init	= 0;

			m_nFlag_Init = RET_ERROR;  // 에러 반환
			return m_nFlag_Init;
		}
		else
		{
			if (st_handler.cwnd_list != NULL)
			{
				sprintf(st_msg.c_normal_msg, "I/O Board Initialized...");
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
			}
			// 출력 포트 초기화 [부저 및 버튼 램프 상태]
			Func.OnClear_OutPort();	// [초기화] 출력 포트 상태

			m_nStep_Process += 8;
			// [체크 상태 변경] 초기화 진행 상태 표시용 라디오 버튼
			OnChangeInitState(1);

			m_nStep_Init = 10;
		}
		break;

	case 10:
		// 모션 보드 인식 여부 검사
		nRet = st_handler.mn_mot_brd_initial;
		if (nRet == TRUE)
		{
			Func.OnSet_MotorUnitDist();	// 모터 환경 파라메터 설정
			// st_teach[i].d_spd_acc 등의 구조체 값임
			Func.OnSet_MotorSpeed();	// 모터 구동 속도 설정

			if (st_handler.cwnd_list != NULL)
			{
				sprintf(st_msg.c_normal_msg, "Motor Board Open Success!");
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
			}

			m_nStep_Process += 8;
			// [체크 상태 변경] 초기화 진행 상태 표시용 라디오 버튼
			OnChangeInitState(2);

			m_nStep_Init = 15;
			break;
		}

		//==========================================================//
		// 모션 보드가 메인 프레임에서 초기화되지 못한 경우 대응책
		//==========================================================//
		{
			// 900002 0 16 "MOTION_BOARD_INITIALIZE_ERR."
			alarm.mstr_code		= "900002";
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;

			// 스텝 정보 초기화
			m_nStep_Process	= 0;
			m_nStep_Init	= 0;
			
			m_nFlag_Init = RET_ERROR;  // 에러 반환
			return m_nFlag_Init;
		}
		//----------------------------------------------------------//
		break;

	case 15:
		// [리셋] 초기화 작업 중 STOP 버튼 눌렸는지 여부
		st_handler.nStopPushInInitial = FALSE;

		// 초기화 작업 시작
		// - 모터 초기화 진행하도록 선택된 경우에는 모터 초기화 작업 진행
		// - 대화상자를 통하여 모터 초기화 작업 여부 선택했음
		if (st_handler.nInitTryREQ == YES)
		{
			m_nStep_Init = 30;
		}
		// [취소] 초기화 작업
		// - 현재는 이 부분 실행되지 않음
		// - [취소] 시에 타이머 생성 없이 메인 화면으로 전환하기 때문임
		else if (st_handler.nInitTryREQ == NO)
		{
			m_nStep_Init = 100;
		}
		break;

	case 30:
		// [초기화 완료, 화면 표시] 모터들에 대해서는 [CTL_YES->CTL_READY] 상태로 변경됨
		// 고로, m_nStep_Process 값은 모터 초기화 작업 완료 시 한번씩만 계산되게 됨
		for(i=0; i<MAXSITE; i++)
		{
			if (st_handler.mn_init_state[i] == CTL_YES)
			{
				m_nStep_Process += 15;
				OnChangeInitState(i +3);

				st_handler.mn_init_state[i] = CTL_READY;
			}
		}

		for(i=0; i<MAXSITE; i++)
		{
			// 몇개의 사이트가 초기화 완료되었는지 계산
			if (st_handler.mn_init_state[i] == CTL_READY)
			{
				j++;
			}
		}

		// 모든 사이트 초기화 작업 완료되었는지 확인
		if (j == MAXSITE)
		{
			for(i=0; i<MAXSITE; i++)
			{
				st_handler.mn_init_state[i] = CTL_YES;
			}

			m_nStep_Init = 100;
		}
		break;

	case 100:
		m_nStep_Process	= 100;
		m_nStep_Init	= 0;

		m_nFlag_Init = RET_GOOD;
		break;
	}

	// 진행 상태 표시할 프로그레스바 설정
	// : 흠, 50%일때에는 2개 프로그레스바 모두가 진행되겠군
	if (m_nStep_Process < 51)
	{
		m_ctrlProgress.SetPos(m_nStep_Process);
	}
	else if (m_nStep_Process > 49)
	{
		if (m_nStep_Process > 100)
		{
			m_nStep_Process = 100;
		}
		m_ctrlProgress.SetPos(50);  // 안해줘도 될 듯 한데..흠
		m_ctrlProgress1.SetPos(m_nStep_Process);
	}
	
	// 초기화 작업 중 STOP 버튼 눌린 경우
	if (st_handler.nStopPushInInitial == TRUE)
	{
		m_nStep_Process = 0;
		m_ctrlProgress.SetPos(0);
		m_ctrlProgress1.SetPos(50);

		m_nStep_Init = 0;
		m_nFlag_Init = RET_ERROR;	// 에러 반환
	}
	return m_nFlag_Init;
}

void CScreen_Initial::OnBtnInitSkip() 
{
	// [리셋] 초기화 진행할지에 대한 사용자 선택 정보
	st_handler.nInitTryREQ = READY;

	// 안전을 위해 모든 모터의 동작을 강제로 정지시킴
	if (st_handler.mn_mot_brd_initial == TRUE)
	{
		Func.OnStop_AxisAll();  // 전체 모터축 긴급 정지
	}
	
	KillTimer(TMR_INIT);  // INIT 작업 진행 타이머 삭제
	
	if (st_handler.cwnd_list != NULL)
	{
		sprintf(st_msg.c_normal_msg, "[initialization] Accy Supply initialization Skip.");
		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
	}
	
	// 설비 상태를 [STOP] 상태로 변경
	if (st_work.nEqpStatus != dSTOP)
	{
		// 김동수 수석이 추가해 놓아서 추가함
		alarm.nReset_AlarmStatus = CTL_NO;
		g_ioMgr.set_out_bit(stIO.o_LampJamClear, IO_OFF);

		Func.OnSet_EqpStatusStop();	// [STOP] 설비 상태 변경
	}
	
	if (st_handler.nMenuLock != FALSE)
	{
		st_handler.nMenuLock = FALSE;  // 메뉴 사용 가능
	}
	
	// 다시 초기화 작업 진행해야 하는데 그렇게 하지 않고 곧바로 [START] 시키는 경우 발생할 수 있어 추가함
	// - 모든 조건을 초기화시켜 반드시 [초기화 작업] 필요하도록 만듬
	st_handler.nIsAllSiteInitEnd = CTL_NO;	// 모든 초기화 작업 완료 여부 설정 [초기화 미완료]
	
	// 나중에 함수 만들면 그 때 추가하라
	/*
	Func.OnInit_InitializeFlag();	// [초기화] 각종 변수
	// 초기화 상태 정보 저장 변수도 OnInit_SyncFlag() 함수에 포함되어 있음
	Func.OnInit_SyncFlag();		// [초기화] SYNC 플래그
	Func.OnInit_LotEndFlag();	// [초기화] LOT END 플래그
	
	Func.OnThreadClassVarINIT();  // 쓰레드 변수 초기화 요청
	*/

	st_handler.nFlag_InitComplete = TRUE;
	::PostMessage(st_handler.hWnd, WM_FORM_CHANGE, 1, 1);  // 메인 화면으로 전환 요청
}

void CScreen_Initial::OnBtnInitRetry() 
{
	
}

//==================================================================//
// [준비] 이니셜 작업
// - 각종 변수 및 상태 정보 초기화
//==================================================================//
void CScreen_Initial::OnInitReady() 
{
	// 모든 초기화 작업 완료 여부 설정 [초기화 미완료]
	st_handler.nIsAllSiteInitEnd = CTL_NO;
	
	// 추후에 함수 만들어지면 그때 추가하라
	/*
	Func.OnInit_InitializeFlag();	// [초기화] 각종 변수
	// 초기화 상태 정보 저장 변수도 OnInit_SyncFlag() 함수에 포함되어 있음
	Func.OnInit_SyncFlag();		// [초기화] SYNC 플래그
	Func.OnInit_LotEndFlag();	// [초기화] LOT END 플래그
	
	Func.OnThreadClassVarINIT();	// 쓰레드 변수 초기화 요청
	*/
	
	if (st_work.nEqpStatus != dINIT)
	{
		// 설비 상태 [INIT]으로 변경 후 도어 CLOSE 작업 진행
		Func.OnSet_EqpStatusInit();	// [INIT] 설비 상태 정보 변경
	}
}

