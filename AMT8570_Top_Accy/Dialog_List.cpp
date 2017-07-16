// Dialog_List.cpp : implementation file
//

#include "stdafx.h"
#include "Handler.h"
#include "Dialog_List.h"

//#include <atlconv.h>	// ���� �ڵ� ��Ʈ�� ��ȯ
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
	ON_MESSAGE(WM_LIST_DATA, OnListDataUpdate)	// ����Ʈ ��Ʈ�ѿ� ������ ���
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialog_List message handlers

BOOL CDialog_List::OnInitDialogBar() 
{
	CInitDialogBar::OnInitDialogBar();

	// ��Ʈ�ѿ� ����� ��Ʈ ���� ����
	mp_fontList = NULL;
	mp_fontList = new CFont;
	mp_fontList->CreateFont(14,6,0,0,900,0,0,0,0,0,0,PROOF_QUALITY,0,"MS Sans Serif");

	OnSet_LblCtrl();	// Į�� �� ����
	OnSet_ClockCtrl();	// ������ �ð� ��Ʈ�� ����
	OnSet_TabStops(TRUE);	// ����Ʈ ��Ʈ�ѿ� Tab Stop ��� ����

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialog_List::OnDestroy() 
{
	// ������ ��Ʈ ����
	delete mp_fontList;
	mp_fontList = NULL;

	if (st_handler.nMenuLock != FALSE)
	{
		st_handler.nMenuLock = FALSE;  // �޴� ��� ����
	}

	CInitDialogBar::OnDestroy();
}

void CDialog_List::OnDblclkListData() 
{
	
}

//==================================================================//
// ����Ʈ ��Ʈ�ѿ� �޽��� �߰��϶�� ��û
//==================================================================//
LRESULT CDialog_List::OnListDataUpdate(WPARAM wParam, LPARAM lParam)
{
	CSingleLock mute(&logmutex);
	mute.Lock();

	int nzPort = (int)wParam;  // ��� ��Ʈ ��ȣ �Ҵ�
	OnReset_ListContent();	// ����Ʈ ��Ʈ�ѿ� ��µ� ���ڿ� ���� [500�� ����]

	switch(lParam)
	{
	case RECEIVE_MSG:
		OnAdd_CommRcvMsg(nzPort);	// [->����Ʈ��Ʈ��] ��� ���� �޽��� �߰�
		break;

	case SEND_MSG:
		OnAdd_CommSendMsg(nzPort);	// [->����Ʈ��Ʈ��] ��� ���� �޽��� �߰�
		break;

	case NORMAL_MSG:
		OnAdd_NormalMsg();	// [->����Ʈ��Ʈ��] �������� �޽��� �߰�
		break;

	case ABNORMAL_MSG:
		OnAdd_AbnormalMsg();	// [->����Ʈ��Ʈ��] ���������� �޽��� �߰�
		break;
		
	case MACHINE_INFO:
		OnDisp_EqpInfo();	// ����̽��� �� EQPID ���� ȭ�� ���
		break;

	case DEVICE_INFO:
		OnDisp_EqpInfo();	// ����̽��� �� EQPID ���� ȭ�� ���
		break;
	}
	
	mute.Unlock();

	return 0;
}

//==================================================================//
// Į�� �� ����
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
// ������ �ð� ��Ʈ�� ����
//==================================================================//
void CDialog_List::OnSet_ClockCtrl() 
{
	m_list_clock.Start(IDB_CLOCKST_PANE, IDB_CLOCKST_BIG, IDB_CLOCKST_SMALL);
}

//==================================================================//
// ����Ʈ ��Ʈ�ѿ� Tab Stop ��� ����
//==================================================================//
void CDialog_List::OnSet_TabStops(int nzTabStops) 
{
	DWORD dwRemove	= (nzTabStops ? 0:LBS_USETABSTOPS);
	DWORD dwAdd		= (nzTabStops ? LBS_USETABSTOPS:0);

	m_list_data.ModifyStyle(dwRemove, dwAdd);
	m_list_data.RedrawWindow();
}

//==================================================================//
// ����Ʈ ��Ʈ�ѿ� ��µ� ���ڿ� ���� [500�� ����]
//==================================================================//
void CDialog_List::OnReset_ListContent() 
{
	int nIdx = m_list_data.GetCount();
	if (nIdx < 500)	{
		return;
	}
	// ����Ʈ ��Ʈ�ѿ� �߰��� ��� ���ڿ� ����
	m_list_data.ResetContent();
}

//==================================================================//
// [->����Ʈ��Ʈ��] �������� �޽��� �߰�
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

	// ���� �ð� ���� ��쿡�� ����Ʈ�� ���� �߰���
	if (m_sOldMsgNormal == st_msg.c_normal_msg)
	{
		// �޽����� �߰��� �����Ǿ� ��� ������ ���� ��µǴ� �� ����
		m_lTimeNormal[1] = GetCurrentTime();
		m_lTimeNormal[2] = m_lTimeNormal[1] - m_lTimeNormal[0];
		if (m_lTimeNormal[2] > MSG_TERM_ / 4)
		{
			// ���� ����
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
// [->����Ʈ��Ʈ��] ���������� �޽��� �߰�
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

	// ���� �ð� ���� ��쿡�� ����Ʈ�� ���� �߰���
	if (mm_sOldMsgAbnormal == st_msg.c_abnormal_msg)
	{
		// �޽����� �߰��� �����Ǿ� ��� ������ ���� ��µǴ� �� ����
		m_lTimeAbnormal[1] = GetCurrentTime();
		m_lTimeAbnormal[2] = m_lTimeAbnormal[1] - m_lTimeAbnormal[0];
		if (m_lTimeAbnormal[2] > MSG_TERM_)
		{
			// ���� ����
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
// [->����Ʈ��Ʈ��] ��� ���� �޽��� �߰�
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

	// ������ �޽��� �Լ� ������ �Ҵ�
	CString sTempMsg = st_serial.str_snd[nzPort -1];

	// ���� �ð� ���� ��쿡�� ����Ʈ�� ���� �߰���
	if (m_sOldMsgCommSend == sTempMsg)
	{
		// �޽����� �߰��� �����Ǿ� ��� ������ ���� ��µǴ� �� ����
		m_lTimeCommSend[1] = GetCurrentTime();
		m_lTimeCommSend[2] = m_lTimeCommSend[1] - m_lTimeCommSend[0];
		if (m_lTimeCommSend[2] > MSG_TERM_)
		{
			// ���� ����
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
// [->����Ʈ��Ʈ��] ��� ���� �޽��� �߰�
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
	
	// ������ �޽��� �Լ� ������ �Ҵ�
	// Kimyangkyu
	//CString sTempMsg = st_serial.str_rec[nzPort -1];

	
	// ���� �ð� ���� ��쿡�� ����Ʈ�� ���� �߰���
	//if (m_sOldMsgCommRcv == sTempMsg)
	if (m_sOldMsgCommRcv == st_other.str_op_msg)
	{
		// �޽����� �߰��� �����Ǿ� ��� ������ ���� ��µǴ� �� ����
		m_lTimeCommRcv[1] = GetCurrentTime();
		m_lTimeCommRcv[2] = m_lTimeCommRcv[1] - m_lTimeCommRcv[0];
		if (m_lTimeCommRcv[2] > MSG_TERM_)
		{
			// ���� ����
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
// ����̽��� �� EQPID ���� ȭ�� ���
//==================================================================//
void CDialog_List::OnDisp_EqpInfo() 
{
	m_msg_eqp_id.SetWindowText(_T(st_basic.sEqpNo));		// ��� ȣ��
	m_msg_device_name.SetWindowText(_T(st_basic.sDvcName));	// ����̽���
}
