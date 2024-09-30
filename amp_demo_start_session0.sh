#!/bin/bash
# --------------------------------------------------------------------------------
# @Title: Bash script to start TRACE32 with AMP mode
# @Description:
#   Linux/MacOS bash script for a one-click start of the amp demo. It assumes
#   TRACE32 is installed to /opt/t32, modify it to fit your needs.
#   Other core(s) will be started by the t32_amp.cmm practice script.
# @Props: NoWelcome NoMetaTags Template
# @Copyright: (C) 1989-2022 Lauterbach GmbH, licensed for use with TRACE32(R) only
# --------------------------------------------------------------------------------
# $Id: amp_demo_start_session0.sh 11356 2017-09-13 14:28:33Z amerkle $


# ppwd will be the absolute path to the current bash script
export ppwd=$( cd $(dirname "$0") && pwd)

P1_PORT=10000
P2_TITLE=ARM-Session-0
P3_TMP=/tmp
P4_SYS=/opt/T32
if [ "${1}"=="" ]; then
  P5_PBI=USB
  P6_OPT=CORE=1
  P7_OPT=
  P8_OPT=
else
  P5_PBI=NET
  P6_OPT=NODE=%1
  P7_OPT=PACKLEN=1024
  P8_OPT=CORE=1
fi

# Detect the system is 32bit or 64bit
MACHINE='pc_linux'
MACHINE_TYPE=`uname -m`
if [ $MACHINE_TYPE == 'x86_64' ]; then
  MACHINE='pc_linux64'
fi

# Try to find t32marm executable
T32MARCH_EXE=${P4_SYS}/bin/${MACHINE}/t32marm-qt
if [ ! -x ${T32MARCH_EXE} ]; then
  echo "Fallback mode: P4_SYS not valid!"
  echo "Fallback mode: Try to use relative path within TRACE32 installation."
  T32MARCH_EXE=${ppwd}/../../../../../bin/${MACHINE}/t32marm-qt
  P4_SYS=${ppwd}/../../../../..
fi

if [ ! -x ${T32MARCH_EXE} ]; then
  echo "t32marm-qt not found."
  echo "Please check that P4_SYS is set to your TRACE32 system directory."
  echo "(Current value: ${P4_SYS})"
  pause
  exit -1
else
  # set T32SYS environment variable
  export T32SYS=${P4_SYS}
fi

cd ${ppwd}
# Last but not least start TRACE32
# Common Syntax:
# t32m<arch> -c <ConfigFile>.t32 [<param> [<param>] [...]] [-s <StartupScript> [<param> [...]]]
${T32SYS}/bin/${MACHINE}/t32marm-qt -c "${ppwd}/config_amp.t32" ${P1_PORT} ${P2_TITLE} ${P3_TMP} ${P4_SYS} ${P5_PBI} ${P6_OPT} ${P7_OPT} ${P8_OPT} -s "${ppwd}/"t32_amp.cmm
