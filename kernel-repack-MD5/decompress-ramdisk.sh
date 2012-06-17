#!/bin/sh
xterm -title 'DECOMPRESS RAMDISK' -e '
if [ -f ./ramdisk/default.prop ] || [ -f ./ramdisk/init* ]; then
	echo "please dele directory ./ramdisk/"; sleep 5; exit
else
	rm -Rf ./ramdisk/; clear; sleep 5
	mkdir ramdisk && cd ramdisk && gzip -dc ../boot.img-ramdisk.gz | cpio -i; cd ..
fi
cd ./ramdisk && strings fsr.ko | grep vermagic; read'