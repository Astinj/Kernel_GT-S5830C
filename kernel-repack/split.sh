#!/bin/sh
xterm -e '
if [ -f /usr/bin/abootimg ]; then
	rm initrd.img zImage bootimg.cfg; clear
else
	sudo apt-get install abootimg
fi
echo "put boot.img file in"; pwd; read
if [ -f boot.img ]; then
abootimg -i boot.img; echo "hit <Enter> to continue!!!"; read; abootimg -x boot.img && rm boot.img; clear
else
	echo "Warong boot.img"; sleep 5; exit
fi
echo "boot.img unpacked"; sleep 5'