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
    OS_S16 Width;    //�ֱ��ʿ��
    OS_S16 Height;   //�ֱ��ʸ߶�
} PicResolution;
#if 0
typedef enum
{
    TrigPic = 0,    //����ץ��ͼƬ
    FullStream = 1, //ȫ�ֱ�����ͼƬ
    D1Stream = 2, //D1�ֱ�����ͼƬ
    SecondFrame //����ͼƬ
} PicType;


typedef enum
{
    RadarTrig = 1, //�״ﴥ��
    CoilDetector,   //��Ȧ������(RS232 RS 485)
    SoftTrig,         // ��������
    TTLTrig,            //IO����
    VideoDetecTrig  //��Ƶ��ⴥ��
} TriType;

#endif

#define TriType ESnapType  //������ITENetSDK.h ��
#define PicType EStreamPicType  //������ITENetSDK.h ��

typedef struct FlashTrigInfo
{
    OS_U8   isFlashOn;      //δ����ֵ0 �������Ӧ�������bit[7:0]λ��1
    OS_U8   FlashType;      //��������� 0 ����(����)  1 LED
    OS_U8   isForceFlash;   //�Ƿ�ǿ������ 1 ǿ������
    OS_U8   isTurnFlash;    //�Ƿ���������
    OS_U8   Reserve[4];     //�����ֶ�
} FlashTrigInfo;

typedef struct TrigInfo
{
    OS_S32                 RedLightAfterTime;      //��ƺ�ʱ�䵥λ����
    ITE_TIME_VAL_T      RedLightOnTime;         //��Ƶ���ʱ��������ʱ�������, struct timeval -> ITE_TIME_VAL_T
    OS_S16               SnapRoadNo;             //ץ�ĳ�����Ϣ      //��1��ʼ
    OS_S16               SnapPicNo;                  //ץ�����      //��1��ʼ
    OS_S16               ttlNO;                      //TTL��������������TTL ������Ч
    OS_S16               nCountGroupSnap;          //ÿ��ץ������
    OS_U8                ExtSuffix[4];               //�ļ�����׺
    ESnapType      TriSource;                  //����Դ����
    OS_U8               DevName[16];                //�����豸����
    OS_S32                 illegalType;                //Υ������ bit[31:0]    all 0 δ֪user define   bit[10:0] Υ��ͣ�� Υ��ֱ�� Υ����ת Υ����ת ѹ�� ռ�� ���� ���� ���� ����
    OS_S16              CarSpeedVal;                //��ǰ���ٵ�λ0.1km/h
    OS_S16              RoadSpeedMax;               //��������  0.1km/h
    OS_S32                 RoadSpeedPer;               //�������ٰٷֱ�
    OS_S32                 CoilInterval_1;         //��Ȧʱ���1   ��λ����
    OS_S32                 CoilInterval_2;         //��Ȧʱ���1   ��λ����
    OS_S32                 CoilDistance_1;         //��Ȧ����1 ��λ0.1��
    OS_S32                 CoilDistance_2;         //��Ȧ����2 ��λ0.1��
    OS_U8               LicenseTag[16];         //������Ϣ
    OS_U8               LicenseTagColor[8];            //������ɫ
    OS_S32                 nReliability;               //���Ŷ�
    OS_S32                 pContext;                   //����������
    FlashTrigInfo   nFlash;                     //�������Ϣ
    OS_S32                 SaveToStorage;              //�Ƿ�洢������
    OS_S32                 nSpeedCorrect;  //�ٶȽ���ֵ
    OS_S16               SnapInterval;               //ץ�ļ��
    OS_S16               Reserv;         // �ֽڶ���ʹ�� ��δ��ֵ
    OS_S32           eIll;                  //Υ������
    OS_S32           CarGroupNO;                //����ͳ��(ץ������),�ӿ�ʼ�ж��Ƿ���һ��ץ�ĳ���ͼƬ
    OS_S32           Reserve;               //�����ֶκ�����չʹ��

} TrigInfo;




typedef struct H264Prm
{
    OS_S16           FrameRate;          //֡��
    OS_S16           BitRate;            //���� ��λ Kbps
    OS_S16           idrFrameRate;       //I֡���
    OS_S16           BitContrl;          //���ʿ��Ʒ�ʽ
    OS_S32             Reserve[2];     //�����ֶ�
} H264Prm;

typedef struct EditionInfo
{
    OS_U8            FirmWareVer[32];                //�̼�����汾
    OS_U8            AlgVer[VER_MAX_LENGTH];         //�̼�����汾
    OS_U8            DspAppsVer[VER_MAX_LENGTH]; //dspӦ�ó���汾
    OS_U8            SDKVer[32];                 //SDK�汾 windows ��д
    OS_U32    FpgaVer;                        //FPGA�汾
    OS_U32    UbootVer;                       //Uboot �汾
    OS_U32    KernlVer;                       //�ں˰汾
    OS_U32    ArmAppsVer;                 //arm Ӧ�ó���汾
    OS_U32    HardwarePcbVer;             //Ӳ���汾  byte[0]  ����ŵ�8bit  byte[1]  ����Ÿ�8bit  byte[2]  sensor���8bit  byte[3]  sensor���8bit
    OS_U32    Reserve[2];                 //�����ֶ�
} EditionInfo;

typedef struct DeviceInfo
{
    OS_U8            CamraType[16];          //����ͺ�
    OS_U8            SensorType[16];         //�������ͺ�
    struct in_addr  IpAddr_1;               //IP��ַ1
    struct in_addr  IpAddr_2;               //IP��ַ2
    OS_U8            MacAddr[MAC_LENGTH];    //�豸MAC��ַ
    OS_U8            szRev[2];
    EditionInfo     VerInfo;                //��ذ汾��Ϣ
    OS_S16           PowerType;              //��Դ����  0:DC   1: AC  2: �����쳣
    OS_S16           AcDelay;                //������λƫ��
    OS_S16           armMHz;                 //arm Ƶ��
    OS_S16           ddrMHz;                 //ddr Ƶ��
    OS_S16           dspMHz;                 //dsp Ƶ��
    OS_S16           Gyroscope;              //���������ᴫ����
    OS_S16           FocusDeepness;          //�۽��̶�
    OS_S16           nRev;
    OS_U8            DeviceName[12];     //�豸����
    OS_U8            DeviceNO[12];           //�豸���
    OS_U8            DeviceDirection[16];    //�豸����
    OS_U8            DeviceAddr[64];     //�豸λ��
    OS_U8            SecretID[12];           //����оƬID
    OS_U8            SN[20];                 //�豸���к�
    OS_S32             Reserve[4];         //�����ֶ�

} DeviceInfo;

typedef struct cpConfigm
{
    OS_S16       nShutterMax;        //��������   ��λ 10us
    OS_S16       nShutterMin;        //��������  ��λ 10us
    OS_S16       nGainMax;           //��������
    OS_S16       nGainMin;           //��������
    OS_S16       nShutter;           //�����ٶ�  ��λ 10us
    OS_S16       nGain;              //����
    OS_S16       nSharpness;     //���
    OS_S16       nSharpReserve[2];   //��ȱ�����չ
    OS_S16       nNoisefilter;       //����
    OS_S16       nFlashStatus;       //���ص�״̬ 0��ʾ�ص� 1��ʾ����
    OS_S16       nBrightness;        //����
    OS_S16       nContrast;          //�Աȶ�
    OS_S16       nSaturation;        //���Ͷ�
    OS_S16       nRed;               //��ɫ����
    OS_S16       nGreen;             //��ɫ����
    OS_S16       nBlue;              //��ɫ����
    OS_S16       nEV;                //�عⲹ��
    OS_S16       nQuality;           //ͼƬ����
    OS_S16       nGamma;             //gamma
    OS_S16       nBlacklevel;        //�ڵ�ƽ
    OS_S16       nSceneMode;     //����ģʽ 0:���� 1:���� 2:�ƻ� 3:�峿 4:�ڰ� 5:��ǰ
    OS_S16       nRotationAngle; //��ת�Ƕ�
    OS_S16       nMirror;            //����ʽ
    OS_S16       nThreLightOn;       //������ֵ
    OS_S16       nThreManualWB;      //�ֶ���ƽ����ֵ
    OS_S16       enRealTimeSnap; //�Ƿ����ü�ʱץ��
    OS_S16       nFrameInterval; //֡���
    OS_S16       nAverageLuma;       //ƽ������
    OS_S16       nTemperature;       //�¶�
    OS_S16       nSubarea;           //����ͳ��
    OS_S16       nAutoExpo;     //�����ֶ�,      0,�ֶ��ع�    1:�Զ��ع�
    OS_S16       nAutoWB;        //�Զ���ƽ�⣬  0,�ֶ���ƽ��; 1:�Զ���ƽ��
    OS_S16        nDenoise;          //�Ƿ����ý��� 0:������ 1������
    OS_S16        nFrameTimeInterval;        //�ع�֡���ʱ��
    //�ڶ��ײ���
    OS_S16       npExp;          //�ٷֱ�1--300
    OS_S16       npGain;         //�ٷֱ�1--300
    OS_S16       nUseSecondParm; //�Ƿ����õڶ��ײ��� 0:������ 1:����
} cpConfigm;

typedef struct IpncSecretData
{
    OS_S32 nCommand; //0:��ȡ����ID  1:д�����ID
    OS_S32 nLen; //���ݳ���
    OS_U8 szData[SECRET_DATA_SIZE]; //��������BUF,��Ч�ֽڳ��� nLen
} IpncSecretData;

typedef struct red_streng
{
    OS_S16 nLeft;          //���Ͻ�X
    OS_S16 nTop;       // ���Ͻ�Y
    OS_S16 nRight;     //���½�X
    OS_S16 nBottom;    // ���½�Y

} red_streng;

typedef struct ExtJfif
{
    OS_U8                Header[4];              //��չͷ����־�ͳ���  header[0:1] 0xFF 0xE1    header[2:3]=OS_U16 (sizeof(EXT_JFIF)-2)
    OS_U8                Company[8];         //��˾���Ƽ���Ʒϵ��  itarge   itargeb  itargek ......
    ENITPVer            NicpVer;                //NITPЭ��汾
    OS_S32                 PicLen;                 //ͼƬ�ܳ���
    PicResolution       ImageRes;               //ͼ��ֱ���
    PicType             ImageType;              //ͼ������
    OS_U32       PicFrameCnt;            //���������֡���
    OS_U32      ExpoTimeUtcS; //struct timeval      ExpoTime;               //�ع�ʱ��linux timeval
    OS_U32      ExpoTimeUtcUS;
    TrigInfo            PicTrig;                //���������Ϣ
    H264Prm             H264Info;               //h264������Ϣ
    DeviceInfo          DevInfo;                //�豸��Ϣ
    cpConfigm            sysInfo;                //ϵͳ������Ϣ
    OS_U8      xzxSecretID[4];
    OS_U16 PWtype; //Ƶ�������������ķ�ʽ 1:���ݿ��Ŵ�С 2:�û�����
    OS_U16 PWvalue; //Ƶ����������������ֵ,��Χ1--5ms,ֻ���û�����ʱ��Ч
    OS_U16 nUseFlashLamp; //��ȡ��������ƿ��� 1:�� 2:��
    OS_U16 RadarSpeed; //��ģʽ �״��ٶ�
    OS_S32 red_strengn_Num; //����������
    red_streng  red_streng_area[4];
    OS_U32 a_unSlAtt[4];
    OS_U8 LightInfo[12];
    OS_S32 Reserve[45];
    //OS_S32 Reserve[48];
    //OS_S32 Reserve[61]; //�����ֶκ�����չʹ��
} ExtJfif;  //ץ��ͼƬͷ����չ�ṹ

typedef struct _RelatedFileName
{
    OS_S32  nNum;
    OS_U8 szFileName[4][512];
} RelatedFileName;


typedef struct  ITE_Alg_FileInfo
{
    int       dirNum;                 //Ŀ¼�ֲ�����
    char     pfilename[512];         //���ɵ��ļ�����Ϣ
    char     dirpath[8][128];        //���8����Ŀ¼ͼƬ�洢·��
    int       resv[16];               //�����ֶ�
} ITE_Alg_FileInfo;

    typedef struct  ITE_PublicPara
    {
        char        header[4];                //��չͷ����־�ͳ���  header[0:1] 0xFF 0xE2    header[2:3]=unsigned short
        int         frameId;                   //��ǰ֡��
        ITE_Alg_FileInfo file;                       //�ϴ��ļ�Ŀ¼���ļ���
        int existOSD;                         //�Ƿ����OSD  ����
        int Osdbufoffset;                   // OSD ������Ϣ�ĵ�ַ
        int Osdbuflen;                       // OSD ������Ϣ����
        int existMergeJPG;                // �Ƿ���ҪͼƬ�ϳ�
        int MergeArgOffset;              // ͼƬ�ϳɲ����ĵ�ַ
        int MergeArglen;                  // ͼƬ�ϳɲ����ĳ���
        int Company;                        //��˾��־
        int sec;                                // ���ӵ�OSD ��
        int msec;                             // ���ӵ�OSD ��
        int capType;                        // ץ�Ľ������ 1  ͨ�� 2ΪΥ��
        int frame_s[3];
	int frame_ms[3];
	char lightSta[4];
	int FlowType;                        // �Ƿ��г���־
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
