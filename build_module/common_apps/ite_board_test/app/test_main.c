/**
 * @file test_main.c
 * @brief board test 应用程序
 * @author  <itarge@itarge.com>
 * @version 1.0.1
 * @date 2018-03-12
 */

/* Copyright(C) 2009-2017, Itarge Inc.
 * All right reserved
 *
 */

#include <ite_common.h>
#include <ite_hal.h>
#include <ite_middle.h>

OS_S32 usage(OS_S8 *func_name)
{
    printf("\nUsage: %s [onboard]\n", func_name);
    printf("\t onboard:  run only in  board to test\n");
    printf("\t no parameter Run In Serial command\n");
    return 0;
}

int main(int argc, char **argv)
{
    OS_S32 ret = 0;
    OS_S32 fd = 0;
    ITE_BOARD_TEST_PACKAGE_T *package = NULL;
    OS_U8 cmd = 0;
    OS_U8 test_value = 0;
    OS_S32 result_value = 0;
    OS_U32 on_board = 0;

    /* 确保存储设备已经正确挂载 */
    system("/usr/bin/mountusb sda /mnt/usb");
    system("/usr/bin/mountsd mmcblk1 /mnt/sd");
    /* 初始化FPGA */
    ret = ITE_FPGA_Init(0);

    if (argc != 1)
    {
        if (strcmp(argv[1], "onboard"))
        {
            usage(argv[0]);
            on_board = 0;
            return 1;
        }
        else
        {
            on_board = 1;
        }
    }

    /* 仅仅通过板子测试 */
    if (on_board)
    {
        ret = ITE_BOARD_TEST_EEPROM_Func();
        ret |= ITE_BOARD_TEST_RTC_Func();
        ret |= ITE_BOARD_TEST_TEMP_Func(&result_value);
        ret |= ITE_BOARD_TEST_Secret_Func();
        ret |= ITE_BOARD_TEST_SPI_Flash_Func();
        ret |= ITE_BOARD_TEST_RS485_Func();
        ret |= ITE_BOARD_TEST_ETH_Func();
        ret |= ITE_BOARD_TEST_Storage_func(STORAGE_SDCARD);
        ret |= ITE_BOARD_TEST_Storage_func(STORAGE_USB);
        ret |= ITE_BOARD_TEST_GPI_Func(0, &result_value);
        ret |= ITE_BOARD_TEST_GPO_Func(test_value);
        return ret;
    }
    /* 初始化Board Test 模块;主要功能是打开指定串行 */
    ret = ITE_BOARD_TEST_INIT_Func(&fd);
    ITE_CHECK_ERROR(ret, 1, return_err, "ITE Board Test Init Func Runing Failed\n");
    package = malloc(sizeof(ITE_BOARD_TEST_PACKAGE_T));
    ITE_CHECK_ERROR((NULL == package), 2, return_err, "ITE Board Test Malloc Failed\n");

    while (1)
    {
        /* 获取数据包 */
        ret = ITE_BOARD_TEST_Get_Package(fd, package);
        if (ret == 0)
        {
            /* 从数据包中解析测试命令*/
            ret = ITE_BOARD_TEST_Package_Prase_Command(package, &cmd);
            /* 从数据包中解析测试值 */
            ret |=  ITE_BOARD_TEST_Package_Prase_Vlaue(package, &test_value);
            /* 检验是否正常解析*/
            ITE_CHECK_ERROR(ret, 3, return_err, "ITE Board Test Prase Command or Value Failed\n");
            /* 根据测试命令进行对应的测试*/
            switch (cmd)
            {
                case BOARD_TEST_EEPROM:
                    ret = ITE_BOARD_TEST_EEPROM_Func();
                    break;
                case BOARD_TEST_RTC:
                    ret = ITE_BOARD_TEST_RTC_Func();
                    break;
                case BOARD_TEST_TEMP:
                    ret = ITE_BOARD_TEST_TEMP_Func(&result_value);
                    ret = 0;
                    break;
                case BOARD_TEST_SDCARD0:
                    ret = ITE_BOARD_TEST_Storage_func(STORAGE_SDCARD);
                    break;
                case BOARD_TEST_DDR:
                    ret = ITE_BOARD_TEST_DDRAM_Func();
                    break;
                case BOARD_TEST_ETHERNET:
                    ret = ITE_BOARD_TEST_ETH_Func();
                    break;
                case BOARD_TEST_USB:
                    ret = ITE_BOARD_TEST_Storage_func(STORAGE_USB);
                    break;
                case BOARD_TEST_SPI:
                    ret = ITE_BOARD_TEST_SPI_Flash_Func();
                    break;
                case BOARD_TEST_GPI:
                    ret = ITE_BOARD_TEST_GPI_Func(0, &result_value);
                    break;
                case BOARD_TEST_GPO:
                    ret = ITE_BOARD_TEST_GPO_Func(test_value);
                    break;
                case BOARD_TEST_RS485:
                    ret = ITE_BOARD_TEST_RS485_Func();
                    break;
                case BOARD_TEST_SECRET:
                    ret = ITE_BOARD_TEST_Secret_Func();
                    break;
                case BOARD_TEST_NAND:
                    ret = ITE_BOARD_TEST_Storage_func(STORAGE_NAND);
                    break;
                case BOARD_TEST_SATA:
                    ret = ITE_BOARD_TEST_Storage_func(STORAGE_SATA);
                    break;
                default:
                    ITE_LOG(ITE_LOG_INFO, "###### %s : line %d invalid cmd %d ######\n", __func__, __LINE__, cmd);
                    break;
            }
            /* 回传测试结果 */
            ret  = ITE_BOARD_TEST_Package_Transfer(fd, cmd, ret, result_value);
            /* 检验回传是否正确*/
            ITE_CHECK_ERROR(ret, 4, return_err, "ITE Board Test Package Transfer Failed\n");
        }
        // usleep(10);
    }
    /* 反初始化设备 */
    ITE_BOARD_TEST_Destory_Func(fd);
    if (package)
    {
        free(package);
    }
    return 0;

return_err:
    ITE_BOARD_TEST_Destory_Func(fd);
    if (package)
    {
        free(package);
    }
    return ret;
}
