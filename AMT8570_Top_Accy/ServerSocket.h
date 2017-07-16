#if !defined(AFX_SERVERSOCKET_H__2A49E7C7_67E3_11D3_8401_006097663D30__INCLUDED_)
#define AFX_SERVERSOCKET_H__2A49E7C7_67E3_11D3_8401_006097663D30__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ServerSocket.h : header file
//

#include <afxmt.h>

#include "ServerItem.h"

// 규이리 수정 [2014.07.28]
#define MAX_RECEIVE_LEN		40960
//#define MAX_RECEIVE_LEN		131072

/////////////////////////////////////////////////////////////////////////////
// CServerSocket command target

class CServerSocket : public CSocket
{
// Operations
public:
	CServerSocket();
	virtual ~CServerSocket();
	
	// 서버 소켓 생성
	BOOL Create(int n_address, int n_port);
	
// Attributes
private:
	
// Attributes
public:
	CCriticalSection cs;
	
// Operations
public:
	int GetClientCount();	// 접속된 클라이언트 갯수 반환
	CSocket* GetClient(int nIndex);	// Client Object
	
	// 모든 클라이언트에 데이터 전송
	void SendClientAll(const void* lpBuf, int nBufLen, int nFlags=0);
	// 특정한 클라이언트에 데이터 전송
	int  SendClientAt(CSocket* ServerItem, const void* lpBuf, int nBufLen, int nFlags=0);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CServerSocket)
	public:
	virtual void OnAccept(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CServerSocket)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
	// 클라이언트 소켓 해제
	void CloseClient(CSocket* ServerItem);
	
	int m_n_address;
	
	CPtrList m_listClient;	// 클라이언트 소켓 리스트 목록
	
	friend CServerItem;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERVERSOCKET_H__2A49E7C7_67E3_11D3_8401_006097663D30__INCLUDED_)
