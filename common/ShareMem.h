// ShareMem.h: interface for the ShareMem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHAREMEM_H__3C625654_DC90_4555_BDDA_4AF6D32AB0A3__INCLUDED_)
#define AFX_SHAREMEM_H__3C625654_DC90_4555_BDDA_4AF6D32AB0A3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>
#include <vector>

enum SM_TYPE 
{
	//ASS'Y  : LOB BUFFER
	SMT_O_LOB_BUFFER_LONG_UPDN_CYL        = 0,  //S0000
	SMT_O_LOB_BUFFER_SHORT_UPDN_CYL        = 1,  //S0001
	SMT_O_LOB_BUFFER_STACKER_LATCH_ONOFF_CYL     = 2,  //S0002
	SMT_O_LOB_BUFFER_STACKER_TRAY_STOPPER_UPDN_CYL    = 3,  //S0003
	SMT_I_LOB_BUFFER_LONG_UP_CHK_SENSOR       = 4,  //PS0000
	SMT_I_LOB_BUFFER_LONG_DN_CHK_SENSOR       = 5,  //PS0001
	SMT_I_LOB_BUFFER_SHORT_UP_CHK_SENSOR      = 6,  //PS0002
	SMT_I_LOB_BUFFER_SHORT_DN_CHK_SENSOR      = 7,  //PS0003
	SMT_I_LOB_BUFFER_STACKER_LATCH1_ON_CHK_SENSOR    = 8,  //PS0004
	SMT_I_LOB_BUFFER_STACKER_LATCH1_OFF_CHK_SENSOR    = 9,  //PS0005
	SMT_I_LOB_BUFFER_STACKER_LATCH2_ON_CHK_SENSOR    = 10,  //PS0006
	SMT_I_LOB_BUFFER_STACKER_LATCH2_OFF_CHK_SENSOR    = 11,  //PS0007
	SMT_I_LOB_BUFFER_STACKER_STOPPER_UP_CHK_SENSOR    = 12,  //PS0008
	SMT_I_LOB_BUFFER_STACKER_STOPPER_DN_CHK_SENSOR    = 13,  //PS0009
	SMT_I_LOB_BUFFER_TRAY_INPUT_CHK_SENSOR      = 14,  //PS0010
	SMT_I_LOB_BUFFER_TRAY_ARRIVAL_CHK_SENSOR     = 15,  //PS0011
	
	//ASS'Y  : LOADING SITE
	SMT_O_LOADING_TRAY_ALIGN1_CYL        = 100,  //S0100
	SMT_O_LOADING_TRAY_ALIGN2_CYL        = 101,  //S0101
	SMT_O_LOADING_TRAY_UP_CYL         = 102,  //S0102
	SMT_O_LOADING_TRAY_DN_CYL		= 103, //S0103
	SMT_O_LOADING_TRAY_STOPPER_UPDN_CYL = 104, //S0104
	
	SMT_I_LOADING_TRAY_INPUT_CHK_SENSOR       = 105,  //PS0100
	SMT_I_LOADING_TRAY_LOWER_SITE_CHK_SENSOR     = 106,  //PS0101
	SMT_I_LOADING_TRAY_UPPER_SITE_CHK_SENSOR     = 107,  //PS0102
	SMT_I_LOADING_TRAY_ALIGN1_ON_CHK_SENSOR      = 108,  //PS0103
	SMT_I_LOADING_TRAY_ALIGN1_OFF_CHK_SENSOR     = 109,  //PS0104
	SMT_I_LOADING_TRAY_ALIGN2_ON_CHK_SENSOR      = 110,  //PS0105
	SMT_I_LOADING_TRAY_ALIGN2_OFF_CHK_SENSOR     = 111,  //PS0106
	SMT_I_LOADING_TRAY_UP_CHK_SENSOR       = 112,  //PS0107
	SMT_I_LOADING_TRAY_DN_CHK_SENSOR       = 113,  //PS0108
	SMT_I_LOADING_TRAY_STOPPER_UP_CHK_SENSOR       = 114,  //PS0109
	SMT_I_LOADING_TRAY_STOPPER_DN_CHK_SENSOR       = 115,  //PS0110
	
	
	//ASS'Y  : EMPTY SITE
	SMT_O_EMPTY_TRAY_ALIGN1_CYL         = 200,  //S0200
	SMT_O_EMPTY_TRAY_ALIGN2_CYL         = 201,  //S0201
	SMT_O_EMPTY_TRAY_UP_CYL  = 202, //S0202
	SMT_O_EMPTY_TRAY_DN_CYL   = 203, //S0203
	SMT_O_EMPTY_TRAY_STOPPER_UPDN_CYL   = 204, //S0203
	
	SMT_I_EMPTY_TRAY_INPUT_CHK_SENSOR = 204,
	SMT_I_EMPTY_TRAY_WORK_POS_CHK_SENSOR = 205,
	SMT_I_EMPTY_TRAY_LOWER_CHK_SENSOR = 206,
	SMT_I_EMPTY_TRAY_UPPER_CHK_SENSOR = 207,
	SMT_I_EMPTY_TRAY_ALIGN1_ON_CHK_SENSOR      = 208,  //PS0201
	SMT_I_EMPTY_TRAY_ALIGN1_OFF_CHK_SENSOR      = 209,  //PS0202
	SMT_I_EMPTY_TRAY_ALIGN2_ON_CHK_SENSOR      = 210,  //PS0203
	SMT_I_EMPTY_TRAY_ALIGN2_OFF_CHK_SENSOR      = 211,  //PS0204
	SMT_I_EMPTY_TRAY_UP_CHK_SENSOR = 212,
	SMT_I_EMPTY_TRAY_DN_CHK_SENSOR = 213,
	SMT_I_EMPTY_TRAY_STOPPER_UP_CHK_SENSOR = 214,
	SMT_I_EMPTY_TRAY_STOPPER_DN_CHK_SENSOR = 215,
	
	//ASS'Y  : STACKER SITE 1
	SMT_O_SITE_1_TRAY_ALIGN1_CYL        = 300,  //S0300
	SMT_O_SITE_1_TRAY_ALIGN2_CYL        = 301,  //S0301
	SMT_O_SITE_1_TRAY_UP_CYL   = 302, //S0302
	SMT_O_SITE_1_TRAY_DN_CYL   = 303, //S0302
	
	SMT_I_SITE_1_TRAY_INPUT_CHK_SENSOR        = 304,  //PS0300
	
	SMT_I_SITE_1_TRAY_LOWER_CHK_SENSOR = 306,
	SMT_I_SITE_1_TRAY_UPPER_CHK_SENSOR = 307,
	SMT_I_SITE_1_TRAY_ALIGN1_ON_CHK_SENSOR      = 308,  //PS0304
	SMT_I_SITE_1_TRAY_ALIGN1_OFF_CHK_SENSOR      = 309,  //PS0305
	SMT_I_SITE_1_TRAY_ALIGN2_ON_CHK_SENSOR      = 310,  //PS0306
	SMT_I_SITE_1_TRAY_ALIGN2_OFF_CHK_SENSOR      = 311,  //PS0307
	SMT_I_SITE_1_TRAY_UP_CHK_SENSOR = 312,
	SMT_I_SITE_1_TRAY_DN_CHK_SENSOR = 313,
	SMT_I_LOB_STACKER_TRAY_LATCH_CHK_SENSOR = 314,
	SMT_I_LOB_STACKER_TRAY_FULL_CHK_SENSOR = 315,
	
	//ASS'Y  : STACKER SITE 2
	SMT_O_SITE_2_TRAY_ALIGN1_CYL        = 400,  //S0300
	SMT_O_SITE_2_TRAY_ALIGN2_CYL        = 401,  //S0301
	SMT_O_SITE_2_TRAY_UP_CYL   = 402, //S0302
	SMT_O_SITE_2_TRAY_DN_CYL   = 403, //S0302
	
	SMT_I_SITE_2_TRAY_INPUT_CHK_SENSOR        = 404,  //PS0300
	
	SMT_I_SITE_2_TRAY_LOWER_CHK_SENSOR = 406,
	SMT_I_SITE_2_TRAY_UPPER_CHK_SENSOR = 407,
	SMT_I_SITE_2_TRAY_ALIGN1_ON_CHK_SENSOR      = 408,  //PS0304
	SMT_I_SITE_2_TRAY_ALIGN1_OFF_CHK_SENSOR      = 409,  //PS0305
	SMT_I_SITE_2_TRAY_ALIGN2_ON_CHK_SENSOR      = 410,  //PS0306
	SMT_I_SITE_2_TRAY_ALIGN2_OFF_CHK_SENSOR      = 411,  //PS0307
	SMT_I_SITE_2_TRAY_UP_CHK_SENSOR = 412,
	SMT_I_SITE_2_TRAY_DN_CHK_SENSOR = 413,
	
	//ASS'Y  : STACKER SITE 3
	SMT_O_SITE_3_TRAY_ALIGN1_CYL        = 500,  //S0300
	SMT_O_SITE_3_TRAY_ALIGN2_CYL        = 501,  //S0301
	SMT_O_SITE_3_TRAY_UP_CYL   = 502, //S0302
	SMT_O_SITE_3_TRAY_DN_CYL   = 503, //S0302
	
	SMT_I_SITE_3_TRAY_INPUT_CHK_SENSOR        = 504,  //PS0300
	SMT_I_SITE_3_TRAY_WORK_POS_CHK_SENSOR = 505,
	SMT_I_SITE_3_TRAY_LOWER_CHK_SENSOR = 506,
	SMT_I_SITE_3_TRAY_UPPER_CHK_SENSOR = 507,
	SMT_I_SITE_3_TRAY_ALIGN1_ON_CHK_SENSOR      = 508,  //PS0304
	SMT_I_SITE_3_TRAY_ALIGN1_OFF_CHK_SENSOR      = 509,  //PS0305
	SMT_I_SITE_3_TRAY_ALIGN2_ON_CHK_SENSOR      = 510,  //PS0306
	SMT_I_SITE_3_TRAY_ALIGN2_OFF_CHK_SENSOR      = 511,  //PS0307
	SMT_I_SITE_3_TRAY_UP_CHK_SENSOR = 512,
	SMT_I_SITE_3_TRAY_DN_CHK_SENSOR = 513,
	
	//ASS'Y  : MULTI STACKER
	SMT_O_MULTI_STACKER_PUSHER_CYL        = 600,  //S0600
	SMT_O_TRAY_TRANSFER_UPDN_CYL        = 601,  //S0601
	SMT_O_TRAY_TRANSFER_PICKER_CYL        = 602,  //S0602
	
	SMT_I_MULTI_STACKER_PUSHER_FORWARD_CHK      = 604,  //PS0600
	SMT_I_MULTI_STACKER_PUSHER_BACKWARD_CHK      = 605,  //PS0601
	SMT_I_MULTI_STACKER_1_TRAY_CHK_SENSOR      = 606,  //PS0602
	SMT_I_MULTI_STACKER_2_TRAY_CHK_SENSOR      = 607,  //PS0603
	SMT_I_MULTI_STACKER_3_TRAY_CHK_SENSOR      = 608,  //PS0604
	
	SMT_I_TRAY_TRANSFER_UP_CHK_SENSOR       = 611,  //PS1000
	SMT_I_TRAY_TRANSFER_DN_CHK_SENSOR       = 612,  //PS1001
	SMT_I_TRAY_TRANSFER_PICKER_ON_CHK_SENSOR     = 613,  //PS1002
	SMT_I_TRAY_TRANSFER_PICKER_OFF_CHK_SENSOR     = 614,  //PS1003
	SMT_I_TRAY_TRANSFER_PCB_CHK_SENSOR       = 615,  //PS1004
	
	//ASS'Y  : ROBOT
	SMT_O_ROBOT_ROTATE_FORWARD_CYL        = 700,  //S0700
	SMT_O_ROBOT_ROTATE_BACKWARD_CYL        = 701,  //S0701
	SMT_O_ROBOT_PICKER_01_UPDN_CYL        = 702,  //S0702
	SMT_O_ROBOT_PICKER_02_UPDN_CYL        = 703,  //S0703
	SMT_O_ROBOT_PICKER_01_VACUUM_SOL      = 704,  //S0704
	SMT_O_ROBOT_PICKER_02_VACUUM_SOL      = 705,  //S0705
	SMT_O_ROBOT_PICKER_01_EJECTOR_SOL      = 706,  //S0704
	SMT_O_ROBOT_PICKER_02_EJECTOR_SOL      = 707,  //S0705
	SMT_I_ROBOT_ROTATE_ON_CHK_SENSEOR       = 708,  //PS0700
	SMT_I_ROBOT_ROTATE_OFF_CHK_SENSOR       = 709,  //PS0701
	SMT_I_ROBOT_PICKER_01_UP_CHK_SENSOR       = 710,  //PS0702
	SMT_I_ROBOT_PICKER_01_DN_CHK_SENSOR       = 711,  //PS0703
	SMT_I_ROBOT_PICKER_02_UP_CHK_SENSOR       = 712,  //PS0704
	SMT_I_ROBOT_PICKER_02_DN_CHK_SENSOR       = 713,  //PS0705
	SMT_I_ROBOT_PICKER_01_PCB_CHK_SENSOR      = 714,  //PS0706
	SMT_I_ROBOT_PICKER_02_PCB_CHK_SENSOR      = 715,  //PS0707
	
	//ASS'Y  : GOOD STACKER
	SMT_O_GOOD_STACKER_LONG_UPDN_CYL   = 1000,
	SMT_O_GOOD_STACKER_SHORT_UPDN_CYL   = 1001,
	SMT_O_GOOD_STACKER_LATCH_CYL   = 1002,
	SMT_O_GOOD_STACKER_STOPPER_CYL   = 1003,
	
	SMT_I_GOOD_STACKER_TRAY_CHK_SENSOR = 1004,
	SMT_I_GOOD_STACKER_TRAY_FULL_CHK_SENSOR = 1005,
	SMT_I_GOOD_STACKER_LONG_UP_CHK_SENSOR = 1006,
	SMT_I_GOOD_STACKER_LONG_DN_CHK_SENSOR = 1007,
	SMT_I_GOOD_STACKER_SHORT_UP_CHK_SENSOR = 1008,
	SMT_I_GOOD_STACKER_SHORT_DN_CHK_SENSOR = 1009,
	SMT_I_GOOD_STACKER_LATCH1_ON_CHK_SENSOR = 1010,
	SMT_I_GOOD_STACKER_LATCH1_OFF_CHK_SENSOR = 1011,
	SMT_I_GOOD_STACKER_LATCH2_ON_CHK_SENSOR = 1012,
	SMT_I_GOOD_STACKER_LATCH2_OFF_CHK_SENSOR = 1013,
	SMT_I_GOOD_STACKER_STOPPER_UP_CHK_SENSOR = 1014,
	SMT_I_GOOD_STACKER_STOPPER_DN_CHK_SENSOR = 1015,
	
	
	//ASS'Y  : EMPTY STACKER
	SMT_O_EMPTY_STACKER_LONG_UPDN_CYL       = 1100,  //S1100
	SMT_O_EMPTY_STACKER_SHORT_UPDN_CYL       = 1101,  //S1101
	SMT_O_EMPTY_STACKER_LATCH_CYL        = 1102,  //S1102
	
	SMT_I_EMPTY_STACKER_TRAY_CHK_SENSOR       = 1104,  //PS1100
	SMT_I_EMPTY_STACKER_TRAY_FULL_CHK_SENSOR     = 1105,  //PS1101
	SMT_I_EMPTY_STACKER_TRAY_LONG_UP_CHK_SENSOR     = 1106,  //PS1102
	SMT_I_EMPTY_STACKER_TRAY_LONG_DN_CHK_SENSOR     = 1107,  //PS1103
	SMT_I_EMPTY_STACKER_TRAY_SHORT_UP_CHK_SENSOR    = 1108,  //PS1104
	SMT_I_EMPTY_STACKER_TRAY_SHORT_DN_CHK_SENSOR    = 1109,  //PS1105
	SMT_I_EMPTY_STACKER_TRAY_LATCH1_ON_CHK_SENSOR    = 1110,  //PS1106
	SMT_I_EMPTY_STACKER_TRAY_LATCH1_OFF_CHK_SENSOR    = 1111,  //PS1107 
	SMT_I_EMPTY_STACKER_TRAY_LATCH2_ON_CHK_SENSOR    = 1112,  //PS1106
	SMT_I_EMPTY_STACKER_TRAY_LATCH2_OFF_CHK_SENSOR    = 1113,  //PS1107 
	
	//ASS'Y  : STACKER 1
	SMT_O_SITE_1_STACKER_TRAY_LIFTER_UPDN_CYL     = 1200,  //S1200
	SMT_O_SITE_1_TRAY_CONVEYOR_MOTOR_ONOFF      = 1201,  //S0303
	
	SMT_I_SITE_1_STACKER_TRAY_CHK_SENSOR      = 1204,  //PS1200
	SMT_I_SITE_1_STACKER_TRAY_LATCH_CHK_SENSOR      = 1205,  //PS1200
	SMT_I_SITE_1_STACKER_TRAY_FULL_CHK_SENSOR     = 1206,  //PS1201
	SMT_I_SITE_1_STACKER_TRAY_LIFTER_UP_CHK_SENSOR    = 1207,  //PS1202
	SMT_I_SITE_1_STACKER_TRAY_LIFTER_DN_CHK_SENSOR    = 1208,  //PS1203
	
	//ASS'Y  : STACKER 2
	SMT_O_SITE_2_STACKER_TRAY_LIFTER_UPDN_CYL     = 1300,  //S1300
	SMT_O_SITE_2_TRAY_CONVEYOR_MOTOR_ONOFF      = 1301,  //S0303
	
	SMT_I_SITE_2_STACKER_TRAY_CHK_SENSOR      = 1304,  //PS1300
	SMT_I_SITE_2_STACKER_TRAY_LATCH_CHK_SENSOR      = 1305,  //PS1300
	SMT_I_SITE_2_STACKER_TRAY_FULL_CHK_SENSOR     = 1306,  //PS1301
	SMT_I_SITE_2_STACKER_TRAY_LIFTER_UP_CHK_SENSOR    = 1307,  //PS1302
	SMT_I_SITE_2_STACKER_TRAY_LIFTER_DN_CHK_SENSOR    = 1308,  //PS1303
	
	//ASS'Y  : STACKER 3
	SMT_O_SITE_3_STACKER_TRAY_LIFTER_UPDN_CYL     = 1400,  //S1400
	SMT_O_MULTI_BIN_TRAY_LIFTER_UPDN_CYL = 1401,
	SMT_O_SITE_3_TRAY_CONVEYOR_MOTOR_ONOFF      = 1402,  //S0303
	SMT_O_MULTI_STACKER_TRAY_CONVEYOR_MOTOR_ONOFF    = 1403,  //S0602
	
	SMT_I_SITE_3_STACKER_TRAY_CHK_SENSOR      = 1404,  //PS1400
	SMT_I_SITE_3_STACKER_TRAY_LATCH_CHK_SENSOR      = 1405,  //PS1400
	SMT_I_SITE_3_STACKER_TRAY_FULL_CHK_SENSOR     = 1406,  //PS1401
	SMT_I_SITE_3_STACKER_TRAY_LIFTER_UP_CHK_SENSOR    = 1407,  //PS1402
	SMT_I_SITE_3_STACKER_TRAY_LIFTER_DN_CHK_SENSOR    = 1408,  //PS1403
	SMT_I_MULTI_BIN_TRAY_CHK_SENSOR  = 1409,
	SMT_I_MULTI_BIN_TRAY_FULL_CHK_SENSOR  = 1410,
	SMT_I_MULTI_BIN_TRAY_LIFTER_UP_CHK_SENSOR  = 1411,
	SMT_I_MULTI_BIN_TRAY_LIFTER_DN_CHK_SENSOR  = 1412,
	SMT_I_MULTI_STACKER_BOTTOM_DOOR1_CHK_SENSOR = 1413,
	SMT_I_MULTI_STACKER_BOTTOM_DOOR2_CHK_SENSOR = 1414,

	//ASS'Y  : TL & DOOR
	SMT_O_START_SWITCH_LAMP          = 1500,  //S1500
	SMT_O_STOP_SWITCH_LAMP          = 1501,  //S1501
	SMT_O_ALARM_CLEAR_LAMP          = 1502,  //S1502
	SMT_O_BUZZER_OFF_LAMP          = 1503,  //S1503
	SMT_I_START_SWITCH_CHK          = 1508,  //PS1500
	SMT_I_STOP_SWITCH_CHK          = 1509,  //PS1501
	SMT_I_ALARM_CLEAR_CHK          = 1510,  //PS1502
	SMT_I_BUZZER_OFF_CHK          = 1511,  //PS1503
	SMT_I_AUTO_MODE_SWITCH_CHK         = 1512,  //PS1504
	SMT_I_MANUAL_MODE_SWITCH_CHK        = 1513,  //PS1505
	SMT_I_FRONT_SELECT_SWITCH_CHK        = 1514,  //PS1506
	SMT_I_REAR_SELECT_SWITCH_CHK        = 1515,  //PS1507
	
	//ASS'Y  : MC & EMO
	SMT_O_MAIN_AIR_ONOFF          = 1600,  //S1600
	SMT_O_TOWER_GREEN_LAMP          = 1601,  //S1601
	SMT_O_TOWER_YELLOW_LAMP          = 1602,  //S1602
	SMT_O_TOWER_RED_LAMP          = 1603,  //S1603
	SMT_O_BUZZER1_ONOFF           = 1604,  //S1604
	SMT_O_BUZZER2_ONOFF           = 1605,  //S1605
	SMT_O_DOOR_LOCK_ONOFF          = 1606,  //S1606
	SMT_I_MAIN_AIR_CHK           = 1608,  //PS1600
	SMT_I_DOOR1_OPENCLOSE_CHK         = 1609,  //PS1601
	SMT_I_DOOR2_OPENCLOSE_CHK         = 1610,  //PS1602
	SMT_I_DOOR3_OPENCLOSE_CHK         = 1611,  //PS1603
	SMT_I_DOOR4_OPENCLOSE_CHK         = 1612,  //PS1604
	SMT_I_DOOR5_OPENCLOSE_CHK         = 1613,  //PS1604
	SMT_I_DOOR6_OPENCLOSE_CHK         = 1614,  //PS1604
	SMT_I_LIGTH_CURTAIN_CHK         = 1615,  //PS160
	
	//ASS'Y  : SMEMA/MC/EMO
	SMT_O_FRONT_SHUTTLE_REQUEST_SIGNAL       = 1700,  //S1700
	SMT_O_LOB_BUFFER_CONVEYOR_MOTOR_ONOFF      = 1703,  //S0103
	SMT_O_LOADING_TRAY_CONVEYOR_MOTOR_ONOFF      = 1704,  //S0104
	SMT_O_EMPTY_TRAY_CONVEYOR_MOTOR_ONOFF      = 1705,  //S0203
	SMT_O_EMPTY_TRAY_CONVEYOR_CW_ONOFF       = 1706,  //S0204
	SMT_O_EMPTY_TRAY_CONVEYOR_CCW_ONOFF       = 1707,  //S0205
	
	SMT_I_FRONT_SHUTTLE_TRANSFER_SIGNAL_CHK  = 1708,  //PS1700
	SMT_I_MC1_CHK            = 1712,  //PS1704
	SMT_I_MC2_CHK            = 1713,  //PS1705
	SMT_I_EMO_SWITCH1_CHK          = 1714,  //PS1706
	SMT_I_EMO_SWITCH2_CHK          = 1715,  //PS1707
	
	//MOTOR TARGET
	SMT_MOTOR_ROBOT_X_TARGET,
	SMT_MOTOR_ROBOT_Y_TARGET,
	SMT_MOTOR_ROBOT_Z_TARGET,
	SMT_MOTOR_ROBOT_PITCH_TARGET,
	SMT_MOTOR_TRAY_TRANSFER_Y_TARGET,
	SMT_MOTOR_MULTISTACKER_TARGET,
	
	// MOTOR POSITION
	SMT_MOTOR_ROBOT_X_POSITION,
	SMT_MOTOR_ROBOT_Y_POSITION,
	SMT_MOTOR_ROBOT_Z_POSITION,
	SMT_MOTOR_ROBOT_PITCH_POSITION,
	SMT_MOTOR_TRAY_TRANSFER_Y_POSITION,
	SMT_MOTOR_MULTISTACKER_POSITION,
	
	SMT_COUNT,
};





struct st_shareMem
{
	SM_TYPE smt;

	HANDLE hFMap;
	char* pInFile;
};

class ShareMem  
{
public:
	ShareMem();
	virtual ~ShareMem();

public:
	void Initialize();
	void UnInitialize();
	void SetData( SM_TYPE smt, std::string strData );
	std::string GetData( SM_TYPE smt );

protected:
	std::vector<st_shareMem> m_vecSM;
};

extern ShareMem g_sm;

#endif // !defined(AFX_SHAREMEM_H__3C625654_DC90_4555_BDDA_4AF6D32AB0A3__INCLUDED_)
