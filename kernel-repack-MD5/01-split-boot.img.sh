#!/bin/sh
xterm -title 'SPLIT boot.img to RAMDISK and KERNEL' -e '
if [ -f ./boot.img ]; then
	echo "now split boot.img"; sleep 5
else
	echo "boot.img not exist, now use ori-boot.img"; sleep 5
	cp ori-boot.img boot.img
fi
cd mkbootimg
if [ -f ../boot.img ]; then
	rm ../boot.img-ramdisk.gz ../boot.img-kernel; clear; sleep 5
else
	echo "cant read file"; sleep 5; exit
fi
perl split_bootimg.pl ../boot.img
mv boot.img-kernel boot.img-ramdisk.gz ../
echo "hit <Enter> to continue"; read'