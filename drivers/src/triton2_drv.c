/*
 * triton2_drv.c: Triton2 driver.
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

/* includes */
#include <common.h>
#include "types.h"
#include "omap3530evm.h"
#include "triton2_utils.h"
#include "i2c_utils.h"
#include "i2c_drv.h"
#include "triton2_utils.h"
#include "triton2_drv.h"

#define MEASURE_VBUS_EN         0x04
#define MEASURE_VBAT_EN         0x02
#define MEASURE_VAC_EN          0x01

U8 active_int = 0;		/* Active interrupt. */

S32 send_singular_pb_message(triton_drv_struct_t *, U8, U8);

/* Returns processor resource groups. */
__inline__ U32 get_processor_group(void)
{
	return (PROCESSOR_GRP1);	/* OMAP 3530 is application processor group */
}

/* Function for writing to Triton2 registers. */
S32 triton2_i2c_write(triton_drv_struct_t * triton2_drv, U32 offset, U32 size, U8 data)
{
	S32 ret_status = SUCCESS;

	ret_status = dg_i2c_write(&(triton2_drv->i2c_init_str), I2C_OFFSET(offset), &size, &data);

	if (SUCCESS != ret_status) {
#ifdef OMAP3530_DEBUG
		DEBUG("%s:%d - Triton2: Codec init failed.\r\n", __FILE__, __LINE__);
#endif
	}

	return ret_status;
}

/* To update the I2C group Ids of Triton2. */
void update_triton2_groupids(triton_drv_struct_t * triton2_drv)
{
	/* Set the I2C group ID for accessing the Triton2 resources. */
	triton2_drv->triton2_power.i2c_addr = 0x4B;	/* For power resources */
	triton2_drv->triton2_audio.i2c_addr = 0x49;	/* For audio resources */
}

/* 
** Configure I2C driver to use the I2C group ID to access the 
** Power resource registers. 
*/
S32 configure_i2c_id_for_power_reg(triton_drv_struct_t * triton2_drv)
{
	S32 ret_status = SUCCESS;
	U32 size;
	U8 data;

	data = triton2_drv->triton2_power.i2c_addr;
	size = 1;

	/* Select the I2C group IDs before accessing the respective Triton2 modules. */
	ret_status = dg_i2c_write(&(triton2_drv->i2c_init_str), I2C_SLAVE_ADDR, &size, &data);

	if (SUCCESS != ret_status) {
#ifdef OMAP3530_DEBUG
		DEBUG("%s:%d - Triton2: I2C Slave Address change failed.\r\n", __FILE__, __LINE__);
#endif
	}

	return ret_status;
}

/*
** Configure I2C driver to use the I2C group ID to access the 
** Audio resource registers.
*/
S32 configure_i2c_id_for_audio_reg(triton_drv_struct_t * triton2_drv)
{
	S32 ret_status;
	U32 size;
	U8 data;

	data = triton2_drv->triton2_audio.i2c_addr;
	size = 1;

	/* Select the I2C group IDs before accessing the respective Triton2 modules. */
	ret_status = dg_i2c_write(&(triton2_drv->i2c_init_str), I2C_SLAVE_ADDR, &size, &data);

	if (SUCCESS != ret_status) {
#ifdef OMAP3530_DEBUG
		DEBUG("%s:%d - Triton2: I2C Slave Address change failed.\r\n", __FILE__, __LINE__);
#endif
	}

	return ret_status;
}

/* To configure the Power Resource VSEL values. */
S32 select_power_res_volt(triton_drv_struct_t * triton2_drv, U8 res_vsel_reg_offset, U8 reg_data)
{
	S32 ret_status = SUCCESS;
	U32 size;

	ret_status = configure_i2c_id_for_power_reg(triton2_drv);
	if (SUCCESS != ret_status) {
#ifdef OMAP3530_DEBUG
		DEBUG("%s:%d - Triton2: I2C Write failed.\r\n", __FILE__, __LINE__);
#endif
		return ret_status;
	}

	size = 1;
	ret_status = dg_i2c_write((&triton2_drv->i2c_init_str), I2C_OFFSET(res_vsel_reg_offset), &size, &reg_data);
	if (SUCCESS != ret_status) {
#ifdef OMAP3530_DEBUG
		DEBUG("%s:%d - Triton2: I2C Write failed.\r\n", __FILE__, __LINE__);
#endif
	}

	return ret_status;
}

/* To configure the Power Resource Group. */
S32 select_power_res_processer_grp(triton_drv_struct_t * triton2_drv, U8 res_grp_reg_offset)
{
	S32 ret_status = SUCCESS;
	U32 size;
	U8 res_group;

	size = 1;
	res_group = (get_processor_group() << 5);
	/* Select the power resource processor group (GRP). */
	ret_status = dg_i2c_write((&triton2_drv->i2c_init_str), I2C_OFFSET(res_grp_reg_offset), &size, &res_group);
	if (SUCCESS != ret_status) {
#ifdef OMAP3530_DEBUG
		DEBUG("%s:%d - Triton2: I2C Write failed.\r\n", __FILE__, __LINE__);
#endif
	}

	return ret_status;
}

/* Initialize the TRITON2 power module. */
status_t dg_triton2_init(triton_drv_struct_t * triton2_drv)
{
	S32 ret_status = SUCCESS;

	/* 
	 ** Initialize the I2C control interface to Triton2. 
	 ** On OMAP3530 EVM, Triton2 is connected to I2C2 of OMAP 3530. 
	 */
	triton2_drv->i2c_init_str.i2c_no = TRITON_I2C;
	triton2_drv->i2c_init_str.clock_speed = I2C_2P6M_CLK;

	/* Initilaize the I2C interface. */
	ret_status = dg_i2c_init((void *)&triton2_drv->i2c_init_str);

	if (SUCCESS != ret_status) {
#ifdef OMAP3530_DEBUG
		DEBUG("%s:%d - Triton2:I2C init failed.\r\n", __FILE__, __LINE__);
#endif
		return ret_status;
	}

	if (NULL != triton2_drv->triton2_init_str.t2pwron_irq_callback) {
		/* FIXME: Kept on hold, need to be coded.  
		   confgure_t2_pwrbutton_int((U32)triton2_drv);
		 */
	}

	/* Update the Triton2 group ids. */
	update_triton2_groupids(triton2_drv);
	if (triton2_drv->triton2_init_str.sampling_rate) {
		configure_i2c_id_for_audio_reg(triton2_drv);
		reset_triton2_codec(triton2_drv);
		triton2_codec_init(triton2_drv);
	}

	return ret_status;
}

static char *lut_bci_ch11[] = {
#include "lut_bci_ac_voltage.h"
};

static char *lut_bci_ch12[] = {
#include "lut_bci_voltage.h"
};

static char *lut_bci_ch10_s0[] = {
#include "lut_bci_current_slop0.h"
};

static char *lut_bci_ch10_s1[] = {
#include "lut_bci_current_slop1.h"
};

const F32 prescale_ratio[16] = { 1 /* MADC CH0 */ , 1 /* MADC CH1 */ , 0.6 /* MADC CH2  */ ,
	0.6 /* MADC CH3  */ , 0.6 /* MADC CH4  */ ,
	0.6 /* MADC CH5 */ , 0.6 /* MADC CH6 */ ,
	0.6 /* MADC CH7  */ , 3.0 / 14.0 /* MADC CH8 */ ,
	0.33 /* MADC CH9 */ , 1 /* MADC CH10 */ ,
	0.15 /* MADC CH11 */ , 0.25 /* MADC CH12  */ ,
	1 /* MADC CH13  */ , 1 /* MADC CH14  */ ,
	0.4			/* MADC CH15 */
};

#define CONV_TO_VOLTAGE(value, channel)    ( (((value) * 1.5) / (prescale_ratio[channel]))/1023)

F32 convert_to_current(U16 value, U8 cgain)
{
	U8 bufr[10];

	if (cgain == 0)
		strcpy((char *)&bufr, lut_bci_ch10_s0[value]);
	else if (cgain == 1)
		strcpy((char *)&bufr, lut_bci_ch10_s1[value]);

	printf("Battery charger current = %s\n", bufr);
	return 0;
}

void print_bci_status(U16 data, int channel_number)
{
	U8 bufr[10];

	if (channel_number == ADC_CHANNEL_12) {
		strcpy((char *)&bufr, lut_bci_ch12[data]);
		printf("Battery voltage = %s\n", bufr);
	} else if (channel_number == ADC_CHANNEL_11) {
		strcpy((char *)&bufr, lut_bci_ch11[data]);
		printf("ac charger voltage = %s\n", bufr);
	}

}

S32 configure_i2c_id_for_bci_reg(triton_drv_struct_t * triton2_drv)
{
	S32 ret_val;
	U32 length = 1;
	U8 slave_addr = 0x4A;

	ret_val = dg_i2c_write(&(triton2_drv->i2c_init_str), I2C_SLAVE_ADDR, &length, &slave_addr);
	if (ret_val != 0) {
		printf("Triton2:I2C Slave Address change failed");
		return ret_val;
	}
	return ret_val;
}

S32 configure_for_power_reg(triton_drv_struct_t * triton2_drv)
{
	S32 ret_status;
	U32 size;
	U8 data;

	data = 0x4B;		// power domain i2c address
	size = 1;

	/* Select the I2C group IDs before accessing the respective Triton2 modules */
	ret_status = dg_i2c_write(&(triton2_drv->i2c_init_str), I2C_SLAVE_ADDR, &size, &data);

	if (ret_status != 0) {
		printf("\r\nTriton2:I2C Slave Address change failed");
	}

	return (ret_status);
}

/* Read battery data from Triton2. */
status_t bci_status_read(triton_drv_struct_t * triton2_drv, t2_tag_t tag, U32 * size, U8 * buffer, struct bci_params * param)
{

	U8 data, cgain;
	U16 adc_data;
	U32 len = 1;
	S32 ret_status = SUCCESS;

	U32 length = 1;
	U8 slave_addr = 0x4A;

	//configure i2c for bci
	ret_status = dg_i2c_write(&(triton2_drv->i2c_init_str), I2C_SLAVE_ADDR, &length, &slave_addr);
	if (ret_status != SUCCESS) {
		printf("Triton2:I2C Slave Address change failed");
		return ret_status;
	}
	/* enable VBAT measure */
	data |= (MEASURE_VBAT_EN | MEASURE_VAC_EN | MEASURE_VBUS_EN);
	ret_status = dg_i2c_write(&(triton2_drv->i2c_init_str), I2C_OFFSET(BCICTL1), &len, &data);
	if (ret_status != SUCCESS) {
		printf("BCICTL1:I2C write failed");
		return ret_status;
	}

	/* Read the Triton2 register */
	*buffer = 0;
	ret_status = dg_i2c_read(&(triton2_drv->i2c_init_str), I2C_OFFSET(BCIMDEN), size, buffer);
	if (SUCCESS != ret_status) {
		printf("%s:%d - Triton2:I2C read failed.\r\n", __FILE__, __LINE__);
		return ret_status;
	}
	if (*buffer & 0x2) {
		printf("Charge Type : AC Charge \n");
		param->battery_type = AC_TYPE;
	} else if (*buffer & 0x1) {
		printf("Charge Type : USB Charge \n");
		param->battery_type = USB_TYPE;
	}
	/* read ac charging voltage */
	*buffer = 0;
	ret_status = dg_i2c_read(&(triton2_drv->i2c_init_str), I2C_OFFSET(BCIVAC1), size, buffer);
	if (SUCCESS != ret_status) {
		printf("%s:%d - Triton2:I2C read failed.\r\n", __FILE__, __LINE__);
		return ret_status;
	}
	adc_data = *buffer;

	*buffer = 0;
	ret_status = dg_i2c_read(&(triton2_drv->i2c_init_str), I2C_OFFSET(BCIVAC2), size, buffer);
	if (SUCCESS != ret_status) {
		printf("%s:%d - Triton2:I2C read failed.\r\n", __FILE__, __LINE__);
		return ret_status;
	}
	/* Forming the 10-bit ADC value */
	adc_data |= ((*buffer & MADC_10BIT_MSB_MASK) << MADC_10BIT_MSB_SHIFT);
	print_bci_status(adc_data, ADC_CHANNEL_11);

	/* read voltage */
	*buffer = 0;
	ret_status = dg_i2c_read(&(triton2_drv->i2c_init_str), I2C_OFFSET(BCIVBAT1), size, buffer);
	if (SUCCESS != ret_status) {
		printf("%s:%d - Triton2:I2C read failed.\r\n", __FILE__, __LINE__);
		return ret_status;
	}
	adc_data = *buffer;

	*buffer = 0;
	ret_status = dg_i2c_read(&(triton2_drv->i2c_init_str), I2C_OFFSET(BCIVBAT2), size, buffer);
	if (SUCCESS != ret_status) {
		printf("%s:%d - Triton2:I2C read failed.\r\n", __FILE__, __LINE__);
		return ret_status;
	}
	/* Forming the 10-bit ADC value */
	adc_data |= ((*buffer & MADC_10BIT_MSB_MASK) << MADC_10BIT_MSB_SHIFT);
	print_bci_status(adc_data, ADC_CHANNEL_12);

	/* read current */
	*buffer = 0;
	ret_status = dg_i2c_read(&(triton2_drv->i2c_init_str), I2C_OFFSET(BCIICHG1), size, buffer);
	if (SUCCESS != ret_status) {
		printf("%s:%d - Triton2:I2C read failed.\r\n", __FILE__, __LINE__);
		return ret_status;
	}
	adc_data = *buffer;

	*buffer = 0;
	ret_status = dg_i2c_read(&(triton2_drv->i2c_init_str), I2C_OFFSET(BCIICHG2), size, buffer);
	if (SUCCESS != ret_status) {
		printf("%s:%d - Triton2:I2C read failed.\r\n", __FILE__, __LINE__);
		return ret_status;
	}
	/* Forming the 10-bit ADC value */
	adc_data |= ((*buffer & MADC_10BIT_MSB_MASK) << MADC_10BIT_MSB_SHIFT);

	*buffer = 0;
	ret_status = dg_i2c_read(&(triton2_drv->i2c_init_str), I2C_OFFSET(BCICTL1), size, buffer);
	if (SUCCESS != ret_status) {
		printf("%s:%d - Triton2:I2C read failed.\r\n", __FILE__, __LINE__);
		return ret_status;
	}

	/* Retrive the CGAIN value */
	cgain = (data & BCICTL_CGAIN_MASK) >> BCICTL_CGAIN_SHIFT;
	param->charging_current = convert_to_current(adc_data, cgain);

	return SUCCESS;
}

/* Read data from Triton2. */
status_t dg_triton2_read(triton_drv_struct_t * triton2_drv, t2_tag_t tag, U32 * size, U8 * buffer)
{

	switch (tag) {

	default:
#ifdef OMAP3530_DEBUG
		DEBUG("dg_triton2_read - Other tags currently not implemented.\r\n");
#endif
		break;
	}

	return SUCCESS;
}

/* Write data to Triton2. */
status_t dg_triton2_write(triton_drv_struct_t * triton2_drv, t2_tag_t tag, U32 * size, U8 * buffer)
{
	S32 ret_status = SUCCESS;

	switch (tag) {
	case TRITON2_VDAC_VOLTAGE_TAG:

		/* Select the VDAC output voltage */
		ret_status = select_power_res_volt(triton2_drv, TRITON2_VDAC_DEDICATED_REG, *buffer);
		if (ret_status != SUCCESS) {
			/* Failure, return from the driver */
			break;
		}

		/* Select the VDAC Processor resource group */
		ret_status = select_power_res_processer_grp(triton2_drv, TRITON2_VDAC_DEV_GRP_REG);
		if (ret_status == SUCCESS) {
			/* Update the DIS member only if the value is written to the device */
			triton2_drv->triton2_power.vdac = *(U8 *) buffer;
		}
		break;

	case TRITON2_VDAC_ON_OFF_TAG:

		/* Send Power Bus message to activiate/deactivate the VDAC power resource */
		ret_status = send_singular_pb_message(triton2_drv, VDAC_RES_ID, *buffer);
		if (ret_status == SUCCESS) {
			/* Update the DIS member only if the value is written to the device */
			triton2_drv->triton2_power.vdac_stat = *buffer;
		}
		break;

	case TRITON2_VAUX2_VOLTAGE_TAG:
		/* Select the VAUX2 output voltage. */
		ret_status = select_power_res_volt(triton2_drv, TRITON2_VAUX2_DEDICATED_REG, *buffer);
		if (SUCCESS != ret_status) {
			/* Failure, return from the driver. */
			break;
		}

		/* Select the VAUX2 Processor resource group. */
		ret_status = select_power_res_processer_grp(triton2_drv, TRITON2_VAUX2_DEV_GRP_REG);
		if (SUCCESS != ret_status) {
			/* 
			 ** Update the DRV member only if the value 
			 ** is written to the device. 
			 */
			triton2_drv->triton2_power.vaux2 = *(U8 *) buffer;
		}
		break;

	case TRITON2_VOLUME_TAG:
		configure_i2c_id_for_audio_reg(triton2_drv);

		ret_status = triton2_SetVolume(triton2_drv, *(U32 *) buffer);
		if (SUCCESS != ret_status) {
#ifdef OMAP3530_DEBUG
			DEBUG("%s:%d - Triton2 : Set Volume failed.\r\n", __FILE__, __LINE__);
#endif
			return ret_status;
		}
		triton2_drv->triton2_audio.volume = *(U32 *) buffer;
		break;

	case TRITON2_LOUDSPEAKER_TAG:
		configure_i2c_id_for_audio_reg(triton2_drv);
		triton2_drv->triton2_audio.loudspeaker = *buffer;

		/*
		 ** We support only Sterio Jack.
		 ** So device settings only for that.
		 ** If more device needed give corresponding tag 
		 ** to "outdev_settings".
		 */

		/* Do the settings for particular output device */
		ret_status = outdev_settings(triton2_drv, tSterioJack);
		if (SUCCESS != ret_status) {
#ifdef OMAP3530_DEBUG
			DEBUG("%s:%d - Triton2 : Codec O/P device setting failed.\r\n", __FILE__, __LINE__);
#endif
			return ret_status;
		}
		break;

	default:
#ifdef OMAP3530_DEBUG
		DEBUG("dg_triton2_write - Other tags currently not " "implemented.\r\n");
#endif
		break;
	}

	return SUCCESS;
}

/* De-initialize the TRITON2 power module. */
status_t dg_triton2_deinit(triton_drv_struct_t * triton2_drv)
{
	S32 ret_status = SUCCESS;

	if (0 != triton2_drv) {
		if (triton2_drv->triton2_init_str.sampling_rate) {
			ret_status = triton2_codec_deinit(triton2_drv);
		}

		if (NULL != triton2_drv->triton2_init_str.t2pwron_irq_callback) {
			/*
			 ** FIXME: 
			 ** Kept on hold, need to be coded. 
			 ** triton2_remove_int_handler(active_int);
			 */
		}

		/* De-initialize the I2C used in T2. */
		ret_status = dg_i2c_deinit(&(triton2_drv->i2c_init_str));
	}

	return ret_status;
}

/*-----------------------------------------------------------------------------
| Function    : form_singular_power_bus_message()
+------------------------------------------------------------------------------
| Description : Configure I2C driver to use the I2C group ID to access the Power
|               resource registers.
|
| Parameters  : dev_grp     - Processor Group of the resource
|               res_id      - ID of the power resource
|               res_state   - State of the Power resource (OFF, ACTIVE, or SLEEP)
|
| Returns     : U16 - singular message
+-----------------------------------------------------------------------------*/
U16 form_singular_power_bus_message(U8 dev_grp, U8 res_id, U8 res_state)
{
	U16 singular_msg;
	/*  |DEV_GRP|0|ID|STATE| */
	/*   DEV_GRP[13:15],0,ID[4:11],and STATE[0:3] */
	singular_msg = ((dev_grp) << 13);	/* Resource group */
	singular_msg |= ((0) << 12);	/* Message type - Singular */
	singular_msg |= ((res_id) << 4);	/* Resource ID */
	singular_msg |= (res_state);	/* Resource State */
	return (singular_msg);
}

/*-----------------------------------------------------------------------------
| Function    : send_singular_pb_message()
+------------------------------------------------------------------------------
| Description : Function for sending the Power Bus singular message.
|
| Parameters  : Triton2 DIS pointer,Power resource ID,Power resource state
|
| Returns     : DAL Status
+-----------------------------------------------------------------------------*/
S32 send_singular_pb_message(triton_drv_struct_t * triton2_dis, U8 power_res_id, U8 res_state)
{

	S32 ret_status;
	U32 size;
	U8 data;
	U16 pb_message;

	ret_status = configure_i2c_id_for_power_reg(triton2_dis);
	if (ret_status != SUCCESS) {
		printf("Triton2:I2C Write failed\n");
		return (ret_status);
	}

	data = 0x02;		/* Enable I2C access to the Power Bus */
	size = 1;

	ret_status = dg_i2c_write(&(triton2_dis->i2c_init_str), I2C_OFFSET(TRITON2_PB_CFG), &size, &data);
	if (ret_status != SUCCESS) {
		/* Power bus access configuration failed */
		printf("Triton2:I2C Write failed\n");
		return (ret_status);
	}

	/* Form the message for VDAC */
	pb_message = form_singular_power_bus_message(get_processor_group(), power_res_id, res_state);

	/* Extract the Message MSB */
	data = (pb_message >> 8);
	size = 1;

	ret_status = dg_i2c_write(&(triton2_dis->i2c_init_str), I2C_OFFSET(TRITON2_PB_MSB), &size, &data);
	if (ret_status != SUCCESS) {
		/* Power bus access configuration failed */
		printf("Triton2:I2C Write failed\n");
		return (ret_status);
	}

	/* Extract the Message LSB */
	data = (pb_message & 0x00FF);
	size = 1;

	ret_status = dg_i2c_write(&(triton2_dis->i2c_init_str), I2C_OFFSET(TRITON2_PB_LSB), &size, &data);
	if (ret_status != SUCCESS) {
		printf("Triton2:I2C Write failed\n");
	}
	return (ret_status);
}
