/*
 * lcd_utils.c: LCD device related routines
 *
 * (C) Copyright 2010
 * Mistral Solutions Private Limited, <www.mistralsolutions.com>
 *
 * Author :
 * Jitendra <jitendra@mistralsolutions.com>
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
#include <asm/io.h>
#include <asm/arch/mux.h>
#include "types.h"
#include "omap3530evm.h"
#include "dg_gpio.h"
#include "i2c_drv.h"
#include "i2c_utils.h"
#include "triton2_utils.h"
#include "lcd_drv.h"
#include "lcd_utils.h"
#include "dss.h"
#include "../../diagnostics/inc/mt9t111.h"
#include "../../diagnostics/inc/dg_videoin.h"

#define  SD_FRAME_BUFFER 0x80377000

static U16 lcd_height = 640;	//320
static U16 lcd_width = 480;	//240

/* TEMP_FIX, need to be changed. */
static U16 *image_address = (U16 *) SD_FRAME_BUFFER;

T_IMAGE_TYPE image_type_val[] = { {96, 128},
{144, 176},
{240, 320},
{480, 640},
{480, 854},
{768, 1024},
{1024, 1280},
{1200, 1600},
{120, 160},
{240, 352}
};

static S16 colour_convbt601_full[][3] = { {298, 409, 0},
{298, -208, -11},
{298, 0, 517}
};

/* This function sets the pin multiplexing for the lcd interface. */
void mux_setup_lcd_bk_vdd(void)
{
	/* LCD_RESB pin configuration, PIN = AC1  */
	MUX_VAL(CP(MCBSP4_FSX), (IDIS | PTD | EN | M4));

	/* LCD_QVGA pin configuration, PIN = AD2 */
	MUX_VAL(CP(MCBSP4_DX), (IDIS | PTD | EN | M4));

	/* LCD_ENVDD pin configuration, PIN = AD1 */
	MUX_VAL(CP(MCBSP4_DR), (IDIS | PTD | EN | M4));

	/* LCD_INI pin configuration, PIN = AE1 */
	MUX_VAL(CP(MCBSP4_CLKX), (IEN | PTD | EN | M4));

	/* LCD_LR pin configuration, PIN = AH26 */
	MUX_VAL(CP(SYS_BOOT0), (IDIS | PTD | DIS | M4))

	    /* LCD_UD pin configuration, PIN = AG26 */
	    MUX_VAL(CP(SYS_BOOT1), (IDIS | PTD | DIS | M4));
}

// ssp - changing the pin assignments
/* 
** This function sets the pin mux configuration registers for 
** all the signals related to LCD.
*/
void mux_setup_lcd(void)
{
#if 0
	/* LCD_D0 pin configuration, PIN = F27 */
	out_regs(CONTROL_PADCONF_DSS_D0, 0x0018);

	/* LCD_D1 pin configuration, PIN = G26 */
	out_regs(CONTROL_PADCONF_DSS_D1, 0x0018);

	/* LCD_D2 pin configuration, PIN = H25 */
	out_regs(CONTROL_PADCONF_DSS_D2, 0x0018);

	/* LCD_D3 pin configuration, PIN = F28 */
	out_regs(CONTROL_PADCONF_DSS_D3, 0x0018);

	/* LCD_D4 pin configuration, PIN = H26 */
	out_regs(CONTROL_PADCONF_DSS_D4, 0x0018);

	/* LCD_D5 pin configuration, PIN = H27 */
	out_regs(CONTROL_PADCONF_DSS_D5, 0x0018);

	/* LCD_D6 pin configuration, PIN = H28 */
	out_regs(CONTROL_PADCONF_DSS_D6, 0x0018);

	/* LCD_D7 pin configuration, PIN = J25 */
	out_regs(CONTROL_PADCONF_DSS_D7, 0x0018);

	/* LCD_D8 pin configuration, PIN = J26 */
	out_regs(CONTROL_PADCONF_DSS_D8, 0x0018);

	/* LCD_D9 pin configuration, PIN = K26 */
	out_regs(CONTROL_PADCONF_DSS_D9, 0x0018);

	/* LCD_D10 pin configuration, PIN = AD28 */
	out_regs(CONTROL_PADCONF_DSS_D10, 0x0018);

	/* LCD_D11 pin configuration, PIN = AD27 */
	out_regs(CONTROL_PADCONF_DSS_D11, 0x0018);

	/* LCD_D12 pin configuration, PIN = AB28 */
	out_regs(CONTROL_PADCONF_DSS_D12, 0x0018);

	/* LCD_D13 pin configuration, PIN = AB27 */
	out_regs(CONTROL_PADCONF_DSS_D13, 0x0018);

	/* LCD_D14 pin configuration, PIN = AA28 */
	out_regs(CONTROL_PADCONF_DSS_D14, 0x0018);

	/* LCD_D15 pin configuration, PIN = AA27 */
	out_regs(CONTROL_PADCONF_DSS_D15, 0x0018);

	/* LCD_D16 pin configuration, PIN = K25 */
	out_regs(CONTROL_PADCONF_DSS_D16, 0x0018);

	/* LCD_D17 pin configuration, PIN = N26 */
	out_regs(CONTROL_PADCONF_DSS_D17, 0x0018);

	/* LCD_PCLK pin configuration, PIN = E28 */
	out_regs(CONTROL_PADCONF_DSS_PCLK, 0x0018);

	/* LCD_VSYNC pin configuration, PIN = E27 */
	out_regs(CONTROL_PADCONF_DSS_VSYNC, 0x0018);

	/* LCD_HSYNC pin configuration, PIN = D28 */
	out_regs(CONTROL_PADCONF_DSS_HSYNC, 0x0018);

	/* LCD_ACBIAS pin configuration, PIN = G25 */
	out_regs(CONTROL_PADCONF_DSS_ACBIAS, 0x0018);
#else
	MUX_VAL(CP(DSS_PCLK), (IDIS | PTD | DIS | M0));
	MUX_VAL(CP(DSS_HSYNC), (IDIS | PTD | DIS | M0));
	MUX_VAL(CP(DSS_VSYNC), (IDIS | PTD | DIS | M0));
	MUX_VAL(CP(DSS_ACBIAS), (IDIS | PTD | DIS | M0));
	MUX_VAL(CP(DSS_DATA0), (IDIS | PTD | DIS | M7));
	MUX_VAL(CP(DSS_DATA1), (IDIS | PTD | DIS | M7));
	MUX_VAL(CP(DSS_DATA2), (IDIS | PTD | DIS | M7));
	MUX_VAL(CP(DSS_DATA3), (IDIS | PTD | DIS | M7));
	MUX_VAL(CP(DSS_DATA4), (IDIS | PTD | DIS | M7));
	MUX_VAL(CP(DSS_DATA5), (IDIS | PTD | DIS | M7));
	MUX_VAL(CP(DSS_DATA6), (IDIS | PTD | DIS | M0));
	MUX_VAL(CP(DSS_DATA7), (IDIS | PTD | DIS | M0));
	MUX_VAL(CP(DSS_DATA8), (IDIS | PTD | DIS | M0));
	MUX_VAL(CP(DSS_DATA9), (IDIS | PTD | DIS | M0));
	MUX_VAL(CP(DSS_DATA10), (IDIS | PTD | DIS | M0));
	MUX_VAL(CP(DSS_DATA11), (IDIS | PTD | DIS | M0));
	MUX_VAL(CP(DSS_DATA12), (IDIS | PTD | DIS | M0));
	MUX_VAL(CP(DSS_DATA13), (IDIS | PTD | DIS | M0));
	MUX_VAL(CP(DSS_DATA14), (IDIS | PTD | DIS | M0));
	MUX_VAL(CP(DSS_DATA15), (IDIS | PTD | DIS | M0));
	MUX_VAL(CP(DSS_DATA16), (IDIS | PTD | DIS | M0));
	MUX_VAL(CP(DSS_DATA17), (IDIS | PTD | DIS | M0));
	MUX_VAL(CP(DSS_DATA18), (IDIS | PTD | DIS | M3));
	MUX_VAL(CP(DSS_DATA19), (IDIS | PTD | DIS | M3));
	MUX_VAL(CP(DSS_DATA20), (IDIS | PTD | DIS | M3));
	MUX_VAL(CP(DSS_DATA21), (IDIS | PTD | DIS | M3));
	MUX_VAL(CP(DSS_DATA22), (IDIS | PTD | DIS | M3));
	MUX_VAL(CP(DSS_DATA23), (IDIS | PTD | DIS | M3));
#endif
}

/* Issue a go LCD command. */
void issue_go_lcd(void)
{
	U32 ctrl;
	U8 timeout = 100;
	U8 count;
	U32 dispc_control;

	dispc_control = in_regl(DISPC_CONTROL);
	dispc_control |= DISPC_CONTROL_GOLCD;	/* Lcd GO command. */
	out_regl(DISPC_CONTROL, dispc_control);

	/* 
	 ** Wait for go. 
	 ** Read dispc_control register.
	 */
	ctrl = in_regl(DISPC_CONTROL);

	while ((ctrl & DISPC_CONTROL_GOLCD) && (timeout > 0)) {
		for (count = 0; count < 100; ++count) ;
		ctrl = in_regl(DISPC_CONTROL);
		timeout--;
	}
}

/* This function disables the LCD controller. */
void disable_dss(void)
{
	U8 timeout, count;
	U32 reg_val;

	/* read the DIPC_CONTROL reg */
	reg_val = in_regl(DISPC_CONTROL);

	timeout = 100;
	/* check if digital output or the lcd output are enabled */
	if (reg_val & (DISPC_CONTROL_DIGITALENABLE | DISPC_CONTROL_LCDENABLE)) {
		/*Disable the lcd output and digital output */
		reg_val &= ~(DISPC_CONTROL_DIGITALENABLE | DISPC_CONTROL_LCDENABLE);
		out_regl(DISPC_CONTROL, reg_val);

		out_regl(DISPC_IRQSTATUS, DISPC_IRQSTATUS_FRAMEDONE);

		while (!((reg_val = in_regl(DISPC_IRQSTATUS)) & DISPC_IRQSTATUS_FRAMEDONE)
		       && (timeout > 0)) {
			for (count = 0; count < 100; ++count) ;
			timeout--;
		}
	}
#ifdef OMAP3530_DEBUG
	DEBUG("DSS disabled.\r\n");
#endif
}

/* This function resets the Display sub system on omap24xx. */
void reset_display_controller(void)
{
	U32 reg_val, count;
	U32 timeout;

	/* Disable the display controller. */
	disable_dss();

	/* Reset the display controller. */
	out_regl(DISPC_SYSCONFIG, DISPC_SYSCONFIG_SOFTRESET);

	/* Wait until reset completes OR timeout occurs. */
	timeout = 100;
	while (!((reg_val = in_regl(DISPC_SYSSTATUS)) & DISPC_SYSSTATUS_RESETDONE)
	       && (timeout > 0)) {
		/* Delay. */
		for (count = 0; count < 100; ++count) ;
		timeout--;
	}
	if (!(reg_val & DISPC_SYSSTATUS_RESETDONE)) {
#ifdef OMAP3530_DEBUG
		DEBUG("DSS reset timeout.\r\n");
#endif
	}

	reg_val = in_regl(DISPC_SYSCONFIG);
	reg_val &= ~(DISPC_SYSCONFIG_SOFTRESET);
	out_regl(DISPC_SYSCONFIG, reg_val);

#ifdef OMAP3530_DEBUG
	DEBUG("DSS reset reset display controller.\r\n");
#endif
}

/* This function is used to enable the power to the LCD device. */
S32 enable_lcd_power(void)
{
	S32 ret_val = SUCCESS;

	ret_val = gpio_pin_init(GPIO_NUM_VDD, 0);
	if (ret_val != SUCCESS) {
#ifdef OMAP3530_DEBUG
		DEBUG("%s:%d - GPIO init failed.\r\n", __FILE__, __LINE__);
#endif
		return ret_val;
	}

	/* Write 0 to the gpio data register to enable LCD power. */
	ret_val = set_gpio_output(GPIO_NUM_VDD, 0);
	if (ret_val != SUCCESS) {
#ifdef OMAP3530_DEBUG
		DEBUG("%s:%d - GPIO set output failed.\r\n", __FILE__, __LINE__);
#endif
		return ret_val;
	}
#ifdef OMAP3530_DEBUG
	DEBUG("DSS enabled lcd power.\r\n");
#endif

	return ret_val;
}

/* This function is used to enable the back light for the LCD controller. */
S32 enable_lcd_backlight(lcd_drv_struct_t * lcd_init_struct)
{
	S32 ret_status = SUCCESS;
	U32 size = 1;
	U8 reg_data;

	/* Configure the i2c slave address for the Power regsiters. */
	configure_i2c_slave_id_for_triton2(&(lcd_init_struct->i2c_init_str), T2_I2C_LED_ADDR_GROUP);

	reg_data = 0x33;
	ret_status = dg_i2c_write((&lcd_init_struct->i2c_init_str), I2C_OFFSET(TRITON2_LED_LEDEN_REG), &size, &reg_data);
	if (SUCCESS != ret_status) {
#ifdef OMAP3530_DEBUG
		DEBUG("%s:%d - LCD: I2C Write failed.\r\n", __FILE__, __LINE__);
#endif
	}

	reg_data = 0x77;
	ret_status = dg_i2c_write((&lcd_init_struct->i2c_init_str), I2C_OFFSET(TRITON2_LED_PWMAON_REG), &size, &reg_data);
	if (SUCCESS != ret_status) {
#ifdef OMAP3530_DEBUG
		DEBUG("%s:%d - LCD: I2C Write failed.\r\n", __FILE__, __LINE__);
#endif
	}

	reg_data = 0x77;
	ret_status = dg_i2c_write((&lcd_init_struct->i2c_init_str), I2C_OFFSET(TRITON2_LED_PWMBON_REG), &size, &reg_data);
	if (SUCCESS != ret_status) {
#ifdef OMAP3530_DEBUG
		DEBUG("%s:%d - LCD: I2C Write failed.\r\n", __FILE__, __LINE__);
#endif
	}

	reg_data = 0x7f;
	ret_status = dg_i2c_write((&lcd_init_struct->i2c_init_str), I2C_OFFSET(TRITON2_LED_PWMAOFF_REG), &size, &reg_data);
	if (SUCCESS != ret_status) {
#ifdef OMAP3530_DEBUG
		DEBUG("%s:%d - LCD: I2C Write failed.\r\n", __FILE__, __LINE__);
#endif
	}

	reg_data = 0x7f;
	ret_status = dg_i2c_write((&lcd_init_struct->i2c_init_str), I2C_OFFSET(TRITON2_LED_PWMBOFF_REG), &size, &reg_data);
	if (SUCCESS != ret_status) {
#ifdef OMAP3530_DEBUG
		DEBUG("%s:%d - LCD: I2C Write failed.\r\n", __FILE__, __LINE__);
#endif
	}
#ifdef OMAP3530_DEBUG
	DEBUG("Enabled lcd backlight.\r\n");
#endif

	return ret_status;
}

/* This function is used to select vertical scanning direction. */
S32 configure_vert_scan_direction(U8 direction)
{
	S32 ret_val = SUCCESS;

	ret_val = gpio_pin_init(GPIO_NUM_UD, 0);
	if (ret_val != SUCCESS) {
#ifdef OMAP3530_DEBUG
		DEBUG("%s:%d - GPIO init failed.\r\n", __FILE__, __LINE__);
#endif
		return ret_val;
	}

	/* 
	 ** Write direction to the gpio data register to enable 
	 ** conventional scanning direction for vertical scanning.
	 */
	ret_val = set_gpio_output(GPIO_NUM_UD, direction);
	if (ret_val != SUCCESS) {
#ifdef OMAP3530_DEBUG
		DEBUG("%s:%d - GPIO set output failed.\r\n", __FILE__, __LINE__);
#endif
		return ret_val;
	}
#ifdef OMAP3530_DEBUG
	DEBUG("Conventional scanning direction for vertical scanning.\r\n");
#endif

	return ret_val;
}

/* This function is used to select horizontal scanning direction. */
S32 configure_horiz_scan_direction(U8 direction)
{
	S32 ret_val = SUCCESS;

	ret_val = gpio_pin_init(GPIO_NUM_LR, 0);
	if (ret_val != SUCCESS) {
#ifdef OMAP3530_DEBUG
		DEBUG("%s:%d - GPIO init failed.\r\n", __FILE__, __LINE__);
#endif
		return ret_val;
	}

	/*
	 ** Write direction to the gpio data register to enable
	 ** conventional scanning direction for horizontal scanning.
	 */
	ret_val = set_gpio_output(GPIO_NUM_LR, direction);
	if (ret_val != SUCCESS) {
#ifdef OMAP3530_DEBUG
		DEBUG("%s:%d - GPIO set output failed.\r\n", __FILE__, __LINE__);
#endif
		return ret_val;
	}
#ifdef OMAP3530_DEBUG
	DEBUG("Conventional scanning direction for horizontal scanning.\r\n");
#endif

	return ret_val;
}

/* This function is used to disable the RESB. */
S32 disable_lcd_reset(void)
{
	S32 ret_val = SUCCESS;

	ret_val = gpio_pin_init(GPIO_NUM_RESB, 0);
	if (ret_val != SUCCESS) {
#ifdef OMAP3530_DEBUG
		DEBUG("%s:%d - GPIO init failed.\r\n", __FILE__, __LINE__);
#endif
		return ret_val;
	}

	/* Write 0 to the gpio data register to disable reset the LCD. */
	ret_val = set_gpio_output(GPIO_NUM_RESB, 0);
	if (ret_val != SUCCESS) {
#ifdef OMAP3530_DEBUG
		DEBUG("%s:%d - GPIO set output failed.\r\n", __FILE__, __LINE__);
#endif
		return ret_val;
	}

	/*
	 ** Ideally wait for more than 20 micro seconds.
	 ** Here we are waiting for 100 micro seconds.
	 */
	udelay(HUNDRED_MICROSEC);

#ifdef OMAP3530_DEBUG
	DEBUG("Disabled lcd reset. RESB = 0.\r\n");
#endif

	return ret_val;
}

/* This function is used to enable the RESB. */
S32 enable_lcd_reset(void)
{
	S32 ret_val = SUCCESS;

	/* Write 1 to the gpio data register to reset the LCD. */
	ret_val = set_gpio_output(GPIO_NUM_RESB, 1);
	if (ret_val != SUCCESS) {
#ifdef OMAP3530_DEBUG
		DEBUG("%s:%d - GPIO set output failed.\r\n", __FILE__, __LINE__);
#endif
		return ret_val;
	}
#ifdef OMAP3530_DEBUG
	DEBUG("Enabled lcd reset. RESB = 1.\r\n");
#endif

	return ret_val;
}

/* This function is used to enable power on control to LCD. */
S32 enable_INI(void)
{
	S32 ret_val = SUCCESS;

	ret_val = gpio_pin_init(GPIO_NUM_INI, 0);
	if (ret_val != SUCCESS) {
#ifdef OMAP3530_DEBUG
		DEBUG("%s:%d - GPIO init failed.\r\n", __FILE__, __LINE__);
#endif
		return ret_val;
	}

	/* Write 1 to the gpio data register to reset the LCD. */
	ret_val = set_gpio_output(GPIO_NUM_INI, 1);
	if (ret_val != SUCCESS) {
#ifdef OMAP3530_DEBUG
		DEBUG("%s:%d - GPIO set output failed.\r\n", __FILE__, __LINE__);
#endif
		return ret_val;
	}
#ifdef OMAP3530_DEBUG
	DEBUG("Enabled LCD_INI.\r\n");
#endif

	return ret_val;
}

/* This function is used to disable power on control to LCD. */
S32 disble_INI(void)
{
	S32 ret_val = SUCCESS;

	/* Write 0 to the gpio data register to reset the LCD. */
	ret_val = set_gpio_output(GPIO_NUM_INI, 0);
	if (ret_val != SUCCESS) {
#ifdef OMAP3530_DEBUG
		DEBUG("%s:%d - GPIO set output failed.\r\n", __FILE__, __LINE__);
#endif
		return ret_val;
	}
#ifdef OMAP3530_DEBUG
	DEBUG("Disabled LCD_INI.\r\n");
#endif

	return ret_val;
}

/* Enable HVIF. */
S32 enable_lcd_HVIF(void)
{
	U32 val;
	S32 ret_val = SUCCESS;

	val = 0x00;

	val = in_regl(DISPC_POL_FREQ);
	val |= ((0 << DISPC_POL_FREQ_RF_SHIFT) | (1 << DISPC_POL_FREQ_ONOFF_SHIFT));
	out_regl(DISPC_POL_FREQ, val);

	return ret_val;
}

/* Set back ground colour. */
void set_default_colour(U32 colour)
{
	colour = colour & DEFAULT_COLOUR_MASK;

	/* Set Default Color. */
	out_regl(DISPC_DEFAULT_COLOR0, colour);
}

/* Set Transparency Color colour. */
void set_transparency_colour(U32 colour)
{
	colour = colour & TRANSPARENCY_COLOUR_MASK;

	/* Set Transparency Color. */
	out_regl(DISPC_TRANS_COLOR0, colour);
}

/* Set Panel size. */
void set_panel_size(U8 panel_type)
{
	U32 val;

	val = (((image_type_val[panel_type].x_res - 1) << DISPC_SIZE_LCD_PPL_SHIFT)
	       & DISPC_SIZE_LCD_PPL)
	    | (((image_type_val[panel_type].y_res - 1) << DISPC_SIZE_LCD_LPP_SHIFT)
	       & DISPC_SIZE_LCD_LPP);

	/* Configures the panel size. */
	out_regl(DISPC_SIZE_LCD, val);
}

/* Selects the TFT/LCD interface datalines (16 lines or 18 lines). */
void set_lcd_interface_datalines(U8 datalines)
{
	U32 dispc_control;

	if (datalines == 18) {
		/* To get the current value from the register. */
		dispc_control = in_regl(DISPC_CONTROL);

		/* Clearing the DISPC_CONTROL.TFTDATALINES bits. */
		dispc_control &= ~((DISPC_CONTROL_TFTDATALINES_OALSB16B)
				   | (DISPC_CONTROL_TFTDATALINES_OALSB18B));

		/* Selecting the 18-bit TFT interface. */
		// ssp
		dispc_control |= DISPC_CONTROL_TFTDATALINES_OALSB18B;

		/* Updating the registers with new values. */
		out_regl(DISPC_CONTROL, dispc_control);

	} else {
		;		/* By default 16 bit interface is selected */
	}
}

/* Set pipe line buffer base address. */
void set_buffer_base_addr(U8 pipeline, U32 base_address)
{

	if (pipeline == 0) {	/* graphics */
		out_regl(DISPC_GFX_BA0, base_address);
		out_regl(DISPC_GFX_BA1, base_address);
	} else if (pipeline == 1) {	/* vedio1 */
		out_regl(DISPC_VID1_BA0, base_address);
		out_regl(DISPC_VID1_BA1, base_address);
	} else if (pipeline == 2) {	/* vedio2 */
		out_regl(DISPC_VID2_BA0, base_address);
		out_regl(DISPC_VID2_BA1, base_address);
	} else {
		;		/* Error */
	}
}

/* Set window position. */
void set_window_pos(U8 pipeline, U32 x_pos, U32 y_pos)
{
	U32 position = 0;

	x_pos = x_pos & X_POS_MASK;
	y_pos = y_pos & Y_POS_MASK;
	position = x_pos | (y_pos << 16);

	if (pipeline == 0) {	/*graphics */
		out_regl(DISPC_GFX_POSITION, position);
	} else if (pipeline == 1) {	/*vedio1 */
		out_regl(DISPC_VID1_POSITION, position);
	} else if (pipeline == 2) {	/*vedio2 */
		out_regl(DISPC_VID2_POSITION, position);
	} else {
		;		/* Error */
	}
}

/* Set window size. */
void set_window_size(U8 pipeline, U8 image_type)
{
	U32 val;

	val = (((image_type_val[image_type].x_res - 1) << DISPC_GFX_SIZE_PPL_SHIFT)
	       & DISPC_GFX_SIZE_PPL)
	    | (((image_type_val[image_type].y_res - 1) << DISPC_GFX_SIZE_LPP_SHIFT)
	       & DISPC_GFX_SIZE_LPP);

	if (pipeline == 0) {	/* graphics */
		out_regl(DISPC_GFX_SIZE, val);
	} else if (pipeline == 1) {	/* vedio1 */
		out_regl(DISPC_VID1_SIZE, val);
		out_regl(DISPC_VID1_PICTURE_SIZE, val);
	} else if (pipeline == 2) {	/* vedio2 */
		out_regl(DISPC_VID2_SIZE, val);
		out_regl(DISPC_VID2_PICTURE_SIZE, val);
	} else {
		;		/* Error */
	}
}

/* Set colour space conversion coff. */
void set_csc_coff(U8 pipeline)
{
	if (pipeline == 1) {
		out_regl(DISPC_VID1_CONV_COEF0, (colour_convbt601_full[0][0] & 0x7FF)
			 | (colour_convbt601_full[0][1] & 0x7FF) << 16);

		out_regl(DISPC_VID1_CONV_COEF1, (colour_convbt601_full[0][2] & 0x7FF)
			 | (colour_convbt601_full[1][0] & 0x7FF) << 16);

		out_regl(DISPC_VID1_CONV_COEF2, (colour_convbt601_full[1][1] & 0x7FF)
			 | (colour_convbt601_full[1][2] & 0x7FF) << 16);

		out_regl(DISPC_VID1_CONV_COEF3, (colour_convbt601_full[2][0] & 0x7FF)
			 | (colour_convbt601_full[2][1] & 0x7FF) << 16);

		out_regl(DISPC_VID1_CONV_COEF4, (colour_convbt601_full[2][2] & 0x7FF));
	} else {
		out_regl(DISPC_VID2_CONV_COEF0, (colour_convbt601_full[0][0] & 0x7FF)
			 | (colour_convbt601_full[0][1] & 0x7FF) << 16);

		out_regl(DISPC_VID2_CONV_COEF1, (colour_convbt601_full[0][2] & 0x7FF)
			 | (colour_convbt601_full[1][0] & 0x7FF) << 16);

		out_regl(DISPC_VID2_CONV_COEF2, (colour_convbt601_full[1][1] & 0x7FF)
			 | (colour_convbt601_full[1][2] & 0x7FF) << 16);

		out_regl(DISPC_VID2_CONV_COEF3, (colour_convbt601_full[2][0] & 0x7FF)
			 | (colour_convbt601_full[2][1] & 0x7FF) << 16);

		out_regl(DISPC_VID2_CONV_COEF4, (colour_convbt601_full[2][2] & 0x7FF));
	}
}

/* Set format. */
void set_format(U8 pipeline, U8 format)
{
	U32 val = 0;

	if (pipeline == 0) {	/* graphics */
		val = in_regl(DISPC_GFX_ATTRIBUTES);
		val |= ((format & FORMAT_MASK) << 1);
		out_regl(DISPC_GFX_ATTRIBUTES, val);
	} else if (pipeline == 1) {	/* vedio1 */
		val = in_regl(DISPC_VID1_ATTRIBUTES);
		val |= ((format & FORMAT_MASK) << 1);

		if ((format == YUV422) || (format == UYVY422)) {
			val |= (VIDCOLOURCONVENABLE);
			set_csc_coff(pipeline);
		}
		out_regl(DISPC_VID1_ATTRIBUTES, val);
	} else if (pipeline == 2) {	/* vedio2 */
		val = in_regl(DISPC_VID2_ATTRIBUTES);
		val |= ((format & FORMAT_MASK) << 1);

		if ((format == YUV422) || (format == UYVY422)) {
			val |= (VIDCOLOURCONVENABLE);
			set_csc_coff(pipeline);
		}
		out_regl(DISPC_VID2_ATTRIBUTES, val);
	} else {
		;		/* Error */
	}
}

/* Set FIFO threshold. */
void set_fifo_threshold(U8 pipeline)
{
	U32 val;

	val = (252 << DISPC_GFX_FIFO_THRESHOLD_HIGH_SHIFT)
	    | (192 << DISPC_GFX_FIFO_THRESHOLD_LOW_SHIFT);

	if (pipeline == 0) {	/* graphics */
		out_regl(DISPC_GFX_FIFO_THRESHOLD, val);
	} else if (pipeline == 1) {	/* vedio1 */
		out_regl(DISPC_VID1_FIFO_THRESHOLD, val);
	} else if (pipeline == 2) {	/* vedio2 */
		out_regl(DISPC_VID2_FIFO_THRESHOLD, val);
	} else {
		;		/* Error */
	}
}

/* Set row increment. */
void set_row_inc(U8 pipeline, U32 inc_val)
{
	if (pipeline == 0) {	/* graphics */
		out_regl(DISPC_GFX_ROW_INC, inc_val);
	} else if (pipeline == 1) {	/* vedio1 */
		out_regl(DISPC_VID1_ROW_INC, inc_val);
	} else if (pipeline == 2) {	/* vedio2 */
		out_regl(DISPC_VID2_ROW_INC, inc_val);
	} else {
		;		/* Error */
	}
}

/* Set pixel increment. */
void set_pixel_inc(U8 pipeline, int inc_val)
{
	inc_val &= PIXEL_INC_MASK;

	if (pipeline == 0) {	/* graphics */
		out_regl(DISPC_GFX_PIXEL_INC, inc_val);
	} else if (pipeline == 1) {	/* vedio1 */
		out_regl(DISPC_VID1_PIXEL_INC, inc_val);
	} else if (pipeline == 2) {	/* vedio2 */
		out_regl(DISPC_VID2_PIXEL_INC, inc_val);
	} else {
		;		/* Error */
	}
}

/* Enable pipeline. */
void enable_pipeline(U8 pipeline)
{
	U32 val = 0;

	if (pipeline == 0) {	/* graphics */
		val = in_regl(DISPC_GFX_ATTRIBUTES);
		val |= ATTRIBUTES_ENABLE;
		out_regl(DISPC_GFX_ATTRIBUTES, val);
	} else if (pipeline == 1) {	/* vedio1 */
		val = in_regl(DISPC_VID1_ATTRIBUTES);
		val |= ATTRIBUTES_ENABLE;
		out_regl(DISPC_VID1_ATTRIBUTES, val);
	} else if (pipeline == 2) {	/* vedio2 */
		val = in_regl(DISPC_VID2_ATTRIBUTES);
		val |= ATTRIBUTES_ENABLE;
		out_regl(DISPC_VID2_ATTRIBUTES, val);
	} else {
		;		/* Error */
	}
}

/* Configure the Display Sub Sytem on omap24xx. */
void configure_dss(U8 mode)
{
	U16 fp, bp, sw;
	U8 temp = 0x00;
	U32 val, data;

	/* No standby, No idle,mormal mode, ocp clock free running */
	data = (DISPC_SYSCONFIG_MIDLEMODE_NSTANDBY | DISPC_SYSCONFIG_SIDLEMODE_NIDLE);
	data &= ~DISPC_SYSCONFIG_SOFTRESET;
	out_regl(DISPC_SYSCONFIG, data);

	/* Not enabling any interrupts */
	out_regl(DISPC_IRQENABLE, 0x00);

	/*  2:1 - Frame Data only loaded every frame (10) */
	temp = DISPC_CONFIG_LOADMODE_FRDATLEFR;
	out_regl(DISPC_CONFIG, temp);

	/* Default Color is white */
	set_default_colour(0xFFFFFF);

	/* Default Transparency Color is black */
	set_transparency_colour(BLACK_COLOR);

	/*timing logic for HSYNC signal */
	bp = 38;
	fp = 44;
	sw = 2;
	data = (bp << DISPC_TIMING_H_HBP_SHIFT) | (fp << DISPC_TIMING_H_HFP_SHIFT)
	    | (sw << DISPC_TIMING_H_HSW_SHIFT);
	out_regl(DISPC_TIMING_H, data);

	/*timing logic for VSYNC signal */
	bp = 1;
	fp = 2;
	sw = 1;

	data = (bp << DISPC_TIMING_V_VBP_SHIFT) | (fp << DISPC_TIMING_V_VFP_SHIFT)
	    | (sw << DISPC_TIMING_V_VSW_SHIFT);
	out_regl(DISPC_TIMING_V, data);

	/*signal configuration */
	val = in_regl(DISPC_POL_FREQ);
	val |= ((0 << DISPC_POL_FREQ_RF_SHIFT) | (1 << DISPC_POL_FREQ_ONOFF_SHIFT)
		| (1 << DISPC_POL_FREQ_IPC_SHIFT) | (1 << DISPC_POL_FREQ_IHS_SHIFT)
		| (1 << DISPC_POL_FREQ_IVS_SHIFT));
	out_regl(DISPC_POL_FREQ, val);

	/*configures the divisor */
	if (mode == VGA) {
		val = (1 << DISPC_DIVISOR_LCD_SHIFT) | (3 << DISPC_DIVISOR_PCD_SHIFT);
	} else {
		val = (1 << DISPC_DIVISOR_LCD_SHIFT) | (13 << DISPC_DIVISOR_PCD_SHIFT);
	}
	out_regl(DISPC_DIVISOR, val);

	/* Set panel size */
	set_panel_size(mode);

	set_lcd_interface_datalines(18);	/* tft interface width = 18. */

	/* 
	 ** Configure Graphics Window. 
	 */

	/* Set buffer base address. */
	set_buffer_base_addr(GFX_PIPELINE, (U32) image_address);

	/* 
	 ** Configure the position of graphics window.
	 ** GFX window Pos x = 0, y = 0
	 */
	set_window_pos(GFX_PIPELINE, 0x0, 0x0);

	/* Size of graphics window. */
	set_window_size(GFX_PIPELINE, mode);

	if ((mt9t111_lcd == 1) || (videoin_lcd == 1)) {
		set_format(GFX_PIPELINE, RGB24);
	}
	else
		set_format(GFX_PIPELINE, RGB16);

	set_fifo_threshold(GFX_PIPELINE);

	set_row_inc(GFX_PIPELINE, 0x1);	/* Default row inc = 1. */
	set_pixel_inc(GFX_PIPELINE, 0x1);	/* Default pixel inc = 1. */
	enable_pipeline(GFX_PIPELINE);

#ifdef OMAP3530_DEBUG
	DEBUG("Configured DSS for LCD.\r\n");
#endif
}

/* Display the image in the frame buffer on the LCD. */
void display_lcd_image(void)
{
	U32 dispc_control;

	/* Lcd output enabled, active display, 16-bit output */
	dispc_control = (DISPC_CONTROL_GPOUT1 | DISPC_CONTROL_GPOUT0 | DISPC_CONTROL_TFTDATALINES_OALSB18B | DISPC_CONTROL_STNTFT | DISPC_CONTROL_LCDENABLE);
	dispc_control &= ~DISPC_CONTROL_RFBIMODE;	/* RFBI mode disabled. */

	out_regl(DISPC_CONTROL, dispc_control);

	issue_go_lcd();
}

/* Replication logic enabled. */
void enable_replication_logic(U8 pipeline)
{
	U32 attributes;

	if (pipeline == 0) {	/* graphics */
		attributes = in_regl(DISPC_GFX_ATTRIBUTES);
		/* Enable GFX repilcation to make 16-bit colour as 18-bit colour. */
		attributes |= DISPC_GFX_ATTRIBUTES_REPLICATION_EN;
		/* Updating the registers with new values. */
		out_regl(DISPC_GFX_ATTRIBUTES, attributes);
	} else if (pipeline == 1) {	/* vedio1 */
		attributes = in_regl(DISPC_VID1_ATTRIBUTES);
		/* Enable GFX repilcation to make 16-bit colour as 18-bit colour. */
		attributes |= DISPC_VID_ATTRIBUTES_REPLICATION_EN;
		/* Updating the registers with new values. */
		out_regl(DISPC_VID1_ATTRIBUTES, attributes);
	} else if (pipeline == 2) {	/* vedio2 */
		attributes = in_regl(DISPC_VID2_ATTRIBUTES);
		/* Enable GFX repilcation to make 16-bit colour as 18-bit colour. */
		attributes |= DISPC_VID_ATTRIBUTES_REPLICATION_EN;
		/* Updating the registers with new values. */
		out_regl(DISPC_VID2_ATTRIBUTES, attributes);
	} else {
		;		/* Error */
	}
}

/* Disable pipeline. */
void disable_pipeline(U8 pipeline)
{
	if (pipeline == 0) {	/* graphics */
		reg_modify32(DISPC_GFX_ATTRIBUTES, CLEAR_NONE, ATTRIBUTES_ENABLE);
	} else if (pipeline == 1) {	/* vedio1 */
		reg_modify32(DISPC_VID1_ATTRIBUTES, CLEAR_NONE, ATTRIBUTES_ENABLE);
	} else if (pipeline == 2) {	/* vedio2 */
		reg_modify32(DISPC_VID2_ATTRIBUTES, CLEAR_NONE, ATTRIBUTES_ENABLE);
	} else {
		;		/* Error. */
	}
}

/* This function is used to disable the back light for the LCD controller. */
S32 disable_lcd_backlight(lcd_drv_struct_t * lcd_init_struct)
{
	S32 ret_status = SUCCESS;
	U32 size = 1;
	U8 reg_data;

	reg_data = 0;

	/* Configure the i2c slave address for the Power regsiters. */
	configure_i2c_slave_id_for_triton2(&(lcd_init_struct->i2c_init_str), T2_I2C_LED_ADDR_GROUP);

	ret_status = dg_i2c_write((&lcd_init_struct->i2c_init_str), I2C_OFFSET(TRITON2_LED_LEDEN_REG), &size, &reg_data);
	if (SUCCESS != ret_status) {
#ifdef OMAP3530_DEBUG
		DEBUG("%s:%d - LCD: I2C Write failed.\r\n", __FILE__, __LINE__);
#endif
	}
#ifdef OMAP3530_DEBUG
	DEBUG("Disabled lcd backlight.\r\n");
#endif

	return ret_status;
}

/* This function is used to disable the power to the LCD device. */
S32 disable_lcd_power(void)
{
	S32 ret_val = SUCCESS;

	/* Write 1 to the gpio data register to enable LCD power. */
	ret_val = set_gpio_output(GPIO_NUM_VDD, 1);
	if (SUCCESS != ret_val) {
#ifdef OMAP3530_DEBUG
		DEBUG("%s:%d - GPIO setoutput failed.\r\n", __FILE__, __LINE__);
#endif
		return ret_val;
	}
#ifdef OMAP3530_DEBUG
	DEBUG("DSS disabled lcd power.\r\n");
#endif
	return ret_val;
}

/* Configure the LCD. */
status_t configure_lcd(lcd_drv_struct_t * lcd_init_struct)
{
	if (lcd_init_struct->mode == VGA) {
		lcd_height = 640;
		lcd_width = 480;
	} else {
		lcd_height = 320;
		lcd_width = 240;
	}

	/* Enable the clock for DSS. */
	dss_clock_init();

	/* Reset the display controller. */
	reset_display_controller();

	/* Pin and Pad settings for LCD. */
	mux_setup_lcd_bk_vdd();
	mux_setup_lcd();

	/* Enable backlight and power to LCD. */
	enable_lcd_power();

	gpio_pin_init(GPIO_NUM_QVGA_nVGA, 0);
	if (lcd_init_struct->mode == VGA)
		set_gpio_output(GPIO_NUM_QVGA_nVGA, 0);
	else
		set_gpio_output(GPIO_NUM_QVGA_nVGA, 1);

	/* Disable RESB. */
	disable_lcd_reset();

	/* Enable CLK, HSYNC, VSYNC. */
	enable_lcd_HVIF();

	/* Enable RESB. */
	enable_lcd_reset();

	/* Enable power on control. */
	enable_INI();

	/* Select the scanning directions. */
	configure_vert_scan_direction(CONV_SCAN_DIRECTION);
	configure_horiz_scan_direction(CONV_SCAN_DIRECTION);

	/* Configure the DSS registers. */
	configure_dss(lcd_init_struct->mode);

	/* Enable replication logic  */
	enable_replication_logic(GFX_PIPELINE);

	/* Display data on LCD. */
	display_lcd_image();

	return SUCCESS;
}

/* Fill the entire frame buffer with the given colour. */
void fill_colour(U16 * image_addr, U16 colour)
{
	U16 xpixel;
	U16 ypixel;

	for (ypixel = 0; ypixel < lcd_height; ypixel++) {
		for (xpixel = 0; xpixel < lcd_width; xpixel++) {
			image_addr[ypixel * lcd_width + xpixel] = colour;
		}
	}
}

/* Fill the entire frame buffer with the given colour. */
void fill_colour_32(U32 * image_addr, U32 colour)
{
	U32 xpixel;
	U32 ypixel;

	for (ypixel = 0; ypixel < lcd_height; ypixel++) {
		for (xpixel = 0; xpixel < lcd_width; xpixel++) {
			image_addr[ypixel * lcd_width + xpixel] = colour;
		}
	}
}
