#!/bin/bash
# 添加源
#sudo vi /etc/apt/sources.list
#deb http://kr.archive.ubuntu.com/ubuntu hardy main universe
# 添加非官方源
#sudo add-apt-repository ppa:tualatrix/ppa
#sudo add-apt-repository ppa:ubuntu-toolchain-r/test2
# 更新源
sudo apt-get -y update
#安装32bit兼容库
sudo apt-get install ia32-libs
#解决依赖的利器 
sudo apt-get -y install apt-file
sudo apt-file update

#安装系统必要库
sudo apt-get -y install fakeroot bison flex libjpeg8 libjpeg8-dev 
sudo apt-get -y install libreadline6 libreadline6-dev mtdev-tools mtd-utils libfreetype6 libfreetype6-dev libcrypto++
sudo apt-get -y install build-essential pkg-config asciidoc docbook2x ssh openssl curl expat tk libcurl4-openssl-dev
sudo apt-get -y install libgtk2.0-0 libgtk2.0-dev libglib2.0-dev libglib2.0-doc libgtk2.0-doc

# 安装automake1.15
tar zxvf /work/tools/automake-1.15.tar.gz
cd automake-1.15
./configure
make
sudo make install
cd -
rm -rf automake-1.15

# 安装autoconf2.69
tar zxvf /work/tools/autoconf-2.69.tar.gz 
cd autoconf-2.69
./configure
make
sudo make install
cd -
rm -rf autoconf-2.69

# 安装gettext-0.19.3
tar zxvf /work/tools/gettext-0.19.3.tar.gz
cd gettext-0.19.3
./configure
make
sudo make install
cd -
rm -rf gettext-0.19.3

# 安装texinfo-6.0.tar.gz
tar zxvf /work/tools/texinfo-6.0.tar.gz
cd texinfo-6.0
./configure
make
sudo make install
cd -
rm -rf texinfo-6.0.tar.gz

# 安装libtool-2.4.4
tar zxvf /work/tools/libtool-2.4.4.tar.gz
cd libtool-2.4.4
./configure
make
sudo make install
cd -
rm -rf libtool-2.4.4

# 安装pkg-config-0.23
tar zxvf /work/tools/pkg-config-0.23.tar.gz
cd pkg-config-0.23
./configure
make
sudo make install
cd -
rm -rf pkg-config-0.23

# 安装nfs
sudo apt-get -y install nfs-kernel-server
sudo apt-get -y install nfs-common
cd /
sudo ln -sf /bakup/nfsroot nfsroot
cd -
sudo apt-get -y install chkconfig

sudo apt-get -y install openssh-server
sudo apt-get -y install libncurses5-dev
sudo apt-get -y install tofrodos
sudo apt-get -y install patch xutils-dev
#sudo apt-get -y install docky

# 安装libiconv
sudo apt-get -y install rpm

# 安装cramfs tool
sudo apt-get -y install cramfsprogs

# 安装openGL库
sudo apt-get -y install mesa-common-dev mesademos libgl1-mesa-dev libglu1-mesa-dev freeglut3-dev

# 安装TK1需要的工具
sudo apt-get -y install libudev-dev
tar jxvf /work/tools/libusb-1.0.20.tar.bz2
cd libusb-1.0.20
./configure
make
sudo make install
cd -
rm -rf libusb-1.0.20

tar zxvf /work/tools/dtc-1.4.1.tar.gz
cd dtc-1.4.1
make
sudo make install PREFIX=/usr/local
cd -
rm -rf dtc-1.4.1



# 禁止updatedb.mlocate以防止硬盘过度读取
sudo killall updatedb.mlocate
