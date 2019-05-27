#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sched.h>
#include <pthread.h>
#include <sys/types.h>
#include <signal.h>
#include <ite_common.h>
#include <ite_log.h>
#include <ite_middle.h>
#include <ITENICP.h>
#include <nicp.h>

/* --------------------------------------------------------------------------*/
/**
 * @brief  Itarge_Camera_Reboot
 *         reboot the system
 *
 * @param[out]  pData
 *              not used
 *
 * @return    NULL
 */
/* --------------------------------------------------------------------------*/
static OS_HANDLE Itarge_Camera_Reboot(OS_HANDLE pData)
{
    pthread_detach(pthread_self());
    sleep(2);
    sync();
#if 0
    while (1)
    {
        system("fpga_test  0x0c 0x0");
        system("fpga_test  0x0c 0x0");
        system("fpga_test  0x0c 0x0");//system("reboot");
        system("fpga_test  0x0c 0x0");
        system("fpga_test  0x0c 0x0");
        system("fpga_test  0x0c 0x0");//system("reboot");
        sleep(1);
    }
#endif
    sleep(1);
    system("reboot");
    return NULL;
}

/* --------------------------------------------------------------------------*/
/**
 * @brief  Itarge_Upgrade_Respond
 *         respond PC the upgrade information
 *
 * @param[out]  outbuf
 *             The upgrade respond content send to PC
 *
 * @return    0->success : the others->failure
 */
/* --------------------------------------------------------------------------*/
static OS_S32 Itarge_Upgrade_Respond(OS_HANDLE inbuf, OS_HANDLE outbuf)
{

    ITE_UPGRADE_REQUEST_INFO_T *upgrade_info = (ITE_UPGRADE_REQUEST_INFO_T *)inbuf;
    ITE_UPGRADE_RESPOND_INFO_T *respond_info = (ITE_UPGRADE_RESPOND_INFO_T *)outbuf;
    OS_U32 i = 0;

    respond_info->file_num = upgrade_info->updatefileinfo.upgrade_head_info.file_number;

    for (i = 0; i < respond_info->file_num; i++)
    {
        memset(respond_info->name[i], 0, (sizeof(OS_S8) * PER_INFO_SIZE));
        memcpy(respond_info->name[i],
               upgrade_info->updatefileinfo.upgrade_head_info.file_info[i].name,
               sizeof(OS_S8)*PER_INFO_SIZE);
        ITE_LOG(ITE_LOG_INFO, "Upgrade name[%d] = %s\n", i, respond_info->name[i]);
    }

    return 0;
}

static OS_S32 nicp_upgrade_system(OS_HANDLE session, OS_HANDLE inbuf, OS_U32 inlen, OS_HANDLE outbuf, OS_S32 *outlen)
{
    ITE_CMD_Struct *resp = (ITE_CMD_Struct *)outbuf;
    ITE_CMD_Struct *inp  = (ITE_CMD_Struct *)inbuf;
    OS_S32 ret = 0;
    pthread_t id;

    memcpy(resp, inp, sizeof(ITE_CMD_Struct));
    ret = Itarge_Upgrade_Respond(inp->body, resp->body);
    resp->head.Ver    = eNICPVer11;
    resp->head.Len    = sizeof(ITE_UPGRADE_RESPOND_INFO_T);
    resp->head.Result = (ret == 0) ? HANDLE_OK_CMD : HANDLE_ERROR_CMD;
    //first send receive OK response
    ITE_HASHCMD_Session_Send(session, resp, sizeof(ITE_CMD_Struct) + resp->head.Len, 1000);

    ITE_UPGRADE_Kill_Apps();

    ret = pthread_create(&id, 0, (OS_HANDLE)ITE_UPGRADE_System, inp->body);
    //ret = ITE_UPGRADE_System(inp->body);
    resp->head.Ver      = eNICPVer11;
    resp->head.Len      = 0;
    resp->head.Result   = HANG_IGNORE_CMD;
    *outlen             = resp->head.Len + sizeof(ITE_CMD_Struct);
    return 0;
}

static OS_S32 nicp_upgrade_status(OS_HANDLE session, OS_HANDLE inbuf, OS_U32 inlen, OS_HANDLE outbuf, OS_S32 *outlen)
{
    ITE_CMD_Struct *resp = (ITE_CMD_Struct *)outbuf;
    ITE_CMD_Struct *inp  = (ITE_CMD_Struct *)inbuf;
    OS_S32 ret = 0;

    memcpy(resp, inp, sizeof(ITE_NICP_HEAD));
    resp->head.Ver      = eNICPVer11;
    resp->head.Len      = 0;
    ret = ITE_UPGRADE_Receive_Percentage(0, &resp->head.Data);
    resp->head.Result   = (ret == 0) ? HANDLE_OK_CMD : HANDLE_ERROR_CMD;
    *outlen             = resp->head.Len + sizeof(ITE_CMD_Struct);
    return 0;
}

static OS_S32 nicp_reboot_device(OS_HANDLE session, OS_HANDLE inbuf, OS_U32 inlen, OS_HANDLE outbuf, OS_S32 *outlen)
{
    pthread_t id;
    ITE_CMD_Struct *resp = (ITE_CMD_Struct *)outbuf;
    ITE_CMD_Struct *inp  = (ITE_CMD_Struct *)inbuf;
    OS_S32 ret = 0;

    memcpy(resp, inp, sizeof(ITE_NICP_HEAD));
    resp->head.Ver      = eNICPVer11;
    resp->head.Len      = 0;
    resp->head.Result   = RECV_OK_CMD;

    ITE_HASHCMD_Session_Send(session, resp, sizeof(ITE_CMD_Struct) + resp->head.Len, 50);

    ret = pthread_create(&id, 0, Itarge_Camera_Reboot, NULL);
    resp->head.Result   = (ret == 0) ? HANDLE_OK_CMD : HANDLE_ERROR_CMD;
    *outlen      = resp->head.Len + sizeof(ITE_CMD_Struct);

    return 0;
}

/* =========================================================================== */
ITE_IPNC_CMD_OPTIONS Ipnc_Cmd_Options_Table[] =
{
    {SET_UPDATE_INFO,       nicp_upgrade_system,        NULL, 0, NULL},         // 4022
    {GET_UPDATE_STATUS,     nicp_upgrade_status,        NULL, 0, NULL},         // 4023
    {SET_UPDATE_RESTART,    nicp_reboot_device,         NULL, 0, NULL}          // 4024
};
