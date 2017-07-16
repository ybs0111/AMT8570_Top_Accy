// Dialog_Network.cpp : implementation file
//

#include "stdafx.h"
#include "handler.h"
#include "Dialog_Network.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialog_Network dialog


CDialog_Network::CDialog_Network(CWnd* pParent /*=NULL*/)
	: StandardDialog(CDialog_Network::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialog_Network)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDialog_Network::DoDataExchange(CDataExchange* pDX)
{
	StandardDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialog_Network)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialog_Network, StandardDialog)
	//{{AFX_MSG_MAP(CDialog_Network)
	ON_BN_CLICKED(IDC_BUTTON1, OnSUPPLU_COMPLETE)
	ON_BN_CLICKED(IDC_BUTTON3, OnSUPPLY_TYPE)
	ON_BN_CLICKED(IDC_BUTTON2, OnALARM)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDOK, OnClose)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialog_Network message handlers

BOOL CDialog_Network::Create(CWnd* pParentWnd)
{
	// TODO: Add your specialized code here and/or call the base class
	
	return StandardDialog::Create(IDD, pParentWnd);
}

void CDialog_Network::PostNcDestroy() 
{
	((CMainFrame*)AfxGetMainWnd())->mp_cNetwork = NULL;  // 대화상자 핸들 초기화
	delete this;
	
	StandardDialog::PostNcDestroy();
}

BOOL CDialog_Network::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return StandardDialog::PreTranslateMessage(pMsg);
}

void CDialog_Network::OnDestroy() 
{
	StandardDialog::OnDestroy();
}

void CDialog_Network::OnClose() 
{	
	DestroyWindow();
}

void CDialog_Network::OnSUPPLU_COMPLETE() 
{
	g_client_accy.OnCreate_SendFormat(MAIN_ACCY_SUPPLY_COMPLETE);
	g_client_accy.OnReq_CommStart();
}

void CDialog_Network::OnSUPPLY_TYPE() 
{
	g_client_accy.OnCreate_SendFormat(MAIN_SUPPLY_TYPE);
	g_client_accy.OnReq_CommStart();
}

void CDialog_Network::OnALARM() 
{
	g_client_accy.OnCreate_SendFormat(MAIN_ALARM);
	g_client_accy.OnReq_CommStart();
}
