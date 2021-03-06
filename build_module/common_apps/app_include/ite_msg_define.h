/**
 * @file ite_msg_define.h
 * @brief  ????????ͷ?ļ?
 * @author  <itarge@itarge.com>
 * @version 1.0.0
 * @date 2017-05-16
 */

/* Copyright (C) 2009-, Itarge Inc.
 * All right reserved
 *
 */

#ifndef __ITE_MSG_DEFINE_H__
#define __ITE_MSG_DEFINE_H__

#ifdef __cplusplus
extern "C" {
#endif

enum
{
    VIDEO_ENCODER_CH1 = 0,
    VIDEO_ENCODER_CH2,
    VIDEO_ENCODER_CH3,
    VIDEO_ENCODER_MAX,
};

enum
{
    PICTURE_ENCODER_CH1 = 0,
    PICTURE_ENCODER_CH2,
    PICTURE_ENCODER_MAX,
};

/*alg, aval: nicp_daemon, search, stream*/
enum   // dsp debug 2 sys_adm
{
    MSG_DBG_VIDEO_FILE = 1,
    MSG_DBG_CMD,
    MSG_DBG_AVAL_QUIT,
    MSG_DBG_END
};

enum
{
    ENC_PROFILE_NULL  = 0,
    ENC_PROFILE_BASE,
    ENC_PROFILE_MAIN,
    ENC_PROFILE_HIGH,
    ENC_PROFILE_DEF,
};


/**
* @msg type: nicp to alg
* @
*/
typedef enum
{
    CMD_ISS_INIT = 1,

    CMD_ISS_SET_CAM1_PUBLIC_CONF,
    CMD_ISS_SET_CAM2_PUBLIC_CONF,
    CMD_ISS_SET_CAM3_PUBLIC_CONF,
    CMD_ISS_SET_CAM4_PUBLIC_CONF,

    CMD_ISS_SET_AE,
    CMD_ISS_GET_AE,
    CMD_ISS_SET_AWB,
    CMD_ISS_GET_AWB,
    CMD_ISS_SET_LIMIT_EXP,
    CMD_ISS_GET_LIMIT_EXP,
    CMD_ISS_SET_LIMIT_GAIN,
    CMD_ISS_GET_LIMIT_GAIN,
    CMD_ISS_SET_NIGHT_AWB,
    CMD_ISS_SET_EXP,
    CMD_ISS_GET_EXP,
    CMD_ISS_SET_GAIN,
    CMD_ISS_GET_GAIN,

    CMD_ISS_SET_NOR_BRIGHTNESS,
    CMD_ISS_GET_NOR_BRIGHTNESS,
    CMD_ISS_SET_DARK_BRIGHTNESS,
    CMD_ISS_GET_DARK_BRIGHTNESS,

    CMD_ISS_SET_NOR_CONTRAST,
    CMD_ISS_GET_NOR_CONTRAST,
    CMD_ISS_SET_DARK_CONTRAST,
    CMD_ISS_GET_DARK_CONTRAST,

    CMD_ISS_SET_NOR_SHARPNESS,
    CMD_ISS_GET_NOR_SHARPNESS,
    CMD_ISS_SET_DARK_SHARPNESS,
    CMD_ISS_GET_DARK_SHARPNESS,


    CMD_ISS_SET_NOR_SATURATION,
    CMD_ISS_GET_NOR_SATURATION,
    CMD_ISS_SET_DARK_SATURATION,
    CMD_ISS_GET_DARK_SATURATION,

    CMD_ISS_SET_NOR_NOISEFILTER,
    CMD_ISS_GET_NOR_NOISEFILTER,
    CMD_ISS_SET_DARK_NOISEFILTER,
    CMD_ISS_GET_DARK_NOISEFILTER,

    CMD_ISS_SET_NOR_BLC,
    CMD_ISS_GET_NOR_BLC,
    CMD_ISS_SET_DARK_BLC,
    CMD_ISS_GET_DARK_BLC,

    CMD_ISS_SET_NOR_GAMMA,
    CMD_ISS_GET_NOR_GAMMA,
    CMD_ISS_SET_DARK_GAMMA,
    CMD_ISS_GET_DARK_GAMMA,

    CMD_ISS_SET_NOR_COMPENSATION,
    CMD_ISS_GET_NOR_COMPENSATION,

    CMD_ISS_SET_COLORMATRIX,//??????ɫ????

    CMD_ISS_GET_DAY_OR_NIGHT,
    CMD_ISS_GET_MOD,


    CMD_ISS_SET_ZONE_EXP,
    CMD_ISS_GET_ZONE_EXP,
    CMD_ISS_END
} ITG_AEWB_CMD;





enum   //sys_adm 2 aval
{
    CMD_AVAL_START = 0,
    CMD_AVAL_SET_FOCUS_MODE,
    CMD_AVAL_GET_FOCUS_MODE,
    CMD_AVAL_SET_STM_HEARTBEAT,
    CMD_AVAL_SET_MULTI_FOCUS_MODE,
    CMD_AVAL_SET_MULTI_STM_ADDR,
    CMD_AVAL_SET_MULTI_STM_HEARTBEAT,
    CMD_AVAL_SET_IMG_ROTATION,
    CMD_AVAL_SET_MJPG_QUALITY,
    CMD_AVAL_SET_PLD_ALG_INFO,
    CMD_AVAL_SET_SNAP_QUALITY,
    CMD_AVAL_SET_STREAM_QUALITY,
    CMD_AVAL_SET_H264_PARAM_INFO,


    CMD_AVAL_QUIT,
    CMD_AVAL_GET_IP,
    CMD_AVAL_END
};


enum   //sys_adm 2 alg
{
    MSG_ALGCOMM_START = 0,
    MSG_ALGPARAM_RELOAD,
    MSG_ALGCOMM_END
};



enum   //sys_adm 2 stream
{
    MSG_STREAM_START = 0,
    MSG_STREAM_TRAFFIC_START,
    MSG_STREAM_TRAFFIC_STOP,
    MSG_STREAM_HEART_BEAT,
    MSG_STREAM_END
};




/*****************************************************************/
enum   //search 2 sys_adm
{
    MSG_SEARCH_START  = 0,
    MSG_SEARCH_SAVE_IPADDR,
    MSG_SEARCH_SAVE_IPADDR_NORESP,
    MSG_SEARCH_GET_IP,
    MSG_SEARCH_END
};




enum   //nicp 2 sys_adm
{
    MSG_NICP_START  = 0,
    MSG_NICP_GET_RESTART_COUNT,
    MSG_NICP_SAVE_CONFIG,
    MSG_NICP_RESET_RESTART_COUNT,
    MSG_NICP_TRIGGER_SINGLE_SNAP,
    MSG_NICP_TRIGGER_MULTI_SNAP,
    MSG_NICP_SET_FOCUS_MODE,
    MSG_NICP_GET_FOCUS_MODE,
    MSG_NICP_SET_AE_STATUS,
    MSG_NICP_GET_AE_STATUS,
    MSG_NICP_SET_AWB_STATUS,
    MSG_NICP_GET_AWB_STATUS,
    MSG_NICP_SET_ETH_INFO,
    MSG_NICP_GET_ETH_INFO,
    MSG_NICP_GET_ETH_INFO_DEF,
    MSG_NICP_SET_FTP_INFO,
    MSG_NICP_GET_FTP_INFO,
    MSG_NICP_GET_FTP_INFO_DEF,

    MSG_NICP_SET_AE_INFO,
    MSG_NICP_GET_AE_INFO,
    MSG_NICP_SET_DEVICE_TIME,
    MSG_NICP_GET_DEVICE_TIME,
    MSG_NICP_SET_STM_ADDR,
    MSG_NICP_SET_JPG_ADDR,
    MSG_NICP_SET_IMAGE_ROTATION,
    MSG_NICP_GET_IMAGE_ROTATION,
    MSG_NICP_GET_IMAGE_ROTATION_DEF,
    MSG_NICP_GET_CAMERA_HEARTBEAT,
    MSG_NICP_GET_DAY_OR_NIGHT,
    MSG_NICP_GET_CUR_CAMERA_MODE,
    MSG_NICP_SET_MJPG_QUALITY,
    MSG_NICP_GET_MJPG_QUALITY,

    MSG_NICP_SET_FRM_INTERVAL,
    MSG_NICP_GET_FRM_INTERVAL,
    MSG_NICP_SET_STM_HEARTBEAT,
    MSG_NICP_SET_H264_INFO,
    MSG_NICP_GET_H264_INFO,
    MSG_NICP_GET_H264_INFO_DEF,
    MSG_NICP_SET_DAY_LIGHT_INFO,
    MSG_NICP_GET_DAY_LIGHT_INFO,
    MSG_NICP_SET_FTPDIR_INFO,
    MSG_NICP_GET_FTPDIR_INFO,
    MSG_NICP_GET_CJQ_INFO,
    MSG_NICP_SET_SECFRM_STATUS,
    MSG_NICP_GET_SECFRM_STATUS,

    MSG_NICP_SET_MULTI_STM_ADDR,
    MSG_NICP_SET_MULTI_JPG_ADDR,
    MSG_NICP_SET_MULTI_STM_HEARTBEAT,
    MSG_NICP_SET_MULTI_FOCUS_MODE,
    MSG_NICP_GET_MULTI_FOCUS_MODE,

    MSG_NICP_SET_NTP_CHK_INFO,
    MSG_NICP_GET_NTP_CHK_INFO,
    MSG_NICP_GET_NTP_CHK_INFO_DEF,
    MSG_NICP_SET_LOG_CONFIG,
    MSG_NICP_GET_LOG_CONFIG,
    MSG_NICP_GET_LOG_CONFIG_DEF,

    MSG_NICP_GET_MULTI_MAX_FRM_RATE,

    MSG_NICP_SET_ACSYNC_INFO,
    MSG_NICP_GET_ACSYNC_INFO,
    MSG_NICP_GET_ACSYNC_INFO_DEF,
    MSG_NICP_SET_RS232_INFO,
    MSG_NICP_GET_RS232_INFO,
    MSG_NICP_GET_RS232_INFO_DEF,
    MSG_NICP_SET_RS485_INFO,
    MSG_NICP_GET_RS485_INFO,
    MSG_NICP_GET_RS485_INFO_DEF,
    MSG_NICP_SET_SPEED_INFO,
    MSG_NICP_GET_SPEED_INFO,
    MSG_NICP_GET_SPEED_INFO_DEF,

    MSG_NICP_SET_PLD_ALG_INFO,
    MSG_NICP_GET_PLD_ALG_INFO,
    MSG_NICP_GET_PLD_ALG_INFO_DEF,

    MSG_NICP_SET_FRAME_INTERVEL_INFO,
    MSG_NICP_GET_FRAME_INTERVEL_INFO,
    MSG_NICP_GET_FRAME_INTERVEL_INFO_DEF,

    MSG_NICP_SET_STREAM_TRANS_MODE,
    MSG_NICP_GET_STREAM_TRANS_MODE,
    MSG_NICP_GET_STREAM_TRANS_MODE_DEF,
    MSG_NICP_SET_STREAM_TRANS_PROTOCOL,
    MSG_NICP_GET_STREAM_TRANS_PROTOCOL,
    MSG_NICP_GET_STREAM_TRANS_PROTOCOL_DEF,

#if 0
    MSG_NICP_SET_RED_AREA_INFO,
    MSG_NICP_GET_RED_AREA_INFO,
    MSG_NICP_GET_RED_AREA_INFO_DEF,
    MSG_NICP_SET_TRANSPARENT_SERIAL,
    MSG_NICP_GET_TRANSPARENT_SERIAL,
    MSG_NICP_GET_TRANSPARENT_SERIAL_DEF,
    MSG_NICP_SET_ALG_RED_LIGHT,
    MSG_NICP_GET_ALG_RED_LIGHT,
    MSG_NICP_GET_ALG_RED_LIGHT_DEF,
#endif
    MSG_NICP_SET_UPLOAD_USER_INFO,
    MSG_NICP_GET_UPLOAD_USER_INFO,
    MSG_NICP_GET_UPLOAD_USER_INFO_DEF,
    MSG_NICP_SET_FORMAT_MARK,
    MSG_NICP_RESET_CAMERA_DEFAULT_PARAM,
    MSG_NICP_SET_SECRET_ID_VALID,
    MSG_NICP_GET_KAL_STATUS_INFO,
    MSG_NICP_SET_MEDIA_FILENAME_INFO,
    MSG_NICP_GET_MEDIA_FILENAME_INFO,
    MSG_NICP_GET_MEDIA_FILENAME_INFO_DEF,

#if 0

    //!Virtual dir abtainess
    MSG_NICP_GET_VIRTUALDIR_RECORD,
    MSG_NICP_GET_VIRTUALFILE_DOWN,
    MSG_NICP_SET_ALG_CHANGE_MODE,
    MSG_NICP_SET_CHECKPOINT_LAMP,
    MSG_NICP_GET_CHECKPOINT_LAMP,
    MSG_NICP_GET_CHECKPOINT_LAMP_DEF,
#endif
    MSG_NICP_SET_SIPIPC_CONFIG,
    MSG_NICP_GET_SIPIPC_CONFIG,
    MSG_NICP_GET_SIPIPC_CONFIG_DEF,
    MSG_NICP_GET_SIPIPC_DYNAMIC_STATUS,
#if 0
    MSG_NICP_SET_CUT_LINE_INFO,
    MSG_NICP_GET_CUT_LINE_INFO,
    MSG_NICP_GET_CUT_LINE_INFO_DEF,
#endif
    MSG_NICP_GET_CODEC_ATTRI_INFO,
    MSG_NICP_SET_EEPROM_CONFIG_INFO,
    MSG_NICP_GET_EEPROM_CONFIG_INFO,
    MSG_NICP_SET_REBOOT_TIME_INFO,
    MSG_NICP_GET_REBOOT_TIME_INFO,

    MSG_NICP_SET_DEV_INFO_EXT,
    MSG_NICP_GET_DEV_INFO_EXT,
    MSG_NICP_GET_DEV_INFO_EXT_DEF,
#if 0
    MSG_NICP_SET_IMAGEMERGE_TYPE,
    MSG_NICP_GET_IMAGEMERGE_TYPE,
#endif
    MSG_NICP_SET_IMG_OSD_INFO_EXT,
    MSG_NICP_GET_IMG_OSD_INFO_EXT,
    MSG_NICP_GET_IMG_OSD_INFO_EXT_DEF,
    MSG_NICP_SET_NETLOG_INFO,
    MSG_NICP_GET_NETLOG_INFO,
    MSG_NICP_SET_NETWORK_LINK_SPEED,
    MSG_NICP_GET_NETWORK_LINK_SPEED,
    MSG_NICP_GET_NETWORK_LINK_SPEED_DEF,
    MSG_NICP_GET_NETWORK_LINK_SPEED_STATUS,
#if 0
    MSG_NICP_SET_FLASH_LAMP_INFO_EX,
    MSG_NICP_GET_FLASH_LAMP_INFO_EX,
    MSG_NICP_GET_FLASH_LAMP_INFO_DEF_EX,
#endif
    MSG_NICP_SET_SERIAL_PORT_SHOW_INFO,
    MSG_NICP_GET_SERIAL_PORT_SHOW_INFO,
#if 0
    MSG_NICP_SET_APPINFO_P2,
    MSG_NICP_GET_APPINFO_P2,
    MSG_NICP_GET_APPINFO_P2_DEF,
    MSG_NICP_SET_PRERECORD_PARAMS,
    MSG_NICP_GET_PRERECORD_PARAMS,
    MSG_NICP_GET_PRERECORD_PARAMS_DEF,
    MSG_NICP_SET_CUSTOM_SERVER_PARAMS,
    MSG_NICP_GET_CUSTOM_SERVER_PARAMS,
#endif

    MSG_NICP_SET_TCAM_ISP_CONF,
    MSG_NICP_GET_TCAM_ISP_CONF,
    MSG_NICP_GET_TCAM_ISP_CONF_DEF,


    MSG_NICP_END,
};


enum   //web 2 sys_adm
{
    MSG_WEB_START  = 0,

    MSG_WEB_GET_RESTART_COUNT,
    MSG_WEB_SAVE_CONFIG,
    MSG_WEB_RESET_RESTART_COUNT,

    MSG_WEB_SET_CAMERA_CONF,
    MSG_WEB_SET_VIDEO_CONF,

    MSG_WEB_SET_DEVINFO_CONF,
    MSG_WEB_SET_LAN_CONF,
    MSG_WEB_SET_WLAN_CONF,
    MSG_WEB_SET_SIPAPP_CONF,//GB28181  or VISS
    MSG_WEB_SET_FTP_CONF,//GB28181  or VISS

    MSG_WEB_SET_NTP_CONF,

    MSG_WEB_END,
};


///////////////////////////for struct storage//////////////////////////////////////

#ifdef __cplusplus
}
#endif

#endif //__ITE_MSG_DEFINE_H__