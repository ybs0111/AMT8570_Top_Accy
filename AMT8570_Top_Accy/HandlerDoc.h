// HandlerDoc.h : interface of the CHandlerDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_HANDLERDOC_H__AD50D593_586E_437A_B007_13740B5A1189__INCLUDED_)
#define AFX_HANDLERDOC_H__AD50D593_586E_437A_B007_13740B5A1189__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CHandlerDoc : public CDocument
{
protected: // create from serialization only
	CHandlerDoc();
	DECLARE_DYNCREATE(CHandlerDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHandlerDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CHandlerDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CHandlerDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HANDLERDOC_H__AD50D593_586E_437A_B007_13740B5A1189__INCLUDED_)
