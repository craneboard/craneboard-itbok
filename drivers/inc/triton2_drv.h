/*
 * triton2_drv.h: Triton2 driver definitions.
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

#ifndef __MS_DIAG_TRITON2_DRV_H__
#define __MS_DIAG_TRITON2_DRV_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "i2c_drv.h"


/* 
** Power Resource values. 
*/

/* VDD1 voltage values */
#define VDD1_1P3        56 /* VSEL = 56 to get the 1.3v (VDD1 = VSEL*12.5mV + 600mV) */

/* VDD2 voltage values */
#define VDD2_1P3        56 /* VSEL = 56 to get the 1.3v (VDD2 = VSEL*12.5mV + 600mV) */

/* VIO possible voltage values */
#define VIO_1P8     0x00
#define VIO_1P85    0x01

/* VDAC possible voltage values */
#define VDAC_1P2    0x00
#define VDAC_1P3    0x01
#define VDAC_1P8    0x02 /* or 0x03 */

/* VPLL1 possible voltage values */
#define VPLL1_1P0   0x00
#define VPLL1_1P2   0x01
#define VPLL1_1P3   0x02
#define VPLL1_1P8   0x03
#define VPLL1_2P8   0x04
#define VPLL1_3P0   0x05

/* VPLL2 possible voltage values */
#define VPLL2_0P7   0x00
#define VPLL2_1P0   0x01
#define VPLL2_1P2   0x02
#define VPLL2_1P3   0x03
#define VPLL2_1P5   0x04
#define VPLL2_1P8   0x05
#define VPLL2_1P85  0x06
#define VPLL2_2P5   0x07
#define VPLL2_2P6   0x08
#define VPLL2_2P8   0x09
#define VPLL2_2P85  0x0A
#define VPLL2_3P0   0x0B
#define VPLL2_3P15  0x0C

/* VMMC1 possible voltage values */
#define VMMC1_1P85  0x00
#define VMMC1_2P85  0x01
#define VMMC1_3P0   0x02
#define VMMC1_3P15  0x03

/* VMMC2 possible voltage values */
#define VMMC2_1P0   0x01
#define VMMC2_1P2   0x02
#define VMMC2_1P3   0x03
#define VMMC2_1P5   0x04
#define VMMC2_1P8   0x05
#define VMMC2_1P85  0x06
#define VMMC2_2P5   0x07
#define VMMC2_2P6   0x08
#define VMMC2_2P8   0x09
#define VMMC2_2P85  0x0A
#define VMMC2_3P0   0x0B
#define VMMC2_3P15  0x0C /* or 0x0D or 0x0E or 0x0F */

/* VSIM possible voltage values */
#define VSIM_1P0    0x00
#define VSIM_1P2    0x01
#define VSIM_1P3    0x02
#define VSIM_1P8    0x03
#define VSIM_2P8    0x04
#define VSIM_3P0    0x05 /* or 0x06 or 0x07 */

/* VAUX1 possible voltage values */
#define VAUX1_1P5   0x00
#define VAUX1_1P8   0x01
#define VAUX1_2P5   0x02
#define VAUX1_2P8   0x03
#define VAUX1_3P0   0x04  /* or 0x05 or 0x06 or 0x07 */

/* VAUX2 possible voltage values */
#define VAUX2_1P0   0x01
#define VAUX2_1P2   0x02
#define VAUX2_1P3   0x03
#define VAUX2_1P5   0x04
#define VAUX2_1P8   0x05
#define VAUX2_1P85  0x06
#define VAUX2_2P5   0x07
#define VAUX2_2P6   0x08
#define VAUX2_2P8   0x09
#define VAUX2_2P85  0x0A
#define VAUX2_3P0   0x0B
#define VAUX2_3P15  0x0C

/* VAUX3 possible voltage values */
#define VAUX3_1P5   0x00
#define VAUX3_1P8   0x01
#define VAUX3_2P5   0x02
#define VAUX3_2P8   0x03
#define VAUX3_3P0   0x04

/* VAUX4 possible voltage values */
#define VAUX4_0P7   0x00
#define VAUX4_1P0   0x01
#define VAUX4_1P2   0x02
#define VAUX4_1P3   0x03
#define VAUX4_1P5   0x04
#define VAUX4_1P8   0x05
#define VAUX4_1P85  0x06
#define VAUX4_2P5   0x07
#define VAUX4_2P6   0x08
#define VAUX4_2P8   0x09
#define VAUX4_2P85  0x0A
#define VAUX4_3P0   0x0B
#define VAUX4_3P15  0x0C

/* Triton2 Power Source IDs */
#define VAUX1_RES_ID 1
#define VAUX2_RES_ID 2
#define VAUX3_RES_ID 3
#define VAUX4_RES_ID 4
#define VMMC1_RES_ID 5
#define VMMC2_RES_ID 6
#define VPLL1_RES_ID 7
#define VPLL2_RES_ID 8
#define VSIM_RES_ID  9
#define VDAC_RES_ID  10
#define VIO_RES_ID   14
#define VDD1_RES_ID  15
#define VDD2_RES_ID  16

/* power on interrupt */
#define PWRON_INT  0x01

#define POWER_MANAGEMENT_DEDICATED_INT  0x20
#define USB_DEDICATED_INT               0x10
#define MADC_DEDICATED_INT              0x08
#define BCI_DEDICATED_INT               0x04
#define KEYPAD_DEDICATED_INT            0x02
#define GPIO_DEDICATED_INT              0x01

#define NUM_OF_BITS_IN_TRITON2_REG      0x8



/* FIXME-Needs to add Audio and other fiels to the structure. */ 
typedef struct
{
    U32 sampling_rate; 
    void (*t2pwron_irq_callback)(void); 
    
} triton2_init_structure_t;


typedef enum
{
    TRITON2_ID1 = 0x48, /* Genenarl purpose I2C bus addressing */
    TRITON2_ID2 = 0x49,
    TRITON2_ID3 = 0x4A,
    TRITON2_ID4 = 0x4B

}triton2_ids;

typedef struct
{
    U8 gpio_pin_num;
    U8 gpio_data_dir;
    U8 gpio_data;

} triton2_gpio_structure_t;

typedef struct
{
    U8 i2c_addr; /* Triton2 I2C group ID */
    U8 vdac;
    U8 vdac_stat;
    U8 vpll1;
    U8 vpll1_stat;
    U8 vpll2;
    U8 vpll2_stat;
    U8 vmmc1;
    U8 vmmc1_stat;
    U8 vmmc2;
    U8 vmmc2_stat;
    U8 vsim;
    U8 vsim_stat;
    U8 vmic1;
    U8 vmic1_stat;
    U8 vmic2;
    U8 vmic2_stat;
    U8 vhsmic;
    U8 vhsmic_stat;
    U8 vaux1;
    U8 vaux1_stat;
    U8 vaux2;
    U8 vaux2_stat;
    U8 vaux3;
    U8 vaux3_stat;
    U8 vaux4;
    U8 vaux4_stat;

} triton2_power_resources_t;


typedef struct
{
    U8 i2c_addr;                         
    U32 volume;                         
    U8 mic;                             
    U8 loudspeaker;                     
    U8 agc;                             
    U8 sidetone;                      
    U8 keyclick_freq;
    U8 dac;

} triton2_audio_drv_t;


typedef struct
{
    i2c_init_structure_t        i2c_init_str;
    triton2_init_structure_t    triton2_init_str;
    triton2_gpio_structure_t    trtn2_gpio;
    triton2_power_resources_t   triton2_power;
    triton2_audio_drv_t         triton2_audio;

} triton_drv_struct_t;


typedef enum
{
    TRITON2_VDAC_VOLTAGE_TAG,
    TRITON2_VDAC_ON_OFF_TAG,
    TRITON2_VPLL1_VOLTAGE_TAG,
    TRITON2_VPLL1_ON_OFF_TAG,
    TRITON2_VPLL2_VOLTAGE_TAG,
    TRITON2_VPLL2_ON_OFF_TAG,
    TRITON2_VMMC1_VOLTAGE_TAG,
    TRITON2_VMMC1_ON_OFF_TAG,
    TRITON2_VMMC2_VOLTAGE_TAG,
    TRITON2_VMMC2_ON_OFF_TAG,
    TRITON2_VSIM_VOLTAGE_TAG,
    TRITON2_VSIM1_VOLTAGE_TAG,
    TRITON2_VSIM2_VOLTAGE_TAG,
    TRITON2_VAUX1_ON_OFF_TAG,
    TRITON2_VAUX2_VOLTAGE_TAG,
    TRITON2_VAUX2_ON_OFF_TAG,
    TRITON2_VAUX3_VOLTAGE_TAG,
    TRITON2_VAUX3_ON_OFF_TAG,
    TRITON2_VAUX4_VOLTAGE_TAG,
    TRITON2_VAUX4_ON_OFF_TAG,
    TRITON2_VOLUME_TAG,
    TRITON2_LOUDSPEAKER_TAG,
    TRITON2_MIC_TAG,
    TRITON2_DAC_TAG,
    TRITON2_GPIO_DATA_DIR_TAG,
    TRITON2_GPIO_DATA_TAG,
    TRITON2_BCI_CHARGE_DISABLE,
    TRITON2_BCI_CHARGE_ENABLE,
    TRITON2_BCI_CONFIG,
    TRITON2_BCI_CHGMODE,
    TRITON2_PWRON_INT_MASK_TAG,
    TRITON2_LED_NUMBER_TAG,
    TRITON2_LED_BRIGHTNESS_TAG,
    TRITON2_LED_ENABLE_DISABLE_TAG    
} t2_tag_t;


/* Function Prototypes. */
__inline__ U32 get_processor_group (void);
void update_triton2_groupids (triton_drv_struct_t* triton2_drv);
S32 configure_i2c_id_for_power_reg (triton_drv_struct_t* triton2_drv);
S32 select_power_res_volt (triton_drv_struct_t *triton2_drv,
                           U8 res_vsel_reg_offset, U8 reg_data);
S32 select_power_res_processer_grp (triton_drv_struct_t *triton2_drv,
                                    U8 res_grp_reg_offset);
S32 triton2_i2c_write (triton_drv_struct_t* triton2_drv, U32 offset, U32 size, U8 data);
S32 configure_i2c_id_for_audio_reg (triton_drv_struct_t* triton2_drv);

status_t dg_triton2_init (triton_drv_struct_t *triton2_drv);
status_t dg_triton2_read (triton_drv_struct_t *triton2_drv, t2_tag_t tag,
                          U32 *size, U8 *buffer);
status_t dg_triton2_write (triton_drv_struct_t *triton2_drv, t2_tag_t tag,
                           U32 *size, U8 *buffer);
status_t dg_triton2_deinit (triton_drv_struct_t *triton2_drv);


#ifdef __cplusplus
}
#endif

#endif  /* __MS_DIAG_TRITON2_DRV_H__ */
