/*
 * tvout.h: TV out driver constant definitions and function prototypes.
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

#ifndef __MS_DIAG_TVOUT_H__
#define __MS_DIAG_TVOUT_H__

#ifdef __cplusplus
extern "C" {
#endif

// #include "dss.h"

/* Defines for Menelaus  */
#define ENABLE_VREF 	    	0x02
#define ENABLE_VDD_ADAC     	0x01
#define DISABLE_VREF 		0x00
#define DISABLE_VDD_ADAC	0x00

#define MENELAUS_I2C_ADDR 	0x72
#define MENELAUS_VREF_REG_ADDR 	0x11

/* Video controller register addresses */
#define VENC_DAC_TST		        (VIDEO_ENCODER_BASE_ADDR + 0x00C4) 
#define VIDEO_ENCODER_BASE_ADDR         (DSS_SPECIFIC_BASE_ADDR + 0x0C00)

#define F_CONTROL                       (VIDEO_ENCODER_BASE_ADDR + 0x0008)
#define VIDOUT_CTRL		  	(VIDEO_ENCODER_BASE_ADDR + 0x0010)
#define SYNC_CONTROL                    (VIDEO_ENCODER_BASE_ADDR + 0x0014)

#define VENC_LLEN                       (VIDEO_ENCODER_BASE_ADDR + 0x001C)
#define VENC_FLENS                      (VIDEO_ENCODER_BASE_ADDR + 0x0020)

#define VENC_HFLTR_CTRL                 (VIDEO_ENCODER_BASE_ADDR + 0x0024)
#define VENC_CC_CARR_WSS_CARR           (VIDEO_ENCODER_BASE_ADDR + 0x0028)
#define VENC_C_PHASE                    (VIDEO_ENCODER_BASE_ADDR + 0x002C)

#define VENC_GAIN_U                     (VIDEO_ENCODER_BASE_ADDR + 0x0030)
#define VENC_GAIN_V                     (VIDEO_ENCODER_BASE_ADDR + 0x0034)
#define VENC_GAIN_Y                     (VIDEO_ENCODER_BASE_ADDR + 0x0038)
#define VENC_BLACK_LEVEL                (VIDEO_ENCODER_BASE_ADDR + 0x003C)
#define VENC_BLANK_LEVEL                (VIDEO_ENCODER_BASE_ADDR + 0x0040)

#define VENC_X_COLOR                    (VIDEO_ENCODER_BASE_ADDR + 0x0044)
#define VENC_M_CONTROL                  (VIDEO_ENCODER_BASE_ADDR + 0x0048)
#define VENC_BSTAMP_WSS_DATA            (VIDEO_ENCODER_BASE_ADDR + 0x004C)
#define VENC_S_CARR                     (VIDEO_ENCODER_BASE_ADDR + 0x0050)

#define VENC_LINE21                     (VIDEO_ENCODER_BASE_ADDR + 0x0054)
#define VENC_LN_SEL                     (VIDEO_ENCODER_BASE_ADDR + 0x0058)
#define VENC_L21_WC_CTL                 (VIDEO_ENCODER_BASE_ADDR + 0x005C)
#define VENC_HTRIGGER_VTRIGGER          (VIDEO_ENCODER_BASE_ADDR + 0x0060)

#define VENC_SAVID_EAVID                (VIDEO_ENCODER_BASE_ADDR + 0x0064)
#define VENC_FLEN_FAL                   (VIDEO_ENCODER_BASE_ADDR + 0x0068)
#define VENC_LAL_PHASE_RESET            (VIDEO_ENCODER_BASE_ADDR + 0x006C)
#define VENC_HS_INT_START_STOP_X        (VIDEO_ENCODER_BASE_ADDR + 0x0070)
#define VENC_HS_EXT_START_STOP_X        (VIDEO_ENCODER_BASE_ADDR + 0x0074)
#define VENC_VS_INT_START_X             (VIDEO_ENCODER_BASE_ADDR + 0x0078)

#define VENC_VS_INT_STOP_X_VS_INT_START_Y    (VIDEO_ENCODER_BASE_ADDR + 0x007C)
#define VENC_VS_INT_STOP_Y_VS_EXT_START_X    (VIDEO_ENCODER_BASE_ADDR + 0x0080)
#define VENC_VS_EXT_STOP_X_VS_EXT_START_Y    (VIDEO_ENCODER_BASE_ADDR + 0x0084)

#define VENC_VS_EXT_STOP_Y              (VIDEO_ENCODER_BASE_ADDR + 0x0088)
#define VENC_AVID_START_STOP_X          (VIDEO_ENCODER_BASE_ADDR + 0x0090)
#define VENC_AVID_START_STOP_Y          (VIDEO_ENCODER_BASE_ADDR + 0x0094)

#define VENC_FID_INT_START_X_FID_INT_START_Y       (VIDEO_ENCODER_BASE_ADDR + 0x00A0)
#define VENC_FID_INT_OFFSET_Y_FID_EXT_START_X      (VIDEO_ENCODER_BASE_ADDR + 0x00A4)
#define VENC_FID_EXT_START_Y_FID_EXT_OFFSET_Y      (VIDEO_ENCODER_BASE_ADDR + 0x00A8)
#define VENC_TVDETGP_INT_START_STOP_X 		   (VIDEO_ENCODER_BASE_ADDR + 0x00B0)
#define VENC_TVDETGP_INT_START_STOP_Y 		   (VIDEO_ENCODER_BASE_ADDR + 0x00B4)
#define VENC_GEN_CTRL				   (VIDEO_ENCODER_BASE_ADDR + 0x00B8)
#define VENC_OUTPUT_CONTROL			   (VIDEO_ENCODER_BASE_ADDR + 0x00C4) // Refer the 2420 Errata
#define VENC_DAC				   (VIDEO_ENCODER_BASE_ADDR + 0x00C8) // Refer the 2420 Errata

/* Video controller register bit values */

/*to reset the video encoder */
#define VENC_RESET_ENCODER              0x00000100
#define VENC_RESET_DONE            	0x00000100

/* video encoder output clock */
#define VENC_54MHx_CLK            	0x00000000

#define FREE_RUNNING_DISABLE            0x00000000
#define FREE_RUNNING_ENABLE             0x00008000

#define COLOR_BAR_ENABLE                0x00000040
#define BACKGROUND_COLOR_ENABLE         0x00000080
#define ENABLE_EXTERNAL_VIDEO		0x00000000

#define VENC_HFLTR_CTRL_EN              0x00000000
#define VENC_X_COLOR_VAL                0x00000000

#define VENC_LINE21_VAL                 0x00000000
#define VENC_LN_SEL_VAL                 0x00000015
#define VENC_HTRIGGER_VTRIGGER_VAL      0x00000000

#define VENC_TVDETGP_INT_START_STOP_X_VAL   0x00140001
#define VENC_TVDETGP_INT_START_STOP_Y_VAL   0x00010001
#define VENC_GEN_CTRL_VAL                   0x00FF0000
#define VENC_GEN_CTRL_PAL_VAL               0x00F90000

/* DAC enable and in normal operation */
#define VENC_DAC_ENABLE			            0x0000000A // Refer the 2420 Errata.
#define VENC_OUTPUT_CONTROL_CHROMA_ENABLE   		(1<<2)
#define VENC_OUTPUT_CONTROL_COMPOSITE_ENABLE		(1<<1)
#define VENC_OUTPUT_CONTROL_DAC_INVT			(1<<3)
#define VENC_OUTPUT_CONTROL_LUMA_ENABLE			(1<<0)
/* VALUES SAME FOR  NTSC,PAL-M AND PAL-60 */
#define VENC_LLEN_EN						0x00000000 /* for llen register */
#define VENC_LLEN_GEN                   			0x00000359
#define VENC_FLENS_GEN                  			0x0000020C
#define VENC_C_PHASE_GEN                			0x00000024
#define VENC_CC_CARR_WSS_CARR_GEN       			0x043F2631
#define VENC_L21_WC_CTL_GEN             			0x00001400
#define VENC_SAVID_EAVID_GEN            			0x069300f4
#define VENC_FLEN_FAL_GEN             				0x0016020c
#define VENC_HS_EXT_START_STOP_X_GEN    			0x000F0359
#define VENC_VS_INT_START_X_GEN         			0x01A00000
#define VENC_VS_EXT_STOP_X_VS_EXT_START_Y_GEN       0x020d01ac

#define VENC_VS_EXT_STOP_Y_GEN                      0x00000006
#define VENC_FID_INT_START_X_FID_INT_START_Y_GEN    0x0001008A
#define VENC_FID_INT_OFFSET_Y_FID_EXT_START_X_GEN   0x01ac0106
#define VENC_FID_EXT_START_Y_FID_EXT_OFFSET_Y_GEN   0x01060006


// VALUES THAT ARE SAME FOR PAL-BDGHI, PAL-N, PAL-NC

#define VENC_LLEN_PAL                   0x0000035F
#define VENC_FLENS_PAL                  0x00000270

#define VENC_LLEN_PAL_M                 0x00000359
#define VENC_FLENS_PAL_M                0x0000020C

#define VENC_C_PHASE_PAL                0x000000F0  //FOR BDGHI and N
#define VENC_C_PHASE_PAL_NC             0x00000000

#define VENC_CC_CARR_WSS_CARR_PAL       0x000025ED

#define VENC_L21_WC_CTL_PAL             0x00000000

#define VENC_SAVID_EAVID_PAL            0x06A70108
#define VENC_FLEN_FAL_PAL               0x00170270
#define VENC_HS_EXT_START_STOP_X_PAL    0x000F035F
#define VENC_VS_INT_START_X_PAL         0x01A70000
#define VENC_VS_EXT_STOP_Y_PAL          0x00000005

#define VENC_VS_EXT_STOP_X_VS_EXT_START_Y_PAL      0x027101AF

#define VENC_GAIN_U_PAL_BDGHI           0x00000130


#define VENC_GAIN_U_PAL_N               0x000000FD
#define VENC_GAIN_U_PAL_NC              0x00000130
#define VENC_GAIN_U_PAL_M               0x00000140
#define VENC_GAIN_U_PAL_60              0x00000140

#define VENC_GAIN_V_PAL_BDGHI           0x000001B0

#define VENC_GAIN_V_PAL_N               0x00000165
#define VENC_GAIN_V_PAL_NC              0x000001B0
#define VENC_GAIN_V_PAL_M               0x00000190
#define VENC_GAIN_V_PAL_60              0x00000190

#define VENC_GAIN_Y_PAL_BDGHI           0x000001B0
#define VENC_GAIN_Y_PAL_N               0x00000177
#define VENC_GAIN_Y_PAL_NC              0x000001B0
#define VENC_GAIN_Y_PAL_M               0x000001C0
#define VENC_GAIN_Y_PAL_60              0x000001C0

#define VENC_BLACK_LEVEL_PAL_BDGHI      0x00000067

#define VENC_BLANK_LEVEL_PAL_BDGHI      0x00000067
#define VENC_BLACK_LEVEL_PAL_NC         0x00000063
#define VENC_BLANK_LEVEL_PAL_NC         0x00000063

#define VENC_BLACK_LEVEL_PAL_N          0x00000060
#define VENC_BLANK_LEVEL_PAL_N          0x00000053

#define VENC_BLACK_LEVEL_PAL_M          0x00000069
#define VENC_BLANK_LEVEL_PAL_M          0x0000005C

#define VENC_BLACK_LEVEL_PAL_60         0x00000069
#define VENC_BLANK_LEVEL_PAL_60         0x0000005C

#define VENC_M_CONTROL_PAL              0x00000002
#define VENC_M_CONTROL_PAL_M            0x00000003
#define VENC_M_CONTROL_PAL_60           0x00000003

#define VENC_BSTAMP_WSS_DATA_PAL_BDGHI  0x00000043

#define VENC_BSTAMP_WSS_DATA_PAL_N      0x00000038
#define VENC_BSTAMP_WSS_DATA_PAL_M      0x0000003F
#define VENC_BSTAMP_WSS_DATA_PAL_NC     0x00000041
#define VENC_BSTAMP_WSS_DATA_PAL_60     0x0000003F

#define VENC_S_CARR_PAL_M               0x21E6EFE3
#define VENC_S_CARR_PAL_NC              0x21E6EFE3
#define VENC_S_CARR_PAL_BDGHI           0x2A098ACB
#define VENC_S_CARR_PAL_60              0x2A098ACB

#define VENC_LAL_PHASE_RESET_PAL        0x00040136        //FOR BDGHI and N
#define VENC_LAL_PHASE_RESET_PAL_NC     0x00040135
#define VENC_LAL_PHASE_RESET_PAL_2      0x00040107       //FOR PAL-M and PAL-60

#define VENC_HS_INT_START_STOP_X_PAL    0x00920358        //FOR BDGHI and N
#define VENC_HS_INT_START_STOP_X_NC     0x00880358
#define VENC_HS_INT_START_STOP_X_PAL_2  0x007e034e       //FOR PAL-M and PAL-60

#define VENC_VS_INT_STOP_X_VS_INT_START_Y_PAL      0x000601A7
#define VENC_VS_INT_STOP_X_VS_INT_START_Y_PAL_2    0x020901a0  //FOR PAL-M and PAL-60
#define VENC_VS_INT_STOP_X_VS_INT_START_Y_PAL_NC   0x026F01A7

#define VENC_VS_INT_STOP_Y_VS_EXT_START_X_PAL      0x01AF0036
#define VENC_VS_INT_STOP_Y_VS_EXT_START_X_PAL_2    0x01ac0022  //FOR PAL-M and PAL-60
#define VENC_VS_INT_STOP_Y_VS_EXT_START_X_PAL_NC   0x01AF002E

#define VENC_AVID_START_STOP_X_PAL        0x03530082     //FOR BDGHI and N
#define VENC_AVID_START_STOP_X_PAL_NC     0x03530083
#define VENC_AVID_START_STOP_X_PAL_2      0x03480078    //FOR PAL-M and PAL-60

#define VENC_AVID_START_STOP_Y_PAL        0x026E002E	//FOR BDGHI and N
#define VENC_AVID_START_STOP_Y_PAL_2      0x02060026    //FOR PAL-M and PAL-60
#define VENC_AVID_START_STOP_Y_PAL_NC     0x026E002E

#define VENC_FID_INT_START_X_FID_INT_START_Y_PAL     0x0005008A

#define VENC_FID_INT_OFFSET_Y_FID_EXT_START_X_PAL     0x002E0138


#define VENC_FID_EXT_START_Y_FID_EXT_OFFSET_Y_PAL     0x01380005


// NTSC
#define VENC_GAIN_U_NTSC_M              0x00000130
#define VENC_GAIN_U_NTSC_J              0x00000100
#define VENC_GAIN_U_NTSC_443            0x00000140

#define VENC_GAIN_V_NTSC_M              0x00000198
#define VENC_GAIN_V_NTSC_J              0x0000016D
#define VENC_GAIN_V_NTSC_443            0x00000190

#define VENC_GAIN_Y_NTSC_M              0x000001C0
#define VENC_GAIN_Y_NTSC_J              0x00000196
#define VENC_GAIN_Y_NTSC_443            0x000001C0

#define VENC_BLACK_LEVEL_NTSC_443       0x00000069
#define VENC_BLANK_LEVEL_NTSC_443       0x0000005C

#define VENC_BLACK_LEVEL_NTSC_M         0x0000006A
#define VENC_BLANK_LEVEL_NTSC_M         0x0000005C

#define VENC_BLACK_LEVEL_NTSC_J         0x00000053
#define VENC_BLANK_LEVEL_NTSC_J         0x00000053

#define VENC_M_CONTROL_NTSC             0x00000001

#define VENC_BSTAMP_WSS_DATA_NTSC       0x0000003F
#define VENC_BSTAMP_WSS_DATA_NTSC_443   0x0000003F

#define VENC_S_CARR_NTSC_443            0x2A098ACB
#define VENC_S_CARR_NTSC                0x21F07C1F

#define VENC_HTRIGGER_VTRIGGER_VAL      0x00000000

#define VENC_FLEN_FAL_NTSC              0x0001020C

#define VENC_LAL_PHASE_RESET_NTSC       0x00060107

#define VENC_HS_INT_START_STOP_X_NTSC   0x008D034E

#define VENC_HS_INT_START_STOP_X_443    0x007e034e

#define VENC_VS_INT_STOP_X_VS_INT_START_Y_NTSC      0x020501a0
#define VENC_VS_INT_STOP_X_VS_INT_START_Y_NTSC_443  0x020901a0

#define VENC_VS_INT_STOP_Y_VS_EXT_START_X_NTSC      0x01ac0024
#define VENC_VS_INT_STOP_Y_VS_EXT_START_X_NTSC_443  0x01ac0022

#define VENC_AVID_START_STOP_X_NTSC        0x03480079
#define VENC_AVID_START_STOP_X_NTSC_443    0x03480078

#define VENC_AVID_START_STOP_Y_NTSC        0x02040024
#define VENC_AVID_START_STOP_Y_NTSC_443    0x02060026

/* function prototypes */
S32 tv_config(U8,U8,U16,U16,U16,U16,U16,U32,U32);
S32 config_video_encoder(U8 video_std);

#ifdef __cplusplus
}
#endif

#endif  /*__MS_DIAG_TVOUT_H__*/

