#!/bin/bash

cd /home/lihb13/android_workspace/jd20_workspace/LINUX/android;
source ./build/envsetup.sh
lunch < ./build/product
make -j40
echo "LHB runing" > lhb.log

cd /home/lihb13/android_workspace/zippo_workspace/LINUX/android
source ./build/envsetup.sh
lunch < ./build/product
make -j40
echo "LHB runing" > lhb.log
