#!/bin/sh
xterm -e '
if [ -f /usr/bin/abootimg ]; then
	rm boot.img PDA-kernel.tar; clear
else
	sudo apt-get install abootimg
fi
echo "put initrd.img and zImage in"; pwd; read
if [ -f initrd.img ] && [ -f zImage ]; then
	abootimg --create boot.img -f bootimg.cfg -k zImage -r initrd.img && tar -cf PDA-kernel.tar boot.img
else
	echo "Warong files"; sleep 5; exit
fi
echo "boot.img and PDA-kernel.tar READY"; sleep 5'