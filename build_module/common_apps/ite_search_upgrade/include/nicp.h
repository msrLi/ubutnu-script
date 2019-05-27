#ifndef __ITE_NICP_H_
#define __ITE_NICP_H_

#define ITE_BRDSND_PORT              (34569)
#define ITE_SEARCH_PORT              (34565)
#define ITE_STATIC_SRV_PORT          (33565)
#define ITE_REP_PORT                 (34561)
#define MAX_NICP_PTHREAD             (8)
#define BC_MAX_SIZE                  (1024)
#define COND_WAIT_TIMEOUT            (250)
#define STREAM_HEART_BEAT_INTERVAL   (500)

enum
{
    NICP_STATU0 = 0,
    NICP_RECEIVE_ERR,
    NICP_NO_COMMAND,
    NICP_RECV_TIMEOUT,
    NICP_SEND_TIMEOUT,
    NICP_COMMAND_HAS_CONTENT,
    NICP_COMMAND_NO_CONTENT,
    NICP_CMD_END
};

typedef struct
{
    OS_U8 FLAG_CHANGEIP[21]; //"iTarge_IPNC_Change_IP" 21?ֽ?
    IPNC_ChangeIP   ChangeIP;
} ITE_BROADCAST_IPMSG_INFO;//BROADCAST_IPMSG_INFO


typedef struct
{
    struct in_addr  ip; ///< IP address in static IP mode
    struct in_addr  netmask; ///< netmask in static IP mode
    struct in_addr  gateway; ///< gateway in static IP mode
} ITE_NET_IP_INFO; //net_ip_info

typedef struct _ITE_IPNC_CMD_OPTIONS
{
    ENICP_CMD   cmd;
    OS_S32(*ITE_HASHCMD_Handle)(OS_HANDLE session, OS_HANDLE inbuf, OS_U32 inlen, OS_HANDLE outbuf, OS_S32 *outlen);
    void        *pData;
    OS_S32      reserved;
    struct _ITE_IPNC_CMD_OPTIONS *next;
} ITE_IPNC_CMD_OPTIONS;

typedef struct _ITE_CMD_Struct
{
    ITE_NICP_HEAD   head;
    OS_U8           body[0];
} ITE_CMD_Struct;

typedef struct time_wyj_t
{
    struct timeval a_time;
    OS_S32 w_zone;
} time_wyj;

#endif

