#!/bin/sh
xterm -e '
cd mkbootimg
if [ -f ../boot.img ]; then
	rm ../boot.img-ramdisk.gz ../boot.img-kernel; clear; sleep 5
else
	echo "cant read file"; sleep 5; exit
fi
perl split_bootimg.pl ../boot.img; echo "hit <Enter> to continue"; read
mv boot.img-kernel boot.img-ramdisk.gz ../'