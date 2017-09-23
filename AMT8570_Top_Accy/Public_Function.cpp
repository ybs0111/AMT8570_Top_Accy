// Public_Function.cpp: implementation of the CPublic_Function class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Handler.h"
#include "Variable.h"			// ���� ���� ���� Ŭ����
#include "Public_Function.h"	// ���� ���� �� I/O ���� ���� Ŭ����
#include <nb30.h>
#include <wininet.h>
#include "Alg_Mysql.h"
#include "FastechPublic_IO.h"
#include "FAS_HSSI.h"
#include "ComizoaPublic.h"
#include "Cmmsdkdef.h"
#include "AMTLotManager.h"
#include "CtlBd_Function.h"
#include "CtlBd_Library.h"

// #include "stdafx.h"
// #include "iostream.h"
// #include "afxmt.h"

#include "ABpcManager.h"
#include "AMTRegistry.h"
// ******************************************************************************
// ��ȭ ���� Ŭ���� �߰�                                                         
// ******************************************************************************
#include "Dialog_Infor.h"
#include "Dialog_Select.h"
#include "Dialog_Message.h"

#include "Dialog_KeyPad.h"
// ******************************************************************************

#include "FastechPublic_IO.h"
#include "Cmmsdk.h"

// ���� ���� ���� ������
#include "Seq_BoxClamp.h"
#include "Seq_BoxLifter.h"
#include "Seq_BufferAlignConv.h"
#include "Seq_LoadingConv.h"
#include "Seq_XYZRobot.h"

typedef struct _ASTAT_ 
{ 

ADAPTER_STATUS adapt; 
NAME_BUFFER NameBuff [30]; 

}ASTAT, * PASTAT; 

ASTAT Adapter; 

#include "math.h"
#include "io.h"			// ���� ���� ���� �˻� �Լ� ȣ���ϱ� ���ؼ��� �ݵ�� �ʿ�

//CMyBasicData     mcls_p_basic;		// �⺻ ���� ���� �ε� �� ���� Ŭ����
CMutex pmutex;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CPublic_Function,CObject,1);	// ���� ������ ����...2K12/02/23/ViboX

CPublic_Function  Func;				// ���� Ŭ���� ������ ����

CPublic_Function::CPublic_Function()
{
	Mot_RetryCount = 3;	
}

CPublic_Function::~CPublic_Function()
{
	
}

//==================================================================//
// ������ ���ͷ�Ʈ ���� Ȯ��
//==================================================================//
int CPublic_Function::OnIsInterruptCondition() 
{
	if (st_handler.mn_mot_brd_initial != TRUE ||
		st_handler.nModeManual != NO ||
		st_handler.nSysLock != FALSE)
	{
		return 1;
	}
	return 0;
}

//==================================================================//
// ����ü�� �÷��� ���� �ʱ�ȭ
// - [�ʱ�ȭ �۾� ����, ��� �� IO �ʱ�ȭ ����]
//==================================================================//
void CPublic_Function::OnReset_FlagStructure() 
{
	st_handler.mn_mot_brd_initial	= CTLBD_NO;	// [����] ��� ���� �ʱ�ȭ �۾�
	st_handler.mn_io_board_initial	= CTLBD_NO;	// [����] I/O ���� �ʱ�ȭ �۾�
	st_ctlbd.n_motorbd_init_end		= CTLBD_NO;
	
	st_work.nAlarmHappened	= NO;	// �˶� �߻� ���� [�̹߻�]
}

//==================================================================//
// [�ʱ�ȭ] ���� ����
//==================================================================//
void CPublic_Function::OnReset_FlagInitialize() 
{
	st_handler.nInitTryREQ			= READY;	// �ʱ�ȭ ���� ���� ����� ���� ���� ����
	st_handler.nIsAllSiteInitEnd	= CTL_NO;	// [�̿Ϸ�] ��� �ʱ�ȭ �۾�
	// �ʱ�ȭ �۾��� Ÿ�̸� ���� �ĺ��� �˶� �����ϵ��� ������
	st_handler.nStart_INIT			= CTL_NO;	// �ʱ�ȭ ���۵��� �ʾ���

	st_handler.mn_motor_init_check	= CTL_READY;

	SeqLoadingConv.m_bflag_INIT = false;
	SeqBoxClamp.m_bflag_INIT = false;
	SeqBoxLifter.m_bflag_INIT = false;
	SeqXYZRobot.m_bflag_INIT = false;
	SeqBufferAlignConv.m_bflag_INIT = false;
}

//==================================================================//
// [�ʱ�ȭ] �ʱ�ȭ �۾����� ���Ͷ� ����
//==================================================================//
void CPublic_Function::OnReset_FlagInterLock() 
{
	stSync.nLotEnd_LoadingConv		= TRUE;
//	stSync.nLotEnd_LoadingConv[1]	= TRUE;
	stSync.nLotEnd_BoxClamp			= TRUE;
	stSync.nLotEnd_BoxLifter		= TRUE;
	stSync.nLotEnd_XYZRobot			= TRUE;
	stSync.nLotEnd_BufferAlignConv	= TRUE;
}

//==================================================================//
// [�ʱ�ȭ] SYNC �÷���
//==================================================================//
void CPublic_Function::OnReset_FlagSyncVar() 
{
	memset(&stSync, 0, sizeof(stSync));
}

//==================================================================//
// [�ʱ�ȭ] LOT ���� �÷���
//==================================================================//
void CPublic_Function::OnReset_FlagLotVar() 
{
	stSync.nLotEnd_LoadingConv	= FALSE;
	//stSync.nLotEnd_LoadingConv[1]	= FALSE;
	stSync.nLotEnd_BoxClamp			= FALSE;
	stSync.nLotEnd_BoxLifter		= FALSE;
	stSync.nLotEnd_XYZRobot			= FALSE;
	stSync.nLotEnd_BufferAlignConv	= FALSE;
}

//==================================================================//
// [�ʱ�ȭ] ����ü�� ���� �� ����
//==================================================================//
void CPublic_Function::OnReset_StructMapInfo() 
{

}

//==================================================================//
// ��ü ����Ʈ ���� ���� �Ϸ� ���� Ȯ��
//==================================================================//
int CPublic_Function::OnIsAllRcvyComplete() 
{
	//kwlee 2017.0825
	 return CTL_YES;

	if (stSync.nRcvyComplete[SITE_BOX_LIFT_]	== CTL_YES &&
		stSync.nRcvyComplete[SITE_XYZ_ROBOT_]	== CTL_YES)
	{
		return CTL_YES;
	}
	return CTL_NO;
}

//==================================================================//
// ���� ���� �Ϸ� ���� Ȯ��
//==================================================================//
void CPublic_Function::OnIsAllForceDischargeComplete() 
{
	if (stSync.nForceDischargeComplete[SITE_BOX_CLAMP_] == CTL_YES && stSync.nForceDischargeComplete[SITE_BOX_LIFT_] == CTL_YES &&
		stSync.nForceDischargeComplete[SITE_XYZ_ROBOT_] == CTL_YES && stSync.nForceDischargeComplete[SITE_BUFFER_CONV] == CTL_YES)
	{
		st_work.nForceDischarge = FORCEDISCHARGE_OFF;
		stSync.nForceDischargeComplete[SITE_BOX_CLAMP_] = CTL_NO;
		stSync.nForceDischargeComplete[SITE_BOX_LIFT_] = CTL_NO;
		stSync.nForceDischargeComplete[SITE_XYZ_ROBOT_] = CTL_NO;
		stSync.nForceDischargeComplete[SITE_BUFFER_CONV] = CTL_NO;

		if (st_work.nForceDischargeType)
		{
			OnSet_EqpStatusRun();
		}
	}

}

//==================================================================//
// [��� ����] SMEMA ���� ����
//==================================================================//
void CPublic_Function::OnSet_UpStreamSmema(int nzCase) 
{
	switch(nzCase)
	{
	case RDY_RESET_:
		g_ioMgr.set_out_bit(stIO.o_Smema_Ready2Main, IO_OFF);
		break;
	case RDY_READY_ON_:
		g_ioMgr.set_out_bit(stIO.o_Smema_Ready2Main, IO_ON);
		break;
	case RDY_READY_OFF_:
		g_ioMgr.set_out_bit(stIO.o_Smema_Ready2Main, IO_OFF);
		break;

	case REQ_EMO_ON_:
		g_ioMgr.set_out_bit(stIO.o_Smema_Emergency2Main, IO_ON);
		break;
	case REQ_EMO_OFF_:
		g_ioMgr.set_out_bit(stIO.o_Smema_Emergency2Main, IO_OFF);
		break;
	}
}

//==================================================================//
// [��� ����] ����� SMEMA ���� ��ȯ
//==================================================================//
int CPublic_Function::OnGet_UpStreamSmema() 
{
	int nRetVal = -1;

	int nSig_Req, nSig_Trsf;
	
	nSig_Req	= g_ioMgr.get_in_bit(stIO.i_Chk_ReqFromMain);
	nSig_Trsf	= g_ioMgr.get_in_bit(stIO.i_Chk_TrsfFromMain);

	if (nSig_Req == IO_OFF && nSig_Trsf == IO_OFF)
	{
		nRetVal = REQ_RESET_;	// CLAMP OFF, COMPLETE �� ����
	}
	else if (nSig_Req == IO_ON)
	{
		nRetVal = REQ_REQUEST_ON_;
	}
	else if (nSig_Req == IO_OFF && nSig_Trsf == IO_ON)
	{
		nRetVal = REQ_REQUEST_OFF_;
	}
	return nRetVal;
}

//==================================================================//
// ���� ȯ�� �Ķ���� ����
//==================================================================//
void CPublic_Function::OnSet_MotorUnitDist() 
{
	int i=0, nRet;
	for(i=0; i<MOT_MAXMOTOR; i++)
	{
		COMI.Set_Simulation_Mode(i, 0);
	}

	//==============================================================//
	// ���� 1ȸ�� �� �޽� �� ���� ���� ����
	// - ����1: ���� ��ȣ
	//==============================================================//
// 	COMI.Set_MotUnitDist(M_LIFTER_Z1, 1000.000);
// 	COMI.Set_MotUnitDist(M_LIFTER_Z2, 1000.000);
// 	COMI.Set_MotUnitDist(M_LIFTER_Z3, 1000.000);
// 	COMI.Set_MotUnitDist(M_LIFTER_Z4, 1000.000);
// 	COMI.Set_MotUnitDist(M_LIFTER_Y, 260.397);
// 	COMI.Set_MotUnitDist(M_BCRREAD_ROBOT_Y, 260.397);
// 	COMI.Set_MotUnitDist(M_BCRREAD_ROBOT_X, 66.662);
// 	COMI.Set_MotUnitDist(M_BCRREAD_ROBOT_Z, 5000.000);
// 
// 
// 	COMI.Set_MotUnitSpeed(M_LIFTER_Z1,			1000.000);
// 	COMI.Set_MotUnitSpeed(M_LIFTER_Z2,			1000.000);
// 	COMI.Set_MotUnitSpeed(M_LIFTER_Z3,			1000.000);
// 	COMI.Set_MotUnitSpeed(M_LIFTER_Z4,			1000.000);
// 	COMI.Set_MotUnitSpeed(M_LIFTER_Y,	260.397);
// 	COMI.Set_MotUnitSpeed(M_BCRREAD_ROBOT_Y,	260.397);
// 	COMI.Set_MotUnitSpeed(M_BCRREAD_ROBOT_X,	66.662);
// 	COMI.Set_MotUnitSpeed(M_BCRREAD_ROBOT_Z,	5000.000);

	//kwlee 2017.0830
	COMI.Set_MotUnitDist(M_LIFTER_Z1, 2000.000);
	COMI.Set_MotUnitDist(M_LIFTER_Z2, 2000.000);
	COMI.Set_MotUnitDist(M_LIFTER_Z3, 2000.000);
	COMI.Set_MotUnitDist(M_LIFTER_Z4, 2000.000);
	COMI.Set_MotUnitDist(M_LIFTER_Y, 500.000);
	COMI.Set_MotUnitDist(M_BCRREAD_ROBOT_Y, 200.087);
	COMI.Set_MotUnitDist(M_BCRREAD_ROBOT_X, 181.932);
	COMI.Set_MotUnitDist(M_BCRREAD_ROBOT_Z, 1000.000);
	
	
	COMI.Set_MotUnitSpeed(M_LIFTER_Z1,			2000.000);
	COMI.Set_MotUnitSpeed(M_LIFTER_Z2,			2000.000);
	COMI.Set_MotUnitSpeed(M_LIFTER_Z3,			2000.000);
	COMI.Set_MotUnitSpeed(M_LIFTER_Z4,			2000.000);
	COMI.Set_MotUnitSpeed(M_LIFTER_Y,	500.000);
	COMI.Set_MotUnitSpeed(M_BCRREAD_ROBOT_Y,	200.087);
	COMI.Set_MotUnitSpeed(M_BCRREAD_ROBOT_X,	181.932);
	COMI.Set_MotUnitSpeed(M_BCRREAD_ROBOT_Z,	1000.000);

	for(i=0; i<MOT_MAXMOTOR; i++)
	{
		// SW ����Ʈ �� ����
		// ȭ�鿡�� ������ �� �ε��Ͽ� ������
		/*
		COMI.md_limit_position[i][0] = -10;
		COMI.md_limit_position[i][1] = 3000;
		*/
		// ����� ����
		// ���� �ະ�� ƼĪ ȭ�鿡�� ������ ���� ���� ����ü ������ �Ҵ�
		st_motor[i].d_limit_position[0] = COMI.md_limit_position[i][0];
		st_motor[i].d_limit_position[1] = COMI.md_limit_position[i][1];

		// ���̸� �ּ� ó�� [2014.02.17]
		//cmmCfgSetCtrlMode(i, 1);
		// ���ڴ� [�ǵ��] �������� ��ǥ ��ǥ�� �����Ͽ� �̼�
		// cmCTRL_SEMI_C
		cmmCfgSetCtrlMode(i, cmCTRL_SEMI_C);

		//  LIMIT ���� HOMING [- �̰���]
		// - [el on]->[stop]->[back]->[el off]->[stop]
		COMI.mn_homecheck_method[i] = 6;

 		st_motor[i].nStepMotor = FALSE;

	
		nRet = COMI.Set_HomeSetConfig(i, COMI.mn_homecheck_method[i], 0, 1000, 10);
		if (nRet != CTL_YES)
		{
			sprintf(st_msg.c_abnormal_msg, "Set_HomeSet_Config Error [MotNum[%d]]", i);
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
		}
	}
	//--------------------------------------------------------------//



	// ���� �Ŀ� ON
	for(i=0; i<MOT_MAXMOTOR; i++)
	{
		COMI.Set_MotPower(i, IO_ON);
	}

	if (st_handler.cwnd_list != NULL)
	{
		//(st_other.str_op_msg).Format("[OnSet_MotorUnitDist]");
		sprintf(st_msg.c_normal_msg, "[OnSet_MotorUnitDist]");
		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
	}
}

BOOL CPublic_Function::CreateFolder(LPCTSTR szPath)
{
	int nRet = 0;
	
	CString strPath = szPath;
	
    do{
        nRet = strPath.Find('\\' , nRet + 1);
        if (nRet > 0 )
        {
            CString strTemp = strPath.Left(nRet+1);
			
            if (_access(strTemp.operator LPCTSTR(), 00) == -1)
			{
				CreateDirectory(strTemp , NULL);
			}
        }
    }while (nRet != -1);
	
    return nRet;
}

//==================================================================//
// I/O ����
//==================================================================//
void CPublic_Function::OnSet_MappingIO() 
{
	int i=0, k=0;

	//==============================================================//
	// ��� ��ȣ : 00 / 01
	//==============================================================//
	stIO.o_TLampGreen			= 1;
	stIO.o_TLampYellow			= 2;
	stIO.o_TLampRed				= 3;
	stIO.o_BuzzerSound1			= 4;
	stIO.o_BuzzerSound2			= 5;
	stIO.o_BuzzerSound3			= 6;
	stIO.o_DoorLock				= 7;

	for(i=0; i<MAX_BUZZ_; i++)
	{
		stIO.o_BuzzerSound[i]	= 4+i;
	}


	stIO.i_Chk_MC1 = 9;
	stIO.i_Chk_EMO[0] = 10;
	stIO.i_Chk_EMO[1] = 11;

	for(i=0; i<MAX_DOOR_; i++)
	{
// 		if(i<=3)stIO.i_Chk_Door[i]	= 12+i;
// 		else stIO.i_Chk_Door[i]		= 100 + i;
		//kwlee 2017.0717
		stIO.i_Chk_Door[i]		= 100 + i;
	}

	//==============================================================//

	//==============================================================//
	// ��� ��ȣ : 02
	//==============================================================//
	stIO.o_LampStart				= 200;
	stIO.o_LampStop					= 201;
	stIO.o_LampJamClear				= 202;
	stIO.o_LampBuzzStop				= 203;
	//kwlee 2017.0717
	stIO.o_BufferConvMotor_Left		= 204;
	stIO.o_BufferConvMotor_Right	= 205;
	
	for(i=0; i<MAX_SITE_; i++)
	{
		stIO.o_BufferConvMotor[i] = 204 + i;
 	}
	//

	stIO.i_Chk_StartSwitch			= 208;
	stIO.i_Chk_StopSwitch			= 209;
	stIO.i_Chk_JamClearSwitch		= 210;
	stIO.i_Chk_BuzzStopSwitch		= 211;
	stIO.i_Chk_AutoSwitch			= 212;
	stIO.i_Chk_ManualSwitch			= 213;
	stIO.i_Chk_FrontSelectSwitch	= 214;
	

	//==============================================================//

	//==============================================================//
	// ��� ��ȣ : 03
	//==============================================================//
	stIO.o_LampManualLoading = 300;
	stIO.o_LampLoadingStart = 301;
	stIO.o_AccyBoxConvMotor2 = 302;
	stIO.o_Cyl_AccyBoxConvStopperUp2 = 303;
	stIO.o_Cyl_AccyBoxConvStopperDn2 = 304;
// 	stIO.o_Cyl_AccyBoxPusher = 305;
		
	stIO.i_Chk_ManualLoadingSwitch = 307;
	stIO.i_Chk_LoadingStartSwitch = 308;
	stIO.i_Chk_AccyBoxConvStopperUp2 = 309;
	stIO.i_Chk_AccyBoxConvStopperDn2 = 310;
// 	stIO.i_Chk_AccyBoxPusherFw = 311;
// 	stIO.i_Chk_AccyBoxPusherBw = 312;
// 
	stIO.i_Chk_AccyBoxConvDetection_First = 313;
	stIO.i_Chk_AccyBoxConvDetection_Second = 314;
	stIO.i_Chk_AccyBoxConvDetection_Third = 315;
	
	for(i=0; i<MAX_DETECTBOX_; i++)
	{
		stIO.i_Chk_AccyBoxConvDetection[i] = 313 + i;
	}

	//==============================================================//

	//==============================================================//
	// ��� ��ȣ : 04
	//==============================================================//
// 	stIO.o_Cyl_AccyBoxLifterUp = 400;
// 	stIO.o_Cyl_AccyBoxLifterDn = 401;	
	//kwlee 2017.0710
	stIO.o_Load_BoxPusherCylPusher		= 400;
	stIO.o_Load_BoxPusherCylPull		= 401;
	//kwlee 2017.0710
	stIO.o_AccyBoxConvMotor				= 402;
	stIO.o_Cyl_AccyBoxConvStopperUp		= 403;
	stIO.o_Cyl_AccyBoxConvStopperDn		= 404;
	stIO.o_Load_BoxClampOnOff			= 405;

// 	for(i=0; i<MAX_SITE_; i++)
// 	{
// 		stIO.i_Chk_AccyBoxLifterUp[i] = 408 + (i * 2);
// 		stIO.i_Chk_AccyBoxLifterDn[i] = 409 + (i * 2);
// 	}

	//kwlee 2017.0710
	stIO.i_Chk_Load_BoxPusherCylPusherCheck            = 408;
	stIO.i_Chk_Load_BoxPusherCylPullCheck			   = 409;

// 	stIO.i_Chk_AccyBoxConvStopperUp = 412;
// 	stIO.i_Chk_AccyBoxConvStopperDn = 413;
	//kwlee 2017.0710
	stIO.i_Chk_AccyBoxConvStopperUp						= 410;
	stIO.i_Chk_AccyBoxConvStopperDn						= 411;
	//kwlee 2017.0710
	stIO.i_Chk_Load_BoxClampOnCheck						= 412;
	stIO.i_Chk_Load_BoxClampOffCheck					= 413;
	

	//==============================================================//

	//==============================================================//
	// ��� ��ȣ : 05
	//==============================================================//
	stIO.o_Cyl_AccyBoxGripperUp = 500;
	stIO.o_Cyl_AccyBoxGripperDn = 501;	
	stIO.o_Cyl_AccyBoxClamp = 502;
	stIO.o_Cyl_AccyBoxUnclamp = 503;	
	
	for(i=0; i<MAX_SITE_; i++)
	{
		stIO.i_Chk_AccyBoxGripperUp[i] = 506 + (i * 2);
		stIO.i_Chk_AccyBoxGripperDn[i] = 507 + (i * 2);

		stIO.i_Chk_AccyBoxClamp[i] = 510 + (i * 2);
		stIO.i_Chk_AccyBoxUnclamp[i] = 511 + (i * 2);
	}
	stIO.i_Chk_GripperSafetyPos = 514;

	//==============================================================//

	//==============================================================//
	// ��� ��ȣ : 06
	//==============================================================//
	
	// ��ȣ�� ���� ���� ���� �������� 0(���� ��), 1(���� ��), 2(���� ��), 3(���� ��)	
	stIO.i_Chk_BoxClampAccyDetection[SIDE_LEFT_][0]		= 600;
	stIO.i_Chk_BoxClampAccyDetection[SIDE_RIGHT_][0]	= 601;
	stIO.i_Chk_BoxClampAccyDetection[SIDE_LEFT_][1]		= 602;
	stIO.i_Chk_BoxClampAccyDetection[SIDE_RIGHT_][1]	= 603;
	stIO.i_Chk_BoxClampAccyDetection[SIDE_LEFT_][2]		= 604;
	stIO.i_Chk_BoxClampAccyDetection[SIDE_RIGHT_][2]	= 605;
	stIO.i_Chk_BoxClampAccyDetection[SIDE_LEFT_][3]		= 606;
	stIO.i_Chk_BoxClampAccyDetection[SIDE_RIGHT_][3]	= 607;
	stIO.i_Chk_BoxClampAccyDetection[SIDE_LEFT_][4]		= 608;
	stIO.i_Chk_BoxClampAccyDetection[SIDE_RIGHT_][4]	= 609;
	stIO.i_Chk_BoxClampAccyDetection[SIDE_LEFT_][5]		= 610;
	stIO.i_Chk_BoxClampAccyDetection[SIDE_RIGHT_][5]	= 611;

	//==============================================================//

	//==============================================================//
	// ��� ��ȣ : 07
	//==============================================================//
	
// 	stIO.o_Load_BoxClampOnOff							= 700;
// 	//stIO.o_Load_BoxPusherFwBw							= 701;
// 	//kwlee 2017.0706
// 	stIO.o_Load_BoxPusherCylPusher						= 701;
// 	stIO.o_Load_BoxPusherCylPull						= 702;
// 
// 	stIO.i_Chk_Load_BoxClampOnCheck						= 707;
// 	stIO.i_Chk_Load_BoxClampOffCheck					= 708;
// 
// // 	stIO.i_Chk_Load_BoxPusherFwCheck					= 709;
// // 	stIO.i_Chk_Load_BoxPusherBwCheck					= 710;	
// 	//kwlee 2017.07
// 	stIO.i_Chk_Load_BoxPusherCylPusherCheck             = 709;
// 	stIO.i_Chk_Load_BoxPusherCylPullCheck			   = 710;

	//kwlee 2017.0710
	stIO.o_Cyl_AccyBoxLifterUp = 700;
	stIO.o_Cyl_AccyBoxLifterDn = 701;	

	for(i=0; i<MAX_SITE_; i++)
	{
	 	stIO.i_Chk_AccyBoxLifterUp[i] = 708 + (i * 2);
		stIO.i_Chk_AccyBoxLifterDn[i] = 709 + (i * 2);
	}
	
	//kwlee 2017.0710
	stIO.i_Chk_AccyBox_Arrive_End = 714;
	//==============================================================//
	
	//==============================================================//
	// ��� ��ȣ : 10
	//==============================================================//
	stIO.o_Slop_Seperator_Stopper_UpDn					= 1000;
	stIO.o_Out_Front_Stopper_UpDn						= 1001;
	stIO.o_Out_Rear_Stopper_UpDn						= 1002;
	
	stIO.i_Chk_Slop_Seperator_StopperUpCheck			= 1004;
	stIO.i_Chk_Slop_Seperator_StopperDnCheck			= 1005;	
	stIO.i_Chk_Out_Front_StopperUpCheck					= 1006;
	stIO.i_Chk_Out_Front_StopperDnCheck					= 1007;	

	stIO.i_Chk_Out_Rear_StopperUpCheck					= 1008;
	stIO.i_Chk_Out_Rear_StopperDnCheck					= 1009;	

	stIO.i_Chk_Slop_Left_AccyDetection					= 1010;
	stIO.i_Chk_Slop_Right_AccyDetection					= 1011;	

	stIO.i_Chk_Out_Front_Left_Accy_DetectionChk			= 1012;
	stIO.i_Chk_Out_Front_Right_Accy_DetectionChk		= 1013;
	stIO.i_Chk_Out_Rear_Left_Accy_DetectionChk			= 1014;
	stIO.i_Chk_Out_Rear_Right_Accy_DetectionChk			= 1015;

	//kwlee 2017.0715
	stIO.i_Chk_BufferAccyConvStopper[SIDE_MIDDLE][0]	 = 1004;
	stIO.i_Chk_BufferAccyConvStopper[SIDE_MIDDLE][1]	 = 1005;	
	
	stIO.i_Chk_BufferAccyConvStopper[SIDE_OUT_FRONT][0]  = 1006;
	stIO.i_Chk_BufferAccyConvStopper[SIDE_OUT_FRONT][1]  = 1007;	
	stIO.i_Chk_BufferAccyConvStopper[SIDE_OUT_REAR][0]   = 1008;
	stIO.i_Chk_BufferAccyConvStopper[SIDE_OUT_REAR][1]   = 1009;

	
	stIO.i_Chk_BufferTryAccyDetection[SIDE_MIDDLE][SIDE_LEFT_]  = 1010;
	stIO.i_Chk_BufferTryAccyDetection[SIDE_MIDDLE][SIDE_RIGHT_] = 1011;	

// 	stIO.i_Chk_BufferTryAccyDetection[SIDE_OUT_FRONT][SIDE_LEFT_]  = 1012;
// 	stIO.i_Chk_BufferTryAccyDetection[SIDE_OUT_FRONT][SIDE_RIGHT_] = 1013;	
// 	stIO.i_Chk_BufferTryAccyDetection[SIDE_OUT_REAR][SIDE_LEFT_]   = 1014;
// 	stIO.i_Chk_BufferTryAccyDetection[SIDE_OUT_REAR][SIDE_RIGHT_]  = 1015;

	//kwlee 2017.0717
	stIO.i_Chk_BufferTryAccyDetection[SIDE_OUT_REAR][SIDE_LEFT_]  = 1012;
	stIO.i_Chk_BufferTryAccyDetection[SIDE_OUT_REAR][SIDE_RIGHT_] = 1013;	
	stIO.i_Chk_BufferTryAccyDetection[SIDE_OUT_FRONT][SIDE_LEFT_]   = 1014;
	stIO.i_Chk_BufferTryAccyDetection[SIDE_OUT_FRONT][SIDE_RIGHT_]  = 1015;

	//==============================================================//

	//==============================================================//
	// ��� ��ȣ : 11
	//==============================================================//
	stIO.o_Slop_Variable_Front_Left_OpenClose			= 1100;
	stIO.o_Slop_Variable_Front_Right_OpenClose			= 1101;
	stIO.o_Slop_Variable_Rear_Left_OpenClose			= 1102;
	stIO.o_Slop_Variable_Rear_Right_OpenClose			= 1103;
	//kwlee 2017.0717
// 	stIO.o_BufferConvMotor_Left							= 1104;
// 	stIO.o_BufferConvMotor_Right						= 1105;
	
// 	for(i=0; i<MAX_SITE_; i++)
// 	{
// 		stIO.o_BufferConvMotor[i] = 1104 + i;
// 	}
	
	stIO.i_Slop_Variable_Font_Left_Open_Check			= 1108;
	stIO.i_Slop_Variable_Font_Left_Close_Check			= 1109;
	stIO.i_Slop_Variable_Font_Right_Open_Check			= 1110;
	stIO.i_Slop_Variable_Font_Right_Close_Check			= 1111;
	
	stIO.i_Slop_Variable_Rear_Left_Open_Check			= 1112;
	stIO.i_Slop_Variable_Rear_Left_Close_Check			= 1113;
	stIO.i_Slop_Variable_Rear_Right_Open_Check			= 1114;
	stIO.i_Slop_Variable_Rear_Right_Close_Check			= 1115;
	
	//==============================================================//

	//==============================================================//
	// ��� ��ȣ : 12
	//==============================================================//
	stIO.o_Picker_fl_updn		= 1200;//front left
	stIO.o_Picker_rl_updn		= 1201;//rear left
	stIO.o_Picker_fr_updn		= 1201;//front right
	stIO.o_Picker_rr_updn		= 1203;//rear right
// 	stIO.o_Picker_updn[SIDE_FRONT_][SIDE_LEFT_]  = 1200;
// 	stIO.o_Picker_updn[SIDE_REAR_][SIDE_LEFT_]   = 1201;
// 	stIO.o_Picker_updn[SIDE_FRONT_][SIDE_RIGHT_] = 1202;
// 	stIO.o_Picker_updn[SIDE_REAR_][SIDE_RIGHT_]  = 1203;
	//kwlee 2017.0814
	stIO.o_Picker_updn[SIDE_FRONT_][SIDE_LEFT_]  = 1200;
	stIO.o_Picker_updn[SIDE_FRONT_][SIDE_RIGHT_] = 1201;
	stIO.o_Picker_updn[SIDE_REAR_][SIDE_LEFT_]	 = 1202;
	stIO.o_Picker_updn[SIDE_REAR_][SIDE_RIGHT_]  = 1203;
	stIO.o_Picker_pitch			= 1204;

	stIO.i_Picker_fl_up_chk		= 1208;
	stIO.i_Picker_rl_up_chk		= 1209;
	stIO.i_Picker_fr_up_chk		= 1210;
	stIO.i_Picker_rr_up_chk		= 1211;
// 	stIO.i_Picker_updn_chk[SIDE_FRONT_][SIDE_LEFT_]  = 1208;
// 	stIO.i_Picker_updn_chk[SIDE_REAR_][SIDE_LEFT_]   = 1209;
// 	stIO.i_Picker_updn_chk[SIDE_FRONT_][SIDE_RIGHT_] = 1210;
// 	stIO.i_Picker_updn_chk[SIDE_REAR_][SIDE_RIGHT_]  = 1211;
	//kwlee 2017.0814
	stIO.i_Picker_updn_chk[SIDE_FRONT_][SIDE_LEFT_]		= 1208;
	stIO.i_Picker_updn_chk[SIDE_FRONT_][SIDE_RIGHT_]	= 1209;
	stIO.i_Picker_updn_chk[SIDE_REAR_][SIDE_LEFT_]		= 1210;
	stIO.i_Picker_updn_chk[SIDE_REAR_][SIDE_RIGHT_]		= 1211;

	stIO.i_Picker_open_check	= 1212;
	stIO.i_Picker_close_check	= 1213;
	stIO.i_Picker_front_up_chk	= 1214;
	stIO.i_Picker_rear_up_chk	= 1215;

	//==============================================================//

	//==============================================================//
	// ��� ��ȣ : 13
	//==============================================================//
	// ��ȣ�� ���� ���� ���� �������� 0(���� ��), 1(���� ��), 2(���� ��), 3(���� ��)
	stIO.o_Vac_Rbt_VacOnOff_Front_Left					= 1300;
	stIO.o_Vac_Rbt_Eject_Front_Left						= 1301;
	stIO.o_Vac_Rbt_VacOnOff_Rear_Left					= 1302;
	stIO.o_Vac_Rbt_Eject_Rear_Left						= 1303;
	stIO.o_Vac_Rbt_VacOnOff_Front_Right					= 1304;
	stIO.o_Vac_Rbt_Eject_Front_Right					= 1305;
	stIO.o_Vac_Rbt_VacOnOff_Rear_Right					= 1306;
	stIO.o_Vac_Rbt_Eject_Rear_Right						= 1307;


//  	stIO.o_Vac_Rbt_VacOnOff[SIDE_FRONT_][SIDE_LEFT_]	= 1300;
//  	stIO.o_Vac_Rbt_VacOnOff[SIDE_FRONT_][SIDE_RIGHT_]	= 1302;
//  	
// 	stIO.o_Vac_Rbt_VacOnOff[SIDE_REAR_][SIDE_LEFT_]		= 1304;
//  	stIO.o_Vac_Rbt_VacOnOff[SIDE_REAR_][SIDE_RIGHT_]	= 1306;
//  	
// 	stIO.o_Vac_Rbt_EjectOnOff[SIDE_FRONT_][SIDE_LEFT_]	= 1301;
//  	stIO.o_Vac_Rbt_EjectOnOff[SIDE_FRONT_][SIDE_RIGHT_]	= 1303;
//  	stIO.o_Vac_Rbt_EjectOnOff[SIDE_REAR_][SIDE_LEFT_]	= 1305;
//  	stIO.o_Vac_Rbt_EjectOnOff[SIDE_REAR_][SIDE_RIGHT_]	= 1307;

	//kwlee 2017.0717
// 	stIO.o_Vac_Rbt_VacOnOff[SIDE_FRONT_][SIDE_LEFT_]	= 1300;
// 	stIO.o_Vac_Rbt_VacOnOff[SIDE_REAR_][SIDE_LEFT_]		= 1302;
// 	stIO.o_Vac_Rbt_VacOnOff[SIDE_FRONT_][SIDE_RIGHT_]	= 1304;	
// 	stIO.o_Vac_Rbt_VacOnOff[SIDE_REAR_][SIDE_RIGHT_]	= 1306;
// 	
// 	stIO.o_Vac_Rbt_EjectOnOff[SIDE_FRONT_][SIDE_LEFT_]	= 1301;
// 	stIO.o_Vac_Rbt_EjectOnOff[SIDE_REAR_][SIDE_LEFT_]	= 1303;
// 	stIO.o_Vac_Rbt_EjectOnOff[SIDE_FRONT_][SIDE_RIGHT_]	= 1305;
// 	stIO.o_Vac_Rbt_EjectOnOff[SIDE_REAR_][SIDE_RIGHT_]	= 1307;
	//kwlee 2017.0814
	stIO.o_Vac_Rbt_VacOnOff[SIDE_FRONT_][SIDE_LEFT_]	= 1300;
	stIO.o_Vac_Rbt_VacOnOff[SIDE_FRONT_][SIDE_RIGHT_]	= 1302;
	stIO.o_Vac_Rbt_VacOnOff[SIDE_REAR_][SIDE_LEFT_]		= 1304;	
	stIO.o_Vac_Rbt_VacOnOff[SIDE_REAR_][SIDE_RIGHT_]	= 1306;
	
	stIO.o_Vac_Rbt_EjectOnOff[SIDE_FRONT_][SIDE_LEFT_]	= 1301;
	stIO.o_Vac_Rbt_EjectOnOff[SIDE_FRONT_][SIDE_RIGHT_]	= 1303;
	stIO.o_Vac_Rbt_EjectOnOff[SIDE_REAR_][SIDE_LEFT_]	= 1305;
	stIO.o_Vac_Rbt_EjectOnOff[SIDE_REAR_][SIDE_RIGHT_]	= 1307;

	
	stIO.i_Chk_Vac_Rbt_VacOnOff_Front_Left				= 1308;
	stIO.i_Chk_Vac_Rbt_VacOnOff_Front_Right				= 1309;
	stIO.i_Chk_Vac_Rbt_VacOnOff_Rear_Left				= 1310;
	stIO.i_Chk_Vac_Rbt_VacOnOff_Rear_Right				= 1311;

//  	stIO.i_Chk_Rbt_VacOn[SIDE_FRONT_][SIDE_LEFT_]		= 1308;
//  	stIO.i_Chk_Rbt_VacOn[SIDE_FRONT_][SIDE_RIGHT_]		= 1309;
//  	stIO.i_Chk_Rbt_VacOn[SIDE_REAR_][SIDE_LEFT_]		= 1310;
//  	stIO.i_Chk_Rbt_VacOn[SIDE_REAR_][SIDE_RIGHT_]		= 1311;
//kwlee 2017.0717
// 	stIO.i_Chk_Rbt_VacOn[SIDE_FRONT_][SIDE_LEFT_]		= 1308;
// 	stIO.i_Chk_Rbt_VacOn[SIDE_REAR_][SIDE_LEFT_]		= 1309;
// 	stIO.i_Chk_Rbt_VacOn[SIDE_FRONT_][SIDE_RIGHT_]		= 1310;
//  	stIO.i_Chk_Rbt_VacOn[SIDE_REAR_][SIDE_RIGHT_]		= 1311;
	//kwlee 2017.0814
	stIO.i_Chk_Rbt_VacOn[SIDE_FRONT_][SIDE_LEFT_]		= 1308;
	stIO.i_Chk_Rbt_VacOn[SIDE_FRONT_][SIDE_RIGHT_]		= 1309;
	stIO.i_Chk_Rbt_VacOn[SIDE_REAR_][SIDE_LEFT_]		= 1310;
 	stIO.i_Chk_Rbt_VacOn[SIDE_REAR_][SIDE_RIGHT_]		= 1311;
	
	stIO.i_Chk_OutLeftBufferAccyDetection				= 1312;
	stIO.i_Chk_OutRightBufferAccyDetection				= 1313;


	//==============================================================//
	// ��� ��ȣ : 14
	//==============================================================//

	stIO.o_Cyl_BufferTryPathAlignUp = 1400;
	stIO.o_Cyl_BufferTryPathAlignDn = 1401;

// 	stIO.o_Cyl_BufferAccyConvStopper = 1102;
	stIO.o_Cyl_BufferAccyConvFrontStopper = 1402;
	stIO.o_Cyl_BufferAccyConvRearStopper = 1403;

	
	stIO.i_Chk_BufferTryPathAlignUp	= 1404;
	stIO.i_Chk_BufferTryPathAlignDn = 1405;

	stIO.i_Chk_BufferAccy_Front_StopperUpCheck = 1406;
	stIO.i_Chk_BufferAccy_Front_StopperDnCheck = 1407;
	stIO.i_Chk_BufferAccy_Rear_StopperUpCheck = 1408;
	stIO.i_Chk_BufferAccy_Rear_StopperDnCheck = 1409;


// 	for(i=0; i<MAX_SITE_; i++)
// 	{
// 		stIO.i_Chk_BufferAccyConvStopper[i][0] = 1406 + (i * 2);
// 		stIO.i_Chk_BufferAccyConvStopper[i][1] = 1407 + (i * 2);
// 	}

	stIO.i_Chk_BufferAccyConvStopper[SIDE_FRONT_][0] = 1406;
	stIO.i_Chk_BufferAccyConvStopper[SIDE_FRONT_][1] = 1407;
	stIO.i_Chk_BufferAccyConvStopper[SIDE_REAR_][0] = 1408;
	stIO.i_Chk_BufferAccyConvStopper[SIDE_REAR_][1] = 1409;

	stIO.i_Chk_BufferTryAccyDetection[SIDE_FRONT_][SIDE_LEFT_]	= 1410;
	stIO.i_Chk_BufferTryAccyDetection[SIDE_FRONT_][SIDE_RIGHT_]	= 1411;
	stIO.i_Chk_BufferTryAccyDetection[SIDE_REAR_][SIDE_LEFT_]	= 1412;
	stIO.i_Chk_BufferTryAccyDetection[SIDE_REAR_][SIDE_RIGHT_]	= 1413;

	stIO.i_Chk_NGBufferTryFull = 1414;


// 	stIO.i_Chk_BufferTryAccyDetection[SIDE_OUT_FRONT][SIDE_LEFT_] = 1008;
// 	stIO.i_Chk_BufferTryAccyDetection[SIDE_OUT_FRONT][SIDE_RIGHT_] = 1009;
// 	
// 	stIO.i_Chk_BufferTryAccyDetection[SIDE_OUT_REAR][SIDE_LEFT_] = 1010;
// 	stIO.i_Chk_BufferTryAccyDetection[SIDE_OUT_REAR][SIDE_RIGHT_] = 1011;
	


	//==============================================================//
	// ��� ��ȣ : 15
	//==============================================================//

	
	
	stIO.o_Smema_Ready2Main = 1500;
	stIO.o_Smema_Emergency2Main = 1501;
	
// 	stIO.i_Chk_UnloaderFull[0] = 1508;
// 	stIO.i_Chk_UnloaderFull[1] = 1509;
	
	//kwlee 2017.0711
	stIO.i_Chk_UnloaderFull[0] = 1500;
	stIO.i_Chk_UnloaderFull[1] = 1501;
	stIO.i_Chk_UnloaderFull[2] = 1502;

	stIO.i_Chk_ReqFromMain = 1510;
	stIO.i_Chk_TrsfFromMain = 1511;


	stIO.i_Chk_NGBufferAccyDetection[SIDE_LEFT_]	= 1512;

	stIO.i_Chk_NGBufferAccyDetection[SIDE_RIGHT_]	= 1514;


	//==============================================================//
}

//==================================================================//
// ���� �̸� ��ȯ
//==================================================================//
CString CPublic_Function::OnGet_MotorName(int nzMotorNo)
{
	CString sRet;

	switch (nzMotorNo)
	{
	case 0:
		sRet = "Lift_Up_Down_Shift1";
		break;
	case 1:
		sRet = "Lift_Up_Down_Shift2";
		break;
	case 2:
		sRet = "Lift_Up_Down_Shift3";
		break;
	case 3:
		sRet = "Lift_Up_Down_Shift4";
		break;
	case 4:
		sRet = "Lifter_X";
		break;
	case 5:
		sRet = "Barcode_Robot_X";
		break;
	case 6:
		sRet = "Barcode_Robot_Y";
		break;
	case 7:
		sRet = "Barcode_Robot_Z";
		break;
	case 8:
		sRet = "Robot_Grip_1";
		break;
	case 9:
		sRet = "Robot_Grip_2";
		break;
	case 10:
		sRet = "Robot_Grip_3";
		break;
	case 11:
		sRet = "Robot_Grip_4";
		break;
	default:
		sRet = "Unknown_Motor";
		break;
	}
	
	return sRet;
}

//==================================================================//
// ���� ���� �ӵ� ����
//==================================================================//
void CPublic_Function::OnSet_MotorSpeed() 
{
	int i, nRet=0;

	for(i=0; i<MOT_MAXMOTOR; i++)
	{
		if (COMI.md_spd_vel[i][1] < 1)
		{
			COMI.md_spd_vel[i][1] = 10000;

			if (st_handler.cwnd_list != NULL)
			{
				sprintf(st_msg.c_abnormal_msg, "%s Motor Acc is under 1 -> 10000 Set up", OnGet_MotorName(i));
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
			}
		}

		if (COMI.md_spd_vel[i][2] < 1)
		{
			COMI.md_spd_vel[i][2] = 10000;

			if (st_handler.cwnd_list != NULL)
			{
				sprintf(st_msg.c_abnormal_msg, "%s Motor Dec is under 1 -> 10000 Set up", OnGet_MotorName(i));
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
			}
		}

		/*
		COMI.Set_MotSpeed(MOT_SPD_VEL, i, cmSMODE_T, COMI.md_spd_vel[i][0], COMI.md_spd_vel[i][1], COMI.md_spd_vel[i][2]);
		COMI.Set_MotSpeed(MOT_SPD_ORG, i, cmSMODE_T, st_motor[i].md_spd_home, COMI.md_spd_vel[i][1]/10, COMI.md_spd_vel[i][2]/10);

		COMI.Set_HomeSetConfig(i, COMI.mn_homecheck_method[i], 0, 2, 1);
		*/
		// ���̸� ���� [2014.08.27]
		nRet = COMI.Set_MotSpeed(MOT_SPD_VEL, i, cmSMODE_T, COMI.md_spd_vel[i][0], COMI.md_spd_vel[i][1], COMI.md_spd_vel[i][2]);
		if (nRet != BD_GOOD)
		{
			sprintf(st_msg.c_abnormal_msg, "MOT_SPD_VEL Set_Mot_Speed Error [MotNum[%d]]", i);
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
		}

		nRet = COMI.Set_MotSpeed(MOT_SPD_ORG, i, cmSMODE_T, st_motor[i].md_spd_home, COMI.md_spd_vel[i][1]/10, COMI.md_spd_vel[i][2]/10);
		if (nRet != BD_GOOD)
		{
			sprintf(st_msg.c_abnormal_msg, "MOT_SPD_ORG Set_Mot_Speed Error [MotNum[%d]]", i);
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
		}
		
		//nRet = COMI.Set_HomeSetConfig(i, COMI.mn_homecheck_method[i], 0, 2, 1);
		//kwlee 2017.0814
		nRet = COMI.Set_HomeSetConfig(i, COMI.mn_homecheck_method[i], 0, 1000, 10);
		if (nRet != BD_GOOD)
		{
			sprintf(st_msg.c_abnormal_msg, "HomeSpeed Setting Error [MotNum[%d]]", i);
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
		}

		Sleep(100);
	}
}

int CPublic_Function::DoorOpenCheckSpot()
{
	int Ret = CTLBD_RET_GOOD, n_check = 0, i = 0;
	
	return Ret;
}

int CPublic_Function::OnMotor_Go_Check(int n_motor, double d_pos)
{
	CString str_msg, str_motor;
	
	int n_response, nRet = CTL_NO;
	
	CDialog_Select	select_dlg;
	
	str_motor = OnGet_MotorName(n_motor);
	
	str_msg.Format("%s Motor, Move value is %3.3f, Go?",str_motor, (float)d_pos);
	
	st_msg.mstr_confirm_msg = _T(str_msg);
	
	n_response = select_dlg.DoModal();
	
	if (n_response == IDOK)
	{
		nRet = CTL_YES;
	}
	else
	{
		nRet = CTL_NO;
	}
	
	return nRet;
}

int CPublic_Function::On_Encoder_Check(double d_pos) 
{
	int nRet = CTL_NO;

	CDialog_Select select_dlg;
	CString str_msg;
	int n_response;

	//str_msg.Format("Now Reading Value is %ld, Setting?", (long)d_pos);
	str_msg.Format("Now Reading Value is %.3f, Setting?", d_pos);
	st_msg.mstr_confirm_msg = _T(str_msg);

	n_response = select_dlg.DoModal();
	if (n_response == IDOK)
	{
		nRet = CTL_YES;
	}
	else
	{
		nRet = CTL_NO;
	}
	return nRet;
}

//==================================================================//
// I/O ���� �̴ϼ� �۾�
//==================================================================//
int CPublic_Function::OnInit_IOBoard() 
{
	if (FAS_IO.mn_simulation_mode == 1 ||
		st_handler.mn_virtual_mode == 1)
	{
		// �ùķ��̼� ��峪 ���� ����� ��쿡�� ������ �Ϸ� �÷��� ������
		st_handler.mn_io_board_initial = TRUE;
		return CTLBD_RET_GOOD;
	}
	
	INT pMasterNo;
	int nRet = FAS_IO.Set_IO_BoardOpen(&pMasterNo, START_NOTHING);  //START_LOADDATA);  //START_NOTHING);  //START_CLEAR);
	if (nRet != STATUS_SUCCESS)
	{
		FAS_IO.m_nflag_IOBrdInit = BD_ERROR;
		return CTLBD_RET_ERROR;
	}

	int i=0;
	for(i=0; i<MAX_IOBRD_NUM_; i++)
	{
		// PORT�� ����� ��� SLAVE I/O ��� �ڵ� ENABLE ��Ŵ
		nRet = FAS_IO.Set_IO_PortAutoEnable(0, i);
		if (nRet != BD_GOOD)
		{
			if (st_handler.cwnd_list != NULL)
			{
				sprintf(st_msg.c_abnormal_msg, "#0 Card, %d Port, not enable", i);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
			}
			st_handler.mn_io_board_initial = FALSE;

			FAS_IO.m_nflag_IOBrdInit = BD_ERROR;
			return CTLBD_RET_ERROR;
		}
	}

	int j=0, k=0;
	for(i=0; i<MAX_IOBRD_NUM_; i++)
	{
		for(j=0; j<8; j++)
		{
			// SLAVE I/O ��� ���� ENABLE/DISABLE ����
			nRet = FAS_IO.Set_IO_SlaveEnable(0, i, j, TRUE);
			if (nRet != BD_GOOD)
			{
				if (st_handler.cwnd_list != NULL)
				{
					sprintf(st_msg.c_abnormal_msg, "#0 Card, Port%d, Slave #%d, not enable", i, j);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
				}

				// for() ���� �ٽ� �����ϰ� �Ǹ� [++] ���� ����Ǳ� ������ [-1]�� ������
				// �׷��� �ٽ� ȣ��Ǹ� 0���� ������ �� �ְ� �Ǳ� ����
				i = -1;
				j = -1;

				// ��õ� Ƚ���� 3ȸ�� ����
				if (k<3)
				{
					k++;
				}
				else
				{
					st_handler.mn_io_board_initial = FALSE;
					
					FAS_IO.m_nflag_IOBrdInit = BD_ERROR;
					return CTLBD_RET_ERROR;
				}

				continue;
			}

			if (st_handler.cwnd_list != NULL)
			{
				sprintf(st_msg.c_normal_msg, "#0 Card, Port %d, Slave #%d, enable success", i, j);
				st_handler.cwnd_list->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);
			}
			Sleep(100);
		}
	}

	INT HSSI_speed = 0;
	for(i=0; i<MAX_IOBRD_NUM_; i++)
	{
		//nRet = FAS_IO.Set_IO_HSSISpeed(0, i, PORT_SPEED_10);
		nRet = FAS_IO.Set_IO_HSSISpeed(0, i, PORT_SPEED_05);
		nRet = FAS_IO.Get_IO_HSSISpeed(0, i, &HSSI_speed);
	}

	FAS_IO.Set_IO_DefineWord(0, 0, 0, 0x00ff); // 0�� Master, 0�� ��Ʈ, 0�� �����̺� , 16bit ����
	FAS_IO.Set_IO_DefineWord(0, 0, 1, 0x000f); // 0�� Master, 0�� ��Ʈ, 1�� �����̺� , 16bit ����
	FAS_IO.Set_IO_DefineWord(0, 0, 2, 0x00ff); // 0�� Master, 0�� ��Ʈ, 2�� �����̺� , 16bit ����
	FAS_IO.Set_IO_DefineWord(0, 0, 3, 0x007f); // 0�� Master, 0�� ��Ʈ, 3�� �����̺� , 16bit ����
	FAS_IO.Set_IO_DefineWord(0, 0, 4, 0x00ff); // 0�� Master, 0�� ��Ʈ, 4�� �����̺� , 16bit ����
	FAS_IO.Set_IO_DefineWord(0, 0, 5, 0x003f); // 0�� Master, 0�� ��Ʈ, 5�� �����̺� , 16bit ����
	FAS_IO.Set_IO_DefineWord(0, 0, 6, 0x000f); // 0�� Master, 0�� ��Ʈ, 6�� �����̺� , 16bit ����
	FAS_IO.Set_IO_DefineWord(0, 0, 7, 0x007f); // 0�� Master, 0�� ��Ʈ, 7�� �����̺� , 16bit ����
	
	FAS_IO.Set_IO_DefineWord(0, 1, 0, 0x01ff); // 0�� Master, 1�� ��Ʈ, 0�� �����̺� , 16bit ����
	FAS_IO.Set_IO_DefineWord(0, 1, 1, 0x000f); // 0�� Master, 1�� ��Ʈ, 1�� �����̺� , 16bit ����
	FAS_IO.Set_IO_DefineWord(0, 1, 2, 0x00ff); // 0�� Master, 1�� ��Ʈ, 2�� �����̺� , 16bit ����
	FAS_IO.Set_IO_DefineWord(0, 1, 3, 0xffff); // 0�� Master, 1�� ��Ʈ, 2�� �����̺� , 16bit ����

	st_handler.mn_io_board_initial = TRUE;
	
	FAS_IO.m_nflag_IOBrdInit = BD_YES;
	return CTLBD_RET_GOOD;
}

//==================================================================//
// ��� ���� ����
//==================================================================//
int CPublic_Function :: Handler_Recovery_Data(int nzDebug, int nzSite, int nzMode)
{
	FILE *fp;
	int nExistFile, nFuncRet = RET_GOOD;
	CString sFileName, sFolderName;
	
	CString sDebug_FileName, sTimeName; //091206 �߰� 
	COleDateTime mtime_cur;						// ���� �ð� ���� ����
	int nYear, nMonth, nDay, nHour, nMinite;
	
	mtime_cur = COleDateTime::GetCurrentTime(); 
	nYear   = mtime_cur.GetYear();  
	nMonth  = mtime_cur.GetMonth();  
    nDay    = mtime_cur.GetDay(); 
	nHour   = mtime_cur.GetHour(); 
	nMinite = mtime_cur.GetMinute(); 
	sTimeName.Format("%04d%02d%02d%02d%02d_", nYear, nMonth, nDay, nHour, nMinite);
	sFolderName.Format("%04d%02d%02d%02d%02d", nYear, nMonth, nDay, nHour, nMinite);

	sDebug_FileName = PGM_BASE_ + "Step\\" + sFolderName + "\\";

	switch(nzSite)
	{
//==================================================================//
// // Variable Data ����/�ε�
//==================================================================//
	case 0:
		if (nzDebug == 1)
		{
			CreateFolder(sDebug_FileName);
			
			//sDebug_FileName += "st_test.dat";
			sFileName = sDebug_FileName;
		}
		else
		{
			//sFileName = PGM_BASE_ + "RunData\\st_test.dat";	
		}
		
		//file write
		if (nzMode == DATA_WRITE)
		{
			if((fp=fopen(sFileName,"wb")) == NULL)
			{			
				if (st_handler.cwnd_list != NULL)
				{
					sprintf(st_msg.c_abnormal_msg, "Fail in Creating [%s] FILE.", sFileName);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // ���� ���� ��� ��û
				}				
				nFuncRet = RET_ERROR;
				break;
			}
			// ����
			else
			{
				//fwrite(&st_test, sizeof(st_test_param), 1, fp);
				if (ferror(fp)) 
				{
					if (st_handler.cwnd_list != NULL)
					{
						sprintf(st_msg.c_abnormal_msg, "[%s] FILE Write Error.", sFileName);
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // ���� ���� ��� ��û
					}
					clearerr(fp);
					nFuncRet = RET_ERROR;
					break;
				}	
				fclose(fp);
			}		
		}
		//file read
		else if(nzMode == DATA_READ)
		{
			nExistFile = access(sFileName, 0);	
			if (nExistFile)
			{
				if (st_handler.cwnd_list != NULL)
				{
					sprintf(st_msg.c_abnormal_msg, "[%s] file is not exist.", sFileName);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // ���� ���� ��� ��û
				}
				nFuncRet = RET_ERROR;
				break;
			}
			
			if((fp=fopen(sFileName,"rb")) == NULL)
			{			
				if (st_handler.cwnd_list != NULL)
				{
					sprintf(st_msg.c_abnormal_msg, "Fail in Creating [%s] FILE.", sFileName);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // ���� ���� ��� ��û
				}				
				nFuncRet = RET_ERROR;
				break;
			}
			//����
			else
			{
				//fread(&st_test, sizeof(st_test_param), 1, fp);
				if (ferror(fp)) 
				{
					if (st_handler.cwnd_list != NULL)
					{
						sprintf(st_msg.c_abnormal_msg, "[%s] FILE Read Error.", sFileName);
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // ���� ���� ��� ��û
					}
					clearerr(fp);
					nFuncRet = RET_ERROR;
					break;
				}	
				fclose(fp);
			}			
		}
		break;

	case 1:
		if (nzDebug == 1)
		{
			CreateFolder(sDebug_FileName);
			
			sDebug_FileName += "stSync.dat";
			sFileName = sDebug_FileName;
		}
		else
		{
			sFileName = PGM_BASE_ + "RunData\\stSync.dat";	
		}
		
		//file write
		if (nzMode == DATA_WRITE)
		{
			if((fp=fopen(sFileName,"wb")) == NULL)
			{			
				if (st_handler.cwnd_list != NULL)
				{
					sprintf(st_msg.c_abnormal_msg, "Fail in Creating [%s] FILE.", sFileName);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // ���� ���� ��� ��û
				}				
				nFuncRet = RET_ERROR;
				break;
			}
			// ����
			else
			{
				fwrite(&stSync, sizeof(tagSYNC_PARAM), 1, fp);
				if (ferror(fp)) 
				{
					if (st_handler.cwnd_list != NULL)
					{
						sprintf(st_msg.c_abnormal_msg, "[%s] FILE Write Error.", sFileName);
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // ���� ���� ��� ��û
					}
					clearerr(fp);
					nFuncRet = RET_ERROR;
					break;
				}	
				fclose(fp);
			}		
		}
		//file read
		else if(nzMode == DATA_READ)
		{
			nExistFile = access(sFileName, 0);	
			if (nExistFile)
			{
				if (st_handler.cwnd_list != NULL)
				{
					sprintf(st_msg.c_abnormal_msg, "[%s] file is not exist.", sFileName);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // ���� ���� ��� ��û
				}
				nFuncRet = RET_ERROR;
				break;
			}
			
			if((fp=fopen(sFileName,"rb")) == NULL)
			{			
				if (st_handler.cwnd_list != NULL)
				{
					sprintf(st_msg.c_abnormal_msg, "Fail in Creating [%s] FILE.", sFileName);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // ���� ���� ��� ��û
				}				
				nFuncRet = RET_ERROR;
				break;
			}
			//����
			else
			{
				fread(&stSync, sizeof(tagSYNC_PARAM), 1, fp);
				if (ferror(fp)) 
				{
					if (st_handler.cwnd_list != NULL)
					{
						sprintf(st_msg.c_abnormal_msg, "[%s] FILE Read Error.", sFileName);
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // ���� ���� ��� ��û
					}
					clearerr(fp);
					nFuncRet = RET_ERROR;
					break;
				}	
				fclose(fp);
			}			
		}
		break;
//==================================================================//

//==================================================================//
// Thread ���� ���� ����/�ε�
//==================================================================//
	// Box Clamp
	case 2:
		if(nzDebug == 1)
		{
			sDebug_FileName += "reco_SeqBoxClamp.dat";
			sFileName = sDebug_FileName;
		}
		else
		{
			sFileName = PGM_BASE_ + "RunData\\reco_SeqBoxClamp.dat";
		}
		
		//file write
		if(nzMode == DATA_WRITE)
		{
			if((fp=fopen(sFileName,"wb")) == NULL)
			{			
				if (st_handler.cwnd_list != NULL)
				{
					sprintf(st_msg.c_abnormal_msg, "Fail in Creating [%s] FILE.", sFileName);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // ���� ���� ��� ��û
				}				
				nFuncRet = RET_ERROR;
				break;
			}
			//���� 
			else
			{
				reco_SeqBoxClamp.nStep_Init = SeqBoxClamp.m_nStep_Init;
				reco_SeqBoxClamp.nStep_Run = SeqBoxClamp.m_nStep_Run;
								
				
				fwrite(&reco_SeqBoxClamp, sizeof(st_SeqBoxClamp_param), 1, fp);
				if (ferror(fp)) 
				{
					if (st_handler.cwnd_list != NULL)
					{
						sprintf(st_msg.c_abnormal_msg, "[%s] FILE Write Error.", sFileName);
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // ���� ���� ��� ��û
					}
					clearerr(fp);
					nFuncRet = RET_ERROR;
					break;
				}
				fclose(fp);
			}				 
		}
		else if(nzMode == DATA_READ) //file read
		{
			nExistFile = access(sFileName, 0);	
			if (nExistFile)
			{
				sprintf(st_msg.c_abnormal_msg, "[%s] file is not exist.", sFileName);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // ���� ���� ��� ��û
				nFuncRet = RET_ERROR;
				break;
			}

			if((fp=fopen(sFileName,"rb")) == NULL)
			{			
				if (st_handler.cwnd_list != NULL)
				{
					sprintf(st_msg.c_abnormal_msg, "Fail in Creating [%s] FILE.", sFileName);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // ���� ���� ��� ��û
				}				
				nFuncRet = RET_ERROR;
				break;
			}
			else
			{ //���� 
				fread(&reco_SeqBoxClamp, sizeof(st_SeqBoxClamp_param), 1, fp);
				if (ferror(fp)) 
				{
					if (st_handler.cwnd_list != NULL)
					{
						sprintf(st_msg.c_abnormal_msg, "[%s] FILE Read Error.", sFileName);
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // ���� ���� ��� ��û
					}
					clearerr(fp);
					nFuncRet = RET_ERROR;
					break;
				}	
				fclose(fp);
				
				SeqBoxClamp.m_nStep_Init = reco_SeqBoxClamp.nStep_Init;
				SeqBoxClamp.m_nStep_Run = reco_SeqBoxClamp.nStep_Run;
			}	
		}
		break;

	// Box Lifter
	case 3:
		if(nzDebug == 1)
		{
			sDebug_FileName += "reco_SeqBoxLifter.dat";
			sFileName = sDebug_FileName;
		}
		else
		{
			sFileName = PGM_BASE_ + "RunData\\reco_SeqBoxLifter.dat";
		}
		
		//file write
		if(nzMode == DATA_WRITE)
		{
			if((fp=fopen(sFileName,"wb")) == NULL)
			{			
				if (st_handler.cwnd_list != NULL)
				{
					sprintf(st_msg.c_abnormal_msg, "Fail in Creating [%s] FILE.", sFileName);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // ���� ���� ��� ��û
				}				
				nFuncRet = RET_ERROR;
				break;
			}
			//���� 
			else
			{
				reco_SeqBoxLifter.nStep_Init = SeqBoxLifter.m_nStep_Init;
				reco_SeqBoxLifter.nStep_Run = SeqBoxLifter.m_nStep_Run;
								
				
				fwrite(&reco_SeqBoxLifter, sizeof(st_SeqBoxLifter_param), 1, fp);
				if (ferror(fp)) 
				{
					if (st_handler.cwnd_list != NULL)
					{
						sprintf(st_msg.c_abnormal_msg, "[%s] FILE Write Error.", sFileName);
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // ���� ���� ��� ��û
					}
					clearerr(fp);
					nFuncRet = RET_ERROR;
					break;
				}
				fclose(fp);
			}				 
		}
		else if(nzMode == DATA_READ) //file read
		{
			nExistFile = access(sFileName, 0);	
			if (nExistFile)
			{
				sprintf(st_msg.c_abnormal_msg, "[%s] file is not exist.", sFileName);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // ���� ���� ��� ��û
				nFuncRet = RET_ERROR;
				break;
			}
			
			if((fp=fopen(sFileName,"rb")) == NULL)
			{			
				if (st_handler.cwnd_list != NULL)
				{
					sprintf(st_msg.c_abnormal_msg, "Fail in Creating [%s] FILE.", sFileName);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // ���� ���� ��� ��û
				}				
				nFuncRet = RET_ERROR;
				break;
			}
			else
			{ //���� 
				fread(&reco_SeqBoxLifter, sizeof(st_SeqBoxLifter_param), 1, fp);
				if (ferror(fp)) 
				{
					if (st_handler.cwnd_list != NULL)
					{
						sprintf(st_msg.c_abnormal_msg, "[%s] FILE Read Error.", sFileName);
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // ���� ���� ��� ��û
					}
					clearerr(fp);
					nFuncRet = RET_ERROR;
					break;
				}	
				fclose(fp);
				
				SeqBoxLifter.m_nStep_Init = reco_SeqBoxLifter.nStep_Init;
				SeqBoxLifter.m_nStep_Run = reco_SeqBoxLifter.nStep_Run;
				
			}	
		}
		break;

	// Buffer Align Conv
	case 4:
		if(nzDebug == 1)
		{
			sDebug_FileName += "reco_SeqBuffAlignCV.dat";
			sFileName = sDebug_FileName;
		}
		else
		{
			sFileName = PGM_BASE_ + "RunData\\reco_SeqBuffAlignCV.dat";
		}
		
		//file write
		if(nzMode == DATA_WRITE)
		{
			if((fp=fopen(sFileName,"wb")) == NULL)
			{			
				if (st_handler.cwnd_list != NULL)
				{
					sprintf(st_msg.c_abnormal_msg, "Fail in Creating [%s] FILE.", sFileName);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // ���� ���� ��� ��û
				}				
				nFuncRet = RET_ERROR;
				break;
			}
			//���� 
			else
			{
				reco_SeqBuffAlignCV.nStep_Init = SeqBufferAlignConv.m_nStep_Init;
				reco_SeqBuffAlignCV.nStep_Run = SeqBufferAlignConv.m_nStep_Run;
				
				
				
				fwrite(&reco_SeqBuffAlignCV, sizeof(st_SeqBufferAlignConv_param), 1, fp);
				if (ferror(fp)) 
				{
					if (st_handler.cwnd_list != NULL)
					{
						sprintf(st_msg.c_abnormal_msg, "[%s] FILE Write Error.", sFileName);
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // ���� ���� ��� ��û
					}
					clearerr(fp);
					nFuncRet = RET_ERROR;
					break;
				}
				fclose(fp);
			}				 
		}
		else if(nzMode == DATA_READ) //file read
		{
			nExistFile = access(sFileName, 0);	
			if (nExistFile)
			{
				sprintf(st_msg.c_abnormal_msg, "[%s] file is not exist.", sFileName);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // ���� ���� ��� ��û
				nFuncRet = RET_ERROR;
				break;
			}
			
			if((fp=fopen(sFileName,"rb")) == NULL)
			{			
				if (st_handler.cwnd_list != NULL)
				{
					sprintf(st_msg.c_abnormal_msg, "Fail in Creating [%s] FILE.", sFileName);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // ���� ���� ��� ��û
				}				
				nFuncRet = RET_ERROR;
				break;
			}
			else
			{ //���� 
				fread(&reco_SeqBuffAlignCV, sizeof(st_SeqBufferAlignConv_param), 1, fp);
				if (ferror(fp)) 
				{
					if (st_handler.cwnd_list != NULL)
					{
						sprintf(st_msg.c_abnormal_msg, "[%s] FILE Read Error.", sFileName);
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // ���� ���� ��� ��û
					}
					clearerr(fp);
					nFuncRet = RET_ERROR;
					break;
				}	
				fclose(fp);
				
				SeqBufferAlignConv.m_nStep_Init = reco_SeqBuffAlignCV.nStep_Init;
				SeqBufferAlignConv.m_nStep_Run = reco_SeqBuffAlignCV.nStep_Run;
				
			}	
		}
		break;

	// Loading Conv
	case 5:
		if(nzDebug == 1)
		{
			sDebug_FileName += "reco_SeqLoadingCV.dat";
			sFileName = sDebug_FileName;
		}
		else
		{
			sFileName = PGM_BASE_ + "RunData\\reco_SeqLoadingCV.dat";
		}
		
		//file write
		if(nzMode == DATA_WRITE)
		{
			if((fp=fopen(sFileName,"wb")) == NULL)
			{			
				if (st_handler.cwnd_list != NULL)
				{
					sprintf(st_msg.c_abnormal_msg, "Fail in Creating [%s] FILE.", sFileName);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // ���� ���� ��� ��û
				}				
				nFuncRet = RET_ERROR;
				break;
			}
			//���� 
			else
			{
				reco_SeqLoadingCV.nStep_Init = SeqLoadingConv.m_nStep_Init;
				reco_SeqLoadingCV.nStep_Run = SeqLoadingConv.m_nStep_Run;
				
				
				
				fwrite(&reco_SeqLoadingCV, sizeof(st_SeqLoadingConv_param), 1, fp);
				if (ferror(fp)) 
				{
					if (st_handler.cwnd_list != NULL)
					{
						sprintf(st_msg.c_abnormal_msg, "[%s] FILE Write Error.", sFileName);
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // ���� ���� ��� ��û
					}
					clearerr(fp);
					nFuncRet = RET_ERROR;
					break;
				}
				fclose(fp);
			}				 
		}
		else if(nzMode == DATA_READ) //file read
		{
			nExistFile = access(sFileName, 0);	
			if (nExistFile)
			{
				sprintf(st_msg.c_abnormal_msg, "[%s] file is not exist.", sFileName);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // ���� ���� ��� ��û
				nFuncRet = RET_ERROR;
				break;
			}
			
			if((fp=fopen(sFileName,"rb")) == NULL)
			{			
				if (st_handler.cwnd_list != NULL)
				{
					sprintf(st_msg.c_abnormal_msg, "Fail in Creating [%s] FILE.", sFileName);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // ���� ���� ��� ��û
				}				
				nFuncRet = RET_ERROR;
				break;
			}
			else
			{ //���� 
				fread(&reco_SeqLoadingCV, sizeof(st_SeqLoadingConv_param), 1, fp);
				if (ferror(fp)) 
				{
					if (st_handler.cwnd_list != NULL)
					{
						sprintf(st_msg.c_abnormal_msg, "[%s] FILE Read Error.", sFileName);
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // ���� ���� ��� ��û
					}
					clearerr(fp);
					nFuncRet = RET_ERROR;
					break;
				}	
				fclose(fp);
				
				SeqLoadingConv.m_nStep_Init = reco_SeqLoadingCV.nStep_Init;
				SeqLoadingConv.m_nStep_Run = reco_SeqLoadingCV.nStep_Run;
				
			}	
		}
		break;

	// XYZ Robot
	case 6:
		if(nzDebug == 1)
		{
			sDebug_FileName += "reco_SeqXYZRBT.dat";
			sFileName = sDebug_FileName;
		}
		else
		{
			sFileName = PGM_BASE_ + "RunData\\reco_SeqXYZRBT.dat";
		}
		
		//file write
		if(nzMode == DATA_WRITE)
		{
			if((fp=fopen(sFileName,"wb")) == NULL)
			{			
				if (st_handler.cwnd_list != NULL)
				{
					sprintf(st_msg.c_abnormal_msg, "Fail in Creating [%s] FILE.", sFileName);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // ���� ���� ��� ��û
				}				
				nFuncRet = RET_ERROR;
				break;
			}
			//���� 
			else
			{
				reco_SeqXYZRBT.nStep_Init = SeqXYZRobot.m_nStep_Init;
				reco_SeqXYZRBT.nStep_Run = SeqXYZRobot.m_nStep_Run;
				
				
				
				fwrite(&reco_SeqXYZRBT, sizeof(st_SeqXYZRobot_param), 1, fp);
				if (ferror(fp)) 
				{
					if (st_handler.cwnd_list != NULL)
					{
						sprintf(st_msg.c_abnormal_msg, "[%s] FILE Write Error.", sFileName);
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // ���� ���� ��� ��û
					}
					clearerr(fp);
					nFuncRet = RET_ERROR;
					break;
				}
				fclose(fp);
			}				 
		}
		else if(nzMode == DATA_READ) //file read
		{
			nExistFile = access(sFileName, 0);	
			if (nExistFile)
			{
				sprintf(st_msg.c_abnormal_msg, "[%s] file is not exist.", sFileName);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // ���� ���� ��� ��û
				nFuncRet = RET_ERROR;
				break;
			}
			
			if((fp=fopen(sFileName,"rb")) == NULL)
			{			
				if (st_handler.cwnd_list != NULL)
				{
					sprintf(st_msg.c_abnormal_msg, "Fail in Creating [%s] FILE.", sFileName);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // ���� ���� ��� ��û
				}				
				nFuncRet = RET_ERROR;
				break;
			}
			else
			{ //���� 
				fread(&reco_SeqXYZRBT, sizeof(st_SeqXYZRobot_param), 1, fp);
				if (ferror(fp)) 
				{
					if (st_handler.cwnd_list != NULL)
					{
						sprintf(st_msg.c_abnormal_msg, "[%s] FILE Read Error.", sFileName);
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // ���� ���� ��� ��û
					}
					clearerr(fp);
					nFuncRet = RET_ERROR;
					break;
				}	
				fclose(fp);
				
				SeqXYZRobot.m_nStep_Init = reco_SeqXYZRBT.nStep_Init;
				SeqXYZRobot.m_nStep_Run = reco_SeqXYZRBT.nStep_Run;
				
			}	
		}
		break;
//==================================================================//

	}

	return nFuncRet;
}

CString CPublic_Function::OnNetworkBodyAnalysis(CString strBody, CString strFind, int nOpt)
{
	CString strFuncRet;
	int n_title, n_equal, n_end, n_length, n_qw = 0;    
	
	n_title =	strBody.Find(strFind, 0);							// TITLE�� ��ġ�� ã�´�.
	n_equal =	strBody.Find("=", n_title + 1);						// �ش� item�� �� ��ġ�� ã�´�.
	
	if (n_title == -1 || n_equal == -1)		return "NULL";
	
	if (nOpt == 0)
	{
		n_end =		strBody.Find(" ", n_title + 1);					// �ش� item�� �� ��ġ�� ã�´�.
		n_length =	n_end - n_equal - 1;							// DATA�� ���� ���.
		strFuncRet = strBody.Mid(n_equal + 1, n_length);
	}
	else if (nOpt == 7)
	{
		strFuncRet = strBody.Mid(n_title + 5);
	}
	else if (nOpt == 2)
	{
		n_end =		strBody.Find("=", n_title + 1);					// �ش� item�� �� ��ġ�� ã�´�.						// DATA�� ���� ���.
		strFuncRet = strBody.Mid(n_end + 1);
	}
	else if (nOpt == 3)
	{
		n_end =		strBody.Find("=", n_title + 1);					// �ش� item�� �� ��ġ�� ã�´�.						// DATA�� ���� ���.
		strFuncRet = strBody.Mid(n_end + 1, 1);
	}
	else if (nOpt == 4)
	{
		n_qw =		strBody.Find("((", n_title + 1);					// �ش� item�� �� ��ġ�� ã�´�.
		n_end =		strBody.Find("))", n_title + 1);					// �ش� item�� �� ��ġ�� ã�´�.
		n_length =	n_end - n_qw - 1;								// DATA�� ���� ���.
		strFuncRet = strBody.Mid(n_qw + 2, n_length - 1);
	}
	else if (nOpt == 5)
	{
		n_qw =		strBody.Find("FAIL_REGNUMBER", n_title + 22);					// �ش� item�� �� ��ġ�� ã�´�.
		n_equal =	strBody.Find("=", n_qw + 1);	
		n_end =		strBody.Find(" ", n_qw + 1);					// �ش� item�� �� ��ġ�� ã�´�.
		n_length =	n_end - n_equal - 1;							// DATA�� ���� ���.
		strFuncRet = strBody.Mid(n_equal + 1, n_length);
	}
	else if (nOpt == 6)
	{
		n_qw =		strBody.Find("ID", n_title + 6);					// �ش� item�� �� ��ġ�� ã�´�.
		n_equal =	strBody.Find("=", n_qw + 1);	
		n_end =		strBody.Find(" ", n_qw + 1);					// �ش� item�� �� ��ġ�� ã�´�.
		n_length =	n_end - n_equal - 1;							// DATA�� ���� ���.
		strFuncRet = strBody.Mid(n_equal + 1, n_length);
	}
	else if ( nOpt == 8)
	{
		n_qw =		strBody.Find("\"", n_equal + 1 );
		n_end =		strBody.Find("\"", n_equal + 2 );
		n_length =	n_end - n_qw - 1;
		strFuncRet = strBody.Mid(n_qw + 1, n_length);
	}
	else
	{
		n_qw =		strBody.Find("(", n_title + 1);					// �ش� item�� �� ��ġ�� ã�´�.
		n_end =		strBody.Find(")", n_title + 1);					// �ش� item�� �� ��ġ�� ã�´�.
		n_length =	n_end - n_qw - 1;								// DATA�� ���� ���.
		strFuncRet = strBody.Mid(n_qw + 1, n_length);
	}
	
	strFuncRet.MakeUpper();
	strFuncRet.TrimLeft(' ');
	strFuncRet.TrimRight(' ');

	return strFuncRet;
}

CString CPublic_Function::OnDeviceInfoAnalysis(CString strBody, int nInfo)
{
	CString strFuncRet = "NONE";
	int nLength;

	int n_start[5], n_check[5], n_length[5];    

	nLength = strBody.GetLength();
	if (nLength < 23)		return "FAIL";		// ���̰� 22���� ª���� FAIL�̴�.

	n_start[0] = 0;
	n_check[0] = strBody.Find("-", n_start[0]);

	if (n_check[0] != -1)
	{
		n_length[0] = n_check[0];
	}
	else
	{
		n_length[0] = 10;
		n_check[0] = 9;
	}

	n_start[1] = n_check[0] + 1;
	n_check[1] = strBody.Find("_", n_start[1]);
	n_length[1] = n_check[1] - n_start[1];

	n_start[2] = n_check[1] + 1;
	n_check[2] = strBody.Find("_", n_start[2]);
	n_length[2] = n_check[2] - n_start[2];

	n_start[3] = n_check[2] + 1;
	n_check[3] = strBody.Find("_", n_start[3]);
	n_length[3] = n_check[3] - n_start[3];

	n_start[4] = n_check[3] + 1;
	n_check[4] = strBody.Find("_", n_start[4]);
	n_length[4] = n_check[4] - n_start[4];

	if (nInfo < 4)
	{
		strFuncRet = strBody.Mid(n_start[nInfo], n_length[nInfo]);
	}
	else
	{
		strFuncRet = strBody.Mid(n_start[nInfo]);
	}
	return strFuncRet;
}

int CPublic_Function::FileSizeCheck(CString FileName, long size, int n_check)
{
	CFileFind finder;
	long Len=0, flag=0;
	BOOL bContinue;
	
	if (bContinue = finder.FindFile(FileName))
	{	
		if (n_check == CTL_YES)			// Size�� üũ�ϴ°��̸�...
		{
			finder.FindFile(FileName);
			finder.FindNextFile();
			finder.GetFileName();
			Len = finder.GetLength();
			if (Len > size)
			{
				flag = 1;
			}
			finder.Close();	
		}
		else						// Size�� Ȯ���ϴ°��̸�...
		{
			finder.FindFile(FileName);
			finder.FindNextFile();
			finder.GetFileName();
			Len = finder.GetLength();
			
			flag = Len;
			
			finder.Close();	
		}
	}
	
	return flag;
}

CString CPublic_Function::GetLocalMacAddress()
{
	NCB Ncb; 
	UCHAR uRetCode; 
	LANA_ENUM lenum; 
	int i; 
	CString str_value; 
	
	memset( &Ncb, 0, sizeof(Ncb) ); 
	Ncb.ncb_command = NCBENUM; 
	Ncb.ncb_buffer = (UCHAR *)&lenum; 
	Ncb.ncb_length = sizeof(lenum); 
	uRetCode = Netbios( &Ncb ); 
	
	for (i=0; i < lenum.length ;i++) 
	{ 
		memset( &Ncb, 0, sizeof(Ncb) ); 
		Ncb.ncb_command = NCBRESET; 
		Ncb.ncb_lana_num = lenum.lana[i]; 
		
		uRetCode = Netbios( &Ncb ); 
		memset( &Ncb, 0, sizeof (Ncb) ); 
		Ncb.ncb_command = NCBASTAT; 
		Ncb.ncb_lana_num = lenum.lana[i]; 
		
		strcpy( (char*)Ncb.ncb_callname, "* " ); 
		Ncb.ncb_buffer = (unsigned char *) &Adapter; 
		Ncb.ncb_length = sizeof(Adapter); 
		
		uRetCode = Netbios( &Ncb ); 
		
		if ( uRetCode == 0 ) 
		{ 
			str_value.Format("%02x-%02x-%02x-%02x-%02x-%02x", 
				Adapter.adapt.adapter_address[0], 
				Adapter.adapt.adapter_address[1], 
				Adapter.adapt.adapter_address[2], 
				Adapter.adapt.adapter_address[3], 
				Adapter.adapt.adapter_address[4], 
				Adapter.adapt.adapter_address[5] ); 
		} 
	} 	
	return str_value;
}

int CPublic_Function::Robot_Reinstatement_Position(int mode)
{
	int Ret = 0, i = 0, n_response = 0;
	int FuncRet = RET_PROCEED;
	int nRet = 0, nRet2 = 0;

	CDialog_Select	select_dlg;
 
	if (mode == 0 && st_work.mn_reinstatement_mode == 0) // ���� �ʱ�ȭ �ĸ� ������ 
	{	// ���� ��ġ ��� 
		for (i = 0; i < MOT_MAXMOTOR; i++)
		{
			st_work.md_reinstatement_Pos[0][i] = COMI.md_cmdpos_backup[i];
		}
								


		st_work.mn_reinstatement_mode = 1; //�ѹ� �����͸� �����Ѵ� 
		reinstatement_Step = 0;

		return RET_GOOD;
	}	 
	else if (mode == 0)
	{
		return RET_GOOD;
	}

	switch (reinstatement_Step)
	{
	case 0:
		if (st_work.mn_reinstatement_mode == 1) // ��ŸƮ Ű�� ������ 2�̴� 
		{
			mn_linearmove_index = 0;			//���� map index 
			ml_AxisCnt = 2;						//������ IndexNum���� ��� ������ ���� ���� �ִ� 4��  
			mlp_AxisNum[0] = M_ROBOT_X;			//������ IndexNum���� ����ϴ� ���� ���� ��ȣ�� ������ 
			mlp_AxisNum[1] = M_ROBOT_Y;  
			mlp_AxisNum[2] = M_ROBOT_Z;  
			
			mdp_PosList[0] = 0;					//�ִ� 4 ����Ʈ�� �ѹ��� �̵������ϴ� 
			mdp_PosList[1] = 0;
			mdp_SpdRatio[0] = 10;				//�ӵ����� [0]:Vel, [1]:Acc, [2]:Dec
			mdp_SpdRatio[1] = 10;
			mdp_SpdRatio[2] = 10;

			reinstatement_Step = 1000;
		}
		break;

	case 1000:
		for (i = 0; i < MOT_MAXMOTOR; i++)
		{
			st_work.md_reinstatement_Pos[1][i] = COMI.Get_MotCurrentPos(i);
		}

		mn_xy_robot_change = CTL_NO;
		mn_multistacker_change = CTL_NO;
		mn_transfer_change = CTL_NO;
		mn_picker_change = CTL_NO;
		
		//����� ��ġ�� ���
		if (st_work.md_reinstatement_Pos[1][M_ROBOT_X] > st_work.md_reinstatement_Pos[0][M_ROBOT_X] + COMI.mn_allow_value[M_ROBOT_X]
			|| st_work.md_reinstatement_Pos[1][M_ROBOT_X] < st_work.md_reinstatement_Pos[0][M_ROBOT_X] - COMI.mn_allow_value[M_ROBOT_X])
		{
			mn_xy_robot_change = CTL_YES;
			sprintf(st_msg.c_abnormal_msg, "Robot X POS [OLD] : %ld -> [NOW] : %ld [GAP] : %ld", (long)st_work.md_reinstatement_Pos[0][M_ROBOT_X], (long)st_work.md_reinstatement_Pos[1][M_ROBOT_X], (long)COMI.mn_allow_value[M_ROBOT_X]);
		}
		else if (st_work.md_reinstatement_Pos[1][M_ROBOT_Y] > st_work.md_reinstatement_Pos[0][M_ROBOT_Y] + COMI.mn_allow_value[M_ROBOT_Y]
			|| st_work.md_reinstatement_Pos[1][M_ROBOT_Y] < st_work.md_reinstatement_Pos[0][M_ROBOT_Y] - COMI.mn_allow_value[M_ROBOT_Y])
		{
			mn_xy_robot_change = CTL_YES;
			sprintf(st_msg.c_abnormal_msg, "Robot Y POS [OLD] : %ld -> [NOW] : %ld [GAP] : %ld", (long)st_work.md_reinstatement_Pos[0][M_ROBOT_Y], (long)st_work.md_reinstatement_Pos[1][M_ROBOT_Y], (long)COMI.mn_allow_value[M_ROBOT_Y]);
		}
		else if (st_work.md_reinstatement_Pos[1][M_ROBOT_Z] > st_work.md_reinstatement_Pos[0][M_ROBOT_Z] + COMI.mn_allow_value[M_ROBOT_Z]
			|| st_work.md_reinstatement_Pos[1][M_ROBOT_Z] < st_work.md_reinstatement_Pos[0][M_ROBOT_Z] - COMI.mn_allow_value[M_ROBOT_Z])
		{
			mn_xy_robot_change = CTL_YES;
			sprintf(st_msg.c_abnormal_msg, "Robot Z POS [OLD] : %ld -> [NOW] : %ld [GAP] : %ld", (long)st_work.md_reinstatement_Pos[0][M_ROBOT_Z], (long)st_work.md_reinstatement_Pos[1][M_ROBOT_Z], (long)COMI.mn_allow_value[M_ROBOT_Z]);
		}
		else if (st_work.md_reinstatement_Pos[1][M_ROBOT_P] > st_work.md_reinstatement_Pos[0][M_ROBOT_P] + COMI.mn_allow_value[M_ROBOT_P]
			|| st_work.md_reinstatement_Pos[1][M_ROBOT_P] < st_work.md_reinstatement_Pos[0][M_ROBOT_P] - COMI.mn_allow_value[M_ROBOT_P])
		{
			mn_xy_robot_change = CTL_YES;
			sprintf(st_msg.c_abnormal_msg, "Robot P POS [OLD] : %ld -> [NOW] : %ld [GAP] : %ld", (long)st_work.md_reinstatement_Pos[0][M_ROBOT_P], (long)st_work.md_reinstatement_Pos[1][M_ROBOT_P], (long)COMI.mn_allow_value[M_ROBOT_P]);
		}
		else if (st_work.md_reinstatement_Pos[1][M_TRAY_TRANSFER_Y] > st_work.md_reinstatement_Pos[0][M_TRAY_TRANSFER_Y] + COMI.mn_allow_value[M_TRAY_TRANSFER_Y]
			|| st_work.md_reinstatement_Pos[1][M_TRAY_TRANSFER_Y] < st_work.md_reinstatement_Pos[0][M_TRAY_TRANSFER_Y] - COMI.mn_allow_value[M_TRAY_TRANSFER_Y])
		{
			mn_transfer_change = CTL_YES;
			sprintf(st_msg.c_abnormal_msg, "TRAY TRANSFER POS [OLD] : %ld -> [NOW] : %ld [GAP] : %ld", (long)st_work.md_reinstatement_Pos[0][M_TRAY_TRANSFER_Y], (long)st_work.md_reinstatement_Pos[1][M_TRAY_TRANSFER_Y], (long)COMI.mn_allow_value[M_TRAY_TRANSFER_Y]);
		}
		else if (st_work.md_reinstatement_Pos[1][M_MULTI_STACKER_Z] > st_work.md_reinstatement_Pos[0][M_MULTI_STACKER_Z] + COMI.mn_allow_value[M_MULTI_STACKER_Z]
			|| st_work.md_reinstatement_Pos[1][M_MULTI_STACKER_Z] < st_work.md_reinstatement_Pos[0][M_MULTI_STACKER_Z] - COMI.mn_allow_value[M_MULTI_STACKER_Z])
		{
			mn_multistacker_change = CTL_YES;
			sprintf(st_msg.c_abnormal_msg, "MULTI STACKER POS [OLD] : %ld -> [NOW] : %ld [GAP] : %ld", (long)st_work.md_reinstatement_Pos[0][M_MULTI_STACKER_Z], (long)st_work.md_reinstatement_Pos[1][M_MULTI_STACKER_Z], (long)COMI.mn_allow_value[M_MULTI_STACKER_Z]);
		}
		else 
		{
			nRet = 0;

			
			
			if (nRet == 0)
			{
				reinstatement_Step = 50000;
			}
			else
			{
				reinstatement_Step = 1100;
			}
			break;
		}
		
		if (st_handler.cwnd_list != NULL)
		{
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
		}

		st_msg.mstr_event_msg[0] = "Recovery Mode.......";
		::PostMessage(st_handler.hWnd, WM_MAIN_EVENT, CTL_YES, 0);

		st_work.mn_reinstatement_mode = 3; //�ʱ�ȭ ������ �Ϸ��ߴ�, Ŭ���� 

		reinstatement_Step = 2000;
		break;
		
	case 50000:
		::PostMessage(st_handler.hWnd, WM_MAIN_EVENT, CTL_NO, 0);
		
		reinstatement_Step = 0;
		st_handler.mn_reinstatement = CTL_NO;			// ���� ������ �ƴϴ�.
		st_work.mn_reinstatement_mode = 0;				// �ʱ�ȭ ������ �Ϸ��ߴ�, Ŭ���� 		
		FuncRet = RET_GOOD;
		break;
	}
 	return FuncRet;
}

void CPublic_Function::Robot_BackData_Set(int site, int type, int xpos, int ypos, int noption)
{
	st_work.n_back_site = site;
	st_work.n_back_type = type;
	st_work.n_back_x_pos = xpos;
	st_work.n_back_y_pos = ypos;
	st_work.n_back_picker = noption;
//	st_work.n_back_z_movepos = P_ROBOT_Z_SAFETY;

	st_work.n_back_ms_pos = ypos;
}

void CPublic_Function::Robot_BackData_TT_Set(int movepos)
{
	st_work.n_back_tt_movepos = movepos;
}

void CPublic_Function::Robot_BackData_Z_Set(int movepos)
{
	st_work.n_back_z_movepos = movepos;
}

//==================================================================//
// ������ ���� ���� TRACE
//==================================================================//
void CPublic_Function::OnTrace_ThreadStep(int nzPos, int nzStep)
{
	// ������ ���� ������ �����ϸ� ���� ���Ͻ�Ŵ
	// - ������ ���� ������ �����ؼ� ���� �ʿ� ���� ������
	if (nzStep == st_work.mn_prev_step[nzPos])
	{
		return;
	}

	st_work.mn_prev_step[nzPos] = nzStep;
	CString sStepList;
	if (0)
	{
		sStepList.Format("STEP[%02d] %05d -> %05d", nzPos, st_other.n_prev_step[nzPos], nzStep);
		cLOG.OnLogEvent(LOG_PGM_, sStepList);
	}

	st_work.mn_prev_step[nzPos] = nzStep;
	StepChangeTime[nzPos] = CTime::GetCurrentTime();	 
	st_work.mstr_last_change_date[nzPos] = StepChangeTime[nzPos].Format("%y/%m/%d %H:%M:%S");	
	
	if (st_work.mstr_history_step[nzPos].GetLength() == 120 ||
		st_work.mstr_history_step[nzPos].GetLength() == 276)
	{
		str_step_temp[nzPos].Format("%05d|", nzStep);
		st_work.mstr_history_step[nzPos] += str_step_temp[nzPos];
		st_work.mstr_history_step[nzPos] += "\r\n|                          |";
	}
	else if (st_work.mstr_history_step[nzPos].GetLength() == 432)
	{
		str_step_temp[nzPos].Format("%05d|", nzStep);
		st_work.mstr_history_step[nzPos] += str_step_temp[nzPos];

		st_work.mstr_history_step_back_up[nzPos] = st_work.mstr_history_step[nzPos];
		st_work.mstr_history_step[nzPos] = "";
	}
	else
	{
		str_step_temp[nzPos].Format("%05d,", nzStep);
		st_work.mstr_history_step[nzPos] += str_step_temp[nzPos];
	}
}

void CPublic_Function::TextAndLine(int dir, CString str_sdata, CString str_data, FILE *fp)
{
	CString line, str_temp_data_temp, str_content;
	
	line = "";
	
	fprintf(fp,"%s", str_sdata);
	
	switch (dir)
	{
	case 6 :
		str_temp_data_temp.Format("%6s", str_data);
		line += str_temp_data_temp;
		line += " |";
		fprintf(fp,"%s\r\n", line);
		str_content = "+----------------------------------------+-------+";
		break;
		
	case 14:
		str_temp_data_temp.Format("%14s", str_data);
		line += str_temp_data_temp;
		line += " |";
		fprintf(fp,"%s\r\n", line);
		str_content = "+----------------------------------------+---------------+";   // +----------------------------------------+----------------------------------+
		break;
		
	case 15:
		str_temp_data_temp.Format("%33s", str_data);
		line += str_temp_data_temp;
		line += " |";
		fprintf(fp,"%s\r\n", line);
		str_content = "+----------------------------------------+----------------------------------+";
		break;
		
	case 24:
		str_temp_data_temp.Format("%24s", str_data);
		line += str_temp_data_temp;
		line += " |";
		fprintf(fp,"%s\r\n", line);
		str_content = "+----------------------------------------+-------------------------+";
		break;
		
	case 140:
		str_temp_data_temp.Format("%14s", str_data);
		line += str_temp_data_temp;
		line += " |";
		fprintf(fp,"%s", line);
		break;
		
	case 30:
		str_temp_data_temp.Format("%30s", str_data);
		line += str_temp_data_temp;
		line += " |";
		fprintf(fp,"%s\r\n", line);
		str_content = "+----------------------------------------+-------------------------------+";
		break;
		
	case 131:
		str_temp_data_temp.Format("%30s", str_data);
		line += str_temp_data_temp;
		line += " |";
		fprintf(fp,"%s\r\n", line);
		str_content = "+----------------------------------------+-----------------------------------------------------------------------------------------------------------------------+";
		break;
		
	case 51:
		str_temp_data_temp.Format("%50s", str_data);
		line += str_temp_data_temp;
		line += " |";
		fprintf(fp,"%s", line);
		break;
		
	case 49:
		str_temp_data_temp.Format("%30s", str_data);
		line += str_temp_data_temp;
		line += " |";
		fprintf(fp,"%s\r\n", line);
		str_content = "+----------------------------------------+---------------------------------------------------+";
		break;
		
	case 50:
		str_temp_data_temp.Format("%50s", str_data);
		line += str_temp_data_temp;
		line += " |";
		fprintf(fp,"%s\r\n", line);
		str_content = "+----------------------------------------+---------------------------------------------------+";
		break;

	case 130:
		str_temp_data_temp.Format("%124s", str_data);
		line += str_temp_data_temp;
		line += " |";
		fprintf(fp,"%s\r\n", line);
		str_content = "+--------------------------+-----------------------------------------------------------------------------------------------------------------------------+";
		break;
		
	case 300:
		str_temp_data_temp.Format("%30s", str_data);
		line += str_temp_data_temp;
		line += " |";
		fprintf(fp,"%s", line);
		break;
	}
	fprintf(fp,"%s\r\n",str_content);
}

void CPublic_Function::DB_Write_Time(st_time_database st_timedb)
{
	CString strTmp[7];
	
	st_handler.lTimeTable[1] = GetCurrentTime();
	st_handler.lTimeTable[2] = st_handler.lTimeTable[1] - st_handler.lTimeTable[0];
	
	st_handler.lTimeTable[0] = GetCurrentTime();
	
	st_timedb.mnTime_Time = (int)st_handler.lTimeTable[2];
	
	strTmp[1].Format("%04d/%02d/%02d %02d:%02d:%02d", st_timedb.mole_date.GetYear(), 
		st_timedb.mole_date.GetMonth(),
		st_timedb.mole_date.GetDay(),
		st_timedb.mole_date.GetHour(),
		st_timedb.mole_date.GetMinute(),
		st_timedb.mole_date.GetSecond());
	
	strTmp[2].Format("%-1s", st_timedb.mstrlot_no);
	strTmp[3].Format("%-1s", st_timedb.mstrpart_no);	
	
	strTmp[4].Format("%d", st_timedb.mnTime_Section);
	strTmp[5].Format("%d", st_timedb.mnTime_Time);
	strTmp[6].Format("%d", st_timedb.mnMode);

	if (Mysql.MySql_Open("localhost", "root", "1111", 3306))
	{
		if (Mysql.Database_Select("AMT8530"))
		{	
			if (Mysql.Table_Select("AMT8530", "TIME_DATA"))
			{
				Mysql.Record_Add("TIME_DATA", strTmp);
			}
			else
			{
				if (Mysql.Table_Create("AMT8530", "TIME_DATA", 7, Mysql.mstr_time_table_name, Mysql.mn_time_table_type, Mysql.mn_time_table_num))
				{
					Mysql.Record_Add("TIME_DATA", strTmp);
				}
			}
		}
		else
		{
			if (Mysql.Database_Create("AMT8530"))
			{
				if (Mysql.Table_Create("AMT8530", "TIME_DATA", 7, Mysql.mstr_time_table_name, Mysql.mn_time_table_type, Mysql.mn_time_table_num))
				{
					Mysql.Record_Add("TIME_DATA", strTmp);
				}
			}
		}
	}
	
	Mysql.MySql_Close();

}

void CPublic_Function::DB_Write_Alarm(st_alarm_database st_alarmdb)
{	
	if (st_basic.n_mode_device == CTL_NO)			return;


	CString strTmp[6];
	
	strTmp[1].Format("%04d/%02d/%02d %02d:%02d:%02d", st_alarmdb.mole_date.GetYear(), 
		st_alarmdb.mole_date.GetMonth(),
		st_alarmdb.mole_date.GetDay(),
		st_alarmdb.mole_date.GetHour(),
		st_alarmdb.mole_date.GetMinute(),
		st_alarmdb.mole_date.GetSecond());
	
	
	strTmp[2].Format("%-1s", st_alarmdb.mstrlot_no);
	strTmp[3].Format("%-1s", st_alarmdb.mstrpart_no);
	
	strTmp[4].Format("%-1s", st_alarmdb.mstrAlarmCode);
	strTmp[5].Format("%-1s", st_alarmdb.mstrAlarmMsg);
	
	if (Mysql.MySql_Open("localhost", "root", "1111", 3306))
	{
		if (Mysql.Database_Select("AMT8530"))
		{	
			if (Mysql.Table_Select("AMT8530", "ALARM_DATA"))
			{
				Mysql.Record_Add("ALARM_DATA", strTmp);
			}
			else
			{
				if (Mysql.Table_Create("AMT8530", "ALARM_DATA", 6, Mysql.mstr_alarm_table_name, Mysql.mn_alarm_table_type, Mysql.mn_alarm_table_num))
				{
					Mysql.Record_Add("ALARM_DATA", strTmp);
				}
			}
		}
		else
		{
			if (Mysql.Database_Create("AMT8530"))
			{
				if (Mysql.Table_Create("AMT8530", "ALARM_DATA", 6, Mysql.mstr_alarm_table_name, Mysql.mn_alarm_table_type, Mysql.mn_alarm_table_num))
				{
					Mysql.Record_Add("ALARM_DATA", strTmp);
				}
			}
		}
	}
	
	Mysql.MySql_Close();

}

CString CPublic_Function::GetStrCurrentTime()
{
	COleDateTime timeCur;
	int iYear, iMonth, iDay, iHour, iMin, iSec;
	CString strTime;
	
	timeCur = COleDateTime::GetCurrentTime();

	iYear = timeCur.GetYear();
	iMonth = timeCur.GetMonth();
	iDay = timeCur.GetDay();

	iHour = timeCur.GetHour();
	iMin = timeCur.GetMinute();
	iSec = timeCur.GetSecond();

	strTime.Format("%04d/%02d/%02d_%02d:%02d:%02d", iYear, iMonth, iDay, iHour, iMin, iSec );
	return strTime;
}

void CPublic_Function::OnLogRecord_Data( int iPos, int iWhere, bool bSet, CString strWhere )
{
	
}

void CPublic_Function::MsgLog( LPCTSTR strMsg )
{
	if (st_handler.cwnd_list != NULL)  // ����Ʈ �� ȭ�� ����
	{
		sprintf(st_msg.c_abnormal_msg, strMsg);
		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // ���� ���� ��� ��û
	}
}

void CPublic_Function::DeleteOldFiles( LPCTSTR lpDirPath, int iDays )
{
	if ( lpDirPath == NULL )
		return;

	CString strNextDirPath = "";
	CFileFind find;
	BOOL bFind = find.FindFile( lpDirPath );

	if ( bFind == FALSE )
		return;

	while( bFind )
	{
		bFind = find.FindNextFile();

		// . or .. pass
		if ( find.IsDots() == TRUE )
			continue;

		if ( find.IsDirectory() )
		{
			CString strDir;
			strDir.Format("%s\\*.*", find.GetFilePath() );
			DeleteOldFiles( strDir, iDays );
		}
		else
		{
			HANDLE hF = CreateFile(find.GetFileName(), GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);

			SYSTEMTIME st;
			GetLocalTime(&st);
			
			CTime tNow(st.wYear, st.wMonth, st.wDay, 0, 0, 0);
			CTime tTime;
			find.GetLastWriteTime(tTime);
			
			CTimeSpan span = tNow - tTime;
			int iDays = span.GetDays();

			if ( span.GetDays() > iDays )
			{
				DeleteFile( find.GetFilePath() );

				CString strLog;
				strLog.Format("Delete File - Lost %d Days : %s", iDays, find.GetFilePath() );
				cLOG.OnLogEvent( LOG_PGM_, strLog );
			}
		}
	}

	find.Close();
}

void CPublic_Function::MakeLogFolder(CString BaseDirectory, const CString strMsg)
{
	CString FilePath = BaseDirectory.Mid(BaseDirectory.GetLength() - 1);
	//====================================================
	// Drirectroy Name Check
	//====================================================
	
	if (FilePath != "\\")
		FilePath = BaseDirectory+"\\";
	else
		FilePath = BaseDirectory;
	
	CFileFind folder;
	
	
	CTime LogTime;
	LogTime = CTime::GetCurrentTime();
	CString strTime;
	strTime.Format("%d��%02d��%02d��%02d��%02d��%02d��",LogTime.GetYear(),LogTime.GetMonth(),
		LogTime.GetDay(),LogTime.GetHour(),LogTime.GetMinute(),LogTime.GetSecond());
	
	int nThisYear   = LogTime.GetYear();
	int nThisMonth  = LogTime.GetMonth();
	int nThisDay    = LogTime.GetDay();
	int nThisHour	= LogTime.GetHour();
	
	CString filename, strMakeFolder,strDeleteFolder;
	CString strTemp;
	
	strMakeFolder.Format("%04d��", nThisYear);
	strMakeFolder = FilePath + strMakeFolder;
	if (nThisYear != m_nLastYear)
		::CreateDirectory(strMakeFolder, NULL);
	
	strTemp.Format("\\%02d��",nThisMonth);
	strMakeFolder = strMakeFolder+strTemp;
	if (nThisMonth != m_nLastMonth) //�� ���� ����
	{
		::CreateDirectory(strMakeFolder, NULL);
		if (nThisMonth > 2)
		{
			strDeleteFolder.Format("%02d��",(nThisMonth-1));
			strDeleteFolder = FilePath + strDeleteFolder;
			FileAccess.DeleteAllFolderFile(strDeleteFolder,"*.*");
		}
		else if (nThisMonth == 2)
		{
			int temp = 12;
			strDeleteFolder.Format("%02d��",temp);
			strDeleteFolder = FilePath + strDeleteFolder;
			FileAccess.DeleteAllFolderFile(strDeleteFolder,"*.*");
		}
		else if (nThisMonth ==1)
		{
			int temp = 11;
			strDeleteFolder.Format("%02d��",temp);
			strDeleteFolder = FilePath + strDeleteFolder;
			FileAccess.DeleteAllFolderFile(strDeleteFolder,"*.*");
		}
		else;		
	}
	
	strTemp.Format("\\%02d��",nThisDay);
	strMakeFolder = strMakeFolder+strTemp;
	if (m_nLastDay != nThisDay) //��¥ ���� ����
	{
		::CreateDirectory(strMakeFolder, NULL);
		Sleep(10);
	}
	
	strTemp.Format("\\%02d��", nThisHour);
	strMakeFolder = strMakeFolder + strTemp;
	if (m_nLastHour != nThisHour)
	{
		::CreateDirectory(strMakeFolder, NULL);
		Sleep(10);
	}
	
	//====================================================
	//���� folder �� ���� ���� �ʴ´ٸ� Return �� ��Ų��.
	//====================================================
	
	CFileFind find;
	if (find.FindFile(strMakeFolder)==0)
		return;
	strTemp.Format("\\Seq%s.log",strTime);
	strMakeFolder = strMakeFolder + strTemp;
	
	if (find.FindFile(strMakeFolder)) // 
		return;
	
	FileAccess.WriteFile(strMakeFolder,strMsg);
	
	m_nLastYear = nThisYear;
	m_nLastMonth = nThisMonth;
	m_nLastDay = nThisDay;
	m_nLastHour = nThisHour;
}

void CPublic_Function::SetActionParam()
{
	pMainFrm = (CMainFrame*)::AfxGetMainWnd();   ////////���α׷��� ���� �ɶ�...MainFrm�� �ʱ�ȭ ���ش�..
	ASSERT(pMainFrm);
}

//==================================================================//
// ���μ����� ���� ����
//==================================================================//
char CPublic_Function::GetNtPrivilege()
{
	HANDLE h_token;
	TOKEN_PRIVILEGES privilege_info;
	
	// ���� ���μ����� ���Ѱ� ���õ� ������ �����ϱ� ���� ��ū������ ����
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES |
		TOKEN_QUERY, &h_token))
	{
		// ���Ѱ� ���õ� ���� ���ٿ� ������
		return 0;
	}
	
	// ���� ���μ����� SE_SHUTDOWN_NAME ������ ����Ҽ� �ֵ��� �����Ѵ�
	LookupPrivilegeValue(NULL, SE_SYSTEMTIME_NAME, &privilege_info.Privileges[0].Luid);
	privilege_info.PrivilegeCount = 1;
	privilege_info.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	
	// ������ ������ ������ �����Ѵ�
	AdjustTokenPrivileges(h_token, FALSE, &privilege_info, 0, (PTOKEN_PRIVILEGES)NULL, 0);
	if (GetLastError() != ERROR_SUCCESS)
	{
		// ���� ������ ������ ���
		return 0;
	}
	return 1;
}

void CPublic_Function::DateTimeChange(CString sDateTime)
{
	// �ð� ����
	SYSTEMTIME csTime;
	csTime.wYear	= atoi(sDateTime.Mid(0,4));
	csTime.wMonth	= atoi(sDateTime.Mid(4,2));
	csTime.wDay		= atoi(sDateTime.Mid(6,2));
	csTime.wHour	= atoi(sDateTime.Mid(8,2));
	csTime.wMinute	= atoi(sDateTime.Mid(10,2));
	csTime.wSecond	= atoi(sDateTime.Mid(12,2));
	csTime.wMilliseconds = 0;
	
	GetNtPrivilege();  // ���μ����� ���� ����
	
	if (SetLocalTime(&csTime))
	{
		sprintf(st_msg.c_normal_msg, "�ý��� ��¥�� ����Ǿ����ϴ�. : %s", sDateTime);
		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // ���� ���� ��� ��û
	}
	else
	{
		sprintf(st_msg.c_abnormal_msg, "�ý��� ��¥�� ������ �� �����ϴ�. : %s", sDateTime);
		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // ���� ���� ��� ��û
	}
}

void CPublic_Function::OnDailyDataSave()
{
	

}

void CPublic_Function::OnDayDataCheck()
{
	CString mstr_chk_temp;
	int mn_chk_year, mn_chk_month, mn_chk_day, mn_chk_hour; // Data ���� �ð����� ���� ���� 
	COleDateTime time_cur;
	int mn_cur_year, mn_cur_month, mn_cur_day, mn_cur_hour; // ���� �ð����� ���� ���� 
	
	mstr_chk_temp.Format("%10d", st_work.n_data_save_time);
	
	mn_chk_year = atoi(mstr_chk_temp.Mid(0, 4));
	mn_chk_month = atoi(mstr_chk_temp.Mid(4, 2));
	mn_chk_day = atoi(mstr_chk_temp.Mid(6, 2));
	mn_chk_hour = atoi(mstr_chk_temp.Mid(8, 2));
	
	time_cur = COleDateTime::GetCurrentTime();
	
	mn_cur_year = time_cur.GetYear();  
	mn_cur_month = time_cur.GetMonth();  
    mn_cur_day = time_cur.GetDay();  
	mn_cur_hour = time_cur.GetHour();
	
	// ���� �ð��� Day Start Time�� �Ǳ� ���̶��...
	if (mn_cur_hour < st_basic.n_day_start_time)
	{
		if (mn_chk_day == mn_cur_day)						// ����� ��¥�� ���� ��¥�� ���ٸ�...
		{
			
		}
		else								// ���� �ʴٸ� ���� Data�̴�.
		{
			if (mn_chk_hour < st_basic.n_day_start_time)	// ����� �ð��� Day Start �ð����� �����̸�.. Reset�̴�.
			{
				OnDayDataReset();			// Day Data Reset
			}
		}
	}
	else		// ���� �ð��� Day Start Time ���Ķ��...
	{
		if (mn_chk_day == mn_cur_day)						// ����� ��¥�� ���� ��¥�� ���ٸ�...
		{
			if (mn_chk_hour < st_basic.n_day_start_time)	// ����� �ð��� Day Start �ð����� �����̸�.. Reset�̴�.
			{
				OnDayDataReset();		// Day Data Reset
			}
		}
		else							// ���� �ʴٸ� ���� Data�̴�.
		{
			OnDayDataReset();			// Day Data Reset
		}
	}
}

void CPublic_Function::OnDayDataReset()
{
	CString mstr_temp;

	st_handler.m_tDR = 0;
	st_handler.m_tDJ = 0;
	st_handler.m_tDM = 0;
	st_handler.m_tDS = 0;
	st_handler.m_tDMTBI = 0;

	mstr_temp.Format("%d",st_handler.m_tDR);
	:: WritePrivateProfileString("BasicData", "Daily_Run_Time", LPCTSTR(mstr_temp), st_path.mstr_basic);
	
	mstr_temp.Format("%d",st_handler.m_tDS);
	:: WritePrivateProfileString("BasicData", "Daily_Stop_Time", LPCTSTR(mstr_temp), st_path.mstr_basic);
	
	mstr_temp.Format("%d",st_handler.m_tDJ);
	:: WritePrivateProfileString("BasicData", "Daily_Alarm_Time", LPCTSTR(mstr_temp), st_path.mstr_basic);
	
	mstr_temp.Format("%d",st_handler.m_tDM);
	:: WritePrivateProfileString("BasicData", "Daily_Maint_Time", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp.Format("%d",st_handler.m_tDMTBI);
	:: WritePrivateProfileString("BasicData", "Daily_MTBI_Time", LPCTSTR(mstr_temp), st_path.mstr_basic);

	st_work.nMdlInputCount[0][1] = 0;
	st_work.nMdlInputCount[1][1] = 0;

	st_work.nMdlPassCount[0][1] = 0;
	st_work.nMdlPassCount[1][1] = 0;

	st_work.nMdlRejectCount[0][1] = 0;
	st_work.nMdlRejectCount[1][1] = 0;

	st_work.nMdlFailCount[0][1] = 0;
	st_work.nMdlFailCount[1][1] = 0;

	st_work.nMdlHtoCount[0][1] = 0;
	st_work.nMdlHtoCount[1][1] = 0;

	st_work.nMdlBtoCount[0][1] = 0;
	st_work.nMdlBtoCount[1][1] = 0;

	st_work.nMdlNtoCount[0][1] = 0;
	st_work.nMdlNtoCount[1][1] = 0;
	
	mstr_temp.Format("%d", st_work.nMdlInputCount[0][1]);
	:: WritePrivateProfileString("BasicData", "Input_Prime_Day", LPCTSTR(mstr_temp), st_path.mstr_basic);
	
	mstr_temp.Format("%d", st_work.nMdlInputCount[1][1]);
	:: WritePrivateProfileString("BasicData", "Input_Cum_Day", LPCTSTR(mstr_temp), st_path.mstr_basic);
	
	mstr_temp.Format("%d", st_work.nMdlPassCount[0][1]);
	:: WritePrivateProfileString("BasicData", "Pass_Prime_Day", LPCTSTR(mstr_temp), st_path.mstr_basic);
	
	mstr_temp.Format("%d", st_work.nMdlPassCount[1][1]);
	:: WritePrivateProfileString("BasicData", "Pass_Cum_Day", LPCTSTR(mstr_temp), st_path.mstr_basic);
	
	mstr_temp.Format("%d", st_work.nMdlRejectCount[0][1]);
	:: WritePrivateProfileString("BasicData", "Reject_Prime_Day", LPCTSTR(mstr_temp), st_path.mstr_basic);
	
	mstr_temp.Format("%d", st_work.nMdlRejectCount[0][1]);
	:: WritePrivateProfileString("BasicData", "Reject_Cum_Day", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp.Format("%d", st_work.nMdlFailCount[0][1]);
	:: WritePrivateProfileString("BasicData", "Fail_Prime_Day", LPCTSTR(mstr_temp), st_path.mstr_basic);
	
	mstr_temp.Format("%d", st_work.nMdlFailCount[0][1]);
	:: WritePrivateProfileString("BasicData", "Fail_Cum_Day", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp.Format("%d", st_work.nMdlHtoCount[0][1]);
	:: WritePrivateProfileString("BasicData", "Hto_Prime_Day", LPCTSTR(mstr_temp), st_path.mstr_basic);
	
	mstr_temp.Format("%d", st_work.nMdlHtoCount[0][1]);
	:: WritePrivateProfileString("BasicData",  "Hto_Cum_Day", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp.Format("%d", st_work.nMdlBtoCount[0][1]);
	:: WritePrivateProfileString("BasicData", "Bto_Prime_Day", LPCTSTR(mstr_temp), st_path.mstr_basic);
	
	mstr_temp.Format("%d", st_work.nMdlBtoCount[0][1]);
	:: WritePrivateProfileString("BasicData", "Bto_Cum_Day", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp.Format("%d", st_work.nMdlNtoCount[0][1]);
	:: WritePrivateProfileString("BasicData", "Nto_Prime_Day", LPCTSTR(mstr_temp), st_path.mstr_basic);
	
	mstr_temp.Format("%d", st_work.nMdlNtoCount[0][1]);
	:: WritePrivateProfileString("BasicData", "Nto_Cum_Day", LPCTSTR(mstr_temp), st_path.mstr_basic);
	
	if (st_handler.cwnd_main != NULL)
	{
		st_handler.cwnd_main->PostMessage(WM_WORK_END, MAIN_COUNTINFO, 0);
	}
	
	st_work.mn_daily_fail_count = 0;
	
	mstr_temp.Format("%d", st_work.mn_daily_fail_count);
	:: WritePrivateProfileString("BasicData", "Daily_Fail_Count", LPCTSTR(mstr_temp), st_path.mstr_basic);
}

void CPublic_Function::GlobalDataSerialize(CArchive &ar)
{
	int i=0; int j=0; int k=0; int l=0; int m=0;

	CObject::Serialize(ar);
	
	if (ar.IsStoring())
	{
		ar << st_handler.nSelectedMenuNum;					// �� ȭ�鿡 ������ ȭ�� ��ȣ ���� ���� 
		ar << st_handler.b_program_exit;				// ���α׷��� ���������°��� Ȯ���Ѵ�.
		
		// *************************************************************************
		// ��� ���� ���� ���� ���� ����                                            
		// *************************************************************************
		ar << st_handler.nFlag_InitComplete;					// �ʱ�ȭ �۾� �Ϸ� �÷���
		ar << st_handler.nModeManual;

		ar << st_handler.nMenuLock;					// �޴� ��� ���� ���� �÷��� 
		ar << st_handler.nSysLock;				// SYSTEM LOCK ���� �÷���
		ar << st_handler.nSwitchLock;				// Dialog�� �� �ִ� ���� �÷��� (Start Switch Lock)

		ar << st_handler.mstr_pass_level;
		ar << st_handler.mn_level_maint;				// ����Ʈ ����(Level 1) ���� �÷���
		ar << st_handler.mn_level_teach;				// ƼĪ ����(Level 2) ���� �÷���
		ar << st_handler.mn_level_speed;				// ���ǵ� ����(Level 3) ���� �÷���
		ar << st_handler.mn_level_admin;				// Admin ����(Level 4) ���� �÷���

		ar << st_handler.mn_mot_brd_initial;			// ��� ���� �ʱ�ȭ �۾� �Ϸ� �÷���
		ar << st_handler.mn_io_board_initial;

		for (i = 0; i < MOT_MAXMOTOR; i++)
		{
			ar << st_handler.mn_home_state[i];			// ���� HOMING �Ϸ� �÷���
		}

		for (i = 0; i < MAXSITE; i++)
		{
			ar << st_handler.mn_init_state[i];			// �ʱ�ȭ �Ϸ� �÷���
		}

		for (i = 0; i < MAXSITE; i++)
		{
			ar << st_handler.mn_lotend_state[i];		// �ʱ�ȭ �Ϸ� �÷���
		}

		ar << st_handler.nStopPushInInitial;

		ar << st_handler.md_run_speed;				// ��� ���۽� �ӵ���
		ar << st_handler.md_manual_speed;				// �޴��� ���۽� �ӵ���
	}
	else
	{
		ar >> st_handler.nSelectedMenuNum;					// �� ȭ�鿡 ������ ȭ�� ��ȣ ���� ���� 
		ar >> st_handler.b_program_exit;				// ���α׷��� ���������°��� Ȯ���Ѵ�.
		
		// *************************************************************************
		// ��� ���� ���� ���� ���� ����                                            
		// *************************************************************************
		ar >> st_handler.nFlag_InitComplete;					// �ʱ�ȭ �۾� �Ϸ� �÷���
		ar >> st_handler.nModeManual;

		ar >> st_handler.nMenuLock;					// �޴� ��� ���� ���� �÷��� 
		ar >> st_handler.nSysLock;				// SYSTEM LOCK ���� �÷���
		ar >> st_handler.nSwitchLock;				// Dialog�� �� �ִ� ���� �÷��� (Start Switch Lock)

		ar >> st_handler.mstr_pass_level;
		ar >> st_handler.mn_level_maint;				// ����Ʈ ����(Level 1) ���� �÷���
		ar >> st_handler.mn_level_teach;				// ƼĪ ����(Level 2) ���� �÷���
		ar >> st_handler.mn_level_speed;				// ���ǵ� ����(Level 3) ���� �÷���
		ar >> st_handler.mn_level_admin;				// Admin ����(Level 4) ���� �÷���

		ar >> st_handler.mn_mot_brd_initial;			// ��� ���� �ʱ�ȭ �۾� �Ϸ� �÷���
		ar >> st_handler.mn_io_board_initial;

		for (i = 0; i < MOT_MAXMOTOR; i++)
		{
			ar >> st_handler.mn_home_state[i];			// ���� HOMING �Ϸ� �÷���
		}

		for (i = 0; i < MAXSITE; i++)
		{
			ar >> st_handler.mn_init_state[i];			// �ʱ�ȭ �Ϸ� �÷���
		}

		for (i = 0; i < MAXSITE; i++)
		{
			ar >> st_handler.mn_lotend_state[i];		// �ʱ�ȭ �Ϸ� �÷���
		}

		ar >> st_handler.nStopPushInInitial;

		ar >> st_handler.md_run_speed;				// ��� ���۽� �ӵ���
		ar >> st_handler.md_manual_speed;				// �޴��� ���۽� �ӵ���
	}
}

void CPublic_Function::Handler_Recovery_Data_Write()
{	
	int nFuncRet, nFlag=CTL_NO;
	
	:: WritePrivateProfileString("SaveMode", "RecoveryMode", LPCTSTR("1"), st_path.mstr_file_basic);
	
	/*
	do{
		nFuncRet = Handler_Recovery_Data(DATA_WRITE);
		
		if (nFuncRet == CTL_ERROR)
		{
			nRet = AfxMessageBox("Recovery Data Save���߿� ������ �߻��Ͽ����ϴ�. ����ϸ� ���� ������ ������ ��� �˴ϴ�. ��õ��Ͻðڽ��ϱ� ?", MB_OKCANCEL);
			if (nRet != IDOK)
			{
				nCancleFlag = CTL_YES;
			}
		}
		else
			
			if (nCancleFlag == CTL_YES)
			{
				:: WritePrivateProfileString("SaveMode", "RecoveryMode", LPCTSTR("0"), st_path.mstr_file_basic);
			}
			
	}
	while(nFuncRet == CTL_ERROR && nCancleFlag == CTL_NO);
	*/
	// ����� ���� [2014.09.20]
	// - AMT7010 ������ �����ϰ� ����
	int i;
	for (i=0; i<HANDLER_BACKUP_DATA_RW; i++)
	{
		nFuncRet = Handler_Recovery_Data(0, i, DATA_WRITE);
		if (nFuncRet == CTL_ERROR)
		{
			if (st_handler.cwnd_list != NULL)  // ����Ʈ �� ȭ�� ����
			{
				sprintf(st_msg.c_abnormal_msg, "[ERROR_WRITE]Recovery Data Save error. [%02d]. cancel is skip data recovery data. ", i);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
			}
			nFlag = YES;
		}
	}

	if(nFlag == YES)
	{
		:: WritePrivateProfileString("SaveMode", "RecoveryMode", LPCTSTR("0"), st_path.mstr_file_basic);
		if (st_handler.cwnd_list != NULL) //james 2010,0708 �ΰ����� �߰� 
		{
			sprintf(st_msg.c_abnormal_msg, "[skip] recovery data save-stop");
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
		}
	}
	else
	{
		:: WritePrivateProfileString("SaveMode", "RecoveryMode", LPCTSTR("1"), st_path.mstr_file_basic);
		if (st_handler.cwnd_list != NULL) //james 2010,0708 �ΰ����� �߰� 
		{
			sprintf(st_msg.c_normal_msg, "[ok]recovery data save-stop");
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
		}
	}
}

void CPublic_Function::Set_SingleActionOnOff(int nIONumber, int nOnOff, int nOption)
{
	g_ioMgr.set_out_bit( nIONumber, nOnOff );
	
	mn_cylinder_flag[nIONumber] = FALSE;
	ml_cylinder_time[nIONumber][0] = GetCurrentTime(); 

	if ( nOption > 0 )
	{
		mn_cylinder_flag[nIONumber + nOption] = FALSE;
		ml_cylinder_time[nIONumber + nOption][0] = GetCurrentTime();
	}
}

void CPublic_Function::Set_DoubleActionOnOff(int nIONumber, int nIONumber2, int nOnOff)
{
	g_ioMgr.set_out_bit(nIONumber, nOnOff);
	g_ioMgr.set_out_bit(nIONumber2, !nOnOff);
	
	mn_cylinder_flag[nIONumber] = FALSE;
	ml_cylinder_time[nIONumber][0] = GetCurrentTime(); 
}

void CPublic_Function::Set_DualActionOnOff(int nIONumber, int nIONumber2, int nOnOff)
{
	g_ioMgr.set_out_bit(nIONumber, nOnOff);
	g_ioMgr.set_out_bit(nIONumber2, nOnOff);
	
	mn_cylinder_flag[nIONumber] = FALSE;
	ml_cylinder_time[nIONumber][0] = GetCurrentTime(); 

	mn_cylinder_flag[nIONumber2] = FALSE;
	ml_cylinder_time[nIONumber2][0] = GetCurrentTime(); 
}

int CPublic_Function::Get_SingActionOnOff(int nIONumberT, int nIONumber, int nOnOff, int nTime, int nOption)
{
	int nFuncRet = -1;
	
	ml_cylinder_time[nIONumberT][1] = GetCurrentTime();
	
	if (nOption == 0)		mn_cylinder_flag[nIONumberT] = TRUE;
	
	if (mn_cylinder_flag[nIONumberT] != TRUE)
	{
		ml_cylinder_time[nIONumberT][0] = GetCurrentTime();
		mn_cylinder_flag[nIONumberT] = TRUE;
	}
	else if (g_ioMgr.get_in_bit(nIONumber, nOnOff) == CTL_ON) // On ����.
	{
		if (nOption == 0)		return CYLINDER_ON;
		
		ml_cylinder_time[nIONumberT][2] = ml_cylinder_time[nIONumberT][1] - ml_cylinder_time[nIONumberT][0];
		
		if (ml_cylinder_time[nIONumberT][2] < 0)		
		{
			ml_cylinder_time[nIONumberT][0] = GetCurrentTime();
			return nFuncRet;
		}
		
		if (nOnOff == CYLINDER_DOWN)		// Down�� Ȯ���ϰ��� �Ѱ��ε� Up �Ǿ� �ִٸ�...
		{
			if (ml_cylinder_time[nIONumberT][2] > st_time.nWait_Limit[nTime])
			{
				nFuncRet = CYLINDER_ERROR;	
				mn_cylinder_flag[nIONumberT] = FALSE;
			}
		}
		else if (ml_cylinder_time[nIONumberT][2] > st_time.nWait_On[nTime])
		{
			nFuncRet = CYLINDER_ON;
			mn_cylinder_flag[nIONumberT] = FALSE;
		}
	}
	else if (g_ioMgr.get_in_bit(nIONumber, nOnOff) == CTL_OFF)	// Off ����.
	{
		if (nOption == 0)		return CYLINDER_OFF;
		
		ml_cylinder_time[nIONumberT][2] = ml_cylinder_time[nIONumberT][1] - ml_cylinder_time[nIONumberT][0];
		
		if (ml_cylinder_time[nIONumberT][2] < 0)		
		{
			ml_cylinder_time[nIONumberT][0] = GetCurrentTime();
			return nFuncRet;
		}
		
		if (nOnOff == CYLINDER_UP)		// Up�� Ȯ���ϰ��� �Ѱ��ε� Down �Ǿ� �ִٸ�...
		{
			if (ml_cylinder_time[nIONumberT][2] > st_time.nWait_Limit[nTime])
			{
				nFuncRet = CYLINDER_ERROR;	
				mn_cylinder_flag[nIONumberT] = FALSE;
			}
		}
		else if (ml_cylinder_time[nIONumberT][2] > st_time.nWait_Off[nTime])
		{
			nFuncRet = CYLINDER_OFF;	
			mn_cylinder_flag[nIONumberT] = FALSE;
		}
	}
	else
	{
		if (nOption == 0)		return CYLINDER_ERROR;
		
		ml_cylinder_time[nIONumberT][2] = ml_cylinder_time[nIONumberT][1] - ml_cylinder_time[nIONumberT][0];
		
		if (ml_cylinder_time[nIONumberT][2] < 0)		
		{
			ml_cylinder_time[nIONumberT][0] = GetCurrentTime();
			return nFuncRet;
		}
		
		if (ml_cylinder_time[nIONumberT][2] > st_time.nLimitTime[nTime])
		{
			nFuncRet = CYLINDER_ERROR;	
			mn_cylinder_flag[nIONumberT] = FALSE;
		}
	}
	
	return nFuncRet;
}

int CPublic_Function::Get_DoubleActionOnOff(int nIONumberT, int nIONumber, int nIONumber2, int nOnOff, int nTime, int nOption)
{
	int nFuncRet = -1;
	
	ml_cylinder_time[nIONumberT][1] = GetCurrentTime();
	
	if (nOption == 0)		mn_cylinder_flag[nIONumberT] = TRUE;
	
	if (mn_cylinder_flag[nIONumberT] != TRUE)
	{
		ml_cylinder_time[nIONumberT][0] = GetCurrentTime();
		mn_cylinder_flag[nIONumberT] = TRUE;
	}
	else if ((g_ioMgr.get_in_bit(nIONumber, nOnOff) == CTL_ON) && (g_ioMgr.get_in_bit(nIONumber2, !nOnOff) == CTL_OFF)) // On ����.
	{
		if (nOption == 0)		return CYLINDER_ON;
		
		ml_cylinder_time[nIONumberT][2] = ml_cylinder_time[nIONumberT][1] - ml_cylinder_time[nIONumberT][0];
		
		if (ml_cylinder_time[nIONumberT][2] < 0)		
		{
			ml_cylinder_time[nIONumberT][0] = GetCurrentTime();
			return nFuncRet;
		}
		
		if (nOnOff == CYLINDER_DOWN)		// Down�� Ȯ���ϰ��� �Ѱ��ε� Up �Ǿ� �ִٸ�...
		{
			if (ml_cylinder_time[nIONumberT][2] > st_time.nWait_Limit[nTime])
			{
				nFuncRet = CYLINDER_ERROR;	
				mn_cylinder_flag[nIONumberT] = FALSE;
			}
		}
		else if (ml_cylinder_time[nIONumberT][2] > st_time.nWait_On[nTime])
		{
			nFuncRet = CYLINDER_ON;
			mn_cylinder_flag[nIONumberT] = FALSE;
		}
	}
	else if ((g_ioMgr.get_in_bit(nIONumber, nOnOff) == CTL_OFF) && (g_ioMgr.get_in_bit(nIONumber2, !nOnOff) == CTL_ON))	// Off ����.
	{
		if (nOption == 0)		return CYLINDER_OFF;
		
		ml_cylinder_time[nIONumberT][2] = ml_cylinder_time[nIONumberT][1] - ml_cylinder_time[nIONumberT][0];
		
		if (ml_cylinder_time[nIONumberT][2] < 0)		
		{
			ml_cylinder_time[nIONumberT][0] = GetCurrentTime();
			return nFuncRet;
		}
		
		if (nOnOff == CYLINDER_UP)		// Up�� Ȯ���ϰ��� �Ѱ��ε� Down �Ǿ� �ִٸ�...
		{
			if (ml_cylinder_time[nIONumberT][2] > st_time.nWait_Limit[nTime])
			{
				nFuncRet = CYLINDER_ERROR;	
				mn_cylinder_flag[nIONumberT] = FALSE;
			}
		}
		else if (ml_cylinder_time[nIONumberT][2] > st_time.nWait_Off[nTime])
		{
			nFuncRet = CYLINDER_OFF;	
			mn_cylinder_flag[nIONumberT] = FALSE;
		}
	}
	else
	{
		if (nOption == 0)		return CYLINDER_ERROR;
		
		ml_cylinder_time[nIONumberT][2] = ml_cylinder_time[nIONumberT][1] - ml_cylinder_time[nIONumberT][0];
		
		if (ml_cylinder_time[nIONumberT][2] < 0)		
		{
			ml_cylinder_time[nIONumberT][0] = GetCurrentTime();
			return nFuncRet;
		}
		
		if (ml_cylinder_time[nIONumberT][2] > st_time.nLimitTime[nTime])
		{
			nFuncRet = CYLINDER_ERROR;	
			mn_cylinder_flag[nIONumberT] = FALSE;
		}
	}
	
	return nFuncRet;
}

int CPublic_Function::Get_SingActionOnOff_Pass_OnIO(int nIONumberT, int nIONumber, int nOnOff, int nTime, int nOption)	// 20130605 ����� ����׼� �� ���̿� �н�(��û) kjh
{
	int nFuncRet = -1;
	
	ml_cylinder_time[nIONumberT][1] = GetCurrentTime();
	
	if (nOption == 0)		mn_cylinder_flag[nIONumberT] = TRUE;
	
	if (mn_cylinder_flag[nIONumberT] != TRUE)
	{
		ml_cylinder_time[nIONumberT][0] = GetCurrentTime();
		mn_cylinder_flag[nIONumberT] = TRUE;
	}
	else if (g_ioMgr.get_in_bit(nIONumber, nOnOff) == CTL_OFF) // On ����.
	{
		if (nOption == 0)		return CYLINDER_ON;
		
		ml_cylinder_time[nIONumberT][2] = ml_cylinder_time[nIONumberT][1] - ml_cylinder_time[nIONumberT][0];
		
		if (ml_cylinder_time[nIONumberT][2] < 0)		
		{
			ml_cylinder_time[nIONumberT][0] = GetCurrentTime();
			return nFuncRet;
		}
		
		if (nOnOff == CYLINDER_DOWN)		// Down�� Ȯ���ϰ��� �Ѱ��ε� Up �Ǿ� �ִٸ�...
		{
			if (ml_cylinder_time[nIONumberT][2] > st_time.nLimitTime[nTime])
			{
				nFuncRet = CYLINDER_ERROR;	
				mn_cylinder_flag[nIONumberT] = FALSE;
			}
		}
		else if (ml_cylinder_time[nIONumberT][2] > st_time.nWait_On[nTime])
		{
			nFuncRet = CYLINDER_ON;
			mn_cylinder_flag[nIONumberT] = FALSE;
		}
	}
	else if (g_ioMgr.get_in_bit(nIONumber, nOnOff) == CTL_ON)	// Off ����.
	{
		if (nOption == 0)		return CYLINDER_OFF;
		
		ml_cylinder_time[nIONumberT][2] = ml_cylinder_time[nIONumberT][1] - ml_cylinder_time[nIONumberT][0];
		
		if (ml_cylinder_time[nIONumberT][2] < 0)		
		{
			ml_cylinder_time[nIONumberT][0] = GetCurrentTime();
			return nFuncRet;
		}
		
		if (nOnOff == CYLINDER_UP)		// Up�� Ȯ���ϰ��� �Ѱ��ε� Down �Ǿ� �ִٸ�...
		{
			if (ml_cylinder_time[nIONumberT][2] > st_time.nWait_Limit[nTime])
			{
				nFuncRet = CYLINDER_ERROR;	
				mn_cylinder_flag[nIONumberT] = FALSE;
			}
		}
		else if (ml_cylinder_time[nIONumberT][2] > st_time.nWait_Off[nTime])
		{
			nFuncRet = CYLINDER_OFF;	
			mn_cylinder_flag[nIONumberT] = FALSE;
		}
	}
	else
	{
		if (nOption == 0)		return CYLINDER_ERROR;
		
		ml_cylinder_time[nIONumberT][2] = ml_cylinder_time[nIONumberT][1] - ml_cylinder_time[nIONumberT][0];
		
		if (ml_cylinder_time[nIONumberT][2] < 0)		
		{
			ml_cylinder_time[nIONumberT][0] = GetCurrentTime();
			return nFuncRet;
		}
		
		if (ml_cylinder_time[nIONumberT][2] > st_time.nLimitTime[nTime])
		{
			nFuncRet = CYLINDER_ERROR;	
			mn_cylinder_flag[nIONumberT] = FALSE;
		}
	}
	
	return nFuncRet;
}	// 20130605 ����� ����׼� �� ���̿� �н�(��û) kjh

int CPublic_Function::OnNumberStringExtraCheck(CString strText, int nCount[3])
{
	int nFuncRet = CTL_GOOD;
	int i = 0;
	char chr_data[100];

	int n_len = strText.GetLength();

	if (n_len <= 0 || n_len > 100)
	{
		return CTL_ERROR;
	}

	for ( i = 0; i < 3; i++ )
	{
		nCount[i] = 0;
	}
	
	strcpy(chr_data, strText);
	
	for ( i = 0; i < n_len; i++ )
	{
		if ( chr_data[i] >= 48 && chr_data[i] <= 57 )			// ���� Ȯ��
		{
			nCount[0]++;
		}
		else if ( ( chr_data[i] >= 65 && chr_data[i] <= 90 ) || ( chr_data[i] >= 97 && chr_data[i] <= 122 ) )	// ���� Ȯ��
		{
			nCount[1]++;
		}
		else
		{
			nCount[2]++;
		}
	}

	return nFuncRet;
}

void CPublic_Function::CalcTime(int count)
{
	int i;
	int length;
	int pos[2];
	
	CString strData, strTmp[7];
	
	strData = Mysql.Record_Read();
	// lot_no
	length = strData.GetLength();
	pos[0] = strData.Find(",", 0);
	
	for (i = 0; i < Mysql.mn_time_table_cnt; i++)
	{	
		pos[1] = strData.Find(",", pos[0]+1);
		
		if (pos[1] > 0)
		{
			strTmp[i]= strData.Mid(pos[0]+1, (pos[1] - pos[0])-1);
			pos[0] = pos[1];
		}
		else
		{
			strTmp[i] = strData.Mid(pos[0]+1, (length - pos[1])-1);
		}
	}
	
	// 3�� Ÿ�ӱ���, 4�� Ÿ��, 5�� Work�� ByPass��.
	dTime[atoi(strTmp[3])][atoi(strTmp[5])] += atol(strTmp[4]);	
}

double CPublic_Function::OnTimeSearch(int nType)
{
	int count=0;
	int i=0, j=0;
	CString strTmp;
	COleDateTime cur_time;
	double dFuncRet=0;
	
	COleDateTime Stime;
	COleDateTime Etime;
	
	cur_time = COleDateTime::GetCurrentTime();
	
	int year_s, year_e, month_s, month_e, day_s, day_e;
	
	Stime = cur_time + COleDateTimeSpan(-1, 0, 0, 0);				// ����
	Etime = cur_time + COleDateTimeSpan(0, 0, 0, 0);				// ����
	
	year_s = Stime.GetYear();
	month_s = Stime.GetMonth();
	day_s = Stime.GetDay();
	
	year_e = Etime.GetYear();
	month_e = Etime.GetMonth();
	day_e = Etime.GetDay();
	
	m_oledate[0].SetDateTime(year_s, month_s, day_s, 22, 0, 0);
	m_oledate[1].SetDateTime(year_e, month_e, day_e, 22, 0, 0);
	
	for (i=0; i<6;i++)
	{
		for (j=0;j<2;j++)
		{
			dTime[i][j] = 0;
		}
	}
	
	if (Mysql.MySql_Open("localhost", "root", "1111", 3306))
	{
		if (Mysql.Database_Select("AMT8530"))
		{	
			count = Mysql.Record_Date_Search("TIME_DATA", "DATE", "<>", m_oledate);
			
			if (count > 0)
			{
				for (i = 0; i<count; i++)
				{
					CalcTime(i);
				}
				
				// �ش� ����� ���� ������ �Ѵ�.
				switch ( nType )
				{
				case dRUN:	dFuncRet = dTime[1][0] + dTime[1][1]; break;		// "Run Time"
				case dSTOP:	dFuncRet = dTime[0][0] + dTime[0][1]; break;		// "Stop Time"
				case dJAM:	dFuncRet = dTime[5][0] + dTime[5][1]; break;		// "Alarm Time"
				}
			}	
		}
	}
	Mysql.MySql_Close();
	
	return dFuncRet;
}

void CPublic_Function::OnHour_Pass_Count()	//20130531 �ð��� ���� kjh
{
	CString mstr_create_file;  // �˶� ���� ������ ���Ͽ� ���� [����]+[���ϸ�]+[Ȯ����] ���� ����
	CString mstr_content;  // �� ���ο� ����� ���ڿ� ���� ���� ���� 
	CString str_new_file;		// ���Ͽ� ����� Ÿ��Ʋ �ð� ���� ����
	CString str_time, strTemp;
	
	int n_cur_year, n_cur_month, n_cur_day;		// ��, ��, �� ���� ����
	int n_cur_hour, n_cur_minute, n_cur_second;		// ��, ��, �� ���� ����
	int mn_existence;	

	char fileName[256];    // �˻��� ���� ���� ���� �Լ� 

	COleDateTime mtime_cur;						// ���� �ð� ���� ����

	FILE  *fp ;            // ���Ͽ� ���� ������ ���� ���� 

	mtime_cur = COleDateTime::GetCurrentTime();
	
	n_cur_year = mtime_cur.GetYear();  
	n_cur_month = mtime_cur.GetMonth();  
    n_cur_day = mtime_cur.GetDay();

	n_cur_hour = mtime_cur.GetHour();
	n_cur_minute = mtime_cur.GetMinute();
	n_cur_second = mtime_cur.GetSecond();

	if ( n_cur_hour >= 22 )	// 20130611 22�ð� üũ�� kjh
	{				
		COleDateTimeSpan span2( 1, 0, 0, 0 );
		
		mtime_cur += span2;

		n_cur_year = mtime_cur.GetYear();  
		n_cur_month = mtime_cur.GetMonth();  
		n_cur_day = mtime_cur.GetDay();
		
		str_new_file.Format("DAY_PASS_COUNT%04d%02d%02d", n_cur_year, n_cur_month, n_cur_day);
		
		mtime_cur -= span2;
		
		n_cur_year = mtime_cur.GetYear();  
		n_cur_month = mtime_cur.GetMonth();  
		n_cur_day = mtime_cur.GetDay();
		
		str_time.Format("%04d/%02d/%02d %02d:%02d:%02d", n_cur_year, n_cur_month, n_cur_day, n_cur_hour, n_cur_minute, n_cur_second);
	}
	else
	{
		str_new_file.Format("DAY_PASS_COUNT%04d%02d%02d", n_cur_year, n_cur_month, n_cur_day);
		str_time.Format("%04d/%02d/%02d %02d:%02d:%02d", n_cur_year, n_cur_month, n_cur_day, n_cur_hour, n_cur_minute, n_cur_second);
	}	// 20130611 22�ð� üũ�� kjh
	
// 	str_new_file.Format("DAY_PASS_COUNT%04d%02d%02d", n_cur_year, n_cur_month, n_cur_day);
// 	str_time.Format("%04d/%02d/%02d %02d:%02d:%02d", n_cur_year, n_cur_month, n_cur_day, n_cur_hour, n_cur_minute, n_cur_second);
	
	mstr_create_file = st_path.str_daily_pass_count + str_new_file;
	mstr_create_file += ".TXT";
	
	sprintf(fileName, "%s", mstr_create_file);
	mn_existence = access(fileName,0);
	
	if (mn_existence == -1)  /* ���� ������ */
	{
		mstr_create_file = st_path.str_daily_pass_count + str_new_file;
		mstr_create_file += ".TXT";
	}
	
	if ((fp=fopen(mstr_create_file,"a+"))==NULL)  
	{
		if (st_handler.mn_language == LANGUAGE_ENGLISH)
		{
			Func.MsgLog("File Open Fail!.");	// 20130926 kjh english
		}
		else
		{
			Func.MsgLog("���� ���� ����!.");
		}
		return;
	}

	mstr_content.Format("%-100s", "Save Time : " + str_time);
	fprintf(fp,"%s\r\n", mstr_content) ;
	
	strTemp.Format("%06d", st_work.nHourPassCount[1]);
	mstr_content.Format("%-100s", "Hour Pass : " + strTemp);
	fprintf(fp,"%s", mstr_content) ;

	strTemp.Format("%06d", st_work.nHourPassCount[0]);
	mstr_content.Format("%-100s", "Total Pass : " + strTemp);
	fprintf(fp,"%s\n\n", mstr_content) ;
	
	if (ferror(fp))  
	{
		if (st_handler.mn_language == LANGUAGE_ENGLISH)
		{
			Func.MsgLog("File Open Fail!.");	// 20130926 kjh english
		}
		else
		{
			Func.MsgLog("���� ���� ����!.");
		}
		return ;
	}
	
	fclose(fp);  /* ���� ���� */
}

void CPublic_Function::OnHour_Pass_Count_Init()	// 20130603 �ð��� ���� kjh
{
	CString mstr_temp, str_part;  // ������ ���� �ӽ� ���� ���� 
	char chr_buf[20];
	int i = 0;
	
	for ( i = 0; i < 3; i++ )
	{
		st_work.nHourPassCount[i] = 0;
		
		str_part.Format("[%d]", i);
		
		mstr_temp = LPCTSTR(_itoa(st_work.nHourPassCount[i], chr_buf, 10));			// 20130603 �ð��� ���� kjh
		:: WritePrivateProfileString("BasicData", "Hour_Pass_Count" + str_part, LPCTSTR(mstr_temp), st_path.mstr_basic);
	}

	mstr_temp = LPCTSTR(_itoa(st_work.iLastTime, chr_buf, 10));	
	:: WritePrivateProfileString("BasicData", "Last_Check_Hour", LPCTSTR(mstr_temp), st_path.mstr_basic);
}

int CPublic_Function::OnHexToDecimal(CString strHex)
{
	char chHex[10];
	int val;
	
	sprintf( chHex,strHex );
	
	chHex[2] = '\0';
	val = strtoul( chHex, NULL, 16 );
	
	return val;
}

// ���̸� �߰� [2014.06.13]
int CPublic_Function::OnCheckBoardVerify() 
{
	// ���� �ʿ�
	int nMasterNo=1, nPortNo=3;	// ������ ��

	int nPortMapData=0;
	int m=0, p=0;
	int iRet=0;
	INT nPortMap=0;

	// Check_IO_SlaveStatus : �� �Լ��� �����̺��� ENABLE/DISABLE ���� ������ ��ȯ
	// - �� �Լ��� ����� ��쿡�� �ʱ⿡ ������ ���� �� ������ ���� ���� ���纸���� ���α׷� �ؾ� ��

	// ������ ���� ������ŭ �ݺ�
	for(m=0; m<nMasterNo; m++)
	{
		for(p=0; p<nPortNo; p++)
		{
			// �ش� ��Ʈ�� �����̺� ���� ���ο� ���� �� ������ ����� [10���� ������ ����]
			nPortMapData = OnConvertStr2Dec(p);
			// ���� I/O ������ ���� ������ ����
			iRet = FAS_IO.Search_IO_Slave(m, p, &nPortMap);
			if (iRet == CTLBD_RET_ERROR)
			{
				return CTLBD_RET_ERROR;
			}
			// ���Ǿ��� �����̺� �� ������ ����� ���忡�� ���� �����̺� �� ������ ��
			if (nPortMapData != nPortMap)
			{
				return CTLBD_RET_ERROR;
			}
		}
	}

	return TRUE;
}

// ���̸� �߰� [2014.06.13]
int CPublic_Function::OnConvertStr2Dec(int nzPortNo) 
{
	// ���� �ʿ�
	// ���忡 �Ҵ�� ���� ����
	CString sBoardMap[3] = { "11111111", "11111111", "00001111" };

	// �ϳ��� ��Ʈ�� �Ҵ�� �����̺� �� ���� ����
	CString s1Data;
	s1Data = sBoardMap[nzPortNo];
	
	int nSlaveNo=8;	// ������ ��
	// [�ʱ�ȭ] ��Ʈ�� �����̺� ���� �� ������ 10���� ������ ��ȯ�� ��
	int nVal=0;
	int nConvertVal=0;
	int nScale;	// ������� �� ��
	CString sTmp;

	int s=0;
	for(s=0; s<nSlaveNo; s++)
	{
		// �ش� ��ġ�� ���ڸ� ���������� ��ȯ��Ŵ
		sTmp = s1Data.Mid(s, 1);
		nConvertVal = atoi(sTmp);
		// �ش� ��ġ�� ���ڿ� ���� ���� ���Ϲ���
		nScale = OnGetScale(s);

		// [�ƽ�Ű->����] ��ȯ�� ���� �����
		nVal += (nConvertVal*nScale);
	}

	return nVal;
}

// ���̸� �߰� [2014.06.13]
int CPublic_Function::OnGetScale(int nzSlaveNo) 
{
	int nScale = 0;

	// ��Ʈ�� �Ҵ�� �����̺� ���� ����
	switch(nzSlaveNo)
	{
	case 0:
		nScale=128;
		break;
	case 1:
		nScale=64;
		break;
	case 2:
		nScale=32;
		break;
	case 3:
		nScale=16;
		break;
	case 4:
		nScale=8;
		break;
	case 5:
		nScale=4;
		break;
	case 6:
		nScale=2;
		break;
	case 7:
		nScale=1;
		break;
	}

	return nScale;
}


//==================================================================//
// [RUN] ���� ���� ����
//==================================================================//
void CPublic_Function::OnSet_EqpStatusRun() 
{
	// yangkyu [2015.04.11]
	if (st_work.nForceDischarge == FORCEDISCHARGE_ON)
	{
		return;
	}

	// ���� ���� ���� [RUN]
	st_work.nEqpStatus	= dRUN;
	COMI.mn_run_status	= dRUN;
	
	st_timedb.mnTime_Section	= st_timedb.mnSectionBuffer;
	st_timedb.mnSectionBuffer	= dRUN;
	st_timedb.mole_date			= COleDateTime::GetCurrentTime();
	DB_Write_Time(st_timedb);

	st_handler.mn_level_teach	= FALSE;
	st_handler.mn_level_maint	= FALSE;

	// ���� ���� �˻��϶�� �÷��� ����
	// - ���� ������ �׻� �˻��ϵ��� ��
	st_handler.mn_safety = CTL_YES;

	// ��ư ���� ���� ����
	g_ioMgr.set_out_bit(stIO.o_LampStart,	IO_ON);
	g_ioMgr.set_out_bit(stIO.o_LampStop,	IO_OFF);
	g_ioMgr.set_out_bit(stIO.o_LampJamClear,	IO_OFF);
	
	if (g_ioMgr.get_out_bit(stIO.o_Smema_Emergency2Main, IO_ON) == IO_ON)
	{
//		OnSet_UpStreamSmema(REQ_EMO_OFF_);
	}

	// ���̸� �߰� [2014.12.18]
	OnCloseDoor();	// ���� �ݱ�
	
	Func.OnSet_BuzzerSound(IO_OFF);	// ���� ���� ON/OFF

	g_client_accy.OnCreate_SendFormat(MAIN_EQP_STATE);
	g_client_accy.OnReq_CommStart();

	// ���� ���� ������ PostMessage() ������� �ʰ� ��ü������ ������ �����Ͽ� ������

	alarm.stl_alarm_start_time = GetCurrentTime();

	// ���� �⵿ ���� ���� ���� [�⵿��]
	st_handler.mn_status = CTL_YES;
}

//==================================================================//
// [STOP] ���� ���� ����
//==================================================================//
void CPublic_Function::OnSet_EqpStatusStop() 
{
	// ���� ���� ���� [RUN]
	st_work.nEqpStatus	= dSTOP;
	COMI.mn_run_status	= dSTOP;

	st_timedb.mnTime_Section	= st_timedb.mnSectionBuffer;
	st_timedb.mnSectionBuffer	= dSTOP;
	st_timedb.mole_date			= COleDateTime::GetCurrentTime();
	DB_Write_Time(st_timedb);
	
	// �Ŵ� Lock ���� ����
	st_handler.nMenuLock = FALSE;
	
	// ��ư ���� ���� ����
	g_ioMgr.set_out_bit(stIO.o_LampStart,	IO_OFF);
	g_ioMgr.set_out_bit(stIO.o_LampStop,	IO_ON);
	
	if (g_ioMgr.get_out_bit(stIO.o_Smema_Emergency2Main, IO_OFF) == IO_OFF)
	{
//		OnSet_UpStreamSmema(REQ_EMO_ON_);
	}

	// �Ŵ��� ����� ��쿡�� ���� Open
	if (st_handler.nModeManual == YES)
	{
		g_ioMgr.set_out_bit(stIO.o_DoorLock,	IO_OFF);
	}

	g_client_accy.OnCreate_SendFormat(MAIN_EQP_STATE);
	g_client_accy.OnReq_CommStart();

	// ���� �⵿ ���� ���� ���� [��������]
	st_handler.mn_status = CTL_NO;
}

//==================================================================//
// [ALARM] ���� ���� ����
// - �˶� �߻� ����, ���� ���� ����
//==================================================================//
void CPublic_Function::OnSet_EqpStatusAlarm(int nzMode) 
{
	st_timedb.mnTime_Section	= st_timedb.mnSectionBuffer;
	st_timedb.mnSectionBuffer	= CTL_dWARNING;
	st_timedb.mole_date			= COleDateTime::GetCurrentTime();
	DB_Write_Time(st_timedb);

	OnSet_BtnLampStop();	// [STOP] ��ư ���� ���� ����
	OnSet_BuzzerSound(IO_ON, nzMode);	// ���� ���� ON
	
	// ���� ����ġ ���� ON
	g_ioMgr.set_out_bit(stIO.o_LampJamClear, IO_ON);
	
	if (alarm.mstr_code != "990000")
	{
		// �˶� �߻� ���� ���� [�߻�]
		st_work.nAlarmHappened = YES;
	}
}

//==================================================================//
// [INIT] ���� ���� ���� ����
//==================================================================//
void CPublic_Function::OnSet_EqpStatusInit() 
{
	st_work.nEqpStatus = dINIT;
	COMI.mn_run_status	= dINIT;
	
	// ���� Close
	g_ioMgr.set_out_bit(stIO.o_DoorLock, IO_ON);
}

// [STOP] ��ư ���� ���� ����
void CPublic_Function::OnSet_BtnLampStop() 
{
	g_ioMgr.set_out_bit(stIO.o_LampStart,	IO_OFF);
	g_ioMgr.set_out_bit(stIO.o_LampStop,	IO_ON);
}

//==================================================================//
// �˶� ���� �Ҹ� On/Off ����
//==================================================================//
void CPublic_Function::OnSet_BuzzerSound(int nzStatus, int nzType) 
{
	int i=0;
	int nType = nzType;

	switch(nzStatus)
	{
	case 0:	// IO_OFF
		g_ioMgr.set_out_bit(stIO.o_BuzzerSound1, IO_OFF);
		g_ioMgr.set_out_bit(stIO.o_BuzzerSound2, IO_OFF);
		g_ioMgr.set_out_bit(stIO.o_BuzzerSound3, IO_OFF);
		
		g_ioMgr.set_out_bit(stIO.o_LampBuzzStop, IO_OFF);
		break;
		
	case 1:	// IO_ON
		// ���� ��� ��忡���� ����
		if (stLAMP.nModeBuzzer == YES)
		{
			if (nType > 2 || nType < 0)
			{
				nType = 0;	// �⺻ ��ġ�� ������ ����
			}
			else if (nzType == 1)
			{
				g_ioMgr.set_out_bit(stIO.o_BuzzerSound2, IO_ON);
			}
			else if (nzType == 2)
			{
				g_ioMgr.set_out_bit(stIO.o_BuzzerSound3, IO_ON);
			}
			else
			{
				g_ioMgr.set_out_bit(stIO.o_BuzzerSound1, IO_ON);
			}
			
			g_ioMgr.set_out_bit(stIO.o_LampBuzzStop, IO_ON);
		}
		break;
	}
}

//==================================================================//
// [�Ŵ��� ���] ���� �� ����ġ ���� ����
//==================================================================//
void CPublic_Function::OnSet_EqpManual() 
{
	st_handler.nModeManual = CTL_YES;

	/*
	g_ioMgr.set_out_bit(stIO.o_auto,		IO_OFF);
	g_ioMgr.set_out_bit(stIO.o_manual,	IO_ON);
	*/
}

//==================================================================//
// [���� ���] ���� �� ����ġ ���� ����
//==================================================================//
void CPublic_Function::OnSet_EqpAuto() 
{
	st_handler.nModeManual = CTL_NO;

//	g_ioMgr.set_out_bit(stIO.o_DoorLock,	IO_ON);
	/*
	g_ioMgr.set_out_bit(stIO.o_auto,		IO_ON);
	g_ioMgr.set_out_bit(stIO.o_manual,	IO_OFF);
	*/
}

// ���̸� �߰� [2014.12.16]
//==================================================================//
// ���� ���� ���� ���� [0:��� ����, 0�̻�:���� ���� ����]
//==================================================================//
int CPublic_Function::OnWatchdogDoor() 
{
	int nFuncRet = 0;	// ���� ���� ����
	int i=0, iSensor;
	
	for(i=0; i<MAX_DOOR_; i++)
	{
		iSensor = g_ioMgr.get_in_bit(stIO.i_Chk_Door[i]);
		if (iSensor == IO_OFF)	// ���� ����
		{
			nFuncRet = i+1;
			break;
		}
	}
	return nFuncRet;
}

//==================================================================//
// ������ [����/����] ���� ����͸�
//==================================================================//
void CPublic_Function::OnMon_Door() 
{
	int nRet = VAR_INIT_;
	int i=0, iSensor;
	CString sTemp;
	
	// Auto ����� ���
	if (st_handler.nModeManual == CTL_NO)
	{
		// ���� �˶� �����̸� ���� ����
		if (st_work.nEqpStatus == dWARNING || st_work.nEqpStatus == dJAM || st_work.nEqpStatus == dLOTEND)
		{
			return;
		}
		
		nRet = OnWatchdogDoor();	// ���� ���� ���� ���� [0:��� ����, 0�̻�:���� ���� ����]
		if (nRet > 0)	// ���� ��� 1�� �̻� ������
		{
			for(i=0; i<MAX_DOOR_; i++)
			{
				iSensor = g_ioMgr.get_in_bit(stIO.i_Chk_Door[i]);
				if (iSensor == IO_OFF)	// ���� ����
				{
					// 900200 0 16 "DOOR_01_OPEN_ERR."
					// 900201 0 16 "DOOR_02_OPEN_ERR."
					// 900202 0 16 "DOOR_03_OPEN_ERR."
					// 900203 0 16 "DOOR_04_OPEN_ERR."
					// 900204 0 16 "DOOR_05_OPEN_ERR."
					// 900205 0 16 "DOOR_06_OPEN_ERR."
					// 900206 0 16 "DOOR_07_OPEN_ERR."
					// 900207 0 16 "DOOR_08_OPEN_ERR."
					// 900208 0 16 "DOOR_09_OPEN_ERR."
					// 900209 0 16 "DOOR_10_OPEN_ERR."
					// 900210 0 16 "DOOR_11_OPEN_ERR."
					// 900211 0 16 "DOOR_12_OPEN_ERR."
					// 900212 0 16 "DOOR_13_OPEN_ERR."
					// 900213 0 16 "DOOR_14_OPEN_ERR."
					// 900214 0 16 "DOOR_15_OPEN_ERR."
					// 900215 0 16 "DOOR_16_OPEN_ERR."
					sTemp.Format("%d", (900200 +i));
					// ���� Ȯ�ε� �˶� �ϳ��� ����ϸ� �ǹǷ� ����� ��� ��������
					break;
				}
			}
			alarm.mstr_code		= sTemp;
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
		}
	}
	else
	{
		for(i=0; i<MAX_DOOR_; i++)
		{
			iSensor = g_ioMgr.get_in_bit(stIO.i_Chk_Door[i]);
			if (iSensor == IO_OFF)	// ���� ����
			{
				if (st_handler.nDetect_DoorOpen[i] != TRUE)
				{
					st_handler.nDetect_DoorOpen[i] = TRUE;
				}
			}
			else
			{
				if (st_handler.nDetect_DoorOpen[i] != FALSE)
				{
					st_handler.nDetect_DoorOpen[i] = FALSE;
				}
			}
		}
	}
}

//==================================================================//
// ���� ����
//==================================================================//
void CPublic_Function::OnOpenDoor() 
{
	// ���� ���� [Stop]
	// ���� ��� [�Ŵ���]
	if (st_work.nEqpStatus == dSTOP && st_handler.nModeManual == CTL_YES)
	{
		g_ioMgr.set_out_bit(stIO.o_DoorLock, IO_OFF);
		
		if (st_handler.cwnd_list != NULL)
		{
			sprintf(st_msg.c_normal_msg, "[Release] Door Lock.");
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
		}
	}
	else
	{
		if (st_handler.cwnd_list != NULL)
		{
			// yangkyu [2015.03.24]
			// ������ �߸� ���ǰ� �־���.
			sprintf(st_msg.c_abnormal_msg, "[STOP+MANUAL], You Do.");
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
		}
	}
}

//==================================================================//
// ���� �ݱ�
//==================================================================//
void CPublic_Function::OnCloseDoor() 
{
	CString sDoorState = OnCheck_DoorState();	// ������ [����/����] ���� üũ
	if (sDoorState == "[CLOSE]")
	{
		g_ioMgr.set_out_bit(stIO.o_DoorLock,		IO_ON);
		
		sprintf(st_msg.c_normal_msg, "[LOCK] All Door Locked.");
	}
	else
	{
		sprintf(st_msg.c_normal_msg, "%s", sDoorState);
	}
	st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
}

//==================================================================//
// ������ [����/����] ���� üũ
//==================================================================//
CString CPublic_Function::OnCheck_DoorState() 
{
	CString sRetMsg = "[OPEN]", sTemp = "";
	int i=0;
	
	// ������ ON
	for(i=0; i<MAX_DOOR_; i++)
	{
		// ��� ��������
		if (g_ioMgr.get_in_bit(stIO.i_Chk_Door[i]) != IO_ON)
		{
			sTemp.Format("<%02d Door Open>", 1+i);
			sRetMsg += sTemp;
		}
	}
	
	int iLen = sRetMsg.GetLength();
	if (iLen <= 6)
	{
		sRetMsg = "[CLOSE]";
	}
	return sRetMsg;
}

//==================================================================//
// ���� ���� ���� ���� [0:��� ����, 0�̻�:���� ���� ����]
//==================================================================//
int CPublic_Function::OnIsDoorOpen() 
{
	int nFuncRet = 0;
	int i=0;

	// ������ ON
	for(i=0; i<MAX_DOOR_; i++)
	{
		// ��� ��������
		if (g_ioMgr.get_in_bit(stIO.i_Chk_Door[i]) != IO_ON)
		{
			nFuncRet = i+1;
			break;
		}
	}

	return nFuncRet;
}

//==================================================================//
// [�ʱ�ȭ] ��� ��Ʈ ����
// - ��ư �� ���� � ���� ���� �۾� ������
//==================================================================//
void CPublic_Function::OnClear_OutPort() 
{
	OnSet_BuzzerSound(IO_OFF, 0);	// ���� ���� OFF

	// ����ġ ��ư ���� ���� �ʱ�ȭ
	g_ioMgr.set_out_bit(stIO.o_LampStart,		IO_OFF);
	g_ioMgr.set_out_bit(stIO.o_LampJamClear,	IO_OFF);
	g_ioMgr.set_out_bit(stIO.o_LampBuzzStop,	IO_OFF);
	g_ioMgr.set_out_bit(stIO.o_LampStop,		IO_OFF);

	g_ioMgr.set_out_bit(stIO.o_LampLoadingStart,		IO_OFF);
	g_ioMgr.set_out_bit(stIO.o_LampManualLoading,		IO_OFF);

	// Grip Step Motor ���� �ΰ�
// 	g_ioMgr.set_out_bit(stIO.o_AccyGripMotor[0], IO_ON);
// 	g_ioMgr.set_out_bit(stIO.o_AccyGripMotor[1], IO_ON);
// 	g_ioMgr.set_out_bit(stIO.o_AccyGripMotor[2], IO_ON);
// 	g_ioMgr.set_out_bit(stIO.o_AccyGripMotor[3], IO_ON);

	g_ioMgr.set_out_bit(stIO.o_Smema_Ready2Main, IO_OFF);
}

//==================================================================//
// ��ü ������ ��� ����
//==================================================================//
void CPublic_Function::OnStop_AxisAll() 
{
	int i=0;
	for(i=0; i<MOT_MAXMOTOR; i++)
	{
		COMI.Set_MotStop(0, i);
	}
	
	//Sleep(100);  // ������ �߻��Ͽ� �ּ� ó��
	
	for(i=0; i<MOT_MAXMOTOR; i++)
	{
		COMI.Set_MotPower(i, FALSE);
	}
}

//==================================================================//
// EMO ����ġ ���� ���� ����͸�
//==================================================================//
void CPublic_Function::OnMon_EMO() 
{
	int i=0, iSen;
	CString sTemp;
	
	// Auto ����� ���
	if (st_handler.nModeManual == CTL_NO)
	{
		// ���� �˶� �����̸� ���� ����
		if (st_work.nEqpStatus == dWARNING || st_work.nEqpStatus == dJAM || st_work.nEqpStatus == dLOTEND)
		{
			return;
		}
		
		for(i=0; i<MAX_EMO_; i++)
		{
			iSen = g_ioMgr.get_in_bit(stIO.i_Chk_EMO[i]);
			if (iSen == IO_ON)
			{
				// 900031 0 16 "EMO_1_CHK_ERR."
				// 900032 0 16 "EMO_2_CHK_ERR."
				sTemp.Format("%d", (900031 +i));
				alarm.mstr_code		= sTemp;
				alarm.mn_count_mode	= 0;
				alarm.mn_type_mode	= eWARNING;
				st_work.nEqpStatus	= dWARNING;
				// ���� Ȯ�ε� �˶� �ϳ��� ����ϸ� �ǹǷ� ����� ��� ��������
				break;
			}
		}
	}
}

//==================================================================//
// MC ���� ������ ���� ����͸�
//==================================================================//
void CPublic_Function::OnMon_MC() 
{
	int iSen;
	// Auto ����� ���
	if (st_handler.nModeManual == CTL_NO)
	{
		// ���� �˶� �����̸� ���� ����
		if (st_work.nEqpStatus == dWARNING || st_work.nEqpStatus == dJAM || st_work.nEqpStatus == dLOTEND)
		{
			return;
		}
		
		iSen = g_ioMgr.get_in_bit(stIO.i_Chk_MC1);
		if (iSen == IO_OFF)
		{
			// 900041 0 16 "MC_1_CHK_ERR."
			alarm.mstr_code		= "900041";
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
		}
	}
}

//==================================================================//
// [Loading Manual] ���� �� ����ġ ���� ����
//==================================================================//
void CPublic_Function::OnSet_EqpLoadingManual()
{
	// ���̸� �߰� [2014.12.11]
	// �Ŵ���� Ʈ���� ���� ���� ������ ���̶�� ���� [�����̾� �����ϸ� �ȵ�]
	st_work.nLoadingStatus = dMANUAL;
	
	// ��ư ���� ���
	g_ioMgr.set_out_bit(stIO.o_LampManualLoading,	IO_ON);
	g_ioMgr.set_out_bit(stIO.o_LampLoadingStart,	IO_OFF);
}

//==================================================================//
// [Loading Start] ���� �� ����ġ ���� ����
//==================================================================//
void CPublic_Function::OnSet_EqpLoadingStart()
{
	// ���̸� �߰� [2014.12.11]
	// �Ŵ���� Ʈ���� ���� ���� �Ϸ� [�����̾� ���� ����]
	st_work.nLoadingStatus = dSTART;
	
	// ��ư ���� ���
	g_ioMgr.set_out_bit(stIO.o_LampManualLoading,	IO_OFF);
	g_ioMgr.set_out_bit(stIO.o_LampLoadingStart,	IO_ON);
}

//==================================================================//
// �Ǽ��縮 ���� ���� ��ȯ
// - �÷��� ������ �⺻���� �˻���
// - ���� Vacuum ���¿� �� �� ���� ���� ���� ��ȯ
//==================================================================//
int CPublic_Function::OnGet_PickerStatus(int nzSite, int nzYesNo, int nzPickerInfo[DATA_TYPE_][MAX_PICKER_]) 
{
	int nFuncRet = CTL_GOOD;

	int i;
	//==============================================================//
	// [WithOut] : ������ �÷��� ������ �̿��Ͽ� ���� ���� ���� ���� ����
	// - ���� �̻�� ���� Vacuum �� ������ ����� �� ����
	//==============================================================//
	if (st_basic.n_mode_device == WITHOUT_DVC_)
	{
		// ���� ���� ���θ� üũ��
		if (nzYesNo == CTL_YES)
		{
			for(i=0; i<MAX_PICKER_; i++)
			{
				if (nzPickerInfo[TYPE_FLAG_][i] == CTL_YES)
				{
					nzPickerInfo[TYPE_SEN_][i] = CTL_YES;
				}
				else
				{
					nzPickerInfo[TYPE_SEN_][i] = CTL_NO;
				}
			}
		}
		// ���� ������ ���θ� üũ��
		else
		{
			for(i=0; i<MAX_PICKER_; i++)
			{
				if (nzPickerInfo[TYPE_FLAG_][i] == CTL_YES)
				{
					nzPickerInfo[TYPE_SEN_][i] = CTL_NO;
				}
				else
				{
					nzPickerInfo[TYPE_SEN_][i] = CTL_YES;
				}
			}
		}
		return nFuncRet;
	}
	//--------------------------------------------------------------//

	//==============================================================//
	// [With] ���� Vacuum ���� üũ�Ͽ� ���� ���� ���θ� �Ǵ���
	//==============================================================//
	int iMaster=0, iSlave=0;

	if (nzSite == 0)	// �׸��� Vacuum
	{
		// [0:�¾�, 1:���, 2:�µ�, 3:���]
		for(i=0; i<MAX_PICKER_; i++)
		{
			iMaster	= i / 2;	// ���� ���
			iSlave	= i % 2;	// ������ ���
			
			nzPickerInfo[TYPE_SEN_][i] = g_ioMgr.get_in_bit(stIO.i_Chk_Rbt_VacOn[iMaster][iSlave]);
		}
	}

	// ���� ���� ���� üũ
	if (nzYesNo == CTL_YES)
	{
		for(i=0; i<MAX_PICKER_; i++)
		{
			// �˻��� ��Ŀ�� ������ ��쿡�� �˻�
			if (nzPickerInfo[TYPE_FLAG_][i] == CTL_YES)
			{
				// ���� ���� ���� Ȯ��
				if (nzPickerInfo[TYPE_SEN_][i] == CTL_NO)
				{
					if (nzSite == 0)	// �׸��� Vacuum
					{
						// 115000 0 00 "ROBOT_GRIP_1_VAC_ON_ERR"
						(alarm.mstr_code).Format("%06d", 115000+i);	// �˶� �ڵ� ����
						nFuncRet = CTL_ERROR;
					}
				}
			}
		}
	}
	// ���� ������ ���� üũ
	else
	{
		for(i=0; i<MAX_PICKER_; i++)
		{
			if (nzPickerInfo[TYPE_FLAG_][i] == CTL_YES)
			{
				if (nzPickerInfo[TYPE_SEN_][i] == CTL_YES)
				{
					if (nzSite == 0)	// �׸��� Vacuum
					{
						// 115010 0 00 "ROBOT_GRIP_1_VAC_OFF_ERR"
						(alarm.mstr_code).Format("%06d", 115010+i);	// �˶� �ڵ� ����
						nFuncRet = CTL_ERROR;
					}
				}
			}
		}
	}
	//--------------------------------------------------------------//

	return nFuncRet;
}
//kwlee 2017.0909
int CPublic_Function::OnGet_ReadyBuffStatus(int nzSite, int nzYesNo, int nzPickerInfo[DATA_TYPE_][MAX_PICKER_])
{
	int nFuncRet = CTL_GOOD;

	int i;
	//==============================================================//
	// [WithOut] : ������ �÷��� ������ �̿��Ͽ� ���� ���� ���� ���� ����
	// - ���� �̻�� ���� Vacuum �� ������ ����� �� ����
	//==============================================================//
	if (st_basic.n_mode_device == WITHOUT_DVC_)
	{
		// ���� ���� ���θ� üũ��
		if (nzYesNo == CTL_YES)
		{
			for(i=0; i<MAX_PICKER_; i++)
			{
				if (nzPickerInfo[TYPE_FLAG_][i] == CTL_YES)
				{
					nzPickerInfo[TYPE_SEN_][i] = CTL_YES;
				}
				else
				{
					nzPickerInfo[TYPE_SEN_][i] = CTL_NO;
				}
			}
		}
		// ���� ������ ���θ� üũ��
		else
		{
			for(i=0; i<MAX_PICKER_; i++)
			{
				if (nzPickerInfo[TYPE_FLAG_][i] == CTL_YES)
				{
					nzPickerInfo[TYPE_SEN_][i] = CTL_NO;
				}
				else
				{
					nzPickerInfo[TYPE_SEN_][i] = CTL_YES;
				}
			}
		}
		return nFuncRet;
	}
	//--------------------------------------------------------------//

	//==============================================================//
	// [With] ���� Vacuum ���� üũ�Ͽ� ���� ���� ���θ� �Ǵ���
	//==============================================================//
	int iMaster=0, iSlave=0;

	if (nzSite == 0)	// �׸��� Vacuum
	{
		// [0:�¾�, 1:���, 2:�µ�, 3:���]
// 		for(i=0; i<MAX_PICKER_; i++)
// 		{
// 			iMaster	= i / 2;	// ��oA�� ��e��e
// 			iSlave	= i % 2;	// ������OAo ��e��e
// 			
// 			nzPickerInfo[TYPE_SEN_][i] = !g_ioMgr.get_in_bit(stIO.i_Chk_BufferTryAccyDetection[iMaster][iSlave]);
// 		}
		

		nzPickerInfo[TYPE_SEN_][0] = !g_ioMgr.get_in_bit(stIO.i_Chk_BufferTryAccyDetection[SIDE_REAR_][SIDE_LEFT_]);
		nzPickerInfo[TYPE_SEN_][1] = !g_ioMgr.get_in_bit(stIO.i_Chk_BufferTryAccyDetection[SIDE_REAR_][SIDE_RIGHT_]);
		nzPickerInfo[TYPE_SEN_][2] = !g_ioMgr.get_in_bit(stIO.i_Chk_BufferTryAccyDetection[SIDE_FRONT_][SIDE_LEFT_]);
		nzPickerInfo[TYPE_SEN_][3] = !g_ioMgr.get_in_bit(stIO.i_Chk_BufferTryAccyDetection[SIDE_FRONT_][SIDE_RIGHT_]);



	// ���� ���� ���� üũ
		if (nzYesNo == CTL_YES)
		{
			for(i=0; i<MAX_PICKER_; i++)
			{
				// �˻��� ��Ŀ�� ������ ��쿡�� �˻�
				if (nzPickerInfo[TYPE_FLAG_][i] == CTL_YES)
				{
					
					// ���� ���� ���� Ȯ��
					if (nzPickerInfo[TYPE_SEN_][i] == CTL_NO)
					{
						if (nzSite == 0)	// �׸��� Vacuum
						{
							// 115000 0 00 "ROBOT_GRIP_1_VAC_ON_ERR"
							//(alarm.mstr_code).Format("%06d", 115000+i);	// �˶� �ڵ� ����
							//kwlee 20170910
							if (i < 2)
							{ 
								//150001 0 00 "PS1408_PS1409_BUFFER_REAR_ACCY_ON_CHK_ERR."
								(alarm.mstr_code).Format("%06d", 150101);
							}
							else
							{
								//"PS1406_PS1407_BUFFER_FRONT_ACCY_ON_CHK_ERR."
								(alarm.mstr_code).Format("%06d", 150001);
							}
							
							nFuncRet = CTL_ERROR;
						}
					}
				}
			}
		}
		// ���� ������ ���� üũ
		else
		{
			for(i=0; i<MAX_PICKER_; i++)
			{
				if (nzPickerInfo[TYPE_FLAG_][i] == CTL_YES)
				{
					if (nzPickerInfo[TYPE_SEN_][i] == CTL_YES)
					{
						if (nzSite == 0)	// �׸��� Vacuum
						{
							// 115010 0 00 "ROBOT_GRIP_1_VAC_OFF_ERR"
							//(alarm.mstr_code).Format("%06d", 115010+i);	// �˶� �ڵ� ����
							//kwlee 20170910
							if (i < 2)
							{ 
								//150001 0 00 "PS1408_PS1409_BUFFER_REAR_ACCY_ON_CHK_ERR."
								(alarm.mstr_code).Format("%06d", 150102);
							}
							else
							{
								//"PS1406_PS1407_BUFFER_FRONT_ACCY_ON_CHK_ERR."
								(alarm.mstr_code).Format("%06d", 150002);
							}
							nFuncRet = CTL_ERROR;
						}
					}
				}
			}
		}
	//--------------------------------------------------------------//
	}
	return nFuncRet;
}	


void CPublic_Function::OnLogBCRData(CString sz_Step)
{
	CString sTmp;
	if (0)
	{
		sTmp = "--------------------- Work Start ----------------------";
		cLOG.OnLogEvent( LOG_BCR_, sTmp );

		sTmp.Format("[Step = %s]", sz_Step);
		cLOG.OnLogEvent( LOG_BCR_, sTmp );

		sTmp.Format("AccyMathCode = %s", st_work.m_sRcvAccyMathCode);
		cLOG.OnLogEvent( LOG_BCR_, sTmp );

		sTmp.Format("[RbtBCR[0] = %s], [RbtBCR[1] = %s], [RbtBCR[2] = %s], [RbtBCR[3] = %s]",
			st_work.sRbtBCR_Data[0], st_work.sRbtBCR_Data[1],
			st_work.sRbtBCR_Data[2], st_work.sRbtBCR_Data[3]);
		cLOG.OnLogEvent( LOG_BCR_, sTmp );

		sTmp.Format("[NGBufferBCR[0] = %s], [NGBufferBCR[1] = %s], [NGBufferBCR[2] = %s], [NGBufferBCR[3] = %s]",
			st_work.sNGBufferBCR_Data[0], st_work.sNGBufferBCR_Data[1],
			st_work.sNGBufferBCR_Data[2], st_work.sNGBufferBCR_Data[3]);
		cLOG.OnLogEvent( LOG_BCR_, sTmp );

		sTmp.Format("[BufferConvBCR[0] = %s], [BufferConvBCR[1] = %s], [BufferConvBCR[2] = %s], [BufferConvBCR[3] = %s]",
			st_work.sBufferConvBCR_Data[0], st_work.sBufferConvBCR_Data[1],
			st_work.sBufferConvBCR_Data[2], st_work.sBufferConvBCR_Data[3]);
		cLOG.OnLogEvent( LOG_BCR_, sTmp );

		sTmp = "--------------------- Work End ----------------------";
		cLOG.OnLogEvent( LOG_BCR_, sTmp );
	}
}