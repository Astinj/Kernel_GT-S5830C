#!/bin/sh
xterm -title 'INSTALL DEPENDENCIES' -e '
sudo apt-get update && sudo apt-get upgrade && sudo apt-get install git git-core gnupg flex bison python original-awk gawk zip rar tar gzip gperf libsdl1.2-dev libesd0-dev libwxgtk2.6-dev squashfs-tools build-essential curl libncurses5-dev zlib1g-dev pngcrush schedtool libc6-dev x11proto-core-dev libx11-dev libgl1-mesa-dev mingw32 tofrodos python-markdown libxml2-utils openjdk-6-jre openjdk-6-jdk libstdc++6 wget texi2html texinfo automake libtool cvs gcc-4.4 g++-4.4 g++-4.4-multilib gcc-4.4-multilib

sudo apt-get clean
sudo apt-get autoremove

sudo ln -s /usr/lib/i386-linux-gnu/libX11.so.6 /usr/lib/i386-linux-gnu/libX11.so
echo "Select <No> in next screen!!!"; echo "hit <Enter> to continue"; read; sudo dpkg-reconfigure -plow dash

#if [ -f arm-2012.03-56-arm-none-eabi.bin ]; then
#	chmod +x arm-2012.03-56-arm-none-eabi.bin && ./arm-2012.03-56-arm-none-eabi.bin
#else
#wget https://sourcery.mentor.com/GNUToolchain/package10387/public/arm-none-eabi/arm-2012.03-56-arm-none-eabi.bin && chmod +x arm-2012.03-56-arm-none-eabi.bin && ./arm-2012.03-56-arm-none-eabi.bin
#fi

if [ -f ./platform_prebuilt/Android.mk ]; then
	git pull https://github.com/android/platform_prebuilt.git
else
	rm -Rf platform_prebuilt
	git clone https://github.com/android/platform_prebuilt.git
fi

'