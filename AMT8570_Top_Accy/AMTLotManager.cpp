// AMTLotManager.cpp: implementation of the AMTLotManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "handler.h"
#include "AMTLotManager.h"
#include "Public_Function.h"

#include "CtlBd_Function.h"
#include "CtlBd_Library.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

AMTLotManager g_lotMgr;
ALot::ALot()
{
	
}

ALot::ALot( CString strLotID, CString strPartID )
{
	m_strLotID = strLotID; 
	m_strPartID = strPartID;
}

ALot::~ALot()
{
	
}

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =			
AMTLotManager::AMTLotManager()
{
	
}

AMTLotManager::~AMTLotManager()
{
	
}

void AMTLotManager::OnNewLotIn(CString strLotID, CString strPartID )
{
	int nLotCnt = m_vecLot.size();

	for( int i=0; i<nLotCnt; i++ )
	{
		if( m_vecLot.at(i).GetLotID() == strLotID )
		{
			//"JAMCODE"
			return;
		}
	}
	
	ALot lot( strLotID, strPartID );
	
	m_vecLot.push_back( lot );
	
	// ȭ�� ���� (main)
	if( st_handler.cwnd_main )
		st_handler.cwnd_main->PostMessage( WM_DRAW_DATA_MAIN, EDDM_LOTID );
}

CString AMTLotManager::GetLotIDAt(int iIdx )
{
	if( iIdx >= m_vecLot.size() )	return "";
	
	return m_vecLot.at(iIdx).GetLotID();
}

ALot& AMTLotManager::GetLotAt( int iIdx )
{
	if( iIdx < 0 ) return m_dummyLot;
	
	return m_vecLot.at( iIdx );
}

ALot& AMTLotManager::GetLotByLotID( CString strLotID )
{
	int nCnt = m_vecLot.size();
	for( int i=0; i< nCnt; i++ )
	{
		if( m_vecLot.at( i ).GetLotID() == strLotID )
		{
			return m_vecLot.at( i );
		}
	}
	
	return m_dummyLot;
}

void AMTLotManager::DeleteLot(int iIdx)					// Index�� Lot �����. DeleteLot( 0 )�� �ϸ� �� ó���� �߰� ��Ų Lot�� ���� �ȴ�.
{
	if( iIdx < 0 || m_vecLot.size() <= iIdx )
		return;
	
	std::vector<ALot>::iterator it =m_vecLot.begin();
	it += iIdx;
	
	m_vecLot.erase(it);
	
	if( st_handler.cwnd_main )
		st_handler.cwnd_main->PostMessage( WM_DRAW_DATA_MAIN, EDDM_LOTID );
}

void AMTLotManager::DeleteLot(CString strLotID)			// Lot ID�� Lot �����. DeleteLot( "LOTID" )�� �ϸ� Index�� ã�Ƽ� �ش� Lot�� ���� �Ѵ�.
{
	int nCnt = m_vecLot.size();
	
	if( strLotID == "" || nCnt <= 0 )
		return;
	
	for( int i=0; i< nCnt; i++ )
	{
		if( m_vecLot.at( i ).GetLotID() == strLotID )
		{
			std::vector<ALot>::iterator it =m_vecLot.begin();
			it += i;
			
			m_vecLot.erase(it);
			
			if( st_handler.cwnd_main )
				st_handler.cwnd_main->PostMessage( WM_DRAW_DATA_MAIN, EDDM_LOTID );
			break;
		}
	}
}