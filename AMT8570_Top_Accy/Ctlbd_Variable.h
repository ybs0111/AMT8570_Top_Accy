#ifndef _CTLBD_VARIABLE_h__
#define _CTLBD_VARIABLE_h__

#include "CtlBd_Variable.h"

#define MODE_CE				0
///////////////////////////////////
// Motor Board Define
//////////////////////////////////
#define MOT_RECOVERY_TIME	10000	//�ּ��� ��ǥ ������ �־�� �� ���� �ð� 
#define MOT_CLASH_TIME		10000	//10�� �̻� ��� ���� ������ ���� �߻� ��Ų�� 
#define MOT_DEBUG			0 //2011.0303 1

//���� ���� ���� 
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

//���� ���� ���� 
#define MOT_PLUS			1
#define MOT_MINUS			0

 
#define MOT_RTY_CNT			4	//�Ľ��� ����� ���� ������ �� �õ��ϴ� Ƚ��

#define MOT_MOVE_LIMITTIME	60000	//�̵� �� 3���̳����� �Ϸ��� �� 	 
#define MOT_TIMEOUT			90000   //1���� time out�� ������ �����Ѵ�  //Ȩ üũ�� ��� 

//������ �޼��� �����û�� 
#define MOT_NORMAL_MSG		0
#define MOT_ERR_MSG			1
#define MOT_ERR_CODE		2
#define DBG_NARMAL_MSG		3 //2011,0123  �߰� 



#define MOT_MOTOR_RATIO		1000				// ȭ�鿡 �����ִ°Ͱ� ���� ���� ������ ���� 

#define MOT_LmMAPINDEX			0	//���� ����Ʈ ��� ���� 
#define MOT_IxMAPINDEX			1	//���� ���� ���� ���� 

#define MAX_POS				22

#define MAX_PLOT			10	// P_LOT 20130902 kjh 5������ ���������� ���� ��û 20130925 kjh
///////////////////////////////////
// IO Board Define
//////////////////////////////////
//picker ���� 
//#define IO_PICKERINFO		4
//#define IO_PICKERNUM		16
//I/O ���� ���� 
//#define IO_MAXIO			500
//I/O on, Off define  




///////////////////////////////////////////
// Control Board(IO, Motor) Board Define
///////////////////////////////////////////

//091010 I/O ���۽� Out/In ���� üũ�� ������ �Ʒ��� ���� �����Ͽ� ���� �Ϸ� ���θ� ��Ȯ�� Ȯ������ 
#define CTL_IOCHK_CLEAR				0 //I/O ������ �����ϴ� ��ġ(Output �ñ׳� ���۽� ���)
#define CTL_IOCHK_START				1 //I/O �ƿ�ǲ ������ �Ϸ��Ͽ� ��ٸ��� �ð��� �����ϴ� ��ġ(Input üũ ������ ó�� ���� �ϴ� ��ġ) 
#define CTL_IOCHK_END				2 //I/O ������ input ��� �ð����� ��� �Ϸ��Ͽ� ���� ����
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#define CTLBD_MOTBD_TYPE			1	//0:�Ľ��� ���ͺ���, 1:Ŀ������
#define CTLBD_IOBD_TYPE				0	//0:�Ľ��� I/O ����, 1:��Ÿ

#define CTLBD_IO_DEBUGMODE			0//HSSI_DEBUGMODE				1
#define CTLBD_DEBUG_MODE			0 //2011.0303 1  //1:debug mode�� ����Ͽ� ������ ȭ�鿡 �޼����� �����Ѵ� 

#define CTL_MODE_NORMAL		0
#define CTL_MODE_PULS		1
#define CTL_MODE_MINUS		2
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//motor, i/O ������� ���ϰ��� �� �͸� ����Ѵ�  
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

#define CTLBD_NO			0		//� ������ Ŭ����� ���� 
#define CTLBD_YES			1		//� ������ ��û�� ���� 
//#define CTLBD_READY			2		//� ������ �C���� �� ������ �غ� �Ϸ��� ����  

#define CTL_WORK_READY		0
#define CTL_WORK_PROCEED    1
#define CTL_WORK_END		2

#define CTL_IO_PROCEED	3  //2009.5.27 

#define CTL_EJECTOR_ON		2
#define CTL_ON				1
#define CTL_OFF				0

#define CTL_MASK			-2 //�޴���� ������ MASK ������ 
#define CTL_SKIP			-1 //remove�� �ϰ� �μ�Ʈ�� ���� ����� �ڳ� �������� ������ ������ 

#define CTL_NO				0		//� ������ Ŭ����� ���� 
#define CTL_YES				1		//� ������ ��û�� ���� 
#define CTL_DVC_SKIP		2		//��Ŀ ������ ��ŵ�� �� ��� 
#define CTL_END				3		//� ������ �۾��� ��� �Ϸ��� ���� //091026 �߰� 

//#define CTL_READY			2		//� ������ ��û�� �� ������ �غ� �Ϸ��� ����  

//Find function return value
#define CTL_NOTFIND			-1001		// �� �ش��ϴ� ������ ����
#define CTL_NOSPACE			-1002		// ������ ������ 
#define CTL_EMPTY			-1004		// ����̽��� ��� ������ 
#define CTL_FULL			-1005		// ����̽��� FULL
#define CTL_TRAYSKIP		-1006		// Ʈ���̿� ������ ��� ���� Ʈ���̸� ��ŵ ������ ���� 

#define CTL_ERROR			-1			// �Լ� ���� ���� 
#define CTL_PROCEED			0			// �Լ��� �������̴� 
#define CTL_GOOD			1			// ������ �����Ǿ��� �� 
#define CTL_RETRY			2			// �ٽ� ó������ ������ �� �õ��Ѵ� 

#define CTL_LOTWAIT				0
#define CTL_LOTSTART			1
#define CTL_LOTEND_START		2
#define CTL_LOTEND_FINISH		5

#define CTL_LOCK			   -1 //��û�� ���� ������ �����߽�.
#define CTL_CLEAR				0 //�ƹ��͵� �������� �ʱ���� 
#define CTL_REQ					1 //�ش� ������ ��û�� ���� 
#define CTL_READY				2 //�ش� ������ ��û�� ���� ������ �Ϸ��Ͽ� �غ�� ���� - ������ ����
#define CTL_FREE				3 //��û�� ������ ������ ��� �Ϸ��� ���·� �������� ������� �������� ������ �� �ֵ��� �ڷ��Ӱ� �����͸� �����ش�.
#define CTL_CHANGE				4 //��ǥ���� ��ü�� ��û�Ҷ� ����Ѵ�(Remove Buffer ��ü�� ��û�Ҷ� ����Ѵ�)


#define CTL_DOWN				3
#define CTL_UP					4

#define CTL_COMPLETE			5
#define CTL_BUSY				7
#define CTL_POSSIBLE			8
#define CTL_SORT				9

//�������� ���� 
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

//	Console application ���α׷��� ���ؼ� �Ʒ� �κ��� ������
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
//�Ϲ� ���� 
//////////////////////////////
// ******************************************************************************
// ��� ���� ���� �� �ʿ��� �޽��� ����                                          
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

// ���̸� �߰� [2014.10.10]
#define REQ_HOMING_			0
#define REQ_MOVE_START_		1
#define REQ_MOVE_DONE_		2
#define REQ_JOG_MOVE_		3

#define DIR_MINUS_			0
#define DIR_PLUS_			1

enum
{
	// yangkyu [2015.03.25]
	// ���� �� ����
// 	M_LIFTER_Z3				= 0,
// 	M_LIFTER_Z1				= 1,
// 	M_LIFTER_Z4				= 2,
// 	M_LIFTER_Z2				= 3,
	//kwlee 2017.0712
	
	M_LIFTER_Z1				= 0,
	M_LIFTER_Z2				= 1,
	M_LIFTER_Z3				= 2,
 	M_LIFTER_Z4				= 3,

	M_LIFTER_Y				= 4,

	M_BCRREAD_ROBOT_Y		= 5,
	M_BCRREAD_ROBOT_Z		= 6,
	M_BCRREAD_ROBOT_X		= 7,
};


// �Ǽ��縮 ����Ʈ ���� ����
// Y��
#define Y_ACC_BOX_AVOID_		0
#define Y_ACC_FRONT_			1
#define Y_ACC_MIDDLE_			2
#define Y_ACC_REAR_				3



// Z��
#define Z_ACC_LIFT_SAFETY_		0
#define Z_ACC_LIFT_LIMIT_		1
#define Z_ACC_LIFT_OFFSET_		2
#define Z_ACC_LIFT_READY_		3

// XYZ �κ� ���� ����
//Y��
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

//X��
#define X_RBT_ACC_BCR_			0
#define X_RBT_ACC_BCR_OFFSET_		1

//Z��
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
	double d_spd_vel[3];			// ���� ���� ���� ���� ����
	double d_cmdpos_backup;
	double d_robot_safety;			// Safety Pos�� �� ������ Ŭ�� Y���� �����ϸ� AirBlow Box�� �浹 �Ҽ� �ִ�.

	int n_retry_cnt;
	long l_waittime[3];
	
	
	
	
	
	//���� ƼĪ���� define�Ͽ� ��ġ���� �����Ѵ� 
    double md_pos[MAX_POS];
	
	bool mb_HomeFlag;
	int mn_retry_cnt;
	long ml_retry_time_wait[3];
	int mn_retry_time_flag;
	
	int mn_homechk_flag;
	
    int mn_wait_time;			// ���� �̵��� ��� �ð� ���� ���� ���� 
	int mn_allow;				// ���� �̵��� ��� ���� ���� ���� ���� 
	double md_mm_for_pulse;		// 1mm �̵� �� ��� �޽� �� ���� ����
	
	double md_deviation[3]; //090803 �߰� Ŀ���� �ǵ�� ��ġ�� ���� ������ Ȯ������ �߰� 
	long   ml_waittime[3];					//time out�� ����Ʈ �簣�� üũ�Ѵ� 
	
	double md_initial_speed;
	double md_spd_acc;			// ���� ���� ���� ���� ����
    double md_spd_dec;			// ���� ���� ���� ���� ����
    double md_spd_vel;			// ���� ���� ���� ���� ����
    double md_spd_home;			// ���� Ȩüũ �ӵ� ���� ���� ����
    double md_spd_jog;			// ���� JOG �ӵ� ���� ���� ����
	
	double md_speed_acc[3];		// ���� ���� ���� ���� ����
    double md_speed_dec[3];		// ���� ���� ���� ���� ����
    double md_speed_vel[3];		// ���� ���� ���� ���� ����
	
    double mcommandposbackup;
	
	int mn_homecheck_method;
	int mn_home_step;
	
	double md_slow_spd_acc;				// ���� ���� ���� ���� ����
    double md_slow_spd_dec;				// ���� ���� ���� ���� ����
    double md_slow_spd_vel;				// ���� ���� ���� ���� ����
	
	double md_fast_spd_acc;				// ���� ���� ���� ���� ����
    double md_fast_spd_dec;				// ���� ���� ���� ���� ����
    double md_fast_spd_vel;				// ���� ���� ���� ���� ����
	
	double md_limit_position[2];		// 0:-, 1:+
	double md_offset_pos[2];			// 0:����, 1:����
	
	int mn_motor_type; //2010.1013 ���� ���� = 0, Step motor = 1, Ȩ üũ�� ���� ���� Ȩ üũ �÷���(st_handler.n_home_state[nAxis] = NO;) Ŭ���� ���� �Ǵ����� �߰� 
	
	//���ͷ�Ʈ ���� ���� ���� 
	int		mn_interrupt_flag;	//interrupt ��� ��/�� �÷��� 
	
	int		mn_interrupt_source;  //���ͷ�Ʈ ó�� ���� 
	//n_Mode : 	szCompareSource[4][50] = { 
	//0				"COMMAND Counter",
	//1				"Feedback Counter",
	//2			    "Deviation Counter",
	//3			    "General Counter"
	
	int		mn_interrupt_method; //����ϴ� ���� ����  
	//CHAR szCompareMethod[6][50] = {
	//0				"Disable",
	//1				"CMP == COUNTER (No direction)",
	//2				"CMP == COUNTER (Only when counting up)",
	//3				"CMP == COUNTER (Only when counting down)",
	//4				"CMP > COUNTER",
	//5				"CMP < COUNTER"
	
	double	mn_interrupt_position; //���ͷ�Ʈ �߻� ��ġ 
	
	int     mn_InterruptMask_Enable_Flag;  //���ͷ�Ʈ�� �� ��尡 CTL_YES�� �� ���¿����� �÷��׸� �����Ű�� 
	int		mn_InterruptMask_Disable_Flag; //���ͷ�Ʈ ����� �ѹ� ��������� �ʾҴٴ� �÷���(DC Fail ���縦 ����Ҷ� ���� ������ �߰�)
	
	long	ml_Set_InterruptMask[20]; //[50]=�ִ� ���� ��, [19]=0~18 ������ ���ͷ�Ʈ ����ũ ���� ���� ���� 
	long	ml_Get_InterruptMask[20]; //[50]=�ִ� ���� ��, [19]=0~18 ������ ���ͷ�Ʈ ����ũ ���� �о���δ�

	// ���̸� �߰� [2014.09.06]
	int nNeedsHoming;	// ���� Homing �۾� �ʿ� ����

	// baeksh [2014.10.16]
	int nStepMotor;	// Step Motor MotionDone 
} ;
extern  st_motor_param  st_motor[MOT_MAXMOTOR];


struct st_ctlbd_param 
{
	long l_LmIndexmotion_time[16][3]; //�̵��ð� üũ 
	long l_IxIndexmotion_time[8][3];  //�̵��ð� üũ 
	long l_Singlemotion_time[MOT_MAXMOTOR][3]; //���� ������ �̵���ġ üũ�ð� 

	int n_motorbd_init_end; //���� �ʱ�ȭ �Ϸ� 
	int n_iobd_init_end;    //i/o board �ʱ�ȭ �Ϸ� 

	int n_TotalMotBoard_Number;		//�� ����� ��ü ���ͺ��� ����
	int n_TotalMotorAxis_Number;	//��ü �������� ���� 
	int n_MotorAxis_Number[10];	//�ִ� 10������ ���� ��밡��  //������ ���ͺ��忡 �ִ� ���� ���� 
//	int MotorBoard_Axis_Number[11];//[0]:0������ �������, [1]:1������ ���� ���, .....  [10]:��ü ���ͺ����� ����  

	//2009.6.9 �߰� 
	int n_motmove_limit_time; //�⺻ 5��, n_motmove_limit_time = MOT_MOVE_LIMITTIME * ((100 - speedrate) + 1)

} ;
extern  st_ctlbd_param  st_ctlbd;
extern  st_ctlbd_param	st_reco_ctlbd;  

struct st_linearmotion_param
{	
	int		 n_indexmap_init;	   //���� index map�� �ʱ�ȭ�� �������� Ȯ���Ѵ� 
	long     l_AxisCnt;            //������ IndexNum���� ��� ������ ���� ���� �ִ� 4��  
	long	 lp_AxisNum[4];        //������ IndexNum���� ����ϴ� ���� ���� ��ȣ�� ������ 
	double	 dp_PosList[4];		   //�ִ� 4 ����Ʈ�� �ѹ��� �̵������ϴ� 
	double   dp_SpdRatio[3];        //�ӵ����� [0]:Vel, [1]:Acc, [2]:Dec
};
extern st_linearmotion_param st_linearmot[8];	 //[8] = LmIndexMap Number 0~ 7���� ����  

struct st_coordmotion_param
{
	int		 n_indexmap_init;		//���� index map�� ������ �ʱ�ȭ�� �������� Ȯ���Ѵ� 
	long	 l_TotalMoveCnt;		//�� ������ ���� ���� �������۵� �ѹ����� �Ѵ� (�ִ� 10ȸ) 
	long     l_TotalAxisCnt;        //������ IndexNum���� ��� ������ ���� ���� 
	long     lp_TotalAxisNum[4];    //������ IndexNum���� ����ϴ� ���͹�ȣ�� ������ 
	
	long    lp_IxIndexNum[8];	    //�ִ� 0 ~ 7��, ���� ���۽� ����� �� �ִ� ��ȣ 
	long    lp_SubAxisCnt[8];	    //���� index���� ������ ���� ����
	long    lp_SubAxisNum[8][4];    //[10] = index ���� 10ȸ //[0][0]:ù��° ���͹�ȣ, [0][1]:2��° ���͹�ȣ  
	double	dp_PosList[8][4];	    //[10] = 10ȸ���� , //[0]=������ ���� �̵� ��(����� �̰͸� ���), [1]=������ ���� �̵� �� (���� ���۽� ���)
	double	dp_AxisRatio[8][3];		    //�ִ� ������ �̵����� �ӵ� ���� ,[0]:vel, [1]:acc, [2]:dec ratio
};
extern st_coordmotion_param st_coordmot[16];	 //[16] = LmIndexMap Number 0~ 15���� ����  

#endif