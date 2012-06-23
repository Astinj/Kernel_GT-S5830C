#!/bin/sh
xterm -title 'Install dependencies' -e '
sudo apt-get update && sudo apt-get upgrade && sudo apt-get install git qt3-dev-tools g++ ia32-libs && sudo apt-get clean && sudo apt-get autoremove

echo "Select <No> in next screen!!!"
echo "hit <Enter> to continue"; read
sudo dpkg-reconfigure -plow dash

echo "Download or Up-date Cross Compiler!!!"

if [ -f platform_prebuilt/.git/config ]; then
	git pull https://github.com/android/platform_prebuilt.git
else
	git clone https://github.com/android/platform_prebuilt.git
fi

echo "Evertyng Installed!!!"
sleep 5'