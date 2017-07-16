// Logger.h: interface for the CLogger class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGGER_H__2CB92E1F_4374_4127_A967_82AC833F8E06__INCLUDED_)
#define AFX_LOGGER_H__2CB92E1F_4374_4127_A967_82AC833F8E06__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// 규이리 추가 [2011.05.26]
#include "Variable.h"

#include "LogFile.h"  // Log 클래스

class CLogger  
{
public:
	CLogger();
	virtual ~CLogger();

// Attributes
public:
	void OnSetPathLogEvent();  // Log Event 저장할 폴더 생성
	void OnLogEvent(int nzLogType, CString szLogEvent);  // Log 메시지 추가
	
// Operations
public:
	// Log 관련 클래스 변수 선언
	CLogFile m_cLogSocket;	//logsoket
	CLogFile m_cLogXGem;
	CLogFile m_cLogComm;
	CLogFile m_cLogPgm;
	CLogFile m_cLogSeq;
	CLogFile m_cLogBcr;

};
extern CLogger	cLOG;

#endif // !defined(AFX_LOGGER_H__2CB92E1F_4374_4127_A967_82AC833F8E06__INCLUDED_)
