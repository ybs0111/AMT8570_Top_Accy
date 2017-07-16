// MyBasicData.h: interface for the CMyBasicData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYBASICDATA_H__F10E96B3_4961_4A37_B43F_FDAD481A0494__INCLUDED_)
#define AFX_MYBASICDATA_H__F10E96B3_4961_4A37_B43F_FDAD481A0494__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Variable.h"  // 전역 변수 정의 클래스 추가 
#include "Public_Function.h"

class CMyBasicData  
{
public:
	CMyBasicData();
	virtual ~CMyBasicData();

// Attributes
public:

// Operations
public:
	void On_Teach_Data_Save();
	int On_Teach_Data_Load();
	CString OnGet_Teach_File_Name(CString strName);
	CString OnGet_File_Name();
	void OnBasic_Data_Save();
	void OnLoad_BasicData();
	void OnMaintenance_Data_Save();
	void OnLoad_MaintData();

	void OnStep_File_Name_Save(CString str_file);
	CString OnStep_File_Name_Load();
	void OnStep_File_Index_Save(CString str_index);
	CString OnStep_File_Index_Load();
	void OnPassWord_Save(CString str_save_level, CString str_pass_data);
	void OnPassWord_Load(CString str_load_level);

	// 백상현 수정 [2014.08.26]
	void OnSaveWaitTimeFile();
	void OnLoad_WaitTimeData();

	void OnSaveMotorSpeedFile();
	void OnLoad_MotSpeedData();

	void On_Network_Data_Load();
	void On_Network_Data_Save();
};

#endif // !defined(AFX_MYBASICDATA_H__F10E96B3_4961_4A37_B43F_FDAD481A0494__INCLUDED_)
