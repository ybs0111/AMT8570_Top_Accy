// Seq_BufferAlignConv.cpp: implementation of the CSeq_BufferAlignConv class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "handler.h"
#include "Seq_BufferAlignConv.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CSeq_BufferAlignConv	SeqBufferAlignConv;

CSeq_BufferAlignConv::CSeq_BufferAlignConv()
{
	//m_nTestSupply = TRUE;
	m_nTestSupply = FALSE;
	m_nReqMidConv = CTL_NO;
	m_nReqOutConv = CTL_NO;
}

CSeq_BufferAlignConv::~CSeq_BufferAlignConv()
{
	OnMove_AcMotor(AC_MV_STOP_);	// AC ���� ����/����
}


//==================================================================//
// ������ ����
//==================================================================//
void CSeq_BufferAlignConv::OnSeq_RUN(void) 
{
	OnSeq_INIT();
	OnSeq_Execute();
}

//==================================================================//
// ������ ���� ���� �ʱ�ȭ
//==================================================================//
void CSeq_BufferAlignConv::OnSeq_INIT(void) 
{
	if( m_bflag_INIT == true )
	{
		return;
	}
	m_bflag_INIT = true;

	m_nStep_Init = 0;
	m_nStep_Run = 0;
	m_nStep_RunMiddle = 0;
	m_nStep_RunReady = 0;
	m_nStep_ForceDischarge = 0;
	m_nReqMidConv = CTL_NO;
	m_nReqOutConv = CTL_NO;

	m_nReq_Completemsg = FALSE;
}

//==================================================================//
// ������ ����
//==================================================================//
void CSeq_BufferAlignConv::OnSeq_Execute(void) 
{
	// ������ ���ͷ�Ʈ ���� Ȯ��
	if (Func.OnIsInterruptCondition() == 1)
	{
		return;
	}
	int nRet = 0;
	
	//==============================================================//
	// AC ���͸� ���� ���¿� ������� �����ϱ� ���Ͽ� �߰���
	// : �̰������� TIME OUT ó�� ���� ����
	// : ���� ���°� START ���·� ����Ǹ� ��ٷ� TIME OUT �˻��ϱ� ����
	//==============================================================//
// 	if (st_work.nEqpStatus != dRUN)
// 	{
// 		// ���� �ð����� �����̾� �����ϴ� ���ۿ����� ���� �ð��� �������� ����
// 		// �� �� ���ۿ����� ���� �ð��� ������
// 		// - ���� ��ġ���� �������� �ð��� ó������ �ٽ� �����ؾ� �ϱ� ����
// 		// - �ȱ׷��� �ð��� �� ������ �������Ƿ� ��ٷ� TIME OUT ���� �߻���
// 		if (m_nReq_AcMotCtrl != REQ_MV_INIT_)
// 		{
// 			// [RUN] ���·� ����Ǳ� ���� �� ������ �ð� ���� ����
// 			// - �� ���Ҵ� [RUN] ���·� ����Ǳ���� ��� ���ŵ� ����
// 			m_lTime_GoesBy[0] = GetCurrentTime();
// 		}
// 	}
// 	if (m_nReq_AcMotCtrl == REQ_MV_ACCY_OUT_)			// [Accy Conv] ù��° ��ġ�� ���� ���� ��û�� ���
// 	{
// 		nRet = OnCheck_OutAccySupply(IO_ON);	// [SIDE_FIRST_] �Ǽ��縮 ���ۿ� ���簡 �����ϴ��� Ȯ��
// 		if (nRet == IO_ON)	// ������ ������
// 		{
// 			m_lTime_GoesBy[0] = GetCurrentTime();
// 			m_nReq_AcMotCtrl = REQ_MV_STABILITY_;
// 		}
// 	}
// 	else if (m_nReq_AcMotCtrl == REQ_MV_STABILITY_)	// ���� �ð���ŭ ���ƴ޶�� ��û�� ���
// 	{
// 		m_lTime_GoesBy[1] = GetCurrentTime();
// 		m_lTime_GoesBy[2] = m_lTime_GoesBy[1] - m_lTime_GoesBy[0];
// 		if (m_lTime_GoesBy[2] < 0)
// 		{
// 			m_lTime_GoesBy[0] = GetCurrentTime();
// 		}
// 		
// 		if (m_lTime_GoesBy[2] >= st_time.nWait_On[WAIT_ACCY_OUT_AC_OFF])	// ���� �ð� �Ŀ� AC ���� ������Ŵ
// 		{
// 			nRet = OnCheck_OutAccySupply(IO_ON);	// [SIDE_FIRST_] �Ǽ��縮 ���ۿ� ���簡 �����ϴ��� Ȯ��
// 			if (nRet == IO_OFF)	// ������ ������
// 			{
// 				OnMove_AcMotor(AC_MV_STOP_);	// AC ���� ����/����
// 			}
// 			
// 		}
// 	}
// 	// �ʱ�ȭ �۾� �� ���� ���� ��û�� ���
// 	else if (m_nReq_AcMotCtrl == REQ_MV_INIT_)
// 	{
// 		m_lTime_GoesBy[1] = GetCurrentTime();
// 		m_lTime_GoesBy[2] = m_lTime_GoesBy[1] - m_lTime_GoesBy[0];
// 		if (m_lTime_GoesBy[2] < 0)
// 		{
// 			m_lTime_GoesBy[0] = GetCurrentTime();
// 			return;
// 		}
// 		
// 		if (m_lTime_GoesBy[2] > 5000)
// 		{
// 			OnMove_AcMotor(AC_MV_STOP_);	// AC ���� ����/����
// 		}
// 	}
// 
// 	switch(st_work.nEqpStatus)
// 	{
// 	case dINIT:
// 		// �ʱ�ȭ ���� ��ȭ���ڿ��� [YES] �����ߴ��� ����
// 		if (st_handler.nInitTryREQ != YES)
// 		{
// 			break;
// 		}
// 		OnRun_Initial();  // BufferAlignConv �ʱ�ȭ �۾�
// 		break;
// 		
// 	case dRUN:
// 		// �ʱ�ȭ �۾��� �Ϸ���� ������ [AUTO] �۾� ���� �Ұ���
// 		// st_handler.nIsAllSiteInitEnd : �ʱ�ȭ ȭ�鿡�� ����
// 		if (st_handler.nIsAllSiteInitEnd == CTL_YES)
// 		{
// // 			OnRun_ReadyMove();
// // 			OnRun_MiddleMove();
// // 			OnRun_Move();
// 		}
// 		break;
// 		
// 	default :
// 		OnRun_ForceDischarge();
// 		break;
// 	}
}

void CSeq_BufferAlignConv::OnRun_Initial() 
{
	// �ʱ�ȭ �۾��� �Ϸ�� ��� ���� ������
	if (st_handler.mn_init_state[INIT_BUFFER_CONV] != CTL_NO)
	{
		return;
	}
	
	int nRet = VAR_INIT_;
	int nRetData[4] = {0,};

	switch(m_nStep_Init)
	{
	case 0:
		memset(st_map.nBufferCovAccyExist, 0, sizeof(st_map.nBufferCovAccyExist));
		memset(st_map.nBufferMiddleAccyExist, 0, sizeof(st_map.nBufferMiddleAccyExist));
		memset(st_map.nBufferOutAccyExist, 0, sizeof(st_map.nBufferOutAccyExist));
		// XYZ Robot�� ������ ��û�� Reset �� ���� Step ����
		stSync.nResp_BufferAlaignConv2XYZRbt_Work = SYNC_RESP_RESET_;
		m_nStep_Init = 200;
		break;
	
	case 200:
		if( st_basic.nMode_Slop_Variable == CTL_YES )
		{
			OnSet_CylBufferFrontTrayPitch(IO_ON);
			OnSet_CylBufferRearTrayPitch(IO_ON);
		}
		else
		{
			OnSet_CylBufferFrontTrayPitch(IO_OFF);
			OnSet_CylBufferRearTrayPitch(IO_OFF);
		}
		m_nStep_Init = 210;
		break;

	case 210:
		if( st_basic.nMode_Slop_Variable == CTL_YES )
		{
			nRetData[0] = OnGet_CylBufferFrontTrayPitch( IO_ON );
			nRetData[1] = OnGet_CylBufferRearTrayPitch( IO_ON );
			if( nRetData[0] == CTL_GOOD && nRetData[1] == CTL_GOOD )
			{
				m_nStep_Init = 300;
			}
			else if( nRetData[0] == CTL_ERROR || nRetData[1] == CTL_ERROR )
			{
				CTL_Lib.Alarm_Error_Occurrence( 4000, CTL_dWARNING, alarm.mstr_code);
			}
		}
		else
		{
			nRetData[0] = OnGet_CylBufferFrontTrayPitch( IO_OFF );
			nRetData[1] = OnGet_CylBufferRearTrayPitch( IO_OFF );
			if( nRetData[0] == CTL_GOOD && nRetData[1] == CTL_GOOD )
			{
				m_nStep_Init = 300;
			}
			else if( nRetData[0] == CTL_ERROR || nRetData[1] == CTL_ERROR )
			{
				CTL_Lib.Alarm_Error_Occurrence( 4000, CTL_dWARNING, alarm.mstr_code);
			}
		}
		break;

	case 300:
		OnMove_AcMotor(AC_MV_STOP_);
		m_nStep_Init = 310;
		break;

	case 310:
		OnSet_CylBufferTrayPathAlign(IO_ON);//Slop
		m_nStep_Init = 320;
		break;

	case 320:
		nRet = OnGet_CylBufferTrayPathAlign(IO_ON);
		if(nRet == CTL_GOOD )
		{
			m_nStep_Init = 330;
		}
		else if(nRet == CTL_ERROR)
		{			
			CTL_Lib.Alarm_Error_Occurrence( 4000, CTL_dWARNING, alarm.mstr_code );
			m_nStep_Init = 310;
		}
		break;

	case 330:
		OnSet_CylStopper(IO_ON, SIDE_FRONT_);
		OnSet_CylStopper(IO_ON, SIDE_REAR_);
		OnSet_CylStopper(IO_ON, SIDE_MIDDLE);
		OnSet_CylStopper(IO_ON, SIDE_OUT_FRONT);
		OnSet_CylStopper(IO_ON, SIDE_OUT_REAR);
		m_nStep_Init = 340;
		break;

	case 340:
		nRet = OnGet_CylStopper( IO_ON, SIDE_FRONT_);
		nRetData[0] = OnGet_CylStopper( IO_ON, SIDE_REAR_);
		nRetData[1] = OnGet_CylStopper( IO_ON, SIDE_MIDDLE);
		nRetData[2] = OnGet_CylStopper( IO_ON, SIDE_OUT_FRONT);
		nRetData[3] = OnGet_CylStopper( IO_ON, SIDE_OUT_REAR);

		if( nRet == CTL_GOOD && nRetData[0] == CTL_GOOD && 
			nRetData[1] == CTL_GOOD && nRetData[2] == CTL_GOOD && nRetData[3] == CTL_GOOD )
		{
			m_nStep_Init = 400;
		}
		else if( nRet == CTL_ERROR || nRetData[0] == CTL_ERROR || nRetData[1] == CTL_ERROR ||
			nRetData[2] == CTL_ERROR || nRetData[3] == CTL_ERROR)
		{
			CTL_Lib.Alarm_Error_Occurrence( 4000, CTL_dWARNING, alarm.mstr_code );
			m_nStep_Init = 330;
		}
		break;

	case 400:
		// �ʱ�ȭ ȭ�鿡�� [�ʱ�ȭ] �Ϸ� ǥ�� ����
		stSync.nResp_BufferAlaignConv2XYZRbt_Work = SYNC_RESP_RESET_;
		st_handler.mn_init_state[INIT_BUFFER_CONV] = CTL_YES;
		m_nStep_Init = 0;
		break;	
	}
}

void CSeq_BufferAlignConv::OnRun_MiddleMove()
{
	if (Func.OnIsAllRcvyComplete() != CTL_YES)
	{
		return;
	}
	int nRet = VAR_INIT_;
	int nRetData[4] = {0,};
	int i = 0;
	CString sTmp;
	
// 	Func.OnTrace_ThreadStep(21, m_nStep_RunMiddle);
// 	switch(m_nStep_RunMiddle)
// 	{
// 	case 0:
// 		if (st_map.nLotStart != CTL_YES)
// 		{
// 			break;
// 		}
// 		st_work.nSkipReq_BufferConv[SIDE_MIDDLE] = CTL_NO; 
// 		m_nStep_RunMiddle = 100;
// 		break;
// 
// 	case 100:		
// // 		125000 0 00 "PS1006_SLOP_MIDDLE_LEFT_ACCY_DETECTION_ON_CHK_ERR."
// // 		125001 0 00 "PS1007_SLOP_MIDDLE_LEFT_ACCY_DETECTION_OFF_CHK_ERR."
// // 		125100 0 00 "PS1006_SLOP_MIDDLE_RIGHTT_ACCY_DETECTION_ON_CHK_ERR."
// // 		125101 0 00 "PS1007_SLOP_MIDDLE_RIGHT_ACCY_DETECTION_OFF_CHK_ERR."
// 		nRetData[0] = g_ioMgr.get_in_bit( stIO.i_Chk_BufferTryAccyDetection[SIDE_MIDDLE][SIDE_LEFT_], IO_OFF );
// 		nRetData[1] = g_ioMgr.get_in_bit(stIO.i_Chk_BufferTryAccyDetection[SIDE_MIDDLE][SIDE_RIGHT_], IO_OFF );
// 		if( nRetData[0] == IO_OFF && nRetData[1] == IO_OFF )
// 		{
// 			m_nStep_RunMiddle = 1000;
// 		}
// 		else
// 		{
// 			if( nRetData[0] == IO_ON ) alarm.mstr_code = "125000";
// 			else					   alarm.mstr_code = "125100";
// 			CTL_Lib.Alarm_Error_Occurrence( 4000, CTL_dWARNING, alarm.mstr_code );
// 		}
// // 		nRet = OnCheck_MiddleAccySupply( IO_OFF );
// // 		if( nRet == IO_OFF )
// // 		{
// // 			m_nStep_RunMiddle = 1000;
// // 		}
// // 		else if( nRet == IO_ON )
// // 		{
// // 			CTL_Lib.Alarm_Error_Occurrence( 4000, CTL_dWARNING, alarm.mstr_code );
// // 		}
// 		break;
// 
// 	case 1000:
// 		OnSet_CylStopper( IO_ON, SIDE_MIDDLE);
// 		m_nStep_RunMiddle = 1100;
// 		break;
// 
// 	case 1100:
// 		nRet = OnGet_CylStopper( IO_ON, SIDE_MIDDLE );
// 		if( nRet == CTL_GOOD )
// 		{
// 			m_nStep_RunMiddle = 1200;
// 		}
// 		else if( nRet == CTL_ERROR )
// 		{
// 			CTL_Lib.Alarm_Error_Occurrence( 4000, CTL_dWARNING, alarm.mstr_code );
// 			m_nStep_RunMiddle = 1000;
// 		}
// 		break;
// 
// 	case 1200:
// 		m_nReqMidConv = CTL_REQ;
// 		m_nStep_RunMiddle = 1300;
// 		break;
// 
// 	case 1300:
// 		if( m_nReqMidConv == CTL_READY )
// 		{
// 			m_lTime_GoesBy[1][0] = GetCurrentTime();
// 			m_nStep_RunMiddle = 1400;
// 		}
// 		break;
// 
// 	case 1400:
// 		if (st_work.nSkipReq_BufferConv[SIDE_MIDDLE] == CTL_YES)
// 		{
// 			st_map.nBufferMiddleAccyExist[0] = ACCY_NO_;
// 			st_map.nBufferMiddleAccyExist[1] = ACCY_NO_;
// 			st_work.sBufferMiddleConvBCR_Data[0] = "";
// 			st_work.sBufferMiddleConvBCR_Data[1] = "";
// 			st_work.nSkipReq_BufferConv[SIDE_MIDDLE] = CTL_NO;
// 			m_nReqMidConv = CTL_CHANGE;
// 			m_nStep_Run = 1600;
// 			break;
// 		}
// 
// 		nRetData[0] = g_ioMgr.get_in_bit( stIO.i_Chk_BufferTryAccyDetection[SIDE_MIDDLE][SIDE_LEFT_], IO_OFF );
// 		nRetData[1] = g_ioMgr.get_in_bit(stIO.i_Chk_BufferTryAccyDetection[SIDE_MIDDLE][SIDE_RIGHT_], IO_OFF );
// 		if( nRetData[0] == IO_ON && nRetData[1] == IO_ON )
// 		{
// 			m_nReqMidConv = CTL_CHANGE;
// 			m_nStep_RunMiddle = 1500;
// 			m_lTime_GoesBy[1][0] = GetCurrentTime();
// 		}
// 		else
// 		{
// // 		125000 0 00 "PS1006_SLOP_MIDDLE_LEFT_ACCY_DETECTION_ON_CHK_ERR."
// // 		125001 0 00 "PS1007_SLOP_MIDDLE_LEFT_ACCY_DETECTION_OFF_CHK_ERR."
// // 		125100 0 00 "PS1006_SLOP_MIDDLE_RIGHTT_ACCY_DETECTION_ON_CHK_ERR."
// // 		125101 0 00 "PS1007_SLOP_MIDDLE_RIGHT_ACCY_DETECTION_OFF_CHK_ERR."
// 			m_lTime_GoesBy[1][1] = GetCurrentTime();
// 			m_lTime_GoesBy[1][2] = m_lTime_GoesBy[1][1] - m_lTime_GoesBy[1][0];
// 			if( m_lTime_GoesBy[1][2] <= 0 ) m_lTime_GoesBy[1][0] = GetCurrentTime();
// 			if( m_lTime_GoesBy[1][2] < 5000) break;
// 			if( nRetData[0] == IO_OFF ) alarm.mstr_code = "125001";
// 			else					    alarm.mstr_code = "125101";
// 			CTL_Lib.Alarm_Error_Occurrence( 4000, CTL_dWARNING, alarm.mstr_code );
// 		}
// 		break;
// 
// 	case 1500:
// 		if (st_work.nSkipReq_BufferConv[SIDE_MIDDLE] == CTL_YES)
// 		{
// 			st_map.nBufferMiddleAccyExist[0] = ACCY_NO_;
// 			st_map.nBufferMiddleAccyExist[1] = ACCY_NO_;
// 			st_work.sBufferMiddleConvBCR_Data[0] = "";
// 			st_work.sBufferMiddleConvBCR_Data[1] = "";
// 			st_work.nSkipReq_BufferConv[SIDE_MIDDLE] = CTL_NO;
// 			m_nReqMidConv = CTL_CHANGE;
// 			m_nStep_Run = 1600;
// 			break;
// 		}
// 		if( m_nReqMidConv == CTL_NO )
// 		{
// 			m_nStep_RunMiddle = 2000;
// 		}
// 		else
// 		{
// 			m_lTime_GoesBy[1][1] = GetCurrentTime();
// 			m_lTime_GoesBy[1][2] = m_lTime_GoesBy[1][1] - m_lTime_GoesBy[1][0];
// 			if( m_lTime_GoesBy[1][2] <= 0 ) m_lTime_GoesBy[1][0] = GetCurrentTime();
// 			if( m_lTime_GoesBy[1][2] < 5000) break;
// 		}
// 		break;
// 
// 	case 1600:
// 		if( m_nReqMidConv == CTL_NO )
// 		{
// 			m_nStep_RunMiddle = 1700;
// 		}
// 		break;
// 
// 	case 1700:
// 		nRetData[0] = g_ioMgr.get_in_bit( stIO.i_Chk_BufferTryAccyDetection[SIDE_MIDDLE][SIDE_LEFT_], IO_OFF );
// 		nRetData[1] = g_ioMgr.get_in_bit(stIO.i_Chk_BufferTryAccyDetection[SIDE_MIDDLE][SIDE_RIGHT_], IO_OFF );
// 		if( nRetData[0] == IO_OFF && nRetData[1] == IO_OFF )
// 		{
// 			m_nStep_RunMiddle = 5000;
// 		}
// 		else
// 		{
// 			m_lTime_GoesBy[1][1] = GetCurrentTime();
// 			m_lTime_GoesBy[1][2] = m_lTime_GoesBy[1][1] - m_lTime_GoesBy[1][0];
// 			if( m_lTime_GoesBy[1][2] <= 0 ) m_lTime_GoesBy[1][0] = GetCurrentTime();
// 			if( m_lTime_GoesBy[1][2] < 5000) break;
// 			if( nRetData[0] == IO_ON ) alarm.mstr_code = "125000";
// 			else					    alarm.mstr_code = "125100";
// 			CTL_Lib.Alarm_Error_Occurrence( 4000, CTL_dWARNING, alarm.mstr_code );
// 		}
// 		break;
// 
// 	case 2000:
// 		if( m_nReqOutConv == CTL_REQ )
// 		{
// 			m_nStep_RunMiddle = 2100;
// 		}
// 		break;
// 
// 	case 2100:
// 		OnSet_CylStopper( IO_OFF, SIDE_MIDDLE );
// 		m_nStep_RunMiddle = 2200;
// 		break;
// 
// 	case 2200:
// 		nRet = OnGet_CylStopper( IO_OFF, SIDE_MIDDLE );
// 		if( nRet == CTL_GOOD )
// 		{
// 			m_lTime_GoesBy[1][0] = GetCurrentTime();
// 			m_nStep_RunMiddle = 2300;
// 		}
// 		else if( nRet == CTL_ERROR)
// 		{
// 			CTL_Lib.Alarm_Error_Occurrence( 4000, CTL_dWARNING, alarm.mstr_code );
// 			m_nStep_RunMiddle = 2100;
// 		}
// 		break;
// 
// 	case 2300:
// 		nRetData[0] = g_ioMgr.get_in_bit( stIO.i_Chk_BufferTryAccyDetection[SIDE_MIDDLE][SIDE_LEFT_], IO_OFF );
// 		nRetData[1] = g_ioMgr.get_in_bit(stIO.i_Chk_BufferTryAccyDetection[SIDE_MIDDLE][SIDE_RIGHT_], IO_OFF );
// 		if( nRetData[0] == IO_OFF && nRetData[1] == IO_OFF )
// 		{
// 			m_nStep_RunMiddle = 2400;
// 			m_lTime_GoesBy[1][0] = GetCurrentTime();
// 		}
// 		else
// 		{
// 			m_lTime_GoesBy[1][1] = GetCurrentTime();
// 			m_lTime_GoesBy[1][2] = m_lTime_GoesBy[1][1] - m_lTime_GoesBy[1][0];
// 			if( m_lTime_GoesBy[1][2] <= 0 ) m_lTime_GoesBy[1][0] = GetCurrentTime();
// 			if( m_lTime_GoesBy[1][2] < 5000) break;
// 			if( nRetData[0] == IO_ON ) alarm.mstr_code = "125000";
// 			else					   alarm.mstr_code = "125100";
// 			CTL_Lib.Alarm_Error_Occurrence( 4000, CTL_dWARNING, alarm.mstr_code );
// 		}
// 		break;
// 
// 	case 2400:
// 		m_lTime_GoesBy[1][1] = GetCurrentTime();
// 		m_lTime_GoesBy[1][2] = m_lTime_GoesBy[1][1] - m_lTime_GoesBy[1][0];
// 		if( m_lTime_GoesBy[1][2] <= 0 ) m_lTime_GoesBy[1][0] = GetCurrentTime();
// 		if( m_lTime_GoesBy[1][2] < 1000) break;
// 
// 		OnSet_CylStopper( IO_ON, SIDE_MIDDLE );
// 		m_nReqOutConv = CTL_READY;
// 		m_nStep_RunMiddle = 2500;
// 		break;
// 
// 	case 2410:
// 		OnSet_CylStopper( IO_ON, SIDE_MIDDLE );
// 		m_nStep_RunMiddle = 2500;
// 		break;
// 
// 	case 2500:
// 		nRet = OnGet_CylStopper( IO_ON, SIDE_MIDDLE );
// 		if( nRet == CTL_GOOD )
// 		{
// 			m_nStep_RunMiddle = 2600;
// 		}
// 		else if( nRet == CTL_ERROR)
// 		{
// 			CTL_Lib.Alarm_Error_Occurrence( 4000, CTL_dWARNING, alarm.mstr_code );
// 			m_nStep_RunMiddle = 2410;
// 		}
// 		break;
// 
// 	case 2600:
// 		nRetData[0] = g_ioMgr.get_in_bit( stIO.i_Chk_BufferTryAccyDetection[SIDE_MIDDLE][SIDE_LEFT_], IO_OFF );
// 		nRetData[1] = g_ioMgr.get_in_bit(stIO.i_Chk_BufferTryAccyDetection[SIDE_MIDDLE][SIDE_RIGHT_], IO_OFF );
// 		if( nRetData[0] == IO_OFF && nRetData[1] == IO_OFF )
// 		{
// 			m_nStep_RunMiddle = 3000;
// 			m_lTime_GoesBy[1][0] = GetCurrentTime();
// 		}
// 		else
// 		{
// 			m_lTime_GoesBy[1][1] = GetCurrentTime();
// 			m_lTime_GoesBy[1][2] = m_lTime_GoesBy[1][1] - m_lTime_GoesBy[1][0];
// 			if( m_lTime_GoesBy[1][2] <= 0 ) m_lTime_GoesBy[1][0] = GetCurrentTime();
// 			if( m_lTime_GoesBy[1][2] < 5000) break;
// 			if( nRetData[0] == IO_ON ) alarm.mstr_code = "125000";
// 			else					   alarm.mstr_code = "125100";
// 			CTL_Lib.Alarm_Error_Occurrence( 4000, CTL_dWARNING, alarm.mstr_code );
// 		}
// 		break;
// 
// 	case 3000:
// 		if( m_nReqOutConv == CTL_CHANGE || m_nReqOutConv == CTL_NO)
// 		{
// 			m_nStep_RunMiddle = 0;
// 		}
// 		break;
// 
// 	case 5000:
// 		m_nStep_RunMiddle = 0;
// 		break;
// 
// 	}
}

void CSeq_BufferAlignConv::OnRun_Move() 
{
	// ��ü ����Ʈ ���� ���� �Ϸ� ���� Ȯ��
	// - ��� �κ��� ���� ������ �Ϸ�� �Ŀ��� ������ �����ϵ��� ��
	// : ���� ���� �߿� ������ �����ϸ� �����ϴ� ������ �浹�� �߻��� �� ����
	if (Func.OnIsAllRcvyComplete() != CTL_YES)
	{
		return;
	}
	int nRet = VAR_INIT_;
	int nRetData[4] = {0,};
	int i = 0;
	CString sTmp;
	
// 	Func.OnTrace_ThreadStep(19, m_nStep_Run);
// 	switch(m_nStep_Run)
// 	{
// 	case 0:
// 		if (stSync.nLotEnd_BufferAlignConv == TRUE)
// 		{
// 			nRet = Func.OnGet_UpStreamSmema();		// [��� ����] SMEMA ���� Ȯ��
// 			if(nRet == REQ_REQUEST_ON_)
// 			{
// 				if (m_nReq_Completemsg == FALSE)
// 				{
// 					m_nReq_Completemsg = TRUE;
// 					
// 					g_client_accy.OnCreate_SendFormat(MAIN_ACCY_SUPPLY_COMPLETE);
// 					g_client_accy.OnReq_CommStart();
// 				}
// 			}
// 		}
// 		
// 		// Lot Start ���°� �ɶ����� ���
// 		if (st_map.nLotStart != CTL_YES)
// 		{
// 			break;
// 		}
// 		//////////////////////////////////////////////////////
// 		if( st_basic.nMode_Slop_Variable == CTL_YES )
// 		{
// 			OnSet_CylBufferFrontTrayPitch(IO_ON);
// 		}
// 		else
// 		{
// 			OnSet_CylBufferFrontTrayPitch(IO_OFF);
// 		}
// 		//////////////////////////////////////////////////////	
// 
// 		// XYZRobot�� Lot End ����
// 		if(	stSync.nLotEnd_XYZRobot == TRUE)
// 		{
// 			// ��� ������ Lot End �������� Conv�� Lot End ���°� �ƴҶ��� Ȯ���Ͽ� Lot End ó��
// 			if (stSync.nLotEnd_LoadingConv == TRUE && stSync.nLotEnd_BoxClamp == TRUE &&
// 				stSync.nLotEnd_BoxLifter == TRUE &&	stSync.nLotEnd_XYZRobot == TRUE &&
// 				stSync.nLotEnd_BufferAlignConv == FALSE)
// 			{
// 				st_map.nLotStart = CTL_NO;
// 				stSync.nLotEnd_BufferAlignConv = TRUE;
// 				stSync.nResp_BufferAlaignConv2XYZRbt_Work = SYNC_RESP_RESET_;
// 				m_nStep_Run = 0;
// 			}
// 			break;
// 		}
// 		else
// 		{
// 			stSync.nLotEnd_BufferAlignConv = FALSE;
// 		}
// 		
// 		if( stSync.nResp_BufferAlaignConv2XYZRbt_Work == SYNC_RESP_LOADING_)
// 		{
// 			m_nReq_Completemsg = FALSE;
// 			m_nStep_RunReady = 50;
// 		}
// 		break;
// 
// 	case 50:
// 		// [Buffer Conv -> XYZRbt] Work Complete ��û ���
// 		if(stSync.nReq_XYZRbt2BufferAlaignConv_Work == SYNC_REQ_WORK_COMPLETE_)
// 		{
// 			m_lTime_GoesBy[2][0] = GetCurrentTime();
// 			m_nStep_Run = 80;
// 		}
// 		break;
// 
// 	case 80:
// 		nRetData[0] = OnCheck_OutFrontAccySupply(IO_ON);//Out Conv Accy
// 		nRetData[1] = OnCheck_OutRearAccySupply(IO_ON);
// 		if(nRetData[0] == IO_ON && nRetData[1] == IO_ON)
// 		{
// 			m_nStep_Run = 100;
// 		}
// 		else
// 		{
// 			m_lTime_GoesBy[2][1] = GetCurrentTime();
// 			m_lTime_GoesBy[2][2] = m_lTime_GoesBy[2][1] - m_lTime_GoesBy[2][0];
// 			if (m_lTime_GoesBy[0][2] < 0)
// 			{
// 				m_lTime_GoesBy[2][0] = GetCurrentTime();
// 				break;
// 			}
// 			
// 			if (m_lTime_GoesBy[2][2] > 500)
// 			{
// 				// 150001 0 00 "ACCY_CONV_ACCY_CHK_ERR."
// 				alarm.mstr_code		= "150001";
// 				alarm.mn_count_mode	= 0;
// 				alarm.mn_type_mode	= eWARNING;
// 				st_work.nEqpStatus	= dWARNING;
// 				CTL_Lib.Alarm_Error_Occurrence( 4000, CTL_dWARNING, alarm.mstr_code );
// 			}
// 		}
// 		break;
// 
// 	case 100:
// 
// 		break;
// 		
// 	case 200:
// 		if( st_basic.nMode_Slop_Variable == CTL_YES )
// 		{
// 			OnSet_CylBufferFrontTrayPitch(IO_ON);
// 		}
// 		else
// 		{
// 			OnSet_CylBufferFrontTrayPitch(IO_OFF);
// 		}
// 		m_nStep_Run = 210;
// 		break;
// 		
// 	case 210:
// 		if( st_basic.nMode_Slop_Variable == CTL_YES )
// 		{
// 			nRetData[0] = OnGet_CylBufferFrontTrayPitch( IO_ON);
// 		}
// 		else
// 		{
// 			nRetData[0] = OnGet_CylBufferFrontTrayPitch( IO_OFF);
// 		}
// 		
// 		if( nRetData[0] == CTL_GOOD)
// 		{
// 			// �׽�Ʈ�� ���� Main�� ���� ���� �ʰ� �׽�Ʈ�� ���� �߰�
// 			if (m_nTestSupply == TRUE)
// 			{
// 				m_nStep_Run = 220;
// 				break;
// 			}
// 			
// 			if(st_basic.n_mode_device == WITHOUT_DVC_)
// 			{
// 				if(st_basic.nMode_SMEMAUsing == SMEMA_NOT_USING)
// 				{
// 					m_lTime_GoesBy[2][0] = GetCurrentTime();
// 					OnMove_AcMotor(AC_MV_CW_);	// AC ���� ����/����
// 					m_nReq_AcMotCtrl = REQ_MV_ACCY_OUT_;
// 					m_nStep_Run = 260;
// 					break;
// 				}
// 			}
// 			else
// 			{
// 				if(st_basic.nMode_SMEMAUsing == SMEMA_NOT_USING)
// 				{
// 					m_lTime_GoesBy[2][0] = GetCurrentTime();
// 					OnMove_AcMotor(AC_MV_CW_);	// AC ���� ����/����
// 					m_nReq_AcMotCtrl = REQ_MV_ACCY_OUT_;
// 					m_nStep_Run = 260;
// 					break;
// 				}
// 			}
// 			
// 			Func.OnLogBCRData("CSeq_BufferAlignConv::OnRun_Move_210");
// 			m_nStep_Run = 250;
// 
// 		}
// 		else if( nRetData[0] == CTL_ERROR )
// 		{
// 			CTL_Lib.Alarm_Error_Occurrence( 4000, CTL_dWARNING, alarm.mstr_code );
// 			m_nStep_Run = 200;
// 		}
// 		break;
// 
// 		// �׽�Ʈ�� ���� Main�� ���� ���� �ʰ� �׽�Ʈ�� ���� �߰�
// 	case 220:
// 		nRetData[0] = OnCheck_OutFrontAccySupply(IO_ON);		// Buffer Conv Front Accy Ȯ��
// 		nRetData[1] = OnCheck_OutRearAccySupply(IO_ON);		// Buffer Conv Rear Accy Ȯ��
// 		nRetData[2] = OnCheck_OutAccySupply(IO_ON);		// Buffer Conv Out Ȯ��
// 		
// 		if (nRetData[0] == IO_OFF && nRetData[1] == IO_OFF && nRetData[2] == IO_OFF)
// 		{
// 			memset(st_map.nBufferOutAccyExist, ACCY_NO_, sizeof(st_map.nBufferOutAccyExist));
// 			
// 			m_nStep_Run = 800;
// 		}
// 		break;
// 
// 	case 250:
// 		if (st_work.nSkipReq_OutBufferConv[SIDE_OUT_FRONT] == CTL_YES)
// 		{
// 			st_map.nBufferOutAccyExist[0] = ACCY_NO_;
// 			st_map.nBufferOutAccyExist[1] = ACCY_NO_;
// 			st_work.sBufferOutConvBCR_Data[0] = "";
// 			st_work.sBufferOutConvBCR_Data[1] = "";
// 			st_work.nSkipReq_OutBufferConv[SIDE_FRONT_] = CTL_NO;
// 			m_nStep_Run = 370;
// 			break;
// 		}
// 		if( st_work.nSkipReq_OutBufferConv[SIDE_OUT_FRONT] == CTL_YES &&
// 			st_work.nSkipReq_OutBufferConv[SIDE_OUT_REAR] == CTL_YES )
// 		{
// 			st_map.nBufferOutAccyExist[0] = ACCY_NO_;
// 			st_map.nBufferOutAccyExist[1] = ACCY_NO_;
// 			st_map.nBufferOutAccyExist[2] = ACCY_NO_;
// 			st_map.nBufferOutAccyExist[3] = ACCY_NO_;
// 			st_work.sBufferOutConvBCR_Data[0] = "";
// 			st_work.sBufferOutConvBCR_Data[1] = "";
// 			st_work.sBufferOutConvBCR_Data[2] = "";
// 			st_work.sBufferOutConvBCR_Data[3] = "";
// 			st_work.nSkipReq_OutBufferConv[SIDE_OUT_FRONT] = CTL_NO;
// 			st_work.nSkipReq_OutBufferConv[SIDE_OUT_REAR] = CTL_NO;
// 			
// 			m_nStep_Run = 700;
// 			break;
// 		}
// 
// 		nRet = Func.OnGet_UpStreamSmema();		// [��� ����] SMEMA ���� Ȯ��
// 		if( nRet == REQ_REQUEST_ON_ )
// 		{
// 			// SLAVE_MODE(TOP)�� ��� Accy Math �ڵ带 ���� ����.
// 			// BCR�� �����Ƿ� ���� ����.
// 			if (st_basic.nMode_SupplyMode == SLAVE_MODE)
// 			{
// 				m_nStep_Run = 255;
// 			}
// 			else
// 			{
// 				//st_work.m_sAccyMathCode = "LA69-01773A";
// 				if (st_work.m_sCurrntAccyMathCode != "")
// 				{
// 					if (st_work.m_sCurrntAccyMathCode == st_work.sBufferConvBCR_Data[0] &&
// 						st_work.m_sCurrntAccyMathCode == st_work.sBufferConvBCR_Data[1])
// 					{
// 						m_nStep_Run = 255;
// 					}
// 					else
// 					{
// 						Func.OnLogBCRData("CSeq_BufferAlignConv::OnRun_Move_250_else");
// 						// 145000 0 00 "ACCY_CONV_DATA_MISS_MATH."
// 						alarm.mstr_code		= "145000";
// 						alarm.mn_count_mode	= 0;
// 						alarm.mn_type_mode	= eWARNING;
// 						st_work.nEqpStatus	= dWARNING;
// 						CTL_Lib.Alarm_Error_Occurrence( 4000, CTL_dWARNING, alarm.mstr_code);
// 					}
// 				}
// 			}
// 		}
// 		break;
// 
// 	case 255:
// 		nRet = Func.OnGet_UpStreamSmema();		// [��� ����] SMEMA ���� Ȯ��
// 		if( nRet == REQ_REQUEST_ON_ )
// 		{
// 			Func.OnSet_UpStreamSmema( RDY_READY_ON_ );
// 			OnMove_AcMotor( AC_MV_CW_);
// 			m_lTime_GoesBy[2][0] = GetCurrentTime();
// 			m_nStep_Run = 260;
// 		}
// 		break;
// 
// 	case 260:
// 		if(st_basic.nMode_SMEMAUsing == SMEMA_NOT_USING)
// 		{
// 			m_lTime_GoesBy[2][1] = GetCurrentTime();
// 			m_lTime_GoesBy[2][2] = m_lTime_GoesBy[2][1] - m_lTime_GoesBy[2][0];
// 			if (m_lTime_GoesBy[2][2] < 0)
// 			{
// 				m_lTime_GoesBy[0] = GetCurrentTime();
// 				break;
// 			}
// 			
// 			// [WithOut] ������� Ȯ��
// 			// - ���簡 ���� ������ ������ �������� ����
// 			// - ������ ���� ���� �� �ִ� ������ �ƴϹǷ� ���� �ð� �Ŀ� ������ ����
// 			if (st_basic.n_mode_device == WITHOUT_DVC_)
// 			{
// 				if (m_lTime_GoesBy[2][2] >= (MAX_WAIT_ACCY_BOX_CONV_ / 5))
// 				{
// 					st_map.nBufferOutAccyExist[0] = ACCY_NO_;
// 					st_map.nBufferOutAccyExist[1] = ACCY_NO_;
// 					st_work.sBufferOutConvBCR_Data[0] = "";
// 					st_work.sBufferOutConvBCR_Data[1] = "";
// 					
// 					if (st_handler.cwnd_main != NULL)
// 					{
// 						// Buffer Cov ���� ���� ���� ǥ�� ��û
// 						st_handler.cwnd_main->PostMessage(WM_UPDATE_MAIN, PLACE_BUFFER_CONV_);
// 					}
// 					OnMove_AcMotor(AC_MV_STOP_);	// AC ���� ����/����
// 					m_nStep_Run = 300;
// 				}
// 			}
// 			else
// 			{
// 				if(m_nReq_AcMotCtrl == REQ_MV_RESET_)
// 				{
// 					st_map.nBufferOutAccyExist[0] = ACCY_NO_;
// 					st_map.nBufferOutAccyExist[1] = ACCY_NO_;
// 					st_work.sBufferOutConvBCR_Data[0] = "";
// 					st_work.sBufferOutConvBCR_Data[1] = "";
// 					
// 					if (st_handler.cwnd_main != NULL)
// 					{
// 						// Buffer Cov ���� ���� ���� ǥ�� ��û
// 						st_handler.cwnd_main->PostMessage(WM_UPDATE_MAIN, PLACE_BUFFER_CONV_);
// 					}
// 					m_nStep_Run = 300;
// 				}
// 			}
// 		}
// 		else
// 		{
// 			nRet = g_ioMgr.get_in_bit(stIO.i_Chk_ReqFromMain);		// [Conv<-Mani] ���� ���� �ϷḦ ��ٸ�
// 			if (nRet == IO_OFF)
// 			{
// 				OnMove_AcMotor(AC_MV_STOP_);	// AC ���� ����/����
// 				st_map.nBufferOutAccyExist[0] = ACCY_NO_;
// 				st_map.nBufferOutAccyExist[1] = ACCY_NO_;
// 				st_work.sBufferOutConvBCR_Data[0] = "";
// 				st_work.sBufferOutConvBCR_Data[1] = "";
// 				
// 				if (st_handler.cwnd_main != NULL)
// 				{
// 					// Buffer Cov ���� ���� ���� ǥ�� ��û
// 					st_handler.cwnd_main->PostMessage(WM_UPDATE_MAIN, PLACE_BUFFER_CONV_);
// 				}
// 				Func.OnSet_UpStreamSmema(RDY_READY_OFF_);	// [��� ����] SMEMA ���� ����
// 				m_nStep_Run = 300;
// 			}
// 		}
// 		break;
// 
// 	case 300:
// 		nRet = OnCheck_OutFrontAccySupply(IO_ON);		// Buffer Conv Front Accy Ȯ��
// 		
// 		if (st_basic.n_mode_device == WITHOUT_DVC_)
// 		{
// 			if (st_map.nBufferOutAccyExist[0] == ACCY_NO_ && st_map.nBufferOutAccyExist[1] == ACCY_NO_)
// 			{
// 				nRet = IO_OFF;
// 			}
// 		}
// 		
// 		if(nRet == IO_OFF && st_map.nBufferOutAccyExist[0] == ACCY_NO_ && st_map.nBufferOutAccyExist[1] == ACCY_NO_)
// 		{
// 			st_map.nResp_AccyReadyCNT -= 2;
// 			g_client_accy.OnCreate_SendFormat(MAIN_SUPPLY_READY_CNT);
// 			g_client_accy.OnReq_CommStart();
// 			m_nStep_Run = 310;
// 		}
// 		else if (nRet == IO_ON && st_map.nBufferOutAccyExist[0] == ACCY_NO_ && st_map.nBufferOutAccyExist[1] == ACCY_NO_)
// 		{
// 			// 142000 0 00 "PS1106_PS1107_PS1108_PS1109_ACCY_CONV_ACCY_EXSIT_CHK_ERR."
// 			alarm.mstr_code		= "142000";
// 			alarm.mn_count_mode	= 0;
// 			alarm.mn_type_mode	= eWARNING;
// 			st_work.nEqpStatus	= dWARNING;
// 			CTL_Lib.Alarm_Error_Occurrence( 4000, CTL_dWARNING, alarm.mstr_code );
// 		}
// 		break;
// 
// 	case 310:
// 		OnSet_CylStopper(IO_OFF, SIDE_OUT_FRONT);		// �Ǽ��縮 ������ [ON:UP, OFF:DOWN]
// 		m_nStep_Run = 320;
// 		break;
// 
// 	case 320:
// 		nRet = OnGet_CylStopper(IO_OFF, SIDE_OUT_FRONT);
// 		
// 		if(nRet == CTL_GOOD)
// 		{
// 			if(st_basic.n_mode_device == WITHOUT_DVC_)
// 			{
// 				if(st_basic.nMode_SMEMAUsing == SMEMA_NOT_USING)
// 				{
// 					m_lTime_GoesBy[2][0] = GetCurrentTime();
// 					OnMove_AcMotor(AC_MV_CW_);	// AC ���� ����/����
// 					m_nReq_AcMotCtrl = REQ_MV_ACCY_OUT_;
// 					m_nStep_Run = 380;
// 					break;
// 				}
// 			}
// 			else
// 			{
// 				if(st_basic.nMode_SMEMAUsing == SMEMA_NOT_USING)
// 				{
// 					m_lTime_GoesBy[2][0] = GetCurrentTime();
// 					OnMove_AcMotor(AC_MV_CW_);	// AC ���� ����/����
// 					m_nReq_AcMotCtrl = REQ_MV_ACCY_OUT_;
// 					m_nStep_Run = 380;
// 					break;
// 				}
// 			}
// 			m_nStep_Run = 370;
// 		}
// 		else if (nRet == CTL_ERROR)
// 		{
// 			m_nStep_Run = 310;
// 		}
// 		break;
// 
// 	case 370:
// 		if (st_work.nSkipReq_OutBufferConv[SIDE_REAR_] == CTL_YES)
// 		{
// 			st_map.nBufferOutAccyExist[2] = ACCY_NO_;
// 			st_map.nBufferOutAccyExist[3] = ACCY_NO_;
// 			st_work.sBufferOutConvBCR_Data[2] = "";
// 			st_work.sBufferOutConvBCR_Data[3] = "";
// 			st_work.nSkipReq_OutBufferConv[SIDE_REAR_] = CTL_NO;
// 			m_nStep_Run = 700;
// 			break;
// 		}
// 		
// 		nRet = Func.OnGet_UpStreamSmema();		// [��� ����] SMEMA ���� Ȯ��
// 		if(nRet == REQ_REQUEST_ON_)
// 		{
// 			// SLAVE_MODE(TOP)�� ��� Accy Math �ڵ带 ���� ����.
// 			// BCR�� �����Ƿ� ���� ����.
// 			if (st_basic.nMode_SupplyMode == SLAVE_MODE)
// 			{
// 				m_nStep_Run = 375;
// 			}
// 			else
// 			{
// 				if (st_work.m_sCurrntAccyMathCode != "")
// 				{
// 					if (st_work.m_sCurrntAccyMathCode == st_work.sBufferOutConvBCR_Data[2] &&
// 						st_work.m_sCurrntAccyMathCode == st_work.sBufferOutConvBCR_Data[3])
// 					{
// 						m_nStep_Run = 375;
// 					}
// 					else
// 					{
// 						Func.OnLogBCRData("CSeq_BufferAlignConv::OnRun_Move_370");
// 						// 145000 0 00 "ACCY_CONV_ACCY_BCR_DATA_MISS_MATH."
// 						alarm.mstr_code		= "145001";
// 						alarm.mn_count_mode	= 0;
// 						alarm.mn_type_mode	= eWARNING;
// 						st_work.nEqpStatus	= dWARNING;
// 						CTL_Lib.Alarm_Error_Occurrence( 4000, CTL_dWARNING, alarm.mstr_code );
// 					}
// 				}
// 			}
// 		}
// 		break;
// 
// 	case 375:
// 		nRet = Func.OnGet_UpStreamSmema();		// [��� ����] SMEMA ���� Ȯ��
// 		if(nRet == REQ_REQUEST_ON_)
// 		{
// 			Func.OnSet_UpStreamSmema(RDY_READY_ON_);	// [��� ����] SMEMA ���� ����
// 			OnMove_AcMotor(AC_MV_CW_);	// AC ���� ����/����
// 			m_lTime_GoesBy[2][0] = GetCurrentTime();
// 			m_nStep_Run = 380;
// 		}
// 		break;
// 		
// 	case 380:
// 		if(st_basic.nMode_SMEMAUsing == SMEMA_NOT_USING)
// 		{
// 			m_lTime_GoesBy[2][1] = GetCurrentTime();
// 			m_lTime_GoesBy[2][2] = m_lTime_GoesBy[2][1] - m_lTime_GoesBy[2][0];
// 			if (m_lTime_GoesBy[2][2] < 0)
// 			{
// 				m_lTime_GoesBy[0] = GetCurrentTime();
// 				break;
// 			}
// 			
// 			// [WithOut] ������� Ȯ��
// 			// - ���簡 ���� ������ ������ �������� ����
// 			// - ������ ���� ���� �� �ִ� ������ �ƴϹǷ� ���� �ð� �Ŀ� ������ ����
// 			if (st_basic.n_mode_device == WITHOUT_DVC_)
// 			{
// 				if (m_lTime_GoesBy[2][2] >= (MAX_WAIT_ACCY_BOX_CONV_ / 5))
// 				{
// 					st_map.nBufferOutAccyExist[2] = ACCY_NO_;
// 					st_map.nBufferOutAccyExist[3] = ACCY_NO_;
// 					st_work.sBufferOutConvBCR_Data[2] = "";
// 					st_work.sBufferOutConvBCR_Data[3] = "";
// 					
// 					if (st_handler.cwnd_main != NULL)
// 					{
// 						// Buffer Cov ���� ���� ���� ǥ�� ��û
// 						st_handler.cwnd_main->PostMessage(WM_UPDATE_MAIN, PLACE_BUFFER_CONV_);
// 					}
// 					OnMove_AcMotor(AC_MV_STOP_);	// AC ���� ����/����
// 					m_nStep_Run = 500;
// 				}
// 			}
// 			else
// 			{
// 				if(m_nReq_AcMotCtrl == REQ_MV_RESET_)
// 				{
// 					st_map.nBufferOutAccyExist[2] = ACCY_NO_;
// 					st_map.nBufferOutAccyExist[3] = ACCY_NO_;
// 					st_work.sBufferOutConvBCR_Data[2] = "";
// 					st_work.sBufferOutConvBCR_Data[3] = "";
// 					
// 					if (st_handler.cwnd_main != NULL)
// 					{
// 						// Buffer Cov ���� ���� ���� ǥ�� ��û
// 						st_handler.cwnd_main->PostMessage(WM_UPDATE_MAIN, PLACE_BUFFER_CONV_);
// 					}
// 					m_nStep_Run = 500;
// 				}
// 			}
// 		}
// 		else
// 		{
// 			nRet = g_ioMgr.get_in_bit(stIO.i_Chk_ReqFromMain);		// [Conv<-Mani] ���� ���� �ϷḦ ��ٸ�
// 			if (nRet == IO_OFF)
// 			{
// 				OnMove_AcMotor(AC_MV_STOP_);	// AC ���� ����/����
// 				st_map.nBufferOutAccyExist[2] = ACCY_NO_;
// 				st_map.nBufferOutAccyExist[3] = ACCY_NO_;
// 				st_work.sBufferOutConvBCR_Data[2] = "";
// 				st_work.sBufferOutConvBCR_Data[3] = "";
// 				
// 				if (st_handler.cwnd_main != NULL)
// 				{
// 					// Buffer Cov ���� ���� ���� ǥ�� ��û
// 					st_handler.cwnd_main->PostMessage(WM_UPDATE_MAIN, PLACE_BUFFER_CONV_);
// 				}
// 				Func.OnSet_UpStreamSmema(RDY_READY_OFF_);	// [��� ����] SMEMA ���� ����
// 				m_nStep_Run = 500;
// 			}
// 		}
// 		break;
// 
// 	case 500:
// 		nRet = OnCheck_RearAccySupply(IO_ON);		// Buffer Conv Rear Accy Ȯ��
// 		
// 		if (st_basic.n_mode_device == WITHOUT_DVC_)
// 		{
// 			if (st_map.nBufferOutAccyExist[2] == ACCY_NO_ && st_map.nBufferOutAccyExist[3] == ACCY_NO_)
// 			{
// 				nRet = IO_OFF;
// 			}
// 		}		
// 		if(nRet == IO_OFF && st_map.nBufferOutAccyExist[2] == ACCY_NO_ && st_map.nBufferOutAccyExist[3] == ACCY_NO_)
// 		{
// 			st_map.nResp_AccyReadyCNT -= 2;
// 			g_client_accy.OnCreate_SendFormat(MAIN_SUPPLY_READY_CNT);
// 			g_client_accy.OnReq_CommStart();
// 			m_nStep_Run = 700;
// 		}
// 		else if (nRet == IO_ON && st_map.nBufferOutAccyExist[2] == ACCY_NO_ && st_map.nBufferOutAccyExist[3] == ACCY_NO_)
// 		{
// 			// 142000 0 00 "PS1106_PS1107_PS1108_PS1109_ACCY_CONV_ACCY_EXSIT_CHK_ERR."
// 			alarm.mstr_code		= "142000";
// 			alarm.mn_count_mode	= 0;
// 			alarm.mn_type_mode	= eWARNING;
// 			st_work.nEqpStatus	= dWARNING;
// 			CTL_Lib.Alarm_Error_Occurrence( 4000, CTL_dWARNING, alarm.mstr_code);
// 		}
// 		break;
// 
// 	case 700:
// 		nRetData[0] = OnCheck_OutFrontAccySupply(IO_ON);		// Buffer Conv Front Accy Ȯ��
// 		nRetData[1] = OnCheck_OutRearAccySupply(IO_ON);		// Buffer Conv Rear Accy Ȯ��
// 		nRetData[2] = OnCheck_OutAccySupply(IO_ON);		// Buffer Conv Out Ȯ��
// 		
// 		if (st_basic.n_mode_device == WITHOUT_DVC_)
// 		{
// 			if (st_map.nBufferOutAccyExist[0] == ACCY_NO_ && st_map.nBufferOutAccyExist[1] == ACCY_NO_)
// 			{
// 				nRetData[0] = IO_OFF;
// 			}
// 			if (st_map.nBufferOutAccyExist[2] == ACCY_NO_ && st_map.nBufferOutAccyExist[3] == ACCY_NO_)
// 			{
// 				nRetData[1] = IO_OFF;
// 				
// 			}
// 			if (nRetData[0] == IO_OFF && nRetData[1] == IO_OFF)
// 			{
// 				nRetData[2] = IO_OFF;
// 			}
// 		}
// 		
// 		if(nRetData[0] == IO_OFF && nRetData[1] == IO_OFF && nRetData[2] == IO_OFF)
// 		{
// 			m_nStep_Run = 800;
// 		}
// 		break;
// 
// 	case 800:
// 		if( st_basic.nMode_Slop_Variable == CTL_YES )
// 		{
// 			OnSet_CylBufferFrontTrayPitch(IO_ON);
// 			OnSet_CylBufferRearTrayPitch(IO_ON);
// 		}
// 		else
// 		{
// 			OnSet_CylBufferFrontTrayPitch(IO_OFF);
// 			OnSet_CylBufferRearTrayPitch(IO_OFF);
// 		}
// 		
// 		m_nStep_Run = 810;
// 		break;
// 		
// 	case 810:
// 		if( st_basic.nMode_Slop_Variable == CTL_YES )
// 		{
// 			nRetData[0] = OnGet_CylBufferFrontTrayPitch( IO_ON );
// 			nRetData[1] = OnGet_CylBufferRearTrayPitch( IO_ON );
// 			if( nRetData[0] == CTL_GOOD && nRetData[1] == CTL_GOOD )
// 			{
// 				m_nStep_Run = 900;
// 			}
// 			else if( nRetData[0] == CTL_ERROR || nRetData[1] == CTL_ERROR )
// 			{
// 				CTL_Lib.Alarm_Error_Occurrence( 4000, CTL_dWARNING, alarm.mstr_code);
// 			}
// 		}
// 		else
// 		{
// 			nRetData[0] = OnGet_CylBufferFrontTrayPitch( IO_OFF );
// 			nRetData[1] = OnGet_CylBufferRearTrayPitch( IO_OFF );
// 			if( nRetData[0] == CTL_GOOD && nRetData[1] == CTL_GOOD )
// 			{
// 				m_nStep_Run = 900;
// 			}
// 			else if( nRetData[0] == CTL_ERROR || nRetData[1] == CTL_ERROR )
// 			{
// 				CTL_Lib.Alarm_Error_Occurrence( 4000, CTL_dWARNING, alarm.mstr_code);
// 			}
// 		}
// 		break;
// 
// 	case 900:
// 		OnSet_CylStopper(IO_ON, SIDE_FRONT_);		// �Ǽ��縮 ������ [ON:UP, OFF:DOWN]
// 		//OnSet_CylBufferTrayPathAlign(IO_ON);		// Accy�� ���� �Ǵ� ������ ���� Buffer Tray Path�� ���� �ϴ� �Ǹ��� [ON:Up, OFF:Down]
// 		m_nStep_Run = 910;
// 		break;
// 		
// 	case 910:
// 		nRetData[0] = OnGet_CylStopper(IO_ON);
// 		//nRetData[1] = OnGet_CylBufferTrayPathAlign(IO_ON);
// 		
// 		if(nRetData[0] == CTL_GOOD /*&& nRetData[1] == CTL_GOOD*/)
// 		{
// 			m_nStep_Run = 10000;
// 		}
// 		else if(nRetData[0] == CTL_ERROR/* || nRetData[0] == CTL_ERROR*/)
// 		{
// 			m_nStep_Run = 900;
// 		}
// 		break;
// 		
// 	case 10000:
// 		stSync.nResp_BufferAlaignConv2XYZRbt_Work = SYNC_RESP_RESET_;
// 		m_nStep_Run = 0;
// 		break;
// 
// 	}
}

void CSeq_BufferAlignConv::OnRun_ReadyMove()
{
	// ��ü ����Ʈ ���� ���� �Ϸ� ���� Ȯ��
	// - ��� �κ��� ���� ������ �Ϸ�� �Ŀ��� ������ �����ϵ��� ��
	// : ���� ���� �߿� ������ �����ϸ� �����ϴ� ������ �浹�� �߻��� �� ����
	if (Func.OnIsAllRcvyComplete() != CTL_YES)
	{
		return;
	}
// 	int nRet = VAR_INIT_;
// 	int nRetData[4] = {0,};
// 	int i = 0;
// 	CString sTmp;
// 	
// 	Func.OnTrace_ThreadStep(20, m_nStep_RunReady );
// 	switch(m_nStep_RunReady)
// 	{
// 	case 0:		
// 		// Lot Start ���°� �ɶ����� ���
// 		if (st_map.nLotStart != CTL_YES)
// 		{
// 			break;
// 		}
// 		//////////////////////////////////////////////////////
// 		if( st_basic.nMode_Slop_Variable == CTL_YES )
// 		{
// 			OnSet_CylBufferRearTrayPitch(IO_ON);
// 		}
// 		else
// 		{
// 			OnSet_CylBufferRearTrayPitch(IO_OFF);			
// 		}
// 		//////////////////////////////////////////////////////
// 		// [Buffer Conv -> XYZRbt] Loading Ready ��û ���
// 		if(stSync.nReq_XYZRbt2BufferAlaignConv_Work == SYNC_REQ_BUFFER_LOADING_READY_)
// 		{
// 			st_work.nSkipReq_BufferConv[SIDE_FRONT_] = CTL_NO; 
// 			st_work.nSkipReq_BufferConv[SIDE_REAR_] = CTL_NO;
// 			
// 			if (st_map.nBoxChange == TRUE)
// 			{
// 				st_map.nBoxChange = FALSE;
// 			}
// 			// [Buffer Conv <- XYZRbt] Loading ����
// 			m_nAccySupply = 0;
// 			m_nStep_RunReady = 50;
// 			
// 		}
// 		break;
// 
// 	case 30:
// 		OnSet_CylBufferTrayPathAlign(IO_OFF);		// Accy�� ���� �Ǵ� ������ ���� Buffer Tray Path�� ���� �ϴ� �Ǹ��� [ON:Up, OFF:Down]
// 		m_nStep_RunReady = 40;
// 		break;
// 		
// 	case 40:
// 		nRetData[0] = OnGet_CylBufferTrayPathAlign(IO_OFF);
// 		
// 		if(nRetData[0] == CTL_GOOD)
// 		{
// 			m_nStep_RunReady = 50;
// 		}
// 		else if(nRetData[0] == CTL_ERROR)
// 		{
// 			CTL_Lib.Alarm_Error_Occurrence( 4000, CTL_dWARNING, alarm.mstr_code );
// 			m_nStep_RunReady = 30;
// 		}
// 		break;
// 
// 	case 50:
// 		nRetData[0] = g_ioMgr.get_in_bit(stIO.i_Chk_BufferTryPathAlignUp);
// 		nRetData[1] = g_ioMgr.get_in_bit(stIO.i_Chk_BufferTryPathAlignDn);
// 		
// 		if( nRetData[0] == IO_ON && nRetData[1] == IO_OFF )
// 		{
// 			OnSet_CylStopper(IO_ON, SIDE_FRONT_);
// 			OnSet_CylStopper(IO_ON, SIDE_REAR_);
// 			m_nStep_RunReady = 100;
// 		}
// 		else
// 		{
// 			m_nStep_RunReady = 30;
// 		}
// 		break;
// 
// 	case 80:
// 		OnSet_CylStopper(IO_ON, SIDE_FRONT_);
// 		OnSet_CylStopper(IO_ON, SIDE_REAR_);
// 		m_nStep_RunReady = 90;
// 		break;
// 		
// 	case 90:
// 		nRetData[0] = OnGet_CylStopper(IO_ON, SIDE_FRONT_);
// 		nRetData[1] = OnGet_CylStopper(IO_ON, SIDE_REAR_);
// 		if( nRetData[0] == CTL_GOOD && nRetData[1] == CTL_GOOD )
// 		{
// 			m_nStep_RunReady = 100;
// 		}
// 		else if( nRetData[0] == CTL_ERROR || nRetData[1] == CTL_ERROR )
// 		{
// 			//139001 0 00 "PS1406_PS1407_BUFFER_ACCY_CONV_FRONT_STOPPER_UP_CHK_ERR."
// 			//139101 0 00 "PS1408_PS1409_BUFFER_ACCY_CONV_REAR_STOPPER_UP_CHK_ERR."
// 			//139201 0 00 "PS1000_PS1001_MIDDLE_BUFFER_ACCY_CONV_STOPPER_UP_CHK_ERR."
// 			if	   ( nRetData[0] == CTL_ERROR )	alarm.mstr_code = "139001";
// 			else								alarm.mstr_code = "139101";
// 			alarm.mn_count_mode	= 0;
// 			alarm.mn_type_mode	= eWARNING;
// 			st_work.nEqpStatus	= dWARNING;
// 			CTL_Lib.Alarm_Error_Occurrence( 4000, CTL_dWARNING, alarm.mstr_code );
// 			m_nStep_RunReady = 80;
// 		}
// 		break;
// 		
// 	case 100:
// 		nRetData[0] = g_ioMgr.get_in_bit(stIO.i_Chk_BufferAccyConvStopper[SIDE_FRONT_][0]);
// 		nRetData[1] = g_ioMgr.get_in_bit(stIO.i_Chk_BufferAccyConvStopper[SIDE_FRONT_][1]);
// 		nRetData[2] = g_ioMgr.get_in_bit(stIO.i_Chk_BufferAccyConvStopper[SIDE_REAR_][0]);
// 		nRetData[3] = g_ioMgr.get_in_bit(stIO.i_Chk_BufferAccyConvStopper[SIDE_REAR_][1]);
// 		if( nRetData[0] == IO_ON && nRetData[1] == IO_OFF && nRetData[2] == IO_ON && nRetData[3] == IO_OFF )
// 		{
// 			m_nStep_RunReady = 500;
// 		}
// 		else
// 		{
// 			m_nStep_RunReady = 80;
// 		}
// 		break;
// 
// 	case 500:
// 		stSync.nResp_BufferAlaignConv2XYZRbt_Work = SYNC_RESP_LOADING_;
// 		m_nStep_RunReady = 600;
// 		break;
// 
// 	case 600:
// 		// [Buffer Conv -> XYZRbt] Work Complete ��û ���
// 		if(stSync.nReq_XYZRbt2BufferAlaignConv_Work == SYNC_REQ_WORK_COMPLETE_)
// 		{
// 			m_nAccySupply = 2;
// 			m_lTime_GoesBy[1][0] = GetCurrentTime();
// 			m_nStep_RunReady = 610;
// 		}
// 		break;
// 
// 	case 610:
// 		nRetData[0] = OnCheck_FrontAccySupply(IO_ON);//Buffer Accy
// 		nRetData[1] = OnCheck_RearAccySupply(IO_ON);
// 		if(nRetData[0] == IO_ON && nRetData[1] == IO_ON)
// 		{
// 			stSync.nResp_BufferAlaignConv2XYZRbt_Work = SYNC_RESP_WORK_;
// 			m_nStep_Run = 700;
// 		}
// 		else
// 		{
// 			m_lTime_GoesBy[1][1] = GetCurrentTime();
// 			m_lTime_GoesBy[1][2] = m_lTime_GoesBy[1][1] - m_lTime_GoesBy[1][0];
// 			if (m_lTime_GoesBy[1][2] < 0)
// 			{
// 				m_lTime_GoesBy[1][0] = GetCurrentTime();
// 				break;
// 			}
// 			
// 			if (m_lTime_GoesBy[1][2] > 500)
// 			{
// 				// 150301 0 00 "PS1008_PS1009_OUT_FRONT_LOST_ACCY_ERR."
// 				// 150401 0 00 "PS1010_PS1011_OUT_REAR_LOST_ACCY_ERR."
// 				if(nRetData[0] != IO_ON )	alarm.mstr_code = "150301";
// 				else                        alarm.mstr_code = "150401";
// 				alarm.mn_count_mode	= 0;
// 				alarm.mn_type_mode	= eWARNING;
// 				st_work.nEqpStatus	= dWARNING;
// 				CTL_Lib.Alarm_Error_Occurrence( 4000, CTL_dWARNING, alarm.mstr_code );
// 			}
// 		}
// 		break;
// 
// 	case 700:
// 		OnSet_CylBufferTrayPathAlign(IO_OFF);		// Accy�� ���� �Ǵ� ������ ���� Buffer Tray Path�� ���� �ϴ� �Ǹ��� [ON:Up, OFF:Down]
// 		m_nStep_RunReady = 800;
// 		break;
// 		
// 	case 800:
// 		nRetData[0] = OnGet_CylBufferTrayPathAlign(IO_OFF);
// 		
// 		if(nRetData[0] == CTL_GOOD)
// 		{
// 			m_nStep_RunReady = 1000;
// 		}
// 		else if(nRetData[0] == CTL_ERROR)
// 		{
// 			CTL_Lib.Alarm_Error_Occurrence( 4000, CTL_dWARNING, alarm.mstr_code );
// 			m_nStep_RunReady = 1100;
// 		}
// 		break;
// 
// 	case 900:
// 		nRetData[0] = g_ioMgr.get_in_bit(stIO.i_Chk_BufferTryPathAlignUp);
// 		nRetData[1] = g_ioMgr.get_in_bit(stIO.i_Chk_BufferTryPathAlignDn);
// 		
// 		if( nRetData[0] == IO_OFF && nRetData[1] == IO_ON )
// 		{
// 			m_nStep_RunReady = 1000;
// 		}
// 		else
// 		{
// 			m_nStep_RunReady = 700;
// 		}
// 		break;	
// 
// 	case 1000:
// 		{
// 			int nReadIO[4] = {0,};
// 			nReadIO[0] = g_ioMgr.get_in_bit(stIO.i_Slop_Variable_Rear_Left_Open_Check);
// 			nReadIO[1] = g_ioMgr.get_in_bit(stIO.i_Slop_Variable_Rear_Left_Close_Check);
// 			nReadIO[2] = g_ioMgr.get_in_bit(stIO.i_Slop_Variable_Rear_Right_Open_Check);
// 			nReadIO[3] = g_ioMgr.get_in_bit(stIO.i_Slop_Variable_Rear_Right_Close_Check);
// 			
// 			if( st_basic.nMode_Slop_Variable == CTL_YES )
// 			{
// 				if( nReadIO[0] == IO_OFF && nReadIO[1] == IO_ON && nReadIO[2] == IO_OFF && nReadIO[3] == IO_ON )
// 				{
// 					nRet = CTL_GOOD;
// 				}
// 				else
// 				{
// 					nRet = CTL_ERROR;
// 				}
// 			}
// 			else 
// 			{
// 				if( nReadIO[0] == IO_ON && nReadIO[1] == IO_OFF && nReadIO[2] == IO_ON && nReadIO[3] == IO_OFF )
// 				{
// 					nRet = CTL_GOOD;
// 				}
// 				else
// 				{
// 					nRet = CTL_ERROR;
// 				}
// 			}
// 			if( nRet == CTL_GOOD )
// 			{
// 				// �׽�Ʈ�� ���� Main�� ���� ���� �ʰ� �׽�Ʈ�� ���� �߰�
// 				if (m_nTestSupply == TRUE)
// 				{
// 					m_nStep_RunReady = 1100;
// 					break;
// 				}
// 				
// 				if(st_basic.n_mode_device == WITHOUT_DVC_)
// 				{
// 					if(st_basic.nMode_SMEMAUsing == SMEMA_NOT_USING)
// 					{
// 						m_nStep_RunReady = 260;
// 						break;
// 					}
// 				}
// 				else
// 				{
// 					if(st_basic.nMode_SMEMAUsing == SMEMA_NOT_USING)
// 					{
// 						m_nStep_RunReady = 260;
// 						break;
// 					}
// 				}			
// 				Func.OnLogBCRData("CSeq_BufferAlignConv::OnRun_Move_210");
// 				m_nStep_RunReady = 250;
// 			}
// 			else
// 			{
// 				if( st_basic.nMode_Slop_Variable == CTL_YES )
// 				{
// 					OnSet_CylBufferRearTrayPitch(IO_ON);
// 				}
// 				else
// 				{
// 					OnSet_CylBufferRearTrayPitch(IO_OFF);		
// 				}
// 				m_nStep_RunReady = 1010;
// 			}
// 		}
// 		break;
// 		
// 	case 1010:
// 		if( st_basic.nMode_Slop_Variable == CTL_YES )
// 		{
// 			nRet = OnGet_CylBufferRearTrayPitch( IO_ON);
// 		}
// 		else
// 		{
// 			nRet = OnGet_CylBufferRearTrayPitch( IO_OFF);
// 		}
// 		
// 		if( nRet == CTL_GOOD )
// 		{
// 			// �׽�Ʈ�� ���� Main�� ���� ���� �ʰ� �׽�Ʈ�� ���� �߰�
// 			if (m_nTestSupply == TRUE)
// 			{
// 				m_nStep_RunReady = 1100;
// 				break;
// 			}
// 			
// 			if(st_basic.n_mode_device == WITHOUT_DVC_)
// 			{
// 				if(st_basic.nMode_SMEMAUsing == SMEMA_NOT_USING)
// 				{
// 					m_nStep_RunReady = 260;
// 					break;
// 				}
// 			}
// 			else
// 			{
// 				if(st_basic.nMode_SMEMAUsing == SMEMA_NOT_USING)
// 				{
// 					m_nStep_RunReady = 260;
// 					break;
// 				}
// 			}			
// 			Func.OnLogBCRData("CSeq_BufferAlignConv::OnRun_MoveReady_1010");
// 			m_nStep_RunReady = 1200;
// 			
// 		}
// 		else if( nRet == CTL_ERROR )
// 		{
// 			CTL_Lib.Alarm_Error_Occurrence( 4000, CTL_dWARNING, alarm.mstr_code );
// 			m_nStep_RunReady = 200;
// 		}
// 		break;
// 
// 
// 	case 1100:
// 		nRetData[0] = OnCheck_FrontAccySupply(IO_ON);
// 		nRetData[1] = OnCheck_RearAccySupply(IO_ON);
// 		
// 		if (nRetData[0] == IO_OFF && nRetData[1] == IO_OFF)
// 		{
// 			memset(st_map.nBufferCovAccyExist, ACCY_NO_, sizeof(st_map.nBufferCovAccyExist));
// 			m_nStep_RunReady = 4000;
// 		}
// 		break;
// 
// 	case 1200:
// 		if (st_work.nSkipReq_BufferConv[SIDE_FRONT_] == CTL_YES && 
// 			st_work.nSkipReq_BufferConv[SIDE_REAR_] == CTL_YES)
// 		{
// 			st_map.nBufferCovAccyExist[0] = ACCY_NO_;
// 			st_map.nBufferCovAccyExist[1] = ACCY_NO_;
// 			st_map.nBufferCovAccyExist[2] = ACCY_NO_;
// 			st_map.nBufferCovAccyExist[3] = ACCY_NO_;
// 			st_work.sBufferConvBCR_Data[0] = "";
// 			st_work.sBufferConvBCR_Data[1] = "";
// 			st_work.sBufferConvBCR_Data[2] = "";
// 			st_work.sBufferConvBCR_Data[3] = "";
// 			st_work.nSkipReq_BufferConv[SIDE_FRONT_] = CTL_NO;
// 			st_work.nSkipReq_BufferConv[SIDE_REAR_] = CTL_NO;
// 			m_nAccySupply = 0;			
// 			m_nStep_Run = 3400;
// 			break;
// 		}
// 		if (st_work.nSkipReq_BufferConv[SIDE_FRONT_] == CTL_YES)
// 		{
// 			st_map.nBufferCovAccyExist[0] = ACCY_NO_;
// 			st_map.nBufferCovAccyExist[1] = ACCY_NO_;
// 			st_work.sBufferConvBCR_Data[0] = "";
// 			st_work.sBufferConvBCR_Data[1] = "";
// 			st_work.nSkipReq_BufferConv[SIDE_FRONT_] = CTL_NO;
// 			m_nAccySupply--;
// 			m_nStep_Run = 3400;
// 			break;
// 		}
// 		if (st_work.nSkipReq_BufferConv[SIDE_REAR_] == CTL_YES)
// 		{
// 			st_map.nBufferCovAccyExist[0] = ACCY_NO_;
// 			st_map.nBufferCovAccyExist[1] = ACCY_NO_;
// 			st_work.sBufferConvBCR_Data[0] = "";
// 			st_work.sBufferConvBCR_Data[1] = "";
// 			st_work.nSkipReq_BufferConv[SIDE_FRONT_] = CTL_NO;
// 			m_nAccySupply--;
// 			m_nStep_Run = 3400;
// 			break;
// 		}
// 
// 		
// 		//st_work.m_sAccyMathCode = "LA69-01773A";
// 		if (st_work.m_sCurrntAccyMathCode != "")
// 		{
// 			if (st_work.m_sCurrntAccyMathCode == st_work.sBufferConvBCR_Data[0] &&
// 				st_work.m_sCurrntAccyMathCode == st_work.sBufferConvBCR_Data[1])
// 			{
// 				m_nStep_Run = 2000;
// 			}
// 			else
// 			{
// 				Func.OnLogBCRData("CSeq_BufferAlignConv::OnRun_ReadyMove_1200");
// 				// 145000 0 00 "ACCY_CONV_DATA_MISS_MATH."
// 				alarm.mstr_code		= "145000";
// 				alarm.mn_count_mode	= 0;
// 				alarm.mn_type_mode	= eWARNING;
// 				st_work.nEqpStatus	= dWARNING;
// 				CTL_Lib.Alarm_Error_Occurrence( 4000, CTL_dWARNING, alarm.mstr_code );
// 			}
// 		}
// 		break;
// 
// 	case 1800:
// 		OnSet_CylBufferTrayPathAlign(IO_OFF);		// Accy�� ���� �Ǵ� ������ ���� Buffer Tray Path�� ���� �ϴ� �Ǹ��� [ON:Up, OFF:Down]
// 		m_nStep_RunReady = 1900;
// 		break;
// 		
// 	case 1900:
// 		nRetData[0] = OnGet_CylBufferTrayPathAlign(IO_OFF);
// 		
// 		if(nRetData[0] == CTL_GOOD)
// 		{
// 			m_nStep_RunReady = 2000;
// 		}
// 		else if(nRetData[0] == CTL_ERROR)
// 		{
// 			CTL_Lib.Alarm_Error_Occurrence( 4000, CTL_dWARNING, alarm.mstr_code );
// 			m_nStep_RunReady = 1800;
// 		}
// 		break;
// 
// 	case 2000:
// 		nRetData[0] = g_ioMgr.get_in_bit(stIO.i_Chk_BufferTryPathAlignUp, IO_OFF);
// 		nRetData[1] = g_ioMgr.get_in_bit(stIO.i_Chk_BufferTryPathAlignDn, IO_ON);
// 
// 		if( nRetData[0] == IO_OFF && nRetData[1] == IO_ON )
// 		{
// 			m_nStep_RunReady = 2100;
// 		}
// 		else
// 		{
// 			m_nStep_RunReady = 1900;
// 		}
// 		break;
// 
// 	case 2100:
// 		if (st_work.nSkipReq_BufferConv[SIDE_FRONT_] == CTL_YES && 
// 			st_work.nSkipReq_BufferConv[SIDE_REAR_] == CTL_YES)
// 		{
// 			st_map.nBufferCovAccyExist[0] = ACCY_NO_;
// 			st_map.nBufferCovAccyExist[1] = ACCY_NO_;
// 			st_map.nBufferCovAccyExist[2] = ACCY_NO_;
// 			st_map.nBufferCovAccyExist[3] = ACCY_NO_;
// 			st_work.sBufferConvBCR_Data[0] = "";
// 			st_work.sBufferConvBCR_Data[1] = "";
// 			st_work.sBufferConvBCR_Data[2] = "";
// 			st_work.sBufferConvBCR_Data[3] = "";
// 			st_work.nSkipReq_BufferConv[SIDE_FRONT_] = CTL_NO;
// 			st_work.nSkipReq_BufferConv[SIDE_REAR_] = CTL_NO;
// 			m_nAccySupply = 0;			
// 			m_nStep_Run = 3400;
// 			break;
// 		}
// 		if (st_work.nSkipReq_BufferConv[SIDE_FRONT_] == CTL_YES)
// 		{
// 			st_map.nBufferCovAccyExist[0] = ACCY_NO_;
// 			st_map.nBufferCovAccyExist[1] = ACCY_NO_;
// 			st_work.sBufferConvBCR_Data[0] = "";
// 			st_work.sBufferConvBCR_Data[1] = "";
// 			st_work.nSkipReq_BufferConv[SIDE_FRONT_] = CTL_NO;
// 			m_nAccySupply--;
// 			m_nStep_Run = 3400;
// 			break;
// 		}
// 		if (st_work.nSkipReq_BufferConv[SIDE_REAR_] == CTL_YES)
// 		{
// 			st_map.nBufferCovAccyExist[0] = ACCY_NO_;
// 			st_map.nBufferCovAccyExist[1] = ACCY_NO_;
// 			st_work.sBufferConvBCR_Data[0] = "";
// 			st_work.sBufferConvBCR_Data[1] = "";
// 			st_work.nSkipReq_BufferConv[SIDE_FRONT_] = CTL_NO;
// 			m_nAccySupply--;
// 			m_nStep_Run = 3400;
// 			break;
// 		}
// 
// 		nRetData[0] = g_ioMgr.get_in_bit( stIO.i_Chk_BufferTryAccyDetection[SIDE_FRONT_][SIDE_LEFT_], IO_ON );
// 		nRetData[1] = g_ioMgr.get_in_bit( stIO.i_Chk_BufferTryAccyDetection[SIDE_FRONT_][SIDE_RIGHT_], IO_ON );
// 		nRetData[2] = g_ioMgr.get_in_bit( stIO.i_Chk_BufferTryAccyDetection[SIDE_REAR_][SIDE_LEFT_], IO_ON );
// 		nRetData[3] = g_ioMgr.get_in_bit( stIO.i_Chk_BufferTryAccyDetection[SIDE_REAR_][SIDE_RIGHT_], IO_ON );
// 
// 		if( nRetData[0] == IO_ON && nRetData[1] == IO_ON && nRetData[2] == IO_ON && nRetData[3] == IO_ON )
// 		{
// 			m_nStep_RunReady = 2200;
// 		}
// 		else if( nRetData[0] != IO_ON || nRetData[1] != IO_ON || nRetData[2] != IO_ON || nRetData[3] != IO_ON )
// 		{
// 			m_lTime_GoesBy[1][1] = GetCurrentTime();
// 			m_lTime_GoesBy[1][2] = m_lTime_GoesBy[1][1] - m_lTime_GoesBy[1][0];
// 			if( m_lTime_GoesBy[1][2] <= 0 ) m_lTime_GoesBy[1][0] = GetCurrentTime();
// 			if( m_lTime_GoesBy[1][2] > 10000 )
// 			{
// // 				150001 0 00 "PS1406_PS1407_BUFFER_FRONT_ACCY_ON_CHK_ERR."
// // 				150002 0 00 "PS1406_PS1407_BUFFER_FRONT_ACCY_OFF_CHK_ERR."
// // 				150101 0 00 "PS1408_PS1409_BUFFER_REAR_ACCY_ON_CHK_ERR."
// // 				150102 0 00 "PS1408_PS1409_BUFFER_REAR_ACCY_OFF_CHK_ERR."
// 				if( nRetData[0] != IO_ON || nRetData[1] != IO_ON) alarm.mstr_code = "150002";
// 				else                                              alarm.mstr_code = "150102";
// 				CTL_Lib.Alarm_Error_Occurrence( 4000, CTL_dWARNING, alarm.mstr_code );
// 			}
// 		}
// 		break;
// 
// 	case 2200:
// 		if (st_work.nSkipReq_BufferConv[SIDE_FRONT_] == CTL_YES && 
// 			st_work.nSkipReq_BufferConv[SIDE_REAR_] == CTL_YES)
// 		{
// 			st_map.nBufferCovAccyExist[0] = ACCY_NO_;
// 			st_map.nBufferCovAccyExist[1] = ACCY_NO_;
// 			st_map.nBufferCovAccyExist[2] = ACCY_NO_;
// 			st_map.nBufferCovAccyExist[3] = ACCY_NO_;
// 			st_work.sBufferConvBCR_Data[0] = "";
// 			st_work.sBufferConvBCR_Data[1] = "";
// 			st_work.sBufferConvBCR_Data[2] = "";
// 			st_work.sBufferConvBCR_Data[3] = "";
// 			st_work.nSkipReq_BufferConv[SIDE_FRONT_] = CTL_NO;
// 			st_work.nSkipReq_BufferConv[SIDE_REAR_] = CTL_NO;
// 			m_nStep_Run = 3400;
// 			break;
// 		}
// 		if (st_work.nSkipReq_BufferConv[SIDE_FRONT_] == CTL_YES)
// 		{
// 			st_map.nBufferCovAccyExist[0] = ACCY_NO_;
// 			st_map.nBufferCovAccyExist[1] = ACCY_NO_;
// 			st_work.sBufferConvBCR_Data[0] = "";
// 			st_work.sBufferConvBCR_Data[1] = "";
// 			st_work.nSkipReq_BufferConv[SIDE_FRONT_] = CTL_NO;
// 			m_nStep_Run = 3400;
// 			break;
// 		}
// 		if (st_work.nSkipReq_BufferConv[SIDE_REAR_] == CTL_YES)
// 		{
// 			st_map.nBufferCovAccyExist[0] = ACCY_NO_;
// 			st_map.nBufferCovAccyExist[1] = ACCY_NO_;
// 			st_work.sBufferConvBCR_Data[0] = "";
// 			st_work.sBufferConvBCR_Data[1] = "";
// 			st_work.nSkipReq_BufferConv[SIDE_FRONT_] = CTL_NO;
// 			m_nStep_Run = 3400;
// 			break;
// 		}
// 
// 		if( m_nReqMidConv == CTL_REQ )
// 		{
// 			m_nStep_RunReady = 2500;
// 		}
// 		break;
// 
// 	case 2500:
// 		nRetData[0] = g_ioMgr.get_in_bit( stIO.i_Chk_BufferTryAccyDetection[SIDE_MIDDLE][SIDE_LEFT_], IO_OFF );
// 		nRetData[1] = g_ioMgr.get_in_bit( stIO.i_Chk_BufferTryAccyDetection[SIDE_MIDDLE][SIDE_RIGHT_], IO_OFF );
// 		if( nRetData[0] == IO_OFF && nRetData[1] == IO_OFF )
// 		{
// 			m_nStep_RunReady = 2510;
// 		}
// 		else
// 		{
// 			//125000 0 00 "PS1006_SLOP_MIDDLE_LEFT_ACCY_DETECTION_CHK_ERR."
// 			//125001 0 00 "PS1007_SLOP_MIDDLE_RIGHT_ACCY_DETECTION_CHK_ERR."
// 			if( nRetData[0] == IO_ON ) alarm.mstr_code = "125000";
// 			else                       alarm.mstr_code = "125001";
// 			CTL_Lib.Alarm_Error_Occurrence( 4000, CTL_dWARNING, alarm.mstr_code);
// 		}
// 		break;
// 
// 	case 2510:
// 		OnSet_CylStopper(IO_OFF, SIDE_FIRST_);
// 		OnSet_CylStopper(IO_OFF, SIDE_RIGHT_);
// 		m_nStep_RunReady = 2520;
// 		break;
// 
// 	case 2520:
// 		nRetData[0]  = OnGet_CylStopper(IO_OFF, SIDE_FIRST_);
// 		nRetData[1]  = OnGet_CylStopper(IO_OFF, SIDE_RIGHT_);
// 		if( nRetData[0] == CTL_GOOD && nRetData[1] == CTL_GOOD )
// 		{
// 			m_lTime_GoesBy[0][0] = GetCurrentTime();
// 			m_nStep_RunReady = 2600;
// 		}
// 		else if( nRetData[0] == CTL_ERROR || nRetData[1] == CTL_ERROR )
// 		{
// 			CTL_Lib.Alarm_Error_Occurrence( 4000, CTL_dWARNING, alarm.mstr_code);
// 			m_nStep_RunReady = 2510;
// 		}
// 		break;
// 
// 	case 2600:
// 		if (st_work.nSkipReq_BufferConv[SIDE_FRONT_] == CTL_YES && 
// 			st_work.nSkipReq_BufferConv[SIDE_REAR_] == CTL_YES)
// 		{
// 			st_map.nBufferCovAccyExist[0] = ACCY_NO_;
// 			st_map.nBufferCovAccyExist[1] = ACCY_NO_;
// 			st_map.nBufferCovAccyExist[2] = ACCY_NO_;
// 			st_map.nBufferCovAccyExist[3] = ACCY_NO_;
// 			st_work.sBufferConvBCR_Data[0] = "";
// 			st_work.sBufferConvBCR_Data[1] = "";
// 			st_work.sBufferConvBCR_Data[2] = "";
// 			st_work.sBufferConvBCR_Data[3] = "";
// 			st_work.nSkipReq_BufferConv[SIDE_FRONT_] = CTL_NO;
// 			st_work.nSkipReq_BufferConv[SIDE_REAR_] = CTL_NO;
// 			m_nStep_Run = 3400;
// 			break;
// 		}
// 		if (st_work.nSkipReq_BufferConv[SIDE_FRONT_] == CTL_YES)
// 		{
// 			st_map.nBufferCovAccyExist[0] = ACCY_NO_;
// 			st_map.nBufferCovAccyExist[1] = ACCY_NO_;
// 			st_work.sBufferConvBCR_Data[0] = "";
// 			st_work.sBufferConvBCR_Data[1] = "";
// 			st_work.nSkipReq_BufferConv[SIDE_FRONT_] = CTL_NO;
// 			m_nStep_Run = 2700;
// 			break;
// 		}
// 
// 		nRetData[0] = g_ioMgr.get_in_bit( stIO.i_Chk_BufferTryAccyDetection[SIDE_MIDDLE][SIDE_LEFT_], IO_OFF );
// 		nRetData[1] = g_ioMgr.get_in_bit( stIO.i_Chk_BufferTryAccyDetection[SIDE_MIDDLE][SIDE_RIGHT_], IO_OFF );
// 		if( nRetData[0] == IO_ON && nRetData[1] == IO_ON )
// 		{
// 			m_nReqMidConv = CTL_READY;
// 			st_map.nBufferCovAccyExist[0] == ACCY_NO_;
// 			st_map.nBufferCovAccyExist[1] == ACCY_NO_;
// 			st_work.sBufferConvBCR_Data[0] = "";
// 			st_work.sBufferConvBCR_Data[1] = "";
// 			m_nStep_RunReady = 2700;
// 		}
// 		else
// 		{
// 		 //125000 0 00 "PS1006_SLOP_MIDDLE_LEFT_ACCY_DETECTION_CHK_ERR."
// 		 //125001 0 00 "PS1007_SLOP_MIDDLE_RIGHT_ACCY_DETECTION_CHK_ERR."
// 			m_lTime_GoesBy[0][1] = GetCurrentTime();
// 			m_lTime_GoesBy[0][2] = m_lTime_GoesBy[0][1] - m_lTime_GoesBy[0][0];
// 			if( m_lTime_GoesBy[0][2] <= 0 )  m_lTime_GoesBy[0][0] = GetCurrentTime();
// 			if( m_lTime_GoesBy[0][2] < 5000) break;
// 			if( nRetData[0] == IO_ON ) alarm.mstr_code = "125000";
// 			else                       alarm.mstr_code = "125001";
// 			CTL_Lib.Alarm_Error_Occurrence( 4000, CTL_dWARNING, alarm.mstr_code);
// 		}
// 		break;
// 
// 	case 2700:
// 		if (st_work.nSkipReq_BufferConv[SIDE_FRONT_] == CTL_YES && 
// 			st_work.nSkipReq_BufferConv[SIDE_REAR_] == CTL_YES)
// 		{
// 			st_map.nBufferCovAccyExist[0] = ACCY_NO_;
// 			st_map.nBufferCovAccyExist[1] = ACCY_NO_;
// 			st_map.nBufferCovAccyExist[2] = ACCY_NO_;
// 			st_map.nBufferCovAccyExist[3] = ACCY_NO_;
// 			st_work.sBufferConvBCR_Data[0] = "";
// 			st_work.sBufferConvBCR_Data[1] = "";
// 			st_work.sBufferConvBCR_Data[2] = "";
// 			st_work.sBufferConvBCR_Data[3] = "";
// 			st_work.nSkipReq_BufferConv[SIDE_FRONT_] = CTL_NO;
// 			st_work.nSkipReq_BufferConv[SIDE_REAR_] = CTL_NO;
// 			m_nStep_Run = 3400;
// 			break;
// 		}
// 		if (st_work.nSkipReq_BufferConv[SIDE_REAR_] == CTL_YES)
// 		{
// 			st_map.nBufferCovAccyExist[0] = ACCY_NO_;
// 			st_map.nBufferCovAccyExist[1] = ACCY_NO_;
// 			st_work.sBufferConvBCR_Data[0] = "";
// 			st_work.sBufferConvBCR_Data[1] = "";
// 			st_work.nSkipReq_BufferConv[SIDE_FRONT_] = CTL_NO;
// 			m_nStep_Run = 3400;
// 			break;
// 		}
// 
// 		nRet = OnCheck_FrontAccySupply(IO_ON);		// Buffer Conv Front Accy Ȯ��
// 		
// 		if (st_basic.n_mode_device == WITHOUT_DVC_)
// 		{
// 			if (st_map.nBufferCovAccyExist[0] == ACCY_NO_ && st_map.nBufferCovAccyExist[1] == ACCY_NO_)
// 			{
// 				nRet = IO_OFF;
// 			}
// 		}
// 		
// 		if(nRet == IO_OFF && st_map.nBufferCovAccyExist[0] == ACCY_NO_ && st_map.nBufferCovAccyExist[1] == ACCY_NO_)
// 		{
// 			m_nStep_Run = 2800;
// 		}
// 		else if (nRet == IO_ON && st_map.nBufferCovAccyExist[0] == ACCY_NO_ && st_map.nBufferCovAccyExist[1] == ACCY_NO_)
// 		{
// 			// 150002 0 00 "PS1406_PS1407_BUFFER_FRONT_ACCY_OFF_CHK_ERR."
// 			alarm.mstr_code		= "150002";
// 			alarm.mn_count_mode	= 0;
// 			alarm.mn_type_mode	= eWARNING;
// 			st_work.nEqpStatus	= dWARNING;
// 			CTL_Lib.Alarm_Error_Occurrence( 4000, CTL_dWARNING, alarm.mstr_code );
// 		}
// 		break;
// 		
// 	case 2800:
// 		if( m_nReqMidConv == CTL_CHANGE)
// 		{
// 			m_nReqMidConv = CTL_NO;
// 			m_nStep_RunReady = 2900;
// 		}
// 		break;
// 
// 	case 2900:
// 		if( m_nReqMidConv == CTL_REQ)
// 		{
// 			m_nStep_RunReady = 3000;
// 		}
// 		break;
// 
// 	case 3000:
// 		nRet = OnCheck_RearAccySupply(IO_ON);		// Buffer Conv Rear Accy Ȯ��
// 		
// 		if (st_basic.n_mode_device == WITHOUT_DVC_)
// 		{
// 			if (st_map.nBufferCovAccyExist[2] == ACCY_NO_ && st_map.nBufferCovAccyExist[3] == ACCY_NO_)
// 			{
// 				nRet = IO_OFF;
// 			}
// 		}		
// 		if(nRet == IO_ON )
// 		{
// 			m_nStep_RunReady = 3100;
// 		}
// 		else
// 		{
// 			// 150102 0 00 "PS1408_PS1409_BUFFER_REAR_ACCY_OFF_CHK_ERR."
// 			alarm.mstr_code		= "150102";
// 			alarm.mn_count_mode	= 0;
// 			alarm.mn_type_mode	= eWARNING;
// 			st_work.nEqpStatus	= dWARNING;
// 			CTL_Lib.Alarm_Error_Occurrence( 4000, CTL_dWARNING, alarm.mstr_code );
// 		}
// 		break;	
// 
// 	case 3100:
// 		nRetData[0] = g_ioMgr.get_in_bit( stIO.i_Chk_BufferTryAccyDetection[SIDE_MIDDLE][SIDE_LEFT_], IO_OFF );
// 		nRetData[1] = g_ioMgr.get_in_bit( stIO.i_Chk_BufferTryAccyDetection[SIDE_MIDDLE][SIDE_RIGHT_], IO_OFF );
// 		if( nRetData[0] == IO_OFF && nRetData[1] == IO_OFF )
// 		{
// 			m_nStep_RunReady = 3200;
// 		}
// 		else
// 		{
// 			//125000 0 00 "PS1006_SLOP_MIDDLE_LEFT_ACCY_DETECTION_CHK_ERR."
// 			//125001 0 00 "PS1007_SLOP_MIDDLE_RIGHT_ACCY_DETECTION_CHK_ERR."
// 			if( nRetData[0] == IO_ON ) alarm.mstr_code = "125000";
// 			else                       alarm.mstr_code = "125001";
// 			CTL_Lib.Alarm_Error_Occurrence( 4000, CTL_dWARNING, alarm.mstr_code);
// 		}
// 		break;
// 		
// 	case 3200:
// 		OnSet_CylStopper(IO_OFF, SIDE_FRONT_);
// 		m_nStep_RunReady = 3210;
// 		break;
// 		
// 	case 3210:
// 		nRet = OnGet_CylStopper(IO_OFF, SIDE_FRONT_);
// 		if( nRet == CTL_GOOD )
// 		{
// 			m_lTime_GoesBy[0][0] = GetCurrentTime();
// 			m_nStep_RunReady = 3300;
// 		}
// 		else if( nRet == CTL_ERROR )
// 		{
// 			CTL_Lib.Alarm_Error_Occurrence( 4000, CTL_dWARNING, alarm.mstr_code);
// 			m_nStep_RunReady = 3200;
// 		}
// 		break;
// 
// 	case 3300:
// 		if (st_work.nSkipReq_BufferConv[SIDE_FRONT_] == CTL_YES && 
// 			st_work.nSkipReq_BufferConv[SIDE_REAR_] == CTL_YES)
// 		{
// 			st_map.nBufferCovAccyExist[0] = ACCY_NO_;
// 			st_map.nBufferCovAccyExist[1] = ACCY_NO_;
// 			st_map.nBufferCovAccyExist[2] = ACCY_NO_;
// 			st_map.nBufferCovAccyExist[3] = ACCY_NO_;
// 			st_work.sBufferConvBCR_Data[0] = "";
// 			st_work.sBufferConvBCR_Data[1] = "";
// 			st_work.sBufferConvBCR_Data[2] = "";
// 			st_work.sBufferConvBCR_Data[3] = "";
// 			st_work.nSkipReq_BufferConv[SIDE_FRONT_] = CTL_NO;
// 			st_work.nSkipReq_BufferConv[SIDE_REAR_] = CTL_NO;
// 			m_nStep_Run = 3400;
// 			break;
// 		}
// 		if (st_work.nSkipReq_BufferConv[SIDE_FRONT_] == CTL_YES)
// 		{
// 			st_map.nBufferCovAccyExist[0] = ACCY_NO_;
// 			st_map.nBufferCovAccyExist[1] = ACCY_NO_;
// 			st_work.sBufferConvBCR_Data[0] = "";
// 			st_work.sBufferConvBCR_Data[1] = "";
// 			st_work.nSkipReq_BufferConv[SIDE_FRONT_] = CTL_NO;
// 			m_nStep_Run = 3400;
// 			break;
// 		}
// 		if (st_work.nSkipReq_BufferConv[SIDE_REAR_] == CTL_YES)
// 		{
// 			st_map.nBufferCovAccyExist[0] = ACCY_NO_;
// 			st_map.nBufferCovAccyExist[1] = ACCY_NO_;
// 			st_work.sBufferConvBCR_Data[0] = "";
// 			st_work.sBufferConvBCR_Data[1] = "";
// 			st_work.nSkipReq_BufferConv[SIDE_FRONT_] = CTL_NO;
// 			m_nStep_Run = 3400;
// 			break;
// 		}
// 		
// 		
// 		nRetData[0] = g_ioMgr.get_in_bit( stIO.i_Chk_BufferTryAccyDetection[SIDE_MIDDLE][SIDE_LEFT_], IO_OFF );
// 		nRetData[1] = g_ioMgr.get_in_bit( stIO.i_Chk_BufferTryAccyDetection[SIDE_MIDDLE][SIDE_RIGHT_], IO_OFF );
// 		if( nRetData[0] == IO_ON && nRetData[1] == IO_ON )
// 		{
// 			m_nReqMidConv = CTL_READY;
// 			st_map.nBufferCovAccyExist[2] == ACCY_NO_;
// 			st_map.nBufferCovAccyExist[3] == ACCY_NO_;
// 			st_work.sBufferConvBCR_Data[2] = "";
// 			st_work.sBufferConvBCR_Data[3] = "";
// 			m_nStep_RunReady = 3400;
// 		}
// 		else
// 		{
// 			//125000 0 00 "PS1006_SLOP_MIDDLE_LEFT_ACCY_DETECTION_CHK_ERR."
// 			//125001 0 00 "PS1007_SLOP_MIDDLE_RIGHT_ACCY_DETECTION_CHK_ERR."
// 			m_lTime_GoesBy[0][1] = GetCurrentTime();
// 			m_lTime_GoesBy[0][2] = m_lTime_GoesBy[0][1] - m_lTime_GoesBy[0][0];
// 			if( m_lTime_GoesBy[0][2] <= 0 )  m_lTime_GoesBy[0][0] = GetCurrentTime();
// 			if( m_lTime_GoesBy[0][2] < 5000) break;
// 			if( nRetData[0] == IO_ON ) alarm.mstr_code = "125000";
// 			else                       alarm.mstr_code = "125001";
// 			CTL_Lib.Alarm_Error_Occurrence( 4000, CTL_dWARNING, alarm.mstr_code);
// 		}
// 		break;
// 
// 	case 3400:
// 		nRetData[0] = OnCheck_FrontAccySupply(IO_ON);		// Buffer Conv Front Accy Ȯ��
// 		nRetData[1] = OnCheck_RearAccySupply(IO_ON);		// Buffer Conv Rear Accy Ȯ��
// 		
// 		if (st_basic.n_mode_device == WITHOUT_DVC_)
// 		{
// 			if (st_map.nBufferCovAccyExist[0] == ACCY_NO_ && st_map.nBufferCovAccyExist[1] == ACCY_NO_)
// 			{
// 				nRetData[0] = IO_OFF;
// 			}
// 			if (st_map.nBufferCovAccyExist[2] == ACCY_NO_ && st_map.nBufferCovAccyExist[3] == ACCY_NO_)
// 			{
// 				nRetData[1] = IO_OFF;
// 				
// 			}
// 			if (nRetData[0] == IO_OFF && nRetData[1] == IO_OFF)
// 			{
// 				nRetData[2] = IO_OFF;
// 			}
// 		}
// 		
// 		if(nRetData[0] == IO_OFF && nRetData[1] == IO_OFF && nRetData[2] == IO_OFF)
// 		{
// 			m_nStep_Run = 4000;
// 		}
// 		break;
// 
// 	case 4000:
// 		OnSet_CylStopper(IO_ON, SIDE_FRONT_);
// 		OnSet_CylStopper(IO_ON, SIDE_REAR_);
// 		OnSet_CylBufferTrayPathAlign(IO_ON);
// 		
// 		m_nStep_RunReady = 4100;
// 		break;
// 		
// 	case 4100:
// 		nRetData[0] = OnGet_CylStopper( IO_OFF, SIDE_FRONT_ );
// 		nRetData[1] = OnGet_CylStopper( IO_ON, SIDE_REAR_ );
// 		nRetData[2] = OnGet_CylBufferTrayPathAlign(IO_ON);
// 		if (nRetData[0] == CTL_GOOD && nRetData[1] == CTL_GOOD && nRetData[2] == CTL_GOOD)
// 		{
// 			m_lTime_GoesBy[1][0] = GetCurrentTime();
// 			m_nStep_RunReady = 4200;
// 		}
// 		else if(nRetData[0] == CTL_ERROR || nRetData[1] == CTL_ERROR || nRetData[2] == CTL_ERROR )
// 		{
// 			CTL_Lib.Alarm_Error_Occurrence( 4000, CTL_dWARNING, alarm.mstr_code );
// 			m_nStep_RunReady = 4000;
// 		}
// 		break;
// 		
// 	case 10000:
// 		stSync.nResp_BufferAlaignConv2XYZRbt_Work = SYNC_RESP_RESET_;
// 		m_nStep_Run = 0;
// 		break;	
// 
// 	}
}

//==================================================================//
// Accy Box ���� ���� �۾��� 
//==================================================================//
int CSeq_BufferAlignConv::OnRun_ForceDischarge() 
{
return true;
}

//==================================================================//
// AC ���� ����/����
//==================================================================//
void CSeq_BufferAlignConv::OnMove_AcMotor(int nzMoveType) 
{
	if (nzMoveType == AC_MV_CW_)
	{
		g_ioMgr.set_out_bit(stIO.o_BufferConvMotor[SIDE_LEFT_], IO_ON);
		g_ioMgr.set_out_bit(stIO.o_BufferConvMotor[SIDE_RIGHT_], IO_ON);
	}
	else if (nzMoveType == AC_MV_STOP_)
	{
		g_ioMgr.set_out_bit(stIO.o_BufferConvMotor[SIDE_LEFT_], IO_OFF);
		g_ioMgr.set_out_bit(stIO.o_BufferConvMotor[SIDE_RIGHT_], IO_OFF);

		m_nReq_AcMotCtrl = REQ_MV_RESET_;	// AC ���� ���� ��û ����
	}
}

//==================================================================//
// �Ǽ��縮�� ���� �Ǵ� ���⿡�� Front Buffer ���� Ȯ��
//==================================================================//
int CSeq_BufferAlignConv::OnCheck_FrontAccySupply(int nzOnOff, int nzMode, int nzSite) 
{
	if (nzSite < SIDE_LEFT_ || nzSite > SIDE_RIGHT_)
	{
		nzSite = SIDE_BOTH_;
	}

	int nReadIO[2] = {0,};
	// ������ B���̶� ���� ���� Ȯ�� �� ���� ���
	nReadIO[0] = !g_ioMgr.get_in_bit(stIO.i_Chk_BufferTryAccyDetection[SIDE_FRONT_][SIDE_LEFT_]);
	nReadIO[1] = !g_ioMgr.get_in_bit(stIO.i_Chk_BufferTryAccyDetection[SIDE_FRONT_][SIDE_RIGHT_]);

	if (nzSite == SIDE_LEFT_)
	{
		// [Right] ���� �Է��� ������ ����
		nReadIO[1] = nzOnOff;
	}
	else if (nzSite == SIDE_RIGHT_)
	{
		// [Left] ���� �Է��� ������ ����
		nReadIO[0] = nzOnOff;
	}

	// ���� �����ϴ� ��쿡 ���� üũ���� Ȯ��
	if (nzMode == CHK_EXIST_)
	{
		// ���� WithOut ������� Ȯ��
		// - ������� ������ �����Ƿ� �������� �ʱ� ������ ������ ������
		if (st_basic.n_mode_device == WITHOUT_DVC_)
		{
			if (st_map.nLoadingAccyBoxExist[1] == 0)
			{
				nReadIO[0] = st_map.nBufferCovAccyExist[0];
				nReadIO[1] = st_map.nBufferCovAccyExist[1];
			}
			else
			{
				nReadIO[0] = st_map.nBufferCovAccyExist[0];
				nReadIO[1] = st_map.nBufferCovAccyExist[1];
			}
		}
	}

	if (nReadIO[0] == IO_ON && nReadIO[1] == IO_ON)
	{
		return IO_ON;	// ���� ����
	}
	else	
	{
		return IO_OFF;	// ���� �̰���
	}
}

//==================================================================//
// �Ǽ��縮�� ���� �Ǵ� ���⿡�� Rear Buffer ���� Ȯ��
//==================================================================//
int CSeq_BufferAlignConv::OnCheck_RearAccySupply(int nzOnOff, int nzMode, int nzSite) 
{
	if (nzSite < SIDE_LEFT_ || nzSite > SIDE_RIGHT_)
	{
		nzSite = SIDE_BOTH_;
	}
	
	int nReadIO[2] = {0,};
	// ������ B���̶� ���� ���� Ȯ�� �� ���� ���
	nReadIO[0] = !g_ioMgr.get_in_bit(stIO.i_Chk_BufferTryAccyDetection[SIDE_REAR_][SIDE_LEFT_]);
	nReadIO[1] = !g_ioMgr.get_in_bit(stIO.i_Chk_BufferTryAccyDetection[SIDE_REAR_][SIDE_RIGHT_]);
	
	if (nzSite == SIDE_LEFT_)
	{
		// [Right] ���� �Է��� ������ ����
		nReadIO[1] = nzOnOff;
	}
	else if (nzSite == SIDE_RIGHT_)
	{
		// [Left] ���� �Է��� ������ ����
		nReadIO[0] = nzOnOff;
	}
	
	// ���� �����ϴ� ��쿡 ���� üũ���� Ȯ��
	if (nzMode == CHK_EXIST_)
	{
		// ���� WithOut ������� Ȯ��
		// - ������� ������ �����Ƿ� �������� �ʱ� ������ ������ ������
		if (st_basic.n_mode_device == WITHOUT_DVC_)
		{
			if (st_map.nLoadingAccyBoxExist[1] == 0)
			{
				nReadIO[0] = st_map.nBufferCovAccyExist[2];
				nReadIO[1] = st_map.nBufferCovAccyExist[3];
			}
			else
			{
				nReadIO[0] = st_map.nBufferCovAccyExist[2];
				nReadIO[1] = st_map.nBufferCovAccyExist[3];
			}
		}
	}
	
	if (nReadIO[0] == IO_ON && nReadIO[1] == IO_ON)
	{
		return IO_ON;	// ���� ����
	}
	else	
	{
		return IO_OFF;	// ���� �̰���
	}
}
//==================================================================//
// �Ǽ��縮�� ���� �Ǵ� ���⿡�� Middle Buffer ���� Ȯ��
//==================================================================//

int CSeq_BufferAlignConv::OnCheck_MiddleAccySupply(int nzOnOff, int nzMode, int nzSite)
{
	if (nzSite < SIDE_LEFT_ || nzSite > SIDE_RIGHT_)
	{
		nzSite = SIDE_BOTH_;
	}
	
	int nReadIO[2] = {0,};
	// ������ B���̶� ���� ���� Ȯ�� �� ���� ���
	nReadIO[0] = !g_ioMgr.get_in_bit(stIO.i_Chk_BufferTryAccyDetection[SIDE_MIDDLE][SIDE_LEFT_]);
	nReadIO[1] = !g_ioMgr.get_in_bit(stIO.i_Chk_BufferTryAccyDetection[SIDE_MIDDLE][SIDE_RIGHT_]);
	
	if (nzSite == SIDE_LEFT_)
	{
		// [Right] ���� �Է��� ������ ����
		nReadIO[1] = nzOnOff;
	}
	else if (nzSite == SIDE_RIGHT_)
	{
		// [Left] ���� �Է��� ������ ����
		nReadIO[0] = nzOnOff;
	}
	
	// ���� �����ϴ� ��쿡 ���� üũ���� Ȯ��
	if (nzMode == CHK_EXIST_)
	{
		// ���� WithOut ������� Ȯ��
		// - ������� ������ �����Ƿ� �������� �ʱ� ������ ������ ������
		if (st_basic.n_mode_device == WITHOUT_DVC_)
		{
			if (st_map.nLoadingAccyBoxExist[1] == 0)
			{
				nReadIO[0] = st_map.nBufferMiddleAccyExist[0];
				nReadIO[1] = st_map.nBufferMiddleAccyExist[1];
			}
			else
			{
				nReadIO[0] = st_map.nBufferMiddleAccyExist[0];
				nReadIO[1] = st_map.nBufferMiddleAccyExist[1];
			}
		}
	}
	
	if (nReadIO[0] == IO_ON && nReadIO[1] == IO_ON)
	{
		return IO_ON;	// ���� ����
	}
	else	
	{
		return IO_OFF;	// ���� �̰���
	}
}

//==================================================================//
// OUT �Ǽ��縮�� ���� �Ǵ� ������ ��ġ�� ����
//==================================================================//
int CSeq_BufferAlignConv::OnCheck_OutFrontAccySupply(int nzOnOff, int nzMode, int nzSite) 
{
	if (nzSite < SIDE_LEFT_ || nzSite > SIDE_RIGHT_)
	{
		nzSite = SIDE_BOTH_;
	}
	
	int nReadIO[2] = {0,};
	// ������ B���̶� ���� ���� Ȯ�� �� ���� ���
	nReadIO[0] = !g_ioMgr.get_in_bit(stIO.i_Chk_BufferTryAccyDetection[SIDE_OUT_FRONT][SIDE_LEFT_]);
	nReadIO[1] = !g_ioMgr.get_in_bit(stIO.i_Chk_BufferTryAccyDetection[SIDE_OUT_FRONT][SIDE_RIGHT_]);
	
	if (nzSite == SIDE_LEFT_)
	{
		// [Right] ���� �Է��� ������ ����
		nReadIO[1] = nzOnOff;
	}
	else if (nzSite == SIDE_RIGHT_)
	{
		// [Left] ���� �Է��� ������ ����
		nReadIO[0] = nzOnOff;
	}
	
	// ���� �����ϴ� ��쿡 ���� üũ���� Ȯ��
	if (nzMode == CHK_EXIST_)
	{
		// ���� WithOut ������� Ȯ��
		// - ������� ������ �����Ƿ� �������� �ʱ� ������ ������ ������
		if (st_basic.n_mode_device == WITHOUT_DVC_)
		{
			if (st_map.nLoadingAccyBoxExist[1] == 0)
			{
				nReadIO[0] = st_map.nBufferOutAccyExist[0];
				nReadIO[1] = st_map.nBufferOutAccyExist[1];
			}
			else
			{
				nReadIO[0] = st_map.nBufferOutAccyExist[0];
				nReadIO[1] = st_map.nBufferOutAccyExist[1];
			}
		}
	}
	
	if (nReadIO[0] == IO_ON && nReadIO[1] == IO_ON)
	{
		return IO_ON;	// ���� ����
	}
	else	
	{
		return IO_OFF;	// ���� �̰���
	}
}

int CSeq_BufferAlignConv::OnCheck_OutRearAccySupply(int nzOnOff, int nzMode, int nzSite) 
{
	if (nzSite < SIDE_LEFT_ || nzSite > SIDE_RIGHT_)
	{
		nzSite = SIDE_BOTH_;
	}
	
	int nReadIO[2] = {0,};
	// ������ B���̶� ���� ���� Ȯ�� �� ���� ���
	nReadIO[0] = !g_ioMgr.get_in_bit(stIO.i_Chk_BufferTryAccyDetection[SIDE_OUT_REAR][SIDE_LEFT_]);
	nReadIO[1] = !g_ioMgr.get_in_bit(stIO.i_Chk_BufferTryAccyDetection[SIDE_OUT_REAR][SIDE_RIGHT_]);
	
	if (nzSite == SIDE_LEFT_)
	{
		// [Right] ���� �Է��� ������ ����
		nReadIO[1] = nzOnOff;
	}
	else if (nzSite == SIDE_RIGHT_)
	{
		// [Left] ���� �Է��� ������ ����
		nReadIO[0] = nzOnOff;
	}
	
	// ���� �����ϴ� ��쿡 ���� üũ���� Ȯ��
	if (nzMode == CHK_EXIST_)
	{
		// ���� WithOut ������� Ȯ��
		// - ������� ������ �����Ƿ� �������� �ʱ� ������ ������ ������
		if (st_basic.n_mode_device == WITHOUT_DVC_)
		{
			if (st_map.nLoadingAccyBoxExist[1] == 0)
			{
				nReadIO[0] = st_map.nBufferOutAccyExist[0];
				nReadIO[1] = st_map.nBufferOutAccyExist[1];
			}
			else
			{
				nReadIO[0] = st_map.nBufferOutAccyExist[0];
				nReadIO[1] = st_map.nBufferOutAccyExist[1];
			}
		}
	}
	
	if (nReadIO[0] == IO_ON && nReadIO[1] == IO_ON)
	{
		return IO_ON;	// ���� ����
	}
	else	
	{
		return IO_OFF;	// ���� �̰���
	}
}

//==================================================================//
// �Ǽ��縮�� ���� �Ǵ� ������ ��ġ�� ����
//==================================================================//
int CSeq_BufferAlignConv::OnCheck_OutAccySupply(int nzOnOff, int nzMode, int nzSite) 
{
	if (nzSite < SIDE_LEFT_ || nzSite > SIDE_RIGHT_)
	{
		nzSite = SIDE_BOTH_;
	}
	
	int nReadIO[2] = {0,};
	// ������ B���̶� ���� ���� Ȯ�� �� ���� ���
	nReadIO[0] = !g_ioMgr.get_in_bit(stIO.i_Chk_OutLeftBufferAccyDetection);
	nReadIO[1] = !g_ioMgr.get_in_bit(stIO.i_Chk_OutRightBufferAccyDetection);
	
	if (nzSite == SIDE_LEFT_)
	{
		// [Right] ���� �Է��� ������ ����
		nReadIO[1] = nzOnOff;
	}
	else if (nzSite == SIDE_RIGHT_)
	{
		// [Left] ���� �Է��� ������ ����
		nReadIO[0] = nzOnOff;
	}
	
	// ���� �����ϴ� ��쿡 ���� üũ���� Ȯ��
	if (nzMode == CHK_EXIST_)
	{
		// ���� WithOut ������� Ȯ��
		// - ������� ������ �����Ƿ� �������� �ʱ� ������ ������ ������
		if (st_basic.n_mode_device == WITHOUT_DVC_)
		{
			nReadIO[0] = nzOnOff;
			nReadIO[1] = nzOnOff;
		}
	}
	
	if (nReadIO[0] == IO_ON && nReadIO[1] == IO_ON)
	{
		return IO_ON;	// ���� ����
	}
	else	
	{
		return IO_OFF;	// ���� �̰���
	}
}

//==================================================================//
// �Ǽ��縮�� ���ް� ���޿� �־� ������ �ٲپ� �ִ� �Ǹ���
// - IO_ON = Fw �����̸�, ���� �޴� ����
// - IO_OFF = Bw �����̸�, ���� �ϴ� ����
//==================================================================//
void CSeq_BufferAlignConv::OnSet_CylBufferFrontTrayPitch(int nzOnOff) 
{
	g_ioMgr.set_out_bit(stIO.o_Slop_Variable_Front_Left_OpenClose, nzOnOff);
	g_ioMgr.set_out_bit(stIO.o_Slop_Variable_Front_Right_OpenClose, !nzOnOff);
	
	m_bflag_BufferFrontTrayPitch = true;
	m_lWait_BufferFrontTrayPitch[0] = GetCurrentTime();
}

int CSeq_BufferAlignConv::OnGet_CylBufferFrontTrayPitch(int nzOnOff, int nzSite)  
{
	int nFunRet = CTL_PROCEED;
	
	int nReadIO[4] = {0,};
	nReadIO[0] = g_ioMgr.get_in_bit(stIO.i_Slop_Variable_Font_Left_Open_Check);
	nReadIO[1] = g_ioMgr.get_in_bit(stIO.i_Slop_Variable_Font_Left_Close_Check);
	nReadIO[2] = g_ioMgr.get_in_bit(stIO.i_Slop_Variable_Font_Right_Open_Check);
	nReadIO[3] = g_ioMgr.get_in_bit(stIO.i_Slop_Variable_Font_Right_Close_Check);
	
	if (nzSite == SIDE_FW_)
	{
		// [Bw] ���� �Է��� ������ ����
		nReadIO[1] = !nzOnOff;
	}
	else if (nzSite == SIDE_BW_)
	{
		// [Fw] ���� �Է��� ������ ����
		nReadIO[0] =  !nzOnOff;
	}
	
	m_lWait_BufferFrontTrayPitch[1] = GetCurrentTime();
	m_lWait_BufferFrontTrayPitch[2] = m_lWait_BufferFrontTrayPitch[1] - m_lWait_BufferFrontTrayPitch[0];
	
	if (m_lWait_BufferFrontTrayPitch[2] < 0)
	{
		m_lWait_BufferFrontTrayPitch[0] = GetCurrentTime();
	}
	
	if (nzOnOff == IO_OFF)		// �Ǽ��縮�� �����ϴ� ����
	{
		if (m_bflag_BufferFrontTrayPitch == true &&
			nReadIO[0] == IO_OFF && nReadIO[1] == IO_ON && nReadIO[2] == IO_OFF && nReadIO[3] == IO_ON )
		{
			m_lWait_BufferFrontTrayPitch[0] = GetCurrentTime();
			m_bflag_BufferFrontTrayPitch = false;
		}
		else if (m_bflag_BufferFrontTrayPitch == false &&
			nReadIO[0] == IO_OFF && nReadIO[1] == IO_ON && nReadIO[2] == IO_OFF && nReadIO[3] == IO_ON )
		{
			if (m_lWait_BufferFrontTrayPitch[2] > st_time.nWait_Off[CYL_ACCY_BUFF_PITCH])
			{
				nFunRet = CTL_GOOD;	// [OFF:DOWN]
			}
		}
		else
		{
			if (m_lWait_BufferFrontTrayPitch[2] > st_time.nWait_Limit[CYL_ACCY_BUFF_PITCH])
			{
				// 140003 0 00 "PS1005_ACCY_CONV_BUFFER_TYAY_PITCH_BW_CHK_ERR."
				alarm.mstr_code		= "140003";
				alarm.mn_count_mode	= 0;
				alarm.mn_type_mode	= eWARNING;
				st_work.nEqpStatus	= dWARNING;

				CTL_Lib.Alarm_Error_Occurrence( 3000, CTL_dWARNING, alarm.mstr_code );	
				nFunRet = CTL_ERROR;
			}
		}
	}
	else		// �Ǽ��縮�� ���� �޴� ����
	{
		if (m_bflag_BufferFrontTrayPitch == true &&
			nReadIO[0] == IO_ON && nReadIO[1] == IO_OFF && nReadIO[2] == IO_ON && nReadIO[3] == IO_OFF)
		{
			m_lWait_BufferFrontTrayPitch[0] = GetCurrentTime();
			m_bflag_BufferFrontTrayPitch = false;
		}
		else if (m_bflag_BufferFrontTrayPitch == false &&
			nReadIO[0] == IO_ON && nReadIO[1] == IO_OFF && nReadIO[2] == IO_ON && nReadIO[3] == IO_OFF)
		{
			if (m_lWait_BufferFrontTrayPitch[2] > st_time.nWait_On[CYL_ACCY_BUFF_PITCH])
			{
				nFunRet = CTL_GOOD;	// [OFF:DOWN]
			}
		}
		else
		{
			if (m_lWait_BufferFrontTrayPitch[2] > st_time.nWait_Limit[CYL_ACCY_BUFF_PITCH])
			{
				// 140002 0 00 "PS1004_ACCY_CONV_BUFFER_TYAY_PITCH_FW_CHK_ERR."
				alarm.mstr_code		= "140002";
				alarm.mn_count_mode	= 0;
				alarm.mn_type_mode	= eWARNING;
				st_work.nEqpStatus	= dWARNING;
				CTL_Lib.Alarm_Error_Occurrence( 3000, CTL_dWARNING, alarm.mstr_code );
				nFunRet = CTL_ERROR;
			}
		}
	}
	return nFunRet;
}

void CSeq_BufferAlignConv::OnSet_CylBufferRearTrayPitch(int nzOnOff) 
{
	g_ioMgr.set_out_bit(stIO.o_Slop_Variable_Rear_Left_OpenClose, nzOnOff);
	g_ioMgr.set_out_bit(stIO.o_Slop_Variable_Rear_Right_OpenClose, !nzOnOff);
	
	m_bflag_BufferRearTrayPitch = true;
	m_lWait_BufferRearTrayPitch[0] = GetCurrentTime();

}

int CSeq_BufferAlignConv::OnGet_CylBufferRearTrayPitch(int nzOnOff, int nzSite)  
{
	int nFunRet = CTL_PROCEED;
	
	int nReadIO[4] = {0,};
	nReadIO[0] = g_ioMgr.get_in_bit(stIO.i_Slop_Variable_Rear_Left_Open_Check);
	nReadIO[1] = g_ioMgr.get_in_bit(stIO.i_Slop_Variable_Rear_Left_Close_Check);
	nReadIO[2] = g_ioMgr.get_in_bit(stIO.i_Slop_Variable_Rear_Right_Open_Check);
	nReadIO[3] = g_ioMgr.get_in_bit(stIO.i_Slop_Variable_Rear_Right_Close_Check);

	if (nzSite == SIDE_FW_)
	{
		// [Bw] ���� �Է��� ������ ����
		nReadIO[1] = !nzOnOff;
	}
	else if (nzSite == SIDE_BW_)
	{
		// [Fw] ���� �Է��� ������ ����
		nReadIO[0] =  !nzOnOff;
	}
	
	m_lWait_BufferRearTrayPitch[1] = GetCurrentTime();
	m_lWait_BufferRearTrayPitch[2] = m_lWait_BufferRearTrayPitch[1] - m_lWait_BufferRearTrayPitch[0];
	
	if (m_lWait_BufferRearTrayPitch[2] < 0)
	{
		m_lWait_BufferRearTrayPitch[0] = GetCurrentTime();
	}
	
	if (nzOnOff == IO_OFF)		// �Ǽ��縮�� �����ϴ� ����
	{
		if (m_bflag_BufferFrontTrayPitch == true &&
			nReadIO[0] == IO_OFF && nReadIO[1] == IO_ON )
		{
			m_lWait_BufferRearTrayPitch[0] = GetCurrentTime();
			m_bflag_BufferFrontTrayPitch = false;
		}
		else if (m_bflag_BufferFrontTrayPitch == false &&
			nReadIO[0] == IO_OFF && nReadIO[1] == IO_ON)
		{
			if (m_lWait_BufferRearTrayPitch[2] > st_time.nWait_Off[CYL_ACCY_BUFF_PITCH])
			{
				nFunRet = CTL_GOOD;	// [OFF:DOWN]
			}
		}
		else
		{
			if (m_lWait_BufferRearTrayPitch[2] > st_time.nWait_Limit[CYL_ACCY_BUFF_PITCH])
			{
				// 140003 0 00 "PS1005_ACCY_CONV_BUFFER_TYAY_PITCH_BW_CHK_ERR."
				alarm.mstr_code		= "140003";
				alarm.mn_count_mode	= 0;
				alarm.mn_type_mode	= eWARNING;
				st_work.nEqpStatus	= dWARNING;
				
				CTL_Lib.Alarm_Error_Occurrence( 3000, CTL_dWARNING, alarm.mstr_code );	
				nFunRet = CTL_ERROR;
			}
		}
	}
	else		// �Ǽ��縮�� ���� �޴� ����
	{
		if (m_bflag_BufferFrontTrayPitch == true &&
			nReadIO[0] == IO_ON && nReadIO[1] == IO_OFF)
		{
			m_lWait_BufferRearTrayPitch[0] = GetCurrentTime();
			m_bflag_BufferFrontTrayPitch = false;
		}
		else if (m_bflag_BufferFrontTrayPitch == false &&
			nReadIO[0] == IO_ON && nReadIO[1] == IO_OFF)
		{
			if (m_lWait_BufferRearTrayPitch[2] > st_time.nWait_On[CYL_ACCY_BUFF_PITCH])
			{
				nFunRet = CTL_GOOD;	// [OFF:DOWN]
			}
		}
		else
		{
			if (m_lWait_BufferRearTrayPitch[2] > st_time.nWait_Limit[CYL_ACCY_BUFF_PITCH])
			{
				// 140002 0 00 "PS1004_ACCY_CONV_BUFFER_TYAY_PITCH_FW_CHK_ERR."
				alarm.mstr_code		= "140002";
				alarm.mn_count_mode	= 0;
				alarm.mn_type_mode	= eWARNING;
				st_work.nEqpStatus	= dWARNING;
				CTL_Lib.Alarm_Error_Occurrence( 3000, CTL_dWARNING, alarm.mstr_code );
				nFunRet = CTL_ERROR;
			}
		}
	}
	return nFunRet;
}

//==================================================================//
// �Ǽ��縮 ���ް� ������ �ޱ� ���� ��, �Ʒ��� �����̴� �Ǹ���
// - IO_ON = UP ���¿��� ����
// - IO_OFF = DOWN ���¿��� ����
//==================================================================//
void CSeq_BufferAlignConv::OnSet_CylBufferTrayPathAlign(int nzOnOff) 
{
	g_ioMgr.set_out_bit(stIO.o_Cyl_BufferTryPathAlignUp, nzOnOff);
	g_ioMgr.set_out_bit(stIO.o_Cyl_BufferTryPathAlignDn, !nzOnOff);

	m_bflag_BufferTrayPathAlign = true;
	m_lWait_BufferTrayPathAlign[0] = GetCurrentTime();
}

int CSeq_BufferAlignConv::OnGet_CylBufferTrayPathAlign(int nzOnOff, int nzSite) 
{
	int nFunRet = CTL_PROCEED;

	int nReadIO[2] = {0,};
	nReadIO[0] = g_ioMgr.get_in_bit(stIO.i_Chk_BufferTryPathAlignUp);
	nReadIO[1] = g_ioMgr.get_in_bit(stIO.i_Chk_BufferTryPathAlignDn);

	if (nzSite == SIDE_UP_)
	{
		// [DOWN] ���� �Է��� ������ ����
		nReadIO[1] = !nzOnOff;
	}
	else if (nzSite == SIDE_DOWN_)
	{
		// [UP] ���� �Է��� ������ ����
		nReadIO[0] =  !nzOnOff;
	}

	m_lWait_BufferTrayPathAlign[1] = GetCurrentTime();
	m_lWait_BufferTrayPathAlign[2] = m_lWait_BufferTrayPathAlign[1] - m_lWait_BufferTrayPathAlign[0];

	if (m_lWait_BufferTrayPathAlign[2] < 0)
	{
		m_lWait_BufferTrayPathAlign[0] = GetCurrentTime();
	}

	if (nzOnOff == IO_OFF)		// �Ǽ��縮�� ���� �ϴ� ��ġ
	{
		if (m_bflag_BufferTrayPathAlign == true &&
			nReadIO[0] == IO_OFF && nReadIO[1] == IO_ON )
		{
			m_lWait_BufferTrayPathAlign[0] = GetCurrentTime();
			m_bflag_BufferTrayPathAlign = false;
		}
		else if (m_bflag_BufferTrayPathAlign == false &&
			nReadIO[0] == IO_OFF && nReadIO[1] == IO_ON)
		{
			if (m_lWait_BufferTrayPathAlign[2] > st_time.nWait_Off[CYL_ACCY_BUFF_PATH_ALIGN])
			{
				nFunRet = CTL_GOOD;	// [OFF:DOWN]
			}
		}
		else
		{
			if (m_lWait_BufferTrayPathAlign[2] > st_time.nWait_Limit[CYL_ACCY_BUFF_PATH_ALIGN])
			{
				// 140004 0 00 "PS1101_ACCY_CONV_BUFFER_TYAY_PATH_ALIGN_DOWN_CHK_ERR."
				alarm.mstr_code		= "140004";
				alarm.mn_count_mode	= 0;
				alarm.mn_type_mode	= eWARNING;
				st_work.nEqpStatus	= dWARNING;

				nFunRet = CTL_ERROR;
			}
		}
	}
	else		// �Ǽ��縮�� ���� �޴� ��ġ
	{
		if (m_bflag_BufferTrayPathAlign == true &&
			nReadIO[0] == IO_ON && nReadIO[1] == IO_OFF)
		{
			m_lWait_BufferTrayPathAlign[0] = GetCurrentTime();
			m_bflag_BufferTrayPathAlign = false;
		}
		else if (m_bflag_BufferTrayPathAlign == false &&
			nReadIO[0] == IO_ON && nReadIO[1] == IO_OFF)
		{
			if (m_lWait_BufferTrayPathAlign[2] > st_time.nWait_On[CYL_ACCY_BUFF_PATH_ALIGN])
			{
				nFunRet = CTL_GOOD;	// [OFF:DOWN]
			}
		}
		else
		{
			if (m_lWait_BufferTrayPathAlign[2] > st_time.nWait_Limit[CYL_ACCY_BUFF_PATH_ALIGN])
			{
				// 140005 0 00 "PS1100_ACCY_CONV_BUFFER_TYAY_PATH_ALIGN_UP_CHK_ERR."
				alarm.mstr_code		= "140005";

				alarm.mn_count_mode	= 0;
				alarm.mn_type_mode	= eWARNING;
				st_work.nEqpStatus	= dWARNING;

				nFunRet = CTL_ERROR;
			}
		}
	}
	return nFunRet;
}

//==================================================================//
// �Ǽ��縮�� 2���� ���� �ϱ����� Front�� Rear�� ������
// Rear OFF�� �Ͽ� 2���� �Ǽ��縮�� 1�� ����, 
// Rear ON�� �Բ� Front OFF �Ͽ� Front�� �ִ� �Ǽ��縮�� Rear ����,
//==================================================================//
void CSeq_BufferAlignConv::OnSet_CylStopper(int nzOnOff, int nPos, int nzSite) 
{
// #define SIDE_FRONT_				0
// #define SIDE_REAR_				1
// #define SIDE_MIDDLE				2
// #define SIDE_OUT_FRONT			3
// #define SIDE_OUT_REAR			4

	if( nPos == SIDE_FRONT_ )
	{
		g_ioMgr.set_out_bit(stIO.o_Cyl_BufferAccyConvFrontStopper,	nzOnOff);
		g_ioMgr.set_out_bit(stIO.o_Cyl_BufferAccyConvRearStopper,	nzOnOff);
	}
	else if( nPos == SIDE_REAR_ )
	{
		g_ioMgr.set_out_bit(stIO.o_Cyl_BufferAccyConvFrontStopper,	nzOnOff);
		g_ioMgr.set_out_bit(stIO.o_Cyl_BufferAccyConvRearStopper,	nzOnOff);
	}
	else if( nPos == SIDE_MIDDLE )
	{
		g_ioMgr.set_out_bit(stIO.o_Slop_Seperator_Stopper_UpDn,	nzOnOff);
	}
	else if( nPos == SIDE_OUT_FRONT )
	{
		g_ioMgr.set_out_bit(stIO.o_Out_Front_Stopper_UpDn,	nzOnOff);
		g_ioMgr.set_out_bit(stIO.o_Out_Rear_Stopper_UpDn,	nzOnOff);
	}
	else// if( nPos == SIDE_OUT_REAR )
	{
		g_ioMgr.set_out_bit(stIO.o_Out_Front_Stopper_UpDn,	nzOnOff);
		g_ioMgr.set_out_bit(stIO.o_Out_Rear_Stopper_UpDn,	nzOnOff);
	}

	m_bflag_Stopper[nPos] = true;
	m_lWait_Stopper[nPos][0] = GetCurrentTime();
}

int CSeq_BufferAlignConv::OnGet_CylStopper(int nzOnOff, int nPos, int nzSite) 
{
// #define SIDE_FRONT_				0
// #define SIDE_REAR_				1
// #define SIDE_MIDDLE				2
// #define SIDE_OUT_FRONT			3
// #define SIDE_OUT_REAR			4
	int nFunRet = CTL_PROCEED;
	if (nzSite < SIDE_FRONT_ || nzSite > SIDE_REAR_)
	{
		nzSite = SIDE_BOTH_;
	}

	int nReadIO[4] = {0,};

	if( nPos == SIDE_FRONT_ )
	{
		nReadIO[0] = g_ioMgr.get_in_bit(stIO.i_Chk_BufferAccyConvStopper[SIDE_FRONT_][0]);//UP
		nReadIO[1] = g_ioMgr.get_in_bit(stIO.i_Chk_BufferAccyConvStopper[SIDE_FRONT_][1]);//DOWN
		nReadIO[2] = g_ioMgr.get_in_bit(stIO.i_Chk_BufferAccyConvStopper[SIDE_REAR_][0]);
		nReadIO[3] = g_ioMgr.get_in_bit(stIO.i_Chk_BufferAccyConvStopper[SIDE_REAR_][1]);

	}
	else if( nPos == SIDE_REAR_ )
	{
		nReadIO[0] = g_ioMgr.get_in_bit(stIO.i_Chk_BufferAccyConvStopper[SIDE_FRONT_][0]);
		nReadIO[1] = g_ioMgr.get_in_bit(stIO.i_Chk_BufferAccyConvStopper[SIDE_FRONT_][1]);
		nReadIO[2] = g_ioMgr.get_in_bit(stIO.i_Chk_BufferAccyConvStopper[SIDE_REAR_][0]);
		nReadIO[3] = g_ioMgr.get_in_bit(stIO.i_Chk_BufferAccyConvStopper[SIDE_REAR_][1]);
	}
	else if( nPos == SIDE_MIDDLE )
	{
		nReadIO[0] = g_ioMgr.get_in_bit(stIO.i_Chk_BufferAccyConvStopper[SIDE_MIDDLE][0]);
		nReadIO[1] = g_ioMgr.get_in_bit(stIO.i_Chk_BufferAccyConvStopper[SIDE_MIDDLE][1]);
		nReadIO[2] = IO_OFF;
		nReadIO[3] = IO_OFF;
	}
	else if( nPos == SIDE_OUT_FRONT )
	{
		nReadIO[0] = g_ioMgr.get_in_bit(stIO.i_Chk_BufferAccyConvStopper[SIDE_OUT_FRONT][0]);	// UP
		nReadIO[1] = g_ioMgr.get_in_bit(stIO.i_Chk_BufferAccyConvStopper[SIDE_OUT_FRONT][1]);	// Down
		nReadIO[2] = g_ioMgr.get_in_bit(stIO.i_Chk_BufferAccyConvStopper[SIDE_OUT_REAR][0]);
		nReadIO[3] = g_ioMgr.get_in_bit(stIO.i_Chk_BufferAccyConvStopper[SIDE_OUT_REAR][1]);
	}
	else// if( nPos == SIDE_OUT_REAR )
	{
		nReadIO[0] = g_ioMgr.get_in_bit(stIO.i_Chk_BufferAccyConvStopper[SIDE_OUT_FRONT][0]);	// UP
		nReadIO[1] = g_ioMgr.get_in_bit(stIO.i_Chk_BufferAccyConvStopper[SIDE_OUT_FRONT][1]);	// Down
		nReadIO[2] = g_ioMgr.get_in_bit(stIO.i_Chk_BufferAccyConvStopper[SIDE_OUT_REAR][0]);
		nReadIO[3] = g_ioMgr.get_in_bit(stIO.i_Chk_BufferAccyConvStopper[SIDE_OUT_REAR][1]);
	}
	
	if (nzSite == SIDE_LEFT_)
	{
		// [Rear] ���� �Է��� ������ ����
		nReadIO[2] = !nzOnOff;
		nReadIO[3] = !nzOnOff;
	}
	else if (nzSite == SIDE_RIGHT_)
	{
		// [Front] ���� �Է��� ������ ����
		nReadIO[0] = !nzOnOff;
		nReadIO[1] = !nzOnOff;
	}

	m_lWait_Stopper[nPos][1] = GetCurrentTime();
	m_lWait_Stopper[nPos][2] = m_lWait_Stopper[nPos][1] - m_lWait_Stopper[nPos][0];
	if (m_lWait_Stopper[nPos][2] < 0)
	{
		m_lWait_Stopper[nPos][0] = GetCurrentTime();
	}

	if (nzOnOff == IO_OFF)
	{
		if( nPos == SIDE_FRONT_ )
		{
			if (m_bflag_Stopper[nPos] == true &&
				nReadIO[0] == IO_OFF && nReadIO[1] == IO_ON && nReadIO[2] == IO_OFF && nReadIO[3] == IO_ON)
			{
				m_lWait_Stopper[nPos][0] = GetCurrentTime();
				m_bflag_Stopper[nPos] = false;
			}
			else if (m_bflag_Stopper[nPos] == false &&
				nReadIO[0] == IO_OFF && nReadIO[1] == IO_ON && nReadIO[2] == IO_OFF && nReadIO[3] == IO_ON)
			{
				if (m_lWait_Stopper[nPos][2] > st_time.nWait_Off[CYL_ACCY_BUFF_IN_STOPPER])
				{
					nFunRet = CTL_GOOD;	// [OFF:DOWN]
				}
			}
			else
			{
				if (m_lWait_Stopper[nPos][2] > st_time.nWait_Limit[CYL_ACCY_BUFF_IN_STOPPER])
				{
					//139000 0 00 "PS1406_PS1407_BUFFER_ACCY_CONV_FRONT_STOPPER_DOWN_CHK_ERR."
					alarm.mstr_code		= "139000";
					alarm.mn_count_mode	= 0;
					alarm.mn_type_mode	= eWARNING;
					st_work.nEqpStatus	= dWARNING;
					nFunRet = CTL_ERROR;
				}
			}
		}
		else if( nPos == SIDE_REAR_ )
		{
			if (m_bflag_Stopper[nPos] == true &&
				nReadIO[0] == IO_OFF && nReadIO[1] == IO_OFF && nReadIO[2] == IO_OFF && nReadIO[3] == IO_OFF)
			{
				m_lWait_Stopper[nPos][0] = GetCurrentTime();
				m_bflag_Stopper[nPos] = false;
			}
			else if (m_bflag_Stopper[nPos] == false &&
				nReadIO[0] == IO_OFF && nReadIO[1] == IO_OFF && nReadIO[2] == IO_OFF && nReadIO[3] == IO_OFF)
			{
				if (m_lWait_Stopper[nPos][2] > st_time.nWait_Off[CYL_ACCY_BUFF_IN_STOPPER])
				{
					nFunRet = CTL_GOOD;	// [OFF:DOWN]
				}
			}
			else
			{
				if (m_lWait_Stopper[nPos][2] > st_time.nWait_Limit[CYL_ACCY_BUFF_IN_STOPPER])
				{
					//139100 0 00 "PS1408_PS1409_BUFFER_ACCY_CONV_REAR_STOPPER_DOWN_CHK_ERR."
					alarm.mstr_code		= "139100";
					alarm.mn_count_mode	= 0;
					alarm.mn_type_mode	= eWARNING;
					st_work.nEqpStatus	= dWARNING;
					nFunRet = CTL_ERROR;
				}
			}
		}
		else if( nPos == SIDE_MIDDLE )
		{
			if (m_bflag_Stopper[nPos] == true && nReadIO[0] == IO_OFF && nReadIO[1] == IO_OFF)
			{
				m_lWait_Stopper[nPos][0] = GetCurrentTime();
				m_bflag_Stopper[nPos] = false;
			}
			else if (m_bflag_Stopper[nPos] == false && nReadIO[0] == IO_OFF && nReadIO[1] == IO_OFF)
			{
				if (m_lWait_Stopper[nPos][2] > st_time.nWait_Off[CYL_ACCY_BUFF_IN_STOPPER])
				{
					nFunRet = CTL_GOOD;	// [OFF:DOWN]
				}
			}
			else
			{
				if (m_lWait_Stopper[nPos][2] > st_time.nWait_Limit[CYL_ACCY_BUFF_IN_STOPPER])
				{
					//139200 0 00 "PS1000_PS1001_MIDDLE_BUFFER_ACCY_CONV_STOPPER_DOWN_CHK_ERR."
					alarm.mstr_code		= "139200";
					alarm.mn_count_mode	= 0;
					alarm.mn_type_mode	= eWARNING;
					st_work.nEqpStatus	= dWARNING;
					nFunRet = CTL_ERROR;
				}
			}
		}
		else if( nPos == SIDE_OUT_FRONT )
		{
			if (m_bflag_Stopper[nPos] == true && 
				nReadIO[0] == IO_OFF && nReadIO[1] == IO_ON && nReadIO[2] == IO_OFF && nReadIO[3] == IO_ON)
			{
				m_lWait_Stopper[nPos][0] = GetCurrentTime();
				m_bflag_Stopper[nPos] = false;
			}
			else if (m_bflag_Stopper[nPos] == false &&
				nReadIO[0] == IO_OFF && nReadIO[1] == IO_ON && nReadIO[2] == IO_OFF && nReadIO[3] == IO_ON)
			{
				if (m_lWait_Stopper[nPos][2] > st_time.nWait_Off[CYL_ACCY_BUFF_IN_STOPPER])
				{
					nFunRet = CTL_GOOD;	// [OFF:DOWN]
				}
			}
			else
			{
				if (m_lWait_Stopper[nPos][2] > st_time.nWait_Limit[CYL_ACCY_BUFF_IN_STOPPER])
				{
					//139300 0 00 "PS1002_PS1003_OUT_BUFFER_ACCY_CONV_FRONT_STOPPER_DOWN_CHK_ERR."
					alarm.mstr_code		= "139300";
					alarm.mn_count_mode	= 0;
					alarm.mn_type_mode	= eWARNING;
					st_work.nEqpStatus	= dWARNING;
					nFunRet = CTL_ERROR;
				}
			}			
		}
		else// if( nPos == SIDE_OUT_REAR )
		{
			if (m_bflag_Stopper[nPos] == true && 
				nReadIO[0] == IO_OFF && nReadIO[1] == IO_OFF && nReadIO[2] == IO_OFF && nReadIO[3] == IO_OFF)
			{
				m_lWait_Stopper[nPos][0] = GetCurrentTime();
				m_bflag_Stopper[nPos] = false;
			}
			else if (m_bflag_Stopper[nPos] == false &&
				nReadIO[0] == IO_OFF && nReadIO[1] == IO_OFF && nReadIO[2] == IO_OFF && nReadIO[3] == IO_OFF)
			{
				if (m_lWait_Stopper[nPos][2] > st_time.nWait_Off[CYL_ACCY_BUFF_IN_STOPPER])
				{
					nFunRet = CTL_GOOD;	// [OFF:DOWN]
				}
			}
			else
			{
				if (m_lWait_Stopper[nPos][2] > st_time.nWait_Limit[CYL_ACCY_BUFF_IN_STOPPER])
				{
					//139400 0 00 "PS1004_PS1005_OUT_BUFFER_ACCY_CONV_REAR_STOPPER_DOWN_CHK_ERR."
					//139401 0 00 "PS1004_PS1005_OUT_BUFFER_ACCY_CONV_REAR_STOPPER_UP_CHK_ERR."
					alarm.mstr_code		= "139400";
					alarm.mn_count_mode	= 0;
					alarm.mn_type_mode	= eWARNING;
					st_work.nEqpStatus	= dWARNING;
					nFunRet = CTL_ERROR;
				}
			}
		}

	}
	else
	{
		if( nPos == SIDE_FRONT_ )
		{
			if (m_bflag_Stopper[nPos] == true &&
				nReadIO[1] == IO_ON && nReadIO[0] == IO_ON && nReadIO[3] == IO_ON && nReadIO[2] == IO_ON)
			{
				m_lWait_Stopper[nPos][0] = GetCurrentTime();
				m_bflag_Stopper[nPos] = false;
			}
			else if (m_bflag_Stopper[nPos] == false &&
				nReadIO[1] == IO_ON && nReadIO[0] == IO_ON && nReadIO[3] == IO_ON && nReadIO[2] == IO_ON)
			{
				if (m_lWait_Stopper[nPos][2] > st_time.nWait_On[CYL_ACCY_BUFF_IN_STOPPER])
				{
					nFunRet = CTL_GOOD;	// [OFF:DOWN]
				}
			}
			else
			{
				if (m_lWait_Stopper[nPos][2] > st_time.nWait_Limit[CYL_ACCY_BUFF_IN_STOPPER])
				{
					//139001 0 00 "PS1406_PS1407_BUFFER_ACCY_CONV_FRONT_STOPPER_UP_CHK_ERR."
					alarm.mstr_code		= "139001";
					alarm.mn_count_mode	= 0;
					alarm.mn_type_mode	= eWARNING;
					st_work.nEqpStatus	= dWARNING;
					nFunRet = CTL_ERROR;
				}
			}
		}
		else if( nPos == SIDE_REAR_ )
		{
			if (m_bflag_Stopper[nPos] == true && nReadIO[2] == IO_ON && nReadIO[3] == IO_ON)
			{
				m_lWait_Stopper[nPos][0] = GetCurrentTime();
				m_bflag_Stopper[nPos] = false;
			}
			else if (m_bflag_Stopper[nPos] == false && nReadIO[2] == IO_ON && nReadIO[3] == IO_ON)
			{
				if (m_lWait_Stopper[nPos][2] > st_time.nWait_Off[CYL_ACCY_BUFF_IN_STOPPER])
				{
					nFunRet = CTL_GOOD;	// [OFF:DOWN]
				}
			}
			else
			{
				if (m_lWait_Stopper[nPos][2] > st_time.nWait_Limit[CYL_ACCY_BUFF_IN_STOPPER])
				{
					//139101 0 00 "PS1408_PS1409_BUFFER_ACCY_CONV_REAR_STOPPER_UP_CHK_ERR."
					alarm.mstr_code		= "139101";
					alarm.mn_count_mode	= 0;
					alarm.mn_type_mode	= eWARNING;
					st_work.nEqpStatus	= dWARNING;
					nFunRet = CTL_ERROR;
				}
			}
		}
		else if( nPos == SIDE_MIDDLE )
		{
			if (m_bflag_Stopper[nPos] == true && nReadIO[0] == IO_ON && nReadIO[1] == IO_ON)
			{
				m_lWait_Stopper[nPos][0] = GetCurrentTime();
				m_bflag_Stopper[nPos] = false;
			}
			else if (m_bflag_Stopper[nPos] == false && nReadIO[0] == IO_ON && nReadIO[1] == IO_ON)
			{
				if (m_lWait_Stopper[nPos][2] > st_time.nWait_Off[CYL_ACCY_BUFF_IN_STOPPER])
				{
					nFunRet = CTL_GOOD;	// [OFF:DOWN]
				}
			}
			else
			{
				if (m_lWait_Stopper[nPos][2] > st_time.nWait_Limit[CYL_ACCY_BUFF_IN_STOPPER])
				{
					//139201 0 00 "PS1000_PS1001_MIDDLE_BUFFER_ACCY_CONV_STOPPER_UP_CHK_ERR."
					alarm.mstr_code		= "139201";
					alarm.mn_count_mode	= 0;
					alarm.mn_type_mode	= eWARNING;
					st_work.nEqpStatus	= dWARNING;
					nFunRet = CTL_ERROR;
				}
			}
		}
		else if( nPos == SIDE_OUT_FRONT )
		{
			if (m_bflag_Stopper[nPos] == true && 
				nReadIO[0] == IO_ON && nReadIO[1] == IO_ON && nReadIO[2] == IO_ON && nReadIO[3] == IO_ON)
			{
				m_lWait_Stopper[nPos][0] = GetCurrentTime();
				m_bflag_Stopper[nPos] = false;
			}
			else if (m_bflag_Stopper[nPos] == false &&
				nReadIO[0] == IO_ON && nReadIO[1] == IO_ON && nReadIO[2] == IO_ON && nReadIO[3] == IO_ON)
			{
				if (m_lWait_Stopper[nPos][2] > st_time.nWait_Off[CYL_ACCY_BUFF_IN_STOPPER])
				{
					nFunRet = CTL_GOOD;	// [OFF:DOWN]
				}
			}
			else
			{
				if (m_lWait_Stopper[nPos][2] > st_time.nWait_Limit[CYL_ACCY_BUFF_IN_STOPPER])
				{
					//139301 0 00 "PS1002_PS1003_OUT_BUFFER_ACCY_CONV_FRONT_STOPPER_UP_CHK_ERR."
					alarm.mstr_code		= "139301";
					alarm.mn_count_mode	= 0;
					alarm.mn_type_mode	= eWARNING;
					st_work.nEqpStatus	= dWARNING;
					nFunRet = CTL_ERROR;
				}
			}			
		}
		else// if( nPos == SIDE_OUT_REAR )
		{
			if (m_bflag_Stopper[nPos] == true && nReadIO[2] == IO_ON && nReadIO[3] == IO_ON)
			{
				m_lWait_Stopper[nPos][0] = GetCurrentTime();
				m_bflag_Stopper[nPos] = false;
			}
			else if (m_bflag_Stopper[nPos] == true && nReadIO[2] == IO_ON && nReadIO[3] == IO_ON)
			{
				if (m_lWait_Stopper[nPos][2] > st_time.nWait_Off[CYL_ACCY_BUFF_IN_STOPPER])
				{
					nFunRet = CTL_GOOD;	// [OFF:DOWN]
				}
			}
			else
			{
				if (m_lWait_Stopper[nPos][2] > st_time.nWait_Limit[CYL_ACCY_BUFF_IN_STOPPER])
				{
					//139401 0 00 "PS1004_PS1005_OUT_BUFFER_ACCY_CONV_REAR_STOPPER_UP_CHK_ERR."
					alarm.mstr_code		= "139401";
					alarm.mn_count_mode	= 0;
					alarm.mn_type_mode	= eWARNING;
					st_work.nEqpStatus	= dWARNING;
					nFunRet = CTL_ERROR;
				}
			}
		}


	}
	return nFunRet;
}
