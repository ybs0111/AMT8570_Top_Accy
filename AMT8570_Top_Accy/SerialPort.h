// SerialPort.h: interface for the CSerialPort class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SERIALPORT_H__D6FD0EB5_8201_454B_B4D8_7FB035741C41__INCLUDED_)
#define AFX_SERIALPORT_H__D6FD0EB5_8201_454B_B4D8_7FB035741C41__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//==================================================================//
// 사용자 정의 메시지 선언
#define WM_COMM_BREAK_DETECTED    WM_USER+61  // A break was detected on input.
#define WM_COMM_CTS_DETECTED      WM_USER+62  // The CTS (clear-to-send) signal changed state.
#define WM_COMM_DSR_DETECTED      WM_USER+63  // The DSR (data-set-ready) signal changed state.
#define WM_COMM_ERR_DETECTED      WM_USER+64  // A line-status error occurred. Line-status errors are CE_FRAME, CE_OVERRUN, and CE_RXPARITY.
#define WM_COMM_RING_DETECTED     WM_USER+65  // A ring indicator was detected.
#define WM_COMM_RLSD_DETECTED     WM_USER+66  // The RLSD (receive-line-signal-detect) signal changed state.
#define WM_COMM_RXCHAR            WM_USER+67  // A character was received and placed in the input buffer.
#define WM_COMM_RXFLAG_DETECTED   WM_USER+68  // The event character was received and placed in the input buffer.
#define WM_COMM_TXEMPTY_DETECTED  WM_USER+69  // The last character in the output buffer was sent.
//------------------------------------------------------------------//

#define dPORT_MAX  20  // 최대 포트 갯수

class CSerialPort  
{
public:
	CSerialPort();
	virtual ~CSerialPort();

	// Attributes
protected:
	CWinThread* m_thrSerial;

	//==============================================================//
	// 동기화 변수
	CRITICAL_SECTION  m_csCommSync;
	BOOL m_bThreadAlive;
	//--------------------------------------------------------------//

	//==============================================================//
	// 핸들 변수
	HANDLE m_hShutdownEvent;
	HANDLE m_hComm;
	HANDLE m_hWriteEvent;
	//--------------------------------------------------------------//

	//==============================================================//
	// 이벤트 처리를 위한 변수
	// One element is used for each event.
	// There are two event handles for each port.
	// A Write event and a receive character event
	// which is located in the overlapped structure (m_ov.hEvent).
	// There is a general shutdown when the port is closed.
	HANDLE m_hEventArray[3];
	//--------------------------------------------------------------//

	//==============================================================//
	// 구조체 선언
	OVERLAPPED   m_ov;
	COMMTIMEOUTS m_CommTimeouts;
	DCB m_dcb;
	//--------------------------------------------------------------//

	CWnd* m_pOwner;  // 윈도우 핸들

	//==============================================================//
	// 통신 파라메터 변수
	UINT  m_nPortNr;
	char* m_cpWriteBuff;
	DWORD m_dwCommEvents;
	DWORD m_dwWriteBuffSize;
	//--------------------------------------------------------------//
	
	// Operations
public:
	BOOL InitPort(CWnd* pzPortOwner, UINT nzPortNr = 1,
		UINT nzBaud = 19200, char czParity = 'N', UINT nzDatabits = 8, UINT nzStopbits = 1,
		DWORD dwzCommEvents = EV_RXCHAR|EV_CTS, UINT nzWriteBuffSize = 512);  // Port 초기화 함수
	
	BOOL StartMonitoring();
	BOOL RestartMonitoring();
	BOOL StopMonitoring();

	// 규이리 추가 [2008.10.17]
	void ClosePort();  // 통신 포트 Close
	BOOL IsOpen();  // 포트 Open 상태 리턴
	
	DWORD GetWriteBufferSize();
	DWORD GetCommEvents();
	DCB GetDCB();
	
	void WriteToPort(char* cpString);  // Message Write 함수

	// Operations
protected:
	void ProcessErrorMessage(char* cpErrorText);
	static UINT CommThread(LPVOID pParam);
	static void ReceiveChar(CSerialPort* pPort, COMSTAT comstat);
	static void WriteChar(CSerialPort* pPort);

};

#endif // !defined(AFX_SERIALPORT_H__D6FD0EB5_8201_454B_B4D8_7FB035741C41__INCLUDED_)
