/**
 * @file ite_sil1127_register.c
 * @brief ¼Ä´æÆ÷²Ù×÷º¯Êý
 * @author  <itarge@itarge.com>
 * @version 1.0.0
 * @date 2018-08-13
 */

/* Copyright(C) 2009-2017, Itarge Inc.
 * All right reserved
 *
 */

#include <ite_common.h>
#include <ite_i2c.h>
#include <ite_sil1127.h>


// Register Slave Addresses
#define CONFIG_I2C_SLAVE_PAGE_0          (0x60>>1)   //       0x31  / 0x30 
#define CONFIG_I2C_SLAVE_PAGE_1          (0x68>>1)   //       0x35  / 0x34  
#define CONFIG_I2C_SLAVE_PAGE_8          (0xC0>>1)   //CEC    0x60 
#define CONFIG_I2C_SLAVE_PAGE_9          (0xE0>>1)   //EDID   0x70  /NO ack  
#define CONFIG_I2C_SLAVE_PAGE_A          (0x64>>1)   //xvYCC  0x32   
#define CONFIG_I2C_SLAVE_PAGE_C          (0xE6>>1)   //CBUS   0x68 

#define DEV_ADDR                        (0x30)      //(0x60 >> 1)
#define FAC_CFG_REG1                    (0x70)      //(0xE0 >> 1)
#define FAC_CFG_REG2                    (0x32)      //(0x64 >> 1)
#define AUTO_VID_REG                    (0x70)      //(0xE0 >> 1) 
#define HDMI_MUTE                       (0x34)      //(0x68 >> 1)
#define EDID_ADDR                       (0x70)      //(0xE0 >> 1)
#define ACR_CONTROL                     (0x32)      //(0x64 >> 1)
#define GPIO                            (0x70)      //(0xE0 >> 1)
#define AUDIO_OUT                       (0x34)      //(0x68 >> 1)
#define CEC_PA_EDID_OFFSET              (0x9A)

OS_U8 physicalAddr[4] = {(0x1000 >> 8) & 0xFF, (0x2000 >> 8) & 0xFF , (0x3000 >> 8) & 0xFF , (0x4000 >> 8) & 0xFF  };

struct s_init_data
{
    OS_U8 dev_addr;
    OS_U8 reg_addr;
    OS_U8 data;
};

static struct s_init_data init_data[] =
{
    {DEV_ADDR, 0x0E, 0x40},
    {FAC_CFG_REG1, 0xE5, 0x02},
    {FAC_CFG_REG2, 0x81, 0x18},
    {FAC_CFG_REG2, 0x87, 0x43},
    {FAC_CFG_REG2, 0x89, 0x00},
    {FAC_CFG_REG2, 0x92, 0x8A},
    {FAC_CFG_REG2, 0x93, 0xAA},
    {FAC_CFG_REG2, 0x94, 0x1A},
    {FAC_CFG_REG2, 0x95, 0x2A},
    {FAC_CFG_REG2, 0xB5, 0x40},
    {FAC_CFG_REG2, 0xBB, 0x04},
    {FAC_CFG_REG2, 0x87, 0x03},
    {FAC_CFG_REG2, 0x81, 0x10},
    {FAC_CFG_REG2, 0x88, 0x40},
    {FAC_CFG_REG2, 0x87, 0x43},
    {FAC_CFG_REG2, 0x81, 0x18},
    {DEV_ADDR, 0x08, 0x06},  //disable port for fixing the Type-c Dongle 1127 HPD signal can't stop source transmit
    {DEV_ADDR, 0xFC, 0x03},
    {DEV_ADDR, 0xFA, 0x03},
    {DEV_ADDR, 0x48, 0xC0}, // old 00  // 0xC0
    {EDID_ADDR, 0x40, 0x0B},
    {DEV_ADDR, 0x5F, 0xF0},
    {DEV_ADDR, 0xB5, 0x04},
    {DEV_ADDR, 0xBA, 0xC0},
    {DEV_ADDR, 0x0A, 0x09},
    {DEV_ADDR, 0x6A, 0x01},
    //{DEV_ADDR, 0x49, 0x02},
    //{DEV_ADDR, 0x4A, 0x1}, //video mode #1 register
    //{DEV_ADDR, 0x81, 0x01}, //TMDS anlog control reg # 2
    {AUDIO_OUT, 0x27, 0x19}, // I2S cntrl #2
    {AUDIO_OUT, 0x29, 0x1C},
    {DEV_ADDR, 0x08, 0x05},    // LHB add
    {DEV_ADDR, 0x07, 0x40}
};

static unsigned char edid[256] =
{
    // HP EDID //
#if 0
    0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x22, 0x03, 0x34, 0x35, 0x00, 0x00, 0x00, 0x00,
    0xFF, 0x1C, 0x01, 0x03, 0x80, 0x00, 0x00, 0x78, 0x0A, 0x0D, 0xC9, 0xA0, 0x57, 0x47, 0x98, 0x27,
    0x12, 0x48, 0x4C, 0x21, 0x08, 0x00, 0x81, 0x00, 0x81, 0x40, 0x81, 0x80, 0x95, 0x00, 0xA9, 0xC0,
    0xA9, 0x40, 0xB3, 0x00, 0x01, 0x01, 0x02, 0x3A, 0x80, 0x18, 0x71, 0x38, 0x2D, 0x40, 0x58, 0x2C,
    0x45, 0x00, 0xC4, 0x8E, 0x21, 0x00, 0x00, 0x1E, 0x01, 0x1D, 0x00, 0x72, 0x51, 0xD0, 0x1E, 0x20,
    0x6E, 0x28, 0x55, 0x00, 0xC4, 0x8E, 0x21, 0x00, 0x00, 0x1E, 0x00, 0x00, 0x00, 0xFC, 0x00, 0x48,
    0x50, 0x20, 0x56, 0x44, 0x4F, 0x20, 0x49, 0x4E, 0x20, 0x47, 0x31, 0x0A, 0x00, 0x00, 0x00, 0xFD,
    0x00, 0x17, 0x78, 0x0F, 0x7E, 0x17, 0x00, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x01, 0x36,

    0x02, 0x03, 0x1C, 0x72, 0x49, 0x90, 0x01, 0x02, 0x03, 0x04, 0x11, 0x12, 0x13, 0x1F, 0x23, 0x09,
    0x07, 0x07, 0x83, 0x01, 0x00, 0x00, 0x65, 0x03, 0x0C, 0x00, 0x10, 0x00, 0x01, 0x1D, 0x00, 0xBC,
    0x52, 0xD0, 0x1E, 0x20, 0xB8, 0x28, 0x55, 0x40, 0xC4, 0x8E, 0x21, 0x00, 0x00, 0x1E, 0xF3, 0x39,
    0x80, 0xD0, 0x72, 0x38, 0x2D, 0x40, 0x10, 0x2C, 0x45, 0x80, 0xC4, 0x8E, 0x21, 0x00, 0x00, 0x1E,
    0x8C, 0x0A, 0xD0, 0x8A, 0x20, 0xE0, 0x2D, 0x10, 0x10, 0x3E, 0x96, 0x00, 0x13, 0x8E, 0x21, 0x00,
    0x00, 0x18, 0x66, 0x21, 0x50, 0xB0, 0x51, 0x00, 0x1B, 0x30, 0x40, 0x70, 0x36, 0x00, 0x12, 0x8E,
    0x21, 0x00, 0x00, 0x18, 0x66, 0x21, 0x56, 0xAA, 0x51, 0x00, 0x1E, 0x30, 0x46, 0x8F, 0x33, 0x00,
    0x12, 0x8E, 0x21, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xBA
#else
    0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x4D, 0x29, 0x23, 0x92, 0x01, 0x00, 0x00, 0x00,
    0x00, 0x12, 0x01, 0x03, 0x80, 0x00, 0x00, 0x78, 0x0A, 0x0D, 0xC9, 0xA0, 0x57, 0x47, 0x98, 0x27,
    0x12, 0x48, 0x4C, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x02, 0x3A, 0x80, 0x18, 0x71, 0x38, 0x2D, 0x40, 0x58, 0x2C,
    0x45, 0x00, 0xC4, 0x8E, 0x21, 0x00, 0x00, 0x1E, 0x01, 0x1D, 0x00, 0x72, 0x51, 0xD0, 0x1E, 0x20,
    0x6E, 0x28, 0x55, 0x00, 0xC4, 0x8E, 0x21, 0x00, 0x00, 0x1E, 0x00, 0x00, 0x00, 0xFC, 0x00, 0x43,
    0x50, 0x39, 0x32, 0x32, 0x33, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00, 0x00, 0x00, 0xFD,
    0x00, 0x17, 0x78, 0x0F, 0x7E, 0x17, 0x00, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x01, 0x66,

    0x02, 0x03, 0x3D, 0x72, 0x55, 0x90, 0x04, 0x03, 0x02, 0x0E, 0x0F, 0x07, 0x23, 0x24, 0x05, 0x94,
    0x13, 0x12, 0x11, 0x1D, 0x1E, 0xA0, 0xA1, 0xA2, 0x01, 0x1F, 0x35, 0x09, 0x7F, 0x07, 0x09, 0x7F,
    0x07, 0x17, 0x07, 0x50, 0x3F, 0x06, 0xC0, 0x57, 0x06, 0x00, 0x5F, 0x7F, 0x01, 0x67, 0x1F, 0x00,
    0x83, 0x4F, 0x00, 0x00, 0x68, 0x03, 0x0C, 0x00, 0x10, 0x00, 0xB8, 0x2D, 0x00, 0x8C, 0x0A, 0xD0,
    0x8A, 0x20, 0xE0, 0x2D, 0x10, 0x10, 0x3E, 0x96, 0x00, 0xC4, 0x8E, 0x21, 0x00, 0x00, 0x18, 0x8C,
    0x0A, 0xD0, 0x90, 0x20, 0x40, 0x31, 0x20, 0x0C, 0x40, 0x55, 0x00, 0xC4, 0x8E, 0x21, 0x00, 0x00,
    0x18, 0x01, 0x1D, 0x80, 0x18, 0x71, 0x1C, 0x16, 0x20, 0x58, 0x2C, 0x25, 0x00, 0xC4, 0x8E, 0x21,
    0x00, 0x00, 0x9E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7E
#endif
};


OS_HANDLE g_i2c_slave_page0_handle = NULL;
OS_HANDLE g_i2c_slave_page1_handle = NULL;
OS_HANDLE g_i2c_slave_page8_handle = NULL;
OS_HANDLE g_i2c_slave_page9_handle = NULL;
OS_HANDLE g_i2c_slave_page_a_handle = NULL;
OS_HANDLE g_i2c_slave_page_c_handle = NULL;


OS_S32 ite_sil1127_i2c_bus_init(void)
{
    OS_S32 ret = 0;
    OS_S32 i2c_bus = 0;

    i2c_bus = 11;
    ret = ITE_I2C_Init(i2c_bus, CONFIG_I2C_SLAVE_PAGE_0, 1, 0, 0, &g_i2c_slave_page0_handle);
    ret = ITE_I2C_Init(i2c_bus, CONFIG_I2C_SLAVE_PAGE_1, 1, 0, 0, &g_i2c_slave_page1_handle);
    ret = ITE_I2C_Init(i2c_bus, CONFIG_I2C_SLAVE_PAGE_8, 1, 0, 0, &g_i2c_slave_page8_handle);
    ret = ITE_I2C_Init(i2c_bus, CONFIG_I2C_SLAVE_PAGE_9, 1, 0, 0, &g_i2c_slave_page9_handle);
    ret = ITE_I2C_Init(i2c_bus, CONFIG_I2C_SLAVE_PAGE_A, 1, 0, 0, &g_i2c_slave_page_a_handle);
    ret = ITE_I2C_Init(i2c_bus, CONFIG_I2C_SLAVE_PAGE_C, 1, 0, 0, &g_i2c_slave_page_c_handle);
    return  ret;
}
//------------------------------------------------------------------------------
// Function: DecodeRegisterAddress
// Description: Generate I2C slave address and offset based on register address.
//              Register address format is composed of 2 OS_U8s, Page Addres and Offset.
//              Page Address OS_U8 is mapped int I2C slave address.
//              Offset is used as is.
//------------------------------------------------------------------------------
OS_HANDLE  DecodeRegisterAddress(OS_U8 regAddr)
{
    switch (regAddr)
    {
        case (0x30):   // 0x60>>1
            return g_i2c_slave_page0_handle;
            break;
        case (0x34):   // 0x68>>1
            return g_i2c_slave_page1_handle;

            break;
        case (0x60):  // 0xC0>>1
            return g_i2c_slave_page8_handle;

            break;
        case (0x70):     // 0xE0>>1
            return g_i2c_slave_page9_handle;

            break;
        case (0x32):    // 0x64>>1
            return g_i2c_slave_page_a_handle;
            break;
        case (0x68):    // 0xE6>>1
            return g_i2c_slave_page_c_handle;
            break;
    }
    return g_i2c_slave_page0_handle;
}

//------------------------------------------------------------------------------
// Function: RegisterRead
// Description: Read a one OS_U8 register.
//              The register address parameter is translated into an I2C slave address and offset.
//              The I2C slave address and offset are used to perform an I2C read operation.
//------------------------------------------------------------------------------
OS_U8 RegisterRead(OS_U8 regAddr, OS_U8 reg_addr, OS_U8 *pvalue)
{
    OS_S32 ret = 0;
    OS_HANDLE slave_handle = NULL;
    OS_U32 real_len = 0;
    slave_handle = DecodeRegisterAddress(regAddr);

    ret = ITE_I2C_Read(slave_handle, reg_addr, pvalue, 1, &real_len);
    return (*pvalue);
}



//------------------------------------------------------------------------------
// Function: RegisterWrite
// Description: Write a one OS_U8 register.
//              The register address parameter is translated into an I2C slave address and offset.
//              The I2C slave address and offset are used to perform an I2C write operation.
//------------------------------------------------------------------------------
void RegisterWrite(OS_U8 regAddr, OS_U8 reg_addr, OS_U8 value)
{
    OS_S32 ret = 0;
    OS_U32 real_len = 0;
    OS_HANDLE slave_handle = NULL;

    slave_handle = DecodeRegisterAddress(regAddr);

    ret =  ITE_I2C_Write(slave_handle, reg_addr, &value, 1, &real_len);
}

void RegisterReads(OS_U8 regAddr, OS_U8 reg_addr, OS_U8 length, OS_U8* buffer)
{
    OS_S32 ret = 0;
    OS_U32 real_len = 0;
    OS_HANDLE slave_handle = NULL;
    // OS_HANDLE slave_handle;

    slave_handle = DecodeRegisterAddress(regAddr);

    ret = ITE_I2C_Read(slave_handle, reg_addr, buffer, length, &real_len);
    // I2C_ReadBlock(slaveID, offset, buffer, length);
}

OS_S32 Si1127GetResolutionFrameRate(OS_U16 *p_x, OS_U16 *p_y, OS_U8 *p_framerate, OS_U8 *p_interlaced)
{
    OS_S32 ret = 0;
    OS_U8 resolution[4];
    OS_U8 v_h_res[4];
    OS_U8 xpcnt[2];
    OS_U8 interlaced[1];
    OS_U16 v_res, h_res, pxclk;
    OS_U32 pclk, tmp;

    *p_x = *p_y = 0;
    RegisterReads(DEV_ADDR, 0x4E, 4, resolution);

    *p_x = (resolution[1] & 0xF << 8) + resolution[0];
    *p_y = (resolution[3] & 0xF << 8) + resolution[2];

    RegisterReads(DEV_ADDR, 0x55, 1, interlaced);

    *p_interlaced = (interlaced[0] & 0x04) ? 1 : 0;

    RegisterReads(DEV_ADDR, 0x3A, 4, v_h_res);

    h_res = (v_h_res[1] & 0x0F) +  v_h_res[0];
    v_res = (v_h_res[3] & 0x0F) + v_h_res[2];

    RegisterReads(DEV_ADDR, 0x6E, 2, xpcnt);

    pxclk = (xpcnt[1] & 0x0F) + xpcnt[0];

    pclk = (2700 * 2048) / pxclk;
    tmp = (pclk * 10000) / (h_res * v_res);
    *p_framerate = (OS_U8) tmp;

    return ret;
}

OS_S32 CyFxUsbI2cSi1127Init(void)
{
    OS_S32 ret = 0;
    OS_U32 i;

    RegisterWrite(DEV_ADDR, 0x05, 0x51);
    sleep(1);
    RegisterWrite(DEV_ADDR, 0x05, 0x50);
    /* reset FIFO */
    RegisterWrite(DEV_ADDR, 0x07, 0x89);
    sleep(1);
    RegisterWrite(DEV_ADDR, 0x07, 0x8);

    for (i = 0; i < sizeof(init_data) / sizeof(struct s_init_data); i++)
    {
        RegisterWrite(init_data[i].dev_addr, init_data[i].reg_addr, init_data[i].data);
    }

    Si1127SetEDID();

    return ret;
}

OS_S32 GetSi1127Pwr5v(OS_U8 *PwrStatus)
{
    OS_S32 ret = 0;
    OS_U8 pwr5v[1];

    usleep(150 * 1000);

    ret =  RegisterRead(DEV_ADDR, 0x06, pwr5v);
    *PwrStatus = pwr5v[0];

    return ret;
}


void Si11227ActivatePort()
{
    RegisterWrite(DEV_ADDR, 0x08, 0x07);
}

void Si11227EnablePort()
{
    //Si11227ActivatePort();
    RegisterWrite(DEV_ADDR, 0x08, 0x07); //for fixing the Type-c Dongle 1127 HPD signal can't stop source transmit
    //RegisterWrite(HDMI_MUTE, 0x3C, 0x01);

    usleep(500 * 1000); // for Typec- dongle no video and audio ouput
    RegisterWrite(EDID_ADDR, 0x01, 0x0F); //enable ports
    RegisterWrite(DEV_ADDR, 0x10, 0x14); //enable hotplug
}

void Si11227DisablePort()
{
    RegisterWrite(EDID_ADDR, 0x01, 0x00); //disable ports
    RegisterWrite(DEV_ADDR, 0x10, 0x00); //disable hotplug
    RegisterWrite(DEV_ADDR, 0x08, 0x06);
}

void Si1127SetEDID()
{
    /*uint32_t i;
    OS_U8 val, boot, re_edid = 0;
    OS_U8 edid_reg = 0x03;
    CyU3PReturnStatus_t status = CY_U3P_SUCCESS;
    */

    OS_U16 i;
    OS_U8 k = 0;
    OS_U8 j = 0;
    OS_U8 cksum;
    OS_U8 firstPort = 0x02;
    OS_U8 lastPort = 0x04;

    //DEBUG_PRINT(("program SRAM! \n"));

    RegisterWrite(DEV_ADDR, 0x10, 0x00); //disable hotplug before changing


    for (j = firstPort; j <= lastPort; j = j << 1)
    {

        cksum = 0;
        RegisterWrite(EDID_ADDR, 0x04, j); //enable ports
        RegisterWrite(EDID_ADDR, 0x02, 0x00); //address to start

        for (i = 0; i < 255; i++)
        {
            if (i == CEC_PA_EDID_OFFSET)
            {
                // Program specific physical address
                //
                // Port0 1000
                // Port1 2000
                // Port2 3000
                // Port3 4000


                RegisterWrite(EDID_ADDR, 0x03, physicalAddr[k]);    //High OS_U8
                cksum = cksum + (OS_U8)physicalAddr[k];

                RegisterWrite(EDID_ADDR, 0x03, 0x00);                  //Low OS_U8
                i = i + 1 ;
                k = k + 1;
            }
            else
            {
                RegisterWrite(EDID_ADDR, 0x03, edid[i]);
                cksum = cksum + edid[i];

            }
        }

        RegisterWrite(EDID_ADDR, 0x03, 0 - cksum); //2nd block

    }
    /*
       CyU3PThreadSleep(100);
       RegisterWrite(EDID_ADDR, 0x01,0x00); //disable ports
       RegisterWrite(DEV_ADDR, 0x10, 0x00); //disable hotplug
       CyU3PThreadSleep(2000);
       RegisterWrite(EDID_ADDR, 0x01,0x0F); //enable ports
       RegisterWrite(DEV_ADDR, 0x10, 0x14); //enable hotplug
    */

}
