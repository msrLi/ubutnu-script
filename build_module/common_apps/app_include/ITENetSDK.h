/**
 * @file ITENetSDK.h
 * @brief  ????SDK?ṹ??ͷ?ļ?
 * @author <itarge@itarge.com>
 * @version 1.0.0
 * @date 2017-06-02
 */

/* Copyright (C) 2009-2016, Itarge Inc.
 * All right reserved
 *
 */

#ifndef __ITE_NET_SDK_H__
#define __ITE_NET_SDK_H__
#include <netinet/in.h>
#include "ite_typedef.h"

#ifndef WIN32
typedef struct in_addr IN_ADDR;
#endif

#ifdef WIN32
typedef SYSTEMTIME IPNC_TIME;                                // win32 ??ϵͳʱ??ʱ?? SYSTEMTIME
#else
// linux
typedef struct timeval IPNC_TIME;                            // linuxƽ̨ϵͳʱ?? timeval
#endif

/************************************************************************
 ** ??��????
 ***********************************************************************/
// SDK?汾?Ŷ???
#ifndef MAKE_VER
typedef OS_U16 _VER_T;
#define MAKE_VER(a, b)  ((_VER_T)(((OS_U8)(((_VER_T)(a)) & 0xff)) | ((_VER_T)((OS_U8)(((_VER_T)(b)) & 0xff))) << 8))
#endif

typedef enum
{
    eSDKVer11 = MAKE_VER(1, 1)                               // 1.01?汾
                // NICP?汾??eNICPVer11 1.01
} ESDKVer;

// ???????ʹ??ţ???ӦITE_IPNC_GetLastError?ӿڵķ???ֵ
#define _EC(x)                      (0x80000000|x)
#define SDK_NOERROR                 0                        // û?д???
#define SDK_NOTINITIALISED          -1                       // û?г?ʼ??SDK????Ҫ?ɹ?????ITE_Init

#define SDK_SYSTEM_ERROR            _EC(0x1)                 // ϵͳ???ô???
#define SDK_HANDLE_ERROR            _EC(0x2)                 // ??????HANDLE??????
#define SDK_CONNECT_ERROR           _EC(0x3)                 // ��???????????ƶ˿?ʧ??
#define SDK_CMD_FAIL                _EC(0x4)                 // ????ִ??ʧ??
#define SDK_HEAD_FAIL               _EC(0x5)                 // ??????Ϣ??????ʧ??
#define SDK_PARAM_ERROR             _EC(0x6)                 // ????????
#define SDK_SEARCH_ERROR            _EC(0x7)                 // ????ָ??IP??????ʧ??

#define SDK_NETWORK_ERROR           _EC(0x100)               // ????????
#define SDK_NETWORK_TIMEOUT         _EC(0x101)               // ???糬ʱ
#define SDK_MUTEX_TIMEOUT           _EC(0x110)               // ?ٽ?????ʱ

#define SDK_NOTSUPPORT              _EC(0x200)               // ???ýӿڲ???֧??
#define SDK_IPNC_APPVER             _EC(0x201)               // Ƕ??ʽ?????汾?ͣ???????Ƕ??ʽ????
#define SDK_IPNC_TYPE_ERROR         _EC(0x202)               // ?????????ͺŴ???
#define SDK_SUPPORT_ERROR           _EC(0x203)               // ?????????ͺŲ?֧?ָýӿڣ????ܣ?
#define SDK_IPNC_SDKVER             _EC(0x204)               // SDK?汾???ͣ???????SDK?汾

#define SDK_SAVE_CONFIGING          _EC(0x300)               // ?????????ڱ?????????SDK?󲿷ֽӿڶ??᷵?ظô?????
// ?û????⵽?ô????????????ӳ??????????Բ???
#define SDK_SAVE_CONFIG_ERROR       _EC(0x301)               // ??????????????ʧ??

#define SDK_SOFT_SNAP_SEND_ERROR    _EC(0x400)               // ?????���??ָ??ʧ??
#define SDK_SOFT_SNAP_RECV_TIMEOUT  _EC(0x401)               // ?????���??ָ??ִ?н?????ʱ
#define SDK_SOFT_SNAP_ERROR         _EC(0x402)               // ?���??ָ??ִ??ʧ??
#define SDK_SOFT_SNAP_CALLBACK      _EC(0x403)               // ?���?????ջص?????û??????

#define SDK_NTP_ERROR               _EC(0x500)               // ??????NTPУʱʧ??
#define SDK_NTP_TIMEOUT             _EC(0x501)               // ?ȴ???????NTPУʱ??????ʱ

#define SDK_STREAM_MAX              _EC(0x800)               // ??????????Ƶ??????????

#define SDK_MEMORY_LACK             _EC(0x1000)              // ?ڴ?????ʧ??

#define DEVICE_NAME_LENGTH          (32)
#define DEVICE_APPVER_LENGTH        (32)
#define DEVICE_RESERVED_LENGTH      (32)
#define DEVICE_TITLE_LENGTH         (12)
#define DEVICE_NUMBER_LENGTH        (16)
#define DEVICE_DIRECTIONLENGTH      (8)
#define DEVICE_POSITION_LENGTH      (64)
#define DEVICE_SN_LENGTH            (32)
#define DEVICE_TYPE_LENGTH          (64)

#define DEV_INFO_ITEM_LENGTH        (64)
#define DEV_USER_DEFINED_LENGTH     (64)
#define DEV_IP_ADDR_LENGTH          (31)

#define USER_LEN                    (32)                     // ?û???????
#define PASSWORD_LEN                (32)                     // ???볤??
#define MAX_FILE_NAME               (128)                    // /< Maximum length of file name.
#define MAX_DOMAIN_LEN              (64)                     // ???????󳤶?




/************************abtain the scence parameter*****************************/
typedef struct IPNC_ITSPrivateParam
{
    OS_S32 nSharpness;                                       // ????
    OS_S32 nBrightness;                                      // ?��?
    OS_S32 nContrast;                                        // ?Աȶ?
    OS_S32 nGamma;                                           // ٤??ֵ
    OS_S32 nBlackLevel;                                      // ?ڵ?ƽ
    OS_S32 nMinAgc;                                          // ????????
    OS_S32 nSaturation;                                      // ???Ͷ?
    OS_S32 nEcompensation;                                   // ?عⲹ??
    OS_S32 nEdgeEnhance;                                     // cancel for kepler                                           // ??Ե??ǿ
    OS_S32 nEdgeSensitive;                                   // cancel for kepler                                           // ??Ե???ж?
    OS_S32 nNoiseFilter;                                     // noise filter for kepler
    OS_S32 nReserved[4];                                     // ????
} IPNC_ITSPrivateParam, *PIPNC_ITSPrivateParam;

typedef struct IPNC_ITSPublicParam
{
    OS_S32 nMaxExpotime;                                     // ????????
    OS_S32 nMinExpotime;                                     // ????????
    OS_S32 nMaxAgc;                                          // ????????
    OS_S32 nPicSnapQuality;                                  // ͼ??ץ????��
    OS_S32 nPicSteramQuality;                                // ͼ??????��
    OS_S32 nEcompensation;                                   // ?عⲹ??
    OS_S32 nRed;                                             // ??ɫ????
    OS_S32 nGreen;                                           // ??ɫ????
    OS_S32 nBlue;                                            // ��ɫ????
    OS_S32 nZoneExp;                                         // ?????ع?ͳ??(4*4)
    OS_S32 nLampThershold;                                   // ?????��???ֵ(-10~~10), Ĭ??Ϊ0
    OS_S32 nMWBThershold;                                    // ?ֶ???ƽ????ֵ(-10~~10)
    OS_S32 nImageRotation;                                   // ͼ????ת
    OS_U16 nNightAWBSwitch;                                  // ҹ????ƽ?⿪?? 0?ر? 1????
    OS_U16 nNightAWBThershiold;                              // ҹ????ƽ????ֵ 0-255
    OS_U16 nRedSecond;                                       // ??ɫ????(???׻??ڶ???SENSOR)
    OS_U16 nGreenSecond;                                     // ??ɫ????(???׻??ڶ???SENSOR)
    OS_U16 nBlueSecond;                                      // ��ɫ????(???׻??ڶ???SENSOR)
    OS_U16 nNightAWBSwitchSecond;                            // ҹ????ƽ?⿪??0?ر?1????(???׻??ڶ???SENSOR)
    OS_U16 nNightAWBThershioldSecond;                        // ҹ????ƽ????ֵ0-255 (???׻??ڶ???SENSOR)
    OS_S16 flashwhite;                                       // ?��??ư?ƽ?ⲹ??
    OS_F32     TomapFreq;                                    // Ƶ??
    OS_F32     TomapLevel;                                   // ǿ?ȵȼ?
    OS_F32     TomapDetail;                                  // ϸ??
    OS_S32     nReserved[3];
} IPNC_ITSPublicParam, *PIPNC_ITSPublicParam;


typedef struct iCAMERA_ISP_Param
{
    OS_S32 sensorchl;                                        // sensor????
    OS_S32 nMaxExpotime;                                     // ????????
    OS_S32 nMinExpotime;                                     // ????????
    OS_S32 nMaxGain;                                         // ????????
    OS_S32 nMinGain;                                         // ????????

    OS_S32 nZoneExp;                                         // ?????ع?ͳ??(4*4)
    OS_S32 nEcompensation;                                   // ?عⲹ??
    OS_S32 nSceneMode;                                       // ????
    OS_S32 nAESpeed;                                         // AE ?????ٶ?

    OS_S32 nDaySharpness;                                    // ????
    OS_S32 nDayBrightness;                                   // ?��?
    OS_S32 nDayContrast;                                     // ?Աȶ?
    OS_S32 nDayGamma;                                        // ٤??ֵ
    OS_S32 nDayBlackLevel;                                   // ?ڵ?ƽ
    OS_S32 nDaySaturation;                                   // ???Ͷ?

    OS_S32 nDayAwbSwitch;                                    // ?????Զ???ƽ?? 0r ?ֶ???ƽ??
    OS_S32 nDayRed;                                          // ??ɫ????
    OS_S32 nDayGreen;                                        // ??ɫ????
    OS_S32 nDayBlue;                                         // ��ɫ????
    OS_S32 nDayDenoise;			                //���콵��
    OS_S32 nDayJquality;                                    //JPEG �����������
    OS_S16 nDayDehaze;                                  // ����͸��
    OS_S16 nDayWDR;                                     // �������̬�ȼ�
    OS_S32 nDayResv[29];                                  //���챣������


    OS_S32 nNightSharpness;                                  // ????
    OS_S32 nNightBrightness;                                 // ?��?
    OS_S32 nNightContrast;                                   // ?Աȶ?
    OS_S32 nNightGamma;                                      // ٤??ֵ
    OS_S32 nNightBlackLevel;                                 // ?ڵ?ƽ
    OS_S32 nNightSaturation;                                 // ???Ͷ?

    OS_S32 nNightAwbSwitch;                                  // ҹ???Զ???ƽ?? or ?ֶ???ƽ??
    OS_S32 nNightAwbThershiold;                              // ҹ????ƽ????ֵ 0-255
    OS_S32 nNightRed;                                        // ??ɫ????
    OS_S32 nNightGreen;                                      // ??ɫ????
    OS_S32 nNightBlue;                                       // ��ɫ????

    OS_S32 nNightSpecialAwbSwitch;                           // ҹ?????ⳡ????ƽ??
    OS_S32 nNightSpecialAwbCondition;                        // ҹ?????ⳡ????ƽ?????? ????????Դ ?��???
    OS_S32 nNightSpecialRed;                                 // ??ɫ????
    OS_S32 nNightSpecialGreen;                               // ??ɫ????
    OS_S32 nNightSpecialBlue;                                // ��ɫ????
    OS_S32 nNightDenoise;                                     // ҹ�併�����
    OS_S32 nNightJquality;                                    //JPEG �����������
    OS_S16 nNightDehaze;                                  // ҹ��͸��
    OS_S16 nNightWDR;                                       // ҹ�����̬�ȼ�
    OS_S32 nNightResv[29];                                   // ҹ�䱣������

    OS_S32 nLampThershold;                                   // ����������ֵ(-10~~10), Ĭ��Ϊ0
    OS_U8  ucresv;                                                
    OS_U8  enFocus;                                                   // 0 ;1;
    OS_U16  enRotation;                                            // 0,90,180,270;
    OS_BOOL lfr;                                                // 1:���õ�֡��ģʽ, 0:����ģʽ
    OS_S32 nReserved[62];                                        // isp��������
} iCAMERA_ISP_Param ;


typedef struct EXT_ISP_Param
{
    OS_S32 sensorIndex;                                        // max 4
    iCAMERA_ISP_Param stISPparam[4];                         // ????֧??4·sensor ????
    OS_S32 szReserverd[32];                                  // ????
} EXT_ISP_Param;



/************************???Ʋ???******************************/

typedef struct IPNC_SmartRecogParam
{
    OS_S32 nMinWindth;                                       // ??????С????
    OS_S32 nMaxWindth;                                       // ????????????
    OS_U8 cProvince[4];                                      // ʡ?????ȼ?
    OS_S32 nDefinition;                                      // ??????ָ??
    OS_S32 nMatchThreshold;                                  // ??λ??ֵ
    OS_S32 nRecogThreshold;                                  // ʶ????ֵ

    OS_U8 bDoubleYellow;                                     // ˫??????
    OS_U8 bIndividuation;                                    // ???Ի?????
    OS_U8 bArmy;                                             // ???ó???
    OS_U8 bDoubleArmy;                                       // ˫?????ó???
    OS_U8 bFarming;                                          // ũ?ó???
    OS_U8 bIsNight;                                          // ҹ??ģʽ
} IPNC_SmartRecogParam;

/****************************************************/

/* ???ò??????ض??? */
// ?豸??Ϣ
typedef struct IPNC_DeviceInfo
{
    // ???????鶼??????Ч?ַ�����??��??????
    OS_U8 szDeviceTitle[DEVICE_TITLE_LENGTH];                // 12 ????
    OS_U8 szDeviceNumber[DEVICE_NUMBER_LENGTH];              // 16 ????
    OS_U8 szDeviceDirection[DEVICE_DIRECTIONLENGTH];         // 8 ????????
    OS_U8 szDevicePosition[DEVICE_POSITION_LENGTH];          // 64 λ??
} IPNC_DeviceInfo, *PIPNC_DeviceInfo;

// ��չ?豸??????Ϣ
typedef struct IPNC_DeviceInfoExt
{
    // ???????鶼??????Ч?ַ�����??��??????
    OS_S8 szDevTitle[DEV_INFO_ITEM_LENGTH];                  // 64bytes  ?豸????
    OS_S8 szDevNumber[DEV_INFO_ITEM_LENGTH];                 // 64bytes  ?豸????
    OS_S8 szDevDirection[DEV_INFO_ITEM_LENGTH];              // 64bytes  ????????
    OS_S8 szDevPosition[DEV_INFO_ITEM_LENGTH];               // 64bytes  ?豸λ??
    OS_S8 szUserDefined1[DEV_USER_DEFINED_LENGTH];           // 64bytes  ?û??Զ????ֶ?1
    OS_S8 szUserDefined2[DEV_USER_DEFINED_LENGTH];           // 64bytes  ?û??Զ????ֶ?2
    OS_S8 szUserDefined3[DEV_USER_DEFINED_LENGTH];           // 64bytes  ?û??Զ????ֶ?3
    OS_S8 szUserDefined4[DEV_USER_DEFINED_LENGTH];           // 64bytes  ?û??Զ????ֶ?4
    OS_S8 video_monitor;                                     // 1bytes   ��Ƶ���
    OS_S8 host_ip_addr[DEV_IP_ADDR_LENGTH];                  // ��Ŀ800W�ܹ���host IP
    OS_U8 szResv[96];                                        // 128bytes �����ֶ�
} IPNC_DeviceInfoExt, *PIPNC_DeviceInfoExt;

typedef struct IPNC_PrecordParams
{
    OS_S32 nSwitch;                                          // 0,off; 1,on
    OS_S32 nResv[28];                                        // ?????ֶ?
} IPNC_PrecordParams, *PIPNC_PrecordParams;



typedef struct IPNC_Netlog_Info
{
    IN_ADDR         ip;
    OS_U16          port;
    OS_S16           enable;
    OS_S16           level;
    OS_S16           nresv[5];
} IPNC_Netlog_Info;

typedef struct IPNC_Serial_ports_show_Info
{
    IN_ADDR         ip;
    OS_U16          port;
    OS_S16          enable;
    OS_S16          nresv[6];
} IPNC_Serial_ports_show_Info;


typedef struct IPNC_syslog_info
{
    OS_U32 ratioErrCount;
    OS_U32 ratioErrTemp;
    OS_U32 noStreamErrCount;
    OS_U32 noStreamErrTemp;
    OS_U32 powerOnNum;
    OS_U32 resv[16];
} IPNC_syslog_info, *PIPNC_syslog_info;

typedef struct IPNC_DateTime
{
    OS_U16 wYear;
    OS_U16 wMonth;
    OS_U16 wDayOfWeek;
    OS_U16 wDay;
    OS_U16 wHour;
    OS_U16 wMinute;
    OS_U16 wSecond;
    OS_U16 wMilliseconds;
} IPNC_DateTime, *PIPNC_DateTime;

// ?豸????ʱ??

typedef enum
{
    WEST12 = -12,
    WEST11 = -11,
    WEST10 = -10,
    WEST9 = -9,
    WEST8 = -8,
    WEST7 = -7,
    WEST6 = -6,
    WEST5 = -5,
    WEST4 = -4,
    WEST3 = -3,
    WEST2 = -2,
    WEST1 = -1,
    GMT0 = 0,                                                // ??ʱ??
    EAST1 = 1,
    EAST2 = 2,
    EAST3 = 3,
    EAST4 = 4,
    EAST5 = 5,
    EAST6 = 6,
    EAST7 = 7,
    EAST8 = 8,
    EST9 = 9,
    EAST10 = 10,
    EAST11 = 11,
    EAST12 = 12
} ETimeZone;

typedef enum
{
    Rotation0 = 0,
    Rotation90 = 90,
    Rotation180 = 180,
    Rotation270 = 270
} EImageRotation;

typedef enum
{
    eCloseItsAWB = 0,                                        // ?ر??Զ???ƽ???㷨
    eOpenItsAWB = 1                                          // ?????Զ???ƽ???㷨
} EItsAWBSwitch;

// ITS ????
typedef struct IPNC_ITSControlParam
{
    OS_S32 nMaxExpotime;                                     // ????????
    OS_S32 nMinExpotime;                                     // ????????
    OS_S32 nMaxAgc;                                          // ????????

    // ͼ??????
    OS_S32 nBrightness;                                      // ?��?
    OS_S32 nContrast;                                        // ?Աȶ?
    OS_S32 nSharpness;                                       // ????
    OS_S32 nJpegquality;                                     // ??��
    OS_S32 nEcompensation;                                   // ?عⲹ??
    OS_S32 nGamma;
    OS_S32 nBlackLevel;                                      // ?ڵ?ƽ

    EItsAWBSwitch ItsAWBSwitch;                              // ?Ƿ??ر??Զ???ƽ???㷨

    OS_S32 nRed;                                             // ??ɫ????
    OS_S32 nGreen;                                           // ??ɫ????
    OS_S32 nBlue;                                            // ��ɫ????
} IPNC_ITSControlParam, *PIPNC_ITSControlParam;

typedef struct IPNC_ITSControlExParam
{
    OS_S32 nMaxExpotime;                                     // ????????
    OS_S32 nMinExpotime;                                     // ????????
    OS_S32 nMaxAgc;                                          // ????????
    // ͼ??????
    OS_S32 nBrightness;                                      // ?��?
    OS_S32 nContrast;                                        // ?Աȶ?
    OS_S32 nSharpness;                                       // ????
    OS_S32 nJpegquality;                                     // ??��
    OS_S32 nEcompensation;                                   // ?عⲹ??
    OS_S32 nGamma;                                           // ٤??ֵ
    OS_S32 nBlackLevel;                                      // ?ڵ?ƽ
    EItsAWBSwitch ItsAWBSwitch;                              // ?Ƿ??ر??Զ???ƽ???㷨
    OS_S32 nRed;                                             // ??ɫ????
    OS_S32 nGreen;                                           // ??ɫ????
    OS_S32 nBlue;                                            // ��ɫ????
    OS_S32 nMinAgc;                                          // ????????
} IPNC_ITSControlExParam, *PIPNC_ITSControlExParam;

#if 0
// ҹ???ֶ?ģʽ
typedef struct IPNC_NightControlParam
{
    OS_S32 nExpoTime;                                        // ????
    OS_S32 nAgc;                                             // ????

    // ͼ??????
    OS_S32 nBrightness;                                      // ?��?
    OS_S32 nContrast;                                        // ?Աȶ?
    OS_S32 nSharpness;                                       // ????
    OS_S32 nJpegquality;                                     // ??��
    OS_S32 Reserve;                                          // ????

    enum
    {
        eCloseAWB = 0,                                       // ?ر??Զ???ƽ???㷨
        eOpenAWB = 1                                         // ?????Զ???ƽ???㷨
    } AWBSwitch;                                             // ?Ƿ??ر??Զ???ƽ???㷨

    OS_S32 nRedGain;                                         // ??ɫ????
    OS_S32 nGreenGain;                                       // ??ɫ????
    OS_S32 nBlueGain;                                        // ��ɫ????
    OS_S32 nGamma;
    OS_S32 nBlackLevel;                                      // ?ڵ?ƽ
} IPNC_NightControlParam, *PIPNC_NightControlParam;
#endif


// ҹ???ֶ?ģʽ
typedef struct IPNC_NightControlParam
{
    OS_S32     nExpoTime;                                    // ????
    OS_S32     nAgc;                                         // ????

    // ͼ??????
    OS_S32     nBrightness;                                  // ?��?
    OS_S32     nContrast;                                    // ?Աȶ?
    OS_S32     nSharpness;                                   // ????
    OS_S32     nJpegquality;                                 // ??��
    OS_S32     Reserve;                                      // ????

    OS_U16 nNightAWBThershiold;                              // ҹ????ƽ????ֵ 0-255
    OS_U16 nNightAWBSwitch;                                  // ҹ????ƽ?⿪?? 0?ر? 1????

    OS_S32     nRedGain;                                     // ??ɫ????
    OS_S32     nGreenGain;                                   // ??ɫ????
    OS_S32     nBlueGain;                                    // ��ɫ????
    OS_S32     nGamma;
    OS_S32     nBlackLevel;                                  // ?ڵ?ƽ
} IPNC_NightControlParam, *PIPNC_NightControlParam;


// for ntp use
typedef enum
{
    eMon = 1,                                                // ????һ
    eTus,                                                    // ???ڶ?
    eWen,                                                    // ??????
    eThs,                                                    // ??????
    eFir,                                                    // ??????
    eSat,                                                    // ??????
    eSun,                                                    // ??????
} Week;

// ??SNTP????
typedef struct IPNC_NTP_FixedTime
{
    OS_S32 Type;                                             // ?̶?ʱ??Уʱ????0:ÿ??1:ÿ??
    Week week;                                               // ???? ???̶?ʱ??Ϊÿ?ܵ?ʱ??ʹ??
    IPNC_DateTime   FixTime;
} IPNC_NTP_FixedTime, *PIPNC_NTP_FixedTime;

typedef struct  IPNC_SNTPParam_Data
{
    OS_U32    bEnable;                                       // ?Ƿ?????NTPУʱ
    OS_S32     iType;                                        // 0:????Уʱ 1:?̶?ʱ??Уʱ
    OS_S32     nInterval;                                    // Уʱ???����?λСʱ??
    IPNC_NTP_FixedTime FixedTime;                            // ?̶?ʱ??
    OS_S8        NTPAddr[MAX_DOMAIN_LEN];                    // IP??????
} IPNC_SNTPParam_Data, *PIPNC_SNTPParam_Data;

typedef struct IPNC_FlashLamp_Data_ex
{
    OS_U8 Lamp1To5_Type[5];                                  // ?Ƶ???????0:LED 1:???? 2:Ƶ?? 3:????
    OS_U8 FlashState;                                        // ?Ƶ??Ĵ???????  0:?????ش???1:?½??ش???
    OS_U8 StrobeState;                                       // ?ƵĴ???????0:Ƶ?��?:?????ش??????��?:?ߵ?ƽ????1:Ƶ?��?:?½??ش??????��?:?͵?ƽ????
    OS_U8 SteadyState;
    OS_U8 Lane[5];                                           // 5?����?ʹ?õĵƺ? Ԫ??ֵ????0:?? 1:??1 2:??2 3:??3 4:??4 5:??5
    OS_U8 Setlampopentime[3];                                // ?ֱ???ӦƵ?��?0?????��⣨1???ͳ??��?2????ֵ????ҹ?俪?ƣ?0???????쿪?ƣ?1????ȫ?쿪?ƣ?2??
    OS_U8 StrobelampInsertValue;                             // Ƶ?��Ʋ?ֵ????
    OS_U8 nUseFlashLamp;
    OS_U8 PWtype;                                            // Ƶ?��??????????ķ?ʽ1:???ݿ??Ŵ?С2:?û?????
    OS_U8 PWvalue;                                           // Ƶ?��?????????????ֵ,??Χ--5ms,ֻ???û?????ʱ??Ч
    OS_U8 Lamp6To10_Type[5];                                 // �����0:� 1:LED 2:�� 3:�� 4:��
    OS_U8 Channel;
    OS_U8 nReserved[20-5-1];                                 // ????
} IPNC_FlashLamp_Data_ex, *PIPNC_FlashLamp_Data_ex;

typedef struct IPNC_FlashLamp_Data
{
    OS_S32 Lamp1To4_Type;                                    // ?Ƶ???????0:LED 1:????
    OS_S32 Lamp1To4_TriggState;                              // ?Ƶ??Ĵ???????  0:?????ش???1:?½??ش???
    OS_S32 Lamp5_Type;                                       // ?Ƶ?????0:Ƶ??1:????
    OS_S32 Lamp5_TriggState;                                 // ?ƵĴ???????0:Ƶ?��?:?????ش??????��?:?ߵ?ƽ????1:Ƶ?��?:?½??ش??????��?:?͵?ƽ????
    OS_S32 Lane[4];                                          // 4?����?ʹ?õĵƺ? Ԫ??ֵ????1:??1 2:??2 3:??3 4:??4 5:??5
    OS_S32 iTurns;                                           // ?Ƿ?????0:??????1: ????(1????ʹ?õ?1,2;2????ʹ?õ?3,4;Laneֵ??Ч)
    OS_S32 iForceFlashlamp;                                  // ?Ƿ?ǿ?ƴ????��???0:??????1: ????
    OS_S32 iForceStrobelamp ;                                // ?Ƿ?ǿ?ƴ???Ƶ?��?0:??????1: ????
    OS_S32 StrobelampInsertValue;                            // Ƶ?��Ʋ?ֵ????
    OS_S32 nUseFlashLamp;                                    // ??ȡ?????��??ƿ???1:?? 2:??
    OS_U16 PWtype;                                           // Ƶ?��??????????ķ?ʽ1:???ݿ??Ŵ?С2:?û?????
    OS_U16 PWvalue;                                          // Ƶ?��?????????????ֵ,??Χ--5ms,ֻ???û?????ʱ??Ч
    OS_S32 nReserved[4];                                     // ????
} IPNC_FlashLamp_Data, *PIPNC_FlashLamp_Data;

// NetWork ????struct in_addr  ip
typedef struct _IPNC_NetWorkParam
{
    IN_ADDR IPAddr;                                          // IP??ַ
    IN_ADDR NetMask;                                         // ????????
    IN_ADDR Gateway;                                         // ????
} IPNC_NetWorkParam, *LPIPNC_NetWorkParam;

// FTP????
typedef struct IPNC_FTPParam
{
    enum
    {
        SDK_Upload = 0,                                      // ?ϴ???SDK
        FTP_Upload = 1                                       // ?ϴ???FTP??????
    } UploadPath;                                            // ???���??ͼƬ?ϴ???ʽ

    OS_S8 FTPAddr[MAX_DOMAIN_LEN];                           // 192.168.88.100 www.iXXX.com
    OS_U16 wPort;                                            // FTP?˿?
    OS_U16 wEnable3G;                                        // ?Ƿ?????3Gģʽ??0:?????? 1:????)
    OS_S8 szUserName[USER_LEN];                              // ?û???
    OS_S8 szPassword[PASSWORD_LEN];                          // ????
    OS_S8 szFolderPath[MAX_FILE_NAME];                       // ·??
} IPNC_FTPParam, *PIPNC_FTPParam;

typedef struct IPNC_UploadUser
{
    OS_S32 nFTP;                                             // ftp?ϴ? 0:??????  1:Ԥ????
    OS_S32 nSDK;                                             // sdk?ϴ? 0:??????  1:Ԥ????
    OS_U8  enFtpIP;                                          // ????IP ?ϴ?
    OS_U8  enFtpDate;                                        // ?????????ϴ?
    OS_U8  enFtpHour;                                        // ????Сʱ?ϴ?
    OS_U8       content;                                     // ftp?ϴ?????(??????),2.0ƽ̨?? 0λ:Υ??JPEGͼƬ 1λ:????JPEGͼƬ 2λ:INI?ļ?
    OS_U8       dir;                                         // ftpĿ¼?ṹ,2.0ƽ̨??0:??Ŀ¼1:һ??Ŀ¼2:????Ŀ¼3??????Ŀ¼4:?ļ?Ŀ¼5???弶Ŀ¼
    OS_U8       folder[5];                                   // ftpһ??Ŀ¼,2.0ƽ̨??0:IP??ַ1:?豸????2:?豸λ??3:?豸????4:?Զ?????Ϣһ5:?Զ?????Ϣ??6:?Զ?????Ϣ??7:?Զ?????Ϣ??
    // 8:ץ??ʱ??9:ץ??????
    OS_U8   notused[2] ;                                     // ????
    OS_S32     nResv[2];                                     // ????
} IPNC_UploadUser, *PIPNC_UploadUser;

typedef struct IPNC_CHECKPOINT_LAMP
{
    OS_S32 everlamp;                                         // ???��??��ȷ?Χ
    OS_S32 flashlamp;                                        // ?��????��ȷ?Χ
    OS_S16 time;                                             // ʱ??
    OS_S16 Reserve;                                          // ????
    OS_S32 bReserve[9];                                      // ????
} IPNC_CHECKPOINT_LAMP, *PIPNC_CHECKPOINT_LAMP;


typedef struct IPNC_FactoryAlgLicense
{
    OS_S32     nFactoryID;
    OS_U8    szFactoryCode[16];                              // ???̵??ַ?????ʾ????ѡ
    OS_S32     nResv[11];                                    // ????
    OS_U8    body[0];
} IPNC_FactoryAlgLicense, *PIPNC_FactoryAlgLicense;          // 64bytes

enum
{
    FACTORY_ALG_LIC_START = 0,
    FACTORY_ALG_LIC_VSDEVT,                                  // ά????չ
    FACTORY_ALG_LIC_JINLING,                                 // ????��??
};

// SNTP????
typedef struct IPNC_SNTPParam
{
    OS_U32 bEnable;                                          // ?Ƿ?????NTPУʱ
    OS_S32 nInterval;                                        // Уʱ???����?λСʱ??
    OS_U8 NTPAddr[MAX_DOMAIN_LEN];                           // 192.168.88.100 www.iXXX.com
} IPNC_SNTPParam, *PIPNC_SNTPParam;

enum
{
    IPNC_OSD_JPEG_NORMAL = 1,
    IPNC_OSD_JPEG_MERGE,
    IPNC_OSD_H264_VIDEO,
};

typedef struct IPNC_JPEG_OSDInfo                             // JPEG ????OSD ????????
{
    OS_U8 nDevPosition;                                      // ?豸λ??
    OS_U8 nDevDirect;                                        // ?豸????
    OS_U8 nDevNO;                                            // ?豸????
    OS_U8 nDevName;                                          // ?豸????
    OS_U8 nIPAddr;                                           // IP??ַ
    OS_U8 nSnapTime;                                         // ץ??ʱ??
    OS_U8 nLance;                                            // ??????Ϣ
    OS_U8 nCarNumber;                                        // ???ƺ???
    OS_U8 nwfType;                                           // Υ??????
    OS_U8 nOsdTime;                                          // OSDʱ??
    OS_U8 nSnapIndex;                                        // ץ??????
    OS_U8 nCarColr;                                          // ??????ɫ
    OS_U8 nConfidence;                                       // ???Ŷ?
    OS_U8 nSpeed;                                            // ????
    OS_U8 nOverSpeedrate;                                    // ???ٱ?
    OS_U8 nSnapType;                                         // ץ??????
    OS_U8 nCarType;                                          // ??��????
    OS_U8 nCarlogo;                                          // ????
    OS_U8 nCarColor;                                         // ??????ɫ
    OS_U8 nLimitSpeed;                                       // ????
    OS_U8 nCustom1;                                          // ?Զ???
    OS_U8 nCustom2;                                          // ?Զ???
    OS_U8 nCustom3;                                          // ?Զ???
    OS_U8 nCustom4;                                          // ?Զ???
    OS_S16 nX;                                               // ?ഹֱ??????Ե
    OS_S16 nY;                                               // ??ˮƽ??????Ե
    OS_U8 nInfoPosition;                                     // ????λ??0:????1:????
    OS_U8 nFontSize;                                         // ??????С12,14,16,18,20,24
    OS_U8 nOsdRed;                                           // OSD??Ϣ??ɫ-??
    OS_U8 nOsdGreen;                                         // OSD??Ϣ??ɫ-??
    OS_U8 nOsdBule;                                          // OSD??Ϣ??ɫ-��
    OS_U8 nOneLineShow;                                      // ??????Ϣһ????ʾ0:??(??ÿ????Ϣ????ռ??һ??) 1:??
    OS_U8 enable;                                            // ?Ƿ?????OSD 0:??????1:????
    OS_U8 nmd5;                                              // ??α??
    OS_U8 nRedtime;                                          // ???ƺ?ʱ??
     OS_U8 nResv[3];  //��PC�˱���һ��
 
     OS_U8 nCarModel;// ������ʽ 
     OS_U8 nSlag;//: �Ƿ������� 
     OS_U8 nSunroof;//: �����촰 
     OS_U8 nSpareTire;//: ���ޱ�̥ 
     OS_U8 nRack;//: ���������
     OS_U8 nBelt;//���Ƿ���ϵ��ȫ����
     OS_U8 nCall;//:  �Ƿ��绰
     OS_U8 nCrash;//: �Ƿ� ����ײ�� 
     OS_U8 nDanger;//: �Ƿ�Σ��Ʒ����
 
    OS_U8 nCresv[15-9-3];                                    // ���ֽڶ��뱣���ֶ�
    OS_U8 order[64];                                         // ?洢??????��??˳??
} IPNC_JPEG_OSDInfo, *PIPNC_JPEG_OSDInfo;

typedef struct IPNC_Info_P2                                  // ?????ļ?????
{
    OS_U8 nDevPosition;                                      // ?豸λ??
    OS_U8 nDevDirect;                                        // ?豸????
    OS_U8 nDevNO;                                            // ?豸????
    OS_U8 nDevName;                                          // ?豸????
    OS_U8 nIPAddr;                                           // IP??ַ
    OS_U8 nSnapTime;                                         // ץ??ʱ??
    OS_U8 nLance;                                            // ??????Ϣ
    OS_U8 nCarNumber;                                        // ???ƺ???
    OS_U8 nwfType;                                           // Υ??????
    OS_U8 nOsdTime;                                          // OSDʱ??
    OS_U8 nSnapIndex;                                        // ץ??????
    OS_U8 nCarColr;                                          // ??????ɫ
    OS_U8 nConfidence;                                       // ???Ŷ?
    OS_U8 nSpeed;                                            // ????
    OS_U8 nOverSpeedrate;                                    // ???ٱ?
    OS_U8 nSnapType;                                         // ץ??????
    OS_U8 nCarType;                                          // ??��????
    OS_U8 nCarlogo;                                          // ????
    OS_U8 nCarColor;                                         // ??????ɫ
    OS_U8 nLimitSpeed;                                       // ????
    OS_U8 nCustom1;                                          // ?Զ???
    OS_U8 nCustom2;                                          // ?Զ???
    OS_U8 nCustom3;                                          // ?Զ???
    OS_U8 nCustom4;                                          // ?Զ???
    OS_S16 nX;                                               // ?ഹֱ??????Ե
    OS_S16 nY;                                               // ??ˮƽ??????Ե
    OS_U8 nInfoPosition;                                     // ????λ??0:????1:????
    OS_U8 nFontSize;                                         // ??????С12,14,16,18,20,24
    OS_U8 nOsdRed;                                           // OSD??Ϣ??ɫ-??
    OS_U8 nOsdGreen;                                         // OSD??Ϣ??ɫ-??
    OS_U8 nOsdBule;                                          // OSD??Ϣ??ɫ-��
    OS_U8 nOneLineShow;                                      // ??????Ϣһ????ʾ0:??(??ÿ????Ϣ????ռ??һ??) 1:??
    OS_U8 enable;                                            // ?Ƿ????? 0:??????1:???? 2:FTP 3:SDK
    OS_U8 nmd5;                                              // ??α??
    OS_U8 nRedtime;                                          // ???ƺ?ʱ??
    OS_U8 file_type;                                         // 0:xml  1:ini
    OS_U8 enTrafficFlow;                                     // ʹ�ܳ�����ͳ�Ʋ��� 0   �����ã� 1 ����
    OS_U8 nTrafficFlowTime;                                  // ������ͳ��ʱ��� ��λ����  ��Чֵ��Χ1-255 ��
    OS_U8 nCarModel;// ������ʽ 
     OS_U8 nSlag;//: �Ƿ������� 
     OS_U8 nSunroof;//: �����촰 
     OS_U8 nSpareTire;//: ���ޱ�̥ 
     OS_U8 nRack;//: ���������
     OS_U8 nBelt;//���Ƿ���ϵ��ȫ����
     OS_U8 nCall;//:  �Ƿ��绰
     OS_U8 nCrash;//: �Ƿ� ����ײ�� 
     OS_U8 nDanger;//: �Ƿ�Σ��Ʒ����

    OS_U8 nCresv[12-9];                                        // ���ֽڶ��뱣���ֶ�
    OS_U8 order[64];                                         // �洢����������˳��
} IPNC_Info_P2, *PIPNC_Info_P2;

typedef struct IPNC_VIDEO_OSDInfo                            // ??Ƶ????OSD ????????(H264   CVBS  HD_SDI)
{
    OS_U8 nDevPosition;                                      // ?豸λ??
    OS_U8 nDevDirect;                                        // ?豸????
    OS_U8 nDevNO;                                            // ?豸????
    OS_U8 nDevName;                                          // ?豸????

    OS_U8 nIPAddr;                                           // IP??ַ
    OS_U8 nOsdTime;                                          // OSDʱ??
    OS_U8 nResv[2];                                          // ?ֽڶ??? ?????ֶ?

    OS_U8 nCustom1;                                          // ?Զ???1
    OS_U8 nCustom2;                                          // ?Զ???2
    OS_U8 nCustom3;                                          // ?Զ???3
    OS_U8 nCustom4;                                          // ?Զ???4

    OS_S16 nX;                                               // ?ഹֱ??????Ե
    OS_S16 nY;                                               // ??ˮƽ??????Ե

    OS_U8 nInfoPosition;                                     // ????λ?? 0:???? 1:????
    OS_U8 nFontSize;                                         // ??????С 12,14,16,18,20,24
    OS_U8 nOsdRed;                                           // OSD??Ϣ??ɫ-??
    OS_U8 nOsdGreen;                                         // OSD??Ϣ??ɫ-??

    OS_U8 nOsdBule;                                          // OSD??Ϣ??ɫ-��
    OS_U8 nOneLineShow;                                      // ??????Ϣһ????ʾ 0:??(??ÿ????Ϣ????ռ??һ??) 1:??
    OS_U8 enable;                                            // ?Ƿ?????OSD 0:?????? 1:????

    OS_U8 nCresv[9];                                         // ???ֽڶ??뼰?????ֶ?
    OS_U8 order[32];                                         // ?洢??????��??˳??
} IPNC_VIDEO_OSDInfo, *PIPNC_VIDEO_OSDInfo;

typedef struct IPNC_SYS_ExtOsdInfo
{
    IPNC_JPEG_OSDInfo stNormalJpgOsdCfg;                     // ???????ż?????OSD????
    IPNC_JPEG_OSDInfo stMergJpgOsdCfg;                       // ?ϳ?ͼ????Υ?? OSD ????
    IPNC_VIDEO_OSDInfo stVideoOsdcfg;                        // H264??ƵOSD???? ĿǰĬ?϶?·??Ƶʹ??һ?ײ???(H264   CVBS  HD_SDI)
    OS_S32 Resv[8];                                          // ?????ֶκ???��չʹ??
} IPNC_SYS_ExtOsdInfo;

typedef struct IPNC_APPINFO_P2
{
    IPNC_Info_P2  stappinfo_p2;
    OS_S32 Resv[8];
} IPNC_APPINFO_P2;

// OSD
typedef struct IPNC_OSDInfo
{
    OS_S32 bDevPosition;                                     // ?豸λ??
    OS_S32 bIPAddr;                                          // IP??ַ
    OS_S32 bRedLamp;                                         // ????ʱ??
    OS_S32 bSnapTime;                                        // ץ??ʱ??
    OS_S32 bDevNO;                                           // ?豸????
    OS_S32 bDirection;                                       // ????????
    OS_S32 bCarNumber;                                       // ???ƺ???
    OS_S32 bLance;                                           // ??????Ϣ
    OS_S32 bSnapNO;                                          // ץ??????
    OS_S32 bSnapType;                                        // ץ??????
    // ??????Ϣ 0:?????? 1:????

    OS_S32 nInfoPosition;                                    // ????λ?? 0:???? 1:????
    OS_S32 nY;                                               // y???꣬>=50

    // OSD??Ϣ??ɫ
    OS_S32 nOsdRed;                                          // ??
    OS_S32 nOsdGreen;                                        // ??
    OS_S32 nOsdBlue;                                         // ��

} IPNC_OSDInfo, *PIPNC_OSDInfo;

// MULTI OSD
typedef struct IPNC_MULTI_OSDInfo
{
    OS_S32 nDevPosition;                                     //
    OS_S32 nIPAddr;                                          // IP
    OS_S32 nRedLamp;                                         //
    OS_S32 nAfterRedLamp;
    OS_S32 nSnapTime;                                        //
    OS_S32 nDevNO;                                           //
    OS_S32 nDirection;                                       //
    OS_S32 nCarNumber;                                       //
    OS_S32 nLance;                                           //
    OS_S32 nSnapNO;                                          //
    OS_S32 nSnapType;                                        //
    OS_S32 nSpeed;
    OS_S32 nMaxSpeed;
    OS_S32 nOverSpeedPer;
    //

    OS_S32 nX;
    OS_S32 nY;                                               // y>=50
    OS_S32 nInfoPosition;                                    //
    OS_S32 nFontSize;

    // OSDD
    OS_S32 nOsdRed;                                          //
    OS_S32 nOsdGreen;                                        //
    OS_S32 nOsdBlue;                                         //

    OS_S32 nOsdTime;
    OS_S32 nOneLineShow;
    OS_S32 nOsdChannel;
    OS_S32 nReserved[7];
} IPNC_MULTI_OSDInfo, *PIPNC_MULTI_OSDInfo;

typedef struct IPNC_OSDInfoEx
{
    OS_S32 bOSDTime;                                         // OSDʱ?? 0:?????? 1:????
    OS_S32 bReserve[10];                                     // ????
} IPNC_OSDInfoEx, *PIPNC_OSDInfoEx;

// H264OSD
typedef struct IPNC_H264OSDInfo
{
    OS_S32 bDevPosition;                                     // ?豸λ??
    OS_S32 bDevNO;                                           // ?豸????
    OS_S32 bTime;                                            // ʱ??
    OS_S32 bRev[20];                                         // ????
    // ??????Ϣ 0:?????? 1:????

} IPNC_H264OSDInfo, *PIPNC_H264OSDInfo;


typedef struct _ITE_Time_Struct
{
    OS_U16 wYear;
    OS_U16 wMonth;
    OS_U16 wDay;
    OS_U16 wHour;
    OS_U16 wMinute;
    OS_U16 wSecond;
    OS_U16 wMilliseconds;
    OS_U16 wReserved;
} ITE_Time_Struct;


typedef struct KalgEncryptInfo
{
    OS_S32 isEncrypt;// 1 ????Ȩ     -1 δ??Ȩ    ????ֵ δ֪
    OS_U8  secretID[8]; // ????оƬID HEX
    OS_U8  EncryptKey[128];// ??Ȩ??Ϣ HEX
    OS_U8  resv[64];
} KalgEncryptInfo;



// ??ȡKϵ??????????״̬?Ľӿ?
typedef struct _KALStatus
{
    OS_S32 m3_vpss_count;                                    // m3_vpss??״̬????
    OS_S32 m3_video_count;                                   // m3_video״̬????
    OS_S32 dsp_count;                                        // dsp ״̬????
    OS_S32 arm_count;                                        // arm?ɼ?????
    OS_S32 arm_to_dsp_count;                                 // arm???????ݸ?DSP????
    OS_S32 reserved[10];                                     // ????
} KALStatus;


// ??ȡKϵ??16????????״̬?Ľӿ?
typedef struct _KALStorageStatus
{
    OS_S32 isExistSata;                                      // ?Ƿ?????Ӳ??
    OS_S32 partitionCount;                                   // ????????
    OS_S32 p1_total;                                         // ??һ???洢??????��
    OS_S32 p2_total;                                         // ?ڶ????洢??????��
    OS_S32 p1_remain;                                        // ??һ???洢??ʣ????��
    OS_S32 p2_remain;                                        // ?ڶ????洢??ʣ????��
    OS_S32 reserved[10];                                     // ????
} KALStorageStatus;

// ??ȡKϵ??16????????״̬?Ľӿ?
typedef struct _KALFormatStatus
{
    OS_S32 p1_format;                                        // ??һ???洢????ʽ??????
    OS_S32 p2_format;                                        // ?ڶ????洢????ʽ??????
    OS_S32 reserved[10];                                     // ????
} KALFormatStatus;

typedef struct IPNC_RecvDataParam
{
    IN_ADDR IPAddr;                                          // IP??ַ
    OS_S32 nType;                                            // 0-ʵʱ 1-??ʷ??  (????)
    OS_S32 rev[6];                                           // ?????ֶ?
} IPNC_RecvDataParam, *PIPNC_RecvDataParam;

enum
{
    DGPIO_ALARM_INUSE = 0,                                   // ?Ƿ?????
    DGPIO_ALARM_WHICHIO,                                     // ????IO
    DGPIO_ALARM_TYPE,                                        // ????????
    DGPIO_ALARM_Howmany,                                     // ץ??????
    DGPIO_ALARM_FlashDelay,
    DGPIO_ALARM_ExpDelay,
    DGPIO_ALARM_Interval,                                    // ץ?ļ???
    DGPIO_ALARM_CHAR,
    DGPIO_ALARM_TRAFFIC,                                     // ????????
    DGPIO_ALARM_Switch                                       //
};

#define BRT_DAGUERRE_BARRETT_ALARM_SIT_NUMBER 20
#define BRT_DAGUERRE_LAMP_NUMBER 4
typedef struct _BRT_SDK_BarrettGpioParam
{
    OS_U32 tRow_delay;                                       // ??????
    OS_U32 Daguerre_LampMode;                                // ?��???-?��???????1??led 2??????
    OS_U32 lamp_time;                                        // ?��???-????ʱ??us
    OS_U32 lamp_Status[BRT_DAGUERRE_LAMP_NUMBER];            // ?��???- ״̬(????????)
    OS_U32 set[BRT_DAGUERRE_BARRETT_ALARM_SIT_NUMBER][DGPIO_ALARM_Switch + 1];
} BRT_SDK_BarrettGpioParam, *LPBRT_SDK_BarrettGpioParam;


typedef struct IPNC_RS232Attri
{
    OS_S32 nBaudRate;                                        // ?????? 0:115200 1:57600 2:56000 3:38400 4:19200 5:14400 6:9600 7:4800
    OS_S32 nDataBits;                                        // ????λ 0:7 1:8
    OS_S32 nParity;                                          // ??żλ 0:?? 1:ż 2:??
    OS_S32 nStopBits;                                        // ֹͣλ 0:1 1:2
    OS_S32 ndataCtl;                                         // ????????  0:??
    OS_S32 nDeviceid;                                        // ?豸ID
    OS_S16    redlightdelay;                                 // ?????Ŵ?????ʱʱ?? ?ⴥ?? rs485??????Ч
    OS_S16    sResv;                                         // ????
    OS_S32      nResv[3];                                    // ????
} IPNC_RS232Attri, *PIPNC_RS232Attri;


typedef struct IPNC_RS485Attri
{
    OS_S32 nBaudRate;                                        // ?????? 115200 57600 56000 38400 19200 14400 9600 4800
    OS_S32 nDataBits;                                        // ????λ 7 8
    OS_S32 nParity;                                          // ??żλ 0:?? 1:ż 2:??
    OS_S32 nStopBits;                                        // ֹͣλ 1 2
    OS_S32 ndataCtl;                                         // ????????  0:??
    OS_S32 nDeviceid;                                        // ????????ַ
    OS_S16   redlightdelay;                                  // ?????Ŵ?????ʱʱ??
    OS_S16   sResv;                                          // ????
    OS_S32     nResv[3];                                     // ????
} IPNC_RS485Attri, *PIPNC_RS485Attri;


typedef struct IPNC_CardDtAndLan
{
    OS_S32 nEnable;                                          // ?Ƿ????ý??????? 0:??ʹ?? 1:ʹ??
    OS_S32 nLanNum;                                          // ????
} IPNC_CardDtAndLan, *PIPNC_CardDtAndLan;


typedef struct IPNC_CaptureNum
{
    OS_S32 nEnable1;                                         // ????ץ?????? 0:ʹ?? 1:??ʹ??
    OS_S32 nEnable2;                                         // ???ڳ???ץ?????? 0:ʹ?? 1:??ʹ??
} IPNC_CaptureNum, *PIPNC_CaptureNum;


// RS232
typedef struct IPNC_RS232Prm
{
    OS_S32 nindex;                                           // ??Ӧ?????????״?????
    IPNC_CardDtAndLan stuCL;
    IPNC_CaptureNum stuCN;
    IPNC_RS232Attri stuRA;
} IPNC_RS232Prm, *PIPNC_RS232Prm;


// RS485
typedef struct IPNC_RS485Prm
{
    OS_S32 nindex;                                           // ??Ӧ?????????״?????
    IPNC_CardDtAndLan stuCL;
    IPNC_CaptureNum stuCN;
    IPNC_RS232Attri stuRA;
} IPNC_RS485Prm, *PIPNC_RS485Prm;

#define ROAD_COUNT 4
typedef struct IPNC_RoadInfo
{
    OS_S32 nType;                                            // ???? 1:io???? 2:?????????? 3:?״?????
    OS_S32 nEnable;                                          // ?в???ʹ?? 1:ʹ??  0:??ֹ
    OS_F32 fDistance;                                        // ??Ȧ????
    OS_F32 fSpeedMax;                                        // ????
    OS_S32 nPrecentMaxSpeed;                                 // ???ٰٷֱ? [0??100]
    OS_S32 nPrecentCorrect;                                  // ?ٶȽ????ٷֱ? [10~200]
    OS_S32 nCount;                                           // ????ץ?????? 0 1 2 3
    OS_S32 nSpeedCount;                                      // ????ץ?????? 0 1 2 3
    OS_S32 nDelay;                                           // ʱ?????? ??λ:ms
    OS_F32   fBigCarSpeedMax;                                // ????????
    OS_S32 nReserve[7];                                      // ????
} IPNC_RoadInfo, *PIPNC_RoadInfo;

typedef struct IPNC_SpeedInfo
{
    IPNC_RoadInfo stuRoadInfo[ROAD_COUNT];
    OS_S32 nSame;                                            // 0:??ͬ  1:??ͬ windows ʹ??
    OS_S32 nResv[3];
} IPNC_SpeedInfo, *PIPNC_SpeedInfo;


// ????ץ?Ŀ??????淶Χ
typedef struct IPNC_QuickSnapParam
{
    OS_S32 nExp;                                             // ?ٷֱ? 1--300
    OS_S32 nGain;                                            // ?ٷֱ? 1--300
    OS_S32 nUseSecondParm;                                   // ?Ƿ??����??ײ???????0????1??????
    OS_S32 nMaxExp;
    OS_S32 nMaxGain;
    OS_S32 nResv[7];
} IPNC_QuickSnapParam, *PIPNC_QuickSnapParam;


// ͸?����?????????
typedef struct IPNC_TransparentSerial
{
    OS_S32 nBaudRate;                                        // ?????? 0:115200 1:57600 2:56000 3:38400 4:19200 5:14400 6:9600 7:4800
    OS_S32 nDataBits;                                        // ????λ 0:7 1:8
    OS_S32 nParity;                                          // ??żλ 0:?? 1:ż 2:??
    OS_S32 nStopBits;                                        // ֹͣλ 0:1 1:2
    OS_S32 ndataCtl;                                         // ????????  0:??
    OS_S32 nResv[4];                                         // ????
} IPNC_TransparentSerial, *PIPNC_TransparentSerial;



// ?ع?ʱ??????
typedef struct IPNC_TimeFrameInterval
{
    OS_S32 nTimeFrame;                                       // ???õ?ʱ??
    OS_S32 nRealTimeFrame;                                   // ʵ??ʱ??(ֻ??)
    OS_S32 nACSTimeFrame;                                    // ????ͬ??ʱʱ??(ֻ??)
    OS_S32 nResv[5];                                         // ????
} IPNC_TimeFrameInterval, *PIPNC_TimeFrameInterval;


typedef struct IPNC_RedLightEx
{
    OS_S32   nCoffCorrection;                                // fY; 0~512  ????У??ϵ??
    OS_F32 fCoffCheck;                                       // fU; 1.0~3.0 ????????????ϵ??
    OS_S32   nExpTime;                                       // nfV;1~1000(10us) ?ع?ʱ??
    OS_U16 reg257;                                           // ?Ĵ???0x257 ??8λ?Ǻ?????ǿ??????????ֵ????8λΪ????????ֵ
    OS_U16 reg204;                                           // ?Ĵ???0x204  ??8λ?ǿ???ֵ?о????? ??8λ???????о?????
    OS_U16 reg211;                                           // ?Ĵ???0x211 ֻ?õ?8λ ???????䷢??ʱ??С??ֵ??
    OS_U16 reg1c1;                                           // ?Ĵ???0x1c1 ֻ?õ???8λ ??????????ɫ??ʱ??С??ֵ??
    OS_S32 nResv[2];                                         // ????
} IPNC_RedLightEx, *PIPNC_RedLightEx;


// kepler????ͨ?��?????????????
typedef struct IPNC_KeplerTDConfig
{
    OS_U8 szTDServerIP[20];                                  // ??????IP
    OS_U16 sTDServerPort;                                    // ???????˿?
    OS_U16 sReserved;                                        // ?????ֶ?
    OS_S32 resv[20];
} IPNC_KeplerTDConfig, *PIPNC_KeplerTDConfig;




#define RED_STRENGTH_WIN_NUM    6
typedef struct _venc_vect_win_param
{
    OS_S32 nLeft;                                            // ???Ͻ?X
    OS_S32 nTop;                                             // ???Ͻ?Y
    OS_S32 nRight;                                           // ???½?X
    OS_S32 nBottom;                                          // ???½?Y
    OS_S32 nReserved;
} venc_vect_win_param;

typedef struct _red_strengthen_info
{
    OS_S32 nEnable;
    OS_S32 nNum;
    OS_S32 nReseved;
    venc_vect_win_param stWinInfo[8];
} red_strengthen_info;




typedef struct IPNC_SoftSnapParam
{
    OS_S32     nCount;                                       // ????
    OS_S32     nDelay;                                       // ץ????ʱ
    OS_S32     nFlash;                                       // ץ?ĵĶ?Ӧ?ư?λ????
    OS_S32     nLane;                                        // ????
    OS_S32     nSpeed;                                       // ????
    OS_S32     nMaxSpeed;                                    // ?????ٶ?
    OS_U8    szCarNum[16];                                   // ????
    OS_S32     pContext;                                     // ????????Ϣ

    OS_S32     nindex;                                       // ץ??????
    OS_S32     nRredLamptime;                                // ???ƺ?ʱ??(ms)
    OS_S32     nResv[7];                                     // ????
} IPNC_SoftSnapParam, *PIPNC_SoftSnapParam;


// FTP?ϴ??ļ???????
#define FILE_NAME_ELEMENT 8
typedef struct IPNC_PicFileName
{
    enum
    {
        FNE_NON = 0,                                         // ??Ч
        FNE_DEV_IP = 1,                                      // IP??ַ
        FNE_DEV_SEQUENCE,                                    // ��??????????
        FNE_DEV_TRI_TIME,                                    // ????ʱ??
        FNE_DEV_TRAFFIC_TIME,                                // ???ƺ??೤ʱ??
        FNE_DEV_SUFFIXES,                                    // ??׺
        FNE_DEV_NUMBER                                       // ????????
    } PicName[FILE_NAME_ELEMENT];
    OS_U8 Suffix[FILE_NAME_ELEMENT];                         // "-" "_"
} IPNC_PicFileName, *PIPNC_PicFileName;

typedef enum
{
    // ??Ϣ?? ???? ????Ϣ???õ??㷨
    OSD_JNON = 0,                                            // ??Ч
    OSD_DEVPOSITION,                                         // ?豸λ?? ????,ITARGE
    OSD_DEVDIRECT,                                           // ?豸???? ????,ITARGE
    OSD_DEVNO,                                               // ?豸???? ????,ITARGE
    OSD_IPADDR,                                              // IP??ַ ????,ITARGE
    OSD_SNAPTIME,                                            // ץ??ʱ?? ????,ITARGE
    OSD_LANCE,                                               // ??????Ϣ ????,ITARGE
    OSD_CARNUMBER,                                           // ???ƺ??? ????,ITARGE
    OSD_OSDTIME,                                             // OSDʱ?? ????
    OSD_WFTYPE,                                              // Υ?????? ????
    OSD_CARINFO,                                             // ??????Ϣ ????
    OSD_SNAPINDEX,                                           // ץ?????? ITARGE
    OSD_CARCOLR,                                             // ??????ɫ ITARGE
    OSD_CONFIDENCE,                                          // ???Ŷ? ITARGE
    OSD_SPEED,                                               // ???? ITARGE
    OSD_OVERSPEEDRATE,                                       // ???ٱ? ITARGE
    OSD_SNAPTYPE,                                            // ץ?????? ITARGE
    OSD_CARTYPE,                                             // ??��???? ITARGE
    OSD_CARLOGO,                                             // ???? ITARGE
    OSD_LIMITSPEED,                                          // ???? ITARGE
    OSD_DEVNAME,                                             // ?豸???? ITARGE
    OSD_CARCOLOR,                                            // ??????ɫ ITARGE
    OSD_CUSTOM1,                                             // ?Զ???1 ITARGE
    OSD_CUSTOM2,                                             // ?Զ???2 ITARGE
    OSD_CUSTOM3,                                             // ?Զ???3 ITARGE
    OSD_CUSTOM4,                                             // ?Զ???4 ITARGE
    OSD_MD5,                                                 // MD5??α??        ITARGE
    OSD_RED_TIME                                             // ???ƺ?ʱ??
} ALG_OSDInfo;


typedef struct IPNC_ALG_OSDInfo                              // ?㷨OSD
{
    OS_S16 nDevPosition;                                     // ?豸λ??
    OS_S16 nDevDirect;                                       // ?豸????
    OS_S16 nDevNO;                                           // ?豸????
    OS_S16 nIPAddr;                                          // IP??ַ
    OS_S16 nSnapTime;                                        // ץ??ʱ??
    OS_S16 nLance;                                           // ??????Ϣ
    OS_S16 nCarNumber;                                       // ???ƺ???

    OS_S16 nX;                                               // ?ഹֱ??????Ե
    OS_S16 nY;                                               // ??ˮƽ??????Ե
    OS_S16 nInfoPosition;                                    // ????λ?? 0:???? 1:????
    OS_S16 nFontSize;                                        // ??????С 12,14,16,18,20,24
    OS_S16 nOsdRed;                                          // OSD??Ϣ??ɫ-??
    OS_S16 nOsdGreen;                                        // OSD??Ϣ??ɫ-??
    OS_S16 nOsdBule;                                         // OSD??Ϣ??ɫ-��
    OS_S16 nOsdTime;                                         // OSDʱ??
    OS_S16 nOneLineShow;                                     // ??????Ϣһ????ʾ 0:??(??ÿ????Ϣ????ռ??һ??) 1:??
    OS_S16 nwfType;                                          // Υ??????
    OS_S16 nCarinfo;                                         // ??????Ϣ

    OS_S16 nSnapIndex;                                       // ץ??????
    OS_S16 nCarColr;                                         // ??????ɫ
    OS_S16 nConfidence;                                      // ???Ŷ?
    OS_S16 nSpeed;                                           // ????
    OS_S16 nOverSpeedrate;                                   // ???ٱ?
    OS_S16 nSnapType;                                        // ץ??????
    OS_S16 nCarType;                                         // ??��????
    OS_S16 nCarlogo;                                         // ????
    OS_S16 nLimitSpeed;                                      // ????
    OS_U8 DevName;                                           // ?豸????
    OS_U8 CarColor;                                          // ??????ɫ
    OS_U8 custom1;                                           // ?Զ???1
    OS_U8 custom2;                                           // ?Զ???2
    OS_U8 custom3;                                           // ?Զ???3
    OS_U8 custom4;                                           // ?Զ???4

    OS_U8 Devdirect[32];                                     // ?豸????
    OS_U8 DevNO[32];                                         // ?豸????
    OS_U8 DevPos[128];                                       // ?豸λ??
    OS_U8 order[48];                                         // ?洢??????��??˳??
    OS_U8 osdtype;                                           // OSD???????? 1:????ͼƬ 2:?ϳ?ͼƬ 3:??Ƶ
    OS_U8 enable;                                            // ?Ƿ?????OSD 0:?????? 1:????
    OS_U8 Recv[590];                                         // ??????
} IPNC_ALG_OSDInfo, *PIPNC_ALG_OSDInfo;



typedef enum
{
    // ??Ϣ?? ???? ????Ϣ???õ??㷨
    FNE_JNON = 0,                                            // ??Ч
    FNE_JIP = 1,                                             // IP??ַ ????,ITARGE
    FNE_JTIME,                                               // ʱ?? ????,ITARGE
    FNE_JLANE_INFO,                                          // ??????Ϣ ????,ITARGE
    FNE_JDEV_INFO,                                           // ?豸??Ϣ ????
    FNE_JDEV_DIR,                                            // ?豸???? ????
    FNE_JDEV_TYPE,                                           // Υ?????? ????
    FNE_JDEV_CARINFO,                                        // ??????Ϣ ????
    FILE_DEV_POS,                                            // ?豸λ?? ITARGE
    FILE_SNAP_TYPE,                                          // ץ?????? ITARGE
    FILE_PIC_INDEX,                                          // ץ?????? ITARGE
    FILE_PLATE_NUM,                                          // ???ƺ??? ITARGE
    FILE_PLATE_COL,                                          // ??????ɫ ITARGE
    FILE_CONFIDENCE,                                         // ???Ŷ? ITARGE
    FILE_CAR_TYPE,                                           // ??��???? ITARGE
    FILE_SPEED,                                              // ?ٶ? ITARGE
    FILE_OVERSPEED_RATE,                                     // ???ٱ? ITARGE
    FILE_LIMITSPEED,                                         // ???? ITARGE
    FILE_DEV_NUM,                                            // ?豸???? ITARGE
    FILE_DEV_NAME,                                           // ?豸???? ITARGE
    FILE_CUSTOM1,                                            // ?豸???? ITARGE
    FILE_CUSTOM2,                                            // ?豸???? ITARGE
    FILE_CUSTOM3,                                            // ?豸???? ITARGE
    FILE_CUSTOM4,                                            // ?豸???? ITARGE
    FILE_CAR_COLOR,                                          // ??????ɫ ITARGE
    FILE_RED_TIME,                                           // ???ƺ?ʱ??
    FILE_CAR_BODY_POS,                                       // ????????
    FILE_CAR_PL_POS                                          // ????????
} ALG_FileName;

// ?㷨?ļ???????
typedef struct IPNC_ALG_FileName
{
    OS_S32 nNum;                                             // ??????Ϣ????
    OS_U8 link;                                              // ��?ӷ? "_"?? "??"
    OS_U8 ALG_linkElement[25];                               // ?????ļ???Ԫ????Ϣ ????ֵΪALG_FileName??ֵ;
    OS_S16 ver;                                              // ????
    OS_S32  nrev[3];                                         // ????
} IPNC_ALG_FileName, *PIPNC_ALG_FileName;


typedef struct _VirtualDir_Time
{
    OS_S32 nh264Duartion;                                    // h264(1-10)????
    OS_S32 nRev[4];
} VirtualDir_Time;



typedef enum
{
    E_SIP_SPECIAL_TYPE_STANDARD = 0,
    E_SIP_SPECIAL_TYPE_VISS     = 32
} E_SIP_SPECIAL_TYPE;

typedef struct AppSpecial_viss
{
    OS_S32 rtpOverFlag;
    OS_S8 cameraId[21];                                      // camara????
    OS_U8 resv[3];
} AppSpecial_viss;

typedef struct App_CwmpInfo_t
{
    int     cwmp_enable;// 0 disable    1 enable
    char  acs_url[64];  //����ƽ̨������URL
    char  acs_user[32];//����ƽ̨�˺�
    char  acs_pwd[32];//����ƽ̨����
    int   acs_HBperiodic;// ƽ̨��������
    int   rev;//����
}App_CwmpInfo;

typedef struct _IPNC_SipIpcConfig
{
    OS_U8  sipIpcVersion[4];                                 // sip ipc ???ð汾.
    OS_U16 sipSpecialAppType;                                // ?ػ?????
    OS_U16 bUseSipDevice ;                                   // ?????豸??־
    OS_S8 sipRegServerUID[21];                               // ע??SIP??????????
    OS_S8 sipRegServerDomain[11];                            // ע??SIP??????????
    OS_S8 sipRegServerIP[16];                                // ע??SIP??????IP
    OS_S8 sipRegServerPwd[32];                               // SIP?豸ע??????(??ע??????????֤)
    OS_U16 sipRegServerPort ;                                // ע??SIP???????˿?
    OS_U16 sipDevicePort ;                                   // ?豸???ض˿?
    OS_S8 sipDeviceUID[21];                                  // SIP?豸????
    OS_S8 sipDeviceDoman[11] ;                               // sip?豸????
    OS_S32 regExpire_s ;                                     // ע????Ч??(??)
    OS_U16 sipDevHBCycle_s;                                  // ?豸????????(??)
    OS_U16 sipDevHBOutTimes;                                 // ?豸??ʱ????????
    OS_S8 sipAlarmID[24];                                    // ����ͨ��ID���
    OS_S8 sipVideoChannelID[3][24];                          // ��Ƶͨ��ID���
    OS_U8 resv[120-24-72];                            // for comm ������
    AppSpecial_viss sip_viss;
    App_CwmpInfo  viss_cwmp;       //�Խ�����ƽ̨����
    char specialVerData[256-28-140]; //�ػ��汾������
} IPNC_SipIpcConfig;

typedef struct IPNC_RESTARTParam_Data
{
    OS_S32 iEnable;                                          // ?Ƿ????????? 0?????? 1???????? 2?̶?ʱ??????
    OS_S32 nInterval;                                        // ???????����?λ?֣?
    IPNC_DateTime FixedTime;                                 // ?̶?ʱ????????????????ʼʱ??
    OS_S32 revr[5];
} IPNC_RESTARTParam_Data;



typedef struct IPNC_WDRInfo
{
    OS_S32 bUseable;                                         // 0 ?ر?1????
    OS_S32 bValue;                                           // ??ֵ-5 Ĭ??Ϊ
    OS_U16 value;                                            // ????˫Ŀȫ??ͼ??  ???ұ??˵???
    OS_U16 Rev;                                              // ????
    OS_S32 bRev[2];                                          // ????
} IPNC_WDRInfo, *PIPNC_WDRInfo;


// ?????нṹ??
typedef struct CutLineInfo
{
    OS_S32 bOpen;                                            // 0 ?رմ????? 1???����???
    OS_S32 LaneNum;                                          // ?????г???????(??Χ1-3) ??2?����?(1??2)??ѡ  ????ѡ??һ?����?  ??ֵ1=????1 ??ֵ2=????2 ??ֵ3=????1+????2
    OS_S32 initLaneSeq;                                      // ??ʼ???��?(??Χ0-9)
    OS_S32 nRev[20];                                         // ????
} CutLineInfo, *PCutLineInfo;

enum
{
    CODEC_FIRST_AE_PARAM = 0,
    CODEC_SECOND_AE_PARAM,
};
enum
{
    CODEC_MJPG_RESO_PARAM = 0,
    CODEC_H264_RESO_PARAM,
};
#define IPNC_RESO_SELECT_NUM (8)
typedef struct IPNC_ResoInfo
{
    OS_S32 width;
    OS_S32 height;
    OS_S32 resv;
} IPNC_ResoInfo;

typedef struct IPNC_CodecAttri
{
    OS_S32             num;
    IPNC_ResoInfo   ResoInfo[IPNC_RESO_SELECT_NUM];
} IPNC_CodecAttri, *PIPNC_CodecAttri;

// !channel config info
typedef struct IPNC_CodecConfigAttri
{
    OS_S32     nChlID;
    OS_S32       width;
    OS_S32       height;
    OS_S32     nIFrmInterval;
    OS_S32        nCodecProfile;                             // 0:baseline 1: main 2:high
    OS_S32        nBitCtrl;                                  // 0:vbr 1:cbr
    OS_S32        nBitrate;
    OS_S32     nResv[24];
} IPNC_CodecConfigAttri, *PIPNC_CodecConfigAttri;

typedef struct IPNC_AESecondParam
{
    OS_S32     nEnable;
    OS_S32     nExp;
    OS_S32     nGain;
    OS_S32     nMaxExp;
    OS_S32     nMinExp;
    OS_S32     nMaxGain;
    OS_S32     nMinGain;
    OS_S32     nResv[24];
} IPNC_AESecondParam, *PIPNC_AESecondParam;

#define PATH_LNE_MAX 512
typedef struct IPNC_DirName
{
    OS_U8        type;                                       // ???????? '0':Ŀ¼  '1':?ļ?
    OS_U8        szName[0];                                  // ??Ŀ¼???????ļ???
} IPNC_DirName, *PIPNC_DirName;

typedef struct IPNC_FileDir
{
    OS_S32 nCount;                                           // ????????
    OS_U8 body[0];                                           // ??????Ϣ
} IPNC_FileDir, *PIPNC_FileDir;


#define PACKET_ASYN_HEADER  (0xA55CA55C)
typedef enum
{
    AVI    = 1,
    CONFIG = 2,
    H264Frame = 3,
    JpegFrame = 4
} recvDataType;                                              // ????????

typedef struct recvDataHead
{
    OS_U32        syncHeader;                                // ͬ??ͷ0xA55CA55C;
    recvDataType type;                                       // ????????
    OS_U8 datanamebuf[1024];                                 // ?ļ???ȫ·??
    OS_S32  datanamelen;                                     // ?ļ???ȫ·?????? strlen
    OS_U32 recvdataLen;                                      // ??Ҫ???յ????ݳ??? (?????ݲ????ó???size of OS_U32)
} recvDataHead;
struct H264FrameParm
{
    OS_F32 nFrame;
    OS_U16 nWidth;
    OS_U16 nHeight;
    OS_S32 nRev[4];
};

//
// ?��???
//
typedef struct IPNC_FlashLamp
{
    enum EFlashLampType
    {
        eLED = 1,                                            // LED??
        eGas,                                                // ??????
        eStroboFlash,                                        // Ƶ?��?
        eLight                                               // ???��?
    } Type;                                                  // ????

    enum EFlashLampTriggState
    {
        eClose = 0,                                          // ????
        eOpen,                                               // ????
    } TriggState;                                            // ????????

    OS_S32 Pulse;                                            // ????????????us
    OS_S32 Timelag;                                          // ??????ʱus
    OS_S32 SnapTimelag;                                      // ץ????ʱus
    OS_S32 Reserved;
} IPNC_FlashLamp, *PIPNC_FlashLamp;

//
// ????????????
//

// ???ô???????ʱ??Ҫ??ȷ????????TTL????????485????
// ��?????ʹ??????Ͳ???ͬʱʹ??
typedef enum
{
    eTTL1 = 0x01,
    eTTL2,
    eTTL3,
    eTTL4,
    eTTL5,
    eTTL6,
    eTTL7,
    eTTL8
} ETriggInput;                                               // ֧??8·????????

typedef enum
{
    eTrailingEdge = 0,                                       // ?½???
    eRisingEdge                                              // ??????
} ETriggEdge;                                                // ?ظ???Ȧ????????

typedef enum
{
    eLowLevel = 0,                                           // ?͵?ƽ
    eHighLevel                                               // ?ߵ?ƽ
} ETriggLevel;                                               // ???̵???Ȧ????????

typedef enum
{
    eNot = 0,
    eAnd = 1
} EOperator;                                                 // ????????????????????

// ????
#define ALARM_SIT_NUMBER 16
#define LAMP_NUMBER 4

typedef enum
{
    eDriveWayNON = 0,                                        // ?޳?????Ϣ
    eDriveWay1 = 1,                                          // 1????
    eDriveWay2,                                              // 2????
    eDriveWay3,                                              // 3????
    eDriveWay4                                               // 4????
} EDriveWay;

typedef struct IPNC_SnapCondition
{
    OS_U32 bEnable;                                          // ?Ƿ???Ч
    EDriveWay DriveWay;                                      // ???����?
    ETriggInput TriggInput1;                                 // ????????1
    ETriggEdge Input1TriggEdge;                              // ????1״̬
    EOperator Operator;                                      // ??????
    ETriggInput TriggInput2;                                 // ????????2
    ETriggLevel Input2TriggLevel;                            // ????2״̬
    OS_S32 nSnapNumber;                                      // ץ??????
    OS_S32 nSnapInterval;                                    // ץ?ļ???ms
    OS_U8 chFileNameSuffix[4];                               // ?????????ļ?????׺??2-3??????ʾ?ַ?
    OS_U32 bFlashLamp[LAMP_NUMBER];                          // ?????��???
    OS_U32 bTurns;                                           // ?Ƿ?????
} IPNC_SnapCondition, *PIPNC_SnapCondition;

// ????????????
typedef struct IPNC_TriggInputSet
{
    IPNC_SnapCondition SnapCondition[ALARM_SIT_NUMBER];      // ֧??16??????????
} IPNC_TriggInputSet, *PIPNC_TriggInputSet;

// ?????ļ?????
typedef enum
{
    eConfigFile_All,                                         // ȫ????????Ϣ
    eConfigFile_Network,                                     // ??????????Ϣ
    eConfigFile_TriggInputSet                                // ????????????????
} EConfigFileType;

// ????IPNC??IPNCӦ????Ϣ
#define MAC_LENGTH              6                            // Length of MAC address.
#define RESERVED_LENGTH         8                            // Length of MAC address.
typedef struct IPNC_Info
{
    IN_ADDR IPAddr;                                          // IP??ַ
    IN_ADDR NetMask;                                         // ????????
    IN_ADDR Gateway;                                         // ????
    OS_U8 MacAddr[MAC_LENGTH];                               // ?豸MAC??ַ
    OS_U8 Reserved[2];                                       // ????

    OS_S32 nCtrlPort;                                        // ???ƶ˿?

    OS_U8 szDeviceName[DEVICE_NAME_LENGTH];                  // ?豸???? ֻ??
    OS_U8 szDeviceAppVer[DEVICE_APPVER_LENGTH];              // ?豸Ƕ??ʽ?????汾
    // ֻ??
    OS_U8 sdkReserved1[DEVICE_RESERVED_LENGTH];              // SDK????,?û???Ҫʹ?ú??޸?

    OS_U8 szDeviceNumber[DEVICE_NUMBER_LENGTH];              // ?豸????
    OS_U8 szDeviceDirection[DEVICE_DIRECTIONLENGTH];         // ????????
    OS_U8 szDevicePosition[DEVICE_POSITION_LENGTH];          // ?豸λ??
    OS_U8 sdkReserved2[RESERVED_LENGTH];                     // SDK????,?û???Ҫʹ?ú??޸?
} IPNC_Info, *PIPNC_Info;

/************************IPNC answer info******************************/
#define DEVICE_SECRET_ID_LENGTH 8
typedef struct IPNC_AnswerInfo
{
    struct in_addr szIPAddr;                                 // ?豸 IP ??ַ
    struct in_addr szSubnetMask;                             // ?豸????????
    struct in_addr szGateway;                                // ?豸????
    OS_U8 szMacAddr[MAC_LENGTH];                             // ?豸MAC??ַ
    OS_U8 product_name[32];
    OS_U8 product_version[32];
    OS_U8 nicp_version[32];
    OS_U8 alg_version[32];
    OS_U8 build_time[32];
    OS_S32 nTransPort;                                       // ?????˿?,?????˿?????״̬????
    OS_S32 nCtrlPort;                                        // ???ƶ˿?
    OS_U8 device_number[DEVICE_NUMBER_LENGTH];
    OS_U8 device_direction[DEVICE_DIRECTIONLENGTH];
    OS_U8 device_position[DEVICE_POSITION_LENGTH];
    OS_U8 device_type[DEVICE_TYPE_LENGTH];
    OS_U8 device_sn[DEVICE_SN_LENGTH];
    OS_U8 secret_id[DEVICE_SECRET_ID_LENGTH];
    OS_U32 kernel_version;                                   // kernel?汾?? 368-> OS_U8 kernalVer[4];
    OS_U8 szEncrypt;                                         // ?Ƿ????? 1:???? 0:δ????
    OS_U8 szMacType;                                         // 0:????????mac 1:yihulu mac
    OS_U8 szReserved[2];                                     // ????
    OS_S16 ddrMHz;                                           // ?ڴ?DDRʱ??Ƶ??
    OS_S16 armMHz;                                           // CPU-arm ʱ??Ƶ??
    OS_U8 dsp_version[8];                                    // SmartCamer 6437 dsp lib version
    /* Added For Barrett Product Begin */
    OS_U32 product_type;                                     // ??Ʒ???ͣ?0-> 368: 1->Barrett: 2-Kepler ........
    OS_U32 fpga_version;                                     // FPGA?汾??
    OS_U32 uboot_version;                                    // U-boot?汾??
    OS_U32 app_version;                                      // Ӧ?ó????汾??
    OS_U32 hw_id;                                            // Ӳ??ʶ??
    /* Added For Barrett Product End */
    OS_S32 resv[13];                                         // ????
} IPNC_AnswerInfo;

#define SOCK_JPGFILE_NAME               "sock_photofile"
#define DAGUERRE_BC_CMD_HEADER          "iTarge_IPNC_Daguerre_CMD"
#define DAGUERRE_BC_SEARCH_HEADER       "Where_iTarge_IPNC_Daguerre"
#define DAGUERRE_BC_CHANGE_IP           "iTarge_IPNC_Change_IP"
#define DAGUERRE_BC_NICP_RESTART        "iTarge_IPNC_BReStart"
#define DAGUERRE_BC_ITARGE_CHANGE_IP    "iTarge_TEST_Change_IP"

typedef struct IPNC_ChangeIP
{
    IN_ADDR IPAddr;                                          // IP??ַ
    IN_ADDR NetMask;                                         // ????????
    IN_ADDR Gateway;                                         // ????
    OS_U8 MacAddr[MAC_LENGTH];                               // ?豸MAC??ַ
    OS_U8 Reserved[2];
} IPNC_ChangeIP, *PIPNC_ChangeIP;

// ?Զ??ع??㷨״̬
typedef enum
{
    AE_Close,                                                // ?㷨?ر?
    AE_Open                                                  // ?㷨????
} AEState;

// ?ع?????(?ر??Զ??ع??㷨)
typedef struct IPNC_AEParam
{
    OS_S32 nExp;                                             // ?ع?ʱ??
    OS_S32 nGain;                                            // ????
} IPNC_AEParam, *PIPNC_AEParam;

// ?Զ???ƽ???㷨״̬
typedef enum
{
    AWB_Close,                                               // ?㷨?ر?
    AWB_Open                                                 // ?㷨????
} AWBState;

// ??ƽ??????(?ر??Զ???ƽ???㷨)
typedef struct IPNC_AWBParam
{
    OS_S32 nRedGain;                                         // ??ɫ????
    OS_S32 nGreenGain;                                       // ??ɫ????
    OS_S32 nBlueGain;                                        // ��ɫ????
} IPNC_AWBParam, *PIPNC_AWBParam;

// ?��???״̬
typedef enum
{
    FL_Close,                                                // ?��??ƹر?
    FL_Open                                                  // ?��??ƴ???
} FlashLampState;

// ??????ʽ
typedef enum
{
    eAppReset,                                               // ????????
    eSysReset                                                // ϵͳ????
} EResetStyle;

typedef enum
{
    eSoft,                                                   // ?���??
    ebayonet,                                                // ????
    eRedlight,                                               // ????
    eRS485,                                                  // RS485????
    eRS232,                                                  // RS232?״ﴥ??
    eSpeed,                                                  // ???ٴ???
    eStream,                                                 // ??
    bVideoDetecTrig                                          // ??Ƶ???ⴥ??
} ESnapType;

typedef unsigned long ITE_HANDLE;
#define INVALID_ITE_HANDLE (-1)                              // ??Ч????

// ץ??ͼƬ??????Ϣ
typedef struct IPNC_SnapPicInfo
{
    ITE_HANDLE hCamera;                                      // ??????????
    ESnapType SnapType;                                      // ץ??????
    IPNC_TIME SnapTime;                                      // ͼƬ?ع?ʱ??

    // ??????Ϣֻ????IO????????????Ч
    OS_U32 RedLampTime;                                      // ͼƬ?ع?ʱ????ʱ??
    OS_S32 SnapIndex;                                        // ץ??????
    OS_U8 Suffix[4];                                         // ץ???ļ?????׺

    OS_S32 TriIndex;                                         // ??????????
    EDriveWay DriveWay;                                      // ??????Ϣ

    OS_S32 nSpeed;                                           // ?ٶ?
    OS_S32 nSpeedMax;                                        // ????
    OS_S32 nCoilTime;                                        // ??Ȧʱ????(??λ:ms)
    OS_U8 cCarMum[16];                                       // ???ƺ???
    OS_S32 nWorkMode;                                        // ??????????ģʽ(0:???? 1:ҹ??)
    OS_S32 nTD1;                                             // ץ????ʱ(??λus)
    OS_S32 nTD2;                                             // ?ع???ʱ(??λus)
    OS_S32 nBrightAVG;                                       // ƽ???��?

} IPNC_SnapPicInfo, *PIPNC_SnapPicInfo;

//
// ??Ƶ?????Ͷ???
//
// ??ͬ?ͺŵ???????֧?ֲ?ͬ??Ƶ?????ԣ?Ŀǰ??��??
// 1??֧??JPG˫?????룬һ·??????׼ͼƬ??????һ·?????ͷֱ???ͼ????640*480????
// 2??֧?ֱ?׼ͼƬ???ȵ??????и????֧??4???ȵ???????
// 3??֧??????ͼƬ???????á?

// ??Ƶ??JPGͼƬ????
#if 0                                                        // ?ʱ??
typedef enum
{
    eNormal = (0x1 << 0),                                    // ??׼?ֱ???ͼƬ
    eLowPixel = (0x1 << 1),                                  // ˫???????ĵڶ?·ͼƬ
    eHot1 = (0x1 << 2),                                      // ?ȵ?????1ͼƬ
    eHot2 = (0x1 << 3),                                      // ?ȵ?????2ͼƬ
    eHot3 = (0x1 << 4),                                      // ?ȵ?????3ͼƬ
    eHot4 = (0x1 << 5)                                       // ?ȵ?????4ͼƬ
} EStreamPicType;
#endif

// ??Ƶ??JPGͼƬ????
typedef enum
{
    // eNormal = (0x1 << 0), // ??׼?ֱ???ͼƬ
    eOldPic = 1,                                             // ?ɰ汾????????????ͼƬ
    eStreamPic = 2,                                          // JPG??ͼƬ
    eSnapPicCloseLamp = 3,                                   // ͬʱ????ץ?ĵ?JPG??ͼƬ????δ?????��???
    eSnapPicOpenLamp = 4                                     // ͬʱ????ץ?ĵ?JPG??ͼƬ??ͬ???????��???
} EStreamPicType;

// ??׼ͼƬ???ȵ??????Ľ???????
typedef enum
{
    eNormalRegion = (0x1 << 0),
    eLowPixelRegion = (0x1 << 1),
    eHotRegion1 = (0x1 << 2),
    eHotRegion2 = (0x1 << 3),
    eHotRegion3 = (0x1 << 4),
    eHotRegion4 = (0x1 << 5),
} EStreamRegionType;

// ͼƬ??????С
typedef struct IPNC_StreamRegion
{
    OS_S32 topx;
    OS_S32 topy;
    OS_S32 botx;
    OS_S32 boty;
} IPNC_StreamRegion, *PIPNC_StreamRegion;

// ͼƬ?ȵ???????Ϣ
#define IPNC_HOTREGION_NUM  4
typedef struct IPNC_StreamHotRegion
{
    IPNC_StreamRegion HotRegion[IPNC_HOTREGION_NUM];
} IPNC_StreamHotRegion, *PIPNC_StreamHotRegion;

// ??ͼƬ??Ϣ
typedef struct IPNC_StreamPicInfo
{
    ITE_HANDLE hCamera;                                      // ??????????
    EStreamPicType picType;                                  // ??ͼƬ????
    IPNC_TIME picExpTime;                                    // ͼƬ?ع?ʱ??
    OS_U32 dwReserved;
} IPNC_StreamPicInfo, *PIPNC_StreamPicInfo;

/*
                                                             // ??ͼƬ??С????????
typedef struct IPNC_StreamJPGSizeParam
{
    OS_S32 nFlag;                                            // 0:??ʹ??  1:ʹ??
    OS_S32 nMaxSize;                                         // ͼƬ??С????
}IPNC_StreamJPGSizeParam,*PIPNC_StreamJPGSizeParam;
*/

// SD??״̬

typedef struct IPNC_SDCardState
{
    enum
    {
        eSDCardPull = 0,                                     // δ????
        eSDCardInserted = 1                                  // ?Ѳ???
    } hasSDCard;
    OS_U32 dwTotalCapacity;                                  // SD??????�� MB???ף?Ϊ??λ
    OS_U32 dwFreeCapacity;                                   // SD??ʣ????????��
} IPNC_SDCardState, *PIPNC_SDCardState;

// SD???ļ?????
typedef enum
{
    eSubdirectory = 0,                                       // ??Ŀ¼
    eJPGPic = 1                                              // ͼƬ?ļ?
} ESDFileType;

// SD??Ŀ¼????
#define MAX_SD_PATH 256

typedef struct IPNC_SDCardFileInfo
{
    ESDFileType type;                                        // ????????
    OS_U8 szName[MAX_SD_PATH];                               // ??Ŀ¼???????ļ???
} IPNC_SDCardFileInfo, *PIPNC_SDCardFileInfo;

typedef struct IPNC_SDCardDirectory
{
    OS_U8 szCurDirectory[MAX_SD_PATH];                       // ??ǰ????·??
    // ?ַ?'\'??ʾ??Ŀ¼
    OS_U32 dwFileCount;                                      // ??Ŀ¼???ļ???��
    IPNC_SDCardFileInfo *pFileInfo;
} IPNC_SDCardDirectory, *PIPNC_SDCardDirectory;

// RS485
typedef struct IPNC_RS485Param
{
    OS_S32 nBaudRate;                                        // ??????
    OS_U8 cCheckBit[4];                                      // У??λ(ֻ??cCheckBit[0]????) N:??
    // O:?? E:ż M:??־ S:?ո?
    OS_S32 nDataBit;                                         // ????λ
    OS_S32 nStopBit;                                         // ֹͣλ
    OS_S32 nType;                                            // ?????????? 1:?Ϻ?ͼ��
    // 2:????ɣ?? 3:?Ϻ???ά 4:???ɻ???
} IPNC_RS485Param, *PIPNC_RS485Param;

// RS232
typedef struct IPNC_RS232Param
{
    OS_S32 nBaudRate;                                        // ??????
    OS_U8 cCheckBit[4];                                      // У??λ N:?? O:?? E:ż M:??־
    // S:?ո?
    OS_S32 nDataBit;                                         // ????λ
    OS_S32 nStopBit;                                         // ֹͣλ
} IPNC_RS232Param, *PIPNC_RS232Param;

// ??Ȧ????
typedef enum
{
    eLane1 = 1,                                              // 1????
    eLane2,                                                  // 2????
    eLane3,                                                  // 3????
    eLane4                                                   // 4????
} EDriveLane;

typedef enum
{
    eRadraSel1 = 1,                                          // ?״?1
    eRadraSel2,                                              // ?״?2
    eRadraSel3,                                              // ?״?3
    eRadraSel4                                               // ?״?4
} ERadraSel;

typedef enum
{
    eInPut1 = 1,
    eInPut2,
    eInPut3,
    eInPut4,
    eInPut5,
    eInPut6,
    eInPut7,
    eInPut8,
} EIOInput;                                                  // ֧??8????

typedef struct LaneInfo
{
    OS_S32 nEnable;                                          // ?Ƿ???Ч 1:??Ч 0:??Ч
    OS_S32 DriveLane;                                        // ???����?
    ERadraSel RadraSel;                                      // ?״?ѡ??
    EIOInput Input1;                                         // I/Oѡ??1
    EIOInput Input2;                                         // I/Oѡ??2
    OS_F32 fDistance;                                        // ??Ȧ????
    OS_F32 fSpeedMax;                                        // ????
} LaneInfo, *PLaneInfo;

typedef struct LaneExInfo
{
    OS_S32 nEnable;                                          // ?Ƿ???Ч 1:??Ч 0:??Ч
    OS_S32 DriveLane;                                        // ???����?
    ERadraSel RadraSel;                                      // ?״?ѡ??
    EIOInput Input1;                                         // I/Oѡ??1
    EIOInput Input2;                                         // I/Oѡ??2
    OS_F32 fDistance;                                        // ??Ȧ????
    OS_F32 fSpeedMax;                                        // ????
    OS_S32 nCount;                                           // ????ץ??????
    OS_S32 nDelay;                                           // ʱ?????? ??λ:ms
    OS_S32 nSpeedCount;                                      // ????ץ??????
    OS_S32 PrecentMaxSpeed;
    OS_S32 nReserve[2];                                      // ????
} LaneExInfo, *PLaneExInfo;

typedef enum
{
    eRadar = 0,                                              // ?״?????
    eCoil = 1                                                // ??Ȧ????
} ESpeedType;

typedef enum
{
    eChuanSuWeiBo = 0,                                       // ????΢??
    eAndoray = 1                                             // ??????
} ERadarType;

#define LANECOUNT 4

typedef struct IPNC_SpeedParam
{
    ESpeedType SpeedType;
    ERadarType RadarType;
    OS_S32 nCurSpeed;                                        // ?Ƿ????ӵ?ǰ???? 1:????
    // 0:??????
    OS_S32 nSpeedMax;                                        // ?Ƿ????????? 1:???? 0:??????
    OS_S32 nOverSpeed;                                       // ?Ƿ????ӳ??? 1:???? 0:??????
    LaneInfo LaneParam[LANECOUNT];

} IPNC_SpeedParam, *PIPNC_SpeedParam;

typedef struct IPNC_SpeedExParam
{
    ESpeedType SpeedType;
    ERadarType RadarType;
    OS_S32 nCurSpeed;                                        // ?Ƿ????ӵ?ǰ???? 1:????
    // 0:??????
    OS_S32 nSpeedMax;                                        // ?Ƿ????????? 1:???? 0:??????
    OS_S32 nOverSpeed;                                       // ?Ƿ????ӳ??? 1:???? 0:??????
    LaneExInfo LaneParam[LANECOUNT];

} IPNC_SpeedExParam, *PIPNC_SpeedExParam;

typedef enum
{
    eOnLine = 0,                                             // ????
    eNoLine = 1                                              // ??????
} EState;

typedef enum
{
    eDay = 0,                                                // ????
    eNight = 1                                               // ҹ??
} EWorkMode;

typedef enum
{
    eFocus = 0,                                              // ????ģʽ
    eCatch = 1                                               // ץ??ģʽ
} ECameraMode;

typedef enum
{
    eNoSyn = 0,                                              // ??ͬ??
    eSyn = 1                                                 // ????ͬ??
} EAutoSyn;

typedef enum
{
    eDC = 0,                                                 // ֱ??
    eAC = 1                                                  // ????
} EElectricity;

// ?���??
typedef struct IPNC_MSoftParam
{
    OS_S32 nCount;                                           // ץ??????
    OS_S32 nWaitTime;                                        // ץ?ļ???(ms)
    OS_S32 nLamp;                                            // Ҫ???Ƶ??��???(??λ????,?????????ĸ??��???)
    EDriveWay DriveWay;                                      // ???����?
    OS_S32 nSpeed;                                           // ????
    OS_S32 nSpeedMax;                                        // ????
    OS_U8 cCarMum[16];                                       // ???ƺ???
    OS_S32 pContext;                                         // ????????Ϣ
} IPNC_MSoftParam, *PIPNC_MSoftParam;

#define LAMP_COUNT  4
typedef struct IPNC_LampParam
{
    OS_S32 nMode[LAMP_COUNT];                                // 0:???? 1:????
    OS_S32 nLampType[LAMP_COUNT];                            // TS:ǰ3????Ĭ??ΪƵ?��?
    // ???ĸ????? 1:Ƶ?��? 2: LED 3: ????
    // TC:4???ƶ?????????(??????????Ƶ?��?) 2: LED 3: ????
} IPNC_LampParam, *PIPNC_LampParam;

typedef struct IPNC_SDCardJPG
{
    OS_S32 nType;                                            // 0:???? 1:ɾ??
    IN_ADDR IPAddr;                                          // IP??ַ
    OS_S32 nPort;                                            // ?˿ںţ?35568??
} IPNC_SDCardJPG;


typedef struct IPNC_ImageMergeType
{
    OS_S32 merge_enable;                                     // 0?رգ?1????(ͼƬ?ϳ?????)
    OS_S32 plate_mul;                                        // Ĭ??Ϊ4??????1-8(??д???ƷŴ?????)
    OS_S32 plate_change;                                     // Ĭ??Ϊ1??????1-3(??д????ѡ??)
    OS_U16 bayonet_one_merge;                                // Ĭ??Ϊ0??????1-22(???ڵ?ͼͼƬ?ϳɷ?ʽ)
    OS_U16 bayonet_expand_height;                            // Ĭ?? 120??????ѡ???? 120   180   240
    OS_S32 bayonet_mul_merge;                                // Ĭ??Ϊ0??????1-22(????Υ?¶?ͼͼƬ?ϳɷ?ʽ)
    OS_U16 police_one_merge;                                 // Ĭ??Ϊ0??????1-22(???ڵ?ͼͼƬ?ϳɷ?ʽ)
    OS_U16 police_expand_height;                             // Ĭ?? 120??????ѡ???? 120   180   240
    OS_S32 police_mul_merge;                                 // Ĭ??Ϊ0??????1-22(???Ӿ???Υ?¶?ͼͼƬ?ϳɷ?ʽ)
    OS_S32 upload;                                           // Ĭ??Ϊ0??????1(SDK?ϴ??Ǻϳ??ļ???????ʽ)
} IPNC_ImageMergeType, *PIPNC_ImageMergeType;

typedef struct _POL_FAN_INFO
{
    OS_U8 ucMaxTmp;                                          // ?????????¶?
    OS_U8 ucMinTmp;                                          // ?رշ????¶?
    OS_U8 cAuto_Fan;                                         // 0ʹ???ֶ?????   1ʹ???Զ?????
    OS_U8 cFlag_Fan;                                         // ?ֶ????ȱ?־ 0Ϊ?ر? 1Ϊת??

    /*OS_U8 ucMaxBr;                                         // ƫ?⾵???ڴ??��?????
    OS_U8 ucMinBr;                                           // ƫ?⾵С?ڴ??��?????*/
    OS_U16 usShutter;                                        // ƫ?????عֵⷧ
    OS_U8 cAuto_Pol;                                         // 0ʹ???ֶ?ƫ?⾵  1ʹ???Զ?ƫ?⾵
    OS_U8 cFlag_Pol;                                         // ?ֶ?ƫ????־ 0Ϊ???? 1Ϊ????
} POL_FAN_info;

typedef struct _LIGHT_CON_INFO
{
    OS_S32  YellowEnable;                                    // ???��?״̬
    OS_U8 RedEnable;                                         // ????״̬
    OS_U8 RedNum;                                            // ????????
    OS_U8 GreenEnable;                                       // ?̵?״̬
    OS_U8 GreenNum;                                          // ?̵?????
} Light_Con_info;

typedef struct stFpsConf
{
    OS_U8 avsFps;// ƽ���ٶȷ���Ƶ�ʣ��û����룬��λ�����ӣ�
    OS_U8 resv[31];
}stFpsConf;

typedef struct stSignalLight
{
    OS_U8 light;// �źŵ�ѡ�� 0Ϊ��ת 1Ϊֱ�� 2Ϊ��ת 3Ϊ��ͷ���û�ѡ��
    OS_U8 resv[3];
}stSignalLight;

typedef struct IPNC_VIDEO_OSDGAT751      //��Ƶ���GAT751OSD ��������(H264   CVBS  HD_SDI)
{   
	char city[16];//����
	char prefecture[16];//����
	char town[16];//����ֵ�
	char site[48];//�ص�
	char IpcInfo[16];//�������Ϣ
	char	province[8];//ʡ
	char res[8];

} IPNC_VIDEO_OSDGAT751;

typedef struct stCustomServerConf
{
    OS_U32 enCustomTrans; // ʹ�� 0 ������ 1 ����
    OS_U8 serverIP[16]; // �����IP
    OS_U16 serverPort; // ����˶˿�
    OS_U16 transByUdp; // ���䷽ʽ 0 Tcp 1 udp ��ʱ��������
    OS_U8 roadNum[64]; // ·�ڱ��
    POL_FAN_info stPfPrm;  // 8char
    OS_U16 hearttime;//����������ʱʱ��
    OS_U16 greentime;//�����̵Ƴ�ʱʱ��
    Light_Con_info lightinfo[3];//0 Ϊ��ת 1Ϊֱ��  2Ϊ��ת 8char���û�ѡ��
    OS_U32 ucMaxBr;//ƫ�⾵���ڴ���������
    OS_U32 ucMinBr;//ƫ�⾵С�ڴ���������
    OS_U8 IPaddr[16]; // �źŻ�IP
    OS_U32 svacTest; // Svac ���� ��ʼ��Ϊ0�����Ϊ1
    OS_U32 port; // �˿�
    IPNC_VIDEO_OSDGAT751 gat751_osd; //128
    OS_U32 IPCNum;
    OS_U32 LevelEnable;
    stFpsConf fpsConf;// Ƶ����ز���
    stSignalLight signalLight;
    OS_U8 username[32]; // ������û���
    OS_U8 passwd[32]; // ���������
    OS_U8 resv[956-128-8-36-64]; // �����ֶ�
}stCustomServerConf; 


/* ????ֵ */
#define RET_ERR 1
#define RET_OK  0
#define RET_SDK_EXISTED -1                                   // ?Ѵ??? SDK ??һ??ʵ?? ITE_Initʧ??

#endif // __ITE_NET_SDK_H__
