#!/bin/bash
xterm -title 'Compile' -e '
rm -Rf ./Kernel_OutPut ./Modules_OutPut; clear
mkdir ./Kernel_OutPut && mkdir -p ./Modules_OutPut/ramdisk && mkdir -p ./Modules_OutPut/system/lib/modules

#Android Toolchain PATH
export ARCH=arm
export SUBARCH=arm
export CCOMPILE=$CROSS_COMPILE
export CROSS_COMPILE=arm-eabi-
export PATH=$PATH:$PWD/platform_prebuilt/linux-x86/toolchain/arm-eabi-4.4.3/bin

cd common

if [ -f .config ]; then
	make clean
else
	make distclean && make bcm21553_cooperve_defconfig
fi
	make xconfig && make -j3 modules && find . ../modules -name '*.ko' -exec cp -v {} ../Modules_OutPut/system/lib/modules \;
	echo "Modules Compiled and stored in folder Modules_OutPut"; echo "Hit <Enter> to compile Kernel"; read
	make clean && make -j3 zImage && find . ../modules -name '*.ko' -exec cp -v {} ../Modules_OutPut/ramdisk \;

cd ..

cp ./common/arch/arm/boot/zImage ./Kernel_OutPut/

if [ -f ./Kernel_OutPut/zImage ]; then
	echo "Kernel and Modules compiled"; read
else
	echo "Compile Fail"; read; exit
fi
echo "zImage is inside ./Kernel_OutPut"
echo "hit <Enter> to continue" read'