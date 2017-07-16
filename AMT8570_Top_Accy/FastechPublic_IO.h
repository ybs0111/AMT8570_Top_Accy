// FastechPublic_IO.h: interface for the CFastechPublic_IO class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FASTECHPUBLIC_IO_H__C22EE9E3_BCDD_43ED_B749_CDFE31F58416__INCLUDED_)
#define AFX_FASTECHPUBLIC_IO_H__C22EE9E3_BCDD_43ED_B749_CDFE31F58416__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


/////////////////////////////////////////////////////////
//DLL 구조로 변경예정 //2011.1007 추가 
/////////////////////////////////////////////////////////
#define BD_NO				0		//어떤 조건이 클리어된 상태 
#define BD_YES				1		//어떤 조건을 요청한 상태 

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//motor, i/O 보드등의 리턴값은 이 것만 사용한다  
//////////////////////////////////////////////////////////////////////////////////////////////////////////
#define BD_ERROR	   -1
#define BD_PROCEED		0
#define BD_GOOD			1
#define BD_RETRY		2
#define BD_SAFETY		3


#define DBIT0	0x0001 
#define DBIT1	0x0002 
#define DBIT2	0x0004 
#define DBIT3	0x0008 
#define DBIT4	0x0010 
#define DBIT5	0x0020 
#define DBIT6	0x0040 
#define DBIT7	0x0080 

#define DBIT8	0x0100 
#define DBIT9	0x0200 
#define DBIT10	0x0400 
#define DBIT11	0x0800 
#define DBIT12	0x1000 
#define DBIT13	0x2000 
#define DBIT14	0x4000 
#define DBIT15	0x8000 

#define DBIT16	0x00010000 
#define DBIT17	0x00020000 
#define DBIT18	0x00040000 
#define DBIT19	0x00080000 
#define DBIT20	0x00100000 
#define DBIT21	0x00200000 
#define DBIT22	0x00400000 
#define DBIT23	0x00800000 

#define DBIT24	0x01000000 
#define DBIT25	0x02000000 
#define DBIT26	0x04000000 
#define DBIT27	0x08000000 
#define DBIT28	0x10000000 
#define DBIT29	0x20000000 
#define DBIT30	0x40000000 
#define DBIT31	0x80000000 
//#define HSSI_DEBUG		1 //2011.1019 
//#define HSSI_GOOD			0
//#define HSSI_ERROR		-1
#define HSSI_MAX_IO		7715 //max io board 2장 까지만 선언하여 사용하자 //max io board 4장 까지만 처리한다 

class CFastechPublic_IO  
{
public:
	CFastechPublic_IO();
	virtual ~CFastechPublic_IO();

public:
	int mn_errormsg_debug_mode; //에러 메세지 출력방법 => 0:사용안함 , 1:사용
	int mn_simulation_mode; //가상 동작을 사용시 사용  => 0:사용안함 , 1:사용 

	int n_in[HSSI_MAX_IO];   // 전체 인 포트 I/O 상태 저장 변수
	int n_out[HSSI_MAX_IO];  // 전체 아웃 포트 I/O 상태 저장 변수

	int m_nflag_IOBrdInit;



public:

	/////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////
	//2011.1019 james lee WORD 단위로 I/O를 제어한다
	//Ver 0.0.1_2011.1019
	/////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////
	int IO_ReadWrite(int n_bd_num, int n_port_num, int n_slave_num, int n_rw_mode);//2011.1019 james //Word 단위로 제어시 사용하는 함수 
	int get_in_bit(int n_IONo, int n_OnOff);  //2011.1019 Simulation 동작구현 목적으로 onoff 추가 //Word 단위로 제어시 사용하는 함수
	int set_out_bit(int n_IONo, int n_OnOff); //2011.1019 Simulation 동작구현 목적으로 onoff 추가 //Word 단위로 제어시 사용하는 함수
	int get_out_bit(int n_IONo, int n_OnOff); //2011.1019 Simulation 동작구현 목적으로 onoff 추가 //Word 단위로 제어시 사용하는 함수
	/////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////


	int  Search_IO_BoardInformation(int nType, int nData);             // HSSI 보드 데이터 확인 및 검사 함수
	int  Set_IO_BoardOpen(INT* piMasterNo, int nOpenMethod);           // I/O 보드 오픈 함수
	int  Set_IO_BoardClose(bool bCloseMethod);                         // I/O 보드 해제 함수
	int  Search_IO_Master(INT* piMasterNo);                            // PC에 연결된 마스터 보드 갯수 검사 함수
	int  Search_IO_Port(int nMasterID, INT* piPortNo);                 // 마스터 보드에 연결된 PORT 갯수 검사 함수
	int  Search_IO_Slave(int nMasterID, int nPortNo, INT* piSlaveNo);  // PORT에 연결된 SLAVE 검사 함수
	BOOL Check_IO_Slave(int nMasterID, int nPortNo, int nSlaveID);     // PORT에 연결된 SLAVE [ENABLE/DISABLE] 상태 검사 함수

	int  Set_IO_HSSISpeed(int nMasterID, int nPortNo, int nType);                  // PORT와 SLAVE I/O 모듈과의 통신 속도 설정 함수
	int  Get_IO_HSSISpeed(int nMasterID, int nPortNo, INT* piType);                // PORT와 SLAVE I/O 모듈과의 통신 속도 리턴 함수
	int  Set_IO_PortAutoEnable(int nMasterID, int nPortNo);                           // PORT와 연결된 모든 SLAVE I/O 모듈 자동 ENABLE 함수
	int  Set_IO_SlaveEnable(int nMasterID, int nPortNo, int nSlaveNo, BOOL bEnable);  // SLAVE I/O 모듈 동작 ENABLE/DISABLE 설정 함수
	
	int  Set_IO_DefineWord(int nMasterID, int nPortNo, int nSlaveNo, WORD wData);               // SLAVE I/O 모듈의 입출력 설정 함수 [WORD 단위]
	int  Set_IO_DefineByte(int nMasterID, int nPortNo, int nSlaveNo, int nType, BYTE btData);   // SLAVE I/O 모듈의 입출력 설정 함수 [BYTE 단위]
	int  Set_IO_DefineBit(int nIONo, BOOL bData);                                               // SLAVE I/O 모듈의 입출력 설정 함수 [BIT 단위]
	int  Get_IO_DefineWord(int nMasterID, int nPortNo, int nSlaveNo, WORD* pwData);             // SLAVE I/O 모듈의 입출력 설정 상태 리턴 [WORD 단위]
	int  Get_IO_DefineByte(int nMasterID, INT iPortNo, int nSlaveNo, int nType, BYTE* pbData);  // SLAVE I/O 모듈의 입출력 설정 상태 리턴 [BYTE 단위]
	int  Get_IO_DefineBit(int nIONo, BOOL* pbData);                                             // SLAVE I/O 모듈의 입출력 설정 상태 리턴 [BIT 단위]
	
	int  Set_IO_LatchWord(int nMasterID, int nPortNo, int nSlaveNo, WORD wData);                  // SLAVE I/O 모듈의 입력을 LATCH 모드로 설정 함수 [WORD 단위]
	int  Set_IO_LatchByte(int nMasterID, int nPortNo, int nSlaveNo, int nType, BYTE btData);      // SLAVE I/O 모듈의 입력을 LATCH 모드로 설정 함수 [BYTE 단위]
	int  Set_IO_LatchBit(int nIONo, BOOL bData);                                                  // SLAVE I/O 모듈의 입력을 LATCH 모드로 설정 함수 [BIT 단위]
	WORD Get_IO_LatchWord(int nMasterID, int nPortNo, int nSlaveNo);                              // SLAVE I/O 모듈의 LATCH 상태 리턴 함수 [WORD 단위]
	BYTE Get_IO_LatchByte(int nMasterID, INT iPortNo, int nSlaveNo, int nType);                   // SLAVE I/O 모듈의 LATCH 상태 리턴 함수 [BYTE 단위]
	BOOL Get_IO_LatchBit(int nIONo);                                                              // SLAVE I/O 모듈의 LATCH 상태 리턴 함수 [BIT 단위]
	int  Reset_IO_LatchWord(int nMasterID, int nPortNo, int nSlaveNo, WORD wData);                // SLAVE I/O 모듈의 LATCH 동작 정지 함수 [WORD 단위]
	int  Reset_IO_LatchByte(int nMasterID, int nPortNo, int nSlaveNo, int nType, BYTE btData);    // SLAVE I/O 모듈의 LATCH 동작 정지 함수 [BYTE 단위]
	int  Reset_IO_LatchBit(int nIONo, BOOL bData);                                                // SLAVE I/O 모듈의 LATCH 동작 정지 함수 [BIT 단위]
	
	int  Check_IO_MasterStatus(INT nMasterID);                            // 마스터 보드 장착 유무 검사 함수
	int  Check_IO_PortStatus(int nMasterID, int nPortNo);                 // PORT 존재 유무 및 정상 동작 검사 함수
	int  Check_IO_SlaveStatus(int nMasterID, int nPortNo, int nSlaveNo);  // SLAVE I/O MODULE이 ENABLE 상태로 마스터 보드에 연결되어 있는지 검사 함수

	int  Check_IO_BoardExist(int nMaxSlot, int nMaxPort, int nMaxSlave);  // 보드 장착 상태 검사 함수

	BOOL Check_Bit(long nValue, int nBitPos);            // 특정 위치 BIT값 검사 함수
	void Set_Bit(long &nValue, int nBit, BOOL bOn);      // 특정 위치 BIT 변경 함수

	
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// I/O 관련 LIB
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	int  Initialize_Auto_IO_Board(void);   // I/O 보드 라이브러리 초기화 함수

	int  Set_Out_Word(int nIONo, WORD wData);              // SLAVE I/O 모듈 출력 신호 발생 함수 [WORD 단위]
	int  Set_Out_Byte(int nIONo, int nType, BYTE btData);  // SLAVE I/O 모듈 출력 신호 발생 함수 [BYTE 단위]
	int  Set_Out_Bit (int nIONo, BOOL bOnOff);             // SLAVE I/O 모듈 출력 신호 발생 함수 [BIT 단위]

	WORD Get_In_Word(int nIONo);                           // SLAVE I/O 모듈 입력 신호 READ 함수 [WORD 단위]
	BYTE Get_In_Byte(int nIONo, int nType);                // SLAVE I/O 모듈 입력 신호 READ 함수 [BYTE 단위]
	BOOL Get_In_Bit(int nIONo);                            // SLAVE I/O 모듈 입력 신호 READ 함수 [BIT 단위]

//	BOOL Get_In_Bit(int nIONo, int n_SetTime);//james 2010,0128 , long * np_GetTime) ; //james 2009.5.27 동작하고 있는 시간을 찾기 위해 추가 

	WORD Get_Out_Word(int nIONo);							// SLAVE I/O 모듈 출력 신호 READ 함수 [WORD 단위]
	BYTE Get_Out_Byte(int nIONo, int nType);				// SLAVE I/O 모듈 출력 신호 READ 함수 [BYTE 단위]
	BOOL Get_Out_Bit(int nIONo);							// SLAVE I/O 모듈 출력 신호 READ 함수 [BIT 단위]

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Word(16bit) 단위로 제어한다 //Picker 등의 제어를 위해 작성  
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	int Lib_Set_Output_16(int nIoNum, int nOnOff, int *np_Data);  
	int Lib_Get_Input_16(int nIoNum, int nOnOff, int *np_Data);
	int Lib_Get_Output_16(int nIoNum, int nOnOff, int *np_Data);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//	int Check_IO_Port_Yield(int n_io, int n_time, int n_yield = 70);
//	int Check_IO_Port(int n_io, int n_time = 0, int n_flag = CTL_ON);
		

	
	
	// **************************************************************************
	// I/O 보드 관련 클래스                                                      
	// **************************************************************************
	char* ErrorOccurInfo(int nErrorNum);  // HSSI DLL 함수 호출 관련 에러 파싱 함수

};
extern CFastechPublic_IO  FAS_IO;  

#endif // !defined(AFX_FASTECHPUBLIC_IO_H__C22EE9E3_BCDD_43ED_B749_CDFE31F58416__INCLUDED_)
