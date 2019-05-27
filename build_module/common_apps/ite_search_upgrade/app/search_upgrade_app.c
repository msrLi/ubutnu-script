#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <time.h>
#include <linux/reboot.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <ite_common.h>
#include <ite_osal.h>
#include <ite_hal.h>
#include <ite_middle.h>
#include <ite_hwinfo.h>
#include <ite_version.h>
#include <ITENICP.h>
#include <nicp.h>
#include <ite_msg_key.h>
#include <ite_msg_define.h>

#define ITG_UPGRADE_PORT        8080
#define ITE_ETHERNET_INTERFACE                  ("eth0")
#define ITE_ETHERNET_INTERFACE_1                ("eth1")

extern ITE_IPNC_CMD_OPTIONS Ipnc_Cmd_Options_Table[];

ITE_OSAL_MSG_INFO_T message;

OS_S32 InitSysAdmCon(void)
{
    if (ITE_MESSAGE_Create(SEARCH_MSG_KEY, &message, 0) < 0)
    {
        return -1;
    }
    return 0;
}

OS_HANDLE sigalrm_fn_addr = NULL;
static void sigalrm_fn(OS_S32 sig)
{
    ITE_OSAL_MSG_HDR_T header;
    header.cmd = MSG_SEARCH_SAVE_IPADDR;
    header.des = MSG_CLIENT;
    header.src = MSG_MASTER;
    header.offset = 0;
    header.len = sizeof(IPNC_NetWorkParam);
    ITE_MESSAGE_Snd(&message, &header, sigalrm_fn_addr, sizeof(IPNC_NetWorkParam));
    return ;
}

OS_S32 ItgSysadmSaveIPAddress(OS_HANDLE ptIPAddress)
{
    ITE_OSAL_MSG_HDR_T header;
    /* 初始化定时器 */
    sigalrm_fn_addr = ptIPAddress;
    signal(SIGALRM, sigalrm_fn);

    header.cmd = MSG_SEARCH_SAVE_IPADDR;
    header.des = MSG_MASTER;
    header.src = MSG_CLIENT;
    header.offset = 0;
    header.len = sizeof(IPNC_NetWorkParam);
    ITE_MESSAGE_Lock(&message);
    ITE_MESSAGE_Snd(&message, &header, ptIPAddress, sizeof(IPNC_NetWorkParam));
    memset(&header, 0, sizeof(ITE_OSAL_MSG_HDR_T));
    header.des = MSG_CLIENT;
    /* 启动 6s 定时器 */
    alarm(6);
    ITE_MESSAGE_Get(&message, &header);
    ITE_MESSAGE_Unlock(&message);
    if (header.ret == MSG_OK)
    {
        return 0;
    }
    return -1;
}

ITE_BROADCAST_IPMSG_INFO IPMsgInfo;

static OS_S32 udp_changeip_reply(OS_S8 *buf, struct sockaddr_in srv_addr, struct sockaddr_in cli_addr)
{
    OS_S32  dataSize = sizeof(IPMsgInfo);
    OS_S8 *dataBuf = buf;
    struct sockaddr_in snd_bdc_lcl;
    OS_S32 socket_send;
    OS_S32 ret = 0;

    snd_bdc_lcl.sin_family       = AF_INET;
    snd_bdc_lcl.sin_addr.s_addr  = INADDR_BROADCAST;
    snd_bdc_lcl.sin_port         = cli_addr.sin_port;
    srv_addr.sin_family          = AF_INET;
    ret = ITE_NET_Get_IP_Addr(ITE_ETHERNET_INTERFACE, &srv_addr.sin_addr.s_addr);
    srv_addr.sin_port            = htons(ITE_BRDSND_PORT);
    socklen_t from_len           = sizeof(cli_addr);

    ITE_LOG_Debug(" dataBuf %s\n", dataBuf);

    socket_send                 = socket(AF_INET, SOCK_DGRAM, 0);
    OS_S32 sendon                  = 1;
    setsockopt(socket_send, SOL_SOCKET, SO_BROADCAST, (OS_HANDLE)&sendon, sizeof(sendon));
    bind(socket_send, (struct sockaddr *)&srv_addr, sizeof(struct sockaddr));
    sendto(socket_send, dataBuf, dataSize, 0, (struct sockaddr*)&snd_bdc_lcl, from_len);
    close(socket_send);

    return ret;
}


static OS_S32 udp_restart_reply(OS_S8 *buf, struct sockaddr_in srv_addr, struct sockaddr_in cli_addr)
{
    OS_S32  dataSize = sizeof(IPMsgInfo);
    OS_S8 *dataBuf = buf;
    struct sockaddr_in snd_bdc_lcl;
    OS_S32 socket_send;
    OS_S32 ret = 0;

    snd_bdc_lcl.sin_family       = AF_INET;
    snd_bdc_lcl.sin_addr.s_addr  = INADDR_BROADCAST;
    snd_bdc_lcl.sin_port         = cli_addr.sin_port;
    srv_addr.sin_family          = AF_INET;
    ret = ITE_NET_Get_IP_Addr(ITE_ETHERNET_INTERFACE, &srv_addr.sin_addr.s_addr);
    srv_addr.sin_port            = htons(ITE_BRDSND_PORT);
    socklen_t from_len           = sizeof(cli_addr);

    ITE_LOG_Debug(" dataBuf %s\n", dataBuf);

    socket_send                 = socket(AF_INET, SOCK_DGRAM, 0);
    OS_S32 sendon                  = 1;
    setsockopt(socket_send, SOL_SOCKET, SO_BROADCAST, (OS_HANDLE)&sendon, sizeof(sendon));
    bind(socket_send, (struct sockaddr *)&srv_addr, sizeof(struct sockaddr));
    sendto(socket_send, dataBuf, dataSize, 0, (struct sockaddr*)&snd_bdc_lcl, from_len);
    close(socket_send);

    return ret;
}

/* --------------------------------------------------------------------------*/
/**
 * @brief  Itarge_Get_Device_Info
 *         Read necessary from spi or configurate file
 *
 * @param[out]  answer_info
 *              some filed should be fill before return
 *
 * @return  0->success: 1->fail;
 */
/* --------------------------------------------------------------------------*/
static OS_S32 Itarge_Get_Device_Info(OS_HANDLE info)
{
    IPNC_AnswerInfo *answer_info = (IPNC_AnswerInfo *)info;
    IPNC_DeviceInfoExt device_info;
    static OS_U32 uboot_version = 0;
    static OS_U32 kernel_version = 0;
    static OS_U32 fs_version = 0;
    static OS_U32 app_version = 0;
    static OS_U32 hardware_version = 0;
    static OS_U32 chip_id = 0;
    OS_U32 ddr_fre = 0;
    static OS_U32 fpga_version = 0;
    static OS_U32 boot_mode = -1;
    OS_S32 len = 0;
    OS_S32 ret = 0;

    memset(&device_info, 0, sizeof(IPNC_DeviceInfoExt));
#if 0
    ret = Itarge_Config_Get_DevInfo_Title_Extended(device_info.szDevTitle, &len);
    ret |= Itarge_Config_Get_DevInfo_Number_Extended(device_info.szDevNumber, &len);
    ret |= Itarge_Config_Get_DevInfo_Direction_Extended(device_info.szDevDirection, &len);
    ret |= Itarge_Config_Get_DevInfo_Position_Extended(device_info.szDevPosition, &len);

    if (ret)
    {
        ITE_LOG_Error("Get DevInfo Fail\n");
        return -1;
    }

    ITE_LOG_Debug("Device Info %s,%s,%s,%s\n", device_info.szDevTitle, device_info.szDevNumber,
                  device_info.szDevDirection, device_info.szDevPosition);

    memcpy(answer_info->device_number,    device_info.szDevNumber,   sizeof(answer_info->device_number));
    memcpy(answer_info->device_direction, device_info.szDevDirection, sizeof(answer_info->device_direction));
    memcpy(answer_info->device_position,  device_info.szDevPosition, sizeof(answer_info->device_position));
    memcpy(answer_info->device_type,      device_info.szDevTitle,    sizeof(answer_info->device_type));
#endif

    /* Fill the version info */
    if (boot_mode == -1)
    {
        ret |= ITE_VERSION_Get_Boot_Mode(&boot_mode);
    }

    if ((kernel_version == 0) && (app_version == 0))
    {
        ret |= ITE_VERSION_Get_UBoot_Version(&uboot_version);
        ret |= ITE_VERSION_Get_Kernel_Version(&kernel_version);
        ret |= ITE_VERSION_Get_Filesystem_Version(&fs_version);
        ret |= ITE_VERSION_Get_Firmware_Version(&app_version);
        if (ret)
        {
            ITE_LOG_Error("ITE_VERSION_Get_xxx_Version fail\n");
        }
        if (boot_mode != SYSTEM_BOOT_FROM_NORMAL)
        {
            /*Changed the major_version and minor_version to "1.0" */
            uboot_version &= 0xFFFF;
            uboot_version |= 0x01000000;
            kernel_version &= 0xFFFF;
            kernel_version |= 0x01000000;
            fs_version &= 0xFFFF;
            fs_version |= 0x01000000;
            app_version &= 0xFFFF;
            app_version |= 0x01000000;
        }
    }
    answer_info->uboot_version = uboot_version;
    answer_info->kernel_version = kernel_version;
    answer_info->app_version = app_version;
    ITE_LOG_Debug("uboot_version = 0x%x kernel_version = 0x%x, fs_version = x0%x app_version = 0x%x\n",
                  uboot_version, kernel_version, fs_version, app_version);

    /* Get the hardware id */
    ret = ITE_HWINFO_Get_BoardID(&hardware_version);

    /* 初始化FPGA接口 */
    ret = ITE_FPGA_Init(0);
    /* 获取FPGA固件版本号 */
    ret = ITE_FPGA_Get_Firmware_Version(0, &fpga_version);
    if (ret)
    {
        fpga_version = 0;
    }

    answer_info->hw_id = hardware_version;
    answer_info->fpga_version = fpga_version;
    /* 设置相机类型 */
    /* 0->DM368 : 1->Barrett : 2-Kepler : 3-Kepler 2.0 : 4-TX1(1000) */
    /* 5->HI3519V101(1100) : 6->ASC8852A(1200) 7->HI3559AV100(1300) : 8-HI3519AV100(1400) */
    ITE_HWINFO_Get_Machine_ID(&chip_id);
    if (chip_id >= 1000)
    {
        answer_info->product_type = (chip_id / 100 - 6); /* 新标准直接公式转换 */
    }
    else
    {
        answer_info->product_type = 3;  /* 如果是旧的系统，强制为kepler系列 */
    }
    answer_info->product_type = 4;  /* 如果是旧的系统，强制为kepler系列 */
    ITE_HWINFO_Get_Machine_DDR_Frequency(&ddr_fre); /* DDR频率，windows端会自动乘2后显示 */
    answer_info->ddrMHz = ddr_fre;
    answer_info->armMHz = 1700;    /* CPU 1700MHz */

    return 0;
}

static OS_S32 Itarge_Fill_Search_Info(IPNC_AnswerInfo *info)
{
    IPNC_AnswerInfo answer_info;
    OS_U8 mac_buf[MAC_LENGTH];
    OS_U32 string_length = 0;
    OS_S8 boot_mode[32] = {'\0'};
    OS_S32 ret = 0;
    OS_U32 ublversion = 0;

    memset(&answer_info, 0, sizeof(IPNC_AnswerInfo));

    ret = ITE_NET_Get_IP_Addr(ITE_ETHERNET_INTERFACE, &answer_info.szIPAddr.s_addr);
    if (ret)
    {
        ret = ITE_NET_Get_IP_Addr(ITE_ETHERNET_INTERFACE_1, &answer_info.szIPAddr.s_addr);
    }
    ret = ITE_NET_Get_Subnet_Mask(ITE_ETHERNET_INTERFACE, &answer_info.szSubnetMask.s_addr);
    if (ret)
    {
        ret = ITE_NET_Get_Subnet_Mask(ITE_ETHERNET_INTERFACE, &answer_info.szSubnetMask.s_addr);
    }
    ret = ITE_NET_Get_Gateway(&answer_info.szGateway.s_addr);
    if (ret)
    {
        ret = ITE_NET_Get_Gateway(&answer_info.szGateway.s_addr);
    }
    answer_info.nCtrlPort               = ITE_REP_PORT;

    ret = ITE_NET_Get_MAC_Addr(ITE_ETHERNET_INTERFACE, mac_buf);
    memcpy(answer_info.szMacAddr, mac_buf, MAC_LENGTH);
    answer_info.nTransPort              = 1;

    /* 获取产品信息 */
    ret = ITE_VERSION_Parase_Content_From_File("PRODUCT_VERSION", (OS_S8 *)answer_info.product_version);
    ret |= ITE_VERSION_Parase_Content_From_File("NICP_VERSION", (OS_S8 *)answer_info.nicp_version);
    ret |= ITE_VERSION_Parase_Content_From_File("ALG_VERSION", (OS_S8 *)answer_info.alg_version);
    ret |= ITE_VERSION_Parase_Content_From_File("BUILD_TIME", (OS_S8 *)answer_info.build_time);
    if (ret)
    {
        memset(answer_info.product_version, 0, sizeof(answer_info.product_version));
        memcpy(answer_info.product_version, "V1.00_B01DA00", strlen("V1.00_B01DA00"));
        memset(answer_info.nicp_version, 0, sizeof(answer_info.nicp_version));
        memcpy(answer_info.nicp_version, "NICP_V2.0", strlen("NICP_V2.0"));
        memset(answer_info.alg_version, 0, sizeof(answer_info.alg_version));
        memcpy(answer_info.alg_version, "BI000", strlen("BI000"));
        memset(answer_info.build_time, 0, sizeof(answer_info.build_time));
        memcpy(answer_info.build_time, "2017-06-05 17:16:16 UTC", strlen("2017-06-05 17:16:16 UTC"));
    }

    /* 获取一级uboot版本信息*/
    ret = ITE_VERSION_Get_UBL_Version(&ublversion);
    if (0 == ret)
    {
        /* 添加到product_version后面*/
        ublversion &= 0xFFFF;
        if ((sizeof(answer_info.product_version) - strlen((OS_S8 *)answer_info.product_version)) > 6)
        {
            sprintf((OS_S8 *)answer_info.product_version, "%s_%d\n", (OS_S8 *)answer_info.product_version, ublversion);
        }
    }

    /* Fill the Sensor Mode */
    ret |= Itarge_Get_Device_Info(&answer_info);
    /* Fill the product_name */
    string_length = sizeof(answer_info.product_name);
    memset(answer_info.product_name, 0, string_length);
    ret |= ITE_HWINFO_Get_Board_Name((OS_S8 *)answer_info.product_name, &string_length);
    ret |= ITE_SYSINFO_Get_Boot_Mode(boot_mode);
    if (!strcmp(boot_mode, "RECOVERY"))
    {
        strcat((OS_S8 *)answer_info.product_name, "-RECOVERY");
    }
    ITE_LOG_Debug("Respond PC ip: %08x\n", answer_info.szIPAddr.s_addr);
    memcpy(info, &answer_info, sizeof(IPNC_AnswerInfo));

    return ret;
}

/* 地址字符串转换为整数 */
void nicp_net_stoi(OS_S8 *addr_src, OS_U32 *addr_dst, OS_S8 *str)
{
    OS_U32 i = 0, addr_i[4] = {0};

    sscanf(addr_src, "%u.%u.%u.%u", &addr_i[3], &addr_i[2], &addr_i[1], &addr_i[0]);

    for (i = 0; i < sizeof(OS_U32); i++)
    {
        *addr_dst += addr_i[i] << (i * 8);
    }

    //ITE_LOG(ITE_LOG_INFO, "================== >>>>> [%s]  addr_src[%s], addr_dst[%d.%d.%d.%d]\n", str,
    //addr_src, addr_i[3], addr_i[2], addr_i[1], addr_i[0]);
}
/* 判断ip地址、子网掩码、网关正确性 */
OS_S32 nicp_net_addr_is_valid(OS_S8 *ip_addr, OS_S8 *subnet, OS_S8 *gateway)
{
    OS_S32 ret = ITE_OSAL_FAIL, i = 0;
    OS_U8 ip_class = 0;
    OS_U32 ip_i = 0, subnet_i = 0, gateway_i = 0;
    OS_U32 host_num = 0;

    /* 将ip地址串存储到32位整数中，必须是小端存储 */
    nicp_net_stoi(ip_addr, &ip_i, "ip_addr");
    nicp_net_stoi(subnet, &subnet_i, "subnet");
    nicp_net_stoi(gateway, &gateway_i, "gateway");

    do
    {
        /* 判断地址是否属于A、B、C类 */
        if ((ip_i >> 24) > 0 && (ip_i >> 24) < 127)
        {
            ip_class = 0;// A类
        }
        else if ((ip_i >> 24) > 127 && (ip_i >> 24) <= 191)
        {
            ip_class = 1;// B类
        }
        else if ((ip_i >> 24) > 191 && (ip_i >> 24) <= 223)
        {
            ip_class = 2;// C类
        }
        else
        {
            ITE_LOG(ITE_LOG_ERROR, "IP %s is not a A/B/C 类 IP address\n", ip_addr);
            break;
        }

        /* 判断子网掩码有效性 */
        //if(((subnet_i - 1) | subnet_i) != ~0)
        if ((0 == ~subnet_i) || (((~subnet_i + 1) & (~subnet_i)) != 0))
        {
            ITE_LOG(ITE_LOG_ERROR, "SUBNET MASK %s is not a valid Address\n", subnet);
            break;
        }

        /* ip地址主机号不能全为0或1 */
        host_num = (~subnet_i & ip_i);
        if (0 == host_num || 255 == host_num)
        {
            ITE_LOG(ITE_LOG_ERROR, "IP address %s and SUBNET MASK %s do not match, host num > 0 && host num < 255\n", ip_addr, subnet);
            break;
        }

        /* ip地址与网关不匹配 */
        if ((ip_i & subnet_i) != (gateway_i & subnet_i))
        {
            ITE_LOG(ITE_LOG_ERROR, "IP address %s and GATEWAY %s do not match\n", ip_addr, gateway);
            break;
        }

        ret = ITE_OSAL_OK;
    }
    while (0);

    return ret;
}

/* 网络配置校验 */
OS_S32 net_info_verify(OS_S8 *ifname, struct in_addr ip, struct in_addr netmask, struct in_addr gateway)
{
    OS_S32 ret = ITE_OSAL_FAIL;
    OS_S8 ip_buf[20] = {0}, subnet_buf[20] = {0}, gateway_buf[20] = {0};

    do
    {
        /* ip地址 */
        if (NULL == inet_ntop(AF_INET, (void *)&ip, ip_buf, 16))
        {
            ITE_LOG(ITE_LOG_ERROR, "IP %s is not a valid address\n", ip_buf);
            break;
        }

        /* 子网掩码 */
        if (NULL == inet_ntop(AF_INET, (void *)&netmask, subnet_buf, 16))
        {
            ITE_LOG(ITE_LOG_ERROR, "SUBNET MASK %s is not a valid address\n", subnet_buf);
            break;
        }

        /* 网关 */
        if (NULL == inet_ntop(AF_INET, (void *)&gateway, gateway_buf, 16))
        {
            ITE_LOG(ITE_LOG_ERROR, "GATEWAY %s is not a valid address\n", gateway_buf);
            break;
        }

        ret = nicp_net_addr_is_valid(ip_buf, subnet_buf, gateway_buf);
        if (ret != 0)
        {
            break;
        }

        ret = ITE_OSAL_OK;
    }
    while (0);

    return ret;
}

OS_HANDLE nicp_search_thread(OS_HANDLE pData)
{
    struct sockaddr_in search_update, cli_addr;
    OS_S32 brdc_socket;
    socklen_t cli_len;
    struct in_addr local_ip;

    OS_S8 *p_answer_header       = "IPNC_AnswerInfo=";
    // OS_S8 *p_cmd_answer_header   = "IPNC_RE_CMD=";
    OS_S8 *p_send_ask_buff = NULL;
    OS_S8 *command_header_addr   = NULL;

    OS_S8 rcv_buf[BC_MAX_SIZE];
    IPNC_AnswerInfo answer_info;
    ITE_NET_IP_INFO ip_info;
    OS_S32 on = 1;
    OS_S32 result;
    OS_S32 change_ip = 0;

    p_send_ask_buff         = malloc(strlen(p_answer_header) + sizeof(IPNC_AnswerInfo));
    search_update.sin_family  = AF_INET;
    search_update.sin_addr.s_addr = INADDR_BROADCAST;
    search_update.sin_port    = htons(ITE_SEARCH_PORT);

    brdc_socket = socket(AF_INET, SOCK_DGRAM, 0);
    setsockopt(brdc_socket, SOL_SOCKET, SO_BROADCAST, (OS_HANDLE)&on, sizeof(on));
    bind(brdc_socket, (struct sockaddr *)&search_update, sizeof(struct sockaddr_in));
    cli_len         = sizeof(struct sockaddr_in);
    result = ITE_NET_Get_IP_Addr(ITE_ETHERNET_INTERFACE, &local_ip.s_addr);
    pthread_detach(pthread_self());
    while (1)
    {
        memset(rcv_buf, '\0', sizeof(rcv_buf));
        result = recvfrom(brdc_socket, rcv_buf, sizeof(rcv_buf), 0, (struct sockaddr *)&cli_addr, &cli_len);
        /* Once the upgrade process worked, the search pthread should not respond any request */
        if (ITE_UPGRADE_Is_System_Upgrading())
        {
            continue;
        }

        /**************************for search info************************************/
        if (strncmp(rcv_buf, DAGUERRE_BC_SEARCH_HEADER, strlen(DAGUERRE_BC_SEARCH_HEADER)) == 0)
        {
            OS_S32 brdc_snd;
            struct sockaddr_in brdc_rcv_addr, brdc_local_addr;
            brdc_rcv_addr.sin_family        = AF_INET;
            brdc_rcv_addr.sin_addr.s_addr   = INADDR_BROADCAST;
            brdc_rcv_addr.sin_port          = cli_addr.sin_port;
            brdc_local_addr.sin_family      = AF_INET;
            result = ITE_NET_Get_IP_Addr(ITE_ETHERNET_INTERFACE, &brdc_local_addr.sin_addr.s_addr);
            brdc_local_addr.sin_port        = htons(ITE_BRDSND_PORT);
            brdc_snd = socket(AF_INET, SOCK_DGRAM, 0);
            OS_S32 sendon = 1;
            OS_S32 tmp_len_1 = strlen(p_answer_header);
            Itarge_Fill_Search_Info(&answer_info);
            memcpy(p_send_ask_buff, p_answer_header, tmp_len_1);
            OS_S32 tmp_len_2 = sizeof(IPNC_AnswerInfo);
            memcpy(p_send_ask_buff + tmp_len_1, &answer_info, sizeof(IPNC_AnswerInfo));
            OS_S32 tmp_len_0 = tmp_len_1 + tmp_len_2;
            result = setsockopt(brdc_snd, SOL_SOCKET, SO_BROADCAST, (OS_HANDLE)&sendon, sizeof(sendon));
            result = bind(brdc_snd, (struct sockaddr *)&brdc_local_addr, sizeof(struct sockaddr_in));
            result = sendto(brdc_snd, p_send_ask_buff, tmp_len_0, 0, (struct sockaddr*)&brdc_rcv_addr, cli_len);
            close(brdc_snd);
            continue;
        }

        /**************************for change IP address************************************/
        /* We will deal with two kinds of change IP protocol */
        /* change the ip address, but not save */
        command_header_addr = strstr(rcv_buf, DAGUERRE_BC_CHANGE_IP);
        if (command_header_addr)
        {
            Itarge_Fill_Search_Info(&answer_info);
            memcpy(&IPMsgInfo, command_header_addr, sizeof(IPMsgInfo));
            if (memcmp(IPMsgInfo.ChangeIP.MacAddr, answer_info.szMacAddr, 6) != 0)
            {
                continue;
            }
            memcpy(&ip_info, &IPMsgInfo.ChangeIP, sizeof(ip_info));
            change_ip = net_info_verify(ITE_ETHERNET_INTERFACE, ip_info.ip, ip_info.netmask, ip_info.gateway);
            if (change_ip)
            {
                continue;
            }
            change_ip |= ITE_NET_Set_IP_Addr(ITE_ETHERNET_INTERFACE,  ip_info.ip.s_addr);
            change_ip |= ITE_NET_Set_Subnet_Mask(ITE_ETHERNET_INTERFACE, ip_info.netmask.s_addr);
            change_ip |= ITE_NET_Set_Gateway(ip_info.gateway.s_addr);

            if (change_ip)
            {
                ITE_LOG_Error("Change IP address failed\n");
                continue;
            }
            else
            {
                ItgSysadmSaveIPAddress((OS_HANDLE)&ip_info);
            }
            udp_changeip_reply(command_header_addr, search_update, cli_addr);
            continue;
        }
        /* change the ip address, save */
        command_header_addr = strstr(rcv_buf, DAGUERRE_BC_ITARGE_CHANGE_IP);
        if (command_header_addr)
        {
            Itarge_Fill_Search_Info(&answer_info);
            memcpy(&IPMsgInfo, command_header_addr, sizeof(IPMsgInfo));
            if (memcmp(IPMsgInfo.ChangeIP.MacAddr, answer_info.szMacAddr, 6) != 0)
            {
                continue;
            }
            memcpy(&ip_info, &IPMsgInfo.ChangeIP, sizeof(ip_info));
            change_ip = net_info_verify(ITE_ETHERNET_INTERFACE, ip_info.ip, ip_info.netmask, ip_info.gateway);
            if (change_ip)
            {
                continue;
            }
            change_ip |= ITE_NET_Set_IP_Addr(ITE_ETHERNET_INTERFACE,  ip_info.ip.s_addr);
            change_ip |= ITE_NET_Set_Subnet_Mask(ITE_ETHERNET_INTERFACE, ip_info.netmask.s_addr);
            change_ip |= ITE_NET_Set_Gateway(ip_info.gateway.s_addr);

            if (change_ip)
            {
                ITE_LOG_Error("Change IP address failed\n");
                continue;
            }
            else
            {
                ItgSysadmSaveIPAddress((OS_HANDLE)&ip_info);
            }
            udp_changeip_reply(command_header_addr, search_update, cli_addr);
            continue;
        }


        /**************************for reboot************************************/
        command_header_addr = strstr(rcv_buf, DAGUERRE_BC_NICP_RESTART);
        if (command_header_addr)
        {
            IPNC_ChangeIP   restartIp;
            Itarge_Fill_Search_Info(&answer_info);

            memcpy(&restartIp, command_header_addr + strlen(DAGUERRE_BC_NICP_RESTART), sizeof(IPNC_ChangeIP));
            ITE_LOG_Info("Recieved broadcast reboot packet\n");
            ITE_LOG_Info("IP:     %s\n",  inet_ntoa(restartIp.IPAddr));
            ITE_LOG_Info("NetMask:%s\n",  inet_ntoa(restartIp.NetMask));
            ITE_LOG_Info("GateWay:%s\n",  inet_ntoa(restartIp.Gateway));

            if (memcmp(restartIp.MacAddr, answer_info.szMacAddr, 6) != 0)
            {
                continue;
            }
            udp_restart_reply(command_header_addr, search_update, cli_addr);
            ITE_LOG_Info("Reboot caused by broadcast packet\n");

            ITE_UPGRADE_Kill_Apps();
            while (1)
            {
#if 0
                system("fpga_test  0x0c 0x0");
                system("fpga_test  0x0c 0x0");
                system("fpga_test  0x0c 0x0");//system("reboot");
                system("fpga_test  0x0c 0x0");
                system("fpga_test  0x0c 0x0");
                system("fpga_test  0x0c 0x0");//system("reboot");
#endif
                system("reboot");

                sleep(1);
            }
            sleep(1);
            system("reboot");
            sleep(3);
            continue;
        }
    }
    return NULL;
}

OS_HANDLE nicp_connect_thread(OS_HANDLE pData)
{
    OS_S32 server_sock_fd, client_sock_fd = -1;
    struct sockaddr_in server_addr;
    OS_S8 connect_buf[256];
    OS_S32  send_byt, data_cnt;
    OS_S32  server_port = ITE_STATIC_SRV_PORT;
    OS_S32  on = 1;
    OS_S8 *answer_header = "IPNC_AnswerInfo=";
    OS_S8 *p_retun_buff = NULL;
    IPNC_AnswerInfo AnswerInfo;

    /*********************variable for select*******************************/
    socklen_t           socklen = sizeof(struct sockaddr_in);
    OS_S32                 maxfd           = 0;
    struct timeval      timeout;
    fd_set              readset;
    OS_S32                 result          = -1;

    pthread_detach(pthread_self());

    server_sock_fd              = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family      = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port        = htons(server_port);
    setsockopt(server_sock_fd, SOL_SOCKET, SO_REUSEADDR, (OS_HANDLE)&on, sizeof(on));

    if (bind(server_sock_fd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr_in)) == -1)
    {
        close(server_sock_fd);
        return NULL;
    }
    if (listen(server_sock_fd, 5) < 0)
    {
        close(server_sock_fd);
        return NULL;
    }
    p_retun_buff = malloc(strlen(answer_header) + sizeof(IPNC_AnswerInfo));
    maxfd = maxfd > server_sock_fd ? maxfd : server_sock_fd;
    while (1)
    {
        memset(connect_buf, 0, 256);
        FD_ZERO(&readset);
        FD_SET(server_sock_fd,  &readset);
        timeout.tv_sec  = 0;
        timeout.tv_usec = 50000;
        result = select(maxfd + 1, &readset, NULL, NULL, &timeout);
        if (result > 0)
        {
            if (FD_ISSET(server_sock_fd,  &readset))
            {
                client_sock_fd = accept(server_sock_fd, (struct sockaddr *)&server_addr, &socklen);
                if (client_sock_fd > 0)
                {
                    data_cnt = recv(client_sock_fd, connect_buf, 256, 0);
                    if (data_cnt <= 0)
                    {
                        close(client_sock_fd);
                        client_sock_fd = -1;
                        continue;
                    }
                    if (strncmp(connect_buf, DAGUERRE_BC_SEARCH_HEADER, strlen(DAGUERRE_BC_SEARCH_HEADER)) == 0)
                    {
                        Itarge_Fill_Search_Info(&AnswerInfo);
                        memcpy(p_retun_buff, &AnswerInfo, sizeof(IPNC_AnswerInfo));
                        data_cnt = sizeof(IPNC_AnswerInfo);//+strlen(answer_header);
                        send_byt = 0;
                        send(client_sock_fd, p_retun_buff, data_cnt, 0);
                        /*
                        data_cnt = data_cnt;
                        while (send_byt != data_cnt) {
                            send_byt += send(client_sock_fd, p_retun_buff + send_byt, data_cnt - send_byt, 0);
                        }
                        */
                        //sleep(2);
                    }
                    close(client_sock_fd);
                    //close(server_sock_fd);
                    //free(p_retun_buff);
                }
            }
        }
        else
        {
            //ITE_LOG_Debug("========================Nicp_Connect_Thread[select result = %d, maxfd = %d]===============================\n", result, maxfd);
        }
    }
    close(server_sock_fd);
    free(p_retun_buff);
    return NULL;
}

static OS_S32 svrProc(OS_HANDLE link, OS_HANDLE arg)
{
    OS_HANDLE session;
    ITE_HASHCMD_SESSION_HANDLE_T shnd;

    shnd.privhnd = link;
    shnd.read    = ITE_NET_Socket_Receive;
    shnd.write   = ITE_NET_Socket_Send;
    shnd.fault   = NULL;

    if (0 != ITE_HASHCMD_Session_Create(&session, &shnd))
    {
        return -1;
    }

    ITE_HASHCMD_Session_Process(session);

    ITE_HASHCMD_Session_Delete(session);

    return 0;
}

OS_HANDLE nicp_upgrade_thread(OS_HANDLE pData)
{
    OS_HANDLE svr;
    ITE_HASHCMD_PARAM_T cmdInitParam;
    OS_S32 idx;

    cmdInitParam.cmd_max      = 300;
    cmdInitParam.session_max  = 10;
    cmdInitParam.session_timeout = 60000;
    pthread_detach(pthread_self());

    ITE_HASHCMD_Init(&cmdInitParam);
    for (idx = 0; idx < 4; idx++)
    {
        if (Ipnc_Cmd_Options_Table[idx].ITE_HASHCMD_Handle != NULL)
        {
            ITE_HASHCMD_Register(Ipnc_Cmd_Options_Table[idx].cmd, 0, 0, Ipnc_Cmd_Options_Table[idx].ITE_HASHCMD_Handle);
        }
    }

    /* Only one client can connect this port */
    ITE_LOG(ITE_LOG_INFO, "@@@@@@@@ Begin to listen the port [ %d ] @@@@@@@@\n", ITG_UPGRADE_PORT);
    if (0 != ITE_NET_Create_TCP_Server(&svr, NULL, ITG_UPGRADE_PORT, 1))
    {
        ITE_LOG_Error("ITE_NET_Create_TCP_Server fail\n");
        return NULL;
    }

    ITE_NET_Socket_Process(svr, svrProc, NULL);
    ITE_NET_Socket_Delete(svr);

    return NULL;
}
