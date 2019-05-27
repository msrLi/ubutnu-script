#ifndef _ITE_AEWB_H_
#define _ITE_AEWB_H_

/* ======================================= for aewb ====================================*/
//相机架设朝向说明
typedef enum alg_direction {
    algNone         = 0,//不设置
    algEastToWest   = 1,//相机由东向西
    algWestToEast   = 2,//相机由西向东
    algNorthToSouth = 3,//相机由北向南
    algSouthToNorth = 4,//相机由南向北
} ALG_DIRECTION;


//相机模式
typedef enum alg_status_mod {
    algNormalMod = 0,           // 正常模式
    algMiddayMod = 1,           // 正午模式
    algDuskMod = 2,             // 黄昏模式
    algMorningMod = 3,          // 清晨模式
    algDarkMod = 4,             // 黑暗模式
    algCurrMod = 5              // 当前模式
} EStatusMod;

//color correction matrix 颜色校正矩阵-OK
typedef struct {
    OS_S32 rgb_mul_rr;             // 取值范围为-128~128
    OS_S32 rgb_mul_gr;             // 取值范围为-128~128
    OS_S32 rgb_mul_br;             // 取值范围为-128~128
    OS_S32 rgb_mul_rg;             // 取值范围为-128~128
    OS_S32 rgb_mul_gg;             // 取值范围为-128~128
    OS_S32 rgb_mul_bg;             // 取值范围为-128~128
    OS_S32 rgb_mul_rb;             // 取值范围为-128~128
    OS_S32 rgb_mul_gb;             // 取值范围为-128~128
    OS_S32 rgb_mul_bb;             // 取值范围为-128~128
    OS_S32 rgb_oft_or;             // 取值范围为-128~128
    OS_S32 rgb_oft_og;             // 取值范围为-128~128
    OS_S32 rgb_oft_ob;             // 取值范围为-128~128
} ALG_COLOR_MATRIX;

typedef struct {
    OS_U32 awb_red;       // 取值范围为0~255
    OS_U32 awb_green;     // 取值范围为0~255
    OS_U32 awb_blue;      // 取值范围为0~255
} AWB_RESULT_VALUE;

typedef struct {
    OS_U32 uDayOrNight;   // 0表示白天; 1表示夜间
    OS_U32 uFlashLightStatus; // 0表示闪光灯关闭;1表示闪光灯开启
    OS_U32 uZoneExp;      // 分区曝光，低位9bit有效，每个bit代表一个方块

    // OS_U32 ALG_uBypassSwitch;
    // //旁路控制开关，若打开，则使用芯片本身的awb/ae
    // OS_U32 ALG_uStatusMod; //当前的模式
    OS_U32 AE_uSwitch;    // 自动曝光算法开关,取值为ALG_OFF
    // ,ALG_ON,ALG_BYPASS
    OS_U32 AWB_uSwitch;   // 自动白平衡算法开关,取值为ALG_OFF
    // ,ALG_ON,ALG_BYPASS
    OS_U32 max_uExposure; // 快门上限,1~7000*10 us
    OS_U32 min_uExposure; // 快门下限,1~7000*10 us
    OS_U32 max_uGain;     // 增益上限,1~100
    OS_U32 min_uGain;     // 增益下限,不设置

    // 当前参数，保存的是当前模式的参数
    OS_U32 uCurBrightness;    // 当前亮度,取值范围为0~255
    OS_U32 uCurContrast;      // 当前对比度,取值范围为0~255
    OS_U32 uCurSaturation;    // 当前饱和度,取值范围为0~255
    OS_U32 uCurNoiseFilter; // 当前边缘增强敏感度,取值范围为0~255
    OS_U32 uCurEEStrength;    // 当前边缘增强强度,取值范围为0~255
    OS_U32 uCurBLC;           // 当前黑电平
    OS_U32 uCurGamma;         // 当前gamma值

    OS_U32 uNorBrightness;    // 正常模式亮度,取值范围为0~255
    OS_U32 uNorContrast;      // 正常模式对比度,取值范围为0~255
    OS_U32 uNorSaturation;    // 正常模式饱和度,取值范围为0~255
    OS_U32 uNorNoiseFilter; // 正常模式边缘增强敏感度,取值范围为0~255
    OS_U32 uNorEEStrength;    // 正常模式边缘增强强度,取值范围为0~255
    OS_S32 uNorBLC;                    // 正常模式黑电平
    OS_U32 uNorGamma;         // 正常模式gamma值

    OS_U32 uMidBrightness;    // 正午模式亮度,取值范围为0~255
    OS_U32 uMidContrast;      // 正午模式对比度,取值范围为0~255
    OS_U32 uMidSaturation;    // 正午模式饱和度,取值范围为0~255
    OS_U32 uMidNoiseFilter; // 正午模式边缘增强敏感度,取值范围为0~255
    OS_U32 uMidEEStrength;    // 正午模式边缘增强强度,取值范围为0~255
    OS_U32 uMidBLC;           // 正午模式黑电平
    OS_U32 uMidGamma;         // 正午模式gamma值

    OS_U32 uDuskBrightness;   // 黄昏模式亮度,取值范围为0~255
    OS_U32 uDuskContrast;     // 黄昏模式对比度,取值范围为0~255
    OS_U32 uDuskSaturation;   // 黄昏模式饱和度,取值范围为0~255
    OS_U32 uDuskNoiseFilter;// 黄昏模式边缘增强敏感度,取值范围为0~255
    OS_U32 uDuskEEStrength;   // 黄昏模式边缘增强强度,取值范围为0~255
    OS_U32 uDuskBLC;          // 黄昏模式黑电平
    OS_U32 uDuskGamma;        // 黄昏模式gamma值

    OS_U32 uMorBrightness;    // 清晨模式亮度,取值范围为0~255
    OS_U32 uMorContrast;      // 清晨模式对比度,取值范围为0~255
    OS_U32 uMorSaturation;    // 清晨模式饱和度,取值范围为0~255
    OS_U32 uMorNoiseFilter; // 清晨模式边缘增强敏感度,取值范围为0~255
    OS_U32 uMorEEStrength;    // 清晨模式边缘增强强度,取值范围为0~255
    OS_U32 uMorBLC;           // 清晨模式黑电平
    OS_U32 uMorGamma;         // 清晨模式gamma值

    OS_U32 uDarkBrightness;   // 黑暗模式亮度,取值范围为0~255
    OS_U32 uDarkContrast;     // 黑暗模式对比度,取值范围为0~255
    OS_U32 uDarkSaturation;   // 黑暗模式饱和度,取值范围为0~255
    OS_U32 uDarkNoiseFilter;// 黑暗模式边缘增强敏感度,取值范围为0~255
    OS_U32 uDarkEEStrength;   // 黑暗模式边缘增强强度,取值范围为0~255
    OS_S32 uDarkBLC;                   // 黑暗模式黑电平
    OS_U32 uDarkGamma;        // 黑暗模式gamma值

    // OS_U32 day_uBrightness; //白天亮度
    // OS_U32 night_uBrightness; //夜间亮度
    // OS_U32 day_uContrast; //白天对比度
    // OS_U32 night_uContrast; //夜间对比度
    // OS_U32 day_uSaturation; //白天饱和度
    // OS_U32 night_uSaturation; //夜间饱和度
    // OS_U32 day_uNoiseFilter; //白天边缘增强敏感度
    // OS_U32 night_uNoiseFilter; //夜间边缘增强敏感度
    // OS_U32 day_uEEStrength; //白天边缘增强强度
    // OS_U32 night_uEEStrength; //夜间边缘增强强度
    // OS_U32 day_uBLC; //白天黑电平
    // OS_U32 night_uBLC; //夜间黑电平
    // OS_U32 day_uGamma; //白天gamma值
    // OS_U32 night_uGamma; //夜间gamma值

    OS_S32 nCurCompensation;       // 当前曝光补偿;夜间无曝光补偿
    OS_S32 nMidCompensation;       // 正午曝光补偿;夜间无曝光补偿
    OS_S32 nNorCompensation;       // 正常曝光补偿;夜间无曝光补偿
    OS_S32 nDuskCompensation;      // 黄昏曝光补偿;夜间无曝光补偿
    OS_S32 nMorCompensation;       // 早晨曝光补偿;夜间无曝光补偿
    OS_S32 nDarkCompensation;      // 黑暗曝光补偿;夜间无曝光补偿

    OS_U32 night_uRedGain;    // OK, 夜间固定红色增益,取值范围为0~255
    OS_U32 night_uGreenGain;  // OK, 夜间固定绿色增益,取值范围为0~255
    OS_U32 night_uBlueGain;   // OK, 夜间固定蓝色增益,取值范围为0~255

    OS_U32 my_uExposure;  // 当前曝光时间,取值范围为1~70000
    OS_U32 my_uGain;      // 当前快门,取值范围为1~100
    OS_U32 uBeforeTowHourExp; // 两个小时之前的曝光
    OS_U32 uBeforeTowHourGain;    // 两个小时之前的增益
    AWB_RESULT_VALUE my_sAWB;   // 算法算出的白平衡系数

    OS_S32 FlashLightLum;          // 闪光灯亮度区间阈值，微调范围-10~10
    OS_S32 ManualAWBLun;           // 手动白平衡阈值，范围-10~10
    ALG_COLOR_MATRIX sColorMatrix;  // 颜色矩阵
    EStatusMod eStatus;         // 相机模式
    OS_U32 uCurLum;       //当前亮度值
    OS_U32 nNightAWBSwitch;//夜间白平衡开关 0关闭 1开启
    OS_U32 nNightAWBThershiold; //夜间白平衡阈值 0-255

    OS_U32 nARM_Ctrl_ISP_Switch;//客户在arm端控制isp的总接口;0客户不控制;1 客户使用自己ae;2 客户使用自己awb;3 客户使用自己的ae和awb

    OS_U32 uAE_Speed_threshold;//AE算法调节快慢的阈值

    OS_U32 uALG_Light_Switch;//算法闪光灯自动控制开关  0表示控制闪关灯 1表示不控制闪关灯

    OS_U32 uALG_Wide_dynamic_switch;//宽动态模式
    OS_U32 uALG_Wide_dynamic_level;//宽动态级别

    OS_U32 AE_uSwitch_II;    //二套参数自动曝光算法开关,取值为ALG_OFF
    OS_U32 my_uExposure_II;  // 二套参数当前曝光时间,取值范围为1~70000
    OS_U32 my_uGain_II;      // 二套参数当前快门,取值范围为1~100
    OS_U32 max_uExposure_II; // 二套参数快门上限,1~7000*10 us
    OS_U32 min_uExposure_II; //二套参数 快门下限,1~7000*10 us
    OS_U32 max_uGain_II;     // 二套参数增益上限,1~100
    OS_U32 min_uGain_II;     // 二套参数增益下限,不设置

    OS_U16 nRedSecond;					//红色增益(二套或第二个SENSOR)
    OS_U16 nGreenSecond;				//绿色增益(二套或第二个SENSOR)
    OS_U16 nBlueSecond;					//蓝色增益(二套或第二个SENSOR)
    
    OS_U16 nNightAWBSwitchSecond;		//夜间白平衡开关0关闭1开启(二套或第二个SENSOR)
    OS_U16 nNightAWBThershioldSecond;	//夜间白平衡阈值0-255 (二套或第二个SENSOR)
    OS_S16 flashwhite;							//闪光灯白平衡补偿
    OS_F32     TomapFreq;  //频率
    OS_F32     TomapLevel; //强度等级
    OS_F32     TomapDetail;//细节

    OS_S32 nReserve[12];           // 保留
} ALG_PARA_DATA;



typedef struct IPNC_CameraPosition {
    OS_S32 direct;//相机方向    =algNone的时候 不配置方向就不启用逆顺光算法处理
    OS_U8 skHour;//顺光起始小时
    OS_U8 skMinute;// 顺光起始分钟
    OS_U8 sjHour;//顺光结束小时
    OS_U8 sjMinute;// 顺光结束分钟
    OS_U8 nkHour;//逆光起始小时
    OS_U8 nkMinute;// 逆光起始分钟
    OS_U8 njHour;//逆光结束小时
    OS_U8 njMinute;// 逆光结束分钟
    OS_S16  nOpenAlg;//是否打开顺逆光自动算法开关 当相机方向=algNone的时候 无效
    OS_S16  sgCompensate;//顺光补偿
    OS_S16  ngCompensate;//顺光补偿
    OS_S16  nResv[13];//保留
} IPNC_CameraPosition, *PIPNC_CameraPosition;

#endif
