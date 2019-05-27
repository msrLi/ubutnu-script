/**
 * @file ite_sil1127.h
 * @brief ¼Ä´æÆ÷²Ù×÷API
 * @author  <itarge@itarge.com>
 * @version 1.0.0
 * @date 2018-08-13
 */

/* Copyright(C) 2009-2017, Itarge Inc.
 * All right reserved
 *
 */

#ifndef __ITE_SIL1127_H__
#define __ITE_SIL1127_H__

OS_S32 ite_sil1127_i2c_bus_init(void);
OS_HANDLE  DecodeRegisterAddress(uint8_t regAddr);
OS_U8 RegisterRead(uint8_t regAddr, uint8_t reg_addr, uint8_t *pvalue);
void RegisterWrite(uint8_t regAddr, uint8_t reg_addr, OS_U8 value);
void RegisterReads(uint8_t regAddr, uint8_t reg_addr, OS_U8 length, OS_U8* buffer);
OS_S32 Si1127GetResolutionFrameRate(uint16_t *p_x, uint16_t *p_y, uint8_t *p_framerate, uint8_t *p_interlaced);
OS_S32 CyFxUsbI2cSi1127Init(void);
OS_S32 GetSi1127Pwr5v(uint8_t *PwrStatus);
void Si11227ActivatePort();
void Si11227EnablePort();
void Si11227DisablePort();
void Si1127SetEDID();
#endif /* __ITE_SIL1127_H__ */
