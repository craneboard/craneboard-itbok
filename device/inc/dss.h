/*
 * dss.h: OMAP DSS constant definitions and function prototypes.
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

#ifndef __MS_DIAG_DSS_H__
#define __MS_DIAG_DSS_H__

#ifdef __cplusplus
extern "C" {
#endif

#define DSS_SPECIFIC_BASE_ADDR          0x48050000
#define DISPC_CONTROLLER_BASE_ADDR      (DSS_SPECIFIC_BASE_ADDR + 0x0400)
#define DSS_CONTROL                     (DSS_SPECIFIC_BASE_ADDR + 0x40)

/*---------------- Display Controller registers--------------------------- */
/*-----------------Register Bits ----------------------------------------------*/ 
/* DISPC_CONTROL register fields */
#define DISPC_CONTROL_GPOUT1                 (1 << 16)
#define DISPC_CONTROL_GPOUT0                 (1 << 15)
#define DISPC_CONTROL_RFBIMODE               (1 << 11)
#define DISPC_CONTROL_TFTDATALINES_OALSB18B  (2 <<  8)
#define DISPC_CONTROL_TFTDATALINES_OALSB16B  (1 <<  8)
#define DISPC_CONTROL_GODIGITAL              (1 <<  6)
#define DISPC_CONTROL_GOLCD                  (1 <<  5)
#define DISPC_CONTROL_STNTFT                 (1 <<  3)
#define DISPC_CONTROL_DIGITALENABLE          (1 <<  1)
#define DISPC_CONTROL_LCDENABLE		   		 (1 <<  0)

/* DISPC_IRQSTATUS registerr fields */
#define DISPC_IRQSTATUS_FRAMEDONE	     	(1 <<  0)
      
/* DISPC_SYSCONFIG register fields */
#define DISPC_SYSCONFIG_MIDLEMODE_NSTANDBY   	(1 << 12)
#define DISPC_SYSCONFIG_SIDLEMODE_NIDLE      	(1 <<  3)
#define DISPC_SYSCONFIG_SOFTRESET            	(1 <<  1)

/* DISPC_SYSSTATUS register field */
#define DISPC_SYSSTATUS_RESETDONE            	(1 << 0)

#define DISPC_CONFIG_LOADMODE_FRDATLEFR      	(1 << 2)

/* DISPC_GFX_ATTRIBUTES register fields */
#define DISPC_GFX_ATTRIBUTES_GFXENABLE      	(1 << 0)
#define DISPC_GFX_CHANNEL_OUT_ENABLE        	(1 << 8)
#define DISPC_GFX_LAYER_CAPABLE             	(1 << 8)

#define DISPC_GFX_RGB16      		    	(6 << 1)
#define DISPC_GFX_RGB24      		    	(8 << 1)
#define DISPC_GFX_BURST_SIZE   		    	(1 << 6)
#define DISPC_VID1_BURST_SIZE   	    	(1 << 14)

#define GFX_PIXEL_INC_ONE_BYTE		    	(1 << 0)
#define DISPC_V1_CHANNEL_OUT_ENABLE         	(1 << 16)
#define GFX_Y_POSITION			    	(16)
#define DIGITAL_LINES_PER_PANEL  		(16)

#define DIGITAL_OUT_ENABLE			(1 << 1)
#define GO_DIGITAL				(1 << 6)
#define DIGITAL_OUT_HOLD_TIME			(1 << 17)


#define DISPC_MASK_ALL_INTERRUPTS       0x00000000
#define DISPC_CLEAR_INTERRUPT_STATUS    0x00007FFF


/* DISPC_GFX_FIFO_THRESHOLD register fields */
#define DISPC_GFX_FIFO_THRESHOLD_HIGH_SHIFT    16
#define DISPC_GFX_FIFO_THRESHOLD_LOW_SHIFT     0

#define DISPC_FIFO_THRESHOLD_HIGH    225
#define DISPC_FIFO_THRESHOLD_LOW     194

/* DISPC_TIMING_H register fields */
#define DISPC_TIMING_H_HBP_SHIFT           20
#define DISPC_TIMING_H_HFP_SHIFT           8
#define DISPC_TIMING_H_HSW_SHIFT           0

/* DISPC_TIMING_V register fields */
#define DISPC_TIMING_V_VBP_SHIFT			20
#define DISPC_TIMING_V_VFP_SHIFT			8
#define DISPC_TIMING_V_VSW_SHIFT			0

/* DISPC_POL_FREQ register fields */
#define DISPC_POL_FREQ_IHS                  (1 << 13)

/* DISPC_DIVISOR */
#define DISPC_DIVISOR_LCD_SHIFT             16
#define DISPC_DIVISOR_PCD_SHIFT             0

/* DISPC_SIZE_LCD register fields */
#define DISPC_SIZE_LCD_LPP                  (0x7FF << 16)
#define DISPC_SIZE_LCD_LPP_SHIFT            16
#define DISPC_SIZE_LCD_PPL                  0x7FF
#define DISPC_SIZE_LCD_PPL_SHIFT            0

/* DISPC_GFX_SIZE register fields */
#define DISPC_GFX_SIZE_LPP                  (0x7FF << 16)
#define DISPC_GFX_SIZE_LPP_SHIFT            16
#define DISPC_GFX_SIZE_PPL                  0x7FF
#define DISPC_GFX_SIZE_PPL_SHIFT            0

/* Values to be configured for DSS */
#define DSS_TV_CLOCK_ENABLE             	0x00000007

#define DSS_CLK1_ENABLE				0x00000001
#define DSS_CLK2_ENABLE				0x00000002
#define TVOUT_CLK_ENABLE			0x00000004

#define DSS_INTERFACE_CLOCK_ENABLE      	0x00000001

#define DSS_CONTROL_VIDEO_VALUE         	0x00000019

#define DSS_VENC_OUT_COMPOSITE          	0x00000000
#define DSS_VENC_OUT_SVIDEO             	0x00000040

#define DSS_ENABLE_APLL		        	0x00000001
#define DSS_VENC_4X_ENABLE	        	0x00000008
#define DSS_DACDEMEN_ENABLE	        	0x00000010
#define DSS_PWRDNBGZ		        	0x00000020
#define DSS_VENC_OUT_SEL			0x00000040
#define DISPC_BASE_ADDR				(DSS_SPECIFIC_BASE_ADDR + 0x0400)
//#define DISPC_CONTROLLER_BASE_ADDR      (DSS_SPECIFIC_BASE_ADDR + 0x0400)

   /* DISPC_GFX_FIFO_THRESHOLD register fields */
#define DISPC_GFX_FIFO_THRESHOLD_HIGH_SHIFT    	16
#define DISPC_GFX_FIFO_THRESHOLD_LOW_SHIFT     	0

#define  DISPC_REVISION				(DISPC_BASE_ADDR + 0x0000)
#define  DISPC_SYSCONFIG			(DISPC_BASE_ADDR + 0x0010)
#define  DISPC_SYSSTATUS			(DISPC_BASE_ADDR + 0x0014)
#define  DISPC_IRQSTATUS			(DISPC_BASE_ADDR + 0x0018)
#define  DISPC_IRQENABLE			(DISPC_BASE_ADDR + 0x001C)
#define  DISPC_CONTROL				(DISPC_BASE_ADDR + 0x0040)
#define  DISPC_CONFIG				(DISPC_BASE_ADDR + 0x0044)
#define  DISPC_CAPABLE				(DISPC_BASE_ADDR + 0x0048)
#define  DISPC_DEFAULT_COLOR0 			(DISPC_BASE_ADDR + 0x004C)
#define  DISPC_DEFAULT_COLOR1			(DISPC_BASE_ADDR + 0x0050)
#define  DISPC_TRANS_COLOR0 			(DISPC_BASE_ADDR + 0x0054)
#define  DISPC_TRANS_COLOR1			(DISPC_BASE_ADDR + 0x0058)
#define  DISPC_LINE_STATUS			(DISPC_BASE_ADDR + 0x005C)
#define  DISPC_LINE_NUMBER			(DISPC_BASE_ADDR + 0x0060)
#define  DISPC_TIMING_H				(DISPC_BASE_ADDR + 0x0064)
#define  DISPC_TIMING_V				(DISPC_BASE_ADDR + 0x0068)
#define  DISPC_POL_FREQ				(DISPC_BASE_ADDR + 0x006C)
#define  DISPC_DIVISOR				(DISPC_BASE_ADDR + 0x0070)
#define  DISPC_SIZE_DIG				(DISPC_BASE_ADDR + 0x0078)
#define  DISPC_SIZE_LCD				(DISPC_BASE_ADDR + 0x007C)
#define  DISPC_GFX_BA0 				(DISPC_BASE_ADDR + 0x0080)
#define  DISPC_GFX_BA1				(DISPC_BASE_ADDR + 0x0084)
#define  DISPC_GFX_POSITION			(DISPC_BASE_ADDR + 0x0088)
#define  DISPC_GFX_SIZE				(DISPC_BASE_ADDR + 0x008C)
#define  DISPC_GFX_ATTRIBUTES			(DISPC_BASE_ADDR + 0x00A0)
#define  DISPC_GFX_FIFO_THRESHOLD		(DISPC_BASE_ADDR + 0x00A4)
#define  DISPC_GFX_FIFO_SIZE_STATUS		(DISPC_BASE_ADDR + 0x00A8)
#define  DISPC_GFX_ROW_INC			(DISPC_BASE_ADDR + 0x00AC)
#define  DISPC_GFX_PIXEL_INC			(DISPC_BASE_ADDR + 0x00B0)
#define  DISPC_GFX_WINDOW_SKIP			(DISPC_BASE_ADDR + 0x00B4)
#define  DISPC_GFX_TABLE_BA			(DISPC_BASE_ADDR + 0x00B8)
#define  DISPC_VID1_BA0 			(DISPC_BASE_ADDR + 0x00BC)
#define  DISPC_VID1_BA1				(DISPC_BASE_ADDR + 0x00C0)
#define  DISPC_VID1_POSITION			(DISPC_BASE_ADDR + 0x00C4)
#define  DISPC_VID1_SIZE			(DISPC_BASE_ADDR + 0x00C8)
#define  DISPC_VID1_ATTRIBUTES			(DISPC_BASE_ADDR + 0x00CC)
#define  DISPC_VID1_FIFO_THRESHOLD		(DISPC_BASE_ADDR + 0x00D0)
#define  DISPC_VID1_FIFO_SIZE_STATUS		(DISPC_BASE_ADDR + 0x00D4)
#define  DISPC_VID1_ROW_INC			(DISPC_BASE_ADDR + 0x00D8)
#define  DISPC_VID1_PIXEL_INC			(DISPC_BASE_ADDR + 0x00DC)
#define  DISPC_VID1_FIR				(DISPC_BASE_ADDR + 0x00E0)
#define  DISPC_VID1_PICTURE_SIZE		(DISPC_BASE_ADDR + 0x00E4)
#define  DISPC_VID1_ACCU0 			(DISPC_BASE_ADDR + 0x00E8)
#define  DISPC_VID1_ACCU1		 	(DISPC_BASE_ADDR + 0x00EC)
#define  DISPC_VID1_FIR_COEF_H0 		(DISPC_BASE_ADDR + 0x00F0)
#define  DISPC_VID1_FIR_COEF_H7			(DISPC_BASE_ADDR + 0x0128)
#define  DISPC_VID1_FIR_COEF_HV0  		(DISPC_BASE_ADDR + 0x00F4)
#define  DISPC_VID1_FIR_COEF_HV7		(DISPC_BASE_ADDR + 0x012C)
#define  DISPC_VID1_CONV_COEF0			(DISPC_BASE_ADDR + 0x0130)
#define  DISPC_VID1_CONV_COEF1			(DISPC_BASE_ADDR + 0x0134)
#define  DISPC_VID1_CONV_COEF2			(DISPC_BASE_ADDR + 0x0138)
#define  DISPC_VID1_CONV_COEF3			(DISPC_BASE_ADDR + 0x013C)
#define  DISPC_VID1_CONV_COEF4			(DISPC_BASE_ADDR + 0x0140)
#define  DISPC_VID2_BA0  			(DISPC_BASE_ADDR + 0x014C)
#define  DISPC_VID2_BA1				(DISPC_BASE_ADDR + 0x0150)
#define  DISPC_VID2_POSITION			(DISPC_BASE_ADDR + 0x0154)
#define  DISPC_VID2_SIZE			(DISPC_BASE_ADDR + 0x0158)
#define  DISPC_VID2_ATTRIBUTES			(DISPC_BASE_ADDR + 0x015C)
#define  DISPC_VID2_FIFO_THRESHOLD		(DISPC_BASE_ADDR + 0x0160)
#define  DISPC_VID2_FIFO_SIZE_STATUS 		(DISPC_BASE_ADDR + 0x0164)
#define  DISPC_VID2_ROW_INC			(DISPC_BASE_ADDR + 0x0168)
#define  DISPC_VID2_PIXEL_INC	     		(DISPC_BASE_ADDR + 0x016C)
#define  DISPC_VID2_FIR				(DISPC_BASE_ADDR + 0x0170)
#define  DISPC_VID2_PICTURE_SIZE 	 	(DISPC_BASE_ADDR + 0x0174)
#define  DISPC_VID2_ACCU0 			(DISPC_BASE_ADDR + 0x0178)
#define  DISPC_VID2_ACCU1			(DISPC_BASE_ADDR + 0x017C)
#define  DISPC_VID2_FIR_COEF_H0 	 	(DISPC_BASE_ADDR + 0x0180)
#define  DISPC_VID2_FIR_COEF_H7		 	(DISPC_BASE_ADDR + 0x01B8)
#define  DISPC_VID2_FIR_COEF_HV0  	 	(DISPC_BASE_ADDR + 0x0184)
#define  DISPC_VID2_FIR_COEF_HV7	 	(DISPC_BASE_ADDR + 0x01BC)
#define  DISPC_VID2_CONV_COEF0	     		(DISPC_BASE_ADDR + 0x01C0)
#define  DISPC_VID2_CONV_COEF1		 	(DISPC_BASE_ADDR + 0x01C4)
#define  DISPC_VID2_CONV_COEF2		 	(DISPC_BASE_ADDR + 0x01C8)
#define  DISPC_VID2_CONV_COEF3		 	(DISPC_BASE_ADDR + 0x01CC)
#define  DISPC_VID2_CONV_COEF4		 	(DISPC_BASE_ADDR + 0x01D0)
#define  DISPC_DATA_CYCLE1			(DISPC_BASE_ADDR + 0x01D4)
#define  DISPC_DATA_CYCLE2			(DISPC_BASE_ADDR + 0x01D8)
#define  DISPC_DATA_CYCLE3			(DISPC_BASE_ADDR + 0x01DC)
#define  DISPC_VID1_FIR_COEF_V0  	 	(DISPC_BASE_ADDR + 0x01E0)
#define  DISPC_VID1_FIR_COEF_V7		 	(DISPC_BASE_ADDR + 0x01FC)
#define  DISPC_VID2_FIR_COEF_V0  	 	(DISPC_BASE_ADDR + 0x0200)
#define  DISPC_VID2_FIR_COEF_V7		 	(DISPC_BASE_ADDR + 0x021C)
#define  DISPC_CPR_R				(DISPC_BASE_ADDR + 0x0220)
#define  DISPC_CPR_G				(DISPC_BASE_ADDR + 0x0224)
#define  DISPC_CPR_B				(DISPC_BASE_ADDR + 0x0228)
#define  DISPC_GFX_PRELOAD			(DISPC_BASE_ADDR + 0x022C)
#define  DISPC_VID1_PRELOAD			(DISPC_BASE_ADDR + 0x0230)
#define  DISPC_VID2_PRELOAD			(DISPC_BASE_ADDR + 0x0234)


#define DEFAULT_COLOR_MASK		      	 (0x00FFFFFF)
#define TRANSPARENCY_COLOR_MASK 		 (0x00FFFFFF)
#define X_POS_MASK				 (0x000007FF)
#define Y_POS_MASK				 (0x000007FF)
#define FORMAT_MASK				 (0x0F)
#define PIXEL_INC_MASK				 (0x0000FFFF)
#define VIDENDIANNESS				 (1<<17)
#define VIDFULLRANGE				 (1<<11)
#define VIDCOLORCONVENABLE			 (1<<9)
#define ATTRIBUTES_ENABLE   			 (1<<0)

#if 0
typedef struct
{
  U16 x_res;
  U16 y_res;

}T_IMAGE_TYPE;

static T_IMAGE_TYPE image_type_val[] = {
						96,	128, 
						144, 176, 
						240, 320, 
						480, 640, 
						480, 854, 
						768, 1024,
						1024, 1280, 
						1200, 1600, 
						120, 160, 
						240, 352
					};
#endif  
#if 0
static S16 color_convbt601_full[][3] ={
						256, 351, 0,
						256, -179, -86,
						256, 0, 443
					};


static S16 color_convbt601_full[][3] ={
						256, 394, 0,
						256, -118, -47,
						256, 0, 465
					};
#endif
#if 0
extern S16 color_convbt601_full[][3] ={
						298, 409, 0,
						298, -208, -11,
						298, 0, 517
					};
#endif

/*================Function Prototypes====================================================*/
void config_display_controller(U16 gfx_x,U16 gfx_y,
					U16 gfx_ppl,U16 gfx_lpf,
					U16 dig_ppl,U32 dig_lpf,U32 image_buffer);
void config_gfx_window(U16 x_position,U16 y_position,U16 size_x,U16 size_y);
void config_digital_out( U16 no_of_pixels,U32 no_of_lines);
//void venc_output_clock();
void reset_video_encoder(void);
void tv_fillcolor(U32 color);
void disable_pipeline(U8 pipeline);

#ifdef __cplusplus
}
#endif

#endif  /*__MS_DIAG_DSS_H__*/


