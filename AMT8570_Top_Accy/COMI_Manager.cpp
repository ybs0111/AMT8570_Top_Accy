// COMI_Manager.cpp: implementation of the COMI_Manager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "handler.h"

#include "COMI_Manager.h"
#include "..\common\ShareMem.h"

#include "Public_Function.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include "ComizoaPublic.h"
//#include "cmmsdkdef.h"
#include "Cmmsdk.h"

COMI_Manager g_comiMgr;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COMI_Manager::COMI_Manager()
{

}

COMI_Manager::~COMI_Manager()
{

}


int COMI_Manager::HomeCheck_Mot(int n_Axis, int n_HomeMode, int n_TimeOut)
{
	int iRtn = CTLBD_RET_GOOD;
	if( st_handler.mn_virtual_mode == 1 )
	{
		iRtn = CTL_Lib.OnSingleMove(n_Axis, 0, COMI.md_spd_home[n_Axis]);
	}
	else
	{
		iRtn = COMI.HomeCheck_Mot( n_Axis, n_HomeMode, n_TimeOut );
	}

	return iRtn;
}

int COMI_Manager::Get_MotPower(int n_Axis)
{
	int iRtn = CTL_GOOD;
	if( st_handler.mn_virtual_mode == 1 )
	{

	}
	else
	{
		iRtn = COMI.Get_MotPower( n_Axis );
	}

	return iRtn;
}

long COMI_Manager::_cmmSxIsDone(long Axis, long *IsDone)
{
	long nRtn = cmERR_NONE;
	if( st_handler.mn_virtual_mode == 1 )
	{
		std::string strTarget = g_sm.GetData( (SM_TYPE)(SMT_MOTOR_ROBOT_X_TARGET + Axis) );
		std::string strPosition = g_sm.GetData( (SM_TYPE)(SMT_MOTOR_ROBOT_X_POSITION + Axis) );
		int iXTarget = atoi( strTarget.c_str() );
		int iXPosition = atoi( strPosition.c_str() );

		if( iXTarget == iXPosition )
			*IsDone = cmTRUE;
		else
			*IsDone = cmFALSE;
	}
	else
	{
		nRtn = cmmSxIsDone( Axis, IsDone );
	}

	if( nRtn != cmERR_NONE )
	{
		CString strLog;
		strLog.Format( "_cmmSxIsDone(%d) : %d", Axis, nRtn );
		cLOG.OnLogEvent( LOG_PGM_, strLog );
	}

	return nRtn;
}

long COMI_Manager::_cmmCfgSetSpeedPattern_T(long Axis, long SpeedMode, double WorkSpeed, double Accel, double Decel)
{
	int iRtn = cmERR_NONE;
	if( st_handler.mn_virtual_mode == 1 )
	{

	}
	else
	{
		iRtn = cmmCfgSetSpeedPattern_T( Axis, SpeedMode, WorkSpeed, Accel, Decel );
	}
	return iRtn;
}

long COMI_Manager::_cmmSxSetSpeedRatio(long Axis, long SpeedMode, double VelRatio, double AccRatio, double DecRatio)
{
	int iRtn = cmERR_NONE;
	if( st_handler.mn_virtual_mode == 1 )
	{

	}
	else
	{
		iRtn = cmmSxSetSpeedRatio( Axis, SpeedMode, VelRatio, AccRatio, DecRatio );
	}
	return iRtn;
}

long COMI_Manager::_cmmSxMoveToStart(long Axis, double Position)
{
	int iRtn = cmERR_NONE;
	if( st_handler.mn_virtual_mode == 1 )
	{
		// target ����.
		char strData[32];
		itoa( Position, strData, 10 );
		g_sm.SetData( (SM_TYPE)(SMT_MOTOR_ROBOT_X_TARGET + Axis), strData );
	}
	else
	{
		iRtn = cmmSxMoveToStart( Axis, Position );
	}
	return iRtn;
}

double COMI_Manager::Get_MotCurrentPos(int n_Axis)
{
	double nRtn = CTLBD_RET_ERROR;
	if( st_handler.mn_virtual_mode == 1 )
	{
		std::string strPos = g_sm.GetData( (SM_TYPE)(SMT_MOTOR_ROBOT_X_POSITION + n_Axis) );
		int iPos = atoi( strPos.c_str() );
		nRtn = iPos;
	}
	else
	{
		nRtn = COMI.Get_MotCurrentPos( n_Axis );
	}

	return nRtn;
}

long COMI_Manager::_cmmMsCheckSlaveState(long SlaveAxis, long* SlaveState)
{
	long lRtn = cmERR_NONE;
	if( st_handler.mn_virtual_mode == 1 )
	{
		*SlaveState = 1;
	}
	else
	{
		lRtn = cmmMsCheckSlaveState( SlaveAxis, SlaveState );
	}
	return lRtn;
}

long COMI_Manager::_cmmIxIsDone( long MapIndex, long *IsDone )
{
	long nRtn = cmERR_NONE;
	if( st_handler.mn_virtual_mode == 1 )
	{
		std::string strXTarget = g_sm.GetData( SMT_MOTOR_ROBOT_X_TARGET );
		std::string strYTarget = g_sm.GetData( SMT_MOTOR_ROBOT_Y_TARGET );

		std::string strXPos = g_sm.GetData( SMT_MOTOR_ROBOT_X_POSITION );
		std::string strYPos = g_sm.GetData( SMT_MOTOR_ROBOT_Y_POSITION );

		int iXTarget = atoi( strXTarget.c_str() );
		int iXPosition = atoi( strXPos.c_str() );
		int iYTarget = atoi( strYTarget.c_str() );
		int iYPosition = atoi( strYPos.c_str() );
		
		if( iXTarget == iXPosition && iYTarget == iYPosition )
			*IsDone = cmTRUE;
		else
			*IsDone = cmFALSE;
	}
	else
	{
		nRtn = cmmIxIsDone( MapIndex, IsDone );
	}

	return nRtn;
}

long COMI_Manager::_cmmIxLineToStart( long MapIndex, double* PosList )
{
	int iRtn = cmERR_NONE;
	if( st_handler.mn_virtual_mode == 1 )
	{
		// target ����.
		char strData[32];
		itoa( PosList[0], strData, 10 );
		g_sm.SetData( SMT_MOTOR_ROBOT_X_TARGET, strData );

		itoa( PosList[1], strData, 10 );
		g_sm.SetData( SMT_MOTOR_ROBOT_Y_TARGET, strData );
	}
	else
	{
		iRtn = cmmIxLineToStart( MapIndex, PosList );
	}
	return iRtn;
}
