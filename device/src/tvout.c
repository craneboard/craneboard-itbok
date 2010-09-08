/*
 * tvout.c: OMAP TV CVBS related routines
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
#include <common.h>
#include "types.h"
#include "omap3530evm.h"
#include "dg_gpio.h"
#include "triton2_utils.h"
#include "i2c_utils.h"
#include "i2c_dis.h"
#include "i2c_drv.h"
#include "tvout_drv.h"
#include "tvout_dis.h"
#include "tvout.h"
#include "dss.h"

extern U32 enable_triton2_vdac(void);
extern U32 disable_triton2_vdac(void);
extern void config_dss_for_tv(void);
extern void tv_image(void);
extern void reset_dss(void);
extern void pal_bdghi(void);
extern void pal_nc(void);
extern void pal_n(void);
extern void pal_m(void);
extern void pal_60(void);
extern void ntsc_m(void);
extern void ntsc_j(void);
extern void ntsc_443(void);

/*-----------------------------------------------------------------------------
| Function    :S32 enable_power
+------------------------------------------------------------------------------
| Description :Enables power for VENC voltages from Power (T2) chip.
|
| Parameters  :void
|
| Returns     :S32 success or failure
+-----------------------------------------------------------------------------*/
S32 enable_power(void)
{
	S32 ret_val;

	/* When M1 is the power chip, VREF is provided by M1 and VDAC is given
	 * by on board power supply. When T2 is the power chip, VDAC is provided 
	 * by T2 and VREF is given by OMAP3530 itself internally.
	 * If T2 is present,  VDAC is provided from T2 and 
	 * VREF is driver by OMAP3530 internally 
	 */
	ret_val = enable_triton2_vdac();
	if (ret_val != SUCCESS) {
		printf("VDAC Enable failed\n");
		return ret_val;
	}
	return ret_val;
}

/*-----------------------------------------------------------------------------
| Function    :S32 disable_power
+------------------------------------------------------------------------------
| Description :This function disable the voltage to VENC (M1 or T2) chip.
|
| Parameters  :void
|
| Returns     :S32 success or failure
+-----------------------------------------------------------------------------*/
S32 disable_power(void)
{
	S32 ret_val;

	/* Disable the VDAC from T2 */
	ret_val = disable_triton2_vdac();
	if (ret_val != SUCCESS) {
		printf("VDAC Enable failed\n");
		return ret_val;
	}
	return ret_val;
}

/*-----------------------------------------------------------------------------
| Function    :S32 tv_config
+------------------------------------------------------------------------------
| Description :This function performs the initializations required for the TV .
|
| Parameters  : U8 video_std,U8 image_cap_flag,U16 gfx_x,U16 gfx_y
|			,U16 gfx_ppl,U16 gfx_lpf,U16 dig_ppl,U32 dig_lpf,U32 image_addr
|
| Returns     :S32 success or failure
+-----------------------------------------------------------------------------*/
S32 tv_config(U8 video_std, U8 image_cap_flag, U16 gfx_x, U16 gfx_y, U16 gfx_ppl, U16 gfx_lpf, U16 dig_ppl, U32 dig_lpf, U32 image_addr)
{
	S32 ret_val = SUCCESS;
	U32 i;

	enable_power();		/* Enable volages to VENC */

	/* configure display subsystem */
	config_dss_for_tv();

	/* Configure the Video Encoder for different TV standards */
	ret_val = config_video_encoder(video_std);	/*argument tv standard */
	if (ret_val != SUCCESS) {
		printf("Tv standard not found\n");
		return ret_val;
	}

	if (image_cap_flag == 0) {	/* for colobar generation and filling color */
		for (i = 0; i < 1000000; i++) ;	/* delay added to remove the black bars */
	}

	if (image_cap_flag == 1) {
		/* configure display controller */
		config_display_controller(gfx_x, gfx_y, gfx_ppl, gfx_lpf, dig_ppl, dig_lpf, image_addr);
		/* enable the image display */
		tv_image();
	}
	return ret_val;
}

/*-----------------------------------------------------------------------------
| Function    :S32 tv_deconfig
+------------------------------------------------------------------------------
| Description :This function reset the DSS and disable the VREF and VDD_ADAC signals.
|
| Parameters  :void
|
| Returns     :S32 success or failure
+-----------------------------------------------------------------------------*/
S32 tv_deconfig(void)
{
	S32 ret_val = SUCCESS;

	/* reset the display subsystem */
	reset_dss();

	/* Disable the voltages */
	disable_power();

	return ret_val;
}

/*-----------------------------------------------------------------------------
| Function    :S32 config_video_encoder
+------------------------------------------------------------------------------
| Description : Configures video encoder for different TV standards.
|
| Parameters  : U8 video_std
|
| Returns     :S32 success or failure
+-----------------------------------------------------------------------------*/
S32 config_video_encoder(U8 video_std)
{
	if (video_std == PAL_BDGHI) {
		pal_bdghi();
	} else if (video_std == PAL_NC) {
		pal_nc();
	} else if (video_std == PAL_N) {
		pal_n();
	} else if (video_std == PAL_M) {
		pal_m();
	} else if (video_std == PAL_60) {
		pal_60();
	} else if (video_std == NTSC_M) {
		ntsc_m();
	} else if (video_std == NTSC_J) {
		ntsc_j();
	} else if (video_std == NTSC_443) {
		ntsc_443();
	} else {
		return FAILURE;
	}
	return SUCCESS;
}
