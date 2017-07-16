// CtlBd_Library.h: interface for the CCtlBd_Library class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CTLBD_LIBRARY_H__95978C35_3D8D_44B9_A9DF_E9F5E1C39135__INCLUDED_)
#define AFX_CTLBD_LIBRARY_H__95978C35_3D8D_44B9_A9DF_E9F5E1C39135__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//장비의 가동상태 정보 셋팅  
#define CTL_dSTOP			0
#define CTL_dRUN			1
#define CTL_dJAM			2
#define CTL_dLOTEND			3
#define CTL_dINIT			4
#define CTL_dWARNING		5
#define CTL_dLOCK			6
#define CTL_dSELFCHECK		7
#define CTL_dMAINT			8

// 규이리 추가 [2014.08.27]
#define MAX_RETRY_			3

class CCtlBd_Library  
{
public:
	CCtlBd_Library();
	virtual ~CCtlBd_Library();

public:

	//linear move  james 2012.0509 
	int mn_linear_motmove_step[8]; //2012.0307 모터 이동스탭을 저장 ->[100]은 모터 100개를 정의 
	int mn_linear_retry_cnt[8]; //각 모터별 리트라이 횟수 
	int mn_linear_max_retry_cnt;  //모든 모터의 최종 리트라이 리미트 횟수 


	int  m_nStep_Single[100];


	int mn_single_motmove_step[100]; //2012.0307 모터 이동스탭을 저장 ->[100]은 모터 100개를 정의 
	int mn_mot_retry_cnt[100]; //각 모터별 리트라이 횟수 
	int mn_mot_max_retry_cnt;  //모든 모터의 최종 리트라이 리미트 횟수 

	char mc_normal_msg[1000]; //장비에서 발생하는 메세지를 저장한다 

	int mn_run_status;	//장비의 가동 정보 셋팅 
	
	char mc_alarmcode[10]; //alarm code 정의
	int mn_alarm_type;	//알람의 종류를 셋팅[예)operator error 인지, maint 성 error 인지 셋팅]
	int mn_alarm_count; //알람 카운트 여부 셋팅 [0:카운트하지 않음, 1:카운트 함]
	int mn_jampos; //jam 이 발생한 위치의 정보를 저장한다 

public:
	int Linear_Move(int n_LinearMapIndex, long l_AxisCnt, long *lp_AxisNum, double *dp_PosList, double *dp_SpdRatio);

	void OnReset_SingleMoveStep(int nzMotorNum);	// SingleMove 스텝 정보 초기화
	int  OnSingleMove(int nzMotorNum, double dzMovePos, int nzSpeedRate=100);	// 입력된 위치로 모터 이동

	BOOL CreateFolder(LPCTSTR sz_path);  //folder 생성 필요시 사용 
	int FileSizeCheck(char * cp_filename, int n_size, int n_check);
	int LogFile_Write(char * cp_dir_path, char * cp_msg);

	
	int Initialize_motor_board(int n_bd_type, CString s_filename);	//motor 보드 초기화시 사용
	int Motor_SafetyCheck(int nzReqMode, int nzAxis, double dzTargetPos);	// 모터 구동 가능 여부 확인 (인터락 조건)
	int OnCheck_MovePossible(int nzMode, int nzAxis, double dzTargetPos);	// 모터별 인터락 조건 체크

	int Alarm_Error_Occurrence(int n_jampos, int n_run_status, char c_alarmcode[10]); //장비 알람 발생시킬때 사용하는 변수 
	int Alarm_Error_Occurrence( int n_jampos, int n_run_status, CString strJamCode );

};
extern CCtlBd_Library CTL_Lib;


#endif // !defined(AFX_CTLBD_LIBRARY_H__95978C35_3D8D_44B9_A9DF_E9F5E1C39135__INCLUDED_)
