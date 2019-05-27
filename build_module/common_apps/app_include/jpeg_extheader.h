#ifndef _JPEG_EXT_HEADER_H_
#define _JPEG_EXT_HEADER_H_

#define MAC_LENGTH 6
#define VER_MAX_LENGTH 16

#define SECRET_DATA_SIZE 64

#define JPGOFFSET 21

#include "ITENITP.h"

typedef struct ITE_TIME_VAL_TAG
{
    OS_U32 tv_sec;
    OS_U32 tv_usec;
}ITE_TIME_VAL_T;

typedef struct PicResolution
{
    OS_S16 Width;    //分辨率宽度
    OS_S16 Height;   //分辨率高度
} PicResolution;
#if 0
typedef enum
{
    TrigPic = 0,    //触发抓拍图片
    FullStream = 1, //全分辨率流图片
    D1Stream = 2, //D1分辨率流图片
    SecondFrame //秒针图片
} PicType;


typedef enum
{
    RadarTrig = 1, //雷达触发
    CoilDetector,   //线圈车检器(RS232 RS 485)
    SoftTrig,         // 网络软触发
    TTLTrig,            //IO触发
    VideoDetecTrig  //视频检测触发
} TriType;

#endif

#define TriType ESnapType  //定义在ITENetSDK.h 中
#define PicType EStreamPicType  //定义在ITENetSDK.h 中

typedef struct FlashTrigInfo
{
    OS_U8   isFlashOn;      //未闪光值0 ，闪光对应闪灯序号bit[7:0]位置1
    OS_U8   FlashType;      //闪光灯类型 0 气体(爆闪)  1 LED
    OS_U8   isForceFlash;   //是否强制闪光 1 强制闪光
    OS_U8   isTurnFlash;    //是否启用轮闪
    OS_U8   Reserve[4];     //保留字段
} FlashTrigInfo;

typedef struct TrigInfo
{
    OS_S32                 RedLightAfterTime;      //红灯后时间单位毫秒
    ITE_TIME_VAL_T      RedLightOnTime;         //红灯点亮时刻年月日时分秒毫秒, struct timeval -> ITE_TIME_VAL_T
    OS_S16               SnapRoadNo;             //抓拍车道信息      //从1开始
    OS_S16               SnapPicNo;                  //抓拍序号      //从1开始
    OS_S16               ttlNO;                      //TTL触发条件索引仅TTL 触发有效
    OS_S16               nCountGroupSnap;          //每组抓拍张数
    OS_U8                ExtSuffix[4];               //文件名后缀
    ESnapType      TriSource;                  //触发源类型
    OS_U8               DevName[16];                //触发设备名称
    OS_S32                 illegalType;                //违法类型 bit[31:0]    all 0 未知user define   bit[10:0] 违法停车 违法直行 违法右转 违法左转 压线 占道 逆行 卡口 超速 闯红
    OS_S16              CarSpeedVal;                //当前车速单位0.1km/h
    OS_S16              RoadSpeedMax;               //车道限速  0.1km/h
    OS_S32                 RoadSpeedPer;               //车道限速百分比
    OS_S32                 CoilInterval_1;         //线圈时间差1   单位毫秒
    OS_S32                 CoilInterval_2;         //线圈时间差1   单位毫秒
    OS_S32                 CoilDistance_1;         //线圈距离1 单位0.1米
    OS_S32                 CoilDistance_2;         //线圈距离2 单位0.1米
    OS_U8               LicenseTag[16];         //牌照信息
    OS_U8               LicenseTagColor[8];            //车牌颜色
    OS_S32                 nReliability;               //可信度
    OS_S32                 pContext;                   //触发上下文
    FlashTrigInfo   nFlash;                     //闪光灯信息
    OS_S32                 SaveToStorage;              //是否存储至外设
    OS_S32                 nSpeedCorrect;  //速度矫正值
    OS_S16               SnapInterval;               //抓拍间隔
    OS_S16               Reserv;         // 字节对齐使用 暂未赋值
    OS_S32           eIll;                  //违法类型
    OS_S32           CarGroupNO;                //车辆统计(抓拍组数),从开始判断是否是一次抓拍出的图片
    OS_S32           Reserve;               //保留字段后期扩展使用

} TrigInfo;




typedef struct H264Prm
{
    OS_S16           FrameRate;          //帧率
    OS_S16           BitRate;            //码率 单位 Kbps
    OS_S16           idrFrameRate;       //I帧间隔
    OS_S16           BitContrl;          //码率控制方式
    OS_S32             Reserve[2];     //保留字段
} H264Prm;

typedef struct EditionInfo
{
    OS_U8            FirmWareVer[32];                //固件程序版本
    OS_U8            AlgVer[VER_MAX_LENGTH];         //固件程序版本
    OS_U8            DspAppsVer[VER_MAX_LENGTH]; //dsp应用程序版本
    OS_U8            SDKVer[32];                 //SDK版本 windows 填写
    OS_U32    FpgaVer;                        //FPGA版本
    OS_U32    UbootVer;                       //Uboot 版本
    OS_U32    KernlVer;                       //内核版本
    OS_U32    ArmAppsVer;                 //arm 应用程序版本
    OS_U32    HardwarePcbVer;             //硬件版本  byte[0]  主板号低8bit  byte[1]  主板号高8bit  byte[2]  sensor板低8bit  byte[3]  sensor板高8bit
    OS_U32    Reserve[2];                 //保留字段
} EditionInfo;

typedef struct DeviceInfo
{
    OS_U8            CamraType[16];          //相机型号
    OS_U8            SensorType[16];         //传感器型号
    struct in_addr  IpAddr_1;               //IP地址1
    struct in_addr  IpAddr_2;               //IP地址2
    OS_U8            MacAddr[MAC_LENGTH];    //设备MAC地址
    OS_U8            szRev[2];
    EditionInfo     VerInfo;                //相关版本信息
    OS_S16           PowerType;              //电源属性  0:DC   1: AC  2: 交流异常
    OS_S16           AcDelay;                //交流相位偏移
    OS_S16           armMHz;                 //arm 频率
    OS_S16           ddrMHz;                 //ddr 频率
    OS_S16           dspMHz;                 //dsp 频率
    OS_S16           Gyroscope;              //陀螺仪三轴传感器
    OS_S16           FocusDeepness;          //聚焦程度
    OS_S16           nRev;
    OS_U8            DeviceName[12];     //设备名称
    OS_U8            DeviceNO[12];           //设备编号
    OS_U8            DeviceDirection[16];    //设备方向
    OS_U8            DeviceAddr[64];     //设备位置
    OS_U8            SecretID[12];           //加密芯片ID
    OS_U8            SN[20];                 //设备序列号
    OS_S32             Reserve[4];         //保留字段

} DeviceInfo;

typedef struct cpConfigm
{
    OS_S16       nShutterMax;        //快门上限   单位 10us
    OS_S16       nShutterMin;        //快门下限  单位 10us
    OS_S16       nGainMax;           //增益上限
    OS_S16       nGainMin;           //增益下限
    OS_S16       nShutter;           //快门速度  单位 10us
    OS_S16       nGain;              //增益
    OS_S16       nSharpness;     //锐度
    OS_S16       nSharpReserve[2];   //锐度保留扩展
    OS_S16       nNoisefilter;       //降噪
    OS_S16       nFlashStatus;       //闪关灯状态 0表示关灯 1表示开灯
    OS_S16       nBrightness;        //亮度
    OS_S16       nContrast;          //对比度
    OS_S16       nSaturation;        //饱和度
    OS_S16       nRed;               //红色分量
    OS_S16       nGreen;             //绿色分量
    OS_S16       nBlue;              //蓝色分量
    OS_S16       nEV;                //曝光补偿
    OS_S16       nQuality;           //图片质量
    OS_S16       nGamma;             //gamma
    OS_S16       nBlacklevel;        //黑电平
    OS_S16       nSceneMode;     //场景模式 0:正常 1:正午 2:黄昏 3:清晨 4:黑暗 5:当前
    OS_S16       nRotationAngle; //旋转角度
    OS_S16       nMirror;            //镜像方式
    OS_S16       nThreLightOn;       //开灯阈值
    OS_S16       nThreManualWB;      //手动白平衡阈值
    OS_S16       enRealTimeSnap; //是否启用即时抓拍
    OS_S16       nFrameInterval; //帧间隔
    OS_S16       nAverageLuma;       //平均亮度
    OS_S16       nTemperature;       //温度
    OS_S16       nSubarea;           //分区统计
    OS_S16       nAutoExpo;     //保留字段,      0,手动曝光    1:自动曝光
    OS_S16       nAutoWB;        //自动白平衡，  0,手动白平衡; 1:自动白平衡
    OS_S16        nDenoise;          //是否启用降噪 0:不启用 1：启用
    OS_S16        nFrameTimeInterval;        //曝光帧间隔时间
    //第二套参数
    OS_S16       npExp;          //百分比1--300
    OS_S16       npGain;         //百分比1--300
    OS_S16       nUseSecondParm; //是否启用第二套参数 0:不启用 1:启用
} cpConfigm;

typedef struct IpncSecretData
{
    OS_S32 nCommand; //0:读取加密ID  1:写入加密ID
    OS_S32 nLen; //数据长度
    OS_U8 szData[SECRET_DATA_SIZE]; //加密数据BUF,有效字节长度 nLen
} IpncSecretData;

typedef struct red_streng
{
    OS_S16 nLeft;          //左上角X
    OS_S16 nTop;       // 左上角Y
    OS_S16 nRight;     //右下角X
    OS_S16 nBottom;    // 右下角Y

} red_streng;

typedef struct ExtJfif
{
    OS_U8                Header[4];              //扩展头部标志和长度  header[0:1] 0xFF 0xE1    header[2:3]=OS_U16 (sizeof(EXT_JFIF)-2)
    OS_U8                Company[8];         //公司名称及产品系列  itarge   itargeb  itargek ......
    ENITPVer            NicpVer;                //NITP协议版本
    OS_S32                 PicLen;                 //图片总长度
    PicResolution       ImageRes;               //图像分辨率
    PicType             ImageType;              //图像类型
    OS_U32       PicFrameCnt;            //相机递增的帧序号
    OS_U32      ExpoTimeUtcS; //struct timeval      ExpoTime;               //曝光时刻linux timeval
    OS_U32      ExpoTimeUtcUS;
    TrigInfo            PicTrig;                //触发相关信息
    H264Prm             H264Info;               //h264参数信息
    DeviceInfo          DevInfo;                //设备信息
    cpConfigm            sysInfo;                //系统参数信息
    OS_U8      xzxSecretID[4];
    OS_U16 PWtype; //频闪灯下输出脉宽的方式 1:根据快门大小 2:用户设置
    OS_U16 PWvalue; //频闪灯下输出脉宽的数值,范围1--5ms,只在用户设置时生效
    OS_U16 nUseFlashLamp; //获取有无闪光灯开关 1:无 2:有
    OS_U16 RadarSpeed; //流模式 雷达速度
    OS_S32 red_strengn_Num; //红灯区域个数
    red_streng  red_streng_area[4];
    OS_U32 a_unSlAtt[4];
    OS_U8 LightInfo[12];
    OS_S32 Reserve[45];
    //OS_S32 Reserve[48];
    //OS_S32 Reserve[61]; //保留字段后期扩展使用
} ExtJfif;  //抓拍图片头部扩展结构

typedef struct _RelatedFileName
{
    OS_S32  nNum;
    OS_U8 szFileName[4][512];
} RelatedFileName;


typedef struct  ITE_Alg_FileInfo
{
    int       dirNum;                 //目录分层数量
    char     pfilename[512];         //生成的文件名信息
    char     dirpath[8][128];        //最多8级子目录图片存储路径
    int       resv[16];               //保留字段
} ITE_Alg_FileInfo;

    typedef struct  ITE_PublicPara
    {
        char        header[4];                //扩展头部标志和长度  header[0:1] 0xFF 0xE2    header[2:3]=unsigned short
        int         frameId;                   //当前帧号
        ITE_Alg_FileInfo file;                       //上传文件目录及文件名
        int existOSD;                         //是否存在OSD  处理
        int Osdbufoffset;                   // OSD 处理信息的地址
        int Osdbuflen;                       // OSD 处理信息长度
        int existMergeJPG;                // 是否需要图片合成
        int MergeArgOffset;              // 图片合成参数的地址
        int MergeArglen;                  // 图片合成参数的长度
        int Company;                        //公司标志
        int sec;                                // 叠加的OSD 秒
        int msec;                             // 叠加的OSD 秒
        int capType;                        // 抓拍结果类型 1  通行 2为违章
        int frame_s[3];
	int frame_ms[3];
	char lightSta[4];
	int FlowType;                        // 是否有车标志
	char armplString[16];
	char dspplString[16];
	int resv[12];
		
    } ITE_PublicPara;

/*
************************************************************************
*   ITG_PlSys_Para_Out_Tag   max size 20K
*   resv buff max size 18K
*   PublicInfo.header[2] = (sizeof(ITG_PlSys_Para_Out_Tag) - 2)>>8;
*   PublicInfo.header[3] = (sizeof(ITG_PlSys_Para_Out_Tag) - 2)&0xff;
*   write   ITG_PlSys_Para_Out_Tag     to jpg file header ext2
************************************************************************
*/
#define EXT_BUF_MAX_LEN (0xA000 - 2048)
typedef struct  ITE_ALG_ExtJfif {
    ITE_PublicPara PublicInfo;
    unsigned char resv[EXT_BUF_MAX_LEN]; //sizeof (resv) << 38K  limit
} ITE_ALG_ExtJfif;



#endif
