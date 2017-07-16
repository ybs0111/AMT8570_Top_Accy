#ifndef _VARIABLE_h__
#define _VARIABLE_h__

#include "Alg_Mysql.h"
#include "CtlBd_Variable.h"

#define DEBUG_FLAG				1

#define DEVELOPER_PC			0
#define BARCODE_USED			1

/////////////////////////////////////////////////////////
//DLL 구조로 변경예정 //2011.1007 추가 
/////////////////////////////////////////////////////////
#define BD_NO		0  //어떤 조건이 클리어된 상태 
#define BD_YES		1  //어떤 조건을 요청한 상태

// 규이리 추가 [2014.08.26]

#define NO						0		//어떤 조건이 클리얻횐 상태 
#define YES						1		//어떤 조건을 요청한 상태 
#define READY					2		//어떤 조건이 욫어한 후 상대방이 준비를 완료한 상태  


//////////////////////////////////////////////////////////////////////////////////////////////////////////
//motor, i/O 보드등의 리턴값은 이 것만 사용한다  
//////////////////////////////////////////////////////////////////////////////////////////////////////////
#define BD_ERROR    -1
#define BD_PROCEED  0
#define BD_GOOD		1
#define BD_RETRY	2
#define BD_SAFETY	3

#define TRAY_X                  0
#define TRAY_Y                  1

#define SOCKET_NUM	20
#define PICKER_NUM	2
#define PICKER_GAP	2
#define PICKER_BUFFER_GAP	1
#define RETEST_NUM	40
#define LOB_NUM		180
#define REJECT_NUM	30

#define INFO_DATA	4

#define MAX_TRAY_Y		10
#define MAX_TRAY_X		5

#define MAX_SITE		20	//각 위치별 사이트 정보	0:Input, 1:Loader, 2:Empty, 3:Uld 1, 4:Uld 2, 5:Uld 3, 6:Multi Stacker 1, 7:Multi Stacker 2, 8:Multi Stacker 3, 
#define DATA_INFO		10	//[0 ] ~ [7] 까지 사용 

#define CONNECT_NO				0
#define CONNECT_YES				1

#define MC_OFF_LINE				0
#define MC_ON_LINE_LOCAL		1
#define MC_ON_LINE_REMOTE		2

#define MC_STOP					0
#define MC_RUN					1
#define MC_ALARM				2
#define MC_WARNING				3
#define MD_IDLE					4

#define FORCEDISCHARGE_OFF		0
#define FORCEDISCHARGE_ON		1

#define WITH_MODE				0
#define WITHOUT_MODE			1
#define CONTINUITY_MODE			0
#define STEP_MODE				1	

#define MASTER_MODE				0
#define SLAVE_MODE				1

#define DEVICE_NONE				0
#define DEVICE_2_5INCH_BTM		1
#define DEVICE_DESKTOP_BTM		2
#define DEVICE_MSATA_BTM		3
#define DEVICE_DESKTOP_TOP		12
#define DEVICE_MSATA_TOP		13

#define SMEMA_USING				0
#define SMEMA_NOT_USING			1

#define GRIP_USING				0
#define GRIP_NOT_USING			1

#define VAC_USING				0
#define VAC_NOT_USING			1

#define OFF_LINE				0
#define ON_LINE					1

#define COMM_TIME_OUT				5000

enum pNetworkID
{
	NID_EC							= 0,
	NID_FRONT,
	NID_EC_LOT_DISPLAY,
	NID_EC_ALARM,
	NID_NEXT,
	NID_CIM,
	NID_RFID,
};

enum pWaitTime
{
	T_STACKER_LONG_UPDN				 = 0,
	T_STACKER_SHORT_UPDN,
	T_STACKER_LATCH_ONOFF,
	T_STACKER_STOPPER_UPDN,
	T_TRAY_UPDN,
	T_TRAY_STOPPER_UPDN,
	T_TRAY_ALIGN_ONOFF,
	T_MULTI_STACKER_PUSHER_ONOFF,
	T_ROBOT_ROTATE_ONOFF,
	T_ROBOT_PICKER_UPDN,
	T_ROBOT_PICKER_VACUUM,
	T_TRAY_TRANSFER_UPDN,
	T_TRAY_TRANSFER_ONOFF,
	T_TRAY_LIFTER_UPDN,
	T_CONVEYOR_STOPPER_LOB,			// 맨앞
	T_CONVEYOR_STOPPER_PLATE,		// 중간
	T_CONVEYOR_STOPPER_STACKER,		// 스테커
	T_SITE_3_TRAY_WAIT,
	T_TOTAL,
};

enum pCylinderOnOff
{
	CYLINDER_OFF = 0,
	CYLINDER_ON,
	CYLINDER_ERROR,
};

enum pCylinderUpDn
{
	CYLINDER_DOWN = 0,
	CYLINDER_UP,
};

enum pInitPart
{
 	INIT_LOAD_CONV			= 0,
 	INIT_BOX_CLAMP,
 	INIT_BOX_LIFT,
 	INIT_XYZ_ROBOT,
 	INIT_BUFFER_CONV,
	MAXSITE,
};

///////////////////////////////////////
///////////////////////////////////////
// 모터 이름 및 위치 8500
///////////////////////////////////////
enum pMotorName
{
	M_ROBOT_X = 				0,
	M_ROBOT_Y,
	M_ROBOT_Z,
	M_ROBOT_P,
	M_TRAY_TRANSFER_Y,
	M_MULTI_STACKER_Z,
	M_MOTOR_COUNT,
};
///////////////////////////////////////

#define COUNT_NO                0    // 초기 자재 갯수 및 공간 갯수 검사 
#define COUNT_YES               1    // 전체 갯수 및 공간 갯수 검사 

#define INFO_SKIP               0    // 해당 데이터 셋팅하지 않음 
#define INFO_SETTING            1    // 해당 데이터 셋팅함 
#define INFO_ADJUST			    2    // Adjust용.

#define PICKDVC                 0
#define PLACEDVC                1
#define ADJUSTDVC               2

#define DVC_EMPTY               0

#define BIN_CLEARBIN			-1
#define BIN_PASS				1
#define BIN_REJECT				2
#define BIN_HTO					3
#define BIN_NTO					4
#define BIN_BTO					5
#define BIN_RETEST				6
#define BIN_BP					7
#define BIN_ALL					10

#define COUNT_TEST				0

#define RET_PICKERNOTFIND       6		// BIN에 해당되는 결과 미존재 
#define RET_NOWHERE             -1		// 해당 데이터 미존재 
#define RET_NOSPACE             -2		// 공간 미존재 
#define RET_LOADTRAYSKIP        -100	// LOADER OFF / 강제 LOT END 


#define PICKER_DVC_CNT          7
#define PICKER_ENABLE_INFO      8		// 피커 정보 확인 

#define MAIN_BUFFER_DSP			34
#define MAIN_RETEST_DSP			35
#define MAIN_REJECT_DSP			36
#define MAIN_LOB_TRAY_DSP		37
#define MAIN_TRAY_DSP			38
#define MAIN_PICKER_INFO_DSP	39
#define MAIN_CUSTOMER_DSP		40
#define MAIN_AIR_BLOW			67		

#define LOT_A					0
#define LOT_B					1
#define LOT_C					2

// *****************************************************************************
// 사용자 정의 메시지 ID 선언                                                   
// *****************************************************************************
#define WM_FORM_CHANGE				WM_USER+200   // 화면이 전환된 때 해당 전환 상태 정보를 전송하기 위한 사용자 정의 메시지 
#define WM_STATUS_CHANGE			WM_USER+201   // 타이틀 바 화면으로 전송된 메시지 출력시키기 위한 사용자 정의 메시지 추가 
#define WM_LIST_DATA				WM_USER + 6   // 통신 메시지 출력 리스트 대화 상자에 

#define WM_DATA_SEND				WM_USER + 7   // Data를 전송하기 위한 화면에서 정보를 전송하기 위해 정의한 사용자 정의 함수
#define WM_WORK_END					WM_USER + 8   // 테스트 작업 완료 시 정보를 메인 화면에 출력시키기 위한 사용자 정의 메시지 추가 
#define WM_SCRAP_DSP				WM_USER + 9

#define WM_BASIC_APPLY				WM_USER + 10  // BASIC DATA를 APPLY 시키기 위한 사용자 정의 메시지 추가 
#define WM_MAINTENANCE_APPLY		WM_USER + 11  // Maintenance DATA를 APPLY 시키기 위한 사용자 정의 메시지 추가
#define WM_TESTREFERENCE_APPLY		WM_USER + 12
#define WM_INTERFACE_APPLY			WM_USER + 13
#define	WM_WAITTIME_APPLY			WM_USER + 14  // Wait Time DATA를 APPLY 시키기 위한 사용자 정의 메시지 추가 

#define	WM_MOTORTEACH_APPLY			WM_USER + 15
#define	WM_SHIFTER_APPLY			WM_USER + 16
#define	WM_MOTORSPEED_APPLY			WM_USER + 17

#define WM_SERIAL_PORT				WM_USER + 18
#define WM_NETWORK_MSG				WM_USER + 19

#define WM_MAINFRAME_WORK			WM_USER + 20
#define WM_WORK_MSG					WM_USER + 21

#define DIAGNOSE_START				WM_USER + 22
#define DIAGNOSE_END				WM_USER + 23

#define SELF_DIAGNOSE_END			WM_USER + 25
#define	WM_MAIN_EVENT				WM_USER + 26
#define WM_TRACKOUT_DSP				WM_USER + 27
// 규이리 추가 [2014.10.09]
#define WM_UPDATE_MAIN				WM_USER	+ 28   // Main Screen Update REQ

#define WM_RECEIVED_NEW_LOT			WM_USER + 36
#define WM_DRAW_LOT_TIME			WM_USER + 37
#define WM_DRAW_WORK_SITE			WM_USER + 38

enum ENUM_WM_MESSAGE
{
	WM_DRAW_UI_MAIN				= WM_USER + 39,
	WM_DRAW_DATA_MAIN,
	WM_MOTOR_COMPLETION,
	WM_SOCKET_ACCEPT,
	
	WM_INIT_BTN,

	WM_DRAW_DATA_XGEM,
};

enum ENUM_DRAW_UI_MAIN
{
	EDDM_ALL,
	EDDM_TRAY_TRANSFER,
	EDDM_TRAY_TRANSFER_REAL_POS,
	EDDM_LOTID,
	
	EDUM_LOADER_PLATE,
	EDUM_EMPTY_PLATE,
	EDUM_BIN_1_PLATE,
	EDUM_BIN_2_PLATE,
	EDUM_BIN_3_PLATE,

	EDDC_SITE,	
	EDDT_INIT,
	EDDT_SEL_TRAY,
	EDDR_PICKER,

	EDDR_HIDE_CONTROL,
	EDDN_NETUSABLE,
};

#define PGM_BASE_				(CString)"C:\\AMT8570_Accy\\"
#define BMP_PATH_BASE_			PGM_BASE_+(CString)"Bmp\\"
#define EXCEL_PATH_BASE_		PGM_BASE_+(CString)"ExcelMap\\"
#define SETTING_PATH_BASE_		PGM_BASE_+(CString)"Setting\\"
#define IMG_PATH_				PGM_BASE_+(CString)"Bin\\res\\"

#define FILE_IOMAP_				(CString)"AMT8570_Accy_IO_MAP.xls"
#define FILE_IO_PART_			(CString)"AMT8570_Accy_IO_PART_MAP.xls"
#define FILE_MOT_AXIS_			(CString)"AMT8570_Accy_MOTOR_AXIS_MAP.xls"
#define FILE_MOT_PART_			(CString)"AMT8570_Accy_MOTOR_PART_MAP.xls"

#define FILE_WAITTIME_MAP_		(CString)"AMT8570_Accy_WAITTIME_MAP.xls"	// 화면 구성
#define FILE_MOT_SPEED_			(CString)"MotorSpeed.TXT"
#define FILE_WAITTIME_			(CString)"WaitTime.TXT"

#define WITHOUT_DVC_		0
#define WITH_DVC_			1

#define VAR_INIT_			0

//==================================================================//
// 소켓 통신을 위해 추가함
// 규이리 추가 [2013.11.12]
//==================================================================//
#define WM_CLIENT_MSG			WM_USER +500
#define WM_CLIENT_MSG_1			WM_USER +500
#define WM_CLIENT_MSG_2			WM_USER +501
#define WM_CLIENT_MSG_3			WM_USER +502
#define WM_CLIENT_MSG_4			WM_USER +503
#define WM_CLIENT_MSG_5			WM_USER +504
#define WM_CLIENT_MSG_6			WM_USER +505
#define WM_CLIENT_MSG_7			WM_USER +506
#define WM_CLIENT_MSG_8			WM_USER +507
#define WM_CLIENT_MSG_9			WM_USER +508
#define WM_CLIENT_MSG_10		WM_USER +509

#define WM_SERVER_MSG			WM_USER +550
#define WM_SERVER_MSG_1			WM_USER +550
#define WM_SERVER_MSG_2			WM_USER +551
#define WM_SERVER_MSG_3			WM_USER +552
#define WM_SERVER_MSG_4			WM_USER +553
#define WM_SERVER_MSG_5			WM_USER +554
#define WM_SERVER_MSG_6			WM_USER +555
#define WM_SERVER_MSG_7			WM_USER +556
#define WM_SERVER_MSG_8			WM_USER +557
#define WM_SERVER_MSG_9			WM_USER +558
#define WM_SERVER_MSG_10		WM_USER +559

enum SOCKET_CLIENT
{
	CLIENT_CONNECT		= 0,
	CLIENT_CLOSE		= 1,
	CLIENT_SEND			= 2,
	CLIENT_REV			= 3,
	// 규이리 추가 [2013.11.27]
	CLIENT_RECONNECT	= 4,
};

enum SOCKET_SERVER
{
	SERVER_CONNECT		= 0,
	SERVER_CLOSE		= 1,
	SERVER_SEND			= 2,
	SERVER_REV			= 3,
	SERVER_CLIENT_COMM	= 4,
};

// 규이리 추가 [2014.08.14]
enum COMM_PARAM
{
	REQ_NONE_		= 0,
	REQ_CALL_		= 1,
	REQ_STOP_		= 2,
	REPLY_ERROR_	= 3,
	REPLY_CORRECT_	= 4,
};

typedef struct comm_param_info
{
	char chSendCmd[100];	// 전송할 메시지 헤더 정보 (문자열)
	char chResponse[100];	// 수신할 메시지의 헤더 정보 (문자열)
	// 전송할 메시지에 대한 에코 여부 및 상태 정보
	// - [0:default, 1:정상, -1:에러, 10:에코필요, 11:에코불필요]
	int nEcho;
	// 전송할 메시지에 대한 응답 기다림 여부 및 상태 정보
	// - [0:default, 1:정상, -1:에러, 10:응답존재(기다림), 11:응답없음]
	int nResponse;
	int nTimeOut;	// 타임아웃 검사할지 여부 [0:검사불필요, 1:검사필요]
	
	char chReceiveCmd[100];	// 수신된 메시지 정보 (문자열)
}comm_param;


struct st_client_param
{
	CString str_ip;	// 소켓 IP 주소
	int n_port;
	int n_connect;		// 연결 상태 정보
	
	int n_rev_length;	// 수신 문자열 길이
	char ch_rev[4096];	// 수신 문자열
	char ch_send[4096];	// 송신 문자열

	char chSendHead[40];	// 전송할 헤더 정보
	char chSendBody[4096];	// 전송할 바디 정보
	
	// 규이리 추가 [2014.11.01]
	int nIndex;	// 전송할 메시지의 인덱스 번호
	comm_param	stComm[10];
};
extern st_client_param	st_client[10];

struct st_server_param
{
	CString str_ip;	// 소겟 IP 주소
	int n_port;
	int n_connect;	// 연결 상태 정보
	
	int n_send_length;	// 송신 문자열 길이
	int n_rev_length;	// 수신 문자열 길이
	char ch_rev[4096];	// 수신 문자열
	char ch_send[4096];	// 송신 문자열
};
extern st_server_param	st_server[10];

enum
{
	TYPE_WORD = 0,
	TYPE_BYTE = 1,
};


// *****************************************************************************
//  Main Screen 출력용 메세지 정의                                              
// *****************************************************************************
enum ENUM_MAIN_SCR_MSG
{
	MAIN_TIMEINFO,
	MAIN_COUNTINFO,
	MAIN_DSP_UPH,
};

// *****************************************************************************
#define LANGUAGE_KOREA		0		// 한글 OS
#define LANGUAGE_ENGLISH	1		// 영어 OS


/* ****************************************************************************** */
/* RS-232C 시리얼 통신 시 사용되는 메시지 정의                                    */
/* -> MAX_PORT : 기본 포트 2개로 구성되어 있다						              */
/* ****************************************************************************** */
#define MAX_PORT	1		// 생성할 시리얼 포트 갯수
#define COM_ERROR	-1		// 에러 메시지

#define BCR_PORT	1		// 현재 BCR READ 포트 번호
#define TESTER_PORT	2		// 현재 Tester Communication 포트 번호

#define BCR_ECHO	8883	//barcode parsing echo

#define BUF_SIZE	1024		// 버퍼 크기

#define PORT_BCR_ACCY_		1

// 규이리 추가 [2012.10.04]
#define MAX_BUF_LEN			4095

enum LOAD_STOP
{
	LOAD_STOP_NONE	= 0,
	LOAD_STOP_YES	= 1,
	LOAD_STOP_NO	= 2,
};

// *************************************************************************
// 기타 정보 저장 구조체 선언                                                    
// *************************************************************************
struct st_other_param 
{
	CString str_op_msg;			// 리스트 화면 출력 동작 에러 메시지 저장 변수
	CString str_normal_msg;		// 리스트 화면 출력 동작 에러 메시지 저장 변수
	CString str_abnormal_msg;	// 리스트 화면 출력 동작 에러 메시지 저장 변수
	
	CString str_fallacy_msg;	// 오류 메시지 대화 상자 타이틀 저장 변수
	CString str_typing_msg;		// 키보드 대화 상자 타이틀 저장 변수
	
	CString str_pass_level;		// 암호 레벨 저장 변수
	CString str_password;		// 암호 저장 변수
	
	CString str_parent_menu;	// 주메뉴 이름 저장 변수
	
	char c_normal_msg[MAX_BUF_LEN];
	char c_abnormal_msg[MAX_BUF_LEN];
	
	////////////////////////////////2K10/02/09/ViboX////////////////////////////////////////
	int		n_prev_step[100];			// 이전 step (변경 되었는지 보기 위함)
	CString	str_history_step[100];		// step_history 약 50개에 해당하는 step을 저장
	CString str_last_change_date[100];	// 최종 step으로 변경되는 시점의 시간
};
extern  st_other_param  st_other;
// *************************************************************************

enum
{
	DLG_SHOW_	= 0,
	DLG_HIDE_	= 1,
};

// 규이리 추가 [2015.03.14]
#define MAX_PICKER_ 			4
// [0:체크할 자재유무, 1:실재 자재존재]
#define DATA_TYPE_				2
#define TYPE_FLAG_				0
#define TYPE_SEN_				1

#define IO_OFF				0
#define IO_ON				1
#define IO_ERROR			2
#define IO_CHK_ERROR		4

#define TMR_MOT_IO_CHECK			2
#define TMR_MOT_POS_CHECK			3
#define TMR_MOT_HOME_CHECK			4
#define TMR_MOT_MOVE_CHECK			5
#define TMR_MOT_MOVE_TEST			6
#define TMR_CYL_IO_CHECK			7
#define TMR_CYL_IO_TEST				8
#define TMR_IO_CHECK				9
#define TMR_BD_ENABLE				10
#define TMR_BD_DISABLE				11

// 메뉴얼 화면에서 사용한다.
#define MOT_MOVE			1
#define MOT_HOME			2

#define HOMING				1
#define MOVING				2
#define MULTI_MOVING		3
#define POS_MOVING			4

// *****************************************************************************
//  메뉴에서 사용할 ID
// *****************************************************************************
#define MENU_SETTING		0
#define MENU_MOTOR			1
#define MENU_LIST			2

// ******************************************************************************
// 장비 동작 제어 시 필요한 메시지 정의                                          
// ******************************************************************************
#define dSTOP			0
#define dRUN			1
#define dJAM			2
#define dLOTEND			3
#define dINIT			4
#define dWARNING		5
#define dLOCK			6
#define dIDLE			7
#define dMAINT			8

#define eWARNING		0
#define eRUNNING		1
#define eSTOP			2
#define eERROR			3

// ******************************************************************************
// 악세사리 박스 Loading 제어 시 필요한 메시지 정의                                          
// ******************************************************************************
#define dMANUAL			0
#define dSTART			1

// *****************************************************************************
//  Title Bar 출력용 메세지 정의                                                
// *****************************************************************************
#define FORM_NAME			1
#define MACHINE_STATUS		2
#define RETEST_MODE			3
#define DEVICE_MODE			4
#define LEVEL_MODE			5
#define FRONT_KEY_MODE		6
#define REAR_KEY_MODE		7
#define MACIHNE_MODE		8	// inline, s/a mode 20130822 kjh


// *****************************************************************************
//  List Box 출력용 메세지 정의                                                 
// *****************************************************************************
#define RECEIVE_MSG			1
#define SEND_MSG			2
#define NORMAL_MSG			3
#define ABNORMAL_MSG		4

#define MACHINE_INFO		5
#define DEVICE_INFO			6

#define TCPIP_MSG			7
#define BARCODE_MSG			8

#define TCPMSG				100
#define TCPCONNECT			11
#define TCPDISCONNECT		12
#define TCPIPCHANGE			13
#define SOCKETCLOSE			14

#define NETWORK_SND_MSG		15
#define NETWORK_RCV_MSG		16

#define RETURN_MSG			17
#define SERVER_CONNECT		18
#define SERVER_MSG			19

// ******************************************************************************
// I/O 모듈 갯수 및 포트 최대 갯수 선언한다                                      
// ******************************************************************************
#define MAX_MODULE		6
#define MAX_MAP			32
#define MAX_IN			716
#define MAX_IO			716
// ******************************************************************************

#define ELEC_PORT		5


// ******************************************************************************
//  MOTOR 보드 관련 변수 선언한다                                                
// ******************************************************************************
#define MOT_GOOD		1
#define MOT_PROGRESS	2
#define MOT_RETRY		3
#define MOT_ERROR		4

#define PLUS			1
#define MINUS			0
#define RUN_MODE		1
#define MANUAL_MODE		2

#define AMPOFF			0
#define AMPON			1

#define ELM				1
#define ELP				2
#define HOME			0

#define MOT_ACC			0
#define MOT_DEC			1
#define MOT_JOG			2
#define MOT_ORG			3
#define MOT_FEEDRATE	4
#define MOT_INTERACC	5

#define MOT_FAST		10
#define MOT_SLOW		20

#define T_NOTTEST		0
#define T_PRIMETEST		1
#define T_RETEST		2

// SOCKET DATA관련.
#define LOT_DATA			0	// 해당 LOT에만...
#define DAY_DATA			1	// 당일
#define MONTH_DATA			2	// 당월
#define SHIFT_DATA			3	// 당 Shift

#define MAX_SOCKET_POS	160
#define TEST_END_OK		1000

//MOT Speed Setting  
#define	MOT_SPD_VEL			0
#define	MOT_SPD_ACC			1
#define MOT_SPD_DEC			2
#define MOT_SPD_JOG			3
#define MOT_SPD_ORG			4
#define MOT_SPD_FEEDRATE	5
#define MOT_SPD_INTERACC	6

//motor sensor 
#define MOT_SENS_HOME		0
#define MOT_SENS_ELM		1
#define MOT_SENS_ELP		2
#define MOT_SENS_SD			3 //slow down sensor 

//모터 방향 정의 
#define MOT_PLUS			1
#define MOT_MINUS			0

enum
{
	POS_NONE = -1,
	POS_COUNT = 2,
};

enum { SOCKET_COUNT = 20 };
enum { BOARD_COUNT = 10 };

#define MOT_RTY_CNT			4	//파스텍 제어기 관련 문제시 재 시도하는 횟수

#define MOT_MOVE_LIMITTIME	60000	//이동 후 3초이내동작 완료할 것 	 
#define MOT_TIMEOUT			90000   //1분의 time out를 가지고 동작한다  //홈 체크시 사용 

//에러등 메세지 지정시사용 
#define MOT_NORMAL_MSG		0
#define MOT_ERR_MSG			1
#define MOT_ERR_CODE		2

#define MOT_LmMAPINDEX			0	//모터 리스트 모션 정의 
#define MOT_IxMAPINDEX			1	//모터 직선 보간 정의 

#define RET_PROCEED		0
#define RET_GOOD		1
#define RET_ERROR		2
#define RET_RETRY		3
#define RET_DOOROPEN	4		// Door Open이 되어 있음. 2K6/06/20/ViboX

#define WORK_INIT				-1
#define	WORK_READY				0
#define	WORK_START				1
#define	WORK_END				2
#define	WORK_MOVE				3
#define	PGM_START				4
#define	WORK_PROCEED			5
#define	WORK_OFFLINE_START		6
#define	WORK_OFFLINE_END		7
#define	WORK_OFFLINE_PROCEED	8

///////////////////////////////////////////
// Control Board(IO, Motor) Board Define
///////////////////////////////////////////

//091010 I/O 동작시 Out/In 동작 체크시 패턴을 아래와 같이 정의하여 동작 완료 여부를 정확히 확인하자 
#define CTL_IOCHK_CLEAR				0 //I/O 동작을 시작하는 위치(Output 시그널 동작시 사용)
#define CTL_IOCHK_START				1 //I/O 아웃풋 동작을 완료하여 기다리는 시간이 시작하는 위치(Input 체크 조건이 처음 만족 하는 위치) 
#define CTL_IOCHK_END				2 //I/O 동작을 input 대기 시간까지 모두 완료하여 끝난 상태
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define HANDLER_BACKUP_DATA_RW	29
#define DATA_READ    0
#define DATA_WRITE   1

// 칼라 정의
#define SKY_C					RGB(187, 250, 255)
#define SKY_D					RGB(0, 255, 255)
#define SKY_L					RGB(128, 255, 255)
#define NOR_C					RGB(164, 164, 164)
#define NOR_L					RGB(190, 190, 190)
#define RED_C					RGB(255, 0, 0)
#define RED_L					RGB(255, 150, 150)
#define RED_D					RGB(80, 20, 0)
#define RED_A					RGB(200, 0, 0)
#define BLUE_C					RGB(0, 0, 255)
#define WHITE_C					RGB(255, 255, 255)
#define BLACK_C					RGB(25, 1, 1)
#define BLACK_L					RGB(62, 55, 55)
#define YELLOW_C				RGB(255,255,0)
#define YELLOW_D				RGB(128, 128, 0)
#define YELLOW_L				RGB(255, 255, 128)
#define GREEN_C					RGB(0,255,0)
#define GREEN_B					RGB(0,255,64)
#define GREEN_L					RGB(150,255,150)
#define GREEN_D					RGB(0,100,20)
#define DSERBLE_BC				RGB(210,210,230)
#define TEXT_BC					RGB(220,230,210)
#define OK_BC					RGB(0, 0, 255)
#define NG_BC					RGB(255, 0, 0)
#define RESULT_BC				RGB(40, 1, 1)
#define ORANGE_C				RGB(0xFF, 0x68, 0x20)
#define ORANGE_L				RGB(255, 200, 128)
#define BLACK					RGB(0, 0, 0)
#define BLUE_D					RGB(0, 0, 128)
#define BLUE				    RGB(0, 0, 255)
#define GRAY_C					RGB(192, 192, 192)
//#define BLUE_L					RGB(150, 150, 255)
#define BLUE_L					RGB(124, 139, 252)
#define ON_C					RGB(0, 255, 0)
#define OFF_C					RGB(0, 150, 0)
#define NG_C					0x0000FF
#define OK_C					0xFF00
#define BLACK_GC				0x00
#define YELLOW_GC				0xFFFF
#define TEST_BC					RGB(194, 231, 150)
#define IN_BC					RGB(204, 153, 255)
#define PINK_D					RGB(252, 233, 218)

#define DVC_READY_C				RGB(153, 153, 255)
#define DVC_LOAD_C				RGB(239, 214, 198)
#define DVC_PGM_C				RGB(255, 255, 0)
#define DVC_UNLOAD_C			RGB(255, 255, 204)
#define DVC_NONE_C				RGB(150, 150, 150)
#define DVC_START_C				RGB(150, 255, 150)
#define DVC_NO_C				RGB(128, 128, 0)

//Grid 정의 
#define DATE					0
#define EDIT					1
#define COMBO					2
#define BUTTON					3
#define STATIC					4
#define TIME					5
#define PERCENT					6
#define CHECK1					7
#define NUMBER					8
#define NUMBER1					9

#define IO_IN					1
#define IO_OUT					0
#define IO_NONE					2



// 규이리 추가 [2014.07.30]
enum
{
	REQ_NONE	= 0,
	REQ_YES		= 1,
};

//==================================================================//
// 로그 타입 정의
// : 규이리 추가 [2012.02.10]
//==================================================================//
#define LOG_SOCKET_				0x01
#define LOG_SEQ_				0x02
#define LOG_COMM_				0x04
#define LOG_PGM_				0x08
#define LOG_BCR_				0x10
#define LOG_SUPPLY_COUNT_		0x20

// 규이리 추가 [2013.11.28]
#define CONFIG_PATH_BASE_		(CString)"d:\\AMTLOG"
 

#define MAX_EMO_			2

// *****************************************************************************
// 화면 동작 관련 기본 데이터 저장 구조체 변수 선언                             
// *****************************************************************************
struct st_handler_param
{
	int nFlag_InitComplete;
	
	int nSelectedMenuNum;	// 현재 선택된 메뉴 ID
	int nShowAlarmDlg;		// 알람 화면 출력되어져 있는지 정보

	int  nStopPushInInitial;	// 초기화 작업 중 STOP 버튼 눌렸는지 여부
		
	BOOL b_program_exit;			//프로그램이 빠져나가는건지 확인한다.
	BOOL b_CalledAlarmEditor;

	int mn_language;				// 어떤 언어의 OS냐.
	
	// *************************************************************************
	// 화면에 대한 핸들 정보 저장 변수 선언                                     
	// *************************************************************************
	HWND hWnd;
	CWnd *cwnd_title;		// 타이틀바 핸들
	CWnd *cwnd_list;		// 리스트바 핸들

	CWnd *cwnd_main;		// 메인 화면 핸들
	CWnd *cwnd_basic;		// 베이직 화면 핸들
	CWnd *cwnd_motor;		// 모터 화면 핸들
	CWnd *cwnd_motorAxis;	// 모터 축별 화면 핸ㄷ르


	// *************************************************************************
	// 장비 상태 정보 설정 변수 선언                                            
	// *************************************************************************

	CString mstr_pass_level;
	int mn_level_maint;				// 메인트 레벨(Level 1) 설정 플래그
	int mn_level_teach;				// 티칭 레벨(Level 2) 설정 플래그
	int mn_level_speed;				// 스피드 레벨(Level 3) 설정 플래그
	int mn_level_admin;				// Admin 레벨(Level 4) 설정 플래그

	int mn_mot_brd_initial;			// 모션 보드 초기화 작업 완료 플래그
	int mn_io_board_initial;

	int mn_home_state[MOT_MAXMOTOR];	// 모터 HOMING 완료 플래그
	int mn_init_state[MAXSITE];		// 초기화 완료 플래그
	int mn_lotend_state[MAXSITE];		// 초기화 완료 플래그

	double md_run_speed;			// 장비 동작시 속도비
	double md_manual_speed;			// 메뉴얼 동작시 속도비

	CString mstr_handler_name;		// 핸들러 이름.
	CString mstr_handler_version;	// 핸들러 버젼
	CString mstr_handler_date;		// 핸들러 버젼 수정 날짜.

	CString mstr_last_alarm;
	int mn_status;					// 장비가 가동중이냐
	int mn_reinstatement;

	int mn_safety;

	int mn_board_onoff_dialog;

	CTimeSpan m_tR;		//Run Time
	CTimeSpan m_tJ;		//Jam Time
	CTimeSpan m_tS;		//Stop Time
	CTimeSpan m_tM;		//Maint Time
	CTimeSpan m_tT;		//Test Time
	CTimeSpan m_tPT;	//Prev Test Time
	CTimeSpan m_tIT;	//Index Time
	CTimeSpan m_tPIT;	//Prev Index Time
	CTimeSpan m_tCT;	//Cycle Time
	CTimeSpan m_tPCT;	//Prev Cycle Time
	CTime m_tRef;
	
	CTimeSpan m_tDR;	//Run Time
	CTimeSpan m_tDRB;	//Run Time
	CTimeSpan m_tDRW;	//Run Time
	CTimeSpan m_tDJ;	//Jam Time
	CTimeSpan m_tDS;	//Stop Time
	CTimeSpan m_tDM;	//Maint Time
	CTimeSpan m_tDMTBI;	//Run Time

	long lTimeTable[3];
	 
	int mn_run_time_check;			// Run Time으로 해야 할것이냐 아니냐.
	int mn_runtime_dll_check;		// 장비 저장용 변수를 한번만 세팅하기 위해서... 2K7/03/15/ViboX

	int mn_motor_init_check;
	int mn_initiailze_end_flag;
	int n_initiailze;

	int mnSerial_Port_Creating[MAX_PORT];

	int mn_reverse_mode;
	int mn_virtual_mode;

	// 규이리 추가 [2013.02.14]
	int nInitTryREQ;		// 초기화 진행 요청 [READY:플래그 초기화, YES:초기화 요청, NO:초기화 SKIP]
	int nIsAllSiteInitEnd;  // 모든 초기화 작업 완료 여부
	
	// 규이리 추가 [2014.02.19]
	int nStart_INIT;	// 초기화 작업 시작중인지 여부
	
	CString sEqpVersion;

	int  nMenuLock;	// 메뉴 사용 가능 여부
	int  nSysLock;	// 시스템 lock 여부
	int  nSwitchLock;
	
	int  nModeManual;

	int nRecoveryReq;

	// 규이리 추가 [2014.12.16]
	int nDetect_DoorOpen[16];
} ;
extern  st_handler_param  st_handler;
// *****************************************************************************

enum enum_Motor_Speed
{
	SPD_HOME =				0,
	SPD_JOG,
	SPD_N_MOVE,
	SPD_S_MOVE,
	SPD_F_MOVE,
};

// *****************************************************************************

// *****************************************************************************
//  장비 부위간 Sync 관련 변수
// *****************************************************************************

#define RCVY_MAX_SITE_		20

enum RECOVERY_SITE
{
	SITE_ACCY_CONV_	= 0,
	SITE_BOX_CLAMP_	= 1,
	SITE_BOX_LIFT_	= 2,
	SITE_XYZ_ROBOT_	= 3,
	SITE_BUFFER_CONV= 4,
};

enum WORK_STATUS
{
	SYNC_REQ_RESET_				= 0,
	SYNC_REQ_WORK_				= 1,
	SYNC_REQ_WORK_COMPLETE_		= 2,
	SYNC_REQ_ACCY_BOX_LOADING_COMPLETE_ = 3,
	SYNC_REQ_ACCY_BOX_CLAMPING_COMPLETE_ = 4,
	SYNC_REQ_BOX_REMOVE_		= 5,
	SYNC_REQ_ACC_LIFT_COMPLETE_		= 6,
	SYNC_REQ_BUFFER_LOADING_READY_		= 7,
	SYNC_REQ_ACCY_REALIGN				= 8,
	SYNC_REQ_PASS_						= 9,

	SYNC_RESP_RESET_			= 21,
	SYNC_RESP_WORK_				= 22,
	SYNC_RESP_WORK_COMPLETE_	= 23,
	SYNC_RESP_BOX_REMOVE_COMPLETE = 24,
	SYNC_RESP_BOX_REMOVE_READY = 25,
	SYNC_RESP_BOX_LIFT_CHANGE_COMPLETE_ = 26,
	SYNC_RESP_LOADING_ = 27,
	SYNC_RESP_PASS_ = 28,
	SYNC_RESP_REMOVE_ = 29,
};

struct tagSYNC_PARAM
{
	int nRcvyComplete[RCVY_MAX_SITE_];	
	
	int nReq_LoadingConv2Conv2_Work;
	int nReq_LoadingConv2Clamp_Work;//[2];//Conv2-> Conv1 Loading conv box

	int nResp_Clamp2LoadingConv_Work;//Box Pusher Clamp->Conv Req
	
	int nReq_Clamp2Lifter_Work;
	int nResp_Lifter2Clamp_Work;
	
	int nReq_Lifter2XYZRbt_Work;
	int nResp_XYZRbt2Lifter_Work;
	
	int nReq_XYZRbt2BufferAlaignConv_Work;
	int nResp_BufferAlaignConv2XYZRbt_Work;
	
	
	int nLotEnd_LoadingConv[2];
	int nLotEnd_BoxClamp;
	int nLotEnd_BoxLifter;
	int nLotEnd_XYZRobot;
	int nLotEnd_BufferAlignConv;

	int nForceDischargeComplete[5];
	
};
extern tagSYNC_PARAM	stSync;

#define INPUT_ACC_BOX_COUNT		2

struct tagWITHOUT_PARAM
{
	int nTestBox;
	int nWorkTestBox;
	int nLoadingTestBox;
	int BCR_State[4];
	int BCR_Count;
	int NGBuffer[4];
	int nBufferConv[4];
	int nBufferConvPath[2];
};
extern tagWITHOUT_PARAM	stWithoutData;

// *****************************************************************************

// *****************************************************************************
//  메세지 관련
// *****************************************************************************
struct st_message_param
{
	CString mstr_infor_msg;
	
	int mn_pad_dialog;
	int mn_dot_use;
	CString mstr_pad_high_limit;
	CString mstr_pad_low_limit;
	CString mstr_keypad_val;

	CString mstr_keypad_msg;
	CString mstr_keyboard_msg;

	CString str_fallacy_msg;		// 오류 메세지.
	CString mstr_confirm_msg;		// 확인 메세지
	CString mstr_typing_msg;		// 키보드 메세지

	CString mstr_parent_menu;

	CString mstr_password;

	CString mstr_barcode;

	// List에 출력 하다가 죽는 현상을 막기 위해서... 2K11/09/01/ViboX
	char c_normal_msg[5000];
	char c_abnormal_msg[5000];

	CString mstr_normal_prev_msg;
	CString mstr_abnormal_prev_msg;

	CString mstr_send;
	CString mstr_recive;

	CString mstr_rpy_msg[10];

	int mn_event_msg_list;
	CString mstr_event_msg[3];

	CString str_step_savefile_name;
	CString str_step_savefile_path;

	char n_select_buffer;
} ;
extern  st_message_param  st_msg;
// *****************************************************************************

// ******************************************************************************
// 파일 경로명 저장 구조체 변수 선언                                             
// ******************************************************************************
struct st_path_param 
{
	CString mstr_run_state;		// 20130830 kjh;
	CString mstr_basic;			// 기본 셋팅 정보 저장 폴더+파일명 설정 변수
	CString mstr_network;			// Network 정보 저장 폴더+파일명 설정 변수
	CString mstr_basic_folder;	// 기본 셋팅 정보 저장 폴더+파일명 설정 변수
	CString mstr_datalogger;	// Data Logger의 폴더+파일명 설정 변수
		
	CString mstr_path_dvc;		// 디바이스별 티칭 정보 저장 파일 생성 폴더 설정 변수
	CString mstr_path_alarm;	// 출력 알람 정보 존재 폴더 설정 변수
	CString mstr_path_op;		// 오퍼레이터 정보 저장 폴더 설정 변수
	CString mstr_path_tray;		// Tray별 정보 저장 파일 생성 폴더 설정 변수
	CString mstr_path_serial;	// Socket 정보 저장 파일 생성 폴더 설정 변수
	
	CString mstr_path_file;		// 파일 생성 폴더 설정 변수
	CString mstr_path_back;		// BACKUP 파일 생성 폴더 설정 변수
	
	CString mstr_file_basic;	// 기본 셋팅 정보 저장 파일명 설정 변수
	CString mstr_file_wait;		// 대기 시간 정보 저장 파일명 설정 변수
	
	CString mstr_operation;		// Operation Log 파일 폴더 설정.
	CString mstr_interface;		// Interface Log 파일 폴더 설정.
	CString mstr_machine;		// Machine Log 파일 폴더 설정.
	CString mstr_barcode;		// Barcode Log 파일 폴더 설정.
	CString mstr_barcode_fail;	// Barcode Log 파일 폴더 설정.
	CString mstr_gpib;			// Total Log 파일 폴더 설정.
	CString mstr_total;			// Total Log 파일 폴더 설정.
	CString mstr_galmg;
	CString str_log_path;
	CString str_lot_data_path;
	CString str_lot_pass_data_path;
	CString str_daily_data_path;
	CString str_daily_pass_count;	//20130531 kjh
	CString mstr_tcpip_front;
	CString mstr_tcpip;
	CString mstr_tcpip_ec_server;
	

	CString mstr_motor;


	CString sIOMap;		// IO MAP 파일 위치 폴더.
	CString sIOPartMap;
	CString sMotPartMap;
	CString sMotAxisMap;
	CString sWaitTimeMap;
	CString sMotSpeed;
	CString sWaitTime;
};
extern  st_path_param  st_path;
// ******************************************************************************

#define MAX_WAIT_TIME_NUM	20

enum time_type_enum
{
	CYL_ACCY_BOX_STOPPER		= 0,
// 	CYL_ACCY_BOX_LR_GRIPPER		= 1,
	CYL_ACCY_CONV_BOX_CLAMP		= 1,
	CYL_ACCY_CONV_BOX_PUSHER	= 2,
	CYL_ACCY_BOX_CLAMP			= 3,//align
	CYL_ACCY_BOX_VAC_GRIPPER	= 4,//vacuum
	CYL_ACCY_BOX_SUPPLY			= 5,
	CYL_ACCY_BOX_UD_GRIPPER		= 6,
	CYL_ACCY_BOX_LIFTER			= 7,
	CYL_ACCY_BOX_PUSHER			= 8,
	CYL_ACCY_ROBOT_PICKER		= 9,
	CYL_ACCY_ROBOT_VAC			= 10,
	CYL_ACCY_ROBOT_EJECT		= 11,
	CYL_ACCY_ROBOT_PITCH		= 12,
	CYL_ACCY_BUFF_IN_STOPPER	= 13,
	CYL_ACCY_BUFF_PATH_ALIGN	= 14,
	CYL_ACCY_BUFF_PITCH			= 15,
	WAIT_RAIL_AC_OFF			= 16,  // [대기 시간만 존재]
	WAIT_ACCY_BOX_NOT_CHK_AC_OFF = 17,
	WAIT_ACCY_OUT_AC_OFF = 18,
};

// ******************************************************************************
// I/O 동작 대기 시간 및 리미트 시간 저장 구조체 선언                            
// ******************************************************************************
struct st_time_param 
{
	int nWaitTime[MAX_WAIT_TIME_NUM][2];
	int nLimitTime[MAX_WAIT_TIME_NUM];

	// 규이리 추가 [2014.09.01]
	int nWait_On[MAX_WAIT_TIME_NUM];
	int nWait_Off[MAX_WAIT_TIME_NUM];
	int nWait_Limit[MAX_WAIT_TIME_NUM];
} ;
extern  st_time_param  st_time;
// ******************************************************************************

// *****************************************************************************
//  Tower Lamp 화면에 대한 정보 저장 구조체 변수 선언                           
// *****************************************************************************
struct st_lamp_param
{
	CString mstr_equip_no;		// 장비 호기 저장 변수
	CString mstr_equip_id;	// 장비 코드 저장 변수
	CString mstr_ec_equip_id;	// 장비 코드 저장 변수	// 20130822 kjh
	int mn_lamp_change_time;	// 램프 변경 시간 저장 변수
} ;
extern  st_lamp_param  st_lamp;
// *****************************************************************************

// ******************************************************************************
// 베이직 화면 환경 설정 정보 저장 구조체 선언                                   
// ******************************************************************************
struct st_basic_param 
{
	int n_mode_device;
	int n_retry_cnt;
	
	int n_day_start_time;		// Data 기록용.. 하루의 시작 시간...
	int n_day_shift_time;		// Data 기록용.. Shift 시간...
			
	CString mstr_device_name;	// 선택된 디바이스 종류 저장 변수
	int mn_alarm_delay_time;
	CString mstr_recipe_name;	// 선택된 디바이스 종류 저장 변수
	
	int mn_electrostatic_check_gap;
	double md_electrostatic_value;

	int mn_network_wait_time;		// Network 대기 시간
	int mn_network_retry_cnt;		// Network Retry 횟수

	// [0] => 복구모드 저장 상태 ( 0:저장되지 않은 상태, 1:복구모드 저장 상태 [이 경우에만 초기화 시 복구 여부 물어봄] )
	// [1] => 초기화 작업 시 사용자의 복구 모드 선택 ( 0:복구모드 선택안함, 1:복구모드 선택 )
	// [2] => 복구 모드 저장 시간 정보
	// [3] => 초기화 작업 시 복구 모드 선택 시간
	int  nFlag_EqpRecovery[4];

	int nMode_BCR;

	// 공급 하는 모드 Master & Slave 로 되어 있다. 
	int nMode_SupplyMode;
	
	// 2.5 Inch, Desktop, M-SATA
	int nMode_SupplyDevice;

	int nMode_SMEMAUsing;

	int nMode_GripRetryCNT;

	// Grip, Vac 사용 모드 추가
	int nMode_VacUsing;
	int nMode_GripUsing;


	// 이것으로 사용하도록
	CString sEqpModel;
	CString sEqpNo;
	CString sOperatorId;
	
	CString sDvcName;


	int n_mode_interface;		// 장비 동작 모드 저장 변수 (1:ON LINE	0:OFF LINE)
	int nInspectRetry;		// 바코드 및 비전 검사 시도 횟수


	int nNCH;

	int n_mode_Network;
	int nMode_PlaceType;	// [1:4개씩, 0:2개씩]

	int nMode_Slop_Variable;

} ;
extern  st_basic_param  st_basic;
// ******************************************************************************

// ******************************************************************************
// 발생한 알람 관련 데이터 저장 구조체 선언                                      
// ******************************************************************************
struct st_alarm_param 
{
	CStringList mstr_list_jam;	// 발생한 Jam Code의 리스트
	CStringList mstr_list_jam_time;	//alarm 발생시간 저장 리스트
	CString mstr_code;			// 발생한 알람 코드 저장 변수
	char mc_code[10];
	CString mstr_pcode;			// 발생한 알람 코드 저장 변수
	int mn_count_mode;			// 알람 카운트 플래그 (0:알람 카운트 미진행    1:알람 카운트 진행)
	int mn_type_mode;			// 알람 종류 저장 변수 (0:경고성    1:진행성    2:장비 정지)
	
	long stl_cur_alarm_time;	// 알람 발생 시간 저장 변수
	int stl_prev_code;			// 바로 전 발생한 알람 코드 저장 변수
	long stl_prev_alarm_time;	// 바로 전 발생한 알람 발생 시간 저장 변수
	long stl_alarm_start_time;	// 시작 버튼이 눌렸을때의 발생 시간 저장 변수
	
	int mn_alarm_occured;		// 알람 발생 횟수 저장 변수
	int mn_alarm_occured_day;	// 알람 발생 횟수 저장 변수

	int mn_emo_alarm;

	int  nStopPushInInitial;	// 초기화 작업 중 STOP 버튼 눌렸는지 여부
	int nReset_AlarmStatus;

} ;
extern  st_alarm_param  alarm;
// ******************************************************************************

// ******************************************************************************
// 알람 데이터 저장 구조체 선언                                                  
// ******************************************************************************
struct st_dsp_alarm_param 
{
	// **************************************************************************
	// 알람 부위 표시 마크 [출력 위치], [크기], [칼라] 저장 변수                 
	// **************************************************************************
	int mn_Offset;
	int mn_Cy;
	int mn_Cx;
	int mn_LineWidth;
	int mn_R;
	int mn_G;
	int mn_B;
	// **************************************************************************
	
	// **************************************************************************
	// 화면에 출력할 알람 정보 저장 변수                                         
	// **************************************************************************
	int mn_action_cnt;            // 출력할 알람 조치 사항 갯수 저장 변수
	CString mstr_style;           // 알람 출력 형태 저장 변수
	CString mstr_bmp_file;        // 알람 표시 그림 파일 저장 변수
	CString mstr_repair_msg;      // 알람 조치 사항 저장 변수
	// **************************************************************************
	
	// **************************************************************************
	// 파일에 저장된 알람 정보 저장 변수                                         
	// **************************************************************************
	CString mstr_e_content[2000];  // 파일에서 로딩한 한 라인 정보 저장 변수
	CString mstr_e_code[2000];     // 알람 코드 저장 변수
	CString mstr_e_state[2000];    // 알람 상태 저장 변수
	CString mstr_e_part[2000];     // 알람 발생 부위 저장 변수
	CString mstr_e_msg[2000];      // 공백 제거 알람 메시지 저장 변수
	// **************************************************************************
	
	int mn_e_count[2000];
	int mn_e_list;
	
	// **************************************************************************
	// 현재 발생한 알람 저장 변수                                                
	// **************************************************************************
	int mn_cur_state;				// 알람 저장 여부 플래그
	CString mstr_cur_state;			// 알람 등급
	CString mstr_cur_msg;			// 발생한 알람 메시지 저장 변수
	// **************************************************************************

	int n_picking_error;
} ;
extern  st_dsp_alarm_param  st_alarm;
// ******************************************************************************

#define ACCY_BOX_NO_		0
#define ACCY_BOX_YES_		1

#define ACCY_NO_			0
#define ACCY_YES_			1

#define ACCY_OUT_YES_			0
#define ACCY_OUT_NO_			1

#define BCR_READ_BAD_	0
#define BCR_READ_GOOD_	1
	
// ******************************************************************************
// 장비 동작 제어 플래그 정보 저장 구조체 선언                                   
// ******************************************************************************
struct st_work_param 
{
	int nEqpStatus; 
	int nLoadingStatus;
	int nLoadingFlag;

	int nMdlInputCount[2][2];	// [Prime, Cum][Lot, Daily]
	int nMdlPassCount[2][2];
	int nHourPassCount[3];	//20130531 kjh
	int nMdlFailCount[2][2];
	int nMdlHtoCount[2][2];
	int nMdlBtoCount[2][2];
	int nMdlNtoCount[2][2];
	int nMdlRejectCount[2][2];

	int iLastTime;

	int mn_daily_fail_count;

	// **************************************************************************
	// 장비 동작 중 발생하는 시간 정보 저장 관련 멤버 변수                       
	// **************************************************************************
	long MachineTime[4];  // 장비 동작 및 정지 시간 정보 저장 변수 (0:STOP TIME    1:RUN TIME    2:ALARM TIME    3:MAINT TIME)
	// **************************************************************************

	int n_data_save_time;						// Data 저장 시간

	// **************************************************************************

	////복귀동작 관리
	int mn_reinstatement_mode;								// 0:복귀클리&완료 , 1:stop전환후//백업후 , 2:스타트 키 정보 저장, 3:복귀중 ...., 복귀완료후 0 그리고 런 하자 
	double md_reinstatement_pos[MOT_MAXMOTOR];				// 0: M_ROBOT_X, 1: M_ROBOT_Y, 2: Robot Z, 3: Robot P, 4: M_TRAY_TRANSFER_Y, 5: M_MULTI_STACKER_Z
	int mn_PickerUpDnStatusBackup[2];
	int mn_PickerTurnStatusBackup[2];
	double md_reinstatement_Pos[2][MOT_MAXMOTOR];

	int n_back_site;		//이동했던 위치
	int n_back_type;		//이동방식이 계산을 통한 위치인지, teach데이터로 이동인지
	int n_back_picker;		//이동했던 로봇의 피커지정
	int n_back_x_pos;		//이동했던 위치의 row
	int n_back_y_pos;		//이동했던 위치의 cols
	int n_back_z_movepos;	//이동했던 위치에서 z axis가 down인지 up동작인지 구분
	int n_back_tt_movepos;
	int n_back_ms_pos;

	double d_reinstatement_x_pos;		//복귀될 계산된 x 위치값
	double d_reinstatement_y_pos;		//복귀될 계산된 y 위치값
	double d_reinstatement_z_pos;		//복귀될 계산된 z 위치값
	double d_reinstatement_tt_pos;		//복귀될 계산된 tray transfer 위치값
	double d_reinstatement_ms_pos;		//복귀될 계산된 multi stacker 위치값

	int		mn_prev_step[100];			// 이전 step (변경 되었는지 보기 위함)
	CString	mstr_history_step[100];		// step_history 약 50개에 해당하는 step을 저장
	CString	mstr_history_step_back_up[100];		// step_history 약 50개에 해당하는 step을 저장
	CString mstr_last_change_date[100];	// 최종 step으로 변경되는 시점의 시간

 	int iBasic_Apply_Check;	// 베이직 어플라이 체크 ( 베이직 어플라이 클릭시 시간 초기화 되는 오류 수정용 ) 20130524 kjh

	// 규이리 추가 [2013.11.30]
	// 알람 발생에 대한 상태 변경 요청
	int nReq_AlarmProc;
	// 규이리 추가 [2013.12.02]
	int nReq_StrtProc;
	int nReq_StopProc;

	// 규이리 수정 [2013.09.01]
	int nAlarmHappened;

	// 백상현 추가 [2014.08.27]
	int    n_text_r[2][2];
	int    n_text_g[2][2];
	int    n_text_b[2][2];
	
	int    n_grid_r[2][2];
	int    n_grid_g[2][2];
	int    n_grid_b[2][2];

	int nForceDischarge;  // 강제 배출
	int nForceDischargeType;	// 0 : stop State, 1 : ForceDischarge after start

	// EQP2Main 연결 상태 확인 변수
	int nLink_Eqp2Main;

	// Lift에서 공급한 Accy 갯수 [WITHOUT 모드에서 사용]
	int nAccyCNT[4];

	// 센서 감지 오류로 인한 Box 미존재 강제 설정 플래그 (Accy Box Conv)
	int nSkipReq_ShiftTray;

	// Buffer Conv 악게사리 Reset Flag
	int nSkipReq_BufferConv[2];
	int nSkipReq_MiddleBufferConv;
	int nSkipReq_OutBufferConv[2];

	int nLiftDownComplete;
	
	int nModeBCR_Test;

	CString sRbtBCR_Data[4];			// XYZ RBT BCR Read Data
	CString sNGBufferBCR_Data[4];		// NGBuffer BCR Data

	CString sBufferConvBCR_Data[4];		// BufferConv BCR Data
	CString sBufferMiddleConvBCR_Data[2];		// BufferMiddler Conv BCR Data
	CString sBufferOutConvBCR_Data[4];		// BufferOutConv BCR Data

	// 이름 변경 [2015.06.18]
	CString m_sRcvAccyMathCode;	// Main에서 받은 Math Code를 저장하는 변수

	CString m_sCurrntAccyMathCode;	// 현재 작업 중인 Math Code를 저장하는 변수

} ;
extern  st_work_param  st_work;

#define MAIN_BCR_STATE	0
#define MAIN_ALARM		1
#define MAIN_ACCY_SUPPLY_COMPLETE	2
#define MAIN_EQP_STATE	3
#define MAIN_SUPPLY_TYPE	4
#define MAIN_SUPPLY_READY_CNT	5
// yangkyu [2015.03.19]
#define MAIN_ACCY_MATH_CODE		6

struct st_Eqp_Map
{
	/////////////////////////// 
	int nLoadingAccyBoxExist[3];	// Accy Box Conv에 있는 자재 확인 변수
	int nEmptyAccyBoxExist[3];		// Empty Accy Box 자재 확인 변수
	int nClampAccyBoxAccy;			// Accy Box Clamp에 있는 자재 확인 변수

	int nLiftWorkSite;  			// Accy 작업 영역 설정 (작업 순서 : Front -> Middle -> Rear)
	int nXYZRbtPickerInfo[4];		// [0 = Flag, 1 = Sen][]
	int nNGOut;						// NG Out 확인 변수
	int nNGBuffer[2];				// NG Buffer Accy 확인 변수
	
	int nBufferCovAccyExist[4];		// Buffer Conv Accy 확인 변수
	int nBufferMiddleAccyExist[2];	// Middle Conv Accy 확인 변수
	int nBufferOutAccyExist[4];		// Out Conv Accy 확인 변수

	//////////////////////////////////////////////////////////////////////////
	int nLotStart;
	int nBoxChange;
	int nResp_SupplyCNT;	//공급한 Accy 갯수
	int nResp_AccyReadyCNT;	//공급 가능한 Accy 갯수

	int nResp_LotType;	// Main에서 전송된 LOT Type를 저장 하는 변수

	int nNGOutCNT;			//NG 처리되어 버려진 Accy 갯수

	int nSupplyTypeMath;

	int nBCR_State[4];		// BCR Read State
	//CString sBCR_Data[4];		// BCR Read Data

	// 악세사리 재정렬을 위해 추가
	int  m_nReq_ReAlign;
};
extern  st_Eqp_Map  st_map;

enum
{
		PLACE_LOADING_CONV_		= 0,
		PLACE_BOX_CLAMP_		= 1,
		PLACE_BOX_LIFTER_		= 2,
		PLACE_XYZ_RBT_			= 3,
		PLACE_NG_OUT_			= 4,
		PLACE_NG_BUFFER_		= 5,
		PLACE_BUFFER_CONV_		= 6,
		READ_BCR_DATA			= 7,
};

// ******************************************************************************
// RS-232C 시리얼 통신 관련 저장 구조체 선언                                     
// ******************************************************************************
struct st_serial_param
{
	int	n_connect[MAX_PORT];
	int n_snd_chk[MAX_PORT];         // 메시지 송신 완료 플래그
	int n_rec_chk[MAX_PORT];         // 메시지 수신 완료 플래그
	// 규이리 추가 [2014.10.07]
	int nReq_CommSend[MAX_PORT];	// 메시지 전송 요청

	CString str_port_chk[MAX_PORT];  // 시리얼 포트 생성 정보 저장 변수
	
	CString str_snd[MAX_PORT];       // 송신 메시지 저장 변수
	CString str_rec[MAX_PORT];       // 수신 메시지 저장 변수
	
	CString comm_snd[MAX_PORT];      // 리스트 화면 출력 송신 메시지 저장 변수
	CString comm_rec[MAX_PORT];      // 리스트 화면 출력 수신 메시지 저장 변수
	CString comm_err[MAX_PORT];      // 리스트 화면 출력 에러 메시지 저장 변수
	
	int nAccept_TRIN;	// 데이터 받아들이라는 플래그
	int nAccept_TROUT;	// 데이터 받아들이라는 플래그
	
	CString comm_msg[MAX_PORT];
} ;
extern  st_serial_param  st_serial;
// ******************************************************************************

enum LAMP_CTRL
{
	LAMP_OFF	= 0,
	LAMP_ON		= 1,
	LAMP_FLICK	= 2,
};


// *****************************************************************************
//  Tower Lamp 화면에 대한 정보 저장 구조체 변수 선언                           
// *****************************************************************************
// 백상현 추가 [2014.07.30]
#define MAX_LAMP_STATUS_TYPE 9

struct tagLAMP_PARAM
{
	int nLampR[MAX_LAMP_STATUS_TYPE];  // RED 램프 [0:OFF, 1:ON, 2:FLICK]
	int nLampY[MAX_LAMP_STATUS_TYPE];  // YELLOW 램프 [0:OFF, 1:ON, 2:FLICK]
	int nLampG[MAX_LAMP_STATUS_TYPE];  // GREEN 램프 [0:OFF, 1:ON, 2:FLICK]
	
	int nModeBuzzer;	// 부저 사용 모드
	
	int nTwinkle;	// 반짝거리는 시간 간격
} ;
extern  tagLAMP_PARAM  stLAMP;

#define MAX_WAIT_ACCY_BOX_CONV_			5000

enum SMEMA_CASE
{
	REQ_RESET_			= 0,
	REQ_REQUEST_ON_		= 1,
	REQ_CLAMP_ON_		= 2,
	REQ_REQUEST_OFF_	= 3,
	REQ_COMPLETE_		= 4,
	RDY_RESET_			= 10,
	RDY_READY_ON_		= 11,
	RDY_READY_OFF_		= 12,
	REQ_EMO_ON_			= 15,
	REQ_EMO_OFF_		= 16,
};

enum AC_REQ_CASE
{
	REQ_MV_RESET_		= 0,
	REQ_MV_HIGH_		= 1,
	REQ_MV_LOW_			= 2,
	REQ_MV_STABILITY_	= 3,
	REQ_MV_OUTPUT_		= 4,
	REQ_MV_INIT_		= 6,
	REQ_MV_FRONT_		= 7,
	REQ_MV_REAR_		= 8,
	REQ_MV_FIRST_		= 9,
	REQ_MV_SECOND_		= 10,
	REQ_MV_THIRD_		= 11,
	REQ_MV_ACCY_OUT_	= 12,
};

#define UNTIL_WAITTIME_			100  // [100ms] 동안 상태 체크

enum AC_MOVE_TYPE
{
	AC_MV_STOP_	= 0,
	AC_MV_CW_	= 1,
	AC_MV_CCW_	= 2,
};

// *****************************************************************************

#define CHK_NOT_EXIST_			0
#define CHK_EXIST_				1

#define MAX_IOBRD_NUM_			2

#define PICKERNUM				8

#define MAX_DOOR_				16
#define MAX_BUZZ_				4
#define MAX_CONV_				3
#define MAX_SITE_				2
#define MAX_UPBUFF_				8
#define MAX_DNBUFF_				4
#define MAX_CLAMP_UNIT_			3
#define MAX_GRIPMOTOR_			4
#define MAX_DETECTBOX_			3

#define BTM_CONV_				0
#define TOP_CONV_				1
#define SIDE_LEFT_				0
#define SIDE_RIGHT_				1

#define	SIDE_FIRST_				0
#define	SIDE_SECOND_			1
#define	SIDE_THIRD_				2

#define SIDE_FW_				0
#define SIDE_BW_				1

#define SIDE_UP_				0
#define SIDE_DOWN_				1

#define SIDE_FRONT_				0
#define SIDE_REAR_				1
#define SIDE_OUT_				2

#define SIDE_MIDDLE				2
#define SIDE_OUT_FRONT			3
#define SIDE_OUT_REAR			4


#define SMEMA_TOP_				0
#define SMEMA_BTM_				1



#define	SIDE_BOTH_				2


// 프로그램에서 사용할 IO MAP 정의
struct tagIO_PARAM
{
	//==============================================================//
	// 모듈 번호 : 00/01
	//==============================================================//
	int o_TLampGreen;
	int o_TLampYellow;
	int o_TLampRed;	
	int o_BuzzerSound1;
	int o_BuzzerSound2;
	int o_BuzzerSound3;
	int o_DoorLock;

	int o_BuzzerSound[MAX_BUZZ_];
	
	int i_Chk_MC1;
	int i_Chk_EMO[2];
	int i_Chk_Door[MAX_DOOR_];											// MAX_DOOR_ = 16개

	//==============================================================//
	// 모듈 번호 : 02
	//==============================================================//
	int o_LampStart;								
	int o_LampStop;									
	int o_LampJamClear;								
	int o_LampBuzzStop;								
	
	int i_Chk_StartSwitch;								
	int i_Chk_StopSwitch;								
	int i_Chk_JamClearSwitch;							
	int i_Chk_BuzzStopSwitch;							
	int i_Chk_AutoSwitch;							
	int i_Chk_ManualSwitch;							
	int i_Chk_FrontSelectSwitch;
	//==============================================================//
	// 모듈 번호 : 03
	//==============================================================//
	int o_LampManualLoading;
	int o_LampLoadingStart;
	int o_AccyBoxConvMotor;
	int o_Cyl_AccyBoxConvStopperUp2;
	int o_Cyl_AccyBoxConvStopperDn2;
// 	int o_Cyl_AccyBoxPusher;											// Nomal Bw
		
	int i_Chk_ManualLoadingSwitch;
	int i_Chk_LoadingStartSwitch;
	int i_Chk_AccyBoxConvStopperUp2;
	int i_Chk_AccyBoxConvStopperDn2;
// 	int i_Chk_AccyBoxPusherFw;	
// 	int i_Chk_AccyBoxPusherBw;
	int i_Chk_AccyBoxConvDetection_First;
	int i_Chk_AccyBoxConvDetection_Second;
	int i_Chk_AccyBoxConvDetection_Third;
	int i_Chk_AccyBoxConvDetection[MAX_DETECTBOX_];						// MAX_DETECTBOX_ = 3개

	//==============================================================//
	// 모듈 번호 : 04
	//==============================================================//
	int o_Cyl_AccyBoxLifterUp;
	int o_Cyl_AccyBoxLifterDn;
	int o_AccyBoxConvMotor2;
	int o_Cyl_AccyBoxConvStopperUp;
	int o_Cyl_AccyBoxConvStopperDn;

			
	int i_Chk_AccyBoxLifterUp[MAX_SITE_];								//MAX_SITE_ = [Left = 0, Right = 1]
	int i_Chk_AccyBoxLifterDn[MAX_SITE_];								//MAX_SITE_ = [Left = 0, Right = 1]
	int i_Chk_AccyBoxConvStopperUp;
	int i_Chk_AccyBoxConvStopperDn;
	int i_Chk_AccyBox_Arrive_End;

	//==============================================================//
	// 모듈 번호 : 05
	//==============================================================//
	int o_Cyl_AccyBoxGripperUp;
	int o_Cyl_AccyBoxGripperDn;	
	int o_Cyl_AccyBoxClamp;//align
	int o_Cyl_AccyBoxUnclamp;

	int i_Chk_AccyBoxGripperUp[MAX_SITE_];								//MAX_SITE_ = [Left = 0, Right = 1]
	int i_Chk_AccyBoxGripperDn[MAX_SITE_];								//MAX_SITE_ = [Left = 0, Right = 1]	
	int i_Chk_AccyBoxClamp[MAX_SITE_];									//MAX_SITE_ = [Left = 0, Right = 1]
	int i_Chk_AccyBoxUnclamp[MAX_SITE_];								//MAX_SITE_ = [Left = 0, Right = 1]
	int i_Chk_GripperSafetyPos;

	//==============================================================//
	// 모듈 번호 : 06
	//==============================================================//
	int i_Chk_BoxClampAccyDetection[MAX_SITE_][6];

	//==============================================================//
	// 모듈 번호 : 07
	//==============================================================//

	int o_Load_BoxClampOnOff;
	int o_Load_BoxPusherFwBw;

	int i_Chk_Load_BoxClampOnCheck;
	int i_Chk_Load_BoxClampOffCheck;

	int i_Chk_Load_BoxPusherFwCheck;
	int i_Chk_Load_BoxPusherBwCheck;


	//==============================================================//
	// 모듈 번호 : 10
	//==============================================================//
	int o_Slop_Seperator_Stopper_UpDn;
	int o_Out_Front_Stopper_UpDn;
	int o_Out_Rear_Stopper_UpDn;

	int i_Chk_Slop_Seperator_StopperUpCheck;
	int i_Chk_Slop_Seperator_StopperDnCheck;

	int i_Chk_Out_Front_StopperUpCheck;
	int i_Chk_Out_Front_StopperDnCheck;

	int i_Chk_Out_Rear_StopperUpCheck;
	int i_Chk_Out_Rear_StopperDnCheck;

	int i_Chk_Slop_Left_AccyDetection;
	int i_Chk_Slop_Right_AccyDetection;

	int i_Chk_Out_Front_Left_Accy_DetectionChk;
	int i_Chk_Out_Front_Right_Accy_DetectionChk;
	int i_Chk_Out_Rear_Left_Accy_DetectionChk;
	int i_Chk_Out_Rear_Right_Accy_DetectionChk;


	
	//==============================================================//
	// 모듈 번호 : 11
	//==============================================================//
	int	o_Slop_Variable_Front_Left_OpenClose;
	int	o_Slop_Variable_Front_Right_OpenClose;
	int	o_Slop_Variable_Rear_Left_OpenClose;
	int	o_Slop_Variable_Rear_Right_OpenClose;

	int i_Slop_Variable_Font_Left_Open_Check;
	int i_Slop_Variable_Font_Left_Close_Check;
	int i_Slop_Variable_Font_Right_Open_Check;
	int i_Slop_Variable_Font_Right_Close_Check;

	int i_Slop_Variable_Rear_Left_Open_Check;
	int i_Slop_Variable_Rear_Left_Close_Check;
	int i_Slop_Variable_Rear_Right_Open_Check;
	int i_Slop_Variable_Rear_Right_Close_Check;


// 	int o_Cyl_BufferTryPathAlignUp;
// 	int o_Cyl_BufferTryPathAlignDn;
// // 	int o_Cyl_BufferAccyConvStopper;
// 	int o_Cyl_BufferAccyConvFrontStopper;
// 	int o_Cyl_BufferAccyConvRearStopper;
// 
// 
// 	int i_Chk_BufferAccyConvStopper[MAX_SITE_][2];						//MAX_SITE_ = [공급 되는 방향의 첫번째 = 0, 공급 되는 방향의 두번째 = 1]
// 	int i_Chk_BufferTryAccyDetection[3][MAX_SITE_];						//MAX_SITE_ = [Left = 0, Right = 1]	
	int i_Chk_NGBufferTryFull;

	//==============================================================//
	// 모듈 번호 : 12
	//==============================================================//
	int o_Picker_fl_updn;//front left
	int o_Picker_rl_updn;//rear left
	int o_Picker_fr_updn;//front right
	int o_Picker_rr_updn;//rear right
	int o_Picker_updn[MAX_SITE_][MAX_SITE_];
	int o_Picker_pitch;
	int i_Picker_fl_up_chk;
	int i_Picker_rl_up_chk;
	int i_Picker_fr_up_chk;
	int i_Picker_rr_up_chk;
	int i_Picker_updn_chk[MAX_SITE_][MAX_SITE_];
	int i_Picker_open_check;
	int i_Picker_close_check;
	int i_Picker_front_up_chk;
	int i_Picker_rear_up_chk;








	int o_Smema_Ready2Main;
	int o_Smema_Emergency2Main;
	
	int i_Chk_UnloaderFull[MAX_SITE_];									//MAX_SITE_ = [ 자중 구간 위치 = 0, 맨마지막 위치 = 1]	
	int i_Chk_ReqFromMain;
	int i_Chk_TrsfFromMain;

	//==============================================================//
	// 모듈 번호 : 13
	//==============================================================//
	int o_Vac_Rbt_VacOnOff_Front_Left;
	int o_Vac_Rbt_VacOnOff_Front_Right;
	int o_Vac_Rbt_VacOnOff_Rear_Left;
	int o_Vac_Rbt_VacOnOff_Rear_Right;
	int o_Vac_Rbt_Eject_Front_Left;
	int o_Vac_Rbt_Eject_Front_Right;
	int o_Vac_Rbt_Eject_Rear_Left;
	int o_Vac_Rbt_Eject_Rear_Right;

	int o_Vac_Rbt_VacOnOff[MAX_SITE_][MAX_SITE_];							//MAX_SITE_ = [Left = 0, Right = 1]
	int o_Vac_Rbt_EjectOnOff[MAX_SITE_][MAX_SITE_];							//MAX_SITE_ = [Left = 0, Right = 1]
	
	int o_BufferConvMotor_Left;
	int o_BufferConvMotor_Right;
	int o_BufferConvMotor[MAX_SITE_];									//MAX_SITE_ = [Left = 0, Right = 1]
// 	int o_Cyl_BufferTryPitchFw;
// 	int o_Cyl_BufferTryPitchBw;
	
	int i_Chk_Vac_Rbt_VacOnOff_Front_Left;
	int i_Chk_Vac_Rbt_VacOnOff_Rear_Left;
	int i_Chk_Vac_Rbt_VacOnOff_Front_Right;
	int i_Chk_Vac_Rbt_VacOnOff_Rear_Right;
	
	
	int i_Chk_Rbt_VacOn[MAX_SITE_][MAX_SITE_];								//MAX_SITE_ = [Left = 0, Right = 1]	


	
	//==============================================================//
	// 모듈 번호 : 14
	//==============================================================//
	int o_Cyl_BufferTryPathAlignUp;
	int o_Cyl_BufferTryPathAlignDn;
	// 	int o_Cyl_BufferAccyConvStopper;
	int o_Cyl_BufferAccyConvFrontStopper;
	int o_Cyl_BufferAccyConvRearStopper;
	 
	int i_Chk_BufferTryPathAlignUp;
	int i_Chk_BufferTryPathAlignDn;

	int i_Chk_BufferAccy_Front_StopperUpCheck;
	int i_Chk_BufferAccy_Front_StopperDnCheck;
	int i_Chk_BufferAccy_Rear_StopperUpCheck;
	int i_Chk_BufferAccy_Rear_StopperDnCheck;

	int i_Chk_BufferAccyConvStopper[5][2];						//MAX_SITE_ = [공급 되는 방향의 첫번째 = 0, 공급 되는 방향의 두번째 = 1]
	int i_Chk_BufferTryAccyDetection[5][MAX_SITE_];						//MAX_SITE_ = [Left = 0, Right = 1]	
	

	int i_Chk_OutLeftBufferAccyDetection;
	int i_Chk_OutRightBufferAccyDetection;
	




	//==============================================================//
	// 모듈 번호 : 15
	//==============================================================//
	
	int i_Chk_NGBufferAccyDetection[2];

};
extern tagIO_PARAM	stIO;

struct st_time_database
{
	int				mndb;
	
	COleDateTime	mole_date;
	
	CString			mstrlot_no;
	CString			mstrpart_no;
	
	int				mnTime_Section;
	int				mnTime_Time;
	int				mnMode;
	
	int				mnSectionBuffer;
};
extern struct st_time_database st_timedb;

struct st_alarm_database
{
	int				mndb;
	
	COleDateTime	mole_date;
	
	CString			mstrlot_no;
	CString			mstrpart_no;
	
	CString			mstrAlarmCode;
	CString			mstrAlarmMsg;
};
extern struct st_alarm_database st_alarmdb;

struct st_part_motor_info
{
	int				n_part_cnt;
	int				n_part_axis_cnt[20];
	int				n_part_axis[20][10];
	
	int				n_axis_pos_num[20][10];
	int				n_axis_minus_el[20][10];
	int				n_axis_plus_el[20][10];
	int				n_axis_home[20][10];
	int				n_axis_sd[20][10];
	int				n_axis_direction[20][10];
	int				n_axis_account[20][10];
	
	CString			str_part_info[20];
	CString			str_part_name[20];
	
	CString			str_part_axis_name[20][10];
	CString			str_axis_pos_name[20][10][25];
	CString			str_axis_pos_info[20][10][25];
};
extern struct st_part_motor_info st_motor_info;

struct st_part_info
{
	int			n_part_cnt;
	int			n_part_info;
	int			n_motor_axis_cnt[20];
	int			n_motor_axis[20][10];
	int			n_io_input_cnt;
	int			n_io_output_cnt;
	int			n_io_input[32];
	int			n_io_output[32];
	
	CString		str_part_name[20];
	CString		str_part_info[20];
	CString		str_motor_axis[20][10];
	CString		str_io_input[32];
	CString		str_io_output[32];
};
extern struct st_part_info st_part;

// yangkyu [2015.03.11]
struct st_Accy_data
{
	double d_X_Gripper_Size;	// Gripper(0, 2)와 Greipper(1, 3)간의 X 간격
	double d_Y_Gripper_Size;	// Gripper(0, 1)와 Greipper(2, 3)간의 Y 간격

	double d_Y_BufferConv_Size;	// Buffer Conv Front와 Rear간의 Y 간격
};
extern st_Accy_data st_accy;

// 복구 관련 구조체
struct st_SeqBoxClamp_param
{
	int nStep_Run;
	int nStep_Init;
	int nStep_Moving;
	
};
extern st_SeqBoxClamp_param reco_SeqBoxClamp;

struct st_SeqBoxLifter_param
{
	int nStep_Run;
	int nStep_Init;
	int nStep_Moving;
	
};
extern st_SeqBoxLifter_param reco_SeqBoxLifter;

struct st_SeqBufferAlignConv_param
{
	int nStep_Run;
	int nStep_Init;
	int nStep_Moving;
	
};
extern st_SeqBufferAlignConv_param reco_SeqBuffAlignCV;

struct st_SeqLoadingConv_param
{
	int nStep_Run;
	int nStep_Init;
	int nStep_Moving;
	
};
extern st_SeqLoadingConv_param reco_SeqLoadingCV;

struct st_SeqXYZRobot_param
{
	int nStep_Run;
	int nStep_Init;
	int nStep_Moving;
	
};
extern st_SeqXYZRobot_param reco_SeqXYZRBT;

#endif

