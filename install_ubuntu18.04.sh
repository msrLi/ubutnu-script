#!/bin/bash


# 解决依赖神器 
sudo apt-get -y install apt-file
sudo apt-file update
# 安装系统必要库 
sudo apt-get -y install pkg-config bison fakeroot flex mtdev-tools mtd-utils  build-essential curl expat tk libcurl4-openssl-dev 
sudo apt-get -y python-dev libfreetype6-dev libfreetype6 
# 添加网络
sudo apt install net-tools

#编译开发
sudo apt-get -y install gcc g++ 
sudo apt-get -y install mtd-utils
#安装zlib
sudo apt-get -y install zlib1g-dev zlibc
#安装lzma
sudo apt-get -y install lzma
# xz 
sudo apt-get -y install zx-utils liblzo2-dev libacl1-dev uuid-dev

# mksquashfs 
sudo apt-get -y install squashfs-tools

sudo apt-get update;
sudo apt-get install vim
sudo apt-get install autoconf
sudo apt-get install git
sudo apt-get insatll htop
sudo apt-get install automake autopoint libtool gettext
sudo apt-get install tree
sudo apt-get install unrar
sudo apt-get install make 

# 安装vsftpd
sudo apt-get -y install vsftpd
sudo sed -in-place -e 's/#write_enable=YES/write_enable=YES/' /etc/vsftpd.conf

# 安装tftpd
sudo apt-get -y install tftpd tftp xinetd
#sudo apt-get -y install tftp-hpa tftpd-hpa openbsd-inetd
#sudo /etc/init.d/tftpd-hpa restart

# 安装Samba
sudo apt-get -y install samba
sudo apt-get -y install cifs-utils

# 安装版本管理软件
sudo apt-get -y install cvs tree
sudo apt-get -y install git-svn git
sudo apt-get -y install subversion subversion-tools
sudo apt-get -y install bash-completion

git config --global color.status auto
git config --global color.diff auto
git config --global color.branch auto
git config --global color.interactive auto
git config --global user.name "lihuibin" 
git config --global user.email "lihuibin705@163.com"

# 安装vim编辑器
sudo apt-get -y install vim exuberant-ctags cscope global

# 安装netperf
sudo apt-get -y install netperf iperf

# 安装minicom
sudo apt-get -y install minicom

# 安装cmake
sudo apt-get -y install cmake cmake-curses-gui

# 安装opssl
sudo apt-get -y install openssl libssl-dev ssh ssh-server
sudo apt-get -y install openssh-server openssh-client
#远程共享桌面
sudo apt-get install dconf-editor

# 安装内存出错管理工具 
sudo apt-get -y install valgrind

echo "export PATH=/opt/bin:\${PATH}" >> ~/.bashrc
echo " " >> ~/.bashrc

#.vimrc 
echo "set runtimepath+=~/vim_runtime" > ~/.vimrc
echo "\" source ~/vim_runtime/vimrcs/basic.vim" >>  ~/.vimrc
echo "source ~/vim_runtime/vimrcs/filetypes.vim" >>  ~/.vimrc
echo "source ~/vim_runtime/vimrcs/DoxygenToolkit.vim" >> ~/.vimrc
echo "\" source ~/vim_runtime/vimrcs/plugine_config.vim" >> ~/.vimrc
echo "\" source ~/vim_runtime/vimrcs/extended.vim" >> ~/.vimrc
echo "try" >>  ~/.vimrc
echo "source ~/vim_runtime/my_configs.vim" >>  ~/.vimrc
echo "catch" >>  ~/.vimrc
echo "endtry" >>  ~/.vimrc

#repo 下载 
curl https://storage.googleapis.com/git-repo-downloads/repo > /opt/bin/repo
chmod +x /opt/bin/repo

#安装ffmepeg
#ffmepeg -i 20.avi %08d.jpg 可以将视频文件逐帧的方式输出jpg文件
sudo apt-get install libavformat57 ffmpeg libavformat-dev libavcodec57 libswscale-dev
