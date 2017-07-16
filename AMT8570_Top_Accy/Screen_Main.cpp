// Screen_Main.cpp : implementation file
//

#include "stdafx.h"
#include "handler.h"
#include "Screen_Main.h"
#include "Dialog_Select.h"
#include "Dialog_Message.h"
#include "Dialog_SetReset.h"

// 규이리 추가 [2014.12.27]
#include "Dialog_Infor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScreen_Main

IMPLEMENT_DYNCREATE(CScreen_Main, CFormView)

CScreen_Main::CScreen_Main()
	: CFormView(CScreen_Main::IDD)
{
	//{{AFX_DATA_INIT(CScreen_Main)
	//}}AFX_DATA_INIT
}

CScreen_Main::~CScreen_Main()
{
}

void CScreen_Main::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CScreen_Main)
	DDX_Control(pDX, IDC_LBL_MAT_CODE, m_lblMatCode);
	DDX_Control(pDX, IDC_GROUP_SUPPLY_INFO, m_group_supply_info);
	DDX_Control(pDX, IDC_GROUP_SMEMA_INFO, m_group_smema_info);
	DDX_Control(pDX, IDC_GROUP_GRIPPER_INFO, m_group_gripper_info);
	DDX_Control(pDX, IDC_GROUP_FORCE_DISACHARGE_STATE, m_group_forcedischarge_state);
	DDX_Control(pDX, IDC_CHK_FORCEDISCHARGE, m_chk_forcedischarge);
	DDX_Control(pDX, IDC_MSG_FORCEDISCHARGE_STATE, m_msg_forcedischarge);
	DDX_Control(pDX, IDC_GROUP_TIME_INFO, m_group_time_info);
	DDX_Control(pDX, IDC_LABEL_TIME_RUN, m_label_time_run);
	DDX_Control(pDX, IDC_LABEL_TIME_STOP, m_label_time_stop);
	DDX_Control(pDX, IDC_LABEL_TIME_ALARM, m_label_time_alarm);
	DDX_Control(pDX, IDC_LABEL_TIME_MTBI, m_label_time_mtbi);
	DDX_Control(pDX, IDC_DGT_TIME_RUN, m_dgt_time_run);
	DDX_Control(pDX, IDC_DGT_TIME_STOP, m_dgt_time_stop);
	DDX_Control(pDX, IDC_DGT_TIME_ALARM, m_dgt_time_alarm);
	DDX_Control(pDX, IDC_DGT_TIME_MTBI, m_dgt_time_mtbi);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CScreen_Main, CFormView)
	//{{AFX_MSG_MAP(CScreen_Main)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_CHK_FORCEDISCHARGE, OnChkForceDischarge)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_NETWORK, OnButtonNetwork)
	ON_BN_CLICKED(IDC_BUTTON_SMEMA, OnButtonSmema)
	ON_BN_CLICKED(IDC_BMP_ACCY_BOX_1, OnBmpAccyBoxView)
	ON_BN_CLICKED(IDC_BUTTON_REALIGN, OnButtonRealign)
	ON_BN_CLICKED(IDC_BMP_NG_BUFFER_ACCY_1, OnBmpNgBufferAccy1)
	ON_BN_CLICKED(IDC_BMP_NG_BUFFER_ACCY_2, OnBmpNgBufferAccy2)
	ON_BN_CLICKED(IDC_BMP_NG_BUFFER_ACCY_3, OnBmpNgBufferAccy3)
	ON_BN_CLICKED(IDC_BMP_NG_BUFFER_ACCY_4, OnBmpNgBufferAccy4)	
	ON_BN_CLICKED(IDC_BMP_BUFFER_CONV_ACCY_1, OnBmpBufferConvAccyFront)
	ON_BN_CLICKED(IDC_BMP_BUFFER_CONV_ACCY_3, OnBmpBufferConvAccyRear)
	ON_BN_CLICKED(IDC_BMP_ACCY_BOX_2, OnBmpAccyBoxView)
	ON_BN_CLICKED(IDC_BMP_ACCY_BOX_3, OnBmpAccyBoxView)
	ON_BN_CLICKED(IDC_BMP_BUFFER_CONV_ACCY_2, OnBmpBufferConvAccyFront)
	ON_BN_CLICKED(IDC_BMP_BUFFER_CONV_ACCY_4, OnBmpBufferConvAccyRear)
	ON_BN_CLICKED(IDC_BTN_BCR_READ_TEST, OnBtnBcrReadTest)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_UPDATE_MAIN, OnMainUpdateReq)  // 메인화면의 데이터 갱신 요청
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScreen_Main diagnostics

#ifdef _DEBUG
void CScreen_Main::AssertValid() const
{
	CFormView::AssertValid();
}

void CScreen_Main::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CScreen_Main message handlers

void CScreen_Main::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class

	// 컨트롤 폰트 생성
	mp_main_font = NULL;
	mp_main_font = new CFont;
	mp_main_font->CreateFont(18,0,0,0,0,0,0,0,0,0,0,DEFAULT_QUALITY,0,"Arial");

	// 화면에 보여주기 위해서 추가함
	st_work.nForceDischarge = FORCEDISCHARGE_OFF;

	OnMain_Display_Main();
	OnMain_Display_AccyBox();
	OnMain_Display_EmptyAccyBox();
	OnMain_Display_ClampAccyBox_Accy();
	OnMain_Display_NGOut_Accy();
	OnMain_Display_NGBuffer();
	OnMain_Display_BufferConv();

	OnMain_Button_Set();		// Button setting
	OnMain_GroupBox_Set();		// GroupBox Setting
	OnMain_Digital_Count_Set(); // Digital  Setting
	OnMain_Label_Set();			// Text Msg Setting
	OnMain_Picture_Set();		// Picture Setting

	OnMain_TimeInfo_Display();

	OnDoorInfoInit();
	OnSupplyInfoInit();
	OnSMEMAInfoInit();

	OnDisplaySMEMAInfo();
	OnDisplaySupplyInfo();
	OnDisplayDoorInfo();

	OnGripperInfoInit();

	GetDlgItem(IDC_BUTTON_REALIGN)->SetWindowText("Box Unclamp");
	GetDlgItem(IDC_BUTTON_REALIGN)->ShowWindow(TRUE);

	st_handler.cwnd_main = this;		// MAIN 화면 핸들 설?

	// 각 State 표시 타이머
	SetTimer(TMR_STATE, 100, NULL);

	// ACCY_DISPLAY 표시 타이머
	SetTimer(ACCY_DISPLAY, 500, NULL);
}

void CScreen_Main::OnDestroy() 
{
	// **************************************************************************
	// 생성한 폰트 정보 삭제한다
	// **************************************************************************
	delete mp_main_font;
	mp_main_font = NULL;
	st_handler.cwnd_main = NULL;
	// **************************************************************************

	CFormView::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

//==================================================================//
// 타이머
//==================================================================//
void CScreen_Main::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == TMR_STATE)
	{
		OnDisp_AcceMatCode();
		//ForceDischarge
		OnBuildForceDischargeState();
		
		OnMain_Time_Display(st_work.nEqpStatus);

		OnDisplayDoorInfo();
		// 규이리 추가 [2015.03.20]
		OnDisp_AcceMatCode();	// 악세사리 매칭 코드 화면 출력

		if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, DEVICE_INFO);  // 디바이스명 출력 요청
	}
	else if (nIDEvent == ACCY_DISPLAY)
	{	
		OnMain_Display_AccyBox();
		OnMain_Display_NGBuffer();
		OnMain_Display_EmptyAccyBox();
		OnDisplaySupplyInfo();
		OnDisplaySMEMAInfo();
	}
	
	
	CFormView::OnTimer(nIDEvent);
}

void CScreen_Main::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CDC *pDC;

	pDC = GetDlgItem(IDC_BMP_VIEW)->GetDC();
	m_p_bmp_view.Show(pDC, CPoint(0, 0), CPoint(1040, 193), 0,0);
	ReleaseDC(pDC);
	//////////////////////////////////////////////////////////////////////////

	pDC = GetDlgItem(IDC_BMP_ACCY_BOX_1)->GetDC();
	m_p_bmp_AccyBox[0].Show(pDC, CPoint(0, 0), CPoint(168, 106), 0,0);
	ReleaseDC(pDC);

	pDC = GetDlgItem(IDC_BMP_ACCY_BOX_2)->GetDC();
	m_p_bmp_AccyBox[1].Show(pDC, CPoint(0, 0), CPoint(168, 106), 0,0);
	ReleaseDC(pDC);

	pDC = GetDlgItem(IDC_BMP_ACCY_BOX_3)->GetDC();
	m_p_bmp_AccyBox[2].Show(pDC, CPoint(0, 0), CPoint(168, 106), 0,0);
	ReleaseDC(pDC);

	//////////////////////////////////////////////////////////////////////////

	pDC = GetDlgItem(IDC_BMP_EMTYP_ACCY_BOX_1)->GetDC();
	m_p_bmp_EmptyAccyBox[0].Show(pDC, CPoint(0, 0), CPoint(168, 106), 0,0);
	ReleaseDC(pDC);

	pDC = GetDlgItem(IDC_BMP_EMTYP_ACCY_BOX_2)->GetDC();
	m_p_bmp_EmptyAccyBox[1].Show(pDC, CPoint(0, 0), CPoint(168, 106), 0,0);
	ReleaseDC(pDC);

	pDC = GetDlgItem(IDC_BMP_EMTYP_ACCY_BOX_3)->GetDC();
	m_p_bmp_EmptyAccyBox[2].Show(pDC, CPoint(0, 0), CPoint(168, 106), 0,0);
	ReleaseDC(pDC);
	//////////////////////////////////////////////////////////////////////////

	pDC = GetDlgItem(IDC_BMP_CLAMP_ACCY_BOX_ACCY)->GetDC();
	m_p_bmp_ClampAccyBox.Show(pDC, CPoint(0, 0), CPoint(168, 106), 0,0);
	ReleaseDC(pDC);

	//////////////////////////////////////////////////////////////////////////

	pDC = GetDlgItem(IDC_BMP_NG_ACCY_OUT)->GetDC();
	m_p_bmp_NGOutAccy.Show(pDC, CPoint(0, 0), CPoint(36, 41), 0,0);
	ReleaseDC(pDC);
	//////////////////////////////////////////////////////////////////////////

	pDC = GetDlgItem(IDC_BMP_NG_BUFFER_ACCY_1)->GetDC();
	m_p_bmp_NGBuffer[0].Show(pDC, CPoint(0, 0), CPoint(36, 41), 0,0);
	ReleaseDC(pDC);

	pDC = GetDlgItem(IDC_BMP_NG_BUFFER_ACCY_2)->GetDC();
	m_p_bmp_NGBuffer[1].Show(pDC, CPoint(0, 0), CPoint(36, 41), 0,0);
	ReleaseDC(pDC);

	pDC = GetDlgItem(IDC_BMP_NG_BUFFER_ACCY_3)->GetDC();
	m_p_bmp_NGBuffer[2].Show(pDC, CPoint(0, 0), CPoint(36, 41), 0,0);
	ReleaseDC(pDC);

	pDC = GetDlgItem(IDC_BMP_NG_BUFFER_ACCY_4)->GetDC();
	m_p_bmp_NGBuffer[3].Show(pDC, CPoint(0, 0), CPoint(36, 41), 0,0);
	ReleaseDC(pDC);
	//////////////////////////////////////////////////////////////////////////
	
	pDC = GetDlgItem(IDC_BMP_BUFFER_CONV_ACCY_1)->GetDC();
	m_p_bmp_BufferConv[0].Show(pDC, CPoint(0, 0), CPoint(36, 41), 0,0);
	ReleaseDC(pDC);

	pDC = GetDlgItem(IDC_BMP_BUFFER_CONV_ACCY_2)->GetDC();
	m_p_bmp_BufferConv[1].Show(pDC, CPoint(0, 0), CPoint(36, 41), 0,0);
	ReleaseDC(pDC);
	
	pDC = GetDlgItem(IDC_BMP_BUFFER_CONV_ACCY_3)->GetDC();
	m_p_bmp_BufferConv[2].Show(pDC, CPoint(0, 0), CPoint(36, 41), 0,0);
	ReleaseDC(pDC);
	
	pDC = GetDlgItem(IDC_BMP_BUFFER_CONV_ACCY_4)->GetDC();
	m_p_bmp_BufferConv[3].Show(pDC, CPoint(0, 0), CPoint(36, 41), 0,0);
	ReleaseDC(pDC);
}

// 메인화면의 데이터 갱신 요청
LRESULT CScreen_Main::OnMainUpdateReq(WPARAM wParam, LPARAM lParam)
{
	int nCommand = (int)wParam;
	int nRequire = (int)lParam;

	switch(nCommand)
	{
	case PLACE_LOADING_CONV_:
		break;

	case PLACE_BOX_CLAMP_:
		OnMain_Display_ClampAccyBox_Accy();
		break;

	case PLACE_BOX_LIFTER_:
		break;

	case PLACE_XYZ_RBT_:
		OnMain_Display_ClampAccyBox_Accy();
		break;

	case PLACE_NG_OUT_:
		OnMain_Display_NGOut_Accy();
		break;

	case PLACE_NG_BUFFER_:
		break;

	case PLACE_BUFFER_CONV_:
		OnMain_Display_BufferConv();
		break;

	case READ_BCR_DATA:
		OnDisplayGripperInfo();
		break;
	}
	return 0;
}

void CScreen_Main::OnMain_Display_Main()
{
	CString sPath;
	
	sPath = IMG_PATH_ + "MainUI.bmp";
	m_p_bmp_view.Load(sPath);
	
	Invalidate(FALSE);
}

void CScreen_Main::OnMain_Display_AccyBox()
{
	CString sPath;
	
	int nCount=0, i=0;

	int nReadIO[3] = {0,};
	// 센서가 B접이라 센서 상태 확인 시 반전 사용
	nReadIO[0] = !g_ioMgr.get_in_bit(stIO.i_Chk_AccyBoxConvDetection[SIDE_THIRD_]);
	nReadIO[1] = !g_ioMgr.get_in_bit(stIO.i_Chk_AccyBoxConvDetection[SIDE_SECOND_]);
	nReadIO[2] = !g_ioMgr.get_in_bit(stIO.i_Chk_AccyBoxConvDetection[SIDE_FIRST_]);

	// [WithOut] 모드인지 확인
	// 변수를 이용하여 표시
	if (st_basic.n_mode_device == WITHOUT_DVC_)
	{
		nReadIO[0] = st_map.nLoadingAccyBoxExist[0];
		nReadIO[1] = st_map.nLoadingAccyBoxExist[1];
		nReadIO[2] = st_map.nLoadingAccyBoxExist[2];
	}
	for(i=0; i<3; i++)
	{
		if (m_nCurrent_AccyBoxExist[i] != nReadIO[i])
		{
			m_nCurrent_AccyBoxExist[i] = nReadIO[i];
			nCount++;
		}
	}
	
	// 값이 바뀐 영영이 하나라도 존재하면..
	if (nCount != 0)
	{
		for(i=0; i<3; i++)
		{
			if (m_nCurrent_AccyBoxExist[i] == IO_ON)
			{
				sPath = IMG_PATH_ + "AccyBox.bmp";
				m_p_bmp_AccyBox[i].Load(sPath);
			}
			else
			{
				m_p_bmp_AccyBox[i].FreePictureData();
			}
		}
		Invalidate(FALSE);
	}
	
}

void CScreen_Main::OnMain_Display_EmptyAccyBox()
{
	CString sPath;
	
	int nCount=0, i=0;
	
	int nReadIO[2] = {0,};
	// 센서가 B접이라 센서 상태 확인 시 반전 사용
	nReadIO[0] = !g_ioMgr.get_in_bit(stIO.i_Chk_UnloaderFull[0]);
	nReadIO[1] = !g_ioMgr.get_in_bit(stIO.i_Chk_UnloaderFull[1]);
	
	for(i=0; i<2; i++)
	{
		if (m_nCurrent_EmptyAccyBoxExist[i] != nReadIO[i])
		{
			m_nCurrent_EmptyAccyBoxExist[i] = nReadIO[i];
			nCount++;
		}
	}
	// kilee [2015.02.28]
	/*
	m_nCurrent_EmptyAccyBoxExist[0] = IO_ON;
	m_nCurrent_EmptyAccyBoxExist[1] = IO_ON;
	*/
	// 값이 바뀐 영영이 하나라도 존재하면..
	if (nCount != 0)
	{
		for(i=0; i<2; i++)
		{
			if (m_nCurrent_EmptyAccyBoxExist[i] == IO_ON)
			{
				sPath = IMG_PATH_ + "EmptyAccyBox.bmp";
				m_p_bmp_EmptyAccyBox[i].Load(sPath);
			}
			else
			{
				m_p_bmp_EmptyAccyBox[i].FreePictureData();
			}
		}
		Invalidate(FALSE);
	}
}

void CScreen_Main::OnMain_Display_ClampAccyBox_Accy()
{
	CString sPath;

	int nCount=0, i=0;

	if (m_nCurrent_ClampAccyBoxAccyExist != st_map.nClampAccyBoxAccy)
	{
		m_nCurrent_ClampAccyBoxAccyExist = st_map.nClampAccyBoxAccy;

		if (m_nCurrent_ClampAccyBoxAccyExist == IO_ON)
		{
			sPath = IMG_PATH_ + "AccyBox.bmp";
			m_p_bmp_ClampAccyBox.Load(sPath);
		}
		else
		{
			m_p_bmp_ClampAccyBox.FreePictureData();
		}
		Invalidate(FALSE);
	}
}

void CScreen_Main::OnMain_Display_NGOut_Accy()
{
	CString sPath;
	if(m_nCurrent_NGOutAccy == st_map.nNGOut)
	{
		return;
	}

	m_nCurrent_NGOutAccy = st_map.nNGOut;
	
	if (m_nCurrent_NGOutAccy == 0)
	{
		m_p_bmp_NGOutAccy.FreePictureData();
	}
	else
	{
		sPath = IMG_PATH_ + "AccyBad.bmp";
		m_p_bmp_NGOutAccy.Load(sPath);	
	}
	Invalidate(FALSE);
}

void CScreen_Main::OnMain_Display_NGBuffer()
{
	CString sPath;

	int nCount=0, i=0;
	
	for(i=0; i<4; i++)
	{
		if (m_nCurrent_NGBufferExist[i] != st_map.nNGBuffer[i])
		{
			m_nCurrent_NGBufferExist[i] = st_map.nNGBuffer[i];
			nCount++;
		}
	}
	
	// 값이 바뀐 영영이 하나라도 존재하면..
	if (nCount != 0)
	{
		for(i=0; i<4; i++)
		{
			if (m_nCurrent_NGBufferExist[i] == IO_ON)
			{
				sPath = IMG_PATH_ + "AccyB.bmp";
				m_p_bmp_NGBuffer[i].Load(sPath);
			}
			else
			{
				sPath = IMG_PATH_ + "AccyE.bmp";
				m_p_bmp_NGBuffer[i].Load(sPath);
			}
		}
		Invalidate(FALSE);
	}
}

void CScreen_Main::OnMain_Display_BufferConv()
{
	CString sPath;
	int nCount=0, i=0;

	for(i=0; i<4; i++)
	{
		if (m_nCurrent_BufferCovAccyExist[i] != st_map.nBufferCovAccyExist[i])
		{
			m_nCurrent_BufferCovAccyExist[i] = st_map.nBufferCovAccyExist[i];
			nCount++;
		}
	}
	
	// 값이 바뀐 영영이 하나라도 존재하면..
	if (nCount != 0)
	{
		for(i=0; i<4; i++)
		{
			if (m_nCurrent_BufferCovAccyExist[i] == IO_ON)
			{
				if (st_basic.nMode_SupplyDevice < 10)
				{
					sPath = IMG_PATH_ + "AccyB.bmp";
				}
				else 
				{
					sPath = IMG_PATH_ + "AccyT.bmp";
				}
				m_p_bmp_BufferConv[i].Load(sPath);
			}
			else
			{
				sPath = IMG_PATH_ + "AccyE.bmp";
				m_p_bmp_BufferConv[i].Load(sPath);
			}
		}
		Invalidate(FALSE);
	}
}

//==================================================================//
// Door Info init
//==================================================================//
void CScreen_Main::OnDoorInfoInit()
{
	TSpread *Grid;
	CString sTitle[2] = { "TOP Door", "BTM Door"};
	int i, j;
	int nControlID;
	
	for (i = 0; i < MAX_DOOR_ / 2; i++)
	{
		switch(i)
		{
		case 0:
			nControlID = IDC_CUSTOM_DOOR_TOP1_BTM9;
			break;
		case 1:
			nControlID = IDC_CUSTOM_DOOR_TOP2_BTM10;
			break;
		case 2:
			nControlID = IDC_CUSTOM_DOOR_TOP3_BTM11;
			break;
		case 3:
			nControlID = IDC_CUSTOM_DOOR_TOP4_BTM12;
			break;
		case 4:
			nControlID = IDC_CUSTOM_DOOR_TOP5_BTM13;
			break;
		case 5:
			nControlID = IDC_CUSTOM_DOOR_TOP6_BTM14;
			break;
		case 6:
			nControlID = IDC_CUSTOM_DOOR_TOP7_BTM15;
			break;
		case 7:
			nControlID = IDC_CUSTOM_DOOR_TOP8_BTM16;
			break;
		}
		Grid = (TSpread*)GetDlgItem(nControlID);
		
		// 그리드 헤더 미사용
		Grid->SetBool(SSB_SHOWCOLHEADERS, FALSE); // col header 삭제
		Grid->SetBool(SSB_SHOWROWHEADERS, FALSE); // row header 삭제
		// 스크롤 미생성
		Grid->SetBool(SSB_HORZSCROLLBAR, FALSE); // 좌우 스크롤바
		Grid->SetBool(SSB_VERTSCROLLBAR, FALSE); // 상하 스크롤바
		// 입력 글자수에 맞게 자동 맞춤 그리드 크기 변경 기능 미사용
		Grid->SetBool(SSB_AUTOSIZE, FALSE);
		// 그리드 갯수 설정
		Grid->SetMaxRows(2);
		Grid->SetMaxCols(1);

		for(j=0; j<2; j++)
		{
			Grid->SetRowHeightInPixels(j+1, 17);
			
			Grid->SetColWidthInPixels(1, 112);
			GridColor(nControlID, j + 1, 1, BLUE_D, WHITE_C);
			GridControl(nControlID, STATIC, j+1, 1, 3);

			GridFont(nControlID, j+1, 1, 11);
			GridData(nControlID, j+1, 1, sTitle[j]);
		}
	}
	Grid = NULL;
	delete Grid;
}

//==================================================================//
// Door Info 표시
//==================================================================//
void CScreen_Main::OnDisplayDoorInfo()
{
	CString sTopTitle[2] = { "TOP Door Close", "TOP Door Open" };
	CString sBtmTitle[2] = { "BTM Door Close", "BTM Door Open" };
	TSpread *Grid;
	CString sUpdateData;
	int nControlID;
	
	for (int i = 0; i < MAX_DOOR_; i++)
	{
		if(m_nCurrent_Door[i] != st_handler.nDetect_DoorOpen[i])
		{
			switch(i)
			{
			case 0:
			case 8:
				nControlID = IDC_CUSTOM_DOOR_TOP1_BTM9;
				break;
			case 1:
			case 9:
				nControlID = IDC_CUSTOM_DOOR_TOP2_BTM10;
				break;
			case 2:
			case 10:
				nControlID = IDC_CUSTOM_DOOR_TOP3_BTM11;
				break;
			case 3:
			case 11:
				nControlID = IDC_CUSTOM_DOOR_TOP4_BTM12;
				break;
			case 4:
			case 12:
				nControlID = IDC_CUSTOM_DOOR_TOP5_BTM13;
				break;
			case 5:
			case 13:
				nControlID = IDC_CUSTOM_DOOR_TOP6_BTM14;
				break;
			case 6:
			case 14:
				nControlID = IDC_CUSTOM_DOOR_TOP7_BTM15;
				break;
			case 7:
			case 15:
				nControlID = IDC_CUSTOM_DOOR_TOP8_BTM16;
				break;
			}
			Grid = (TSpread*)GetDlgItem(nControlID);
			m_nCurrent_Door[i] = st_handler.nDetect_DoorOpen[i];

			if (i > 7)
			{
				Grid->SetValue(1, 2, sBtmTitle[m_nCurrent_Door[i]]);
				if (m_nCurrent_Door[i] == TRUE)
				{
					GridColor(nControlID, 2, 1, RED_C, WHITE_C);
				}
				else
				{
					GridColor(nControlID, 2, 1, GREEN_D, WHITE_C);
				}
			}
			else
			{
				Grid->SetValue(1, 1, sTopTitle[m_nCurrent_Door[i]]);
				if (m_nCurrent_Door[i] == TRUE)
				{
					GridColor(nControlID, 1, 1, RED_C, WHITE_C);
				}
				else
				{
					GridColor(nControlID, 1, 1, GREEN_D, WHITE_C);
				}
			}
		}
	}
}

//==================================================================//
// Supply Info 표시
//==================================================================//
void CScreen_Main::OnSupplyInfoInit()
{
	TSpread *Grid;
	CString sTitle[3] = { "Dvc Type", "Ready Accy Cnt", "NG Out Count"};
	
	Grid = (TSpread*)GetDlgItem(IDC_CUSTOM_SUPPLY_INFO);
	
	// 그리드 헤더 미사용
	Grid->SetBool(SSB_SHOWCOLHEADERS, FALSE); // col header 삭제
	Grid->SetBool(SSB_SHOWROWHEADERS, FALSE); // row header 삭제
	// 스크롤 미생성
	Grid->SetBool(SSB_HORZSCROLLBAR, FALSE); // 좌우 스크롤바
	Grid->SetBool(SSB_VERTSCROLLBAR, FALSE); // 상하 스크롤바
	// 입력 글자수에 맞게 자동 맞춤 그리드 크기 변경 기능 미사용
	Grid->SetBool(SSB_AUTOSIZE, FALSE); // 상하 스크롤바
	// 그리드 갯수 설정
	Grid->SetMaxRows(3);
	Grid->SetMaxCols(2);
	
	int i, j;
	for(i=0; i<3; i++)
	{
		Grid->SetRowHeightInPixels(i+1, 26);

		for(j=0; j<2; j++)
		{
			switch(j)
			{
			case 0:
				Grid->SetColWidthInPixels(j+1, 95);
				GridColor(IDC_CUSTOM_SUPPLY_INFO, i + 1, j + 1, BLUE_D, WHITE_C);
				break;
			case 1:
				Grid->SetColWidthInPixels(j+1, 85);
				GridColor(IDC_CUSTOM_SUPPLY_INFO, i + 1, j + 1, BLACK_L, YELLOW_L);
				break;
			}
			GridControl(IDC_CUSTOM_SUPPLY_INFO, STATIC, i+1, j+1, 3);
		}
		GridFont(IDC_CUSTOM_SUPPLY_INFO, i+1, 1, 14);
		GridData(IDC_CUSTOM_SUPPLY_INFO, i+1, 1, sTitle[i]);
		GridFont(IDC_CUSTOM_SUPPLY_INFO, i+1, 2, 15);
	}
	Grid = NULL;
	delete Grid;
}

//==================================================================//
// Supply Info 표시
//==================================================================//
void CScreen_Main::OnDisplaySupplyInfo()
{
	TSpread *Grid;

	CString sUpdateData;

	if(m_nCurrent_Supply_LotType != st_basic.nMode_SupplyDevice)
	{
		Grid = (TSpread*)GetDlgItem(IDC_CUSTOM_SUPPLY_INFO);

		m_nCurrent_Supply_LotType = st_basic.nMode_SupplyDevice;
		
		if (m_nCurrent_Supply_LotType == 1)
		{
			sUpdateData = "2.5 Inch(B)";
		} 
		else if(m_nCurrent_Supply_LotType == 2)
		{
			sUpdateData = "Desktop(B)";
		}
		else if (m_nCurrent_Supply_LotType == 3)
		{ 
			sUpdateData = "M-SATA(B)";
		}
		else if (m_nCurrent_Supply_LotType == 12)
		{
			sUpdateData = "Desktop(T)";
		}
		else if (m_nCurrent_Supply_LotType == 13)
		{
			sUpdateData = "M-SATA(T)";
		}
		else
		{
			sUpdateData = "NONE";
		}
		
		Grid->SetValue(2, 1, sUpdateData);
	}
	
	if(m_nCurrent_Supply_Count != st_map.nResp_AccyReadyCNT ||
		m_nCurrent_LoadingConv_State[0] != !g_ioMgr.get_in_bit(stIO.i_Chk_AccyBoxConvDetection[SIDE_THIRD_]) ||
		m_nCurrent_LoadingConv_State[1] != !g_ioMgr.get_in_bit(stIO.i_Chk_AccyBoxConvDetection[SIDE_SECOND_]) ||
		m_nCurrent_LoadingConv_State[2] != !g_ioMgr.get_in_bit(stIO.i_Chk_AccyBoxConvDetection[SIDE_FIRST_]))
	{
		Grid = (TSpread*)GetDlgItem(IDC_CUSTOM_SUPPLY_INFO);

		// yangkyu [2015.02.21]
		int nReadIO[3] = {0,};
		// 센서가 B접이라 센서 상태 확인 시 반전 사용
		m_nCurrent_LoadingConv_State[0] = !g_ioMgr.get_in_bit(stIO.i_Chk_AccyBoxConvDetection[SIDE_THIRD_]);
		m_nCurrent_LoadingConv_State[1] = !g_ioMgr.get_in_bit(stIO.i_Chk_AccyBoxConvDetection[SIDE_SECOND_]);
		m_nCurrent_LoadingConv_State[2] = !g_ioMgr.get_in_bit(stIO.i_Chk_AccyBoxConvDetection[SIDE_FIRST_]);
 
		m_nCurrent_Supply_Count = st_map.nResp_AccyReadyCNT + m_nCurrent_LoadingConv_State[0] * 160 + m_nCurrent_LoadingConv_State[1] * 160 + m_nCurrent_LoadingConv_State[2] * 160;
		sUpdateData.Format("%d", m_nCurrent_Supply_Count);
		Grid->SetValue(2, 2, sUpdateData);
	}
	if(m_nCurrent_NG_Count != st_map.nNGOutCNT)
	{
		Grid = (TSpread*)GetDlgItem(IDC_CUSTOM_SUPPLY_INFO);
		
		m_nCurrent_NG_Count = st_map.nNGOutCNT;
		sUpdateData.Format("%d", m_nCurrent_NG_Count);
		Grid->SetValue(2, 3, sUpdateData);
	}
	Grid = NULL;
	delete Grid;
}

//==================================================================//
// SMEMA Info 표시
//==================================================================//
void CScreen_Main::OnSMEMAInfoInit()
{
	TSpread *Grid;
	CString sTitle[4] = { "Rear Ready", "Rear Interlock", "Rear Request", "Rear Transfer"};
	
	Grid = (TSpread*)GetDlgItem(IDC_CUSTOM_SMEMA_INFO);
	
	// 그리드 헤더 미사용
	Grid->SetBool(SSB_SHOWCOLHEADERS, FALSE); // col header 삭제
	Grid->SetBool(SSB_SHOWROWHEADERS, FALSE); // row header 삭제
	// 스크롤 미생성
	Grid->SetBool(SSB_HORZSCROLLBAR, FALSE); // 좌우 스크롤바
	Grid->SetBool(SSB_VERTSCROLLBAR, FALSE); // 상하 스크롤바
	// 입력 글자수에 맞게 자동 맞춤 그리드 크기 변경 기능 미사용
	Grid->SetBool(SSB_AUTOSIZE, FALSE); // 상하 스크롤바
	// 그리드 갯수 설정
	Grid->SetMaxRows(4);
	Grid->SetMaxCols(2);
	
	int i, j;
	for(i=0; i<4; i++)
	{
		Grid->SetRowHeightInPixels(i+1, 26);
		
		for(j=0; j<2; j++)
		{
			switch(j)
			{
			case 0:
				Grid->SetColWidthInPixels(j+1, 120);
				GridColor(IDC_CUSTOM_SMEMA_INFO, i + 1, j + 1, BLUE_D, WHITE_C);
				break;
			case 1:
				Grid->SetColWidthInPixels(j+1, 90);
				GridColor(IDC_CUSTOM_SMEMA_INFO, i + 1, j + 1, BLACK_L, YELLOW_L);
				break;
			}
			GridControl(IDC_CUSTOM_SMEMA_INFO, STATIC, i+1, j+1, 3);
		}
		GridFont(IDC_CUSTOM_SMEMA_INFO, i+1, 1, 12);
		GridData(IDC_CUSTOM_SMEMA_INFO, i+1, 1, sTitle[i]);
		GridFont(IDC_CUSTOM_SMEMA_INFO, i+1, 2, 13);
	}
	Grid = NULL;
	delete Grid;
}

//==================================================================//
// SMEMA Info 표시
//==================================================================//
void CScreen_Main::OnDisplaySMEMAInfo()
{
	TSpread *Grid;

	int nSig_Req	= g_ioMgr.get_in_bit(stIO.i_Chk_ReqFromMain);
	int nSig_Trsf	= g_ioMgr.get_in_bit(stIO.i_Chk_TrsfFromMain);
	int nSig_RearReady = g_ioMgr.get_out_bit(stIO.o_Smema_Ready2Main, IO_OFF);
	int nSig_nRearInterlock = g_ioMgr.get_out_bit(stIO.o_Smema_Emergency2Main, IO_OFF);

	if(m_nCurrent_RearReady != nSig_RearReady)
	{
		Grid = (TSpread*)GetDlgItem(IDC_CUSTOM_SMEMA_INFO);
		
		m_nCurrent_RearReady = nSig_RearReady;

		if (m_nCurrent_RearReady == IO_ON)
		{
			Grid->SetValue(2, 1, "IO_ON");
			GridColor(IDC_CUSTOM_SMEMA_INFO, 1, 2, GREEN_B, WHITE_C);
		}
		else
		{
			Grid->SetValue(2, 1, "IO_OFF");
			GridColor(IDC_CUSTOM_SMEMA_INFO, 1, 2, RED_A, WHITE_C);
		}
	}
	
	if(m_nCurrent_RearInterlock != nSig_nRearInterlock)
	{
		Grid = (TSpread*)GetDlgItem(IDC_CUSTOM_SMEMA_INFO);
		
		m_nCurrent_RearInterlock = nSig_nRearInterlock;

		if (m_nCurrent_RearInterlock == IO_ON)
		{
			Grid->SetValue(2, 2, "IO_ON");
			GridColor(IDC_CUSTOM_SMEMA_INFO, 2, 2, GREEN_B, WHITE_C);
		}
		else
		{
			Grid->SetValue(2, 2, "IO_OFF");
			GridColor(IDC_CUSTOM_SMEMA_INFO, 2, 2, RED_A, WHITE_C);
		}
	}

	if(m_nCurrent_RearRequest != nSig_Req)
	{
		Grid = (TSpread*)GetDlgItem(IDC_CUSTOM_SMEMA_INFO);
		
		m_nCurrent_RearRequest = nSig_Req;

		if (m_nCurrent_RearRequest == IO_ON)
		{
			Grid->SetValue(2, 3, "IO_ON");
			GridColor(IDC_CUSTOM_SMEMA_INFO, 3, 2, GREEN_B, WHITE_C);
		}
		else
		{
			Grid->SetValue(2, 3, "IO_OFF");
			GridColor(IDC_CUSTOM_SMEMA_INFO, 3, 2, RED_A, WHITE_C);
		}
	}

	if(m_nCurrent_RearTransfer != nSig_Trsf)
	{
		Grid = (TSpread*)GetDlgItem(IDC_CUSTOM_SMEMA_INFO);
		
		m_nCurrent_RearTransfer = nSig_Trsf;

		if (m_nCurrent_RearTransfer == IO_ON)
		{
			Grid->SetValue(2, 4, "IO_ON");
			GridColor(IDC_CUSTOM_SMEMA_INFO, 4, 2, GREEN_B, WHITE_C);
		}
		else
		{
			Grid->SetValue(2, 4, "IO_OFF");
			GridColor(IDC_CUSTOM_SMEMA_INFO, 4, 2, RED_A, WHITE_C);
		}
	}
}

//==================================================================//
// Gripper Info 표시
//==================================================================//
void CScreen_Main::OnGripperInfoInit()
{
	TSpread *Grid;
	CString sTitle[4] = { "Gripper 1", "Gripper 2", "Gripper 3", "Gripper 4"};
	
	Grid = (TSpread*)GetDlgItem(IDC_CUSTOM_GRIPPER_INFO);
	
	// 그리드 헤더 미사용
	Grid->SetBool(SSB_SHOWCOLHEADERS, FALSE); // col header 삭제
	Grid->SetBool(SSB_SHOWROWHEADERS, FALSE); // row header 삭제
	// 스크롤 미생성
	Grid->SetBool(SSB_HORZSCROLLBAR, FALSE); // 좌우 스크롤바
	Grid->SetBool(SSB_VERTSCROLLBAR, FALSE); // 상하 스크롤바
	// 입력 글자수에 맞게 자동 맞춤 그리드 크기 변경 기능 미사용
	Grid->SetBool(SSB_AUTOSIZE, FALSE); // 상하 스크롤바
	// 그리드 갯수 설정
	Grid->SetMaxRows(4);
	Grid->SetMaxCols(2);
	
	int i, j;
	for(i=0; i<4; i++)
	{
		Grid->SetRowHeightInPixels(i+1, 26);
		
		for(j=0; j<2; j++)
		{
			switch(j)
			{
			case 0:
				Grid->SetColWidthInPixels(j+1, 78);
				GridColor(IDC_CUSTOM_GRIPPER_INFO, i + 1, j + 1, BLUE_D, WHITE_C);
				break;
			case 1:
				Grid->SetColWidthInPixels(j+1, 200);
				GridColor(IDC_CUSTOM_GRIPPER_INFO, i + 1, j + 1, BLACK_L, YELLOW_L);
				break;
			}
			GridControl(IDC_CUSTOM_GRIPPER_INFO, STATIC, i+1, j+1, 3);
		}
		GridFont(IDC_CUSTOM_GRIPPER_INFO, i+1, 1, 12);
		GridData(IDC_CUSTOM_GRIPPER_INFO, i+1, 1, sTitle[i]);
		GridFont(IDC_CUSTOM_GRIPPER_INFO, i+1, 2, 13);
	}
	Grid = NULL;
	delete Grid;
}

//==================================================================//
// Gripper Info 표시
//==================================================================//
void CScreen_Main::OnDisplayGripperInfo()
{
	TSpread *Grid;
	int ncount = 0;
	int i;

	for (i = 0; i < 4; i++)
	{
		if(m_nCurrent_Gripper[i] != st_map.nBCR_State[i])
		{
			ncount++;	
		}
	}

	if (ncount > 0)
	{
		Grid = (TSpread*)GetDlgItem(IDC_CUSTOM_GRIPPER_INFO);
		
		for (i = 0; i < 4; i++)
		{
			m_nCurrent_Gripper[i] = st_map.nBCR_State[i];
			
			if (m_nCurrent_Gripper[i] == -1)
			{
				// yangkyu [2015.03.24]
				// BCR Data 표시하도록 추가
				GridData(IDC_CUSTOM_GRIPPER_INFO, i + 1, 2, "");
				GridColor(IDC_CUSTOM_GRIPPER_INFO, i + 1, 2, BLACK_L, YELLOW_L);
			}
			else if(m_nCurrent_Gripper[i] == 1)
			{
				// yangkyu [2015.03.24]
				// BCR Data 표시하도록 추가
				GridData(IDC_CUSTOM_GRIPPER_INFO, i + 1, 2, st_work.sRbtBCR_Data[i]);
				GridColor(IDC_CUSTOM_GRIPPER_INFO, i + 1, 2, GREEN_B, BLUE_D);
			}
			else if(m_nCurrent_Gripper[i] == 0)
			{
				// yangkyu [2015.03.24]
				// BCR Data 표시하도록 추가
				GridData(IDC_CUSTOM_GRIPPER_INFO, i + 1, 2, "NO-READ");
				GridColor(IDC_CUSTOM_GRIPPER_INFO, i + 1, 2, RED_A, WHITE_C);
			}
		}
	}
}

void CScreen_Main::OnMain_Button_Set()
{
	short	shBtnColor = 30;
	
	m_chk_forcedischarge.SetFont(mp_main_font);
	m_chk_forcedischarge.SetIcon(IDI_RED_LED_ICON, IDI_GRAY_LED_ICON);
	m_chk_forcedischarge.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_chk_forcedischarge.SetButtonColor(1);
}

void CScreen_Main::OnMain_GroupBox_Set()
{
	CSxLogFont main_font(15, FW_SEMIBOLD, false, "Arial");

	// time info
	m_group_time_info.SetFont(main_font);
	m_group_time_info.SetCatptionTextColor(RGB(145,25,0));
	m_group_time_info.SetFontBold(TRUE);

	// Forcedischarge
	m_group_forcedischarge_state.SetFont(main_font);
	m_group_forcedischarge_state.SetCatptionTextColor(RGB(145,25,0));
	m_group_forcedischarge_state.SetFontBold(TRUE);

	// Supply Info
	m_group_supply_info.SetFont(main_font);
	m_group_supply_info.SetCatptionTextColor(RGB(145,25,0));
	m_group_supply_info.SetFontBold(TRUE);

	// SMEMA Info
	m_group_smema_info.SetFont(main_font);
	m_group_smema_info.SetCatptionTextColor(RGB(145,25,0));
	m_group_smema_info.SetFontBold(TRUE);

	// Gripper Info
	m_group_gripper_info.SetFont(main_font);
	m_group_gripper_info.SetCatptionTextColor(RGB(145,25,0));
	m_group_gripper_info.SetFontBold(TRUE);
}

void CScreen_Main::OnMain_Digital_Count_Set()
{
	// Time 설정
	m_dgt_time_run.SetStyle(CDigit::DS_TIMER, NULL, CDigit::DC_GREEN, CDigit::DC_GREEN);
	m_dgt_time_stop.SetStyle(CDigit::DS_TIMER, NULL, CDigit::DC_RED, CDigit::DC_RED);
	m_dgt_time_alarm.SetStyle(CDigit::DS_TIMER, NULL, CDigit::DC_YELLOW, CDigit::DC_YELLOW);
	m_dgt_time_mtbi.SetStyle(CDigit::DS_TIMER, NULL, CDigit::DC_GREEN, CDigit::DC_GREEN);
}

void CScreen_Main::OnMain_Label_Set()
{
	// Run 
	m_label_time_run.SetFont(mp_main_font);
	m_label_time_run.SetWindowText(_T("Run"));
	m_label_time_run.SetCenterText();
	m_label_time_run.SetColor(RGB(0,0,0));
	m_label_time_run.SetGradientColor(RGB(0,0,255));
	m_label_time_run.SetTextColor(RGB(255,255,255));
	
	// Stop 
	m_label_time_stop.SetFont(mp_main_font);
	m_label_time_stop.SetWindowText(_T("Stop"));
	m_label_time_stop.SetCenterText();
	m_label_time_stop.SetColor(RGB(0,0,0));
	m_label_time_stop.SetGradientColor(RGB(255,0,0));
	m_label_time_stop.SetTextColor(RGB(255,255,255));
	
	// Alarm
	m_label_time_alarm.SetFont(mp_main_font);
	m_label_time_alarm.SetWindowText(_T("Alarm"));
	m_label_time_alarm.SetCenterText();
	m_label_time_alarm.SetColor(RGB(0,0,0));
	m_label_time_alarm.SetGradientColor(RGB(255,255,0));
	m_label_time_alarm.SetTextColor(RGB(255,255,255));

	// mtbi 
	m_label_time_mtbi.SetFont(mp_main_font);
	m_label_time_mtbi.SetWindowText(_T("MTBI"));
	m_label_time_mtbi.SetCenterText();
	m_label_time_mtbi.SetColor(RGB(0,0,0));
	m_label_time_mtbi.SetGradientColor(RGB(178, 98, 4));
	m_label_time_mtbi.SetTextColor(RGB(255,255,255));

	// ForceDischarge State
	m_msg_forcedischarge.SetText(_T(""));
	m_msg_forcedischarge.SetTextColor(RGB(25,25,85));
	m_msg_forcedischarge.SetBkColor(RGB(180,230,250));
	m_msg_forcedischarge.SetFontBold(TRUE);
	m_msg_forcedischarge.SetFontName("Tahoma");
	m_msg_forcedischarge.SetFontSize(14);
	
	// 규이리 추가 [2015.03.20]
	m_lblMatCode.SetText(_T(""));
	m_lblMatCode.SetTextColor(RGB(25,25,85));
	m_lblMatCode.SetBkColor(RGB(180,230,250));
	m_lblMatCode.SetFontBold(TRUE);
	m_lblMatCode.SetFontName("Tahoma");
	m_lblMatCode.SetFontSize(14);
}

void CScreen_Main::OnMain_Picture_Set()
{
	
}

void CScreen_Main::OnChkForceDischarge() 
{
	CString strTmp;
	int mn_response = 0;  // 대화 상자에 대한 리턴 값 저장 변수

	CDialog_Select select_dlg;

	CDialog_Message msg_dlg;

	// yangkyu [2015.04.11]
	if (st_work.nEqpStatus == dRUN)
	{
		st_msg.str_fallacy_msg = _T("EQP State STOP");
		msg_dlg.DoModal();
		m_chk_forcedischarge.SetCheck(FORCEDISCHARGE_OFF);
		return;
	}

	if (st_work.nForceDischarge != FORCEDISCHARGE_OFF)
	{
		m_chk_forcedischarge.SetCheck(FORCEDISCHARGE_ON);
		return;
	}
	// Chk 상태를 확인
	if (m_chk_forcedischarge.GetCheck() == CTL_YES)
	{
		st_msg.mstr_confirm_msg = _T("Force Discharge Would you like to ?");	
	}

	mn_response = select_dlg.DoModal();
	
	if (mn_response == IDOK)
	{
		if (m_chk_forcedischarge.GetCheck() == FORCEDISCHARGE_ON)
		{
			st_work.nForceDischargeType = 0;
			st_work.nForceDischarge = FORCEDISCHARGE_ON;
		}
	}
	else
	{
		m_chk_forcedischarge.SetCheck(st_work.nForceDischarge);
		return;
	}
}

void CScreen_Main::OnMain_TimeInfo_Display()
{
	m_dgt_time_run.ActivateTimer(FALSE);
	m_dgt_time_stop.ActivateTimer(FALSE);
	m_dgt_time_alarm.ActivateTimer(FALSE);

	if (mn_time_select == 0)	// Total
	{
		m_dgt_time_run.SetTime(st_handler.m_tDR.GetTotalHours(), st_handler.m_tDR.GetMinutes(), st_handler.m_tDR.GetSeconds());
		m_dgt_time_stop.SetTime(st_handler.m_tDS.GetTotalHours(), st_handler.m_tDS.GetMinutes(), st_handler.m_tDS.GetSeconds());
		m_dgt_time_alarm.SetTime(st_handler.m_tDJ.GetTotalHours(), st_handler.m_tDJ.GetMinutes(), st_handler.m_tDJ.GetSeconds());
	}
	else
	{
		m_dgt_time_run.SetTime(st_handler.m_tR.GetTotalHours(), st_handler.m_tR.GetMinutes(), st_handler.m_tR.GetSeconds());
		m_dgt_time_stop.SetTime(st_handler.m_tS.GetTotalHours(), st_handler.m_tS.GetMinutes(), st_handler.m_tS.GetSeconds());
		m_dgt_time_alarm.SetTime(st_handler.m_tJ.GetTotalHours(), st_handler.m_tJ.GetMinutes(), st_handler.m_tJ.GetSeconds());
	}

	m_dgt_time_run.UpdateTimer();
	m_dgt_time_stop.UpdateTimer();
	m_dgt_time_alarm.UpdateTimer();

	//m_dgt_time_boxchange.SetFloatVal(st_handler.d_traychangetime[2] / 1000);
}

void CScreen_Main::OnMain_Time_Display(int n_state)
{
	m_dgt_time_run.ActivateTimer(FALSE);
	m_dgt_time_stop.ActivateTimer(FALSE);
	m_dgt_time_alarm.ActivateTimer(FALSE);

	switch(n_state)
	{
	case dRUN:
		m_dgt_time_run.ActivateTimer();
		if (mn_time_select == 0)	// Total
		{
			m_dgt_time_run.SetTime(st_handler.m_tDR.GetTotalHours(), st_handler.m_tDR.GetMinutes(), st_handler.m_tDR.GetSeconds());
		}
		else
		{	
			m_dgt_time_run.SetTime(st_handler.m_tR.GetTotalHours(), st_handler.m_tR.GetMinutes(), st_handler.m_tR.GetSeconds());
		}
		break;

	case dWARNING:
	case dJAM:
		m_dgt_time_alarm.ActivateTimer();
		if (mn_time_select == 0)	// Total
		{
			m_dgt_time_alarm.SetTime(st_handler.m_tDJ.GetTotalHours(), st_handler.m_tDJ.GetMinutes(), st_handler.m_tDJ.GetSeconds());
		}
		else
		{
			m_dgt_time_alarm.SetTime(st_handler.m_tJ.GetTotalHours(), st_handler.m_tJ.GetMinutes(), st_handler.m_tJ.GetSeconds());
		}
		break;
	
	case dLOCK:
	case dLOTEND:									// Lot End 시에도 Stop으로 시간을 올린다.
	case dSTOP:
		m_dgt_time_stop.ActivateTimer();
		if (mn_time_select == 0)	// Total
		{
			m_dgt_time_stop.SetTime(st_handler.m_tDS.GetTotalHours(), st_handler.m_tDS.GetMinutes(), st_handler.m_tDS.GetSeconds());
		}
		else
		{
			m_dgt_time_stop.SetTime(st_handler.m_tS.GetTotalHours(), st_handler.m_tS.GetMinutes(), st_handler.m_tS.GetSeconds());
		}
		break;
	}

	m_dgt_time_run.UpdateTimer();
	m_dgt_time_stop.UpdateTimer();
	m_dgt_time_alarm.UpdateTimer();	
}

//==================================================================//
// ForceDischarge State 표시
//==================================================================//
void CScreen_Main::OnBuildForceDischargeState()
{
	if (m_nForceDischarge == st_work.nForceDischarge)
	{
		return;
	}
	m_nForceDischarge = st_work.nForceDischarge;

	CString sUpdateData;
	switch(m_nForceDischarge)
	{
	case FORCEDISCHARGE_OFF:
		sUpdateData = _T("OFF");
		m_msg_forcedischarge.SetText(sUpdateData);
		m_msg_forcedischarge.SetTextColor(RGB(25,25,85));
		m_msg_forcedischarge.SetBkColor(RGB(180,230,250));
		m_chk_forcedischarge.SetCheck(FALSE);
		break;
		
	case FORCEDISCHARGE_ON:
		sUpdateData = _T("ON");
		m_msg_forcedischarge.SetText(sUpdateData);
		m_msg_forcedischarge.SetTextColor(RGB(25,25,85));
		m_msg_forcedischarge.SetBkColor(RGB(255,0,0));
		break;
	}
}

void CScreen_Main::GridColor(UINT nID, int row, int col, COLORREF bk, COLORREF tk)
{
	TSpread *Grid = (TSpread*)GetDlgItem(nID);
	Grid->SetColor(col, row, bk, tk);
	Grid->SetBackColorStyle(SS_BACKCOLORSTYLE_UNDERGRID);

	Grid = NULL;
	delete Grid;
}

void CScreen_Main::GridControl(UINT nID, int type, int row, int col, int pos)
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
			// SetValue(col, row, data); data = ""0"(1선택) data = "1"(2선택) data = "2"(3선택)
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

	Grid = NULL;
	delete Grid;
}

void CScreen_Main::GridData(UINT nID, int row, int col, CString data)
{
	TSpread *Grid = (TSpread*)GetDlgItem(nID);
	
	Grid->SetValue(col, row, data);

	Grid = NULL;
	delete Grid;
}

void CScreen_Main::GridFont(UINT nID, int row, int col, int size)
{
	TSpread *Grid = (TSpread *)GetDlgItem(nID);
	HFONT font;
	CString strTemp;
	
	font = CreateFont(size, 0, 0, 0, FW_BOLD, 0, 0, 0, 0, 0, 0, 0, 0, "돋음체");
	Grid->SetFont(col, row, font, TRUE);

	Grid = NULL;
	delete Grid;
}

void CScreen_Main::GridMerge(UINT nID, int srow, int scol, int nrow, int ncol)
{
	TSpread *Grid = (TSpread*)GetDlgItem(nID);
	
	Grid->AddCellSpan(scol, srow, ncol, nrow);

	Grid = NULL;
	delete Grid;
}

void CScreen_Main::OnButtonNetwork() 
{
	::PostMessage(st_handler.hWnd, WM_FORM_CHANGE, 60, DLG_SHOW_);

	// tEST
	//st_work.m_sRcvAccyMathCode = "LA69-01773B";
}

void CScreen_Main::OnButtonSmema() 
{
	::PostMessage(st_handler.hWnd, WM_FORM_CHANGE, 61, DLG_SHOW_);
}

void CScreen_Main::OnBmpAccyBoxView() 
{
	CDialog_Infor	dlgSelect;
	st_msg.mstr_confirm_msg = "Wanna (data reset) of Accy Box?";
	
	int mn_response = dlgSelect.DoModal();
	if (mn_response == IDOK)
	{
		// (Accy Box Conv) 박스 감지 상태는 센서 오류로 인한 상태라고 설정
		st_work.nSkipReq_ShiftTray = CTL_YES;
	}
	else if (mn_response == IDCANCEL)
	{
		st_work.nSkipReq_ShiftTray = CTL_NO;
	}
}

void CScreen_Main::OnButtonRealign() 
{
// 	g_ioMgr.set_out_bit(stIO.o_Cyl_AccyBoxClamp, IO_OFF);
// 	g_ioMgr.set_out_bit(stIO.o_Cyl_AccyBoxUnclamp, IO_ON);
// 	g_ioMgr.set_out_bit(stIO.o_Vac_AccyBoxGripper_VacOnOff[SIDE_LEFT_],	IO_OFF);
// 	g_ioMgr.set_out_bit(stIO.o_Vac_AccyBoxGripper_VacOnOff[SIDE_RIGHT_], IO_OFF);
// 	g_ioMgr.set_out_bit(stIO.o_Cyl_AccyBoxGripperFw, IO_OFF);
// 	g_ioMgr.set_out_bit(stIO.o_Cyl_AccyBoxGripperBw, IO_ON);
// 	g_ioMgr.set_out_bit(stIO.o_Cyl_AccyBoxPusher, IO_OFF);
//  	g_ioMgr.set_out_bit(stIO.o_Cyl_AccyBoxConvStopperUp, IO_OFF);
//  	g_ioMgr.set_out_bit(stIO.o_Cyl_AccyBoxConvStopperDn, IO_ON);
// 
// 	g_ioMgr.set_out_bit(stIO.o_Cyl_AccyBoxConvStopperUp, IO_ON);
//  	g_ioMgr.set_out_bit(stIO.o_Cyl_AccyBoxConvStopperDn, IO_OFF);
// 	g_ioMgr.set_out_bit(stIO.o_Cyl_AccyBoxPusher, IO_ON);
// 	g_ioMgr.set_out_bit(stIO.o_Cyl_AccyBoxClamp, IO_ON);
// 	g_ioMgr.set_out_bit(stIO.o_Cyl_AccyBoxUnclamp, IO_OFF);
// 	g_ioMgr.set_out_bit(stIO.o_Cyl_AccyBoxGripperFw, IO_ON);
// 	g_ioMgr.set_out_bit(stIO.o_Cyl_AccyBoxGripperBw, IO_OFF);
// 	
	

	if (0)
	{
		if (st_work.nEqpStatus == dSTOP && stSync.nReq_Clamp2Lifter_Work == SYNC_REQ_ACCY_BOX_CLAMPING_COMPLETE_)
		{
			CDialog_Infor	dlgSelect;
			st_msg.mstr_confirm_msg = "Wanna Accy ReAlign?";
			
			int mn_response = dlgSelect.DoModal();
			if (mn_response == IDOK)
			{
				st_map.m_nReq_ReAlign = SYNC_REQ_ACCY_REALIGN;
			}
		}
		else
		{
			CDialog_Message dlgMSG;
			
			st_msg.str_fallacy_msg = _T("Possible to EQP State Stop and Box Clamping");
			dlgMSG.DoModal();
		}
	}
	
}

// yangkyu [2015.02.21]
void CScreen_Main::OnBmpNgBufferAccy1() 
{
	CDialog_SetReset	dlgSelect;

	st_msg.mstr_confirm_msg = "Wanna (data set or Reset) of NG Buffer Accy1?";
	
	int mn_response = dlgSelect.DoModal();
	if (mn_response == IDOK)
	{
		st_map.nNGBuffer[0] = dlgSelect.mn_Result;
	}	
}

// yangkyu [2015.02.21]
void CScreen_Main::OnBmpNgBufferAccy2() 
{
	CDialog_SetReset	dlgSelect;
	
	st_msg.mstr_confirm_msg = "Wanna (data set or Reset) of NG Buffer Accy2?";
	
	int mn_response = dlgSelect.DoModal();
	if (mn_response == IDOK)
	{
		st_map.nNGBuffer[1] = dlgSelect.mn_Result;
	}	
}

// yangkyu [2015.02.21]
void CScreen_Main::OnBmpNgBufferAccy3() 
{
	CDialog_SetReset	dlgSelect;
	
	st_msg.mstr_confirm_msg = "Wanna (data set or Reset) of NG Buffer Accy3?";
	
	int mn_response = dlgSelect.DoModal();
	if (mn_response == IDOK)
	{
		st_map.nNGBuffer[2] = dlgSelect.mn_Result;
	}	
}

// yangkyu [2015.02.21]
void CScreen_Main::OnBmpNgBufferAccy4() 
{
	CDialog_SetReset	dlgSelect;
	
	st_msg.mstr_confirm_msg = "Wanna (data set or Reset) of NG Buffer Accy4?";
	
	int mn_response = dlgSelect.DoModal();
	if (mn_response == IDOK)
	{
		st_map.nNGBuffer[3] = dlgSelect.mn_Result;
	}	
}

//==================================================================//
// 악세사리 매칭 코드 화면 출력
// 규이리 추가 [2015.03.20]
//==================================================================//
void CScreen_Main::OnDisp_AcceMatCode() 
{
	// 악세사리 매칭 코드가 동일하면 리턴시킴
	if (m_sAcceMatCode == st_work.m_sRcvAccyMathCode)
	{
		return;
	}
	m_sAcceMatCode = st_work.m_sRcvAccyMathCode;

	CString sUpdateData;
	sUpdateData.Format( "[ ACCE_MAT_CODE ] : %s", m_sAcceMatCode);
	m_lblMatCode.SetText(sUpdateData);
}

void CScreen_Main::OnBmpBufferConvAccyFront() 
{
	CDialog_Infor	dlgSelect;
	st_msg.mstr_confirm_msg = "Wanna (data reset) of Front Accy?";
	
	int mn_response = dlgSelect.DoModal();
	if (mn_response == IDOK)
	{
		st_work.nSkipReq_BufferConv[SIDE_FRONT_] = CTL_YES;
	}
	else if (mn_response == IDCANCEL)
	{
		st_work.nSkipReq_BufferConv[SIDE_FRONT_] = CTL_NO;
	}
	if (st_handler.cwnd_main != NULL)
	{
		st_handler.cwnd_main->PostMessage(WM_UPDATE_MAIN, PLACE_BUFFER_CONV_);	// 화면 표시 요청
	}
}

void CScreen_Main::OnBmpBufferConvAccyRear() 
{
	CDialog_Infor	dlgSelect;
	st_msg.mstr_confirm_msg = "Wanna (data reset) of Rear Accy?";
	
	int mn_response = dlgSelect.DoModal();
	if (mn_response == IDOK)
	{
		st_work.nSkipReq_BufferConv[SIDE_REAR_] = CTL_YES;
	}
	else if (mn_response == IDCANCEL)
	{
		st_work.nSkipReq_BufferConv[SIDE_REAR_] = CTL_NO;
	}
	if (st_handler.cwnd_main != NULL)
	{
		st_handler.cwnd_main->PostMessage(WM_UPDATE_MAIN, PLACE_BUFFER_CONV_);	// 화면 표시 요청
	}
}

void CScreen_Main::OnBtnBcrReadTest() 
{
	if (st_work.nEqpStatus == dSTOP)
	{
		// 김양규 추가 [2015.05.19]
		// BCR Test Mode 일경우에는 화면에 출력 후 끝내도록 하기위해 st_work.nModeBCR_Test값을 True로 함.
		st_work.nModeBCR_Test = TRUE;
		// 바코드 읽기 요청
		::PostMessage(st_handler.hWnd, WM_DATA_SEND, PORT_BCR_ACCY_, 0);
		st_serial.n_rec_chk[PORT_BCR_ACCY_ -1] = FALSE;
	}
	else
	{
		CDialog_Message dlgMSG;
		
		st_msg.str_fallacy_msg = _T("Possible to EQP State Stop");
		dlgMSG.DoModal();
	}
}
