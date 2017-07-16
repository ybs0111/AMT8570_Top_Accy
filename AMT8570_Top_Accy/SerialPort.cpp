//////////////////////////////////////////////////////////////////////
//==================================================================//
// FILE NAME : SerialPort.cpp
//
// Copyright (C) 2008 DEnT Corporation
// All rights reserved.
//
// CREATED BY  : Remon Spekreijse
// REVISION    :
//     1> 1997.11.12 : Created
//     2> 2008.10.15 : Modified (by Kilee)
// DESCRIPTION :
//     This class can read, write and watch one serial port.
//	   It sends messages to its owner when something happends on the port.
//	   The class creates a thread for reading and writing so the main
//	   program is not blocked.
//==================================================================//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SerialPort.h"

#include <assert.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSerialPort::CSerialPort()
{
	m_thrSerial = NULL;  // 규이리 추가 [2010.02.25]
	m_hComm = NULL;  // 통신 포트 핸들 초기화

	// initialize overlapped structure members to zero
	m_ov.Offset = 0;
	m_ov.OffsetHigh = 0;

	// create events
	m_ov.hEvent = NULL;
	m_hWriteEvent = NULL;
	m_hShutdownEvent = NULL;

	m_cpWriteBuff = NULL;

	m_bThreadAlive = FALSE;
	//TRACE("Flag [false]\n");
}

CSerialPort::~CSerialPort()
{
	// 규이리 추가 [2008.10.17]
	ClosePort();  // 통신 포트 Close
}

//==================================================================//
// Port 초기화
// pzPortOwner : the owner (CWnd) of the port (receives message)
// nzPortNr    : portnumber (1..4)
// nzBaud      : baudrate
// czParity    : parity bit
// nzDatabits  : data bit
// dwzCommEvents   : EV_RXCHAR, EV_CTS etc
// nzWriteBuffSize : size to the writebuffer
BOOL CSerialPort::InitPort(CWnd* pzPortOwner,
						   UINT nzPortNr, UINT nzBaud, char czParity, UINT nzDatabits, UINT nzStopbits,
						   DWORD dwzCommEvents, UINT nzWriteBuffSize)
{
	assert(nzPortNr > 0 && nzPortNr < dPORT_MAX);
	assert(pzPortOwner != NULL);

	//==============================================================//
	// if the thread is alive: Kill
	if (m_bThreadAlive)
	{
		do	{
			SetEvent(m_hShutdownEvent);
		} while(m_bThreadAlive);
		TRACE("Thread Ended\n");
	}
	//--------------------------------------------------------------//

	//==============================================================//
	// create events [이벤트 생성 실패는 처리하지 않음]
	if (m_ov.hEvent != NULL)	{
		ResetEvent(m_ov.hEvent);
	}
	m_ov.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	if (m_hWriteEvent != NULL)	{
		ResetEvent(m_hWriteEvent);
	}
	m_hWriteEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	if (m_hShutdownEvent != NULL)	{
		ResetEvent(m_hShutdownEvent);
	}
	m_hShutdownEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	//--------------------------------------------------------------//

	// initialize the event objects
	m_hEventArray[0] = m_hShutdownEvent;
	m_hEventArray[1] = m_ov.hEvent;
	m_hEventArray[2] = m_hWriteEvent;

	// initialize critical section
	InitializeCriticalSection(&m_csCommSync);

	// set buffersize for writing and save the owner
	m_pOwner = pzPortOwner;

	if (m_cpWriteBuff != NULL)	{
		delete[] m_cpWriteBuff;
	}
	m_cpWriteBuff = new char[nzWriteBuffSize];  // 초기 살정한 버퍼 크기만큼 할당

	m_nPortNr = nzPortNr;

	m_dwWriteBuffSize = nzWriteBuffSize;
	m_dwCommEvents = dwzCommEvents;

	BOOL bResult = FALSE;
	char *szPort = new char[50];
	char *szBaud = new char[50];

	//==============================================================//
	// CriticalSection 선언
	// 중복 접근을 방지하기 위하여 설정
	EnterCriticalSection(&m_csCommSync);

	// if the port is already opened: close it
	if (m_hComm != NULL)	{
		CloseHandle(m_hComm);
		m_hComm = NULL;
	}

	// prepare port strings
	if (nzPortNr > 9)	{
		sprintf(szPort, "\\\\.\\COM%d", nzPortNr);
	}
	else	{
		sprintf(szPort, "COM%d", nzPortNr);
	}
	sprintf(szBaud, "baud=%d parity=%c data=%d stop=%d", nzBaud, czParity, nzDatabits, nzStopbits);

	// get a handle to the port
	m_hComm = CreateFile(szPort,     // 디바이스 파일 이름
		GENERIC_READ|GENERIC_WRITE,  // 파일 엑세스 모드 [디바이스에서 데이터를 읽고,쓰고 파일포인터를 움직일수 있다]
		0,                           // 공유모드 [디바이스 파일은 공유될 수 없으므로 0]
		NULL,                        // 보안정보 [디바이스 파일에서는 사용하지 않으므로 NULL]
		OPEN_EXISTING,               // 생성방법 [OPEN_EXISTING 이미 존재하는 파일을 연다. 존재하고 있지 않다면 생성할 수 있는 것이 아니므로]
		FILE_FLAG_OVERLAPPED,        // 파일 플래그와 속성
		0);                          // 95/98은 템플릿 파일을 지원하지 않으므로 반드시 NULL이어야 한다
	
	if (m_hComm == INVALID_HANDLE_VALUE)
	{
		// port not found
		delete[] szPort;
		delete[] szBaud;

		return FALSE;
	}

	// set the timeout values
	// xFFFFFF = MAXDWORD 의미로 문자 사이의 전달 지연시간을 설정하는 것인데, 즉시로 설정한 것
	//m_CommTimeouts.ReadIntervalTimeout = 0xFFFFFFFF;
	//m_CommTimeouts.ReadTotalTimeoutMultiplier = 0;
	//m_CommTimeouts.WriteTotalTimeoutMultiplier = 2*CBR_9600 / nzBaud;
	m_CommTimeouts.ReadIntervalTimeout = 1000;
	m_CommTimeouts.ReadTotalTimeoutMultiplier = 1000;
	m_CommTimeouts.ReadTotalTimeoutConstant = 1000;
	m_CommTimeouts.WriteTotalTimeoutMultiplier = 1000;
	m_CommTimeouts.WriteTotalTimeoutConstant = 1000;

	// 규이리 추가 [2008.10.17]
	BOOL bSetConf = TRUE;

	// configure
	if (SetCommTimeouts(m_hComm, &m_CommTimeouts))
	{
		// SetCommMask는 기본적으로 [EV_RXCHAR|EV_CTS]로 설정된다
		// RX 데이타가 수신되면 이벤트를 받음
		if (SetCommMask(m_hComm, dwzCommEvents))
		{
			// 규이리 추가 [2008.10.17]
			// 통신 포트의 실제적인 제어를 담당하는 DCB 구조체값 셋팅
			m_dcb.DCBlength = sizeof(DCB);

			// 현재 값을 읽음
			if (GetCommState(m_hComm, &m_dcb))
			{
				// 규이리 추가 [2010.02.25]
				m_dcb.fInX = m_dcb.fOutX = 1;  // Xon, Xoff 사용. 0이면 없음 fInX-입력흐름제어/fOutX-전송흐름제어
				//dcb.XonChar  = ASCII_XON;
				//dcb.XoffChar = ASCII_XOFF;
				m_dcb.XonLim  = 100;  // 전송 XON 제한치
				m_dcb.XoffLim = 100;  // 전송 XOFF 제한치

				m_dcb.fRtsControl = RTS_CONTROL_ENABLE;  // set RTS bit high!
				if (BuildCommDCB(szBaud, &m_dcb))
				{
					// 통신 포트를 설정된 값으로 재설정
					if (SetCommState(m_hComm, &m_dcb))	{
						; // normal operation... continue
					}
					else	{
						bSetConf = FALSE;  // 규이리 추가 [2008.10.17]
						ProcessErrorMessage("SetCommState()");
					}
				}
				else	{
					bSetConf = FALSE;
					ProcessErrorMessage("BuildCommDCB()");
				}
			}
			else	{
				bSetConf = FALSE;
				ProcessErrorMessage("GetCommState()");
			}
		}
		else	{
			bSetConf = FALSE;
			ProcessErrorMessage("SetCommMask()");
		}
	}
	else	{
		bSetConf = FALSE;
		ProcessErrorMessage("SetCommTimeouts()");
	}

	delete[] szPort;
	delete[] szBaud;

	// 규이리 추가 [2010.02.25]
	// 입출력 버퍼 크기 설정
	SetupComm(m_hComm, 4096, 4096);

	// flush the port
	PurgeComm(m_hComm, PURGE_RXCLEAR|PURGE_TXCLEAR|PURGE_RXABORT|PURGE_TXABORT);

	// release critical section
	LeaveCriticalSection(&m_csCommSync);
	//--------------------------------------------------------------//

	// 규이리 추가 [2008.10.17]
	if (bSetConf == FALSE)	{
		return FALSE;
	}

	TRACE("Initialisation for communicationport %d completed.\n", nzPortNr);
	return TRUE;
}

//==================================================================//
// 시리얼 통신 감시
// 통신 포트를 감시하고, 읽힌 내용이 있으면 MainWnd에 메시지를 보내어
// Buffer의 내용을 읽어가라고 신고한다
UINT CSerialPort::CommThread(LPVOID pParam)
{
	// Cast the void pointer passed to the thread back to
	// a pointer of CSerialPort class
	CSerialPort *port = (CSerialPort*)pParam;

	// 쓰레드 생성 플래그 설정 [성공]
	// Set the status variable in the dialog class to
	// TRUE to indicate the thread is running
	port->m_bThreadAlive = TRUE;
	//TRACE("[COM%d]   Flag [true]\n", port->m_nPortNr);
	
	//==============================================================//
	// 변수 초기화
	DWORD BytesTransfered = 0;
	DWORD Event = 0;
	DWORD CommEvent = 0;
	DWORD dwError = 0;
	COMSTAT comstat;
	BOOL bResult = TRUE;
	//--------------------------------------------------------------//

	// Clear comm buffers at startup
	// check if the port is opened
	if (port->m_hComm)	{
		PurgeComm(port->m_hComm, PURGE_RXCLEAR|PURGE_TXCLEAR|PURGE_RXABORT|PURGE_TXABORT);
	}

	// begin forever loop.  This loop will run as long as the thread is alive
	for(;;)
	{
		// Make a call to WaitCommEvent().  This call will return immediatly
		// because our port was created as an async port (FILE_FLAG_OVERLAPPED
		// and an m_OverlappedStructerlapped structure specified).  This call will cause the
		// m_OverlappedStructerlapped element m_OverlappedStruct.hEvent, which is part of the m_hEventArray to
		// be placed in a non-signeled state if there are no bytes available to be read,
		// or to a signeled state if there are bytes available.  If this event handle
		// is set to the non-signeled state, it will be set to signeled when a
		// character arrives at the port
		// 포트에 읽을 정보가 들어올때까지 대기
		// async로 Port를 생성했기 때문에 곧바로 리턴한다 (FILE_FLAG_OVERLAPPED)
		bResult = WaitCommEvent(port->m_hComm, &Event, &port->m_ov);
		if (!bResult)
		{
			// If WaitCommEvent() returns FALSE, process the last error to determin the reason..
			switch(dwError = GetLastError())
			{
			case ERROR_IO_PENDING :
				{
					// This is a normal return value if there are no bytes
					// to read at the port
					// Do nothing and continue
					break;
				}
			case 87 :
				{
					// Under Windows NT, this value is returned for some reason
					// I have not investigated why, but it is also a valid reply
					// Also do nothing and continue
					break;
				}
			default :
				{
					// All other error codes indicate a serious error has occured
					// Process this error
					port->ProcessErrorMessage("WaitCommEvent()");
					break;
				}
			}  // end of switch(dwError = GetLastError())
		}
		else
		{
			// If WaitCommEvent() returns TRUE, check to be sure there are
			// actually bytes in the buffer to read.
			//
			// If you are reading more than one byte at a time from the buffer
			// (which this program does not do) you will have the situation occur
			// where the first byte to arrive will cause the WaitForMultipleObjects()
			// function to stop waiting.  The WaitForMultipleObjects() function
			// resets the event handle in m_OverlappedStruct.hEvent to the non-signelead state
			// as it returns.
			//
			// If in the time between the reset of this event and the call to
			// ReadFile() more bytes arrive, the m_OverlappedStruct.hEvent handle will be set again
			// to the signeled state. When the call to ReadFile() occurs, it will
			// read all of the bytes from the buffer, and the program will
			// loop back around to WaitCommEvent().
			//
			// At this point you will be in the situation where m_OverlappedStruct.hEvent is set,
			// but there are no bytes available to read.  If you proceed and call
			// ReadFile(), it will return immediatly due to the async port setup, but
			// GetOverlappedResults() will not return until the next character arrives.
			//
			// It is not desirable for the GetOverlappedResults() function to be in
			// this state.  The thread shutdown event (event 0) and the WriteFile()
			// event (Event2) will not work if the thread is blocked by GetOverlappedResults().
			//
			// The solution to this is to check the buffer with a call to ClearCommError().
			// This call will reset the event handle, and if there are no bytes to read
			// we can loop back through WaitCommEvent() again, then proceed.
			// If there are really bytes to read, do nothing and proceed.

			bResult = ClearCommError(port->m_hComm, &dwError, &comstat);

			// 실재로 수신 버퍼에 Read할 문자가 존재하는지 검사
			if (comstat.cbInQue == 0)	{
				continue;
			}
		}  // end of if (!bResult)

		// Main wait function.  This function will normally block the thread
		// until one of nine events occur that require action.
		Event = WaitForMultipleObjects(3, port->m_hEventArray, FALSE, INFINITE);

		switch(Event)
		{
		case 0 :
			{
				// Shutdown event.
				// This is event zero so it will be the higest priority and be serviced first.
				port->m_bThreadAlive = FALSE;
				//TRACE("Flag [false]\n");

				// Kill this thread.
				// break is not needed, but makes me feel better.
				AfxEndThread(100);
				break;
			}
		case 1 :  // read event
			{
				// 데이터가 수신되었다는 메세지가 발생하면
				GetCommMask(port->m_hComm, &CommEvent);

				if (CommEvent & EV_CTS)	{
					::SendMessage(port->m_pOwner->m_hWnd, WM_COMM_CTS_DETECTED, (WPARAM)0, (LPARAM)port->m_nPortNr);
					//::PostMessage(port->m_pOwner->m_hWnd, WM_COMM_CTS_DETECTED, (WPARAM)0, (LPARAM) port->m_nPortNr);
				}
				if (CommEvent & EV_RXFLAG)	{
					::SendMessage(port->m_pOwner->m_hWnd, WM_COMM_RXFLAG_DETECTED, (WPARAM)0, (LPARAM)port->m_nPortNr);
					//::PostMessage(port->m_pOwner->m_hWnd, WM_COMM_RXFLAG_DETECTED, (WPARAM)0, (LPARAM) port->m_nPortNr);
				}
				if (CommEvent & EV_BREAK)	{
					::SendMessage(port->m_pOwner->m_hWnd, WM_COMM_BREAK_DETECTED, (WPARAM)0, (LPARAM)port->m_nPortNr);
					//::PostMessage(port->m_pOwner->m_hWnd, WM_COMM_BREAK_DETECTED, (WPARAM)0, (LPARAM) port->m_nPortNr);
				}
				if (CommEvent & EV_ERR)	{
					::SendMessage(port->m_pOwner->m_hWnd, WM_COMM_ERR_DETECTED, (WPARAM)0, (LPARAM)port->m_nPortNr);
					//::PostMessage(port->m_pOwner->m_hWnd, WM_COMM_ERR_DETECTED, (WPARAM)0, (LPARAM) port->m_nPortNr);
				}
				if (CommEvent & EV_RING)	{
					::SendMessage(port->m_pOwner->m_hWnd, WM_COMM_RING_DETECTED, (WPARAM)0, (LPARAM)port->m_nPortNr);
					//::PostMessage(port->m_pOwner->m_hWnd, WM_COMM_RING_DETECTED, (WPARAM)0, (LPARAM) port->m_nPortNr);
				}
				if (CommEvent & EV_RXCHAR)	{
					ReceiveChar(port, comstat);
				}
				break;
			}
		case 2 :  // write event
			{
				// Write character event from port
				WriteChar(port);
				break;
			}
		}  // end of switch(Event)
	}  // end of for(;;)
	return 0;
}

//==================================================================//
// 송/수신 담당 쓰레드 생성
BOOL CSerialPort::StartMonitoring()
{
	if (!(m_thrSerial = AfxBeginThread(CommThread, this)))
	{
		// 규이리 추가 [2008.10.17]
		ClosePort();  // 통신 포트 Close
		return FALSE;
	}

	TRACE("Thread Started\n");
	return TRUE;
}

//==================================================================//
// 통신 쓰레드 ReStart
BOOL CSerialPort::RestartMonitoring()
{
	TRACE("Thread Resumed\n");
	m_thrSerial->ResumeThread();

	return TRUE;
}

//==================================================================//
// 통신 쓰레드 Suspend 함수
BOOL CSerialPort::StopMonitoring()
{
	TRACE("Thread Suspended\n");
	m_thrSerial->SuspendThread();
	
	return TRUE;
}

//==================================================================//
// 규이리 추가 [2008.10.17]
// 통신 포트 Close
void CSerialPort::ClosePort()
{
	// 규이리 주석 처리 [2010.02.23]
	/*
	if (m_bThreadAlive)	{
		TRACE("[COM%d]  COMM Thread Ended\n", m_nPortNr);
	}
	
	do
	{
		SetEvent(m_hShutdownEvent);
	} while(m_bThreadAlive);
	*/
	do
	{
		if (m_bThreadAlive)	{
			//TRACE("[COM%d]   m_bThreadAlive [true]\n", m_nPortNr);
		}
		else	{
			//TRACE("[COM%d]   m_bThreadAlive [flase]\n", m_nPortNr);
		}

		SetEvent(m_hShutdownEvent);
	} while(m_bThreadAlive);
	TRACE("Thread ended\n");

	// 규이리 추가 [2010.02.23]
	if (m_cpWriteBuff != NULL)	{
		delete [] m_cpWriteBuff;

		// 규이리 추가 [2010.08.23]
		// Port Close 후에 다시 Open하는 경우 문제 발생하여 추가함
		m_cpWriteBuff = NULL;
	}

	// 마스크 해제
	SetCommMask(m_hComm, 0);

	// 포트를 비운다
	PurgeComm(m_hComm, PURGE_RXCLEAR|PURGE_TXCLEAR|PURGE_RXABORT|PURGE_TXABORT);

	// Port가 이미 열려있으면 닫는다
	if (m_hComm != NULL)
	{
		CloseHandle(m_hComm);
		m_hComm = NULL;

		// 규이리 추가 [2010.02.27]
		if (!m_thrSerial)
		{
			if (WaitForSingleObject(m_thrSerial, INFINITE) == WAIT_TIMEOUT)	{
				TerminateThread(m_thrSerial, 1L);  // Thread Terminate
			}
			//CloseHandle(m_thrSerial);
			m_thrSerial = NULL;
		}
	}
}

//==================================================================//
// 에러 메시지 출력
void CSerialPort::ProcessErrorMessage(char* cpErrorText)
{
	char *Temp = new char[200];

	LPVOID lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),  // Default language
		(LPTSTR) &lpMsgBuf,
		0,
		NULL);
	
	sprintf(Temp, "WARNING:  %s Failed with the following error: \n%s\nPort: %d\n", (char*)cpErrorText, lpMsgBuf, m_nPortNr);
	MessageBox(NULL, Temp, "Application Error", MB_ICONSTOP);

	LocalFree(lpMsgBuf);
	delete[] Temp;
}

//==================================================================//
// 문자열 송신
// LeaveCriticalSection() 함수 처리 부분 수정
void CSerialPort::WriteChar(CSerialPort* pPort)
{
	BOOL bWrite  = TRUE;
	BOOL bResult = TRUE;
	DWORD dwBytesSent = 0;
	
	// 규이리 추가 [2008.10.20]
	// 포트가 연결되어 있지 않은 상태이면
	if (!(pPort->m_bThreadAlive))
	{
		pPort->ProcessErrorMessage("Port not alive in WriteFile()");
		return;
	}
	
	ResetEvent(pPort->m_hWriteEvent);
	
	// Gain ownership of the critical section
	EnterCriticalSection(&pPort->m_csCommSync);

	if (bWrite)
	{
		// Initailize variables
		pPort->m_ov.Offset = 0;
		pPort->m_ov.OffsetHigh = 0;

		// Clear buffer
		PurgeComm(pPort->m_hComm, PURGE_RXCLEAR|PURGE_TXCLEAR|PURGE_RXABORT|PURGE_TXABORT);

		// 인자로 들어온 버퍼의 내용을 m_cpWriteBuff 만큼 Write하고
		// Write한 갯수를 dwBytesSent에 넘긴다
		bResult = WriteFile(pPort->m_hComm,       // Handle to COMM Port
			pPort->m_cpWriteBuff,                 // Pointer to message buffer in calling function
			strlen((char*)pPort->m_cpWriteBuff),  // Length of message to send
			&dwBytesSent,                         // Where to store the number of bytes sent
			&pPort->m_ov);                        // Overlapped structure
		
		// deal with any error codes
		if (!bResult)
		{
			DWORD dwError = GetLastError();
			switch(dwError)
			{
			case ERROR_IO_PENDING :
				{
					// continue to GetOverlappedResults()
					dwBytesSent = 0;
					bWrite = FALSE;
					break;
				}
			default :
				{
					// all other error codes
					pPort->ProcessErrorMessage("WriteFile()");
					break;  // 규이리 추가 [2008.10.16]
				}
			}
		}
		else	{
			LeaveCriticalSection(&pPort->m_csCommSync);
			bWrite = TRUE;  // 규이리 추가 [2008.10.16]
		}
	}  // end of if (bWrite)

	if (!bWrite)
	{
		bWrite = TRUE;

		// 읽을 문자가 남아 있거나 전송할 문자가 남아 있을 경우 Overapped IO의
		// 특성에 따라 ERROR_IO_PENDING 에러 메시지가 전달된다
		bResult = GetOverlappedResult(pPort->m_hComm,  // Handle to COMM port 
			&pPort->m_ov,    // Overlapped structure
			&dwBytesSent,    // Stores number of bytes sent
			TRUE);           // Wait flag
		
		LeaveCriticalSection(&pPort->m_csCommSync);

		// deal with the error code 
		if (!bResult)	{
			pPort->ProcessErrorMessage("GetOverlappedResults() in WriteFile()");
		}
	}

	// Verify that the data size send equals what we tried to send
	if (dwBytesSent != strlen((char*)pPort->m_cpWriteBuff))
	{
		TRACE("WARNING: WriteFile() error.. Bytes Sent: %d; Message Length: %d\n", dwBytesSent, strlen((char*)pPort->m_cpWriteBuff));
	}
}

//==================================================================//
// 문자열 수신 함수
void CSerialPort::ReceiveChar(CSerialPort* pPort, COMSTAT comstat)
{
	BOOL bRead   = TRUE;
	BOOL bResult = TRUE;
	DWORD dwError = 0;
	DWORD dwBytesRead = 0;
	unsigned char RXBuff;

	for(;;)
	{
		// Gain ownership of the comm port critical section.
		// This process guarantees no other part of this program
		// is using the port object.
		EnterCriticalSection(&pPort->m_csCommSync);

		// ClearCommError() will update the COMSTAT structure and
		// clear any other errors.
		// system queue에 도착한 byte수만 미리 읽는다
		bResult = ClearCommError(pPort->m_hComm, &dwError, &comstat);
		
		LeaveCriticalSection(&pPort->m_csCommSync);

		// start forever loop.  I use this type of loop because I
		// do not know at runtime how many loops this will have to run.
		// My solution is to start a forever loop and to
		// break out of it when I have processed all of the
		// data available.  Be careful with this approach and
		// be sure your loop will exit.
		// My reasons for this are not as clear in this sample
		// as it is in my production code, but I have found this
		// solutiion to be the most efficient way to do this.
		if (comstat.cbInQue == 0)	{
			// break out when all bytes have been read
			break;
		}

		EnterCriticalSection(&pPort->m_csCommSync);

		if (bRead)
		{
			// //오버랩대신 NULL로 하면 통신 안됨
			bResult = ReadFile(pPort->m_hComm,  // Handle to COMM port 
				&RXBuff,                        // RX Buffer Pointer
				1,                              // Read one byte
				&dwBytesRead,                   // Stores number of bytes read
				&pPort->m_ov);                  // pointer to the m_ov structure
			
			// deal with the error code
			if (!bResult)
			{
				switch(dwError = GetLastError())
				{
				case ERROR_IO_PENDING :  // 입출력 대기상태
					{
						// 읽을 문자가 남아 있거나 전송할 문자가 남아 있을 경우 Overapped IO의 특성에 따라
						// ERROR_IO_PENDING에러메세지가 전달 된다.
						// timeout에 정해준 시간만큼 기다려준다.
						// asynchronous i/o is still in progress
						// Proceed on to GetOverlappedResults();
						bRead = FALSE;
						break;
					}
				default :
					{
						// Another error has occured.  Process this error.
						pPort->ProcessErrorMessage("ReadFile()");
						break;
					}
				}  // End switch
			}
			else
			{
				// ReadFile() returned complete.
				// It is not necessary to call GetOverlappedResults()
				bRead = TRUE;
			}
		}  // end of if (bRead)

		// 읽기 실패인 경우 Timeouts에 정해준 시간만큼 기다려준다
		if (!bRead)
		{
			bRead = TRUE;
			bResult = GetOverlappedResult(pPort->m_hComm,  // Handle to COMM port 
				&pPort->m_ov,    // Overlapped structure
				&dwBytesRead,    // Stores number of bytes read
				TRUE);           // Wait flag
			
			// deal with the error code
			if (!bResult)	{
				pPort->ProcessErrorMessage("GetOverlappedResults() in ReadFile()");
			}
		}

		LeaveCriticalSection(&pPort->m_csCommSync);

		// notify parent that a byte was received
		::SendMessage((pPort->m_pOwner)->m_hWnd, WM_COMM_RXCHAR, (WPARAM)RXBuff, (LPARAM)pPort->m_nPortNr);
		//::PostMessage((pPort->m_pOwner)->m_hWnd, WM_COMM_RXCHAR, (WPARAM)RXBuff, (LPARAM)pPort->m_nPortNr);
	}  // end of for(;;)
}

//==================================================================//
// 문자열 전송
void CSerialPort::WriteToPort(char* cpString)
{
	assert(m_hComm != 0);

	memset(m_cpWriteBuff, 0, sizeof(m_cpWriteBuff));
	strcpy(m_cpWriteBuff, cpString);

	// set event for write
	SetEvent(m_hWriteEvent);
}

//==================================================================//
// Device Control Block 반환
DCB CSerialPort::GetDCB()
{
	return m_dcb;
}

//==================================================================//
// 통신 이벤트 마스크 상태 반환
DWORD CSerialPort::GetCommEvents()
{
	return m_dwCommEvents;
}

//==================================================================//
// 송신 버퍼 크기 반환
DWORD CSerialPort::GetWriteBufferSize()
{
	return m_dwWriteBuffSize;
}

//==================================================================//
// 포트 Open 상태 반환
BOOL CSerialPort::IsOpen()
{
	// 포트가 열려있어도 쓰레드가 살지 않았으면
	// 아무런 의미가 없기에 쓰레드 핸들로 수정
	// 규이리 수정 [2008.10.22]
	// Port가 이미 열려있으면
	if (m_hComm == NULL ||
		m_hComm == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}
	/*
	if (!m_thrSerial)	{
		return FALSE;
	}
	*/
	return TRUE;
}
