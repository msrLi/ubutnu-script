#!/bin/bash

PWD=`pwd`
source prodect_upgrade.mk
export LD_LIBRARY_PATH=${PWD}/lib:${export}
export PATH=${PWD}:${PATH}


ENV=`env`
DATE=`date +"%Y-%m-%d"`

#设置发送邮件命令
#SENDMAILCMD=mailx
SENDMAILCMD=${SENDMAILCMD:=sendmail}
#获取当前日期
TODAY=`date +"%Y-%m-%d"`

##########################################################
# function BuildErrorSendMail
# param $1 日历文件(路径加名称)
# param $2 邮件主题
# param $3 工作目录
##########################################################
function ErrorSendMail ()
{
#备份环境变量
LANG_OLD=${LANG}
LANGUAGE_OLD=${LANGUAGE}
#export LANG=UTF-8
#export LANG=en_US.en
export LANG=zh_CN.UTF-8
export LANGUAGE=en_US.en
#检查平台编译是否存在问题
build_errors=`grep Error $1 | grep ignored -v | wc -l | awk  '{print $1}'`
#获取svn提交用户消息
TARGET_NAME=${2##*/}
if [ $build_errors -ne 0 ]; then # 1 始终发送邮件 
        if [ ${SENDMAILCMD} == "sendmail" ]; then
               	echo "Subject: Upgrade Server ${TODAY} Failed" >/home/${USER}/mail.txt
               	echo "From: dailybuilder@itarge.com" >>/home/${USER}/mail.txt
               	#echo "Cc: rongle@itarge.com" >>/home/${USER}/mail.txt
               	for editor in ${MAIL_LISTS}
               	do
                	echo "To: ${editor}@itarge.com" >>/home/${USER}/mail.txt
               	done
		echo "MIME-Version: 1.0" >> /home/${USER}/mail.txt
		echo 'Content-Type: multipart/mixed; boundary="GvXjxJ+pjyke8COw"' >> /home/${USER}/mail.txt
		echo "" >> /home/${USER}/mail.txt
		echo "--GvXjxJ+pjyke8COw" >> /home/${USER}/mail.txt
		echo "Content-Type: text/plain" >> /home/${USER}/mail.txt
		echo "" >> /home/${USER}/mail.txt
		cat $1 >>/home/${USER}/mail.txt
               	echo "" >>/home/${USER}/mail.txt
               	#code_gb2312onefile.sh /home/${USER}/mail.txt
               	${SENDMAILCMD} -vt </home/${USER}/mail.txt
        else
               	editor_name="rongle@itarge.com"
               	cc_name="rongle@itarge.com"
               	editor_list=`svn log -r {$YESTERDAY}:{$TODAY} | grep "^r" | awk '{print $3}' | sort | uniq`
               	for editor in ${MAIL_LISTS} 
               	do
                       editor_name+=",${editor}@itarge.com"
               	done
               	echo $editor_name
               	echo "--------This mail is send by Daily_Builder from ${LOCAL_IP} automatically---------" > /home/${USER}/mail.txt
               	cat $1 >> /home/${USER}/mail.txt
               	cat  /home/${USER}/mail.txt  | mailx -s "Project Daily Build $YESTERDAY Failed" -a $1 -c $cc_name ${editor_name}
        fi
fi
#恢复环境变量
export LANG=${LANG_OLD}
export LANGUAGE=${LANGUAGE_OLD}
}

rm -rf ${UPGRADE_LOG}
IFS_OLD=${IFS}
#IPLIST=(${UPGRADE_LISTS})
#FILE=${PLATFORM_ARRAYS[-1]}
#unset IPLIST[-1]
IFS=':'
PLATFORM_ARRAYS=(${UPGRADE_LISTS})
for var in ${PLATFORM_ARRAYS[@]}
do
	rm -rf ${UPGRADE_LOG}
	echo ${var}
	IFS=${IFS_OLD}
	IP_LISTS=(${var})
	UP_FILE=(${var})
	unset IP_LISTS[-1]
	echo "${IP_LISTS[@]}"
	echo "${UP_FILE[-1]}"
	if [ -f ${PRODUCT_ROOT_PATH}/${DATE}/${UP_FILE[-1]} ];then
		ite_net_upgrade ${IP_LISTS[@]} ${PRODUCT_ROOT_PATH}/${DATE}/${UP_FILE[-1]} > ${UPGRADE_LOG}
		#检查升级是否错误
		if [ $? -ne 0 ];then
			echo "Upgrade ${UP_LISTS[@]} ${PRODUCT_ROOT_PATH}/${DATE}/${UP_FILE[-1]} Error"  >>  ${UPGRADE_LOG}			
			ErrorSendMail ${UPGRADE_LOG} 
		fi	
	else 
		echo "File ${PRODUCT_ROOT_PATH}/${DATE}/${UP_FILE[-1]} not exit" > ${UPGRADE_LOG}
		echo "Error by dailybuild "  >>  ${UPGRADE_LOG}
	
	fi
done

