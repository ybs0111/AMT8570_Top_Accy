// CtlBd_Library.cpp: implementation of the CCtlBd_Library class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CtlBd_Library.h"


#include "io.h"			//2011.0514  ���� ���� ���� �˻� �Լ� ȣ���ϱ� ���ؼ��� �ݵ�� �ʿ�
#include "FAS_HSSI.h"
#include "ComizoaPublic.h"
#include "FastechPublic_IO.h"
#include "CtlBd_Function.h"
#include "MyJamData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCtlBd_Library::CCtlBd_Library()
{
	// ���̸� �߰� [2014.08.27]
	int i;
	for(i=0; i<100; i++)
	{
		m_nStep_Single[i] = 0;
	}

}

CCtlBd_Library::~CCtlBd_Library()
{

}

CCtlBd_Library CTL_Lib;

//////////////////////////////////////////////////////////////////////////////
//version:0.1 (2011.1026) 
//�˶��߻� ó�����, ���� �� I/O ���� ���̺귯���� �ʱ�ȭ �� �����Ͽ� ��������ϴ� �Լ����� 
//
//���� �⺻�Լ� 
//1. #include "ComizoaPublic.h"
//2. #include "FastechPublic_IO.h"
///////////////////////////////////////////////////////////////////////////////

int CCtlBd_Library::Alarm_Error_Occurrence(int n_jampos, int n_run_status, char c_alarmcode[10])
{
	//�װ��� �˶��� �߻��ϸ� �� �����忡�� jamcode �� run status ������ �ٲ�� ��� �����ϰ� 
	//������ �ذ������� �����δ� �� �Լ��� �������� ����Ͽ� �˶����� �� ���� ���¸� ǥ���Լ� �ϳ��� 
	//����Ͽ� ��� ���� �� �˶� ������ ó���Ѵ� 
	//
	//�� �����忡 �����ִ� �˶� ó������� �� �Լ��� ��� ����Ѵ� 
   //
	int n_cur_num = -1;

	CTL_Lib.mn_jampos = n_jampos; //jam�� �߻��� ��ġ�� �����Ѵ� 
	CTL_Lib.mn_run_status = n_run_status; //����� �������¸� �����Ѵ� 
	COMI.mn_run_status = n_run_status;
	st_work.nEqpStatus = n_run_status;
	alarm.mstr_code = c_alarmcode;
	alarm.stl_cur_alarm_time = GetCurrentTime();

	strcpy(mc_alarmcode, c_alarmcode);
	MyJamData.OnSet_Info2StrctAlarm(alarm.mstr_code);	// �߻��� �˶� ������ ����ü�� �Ҵ�

	st_handler.m_tDMTBI = 0;
	st_work.mn_daily_fail_count++;
	

	if (st_handler.cwnd_list != NULL)
	{
		sprintf(st_msg.c_abnormal_msg, "[Alarm] Pos: %d %s", n_jampos, st_alarm.mstr_e_msg[n_cur_num]);
		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // ���� ���� ��� ��û
	}

	CString mstr_temp;
	mstr_temp.Format("%d", st_work.mn_daily_fail_count);
	:: WritePrivateProfileString("BasicData", "Daily_Fail_Count", LPCTSTR(mstr_temp), st_path.mstr_basic);

	return BD_GOOD;
}

int CCtlBd_Library::Alarm_Error_Occurrence( int n_jampos, int n_run_status, CString strJamCode )
{
	// 	//2013,0123
	// 	if(COMI.mn_run_status == dINIT) alarm.stl_a .stl_alarm_start_time = GetCurrentTime();
	int n_cur_num = -1;
	CTL_Lib.mn_jampos = n_jampos; //jam�� �߻��� ��ġ�� �����Ѵ� 
	CTL_Lib.mn_run_status = n_run_status; //����� �������¸� �����Ѵ� 
	COMI.mn_run_status = n_run_status;
	alarm.mstr_code = strJamCode;
	st_work.nEqpStatus = n_run_status;
	alarm.stl_cur_alarm_time = GetCurrentTime();
	
	strcpy(mc_alarmcode, (LPCSTR)strJamCode);
	
	st_work.mn_daily_fail_count++;
	MyJamData.OnSet_Info2StrctAlarm(alarm.mstr_code);	// �߻��� �˶� ������ ����ü�� �Ҵ�
	
	
	if (st_handler.cwnd_list != NULL)  // ����Ʈ �� ȭ�� ����
	{
		sprintf(st_msg.c_abnormal_msg, "[%d] [%s] [%s]", CTL_Lib.mn_jampos, alarm.mstr_code, st_alarm.mstr_cur_msg);
		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
	}
	
	CString mstr_temp;
	mstr_temp.Format("%d", st_work.mn_daily_fail_count);
	:: WritePrivateProfileString("BasicData", "Daily_Fail_Count", LPCTSTR(mstr_temp), st_path.mstr_basic);
	
	
	return BD_GOOD;
}


int CCtlBd_Library::Initialize_motor_board(int n_bd_type, CString s_filename)	//motor ���� �ʱ�ȭ�� ���
{
	int nRet = 0, i=0;

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//Motor Board Initialize 
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	if( n_bd_type == 0 ) //0:�Ľ��� ���带 ����ϴ� ����̸�, 1:Ŀ������ ���͸� ����ϴ� ��� 
	{		//���� ���庰 ����� �����Ѵ� 

		COMI.mn_totalmotboard_number = 3;  //���ͺ��尡 3�� �ִ� 
		COMI.mn_totalmotoraxis_number= 18; //������ �� ������ 18���̴�  
		COMI.mn_bd_axis_number[0] = 8;   //0�� ���ͺ���� 8��� �����̴�  
		COMI.mn_bd_axis_number[1] = 6;   //1�� ���ͺ���� 6��� �����̴� 
		COMI.mn_bd_axis_number[2] = 4;   //2�� ���ͺ���� 4��� �����̴� 
 	}
	else if( n_bd_type == 1 ) //1:Ŀ������ ���带 ����ϴ� ����̸�, 1:Ŀ������ ���͸� ����ϴ� ��� 
	{
		//1. ���� ���� �о���δ� 
		//2. ���� ���� ������ �����Ѵ� 
		nRet = COMI.Initialize_MotBoard(s_filename);			// MOTOR ���� �ʱ�ȭ ���� �˻��Ѵ�
		if (nRet == BD_ERROR)
		{
			sprintf(mc_normal_msg,"[MOTOR BOARD] �ʱ�ȭ ����.");
			LogFile_Write("c:\\AMT_LOG\\Motor\\", mc_normal_msg);
			return BD_ERROR;
		}		

		COMI.mn_totalmotboard_number = 3;  //���ͺ��尡 3�� �ִ� 
		COMI.mn_totalmotoraxis_number= 18; //������ �� ������ 18���̴�  
		COMI.mn_bd_axis_number[0] = 8;   //0�� ���ͺ���� 8��� �����̴�  
		COMI.mn_bd_axis_number[1] = 6;   //1�� ���ͺ���� 6��� �����̴� 
		COMI.mn_bd_axis_number[2] = 4;   //2�� ���ͺ���� 4��� �����̴� 

		if(COMI.mn_motorbd_init_end == BD_YES)
		{			
			for(i=0; i<COMI.mn_totalmotoraxis_number; i++)
			{//n_simul_mode => 0:�ùķ��̼� ��� ��Ȱ��, 1:�ùķ��̼� ��� Ȱ��

				if(COMI.mn_simulation_mode == 1) //���͸� �������� �����Ͽ� ���� ���� ����� ������ �ⱸ���� �̵��Ѱ�ó�� �����Ѵ� 
				{//Ȱ��
					COMI.Set_Simulation_Mode(i, 1); 
				}
				else
				{//��Ȱ�� 
					COMI.Set_Simulation_Mode(i, 0); 
				}
			}		
		}
	}

	return  BD_GOOD;
}

//==================================================================//
// ���� ���� ���� ���� Ȯ�� (���Ͷ� ����)
// - nzReqMode : ���� ���� ����
// #define REQ_HOMING_			0
// #define REQ_MOVE_START_		1
// #define REQ_MOVE_DONE_		2
// #define REQ_JOG_MOVE_		3
//==================================================================//
int CCtlBd_Library::Motor_SafetyCheck(int nzReqMode, int nzAxis, double dzTargetPos) 
{
	// ��� ���� �ʱ�ȭ ���� �˻�
	// - �ʱ�ȭ ���� �Լ� ȣ��Ǹ� ���α׷� ���� �߻�
	if (st_handler.mn_mot_brd_initial != TRUE)
	{
		return BD_ERROR;
	}

	// ���� �̵� �� ���Ͷ� ���� üũ�� ���
	// - ���� ���¸� üũ�Ͽ� ��ȯ��Ŵ
	// : �˶� ����, ���� �Ŀ� ���� ���� �۾��� �������� ����
	// : Start_SingleMove() �Լ����� �絿�� �� ������ ����
	// - ���� �̵� �� �˶� �߻��ؼ� ���� ���� ���� �Ŀ��� �ٽ� On ��Ų�ٰ� �������� �ʱ� ����
	// - Start_SingleMove() �Լ��� �ٽ� ȣ���ؾ� ���Ͱ� ���� ���۵�
	if (nzReqMode == REQ_MOVE_DONE_)
	{
		if (g_comiMgr.Get_MotPower(nzAxis) == CTL_ERROR)
		{
			return BD_ERROR;
		}

		if (COMI.Get_MotAlarmStatus(nzAxis) == CTL_ERROR)
		{
			return BD_ERROR;
		}
		return BD_GOOD;
	}

	char chJamCode[10] = {NULL};
	//==============================================================//
	// S/W ����Ʈ �������� �̵� ��û���� Ȯ��
	// - ƼĪ ȭ���� OnMotorSpeed_Data_Apply() �Լ����� ����ü�� �Ҵ��
	// : COMI.md_limit_position ����ü�� ȭ�� �������� �Է���
	//==============================================================//
	// (-) ���� ���� üũ
	if (dzTargetPos < COMI.md_limit_position[nzAxis][0])
	{
		// 000004 0 0 "SSD_DUMP_RBT_X_ALARM_ERR"
		sprintf(chJamCode, "%02d0004", nzAxis);
		Alarm_Error_Occurrence(0, CTL_dWARNING, chJamCode);

		if (st_handler.cwnd_list != NULL)
		{
			sprintf(st_msg.c_abnormal_msg, "Motor[%d] Software Limit Error_1[%4.3f] [%4.3f]", nzAxis, COMI.md_limit_position[nzAxis][0], dzTargetPos);
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
		}
		return BD_ERROR;
	}
	// (+) ���� ���� üũ
	if (dzTargetPos > COMI.md_limit_position[nzAxis][1])
	{
		// 000004 0 0 "SSD_DUMP_RBT_X_ALARM_ERR"
		sprintf(chJamCode, "%02d0004", nzAxis);
		Alarm_Error_Occurrence(1, CTL_dWARNING, chJamCode);

		if (st_handler.cwnd_list != NULL)
		{
			sprintf(st_msg.c_abnormal_msg, "Motor[%d] Software Limit Error_2[%4.3f] [%4.3f]", nzAxis, COMI.md_limit_position[nzAxis][1], dzTargetPos);
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
		}
		return BD_ERROR;
	}
	//--------------------------------------------------------------//

	//==============================================================//
	// ���� �Ŀ� �� �˶� ���� Ȯ��
	// - [POWER OFF] : [POWER ON] �۾� �õ�
	// - [���� �˶�] : [�˶� ����] �۾� �õ�
	//==============================================================//
	// [POWER OFF] �������� �˻�
	if (g_comiMgr.Get_MotPower(nzAxis) == CTL_ERROR)
	{
		// ���� [POWER ON] �۾� �õ�
		if (COMI.Set_MotPower(nzAxis, CTL_ON) == CTLBD_RET_GOOD)
		{
			if (st_handler.cwnd_list != NULL)
			{
				sprintf(st_msg.c_normal_msg, "[MotorPowerOn_1] Axis=%d, rcnt=%d", nzAxis, st_motor[nzAxis].n_retry_cnt);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
			}

			// [POWER ON] ���� ���� �� �˶� ���
			if (COMI.Get_MotPower(nzAxis) == CTL_ERROR)
			{
				//if (st_motor[nzAxis].n_retry_cnt > (MOT_RTY_CNT))
				// ���̸� ���� [2014.10.10]
				if ((COMI.mn_retry_cnt[nzAxis]) > MOT_RTY_CNT)
				{
					// 000005 0 0 "SSD_DUMP_RBT_X_POWER_OFF_ERR"
					sprintf(chJamCode, "%02d0005", nzAxis);
					Alarm_Error_Occurrence(2, CTL_dWARNING, chJamCode);
					return BD_ERROR;
				}
				else
				{
					(COMI.mn_retry_cnt[nzAxis])++;
					return BD_RETRY;
				}
			}
		}
		else
		{
			//if (st_motor[nzAxis].n_retry_cnt > MOT_RTY_CNT)
			// ���̸� ���� [2014.10.10]
			if ((COMI.mn_retry_cnt[nzAxis]) > MOT_RTY_CNT)
			{
				// 000005 0 0 "SSD_DUMP_RBT_X_POWER_OFF_ERR"
				sprintf(chJamCode, "%02d0005", nzAxis);
				Alarm_Error_Occurrence(3, CTL_dWARNING, chJamCode);
				return BD_ERROR;
			}
			else
			{
				(COMI.mn_retry_cnt[nzAxis])++;
				return BD_RETRY;
			}
		}
	}

	// [���� �˶�] �߻� �������� �˻�
	if (COMI.Get_MotAlarmStatus(nzAxis) == CTL_ERROR)
	{
		// [�˶� ����] �۾� �õ�
		if (COMI.Set_MotAlarmClear(nzAxis) == CTL_GOOD)
		{
			if (st_handler.cwnd_list != NULL)
			{
				sprintf(st_msg.c_normal_msg, "[Alarm Reset] Axis=%d, rcnt=%d", nzAxis, COMI.mn_retry_cnt[nzAxis]);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
			}

			// [�˶� ����] �۾� ���� �� �˶� ���
			if (COMI.Get_MotAlarmStatus(nzAxis) == CTL_ERROR)
			{
				if ((COMI.mn_retry_cnt[nzAxis]) > MOT_RTY_CNT)
				{
					// 000004 0 00 "LIFTER_UP_DOWN_SHIFT1_ALARM_ERR."
					// 010004 0 01 "LIFTER_UP_DOWN_SHIFT2_ALARM_ERR."
					// 020004 0 02 "LIFTER_UP_DOWN_SHIFT3_ALARM_ERR."
					// 030004 0 03 "LIFTER_UP_DOWN_SHIFT4_ALARM_ERR."
					// 050004 0 05 "BARCODE_ROBOT_X_ALARM_ERR."
					// 060004 0 06 "BARCODE_ROBOT_Y_ALARM_ERR."
					// 070004 0 07 "BARCODE_ROBOT_Z_ALARM_ERR."
					// 080004 0 08 "ROBOT_GRIP_1_ALARM_ERR."
					// 090004 0 09 "ROBOT_GRIP_2_ALARM_ERR."
					// 100003 0 10 "ROBOT_GRIP_3_HOMING_ERR."
					// 110003 0 10 "ROBOT_GRIP_4_HOMING_ERR."
					sprintf(chJamCode, "%02d0004", nzAxis);
					alarm.mstr_code = _T(chJamCode);
					sprintf(alarm.mc_code, "%02d0004", nzAxis);
					CtlBdFunc.ms_ErrMsg.Format("[Safety] Mot[%d] rcnt[%d] Get Alarm Check Error", nzAxis, COMI.mn_retry_cnt[nzAxis]);
					CtlBdFunc.Send_Error_Message(MOT_ERR_CODE, nzAxis, chJamCode, CtlBdFunc.ms_ErrMsg);
					return BD_RETRY;
				}
				else
				{
					(COMI.mn_retry_cnt[nzAxis])++;
					return BD_RETRY;
				}
			}
		}
		else
		{
			if ((COMI.mn_retry_cnt[nzAxis]) > MOT_RTY_CNT)
			{
				// 000004 0 00 "LIFTER_UP_DOWN_SHIFT1_ALARM_ERR."
				// 010004 0 01 "LIFTER_UP_DOWN_SHIFT2_ALARM_ERR."
				// 020004 0 02 "LIFTER_UP_DOWN_SHIFT3_ALARM_ERR."
				// 030004 0 03 "LIFTER_UP_DOWN_SHIFT4_ALARM_ERR."
				// 050004 0 05 "BARCODE_ROBOT_X_ALARM_ERR."
				// 060004 0 06 "BARCODE_ROBOT_Y_ALARM_ERR."
				// 070004 0 07 "BARCODE_ROBOT_Z_ALARM_ERR."
				// 080004 0 08 "ROBOT_GRIP_1_ALARM_ERR."
				// 090004 0 09 "ROBOT_GRIP_2_ALARM_ERR."
				// 100003 0 10 "ROBOT_GRIP_3_HOMING_ERR."
				// 110003 0 10 "ROBOT_GRIP_4_HOMING_ERR."
				sprintf(chJamCode, "%02d0004", nzAxis);
				alarm.mstr_code = _T(chJamCode);
				sprintf(alarm.mc_code, "%02d0004", nzAxis);
				CtlBdFunc.ms_ErrMsg.Format("[Safety] Mot[%d] rcnt[%d] Set Alarm Return Error", nzAxis, COMI.mn_retry_cnt[nzAxis]);
				CtlBdFunc.Send_Error_Message(MOT_ERR_CODE, nzAxis, chJamCode, CtlBdFunc.ms_ErrMsg);
				return BD_ERROR;
			}
			else
			{
				(COMI.mn_retry_cnt[nzAxis])++;
				return BD_RETRY;
			}
		}
	}

	int nRet = 0;
	if (nzReqMode == REQ_MOVE_START_ || nzReqMode == REQ_JOG_MOVE_)
	{
		// ���ͺ� ���Ͷ� ���� üũ
		nRet = OnCheck_MovePossible(nzReqMode, nzAxis, dzTargetPos);
		if (nRet != BD_GOOD)
		{
			return nRet;
		}
	}

	return BD_GOOD;
}

//==================================================================//
// ���ͺ� ���Ͷ� ���� üũ
//==================================================================//
int CCtlBd_Library::OnCheck_MovePossible(int nzMode, int nzAxis, double dzTargetPos) 
{
	int d_CurPos[4];
	int d_Pos[4];

	switch(nzAxis)
	{
	case M_BCRREAD_ROBOT_Y:
		/*
		d_CurPos[0] = COMI.Get_MotCurrentPos(M_BCRREAD_ROBOT_Z);
		d_Pos[0] = st_motor[M_BCRREAD_ROBOT_Z].d_pos[Z_RBT_SAFETY_] + st_motor[Z_RBT_SAFETY_].mn_allow;

		// ���̸� �ּ� ó�� [2015.03.10]
		// �Ǽ��縮�� �����̾ 2���� �������� Z���� �ణ ��� �̵��ϱ� ���Ͽ� �ּ� ó����
		if(d_CurPos[0] > d_Pos[0])
		{
			// 070000 0 07 "BARCODE_ROBOT_Z_SAFETY_NOT_EXIST_ERR."
			alarm.mstr_code		= "070000";
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
		}
		*/
		break;

	case M_LIFTER_Y:
		// Z���� ���� ��ġ�� ����
		d_CurPos[0] = COMI.Get_MotCurrentPos(M_LIFTER_Z1);
		d_CurPos[1] = COMI.Get_MotCurrentPos(M_LIFTER_Z2);
		d_CurPos[2] = COMI.Get_MotCurrentPos(M_LIFTER_Z3);
		d_CurPos[3] = COMI.Get_MotCurrentPos(M_LIFTER_Z4);
		// Z���� ���� ��ġ ���� ����
		d_Pos[0] = st_motor[M_LIFTER_Z1].d_pos[Z_ACC_LIFT_SAFETY_] + st_motor[M_LIFTER_Z1].mn_allow;
		d_Pos[1] = st_motor[M_LIFTER_Z2].d_pos[Z_ACC_LIFT_SAFETY_] + st_motor[M_LIFTER_Z2].mn_allow;
		d_Pos[2] = st_motor[M_LIFTER_Z3].d_pos[Z_ACC_LIFT_SAFETY_] + st_motor[M_LIFTER_Z3].mn_allow;
		d_Pos[3] = st_motor[M_LIFTER_Z4].d_pos[Z_ACC_LIFT_SAFETY_] + st_motor[M_LIFTER_Z4].mn_allow;

		if(d_CurPos[0] > d_Pos[0])
		{
			// 000000 0 00 "LIFTER_UP_DOWN_SHIFT1_SAFETY_NOT_EXIST_ERR."
			alarm.mstr_code		= "000000";
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
		}
		else if(d_CurPos[1] > d_Pos[1])
		{
			// 010000 0 01 "LIFTER_UP_DOWN_SHIFT2_SAFETY_NOT_EXIST_ERR."
			alarm.mstr_code		= "010000";
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
		}
		else if(d_CurPos[2] > d_Pos[2])
		{
			// 020000 0 02 "LIFTER_UP_DOWN_SHIFT3_SAFETY_NOT_EXIST_ERR."
			alarm.mstr_code		= "020000";
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
		}
		else if(d_CurPos[3] > d_Pos[3])
		{
			// 030000 0 03 "LIFTER_UP_DOWN_SHIFT4_SAFETY_NOT_EXIST_ERR."
			alarm.mstr_code		= "030000";
			alarm.mn_count_mode	= 0;
			alarm.mn_type_mode	= eWARNING;
			st_work.nEqpStatus	= dWARNING;
		}
		break;

	case M_LIFTER_Z1:
	case M_LIFTER_Z2:
	case M_LIFTER_Z3:
	case M_LIFTER_Z4:
		// X���� ���� ��ġ�� ����
		d_CurPos[0] = COMI.Get_MotCurrentPos(M_LIFTER_Y);
		// X���� �ּ� �۾� ��ġ ���� ����
		d_Pos[0] = st_motor[M_LIFTER_Y].d_pos[Y_ACC_FRONT_] - st_motor[M_LIFTER_Y].mn_allow;
		
		if(d_CurPos[0] < d_Pos[0])
		{
		 	// 040000 0 00 "LIFTER_Y_SAFETY_NOT_EXIST_ERR."
		 	alarm.mstr_code		= "040000";
		 	alarm.mn_count_mode	= 0;
		 	alarm.mn_type_mode	= eWARNING;
		 	st_work.nEqpStatus	= dWARNING;
		}
		break;
	}

	st_motor[nzAxis].mn_retry_time_flag = CTL_NO;//2011.0201 �߰� 

	return BD_GOOD;
}

BOOL CCtlBd_Library::CreateFolder(LPCTSTR szPath)
{
	int nRet = 0;
	CString strTemp;
	CString strPath = szPath;
	
    do{
        nRet = strPath.Find('\\' , nRet + 1);
        if (nRet > 0 )
        {
            strTemp = strPath.Left(nRet+1);
			
            if (_access(strTemp.operator LPCTSTR(), 00) == -1)
			{
				CreateDirectory(strTemp , NULL);
			}
        }
    }while (nRet != -1);
	
    return nRet;
}

int CCtlBd_Library::FileSizeCheck(char * cp_filename, int n_size, int n_check)
{
	CFileFind finder;
	long nLen=0, nflag=0;
	BOOL bContinue;
	
	if(bContinue = finder.FindFile(cp_filename))
	{	
		if (n_check == BD_YES)			// Size�� üũ�ϴ°��̸�...
		{
			finder.FindFile(cp_filename);
			finder.FindNextFile();
			finder.GetFileName();
			nLen = finder.GetLength();
			if(nLen > n_size)
			{
				nflag = 1;
			}
			finder.Close();	
		}
		else						// Size�� Ȯ���ϴ°��̸�...
		{
			finder.FindFile(cp_filename);
			finder.FindNextFile();
			finder.GetFileName();
			nLen = finder.GetLength();
			
			nflag = nLen;
			
			finder.Close();	
		}
	}
	
	return nflag;
}

int CCtlBd_Library::LogFile_Write(char * cp_dir_path, char * cp_msg)
{
//2011.0417	sing.Lock(); //2011.0417 

	CString mstr_cur_year, mstr_cur_month, mstr_cur_day, str_display_time; // ���� ��, ��, �� ���� ���������� ��ȯ�Ͽ� ������ ���� 
	int mn_cur_year, mn_cur_month, mn_cur_day, mn_cur_hour; // ���� ��, ��, �� ���� ���� ���� 
	CString mstr_file_name, mstr_dir_path, str_msg;		// ���������� ������ ���� �̸� ���� ���� 
	CString mstr_create_file;	// �˶� ���� ������ ���Ͽ� ���� [����]+[���ϸ�]+[Ȯ����] ���� ���� 
	CString mstr_list_name, mstr_temp_data;  // �� �κк� �˶� �߻� Ƚ�� ���� ������ �α� ���� ���� ���� ���� 
	CString mstr_content;		// �� ���ο� ����� ���ڿ� ���� ���� ���� 
	COleDateTime time_cur;		// �˻��� �ð� ���� ���� ���� 
	CTime m_time_current;		// ������ ����� ���� �ð� ���� ���� ����
	char chr_buf[20]={0,};
	int mn_existence, nlength;			// ���� ���� ���� ���� �÷��� 
	char cfileName[256]={0,};			// �˻��� ���� ���� ���� �Լ� 
	FILE  *fp ;					// ���Ͽ� ���� ������ ���� ���� 

	CString BackupName;
	CString mstr_cur_hour, mstr_cur_min, mstr_cur_sec;
	int Ret=0;

	nlength = strlen(cp_msg);
	if(nlength <= 0) //������ �޼����� ���� 
	{
		return BD_ERROR;
	}
	str_msg = cp_msg;
 
	// **************************************************************************
	// ���� �̸����� ����� ��¥ ������ ��´�                                   
	// **************************************************************************
	time_cur = COleDateTime::GetCurrentTime();  // ���� �ð� ������ ��´�. 

	m_time_current = CTime::GetCurrentTime() ;  // ������ ������ ���� �ð� ���� ��´�. 

	mn_cur_year = time_cur.GetYear();  

	mn_cur_month = time_cur.GetMonth();  

    mn_cur_day = time_cur.GetDay();  

	mn_cur_hour = time_cur.GetHour();
	// **************************************************************************

	// **************************************************************************
	// ��¥ ������ ���������� ��ȯ�Ͽ� ������ �����Ѵ�                           
	// **************************************************************************
	mstr_cur_year = LPCTSTR(_itoa( mn_cur_year, chr_buf, 10 ));  
	mstr_cur_month.Format("%02d", mn_cur_month);
	mstr_cur_day.Format("%02d", mn_cur_day);

	// **************************************************************************
	// ���� �ð� ���� ��´�                                                     
	// **************************************************************************
	mn_cur_hour = time_cur.GetHour();				// ���� �ð� ������ �����Ѵ�. 
	mstr_cur_hour.Format("%d",time_cur.GetHour());	// ���� �ð� ������ �����Ѵ�.
	mstr_cur_min.Format("%d",time_cur.GetMinute());	// ���� �� ������ �����Ѵ�. 
	mstr_cur_sec.Format("%d",time_cur.GetSecond());	// ���� �� ������ �����Ѵ�. 
	str_display_time = m_time_current.Format("%c");	// ����Ʈ ���Ͽ� ����� Ÿ��Ʋ �ð� ���� ���� 
	// **************************************************************************


	mstr_dir_path = cp_dir_path;
	mstr_file_name = mstr_cur_year;
	mstr_file_name += mstr_cur_month; 
	mstr_file_name += mstr_cur_day; 
	mstr_create_file = mstr_dir_path + mstr_file_name;

	mstr_create_file += ".TXT";

	sprintf(cfileName, "%s", mstr_create_file);
	mn_existence = access(cfileName,0) ;	
	if (mn_existence == -1)
	{//�ش� ������ �������� �ʴ� �����̴� 
		CreateFolder(cp_dir_path); 
	}
	else 
	{//�ش� ������ �̹� �����Ѵ�.
		//���� 
	}
	
	Ret = FileSizeCheck(cfileName, 1048576, BD_YES); //size and rename	
	if(Ret == 1) //file size over
	{
		BackupName = mstr_create_file + mstr_cur_hour + mstr_cur_min +  mstr_cur_sec + ".bak" ;
		rename(mstr_create_file, BackupName);
	}

	if ((fp = fopen(mstr_create_file,"a+")) == NULL)  
	{
		//AfxMessageBox("File open failure!..");
		sprintf(mc_normal_msg,"LogFile_Write a+ Error"); 
		
 		return BD_ERROR;
	}
	// **************************************************************************
	
	mstr_content += str_display_time;
	mstr_content += " : " + str_msg;

	fprintf(fp,"%s\n",mstr_content) ;
	if (ferror(fp))  
	{
		sprintf(mc_normal_msg,"LogFile_Write ferror return error");

		clearerr(fp); //2011.0418 

		fclose(fp) ;  //2011.0417  ���� ����

//2011.0417		sing.Unlock(); //2011.0417 �߰� 
		return BD_ERROR;
	}
	// **************************************************************************
	fclose(fp);  // ������ �����Ѵ�.

//2011.0417	sing.Unlock(); //2011.0417 �߰� 
	return BD_GOOD;
}

//==================================================================//
// SingleMove ���� ���� �ʱ�ȭ
// - ���� �� ������ ��� ������ ó������ �ٽ� �����ϱ� ���� �ʿ���
//==================================================================//
void CCtlBd_Library::OnReset_SingleMoveStep(int nzMotorNum) 
{
	if (m_nStep_Single[nzMotorNum] != 0)
	{
		m_nStep_Single[nzMotorNum] = 0;
	}
}

//==================================================================//
// �Էµ� ��ġ�� ���� �̵�
// - [���� ��ġ == �̵��� ��ġ] �׳� ���ϵ�
//==================================================================//
int CCtlBd_Library::OnSingleMove(int nzMotorNum, double dzMovePos, int nzSpeedRate) 
{
	int nFuncRet = CTLBD_RET_PROCEED;
	
	int nRet;
	// ���� �ະ�� ������ ����
	switch(m_nStep_Single[nzMotorNum])
	{
	case 0:
		nRet = COMI.Check_MotPosRange(nzMotorNum, dzMovePos, COMI.mn_allow_value[nzMotorNum]);
		// ���̸� ���� [2014.08.26]
		//nRet = COMI.Check_MotPosRange(nzMotorNum, dzMovePos, 0);
		if (nRet == CTLBD_RET_GOOD)  // �̵��� ��ġ�� �̹� ������
		{
			m_nStep_Single[nzMotorNum] = 0;
			nFuncRet = CTLBD_RET_GOOD;
			break;
		}

		// ��õ� Ƚ�� �ʱ�ȭ
		mn_mot_retry_cnt[nzMotorNum] = 0;

		m_nStep_Single[nzMotorNum] = 100;
		break;

	case 100:
		// �Էµ� ��ġ�� ���� �̵���Ŵ
		nRet = COMI.Start_SingleMove(nzMotorNum, dzMovePos, nzSpeedRate);
		if (nRet == CTLBD_RET_GOOD)
		{
			m_nStep_Single[nzMotorNum] = 200;
		}
		else if (nRet == CTLBD_RET_ERROR || nRet == CTLBD_RET_SAFETY)
		{
			// ��õ� Ƚ�� ������Ŵ
			mn_mot_retry_cnt[nzMotorNum]++;

			if (mn_mot_retry_cnt[nzMotorNum] > MAX_RETRY_)
			{
				m_nStep_Single[nzMotorNum] = 0;
				nFuncRet = nRet;
			}
			else
			{
				m_nStep_Single[nzMotorNum] = 100;
			}
		}
		else if (nRet == CTLBD_RET_RETRY)
		{
			// ��õ� Ƚ�� ������Ŵ
			//mn_mot_retry_cnt[nzMotorNum]++;
			
			if (mn_mot_retry_cnt[nzMotorNum] > MAX_RETRY_)
			{
				m_nStep_Single[nzMotorNum] = 0;
				nFuncRet = CTLBD_RET_ERROR;
			}
			else
			{
				m_nStep_Single[nzMotorNum] = 100;
			}
		}
		break;

	case 200:
		nRet = COMI.Check_SingleMove(nzMotorNum, dzMovePos);  // �̵� �Ϸ� Ȯ��
		if (nRet == CTLBD_RET_GOOD)  // ���� �Ϸ�
		{
			m_nStep_Single[nzMotorNum] = 0;
			nFuncRet = CTLBD_RET_GOOD;
		}
		else if (nRet == CTLBD_RET_ERROR || nRet == CTLBD_RET_SAFETY)
		{
			// ��õ� Ƚ�� ������Ŵ
			mn_mot_retry_cnt[nzMotorNum]++;
			
			if (mn_mot_retry_cnt[nzMotorNum] > MAX_RETRY_)
			{
				m_nStep_Single[nzMotorNum] = 0;
				nFuncRet = nRet;
			}
			else
			{
				m_nStep_Single[nzMotorNum] = 100;
			}
		}
		else if (nRet == CTLBD_RET_RETRY)  // ���� ��õ�
		{
			// ��õ� Ƚ�� ������Ŵ
			//mn_mot_retry_cnt[nzMotorNum]++;
			
			// ���ѷ����� �������� ������ Ƚ���� �����ؼ� ����ϸ� ���� ����
			if (mn_mot_retry_cnt[nzMotorNum] > MAX_RETRY_)
			{
				m_nStep_Single[nzMotorNum] = 0;
				nFuncRet = CTLBD_RET_ERROR;
			}
			else
			{
				m_nStep_Single[nzMotorNum] = 100;
			}
		}
		break;
	}
	return nFuncRet;
}

int CCtlBd_Library::Linear_Move(int n_LinearMapIndex, long l_AxisCnt, long *lp_AxisNum, double *dp_PosList, double *dp_SpdRatio)
{
	int nFuncRet = BD_PROCEED;
	int nRet[4] = {0,}, nCnt =0, i=0;
	
	switch(mn_linear_motmove_step[n_LinearMapIndex])
	{
	case 0:

		nCnt = 0;
		st_linearmot[n_LinearMapIndex].l_AxisCnt = l_AxisCnt;
		for(i=0; i<l_AxisCnt; i++)
		{		
			st_linearmot[n_LinearMapIndex].lp_AxisNum[i] = lp_AxisNum[i]; 
			st_linearmot[n_LinearMapIndex].dp_PosList[i] = dp_PosList[i];

			nRet[i] = COMI.Check_MotPosRange(lp_AxisNum[i], dp_PosList[i], COMI.mn_allow_value[lp_AxisNum[i]]);
			if(nRet[i] == BD_GOOD) nCnt++;
		}

		if(nCnt == l_AxisCnt)
		{//�̹� ��� ������ ���� ��ġ�� �̵��Ϸ��� ��ġ�� ������ �ȿ� ������ ������ �ʿ䰡 ������ ������.
			nFuncRet = BD_GOOD;
			break;
		}
		
		mn_linear_retry_cnt[n_LinearMapIndex] = 0;
		mn_linear_max_retry_cnt = 3; //�ִ� 3ȸ ��Ʈ���� ������ �����Ѵ� 
		mn_linear_motmove_step[n_LinearMapIndex] = 100;
		break;
		
	case 100:
		nRet[0] = COMI.Start_LinearMove(n_LinearMapIndex, st_linearmot[n_LinearMapIndex].dp_PosList, dp_SpdRatio);
		if (nRet[0] == BD_GOOD)
		{
			mn_linear_motmove_step[n_LinearMapIndex] = 200;
		}
		else if (nRet[0] == BD_ERROR || nRet[0] == BD_SAFETY)
		{
			mn_linear_retry_cnt[n_LinearMapIndex]++;
			
			if(mn_linear_retry_cnt[n_LinearMapIndex] > mn_linear_max_retry_cnt) 
			{
				mn_linear_motmove_step[n_LinearMapIndex] = 0;
				nFuncRet = nRet[0];
			}
			else 
			{				
				mn_linear_motmove_step[n_LinearMapIndex] = 100;
			}
		}
		else if (nRet[0] == BD_RETRY)  // ���� ��õ�
		{
			if(mn_linear_retry_cnt[n_LinearMapIndex] > mn_linear_max_retry_cnt) 
			{
				mn_linear_motmove_step[n_LinearMapIndex] = 0;
				nFuncRet = BD_ERROR;
			}
			else 
			{				
				mn_linear_motmove_step[n_LinearMapIndex] = 100;
			}
		}
		break;
		
	case 200:
		nRet[0] = COMI.Check_LinearMove(n_LinearMapIndex, st_linearmot[n_LinearMapIndex].dp_PosList) ;
		if (nRet[0] == BD_GOOD)  // ���� �Ϸ�
		{
			mn_linear_motmove_step[n_LinearMapIndex] = 0;
			nFuncRet = nRet[0];
		}
		else if (nRet[0] == BD_ERROR || nRet[0] == BD_SAFETY)
		{
			mn_linear_retry_cnt[n_LinearMapIndex]++;
			
			if(mn_linear_retry_cnt[n_LinearMapIndex] > mn_linear_max_retry_cnt) 
			{
				mn_linear_motmove_step[n_LinearMapIndex] = 0;
				nFuncRet = nRet[0];
			}
			else 
			{				
				mn_linear_motmove_step[n_LinearMapIndex] = 100;
			}
		}
		else if (nRet[0] == BD_RETRY)  // ���� ��õ�
		{
			// ���ѷ����� �������� ������ Ƚ���� �����ؼ� ����ϸ� ���� ����
			mn_linear_retry_cnt[n_LinearMapIndex]++;
			
			if(mn_linear_retry_cnt[n_LinearMapIndex] > mn_linear_max_retry_cnt) 
			{
				mn_linear_motmove_step[n_LinearMapIndex] = 0;
				nFuncRet = BD_ERROR;
			}
			else 
			{				
				mn_linear_motmove_step[n_LinearMapIndex] = 100;
			}
		}	
		break;
	}
	return nFuncRet;
}
