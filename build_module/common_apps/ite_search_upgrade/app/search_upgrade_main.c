#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <sys/stat.h>
#include <syslog.h>
#include <sys/types.h>
#include <ite_common.h>
#include <ite_osal.h>
#include <ite_middle.h>

/* 用于程序退出 */
OS_U32 g_bTerminate = 0;

static void Main_Handle_Signal(OS_S32 signum)
{
    switch (signum)
    {
        case SIGUSR1:
            /* 将打印信息输出级别修改为全开 */
            ITE_LOG_Set_Level(ITE_LOG_DEBUG);
            break;
        case SIGHUP:
        case SIGINT:
        case SIGKILL:
        case SIGTERM:
            g_bTerminate = 1;
            break;
        default:
            break;
    }
    return;
}

static void Main_Init_Signals(void)
{
    struct sigaction sigaInst;
    sigemptyset(&sigaInst.sa_mask);
    sigaInst.sa_flags = SA_RESTART;

    sigaddset(&sigaInst.sa_mask, SIGTERM);
    sigaddset(&sigaInst.sa_mask, SIGINT);
    sigaddset(&sigaInst.sa_mask, SIGHUP);
    sigaddset(&sigaInst.sa_mask, SIGUSR1);

    sigaInst.sa_handler = Main_Handle_Signal;
    sigaction(SIGTERM, &sigaInst, NULL);
    sigaction(SIGINT, &sigaInst, NULL);
    sigaction(SIGHUP, &sigaInst, NULL);
    sigaction(SIGUSR1, &sigaInst, NULL);
}

static void print_usage(OS_S8 *app_name)
{
    ITE_LOG(ITE_LOG_INFO, "search_daemon process\n"
            "Usage:\n"
            "    %s [-D] [-h] [-t waittime] [-p pidfile]\n"
            "Options:\n"
            "    -D                 Run as Daemon\n"
            "    -t wait time       wait some seconds to begin execute\n"
            "    -p pid_file        Write PID to this file\n"
            "    -h                 This help\n", app_name);
}

OS_HANDLE nicp_search_thread(OS_HANDLE pData);
OS_HANDLE nicp_upgrade_thread(OS_HANDLE pData);
OS_HANDLE nicp_connect_thread(OS_HANDLE pData);
extern OS_S32 InitSysAdmCon(void);

OS_S32 main(OS_S32 argc, OS_S8 *argv[])
{
    pthread_t tsk_id[4];
    static OS_S8 *szPidFile = NULL;
    OS_S32 param = 0;
    OS_S32 retval = 0;
    OS_U32 wait_time = 1;
    OS_U32 bIsDaemon = 0;
    OS_S32 ret = 0;

    g_bTerminate = 0;

    while ((param = getopt(argc, argv, "D:p:t:h")) != -1)
    {
        switch (param)
        {
            case 'D':
                bIsDaemon = 1;
                break;
            case 'p':
                szPidFile = strdup(optarg);
                break;
            case 't':
                wait_time = atoi(optarg);
                break;
            case 'h':
            default:
                print_usage(argv[0]);
                exit(1);
        }
    }

    /* run as Daemon */
    if (bIsDaemon)
    {
        ITE_PID_Daemonize();
    }
    // If the szPidFile is assigned write pid to szPidFile
    if (szPidFile != NULL)
    {
        if (ITE_PID_Gen_PID_File(szPidFile))
        {
            ITE_LOG_Error("[ITE_SEARCH_UPGRADE] Generate pid file [%s] fail!\n", szPidFile);
            goto exit;
        }
    }
    //ITE_LOG_Set_Level(ITE_LOG_DEBUG);

    /* init the signal process */
    Main_Init_Signals();
    InitSysAdmCon();

    if (wait_time)
    {
        sleep(wait_time);
    }

    //retval  = pthread_create(&tsk_id[0], 0, Itarge_Upgrade_Recieve_File_Thread,  NULL);
    retval |= pthread_create(&tsk_id[1], 0, nicp_upgrade_thread,  NULL);
    retval |= pthread_create(&tsk_id[2], 0, nicp_connect_thread,  NULL);
    retval |= pthread_create(&tsk_id[3], 0, nicp_search_thread,   NULL);

    /* 防止退出 */
    while (g_bTerminate == 0)
    {
        sleep(1);
    }

exit:
    if (szPidFile != NULL)
    {
        ITE_PID_Del_PID_File(szPidFile);
        free(szPidFile);
        szPidFile = NULL;
    }

    ITE_LOG(ITE_LOG_INFO, "[ITE_SEARCH_UPGRADE] Stop process !!\n");
    return 0;
}
