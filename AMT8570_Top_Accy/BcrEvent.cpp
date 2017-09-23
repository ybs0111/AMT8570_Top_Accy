// BcrEvent.cpp: implementation of the CBcrEvent class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BcrEvent.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CBcrEvent	evBCR;

CBcrEvent::CBcrEvent()
{

}

CBcrEvent::~CBcrEvent()
{

}

//==================================================================//
// 쓰레드 동작
//==================================================================//
void CBcrEvent::OnSeq_RUN(void) 
{
}

//==================================================================//
// 쓰레드 관련 변수 초기화
//==================================================================//
void CBcrEvent::OnSeq_Init(void) 
{

}

//==================================================================//
// 쓰레드 제어
//==================================================================//
void CBcrEvent::OnSeq_Execute(void) 
{
}

//==================================================================//
// [EQP<-BCR] 메시지 수신
//==================================================================//
void CBcrEvent::OnRcvDataFromBCR(WPARAM ch, LPARAM port) 
{
	// 포트 정보 얻음
	int nPort = (int)port;
	int nLen;
	int FuncRet = RET_PROCEED;

	// 마지막 문자 입력 [CR] 입력인지 확인
	if (ch == 0x0A)
	{
		m_sRcvedData[nPort -1] += (char)ch;
		//m_sRcvedData[nPort-1].TrimLeft(' ');
		//m_sRcvedData[nPort-1].TrimRight(' ');
		st_serial.comm_rec[nPort -1] = m_sRcvedData[nPort -1];
		(m_sRcvedData[nPort -1]).Empty();

		m_sLogMsg.Format( "[RCV]BCR:%s", st_serial.comm_rec[nPort -1]);
		cLOG.OnLogEvent(LOG_COMM_, m_sLogMsg);

		// 김양규 추가 [2015.05.19]
		// BCR Test Mode 일경우에는 화면에 출력 후 끝내도록 함.
		if (st_work.nModeBCR_Test == TRUE)
		{
			st_work.nModeBCR_Test = FALSE;
			// 화면에 수신 메시지 출력 요청
			if (st_handler.cwnd_list != NULL)
			{
				st_other.str_op_msg = st_serial.comm_rec[nPort -1];
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, BCR_PORT, RECEIVE_MSG);
			}
			return;
		}

		// 수신 메시지의 전체 길이 계산
		// 최소 3글자 이상 수신되어야 정상적인 메시지로 인정
		nLen = (st_serial.comm_rec[nPort -1]).GetLength();
		if (nLen > 2)
		{
			// CR+LF 제거한 문자열만 저장함
			st_serial.comm_rec[nPort -1] = (st_serial.comm_rec[nPort-1]).Mid(0, nLen-2);

			/*
			OnSet_2Struct(nPort);	// 구조체에 수신된 정보 설정
			st_serial.n_rec_chk[nPort -1] = TRUE;
			*/
			// 규이리 수정 [2015.01.03]
			if (st_serial.comm_rec[nPort -1] != "No-Read")
			{
				FuncRet = OnSet_2Struct(nPort);	// 구조체에 수신된 정보 설정
				if (FuncRet == CTL_YES)
				{
					st_serial.n_rec_chk[nPort -1] = TRUE;
				}
				else
				{
					st_serial.n_rec_chk[nPort -1] = FALSE;
				}
			}
			else
			{
				st_serial.n_rec_chk[nPort -1] = COM_ERROR;
			}
		}
		else
		{
			st_serial.n_rec_chk[nPort -1] = COM_ERROR;
		}

		// 화면에 수신 메시지 출력 요청
		if (st_handler.cwnd_list != NULL)
		{
			st_other.str_op_msg = st_serial.comm_rec[nPort -1];
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, BCR_PORT, RECEIVE_MSG);
		}
	}
	else
	{
		
		m_sRcvedData[port -1] += (char)ch;
		//kwlee 2017.0825
		if (ch == 0x03)
		{
			st_serial.comm_rec[nPort -1] = m_sRcvedData[nPort -1];
			(m_sRcvedData[nPort -1]).Empty();
		}
	}
}

//==================================================================//
// 구조체에 수신된 정보 설정
//==================================================================//
int CBcrEvent::OnSet_2Struct(int nzPort) 
{
	int nRet = CTL_NO;	// 반환 인자 설정
	CString sDataBcr = st_serial.comm_rec[nzPort -1];

	//==============================================================//
	// 규이리 추가 [2015.03.20]
	// 메시지가 2개로 구성되어 있음 (a;b;)
	// - 앞에 정보는 악세사리 매칭 코드이고 뒤의 정보는 악세사리 박스 코드임
	//==============================================================//
	CString sAcceMatCode;
	int iStart=0, iEnd;

	iEnd = sDataBcr.Find(';', iStart);
	if (iEnd == -1)
	{
		sAcceMatCode = sDataBcr;
	}
	else
	{
		sAcceMatCode = sDataBcr.Mid(iStart, (iEnd - iStart));
	}
	//==============================================================//

	//if (st_work.sBCR_Data[0] == NULL && st_map.nBCR_State[0] != BCR_READ_BAD_)
	// 규이리 수정 [2015.03.20]
	if (st_work.sRbtBCR_Data[0] == "" && st_map.nBCR_State[0] != BCR_READ_BAD_)
	{
		//st_work.sBCR_Data[0] = sDataBcr;
		// 규이리 수정 [2015.03.20]
		st_work.sRbtBCR_Data[0] = sAcceMatCode;
		nRet = CTL_YES;
		// kilee [2015.03.23]
		return nRet;
	}
	//else if (st_work.sBCR_Data[1] == NULL && st_map.nBCR_State[1] != BCR_READ_BAD_)
	// 규이리 수정 [2015.03.20]
	else if (st_work.sRbtBCR_Data[1] == "" && st_map.nBCR_State[1] != BCR_READ_BAD_)
	{
		//st_work.sBCR_Data[1] = sDataBcr;
		// 규이리 수정 [2015.03.20]
		st_work.sRbtBCR_Data[1] = sAcceMatCode;
		nRet = CTL_YES;
		// kilee [2015.03.23]
		return nRet;
	}
	//else if (st_work.sBCR_Data[3] == NULL && st_map.nBCR_State[3] != BCR_READ_BAD_)
	// 규이리 수정 [2015.03.20]
	else if (st_work.sRbtBCR_Data[3] == "" && st_map.nBCR_State[3] != BCR_READ_BAD_)
	{
		//st_work.sBCR_Data[3] = sDataBcr;
		// 규이리 수정 [2015.03.20]
		st_work.sRbtBCR_Data[3] = sAcceMatCode;
		nRet = CTL_YES;
		// kilee [2015.03.23]
		return nRet;
	}
	//else if (st_work.sBCR_Data[2] == NULL && st_map.nBCR_State[2] != BCR_READ_BAD_)
	// 규이리 수정 [2015.03.20]
	else if (st_work.sRbtBCR_Data[2] == "" && st_map.nBCR_State[2] != BCR_READ_BAD_)
	{
		//st_work.sBCR_Data[2] = sDataBcr;
		// 규이리 수정 [2015.03.20]
		st_work.sRbtBCR_Data[2] = sAcceMatCode;
		nRet = CTL_YES;
	}
	return nRet;
}