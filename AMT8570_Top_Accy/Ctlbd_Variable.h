#ifndef _CTLBD_VARIABLE_h__
#define _CTLBD_VARIABLE_h__

#include "CtlBd_Variable.h"

#define MODE_CE				0
///////////////////////////////////
// Motor Board Define
//////////////////////////////////
#define MOT_RECOVERY_TIME	10000	//최소한 목표 지점에 있어야 할 최종 시간 
#define MOT_CLASH_TIME		10000	//10초 이상 계속 돌고 있으면 에러 발생 시킨다 
#define MOT_DEBUG			0 //2011.0303 1

//모터 수량 정보 
#define MOT_MAXMOTOR		12

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

 
#define MOT_RTY_CNT			4	//파스텍 제어기 관련 문제시 재 시도하는 횟수

#define MOT_MOVE_LIMITTIME	60000	//이동 후 3초이내동작 완료할 것 	 
#define MOT_TIMEOUT			90000   //1분의 time out를 가지고 동작한다  //홈 체크시 사용 

//에러등 메세지 지정시사용 
#define MOT_NORMAL_MSG		0
#define MOT_ERR_MSG			1
#define MOT_ERR_CODE		2
#define DBG_NARMAL_MSG		3 //2011,0123  추가 



#define MOT_MOTOR_RATIO		1000				// 화면에 보여주는것과 실제 모터 값과의 배율 

#define MOT_LmMAPINDEX			0	//모터 리스트 모션 정의 
#define MOT_IxMAPINDEX			1	//모터 직선 보간 정의 

#define MAX_POS				22

#define MAX_PLOT			10	// P_LOT 20130902 kjh 5개에서 무제한으로 변경 요청 20130925 kjh
///////////////////////////////////
// IO Board Define
//////////////////////////////////
//picker 정보 
//#define IO_PICKERINFO		4
//#define IO_PICKERNUM		16
//I/O 수량 정보 
//#define IO_MAXIO			500
//I/O on, Off define  




///////////////////////////////////////////
// Control Board(IO, Motor) Board Define
///////////////////////////////////////////

//091010 I/O 동작시 Out/In 동작 체크시 패턴을 아래와 같이 정의하여 동작 완료 여부를 정확히 확인하자 
#define CTL_IOCHK_CLEAR				0 //I/O 동작을 시작하는 위치(Output 시그널 동작시 사용)
#define CTL_IOCHK_START				1 //I/O 아웃풋 동작을 완료하여 기다리는 시간이 시작하는 위치(Input 체크 조건이 처음 만족 하는 위치) 
#define CTL_IOCHK_END				2 //I/O 동작을 input 대기 시간까지 모두 완료하여 끝난 상태
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#define CTLBD_MOTBD_TYPE			1	//0:파스텍 모터보드, 1:커미조아
#define CTLBD_IOBD_TYPE				0	//0:파스텍 I/O 보드, 1:기타

#define CTLBD_IO_DEBUGMODE			0//HSSI_DEBUGMODE				1
#define CTLBD_DEBUG_MODE			0 //2011.0303 1  //1:debug mode를 사용하여 에러시 화면에 메세지를 전송한다 

#define CTL_MODE_NORMAL		0
#define CTL_MODE_PULS		1
#define CTL_MODE_MINUS		2
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//motor, i/O 보드등의 리턴값은 이 것만 사용한다  
//////////////////////////////////////////////////////////////////////////////////////////////////////////
#define CTLBD_RET_ERROR	   -1
#define CTLBD_RET_PROCEED	0
#define CTLBD_RET_GOOD		1
#define CTLBD_RET_RETRY		2
#define CTLBD_RET_SAFETY	3
#define CTLBD_RET_ABORT		4
#define CTLBD_RET_SKIP		5
#define CTLBD_RET_RACKSKIP	6
#define CTLBD_RET_NO_ACC	7

#define CTLBD_NO			0		//어떤 조건이 클리어된 상태 
#define CTLBD_YES			1		//어떤 조건을 요청한 상태 
//#define CTLBD_READY			2		//어떤 조건이 욫어한 후 상대방이 준비를 완료한 상태  

#define CTL_WORK_READY		0
#define CTL_WORK_PROCEED    1
#define CTL_WORK_END		2

#define CTL_IO_PROCEED	3  //2009.5.27 

#define CTL_EJECTOR_ON		2
#define CTL_ON				1
#define CTL_OFF				0

#define CTL_MASK			-2 //메뉴얼로 소켓을 MASK 했을때 
#define CTL_SKIP			-1 //remove만 하고 인서트는 하지 말라고 자넷 서버에서 정보가 왔으면 

#define CTL_NO				0		//어떤 조건이 클리어된 상태 
#define CTL_YES				1		//어떤 조건을 요청한 상태 
#define CTL_DVC_SKIP		2		//피커 정보를 스킵할 때 사용 
#define CTL_END				3		//어떤 조건의 작업을 모두 완료한 상태 //091026 추가 

//#define CTL_READY			2		//어떤 조건을 요청한 후 상대방이 준비를 완료한 상태  

//Find function return value
#define CTL_NOTFIND			-1001		// 빈에 해당하는 정보가 없다
#define CTL_NOSPACE			-1002		// 공간이 없을때 
#define CTL_EMPTY			-1004		// 디바이스가 모두 없을때 
#define CTL_FULL			-1005		// 디바이스가 FULL
#define CTL_TRAYSKIP		-1006		// 트레이에 정보가 모두 없어 트레이를 스킵 가능한 조건 

#define CTL_ERROR			-1			// 함수 리턴 에러 
#define CTL_PROCEED			0			// 함수가 진행중이다 
#define CTL_GOOD			1			// 조건이 만족되었다 참 
#define CTL_RETRY			2			// 다시 처음부터 조건을 재 시도한다 

#define CTL_LOTWAIT				0
#define CTL_LOTSTART			1
#define CTL_LOTEND_START		2
#define CTL_LOTEND_FINISH		5

#define CTL_LOCK			   -1 //요청에 따라 동작을 시작했슴.
#define CTL_CLEAR				0 //아무것도 하지않은 초기상태 
#define CTL_REQ					1 //해당 동작을 요청한 상태 
#define CTL_READY				2 //해당 동작의 요청에 따라 동작을 완료하여 준비된 상태 - 응대한 상태
#define CTL_FREE				3 //요청한 측에서 동작을 모두 완료한 상태로 간섭등을 벗어났을때 상태측이 동작할 수 있도록 자류롭게 데이터를 놓아준다.
#define CTL_CHANGE				4 //목표물을 교체를 요청할때 사용한다(Remove Buffer 교체를 요청할때 사용한다)
#define CTL_SORT				5

#define CTL_DOWN				3
#define CTL_UP					4

#define CTL_COMPLETE			5
#define CTL_BUSY				7
#define CTL_POSSIBLE			8

//가동상태 저장 
#define CTL_dSTOP			0
#define CTL_dRUN			1
#define CTL_dJAM			2
#define CTL_dLOTEND			3
#define CTL_dINIT			4
#define CTL_dWARNING		5
#define CTL_dLOCK			6
#define CTL_dSELFCHECK		7
#define CTL_dMAINT			8

#define CTL_eWARNING		5	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define CTL_RACK_NO			0
#define CTL_RACK_READY		1
#define CTL_RACK_ING		2
#define CTL_RACK_WORKEND	3
//////////////////////////////////////////////////////////////////////////////////////////////////////////

//	Console application 프로그램을 위해서 아래 부분을 정의함
typedef unsigned long int				DWORD;
typedef unsigned short int				WORD;
typedef unsigned char					BYTE;
typedef int								BOOL;
typedef void							*HANDLE;

//typedef int							BOOL;			// 0(FALSE), 1(TRUE)
typedef signed char						INT8;			// -128 .. 127
typedef unsigned char					UINT8;			// 0 .. 255
typedef signed short int				INT16;			// -32768 .. 32767
typedef unsigned short int				UINT16;			// 0 .. 65535
typedef int								INT32;			// -2,147,483,648 .. 2,147,483,647
typedef unsigned int					UINT32;			// 0 .. 4,294,967,295
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////
//일반 선언 
//////////////////////////////
// ******************************************************************************
// 장비 동작 제어 시 필요한 메시지 정의                                          
// ******************************************************************************

#define DBIT0	0x0001 
#define DBIT1	0x0002 
#define DBIT2	0x0004 
#define DBIT3	0x0008 
#define DBIT4	0x0010 
#define DBIT5	0x0020 
#define DBIT6	0x0040 
#define DBIT7	0x0080 

#define DBIT8	0x0100 
#define DBIT9	0x0200 
#define DBIT10	0x0400 
#define DBIT11	0x0800 
#define DBIT12	0x1000 
#define DBIT13	0x2000 
#define DBIT14	0x4000 
#define DBIT15	0x8000 

#define DBIT16	0x00010000 
#define DBIT17	0x00020000 
#define DBIT18	0x00040000 
#define DBIT19	0x00080000 
#define DBIT20	0x00100000 
#define DBIT21	0x00200000 
#define DBIT22	0x00400000 
#define DBIT23	0x00800000 

#define DBIT24	0x01000000 
#define DBIT25	0x02000000 
#define DBIT26	0x04000000 
#define DBIT27	0x08000000 
#define DBIT28	0x10000000 
#define DBIT29	0x20000000 
#define DBIT30	0x40000000 
#define DBIT31	0x80000000 

// 규이리 추가 [2014.10.10]
#define REQ_HOMING_			0
#define REQ_MOVE_START_		1
#define REQ_MOVE_DONE_		2
#define REQ_JOG_MOVE_		3

#define DIR_MINUS_			0
#define DIR_PLUS_			1

enum
{
	// yangkyu [2015.03.25]
	// 모터 값 수정
 	//M_LIFTER_Z1				= 0,
 	//M_LIFTER_Z2				= 1,
 	//M_LIFTER_Z3				= 2,
 	//M_LIFTER_Z4				= 3,
	M_LIFTER_Z3				= 0,
	M_LIFTER_Z1				= 1,
	M_LIFTER_Z4				= 2,
	M_LIFTER_Z2				= 3,

	M_LIFTER_Y				= 4,

	M_BCRREAD_ROBOT_Y		= 5,
	M_BCRREAD_ROBOT_X		= 6,
	M_BCRREAD_ROBOT_Z		= 7,
};


// 악세사리 리프트 관련 모터
// Y축
#define Y_ACC_BOX_AVOID_		0
#define Y_ACC_FRONT_			1
#define Y_ACC_MIDDLE_			2//2017.0713
#define Y_ACC_REAR_				3
// Z축
#define Z_ACC_LIFT_SAFETY_		0
#define Z_ACC_LIFT_LIMIT_		1
#define Z_ACC_LIFT_OFFSET_		2
#define Z_ACC_LIFT_READY_		3

// XYZ 로봇 관련 모터
//Y축
#define Y_RBT_SAFETY_				0
#define Y_RBT_ACC_BCR_				1
#define Y_RBT_ACC_GRIP_FRONT_		2
#define Y_RBT_ACC_GRIP_REAR_		3
#define Y_RBT_ACC_GRIP_BACK_		4//2017.0630
#define Y_RBT_ACC_NG_OUT_			5
#define Y_RBT_ACC_NG_BUFFER_		6
#define Y_RBT_ACC_CONV_R_BUFF_		7
// KILEE [2015.03.17]
#define Y_RBT_ACC_CONV_F_BUFF_		7

//X축
#define X_RBT_ACC_BCR_			0
#define X_RBT_ACC_BCR_OFFSET_		1

//Z축
#define Z_RBT_SAFETY_			0
#define Z_RBT_ACC_GRIP_			1
#define Z_RBT_ACC_NG_BUFFER_PLACE_	2
#define Z_RBT_ACC_NG_BUFFER_PICK_	3
#define Z_RBT_ACC_BUFFER_CONV_PLACE_	4
#define Z_RBT_ACC_BUFFER_CONV_PICK_	5
//GRIP
#define GRIP_RBT_OPEN_					0
#define GRIP_RBT_GRIPPER_	1

struct st_motor_param 
{
	double d_pos[MAX_POS];
	int nTeachingMethod;

	double d_initial_speed;
	double d_limit_position[2];
	double d_spd_vel[3];			// 모터 정속 정보 저장 변수
	double d_cmdpos_backup;
	double d_robot_safety;			// Safety Pos로 이 값보다 클때 Y축이 동작하면 AirBlow Box와 충돌 할수 있다.

	int n_retry_cnt;
	long l_waittime[3];
	
	
	
	
	
	//모터 티칭값을 define하여 위치값을 정의한다 
    double md_pos[MAX_POS];
	
	bool mb_HomeFlag;
	int mn_retry_cnt;
	long ml_retry_time_wait[3];
	int mn_retry_time_flag;
	
	int mn_homechk_flag;
	
    int mn_wait_time;			// 모터 이동시 대기 시간 정보 저장 변수 
	int mn_allow;				// 모터 이동시 허용 오차 정보 저장 변수 
	double md_mm_for_pulse;		// 1mm 이동 시 출력 펄스 수 저장 변수
	
	double md_deviation[3]; //090803 추가 커멘드와 피드백 위치값 차이 문제로 확인위해 추가 
	long   ml_waittime[3];					//time out등 리미트 사간을 체크한다 
	
	double md_initial_speed;
	double md_spd_acc;			// 모터 가속 정보 저장 변수
    double md_spd_dec;			// 모터 감속 정보 저장 변수
    double md_spd_vel;			// 모터 정속 정보 저장 변수
    double md_spd_home;			// 모터 홈체크 속도 정보 저장 변수
    double md_spd_jog;			// 모터 JOG 속도 정보 저장 변수
	
	double md_speed_acc[3];		// 모터 가속 정보 저장 변수
    double md_speed_dec[3];		// 모터 감속 정보 저장 변수
    double md_speed_vel[3];		// 모터 정속 정보 저장 변수
	
    double mcommandposbackup;
	
	int mn_homecheck_method;
	int mn_home_step;
	
	double md_slow_spd_acc;				// 모터 가속 정보 저장 변수
    double md_slow_spd_dec;				// 모터 감속 정보 저장 변수
    double md_slow_spd_vel;				// 모터 정속 정보 저장 변수
	
	double md_fast_spd_acc;				// 모터 가속 정보 저장 변수
    double md_fast_spd_dec;				// 모터 감속 정보 저장 변수
    double md_fast_spd_vel;				// 모터 정속 정보 저장 변수
	
	double md_limit_position[2];		// 0:-, 1:+
	double md_offset_pos[2];			// 0:집기, 1:놓기
	
	int mn_motor_type; //2010.1013 서버 모터 = 0, Step motor = 1, 홈 체크시 에러 나면 홈 체크 플레그(st_handler.n_home_state[nAxis] = NO;) 클리어 여부 판단위해 추가 
	
	//인터럽트 관련 셋팅 변수 
	int		mn_interrupt_flag;	//interrupt 사용 유/무 플레그 
	
	int		mn_interrupt_source;  //인터럽트 처리 조건 
	//n_Mode : 	szCompareSource[4][50] = { 
	//0				"COMMAND Counter",
	//1				"Feedback Counter",
	//2			    "Deviation Counter",
	//3			    "General Counter"
	
	int		mn_interrupt_method; //사용하는 조건 정의  
	//CHAR szCompareMethod[6][50] = {
	//0				"Disable",
	//1				"CMP == COUNTER (No direction)",
	//2				"CMP == COUNTER (Only when counting up)",
	//3				"CMP == COUNTER (Only when counting down)",
	//4				"CMP > COUNTER",
	//5				"CMP < COUNTER"
	
	double	mn_interrupt_position; //인터럽트 발생 위치 
	
	int     mn_InterruptMask_Enable_Flag;  //인터럽트는 이 모드가 CTL_YES가 된 상태에서만 플레그를 변경시키자 
	int		mn_InterruptMask_Disable_Flag; //인터럽트 기능을 한번 ㅁ사용하지 않았다는 플레그(DC Fail 자재를 사용할때 안전 문제로 추가)
	
	long	ml_Set_InterruptMask[20]; //[50]=최대 모터 수, [19]=0~18 까지의 인터럽트 마스크 정보 셋팅 저장 
	long	ml_Get_InterruptMask[20]; //[50]=최대 모터 수, [19]=0~18 까지의 인터럽트 마스크 정보 읽어들인다

	// 규이리 추가 [2014.09.06]
	int nNeedsHoming;	// 모터 Homing 작업 필요 여부

	// baeksh [2014.10.16]
	int nStepMotor;	// Step Motor MotionDone 
} ;
extern  st_motor_param  st_motor[MOT_MAXMOTOR];


struct st_ctlbd_param 
{
	long l_LmIndexmotion_time[16][3]; //이동시간 체크 
	long l_IxIndexmotion_time[8][3];  //이동시간 체크 
	long l_Singlemotion_time[MOT_MAXMOTOR][3]; //단축 모터의 이동위치 체크시간 

	int n_motorbd_init_end; //보드 초기화 완료 
	int n_iobd_init_end;    //i/o board 초기화 완료 

	int n_TotalMotBoard_Number;		//이 장비의 전체 모터보드 수량
	int n_TotalMotorAxis_Number;	//전체 모터축의 수량 
	int n_MotorAxis_Number[10];	//최대 10개까지 보드 사용가능  //각각의 모터보드에 있는 축의 수량 
//	int MotorBoard_Axis_Number[11];//[0]:0보드의 모터축수, [1]:1보드의 모터 축수, .....  [10]:전체 모터보드의 수량  

	//2009.6.9 추가 
	int n_motmove_limit_time; //기본 5초, n_motmove_limit_time = MOT_MOVE_LIMITTIME * ((100 - speedrate) + 1)

} ;
extern  st_ctlbd_param  st_ctlbd;
extern  st_ctlbd_param	st_reco_ctlbd;  

struct st_linearmotion_param
{	
	int		 n_indexmap_init;	   //현재 index map이 초기화가 끝났는지 확인한다 
	long     l_AxisCnt;            //현재의 IndexNum에서 사용 가능한 모터 수량 최대 4개  
	long	 lp_AxisNum[4];        //현재의 IndexNum에서 사용하는 모터 실재 번호를 가진다 
	double	 dp_PosList[4];		   //최대 4 포인트를 한번에 이동가능하다 
	double   dp_SpdRatio[3];        //속도비율 [0]:Vel, [1]:Acc, [2]:Dec
};
extern st_linearmotion_param st_linearmot[8];	 //[8] = LmIndexMap Number 0~ 7까지 가능  

struct st_coordmotion_param
{
	int		 n_indexmap_init;		//현재 index map이 셋팅이 초기화가 끝났는지 확인한다 
	long	 l_TotalMoveCnt;		//총 동작할 구간 정의 보간동작도 한번으로 한다 (최대 10회) 
	long     l_TotalAxisCnt;        //현재의 IndexNum에서 사용 가능한 모터 수량 
	long     lp_TotalAxisNum[4];    //현재의 IndexNum에서 사용하는 모터번호를 가진다 
	
	long    lp_IxIndexNum[8];	    //최대 0 ~ 7번, 보간 동작시 사용할 수 있는 번호 
	long    lp_SubAxisCnt[8];	    //현재 index에서 동작할 모터 수량
	long    lp_SubAxisNum[8][4];    //[10] = index 구간 10회 //[0][0]:첫번째 모터번호, [0][1]:2번째 모터번호  
	double	dp_PosList[8][4];	    //[10] = 10회구간 , //[0]=앞축의 모터 이동 값(단축시 이것만 사용), [1]=뒤축의 모터 이동 값 (보간 동작시 사용)
	double	dp_AxisRatio[8][3];		    //최대 두축의 이동구간 속도 비율 ,[0]:vel, [1]:acc, [2]:dec ratio
};
extern st_coordmotion_param st_coordmot[16];	 //[16] = LmIndexMap Number 0~ 15까지 가능  

#endif