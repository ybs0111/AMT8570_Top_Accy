// MyJamData.h: interface for the CMyJamData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYJAMDATA_H__CB595225_06C8_44CA_9BC6_742D8AC656E1__INCLUDED_)
#define AFX_MYJAMDATA_H__CB595225_06C8_44CA_9BC6_742D8AC656E1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Variable.h"       // 전역 변수 정의 클래스

class CMyJamData  
{
public:
	CMyJamData();
	virtual ~CMyJamData();

// Attributes
public:

// Operations
public:
	CString OnLoad_AlarmDispInfo(CString szPath, CString szDataFile, CString szActFile, CString szCode);	// 발생한 알람에 대한 출력 정보 설정
	CString OnSearch_AlarmBmpInfo(CString szPath, CString szDataFile, CString szCode);			// 발생한 알람에 대한 비트맵 이미지 정보 찾기
	CString OnSearch_AlarmDispStyleInfo(CString szPath, CString szDataFile, CString szCode);	// 발생한 알람에 대한 화면 출력 스타일 정보 찾기
	CString OnSearch_AlarmRepairInfo(CString szPath, CString szActFile, CString szCode);		// 발생한 알람에 대한 조치 사항 정보 찾기

	void On_Alarm_Display();					// 발생한 알람 화면 출력 함수
	void OnSave_AlarmInfo2LogFile();	// [로그 파일] 발생한 알람 정보 파일에 저장
	void OnLoad_AlarmInfoFronFile();	// [파일->구조체] 모든 알람 정보 로딩

	void On_Occured_Alarm_Saving_to_File();		// 발생한 알람 로그 파일에 저장하는 함수
	void On_Alarm_Info_Set_to_Variable(CString strErrCode);  // 발생한 알람 메시지 전역 변수에 설정하는 함수
	void OnSet_Info2StrctAlarm(CString szJamCode);	// 발생한 알람 정보를 구조체에 할당
	void OnAdd_2DailyFile(CString szJamCode, CString szTimeDisp, CString sJamMsg);	// [일별] 알람 정보 파일에 저장

};
extern CMyJamData	MyJamData;

#endif // !defined(AFX_MYJAMDATA_H__CB595225_06C8_44CA_9BC6_742D8AC656E1__INCLUDED_)
