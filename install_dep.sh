#!/bin/bash
xterm -title 'Install dependencies' -e '
sudo apt-get update && sudo apt-get upgrade && sudo apt-get install gcc-arm-linux-gnueabi
sudo apt-get clean
sudo apt-get autoremove

sudo ln -s /usr/lib/i386-linux-gnu/libX11.so.6 /usr/lib/i386-linux-gnu/libX11.so
echo "Select <No> in next screen!!!"
echo "hit <Enter> to continue"; read
sudo dpkg-reconfigure -plow dash
'