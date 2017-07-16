// Dialog_Smema.cpp : implementation file
//

#include "stdafx.h"
#include "handler.h"
#include "Dialog_Smema.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialog_Smema dialog


CDialog_Smema::CDialog_Smema(CWnd* pParent /*=NULL*/)
	: StandardDialog(CDialog_Smema::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialog_Smema)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDialog_Smema::DoDataExchange(CDataExchange* pDX)
{
	StandardDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialog_Smema)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialog_Smema, StandardDialog)
	//{{AFX_MSG_MAP(CDialog_Smema)
	ON_BN_CLICKED(IDC_BUTTON1, OnRearReadySignalON)
	ON_BN_CLICKED(IDC_BUTTON10, OnRearReadySignalOFF)
	ON_BN_CLICKED(IDOK, OnClose)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialog_Smema message handlers

void CDialog_Smema::OnClose() 
{
	// TODO: Add extra validation here
	
	DestroyWindow();
}

void CDialog_Smema::OnRearReadySignalON() 
{
	g_ioMgr.set_out_bit(stIO.o_Smema_Ready2Main, IO_ON);
}

void CDialog_Smema::OnRearReadySignalOFF() 
{
	g_ioMgr.set_out_bit(stIO.o_Smema_Ready2Main, IO_OFF);
}

BOOL CDialog_Smema::Create(CWnd* pParentWnd)
{
	// TODO: Add your specialized code here and/or call the base class
	
	return StandardDialog::Create(IDD, pParentWnd);
}

void CDialog_Smema::PostNcDestroy() 
{
	((CMainFrame*)AfxGetMainWnd())->mp_cSmema = NULL;  // 대화상자 핸들 초기화
	delete this;
	
	StandardDialog::PostNcDestroy();
}

BOOL CDialog_Smema::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return StandardDialog::PreTranslateMessage(pMsg);
}
