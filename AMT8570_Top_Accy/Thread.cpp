// Thread.cpp: implementation of the Thread class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "iostream.h"
#include "afxmt.h"
#include "Thread.h"

#include "Variable.h"  // 전역 변수 정의 클래스

#include "MyJamData.h"  // 알람 출력 정보 로딩 및 알람 메시지 저장 클래스

// 규이리 추가 [2013.11.13]
#include "Seq_IO_ReadWrite.h"
#include "Seq_MotMove.h"
#include "Seq_TowerLamp.h"
#include "Seq_LampSwitch.h"

#include "MainEvent.h"
#include "Seq_LoadingConv.h"
#include "Seq_BoxClamp.h"
#include "Seq_BoxLifter.h"
#include "Seq_BufferAlignConv.h"
#include "Seq_XYZRobot.h"

#include "BcrEvent.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// ******************************************************************************
// 알람 출력 클래스 변수 선언                                                    
// ******************************************************************************
CMyJamData   mcls_alarm_disp;  // 발생 알람 출력 정보 로딩 클래스 변수
// ******************************************************************************

// ******************************************************************************
// 쓰레드 및 핸들 저장 변수 선언                                                 
// ******************************************************************************
CWinThread  *m_thread[25];
HANDLE		hThrHandle[25];
// ******************************************************************************

// ******************************************************************************
// 전역 오퍼레이션 쓰레드 함수 선언한다                                          
// ******************************************************************************
UINT OnThread_ReadIO(LPVOID lParam);
UINT OnThread_MotMove(LPVOID lParam);
UINT OnThread_TowerLamp(LPVOID lParam);
UINT OnThread_LampSwitch(LPVOID lParam);
UINT OnThread_RunCondition(LPVOID lParam);//
UINT OnThread_AlarmDisplay(LPVOID lParam);
UINT OnThread_NetWork(LPVOID lParam);
UINT  OnThread_Handler_Check(LPVOID lParam);		
UINT  OnThread_Tower_Lamp_Check(LPVOID lParam);		
UINT  OnThread_Alarm_Display(LPVOID lParam);		
// 규이리 추가 [2013.11.15]
UINT OnThread_Net(LPVOID lParam);
UINT OnThread_Serial(LPVOID lParam);

UINT OnThread_LoadingConv(LPVOID lParam);
UINT OnThread_BoxClamp(LPVOID lParam);
UINT OnThread_BoxLifter(LPVOID lParam);
UINT OnThread_XYZRobot(LPVOID lParam);
UINT OnThread_BufferAlignConv(LPVOID lParam);

// ******************************************************************************


CMutex mutex ;

int extStopAll ;			// 쓰레드 상태 플래그
int threadtimer[3];
int countflag = 0;

//==================================================================//
// I/O 입출력 상태 읽기 쓰레드
//==================================================================//
UINT OnThread_ReadIO(LPVOID lParam)
{
	CSingleLock sing(&mutex);

	while(!extStopAll)
	{
		sing.Lock();

		seqIO_ReadWrite.OnSeq_RUN();
		
		Sleep(0);
		sing.Unlock();
	}
	return TRUE;
}

//==================================================================//
// 매뉴얼 모터 구동 쓰레드
//==================================================================//
UINT OnThread_MotMove(LPVOID lParam)
{
	CSingleLock sing(&mutex);

	while(!extStopAll)
	{
		sing.Lock();

		seqMotMove.OnSeq_RUN();

		Sleep(1);
		sing.Unlock();
		//Sleep(1);
	}
	return TRUE;
}

//==================================================================//
// 타워 램프 상태 변경 쓰레드
//==================================================================//
UINT OnThread_TowerLamp(LPVOID lParam)
{
	CSingleLock sing(&mutex);
	
	while(!extStopAll)
	{
		sing.Lock();
		
		seqTowerLamp.OnSeq_RUN();
		
		Sleep(0);
		sing.Unlock();
	}
	return TRUE;
}

//==================================================================//
// 스위치 눌림 상태 체크 쓰레드
//==================================================================//
UINT OnThread_LampSwitch(LPVOID lParam)
{
	CSingleLock sing(&mutex);
	
	while(!extStopAll)
	{
		sing.Lock();
		
		seqSwitch.OnSeq_RUN();
		
		Sleep(0);
		sing.Unlock();
	}
	return TRUE;
}

//==================================================================//
// 환경 안전 INTERLOCK 모니터링 쓰레드
//==================================================================//
UINT OnThread_RunCondition(LPVOID lParam)
{
	CSingleLock sing(&mutex);
	
	while(!extStopAll)
	{
		sing.Lock();
		
		Func.OnIsAllForceDischargeComplete();
		Func.OnMon_Door();	// 도어의 [열림/닫힘] 상태 모니터링
		// 규이리 추가 [2015.01.03]
		Func.OnMon_EMO();	// EMO 스위치 눌림 상태 모니터링
		Func.OnMon_MC();	// MC 전원 떨어진 상태 모니터링
		
		Sleep(0);
		sing.Unlock();
	}
	
	return TRUE;
}

//==================================================================//
// 알람 출력 쓰레드
//==================================================================//
UINT OnThread_AlarmDisplay(LPVOID lParam)
{
	CSingleLock sing(&mutex);
	
	while(!extStopAll)
	{
		sing.Lock();

		mcls_alarm_disp.On_Alarm_Display(); // 알람 발생한 경우 알람 화면 출력 함수
		
		Sleep(0);
		sing.Unlock();
	}
	return TRUE;
}

//==================================================================//
// [MAIN<->ACCY] 통신 담당 쓰레드
//==================================================================//
UINT OnThread_NetWork(LPVOID lParam)
{
	CSingleLock sing(&mutex);
	
	while(!extStopAll)
	{
		sing.Lock();
		
		g_client_accy.OnSeq_RUN();
		
		Sleep(0);
		//Sleep(10);
		
		sing.Unlock();
	}
	// 규이리 주석 처리 [2015.03.25]
	// 쓰레드 종료 함수에서 일괄적으로 처리됨
	//hThrHandle[21] = NULL;
	return TRUE;
}

//==================================================================//
// 시리얼 통신 담당 쓰레드
//==================================================================//
UINT OnThread_Serial(LPVOID lParam)
{
	CSingleLock sing(&mutex);
	
	while(!extStopAll)
	{
		sing.Lock();
		
		evBCR.OnSeq_RUN();
		
		Sleep(0);
		//Sleep(10);
		
		sing.Unlock();
	}
	return TRUE;
}

//==================================================================//
// LoadingConv 담당 쓰레드
//==================================================================//
UINT OnThread_LoadingConv(LPVOID lParam)
{
	CSingleLock sing(&mutex);
	
	while(!extStopAll)
	{
		sing.Lock();
		
		
		SeqLoadingConv.OnSeq_RUN();
		Sleep(0);
		
		sing.Unlock();
	}
	return TRUE;
}

//==================================================================//
// BoxClamp 담당 쓰레드
//==================================================================//
UINT OnThread_BoxClamp(LPVOID lParam)
{
	CSingleLock sing(&mutex);
	
	while(!extStopAll)
	{
		sing.Lock();
		
		
		SeqBoxClamp.OnSeq_RUN();
		Sleep(0);
		
		sing.Unlock();
	}
	return TRUE;
}

//==================================================================//
// BoxLifter 담당 쓰레드
//==================================================================//
UINT OnThread_BoxLifter(LPVOID lParam)
{
	CSingleLock sing(&mutex);
	
	while(!extStopAll)
	{
		sing.Lock();
		
		
		SeqBoxLifter.OnSeq_RUN();
		Sleep(0);
		
		sing.Unlock();
	}
	return TRUE;
}

//==================================================================//
// XYZRobot 담당 쓰레드
//==================================================================//
UINT OnThread_XYZRobot(LPVOID lParam)
{
	CSingleLock sing(&mutex);
	
	while(!extStopAll)
	{
		sing.Lock();
		
		
		SeqXYZRobot.OnSeq_RUN();
		Sleep(0);
		
		sing.Unlock();
	}
	return TRUE;
}

//==================================================================//
// BufferAlignConv 담당 쓰레드
//==================================================================//
UINT OnThread_BufferAlignConv(LPVOID lParam)
{
	CSingleLock sing(&mutex);
	
	while(!extStopAll)
	{
		sing.Lock();
		
		
		SeqBufferAlignConv.OnSeq_RUN();
		Sleep(0);
		
		sing.Unlock();
	}
	return TRUE;
}