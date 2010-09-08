/*
 * dss.c: TV out OMAP Display driver routines
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
#include "omap3530evm.h"
#include "types.h"
#include "i2c_drv.h"
#include "i2c_utils.h"
#include "tvout_dis.h"
#include "tvout_drv.h"
#include "lcd_utils.h"
#include "dss.h"
#include "tvout.h"
#include "tvout_dis.h"

extern unsigned char tv_mode;

/* LCD routines are in lcd-utils.c */

S16 color_convbt601_full[][3] = {
	{298, 409, 0},
	{298, -208, -11},
	{298, 0, 517},
};

/*functions common for lcd and tv*/
/*-----------------------------------------------------------------------------
| Function    : void reset_display_controller
+------------------------------------------------------------------------------
| Description : This function resets the display controller for the  tv tests.
|
| Parameters  : void
|
| Returns     : void
+-----------------------------------------------------------------------------*/
void reset_display_controller_tv(void)
{
	U32 reg_val;

	/* set the Display Controller */
	reg_val = (DISPC_SYSCONFIG_SOFTRESET | DISPC_SYSCONFIG_SIDLEMODE_NIDLE | DISPC_SYSCONFIG_MIDLEMODE_NSTANDBY);
	out_regl(DISPC_SYSCONFIG, reg_val);

	/*DISPC_SYSSTATUS Read the Reset Status Register */
	while (!((reg_val = in_regl(DISPC_SYSSTATUS)) & DISPC_SYSSTATUS_RESETDONE)) ;
}

/*-----------------------------------------------------------------------------
| Function    : void enable_dss_clock_tv
+------------------------------------------------------------------------------
| Description : This function enables the DSS clocks .
|
| Parameters  : void
|
| Returns     : void
+-----------------------------------------------------------------------------*/
void enable_dss_clock_tv(void)
{
	U32 reg_val;

	/* Enable the functional clocks for the DSS1,DSS2 and TV OUT */
	reg_val = in_regl(CM_FCLKEN1_CORE);
	reg_val |= (DSS_CLK1_ENABLE | DSS_CLK2_ENABLE | TVOUT_CLK_ENABLE);
	out_regl(CM_FCLKEN1_CORE, reg_val);

	/* Enable the interface clock for the DSS */
	reg_val = in_regl(CM_ICLKEN1_CORE);
	reg_val |= DSS_INTERFACE_CLOCK_ENABLE;
	out_regl(CM_ICLKEN1_CORE, reg_val);
}

/*-----------------------------------------------------------------------------
| Function    : void config_gfx_dma
+------------------------------------------------------------------------------
| Description : This function configure the Graphics DMA
|
| Parameters  : U32 image_buffer
|
| Returns     : void
+-----------------------------------------------------------------------------*/
void config_gfx_dma(U32 image_buffer)
{
	U32 reg_val;

	/* Enable Graphics pipeline */
	out_regl(DISPC_GFX_ATTRIBUTES, DISPC_GFX_ATTRIBUTES_GFXENABLE);

	/* Enable Graphics channel out */
	reg_val = in_regl(DISPC_GFX_ATTRIBUTES);
	reg_val |= DISPC_GFX_CHANNEL_OUT_ENABLE;
	out_regl(DISPC_GFX_ATTRIBUTES, reg_val);

	/* Configure the image address 0 */
	out_regl(DISPC_GFX_BA0, image_buffer);

	/* Configure the image address 1 */
	out_regl(DISPC_GFX_BA1, image_buffer);

	/* Configure the FIFO threshold */
	reg_val = (DISPC_FIFO_THRESHOLD_HIGH << DISPC_GFX_FIFO_THRESHOLD_HIGH_SHIFT) | (DISPC_FIFO_THRESHOLD_HIGH << DISPC_GFX_FIFO_THRESHOLD_LOW_SHIFT);
	out_regl(DISPC_GFX_FIFO_THRESHOLD, reg_val);

}

/*-----------------------------------------------------------------------------
| Function    : void config_gfx_window
+------------------------------------------------------------------------------
| Description : This function configure the Graphics Window
|
| Parameters  : U16 x_position
|				U16 y_position
|				U16 size_x
|				U16 size_y
|
| Returns     : void
+-----------------------------------------------------------------------------*/
void config_gfx_window(U16 x_position, U16 y_position, U16 size_x, U16 size_y)
{
	U32 reg_val;

	/* Graphics format RGB16 */
	reg_val = in_regl(DISPC_GFX_ATTRIBUTES);
	reg_val |= DISPC_GFX_RGB16;
	out_regl(DISPC_GFX_ATTRIBUTES, reg_val);

	/* Graphics burst size */
	reg_val = in_regl(DISPC_GFX_ATTRIBUTES);
	reg_val |= DISPC_GFX_BURST_SIZE;
	out_regl(DISPC_GFX_ATTRIBUTES, reg_val);

	/* Configure the image position */
	reg_val = ((y_position) << GFX_Y_POSITION) | ((x_position));
	out_regl(DISPC_GFX_POSITION, reg_val);

	/* Configure the image size */
	reg_val = ((size_y - 1) << GFX_Y_POSITION) | ((size_x - 1));
	out_regl(DISPC_GFX_SIZE, reg_val);

	/* Enable Graphics layer capable */
	out_regl(DISPC_CAPABLE, DISPC_GFX_LAYER_CAPABLE);

}

/*-----------------------------------------------------------------------------
| Function    : void config_digital_out
+------------------------------------------------------------------------------
| Description : This function configures Digital out for  the TV
|
| Parameters  : U16 no_of_pixels
|				U16 no_of_lines
|
| Returns     : void
+-----------------------------------------------------------------------------*/
void config_digital_out(U16 no_of_pixels, U32 no_of_lines)
{
	U32 reg_val;

	/* Define the Display window size */
	reg_val = ((no_of_lines - 1) << DIGITAL_LINES_PER_PANEL) | ((no_of_pixels - 1));
	out_regl(DISPC_SIZE_DIG, reg_val);

	reg_val = in_regl(DISPC_CONTROL);
	reg_val |= DIGITAL_OUT_HOLD_TIME;
	out_regl(DISPC_CONTROL, reg_val);
}

/*-----------------------------------------------------------------------------
| Function    : void digital_go
+------------------------------------------------------------------------------
| Description : This function send the Digital go
|
| Parameters  : void
|
| Returns     : void
+-----------------------------------------------------------------------------*/
void digital_go(void)
{
	U32 reg_val;

	/*  Set the Digital GO command and Enable Digital out */
	reg_val = in_regl(DISPC_CONTROL);
	reg_val |= (GO_DIGITAL | DIGITAL_OUT_ENABLE);
	out_regl(DISPC_CONTROL, reg_val);

	/* Wait till the register values are taken from the shadow registers */
	reg_val = in_regl(DISPC_CONTROL);
	while (!(reg_val & GO_DIGITAL)) {
		reg_val = in_regl(DISPC_CONTROL);
	}
}

/*-----------------------------------------------------------------------------
| Function    : void config_display_controller
+------------------------------------------------------------------------------
| Description : This function configure Display controller for displating the image on TV.
|
| Parameters  : U16 gfx_x,U16 gfx_y,U16 gfx_ppl,U16 gfx_lpf,
|				U16 dig_ppl,U32 dig_lpf
|
| Returns     : void
+-----------------------------------------------------------------------------*/
void config_display_controller(U16 gfx_x, U16 gfx_y, U16 gfx_ppl, U16 gfx_lpf, U16 dig_ppl, U32 dig_lpf, U32 image_buffer)
{
	/* U32 image_buffer=(U32)(0x80365000); */
	/* Clear the interrupt status */
	out_regl(DISPC_IRQSTATUS, DISPC_CLEAR_INTERRUPT_STATUS);

	/* Disable the Interrupts */
	out_regl(DISPC_IRQENABLE, DISPC_MASK_ALL_INTERRUPTS);

	/* Frame data only loaded every frame */
	out_regl(DISPC_CONFIG, DISPC_CONFIG_LOADMODE_FRDATLEFR);

	/* Configure Graphics pipeline DMA */
	config_gfx_dma(image_buffer);

	/* Configure Graphics pipeline window */
	config_gfx_window(gfx_x, gfx_y, gfx_ppl, gfx_lpf);

	/* Configuration specific to TV out */
	config_digital_out(dig_ppl, dig_lpf);

	/* Row increment settings. Since the Window size is 360, we are skipping few information from camera buffer (640 pixel per line) */
	out_regl(DISPC_GFX_ROW_INC, 0x0231);	// row increment, 0x231 == ((280 * 2) + 1), where 280 == (640 - 360), and each pixel is 2 bytes

	/* Enable Digital out GO */
	digital_go();
}

/*-----------------------------------------------------------------------------
| Function    : void reset_dss
+------------------------------------------------------------------------------
| Description : This function resets the display sub system
|
| Parameters  : void
|
| Returns     : void
+-----------------------------------------------------------------------------*/
void reset_dss(void)
{
	U32 reg_val;

	reset_display_controller_tv();
	reset_video_encoder();

	/* Reset the Display Subsystem control register which is configured for Tv Out test
	   Reset to use the DPLL as clock */
	reg_val = in_regl(DSS_CONTROL);

	if (1)			// (companion_board_type() == POWER_T2)
	{
		/* Disable the VREF from OMAP itself by setting the Power Bandgap bit */
		/* This is required when T2 boards is present, since T2 does not provided the VREF voltage */
		if (tv_mode == 0) {
			reg_val &= ~(DSS_ENABLE_APLL | DSS_VENC_4X_ENABLE | DSS_DACDEMEN_ENABLE | DSS_PWRDNBGZ);
		} else {
			reg_val &= ~(DSS_VENC_OUT_SEL | DSS_ENABLE_APLL | DSS_VENC_4X_ENABLE | DSS_DACDEMEN_ENABLE | DSS_PWRDNBGZ);
		}
	} else {
		reg_val &= ~(DSS_ENABLE_APLL | DSS_VENC_4X_ENABLE | DSS_DACDEMEN_ENABLE);
	}
	out_regl(DSS_CONTROL, reg_val);
}

/*-----------------------------------------------------------------------------
| Function    : void config_dss_for_tv
+------------------------------------------------------------------------------
| Description : This  funciton calls the funcitons to configure DSS for the tv
|				Display.
|
| Parameters  : void
|
| Returns     : void
+-----------------------------------------------------------------------------*/
void config_dss_for_tv(void)
{
	U32 reg_val;
	/* enable the clocks for DSS Module */
	enable_dss_clock_tv();

	/* Reset the Display Controller */
	reset_display_controller_tv();

	/*  Configure DSS for TV tests */
	/*  Enable the VENC clock 4x mode, switch the clock to APLL mode and
	   enable DAC dynamic matching element to enhance TV color display. */
	if (1)			//(companion_board_type() == POWER_T2)
	{
		/* Enable the VREF from OMAP internally by setting the Power Bandgap bit */
		if (tv_mode == 0) {
			reg_val = (DSS_ENABLE_APLL | DSS_VENC_4X_ENABLE | DSS_DACDEMEN_ENABLE | DSS_PWRDNBGZ);
		} else {
			reg_val = (DSS_VENC_OUT_SEL | DSS_ENABLE_APLL | DSS_VENC_4X_ENABLE | DSS_DACDEMEN_ENABLE | DSS_PWRDNBGZ);
		}
	} else {
		reg_val = (DSS_ENABLE_APLL | DSS_VENC_4X_ENABLE | DSS_DACDEMEN_ENABLE);
	}
	out_regl(DSS_CONTROL, reg_val);

	/* reset the video encoder */
	reset_video_encoder();
}

/*==== Video Encoder Functions ==========================================================*/
/*-----------------------------------------------------------------------------
| Function    : void colorbar_generate
+------------------------------------------------------------------------------
| Description : This function sets the encoder for displaying the colorba on
|				the TV screen.
|
| Parameters  : void
|
| Returns     : void
+-----------------------------------------------------------------------------*/
void tv_colorbar_generate(void)
{
	/* Enable the free running mode of the VENC */
	out_regl(SYNC_CONTROL, FREE_RUNNING_ENABLE);

	/* Generate Color Bar */
	out_regl(F_CONTROL, COLOR_BAR_ENABLE);
}

/*-----------------------------------------------------------------------------
| Function    : void colorbar_generate
+------------------------------------------------------------------------------
| Description : This function fills the TV screen with the color specified by the
|				user.
|
| Parameters  : U8 color -This parameter specifies the color to be filled .
|
| Returns     : void
+-----------------------------------------------------------------------------*/
void tv_fillcolor(U32 color)
{
	U32 reg_val;

	/* Enable the free running mode of the VENC */
	out_regl(SYNC_CONTROL, FREE_RUNNING_ENABLE);

	/* Enable background color */
	out_regl(F_CONTROL, BACKGROUND_COLOR_ENABLE);

	/* Enable the selected color */
	reg_val = in_regl(F_CONTROL);
	reg_val |= color;
	out_regl(F_CONTROL, reg_val);
}

/*-----------------------------------------------------------------------------
| Function    : void tv_image
+------------------------------------------------------------------------------
| Description : This function configure the VENC for external data from Display
|				Controller.
|
| Parameters  : U8 color -This parameter specifies the color to be filled .
|
| Returns     : void
+-----------------------------------------------------------------------------*/
void tv_image(void)
{
	/* Disable the free running mode of the VENC */
	out_regl(SYNC_CONTROL, FREE_RUNNING_DISABLE);

	/* Select Video source as Display controller */
	out_regl(F_CONTROL, ENABLE_EXTERNAL_VIDEO);
}

/*-----------------------------------------------------------------------------
| Function    : void reset_video_encoder
+------------------------------------------------------------------------------
| Description : This function Resets the Video Encoder
|
| Parameters  : void
|
| Returns     : void
+-----------------------------------------------------------------------------*/
void reset_video_encoder(void)
{
	U32 reg_val;
	/* Reset the VENC */
	out_regl(F_CONTROL, VENC_RESET_ENCODER);

	/* wait in a loop till the encoder resets */
	reg_val = in_regl(F_CONTROL);
	while (reg_val & VENC_RESET_DONE) {	/* bit is automatically set to Zero */
		reg_val = in_regl(F_CONTROL);
	}

	/* set the video encoder output clock */
	/* Set VENC output clock as 54 MHz */
	out_regl(VIDOUT_CTRL, VENC_54MHx_CLK);

	/* Resetting the  register */
	out_regl(SYNC_CONTROL, 0x00000000);

	/* Resetting the  register */
	out_regl(F_CONTROL, 0x00000000);
}

/*-----------------------------------------------------------------------------
| Function    :S32 pal_bdghi
+------------------------------------------------------------------------------
| Description :This function makes the necessary settings for the
|            PAL BDGHI standard.
|
| Parameters  :void
|
| Returns     :S32 success or failure
+-----------------------------------------------------------------------------*/
void pal_bdghi(void)
{

	/* register settings for PAL BDGHI starts */

	/* total number of pixels in a scan line */
	out_regl(VENC_LLEN, (VENC_LLEN_PAL | VENC_LLEN_EN));

	/* total number of lines in a frame from source image */
	out_regl(VENC_FLENS, VENC_FLENS_PAL);

	/* enable the chrominance interpolation filter and luminance interpolation filters */
	out_regl(VENC_HFLTR_CTRL, VENC_HFLTR_CTRL_EN);

	/* wide screen signalling and close caption control */
	out_regl(VENC_CC_CARR_WSS_CARR, VENC_CC_CARR_WSS_CARR_PAL);

	/*CHOOSE THE PHASE OF THE ENCODED VIDEO COLOR SUBCARRIER RELATIVE TO H-SYNC */
	out_regl(VENC_C_PHASE, VENC_C_PHASE_PAL);

	/*gain control for Cb signal */
	out_regl(VENC_GAIN_U, VENC_GAIN_U_PAL_BDGHI);

	/* gain control for Cr signal */
	out_regl(VENC_GAIN_V, VENC_GAIN_V_PAL_BDGHI);

	/* gain control for Y signal */
	out_regl(VENC_GAIN_Y, VENC_GAIN_Y_PAL_BDGHI);

	/* BLACK LEVEL SETTING */
	out_regl(VENC_BLACK_LEVEL, VENC_BLACK_LEVEL_PAL_BDGHI);

	/* BLACK LEVEL SETTING */
	out_regl(VENC_BLANK_LEVEL, VENC_BLANK_LEVEL_PAL_BDGHI);

	/* X_COLOR */
	out_regl(VENC_X_COLOR, VENC_X_COLOR_VAL);

	/*enable phase alternation line encoding */
	out_regl(VENC_M_CONTROL, VENC_M_CONTROL_PAL);

	/* the ITU-R 601 sampling rate and the amplitude of the color burst are set. */
	out_regl(VENC_BSTAMP_WSS_DATA, VENC_BSTAMP_WSS_DATA_PAL_BDGHI);

	/*COLOR SUBCARRIER FREQUENCY  */
	out_regl(VENC_S_CARR, VENC_S_CARR_PAL_BDGHI);

	/*CLOSED CAPTION DATA */
	out_regl(VENC_LINE21, VENC_LINE21_VAL);

	/* line selection for closed caption data are encoded */
	out_regl(VENC_LN_SEL, VENC_LN_SEL_VAL);

	/* L21 and WC related setting */
	out_regl(VENC_L21_WC_CTL, VENC_L21_WC_CTL_PAL);

	/* horizontal and vertical trigger phase reference */
	out_regl(VENC_HTRIGGER_VTRIGGER, VENC_HTRIGGER_VTRIGGER_VAL);

	/* the starting and ending positions of the active video on a horizontal line */
	out_regl(VENC_SAVID_EAVID, VENC_SAVID_EAVID_PAL);

	/* specify the first active line of Field and Field length */
	out_regl(VENC_FLEN_FAL, VENC_FLEN_FAL_PAL);

	/* mention the phase reset mode(reset every eight fields) and the last active line of Field */
	out_regl(VENC_LAL_PHASE_RESET, VENC_LAL_PHASE_RESET_PAL);

	/* specify the HSYNC internal start and stop pixel values */
	out_regl(VENC_HS_INT_START_STOP_X, VENC_HS_INT_START_STOP_X_PAL);

	/* specify the HSYNC external start and stop pixel values */
	out_regl(VENC_HS_EXT_START_STOP_X, VENC_HS_EXT_START_STOP_X_PAL);

	/* specify the VSYNC internal start PIXEL  values */
	out_regl(VENC_VS_INT_START_X, VENC_VS_INT_START_X_PAL);

	/* specify the VSYNC internal start line and stop pixel values */
	out_regl(VENC_VS_INT_STOP_X_VS_INT_START_Y, VENC_VS_INT_STOP_X_VS_INT_START_Y_PAL);

	/* specify the VSYNC internal stop line and external start pixel values */
	out_regl(VENC_VS_INT_STOP_Y_VS_EXT_START_X, VENC_VS_INT_STOP_Y_VS_EXT_START_X_PAL);

	/* specify the VSYNC external start line and stop pixel values */
	out_regl(VENC_VS_EXT_STOP_X_VS_EXT_START_Y, VENC_VS_EXT_STOP_X_VS_EXT_START_Y_PAL);

	/* specify the VSYNC external stop line values */
	out_regl(VENC_VS_EXT_STOP_Y, VENC_VS_EXT_STOP_Y_PAL);

	/* define AVID start and stop pixel values */
	out_regl(VENC_AVID_START_STOP_X, VENC_AVID_START_STOP_X_PAL);

	/* define AVID start and stop line values */
	out_regl(VENC_AVID_START_STOP_Y, VENC_AVID_START_STOP_Y_PAL);

	/* define FID internal start pixel and line values */
	out_regl(VENC_FID_INT_START_X_FID_INT_START_Y, VENC_FID_INT_START_X_FID_INT_START_Y_PAL);

	/* define FID external start pixel and internal offset line values */
	out_regl(VENC_FID_INT_OFFSET_Y_FID_EXT_START_X, VENC_FID_INT_OFFSET_Y_FID_EXT_START_X_PAL);

	/* define FID external start line and external offset line values */
	out_regl(VENC_FID_EXT_START_Y_FID_EXT_OFFSET_Y, VENC_FID_EXT_START_Y_FID_EXT_OFFSET_Y_GEN);

	/* TVDETGP generation is disabled and SYNC_POLARITY and UVPHASE_POL polarity selection */
	out_regl(VENC_GEN_CTRL, VENC_GEN_CTRL_PAL_VAL);

	/* Enable the DAC */
	if (tv_mode == 0)
		out_regl(VENC_DAC_TST, 0x0000000A);
	else
		out_regl(VENC_DAC_TST, 0x0000000D);

}

/*-----------------------------------------------------------------------------
| Function    :S32 pal_n
+------------------------------------------------------------------------------
| Description :This function makes the necessary settings for the PAL N standard.
|
| Parameters  :void
|
| Returns     :S32 success or failure
+-----------------------------------------------------------------------------*/
void pal_n(void)
{
	U32 reg_val;
	pal_bdghi();

	/*  register settings for PAL N starts */

	/* gain control for Cb signal */
	out_regl(VENC_GAIN_U, VENC_GAIN_U_PAL_N);

	/* gain control for Cr signal */
	out_regl(VENC_GAIN_V, VENC_GAIN_V_PAL_N);

	/* gain control for Y signal */
	out_regl(VENC_GAIN_Y, VENC_GAIN_Y_PAL_N);

	/* BLACK LEVEL SETTING */
	out_regl(VENC_BLACK_LEVEL, VENC_BLACK_LEVEL_PAL_N);

	/* BLACK LEVEL SETTING */
	out_regl(VENC_BLANK_LEVEL, VENC_BLANK_LEVEL_PAL_N);

	reg_val = in_regl(VENC_M_CONTROL);
	reg_val |= 0x00000040;	/* enable the pal n bit */
	out_regl(VENC_M_CONTROL, reg_val);
}

/*-----------------------------------------------------------------------------
| Function    :pal_m
+------------------------------------------------------------------------------
| Description :This function makes the necessary settings for the PAL M standard.
|
| Parameters  :void
|
| Returns     :S32 success or failure
+-----------------------------------------------------------------------------*/
void pal_m(void)
{
	/*  register settings for PAL M starts */

	/* total number of pixels in a scan line */
	out_regl(VENC_LLEN, (VENC_LLEN_PAL_M | VENC_LLEN_EN));

	/* total number of lines in a frame from source image */
	out_regl(VENC_FLENS, VENC_FLENS_PAL_M);

	/* enable the chrominance interpolation filter and luminance interpolation filters */
	out_regl(VENC_HFLTR_CTRL, VENC_HFLTR_CTRL_EN);

	/* wide screen signalling and close caption control */
	out_regl(VENC_CC_CARR_WSS_CARR, VENC_CC_CARR_WSS_CARR_GEN);

	/* CHOOSE THE PHASE OF THE ENCODED VIDEO COLOR SUBCARRIER RELATIVE TO H-SYNC */
	out_regl(VENC_C_PHASE, VENC_C_PHASE_GEN);

	/* gain control for Cb signal */
	out_regl(VENC_GAIN_U, VENC_GAIN_U_PAL_M);

	/* gain control for Cr signal */
	out_regl(VENC_GAIN_V, VENC_GAIN_V_PAL_M);

	/* gain control for Y signal */
	out_regl(VENC_GAIN_Y, VENC_GAIN_Y_PAL_M);

	/* BLACK LEVEL SETTING */
	out_regl(VENC_BLACK_LEVEL, VENC_BLACK_LEVEL_PAL_M);

	/* BLACK LEVEL SETTING */
	out_regl(VENC_BLANK_LEVEL, VENC_BLANK_LEVEL_PAL_M);

	/* X_COLOR */
	out_regl(VENC_X_COLOR, VENC_X_COLOR_VAL);

	/* enable phase alternation line encoding */
	out_regl(VENC_M_CONTROL, VENC_M_CONTROL_PAL_M);

	/*  the ITU-R 601 sampling rate and the amplitude of the color burst are set. */
	out_regl(VENC_BSTAMP_WSS_DATA, VENC_BSTAMP_WSS_DATA_PAL_M);

	/* COLOR SUBCARRIER FREQUENCY */
	out_regl(VENC_S_CARR, VENC_S_CARR_PAL_M);

	/* CLOSED CAPTION DATA */
	out_regl(VENC_LINE21, VENC_LINE21_VAL);

	/* line selection for closed caption data are encoded */
	out_regl(VENC_LN_SEL, VENC_LN_SEL_VAL);

	/* L21 and WC related setting */
	out_regl(VENC_L21_WC_CTL, VENC_L21_WC_CTL_GEN);

	/* horizontal and vertical trigger phase reference */
	out_regl(VENC_HTRIGGER_VTRIGGER, VENC_HTRIGGER_VTRIGGER_VAL);

	/* the starting and ending positions of the active video on a horizontal line */
	out_regl(VENC_SAVID_EAVID, VENC_SAVID_EAVID_GEN);
	/* specify the first active line of Field and Field length */
	out_regl(VENC_FLEN_FAL, VENC_FLEN_FAL_GEN);

	/* mention the phase reset mode(reset every eight fields) and the last active line of Field */
	out_regl(VENC_LAL_PHASE_RESET, VENC_LAL_PHASE_RESET_PAL_2);

	/* specify the HSYNC internal start and stop pixel values */
	out_regl(VENC_HS_INT_START_STOP_X, VENC_HS_INT_START_STOP_X_PAL_2);

	/* specify the HSYNC external start and stop pixel values */
	out_regl(VENC_HS_EXT_START_STOP_X, VENC_HS_EXT_START_STOP_X_GEN);

	/* specify the VSYNC internal start PIXEL  values */
	out_regl(VENC_VS_INT_START_X, VENC_VS_INT_START_X_GEN);

	/* specify the VSYNC internal start line and stop pixel values */
	out_regl(VENC_VS_INT_STOP_X_VS_INT_START_Y, VENC_VS_INT_STOP_X_VS_INT_START_Y_PAL_2);

	/* specify the VSYNC internal stop line and external start pixel values */
	out_regl(VENC_VS_INT_STOP_Y_VS_EXT_START_X, VENC_VS_INT_STOP_Y_VS_EXT_START_X_PAL_2);

	/* specify the VSYNC external start line and stop pixel values */
	out_regl(VENC_VS_EXT_STOP_X_VS_EXT_START_Y, VENC_VS_EXT_STOP_X_VS_EXT_START_Y_GEN);

	/* specify the VSYNC external stop line values */
	out_regl(VENC_VS_EXT_STOP_Y, VENC_VS_EXT_STOP_Y_GEN);

	/* define AVID start and stop pixel values */
	out_regl(VENC_AVID_START_STOP_X, VENC_AVID_START_STOP_X_PAL_2);

	/* define AVID start and stop line values */
	out_regl(VENC_AVID_START_STOP_Y, VENC_AVID_START_STOP_Y_PAL_2);

	/* define FID internal start pixel and line values */
	out_regl(VENC_FID_INT_START_X_FID_INT_START_Y, VENC_FID_INT_START_X_FID_INT_START_Y_GEN);

	/* define FID external start pixel and internal offset line values */
	out_regl(VENC_FID_INT_OFFSET_Y_FID_EXT_START_X, VENC_FID_INT_OFFSET_Y_FID_EXT_START_X_GEN);

	/* define FID external start line and external offset line values */
	out_regl(VENC_FID_EXT_START_Y_FID_EXT_OFFSET_Y, VENC_FID_EXT_START_Y_FID_EXT_OFFSET_Y_GEN);

	/* TVDETGP generation is disabled and SYNC_POLARITY and UVPHASE_POL polarity selection */
	out_regl(VENC_GEN_CTRL, VENC_GEN_CTRL_VAL);

	/* Enable the DAC */
	if (tv_mode == 0)
		out_regl(VENC_DAC_TST, 0x0000000A);
	else
		out_regl(VENC_DAC_TST, 0x0000000D);

}

/*-----------------------------------------------------------------------------
| Function    :S32 pal_nc
+------------------------------------------------------------------------------
| Description :This function makes the necessary settings for the PAL Nc standard.
|
| Parameters  :void
|
| Returns     :S32 success or failure
+-----------------------------------------------------------------------------*/
void pal_nc(void)
{

	/*  register settings for PAL Nc starts */

	/* total number of pixels in a scan line */
	out_regl(VENC_LLEN, (VENC_LLEN_PAL | VENC_LLEN_EN));

	/* total number of lines in a frame from source image */
	out_regl(VENC_FLENS, VENC_FLENS_PAL);

	/* enable the chrominance interpolation filter and luminance interpolation filters */
	out_regl(VENC_HFLTR_CTRL, VENC_HFLTR_CTRL_EN);

	/* wide screen signalling and close caption control */
	out_regl(VENC_CC_CARR_WSS_CARR, VENC_CC_CARR_WSS_CARR_PAL);

	/* CHOOSE THE PHASE OF THE ENCODED VIDEO COLOR SUBCARRIER RELATIVE TO H-SYNC */
	out_regl(VENC_C_PHASE, VENC_C_PHASE_PAL_NC);

	/* gain control for Cb signal */
	out_regl(VENC_GAIN_U, VENC_GAIN_U_PAL_NC);

	/* gain control for Cr signal */
	out_regl(VENC_GAIN_V, VENC_GAIN_V_PAL_NC);

	/* gain control for Y signal */
	out_regl(VENC_GAIN_Y, VENC_GAIN_Y_PAL_NC);

	/* BLACK LEVEL SETTING */
	out_regl(VENC_BLACK_LEVEL, VENC_BLACK_LEVEL_PAL_NC);

	/* BLACK LEVEL SETTING */
	out_regl(VENC_BLANK_LEVEL, VENC_BLANK_LEVEL_PAL_NC);

	/* X_COLOR */
	out_regl(VENC_X_COLOR, VENC_X_COLOR_VAL);

	/* enable phase alternation line encoding */
	out_regl(VENC_M_CONTROL, VENC_M_CONTROL_PAL);

	/*  the ITU-R 601 sampling rate and the amplitude of the color burst are set. */
	out_regl(VENC_BSTAMP_WSS_DATA, VENC_BSTAMP_WSS_DATA_PAL_NC);

	/* COLOR SUBCARRIER FREQUENCY */
	out_regl(VENC_S_CARR, VENC_S_CARR_PAL_NC);

	/* CLOSED CAPTION DATA */
	out_regl(VENC_LINE21, VENC_LINE21_VAL);

	/* line selection for closed caption data are encoded */
	out_regl(VENC_LN_SEL, VENC_LN_SEL_VAL);

	/* L21 and WC related setting */
	out_regl(VENC_L21_WC_CTL, VENC_L21_WC_CTL_PAL);

	/* horizontal and vertical trigger phase reference */
	out_regl(VENC_HTRIGGER_VTRIGGER, VENC_HTRIGGER_VTRIGGER_VAL);

	/* the starting and ending positions of the active video on a horizontal line */
	out_regl(VENC_SAVID_EAVID, VENC_SAVID_EAVID_PAL);

	/* specify the first active line of Field and Field length */
	out_regl(VENC_FLEN_FAL, VENC_FLEN_FAL_PAL);

	/* mention the phase reset mode(reset every eight fields) and the last active line of Field */
	out_regl(VENC_LAL_PHASE_RESET, VENC_LAL_PHASE_RESET_PAL_NC);

	/* specify the HSYNC internal start and stop pixel values */
	out_regl(VENC_HS_INT_START_STOP_X, VENC_HS_INT_START_STOP_X_NC);

	/* specify the HSYNC external start and stop pixel values */
	out_regl(VENC_HS_EXT_START_STOP_X, VENC_HS_EXT_START_STOP_X_PAL);

	/* specify the VSYNC internal start PIXEL  values */
	out_regl(VENC_VS_INT_START_X, VENC_VS_INT_START_X_PAL);

	/* specify the VSYNC internal start line and stop pixel values */
	out_regl(VENC_VS_INT_STOP_X_VS_INT_START_Y, VENC_VS_INT_STOP_X_VS_INT_START_Y_PAL_NC);

	/* specify the VSYNC internal stop line and external start pixel values */
	out_regl(VENC_VS_INT_STOP_Y_VS_EXT_START_X, VENC_VS_INT_STOP_Y_VS_EXT_START_X_PAL_NC);

	/* specify the VSYNC external start line and stop pixel values */
	out_regl(VENC_VS_EXT_STOP_X_VS_EXT_START_Y, VENC_VS_EXT_STOP_X_VS_EXT_START_Y_PAL);

	/* specify the VSYNC external stop line values */
	out_regl(VENC_VS_EXT_STOP_Y, VENC_VS_EXT_STOP_Y_PAL);

	/* define AVID start and stop pixel values */
	out_regl(VENC_AVID_START_STOP_X, VENC_AVID_START_STOP_X_PAL_NC);

	/* define AVID start and stop line values */
	out_regl(VENC_AVID_START_STOP_Y, VENC_AVID_START_STOP_Y_PAL_NC);

	/* define FID internal start pixel and line values */
	out_regl(VENC_FID_INT_START_X_FID_INT_START_Y, VENC_FID_INT_START_X_FID_INT_START_Y_PAL);

	/* define FID external start pixel and internal offset line values */
	out_regl(VENC_FID_INT_OFFSET_Y_FID_EXT_START_X, VENC_FID_INT_OFFSET_Y_FID_EXT_START_X_PAL);

	/* define FID external start line and external offset line values */
	out_regl(VENC_FID_EXT_START_Y_FID_EXT_OFFSET_Y, VENC_FID_EXT_START_Y_FID_EXT_OFFSET_Y_PAL);

	/* TVDETGP generation is disabled and SYNC_POLARITY and UVPHASE_POL polarity selection */
	out_regl(VENC_GEN_CTRL, VENC_GEN_CTRL_VAL);

	/* Enable the DAC */
	if (tv_mode == 0)
		out_regl(VENC_DAC_TST, 0x0000000A);
	else
		out_regl(VENC_DAC_TST, 0x0000000D);

}

/*-----------------------------------------------------------------------------
| Function    :S32 pal_60
+------------------------------------------------------------------------------
| Description :This function makes the necessary settings for the PAL 60 standard.
|
| Parameters  :void
|
| Returns     :S32 success or failure
+-----------------------------------------------------------------------------*/
void pal_60(void)
{

	/*  register settings for PAL 60 starts */

	/* total number of pixels in a scan line */
	out_regl(VENC_LLEN, (VENC_LLEN_GEN | VENC_LLEN_EN));

	/* total number of lines in a frame from source image */
	out_regl(VENC_FLENS, VENC_FLENS_GEN);

	/* enable the chrominance interpolation filter and luminance interpolation filters */
	out_regl(VENC_HFLTR_CTRL, VENC_HFLTR_CTRL_EN);

	/* wide screen signalling and close caption control */
	out_regl(VENC_CC_CARR_WSS_CARR, VENC_CC_CARR_WSS_CARR_GEN);

	/* CHOOSE THE PHASE OF THE ENCODED VIDEO COLOR SUBCARRIER RELATIVE TO H-SYNC */
	out_regl(VENC_C_PHASE, VENC_C_PHASE_GEN);

	/* gain control for Cb signal */
	out_regl(VENC_GAIN_U, VENC_GAIN_U_PAL_60);

	/* gain control for Cr signal */
	out_regl(VENC_GAIN_V, VENC_GAIN_V_PAL_60);

	/* gain control for Y signal */
	out_regl(VENC_GAIN_Y, VENC_GAIN_Y_PAL_60);

	/* BLACK LEVEL SETTING */
	out_regl(VENC_BLACK_LEVEL, VENC_BLACK_LEVEL_PAL_60);

	/* BLACK LEVEL SETTING */
	out_regl(VENC_BLANK_LEVEL, VENC_BLANK_LEVEL_PAL_60);

	/* X_COLOR */
	out_regl(VENC_X_COLOR, VENC_X_COLOR_VAL);

	/* enable phase alternation line encoding */
	out_regl(VENC_M_CONTROL, VENC_M_CONTROL_PAL_60);

	/*  the ITU-R 601 sampling rate and the amplitude of the color burst are set. */
	out_regl(VENC_BSTAMP_WSS_DATA, VENC_BSTAMP_WSS_DATA_PAL_60);

	/* COLOR SUBCARRIER FREQUENCY */
	out_regl(VENC_S_CARR, VENC_S_CARR_PAL_60);

	/* CLOSED CAPTION DATA */
	out_regl(VENC_LINE21, VENC_LINE21_VAL);

	/* line selection for closed caption data are encoded */
	out_regl(VENC_LN_SEL, VENC_LN_SEL_VAL);

	/* L21 and WC related setting */
	out_regl(VENC_L21_WC_CTL, VENC_L21_WC_CTL_GEN);

	/* horizontal and vertical trigger phase reference */
	out_regl(VENC_HTRIGGER_VTRIGGER, VENC_HTRIGGER_VTRIGGER_VAL);

	/* the starting and ending positions of the active video on a horizontal line */
	out_regl(VENC_SAVID_EAVID, VENC_SAVID_EAVID_GEN);

	/* specify the first active line of Field and Field length */
	out_regl(VENC_FLEN_FAL, VENC_FLEN_FAL_GEN);

	/* mention the phase reset mode(reset every eight fields) and the last active line of Field */
	out_regl(VENC_LAL_PHASE_RESET, VENC_LAL_PHASE_RESET_PAL_2);

	/* specify the HSYNC internal start and stop pixel values */
	out_regl(VENC_HS_INT_START_STOP_X, VENC_HS_INT_START_STOP_X_PAL_2);

	/* specify the HSYNC external start and stop pixel values */
	out_regl(VENC_HS_EXT_START_STOP_X, VENC_HS_EXT_START_STOP_X_GEN);

	/* specify the VSYNC internal start PIXEL  values */
	out_regl(VENC_VS_INT_START_X, VENC_VS_INT_START_X_GEN);

	/* specify the VSYNC internal start line and stop pixel values */
	out_regl(VENC_VS_INT_STOP_X_VS_INT_START_Y, VENC_VS_INT_STOP_X_VS_INT_START_Y_PAL_2);

	/* specify the VSYNC internal stop line and external start pixel values */
	out_regl(VENC_VS_INT_STOP_Y_VS_EXT_START_X, VENC_VS_INT_STOP_Y_VS_EXT_START_X_PAL_2);

	/* specify the VSYNC external start line and stop pixel values */
	out_regl(VENC_VS_EXT_STOP_X_VS_EXT_START_Y, VENC_VS_EXT_STOP_X_VS_EXT_START_Y_GEN);

	/* specify the VSYNC external stop line values */
	out_regl(VENC_VS_EXT_STOP_Y, VENC_VS_EXT_STOP_Y_GEN);

	/* define AVID start and stop pixel valuesv */
	out_regl(VENC_AVID_START_STOP_X, VENC_AVID_START_STOP_X_PAL_2);

	/* define AVID start and stop line values */
	out_regl(VENC_AVID_START_STOP_Y, VENC_AVID_START_STOP_Y_PAL_2);

	/* define FID internal start pixel and line values */
	out_regl(VENC_FID_INT_START_X_FID_INT_START_Y, VENC_FID_INT_START_X_FID_INT_START_Y_GEN);

	/* define FID external start pixel and internal offset line values */
	out_regl(VENC_FID_INT_OFFSET_Y_FID_EXT_START_X, VENC_FID_INT_OFFSET_Y_FID_EXT_START_X_GEN);

	/* define FID external start line and external offset line values */
	out_regl(VENC_FID_EXT_START_Y_FID_EXT_OFFSET_Y, VENC_FID_EXT_START_Y_FID_EXT_OFFSET_Y_GEN);

	/* TVDETGP generation is disabled and SYNC_POLARITY and UVPHASE_POL polarity selection */
	out_regl(VENC_GEN_CTRL, VENC_GEN_CTRL_VAL);

	/* Enable the DAC */
	if (tv_mode == 0)
		out_regl(VENC_DAC_TST, 0x0000000A);
	else
		out_regl(VENC_DAC_TST, 0x0000000D);

}

/*-----------------------------------------------------------------------------
| Function    :S32 ntsc_m
+------------------------------------------------------------------------------
| Description :This function makes the necessary settings for the NTSC M standard.
|
| Parameters  :void
|
| Returns     :S32 success or failure
+-----------------------------------------------------------------------------*/
void ntsc_m(void)
{
	/* register settings for NTSC M starts */

	/* total number of pixels in a scan line */
	out_regl(VENC_LLEN, (VENC_LLEN_GEN | VENC_LLEN_EN));

	/* total number of lines in a frame from source image */
	out_regl(VENC_FLENS, VENC_FLENS_GEN);

	/* enable the chrominance interpolation filter and luminance interpolation filters */
	out_regl(VENC_HFLTR_CTRL, VENC_HFLTR_CTRL_EN);

	/* wide screen signalling and close caption control */
	out_regl(VENC_CC_CARR_WSS_CARR, VENC_CC_CARR_WSS_CARR_GEN);

	/* CHOOSE THE PHASE OF THE ENCODED VIDEO COLOR SUBCARRIER RELATIVE TO H-SYNC */
	out_regl(VENC_C_PHASE, VENC_C_PHASE_GEN);

	/* gain control for Cb signal */
	out_regl(VENC_GAIN_U, VENC_GAIN_U_NTSC_M);

	/* gain control for Cr signal */
	out_regl(VENC_GAIN_V, VENC_GAIN_V_NTSC_M);

	/* gain control for Y signal */
	out_regl(VENC_GAIN_Y, VENC_GAIN_Y_NTSC_M);

	/* BLACK LEVEL SETTING */
	out_regl(VENC_BLACK_LEVEL, VENC_BLACK_LEVEL_NTSC_M);

	/* BLACK LEVEL SETTING */
	out_regl(VENC_BLANK_LEVEL, VENC_BLANK_LEVEL_NTSC_M);

	/* X_COLOR */
	out_regl(VENC_X_COLOR, VENC_X_COLOR_VAL);

	/* enable phase alternation line encoding */
	out_regl(VENC_M_CONTROL, VENC_M_CONTROL_NTSC);

	/* the ITU-R 601 sampling rate and the amplitude of the color burst are set. */
	out_regl(VENC_BSTAMP_WSS_DATA, VENC_BSTAMP_WSS_DATA_NTSC);

	/* COLOR SUBCARRIER FREQUENCY */
	out_regl(VENC_S_CARR, VENC_S_CARR_NTSC);

	/* CLOSED CAPTION DATA */
	out_regl(VENC_LINE21, VENC_LINE21_VAL);

	/* line selection for closed caption data are encoded */
	out_regl(VENC_LN_SEL, VENC_LN_SEL_VAL);

	/* L21 and WC related setting */
	out_regl(VENC_L21_WC_CTL, VENC_L21_WC_CTL_GEN);

	/* horizontal and vertical trigger phase reference */
	out_regl(VENC_HTRIGGER_VTRIGGER, VENC_HTRIGGER_VTRIGGER_VAL);

	/* the starting and ending positions of the active video on a horizontal line */
	out_regl(VENC_SAVID_EAVID, VENC_SAVID_EAVID_GEN);

	/* specify the first active line of Field and Field length */
	out_regl(VENC_FLEN_FAL, VENC_FLEN_FAL_GEN);

	/* mention the phase reset mode(reset every eight fields) and the last active line of Field */
	out_regl(VENC_LAL_PHASE_RESET, VENC_LAL_PHASE_RESET_NTSC);

	/* specify the HSYNC internal start and stop pixel values */
	out_regl(VENC_HS_INT_START_STOP_X, VENC_HS_INT_START_STOP_X_NTSC);

	/* specify the HSYNC external start and stop pixel values */
	out_regl(VENC_HS_EXT_START_STOP_X, VENC_HS_EXT_START_STOP_X_GEN);

	/* specify the VSYNC internal start PIXEL  values */
	out_regl(VENC_VS_INT_START_X, VENC_VS_INT_START_X_GEN);

	/* specify the VSYNC internal start line and stop pixel values */
	out_regl(VENC_VS_INT_STOP_X_VS_INT_START_Y, VENC_VS_INT_STOP_X_VS_INT_START_Y_NTSC);

	/* specify the VSYNC internal stop line and external start pixel values */
	out_regl(VENC_VS_INT_STOP_Y_VS_EXT_START_X, VENC_VS_INT_STOP_Y_VS_EXT_START_X_NTSC);

	/* specify the VSYNC external start line and stop pixel values */
	out_regl(VENC_VS_EXT_STOP_X_VS_EXT_START_Y, VENC_VS_EXT_STOP_X_VS_EXT_START_Y_GEN);

	/* specify the VSYNC external stop line values */
	out_regl(VENC_VS_EXT_STOP_Y, VENC_VS_EXT_STOP_Y_GEN);

	/* define AVID start and stop pixel values */
	out_regl(VENC_AVID_START_STOP_X, VENC_AVID_START_STOP_X_NTSC);

	/* define AVID start and stop line values */
	out_regl(VENC_AVID_START_STOP_Y, VENC_AVID_START_STOP_Y_NTSC);

	/* define FID internal start pixel and line values */
	out_regl(VENC_FID_INT_START_X_FID_INT_START_Y, VENC_FID_INT_START_X_FID_INT_START_Y_GEN);

	/* define FID external start pixel and internal offset line values */
	out_regl(VENC_FID_INT_OFFSET_Y_FID_EXT_START_X, VENC_FID_INT_OFFSET_Y_FID_EXT_START_X_GEN);

	/* define FID external start line and external offset line values */
	out_regl(VENC_FID_EXT_START_Y_FID_EXT_OFFSET_Y, VENC_FID_EXT_START_Y_FID_EXT_OFFSET_Y_GEN);

	/* TVDETGP generation is disabled and SYNC_POLARITY and UVPHASE_POL polarity selection */
	out_regl(VENC_GEN_CTRL, VENC_GEN_CTRL_VAL);

	/* Enable the DAC */
	if (tv_mode == 0)
		out_regl(VENC_DAC_TST, 0x0000000A);
	else
		out_regl(VENC_DAC_TST, 0x0000000D);

}

/*-----------------------------------------------------------------------------
| Function    :S32 ntsc_j
+------------------------------------------------------------------------------
| Description :This function makes the necessary settings for the NTSC J standard.
|
| Parameters  :void
|
| Returns     :S32 success or failure
+-----------------------------------------------------------------------------*/
void ntsc_j(void)
{

	ntsc_m();

	/* register settings for NTSC J starts */

	/* gain control for Cb signal */
	out_regl(VENC_GAIN_U, VENC_GAIN_U_NTSC_J);

	/* gain control for Cr signal */
	out_regl(VENC_GAIN_V, VENC_GAIN_V_NTSC_J);

	/* gain control for Y signal */
	out_regl(VENC_GAIN_Y, VENC_GAIN_Y_NTSC_J);

	/* BLACK LEVEL SETTING */
	out_regl(VENC_BLACK_LEVEL, VENC_BLACK_LEVEL_NTSC_J);

	/* BLACK LEVEL SETTING */
	out_regl(VENC_BLANK_LEVEL, VENC_BLANK_LEVEL_NTSC_J);

}

/*-----------------------------------------------------------------------------
| Function    : ntsc_443
+------------------------------------------------------------------------------
| Description :This function makes the necessary settings for the NTSC 443 standard.
|
| Parameters  :void
|
| Returns     :S32 success or failure
+-----------------------------------------------------------------------------*/
void ntsc_443(void)
{

	/* register settings for NTSC J starts */

	/* total number of pixels in a scan line */
	out_regl(VENC_LLEN, (VENC_LLEN_GEN | VENC_LLEN_EN));

	/* total number of lines in a frame from source image */
	out_regl(VENC_FLENS, VENC_FLENS_GEN);

	/* enable the chrominance interpolation filter and luminance interpolation filters */
	out_regl(VENC_HFLTR_CTRL, VENC_HFLTR_CTRL_EN);

	/* wide screen signalling and close caption control */
	out_regl(VENC_CC_CARR_WSS_CARR, VENC_CC_CARR_WSS_CARR_GEN);

	/* CHOOSE THE PHASE OF THE ENCODED VIDEO COLOR SUBCARRIER RELATIVE TO H-SYNC */
	out_regl(VENC_C_PHASE, VENC_C_PHASE_GEN);

	/* gain control for Cb signal */
	out_regl(VENC_GAIN_U, VENC_GAIN_U_NTSC_443);

	/* gain control for Cr signal */
	out_regl(VENC_GAIN_V, VENC_GAIN_V_NTSC_443);

	/* gain control for Y signal */
	out_regl(VENC_GAIN_Y, VENC_GAIN_Y_NTSC_443);

	/* BLACK LEVEL SETTING */
	out_regl(VENC_BLACK_LEVEL, VENC_BLACK_LEVEL_NTSC_443);

	/* BLACK LEVEL SETTING */
	out_regl(VENC_BLANK_LEVEL, VENC_BLANK_LEVEL_NTSC_443);

	/* X_COLOR */
	out_regl(VENC_X_COLOR, VENC_X_COLOR_VAL);

	/* enable phase alternation line encoding */
	out_regl(VENC_M_CONTROL, VENC_M_CONTROL_NTSC);

	/* the ITU-R 601 sampling rate and the amplitude of the color burst are set. */
	out_regl(VENC_BSTAMP_WSS_DATA, VENC_BSTAMP_WSS_DATA_NTSC_443);

	/* COLOR SUBCARRIER FREQUENCY */
	out_regl(VENC_S_CARR, VENC_S_CARR_NTSC_443);

	/* CLOSED CAPTION DATA */
	out_regl(VENC_LINE21, VENC_LINE21_VAL);

	/* line selection for closed caption data are encoded */
	out_regl(VENC_LN_SEL, VENC_LN_SEL_VAL);

	/* L21 and WC related setting */
	out_regl(VENC_L21_WC_CTL, VENC_L21_WC_CTL_GEN);

	/* horizontal and vertical trigger phase reference */
	out_regl(VENC_HTRIGGER_VTRIGGER, VENC_HTRIGGER_VTRIGGER_VAL);

	/* the starting and ending positions of the active video on a horizontal line */
	out_regl(VENC_SAVID_EAVID, VENC_SAVID_EAVID_GEN);

	/* specify the first active line of Field and Field length */
	out_regl(VENC_FLEN_FAL, VENC_FLEN_FAL_GEN);

	/* mention the phase reset mode(reset every eight fields) and the last active line of Field */
	out_regl(VENC_LAL_PHASE_RESET, VENC_LAL_PHASE_RESET_NTSC);

	/* specify the HSYNC internal start and stop pixel values */
	out_regl(VENC_HS_INT_START_STOP_X, VENC_HS_INT_START_STOP_X_443);

	/* specify the HSYNC external start and stop pixel values */
	out_regl(VENC_HS_EXT_START_STOP_X, VENC_HS_EXT_START_STOP_X_GEN);

	/* specify the VSYNC internal start PIXEL  values */
	out_regl(VENC_VS_INT_START_X, VENC_VS_INT_START_X_GEN);

	/* specify the VSYNC internal start line and stop pixel values */
	out_regl(VENC_VS_INT_STOP_X_VS_INT_START_Y, VENC_VS_INT_STOP_X_VS_INT_START_Y_NTSC_443);

	/* specify the VSYNC internal stop line and external start pixel values */
	out_regl(VENC_VS_INT_STOP_Y_VS_EXT_START_X, VENC_VS_INT_STOP_Y_VS_EXT_START_X_NTSC_443);

	/* specify the VSYNC external start line and stop pixel values */
	out_regl(VENC_VS_EXT_STOP_X_VS_EXT_START_Y, VENC_VS_EXT_STOP_X_VS_EXT_START_Y_GEN);

	/* specify the VSYNC external stop line values */
	out_regl(VENC_VS_EXT_STOP_Y, VENC_VS_EXT_STOP_Y_GEN);

	/* define AVID start and stop pixel values */
	out_regl(VENC_AVID_START_STOP_X, VENC_AVID_START_STOP_X_NTSC_443);

	/* define AVID start and stop line values */
	out_regl(VENC_AVID_START_STOP_Y, VENC_AVID_START_STOP_Y_NTSC_443);

	/* define FID internal start pixel and line values */
	out_regl(VENC_FID_INT_START_X_FID_INT_START_Y, VENC_FID_INT_START_X_FID_INT_START_Y_GEN);

	/* define FID external start pixel and internal offset line values */
	out_regl(VENC_FID_INT_OFFSET_Y_FID_EXT_START_X, VENC_FID_INT_OFFSET_Y_FID_EXT_START_X_GEN);

	/* define FID external start line and external offset line values */
	out_regl(VENC_FID_EXT_START_Y_FID_EXT_OFFSET_Y, VENC_FID_EXT_START_Y_FID_EXT_OFFSET_Y_GEN);

	/* TVDETGP generation is disabled and SYNC_POLARITY and UVPHASE_POL polarity selection */
	out_regl(VENC_GEN_CTRL, VENC_GEN_CTRL_VAL);

	/* Enable the DAC */
	if (tv_mode == 0)
		out_regl(VENC_DAC_TST, 0x0000000A);
	else
		out_regl(VENC_DAC_TST, 0x0000000D);

}

/*****************************************************************************
 Set back ground color
******************************************************************************/
void set_default_color(U32 color)
{
	color = color & DEFAULT_COLOR_MASK;
	/* set Default Color */
	out_regl(DISPC_DEFAULT_COLOR0, color);
}

/*****************************************************************************
 Set Transparency Color color
******************************************************************************/
void set_transparency_color(U32 color)
{
	color = color & TRANSPARENCY_COLOR_MASK;
	/* set Transparency Color */
	out_regl(DISPC_TRANS_COLOR0, color);
}

/*end of file */
