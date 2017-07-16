// Dialog_List.cpp : implementation file
//

#include "stdafx.h"
#include "Handler.h"
#include "Dialog_List.h"

//#include <atlconv.h>	// 유니 코드 스트링 변환
#include "Public_Function.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CMutex logmutex;

/////////////////////////////////////////////////////////////////////////////
// CDialog_List dialog


CDialog_List::CDialog_List()
{
	//{{AFX_DATA_INIT(CDialog_List)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDialog_List::DoDataExchange(CDataExchange* pDX)
{
	CInitDialogBar::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialog_List)
	DDX_Control(pDX, IDC_MSG_EQP_ID, m_msg_eqp_id);
	DDX_Control(pDX, IDC_MSG_DEVICE_NAME, m_msg_device_name);
	DDX_Control(pDX, IDC_LABEL_EQP_ID, m_label_eqp_id);
	DDX_Control(pDX, IDC_LABEL_DEVICE_NAME, m_label_device_name);
	DDX_Control(pDX, IDC_LIST_DATA, m_list_data);
	DDX_Control(pDX, IDC_LIST_CLOCK, m_list_clock);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialog_List, CInitDialogBar)
	//{{AFX_MSG_MAP(CDialog_List)
	ON_WM_DESTROY()
	ON_LBN_DBLCLK(IDC_LIST_DATA, OnDblclkListData)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_LIST_DATA, OnListDataUpdate)	// 리스트 컨트롤에 데이터 출력
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialog_List message handlers

BOOL CDialog_List::OnInitDialogBar() 
{
	CInitDialogBar::OnInitDialogBar();

	// 컨트롤에 사용할 폰트 정보 생성
	mp_fontList = NULL;
	mp_fontList = new CFont;
	mp_fontList->CreateFont(14,6,0,0,900,0,0,0,0,0,0,PROOF_QUALITY,0,"MS Sans Serif");

	OnSet_LblCtrl();	// 칼라 라벨 생성
	OnSet_ClockCtrl();	// 디지털 시계 컨트롤 생성
	OnSet_TabStops(TRUE);	// 리스트 컨트롤에 Tab Stop 기능 설정

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialog_List::OnDestroy() 
{
	// 생성한 폰트 삭제
	delete mp_fontList;
	mp_fontList = NULL;

	if (st_handler.nMenuLock != FALSE)
	{
		st_handler.nMenuLock = FALSE;  // 메뉴 사용 가능
	}

	CInitDialogBar::OnDestroy();
}

void CDialog_List::OnDblclkListData() 
{
	
}

//==================================================================//
// 리스트 컨트롤에 메시지 추가하라는 요청
//==================================================================//
LRESULT CDialog_List::OnListDataUpdate(WPARAM wParam, LPARAM lParam)
{
	CSingleLock mute(&logmutex);
	mute.Lock();

	int nzPort = (int)wParam;  // 통신 포트 번호 할당
	OnReset_ListContent();	// 리스트 컨트롤에 출력된 문자열 리셋 [500개 기준]

	switch(lParam)
	{
	case RECEIVE_MSG:
		OnAdd_CommRcvMsg(nzPort);	// [->리스트컨트롤] 통신 수신 메시지 추가
		break;

	case SEND_MSG:
		OnAdd_CommSendMsg(nzPort);	// [->리스트컨트롤] 통신 전송 메시지 추가
		break;

	case NORMAL_MSG:
		OnAdd_NormalMsg();	// [->리스트컨트롤] 정상적인 메시지 추가
		break;

	case ABNORMAL_MSG:
		OnAdd_AbnormalMsg();	// [->리스트컨트롤] 비정상적인 메시지 추가
		break;
		
	case MACHINE_INFO:
		OnDisp_EqpInfo();	// 디바이스명 및 EQPID 정보 화면 출력
		break;

	case DEVICE_INFO:
		OnDisp_EqpInfo();	// 디바이스명 및 EQPID 정보 화면 출력
		break;
	}
	
	mute.Unlock();

	return 0;
}

//==================================================================//
// 칼라 라벨 생성
//==================================================================//
void CDialog_List::OnSet_LblCtrl() 
{
	m_label_eqp_id.SetText("EQP ID");
	m_label_eqp_id.SetTextColor(RGB(0,0,0));
	m_label_eqp_id.SetBkColor(RGB(181,182,140));
	m_label_eqp_id.SetFontBold(TRUE);
	m_label_eqp_id.SetFontName("MS Sans Serif");
	m_label_eqp_id.SetFontSize(11);

	m_msg_eqp_id.SetFont(mp_fontList);
	m_msg_eqp_id.SetCenterText();
	m_msg_eqp_id.SetColor(RGB(0,0,0));
	m_msg_eqp_id.SetGradientColor(RGB(0,0,0));
	m_msg_eqp_id.SetTextColor(RGB(255,255,255));

	m_label_device_name.SetText("DEVICE NAME");
	m_label_device_name.SetTextColor(RGB(0,0,0));
	m_label_device_name.SetBkColor(RGB(181,182,140));
	m_label_device_name.SetFontBold(TRUE);
	m_label_device_name.SetFontName("MS Sans Serif");
	m_label_device_name.SetFontSize(11);

	m_msg_device_name.SetFont(mp_fontList);
	m_msg_device_name.SetCenterText();
	m_msg_device_name.SetColor(RGB(0,0,0));
	m_msg_device_name.SetGradientColor(RGB(0,0,0));
	m_msg_device_name.SetTextColor(RGB(255,255,255));
}

//==================================================================//
// 디지털 시계 컨트롤 생성
//==================================================================//
void CDialog_List::OnSet_ClockCtrl() 
{
	m_list_clock.Start(IDB_CLOCKST_PANE, IDB_CLOCKST_BIG, IDB_CLOCKST_SMALL);
}

//==================================================================//
// 리스트 컨트롤에 Tab Stop 기능 설정
//==================================================================//
void CDialog_List::OnSet_TabStops(int nzTabStops) 
{
	DWORD dwRemove	= (nzTabStops ? 0:LBS_USETABSTOPS);
	DWORD dwAdd		= (nzTabStops ? LBS_USETABSTOPS:0);

	m_list_data.ModifyStyle(dwRemove, dwAdd);
	m_list_data.RedrawWindow();
}

//==================================================================//
// 리스트 컨트롤에 출력된 문자열 리셋 [500개 기준]
//==================================================================//
void CDialog_List::OnReset_ListContent() 
{
	int nIdx = m_list_data.GetCount();
	if (nIdx < 500)	{
		return;
	}
	// 리스트 컨트롤에 추가된 모든 문자열 삭제
	m_list_data.ResetContent();
}

//==================================================================//
// [->리스트컨트롤] 정상적인 메시지 추가
//==================================================================//
void CDialog_List::OnAdd_NormalMsg() 
{
	SYSTEMTIME systime;
	GetLocalTime(&systime);
	
	CString sTime;
	sTime.Format("[%02d:%02d:%02d.%03d] ",
		systime.wHour,
		systime.wMinute,
		systime.wSecond,
		systime.wMilliseconds);

	// 일정 시간 지난 경우에만 리스트에 내용 추가함
	if (m_sOldMsgNormal == st_msg.c_normal_msg)
	{
		// 메시지가 중간에 변질되어 계속 동일한 문자 출력되는 것 방지
		m_lTimeNormal[1] = GetCurrentTime();
		m_lTimeNormal[2] = m_lTimeNormal[1] - m_lTimeNormal[0];
		if (m_lTimeNormal[2] > MSG_TERM_ / 4)
		{
			// 내용 저장
		}
		else
		{
			m_lTimeNormal[0] = GetCurrentTime();
			return;
		}
	}

	std::string sAddMsg = "";
	if (st_msg.c_normal_msg != _T(""))
	{
		m_lTimeNormal[0] = GetCurrentTime();

		m_sOldMsgNormal = st_msg.c_normal_msg;
		sAddMsg = sTime + st_msg.c_normal_msg;
	}
	else
	{
		sAddMsg = "Display Message is Empty";
	}

	int nIdx = m_list_data.AddString(sAddMsg.c_str(), RGB(0,0,255));
	m_list_data.SetCurSel(nIdx);
}

//==================================================================//
// [->리스트컨트롤] 비정상적인 메시지 추가
//==================================================================//
void CDialog_List::OnAdd_AbnormalMsg() 
{
	SYSTEMTIME systime;
	GetLocalTime(&systime);
	
	CString sTime;
	sTime.Format("[%02d:%02d:%02d.%03d] ",
		systime.wHour,
		systime.wMinute,
		systime.wSecond,
		systime.wMilliseconds);

	// 일정 시간 지난 경우에만 리스트에 내용 추가함
	if (mm_sOldMsgAbnormal == st_msg.c_abnormal_msg)
	{
		// 메시지가 중간에 변질되어 계속 동일한 문자 출력되는 것 방지
		m_lTimeAbnormal[1] = GetCurrentTime();
		m_lTimeAbnormal[2] = m_lTimeAbnormal[1] - m_lTimeAbnormal[0];
		if (m_lTimeAbnormal[2] > MSG_TERM_)
		{
			// 내용 저장
		}
		else
		{
			m_lTimeAbnormal[0] = GetCurrentTime();
			return;
		}
	}

	std::string sAddMsg = "";
	if (st_msg.c_abnormal_msg != _T(""))
	{
		m_lTimeAbnormal[0] = GetCurrentTime();

		mm_sOldMsgAbnormal = st_msg.c_abnormal_msg;
		sAddMsg = sTime + st_msg.c_abnormal_msg;
	}
	else
	{
		sAddMsg = "Display Message is Empty";
	}

	int nIdx = m_list_data.AddString(sAddMsg.c_str(), RGB(255,0,0));
	m_list_data.SetCurSel(nIdx);
}

//==================================================================//
// [->리스트컨트롤] 통신 전송 메시지 추가
//==================================================================//
void CDialog_List::OnAdd_CommSendMsg(int nzPort) 
{
	SYSTEMTIME systime;
	GetLocalTime(&systime);
	
	CString sTime;
	sTime.Format("Snd  Com%02d : [%02d:%02d:%02d.%03d] ",
		nzPort,
		systime.wHour,
		systime.wMinute,
		systime.wSecond,
		systime.wMilliseconds);

	// 전송한 메시지 함수 변수에 할당
	CString sTempMsg = st_serial.str_snd[nzPort -1];

	// 일정 시간 지난 경우에만 리스트에 내용 추가함
	if (m_sOldMsgCommSend == sTempMsg)
	{
		// 메시지가 중간에 변질되어 계속 동일한 문자 출력되는 것 방지
		m_lTimeCommSend[1] = GetCurrentTime();
		m_lTimeCommSend[2] = m_lTimeCommSend[1] - m_lTimeCommSend[0];
		if (m_lTimeCommSend[2] > MSG_TERM_)
		{
			// 내용 저장
		}
		else
		{
			m_lTimeCommSend[0] = GetCurrentTime();
			return;
		}
	}

	std::string sAddMsg = "";
	if (sTempMsg != _T(""))
	{
		m_lTimeCommSend[0] = GetCurrentTime();

		m_sOldMsgCommSend = sTempMsg;
		sAddMsg = sTime + sTempMsg;
	}
	else
	{
		sAddMsg = "Display Message is Empty";
	}
	
	int nIdx = m_list_data.AddString(sAddMsg.c_str(), RGB(25,25,85));
	m_list_data.SetCurSel(nIdx);
}

//==================================================================//
// [->리스트컨트롤] 통신 수신 메시지 추가
//==================================================================//
void CDialog_List::OnAdd_CommRcvMsg(int nzPort) 
{
	SYSTEMTIME systime;
	GetLocalTime(&systime);
	
	CString sTime;
	sTime.Format("Rcv  Com%02d : [%02d:%02d:%02d.%03d] ",
		nzPort,
		systime.wHour,
		systime.wMinute,
		systime.wSecond,
		systime.wMilliseconds);
	
	// 전송한 메시지 함수 변수에 할당
	// Kimyangkyu
	//CString sTempMsg = st_serial.str_rec[nzPort -1];

	
	// 일정 시간 지난 경우에만 리스트에 내용 추가함
	//if (m_sOldMsgCommRcv == sTempMsg)
	if (m_sOldMsgCommRcv == st_other.str_op_msg)
	{
		// 메시지가 중간에 변질되어 계속 동일한 문자 출력되는 것 방지
		m_lTimeCommRcv[1] = GetCurrentTime();
		m_lTimeCommRcv[2] = m_lTimeCommRcv[1] - m_lTimeCommRcv[0];
		if (m_lTimeCommRcv[2] > MSG_TERM_)
		{
			// 내용 저장
		}
		else
		{
			m_lTimeCommRcv[0] = GetCurrentTime();
			return;
		}
	}
	
	std::string sAddMsg = "";
	if (st_other.str_op_msg != _T(""))
	{
		m_lTimeCommRcv[0] = GetCurrentTime();
		
		m_sOldMsgCommRcv = st_other.str_op_msg;
		sAddMsg = sTime + st_other.str_op_msg;
	}
	else
	{
		sAddMsg = "Display Message is Empty";
	}
	
	int nIdx = m_list_data.AddString(sAddMsg.c_str(), RGB(50,100,150));
	m_list_data.SetCurSel(nIdx);
}

//==================================================================//
// 디바이스명 및 EQPID 정보 화면 출력
//==================================================================//
void CDialog_List::OnDisp_EqpInfo() 
{
	m_msg_eqp_id.SetWindowText(_T(st_basic.sEqpNo));		// 장비 호기
	m_msg_device_name.SetWindowText(_T(st_basic.sDvcName));	// 디바이스명
}
