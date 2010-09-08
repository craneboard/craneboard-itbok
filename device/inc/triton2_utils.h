/*
 * triton2_utils.h: Triton2 related definitions and function prototypes.
 *
 * (C) Copyright 2010
 * Mistral Solutions Private Limited, <www.mistralsolutions.com>
 *
 * Author :
 *	Ganeshan <nganeshan@mistralsolutions.com>
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

#ifndef __MS_DIAG_TRITON2_UTILS_H__
#define __MS_DIAG_TRITON2_UTILS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "i2c_drv.h"
#include "triton2_drv.h"

#define TRITON2_GPIO_INPUT  0
#define TRITON2_GPIO_OUTPUT 1

#define TRITON2_GPIO_SET_DATA   1
#define TRITON2_GPIO_CLEAR_DATA 0

#define TRITON2_AUDIO_MUTE_ENABLE  1
#define TRITON2_AUDIO_MUTE_DISABLE 0

#define VMMC1_V1P8 0
#define VMMC1_V3P0 1

#define TRITON2_GPIO_0  0
#define TRITON2_GPIO_1  1
#define TRITON2_GPIO_2  2
#define TRITON2_GPIO_3  3
#define TRITON2_GPIO_4  4
#define TRITON2_GPIO_5  5
#define TRITON2_GPIO_6  6
#define TRITON2_GPIO_7  7
#define TRITON2_GPIO_8  8
#define TRITON2_GPIO_9  9
#define TRITON2_GPIO_10 10
#define TRITON2_GPIO_11 11
#define TRITON2_GPIO_12 12
#define TRITON2_GPIO_13 13
#define TRITON2_GPIO_14 14
#define TRITON2_GPIO_15 15
#define TRITON2_GPIO_16 16
#define TRITON2_GPIO_17 17


/* Triton2 Processor Groups. */
#define PROCESSOR_GRP1              (0x01) /* Applications devices - Eg: OMAP2430 */
#define PROCESSOR_GRP2              (0x02) /* Modem Devices - Eg: N3G */
#define PROCESSOR_GRP3              (0x04) /* Peripherals */

#define TRITON2_RES_OFF         0x00    /* Resource in OFF */
#define TRITON2_RES_SLEEP       0x08    /* Resource in SLEEP */
#define TRITON2_RES_ACTIVE      0x0D    /* Resource in ACTIVE */

/* LED registers are located in this slave address. */
#define T2_I2C_LED_ADDR_GROUP   0x4A

/* Triton2 Power Resources register offsets. */
#define TRITON2_POWER_REG_OFFSET    (0x00)

#define TRITON2_VDAC_DEV_GRP_REG    (TRITON2_POWER_REG_OFFSET + 0x96)
#define TRITON2_VDAC_DEDICATED_REG  (TRITON2_POWER_REG_OFFSET + 0x99)

#define TRITON2_VPLL1_DEV_GRP_REG   (TRITON2_POWER_REG_OFFSET + 0x8A)
#define TRITON2_VPLL1_DEDICATED_REG (TRITON2_POWER_REG_OFFSET + 0x8D)

#define TRITON2_VPLL2_DEV_GRP_REG   (TRITON2_POWER_REG_OFFSET + 0x9E)
#define TRITON2_VPLL2_DEDICATED_REG (TRITON2_POWER_REG_OFFSET + 0x91)

#define TRITON2_VMMC1_DEV_GRP_REG   (TRITON2_POWER_REG_OFFSET + 0x82)
#define TRITON2_VMMC1_DEDICATED_REG (TRITON2_POWER_REG_OFFSET + 0x85)

#define TRITON2_VMMC2_DEV_GRP_REG   (TRITON2_POWER_REG_OFFSET + 0x86)
#define TRITON2_VMMC2_DEDICATED_REG (TRITON2_POWER_REG_OFFSET + 0x89)

#define TRITON2_VSIM_DEV_GRP_REG    (TRITON2_POWER_REG_OFFSET + 0x92)
#define TRITON2_VSIM_DEDICATED_REG  (TRITON2_POWER_REG_OFFSET + 0x95)

#define TRITON2_VAUX1_DEV_GRP_REG   (TRITON2_POWER_REG_OFFSET + 0x72)
#define TRITON2_VAUX1_DEDICATED_REG (TRITON2_POWER_REG_OFFSET + 0x75)

#define TRITON2_VAUX2_DEV_GRP_REG   (TRITON2_POWER_REG_OFFSET + 0x76)
#define TRITON2_VAUX2_DEDICATED_REG (TRITON2_POWER_REG_OFFSET + 0x79)

#define TRITON2_VAUX3_DEV_GRP_REG   (TRITON2_POWER_REG_OFFSET + 0x7A)
#define TRITON2_VAUX3_DEDICATED_REG (TRITON2_POWER_REG_OFFSET + 0x7D)

#define TRITON2_VAUX4_DEV_GRP_REG   (TRITON2_POWER_REG_OFFSET + 0x7E)
#define TRITON2_VAUX4_DEDICATED_REG (TRITON2_POWER_REG_OFFSET + 0x81)


/* Triton2 Power Bus Registers. */
#define TRITON2_PB_CFG              (TRITON2_POWER_REG_OFFSET + 0x4A)
#define TRITON2_PB_MSB              (TRITON2_POWER_REG_OFFSET + 0x4B)
#define TRITON2_PB_LSB              (TRITON2_POWER_REG_OFFSET + 0x4C)

/* Triton2 Processor Groups. */
#define PROCESSOR_GRP1              (0x01) /* Applications devices - ex:- OMAP2430 */
#define PROCESSOR_GRP2              (0x02) /* Modem Devices - ex:- N3G */
#define PROCESSOR_GRP3              (0x04) /* Peripherals */

/* BCI tags */
#define TRITON2_BCI_CHARGER_SEL     0xC001
#define TRITON2_BCI_CHGMODE         0xC002   /* BCI charging mode*/
#define TRITON2_BCI_BATTERY_TYPE    0xC003
#define TRITON2_BCI_CONFIG          0xC004
#define TRITON2_BCI_CHARGE_DISABLE  0xC005
#define TRITON2_BCI_CHARGE_ENABLE   0xC006
#define TRITON2_BCI_VOLTAGE         0xC007
#define TRITON2_BCI_CURRENT         0xC008
#define TRITON2_BCI_TEMPERATURE     0xC009
#define TRITON2_BCI_MONITOR_TAG     0xC00A


#define BCIMDEN         0x74
#define BCIMDKEY        0x75
#define BCIMSTATEC      0x76
#define BCIMSTATEP      0x77
#define BCIVBAT1        0x78
#define BCIVBAT2        0x79
#define BCITBAT1        0x7A
#define BCITBAT2        0x7B
#define BCIICHG1        0x7C
#define BCIICHG2        0x7D
#define BCIVAC1         0x7E
#define BCIVAC2         0x7F
#define BCIVBUS1        0x80
#define BCIVBUS2        0x81
#define BCIMFSTS2       0x82
#define BCIMFSTS3       0x83
#define BCIMFSTS4       0x84
#define BCIMFKEY    0x85
#define BCIMFEN1    0x86
#define BCIMFEN2    0x87
#define BCIMFEN3    0x88
#define BCIMFEN4    0x89
#define BCIMFKEY    0x85
#define BCIMFEN1    0x86
#define BCIMFEN2    0x87
#define BCIMFEN3    0x88
#define BCIMFEN4    0x89
#define BCIMFTH1     0x8A
#define BCIMFTH2     0x8B
#define BCIMFTH3     0x8C
#define BCIMFTH4     0x8D
#define BCIMFTH5     0x8E
#define BCIMFTH6     0x8F
#define BCIMFTH7     0x90
#define BCIMFTH8     0x91
#define BCIMFTH9     0x92
#define BCITIMER1       0x93
#define BCITIMER2       0x94
#define BCIWDKEY        0x95
#define BCIWD           0x96
#define BCICTL1         0x97
#define BCICTL2         0x98
#define BCIVREF1        0x99
#define BCIVREF2        0x9A
#define BCIIREF1        0x9B
#define BCIIREF2        0x9C
#define BCIPWM2         0x9D
#define BCIPWM1         0x9E
#define BCITRIM1        0x9F
#define BCITRIM2        0xA0
#define BCITRIM3        0xA1
#define BCITRIM4        0xA2
#define BCIVREFCOMB1    0xA3
#define BCIVREFCOMB2    0xA4
#define BCIIREFCOMB1    0xA5
#define BCIIREFCOMB2    0xA6
#define BCIMNTEST1      0xA7
#define BCIMNTEST2      0xA8
#define BCIMNTEST3      0xA9
#define BCIPSTATE       0xAA
#define BCIMFSTS1       0xAB
#define BCITRIM5        0xAC



/* Triton2 LED register offsets. */
#define TRITON2_LED_REG_OFFSET      (0x00)

#define TRITON2_LED_LEDEN_REG       (TRITON2_LED_REG_OFFSET + 0xEE)
#define TRITON2_LED_PWMAON_REG      (TRITON2_LED_REG_OFFSET + 0xEF)
#define TRITON2_LED_PWMAOFF_REG     (TRITON2_LED_REG_OFFSET + 0xF0)

#define TRITON2_LED_PWMBON_REG      (TRITON2_LED_REG_OFFSET + 0xF1)
#define TRITON2_LED_PWMBOFF_REG     (TRITON2_LED_REG_OFFSET + 0xF2)


/* LEDEN Register Bits */
#define PWM_LENGTHB             0x80
#define PWM_LENGTHA             0x40
#define LEDBPWM                 0x20
#define LEDAPWM                 0x10
#define LEDBEXT                 0x08
#define LEDAEXT                 0x04
#define LEDBON                  0x02
#define LEDAON                  0x01
#define LEDOFF                  0x00

/* Triton2 Audio Resources register offsets. */
#define TRITON2_AUDIO_REG_OFFSET    (0x00)

/* Register offsets. */
#define TRITON2_CODEC_MODE_OFFSET           (TRITON2_AUDIO_REG_OFFSET + 0x01)
#define TRITON2_OPTION_OFFSET               (TRITON2_AUDIO_REG_OFFSET + 0x02)
#define TRITON2_MICBIAS_CTL_OFFSET          (TRITON2_AUDIO_REG_OFFSET + 0x04)
#define TRITON2_ANAMICL_OFFSET              (TRITON2_AUDIO_REG_OFFSET + 0x05)
#define TRITON2_ANAMICR_OFFSET              (TRITON2_AUDIO_REG_OFFSET + 0x06)
#define TRITON2_AVADC_CTL_OFFSET            (TRITON2_AUDIO_REG_OFFSET + 0x07)
#define TRITON2_ADCMICSEL_OFFSET            (TRITON2_AUDIO_REG_OFFSET + 0x08)
#define TRITON2_DIGMIXING_OFFSET            (TRITON2_AUDIO_REG_OFFSET + 0x09)
#define TRITON2_ATXL1PGA_OFFSET             (TRITON2_AUDIO_REG_OFFSET + 0x0A)
#define TRITON2_ATXR1PGA_OFFSET             (TRITON2_AUDIO_REG_OFFSET + 0x0B)
#define TRITON2_AVTXL2PGA_OFFSET            (TRITON2_AUDIO_REG_OFFSET + 0x0C)
#define TRITON2_AVTXR2PGA_OFFSET            (TRITON2_AUDIO_REG_OFFSET + 0x0D)
#define TRITON2_AUDIO_IF_OFFSET             (TRITON2_AUDIO_REG_OFFSET + 0x0E)
#define TRITON2_VOICE_IF_OFFSET             (TRITON2_AUDIO_REG_OFFSET + 0x0F)
#define TRITON2_ARXR1PGA_OFFSET             (TRITON2_AUDIO_REG_OFFSET + 0x10)
#define TRITON2_ARXL1PGA_OFFSET             (TRITON2_AUDIO_REG_OFFSET + 0x11)
#define TRITON2_ARXR2PGA_OFFSET             (TRITON2_AUDIO_REG_OFFSET + 0x12)
#define TRITON2_ARXL2PGA_OFFSET             (TRITON2_AUDIO_REG_OFFSET + 0x13)
#define TRITON2_VRXPGA_OFFSET               (TRITON2_AUDIO_REG_OFFSET + 0x14)
#define TRITON2_VSTPGA_OFFSET               (TRITON2_AUDIO_REG_OFFSET + 0x15)
#define TRITON2_VRX2ARXPGA_OFFSET           (TRITON2_AUDIO_REG_OFFSET + 0x16)
#define TRITON2_ADAC_CTL_OFFSET             (TRITON2_AUDIO_REG_OFFSET + 0x17)
#define TRITON2_ARX2VTXPGA_OFFSET           (TRITON2_AUDIO_REG_OFFSET + 0x18)
#define TRITON2_ARXL1_APGA_CTL_OFFSET       (TRITON2_AUDIO_REG_OFFSET + 0x19)
#define TRITON2_ARXR1_APGA_CTL_OFFSET       (TRITON2_AUDIO_REG_OFFSET + 0x1A)
#define TRITON2_ARXL2_APGA_CTL_OFFSET       (TRITON2_AUDIO_REG_OFFSET + 0x1B)
#define TRITON2_ARXR2_APGA_CTL_OFFSET       (TRITON2_AUDIO_REG_OFFSET + 0x1C)
#define TRITON2_BT_IF_OFFSET                (TRITON2_AUDIO_REG_OFFSET + 0x1E)
#define TRITON2_BTPGA_OFFSET                (TRITON2_AUDIO_REG_OFFSET + 0x1F)
#define TRITON2_BTSTPGA_OFFSET              (TRITON2_AUDIO_REG_OFFSET + 0x20)
#define TRITON2_EAR_CTL_OFFSET              (TRITON2_AUDIO_REG_OFFSET + 0x21)
#define TRITON2_HS_SEL_OFFSET               (TRITON2_AUDIO_REG_OFFSET + 0x22)
#define TRITON2_HS_GAIN_SET_OFFSET          (TRITON2_AUDIO_REG_OFFSET + 0x23)
#define TRITON2_HS_POPN_SET_OFFSET          (TRITON2_AUDIO_REG_OFFSET + 0x24)
#define TRITON2_PREDL_CTL_OFFSET            (TRITON2_AUDIO_REG_OFFSET + 0x25)
#define TRITON2_PREDR_CTL_OFFSET            (TRITON2_AUDIO_REG_OFFSET + 0x26)
#define TRITON2_PRECKL_CTL_OFFSET           (TRITON2_AUDIO_REG_OFFSET + 0x27)
#define TRITON2_PRECKR_CTL_OFFSET           (TRITON2_AUDIO_REG_OFFSET + 0x28)
#define TRITON2_HFL_CTL_OFFSET              (TRITON2_AUDIO_REG_OFFSET + 0x29)
#define TRITON2_HFR_CTL_OFFSET              (TRITON2_AUDIO_REG_OFFSET + 0x2A)
#define TRITON2_ALC_CTL_OFFSET              (TRITON2_AUDIO_REG_OFFSET + 0x2B)
#define TRITON2_ALC_SET1_OFFSET             (TRITON2_AUDIO_REG_OFFSET + 0x2C)
#define TRITON2_ALC_SET2_OFFSET             (TRITON2_AUDIO_REG_OFFSET + 0x2D)
#define TRITON2_BOOST_CTL_OFFSET            (TRITON2_AUDIO_REG_OFFSET + 0x2E)
#define TRITON2_SOFTVOL_CTL_OFFSET          (TRITON2_AUDIO_REG_OFFSET + 0x2F)
#define TRITON2_DTMF_FREQSEL_OFFSET         (TRITON2_AUDIO_REG_OFFSET + 0x30)
#define TRITON2_DTMF_TONEXT1H_OFFSET        (TRITON2_AUDIO_REG_OFFSET + 0x31)
#define TRITON2_DTMF_TONEXT1L_OFFSET        (TRITON2_AUDIO_REG_OFFSET + 0x32)
#define TRITON2_DTMF_TONEXT2H_OFFSET        (TRITON2_AUDIO_REG_OFFSET + 0x33)
#define TRITON2_DTMF_TONEXT2L_OFFSET        (TRITON2_AUDIO_REG_OFFSET + 0x34)
#define TRITON2_DTMF_TONOFF_OFFSET          (TRITON2_AUDIO_REG_OFFSET + 0x35)
#define TRITON2_DTMF_WANONOFF_OFFSET        (TRITON2_AUDIO_REG_OFFSET + 0x36)
#define TRITON2_I2S_DAC_SCRAMBLE_H_OFFSET   (TRITON2_AUDIO_REG_OFFSET + 0x37)
#define TRITON2_I2S_DAC_SCRAMBLE_M_OFFSET   (TRITON2_AUDIO_REG_OFFSET + 0x38)
#define TRITON2_I2S_DAC_SCRAMBLE_L_OFFSET   (TRITON2_AUDIO_REG_OFFSET + 0x39)
#define TRITON2_APLL_CTL_OFFSET             (TRITON2_AUDIO_REG_OFFSET + 0x3A)
#define TRITON2_DTMF_CTL_OFFSET             (TRITON2_AUDIO_REG_OFFSET + 0x3B)
#define TRITON2_DTMF_PGA_CTL2_OFFSET        (TRITON2_AUDIO_REG_OFFSET + 0x3C)
#define TRITON2_DTMF_PGA_CTL1_OFFSET        (TRITON2_AUDIO_REG_OFFSET + 0x3D)
#define TRITON2_MISC_SET_1_OFFSET           (TRITON2_AUDIO_REG_OFFSET + 0x3E)
#define TRITON2_PCMBTMUX_OFFSET             (TRITON2_AUDIO_REG_OFFSET + 0x3F)
#define TRITON2_RXPATHSEL_OFFSET            (TRITON2_AUDIO_REG_OFFSET + 0x43)
#define TRITON2_VDL_APGA_CTL_OFFSET         (TRITON2_AUDIO_REG_OFFSET + 0x44)
#define TRITON2_VIBRA_CTL_OFFSET            (TRITON2_AUDIO_REG_OFFSET + 0x45)
#define TRITON2_VIBRA_SET_OFFSET            (TRITON2_AUDIO_REG_OFFSET + 0x46)
#define TRITON2_VIBRA_PWM_SET_OFFSET        (TRITON2_AUDIO_REG_OFFSET + 0x47)
#define TRITON2_ANAMIC_GAIN_OFFSET          (TRITON2_AUDIO_REG_OFFSET + 0x48)
#define TRITON2_MISC_SET_2_OFFSET           (TRITON2_AUDIO_REG_OFFSET + 0x49)

struct bci_params
{
  F32 battery_type;     /* Battery type monitored by MADC */
  F32 battery_voltage;      /* Battery voltage monitored by MADC */
  F32 charging_current;     /* Battery charging current monitored by MADC */
  F32 ac_charger_voltage;   /* AC charger voltage monitored by MADC */
};

/* Mask and shift values used for getting CGAIN value from BCICTL1 */
#define MADC_10BIT_MSB_MASK         0x03
#define MADC_10BIT_MSB_SHIFT        0x08
#define BCICTL_CGAIN_MASK   0x20
#define BCICTL_CGAIN_SHIFT  0x05
#define AC_TYPE	1
#define USB_TYPE 2

/* MADC channels */
#define ADC_CHANNEL_0    0
#define ADC_CHANNEL_1    1
#define ADC_CHANNEL_2    2
#define ADC_CHANNEL_3    3
#define ADC_CHANNEL_4    4
#define ADC_CHANNEL_5    5
#define ADC_CHANNEL_6    6
#define ADC_CHANNEL_7    7
#define ADC_CHANNEL_8    8
#define ADC_CHANNEL_9    9
#define ADC_CHANNEL_10   10
#define ADC_CHANNEL_11   11
#define ADC_CHANNEL_12   12
#define ADC_CHANNEL_13   13
#define ADC_CHANNEL_14   14
#define ADC_CHANNEL_15   15

typedef enum
{
    tLeftLS = 0,
    tRightLS,
    tHeadSet,
    tSterioJack,
    tEarphone
} tOutputs;

/* Function Prototypes. */
U32 configure_triton2_power_resources (U32 power_res_tag, U8 res_voltage,
                                       U32 power_res_on_off_tag,
                                       U8 power_res_state);
U32 enable_triton2_vaux2 (void);
S32 configure_i2c_slave_id_for_triton2 (i2c_init_structure_t *i2c_init_str,
                                        U8 i2c_addr);
S32 store_i2c_id_for_triton2 (i2c_init_structure_t *i2c_init_str);
S32 configure_i2c_id_for_triton2_addr_group2 (i2c_init_structure_t *i2c_init_str);
S32 restore_i2c_id_for_triton2 (i2c_init_structure_t *i2c_init_str);
S32 audio_mute_control (U8 enable_disable);
S32 headset_detect (void);
S32 triton2_gpio_pin_init (U8 pin_num, U8 in_out);
S32 triton2_get_gpio_input (U16 pin_num, U8 *gpio_data);
S32 triton2_set_gpio_output (U16 pin_num, U8 set);
S32 triton2_gpio_pin_deinit (U16 pin_num);
S32 triton2_codec_init (triton_drv_struct_t* triton2_drv);
S32 triton2_codec_deinit (triton_drv_struct_t* triton2_drv);
void reset_triton2_codec (triton_drv_struct_t* triton2_drv);
S32 set_t2_analog_gain (triton_drv_struct_t* triton2_drv, U8 gain);
S32 triton2_SetVolume (triton_drv_struct_t* triton2_drv, U32 vol_level);
S32 av_enable_out_dev (triton_drv_struct_t* triton2_drv, tOutputs outdev);
S32 outdev_settings (triton_drv_struct_t* triton2_drv, tOutputs outdev);

// SSP
U32 triton2_fm_loop_init ( triton_drv_struct_t* triton2_drv );
U32 triton2_line_in_init ( triton_drv_struct_t* triton2_drv );
U32 triton2_fm_loop_deinit (triton_drv_struct_t* triton2_drv);

status_t bci_status_read (triton_drv_struct_t *triton2_drv, t2_tag_t tag,
                          U32 *size, U8 *buffer, struct bci_params *param);
#ifdef __cplusplus
}
#endif

#endif  /* __MS_DIAG_TRITON2_UTILS_H__ */
