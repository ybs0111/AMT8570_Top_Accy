// MyBasicData.cpp: implementation of the CMyBasicData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Handler.h"
#include "MyBasicData.h"
#include "io.h"
#include "math.h"
#include "ComizoaPublic.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMyBasicData::CMyBasicData()
{

}

CMyBasicData::~CMyBasicData()
{

}

void CMyBasicData::OnPassWord_Load(CString str_load_level)
{
	CString str_pass;
	char chr_data[50];						// 암호는 25글자 이내로 제한

	(st_msg.mstr_password).Empty();		// 암호 저장 변수 초기화

	if (str_load_level == "Lock")			// SYSTEM LOCK 암호
		:: GetPrivateProfileString("Password", "SysLock", "M", chr_data, 50, st_path.mstr_basic);
	else if (str_load_level == "Level1")	// 메인트 암호
		:: GetPrivateProfileString("Password", "Level_1", "M", chr_data, 50, st_path.mstr_basic);
	else if (str_load_level == "Level2")	// 티칭 암호
		:: GetPrivateProfileString("Password", "Level_2", "T", chr_data, 50, st_path.mstr_basic);
	else  
		return;

	str_pass = chr_data;
	str_pass.MakeUpper();     // 문자열 대문자로 변경
	str_pass.TrimLeft(' ');   // 좌측 문자열 공백 제거              
	str_pass.TrimRight(' ');  // 우측 문자열 공백 제거

	st_msg.mstr_password = str_pass;  // 암호 전역 변수에 설정
}

void CMyBasicData::OnPassWord_Save(CString str_save_level, CString str_pass_data)
{
	if (str_save_level=="Lock")         // SYSTEM LOCK 암호
	{
		:: WritePrivateProfileString("Password", "SysLock", LPCTSTR(str_pass_data), st_path.mstr_basic);
		:: WritePrivateProfileString("Password", "SysLockStatus", LPCTSTR("YES"), st_path.mstr_basic);
	}
	else if (str_save_level=="Level1")  // 메인트 암호
	{
		:: WritePrivateProfileString("Password", "Level_1", LPCTSTR(str_pass_data), st_path.mstr_basic);
	}
	else if (str_save_level=="Level2")  // 티칭 암호
	{
		:: WritePrivateProfileString("Password", "Level_2", LPCTSTR(str_pass_data), st_path.mstr_basic);
	}
}

CString CMyBasicData::OnStep_File_Index_Load()
{
	CString str_file_index;  // 파일 인덱스 저장 변수

	char chr_load[20];

	:: GetPrivateProfileString("Thread_Step_file", "File_Index", "00", chr_load, 20, st_path.mstr_basic);
	str_file_index = chr_load;
	str_file_index.TrimLeft(' ');               
	str_file_index.TrimRight(' ');

	return str_file_index;  // 파일 인덱스 리턴
}

void CMyBasicData::OnStep_File_Index_Save(CString str_index)
{
	:: WritePrivateProfileString("Thread_Step_file", "File_Index", LPCTSTR(str_index), st_path.mstr_basic);
}

CString CMyBasicData::OnStep_File_Name_Load()
{
	CString str_file_name;  // 파일명 저장 변수

	char chr_load[20];

	:: GetPrivateProfileString("Thread_Step_file", "File_Name", "DEFAULT", chr_load, 20, st_path.mstr_basic);
	str_file_name = chr_load;
	str_file_name.TrimLeft(' ');               
	str_file_name.TrimRight(' ');

	return str_file_name;  // 파일명 리턴
}

void CMyBasicData::OnStep_File_Name_Save(CString str_file)
{
	:: WritePrivateProfileString("Thread_Step_file", "File_Name", LPCTSTR(str_file), st_path.mstr_basic);
}

void CMyBasicData::OnLoad_MotSpeedData()
{
	int  n_chk, i;
	CString str_temp;       // 임시 저장 변수
	CString str_motor_name;
	char chr_data[20];

	for (i=0; i<MOT_MAXMOTOR; i++)
	{
		str_motor_name = Func.OnGet_MotorName(i) + "_Speed";

		n_chk = ::GetPrivateProfileInt(str_motor_name, "Velocity", 100, st_path.sMotSpeed);
		if (n_chk < 0)
		{
			n_chk = 1000;
		}
		COMI.md_spd_vel[i][0] = n_chk;

		n_chk = ::GetPrivateProfileInt(str_motor_name, "Accel", 100, st_path.sMotSpeed);
		if (n_chk < 0)
		{
			n_chk = 1000;
		}
		COMI.md_spd_vel[i][1] = n_chk;

		n_chk = ::GetPrivateProfileInt(str_motor_name, "Decel", 100, st_path.sMotSpeed);
		if (n_chk < 0)
		{
			n_chk = 1000;
		}
		COMI.md_spd_vel[i][2] = n_chk;

		n_chk = ::GetPrivateProfileInt(str_motor_name, "Home", 100, st_path.sMotSpeed);
		if (n_chk < 0)
		{
			n_chk = 1000;
		}
		COMI.md_spd_home[i] = n_chk;

		n_chk = ::GetPrivateProfileInt(str_motor_name, "Jog", 100, st_path.sMotSpeed);
		if (n_chk < 0)
		{
			n_chk = 1000;
		}
		COMI.md_spd_jog[i] = n_chk;

		n_chk = ::GetPrivateProfileInt(str_motor_name, "Allow", 100, st_path.sMotSpeed);
		if (n_chk < 0)
		{
			n_chk = 1000;
		}
		COMI.mn_allow_value[i] = n_chk;
		st_motor[i].mn_allow = n_chk;

		n_chk = ::GetPrivateProfileInt(str_motor_name, "- Limit", 100, st_path.sMotSpeed);
		if (n_chk < -100)
		{
			n_chk = 1000;
		}
		COMI.md_limit_position[i][0] = n_chk;

		n_chk = ::GetPrivateProfileInt(str_motor_name, "+ Limit", 100, st_path.sMotSpeed);
		if (n_chk < 0)
		{
			n_chk = 1000;
		}
		COMI.md_limit_position[i][1] = n_chk;

	}
	//Speed Rate
	:: GetPrivateProfileString("Speed_Rate", "Run", "100", chr_data, 10, st_path.sMotSpeed);
	n_chk = atoi(chr_data);
	COMI.mn_runspeed_rate = n_chk;
	st_handler.md_run_speed = n_chk;
	
	:: GetPrivateProfileString("Speed_Rate", "Manual", "80", chr_data, 10, st_path.sMotSpeed);
	n_chk = atoi(chr_data);
	COMI.mn_manualspeed_rate = n_chk;
	st_handler.md_manual_speed = n_chk;
}

void CMyBasicData::OnSaveMotorSpeedFile()
{
	int i;
	CString str_temp;       // 임시 저장 변수
	CString str_motor_name;
	char chr_buf[20];

	for (i = 0; i < MOT_MAXMOTOR; i++)
	{
		str_motor_name = Func.OnGet_MotorName(i) + "_Speed";

		str_temp = LPCTSTR(_itoa((int)(COMI.md_spd_vel[i][0]), chr_buf, 10));
		:: WritePrivateProfileString(str_motor_name, "Velocity", LPCTSTR(str_temp), st_path.sMotSpeed);

		str_temp = LPCTSTR(_itoa((int)(COMI.md_spd_vel[i][1]), chr_buf, 10));
		:: WritePrivateProfileString(str_motor_name, "Accel", LPCTSTR(str_temp), st_path.sMotSpeed);

		str_temp = LPCTSTR(_itoa((int)(COMI.md_spd_vel[i][2]), chr_buf, 10));
		:: WritePrivateProfileString(str_motor_name, "Decel", LPCTSTR(str_temp), st_path.sMotSpeed);

		str_temp = LPCTSTR(_itoa((int)(COMI.md_spd_home[i]), chr_buf, 10));
		:: WritePrivateProfileString(str_motor_name, "Home", LPCTSTR(str_temp), st_path.sMotSpeed);

		str_temp = LPCTSTR(_itoa((int)(COMI.md_spd_jog[i]), chr_buf, 10));
		:: WritePrivateProfileString(str_motor_name, "Jog", LPCTSTR(str_temp), st_path.sMotSpeed);

		str_temp = LPCTSTR(_itoa((int)(COMI.mn_allow_value[i]), chr_buf, 10));
		:: WritePrivateProfileString(str_motor_name, "Allow", LPCTSTR(str_temp), st_path.sMotSpeed);

		str_temp = LPCTSTR(_itoa((int)(COMI.md_limit_position[i][0]), chr_buf, 10));
		:: WritePrivateProfileString(str_motor_name, "- Limit", LPCTSTR(str_temp), st_path.sMotSpeed);

		str_temp = LPCTSTR(_itoa((int)(COMI.md_limit_position[i][1]), chr_buf, 10));
		:: WritePrivateProfileString(str_motor_name, "+ Limit", LPCTSTR(str_temp), st_path.sMotSpeed);
	}
	// Speed Rate
	str_temp = LPCTSTR(_itoa((int)(st_handler.md_run_speed), chr_buf, 10));
	:: WritePrivateProfileString("Speed_Rate", "Run", LPCTSTR(str_temp), st_path.sMotSpeed);
	
	str_temp = LPCTSTR(_itoa((int)(st_handler.md_manual_speed), chr_buf, 10));
	:: WritePrivateProfileString("Speed_Rate", "Manual", LPCTSTR(str_temp), st_path.sMotSpeed);
}

void CMyBasicData::OnLoad_WaitTimeData()
{
	CString str_temp;  // 로딩 정보 임시 저장 변수
	int nChk;
	int i;

	for (i=0; i<MAX_WAIT_TIME_NUM; i++)
	{
		str_temp.Format("WaitTime[1]_%02d", i);

		// On WaithTime
		nChk = ::GetPrivateProfileInt("OnWaitTime_Data", str_temp, 1, st_path.sWaitTime);
		if (nChk < 0 ||
			nChk > 50000)
		{
			nChk = 100;
		}
		st_time.nWait_On[i] = nChk;
	}

	for (i=0; i<MAX_WAIT_TIME_NUM; i++)
	{
		str_temp.Format("WaitTime[0]_%02d", i);

		// Off WaitTime
		nChk = ::GetPrivateProfileInt("OffWaitTime_Data", str_temp, 100, st_path.sWaitTime);
		if (nChk < 0 ||
			nChk > 50000)
		{
			nChk = 100;
		}
		st_time.nWait_Off[i] = nChk;
	}

	for (i=0; i<MAX_WAIT_TIME_NUM; i++)
	{
		str_temp.Format("LimitTime_%02d", i);

		// LimitTime
		nChk = ::GetPrivateProfileInt("LimitTime_Data", str_temp, 100, st_path.sWaitTime);
		if (nChk < 0 ||
			nChk > 50000)
		{
			nChk = 100;
		}
		st_time.nWait_Limit[i] = nChk;
	}
}

void CMyBasicData::OnSaveWaitTimeFile()
{
	CString sTemp;  // 로딩 정보 임시 저장 변수
	CString sDataName;
	char chr_buf[20];
	int i;
	
	for (i=0; i<MAX_WAIT_TIME_NUM; i++)
	{
		sDataName.Format("WaitTime[1]_%02d", i);

		// On WaitTime
		sTemp = LPCTSTR(_itoa(st_time.nWait_On[i], chr_buf, 10));
		:: WritePrivateProfileString("OnWaitTime_Data", sDataName, LPCTSTR(sTemp), st_path.sWaitTime);
	}

	for (i=0; i<MAX_WAIT_TIME_NUM; i++)
	{
		sDataName.Format("WaitTime[0]_%02d", i);

		// Off WaitTime
		sTemp = LPCTSTR(_itoa(st_time.nWait_Off[i], chr_buf, 10));
		:: WritePrivateProfileString("OffWaitTime_Data", sDataName, LPCTSTR(sTemp), st_path.sWaitTime);
	}

	for (i=0; i<MAX_WAIT_TIME_NUM; i++)
	{
		sDataName.Format("LimitTime_%02d", i);

		// Limit
		sTemp = LPCTSTR(_itoa(st_time.nWait_Limit[i], chr_buf, 10));
		:: WritePrivateProfileString("LimitTime_Data", sDataName, LPCTSTR(sTemp), st_path.sWaitTime);
	}
}

void CMyBasicData::OnLoad_MaintData()
{
	CString str_temp, str_part;  // 로딩 정보 임시 저장 변수
	int n_chk=0, i=0;
	CString strLoadFile;

	char chr_data[20], chr_buf[20] ;

	/* ************************************************************************** */
	/* 타워 램프 RED 상태 로딩하여 전역 변수에 설정한다                           */
	/* ->  STOP			= 0,
		   RUN			= 1,
		   ALARM		= 2,
		   LOTEND		= 3,
		   INIT			= 4,
		   WARRING		= 5,
		   LOCK			= 6,
		   SELFCHECK	= 7														  */
	/* -> 로딩 값 [0:OFF    1:ON    2:FLICKER]                                    */
	/* ************************************************************************** */
	:: GetPrivateProfileString("TowerLampData_Red", "Stop_State", "0", chr_data, 10, st_path.mstr_basic);
	n_chk = atoi(chr_data);
	if (n_chk<0 || n_chk>2)
	{
		stLAMP.nLampR[0] = 0;
		str_temp = LPCTSTR(_itoa(stLAMP.nLampR[0], chr_buf, 10)) ;
		:: WritePrivateProfileString("TowerLampData_Red", "Stop_State", LPCTSTR(str_temp), st_path.mstr_basic);
	}
	else
		stLAMP.nLampR[0] = n_chk;

	:: GetPrivateProfileString("TowerLampData_Red", "Run_State", "0", chr_data, 10, st_path.mstr_basic);
	n_chk = atoi(chr_data);
	if (n_chk<0 || n_chk>2)
	{
		stLAMP.nLampR[1] = 0;
		str_temp = LPCTSTR(_itoa(stLAMP.nLampR[1], chr_buf, 10)) ;
		:: WritePrivateProfileString("TowerLampData_Red", "Run_State", LPCTSTR(str_temp), st_path.mstr_basic);
	}
	else
		stLAMP.nLampR[1] = n_chk;

	:: GetPrivateProfileString("TowerLampData_Red", "Alarm_State", "0", chr_data, 10, st_path.mstr_basic);
	n_chk = atoi(chr_data);
	if (n_chk<0 || n_chk>2)
	{
		stLAMP.nLampR[2] = 2;
		str_temp = LPCTSTR(_itoa(stLAMP.nLampR[2], chr_buf, 10)) ;
		:: WritePrivateProfileString("TowerLampData_Red", "Alarm_State", LPCTSTR(str_temp), st_path.mstr_basic);
	}
	else
		stLAMP.nLampR[2] = n_chk;

	:: GetPrivateProfileString("TowerLampData_Red", "LotEnd_State", "0", chr_data, 10, st_path.mstr_basic);
	n_chk = atoi(chr_data);
	if (n_chk<0 || n_chk>2)
	{
		stLAMP.nLampR[3] = 2;
		str_temp = LPCTSTR(_itoa(stLAMP.nLampR[3], chr_buf, 10)) ;
		:: WritePrivateProfileString("TowerLampData_Red", "LotEnd_State", LPCTSTR(str_temp), st_path.mstr_basic);
	}
	else
		stLAMP.nLampR[3] = n_chk;

	:: GetPrivateProfileString("TowerLampData_Red", "Initial_State", "0", chr_data, 10, st_path.mstr_basic);
	n_chk = atoi(chr_data);
	if (n_chk<0 || n_chk>2)
	{
		stLAMP.nLampR[4] = 2;
		str_temp = LPCTSTR(_itoa(stLAMP.nLampR[4], chr_buf, 10)) ;
		:: WritePrivateProfileString("TowerLampData_Red", "Initial_State", LPCTSTR(str_temp), st_path.mstr_basic);
	}
	else
		stLAMP.nLampR[4] = n_chk;

	:: GetPrivateProfileString("TowerLampData_Red", "Warring_State", "0", chr_data, 10, st_path.mstr_basic);
	n_chk = atoi(chr_data);
	if (n_chk<0 || n_chk>2)
	{
		stLAMP.nLampR[5] = 2;
		str_temp = LPCTSTR(_itoa(stLAMP.nLampR[5], chr_buf, 10)) ;
		:: WritePrivateProfileString("TowerLampData_Red", "Warring_State", LPCTSTR(str_temp), st_path.mstr_basic);
	}
	else
		stLAMP.nLampR[5] = n_chk;

	:: GetPrivateProfileString("TowerLampData_Red", "Lock_State", "0", chr_data, 10, st_path.mstr_basic);
	n_chk = atoi(chr_data);
	if (n_chk<0 || n_chk>2)
	{
		stLAMP.nLampR[6] = 2;
		str_temp = LPCTSTR(_itoa(stLAMP.nLampR[6], chr_buf, 10)) ;
		:: WritePrivateProfileString("TowerLampData_Red", "Lock_State", LPCTSTR(str_temp), st_path.mstr_basic);
	}
	else
		stLAMP.nLampR[6] = n_chk;

	:: GetPrivateProfileString("TowerLampData_Red", "SelfCheck_State", "0", chr_data, 10, st_path.mstr_basic);
	n_chk = atoi(chr_data);
	if (n_chk<0 || n_chk>2)
	{
		stLAMP.nLampR[7] = 2;
		str_temp = LPCTSTR(_itoa(stLAMP.nLampR[7], chr_buf, 10)) ;
		:: WritePrivateProfileString("TowerLampData_Red", "SelfCheck_State", LPCTSTR(str_temp), st_path.mstr_basic);
	}
	else
		stLAMP.nLampR[7] = n_chk;
	/* ************************************************************************** */


	/* ************************************************************************** */
	/* 타워 램프 YELLOW 상태 로딩하여 전역 변수에 설정한다                        */
	/* ->  STOP			= 0,
		   RUN			= 1,
		   ALARM		= 2,
		   LOTEND		= 3,
		   INIT			= 4,
		   WARRING		= 5,
		   LOCK			= 6,
		   SELFCHECK	= 7														  */
	/* -> 로딩 값 [0:OFF    1:ON    2:FLICKER]                                    */
	/* ************************************************************************** */
	:: GetPrivateProfileString("TowerLampData_Yellow", "Stop_State", "0", chr_data, 10, st_path.mstr_basic);
	n_chk = atoi(chr_data);
	if (n_chk<0 || n_chk>2)
	{
		stLAMP.nLampY[0] = 0;
		str_temp = LPCTSTR(_itoa(stLAMP.nLampY[0], chr_buf, 10)) ;
		:: WritePrivateProfileString("TowerLampData_Yellow", "Stop_State", LPCTSTR(str_temp), st_path.mstr_basic);
	}
	else
		stLAMP.nLampY[0] = n_chk;

	:: GetPrivateProfileString("TowerLampData_Yellow", "Run_State", "0", chr_data, 10, st_path.mstr_basic);
	n_chk = atoi(chr_data);
	if (n_chk<0 || n_chk>2)
	{
		stLAMP.nLampY[1] = 1;
		str_temp = LPCTSTR(_itoa(stLAMP.nLampY[1], chr_buf, 10)) ;
		:: WritePrivateProfileString("TowerLampData_Yellow", "Run_State", LPCTSTR(str_temp), st_path.mstr_basic);
	}
	else
		stLAMP.nLampY[1] = n_chk;

	:: GetPrivateProfileString("TowerLampData_Yellow", "Alarm_State", "0", chr_data, 10, st_path.mstr_basic);
	n_chk = atoi(chr_data);
	if (n_chk<0 || n_chk>2)
	{
		stLAMP.nLampY[2] = 0;
		str_temp = LPCTSTR(_itoa(stLAMP.nLampY[2], chr_buf, 10)) ;
		:: WritePrivateProfileString("TowerLampData_Yellow", "Alarm_State", LPCTSTR(str_temp), st_path.mstr_basic);
	}
	else
		stLAMP.nLampY[2] = n_chk;

	:: GetPrivateProfileString("TowerLampData_Yellow", "LotEnd_State", "0", chr_data, 10, st_path.mstr_basic);
	n_chk = atoi(chr_data);
	if (n_chk<0 || n_chk>2)
	{
		stLAMP.nLampY[3] = 2;
		str_temp = LPCTSTR(_itoa(stLAMP.nLampY[3], chr_buf, 10)) ;
		:: WritePrivateProfileString("TowerLampData_Yellow", "LotEnd_State", LPCTSTR(str_temp), st_path.mstr_basic);
	}
	else
		stLAMP.nLampY[3] = n_chk;

	:: GetPrivateProfileString("TowerLampData_Yellow", "Initial_State", "0", chr_data, 10, st_path.mstr_basic);
	n_chk = atoi(chr_data);
	if (n_chk<0 || n_chk>2)
	{
		stLAMP.nLampY[4] = 2;
		str_temp = LPCTSTR(_itoa(stLAMP.nLampY[4], chr_buf, 10)) ;
		:: WritePrivateProfileString("TowerLampData_Yellow", "Initial_State", LPCTSTR(str_temp), st_path.mstr_basic);
	}
	else
		stLAMP.nLampY[4] = n_chk;

	:: GetPrivateProfileString("TowerLampData_Yellow", "Warring_State", "0", chr_data, 10, st_path.mstr_basic);
	n_chk = atoi(chr_data);
	if (n_chk<0 || n_chk>2)
	{
		stLAMP.nLampY[5] = 2;
		str_temp = LPCTSTR(_itoa(stLAMP.nLampY[5], chr_buf, 10)) ;
		:: WritePrivateProfileString("TowerLampData_Yellow", "Warring_State", LPCTSTR(str_temp), st_path.mstr_basic);
	}
	else
		stLAMP.nLampY[5] = n_chk;

	:: GetPrivateProfileString("TowerLampData_Yellow", "Lock_State", "0", chr_data, 10, st_path.mstr_basic);
	n_chk = atoi(chr_data);
	if (n_chk<0 || n_chk>2)
	{
		stLAMP.nLampY[6] = 2;
		str_temp = LPCTSTR(_itoa(stLAMP.nLampY[6], chr_buf, 10)) ;
		:: WritePrivateProfileString("TowerLampData_Yellow", "Lock_State", LPCTSTR(str_temp), st_path.mstr_basic);
	}
	else
		stLAMP.nLampY[6] = n_chk;

	:: GetPrivateProfileString("TowerLampData_Yellow", "SelfCheck_State", "0", chr_data, 10, st_path.mstr_basic);
	n_chk = atoi(chr_data);
	if (n_chk<0 || n_chk>2)
	{
		stLAMP.nLampY[7] = 2;
		str_temp = LPCTSTR(_itoa(stLAMP.nLampY[7], chr_buf, 10)) ;
		:: WritePrivateProfileString("TowerLampData_Yellow", "SelfCheck_State", LPCTSTR(str_temp), st_path.mstr_basic);
	}
	else
		stLAMP.nLampY[7] = n_chk;
	/* ************************************************************************** */

	/* ************************************************************************** */
	/* 타워 램프 GREEN 상태 로딩하여 전역 변수에 설정한다                         */
	/* ->  STOP			= 0,
		   RUN			= 1,
		   ALARM		= 2,
		   LOTEND		= 3,
		   INIT			= 4,
		   WARRING		= 5,
		   LOCK			= 6,
		   SELFCHECK	= 7														  */
	/* -> 로딩 값 [0:OFF    1:ON    2:FLICKER]                                    */
	/* ************************************************************************** */
	:: GetPrivateProfileString("TowerLampData_Green", "Stop_State", "0", chr_data, 10, st_path.mstr_basic);
	n_chk = atoi(chr_data);
	if (n_chk<0 || n_chk>2)
	{
		stLAMP.nLampG[0] = 1;
		str_temp = LPCTSTR(_itoa(stLAMP.nLampG[0], chr_buf, 10)) ;
		:: WritePrivateProfileString("TowerLampData_Green", "Stop_State", LPCTSTR(str_temp), st_path.mstr_basic);
	}
	else
		stLAMP.nLampG[0] = n_chk;

	:: GetPrivateProfileString("TowerLampData_Green", "Run_State", "0", chr_data, 10, st_path.mstr_basic);
	n_chk = atoi(chr_data);
	if (n_chk<0 || n_chk>2)
	{
		stLAMP.nLampG[1] = 0;
		str_temp = LPCTSTR(_itoa(stLAMP.nLampG[1], chr_buf, 10)) ;
		:: WritePrivateProfileString("TowerLampData_Green", "Run_State", LPCTSTR(str_temp), st_path.mstr_basic);
	}
	else
		stLAMP.nLampG[1] = n_chk;

	:: GetPrivateProfileString("TowerLampData_Green", "Alarm_State", "0", chr_data, 10, st_path.mstr_basic);
	n_chk = atoi(chr_data);
	if (n_chk<0 || n_chk>2)
	{
		stLAMP.nLampG[2] = 0;
		str_temp = LPCTSTR(_itoa(stLAMP.nLampG[2], chr_buf, 10)) ;
		:: WritePrivateProfileString("TowerLampData_Green", "Alarm_State", LPCTSTR(str_temp), st_path.mstr_basic);
	}
	else
		stLAMP.nLampG[2] = n_chk;

	:: GetPrivateProfileString("TowerLampData_Green", "LotEnd_State", "0", chr_data, 10, st_path.mstr_basic);
	n_chk = atoi(chr_data);
	if (n_chk<0 || n_chk>2)
	{
		stLAMP.nLampG[3] = 2;
		str_temp = LPCTSTR(_itoa(stLAMP.nLampG[3], chr_buf, 10)) ;
		:: WritePrivateProfileString("TowerLampData_Green", "LotEnd_State", LPCTSTR(str_temp), st_path.mstr_basic);
	}
	else
		stLAMP.nLampG[3] = n_chk;

	:: GetPrivateProfileString("TowerLampData_Green", "Initial_State", "0", chr_data, 10, st_path.mstr_basic);
	n_chk = atoi(chr_data);
	if (n_chk<0 || n_chk>2)
	{
		stLAMP.nLampG[4] = 2;
		str_temp = LPCTSTR(_itoa(stLAMP.nLampG[4], chr_buf, 10)) ;
		:: WritePrivateProfileString("TowerLampData_Green", "Initial_State", LPCTSTR(str_temp), st_path.mstr_basic);
	}
	else
		stLAMP.nLampG[4] = n_chk;

	:: GetPrivateProfileString("TowerLampData_Green", "Warring_State", "0", chr_data, 10, st_path.mstr_basic);
	n_chk = atoi(chr_data);
	if (n_chk<0 || n_chk>2)
	{
		stLAMP.nLampG[5] = 2;
		str_temp = LPCTSTR(_itoa(stLAMP.nLampG[5], chr_buf, 10)) ;
		:: WritePrivateProfileString("TowerLampData_Green", "Warring_State", LPCTSTR(str_temp), st_path.mstr_basic);
	}
	else
		stLAMP.nLampG[5] = n_chk;

	:: GetPrivateProfileString("TowerLampData_Green", "Lock_State", "0", chr_data, 10, st_path.mstr_basic);
	n_chk = atoi(chr_data);
	if (n_chk<0 || n_chk>2)
	{
		stLAMP.nLampG[6] = 2;
		str_temp = LPCTSTR(_itoa(stLAMP.nLampG[6], chr_buf, 10)) ;
		:: WritePrivateProfileString("TowerLampData_Green", "Lock_State", LPCTSTR(str_temp), st_path.mstr_basic);
	}
	else
		stLAMP.nLampG[6] = n_chk;

	:: GetPrivateProfileString("TowerLampData_Green", "SelfCheck_State", "0", chr_data, 10, st_path.mstr_basic);
	n_chk = atoi(chr_data);
	if (n_chk<0 || n_chk>2)
	{
		stLAMP.nLampG[7] = 2;
		str_temp = LPCTSTR(_itoa(stLAMP.nLampG[7], chr_buf, 10)) ;
		:: WritePrivateProfileString("TowerLampData_Green", "SelfCheck_State", LPCTSTR(str_temp), st_path.mstr_basic);
	}
	else
		stLAMP.nLampG[7] = n_chk;
	/* ************************************************************************** */

	/* ************************************************************************** */
	/* 부저 사용 모드 로딩하여 전역 변수에 설정                                   */
	/* -> 0:사용    1:미사용                                                      */
	/* ************************************************************************** */
	:: GetPrivateProfileString("TowerLampData", "n_buzzer_mode", "0", chr_data, 10, st_path.mstr_basic);
	n_chk = atoi(chr_data);
	if (n_chk<0 || n_chk>1)
	{
		stLAMP.nModeBuzzer = 1;
		str_temp = LPCTSTR(_itoa(stLAMP.nModeBuzzer, chr_buf, 10)) ;
		:: WritePrivateProfileString("TowerLampData", "n_buzzer_mode", LPCTSTR(str_temp), st_path.mstr_basic);
	}
	else
		stLAMP.nModeBuzzer = n_chk;
	/* ************************************************************************** */

	/* ************************************************************************** */
	/* 장비 호기 및 장비 코드 로딩하여 전역 변수에 설정                           */
	/* -> 로딩 정보에서 앞/뒤 공백은 제거한다                                     */
	/* ************************************************************************** */
	:: GetPrivateProfileString("TowerLampData", "str_equip_no", "AMT", chr_data, 20, st_path.mstr_basic);
	st_lamp.mstr_equip_no = chr_data;
	(st_lamp.mstr_equip_no).TrimLeft(' ');               
	(st_lamp.mstr_equip_no).TrimRight(' ');

	:: GetPrivateProfileString("TowerLampData", "str_equip_code", "AMT", chr_data, 20, st_path.mstr_basic);
	st_lamp.mstr_equip_id = chr_data;
	(st_lamp.mstr_equip_id).TrimLeft(' ');               
	(st_lamp.mstr_equip_id).TrimRight(' ');

	:: GetPrivateProfileString("TowerLampData", "str_ec_equip_code", "AMT", chr_data, 20, st_path.mstr_basic);	// 20130822 kjh
	st_lamp.mstr_ec_equip_id = chr_data;
	(st_lamp.mstr_ec_equip_id).TrimLeft(' ');               
	(st_lamp.mstr_ec_equip_id).TrimRight(' ');
	/* ************************************************************************** */

	/* ************************************************************************** */
	/* 타워 램프 ON/OFF 대기 시간 로딩하여 전역 변수에 설정                       */
	/* ************************************************************************** */
	:: GetPrivateProfileString("TowerLampData", "n_lamp_on_time_w", "0", chr_data, 10, st_path.mstr_basic);
	n_chk = atoi(chr_data);
	if (n_chk < 1)
	{
		stLAMP.nTwinkle = 500;
		str_temp = LPCTSTR(_itoa(stLAMP.nTwinkle, chr_buf, 10)) ;
		:: WritePrivateProfileString("TowerLampData", "n_lamp_on_time_w", LPCTSTR(str_temp), st_path.mstr_basic);
	}
	else
		stLAMP.nTwinkle = n_chk;
	/* ************************************************************************** */

	/* ************************************************************************** */
	/* 타워 램프 변경 시간 로딩하여 전역 변수에 설정                              */
	/* ************************************************************************** */
	:: GetPrivateProfileString("TowerLampData", "n_lamp_change_time", "0", chr_data, 10, st_path.mstr_basic);
	n_chk = atoi(chr_data);
	if (n_chk < 0)
	{
		st_lamp.mn_lamp_change_time = 5;
		str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_change_time, chr_buf, 10)) ;
		:: WritePrivateProfileString("TowerLampData", "n_lamp_change_time", LPCTSTR(str_temp), st_path.mstr_basic);
	}
	else
		st_lamp.mn_lamp_change_time = n_chk;
	/* ************************************************************************** */

	:: GetPrivateProfileString("AccySize", "d_X_Gripper_Size", "158.000", chr_data, 10, st_path.mstr_basic);
	st_accy.d_X_Gripper_Size = atof(chr_data);
	
	:: GetPrivateProfileString("AccySize", "d_Y_Gripper_Size", "158.000", chr_data, 10, st_path.mstr_basic);
	st_accy.d_Y_Gripper_Size = atof(chr_data);
	
	:: GetPrivateProfileString("AccySize", "d_Y_BufferConv_Size", "20.000", chr_data, 10, st_path.mstr_basic);
	st_accy.d_Y_BufferConv_Size = atof(chr_data);
}

void CMyBasicData::OnMaintenance_Data_Save()
{
	CString str_temp, str_part;  // 로딩 정보 임시 저장 변수
	char chr_buf[20] ;
	CString strSaveFile;
	int i=0;

	/* ************************************************************************** */
	/* 타워 램프 RED 상태 정보를 파일에 저장한다                                  */
	/* ->  STOP			= 0,
		   RUN			= 1,
		   ALARM		= 2,
		   LOTEND		= 3,
		   INIT			= 4,
		   WARRING		= 5,
		   LOCK			= 6,
		   SELFCHECK	= 7														  */
	/* -> 로딩 값 [0:OFF    1:ON    2:FLICKER]                                    */
	/* ************************************************************************** */
	str_temp = LPCTSTR(_itoa(stLAMP.nLampR[0], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Red", "Stop_State", LPCTSTR(str_temp), st_path.mstr_basic);

	str_temp = LPCTSTR(_itoa(stLAMP.nLampR[1], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Red", "Run_State", LPCTSTR(str_temp), st_path.mstr_basic);

	str_temp = LPCTSTR(_itoa(stLAMP.nLampR[2], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Red", "Alarm_State", LPCTSTR(str_temp), st_path.mstr_basic);

	str_temp = LPCTSTR(_itoa(stLAMP.nLampR[3], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Red", "LotEnd_State", LPCTSTR(str_temp), st_path.mstr_basic);

	str_temp = LPCTSTR(_itoa(stLAMP.nLampR[4], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Red", "Initial_State", LPCTSTR(str_temp), st_path.mstr_basic);

	str_temp = LPCTSTR(_itoa(stLAMP.nLampR[5], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Red", "Warring_State", LPCTSTR(str_temp), st_path.mstr_basic);

	str_temp = LPCTSTR(_itoa(stLAMP.nLampR[6], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Red", "Lock_State", LPCTSTR(str_temp), st_path.mstr_basic);

	str_temp = LPCTSTR(_itoa(stLAMP.nLampR[7], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Red", "SelfCheck_State", LPCTSTR(str_temp), st_path.mstr_basic);
	/* ************************************************************************** */


	/* ************************************************************************** */
	/* 타워 램프 YELLOW 상태 정보를 파일에 저장한다                               */
	/* ->  STOP			= 0,
		   RUN			= 1,
		   ALARM		= 2,
		   LOTEND		= 3,
		   INIT			= 4,
		   WARRING		= 5,
		   LOCK			= 6,
		   SELFCHECK	= 7														  */
	/* -> 로딩 값 [0:OFF    1:ON    2:FLICKER]                                    */
	/* ************************************************************************** */
	str_temp = LPCTSTR(_itoa(stLAMP.nLampY[0], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Yellow", "Stop_State", LPCTSTR(str_temp), st_path.mstr_basic);

	str_temp = LPCTSTR(_itoa(stLAMP.nLampY[1], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Yellow", "Run_State", LPCTSTR(str_temp), st_path.mstr_basic);

	str_temp = LPCTSTR(_itoa(stLAMP.nLampY[2], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Yellow", "Alarm_State", LPCTSTR(str_temp), st_path.mstr_basic);

	str_temp = LPCTSTR(_itoa(stLAMP.nLampY[3], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Yellow", "LotEnd_State", LPCTSTR(str_temp), st_path.mstr_basic);

	str_temp = LPCTSTR(_itoa(stLAMP.nLampY[4], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Yellow", "Initial_State", LPCTSTR(str_temp), st_path.mstr_basic);

	str_temp = LPCTSTR(_itoa(stLAMP.nLampY[5], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Yellow", "Warring_State", LPCTSTR(str_temp), st_path.mstr_basic);

	str_temp = LPCTSTR(_itoa(stLAMP.nLampY[6], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Yellow", "Lock_State", LPCTSTR(str_temp), st_path.mstr_basic);

	str_temp = LPCTSTR(_itoa(stLAMP.nLampY[7], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Yellow", "SelfCheck_State", LPCTSTR(str_temp), st_path.mstr_basic);

	/* ************************************************************************** */

	/* ************************************************************************** */
	/* 타워 램프 GREEN 상태 정보를 파일에 저장한다                                */
	/* ->  STOP			= 0,
		   RUN			= 1,
		   ALARM		= 2,
		   LOTEND		= 3,
		   INIT			= 4,
		   WARRING		= 5,
		   LOCK			= 6,
		   SELFCHECK	= 7														  */
	/* -> 로딩 값 [0:OFF    1:ON    2:FLICKER]                                    */
	/* ************************************************************************** */
	str_temp = LPCTSTR(_itoa(stLAMP.nLampG[0], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Green", "Stop_State", LPCTSTR(str_temp), st_path.mstr_basic);

	str_temp = LPCTSTR(_itoa(stLAMP.nLampG[1], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Green", "Run_State", LPCTSTR(str_temp), st_path.mstr_basic);

	str_temp = LPCTSTR(_itoa(stLAMP.nLampG[2], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Green", "Alarm_State", LPCTSTR(str_temp), st_path.mstr_basic);

	str_temp = LPCTSTR(_itoa(stLAMP.nLampG[3], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Green", "LotEnd_State", LPCTSTR(str_temp), st_path.mstr_basic);

	str_temp = LPCTSTR(_itoa(stLAMP.nLampG[4], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Green", "Initial_State", LPCTSTR(str_temp), st_path.mstr_basic);

	str_temp = LPCTSTR(_itoa(stLAMP.nLampG[5], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Green", "Warring_State", LPCTSTR(str_temp), st_path.mstr_basic);

	str_temp = LPCTSTR(_itoa(stLAMP.nLampG[6], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Green", "Lock_State", LPCTSTR(str_temp), st_path.mstr_basic);

	str_temp = LPCTSTR(_itoa(stLAMP.nLampG[7], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Green", "SelfCheck_State", LPCTSTR(str_temp), st_path.mstr_basic);

	/* ************************************************************************** */

	/* ************************************************************************** */
	/* 부저 사용 모드 로딩하여 전역 변수에 설정                                   */
	/* -> 0:사용    1:미사용                                                      */
	/* ************************************************************************** */
	str_temp = LPCTSTR(_itoa(stLAMP.nModeBuzzer, chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData", "n_buzzer_mode", LPCTSTR(str_temp), st_path.mstr_basic);
	/* ************************************************************************** */

	/* ************************************************************************** */
	/* 장비 호기 및 장비 코드 로딩하여 전역 변수에 설정                           */
	/* -> 로딩 정보에서 앞/뒤 공백은 제거한다                                     */
	/* ************************************************************************** */
	:: WritePrivateProfileString("TowerLampData", "str_equip_no", LPCTSTR(st_lamp.mstr_equip_no), st_path.mstr_basic);
	:: WritePrivateProfileString("TowerLampData", "str_equip_code", LPCTSTR(st_lamp.mstr_equip_id), st_path.mstr_basic);
	:: WritePrivateProfileString("TowerLampData", "str_ec_equip_code", LPCTSTR(st_lamp.mstr_ec_equip_id), st_path.mstr_basic);	// 20130822 kjh
	/* ************************************************************************** */

	/* ************************************************************************** */
	/* 타워 램프 ON/OFF 대기 시간 로딩하여 전역 변수에 설정                       */
	/* ************************************************************************** */
	str_temp = LPCTSTR(_itoa(stLAMP.nTwinkle, chr_buf, 10));
	:: WritePrivateProfileString("TowerLampData", "n_lamp_on_time_w", LPCTSTR(str_temp), st_path.mstr_basic);
	/* ************************************************************************** */

	str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_change_time, chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData", "n_lamp_change_time", LPCTSTR(str_temp), st_path.mstr_basic);

	//str_temp = LPCTSTR(_itoa(st_tray.d_Z_Pocket_Size, chr_buf, 10)) ;
	str_temp.Format("%.3f", st_accy.d_X_Gripper_Size);
	:: WritePrivateProfileString("AccySize", "d_X_Gripper_Size", LPCTSTR(str_temp), st_path.mstr_basic);
	
	//str_temp = LPCTSTR(_itoa(st_tray.d_X_Pocket_Size, chr_buf, 10)) ;
	str_temp.Format("%.3f", st_accy.d_Y_Gripper_Size);
	:: WritePrivateProfileString("AccySize", "d_Y_Gripper_Size", LPCTSTR(str_temp), st_path.mstr_basic);
	
	//str_temp = LPCTSTR(_itoa(st_tray.d_Y_Pocket_Size, chr_buf, 10)) ;
	str_temp.Format("%.3f", st_accy.d_Y_BufferConv_Size);
	:: WritePrivateProfileString("AccySize", "d_Y_BufferConv_Size", LPCTSTR(str_temp), st_path.mstr_basic);
}

void CMyBasicData::OnLoad_BasicData()
{
	CString str_load_device;	// 로딩 디바이스명 저장 변수
	CString str_load_file;
	CString str_chk_ext;		// 파일 확장자 저장 변수
	CString mstr_temp, str_part, str_station, str_board;			// 저장할 정보 임시 저장 변수 
	char chr_data[50], chr_buf[20];	
	int mn_chk = 0, mn_pos = 0, n_pos = 0, i = 0, j = 0;
	double md_chk = 0;

	// Device Name
	:: GetPrivateProfileString("BasicData", "DeviceType", "DEFAULT", chr_data, 50, st_path.mstr_basic);
	str_load_device = chr_data;
	str_load_device.TrimLeft(' ');               
	str_load_device.TrimRight(' ');	
	
	mn_pos = str_load_device.Find(".TXT");	 //(".");								// 확장자 위치 검사
	if (mn_pos == -1)
		st_basic.sDvcName = str_load_device + _T(".TXT");	// 확장자 추가
	else 
	{
		str_chk_ext = str_load_device.Mid(mn_pos);					// 파일 확장자 설정
		if (str_chk_ext != _T(".TXT"))  
			st_basic.sDvcName = _T("DEFAULT.TXT");			// 초기 디바이스 파일명 설정
		else 
			st_basic.sDvcName = str_load_device;				// 디바이스 파일명 설정
	}

	/* ************************************************************************** */
    /*  데이터 로딩할 파일 설정한다 [파일 확장자 검사]                            */
    /* ************************************************************************** */
	//str_load_file = st_path.mstr_path_dvc + st_basic.mstr_device_name;  // 티칭 데이터 로딩 파일 설정
	//kwlee 2017.0922
	str_load_file = st_path.mstr_path_dvc + st_basic.sDvcName;  // 티칭 데이터 로딩 파일 설정
	
	n_pos = str_load_file.Find(".");  // 확장자 위치 검사

	if (n_pos == -1)
		str_load_file += _T(".TXT");  // 확장자 추가
	else 
	{
		str_chk_ext = str_load_file.Mid(n_pos);  // 파일 확장자 설정
		if (str_chk_ext != _T(".TXT"))  
		{
			str_load_file = st_path.mstr_path_dvc + _T("DEFAULT.TXT");  // 로딩 로봇 X축 티칭 데이터 로딩 새로운 파일 설정

			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
			{
//				st_msg.mstr_abnormal_msg = _T("[DEVICE FILE] The error happened at a file extension.");
				sprintf(st_msg.c_abnormal_msg, "[DEVICE FILE] The error happened at a file extension.");
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
			}
		}
	}
	/* ************************************************************************** */

	// **************************************************************************
	// 장비 호기 및 장비 코드 로딩하여 전역 변수에 설정                          
	// -> 로딩 정보에서 앞/뒤 공백은 제거한다                                    
	// **************************************************************************
	:: GetPrivateProfileString("BasicData", "equip_no", "AMT001", chr_data, 20, st_path.mstr_basic);
	st_basic.sEqpNo = chr_data;
	(st_basic.sEqpNo).TrimLeft(' ');               
	(st_basic.sEqpNo).TrimRight(' ');
	
	:: GetPrivateProfileString("BasicData", "equip_model", "AMT7010", chr_data, 20, st_path.mstr_basic);
	st_basic.sEqpModel = chr_data;
	(st_basic.sEqpModel).TrimLeft(' ');               
	(st_basic.sEqpModel).TrimRight(' ');
	// **************************************************************************
	
	mn_chk = GetPrivateProfileInt("BasicData", "BCR_Read_State", 0, st_path.mstr_basic);
	if (mn_chk < 0 ||
		mn_chk > 1)
	{
		mn_chk = 0;
	}
	st_basic.nMode_BCR = mn_chk;

	// 자재 사용 모드 (0:Without, 1:With)
	:: GetPrivateProfileString("BasicData", "Device_Mode", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0 || mn_chk > 1)
	{
		st_basic.n_mode_device = 1;
		mstr_temp.Format("%d", st_basic.n_mode_device);
		:: WritePrivateProfileString("BasicData", "Device_Mode", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_basic.n_mode_device = mn_chk;

	// 자재 사용 모드 (0:Offline, 1:Online)
	:: GetPrivateProfileString("BasicData", "Network_Mode", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0 || mn_chk > 1)
	{
		st_basic.n_mode_Network = 1;
		mstr_temp.Format("%d", st_basic.n_mode_Network);
		:: WritePrivateProfileString("BasicData", "Network_Mode", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_basic.n_mode_Network = mn_chk;


	///////////////////// 김양규 추가
	:: GetPrivateProfileString("BasicData", "MasterSlave_Mode", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0 || mn_chk > 1)
	{
		st_basic.nMode_SupplyMode = 1;
		mstr_temp.Format("%d", st_basic.nMode_SupplyMode);
		:: WritePrivateProfileString("BasicData", "MasterSlave_Mode", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_basic.nMode_SupplyMode = mn_chk;

	:: GetPrivateProfileString("BasicData", "Supply_Device", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0 || mn_chk > 14)
	{
		st_basic.nMode_SupplyDevice = 0;
		mstr_temp.Format("%d", st_basic.nMode_SupplyDevice);
		:: WritePrivateProfileString("BasicData", "Supply_Device", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_basic.nMode_SupplyDevice = mn_chk;

	:: GetPrivateProfileString("BasicData", "SMEMA_Mode", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0 || mn_chk > 4)
	{
		st_basic.nMode_SMEMAUsing = 0;
		mstr_temp.Format("%d", st_basic.nMode_SMEMAUsing);
		:: WritePrivateProfileString("BasicData", "SMEMA_Mode", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_basic.nMode_SMEMAUsing = mn_chk;

	:: GetPrivateProfileString("BasicData", "GripRetryCNT", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0 || mn_chk > 4)
	{
		st_basic.nMode_GripRetryCNT = 1;
		mstr_temp.Format("%d", st_basic.nMode_GripRetryCNT);
		:: WritePrivateProfileString("BasicData", "GripRetryCNT", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_basic.nMode_GripRetryCNT = mn_chk;

	////////////////////////////////////////

	// yangkyu [2015.02.28]
	// Grip, Vac 사용 모드 추가
	:: GetPrivateProfileString("BasicData", "VacUsingMode", "1", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0 || mn_chk > 1)
	{
		st_basic.nMode_VacUsing = VAC_NOT_USING;
		mstr_temp.Format("%d", st_basic.nMode_VacUsing);
		:: WritePrivateProfileString("BasicData", "VacUsingMode", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_basic.nMode_VacUsing = mn_chk;

	:: GetPrivateProfileString("BasicData", "GripUsingMode", "1", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0 || mn_chk > 1)
	{
		st_basic.nMode_GripUsing = GRIP_NOT_USING;
		mstr_temp.Format("%d", st_basic.nMode_GripUsing);
		:: WritePrivateProfileString("BasicData", "GripUsingMode", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_basic.nMode_GripUsing = mn_chk;

	// 규이리 추가 [2015.03.25]
	// 악세사리 내려놓기 모드 [1:4개씩, 0:2개씩]
	mn_chk = GetPrivateProfileInt("BasicData", "nMode_PlaceType", 0, st_path.mstr_basic);
	if (mn_chk < 0 ||
		mn_chk > 1)
	{
		mn_chk = 0;
	}
	st_basic.nMode_PlaceType = mn_chk;
	//////////////////////////////////////////////////////////////////////////

//	mn_chk = GetPrivateProfileInt("BasicData", "nMode_Pitch_Variable", 0, st_path.mstr_basic);
	//kwlee 2017.0922
	mn_chk = GetPrivateProfileInt("BasicData", "nMode_Pitch_Variable", 0, str_load_file);
	
	if (mn_chk < 0 ||
		mn_chk > 1)
	{
		mn_chk = 0;
	}
	st_basic.nMode_Slop_Variable = mn_chk;

	:: GetPrivateProfileString("BasicData", "Electric_Check", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 1 || mn_chk > 1440)
	{
		st_basic.mn_electrostatic_check_gap = 120;
		mstr_temp.Format("%d", st_basic.mn_electrostatic_check_gap);
		:: WritePrivateProfileString("BasicData", "Electric_Check", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_basic.mn_electrostatic_check_gap = mn_chk;

	:: GetPrivateProfileString("BasicData", "Electrostatic_Value", "0", chr_data, 10, st_path.mstr_basic);
	md_chk = atof(chr_data);
	if (md_chk < 1 || md_chk > 1440)
	{
		st_basic.md_electrostatic_value = 120;
		mstr_temp.Format("%d", st_basic.md_electrostatic_value);
		:: WritePrivateProfileString("BasicData", "Electrostatic_Value", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_basic.md_electrostatic_value = md_chk;

	// Data Logger에 있는 하루의 시작 시간을 가져온다. 같이 사용하니까.
	:: GetPrivateProfileString("DAY START TIME", "Time", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0 || mn_chk > 24)
	{
		st_basic.n_day_start_time = 22;
	}
	else  st_basic.n_day_start_time = mn_chk;
	st_basic.n_day_start_time = 22;

	// Data Logger에 있는 Shift 시간을 가져온다. 같이 사용하니까.
	:: GetPrivateProfileString("DAY SHIFT TIME", "Time", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0 || mn_chk > 24)
	{
		st_basic.n_day_shift_time = 8;
	}
	else  st_basic.n_day_shift_time = mn_chk;
	st_basic.n_day_shift_time = 8;

	:: GetPrivateProfileString("BasicData", "Day_Alarm_Count", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0)
	{
		alarm.mn_alarm_occured_day = 0;
		mstr_temp = LPCTSTR(_itoa(alarm.mn_alarm_occured_day, chr_buf, 10)) ;
		:: WritePrivateProfileString("BasicData", "Day_Alarm_Count", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  alarm.mn_alarm_occured_day = mn_chk;

	:: GetPrivateProfileString("BasicData", "Daily_Run_Time", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0)
	{
		st_handler.m_tDR = 0;
	}
	else  st_handler.m_tDR = mn_chk;

	:: GetPrivateProfileString("BasicData", "Daily_Stop_Time", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0)
	{
		st_handler.m_tDS = 0;
	}
	else  st_handler.m_tDS = mn_chk;

	:: GetPrivateProfileString("BasicData", "Daily_Alarm_Time", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0)
	{
		st_handler.m_tDJ = 0;
	}
	else  st_handler.m_tDJ = mn_chk;

	:: GetPrivateProfileString("BasicData", "Daily_Maint_Time", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0)
	{
		st_handler.m_tDM = 0;
	}
	else  st_handler.m_tDM = mn_chk;

	:: GetPrivateProfileString("BasicData", "Daily_MTBI_Time", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0)
	{
		st_handler.m_tDMTBI = 0;
	}
	else  st_handler.m_tDMTBI = mn_chk;

	:: GetPrivateProfileString("BasicData", "Data_Save_Time", "0", chr_data, 11, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0)
	{
		st_work.n_data_save_time = 0;
	}
	else  st_work.n_data_save_time = mn_chk;

	if ( st_work.iBasic_Apply_Check == CTL_YES )	// 베이직 어플라이 체크 20130524 kjh
	{
		st_work.iBasic_Apply_Check = CTL_NO;	// 베이직 어플라이 체크 20130524 kjh
	}
	else	// 베이직 어플라이 체크 20130524 kjh
	{
		Func.OnDayDataCheck();	// 베이직 어플라이 체크 20130524 kjh
	}

}

void CMyBasicData::OnBasic_Data_Save()
{
	// 백상현 추가 [2014.08.27]
	CString mstr_temp;  // 저장할 정보 임시 저장 변수
	char chr_buf[20];
	CString str_save_file;

	str_save_file	= st_path.mstr_path_dvc + st_basic.sDvcName;	//kwlee 20170922

	:: WritePrivateProfileString("BasicData", "DeviceType", LPCTSTR(st_basic.sDvcName), st_path.mstr_basic);
	
	// **************************************************************************
	// 장비 호기 및 장비 코드 로딩하여 전역 변수에 설정                          
	// -> 로딩 정보에서 앞/뒤 공백은 제거한다                                    
	// **************************************************************************
	:: WritePrivateProfileString("BasicData", "equip_no", LPCTSTR(st_basic.sEqpNo), st_path.mstr_basic);
	:: WritePrivateProfileString("BasicData", "equip_model", LPCTSTR(st_basic.sEqpModel), st_path.mstr_basic);
	
	// 자재 사용 모드 (0:Without, 1:With)
	mstr_temp = LPCTSTR(_itoa(st_basic.n_mode_device, chr_buf, 10));
	:: WritePrivateProfileString("BasicData", "Device_Mode", LPCTSTR(mstr_temp), st_path.mstr_basic);

	// 자재 사용 모드 (0:Without, 1:With)
	mstr_temp = LPCTSTR(_itoa(st_basic.n_mode_Network, chr_buf, 10));
	:: WritePrivateProfileString("BasicData", "Network_Mode", LPCTSTR(mstr_temp), st_path.mstr_basic);
	
	// BCR 동작 모드 [0:Step동작, 1:SelfTest]
	mstr_temp = LPCTSTR(_itoa(st_basic.nMode_BCR, chr_buf, 10));
	:: WritePrivateProfileString("BasicData", "BCR_Read_State", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp = LPCTSTR(_itoa(st_basic.nMode_SupplyMode, chr_buf, 10));
	:: WritePrivateProfileString("BasicData", "MasterSlave_Mode", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp = LPCTSTR(_itoa(st_basic.nMode_SupplyDevice, chr_buf, 10));
	:: WritePrivateProfileString("BasicData", "Supply_Device", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp = LPCTSTR(_itoa(st_basic.nMode_SMEMAUsing, chr_buf, 10));
	:: WritePrivateProfileString("BasicData", "SMEMA_Mode", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp = LPCTSTR(_itoa(st_basic.nMode_GripRetryCNT, chr_buf, 10));
	:: WritePrivateProfileString("BasicData", "GripRetryCNT", LPCTSTR(mstr_temp), st_path.mstr_basic);

	
	// yangkyu [2015.02.28]
	// Grip, Vac 사용 모드 추가
	mstr_temp = LPCTSTR(_itoa(st_basic.nMode_VacUsing, chr_buf, 10));
	:: WritePrivateProfileString("BasicData", "VacUsingMode", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp = LPCTSTR(_itoa(st_basic.nMode_GripUsing, chr_buf, 10));
	:: WritePrivateProfileString("BasicData", "GripUsingMode", LPCTSTR(mstr_temp), st_path.mstr_basic);

	// 규이리 추가 [2015.03.25]
	mstr_temp = LPCTSTR(_itoa(st_basic.nMode_PlaceType, chr_buf, 10));
	::WritePrivateProfileString("BasicData", "nMode_PlaceType", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp = LPCTSTR(_itoa(st_basic.nMode_Slop_Variable, chr_buf, 10));
	//::WritePrivateProfileString("BasicData", "nMode_Pitch_Variable", LPCTSTR(mstr_temp), st_path.mstr_basic);
	//kwlee 20170922
	::WritePrivateProfileString("BasicData", "nMode_Pitch_Variable", LPCTSTR(mstr_temp), str_save_file);
	
}

CString CMyBasicData::OnGet_File_Name()
{
	CString str_temp;       // 임시 저장 변수
	CString str_save_file;  // 저장 파일 임시 설정 변수
	CString str_chk_ext;    // 파일 확장자 저장 변수
	CString str_part, str_part2;
	CString str_new_save_file;
	int n_pos;
	
	// **************************************************************************
    // Socket Contact Count 데이터 저장할 파일 설정한다 [파일 확장자 검사]        
    // **************************************************************************
	str_save_file = st_path.mstr_path_dvc + st_basic.mstr_device_name;  // 티칭 데이터 저장 파일 설정
	
	n_pos = str_save_file.Find(".");  // 확장자 위치 검사
	
	if (n_pos == -1)
		str_save_file += _T(".TXT");  // 확장자 추가
	else 
	{
		str_chk_ext = str_save_file.Mid(n_pos);  // 파일 확장자 설정
		if (str_chk_ext != _T(".TXT"))  
		{
			str_save_file = st_path.mstr_path_dvc + _T("DEFAULT.TXT");  // 티칭 데이터 저장 새로운 파일 설정
			
			if (st_handler.cwnd_list != NULL)	// 리스트 바 화면 존재
			{
//				st_msg.mstr_abnormal_msg = _T("[DEVICE FILE] The error happened at a file extension.");
				sprintf(st_msg.c_abnormal_msg, "[DEVICE FILE] The error happened at a file extension.");
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
			}
		}
	}
	// **************************************************************************

	return str_save_file;  // 파일명 리턴 
}

CString CMyBasicData::OnGet_Teach_File_Name(CString strName)
{
	CString str_temp;       // 임시 저장 변수
	CString str_save_file;  // 저장 파일 임시 설정 변수
	CString str_chk_ext;    // 파일 확장자 저장 변수
	CString str_part, str_part2;
	CString str_new_save_file;
	CString str_device_name;
	int n_pos;
	
	n_pos = st_basic.mstr_device_name.Find(".");  // 확장자 위치 검사
	
	if (n_pos == -1)
	{
		st_basic.mstr_device_name += _T(".DAT");  // 확장자 추가
		n_pos = st_basic.mstr_device_name.Find(".");  // 확장자 위치 검사
	}
	
	str_device_name = st_basic.mstr_device_name.Mid(0, n_pos);
	
	
	if (strName.Find("Back") != -1)
	{
		str_save_file = st_path.mstr_path_back + str_device_name;  // 티칭 데이터 저장 파일 설정
	}
	else
	{
		str_save_file = st_path.mstr_path_dvc + str_device_name;  // 티칭 데이터 저장 파일 설정
	}
	
	if (strName != "")
	{
		str_save_file += "_";
		str_save_file += strName;
	}
	
	n_pos = str_save_file.Find(".");  // 확장자 위치 검사
	
	if (n_pos == -1)
	{
		str_save_file += _T(".DAT");  // 확장자 추가
	}
	else 
	{
		str_chk_ext = str_save_file.Mid(n_pos);  // 파일 확장자 설정
		if (str_chk_ext != _T(".DAT"))  
		{
			str_save_file = st_path.mstr_path_dvc + _T("DEFAULT.DAT");  // 티칭 데이터 저장 새로운 파일 설정
			
			if (st_handler.cwnd_list != NULL)	// 리스트 바 화면 존재
			{
				sprintf(st_msg.c_abnormal_msg, "[DEVICE FILE] The error happened at a file extension.");
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 오류 출력 요청
			}
		}
	}
	// **************************************************************************
	
	return str_save_file;  // 파일명 리턴 
}

int CMyBasicData::On_Teach_Data_Load()
{
	CString str_temp, str_msg, str_LoadFile;       // 임시 저장 변수
	CString sReadFileName;
	long l_chk;
	char chr_data[20];
	int i;
	
	sReadFileName	= st_basic.sDvcName;
	str_LoadFile	= st_path.mstr_path_dvc + sReadFileName;

	CString sM_Lift_Y[4] = { "Y_ACC_BOX_AVOID_","Y_ACC_FRONT_","Y_ACC_MIDDLE_","Y_ACC_REAR_" };
	CString sM_Lift_Z[4] = { "Z_ACC_LIFT_SAFETY_", "Z_ACC_LIFT_LIMIT_", "Z_ACC_LIFT_OFFSET_", "Z_ACC_LIFT_READY_" };

	CString sM_BCRRbt_X[2] = { "X_RBT_ACC_BCR_", "X_RBT_ACC_BCR_OFFSET_" };
	/*
	CString sM_BCRRbt_Y[7] = { "Y_RBT_SAFETY_", "Y_RBT_ACC_BCR_", "Y_RBT_ACC_GRIP_FRONT_", "Y_RBT_ACC_GRIP_REAR_",
		"Y_RBT_ACC_NG_OUT_", "Y_RBT_ACC_NG_BUFFER_", "Y_RBT_ACC_CONV_R_BUFF_"};
	*/
	// kilee [2015.03.17]
	CString sM_BCRRbt_Y[9] = { "Y_RBT_SAFETY_", "Y_RBT_ACC_BCR_", "Y_RBT_ACC_GRIP_FRONT_", "Y_RBT_ACC_GRIP_REAR_","Y_RBT_ACC_GRIP_BACK_",
		"Y_RBT_ACC_NG_OUT_", "Y_RBT_ACC_NG_BUFFER_", "Y_RBT_ACC_CONV_R_BUFF_", "Y_RBT_ACC_CONV_F_BUFF_"};
	CString sM_BCRRbt_Z[6] = { "Z_RBT_SAFETY_", "Z_RBT_ACC_GRIP_", "Z_RBT_ACC_NG_BUFFER_PLACE_",  "Z_RBT_ACC_NG_BUFFER_PICK_",
		"Z_RBT_ACC_BUFFER_CONV_PLACE_", "Z_RBT_ACC_BUFFER_CONV_PICK_" };
	CString sM_Rbt_Grip[2] = { "GRIP_RBT_OPEN_", "GRIP_RBT_GRIPPER_" };

	for (i=0; i<4; i++)
	{
		:: GetPrivateProfileString("M_LIFTER_Y", sM_Lift_Y[i], "0", chr_data, 10, str_LoadFile);
		l_chk = atol(chr_data);
		st_motor[M_LIFTER_Y].d_pos[i] = (double)l_chk;
	}

	for (i=0; i<4; i++)
	{
		// yangkyu 중국 출장시 수정 사항
		// GetPrivateProfileString("M_LIFTER_Z1" 값과 st_motor[M_LIFTER_Z3].d_pos[i]값에 들어가는 명이 동일하게 수정해야함
		:: GetPrivateProfileString("M_LIFTER_Z3", sM_Lift_Z[i], "0", chr_data, 10, str_LoadFile);
		l_chk = atol(chr_data);
		st_motor[M_LIFTER_Z3].d_pos[i] = (double)l_chk;

		:: GetPrivateProfileString("M_LIFTER_Z1", sM_Lift_Z[i], "0", chr_data, 10, str_LoadFile);
		l_chk = atol(chr_data);
		st_motor[M_LIFTER_Z1].d_pos[i] = (double)l_chk;

		:: GetPrivateProfileString("M_LIFTER_Z4", sM_Lift_Z[i], "0", chr_data, 10, str_LoadFile);
		l_chk = atol(chr_data);
		st_motor[M_LIFTER_Z4].d_pos[i] = (double)l_chk;

		:: GetPrivateProfileString("M_LIFTER_Z2", sM_Lift_Z[i], "0", chr_data, 10, str_LoadFile);
		l_chk = atol(chr_data);
		st_motor[M_LIFTER_Z2].d_pos[i] = (double)l_chk;
	}

	for (i=0; i<2; i++)
	{
		:: GetPrivateProfileString("M_BCRREAD_ROBOT_X", sM_BCRRbt_X[i], "0", chr_data, 10, str_LoadFile);
		l_chk = atol(chr_data);
		st_motor[M_BCRREAD_ROBOT_X].d_pos[i] = (double)l_chk;
	}
	
	//for (i=0; i<7; i++)
	// kilee [2015.03.17]
	for(i=0; i<9; i++)
	{
		:: GetPrivateProfileString("M_BCRREAD_ROBOT_Y", sM_BCRRbt_Y[i], "0", chr_data, 10, str_LoadFile);
		l_chk = atol(chr_data);
		st_motor[M_BCRREAD_ROBOT_Y].d_pos[i] = (double)l_chk;
	}
	
	for (i=0; i<6; i++)
	{
		:: GetPrivateProfileString("M_BCRREAD_ROBOT_Z", sM_BCRRbt_Z[i], "0", chr_data, 10, str_LoadFile);
		l_chk = atol(chr_data);
		st_motor[M_BCRREAD_ROBOT_Z].d_pos[i] = (double)l_chk;
	}


	return 1;
}

void CMyBasicData::On_Teach_Data_Save()
{
	CString str_temp, str_SaveFile;			// 임시 저장 변수
	CString sSaveFileName;
	
	char chr_buf[20];
	
	sSaveFileName	= st_basic.sDvcName;
	str_SaveFile	= st_path.mstr_path_dvc + sSaveFileName;
	
	int i;

	CString sM_Lift_Y[4] = { "Y_ACC_BOX_AVOID_","Y_ACC_FRONT_","Y_ACC_MIDDLE_","Y_ACC_REAR_"};
	CString sM_Lift_Z[4] = { "Z_ACC_LIFT_SAFETY_", "Z_ACC_LIFT_LIMIT_", "Z_ACC_LIFT_OFFSET_", "Z_ACC_LIFT_READY_" };

	CString sM_BCRRbt_X[2] = { "X_RBT_ACC_BCR_", "X_RBT_ACC_BCR_OFFSET_" };
	/*
	CString sM_BCRRbt_Y[7] = { "Y_RBT_SAFETY_", "Y_RBT_ACC_BCR_", "Y_RBT_ACC_GRIP_FRONT_", "Y_RBT_ACC_GRIP_REAR_",
		"Y_RBT_ACC_NG_OUT_", "Y_RBT_ACC_NG_BUFFER_", "Y_RBT_ACC_CONV_R_BUFF_"};
	*/
	// kilee [2015.03.17]
	CString sM_BCRRbt_Y[9] = { "Y_RBT_SAFETY_", "Y_RBT_ACC_BCR_", "Y_RBT_ACC_GRIP_FRONT_", "Y_RBT_ACC_GRIP_REAR_","Y_RBT_ACC_GRIP_BACK_",
		"Y_RBT_ACC_NG_OUT_", "Y_RBT_ACC_NG_BUFFER_", "Y_RBT_ACC_CONV_R_BUFF_", "Y_RBT_ACC_CONV_F_BUFF_"};
	CString sM_BCRRbt_Z[6] = { "Z_RBT_SAFETY_", "Z_RBT_ACC_GRIP_", "Z_RBT_ACC_NG_BUFFER_PLACE_",  "Z_RBT_ACC_NG_BUFFER_PICK_",
		"Z_RBT_ACC_BUFFER_CONV_PLACE_", "Z_RBT_ACC_BUFFER_CONV_PICK_" };
	CString sM_Rbt_Grip[2] = { "GRIP_RBT_OPEN_", "GRIP_RBT_GRIPPER_" };
	
	for (i=0; i<4; i++)
	{
		str_temp = LPCTSTR(_itoa((int)(st_motor[M_LIFTER_Y].d_pos[i]), chr_buf, 10));
		:: WritePrivateProfileString("M_LIFTER_Y", sM_Lift_Y[i], LPCTSTR(str_temp), str_SaveFile);
	}

	for (i=0; i<4; i++)
	{
		// yangkyu 중국 출장시 수정 사항
		// WritePrivateProfileString("M_LIFTER_Z1" 값과 st_motor[M_LIFTER_Z3].d_pos[i]값에 들어가는 명이 동일하게 수정해야함
		str_temp = LPCTSTR(_itoa((int)(st_motor[M_LIFTER_Z3].d_pos[i]), chr_buf, 10));
		:: WritePrivateProfileString("M_LIFTER_Z3", sM_Lift_Z[i], LPCTSTR(str_temp), str_SaveFile);

		str_temp = LPCTSTR(_itoa((int)(st_motor[M_LIFTER_Z1].d_pos[i]), chr_buf, 10));
		:: WritePrivateProfileString("M_LIFTER_Z1", sM_Lift_Z[i], LPCTSTR(str_temp), str_SaveFile);

		str_temp = LPCTSTR(_itoa((int)(st_motor[M_LIFTER_Z4].d_pos[i]), chr_buf, 10));
		:: WritePrivateProfileString("M_LIFTER_Z4", sM_Lift_Z[i], LPCTSTR(str_temp), str_SaveFile);

		str_temp = LPCTSTR(_itoa((int)(st_motor[M_LIFTER_Z2].d_pos[i]), chr_buf, 10));
		:: WritePrivateProfileString("M_LIFTER_Z2", sM_Lift_Z[i], LPCTSTR(str_temp), str_SaveFile);
	}

	for (i=0; i<2; i++)
	{
		str_temp = LPCTSTR(_itoa((int)(st_motor[M_BCRREAD_ROBOT_X].d_pos[i]), chr_buf, 10));
		:: WritePrivateProfileString("M_BCRREAD_ROBOT_X", sM_BCRRbt_X[i], LPCTSTR(str_temp), str_SaveFile);
	}

	//for (i=0; i<7; i++)
	// kilee [2015.03.17]
	for(i=0; i<9; i++)
	{
		str_temp = LPCTSTR(_itoa((int)(st_motor[M_BCRREAD_ROBOT_Y].d_pos[i]), chr_buf, 10));
		:: WritePrivateProfileString("M_BCRREAD_ROBOT_Y", sM_BCRRbt_Y[i], LPCTSTR(str_temp), str_SaveFile);
	}

	for (i=0; i<6; i++)
	{
		str_temp = LPCTSTR(_itoa((int)(st_motor[M_BCRREAD_ROBOT_Z].d_pos[i]), chr_buf, 10));
		:: WritePrivateProfileString("M_BCRREAD_ROBOT_Z", sM_BCRRbt_Z[i], LPCTSTR(str_temp), str_SaveFile);
	}

}


void CMyBasicData::On_Network_Data_Load()
{
	CString str_temp;			// 임시 저장 변수
	char chr_data[100];
	int i, n_Chk = 0;
	CString sPeerIP[1] = { "Main_Address"};
	CString sPeerPort[1] = { "Main_Port"};
	
	for (i=0; i<1; i++)
	{
		:: GetPrivateProfileString("Server_Network_Info", sPeerIP[i], "0", chr_data, 100, st_path.mstr_network);
		str_temp = chr_data;
		if (str_temp == "")
		{
			n_Chk++;
		}
		else
		{  
			st_client[i].str_ip = str_temp;	
		}
		
		:: GetPrivateProfileString("Server_Network_Info", sPeerPort[i], "0", chr_data, 100, st_path.mstr_network);
		str_temp = chr_data;
		if (str_temp == "")
		{
			n_Chk++;
		}
		else
		{
			st_client[i].n_port = atoi(str_temp);
		}
	}
}
void CMyBasicData::On_Network_Data_Save()
{
	
}
