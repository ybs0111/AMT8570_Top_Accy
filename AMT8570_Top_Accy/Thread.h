#ifndef _AFX_THREAD_H__
#define _AFX_THREAD_H__

// ******************************************************************************
// ������ �� ������ �ڵ� ���� ���� ����                                          
// ******************************************************************************
extern CWinThread  *m_thread[25] ;
extern HANDLE		hThrHandle[25];
// ******************************************************************************

// ******************************************************************************
// ���� ������ �Լ� �����Ѵ�                                                     
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
	
// ���̸� �߰� [2013.11.15]
extern UINT OnThread_Net(LPVOID lParam);					
extern UINT OnThread_Serial(LPVOID lParam);

extern UINT OnThread_LoadingConv(LPVOID lParam);
extern UINT OnThread_BoxClamp(LPVOID lParam);
extern UINT OnThread_BoxLifter(LPVOID lParam);
extern UINT OnThread_XYZRobot(LPVOID lParam);
extern UINT OnThread_BufferAlignConv(LPVOID lParam);
// ******************************************************************************

extern int extStopAll ;  // ������ ���� �÷���

#endif