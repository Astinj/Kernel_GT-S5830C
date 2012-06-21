#!/bin/bash
xterm -title 'Install dependencies' -e '
sudo apt-get update && sudo apt-get upgrade && sudo apt-get install libncurses5-dev ia32-libs gcc-arm-linux-gnueabi
sudo apt-get clean
sudo apt-get autoremove

echo "Select <No> in next screen!!!"
echo "hit <Enter> to continue"; read
sudo dpkg-reconfigure -plow dash

echo "Evertyng Installed!!!"
sleep 5'