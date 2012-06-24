#!/bin/sh
xterm -title 'COMPRESS RAMDISK' -e '
cd ./ramdisk/ && rm ../boot.img-ramdisk.gz; clear; sleep 5; find . | cpio -o -H newc | lzma > ../boot.img-ramdisk.gz'