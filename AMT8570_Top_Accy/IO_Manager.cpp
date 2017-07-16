// IO_Manager.cpp: implementation of the IO_Manager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "handler.h"
#include "IO_Manager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include "..\common\ShareMem.h"
#include "FastechPublic_IO.h"

IO_Manager g_ioMgr;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IO_Manager::IO_Manager()
{

}

IO_Manager::~IO_Manager()
{

}

BOOL IO_Manager::Get_In_Bit( int iIoNum )
{
	BOOL bRtn = FALSE;
	if( st_handler.mn_virtual_mode == 1 )
	{
		std::string strGet = g_sm.GetData( (SM_TYPE)iIoNum );
		if( strGet == "1" )
			bRtn = TRUE;
	}
	else
	{
		bRtn = FAS_IO.Get_In_Bit( iIoNum );
	}

	return bRtn;
}

int IO_Manager::Get_In_Bit( int iIoNum, int iDummy )
{
	return Get_In_Bit( iIoNum );
}

int IO_Manager::Set_Out_Bit( int nIONo, BOOL bOnOff ) 
{
	int iRtn = CTLBD_RET_GOOD;

	if( st_handler.mn_virtual_mode == 1 )
	{
		std::string strOnOff = "0";
		if( bOnOff == TRUE )
			strOnOff = "1";

		g_sm.SetData( (SM_TYPE)nIONo, strOnOff );
	}
	else
	{
		iRtn = FAS_IO.Set_Out_Bit( nIONo, bOnOff );
	}
	return iRtn;
}

int IO_Manager::Set_IO_PortAutoEnable(int nMasterID, int nPortNo)
{
	int iRtn = CTL_YES;
	if( st_handler.mn_virtual_mode == 1 )
	{

	}
	else
	{
		iRtn = FAS_IO.Set_IO_PortAutoEnable( nMasterID, nPortNo );
	}

	return iRtn;
}

int IO_Manager::Set_IO_SlaveEnable(int nMasterID, int nPortNo, int nSlaveNo, BOOL bEnable)
{
	int iRtn = CTL_YES;
	if( st_handler.mn_virtual_mode == 1 )
	{

	}
	else
	{
		iRtn = FAS_IO.Set_IO_SlaveEnable( nMasterID, nPortNo, nSlaveNo, bEnable );
	}

	return iRtn;
}

BOOL IO_Manager::Get_Out_Bit( int iIoNum )
{
	BOOL bRtn = FALSE;
	if( st_handler.mn_virtual_mode == 1 )
	{
		std::string strGet = g_sm.GetData( (SM_TYPE)iIoNum );
		if( strGet == "1" )
			bRtn = TRUE;
	}
	else
	{
		bRtn = FAS_IO.Get_Out_Bit( iIoNum );
	}
	
	return bRtn;	
}

BOOL IO_Manager::Get_Out_Bit( int iIoNum, int iDummy )
{
	return Get_Out_Bit( iIoNum );
}

WORD IO_Manager::Get_In_Word(int nIONo)
{
	WORD wRtn = 0x00;
	if( st_handler.mn_virtual_mode == 1 )
	{
		for( int i=0; i<16; i++ )
		{
			std::string strGet = g_sm.GetData( (SM_TYPE) (nIONo * 100 + i) );
			if( strGet == "1" )
				wRtn |= (1 << i);
		}
	}
	else
	{
		wRtn = FAS_IO.Get_In_Word( nIONo );
	}
	return wRtn;
}

WORD IO_Manager::Get_Out_Word(int nIONo)
{
	WORD wRtn = 0x00;
	if( st_handler.mn_virtual_mode ==  1 )
	{
		for( int i=0; i<16; i++ )
		{
			std::string strGet = g_sm.GetData( (SM_TYPE) (nIONo * 100 + i) );
			if( strGet == "1" )
				wRtn |= (1 << i);
		}
	}
	else
	{
		wRtn = FAS_IO.Get_Out_Word( nIONo );
	}
	return wRtn;
}

int IO_Manager::set_out_bit( int nIONo, BOOL bOnOff )
{
	int iRtn = 0;
	if( st_handler.mn_virtual_mode == 1 )
		iRtn = Set_Out_Bit( nIONo, bOnOff );
	else
		iRtn = FAS_IO.set_out_bit( nIONo, bOnOff );

	return iRtn;
}

BOOL IO_Manager::get_in_bit( int iIoNum, int iDummy )
{
	BOOL bRtn = FALSE;
	if( st_handler.mn_virtual_mode == 1 )
		bRtn = Get_In_Bit( iIoNum, iDummy );
	else
		bRtn = FAS_IO.get_in_bit( iIoNum, iDummy );

	return bRtn;
}

BOOL IO_Manager::get_out_bit( int iIoNum, int iDummy )
{
	BOOL bRtn = FALSE;
	if( st_handler.mn_virtual_mode == 1 )
		bRtn = Get_Out_Bit( iIoNum, iDummy );
	else
		bRtn = FAS_IO.get_out_bit( iIoNum, iDummy );

	return bRtn;
}
