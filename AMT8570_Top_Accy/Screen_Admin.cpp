// Screen_Admin.cpp : implementation file
//

#include "stdafx.h"
#include "handler.h"
#include "Screen_Admin.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScreen_Admin

IMPLEMENT_DYNCREATE(CScreen_Admin, CFormView)

CScreen_Admin::CScreen_Admin()
	: CFormView(CScreen_Admin::IDD)
{
	//{{AFX_DATA_INIT(CScreen_Admin)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CScreen_Admin::~CScreen_Admin()
{
}

void CScreen_Admin::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CScreen_Admin)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CScreen_Admin, CFormView)
	//{{AFX_MSG_MAP(CScreen_Admin)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScreen_Admin diagnostics

#ifdef _DEBUG
void CScreen_Admin::AssertValid() const
{
	CFormView::AssertValid();
}

void CScreen_Admin::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CScreen_Admin message handlers

void CScreen_Admin::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	
}

void CScreen_Admin::OnDestroy() 
{
	CFormView::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

void CScreen_Admin::OnButton1() 
{
	m_sLogMsg.Format( "[RCV]BCR:");
	cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
/*
	CString csPath = "\\\\10.31.47.18\\TestData\\aaaaaa\\";

	CString csPrefix(_T("")), csToken(_T(""));
    int nStart = 0, nEnd;
    while( (nEnd = csPath.Find('/', nStart)) >= 0)
    {
        CString csToken = csPath.Mid(nStart, nEnd-nStart);
        CreateDirectory(csPrefix + csToken, NULL);
		
        csPrefix += csToken;
		csPrefix += _T("/");
		nStart = nEnd+1;
	} 
	csToken = csPath.Mid(nStart);
	CreateDirectory(csPrefix + csToken, NULL);*/
}

void CScreen_Admin::OnButton2() 
{
	st_handler.nStart_INIT = CTL_YES;
	st_handler.mn_status = CTL_YES;

	alarm.mstr_code		= "001002";
	
	alarm.mn_count_mode	= 0;
	alarm.mn_type_mode	= eWARNING;
	st_work.nEqpStatus	= dWARNING;	
}
