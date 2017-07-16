// ABpcManager.cpp: implementation of the ABpcManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "handler.h"
#include "ABpcManager.h"
#include "CtlBd_Function.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


ABpcManager g_bpc;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ABpcManager::ABpcManager()
{
	Initialize();
}

ABpcManager::~ABpcManager()
{

}

void ABpcManager::Initialize()
{
	m_iTrackInResult = TRACKIN_RES_PASS;
}

void ABpcManager::SetTrackInResult( int iRes )
{
	if(m_iTrackInResult == iRes )
		return;

	m_iTrackInResult = iRes; 

	if( m_iTrackInResult == TRACKIN_RES_ERR_MSS )
	{
//		CtlBdFunc.OnAlarmStart( "200500" );
	}
}