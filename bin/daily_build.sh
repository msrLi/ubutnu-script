#!/bin/bash
PLATFORM_PROJICT_PATH=$(cat /home/${USER}/bin/BuildRule.mk)
echo $PLATFORM_PROJICT_PATH
source /home/${USER}/.bashrc
export PATH=/home/${USER}/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:$PATH
export USER=kepler

echo $USER
#设置发送邮件命令
SENDMAILCMD=mail
SENDMAILCMD=${SENDMAILCMD:=sendmail}
#获取当前日期
YESTERDAY=`date --date='1 days ago' +"%Y-%m-%d"`
TODAY=`date +"%Y-%m-%d"`
#编译平台版本
#ip地址
LOCAL_IP=`ifconfig eth0 | grep "inet addr" | cut -d: -f 2 | cut -d " " -f 1`
if [ -z ${LOCAL_IP} ]; then
    LOCAL_IP=`ifconfig eth0 | grep "inet 地址" | cut -d: -f 2 | cut -d " " -f 1`
fi
echo $LOCAL_IP
BuildErrorSendMail ()
{
#检查平台编译是否存在问题
build_errors=`grep Error /tmp/build.log | grep ignored -v | wc -l | awk  '{print $1}'`
if [ $build_errors -ne 1 ]; then
    if [ ${SENDMAILCMD} == "sendmail" ]; then
        echo sendmail
        editor_list=`svn log -r {$YESTERDAY}:{$TODAY} | grep "^r" | awk '{print $3}' | sort | uniq` 
        echo "Subject: Platform Daily Build $YESTERDAY Failed" >/tmp/mail.txt
        echo "From: Daily_Builder@itarge.com" >>/tmp/mail.txt
        echo "To: lihuibin@itarge.com" >>/tmp/mail.txt
        for editor in $editor_list
        do
            echo "To: $editor@itarge.com" >>/tmp/mail.txt
        done
        echo "--------This is an auto mail send by Daily_Builder from ${LOCAL_IP}---------" >>/tmp/mail.txt
        echo "" >>/tmp/mail.txt
        cat /tmp/build.log >>/tmp/mail.txt
        ${SENDMAILCMD} -vt </tmp/mail.txt
    else
        echo "mailx"
        editor_name="lihuibin@itarge.com"
        cc_name="931618179@qq.com"
        editor_list=`svn log -r {$YESTERDAY}:{$TODAY} | grep "^r" | awk '{print $3}' | sort | uniq`
        #editor_list="lihuibin705 liuhaichao1 931918176 hello"
        for editor in $editor_list
        do
            editor_name+=",${editor}@itarge.com"
        done
        echo $editor_name
        echo "--------This is an auto mail send by Daily_Builder from ${LOCAL_IP}---------" | mail -s "trunk Daily Build $YESTERDAY Failed" -a /tmp/build.log -c $cc_name ${editor_name} 
    fi

    
fi
}
#循环进入目录进行编译
cd /home/${USER}/work/
LS_PATH=`ls`
for list_path in $PLATFORM_PROJICT_PATH
do
    echo ${list_path}
    tmp_var= echo ${list_path} | cut -d\/ -f1-4
    echo ${tmp_var}
    #make &>/tmp/build.log
    #BuildErrorSendMail
done
exit 0

#替换最新的平台目录，/home/kepler/work/project_kepler_se/trunk/platform_release直接链接到
#/home/kepler/work/platform/trunk/ti8168/platform_release/

#编译trunk
rm -rf /home/kepler/kepler2.0_images/*
#获取相关版本号
cd /home/kepler/work/project_kepler_se/trunk
svn up --force
SVN_VERSION=`LC_ALL=C svn info | grep Revision | sed -e "s/Revision: //g"`
DIR_NAME=trunk_images_svn${PLATFORM_SVN_VERSION}_${SVN_VERSION}

make clean
svn diff >build.log
find applications -name "build" -exec rm -rf {} \;
make &>>build.log

#检查trunk编译是否存在问题
build_errors=`grep Error build.log | grep ignored -v | wc -l | awk '{print $1}'`
if [ $build_errors -ne 0 ]; then
	editor_list=`svn log -r {$YESTERDAY}:{$TODAY} | grep "^r" | awk '{print $3}' | sort | uniq` 
    editor_list=lihuibin
	echo "Subject: trunk Daily Build $YESTERDAY Failed" >/tmp/mail.txt
	echo "From: Daily_Builder@itarge.com" >>/tmp/mail.txt
	echo "To: rongle@itarge.com" >>/tmp/mail.txt
	for editor in $editor_list
	do
		echo "To: $editor@itarge.com" >>/tmp/mail.txt
	done
	echo "--------This is an auto mail send by Daily_Builder from ${LOCAL_IP}---------" >>/tmp/mail.txt
	echo "" >>/tmp/mail.txt
	cat build.log >>/tmp/mail.txt
	sendmail -vt </tmp/mail.txt
fi

# 如果没有重复编译，就copy
if [ ! -d /home/kepler/daily_build/trunk/${DIR_NAME} ]; then
	mkdir -p /home/kepler/daily_build/trunk/${DIR_NAME}
	cp -af /home/kepler/kepler2.0_images/* /home/kepler/daily_build/trunk/${DIR_NAME}/
	cp -af build.log /home/kepler/daily_build/trunk/${DIR_NAME}/
fi

# Build epolice_V2.0 branch
rm -rf /home/kepler/kepler2.0_images/*
#获取相关版本号
cd /home/kepler/work/project_kepler_se/branches/epolice_V2.0
svn up --force
SVN_VERSION=`LC_ALL=C svn info | grep Revision | sed -e "s/Revision: //g"`
DIR_NAME=epolice_V2.0_images_svn${PLATFORM_SVN_VERSION}_${SVN_VERSION}

make clean
svn diff >build.log
find applications -name "build" -exec rm -rf {} \;
make &>>build.log

#检查epolice_V2.0编译是否存在问题

# 如果没有重复编译，就copy
if [ ! -d /home/kepler/daily_build/epolice_V2.0/${DIR_NAME} ]; then
	mkdir -p /home/kepler/daily_build/epolice_V2.0/${DIR_NAME}
	cp -af /home/kepler/kepler2.0_images/* /home/kepler/daily_build/epolice_V2.0/${DIR_NAME}/
	cp -af build.log /home/kepler/daily_build/epolice_V2.0/${DIR_NAME}/
fi


# Build eCheckpoint branch
rm -rf /home/kepler/kepler2.0_images/*
#获取相关版本号
cd /home/kepler/work/project_kepler_se/branches/eCheckpoint
svn up --force
SVN_VERSION=`LC_ALL=C svn info | grep Revision | sed -e "s/Revision: //g"`
DIR_NAME=eCheckpoint_images_svn${PLATFORM_SVN_VERSION}_${SVN_VERSION}

make clean
svn diff >build.log
find applications -name "build" -exec rm -rf {} \;
make &>>build.log

#检查eCheckpoint编译是否存在问题

# 如果没有重复编译，就copy
if [ ! -d /home/kepler/daily_build/eCheckpoint/${DIR_NAME} ]; then
	mkdir -p /home/kepler/daily_build/eCheckpoint/${DIR_NAME}
	cp -af /home/kepler/kepler2.0_images/* /home/kepler/daily_build/eCheckpoint/${DIR_NAME}/
	cp -af build.log /home/kepler/daily_build/eCheckpoint/${DIR_NAME}/
fi

# Build binocular_vision branch
rm -rf /home/kepler/kepler2.0_images/*
#获取相关版本号
cd /home/kepler/work/project_kepler_se/branches/binocular_vision
svn up --force
SVN_VERSION=`LC_ALL=C svn info | grep Revision | sed -e "s/Revision: //g"`
DIR_NAME=binocular_vision_images_svn${PLATFORM_SVN_VERSION}_${SVN_VERSION}

make clean
svn diff >build.log
find applications -name "build" -exec rm -rf {} \;
make &>>build.log

#检查binocular_vision编译是否存在问题

# 如果没有重复编译，就copy
if [ ! -d /home/kepler/daily_build/binocular_vision/${DIR_NAME} ]; then
	mkdir -p /home/kepler/daily_build/binocular_vision/${DIR_NAME}
	cp -af /home/kepler/kepler2.0_images/* /home/kepler/daily_build/binocular_vision/${DIR_NAME}/
	cp -af build.log /home/kepler/daily_build/binocular_vision/${DIR_NAME}/
fi
