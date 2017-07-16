// CtlBd_Library.h: interface for the CCtlBd_Library class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CTLBD_LIBRARY_H__95978C35_3D8D_44B9_A9DF_E9F5E1C39135__INCLUDED_)
#define AFX_CTLBD_LIBRARY_H__95978C35_3D8D_44B9_A9DF_E9F5E1C39135__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//����� �������� ���� ����  
#define CTL_dSTOP			0
#define CTL_dRUN			1
#define CTL_dJAM			2
#define CTL_dLOTEND			3
#define CTL_dINIT			4
#define CTL_dWARNING		5
#define CTL_dLOCK			6
#define CTL_dSELFCHECK		7
#define CTL_dMAINT			8

// ���̸� �߰� [2014.08.27]
#define MAX_RETRY_			3

class CCtlBd_Library  
{
public:
	CCtlBd_Library();
	virtual ~CCtlBd_Library();

public:

	//linear move  james 2012.0509 
	int mn_linear_motmove_step[8]; //2012.0307 ���� �̵������� ���� ->[100]�� ���� 100���� ���� 
	int mn_linear_retry_cnt[8]; //�� ���ͺ� ��Ʈ���� Ƚ�� 
	int mn_linear_max_retry_cnt;  //��� ������ ���� ��Ʈ���� ����Ʈ Ƚ�� 


	int  m_nStep_Single[100];


	int mn_single_motmove_step[100]; //2012.0307 ���� �̵������� ���� ->[100]�� ���� 100���� ���� 
	int mn_mot_retry_cnt[100]; //�� ���ͺ� ��Ʈ���� Ƚ�� 
	int mn_mot_max_retry_cnt;  //��� ������ ���� ��Ʈ���� ����Ʈ Ƚ�� 

	char mc_normal_msg[1000]; //��񿡼� �߻��ϴ� �޼����� �����Ѵ� 

	int mn_run_status;	//����� ���� ���� ���� 
	
	char mc_alarmcode[10]; //alarm code ����
	int mn_alarm_type;	//�˶��� ������ ����[��)operator error ����, maint �� error ���� ����]
	int mn_alarm_count; //�˶� ī��Ʈ ���� ���� [0:ī��Ʈ���� ����, 1:ī��Ʈ ��]
	int mn_jampos; //jam �� �߻��� ��ġ�� ������ �����Ѵ� 

public:
	int Linear_Move(int n_LinearMapIndex, long l_AxisCnt, long *lp_AxisNum, double *dp_PosList, double *dp_SpdRatio);

	void OnReset_SingleMoveStep(int nzMotorNum);	// SingleMove ���� ���� �ʱ�ȭ
	int  OnSingleMove(int nzMotorNum, double dzMovePos, int nzSpeedRate=100);	// �Էµ� ��ġ�� ���� �̵�

	BOOL CreateFolder(LPCTSTR sz_path);  //folder ���� �ʿ�� ��� 
	int FileSizeCheck(char * cp_filename, int n_size, int n_check);
	int LogFile_Write(char * cp_dir_path, char * cp_msg);

	
	int Initialize_motor_board(int n_bd_type, CString s_filename);	//motor ���� �ʱ�ȭ�� ���
	int Motor_SafetyCheck(int nzReqMode, int nzAxis, double dzTargetPos);	// ���� ���� ���� ���� Ȯ�� (���Ͷ� ����)
	int OnCheck_MovePossible(int nzMode, int nzAxis, double dzTargetPos);	// ���ͺ� ���Ͷ� ���� üũ

	int Alarm_Error_Occurrence(int n_jampos, int n_run_status, char c_alarmcode[10]); //��� �˶� �߻���ų�� ����ϴ� ���� 
	int Alarm_Error_Occurrence( int n_jampos, int n_run_status, CString strJamCode );

};
extern CCtlBd_Library CTL_Lib;


#endif // !defined(AFX_CTLBD_LIBRARY_H__95978C35_3D8D_44B9_A9DF_E9F5E1C39135__INCLUDED_)
