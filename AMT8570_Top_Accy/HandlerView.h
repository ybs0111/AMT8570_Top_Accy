// HandlerView.h : interface of the CHandlerView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_HANDLERVIEW_H__072D5D53_2E15_4B56_B816_021DFD359D1B__INCLUDED_)
#define AFX_HANDLERVIEW_H__072D5D53_2E15_4B56_B816_021DFD359D1B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CHandlerView : public CFormView
{
protected: // create from serialization only
	CHandlerView();
	DECLARE_DYNCREATE(CHandlerView)

public:
	//{{AFX_DATA(CHandlerView)
	enum{ IDD = IDD_HANDLER_FORM };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:
	CHandlerDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHandlerView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CHandlerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CHandlerView)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in HandlerView.cpp
inline CHandlerDoc* CHandlerView::GetDocument()
   { return (CHandlerDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HANDLERVIEW_H__072D5D53_2E15_4B56_B816_021DFD359D1B__INCLUDED_)
