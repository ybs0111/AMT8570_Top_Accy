// ServerItem.cpp : implementation file
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
// CServerItem

CServerItem::CServerItem(CSocket* pServerSocket)
{
	m_ServerSocket = pServerSocket;
}

CServerItem::~CServerItem()
{

}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CServerItem, CSocket)
	//{{AFX_MSG_MAP(CServerItem)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CServerItem member functions

// 소켓 종료하기
void CServerItem::OnClose(int nErrorCode) 
{
	CServerSocket* ServerSocket = (CServerSocket*)m_ServerSocket;

	// 소켓 해제
	ServerSocket->CloseClient(this);
	// 규이리 위치 이동시킴 [2013.11.12]
	// 메인 프레임으로 메시지 전송하여 소켓을 종료시킴
	::PostMessage(st_handler.hWnd, WM_SERVER_MSG + ServerSocket->m_n_address, SERVER_CLOSE, ServerSocket->m_n_address);

	CSocket::OnClose(nErrorCode);

	// 통지 : SCM_CLOSE
	//::PostMessage(st_handler.hWnd, WM_SERVER_MSG, SERVER_CLOSE, ServerSocket->m_n_address);
}

// 데이터 수신 처리
void CServerItem::OnReceive(int nErrorCode) 
{
	CServerSocket* ServerSocket = (CServerSocket*)m_ServerSocket;

	// 수신된 데이터 읽기
	//st_server[ServerSocket->m_n_address].n_rev_length = Receive(st_server[ServerSocket->m_n_address].ch_rev, 1024);
	// 규이리 수정 [2014.08.11]
	st_server[ServerSocket->m_n_address].n_rev_length = Receive(st_server[ServerSocket->m_n_address].ch_rev, 2048);
	st_server[ServerSocket->m_n_address].ch_rev[st_server[ServerSocket->m_n_address].n_rev_length+1] = '\r';

	// 메인 프레임으로 수신 메시지 전송하여 처리함
	::PostMessage(st_handler.hWnd, WM_SERVER_MSG + ServerSocket->m_n_address, SERVER_REV, ServerSocket->m_n_address);

	CSocket::OnReceive(nErrorCode);
}
