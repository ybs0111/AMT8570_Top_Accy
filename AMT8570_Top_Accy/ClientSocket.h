#if !defined(AFX_CLIENTSOCKET_H__2A49E7C6_67E3_11D3_8401_006097663D30__INCLUDED_)
#define AFX_CLIENTSOCKET_H__2A49E7C6_67E3_11D3_8401_006097663D30__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ClientSocket.h : header file
//

// 규이리 추가 [2014.07.25]
#include <afxSock.h>

#define MAX_RECEIVE_LEN		40960
// 규이리 수정 [2014.07.26]
//#define MAX_RECEIVE_LEN		131072

/////////////////////////////////////////////////////////////////////////////
// CClientSocket command target

class CClientSocket : public CSocket
{
// Operations
public:
	CClientSocket();
	virtual ~CClientSocket();

	BOOL Create(int n_address, CString str_ip, int n_port, int n_timeout);
	
// Attributes
private:
	
// Attributes
public:
	BYTE	m_by_rev[1024];	// 수신 버퍼
	int		m_n_rev_length;	// 수신 데이터 길이
	int     m_n_address;	// 주소값 저장

// Operations
public:
	BOOL IsConnect();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CClientSocket)
	public:
	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	virtual void OnAccept(int nErrorCode);
	virtual void OnConnect(int nErrorCode);
	virtual int Receive(void* lpBuf, int nBufLen, int nFlags = 0);
	virtual BOOL ConnectHelper(const SOCKADDR* lpSockAddr, int nSockAddrLen);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CClientSocket)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLIENTSOCKET_H__2A49E7C6_67E3_11D3_8401_006097663D30__INCLUDED_)
