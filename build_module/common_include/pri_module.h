/**
 * @file ite_module.h
 * @brief  ��������ͷ�ļ�
 * @author  <itarge@itarge.com>
 * @version 1.0.0
 * @date 2017-03-10
 */

/* Copyright (C) 2009-2016, Itarge Inc.
 * All right reserved
 *
 */

#ifndef __ITE_MODULE_H__
#define __ITE_MODULE_H__


#ifdef __cplusplus
extern "C" {
#endif

/* ģ��Ŷ��壬 ����ͨ�����󷵻�ֵ���ٶ�λģ�� */
typedef enum E_ITE_MODULE
{
    E_ITE_MODULE_LOG = 0,       /* ��־ģ�� */
    E_ITE_MODULE_RTC,           /* RTCģ�� */
    E_ITE_MODULE_I2C,           /* I2Cģ�� */
    E_ITE_MODULE_SPI,           /* SPIģ�� */
    E_ITE_MODULE_UART,          /* UARTģ�� */
    E_ITE_MODULE_FPGA,          /* FPGAģ�� */
    E_ITE_MODULE_EEPROM,        /* EEPROMģ�� */
    E_ITE_MODULE_VIN,           /* Video����/�ɼ�ģ�� */
    E_ITE_MODULE_GPIO,          /* GPIO ģ�� */
    E_ITE_MODULE_TEMPERATURE,   /* �¶ȴ�����ģ�� */
    E_ITE_MODULE_LOCAL_BUS,     /* ��������ģ�� */
    E_ITE_MODULE_JPEG_SW,       /* jpeg�����ģ�� */
    E_ITE_MODULE_MESSAGE,       /* ���ڹ����ڴ����Ϣ����ģ�� */
    E_ITE_MODULE_SHM,           /* �����ڴ�ģ�� */
    E_ITE_MODULE_MSG,           /* ��Ϣ����ģ�� */
    E_ITE_MODULE_SEM,           /* Posix�ź���ģ�� */
    E_ITE_MODULE_LOCK,          /* �߳���ģ�� */
    E_ITE_MODULE_TASK,          /* ����ģ�� */
    E_ITE_MODULE_SWOSD,         /* ���OSD */
    E_ITE_MODULE_ISP,           /* ͼ����ģ�� */
    E_ITE_MODULE_ENCODE,        /* ����ģ�� */
    E_ITE_MODULE_WATCHDOG,      /* ���Ź�ģ�� */
    E_ITE_MODULE_SEMAPHORE,     /* System V�ź���ģ�� */
    E_ITE_MODULE_SHARE_BUFFER,  /* ����bufferģ�� */
    E_ITE_MODULE_SECRET,        /* ����ģ�� */
    E_ITE_MODULE_PID,           /* �߳�ģ�� */
    E_ITE_MODULE_SYSINFO,       /* ϵͳ��Ϣģ�� */
    E_ITE_MODULE_UPGRADE,       /* ����ģ�� */
    E_ITE_MODULE_UPGRADE_SERVER,       /* ����ģ�� ������*/
    E_ITE_MODULE_MTD,           /* MTDģ�� */
    E_ITE_MODULE_IPC,           /* �˼�ͨѶģ�� */
    E_ITE_MODULE_NET,           /* NETģ�� */
    E_ITE_MODULE_SOCKET,        /* SOCKETģ�� */
    E_ITE_MODULE_HASHCMD,       /* HASHCMDģ�� */
    E_ITE_MODULE_MEMCPY,        /* MEMCPYģ�� */
    E_ITE_MODULE_PINCTRL,       /* PINCTRLģ�� */
    E_ITE_MODULE_XC7060,        /* XC7060 ISPģ��(�Ϻ�) */
    E_ITE_MODULE_CONVERT,       /* ͼ��ת��ģ�� */
    E_ITE_MODULE_SYSCTRL,       /* ϵͳ����ģ�� */
    E_ITE_MODULE_HWINFO,        /* HAL ����Ϣģ�� */
    E_ITE_MODULE_RESIZE,        /* ͼƬ����ģ�� */
    E_ITE_MODULE_VERSION,       /* �汾��Ϣ��ȡģ�� */
    E_ITE_MODULE_BOARD_TEST,    /* Ӳ������ģ�� */
    E_ITE_MODULE_FILE,          /* �ļ�����ģ�� */
    E_ITE_MODULE_LED,           /* LEDģ�� */
    E_ITE_MODULE_COMPRESS,      /* ѹ����ѹ��ģ�� */
    E_ITE_MODULE_PWM,           /* PWMģ�� */
    E_ITE_MODULE_IRIS,          /* �Զ���Ȧģ�� */
    E_ITE_MODULE_SIL1227,       /* sil1127ģ�� */
    E_ITE_MODULE_MACHXO2,       /* Machxo2 cpld ģ�� */
    E_ITE_MODULE_CRYPTO,        /* �������ģ�� */
    E_ITE_MODULE_DIR,           /* Ŀ¼��ز���ģ�� */
    E_ITE_MODULE_AUTO_TEST,     /* �Զ�����ģ�� */
} E_ITE_MODULE;

/* bit[31:16] ���ڴ洢ģ��ţ�bit[15:0]���ڴ洢������ */
#define ITE_MODULE_SHIFT        (16)
/*  ת������λ�����ڴ���ֱ��ʹ�� */
#define ITE_MODULE_VALUE(m)     (m <<ITE_MODULE_SHIFT)
#define ITE_MODULE_MASK         (0xFFFF0000)
/* bit[15:0] ���ڴ洢������ */
#define ITE_ERROR_MASK          (0x0000FFFF)

/* ����ģ��ź꣬����ʹ�� */
#define ITE_MODULE_LOG          ITE_MODULE_VALUE(E_ITE_MODULE_LOG)
#define ITE_MODULE_RTC          ITE_MODULE_VALUE(E_ITE_MODULE_RTC)
#define ITE_MODULE_I2C          ITE_MODULE_VALUE(E_ITE_MODULE_I2C)
#define ITE_MODULE_SPI          ITE_MODULE_VALUE(E_ITE_MODULE_SPI)
#define ITE_MODULE_UART         ITE_MODULE_VALUE(E_ITE_MODULE_UART)
#define ITE_MODULE_FPGA         ITE_MODULE_VALUE(E_ITE_MODULE_FPGA)
#define ITE_MODULE_EEPROM       ITE_MODULE_VALUE(E_ITE_MODULE_EEPROM)
#define ITE_MODULE_VIN          ITE_MODULE_VALUE(E_ITE_MODULE_VIN)
#define ITE_MODULE_GPIO         ITE_MODULE_VALUE(E_ITE_MODULE_GPIO)
#define ITE_MODULE_TEMPERATURE  ITE_MODULE_VALUE(E_ITE_MODULE_TEMPERATURE)
#define ITE_MODULE_LOCAL_BUS    ITE_MODULE_VALUE(E_ITE_MODULE_LOCAL_BUS)
#define ITE_MODULE_JPEG_SW      ITE_MODULE_VALUE(E_ITE_MODULE_JPEG_SW)
#define ITE_MODULE_MESSAGE      ITE_MODULE_VALUE(E_ITE_MODULE_MESSAGE)
#define ITE_MODULE_SHM          ITE_MODULE_VALUE(E_ITE_MODULE_SHM)
#define ITE_MODULE_MSG          ITE_MODULE_VALUE(E_ITE_MODULE_MSG)
#define ITE_MODULE_SEM          ITE_MODULE_VALUE(E_ITE_MODULE_SEM)
#define ITE_MODULE_LOCK         ITE_MODULE_VALUE(E_ITE_MODULE_LOCK)
#define ITE_MODULE_TASK         ITE_MODULE_VALUE(E_ITE_MODULE_TASK)
#define ITE_MODULE_SWOSD        ITE_MODULE_VALUE(E_ITE_MODULE_SWOSD)
#define ITE_MODULE_ISP          ITE_MODULE_VALUE(E_ITE_MODULE_ISP)
#define ITE_MODULE_ENCODE       ITE_MODULE_VALUE(E_ITE_MODULE_ENCODE)
#define ITE_MODULE_WATCHDOG     ITE_MODULE_VALUE(E_ITE_MODULE_WATCHDOG)
#define ITE_MODULE_SEMAPHORE    ITE_MODULE_VALUE(E_ITE_MODULE_SEMAPHORE)
#define ITE_MODULE_SHARE_BUFFER ITE_MODULE_VALUE(E_ITE_MODULE_SHARE_BUFFER)
#define ITE_MODULE_SECRET       ITE_MODULE_VALUE(E_ITE_MODULE_SECRET)
#define ITE_MODULE_PID          ITE_MODULE_VALUE(E_ITE_MODULE_PID)
#define ITE_MODULE_SYSINFO      ITE_MODULE_VALUE(E_ITE_MODULE_SYSINFO)
#define ITE_MODULE_UPGRADE      ITE_MODULE_VALUE(E_ITE_MODULE_UPGRADE)
#define ITE_MODULE_UPGRADE_SERVER      ITE_MODULE_VALUE(E_ITE_MODULE_UPGRADE_SERVER)
#define ITE_MODULE_MTD          ITE_MODULE_VALUE(E_ITE_MODULE_MTD)
#define ITE_MODULE_IPC          ITE_MODULE_VALUE(E_ITE_MODULE_IPC)
#define ITE_MODULE_NET          ITE_MODULE_VALUE(E_ITE_MODULE_NET)
#define ITE_MODULE_SOCKET       ITE_MODULE_VALUE(E_ITE_MODULE_SOCKET)
#define ITE_MODULE_HASHCMD      ITE_MODULE_VALUE(E_ITE_MODULE_HASHCMD)
#define ITE_MODULE_MEMCPY       ITE_MODULE_VALUE(E_ITE_MODULE_MEMCPY)
#define ITE_MODULE_PINCTRL      ITE_MODULE_VALUE(E_ITE_MODULE_PINCTRL)
#define ITE_MODULE_XC7060       ITE_MODULE_VALUE(E_ITE_MODULE_XC7060)
#define ITE_MODULE_CONVERT      ITE_MODULE_VALUE(E_ITE_MODULE_CONVERT)
#define ITE_MODULE_SYSCTRL      ITE_MODULE_VALUE(E_ITE_MODULE_SYSCTRL)
#define ITE_MODULE_HWINFO       ITE_MODULE_VALUE(E_ITE_MODULE_HWINFO)
#define ITE_MODULE_RESIZE       ITE_MODULE_VALUE(E_ITE_MODULE_RESIZE)
#define ITE_MODULE_VERSION      ITE_MODULE_VALUE(E_ITE_MODULE_VERSION)
#define ITE_MODULE_BOARD_TEST   ITE_MODULE_VALUE(E_ITE_MODULE_BOARD_TEST)
#define ITE_MODULE_FILE         ITE_MODULE_VALUE(E_ITE_MODULE_FILE)
#define ITE_MODULE_LED          ITE_MODULE_VALUE(E_ITE_MODULE_LED)
#define ITE_MODULE_COMPRESS     ITE_MODULE_VALUE(E_ITE_MODULE_COMPRESS)
#define ITE_MODULE_PWM          ITE_MODULE_VALUE(E_ITE_MODULE_PWM)
#define ITE_MODULE_IRIS         ITE_MODULE_VALUE(E_ITE_MODULE_IRIS)
#define ITE_MODULE_SIL1127      ITE_MODULE_VALUE(E_ITE_MODULE_SIL1227)
#define ITE_MODULE_MACHXO2      ITE_MODULE_VALUE(E_ITE_MODULE_MACHXO2)
#define ITE_MODULE_CRYPTO       ITE_MODULE_VALUE(E_ITE_MODULE_CRYPTO)
#define ITE_MODULE_DIR          ITE_MODULE_VALUE(E_ITE_MODULE_DIR)
#define ITE_MODULE_AUTO_TEST    ITE_MODULE_VALUE(E_ITE_MODULE_AUTO_TEST)

#ifdef __cplusplus
}
#endif

#endif //__ITE_MODULE_H__
