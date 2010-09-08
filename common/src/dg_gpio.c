/*
 * dg_gpio.c: OMAP3530 GPIO code.
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
#include "dg_gpio.h"

T_GPIO_PIN_INT_HANDLER gpio_handler_table[GPIO_MAX_MODULES][NUM_PINS_PER_GPIO_MODULE];

U32 g_gpio_module_base_address[GPIO_MAX_MODULES] = { GPIO1_MODULE_BA,
	GPIO2_MODULE_BA,
	GPIO3_MODULE_BA,
	GPIO4_MODULE_BA,
	GPIO5_MODULE_BA,
	GPIO6_MODULE_BA
};

/* This function checks the validity of the module number. */
S32 check_gpio_pin_num(U16 pin_num)
{
	/*
	 ** Check whether the pin number is within 0 and 128. 
	 ** If yes return SUCCESS, else return FAILURE.
	 */
	if (pin_num > MAX_GPIO_PINS) {
#ifdef OMAP3530_DEBUG
		DEBUG("%s:%d - Wrong gpio-pin number.\r\n", __FILE__, __LINE__);
#endif
		return FAILURE;
	}
#ifdef OMAP3530_DEBUG
	DEBUG("gpio-pin number is valid.\r\n");
#endif
	return SUCCESS;
}

/* This function is to initialize the gpio pins. */
void set_gpio_in_out(U8 module_num, U32 pin_mask, U32 io_mask)
{
	U32 temp_oe;
	U32 gpio_pin_output_en_reg;

	/* Get the GPIO_OE register address. */
	gpio_pin_output_en_reg = (g_gpio_module_base_address[module_num - 1]
				  + GPIO_OE);
	/*Get GPIO_OE register contents. */
	temp_oe = in_regl(gpio_pin_output_en_reg);
	temp_oe = temp_oe & ~pin_mask;	/* manipulate only the pins that needs
					 ** configuration. */
	temp_oe = temp_oe | io_mask;	/* set the pins as input  or output as
					 ** specified. */
	/* Write back. */
	out_regl(gpio_pin_output_en_reg, temp_oe);

#ifdef OMAP3530_DEBUG
	DEBUG("gpio in/out set.\r\n");
#endif
	return;
}

/* This function is to initialize the gpio pin as input or output. */
S32 gpio_pin_init(U16 pin_num, U8 in_out)
{
	/* GPIO module to which the GPIO number belongs to. */
	U8 module_num = (pin_num / NUM_OF_BITS_IN_REG + 1);
	U32 snum = ((module_num - 1) * NUM_OF_BITS_IN_REG);
	U32 offset = (pin_num - snum);
	U32 pinmask;
	S32 ret_val = SUCCESS;

	/* 
	 ** Check whether the gpio number is valid. 
	 ** If yes continue else return. 
	 */
	ret_val = check_gpio_pin_num(pin_num);
	if (SUCCESS != ret_val) {
#ifdef OMAP3530_DEBUG
		DEBUG("%s:%d - Wrong gpio-pin number.\r\n", __FILE__, __LINE__);
#endif
		return (ret_val);
	}

	/*
	 ** This variable will have the position of the pin in 
	 ** the corresponding gpio module.
	 */
	pinmask = (GPIO_PIN << offset);

	/* Call this function to configure the pin as input or output. */
	set_gpio_in_out(module_num, pinmask, (in_out << offset));

#ifdef OMAP3530_DEBUG
	DEBUG("gpio pin %d initialized.\r\n", pin_num);
#endif
	return (ret_val);
}

/* This function writes to the DATA_OUT register of the GPIO. */
void gpio_write_output_pin(U8 module_num, U32 pin_mask, U32 data)
{
	U32 temp;
	U32 gpio_data_out_reg;

	/* Get the GPIO_DATAOUT register address. */
	gpio_data_out_reg = (g_gpio_module_base_address[module_num - 1]
			     + GPIO_DATAOUT);

	/*
	 ** Get the current data for all pins other than the ones, 
	 ** we are programming with new values. temp will have the current data. 
	 */
	temp = in_regl(gpio_data_out_reg);
	temp = temp & ~pin_mask;

	/*  Now OR in the new values and write it out! */
	out_regl(gpio_data_out_reg, (temp | (data & pin_mask)));

#ifdef OMAP3530_DEBUG
	DEBUG("gpio write successful.\r\n");
#endif
	return;
}

/* Writing data to the GPIO pins. */
S32 set_gpio_output(U16 pin_num, U8 set)
{
	S32 ret_val = FAILURE;
	/* GPIO module to which the GPIO pin number belongs to. */
	U8 module_num = (pin_num / NUM_OF_BITS_IN_REG + 1);
	U32 snum = ((module_num - 1) * NUM_OF_BITS_IN_REG);
	/* Position of the GPIO pin in respective module register. */
	U32 offset = (pin_num - snum);

	/* 
	 ** Check whether the gpio number is valid. 
	 ** if yes continue else return.
	 */
	ret_val = check_gpio_pin_num(pin_num);
	if (SUCCESS != ret_val) {
#ifdef OMAP3530_DEBUG
		DEBUG("%s:%d - Wrong gpio-pin number.\r\n", __FILE__, __LINE__);
#endif
		return (ret_val);
	}

	/* 
	 ** If the output pin value has to be made 1, then the data to be
	 ** written will be - the bit corresponding to that pin is set
	 ** else that bit is cleared and that function is called. 
	 */
	if (set) {
		gpio_write_output_pin(module_num, (1 << offset), (1 << offset));
	} else {
		gpio_write_output_pin(module_num, (1 << offset), (0 << offset));
	}

#ifdef OMAP3530_DEBUG
	DEBUG("Wrote the data to gpio pin - %d.\r\n", pin_num);
#endif
	return (ret_val);
}

void gpio_read_input_pin(U8 module_num, U32 pin_mask, U32 * data)
{
	U32 gpio_data_in_reg;
	U32 temp;		/*temporary variable */

	/*get the GPIO_DATAIN register address */
	gpio_data_in_reg = (g_gpio_module_base_address[module_num - 1] + GPIO_DATAIN);

	/* get the DATA_IN register value */
	temp = in_regl(gpio_data_in_reg);
	/* get the data relevant to the pin mask */
	*data = temp & pin_mask;
	return;
}

S32 get_gpio_input(U32 pin_num)
{
	U8 module_num = pin_num / NUM_OF_BITS_IN_REG + 1;
	/* GPIO module to which the GPIO pin number belongs to */
	U32 snum = (module_num - 1) * NUM_OF_BITS_IN_REG;
	U32 offset = pin_num - snum;
	/* position of the pin in the respective gpio module */
	U32 data = 0;
	S32 ret_val = FAILURE;

	/* check whether the gpio number is valid */
	/* if yes continue else return */
	ret_val = check_gpio_pin_num(pin_num);
	if (ret_val != SUCCESS) {
		fprintf(stdout, "wrong gpio-pin number \r\n");
		return (ret_val);
	}

	/* call this function to read the value at the particular pin
	   of the corresponding module */
	gpio_read_input_pin(module_num, (1 << offset), &data);
	data = data >> offset;
	/* return from this function */
#ifdef OMAP3530_DEBUG
	DEBUG("read the data at gpio pin %d \n", pin_num);
#endif
	return (data);
}
