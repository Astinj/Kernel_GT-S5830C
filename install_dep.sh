#!/bin/sh
xterm -e '
sudo apt-get update && sudo apt-get install git-core gnupg flex bison python rar original-awk gawk p7zip-full gperf libsdl1.2-dev libesd0-dev libwxgtk2.6-dev squashfs-tools build-essential zip curl libncurses5-dev zlib1g-dev pngcrush schedtool libc6-dev x11proto-core-dev libx11-dev libgl1-mesa-dev mingw32 tofrodos python-markdown libxml2-utils openjdk-6-jre openjdk-6-jdk gcc libstdc++6

if [ -f ./platform_prebuilt/Android.mk ]; then
	git pull https://github.com/android/platform_prebuilt.git
else
	rm -Rf platform_prebuilt
	git clone https://github.com/android/platform_prebuilt.git
fi

sudo ln -s /usr/lib/i386-linux-gnu/libX11.so.6 /usr/lib/i386-linux-gnu/libX11.so

echo "Select <No> in next screen!!!"; echo "hit enter to continue"; read; sudo dpkg-reconfigure -plow dash'