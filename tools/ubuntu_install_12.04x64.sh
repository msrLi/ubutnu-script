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

# ��װvsftpd
sudo apt-get -y install vsftpd
sudo sed -in-place -e 's/#write_enable=YES/write_enable=YES/' /etc/vsftpd.conf 
# ��װtftpd
sudo apt-get -y install tftpd tftp xinetd
#sudo apt-get -y install tftp-hpa tftpd-hpa openbsd-inetd
#sudo /etc/init.d/tftpd-hpa restart
# ��װSamba
sudo apt-get -y install samba
sudo apt-get -y install smbfs

# ��װzlib
sudo apt-get -y install libacl1-dev zlib1g-dev liblzo2-dev
sudo apt-get -y install xz-utils uuid-dev

# ��װlibiconv
sudo apt-get -y install rpm

# ��װmksquashfs
sudo apt-get -y install squashfs-tools 

# ��װcramfs tool
sudo apt-get -y install cramfsprogs

# ��װnetperf
sudo apt-get -y install netperf iperf

# ��װopenGL��
sudo apt-get -y install mesa-common-dev mesademos libgl1-mesa-dev libglu1-mesa-dev freeglut3-dev

# ��װvim�༭��
sudo apt-get -y install vim exuberant-ctags cscope global 

# ��װ�汾�������
sudo apt-get -y install cvs tree
sudo apt-get -y install subversion subversion-tools
tar zxvf /work/tools/git-1.8.3.1.tar.gz
cd git-1.8.3.1
make prefix=/usr/local all; sudo make prefix=/usr/local install
cd -
rm -rf git-1.8.3.1

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

# ��װѹ����
# zlib���ܸ��£�����UI����������
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

# ��װsqashfs����
tar jxvf /work/tools/squashfs-4.3.tar.bz2
cd squashfs-4.3/squashfs-tools/
sed --in-place -e 's;#XZ_SUPPORT = 1;XZ_SUPPORT = 1;g' Makefile
make
sudo make install
cd -
rm -rf squashfs-4.3

# ��װmtd����
tar jxvf /work/tools/mtd-utils-1.5.1.tar.bz2
cd mtd-utils-1.5.1
make V=1
sudo make install
# ��Ϊû�а�װͷ�ļ�������ֱ�ӽű���װ
sudo mv /usr/include/mtd /usr/include/mtd_old
sudo cp -a include/mtd /usr/include
cd -
rm -rf mtd-utils-1.5.1

# ��װminicom
sudo apt-get -y install minicom

# ��װcmake
sudo apt-get -y install cmake cmake-curses-gui

# ��װopssl
sudo apt-get -y install openssl libssl-dev

# �ָ�ϰ��������
cd Utilities
./do.ins
cd -

# ��ֹupdatedb.mlocate�Է�ֹӲ�̹��ȶ�ȡ
sudo killall updatedb.mlocate
sudo chmod -x /etc/cron.daily/mlocate

#��װ ffmpeg  
#ffmpeg -i 20.avi %08d.jpg ���Խ���Ƶ�ļ�����֡�ķ�ʽ���jpg�ļ�
sudo apt-get -y install ffmpeg libavformat53 libavformat-dev libavcodec53 libswscale-dev

