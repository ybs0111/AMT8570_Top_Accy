// Copyright (C) 2002 AMT Corporation
// All rights reserved.
//
// DIVISION    : A.M.T. Coporation
// MODIFIED BY : ViboX Kim
// REVISION    :
//     2011. 02. 19 : Modified by ViboX Kim
//

#ifndef _STRUCT_H_
#define _STRUCT_H_

// *****************************************************************************
// 화면 뷰 클래스 추가 부분
// *****************************************************************************
#include "Screen_Main.h"				// 메인 화면 클래스 추가 
#include "Screen_Lock.h"				// System Locking 화면 클래스 추가 
#include "Screen_Initial.h"				// 초기화 진행 화면 클래스 추가 

#include "Screen_Basic.h"				// 베이직 설정 화면 클래스 추가 

#include "Screen_Set_Maintenance.h"		// 메인트 설정 화면 클래스 추가 
#include "Screen_Wait_Time.h"			// 대기 시간 정보 설정 화면 클래스 추가 
#include "Screen_File_Editor.h"			// File Editor 화면 클래스 추가 
 
#include "Screen_Motor.h"
#include "Screen_Motor_Speed.h"			// 모터 스피드 화면 클래스 추가.
#include "Screen_List_Operation.h"		// Operation 정보 출력 화면 클래스 추가 
#include "Screen_List_Data_View.h"
#include "Screen_IO.h"					// I/O 상태 정보 출력 화면 클래스 추가 

#include "Screen_Admin.h"				// Administrator 출력 화면 클래스 추가 
// *****************************************************************************

// *****************************************************************************
// 화면 뷰 클래스 ID 설정 부분
// *****************************************************************************
#define IDW_SCREEN_MAIN					101
#define IDW_SCREEN_LOCK					102
#define IDW_SCREEN_INIT					103
	
#define IDW_SCREEN_BASIC		 		201

#define IDW_SCREEN_SET_MAINTENANCE		301

#define IDW_SCREEN_WAIT_TIME			401

#define IDW_SCREEN_MOTOR_TEACH			501
#define IDW_SCREEN_MOTOR_SPEED			502

#define IDW_SCREEN_LIST_OPERATION		601
#define IDW_SCREEN_LIST_DATA_VIEW		602
#define IDW_SCREEN_LIST_STEP			603

#define IDW_SCREEN_IO					701

#define IDW_SCREEN_ADMINISTRATOR		801
#define IDW_SCREEN_FILE_EDITOR			802
// *****************************************************************************

#endif