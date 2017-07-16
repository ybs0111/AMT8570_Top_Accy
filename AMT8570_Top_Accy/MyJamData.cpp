// MyJamData.cpp: implementation of the CMyJamData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Handler.h"
#include "MyJamData.h"

#include <direct.h>
#include "io.h" 
#include "Public_Function.h"  // ���� ���� �� I/O ���� ���� Ŭ����
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
// �߻��� �˶��� ���� ��� ���� ����
//==================================================================//
CString CMyJamData::OnLoad_AlarmDispInfo(CString szPath, CString szDataFile, CString szActFile, CString szCode) 
{
	CString sPathInfo = szPath;
	if (sPathInfo.Right(1) != "\\")
	{
		sPathInfo += "\\";
	}

	CString sRetVal = "";
	sRetVal = OnSearch_AlarmBmpInfo(sPathInfo, szDataFile, szCode);			// �߻��� �˶��� ���� ��Ʈ�� �̹��� ���� ã��
	int nPos = sRetVal.Find("Trouble");
	if (nPos >= 0)	// ���� �߻�
	{
		return sRetVal;
	}

	sRetVal = OnSearch_AlarmDispStyleInfo(sPathInfo, szDataFile, szCode);	// �߻��� �˶��� ���� ȭ�� ��� ��Ÿ�� ���� ã��
	nPos = sRetVal.Find("Trouble");
	if (nPos >= 0)	// ���� �߻�
	{
		return sRetVal;
	}

	sRetVal = OnSearch_AlarmRepairInfo(sPathInfo, szActFile, szCode);	// �߻��� �˶��� ���� ��ġ ���� ���� ã��
	nPos = sRetVal.Find("Trouble");
	if (nPos >= 0)	// ���� �߻�
	{
		return sRetVal;
	}

	return sRetVal;
}

//==================================================================//
// �߻��� �˶��� ���� ��Ʈ�� �̹��� ���� ã��
//==================================================================//
CString CMyJamData::OnSearch_AlarmBmpInfo(CString szPath, CString szDataFile, CString szCode) 
{
	CString sReturnVal;
	sReturnVal = "OK";

	CString sBmpFile = szPath + szDataFile;	// [����+���ϸ�]

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
// �߻��� �˶��� ���� ȭ�� ��� ��Ÿ�� ���� ã��
//==================================================================//
CString CMyJamData::OnSearch_AlarmDispStyleInfo(CString szPath, CString szDataFile, CString szCode) 
{
	CString sReturnVal;
	sReturnVal = "OK";

	CString sCheckFile = szPath + szDataFile;	// [����+���ϸ�]

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
	
	// �˶� ȭ�� ��� ��� ����
	sTemp1.Format("%s", chBuff);

	for(i=0; i<max; i++)
	{
		end = sTemp1.Find(';', start);
		// ������ ������ ������ ������
		if (end == -1)
		{
			break;
		}
		// ������ ������ ������ ����
		else
		{
			// ������ ������ �Ѱ� ������ ����
			sTemp2 = sTemp1.Mid(start, end-start);
			sTemp2.TrimLeft(' ');
			sTemp2.TrimRight(' ');

			if (i == 0)
			{
				st_alarm.mstr_style = sTemp2;	// �˶� ȭ�� ��� ���� ����
				if (st_alarm.mstr_style == "CIRCLE")
				{
					// �˶� ǥ�� ������ ���̸� �ʿ��� �������� ������ 8����
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
					st_alarm.mn_Cx = atoi(num);	// X�� �ȼ� ��ġ
				}
				else if (i == 2)
				{
					st_alarm.mn_Cy = atoi(num);	// Y�� �ȼ� ��ġ
				}
				else if (i == 3)
				{
					st_alarm.mn_Offset = atoi(num);	// ��ũ��
				}
				else if (i == 4)
				{
					st_alarm.mn_LineWidth = atoi(num);	// ǥ�� ���� �β�
				}
				else if (i == 5)
				{
					st_alarm.mn_R = atoi(num);	// RGB �� �� R��
				}
				else if (i == 6)
				{
					st_alarm.mn_G = atoi(num);	// RGB �� �� G��
				}
				else if (i == 7)
				{
					st_alarm.mn_B = atoi(num);	// RGB �� �� B��
				}
			}
			start = end +1;	// ���� ��ġ�� �̵�
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
// �߻��� �˶��� ���� ��ġ ���� ���� ã��
//==================================================================//
CString CMyJamData::OnSearch_AlarmRepairInfo(CString szPath, CString szActFile, CString szCode) 
{
	CString sRetVal;
	sRetVal = "OK";

	CString sRepairFile = szPath + szActFile;	// [����+���ϸ�]
	char chBuff[1000];

	if (!GetPrivateProfileString(szCode, "COUNT", "0", chBuff, 10, sRepairFile))
	{
		sRetVal  = "File Name : " + sRepairFile;
		sRetVal += "\r\n";
		sRetVal += "[COUNT] The information we read. Trouble -> Code : ";
		sRetVal += szCode;

		return sRetVal;
	}

	// �о���� ��ġ ���� ���� ����
	int nReadRepairCnt = atoi(chBuff);
	// ��ġ ������ ���� ���
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
	// ��ġ ���� ���� Ŭ����
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

		// ������ ��ġ���� ������ �ƴϸ� �ٹٲ� �߰�
		if (i != ((st_alarm.mn_action_cnt) -1))
		{
			st_alarm.mstr_repair_msg += "\r\n";
		}
	}
	return sRetVal;
}

//==================================================================//
// �߻��� �˶� ȭ�� ���
//==================================================================//
void CMyJamData::On_Alarm_Display() 
{
	// ���̸� �߰� [2014.09.01]
	// �ʱ�ȭ �۾��� Ÿ�̸� ���� �ĺ��� �˶� �����ϵ��� ������
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
		// �˶��� �߻��߾ �̹� �˶� ȭ���� ��µǾ� ������ ����
		// - ���� �߻��� �˶����� �������� �ʾƵ� �Ǳ� ����
		if (st_handler.nShowAlarmDlg == TRUE)
		{
			return;
		}

		if (st_handler.nSysLock != FALSE)
		{
			return;	// ���� ����
		}

		// �˶� �߻� �ð� ���� ����
		cur_time		= COleDateTime::GetCurrentTime();
		nCurrHour		= cur_time.GetHour();
		ctJamHappened	= CTime::GetCurrentTime();
		sTimeDisplay	= ctJamHappened.Format("%c");

		// �˶� ȭ���� ��µ��� ���� ��쿡�� �˶� ȭ�� �����
		if (st_handler.nShowAlarmDlg != TRUE)
		{
			alarm.nReset_AlarmStatus = CTL_YES;	// �˶� ���� ���� �ʿ��ϴٰ� ����

			// �߻��� �˶��� ����Ʈ�� �߰���
			if (alarm.mstr_code != "")
			{
				//2015.0604
				st_basic.mn_alarm_delay_time = 120;//(2min)
				// ���� �߻��� �˶��� ����
				st_handler.mstr_last_alarm = alarm.mstr_code;
				OnSet_Info2StrctAlarm(alarm.mstr_code);	// �߻��� �˶� ������ ����ü�� �Ҵ�

				sprintf(chTempCode, "%s", alarm.mstr_code);
				sCount.Format("%d", st_alarm.mn_cur_state);
				sprintf(chTempCount, "%s", sCount);

				alarm.stl_cur_alarm_time = GetCurrentTime();

				nCheck = atoi(alarm.mstr_code);
				nIsDataSave = CTL_NO;

				// �ٷ� ���� �߻��� �˶� �ڵ�� ��
				// - ������ �˶��� ���ؼ��� ���� �ð� ����� �Ŀ� �߻��� �˶��� ó����
				if (alarm.stl_prev_code == nCheck)
				{
					lCheckTime = alarm.stl_cur_alarm_time - alarm.stl_alarm_start_time;
					if (lCheckTime < (st_basic.mn_alarm_delay_time)*1000 ||
						alarm.stl_alarm_start_time == 0)
					{
						alarm.stl_prev_alarm_time = alarm.stl_cur_alarm_time;
						// ���̸� �߰� [2014.09.01]
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
					OnSave_AlarmInfo2LogFile();	// [�α� ����] �߻��� �˶� ���� ���Ͽ� ����
				}
			}

			// �˶� ȭ�� ����� �Ŷ�� �̸� �÷��� ����
			st_handler.nShowAlarmDlg = TRUE;

			// ���� �߰� 2014.09.23	alarm.mstr_code		st_alarm.mstr_cur_msg
			// �˶� �߻��� Main���� �˶� �ڵ�� �˶� Ÿ��Ʋ ����
			g_client_accy.OnCreate_SendFormat(MAIN_ALARM);
			g_client_accy.OnReq_CommStart();

			Func.OnSet_EqpStatusAlarm(st_alarm.mn_cur_state);	// [ALARM] ���� ���� ����
			// ���� �����ӿ� �˶� ȭ�� ��� ��û
			::PostMessage(st_handler.hWnd, WM_FORM_CHANGE, 7, 1);

			// ���̸� �ּ� ó�� [2014.09.01]
			// �����忡�� ��ü������ ����ϵ��� ������
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
	int n_cur_num = -1;    // �˶��� ���� �迭 ��ġ ���� ����
	CString str_err_code;  // �˶� Code ���� ����
	
	// **************************************************************************
    // �Էµ� �˶� Code���� ��/�� ������ �����Ѵ�                                
    // **************************************************************************
	str_err_code = strErrCode;
	str_err_code.TrimLeft(' ');	
	str_err_code.TrimRight(' ');
	// **************************************************************************
	
	// **************************************************************************
    // �߻��� �˶� Code�� ���� �迭 ��ġ �˻��Ѵ�                                
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
	
	if (n_cur_num == -1)  // ������ �˶�
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
    // �߻��� �˶��� ���� ���¿� �޽��� ���� ������ �����Ѵ�                     
    // **************************************************************************
	st_alarm.mn_cur_state = atoi(st_alarm.mstr_e_state[n_cur_num]);
	st_alarm.mstr_cur_msg = st_alarm.mstr_e_msg[n_cur_num];
	// **************************************************************************
}
// ******************************************************************************

//==================================================================//
// [�α� ����] �߻��� �˶� ���� ���Ͽ� ����
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

	OnAdd_2DailyFile(sOccuredAlarmCode, sTimeDisplay, sOccuredAlarmMsg);	// [�Ϻ�] �˶� ���� ���Ͽ� ����
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

	if (nCurrHour >= 6 && nCurrHour < 14)	// SHIFT �� DAY
	{

	}
	else if (nCurrHour >= 14 && nCurrHour < 22)	// SHIFT �� SWING
	{

	}
	else	// SHIFT �� GY
	{

	}
}

//==================================================================//
// [����->����ü] ��� �˶� ���� �ε�
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
	// ������ ������ ��ġ���� ����
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
		// ���� �� �ּ� �κ� üũ
		if (!sReadData.IsEmpty() && nLen > 2)
		{
			sComment = sReadData.Left(2);
			// �ּ������� Ȯ��
			if (sComment != "//")
			{
				st_alarm.mstr_e_content[nPos]	= sReadData;
				st_alarm.mstr_e_code[nPos]		= sReadData.Left(6);	// �˶� �ڵ�
				st_alarm.mstr_e_state[nPos]		= sReadData.Mid(7,1);	// �˶� ���� ����
				st_alarm.mstr_e_part[nPos]		= sReadData.Mid(9,1);	// �˶� �߻� ����

				// �˶� �޽��� ����
				sTmpData = sReadData.Mid(12,270);
				
				if (st_alarm.mstr_e_code[nPos] != "999999")
				{
					// ������ ������ �˶� �޽��� ����
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

	fclose(fp);	// ���� ����
}

//==================================================================//
// �߻��� �˶� ������ ����ü�� �Ҵ�
//==================================================================//
void CMyJamData::OnSet_Info2StrctAlarm(CString szJamCode) 
{
	CString sErrorCode = szJamCode;
	sErrorCode.TrimLeft(' ');
	sErrorCode.TrimRight(' ');

	int nCount=0;
	int nArrayNum = -1;	// ��ġ�ϴ� �˶��� �迭 ��ġ ����
	COleDateTime timeCurr;
	CTime ctJamHappened;
	int nCurrYear, nCurrMonth, nCurrDay;
	CString sBackUpFile, sTimeDisplay;
	CString sCreateFileName, sContent;
	int nExistence;

	char fileName[256];
	FILE *fp;

	// �߻��� �˶��� ã��
	for(nCount=0; nCount<2000; nCount++)
	{
		if ((st_alarm.mstr_e_code[nCount]).Compare(sErrorCode) == 0)
		{
			// ã�� �˶��� �迭 ��ġ ����
			nArrayNum = nCount;
			break;
		}
	}

	// �߻��� �˶��� �������� ����
	// - ���Ͽ� �ش� ������ ���� ���Ŀ� �߰��� �� �ֵ��� ��
	if (nArrayNum == -1)
	{
		timeCurr	= COleDateTime::GetCurrentTime();
		nCurrYear	= timeCurr.GetYear();
		nCurrMonth	= timeCurr.GetMonth();
		nCurrDay	= timeCurr.GetDay();
		
		ctJamHappened	= CTime::GetCurrentTime();
		sTimeDisplay	= ctJamHappened.Format("%Y/%m/%d  %H:%M:%S");
		// ������ ���ϸ��� ����
		sBackUpFile.Format("ACD%04d%02d%02d", nCurrYear, nCurrMonth, nCurrDay);

		// ������ ���Ͽ� ���� ���� ���� ����
		sCreateFileName  = "D:\\AMTLOG\\AlarmCodeDebug\\" + sBackUpFile;
		sCreateFileName += ".TXT";
		sprintf(fileName, "%s", sCreateFileName);
		nExistence = access(fileName, 0);
		// ���� ��������
		if (nExistence == -1)
		{
			sCreateFileName  = "D:\\AMTLOG\\AlarmCodeDebug\\" + sBackUpFile;
			sCreateFileName += ".TXT";
		}

		// ���� ���� ������ ��쿡�� ���� ���Ͻ�Ŵ
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
		fclose(fp);	// ���� ����

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
// [�Ϻ�] �˶� ���� ���Ͽ� ����
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

	// �˶� �ڵ� ��������
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

	fclose(fp);	// ���� ����
}
