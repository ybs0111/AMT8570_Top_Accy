// Public_Function.h: interface for the CPublic_Function class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PUBLIC_FUNCTION_H__692EA0CF_F10D_4B4E_A14A_1CD148AC075B__INCLUDED_)
#define AFX_PUBLIC_FUNCTION_H__692EA0CF_F10D_4B4E_A14A_1CD148AC075B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MainFrm.h"

#include "Variable.h"		// 전역 변수 정의 클래스
#include <wininet.h>
#include <string>
#include <vector>
#include "Ctlbd_Variable.h"	// Added by ClassView

class CMainFrame;
class CDialog_Part_IO;
class CDialog_Motor_Part;
class CDialog_Part_Manual;
class CDialog_Manual_Part_1;
class CDialog_Manual_Part_2;
class CDialog_Manual_Part_3;
class CDialog_Manual_Part_4;

class CPublic_Function    : public CObject
{
public:
	DECLARE_SERIAL(CPublic_Function);
	CPublic_Function();
	virtual ~CPublic_Function();

public:
	CDialog_Part_IO		*m_p_io;
	CDialog_Motor_Part		*m_p_motor_part;
	CDialog_Part_Manual		*m_p_manual;
	CDialog_Manual_Part_1	*mp_manual_1;
	CDialog_Manual_Part_2	*mp_manual_2;
	CDialog_Manual_Part_3	*mp_manual_3;
	CDialog_Manual_Part_4	*mp_manual_4;

	COleDateTime		m_oledate[2];
	COleDateTime		m_oletime[2];
	double dTime[6][2];

	bool CYLFlag[MAX_IO];
	long CYLTime[MAX_IO][3];
	bool SiteFlag[MAX_IO];
	long SiteTime[MAX_IO][3];
	bool IOFlag[MAX_IO];
	long IOTime[MAX_IO][3];
	int Mot_HomeStep[MOT_MAXMOTOR];
	int Mot_RetryCount;
	int Mot_CurTime[MOT_MAXMOTOR][3];
	int Mot_RetryFlag[MOT_MAXMOTOR];

	int reinstatement_Step;
	long backuptime[3];
	int mn_check[2];
	int mn_stamp_change;
	
	CString strFile;
	int nGetPCBInfoStep;
	
	HINTERNET m_hOpen;
	HINTERNET m_hConnect;
	
	long NetworkConnectTime[3];
	long NetworkingTime[3];
	long TesterCommTime[3];
	char cJamcode[10];

	CString strFTPFileName[100];
	int nFTPFileCheck;
	int nCheckTemp;

	CString strQuery;			//쿼리문을 담을 변수 

	CString Itemsortnum;		//정렬조건을 담을 문자열 변수
	
	CString str_O_Pulse, str_N_Pulse;

	int mn_transfer_change;
	int mn_picker_change;
	int mn_multistacker_change;
	int mn_xy_robot_change;
	
	int m_nLastYear;
	int m_nLastMonth;
	int m_nLastDay;
	int m_nLastHour;

	double md_ms_target_pos;
	double md_ms_current_pos;

	//linear move setting 
	int		 mn_linearmove_index;   //현재 map index 
	long     ml_AxisCnt;            //현재의 IndexNum에서 사용 가능한 모터 수량 최대 4개  
	long	 mlp_AxisNum[4];        //현재의 IndexNum에서 사용하는 모터 실재 번호를 가진다 
	double	 mdp_PosList[4];		   //최대 4 포인트를 한번에 이동가능하다 
 	double   mdp_SpdRatio[3];        //속도비율 [0]:Vel, [1]:Acc, [2]:Dec

	int m_iRfidPos[50];
	int	m_nRfidNum[50];

	CMainFrame						*pMainFrm;

	// 규이리 추가 [2013.11.29]
	int m_nCngStatus;
	
public:
	int  OnIsInterruptCondition();	// 시컨스 인터럽트 조건 확인

	void OnReset_FlagStructure();	// 구조체의 플래그 정보 초기화
	void OnReset_FlagInitialize();	// [초기화] 각종 변수
	void OnReset_FlagInterLock();	// [초기화] 초기화 작업시의 인터락 변수
	void OnReset_FlagSyncVar();		// [초기화] SYNC 플래그
	void OnReset_FlagLotVar();		// [초기화] LOT 관련 플래그
	void OnReset_StructMapInfo();	// [초기화] 구조체의 각종 맵 정보
	int  OnIsAllRcvyComplete();	// 전체 사이트 복구 동작 완료 여부 확인

	void OnSet_UpStreamSmema(int nzCase);	// [상류 설비] SMEMA 상태 설정
	int  OnGet_UpStreamSmema();	// [상류 설비] 상대편 SMEMA 상태 반환

	void OnSet_MotorUnitDist();	// 모터 환경 파라메터 설정

	int OnHexToDecimal( CString strHex );

	double OnTimeSearch(int nType);
	void CalcTime(int count);
	int OnNumberStringExtraCheck(CString strText, int nCount[3]);
	void Robot_BackData_TT_Set(int movepos);
	void Set_DualActionOnOff(int nIONumber, int nIONumber2, int nOnOff);
	void Set_DoubleActionOnOff(int nIONumber, int nIONumber2, int nOnOff );
	int Get_DoubleActionOnOff(int nIONumberT, int nIONumber, int nIONumber2, int nOnOff, int nTime, int nOption = 1);
	int Get_SingActionOnOff(int nIONumberT, int nIONumber, int nOnOff, int nTime, int nOption = 1);
	int Get_SingActionOnOff_Pass_OnIO(int nIONumberT, int nIONumber, int nOnOff, int nTime, int nOption = 1);	// 20130605 어라인 더블액션 온 아이오 패스(요청) kjh
	void Set_SingleActionOnOff(int nIONumber, int nOnOff, int nOption = 0);
	void Handler_Recovery_Data_Write();
	void GlobalDataSerialize(CArchive &ar);
	void Robot_BackData_Z_Set(int movepos);
	void OnDayDataReset();
	void OnDayDataCheck();
	void OnDailyDataSave();
	int Find_SitePosData(int nSite, int nRowcolmode, int nBinnum, int nDvcyesno, int nCountmode);
	int OnRetestModuleRemainCheck();
	CString OnRetestModuleCheck(int nPos, int nLast = 0);
	void DateTimeChange(CString sDateTime);
	char GetNtPrivilege();
	void SetActionParam();
	void MakeLogFolder(CString BaseDirectory,const CString strMsg);
	void DB_Write_Alarm(st_alarm_database st_alarmdb);
	void DB_Write_Time(st_time_database st_timedb);
	void TextAndLine(int dir, CString str_sdata, CString str_data, FILE *fp);
	void ThreadFunctionStepTrace(int nPos, int nStep);
	void Robot_BackData_Set(int site, int type, int xpos, int ypos, int noption = -1);
	void Robot_BackMovePosValCheck();
	int Robot_Reinstatement_Position(int mode);
	CString GetLocalMacAddress();
	int FileSizeCheck(CString FileName, long size, int n_check);
	CString OnDeviceInfoAnalysis(CString strBody, int nInfo);
	CString OnNetworkBodyAnalysis(CString strBody, CString strFind, int nOpt = 0);
	//int Handler_Recovery_Data(int n_Mode);
	int Handler_Recovery_Data(int n_debug, int n_Site, int n_Mode);
	
	void OnTrace_ThreadStep(int nzPos, int nzStep);	// 쓰레드 스텝 정보 TRACE

	int On_Encoder_Check(double d_pos);
	int OnMotor_Go_Check(int n_motor, double d_pos);
	int DoorOpenCheckSpot();
	CString GetDoorName(int nPos);
	void OnSet_MotorSpeed();	// 모터 구동 속도 설정

	CString OnGet_MotorName(int nzMotorNo);	// 모터 이름 반환
	void IO_Map_Initialize();
	BOOL CreateFolder(LPCTSTR szPath);
	void MsgLog( LPCTSTR strMsg );
	CString GetStrCurrentTime();
	void OnLogRecord_Data( int iPos, int iWhere, bool bSet, CString strWhere );
	static void DeleteOldFiles( LPCTSTR lpDirPath, int iDays );

	void OnHour_Pass_Count();	// 랏 페스 카운트 1시간마다 저장 함수 20130531 kjh
	void OnHour_Pass_Count_Init();	// 20130603 시간당 수량 kjh
	void UldStackerLotIdCntSave(int Num);	// Lot id 및 카운트 저장 20130726 kjh
	void UldStackerLotIdCntLoad(int Num);	// Lot id 및 카운트 불러오기 20130726 kjh
	void UldStackerDelete();				// 20130917 kjh


	int  OnInit_IOBoard();	// I/O 보드 이니셜 작업
	void OnSet_MappingIO();	// I/O 매핑


	// 규이리 추가 [2013.06.13]
	int OnCheckBoardVerify();
	int OnConvertStr2Dec(int nzPortNo);
	int OnGetScale(int nzSlaveNo);
	
	void OnSet_EqpStatusRun();	// [RUN] 설비 상태 변경
	void OnSet_EqpStatusStop();	// [STOP] 설비 상태 변경
	void OnSet_EqpStatusAlarm(int nzMode);	// [ALARM] 설비 상태 변경
	void OnSet_EqpStatusInit();	// [INIT] 설비 상태 정보 변경
	
	void OnSet_BtnLampStop();	// [STOP] 버튼 램프 상태 변경
	void OnSet_BuzzerSound(int nzStatus, int nzType=0);	// 알람 부저 소리 On/Off 동작
	
	void OnSet_EqpManual();	// [매뉴얼 모드] 설정 및 스위치 램프 변경
	void OnSet_EqpAuto();	// [오토 모드] 설정 및 스위치 램프 변경
	
	int  OnIsDoorOpen();	// 도어 열림 상태 조사 [0:모두 닫힘, 0이상:열린 도어 존재]
	
	void OnClear_OutPort();	// [초기화] 출력 포트 상태
	
	void OnStop_AxisAll();	// 전체 모터축 긴급 정지

	// 규이리 추가 [2014.12.16]
	int  OnWatchdogDoor();	// 도어 열림 상태 조사 [0:모두 닫힘, 0이상:열린 도어 존재]
	void OnMon_Door();	// 도어의 [열림/닫힘] 상태 모니터링
	void OnOpenDoor();	// 도어 열기
	void OnCloseDoor();	// 도어 닫기
	CString OnCheck_DoorState();	// 도어의 [열림/닫힘] 상태 체크

	void OnIsAllForceDischargeComplete();
	// 규이리 추가 [2015.01.03]
	void OnMon_EMO();	// EMO 스위치 눌림 상태 모니터링
	void OnMon_MC();	// MC 전원 떨어진 상태 모니터링

	void OnSet_EqpLoadingStart();
	void OnSet_EqpLoadingManual();

	// 규이리 추가 [2015.03.14]
	int  OnGet_PickerStatus(int nzSite, int nzYesNo, int nzPickerInfo[DATA_TYPE_][MAX_PICKER_]);	// 악세사리 존재 상태 반환

	void OnLogBCRData(CString sz_Step);

private:
	int AreaSensor_Time[MOT_MAXMOTOR][3];
	int AreaSensor_Flag[MOT_MAXMOTOR];

	double d_motiondone_Time[MOT_MAXMOTOR][3];
	int n_motiondone_Flag[MOT_MAXMOTOR];

	int n_barcodepos_check;

	long ml_door_check[3];
	int mn_door_check_flag;

	CString str_step_temp[100];
	CTime  StepChangeTime[100];

	int mn_cylinder_flag[1800];
	long ml_cylinder_time[1800][3];
};
#endif // !defined(AFX_PUBLIC_FUNCTION_H__692EA0CF_F10D_4B4E_A14A_1CD148AC075B__INCLUDED_)
