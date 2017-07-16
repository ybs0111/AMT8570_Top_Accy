// ServerSocket.cpp : implementation file
//
// 이현철
// forcewin@popsmail.com

#include "stdafx.h"
#include "ServerSocket.h"

#include "Variable.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CServerSocket

CServerSocket::CServerSocket()
{

}

CServerSocket::~CServerSocket()
{
	CServerItem *ServerItem;

	// 연결되어 있는 클라이언트 갯수만큼 반복
	while(m_listClient.GetCount())
	{
		// 연결된 서버 객체 닫기
		ServerItem = (CServerItem*)m_listClient.GetHead();
		ServerItem->Close();
		// 서버와 연결이 종료된 클라이언트 정보 삭제
		m_listClient.RemoveHead();
		delete ServerItem;
	}
}

// 서버 소켓 생성
BOOL CServerSocket::Create(int n_address, int n_port) 
{
	if (!CSocket::Create(n_port, SOCK_STREAM))
	{
		// 규이리 주석 처리 [2013.11.12]
		//AfxMessageBox("Error : Create() in socket");
		return FALSE;
	}

	if (!CSocket::Listen())
	{
		// 규이리 주석 처리 [2013.11.12]
		//AfxMessageBox("Error : Listen() in socket");
		return FALSE;
	}

	m_n_address = n_address;

	return TRUE;
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CServerSocket, CSocket)
	//{{AFX_MSG_MAP(CServerSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CServerSocket member functions

// [서버<->클라이언트] 소켓 연결
void CServerSocket::OnAccept(int nErrorCode) 
{
	CServerItem* ServerItem = new CServerItem(this);

	if (CSocket::Accept(*ServerItem))
	{
		m_listClient.AddTail(ServerItem);
	}
	else
	{
		delete ServerItem;
	}
	
	CSocket::OnAccept(nErrorCode);
}

// 클라이언트 소켓 닫기
void CServerSocket::CloseClient(CSocket* ServerItem)
{
	POSITION pos = m_listClient.Find(ServerItem);
	if (pos != NULL)
	{
		ServerItem->Close();
		m_listClient.RemoveAt(pos);
		delete ServerItem;
	}
}

// 모든 클라이언트에 데이터 전송
void CServerSocket::SendClientAll(const void *lpBuf, int nBufLen, int nFlags)
{
	CServerItem *ServerItem;
	POSITION pos = m_listClient.GetHeadPosition();

	while(pos != NULL)
	{
		ServerItem = (CServerItem*)m_listClient.GetNext(pos);
		ServerItem->Send(lpBuf, nBufLen, nFlags);
	}
}

// 특정 Client에 데이터 전송
int CServerSocket::SendClientAt(CSocket* ServerItem, const void *lpBuf, int nBufLen, int nFlags)
{
	return ServerItem->Send(lpBuf, nBufLen, nFlags);
}

// 접속된 Client 개수
int CServerSocket::GetClientCount()
{
	return m_listClient.GetCount();
}

// Client Object
CSocket* CServerSocket::GetClient(int nIndex)
{
	return (CSocket*)m_listClient.GetAt(m_listClient.FindIndex(nIndex));
}

void CServerSocket::OnReceive(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CSocket::OnReceive(nErrorCode);
}

void CServerSocket::OnClose(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CSocket::OnClose(nErrorCode);
}
