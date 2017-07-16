// Seq_XYZRobot.cpp: implementation of the CSeq_XYZRobot class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "handler.h"
#include "Seq_XYZRobot.h"
#include <TIME.H>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CSeq_XYZRobot	SeqXYZRobot;

CSeq_XYZRobot::CSeq_XYZRobot()
{
	// ���� ��ȣ �Ҵ�
	m_nMotY = M_BCRREAD_ROBOT_Y;
	m_nMotX = M_BCRREAD_ROBOT_X;
	m_nMotZ = M_BCRREAD_ROBOT_Z;

	// ���� �� ����
	m_nMotZ0 = M_LIFTER_Z1;
	m_nMotZ1 = M_LIFTER_Z2;
	m_nMotZ2 = M_LIFTER_Z3;
	m_nMotZ3 = M_LIFTER_Z4;
}

CSeq_XYZRobot::~CSeq_XYZRobot()
{

}

//==================================================================//
// ������ ����
//==================================================================//
void CSeq_XYZRobot::OnSeq_RUN(void) 
{
	OnSeq_INIT();     // ������ ���� ���� �ʱ�ȭ
	OnSeq_Execute();  // ������ ����
}

//==================================================================//
// ������ ���� ���� �ʱ�ȭ
//==================================================================//
void CSeq_XYZRobot::OnSeq_INIT(void) 
{
	if( m_bflag_INIT == true)
		return;

	m_bflag_INIT = true;

	CTL_Lib.OnReset_SingleMoveStep( m_nMotX );
	CTL_Lib.OnReset_SingleMoveStep( m_nMotY );
	CTL_Lib.OnReset_SingleMoveStep( m_nMotZ );

	m_nStep_Init = 0;
	m_nStep_Run  = 0;
	m_nStep_Rcvy = 0;

	m_nStep_BCRRead	= 0;
	///m_nStep_BCRBad	= 0;
	m_nStep_NG_Buff	= 0;
	m_nStep_ForceDischarge	= 0;
	m_nStep_NGAccyOut = 0;
	m_nStep_ForceDischarge = 0;
	m_nStep_BufferConvClear = 0;
	m_nStep_ReadMove = 0;
	m_nStep_WorkSiteChange = 0;
	m_nStep_AccyVacPick = 0;
	m_nStep_AccyPlace = 0;
	m_nStep_BCRBad = 0;
	m_nStep_BufferConvPlace = 0;
	m_nStep_NGBufferPick = 0;
	m_nStep_NGBufferPlace = 0;

	m_nStep_GripRetry = 0;

	m_nGripRetryCnt = 0;

	
	st_map.nSupplyTypeMath	= 0;

	m_nBCRReadAllBadCount = 0;

	st_work.m_sCurrntAccyMathCode = "";

	// [����] ���� ���� ��û �÷���
	stSync.nRcvyComplete[SITE_XYZ_ROBOT_] = CTL_YES;
}

//==================================================================//
// ������ ����
//==================================================================//
void CSeq_XYZRobot::OnSeq_Execute(void) 
{
	// ������ ���ͷ�Ʈ ���� Ȯ��
	if (Func.OnIsInterruptCondition() == 1)
	{
		return;
	}
	int nRet = 0;

	switch( st_work.nEqpStatus )
	{
	case dINIT:
		if( st_handler.nInitTryREQ != YES )
		{
			break;
		}
		OnRun_Initial();
		break;

	case dRUN:
		// ���� ���� ���� �� ������ ���� ������
		if(stSync.nRcvyComplete[SITE_XYZ_ROBOT_] == CTL_YES )
		{
			//nRet = OnRcvy_RobotPos();	// XYZRobot ���� ������ ��ġ ���� ����
			nRet = CTL_GOOD;
			if(nRet == CTL_GOOD)
			{
				stSync.nRcvyComplete[SITE_XYZ_ROBOT_] = CTL_YES;
			}
		}
		else
		{
			// �ʱ�ȭ �۾��� �Ϸ���� ������ [AUTO] �۾� ���� �Ұ���
			// - st_handler.nIsAllSiteInitEnd : �ʱ�ȭ ȭ�鿡�� ����
			if( st_handler.nIsAllSiteInitEnd == CTL_YES )
			{
				OnRun_Move();
			}
		}
		break;

	default:
		if( stSync.nRcvyComplete[SITE_XYZ_ROBOT_] != CTL_NO )
		{
			stSync.nRcvyComplete[SITE_XYZ_ROBOT_] = CTL_NO;
		}

		if( st_work.nForceDischarge != FORCEDISCHARGE_ON )
		{
			CTL_Lib.OnReset_SingleMoveStep( m_nMotX );
			CTL_Lib.OnReset_SingleMoveStep( m_nMotY );
			CTL_Lib.OnReset_SingleMoveStep( m_nMotZ );
		}

		// Run �����϶��� �ƴ� Stop �����϶� ������ �ϵ��� ����.
		OnRun_ForceDischarge();
		break;

	}
}

void CSeq_XYZRobot::OnRun_Initial() 
{
	if( st_handler.mn_init_state[INIT_XYZ_ROBOT] != CTL_NO )
		return;
	int nRet = VAR_INIT_;
	int i ;
	int nRetData[4] = {0,};

	switch( m_nStep_Init )
	{
	case 0:
		memset( st_map.nXYZRbtPickerInfo, 0x00, sizeof( st_map.nXYZRbtPickerInfo ) );
		st_map.nNGOutCNT = 0;	// NG ���ۿ� ������ �Ǽ��縮 ����
		m_nStep_Init = 100;
		break;

	case 100://picker up
		for(i=0; i<MAX_PICKER_; i++)
		{
			m_nPickerUnDn[TYPE_FLAG_][i] = CTL_YES;
		}
		OnSet_SolVacuum(IO_OFF, m_nPickerUnDn[TYPE_FLAG_]);
		m_nStep_Init = 110;
		break;

	case 110:
		nRet = OnGet_PickerUpDn( IO_OFF, m_nPickerUnDn[TYPE_FLAG_], m_nPickerUnDn[TYPE_SEN_] );
		if (nRet == CTL_GOOD)
		{
			m_nStep_Init = 120;
		}
		else if( nRet == CTL_ERROR )
		{
			m_nStep_Init = 100;
			CTL_Lib.Alarm_Error_Occurrence( 2000, CTL_dWARNING, alarm.mstr_code);
		}
		break;

	case 120://Pitch close
		OnSet_PickerPitchOpenClose( IO_ON );
		m_nStep_Init = 130;
		break;

	case 130:
		nRet = OnGet_PickerPitch( IO_ON );
		if( nRet == CTL_GOOD)
		{
			m_nStep_Init = 120;
		}
		else if( nRet == CTL_ERROR )
		{
			m_nStep_Init = 100;
			CTL_Lib.Alarm_Error_Occurrence( 2000, CTL_dWARNING, alarm.mstr_code);
		}
		break;

	case 200:
		nRet = g_comiMgr.HomeCheck_Mot( m_nMotZ, 0, MOT_TIMEOUT );
		if( nRet == CTLBD_RET_GOOD )
		{
			m_nStep_Init = 210;
		}
		else if( nRet == CTLBD_RET_ERROR )
		{
			// 070003 0 07 "BARCODE_ROBOT_Z_HOMING_ERR."
			alarm.mstr_code		= "070003";
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
			CTL_Lib.Alarm_Error_Occurrence( 2000, CTL_dWARNING, alarm.mstr_code);
		}
		break;

	case 210:
		nRet = CTL_Lib.OnSingleMove( m_nMotZ, st_motor[m_nMotZ].d_pos[Z_RBT_SAFETY_], (int)st_handler.md_run_speed );
		if( nRet == CTLBD_RET_GOOD )
		{
			m_nStep_Init = 220;
		}
		else if( nRet == CTLBD_RET_ERROR || nRet == CTLBD_RET_RETRY )
		{
			// 071005 0 00 "BARCODE_ROBOT_Z_RBT_SAFETY_MOVE_ERR."
			alarm.mstr_code		= "071005";
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
			CTL_Lib.Alarm_Error_Occurrence( 2000, CTL_dWARNING, alarm.mstr_code);
		}
		break;

	case 220:
		// �Ǽ��縮 ���޿� �����Ͱ� Homing �۾� �Ϸ��Ҷ����� ��ٸ�
		// - ������ �־� �����Ͱ� �ִ�� �ö�� �ִ� ��� �Ǽ��縮�� ���� �׿����� �Ǽ� xyz �κ��� �浹 ���ɼ� ����
		// - �ⱸ������ �ļ� ������ ������ �Ǽ��縮�� ���� �ٴڿ� ����� �� �־� ���� �߰���
		if( st_work.nLiftDownComplete == CTL_YES )
		{
			m_nStep_Init = 230;
		}
		break;

	case 230:
		nRet = g_comiMgr.HomeCheck_Mot( m_nMotY, 0, MOT_TIMEOUT );
		if( nRet == CTLBD_RET_GOOD )
		{
			m_nStep_Init = 240;
		}
		else if( nRet == CTLBD_RET_ERROR )
		{
			// 050003 0 00 "BARCODE_ROBOT_Y_HOMING_ERR."
			alarm.mstr_code		= "050003";
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
			CTL_Lib.Alarm_Error_Occurrence( 2001, CTL_dWARNING, alarm.mstr_code);
		}
		break;

	case 240:
		m_dTargetPos = st_motor[m_nMotY].d_pos[Y_RBT_ACC_NG_OUT_];
		nRet = CTL_Lib.OnSingleMove(m_nMotY, m_dTargetPos, (int)st_handler.md_run_speed);
		if (nRet == CTLBD_RET_GOOD)
		{
			m_nStep_Init = 300;
		}
		else if (nRet == CTLBD_RET_ERROR || nRet == CTLBD_RET_RETRY)
		{
			// 051007 0 00 "BARCODE_ROBOT_Y_RBT_ACC_NG_OUT_MOVE_ERR."
			alarm.mstr_code		= "051007";
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
			CTL_Lib.Alarm_Error_Occurrence( 2001, CTL_dWARNING, alarm.mstr_code);
		}
		break;

	case 300:
		for(i=0; i<MAX_PICKER_; i++)
		{
			if (i < 2)
			{
				m_nPickerUnDn[TYPE_FLAG_][i] = CTL_YES;
				m_nPicker[TYPE_FLAG_][i] = CTL_YES;
			}
			else
			{
				m_nPickerUnDn[TYPE_FLAG_][i] = CTL_NO;
				m_nPicker[TYPE_FLAG_][i] = CTL_NO;
			}
		}
		// [0:�¾�, 1:���] 2���� Vacuum ����
		OnSet_PickerUpDn(IO_ON, m_nPickerUnDn[TYPE_FLAG_]);
		OnSet_SolVacuum(IO_OFF, m_nPicker[TYPE_FLAG_]);	// �׸��� Vacuum [ON:��������, OFF:��������]
		m_nStep_Init = 310;


	case 310:
		nRetData[0] = OnGet_PickerUpDn(IO_ON, m_nPickerUnDn[TYPE_FLAG_], m_nPickerUnDn[TYPE_SEN_] );
		nRetData[1] = OnGet_SolVacuum(IO_OFF, m_nPicker[TYPE_FLAG_], m_nPicker[TYPE_SEN_]);
		// ����Ǿ��� ������ �Ϸ�ɶ����� ���
		if (nRetData[0] == CTL_GOOD && nRetData[1] == CTL_GOOD)
		{
			m_lTime_GoesBy[0] = GetCurrentTime();
			m_nStep_Init = 320;
		}
		else if( nRetData[0] == CTL_ERROR || nRetData[1] == CTL_ERROR )
		{
			m_nStep_Init = 300;
			CTL_Lib.Alarm_Error_Occurrence( 2001, CTL_dWARNING, alarm.mstr_code);
		}
		break;

	case 320:
		nRet = Func.OnGet_PickerStatus(0, CTL_NO, m_nPicker);
		if( nRet == CTL_GOOD )
		{

			m_nStep_Init = 330;
		}
		else if (nRet == CTL_ERROR)
		{
			m_lTime_GoesBy[1] = GetCurrentTime();
			m_lTime_GoesBy[2] = m_lTime_GoesBy[1] - m_lTime_GoesBy[0];
			if (m_lTime_GoesBy[2] < 0)
			{
				m_lTime_GoesBy[0] = GetCurrentTime();
				break;
			}
			
			if (m_lTime_GoesBy[2] > 10)
			{
				// OnGet_PickerStatus() �Լ� �ȿ��� �˶� �ڵ� ������
				alarm.mn_count_mode	= 0;
				alarm.mn_type_mode	= eWARNING;
				st_work.nEqpStatus	= dWARNING;
				CTL_Lib.Alarm_Error_Occurrence( 3000, CTL_dWARNING, alarm.mstr_code );
			}
		}
		break;

	case 330:
		for(i=0; i<MAX_PICKER_; i++)
		{
			m_nPickerUnDn[TYPE_FLAG_][i] = CTL_YES;
		}
		OnSet_SolVacuum(IO_OFF, m_nPickerUnDn[TYPE_FLAG_]);
		m_nStep_Init = 340;
		break;
		
	case 340:
		nRet = OnGet_PickerUpDn( IO_OFF, m_nPickerUnDn[TYPE_FLAG_], m_nPickerUnDn[TYPE_SEN_] );
		if (nRet == CTL_GOOD)
		{
			m_nStep_Init = 350;
		}
		else if( nRet == CTL_ERROR )
		{
			m_nStep_Init = 330;
			CTL_Lib.Alarm_Error_Occurrence( 2000, CTL_dWARNING, alarm.mstr_code);
		}
		break;

		// ��Ŀ�� ���ʿ� �׸��� �� Vacuum�� ���۽�Ű�� ���Ͽ� Y�� ���� �̵�
	case 350:
		// �Ǽ��縮 �ڽ� ���Ա� ������ Home �����̹Ƿ� ���ʿ� �׸����� Vacuum�� �����ϱ� ���ؼ��� (+)����� ��
		m_dTargetPos = st_motor[m_nMotY].d_pos[Y_RBT_ACC_NG_OUT_] + st_accy.d_Y_Gripper_Size;
		
		nRet = CTL_Lib.OnSingleMove(m_nMotY, m_dTargetPos, (int)st_handler.md_run_speed);
		if (nRet == CTLBD_RET_GOOD)
		{
			m_nStep_Init = 360;
		}
		else if (nRet == CTLBD_RET_ERROR || nRet == CTLBD_RET_RETRY)
		{
			// 051007 0 00 "BARCODE_ROBOT_Y_RBT_ACC_NG_OUT_MOVE_ERR."
			alarm.mstr_code		= "051007";
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
			CTL_Lib.Alarm_Error_Occurrence( 2001, CTL_dWARNING, alarm.mstr_code);
		}
		break;

	case 360://Pitch close
		OnSet_PickerPitchOpenClose( IO_ON );
		m_nStep_Init = 370;
		break;
		
	case 370:
		nRet = OnGet_PickerPitch( IO_ON );
		if( nRet == CTL_GOOD)
		{
			m_nStep_Init = 380;
		}
		else if( nRet == CTL_ERROR )
		{
			m_nStep_Init = 360;
			CTL_Lib.Alarm_Error_Occurrence( 2000, CTL_dWARNING, alarm.mstr_code);
		}
		break;		
		
	case 380:
		for(i=0; i<MAX_PICKER_; i++)
		{
			if (i < 2)
			{
				m_nPickerUnDn[TYPE_FLAG_][i] = CTL_NO;
				m_nPicker[TYPE_FLAG_][i] = CTL_NO;
			}
			else
			{
				m_nPickerUnDn[TYPE_FLAG_][i] = CTL_YES;
				m_nPicker[TYPE_FLAG_][i] = CTL_YES;
			}
		}
		OnSet_PickerUpDn(IO_ON, m_nPickerUnDn[TYPE_FLAG_]);
		// [2:�µ�, 3:���] 2���� Vacuum ����
		OnSet_SolVacuum(IO_OFF, m_nPicker[TYPE_FLAG_]);	// �׸��� Vacuum [ON:��������, OFF:��������]
		m_nStep_Init = 390;
		break;
		
	case 390:
		nRetData[0] = OnGet_PickerUpDn(IO_ON, m_nPickerUnDn[TYPE_FLAG_], m_nPickerUnDn[TYPE_SEN_] );
		nRetData[1] = OnGet_SolVacuum(IO_OFF, m_nPicker[TYPE_FLAG_], m_nPicker[TYPE_SEN_]);
		// ����Ǿ��� ������ �Ϸ�ɶ����� ���
		if (nRetData[0] == CTL_GOOD && nRetData[1] == CTL_GOOD)
		{
			m_lTime_GoesBy[0] = GetCurrentTime();
			m_nStep_Init = 400;
		}
		else if(nRetData[0] == CTL_ERROR || nRetData[1] == CTL_ERROR)
		{
			m_nStep_Init = 380;
			CTL_Lib.Alarm_Error_Occurrence( 2000, CTL_dWARNING, alarm.mstr_code);
		}
		break;
		
	case 400:
		// �׸����� ���� ���� ���� ��ȯ
		// - m_nPicker : [case 250:] ���� ������
		nRet = Func.OnGet_PickerStatus(0, CTL_NO, m_nPicker);
		if (nRet == CTL_ERROR)
		{
			m_lTime_GoesBy[1] = GetCurrentTime();
			m_lTime_GoesBy[2] = m_lTime_GoesBy[1] - m_lTime_GoesBy[0];
			if (m_lTime_GoesBy[2] < 0)
			{
				m_lTime_GoesBy[0] = GetCurrentTime();
				break;
			}
			
			if (m_lTime_GoesBy[2] > 10)
			{
				// OnGet_PickerStatus() �Լ� �ȿ��� �˶� �ڵ� ������
				alarm.mn_count_mode	= 0;
				alarm.mn_type_mode	= eWARNING;
				st_work.nEqpStatus	= dWARNING;
				CTL_Lib.Alarm_Error_Occurrence( 3000, CTL_dWARNING, alarm.mstr_code );
			}
			break;
		}
		m_nStep_Init = 410;

	case 410:
		for(i=0; i<MAX_PICKER_; i++)
		{
			m_nPickerUnDn[TYPE_FLAG_][i] = CTL_YES;
		}
		OnSet_SolVacuum(IO_OFF, m_nPickerUnDn[TYPE_FLAG_]);
		m_nStep_Init = 420;
		break;
		
	case 420:
		nRet = OnGet_PickerUpDn( IO_OFF, m_nPickerUnDn[TYPE_FLAG_], m_nPickerUnDn[TYPE_SEN_] );
		if (nRet == CTL_GOOD)
		{
			m_nStep_Init = 500;
		}
		else if( nRet == CTL_ERROR )
		{
			m_nStep_Init = 410;
			CTL_Lib.Alarm_Error_Occurrence( 2000, CTL_dWARNING, alarm.mstr_code);
		}
		break;

	case 500:
		// ���ڵ� ������ [����->����] ����Ǹ鼭 ������ �߻�
		// - Y�� ������ Homing ��ġ���� X�� ���� Homing �۾��ϸ� ���ڵ� ������ ���Ա��� �ⱸ���� �浹 �߻�
		nRet = g_comiMgr.HomeCheck_Mot(m_nMotX, 0, MOT_TIMEOUT);
		if (nRet == CTLBD_RET_GOOD)
		{
			m_nStep_Init = 600;
		}
		else if (nRet == CTLBD_RET_ERROR)
		{
			// 060003 0 00 "BARCODE_ROBOT_X_HOMING_ERR."
			alarm.mstr_code		= "060003";
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
			CTL_Lib.Alarm_Error_Occurrence( 2000, CTL_dWARNING, alarm.mstr_code);
		}
		break;

	case 600:
		for(i=0; i<2; i++)
		{
			m_nMotflag[i] = CTL_NO;
		}
		
		// �ʱ� ��� ��ġ [NG ���� ��ġ]
		nRetData[0] = COMI.Start_SingleMove(m_nMotX, st_motor[m_nMotX].d_pos[X_RBT_ACC_BCR_],		(int)st_handler.md_run_speed);
		nRetData[1] = COMI.Start_SingleMove(m_nMotY, st_motor[m_nMotY].d_pos[Y_RBT_ACC_NG_BUFFER_],	(int)st_handler.md_run_speed);
		if (nRetData[0] == CTLBD_RET_GOOD)
		{
			m_nMotflag[0] = CTL_YES;
		}
		if (nRetData[1] == CTLBD_RET_GOOD)
		{
			m_nMotflag[1] = CTL_YES;
		}
		
		if (m_nMotflag[0] == CTL_YES && m_nMotflag[1] == CTL_YES)
		{
			for(i=0; i<2; i++)
			{
				m_nMotflag[i] = CTL_NO;
			}
			m_nStep_Init = 610;
		}
		else if (nRetData[0] == CTLBD_RET_RETRY ||	nRetData[1] == CTLBD_RET_RETRY)
		{
			m_nStep_Init = 600;
		}
		else if (nRetData[0] != CTLBD_RET_PROCEED && nRetData[0] != CTLBD_RET_GOOD)
		{
			m_nStep_Init = 600;
			
			// 061000 0 00 "BARCODE_ROBOT_X_RBT_ACC_BCR_POS_MOVE_ERR."
			alarm.mstr_code		= "061000";
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
			CTL_Lib.Alarm_Error_Occurrence( 2000, CTL_dWARNING, alarm.mstr_code);
		}
		else if (nRetData[1] != CTLBD_RET_PROCEED && nRetData[1] != CTLBD_RET_GOOD)
		{
			m_nStep_Init = 600;
			
			// 051008 0 00 "BARCODE_ROBOT_Y_RBT_ACC_NG_BUFFER_MOVE_ERR."
			alarm.mstr_code		= "051008";
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
			CTL_Lib.Alarm_Error_Occurrence( 2000, CTL_dWARNING, alarm.mstr_code);
		}
		break;

	case 610:
		nRetData[0] = COMI.Check_SingleMove(m_nMotX, st_motor[m_nMotX].d_pos[X_RBT_ACC_BCR_]);
		nRetData[1] = COMI.Check_SingleMove(m_nMotY, st_motor[m_nMotY].d_pos[Y_RBT_ACC_NG_BUFFER_]);
		if (nRetData[0] == CTLBD_RET_GOOD)
		{
			m_nMotflag[0] = CTL_YES;
		}
		if (nRetData[1] == CTLBD_RET_GOOD)
		{
			m_nMotflag[1] = CTL_YES;
		}
		
		if (m_nMotflag[0] == CTL_YES &&	m_nMotflag[1] == CTL_YES)
		{
			for(i=0; i<2; i++)
			{
				m_nMotflag[i] = CTL_NO;
			}
			m_nStep_Init = 700;
		}
		else if (nRetData[0] == CTLBD_RET_RETRY || nRetData[1] == CTLBD_RET_RETRY)
		{
			m_nStep_Init = 600;
		}
		else if (nRetData[0] != CTLBD_RET_PROCEED && nRetData[0] != CTLBD_RET_GOOD)
		{
			m_nStep_Init = 600;
			
			// 061000 0 00 "BARCODE_ROBOT_X_RBT_ACC_BCR_POS_MOVE_ERR."
			alarm.mstr_code		= "061000";
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
			CTL_Lib.Alarm_Error_Occurrence( 2000, CTL_dWARNING, alarm.mstr_code);
		}
		else if (nRetData[1] != CTLBD_RET_PROCEED && nRetData[1] != CTLBD_RET_GOOD)
		{
			m_nStep_Init = 600;
			
			// 051008 0 00 "BARCODE_ROBOT_Y_RBT_ACC_NG_BUFFER_MOVE_ERR."
			alarm.mstr_code		= "051008";
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
			CTL_Lib.Alarm_Error_Occurrence( 2000, CTL_dWARNING, alarm.mstr_code);
		}		
		break;

	case 700:
		//middle, out front rear box check
		nRetData[0] = OnCheck_MiddleAccySupply(IO_ON, CHK_NOT_EXIST_, SIDE_LEFT_);
		nRetData[1] = OnCheck_MiddleAccySupply(IO_ON, CHK_NOT_EXIST_, SIDE_RIGHT_);
		nRetData[2] = OnCheck_OutFrontAccySupply(IO_ON, CHK_NOT_EXIST_, SIDE_LEFT_);
		nRetData[3] = OnCheck_OutFrontAccySupply(IO_ON, CHK_NOT_EXIST_, SIDE_RIGHT_);
		nRetData[4] = OnCheck_OutRearAccySupply(IO_ON, CHK_NOT_EXIST_, SIDE_LEFT_);
		nRetData[5] = OnCheck_OutRearAccySupply(IO_ON, CHK_NOT_EXIST_, SIDE_RIGHT_);
		if (nRetData[0] == IO_ON || nRetData[1] == IO_ON || nRetData[2] == IO_ON || nRetData[3] == IO_ON || nRetData[4] == IO_ON || nRetData[5] == IO_ON )
		{
			//125000 0 00 "PS1006_SLOP_MIDDLE_LEFT_ACCY_DETECTION_CHK_ERR."
			//125001 0 00 "PS1007_SLOP_MIDDLE_ACCY_DETECTION_CHK_ERR."
			//126000 0 00 "PS1406_SLOP_FRONT_LEFT_ACCY_DETECTION_CHK_ERR."
			//126001 0 00 "PS1407_SLOP_FRONT_RIGHT_ACCY_DETECTION_CHK_ERR."
			//127000 0 00 "PS1408_SLOP_REAR_LEFT_ACCY_DETECTION_CHK_ERR."
			//127001 0 00 "PS1409_SLOP_REAR_RIGHT_ACCY_DETECTION_CHK_ERR."
			if (nRetData[0] == IO_ON || nRetData[1] == IO_ON )
			{
				if (nRetData[0] == IO_ON) alarm.mstr_code = "125000";
				else					  alarm.mstr_code = "125001";
			}
			else if (nRetData[2] == IO_ON || nRetData[3] == IO_ON )
			{
				if (nRetData[2] == IO_ON) alarm.mstr_code = "126000";
				else					  alarm.mstr_code = "126001";
			}
			else
			{
				if (nRetData[4] == IO_ON) alarm.mstr_code = "127000";
				else					  alarm.mstr_code = "127001";
			}
			CTL_Lib.Alarm_Error_Occurrence( 2000, CTL_dWARNING, alarm.mstr_code);
		}
		else
		{
			m_nStep_Init = 1000;
		}
		break;

	case 1000:
		nRetData[0] = OnCheck_NGBufferAccy(IO_ON,	CHK_NOT_EXIST_, SIDE_LEFT_);
		nRetData[1] = OnCheck_NGBufferAccy(IO_ON,	CHK_NOT_EXIST_, SIDE_RIGHT_);
		
		if (nRetData[0] == IO_OFF && nRetData[1] == IO_OFF)
		{
			memset(st_map.nNGBuffer, 0, sizeof(st_map.nNGBuffer));
			m_nStep_Init = 1200;
		}
		else
		{
			m_nStep_Init = 1100;
		}
		break;

	case 1100:
		// NG ������ �Ǽ��縮 ����
		nRet = OnProc_NGBufferClear();	// NG ������ �Ǽ��縮 ����
		if (nRet == CTLBD_RET_GOOD)
		{
			// �ٽ� �ѹ� NG ���ۿ� �Ǽ��縮 �����ϴ��� ��˻�
			m_nStep_Init = 1000;
		}
		break;

	case 1200:
		nRetData[0] = OnCheck_FrontAccySupply(IO_ON, CHK_NOT_EXIST_, SIDE_LEFT_);
		nRetData[1] = OnCheck_FrontAccySupply(IO_ON, CHK_NOT_EXIST_, SIDE_RIGHT_);
		nRetData[2] = OnCheck_RearAccySupply(IO_ON,	 CHK_NOT_EXIST_, SIDE_LEFT_);
		nRetData[3] = OnCheck_RearAccySupply(IO_ON,	 CHK_NOT_EXIST_, SIDE_RIGHT_);
		
		if (nRetData[0] == IO_OFF && nRetData[1] == IO_OFF &&
			nRetData[2] == IO_OFF && nRetData[3] == IO_OFF)
		{
			// Conv ������ ���� ���� ���� �Ŀ��� [case 1200:] ȣ��ǹǷ� NG ���۷� Y�� ���� �̵���Ŵ
			m_nStep_Init = 2000;
		}
		else
		{
			m_nStep_Init = 1300;
		}
		break;

	case 1300:
		// Buffer Conv�� Init ���°� �Ϸ� �Ǵ� �غ� �ɶ����� ���
		if (st_handler.mn_init_state[INIT_BUFFER_CONV] == CTL_YES ||
			st_handler.mn_init_state[INIT_BUFFER_CONV] == CTL_READY)
		{
			m_nStep_Init = 1400;
		}
		break;

	case 1400:
		// Conv ������ �Ǽ��縮 ����
		nRet = OnProc_BufferConvClear();
		if (nRet == CTLBD_RET_GOOD)
		{
			m_nStep_Init = 1200;
		}
		break;

	case 2000:
		nRet = CTL_Lib.OnSingleMove(m_nMotY, st_motor[m_nMotY].d_pos[Y_RBT_ACC_NG_BUFFER_], (int)st_handler.md_run_speed);
		if (nRet == CTLBD_RET_GOOD)
		{
			m_nStep_Init = 3000;
		}
		else if (nRetData[1] == CTLBD_RET_ERROR || nRetData[1] == CTLBD_RET_RETRY)
		{
			// 051008 0 00 "BARCODE_ROBOT_Y_RBT_ACC_NG_BUFFER_MOVE_ERR."
			alarm.mstr_code		= "051008";
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
			CTL_Lib.Alarm_Error_Occurrence( 3000, CTL_dWARNING, alarm.mstr_code);
		}
		break;

	case 3000:
		stSync.nResp_XYZRbt2Lifter_Work = SYNC_RESP_RESET_;
		stSync.nReq_XYZRbt2BufferAlaignConv_Work = SYNC_REQ_RESET_;
		
		// �ʱ�ȭ ȭ�鿡�� [�ʱ�ȭ] �Ϸ� ǥ�� ����
		st_handler.mn_init_state[INIT_XYZ_ROBOT] = CTL_YES;
		m_nStep_Init = 0;
		break;


	}

}

void CSeq_XYZRobot::OnRun_Move() 
{
	// ��ü ����Ʈ ���� ���� �Ϸ� ���� Ȯ��
	// - ��� �κ��� ���� ������ �Ϸ�� �Ŀ��� ������ �����ϵ��� ��
	// : ���� ���� �߿� ������ �����ϸ� �����ϴ� ������ �浹�� �߻��� �� ����
	if (Func.OnIsAllRcvyComplete() != CTL_YES)
	{
		return;
	}
	int i = 0, nRet = VAR_INIT_;
	int nRetData[4] = {0,};
	int nDeviceType = 0, nSum = 0, nAccyCodeMathChk = 0;
	CString tmp, sSndMeg;

	Func.OnTrace_ThreadStep(9, m_nStep_Run);	// ������ ���� ���� TRACE

	switch( m_nStep_Run )
	{

	case 0:
		// Lot Start ���°� �ɶ����� ���
		if( st_map.nLotStart != CTL_YES )
		{
			break;
		}
		// Box Lift�� Lot End ���� �Ǵ� �ڽ� ���� ������ ���
		if( stSync.nLotEnd_BoxLifter == TRUE)
		{
			st_map.nSupplyTypeMath = 0;
			stSync.nLotEnd_XYZRobot = TRUE;
			break;
		}
		else
		{
			stSync.nLotEnd_XYZRobot = FALSE;
		}

		// �����Ϳ��� �Ǽ��縮 �غ� �Ǿ����� Ȯ��
		// - �غ�� : �����Ͱ� �غ��� �Ǽ��縮 ������ �̵�
		// - ���غ� : NG ���� �������� �ϴ� �̵� [�̵� �ð��� ���̱� ����]
		if( stSync.nReq_Lifter2XYZRbt_Work == SYNC_REQ_ACC_LIFT_COMPLETE_ )
		{
			m_nStep_Run = 1000;
		}
		else
		{
			m_nStep_Run = 100;
		}
		break;

		//==========================================================//
		// NG ���� �������� �ϴ� �̵���Ŵ
		//==========================================================//
	case 100:
		nRet = CTL_Lib.OnSingleMove(m_nMotZ, st_motor[m_nMotZ].d_pos[Z_RBT_SAFETY_], (int)st_handler.md_run_speed);
		if (nRet == CTLBD_RET_GOOD)
		{
			m_nStep_Run = 200;
		}
		else if (nRet == CTLBD_RET_ERROR || nRet == CTLBD_RET_RETRY)
		{
			// 071005 0 0 "BARCODE_ROBOT_Z_RBT_SAFETY_MOVE_ERR"
			alarm.mstr_code		= "071005";
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2000, CTL_dWARNING, alarm.mstr_code);
		}
		break;

	case 200:
		nRet = CTL_Lib.OnSingleMove(m_nMotY, st_motor[m_nMotY].d_pos[Y_RBT_ACC_NG_BUFFER_], (int)st_handler.md_run_speed);
		if (nRet == CTLBD_RET_GOOD)
		{
			m_nStep_Run = 210;
		}
		else if (nRetData[1] == CTLBD_RET_ERROR || nRetData[1] == CTLBD_RET_RETRY)
		{
			// 051008 0 00 "BARCODE_ROBOT_Y_RBT_ACC_NG_BUFFER_MOVE_ERR."
			alarm.mstr_code		= "051008";
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2000, CTL_dWARNING, alarm.mstr_code);
		}
		break;

	case 210:
		if( stSync.nReq_Lifter2XYZRbt_Work == SYNC_REQ_ACC_LIFT_COMPLETE_ )
		{
			m_nStep_Run = 1000;
		}
		break;

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	case 1000:
		// st_work.m_sRcvAccyMathCode ���� ������ ��� 
		//st_work.m_sRcvAccyMathCode = "LA69-01773A";  //test
		//st_work.m_sRcvAccyMathCode = "LA69-01773B";  //test
		if( st_work.m_sRcvAccyMathCode !="" )
		{
			// ó�� ������ ��� st_work.m_sCurrntAccyMathCode ���� NULL �̱� ������ st_work.m_sRcvAccyMathCode ���� ���� �Ѵ�.
			if( st_work.m_sCurrntAccyMathCode = "" )
			{
				st_work.m_sCurrntAccyMathCode = st_work.m_sRcvAccyMathCode;
				for( i = 0; i<4; i++)
				{
					st_work.sRbtBCR_Data[i] = "";
				}
				m_nStep_Run = 1100;
				Func.OnLogBCRData("[Seq_XYZRobot_RunMove_1000_""]");
			}		
			else
			{
				if( st_work.m_sCurrntAccyMathCode == st_work.m_sRcvAccyMathCode )
				{
					for( i = 0; i < 4; i++ )
					{
						st_work.sRbtBCR_Data[i] = "";
					}
					m_nStep_Run = 1100;
				}
				else
				{
					st_work.m_sCurrntAccyMathCode = "";
					m_nStep_Run = 0;
					st_work.nForceDischargeType = 0;
					st_work.nForceDischarge = FORCEDISCHARGE_ON;
					Func.OnSet_EqpStatusStop();
					break;
				}
			}
		}
		else
		{
			// ���簡 ���� ����̱� ������ st_work.m_sRcvAccyMathCode�� Test�� �־� ����
			if( st_basic.n_mode_device == WITHOUT_DVC_)
			{
				st_work.m_sRcvAccyMathCode = "test";//st_work.m_sRcvAccyMathCode = "TE69-01773A";  //test
			}
		}
		break;

	case 1100:
		// Box Lift�� Lot ���� Ȯ��
		if( stSync.nLotEnd_BoxLifter == TRUE )
		{
			m_nStep_Run = 0;
			break;
		}

		// Accy ���ڵ� �д� �۾� ����
		nRet = OnProc_ReadMove();
		if( nRet == CTL_GOOD)
		{
			Func.OnLogBCRData("[Seq_XYZRobot_RunMove_500_Good]");
			
			if (st_handler.cwnd_main != NULL)
			{
				// XYZ �κ��� ���� ǥ�� ��û
				st_handler.cwnd_main->PostMessage(WM_UPDATE_MAIN, PLACE_XYZ_RBT_);
			}
			m_nStep_Run = 2000;
		}
		else if( nRet == CTL_ERROR )
		{
			m_nStep_Run = 6000;
		}
		break;

	case 2000:
		nRet = OnProc_AccyVacPick();
		
		if (nRet == CTL_GOOD)
		{
			m_nStep_Run = 3000;
		}
		else if (nRet == CTL_ERROR)
		{
			m_nStep_Run = 6000;
		}
		break;

		//////////////////////////////////////////////////////////////////////////
		// Accy Place �۾�
	case 3000:
		nRet = OnProc_AccyPlace();
		
		if (nRet == CTL_GOOD)
		{
			m_nStep_Run = 9000;
		}
		break;


		///////////////////////////////////////////////////////////////////////////
		// Lift Z���� 2mm�� 10ȸ �õ� �� Gripper�� ������ ������ ���� �ʾ��� ���
		// Lift Z���� �����鼭 ���� ������ ���� ���� �ϴ� �κ� 
	case 6000:
		nRet = OnProc_WorkSiteChange();
		if (nRet == CTLBD_RET_GOOD)
		{
			m_nStep_Run = 9000;
		}
		break;
		///////////////////////////////////////////////////////////////////////////

	case 9000:
		stSync.nResp_XYZRbt2Lifter_Work = SYNC_RESP_RESET_;
		// BCR ����ü �� �ʱ�ȭ
		memset(st_map.nBCR_State, -1, sizeof(st_map.nBCR_State));
		
		if (st_handler.cwnd_main != NULL) 
		{
			// ���ڵ� ���� ǥ�� �ʱ�ȭ ��û
			st_handler.cwnd_main->PostMessage(WM_UPDATE_MAIN, READ_BCR_DATA);
		}
		
		m_nStep_Run = 0;
		m_nPickRetryCnt = 0;
		break;

	}
}

//////////////////////////////////////////////////////////////////////////
// Vacuum �� �̿��Ͽ� ���縦 ¤�� �Լ�
//////////////////////////////////////////////////////////////////////////
int CSeq_XYZRobot::OnProc_AccyVacPick()
{

	int nFuncRet = CTL_PROCEED;
	
	int nRet = VAR_INIT_;
	int nRetData[4] = {0,};
	int i = 0;
	int iCheckCount = 0;
	
	Func.OnTrace_ThreadStep(28, m_nStep_AccyVacPick);
	switch(m_nStep_AccyVacPick)
	{
	case 0:
		nRet = CTL_Lib.OnSingleMove(m_nMotZ, st_motor[m_nMotZ].d_pos[Z_RBT_SAFETY_], (int)st_handler.md_run_speed);
		if (nRet == CTLBD_RET_GOOD)
		{
			m_nStep_AccyVacPick = 10;
		}
		else if (nRet == CTLBD_RET_ERROR || nRet == CTLBD_RET_RETRY)
		{
			// 071005 0 0 "BARCODE_ROBOT_Z_RBT_SAFETY_MOVE_ERR"
			alarm.mstr_code		= "071005";
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
			CTL_Lib.Alarm_Error_Occurrence( 3000, CTL_dWARNING, alarm.mstr_code );
		}
		break;

	case 10:
		for(i=0; i<MAX_PICKER_; i++)
		{
			m_nPickerUnDn[TYPE_FLAG_][i] = CTL_YES;
			m_nPicker[TYPE_FLAG_][i] = CTL_YES;	// ������ Vacuum���� ����
		}
		
		// �ð��� ����� �Ŀ� Ʈ���� ������ �� �����Ƿ� �ٽ� �˻�
		m_lTime_GoesBy[0] = GetCurrentTime();
		m_nStep_AccyVacPick = 20;
		break;

	case 20:
		nRet = Func.OnGet_PickerStatus(0, CTL_NO, m_nPicker);
		if (nRet == CTL_ERROR)
		{
			m_lTime_GoesBy[1] = GetCurrentTime();
			m_lTime_GoesBy[2] = m_lTime_GoesBy[1] - m_lTime_GoesBy[0];
			if (m_lTime_GoesBy[2] < 0)
			{
				m_lTime_GoesBy[0] = GetCurrentTime();
				break;
			}
			
			if (m_lTime_GoesBy[2] > 10)
			{
				// OnGet_PickerStatus() �Լ� �ȿ��� �˶� �ڵ� ������
				alarm.mn_count_mode	= 0;
				alarm.mn_type_mode	= eWARNING;
				st_work.nEqpStatus	= dWARNING;
				CTL_Lib.Alarm_Error_Occurrence( 3000, CTL_dWARNING, alarm.mstr_code );
			}
			break;
		}
		m_nStep_AccyVacPick = 50;
		break;

	case 50:
		// �⺻�� Y_RBT_ACC_GRIP_FRONT_�� �ϸ�, �۾� ��ġ�� ���� �̵��� �ϵ��� ����
		nRet = CTL_Lib.OnSingleMove(m_nMotY, st_motor[m_nMotY].d_pos[Y_RBT_ACC_GRIP_FRONT_ + st_map.nLiftWorkSite], (int)st_handler.md_run_speed);
		if(nRet == CTLBD_RET_GOOD)
		{
			m_nStep_AccyVacPick = 200;
		}
		else if (nRet == CTLBD_RET_ERROR || nRet == CTLBD_RET_RETRY)
		{
			// 051005 0 00 "BARCODE_ROBOT_Y_RBT_ACC_GRIP_FRONT_MOVE_ERR."
			// 051006 0 00 "BARCODE_ROBOT_Y_RBT_ACC_GRIP_REAR_MOVE_ERR."
			alarm.mstr_code.Format("05100%d", 5 + st_map.nLiftWorkSite);
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
			CTL_Lib.Alarm_Error_Occurrence( 3000, CTL_dWARNING, alarm.mstr_code );
		}
		break;

	case 200:
		nRet = COMI.Start_SingleMove(m_nMotZ, st_motor[m_nMotZ].d_pos[Z_RBT_ACC_GRIP_], (int)st_handler.md_run_speed);
		if (nRet == CTLBD_RET_GOOD)
		{
			OnSet_PickerUpDn( IO_ON, m_nPickerUnDn[TYPE_FLAG_]);
			OnSet_SolVacuum(IO_ON, m_nPicker[TYPE_FLAG_]);	// �׸��� Vacuum [ON:��������, OFF:��������]
			m_nStep_AccyVacPick = 210;
		}
		else if (nRet == CTLBD_RET_ERROR || nRet == CTLBD_RET_RETRY)
		{
			// 071000 0 0 "BARCODE_ROBOT_Z_RBT_ACC_GRIP_MOVE_ERR"
			alarm.mstr_code		= "071000";
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
			CTL_Lib.Alarm_Error_Occurrence( 3000, CTL_dWARNING, alarm.mstr_code );
		}
		break;


	case 210:
		nRet = COMI.Check_SingleMove(m_nMotZ, st_motor[m_nMotZ].d_pos[Z_RBT_ACC_GRIP_]);
		if (nRet == CTLBD_RET_GOOD)
		{
			m_nStep_AccyVacPick = 300;
		}
		else if (nRet == CTLBD_RET_ERROR || nRet == CTLBD_RET_SAFETY)
		{
			m_nStep_AccyVacPick = 200;
			
			// 071000 0 00 "BARCODE_ROBOT_Z_RBT_ACC_GRIP_MOVE_ERR."
			alarm.mstr_code		= "071000";
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
			CTL_Lib.Alarm_Error_Occurrence( 3000, CTL_dWARNING, alarm.mstr_code );
		}
		else if (nRet == CTLBD_RET_RETRY)
		{
			m_nStep_AccyVacPick = 200;
		}
		break;

	case 300:
		nRetData[0] = g_ioMgr.get_in_bit(stIO.i_Chk_Rbt_VacOn[SIDE_FRONT_][SIDE_LEFT_]);
		nRetData[1] = g_ioMgr.get_in_bit(stIO.i_Chk_Rbt_VacOn[SIDE_FRONT_][SIDE_RIGHT_]);
		nRetData[2] = g_ioMgr.get_in_bit(stIO.i_Chk_Rbt_VacOn[SIDE_REAR_][SIDE_LEFT_]);
		nRetData[3] = g_ioMgr.get_in_bit(stIO.i_Chk_Rbt_VacOn[SIDE_REAR_][SIDE_RIGHT_]);
		
		if (st_basic.n_mode_device == WITHOUT_DVC_)
		{
			m_nStep_AccyVacPick = 500;
			break;
		}
		if (nRetData[0] == IO_ON && nRetData[1] == IO_ON && nRetData[2] == IO_ON && nRetData[3] == IO_ON )
		{
			m_nStep_AccyVacPick = 500;
		}
		else
		{
			memset(dAccyLiftPos, 0, sizeof(dAccyLiftPos));
			m_nStep_AccyVacPick = 310;
		}
		break;

	case 310:
		nRet = OnProc_GripRetry();
		if (nRet == CTL_GOOD)
		{
			memset(dAccyLiftPos, 0, sizeof(dAccyLiftPos));
			m_nGripRetryCnt = 0;
			m_nStep_AccyVacPick = 300;
		}
		else if (nRet == CTL_ERROR)
		{
			nFuncRet = CTL_ERROR;
			
			stSync.nResp_XYZRbt2Lifter_Work = SYNC_RESP_PASS_;
			m_nGripRetryCnt = 0;
			m_nStep_AccyVacPick = 0;
		}
		break;

	case 500:
		nRet = CTL_Lib.OnSingleMove(m_nMotZ, st_motor[m_nMotZ].d_pos[Z_RBT_SAFETY_], (int)st_handler.md_run_speed);
		if (nRet == CTLBD_RET_GOOD)
		{
			m_lTime_GoesBy[0] = GetCurrentTime();
			m_nStep_AccyVacPick = 510;
		}
		else if (nRet == CTLBD_RET_ERROR || nRet == CTLBD_RET_RETRY)
		{
			// 071005 0 0 "BARCODE_ROBOT_Z_RBT_SAFETY_MOVE_ERR"
			alarm.mstr_code		= "071005";
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
			CTL_Lib.Alarm_Error_Occurrence( 3000, CTL_dWARNING, alarm.mstr_code );
		}
		break;
		
	case 510:
		// �׸����� ���� ���� ��ȯ
		// - m_nPicker : [case 100:]���� ������
		nRet = Func.OnGet_PickerStatus(0, CTL_YES, m_nPicker);
		if (nRet == CTL_ERROR)
		{
			m_lTime_GoesBy[1] = GetCurrentTime();
			m_lTime_GoesBy[2] = m_lTime_GoesBy[1] - m_lTime_GoesBy[0];
			if (m_lTime_GoesBy[2] < 0)
			{
				m_lTime_GoesBy[0] = GetCurrentTime();
				break;
			}
			
			if (m_lTime_GoesBy[2] < 100)
			{
				break;
			}
			CTL_Lib.Alarm_Error_Occurrence( 3000, CTL_dWARNING, alarm.mstr_code );
			break;
		}
		
		// ��ü �׸��� ���� ���
		for(i=0; i<MAX_PICKER_; i++)
		{
			// �Ǽ��縮 ���� ���� ������ �׸��ۿ� ���ؼ� �Ǽ��縮�� Vacuum ������ ������ ���
			if (m_nPicker[TYPE_FLAG_][i] == CTL_YES && m_nPicker[TYPE_SEN_][i] == CTL_YES)
			{
				// �׸����� ����ü ���� ���� [�Ǽ��縮 ������]
				st_map.nXYZRbtPickerInfo[i]	= CTL_YES;
				m_nPicker[TYPE_FLAG_][i]	= CTL_NO;
			}
		}
		
		// OnGet_PickerStatus() �Լ������� [CTL_GOOD/CTL_ERROR] ��ȯ�Ǿ��� ����
		if (nRet == CTL_GOOD)
		{
			m_nStep_AccyVacPick = 1000;
			stSync.nResp_XYZRbt2Lifter_Work = SYNC_RESP_WORK_COMPLETE_;
		}
		else
		{
			m_nStep_AccyVacPick = 600;
		}
		break;

		//==========================================================//
		// �˶��� ��� �κ�
		//==========================================================//
	case 600:
		nRet = CTL_Lib.OnSingleMove(m_nMotY, st_motor[m_nMotY].d_pos[Y_RBT_ACC_NG_BUFFER_], (int)st_handler.md_run_speed);
		if(nRet == CTLBD_RET_GOOD)
		{
			m_nStep_AccyVacPick = 0;
			st_work.nForceDischargeType = 1;
			st_work.nForceDischarge = FORCEDISCHARGE_ON;
			Func.OnSet_EqpStatusStop();
		}
		else if (nRet == CTLBD_RET_ERROR || nRet == CTLBD_RET_RETRY)
		{
			// 051008 0 00 "BARCODE_ROBOT_Y_RBT_ACC_NG_BUFFER_MOVE_ERR."
			alarm.mstr_code		= "051008";
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
			CTL_Lib.Alarm_Error_Occurrence( 3000, CTL_dWARNING, alarm.mstr_code );
		}
		break;
		//==========================================================//

	case 1000:
		// [Box Lifter -> XYZRbt] ���� ��û ���
		if (stSync.nReq_Lifter2XYZRbt_Work == SYNC_REQ_RESET_)
		{
			stSync.nResp_XYZRbt2Lifter_Work = SYNC_RESP_RESET_;
			
			nFuncRet = CTL_GOOD;
			m_nStep_AccyVacPick = 0;
		}
		break;

	}

	return nFuncRet;
}

//////////////////////////////////////////////////////////////////////////
// Buffer Conv�� ���縦 ���� �Լ�
//////////////////////////////////////////////////////////////////////////
int CSeq_XYZRobot::OnProc_AccyPlace()
{

	int nFuncRet = CTL_PROCEED;
	
	int nRet = VAR_INIT_;
	int nRetData[4] = {0,};
	int i = 0;
	int iCheckCount = 0;
	CString sTmp;
	Func.OnTrace_ThreadStep(26, m_nStep_AccyPlace);
	
	switch(m_nStep_AccyPlace)
	{
	case 0:
		// BCR ���°� All Good �̸� Buffer Conv ���� Step �̵�
		if(st_map.nBCR_State[0] == BCR_READ_GOOD_ && st_map.nBCR_State[1] == BCR_READ_GOOD_ && st_map.nBCR_State[2] == BCR_READ_GOOD_ && st_map.nBCR_State[3] == BCR_READ_GOOD_)
		{
			stSync.nReq_XYZRbt2BufferAlaignConv_Work = SYNC_REQ_BUFFER_LOADING_READY_;
			m_nStep_AccyPlace = 500;
		}
		// 4���� Accy �� �Ѱ��� Bad �ϰ�� ���� Bad Step ����
		else if(st_map.nBCR_State[0] == BCR_READ_BAD_ || st_map.nBCR_State[1] == BCR_READ_BAD_ || st_map.nBCR_State[2] == BCR_READ_BAD_ || st_map.nBCR_State[3] == BCR_READ_BAD_)
		{
			m_nStep_AccyPlace = 100;
		}
		break;

	case 100:
		nRet = OnProc_BCRStateBad();
		
		if (nRet == CTL_GOOD)
		{
			Func.OnLogBCRData("[Seq_XYZRobot_OnProc_AccyPlace_100_Good]");
			m_nStep_AccyPlace = 1000;
		}
		break;

	case 500:
		// [1:4����, 0:2����]
		if (st_basic.nMode_PlaceType == 0)
		{
			nRet = OnProc_BufferPlace();	// [Main<-Accy] ���޿� ���Ͽ� Rear 2�� �Ǽ��縮 ���� ���� �� Front 2�� �Ǽ��縮�� ��������
		}
		else
		{
			nRet = OnProc_4BufferPlace();	// [Main<-Accy] ���޿� ���Ͽ� 4�� �Ǽ��縮 �Ѳ����� ��������
		}
		
		if (nRet == CTL_GOOD)
		{
			Func.OnLogBCRData("[Seq_XYZRobot_OnProc_AccyPlace_500_Good]");
			stSync.nReq_XYZRbt2BufferAlaignConv_Work = SYNC_REQ_WORK_COMPLETE_;
			m_nStep_AccyPlace = 1000;
		}
		break;

	case 1000:
		nFuncRet = CTL_GOOD;
		m_nStep_AccyPlace = 0;
		break;
	
	}

	return nFuncRet;
}

//==================================================================//
// �Ǽ��縮 ������ ��� ��Ʈ���� ����
//==================================================================//
int CSeq_XYZRobot::OnProc_GripRetry() 
{
	int nFuncRet = CTL_PROCEED;
	
	int nRet = VAR_INIT_;
	int i=0;
	int nRetData[4] = {0,};
	
	Func.OnTrace_ThreadStep(16, m_nStep_GripRetry);	// ������ ���� ���� TRACE
	switch(m_nStep_GripRetry)
	{
	case 0:
		nRetData[0] = g_ioMgr.get_in_bit(stIO.i_Chk_Rbt_VacOn[SIDE_FRONT_][SIDE_LEFT_]);
		nRetData[1] = g_ioMgr.get_in_bit(stIO.i_Chk_Rbt_VacOn[SIDE_FRONT_][SIDE_RIGHT_]);
		nRetData[2] = g_ioMgr.get_in_bit(stIO.i_Chk_Rbt_VacOn[SIDE_REAR_][SIDE_LEFT_]);
		nRetData[3] = g_ioMgr.get_in_bit(stIO.i_Chk_Rbt_VacOn[SIDE_REAR_][SIDE_RIGHT_]);
		// ����
		if( nRetData[0] == IO_OFF )
		{
			dAccyLiftPos[0] = COMI.Get_MotCurrentPos(m_nMotZ0);
			dAccyLiftPos[0] += 2;
			if (dAccyLiftPos[0] > st_motor[m_nMotZ0].d_pos[Z_ACC_LIFT_LIMIT_])
			{
				dAccyLiftPos[0] = st_motor[m_nMotZ0].d_pos[Z_ACC_LIFT_LIMIT_];
			}
		}
		// �տ�
		if (nRetData[1] == IO_OFF)
		{
			dAccyLiftPos[1] = COMI.Get_MotCurrentPos(m_nMotZ1);
			//dAccyLiftPos[1] += st_motor[m_nMotZ1].d_pos[Z_ACC_LIFT_OFFSET_];
			dAccyLiftPos[1] += 2;
			// ���̸� �߰� [2015.03.25]
			if (dAccyLiftPos[1] > st_motor[m_nMotZ1].d_pos[Z_ACC_LIFT_LIMIT_])
			{
				dAccyLiftPos[1] = st_motor[m_nMotZ1].d_pos[Z_ACC_LIFT_LIMIT_];
			}
		}
		// ����
		if (nRetData[2] == IO_OFF)
		{
			dAccyLiftPos[2] = COMI.Get_MotCurrentPos(m_nMotZ2);
			dAccyLiftPos[2] += 2;
			// ���̸� �߰� [2015.03.25]
			if (dAccyLiftPos[2] > st_motor[m_nMotZ2].d_pos[Z_ACC_LIFT_LIMIT_])
			{
				dAccyLiftPos[2] = st_motor[m_nMotZ2].d_pos[Z_ACC_LIFT_LIMIT_];
			}
		}
		// �ڿ�
		if (nRetData[3] == IO_OFF)
		{
			dAccyLiftPos[3] = COMI.Get_MotCurrentPos(m_nMotZ3);
			dAccyLiftPos[3] += 2;
			// ���̸� �߰� [2015.03.25]
			if (dAccyLiftPos[3] > st_motor[m_nMotZ3].d_pos[Z_ACC_LIFT_LIMIT_])
			{
				dAccyLiftPos[3] = st_motor[m_nMotZ3].d_pos[Z_ACC_LIFT_LIMIT_];
			}
		}
		m_nStep_GripRetry = 1000;
		break;

	case 1000:
		for(i=0; i<4; i++)
		{
			m_nResp_Lift[i] = CTL_NO;
		}

		// dAccyLiftPos : OnProc_GripRetry() �Լ� ȣ������ 0���� ���µ�
		if (dAccyLiftPos[0] != 0)
		{
			nRetData[0] = COMI.Start_SingleMove(m_nMotZ0, dAccyLiftPos[0], (int)st_handler.md_run_speed);
		}
		else
		{
			nRetData[0] = CTLBD_RET_GOOD;
		}

		if (dAccyLiftPos[1] != 0)
		{
			nRetData[1] = COMI.Start_SingleMove(m_nMotZ1, dAccyLiftPos[1], (int)st_handler.md_run_speed);
		}
		else
		{
			nRetData[1] = CTLBD_RET_GOOD;
		}

		if (dAccyLiftPos[2] != 0)
		{
			nRetData[2] = COMI.Start_SingleMove(m_nMotZ2, dAccyLiftPos[2], (int)st_handler.md_run_speed);
		}
		else
		{
			nRetData[2] = CTLBD_RET_GOOD;
		}

		if (dAccyLiftPos[3] != 0)
		{
			nRetData[3] = COMI.Start_SingleMove(m_nMotZ3, dAccyLiftPos[3], (int)st_handler.md_run_speed);
		}
		else
		{
			nRetData[3] = CTLBD_RET_GOOD;
		}

		if (nRetData[0] == CTLBD_RET_GOOD)
		{
			m_nResp_Lift[0] = CTL_YES;
		}
		if (nRetData[1] == CTLBD_RET_GOOD)
		{
			m_nResp_Lift[1] = CTL_YES;
		}
		if (nRetData[2] == CTLBD_RET_GOOD)
		{
			m_nResp_Lift[2] = CTL_YES;
		}
		if (nRetData[3] == CTLBD_RET_GOOD)
		{
			m_nResp_Lift[3] = CTL_YES;
		}

		if (m_nResp_Lift[0] == CTL_YES && m_nResp_Lift[1] == CTL_YES &&	m_nResp_Lift[2] == CTL_YES && m_nResp_Lift[3] == CTL_YES)
		{
			for(i=0; i<4; i++)
			{
				m_nResp_Lift[i] = CTL_NO;
			}
			m_nStep_GripRetry = 1100;
		}
		else if (nRetData[0] == CTLBD_RET_RETRY || nRetData[1] == CTLBD_RET_RETRY || nRetData[2] == CTLBD_RET_RETRY || nRetData[3] == CTLBD_RET_RETRY)
		{
			m_nStep_GripRetry = 1000;
		}
		else if (nRetData[0] != CTLBD_RET_PROCEED && nRetData[0] != CTLBD_RET_GOOD)
		{
			m_nStep_GripRetry = 1000;
			// 001003 0 00 "LIFTER_UP_DOWN_SHIFT1_Z_ACC_LIFT_OFFSET_MOVE_ERR."
			alarm.mstr_code		= "001003";
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
			CTL_Lib.Alarm_Error_Occurrence( 3000, CTL_dWARNING, alarm.mstr_code );
		}
		else if (nRetData[1] != CTLBD_RET_PROCEED && nRetData[1] != CTLBD_RET_GOOD)
		{
			m_nStep_GripRetry = 1000;
			// 011003 0 00 "LIFTER_UP_DOWN_SHIFT2_Z_ACC_LIFT_OFFSET_MOVE_ERR."
			alarm.mstr_code		= "011003";
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
			CTL_Lib.Alarm_Error_Occurrence( 3000, CTL_dWARNING, alarm.mstr_code );
		}
		else if (nRetData[2] != CTLBD_RET_PROCEED && nRetData[2] != CTLBD_RET_GOOD)
		{
			m_nStep_GripRetry = 1000;
			// 021003 0 00 "LIFTER_UP_DOWN_SHIFT3_Z_ACC_LIFT_OFFSET_MOVE_ERR."
			alarm.mstr_code		= "021003";
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
			CTL_Lib.Alarm_Error_Occurrence( 3000, CTL_dWARNING, alarm.mstr_code );
		}
		else if (nRetData[3] != CTLBD_RET_PROCEED && nRetData[3] != CTLBD_RET_GOOD)
		{
			m_nStep_GripRetry = 1000;
			// 031003 0 00 "LIFTER_UP_DOWN_SHIFT4_Z_ACC_LIFT_OFFSET_MOVE_ERR."
			alarm.mstr_code		= "031003";
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
			CTL_Lib.Alarm_Error_Occurrence( 3000, CTL_dWARNING, alarm.mstr_code );
		}
		break;

	case 1100:
		if (dAccyLiftPos[0] != 0)
		{
			nRetData[0] = COMI.Check_SingleMove(m_nMotZ0, dAccyLiftPos[0]);
		}
		else
		{
			nRetData[0] = CTLBD_RET_GOOD;
		}
		
		if (dAccyLiftPos[1] != 0)
		{
			nRetData[1] = COMI.Check_SingleMove(m_nMotZ1, dAccyLiftPos[1]);
		}
		else
		{
			nRetData[1] = CTLBD_RET_GOOD;
		}
		
		if (dAccyLiftPos[2] != 0)
		{
			nRetData[2] = COMI.Check_SingleMove(m_nMotZ2, dAccyLiftPos[2]);
		}
		else
		{
			nRetData[2] = CTLBD_RET_GOOD;
		}
		
		if (dAccyLiftPos[3] != 0)
		{
			nRetData[3] = COMI.Check_SingleMove(m_nMotZ3, dAccyLiftPos[3]);
		}
		else
		{
			nRetData[3] = CTLBD_RET_GOOD;
		}

		if (nRetData[0] == CTLBD_RET_GOOD)
		{
			m_nResp_Lift[0] = CTL_YES;
		}
		if (nRetData[1] == CTLBD_RET_GOOD)
		{
			m_nResp_Lift[1] = CTL_YES;
		}
		if (nRetData[2] == CTLBD_RET_GOOD)
		{
			m_nResp_Lift[2] = CTL_YES;
		}
		if (nRetData[3] == CTLBD_RET_GOOD)
		{
			m_nResp_Lift[3] = CTL_YES;
		}
		
		if (m_nResp_Lift[0] == CTL_YES && m_nResp_Lift[1] == CTL_YES &&	m_nResp_Lift[2] == CTL_YES && m_nResp_Lift[3] == CTL_YES)
		{
			for(i=0; i<4; i++)
			{
				dAccyLiftPos[i] = 0;
				m_nResp_Lift[i] = CTL_NO;
			}
			m_nStep_GripRetry = 10000;
		}
		else if (nRetData[0] == CTLBD_RET_RETRY || nRetData[1] == CTLBD_RET_RETRY || nRetData[2] == CTLBD_RET_RETRY || nRetData[3] == CTLBD_RET_RETRY)
		{
			m_nStep_GripRetry = 1000;
		}
		else if (nRetData[0] != CTLBD_RET_PROCEED && nRetData[0] != CTLBD_RET_GOOD)
		{
			m_nStep_GripRetry = 1000;
			// 001003 0 00 "LIFTER_UP_DOWN_SHIFT1_Z_ACC_LIFT_OFFSET_MOVE_ERR."
			alarm.mstr_code		= "001003";
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
			CTL_Lib.Alarm_Error_Occurrence( 3000, CTL_dWARNING, alarm.mstr_code );
		}
		else if (nRetData[1] != CTLBD_RET_PROCEED && nRetData[1] != CTLBD_RET_GOOD)
		{
			m_nStep_GripRetry = 1000;
			// 011003 0 00 "LIFTER_UP_DOWN_SHIFT2_Z_ACC_LIFT_OFFSET_MOVE_ERR."
			alarm.mstr_code		= "011003";
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
			CTL_Lib.Alarm_Error_Occurrence( 3000, CTL_dWARNING, alarm.mstr_code );
		}
		else if (nRetData[2] != CTLBD_RET_PROCEED && nRetData[2] != CTLBD_RET_GOOD)
		{
			m_nStep_GripRetry = 1000;
			// 021003 0 00 "LIFTER_UP_DOWN_SHIFT3_Z_ACC_LIFT_OFFSET_MOVE_ERR."
			alarm.mstr_code		= "021003";
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
			CTL_Lib.Alarm_Error_Occurrence( 3000, CTL_dWARNING, alarm.mstr_code );
		}
		else if (nRetData[3] != CTLBD_RET_PROCEED && nRetData[3] != CTLBD_RET_GOOD)
		{
			m_nStep_GripRetry = 1000;
			// 031003 0 00 "LIFTER_UP_DOWN_SHIFT4_Z_ACC_LIFT_OFFSET_MOVE_ERR."
			alarm.mstr_code		= "031003";
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
			CTL_Lib.Alarm_Error_Occurrence( 3000, CTL_dWARNING, alarm.mstr_code );
		}
		break;


	case 10000:
		// Vacuum ���� ���� Ȯ��
		nRetData[0] = g_ioMgr.get_in_bit(stIO.i_Chk_Rbt_VacOn[SIDE_FRONT_][SIDE_LEFT_]);
		nRetData[1] = g_ioMgr.get_in_bit(stIO.i_Chk_Rbt_VacOn[SIDE_FRONT_][SIDE_RIGHT_]);
		nRetData[2] = g_ioMgr.get_in_bit(stIO.i_Chk_Rbt_VacOn[SIDE_REAR_][SIDE_LEFT_]);
		nRetData[3] = g_ioMgr.get_in_bit(stIO.i_Chk_Rbt_VacOn[SIDE_REAR_][SIDE_RIGHT_]);
	
		
		if (nRetData[0] == IO_ON && nRetData[1] == IO_ON && nRetData[2] == IO_ON && nRetData[3] == IO_ON)
		{
			m_nStep_GripRetry = 0;
			nFuncRet = CTL_GOOD;
		}
		else
		{
			m_nGripRetryCnt++;			
			if (m_nGripRetryCnt > 20)
			{
				m_nGripRetryCnt = 20;
			}
			m_nStep_GripRetry = 0;
			
			if (m_nGripRetryCnt >= 20)
			{
				nFuncRet = CTL_ERROR;	// ������ �����Ŵ
			}
		}
		break;
	}

	return nFuncRet;
}

//////////////////////////////////////////////////////////////////////////
// Accy ���ڵ� �д� �۾�
//////////////////////////////////////////////////////////////////////////
int CSeq_XYZRobot::OnProc_ReadMove()
{
	int nFuncRet = CTL_PROCEED;
	int nRet = VAR_INIT_;
	int nRetData[4] = {0,};

	Func.OnTrace_ThreadStep(13, m_nStep_ReadMove);
	switch( m_nStep_ReadMove)
	{
	case 0:
		nRet = CTL_Lib.OnSingleMove(m_nMotZ, st_motor[m_nMotZ].d_pos[Z_RBT_SAFETY_], (int)st_handler.md_run_speed);
		if (nRet == CTLBD_RET_GOOD)
		{
			// [Box Lifter -> XYZRbt] LIFT COMPLETE_ ��û ���
			if(stSync.nReq_Lifter2XYZRbt_Work == SYNC_REQ_ACC_LIFT_COMPLETE_)
			{
				nRetData[0] = !g_ioMgr.get_in_bit(stIO.i_Chk_BoxClampAccyDetection[st_map.nLiftWorkSite][0]);
				nRetData[1] = !g_ioMgr.get_in_bit(stIO.i_Chk_BoxClampAccyDetection[st_map.nLiftWorkSite][1]);
				nRetData[2] = !g_ioMgr.get_in_bit(stIO.i_Chk_BoxClampAccyDetection[st_map.nLiftWorkSite][2]);
				nRetData[3] = !g_ioMgr.get_in_bit(stIO.i_Chk_BoxClampAccyDetection[st_map.nLiftWorkSite][3]);
				if (nRetData[0] == IO_OFF || nRetData[1] == IO_OFF || nRetData[2] == IO_OFF || nRetData[3] == IO_OFF)
				{
					stSync.nResp_XYZRbt2Lifter_Work = SYNC_RESP_PASS_;
					nFuncRet = CTL_ERROR;
					m_nStep_ReadMove = 0;
				}
				else 
				{
					// [Box Lifter <- XYZRbt] �۾� ������
					stSync.nResp_XYZRbt2Lifter_Work = SYNC_RESP_WORK_;					
					memset(st_map.nBCR_State, -1, sizeof(st_map.nBCR_State));
					m_nStep_ReadMove = 100;
				}
			}
			else
			{
				m_nStep_ReadMove = 50;
			}
		}
		else if (nRet == CTLBD_RET_ERROR || nRet == CTLBD_RET_RETRY)
		{
			// 071005 0 0 "BARCODE_ROBOT_Z_RBT_SAFETY_MOVE_ERR"
			alarm.mstr_code		= "071005";
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
			CTL_Lib.Alarm_Error_Occurrence( 3000, CTL_dWARNING, alarm.mstr_code);
		}
		break;

	case 50:
		// [Box Lifter -> XYZRbt] LIFT COMPLETE_ ��û ���
		if(stSync.nReq_Lifter2XYZRbt_Work == SYNC_REQ_ACC_LIFT_COMPLETE_)
		{
			nRetData[0] = !g_ioMgr.get_in_bit(stIO.i_Chk_BoxClampAccyDetection[st_map.nLiftWorkSite][0]);
			nRetData[1] = !g_ioMgr.get_in_bit(stIO.i_Chk_BoxClampAccyDetection[st_map.nLiftWorkSite][1]);
			nRetData[2] = !g_ioMgr.get_in_bit(stIO.i_Chk_BoxClampAccyDetection[st_map.nLiftWorkSite][2]);
			nRetData[3] = !g_ioMgr.get_in_bit(stIO.i_Chk_BoxClampAccyDetection[st_map.nLiftWorkSite][3]);
			if (nRetData[0] == IO_OFF || nRetData[1] == IO_OFF || nRetData[2] == IO_OFF || nRetData[3] == IO_OFF)
			{
				stSync.nResp_XYZRbt2Lifter_Work = SYNC_RESP_PASS_;
				nFuncRet = CTL_ERROR;
				m_nStep_ReadMove = 0;
			}
			else
			{
				// [Box Lifter <- XYZRbt] �۾� ������
				stSync.nResp_XYZRbt2Lifter_Work = SYNC_RESP_WORK_;				
				memset(st_map.nBCR_State, -1, sizeof(st_map.nBCR_State));
				m_nStep_ReadMove = 100;
			}
		}
		break;

	case 100:
		// Y_RBT_ACC_BCR_ ��ġ�� d_Y_Gripper_Size�� (st_map.nLiftWorkSite * 2) + Site��ŭ�� ���� ��ġ�� �̵���.
		// st_map.nLiftWorkSite(SIDE_FRONT = 0, SIDE_REAR = 1)
		m_dTargetPos = st_motor[m_nMotY].d_pos[Y_RBT_ACC_BCR_] - (st_accy.d_Y_Gripper_Size * ( (st_map.nLiftWorkSite * 2) + SIDE_FRONT_) );
		nRet = CTL_Lib.OnSingleMove(m_nMotY, m_dTargetPos, (int)st_handler.md_run_speed);
		
		if(nRet == CTLBD_RET_GOOD)
		{
			cLOG.OnLogEvent(LOG_COMM_, "---------------READ_START---------------");
			m_nStep_ReadMove = 200;
		}
		else if (nRet == CTLBD_RET_ERROR || nRet == CTLBD_RET_RETRY)
		{
			// 051001 0 00 "BARCODE_ROBOT_Y_RBT_ACC_BCR_1_MOVE_ERR."
			// 051002 0 00 "BARCODE_ROBOT_Y_RBT_ACC_BCR_2_MOVE_ERR."
			// 051003 0 00 "BARCODE_ROBOT_Y_RBT_ACC_BCR_3_MOVE_ERR."
			// 051004 0 00 "BARCODE_ROBOT_Y_RBT_ACC_BCR_4_MOVE_ERR."
			alarm.mstr_code.Format("05100%d", 1 + ( (st_map.nLiftWorkSite * 2) + SIDE_FRONT_) );
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
			CTL_Lib.Alarm_Error_Occurrence( 3000, CTL_dWARNING, alarm.mstr_code);
		}	
		break;

	case 200:
		// Lift 1������ ���޵� Accy ���ڵ� �б� �۾�
		nRet = OnBCRRead(SIDE_LEFT_);
		if (nRet == CTL_GOOD)
		{
			// ���簡 ���� ��쿡�� Rand Ȯ���� �̿��Ͽ� ���� �ϵ��� ����
			if (st_basic.n_mode_device == WITHOUT_DVC_)
			{
				srand((unsigned)time(NULL));
				int nRand = rand() % 10;
				
				if(nRand < 8)
				{
					st_map.nBCR_State[0] = BCR_READ_GOOD_;
				}
				else
				{
					st_map.nBCR_State[0] = BCR_READ_BAD_;
				}
			}
			else
			{
				if (st_work.sRbtBCR_Data[0] != "")
				{
					if (st_work.sRbtBCR_Data[0] == st_work.m_sRcvAccyMathCode)
					{
						st_map.nBCR_State[0] = BCR_READ_GOOD_;
					}
					else
					{
						st_map.nBCR_State[0] = BCR_READ_BAD_;
					}
				}
				else
				{
					st_map.nBCR_State[0] = BCR_READ_BAD_;
				}
			}
			if (st_handler.cwnd_main != NULL)
			{
				// ���ڵ� ���� ǥ�� �ʱ�ȭ ��û
				st_handler.cwnd_main->PostMessage(WM_UPDATE_MAIN, READ_BCR_DATA);
			}
			m_nStep_ReadMove = 300;
			
		}
		break;
		
	case 300:
		// Lift 2������ ���޵� Accy ���ڵ� �б� �۾�
		nRet = OnBCRRead(SIDE_RIGHT_);
		if (nRet == CTL_GOOD)
		{
			// ���簡 ���� ��쿡�� Rand Ȯ���� �̿��Ͽ� ���� �ϵ��� ����
			if (st_basic.n_mode_device == WITHOUT_DVC_)
			{
				srand((unsigned)time(NULL));
				int nRand = rand() % 10;
				
				if(nRand < 8)
				{
					st_map.nBCR_State[1] = BCR_READ_GOOD_;
				}
				else
				{
					st_map.nBCR_State[1] = BCR_READ_BAD_;
				}
			}
			else
			{
				if (st_work.sRbtBCR_Data[1] != "")
				{
					if (st_work.sRbtBCR_Data[1] == st_work.m_sRcvAccyMathCode)
					{
						st_map.nBCR_State[1] = BCR_READ_GOOD_;
					}
					else
					{
						st_map.nBCR_State[1] = BCR_READ_BAD_;
					}
				}
				else
				{
					st_map.nBCR_State[1] = BCR_READ_BAD_;
				}
			}
			if (st_handler.cwnd_main != NULL)
			{
				// ���ڵ� ���� ǥ�� �ʱ�ȭ ��û
				st_handler.cwnd_main->PostMessage(WM_UPDATE_MAIN, READ_BCR_DATA);
			}
			m_nStep_ReadMove = 600;
		}
		break;

	case 600:
		// Y_RBT_ACC_BCR_ ��ġ�� d_Y_Gripper_Size�� (st_map.nLiftWorkSite * 2) + Site��ŭ�� ���� ��ġ�� �̵���.
		// st_map.nLiftWorkSite(SIDE_FRONT = 0, SIDE_REAR = 1)
		m_dTargetPos = st_motor[m_nMotY].d_pos[Y_RBT_ACC_BCR_] - (st_accy.d_Y_Gripper_Size * ( (st_map.nLiftWorkSite * 2) + SIDE_REAR_) );
		nRet = CTL_Lib.OnSingleMove(m_nMotY, m_dTargetPos, (int)st_handler.md_run_speed);
		
		if(nRet == CTLBD_RET_GOOD)
		{
			m_nStep_ReadMove = 800;
		}
		else if (nRet == CTLBD_RET_ERROR || nRet == CTLBD_RET_RETRY)
		{
			// 051001 0 00 "BARCODE_ROBOT_Y_RBT_ACC_BCR_1_MOVE_ERR."
			// 051002 0 00 "BARCODE_ROBOT_Y_RBT_ACC_BCR_2_MOVE_ERR."
			// 051003 0 00 "BARCODE_ROBOT_Y_RBT_ACC_BCR_3_MOVE_ERR."
			// 051004 0 00 "BARCODE_ROBOT_Y_RBT_ACC_BCR_4_MOVE_ERR."
			alarm.mstr_code.Format("05100%d", 1 + ( (st_map.nLiftWorkSite * 2) + SIDE_REAR_) );
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
			CTL_Lib.Alarm_Error_Occurrence( 3000, CTL_dWARNING, alarm.mstr_code);
		}
		break;

	case 800:
		// Lift 4������ ���޵� Accy ���ڵ� �б� �۾�
		nRet = OnBCRRead(SIDE_RIGHT_);
		if (nRet == CTL_GOOD)
		{
			// ���簡 ���� ��쿡�� Rand Ȯ���� �̿��Ͽ� ���� �ϵ��� ����
			if (st_basic.n_mode_device == WITHOUT_DVC_)
			{
				srand((unsigned)time(NULL));
				int nRand = rand() % 10;
				
				if(nRand < 8)
				{
					st_map.nBCR_State[3] = BCR_READ_GOOD_;
				}
				else
				{
					st_map.nBCR_State[3] = BCR_READ_BAD_;
				}
			}
			else
			{
				if (st_work.sRbtBCR_Data[3] != "")
				{
					if (st_work.sRbtBCR_Data[3] == st_work.m_sRcvAccyMathCode)
					{
						st_map.nBCR_State[3] = BCR_READ_GOOD_;
					}
					else
					{
						st_map.nBCR_State[3] = BCR_READ_BAD_;
					}
				}
				else
				{
					st_map.nBCR_State[3] = BCR_READ_BAD_;
				}
			}
			if (st_handler.cwnd_main != NULL)
			{
				// ���ڵ� ���� ǥ�� �ʱ�ȭ ��û
				st_handler.cwnd_main->PostMessage(WM_UPDATE_MAIN, READ_BCR_DATA);
			}
			m_nStep_ReadMove = 900;
		}
		break;

	case 900:
		// Lift 3������ ���޵� Accy ���ڵ� �б� �۾�
		nRet = OnBCRRead(SIDE_LEFT_);
		if (nRet == CTL_GOOD)
		{
			// ���簡 ���� ��쿡�� Rand Ȯ���� �̿��Ͽ� ���� �ϵ��� ����
			if (st_basic.n_mode_device == WITHOUT_DVC_)
			{
				srand((unsigned)time(NULL));
				int nRand = rand() % 10;
				
				if(nRand < 8)
				{
					st_map.nBCR_State[2] = BCR_READ_GOOD_;
				}
				else
				{
					st_map.nBCR_State[2] = BCR_READ_BAD_;
				}
			}
			else
			{
				if (st_work.sRbtBCR_Data[2] != "")
				{
					if (st_work.sRbtBCR_Data[2] == st_work.m_sRcvAccyMathCode)
					{
						st_map.nBCR_State[2] = BCR_READ_GOOD_;
					}
					else
					{
						st_map.nBCR_State[2] = BCR_READ_BAD_;
					}
				}
				else
				{
					st_map.nBCR_State[2] = BCR_READ_BAD_;
				}
			}
			if (st_handler.cwnd_main != NULL)
			{
				// ���ڵ� ���� ǥ�� �ʱ�ȭ ��û
				st_handler.cwnd_main->PostMessage(WM_UPDATE_MAIN, READ_BCR_DATA);
			}
			cLOG.OnLogEvent(LOG_COMM_, "---------------READ_END---------------");
			m_nStep_ReadMove = 1000;
		}
		break;

	case 1000:
		if (st_map.nBCR_State[0] == BCR_READ_BAD_ && st_map.nBCR_State[1] == BCR_READ_BAD_ && 
			st_map.nBCR_State[2] == BCR_READ_BAD_ && st_map.nBCR_State[3] == BCR_READ_BAD_)
		{
			if (m_nBCRReadAllBadCount >= 1)
			{
				m_nStep_ReadMove = 1100;
			}
			else
			{
				m_nBCRReadAllBadCount++;
				m_nStep_ReadMove = 2000;
			}
		}
		else
		{
			m_nBCRReadAllBadCount = 0;
			m_nStep_ReadMove = 2000;
		}
		break;

	case 1100:
		nRet = CTL_Lib.OnSingleMove(m_nMotY, st_motor[m_nMotY].d_pos[Y_RBT_ACC_NG_BUFFER_], (int)st_handler.md_run_speed);
		if (nRet == CTLBD_RET_GOOD)
		{
			m_nStep_ReadMove = 1200;
		}
		else if (nRet == CTLBD_RET_ERROR || nRet == CTLBD_RET_RETRY)
		{
			// 051008 0 00 "BARCODE_ROBOT_Y_RBT_ACC_NG_BUFFER_MOVE_ERR."
			alarm.mstr_code		= "051008";
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
			CTL_Lib.Alarm_Error_Occurrence( 3000, CTL_dWARNING, alarm.mstr_code );
		}
		break;

	case 1200:
		// Run_Step�� �ʱ�ȭ ���� �ʾ� Ȧ��Ǿ� �߰���.
		m_nStep_Run = 0;
		m_nBCRReadAllBadCount = 0;
		m_nStep_ReadMove = 0;
		st_work.nForceDischargeType = 1;
		st_work.nForceDischarge = FORCEDISCHARGE_ON;
		Func.OnSet_EqpStatusStop();
		break;

	case 2000:
		nFuncRet = CTL_GOOD;
		m_nStep_ReadMove = 0;
		break;

	}
	return nFuncRet;
}


//////////////////////////////////////////////////////////////////////////
// Lift Z���� 2mm�� �ִ� ȹ�� ���� �ø� �� Sensor ������ ������ ���� �ʾ��� ���
// Lift Z���� �����鼭 �۾� ��ġ�� ����
//////////////////////////////////////////////////////////////////////////
int CSeq_XYZRobot::OnProc_WorkSiteChange()
{	
	int nFuncRet = CTL_PROCEED;
	
	int nRet = VAR_INIT_;
	int nRetData[4] = {0,};
	int i = 0, iCheckCount = 0;
	
	Func.OnTrace_ThreadStep(27, m_nStep_WorkSiteChange);

	switch(m_nStep_WorkSiteChange)
	{
	case 0:
		m_nStep_WorkSiteChange = 100;
		break;

	case 100:
		for(i=0; i<MAX_PICKER_; i++)
		{
			m_nPickerUnDn[TYPE_FLAG_][i] = CTL_YES;
			m_nPicker[TYPE_FLAG_][i] = CTL_YES;	// [����] �÷��� ����
		}
		// [0:�¾�, 1:���] 2���� Vacuum ����
		OnSet_PickerUpDn( IO_OFF, m_nPicker[TYPE_FLAG_]);
		OnSet_SolVacuum(IO_OFF, m_nPicker[TYPE_FLAG_]);
		m_nStep_WorkSiteChange = 110;
		break;

	case 110:
		nRetData[0] = OnGet_PickerUpDn( IO_OFF, m_nPickerUnDn[TYPE_FLAG_], m_nPickerUnDn[TYPE_SEN_]);
		nRetData[1] = OnGet_SolVacuum(IO_OFF, m_nPicker[TYPE_FLAG_], m_nPicker[TYPE_SEN_]);
		// ����Ǿ��� ������ �Ϸ�ɶ����� ���
		if ( nRetData[0] == CTL_GOOD && ( nRetData[1] == CTL_GOOD || nRetData[1] == CTL_ERROR ) )
		{
			m_lTime_GoesBy[0] = GetCurrentTime();
			m_nStep_WorkSiteChange = 200;
		}
		else if( nRetData[0] == CTL_ERROR)
		{
			m_nStep_WorkSiteChange = 100;
			CTL_Lib.Alarm_Error_Occurrence( 3000, CTL_dWARNING, alarm.mstr_code);
		}
		break;

	case 200:
		// - m_nPicker : [case 230:] ���� ������
		nRet = Func.OnGet_PickerStatus(0, CTL_NO, m_nPicker);
		if (nRet == CTL_ERROR)
		{
			m_lTime_GoesBy[1] = GetCurrentTime();
			m_lTime_GoesBy[2] = m_lTime_GoesBy[1] - m_lTime_GoesBy[0];
			if (m_lTime_GoesBy[2] < 0)
			{
				m_lTime_GoesBy[0] = GetCurrentTime();
				break;
			}
			
			if (m_lTime_GoesBy[2] > 10)
			{
				// OnGet_PickerStatus() �Լ� �ȿ��� �˶� �ڵ� ������
				alarm.mn_count_mode	= 0;
				alarm.mn_type_mode	= eWARNING;
				st_work.nEqpStatus	= dWARNING;
				CTL_Lib.Alarm_Error_Occurrence( 3000, CTL_dWARNING, alarm.mstr_code);
			}
			break;
		}
		m_nStep_WorkSiteChange = 300;
		break;

	case 300:
		nRet = CTL_Lib.OnSingleMove(m_nMotZ, st_motor[m_nMotZ].d_pos[Z_RBT_SAFETY_], (int)st_handler.md_run_speed);
		if (nRet == CTLBD_RET_GOOD)
		{
			// Z���� �� ������ �� X�� �̵��� ���� ���
			if (stSync.nReq_Lifter2XYZRbt_Work == SYNC_REQ_RESET_)
			{
				m_nStep_WorkSiteChange = 500;
			}
			else
			{
				m_nStep_WorkSiteChange = 400;
			}
		}
		else if (nRet == CTLBD_RET_ERROR || nRet == CTLBD_RET_RETRY)
		{
			// 071005 0 0 "BARCODE_ROBOT_Z_RBT_SAFETY_MOVE_ERR"
			alarm.mstr_code		= "071005";
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
			CTL_Lib.Alarm_Error_Occurrence( 3000, CTL_dWARNING, alarm.mstr_code);
		}
		break;

	case 400:
		// Z���� �� ������ �� X�� �̵��� ���� ���
		if (stSync.nReq_Lifter2XYZRbt_Work == SYNC_REQ_RESET_)
		{
			m_nStep_WorkSiteChange = 500;
		}
		break;
		
	case 500:
		nRet = CTL_Lib.OnSingleMove(m_nMotY, st_motor[m_nMotY].d_pos[Y_RBT_ACC_NG_BUFFER_], (int)st_handler.md_run_speed);
		if (nRet == CTLBD_RET_GOOD)
		{
			m_nStep_WorkSiteChange = 1000;
		}
		else if (nRetData[1] == CTLBD_RET_ERROR || nRetData[1] == CTLBD_RET_RETRY)
		{
			// 051008 0 00 "BARCODE_ROBOT_Y_RBT_ACC_NG_BUFFER_MOVE_ERR."
			alarm.mstr_code		= "051008";
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
			CTL_Lib.Alarm_Error_Occurrence( 3000, CTL_dWARNING, alarm.mstr_code);
		}
		break;

	case 1000:
		m_nStep_WorkSiteChange = 0;
		nFuncRet = CTL_GOOD;
		break;
	}

	return nFuncRet;
}

//==================================================================//
// NG ������ �Ǽ��縮 ����
// - �۾� ���� �� Y�� ���ʹ� NG ���� ��ġ�� ������
//==================================================================//
int CSeq_XYZRobot::OnProc_NGBufferClear() 
{
	int nFuncRet = CTL_PROCEED;
	
	int nRet = VAR_INIT_;
	int i, rest=0, iCheckCount = 0;
	int nRetData[4] = {0,};

	Func.OnTrace_ThreadStep(10, m_nStep_NG_Buff);

	switch(m_nStep_NG_Buff)
	{
	case 0:
		nRet = CTL_Lib.OnSingleMove(m_nMotZ, st_motor[m_nMotZ].d_pos[Z_RBT_SAFETY_], (int)st_handler.md_run_speed);
		if (nRet == CTLBD_RET_GOOD)
		{
			m_nStep_NG_Buff = 100;
		}
		else if (nRet == CTLBD_RET_ERROR || nRet == CTLBD_RET_RETRY)
		{
			// 071005 0 07 "BARCODE_ROBOT_Z_RBT_SAFETY_MOVE_ERR."
			alarm.mstr_code		= "071005";
			CTL_Lib.Alarm_Error_Occurrence( 2000, CTL_dWARNING, alarm.mstr_code);			
		}
		break;

	case 100:
		nRet = COMI.Start_SingleMove(m_nMotY, st_motor[m_nMotY].d_pos[Y_RBT_ACC_NG_BUFFER_], (int)st_handler.md_run_speed);
		if( nRet == CTLBD_RET_GOOD )
		{
			OnSet_PickerPitchOpenClose( IO_ON );
			m_nStep_NG_Buff = 110;
		}
		else if (nRet == CTLBD_RET_ERROR || nRet == CTLBD_RET_RETRY)
		{
			// 051008 0 00 "BARCODE_ROBOT_Y_RBT_ACC_NG_BUFFER_MOVE_ERR."
			alarm.mstr_code		= "051008";
			CTL_Lib.Alarm_Error_Occurrence( 2000, CTL_dWARNING, alarm.mstr_code);						
		}
		break;

	case 110:
		nRet = COMI.Check_SingleMove( m_nMotY, st_motor[m_nMotY].d_pos[Y_RBT_ACC_NG_BUFFER_]);
		if( nRet == CTLBD_RET_GOOD )
		{
			m_nStep_NG_Buff = 200;
		}
		else if (nRet == CTLBD_RET_ERROR || nRet == CTLBD_RET_RETRY)
		{
			// 051008 0 00 "BARCODE_ROBOT_Y_RBT_ACC_NG_BUFFER_MOVE_ERR."
			alarm.mstr_code		= "051008";
			CTL_Lib.Alarm_Error_Occurrence( 2000, CTL_dWARNING, alarm.mstr_code);						
		}
		break;

	case 120:
		OnSet_PickerPitchOpenClose( IO_ON );
		m_nStep_NG_Buff = 200;
		break;

	case 200:
		nRet = OnGet_PickerPitchOPenClose( IO_ON );
		if( nRet == CTL_GOOD )
		{
			m_nStep_NG_Buff = 210;
		}
		else if( nRet == CTL_ERROR )
		{
			m_nStep_NG_Buff = 120;
			CTL_Lib.Alarm_Error_Occurrence( 2000, CTL_dWARNING, alarm.mstr_code);						
		}
		break;

	case 210:
		nRetData[0] = OnCheck_NGBufferAccy(IO_ON,	CHK_NOT_EXIST_,	SIDE_LEFT_);
		nRetData[1] = OnCheck_NGBufferAccy(IO_ON,	CHK_NOT_EXIST_,	SIDE_RIGHT_);
		
		if (st_basic.n_mode_device == WITHOUT_DVC_)
		{
			for(i=0; i<MAX_PICKER_; i++)
			{
				nRetData[i] = st_map.nNGBuffer[i];
			}
		}
		
		for(i=0; i<MAX_PICKER_; i++)
		{
			m_nSensor[i] = CTL_NO;
		}
		m_nSensor[0] = nRetData[0]; m_nSensor[1] = nRetData[1];
		
		m_nGirpSum = (nRetData[0]*1) + (nRetData[1]*2);
		
		// �ð��� ���� �Ŀ� Vacuum ������ �� �����Ƿ� �ٽ� �˻�
		m_lTime_GoesBy[0] = GetCurrentTime();
		m_nStep_NG_Buff = 220;
		break;

		//==========================================================//
		// Vacuum
		//==========================================================//
	case 220:
		m_nRepickCount = 0;  // [����] ���� ���� �õ� Ƚ��
		
		// ��ü ��Ŀ ���� ���
		for(i=0; i<MAX_PICKER_; i++)
		{
			m_nPickerUnDn[TYPE_FLAG_][i] = CTL_NO;
			m_nPicker[TYPE_FLAG_][i] = CTL_NO;	// [����] �÷��� ����
			
			if (m_nSensor[i] == IO_ON)
			{
				m_nPickerUnDn[TYPE_FLAG_][i] = CTL_YES;
				m_nPicker[TYPE_FLAG_][i] = CTL_YES;	// ������ Vacuum���� ����
			}
		}
		m_nStep_NG_Buff = 230;
		break;

	case 230:
		nRet = COMI.Start_SingleMove(m_nMotZ, st_motor[m_nMotZ].d_pos[Z_RBT_ACC_NG_BUFFER_PICK_], (int)st_handler.md_run_speed);
		if (nRet == CTLBD_RET_GOOD)
		{
			// ���� �� ������ ���� ���̰� ���� �ð� ���̱� ���� �̰����� ���۽�Ŵ
			OnSet_PickerUpDn(IO_ON, m_nPickerUnDn[TYPE_FLAG_]);
			OnSet_SolVacuum(IO_ON, m_nPicker[TYPE_FLAG_]);	// �׸��� Vacuum [ON:��������, OFF:��������]
			m_nStep_NG_Buff = 240;
		}
		else if (nRet == CTLBD_RET_ERROR || nRet == CTLBD_RET_SAFETY)
		{
			m_nStep_NG_Buff = 230;
			
			// 071002 0 00 "BARCODE_ROBOT_Z_RBT_ACC_NG_BUFFER_PICK_MOVE_ERR."
			alarm.mstr_code		= "071002";
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
			CTL_Lib.Alarm_Error_Occurrence( 2000, CTL_dWARNING, alarm.mstr_code);						
		}
		break;

	case 240:
		nRet = COMI.Check_SingleMove(m_nMotZ, st_motor[m_nMotZ].d_pos[Z_RBT_ACC_NG_BUFFER_PICK_]);
		if (nRet == CTLBD_RET_GOOD)
		{
			m_nStep_NG_Buff = 250;
		}
		else if (nRet == CTLBD_RET_RETRY)
		{
			m_nStep_NG_Buff = 230;
		}
		else if (nRet == CTLBD_RET_ERROR || nRet == CTLBD_RET_SAFETY)
		{
			m_nStep_NG_Buff = 230;
			
			// 071002 0 00 "BARCODE_ROBOT_Z_RBT_ACC_NG_BUFFER_PICK_MOVE_ERR."
			alarm.mstr_code		= "071002";
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
			CTL_Lib.Alarm_Error_Occurrence( 2000, CTL_dWARNING, alarm.mstr_code);						
		}
		break;

	case 250:
		OnSet_PickerUpDn(IO_ON, m_nPickerUnDn[TYPE_FLAG_]);
		OnSet_SolVacuum(IO_ON, m_nPicker[TYPE_FLAG_]);	// �׸��� Vacuum [ON:��������, OFF:��������]
		m_nStep_NG_Buff = 255;
		break;

	case 255:
		nRetData[0] = OnGet_PickerUpDn( IO_ON, m_nPickerUnDn[TYPE_FLAG_], m_nPickerUnDn[TYPE_SEN_] );
		nRetData[1] = OnGet_SolVacuum(IO_ON, m_nPicker[TYPE_FLAG_], m_nPicker[TYPE_SEN_]);
		if (nRetData[0] == CTL_GOOD && nRetData[1] == CTL_GOOD)
		{
			m_lTime_GoesBy[0] = GetCurrentTime();
			m_nStep_NG_Buff = 260;
		}
		else if( nRetData[0] == CTL_ERROR || nRetData[1] == CTL_ERROR )
		{
			m_nStep_Init = 250;
			CTL_Lib.Alarm_Error_Occurrence( 2001, CTL_dWARNING, alarm.mstr_code);
		}
		break;

	case 260:
		nRet = COMI.Start_SingleMove(m_nMotZ, st_motor[m_nMotZ].d_pos[Z_RBT_SAFETY_], (int)st_handler.md_run_speed);
		if (nRet == CTLBD_RET_GOOD)
		{
			OnSet_PickerUpDn(IO_OFF, m_nPickerUnDn[TYPE_FLAG_]);
			m_nStep_NG_Buff = 270;
		}
		else if (nRet == CTLBD_RET_ERROR || nRet == CTLBD_RET_RETRY)
		{
			// 071005 0 07 "BARCODE_ROBOT_Z_RBT_SAFETY_MOVE_ERR."
			alarm.mstr_code		= "071005";
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
			CTL_Lib.Alarm_Error_Occurrence( 2000, CTL_dWARNING, alarm.mstr_code);						
		}
		break;

	case 270:
		nRet = COMI.Check_SingleMove(m_nMotZ, st_motor[m_nMotZ].d_pos[Z_RBT_SAFETY_]);
		if (nRet == CTLBD_RET_GOOD)
		{
			m_nStep_NG_Buff = 280;
		}
		else if (nRet == CTLBD_RET_ERROR || nRet == CTLBD_RET_RETRY)
		{
			// 071005 0 07 "BARCODE_ROBOT_Z_RBT_SAFETY_MOVE_ERR."
			alarm.mstr_code		= "071005";
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
			CTL_Lib.Alarm_Error_Occurrence( 2000, CTL_dWARNING, alarm.mstr_code);						
			m_nStep_NG_Buff = 260;
		}
		break;

	case 275:
		OnSet_PickerUpDn(IO_OFF, m_nPickerUnDn[TYPE_FLAG_]);
		m_nStep_NG_Buff = 280;
		break;

	case 280:
		nRet = OnGet_PickerUpDn( IO_OFF, m_nPickerUnDn[TYPE_FLAG_], m_nPickerUnDn[TYPE_SEN_] );
		if (nRet == CTL_GOOD)
		{
			m_lTime_GoesBy[0] = GetCurrentTime();
			m_nStep_NG_Buff = 290;
		}
		else if( nRet == CTL_ERROR )
		{
			m_nStep_Init = 275;
			CTL_Lib.Alarm_Error_Occurrence( 2001, CTL_dWARNING, alarm.mstr_code);
		}
		break;

	case 290:
		nRet = Func.OnGet_PickerStatus(0, CTL_YES, m_nPicker);
		if (nRet == CTL_ERROR)
		{
			m_lTime_GoesBy[1] = GetCurrentTime();
			m_lTime_GoesBy[2] = m_lTime_GoesBy[1] - m_lTime_GoesBy[0];
			if (m_lTime_GoesBy[2] < 0)
			{
				m_lTime_GoesBy[0] = GetCurrentTime();
				break;
			}
			
			if (m_lTime_GoesBy[2] < 10)
			{
				break;
			}
		}
		// [���� ����] : ��ٷ� �ؿ� �κ� ����
		// [���� ����] : ���� �ð� ��� �Ŀ� �ؿ� �κ� ����
		
		// ��ü �׸��� ���� ���
		for(i=0; i<MAX_PICKER_; i++)
		{
			// �Ǽ��縮 ���� ���� ������ �׸��ۿ� ���ؼ� �Ǽ��縮�� Vacuum ������ ������ ���
			if (m_nPicker[TYPE_FLAG_][i] == CTL_YES && m_nPicker[TYPE_SEN_][i] == CTL_YES)
			{
				// �׸����� ����ü ���� ���� [���� ������]
				st_map.nXYZRbtPickerInfo[i]	= CTL_YES;
				m_nPicker[TYPE_FLAG_][i]	= CTL_NO;
			}
		}
		
		// OnGet_PickerStatus() �Լ������� [CTL_GOOD/CTL_ERROR] ��ȯ�Ǿ��� ����
		if (nRet == CTL_GOOD)
		{
			m_nStep_NG_Buff = 500;
		}
		else
		{
			m_nRepickCount++;	// ��õ� Ƚ�� ����
			if (m_nRepickCount > 2)
			{
				m_nRepickCount = 2;
			}
			m_nStep_NG_Buff = 230;	// �Ǽ��縮 ���� ���� ��õ�
			
			if (m_nRepickCount >= 2)
			{
				m_nRepickCount = 0;	// [����] ���� ���� �õ� Ƚ��
				
				// OnGet_PickerStatus() �Լ� �ȿ��� �˶� �ڵ� ������
				alarm.mn_count_mode	= 0;
				alarm.mn_type_mode	= eWARNING;
				st_work.nEqpStatus	= dWARNING;
				CTL_Lib.Alarm_Error_Occurrence( 3000, CTL_dWARNING, alarm.mstr_code );
			}
		}
		break;

	case 500:
		nRetData[0] = OnCheck_NGBufferAccy(IO_ON,	CHK_NOT_EXIST_, SIDE_LEFT_);
		nRetData[1] = OnCheck_NGBufferAccy(IO_ON,	CHK_NOT_EXIST_, SIDE_RIGHT_);

		for( i = 0; i < 2/*MAX_PICKER_*/; i++ )
		{
			if( st_map.nXYZRbtPickerInfo[i] == CTL_YES )
			{
				if( nRetData[i] != IO_OFF)
				{
					// ������ �Ǽ��縮 ������
					// Vacuum ������ ������ �־� ����� �� �����µ� ������ ���
					iCheckCount++;
				}
			}
		}

		if( iCheckCount > 0 )
		{//143000 0 00 "PS1406_PS1407_NG_BUFFER_ACCY_EXSIT_CHK_ERR."
			m_nStep_NG_Buff = 0;
			alarm.mstr_code	= "143000";
			CTL_Lib.Alarm_Error_Occurrence( 2001, CTL_dWARNING, alarm.mstr_code);
			break;
		}

		for( i = 0; i < 2/*MAX_PICKER_*/; i++ )
		{
			if( st_map.nXYZRbtPickerInfo[i] == CTL_YES )
			{
				st_map.nBCR_State[i] = BCR_READ_BAD_;
			}
			else
			{
				st_map.nBCR_State[i] = BCR_READ_GOOD_;
			}
		}
		m_nStep_NG_Buff = 1000;
		break;

	case 1000:
		// Gripper�� ��� �ִ� Accy ���� (SIDE_FRONT_ : 1, 2�� Gripper, SIDE_REAR_ : 3, 4�� Gripper)
		nRet = OnProc_AccyOut( st_map.nBCR_State, SIDE_FRONT_ );
		if( nRet == CTL_GOOD )
		{
			m_nStep_NG_Buff = 1100;
		}
		break;

	case 1100:
		nRet = CTL_Lib.OnSingleMove(m_nMotY, st_motor[m_nMotY].d_pos[Y_RBT_ACC_NG_BUFFER_], (int)st_handler.md_run_speed);
		if (nRet == CTLBD_RET_GOOD)
		{
			m_nStep_NG_Buff = 2000;
		}
		else if (nRet == CTLBD_RET_ERROR || nRet == CTLBD_RET_RETRY)
		{
			// 051008 0 00 "BARCODE_ROBOT_Y_RBT_ACC_NG_BUFFER_MOVE_ERR."
			alarm.mstr_code		= "051008";
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
		}
		break;

	case 2000:
		memset(st_map.nNGBuffer, ACCY_NO_, sizeof(st_map.nNGBuffer));
		m_nStep_NG_Buff = 0;
		nFuncRet = CTL_GOOD;
		break;

	}

	return nFuncRet;
}

//////////////////////////////////////////////////////////////////////////
// Accy ���� �۾�
// int nzNGAccyInfo[4] : Gripper ����
// Gripper�� ��� �ִ� Accy ���� (0 : 1, 2�� Gripper, 1 : 3, 4�� Gripper)
//////////////////////////////////////////////////////////////////////////
int CSeq_XYZRobot::OnProc_AccyOut(int nzNGAccyInfo[4], int nzSite)
{
	int nFuncRet = CTL_PROCEED;
	int nRet = VAR_INIT_;
	int i=0;
	int nRetData[4] = {0,};

	Func.OnTrace_ThreadStep(31, m_nStep_NGAccyOut);

	switch( m_nStep_NGAccyOut )
	{
	case 0:
		if( nzSite == SIDE_FRONT_ )
		{
			if( nzNGAccyInfo[0] == 1 && nzNGAccyInfo[1] == 1 )
			{
				nFuncRet = CTL_GOOD;
				break;
			}
		}
		else if( nzSite == SIDE_REAR_)
		{
			if( nzNGAccyInfo[2] == 1 && nzNGAccyInfo[3] == 1 )
			{
				nFuncRet = CTL_GOOD;
				break;
			}
		}
		m_nStep_NGAccyOut = 50;
		break;

	case 50:
		nRet = CTL_Lib.OnSingleMove(m_nMotZ, st_motor[m_nMotZ].d_pos[Z_RBT_SAFETY_], (int)st_handler.md_run_speed);
		if (nRet == CTLBD_RET_GOOD)
		{
			m_nStep_NGAccyOut = 100;
		}
		else if (nRet == CTLBD_RET_ERROR || nRet == CTLBD_RET_RETRY)
		{
			// 071005 0 0 "BARCODE_ROBOT_Z_RBT_SAFETY_MOVE_ERR"
			alarm.mstr_code		= "071005";
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
			CTL_Lib.Alarm_Error_Occurrence( 2001, CTL_dWARNING, alarm.mstr_code);
		}
		break;

	case 100:
		// nzSite�� ���� d_Y_Gripper_Size ���� ��ġ�� �̵���. (SIDE_FRONT = 0, SIDE_REAR = 1)
		m_dTargetPos = st_motor[m_nMotY].d_pos[Y_RBT_ACC_NG_OUT_] + (st_accy.d_Y_Gripper_Size * nzSite);
		nRet = CTL_Lib.OnSingleMove(m_nMotY, m_dTargetPos, (int)st_handler.md_run_speed);
		if (nRet == CTLBD_RET_GOOD)
		{
			m_nStep_NGAccyOut = 200;
			st_map.nNGOut = 1;
			if (st_handler.cwnd_main != NULL)
			{
				// NG Out ���� ǥ�� ��û
				st_handler.cwnd_main->PostMessage(WM_UPDATE_MAIN, PLACE_NG_OUT_);
			}
		}
		else if (nRet == CTLBD_RET_ERROR || nRet == CTLBD_RET_RETRY)
		{
			// 051007 0 00 "BARCODE_ROBOT_Y_RBT_ACC_NG_OUT_MOVE_ERR."
			alarm.mstr_code		= "051007";
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
			CTL_Lib.Alarm_Error_Occurrence( 2001, CTL_dWARNING, alarm.mstr_code);
		}
		break;

	case 200:
		for( i = 0; i < MAX_PICKER_; i++ )
		{
			m_nPicker[TYPE_FLAG_][i] = CTL_NO;
			m_nPickerUnDn[TYPE_FLAG_][i] = CTL_NO;
		}
		m_nStep_NGAccyOut = 210;
		break;

	case 210:
		if( nzSite == SIDE_FRONT_ )
		{
			if( nzNGAccyInfo[0] == BCR_READ_BAD_ )
			{
				m_nPicker[TYPE_FLAG_][0] = CTL_YES;
				m_nPickerUnDn[TYPE_FLAG_][0] = CTL_YES;
			}
		}
		else if( nzSite == SIDE_REAR_ )
		{
			if( nzNGAccyInfo[2] == BCR_READ_BAD_ )
			{
				m_nPicker[TYPE_FLAG_][2] = CTL_YES;
				m_nPickerUnDn[TYPE_FLAG_][2] = CTL_YES;
			}
		}
		m_nStep_NGAccyOut = 600;
		break;

	case 600:
		OnSet_PickerUpDn( IO_ON, m_nPickerUnDn[TYPE_FLAG_] );
		OnSet_SolVacuum( IO_OFF, m_nPicker[TYPE_FLAG_]);
		OnSet_SolVacuum( IO_ON, m_nPicker[TYPE_FLAG_]);
		m_nStep_NGAccyOut = 610;
		break;

	case 610:
		nRetData[0] = OnGet_PickerUpDn( IO_ON, m_nPickerUnDn[TYPE_FLAG_], m_nPickerUnDn[TYPE_SEN_]);
		nRetData[1] = OnGet_SolVacuum( IO_OFF, m_nPicker[TYPE_FLAG_], m_nPicker[TYPE_SEN_] );
		nRetData[2] = OnGet_EjectOnOff( IO_ON, m_nPicker[TYPE_FLAG_]);
		if( nRetData[0] == CTL_GOOD && nRetData[1] == CTL_GOOD && nRetData[2] == CTL_GOOD )
		{
			if( nzSite == SIDE_FRONT_)
			{
				if( m_nPickerUnDn[TYPE_FLAG_][0] == CTL_YES && m_nPickerUnDn[TYPE_SEN_][0] == CTL_NO &&
				    m_nPicker[TYPE_FLAG_][0] == CTL_YES && m_nPicker[TYPE_SEN_][0] == CTL_NO )
				{
					st_map.nXYZRbtPickerInfo[0] = CTL_NO;
					m_nPickerUnDn[TYPE_FLAG_][0] = CTL_NO;
					m_nPicker[TYPE_FLAG_][0] = CTL_NO;
				}
			}
			else if( nzSite == SIDE_REAR_ )
			{
				if( m_nPickerUnDn[TYPE_FLAG_][2] == CTL_YES && m_nPickerUnDn[TYPE_FLAG_][2] == CTL_NO &&
					m_nPicker[TYPE_FLAG_][2] == CTL_YES && m_nPicker[TYPE_FLAG_][2] == CTL_NO )
				{
					st_map.nXYZRbtPickerInfo[2] = CTL_NO;
					m_nPickerUnDn[TYPE_FLAG_][2] = CTL_NO;
					m_nPicker[TYPE_FLAG_][2] = CTL_NO;
					OnSet_SolVacuum( IO_ON, m_nPicker[TYPE_FLAG_]);
				}
			}
			st_map.nNGOutCNT += nzSite == SIDE_FRONT_ ? !nzNGAccyInfo[0] : !nzNGAccyInfo[2];
			m_nStep_NGAccyOut = 620;

		}
		else if( nRetData[0] == CTL_ERROR || nRetData[1] == CTL_ERROR )
		{
			m_nStep_NGAccyOut = 600;
		}
		break;

	case 620:
		OnGet_EjectOnOff(IO_OFF, m_nPicker[TYPE_FLAG_]);
		m_nStep_NGAccyOut = 700;
		break;

	case 700:
		for ( i = 0; i < MAX_PICKER_; i++)
		{
			m_nPickerUnDn[TYPE_FLAG_][i] = CTL_NO;
			m_nPicker[TYPE_FLAG_][i] = CTL_NO;
		}
		m_nStep_NGAccyOut = 710;
		break;

	case 710:
		if (nzSite == SIDE_FRONT_)
		{
			if (nzNGAccyInfo[1] == BCR_READ_BAD_)
			{
				m_nPickerUnDn[TYPE_FLAG_][1] = CTL_YES;
				m_nPicker[TYPE_FLAG_][1] = CTL_YES;	// ������ Vacuum���� ����
			}
		}
		else if (nzSite == SIDE_REAR_)
		{
			if (nzNGAccyInfo[3] == BCR_READ_BAD_)
			{
				m_nPickerUnDn[TYPE_FLAG_][3] = CTL_YES;
				m_nPicker[TYPE_FLAG_][3] = CTL_YES;	// ������ Vacuum���� ����
			}
		}
		m_nStep_NGAccyOut = 800;
		break;
		
	case 800:
		OnSet_PickerUpDn( IO_OFF, m_nPickerUnDn[TYPE_FLAG_]);
		OnSet_SolVacuum(IO_OFF, m_nPicker[TYPE_FLAG_]);	//Vacuum [ON:��������, OFF:��������]
		OnGet_EjectOnOff( IO_ON, m_nPicker[TYPE_FLAG_]);
		m_nStep_NGAccyOut = 810;
		break;
		
	case 810:
		nRetData[0] = OnGet_PickerUpDn( IO_OFF, m_nPickerUnDn[TYPE_FLAG_], m_nPickerUnDn[TYPE_SEN_]);
		nRetData[1] = OnGet_SolVacuum( IO_OFF, m_nPicker[TYPE_FLAG_], m_nPicker[TYPE_FLAG_]);
		nRetData[2] = OnGet_EjectOnOff( IO_ON, m_nPicker[TYPE_FLAG_]);
		if( nRetData[0] == CTL_GOOD && nRetData[1] == CTL_GOOD && nRetData[2] == CTL_GOOD)
		{
			if (nzSite == SIDE_FRONT_)
			{
				// �Ǽ��縮 ������ ���� ������ �Ǽ��縮�� Vacuum ������ ������ ���
				if( m_nPickerUnDn[TYPE_FLAG_][1] == CTL_YES && m_nPickerUnDn[TYPE_SEN_][1] == CTL_NO &&
					m_nPicker[TYPE_FLAG_][1] == CTL_YES && m_nPicker[TYPE_SEN_][1] == CTL_NO)
				{
					// �׸����� ����ü ���� ���� [���� ������]
					st_map.nXYZRbtPickerInfo[1]	= CTL_NO;
					m_nPicker[TYPE_FLAG_][1]	= CTL_NO;
				}
			}
			else if (nzSite == SIDE_REAR_)
			{
				// �Ǽ��縮 ������ ���� ������ �Ǽ��縮�� Vacuum ������ ������ ���
				if( m_nPickerUnDn[TYPE_FLAG_][3] == CTL_YES && m_nPickerUnDn[TYPE_SEN_][3] == CTL_NO &&
					m_nPicker[TYPE_FLAG_][3] == CTL_YES && m_nPicker[TYPE_SEN_][3] == CTL_NO)
				{
					// �׸����� ����ü ���� ���� [���� ������]
					st_map.nXYZRbtPickerInfo[3]	= CTL_NO;
					m_nPicker[TYPE_FLAG_][3]	= CTL_NO;
				}
			}
			st_map.nNGOutCNT += nzSite == SIDE_FRONT_ ? !nzNGAccyInfo[1] : !nzNGAccyInfo[3];
			m_nStep_NGAccyOut = 1000;
		}
		else if( nRetData[0] == CTL_ERROR || nRetData[1] == CTL_ERROR )
		{
			m_nStep_NGAccyOut = 800;
			CTL_Lib.Alarm_Error_Occurrence( 2001, CTL_dWARNING, alarm.mstr_code);
		}
		break;

	case 1000:
		nRet = COMI.Start_SingleMove(m_nMotZ, st_motor[m_nMotZ].d_pos[Z_RBT_SAFETY_], (int)st_handler.md_run_speed);
		if (nRet == CTLBD_RET_GOOD)
		{
			m_nStep_NGAccyOut = 1100;
		}
		else if (nRet == CTLBD_RET_ERROR || nRet == CTLBD_RET_RETRY)
		{
			// 071005 0 07 "BARCODE_ROBOT_Z_RBT_SAFETY_MOVE_ERR."
			alarm.mstr_code		= "071005";
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
			CTL_Lib.Alarm_Error_Occurrence( 2001, CTL_dWARNING, alarm.mstr_code);
		}
		break;

	case 1100:
		nRet = COMI.Check_SingleMove(m_nMotZ, st_motor[m_nMotZ].d_pos[Z_RBT_SAFETY_]);
		if( nRet == CTLBD_RET_GOOD )
		{
			m_nStep_NGAccyOut = 1200;
		}
		else if ( nRet == CTLBD_RET_ERROR || nRet == CTLBD_RET_RETRY)
		{
			// 071005 0 07 "BARCODE_ROBOT_Z_RBT_SAFETY_MOVE_ERR."
			alarm.mstr_code		= "071005";
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
			CTL_Lib.Alarm_Error_Occurrence( 2001, CTL_dWARNING, alarm.mstr_code);
		}
		break;

	case 1200:
		OnGet_EjectOnOff(IO_OFF, m_nPicker[TYPE_FLAG_]);
		m_nStep_NGAccyOut = 2000;
		break;


// 	case 1110:
// 		for ( i = 0; i < MAX_PICKER_; i++)
// 		{
// 			m_nPickerUnDn[TYPE_FLAG_][i] = CTL_YES;
// 			m_nPicker[TYPE_FLAG_][0] = CTL_NO;
// 		}
// 		OnSet_PickerUpDn( IO_OFF, m_nPickerUnDn[TYPE_FLAG_]);
// 		OnSet_SolVacuum( IO_OFF)
// 		m_nStep_NGAccyOut = 1200;
// 		break;
// 
// 	case 1200:
// 		nRetData[0] = OnGet_PickerUpDn( IO_OFF, m_nPickerUnDn[TYPE_FLAG_], m_nPickerUnDn[TYPE_SEN_]);
// 		nRetData[1] = OnGet_SolVacuum( IO_OFF, m_nPicker[TYPE_FLAG_], m_nPicker[TYPE_FLAG_]);
// 		if( nRetData[0] == CTL_GOOD && nRetData[1] == CTL_GOOD )
// 		{
// 			if (nzSite == SIDE_FRONT_)
// 			{
// 				// �Ǽ��縮 ������ ���� ������ �Ǽ��縮�� Vacuum ������ ������ ���
// 				if( m_nPickerUnDn[TYPE_FLAG_][1] == CTL_YES && m_nPickerUnDn[TYPE_SEN_][1] == CTL_NO &&
// 					m_nPicker[TYPE_FLAG_][1] == CTL_YES && m_nPicker[TYPE_SEN_][1] == CTL_NO)
// 				{
// 					// �׸����� ����ü ���� ���� [���� ������]
// 					st_map.nXYZRbtPickerInfo[1]	= CTL_NO;
// 					m_nPicker[TYPE_FLAG_][1]	= CTL_NO;
// 				}
// 			}
// 			else if (nzSite == SIDE_REAR_)
// 			{
// 				// �Ǽ��縮 ������ ���� ������ �Ǽ��縮�� Vacuum ������ ������ ���
// 				if( m_nPickerUnDn[TYPE_FLAG_][3] == CTL_YES && m_nPickerUnDn[TYPE_SEN_][3] == CTL_NO &&
// 					m_nPicker[TYPE_FLAG_][3] == CTL_YES && m_nPicker[TYPE_SEN_][3] == CTL_NO)
// 				{
// 					// �׸����� ����ü ���� ���� [���� ������]
// 					st_map.nXYZRbtPickerInfo[3]	= CTL_NO;
// 					m_nPicker[TYPE_FLAG_][3]	= CTL_NO;
// 				}
// 			}
// 			st_map.nNGOutCNT += nzSite == SIDE_FRONT_ ? !nzNGAccyInfo[1] : !nzNGAccyInfo[3];
// 			m_nStep_NGAccyOut = 1000;
// 		}
// 		else if( nRetData[0] == CTL_ERROR || nRetData[1] == CTL_ERROR )
// 		{
// 			m_nStep_NGAccyOut = 800;
// 			CTL_Lib.Alarm_Error_Occurrence( 2001, CTL_dWARNING, alarm.mstr_code);
// 		}
// 
// 		if( nRet == CTL_GOOD )
// 		{
// 			m_nStep_NGAccyOut = 2000;
// 		}
// 		else if( nRet == CTL_ERROR )
// 		{
// 			m_nStep_NGAccyOut = 800;
// 			CTL_Lib.Alarm_Error_Occurrence( 2001, CTL_dWARNING, alarm.mstr_code);
// 		}
// 		break;

	case 2000:
		st_map.nNGOut = 0;
		if (st_handler.cwnd_main != NULL)
		{
			// NG Out ���� ���� ǥ�� ��û
			st_handler.cwnd_main->PostMessage(WM_UPDATE_MAIN, PLACE_NG_OUT_);
		}
		nFuncRet = CTL_GOOD;
		m_nStep_NGAccyOut = 0;
		break;

	}
	

	return nFuncRet;
}


//////////////////////////////////////////////////////////////////////////
// BCR Read �Լ�
// nzMode = 0 �̸� �������� �̵��Ͽ� BCR Read
// nzMode = 1 �̸� ���������� �̵��Ͽ� BCR Read
//////////////////////////////////////////////////////////////////////////
int CSeq_XYZRobot::OnBCRRead(int nzMode)
{
	int nFuncRet = -1;

	int nRet = VAR_INIT_;
	int nRetData[4] = {0,};

	Func.OnTrace_ThreadStep(14, m_nStep_BCRRead);	// ������ ���� ���� TRACE
	switch(m_nStep_BCRRead)
	{
		// KILEE [2015.03.23]
	case 0:
		nRet = CTL_Lib.OnSingleMove(m_nMotZ, st_motor[m_nMotZ].d_pos[Z_RBT_SAFETY_], (int)st_handler.md_run_speed);
		if (nRet == CTLBD_RET_GOOD)
		{
			m_nStep_BCRRead = 50;
		}
		else if (nRet == CTLBD_RET_ERROR || nRet == CTLBD_RET_RETRY)
		{
			// 071005 0 0 "BARCODE_ROBOT_Z_RBT_SAFETY_MOVE_ERR"
			alarm.mstr_code		= "071005";
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
		}
		break;

	case 50:
		// X_RBT_ACC_BCR_ ��ġ�� d_X_Gripper_Size�� nzMode��ŭ�� ���� ��ġ�� �̵���.
		// nzMode(SIDE_LEFT = 0, SIDE_RIGHT = 1)
		m_dTargetPos = st_motor[m_nMotX].d_pos[X_RBT_ACC_BCR_] + (st_accy.d_X_Gripper_Size * nzMode);
		nRet = CTL_Lib.OnSingleMove(m_nMotX, m_dTargetPos, (int)st_handler.md_run_speed);
		if (nRet == CTLBD_RET_GOOD)
		{
			// ���簡 ���� ��� �̹Ƿ� BCR�� Read�� ���� ���� �ʰ� �Ϸ� Step���� �̵�
			if (st_basic.n_mode_device == WITHOUT_DVC_)
			{
				m_nStep_BCRRead = 5000;
				break;
			}
			m_nStep_BCRRead = 100;
		}
		else if (nRet == CTLBD_RET_ERROR || nRet == CTLBD_RET_RETRY)
		{
			// 061000 0 00 "BARCODE_ROBOT_X_RBT_ACC_BCR_POS_MOVE_ERR."
			alarm.mstr_code = "061000";
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
			CTL_Lib.Alarm_Error_Occurrence( 3000, CTL_dWARNING, alarm.mstr_code);
		}
		break;

	case 100:
		m_nResp_Move = CTL_NO;
		// BCR �б� ���� �� [��/��] �̵��Ͽ� BCR �б� �۾� ������
		// - �� �� ����� ���� ����
		m_nNearby = -1;
		m_nRetryCount = 0;	// [����] ��� �õ� Ƚ�� ����

		m_nStep_BCRRead = 110;
		break;

	case 110:
		if (m_nResp_Move == CTL_NO)
		{
			// ���ڵ� �б� ��û
     		evBCR.OnReq_ACCY();
		}
		m_nStep_BCRRead = 120;
		break;
	
	case 120:
		if (m_nResp_Move == CTL_NO)
		{
			nRet = evBCR.OnGet_ACCY();
		}
		else
		{
			nRet = REPLY_CORRECT_;
		}

		if (nRet == REPLY_CORRECT_)
		{
			m_nResp_Move = CTL_YES;
		}

		if (m_nResp_Move == CTL_YES)
		{
			m_nResp_Move = CTL_NO;
			// �Ϸ� Step �̵�
			m_nStep_BCRRead = 5000;
		}
		else if (nRet == REPLY_ERROR_)
		{
			// BCR �б� �۾� �õ� Ƚ�� �˻�
			if (m_nRetryCount >= 2)//st_basic.nInspectRetry)
			{
				m_nStep_BCRRead = 200;
			}
			else
			{
				m_nRetryCount++;
				m_nStep_BCRRead = 110;
			}
		}
		break;

	case 200:
		// [ƼĪ ��ġ]->[����]->[����] ������ BCR �б� ��õ�
		if (m_nNearby == -1)
		{
			m_nRetryCount = 0;	// [����] ��� �õ�
			m_nNearby = SIDE_LEFT_;
			m_dMaxOffset = (double)0;	// [����] ���ڵ� �б� ���� �� ������ �ɼ�

			m_nStep_BCRRead = 300;
		}
		else if (m_nNearby == SIDE_LEFT_)
		{
			if (m_dMaxOffset >= MAX_OFFSET_)
			{
				m_nRetryCount = 0;	// [����] ��� �õ�
				// ���� ��ġ �õ� �Ϸ� [���� ��ġ �õ���]
				m_nNearby = SIDE_RIGHT_;
				m_dMaxOffset = (double)0;	// [����] ���ڵ� �б� ���� �� ������ �ɼ�
			}
			else
			{
				m_nRetryCount = 0;	// [����] ��� �õ�
			}
			m_nStep_BCRRead = 300;
		}
		else if (m_nNearby == SIDE_RIGHT_)
		{
			if (m_dMaxOffset >= MAX_OFFSET_)
			{
				m_nStep_BCRRead = 5000;
			}
			else
			{
				m_nRetryCount = 0;	// [����] ��� �õ�
				m_nStep_BCRRead = 300;
			}
		}
		break;

	case 300:
		// BCR Read Retry �� m_dPos_Target�� X_RBT_ACC_BCR_���� d_X_Gripper_Size * nzMode�� ���� ���� ���� ��ġ�̴�.
		// nzMode(SIDE_LEFT=0, SIDE_RIGHT=1)
		m_dPos_Target[0] = st_motor[m_nMotX].d_pos[X_RBT_ACC_BCR_] + (st_accy.d_X_Gripper_Size * nzMode);
		m_dMaxOffset += st_motor[m_nMotX].d_pos[X_RBT_ACC_BCR_OFFSET_];

		if (m_nNearby == SIDE_LEFT_)
		{
			// [OFFSET] ������ [5mm] �̳��� ����
			if (m_dMaxOffset > MAX_OFFSET_)
			{
				m_dMaxOffset = MAX_OFFSET_;
			}
			// Y�� ���͸� �ϴ����� �̵��� ��ġ ���� ����
			m_dPos_Target[0] += m_dMaxOffset;
		}
		else if (m_nNearby == SIDE_RIGHT_)
		{
			if (m_dMaxOffset > MAX_OFFSET_)
			{
				m_dMaxOffset = MAX_OFFSET_;
			}
			// Y�� ���͸� ������� �̵��� ��ġ ���� ����
			m_dPos_Target[0] -= m_dMaxOffset;
		}
		m_nStep_BCRRead = 310;
		break;

	case 310:
		// Y���� Offset ��ŭ �̵�
		nRet = CTL_Lib.OnSingleMove(m_nMotX, m_dPos_Target[0], (int)st_handler.md_run_speed);
		if (nRet == CTLBD_RET_GOOD)
		{
			m_nStep_BCRRead = 110;
		}
		else if (nRet == CTLBD_RET_ERROR || nRet == CTLBD_RET_RETRY)
		{
			// 069001 0 00 "BARCODE_ROBOT_X_RBT_ACC_BCR_RETRY_OFFSET_MOVE_ERR."
			alarm.mstr_code		= "069001";
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
			CTL_Lib.Alarm_Error_Occurrence( 3000, CTL_dWARNING, alarm.mstr_code);
		}
		break;

	case 5000:
		nFuncRet = CTL_GOOD;
		m_nStep_BCRRead = 0;
		break;
	}
	return nFuncRet;
}

//////////////////////////////////////////////////////////////////////////
// ���ڵ� ���°� Bad�� ��� ó���ϴ� �Լ�
//////////////////////////////////////////////////////////////////////////
int CSeq_XYZRobot::OnProc_BCRStateBad()
{
	int nFuncRet = CTL_PROCEED;
	
	int nRet = VAR_INIT_;
	int nRetData[4] = {0,};
	int i = 0;
	int iCheckCount = 0;
	int nSupplyMathCount = 0;
	
	Func.OnTrace_ThreadStep(11, m_nStep_BCRBad);	// ������ ���� ���� TRACE
	
	switch(m_nStep_BCRBad)
	{
	case 0:
		// ��ȣ�� ���� ���� ���� �������� 0(���� ��), 1(���� ��), 2(���� ��), 3(���� ��)
		// NG Buffer ���� Ȯ��
		nRetData[0] = OnCheck_NGBufferAccy(IO_ON, CHK_EXIST_, SIDE_LEFT_);
		nRetData[1] = OnCheck_NGBufferAccy(IO_ON, CHK_EXIST_, SIDE_RIGHT_);
// 		nRetData[2] = OnCheck_NGBufferRearAccy(IO_ON, CHK_EXIST_, SIDE_LEFT_);
// 		nRetData[3] = OnCheck_NGBufferRearAccy(IO_ON, CHK_EXIST_, SIDE_RIGHT_);

		if (st_basic.n_mode_device == WITHOUT_DVC_)
		{
			nRetData[0] = st_map.nNGBuffer[0];
			nRetData[1] = st_map.nNGBuffer[1];
// 			nRetData[2] = st_map.nNGBuffer[2];
// 			nRetData[3] = st_map.nNGBuffer[3];			
		}

		for (i = 0; i < 2/*4*/; i++)
		{
			if (st_map.nNGBuffer[i] != nRetData[i])
			{
				iCheckCount++;
			}
		}

		if (iCheckCount > 0)
		{
			// 143000 0 00 "PS1506_PS1507_NG_BUFFER_ACCY_EXSIT_CHK_ERR."
			alarm.mstr_code		= "143000";
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
			CTL_Lib.Alarm_Error_Occurrence( 3000, CTL_dWARNING, alarm.mstr_code );
			break;
		}

		// 1�� Gripper : NG Buffer(1, 3) Ȯ��
		if(st_map.nBCR_State[0] == BCR_READ_BAD_)
		{
			if (nRetData[0] == IO_ON)
			{
				nSupplyMathCount++;
				nRetData[0] = IO_OFF;
			}
			else if (nRetData[2] == IO_ON)
			{
				nSupplyMathCount++;
				nRetData[2] = IO_OFF;
			}
		}
		else if(st_map.nBCR_State[0] == BCR_READ_GOOD_)
		{
			nSupplyMathCount++;
		}

		// 2�� Gripper : NG Buffer(2, 4) Ȯ��
		if(st_map.nBCR_State[1] == BCR_READ_BAD_)
		{
			if (nRetData[1] == IO_ON)
			{
				nSupplyMathCount++;
				nRetData[1] = IO_OFF;
			}
			else if (nRetData[3] == IO_ON)
			{
				nSupplyMathCount++;
				nRetData[3] = IO_OFF;
			}
		}
		else if(st_map.nBCR_State[1] == BCR_READ_GOOD_)
		{
			nSupplyMathCount++;
		}

// 		// 3�� Gripper : NG Buffer(3, 4) Ȯ��
// 		if(st_map.nBCR_State[2] == BCR_READ_BAD_)
// 		{
// 			if (nRetData[2] == IO_ON)
// 			{
// 				nSupplyMathCount++;
// 				nRetData[2] = IO_OFF;
// 			}
// 			else if (nRetData[0] == IO_ON)
// 			{
// 				nSupplyMathCount++;
// 				nRetData[0] = IO_OFF;
// 			}
// 		}
// 		else if(st_map.nBCR_State[2] == BCR_READ_GOOD_)
// 		{
// 			nSupplyMathCount++;
// 		}

// 		// 4�� Gripper : NG Buffer(3, 4) Ȯ��
// 		if(st_map.nBCR_State[3] == BCR_READ_BAD_)
// 		{
// 			if (nRetData[3] == IO_ON)
// 			{
// 				nSupplyMathCount++;
// 				nRetData[3] = IO_OFF;
// 			}
// 			else if (nRetData[1] == IO_ON)
// 			{
// 				nSupplyMathCount++;
// 				nRetData[1] = IO_OFF;
// 			}
// 		}
// 		else if(st_map.nBCR_State[3] == BCR_READ_GOOD_)
// 		{
// 			nSupplyMathCount++;
// 		}

		// ���� �� ���簡 ���� ���
		if (nSupplyMathCount == 2/*4*/)
		{
			m_nStep_BCRBad = 1000;
		}
		// ���� �� ���簡 ���� ��� NG Buffer�� �ߺ��Ǵ� Gripper�� ���� �� �� Accy ����
		else
		{
			m_nStep_BCRBad = 8000;
		}
		break;

	case 1000:
		// Gripper�� ��� �ִ� Accy ���� (SIDE_FRONT_ : 1, 2�� Gripper, SIDE_REAR_ : 3, 4�� Gripper)
		nRet = OnProc_AccyOut(st_map.nBCR_State, SIDE_FRONT_);
		
		if (nRet == CTL_GOOD)
		{
			m_nStep_BCRBad = 1100;
		}
		break;

	case 1100:
		// Gripper�� ��� �ִ� Accy ���� (SIDE_FRONT_ : 1, 2�� Gripper, SIDE_REAR_ : 3, 4�� Gripper)
		nRet = OnProc_AccyOut(st_map.nBCR_State, SIDE_REAR_);
		
		if (nRet == CTL_GOOD)
		{
			stSync.nReq_XYZRbt2BufferAlaignConv_Work = SYNC_REQ_BUFFER_LOADING_READY_;
			m_nStep_BCRBad = 1300;
		}
		break;

	case 1300:
		// [1:4����, 0:2����]
		if (st_basic.nMode_PlaceType == 0)
		{
			nRet = OnProc_BufferPlace();
		}
		else
		{
			nRet = OnProc_4BufferPlace();	// [Main<-Accy] ���޿� ���Ͽ� 4�� �Ǽ��縮 �Ѳ����� ��������
		}
		
		if (nRet == CTL_GOOD)
		{
			Func.OnLogBCRData("[Seq_XYZRobot_OnProc_BCRStateBad_1300_Good]");
			m_nStep_BCRBad = 1400;
		}
		break;

	case 1400:
		// ��ȣ�� ���� ���� ���� �������� 0(���� ��), 1(���� ��), 2(���� ��), 3(���� ��)
		// NG Buffer ���� Ȯ��
		nRetData[0] = OnCheck_NGBufferAccy(IO_ON, CHK_EXIST_, SIDE_LEFT_);
		nRetData[1] = OnCheck_NGBufferAccy(IO_ON, CHK_EXIST_, SIDE_RIGHT_);
		nRetData[2] = IO_OFF;
		nRetData[3] = IO_OFF;
		iCheckCount = 0;

		if (st_basic.n_mode_device == WITHOUT_DVC_)
		{
			for (i = 0; i < 4; i++)
			{
				nRetData[i] = st_map.nNGBuffer[i];
			}
		}

		for (i = 0; i < 4; i++)
		{
			if (st_map.nNGBuffer[i] != nRetData[i])
			{
				iCheckCount++;
			}
		}

		if (iCheckCount > 0)
		{
			// 151000 0 00 "PS1204_PS1205_PS1206_PS1207_NG_BUFFER_DATA_MISS_MATH."
			alarm.mstr_code		= "151000";
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
			CTL_Lib.Alarm_Error_Occurrence( 3000, CTL_dWARNING, alarm.mstr_code );
			break;
		}

		// Gripper �� NG Buffer ���� 1:1 �� 
		if (st_map.nBCR_State[0] == BCR_READ_BAD_ && nRetData[0] == IO_ON ||
			st_map.nBCR_State[1] == BCR_READ_BAD_ && nRetData[1] == IO_ON ||
			st_map.nBCR_State[2] == BCR_READ_BAD_ && nRetData[2] == IO_ON ||
			st_map.nBCR_State[3] == BCR_READ_BAD_ && nRetData[3] == IO_ON)
		{
			// ���� ���� ��ġ�� ���� ���簡 �ִ� ���
			m_nNGBufferPickSite = SIDE_BOTH_; 
		}		
		// ���� Line ���� ��
		// Gripper 0 �Ǵ� 1�� BCR ���°� Bad�� ��� NG Buffer Rear�� ���縦 Ȯ��
// 		else if(st_map.nBCR_State[0] == BCR_READ_BAD_ && nRetData[2] == IO_ON || st_map.nBCR_State[1] == BCR_READ_BAD_ && nRetData[3] == IO_ON )
// 		{
// 			m_nNGBufferPickSite = SIDE_REAR_; 
// 		}
// 		// Gripper 2 �Ǵ� 3�� BCR ���°� Bad�� ��� NG Buffer Front�� ���縦 Ȯ��
// 		else if(st_map.nBCR_State[2] == BCR_READ_BAD_ && nRetData[0] == IO_ON || st_map.nBCR_State[3] == BCR_READ_BAD_ && nRetData[1] == IO_ON)
// 		{
// 			m_nNGBufferPickSite = SIDE_FRONT_; 
// 		}
		else if(st_map.nBCR_State[0] == BCR_READ_BAD_ && nRetData[0] == IO_ON || st_map.nBCR_State[1] == BCR_READ_BAD_ && nRetData[1] == IO_ON )
		{
			m_nNGBufferPickSite = SIDE_FRONT_; 
		}
		m_nStep_BCRBad = 2000;
		break;

	case  2000:
		nRet = OnProc_NGBufferPick(m_nNGBufferPickSite);
		if (nRet == CTL_GOOD)
		{
			Func.OnLogBCRData("[Seq_XYZRobot_OnProc_BCRStateBad_2000_Good]");
			m_nStep_BCRBad = 2100;	
		}
		break;

	case 2100:
		// ���̸� ���� [2015.03.25]
		// [1:4����, 0:2����]
		if (st_basic.nMode_PlaceType == 0)
		{
			nRet = OnProc_BufferPlace();
		}
		else
		{
			nRet = OnProc_4BufferPlace();	// [Main<-Accy] ���޿� ���Ͽ� 4�� �Ǽ��縮 �Ѳ����� ��������
		}
		
		if (nRet == CTL_GOOD)
		{
			Func.OnLogBCRData("[Seq_XYZRobot_OnProc_BCRStateBad_2100_Good]");
			// BCR ���°� All Good �̸� �Ϸ� Step���� �̵�
			if(st_map.nBCR_State[0] == BCR_READ_GOOD_ &&
				st_map.nBCR_State[1] == BCR_READ_GOOD_ &&
				st_map.nBCR_State[2] == BCR_READ_GOOD_ &&
				st_map.nBCR_State[3] == BCR_READ_GOOD_)
			{
				stSync.nReq_XYZRbt2BufferAlaignConv_Work = SYNC_REQ_WORK_COMPLETE_;
				m_nStep_BCRBad = 10000;
			}
			// BCR ���°� Bad�� ���� ��� 
			// �ٸ� ��ġ�� �Ǽ��縮 ������ ���� 1800 Step�� �̵�
			else if(st_map.nBCR_State[0] == BCR_READ_BAD_ || st_map.nBCR_State[1] == BCR_READ_BAD_ || st_map.nBCR_State[2] == BCR_READ_BAD_ || st_map.nBCR_State[3] == BCR_READ_BAD_)
			{
				m_nStep_BCRBad = 1800;
			}
		}
		break;

	case 8000:
		nRet = OnProc_NGBufferPlace(m_nNGBufferPickSite);
		if (nRet == CTL_GOOD)
		{
			Func.OnLogBCRData("[Seq_XYZRobot_OnProc_BCRStateBad_8000_Good]");
			m_nStep_BCRBad = 10000;
		}
		break;
		
	case 10000:
		m_nStep_BCRBad = 0;
		nFuncRet = CTL_GOOD;
		break;
		

	}

	return nFuncRet;

}

//////////////////////////////////////////////////////////////////////////
// SSlop Front Rear Buffer���� �۾�
//////////////////////////////////////////////////////////////////////////
int CSeq_XYZRobot::OnProc_BufferConvClear()
{
	int nFuncRet = CTL_PROCEED;
	int nRet = VAR_INIT_;
	int i, iCheckCount = 0;
	int nRetData[4] = {0,};

	Func.OnTrace_ThreadStep(20, m_nStep_BufferConvClear);	// ������ ���� ���� TRACE

	switch( m_nStep_BufferConvClear )
	{
	case 0:
		nRet = CTL_Lib.OnSingleMove(m_nMotZ, st_motor[m_nMotZ].d_pos[Z_RBT_SAFETY_], (int)st_handler.md_run_speed);
		if (nRet == CTLBD_RET_GOOD)
		{
			m_nStep_BufferConvClear = 10;
		}
		else if (nRet == CTLBD_RET_ERROR || nRet == CTLBD_RET_RETRY)
		{
			// 071005 0 07 "BARCODE_ROBOT_Z_RBT_SAFETY_MOVE_ERR."
			alarm.mstr_code		= "071005";
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
			CTL_Lib.Alarm_Error_Occurrence( 3000, CTL_dWARNING, alarm.mstr_code);
		}
		break;

	case 10:
		nRet = CTL_Lib.OnSingleMove(m_nMotY, st_motor[m_nMotY].d_pos[Y_RBT_ACC_CONV_R_BUFF_], (int)st_handler.md_run_speed);
		if (nRet == CTLBD_RET_GOOD)
		{
			m_nStep_BufferConvClear = 100;
		}
		else if (nRet == CTLBD_RET_ERROR || nRet == CTLBD_RET_RETRY)
		{
			// 051010 0 00 "BARCODE_ROBOT_Y_RBT_ACC_BUFFER_CONV_REAR_MOVE_ERR."
			alarm.mstr_code		= "051010";
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
			CTL_Lib.Alarm_Error_Occurrence( 3000, CTL_dWARNING, alarm.mstr_code);
		}		
		break;

	case 100:
		nRetData[0] = OnCheck_FrontAccySupply(IO_ON, CHK_NOT_EXIST_, SIDE_LEFT_);		// Buffer Conv Front Accy Ȯ��
		nRetData[1] = OnCheck_FrontAccySupply(IO_ON, CHK_NOT_EXIST_, SIDE_RIGHT_);		// Buffer Conv Front Accy Ȯ��		
		nRetData[2] = OnCheck_RearAccySupply(IO_ON, CHK_NOT_EXIST_, SIDE_LEFT_);		// Buffer Conv Rear Accy Ȯ��
		nRetData[3] = OnCheck_RearAccySupply(IO_ON, CHK_NOT_EXIST_, SIDE_RIGHT_);		// Buffer Conv Rear Accy Ȯ��
		
		if (st_basic.n_mode_device == WITHOUT_DVC_)
		{
			for(i=0; i<MAX_PICKER_; i++)
			{
				nRetData[i] = st_map.nBufferCovAccyExist[i];
			}
		}
		
		// kilee [2015.03.17]
		for(i=0; i<MAX_PICKER_; i++)
		{
			m_nSensor[i] = nRetData[i];
		}
		
		m_nGirpSum = (nRetData[0] * 1) + (nRetData[1] * 2) + (nRetData[2] * 4) + (nRetData[3] * 8);
		
		m_nStep_BufferConvClear = 200;
		break;

	case 200:
		m_nRepickCount = 0;  // [����] ���� ���� �õ� Ƚ��

		// ��ü ��Ŀ ���� ���
		for(i=0; i<MAX_PICKER_; i++)
		{
			m_nPickerUnDn[TYPE_FLAG_][i] = CTL_NO;	// [����] �÷��� ����
			m_nPicker[TYPE_FLAG_][i] = CTL_NO;
			
			if (m_nSensor[i] == IO_ON)
			{
				m_nPickerUnDn[TYPE_FLAG_][i] = CTL_YES;// ������ Picker�� ����
				m_nPicker[TYPE_FLAG_][i] = CTL_YES;	// ������ Vacuum���� ����
			}
		}
		m_nStep_BufferConvClear = 210;
		break;

	case 210:
		// Z�� ���͸� NG ���ۿ��� �Ǽ��縮 ���� ���� ��ġ�� �̵�
		nRet = COMI.Start_SingleMove(m_nMotZ, st_motor[m_nMotZ].d_pos[Z_RBT_ACC_BUFFER_CONV_PICK_], (int)st_handler.md_run_speed);
		if (nRet == CTLBD_RET_GOOD)
		{
			OnSet_PickerUpDn( IO_ON, m_nPicker[TYPE_FLAG_]);
			OnSet_SolVacuum(IO_ON, m_nPicker[TYPE_FLAG_]);	//Vacuum [ON:��������, OFF:��������]
			m_nStep_BufferConvClear = 220;
		}
		else if (nRet == CTLBD_RET_ERROR || nRet == CTLBD_RET_SAFETY)
		{
			m_nStep_BufferConvClear = 210;

			// 071004 0 00 "BARCODE_ROBOT_Z_RBT_ACC_BUFFER_CONV_PICK_MOVE_ERR."
			alarm.mstr_code		= "071004";
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
			CTL_Lib.Alarm_Error_Occurrence( 3000, CTL_dWARNING, alarm.mstr_code);
		}
		break;

	case 220:
		nRet = COMI.Check_SingleMove(m_nMotZ, st_motor[m_nMotZ].d_pos[Z_RBT_ACC_BUFFER_CONV_PICK_]);
		if (nRet == CTLBD_RET_GOOD)
		{
			m_nStep_BufferConvClear = 230;
		}
		else if (nRet == CTLBD_RET_RETRY)
		{
			m_nStep_BufferConvClear = 210;
		}
		else if (nRet == CTLBD_RET_ERROR || nRet == CTLBD_RET_SAFETY)
		{
			m_nStep_BufferConvClear = 210;
			
			// 071004 0 00 "BARCODE_ROBOT_Z_RBT_ACC_BUFFER_CONV_PICK_MOVE_ERR."
			alarm.mstr_code		= "071004";
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
			CTL_Lib.Alarm_Error_Occurrence( 3000, CTL_dWARNING, alarm.mstr_code);
		}
		break;

	case 230:
		nRetData[0] = OnGet_PickerUpDn( IO_ON, m_nPickerUnDn[TYPE_FLAG_], m_nPickerUnDn[TYPE_SEN_]);
		nRetData[1] = OnGet_SolVacuum(IO_ON, m_nPicker[TYPE_FLAG_], m_nPicker[TYPE_SEN_]);
		if ( ( nRetData[0] == CTL_GOOD || nRetData[0] == CTL_ERROR ) && ( nRetData[1] == CTL_GOOD || nRetData[1] == CTL_ERROR ) )
		{
			m_nStep_BufferConvClear = 240;
		}
		break;

	case 240:
		nRet = COMI.Start_SingleMove(m_nMotZ, st_motor[m_nMotZ].d_pos[Z_RBT_SAFETY_], (int)st_handler.md_run_speed);
		if (nRet == CTLBD_RET_GOOD)
		{
			OnSet_PickerUpDn( IO_OFF, m_nPickerUnDn[TYPE_FLAG_]);

			m_lTime_GoesBy[0] = GetCurrentTime();
			m_nStep_BufferConvClear = 250;
		}
		else if (nRet == CTLBD_RET_ERROR || nRet == CTLBD_RET_RETRY)
		{
			// 071005 0 07 "BARCODE_ROBOT_Z_RBT_SAFETY_MOVE_ERR."
			alarm.mstr_code		= "071005";
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
			CTL_Lib.Alarm_Error_Occurrence( 3000, CTL_dWARNING, alarm.mstr_code);
		}
		break;

	case 250:
		nRet = COMI.Check_SingleMove(m_nMotZ, st_motor[m_nMotZ].d_pos[Z_RBT_SAFETY_]);
		if (nRet == CTLBD_RET_GOOD)
		{
			m_nStep_BufferConvClear = 260;
		}
		else if (nRet == CTLBD_RET_RETRY)
		{
			m_nStep_BufferConvClear = 240;
		}
		else if (nRet == CTLBD_RET_ERROR || nRet == CTLBD_RET_RETRY)
		{
			// 071005 0 07 "BARCODE_ROBOT_Z_RBT_SAFETY_MOVE_ERR."
			alarm.mstr_code		= "071005";
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
			CTL_Lib.Alarm_Error_Occurrence( 3000, CTL_dWARNING, alarm.mstr_code);
			m_nStep_BufferConvClear = 240;
		}
		break;

	case 260:
		nRetData[0] = OnGet_PickerUpDn( IO_OFF, m_nPickerUnDn[TYPE_FLAG_], m_nPickerUnDn[TYPE_SEN_]);
		nRetData[1] = OnGet_SolVacuum(IO_ON, m_nPicker[TYPE_FLAG_], m_nPicker[TYPE_SEN_]);
		if ( ( nRetData[0] == CTL_GOOD || nRetData[0] == CTL_ERROR ) && ( nRetData[1] == CTL_GOOD || nRetData[1] == CTL_ERROR ) )
		{
			m_lTime_GoesBy[0] = GetCurrentTime();
			m_nStep_BufferConvClear = 270;
		}
		break;

	case 270:
		nRet = Func.OnGet_PickerStatus(0, CTL_YES, m_nPicker);
		if (nRet == CTL_ERROR)
		{
			m_lTime_GoesBy[1] = GetCurrentTime();
			m_lTime_GoesBy[2] = m_lTime_GoesBy[1] - m_lTime_GoesBy[0];
			if (m_lTime_GoesBy[2] < 0)
			{
				m_lTime_GoesBy[0] = GetCurrentTime();
				break;
			}
			
			if (m_lTime_GoesBy[2] < 10)
			{
				break;
			}
		}
		// [���� ����] : ��ٷ� �ؿ� �κ� ����
		// [���� ����] : ���� �ð� ��� �Ŀ� �ؿ� �κ� ����

		// ��ü �׸��� ���� ���
		for(i=0; i<MAX_PICKER_; i++)
		{
			// �Ǽ��縮 ���� ���� ������ �׸��ۿ� ���ؼ� �Ǽ��縮�� Vacuum ������ ������ ���
			if (m_nPicker[TYPE_FLAG_][i] == CTL_YES && m_nPicker[TYPE_SEN_][i] == CTL_YES)
			{
				// ����ü ���� ���� [���� PICKUP]
				st_map.nXYZRbtPickerInfo[i]	= CTL_YES;
				m_nPicker[TYPE_FLAG_][i]	= CTL_NO;
			}
		}

		// OnGet_PickerStatus() �Լ������� [CTL_GOOD/CTL_ERROR] ��ȯ�Ǿ��� ����
		if (nRet == CTL_GOOD)
		{
			m_nStep_BufferConvClear = 500;
		}
		else
		{
			m_nRepickCount++;	// ��õ� Ƚ�� ����
			if (m_nRepickCount > 2)
			{
				m_nRepickCount = 2;
			}
			m_nStep_BufferConvClear = 210;	// �Ǽ��縮 ���� ���� ��õ�

			if (m_nRepickCount >= 2)
			{
				m_nRepickCount = 0;	// [����] ���� ���� �õ� Ƚ��

				// OnGet_PickerStatus() �Լ� �ȿ��� �˶� �ڵ� ������
				alarm.mn_count_mode	= 0;
				alarm.mn_type_mode	= eWARNING;
				st_work.nEqpStatus	= dWARNING;
				CTL_Lib.Alarm_Error_Occurrence( 3000, CTL_dWARNING, alarm.mstr_code );
			}
		}
		break;
		//==========================================================//

	case 500:
		nRetData[0] = OnCheck_FrontAccySupply(IO_ON, CHK_NOT_EXIST_, SIDE_LEFT_);
		nRetData[1] = OnCheck_FrontAccySupply(IO_ON, CHK_NOT_EXIST_, SIDE_RIGHT_);	
		nRetData[2] = OnCheck_RearAccySupply(IO_ON, CHK_NOT_EXIST_, SIDE_LEFT_);
		nRetData[3] = OnCheck_RearAccySupply(IO_ON, CHK_NOT_EXIST_, SIDE_RIGHT_);

		for (i = 0; i < MAX_PICKER_; i++)
		{
			if(st_map.nXYZRbtPickerInfo[i] == IO_ON)
			{
				if (nRetData[i] != IO_OFF)
				{
					iCheckCount++;
				}
			}
		}

		if (iCheckCount > 0)
		{
			m_nStep_BufferConvClear = 0;

			// 142000 0 00 "PS1106_PS1107_PS1108_PS1109_ACCY_CONV_ACCY_EXSIT_CHK_ERR."
			alarm.mstr_code		= "142000";
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
			CTL_Lib.Alarm_Error_Occurrence( 3000, CTL_dWARNING, alarm.mstr_code);
			break;
		}
		else
		{
			for (i = 0; i < MAX_PICKER_; i++)
			{
				if(st_map.nXYZRbtPickerInfo[i] == CTL_YES)
				{
					st_map.nBCR_State[i] = BCR_READ_BAD_;
				}
				else
				{
					st_map.nBCR_State[i] = BCR_READ_GOOD_;
				}
			}
			m_nStep_BufferConvClear = 1000;
		}
		break;
		
	case 1000:
		// Accy ���� (SIDE_FRONT_ : 1, 2�� Gripper, SIDE_REAR_ : 3, 4�� Picker)
		nRet = OnProc_AccyOut(st_map.nBCR_State, SIDE_REAR_);
		if (nRet == CTLBD_RET_GOOD)
		{
			m_nStep_BufferConvClear = 1100;
		}
		break;
		
	case 1100:
		// Accy ���� (SIDE_FRONT_ : 1, 2�� Gripper, SIDE_REAR_ : 3, 4�� Picker)
		nRet = OnProc_AccyOut(st_map.nBCR_State, SIDE_FRONT_);
		if (nRet == CTLBD_RET_GOOD)
		{
			m_nStep_BufferConvClear = 1200;
		}
		break;
		
	case 1200:
		nRet = CTL_Lib.OnSingleMove(m_nMotY, st_motor[m_nMotY].d_pos[Y_RBT_ACC_NG_BUFFER_], (int)st_handler.md_run_speed);
		if (nRet == CTLBD_RET_GOOD)
		{
			m_nStep_BufferConvClear = 2000;
		}
		else if (nRet == CTLBD_RET_ERROR || nRet == CTLBD_RET_RETRY)
		{
			// 051008 0 00 "BARCODE_ROBOT_Y_RBT_ACC_NG_BUFFER_MOVE_ERR."
			alarm.mstr_code		= "051008";
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
		}
		break;

	case 2000:
		memset(st_map.nBufferCovAccyExist, ACCY_NO_, sizeof(st_map.nBufferCovAccyExist));
		if (st_handler.cwnd_main != NULL)
		{
			st_handler.cwnd_main->PostMessage(WM_UPDATE_MAIN, PLACE_BUFFER_CONV_);	// ȭ�� ǥ�� ��û
		}
		m_nStep_BufferConvClear = 0;
		nFuncRet = CTL_GOOD;
		break;
	}

	return nFuncRet;
}

// Buffer Conv�� ���縦 ���� �Լ�
int CSeq_XYZRobot::OnProc_BufferPlace()
{
	int nFuncRet = CTL_PROCEED;
	
	int nRet = VAR_INIT_;
	int nRetData[4] = {0,};
	int i = 0, iExist;
	int iCheckCount = 0;
	
	Func.OnTrace_ThreadStep(22, m_nStep_BufferConvPlace);	// ������ ���� ���� TRACE
	
	switch(m_nStep_BufferConvPlace)
	{
	case 0:
		nRet = CTL_Lib.OnSingleMove(m_nMotZ, st_motor[m_nMotZ].d_pos[Z_RBT_SAFETY_], (int)st_handler.md_run_speed);
		if (nRet == CTLBD_RET_GOOD)
		{
			m_nStep_BufferConvPlace = 10;
		}
		else if (nRet == CTLBD_RET_ERROR || nRet == CTLBD_RET_RETRY)
		{
			// 071005 0 0 "BARCODE_ROBOT_Z_RBT_SAFETY_MOVE_ERR"
			alarm.mstr_code		= "071005";
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
			CTL_Lib.Alarm_Error_Occurrence( 3000, CTL_dWARNING, alarm.mstr_code );
		}
		break;
	
		//==========================================================//
		// Vacuum ��� ���
		//==========================================================//
	case 10:
		for(i=0; i<MAX_PICKER_; i++)
		{
			m_nPicker[TYPE_FLAG_][i] = st_map.nXYZRbtPickerInfo[i];
		}
		m_lTime_GoesBy[0] = GetCurrentTime();
		m_nStep_BufferConvPlace = 20;
		break;

	case 20:
		// ��Ŀ�� ���� ���� ���� ��ȯ
		nRet = Func.OnGet_PickerStatus(0, CTL_YES, m_nPicker);
		if (nRet == CTL_ERROR)
		{
			m_lTime_GoesBy[1] = GetCurrentTime();
			m_lTime_GoesBy[2] = m_lTime_GoesBy[1] - m_lTime_GoesBy[0];
			if (m_lTime_GoesBy[2] < 0)
			{
				m_lTime_GoesBy[0] = GetCurrentTime();
				break;
			}
			
			if (m_lTime_GoesBy[2] < 10)
			{
				break;
			}
		}
		
		// OnGet_PickerStatus() �Լ������� [CTL_GOOD/CTL_ERROR] ��ȯ�Ǿ��� ����
		if (nRet == CTL_GOOD)
		{
			m_nStep_BufferConvPlace = 50;
		}
		else
		{
			// OnGet_PickerStatus() �Լ� �ȿ��� �˶� �ڵ� ������
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
			CTL_Lib.Alarm_Error_Occurrence( 3000, CTL_dWARNING, alarm.mstr_code );
		}
		break;

	case 50:
		// ��ü ��Ŀ ���� ���
		for(i=0; i<MAX_PICKER_; i++)
		{
			m_nPicker[TYPE_FLAG_][i] = CTL_NO;
			m_nPickerUnDn[TYPE_FLAG_][i] = CTL_NO;
			
			// ���� ���� ���θ� �˻� [����ü �÷��� ����]
			// - �Ǽ��縮 �������� ������ ������ ��Ŀ�� ����
			iExist = st_map.nXYZRbtPickerInfo[i];
			if (iExist == CTL_YES)
			{
				m_nPicker[TYPE_FLAG_][i] = CTL_YES;	// ������ ��Ŀ�� ����
				m_nPickerUnDn[TYPE_FLAG_][i] = CTL_YES;
			}
		}
		
		// (m_nPicker[TYPE_FLAG_]�� Accy ������ Ȯ��
		if (m_nPicker[TYPE_FLAG_][2] == ACCY_YES_ || m_nPicker[TYPE_FLAG_][3] == ACCY_YES_)
		{
			m_nStep_BufferConvPlace = 100;
		}
		else if(m_nPicker[TYPE_FLAG_][0] == ACCY_YES_ || m_nPicker[TYPE_FLAG_][1] == ACCY_YES_)
		{
			m_nStep_BufferConvPlace = 500;
		}
		else
		{
			m_nStep_BufferConvPlace = 1000;
		}
		break;	
		
	case 100:
		// �̵��� ���� ��ġ ����
		m_dTargetPos = st_motor[m_nMotY].d_pos[Y_RBT_ACC_CONV_R_BUFF_];
		
		nRet = CTL_Lib.OnSingleMove(m_nMotY, m_dTargetPos, (int)st_handler.md_run_speed);
		if (nRet == CTLBD_RET_GOOD)
		{
			m_nStep_BufferConvPlace = 110;			
		}
		else if (nRet == CTLBD_RET_ERROR || nRet == CTLBD_RET_RETRY)
		{
			// 051011 0 0 "BARCODE_ROBOT_X_RBT_ACC_BUFFER_CONV_MOVE_ERR"
			alarm.mstr_code		= "051011";
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
			CTL_Lib.Alarm_Error_Occurrence( 3000, CTL_dWARNING, alarm.mstr_code );
		}
		break;

	case 110:
		// ��Ŀ�� ���� ���� ���� ��ȯ
		nRet = Func.OnGet_PickerStatus(0, CTL_YES, m_nPicker);
		if (nRet == CTL_ERROR)
		{
			m_lTime_GoesBy[1] = GetCurrentTime();
			m_lTime_GoesBy[2] = m_lTime_GoesBy[1] - m_lTime_GoesBy[0];
			if (m_lTime_GoesBy[2] < 0)
			{
				m_lTime_GoesBy[0] = GetCurrentTime();
				break;
			}
			
			if (m_lTime_GoesBy[2] < 10)
			{
				break;
			}
		}
		
		// OnGet_PickerStatus() �Լ������� [CTL_GOOD/CTL_ERROR] ��ȯ�Ǿ��� ����
		if (nRet == CTL_GOOD)
		{
			// [Conv->Xyz] �Ǽ��縮 ���� ��û ��ٸ�
			if (stSync.nResp_BufferAlaignConv2XYZRbt_Work == SYNC_RESP_LOADING_)
			{
				m_nStep_BufferConvPlace = 200;
			}
			else
			{
				m_nStep_BufferConvPlace = 150;
			}
		}
		else
		{
			// OnGet_PickerStatus() �Լ� �ȿ��� �˶� �ڵ� ������
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
			CTL_Lib.Alarm_Error_Occurrence( 3000, CTL_dWARNING, alarm.mstr_code );
		}
		break;

	case 150:
		// [Conv->Xyz] �Ǽ��縮 ���� ��û ��ٸ�
		if (stSync.nResp_BufferAlaignConv2XYZRbt_Work == SYNC_RESP_LOADING_)
		{
			m_nStep_BufferConvPlace = 200;
		}
		else
		{
			m_nStep_BufferConvPlace = 160;
			
		}
		break;

	case 160:
		// Conv�� �Ǽ��縮 �������� �ð��� ���̱� ���Ͽ� Ư�� ��ġ���� ������ ����
		m_dTargetPos = st_motor[m_nMotZ].d_pos[Z_RBT_ACC_BUFFER_CONV_PLACE_] -40;
		
		//nRet = CTL_Lib.OnSingleMove(m_nMotZ, m_dTargetPos, (int)st_handler.md_run_speed);
		nRet = COMI.Start_SingleMove(m_nMotZ, m_dTargetPos, (int)st_handler.md_run_speed);
		if (nRet == CTLBD_RET_GOOD)
		{
			OnSet_PickerUpDn( IO_ON, m_nPickerUnDn[TYPE_FLAG_] );
			m_nStep_BufferConvPlace = 170;
		}
		else if (nRet == CTLBD_RET_ERROR || nRet == CTLBD_RET_RETRY)
		{
			// 071005 0 0 "BARCODE_ROBOT_Z_RBT_SAFETY_MOVE_ERR"
			alarm.mstr_code		= "071005";
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
			CTL_Lib.Alarm_Error_Occurrence( 3000, CTL_dWARNING, alarm.mstr_code );
		}
		break;

	case 170:
		nRet = COMI.Check_SingleMove(m_nMotZ, st_motor[m_nMotZ].d_pos[Z_RBT_ACC_BUFFER_CONV_PICK_]);
		if (nRet == CTLBD_RET_GOOD)
		{
			m_nStep_BufferConvPlace = 180;
		}
		else if (nRet == CTLBD_RET_RETRY)
		{
			m_nStep_BufferConvPlace = 170;
		}
		else if (nRet == CTLBD_RET_ERROR || nRet == CTLBD_RET_SAFETY)
		{
			m_nStep_BufferConvPlace = 160;
			
			// 071005 0 0 "BARCODE_ROBOT_Z_RBT_SAFETY_MOVE_ERR"
			alarm.mstr_code		= "071005";
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
			CTL_Lib.Alarm_Error_Occurrence( 3000, CTL_dWARNING, alarm.mstr_code);
		}
		break;	


	case 190:
		// [Conv->Xyz] �Ǽ��縮 ���� ��û ��ٸ�
		if (stSync.nResp_BufferAlaignConv2XYZRbt_Work == SYNC_RESP_LOADING_)
		{
			m_nStep_BufferConvPlace = 200;
		}
		break;

	case 200:
		// ��ü ��Ŀ ���� ���
		for(i=0; i<MAX_PICKER_; i++)
		{
			m_nPicker[TYPE_FLAG_][i] = CTL_NO;
			m_nPickerUnDn[TYPE_FLAG_][i] = CTL_NO;
			
			// ���� ���� ���θ� �˻� [����ü �÷��� ����]
			// - �Ǽ��縮 �������� ������ ������ ��Ŀ�� ����
			iExist = st_map.nBCR_State[i];
			if (iExist == BCR_READ_GOOD_)
			{
				// Rear �κи� �����ϵ��� ��.
				if (i > 1)
				{
					m_nPicker[TYPE_FLAG_][i] = CTL_YES;	// ������ ��Ŀ�� ����
					m_nPickerUnDn[TYPE_FLAG_][i] = CTL_YES;	
				}
			}
		}
		m_nStep_BufferConvPlace = 210;
		break;

	case 210:
		// �Ǽ��縮 �������� ��ġ�� Z�� ���� �̵�
		//nRet = CTL_Lib.OnSingleMove(m_nMotZ, st_motor[m_nMotZ].d_pos[Z_RBT_ACC_BUFFER_CONV_PLACE_], (int)st_handler.md_run_speed);
		nRet = COMI.Start_SingleMove(m_nMotZ, st_motor[m_nMotZ].d_pos[Z_RBT_ACC_BUFFER_CONV_PLACE_], (int)st_handler.md_run_speed);
		if (nRet == CTLBD_RET_GOOD)
		{
			OnSet_PickerUpDn( IO_ON, m_nPickerUnDn[TYPE_FLAG_] );
			m_nStep_BufferConvPlace = 220;	// Vacuum ��� ���
		}
		else if (nRet == CTLBD_RET_ERROR || nRet == CTLBD_RET_RETRY)
		{
			// 071004 0 0 "BARCODE_ROBOT_Z_RBT_ACC_BUFFER_CONV_MOVE_ERR"
			alarm.mstr_code		= "071004";
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
			CTL_Lib.Alarm_Error_Occurrence( 3000, CTL_dWARNING, alarm.mstr_code );
		}
		break;

	case 220:
		nRet = COMI.Check_SingleMove(m_nMotZ, st_motor[m_nMotZ].d_pos[Z_RBT_ACC_BUFFER_CONV_PICK_]);
		if (nRet == CTLBD_RET_GOOD)
		{
			m_nStep_BufferConvClear = 230;
		}
		else if (nRet == CTLBD_RET_RETRY)
		{
			m_nStep_BufferConvClear = 220;
		}
		else if (nRet == CTLBD_RET_ERROR || nRet == CTLBD_RET_SAFETY)
		{
			m_nStep_BufferConvClear = 210;
			
			// 071004 0 0 "BARCODE_ROBOT_Z_RBT_ACC_BUFFER_CONV_MOVE_ERR"
			alarm.mstr_code		= "071004";
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
			CTL_Lib.Alarm_Error_Occurrence( 3000, CTL_dWARNING, alarm.mstr_code);
		}
		break;


	case 225:
		OnSet_PickerUpDn( IO_ON, m_nPickerUnDn[TYPE_FLAG_] );
		m_nStep_BufferConvPlace = 180;
		break;
		
	case 230:
		nRet = OnGet_PickerUpDn( IO_ON, m_nPickerUnDn[TYPE_FLAG_], m_nPickerUnDn[TYPE_SEN_]);
		if( nRet == CTL_GOOD )
		{
			m_nStep_BufferConvPlace = 300;
		}
		else if( nRet == CTL_ERROR )
		{
			m_nStep_WorkSiteChange = 225;
			CTL_Lib.Alarm_Error_Occurrence( 3000, CTL_dWARNING, alarm.mstr_code);
		}
		break;


	case 300:
		OnSet_SolVacuum(IO_OFF, m_nPicker[TYPE_FLAG_]);	// �׸��� Vacuum [ON:��������, OFF:��������]
		OnSet_EjectOnOff( IO_ON, m_nPicker[TYPE_FLAG_]);
		m_nStep_BufferConvPlace = 310;
		break;

	case 310:
		nRetData[0] = OnGet_SolVacuum(IO_OFF, m_nPicker[TYPE_FLAG_], m_nPicker[TYPE_SEN_]);
		nRetData[1] = OnGet_EjectOnOff( IO_ON, m_nPicker[TYPE_FLAG_]/*, m_nPicker[TYPE_SEN_]*/);
		// ����Ǿ��� ������ �Ϸ�ɶ����� ���
		// - ��Ŀ�� ��� �˶��� ��½�Ű�� ����
		if ( ( nRetData[0] == CTL_GOOD || nRetData[0] == CTL_ERROR ) && ( nRetData[1] == CTL_GOOD || nRetData[1] == CTL_ERROR ) )
		{
			OnGet_EjectOnOff( IO_OFF, m_nPicker[TYPE_FLAG_]/*, m_nPicker[TYPE_SEN_]*/);
			OnSet_PickerUpDn( IO_OFF, m_nPickerUnDn[TYPE_FLAG_] );
			m_nStep_BufferConvPlace = 320;
		}
		break;

	case 320:
		nRetData[0] = OnGet_EjectOnOff( IO_OFF, m_nPicker[TYPE_FLAG_]/*, m_nPicker[TYPE_SEN_]*/);
		nRetData[1] = OnGet_PickerUpDn( IO_OFF, m_nPickerUnDn[TYPE_FLAG_], m_nPickerUnDn[TYPE_SEN_]);
		if ( ( nRetData[0] == CTL_GOOD || nRetData[0] == CTL_ERROR ) && ( nRetData[1] == CTL_GOOD || nRetData[1] == CTL_ERROR ) )
		{
			m_nStep_BufferConvPlace = 330;
		}
		break;

	case 330:
		// �׸����� �Ǽ��縮 ���� ���� Ȯ��
		// - �Ǽ��縮�� Conv�� �����������Ƿ� �����Ǹ� �ȵ�
		// - m_nPicker : [case 400:]���� ������
		nRet = Func.OnGet_PickerStatus(0, CTL_NO, m_nPicker);
		if (nRet == CTL_ERROR)
		{
			m_lTime_GoesBy[1] = GetCurrentTime();
			m_lTime_GoesBy[2] = m_lTime_GoesBy[1] - m_lTime_GoesBy[0];
			if (m_lTime_GoesBy[2] < 0)
			{
				m_lTime_GoesBy[0] = GetCurrentTime();
				break;
			}
			
			if (m_lTime_GoesBy[2] > 10)
			{
				// OnGet_PickerStatus() �Լ� �ȿ��� �˶� �ڵ� ������
				alarm.mn_count_mode	= 0;
				alarm.mn_type_mode	= eWARNING;
				st_work.nEqpStatus	= dWARNING;
				CTL_Lib.Alarm_Error_Occurrence( 3000, CTL_dWARNING, alarm.mstr_code );
			}
			break;
		}
		
		// ��ü ��Ŀ ���� ���
		for(i=2; i<MAX_PICKER_; i++)
		{
			// �Ǽ��縮 �������� ������ �� ��Ŀ�� ���ؼ� ����� ���縦 �������� ��� (������ �̰���)
			if (m_nPicker[TYPE_FLAG_][i] == CTL_YES && m_nPicker[TYPE_SEN_][i] == CTL_NO)
			{
				// �׸����� ����ü ���� ���� [���� ������]
				st_map.nXYZRbtPickerInfo[i]	= CTL_NO;
				
				// �� ���ܿ��� �ٽ� �ʱ�ȭ ��Ų �� �缳���ϱ� ������ �ʿ�� ������..
				m_nPicker[TYPE_FLAG_][i]	= CTL_NO;
			}
			
			if (st_basic.n_mode_device == WITHOUT_DVC_)
			{
				stWithoutData.nBufferConv[i] = st_map.nBCR_State[i];
			}
			
			st_map.nBufferCovAccyExist[i] = st_map.nBCR_State[i];
			if (st_map.nBCR_State[i] == BCR_READ_GOOD_)
			{
				if (st_work.sRbtBCR_Data[i] !="")
				{
					st_work.sBufferConvBCR_Data[i] = st_work.sRbtBCR_Data[i];
					st_work.sRbtBCR_Data[i] = "";
				}
			}
		}
		
		if (st_handler.cwnd_main != NULL)
		{
			st_handler.cwnd_main->PostMessage(WM_UPDATE_MAIN, PLACE_BUFFER_CONV_);	// ȭ�� ǥ�� ��û
		}
		m_nStep_BufferConvPlace = 500;
		break;	
		

	case 500:
		// Front ��ġ�� �Ǽ��縮 �������� �ð��� ���̱� ���Ͽ� Z_RBT_ACC_BUFFER_CONV_PLACE_ - 40 ��ġ���� �ø�
		m_dTargetPos = st_motor[m_nMotZ].d_pos[Z_RBT_ACC_BUFFER_CONV_PLACE_] -40;
		
		nRet = CTL_Lib.OnSingleMove(m_nMotZ, m_dTargetPos, (int)st_handler.md_run_speed);
		if (nRet == CTLBD_RET_GOOD)
		{
			m_lTime_GoesBy[0] = GetCurrentTime();
			m_nStep_BufferConvPlace = 510;	// Vacuum ��� ���
		}
		else if (nRet == CTLBD_RET_ERROR || nRet == CTLBD_RET_RETRY)
		{
			// 071005 0 0 "BARCODE_ROBOT_Z_RBT_SAFETY_MOVE_ERR"
			alarm.mstr_code		= "071005";
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
		}
		break;

	case 510:
		for(i=0; i<MAX_PICKER_; i++)
		{
			m_nPicker[TYPE_FLAG_][i] = st_map.nXYZRbtPickerInfo[i];
		}
		m_lTime_GoesBy[0] = GetCurrentTime();
		m_nStep_BufferConvPlace = 520;
		break;
		
	case 520:
		// ��Ŀ�� ���� ���� ���� ��ȯ
		nRet = Func.OnGet_PickerStatus(0, CTL_YES, m_nPicker);
		if (nRet == CTL_ERROR)
		{
			m_lTime_GoesBy[1] = GetCurrentTime();
			m_lTime_GoesBy[2] = m_lTime_GoesBy[1] - m_lTime_GoesBy[0];
			if (m_lTime_GoesBy[2] < 0)
			{
				m_lTime_GoesBy[0] = GetCurrentTime();
				break;
			}
			
			if (m_lTime_GoesBy[2] < 10)
			{
				break;
			}
		}
		
		// OnGet_PickerStatus() �Լ������� [CTL_GOOD/CTL_ERROR] ��ȯ�Ǿ��� ����
		if (nRet == CTL_GOOD)
		{
			m_nStep_BufferConvPlace = 550;
		}
		else
		{
			// OnGet_PickerStatus() �Լ� �ȿ��� �˶� �ڵ� ������
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
			CTL_Lib.Alarm_Error_Occurrence( 3000, CTL_dWARNING, alarm.mstr_code );
		}
		break;

	case 550:
		// ��ü ��Ŀ ���� ���
		for(i=0; i<MAX_PICKER_; i++)
		{
			m_nPicker[TYPE_FLAG_][i] = CTL_NO;
			m_nPickerUnDn[TYPE_FLAG_][i] = CTL_NO;
			
			// ���� ���� ���θ� �˻� [����ü �÷��� ����]
			// - �Ǽ��縮 �������� ������ ������ ��Ŀ�� ����
			iExist = st_map.nXYZRbtPickerInfo[i];
			if (iExist == CTL_YES)
			{
				// Front �κи� �����ϵ��� ��.
				if (i < 2)
				{
					m_nPicker[TYPE_FLAG_][i] = CTL_YES;	// ������ ��Ŀ�� ����
					m_nPickerUnDn[TYPE_FLAG_][i] = CTL_YES;
				}
			}
		}
		if(m_nPicker[TYPE_FLAG_][0] == ACCY_YES_ || m_nPicker[TYPE_FLAG_][1] == ACCY_YES_)
		{
			m_nStep_BufferConvPlace = 600;
		}
		else
		{
			m_nStep_BufferConvPlace = 1000;
		}
		break;

	case 600:
		// �̵��� ���� ��ġ ����
		m_dTargetPos = st_motor[m_nMotY].d_pos[Y_RBT_ACC_CONV_F_BUFF_];
		
		nRet = CTL_Lib.OnSingleMove(m_nMotY, m_dTargetPos, (int)st_handler.md_run_speed);
		if (nRet == CTLBD_RET_GOOD)
		{
			m_lTime_GoesBy[0] = GetCurrentTime();
			m_nStep_BufferConvPlace = 610;	// Vacuum ��� ���
		}
		else if (nRet == CTLBD_RET_ERROR || nRet == CTLBD_RET_RETRY)
		{
			// 051011 0 0 "BARCODE_ROBOT_X_RBT_ACC_BUFFER_CONV_MOVE_ERR"
			alarm.mstr_code		= "051011";
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
			CTL_Lib.Alarm_Error_Occurrence( 3000, CTL_dWARNING, alarm.mstr_code );
		}
		break;

	case 610:
		// ��Ŀ�� ���� ���� ���� ��ȯ
		nRet = Func.OnGet_PickerStatus(0, CTL_YES, m_nPicker);
		if (nRet == CTL_ERROR)
		{
			m_lTime_GoesBy[1] = GetCurrentTime();
			m_lTime_GoesBy[2] = m_lTime_GoesBy[1] - m_lTime_GoesBy[0];
			if (m_lTime_GoesBy[2] < 0)
			{
				m_lTime_GoesBy[0] = GetCurrentTime();
				break;
			}
			
			if (m_lTime_GoesBy[2] < 10)
			{
				break;
			}
		}
		
		// OnGet_PickerStatus() �Լ������� [CTL_GOOD/CTL_ERROR] ��ȯ�Ǿ��� ����
		if (nRet == CTL_GOOD)
		{
			// [Conv->Xyz] �Ǽ��縮 ���� ��û ��ٸ�
			if (stSync.nResp_BufferAlaignConv2XYZRbt_Work == SYNC_RESP_LOADING_)
			{
				m_nStep_BufferConvPlace = 800;
			}
			else
			{
				m_nStep_BufferConvPlace = 650;
			}
		}
		else
		{
			// OnGet_PickerStatus() �Լ� �ȿ��� �˶� �ڵ� ������
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
			CTL_Lib.Alarm_Error_Occurrence( 3000, CTL_dWARNING, alarm.mstr_code );
		}
		break;

	case 650:
		// [Conv->Xyz] �Ǽ��縮 ���� ��û ��ٸ�
		if (stSync.nResp_BufferAlaignConv2XYZRbt_Work == SYNC_RESP_LOADING_)
		{
			m_nStep_BufferConvPlace = 800;
		}
		else
		{
			m_nStep_BufferConvPlace = 660;
			
		}
		break;

	case 660:
		// Conv�� �Ǽ��縮 �������� �ð��� ���̱� ���Ͽ� Ư�� ��ġ���� ������ ����
		m_dTargetPos = st_motor[m_nMotZ].d_pos[Z_RBT_ACC_BUFFER_CONV_PLACE_] -40;
		
		nRet = CTL_Lib.OnSingleMove(m_nMotZ, m_dTargetPos, (int)st_handler.md_run_speed);
		if (nRet == CTLBD_RET_GOOD)
		{
			m_nStep_BufferConvPlace = 670;
		}
		else if (nRet == CTLBD_RET_ERROR || nRet == CTLBD_RET_RETRY)
		{
			// 071005 0 0 "BARCODE_ROBOT_Z_RBT_SAFETY_MOVE_ERR"
			alarm.mstr_code		= "071005";
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
			CTL_Lib.Alarm_Error_Occurrence( 3000, CTL_dWARNING, alarm.mstr_code );
		}
		break;

	case 670:
		// [Conv->Xyz] �Ǽ��縮 ���� ��û ��ٸ�
		if (stSync.nResp_BufferAlaignConv2XYZRbt_Work == SYNC_RESP_LOADING_)
		{
			m_nStep_BufferConvPlace = 700;
		}
		break;

	case 700:
		// ��ü ��Ŀ ���� ���
		for(i=0; i<MAX_PICKER_; i++)
		{
			m_nPicker[TYPE_FLAG_][i] = CTL_NO;
			m_nPickerUnDn[TYPE_FLAG_][i] = CTL_NO;
			
			// ���� ���� ���θ� �˻� [����ü �÷��� ����]
			// - �Ǽ��縮 �������� ������ ������ ��Ŀ�� ����
			iExist = st_map.nBCR_State[i];
			if (iExist == BCR_READ_GOOD_)
			{
				// Front �κи� �����ϵ��� ��.
				if (i < 2)
				{
					m_nPicker[TYPE_FLAG_][i] = CTL_YES;	// ������ ��Ŀ�� ����
					m_nPickerUnDn[TYPE_FLAG_][i] = CTL_YES;
				}
			}
		}
		m_nStep_BufferConvPlace = 1000;
		break;

	case 1000:
		// �Ǽ��縮 �������� ��ġ�� Z�� ���� �̵�
		//nRet = CTL_Lib.OnSingleMove(m_nMotZ, st_motor[m_nMotZ].d_pos[Z_RBT_ACC_BUFFER_CONV_PLACE_], (int)st_handler.md_run_speed);
		nRet = COMI.Start_SingleMove(m_nMotZ, st_motor[m_nMotZ].d_pos[Z_RBT_ACC_BUFFER_CONV_PLACE_], (int)st_handler.md_run_speed);
		if (nRet == CTLBD_RET_GOOD)
		{
			OnSet_PickerUpDn( IO_ON, m_nPickerUnDn[TYPE_FLAG_]);
			m_nStep_BufferConvPlace = 1010;
		}
		else if (nRet == CTLBD_RET_ERROR || nRet == CTLBD_RET_RETRY)
		{
			// 071004 0 0 "BARCODE_ROBOT_Z_RBT_ACC_BUFFER_CONV_MOVE_ERR"
			alarm.mstr_code		= "071004";
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
			CTL_Lib.Alarm_Error_Occurrence( 3000, CTL_dWARNING, alarm.mstr_code );
		}
		break;

	case 1010:
		nRet = COMI.Check_SingleMove(m_nMotZ, st_motor[m_nMotZ].d_pos[Z_RBT_ACC_BUFFER_CONV_PICK_]);
		if (nRet == CTLBD_RET_GOOD)
		{
			m_nStep_BufferConvPlace = 1020;
		}
		else if (nRet == CTLBD_RET_RETRY)
		{
			m_nStep_BufferConvPlace = 1010;
		}
		else if (nRet == CTLBD_RET_ERROR || nRet == CTLBD_RET_SAFETY)
		{
			m_nStep_BufferConvPlace = 1000;
			
			// 071004 0 00 "BARCODE_ROBOT_Z_RBT_ACC_BUFFER_CONV_PICK_MOVE_ERR."
			alarm.mstr_code		= "071004";
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
			CTL_Lib.Alarm_Error_Occurrence( 3000, CTL_dWARNING, alarm.mstr_code);
		}
		break;

	case 1015:
		OnSet_PickerUpDn( IO_ON, m_nPickerUnDn[TYPE_FLAG_]);
		m_nStep_BufferConvPlace = 1020;
		break;

	case 1020:
		nRet = OnGet_PickerUpDn( IO_ON, m_nPickerUnDn[TYPE_FLAG_], m_nPickerUnDn[TYPE_SEN_]);
		if ( nRet == CTL_GOOD || nRet == CTL_ERROR )
		{
			m_nStep_BufferConvPlace = 1100;
		}
		break;
		
		//==========================================================//
		// Vacuum ��� ���
		//==========================================================//
	case 1100:
		OnSet_SolVacuum(IO_OFF, m_nPicker[TYPE_FLAG_]);	// �׸��� Vacuum [ON:��������, OFF:��������]
		OnSet_EjectOnOff( IO_ON, m_nPicker[TYPE_FLAG_]);	
		m_nStep_BufferConvPlace = 1110;
		break;
		
	case 1110:
		nRetData[0] = OnGet_SolVacuum(IO_OFF, m_nPicker[TYPE_FLAG_], m_nPicker[TYPE_SEN_]);
		nRetData[1] = OnGet_EjectOnOff( IO_ON, m_nPicker[TYPE_FLAG_] );
		// ����Ǿ��� ������ �Ϸ�ɶ����� ���
		// - ��Ŀ�� ��� �˶��� ��½�Ű�� ����
		if ( nRetData[0] == CTL_GOOD && nRetData[1] == CTL_GOOD )
		{
			OnSet_EjectOnOff( IO_OFF, m_nPicker[TYPE_FLAG_]);
			m_nStep_BufferConvPlace = 1200;
		}
		else if( nRetData[1] == CTL_ERROR || nRetData[1] == CTL_ERROR )
		{
			CTL_Lib.Alarm_Error_Occurrence( 3000, CTL_dWARNING, alarm.mstr_code );
		}
		break;

	case 1200:
		OnSet_PickerUpDn( IO_OFF, m_nPickerUnDn[TYPE_FLAG_]);
		m_nStep_BufferConvPlace = 1210;
		break;


	case 1210:
		nRet = OnGet_PickerUpDn( IO_OFF, m_nPickerUnDn[TYPE_FLAG_], m_nPickerUnDn[TYPE_SEN_]);
		if ( nRet == CTL_GOOD )
		{
			m_nStep_BufferConvPlace = 1300;
		}
		else if( nRet == CTL_ERROR )
		{
			CTL_Lib.Alarm_Error_Occurrence( 3000, CTL_dWARNING, alarm.mstr_code );
		}
		break;

	case 1300:
		// �׸����� �Ǽ��縮 ���� ���� Ȯ��
		// - �Ǽ��縮�� Conv�� �����������Ƿ� �����Ǹ� �ȵ�
		// - m_nPicker : [case 400:]���� ������
		nRet = Func.OnGet_PickerStatus(0, CTL_NO, m_nPicker);
		if (nRet == CTL_ERROR)
		{
			m_lTime_GoesBy[1] = GetCurrentTime();
			m_lTime_GoesBy[2] = m_lTime_GoesBy[1] - m_lTime_GoesBy[0];
			if (m_lTime_GoesBy[2] < 0)
			{
				m_lTime_GoesBy[0] = GetCurrentTime();
				break;
			}
			
			if (m_lTime_GoesBy[2] > 10)
			{
				// OnGet_PickerStatus() �Լ� �ȿ��� �˶� �ڵ� ������
				alarm.mn_count_mode	= 0;
				alarm.mn_type_mode	= eWARNING;
				st_work.nEqpStatus	= dWARNING;
			}
			break;
		}
		
		// ��ü ��Ŀ ���� ���
		for(i=0; i<2; i++)
		{
			// �Ǽ��縮 �������� ������ �� ��Ŀ�� ���ؼ� ����� ���縦 �������� ��� (������ �̰���)
			if (m_nPicker[TYPE_FLAG_][i] == CTL_YES && m_nPicker[TYPE_SEN_][i] == CTL_NO)
			{
				// �׸����� ����ü ���� ���� [���� ������]
				st_map.nXYZRbtPickerInfo[i]	= CTL_NO;
				
				// �� ���ܿ��� �ٽ� �ʱ�ȭ ��Ų �� �缳���ϱ� ������ �ʿ�� ������..
				m_nPicker[TYPE_FLAG_][i]	= CTL_NO;
			}
			
			if (st_basic.n_mode_device == WITHOUT_DVC_)
			{
				stWithoutData.nBufferConv[i] = st_map.nBCR_State[i];
			}
			
			st_map.nBufferCovAccyExist[i] = st_map.nBCR_State[i];
			if (st_map.nBCR_State[i] == BCR_READ_GOOD_)
			{
				if (st_work.sRbtBCR_Data[i] !="")
				{
					st_work.sBufferConvBCR_Data[i] = st_work.sRbtBCR_Data[i];
					st_work.sRbtBCR_Data[i] = "";
				}
			}
		}
		
		if (st_handler.cwnd_main != NULL)
		{
			st_handler.cwnd_main->PostMessage(WM_UPDATE_MAIN, PLACE_BUFFER_CONV_);	// ȭ�� ǥ�� ��û
		}
		m_nStep_BufferConvPlace = 2000;
		break;
		//==========================================================//
		
	case 2000:
		m_nStep_BufferConvPlace = 0;
		nFuncRet = CTL_GOOD;
		break;
	}
	return nFuncRet;
}

//==================================================================//
// [Main<-Accy] ���޿� ���Ͽ� 4�� �Ǽ��縮 �Ѳ����� ��������
//==================================================================//
int CSeq_XYZRobot::OnProc_4BufferPlace()
{
	int nFuncRet = CTL_PROCEED;	// �Լ� ���� �÷���
	
	int nRet = VAR_INIT_;
	int nRetData[4] = {0,};
	int i=0, iExist;
	int iCheckCount = 0;	// �������� �Ǽ��縮 ����
	
	Func.OnTrace_ThreadStep(23, m_nStep_BufferConvPlace);	// ������ ���� ���� TRACE
	switch(m_nStep_BufferConvPlace)
	{
	case 0:
		nRet = CTL_Lib.OnSingleMove(m_nMotZ, st_motor[m_nMotZ].d_pos[Z_RBT_SAFETY_], (int)st_handler.md_run_speed);
		if (nRet == CTLBD_RET_GOOD)
		{
			m_nStep_BufferConvPlace = 10;
		}
		else if (nRet == CTLBD_RET_ERROR || nRet == CTLBD_RET_RETRY)
		{
			// 071005 0 0 "BARCODE_ROBOT_Z_RBT_SAFETY_MOVE_ERR"
			alarm.mstr_code		= "071005";
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
			CTL_Lib.Alarm_Error_Occurrence( 3000, CTL_dWARNING, alarm.mstr_code );
		}
		break;

	case 10:
		for(i=0; i<MAX_PICKER_; i++)
		{
			m_nPicker[TYPE_FLAG_][i] = st_map.nXYZRbtPickerInfo[i];
		}
		m_lTime_GoesBy[0] = GetCurrentTime();
		m_nStep_BufferConvPlace = 20;
		break;

	case 20:
		// ��Ŀ�� ���� ���� ���� ��ȯ
		nRet = Func.OnGet_PickerStatus(0, CTL_YES, m_nPicker);
		if (nRet == CTL_ERROR)
		{
			m_lTime_GoesBy[1] = GetCurrentTime();
			m_lTime_GoesBy[2] = m_lTime_GoesBy[1] - m_lTime_GoesBy[0];
			if (m_lTime_GoesBy[2] < 0)
			{
				m_lTime_GoesBy[0] = GetCurrentTime();
				break;
			}
			
			if (m_lTime_GoesBy[2] < 10)
			{
				break;
			}
		}
		
		// OnGet_PickerStatus() �Լ������� [CTL_GOOD/CTL_ERROR] ��ȯ�Ǿ��� ����
		if (nRet == CTL_GOOD)
		{
			m_nStep_BufferConvPlace = 50;
		}
		else
		{
			// OnGet_PickerStatus() �Լ� �ȿ��� �˶� �ڵ� ������
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
			CTL_Lib.Alarm_Error_Occurrence( 3000, CTL_dWARNING, alarm.mstr_code);
		}
		break;

	case 50:
		// ��ü ��Ŀ ���� ���
		for(i=0; i<MAX_PICKER_; i++)
		{
			m_nPickerUnDn[TYPE_FLAG_][i] = CTL_NO;
			m_nPicker[TYPE_FLAG_][i] = CTL_NO;
			
			// ���� ���� ���θ� �˻� [����ü �÷��� ����]
			// - �Ǽ��縮 �������� ������ ������ ��Ŀ�� ����
			iExist = st_map.nXYZRbtPickerInfo[i];
			if (iExist == CTL_YES)
			{
				m_nPickerUnDn[TYPE_FLAG_][i] = CTL_YES;
				m_nPicker[TYPE_FLAG_][i] = CTL_YES;	// ������ ��Ŀ�� ����
			}
		}
		m_nStep_BufferConvPlace = 100;
		break;

	case 100:
		// �̵��� ���� ��ġ ����
		m_dTargetPos = st_motor[m_nMotY].d_pos[Y_RBT_ACC_CONV_R_BUFF_];
		
		nRet = CTL_Lib.OnSingleMove(m_nMotY, m_dTargetPos, (int)st_handler.md_run_speed);
		if (nRet == CTLBD_RET_GOOD)
		{
			m_lTime_GoesBy[0] = GetCurrentTime();
			m_nStep_BufferConvPlace = 110;	// Vacuum ��� ���
		}
		else if (nRet == CTLBD_RET_ERROR || nRet == CTLBD_RET_RETRY)
		{
			// 051011 0 0 "BARCODE_ROBOT_X_RBT_ACC_BUFFER_CONV_MOVE_ERR"
			alarm.mstr_code		= "051011";
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
			CTL_Lib.Alarm_Error_Occurrence( 3000, CTL_dWARNING, alarm.mstr_code);
		}
		break;

		//==========================================================//
		// Vacuum ��� ���
		//==========================================================//
	case 110:
		// ��Ŀ�� ���� ���� ���� ��ȯ
		nRet = Func.OnGet_PickerStatus(0, CTL_YES, m_nPicker);
		if (nRet == CTL_ERROR)
		{
			m_lTime_GoesBy[1] = GetCurrentTime();
			m_lTime_GoesBy[2] = m_lTime_GoesBy[1] - m_lTime_GoesBy[0];
			if (m_lTime_GoesBy[2] < 0)
			{
				m_lTime_GoesBy[0] = GetCurrentTime();
				break;
			}
			
			if (m_lTime_GoesBy[2] < 10)
			{
				break;
			}
		}
		
		// OnGet_PickerStatus() �Լ������� [CTL_GOOD/CTL_ERROR] ��ȯ�Ǿ��� ����
		if (nRet == CTL_GOOD)
		{
			// [Conv->Xyz] �Ǽ��縮 ���� ��û ��ٸ�
			if (stSync.nResp_BufferAlaignConv2XYZRbt_Work == SYNC_RESP_LOADING_)
			{
				m_nStep_BufferConvPlace = 200;
			}
			else
			{
				m_nStep_BufferConvPlace = 150;
			}
		}
		else
		{
			// OnGet_PickerStatus() �Լ� �ȿ��� �˶� �ڵ� ������
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
			CTL_Lib.Alarm_Error_Occurrence( 3000, CTL_dWARNING, alarm.mstr_code);
		}
		break;
		//==========================================================//

	case 150:
		// [Conv->Xyz] �Ǽ��縮 ���� ��û ��ٸ�
		if (stSync.nResp_BufferAlaignConv2XYZRbt_Work == SYNC_RESP_LOADING_)
		{
			m_nStep_BufferConvPlace = 200;
		}
		else
		{
			m_nStep_BufferConvPlace = 160;
			
		}
		break;

	case 160:
		// Conv�� �Ǽ��縮 �������� �ð��� ���̱� ���Ͽ� Ư�� ��ġ���� ������ ����
		//m_dTargetPos = st_motor[m_nMotZ].d_pos[Z_RBT_ACC_BUFFER_CONV_PLACE_] -40;
		//nRet = CTL_Lib.OnSingleMove(m_nMotZ, m_dTargetPos, (int)st_handler.md_run_speed);
		m_dTargetPos = st_motor[m_nMotZ].d_pos[Z_RBT_ACC_BUFFER_CONV_PLACE_] -15;
		
		nRet = COMI.Start_SingleMove(m_nMotZ, m_dTargetPos, (int)st_handler.md_run_speed);
		if (nRet == CTLBD_RET_GOOD)
		{
			OnSet_PickerUpDn(IO_ON, m_nPickerUnDn[TYPE_FLAG_]);
			m_nStep_BufferConvPlace = 170;
		}
		else if (nRet == CTLBD_RET_ERROR || nRet == CTLBD_RET_SAFETY)
		{
			// 071005 0 0 "BARCODE_ROBOT_Z_RBT_SAFETY_MOVE_ERR"
			alarm.mstr_code		= "071005";
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
			CTL_Lib.Alarm_Error_Occurrence( 3000, CTL_dWARNING, alarm.mstr_code);
		}
		break;

	case 170:
		nRet = COMI.Check_SingleMove(m_nMotZ, m_dTargetPos);
		if (nRet == CTLBD_RET_GOOD)
		{
			m_nStep_BufferConvPlace = 180;
		}
		else if( nRet == CTLBD_RET_RETRY )
		{
			m_nStep_BufferConvPlace = 170;
		}
		else if (nRet == CTLBD_RET_ERROR || nRet == CTLBD_RET_SAFETY)
		{
			// 071005 0 0 "BARCODE_ROBOT_Z_RBT_SAFETY_MOVE_ERR"
			alarm.mstr_code		= "071005";
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
			CTL_Lib.Alarm_Error_Occurrence( 3000, CTL_dWARNING, alarm.mstr_code);
			m_nStep_BufferConvPlace = 160;
		}
		break;

	case 180:
		nRet = OnGet_PickerUpDn(IO_ON, m_nPickerUnDn[TYPE_FLAG_], m_nPickerUnDn[TYPE_SEN_] );
		if( nRet == CTL_GOOD )
		{
			m_nStep_BufferConvPlace = 190;
		}
		else if( nRet == CTL_ERROR )
		{
			CTL_Lib.Alarm_Error_Occurrence( 2001, CTL_dWARNING, alarm.mstr_code);
		}
		break;

	case 190:
		// [Conv->Xyz] �Ǽ��縮 ���� ��û ��ٸ�
		if (stSync.nResp_BufferAlaignConv2XYZRbt_Work == SYNC_RESP_LOADING_)
		{
			m_nStep_BufferConvPlace = 200;
		}
		break;

	case 200:
		// �Ǽ��縮 �������� ��ġ�� Z�� ���� �̵�
		nRet = CTL_Lib.OnSingleMove(m_nMotZ, st_motor[m_nMotZ].d_pos[Z_RBT_ACC_BUFFER_CONV_PLACE_]  -15, (int)st_handler.md_run_speed);
		if (nRet == CTLBD_RET_GOOD)
		{
			m_nStep_BufferConvPlace = 300;
		}
		else if (nRet == CTLBD_RET_ERROR || nRet == CTLBD_RET_RETRY)
		{
			// 071004 0 0 "BARCODE_ROBOT_Z_RBT_ACC_BUFFER_CONV_MOVE_ERR"
			alarm.mstr_code		= "071004";
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
			CTL_Lib.Alarm_Error_Occurrence( 3000, CTL_dWARNING, alarm.mstr_code);
		}
		break;

	case 300:
		// ��ü ��Ŀ ���� ���
		for(i=0; i<MAX_PICKER_; i++)
		{
			m_nPicker[TYPE_FLAG_][i] = CTL_NO;
			m_nPickerUnDn[TYPE_FLAG_][i] = CTL_NO;
			
			// ���� ���� ���θ� �˻� [����ü �÷��� ����]
			// - �Ǽ��縮 �������� ������ ������ ��Ŀ�� ����
			iExist = st_map.nBCR_State[i];
			if (iExist == BCR_READ_GOOD_)
			{
				if (st_work.sRbtBCR_Data[i] !="")
				{
					m_nPicker[TYPE_FLAG_][i] = CTL_YES;	// ������ ��Ŀ�� ����
					m_nPickerUnDn[TYPE_FLAG_][i] = CTL_YES;
				}
			}
		}
		m_nStep_BufferConvPlace = 310;
		break;

	case 310:
		OnSet_SolVacuum(IO_OFF, m_nPicker[TYPE_FLAG_]);
		OnSet_EjectOnOff(IO_ON, m_nPicker[TYPE_FLAG_]);
		m_nStep_BufferConvPlace = 320;
		break;

	case 320:
		nRetData[0] = OnGet_SolVacuum(IO_OFF, m_nPicker[TYPE_FLAG_], m_nPicker[TYPE_SEN_]);
		nRetData[1] = OnGet_EjectOnOff(IO_ON, m_nPicker[TYPE_FLAG_]);
		// ����Ǿ��� ������ �Ϸ�ɶ����� ���
		// - ��Ŀ�� ��� �˶��� ��½�Ű�� ����
		if ( ( nRetData[0] == CTL_GOOD || nRetData[0] == CTL_ERROR ) && ( nRetData[1] == CTL_GOOD || nRetData[1] == CTL_ERROR ) )
		{
			OnSet_EjectOnOff(IO_OFF, m_nPicker[TYPE_FLAG_]);
			m_nStep_BufferConvPlace = 330;
		}
		break;

	case 330:
		nRet = OnGet_EjectOnOff(IO_OFF, m_nPicker[TYPE_FLAG_]);
		if( nRet == CTL_GOOD || nRet == CTL_ERROR )
		{
			OnSet_PickerUpDn(IO_OFF, m_nPickerUnDn[TYPE_FLAG_]);
			m_nStep_BufferConvPlace = 340;
		}
		break;

	case 340:
		nRet = OnGet_PickerUpDn( IO_OFF, m_nPickerUnDn[TYPE_FLAG_], m_nPickerUnDn[TYPE_SEN_]);
		if( nRet == CTL_GOOD || nRet == CTL_ERROR )
		{
			// ���� �ӵ��� ���̱� ���� ������
			m_lTime_GoesBy[0] = GetCurrentTime();
			m_nStep_BufferConvPlace = 350;
		}
		break;

	case 350:
		// �׸����� �Ǽ��縮 ���� ���� Ȯ��
		// - �Ǽ��縮�� Conv�� �����������Ƿ� �����Ǹ� �ȵ�
		nRet = Func.OnGet_PickerStatus(0, CTL_NO, m_nPicker);
		if (nRet == CTL_ERROR)
		{
			m_lTime_GoesBy[1] = GetCurrentTime();
			m_lTime_GoesBy[2] = m_lTime_GoesBy[1] - m_lTime_GoesBy[0];
			if (m_lTime_GoesBy[2] < 0)
			{
				m_lTime_GoesBy[0] = GetCurrentTime();
				break;
			}
			
			if (m_lTime_GoesBy[2] > 100)
			{
				// OnGet_PickerStatus() �Լ� �ȿ��� �˶� �ڵ� ������
				alarm.mn_count_mode	= 0;
				alarm.mn_type_mode	= eWARNING;
				st_work.nEqpStatus	= dWARNING;
				CTL_Lib.Alarm_Error_Occurrence( 3000, CTL_dWARNING, alarm.mstr_code);
			}
			break;
		}
		
		// ��ü ��Ŀ ���� ���
		for(i=0; i<MAX_PICKER_; i++)
		{
			// �Ǽ��縮 �������� ������ �� ��Ŀ�� ���ؼ� ����� ���縦 �������� ��� (������ �̰���)
			if (m_nPicker[TYPE_FLAG_][i] == CTL_YES && m_nPicker[TYPE_SEN_][i] == CTL_NO)
			{
				// �׸����� ����ü ���� ���� [���� ������]
				st_map.nXYZRbtPickerInfo[i]	= CTL_NO;
				
				// �� ���ܿ��� �ٽ� �ʱ�ȭ ��Ų �� �缳���ϱ� ������ �ʿ�� ������..
				m_nPicker[TYPE_FLAG_][i]	= CTL_NO;
			}
			
			if (st_basic.n_mode_device == WITHOUT_DVC_)
			{
				stWithoutData.nBufferConv[i] = st_map.nBCR_State[i];
			}
			
			st_map.nBufferCovAccyExist[i] = st_map.nBCR_State[i];
			if (st_map.nBCR_State[i] == BCR_READ_GOOD_)
			{
				if (st_work.sRbtBCR_Data[i] !="")
				{
					st_work.sBufferConvBCR_Data[i] = st_work.sRbtBCR_Data[i];
					st_work.sRbtBCR_Data[i] = "";
				}
			}
		}
		
		if (st_handler.cwnd_main != NULL)
		{
			st_handler.cwnd_main->PostMessage(WM_UPDATE_MAIN, PLACE_BUFFER_CONV_);	// ȭ�� ǥ�� ��û
		}
		m_nStep_BufferConvPlace = 1000;
		break;

	case 1000:
		m_nStep_BufferConvPlace = 0;
		nFuncRet = CTL_GOOD;
		break;

	}

	return nFuncRet;
}

//////////////////////////////////////////////////////////////////////////
// NG Accy�� ���� �� NG Buffer�� ���� �۾�
//////////////////////////////////////////////////////////////////////////
int CSeq_XYZRobot::OnProc_NGBufferPick(int nzSite)
{
	int nFuncRet = CTL_PROCEED;
	
	int nRet = VAR_INIT_;
	int nRetData[4] = {0,};
	int i = 0;
	int iCheckCount = 0;
	
	Func.OnTrace_ThreadStep(24, m_nStep_NGBufferPick);
	
	switch(m_nStep_NGBufferPick)
	{
	case 0:
		nRet = CTL_Lib.OnSingleMove(m_nMotZ, st_motor[m_nMotZ].d_pos[Z_RBT_SAFETY_], (int)st_handler.md_run_speed);
		if (nRet == CTLBD_RET_GOOD)
		{
			m_nStep_NGBufferPick = 10;
		}
		else if (nRet == CTLBD_RET_ERROR || nRet == CTLBD_RET_RETRY)
		{
			// 071005 0 0 "BARCODE_ROBOT_Z_RBT_SAFETY_MOVE_ERR"
			alarm.mstr_code		= "071005";
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
			CTL_Lib.Alarm_Error_Occurrence( 3000, CTL_dWARNING, alarm.mstr_code );
		}
		break;

	case 10:
		if (nzSite == SIDE_BOTH_)	// NG Buffer SIDE_BOTH_
		{
			m_dTargetPos = st_motor[m_nMotY].d_pos[Y_RBT_ACC_NG_BUFFER_];
		}
		else if (nzSite == SIDE_FRONT_) // NG Buffer SIDE_FRONT
		{
			m_dTargetPos = st_motor[m_nMotY].d_pos[Y_RBT_ACC_NG_BUFFER_] + st_accy.d_Y_Gripper_Size;
		}
		else if (nzSite == SIDE_REAR_)	// NG Buffer SIDE_REAR
		{
			m_dTargetPos = st_motor[m_nMotY].d_pos[Y_RBT_ACC_NG_BUFFER_] - st_accy.d_Y_Gripper_Size;
		}
		
		// nzSite���� ���� NG Buffer �̵�
		nRet = CTL_Lib.OnSingleMove(m_nMotY, m_dTargetPos, (int)st_handler.md_run_speed);
		if (nRet == CTLBD_RET_GOOD)
		{
			m_nStep_NGBufferPick = 100;
			m_nGirpSum = 0;
		}
		else if (nRet == CTLBD_RET_ERROR || nRet == CTLBD_RET_RETRY)
		{
			// 051008 0 00 "BARCODE_ROBOT_Y_RBT_ACC_NG_BUFFER_MOVE_ERR."
			alarm.mstr_code		= "051008";
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
			CTL_Lib.Alarm_Error_Occurrence( 3000, CTL_dWARNING, alarm.mstr_code );
		}
		break;

	case 100:
		// ��ȣ�� ���� ���� ���� �������� 0(���� ��), 1(���� ��), 2(���� ��), 3(���� ��)
		// NG Buffer ���� Ȯ��	
		nRetData[0] = OnCheck_NGBufferAccy(IO_ON, CHK_EXIST_, SIDE_LEFT_);
		nRetData[1] = OnCheck_NGBufferAccy(IO_ON, CHK_EXIST_, SIDE_RIGHT_);
		nRetData[2] = IO_OFF;
		nRetData[3] = IO_OFF;
		
		if (st_basic.n_mode_device == WITHOUT_DVC_)
		{
			nRetData[0] = st_map.nNGBuffer[0];
			nRetData[1] = st_map.nNGBuffer[1];
			nRetData[2] = st_map.nNGBuffer[2];
			nRetData[3] = st_map.nNGBuffer[3];
			
		}
		
		m_nRepickCount = 0;  // [����] ���� ���� �õ� Ƚ��
		
		for(i=0; i<MAX_PICKER_; i++)
		{
			m_nPicker[TYPE_FLAG_][i] = CTL_NO;	// [����] �÷��� ����
			m_nPickerUnDn[TYPE_FLAG_][i] = CTL_NO;
		}
		
		if (nzSite == SIDE_BOTH_)	// NG Buffer SIDE_BOTH_
		{
			for(i=0; i<MAX_PICKER_; i++)
			{	
				// NG Buffer���� ���� �� �κп� ���ؼ��� Vaccum ����
				if(st_map.nBCR_State[i] == BCR_READ_BAD_ && nRetData[i] == IO_ON)
				{
					m_nPicker[TYPE_FLAG_][i] = CTL_YES;	// ������ Vacuum���� ����
					m_nPickerUnDn[TYPE_FLAG_][i] = CTL_YES;
				}
			}
		}
// 		else if (nzSite == SIDE_FRONT_) // NG Buffer SIDE_FRONT
// 		{
// 			for(i = 2; i < MAX_PICKER_; i++)
// 			{
// 				// Gripper 3, 4�� ���ڵ� ������  BCR_READ_BAD_ �ϰ�� NG Buffer 1, 2���� ���� �ϱ����� ������ Vacuum���� ����
// 				if(st_map.nBCR_State[i] == BCR_READ_BAD_ && nRetData[i - 2] == IO_ON)
// 				{
// 					m_nPicker[TYPE_FLAG_][i] = CTL_YES;	// ������ Vacuum���� ����
// 				}
// 			}
// 		}
// 		else if (nzSite == SIDE_REAR_)	// NG Buffer SIDE_REAR
// 		{
// 			for(i = 0; i < 2; i++)
// 			{
// 				// Gripper 1, 2�� ���ڵ� ������  BCR_READ_BAD_ �ϰ�� NG Buffer 3, 4���� ���� �ϱ����� ������ Vacuum���� ����
// 				if(st_map.nBCR_State[i] == BCR_READ_BAD_ && nRetData[i + 2] == IO_ON)
// 				{
// 					m_nPicker[TYPE_FLAG_][i] = CTL_YES;	// ������ Vacuum���� ����
// 				}
// 			}
// 		}
		else if (nzSite == SIDE_FRONT_)	// NG Buffer SIDE_REAR
		{
			for(i = 0; i < 2; i++)
			{
				if(st_map.nBCR_State[i] == BCR_READ_BAD_ && nRetData[i] == IO_ON)
				{
					m_nPicker[TYPE_FLAG_][i] = CTL_YES;	// ������ Vacuum���� ����
				}
			}
		}		
		m_nStep_NGBufferPick= 200;
		break;

	case 200:
		nRet = COMI.Start_SingleMove(m_nMotZ, st_motor[m_nMotZ].d_pos[Z_RBT_ACC_NG_BUFFER_PICK_], (int)st_handler.md_run_speed);
		if (nRet == CTLBD_RET_GOOD)
		{
			// ���� �� ������ ���� ���̰� ���� �ð� ���̱� ���� �̰����� ���۽�Ŵ
			// - m_nPicker[FLAG_INFO] : [case 300:]���� ������
			OnSet_SolVacuum(IO_ON, m_nPicker[TYPE_FLAG_]);	// �׸��� Vacuum [ON:��������, OFF:��������]
			OnSet_PickerUpDn( IO_ON, m_nPickerUnDn[TYPE_FLAG_]);
			m_nStep_NGBufferPick= 210;
		}
		else if (nRet == CTLBD_RET_ERROR || nRet == CTLBD_RET_SAFETY)
		{
			// 071002 0 00 "BARCODE_ROBOT_Z_RBT_ACC_NG_BUFFER_PICK_MOVE_ERR."
			alarm.mstr_code		= "071002";
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
			CTL_Lib.Alarm_Error_Occurrence( 3000, CTL_dWARNING, alarm.mstr_code );
		}
		break;
		
	case 210:
		nRet = COMI.Check_SingleMove(m_nMotZ, st_motor[m_nMotZ].d_pos[Z_RBT_ACC_NG_BUFFER_PICK_]);
		if (nRet == CTLBD_RET_GOOD)
		{
			m_nStep_NGBufferPick= 220;
		}
		else if (nRet == CTLBD_RET_RETRY)
		{
			m_nStep_NGBufferPick= 210;
		}
		else if (nRet == CTLBD_RET_ERROR || nRet == CTLBD_RET_SAFETY)
		{
			m_nStep_NGBufferPick= 200;
			
			// 071002 0 00 "BARCODE_ROBOT_Z_RBT_ACC_NG_BUFFER_PICK_MOVE_ERR."
			alarm.mstr_code		= "071002";
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
			CTL_Lib.Alarm_Error_Occurrence( 3000, CTL_dWARNING, alarm.mstr_code );
		}
		break;

	case 220:
		nRetData[0] = OnGet_SolVacuum(IO_ON, m_nPicker[TYPE_FLAG_], m_nPicker[TYPE_SEN_]);
		nRetData[1] = OnGet_PickerUpDn(IO_ON, m_nPicker[TYPE_FLAG_], m_nPicker[TYPE_SEN_]);
		// ����Ǿ��� ������ �Ϸ�ɶ����� ���
		// - ��Ŀ�� ��� �˶��� ��½�Ű�� ����
		if (nRetData[0] == CTL_GOOD && nRetData[1] == CTL_GOOD)
		{
			m_nStep_NGBufferPick= 230;
		}
		else if( nRetData[0] == CTL_ERROR || nRetData[1] == CTL_ERROR )
		{
			CTL_Lib.Alarm_Error_Occurrence( 3000, CTL_dWARNING, alarm.mstr_code );
		}
		break;

	case 230:
		//nRet = CTL_Lib.OnSingleMove(m_nMotZ, st_motor[m_nMotZ].d_pos[Z_RBT_SAFETY_], (int)st_handler.md_run_speed);
		nRet = COMI.Start_SingleMove(m_nMotZ, st_motor[m_nMotZ].d_pos[Z_RBT_SAFETY_], (int)st_handler.md_run_speed);
		if (nRet == CTLBD_RET_GOOD)
		{
			OnSet_PickerUpDn( IO_OFF, m_nPickerUnDn[TYPE_FLAG_]);
			m_nStep_NGBufferPick = 240;
		}
		else if (nRet == CTLBD_RET_ERROR || nRet == CTLBD_RET_RETRY)
		{
			// 071005 0 07 "BARCODE_ROBOT_Z_RBT_SAFETY_MOVE_ERR."
			alarm.mstr_code		= "071005";
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
			CTL_Lib.Alarm_Error_Occurrence( 3000, CTL_dWARNING, alarm.mstr_code );
		}
		break;

	case 240:
		nRet = COMI.Check_SingleMove(m_nMotZ, st_motor[m_nMotZ].d_pos[Z_RBT_ACC_NG_BUFFER_PICK_]);
		if (nRet == CTLBD_RET_GOOD)
		{
			m_lTime_GoesBy[0] = GetCurrentTime();
			m_nStep_NGBufferPick= 250;
		}
		else if (nRet == CTLBD_RET_RETRY)
		{
			m_nStep_NGBufferPick= 240;
		}
		else if (nRet == CTLBD_RET_ERROR || nRet == CTLBD_RET_SAFETY)
		{
			m_nStep_NGBufferPick= 230;
			
			// 071002 0 00 "BARCODE_ROBOT_Z_RBT_ACC_NG_BUFFER_PICK_MOVE_ERR."
			alarm.mstr_code		= "071002";
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
			CTL_Lib.Alarm_Error_Occurrence( 3000, CTL_dWARNING, alarm.mstr_code );
		}
		break;

	case 245:
		OnSet_PickerUpDn( IO_OFF, m_nPickerUnDn[TYPE_FLAG_]);
		m_nStep_NGBufferPick = 250;
		break;

	case 250:
		nRet = OnGet_PickerUpDn( IO_OFF, m_nPickerUnDn[TYPE_FLAG_], m_nPickerUnDn[TYPE_SEN_] );
		if( nRet == CTL_GOOD )
		{
			m_nStep_NGBufferPick = 300;
		}
		else if( nRet == CTL_ERROR )
		{
			m_nStep_NGBufferPick = 245;
			CTL_Lib.Alarm_Error_Occurrence( 3000, CTL_dWARNING, alarm.mstr_code );
		}
		break;

	case 300:
		// �׸����� ���� ���� ��ȯ
		// - m_nPicker : [case 300:]���� ������
		nRet = Func.OnGet_PickerStatus(0, CTL_YES, m_nPicker);
		if (nRet == CTL_ERROR)
		{
			m_lTime_GoesBy[1] = GetCurrentTime();
			m_lTime_GoesBy[2] = m_lTime_GoesBy[1] - m_lTime_GoesBy[0];
			if (m_lTime_GoesBy[2] < 0)
			{
				m_lTime_GoesBy[0] = GetCurrentTime();
				break;
			}
			
			if (m_lTime_GoesBy[2] < 10)
			{
				break;
			}
		}
		// OnGet_PickerStatus() �Լ������� [CTL_GOOD/CTL_ERROR] ��ȯ�Ǿ��� ����
		if (nRet == CTL_GOOD)
		{
			// [���� ����] : ��ٷ� �ؿ� �κ� ����
			// [���� ����] : ���� �ð� ��� �Ŀ� �ؿ� �κ� ����
			
			// ��ü �׸��� ���� ���
			for(i=0; i<MAX_PICKER_; i++)
			{
				// �Ǽ��縮 ���� ���� ������ �׸��ۿ� ���ؼ� �Ǽ��縮�� Vacuum ������ ������ ���
				if (m_nPicker[TYPE_FLAG_][i] == CTL_YES && m_nPicker[TYPE_SEN_][i] == CTL_YES)
				{
					// �׸����� ����ü ���� ���� [���� ������]
					st_map.nXYZRbtPickerInfo[i]	= CTL_YES;
					m_nPicker[TYPE_FLAG_][i]	= CTL_NO;
					// ���ڵ� ����ü ���� ���� [Good ����]
					st_map.nBCR_State[i]		= BCR_READ_GOOD_;
					if (nzSite == SIDE_BOTH_)	// NG Buffer SIDE_BOTH_
					{
						st_map.nNGBuffer[i] = CTL_NO;
						st_work.sRbtBCR_Data[i] = st_work.sNGBufferBCR_Data[i];
						st_work.sNGBufferBCR_Data[i] = "";
					}
					else if (nzSite == SIDE_FRONT_) // NG Buffer SIDE_FRONT
					{
						st_map.nNGBuffer[i-2] = CTL_NO;
						st_work.sRbtBCR_Data[i] = st_work.sNGBufferBCR_Data[i-2];
						st_work.sNGBufferBCR_Data[i-2] = "";
					}
					else if (nzSite == SIDE_REAR_)	// NG Buffer SIDE_REAR
					{
						st_map.nNGBuffer[i+2] = CTL_NO;
						st_work.sRbtBCR_Data[i] = st_work.sNGBufferBCR_Data[i+2];
						st_work.sNGBufferBCR_Data[i+2] = "";
					}
				}
			}
			m_nStep_NGBufferPick= 1000;
		}
		else
		{
			m_nRepickCount++;	// ��õ� Ƚ�� ����
			if (m_nRepickCount > 2)
			{
				m_nRepickCount = 2;
			}
			m_nStep_NGBufferPick= 310;	// �Ǽ��縮 ���� ���� ��õ�
			
			if (m_nRepickCount >= 2)
			{
				m_nRepickCount = 0;	// [����] ���� ���� �õ� Ƚ��
				
				// OnGet_PickerStatus() �Լ� �ȿ��� �˶� �ڵ� ������
				alarm.mn_count_mode	= 0;
				alarm.mn_type_mode	= eWARNING;
				st_work.nEqpStatus	= dWARNING;
				CTL_Lib.Alarm_Error_Occurrence( 3000, CTL_dWARNING, alarm.mstr_code );
			}
		}
		break;
		//==========================================================//
		
	case 1000:
		m_nStep_NGBufferPick = 0;
		nFuncRet = CTL_GOOD;
		break;
	}

	return nFuncRet;
}

//////////////////////////////////////////////////////////////////////////
// NG Accy�� ���� �� NG Buffer�� ���� �۾�
//////////////////////////////////////////////////////////////////////////
int CSeq_XYZRobot::OnProc_NGBufferPlace(int nzSite)
{
	
	int nFuncRet = CTL_PROCEED;
	
	int nRet = VAR_INIT_;
	int nRetData[4] = {0,};
	int i = 0;
	int iCheckCount = 0;

	Func.OnTrace_ThreadStep(25, m_nStep_NGBufferPlace);
	
	switch(m_nStep_NGBufferPlace)
	{
	case 0:
		// ��ȣ�� ���� ���� ���� �������� 0(���� ��), 1(���� ��), 2(���� ��), 3(���� ��)
		// NG Buffer ���� Ȯ��
		nRetData[0] = OnCheck_NGBufferAccy(IO_ON, CHK_EXIST_, SIDE_LEFT_);
		nRetData[1] = OnCheck_NGBufferAccy(IO_ON, CHK_EXIST_, SIDE_RIGHT_);
		nRetData[2] = IO_OFF;
		nRetData[3] = IO_OFF;
		
		if (st_basic.n_mode_device == WITHOUT_DVC_)
		{
			for (i = 0; i < 4; i++)
			{
				nRetData[i] = st_map.nNGBuffer[i];
			}
		}
		
		for (i = 0; i < 4; i++)
		{
			if (st_map.nNGBuffer[i] != nRetData[i])
			{
				iCheckCount++;
			}
		}
		
		if (iCheckCount > 0)
		{
			// 151000 0 00 "PS1204_PS1205_PS1206_PS1207_NG_BUFFER_DATA_MISS_MATH."
			alarm.mstr_code		= "151000";
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
			CTL_Lib.Alarm_Error_Occurrence( 3000, CTL_dWARNING, alarm.mstr_code );
			break;
		}
		
		if (st_map.nNGBuffer[0] == ACCY_NO_ &&  st_map.nNGBuffer[1] == ACCY_NO_ && st_map.nNGBuffer[2] == ACCY_NO_ && st_map.nNGBuffer[3] == ACCY_NO_)
		{
			//m_nStep_NGBufferPlace = 200;
			m_nStep_NGBufferPlace = 100;
			break;
		}
		
		// Gripper i���� ���¿� NG Buffer i�� ���¸� ���Ͽ� �ߺ��Ǵ� ������ ����
		for (i = 0; i < MAX_PICKER_; i++)
		{
			// ���ڵ� ���°� Bad�� ��쿡�� Ȯ�� �ʿ� ����
			if( st_map.nNGBuffer[i] == ACCY_YES_ && st_map.nBCR_State[i] == BCR_READ_GOOD_)
			{
				// BCR ���� ȭ�� ǥ�ø� ���� �߰���
				st_map.nBCR_State[i] = BCR_READ_BAD_;
			}
		}
		
		// �ߺ� �Ǵ� Accy�� ������ ���� Bad ó�� �Ͽ����Ƿ� ���ڵ� ǥ�ø� �ٽ� ��.
		if (st_handler.cwnd_main != NULL)
		{
			st_handler.cwnd_main->PostMessage(WM_UPDATE_MAIN, READ_BCR_DATA);
		}
		
		m_nStep_NGBufferPlace = 100;
		break;
		
	case 100:
		// Gripper�� ��� �ִ� Accy ���� (SIDE_FRONT_ : 1, 2�� Gripper, SIDE_REAR_ : 3, 4�� Gripper)
		nRet = OnProc_AccyOut(st_map.nBCR_State, SIDE_FRONT_);
		
		if (nRet == CTL_GOOD)
		{
			m_nStep_NGBufferPlace = 110;
		}
		break;

	case 110:
		// Gripper�� ��� �ִ� Accy ���� (SIDE_FRONT_ : 1, 2�� Gripper, SIDE_REAR_ : 3, 4�� Gripper)
		nRet = OnProc_AccyOut(st_map.nBCR_State, SIDE_REAR_);
		
		if (nRet == CTL_GOOD)
		{
			m_nStep_NGBufferPlace = 200;
		}
		break;

	case 200:
		// ��� Accy ���ڵ� ���°� Bad �ϰ�� �� ���� �� �̱� ������ �Ϸ� Step ����
		if (st_map.nBCR_State[0] == BCR_READ_BAD_ && st_map.nBCR_State[1] == BCR_READ_BAD_ &&
			st_map.nBCR_State[2] == BCR_READ_BAD_ && st_map.nBCR_State[3] == BCR_READ_BAD_)
		{
			// kilee [2015.03.23]
			m_nStep_NGBufferPlace = 0;
			nFuncRet = CTL_GOOD;
		}
		// NG Buffer�� ������ �� �� �ֱ� ������ ���� Step ����
		else
		{
			m_nStep_NGBufferPlace = 300;
		}
		break;
	
	case 300:
		nRet = CTL_Lib.OnSingleMove(m_nMotY, st_motor[m_nMotY].d_pos[Y_RBT_ACC_NG_BUFFER_], (int)st_handler.md_run_speed);
		if (nRet == CTLBD_RET_GOOD)
		{
			m_nStep_NGBufferPlace = 310;
		}
		else if (nRet == CTLBD_RET_ERROR || nRet == CTLBD_RET_RETRY)
		{
			// 051008 0 00 "BARCODE_ROBOT_Y_RBT_ACC_NG_BUFFER_MOVE_ERR."
			alarm.mstr_code		= "051008";
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
		}
		break;
		
	case 310:
		// ���� ���ۿ��� �̹� (Vacuum Off) �������� ���̹Ƿ� �ٽ� �������� �ʵ��� ��
		// - �ص� ���������� ��� �ð���ŭ�� �ð��� �� �ҿ��
		m_nStep_NGBufferPlace = 500;	// Vacuum ��� ���
		m_lTime_GoesBy[0] = GetCurrentTime();
		break;

		//==========================================================//
		// Vacuum ��� ���
		//==========================================================//
	case 500:
		// �׸����� ���� ���� ��ȯ
		// - m_nPicker : [case 300:]���� ������
		nRet = Func.OnGet_PickerStatus(0, CTL_YES, m_nPicker);
		if (nRet == CTL_ERROR)
		{
			m_lTime_GoesBy[1] = GetCurrentTime();
			m_lTime_GoesBy[2] = m_lTime_GoesBy[1] - m_lTime_GoesBy[0];
			if (m_lTime_GoesBy[2] < 0)
			{
				m_lTime_GoesBy[0] = GetCurrentTime();
				break;
			}
			
			if (m_lTime_GoesBy[2] < 10)
			{
				break;
			}
		}
		
		// OnGet_PickerStatus() �Լ������� [CTL_GOOD/CTL_ERROR] ��ȯ�Ǿ��� ����
		if (nRet == CTL_GOOD)
		{
			m_nStep_NGBufferPlace = 510;
		}
		else
		{
			// OnGet_PickerStatus() �Լ� �ȿ��� �˶� �ڵ� ������
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
			CTL_Lib.Alarm_Error_Occurrence( 3000, CTL_dWARNING, alarm.mstr_code );
		}
		break;

	case 510:
		// ��ü ��Ŀ ���� ���
		for(i=0; i<MAX_PICKER_; i++)
		{
			m_nPicker[TYPE_FLAG_][i] = CTL_YES;	// [����] �÷��� ����
			m_nPickerUnDn[TYPE_FLAG_][i] = CTL_YES;
		}
		m_nStep_NGBufferPlace = 520;
		break;

	case 520:
		//nRet = CTL_Lib.OnSingleMove(m_nMotZ, st_motor[m_nMotZ].d_pos[Z_RBT_ACC_NG_BUFFER_PLACE_], (int)st_handler.md_run_speed);
		nRet = COMI.Start_SingleMove(m_nMotZ, st_motor[m_nMotZ].d_pos[Z_RBT_ACC_NG_BUFFER_PLACE_], (int)st_handler.md_run_speed);
		if (nRet == CTLBD_RET_GOOD)
		{
			OnSet_PickerUpDn(IO_ON, m_nPickerUnDn[TYPE_FLAG_]);
			m_nStep_NGBufferPlace = 530;
		}
		else if (nRet == CTLBD_RET_ERROR || nRet == CTLBD_RET_RETRY)
		{
			// 071001 0 00 "BARCODE_ROBOT_Z_RBT_ACC_NG_BUFFER_PLACE_MOVE_ERR."
			alarm.mstr_code		= "071001";
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
			CTL_Lib.Alarm_Error_Occurrence( 3000, CTL_dWARNING, alarm.mstr_code );
		}
		break;
		
	case 530:
		nRet = COMI.Check_SingleMove(m_nMotZ, st_motor[m_nMotZ].d_pos[Z_RBT_ACC_NG_BUFFER_PLACE_]);
		if (nRet == CTLBD_RET_GOOD)
		{
			m_nStep_NGBufferPlace = 540;
		}
		else if (nRet == CTLBD_RET_RETRY)
		{
			m_nStep_NGBufferPlace = 530;
		}
		else if (nRet == CTLBD_RET_ERROR || nRet == CTLBD_RET_SAFETY)
		{
			m_nStep_NGBufferPlace = 520;
			
			// 071001 0 00 "BARCODE_ROBOT_Z_RBT_ACC_NG_BUFFER_PLACE_MOVE_ERR."
			alarm.mstr_code		= "071001";
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
			CTL_Lib.Alarm_Error_Occurrence( 3000, CTL_dWARNING, alarm.mstr_code );
		}
		break;

	case 535:
		OnSet_PickerUpDn(IO_ON, m_nPickerUnDn[TYPE_FLAG_]);
		m_nStep_NGBufferPlace = 540;
		break;

	case 540:
		nRet = OnGet_PickerUpDn(IO_ON, m_nPickerUnDn[TYPE_FLAG_], m_nPickerUnDn[TYPE_SEN_]);
		if( nRet == CTL_GOOD )
		{
			m_nStep_NGBufferPlace = 550;
		}
		else if( nRet == CTL_ERROR )
		{
			m_nStep_NGBufferPlace = 535;
			CTL_Lib.Alarm_Error_Occurrence( 3000, CTL_dWARNING, alarm.mstr_code );
		}
		break;

	case 550:
		// - m_nPicker[FLAG_INFO] : [case 520:]���� ������
		OnSet_SolVacuum(IO_OFF, m_nPicker[TYPE_FLAG_]);	// �׸��� Vacuum [ON:��������, OFF:��������]
		m_nStep_NGBufferPlace = 560;
		break;
		
	case 560:
		nRetData[0] = OnGet_SolVacuum(IO_OFF, m_nPicker[TYPE_FLAG_], m_nPicker[TYPE_SEN_]);
		// ����Ǿ��� ������ �Ϸ�ɶ����� ���
		// - ��Ŀ�� ��� �˶��� ��½�Ű�� ����
		if (nRetData[0] == CTL_GOOD)
		{
			OnSet_EjectOnOff(IO_OFF, m_nPicker[TYPE_FLAG_]);
			m_nStep_NGBufferPlace = 570;
		}
		else if( nRetData[0] == CTL_ERROR )
		{
			CTL_Lib.Alarm_Error_Occurrence( 3000, CTL_dWARNING, alarm.mstr_code);
		}
		break;
		
	case 570:
		//nRet = CTL_Lib.OnSingleMove(m_nMotZ, st_motor[m_nMotZ].d_pos[Z_RBT_SAFETY_], (int)st_handler.md_run_speed);
		nRet = COMI.Start_SingleMove(m_nMotZ, st_motor[m_nMotZ].d_pos[Z_RBT_SAFETY_], (int)st_handler.md_run_speed);
		if (nRet == CTLBD_RET_GOOD)
		{
			OnSet_PickerUpDn(IO_OFF, m_nPickerUnDn[TYPE_FLAG_]);
			m_nStep_NGBufferPlace = 580;
		}
		else if (nRet == CTLBD_RET_ERROR || nRet == CTLBD_RET_RETRY)
		{
			// 071005 0 07 "BARCODE_ROBOT_Z_RBT_SAFETY_MOVE_ERR."
			alarm.mstr_code		= "071005";
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
		}
		break;

	case 580:
		nRet = COMI.Check_SingleMove(m_nMotZ, st_motor[m_nMotZ].d_pos[Z_RBT_SAFETY_]);
		if (nRet == CTLBD_RET_GOOD)
		{
			m_lTime_GoesBy[0] = GetCurrentTime();
			m_nStep_NGBufferPlace = 590;
		}
		else if (nRet == CTLBD_RET_RETRY)
		{
			m_nStep_NGBufferPlace = 580;
		}
		else if (nRet == CTLBD_RET_ERROR || nRet == CTLBD_RET_RETRY)
		{
			// 071005 0 07 "BARCODE_ROBOT_Z_RBT_SAFETY_MOVE_ERR."
			alarm.mstr_code		= "071005";
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
			CTL_Lib.Alarm_Error_Occurrence( 3000, CTL_dWARNING, alarm.mstr_code);
			m_nStep_NGBufferPlace = 570;
		}
		break;

	case 585:
		OnSet_PickerUpDn(IO_OFF, m_nPickerUnDn[TYPE_FLAG_]);
		m_nStep_NGBufferPlace = 590;
		break;

	case 590:
		nRet = OnGet_PickerUpDn( IO_OFF, m_nPickerUnDn[TYPE_FLAG_], m_nPickerUnDn[TYPE_SEN_]);
		if( nRet == CTL_GOOD )
		{
			m_nStep_NGBufferPlace = 600;
		}
		else if( nRet == CTL_ERROR )
		{
			m_nStep_NGBufferPlace = 585;
			CTL_Lib.Alarm_Error_Occurrence( 3000, CTL_dWARNING, alarm.mstr_code );
		}
		break;

	case 600:
		// �׸����� ���� ���� ��ȯ
		// - m_nPicker : [case 300:]���� ������
		nRet = Func.OnGet_PickerStatus(0, CTL_NO, m_nPicker);
		if (nRet == CTL_ERROR)
		{
			m_lTime_GoesBy[1] = GetCurrentTime();
			m_lTime_GoesBy[2] = m_lTime_GoesBy[1] - m_lTime_GoesBy[0];
			if (m_lTime_GoesBy[2] < 0)
			{
				m_lTime_GoesBy[0] = GetCurrentTime();
				break;
			}
			
			if (m_lTime_GoesBy[2] < 10)
			{
				break;
			}
		}
		// [���� ����] : ��ٷ� �ؿ� �κ� ����
		// [���� ����] : ���� �ð� ��� �Ŀ� �ؿ� �κ� ����
		
		// ��ü �׸��� ���� ���
		for(i=0; i<MAX_PICKER_; i++)
		{
			// �Ǽ��縮 ���� ������ ������ �׸��ۿ� ���ؼ� �Ǽ��縮�� Vacuum ������ ������ �ȵ� ���
			if (m_nPicker[TYPE_FLAG_][i] == CTL_YES && m_nPicker[TYPE_SEN_][i] == CTL_NO)
			{
				// �׸����� ����ü ���� ���� [���� ������]
				st_map.nXYZRbtPickerInfo[i]	= CTL_NO;
				m_nPicker[TYPE_FLAG_][i]	= CTL_NO;
			}
		}
		
		// OnGet_PickerStatus() �Լ������� [CTL_GOOD/CTL_ERROR] ��ȯ�Ǿ��� ����
		if (nRet == CTL_GOOD)
		{
			m_nStep_NGBufferPlace = 1000;
		}
		else
		{
			// OnGet_PickerStatus() �Լ� �ȿ��� �˶� �ڵ� ������
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
			CTL_Lib.Alarm_Error_Occurrence( 3000, CTL_dWARNING, alarm.mstr_code );
		}
		break;

	case 1000:
		for (i = 0; i <4; i++)
		{
			if (st_map.nBCR_State[i] == BCR_READ_GOOD_)
			{
				if (st_work.sRbtBCR_Data[i] != "")
				{
					st_map.nNGBuffer[i] = st_map.nBCR_State[i];
					st_work.sNGBufferBCR_Data[i] = st_work.sRbtBCR_Data[i];
					st_work.sRbtBCR_Data[i] = "";
				}
				
			}
		}
		
		if (st_handler.cwnd_main != NULL)
		{
			// NG Buffer ���� ���� ǥ�� ��û
			st_handler.cwnd_main->PostMessage(WM_UPDATE_MAIN, PLACE_NG_BUFFER_);
		}
		m_nStep_NGBufferPlace = 0;
		nFuncRet = CTL_GOOD;
		break;

	}

	return nFuncRet;
}

//==================================================================//
// �Ǽ��縮 �ڽ��� ���� �Ǵ� ���⿡�� ���� Ȯ��
//==================================================================//
int CSeq_XYZRobot::OnCheck_NGBufferAccy(int nzOnOff, int nzMode, int nzSite) 
{
	if (nzSite < SIDE_LEFT_ || nzSite > SIDE_RIGHT_)
	{
		nzSite = SIDE_BOTH_;
	}
	
	int nReadIO[2] = {0,};
	// ������ B���̶� ���� ���� Ȯ�� �� ���� ���
	nReadIO[0] = !g_ioMgr.get_in_bit(stIO.i_Chk_NGBufferAccyDetection[SIDE_LEFT_]);
	nReadIO[1] = !g_ioMgr.get_in_bit(stIO.i_Chk_NGBufferAccyDetection[SIDE_RIGHT_]);
	
	if (nzSite == SIDE_LEFT_)
	{
		nReadIO[1] = nzOnOff;
	}
	else if (nzSite == SIDE_RIGHT_)
	{
		nReadIO[0] = nzOnOff;
	}
	
	if (nzMode == CHK_EXIST_)
	{
		// ���� WithOut ������� Ȯ��
		if (st_basic.n_mode_device == WITHOUT_DVC_)
		{
			nReadIO[0] = !nzOnOff;
			nReadIO[1] = !nzOnOff;
		}
	}
	
	if (nReadIO[0] == IO_ON && nReadIO[1] == IO_ON)
	{
		return IO_ON;	// ���� ����
	}
	else	
	{
		return IO_OFF;	// ���� �̰���
	}
}

//==================================================================//
// �Ǽ��縮�� ���� �Ǵ� ���⿡�� Front Buffer ���� Ȯ��
//==================================================================//
int CSeq_XYZRobot::OnCheck_FrontAccySupply(int nzOnOff, int nzMode, int nzSite) 
{
	if (nzSite < SIDE_LEFT_ || nzSite > SIDE_RIGHT_)
	{
		nzSite = SIDE_BOTH_;
	}

	int nReadIO[2] = {0,};
	// ������ B���̶� ���� ���� Ȯ�� �� ���� ���
	nReadIO[0] = !g_ioMgr.get_in_bit(stIO.i_Chk_BufferTryAccyDetection[SIDE_FRONT_][SIDE_LEFT_]);
	nReadIO[1] = !g_ioMgr.get_in_bit(stIO.i_Chk_BufferTryAccyDetection[SIDE_FRONT_][SIDE_RIGHT_]);

	if (nzSite == SIDE_LEFT_)
	{
		// [Right] ���� �Է��� ������ ����
		nReadIO[1] = nzOnOff;
	}
	else if (nzSite == SIDE_RIGHT_)
	{
		// [Left] ���� �Է��� ������ ����
		nReadIO[0] = nzOnOff;
	}

	if (nzMode == CHK_EXIST_)
	{
		// ���� WithOut ������� Ȯ��
		// - ������� ������ �����Ƿ� �������� �ʱ� ������ ������ ������
		if (st_basic.n_mode_device == WITHOUT_DVC_)
		{
			if (st_map.nLoadingAccyBoxExist[1] == 0)
			{
				nReadIO[0] = st_map.nBufferCovAccyExist[0];
				nReadIO[1] = st_map.nBufferCovAccyExist[1];
			}
			else
			{
				nReadIO[0] = st_map.nBufferCovAccyExist[0];
				nReadIO[1] = st_map.nBufferCovAccyExist[1];
			}
		}
	}

	if (nReadIO[0] == IO_ON && nReadIO[1] == IO_ON)
	{
		return IO_ON;	// ���� ����
	}
	else	
	{
		return IO_OFF;	// ���� �̰���
	}
}

//==================================================================//
// �Ǽ��縮�� ���� �Ǵ� ���⿡�� Rear Buffer ���� Ȯ��
//==================================================================//
int CSeq_XYZRobot::OnCheck_RearAccySupply(int nzOnOff, int nzMode, int nzSite) 
{
	if (nzSite < SIDE_LEFT_ || nzSite > SIDE_RIGHT_)
	{
		nzSite = SIDE_BOTH_;
	}
	
	int nReadIO[2] = {0,};
	// ������ B���̶� ���� ���� Ȯ�� �� ���� ���
	nReadIO[0] = !g_ioMgr.get_in_bit(stIO.i_Chk_BufferTryAccyDetection[SIDE_REAR_][SIDE_LEFT_]);
	nReadIO[1] = !g_ioMgr.get_in_bit(stIO.i_Chk_BufferTryAccyDetection[SIDE_REAR_][SIDE_RIGHT_]);
	
	if (nzSite == SIDE_LEFT_)
	{
		// [Right] ���� �Է��� ������ ����
		nReadIO[1] = nzOnOff;
	}
	else if (nzSite == SIDE_RIGHT_)
	{
		// [Left] ���� �Է��� ������ ����
		nReadIO[0] = nzOnOff;
	}
	
	if (nzMode == CHK_EXIST_)
	{
		// ���� WithOut ������� Ȯ��
		// - ������� ������ �����Ƿ� �������� �ʱ� ������ ������ ������
		if (st_basic.n_mode_device == WITHOUT_DVC_)
		{
			if (st_map.nLoadingAccyBoxExist[1] == 0)
			{
				nReadIO[0] = st_map.nBufferCovAccyExist[2];
				nReadIO[1] = st_map.nBufferCovAccyExist[3];
			}
			else
			{
				nReadIO[0] = st_map.nBufferCovAccyExist[2];
				nReadIO[1] = st_map.nBufferCovAccyExist[3];
			}
		}
	}
	
	if (nReadIO[0] == IO_ON && nReadIO[1] == IO_ON)
	{
		return IO_ON;	// ���� ����
	}
	else	
	{
		return IO_OFF;	// ���� �̰���
	}
}

//==================================================================//
// �Ǽ��縮�� ���� �Ǵ� ���⿡�� Middle Buffer ���� Ȯ��
//==================================================================//
int CSeq_XYZRobot::OnCheck_MiddleAccySupply(int nzOnOff, int nzMode, int nzSite) 
{
	if (nzSite < SIDE_LEFT_ || nzSite > SIDE_RIGHT_)
	{
		nzSite = SIDE_BOTH_;
	}
	
	int nReadIO[2] = {0,};
	// ������ B���̶� ���� ���� Ȯ�� �� ���� ���
	nReadIO[0] = !g_ioMgr.get_in_bit(stIO.i_Chk_BufferTryAccyDetection[SIDE_MIDDLE][SIDE_LEFT_]);
	nReadIO[1] = !g_ioMgr.get_in_bit(stIO.i_Chk_BufferTryAccyDetection[SIDE_MIDDLE][SIDE_RIGHT_]);
	
	if (nzSite == SIDE_LEFT_)
	{
		nReadIO[0] = nzOnOff;
	}
	else if (nzSite == SIDE_RIGHT_)
	{
		nReadIO[1] = nzOnOff;
	}
	
	if (nzMode == CHK_EXIST_)
	{
		// ���� WithOut ������� Ȯ��
		// - ������� ������ �����Ƿ� �������� �ʱ� ������ ������ ������
		if (st_basic.n_mode_device == WITHOUT_DVC_)
		{
			if (st_map.nLoadingAccyBoxExist[1] == 0)
			{
				nReadIO[0] = st_map.nBufferCovAccyExist[0];
				nReadIO[1] = st_map.nBufferCovAccyExist[1];
			}
			else
			{
				nReadIO[0] = st_map.nBufferCovAccyExist[0];
				nReadIO[1] = st_map.nBufferCovAccyExist[1];
			}
		}
	}
	
	if (nReadIO[0] == IO_ON && nReadIO[1] == IO_ON)
	{
		return IO_ON;	// ���� ����
	}
	else	
	{
		return IO_OFF;	// ���� �̰���
	}
}


//==================================================================//
// �Ǽ��縮�� ���� �Ǵ� ���⿡�� Out Front Buffer ���� Ȯ��
//==================================================================//
int CSeq_XYZRobot::OnCheck_OutFrontAccySupply(int nzOnOff, int nzMode, int nzSite) 
{
	if (nzSite < SIDE_LEFT_ || nzSite > SIDE_RIGHT_)
	{
		nzSite = SIDE_BOTH_;
	}

	int nReadIO[2] = {0,};
	// ������ B���̶� ���� ���� Ȯ�� �� ���� ���
	nReadIO[0] = !g_ioMgr.get_in_bit(stIO.i_Chk_BufferTryAccyDetection[SIDE_OUT_FRONT][SIDE_LEFT_]);
	nReadIO[1] = !g_ioMgr.get_in_bit(stIO.i_Chk_BufferTryAccyDetection[SIDE_OUT_FRONT][SIDE_RIGHT_]);

	if (nzSite == SIDE_LEFT_)
	{
		// [Right] ���� �Է��� ������ ����
		nReadIO[1] = nzOnOff;
	}
	else if (nzSite == SIDE_RIGHT_)
	{
		// [Left] ���� �Է��� ������ ����
		nReadIO[0] = nzOnOff;
	}

	if (nzMode == CHK_EXIST_)
	{
		// ���� WithOut ������� Ȯ��
		// - ������� ������ �����Ƿ� �������� �ʱ� ������ ������ ������
		if (st_basic.n_mode_device == WITHOUT_DVC_)
		{
			if (st_map.nLoadingAccyBoxExist[1] == 0)
			{
				nReadIO[0] = st_map.nBufferCovAccyExist[0];
				nReadIO[1] = st_map.nBufferCovAccyExist[1];
			}
			else
			{
				nReadIO[0] = st_map.nBufferCovAccyExist[0];
				nReadIO[1] = st_map.nBufferCovAccyExist[1];
			}
		}
	}

	if (nReadIO[0] == IO_ON && nReadIO[1] == IO_ON)
	{
		return IO_ON;	// ���� ����
	}
	else	
	{
		return IO_OFF;	// ���� �̰���
	}
}

//==================================================================//
// �Ǽ��縮�� ���� �Ǵ� ���⿡�� Rear Buffer ���� Ȯ��
//==================================================================//
int CSeq_XYZRobot::OnCheck_OutRearAccySupply(int nzOnOff, int nzMode, int nzSite) 
{
	if (nzSite < SIDE_LEFT_ || nzSite > SIDE_RIGHT_)
	{
		nzSite = SIDE_BOTH_;
	}
	
	int nReadIO[2] = {0,};
	// ������ B���̶� ���� ���� Ȯ�� �� ���� ���
	nReadIO[0] = !g_ioMgr.get_in_bit(stIO.i_Chk_BufferTryAccyDetection[SIDE_OUT_REAR][SIDE_LEFT_]);
	nReadIO[1] = !g_ioMgr.get_in_bit(stIO.i_Chk_BufferTryAccyDetection[SIDE_OUT_REAR][SIDE_RIGHT_]);
	
	if (nzSite == SIDE_LEFT_)
	{
		// [Right] ���� �Է��� ������ ����
		nReadIO[1] = nzOnOff;
	}
	else if (nzSite == SIDE_RIGHT_)
	{
		// [Left] ���� �Է��� ������ ����
		nReadIO[0] = nzOnOff;
	}
	
	if (nzMode == CHK_EXIST_)
	{
		// ���� WithOut ������� Ȯ��
		// - ������� ������ �����Ƿ� �������� �ʱ� ������ ������ ������
		if (st_basic.n_mode_device == WITHOUT_DVC_)
		{
			if (st_map.nLoadingAccyBoxExist[1] == 0)
			{
				nReadIO[0] = st_map.nBufferCovAccyExist[2];
				nReadIO[1] = st_map.nBufferCovAccyExist[3];
			}
			else
			{
				nReadIO[0] = st_map.nBufferCovAccyExist[2];
				nReadIO[1] = st_map.nBufferCovAccyExist[3];
			}
		}
	}
	
	if (nReadIO[0] == IO_ON && nReadIO[1] == IO_ON)
	{
		return IO_ON;	// ���� ����
	}
	else	
	{
		return IO_OFF;	// ���� �̰���
	}
}

//==================================================================//
// Picker Pitch [ON: Close, OFF: Open]
//==================================================================//
void CSeq_XYZRobot::OnSet_PickerPitchOpenClose( int nOzOff, int nzMode)
{
	m_bPickerPitchFlag	= false;
	m_lWaitPickerPitch[0]	= GetCurrentTime();
	
	g_ioMgr.set_out_bit( stIO.o_Picker_pitch, nOzOff);
}


int CSeq_XYZRobot::OnGet_PickerPitchOPenClose( int nOzOff, int nzMode)
{
	int nFuncRet = CTL_PROCEED;

	if (nOzOff == IO_OFF)
	{
		if (m_bPickerPitchFlag == false &&
			g_ioMgr.get_in_bit(stIO.i_Picker_close_check,	IO_OFF)	== IO_OFF &&
			g_ioMgr.get_in_bit(stIO.i_Picker_open_check,	IO_ON)	== IO_ON)
		{
			m_bPickerPitchFlag		= true;
			m_lWaitPickerPitch[0]	= GetCurrentTime();
		}
		else if (m_bPickerPitchFlag == true &&
			g_ioMgr.get_in_bit(stIO.i_Picker_close_check,	IO_OFF)	== IO_OFF &&
			g_ioMgr.get_in_bit(stIO.i_Picker_open_check,	IO_ON)	== IO_ON)
		{
			m_lWaitPickerPitch[1] = GetCurrentTime();
			m_lWaitPickerPitch[2] = m_lWaitPickerPitch[1] - m_lWaitPickerPitch[0];
			
			if (m_lWaitPickerPitch[2] <= 0)
			{
				m_lWaitPickerPitch[0] = GetCurrentTime();
				return CTL_PROCEED;
			}
			if (m_lWaitPickerPitch[2] > (DWORD)st_time.nWait_Off[CYL_ACCY_ROBOT_PITCH])
			{
				return CTL_GOOD;
			}
		}
		else
		{
			m_lWaitPickerPitch[1] = GetCurrentTime();
			m_lWaitPickerPitch[2] = m_lWaitPickerPitch[1] - m_lWaitPickerPitch[0];
			
			if (m_lWaitPickerPitch[2] <= 0)
			{
				m_lWaitPickerPitch[0] = GetCurrentTime();
				return CTL_PROCEED;
			}
// 			129000 0 00 "ROBOT_PICKER_PITCH_OPEN_ERR"

			if (m_lWaitPickerPitch[2] > (DWORD)st_time.nWait_Limit[CYL_ACCY_ROBOT_PITCH])
			{
				alarm.mstr_code		= "129000";
				return CTL_ERROR;
			}
		}
	}
	else
	{
		if (m_bPickerPitchFlag == false &&
			g_ioMgr.get_in_bit(stIO.i_Picker_close_check,	IO_ON)	== IO_ON &&
			g_ioMgr.get_in_bit(stIO.i_Picker_open_check,	IO_OFF)	== IO_OFF)
		{
			m_bPickerPitchFlag		= true;
			m_lWaitPickerPitch[0]	= GetCurrentTime();
		}
		else if (m_bPickerPitchFlag == true &&
			g_ioMgr.get_in_bit(stIO.i_Picker_close_check,	IO_ON)	== IO_ON &&
			g_ioMgr.get_in_bit(stIO.i_Picker_open_check,	IO_OFF)	== IO_OFF)
		{
			m_lWaitPickerPitch[1]	= GetCurrentTime();
			m_lWaitPickerPitch[2]	= m_lWaitPickerPitch[1] - m_lWaitPickerPitch[0];
			
			if (m_lWaitPickerPitch[2] <= 0)
			{
				m_lWaitPickerPitch[0]	= GetCurrentTime();
				return CTL_PROCEED;
			}
			if (m_lWaitPickerPitch[2] > (DWORD)st_time.nWait_On[CYL_ACCY_ROBOT_PITCH])
			{
				return CTL_GOOD;
			}
		}
		else
		{
			m_lWaitPickerPitch[1]	= GetCurrentTime();
			m_lWaitPickerPitch[2]	= m_lWaitPickerPitch[1] - m_lWaitPickerPitch[0];
			
			if (m_lWaitPickerPitch[2] <= 0)
			{
				m_lWaitPickerPitch[0]	= GetCurrentTime();
				return CTL_PROCEED;
			}
			if (m_lWaitPickerPitch[2] > (DWORD)st_time.nWait_Limit[CYL_ACCY_ROBOT_PITCH])
			{
				//129001 0 00 "ROBOT_PICKER_PITCH_CLOSE_ERR"
				alarm.mstr_code		= "129001";
				return CTL_ERROR;
			}
		}
	}
	
	return RET_PROCEED;
}



//==================================================================//
// Picker UpDn [ON: DOWN, OFF:UP]
//==================================================================//

void CSeq_XYZRobot::OnSet_PickerUpDn(int nzOnOff, int nzPickerInfo[MAX_PICKER_], int nzMode)
{
	int i=0;
	int iMaster=0, iSlave=0;
	// [0:�¾�, 1:���, 2:�µ�, 3:���]
	for(i=0; i<MAX_PICKER_; i++)
	{
		iMaster	= i / 2;	// ���� ���
		iSlave	= i % 2;	// ������ ���
		
		if (nzMode == 0)
		{
			if (nzPickerInfo[i] == CTL_YES)
			{
				if (st_basic.n_mode_device != WITHOUT_DVC_)
				{
					g_ioMgr.set_out_bit(stIO.o_Picker_updn[iMaster][iSlave], nzOnOff);
				}
			}
		}
		else
		{
			if (st_basic.n_mode_device != WITHOUT_DVC_)
			{
				g_ioMgr.set_out_bit(stIO.o_Picker_updn[iMaster][iSlave], nzOnOff);
				
			}
		}
	}
	m_bflag_Picker = false;
	m_lWait_Picker[0] = GetCurrentTime();	
}

int CSeq_XYZRobot::OnGet_PickerUpDn(int nzOnOff, int nzPickerInfo[MAX_PICKER_], int nzUnDnStatus[MAX_PICKER_], int nzMode)
{
	int nFuncRet = CTL_PROCEED;
	int i=0, iSensor[MAX_PICKER_]={0,};
	int iMaster=0, iSlave=0;
	// [0:�¾�, 1:���, 2:�µ�, 3:���]
	for(i=0; i<MAX_PICKER_; i++)
	{
		iMaster	= i / 2;	// ���� ���
		iSlave	= i % 2;	// ������ ���

		iSensor[i] = g_ioMgr.get_in_bit(stIO.i_Picker_updn_chk[iMaster][iSlave]);
		if (nzPickerInfo[i] == CTL_NO)
		{
			iSensor[i] = nzOnOff;
		}
	}
		
	if (nzMode == 0)
	{
		if (st_basic.n_mode_device == WITHOUT_DVC_)
		{
			// [WithOut] ��忡���� Vacuum ���¸� �����ϰ��� �ϴ� ���·� ������ �Ҵ���
			for(i=0; i<MAX_PICKER_; i++)
			{
				iSensor[i] = nzOnOff;
			}
		}
	}

	for(i=0; i<MAX_PICKER_; i++)
	{
		nzUnDnStatus[i] = iSensor[i];
	}

	if (iSensor[0] == nzOnOff && iSensor[1] == nzOnOff && iSensor[2] == nzOnOff && iSensor[3] == nzOnOff &&
		m_bflag_Picker == false)
	{
		m_lWait_Picker[0] = GetCurrentTime();
		m_bflag_Picker = true;
	}
	else if(iSensor[0] == nzOnOff && iSensor[1] == nzOnOff && iSensor[2] == nzOnOff && iSensor[3] == nzOnOff &&
		m_bflag_Picker == true)
	{
		m_lWait_Picker[1] = GetCurrentTime();
		m_lWait_Picker[2] = m_lWait_Picker[1] - m_lWait_Picker[0];
		if( m_lWait_Picker[2] <= 0 )
		{
			m_lWait_Picker[0] = GetCurrentTime();
			return nFuncRet;
		}

		if (nzOnOff == IO_ON)
		{
			if (m_lWait_Vac[2] >= st_time.nWait_On[CYL_ACCY_ROBOT_PICKER])
			{
				nFuncRet = CTL_GOOD;
			}
		}
		else
		{
			if (m_lWait_Vac[2] >= st_time.nWait_Off[CYL_ACCY_ROBOT_PICKER])
			{
				nFuncRet = CTL_GOOD;
			}
		}
	}
	else
	{
// 		128000 0 00 "ROBOT_PICKER_FRONT_LEFT_PICK_OFF_ERR"
// 		128001 0 00 "ROBOT_PICKER_REAR_LEFT_PICK_OFF_ERR"
// 		128002 0 00 "ROBOT_PICKER_FRONT_RIGHT_PICK_OFF_ERR"
// 		128003 0 00 "ROBOT_PICKER_REAR_RIGHT_PICK_OFF_ERR"
// 		128010 0 00 "ROBOT_PICKER_FRONT_LEFT_PICK_ON_ERR"
// 		128011 0 00 "ROBOT_PICKER_REAR_LEFT_PICK_ON_ERR"
// 		128012 0 00 "ROBOT_PICKER_FRONT_RIGHT_PICK_ON_ERR"
// 		128013 0 00 "ROBOT_PICKER_REAR_RIGHT_PICK_ON_ERR"
		m_lWait_Picker[1] = GetCurrentTime();
		m_lWait_Picker[2] = m_lWait_Picker[1] - m_lWait_Picker[0];
		if( m_lWait_Picker[2] <= 0 )
		{
			m_lWait_Picker[0] = GetCurrentTime();
			return nFuncRet;
		}
		if( m_lWait_Picker[2] > st_time.nWait_On[CYL_ACCY_ROBOT_PICKER])
		{
			if( iSensor[0] != nzOnOff )
			{
				if( nzOnOff == IO_OFF )
				{
					alarm.mstr_code		= "128000";
				}
				else
				{
					alarm.mstr_code		= "128010";
				}
			}
			else if( iSensor[1] != nzOnOff )
			{
				if( nzOnOff == IO_OFF )
				{
					alarm.mstr_code		= "128001";
				}
				else
				{
					alarm.mstr_code		= "128011";
				}
			}
			else if( iSensor[2] != nzOnOff )
			{
				if( nzOnOff == IO_OFF )
				{
					alarm.mstr_code		= "128002";
				}
				else
				{
					alarm.mstr_code		= "128012";
				}
			}
			else if( iSensor[3] != nzOnOff )
			{
				if( nzOnOff == IO_OFF )
				{
					alarm.mstr_code		= "128003";
				}
				else
				{
					alarm.mstr_code		= "128013";
				}
			}
			nFuncRet = CTL_ERROR;
		}

	}
	return nFuncRet;
}


//==================================================================//
// Picker Vacuum [ON:��������, OFF:��������]
//==================================================================//
void CSeq_XYZRobot::OnSet_SolVacuum(int nzOnOff, int nzPickerInfo[MAX_PICKER_], int nzMode)
{
	int i=0;//, iSensor[MAX_PICKER_];
	int iMaster=0, iSlave=0;
	// [0:�¾�, 1:���, 2:�µ�, 3:���]
	for(i=0; i<MAX_PICKER_; i++)
	{
		iMaster	= i / 2;	// ���� ���
		iSlave	= i % 2;	// ������ ���
		
		if (nzMode == 0)
		{
			// �÷��װ� �������� ���� �ۺ��۴� Vacuum Off ��Ű�� �ȵ�
			// - �Ǽ��縮 ���� ���� ��� Vacuum ���� �÷��װ� �������� �ʾ��� ���̱� ������
			if (nzPickerInfo[i] == CTL_YES)
			{
				if (st_basic.n_mode_device != WITHOUT_DVC_)
				{
					g_ioMgr.set_out_bit(stIO.o_Vac_Rbt_VacOnOff[iMaster][iSlave], nzOnOff);
				}
			}
		}
		else
		{
			if (st_basic.n_mode_device != WITHOUT_DVC_)
			{
				g_ioMgr.set_out_bit(stIO.o_Vac_Rbt_VacOnOff[iMaster][iSlave], nzOnOff);
				
			}
		}
	}
	m_bflag_Vac = false;
	m_lWait_Vac[0] = GetCurrentTime();
}

int CSeq_XYZRobot::OnGet_SolVacuum(int nzOnOff, int nzPickerInfo[MAX_PICKER_], int nzVacStatus[MAX_PICKER_], int nzMode)
{
	int nFuncRet = CTL_PROCEED;

	int i, iSensor[MAX_PICKER_];
	int iMaster=0, iSlave=0;
	// [0:�¾�, 1:���, 2:�µ�, 3:���]
	for(i=0; i<MAX_PICKER_; i++)
	{
		iMaster	= i / 2;	// ���� ���
		iSlave	= i % 2;	// ������ ���

		iSensor[i] = g_ioMgr.get_in_bit(stIO.i_Chk_Rbt_VacOn[iMaster][iSlave]);
		// �ش� ��ġ�� Vacuum �˻�� �����ؾ� �� ���
		// - Vacuum ���¸� �����ϰ��� �ϴ� ���·� ������ ����
		if (nzPickerInfo[i] == CTL_NO)
		{
			iSensor[i] = nzOnOff;
		}
	}

	if (nzMode == 0)
	{
		if (st_basic.n_mode_device == WITHOUT_DVC_)
		{
			// [WithOut] ��忡���� Vacuum ���¸� �����ϰ��� �ϴ� ���·� ������ �Ҵ���
			for(i=0; i<MAX_PICKER_; i++)
			{
				iSensor[i] = nzOnOff;
			}
		}
	}
	// ������ Vacuum ���� ��ȯ
	// - [WithOut] ��� �Ǵ� �ش� ��Ʈ ��� ���ο� ���� ���� ����� �� ����
	for(i=0; i<MAX_PICKER_; i++)
	{
		nzVacStatus[i] = iSensor[i];
	}

	if (iSensor[0] == nzOnOff && iSensor[1] == nzOnOff && iSensor[2] == nzOnOff && iSensor[3] == nzOnOff &&
		m_bflag_Vac == false)
	{
		m_lWait_Vac[0] = GetCurrentTime();
		m_bflag_Vac = true;
	}
	else if (iSensor[0] == nzOnOff && iSensor[1] == nzOnOff && iSensor[2] == nzOnOff && iSensor[3] == nzOnOff &&
		m_bflag_Vac == true)
	{
		m_lWait_Vac[1] = GetCurrentTime();
		m_lWait_Vac[2] = m_lWait_Vac[1] - m_lWait_Vac[0];
		if (m_lWait_Vac[2] < 0)
		{
			m_lWait_Vac[0] = GetCurrentTime();
			return nFuncRet;
		}

		if (nzOnOff == IO_ON)
		{
			if (m_lWait_Vac[2] >= st_time.nWait_On[CYL_ACCY_ROBOT_VAC])
			{
				nFuncRet = CTL_GOOD;
			}
		}
		else
		{
			if (m_lWait_Vac[2] >= st_time.nWait_Off[CYL_ACCY_ROBOT_VAC])
			{
				nFuncRet = CTL_GOOD;
			}
		}
	}
	else
	{
		m_lWait_Vac[1] = GetCurrentTime();
		m_lWait_Vac[2] = m_lWait_Vac[1] - m_lWait_Vac[0];
		if (m_lWait_Vac[2] < 0)
		{
			m_lWait_Vac[0] = GetCurrentTime();
			return nFuncRet;
		}

		if (m_lWait_Vac[2] >= st_time.nWait_Limit[CYL_ACCY_ROBOT_VAC])
		{
			if (iSensor[0] != nzOnOff)
			{
				if (nzOnOff == IO_OFF)
				{
					// 115010 0 00 "PS1000_ROBOT_GRIP_1_VAC_OFF_ERR."
					alarm.mstr_code		= "115010";
				}
				else
				{
					// 115000 0 00 "PS1000_ROBOT_GRIP_1_VAC_ON_ERR."
					alarm.mstr_code		= "115000";
				}
			}
			else if (iSensor[1] != nzOnOff)
			{
				if (nzOnOff == IO_OFF)
				{
					// 115012 0 00 "PS1002_ROBOT_GRIP_3_VAC_OFF_ERR."
					alarm.mstr_code		= "115012";
				}
				else
				{
					// 115002 0 00 "PS1002_ROBOT_GRIP_3_VAC_ON_ERR."
					alarm.mstr_code		= "115002";
				}
			}
			else if (iSensor[2] != nzOnOff)
			{
				if (nzOnOff == IO_OFF)
				{
					// 115011 0 00 "PS1001_ROBOT_GRIP_2_VAC_OFF_ERR."
					alarm.mstr_code		= "115011";
				}
				else
				{
					// 115001 0 00 "PS1001_ROBOT_GRIP_2_VAC_ON_ERR."
					alarm.mstr_code		= "115001";
				}
			}
			else if (iSensor[3] != nzOnOff)
			{
				if (nzOnOff == IO_OFF)
				{
					// 115013 0 00 "PS1003_ROBOT_GRIP_4_VAC_OFF_ERR."
					alarm.mstr_code		= "115013";
				}
				else
				{
					// 115003 0 00 "PS1003_ROBOT_GRIP_4_VAC_ON_ERR."
					alarm.mstr_code		= "115003";
				}
			}
			nFuncRet = CTL_ERROR;
		}
	}
	return nFuncRet;
}

//Eject
void CSeq_XYZRobot::OnSet_EjectOnOff(int nzOnOff, int nzPickerInfo[MAX_PICKER_], int nzMode)
{
	int i=0;
	int iMaster=0, iSlave=0;
	// [0:�¾�, 1:���, 2:�µ�, 3:���]
	for(i=0; i<MAX_PICKER_; i++)
	{
		iMaster	= i / 2;	// ���� ���
		iSlave	= i % 2;	// ������ ���
		
		if (nzMode == 0)
		{
			if (nzPickerInfo[i] == CTL_YES)
			{
				if (st_basic.n_mode_device != WITHOUT_DVC_)
				{
					g_ioMgr.set_out_bit(stIO.o_Vac_Rbt_EjectOnOff[iMaster][iSlave], nzOnOff);
				}
			}
		}
		else
		{
			if (st_basic.n_mode_device != WITHOUT_DVC_)
			{
				g_ioMgr.set_out_bit(stIO.o_Vac_Rbt_EjectOnOff[iMaster][iSlave], nzOnOff);
				
			}
		}
	}
	m_bflag_Eject = false;
	m_lWait_Eject[0] = GetCurrentTime();	
}

int CSeq_XYZRobot::OnGet_EjectOnOff(int nzOnOff, int nzPickerInfo[MAX_PICKER_], int nzMode)
{
	int nFuncRet = CTL_PROCEED;

	int i, iSensor[MAX_PICKER_];
	int iMaster=0, iSlave=0;
	// [0:�¾�, 1:���, 2:�µ�, 3:���]
	for(i=0; i<MAX_PICKER_; i++)
	{
		iMaster	= i / 2;	// ���� ���
		iSlave	= i % 2;	// ������ ���

		iSensor[i] = g_ioMgr.get_out_bit(stIO.o_Vac_Rbt_EjectOnOff[iMaster][iSlave]);
		if (nzPickerInfo[i] == CTL_NO)
		{
			iSensor[i] = nzOnOff;
		}
	}

	m_lWait_Eject[1] = GetCurrentTime();
	m_lWait_Eject[2] = m_lWait_Eject[1] - m_lWait_Eject[0];
	if (m_lWait_Eject[2] < 0)
	{
		m_lWait_Eject[0] = GetCurrentTime();
		return nFuncRet;
	}

	if (nzOnOff == IO_ON)
	{
		if (m_lWait_Vac[2] >= st_time.nWait_On[CYL_ACCY_ROBOT_EJECT])
		{
			nFuncRet = CTL_GOOD;
		}
	}
	else
	{
		if (m_lWait_Vac[2] >= st_time.nWait_Off[CYL_ACCY_ROBOT_EJECT])
		{
			nFuncRet = CTL_GOOD;
		}
	}
	return nFuncRet;
}