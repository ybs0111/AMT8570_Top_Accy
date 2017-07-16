// MyJamData.cpp: implementation of the CMyJamData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Handler.h"
#include "MyJamData.h"

#include <direct.h>
#include "io.h" 
#include "Public_Function.h"  // 모터 보드 및 I/O 보드 관련 클래스
#include "FastechPublic_IO.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMyJamData	MyJamData;

CMyJamData::CMyJamData()
{

}

CMyJamData::~CMyJamData()
{

}

//==================================================================//
// 발생한 알람에 대한 출력 정보 설정
//==================================================================//
CString CMyJamData::OnLoad_AlarmDispInfo(CString szPath, CString szDataFile, CString szActFile, CString szCode) 
{
	CString sPathInfo = szPath;
	if (sPathInfo.Right(1) != "\\")
	{
		sPathInfo += "\\";
	}

	CString sRetVal = "";
	sRetVal = OnSearch_AlarmBmpInfo(sPathInfo, szDataFile, szCode);			// 발생한 알람에 대한 비트맵 이미지 정보 찾기
	int nPos = sRetVal.Find("Trouble");
	if (nPos >= 0)	// 에러 발생
	{
		return sRetVal;
	}

	sRetVal = OnSearch_AlarmDispStyleInfo(sPathInfo, szDataFile, szCode);	// 발생한 알람에 대한 화면 출력 스타일 정보 찾기
	nPos = sRetVal.Find("Trouble");
	if (nPos >= 0)	// 에러 발생
	{
		return sRetVal;
	}

	sRetVal = OnSearch_AlarmRepairInfo(sPathInfo, szActFile, szCode);	// 발생한 알람에 대한 조치 사항 정보 찾기
	nPos = sRetVal.Find("Trouble");
	if (nPos >= 0)	// 에러 발생
	{
		return sRetVal;
	}

	return sRetVal;
}

//==================================================================//
// 발생한 알람에 대한 비트맵 이미지 정보 찾기
//==================================================================//
CString CMyJamData::OnSearch_AlarmBmpInfo(CString szPath, CString szDataFile, CString szCode) 
{
	CString sReturnVal;
	sReturnVal = "OK";

	CString sBmpFile = szPath + szDataFile;	// [폴더+파일명]

	char chBuff[1000];
	if (!GetPrivateProfileString(szCode, "DEF", "", chBuff, 1000, sBmpFile))
	{
		sReturnVal  = "File Name : " + sBmpFile;
		sReturnVal += "\r\n";
		sReturnVal += "[DEF] The information we read. Trouble -> Code : ";
		sReturnVal += szCode;

		return sReturnVal;
	}

	CString sTemp;
	sTemp.Format("%s", chBuff);
	st_alarm.mstr_bmp_file = sTemp;

	return sReturnVal;
}

//==================================================================//
// 발생한 알람에 대한 화면 출력 스타일 정보 찾기
//==================================================================//
CString CMyJamData::OnSearch_AlarmDispStyleInfo(CString szPath, CString szDataFile, CString szCode) 
{
	CString sReturnVal;
	sReturnVal = "OK";

	CString sCheckFile = szPath + szDataFile;	// [폴더+파일명]

	char chBuff[1000];
	char num[20];
	if (!GetPrivateProfileString(szCode, "STYLE", "", chBuff, 1000, sCheckFile))
	{
		sReturnVal  = "File Name : " + sCheckFile;
		sReturnVal += "\r\n";
		sReturnVal += "[STYLE] The information we read. Trouble -> Code : ";
		sReturnVal += szCode;

		return sReturnVal;
	}

	CString sTemp1, sTemp2;
	int start=0, end, i;
	int max=4;
	
	// 알람 화면 출력 모양 설정
	sTemp1.Format("%s", chBuff);

	for(i=0; i<max; i++)
	{
		end = sTemp1.Find(';', start);
		// 데이터 사이의 구분자 미존재
		if (end == -1)
		{
			break;
		}
		// 데이터 사이의 구분자 존재
		else
		{
			// 구반자 사이의 한개 아이템 설정
			sTemp2 = sTemp1.Mid(start, end-start);
			sTemp2.TrimLeft(' ');
			sTemp2.TrimRight(' ');

			if (i == 0)
			{
				st_alarm.mstr_style = sTemp2;	// 알람 화면 출력 형태 설정
				if (st_alarm.mstr_style == "CIRCLE")
				{
					// 알람 표시 유형이 원이면 필요한 데이터의 갯수는 8개임
					max = 8;
				}
				else if (st_alarm.mstr_style == "NONE")
				{
					max = 1;
				}
			}
			else
			{
				lstrcpy(num, sTemp2);

				if (i == 1)
				{
					st_alarm.mn_Cx = atoi(num);	// X축 픽셀 위치
				}
				else if (i == 2)
				{
					st_alarm.mn_Cy = atoi(num);	// Y축 픽셀 위치
				}
				else if (i == 3)
				{
					st_alarm.mn_Offset = atoi(num);	// 원크기
				}
				else if (i == 4)
				{
					st_alarm.mn_LineWidth = atoi(num);	// 표시 라인 두께
				}
				else if (i == 5)
				{
					st_alarm.mn_R = atoi(num);	// RGB 값 중 R값
				}
				else if (i == 6)
				{
					st_alarm.mn_G = atoi(num);	// RGB 값 중 G값
				}
				else if (i == 7)
				{
					st_alarm.mn_B = atoi(num);	// RGB 값 중 B값
				}
			}
			start = end +1;	// 다음 위치로 이동
		}
	}

	if (i != max)
	{
		sReturnVal  = "File Name : " + sCheckFile;
		sReturnVal += "\r\n";
		sReturnVal += "[STYLE] Data Format. Trouble -> Code : ";
		sReturnVal += szCode;

		return sReturnVal;
	}
	return sReturnVal;
}

//==================================================================//
// 발생한 알람에 대한 조치 사항 정보 찾기
//==================================================================//
CString CMyJamData::OnSearch_AlarmRepairInfo(CString szPath, CString szActFile, CString szCode) 
{
	CString sRetVal;
	sRetVal = "OK";

	CString sRepairFile = szPath + szActFile;	// [폴더+파일명]
	char chBuff[1000];

	if (!GetPrivateProfileString(szCode, "COUNT", "0", chBuff, 10, sRepairFile))
	{
		sRetVal  = "File Name : " + sRepairFile;
		sRetVal += "\r\n";
		sRetVal += "[COUNT] The information we read. Trouble -> Code : ";
		sRetVal += szCode;

		return sRetVal;
	}

	// 읽어야할 조치 사항 갯수 얻음
	int nReadRepairCnt = atoi(chBuff);
	// 조치 사항이 없는 경우
	if (nReadRepairCnt < 1)
	{
		sRetVal  = "File Name : " + sRepairFile;
		sRetVal += "\r\n";
		sRetVal += "[COUNT] Data Format Trouble -> Code : ";
		sRetVal += szCode;

		return sRetVal;
	}
	else
	{
		st_alarm.mn_action_cnt = nReadRepairCnt;
	}
	// 조치 사항 정보 클리어
	(st_alarm.mstr_repair_msg).Empty();

	CString sActCaption, sActItemMsg;
	int i;
	for(i=0; i<(st_alarm.mn_action_cnt); i++)
	{
		sActCaption.Format("%d", i+1);
		if (!GetPrivateProfileString(szCode, (LPCTSTR)sActCaption, "", chBuff, 1000, sRepairFile))
		{
			sRetVal  = "File Name : " + sRepairFile;
			sRetVal += "\r\n";
			sRetVal += "An action subject. The information we read. Trouble";
			sRetVal += "\r\n";
			sRetVal += "Code : ";
			sRetVal += szCode;
			sRetVal += "\r\n";
			sRetVal += "Position : ";
			sRetVal += sActCaption;

			return sRetVal;
		}
		st_alarm.mstr_repair_msg += "\r\n";

		sActItemMsg.Format(" %s", chBuff);
		st_alarm.mstr_repair_msg += sActItemMsg;

		// 마지막 조치사항 정보가 아니면 줄바꿈 추가
		if (i != ((st_alarm.mn_action_cnt) -1))
		{
			st_alarm.mstr_repair_msg += "\r\n";
		}
	}
	return sRetVal;
}

//==================================================================//
// 발생한 알람 화면 출력
//==================================================================//
void CMyJamData::On_Alarm_Display() 
{
	// 규이리 추가 [2014.09.01]
	// 초기화 작업용 타이머 생성 후부터 알람 검출하도록 수정함
	if (st_handler.nStart_INIT == CTL_NO)
	{
		return;
	}

	COleDateTime cur_time;
	CTime ctJamHappened;
	int nCurrHour=0;
	int nCheck=0;
	int nIsDataSave=0;
	int i;
	long lCheckTime=0;
	CString sTimeDisplay;
	CString sCount, sTempData;
	char chTempCode[10]={0,}, chTempCount[10]={0,}, chBuff[20]={0,};

	if (st_work.nEqpStatus == CTL_dJAM ||
		st_work.nEqpStatus == CTL_dWARNING ||
		st_work.nEqpStatus == CTL_dLOTEND)
	{
		// 알람이 발생했어도 이미 알람 화면이 출력되어 있으면 리턴
		// - 현재 발생한 알람으로 갱신하지 않아도 되기 때문
		if (st_handler.nShowAlarmDlg == TRUE)
		{
			return;
		}

		if (st_handler.nSysLock != FALSE)
		{
			return;	// 강제 리턴
		}

		// 알람 발생 시간 정보 설정
		cur_time		= COleDateTime::GetCurrentTime();
		nCurrHour		= cur_time.GetHour();
		ctJamHappened	= CTime::GetCurrentTime();
		sTimeDisplay	= ctJamHappened.Format("%c");

		// 알람 화면이 출력되지 않은 경우에만 알람 화면 출력함
		if (st_handler.nShowAlarmDlg != TRUE)
		{
			alarm.nReset_AlarmStatus = CTL_YES;	// 알람 리셋 동작 필요하다고 설정

			// 발생한 알람을 리스트에 추가함
			if (alarm.mstr_code != "")
			{
				//2015.0604
				st_basic.mn_alarm_delay_time = 120;//(2min)
				// 현재 발생한 알람을 설정
				st_handler.mstr_last_alarm = alarm.mstr_code;
				OnSet_Info2StrctAlarm(alarm.mstr_code);	// 발생한 알람 정보를 구조체에 할당

				sprintf(chTempCode, "%s", alarm.mstr_code);
				sCount.Format("%d", st_alarm.mn_cur_state);
				sprintf(chTempCount, "%s", sCount);

				alarm.stl_cur_alarm_time = GetCurrentTime();

				nCheck = atoi(alarm.mstr_code);
				nIsDataSave = CTL_NO;

				// 바로 전에 발생할 알람 코드와 비교
				// - 동일한 알람에 대해서는 일정 시간 경과한 후에 발생한 알람만 처리함
				if (alarm.stl_prev_code == nCheck)
				{
					lCheckTime = alarm.stl_cur_alarm_time - alarm.stl_alarm_start_time;
					if (lCheckTime < (st_basic.mn_alarm_delay_time)*1000 ||
						alarm.stl_alarm_start_time == 0)
					{
						alarm.stl_prev_alarm_time = alarm.stl_cur_alarm_time;
						// 규이리 추가 [2014.09.01]
						nIsDataSave = CTL_NO;
					}
					else
					{
						alarm.stl_prev_alarm_time = alarm.stl_cur_alarm_time;
						nIsDataSave = CTL_YES;
					}
				}
				else
				{
					alarm.stl_prev_code			= nCheck;
					alarm.stl_prev_alarm_time	= alarm.stl_cur_alarm_time;

					nIsDataSave = CTL_YES;
				}
			}

			if (st_alarm.mstr_cur_state == "1" &&
				nIsDataSave == CTL_YES)
			{
				for(i=0; i<2000; i++)
				{
					if (alarm.mstr_code == st_alarm.mstr_e_code[i])
					{
						(st_alarm.mn_e_count[i])++;
						break;
					}
				}
			}

			if (nIsDataSave == CTL_YES)
			{
				if (st_alarm.mstr_cur_state == "1")
				{
					(alarm.mn_alarm_occured)++;
					(alarm.mn_alarm_occured_day)++;

					sTempData = LPCTSTR(_itoa(alarm.mn_alarm_occured_day, chBuff, 10));
					::WritePrivateProfileString("BasicData", "Day_Alarm_Count", LPCTSTR(sTempData), st_path.mstr_basic);
				}

				if (st_handler.mn_status == CTL_YES)
				{
					OnSave_AlarmInfo2LogFile();	// [로그 파일] 발생한 알람 정보 파일에 저장
				}
			}

			// 알람 화면 출력할 거라고 미리 플래그 설정
			st_handler.nShowAlarmDlg = TRUE;

			// 김양규 추가 2014.09.23	alarm.mstr_code		st_alarm.mstr_cur_msg
			// 알람 발생시 Main으로 알람 코드와 알람 타일틀 전송
			g_client_accy.OnCreate_SendFormat(MAIN_ALARM);
			g_client_accy.OnReq_CommStart();

			Func.OnSet_EqpStatusAlarm(st_alarm.mn_cur_state);	// [ALARM] 설비 상태 변경
			// 메인 프레임에 알람 화면 출력 요청
			::PostMessage(st_handler.hWnd, WM_FORM_CHANGE, 7, 1);

			// 규이리 주석 처리 [2014.09.01]
			// 쓰레드에서 자체적으로 출력하도록 수정함
			/*
			if (st_handler.cwnd_title != NULL)
			{
				st_handler.cwnd_title->PostMessage(WM_STATUS_CHANGE, MACHINE_STATUS, st_work.nEqpStatus);
			}
			*/
		}
	}
}

void CMyJamData::On_Alarm_Info_Set_to_Variable(CString strErrCode)
{
	int n_cnt ;
	int n_cur_num = -1;    // 알람에 대한 배열 위치 저장 변수
	CString str_err_code;  // 알람 Code 저장 변수
	
	// **************************************************************************
    // 입력된 알람 Code에서 좌/우 공백을 제거한다                                
    // **************************************************************************
	str_err_code = strErrCode;
	str_err_code.TrimLeft(' ');	
	str_err_code.TrimRight(' ');
	// **************************************************************************
	
	// **************************************************************************
    // 발생한 알람 Code에 대한 배열 위치 검색한다                                
    // **************************************************************************
	for(n_cnt=0; n_cnt<2000; n_cnt++)
	{  
		if ( (st_alarm.mstr_e_code[n_cnt]).Compare(str_err_code) == 0 )
		{
			n_cur_num = n_cnt ; 
			break;
		}
	}
	// **************************************************************************
	
	if (n_cur_num == -1)  // 미존재 알람
	{
		alarm.mstr_pcode = alarm.mstr_code;
		alarm.mstr_code = "999999";
		str_err_code = "999999";
		
		for(n_cnt=0; n_cnt<2000; n_cnt++)
		{  
			if ( (st_alarm.mstr_e_code[n_cnt]).Compare(str_err_code) == 0 )
			{
				n_cur_num = n_cnt ; 
				break;
			}
		}
		// **************************************************************************
	}
	
	// **************************************************************************
    // 발생한 알람에 대한 상태와 메시지 전역 변수에 설정한다                     
    // **************************************************************************
	st_alarm.mn_cur_state = atoi(st_alarm.mstr_e_state[n_cur_num]);
	st_alarm.mstr_cur_msg = st_alarm.mstr_e_msg[n_cur_num];
	// **************************************************************************
}
// ******************************************************************************

//==================================================================//
// [로그 파일] 발생한 알람 정보 파일에 저장
//==================================================================//
void CMyJamData::OnSave_AlarmInfo2LogFile() 
{
	COleDateTime cur_time;
	CTime ctJamHappened;
	int nCurrHour;
	CString sTimeDisplay;
	CString sOccuredAlarmCode, sOccuredAlarmMsg;

	cur_time	= COleDateTime::GetCurrentTime();
	nCurrHour	= cur_time.GetHour();
	ctJamHappened	= CTime::GetCurrentTime();
	sTimeDisplay	= ctJamHappened.Format("%Y/%m/%d  %H:%M:%S");

	sOccuredAlarmCode	= alarm.mstr_code;
	sOccuredAlarmMsg	= st_alarm.mstr_cur_msg;
	
	st_alarmdb.mole_date		= COleDateTime::GetCurrentTime();
	st_alarmdb.mstrAlarmCode	= sOccuredAlarmCode;
	st_alarmdb.mstrAlarmMsg		= sOccuredAlarmMsg;

	OnAdd_2DailyFile(sOccuredAlarmCode, sTimeDisplay, sOccuredAlarmMsg);	// [일별] 알람 정보 파일에 저장
	Func.DB_Write_Alarm(st_alarmdb);

	alarm.stl_cur_alarm_time = GetCurrentTime();
	int nCheck;
	nCheck = atoi(sOccuredAlarmCode);

	long lCheckTime;
	if (alarm.stl_prev_code == nCheck)
	{
		lCheckTime = alarm.stl_cur_alarm_time - alarm.stl_alarm_start_time;
		if (lCheckTime < (st_basic.mn_alarm_delay_time)*1000)
		{
			alarm.stl_prev_alarm_time = alarm.stl_cur_alarm_time;
			return;
		}
	}
	else
	{
		alarm.stl_prev_code			= nCheck;
		alarm.stl_prev_alarm_time	= alarm.stl_cur_alarm_time;
	}

	if (nCurrHour >= 6 && nCurrHour < 14)	// SHIFT 중 DAY
	{

	}
	else if (nCurrHour >= 14 && nCurrHour < 22)	// SHIFT 중 SWING
	{

	}
	else	// SHIFT 중 GY
	{

	}
}

//==================================================================//
// [파일->구조체] 모든 알람 정보 로딩
//==================================================================//
void CMyJamData::OnLoad_AlarmInfoFronFile() 
{
	CString sLoadFile;
	sLoadFile  = BMP_PATH_BASE_;
	sLoadFile += "Error.txt";

	int nCount=0;
	for(nCount=0; nCount<2000; nCount++)
	{
		(st_alarm.mstr_e_content[nCount]).Empty();
		(st_alarm.mstr_e_code[nCount]).Empty();
		(st_alarm.mstr_e_state[nCount]).Empty();
		(st_alarm.mstr_e_part[nCount]).Empty();
		(st_alarm.mstr_e_msg[nCount]).Empty();
	}

	char chr_s[300];
	FILE *fp;
	if ((fp=fopen(sLoadFile, "rt")) == NULL)
	{
		Func.MsgLog("The failure because we open the file.");
		return;
	}

	CString sReadData, sComment, sTmpData;
	int nPos=0, nLen;
	// 파일의 마지막 위치까지 읽음
	while(!feof(fp))
	{
		sprintf(chr_s, "");

		if (fgets(chr_s, 300, fp) == NULL &&
			ferror(fp))
		{
			Func.MsgLog("The failure because we read the file.");
			return;
		}

		sReadData.Format("%s", chr_s);
		nLen = lstrlen(sReadData);
		// 빈줄 및 주석 부분 체크
		if (!sReadData.IsEmpty() && nLen > 2)
		{
			sComment = sReadData.Left(2);
			// 주석줄인지 확인
			if (sComment != "//")
			{
				st_alarm.mstr_e_content[nPos]	= sReadData;
				st_alarm.mstr_e_code[nPos]		= sReadData.Left(6);	// 알람 코드
				st_alarm.mstr_e_state[nPos]		= sReadData.Mid(7,1);	// 알람 상태 정보
				st_alarm.mstr_e_part[nPos]		= sReadData.Mid(9,1);	// 알람 발생 부위

				// 알람 메시지 설정
				sTmpData = sReadData.Mid(12,270);
				
				if (st_alarm.mstr_e_code[nPos] != "999999")
				{
					// 공백을 제거한 알람 메시지 설정
					st_alarm.mstr_e_msg[nPos] = sTmpData.Left(sTmpData.GetLength() -3);
				}
				else
				{
					st_alarm.mstr_e_msg[nPos] = sTmpData.Left(sTmpData.GetLength() -1);
				}

				nPos++;
			}
		}
	}

	fclose(fp);	// 파일 종료
}

//==================================================================//
// 발생한 알람 정보를 구조체에 할당
//==================================================================//
void CMyJamData::OnSet_Info2StrctAlarm(CString szJamCode) 
{
	CString sErrorCode = szJamCode;
	sErrorCode.TrimLeft(' ');
	sErrorCode.TrimRight(' ');

	int nCount=0;
	int nArrayNum = -1;	// 일치하는 알람의 배열 위치 정보
	COleDateTime timeCurr;
	CTime ctJamHappened;
	int nCurrYear, nCurrMonth, nCurrDay;
	CString sBackUpFile, sTimeDisplay;
	CString sCreateFileName, sContent;
	int nExistence;

	char fileName[256];
	FILE *fp;

	// 발생한 알람을 찾음
	for(nCount=0; nCount<2000; nCount++)
	{
		if ((st_alarm.mstr_e_code[nCount]).Compare(sErrorCode) == 0)
		{
			// 찾은 알람의 배열 위치 설정
			nArrayNum = nCount;
			break;
		}
	}

	// 발생한 알람이 존재하지 않음
	// - 파일에 해당 정보를 남겨 추후에 추가할 수 있도록 함
	if (nArrayNum == -1)
	{
		timeCurr	= COleDateTime::GetCurrentTime();
		nCurrYear	= timeCurr.GetYear();
		nCurrMonth	= timeCurr.GetMonth();
		nCurrDay	= timeCurr.GetDay();
		
		ctJamHappened	= CTime::GetCurrentTime();
		sTimeDisplay	= ctJamHappened.Format("%Y/%m/%d  %H:%M:%S");
		// 생성할 파일명을 만듬
		sBackUpFile.Format("ACD%04d%02d%02d", nCurrYear, nCurrMonth, nCurrDay);

		// 생성할 파일에 대한 폴더 정보 설정
		sCreateFileName  = "D:\\AMTLOG\\AlarmCodeDebug\\" + sBackUpFile;
		sCreateFileName += ".TXT";
		sprintf(fileName, "%s", sCreateFileName);
		nExistence = access(fileName, 0);
		// 파일 미존재함
		if (nExistence == -1)
		{
			sCreateFileName  = "D:\\AMTLOG\\AlarmCodeDebug\\" + sBackUpFile;
			sCreateFileName += ".TXT";
		}

		// 파일 열기 실패한 경우에는 강제 리턴시킴
		if ((fp=fopen(sCreateFileName, "a+")) == NULL)
		{
			return;
		}

		sContent = "Alarm code is not found : " + sErrorCode;
		fprintf(fp, "%s\n", sContent);
		sContent = "time : " + sTimeDisplay;
		fprintf(fp, "%s\n", sContent);
		sContent = "------------------------------------------------------------------";
		fprintf(fp, "%s\n", sContent);

		if (ferror(fp))
		{
			Func.MsgLog("File Open Fail!.");
			return;
		}
		fclose(fp);	// 파일 종료

		alarm.mstr_pcode	= alarm.mstr_code;
		alarm.mstr_code		= "999999";
		sprintf(alarm.mc_code, "999999");
		sErrorCode = "999999";

		for(nCount=0; nCount<2000; nCount++)
		{
			if ((st_alarm.mstr_e_code[nCount]).Compare(sErrorCode) == 0)
			{
				nArrayNum = nCount;
				break;
			}
		}
	}

	st_alarm.mstr_cur_state	= st_alarm.mstr_e_state[nArrayNum];
	st_alarm.mstr_cur_msg	= st_alarm.mstr_e_msg[nArrayNum];
}

//==================================================================//
// [일별] 알람 정보 파일에 저장
//==================================================================//
void CMyJamData::OnAdd_2DailyFile(CString szJamCode, CString szTimeDisp, CString sJamMsg) 
{
	COleDateTime timeCurr;
	int nCurrYear, nCurrMonth, nCurrDay;

	timeCurr	= COleDateTime::GetCurrentTime();
	nCurrYear	= timeCurr.GetYear();
	nCurrMonth	= timeCurr.GetMonth();
	nCurrDay	= timeCurr.GetDay();

	CString sBackUpFile;
	sBackUpFile.Format("DY%04d%02d%02d", nCurrYear, nCurrMonth, nCurrDay);

	// 알람 코드 미존재함
	if (szJamCode.IsEmpty() == TRUE)
	{
		return;
	}

	CString sCreateFileName;
	sCreateFileName  = CONFIG_PATH_BASE_ + "\\ALARM\\Daily\\" + sBackUpFile;
	sCreateFileName += ".TXT";

	char fileName[256];
	FILE *fp;
	sprintf(fileName, "%s", sCreateFileName);
	int nExistence = access(fileName, 0);
	if (nExistence == -1)
	{
		CString strDRVName, strFolder;
		
		int  nDrive, nPer;
		
		strDRVName = sCreateFileName.Left(3);
		nDrive = strDRVName[0] - 'A' + 1;
		_chdrive(nDrive);
		
		for(nPer=1; nPer <= 20; nPer++)
		{
			_chdir("..");
		}
		
		sCreateFileName = sCreateFileName.Mid(3);
		while( !sCreateFileName.IsEmpty() )
		{
			nPer = sCreateFileName.Find('\\');
			if (nPer == -1)
			{
				break;
			}
			
			strFolder = sCreateFileName.Left(nPer);
			if ( _chdir(strFolder) )
			{
				_mkdir(strFolder);
				_chdir(strFolder);
			}
			
		sCreateFileName = sCreateFileName.Mid(nPer+1);
		}
	}

	if ((fp=fopen(sCreateFileName, "a+")) == NULL)
	{
		Func.MsgLog("File Open Fail!.");
		return;
	}

	CString sContent;
	//sContent = "--------------------------------------------------------";
	//fprintf(fp, "%s\n", sContent);

	(st_alarm.mn_e_list)++;

	CString sTempData;
	sTempData.Format("| %4d |", st_alarm.mn_e_list);
	sContent = sTempData;
	sTempData.Format(" %6s |", szJamCode);
	sContent += sTempData;
	sTempData.Format(" %-44s |", sJamMsg);
	sContent += sTempData;
	sTempData.Format("          |     |");
	sContent += sTempData;
	sTempData.Format(" %17s |", szTimeDisp);
	sContent += sTempData;
	fprintf(fp, "%s\n", sContent);

	sContent = "+------------------------------------------------------------------------------------------------------+";
	fprintf(fp, "%s\n", sContent);

	if (ferror(fp))
	{
		Func.MsgLog("File Open Fail!.");
		return;
	}

	fclose(fp);	// 파일 종료
}
