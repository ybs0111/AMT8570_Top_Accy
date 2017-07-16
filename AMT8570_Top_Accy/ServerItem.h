// 규이리 수정 [2014.07.28]
/*
#if !defined(AFX_CLIENTSOCKET_H__2A49E7C8_67E3_11D3_8401_006097663D30__INCLUDED_)
#define AFX_CLIENTSOCKET_H__2A49E7C8_67E3_11D3_8401_006097663D30__INCLUDED_
*/
#if !defined(AFX_SEVERITEM_H__2A49E7C8_67E3_11D3_8401_006097663D30__INCLUDED_)
#define AFX_SEVERITEM_H__2A49E7C8_67E3_11D3_8401_006097663D30__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ServerItem.h : header file
//

// 규이리 추가 [2014.07.28]
#include <afxsock.h>

/////////////////////////////////////////////////////////////////////////////
// CServerItem command target

class CServerItem : public CSocket
{
// Operations
public:
	CServerItem(CSocket* pServerSocket);
	virtual ~CServerItem();
	
// Attributes
private:
	
// Attributes
public:
	int		m_n_rev_length;	// 수신 데이터 길이
	
	CSocket* m_ServerSocket;	// 서버 소켓
	
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CServerItem)
	public:
	virtual void OnClose(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CServerItem)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

// 규이리 수정 [2014.07.28]
//#endif // !defined(AFX_CLIENTSOCKET_H__2A49E7C8_67E3_11D3_8401_006097663D30__INCLUDED_)
#endif // !defined(AFX_SEVERITEM_H__2A49E7C8_67E3_11D3_8401_006097663D30__INCLUDED_)
