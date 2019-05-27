/**
 * @file pri_common.h
 * @brief  ��������ͷ�ļ�
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

/* ���ñ�׼C��ͷ�ļ� */
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
#include <sys/sysinfo.h>    /* sysinfo����, ��ȡϵͳ����ʱ�� */
#include <sys/ipc.h>        /* �˼�ͨѶ */
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
/* Ӣ̩��OS����� */
//#include <pri_osal.h>
/* Ӣ̩��Ӳ������� */
//#include <pri_hal.h>
/* Ӣ̩���м�� */
//#include <pri_middle.h>

#endif //__PRI_COMMON_H__
