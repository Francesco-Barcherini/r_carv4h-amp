@ECHO OFF
REM --------------------------------------------------------------------------------
REM @Title: Batch file to start TRACE32 with AMP mode
REM @Description:
REM   Windows batch file for a one-click start of the amp demo. It assumes
REM   TRACE32 is installed to C:\T32, modify it to fit your needs.
REM   Other core(s) will be started by the t32_amp.cmm practice script.
REM   Please specify pathes without trailing (back-)slash!
REM @Props: NoWelcome NoMetaTags Template
REM @Copyright: (C) 1989-2022 Lauterbach GmbH, licensed for use with TRACE32(R) only
REM --------------------------------------------------------------------------------
REM $Id: amp_demo_start_session0.bat 10545 2017-03-09 10:34:46Z amerkle $


SET P1_PORT=10000
SET P2_TITLE=ARM-Session-0
SET P3_TMP=%TMP%
SET P4_SYS=C:\t32
IF "%1"=="" (
  REM Parameters for USB connection
  SET P5_PBI=USB
  SET P6_OPT=CORE=1
  SET P7_OPT=
  SET P8_OPT=
) ELSE (
  REM Parameters for Ethernet connection
  SET P5_PBI=NET
  SET P6_OPT=NODE=%1
  SET P7_OPT=PACKLEN=1024
  SET P8_OPT=CORE=1
)

REM Switch to current DRIVE + into DRIVE&PATH
%~d0
CD %~dp0

REM Try to find t32marm executable in P4_SYS
SET T32MARCH_EXE=%P4_SYS%\bin\windows64\t32marm.exe
IF NOT EXIST %T32MARCH_EXE% (
  SET T32MARCH_EXE=%P4_SYS%\bin\windows\t32marm.exe
)
REM If P4_SYS is not valid, assume the demo is located in the demo directory
IF NOT EXIST %T32MARCH_EXE% (
  ECHO "Fallback mode: P4_SYS not valid!"
  ECHO "Fallback mode: Try to use relative path within TRACE32 installation."
  SET T32MARCH_EXE=%~dp0..\..\..\..\..\bin\windows64\t32marm.exe
  SET P4_SYS=%~dp0..\..\..\..\..
)
IF NOT EXIST %T32MARCH_EXE% (
  SET T32MARCH_EXE=%~dp0..\..\..\..\..\bin\windows\t32marm.exe
)
IF NOT EXIST %T32MARCH_EXE% (
  ECHO "t32marm.exe not found."
  ECHO "Please check that P4_SYS is set to your TRACE32 system directory."
  ECHO "(Current value: %P4_SYS%)"
  PAUSE
)

REM Last but not least start TRACE32
REM Common Syntax:
REM t32m<arch> -c <ConfigFile>.t32 [<param> [<param>] [...]] [-s <StartupScript> [<param> [...]]]
start %T32MARCH_EXE% -c config_amp.t32 %P1_PORT% %P2_TITLE% %P3_TMP% %P4_SYS% %P5_PBI% %P6_OPT% %P7_OPT% %P8_OPT% -s t32_amp.cmm
