#!/bin/sh
xterm -e '
rm ./Kernel_OutPut/* ./Modules_OutPut/system/lib/modules/*; clear

#Android Toolchain PATH
export ARCH=arm
export CCOMPILE=$CROSS_COMPILE
export CROSS_COMPILE=arm-eabi-
export PATH=$PATH:$PWD/platform_prebuilt/linux-x86/toolchain/arm-eabi-4.4.3/bin/

cd common

if [ -f .config ]; then
	make clean
else
	make mrproper && make bcm21553_cooperve_defconfig
fi
	make menuconfig && make CONFIG_DEBUG_SECTION_MISMATCH=y

cd ..

find . -name '*.ko' -exec cp -v {} ./Modules_OutPut/system/lib/modules \;

cp ./common/arch/arm/boot/zImage ./Kernel_OutPut/

if [ -f ./Kernel_OutPut/zImage ]; then
	echo "Kernel and Modules compiled"; read
else
	echo "Compile Fail"; read; exit
fi
echo "zImage is inside ./Kernel_OutPut and modules in ./Modules_OutPut"
echo "hit <Enter> to continue" read'
