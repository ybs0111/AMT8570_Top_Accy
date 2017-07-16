# Microsoft Developer Studio Project File - Name="Handler" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Handler - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Handler.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Handler.mak" CFG="Handler - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Handler - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Handler - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Handler - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 TipsWareExt.lib libmysql.lib MmdSerial.lib FM_8000.lib netapi32.lib HSSI.lib Spr32d70.lib wininet.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "Handler - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 TipsWareExt.lib libmysql.lib FM_8000.lib netapi32.lib HSSI.lib Spr32d70.lib wininet.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "Handler - Win32 Release"
# Name "Handler - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ABpcManager.cpp
# End Source File
# Begin Source File

SOURCE=.\Alg_Mysql.cpp
# End Source File
# Begin Source File

SOURCE=.\AMTLotManager.cpp
# End Source File
# Begin Source File

SOURCE=.\AMTRegistry.cpp
# End Source File
# Begin Source File

SOURCE=.\BCMenu.cpp
# End Source File
# Begin Source File

SOURCE=.\BmpImage.cpp
# End Source File
# Begin Source File

SOURCE=.\BmpImageST.cpp
# End Source File
# Begin Source File

SOURCE=.\BtnST.cpp
# End Source File
# Begin Source File

SOURCE=.\CeXDib.cpp
# End Source File
# Begin Source File

SOURCE=.\CharSetCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\ClockST.cpp
# End Source File
# Begin Source File

SOURCE=.\Cmmsdk.cpp
# End Source File
# Begin Source File

SOURCE=.\ColorListBox.cpp
# End Source File
# Begin Source File

SOURCE=.\COMI_Manager.cpp
# End Source File
# Begin Source File

SOURCE=.\ComizoaPublic.cpp
# End Source File
# Begin Source File

SOURCE=.\ControlToolTip.cpp
# End Source File
# Begin Source File

SOURCE=.\CtlBd_Function.cpp
# End Source File
# Begin Source File

SOURCE=.\CtlBd_Library.cpp
# End Source File
# Begin Source File

SOURCE=.\Dialog_About.cpp
# End Source File
# Begin Source File

SOURCE=.\Dialog_Alarm.cpp
# End Source File
# Begin Source File

SOURCE=.\Dialog_Copy.cpp
# End Source File
# Begin Source File

SOURCE=.\Dialog_Event_Msg.cpp
# End Source File
# Begin Source File

SOURCE=.\Dialog_Exit.cpp
# End Source File
# Begin Source File

SOURCE=.\Dialog_Infor.cpp
# End Source File
# Begin Source File

SOURCE=.\Dialog_Keyboard.cpp
# End Source File
# Begin Source File

SOURCE=.\Dialog_KeyPad.cpp
# End Source File
# Begin Source File

SOURCE=.\Dialog_List.cpp
# End Source File
# Begin Source File

SOURCE=.\Dialog_Menu.cpp
# End Source File
# Begin Source File

SOURCE=.\Dialog_Message.cpp
# End Source File
# Begin Source File

SOURCE=.\Dialog_Motor_Axis_1.cpp
# End Source File
# Begin Source File

SOURCE=.\Dialog_Motor_Axis_Tab.cpp
# End Source File
# Begin Source File

SOURCE=.\Dialog_Motor_Part.cpp
# End Source File
# Begin Source File

SOURCE=.\Dialog_Multi_Msg.cpp
# End Source File
# Begin Source File

SOURCE=.\Dialog_Network.cpp
# End Source File
# Begin Source File

SOURCE=.\Dialog_Part_IO.cpp
# End Source File
# Begin Source File

SOURCE=.\Dialog_Pass_Change.cpp
# End Source File
# Begin Source File

SOURCE=.\Dialog_Pass_Check.cpp
# End Source File
# Begin Source File

SOURCE=.\Dialog_Select.cpp
# End Source File
# Begin Source File

SOURCE=.\Dialog_SetReset.cpp
# End Source File
# Begin Source File

SOURCE=.\Dialog_Smema.cpp
# End Source File
# Begin Source File

SOURCE=.\Dialog_Title.cpp
# End Source File
# Begin Source File

SOURCE=.\Digit.cpp
# End Source File
# Begin Source File

SOURCE=.\editex.cpp
# End Source File
# Begin Source File

SOURCE=.\EXDigitST.cpp
# End Source File
# Begin Source File

SOURCE=.\exgem.cpp
# End Source File
# Begin Source File

SOURCE=.\FastechPublic_IO.cpp
# End Source File
# Begin Source File

SOURCE=.\FileDialogST.cpp
# End Source File
# Begin Source File

SOURCE=.\FileManagement.cpp
# End Source File
# Begin Source File

SOURCE=.\FloatST.cpp
# End Source File
# Begin Source File

SOURCE=.\FPSDatePickerCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\FPSDateTimeButtonCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\FPSDateTimeCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\FPSDateTimePopupCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\FPSMiniCalendarCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\FPSMiniCalendarListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\GradientStatic.cpp
# End Source File
# Begin Source File

SOURCE=.\GridControlAlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Handler.cpp
# End Source File
# Begin Source File

SOURCE=.\Handler.rc
# End Source File
# Begin Source File

SOURCE=.\HandlerDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\HandlerView.cpp
# End Source File
# Begin Source File

SOURCE=.\InitDialogBar.cpp
# End Source File
# Begin Source File

SOURCE=.\IO_Manager.cpp
# End Source File
# Begin Source File

SOURCE=.\LedButton.cpp
# End Source File
# Begin Source File

SOURCE=.\ListBoxST.cpp
# End Source File
# Begin Source File

SOURCE=.\MacButtons.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\MatrixStatic.cpp
# End Source File
# Begin Source File

SOURCE=.\MyBasicData.cpp
# End Source File
# Begin Source File

SOURCE=.\MyJamData.cpp
# End Source File
# Begin Source File

SOURCE=.\NetWork_Comm.cpp
# End Source File
# Begin Source File

SOURCE=.\NewLabel.cpp
# End Source File
# Begin Source File

SOURCE=.\OleDateTimeEx.cpp
# End Source File
# Begin Source File

SOURCE=.\Picture.cpp
# End Source File
# Begin Source File

SOURCE=.\PictureEx.cpp
# End Source File
# Begin Source File

SOURCE=.\Screen_Admin.cpp
# End Source File
# Begin Source File

SOURCE=.\Screen_Basic.cpp
# End Source File
# Begin Source File

SOURCE=.\Screen_File_Editor.cpp
# End Source File
# Begin Source File

SOURCE=.\Screen_Initial.cpp
# End Source File
# Begin Source File

SOURCE=.\Screen_IO.cpp
# End Source File
# Begin Source File

SOURCE=.\Screen_List_Data_View.cpp
# End Source File
# Begin Source File

SOURCE=.\Screen_List_Operation.cpp
# End Source File
# Begin Source File

SOURCE=.\Screen_Lock.cpp
# End Source File
# Begin Source File

SOURCE=.\Screen_Main.cpp
# End Source File
# Begin Source File

SOURCE=.\Screen_Motor.cpp
# End Source File
# Begin Source File

SOURCE=.\Screen_Motor_Speed.cpp
# End Source File
# Begin Source File

SOURCE=.\Screen_Set_Maintenance.cpp
# End Source File
# Begin Source File

SOURCE=.\Screen_Wait_Time.cpp
# End Source File
# Begin Source File

SOURCE=.\ShadeButtonST.cpp
# End Source File
# Begin Source File

SOURCE=.\SingleInstanceApp.cpp
# End Source File
# Begin Source File

SOURCE=.\StarWarsCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SuperProgressCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\TrueColorToolBar.cpp
# End Source File
# Begin Source File

SOURCE=.\UniButtonEx.cpp
# End Source File
# Begin Source File

SOURCE=.\VLabel.cpp
# End Source File
# Begin Source File

SOURCE=.\XPButton.cpp
# End Source File
# Begin Source File

SOURCE=.\XPGroupBox.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "Control"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\BmpImage.h
# End Source File
# Begin Source File

SOURCE=.\BmpImageST.h
# End Source File
# Begin Source File

SOURCE=.\BtnST.h
# End Source File
# Begin Source File

SOURCE=.\ClockST.h
# End Source File
# Begin Source File

SOURCE=.\ColorListBox.h
# End Source File
# Begin Source File

SOURCE=.\ControlToolTip.h
# End Source File
# Begin Source File

SOURCE=.\Digit.h
# End Source File
# Begin Source File

SOURCE=.\editex.h
# End Source File
# Begin Source File

SOURCE=.\EXDigitST.h
# End Source File
# Begin Source File

SOURCE=.\FloatST.h
# End Source File
# Begin Source File

SOURCE=.\GradientStatic.h
# End Source File
# Begin Source File

SOURCE=.\LedButton.h
# End Source File
# Begin Source File

SOURCE=.\ListBoxST.h
# End Source File
# Begin Source File

SOURCE=.\MacButtons.h
# End Source File
# Begin Source File

SOURCE=.\MatrixStatic.h
# End Source File
# Begin Source File

SOURCE=.\NewLabel.h
# End Source File
# Begin Source File

SOURCE=.\PictureEx.h
# End Source File
# Begin Source File

SOURCE=.\StarWarsCtrl.h
# End Source File
# Begin Source File

SOURCE=.\TrueColorToolBar.h
# End Source File
# Begin Source File

SOURCE=.\UniButtonEx.h
# End Source File
# Begin Source File

SOURCE=.\VLabel.h
# End Source File
# Begin Source File

SOURCE=.\XPButton.h
# End Source File
# Begin Source File

SOURCE=.\XPGroupBox.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\ABpcManager.h
# End Source File
# Begin Source File

SOURCE=.\ACriticalSection.h
# End Source File
# Begin Source File

SOURCE=.\Alg_Mysql.h
# End Source File
# Begin Source File

SOURCE=.\AMTLotManager.h
# End Source File
# Begin Source File

SOURCE=.\AMTRegistry.h
# End Source File
# Begin Source File

SOURCE=.\BCMenu.h
# End Source File
# Begin Source File

SOURCE=.\CeXDib.h
# End Source File
# Begin Source File

SOURCE=.\CharSetCtrl.h
# End Source File
# Begin Source File

SOURCE=.\Cmmsdk.h
# End Source File
# Begin Source File

SOURCE=.\CmmsdkDef.h
# End Source File
# Begin Source File

SOURCE=.\COMI_Manager.h
# End Source File
# Begin Source File

SOURCE=.\ComizoaPublic.h
# End Source File
# Begin Source File

SOURCE=.\CtlBd_Function.h
# End Source File
# Begin Source File

SOURCE=.\CtlBd_Library.h
# End Source File
# Begin Source File

SOURCE=.\Ctlbd_Variable.h
# End Source File
# Begin Source File

SOURCE=.\Dialog_About.h
# End Source File
# Begin Source File

SOURCE=.\Dialog_Alarm.h
# End Source File
# Begin Source File

SOURCE=.\Dialog_Copy.h
# End Source File
# Begin Source File

SOURCE=.\Dialog_Event_Msg.h
# End Source File
# Begin Source File

SOURCE=.\Dialog_Exit.h
# End Source File
# Begin Source File

SOURCE=.\Dialog_Infor.h
# End Source File
# Begin Source File

SOURCE=.\Dialog_Keyboard.h
# End Source File
# Begin Source File

SOURCE=.\Dialog_KeyPad.h
# End Source File
# Begin Source File

SOURCE=.\Dialog_List.h
# End Source File
# Begin Source File

SOURCE=.\Dialog_Menu.h
# End Source File
# Begin Source File

SOURCE=.\Dialog_Message.h
# End Source File
# Begin Source File

SOURCE=.\Dialog_Motor_Axis_1.h
# End Source File
# Begin Source File

SOURCE=.\Dialog_Motor_Axis_Tab.h
# End Source File
# Begin Source File

SOURCE=.\Dialog_Motor_Part.h
# End Source File
# Begin Source File

SOURCE=.\Dialog_Multi_Msg.h
# End Source File
# Begin Source File

SOURCE=.\Dialog_Network.h
# End Source File
# Begin Source File

SOURCE=.\Dialog_Part_IO.h
# End Source File
# Begin Source File

SOURCE=.\Dialog_Pass_Change.h
# End Source File
# Begin Source File

SOURCE=.\Dialog_Pass_Check.h
# End Source File
# Begin Source File

SOURCE=.\Dialog_Select.h
# End Source File
# Begin Source File

SOURCE=.\Dialog_SetReset.h
# End Source File
# Begin Source File

SOURCE=.\Dialog_Smema.h
# End Source File
# Begin Source File

SOURCE=.\Dialog_Title.h
# End Source File
# Begin Source File

SOURCE=.\exgem.h
# End Source File
# Begin Source File

SOURCE=.\FAS_HSSI.h
# End Source File
# Begin Source File

SOURCE=.\FastechPublic_IO.h
# End Source File
# Begin Source File

SOURCE=.\FileDialogST.h
# End Source File
# Begin Source File

SOURCE=.\FileManagement.h
# End Source File
# Begin Source File

SOURCE=.\FPSDatePickerCtrl.h
# End Source File
# Begin Source File

SOURCE=.\FPSDateTimeButtonCtrl.h
# End Source File
# Begin Source File

SOURCE=.\FPSDateTimeCtrl.h
# End Source File
# Begin Source File

SOURCE=.\FPSDateTimePopupCtrl.h
# End Source File
# Begin Source File

SOURCE=.\FPSMiniCalendarCtrl.h
# End Source File
# Begin Source File

SOURCE=.\FPSMiniCalendarListCtrl.h
# End Source File
# Begin Source File

SOURCE=.\GridControlAlg.h
# End Source File
# Begin Source File

SOURCE=.\Handler.h
# End Source File
# Begin Source File

SOURCE=.\HandlerDoc.h
# End Source File
# Begin Source File

SOURCE=.\HandlerView.h
# End Source File
# Begin Source File

SOURCE=.\InitDialogBar.h
# End Source File
# Begin Source File

SOURCE=.\IO_Manager.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\MemDC.h
# End Source File
# Begin Source File

SOURCE=.\MenuHead.h
# End Source File
# Begin Source File

SOURCE=.\MyBasicData.h
# End Source File
# Begin Source File

SOURCE=.\MyJamData.h
# End Source File
# Begin Source File

SOURCE=.\NetWork_Comm.h
# End Source File
# Begin Source File

SOURCE=.\OleDateTimeEx.h
# End Source File
# Begin Source File

SOURCE=.\Picture.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\Screen_Admin.h
# End Source File
# Begin Source File

SOURCE=.\Screen_Basic.h
# End Source File
# Begin Source File

SOURCE=.\Screen_File_Editor.h
# End Source File
# Begin Source File

SOURCE=.\Screen_Initial.h
# End Source File
# Begin Source File

SOURCE=.\Screen_IO.h
# End Source File
# Begin Source File

SOURCE=.\Screen_List_Data_View.h
# End Source File
# Begin Source File

SOURCE=.\Screen_List_Operation.h
# End Source File
# Begin Source File

SOURCE=.\Screen_Lock.h
# End Source File
# Begin Source File

SOURCE=.\Screen_Main.h
# End Source File
# Begin Source File

SOURCE=.\Screen_Motor.h
# End Source File
# Begin Source File

SOURCE=.\Screen_Motor_Speed.h
# End Source File
# Begin Source File

SOURCE=.\Screen_Set_Maintenance.h
# End Source File
# Begin Source File

SOURCE=.\Screen_Wait_Time.h
# End Source File
# Begin Source File

SOURCE=.\ShadeButtonST.h
# End Source File
# Begin Source File

SOURCE=.\SingleInstanceApp.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\SuperProgressCtrl.h
# End Source File
# Begin Source File

SOURCE=.\SxLogFont.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\Alarm.gif
# End Source File
# Begin Source File

SOURCE=.\res\amt_logo_fast.gif
# End Source File
# Begin Source File

SOURCE=.\res\Big_3.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Choice.gif
# End Source File
# Begin Source File

SOURCE=.\res\ClockST_big.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ClockST_pane.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ClockST_small.bmp
# End Source File
# Begin Source File

SOURCE=.\res\cursor1.cur
# End Source File
# Begin Source File

SOURCE=.\res\cursor2.cur
# End Source File
# Begin Source File

SOURCE=.\res\cursor3.cur
# End Source File
# Begin Source File

SOURCE=.\res\datepick.bmp
# End Source File
# Begin Source File

SOURCE=.\res\default1.bin
# End Source File
# Begin Source File

SOURCE=.\res\digit_b.bmp
# End Source File
# Begin Source File

SOURCE=.\res\digit_bd.bmp
# End Source File
# Begin Source File

SOURCE=.\res\digit_c.bmp
# End Source File
# Begin Source File

SOURCE=.\res\digit_d.bmp
# End Source File
# Begin Source File

SOURCE=.\res\digit_g.bmp
# End Source File
# Begin Source File

SOURCE=.\res\digit_r.bmp
# End Source File
# Begin Source File

SOURCE=.\res\digit_y.bmp
# End Source File
# Begin Source File

SOURCE=.\res\digitB_b.bmp
# End Source File
# Begin Source File

SOURCE=.\res\digitB_bd.bmp
# End Source File
# Begin Source File

SOURCE=.\res\digitB_bl.bmp
# End Source File
# Begin Source File

SOURCE=.\res\digitB_c.bmp
# End Source File
# Begin Source File

SOURCE=.\res\digitB_d.bmp
# End Source File
# Begin Source File

SOURCE=.\res\digitB_g.bmp
# End Source File
# Begin Source File

SOURCE=.\res\digitB_r.bmp
# End Source File
# Begin Source File

SOURCE=.\res\digitB_y.bmp
# End Source File
# Begin Source File

SOURCE=.\res\dn.ico
# End Source File
# Begin Source File

SOURCE=.\res\emg_stop.ico
# End Source File
# Begin Source File

SOURCE=.\res\Exit.gif
# End Source File
# Begin Source File

SOURCE=.\res\exit.ico
# End Source File
# Begin Source File

SOURCE=.\res\greenButton.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Handler.ico
# End Source File
# Begin Source File

SOURCE=.\res\Handler.rc2
# End Source File
# Begin Source File

SOURCE=.\res\HandlerDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\home.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\ICT.ico
# End Source File
# Begin Source File

SOURCE=".\res\initial-1.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\initial-2.bmp"
# End Source File
# Begin Source File

SOURCE=.\res\Initial.gif
# End Source File
# Begin Source File

SOURCE=.\res\io.ico
# End Source File
# Begin Source File

SOURCE=.\res\Keyboard.ico
# End Source File
# Begin Source File

SOURCE=.\res\LedGreen.ico
# End Source File
# Begin Source File

SOURCE=.\res\LedNone.ico
# End Source File
# Begin Source File

SOURCE=.\res\LedRed.ico
# End Source File
# Begin Source File

SOURCE=.\res\left.ico
# End Source File
# Begin Source File

SOURCE=.\res\list_arm.ico
# End Source File
# Begin Source File

SOURCE=.\res\list_wor.ico
# End Source File
# Begin Source File

SOURCE=.\res\locking.gif
# End Source File
# Begin Source File

SOURCE=.\res\maintena.ico
# End Source File
# Begin Source File

SOURCE=.\res\matrixsetblue.bmp
# End Source File
# Begin Source File

SOURCE=.\res\matrixsetsmallblue.bmp
# End Source File
# Begin Source File

SOURCE=.\res\matrixsettinyblue.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Menu.gif
# End Source File
# Begin Source File

SOURCE=.\res\module.ico
# End Source File
# Begin Source File

SOURCE=.\res\mot_pos.ico
# End Source File
# Begin Source File

SOURCE=.\res\mot_spd.ico
# End Source File
# Begin Source File

SOURCE=.\res\motor.ico
# End Source File
# Begin Source File

SOURCE=.\res\MSG.gif
# End Source File
# Begin Source File

SOURCE=.\res\n_dn.ico
# End Source File
# Begin Source File

SOURCE=.\res\n_left.ico
# End Source File
# Begin Source File

SOURCE=.\res\n_right.ico
# End Source File
# Begin Source File

SOURCE=.\res\n_up.ico
# End Source File
# Begin Source File

SOURCE=.\res\open_fol.ico
# End Source File
# Begin Source File

SOURCE=.\res\redButton.bmp
# End Source File
# Begin Source File

SOURCE=.\res\right.ico
# End Source File
# Begin Source File

SOURCE=".\res\Sound Off.ico"
# End Source File
# Begin Source File

SOURCE=".\res\Sound On.ico"
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ToolbarDraw.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ToolbarDrawDisabled.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ToolbarDrawHot.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tray.ico
# End Source File
# Begin Source File

SOURCE=.\res\up1.ico
# End Source File
# End Group
# Begin Group "UpdateList"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\IssueList.info
# End Source File
# Begin Source File

SOURCE=.\TodoList.info
# End Source File
# Begin Source File

SOURCE=.\UpdateList.info
# End Source File
# End Group
# Begin Group "Common"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\common\ShareMem.cpp
# End Source File
# Begin Source File

SOURCE=..\common\ShareMem.h
# End Source File
# Begin Source File

SOURCE=..\common\StdAfx.h
# End Source File
# End Group
# Begin Group "SrcBase"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\SrcBase\AMotor.cpp
# End Source File
# Begin Source File

SOURCE=.\SrcBase\AMotor.h
# End Source File
# End Group
# Begin Group "Other Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CommonFunc.cpp
# End Source File
# Begin Source File

SOURCE=.\CommonFunc.h
# End Source File
# Begin Source File

SOURCE=.\ExternFunctions.cpp
# End Source File
# Begin Source File

SOURCE=.\ExternFunctions.h
# End Source File
# Begin Source File

SOURCE=.\Public_Function.cpp
# End Source File
# Begin Source File

SOURCE=.\Public_Function.h
# End Source File
# Begin Source File

SOURCE=.\Variable.h
# End Source File
# End Group
# Begin Group "UI Controls"

# PROP Default_Filter ""
# Begin Group "Log Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\LogFile.cpp
# End Source File
# Begin Source File

SOURCE=.\LogFile.h
# End Source File
# Begin Source File

SOURCE=.\Logger.cpp
# End Source File
# Begin Source File

SOURCE=.\Logger.h
# End Source File
# End Group
# End Group
# Begin Group "Socket Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ClientSocket.cpp
# End Source File
# Begin Source File

SOURCE=.\ClientSocket.h
# End Source File
# Begin Source File

SOURCE=.\MainEvent.cpp
# End Source File
# Begin Source File

SOURCE=.\MainEvent.h
# End Source File
# Begin Source File

SOURCE=.\ServerItem.cpp
# End Source File
# Begin Source File

SOURCE=.\ServerItem.h
# End Source File
# Begin Source File

SOURCE=.\ServerSocket.cpp
# End Source File
# Begin Source File

SOURCE=.\ServerSocket.h
# End Source File
# End Group
# Begin Group "Sequence Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Seq_BoxClamp.cpp
# End Source File
# Begin Source File

SOURCE=.\Seq_BoxClamp.h
# End Source File
# Begin Source File

SOURCE=.\Seq_BoxLifter.cpp
# End Source File
# Begin Source File

SOURCE=.\Seq_BoxLifter.h
# End Source File
# Begin Source File

SOURCE=.\Seq_BufferAlignConv.cpp
# End Source File
# Begin Source File

SOURCE=.\Seq_BufferAlignConv.h
# End Source File
# Begin Source File

SOURCE=.\Seq_IO_ReadWrite.cpp
# End Source File
# Begin Source File

SOURCE=.\Seq_IO_ReadWrite.h
# End Source File
# Begin Source File

SOURCE=.\Seq_LampSwitch.cpp
# End Source File
# Begin Source File

SOURCE=.\Seq_LampSwitch.h
# End Source File
# Begin Source File

SOURCE=.\Seq_LoadingConv.cpp
# End Source File
# Begin Source File

SOURCE=.\Seq_LoadingConv.h
# End Source File
# Begin Source File

SOURCE=.\Seq_MotMove.cpp
# End Source File
# Begin Source File

SOURCE=.\Seq_MotMove.h
# End Source File
# Begin Source File

SOURCE=.\Seq_TowerLamp.cpp
# End Source File
# Begin Source File

SOURCE=.\Seq_TowerLamp.h
# End Source File
# Begin Source File

SOURCE=.\Seq_XYZRobot.cpp
# End Source File
# Begin Source File

SOURCE=.\Seq_XYZRobot.h
# End Source File
# Begin Source File

SOURCE=.\Thread.cpp
# End Source File
# Begin Source File

SOURCE=.\Thread.h
# End Source File
# End Group
# Begin Group "Serial Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\BcrEvent.cpp
# End Source File
# Begin Source File

SOURCE=.\BcrEvent.h
# End Source File
# Begin Source File

SOURCE=.\SerialCommBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SerialCommBase.h
# End Source File
# Begin Source File

SOURCE=.\SerialPort.cpp
# End Source File
# Begin Source File

SOURCE=.\SerialPort.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\app.manifest
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
# Section Handler : {9CB231B6-6648-4E5F-8A37-8DDCA03D732C}
# 	1:25:IDD_SCREEN_LIST_OPERATION:122
# 	2:16:Resource Include:resource.h
# 	2:25:Screen_List_Operation.cpp:Screen_List_Operation.cpp
# 	2:23:Screen_List_Operation.h:Screen_List_Operation.h
# 	2:25:IDD_SCREEN_LIST_OPERATION:IDD_SCREEN_LIST_OPERATION
# 	2:29:CLASS: CScreen_List_Operation:CScreen_List_Operation
# 	2:10:ENUM: enum:enum
# 	2:19:Application Include:handler.h
# End Section
# Section Handler : {27140B28-9FA2-48F7-BD00-630A91633EAA}
# 	1:17:IDD_SCREEN_BASIC1:117
# 	2:16:Resource Include:resource.h
# 	2:20:CLASS: CScreen_Basic:CScreen_Basic
# 	2:16:Screen_Basic.cpp:Screen_Basic1.cpp
# 	2:14:Screen_Basic.h:Screen_Basic1.h
# 	2:10:ENUM: enum:enum
# 	2:19:Application Include:handler.h
# 	2:16:IDD_SCREEN_BASIC:IDD_SCREEN_BASIC1
# End Section
# Section Handler : {2EAA00E8-BB79-47DA-9549-FACD4D55ED2D}
# 	1:24:IDD_XGEMINTERFACE_DIALOG:1864
# 	2:16:Resource Include:resource.h
# 	2:24:IDD_XGEMINTERFACE_DIALOG:IDD_XGEMINTERFACE_DIALOG
# 	2:22:Dialog_XgemInterface.h:Dialog_XgemInterface.h
# 	2:24:Dialog_XgemInterface.cpp:Dialog_XgemInterface.cpp
# 	2:10:ENUM: enum:enum
# 	2:28:CLASS: CDialog_XgemInterface:CDialog_XgemInterface
# 	2:19:Application Include:handler.h
# End Section
# Section Handler : {7ABB6075-8E6E-43D3-8F39-82CCEF73B332}
# 	1:19:IDD_SCREEN_PGM_INFO:145
# 	2:17:Screen_Pgm_Info.h:Screen_Pgm_Info.h
# 	2:16:Resource Include:resource.h
# 	2:19:Screen_Pgm_Info.cpp:Screen_Pgm_Info.cpp
# 	2:19:IDD_SCREEN_PGM_INFO:IDD_SCREEN_PGM_INFO
# 	2:10:ENUM: enum:enum
# 	2:19:Application Include:handler.h
# 	2:23:CLASS: CScreen_Pgm_Info:CScreen_Pgm_Info
# End Section
# Section Handler : {2FAED75F-EDBA-4BAC-A1F3-06761CF9DA1F}
# 	1:17:IDD_DIALOG_SELECT:112
# 	2:16:Resource Include:resource.h
# 	2:17:IDD_DIALOG_SELECT:IDD_DIALOG_SELECT
# 	2:10:ENUM: enum:enum
# 	2:19:Application Include:handler.h
# 	2:17:Dialog_Select.cpp:Dialog_Select.cpp
# 	2:15:Dialog_Select.h:Dialog_Select.h
# 	2:21:CLASS: CDialog_Select:CDialog_Select
# End Section
# Section Handler : {BF36ECE5-63A5-49D7-A0B6-D343689DEDD9}
# 	1:15:IDD_DIALOG_MENU:110
# 	2:16:Resource Include:resource.h
# 	2:13:Dialog_Menu.h:Dialog_Menu.h
# 	2:15:Dialog_Menu.cpp:Dialog_Menu.cpp
# 	2:10:ENUM: enum:enum
# 	2:15:IDD_DIALOG_MENU:IDD_DIALOG_MENU
# 	2:19:CLASS: CDialog_Menu:CDialog_Menu
# 	2:19:Application Include:Handler.h
# End Section
# Section Handler : {D1E47E53-FE8F-4AB0-82CC-28F2479393B8}
# 	1:16:IDD_MOTOR_AXIS_2:124
# 	2:16:Resource Include:resource.h
# 	2:27:CLASS: CDialog_Motor_Axis_2:CDialog_Motor_Axis_2
# 	2:23:Dialog_Motor_Axis_2.cpp:Dialog_Motor_Axis_2.cpp
# 	2:10:ENUM: enum:enum
# 	2:16:IDD_MOTOR_AXIS_2:IDD_MOTOR_AXIS_2
# 	2:21:Dialog_Motor_Axis_2.h:Dialog_Motor_Axis_2.h
# 	2:19:Application Include:handler.h
# End Section
# Section Handler : {30E78C27-1EBE-4A5D-B6C2-E519218D337E}
# 	1:17:IDD_MOTOR_AXIS_10:139
# 	2:16:Resource Include:resource.h
# 	2:17:IDD_MOTOR_AXIS_10:IDD_MOTOR_AXIS_10
# 	2:10:ENUM: enum:enum
# 	2:28:CLASS: CDialog_Motor_Axis_10:CDialog_Motor_Axis_10
# 	2:22:Dialog_Motor_Axis_10.h:Dialog_Motor_Axis_10.h
# 	2:19:Application Include:handler.h
# 	2:24:Dialog_Motor_Axis_10.cpp:Dialog_Motor_Axis_10.cpp
# End Section
# Section Handler : {A2E0A297-8B0B-4BC2-828E-F1BB188BBA45}
# 	1:16:IDD_MOTOR_AXIS_8:137
# 	2:16:Resource Include:resource.h
# 	2:21:Dialog_Motor_Axis_8.h:Dialog_Motor_Axis_8.h
# 	2:10:ENUM: enum:enum
# 	2:27:CLASS: CDialog_Motor_Axis_8:CDialog_Motor_Axis_8
# 	2:23:Dialog_Motor_Axis_8.cpp:Dialog_Motor_Axis_8.cpp
# 	2:19:Application Include:handler.h
# 	2:16:IDD_MOTOR_AXIS_8:IDD_MOTOR_AXIS_8
# End Section
# Section Handler : {84059BD8-F9F5-48D8-964E-ABED2F8C43D4}
# 	1:16:IDD_DIALOG_INFOR:106
# 	2:16:Resource Include:resource.h
# 	2:16:IDD_DIALOG_INFOR:IDD_DIALOG_INFOR
# 	2:20:CLASS: CDialog_Infor:CDialog_Infor
# 	2:14:Dialog_Infor.h:Dialog_Infor.h
# 	2:10:ENUM: enum:enum
# 	2:16:Dialog_Infor.cpp:Dialog_Infor.cpp
# 	2:19:Application Include:Handler.h
# End Section
# Section Handler : {82440828-E6E4-42C0-A992-C374F1A1BA11}
# 	1:21:IDD_DIALOG_PASS_CHECK:114
# 	2:16:Resource Include:resource.h
# 	2:21:Dialog_Pass_Check.cpp:Dialog_Pass_Check.cpp
# 	2:21:IDD_DIALOG_PASS_CHECK:IDD_DIALOG_PASS_CHECK
# 	2:19:Dialog_Pass_Check.h:Dialog_Pass_Check.h
# 	2:10:ENUM: enum:enum
# 	2:25:CLASS: CDialog_Pass_Check:CDialog_Pass_Check
# 	2:19:Application Include:handler.h
# End Section
# Section Handler : {5D1FD30B-A459-4C2B-AF6A-7DFD39FFADDB}
# 	1:16:IDD_MOTOR_AXIS_1:123
# 	2:16:Resource Include:resource.h
# 	2:27:CLASS: CDialog_Motor_Axis_1:CDialog_Motor_Axis_1
# 	2:23:Dialog_Motor_Axis_1.cpp:Dialog_Motor_Axis_1.cpp
# 	2:16:IDD_MOTOR_AXIS_1:IDD_MOTOR_AXIS_1
# 	2:10:ENUM: enum:enum
# 	2:21:Dialog_Motor_Axis_1.h:Dialog_Motor_Axis_1.h
# 	2:19:Application Include:handler.h
# End Section
# Section Handler : {6506618D-6B0F-4392-995D-E0CEE9C2FAC8}
# 	1:16:IDD_MOTOR_AXIS_3:125
# 	2:16:Resource Include:resource.h
# 	2:27:CLASS: CDialog_Motor_Axis_3:CDialog_Motor_Axis_3
# 	2:23:Dialog_Motor_Axis_3.cpp:Dialog_Motor_Axis_3.cpp
# 	2:10:ENUM: enum:enum
# 	2:16:IDD_MOTOR_AXIS_3:IDD_MOTOR_AXIS_3
# 	2:19:Application Include:handler.h
# 	2:21:Dialog_Motor_Axis_3.h:Dialog_Motor_Axis_3.h
# End Section
# Section Handler : {1A424EFC-3935-476A-BB74-25A5ABDDD317}
# 	1:16:IDD_MOTOR_AXIS_9:138
# 	2:16:Resource Include:resource.h
# 	2:21:Dialog_Motor_Axis_9.h:Dialog_Motor_Axis_9.h
# 	2:10:ENUM: enum:enum
# 	2:27:CLASS: CDialog_Motor_Axis_9:CDialog_Motor_Axis_9
# 	2:23:Dialog_Motor_Axis_9.cpp:Dialog_Motor_Axis_9.cpp
# 	2:19:Application Include:handler.h
# 	2:16:IDD_MOTOR_AXIS_9:IDD_MOTOR_AXIS_9
# End Section
# Section Handler : {208E09E9-EDF4-4648-9D6D-48395B1D9227}
# 	1:19:IDD_DIALOG_KEYBOARD:109
# 	2:16:Resource Include:resource.h
# 	2:10:ENUM: enum:enum
# 	2:19:Dialog_Keyboard.cpp:Dialog_Keyboard.cpp
# 	2:19:IDD_DIALOG_KEYBOARD:IDD_DIALOG_KEYBOARD
# 	2:17:Dialog_Keyboard.h:Dialog_Keyboard.h
# 	2:19:Application Include:Handler.h
# 	2:23:CLASS: CDialog_Keyboard:CDialog_Keyboard
# End Section
# Section Handler : {4985D71F-FA84-4146-A98C-D4EF0FF4358B}
# 	1:18:IDD_DIALOG_PART_IO:1880
# 	2:16:Resource Include:resource.h
# 	2:18:Dialog_Part_IO.cpp:Dialog_Part_IO.cpp
# 	2:16:Dialog_Part_IO.h:Dialog_Part_IO.h
# 	2:10:ENUM: enum:enum
# 	2:18:IDD_DIALOG_PART_IO:IDD_DIALOG_PART_IO
# 	2:22:CLASS: CDialog_Part_IO:CDialog_Part_IO
# 	2:19:Application Include:handler.h
# End Section
# Section Handler : {DAE8132B-625E-479F-AA56-53451D2E2D74}
# 	1:22:IDD_DIALOG_PASS_CHANGE:113
# 	2:16:Resource Include:resource.h
# 	2:22:Dialog_Pass_Change.cpp:Dialog_Pass_Change.cpp
# 	2:10:ENUM: enum:enum
# 	2:22:IDD_DIALOG_PASS_CHANGE:IDD_DIALOG_PASS_CHANGE
# 	2:26:CLASS: CDialog_Pass_Change:CDialog_Pass_Change
# 	2:19:Application Include:handler.h
# 	2:20:Dialog_Pass_Change.h:Dialog_Pass_Change.h
# End Section
# Section Handler : {2ACB1E55-95A5-43D8-AE8C-8CB5C0AB7085}
# 	2:29:CLASS: CDialog_Motor_Axis_Tab:CDialog_Motor_Axis_Tab
# 	2:25:Dialog_Motor_Axis_Tab.cpp:Dialog_Motor_Axis_Tab.cpp
# 	2:23:Dialog_Motor_Axis_Tab.h:Dialog_Motor_Axis_Tab.h
# 	2:19:Application Include:handler.h
# End Section
# Section Handler : {1E6C4F0E-DA1E-4D4A-ADBF-DA62D0777C82}
# 	1:18:IDD_DIALOG_MESSAGE:108
# 	2:16:Resource Include:resource.h
# 	2:22:CLASS: CDialog_Message:CDialog_Message
# 	2:18:IDD_DIALOG_MESSAGE:IDD_DIALOG_MESSAGE
# 	2:18:Dialog_Message.cpp:Dialog_Message.cpp
# 	2:16:Dialog_Message.h:Dialog_Message.h
# 	2:10:ENUM: enum:enum
# 	2:19:Application Include:Handler.h
# End Section
# Section Handler : {620151F6-3A4F-482D-87B7-FD82B9637942}
# 	1:19:IDD_SCREEN_INITIAL1:120
# 	2:16:Resource Include:resource.h
# 	2:22:CLASS: CScreen_Initial:CScreen_Initial
# 	2:18:IDD_SCREEN_INITIAL:IDD_SCREEN_INITIAL1
# 	2:10:ENUM: enum:enum
# 	2:18:Screen_Initial.cpp:Screen_Initial1.cpp
# 	2:19:Application Include:handler.h
# 	2:16:Screen_Initial.h:Screen_Initial1.h
# End Section
# Section Handler : {2220F20C-2FA3-461C-84D4-948B6D07E79B}
# 	2:21:DefaultSinkHeaderFile:exgem.h
# 	2:16:DefaultSinkClass:CeXGem
# End Section
# Section Handler : {AF5DA7F1-332C-4642-8C7B-30DAB3A68D5D}
# 	1:16:IDD_MOTOR_AXIS_5:127
# 	2:16:Resource Include:resource.h
# 	2:27:CLASS: CDialog_Motor_Axis_5:CDialog_Motor_Axis_5
# 	2:23:Dialog_Motor_Axis_5.cpp:Dialog_Motor_Axis_5.cpp
# 	2:10:ENUM: enum:enum
# 	2:16:IDD_MOTOR_AXIS_5:IDD_MOTOR_AXIS_5
# 	2:19:Application Include:handler.h
# 	2:21:Dialog_Motor_Axis_5.h:Dialog_Motor_Axis_5.h
# End Section
# Section Handler : {EFE5357C-0A0B-45D2-8277-D2581811E006}
# 	1:16:IDD_DIALOG_COLOR:105
# 	2:16:Resource Include:resource.h
# 	2:20:CLASS: CDialog_Color:CDialog_Color
# 	2:14:Dialog_Color.h:Dialog_Color.h
# 	2:10:ENUM: enum:enum
# 	2:16:Dialog_Color.cpp:Dialog_Color.cpp
# 	2:19:Application Include:Handler.h
# 	2:16:IDD_DIALOG_COLOR:IDD_DIALOG_COLOR
# End Section
# Section Handler : {600F1C05-63AA-448D-8DEC-B9E42D1AE9DD}
# 	1:16:IDD_DIALOG_COPY1:1879
# 	2:16:Resource Include:resource.h
# 	2:15:Dialog_Copy.cpp:Dialog_Copy1.cpp
# 	2:13:Dialog_Copy.h:Dialog_Copy1.h
# 	2:10:ENUM: enum:enum
# 	2:15:IDD_DIALOG_COPY:IDD_DIALOG_COPY1
# 	2:19:Application Include:handler.h
# 	2:19:CLASS: CDialog_Copy:CDialog_Copy
# End Section
# Section Handler : {1EEE6545-7252-4C6F-87FB-CC43BC6163E5}
# 	1:16:IDD_SCREEN_MOTOR:143
# 	2:16:Resource Include:resource.h
# 	2:16:IDD_SCREEN_MOTOR:IDD_SCREEN_MOTOR
# 	2:10:ENUM: enum:enum
# 	2:20:CLASS: CScreen_Motor:CScreen_Motor
# 	2:14:Screen_Motor.h:Screen_Motor.h
# 	2:16:Screen_Motor.cpp:Screen_Motor.cpp
# 	2:19:Application Include:handler.h
# End Section
# Section Handler : {D7A2FECE-F335-4DE1-AA39-B980EBF671E5}
# 	1:16:IDD_DIALOG_ALARM:115
# 	2:16:Resource Include:resource.h
# 	2:20:CLASS: CDialog_Alarm:CDialog_Alarm
# 	2:14:Dialog_Alarm.h:Dialog_Alarm.h
# 	2:10:ENUM: enum:enum
# 	2:19:Application Include:handler.h
# 	2:16:IDD_DIALOG_ALARM:IDD_DIALOG_ALARM
# 	2:16:Dialog_Alarm.cpp:Dialog_Alarm.cpp
# End Section
# Section Handler : {1AE3EBFC-A018-4617-87A1-1BB333106B17}
# 	1:16:IDD_DIALOG_ABOUT:102
# 	2:16:Resource Include:resource.h
# 	2:16:Dialog_About.cpp:Dialog_About.cpp
# 	2:10:ENUM: enum:enum
# 	2:16:IDD_DIALOG_ABOUT:IDD_DIALOG_ABOUT
# 	2:20:CLASS: CDialog_About:CDialog_About
# 	2:19:Application Include:Handler.h
# 	2:14:Dialog_About.h:Dialog_About.h
# End Section
# Section Handler : {AF2E90B9-B07E-4D1D-BB3A-483DCB941EA2}
# 	1:16:IDD_MOTOR_AXIS_4:126
# 	2:16:Resource Include:resource.h
# 	2:27:CLASS: CDialog_Motor_Axis_4:CDialog_Motor_Axis_4
# 	2:23:Dialog_Motor_Axis_4.cpp:Dialog_Motor_Axis_4.cpp
# 	2:10:ENUM: enum:enum
# 	2:16:IDD_MOTOR_AXIS_4:IDD_MOTOR_AXIS_4
# 	2:19:Application Include:handler.h
# 	2:21:Dialog_Motor_Axis_4.h:Dialog_Motor_Axis_4.h
# End Section
# Section Handler : {0F942E8E-D884-4FFB-A5F6-3AEAD331A542}
# 	1:15:IDD_DIALOG_EXIT:103
# 	2:16:Resource Include:resource.h
# 	2:15:IDD_DIALOG_EXIT:IDD_DIALOG_EXIT
# 	2:19:CLASS: CDialog_Exit:CDialog_Exit
# 	2:15:Dialog_Exit.cpp:Dialog_Exit.cpp
# 	2:10:ENUM: enum:enum
# 	2:19:Application Include:Handler.h
# 	2:13:Dialog_Exit.h:Dialog_Exit.h
# End Section
# Section Handler : {5CC31086-2C43-47F3-A210-E6162B593BC7}
# 	1:16:IDD_MOTOR_AXIS_7:135
# 	2:16:Resource Include:resource.h
# 	2:21:Dialog_Motor_Axis_7.h:Dialog_Motor_Axis_7.h
# 	2:27:CLASS: CDialog_Motor_Axis_7:CDialog_Motor_Axis_7
# 	2:10:ENUM: enum:enum
# 	2:23:Dialog_Motor_Axis_7.cpp:Dialog_Motor_Axis_7.cpp
# 	2:16:IDD_MOTOR_AXIS_7:IDD_MOTOR_AXIS_7
# 	2:19:Application Include:handler.h
# End Section
# Section Handler : {9927F408-6788-4B97-9EAD-900FB16629FF}
# 	1:14:IDD_MOTOR_PART:141
# 	2:16:Resource Include:resource.h
# 	2:21:Dialog_Motor_Part.cpp:Dialog_Motor_Part.cpp
# 	2:14:IDD_MOTOR_PART:IDD_MOTOR_PART
# 	2:25:CLASS: CDialog_Motor_Part:CDialog_Motor_Part
# 	2:10:ENUM: enum:enum
# 	2:19:Application Include:handler.h
# 	2:19:Dialog_Motor_Part.h:Dialog_Motor_Part.h
# End Section
# Section Handler : {6F88E29E-8982-49D6-9ACC-CBD4952D6D3B}
# 	2:5:Class:CeXGem
# 	2:10:HeaderFile:exgem.h
# 	2:8:ImplFile:exgem.cpp
# End Section
# Section Handler : {11BC8D80-C4EF-4E16-906F-D159962CA885}
# 	1:17:IDD_DIALOG_KEYPAD:107
# 	2:16:Resource Include:resource.h
# 	2:15:Dialog_KeyPad.h:Dialog_KeyPad.h
# 	2:17:Dialog_KeyPad.cpp:Dialog_KeyPad.cpp
# 	2:10:ENUM: enum:enum
# 	2:21:CLASS: CDialog_KeyPad:CDialog_KeyPad
# 	2:17:IDD_DIALOG_KEYPAD:IDD_DIALOG_KEYPAD
# 	2:19:Application Include:Handler.h
# End Section
# Section Handler : {9A622C2A-82CB-4C1C-AEA9-143A1FAE7C9F}
# 	1:25:IDD_SCREEN_LIST_DATA_VIEW:123
# 	2:16:Resource Include:resource.h
# 	2:25:Screen_List_Data_View.cpp:Screen_List_Data_View.cpp
# 	2:25:IDD_SCREEN_LIST_DATA_VIEW:IDD_SCREEN_LIST_DATA_VIEW
# 	2:10:ENUM: enum:enum
# 	2:29:CLASS: CScreen_List_Data_View:CScreen_List_Data_View
# 	2:19:Application Include:handler.h
# 	2:23:Screen_List_Data_View.h:Screen_List_Data_View.h
# End Section
# Section Handler : {751C431C-B285-424F-B021-42F5AE9B0FA2}
# 	1:15:IDD_SCREEN_MAIN:144
# 	2:16:Resource Include:resource.h
# 	2:15:Screen_Main.cpp:Screen_Main.cpp
# 	2:10:ENUM: enum:enum
# 	2:15:IDD_SCREEN_MAIN:IDD_SCREEN_MAIN
# 	2:19:CLASS: CScreen_Main:CScreen_Main
# 	2:19:Application Include:handler.h
# 	2:13:Screen_Main.h:Screen_Main.h
# End Section
# Section Handler : {1694ABA9-91FD-489C-85E9-6AE269E133E8}
# 	1:23:IDD_SCREEN_MOTOR_SPEED1:118
# 	2:16:Resource Include:resource.h
# 	2:22:IDD_SCREEN_MOTOR_SPEED:IDD_SCREEN_MOTOR_SPEED1
# 	2:10:ENUM: enum:enum
# 	2:19:Application Include:handler.h
# 	2:26:CLASS: CScreen_Motor_Speed:CScreen_Motor_Speed
# 	2:22:Screen_Motor_Speed.cpp:Screen_Motor_Speed1.cpp
# 	2:20:Screen_Motor_Speed.h:Screen_Motor_Speed1.h
# End Section
# Section Handler : {DF4E14F7-8883-4935-9A03-564D21335C90}
# 	1:16:IDD_MOTOR_AXIS_6:133
# 	2:16:Resource Include:resource.h
# 	2:21:Dialog_Motor_Axis_6.h:Dialog_Motor_Axis_6.h
# 	2:27:CLASS: CDialog_Motor_Axis_6:CDialog_Motor_Axis_6
# 	2:23:Dialog_Motor_Axis_6.cpp:Dialog_Motor_Axis_6.cpp
# 	2:10:ENUM: enum:enum
# 	2:16:IDD_MOTOR_AXIS_6:IDD_MOTOR_AXIS_6
# 	2:19:Application Include:handler.h
# End Section
# Section Handler : {2C587007-446D-4516-9904-664F320777B2}
# 	1:26:IDD_SCREEN_SET_MAINTENANCE:116
# 	2:16:Resource Include:resource.h
# 	2:26:IDD_SCREEN_SET_MAINTENANCE:IDD_SCREEN_SET_MAINTENANCE
# 	2:30:CLASS: CScreen_Set_Maintenance:CScreen_Set_Maintenance
# 	2:10:ENUM: enum:enum
# 	2:26:Screen_Set_Maintenance.cpp:Screen_Set_Maintenance.cpp
# 	2:24:Screen_Set_Maintenance.h:Screen_Set_Maintenance.h
# 	2:19:Application Include:handler.h
# End Section
# Section Handler : {03A754E5-82B4-4748-96C5-B6B0E0D34BB3}
# 	1:16:IDD_DIALOG_TITLE:1880
# 	2:16:Resource Include:resource.h
# 	2:16:IDD_DIALOG_TITLE:IDD_DIALOG_TITLE
# 	2:14:Dialog_Title.h:Dialog_Title.h
# 	2:20:CLASS: CDialog_Title:CDialog_Title
# 	2:10:ENUM: enum:enum
# 	2:16:Dialog_Title.cpp:Dialog_Title.cpp
# 	2:19:Application Include:handler.h
# End Section
# Section Handler : {048EE095-669E-46A7-80E6-FF8D771EFC15}
# 	1:20:IDD_SCREEN_DATA_VIEW:121
# 	2:16:Resource Include:resource.h
# 	2:24:CLASS: CScreen_Data_View:CScreen_Data_View
# 	2:10:ENUM: enum:enum
# 	2:20:IDD_SCREEN_DATA_VIEW:IDD_SCREEN_DATA_VIEW
# 	2:19:Application Include:handler.h
# 	2:20:Screen_Data_View.cpp:Screen_Data_View.cpp
# 	2:18:Screen_Data_View.h:Screen_Data_View.h
# End Section
# Section Handler : {8DC1B3E7-834E-4FBA-AFAC-F5F97C0F2C24}
# 	1:18:IDD_DIALOG_PART_IO:142
# 	2:16:Resource Include:resource.h
# 	2:18:Dialog_Part_IO.cpp:Dialog_Part_IO.cpp
# 	2:16:Dialog_Part_IO.h:Dialog_Part_IO.h
# 	2:10:ENUM: enum:enum
# 	2:18:IDD_DIALOG_PART_IO:IDD_DIALOG_PART_IO
# 	2:22:CLASS: CDialog_Part_IO:CDialog_Part_IO
# 	2:19:Application Include:handler.h
# End Section
# Section Handler : {AC4D5BA0-C396-48B0-BCA6-B9F265B4FE5A}
# 	1:22:IDD_SCREEN_MOTOR_LDULD:119
# 	2:16:Resource Include:resource.h
# 	2:22:Screen_Motor_LdUld.cpp:Screen_Motor_LdUld.cpp
# 	2:20:Screen_Motor_LdUld.h:Screen_Motor_LdUld.h
# 	2:22:IDD_SCREEN_MOTOR_LDULD:IDD_SCREEN_MOTOR_LDULD
# 	2:10:ENUM: enum:enum
# 	2:26:CLASS: CScreen_Motor_LdUld:CScreen_Motor_LdUld
# 	2:19:Application Include:handler.h
# End Section
# Section Handler : {987F7439-F23D-4176-82D2-3ADC8E437864}
# 	1:20:IDD_DIALOG_MULTI_MSG:111
# 	2:16:Resource Include:resource.h
# 	2:20:IDD_DIALOG_MULTI_MSG:IDD_DIALOG_MULTI_MSG
# 	2:20:Dialog_Multi_Msg.cpp:Dialog_Multi_Msg.cpp
# 	2:10:ENUM: enum:enum
# 	2:18:Dialog_Multi_Msg.h:Dialog_Multi_Msg.h
# 	2:19:Application Include:handler.h
# 	2:24:CLASS: CDialog_Multi_Msg:CDialog_Multi_Msg
# End Section
