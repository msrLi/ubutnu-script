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

# 安装vsftpd
sudo apt-get -y install vsftpd
sudo sed -in-place -e 's/#write_enable=YES/write_enable=YES/' /etc/vsftpd.conf 
# 安装tftpd
sudo apt-get -y install tftpd tftp xinetd
#sudo apt-get -y install tftp-hpa tftpd-hpa openbsd-inetd
#sudo /etc/init.d/tftpd-hpa restart
# 安装Samba
sudo apt-get -y install samba
sudo apt-get -y install smbfs

# 安装zlib
sudo apt-get -y install libacl1-dev zlib1g-dev liblzo2-dev
sudo apt-get -y install xz-utils uuid-dev

# 安装libiconv
sudo apt-get -y install rpm

# 安装mksquashfs
sudo apt-get -y install squashfs-tools 

# 安装cramfs tool
sudo apt-get -y install cramfsprogs

# 安装netperf
sudo apt-get -y install netperf iperf

# 安装openGL库
sudo apt-get -y install mesa-common-dev mesademos libgl1-mesa-dev libglu1-mesa-dev freeglut3-dev

# 安装vim编辑器
sudo apt-get -y install vim exuberant-ctags cscope global 

# 安装版本管理软件
sudo apt-get -y install cvs tree
sudo apt-get -y install subversion subversion-tools
tar zxvf /work/tools/git-1.8.3.1.tar.gz
cd git-1.8.3.1
make prefix=/usr/local all; sudo make prefix=/usr/local install
cd -
rm -rf git-1.8.3.1

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

# 安装压缩库
# zlib不能更新，否则UI启动不起来
#tar jxvf /work/tools/zlib-1.2.5.tar.bz2
#cd zlib-1.2.5
#./configure
#make
#sudo make install
#cd -
#rm -rf zlib-1.2.5

#tar jxvf /work/tools/lzma-4.32.7.tar.bz2
#cd lzma-4.32.7
#./configure
#make
#sudo make install
#cd -
#rm -rf lzma-4.32.7

#tar jxvf /work/tools/xz-5.2.1.tar.bz2
cd xz-5.2.1
./configure
make
sudo make install
cd -
rm -rf xz-5.2.1

# 安装sqashfs工具
tar jxvf /work/tools/squashfs-4.3.tar.bz2
cd squashfs-4.3/squashfs-tools/
sed --in-place -e 's;#XZ_SUPPORT = 1;XZ_SUPPORT = 1;g' Makefile
make
sudo make install
cd -
rm -rf squashfs-4.3

# 安装mtd工具
tar jxvf /work/tools/mtd-utils-1.5.1.tar.bz2
cd mtd-utils-1.5.1
make V=1
sudo make install
# 因为没有安装头文件，所以直接脚本安装
sudo mv /usr/include/mtd /usr/include/mtd_old
sudo cp -a include/mtd /usr/include
cd -
rm -rf mtd-utils-1.5.1

# 安装minicom
sudo apt-get -y install minicom

# 安装cmake
sudo apt-get -y install cmake cmake-curses-gui

# 安装opssl
sudo apt-get -y install openssl libssl-dev

# 恢复习惯性配置
cd Utilities
./do.ins
cd -

# 禁止updatedb.mlocate以防止硬盘过度读取
sudo killall updatedb.mlocate
sudo chmod -x /etc/cron.daily/mlocate

#安装 ffmpeg  
#ffmpeg -i 20.avi %08d.jpg 可以将视频文件以逐帧的方式输出jpg文件
sudo apt-get -y install ffmpeg libavformat53 libavformat-dev libavcodec53 libswscale-dev

