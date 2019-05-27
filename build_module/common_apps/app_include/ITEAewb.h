#ifndef _ITE_AEWB_H_
#define _ITE_AEWB_H_

/* ======================================= for aewb ====================================*/
//������賯��˵��
typedef enum alg_direction {
    algNone         = 0,//������
    algEastToWest   = 1,//����ɶ�����
    algWestToEast   = 2,//���������
    algNorthToSouth = 3,//����ɱ�����
    algSouthToNorth = 4,//���������
} ALG_DIRECTION;


//���ģʽ
typedef enum alg_status_mod {
    algNormalMod = 0,           // ����ģʽ
    algMiddayMod = 1,           // ����ģʽ
    algDuskMod = 2,             // �ƻ�ģʽ
    algMorningMod = 3,          // �峿ģʽ
    algDarkMod = 4,             // �ڰ�ģʽ
    algCurrMod = 5              // ��ǰģʽ
} EStatusMod;

//color correction matrix ��ɫУ������-OK
typedef struct {
    OS_S32 rgb_mul_rr;             // ȡֵ��ΧΪ-128~128
    OS_S32 rgb_mul_gr;             // ȡֵ��ΧΪ-128~128
    OS_S32 rgb_mul_br;             // ȡֵ��ΧΪ-128~128
    OS_S32 rgb_mul_rg;             // ȡֵ��ΧΪ-128~128
    OS_S32 rgb_mul_gg;             // ȡֵ��ΧΪ-128~128
    OS_S32 rgb_mul_bg;             // ȡֵ��ΧΪ-128~128
    OS_S32 rgb_mul_rb;             // ȡֵ��ΧΪ-128~128
    OS_S32 rgb_mul_gb;             // ȡֵ��ΧΪ-128~128
    OS_S32 rgb_mul_bb;             // ȡֵ��ΧΪ-128~128
    OS_S32 rgb_oft_or;             // ȡֵ��ΧΪ-128~128
    OS_S32 rgb_oft_og;             // ȡֵ��ΧΪ-128~128
    OS_S32 rgb_oft_ob;             // ȡֵ��ΧΪ-128~128
} ALG_COLOR_MATRIX;

typedef struct {
    OS_U32 awb_red;       // ȡֵ��ΧΪ0~255
    OS_U32 awb_green;     // ȡֵ��ΧΪ0~255
    OS_U32 awb_blue;      // ȡֵ��ΧΪ0~255
} AWB_RESULT_VALUE;

typedef struct {
    OS_U32 uDayOrNight;   // 0��ʾ����; 1��ʾҹ��
    OS_U32 uFlashLightStatus; // 0��ʾ����ƹر�;1��ʾ����ƿ���
    OS_U32 uZoneExp;      // �����ع⣬��λ9bit��Ч��ÿ��bit����һ������

    // OS_U32 ALG_uBypassSwitch;
    // //��·���ƿ��أ����򿪣���ʹ��оƬ�����awb/ae
    // OS_U32 ALG_uStatusMod; //��ǰ��ģʽ
    OS_U32 AE_uSwitch;    // �Զ��ع��㷨����,ȡֵΪALG_OFF
    // ,ALG_ON,ALG_BYPASS
    OS_U32 AWB_uSwitch;   // �Զ���ƽ���㷨����,ȡֵΪALG_OFF
    // ,ALG_ON,ALG_BYPASS
    OS_U32 max_uExposure; // ��������,1~7000*10 us
    OS_U32 min_uExposure; // ��������,1~7000*10 us
    OS_U32 max_uGain;     // ��������,1~100
    OS_U32 min_uGain;     // ��������,������

    // ��ǰ������������ǵ�ǰģʽ�Ĳ���
    OS_U32 uCurBrightness;    // ��ǰ����,ȡֵ��ΧΪ0~255
    OS_U32 uCurContrast;      // ��ǰ�Աȶ�,ȡֵ��ΧΪ0~255
    OS_U32 uCurSaturation;    // ��ǰ���Ͷ�,ȡֵ��ΧΪ0~255
    OS_U32 uCurNoiseFilter; // ��ǰ��Ե��ǿ���ж�,ȡֵ��ΧΪ0~255
    OS_U32 uCurEEStrength;    // ��ǰ��Ե��ǿǿ��,ȡֵ��ΧΪ0~255
    OS_U32 uCurBLC;           // ��ǰ�ڵ�ƽ
    OS_U32 uCurGamma;         // ��ǰgammaֵ

    OS_U32 uNorBrightness;    // ����ģʽ����,ȡֵ��ΧΪ0~255
    OS_U32 uNorContrast;      // ����ģʽ�Աȶ�,ȡֵ��ΧΪ0~255
    OS_U32 uNorSaturation;    // ����ģʽ���Ͷ�,ȡֵ��ΧΪ0~255
    OS_U32 uNorNoiseFilter; // ����ģʽ��Ե��ǿ���ж�,ȡֵ��ΧΪ0~255
    OS_U32 uNorEEStrength;    // ����ģʽ��Ե��ǿǿ��,ȡֵ��ΧΪ0~255
    OS_S32 uNorBLC;                    // ����ģʽ�ڵ�ƽ
    OS_U32 uNorGamma;         // ����ģʽgammaֵ

    OS_U32 uMidBrightness;    // ����ģʽ����,ȡֵ��ΧΪ0~255
    OS_U32 uMidContrast;      // ����ģʽ�Աȶ�,ȡֵ��ΧΪ0~255
    OS_U32 uMidSaturation;    // ����ģʽ���Ͷ�,ȡֵ��ΧΪ0~255
    OS_U32 uMidNoiseFilter; // ����ģʽ��Ե��ǿ���ж�,ȡֵ��ΧΪ0~255
    OS_U32 uMidEEStrength;    // ����ģʽ��Ե��ǿǿ��,ȡֵ��ΧΪ0~255
    OS_U32 uMidBLC;           // ����ģʽ�ڵ�ƽ
    OS_U32 uMidGamma;         // ����ģʽgammaֵ

    OS_U32 uDuskBrightness;   // �ƻ�ģʽ����,ȡֵ��ΧΪ0~255
    OS_U32 uDuskContrast;     // �ƻ�ģʽ�Աȶ�,ȡֵ��ΧΪ0~255
    OS_U32 uDuskSaturation;   // �ƻ�ģʽ���Ͷ�,ȡֵ��ΧΪ0~255
    OS_U32 uDuskNoiseFilter;// �ƻ�ģʽ��Ե��ǿ���ж�,ȡֵ��ΧΪ0~255
    OS_U32 uDuskEEStrength;   // �ƻ�ģʽ��Ե��ǿǿ��,ȡֵ��ΧΪ0~255
    OS_U32 uDuskBLC;          // �ƻ�ģʽ�ڵ�ƽ
    OS_U32 uDuskGamma;        // �ƻ�ģʽgammaֵ

    OS_U32 uMorBrightness;    // �峿ģʽ����,ȡֵ��ΧΪ0~255
    OS_U32 uMorContrast;      // �峿ģʽ�Աȶ�,ȡֵ��ΧΪ0~255
    OS_U32 uMorSaturation;    // �峿ģʽ���Ͷ�,ȡֵ��ΧΪ0~255
    OS_U32 uMorNoiseFilter; // �峿ģʽ��Ե��ǿ���ж�,ȡֵ��ΧΪ0~255
    OS_U32 uMorEEStrength;    // �峿ģʽ��Ե��ǿǿ��,ȡֵ��ΧΪ0~255
    OS_U32 uMorBLC;           // �峿ģʽ�ڵ�ƽ
    OS_U32 uMorGamma;         // �峿ģʽgammaֵ

    OS_U32 uDarkBrightness;   // �ڰ�ģʽ����,ȡֵ��ΧΪ0~255
    OS_U32 uDarkContrast;     // �ڰ�ģʽ�Աȶ�,ȡֵ��ΧΪ0~255
    OS_U32 uDarkSaturation;   // �ڰ�ģʽ���Ͷ�,ȡֵ��ΧΪ0~255
    OS_U32 uDarkNoiseFilter;// �ڰ�ģʽ��Ե��ǿ���ж�,ȡֵ��ΧΪ0~255
    OS_U32 uDarkEEStrength;   // �ڰ�ģʽ��Ե��ǿǿ��,ȡֵ��ΧΪ0~255
    OS_S32 uDarkBLC;                   // �ڰ�ģʽ�ڵ�ƽ
    OS_U32 uDarkGamma;        // �ڰ�ģʽgammaֵ

    // OS_U32 day_uBrightness; //��������
    // OS_U32 night_uBrightness; //ҹ������
    // OS_U32 day_uContrast; //����Աȶ�
    // OS_U32 night_uContrast; //ҹ��Աȶ�
    // OS_U32 day_uSaturation; //���챥�Ͷ�
    // OS_U32 night_uSaturation; //ҹ�䱥�Ͷ�
    // OS_U32 day_uNoiseFilter; //�����Ե��ǿ���ж�
    // OS_U32 night_uNoiseFilter; //ҹ���Ե��ǿ���ж�
    // OS_U32 day_uEEStrength; //�����Ե��ǿǿ��
    // OS_U32 night_uEEStrength; //ҹ���Ե��ǿǿ��
    // OS_U32 day_uBLC; //����ڵ�ƽ
    // OS_U32 night_uBLC; //ҹ��ڵ�ƽ
    // OS_U32 day_uGamma; //����gammaֵ
    // OS_U32 night_uGamma; //ҹ��gammaֵ

    OS_S32 nCurCompensation;       // ��ǰ�عⲹ��;ҹ�����عⲹ��
    OS_S32 nMidCompensation;       // �����عⲹ��;ҹ�����عⲹ��
    OS_S32 nNorCompensation;       // �����عⲹ��;ҹ�����عⲹ��
    OS_S32 nDuskCompensation;      // �ƻ��عⲹ��;ҹ�����عⲹ��
    OS_S32 nMorCompensation;       // �糿�عⲹ��;ҹ�����عⲹ��
    OS_S32 nDarkCompensation;      // �ڰ��عⲹ��;ҹ�����عⲹ��

    OS_U32 night_uRedGain;    // OK, ҹ��̶���ɫ����,ȡֵ��ΧΪ0~255
    OS_U32 night_uGreenGain;  // OK, ҹ��̶���ɫ����,ȡֵ��ΧΪ0~255
    OS_U32 night_uBlueGain;   // OK, ҹ��̶���ɫ����,ȡֵ��ΧΪ0~255

    OS_U32 my_uExposure;  // ��ǰ�ع�ʱ��,ȡֵ��ΧΪ1~70000
    OS_U32 my_uGain;      // ��ǰ����,ȡֵ��ΧΪ1~100
    OS_U32 uBeforeTowHourExp; // ����Сʱ֮ǰ���ع�
    OS_U32 uBeforeTowHourGain;    // ����Сʱ֮ǰ������
    AWB_RESULT_VALUE my_sAWB;   // �㷨����İ�ƽ��ϵ��

    OS_S32 FlashLightLum;          // ���������������ֵ��΢����Χ-10~10
    OS_S32 ManualAWBLun;           // �ֶ���ƽ����ֵ����Χ-10~10
    ALG_COLOR_MATRIX sColorMatrix;  // ��ɫ����
    EStatusMod eStatus;         // ���ģʽ
    OS_U32 uCurLum;       //��ǰ����ֵ
    OS_U32 nNightAWBSwitch;//ҹ���ƽ�⿪�� 0�ر� 1����
    OS_U32 nNightAWBThershiold; //ҹ���ƽ����ֵ 0-255

    OS_U32 nARM_Ctrl_ISP_Switch;//�ͻ���arm�˿���isp���ܽӿ�;0�ͻ�������;1 �ͻ�ʹ���Լ�ae;2 �ͻ�ʹ���Լ�awb;3 �ͻ�ʹ���Լ���ae��awb

    OS_U32 uAE_Speed_threshold;//AE�㷨���ڿ�������ֵ

    OS_U32 uALG_Light_Switch;//�㷨������Զ����ƿ���  0��ʾ�������ص� 1��ʾ���������ص�

    OS_U32 uALG_Wide_dynamic_switch;//��̬ģʽ
    OS_U32 uALG_Wide_dynamic_level;//��̬����

    OS_U32 AE_uSwitch_II;    //���ײ����Զ��ع��㷨����,ȡֵΪALG_OFF
    OS_U32 my_uExposure_II;  // ���ײ�����ǰ�ع�ʱ��,ȡֵ��ΧΪ1~70000
    OS_U32 my_uGain_II;      // ���ײ�����ǰ����,ȡֵ��ΧΪ1~100
    OS_U32 max_uExposure_II; // ���ײ�����������,1~7000*10 us
    OS_U32 min_uExposure_II; //���ײ��� ��������,1~7000*10 us
    OS_U32 max_uGain_II;     // ���ײ�����������,1~100
    OS_U32 min_uGain_II;     // ���ײ�����������,������

    OS_U16 nRedSecond;					//��ɫ����(���׻�ڶ���SENSOR)
    OS_U16 nGreenSecond;				//��ɫ����(���׻�ڶ���SENSOR)
    OS_U16 nBlueSecond;					//��ɫ����(���׻�ڶ���SENSOR)
    
    OS_U16 nNightAWBSwitchSecond;		//ҹ���ƽ�⿪��0�ر�1����(���׻�ڶ���SENSOR)
    OS_U16 nNightAWBThershioldSecond;	//ҹ���ƽ����ֵ0-255 (���׻�ڶ���SENSOR)
    OS_S16 flashwhite;							//����ư�ƽ�ⲹ��
    OS_F32     TomapFreq;  //Ƶ��
    OS_F32     TomapLevel; //ǿ�ȵȼ�
    OS_F32     TomapDetail;//ϸ��

    OS_S32 nReserve[12];           // ����
} ALG_PARA_DATA;



typedef struct IPNC_CameraPosition {
    OS_S32 direct;//�������    =algNone��ʱ�� �����÷���Ͳ�������˳���㷨����
    OS_U8 skHour;//˳����ʼСʱ
    OS_U8 skMinute;// ˳����ʼ����
    OS_U8 sjHour;//˳�����Сʱ
    OS_U8 sjMinute;// ˳���������
    OS_U8 nkHour;//�����ʼСʱ
    OS_U8 nkMinute;// �����ʼ����
    OS_U8 njHour;//������Сʱ
    OS_U8 njMinute;// ����������
    OS_S16  nOpenAlg;//�Ƿ��˳����Զ��㷨���� ���������=algNone��ʱ�� ��Ч
    OS_S16  sgCompensate;//˳�ⲹ��
    OS_S16  ngCompensate;//˳�ⲹ��
    OS_S16  nResv[13];//����
} IPNC_CameraPosition, *PIPNC_CameraPosition;

#endif
