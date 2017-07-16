// ServerItem.cpp : implementation file
//
// ����ö
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

// ���� �����ϱ�
void CServerItem::OnClose(int nErrorCode) 
{
	CServerSocket* ServerSocket = (CServerSocket*)m_ServerSocket;

	// ���� ����
	ServerSocket->CloseClient(this);
	// ���̸� ��ġ �̵���Ŵ [2013.11.12]
	// ���� ���������� �޽��� �����Ͽ� ������ �����Ŵ
	::PostMessage(st_handler.hWnd, WM_SERVER_MSG + ServerSocket->m_n_address, SERVER_CLOSE, ServerSocket->m_n_address);

	CSocket::OnClose(nErrorCode);

	// ���� : SCM_CLOSE
	//::PostMessage(st_handler.hWnd, WM_SERVER_MSG, SERVER_CLOSE, ServerSocket->m_n_address);
}

// ������ ���� ó��
void CServerItem::OnReceive(int nErrorCode) 
{
	CServerSocket* ServerSocket = (CServerSocket*)m_ServerSocket;

	// ���ŵ� ������ �б�
	//st_server[ServerSocket->m_n_address].n_rev_length = Receive(st_server[ServerSocket->m_n_address].ch_rev, 1024);
	// ���̸� ���� [2014.08.11]
	st_server[ServerSocket->m_n_address].n_rev_length = Receive(st_server[ServerSocket->m_n_address].ch_rev, 2048);
	st_server[ServerSocket->m_n_address].ch_rev[st_server[ServerSocket->m_n_address].n_rev_length+1] = '\r';

	// ���� ���������� ���� �޽��� �����Ͽ� ó����
	::PostMessage(st_handler.hWnd, WM_SERVER_MSG + ServerSocket->m_n_address, SERVER_REV, ServerSocket->m_n_address);

	CSocket::OnReceive(nErrorCode);
}
