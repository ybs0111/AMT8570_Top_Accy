// CtlBd_Library.cpp: implementation of the CCtlBd_Library class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CtlBd_Library.h"


#include "io.h"			//2011.0514  파일 존재 유무 검사 함수 호출하기 위해서는 반드시 필요
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
	// 규이리 추가 [2014.08.27]
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
//알람발생 처리방법, 모터 및 I/O 관련 라이브러리를 초기화 및 응용하여 실제사용하는 함수정리 
//
//참고 기본함수 
//1. #include "ComizoaPublic.h"
//2. #include "FastechPublic_IO.h"
///////////////////////////////////////////////////////////////////////////////

int CCtlBd_Library::Alarm_Error_Occurrence(int n_jampos, int n_run_status, char c_alarmcode[10])
{
	//그간은 알람이 발생하면 각 쓰레드에서 jamcode 및 run status 정보를 바뀌어 장비를 정지하고 
	//문제를 해결했으나 앞으로는 이 함수를 모든곳에서 사용하여 알람정보 및 가동 상태를 표준함수 하나를 
	//사용하여 장비 가동 및 알람 문제를 처리한다 
	//
	//각 쓰레드에 각각있던 알람 처리방법은 이 함수를 모두 사용한다 
   //
	int n_cur_num = -1;

	CTL_Lib.mn_jampos = n_jampos; //jam이 발생한 위치를 저장한다 
	CTL_Lib.mn_run_status = n_run_status; //장비의 가동상태를 변경한다 
	COMI.mn_run_status = n_run_status;
	st_work.nEqpStatus = n_run_status;
	alarm.mstr_code = c_alarmcode;
	alarm.stl_cur_alarm_time = GetCurrentTime();

	strcpy(mc_alarmcode, c_alarmcode);
	MyJamData.OnSet_Info2StrctAlarm(alarm.mstr_code);	// 발생한 알람 정보를 구조체에 할당

	st_handler.m_tDMTBI = 0;
	st_work.mn_daily_fail_count++;
	

	if (st_handler.cwnd_list != NULL)
	{
		sprintf(st_msg.c_abnormal_msg, "[Alarm] Pos: %d %s", n_jampos, st_alarm.mstr_e_msg[n_cur_num]);
		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
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
	CTL_Lib.mn_jampos = n_jampos; //jam이 발생한 위치를 저장한다 
	CTL_Lib.mn_run_status = n_run_status; //장비의 가동상태를 변경한다 
	COMI.mn_run_status = n_run_status;
	alarm.mstr_code = strJamCode;
	st_work.nEqpStatus = n_run_status;
	alarm.stl_cur_alarm_time = GetCurrentTime();
	
	strcpy(mc_alarmcode, (LPCSTR)strJamCode);
	
	st_work.mn_daily_fail_count++;
	MyJamData.OnSet_Info2StrctAlarm(alarm.mstr_code);	// 발생한 알람 정보를 구조체에 할당
	
	
	if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
	{
		sprintf(st_msg.c_abnormal_msg, "[%d] [%s] [%s]", CTL_Lib.mn_jampos, alarm.mstr_code, st_alarm.mstr_cur_msg);
		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
	}
	
	CString mstr_temp;
	mstr_temp.Format("%d", st_work.mn_daily_fail_count);
	:: WritePrivateProfileString("BasicData", "Daily_Fail_Count", LPCTSTR(mstr_temp), st_path.mstr_basic);
	
	
	return BD_GOOD;
}


int CCtlBd_Library::Initialize_motor_board(int n_bd_type, CString s_filename)	//motor 보드 초기화시 사용
{
	int nRet = 0, i=0;

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//Motor Board Initialize 
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	if( n_bd_type == 0 ) //0:파스텍 보드를 사용하는 장비이면, 1:커미조아 모터를 사용하는 장비 
	{		//모터 보드별 축수를 정의한다 

		COMI.mn_totalmotboard_number = 3;  //모터보드가 3개 있다 
		COMI.mn_totalmotoraxis_number= 18; //모터의 총 수량은 18개이다  
		COMI.mn_bd_axis_number[0] = 8;   //0번 모터보드는 8축용 보드이다  
		COMI.mn_bd_axis_number[1] = 6;   //1번 모터보드는 6축용 보드이다 
		COMI.mn_bd_axis_number[2] = 4;   //2번 모터보드는 4축용 보드이다 
 	}
	else if( n_bd_type == 1 ) //1:커미조아 보드를 사용하는 장비이면, 1:커미조아 모터를 사용하는 장비 
	{
		//1. 보드 정보 읽어들인다 
		//2. 각종 셋팅 정보를 적용한다 
		nRet = COMI.Initialize_MotBoard(s_filename);			// MOTOR 보드 초기화 여부 검사한다
		if (nRet == BD_ERROR)
		{
			sprintf(mc_normal_msg,"[MOTOR BOARD] 초기화 에러.");
			LogFile_Write("c:\\AMT_LOG\\Motor\\", mc_normal_msg);
			return BD_ERROR;
		}		

		COMI.mn_totalmotboard_number = 3;  //모터보드가 3개 있다 
		COMI.mn_totalmotoraxis_number= 18; //모터의 총 수량은 18개이다  
		COMI.mn_bd_axis_number[0] = 8;   //0번 모터보드는 8축용 보드이다  
		COMI.mn_bd_axis_number[1] = 6;   //1번 모터보드는 6축용 보드이다 
		COMI.mn_bd_axis_number[2] = 4;   //2번 모터보드는 4축용 보드이다 

		if(COMI.mn_motorbd_init_end == BD_YES)
		{			
			for(i=0; i<COMI.mn_totalmotoraxis_number; i++)
			{//n_simul_mode => 0:시뮬레이션 모드 비활성, 1:시뮬레이션 모드 활성

				if(COMI.mn_simulation_mode == 1) //모터를 가상으로 제어하여 실제 모터 출력이 나가고 기구물이 이동한것처럼 동작한다 
				{//활성
					COMI.Set_Simulation_Mode(i, 1); 
				}
				else
				{//비활성 
					COMI.Set_Simulation_Mode(i, 0); 
				}
			}		
		}
	}

	return  BD_GOOD;
}

//==================================================================//
// 모터 구동 가능 여부 확인 (인터락 조건)
// - nzReqMode : 모터 구동 조건
// #define REQ_HOMING_			0
// #define REQ_MOVE_START_		1
// #define REQ_MOVE_DONE_		2
// #define REQ_JOG_MOVE_		3
//==================================================================//
int CCtlBd_Library::Motor_SafetyCheck(int nzReqMode, int nzAxis, double dzTargetPos) 
{
	// 모션 보드 초기화 여부 검사
	// - 초기화 전에 함수 호출되면 프로그램 오류 발생
	if (st_handler.mn_mot_brd_initial != TRUE)
	{
		return BD_ERROR;
	}

	// 모터 이동 중 인터락 조건 체크인 경우
	// - 모터 상태만 체크하여 반환시킴
	// : 알람 리셋, 모터 파워 설정 등의 작업은 진행하지 않음
	// : Start_SingleMove() 함수에서 재동작 시 진행할 것임
	// - 모터 이동 중 알람 발생해서 멈춘 경우는 모터 파워를 다시 On 시킨다고 구동되지 않기 때문
	// - Start_SingleMove() 함수를 다시 호출해야 모터가 구동 시작됨
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
	// S/W 리미트 범위내의 이동 요청인지 확인
	// - 티칭 화면의 OnMotorSpeed_Data_Apply() 함수에서 구조체에 할당됨
	// : COMI.md_limit_position 구조체에 화면 설정값을 입력함
	//==============================================================//
	// (-) 설정 범위 체크
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
	// (+) 설정 범위 체크
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
	// 모터 파워 및 알람 상태 확인
	// - [POWER OFF] : [POWER ON] 작업 시도
	// - [모터 알람] : [알람 해제] 작업 시도
	//==============================================================//
	// [POWER OFF] 상태인지 검사
	if (g_comiMgr.Get_MotPower(nzAxis) == CTL_ERROR)
	{
		// 모터 [POWER ON] 작업 시도
		if (COMI.Set_MotPower(nzAxis, CTL_ON) == CTLBD_RET_GOOD)
		{
			if (st_handler.cwnd_list != NULL)
			{
				sprintf(st_msg.c_normal_msg, "[MotorPowerOn_1] Axis=%d, rcnt=%d", nzAxis, st_motor[nzAxis].n_retry_cnt);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
			}

			// [POWER ON] 동작 실패 시 알람 출력
			if (COMI.Get_MotPower(nzAxis) == CTL_ERROR)
			{
				//if (st_motor[nzAxis].n_retry_cnt > (MOT_RTY_CNT))
				// 규이리 수정 [2014.10.10]
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
			// 규이리 수정 [2014.10.10]
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

	// [모터 알람] 발생 상태인지 검사
	if (COMI.Get_MotAlarmStatus(nzAxis) == CTL_ERROR)
	{
		// [알람 해제] 작업 시도
		if (COMI.Set_MotAlarmClear(nzAxis) == CTL_GOOD)
		{
			if (st_handler.cwnd_list != NULL)
			{
				sprintf(st_msg.c_normal_msg, "[Alarm Reset] Axis=%d, rcnt=%d", nzAxis, COMI.mn_retry_cnt[nzAxis]);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
			}

			// [알람 해제] 작업 실패 시 알람 출력
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
		// 모터별 인터락 조건 체크
		nRet = OnCheck_MovePossible(nzReqMode, nzAxis, dzTargetPos);
		if (nRet != BD_GOOD)
		{
			return nRet;
		}
	}

	return BD_GOOD;
}

//==================================================================//
// 모터별 인터락 조건 체크
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

		// 규이리 주석 처리 [2015.03.10]
		// 악세사리를 컨베이어에 2개씩 내려놓고 Z축을 약간 들어 이동하기 위하여 주석 처리함
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
		// Z축의 현재 위치를 얻음
		d_CurPos[0] = COMI.Get_MotCurrentPos(M_LIFTER_Z1);
		d_CurPos[1] = COMI.Get_MotCurrentPos(M_LIFTER_Z2);
		d_CurPos[2] = COMI.Get_MotCurrentPos(M_LIFTER_Z3);
		d_CurPos[3] = COMI.Get_MotCurrentPos(M_LIFTER_Z4);
		// Z축의 안전 위치 정보 얻음
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
		// X축의 현재 위치를 얻음
		d_CurPos[0] = COMI.Get_MotCurrentPos(M_LIFTER_Y);
		// X축의 최소 작업 위치 정보 얻음
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

	st_motor[nzAxis].mn_retry_time_flag = CTL_NO;//2011.0201 추가 

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
		if (n_check == BD_YES)			// Size를 체크하는것이면...
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
		else						// Size를 확인하는것이면...
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

	CString mstr_cur_year, mstr_cur_month, mstr_cur_day, str_display_time; // 현재 년, 월, 일 정보 문자형으로 변환하여 저장할 변수 
	int mn_cur_year, mn_cur_month, mn_cur_day, mn_cur_hour; // 현재 년, 월, 일 정보 저장 변수 
	CString mstr_file_name, mstr_dir_path, str_msg;		// 마지막으로 생성된 파일 이름 저장 변수 
	CString mstr_create_file;	// 알람 정보 저장할 파일에 대한 [폴더]+[파일명]+[확장자] 설정 변수 
	CString mstr_list_name, mstr_temp_data;  // 각 부분별 알람 발생 횟수 정보 저장할 로그 파일 정보 저장 변수 
	CString mstr_content;		// 한 라인에 출력할 문자열 정보 저장 변수 
	COleDateTime time_cur;		// 검사할 시간 정보 저장 변수 
	CTime m_time_current;		// 간략한 헝식의 현재 시간 정보 저장 변수
	char chr_buf[20]={0,};
	int mn_existence, nlength;			// 파일 존재 유무 설정 플래그 
	char cfileName[256]={0,};			// 검색할 파일 정보 설정 함수 
	FILE  *fp ;					// 파일에 대한 포인터 설정 변수 

	CString BackupName;
	CString mstr_cur_hour, mstr_cur_min, mstr_cur_sec;
	int Ret=0;

	nlength = strlen(cp_msg);
	if(nlength <= 0) //저장할 메세지가 없다 
	{
		return BD_ERROR;
	}
	str_msg = cp_msg;
 
	// **************************************************************************
	// 파일 이름으로 사용할 날짜 정보를 얻는다                                   
	// **************************************************************************
	time_cur = COleDateTime::GetCurrentTime();  // 현재 시간 정보를 얻는다. 

	m_time_current = CTime::GetCurrentTime() ;  // 간략한 형식의 현재 시간 정보 얻는다. 

	mn_cur_year = time_cur.GetYear();  

	mn_cur_month = time_cur.GetMonth();  

    mn_cur_day = time_cur.GetDay();  

	mn_cur_hour = time_cur.GetHour();
	// **************************************************************************

	// **************************************************************************
	// 날짜 정보를 문자형으로 변환하여 변수에 설정한다                           
	// **************************************************************************
	mstr_cur_year = LPCTSTR(_itoa( mn_cur_year, chr_buf, 10 ));  
	mstr_cur_month.Format("%02d", mn_cur_month);
	mstr_cur_day.Format("%02d", mn_cur_day);

	// **************************************************************************
	// 현재 시간 정보 얻는다                                                     
	// **************************************************************************
	mn_cur_hour = time_cur.GetHour();				// 현재 시간 정보를 설정한다. 
	mstr_cur_hour.Format("%d",time_cur.GetHour());	// 현재 시간 정보를 설정한다.
	mstr_cur_min.Format("%d",time_cur.GetMinute());	// 현재 분 정보를 설정한다. 
	mstr_cur_sec.Format("%d",time_cur.GetSecond());	// 현재 초 정보를 설정한다. 
	str_display_time = m_time_current.Format("%c");	// 리스트 파일에 출력할 타이틀 시간 정보 설정 
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
	{//해당 파일이 존재하지 않는 상태이다 
		CreateFolder(cp_dir_path); 
	}
	else 
	{//해당 파일이 이미 존재한다.
		//정상 
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

		fclose(fp) ;  //2011.0417  파일 종료

//2011.0417		sing.Unlock(); //2011.0417 추가 
		return BD_ERROR;
	}
	// **************************************************************************
	fclose(fp);  // 파일을 종료한다.

//2011.0417	sing.Unlock(); //2011.0417 추가 
	return BD_GOOD;
}

//==================================================================//
// SingleMove 스텝 정보 초기화
// - 구동 중 정지된 경우 동작을 처음부터 다시 진행하기 위해 필요함
//==================================================================//
void CCtlBd_Library::OnReset_SingleMoveStep(int nzMotorNum) 
{
	if (m_nStep_Single[nzMotorNum] != 0)
	{
		m_nStep_Single[nzMotorNum] = 0;
	}
}

//==================================================================//
// 입력된 위치로 모터 이동
// - [현재 위치 == 이동할 위치] 그냥 리턴됨
//==================================================================//
int CCtlBd_Library::OnSingleMove(int nzMotorNum, double dzMovePos, int nzSpeedRate) 
{
	int nFuncRet = CTLBD_RET_PROCEED;
	
	int nRet;
	// 모터 축별로 데이터 관리
	switch(m_nStep_Single[nzMotorNum])
	{
	case 0:
		nRet = COMI.Check_MotPosRange(nzMotorNum, dzMovePos, COMI.mn_allow_value[nzMotorNum]);
		// 규이리 수정 [2014.08.26]
		//nRet = COMI.Check_MotPosRange(nzMotorNum, dzMovePos, 0);
		if (nRet == CTLBD_RET_GOOD)  // 이동할 위치에 이미 존재함
		{
			m_nStep_Single[nzMotorNum] = 0;
			nFuncRet = CTLBD_RET_GOOD;
			break;
		}

		// 재시도 횟수 초기화
		mn_mot_retry_cnt[nzMotorNum] = 0;

		m_nStep_Single[nzMotorNum] = 100;
		break;

	case 100:
		// 입력된 위치로 모터 이동시킴
		nRet = COMI.Start_SingleMove(nzMotorNum, dzMovePos, nzSpeedRate);
		if (nRet == CTLBD_RET_GOOD)
		{
			m_nStep_Single[nzMotorNum] = 200;
		}
		else if (nRet == CTLBD_RET_ERROR || nRet == CTLBD_RET_SAFETY)
		{
			// 재시도 횟수 증가시킴
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
			// 재시도 횟수 증가시킴
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
		nRet = COMI.Check_SingleMove(nzMotorNum, dzMovePos);  // 이동 완료 확인
		if (nRet == CTLBD_RET_GOOD)  // 정상 완료
		{
			m_nStep_Single[nzMotorNum] = 0;
			nFuncRet = CTLBD_RET_GOOD;
		}
		else if (nRet == CTLBD_RET_ERROR || nRet == CTLBD_RET_SAFETY)
		{
			// 재시도 횟수 증가시킴
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
		else if (nRet == CTLBD_RET_RETRY)  // 동작 재시도
		{
			// 재시도 횟수 증가시킴
			//mn_mot_retry_cnt[nzMotorNum]++;
			
			// 무한루프에 빠질수도 있지만 횟수를 지정해서 사용하면 문제 없음
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
		{//이미 모든 모터의 현재 위치가 이동하려는 위치와 허용오차 안에 있으면 동작할 필요가 없으니 끝내자.
			nFuncRet = BD_GOOD;
			break;
		}
		
		mn_linear_retry_cnt[n_LinearMapIndex] = 0;
		mn_linear_max_retry_cnt = 3; //최대 3회 리트라이 동작을 수행한다 
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
		else if (nRet[0] == BD_RETRY)  // 동작 재시도
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
		if (nRet[0] == BD_GOOD)  // 정상 완료
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
		else if (nRet[0] == BD_RETRY)  // 동작 재시도
		{
			// 무한루프에 빠질수도 있지만 횟수를 지정해서 사용하면 문제 없다
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
