/*
 * triton2_usb.h: Triton2 USB driver definitions.
 *
 * (C) Copyright 2010
 * Mistral Solutions Private Limited, <www.mistralsolutions.com>
 *
 * Author :
 * Naveen <anaveen@mistralsolutions.com>
 *
 * Derived from OMAP2EVM ITBOK.
 *
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */
/*
 * modification history
 * --------------------
 *   10 mar 2009 - Created from OMAP2 ITBOK source.
 */


#ifndef __MS_DIAG_T2_USB_DRV_H__
#define __MS_DIAG_T2_USB_DRV_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "types.h"
#include "mcbsp_drv.h"
#include "triton2_drv.h"

/* USB area in Triton2 I2C */
#define	T2USB_DEVICE_ADDR				0x48
/* Audio area in Triton2 I2C */
#define	T2AUDIO_DEVICE_ADDR				0x49
/* Power area in Triton2 I2C */
#define T2PWR_DEVICE_ADDR           	0x4B


/* Power register offsets */
#define T2PROTECT_KEY_REG				0x44
#define T2VUSB_DEDICATED2_REG           0xD9
#define T2CFG_BOOT_REG					0x3B
#define	T2VUSB1V5_DEV_GRP_REG			0xCC
#define	T2VUSB1V8_DEV_GRP_REG			0xCF
#define	T2VUSB3V2_DEV_GRP_REG			0xD2

#define TRITON2_IDCODE_SIZE				0x4
#define TRITON2_IDCODE_ES1_0			0x0009002F
#define TRITON2_IDCODE_ES2_0			0x1009002F
#define TRITON2_IDCODE_ES2_1			0x2009002F
#define TRITON2_IDCODE_REG_OFFSET  		0x85

#define TRITON2_NOT_PRESENT				0
#define TRITON2_PRESENT					1


#define USB_ULPI					1
#define USB_ULPI_3PIN_MODE			2
#define USB_ULPI_4PIN_MODE			3
#define USB_ULPI_4PIN_2430C_MODE 	4
#define USB_CEA2011_3PIN_MODE		5
#define USB_CEA2011_4PIN_MODE		7

/* Register offset address definitions */

/* i2c_0x48_usb */
#define I2C_USB                                                                 0x48
#define USB_U_BASE                                                              0x0

/* i2c_0x49_aud_int */
#define I2C_AUD_INT                                                             0x49
#define AUDIO_VOICE_U_BASE                                                      0x1
#define TEST_U_BASE                                                             0x4c
#define PIH_U_BASE                                                              0x80
#define INTBR_U_BASE                                                            0x85
#define GPIO_U_BASE                                                             0x98

/* i2c_0x4a_aux */
#define I2C_AUX                                                                 0x4a
#define MADC_U_BASE                                                             0x0
#define MAIN_CHARGE_U_BASE                                                      0x74
#define PRECHARGE_U_BASE                                                        0xaa
#define INTERRUPTS_U_BASE                                                       0xb9
#define keypad_U_BASE                                                           0xd2
#define LED_U_BASE                                                              0xee
#define PWMA_U_BASE                                                             0xef
#define PWMB_U_BASE                                                             0xf1
#define PWM0_U_BASE                                                             0xf8
#define PWM1_U_BASE                                                             0xfb

/* i2c_0x4B_pwr */
#define I2C_PWR                                                                 0x4B
#define SECURED_REG_U_BASE                                                      0x0
#define BACKUP_REG_U_BASE                                                       0x14
#define RTC_U_BASE                                                              0x1c
#define INT_U_BASE                                                              0x2e
#define PM_MASTER_U_BASE                                                        0x36
#define PM_RECEIVER_U_BASE                                                      0x5b

#define INTBRPMBR1_OFFSET  															0x92

/* Register set LDO_USB address offset, bank address increment and number of banks */

#define PM_RECEIVER_LDO_USB_OFFSET                                              0x71
#define PM_RECEIVER_LDO_USB_STEP                                                0xe
#define PM_RECEIVER_LDO_USB_BANKS                                               1

/* Register offset address definitions relative to register set LDO_USB */

#define PM_RECEIVER_VUSB1V5_DEV_GRP_OFFSET                                      0x0
#define PM_RECEIVER_VUSB1V5_TYPE_OFFSET                                         0x1
#define PM_RECEIVER_VUSB1V5_REMAP_OFFSET                                        0x2
#define PM_RECEIVER_VUSB1V8_DEV_GRP_OFFSET                                      0x3
#define PM_RECEIVER_VUSB1V8_TYPE_OFFSET                                         0x4
#define PM_RECEIVER_VUSB1V8_REMAP_OFFSET                                        0x5
#define PM_RECEIVER_VUSB3V1_DEV_GRP_OFFSET                                      0x6
#define PM_RECEIVER_VUSB3V1_TYPE_OFFSET                                         0x7
#define PM_RECEIVER_VUSB3V1_REMAP_OFFSET                                        0x8
#define PM_RECEIVER_VUSBCP_DEV_GRP_OFFSET                                       0x9
#define PM_RECEIVER_VUSBCP_TYPE_OFFSET                                          0xa
#define PM_RECEIVER_VUSBCP_REMAP_OFFSET                                         0xb
#define PM_RECEIVER_VUSB_DEDICATED1_OFFSET                                      0xc
#define PM_RECEIVER_VUSB_DEDICATED2_OFFSET                                      0xd



// Register Addresses:
enum Triton2USBRegs
{
    VENDOR_ID_LO,
    VENDOR_ID_HI,
    PRODUCT_ID_LO,
    PRODUCT_ID_HI,
    FUNC_CTRL,
    FUNC_CTRL_SET,
    FUNC_CTRL_CLR,
    IFC_CTRL,
    IFC_CTRL_SET,
    IFC_CTRL_CLR,
    OTG_CTRL,
    OTG_CTRL_SET,
    OTG_CTRL_CLR,
    USB_INT_EN_RISE,
    USB_INT_EN_RISE_SET,
    USB_INT_EN_RISE_CLR,
    USB_INT_EN_FALL,
    USB_INT_EN_FALL_SET,
    USB_INT_EN_FALL_CLR,
    USB_INT_STS,
    USB_INT_LATCH,
    DEBUG,
    SCRATCH_REG,
    SCRATCH_REG_SET,
    SCRATCH_REG_CLR,
    CARKIT_CTRL,
    CARKIT_CTRL_SET,
    CARKIT_CTRL_CLR,
    CARKIT_INT_DELAY,
    CARKIT_INT_EN,
    CARKIT_INT_EN_SET,
    CARKIT_INT_EN_CLR,
    CARKIT_INT_STS,
    CARKIT_INT_LATCH,
    CARKIT_PLS_CTRL,
    CARKIT_PLS_CTRL_SET,
    CARKIT_PLS_CTRL_CLR,
    TRANS_POS_WIDTH,
    TRANS_NEG_WIDTH,
    RCV_PLTY_RECOVERY,
    MCPC_CTRL = 0x30,
    MCPC_CTRL_SET,
    MCPC_CTRL_CLR,
    MCPC_IO_CTRL,
    MCPC_IO_CTRL_SET,
    MCPC_IO_CTRL_CLR,
    MCPC_CTRL2,
    MCPC_CTRL2_SET,
    MCPC_CTRL2_CLR,
    OTHER_FUNC_CTRL = 0x80,
    OTHER_FUNC_CTRL_SET,
    OTHER_FUNC_CTRL_CLR,
    OTHER_IFC_CTRL,
    OTHER_IFC_CTRL_SET,
    OTHER_IFC_CTRL_CLR,
    OTHER_INT_EN_RISE,
    OTHER_INT_EN_RISE_SET,
    OTHER_INT_EN_RISE_CLR,
    OTHER_INT_EN_FALL,
    OTHER_INT_EN_FALL_SET,
    OTHER_INT_EN_FALL_CLR,
    OTHER_INT_STS,
    OTHER_INT_LATCH,
    ID_INT_EN_RISE,
    ID_INT_EN_RISE_SET,
    ID_INT_EN_RISE_CLR,
    ID_INT_EN_FALL,
    ID_INT_EN_FALL_SET,
    ID_INT_EN_FALL_CLR,
    ID_INT_STS,
    ID_INT_LATCH,
    ID_STATUS,
    CARKIT_SM_1_INT_EN,
    CARKIT_SM_1_INT_EN_SET,
    CARKIT_SM_1_INT_EN_CLR,
    CARKIT_SM_1_INT_STS,
    CARKIT_SM_1_INT_LATCH,
    CARKIT_SM_2_INT_EN,
    CARKIT_SM_2_INT_EN_SET,
    CARKIT_SM_2_INT_EN_CLR,
    CARKIT_SM_2_INT_STS,
    CARKIT_SM_2_INT_LATCH,
    CARKIT_SM_CTRL,
    CARKIT_SM_CTRL_SET,
    CARKIT_SM_CTRL_CLR,
    CARKIT_SM_CMD,
    CARKIT_SM_CMD_SET,
    CARKIT_SM_CMD_CLR,
    CARKIT_SM_CMD_STS,
    CARKIT_SM_STATUS,
    CARKIT_SM_NEXT_STATUS,
    CARKIT_SM_ERR_STATUS,
    CARKIT_SM_CTRL_STATE,
    POWER_CTRL,
    POWER_CTRL_SET,
    POWER_CTRL_CLR,
    OTHER_IFC_CTRL2,
    OTHER_IFC_CTRL2_SET,
    OTHER_IFC_CTRL2_CLR,
    REG_CTRL_EN,
    REG_CTRL_EN_SET,
    REG_CTRL_EN_CLR,
    REG_CTRL_ERROR,
    REG_CTRL_ERROR_SET,
    REG_CTRL_ERROR_CLR,
    OTHER_FUNC_CTRL2 = 0xB8,
    OTHER_FUNC_CTRL2_SET,
    OTHER_FUNC_CTRL2_CLR,
    CARKIT_ANA_CTRL,
    CARKIT_ANA_CTRL_SET,
    CARKIT_ANA_CTRL_CLR,
    VBUS_DEBOUNCE = 0xC0,
    ID_DEBOUNCE,
    TPH_DP_CON_MIN,
    TPH_DP_CON_MAX,
    TCR_DP_CON_MIN	,
    TCR_DP_CON_MAX,
    TPH_DP_PD_SHORT,
    TPH_CMD_DLY,
    TPH_DET_RST,
    TPH_AUD_BIAS,
    TCR_UART_DET_MIN,
    TCR_UART_DET_MAX,
    TPH_STLO_DET,
    TPH_ID_INT_PW,
    TACC_ID_INT_WAIT,
    TACC_ID_INT_PW,
    TPH_CMD_WAIT,
    TPH_ACK_WAIT,
    TPH_DP_DISC_DET,
    VBAT_TIMER_VALUE,
    CARKIT_4W_DEBUG = 0xE0,
    CARKIT_5W_DEBUG,
    TEST_CTRL_SET = 0xEA,
    TEST_CTRL_CLR,
    TEST_CARKIT_SET,
    TEST_CARKIT_CLR,
    TEST_POWER_SET,
    TEST_POWER_CLR,
    TEST_ULPI,
    TXVR_EN_TEST_SET = 0xF2,
    TXVR_EN_TEST_CLR,
    VBUS_EN_TEST,
    ID_EN_TEST,
    PSM_EN_TEST_SET,
    PSM_EN_TEST_CLR,
    PHY_TRIM_CTRL = 0xFC,
    PHY_PWR_CTRL,
    PHY_CLK_CTRL,
    PHY_CLK_CTRL_STS
};

/*==== TYPES ===============================================================*/

/* Prototype Functions */
void t2usb_enable_i2crw(void);
void t2usb_enable_phypower(void);
void t2usb_power_otg(void);
S32 t2usb_init(void);
S32 t2usb_register_read(U8 t2_usbregoffset,U8 *buff);
S32 t2usb_powerconfig();
S32 t2_usb_fs_deinit(U32);

#ifdef __cplusplus
}
#endif

#endif  /*__MS_DIAG_T2_USB_DRV_H__*/

