#!/bin/sh
xterm -e '
rm ./Kernel_OutPut/* ./Modules_OutPut/system/lib/modules/*; clear
cd common
make distclean; echo "compilations and configurations clear!!!"; sleep 5'