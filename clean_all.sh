#!/bin/bash
xterm -title 'Clean all' -e '

rm -Rf ./Kernel_OutPut/ ./Modules_OutPut/ ./logs/ ./kernel-repack-MD5/boot.im* ./kernel-repack-MD5/ramdisk/; clear

#Android Toolchain PATH
export ARCH=arm
export SUBARCH=arm
export CCOMPILE=$CROSS_COMPILE
export CROSS_COMPILE=arm-eabi-
export PATH=$PATH:$PWD/platform_prebuilt/linux-x86/toolchain/arm-eabi-4.4.3/bin

cd common
make distclean; echo "compilations and configurations clear!!!"; sleep 5'