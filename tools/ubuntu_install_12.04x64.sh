#!/bin/bash
# ���Դ
#sudo vi /etc/apt/sources.list
#deb http://kr.archive.ubuntu.com/ubuntu hardy main universe
# ��ӷǹٷ�Դ
#sudo add-apt-repository ppa:tualatrix/ppa
#sudo add-apt-repository ppa:ubuntu-toolchain-r/test2
# ����Դ
sudo apt-get -y update
#��װ32bit���ݿ�
sudo apt-get install ia32-libs
#������������� 
sudo apt-get -y install apt-file
sudo apt-file update

#��װϵͳ��Ҫ��
sudo apt-get -y install fakeroot bison flex libjpeg8 libjpeg8-dev 
sudo apt-get -y install libreadline6 libreadline6-dev mtdev-tools mtd-utils libfreetype6 libfreetype6-dev libcrypto++
sudo apt-get -y install build-essential pkg-config asciidoc docbook2x ssh openssl curl expat tk libcurl4-openssl-dev
sudo apt-get -y install libgtk2.0-0 libgtk2.0-dev libglib2.0-dev libglib2.0-doc libgtk2.0-doc

# ��װautomake1.15
tar zxvf /work/tools/automake-1.15.tar.gz
cd automake-1.15
./configure
make
sudo make install
cd -
rm -rf automake-1.15

# ��װautoconf2.69
tar zxvf /work/tools/autoconf-2.69.tar.gz 
cd autoconf-2.69
./configure
make
sudo make install
cd -
rm -rf autoconf-2.69

# ��װgettext-0.19.3
tar zxvf /work/tools/gettext-0.19.3.tar.gz
cd gettext-0.19.3
./configure
make
sudo make install
cd -
rm -rf gettext-0.19.3

# ��װtexinfo-6.0.tar.gz
tar zxvf /work/tools/texinfo-6.0.tar.gz
cd texinfo-6.0
./configure
make
sudo make install
cd -
rm -rf texinfo-6.0.tar.gz

# ��װlibtool-2.4.4
tar zxvf /work/tools/libtool-2.4.4.tar.gz
cd libtool-2.4.4
./configure
make
sudo make install
cd -
rm -rf libtool-2.4.4

# ��װpkg-config-0.23
tar zxvf /work/tools/pkg-config-0.23.tar.gz
cd pkg-config-0.23
./configure
make
sudo make install
cd -
rm -rf pkg-config-0.23

# ��װnfs
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

# ��װlibiconv
sudo apt-get -y install rpm

# ��װcramfs tool
sudo apt-get -y install cramfsprogs

# ��װopenGL��
sudo apt-get -y install mesa-common-dev mesademos libgl1-mesa-dev libglu1-mesa-dev freeglut3-dev

# ��װTK1��Ҫ�Ĺ���
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



# ��ֹupdatedb.mlocate�Է�ֹӲ�̹��ȶ�ȡ
sudo killall updatedb.mlocate
