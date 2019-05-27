/**
 * @file ite_sil1127_main.c
 * @brief sil1127 ������
 * @author  <itarge@itarge.com>
 * @version 1.0.0
 * @date 2018-08-13
 */

/* Copyright(C) 2009-2017, Itarge Inc.
 * All right reserved
 *
 */


#include <ite_common.h>
#include <ite_i2c.h>
#include <ite_sil1127.h>


OS_S32 main(OS_S32 argc, OS_S8 *argv[])
{
    OS_S32 ret = 0;

    /* i2c ���߳�ʼ�� */
    ret = ite_sil1127_i2c_bus_init();
    /* ��������ʼ�� */
    CyFxUsbI2cSi1127Init();
    /* �˿ڳ�ʼ�� */
    Si11227EnablePort();
    /* ����˿� */
    Si11227ActivatePort();

    return ret;
}
