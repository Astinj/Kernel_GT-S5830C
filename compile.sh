#!/bin/bash
xterm -title 'Configure to Compile Modules and Kernel' -e '
rm -Rf ./Kernel_OutPut ./Modules_OutPut; mkdir logs; clear
mkdir ./Kernel_OutPut
mkdir -p ./Modules_OutPut/system/lib/modules

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
	make distclean && make bcm21553_cooperve_defconfig && make silentoldconfig
fi
	make xconfig && make -j3 modules CONFIG_DEBUG_SECTION_MISMATCH=y 2>&1 | tee ../logs/$(date +%Y%m%d-%H%M)-make-modules.log
	find . ../modules -name '*.ko' -exec cp -v {} ../Modules_OutPut/system/lib/modules \;
	echo "Modules Compiled and stored in folder ./Modules_OutPut"; echo "Hit <Enter> to compile Kernel"; read
	make clean && make -j3 zImage CONFIG_DEBUG_SECTION_MISMATCH=y 2>&1 | tee ../logs/$(date +%Y%m%d-%H%M)-make-kernel.log

cd ..

cp ./common/arch/arm/boot/zImage ./Kernel_OutPut/

if [ -f ./Kernel_OutPut/zImage ]; then
	echo "Kernel Compiled and stored in folder ./Kernel_OutPut"
else
	echo "Compile Fail"
fi
echo "Hit <Enter> to continue!!!"; read'