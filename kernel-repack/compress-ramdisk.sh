#!/bin/sh
xterm -title 'COMPRESS RAMDISK' -e '
cd ./ramdisk/ && rm ../initrd.img; clear; sleep 5; find . | cpio -o -H newc | gzip > ../initrd.img'