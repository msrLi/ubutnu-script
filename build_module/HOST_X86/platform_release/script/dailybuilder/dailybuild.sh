#!/bin/bash
source dailybuild.mk
source /home/${USER}/.bashrc
export PATH=/home/${USER}/bin/dailybuilder:/home/${USER}/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:$PATH

IFS_OLD=${IFS}
DATE=`date +"%Y-%m-%d"`
#编译前清除build release目录
rm -rf /home/${USER}/build /home/${USER}/release
#编译前更新svn
cd  ${SVN_ROOT_PATH}/
SVN_LIST=`ls -l | grep "^d" | awk '{print $9}'`
for SVN_DIR in ${SVN_LIST}
do
	echo "${SVN_ROOT_PATH}/${SVN_DIR} ${BUILD_ROOT_PATH}/"
	#更新svn
	cd ${SVN_ROOT_PATH}/${SVN_DIR}
	svn update --force 
	#同步到编译目录
	rsync --delete -av ${SVN_ROOT_PATH}/${SVN_DIR} ${BUILD_ROOT_PATH}/
done
#################################################################################
#			删除目录建立文件连接表 					#
#################################################################################
rm -rf ${BUILD_ROOT_PATH}/platform_v2_app/trunk/HI3519AV100/platform_release
ln -sf ${BUILD_ROOT_PATH}/platform_v2/trunk/HI3519AV100/platform_release ${BUILD_ROOT_PATH}/platform_v2_app/trunk/HI3519AV100/platform_release
rm -rf ${BUILD_ROOT_PATH}/platform_v2_app/trunk/HI3519V101/platform_release
ln -sf ${BUILD_ROOT_PATH}/platform_v2/trunk/HI3519AV101/platform_release ${BUILD_ROOT_PATH}/platform_v2_app/trunk/HI3519V101/platform_release			
rm -rf ${BUILD_ROOT_PATH}/platform_v2_app/trunk/HI3559AV100/platform_release
ln -sf ${BUILD_ROOT_PATH}/platform_v2/trunk/HI3559AV100/platform_release ${BUILD_ROOT_PATH}/platform_v2_app/trunk/HI3559AV100/platform_release
rm -rf ${BUILD_ROOT_PATH}/platform_v2_app/trunk/TX1/platform_release
ln -sf ${BUILD_ROOT_PATH}/platform_v2/trunk/TX1/platform_release ${BUILD_ROOT_PATH}/platform_v2_app/trunk/TX1/platform_release
#应用
rm -rf ${BUILD_ROOT_PATH}/platform_v2_app/branches/project_eleven/TX1/platform_release 
ln -sf ${BUILD_ROOT_PATH}/platform_v2/trunk/TX1/platform_release ${BUILD_ROOT_PATH}/platform_v2_app/branches/project_eleven/TX1/platform_release
rm -rf ${BUILD_ROOT_PATH}/platform_v2_app/branches/project_eleven/TX1/apps_release
ln -sf ${BUILD_ROOT_PATH}/platform_v2_app/trunk/TX1/apps_release ${BUILD_ROOT_PATH}/platform_v2_app/branches/project_eleven/TX1/apps_release

rm -rf ${BUILD_ROOT_PATH}/platform_v2_app/branches/project_etc/TX1/platform_release 
ln -sf ${BUILD_ROOT_PATH}/platform_v2/trunk/TX1/platform_release ${BUILD_ROOT_PATH}/platform_v2_app/branches/project_etc/TX1/platform_release
rm -rf ${BUILD_ROOT_PATH}/platform_v2_app/branches/project_etc/TX1/apps_release
ln -sf ${BUILD_ROOT_PATH}/platform_v2_app/trunk/TX1/apps_release ${BUILD_ROOT_PATH}/platform_v2_app/branches/project_etc/TX1/apps_release

rm -rf ${BUILD_ROOT_PATH}/platform_v2_app/branches/project_falconits/HI3519AV100/platform_release
ln -sf ${BUILD_ROOT_PATH}/platform_v2/trunk/HI3519AV100/platform_release ${BUILD_ROOT_PATH}/platform_v2_app/branches/project_falconits/HI3519AV100/platform_release
rm -rf ${BUILD_ROOT_PATH}/platform_v2_app/branches/project_falconits/HI3519AV100/apps_release
ln -sf ${BUILD_ROOT_PATH}/platform_v2_app/trunk/HI3519AV100/apps_release ${BUILD_ROOT_PATH}/platform_v2_app/branches/project_falconits/HI3519AV100/apps_release

rm -rf ${BUILD_ROOT_PATH}/platform_v2_app/branches/project_falconits/HI3559AV100/platform_release
ln -sf ${BUILD_ROOT_PATH}/platform_v2/trunk/HI3559AV100/platform_release ${BUILD_ROOT_PATH}/platform_v2_app/branches/project_falconits/HI3559AV100/platform_release
rm -rf ${BUILD_ROOT_PATH}/platform_v2_app/branches/project_falconits/HI3559AV100/apps_release
ln -sf ${BUILD_ROOT_PATH}/platform_v2_app/trunk/HI3559AV100/apps_release ${BUILD_ROOT_PATH}/platform_v2_app/branches/project_falconits/HI3559AV100/apps_release

rm -rf ${BUILD_ROOT_PATH}/platform_v2_app/branches/project_gas/TX1/platform_release
ln -sf ${BUILD_ROOT_PATH}/platform_v2/trunk/TX1/platform_release ${BUILD_ROOT_PATH}/platform_v2_app/branches/project_gas/TX1/platform_release
rm -rf ${BUILD_ROOT_PATH}/platform_v2_app/branches/project_gas/TX1/apps_release
ln -sf ${BUILD_ROOT_PATH}/platform_v2_app/trunk/TX1/apps_release ${BUILD_ROOT_PATH}/platform_v2_app/branches/project_gas/TX1/apps_release

rm -rf ${BUILD_ROOT_PATH}/platform_v2_app/branches/project_ipark/TX1/platform_release 
ln -sf ${BUILD_ROOT_PATH}/platform_v2/trunk/TX1/platform_release  ${BUILD_ROOT_PATH}/platform_v2_app/branches/project_ipark/TX1/platform_release
rm -rf ${BUILD_ROOT_PATH}/platform_v2_app/branches/project_ipark/TX1/apps_release
ln -sf ${BUILD_ROOT_PATH}/platform_v2_app/trunk/TX1/apps_release ${BUILD_ROOT_PATH}/platform_v2_app/branches/project_ipark/TX1/apps_release

rm -rf ${BUILD_ROOT_PATH}/platform_v2_app/branches/project_its/TX1/platform_release
ln -sf ${BUILD_ROOT_PATH}/platform_v2/trunk/TX1/platform_release ${BUILD_ROOT_PATH}/platform_v2_app/branches/project_its/TX1/platform_release
rm -rf ${BUILD_ROOT_PATH}/platform_v2_app/branches/project_its/TX1/apps_release
ln -sf ${BUILD_ROOT_PATH}/platform_v2_app/trunk/TX1/apps_release ${BUILD_ROOT_PATH}/platform_v2_app/branches/project_its/TX1/apps_release

rm -rf ${BUILD_ROOT_PATH}/platform_v2_app/branches/project_neuron/TX1/platform_release
ln -sf ${BUILD_ROOT_PATH}/platform_v2/trunk/TX1/platform_release ${BUILD_ROOT_PATH}/platform_v2_app/branches/project_neuron/TX1/platform_release
rm -rf ${BUILD_ROOT_PATH}/platform_v2_app/branches/project_neuron/TX1/apps_release
ln -sf ${BUILD_ROOT_PATH}/platform_v2_app/trunk/TX1/apps_release ${BUILD_ROOT_PATH}/platform_v2_app/branches/project_neuron/TX1/apps_release

rm -rf ${BUILD_ROOT_PATH}/platform_v2_app/branches/project_spy/HI3559AV100/platform_release
ln -sf ${BUILD_ROOT_PATH}/platform_v2/trunk/HI3559AV100/platform_release ${BUILD_ROOT_PATH}/platform_v2_app/branches/project_spy/HI3559AV100/platform_release
rm -rf ${BUILD_ROOT_PATH}/platform_v2_app/branches/project_spy/HI3559AV100/apps_release
ln -sf ${BUILD_ROOT_PATH}/platform_v2_app/trunk/HI3519AV100/apps_release  ${BUILD_ROOT_PATH}/platform_v2_app/branches/project_spy/HI3559AV100/apps_release


##################################################################################
#				平台代码编译					 #
##################################################################################
#以":"为分隔符,拆分平台列表 并编译
IFS=':'
PLATFORM_ARRAYS=(${PLATFORM_LISTS}) 
for var in ${PLATFORM_ARRAYS[@]}
do	
	#更改分割符为空格
	IFS='  '
	#获取各自平台列表
	ARRAY=(${var})
	#第一个数据为路径
	#如果路径为空则推出
	if [ "${ARRAY[0]}" = "" ];then
		continue;
	fi
	#记录平台路径
	PLATFORM_PATH=${BUILD_ROOT_PATH}/${ARRAY[0]}
	value=0
	for var_tmp in ${ARRAY[@]}
	do 
		let value++
		#跳过第一个路径
		if [ $value -gt 1 ];then
			#检测 platform_release 文件夹是否存在
			if [ ! -d "${PLATFORM_PATH}/${var_tmp}/platform_release" ];then
				ORING_FILE=`echo "${PLATFORM_PATH}/${var_tmp}/platform_release" | sed 's/_app//g'`
			fi
			. dailybuild.func;build_makefile ${PLATFORM_PATH} ${var_tmp} ${PLATFORM_PATH}/${var_tmp}/build.log
			. dailybuild.func;BuildErrorSendMail ${PLATFORM_PATH}/${var_tmp}/build.log ${PLATFORM_PATH}/${var_tmp}
		fi

	done
done

################################################################################
#  		      platform_v2_app 应用代码编译			       #
################################################################################
#以":"为分隔符,拆分平台列表 并编译
IFS=':'
PLATFORM_ARRAYS=(${PRIJECT_LISTS})
for var in ${PLATFORM_ARRAYS[@]}
do
        #更改分割符为空格
        IFS='  '
        #获取各自平台列表
        ARRAY=(${var})
        #第一个数据为路径
        #如果路径为空则推出
        if [ "${ARRAY[0]}" = "" ];then
                continue;
        fi
        value=0
        for var_tmp in ${ARRAY[@]}
        do
                let value++
                #跳过第一个路径
                if [ $value -gt 1 ];then
			PLATFORM_PATH=${BUILD_ROOT_PATH}/${ARRAY[0]}/${var_tmp}
			cd ${PLATFORM_PATH}
			# 获取平台目录 
			LIST=`ls -l | grep "^d" | awk '{print $9}'`
			LIST_BIGGER=
			THIS_IFS=${IFS}
			IFS=$IFS_OLD
			for isbig in ${LIST[@]}
			do
				if [[ ${isbig} != *[[:lower:]]* ]];then
					LIST_BIGGER+="${isbig} "
				fi
			done
			#去除svn up 
			sed --in-place -e 's;svn update; ;g' ${PLATFORM_PATH}/Makefile
			IFS=${THIS_IFS}
			# 循环编译应用程序
			for val in ${LIST_BIGGER}
			do 
                rm -rf ${PLATFORM_PATH}/${val}/objs
				# 根据项目依次编译相关平台生成应用程序
				. dailybuild.func;build_makefile ${PLATFORM_PATH} ${val} ${PLATFORM_PATH}/${val}/build.log
				# 检查编译是否正确
				. dailybuild.func;BuildErrorSendMail ${PLATFORM_PATH}/${val}/build.log ${PLATFORM_PATH}/${val}
				#保存编译生成的应用程序
				mkdir -p ${BUILD_PRODUCT_PATH}/${DATE}/${val}_Image/${var_tmp}
				mv /home/${USER}/${val}_images/*.bin  ${BUILD_PRODUCT_PATH}/${DATE}/${val}_Image/${var_tmp}/
			done
                fi
        done
done

#编译应用程序
#DIR_NAME=${TRUCK_PATH##*/}  # 截留 最后出现 "/" 右边的值
#TRUCK_PATH=${TRUCK_PATH%/*} #截留 最后出现"/" 坐标的的值
