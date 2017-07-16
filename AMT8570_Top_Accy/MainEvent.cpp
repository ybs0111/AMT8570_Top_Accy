// MainEvent.cpp: implementation of the CMainEvent class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "handler.h"
#include "MainEvent.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMainEvent	g_client_accy;

CMainEvent::CMainEvent()
{
	m_nArrayPos = 0;
	//st_client[m_nArrayPos].str_ip = "192.168.1.10";
// 	st_client[m_nArrayPos].str_ip = "192.168.1.10";
// 	st_client[m_nArrayPos].n_port = 2000;
}

CMainEvent::~CMainEvent()
{

}

//==================================================================//
// 쓰레드 동작
//==================================================================//
void CMainEvent::OnSeq_RUN(void) 
{
	OnSeq_Init();     // 쓰레드 관련 변수 초기화
	OnSeq_Execute();  // 쓰레드 제어
}

//==================================================================//
// 쓰레드 관련 변수 초기화
//==================================================================//
void CMainEvent::OnSeq_Init(void) 
{
	if (m_bflag_INIT == true)	{
		return;
	}
	m_bflag_INIT = true;
	
	// 스텝 정보 초기화
	m_nStep_RUN	= 0;
	
	m_sRcvData.Empty();
}

//==================================================================//
// 쓰레드 제어
//==================================================================//
void CMainEvent::OnSeq_Execute(void) 
{
	if (m_nReqStart == REQ_CALL_)
	{
		OnRun_MainWork();		// [MAIN->ACCY] 데이터 전송 작업
	}
}

//==================================================================//
// [MAIN<->ACCY] MAIN 과 읽기/쓰기 작업
//==================================================================//
int CMainEvent::OnRun_MainWork() 
{
		// [초기화] 함수 리턴 플래그 (작업중)
	int FuncRet = RET_PROCEED;

	CString sMsg;
	switch(m_nStep_RUN)
	{
	case 0:
		// [MAIN<->ACCY] 연결되어 있는지 확인
		if (st_client[m_nArrayPos].n_connect == CTL_YES)
		{
			m_nStep_RUN = 1000;
		}
		// 연결되어 있지 않으면 연결 작업 시도
		else
		{
			::PostMessage(st_handler.hWnd, WM_CLIENT_MSG_1, CLIENT_CONNECT, m_nArrayPos);

			// 연결 시작 시간 설정
			m_lWaitResp[0] = GetCurrentTime();
			m_nStep_RUN = 100;
		}
		break;

	case 100:
		m_lWaitResp[1] = GetCurrentTime();
		m_lWaitResp[2] = m_lWaitResp[1] - m_lWaitResp[0];
		if (m_lWaitResp[2] < 0)
		{
			m_lWaitResp[0] = GetCurrentTime();
			break;
		}

		if (st_client[m_nArrayPos].n_connect == CTL_YES)
		{
			m_nStep_RUN = 1000;
		}
		else
		{
			if (m_lWaitResp[2] > 5000)
			{
				m_nStep_RUN = 5000;	// 에러 반환 스텝
			}
		}
		break;

	// 전송할 메시지 생성 OnReq_MesWork() 함수 호출
	case 1000:
		sMsg.Format("%s%s", st_client[m_nArrayPos].chSendHead, st_client[m_nArrayPos].chSendBody);
		PushSendMsg(sMsg);	// [EQP->LowerStream] 메시지 전송
		
		m_nIdx = st_client[m_nArrayPos].nIndex;
		// 전송할 메시지에 대한 응답 기다림 여부 및 상태 정보
		// - [0:default, 1:정상, -1:에러, 10:응답존재(기다림), 11:응답없음]
		if ((st_client[m_nArrayPos].stComm[m_nIdx]).nResponse == 10)
		{
			// 타임아웃 검사할지 여부 [0:검사불필요, 1:검사필요]
			if ((st_client[m_nArrayPos].stComm[m_nIdx]).nTimeOut == 1)
			{
				m_nStep_RUN = 1500;
			}
			else
			{
				m_nStep_RUN = 1100;
			}
		}
		else
		{
			m_nStep_RUN = 4000;
		}
		break;

	case 1100:
		// 전송한 메시지에 대한 응답 체크 시작 시간 설정
		m_lWaitResp[0] = GetCurrentTime();
		m_nStep_RUN = 1200;
		break;
		
	case 1200:
		m_lWaitResp[1] = GetCurrentTime();
		m_lWaitResp[2] = m_lWaitResp[1] - m_lWaitResp[0];
		if (m_lWaitResp[2] < 0)
		{
			m_lWaitResp[0] = GetCurrentTime();
			break;
		}
		
		// [상류->하류] 수신 메시지 파싱 함수에서 설정함
		
		// 전송할 메시지에 대한 응답 기다림 여부 및 상태 정보
		// - [0:default, 1:정상, -1:에러, 10:응답존재(기다림), 11:응답없음]
		if ((st_client[m_nArrayPos].stComm[m_nIdx]).nResponse == CTL_GOOD)
		{
			m_nStep_RUN = 4000;
		}
		else if ((st_client[m_nArrayPos].stComm[m_nIdx]).nResponse == CTL_ERROR)
		{
			m_nStep_RUN = 5000;
		}
		else
		{
			// 통신 타임 아웃 발생
			if (m_lWaitResp[2] > COMM_TIME_OUT)
			{
				m_nStep_RUN = 5000;
			}
		}
		break;
		
	case 1500:
		// [상류->하류] 수신 메시지 파싱 함수에서 설정함
		
		// 전송할 메시지에 대한 응답 기다림 여부 및 상태 정보
		// - [0:default, 1:정상, -1:에러, 10:응답존재(기다림), 11:응답없음]
		if ((st_client[m_nArrayPos].stComm[m_nIdx]).nResponse == CTL_GOOD)
		{
			m_nStep_RUN = 4000;
		}
		else if ((st_client[m_nArrayPos].stComm[m_nIdx]).nResponse == CTL_ERROR)
		{
			m_nStep_RUN = 5000;
		}
		break;

	// 정상 완료 스텝
	case 4000:
		m_nReqStart	= REPLY_CORRECT_;

		m_nStep_RUN = 0;
		FuncRet = RET_GOOD;
		break;

		// 에러 반환 스텝
	case 5000:
		m_nReqStart	= REPLY_ERROR_;

		m_nStep_RUN = 0;
		FuncRet = RET_ERROR;
		break;
	}

	return FuncRet;
}

//==================================================================//
// [Main->Accy] 수신된 메시지 구분하여 처리
//==================================================================//
int CMainEvent::OnDivide_FromMain(int nzSocketNo, CString szCommand) 
{
	int nRet = 0;
	
	// 수신된 데이터가 없으면 강제 리턴
	if (szCommand == "")
	{
		return -1;
	}
	// kilee [2015.03.20]
	/*
	// 수신된 메시지 로그에 남김
	m_sLogMsg.Format("Client_[%02d] Data Receive - %s", nzSocketNo, szCommand);
	cLOG.OnLogEvent(LOG_SOCKET_, m_sLogMsg);
	*/

	CString sPart_Head, sPart_Len, sPart_Body, sData_Process;
	int nPart_Len, nPart_Body, nLen_TotRcv;

	// 전송한 명령어의 인덱스 정보 얻음
	// - 전송한 명령어에 대한 응답인지 확인하기 위함
	int nIdx = st_client[m_nArrayPos].nIndex;

	m_sRcvData += szCommand;	// 수신 메시지 누적시킴
	
	do{
		nLen_TotRcv = m_sRcvData.GetLength();
		// 최소한 길이부+데이터(1글자) 이상이어야 함
		if (nLen_TotRcv > 20)
		{
			// 데이터의 헤더부 얻기 [ 예를 들면 (HD=) ]
			sPart_Head	= m_sRcvData.Mid(0,3);
			// 데이터의 길이부 얻기 [예를 들면 (000025) ]
			sPart_Len	= m_sRcvData.Mid(3,16);
			// 수신할 데이터의 길이 정보 설정
			nPart_Len	= atoi((LPCSTR)sPart_Len);
			// 실재 데이터부 얻기 [ 예를 들면 (FUNC=INTERFACE,CONNECT=0,) ]
			sPart_Body	= m_sRcvData.Mid(19);
			// Body(데이터) 정보 설정
			nPart_Body	= sPart_Body.GetLength();
			
			// 데이터의 정합성 검사
			// - 길이부의 문자가 숫자형인지 검사
			// : 길이부의 첫글자만 비교함
			// : 이 부분은 숫자형으로 구성되어 있어야 함
			if (sPart_Head == "HD=" &&
				m_sRcvData.GetAt(3) >= '0' && m_sRcvData.GetAt(3) <= '9')
			{
				sData_Process = m_sRcvData.Mid(0, nPart_Len +19);
				// 전제 수신된 문자열 정보 설정
				nLen_TotRcv = sData_Process.GetLength();
				// 처리하기 위해 뽑아낸 데이터는 삭제함
				// - 수신된 데이터에서 처리한 문자열만큼 삭제시킴
				// - 처리하고 남은 데이터를 버퍼 변수에 재할담
				m_sRcvData = m_sRcvData.Mid(nLen_TotRcv);
				
				// 정보가 2개 연달아 (붙어서) 들어오는 경우 이곳에서 곧바로 파싱하는 함수 호출하면 문제가 발생함
				// - 처음에 수신된 명령어는 처리되는데 뒤에 붙어서 수신된 명령어는 처리되지 못하고 다음 명령어 수신된 경우에 처리되게 됨
				// - 즉, 명령어가 밀려서 처리될 수 있음
				// - 하지만 현재 메시지 쳬계로는 [명령어<->응답] 구조이므로 2개 명령어가 연달아 수신되지 않아 문제는 없음
				int nResp = OnAnalysis_MesData(sData_Process);
				if (nResp == RET_ERROR)
				{
					//m_sRcvData = "";
					
					// 수신 데이터 파싱 작업 중 에러 발생함
					// 전송할 메시지에 대한 응답 기다림 여부 및 상태 정보
					// - [0:default, 1:정상, -1:에러, 10:응답존재(기다림), 11:응답없음]
					(st_client[m_nArrayPos].stComm[nIdx]).nResponse = CTL_ERROR;
				}
				else if (nResp == RET_GOOD)
				{
					m_sRcvData = "";
					
					//PushSendMsg(sData_Process);
					
					// 수신 데이터 정상 파싱됨
					// 전송할 메시지에 대한 응답 기다림 여부 및 상태 정보
					// - [0:default, 1:정상, -1:에러, 10:응답존재(기다림), 11:응답없음]
					(st_client[m_nArrayPos].stComm[nIdx]).nResponse = CTL_GOOD;
				}
			}
			else
			{
				// 처리해야 할 한개의 명령어 길이보다 수신 데이터의 길이가 크면서 명령어 포멧에 문제가 있는 경우
				// - 강제로 버퍼를 비워 다음 수신 데이터부터 처리되도록 함
				if (nPart_Body > nPart_Len)
				{
					// 지금까지 수신된 데이터는 폐기함
					// - 추후에 수신되는 데이터부터 처리되도록 함
					m_sRcvData.Empty();
				}
				// 수신된 데이터가 처리 불가능한 데이터
				// - 다음 수신 메시지의 처리를 위해 강제로 리턴시킴
				nRet = -1;
			}
		}
		// 명령어 최소 길이 포멧을 만족하지 못함
		// - 데이터를 더 수신해야 함
		else
		{
			nRet = -1;
		}
    }while (nRet != -1);
	return 0;
}

//==================================================================//
// 수신된 메시지 분석 작업
//==================================================================//
int CMainEvent::OnAnalysis_MesData(CString szRcvData) 
{
	/*
	// 헤더 정보를 추출해냄
	// - 공백 또는 찾은 문자열이 반환됨
	CString sHeader = OnAnalysis_MesBody(0, "FUNCTION=", szRcvData);
	CString sBody[5];

	// 전송한 명령어의 인덱스 정보 얻음
	// - 전송한 명령어에 대한 응답인지 확인하기 위함
	int nIdx = st_client[m_nArrayPos].nIndex;

	if (sHeader == "ACCY_MATH_CODE")	
	{
		st_work.m_sAccyMathCode = OnAnalysis_MesBody(0, "VALUE=", szRcvData);
	}
	else
	{
		return RET_ERROR;
	}
	*/

	CString sHeader = OnFindInRcvMsg("FUNCTION=", szRcvData, ";");
	CString sBody[5];
	
	// 전송한 명령어의 인덱스 정보 얻음
	// - 전송한 명령어에 대한 응답인지 확인하기 위함
	int nIdx = st_client[m_nArrayPos].nIndex;
	
	if (sHeader == "ACCY_MATH_CODE")	
	{
		st_work.m_sRcvAccyMathCode = OnFindInRcvMsg("VALUE=", szRcvData, ";");
	}
	else
	{
		return RET_ERROR;
	}

	return RET_GOOD;
}

//==================================================================//
// Body 정보 분석
//==================================================================//
CString CMainEvent::OnAnalysis_MesBody(int nzMode, CString szFind, CString szBody) 
{
	CString sFindData = "";

	int nLen;
	int nPos[4];

	// 문자열 길이 계산
	nLen = szBody.GetLength();

	nPos[0] = -1;
	// 찾고자 하는 문자열의 위치 얻음
	nPos[0] = szBody.Find(szFind, 0);
	// 찾고자 하는 문자열이 존재하지 않으면 공백 문자 반환
	if (nPos[0] < 0)
	{
		return sFindData;
	}

	nPos[1] = -1;
	nPos[1] = szBody.Find("=", nPos[0] +1);
	if (nPos[1] < 0)
	{
		return sFindData;
	}

	switch(nzMode)
	{
	case 0:
		nPos[2] = -1;
		nPos[2] = szBody.Find(";", nPos[0] +1);

		if (nPos[2] == -1)
		{
			sFindData = szBody.Mid(nPos[1] +1, nLen - (nPos[1] +1));
		}
		else	{
			sFindData = szBody.Mid(nPos[1] +1, nPos[2] - (nPos[1] +1));
		}
		break;

	case 1:
		nPos[2] = -1;
		nPos[2] = szBody.Find(")", nPos[1] +1);
		
		sFindData = szBody.Mid(nPos[1] +1, nPos[2] - (nPos[1]));
		break;

	case 2:
		nPos[2] = -1;
		nPos[2] = szBody.Find("^XZ", nPos[1] +1);

		sFindData = szBody.Mid(nPos[1] +1, nPos[2] - (nPos[1] +1));
		break;

	case 3:
		nPos[2] = -1;
		nPos[2] = szBody.Find("\"", nPos[1] +1);

		nPos[3] = -1;
		nPos[3] = szBody.Find("\"", nPos[2] +1);

		sFindData = szBody.Mid(nPos[2] +1, nPos[3] - (nPos[2] +1));
		break;

	case 4:
		nPos[2] = -1;
		nPos[2] = szBody.GetLength();

		sFindData = szBody.Mid(nPos[1] +1, nPos[2] - (nPos[1]));
		break;
	}

	return sFindData;
}

//==================================================================//
// [EQP->MES] 메시지 전송
//==================================================================//
int CMainEvent::PushSendMsg(CString szMsg) 
{
	// 전송할 문자열 설정
	sprintf(st_client[m_nArrayPos].ch_send, szMsg);
	//::SendMessage(st_handler.hWnd, WM_CLIENT_MSG_1, CLIENT_SEND, m_nArrayPos);
	::PostMessage(st_handler.hWnd, WM_CLIENT_MSG + m_nArrayPos, CLIENT_SEND, m_nArrayPos);
	
	return 0;
}

//==================================================================//
// [MAIN->ACCY] 전송할 메시지 생성
//==================================================================//
void CMainEvent::OnCreate_SendFormat(int nzIdx) 
{
	CString sFunction;
	CString sHead, sBody, sTemp, sBuff;
	int nReadIO[3] = {0,};

	switch(nzIdx)
	{
	case MAIN_BCR_STATE:
		sTemp.Empty();
		sBody.Format("FUNCTION_RPY=BCR_STATE;EQP_ID=%d;", st_map.nSupplyTypeMath);
		sHead.Format("HD=%016d;", sBody.GetLength());
		
		sprintf(st_client[m_nArrayPos].chSendHead, sHead);
		sprintf(st_client[m_nArrayPos].chSendBody, sBody);
		
		sFunction = "BCR_STATE";
		sprintf((st_client[m_nArrayPos].stComm[nzIdx]).chSendCmd, sFunction);
		sFunction = "BCR_STATE";
		sprintf((st_client[m_nArrayPos].stComm[nzIdx]).chResponse, sFunction);
		break;
		
	case MAIN_ALARM:
		sTemp.Empty();
		sBody.Format("FUNCTION_RPY=ALARM;ALARM_CODE=%s;ALARM_TITLE=%s;", alarm.mstr_code, st_alarm.mstr_cur_msg);
		sHead.Format("HD=%016d;", sBody.GetLength());
		
		sprintf(st_client[m_nArrayPos].chSendHead, sHead);
		sprintf(st_client[m_nArrayPos].chSendBody, sBody);
		
		sFunction = "ALARM";
		sprintf((st_client[m_nArrayPos].stComm[nzIdx]).chSendCmd, sFunction);
		sFunction = "ALARM";
		sprintf((st_client[m_nArrayPos].stComm[nzIdx]).chResponse, sFunction);
		break;
		
	case MAIN_ACCY_SUPPLY_COMPLETE:
		sTemp.Empty();
		sBody.Format("FUNCTION_RPY=COMPLETE;");
		sHead.Format("HD=%016d;", sBody.GetLength());
		
		sprintf(st_client[m_nArrayPos].chSendHead, sHead);
		sprintf(st_client[m_nArrayPos].chSendBody, sBody);
		
		sFunction = "COMPLETE";
		sprintf((st_client[m_nArrayPos].stComm[nzIdx]).chSendCmd, sFunction);
		sFunction = "COMPLETE";
		sprintf((st_client[m_nArrayPos].stComm[nzIdx]).chResponse, sFunction);
		break;
		
	case MAIN_EQP_STATE:
		sTemp.Empty();
		sBody.Format("FUNCTION_RPY=EQP_STATE;EQP_STATE=%d;", st_work.nEqpStatus);
		sHead.Format("HD=%016d;", sBody.GetLength());
		
		sprintf(st_client[m_nArrayPos].chSendHead, sHead);
		sprintf(st_client[m_nArrayPos].chSendBody, sBody);
		
		sFunction = "EQP_STATE";
		sprintf((st_client[m_nArrayPos].stComm[nzIdx]).chSendCmd, sFunction);
		sFunction = "EQP_STATE";
		sprintf((st_client[m_nArrayPos].stComm[nzIdx]).chResponse, sFunction);
		break;
		
	case MAIN_SUPPLY_TYPE:
		sTemp.Empty();
		sBody.Format("FUNCTION_RPY=SUPPLY_TYPE;LOT_TYPE=%d;", st_basic.nMode_SupplyDevice);
		sHead.Format("HD=%016d;", sBody.GetLength());
		
		sprintf(st_client[m_nArrayPos].chSendHead, sHead);
		sprintf(st_client[m_nArrayPos].chSendBody, sBody);
		
		sFunction = "SUPPLY_TYPE";
		sprintf((st_client[m_nArrayPos].stComm[nzIdx]).chSendCmd, sFunction);
		sFunction = "SUPPLY_TYPE";
		sprintf((st_client[m_nArrayPos].stComm[nzIdx]).chResponse, sFunction);
		break;

	case MAIN_SUPPLY_READY_CNT:
		sTemp.Empty();
		
		// yangkyu [2015.02.26]
		// 센서가 B접이라 센서 상태 확인 시 반전 사용
		nReadIO[0] = !g_ioMgr.get_in_bit(stIO.i_Chk_AccyBoxConvDetection[SIDE_THIRD_]);
		nReadIO[1] = !g_ioMgr.get_in_bit(stIO.i_Chk_AccyBoxConvDetection[SIDE_SECOND_]);
		nReadIO[2] = !g_ioMgr.get_in_bit(stIO.i_Chk_AccyBoxConvDetection[SIDE_FIRST_]);
		
		sBody.Format("FUNCTION_RPY=SUPPLY_CNT;ACCY_CNT=%d;", st_map.nResp_AccyReadyCNT + (nReadIO[0] * 160) + (nReadIO[1] * 160) + (nReadIO[2] * 160));
		sHead.Format("HD=%016d;", sBody.GetLength());
		
		sprintf(st_client[m_nArrayPos].chSendHead, sHead);
		sprintf(st_client[m_nArrayPos].chSendBody, sBody);
		
		sFunction = "SUPPLY_CNT";
		sprintf((st_client[m_nArrayPos].stComm[nzIdx]).chSendCmd, sFunction);
		sFunction = "SUPPLY_CNT";
		sprintf((st_client[m_nArrayPos].stComm[nzIdx]).chResponse, sFunction);
		break;

		// yangkyu [2015.03.19]
		// 통신 프로토콜 추가
	case MAIN_ACCY_MATH_CODE:
		sTemp.Empty();
		sBody.Format("FUNCTION_RPY=REQ_ACCY_MATH_CODE;");
		sHead.Format("HD=%016d;", sBody.GetLength());
		
		sprintf(st_client[m_nArrayPos].chSendHead, sHead);
		sprintf(st_client[m_nArrayPos].chSendBody, sBody);
		
		sFunction = "REQ_ACCY_MATH_CODE";
		sprintf((st_client[m_nArrayPos].stComm[nzIdx]).chSendCmd, sFunction);
		sFunction = "REQ_ACCY_MATH_CODE";
		sprintf((st_client[m_nArrayPos].stComm[nzIdx]).chResponse, sFunction);
		break;
	}
}



//==================================================================//
// 특정한 문자열 사이의 데이터 반환
//==================================================================//
CString CMainEvent::OnFindInRcvMsg(CString szFind, CString szOrg, CString szDelimiter) 
{
	CString sRetChar;	// 리턴할 문자열 저장 변수
	
	int nOrgLen		= szOrg.GetLength();	// 입력된 문자열의 전체 길이
	// 전체 문자열 중에서 찾고자 하는 문자열의 시작 위치 검색
	int nFindPos	= szOrg.Find(szFind);
	int nFindLen	= szFind.GetLength();	// 찾을 문자열의 총 길이 계산
	// 데이터 사이의 구분자 위치 검색
	// - 찾고자 하는 문자열의 다음 위치부터 찾기 시작함
	//int nEndPos	= szOrg.Find(szDelimiter, nFindPos+1);
	int nEndPos	= szOrg.Find(szDelimiter, (nFindPos+nFindLen));
	// 오류 조건 확인
	//if ((nFindPos<0 || nEndPos<0) || (nEndPos <= (nFindPos+nFindLen)))
	// 규이리 수정 [2014.11.24]
	// 마지막 위치의 정보는 구분자가 존재하지 않아 수정
	if (nFindPos<0 ||
		(nEndPos >= 0 && (nEndPos < (nFindPos+nFindLen)+1)))
	{
		sRetChar = "";
	}
	// 마지막 정보는 구분자가 없음
	else if (nEndPos<0)
	{
		sRetChar = szOrg.Mid(nFindPos+nFindLen, nOrgLen-(nFindPos+nFindLen));
	}
	else
	{
		// [검색문자열~구분자] 사이의 정보 설정
		sRetChar = szOrg.Mid(nFindPos+nFindLen, nEndPos-(nFindPos+nFindLen));
	}
	return sRetChar;
}
