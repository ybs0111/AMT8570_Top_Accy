// Handler.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Handler.h"

#include "MainFrm.h"
#include "HandlerDoc.h"
#include "HandlerView.h"

#include "Screen_Main.h"  // 초기에 메인 화면을 출력하기 위하여 추가 

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHandlerApp

BEGIN_MESSAGE_MAP(CHandlerApp, CWinApp)
	//{{AFX_MSG_MAP(CHandlerApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHandlerApp construction

CHandlerApp::CHandlerApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CHandlerApp object

CHandlerApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CHandlerApp initialization

BOOL CHandlerApp::InitInstance()
{
	// SingleInstance 사용시 이부분이 없으면 비어있는 문서를 만드는데 실패 했다고 뜬다. 2K11/02/18/ViboX
	if (!CheckSingleInstance(IDR_MAINFRAME))	// 생성할 응용 프로그램이 이미 존재하는지 검사..Need for SingleInstance .. 
		return FALSE;

	if (!AfxSocketInit())
	{
		//AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		if (st_handler.mn_language == LANGUAGE_ENGLISH)
		{
			Func.MsgLog("Failed to initialize Windows Sockets.");	// 20130926 kjh english
		}
		else
		{
			Func.MsgLog("Windows 소켓 초기화에 실패하였습니다.");
		}
		return FALSE;
	}

	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// yangkyu [2015.04.03]
	// 언어 부분이 빠져있어 추가함.
	int nLanguage;
	
	nLanguage = GetACP();
	if(nLanguage == 949)
	{
		st_handler.mn_language = LANGUAGE_KOREA;
	}
	else if(nLanguage == 1252)
	{
		st_handler.mn_language = LANGUAGE_ENGLISH;
	}
	else
	{
		st_handler.mn_language = LANGUAGE_KOREA;
	}
	
	// kilee [2015.03.01]
	TAPI_LoadSystemButtonImage();

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_EMPTY,
		RUNTIME_CLASS(CHandlerDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
//		RUNTIME_CLASS(CHandlerView));
		RUNTIME_CLASS(CScreen_Main));
	AddDocTemplate(pDocTemplate);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it.
	m_pMainWnd->ShowWindow(SW_SHOWMAXIMIZED);
	m_pMainWnd->UpdateWindow();

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CHandlerApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CHandlerApp message handlers


int CHandlerApp::ExitInstance() 
{
	// KILEE [2015.03.01]
	TAPI_DeleteSystemButtonImage();
	
	return CWinApp::ExitInstance();
}
