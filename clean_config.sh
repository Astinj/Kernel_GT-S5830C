#!/bin/sh
xterm -title 'CLEAN RESIDUALS AND CONFIGURATIONS' -e '
rm -Rf ./Kernel_OutPut/ ./Modules_OutPut/; clear
cd common
make distclean; echo "compilations and configurations clear!!!"; sleep 5'