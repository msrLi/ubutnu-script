#ifndef _ITE_CONFIG_INFO_H_
#define _ITE_CONFIG_INFO_H_
#include <ITENetSDK.h>
#include <ITEAewb.h>
#include <ITENITP.h>
#include <ITENICP.h>




#define ITE_ETHERNET_INTERFACE                  "eth0"
#define ITE_ETHERNET_INTERFACE_1              "eth1"



/******************************************************************************
* alg config storage location
******************************************************************************/
#define ITE_CONFIG_INFO_PATH            "/opt/config/"// /configs/sys/"//"/mnt/nand"

#define ITE_EXT_ALGOSD_INFO             "alg_osd.ini"
#define ITE_EXT_FILENAME_INFO          "ext_filename.ini"



/******************************************************************************
* alg storage device location
******************************************************************************/
#define RAMDISK_TMPFS_DIR        "/mnt/ramdisk/"
#define TMP_JPEG_FOLDER          "/mnt/ramdisk/pic/"
#define USER_JPEG_FOLDER         "/mnt/ramdisk/userpic/"
#define TMP_RAW_FOLDER           "/mnt/ramdisk/raw/"
#define RAMDISK_CTCP_DIR         "/mnt/ramdisk/ctcp/"

#define MMC_MOUNT_DIR            "/mnt/sd"
#define DECT_HDD_MOUNT_DIR       "/mnt/sata/sda1"
#define HDD_MOUNT_DIR            "/mnt/sata/sda1/bak"
#define HDD_HISTORY_DATA_DIR     "/mnt/sata/sda1/historydata"



/******************************************************************************
* frame info
******************************************************************************/
#define FRAME_WIDTH_1080P 1920
#define FRAME_PITCH_1080P 1920
#define FRAME_HEIGHT_1080P 1080

#define FRAME_WIDTH_8M 3840
#define FRAME_PITCH_8M 3840
#define FRAME_HEIGHT_8M 2160

#define FRAME_WIDTH_4096 4096
#define FRAME_HEIGHT_2160 2160

/******************************************************************************
* the info for boot count
******************************************************************************/

enum
{
    BOOT_PARAM_FROM_ERR = -1,
    BOOT_PARAM_FROM_ARM = 0,
    //BOOT_PARAM_FROM_DEF,
};

enum
{
    BOOT_PARAM_ERR = -1,
    BOOT_PARAM_STD = 0,
    BOOT_PARAM_DEF,
};

enum
{
    RESET_PARAM_FROM_ERR = -1,
    RESET_PARAM_FROM_ARM = 0,
    RESET_PARAM_FROM_INIT,
};




#define OFFSETOF(s,m) (size_t)&(((s *)0)->m)

#define ITE_HEART_BEAT_SLICE            (1000)

enum
{
    MARK_CONFIG_SYS_DEV_PARAM,
    MARK_CONFIG_SYS_COMM_PARAM,
    MARK_CONFIG_SYS_ALG1_PARAM,
    MARK_CONFIG_SYS_ALG2_PARAM,
    MARK_CONFIG_SYS_ALG3_PARAM,
    MARK_CONFIG_STATUS_MAX,
};

#define ITE_CONFIG_FILE_MAGIC         "ITARGE-MAGIC"
#define ITE_CONFIG_VERSION               "CONFIG-V1.0"

#define ITE_CONFIG_FILE_NAME_DEVICE ITE_CONFIG_INFO_PATH"sysdevice.ini"
#define ITE_CONFIG_FILE_NAME_COMM   ITE_CONFIG_INFO_PATH"syscomm.ini"
#define ITE_CONFIG_FILE_NAME_ALG1    ITE_CONFIG_INFO_PATH"alg_config1.ini"
#define ITE_CONFIG_FILE_NAME_ALG2    ITE_CONFIG_INFO_PATH"alg_config2.ini"
#define ITE_CONFIG_FILE_NAME_ALG3    ITE_CONFIG_INFO_PATH"alg_config3.ini"

#define ITE_CONFIG_FILE_NAME_ALG1_DEF    ITE_CONFIG_INFO_PATH"alg_def_config1.ini"
#define ITE_CONFIG_FILE_NAME_ALG2_DEF    ITE_CONFIG_INFO_PATH"alg_def_config2.ini"
#define ITE_CONFIG_FILE_NAME_ALG3_DEF    ITE_CONFIG_INFO_PATH"alg_def_config3.ini"

#define ITE_CONFIG_FILE_NAME_CPALG1    ITE_CONFIG_INFO_PATH"cp_alg1.ini"
#define ITE_CONFIG_FILE_NAME_CPALG2    ITE_CONFIG_INFO_PATH"cp_alg2.ini"
#define ITE_CONFIG_FILE_NAME_MDTALG    ITE_CONFIG_INFO_PATH"mdt_alg.ini"

#define ITE_CONFIG_FILE_NAME_BOOT    ITE_CONFIG_INFO_PATH"boot.ini"

#define ITE_AI_ALG_CONFIG_MAX_SIZE 128 *1024 //128K
#define ITE_SYS_CONFIG_MAX_SIZE (64*1024)
#define ITE_CONFIG_MAX_SIZE (256 * 1024)

#define ITE_COMMON_DEBUG_OSD_CNT_MAX (16)

typedef enum
{
    ITE_CONFIG_FILE_DEVICE = 0,   //
    ITE_CONFIG_FILE_COMM,   //
    ITE_CONFIG_FILE_ALG1,   //
    ITE_CONFIG_FILE_ALG2,   //
    ITE_CONFIG_FILE_ALG3,   //
    ITE_CONFIG_FILE_MAX,
} ITE_CONFIG_FILE_ID;


typedef enum 
{
	H264 = 1,		//标准H264流
	JPEG,			//标准JPEG流
	D1H264,			//D1规格H264流
	D1JPEG,			//D1规格JPEG流
	SANMUCAMERA1,	//三目相机镜头一
	SANMUCAMERA2,	//三目相机镜头二
	SANMUCAMERA3	//三目相机镜头三
}IPNC_CHANNEL;

/******************************************************************************
* For Configuration Storage Info
******************************************************************************/
#define ITE_VIDEO_CHANNEL_NUM 4
#define ITE_PICTURE_CHANNEL_NUM 2
#define ITE_MAX_ACCOUNT_NUM 4




typedef struct IPNC_H264Param
{
    OS_S32 nCodeRate; //码率
    OS_S32 nFrameRate; //帧率(只读)
    OS_S32 nWidth; //宽(只读)
    OS_S32 nHeight; //高(只读)
    OS_S32 nIFrameInterval;//I帧间隔
    OS_S32 nCodeMethod; //编码方式 0:baseline 1: main 2:high
    OS_S32 nCodeControl; //码流控制 0:None 1:VQCB 2:CQCB 3:CVBR
    OS_S32  channel; //通道号 只在H264与D1H264时结构体有效
    OS_U16 pixH264;  //目前只对H264分辨率有效  0-全分辨率 1-1080P 2-720P 3-H264D1
    OS_U16 ncodetype;// H265  H264
    OS_U16 nReserve[4];//保留参数
} IPNC_H264Param, *PIPNC_H264Param;

typedef struct ITE_PictureParam
{
    OS_S32 ncodetype; //jpeg 
    OS_S32 nFrameRate;//帧率
    OS_S32 nWidth; //宽(只读)
    OS_S32 nHeight; //高(只读)
    OS_S32 nCodeQuality; //编码质量 30 -100
    OS_S32 nCodeControl;
    OS_U16 pixformat;  //目前只对 YUV420I
    OS_U16 nReserve[16];//保留参数
} ITE_PictureParam;


typedef struct AcInfo
{
    OS_S32 whichAc;             // 内部交流电 0    外部交流电 1
    OS_S32 isAc;                  //是否是交流电   1 交流    0 直流  2 交流异常  只读
    OS_S32 enableAc;             //使能ac    1 使能    0 不是能，是交流电才能配置使能
    OS_S32 acDelay;                 //单位10us 相位 范围  0--20 ms
    OS_S32 acException;            //相机端程序使用此变量值
    OS_S32 rsev[2];                 //保留字段
} AcInfo;


typedef struct AcAttr
{
    OS_U16 whichIO;     //交流同步IO 序号，若使用内部交流电不可配
    OS_U16 miniLow;     //统计周期内低电平计数最小值        可配置范围   0-10000    配置值要小于 maxLow
    OS_U16 miniHi;          //统计周期内高电平计数最小值    可配置范围   0-10000    配置值要小于 maxHi
    OS_U16 maxLow;      //统计周期内低电平计数最大值        可配置范围   0-10000
    OS_U16 maxHi;           //统计周期内高电平计数最大值    可配置范围   0-10000
    OS_U16 resv[4];     //保留字段
} AcAttr;

//NICP 接口配置参数的结构体
typedef struct AcSyncParm
{
    AcInfo ac_info;
    AcAttr ac_attr;
} AcSyncParm;




typedef struct _LOGSRV_CONFIG_INFO
{
    OS_U8     IPAddr[16];             //
    OS_U16 port;                  //
    OS_U16 enable;//0,关闭; 1,打开
    OS_U16 type;
    OS_U16 level;
    OS_U32   reserved[4];
} LOGSRV_CONFIG_INFO, *PLOGSRV_CONFIG_INFO;



typedef struct ITE_LAN_PARAM_t   //old + 48
{
    IPNC_NetWorkParam   stNetWorkParam;
    IN_ADDR                     DnsIPAddr;
    OS_S32                      nNetworkSpeed;
    OS_U8                        szReserverd[40];
} ITE_LAN_PARAM_T;


typedef struct ITE_WIFI_PARAM_t
{
    OS_U16      enable;                             //en
    OS_U16      mode;                               //mode
    IN_ADDR     staticIP;                           // IP地址
    IN_ADDR     staticNetMask;                // 子网掩码
    IN_ADDR     staticGateway;                // 网关
    OS_S8        wpa_ssid[32];                  // wifi ssid
    OS_S8        wpa_passwd[32];          // passwd key
    OS_S8        ap_ssid[32];                   //ap  ssid
    OS_S8        ap_passwd[32];             // ap passwd key
    OS_U32      resv[32];                         //保留字段
} ITE_WIFI_PARAM_T;


typedef struct ITE_FTP_PARAM_t
{
    IPNC_FTPParam   stFTPParam;
    IPNC_UploadUser stUploadUser;
    struct in_addr  downloadip;// 下载的IP
    struct in_addr  realtimeip;//  实时上传的IPs
    OS_U16 szReserverd[64];
} ITE_FTP_PARAM_T;



typedef struct ITE_ENCODER_PARAM_t   //2000
{
    OS_S32 videoNum;
    OS_S32 pictureNum;
    IPNC_H264Param    VideoEncoderParam[ITE_VIDEO_CHANNEL_NUM];
    ITE_PictureParam     PictureEncoderParam[ITE_PICTURE_CHANNEL_NUM];
    OS_S32 pre_recode;  //违章录像开关
    OS_S32 nResv[128-1];
} ITE_ENCODER_PARAM_T;


typedef struct ITE_REDLIGHT_ENHANCE_t   // 66 x int
{
    red_strengthen_info redlightZone;//红灯增强区域43 x int
    IPNC_RedLightEx      redlightFactor;  //红灯增强系数 7 x int
    OS_S32 nResv[16];
} ITE_REDLIGHT_ENHANCE_T;


typedef struct ITE_NTP_PARAM_t
{
    IPNC_SNTPParam_Data stNtpParamData;
    OS_U8 szReserved[64];
} ITE_NTP_PARAM_T;

typedef struct ITE_ACSYNC_PARAM_t
{
    AcSyncParm stAcSyncParm;
    OS_U8 szReserved[64];
} ITE_ACSYNC_PARAM_T;


typedef struct _ITE_GpioParam_t
{
    BRT_SDK_BarrettGpioParam stGpioParam;
    OS_U8 szReserved[64];
} ITE_GpioParam_T;

typedef struct ITE_RS232_PARAM_t
{
    IPNC_RS232Prm stRS232Prm;
    OS_U8 szReserved[64];
} ITE_RS232_PARAM_T;

typedef struct ITE_RS485_PARAM_t
{
    IPNC_RS485Prm stRS485Prm;
    OS_U8 szReserved[64];
} ITE_RS485_PARAM_T;


typedef struct ITE_SPEED_PARAM_t
{
    IPNC_SpeedInfo stSpeedInfo;
    OS_U8 szReserved[64];
} ITE_SPEED_PARAM_T;


typedef struct ITE_CONFIG_MODIFIED_t
{
    OS_S32 bModifiedRefresh[MARK_CONFIG_STATUS_MAX];
} ITE_CONFIG_MODIFIED_T;


///////////////////////////////////////////////////////

typedef struct ITE_WEBMEDIA_PARAM_t
{
    ITE_INET_ADDR stStreamAddr;
    ITE_INET_ADDR stSnapMjpgAddr;
} ITE_WEBMEDIA_PARAM_T;



typedef struct ITE_ACCOUNT_t
{
    OS_S8	    user[USER_LEN+2];			///< username
    OS_S8	    password[PASSWORD_LEN+2];	///< password
    OS_U16   authority;				///< user authority
}ITE_ACCOUNT_T;

typedef struct ITE_Ai_DEBUG_CONT_TAG
{
    ITE_RECT_T AiDetectPos;
    OS_U8 text[32];
}ITE_Ai_DEBUG_CONT_T;
typedef struct ITE_AI_DEBUG_TAG
{
    OS_U32 enable;
    OS_U32 uChannel;
    OS_U32 uFrameIndex;
    OS_U32 uPosNums;
    ITE_Ai_DEBUG_CONT_T content[16];
}ITE_Ai_DEBUG;

typedef struct AiEncryptInfo_t
{
    OS_S8  AiCompany[64];         //算法厂家 缩写  相机端赋值
    OS_S32 isEncrypt;             // -1 未授权   ， 1 已授权     2授权已经过期，其他值 未知，相机端赋值
    OS_U32 indate_year;           // 有效期年 
    OS_U32 indate_month;          // 有效期月
    OS_U32 indate_day;            // 有效期日
   
    OS_U8  HWID[8];               // 硬件ID HEX ，加密芯片ID 或者MAC 相机端赋值,主要用于授权文件名的匹配

    OS_U8  hwresv[64];            //保留
    
    OS_U32 EncryptHWSize;         //  相机端赋值
    OS_U8  EncryptHWinfo[512];    //硬件系统导出信息 相机端赋值
    
    OS_U32 LicenseDATASize;       // windows 端赋值
    OS_U8  LicenseDATA[1024];     //授权license 数据 // windows 端赋值
    
    OS_U8  resv[1024];
} AiEncryptInfo_T;

typedef struct ITE_ISP_STATUS_T
{

    OS_U8 aeMode;// AE 模式
    OS_U8 awbMode; // AWB 模式
    OS_U8 afMode;    // af 模式
    OS_U8 algMode;  // 算法模式

    OS_U16 exp;      // 当前曝光时间
    OS_U16 gain;    // 当前增益
    OS_U16 iso;      // 当前增益
    OS_U16 luma;  // 当前亮度

    OS_U8 nSharpness;                                    // 锐度
    OS_U8 nBrightness;                                   // 亮度
    OS_U8 nContrast;                                     // 对比度
    OS_U8 nGamma;                                        // 伽马值

    OS_U8 nBlackLevel;                                   // 黑电平
    OS_U8 nSaturation ;                                   // 饱和度
    OS_U8 nDenoise ;			                //降噪
    OS_U8 nJquality;                                    //JPEG 编码质量

    OS_U32  rsz[11]; 
}ITE_ISP_STATUS;

typedef struct ITE_SYS_DYNAMIC_INFO_t
{
    OS_S8 szH264ResolutionFrist[16];
    OS_S8 szH264ResolutionSecond[16];
    OS_S8 szMJPGResolution[16];
    OS_F32 frame_rate;
    OS_S32 secret_status;
    OS_S32 camera_model;
    OS_S32 boot_param; //! 1,standard; 2,default
    OS_S8   szModel[8];
    OS_S32 sktprocess_status;
    OS_U8   xzxSecret[4];
    OS_S16 RadarSpeed[4];    //雷达速度值
    OS_S16 RadarSyncTimes[4];    //雷达速度值同步到jpg 的次数
    OS_S32 plBright;           //当前车牌亮度信息
    OS_S32 imgBright;
    OS_S16 vehTopBright;
    OS_S16 vehBottomBright;
    OS_S32 vehID;
    ITE_ISP_STATUS isp_status;
    AcInfo     ac_status;
    OS_S32 reserved[128-16-7];
} ITE_SYS_DYNAMIC_INFO_T;

typedef struct stConfigHdr
{
    OS_U8                    magic[12];//
    OS_U8                    szBootMode[4];//arm,  dsp
    OS_S32                   len;
    OS_U32                   crc32_chk;
    OS_U8                    configVer[12];//平台参数版本1.0没有   2.0 从"kep-v2.0"
    OS_U8                    szReserved[32];
} stConfigHdr;

/* 调试框 */
typedef struct ITE_COMMON_COLOR_TAG
{
    OS_U8 r;
    OS_U8 g;
    OS_U8 b;
    OS_U8 resv;
}ITE_COMMON_COLOR_T;
typedef struct ITE_COMMON_DEBUG_TAG
{
    OS_U16 port;
    OS_U16 nums;
    OS_U32 frame_idx;
    ITE_RECT_U16_T rect[ITE_COMMON_DEBUG_OSD_CNT_MAX];
    ITE_COMMON_COLOR_T color[ITE_COMMON_DEBUG_OSD_CNT_MAX];
}ITE_COMMON_DEBUG_T;

typedef struct iConfigInfo
{

    /*-------------------------------------------------------------
                   设备信息-网络参数
    -------------------------------------------------------------*/
    IPNC_DeviceInfoExt            DevParam;       //设备信息名称参数
    ITE_LAN_PARAM_T               LanParam;       //有线网络参数
    ITE_WIFI_PARAM_T              WlanParam;      // 无线网络参数
    ITE_FTP_PARAM_T               FtpParam;       //ftp 上传参数
    ITE_NTP_PARAM_T               NtpParam;       // NTP 校时参数
    OS_S32 resvDev[1024];

    /*-------------------------------------------------------------
                       视频 编解码参数图像参数
    -------------------------------------------------------------*/
    ITE_ENCODER_PARAM_T      CodecParam;           // 编解码参数
    EXT_ISP_Param                    IspParam;             // ISP图像参数
    IPNC_SYS_ExtOsdInfo           MediaOsdParam;        // OSD 叠加参数
    IPNC_ALG_FileName             MediaFileNameParam;   // 媒体存储文件名参数
    IPNC_ImageMergeType         ImageMergeParam;      //图像合成参数
    ITE_REDLIGHT_ENHANCE_T  RedLightEnhanceParam;// 红灯增强 264
    OS_S32 resvCodec[1016 - 66];


    /*-------------------------------------------------------------
                      外设接入参数和IO 接口配置参数
    -------------------------------------------------------------*/
    ITE_ACSYNC_PARAM_T            AcSyncParam;      // 交流同步参数
    ITE_SPEED_PARAM_T             SpeedParam;       // 测速及超速配置参数
    ITE_RS232_PARAM_T             Rs232Param;       // 232 参数
    ITE_RS485_PARAM_T             Rs485Param;       // 485参数
    IPNC_FlashLamp_Data_ex        FlashLampParamEx; // 闪光灯参数 -1
    IPNC_CHECKPOINT_LAMP          CheckPointLamp;   // 闪光灯参数 -2
    ITE_GpioParam_T               GpioParam;        // IO参数，892字节
    OS_S32 resvIO[1990-892];

    /*-------------------------------------------------------------
                   其他系统参数
    -------------------------------------------------------------*/
    ITE_ACCOUNT_T                  AccountParam[ITE_MAX_ACCOUNT_NUM];
    ITE_WEBMEDIA_PARAM_T           MediaNetworkParam; // 流媒体参数视频图片发送地址
    IPNC_SipIpcConfig              SipIpcParam;       // 28181 & VISS sip 协议参数
    IPNC_Netlog_Info               NetlogParam;       // 网络日志
    IPNC_RESTARTParam_Data         TimeRebootParam;   // 系统维护和定时重启
    IPNC_Serial_ports_show_Info    SerialDbgParam;    // 串口设备接入调试参数
    ITE_Ai_DEBUG                           AiDebug;
    stCustomServerConf             CustomServerParam;//278(int)
    IPNC_APPINFO_P2                stAppInfo_p2;      //附属文件参数 37*(int) = 148字节
    ITE_NON_MOTOR_CONFIG_T         stRfidParam; //非机动车基站线圈位置配置 88*(int) = 352字节
    OS_S32 resOther[2048-196-278-37-88];
    //!clear it when read it from config file
    ITE_CONFIG_MODIFIED_T          stModifiedInfo;
    ITE_SYS_DYNAMIC_INFO_T         SysDynamicInfo;
} iConfigInfo;


typedef struct ITE_ConfigFileInfo
{
    OS_S8 fileName[512];
    OS_S8 fileNameDef[512];
    stConfigHdr pHdr;
    OS_U8 *pData;
    OS_U8 *pDataDef;
} ITE_ConfigFileInfo;


/******************************************************************************
* For Export EEProm Config Info
******************************************************************************/
#define EXPORT_EEPROM_HEADER_LEN    (1024)
#define EXPORT_HEADER_ITEM_LEN      (64)
#define EXPORT_HEADER_RESV_LEN      (EXPORT_EEPROM_HEADER_LEN - \
                                        5*EXPORT_HEADER_ITEM_LEN - \
                                        4-68)

typedef struct _ExportSysConfig
{
    OS_U8 szProductVersion[EXPORT_HEADER_ITEM_LEN];
    OS_U8 szNicpVersion[EXPORT_HEADER_ITEM_LEN];
    OS_U8 szAlgVersion[EXPORT_HEADER_ITEM_LEN];
    OS_U8 szExportTime[EXPORT_HEADER_ITEM_LEN];
    OS_U8 szDeviceType[EXPORT_HEADER_ITEM_LEN];
    OS_S32  iSysConfigLen;
    stConfigHdr pHdr;
    OS_U8 szResv[EXPORT_HEADER_RESV_LEN];
    iConfigInfo stSysConfig[0];
} ExportSysConfig;




/******************************************************************************
* For Special Extension Storage Info
******************************************************************************/


typedef struct extOFCfg
{
    IPNC_ALG_OSDInfo  osdCfg;
    IPNC_ALG_FileName    fnameCfg;
    OS_U8 ExtBuf[80 * 1024];
} extOFCfg;


typedef struct ItgNetInfo
{
    OS_S8    ip[16];
    OS_S32       port;
    OS_S8    netmask[16];
    OS_S8    gateway[16];
} ItgNetInfo;

#endif

