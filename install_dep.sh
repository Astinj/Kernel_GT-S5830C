#!/bin/bash
xterm -title 'Install dependencies' -e '
sudo apt-get update && sudo apt-get install git gcc libncurses5-dev libstdc++6 automake

sudo apt-get clean
sudo apt-get autoremove

sudo ln -s /usr/lib/i386-linux-gnu/libX11.so.6 /usr/lib/i386-linux-gnu/libX11.so
echo "Select <No> in next screen!!!"; echo "hit <Enter> to continue"; read; sudo dpkg-reconfigure -plow dash

cd ~

if [ -f ~/platform_prebuilt/.git/config ]; then
	echo "Update Cross Compiler"
	git pull https://github.com/android/platform_prebuilt.git
	echo "Cross Compiler updated!!!"
else
	echo "Download Cross Compiler"
	git clone https://github.com/android/platform_prebuilt.git
	echo "Cross Compiler READY!!!"
fi
echo "Evertyng Installed!!!"
sleep 5'