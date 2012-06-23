#!/bin/sh
xterm -title 'Install dependencies' -e '
sudo apt-get update && sudo apt-get upgrade && sudo apt-get install gnupg flex bison \
gperf libsdl-dev libesd0-dev libwxgtk2.6-dev build-essential zip curl libncurses5-dev \
zlib1g-dev valgrind qt3-dev-tools ia32-libs && sudo apt-get clean && sudo apt-get autoremove;

if [ -f platform_prebuilt/.git/config ]; then
	git pull https://github.com/android/platform_prebuilt.git
else
	git clone https://github.com/android/platform_prebuilt.git
fi

echo "Select <No> in next screen!!!"
echo "hit <Enter> to continue"; read
sudo dpkg-reconfigure -plow dash

echo "Evertyng Installed!!!"
sleep 5'