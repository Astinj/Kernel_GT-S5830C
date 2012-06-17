#!/bin/sh
xterm -e '
cd ./ramdisk/ && rm ../boot.img-ramdisk.gz; sleep 5; find . | cpio -o -H newc | gzip > ../boot.img-ramdisk.gz'