/*
 * triton2_utils.c: Triton2 power resource configuration functions.
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

/* includes */
#include "types.h"
#include "omap3530evm.h"
#include "i2c_utils.h"
#include "i2c_drv.h"
#include "triton2_utils.h"
#include "triton2_drv.h"

U8 triton2_slave_addr = 0;
U8 prev_slave_addr = 0;

static U8 volume_set[21] = {
	0x93, 0x93, 0x93, 0x8B, 0x83, 0x7B, 0x73, 0x6B, 0x63, 0x5B,
	0x53, 0x4B, 0x43, 0x3B, 0x33, 0x2B, 0x23, 0x1B, 0x13, 0x0B,
	0x03
};

/* Function to read the battery charging status. */
int read_battery_status(void)
{
	U32 ret_val;
	U32 size;
	char val;
	struct bci_params params;

	triton_drv_struct_t triton2_drv;
	triton2_drv.triton2_init_str.sampling_rate = 0;
	triton2_drv.triton2_init_str.t2pwron_irq_callback = NULL;

	ret_val = dg_triton2_init(&triton2_drv);
	if (SUCCESS != ret_val) {
		printf("%s:%d - Triton2: Init failed.\r\n", __FILE__, __LINE__);
		return ret_val;
	}

	size = 1;
	triton2_drv.trtn2_gpio.gpio_pin_num = 0;
	triton2_drv.trtn2_gpio.gpio_data = 0;
	ret_val = bci_status_read(&triton2_drv, 0, &size, (U8 *) & val, &params);
	if (SUCCESS != ret_val) {
		printf("%s:%d - Triton2: Read failed.\r\n", __FILE__, __LINE__);
		ret_val = -1;
	}

	params.battery_voltage = 0;
	params.charging_current = 0;
	params.ac_charger_voltage = 0;
	ret_val = dg_triton2_deinit(&triton2_drv);
	if (SUCCESS != ret_val) {
		printf("%s:%d - Triton2: Deinit failed.\r\n", __FILE__, __LINE__);
		return ret_val;
	}
	return ret_val;
}

/* Common function for disabling and enabling the power resources. */
U32 configure_triton2_power_resources(U32 power_res_tag, U8 res_voltage, U32 power_res_on_off_tag, U8 power_res_state)
{
	U32 ret_val;
	U32 size;

	triton_drv_struct_t triton2_drv;

	/* Initialize the init structure */
	triton2_drv.triton2_init_str.sampling_rate = 0;
	triton2_drv.triton2_init_str.t2pwron_irq_callback = NULL;

	/* Initialize the Triton2 DIS */
	ret_val = dg_triton2_init(&triton2_drv);
	if (SUCCESS != ret_val) {
#ifdef OMAP3530_DEBUG
		DEBUG("%s:%d - Triton2: T2 init failed.\r\n", __FILE__, __LINE__);
#endif
		return ret_val;
	}

	/* No need to set the voltage if we want to turn the power off. */
	if (TRITON2_RES_ACTIVE == power_res_state) {
		size = 1;
		/* Configure the power resource voltage. */
		ret_val = dg_triton2_write(&triton2_drv, power_res_tag, &size, &res_voltage);

		if (SUCCESS != ret_val) {
#ifdef OMAP3530_DEBUG
			DEBUG("%s:%d - Triton2: T2 write failed.\r\n", __FILE__, __LINE__);
#endif
			/* Deinitialize the Triton2 . */
			ret_val = dg_triton2_deinit(&triton2_drv);
			if (SUCCESS != ret_val) {
#ifdef OMAP3530_DEBUG
				DEBUG("%s:%d - Triton2: T2 deinit failed.\r\n", __FILE__, __LINE__);
#endif
				return ret_val;
			}
			return ret_val;
		}
	}

	size = 1;
	/* Enable or Disable the power source. */
	ret_val = dg_triton2_write(&triton2_drv, power_res_on_off_tag, &size, &power_res_state);
	if (SUCCESS != ret_val) {
#ifdef OMAP3530_DEBUG
		DEBUG("%s:%d - Triton2: T2 write failed.\r\n", __FILE__, __LINE__);
#endif
		/* Deinitialize the Triton2. */
		ret_val = dg_triton2_deinit(&triton2_drv);
		if (SUCCESS != ret_val) {
#ifdef OMAP3530_DEBUG
			DEBUG("%s:%d - Triton2: T2 deinit failed.\r\n", __FILE__, __LINE__);
#endif
			return ret_val;
		}
		return (ret_val);
	}

	/* Deinitialize the Triton2. */
	ret_val = dg_triton2_deinit(&triton2_drv);
	if (SUCCESS != ret_val) {
#ifdef OMAP3530_DEBUG
		DEBUG("%s:%d - Triton2: T2 deinit failed.\r\n", __FILE__, __LINE__);
#endif
		return ret_val;
	}

	return ret_val;
}

/* Enable the Triton2 VAUX2 power resources for 3530 EVM. */
U32 enable_triton2_vaux2(void)
{
	U32 ret_val;

	/* Configure and enable VAUX2 for 2.8v. */
	ret_val = configure_triton2_power_resources(TRITON2_VAUX2_VOLTAGE_TAG, VAUX2_2P8, TRITON2_VAUX2_ON_OFF_TAG, TRITON2_RES_ACTIVE);
	if (SUCCESS != ret_val) {
#ifdef OMAP3530_DEBUG
		DEBUG("%s:%d - Triton2: VAUX2 configuration failed.\r\n", __FILE__, __LINE__);
#endif
	}

	return ret_val;
}

/* This function stores the present value of I2c slave address. */
S32 store_i2c_id_for_triton2(i2c_init_structure_t * i2c_init_str)
{
	S32 ret_val;
	U32 size;

	size = 1;

	ret_val = dg_i2c_read(i2c_init_str, I2C_SLAVE_ADDR, &size, &triton2_slave_addr);
	if (SUCCESS != ret_val) {
#ifdef OMAP3530_DEBUG
		DEBUG("%s:%d - Triton2: I2C Slave Address read failed.\r\n", __FILE__, __LINE__);
#endif
	}

	prev_slave_addr = triton2_slave_addr;
	return ret_val;
}

/* This function configures the i2c slave address. */
S32 configure_i2c_slave_id_for_triton2(i2c_init_structure_t * i2c_init_str, U8 i2c_addr)
{
	S32 ret_val = SUCCESS;
	U32 length = 1;

	ret_val = dg_i2c_write(i2c_init_str, I2C_SLAVE_ADDR, &length, &i2c_addr);
	if (SUCCESS != ret_val) {
#ifdef OMAP3530_DEBUG
		DEBUG("%s:%d - Triton2: I2C Slave Address change failed.\r\n", __FILE__, __LINE__);
#endif
	}

	return ret_val;
}

/* 
** Configure I2C driver to use the I2C group ID to access the 
** address group 2 registers.
*/
S32 configure_i2c_id_for_triton2_addr_group2(i2c_init_structure_t * i2c_init_str)
{
	S32 ret_val = SUCCESS;
	U32 size;
	U8 data;

	data = TRITON2_ID2;
	size = 1;

	ret_val = dg_i2c_write(i2c_init_str, I2C_SLAVE_ADDR, &size, &data);
	if (SUCCESS != ret_val) {
#ifdef OMAP3530_DEBUG
		DEBUG("%s:%d - Triton2: I2C Slave Address change failed.\r\n", __FILE__, __LINE__);
#endif
	}

	return ret_val;
}

/* This function restores the value of i2c slave address. */
S32 restore_i2c_id_for_triton2(i2c_init_structure_t * i2c_init_str)
{
	S32 ret_val = SUCCESS;
	U32 size;

	size = 1;

	ret_val = dg_i2c_write(i2c_init_str, I2C_SLAVE_ADDR, &size, &triton2_slave_addr);
	if (SUCCESS != ret_val) {
#ifdef OMAP3530_DEBUG
		DEBUG("%s:%d - Triton2: I2C Slave Address change failed.\r\n", __FILE__, __LINE__);
#endif
	}

	return ret_val;
}

#if 1

/* This function is used to enable/disable audio mute feature. */
S32 audio_mute_control(U8 enable_disable)
{
	U8 audio_mute_control_pin = TRITON2_GPIO_1;
	S32 ret_val = SUCCESS;

	if (TRITON2_AUDIO_MUTE_ENABLE == enable_disable) {
		/* Initialize the gpio pin as output. */
		ret_val = triton2_gpio_pin_init(audio_mute_control_pin, TRITON2_GPIO_OUTPUT);
		if (SUCCESS != ret_val) {
#ifdef OMAP3530_DEBUG
			DEBUG("%s:%d - Triton2: gpio init failed.\r\n", __FILE__, __LINE__);
#endif
			return ret_val;
		}

		/* Configure the enable_disable value to the gpio pin. */
		ret_val = triton2_set_gpio_output(audio_mute_control_pin, enable_disable);
		if (SUCCESS != ret_val) {
#ifdef OMAP3530_DEBUG
			DEBUG("%s:%d - Triton2: gpio set output failed.\r\n", __FILE__, __LINE__);
#endif
			/* Deinitialize the gpio pin settings and then return. */
			ret_val = triton2_gpio_pin_deinit(audio_mute_control_pin);
			if (SUCCESS != ret_val) {
#ifdef OMAP3530_DEBUG
				DEBUG("%s:%d - Triton2: gpio deinit failed.\r\n", __FILE__, __LINE__);
#endif
			}
			return ret_val;
		}
	} else {
		/*
		 ** To keep high state on the GPIO6 dont deinitialize 
		 ** when mute is enabled.
		 */
		ret_val = triton2_gpio_pin_deinit(audio_mute_control_pin);
		if (SUCCESS != ret_val) {
#ifdef OMAP3530_DEBUG
			DEBUG("%s:%d - Triton2: gpio deinit failed.\r\n", __FILE__, __LINE__);
#endif
		}
	}

	return ret_val;
}

#define OMAP3530_DEBUG   1

/* This function is used to enable/disable audio mute feature. */
S32 audio_mute_control1(U8 enable_disable)
{
	U8 audio_mute_control_pin = TRITON2_GPIO_6;
	S32 ret_val = SUCCESS;

	if (TRITON2_AUDIO_MUTE_DISABLE == enable_disable) {
		/* Initialize the gpio pin as output. */
		ret_val = triton2_gpio_pin_init(audio_mute_control_pin, TRITON2_GPIO_OUTPUT);
		if (SUCCESS != ret_val) {
#ifdef OMAP3530_DEBUG
			DEBUG("%s:%d - Triton2: gpio init failed.\r\n", __FILE__, __LINE__);
#endif
			return ret_val;
		}

		DEBUG("%s:%d - Triton2: gpio init failed.\r\n", __FILE__, __LINE__);
		/* Configure the enable_disable value to the gpio pin. */
		ret_val = triton2_set_gpio_output(audio_mute_control_pin, enable_disable);
		if (SUCCESS != ret_val) {
#ifdef OMAP3530_DEBUG
			DEBUG("%s:%d - Triton2: gpio set output failed.\r\n", __FILE__, __LINE__);
#endif
			/* Deinitialize the gpio pin settings and then return. */
			ret_val = triton2_gpio_pin_deinit(audio_mute_control_pin);
			if (SUCCESS != ret_val) {
#ifdef OMAP3530_DEBUG
				DEBUG("%s:%d - Triton2: gpio deinit failed.\r\n", __FILE__, __LINE__);
#endif
			}
			return ret_val;
		}
	} else {
		/*
		 ** To keep high state on the GPIO6 dont deinitialize 
		 ** when mute is enabled.
		 */
		ret_val = triton2_gpio_pin_deinit(audio_mute_control_pin);
		if (SUCCESS != ret_val) {
#ifdef OMAP3530_DEBUG
			DEBUG("%s:%d - Triton2: gpio deinit failed.\r\n", __FILE__, __LINE__);
#endif
		}
	}

	return ret_val;
}
#endif

/* This function is used to detect the presence of headset. */
S32 headset_detect(void)
{
	U8 headset_detector = TRITON2_GPIO_2;
	U8 detect_val;
	S32 ret_val = SUCCESS;

	/* Initialize the T2 GPIO2 as input for headset detect. */
	ret_val = triton2_gpio_pin_init(headset_detector, TRITON2_GPIO_INPUT);
	if (SUCCESS != ret_val) {
#ifdef OMAP3530_DEBUG
		DEBUG("%s:%d - Triton2: gpio init failed.\r\n", __FILE__, __LINE__);
#endif
	}

	/* Drive the headset mute GPIO. */
	ret_val = audio_mute_control(TRITON2_AUDIO_MUTE_ENABLE);
	if (SUCCESS != ret_val) {
#ifdef OMAP3530_DEBUG
		DEBUG("%s:%d - Triton2: Headset mute on failed.\r\n", __FILE__, __LINE__);
#endif
		return ret_val;
	}

	/*
	 ** Get the headset detect GPIO input value to see 
	 ** whether headset is present.
	 */
	ret_val = triton2_get_gpio_input(headset_detector, &detect_val);
	printf("detected headset value is %d\n ", detect_val);

	if (SUCCESS != ret_val) {
#ifdef OMAP3530_DEBUG
		DEBUG("%s:%d - Triton2: get gpio input failed.\r\n", __FILE__, __LINE__);
#endif
		/* Deinitialize the GPIO and then return. */
		ret_val = triton2_gpio_pin_deinit(headset_detector);
		if (SUCCESS != ret_val) {
#ifdef OMAP3530_DEBUG
			DEBUG("%s:%d - Triton2: gpio deinit failed.\r\n", __FILE__, __LINE__);
#endif
		}
		return FALSE;
	}

	/* Deinitialize the headset detect gpio settings. */
	ret_val = audio_mute_control(TRITON2_AUDIO_MUTE_DISABLE);
	if (SUCCESS != ret_val) {
#ifdef OMAP3530_DEBUG
		DEBUG("%s:%d - Triton2: Headset mute on failed.\r\n", __FILE__, __LINE__);
#endif
		return ret_val;
	}

	/* Deinitialize the headset mute GPIO6 settings */
	ret_val = triton2_gpio_pin_deinit(headset_detector);
	if (SUCCESS != ret_val) {
#ifdef OMAP3530_DEBUG
		DEBUG("%s:%d - Triton2: gpio deinit failed.\r\n", __FILE__, __LINE__);
#endif
		return ret_val;
	}

	/*
	 ** GPIO input(detect_val) is one means, headset is present. 
	 ** GPIO input(detect_val) is zero means, headset is not present.
	 */
	return ((1 == detect_val) ? TRUE : FALSE);
}

/* This function is to initialize the gpio pin as input or output. */
S32 triton2_gpio_pin_init(U8 pin_num, U8 in_out)
{
	S32 ret_val = SUCCESS;
	U32 size = 0;
	U8 deinit = 1;
	U8 failure = 0;
	triton_drv_struct_t triton2_drv;

	/* Initialize the init structure. */
	triton2_drv.triton2_init_str.sampling_rate = 0;
	triton2_drv.triton2_init_str.t2pwron_irq_callback = NULL;

	/* 
	 ** Validate the pin number.
	 ** There are 18 gpio pins. (0 - 17)
	 */
	if (pin_num > 17) {
#ifdef OMAP3530_DEBUG
		DEBUG("%s:%d - Triton2: Max number of gpio's = 18.\r\n", __FILE__, __LINE__);
#endif
	}

	/* Initialize the Triton2. */
	ret_val = dg_triton2_init(&triton2_drv);
	if (SUCCESS != ret_val) {
#ifdef OMAP3530_DEBUG
		DEBUG("%s:%d - Triton2: T2 init failed.\r\n", __FILE__, __LINE__);
#endif
		return ret_val;
	} else {
		deinit = 0;
	}

	/* Set the data direction of the triton2 gpio pin specified. */
	size = 1;
	triton2_drv.trtn2_gpio.gpio_pin_num = pin_num;
	triton2_drv.trtn2_gpio.gpio_data_dir = in_out;

	ret_val = dg_triton2_write(&triton2_drv, TRITON2_GPIO_DATA_DIR_TAG, &size, NULL);
	if (SUCCESS != ret_val) {
#ifdef OMAP3530_DEBUG
		DEBUG("%s:%d - Triton2: Write to GPIO failed.\r\n", __FILE__, __LINE__);
#endif
		failure = 1;
	}

	/* Deinitialize the Triton2. */
	if (1 == deinit) {
		ret_val = dg_triton2_deinit(&triton2_drv);
		if (SUCCESS != ret_val) {
#ifdef OMAP3530_DEBUG
			DEBUG("%s:%d - Triton2: Deinit failed.\r\n", __FILE__, __LINE__);
#endif
			failure = 1;
		}
	}

	return ((1 == failure) ? FAILURE : SUCCESS);
}

/* This function is for for getting data from GPIO pins. */
S32 triton2_get_gpio_input(U16 pin_num, U8 * gpio_data)
{
	S32 ret_val = SUCCESS;
	U32 size = 0;
	U8 deinit = 1;
	U8 failure = 0;
	triton_drv_struct_t triton2_drv;

	/* Initialize the init structure. */
	triton2_drv.triton2_init_str.sampling_rate = 0;
	triton2_drv.triton2_init_str.t2pwron_irq_callback = NULL;

	/*
	 ** Validate the pin number.
	 ** There are 18 gpio pins. (0 - 17)
	 */
	if (pin_num > 17) {
#ifdef OMAP3530_DEBUG
		DEBUG("%s:%d - Triton2: Max number of gpio's = 18.\r\n", __FILE__, __LINE__);
#endif
	}

	/* Initialize the Triton2. */
	ret_val = dg_triton2_init(&triton2_drv);
	if (SUCCESS != ret_val) {
#ifdef OMAP3530_DEBUG
		DEBUG("%s:%d - Triton2: T2 init failed.\r\n", __FILE__, __LINE__);
#endif
		return ret_val;
	} else {
		deinit = 0;
	}

	/* Set the data direction of the triton2 gpio pin specified. */
	size = 1;
	triton2_drv.trtn2_gpio.gpio_pin_num = pin_num;
	triton2_drv.trtn2_gpio.gpio_data = 0;

	ret_val = dg_triton2_read(&triton2_drv, TRITON2_GPIO_DATA_TAG, &size, NULL);
	if (SUCCESS != ret_val) {
#ifdef OMAP3530_DEBUG
		DEBUG("%s:%d - Triton2: Read from GPIO failed.\r\n", __FILE__, __LINE__);
#endif
		failure = 1;
	}

	/* 
	 ** Store the value in the variable specified by 
	 ** the user of this function.
	 */
	*gpio_data = triton2_drv.trtn2_gpio.gpio_data;

	/* Deinitialize the Triton2. */
	if (1 == deinit) {
		ret_val = dg_triton2_deinit(&triton2_drv);
		if (SUCCESS != ret_val) {
#ifdef OMAP3530_DEBUG
			DEBUG("%s:%d - Triton2: Deinit failed.\r\n", __FILE__, __LINE__);
#endif
			failure = 1;
		}
	}

	return ((1 == failure) ? FAILURE : SUCCESS);
}

/* This function is for writing data to the GPIO pins. */
S32 triton2_set_gpio_output(U16 pin_num, U8 set)
{
	S32 ret_val = SUCCESS;
	U32 size = 0;
	U8 deinit = 1;
	U8 failure = 0;
	triton_drv_struct_t triton2_drv;

	/* Initialize the init structure. */
	triton2_drv.triton2_init_str.sampling_rate = 0;
	triton2_drv.triton2_init_str.t2pwron_irq_callback = NULL;

	/*
	 ** Validate the pin number.
	 ** There are 18 gpio pins. (0 - 17)
	 */
	if (pin_num > 17) {
#ifdef OMAP3530_DEBUG
		DEBUG("%s:%d - Triton2: Max number of gpio's = 18.\r\n", __FILE__, __LINE__);
#endif
	}

	/* Initialize the Triton2. */
	ret_val = dg_triton2_init(&triton2_drv);
	if (SUCCESS != ret_val) {
#ifdef OMAP3530_DEBUG
		DEBUG("%s:%d - Triton2: T2 init failed.\r\n", __FILE__, __LINE__);
#endif
		return ret_val;
	} else {
		deinit = 0;
	}

	/* Set the data direction of the triton2 gpio pin specified. */
	size = 1;
	triton2_drv.trtn2_gpio.gpio_pin_num = pin_num;
	triton2_drv.trtn2_gpio.gpio_data = set;

	ret_val = dg_triton2_write(&triton2_drv, TRITON2_GPIO_DATA_TAG, &size, NULL);
	if (SUCCESS != ret_val) {
#ifdef OMAP3530_DEBUG
		DEBUG("%s:%d - Triton2: Write to GPIO failed.\r\n", __FILE__, __LINE__);
#endif
		failure = 1;
	}

	/* Deinitialize the Triton2. */
	if (1 == deinit) {
		ret_val = dg_triton2_deinit(&triton2_drv);
		if (SUCCESS != ret_val) {
#ifdef OMAP3530_DEBUG
			DEBUG("%s:%d - Triton2: Deinit failed.\r\n", __FILE__, __LINE__);
#endif
			failure = 1;
		}
	}

	return ((1 == failure) ? FAILURE : SUCCESS);
}

/*
** This function de initializes all the features associated with
** the pins and restores its default/reset state.
*/
S32 triton2_gpio_pin_deinit(U16 pin_num)
{
	S32 ret_val = SUCCESS;
	U32 size = 0;
	U8 deinit = 1;
	U8 failure = 0;
	triton_drv_struct_t triton2_drv;

	/* Initialize the init structure. */
	triton2_drv.triton2_init_str.sampling_rate = 0;
	triton2_drv.triton2_init_str.t2pwron_irq_callback = NULL;

	/*
	 ** Validate the pin number.
	 ** There are 18 gpio pins. (0 - 17)
	 */
	if (pin_num > 17) {
#ifdef OMAP3530_DEBUG
		DEBUG("%s:%d - Triton2: Max number of gpio's = 18.\r\n", __FILE__, __LINE__);
#endif
	}

	/* Initialize the Triton2. */
	ret_val = dg_triton2_init(&triton2_drv);
	if (SUCCESS != ret_val) {
#ifdef OMAP3530_DEBUG
		DEBUG("%s:%d - Triton2: T2 init failed.\r\n", __FILE__, __LINE__);
#endif
		return ret_val;
	} else {
		deinit = 0;
	}

	/* Set the data direction of the triton2 gpio pin specified. */
	size = 1;
	triton2_drv.trtn2_gpio.gpio_pin_num = pin_num;
	triton2_drv.trtn2_gpio.gpio_data_dir = TRITON2_GPIO_INPUT;

	ret_val = dg_triton2_write(&triton2_drv, TRITON2_GPIO_DATA_DIR_TAG, &size, NULL);
	if (SUCCESS != ret_val) {
#ifdef OMAP3530_DEBUG
		DEBUG("%s:%d - Triton2: Write to GPIO failed.\r\n", __FILE__, __LINE__);
#endif
		failure = 1;
	}

	/* Deinitialize the Triton2. */
	if (1 == deinit) {
		ret_val = dg_triton2_deinit(&triton2_drv);
		if (SUCCESS != ret_val) {
#ifdef OMAP3530_DEBUG
			DEBUG("%s:%d - Triton2: Deinit failed.\r\n", __FILE__, __LINE__);
#endif
			failure = 1;
		}
	}

	return ((1 == failure) ? FAILURE : SUCCESS);
}

/* 
	Basic initilizations for triton2 audio codec
    auxilliary/FM line input selection 
 */
U32 triton2_line_in_init(triton_drv_struct_t * triton2_drv)
{
	S32 ret_status = SUCCESS;
	U32 size;
	U8 data;

	size = 1;
	data = 0x93;		//44.1KHz sampling
	ret_status = dg_i2c_write(&(triton2_drv->i2c_init_str), I2C_OFFSET(TRITON2_CODEC_MODE_OFFSET), &size, &data);

	/* The active channel is TxPath channel 1, both Left and right */
	size = 1;
	data = 0xC3;
	ret_status = dg_i2c_write(&(triton2_drv->i2c_init_str), I2C_OFFSET(TRITON2_OPTION_OFFSET), &size, &data);

	/* Select AUXILLIARY line inputs for codec (no MIC inputs) */
	size = 1;
	data = 0x14;		/* select auxilliary inputs for audio in Left */

	ret_status = dg_i2c_write(&(triton2_drv->i2c_init_str), I2C_OFFSET(TRITON2_ANAMICL_OFFSET), &size, &data);
	size = 1;
	data = 0x14;		/* select auxilliary inputs for audio in Right */
	ret_status = dg_i2c_write(&(triton2_drv->i2c_init_str), I2C_OFFSET(TRITON2_ANAMICR_OFFSET), &size, &data);

	/* Enable both the ADC's */
	size = 1;
	data = 0x0A;
	ret_status = dg_i2c_write(&(triton2_drv->i2c_init_str), I2C_OFFSET(TRITON2_AVADC_CTL_OFFSET), &size, &data);

#ifdef INPUT_MAIN_MIC
	size = 1;
	data = 0x93;		//44.1KHz sampling
	ret_status = dg_i2c_write(&(triton2_drv->i2c_init_str), I2C_OFFSET(TRITON2_CODEC_MODE_OFFSET), &size, &data);

	/* The active channel is TxPath channel 1 - left channel */
	triton2_i2c_write(triton2_drv, TRITON2_OPTION_OFFSET, size, 0x01);

	/* Enable the main MIC and select ADCL input */
	triton2_i2c_write(triton2_drv, TRITON2_ANAMICL_OFFSET, size, 0x11);

	/* Set the MICBIAS for analog MIC and enable it */
	triton2_i2c_write(triton2_drv, TRITON2_MICBIAS_CTL_OFFSET, size, 0x01);

	/* Disable ANAMICR */
	triton2_i2c_write(triton2_drv, TRITON2_ANAMICR_OFFSET, size, 0x00);

	/* Enable Left ADC */
	triton2_i2c_write(triton2_drv, TRITON2_AVADC_CTL_OFFSET, size, 0x08);

	/* Set off digital mics and select ADC o/p as i/p to Tx 1 */
	triton2_i2c_write(triton2_drv, TRITON2_ADCMICSEL_OFFSET, size, 0x00);

	/* Configure 0dB gain on input */
	triton2_i2c_write(triton2_drv, TRITON2_ANAMIC_GAIN_OFFSET, size, 0x04);

	/* Set Tx path volume control as 0dB */
	triton2_i2c_write(triton2_drv, TRITON2_ATXL1PGA_OFFSET, size, 0x14);
	triton2_i2c_write(triton2_drv, TRITON2_ATXR1PGA_OFFSET, size, 0x00);
	triton2_i2c_write(triton2_drv, TRITON2_AVTXL2PGA_OFFSET, size, 0x00);
	triton2_i2c_write(triton2_drv, TRITON2_AVTXR2PGA_OFFSET, size, 0x00);

	/* Here we enable the mixing of L2 and R2, this is for playing the recorded mono
	   to headset */
	triton2_i2c_write(triton2_drv, TRITON2_RXPATHSEL_OFFSET, size, 0x30);
#endif

#ifdef INPUT_SUB_MIC
	size = 1;
	data = 0x93;		//44.1KHz sampling
	ret_status = dg_i2c_write(&(triton2_drv->i2c_init_str), I2C_OFFSET(TRITON2_CODEC_MODE_OFFSET), &size, &data);

	/* The active channel is TxPath channel 1, only Right channel */
	triton2_i2c_write(triton2_drv, TRITON2_OPTION_OFFSET, size, 0x02);

	/* Set the MICBIAS for analog Sub MIC and enable it */
	triton2_i2c_write(triton2_drv, TRITON2_MICBIAS_CTL_OFFSET, size, 0x02);

	/* SubMIC is connected to Right channel, no need to left channel */
	triton2_i2c_write(triton2_drv, TRITON2_ANAMICL_OFFSET, size, 0x00);

	/* Enable ANAMICR */
	triton2_i2c_write(triton2_drv, TRITON2_ANAMICR_OFFSET, size, 0x11);

	/* Enable the right ADC */
	triton2_i2c_write(triton2_drv, TRITON2_AVADC_CTL_OFFSET, size, 0x02);

	/* Set off digital mics and select ADC o/p as i/p to Tx 1 */
	triton2_i2c_write(triton2_drv, TRITON2_ADCMICSEL_OFFSET, size, 0x00);

	/* Configure 0dB gain on input */
	triton2_i2c_write(triton2_drv, TRITON2_ANAMIC_GAIN_OFFSET, size, 0x20);

	/* Set Tx path volume control as 0dB */
	triton2_i2c_write(triton2_drv, TRITON2_ATXL1PGA_OFFSET, size, 0x00);
	triton2_i2c_write(triton2_drv, TRITON2_ATXR1PGA_OFFSET, size, 0x0A);
	triton2_i2c_write(triton2_drv, TRITON2_AVTXL2PGA_OFFSET, size, 0x00);
	triton2_i2c_write(triton2_drv, TRITON2_AVTXR2PGA_OFFSET, size, 0x00);

	/* Here we enable the mixing of L2 and R2, this is for playing the 
	   recorded mono to headset */
	triton2_i2c_write(triton2_drv, TRITON2_RXPATHSEL_OFFSET, size, 0x30);
#endif

#ifdef INPUT_HEAD_SET_MIC
	size = 1;
	data = 0x93;		//44.1KHz sampling
	ret_status = dg_i2c_write(&(triton2_drv->i2c_init_str), I2C_OFFSET(TRITON2_CODEC_MODE_OFFSET), &size, &data);

	/* The active channel is TxPath channel 1, both Left and right */
	size = 1;
	data = 0xC3;
	ret_status = dg_i2c_write(&(triton2_drv->i2c_init_str), I2C_OFFSET(TRITON2_OPTION_OFFSET), &size, &data);
	/* Set the MICBIAS for analog MIC and enable it */
	size = 1;
	data = 0x04;
	ret_status = dg_i2c_write(&(triton2_drv->i2c_init_str), I2C_OFFSET(TRITON2_MICBIAS_CTL_OFFSET), &size, &data);

	/* Select MIC inputs for codec  */
	size = 1;
	data = 0x12;		/* select mic for audio in Left */
	ret_status = dg_i2c_write(&(triton2_drv->i2c_init_str), I2C_OFFSET(TRITON2_ANAMICL_OFFSET), &size, &data);
	size = 1;
	data = 0x00;
	ret_status = dg_i2c_write(&(triton2_drv->i2c_init_str), I2C_OFFSET(TRITON2_ANAMICR_OFFSET), &size, &data);

	/* Enable both the ADC's */
	size = 1;
	data = 0x0A;
	ret_status = dg_i2c_write(&(triton2_drv->i2c_init_str), I2C_OFFSET(TRITON2_AVADC_CTL_OFFSET), &size, &data);
	size = 1;
	data = 0x04;		// gain set 
	ret_status = dg_i2c_write(&(triton2_drv->i2c_init_str), I2C_OFFSET(TRITON2_ANAMIC_GAIN_OFFSET), &size, &data);

	data = 0x00;
	/* Set off digital mics and select ADC o/p as i/p to Tx 1 */
	ret_status = dg_i2c_write(&(triton2_drv->i2c_init_str), I2C_OFFSET(TRITON2_ADCMICSEL_OFFSET), &size, &data);
	/* Set Tx path volume control as 0dB */
	data = 0x0A;
	ret_status = dg_i2c_write(&(triton2_drv->i2c_init_str), I2C_OFFSET(TRITON2_ATXL1PGA_OFFSET), &size, &data);
	data = 0x00;
	ret_status = dg_i2c_write(&(triton2_drv->i2c_init_str), I2C_OFFSET(TRITON2_ATXR1PGA_OFFSET), &size, &data);
	data = 0x00;
	ret_status = dg_i2c_write(&(triton2_drv->i2c_init_str), I2C_OFFSET(TRITON2_AVTXL2PGA_OFFSET), &size, &data);
	data = 0x00;
	ret_status = dg_i2c_write(&(triton2_drv->i2c_init_str), I2C_OFFSET(TRITON2_AVTXR2PGA_OFFSET), &size, &data);

	/* Here we enable the mixing of L2 and R2, this is for playing the recorded mono
	   to headset */
	data = 0x30;
	ret_status = dg_i2c_write(&(triton2_drv->i2c_init_str), I2C_OFFSET(TRITON2_RXPATHSEL_OFFSET), &size, &data);
#endif

	return (ret_status);
}

/* SSP - auxilliary/FM line loop back enable */
U32 triton2_fm_loop_init(triton_drv_struct_t * triton2_drv)
{
	S32 ret_status = SUCCESS;
	U32 size;
	U8 data;

	// FM loop back
	size = 1;
	data = 0x35;
	ret_status = dg_i2c_write(&(triton2_drv->i2c_init_str), I2C_OFFSET(TRITON2_ARXL2_APGA_CTL_OFFSET), &size, &data);
	ret_status = dg_i2c_write(&(triton2_drv->i2c_init_str), I2C_OFFSET(TRITON2_ARXR2_APGA_CTL_OFFSET), &size, &data);

	// enable FM loopback
	data = 0x22;
	ret_status = dg_i2c_write(&(triton2_drv->i2c_init_str), I2C_OFFSET(TRITON2_MISC_SET_1_OFFSET), &size, &data);

	// Head set select and gain
	data = 0x24;
	ret_status = dg_i2c_write(&(triton2_drv->i2c_init_str), I2C_OFFSET(TRITON2_HS_SEL_OFFSET), &size, &data);

	data = 0x0A;
	ret_status = dg_i2c_write(&(triton2_drv->i2c_init_str), I2C_OFFSET(TRITON2_HS_GAIN_SET_OFFSET), &size, &data);
	return ret_status;
}

/* SSP - auxilliary/FM line loop back disnable */
U32 triton2_fm_loop_deinit(triton_drv_struct_t * triton2_drv)
{
	S32 ret_status = SUCCESS;
	U32 size;
	U8 data;

	size = 1;

	data = 0x31;
	ret_status = dg_i2c_write(&(triton2_drv->i2c_init_str), I2C_OFFSET(TRITON2_ARXL2_APGA_CTL_OFFSET), &size, &data);
	if (SUCCESS != ret_status)
		return FAILURE;

	ret_status = dg_i2c_write(&(triton2_drv->i2c_init_str), I2C_OFFSET(TRITON2_ARXR2_APGA_CTL_OFFSET), &size, &data);
	if (SUCCESS != ret_status)
		return FAILURE;

	data = 0x00;
	ret_status = dg_i2c_write(&(triton2_drv->i2c_init_str), I2C_OFFSET(TRITON2_MISC_SET_1_OFFSET), &size, &data);
	if (SUCCESS != ret_status)
		return FAILURE;

	return SUCCESS;
}

/* Basic initilizations for triton2 audio codec. */
S32 triton2_codec_init(triton_drv_struct_t * triton2_drv)
{
	S32 ret_status = SUCCESS;
	U32 size;
	U8 data;

	configure_i2c_id_for_audio_reg(triton2_drv);

	/*
	 ** Power on the codec with the sampling rate 44.1KHz
	 ** And Option-1 for audio selection.
	 */
	size = 1;
	data = 0x93;

	ret_status = dg_i2c_write(&(triton2_drv->i2c_init_str), I2C_OFFSET(TRITON2_CODEC_MODE_OFFSET), &size, &data);
	if (SUCCESS != ret_status) {
#ifdef OMAP3530_DEBUG
		DEBUG("%s:%d - Triton2: Codec init failed.\r\n", __FILE__, __LINE__);
#endif
	}

	/* Input clock freq - 26Mhz and APLL is enabled. */
	data = 0x16;
	ret_status = dg_i2c_write(&(triton2_drv->i2c_init_str), I2C_OFFSET(TRITON2_APLL_CTL_OFFSET), &size, &data);
	if (SUCCESS != ret_status) {
#ifdef OMAP3530_DEBUG
		DEBUG("%s:%d - Triton2: Codec init failed.\r\n", __FILE__, __LINE__);
#endif
	}

	/*
	 ** T2 I2S master (default settings),
	 ** 16 bit sample and word len, I2S mode data format,
	 ** Application mode for audio interface output pins.
	 */
	data = 0x01;
	ret_status = dg_i2c_write(&(triton2_drv->i2c_init_str), I2C_OFFSET(TRITON2_AUDIO_IF_OFFSET), &size, &data);
	if (SUCCESS != ret_status) {
#ifdef OMAP3530_DEBUG
		DEBUG("%s:%d - Triton2: Codec init failed.\r\n", __FILE__, __LINE__);
#endif
	}

	ret_status = triton2_line_in_init(triton2_drv);

	return ret_status;
}

/* Diable the T2 CODEC interface and shutdown the CODEC. */
S32 triton2_codec_deinit(triton_drv_struct_t * triton2_drv)
{
	S32 ret_status = SUCCESS;
	U32 size;
	U8 data;

	/* Power off codec. */
	size = 1;
	data = 0x00;

	ret_status = dg_i2c_write(&(triton2_drv->i2c_init_str), I2C_OFFSET(TRITON2_CODEC_MODE_OFFSET), &size, &data);
	if (SUCCESS != ret_status) {
#ifdef OMAP3530_DEBUG
		DEBUG("%s:%d - Triton2: Codec deinit failed.\r\n", __FILE__, __LINE__);
#endif
	}

	return ret_status;
}

/* Reset the Triton2 CODEC registers. */
void reset_triton2_codec(triton_drv_struct_t * triton2_drv)
{
	U32 size = 1;
	U8 data = 0x00;
	configure_i2c_id_for_audio_reg(triton2_drv);

	triton2_i2c_write(triton2_drv, TRITON2_ATXL1PGA_OFFSET, size, data);
	triton2_i2c_write(triton2_drv, TRITON2_ATXR1PGA_OFFSET, size, data);
	triton2_i2c_write(triton2_drv, TRITON2_ARXR1PGA_OFFSET, size, data);
	triton2_i2c_write(triton2_drv, TRITON2_ARXL1PGA_OFFSET, size, data);
	triton2_i2c_write(triton2_drv, TRITON2_ARXR2PGA_OFFSET, size, data);
	triton2_i2c_write(triton2_drv, TRITON2_ARXL2PGA_OFFSET, size, data);
	triton2_i2c_write(triton2_drv, TRITON2_AVTXL2PGA_OFFSET, size, data);
	triton2_i2c_write(triton2_drv, TRITON2_AVTXR2PGA_OFFSET, size, data);
	triton2_i2c_write(triton2_drv, TRITON2_VRXPGA_OFFSET, size, data);
	triton2_i2c_write(triton2_drv, TRITON2_ARXL1_APGA_CTL_OFFSET, size, data);
	triton2_i2c_write(triton2_drv, TRITON2_ARXR1_APGA_CTL_OFFSET, size, data);
	triton2_i2c_write(triton2_drv, TRITON2_ARXL2_APGA_CTL_OFFSET, size, data);
	triton2_i2c_write(triton2_drv, TRITON2_ARXR2_APGA_CTL_OFFSET, size, data);
	triton2_i2c_write(triton2_drv, TRITON2_BTPGA_OFFSET, size, data);
	triton2_i2c_write(triton2_drv, TRITON2_ALC_CTL_OFFSET, size, data);
	triton2_i2c_write(triton2_drv, TRITON2_DTMF_FREQSEL_OFFSET, size, data);
	triton2_i2c_write(triton2_drv, TRITON2_DTMF_TONOFF_OFFSET, size, data);
	triton2_i2c_write(triton2_drv, TRITON2_DTMF_WANONOFF_OFFSET, size, data);
	triton2_i2c_write(triton2_drv, TRITON2_APLL_CTL_OFFSET, size, data);
	triton2_i2c_write(triton2_drv, TRITON2_DTMF_PGA_CTL2_OFFSET, size, data);
	triton2_i2c_write(triton2_drv, TRITON2_DTMF_PGA_CTL1_OFFSET, size, data);
	triton2_i2c_write(triton2_drv, TRITON2_VDL_APGA_CTL_OFFSET, size, data);
	triton2_i2c_write(triton2_drv, TRITON2_RXPATHSEL_OFFSET, size, data);
}

S32 set_t2_analog_gain(triton_drv_struct_t * triton2_drv, U8 gain)
{
	S32 ret_status = SUCCESS;
	U32 size = 1;
	U8 setvol;

	setvol = gain;

	triton2_i2c_write(triton2_drv, TRITON2_ARXL1_APGA_CTL_OFFSET, size, setvol);
	triton2_i2c_write(triton2_drv, TRITON2_ARXR1_APGA_CTL_OFFSET, size, setvol);
	triton2_i2c_write(triton2_drv, TRITON2_ARXL2_APGA_CTL_OFFSET, size, setvol);
	triton2_i2c_write(triton2_drv, TRITON2_ARXR2_APGA_CTL_OFFSET, size, setvol);

	return ret_status;
}

S32 triton2_SetVolume(triton_drv_struct_t * triton2_drv, U32 vol_level)
{
	return (set_t2_analog_gain(triton2_drv, volume_set[vol_level]));
}

/* Enable Triton2 CODEC analog output devices. */
S32 av_enable_out_dev(triton_drv_struct_t * triton2_drv, tOutputs outdev)
{
	U32 size = 1;
	switch (outdev) {
	case tSterioJack:
		/* Enable  only headset R2 and L2. */
		triton2_i2c_write(triton2_drv, TRITON2_EAR_CTL_OFFSET, size, 0x00);
		triton2_i2c_write(triton2_drv, TRITON2_HS_SEL_OFFSET, size, 0x24);
		triton2_i2c_write(triton2_drv, TRITON2_HS_GAIN_SET_OFFSET, size, 0x0A);
		triton2_i2c_write(triton2_drv, TRITON2_PREDL_CTL_OFFSET, size, 0x00);
		triton2_i2c_write(triton2_drv, TRITON2_PREDR_CTL_OFFSET, size, 0x00);
		triton2_i2c_write(triton2_drv, TRITON2_PRECKL_CTL_OFFSET, size, 0x00);
		triton2_i2c_write(triton2_drv, TRITON2_PRECKR_CTL_OFFSET, size, 0x00);
		triton2_i2c_write(triton2_drv, TRITON2_HFL_CTL_OFFSET, size, 0x00);
		triton2_i2c_write(triton2_drv, TRITON2_HFR_CTL_OFFSET, size, 0x00);
		triton2_i2c_write(triton2_drv, TRITON2_HS_POPN_SET_OFFSET, size, 0x40);
		triton2_i2c_write(triton2_drv, TRITON2_HS_POPN_SET_OFFSET, size, 0x42);
		break;

	case tLeftLS:
		/* Enable only Left handsfree */
		triton2_i2c_write(triton2_drv, TRITON2_EAR_CTL_OFFSET, size, 0x00);
		triton2_i2c_write(triton2_drv, TRITON2_HS_SEL_OFFSET, size, 0x00);
		triton2_i2c_write(triton2_drv, TRITON2_HS_GAIN_SET_OFFSET, size, 0x00);
		triton2_i2c_write(triton2_drv, TRITON2_PREDL_CTL_OFFSET, size, 0x00);
		triton2_i2c_write(triton2_drv, TRITON2_PREDR_CTL_OFFSET, size, 0x00);
		triton2_i2c_write(triton2_drv, TRITON2_PRECKL_CTL_OFFSET, size, 0x00);
		triton2_i2c_write(triton2_drv, TRITON2_PRECKR_CTL_OFFSET, size, 0x00);
		triton2_i2c_write(triton2_drv, TRITON2_HFL_CTL_OFFSET, size, 0x20);
		triton2_i2c_write(triton2_drv, TRITON2_HFL_CTL_OFFSET, size, 0x30);
		triton2_i2c_write(triton2_drv, TRITON2_HFL_CTL_OFFSET, size, 0x3C);
		triton2_i2c_write(triton2_drv, TRITON2_HFL_CTL_OFFSET, size, 0x3E);
		break;
	case tRightLS:
		/* Enable only Right handsfree */
		triton2_i2c_write(triton2_drv, TRITON2_EAR_CTL_OFFSET, size, 0x00);
		triton2_i2c_write(triton2_drv, TRITON2_HS_SEL_OFFSET, size, 0x00);
		triton2_i2c_write(triton2_drv, TRITON2_HS_GAIN_SET_OFFSET, size, 0x00);
		triton2_i2c_write(triton2_drv, TRITON2_PREDL_CTL_OFFSET, size, 0x00);
		triton2_i2c_write(triton2_drv, TRITON2_PREDR_CTL_OFFSET, size, 0x00);
		triton2_i2c_write(triton2_drv, TRITON2_PRECKL_CTL_OFFSET, size, 0x00);
		triton2_i2c_write(triton2_drv, TRITON2_PRECKR_CTL_OFFSET, size, 0x00);
		triton2_i2c_write(triton2_drv, TRITON2_HFR_CTL_OFFSET, size, 0x20);
		triton2_i2c_write(triton2_drv, TRITON2_HFR_CTL_OFFSET, size, 0x30);
		triton2_i2c_write(triton2_drv, TRITON2_HFR_CTL_OFFSET, size, 0x3C);
		triton2_i2c_write(triton2_drv, TRITON2_HFR_CTL_OFFSET, size, 0x3E);
		break;
	default:
		/* Enable  only headset R2 and L2. */
		triton2_i2c_write(triton2_drv, TRITON2_EAR_CTL_OFFSET, size, 0x00);
		triton2_i2c_write(triton2_drv, TRITON2_HS_SEL_OFFSET, size, 0x24);
		triton2_i2c_write(triton2_drv, TRITON2_HS_GAIN_SET_OFFSET, size, 0x0A);
		triton2_i2c_write(triton2_drv, TRITON2_PREDL_CTL_OFFSET, size, 0x00);
		triton2_i2c_write(triton2_drv, TRITON2_PREDR_CTL_OFFSET, size, 0x00);
		triton2_i2c_write(triton2_drv, TRITON2_PRECKL_CTL_OFFSET, size, 0x00);
		triton2_i2c_write(triton2_drv, TRITON2_PRECKR_CTL_OFFSET, size, 0x00);
		triton2_i2c_write(triton2_drv, TRITON2_HFL_CTL_OFFSET, size, 0x00);
		triton2_i2c_write(triton2_drv, TRITON2_HFR_CTL_OFFSET, size, 0x00);
		break;
	}

	return SUCCESS;
}

S32 outdev_settings(triton_drv_struct_t * triton2_drv, tOutputs outdev)
{
	U32 size = 1;
	S32 ret_status = SUCCESS;

	configure_i2c_id_for_audio_reg(triton2_drv);

	/* Enable the particular output device */
	ret_status = av_enable_out_dev(triton2_drv, outdev);
	if (SUCCESS != ret_status) {
#ifdef OMAP3530_DEBUG
		DEBUG("%s:%d - Triton2: Codec  O/P device setting failed.\r\n", __FILE__, __LINE__);
#endif
		return ret_status;
	}

	switch (outdev) {
	case tLeftLS:
		/* Enable Audio RX Right 2 and Audio RX Left 2 digital filers */
		triton2_i2c_write(triton2_drv, TRITON2_OPTION_OFFSET, size, 0xC0);
		/* Set the RXL2 gain */
		triton2_i2c_write(triton2_drv, TRITON2_ARXL2PGA_OFFSET, size, 0xFF);
		/* Enable the DACL2 power control */
		triton2_i2c_write(triton2_drv, TRITON2_ADAC_CTL_OFFSET, size, 0x08);
		/* SDRL2 to RXL2 PGA */
		triton2_i2c_write(triton2_drv, TRITON2_RXPATHSEL_OFFSET, size, 0x28);
		break;
	case tRightLS:
		/* Enable Audio RX Right 2 and Audio RX Left 2 digital filers */
		triton2_i2c_write(triton2_drv, TRITON2_OPTION_OFFSET, size, 0xC0);
		/* Set the RXR2 gain */
		triton2_i2c_write(triton2_drv, TRITON2_ARXR2PGA_OFFSET, size, 0xFF);
		/* Enable the DACL2 power control */
		triton2_i2c_write(triton2_drv, TRITON2_ADAC_CTL_OFFSET, size, 0x04);
		/* SDRR2 to RXR2 PGA */
		triton2_i2c_write(triton2_drv, TRITON2_RXPATHSEL_OFFSET, size, 0x12);
		break;
	case tSterioJack:
	default:
		/* Enable Audio RX Right 2 and Audio RX Left 2 digital filers */
		triton2_i2c_write(triton2_drv, TRITON2_OPTION_OFFSET, size, 0xC0);
		/* Set the RXR2 gain */
		triton2_i2c_write(triton2_drv, TRITON2_ARXR2PGA_OFFSET, size, 0x36);
		/* Set the RXL2 gain */
		triton2_i2c_write(triton2_drv, TRITON2_ARXL2PGA_OFFSET, size, 0x36);
		/* enable the DACR2 and DACL2 power control */
		triton2_i2c_write(triton2_drv, TRITON2_ADAC_CTL_OFFSET, size, 0x0C);
		break;
	}
	return SUCCESS;
}

/*-----------------------------------------------------------------------------
| Function    : enable_triton2_vdac
+------------------------------------------------------------------------------
| Description : Enable the Triton2 VDAC power resources for 3530 SDP
|
| Parameters  : none
|
| Returns     : U32 (error code)
+-----------------------------------------------------------------------------*/
U32 enable_triton2_vdac(void)
{
	U32 ret_val;
	/* Configure and enable VDAC for 1.8v */
	ret_val = configure_triton2_power_resources(TRITON2_VDAC_VOLTAGE_TAG, VDAC_1P8, TRITON2_VDAC_ON_OFF_TAG, TRITON2_RES_ACTIVE);
	if (ret_val != SUCCESS) {
		printf("Triton2: VDAC configuration failed\n");
	}
	return (ret_val);
}

/*-----------------------------------------------------------------------------
| Function    : disable_triton2_vdac
+------------------------------------------------------------------------------
| Description : Disable the Triton2 VDAC power resources for 3530 SDP
|
| Parameters  : none
|
| Returns     : U32 (error code)
+-----------------------------------------------------------------------------*/
U32 disable_triton2_vdac(void)
{
	U32 ret_val;
	/* Disable the VDAC */
	ret_val = configure_triton2_power_resources(TRITON2_VDAC_VOLTAGE_TAG, VDAC_1P8, TRITON2_VDAC_ON_OFF_TAG, TRITON2_RES_OFF);
	if (ret_val != SUCCESS) {
		printf("Triton2: VDAC configuration failed\n");
	}
	return (ret_val);
}
