// HandlerDoc.cpp : implementation of the CHandlerDoc class
//

#include "stdafx.h"
#include "Handler.h"

#include "HandlerDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHandlerDoc

IMPLEMENT_DYNCREATE(CHandlerDoc, CDocument)

BEGIN_MESSAGE_MAP(CHandlerDoc, CDocument)
	//{{AFX_MSG_MAP(CHandlerDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHandlerDoc construction/destruction

CHandlerDoc::CHandlerDoc()
{
	// TODO: add one-time construction code here

}

CHandlerDoc::~CHandlerDoc()
{
}

BOOL CHandlerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CHandlerDoc serialization

void CHandlerDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CHandlerDoc diagnostics

#ifdef _DEBUG
void CHandlerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CHandlerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CHandlerDoc commands
