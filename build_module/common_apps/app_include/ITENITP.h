/**
 * @file ITENITP.h
 * @brief  ͼƬͷ?ṹͷ?ļ?
 * @author <itarge@itarge.com>
 * @version 1.0.0
 * @date 2017-06-02
 */

/* Copyright (C) 2009-2016, Itarge Inc.
 * All right reserved
 *
 */
#ifndef __ITE_NITP_H__
#define __ITE_NITP_H__

#include <ITENetSDK.h>

typedef enum
{
    eNITPVer11 = MAKE_VER(1, 1)                             // 1.01?汾
} ENITPVer;

typedef struct
{
    OS_F32  rgb_mul_rr;                                     // -4.000~4.000
    OS_F32  rgb_mul_gr;                                     // -2.000~2.000
    OS_F32  rgb_mul_br;                                     // -2.000~2.000
    OS_F32  rgb_mul_rg;                                     // -2.000~2.000
    OS_F32  rgb_mul_gg;                                     // -4.000~4.000
    OS_F32  rgb_mul_bg;                                     // -2.000~2.000
    OS_F32  rgb_mul_rb;                                     // -2.000~2.000
    OS_F32  rgb_mul_gb;                                     // -2.000~2.000
    OS_F32  rgb_mul_bb;                                     // -4.000~4.000
    OS_S32    rgb_oft_or;                                   // 0~15
    OS_S32    rgb_oft_og;                                   // 0~15
    OS_S32    rgb_oft_ob;                                   // 0~15
} IPNC_ALG_COLOR_MATRIX;


typedef struct ITE_JPGHeadEx
{
    ENITPVer Ver;                                           // NITPЭ???汾
    OS_U32 PicLen;                                          // ͼƬ????
    ESnapType SnapType;                                     // ????????
    OS_U32 Context;                                         // ?���????????(IO??????Ч)
    union
    {
        struct timeval Exp_Timeval;                         // linux timeval
    } ExpTime;                                              // ?ع?ʱ??
    OS_U32 RedLampTime;                                     // ???Ƶ??��?ʱ??
    OS_S32 SnapIndex;                                       // ץ??????
    OS_U8 Suffix[4];                                        // ?ļ?????׺

    EDriveWay DriveWay;                                     // ??????Ϣ
    OS_S32 TriIndex;                                        // ??????????
    OS_S32 nSpeed;                                          // ?ٶ?
    OS_S32 nSpeedMax;                                       // ????
    OS_S32 nCoilTime;                                       // ??Ȧʱ????(??λ:ms)
    OS_S32 nCoilDis;                                        // ??Ȧ???루??λ:cm??
    OS_U8 cCarMum[16];                                      // ???ƺ???
    struct timeval Red_Timeval;                             // linux timeval ???ƿ?ʼʱ??

} ITE_JPGHeadEx, *PITE_JPGHeadEx;

typedef struct
{
    OS_U8 header[4];                                        // ��չͷ????־?ͳ???
    OS_U8 company[8];                                       // ??˾????
    ITE_JPGHeadEx JPGHeadEx;
} EXT_JFIF_SNAP;                                            // ץ??ͼƬͷ??��չ?ṹ

#define EXT_JFIF_SNAP_LEN_1     46
#define EXT_JFIF_SNAP_LEN_2     (sizeof(EXT_JFIF_SNAP) - 2)

#define EXT_JFIF_SNAP_LEN_3     90
typedef struct ITE_StreamHeadEx
{
    ENITPVer Ver;                                           // NITPЭ???汾
    OS_U32 PicLen;                                          // ͼƬ????
    EStreamPicType PicType;                                 // ͼƬ????
    union
    {
        struct timeval Exp_Timeval;                         // linux timeval
    } ExpTime;                                              // ?ع?ʱ??

    OS_U8 ICCardID[8];                                      // ????оƬID
    OS_U32 picIndex;                                        // ͼƬ???ţ?ѭ??ʹ?ã?
} ITE_StreamHeadEx, *PITE_StreamHeadEx;

typedef struct
{
    OS_U8 header[4];
    OS_U8 company[8];
    ITE_StreamHeadEx StreamHeadEx;
} EXT_JFIF_STREAM;                                          // ץ??ͼƬͷ??��չ?ṹ

// ��չͷ2
typedef struct
{
    OS_U8 header[4];                                        // [0] 0xFF [1] 0xE2 [2-3] ��չ??Ϣ???? ??4?ֽ?
    OS_U8 company[8];                                       // "itarge" ??8?ֽ?

    OS_U8 cameraType[32];                                   // ???????ͺ? ??32?ֽ?
    OS_U8 szIP[16];                                         // IP??ַ ??16?ֽ?
    OS_U8 sensorType[32];                                   // ???????ͺ? ??32?ֽ?
    OS_S32 expTime;                                         // ???? ??4?ֽ?
    OS_S32 gain;                                            // ???? ??4?ֽ?
    OS_S32 brightness;                                      // ?��? ??4?ֽ?
    OS_S32 contrast;                                        // ?Աȶ? ??4?ֽ?
    OS_S32 sharpness;                                       // ???? ??4?ֽ?
    OS_S32 saturation;                                      // ???Ͷ? ??4?ֽ? δʵ?֣??ݲ???ʾ
    OS_S32 redGain;                                         // ??ɫ???? ??4?ֽ?
    OS_S32 greenGain;                                       // ??ɫ???? ??4?ֽ?
    OS_S32 blueGain;                                        // ��ɫ???? ??4?ֽ?
    OS_S32 ecompensation;                                   // ?عⲹ?? ??4?ֽ?
    OS_S32 jpegquality;                                     // ͼƬ??�� ??4?ֽ?
    OS_S32 flashType;                                       // 01??LED 02???????? ??4?ֽ?
    OS_S32 triggerType;                                     // ͼƬ???? 0?���?? / 1???? / 2???? / 3 RS485 / 4 ?״? / 5???? / 6??ͼƬ ??4?ֽ?
    OS_S32 gamma;                                           // gamma ??4?ֽ?
    OS_S32 level;                                           // ?ڵ?ƽ ??4?ֽ?
    OS_S16 width;                                           // ͼ????
    OS_S16 height;                                          // ͼ????
    OS_U8 firmware_version[16];                             // ?̼??????汾
    OS_U8 sn_no[20];                                        // SN???? 1????Ҫͨ?????????????ã?2??????ǰ?????ܹ̻?flash???⣬?������?????Ӱ?졣
    OS_S32 day_night;                                       // ??????????
    OS_S32 td1;                                             // ץ????ʱ(??λus)
    OS_S32 td2;                                             // ?ع???ʱ(??λus)
    OS_S32 yavg;                                            // ƽ???��?
    OS_S32 st_usr_param;                                    // ?���???û?????(ץ??ʱ????????)
    OS_F32 fsaturationNo;                                   // ?????Ͷ?
    OS_S32 nNotifyACorDC;                                   // ??ǰ?õ? 0:ֱ?? 1:???? 2:?????쳣
    OS_U16 nArm_Hz;                                         // arm ??Ƶ??
    OS_U16 nDdr_Hz;                                         // DDR??Ƶ??
    OS_S32 nCam_Temperature;                                // ?????¶ȼĴ?????ֵ
    OS_U8 szColor[8];                                       // ??????ɫ
    OS_S32 nReliability;                                    // ???Ŷ?
    OS_S32 nSnapIndex;                                      // ץ??ͼƬ????
    OS_U8 szReserve[4];                                     // ?????¼???
    OS_S32 picflage;                                        // ͼƬ??־??????λΪ??1??ʾ?ñ?־??????Ч??0??ʾ??Ч
    // ????һλΪ1????SD???ϴ?ͼƬ??0??Ϊ??ʹץ???ϴ?ͼƬ??????λ??ʱδ?á?
    OS_S32 rev[18];                                         // ????????
    OS_U8 szSDKVer[16];                                     // SDK?汾

} EXT_JFIF2;                                                // JFIFͷ??��չE2?ṹ

#endif // __ITE_NITP_H__
