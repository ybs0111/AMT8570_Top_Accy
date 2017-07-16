#ifndef _AFX_THREAD_H__
#define _AFX_THREAD_H__

// ******************************************************************************
// 쓰레드 및 쓰레드 핸들 저장 변수 선언                                          
// ******************************************************************************
extern CWinThread  *m_thread[25] ;
extern HANDLE		hThrHandle[25];
// ******************************************************************************

// ******************************************************************************
// 전역 쓰레드 함수 선언한다                                                     
// ******************************************************************************
extern UINT OnThread_ReadIO(LPVOID lParam);
extern UINT OnThread_MotMove(LPVOID lParam);
extern UINT OnThread_TowerLamp(LPVOID lParam);
extern UINT OnThread_LampSwitch(LPVOID lParam);
extern UINT OnThread_RunCondition(LPVOID lParam);
extern UINT OnThread_AlarmDisplay(LPVOID lParam);
extern UINT OnThread_NetWork(LPVOID lParam);
extern UINT OnThread_Handler_Check(LPVOID lParam);			
extern UINT OnThread_Tower_Lamp_Check(LPVOID lParam);		
extern UINT OnThread_Alarm_Display(LPVOID lParam);
	
// 규이리 추가 [2013.11.15]
extern UINT OnThread_Net(LPVOID lParam);					
extern UINT OnThread_Serial(LPVOID lParam);

extern UINT OnThread_LoadingConv(LPVOID lParam);
extern UINT OnThread_BoxClamp(LPVOID lParam);
extern UINT OnThread_BoxLifter(LPVOID lParam);
extern UINT OnThread_XYZRobot(LPVOID lParam);
extern UINT OnThread_BufferAlignConv(LPVOID lParam);
// ******************************************************************************

extern int extStopAll ;  // 쓰레드 상태 플래그

#endif