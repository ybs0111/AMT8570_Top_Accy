#ifndef _VARIABLE_h__
#define _VARIABLE_h__

#include "Alg_Mysql.h"
#include "CtlBd_Variable.h"

#define DEBUG_FLAG				1

#define DEVELOPER_PC			0
#define BARCODE_USED			1

/////////////////////////////////////////////////////////
//DLL ������ ���濹�� //2011.1007 �߰� 
/////////////////////////////////////////////////////////
#define BD_NO		0  //� ������ Ŭ����� ���� 
#define BD_YES		1  //� ������ ��û�� ����

// ���̸� �߰� [2014.08.26]

#define NO						0		//� ������ Ŭ����Ⱥ ���� 
#define YES						1		//� ������ ��û�� ���� 
#define READY					2		//� ������ �C���� �� ������ �غ� �Ϸ��� ����  


//////////////////////////////////////////////////////////////////////////////////////////////////////////
//motor, i/O ������� ���ϰ��� �� �͸� ����Ѵ�  
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

#define MAX_SITE		20	//�� ��ġ�� ����Ʈ ����	0:Input, 1:Loader, 2:Empty, 3:Uld 1, 4:Uld 2, 5:Uld 3, 6:Multi Stacker 1, 7:Multi Stacker 2, 8:Multi Stacker 3, 
#define DATA_INFO		10	//[0 ] ~ [7] ���� ��� 

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
	T_CONVEYOR_STOPPER_LOB,			// �Ǿ�
	T_CONVEYOR_STOPPER_PLATE,		// �߰�
	T_CONVEYOR_STOPPER_STACKER,		// ����Ŀ
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
// ���� �̸� �� ��ġ 8500
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

#define COUNT_NO                0    // �ʱ� ���� ���� �� ���� ���� �˻� 
#define COUNT_YES               1    // ��ü ���� �� ���� ���� �˻� 

#define INFO_SKIP               0    // �ش� ������ �������� ���� 
#define INFO_SETTING            1    // �ش� ������ ������ 
#define INFO_ADJUST			    2    // Adjust��.

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

#define RET_PICKERNOTFIND       6		// BIN�� �ش�Ǵ� ��� ������ 
#define RET_NOWHERE             -1		// �ش� ������ ������ 
#define RET_NOSPACE             -2		// ���� ������ 
#define RET_LOADTRAYSKIP        -100	// LOADER OFF / ���� LOT END 


#define PICKER_DVC_CNT          7
#define PICKER_ENABLE_INFO      8		// ��Ŀ ���� Ȯ�� 

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
// ����� ���� �޽��� ID ����                                                   
// *****************************************************************************
#define WM_FORM_CHANGE				WM_USER+200   // ȭ���� ��ȯ�� �� �ش� ��ȯ ���� ������ �����ϱ� ���� ����� ���� �޽��� 
#define WM_STATUS_CHANGE			WM_USER+201   // Ÿ��Ʋ �� ȭ������ ���۵� �޽��� ��½�Ű�� ���� ����� ���� �޽��� �߰� 
#define WM_LIST_DATA				WM_USER + 6   // ��� �޽��� ��� ����Ʈ ��ȭ ���ڿ� 

#define WM_DATA_SEND				WM_USER + 7   // Data�� �����ϱ� ���� ȭ�鿡�� ������ �����ϱ� ���� ������ ����� ���� �Լ�
#define WM_WORK_END					WM_USER + 8   // �׽�Ʈ �۾� �Ϸ� �� ������ ���� ȭ�鿡 ��½�Ű�� ���� ����� ���� �޽��� �߰� 
#define WM_SCRAP_DSP				WM_USER + 9

#define WM_BASIC_APPLY				WM_USER + 10  // BASIC DATA�� APPLY ��Ű�� ���� ����� ���� �޽��� �߰� 
#define WM_MAINTENANCE_APPLY		WM_USER + 11  // Maintenance DATA�� APPLY ��Ű�� ���� ����� ���� �޽��� �߰�
#define WM_TESTREFERENCE_APPLY		WM_USER + 12
#define WM_INTERFACE_APPLY			WM_USER + 13
#define	WM_WAITTIME_APPLY			WM_USER + 14  // Wait Time DATA�� APPLY ��Ű�� ���� ����� ���� �޽��� �߰� 

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
// ���̸� �߰� [2014.10.09]
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

#define FILE_WAITTIME_MAP_		(CString)"AMT8570_Accy_WAITTIME_MAP.xls"	// ȭ�� ����
#define FILE_MOT_SPEED_			(CString)"MotorSpeed.TXT"
#define FILE_WAITTIME_			(CString)"WaitTime.TXT"

#define WITHOUT_DVC_		0
#define WITH_DVC_			1

#define VAR_INIT_			0

//==================================================================//
// ���� ����� ���� �߰���
// ���̸� �߰� [2013.11.12]
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
	// ���̸� �߰� [2013.11.27]
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

// ���̸� �߰� [2014.08.14]
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
	char chSendCmd[100];	// ������ �޽��� ��� ���� (���ڿ�)
	char chResponse[100];	// ������ �޽����� ��� ���� (���ڿ�)
	// ������ �޽����� ���� ���� ���� �� ���� ����
	// - [0:default, 1:����, -1:����, 10:�����ʿ�, 11:���ں��ʿ�]
	int nEcho;
	// ������ �޽����� ���� ���� ��ٸ� ���� �� ���� ����
	// - [0:default, 1:����, -1:����, 10:��������(��ٸ�), 11:�������]
	int nResponse;
	int nTimeOut;	// Ÿ�Ӿƿ� �˻����� ���� [0:�˻���ʿ�, 1:�˻��ʿ�]
	
	char chReceiveCmd[100];	// ���ŵ� �޽��� ���� (���ڿ�)
}comm_param;


struct st_client_param
{
	CString str_ip;	// ���� IP �ּ�
	int n_port;
	int n_connect;		// ���� ���� ����
	
	int n_rev_length;	// ���� ���ڿ� ����
	char ch_rev[4096];	// ���� ���ڿ�
	char ch_send[4096];	// �۽� ���ڿ�

	char chSendHead[40];	// ������ ��� ����
	char chSendBody[4096];	// ������ �ٵ� ����
	
	// ���̸� �߰� [2014.11.01]
	int nIndex;	// ������ �޽����� �ε��� ��ȣ
	comm_param	stComm[10];
};
extern st_client_param	st_client[10];

struct st_server_param
{
	CString str_ip;	// �Ұ� IP �ּ�
	int n_port;
	int n_connect;	// ���� ���� ����
	
	int n_send_length;	// �۽� ���ڿ� ����
	int n_rev_length;	// ���� ���ڿ� ����
	char ch_rev[4096];	// ���� ���ڿ�
	char ch_send[4096];	// �۽� ���ڿ�
};
extern st_server_param	st_server[10];

enum
{
	TYPE_WORD = 0,
	TYPE_BYTE = 1,
};


// *****************************************************************************
//  Main Screen ��¿� �޼��� ����                                              
// *****************************************************************************
enum ENUM_MAIN_SCR_MSG
{
	MAIN_TIMEINFO,
	MAIN_COUNTINFO,
	MAIN_DSP_UPH,
};

// *****************************************************************************
#define LANGUAGE_KOREA		0		// �ѱ� OS
#define LANGUAGE_ENGLISH	1		// ���� OS


/* ****************************************************************************** */
/* RS-232C �ø��� ��� �� ���Ǵ� �޽��� ����                                    */
/* -> MAX_PORT : �⺻ ��Ʈ 2���� �����Ǿ� �ִ�						              */
/* ****************************************************************************** */
#define MAX_PORT	1		// ������ �ø��� ��Ʈ ����
#define COM_ERROR	-1		// ���� �޽���

#define BCR_PORT	1		// ���� BCR READ ��Ʈ ��ȣ
#define TESTER_PORT	2		// ���� Tester Communication ��Ʈ ��ȣ

#define BCR_ECHO	8883	//barcode parsing echo

#define BUF_SIZE	1024		// ���� ũ��

#define PORT_BCR_ACCY_		1

// ���̸� �߰� [2012.10.04]
#define MAX_BUF_LEN			4095

enum LOAD_STOP
{
	LOAD_STOP_NONE	= 0,
	LOAD_STOP_YES	= 1,
	LOAD_STOP_NO	= 2,
};

// *************************************************************************
// ��Ÿ ���� ���� ����ü ����                                                    
// *************************************************************************
struct st_other_param 
{
	CString str_op_msg;			// ����Ʈ ȭ�� ��� ���� ���� �޽��� ���� ����
	CString str_normal_msg;		// ����Ʈ ȭ�� ��� ���� ���� �޽��� ���� ����
	CString str_abnormal_msg;	// ����Ʈ ȭ�� ��� ���� ���� �޽��� ���� ����
	
	CString str_fallacy_msg;	// ���� �޽��� ��ȭ ���� Ÿ��Ʋ ���� ����
	CString str_typing_msg;		// Ű���� ��ȭ ���� Ÿ��Ʋ ���� ����
	
	CString str_pass_level;		// ��ȣ ���� ���� ����
	CString str_password;		// ��ȣ ���� ����
	
	CString str_parent_menu;	// �ָ޴� �̸� ���� ����
	
	char c_normal_msg[MAX_BUF_LEN];
	char c_abnormal_msg[MAX_BUF_LEN];
	
	////////////////////////////////2K10/02/09/ViboX////////////////////////////////////////
	int		n_prev_step[100];			// ���� step (���� �Ǿ����� ���� ����)
	CString	str_history_step[100];		// step_history �� 50���� �ش��ϴ� step�� ����
	CString str_last_change_date[100];	// ���� step���� ����Ǵ� ������ �ð�
};
extern  st_other_param  st_other;
// *************************************************************************

enum
{
	DLG_SHOW_	= 0,
	DLG_HIDE_	= 1,
};

// ���̸� �߰� [2015.03.14]
#define MAX_PICKER_ 			4
// [0:üũ�� ��������, 1:���� ��������]
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

// �޴��� ȭ�鿡�� ����Ѵ�.
#define MOT_MOVE			1
#define MOT_HOME			2

#define HOMING				1
#define MOVING				2
#define MULTI_MOVING		3
#define POS_MOVING			4

// *****************************************************************************
//  �޴����� ����� ID
// *****************************************************************************
#define MENU_SETTING		0
#define MENU_MOTOR			1
#define MENU_LIST			2

// ******************************************************************************
// ��� ���� ���� �� �ʿ��� �޽��� ����                                          
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
// �Ǽ��縮 �ڽ� Loading ���� �� �ʿ��� �޽��� ����                                          
// ******************************************************************************
#define dMANUAL			0
#define dSTART			1

// *****************************************************************************
//  Title Bar ��¿� �޼��� ����                                                
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
//  List Box ��¿� �޼��� ����                                                 
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
// I/O ��� ���� �� ��Ʈ �ִ� ���� �����Ѵ�                                      
// ******************************************************************************
#define MAX_MODULE		6
#define MAX_MAP			32
#define MAX_IN			716
#define MAX_IO			716
// ******************************************************************************

#define ELEC_PORT		5


// ******************************************************************************
//  MOTOR ���� ���� ���� �����Ѵ�                                                
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

// SOCKET DATA����.
#define LOT_DATA			0	// �ش� LOT����...
#define DAY_DATA			1	// ����
#define MONTH_DATA			2	// ���
#define SHIFT_DATA			3	// �� Shift

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

//���� ���� ���� 
#define MOT_PLUS			1
#define MOT_MINUS			0

enum
{
	POS_NONE = -1,
	POS_COUNT = 2,
};

enum { SOCKET_COUNT = 20 };
enum { BOARD_COUNT = 10 };

#define MOT_RTY_CNT			4	//�Ľ��� ����� ���� ������ �� �õ��ϴ� Ƚ��

#define MOT_MOVE_LIMITTIME	60000	//�̵� �� 3���̳����� �Ϸ��� �� 	 
#define MOT_TIMEOUT			90000   //1���� time out�� ������ �����Ѵ�  //Ȩ üũ�� ��� 

//������ �޼��� �����û�� 
#define MOT_NORMAL_MSG		0
#define MOT_ERR_MSG			1
#define MOT_ERR_CODE		2

#define MOT_LmMAPINDEX			0	//���� ����Ʈ ��� ���� 
#define MOT_IxMAPINDEX			1	//���� ���� ���� ���� 

#define RET_PROCEED		0
#define RET_GOOD		1
#define RET_ERROR		2
#define RET_RETRY		3
#define RET_DOOROPEN	4		// Door Open�� �Ǿ� ����. 2K6/06/20/ViboX

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

//091010 I/O ���۽� Out/In ���� üũ�� ������ �Ʒ��� ���� �����Ͽ� ���� �Ϸ� ���θ� ��Ȯ�� Ȯ������ 
#define CTL_IOCHK_CLEAR				0 //I/O ������ �����ϴ� ��ġ(Output �ñ׳� ���۽� ���)
#define CTL_IOCHK_START				1 //I/O �ƿ�ǲ ������ �Ϸ��Ͽ� ��ٸ��� �ð��� �����ϴ� ��ġ(Input üũ ������ ó�� ���� �ϴ� ��ġ) 
#define CTL_IOCHK_END				2 //I/O ������ input ��� �ð����� ��� �Ϸ��Ͽ� ���� ����
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define HANDLER_BACKUP_DATA_RW	29
#define DATA_READ    0
#define DATA_WRITE   1

// Į�� ����
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

//Grid ���� 
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



// ���̸� �߰� [2014.07.30]
enum
{
	REQ_NONE	= 0,
	REQ_YES		= 1,
};

//==================================================================//
// �α� Ÿ�� ����
// : ���̸� �߰� [2012.02.10]
//==================================================================//
#define LOG_SOCKET_				0x01
#define LOG_SEQ_				0x02
#define LOG_COMM_				0x04
#define LOG_PGM_				0x08
#define LOG_BCR_				0x10
#define LOG_SUPPLY_COUNT_		0x20

// ���̸� �߰� [2013.11.28]
#define CONFIG_PATH_BASE_		(CString)"d:\\AMTLOG"
 

#define MAX_EMO_			2

// *****************************************************************************
// ȭ�� ���� ���� �⺻ ������ ���� ����ü ���� ����                             
// *****************************************************************************
struct st_handler_param
{
	int nFlag_InitComplete;
	
	int nSelectedMenuNum;	// ���� ���õ� �޴� ID
	int nShowAlarmDlg;		// �˶� ȭ�� ��µǾ��� �ִ��� ����

	int  nStopPushInInitial;	// �ʱ�ȭ �۾� �� STOP ��ư ���ȴ��� ����
		
	BOOL b_program_exit;			//���α׷��� ���������°��� Ȯ���Ѵ�.
	BOOL b_CalledAlarmEditor;

	int mn_language;				// � ����� OS��.
	
	// *************************************************************************
	// ȭ�鿡 ���� �ڵ� ���� ���� ���� ����                                     
	// *************************************************************************
	HWND hWnd;
	CWnd *cwnd_title;		// Ÿ��Ʋ�� �ڵ�
	CWnd *cwnd_list;		// ����Ʈ�� �ڵ�

	CWnd *cwnd_main;		// ���� ȭ�� �ڵ�
	CWnd *cwnd_basic;		// ������ ȭ�� �ڵ�
	CWnd *cwnd_motor;		// ���� ȭ�� �ڵ�
	CWnd *cwnd_motorAxis;	// ���� �ະ ȭ�� �ڤ���


	// *************************************************************************
	// ��� ���� ���� ���� ���� ����                                            
	// *************************************************************************

	CString mstr_pass_level;
	int mn_level_maint;				// ����Ʈ ����(Level 1) ���� �÷���
	int mn_level_teach;				// ƼĪ ����(Level 2) ���� �÷���
	int mn_level_speed;				// ���ǵ� ����(Level 3) ���� �÷���
	int mn_level_admin;				// Admin ����(Level 4) ���� �÷���

	int mn_mot_brd_initial;			// ��� ���� �ʱ�ȭ �۾� �Ϸ� �÷���
	int mn_io_board_initial;

	int mn_home_state[MOT_MAXMOTOR];	// ���� HOMING �Ϸ� �÷���
	int mn_init_state[MAXSITE];		// �ʱ�ȭ �Ϸ� �÷���
	int mn_lotend_state[MAXSITE];		// �ʱ�ȭ �Ϸ� �÷���

	double md_run_speed;			// ��� ���۽� �ӵ���
	double md_manual_speed;			// �޴��� ���۽� �ӵ���

	CString mstr_handler_name;		// �ڵ鷯 �̸�.
	CString mstr_handler_version;	// �ڵ鷯 ����
	CString mstr_handler_date;		// �ڵ鷯 ���� ���� ��¥.

	CString mstr_last_alarm;
	int mn_status;					// ��� �������̳�
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
	 
	int mn_run_time_check;			// Run Time���� �ؾ� �Ұ��̳� �ƴϳ�.
	int mn_runtime_dll_check;		// ��� ����� ������ �ѹ��� �����ϱ� ���ؼ�... 2K7/03/15/ViboX

	int mn_motor_init_check;
	int mn_initiailze_end_flag;
	int n_initiailze;

	int mnSerial_Port_Creating[MAX_PORT];

	int mn_reverse_mode;
	int mn_virtual_mode;

	// ���̸� �߰� [2013.02.14]
	int nInitTryREQ;		// �ʱ�ȭ ���� ��û [READY:�÷��� �ʱ�ȭ, YES:�ʱ�ȭ ��û, NO:�ʱ�ȭ SKIP]
	int nIsAllSiteInitEnd;  // ��� �ʱ�ȭ �۾� �Ϸ� ����
	
	// ���̸� �߰� [2014.02.19]
	int nStart_INIT;	// �ʱ�ȭ �۾� ���������� ����
	
	CString sEqpVersion;

	int  nMenuLock;	// �޴� ��� ���� ����
	int  nSysLock;	// �ý��� lock ����
	int  nSwitchLock;
	
	int  nModeManual;

	int nRecoveryReq;

	// ���̸� �߰� [2014.12.16]
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
//  ��� ������ Sync ���� ����
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
//  �޼��� ����
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

	CString str_fallacy_msg;		// ���� �޼���.
	CString mstr_confirm_msg;		// Ȯ�� �޼���
	CString mstr_typing_msg;		// Ű���� �޼���

	CString mstr_parent_menu;

	CString mstr_password;

	CString mstr_barcode;

	// List�� ��� �ϴٰ� �״� ������ ���� ���ؼ�... 2K11/09/01/ViboX
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
// ���� ��θ� ���� ����ü ���� ����                                             
// ******************************************************************************
struct st_path_param 
{
	CString mstr_run_state;		// 20130830 kjh;
	CString mstr_basic;			// �⺻ ���� ���� ���� ����+���ϸ� ���� ����
	CString mstr_network;			// Network ���� ���� ����+���ϸ� ���� ����
	CString mstr_basic_folder;	// �⺻ ���� ���� ���� ����+���ϸ� ���� ����
	CString mstr_datalogger;	// Data Logger�� ����+���ϸ� ���� ����
		
	CString mstr_path_dvc;		// ����̽��� ƼĪ ���� ���� ���� ���� ���� ���� ����
	CString mstr_path_alarm;	// ��� �˶� ���� ���� ���� ���� ����
	CString mstr_path_op;		// ���۷����� ���� ���� ���� ���� ����
	CString mstr_path_tray;		// Tray�� ���� ���� ���� ���� ���� ���� ����
	CString mstr_path_serial;	// Socket ���� ���� ���� ���� ���� ���� ����
	
	CString mstr_path_file;		// ���� ���� ���� ���� ����
	CString mstr_path_back;		// BACKUP ���� ���� ���� ���� ����
	
	CString mstr_file_basic;	// �⺻ ���� ���� ���� ���ϸ� ���� ����
	CString mstr_file_wait;		// ��� �ð� ���� ���� ���ϸ� ���� ����
	
	CString mstr_operation;		// Operation Log ���� ���� ����.
	CString mstr_interface;		// Interface Log ���� ���� ����.
	CString mstr_machine;		// Machine Log ���� ���� ����.
	CString mstr_barcode;		// Barcode Log ���� ���� ����.
	CString mstr_barcode_fail;	// Barcode Log ���� ���� ����.
	CString mstr_gpib;			// Total Log ���� ���� ����.
	CString mstr_total;			// Total Log ���� ���� ����.
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


	CString sIOMap;		// IO MAP ���� ��ġ ����.
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
	WAIT_RAIL_AC_OFF			= 16,  // [��� �ð��� ����]
	WAIT_ACCY_BOX_NOT_CHK_AC_OFF = 17,
	WAIT_ACCY_OUT_AC_OFF = 18,
};

// ******************************************************************************
// I/O ���� ��� �ð� �� ����Ʈ �ð� ���� ����ü ����                            
// ******************************************************************************
struct st_time_param 
{
	int nWaitTime[MAX_WAIT_TIME_NUM][2];
	int nLimitTime[MAX_WAIT_TIME_NUM];

	// ���̸� �߰� [2014.09.01]
	int nWait_On[MAX_WAIT_TIME_NUM];
	int nWait_Off[MAX_WAIT_TIME_NUM];
	int nWait_Limit[MAX_WAIT_TIME_NUM];
} ;
extern  st_time_param  st_time;
// ******************************************************************************

// *****************************************************************************
//  Tower Lamp ȭ�鿡 ���� ���� ���� ����ü ���� ����                           
// *****************************************************************************
struct st_lamp_param
{
	CString mstr_equip_no;		// ��� ȣ�� ���� ����
	CString mstr_equip_id;	// ��� �ڵ� ���� ����
	CString mstr_ec_equip_id;	// ��� �ڵ� ���� ����	// 20130822 kjh
	int mn_lamp_change_time;	// ���� ���� �ð� ���� ����
} ;
extern  st_lamp_param  st_lamp;
// *****************************************************************************

// ******************************************************************************
// ������ ȭ�� ȯ�� ���� ���� ���� ����ü ����                                   
// ******************************************************************************
struct st_basic_param 
{
	int n_mode_device;
	int n_retry_cnt;
	
	int n_day_start_time;		// Data ��Ͽ�.. �Ϸ��� ���� �ð�...
	int n_day_shift_time;		// Data ��Ͽ�.. Shift �ð�...
			
	CString mstr_device_name;	// ���õ� ����̽� ���� ���� ����
	int mn_alarm_delay_time;
	CString mstr_recipe_name;	// ���õ� ����̽� ���� ���� ����
	
	int mn_electrostatic_check_gap;
	double md_electrostatic_value;

	int mn_network_wait_time;		// Network ��� �ð�
	int mn_network_retry_cnt;		// Network Retry Ƚ��

	// [0] => ������� ���� ���� ( 0:������� ���� ����, 1:������� ���� ���� [�� ��쿡�� �ʱ�ȭ �� ���� ���� ���] )
	// [1] => �ʱ�ȭ �۾� �� ������� ���� ��� ���� ( 0:������� ���þ���, 1:������� ���� )
	// [2] => ���� ��� ���� �ð� ����
	// [3] => �ʱ�ȭ �۾� �� ���� ��� ���� �ð�
	int  nFlag_EqpRecovery[4];

	int nMode_BCR;

	// ���� �ϴ� ��� Master & Slave �� �Ǿ� �ִ�. 
	int nMode_SupplyMode;
	
	// 2.5 Inch, Desktop, M-SATA
	int nMode_SupplyDevice;

	int nMode_SMEMAUsing;

	int nMode_GripRetryCNT;

	// Grip, Vac ��� ��� �߰�
	int nMode_VacUsing;
	int nMode_GripUsing;


	// �̰����� ����ϵ���
	CString sEqpModel;
	CString sEqpNo;
	CString sOperatorId;
	
	CString sDvcName;


	int n_mode_interface;		// ��� ���� ��� ���� ���� (1:ON LINE	0:OFF LINE)
	int nInspectRetry;		// ���ڵ� �� ���� �˻� �õ� Ƚ��


	int nNCH;

	int n_mode_Network;
	int nMode_PlaceType;	// [1:4����, 0:2����]

	int nMode_Slop_Variable;

} ;
extern  st_basic_param  st_basic;
// ******************************************************************************

// ******************************************************************************
// �߻��� �˶� ���� ������ ���� ����ü ����                                      
// ******************************************************************************
struct st_alarm_param 
{
	CStringList mstr_list_jam;	// �߻��� Jam Code�� ����Ʈ
	CStringList mstr_list_jam_time;	//alarm �߻��ð� ���� ����Ʈ
	CString mstr_code;			// �߻��� �˶� �ڵ� ���� ����
	char mc_code[10];
	CString mstr_pcode;			// �߻��� �˶� �ڵ� ���� ����
	int mn_count_mode;			// �˶� ī��Ʈ �÷��� (0:�˶� ī��Ʈ ������    1:�˶� ī��Ʈ ����)
	int mn_type_mode;			// �˶� ���� ���� ���� (0:���    1:���༺    2:��� ����)
	
	long stl_cur_alarm_time;	// �˶� �߻� �ð� ���� ����
	int stl_prev_code;			// �ٷ� �� �߻��� �˶� �ڵ� ���� ����
	long stl_prev_alarm_time;	// �ٷ� �� �߻��� �˶� �߻� �ð� ���� ����
	long stl_alarm_start_time;	// ���� ��ư�� ���������� �߻� �ð� ���� ����
	
	int mn_alarm_occured;		// �˶� �߻� Ƚ�� ���� ����
	int mn_alarm_occured_day;	// �˶� �߻� Ƚ�� ���� ����

	int mn_emo_alarm;

	int  nStopPushInInitial;	// �ʱ�ȭ �۾� �� STOP ��ư ���ȴ��� ����
	int nReset_AlarmStatus;

} ;
extern  st_alarm_param  alarm;
// ******************************************************************************

// ******************************************************************************
// �˶� ������ ���� ����ü ����                                                  
// ******************************************************************************
struct st_dsp_alarm_param 
{
	// **************************************************************************
	// �˶� ���� ǥ�� ��ũ [��� ��ġ], [ũ��], [Į��] ���� ����                 
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
	// ȭ�鿡 ����� �˶� ���� ���� ����                                         
	// **************************************************************************
	int mn_action_cnt;            // ����� �˶� ��ġ ���� ���� ���� ����
	CString mstr_style;           // �˶� ��� ���� ���� ����
	CString mstr_bmp_file;        // �˶� ǥ�� �׸� ���� ���� ����
	CString mstr_repair_msg;      // �˶� ��ġ ���� ���� ����
	// **************************************************************************
	
	// **************************************************************************
	// ���Ͽ� ����� �˶� ���� ���� ����                                         
	// **************************************************************************
	CString mstr_e_content[2000];  // ���Ͽ��� �ε��� �� ���� ���� ���� ����
	CString mstr_e_code[2000];     // �˶� �ڵ� ���� ����
	CString mstr_e_state[2000];    // �˶� ���� ���� ����
	CString mstr_e_part[2000];     // �˶� �߻� ���� ���� ����
	CString mstr_e_msg[2000];      // ���� ���� �˶� �޽��� ���� ����
	// **************************************************************************
	
	int mn_e_count[2000];
	int mn_e_list;
	
	// **************************************************************************
	// ���� �߻��� �˶� ���� ����                                                
	// **************************************************************************
	int mn_cur_state;				// �˶� ���� ���� �÷���
	CString mstr_cur_state;			// �˶� ���
	CString mstr_cur_msg;			// �߻��� �˶� �޽��� ���� ����
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
// ��� ���� ���� �÷��� ���� ���� ����ü ����                                   
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
	// ��� ���� �� �߻��ϴ� �ð� ���� ���� ���� ��� ����                       
	// **************************************************************************
	long MachineTime[4];  // ��� ���� �� ���� �ð� ���� ���� ���� (0:STOP TIME    1:RUN TIME    2:ALARM TIME    3:MAINT TIME)
	// **************************************************************************

	int n_data_save_time;						// Data ���� �ð�

	// **************************************************************************

	////���͵��� ����
	int mn_reinstatement_mode;								// 0:����Ŭ��&�Ϸ� , 1:stop��ȯ��//����� , 2:��ŸƮ Ű ���� ����, 3:������ ...., ���ͿϷ��� 0 �׸��� �� ���� 
	double md_reinstatement_pos[MOT_MAXMOTOR];				// 0: M_ROBOT_X, 1: M_ROBOT_Y, 2: Robot Z, 3: Robot P, 4: M_TRAY_TRANSFER_Y, 5: M_MULTI_STACKER_Z
	int mn_PickerUpDnStatusBackup[2];
	int mn_PickerTurnStatusBackup[2];
	double md_reinstatement_Pos[2][MOT_MAXMOTOR];

	int n_back_site;		//�̵��ߴ� ��ġ
	int n_back_type;		//�̵������ ����� ���� ��ġ����, teach�����ͷ� �̵�����
	int n_back_picker;		//�̵��ߴ� �κ��� ��Ŀ����
	int n_back_x_pos;		//�̵��ߴ� ��ġ�� row
	int n_back_y_pos;		//�̵��ߴ� ��ġ�� cols
	int n_back_z_movepos;	//�̵��ߴ� ��ġ���� z axis�� down���� up�������� ����
	int n_back_tt_movepos;
	int n_back_ms_pos;

	double d_reinstatement_x_pos;		//���͵� ���� x ��ġ��
	double d_reinstatement_y_pos;		//���͵� ���� y ��ġ��
	double d_reinstatement_z_pos;		//���͵� ���� z ��ġ��
	double d_reinstatement_tt_pos;		//���͵� ���� tray transfer ��ġ��
	double d_reinstatement_ms_pos;		//���͵� ���� multi stacker ��ġ��

	int		mn_prev_step[100];			// ���� step (���� �Ǿ����� ���� ����)
	CString	mstr_history_step[100];		// step_history �� 50���� �ش��ϴ� step�� ����
	CString	mstr_history_step_back_up[100];		// step_history �� 50���� �ش��ϴ� step�� ����
	CString mstr_last_change_date[100];	// ���� step���� ����Ǵ� ������ �ð�

 	int iBasic_Apply_Check;	// ������ ���ö��� üũ ( ������ ���ö��� Ŭ���� �ð� �ʱ�ȭ �Ǵ� ���� ������ ) 20130524 kjh

	// ���̸� �߰� [2013.11.30]
	// �˶� �߻��� ���� ���� ���� ��û
	int nReq_AlarmProc;
	// ���̸� �߰� [2013.12.02]
	int nReq_StrtProc;
	int nReq_StopProc;

	// ���̸� ���� [2013.09.01]
	int nAlarmHappened;

	// ����� �߰� [2014.08.27]
	int    n_text_r[2][2];
	int    n_text_g[2][2];
	int    n_text_b[2][2];
	
	int    n_grid_r[2][2];
	int    n_grid_g[2][2];
	int    n_grid_b[2][2];

	int nForceDischarge;  // ���� ����
	int nForceDischargeType;	// 0 : stop State, 1 : ForceDischarge after start

	// EQP2Main ���� ���� Ȯ�� ����
	int nLink_Eqp2Main;

	// Lift���� ������ Accy ���� [WITHOUT ��忡�� ���]
	int nAccyCNT[4];

	// ���� ���� ������ ���� Box ������ ���� ���� �÷��� (Accy Box Conv)
	int nSkipReq_ShiftTray;

	// Buffer Conv �ǰԻ縮 Reset Flag
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

	// �̸� ���� [2015.06.18]
	CString m_sRcvAccyMathCode;	// Main���� ���� Math Code�� �����ϴ� ����

	CString m_sCurrntAccyMathCode;	// ���� �۾� ���� Math Code�� �����ϴ� ����

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
	int nLoadingAccyBoxExist[3];	// Accy Box Conv�� �ִ� ���� Ȯ�� ����
	int nEmptyAccyBoxExist[3];		// Empty Accy Box ���� Ȯ�� ����
	int nClampAccyBoxAccy;			// Accy Box Clamp�� �ִ� ���� Ȯ�� ����

	int nLiftWorkSite;  			// Accy �۾� ���� ���� (�۾� ���� : Front -> Middle -> Rear)
	int nXYZRbtPickerInfo[4];		// [0 = Flag, 1 = Sen][]
	int nNGOut;						// NG Out Ȯ�� ����
	int nNGBuffer[2];				// NG Buffer Accy Ȯ�� ����
	
	int nBufferCovAccyExist[4];		// Buffer Conv Accy Ȯ�� ����
	int nBufferMiddleAccyExist[2];	// Middle Conv Accy Ȯ�� ����
	int nBufferOutAccyExist[4];		// Out Conv Accy Ȯ�� ����

	//////////////////////////////////////////////////////////////////////////
	int nLotStart;
	int nBoxChange;
	int nResp_SupplyCNT;	//������ Accy ����
	int nResp_AccyReadyCNT;	//���� ������ Accy ����

	int nResp_LotType;	// Main���� ���۵� LOT Type�� ���� �ϴ� ����

	int nNGOutCNT;			//NG ó���Ǿ� ������ Accy ����

	int nSupplyTypeMath;

	int nBCR_State[4];		// BCR Read State
	//CString sBCR_Data[4];		// BCR Read Data

	// �Ǽ��縮 �������� ���� �߰�
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
// RS-232C �ø��� ��� ���� ���� ����ü ����                                     
// ******************************************************************************
struct st_serial_param
{
	int	n_connect[MAX_PORT];
	int n_snd_chk[MAX_PORT];         // �޽��� �۽� �Ϸ� �÷���
	int n_rec_chk[MAX_PORT];         // �޽��� ���� �Ϸ� �÷���
	// ���̸� �߰� [2014.10.07]
	int nReq_CommSend[MAX_PORT];	// �޽��� ���� ��û

	CString str_port_chk[MAX_PORT];  // �ø��� ��Ʈ ���� ���� ���� ����
	
	CString str_snd[MAX_PORT];       // �۽� �޽��� ���� ����
	CString str_rec[MAX_PORT];       // ���� �޽��� ���� ����
	
	CString comm_snd[MAX_PORT];      // ����Ʈ ȭ�� ��� �۽� �޽��� ���� ����
	CString comm_rec[MAX_PORT];      // ����Ʈ ȭ�� ��� ���� �޽��� ���� ����
	CString comm_err[MAX_PORT];      // ����Ʈ ȭ�� ��� ���� �޽��� ���� ����
	
	int nAccept_TRIN;	// ������ �޾Ƶ��̶�� �÷���
	int nAccept_TROUT;	// ������ �޾Ƶ��̶�� �÷���
	
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
//  Tower Lamp ȭ�鿡 ���� ���� ���� ����ü ���� ����                           
// *****************************************************************************
// ����� �߰� [2014.07.30]
#define MAX_LAMP_STATUS_TYPE 9

struct tagLAMP_PARAM
{
	int nLampR[MAX_LAMP_STATUS_TYPE];  // RED ���� [0:OFF, 1:ON, 2:FLICK]
	int nLampY[MAX_LAMP_STATUS_TYPE];  // YELLOW ���� [0:OFF, 1:ON, 2:FLICK]
	int nLampG[MAX_LAMP_STATUS_TYPE];  // GREEN ���� [0:OFF, 1:ON, 2:FLICK]
	
	int nModeBuzzer;	// ���� ��� ���
	
	int nTwinkle;	// ��¦�Ÿ��� �ð� ����
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

#define UNTIL_WAITTIME_			100  // [100ms] ���� ���� üũ

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


// ���α׷����� ����� IO MAP ����
struct tagIO_PARAM
{
	//==============================================================//
	// ��� ��ȣ : 00/01
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
	int i_Chk_Door[MAX_DOOR_];											// MAX_DOOR_ = 16��

	//==============================================================//
	// ��� ��ȣ : 02
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
	// ��� ��ȣ : 03
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
	int i_Chk_AccyBoxConvDetection[MAX_DETECTBOX_];						// MAX_DETECTBOX_ = 3��

	//==============================================================//
	// ��� ��ȣ : 04
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
	// ��� ��ȣ : 05
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
	// ��� ��ȣ : 06
	//==============================================================//
	int i_Chk_BoxClampAccyDetection[MAX_SITE_][6];

	//==============================================================//
	// ��� ��ȣ : 07
	//==============================================================//

	int o_Load_BoxClampOnOff;
	int o_Load_BoxPusherFwBw;

	int i_Chk_Load_BoxClampOnCheck;
	int i_Chk_Load_BoxClampOffCheck;

	int i_Chk_Load_BoxPusherFwCheck;
	int i_Chk_Load_BoxPusherBwCheck;


	//==============================================================//
	// ��� ��ȣ : 10
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
	// ��� ��ȣ : 11
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
// 	int i_Chk_BufferAccyConvStopper[MAX_SITE_][2];						//MAX_SITE_ = [���� �Ǵ� ������ ù��° = 0, ���� �Ǵ� ������ �ι�° = 1]
// 	int i_Chk_BufferTryAccyDetection[3][MAX_SITE_];						//MAX_SITE_ = [Left = 0, Right = 1]	
	int i_Chk_NGBufferTryFull;

	//==============================================================//
	// ��� ��ȣ : 12
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
	
	int i_Chk_UnloaderFull[MAX_SITE_];									//MAX_SITE_ = [ ���� ���� ��ġ = 0, �Ǹ����� ��ġ = 1]	
	int i_Chk_ReqFromMain;
	int i_Chk_TrsfFromMain;

	//==============================================================//
	// ��� ��ȣ : 13
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
	// ��� ��ȣ : 14
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

	int i_Chk_BufferAccyConvStopper[5][2];						//MAX_SITE_ = [���� �Ǵ� ������ ù��° = 0, ���� �Ǵ� ������ �ι�° = 1]
	int i_Chk_BufferTryAccyDetection[5][MAX_SITE_];						//MAX_SITE_ = [Left = 0, Right = 1]	
	

	int i_Chk_OutLeftBufferAccyDetection;
	int i_Chk_OutRightBufferAccyDetection;
	




	//==============================================================//
	// ��� ��ȣ : 15
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
	double d_X_Gripper_Size;	// Gripper(0, 2)�� Greipper(1, 3)���� X ����
	double d_Y_Gripper_Size;	// Gripper(0, 1)�� Greipper(2, 3)���� Y ����

	double d_Y_BufferConv_Size;	// Buffer Conv Front�� Rear���� Y ����
};
extern st_Accy_data st_accy;

// ���� ���� ����ü
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

