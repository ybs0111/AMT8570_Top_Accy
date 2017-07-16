// Logger.cpp: implementation of the CLogger class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Logger.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CLogger	cLOG;  // 클래스 접근자 선언

CLogger::CLogger()
{

}

CLogger::~CLogger()
{

}

//==================================================================//
// Log Event 저장할 폴더 생성
void CLogger::OnSetPathLogEvent() 
{
	//CString sMakeFolder = OnGet_CurrentFolder();  // 현재 작업 폴더 반환

	// Log Creating Path Setting
	CString sMakeFolder;
	sMakeFolder = CONFIG_PATH_BASE_ + _T("\\SOCKET\\");
	m_cLogSocket.OnSetPath(sMakeFolder);

	sMakeFolder = CONFIG_PATH_BASE_ + _T("\\COMM\\");
	m_cLogComm.OnSetPath(sMakeFolder);

	sMakeFolder = CONFIG_PATH_BASE_ + _T("\\PGM\\");
	m_cLogPgm.OnSetPath(sMakeFolder);

	sMakeFolder = CONFIG_PATH_BASE_ + _T("\\XGEM\\");
	m_cLogXGem.OnSetPath(sMakeFolder);

	sMakeFolder = CONFIG_PATH_BASE_ + _T("\\SEQ\\");
	m_cLogSeq.OnSetPath(sMakeFolder);
	
	sMakeFolder = CONFIG_PATH_BASE_ + _T("\\BCR\\");
	m_cLogBcr.OnSetPath(sMakeFolder);
}

//==================================================================//
// Log 메시지 추가
void CLogger::OnLogEvent(int nzLogType, CString szLogEvent) 
{
	CString sFileName;  // 생성할 Log 파일명
	
	SYSTEMTIME  systime;
	GetLocalTime(&systime);

	// 소켓 통신 관련 로그
	if (nzLogType == LOG_SOCKET_)
	{
		// 소켓 통신 관련 로그 작성
		sFileName.Format("%i\\%02i\\LOG_SOCKET_%02i%02i%02i.txt",
			systime.wYear,
			systime.wMonth,
			systime.wYear,
			systime.wMonth,
			systime.wDay);
		m_cLogSocket.OnChangeFile(sFileName);
		m_cLogSocket.OnWrite(szLogEvent);

		// 전체 시컨스 과련 로그 작성
		sFileName.Format("%i\\%02i\\LOG_SEQ_%02i%02i%02i.txt",
			systime.wYear,
			systime.wMonth,
			systime.wYear,
			systime.wMonth,
			systime.wDay);
		
		m_cLogSeq.OnChangeFile(sFileName);
		m_cLogSeq.OnWrite(szLogEvent);
	}
	// 시리얼 통신 관련 로그
	else if (nzLogType == LOG_COMM_)
	{
		sFileName.Format("%i\\%02i\\LOG_COMM_%02i%02i%02i.txt",
			systime.wYear,
			systime.wMonth,
			systime.wYear,
			systime.wMonth,
			systime.wDay);
		m_cLogComm.OnChangeFile(sFileName);
		m_cLogComm.OnWrite(szLogEvent);

		sFileName.Format("%i\\%02i\\LOG_SEQ_%02i%02i%02i.txt",
			systime.wYear,
			systime.wMonth,
			systime.wYear,
			systime.wMonth,
			systime.wDay);
		m_cLogSeq.OnChangeFile(sFileName);
		m_cLogSeq.OnWrite(szLogEvent);
	}
	// 화면 셋팅 관련 로그
	else if (nzLogType == LOG_PGM_)
	{
		sFileName.Format("%i\\%02i\\LOG_PGM_%02i%02i%02i.txt",
			systime.wYear,
			systime.wMonth,
			systime.wYear,
			systime.wMonth,
			systime.wDay);
		m_cLogPgm.OnChangeFile(sFileName);
		m_cLogPgm.OnWrite(szLogEvent);

		sFileName.Format("%i\\%02i\\LOG_SEQ_%02i%02i%02i.txt",
			systime.wYear,
			systime.wMonth,
			systime.wYear,
			systime.wMonth,
			systime.wDay);
		m_cLogSeq.OnChangeFile(sFileName);
		m_cLogSeq.OnWrite(szLogEvent);
	}
	// 시컨스 동작 관련 로그
	else if (nzLogType == LOG_SEQ_)
	{
		sFileName.Format("%i\\%02i\\LOG_SEQ_%02i%02i%02i.txt",
			systime.wYear,
			systime.wMonth,
			systime.wYear,
			systime.wMonth,
			systime.wDay);
		m_cLogSeq.OnChangeFile(sFileName);
		m_cLogSeq.OnWrite(szLogEvent);
	}
	// 바코드 관련 로그
	else if (nzLogType == LOG_BCR_)
	{
		sFileName.Format("%i\\%02i\\LOG_BCR_%02i%02i%02i.txt",
			systime.wYear,
			systime.wMonth,
			systime.wYear,
			systime.wMonth,
			systime.wDay);
		m_cLogBcr.OnChangeFile(sFileName);
		m_cLogBcr.OnWrite(szLogEvent);
	}
	// Accy 공급 로그
	else if (nzLogType == LOG_SUPPLY_COUNT_)
	{
		sFileName.Format("%i\\%02i\\LOG_SUPPLY_COUNT_%02i%02i%02i.txt",
			systime.wYear,
			systime.wMonth,
			systime.wYear,
			systime.wMonth,
			systime.wDay);
		m_cLogBcr.OnChangeFile(sFileName);
		m_cLogBcr.OnWrite(szLogEvent);
	}
}
