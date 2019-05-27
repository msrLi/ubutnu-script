/**
 * @file pri_common.h
 * @brief  包含常用头文件
 * @author <itarge@itarge.com>
 * @version 1.0.0
 * @date 2017-03-10
 */

/* Copyright (C) 2009-2016, Itarge Inc.
 * All right reserved
 *
 */

#ifndef __PRI_COMMON_H__
#define __PRI_COMMON_H__

/* 常用标准C库头文件 */
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>
#include <asm/types.h>
#include <sys/param.h>   /* NOFILE */
#include <sys/time.h>
#include <sys/times.h>
#include <sys/types.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/sysinfo.h>    /* sysinfo函数, 获取系统运行时间 */
#include <sys/ipc.h>        /* 核间通讯 */
#include <sys/msg.h>
#include <sys/sem.h>
#include <semaphore.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <net/route.h>
#include <ifaddrs.h>
#include <unistd.h>
#include <signal.h>
#include <getopt.h>

#include <linux/sockios.h>
#include <pthread.h>
#include <errno.h>

/* data types */
#include <pri_typedef.h>
/* module enum */
#include <pri_module.h>
/* about log file */
#include <pri_log.h>
/* 英泰智OS抽象层 */
//#include <pri_osal.h>
/* 英泰智硬件抽象层 */
//#include <pri_hal.h>
/* 英泰智中间层 */
//#include <pri_middle.h>

#endif //__PRI_COMMON_H__
