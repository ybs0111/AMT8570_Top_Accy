// IO_Manager.h: interface for the IO_Manager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IO_MANAGER_H__EA6CB312_2364_436A_97C1_903DD2657C23__INCLUDED_)
#define AFX_IO_MANAGER_H__EA6CB312_2364_436A_97C1_903DD2657C23__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\common\ShareMem.h"

class IO_Manager  
{
public:
	IO_Manager();
	virtual ~IO_Manager();

	BOOL Get_In_Bit( int iIoNum );
	BOOL Get_In_Bit( int iIoNum, int iDummy );
	BOOL Get_Out_Bit( int iIoNum );
	BOOL Get_Out_Bit( int iIoNum, int iDummy );
	int Set_Out_Bit( int nIONo, BOOL bOnOff ) ;
	int Set_IO_PortAutoEnable(int nMasterID, int nPortNo);
	int Set_IO_SlaveEnable(int nMasterID, int nPortNo, int nSlaveNo, BOOL bEnable);

	WORD Get_In_Word(int nIONo);
	WORD Get_Out_Word(int nIONo);

	int set_out_bit( int nIONo, BOOL bOnOff );
	BOOL get_in_bit( int iIoNum, int iDummy=IO_OFF );
	BOOL get_out_bit( int iIoNum, int iDummy=IO_OFF );
};

extern IO_Manager g_ioMgr;

#endif // !defined(AFX_IO_MANAGER_H__EA6CB312_2364_436A_97C1_903DD2657C23__INCLUDED_)
