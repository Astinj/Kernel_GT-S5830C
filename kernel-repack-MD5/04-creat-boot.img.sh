#!/bin/sh
xterm -title 'MERGE RANDISK AND zImage TO boot.img' -e '
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
echo " --ramdisk ../boot.img-ramdisk.gz --base 0x81600000 -o ../boot.img" >> run.sh
else
	echo "cant read files"; sleep 5; exit
fi
rm ../boot.img ../PDA-kernel.tar; clear; sleep 5
chmod +x run.sh mkbootimg && ./run.sh && rm run.sh md5
cd ..
tar -cvf PDA-kernel.tar boot.img
echo "boot.img READY"
echo "hit <Enter> to close!"; read'