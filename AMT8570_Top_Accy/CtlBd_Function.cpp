// CtlBd_Function.cpp: implementation of the CCtlBd_Function class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "handler.h"
#include "CtlBd_Function.h"

#include "Cmmsdk.h"
#include "CmmsdkDef.h"
#include "ComizoaPublic.h"

#include "FastechPublic_IO.h"

#include "FAS_HSSI.h"
#include "math.h"

#include "Public_Function.h"
#include "Variable.h"
#include "MyJamData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CCtlBd_Function  CtlBdFunc;
CCtlBd_Function::CCtlBd_Function()
{

}

CCtlBd_Function::~CCtlBd_Function()
{
//st_ctlbd.n_TotalMotBoard_Number
}

int CCtlBd_Function::Board_Initialize(int n_mode)
{
	int nRet = 0;

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//Motor Board Initialize 
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	if(CTLBD_MOTBD_TYPE == 0) //0:파스텍 보드를 사용하는 장비이면, 1:커미조아 모터를 사용하는 장비 
	{
		st_ctlbd.n_TotalMotBoard_Number = 3;  //모터보드가 3개 있다 
		st_ctlbd.n_TotalMotorAxis_Number= 18; //모터의 총 수량은 18개이다  
		st_ctlbd.n_MotorAxis_Number[0] = 8;   //0번 모터보드는 8축용 보드이다  
		st_ctlbd.n_MotorAxis_Number[1] = 6;   //1번 모터보드는 6축용 보드이다 
		st_ctlbd.n_MotorAxis_Number[2] = 4;   //2번 모터보드는 4축용 보드이다 
/* 2011.0113 
		nRet = FAS_MOT.Initialize_MotBoard();
		if(nRet = CTLBD_RET_ERROR)
		{
			return CTLBD_RET_ERROR;
		}
*/
	}
	else if(CTLBD_MOTBD_TYPE == 1) //1:커미조아 보드를 사용하는 장비이면, 1:커미조아 모터를 사용하는 장비 
	{


	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//I/O Board Initialize 
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	if(CTLBD_IOBD_TYPE == 0) //0: 파스텍 I/O 보드를 처리할때 사용 
	{
		nRet = Initialize_FASIO_Board();
		if(nRet = CTLBD_RET_ERROR)
		{
			return CTLBD_RET_ERROR;
		}
	}

	return  CTLBD_RET_GOOD;
}

int CCtlBd_Function::Robot_Clash_Safety_Check(int n_Mode, int n_SourceAxis, int n_ReferenceAxis, double d_Safety_Gap, double d_TargetPos)
{//d_Safety_Gap = 300; //300mm 이상 떨어져 있어야 한다 //DC Loading 사이트 

	//n_Mode => 0: 모터 이동전 순수하게 체크만 하는 상태, 1: 모터가 이동중일때 체크하는 상태 
	int nRet = CTL_ERROR;
	char cJamcode[10] = {NULL};
	double dCurrentPos[2] = {0,0}, dCalPos[3] = {0,0,0}, dSafepos_gap[3] = {0,0,0}, dCurrMotPos = 0; 
	int nSafetyPos_Flag = CTL_NO; 

	return nRet;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inposition 생략 및 거리별 속도를 관리한다 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int CCtlBd_Function::MotorSpeed_Control(int n_Axis, double d_CurrentPos, double d_TargetPos, double * dp_Speed)
{
	double dCurrentPos = 0, dTargetPos = 0, dVal =0, d_abs_val = 0, dAccDecTime=0; 
	int nRetStatus=0;

	return CTL_GOOD;
}


int CCtlBd_Function::Alarm_Error_Occurrence(int n_Count, int n_Type, int n_Status, CString s_JamCode)
{
      alarm.mn_count_mode = n_Count; alarm.mn_type_mode = n_Type; st_work.nEqpStatus = n_Status; 
	  alarm.mstr_code = s_JamCode; 
	  sprintf(alarm.mc_code, s_JamCode);

	  MyJamData.OnSet_Info2StrctAlarm(alarm.mstr_code);	// 발생한 알람 정보를 구조체에 할당
	  
	  if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
	  {
		  if (MOT_DEBUG)
		  {
			  sprintf(st_msg.c_abnormal_msg, "[ALARM]=> [%s] [%s] %d", alarm.mstr_code, st_alarm.mstr_cur_msg, n_Count);	// 20130916 kjh
			  st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
		  }
		  else
		  {
//			  st_msg.mstr_abnormal_msg.Format("[%s] [%s]", alarm.mstr_code, st_alarm.mstr_cur_msg);
			  sprintf(st_msg.c_abnormal_msg, "[%s] [%s] %d", alarm.mstr_code, st_alarm.mstr_cur_msg, n_Count);	// 20130916 kjh
			  st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
		  }
	  }
	  
	return CTL_GOOD;
}

//2011.0123 일부 내용 변경 
//이곳은 알람 메세지 및 일부 내용을 파일로 저장하는 역활을 담당한다 
int CCtlBd_Function::Send_Error_Message(int n_Mode, int n_Axis, CString s_JamCode, CString s_JamMsg)
{
	long lErrorCode;
	short int nErrorParseAxis;
	short int nErrorParseReason;
	char cErrorMsg[200];
	CString sMsg, sTemp;
	
	memset(&cErrorMsg, 0, sizeof(cErrorMsg));  // 데이터 저장 변수 초기화 

	//2011.0123 아래내용 추가 
	if(n_Mode == MOT_ERR_MSG) 
	{//문자 메세지를 받아 리스트 화면에 디스플레이 한다   
		if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
		{
//			st_msg.mstr_abnormal_msg = s_JamMsg;
			sprintf(st_msg.c_abnormal_msg,s_JamMsg);
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
		}		
	}
	else if(n_Mode == MOT_ERR_CODE)
	{//jamcode  설정 		
		if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
		{
//			st_msg.mstr_abnormal_msg = s_JamMsg;
			sprintf(st_msg.c_abnormal_msg, s_JamMsg);
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
		}	
	}
	else
	{//jamcode  설정 		
		if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
		{
//			st_msg.mstr_normal_msg = s_JamMsg;
			sprintf(st_msg.c_normal_msg, s_JamMsg);
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
		}	
	}//2011.0123 추가
	
	if(CTLBD_DEBUG_MODE) 
	{
		if(CTLBD_MOTBD_TYPE == 0) //파스텍 보드 
		{

		}
		else if(CTLBD_MOTBD_TYPE == 1) //커미조아 보드 
		{
			cmmErrGetLastCode(&lErrorCode);						//마지막 발생한 에러 코드 확인  
			nErrorParseAxis = cmmErrParseAxis(lErrorCode);     //에러를 발생시킨 축 정보를 가져온다 
			nErrorParseReason = cmmErrParseReason(lErrorCode); //error code를 받는다 
			cmmErrGetString(lErrorCode, cErrorMsg, 200);

			sTemp.Format("Msg=%s", cErrorMsg);
			sMsg.Format("[MSG]MotNum:%d, ECode=%d, EAxis=%d, EReason=%d, Msg[%s]", n_Axis, lErrorCode, nErrorParseAxis, nErrorParseReason, sTemp);
			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
			{
//				st_msg.mstr_abnormal_msg = sMsg;
				sprintf(st_msg.c_abnormal_msg, sMsg);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
			}	
		}
	} 
	//2011.0123 삭제 alarm.mstr_code = s_JamCode; //alarm.mn_count_mode = 0; alarm.mn_type_mode = CTL_dWARNING;
	
	return CTLBD_YES; 
}

int CCtlBd_Function::Initialize_FASIO_Board(void) 
{
	int m=0, p=0, s=0, i;
	int nRet=0;
	WORD wMaskMap = 0;

	INT nHSSI_speed=0;
	INT nMasterNo=0, nPortNo =0, nSlaveNo=0; 

	nRet = FAS_IO.Set_IO_BoardOpen(&nMasterNo, START_NOTHING);  // I/O 보드 오픈 함수 
	if (nRet == CTLBD_RET_ERROR)
	{
		return CTLBD_RET_ERROR; //IO_BOARD_OPEN_FAILED;
	}

	nRet = FAS_IO.Search_IO_Master(&nMasterNo);              // PC에 연결된 마스터 보드 갯수 검사 함수 
	if (nRet == CTLBD_RET_ERROR)
	{
		return CTLBD_RET_ERROR;
	}
	
	for(m=0; m<nMasterNo; m++)
	{
		nRet = FAS_IO.Search_IO_Port(m, &nPortNo);						// 마스터 보드에 연결된 PORT 갯수 검사 함수 
		if (nRet == CTLBD_RET_ERROR) return CTLBD_RET_ERROR;	//PORT_SEARCH_FAILED;
		
		for(p=0; p<nPortNo; p++)
		{
			nRet = FAS_IO.Set_IO_HSSISpeed(m, p, PORT_SPEED_10);		// PORT와 SLAVE I/O 모듈과의 통신 속도 설정 함수 
			if (nRet == CTLBD_RET_ERROR) return CTLBD_RET_ERROR; //IO_SPEED_SET_FAILED;
			 
			nRet = FAS_IO.Search_IO_Slave(m, p, &nSlaveNo);			// PORT에 연결된 SLAVE 검사 함수 
			if (nRet == CTLBD_RET_ERROR) return CTLBD_RET_ERROR; //IO_SLAVE_SEARCH_FAILED;

			for(s=0; s<nSlaveNo; s++)
			{
				nRet = FAS_IO.Set_IO_SlaveEnable(m, p, s, CTLBD_YES);		// SLAVE I/O 모듈 동작 ENABLE/DISABLE 설정 함수 
				if (nRet == CTLBD_RET_ERROR)	return CTLBD_RET_ERROR; //IO_SLAVE_ENABLE_FAILED;

				wMaskMap = 0; //clear
				for(i=0; i<16; i++) //0~15 까지의 Port 16bit 셋팅, 각각의 I/O 상태를 정의한다 
				{			
					wMaskMap = wMaskMap | (WORD)pow(2, mw_IOMaskMap[m][p][s][i]);					 
				}
				FAS_IO.Set_IO_DefineWord(m, p, s, wMaskMap); // x Master, x포트, x번 슬레이브 , 16bit 셋팅
				//FAS_IO.Set_IO_DefineWord(0, 0, 0, 0x0003); // 0번 Master, 0번 포트, 0번 슬레이브 , 16bit 셋팅
				//FAS_IO.Set_IO_DefineWord(0, 0, 1, 0x0003); // 0번 Master, 0번 포트, 1번 슬레이브 , 16bit 셋팅
				//FAS_IO.Set_IO_DefineWord(0, 1, 0, 0x00ff); // 0번 Master, 1번 포트, 0번 슬레이브 , 16bit 셋팅
			}
		}
	}
	
	return CTLBD_RET_GOOD;
}



int CCtlBd_Function::Initialize_InterruptMask_TriggerConfig(void) 
{
	int i, nAxis;
	
	for(i=0; i<5; i++)	//for(nAxis=0; nAxis<MOT_MAXMOTOR; nAxis++)
	{
		if(i == 0)
		{
			nAxis = 0; 
			st_motor[nAxis].mn_interrupt_flag = CTL_YES; //dc load robot Y 무조건 설정 
		}
		else if(i == 1)
		{
			nAxis = 5;
			st_motor[nAxis].mn_interrupt_flag = CTL_YES; //dc unload robot Y 무조건 설정
		}
		else if(i == 2) 
		{
			nAxis = 16;	
			st_motor[nAxis].mn_interrupt_flag = CTL_YES; //insert robot Y 무조건 설정
		}
		else if(i == 3)
		{
			nAxis = 19;		//2009.5.27 james 소켓 푸쉬 추가 
			//st_motor[nAxis].mn_interrupt_flag = CTL_YES; //소켓 푸쉬 무조건 설정
		}
		else if(i == 4)
		{
			nAxis = 20;	
			st_motor[nAxis].mn_interrupt_flag = CTL_YES; //remove robot Y 무조건 설정
		}

		if(st_motor[nAxis].mn_interrupt_flag == CTL_YES) //인터럽트를 사용하는 모터이면 
		{
			st_motor[nAxis].ml_Set_InterruptMask[0] = CTL_NO; //"Normal Stop", // 0
			st_motor[nAxis].ml_Set_InterruptMask[1] = CTL_NO; //"Succesive start", // 1
			st_motor[nAxis].ml_Set_InterruptMask[2] = CTL_NO; //"", // 2
			st_motor[nAxis].ml_Set_InterruptMask[3] = CTL_NO; //"", // 3
			st_motor[nAxis].ml_Set_InterruptMask[4] = CTL_NO; //"Start of acceleration", //4
			st_motor[nAxis].ml_Set_InterruptMask[5] = CTL_NO; //End of acceleration", // 5
			st_motor[nAxis].ml_Set_InterruptMask[6] = CTL_NO; //"Start of deceleration", // 6
			st_motor[nAxis].ml_Set_InterruptMask[7] = CTL_NO; //"End of deceleration", // 7
			st_motor[nAxis].ml_Set_InterruptMask[8] = CTL_NO; //"", // 8
			st_motor[nAxis].ml_Set_InterruptMask[9] = CTL_NO; //"", // 9
			st_motor[nAxis].ml_Set_InterruptMask[10] = CTL_NO; //"Position error tolerance exceed", // 10
			st_motor[nAxis].ml_Set_InterruptMask[11] = CTL_NO; //"General Comparator", // 11

			//이것만 우선 사용한다 
			st_motor[nAxis].ml_Set_InterruptMask[12] = CTL_YES; //"Compared trigger", // 12

			st_motor[nAxis].ml_Set_InterruptMask[13] = CTL_NO; //"CLR signal resets the counter value", // 13
			st_motor[nAxis].ml_Set_InterruptMask[14] = CTL_NO; //"LTC input making counter value latched", // 14
			st_motor[nAxis].ml_Set_InterruptMask[15] = CTL_NO; //"ORG input signal ON", // 15
			st_motor[nAxis].ml_Set_InterruptMask[16] = CTL_NO; //"SD input signal ON", // 16
			st_motor[nAxis].ml_Set_InterruptMask[17] = CTL_NO; //"+DR input changes", // 17
			st_motor[nAxis].ml_Set_InterruptMask[18] = CTL_NO; //"-DR input changes" // 18

			//인터럽트 마스키 정보를 셋팅한다 
			//Set_InterruptMask(int n_Axis, long l_Set_InterruptMask[19])
			COMI.Set_InterruptMask(nAxis, st_motor[nAxis].ml_Set_InterruptMask);

			//인터럽트 발생 조건 및 구간 설정 
			//Set_TriggerSet_OneCompareConfig(int n_Axis, int n_Source, int n_Method, double d_ComparePosition)
			COMI.Set_TriggerSet_OneCompareConfig(nAxis, st_motor[nAxis].mn_interrupt_source, st_motor[nAxis].mn_interrupt_method, st_motor[nAxis].mn_interrupt_position);
		}
	}		
	return CTL_GOOD;
}
