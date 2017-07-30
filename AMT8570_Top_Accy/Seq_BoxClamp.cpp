// Seq_BoxClamp.cpp: implementation of the CSeq_BoxClamp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "handler.h"
#include "Seq_BoxClamp.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

long m_lTime_Without[3];

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CSeq_BoxClamp	SeqBoxClamp;

CSeq_BoxClamp::CSeq_BoxClamp()
{

}

CSeq_BoxClamp::~CSeq_BoxClamp()
{

}

//==================================================================//
// ������ ����
//==================================================================//
void CSeq_BoxClamp::OnSeq_RUN(void) 
{
	OnSeq_INIT();
	OnSeq_Execute();
}

//==================================================================//
// ������ ���� ���� �ʱ�ȭ
//==================================================================//
void CSeq_BoxClamp::OnSeq_INIT(void) 
{
	if( m_bflag_INIT == true )
	{
		return;
	}
	m_bflag_INIT = true;

	m_nStep_Init = 0;
	m_nStep_Run = 0;
	m_nRcvyStep = 0;
}

//==================================================================//
// ������ ����
//==================================================================//
void CSeq_BoxClamp::OnSeq_Execute(void) 
{
	if( Func.OnIsInterruptCondition() == 1 )
	{
		return;
	}

	int nRet = 0;

	switch( st_work.nEqpStatus )
	{
	case dINIT:
		// �ʱ�ȭ ���� ��ȭ���ڿ��� [YES] �����ߴ��� ����
		if( st_handler.nInitTryREQ != YES )
		{
			break;
		}
		OnRun_Initial();
		break;

	case dRUN:
		// �ʱ�ȭ �۾��� �Ϸ���� ������ [AUTO] �۾� ���� �Ұ���
		// - st_handler.nIsAllSiteInitEnd : �ʱ�ȭ ȭ�鿡�� ����
		if( st_handler.nIsAllSiteInitEnd == CTL_YES )
		{
			OnRun_Move();
		}
		break;

	default:
		OnRun_ForceDischarge();
		break;
	}
}

void CSeq_BoxClamp::OnRun_Initial() 
{
	// �ʱ�ȭ �۾��� �Ϸ�� ��� ���� ������
	if (st_handler.mn_init_state[INIT_BOX_CLAMP] != CTL_NO)
	{
		return;
	}
	
	int nRet = VAR_INIT_;
	int nRetData[4] = {0,};

	switch(m_nStep_Init)
	{
	case 0:
		m_nStep_Init = 10;
		break;

	case 10://have to check...
		// XYZ Robot�� ���� ������ ������ Box Clamp�� �ʱ�ȭ
		// ������ B���̶� ���� ���� Ȯ�� �� ���� ���
		nRet = !g_ioMgr.get_in_bit(stIO.i_Chk_GripperSafetyPos);
		if (nRet == IO_OFF)
		{
			m_nStep_Init = 100;
		}
		break;

	case 100:
// 		OnSet_CylClamp(IO_OFF);			// Align
// 		OnSet_CylUDGripper(IO_OFF);		// �Ǽ��縮 �ڽ� ���κ��� ���� �ϴ� �Ǹ��� [ON:����, OFF:����]
//		m_nStep_Init = 110;

		//kwlee 2017.0712
		if( st_work.nLiftDownComplete == CTL_YES )
		{
			OnSet_CylClamp(IO_OFF);			// Align
			OnSet_CylUDGripper(IO_OFF);		// �Ǽ��縮 �ڽ� ���κ��� ���� �ϴ� �Ǹ��� [ON:����, OFF:����]
			m_nStep_Init = 110;
		}
		break;

	case 110:
		nRetData[0] = OnGet_CylClamp(IO_OFF);
		nRetData[1] = OnGet_CylUDGripper(IO_OFF);
		if(nRetData[0] == CTL_GOOD && nRetData[1] == CTL_GOOD)
		{
			stSync.nResp_Clamp2LoadingConv_Work = SYNC_RESP_RESET_;
			m_nStep_Init = 200;
		}
		else if (nRetData[0] == CTL_ERROR || nRetData[1] == CTL_ERROR)
		{
			// �˶� �ڵ�� OnGet_CylLRGripper() �Լ� �ȿ��� ������
			m_nStep_Init = 100;
			CTL_Lib.Alarm_Error_Occurrence( 1000, CTL_dWARNING, alarm.mstr_code );
		}
		break;

		
	case 200:
		OnSet_ConvBoxPusher(IO_OFF);			// pusher backward
		m_nStep_Init = 210;
		break;

	case 210:
		nRet = OnGet_ConvBoxPusher( IO_OFF );
		if( nRet == CTL_GOOD )
		{
			m_nStep_Init = 300;
		}
		else if( nRet == CTL_ERROR )
		{
			m_nStep_Init = 200;
			CTL_Lib.Alarm_Error_Occurrence( 1001, CTL_dWARNING, alarm.mstr_code );
		}
		break;

	case 300:
		OnSet_ConvBoxClamp( IO_OFF );
		m_nStep_Init = 310;
		break;

	case 310:
		nRet = OnGet_ConvBoxClamp( IO_OFF );
		if( nRet == CTL_GOOD )
		{
			m_nStep_Init = 400;
		}
		else if( nRet == CTL_ERROR )
		{
			m_nStep_Init = 300;
			CTL_Lib.Alarm_Error_Occurrence( 1002, CTL_dWARNING, alarm.mstr_code );
		}
		break;

	case 400:
		// [ BoxLift -> BoxClamp ] �۾� �Ϸ� ���
		if (stSync.nResp_Lifter2Clamp_Work == SYNC_RESP_WORK_COMPLETE_)
		{
			m_nStep_Init = 500;
		}
		break;

	case 500:
		OnSet_CylLifter( IO_OFF );
		m_nStep_Init = 510;
		break;

	case 510:
		nRet = OnGet_CylLifter( IO_OFF );
		if( nRet == CTL_GOOD )
		{
			m_nStep_Init = 520;
		}
		else if( nRet == CTL_ERROR )
		{
			m_nStep_Init = 500;
			CTL_Lib.Alarm_Error_Occurrence( 1003, CTL_dWARNING, alarm.mstr_code );
		}
		break;

	case 520:
		// Lift �Ǹ����� �������� Lifter���� SYNC_REQ_WORK_COMPLETE_ ��ȣ ����
		stSync.nReq_Clamp2Lifter_Work = SYNC_REQ_WORK_COMPLETE_;
		m_nStep_Init = 530;
		break;

	case 530:
		// Lifter�� ���� ��ȣ��  SYNC_RESP_WORK_COMPLETE_���� ����Ǹ� 
		// Lifter X���� ù��° ���� ��ġ�� �̵��ϸ鼭 Accy Box�� ���� �Ҷ� ���� ���
		if( stSync.nResp_Lifter2Clamp_Work == SYNC_RESP_WORK_COMPLETE_ )
		{
			stSync.nReq_Clamp2Lifter_Work = SYNC_REQ_RESET_;
			m_nStep_Init = 600;
		}
		break;

	case 600:
		OnSet_CylLifter( IO_ON );
		m_nStep_Init = 610;
		break;

	case 610:
		nRet = OnGet_CylLifter( IO_ON );
		if( nRet == CTL_GOOD )
		{
			m_nStep_Init = 1000;
		}
		else if( nRet == CTL_ERROR )
		{
			m_nStep_Init = 600;
			CTL_Lib.Alarm_Error_Occurrence( 1004, CTL_dWARNING, alarm.mstr_code );
		}
		break;

	case 1000:
		// �ʱ�ȭ ȭ�鿡�� [�ʱ�ȭ] �Ϸ� ǥ�� ����
		st_handler.mn_init_state[INIT_BOX_CLAMP] = CTL_YES;
		stSync.nReq_Clamp2Lifter_Work = SYNC_REQ_RESET_;
		m_nStep_Init = 0;
		break;

	}
}

void CSeq_BoxClamp::OnRun_Move() 
{
	// ��ü ����Ʈ ���� ���� �Ϸ� ���� Ȯ��
	// - ��� �κ��� ���� ������ �Ϸ�� �Ŀ��� ������ �����ϵ��� ��
	// : ���� ���� �߿� ������ �����ϸ� �����ϴ� ������ �浹�� �߻��� �� ����
	if( Func.OnIsAllRcvyComplete() != CTL_YES )
	{
		return;
	}
	Func.OnTrace_ThreadStep(2, m_nStep_Run);

	int nRet = VAR_INIT_;
	int nRetData[4] = {0,};
	int i = 0;

	switch( m_nStep_Run )
	{
	case 0:
		// Loading Cov�� LotEnd �����̸� Box Clamp�� LotEnd ó�� �̸� ���
		if( stSync.nLotEnd_LoadingConv == TRUE )
		{
			stSync.nLotEnd_BoxClamp = TRUE;
			break;
		}
		else
		{
			stSync.nLotEnd_BoxClamp = FALSE;
		}

		if( st_basic.n_mode_device == WITHOUT_DVC_ )
		{
			if( st_map.nLoadingAccyBoxExist[0] == 0 &&
				st_map.nLoadingAccyBoxExist[1] == 0 &&
				st_map.nLoadingAccyBoxExist[2] == 0 )
			{
				stSync.nResp_Clamp2LoadingConv_Work = SYNC_RESP_RESET_;
				break;
			}
		}

		stSync.nResp_Clamp2LoadingConv_Work = SYNC_RESP_RESET_;
		m_nStep_Run = 100;
		//kwlee 2017.0711
		//m_nStep_Run = 10;
		break;

	case 100:
		// Load Conv���� Accy Box ������ �Ϸ� �ɶ����� ���
		if( stSync.nReq_LoadingConv2Clamp_Work == SYNC_REQ_ACCY_BOX_LOADING_COMPLETE_)
		{
			stSync.nResp_Clamp2LoadingConv_Work = SYNC_RESP_WORK_;
			// �ڽ��� ���� ��� �˶� �Ҹ��� ��� �ڽ� ������ �ϱ� ���� �˶��� �︮���� �س���.
			Func.OnSet_BuzzerSound(IO_OFF, 0);

			m_nPusher_Retry = 0;
			m_nStep_Run = 900;
		}
		break;

	//////////////////////////////////////////////////////////////////////////
	// Accy Box Gripping�� Lift ���ֿ� Accy Box�� ���� �ϴ� �۾�
	//////////////////////////////////////////////////////////////////////////
	case 900:
		{
			int ReadIO[4] = {0,};
 			ReadIO[0] = g_ioMgr.get_in_bit( stIO.i_Chk_Load_BoxClampOnCheck, IO_OFF);
 			ReadIO[1] = g_ioMgr.get_in_bit( stIO.i_Chk_Load_BoxClampOffCheck, IO_ON);
 			ReadIO[2] = g_ioMgr.get_in_bit( stIO.i_Chk_Load_BoxPusherCylPusherCheck, IO_OFF);
 			ReadIO[3] = g_ioMgr.get_in_bit( stIO.i_Chk_Load_BoxPusherCylPullCheck, IO_ON);

			if( ReadIO[0] == IO_OFF && ReadIO[1] == IO_ON && ReadIO[2] == IO_OFF && ReadIO[3] == IO_ON )
			{
				m_nStep_Run = 2000;
			}
			else
			{
				m_nStep_Run = 1000;
			}
		}
		break;

	case 1000:
		OnSet_ConvBoxPusher(IO_OFF);			// Align
		m_nStep_Run = 1100;
		break;

	case 1100:
		nRet = OnGet_ConvBoxPusher( IO_OFF );
		if( nRet == CTL_GOOD )
		{
			m_nStep_Run = 1200;
		}
		else if( nRet == CTL_ERROR )
		{
			m_nStep_Run = 1000;
			CTL_Lib.Alarm_Error_Occurrence( 1005, CTL_dWARNING, alarm.mstr_code );
		}
		break;

	case  1200:
		OnSet_ConvBoxClamp( IO_OFF );
		m_nStep_Run = 1300;
		break;
		
	case 1300:
		nRet = OnGet_ConvBoxClamp( IO_OFF );
		if( nRet == CTL_GOOD )
		{
			m_nStep_Run = 1400;
		}
		else if( nRet == CTL_ERROR )
		{
			m_nStep_Run = 1200;
			CTL_Lib.Alarm_Error_Occurrence( 1006, CTL_dWARNING, alarm.mstr_code );
		}
		break;


		//Conv Box Pusher
	case  2000:
		OnSet_ConvBoxClamp( IO_ON );
		m_nStep_Run = 2100;
		break;
		
	case 2100:
		nRet = OnGet_ConvBoxClamp( IO_ON );
		if( nRet == CTL_GOOD )
		{
			m_nStep_Run = 2200;
		}
		else if( nRet == CTL_ERROR )
		{
			m_nStep_Run = 2000;
			CTL_Lib.Alarm_Error_Occurrence( 1007, CTL_dWARNING, alarm.mstr_code );
		}
		break;

	case 2200:
		OnSet_ConvBoxPusher(IO_ON);			// Align
		m_nStep_Run = 2300;
		break;
		
	case 2300:
		nRet = OnGet_ConvBoxPusher( IO_ON );
		if( nRet == CTL_GOOD )
		{
			m_lTime_GoesBy[0] = GetCurrentTime();
			m_nStep_Run = 2400;
		}
		else if( nRet == CTL_ERROR )
		{
			m_nStep_Run = 2200;
			CTL_Lib.Alarm_Error_Occurrence( 1008, CTL_dWARNING, alarm.mstr_code );
		}
		break;

	case 2400:
		nRet = g_ioMgr.get_in_bit( stIO.i_Chk_AccyBox_Arrive_End, IO_ON );
		if( nRet == IO_ON )
		{
			m_nStep_Run = 2500;
		}
		else
		{
			m_lTime_GoesBy[1] = GetCurrentTime();
			m_lTime_GoesBy[2] = m_lTime_GoesBy[1] - m_lTime_GoesBy[0];
			if( m_lTime_GoesBy[2] < 0) m_lTime_GoesBy[0] = GetCurrentTime();
			if( m_lTime_GoesBy[2] < 100 ) break;
			//132000 0 00 "PS0407_LOAD_BOX_ARRIVE_END_OFF_CHK_ERR."
			CTL_Lib.Alarm_Error_Occurrence( 1009, CTL_dWARNING, "132000" );
		}
		break;

	case 2500://alig
		OnSet_CylClamp(IO_ON);
		m_nStep_Run = 2510;
		break;

	case 2510:
		nRet = OnGet_CylClamp(IO_ON);
		if( nRet == CTL_GOOD )
		{
			m_nStep_Run = 2600;
		}
		else
		{
			CTL_Lib.Alarm_Error_Occurrence( 1010, CTL_dWARNING, alarm.mstr_code );
			m_nStep_Run = 2500;
		}
		break;

	case 2600://IO_OFF = DOWN
		OnSet_CylUDGripper(IO_OFF);
	//	OnSet_ConvBoxPusher(IO_OFF);
		m_nStep_Run = 2610;
		break;

	case 2610:
		nRetData[0] = OnGet_CylUDGripper( IO_OFF );
 	//	nRetData[1] = OnGet_ConvBoxPusher( IO_OFF ); 
 	//	if ( nRetData[0] == CTL_GOOD && nRetData[1] == CTL_GOOD )
		//kwlee 2017.0712 
		if ( nRetData[0] == CTL_GOOD)
		{
			m_nStep_Run = 2700;
			
		}
		else if ( nRetData[0] == CTL_ERROR || nRetData[1] == CTL_ERROR )
		{
			CTL_Lib.Alarm_Error_Occurrence( 1011, CTL_dWARNING, alarm.mstr_code );
			m_nStep_Run = 2600;
		}
		break;

		//Clamp & Pusher ���� OFF
	case 2700:
		OnSet_ConvBoxClamp( IO_OFF );
		OnSet_ConvBoxPusher(IO_OFF); //kwlee 2017.0712
		m_nStep_Run = 2710;
		break;
		
	case 2710:
		//nRet = OnGet_ConvBoxClamp( IO_OFF );
		//if( nRet == CTL_GOOD )
		//kwlee 2017.0712
		nRetData[0] = OnGet_ConvBoxClamp( IO_OFF );
		nRetData[1] = OnGet_ConvBoxPusher( IO_OFF ); 

		if ( nRetData[0] == CTL_GOOD && nRetData[1] == CTL_GOOD )
		{
			// [WithOut] ������� Ȯ��
			if (st_basic.n_mode_device == WITHOUT_DVC_)
			{
				st_map.nLoadingAccyBoxExist[0] = 0;
				
				if (stWithoutData.nWorkTestBox != 0)
				{
					stWithoutData.nWorkTestBox--;
				}
			}
			// BoxClamp�� �۾� ������ ���� �ߴٰ� Loading Conv�� �˸�
			stSync.nResp_Clamp2LoadingConv_Work = SYNC_RESP_RESET_;
			
			m_nStep_Run = 2800;
		}
		else if( nRet == CTL_ERROR )
		{
			m_nStep_Run = 2700;
			CTL_Lib.Alarm_Error_Occurrence( 1012, CTL_dWARNING, alarm.mstr_code );
		}
		break;

	case 2800:
		st_map.nBoxChange = TRUE;
		st_map.nResp_AccyReadyCNT = 240;
		// Lifter���� Accy Box Clamping �Ϸ� �˸�(��������?? ����Ʈ����??)(�׷� ������ ����?)
		stSync.nReq_Clamp2Lifter_Work = SYNC_REQ_ACCY_BOX_CLAMPING_COMPLETE_;
		
		st_map.nClampAccyBoxAccy = 1;
		
		if (st_handler.cwnd_main != NULL)
		{
			// Box Clamp ���� ���� ���� ǥ�� ��û
			st_handler.cwnd_main->PostMessage(WM_UPDATE_MAIN, PLACE_BOX_CLAMP_);
		}
		m_nStep_Run = 3000;
		break;


		//////////////////////////////////////////////////////////////////////////
		// Accy Box ���� �۾�
		//////////////////////////////////////////////////////////////////////////
	case 3000:
		// [BoxLifter -> BoxClamp] �۾� �Ϸ� ���
		if( stSync.nResp_Lifter2Clamp_Work == SYNC_RESP_WORK_COMPLETE_ )
		{		   
			//stSync.nReq_Clamp2Lifter_Work = SYNC_REQ_WORK_; //kwlee 2017.0711
			m_nStep_Run = 3100;
		}
		break;

	case 3100:
		OnSet_CylClamp(IO_OFF);	            // align
		//OnSet_CylUDGripper(IO_OFF);			// �Ǽ��縮 �ڽ� ���κ��� ���� �ϴ� �Ǹ��� [ON:����, OFF:����]
		//kwlee 2017.0712
		OnSet_CylUDGripper(IO_ON);			// �Ǽ��縮 �ڽ� ���κ��� ���� �ϴ� �Ǹ��� [ON:����, OFF:����]
		m_nStep_Run = 3110;
		break;

	case 3110:
		nRetData[0] = OnGet_CylClamp( IO_OFF );
		//nRetData[1] = OnGet_CylUDGripper( IO_OFF );
		//kwlee 2017.0712 IO_ON = UP
		nRetData[1] = OnGet_CylUDGripper( IO_ON );
		if( nRetData[0] == CTL_GOOD && nRetData[1] == CTL_GOOD )
		{
			m_lTime_GoesBy[01] = GetCurrentTime();
			m_nStep_Run = 4000;
			//stSync.nReq_Clamp2Lifter_Work = SYNC_REQ_RESET_;
			//
		}
		else if( nRetData[0] == CTL_ERROR || nRetData[1] == CTL_ERROR )
		{
			CTL_Lib.Alarm_Error_Occurrence( 1013, CTL_dWARNING, alarm.mstr_code );
			m_nStep_Run = 3100;
		}
		break;

	case 4000:
		//kwlee 2017.0707
 		nRetData[0] = g_ioMgr.get_in_bit( stIO.i_Chk_Load_BoxPusherCylPusherCheck, IO_OFF);
 		nRetData[1] = g_ioMgr.get_in_bit( stIO.i_Chk_Load_BoxPusherCylPullCheck, IO_ON);
 		if( nRetData[0] == IO_OFF && nRetData[1] == IO_ON )
 		{
 			m_nStep_Run = 4100;
 		}
 		else
 		{
 			m_lTime_GoesBy[1] = GetCurrentTime();
 			m_lTime_GoesBy[2] = m_lTime_GoesBy[1] - m_lTime_GoesBy[0];
 			if( m_lTime_GoesBy[2] < 0) m_lTime_GoesBy[0] = GetCurrentTime();
 			if( m_lTime_GoesBy[2] < 100 ) break;
 			//131001 0 00 "PS0702_PS0703_LOAD_BOX_PUSHER_FORWARD_CHK_ERR."
 			CTL_Lib.Alarm_Error_Occurrence( 1014, CTL_dWARNING, "131001" );
 		}
		break;

	case 4100:
		OnSet_CylLifter(IO_OFF);		// �Ǽ��縮 �ڽ� �غκ��� �����ִ� �Ǹ��� [ON:Up, OFF:Down]
		m_nStep_Run = 4110;
		break;


	case 4110:
		nRet = OnGet_CylLifter( IO_OFF );
		if( nRet == CTL_GOOD )
		{
			//stSync.nReq_Clamp2Lifter_Work = SYNC_REQ_RESET_; 
			//kwlee 2017.0711
			stSync.nReq_Clamp2Lifter_Work = SYNC_REQ_WORK_;
			m_nStep_Run = 4200;
		}
		else if( nRet == CTL_ERROR )
		{
			CTL_Lib.Alarm_Error_Occurrence( 1015, CTL_dWARNING, alarm.mstr_code );
			m_nStep_Run = 4100;
		}
		break;

	case 4200:
		// Box Remove �Ϸ� ��� 
		
		if( stSync.nResp_Lifter2Clamp_Work == SYNC_RESP_BOX_REMOVE_COMPLETE )
		{
			//stSync.nReq_Clamp2Lifter_Work = SYNC_REQ_WORK_COMPLETE_; //kwlee 2017.0711
			m_nStep_Run = 4300;
		}
		else if( stSync.nReq_LoadingConv2Clamp_Work == SYNC_REQ_ACCY_BOX_LOADING_COMPLETE_ )
		{
			stSync.nResp_Clamp2LoadingConv_Work = SYNC_RESP_WORK_;
			m_nStep_Run = 4300;
		}
		break;

	case 4300:
		// Box Remove �Ϸ� ��� 
		if(stSync.nResp_Lifter2Clamp_Work == SYNC_RESP_BOX_REMOVE_COMPLETE)
		{
			//stSync.nReq_Clamp2Lifter_Work = SYNC_REQ_WORK_COMPLETE_; //kwlee 2017.0712
			m_nStep_Run = 4500;
		}
		break;

	case 4500:
		// ���� Accy Box�� ���� �ޱ� ���� Lift�� �ø�
		OnSet_CylLifter(IO_ON);		// �Ǽ��縮 �ڽ� �غκ��� �����ִ� �Ǹ��� [ON:Up, OFF:Down]
		m_nStep_Run = 4510;
		break;

	case 4510:
		nRet = OnGet_CylLifter(IO_ON);
		if (nRet == CTL_GOOD)
		{
			//stSync.nReq_Clamp2Lifter_Work = SYNC_REQ_WORK_COMPLETE_; //kwlee 2017.0711 �÷ȴٰ� �˷���.
			m_nStep_Run = 5000;
		}
		else if (nRet == CTL_ERROR)
		{
			CTL_Lib.Alarm_Error_Occurrence( 1016, CTL_dWARNING, alarm.mstr_code );
			m_nStep_Run = 4500;
		}
		break;

	case 5000:
		stSync.nReq_Clamp2Lifter_Work = SYNC_REQ_RESET_;
		m_nStep_Run = 0;
		break;

	}

}

//==================================================================//
// ���� �۾� ���� Box�� ���� �ϴ� �Լ�
//==================================================================//
int CSeq_BoxClamp::OnRun_ForceDischarge() 
{
	int nFuncRet = CTL_PROCEED;
//	int nRet;
	int nRetData[4] = {0,};
	
	Func.OnTrace_ThreadStep(4, m_nStep_ForceDischarge);	// ������ ���� ���� TRACE
	
	// Box ���� ���
	if(st_work.nForceDischarge != FORCEDISCHARGE_ON)
	{
		return true;
	}
	
// 	switch(m_nStep_ForceDischarge)
// 	{
// 	case 0:
// 		if(stSync.nForceDischargeComplete[SITE_BOX_CLAMP_] != CTL_YES)
// 		{
// 			stSync.nReq_Clamp2Lifter_Work = SYNC_REQ_WORK_;
// 			m_nStep_ForceDischarge = 10;
// 		}
// 		break;
// 
// 	case 10:
// 		if(stSync.nResp_Lifter2Clamp_Work == SYNC_RESP_BOX_REMOVE_READY)
// 		{
// 			m_nStep_ForceDischarge = 100;
// 		}
// 		break;
// 
// 	case 100:
// // 		OnSet_CylPusher(IO_OFF);		// �Ǽ��縮 �ڽ� �޺κ��� ���� �ϴ� Ǫ�� [ON:����, OFF:����]
// 		OnSet_CylClamp(IO_OFF);		// �Ǽ��縮 �ڽ� ���κ��� ���� �ϴ� �Ǹ��� [ON:����, OFF:����]
// // 		OnSet_CylLRGripper(IO_OFF);		// Box LR Gripper [ON:Grip, OFF:UnGrip]
// 		m_nStep_ForceDischarge = 110;
// 		break;
// 
// 	case 110:
// // 		nRetData[0] = OnGet_CylPusher(IO_OFF);
// 		nRetData[1] = OnGet_CylClamp(IO_OFF);
// // 		nRetData[2] = OnGet_CylLRGripper(IO_OFF);
// 		if (nRetData[0] == CTL_GOOD && nRetData[1] == CTL_GOOD && nRetData[2] == CTL_GOOD)
// 		{
// 			m_nStep_ForceDischarge = 200;
// 		}
// 		else if (nRetData[0] == CTL_ERROR || nRetData[1] == CTL_ERROR || nRetData[2] == CTL_ERROR)
// 		{
// 			// �˶� �ڵ�� OnGet_CylPusher() �Լ� �ȿ��� ������
// 			// �˶� �ڵ�� OnGet_CylClamp() �Լ� �ȿ��� ������
// 			// �˶� �ڵ�� OnGet_CylLRGripper() �Լ� �ȿ��� ������
// 			m_nStep_ForceDischarge = 100;
// 		}
// 		break;
// 
// 	case 200:
// 		OnSet_CylSupply(IO_OFF);		// �Ǽ��縮 �ڽ� ���� �Ǹ���[ON:�۾� ����, OFF:¤�� ����]
// 		m_nStep_ForceDischarge = 210;
// 		break;
// 
// 	case 210:
// 		nRet = g_ioMgr.get_in_bit(stIO.i_Chk_AccyBoxSupplyFw);
// 		if (nRet == IO_OFF)
// 		{
// 			m_nStep_ForceDischarge = 300;
// 		}
// 		break;
// 
// 	case 300:
// 		OnSet_CylLifter(IO_OFF);		// �Ǽ��縮 �ڽ� �غκ��� �����ִ� �Ǹ��� [ON:Up, OFF:Down]
// 		m_nStep_ForceDischarge = 310;
// 		break;
// 
// 	case 310:
// 		nRetData[0] = OnGet_CylLifter(IO_OFF);
// 		nRetData[1] = OnGet_CylSupply(IO_OFF);
// 		if (nRetData[0] == CTL_GOOD && nRetData[1] == CTL_GOOD)
// 		{
// 			stSync.nReq_Clamp2Lifter_Work = SYNC_REQ_RESET_;
// 
// 			st_map.nClampAccyBoxAccy = 0;
// 			
// 			if (st_handler.cwnd_main != NULL)
// 			{
// 				// Box Clamp ���� ���� ���� ǥ�� ��û
// 				st_handler.cwnd_main->PostMessage(WM_UPDATE_MAIN, PLACE_BOX_CLAMP_);
// 			}
// 			m_nStep_ForceDischarge = 400;
// 		}
// 		else if (nRetData[0] == CTL_ERROR)
// 		{
// 			// �˶� �ڵ�� OnGet_CylLifter() �Լ� �ȿ��� ������
// 			m_nStep_ForceDischarge = 300;
// 		}
// 		else if (nRetData[1] == CTL_ERROR)
// 		{
// 			// �˶� �ڵ�� OnGet_CylSupply() �Լ� �ȿ��� ������
// 			m_nStep_ForceDischarge = 200;
// 		}
// 		break;
// 
// 	case 400:
// 		// ���� �߰� [2015.06.24]
// 		// Lift �ʿ��� ��ũ ���� Ÿ���� 2���� �ְ� �־� �߰� ��.
// 		if(stSync.nResp_Lifter2Clamp_Work == SYNC_RESP_RESET_ || stSync.nResp_Lifter2Clamp_Work == SYNC_RESP_BOX_REMOVE_COMPLETE)
// 		{
// 			m_nStep_ForceDischarge = 500;
// 		}
// 		break;
// 
// 	case 500:
// 		OnSet_CylLifter(IO_ON);		// �Ǽ��縮 �ڽ� �غκ��� �����ִ� �Ǹ��� [ON:Up, OFF:Down]
// 		m_nStep_ForceDischarge = 510;
// 		break;
// 
// 	case 510:
// 		nRet = OnGet_CylLifter(IO_ON);
// 		if (nRet == CTL_GOOD)
// 		{
// 			m_nStep_ForceDischarge = 600;
// 		}
// 		else if (nRet == CTL_ERROR)
// 		{
// 			// �˶� �ڵ�� OnGet_CylLifter() �Լ� �ȿ��� ������
// 			m_nStep_ForceDischarge = 500;
// 		}
// 		break;
// 
// 	case 600:
// 		m_nStep_Run = 0;
// 		m_nStep_ForceDischarge = 0;
// 		stSync.nForceDischargeComplete[SITE_BOX_CLAMP_] = CTL_YES;
// 		stSync.nResp_Clamp2LoadingConv_Work = SYNC_RESP_RESET_;
// 		stSync.nReq_Clamp2Lifter_Work = SYNC_REQ_RESET_;
// 		break;
// 	}

	return nFuncRet;
}

//==================================================================//
// Load Conv Box Clamp
//==================================================================//
void CSeq_BoxClamp::OnSet_ConvBoxClamp(int nzOnOff) 
{
	g_ioMgr.set_out_bit(stIO.o_Load_BoxClampOnOff, nzOnOff);

	m_bflag_ConvClamp = true;
	m_lWait_ConvClamp[0] = GetCurrentTime();
}

int CSeq_BoxClamp::OnGet_ConvBoxClamp(int nzOnOff) 
{
	int nFunRet = CTL_PROCEED;

	int nReadIO[2] = {0,};
	nReadIO[0] = g_ioMgr.get_in_bit(stIO.i_Chk_Load_BoxClampOnCheck);
	nReadIO[1] = g_ioMgr.get_in_bit(stIO.i_Chk_Load_BoxClampOffCheck);

	m_lWait_ConvClamp[1] = GetCurrentTime();
	m_lWait_ConvClamp[2] = m_lWait_ConvClamp[1] - m_lWait_ConvClamp[0];

	if (m_lWait_ConvClamp[2] < 0)
	{
		m_lWait_ConvClamp[0] = GetCurrentTime();
	}

	if (nzOnOff == IO_OFF)		// �ڽ��� ���� ����
	{
		if (m_bflag_ConvClamp == true &&
			nReadIO[0] == IO_OFF && nReadIO[1] == IO_ON)
		{
			m_lWait_ConvClamp[0] = GetCurrentTime();
			m_bflag_ConvClamp = false;
		}
		else if (m_bflag_ConvClamp == false &&
			nReadIO[0] == IO_OFF && nReadIO[1] == IO_ON )
		{
			if (m_lWait_ConvClamp[2] > st_time.nWait_Off[CYL_ACCY_CONV_BOX_CLAMP])
			{
				nFunRet = CTL_GOOD;
			}
		}
		else
		{
			if (m_lWait_ConvClamp[2] > st_time.nWait_Limit[CYL_ACCY_CONV_BOX_CLAMP])
			{
				//  130000 0 00 "PS0701_PS0703_LOAD_BOX_CLAMP_OFF_CHK_ERR."
				alarm.mstr_code		= "130000";

				alarm.mn_count_mode	= 0;
				alarm.mn_type_mode	= eWARNING;
				st_work.nEqpStatus	= dWARNING;

				nFunRet = CTL_ERROR;
			}
		}
	}
	else		// �ڽ��� ���� ����
	{
		if (m_bflag_ConvClamp == true &&
			nReadIO[0] == IO_ON && nReadIO[1] == IO_OFF)
		{
			m_lWait_ConvClamp[0] = GetCurrentTime();
			m_bflag_ConvClamp = false;
		}
		else if (m_bflag_ConvClamp == false &&
			nReadIO[0] == IO_ON && nReadIO[1] == IO_OFF)
		{
			if (m_lWait_ConvClamp[2] > st_time.nWait_On[CYL_ACCY_CONV_BOX_CLAMP])
			{
				nFunRet = CTL_GOOD;
			}
		}
		else
		{
			if (m_lWait_ConvClamp[2] > st_time.nWait_Limit[CYL_ACCY_CONV_BOX_CLAMP])
			{
				// 130001 0 00 "PS0701_PS0703_LOAD_BOX_CLAMP_ON_CHK_ERR."
				alarm.mstr_code		= "130001";

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
// Load Conv Box Pusher
//==================================================================//
void CSeq_BoxClamp::OnSet_ConvBoxPusher(int nzOnOff) 
{
	g_ioMgr.set_out_bit(stIO.o_Load_BoxPusherCylPusher, nzOnOff);
	g_ioMgr.set_out_bit(stIO.o_Load_BoxPusherCylPull, !nzOnOff);
	
	m_bflag_ConvClamp = true;
	m_lWait_ConvClamp[0] = GetCurrentTime();
}

int CSeq_BoxClamp::OnGet_ConvBoxPusher(int nzOnOff) 
{
	int nFunRet = CTL_PROCEED;
	
	int nReadIO[2] = {0,};
	//kwlee 2017.0707
// 	nReadIO[0] = g_ioMgr.get_in_bit(stIO.i_Chk_Load_BoxPusherFwCheck);
// 	nReadIO[1] = g_ioMgr.get_in_bit(stIO.i_Chk_Load_BoxPusherBwCheck);
	
	m_lWait_ConvClamp[1] = GetCurrentTime();
	m_lWait_ConvClamp[2] = m_lWait_ConvClamp[1] - m_lWait_ConvClamp[0];
	
	if (m_lWait_ConvClamp[2] < 0)
	{
		m_lWait_ConvClamp[0] = GetCurrentTime();
	}
	
	if (nzOnOff == IO_OFF)		// �ڽ��� ���� ����
	{
		if (m_bflag_ConvClamp == true &&
			nReadIO[0] == IO_OFF && nReadIO[1] == IO_ON)
		{
			m_lWait_ConvClamp[0] = GetCurrentTime();
			m_bflag_ConvClamp = false;
		}
		else if (m_bflag_ConvClamp == false &&
			nReadIO[0] == IO_OFF && nReadIO[1] == IO_ON )
		{
			if (m_lWait_ConvClamp[2] > st_time.nWait_Off[CYL_ACCY_CONV_BOX_CLAMP])
			{
				nFunRet = CTL_GOOD;
			}
		}
		else
		{
			if (m_lWait_ConvClamp[2] > st_time.nWait_Limit[CYL_ACCY_CONV_BOX_CLAMP])
			{
				//  131000 0 00 "PS0702_PS0703_LOAD_BOX_PUSHER_BACKWARD_CHK_ERR."
				alarm.mstr_code		= "131000";
				
				alarm.mn_count_mode	= 0;
				alarm.mn_type_mode	= eWARNING;
				st_work.nEqpStatus	= dWARNING;
				
				nFunRet = CTL_ERROR;
			}
		}
	}
	else		// �ڽ��� ���� ����
	{
		if (m_bflag_ConvClamp == true &&
			nReadIO[0] == IO_ON && nReadIO[1] == IO_OFF)
		{
			m_lWait_ConvClamp[0] = GetCurrentTime();
			m_bflag_ConvClamp = false;
		}
		else if (m_bflag_ConvClamp == false &&
			nReadIO[0] == IO_ON && nReadIO[1] == IO_OFF)
		{
			if (m_lWait_ConvClamp[2] > st_time.nWait_On[CYL_ACCY_CONV_BOX_CLAMP])
			{
				nFunRet = CTL_GOOD;
			}
		}
		else
		{
			if (m_lWait_ConvClamp[2] > st_time.nWait_Limit[CYL_ACCY_CONV_BOX_CLAMP])
			{
				// 130001 0 00 "PS0701_PS0703_LOAD_BOX_CLAMP_ON_CHK_ERR."
				alarm.mstr_code		= "130001";
				
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
// �Ǽ��縮 �ڽ��� ����� ���� ���� �� ��� �Ǹ���
// - IO_ON = Clamp �����̸�, ��� �ڽ��� ����
// - IO_OFF = Unclamp �����̸�, ��� �ڽ��� ����
//==================================================================//
void CSeq_BoxClamp::OnSet_CylClamp(int nzOnOff) 
{
	g_ioMgr.set_out_bit(stIO.o_Cyl_AccyBoxClamp, nzOnOff);
	g_ioMgr.set_out_bit(stIO.o_Cyl_AccyBoxUnclamp, !nzOnOff);

	m_bflag_Clamp = true;
	m_lWait_Clamp[0] = GetCurrentTime();
}

int CSeq_BoxClamp::OnGet_CylClamp(int nzOnOff, int nzSite) 
{
	int nFunRet = CTL_PROCEED;

	int nReadIO[4] = {0,};
	nReadIO[0] = g_ioMgr.get_in_bit(stIO.i_Chk_AccyBoxClamp[SIDE_LEFT_]);
	nReadIO[1] = g_ioMgr.get_in_bit(stIO.i_Chk_AccyBoxUnclamp[SIDE_LEFT_]);
	nReadIO[2] = g_ioMgr.get_in_bit(stIO.i_Chk_AccyBoxClamp[SIDE_RIGHT_]);
	nReadIO[3] = g_ioMgr.get_in_bit(stIO.i_Chk_AccyBoxUnclamp[SIDE_RIGHT_]);

	if (nzSite == SIDE_LEFT_)
	{
		// [Right] ���� �Է��� ������ ����
		nReadIO[2] =  nzOnOff;
		nReadIO[3] = !nzOnOff;
	}
	else if (nzSite == SIDE_RIGHT_)
	{
		// [Left] ���� �Է��� ������ ����
		nReadIO[0] =  nzOnOff;
		nReadIO[1] = !nzOnOff;
	}

	m_lWait_Clamp[1] = GetCurrentTime();
	m_lWait_Clamp[2] = m_lWait_Clamp[1] - m_lWait_Clamp[0];

	if (m_lWait_Clamp[2] < 0)
	{
		m_lWait_Clamp[0] = GetCurrentTime();
	}

	if (nzOnOff == IO_OFF)		// �ڽ� ����� ���� ����
	{
		if (m_bflag_Clamp == true &&
			nReadIO[0] == IO_OFF && nReadIO[1] == IO_ON &&
			nReadIO[2] == IO_OFF && nReadIO[3] == IO_ON)
		{
			m_lWait_Clamp[0] = GetCurrentTime();
			m_bflag_Clamp = false;
		}
		else if (m_bflag_Clamp == false &&
			nReadIO[0] == IO_OFF && nReadIO[1] == IO_ON &&
			nReadIO[2] == IO_OFF && nReadIO[3] == IO_ON)
		{
			if (m_lWait_Clamp[2] > st_time.nWait_Off[CYL_ACCY_BOX_CLAMP])
			{
				nFunRet = CTL_GOOD;
			}
		}
		else
		{
			if (m_lWait_Clamp[2] > st_time.nWait_Limit[CYL_ACCY_BOX_CLAMP])
			{
				// 133000 0 00 "PS0505_PS0507_BOX_CLAMP_UNCLAMP_CHK_ERR."
				alarm.mstr_code		= "133000";

				alarm.mn_count_mode	= 0;
				alarm.mn_type_mode	= eWARNING;
				st_work.nEqpStatus	= dWARNING;

				nFunRet = CTL_ERROR;
			}
		}
	}
	else		// �ڽ� ����� ���� ����
	{
		if (m_bflag_Clamp == true &&
			nReadIO[0] == IO_ON && nReadIO[1] == IO_OFF &&
			nReadIO[2] == IO_ON && nReadIO[3] == IO_OFF)
		{
			m_lWait_Clamp[0] = GetCurrentTime();
			m_bflag_Clamp = false;
		}
		else if (m_bflag_Clamp == false &&
			nReadIO[0] == IO_ON && nReadIO[1] == IO_OFF &&
			nReadIO[2] == IO_ON && nReadIO[3] == IO_OFF)
		{
			if (m_lWait_Clamp[2] > st_time.nWait_On[CYL_ACCY_BOX_CLAMP])
			{
				nFunRet = CTL_GOOD;	// [OFF:DOWN]
			}
		}
		else
		{
			if (m_lWait_Clamp[2] > st_time.nWait_Limit[CYL_ACCY_BOX_CLAMP])
			{
				//  133001 0 00 "PS0504_PS0506_BOX_CLAMP_CLAMP_CHK_ERR."
				alarm.mstr_code		= "133001";

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
// �Ǽ��縮 �ڽ��� �۾� ������ �������� �� ������
// - IO_ON = UP ���¿��� �۾� ������ �Ǽ��縮 �ڽ��� ������ ���� ����
// - IO_OFF = Down ���¿��� �۾� �������� �Ǽ��縮 �ڽ��� �������� ���� ����
//==================================================================//
void CSeq_BoxClamp::OnSet_CylStopper(int nzOnOff) 
{
	g_ioMgr.set_out_bit(stIO.o_Cyl_AccyBoxConvStopperUp, nzOnOff);
	g_ioMgr.set_out_bit(stIO.o_Cyl_AccyBoxConvStopperDn, !nzOnOff);

	m_bflag_Stopper = true;
	m_lWait_Stopper[0] = GetCurrentTime();
}

int CSeq_BoxClamp::OnGet_CylStopper(int nzOnOff, int nzSite) 
{
	int nFunRet = CTL_PROCEED;

	int nReadIO[2] = {0,};
	nReadIO[0] = g_ioMgr.get_in_bit(stIO.i_Chk_AccyBoxConvStopperUp);
	nReadIO[1] = g_ioMgr.get_in_bit(stIO.i_Chk_AccyBoxConvStopperDn);

	if (nzSite == SIDE_UP_)
	{
		// [Down] ���� �Է��� ������ ����
		nReadIO[1] = !nzOnOff;
	}
	else if (nzSite == SIDE_DOWN_)
	{
		// [UP] ���� �Է��� ������ ����
		nReadIO[0] =  !nzOnOff;
	}

	m_lWait_Stopper[1] = GetCurrentTime();
	m_lWait_Stopper[2] = m_lWait_Stopper[1] - m_lWait_Stopper[0];

	if (m_lWait_Stopper[2] < 0)
	{
		m_lWait_Stopper[0] = GetCurrentTime();
	}

	if (nzOnOff == IO_OFF)		// ������ ������ ����
	{
		if (m_bflag_Stopper == true &&
			nReadIO[0] == IO_OFF && nReadIO[1] == IO_ON)
		{
			m_lWait_Stopper[0] = GetCurrentTime();
			m_bflag_Stopper = false;
		}
		else if (m_bflag_Stopper == false &&
			nReadIO[0] == IO_OFF && nReadIO[1] == IO_ON)
		{
			if (m_lWait_Stopper[2] > st_time.nWait_Off[CYL_ACCY_BOX_STOPPER])
			{
				nFunRet = CTL_GOOD;	
			}
		}
		else
		{
			if (m_lWait_Stopper[2] > st_time.nWait_Limit[CYL_ACCY_BOX_STOPPER])
			{
				// 120100 0 00 "PS0303_LOADING_CONV_STOPPER_DOWN_CHK_ERR."
				alarm.mstr_code		= "120100";

				alarm.mn_count_mode	= 0;
				alarm.mn_type_mode	= eWARNING;
				st_work.nEqpStatus	= dWARNING;

				nFunRet = CTL_ERROR;
			}
		}
	}
	else		// ������ �ö�� ����
	{
		if (m_bflag_Stopper == true &&
			nReadIO[0] == IO_ON && nReadIO[1] == IO_OFF)
		{
			m_lWait_Stopper[0] = GetCurrentTime();
			m_bflag_Stopper = false;
		}
		else if (m_bflag_Stopper == false &&
			nReadIO[0] == IO_ON && nReadIO[1] == IO_OFF)
		{
			if (m_lWait_Stopper[2] > st_time.nWait_On[CYL_ACCY_BOX_STOPPER])
			{
				nFunRet = CTL_GOOD;	
			}
		}
		else
		{
			if (m_lWait_Stopper[2] > st_time.nWait_Limit[CYL_ACCY_BOX_STOPPER])
			{
				// 120101 0 00 "PS0302_LOADING_CONV_STOPPER_UP_CHK_ERR."
				alarm.mstr_code		= "120101";
				nFunRet = CTL_ERROR;
			}
		}
	}
	return nFunRet;
}

//==================================================================//
// �Ǽ��縮 �ڽ��� �۾� �������� �ȱ�� �Ǹ���
// - IO_ON = Fw �����̸�, �ڽ��� ��ġ�� �۾� ����
// - IO_OFF = Bw �����̸�, ¤�� ����
//==================================================================//
void CSeq_BoxClamp::OnSet_CylSupply(int nzOnOff) 
{
// 	g_ioMgr.set_out_bit(stIO.o_Cyl_AccyBoxSupplyFw, nzOnOff);
// 	g_ioMgr.set_out_bit(stIO.o_Cyl_AccyBoxSupplyBw, !nzOnOff);

	m_bflag_Supply = true;
	m_lWait_Supply[0] = GetCurrentTime();
}

int CSeq_BoxClamp::OnGet_CylSupply(int nzOnOff, int nzSite)  
{
	int nFunRet = CTL_PROCEED;

	int nReadIO[2] = {0,};
// 	nReadIO[0] = g_ioMgr.get_in_bit(stIO.i_Chk_AccyBoxSupplyFw);
// 	nReadIO[1] = g_ioMgr.get_in_bit(stIO.i_Chk_AccyBoxSupplyBw);

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

	m_lWait_Supply[1] = GetCurrentTime();
	m_lWait_Supply[2] = m_lWait_Supply[1] - m_lWait_Supply[0];

	if (m_lWait_Supply[2] < 0)
	{
		m_lWait_Supply[0] = GetCurrentTime();
	}

	if (nzOnOff == IO_OFF)		// �ڽ��� ¤�� ���� ����
	{
		if (m_bflag_Supply == true &&
			nReadIO[0] == IO_OFF && nReadIO[1] == IO_ON )
		{
			m_lWait_Supply[0] = GetCurrentTime();
			m_bflag_Supply = false;
		}
		else if (m_bflag_Supply == false &&
			nReadIO[0] == IO_OFF && nReadIO[1] == IO_ON)
		{
			if (m_lWait_Supply[2] > st_time.nWait_Off[CYL_ACCY_BOX_SUPPLY])
			{
				nFunRet = CTL_GOOD;
			}
		}
		else
		{
			if (m_lWait_Supply[2] > st_time.nWait_Limit[CYL_ACCY_BOX_SUPPLY])
			{
				
				// 133001 0 00 "PS0707_BOX_CLAMP_BOX_SUPPLY_BW_CHK_ERR."
				alarm.mstr_code		= "133001";
				
				alarm.mn_count_mode	= 0;
				alarm.mn_type_mode	= eWARNING;
				st_work.nEqpStatus	= dWARNING;
				
				nFunRet = CTL_ERROR;
			}
		}
	}
	else		// �ڽ��� �۾� �ϴ� ����
	{
		if (m_bflag_Supply == true &&
			nReadIO[0] == IO_ON && nReadIO[1] == IO_OFF)
		{
			m_lWait_Supply[0] = GetCurrentTime();
			m_bflag_Supply = false;
		}
		else if (m_bflag_Supply == false &&
			nReadIO[0] == IO_ON && nReadIO[1] == IO_OFF)
		{
			if (m_lWait_Supply[2] > st_time.nWait_On[CYL_ACCY_BOX_SUPPLY])
			{
				nFunRet = CTL_GOOD;	// [OFF:DOWN]
			}
		}
		else
		{
			if (m_lWait_Supply[2] > st_time.nWait_Limit[CYL_ACCY_BOX_SUPPLY])
			{
				// 133000 0 00 "PS0706_BOX_CLAMP_BOX_SUPPLY_FW_CHK_ERR."
				alarm.mstr_code		= "133000";
				
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
// �Ǽ��縮 �ڽ��� ��, �Ʒ��� �����̴� �Ǹ���
// - IO_ON = UP �����̴�
// - IO_OFF = DOWN �����̴�
//==================================================================//
void CSeq_BoxClamp::OnSet_CylUDGripper(int nzOnOff) 
{
	g_ioMgr.set_out_bit(stIO.o_Cyl_AccyBoxGripperUp, nzOnOff);
	g_ioMgr.set_out_bit(stIO.o_Cyl_AccyBoxGripperDn, !nzOnOff);

	m_bflag_UDGripper = true;
	m_lWait_UDGripper[0] = GetCurrentTime();
}

int CSeq_BoxClamp::OnGet_CylUDGripper(int nzOnOff, int nzSite) 
{
	int nFunRet = CTL_PROCEED;

	int nReadIO[4] = {0,};
	nReadIO[0] = g_ioMgr.get_in_bit(stIO.i_Chk_AccyBoxGripperUp[SIDE_LEFT_]);
	nReadIO[1] = g_ioMgr.get_in_bit(stIO.i_Chk_AccyBoxGripperDn[SIDE_LEFT_]);
	nReadIO[2] = g_ioMgr.get_in_bit(stIO.i_Chk_AccyBoxGripperUp[SIDE_RIGHT_]);
	nReadIO[3] = g_ioMgr.get_in_bit(stIO.i_Chk_AccyBoxGripperDn[SIDE_RIGHT_]);

	if (nzSite == SIDE_LEFT_)
	{
		// [Right] ���� �Է��� ������ ����
		nReadIO[2] =  nzOnOff;
		nReadIO[3] = !nzOnOff;
	}
	else if (nzSite == SIDE_RIGHT_)
	{
		// [Left] ���� �Է��� ������ ����
		nReadIO[0] =  nzOnOff;
		nReadIO[1] = !nzOnOff;
	}

	m_lWait_UDGripper[1] = GetCurrentTime();
	m_lWait_UDGripper[2] = m_lWait_UDGripper[1] - m_lWait_UDGripper[0];

	if (m_lWait_UDGripper[2] < 0)
	{
		m_lWait_UDGripper[0] = GetCurrentTime();
	}

	if (nzOnOff == IO_OFF)		// �ڽ��� Down �� ����
	{
		if (m_bflag_UDGripper == true &&
			nReadIO[0] == IO_OFF && nReadIO[1] == IO_ON &&
			nReadIO[2] == IO_OFF && nReadIO[3] == IO_ON)
		{
			m_lWait_UDGripper[0] = GetCurrentTime();
			m_bflag_UDGripper = false;
		}
		else if (m_bflag_UDGripper == false &&
			nReadIO[0] == IO_OFF && nReadIO[1] == IO_ON &&
			nReadIO[2] == IO_OFF && nReadIO[3] == IO_ON)
		{
			if (m_lWait_UDGripper[2] > st_time.nWait_Off[CYL_ACCY_BOX_UD_GRIPPER])
			{
				nFunRet = CTL_GOOD;	// [OFF:DOWN]
			}
		}
		else
		{
			if (m_lWait_UDGripper[2] > st_time.nWait_Limit[CYL_ACCY_BOX_UD_GRIPPER])
			{
				// 134000 0 00 "PS0501_BOX_CLAMP_GRIPPER_DOWN_CHK_ERR."
				alarm.mstr_code		= "134000";

				alarm.mn_count_mode	= 0;
				alarm.mn_type_mode	= eWARNING;
				st_work.nEqpStatus	= dWARNING;

				nFunRet = CTL_ERROR;
			}
		}
	}
	else		// �ڽ��� UP �� ����
	{
		if (m_bflag_UDGripper == true &&
			nReadIO[0] == IO_ON && nReadIO[1] == IO_OFF &&
			nReadIO[2] == IO_ON && nReadIO[3] == IO_OFF)
		{
			m_lWait_UDGripper[0] = GetCurrentTime();
			m_bflag_UDGripper = false;
		}
		else if (m_bflag_UDGripper == false &&
			nReadIO[0] == IO_ON && nReadIO[1] == IO_OFF &&
			nReadIO[2] == IO_ON && nReadIO[3] == IO_OFF)
		{
			if (m_lWait_UDGripper[2] > st_time.nWait_On[CYL_ACCY_BOX_UD_GRIPPER])
			{
				nFunRet = CTL_GOOD;	// [OFF:DOWN]
			}
		}
		else
		{
			if (m_lWait_UDGripper[2] > st_time.nWait_Limit[CYL_ACCY_BOX_UD_GRIPPER])
			{
				// 134001 0 00 "PS0500_BOX_CLAMP_GRIPPER_UP_CHK_ERR."
				alarm.mstr_code		= "134001";

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
// �Ǽ��縮 �ڽ��� �����ְ�, ������ ���� �����ִ� �Ǹ���
// - IO_ON = UP �����̸�, �ڽ��� ��ħ
// - IO_OFF = Down �����̸�, �ڽ��� ����
//==================================================================//
void CSeq_BoxClamp::OnSet_CylLifter(int nzOnOff) 
{
	g_ioMgr.set_out_bit(stIO.o_Cyl_AccyBoxLifterUp, nzOnOff);
	g_ioMgr.set_out_bit(stIO.o_Cyl_AccyBoxLifterDn, !nzOnOff);

	m_bflag_Lifter = true;
	m_lWait_Lifter[0] = GetCurrentTime();
}

int CSeq_BoxClamp::OnGet_CylLifter(int nzOnOff, int nzSite) 
{
	int nFunRet = CTL_PROCEED;

	int nReadIO[4] = {0,};
	nReadIO[0] = g_ioMgr.get_in_bit(stIO.i_Chk_AccyBoxLifterUp[SIDE_LEFT_]);
	nReadIO[1] = g_ioMgr.get_in_bit(stIO.i_Chk_AccyBoxLifterDn[SIDE_LEFT_]);
	nReadIO[2] = g_ioMgr.get_in_bit(stIO.i_Chk_AccyBoxLifterUp[SIDE_RIGHT_]);
	nReadIO[3] = g_ioMgr.get_in_bit(stIO.i_Chk_AccyBoxLifterDn[SIDE_RIGHT_]);

	if (nzSite == SIDE_LEFT_)
	{
		// [Right] ���� �Է��� ������ ����
		nReadIO[2] =  nzOnOff;
		nReadIO[3] = !nzOnOff;
	}
	else if (nzSite == SIDE_RIGHT_)
	{
		// [Left] ���� �Է��� ������ ����
		nReadIO[0] =  nzOnOff;
		nReadIO[1] = !nzOnOff;
	}

	m_lWait_Lifter[1] = GetCurrentTime();
	m_lWait_Lifter[2] = m_lWait_Lifter[1] - m_lWait_Lifter[0];

	if (m_lWait_Lifter[2] < 0)
	{
		m_lWait_Lifter[0] = GetCurrentTime();
	}

	if (nzOnOff == IO_OFF)		// �ڽ� ���� ��ġ
	{
		if (m_bflag_Lifter == true &&
			nReadIO[0] == IO_OFF && nReadIO[1] == IO_ON &&
			nReadIO[2] == IO_OFF && nReadIO[3] == IO_ON)
		{
			m_lWait_Lifter[0] = GetCurrentTime();
			m_bflag_Lifter = false;
		}
		else if (m_bflag_Lifter == false &&
			nReadIO[0] == IO_OFF && nReadIO[1] == IO_ON &&
			nReadIO[2] == IO_OFF && nReadIO[3] == IO_ON)
		{
			if (m_lWait_Lifter[2] > st_time.nWait_Off[CYL_ACCY_BOX_LIFTER])
			{
				nFunRet = CTL_GOOD;	
			}
		}
		else
		{
			if (m_lWait_Lifter[2] > st_time.nWait_Limit[CYL_ACCY_BOX_LIFTER])
			{
				// 135000 0 00 "PS0401_PS0403_BOX_LIFTER_DOWN_CHK_ERR."
				alarm.mstr_code		= "135000";

				alarm.mn_count_mode	= 0;
				alarm.mn_type_mode	= eWARNING;
				st_work.nEqpStatus	= dWARNING;

				nFunRet = CTL_ERROR;
			}
		}
	}
	else		// �ڽ� �ϴ� ���� ��ġ
	{
		if (m_bflag_Lifter == true &&
			nReadIO[0] == IO_ON && nReadIO[1] == IO_OFF &&
			nReadIO[2] == IO_ON && nReadIO[3] == IO_OFF)
		{
			m_lWait_Lifter[0] = GetCurrentTime();
			m_bflag_Lifter = false;
		}
		else if (m_bflag_Lifter == false &&
			nReadIO[0] == IO_ON && nReadIO[1] == IO_OFF &&
			nReadIO[2] == IO_ON && nReadIO[3] == IO_OFF)
		{
			if (m_lWait_Lifter[2] > st_time.nWait_On[CYL_ACCY_BOX_LIFTER])
			{
				nFunRet = CTL_GOOD;	
			}
		}
		else
		{
			if (m_lWait_Lifter[2] > st_time.nWait_Limit[CYL_ACCY_BOX_LIFTER])
			{
				// 135001 0 00 "PS0400_PS0402_BOX_LIFTER_UP_CHK_ERR."
				alarm.mstr_code		= "135001";

				alarm.mn_count_mode	= 0;
				alarm.mn_type_mode	= eWARNING;
				st_work.nEqpStatus	= dWARNING;

				nFunRet = CTL_ERROR;
			}
		}
	}
	return nFunRet;
}



