// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__00C7965C_CE5F_4297_84D7_069EC7B64F04__INCLUDED_)
#define AFX_STDAFX_H__00C7965C_CE5F_4297_84D7_069EC7B64F04__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#pragma warning(disable:4786)
#pragma warning(disable:4652)
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxsock.h>		// MFC socket extensions
#include <afxtempl.h>		// Tool Tip용 ViboX 추가.
#include <afxdlgs.h>		// Tips Ware용...

#include "./lib/Criticalsection.h"
#include "./lib/MultiThreadSync.h"
#include "./lib/CircularQueue.h"

#include "afxmt.h"		// Mutex header
#include <afxcmn.h>			// MFC support for Windows Common Controls

/* ****************************************************************************** */
/* 대화 상자에 대한 OK, Cancel 버튼 생성 함수                                     */
/* ****************************************************************************** */
#include "tw_step.h"  // 대화 상자 생성 클래스 추가 
#include "resource.h"       // main symbols
#include "ssdllmfc.h"
#include "GridControlAlg.h"
#include "FileManagement.h"
#include "Public_Function.h"
#include "CtlBd_Library.h"
#include "ComizoaPublic.h"

#include "IO_Manager.h"
#include "COMI_Manager.h"

extern CButton *CreateOkButton(int parm_resource_id, CWnd *parm_parent);
extern CButton *CreateCancelButton(int parm_resource_id, CWnd *parm_parent);
extern CButton *CreateSaveOkButton(int parm_resource_id, CWnd *parm_parent);
extern CButton *CreateExitButton(int parm_resource_id, CWnd *parm_parent);

extern CFileManagement				FileAccess;
extern CPublic_Function				Func;
extern CMainFrame					cmain;
/* ****************************************************************************** */
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__00C7965C_CE5F_4297_84D7_069EC7B64F04__INCLUDED_)
