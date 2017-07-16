// MyJamData.h: interface for the CMyJamData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYJAMDATA_H__CB595225_06C8_44CA_9BC6_742D8AC656E1__INCLUDED_)
#define AFX_MYJAMDATA_H__CB595225_06C8_44CA_9BC6_742D8AC656E1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Variable.h"       // ���� ���� ���� Ŭ����

class CMyJamData  
{
public:
	CMyJamData();
	virtual ~CMyJamData();

// Attributes
public:

// Operations
public:
	CString OnLoad_AlarmDispInfo(CString szPath, CString szDataFile, CString szActFile, CString szCode);	// �߻��� �˶��� ���� ��� ���� ����
	CString OnSearch_AlarmBmpInfo(CString szPath, CString szDataFile, CString szCode);			// �߻��� �˶��� ���� ��Ʈ�� �̹��� ���� ã��
	CString OnSearch_AlarmDispStyleInfo(CString szPath, CString szDataFile, CString szCode);	// �߻��� �˶��� ���� ȭ�� ��� ��Ÿ�� ���� ã��
	CString OnSearch_AlarmRepairInfo(CString szPath, CString szActFile, CString szCode);		// �߻��� �˶��� ���� ��ġ ���� ���� ã��

	void On_Alarm_Display();					// �߻��� �˶� ȭ�� ��� �Լ�
	void OnSave_AlarmInfo2LogFile();	// [�α� ����] �߻��� �˶� ���� ���Ͽ� ����
	void OnLoad_AlarmInfoFronFile();	// [����->����ü] ��� �˶� ���� �ε�

	void On_Occured_Alarm_Saving_to_File();		// �߻��� �˶� �α� ���Ͽ� �����ϴ� �Լ�
	void On_Alarm_Info_Set_to_Variable(CString strErrCode);  // �߻��� �˶� �޽��� ���� ������ �����ϴ� �Լ�
	void OnSet_Info2StrctAlarm(CString szJamCode);	// �߻��� �˶� ������ ����ü�� �Ҵ�
	void OnAdd_2DailyFile(CString szJamCode, CString szTimeDisp, CString sJamMsg);	// [�Ϻ�] �˶� ���� ���Ͽ� ����

};
extern CMyJamData	MyJamData;

#endif // !defined(AFX_MYJAMDATA_H__CB595225_06C8_44CA_9BC6_742D8AC656E1__INCLUDED_)
