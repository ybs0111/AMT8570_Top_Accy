/*========================================================================================
	FILE	  : Global.h
	DESCRIPT  : Definition file of the globals
	COPYRIGHT : 2001 ~ 2005, IT Innovation Co., Ltd.
========================================================================================*/
#ifndef __GlobalH__
#define __GlobalH__


/*========================================================================================
	ERROR CODES
========================================================================================*/
#ifndef WM_U_USER
	#define WM_U_USER			(WM_USER + 100)
	#define WM_U_S1F1			(WM_USER + 101)
	#define WM_U_DOCUMENT		(WM_USER + 102)
#endif

#ifndef WM_U_COMMAND
	#define WM_U_COMMAND			(WM_USER + 11)	//	User command
#endif	//	WM_U_COMMAND


#ifndef WM_U_UPDATE
	#define WM_U_UPDATE			(WM_USER + 51)	//	User command
#endif	//	WM_U_UPDATE


/*========================================================================================
	ERROR CODES
========================================================================================*/
//	Base error code
#ifndef RV_SUCCESS
	#define RV_SUCCESS			0				//	Success!
#endif	//	RV_SUCCESS
#ifndef ERS_NO_ERR
	#define ERS_NO_ERR			RV_SUCCESS		//	No error
#endif	//	ERS_NO_ERR
#ifndef ERS_
	#define ERS_				-20000			//	Base error
	#define ERS_BEGIN_THREAD	(ERS_ - 1)		//	Fail on begin thread
#endif	//	ERS_

//#define __EQUIP__
#define INIT_X					0				//-1280


/*========================================================================================
	MACROS
========================================================================================*/
//	Length or size of the variables
#ifndef LEN_COMMON
	#define LEN_COMMON			64		//	Common string length
	#define LEN_COMMON2			(LEN_COMMON * 2)	//	Common string length * 2
	#define LEN_GENERAL			(LEN_COMMON * 4)	//	General string length
	#define LEN_GENERAL2		(LEN_GENERAL * 2)	//	General string length * 2
	#define LEN_GENERAL4		(LEN_GENERAL * 4)	//	General string length * 4
	#define LEN_GENERAL8		(LEN_GENERAL * 8)	//	General string length * 8
	#define LEN_MESSAGE			1024
#endif		//	LEN_COMMON


/*========================================================================================
	WPARAM Information.
========================================================================================*/
#define UM_XGEM_STATE				1
#define UM_COMM_STATE				2
#define UM_CONTROL_STATE			3
#define UM_RECV_MESSAGE				4
#define UM_SEND_MESSAGE				5
#define UM_SCENARIO_MSG				6
#define UM_XGEM_START				7


/*========================================================================================
	View ID Information.
========================================================================================*/
#define VIEWID_EQUIPS				1
#define VIEWID_CONSTS				2
#define VIEWID_LOGINS				3
#define VIEWID_MODULE				4
#define VIEWID_EQSIMS				5
#define VIEWID_GEM					6
#define VIEWID_CMS					7
#define VIEWID_PJM					8
#define VIEWID_CJM					9
#define VIEWID_STS					10

/*========================================================================================
	Form View Handle ID Information.
========================================================================================*/
#define MAX_VIEW_COUNT			20
#define VIEW_TITLES				0
#define VIEW_LOGINS				1
#define VIEW_LOGO				2
#define VIEW_VIEWIO				3
#define VIEW_BUTTON				4
#define VIEW_EQSIMS				5
#define VIEW_GEM				6
#define VIEW_CMS				7
#define VIEW_PJM				8
#define VIEW_CJM				9
#define VIEW_STS				10


//========================================================================================
//	ENUMERATIONS
//========================================================================================
// XGem state
enum _eXGemState{
	XGemState_Unknown = -1,
	XGemState_Init = 0,
	XGemState_Idle,
	XGemState_Setup,
	XGemState_Ready,
	XGemState_Execute
};

//	Communication state
enum _eCommState 
{
	Comm_None = -1, 
	Comm_Disabled = 1, 
	Comm_WaitCRFromHost, 
	Comm_WaitDelay, 
	Comm_WaitCRA,
	Comm_Communicating,
	Comm_Enabled
};

//	Control state
enum _eControlState 
{
	Control_None = -1, 
	Control_EqOffline = 1, 
	Control_AttemptOnline, 
	Control_HostOffline,
	Control_Local, 
	Control_Remote
};


//   Processing state for sample(equipment dependent)
enum _eProcessingState
{
	Processing_None = -1,
	Processing_SystemPowerUp = 1,
	Processing_Empty,
	Processing_Idle,
	Processing_Standby,
	Processing_Processing,
	Processing_HoldProcessing
};

//========================================================================================
//	COLOR INFORMATION DEFINITION
//========================================================================================
#define WNDBACK         0x00C8D0D4          // 화면 기본 컬러
#define LABELBACK       0x00400000          // 진청색
#define CLOUDBLUE		RGB(128, 184, 223)
#define WHITE			RGB(255, 255, 255)
//#define BLACK			RGB(1, 1, 1)
#define DKGRAY			RGB(128, 128, 128)
#define LTGRAY			RGB(192, 192, 192)
#define YELLOW			RGB(255, 255, 0)
#define DKYELLOW		RGB(128, 128, 0)
#define RED			    RGB(255, 0, 0)
#define DKRED			RGB(128, 0, 0)
#define BLUE			RGB(0, 0, 255)
#define DKBLUE			RGB(0, 0, 128)
#define CYAN			RGB(0, 255, 255)
#define DKCYAN			RGB(0, 128, 128)
#define GREEN			RGB(0, 255, 0)
#define DKGREEN		    RGB(0, 128, 0)
#define MAGENTA		    RGB(255, 0, 255)
#define DKMAGENTA		RGB(128, 0, 128)

#define BACK_COLOR		RGB(100,135,188)
#define LABEL_COLOR		RGB(215,222,248)


short Split(const char *pszValue, TCHAR ch, CStringArray &asValue);

#endif		//	GlobalH