// MySqlAlg.cpp: implementation of the MySqlAlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Handler.h"
#include "Alg_Mysql.h"
#include "Public_Function.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CAlg_Mysql Mysql;

CAlg_Mysql::CAlg_Mysql()
{
	// Mysql DB 정의 
	mn_lot_table_cnt		= 8;
	mstr_lot_table_name[0]	= "no";
	mstr_lot_table_name[1]	= "LOT_NO";
	mstr_lot_table_name[2]	= "START_DATE";
	mstr_lot_table_name[3]	= "END_DATE";
	mstr_lot_table_name[4]	= "TOTAL_CNT";
	mstr_lot_table_name[5]	= "PASS_CNT";
	mstr_lot_table_name[6]	= "FAIL_CNT";
	mstr_lot_table_name[7]	= "YIELD";
	
	mn_lot_table_num[0]		= 5;
	mn_lot_table_num[1]		= 25;
	mn_lot_table_num[2]		= 20;
	mn_lot_table_num[3]		= 20;
	mn_lot_table_num[4]		= 6;
	mn_lot_table_num[5]		= 6;
	mn_lot_table_num[6]		= 6;
	mn_lot_table_num[7]		= 6;
	
	mn_lot_table_type[0]	= ID_TYPE;
	mn_lot_table_type[1]	= TEXT_TYPE;
	mn_lot_table_type[2]	= DATE_TYPE;
	mn_lot_table_type[3]	= DATE_TYPE;
	mn_lot_table_type[4]	= INT_TYPE;
	mn_lot_table_type[5]	= INT_TYPE;
	mn_lot_table_type[6]	= INT_TYPE; 
	mn_lot_table_type[7]	= DOUBLE_TYPE;

	mn_ground_table_cnt		= 7;
	mstr_ground_table_name[0]	= "no";
	mstr_ground_table_name[1]	= "DATE";
	mstr_ground_table_name[2]	= "CHANNEL1";
	mstr_ground_table_name[3]	= "CHANNEL2";
	mstr_ground_table_name[4]	= "CHANNEL3";
	mstr_ground_table_name[5]	= "CHANNEL4";
	mstr_ground_table_name[6]	= "CHANNEL5";
	
	mn_ground_table_num[0]		= 5;
	mn_ground_table_num[1]		= 20;
	mn_ground_table_num[2]		= 6;
	mn_ground_table_num[3]		= 6;
	mn_ground_table_num[4]		= 6;
	mn_ground_table_num[5]		= 6;
	mn_ground_table_num[6]		= 6;	
	
	mn_ground_table_type[0]	= ID_TYPE;
	mn_ground_table_type[1]	= DATE_TYPE;
	mn_ground_table_type[2]	= TEXT_TYPE;
	mn_ground_table_type[3]	= TEXT_TYPE;
	mn_ground_table_type[4]	= TEXT_TYPE;
	mn_ground_table_type[5]	= TEXT_TYPE;
	mn_ground_table_type[6]	= TEXT_TYPE; 

	mn_alarm_table_cnt		= 6;
	mstr_alarm_table_name[0]	= "no";
	mstr_alarm_table_name[1]	= "DATE";
	mstr_alarm_table_name[2]	= "LOT_NO";					// LOT NO
	mstr_alarm_table_name[3]	= "PART_NO";				// PART NO
	mstr_alarm_table_name[4]	= "ALARM_CODE";
	mstr_alarm_table_name[5]	= "ALARM_MSG";
	
	mn_alarm_table_num[0]		= 5;
	mn_alarm_table_num[1]		= 20;
	mn_alarm_table_num[2]		= 20;
	mn_alarm_table_num[3]		= 25;
	mn_alarm_table_num[4]		= 8;
	mn_alarm_table_num[5]		= 100;
		
	mn_alarm_table_type[0]	= ID_TYPE;
	mn_alarm_table_type[1]	= DATE_TYPE;
	mn_alarm_table_type[2]	= TEXT_TYPE;
	mn_alarm_table_type[3]	= TEXT_TYPE;
	mn_alarm_table_type[4]	= TEXT_TYPE;
	mn_alarm_table_type[5]	= TEXT_TYPE;

	mn_time_table_cnt		= 7;
	mstr_time_table_name[0]	= "no";
	mstr_time_table_name[1]	= "DATE";
	mstr_time_table_name[2]	= "LOT_NO";					// LOT NO
	mstr_time_table_name[3]	= "PART_NO";				// PART NO
	mstr_time_table_name[4]	= "TIME_SECTION";
	mstr_time_table_name[5]	= "TIME_TIME";
	mstr_time_table_name[6]	= "MODE";
	
	mn_time_table_num[0]		= 5;
	mn_time_table_num[1]		= 20;
	mn_time_table_num[2]		= 20;
	mn_time_table_num[3]		= 25;
	mn_time_table_num[4]		= 8;
	mn_time_table_num[5]		= 20;
	mn_time_table_num[6]		= 8;
	
	mn_time_table_type[0]	= ID_TYPE;
	mn_time_table_type[1]	= DATE_TYPE;
	mn_time_table_type[2]	= TEXT_TYPE;
	mn_time_table_type[3]	= TEXT_TYPE;
	mn_time_table_type[4]	= INT_TYPE;
	mn_time_table_type[5]	= INT_TYPE;
	mn_time_table_type[6]	= INT_TYPE;

	mn_buffer_table_cnt		= 9;
	mstr_buffer_table_name[0]	= "no";
	mstr_buffer_table_name[1]	= "DATE";
	mstr_buffer_table_name[2]	= "LOT_NO";					// LOT NO
	mstr_buffer_table_name[3]	= "PART_NO";				// PART NO
	mstr_buffer_table_name[4]	= "BUFFER_POS";
	mstr_buffer_table_name[5]	= "LAST_MODULE";
	mstr_buffer_table_name[6]	= "BYPASS";
	mstr_buffer_table_name[7]	= "MODULE_CNT";
	mstr_buffer_table_name[8]	= "RETEST";
	
	mn_buffer_table_num[0]		= 5;
	mn_buffer_table_num[1]		= 20;
	mn_buffer_table_num[2]		= 20;
	mn_buffer_table_num[3]		= 20;
	mn_buffer_table_num[4]		= 2;
	mn_buffer_table_num[5]		= 2;
	mn_buffer_table_num[6]		= 2;
	mn_buffer_table_num[7]		= 2;
	mn_buffer_table_num[8]		= 2;
	
	mn_buffer_table_type[0]	= ID_TYPE;
	mn_buffer_table_type[1]	= DATE_TYPE;
	mn_buffer_table_type[2]	= TEXT_TYPE;
	mn_buffer_table_type[3]	= TEXT_TYPE;
	mn_buffer_table_type[4]	= INT_TYPE;
	mn_buffer_table_type[5]	= INT_TYPE;
	mn_buffer_table_type[6]	= INT_TYPE;
	mn_buffer_table_type[7]	= INT_TYPE;
	mn_buffer_table_type[8]	= INT_TYPE;

	mn_scrap_table_cnt		= 6;
	mstr_scrap_table_name[0]	= "no";
	mstr_scrap_table_name[1]	= "DATE";
	mstr_scrap_table_name[2]	= "LOT_NO";					// LOT NO
	mstr_scrap_table_name[3]	= "PART_NO";				// PART NO
	mstr_scrap_table_name[4]	= "SERIAL_NO";
	mstr_scrap_table_name[5]	= "SCRAP_CODE";
	
	mn_scrap_table_num[0]		= 5;
	mn_scrap_table_num[1]		= 20;
	mn_scrap_table_num[2]		= 20;
	mn_scrap_table_num[3]		= 25;
	mn_scrap_table_num[4]		= 20;
	mn_scrap_table_num[5]		= 20;
	
	mn_scrap_table_type[0]	= ID_TYPE;
	mn_scrap_table_type[1]	= DATE_TYPE;
	mn_scrap_table_type[2]	= TEXT_TYPE;
	mn_scrap_table_type[3]	= TEXT_TYPE;
	mn_scrap_table_type[4]	= TEXT_TYPE;
	mn_scrap_table_type[5]	= TEXT_TYPE;
}

CAlg_Mysql::~CAlg_Mysql()
{

}

BOOL CAlg_Mysql::MySql_Open(CString hostname, CString username,  CString password, int port)
{

	mysql_init(&m_pMySql);
	
	if(!(mysql_real_connect(&m_pMySql, hostname, username, password, NULL,port,NULL,NULL)))
	{
		Func.MsgLog(mysql_error(&m_pMySql));
		return FALSE;
	}

	// 한글 지원하기 위해
	if(mysql_query(&m_pMySql, "SET NAMES euckr"))
	{
		Func.MsgLog(mysql_error(&m_pMySql));
		return FALSE;
	}
	
	return TRUE;
}

BOOL CAlg_Mysql::Database_Create(CString databasename)
{
	CString strQuery;

	strQuery = "CREATE DATABASE " + databasename;

	if(mysql_query(&m_pMySql, strQuery))
	{
		Func.MsgLog(mysql_error(&m_pMySql));
		return FALSE;
	}

	return TRUE;
}

BOOL CAlg_Mysql::Database_Delete(CString databasename)
{
	CString strQuery;
	
	strQuery = "DROP DATABASE " + databasename;
	
	if(mysql_query(&m_pMySql, strQuery))
	{
		Func.MsgLog(mysql_error(&m_pMySql));
		return FALSE;
	}
	
	return TRUE;
}

/*
TYNYINT_TYPE			0
SMALLINT_TYPE			1
MEDIUMINT_TYPE			2
INT_TYPE				3
BIGINT_TYPE				4
FLOAT_TYPE				5
DOUBLE_TYPE				6
DATE_TYPE				7
DATETIME_TYPE			8
TIMESTAMP_TYPE			9 
TIME_TYPE				10
YEAR_TYPE				11
CHAR_TYPE				12
VARCHAR_TYPE			13
TINYBLOB_TYPE			14
TINYTEXT_TYPE			15
BLOB_TYPE				16
TEXT_TYPE				17
MEDIUMBLOB_TYPE			18
MEDIUMTEXT_TYPE			19
LONGBLOB_TYPE			20
LONGTEXT_TYPE			21
NUMBER_TYEP				22
*/

BOOL CAlg_Mysql::Table_Create(CString database, CString tablename, int itemnum, CString *itemname, int *itemtype, int *itemhundred)
{
	CString strQuery;
	CString strItem;
	CString strTmp;
	int i;

	strQuery = "USE " + database;

	if(mysql_query(&m_pMySql, strQuery))
	{
		Func.MsgLog(mysql_error(&m_pMySql));
		return FALSE;
	}


	for(i=0; i<itemnum; i++)
	{
		switch(itemtype[i])
		{
			case ID_TYPE:
  				strTmp.Format("%s INT PRIMARY KEY NOT NULL AUTO_INCREMENT", itemname[i]);
				strItem += strTmp;
				break;
			case INT_TYPE:
				strTmp.Format("%s INT", itemname[i]);
				strItem += strTmp;
				break;
			case FLOAT_TYPE:
				strTmp.Format("%s FLOAT", itemname[i]);
				strItem += strTmp;
				break;
			case DOUBLE_TYPE:
				strTmp.Format("%s DOUBLE", itemname[i]);
				strItem += strTmp;
				break;
			case DATE_TYPE:
				strTmp.Format("%s DATETIME", itemname[i]);
				strItem += strTmp;
				break;
			case DATEAUTO_TYPE:
				strTmp.Format("%s IMESTAMP", itemname[i]);
				strItem += strTmp;
				break;
			case TEXT_TYPE:
				strTmp.Format("%s VARCHAR(%d)", itemname[i], itemhundred[i]);
				strItem += strTmp;
				break;	
		}

		if((itemnum - 1) > i)
		{
			strItem += ",";
		}
	}
	strQuery.Format("CREATE TABLE %s(%s)", tablename, strItem);

//	Func.MsgLog(strItem);

	if(mysql_query(&m_pMySql, strQuery))
	{
		strTmp = mysql_error(&m_pMySql);

		if(strTmp == "Table 'automation_machine' already exists")
		{
			return TRUE;
		}
		else{
			Func.MsgLog(mysql_error(&m_pMySql));
			return FALSE;
		}
	}

	return TRUE;
}

BOOL CAlg_Mysql::Table_Delete(CString tablename)
{
	CString strQuery;
	
	strQuery = "DROP TABLE " + tablename;
	
	if(mysql_query(&m_pMySql, strQuery))
	{
		Func.MsgLog(mysql_error(&m_pMySql));
		return FALSE;
	}

	return TRUE;
}

BOOL CAlg_Mysql::Record_Add(CString tablename, CString *itemdata)
{
	MYSQL_RES *Res;
	MYSQL_ROW Row;
	MYSQL_FIELD *Fields;

	CString strValue;
	CString strItem;
	CString strTmp;
	CString strQuery;
	CString strType;
	CString strLog;

	int i;
	int pos;
	int num;

//	Func.On_LogFile_Add(99, "[DB_Write_] : 5.Record_Add() ");
	strQuery.Format("DESCRIBE %s", tablename);
	
//	strLog.Format( "[DB_Write_] : Query : %s", strQuery );
//	Func.On_LogFile_Add(99, strLog );
	if(mysql_query(&m_pMySql, strQuery))
	{
// 		strLog.Format("[DB_Write_] : 6-1.query Fail : %s", strQuery);
// 		Func.On_LogFile_Add(99, strLog );
		Func.MsgLog(mysql_error(&m_pMySql));
		return FALSE;
	}
// 	strLog.Format("[DB_Write_] : 6.query Success : %s ", strQuery);
// 	Func.On_LogFile_Add(99, strLog);
	
	Res = mysql_store_result(&m_pMySql);
	num = (int)mysql_num_rows(Res);
	Fields = mysql_fetch_fields(Res);

	for(i=0; i<num; i++)
	{
		Row = mysql_fetch_row(Res);
		if(itemdata[i] != "")
		{
			strItem += Row[0];
			strTmp = Row[1];
			pos = strTmp.Find("(", 0);
			if(pos < 0)
			{
				pos = strTmp.GetLength();
			}
			strType = strTmp.Mid(0, pos);
			strType.MakeUpper();

			strTmp = "";
			if(strType == "INT")
			{
				strTmp += itemdata[i];
			}
			else if(strType == "FLOAT")
			{
				strTmp += itemdata[i];
			}
			else if(strType == "DOUBLE")
			{
				strTmp += itemdata[i];
			}
			else if(strType == "DATETIME")
			{
				strTmp.Format("'%s'", itemdata[i]);
			}
			else if(strType == "TIMESTAMP")
			{
				
			}
			else if(strType == "VARCHAR")
			{
				strTmp.Format("'%s'", itemdata[i]);
			}
			else
			{
				strTmp += itemdata[i];
			}

			strValue += strTmp;
			
 			if((num - 1) > i)
			{
				strItem += ",";
				strValue += ",";
			}
		}
	}

	strQuery.Format("INSERT INTO %s(%s) VALUES(%s)", tablename, strItem, strValue);

	if(mysql_query(&m_pMySql, strQuery))
	{
// 		strLog.Format("[DB_Write_] : 7.query Fail : %s ", strQuery);
// 		Func.On_LogFile_Add(99, strLog);
		Func.MsgLog(mysql_error(&m_pMySql));
		return FALSE;
	}
// 	strLog.Format("[DB_Write_] : 7.query Success : %s ", strQuery);
// 	Func.On_LogFile_Add(99, strLog);
	
	mysql_free_result(Res);

//	Func.On_LogFile_Add(99, "[DB_Write_] : 8.Record_Add() Finish");
	return TRUE;
}

void CAlg_Mysql::MySql_Close()
{
	mysql_close(&m_pMySql);
}

BOOL CAlg_Mysql::Database_Select(CString databasename)
{
	if(mysql_select_db(&m_pMySql, databasename))
	{
		return FALSE;
	}

	return TRUE;
}


BOOL CAlg_Mysql::Record_Updata(CString tablename, CString data)
{
	CString strQuery;

	strQuery.Format("UPDATE %s SET %s", tablename, data);

	if(mysql_query(&m_pMySql, strQuery))
	{
		Func.MsgLog(mysql_error(&m_pMySql));
		return FALSE;
	}
	
	return TRUE;
}

BOOL CAlg_Mysql::Record_Int_Delete(CString tablename, CString itemname, CString range, int value[])
{
	CString strQuery;

	if(range == "<")
	{
		strQuery.Format("DELETE FROM %s WHERE %s < %d", tablename, itemname, value[0]);
	}
	else if(range == ">")
	{
		strQuery.Format("DELETE FROM %s WHERE %s > %d", tablename, itemname, value[0]);
	}
	else if(range == "<>")
	{
		strQuery.Format("DELETE FROM %s WHERE %s > %d AND %s < %d", tablename, itemname, value[0], itemname, value[1]);
	}
	else if(range == "><")
	{
		strQuery.Format("DELETE FROM %s WHERE %s < %d OR %s > %d", tablename, itemname, value[0], itemname, value[1]);
	}
	else
	{
		strQuery.Format("DELETE FROM %s WHERE %s = %d", tablename, itemname, value[0]);
	}
	

	if(mysql_query(&m_pMySql, strQuery))
	{
		Func.MsgLog(mysql_error(&m_pMySql));
		return FALSE;
	}
	return TRUE;
}

BOOL CAlg_Mysql::Record_Float_Delete(CString tablename, CString itemname, CString range, float value[])
{
	CString strQuery;
	
	if(range == "<")
	{
		strQuery.Format("DELETE FROM %s WHERE %s < %f", tablename, itemname, value[0]);
	}
	else if(range == ">")
	{
		strQuery.Format("DELETE FROM %s WHERE %s > %f", tablename, itemname, value[0]);
	}
	else if(range == "<>")
	{
		strQuery.Format("DELETE FROM %s WHERE %s > %f AND %s < %f", tablename, itemname, value[0], itemname, value[1]);
	}
	else if(range == "><")
	{
		strQuery.Format("DELETE FROM %s WHERE %s < %f OR %s > %f", tablename, itemname, value[0], itemname, value[1]);
	}
	else
	{
		strQuery.Format("DELETE FROM %s WHERE %s = %f", tablename, itemname, value[0]);
	}
	
	
	if(mysql_query(&m_pMySql, strQuery))
	{
		Func.MsgLog(mysql_error(&m_pMySql));
		return FALSE;
	}
	return TRUE;
}

BOOL CAlg_Mysql::Record_Double_Delete(CString tablename, CString itemname, CString range, double value[])
{
	CString strQuery;
	
	if(range == "<")
	{
		strQuery.Format("DELETE FROM %s WHERE %s < %f", tablename, itemname, value[0]);
	}
	else if(range == ">")
	{
		strQuery.Format("DELETE FROM %s WHERE %s > %f", tablename, itemname, value[0]);
	}
	else if(range == "<>")
	{
		strQuery.Format("DELETE FROM %s WHERE %s > %f AND %s < %f", tablename, itemname, value[0], itemname, value[1]);
	}
	else if(range == "><")
	{
		strQuery.Format("DELETE FROM %s WHERE %s < %f OR %s > %f", tablename, itemname, value[0], itemname, value[1]);
	}
	else
	{
		strQuery.Format("DELETE FROM %s WHERE %s = %f", tablename, itemname, value[0]);
	}
	
	
	if(mysql_query(&m_pMySql, strQuery))
	{
		Func.MsgLog(mysql_error(&m_pMySql));
		return FALSE;
	}
	return TRUE;
}

BOOL CAlg_Mysql::Record_Text_Delete(CString tablename, CString itemname, CString value)
{
	CString strQuery;

	strQuery.Format("DELETE FROM %s WHERE %s = '%s'", tablename, itemname, value);

	if(mysql_query(&m_pMySql, strQuery))
	{
		Func.MsgLog(mysql_error(&m_pMySql));
		return FALSE;
	}
	return TRUE;
}

BOOL CAlg_Mysql::Record_Date_Delete(CString tablename, CString itemname, CString range, COleDateTime time[])
{
	CString strQuery;
	CString strTmp[2];
	
	if(range == "<")
	{
		strTmp[0].Format("%04d/%02d/%02d %02d:%02d:%02d", time[0].GetYear(), 
														  time[0].GetMonth(),
														  time[0].GetDay(),
														  time[0].GetHour(),
														  time[0].GetMinute(),
														  time[0].GetSecond());
		strQuery.Format("DELETE FROM %s WHERE %s < '%s'", tablename, itemname, strTmp[0]);
	}
	else if(range == ">")
	{
		strTmp[0].Format("%04d/%02d/%02d %02d:%02d:%02d", time[0].GetYear(), 
														  time[0].GetMonth(),
														  time[0].GetDay(),
														  time[0].GetHour(),
														  time[0].GetMinute(),
														  time[0].GetSecond());
		strQuery.Format("DELETE FROM %s WHERE %s > '%s'", tablename, itemname, strTmp[0]);
	}
	else if(range == "<>")
	{
		strTmp[0].Format("%04d/%02d/%02d %02d:%02d:%02d", time[0].GetYear(), 
														  time[0].GetMonth(),
														  time[0].GetDay(),
														  time[0].GetHour(),
														  time[0].GetMinute(),
														  time[0].GetSecond());

		strTmp[1].Format("%04d/%02d/%02d %02d:%02d:%02d", time[1].GetYear(), 
														  time[1].GetMonth(),
														  time[1].GetDay(),
														  time[1].GetHour(),
														  time[1].GetMinute(),
														  time[1].GetSecond());
		strQuery.Format("DELETE FROM %s WHERE %s > '%s' AND %s < '%s'", tablename, itemname, strTmp[0], itemname, strTmp[1]);
	}
	else if(range == "><")
	{
		strTmp[0].Format("%04d/%02d/%02d %02d:%02d:%02d", time[0].GetYear(), 
														  time[0].GetMonth(),
														  time[0].GetDay(),
														  time[0].GetHour(),
														  time[0].GetMinute(),
														  time[0].GetSecond());

		strTmp[1].Format("%04d/%02d/%02d %02d:%02d:%02d", time[1].GetYear(), 
														  time[1].GetMonth(),
														  time[1].GetDay(),
														  time[1].GetHour(),
														  time[1].GetMinute(),
														  time[1].GetSecond());
		strQuery.Format("DELETE FROM %s WHERE %s < '%s' OR %s > '%s'", tablename, itemname, strTmp[0], itemname, strTmp[1]);
	}
	else
	{
		strTmp[0].Format("%04d/%02d/%02d %02d:%02d:%02d", time[0].GetYear(), 
														  time[0].GetMonth(),
														  time[0].GetDay(),
														  time[0].GetHour(),
														  time[0].GetMinute(),
														  time[0].GetSecond());
		strQuery.Format("DELETE FROM %s WHERE %s = '%s'", tablename, itemname, strTmp[0]);
	}

	if(mysql_query(&m_pMySql, strQuery))
	{
		Func.MsgLog(mysql_error(&m_pMySql));
		return FALSE;
	}
	return TRUE;
}

long CAlg_Mysql::Record_Int_Search(CString tablename, CString itemsearch, CString range, int value[])
{
	MYSQL_RES *Res;
	MYSQL_ROW Row;
	MYSQL_FIELD *Fields;

	CString strQuery;
	CString stritem;
	CString strTmp[2], strData;
	int i, j;
	int num;
	m_nfind_cnt = 0;

	stritem = "";

	strQuery.Format("DESCRIBE %s", tablename);
	
	if(mysql_query(&m_pMySql, strQuery))
	{
		Func.MsgLog(mysql_error(&m_pMySql));
		return 0;
	}
	
	Res = mysql_store_result(&m_pMySql);
	num = (int)mysql_num_rows(Res);
	Fields = mysql_fetch_fields(Res);
	
	for(i=0; i<num; i++)
	{
		Row = mysql_fetch_row(Res);
		
		stritem += Row[0];
		if((num-1) != i)
		{
			stritem += ",";
		}
	}
	//	SELECT name,age from guest where age <= 25 ;
	if(range == "<")
	{
		strQuery.Format("SELECT %s FROM %s WHERE %s > %d", stritem, tablename, itemsearch, value[0]);
	}
	else if(range == ">")
	{
		strQuery.Format("SELECT %s FROM %s WHERE %s < %d", stritem, tablename, itemsearch, value[0]);
	}
	else if(range == "<>")
	{
		strQuery.Format("SELECT %s FROM %s WHERE %s > %d AND %s < %d", stritem, tablename, itemsearch, value[0], itemsearch, value[1]);
	}
	else if(range == "><")
	{
		strQuery.Format("SELECT %s FROM %s WHERE %s < %d OR %s > %d", stritem, tablename, itemsearch, value[0], itemsearch, value[1]);
	}
	else
	{
		strQuery.Format("SELECT %s FROM %s WHERE %s = %d", stritem, tablename, itemsearch, value[0]);
	}

	mysql_free_result(Res);

	if(mysql_query(&m_pMySql, strQuery))
	{
		Func.MsgLog(mysql_error(&m_pMySql));
		return m_nfind_cnt;
	}

	Res = mysql_store_result(&m_pMySql);
	m_nfind_cnt = (long)mysql_num_rows(Res);
	Fields = mysql_fetch_fields(Res);

	m_strquery = new CString[m_nfind_cnt];
	
	for(i=0; i<m_nfind_cnt; i++)
	{
		num = mysql_num_fields(Res);
		Row = mysql_fetch_row(Res);
		
		if(Row == NULL)
		{
			mysql_free_result(Res);
		}
		
		strData = "";
		for(j=0; j<num; j++)
		{
			strData += Row[j];
			strData += ",";
		}
		
		m_strquery[i] = strData;
	}
	
	mysql_free_result(Res);
	m_nread_cnt = 0;
	
	return m_nfind_cnt;
}

long CAlg_Mysql::Record_Float_Search(CString tablename, CString itemsearch, CString range, float value[])
{
	MYSQL_RES *Res;
	MYSQL_ROW Row;
	MYSQL_FIELD *Fields;

	CString strQuery;
	CString stritem;
	CString strData;
	int i, j;
	int num;
	m_nfind_cnt = 0;

	stritem = "";
	strQuery.Format("DESCRIBE %s", tablename);
	
	if(mysql_query(&m_pMySql, strQuery))
	{
		Func.MsgLog(mysql_error(&m_pMySql));
		return m_nfind_cnt;
	}
	
	Res = mysql_store_result(&m_pMySql);
	num = (int)mysql_num_rows(Res);
	Fields = mysql_fetch_fields(Res);
	
	for(i=0; i<num; i++)
	{
		Row = mysql_fetch_row(Res);
		
		stritem += Row[0];
		if((num-1) != i)
		{
			stritem += ",";
		}
	}
	
	//	SELECT name,age from guest where age <= 25 ;
	if(range == "<")
	{
		strQuery.Format("SELECT %s FROM %s WHERE %s > %f", stritem, tablename, itemsearch, value[0]);
	}
	else if(range == ">")
	{
		strQuery.Format("SELECT %s FROM %s WHERE %s < %f", stritem, tablename, itemsearch, value[0]);
	}
	else if(range == "<>")
	{
		strQuery.Format("SELECT %s FROM %s WHERE %s > %f AND %s < %f", stritem, tablename, itemsearch, value[0], itemsearch, value[1]);
	}
	else if(range == "><")
	{
		strQuery.Format("SELECT %s FROM %s WHERE %s < %f OR %s > %f", stritem, tablename, itemsearch, value[0], itemsearch, value[1]);
	}
	else
	{
		strQuery.Format("SELECT %s FROM %s WHERE %s = %f", stritem, tablename, itemsearch, value[0]);
	}
	
	mysql_free_result(Res);
	
	if(mysql_query(&m_pMySql, strQuery))
	{
		Func.MsgLog(mysql_error(&m_pMySql));
		return m_nfind_cnt;
	}
	
	Res = mysql_store_result(&m_pMySql);
	m_nfind_cnt = (long)mysql_num_rows(Res);
	Fields = mysql_fetch_fields(Res);
	
	m_strquery = new CString[m_nfind_cnt];
	
	for(i=0; i<m_nfind_cnt; i++)
	{
		num = mysql_num_fields(Res);
		Row = mysql_fetch_row(Res);
		
		if(Row == NULL)
		{
			mysql_free_result(Res);
		}
		
		strData = "";
		for(j=0; j<num; j++)
		{
			strData += Row[j];
			strData += ",";
		}
		
		m_strquery[i] = strData;
	}
	
	mysql_free_result(Res);
	m_nread_cnt = 0;
	
	return m_nfind_cnt;
}

long CAlg_Mysql::Record_Double_Search(CString tablename, CString itemsearch, CString range, double value[])
{
	MYSQL_RES *Res;
	MYSQL_ROW Row;
	MYSQL_FIELD *Fields;
	
	CString strQuery;
	CString stritem;
	CString strData;
	int i, j;
	int num;
	m_nfind_cnt = 0;
	
	stritem = "";
	strQuery.Format("DESCRIBE %s", tablename);
	
	if(mysql_query(&m_pMySql, strQuery))
	{
		Func.MsgLog(mysql_error(&m_pMySql));
		return m_nfind_cnt;
	}
	
	Res = mysql_store_result(&m_pMySql);
	num = (int)mysql_num_rows(Res);
	Fields = mysql_fetch_fields(Res);
	
	for(i=0; i<num; i++)
	{
		Row = mysql_fetch_row(Res);
		
		stritem += Row[0];
		if((num-1) != i)
		{
			stritem += ",";
		}
	}
	
	//	SELECT name,age from guest where age <= 25 ;
	if(range == "<")
	{
		strQuery.Format("SELECT %s FROM %s WHERE %s > %f", stritem, tablename, itemsearch, value[0]);
	}
	else if(range == ">")
	{
		strQuery.Format("SELECT %s FROM %s WHERE %s < %f", stritem, tablename, itemsearch, value[0]);
	}
	else if(range == "<>")
	{
		strQuery.Format("SELECT %s FROM %s WHERE %s > %f AND %s < %f", stritem, tablename, itemsearch, value[0], itemsearch, value[1]);
	}
	else if(range == "><")
	{
		strQuery.Format("SELECT %s FROM %s WHERE %s < %f OR %s > %f", stritem, tablename, itemsearch, value[0], itemsearch, value[1]);
	}
	else
	{
		strQuery.Format("SELECT %s FROM %s WHERE %s = %f", stritem, tablename, itemsearch, value[0]);
	}
	
	mysql_free_result(Res);
	
	if(mysql_query(&m_pMySql, strQuery))
	{
		Func.MsgLog(mysql_error(&m_pMySql));
		return m_nfind_cnt;
	}
	
	Res = mysql_store_result(&m_pMySql);
	m_nfind_cnt = (long)mysql_num_rows(Res);
	Fields = mysql_fetch_fields(Res);
	
	m_strquery = new CString[m_nfind_cnt];
	
	for(i=0; i<m_nfind_cnt; i++)
	{
		num = mysql_num_fields(Res);
		Row = mysql_fetch_row(Res);
		
		if(Row == NULL)
		{
			mysql_free_result(Res);
		}
		
		strData = "";
		for(j=0; j<num; j++)
		{
			strData += Row[j];
			strData += ",";
		}
		
		m_strquery[i] = strData;
	}
	
	mysql_free_result(Res);
	m_nread_cnt = 0;
	
	return m_nfind_cnt;
}

long CAlg_Mysql::Record_Text_Search(CString tablename, CString itemsearch, CString value)
{
	MYSQL_RES *Res;
	MYSQL_ROW Row;
	MYSQL_FIELD *Fields;
	
	CString strQuery;
	CString stritem;
	CString strTmp[2], strData;
	int i, j;
	int num;
	m_nfind_cnt = 0;
	
	stritem = "";
	strQuery.Format("DESCRIBE %s", tablename);
	
	if(mysql_query(&m_pMySql, strQuery))
	{
		Func.MsgLog(mysql_error(&m_pMySql));
		return m_nfind_cnt;
	}
	
	Res = mysql_store_result(&m_pMySql);
	num = (int)mysql_num_rows(Res);
	Fields = mysql_fetch_fields(Res);
	
	for(i=0; i<num; i++)
	{
		Row = mysql_fetch_row(Res);
		
		stritem += Row[0];
		if((num-1) != i)
		{
			stritem += ",";
		}
	}
	
	strQuery.Format("SELECT %s FROM %s WHERE %s = '%s'", stritem, tablename, itemsearch, value);
	
	mysql_free_result(Res);
	
	if(mysql_query(&m_pMySql, strQuery))
	{
		Func.MsgLog(mysql_error(&m_pMySql));
		
		mysql_free_result(m_pRes);
		return m_nfind_cnt;
	}
	
	//	mysql_free_result(m_pRes);
	
	Res = mysql_store_result(&m_pMySql);
	m_nfind_cnt = (long)mysql_num_rows(Res);
	Fields = mysql_fetch_fields(Res);
	
	m_strquery = new CString[m_nfind_cnt];
	
	for(i=0; i<m_nfind_cnt; i++)
	{
		num = mysql_num_fields(Res);
		Row = mysql_fetch_row(Res);
		
		if(Row == NULL)
		{
			mysql_free_result(Res);
		}
		
		strData = "";
		for(j=0; j<num; j++)
		{
			strData += Row[j];
			strData += ",";
		}
		
		m_strquery[i] = strData;
	}
	
	mysql_free_result(Res);
	m_nread_cnt = 0;
	
	return m_nfind_cnt;
}

long CAlg_Mysql::Record_Date_Search(CString tablename, CString itemsearch, CString range, COleDateTime time[], int nMethod)
{
	MYSQL_RES *Res;
	MYSQL_ROW Row;
	MYSQL_FIELD *Fields;

	CString strQuery;
	CString stritem;
	CString strTmp[2], strData;
	int i, j;
	int num;
	m_nfind_cnt = 0;
	
	stritem = "";
	strQuery.Format("DESCRIBE %s", tablename);
	
	if(mysql_query(&m_pMySql, strQuery))
	{
		Func.MsgLog(mysql_error(&m_pMySql));
		return m_nfind_cnt;
	}
	
//	m_pRes = mysql_store_result(&m_pMySql);
//	num = (int)mysql_num_rows(m_pRes);
//	m_pFields = mysql_fetch_fields(m_pRes);

	Res = mysql_store_result(&m_pMySql);
	num = (int)mysql_num_rows(Res);
	Fields = mysql_fetch_fields(Res);
	
	for(i=0; i<num; i++)
	{
//		m_pRow = mysql_fetch_row(m_pRes);
		Row = mysql_fetch_row(Res);
//		stritem += m_pRow[0];
		stritem += Row[0];
		if((num-1) != i)
		{
			stritem += ",";
		}
	}

	if(range == "<")
	{
		strTmp[0].Format("%04d/%02d/%02d %02d:%02d:%02d", time[0].GetYear(), 
														  time[0].GetMonth(),
														  time[0].GetDay(),
														  time[0].GetHour(),
														  time[0].GetMinute(),
														  time[0].GetSecond());

		strQuery.Format("SELECT %s FROM %s WHERE %s > '%s'", stritem, tablename, itemsearch, strTmp[0]);
	}
	else if(range == ">")
	{
		strTmp[0].Format("%04d/%02d/%02d %02d:%02d:%02d", time[0].GetYear(), 
														  time[0].GetMonth(),
														  time[0].GetDay(),
														  time[0].GetHour(),
														  time[0].GetMinute(),
														  time[0].GetSecond());

		strQuery.Format("SELECT %s FROM %s WHERE %s < '%s'", stritem, tablename, itemsearch, strTmp[0]);
	}
	else if(range == "<>")
	{
		strTmp[0].Format("%04d/%02d/%02d %02d:%02d:%02d", time[0].GetYear(), 
														  time[0].GetMonth(),
														  time[0].GetDay(),
														  time[0].GetHour(),
														  time[0].GetMinute(),
														  time[0].GetSecond());
		strTmp[1].Format("%04d/%02d/%02d %02d:%02d:%02d", time[1].GetYear(), 
														  time[1].GetMonth(),
														  time[1].GetDay(),
														  time[1].GetHour(),
													      time[1].GetMinute(),
														  time[1].GetSecond());
		strQuery.Format("SELECT %s FROM %s WHERE %s > '%s' AND %s < '%s'", stritem, tablename, itemsearch, strTmp[0], itemsearch, strTmp[1]);
	}
	else if(range == "<!>")		// LOT_NO로 중복 제거.
	{
		strTmp[0].Format("%04d/%02d/%02d %02d:%02d:%02d", time[0].GetYear(), 
														  time[0].GetMonth(),
														  time[0].GetDay(),
														  time[0].GetHour(),
														  time[0].GetMinute(),
														  time[0].GetSecond());
		strTmp[1].Format("%04d/%02d/%02d %02d:%02d:%02d", time[1].GetYear(), 
  														  time[1].GetMonth(),
  														  time[1].GetDay(),
														  time[1].GetHour(),
														  time[1].GetMinute(),
														  time[1].GetSecond());

		if (nMethod == 0)
		{
			strQuery.Format("SELECT DISTINCT %s FROM %s WHERE %s > '%s' AND %s < '%s'", "LOT_NO", tablename, itemsearch, strTmp[0], itemsearch, strTmp[1]);
		}
		else if (nMethod == 1)
		{
			strQuery.Format("SELECT DISTINCT %s FROM %s WHERE %s > '%s' AND %s < '%s'", "SCRAP_CODE", tablename, itemsearch, strTmp[0], itemsearch, strTmp[1]);
		}
		else if (nMethod == 2)
		{
			strQuery.Format("SELECT %s FROM %s WHERE %s > '%s' AND %s < '%s'", stritem, tablename, itemsearch, strTmp[0], itemsearch, strTmp[1]);
		}
	}	
	else if(range == "<!!>")		// ALARM_CODE 중복 제거.
	{
		strTmp[0].Format("%04d/%02d/%02d %02d:%02d:%02d", time[0].GetYear(), 
			time[0].GetMonth(),
			time[0].GetDay(),
			time[0].GetHour(),
			time[0].GetMinute(),
			time[0].GetSecond());
		strTmp[1].Format("%04d/%02d/%02d %02d:%02d:%02d", time[1].GetYear(), 
			time[1].GetMonth(),
			time[1].GetDay(),
			time[1].GetHour(),
			time[1].GetMinute(),
			time[1].GetSecond());
		
		strQuery.Format("SELECT DISTINCT %s FROM %s WHERE %s > '%s' AND %s < '%s'", "ALARM_CODE", tablename, itemsearch, strTmp[0], itemsearch, strTmp[1]);
	}
	else if(range == "<@>")		// 시작 시간
	{
		strTmp[0].Format("%04d/%02d/%02d %02d:%02d:%02d", time[0].GetYear(), 
			time[0].GetMonth(),
			time[0].GetDay(),
			time[0].GetHour(),
			time[0].GetMinute(),
			time[0].GetSecond());
		strTmp[1].Format("%04d/%02d/%02d %02d:%02d:%02d", time[1].GetYear(), 
			time[1].GetMonth(),
			time[1].GetDay(),
			time[1].GetHour(),
			time[1].GetMinute(),
			time[1].GetSecond());
		strQuery.Format("SELECT MIN(STARTDATE) FROM %s WHERE %s > '%s' AND %s < '%s'", tablename, itemsearch, strTmp[0], itemsearch, strTmp[1]);
	}
	else if(range == "<@@>")		// 끝
	{
		strTmp[0].Format("%04d/%02d/%02d %02d:%02d:%02d", time[0].GetYear(), 
			time[0].GetMonth(),
			time[0].GetDay(),
			time[0].GetHour(),
			time[0].GetMinute(),
			time[0].GetSecond());
		strTmp[1].Format("%04d/%02d/%02d %02d:%02d:%02d", time[1].GetYear(), 
			time[1].GetMonth(),
			time[1].GetDay(),
			time[1].GetHour(),
			time[1].GetMinute(),
			time[1].GetSecond());
		strQuery.Format("SELECT MAX(ENDDATE) FROM %s WHERE %s > '%s' AND %s < '%s'", tablename, itemsearch, strTmp[0], itemsearch, strTmp[1]);
	}
	else if(range == "><")
	{
		strTmp[0].Format("%04d/%02d/%02d %02d:%02d:%02d", time[0].GetYear(), 
			time[0].GetMonth(),
			time[0].GetDay(),
														  time[0].GetHour(),
														  time[0].GetMinute(),
														  time[0].GetSecond());
		strTmp[1].Format("%04d/%02d/%02d %02d:%02d:%02d", time[1].GetYear(), 
														  time[1].GetMonth(),
														  time[1].GetDay(),
														  time[1].GetHour(),
														  time[1].GetMinute(),
														  time[1].GetSecond());
		strQuery.Format("SELECT %s FROM %s WHERE %s < '%s' OR %s > '%s'", stritem, tablename, itemsearch, strTmp[0], itemsearch, strTmp[1]);
	}
	else if(range == "<C>")		// ALARM CODE 중복 제거.
	{
		strTmp[0].Format("%04d/%02d/%02d %02d:%02d:%02d", time[0].GetYear(), 
			time[0].GetMonth(),
			time[0].GetDay(),
			time[0].GetHour(),
			time[0].GetMinute(),
			time[0].GetSecond());
		strTmp[1].Format("%04d/%02d/%02d %02d:%02d:%02d", time[1].GetYear(), 
			time[1].GetMonth(),
			time[1].GetDay(),
			time[1].GetHour(),
			time[1].GetMinute(),
			time[1].GetSecond());
		
		if (nMethod == 0 || nMethod == 1)
		{
			strQuery.Format("SELECT DISTINCT %s FROM %s WHERE %s > '%s' AND %s < '%s'", "ALARM_CODE", tablename, itemsearch, strTmp[0], itemsearch, strTmp[1]);
		}
		else if (nMethod == 2)
		{
			strQuery.Format("SELECT %s FROM %s WHERE %s > '%s' AND %s < '%s'", stritem, tablename, itemsearch, strTmp[0], itemsearch, strTmp[1]);
		}
	}
	else
	{
		strTmp[0].Format("%04d/%02d/%02d %02d:%02d:%02d", time[0].GetYear(), 
														  time[0].GetMonth(),
														  time[0].GetDay(),
														  time[0].GetHour(),
														  time[0].GetMinute(),
														  time[0].GetSecond());
		strQuery.Format("SELECT %s FROM %s WHERE %s = '%s'", stritem, tablename, itemsearch, strTmp[0]);
	}
	
	mysql_free_result(Res);
	
	if(mysql_query(&m_pMySql, strQuery))
	{
		Func.MsgLog(mysql_error(&m_pMySql));
		mysql_free_result(m_pRes);
		return m_nfind_cnt;
	}
	
	//	m_pRes = mysql_store_result(&m_pMySql);
	//	m_nfind_cnt = (long)mysql_num_rows(m_pRes);
	//	m_pFields = mysql_fetch_fields(m_pRes);
	
	Res = mysql_store_result(&m_pMySql);
	m_nfind_cnt = (long)mysql_num_rows(Res);
	Fields = mysql_fetch_fields(Res);
	
	m_strquery = new CString[m_nfind_cnt];
	
	for(i=0; i<m_nfind_cnt; i++)
	{
		num = mysql_num_fields(Res);
		Row = mysql_fetch_row(Res);
		
		if(Row == NULL)
		{
			mysql_free_result(Res);
		}
	
		strData = "";

		for(j=0; j<num; j++)
		{
			strData += Row[j];
			strData += ",";
		}
		
		m_strquery[i] = strData;
	}
	
	mysql_free_result(Res);
	m_nread_cnt = 0;
	
	return m_nfind_cnt;
}

BOOL CAlg_Mysql::Table_Select(CString databasename, CString tablename)
{
	CString strQuery;
	int num, i;
	CString strTmp;
	BOOL find;

	if(mysql_query(&m_pMySql, "SHOW TABLES"))
	{
		Func.MsgLog(mysql_error(&m_pMySql));
		return FALSE;
	}

	m_pRes = mysql_store_result(&m_pMySql);
	num = (int)mysql_num_rows(m_pRes);
	m_pFields = mysql_fetch_fields(m_pRes);
	
	find = FALSE;

	for(i=0; i<num; i++)
	{
		m_pRow = mysql_fetch_row(m_pRes);
		
		strTmp = m_pRow[0];

		strTmp.MakeUpper();
		
		if(strTmp == tablename)
		{
			find = TRUE;
			break;
		}
	}
	
	if(find)
	{
		if(mysql_select_db(&m_pMySql, tablename))
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	
	mysql_free_result(m_pRes);

	return FALSE;
}

CString CAlg_Mysql::Record_Read()
{
	CString strTmp;
	
	strTmp = "";
	m_nread_cnt++;

	strTmp = m_strquery[m_nread_cnt-1];
	if(m_nread_cnt == m_nfind_cnt)
	{
		delete [] m_strquery;
		m_strquery = NULL;
	}
	return strTmp;
}

long CAlg_Mysql::Record_DateLot_Search(CString lotname, CString tablename, CString itemsearch, CString range, COleDateTime time[], int nMethod)
{
	MYSQL_RES *Res;
	MYSQL_ROW Row;
	MYSQL_FIELD *Fields;

	CString strQuery;
	CString stritem;
	CString strTmp[2], strData;
	int i, j;
	int num;
	m_nfind_cnt = 0;
	
	stritem = "";
	strQuery.Format("DESCRIBE %s", tablename);
	
	if(mysql_query(&m_pMySql, strQuery))
	{
		Func.MsgLog(mysql_error(&m_pMySql));
		return m_nfind_cnt;
	}

	Res = mysql_store_result(&m_pMySql);
	num = (int)mysql_num_rows(Res);
	Fields = mysql_fetch_fields(Res);
	
	for(i=0; i<num; i++)
	{
		Row = mysql_fetch_row(Res);
		stritem += Row[0];
		if((num-1) != i)
		{
			stritem += ",";
		}
	}

	if(range == "<")
	{
		strTmp[0].Format("%04d/%02d/%02d %02d:%02d:%02d", time[0].GetYear(), 
														  time[0].GetMonth(),
														  time[0].GetDay(),
														  time[0].GetHour(),
														  time[0].GetMinute(),
														  time[0].GetSecond());

		strQuery.Format("SELECT %s FROM %s WHERE %s > '%s'", stritem, tablename, itemsearch, strTmp[0]);
	}
	else if(range == ">")
	{
		strTmp[0].Format("%04d/%02d/%02d %02d:%02d:%02d", time[0].GetYear(), 
														  time[0].GetMonth(),
														  time[0].GetDay(),
														  time[0].GetHour(),
														  time[0].GetMinute(),
														  time[0].GetSecond());

		strQuery.Format("SELECT %s FROM %s WHERE %s < '%s'", stritem, tablename, itemsearch, strTmp[0]);
	}
	else if(range == "<>")
	{
		strTmp[0].Format("%04d/%02d/%02d %02d:%02d:%02d", time[0].GetYear(), 
														  time[0].GetMonth(),
														  time[0].GetDay(),
														  time[0].GetHour(),
														  time[0].GetMinute(),
														  time[0].GetSecond());
		strTmp[1].Format("%04d/%02d/%02d %02d:%02d:%02d", time[1].GetYear(), 
														  time[1].GetMonth(),
														  time[1].GetDay(),
														  time[1].GetHour(),
													      time[1].GetMinute(),
														  time[1].GetSecond());

		if (nMethod == 0)
		{
			strQuery.Format("SELECT %s LOT_NO FROM %s WHERE %s > '%s' AND %s < '%s' AND LOT_NO = '%s'", stritem, tablename, itemsearch, strTmp[0], itemsearch, strTmp[1], lotname);
		}
		else if (nMethod == 1)
		{
			strQuery.Format("SELECT %s SCRAP_CODE FROM %s WHERE %s > '%s' AND %s < '%s' AND SCRAP_CODE = '%s'", stritem, tablename, itemsearch, strTmp[0], itemsearch, strTmp[1], lotname);
		}
		else if (nMethod == 2)
		{
			strQuery.Format("SELECT %s FROM %s WHERE %s > '%s' AND %s < '%s'", stritem, tablename, itemsearch, strTmp[0], itemsearch, strTmp[1]);
		}
	}
	else if(range == "<!>")		// LOT_NO로 중복 제거.
	{
		strTmp[0].Format("%04d/%02d/%02d %02d:%02d:%02d", time[0].GetYear(), 
														  time[0].GetMonth(),
														  time[0].GetDay(),
														  time[0].GetHour(),
														  time[0].GetMinute(),
														  time[0].GetSecond());
		strTmp[1].Format("%04d/%02d/%02d %02d:%02d:%02d", time[1].GetYear(), 
  														  time[1].GetMonth(),
  														  time[1].GetDay(),
														  time[1].GetHour(),
														  time[1].GetMinute(),
														  time[1].GetSecond());

		if (nMethod == 0)
		{
			strQuery.Format("SELECT DISTINCT %s FROM %s WHERE %s > '%s' AND %s < '%s'", "LOT_NO", tablename, itemsearch, strTmp[0], itemsearch, strTmp[1]);
		}
		else if (nMethod == 1)
		{
			strQuery.Format("SELECT DISTINCT %s FROM %s WHERE %s > '%s' AND %s < '%s'", "BOARD_NO", tablename, itemsearch, strTmp[0], itemsearch, strTmp[1]);
		}
		else if (nMethod == 2)
		{
			strQuery.Format("SELECT %s FROM %s WHERE %s > '%s' AND %s < '%s'", stritem, tablename, itemsearch, strTmp[0], itemsearch, strTmp[1]);
		}
	}	
	else if(range == "<@>")		// 시작 시간
	{
		strTmp[0].Format("%04d/%02d/%02d %02d:%02d:%02d", time[0].GetYear(), 
			time[0].GetMonth(),
			time[0].GetDay(),
			time[0].GetHour(),
			time[0].GetMinute(),
			time[0].GetSecond());
		strTmp[1].Format("%04d/%02d/%02d %02d:%02d:%02d", time[1].GetYear(), 
			time[1].GetMonth(),
			time[1].GetDay(),
			time[1].GetHour(),
			time[1].GetMinute(),
			time[1].GetSecond());

		if (nMethod == 0)
		{
			strQuery.Format("SELECT MIN(STARTDATE) LOT_NO FROM %s WHERE %s > '%s' AND %s < '%s' AND LOT_NO = '%s'", tablename, itemsearch, strTmp[0], itemsearch, strTmp[1], lotname);
		}
		else if (nMethod == 1)
		{
			strQuery.Format("SELECT MIN(STARTDATE) PART_NO FROM %s WHERE %s > '%s' AND %s < '%s' AND BOARD_NO = '%s'", tablename, itemsearch, strTmp[0], itemsearch, strTmp[1], lotname);
		}
		else if (nMethod == 2)
		{
			strQuery.Format("SELECT MIN(STARTDATE) FROM %s WHERE %s > '%s' AND %s < '%s'", tablename, itemsearch, strTmp[0], itemsearch, strTmp[1]);
		}
	}
	else if(range == "<@@>")		// 끝
	{
		strTmp[0].Format("%04d/%02d/%02d %02d:%02d:%02d", time[0].GetYear(), 
			time[0].GetMonth(),
			time[0].GetDay(),
			time[0].GetHour(),
			time[0].GetMinute(),
			time[0].GetSecond());
		strTmp[1].Format("%04d/%02d/%02d %02d:%02d:%02d", time[1].GetYear(), 
			time[1].GetMonth(),
			time[1].GetDay(),
			time[1].GetHour(),
			time[1].GetMinute(),
			time[1].GetSecond());

		if (nMethod == 0)
		{
			strQuery.Format("SELECT MAX(ENDDATE) LOT_NO FROM %s WHERE %s > '%s' AND %s < '%s' AND LOT_NO = '%s'", tablename, itemsearch, strTmp[0], itemsearch, strTmp[1], lotname);
		}
		else if (nMethod == 1)
		{
			strQuery.Format("SELECT MAX(ENDDATE) BOARD_NO FROM %s WHERE %s > '%s' AND %s < '%s' AND BOARD_NO = '%s'", tablename, itemsearch, strTmp[0], itemsearch, strTmp[1], lotname);
		}
		else if (nMethod == 2)
		{
			strQuery.Format("SELECT MIN(STARTDATE) FROM %s WHERE %s > '%s' AND %s < '%s'", tablename, itemsearch, strTmp[0], itemsearch, strTmp[1]);
		}
	}
	else if(range == "><")
	{
		strTmp[0].Format("%04d/%02d/%02d %02d:%02d:%02d", time[0].GetYear(), 
			time[0].GetMonth(),
			time[0].GetDay(),
														  time[0].GetHour(),
														  time[0].GetMinute(),
														  time[0].GetSecond());
		strTmp[1].Format("%04d/%02d/%02d %02d:%02d:%02d", time[1].GetYear(), 
														  time[1].GetMonth(),
														  time[1].GetDay(),
														  time[1].GetHour(),
														  time[1].GetMinute(),
														  time[1].GetSecond());
		strQuery.Format("SELECT %s FROM %s WHERE %s < '%s' OR %s > '%s'", stritem, tablename, itemsearch, strTmp[0], itemsearch, strTmp[1]);
	}
	else
	{
		strTmp[0].Format("%04d/%02d/%02d %02d:%02d:%02d", time[0].GetYear(), 
														  time[0].GetMonth(),
														  time[0].GetDay(),
														  time[0].GetHour(),
														  time[0].GetMinute(),
														  time[0].GetSecond());
		strQuery.Format("SELECT %s FROM %s WHERE %s = '%s'", stritem, tablename, itemsearch, strTmp[0]);
	}
	
	mysql_free_result(Res);

	if(mysql_query(&m_pMySql, strQuery))
	{
		Func.MsgLog(mysql_error(&m_pMySql));
		mysql_free_result(m_pRes);
		return m_nfind_cnt;
	}
	
	Res = mysql_store_result(&m_pMySql);
	m_nfind_cnt = (long)mysql_num_rows(Res);
	Fields = mysql_fetch_fields(Res);
	
	m_strquery = new CString[m_nfind_cnt];
	
	for(i=0; i<m_nfind_cnt; i++)
	{
		num = mysql_num_fields(Res);
		Row = mysql_fetch_row(Res);
		
		if(Row == NULL)
		{
			mysql_free_result(Res);
		}
		
		strData = "";
		for(j=0; j<num; j++)
		{
			strData += Row[j];
			strData += ",";
		}
		
		m_strquery[i] = strData;
	}
	
	mysql_free_result(Res);
	m_nread_cnt = 0;
	
	return m_nfind_cnt;
}