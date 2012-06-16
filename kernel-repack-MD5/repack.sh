#!/bin/sh
xterm -e '
if [ -f zImage ] && [ boot.img-kernel ]; then
	rm boot.img-kernel; clear; mv zImage boot.img-kernel; clear
else
	mv zImage boot.img-kernel; clear
fi
cd mkbootimg
if [ -f ../boot.img-ramdisk.gz ] && [ -f ../boot.img-kernel ]; then
	md5sum ../boot.img-kernel > md5
echo "./mkbootimg --kernel ../boot.img-kernel --kernelMD5 " > run.sh
dd if=md5 of=run.sh bs=1 seek=52 count=32
echo " --base 0x81600000 --ramdisk ../boot.img-ramdisk.gz --pagesize 4096 --ramdiskaddr 0x82600000 -o ../boot.img" >> run.sh
else
	echo "cant read files"; sleep 5; exit
fi
rm ../boot.img; clear; sleep 5
chmod +x run.sh mkbootimg && ./run.sh && rm run.sh md5
echo "boot.img READY"
echo "hit <Enter> to close!"; read'