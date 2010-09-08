/*
 * lcd_utils.h: LCD utilities related constants and function prototypes.
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

#ifndef __MS_DIAG_LCD_TRY_H__
#define __MS_DIAG_LCD_TRY_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "types.h"
#include "lcd_drv.h"

#define GFX_PIPELINE    0x0
#define V1_PIPELINE     0x1
#define V2_PIPELINE     0x2


#define CONTROL_PADCONF_GPIO_173        (0x480021CC)
#define CONTROL_PADCONF_GPIO_175        (0x480021D0)
#define CONTROL_PADCONF_GPIO_176        (0x480021D2)
#define CONTROL_PADCONF_GPIO_177        (0x490021D4)

#define CONTROL_PADCONF_GPIO_152	(0x48002184)
#define CONTROL_PADCONF_GPIO_153	(0x48002186)
#define CONTROL_PADCONF_GPIO_154	(0x48002188)
#define CONTROL_PADCONF_GPIO_155	(0x4800218A)

/* DSS[00:17] */
#define CONTROL_PADCONF_DSS_D0         	(0x480020DC)	
#define CONTROL_PADCONF_DSS_D1          (0x480020DE)
#define CONTROL_PADCONF_DSS_D2          (0x480020E0)
#define CONTROL_PADCONF_DSS_D3          (0x480020E2)
#define CONTROL_PADCONF_DSS_D4          (0x480020E4)
#define CONTROL_PADCONF_DSS_D5          (0x480020E6) 
#define CONTROL_PADCONF_DSS_D6          (0x480020E8)
#define CONTROL_PADCONF_DSS_D7          (0x480020EA)
#define CONTROL_PADCONF_DSS_D8          (0x480020EC)
#define CONTROL_PADCONF_DSS_D9          (0x480020EE)
#define CONTROL_PADCONF_DSS_D10         (0x480020F0)
#define CONTROL_PADCONF_DSS_D11         (0x480020F2)
#define CONTROL_PADCONF_DSS_D12         (0x480020F4)
#define CONTROL_PADCONF_DSS_D13         (0x480020F6)
#define CONTROL_PADCONF_DSS_D14         (0x480020F8)
#define CONTROL_PADCONF_DSS_D15         (0x480020FA)
#define CONTROL_PADCONF_DSS_D16         (0x480020FC)
#define CONTROL_PADCONF_DSS_D17         (0x480020FE)

/* DSS[18:23] */
#define CONTROL_PADCONF_DSS_D18         (0x48002100)
#define CONTROL_PADCONF_DSS_D19         (0x48002102)
#define CONTROL_PADCONF_DSS_D20         (0x48002104)
#define CONTROL_PADCONF_DSS_D21         (0x48002106)
#define CONTROL_PADCONF_DSS_D22         (0x48002108)
#define CONTROL_PADCONF_DSS_D23         (0x4800210A)

/* #define CONTROL_PADCONF_DSS_PCLK        (0x480020D4)
#define CONTROL_PADCONF_DSS_VSYNC       (0x480020D8)
#define CONTROL_PADCONF_DSS_HSYNC       (0x480020D6)
#define CONTROL_PADCONF_DSS_ACBIAS      (0x480020DA) */

#if 0
#define DSS_SPECIFIC_BASE_ADDR          (0x48050000)
#define DISPC_BASE_ADDR                 (DSS_SPECIFIC_BASE_ADDR + 0x0400)

#define  DISPC_REVISION                 (DISPC_BASE_ADDR + 0x0000)
#define  DISPC_SYSCONFIG                (DISPC_BASE_ADDR + 0x0010)
#define  DISPC_SYSSTATUS                (DISPC_BASE_ADDR + 0x0014)
#define  DISPC_IRQSTATUS                (DISPC_BASE_ADDR + 0x0018)
#define  DISPC_IRQENABLE                (DISPC_BASE_ADDR + 0x001C)
#define  DISPC_CONTROL                  (DISPC_BASE_ADDR + 0x0040)
#define  DISPC_CONFIG                   (DISPC_BASE_ADDR + 0x0044)
#define  DISPC_CAPABLE                  (DISPC_BASE_ADDR + 0x0048)
#define  DISPC_DEFAULT_COLOUR0          (DISPC_BASE_ADDR + 0x004C)
#define  DISPC_DEFAULT_COLOUR1          (DISPC_BASE_ADDR + 0x0050)
#define  DISPC_TRANS_COLOUR0            (DISPC_BASE_ADDR + 0x0054)
#define  DISPC_TRANS_COLOUR1            (DISPC_BASE_ADDR + 0x0058)
#define  DISPC_LINE_STATUS              (DISPC_BASE_ADDR + 0x005C)
#define  DISPC_LINE_NUMBER              (DISPC_BASE_ADDR + 0x0060)
#define  DISPC_TIMING_H                 (DISPC_BASE_ADDR + 0x0064)
#define  DISPC_TIMING_V                 (DISPC_BASE_ADDR + 0x0068)
#define  DISPC_POL_FREQ                 (DISPC_BASE_ADDR + 0x006C)
#define  DISPC_DIVISOR                  (DISPC_BASE_ADDR + 0x0070)
#define  DISPC_SIZE_DIG                 (DISPC_BASE_ADDR + 0x0078)
#define  DISPC_SIZE_LCD                 (DISPC_BASE_ADDR + 0x007C)
#define  DISPC_GFX_BA0                  (DISPC_BASE_ADDR + 0x0080)
#define  DISPC_GFX_BA1                  (DISPC_BASE_ADDR + 0x0084)
#define  DISPC_GFX_POSITION             (DISPC_BASE_ADDR + 0x0088)
#define  DISPC_GFX_SIZE                 (DISPC_BASE_ADDR + 0x008C)
#define  DISPC_GFX_ATTRIBUTES           (DISPC_BASE_ADDR + 0x00A0)
#define  DISPC_GFX_FIFO_THRESHOLD       (DISPC_BASE_ADDR + 0x00A4)
#define  DISPC_GFX_FIFO_SIZE_STATUS     (DISPC_BASE_ADDR + 0x00A8)
#define  DISPC_GFX_ROW_INC              (DISPC_BASE_ADDR + 0x00AC)
#define  DISPC_GFX_PIXEL_INC            (DISPC_BASE_ADDR + 0x00B0)
#define  DISPC_GFX_WINDOW_SKIP          (DISPC_BASE_ADDR + 0x00B4)
#define  DISPC_GFX_TABLE_BA             (DISPC_BASE_ADDR + 0x00B8)
#define  DISPC_VID1_BA0                 (DISPC_BASE_ADDR + 0x00BC)
#define  DISPC_VID1_BA1                 (DISPC_BASE_ADDR + 0x00C0)
#define  DISPC_VID1_POSITION            (DISPC_BASE_ADDR + 0x00C4)
#define  DISPC_VID1_SIZE                (DISPC_BASE_ADDR + 0x00C8)
#define  DISPC_VID1_ATTRIBUTES          (DISPC_BASE_ADDR + 0x00CC)
#define  DISPC_VID1_FIFO_THRESHOLD      (DISPC_BASE_ADDR + 0x00D0)
#define  DISPC_VID1_FIFO_SIZE_STATUS    (DISPC_BASE_ADDR + 0x00D4)
#define  DISPC_VID1_ROW_INC             (DISPC_BASE_ADDR + 0x00D8)
#define  DISPC_VID1_PIXEL_INC           (DISPC_BASE_ADDR + 0x00DC)
#define  DISPC_VID1_FIR                 (DISPC_BASE_ADDR + 0x00E0)
#define  DISPC_VID1_PICTURE_SIZE        (DISPC_BASE_ADDR + 0x00E4)
#define  DISPC_VID1_ACCU0               (DISPC_BASE_ADDR + 0x00E8)
#define  DISPC_VID1_ACCU1               (DISPC_BASE_ADDR + 0x00EC)
#define  DISPC_VID1_FIR_COEF_H0         (DISPC_BASE_ADDR + 0x00F0)
#define  DISPC_VID1_FIR_COEF_H7         (DISPC_BASE_ADDR + 0x0128)
#define  DISPC_VID1_FIR_COEF_HV0        (DISPC_BASE_ADDR + 0x00F4)
#define  DISPC_VID1_FIR_COEF_HV7        (DISPC_BASE_ADDR + 0x012C)
#define  DISPC_VID1_CONV_COEF0          (DISPC_BASE_ADDR + 0x0130)
#define  DISPC_VID1_CONV_COEF1          (DISPC_BASE_ADDR + 0x0134)
#define  DISPC_VID1_CONV_COEF2          (DISPC_BASE_ADDR + 0x0138)
#define  DISPC_VID1_CONV_COEF3          (DISPC_BASE_ADDR + 0x013C)
#define  DISPC_VID1_CONV_COEF4          (DISPC_BASE_ADDR + 0x0140)
#define  DISPC_VID2_BA0                 (DISPC_BASE_ADDR + 0x014C)
#define  DISPC_VID2_BA1                 (DISPC_BASE_ADDR + 0x0150)
#define  DISPC_VID2_POSITION            (DISPC_BASE_ADDR + 0x0154)
#define  DISPC_VID2_SIZE                (DISPC_BASE_ADDR + 0x0158)
#define  DISPC_VID2_ATTRIBUTES          (DISPC_BASE_ADDR + 0x015C)
#define  DISPC_VID2_FIFO_THRESHOLD      (DISPC_BASE_ADDR + 0x0160)
#define  DISPC_VID2_FIFO_SIZE_STATUS    (DISPC_BASE_ADDR + 0x0164)
#define  DISPC_VID2_ROW_INC             (DISPC_BASE_ADDR + 0x0168)
#define  DISPC_VID2_PIXEL_INC           (DISPC_BASE_ADDR + 0x016C)
#define  DISPC_VID2_FIR                 (DISPC_BASE_ADDR + 0x0170)
#define  DISPC_VID2_PICTURE_SIZE        (DISPC_BASE_ADDR + 0x0174)
#define  DISPC_VID2_ACCU0               (DISPC_BASE_ADDR + 0x0178)
#define  DISPC_VID2_ACCU1               (DISPC_BASE_ADDR + 0x017C)
#define  DISPC_VID2_FIR_COEF_H0         (DISPC_BASE_ADDR + 0x0180)
#define  DISPC_VID2_FIR_COEF_H7         (DISPC_BASE_ADDR + 0x01B8)
#define  DISPC_VID2_FIR_COEF_HV0        (DISPC_BASE_ADDR + 0x0184)
#define  DISPC_VID2_FIR_COEF_HV7        (DISPC_BASE_ADDR + 0x01BC)
#define  DISPC_VID2_CONV_COEF0          (DISPC_BASE_ADDR + 0x01C0)
#define  DISPC_VID2_CONV_COEF1          (DISPC_BASE_ADDR + 0x01C4)
#define  DISPC_VID2_CONV_COEF2          (DISPC_BASE_ADDR + 0x01C8)
#define  DISPC_VID2_CONV_COEF3          (DISPC_BASE_ADDR + 0x01CC)
#define  DISPC_VID2_CONV_COEF4          (DISPC_BASE_ADDR + 0x01D0)
#define  DISPC_DATA_CYCLE1              (DISPC_BASE_ADDR + 0x01D4)
#define  DISPC_DATA_CYCLE2              (DISPC_BASE_ADDR + 0x01D8)
#define  DISPC_DATA_CYCLE3              (DISPC_BASE_ADDR + 0x01DC)
#define  DISPC_VID1_FIR_COEF_V0         (DISPC_BASE_ADDR + 0x01E0)
#define  DISPC_VID1_FIR_COEF_V7         (DISPC_BASE_ADDR + 0x01FC)
#define  DISPC_VID2_FIR_COEF_V0         (DISPC_BASE_ADDR + 0x0200)
#define  DISPC_VID2_FIR_COEF_V7         (DISPC_BASE_ADDR + 0x021C)
#define  DISPC_CPR_R                    (DISPC_BASE_ADDR + 0x0220)
#define  DISPC_CPR_G                    (DISPC_BASE_ADDR + 0x0224)
#define  DISPC_CPR_B                    (DISPC_BASE_ADDR + 0x0228)
#define  DISPC_GFX_PRELOAD              (DISPC_BASE_ADDR + 0x022C)
#define  DISPC_VID1_PRELOAD             (DISPC_BASE_ADDR + 0x0230)
#define  DISPC_VID2_PRELOAD             (DISPC_BASE_ADDR + 0x0234)
#endif

#define DEFAULT_COLOUR_MASK              (0x00FFFFFF)
#define TRANSPARENCY_COLOUR_MASK         (0x00FFFFFF)
#define X_POS_MASK                       (0x000007FF)
#define Y_POS_MASK                       (0x000007FF)
#define FORMAT_MASK                      (0x0F)
#define PIXEL_INC_MASK                   (0x0000FFFF)
// #define VIDENDIANNESS                    (1 << 17)
// #define VIDFULLRANGE                     (1 << 11)
#define VIDCOLOURCONVENABLE              (1 << 9)
// #define ATTRIBUTES_ENABLE                (1 << 0)

/* DISPC_CONTROL register fields. */
#define DISPC_CONTROL_GPOUT1                    (1 << 16)
#define DISPC_CONTROL_GPOUT0                    (1 << 15)
#define DISPC_CONTROL_RFBIMODE                  (1 << 11)
#define DISPC_CONTROL_TFTDATALINES_OALSB24B     (3 <<  8)
#define DISPC_CONTROL_TFTDATALINES_OALSB18B     (2 <<  8)
#define DISPC_CONTROL_TFTDATALINES_OALSB16B     (1 <<  8)
#define DISPC_CONTROL_GODIGITAL                 (1 <<  6)
#define DISPC_CONTROL_GOLCD                     (1 <<  5)
#define DISPC_CONTROL_STNTFT                    (1 <<  3)
#define DISPC_CONTROL_DIGITALENABLE             (1 <<  1)
#define DISPC_CONTROL_LCDENABLE                 (1 <<  0)

/* DISPC_SYSCONFIG register fields. */
#define DISPC_SYSCONFIG_MIDLEMODE_NSTANDBY      (1 << 12)
#define DISPC_SYSCONFIG_SIDLEMODE_NIDLE         (1 <<  3)
#define DISPC_SYSCONFIG_SOFTRESET               (1 <<  1)

/* DISPC_SYSSTATUS register field. */
#define DISPC_SYSSTATUS_RESETDONE               (1 << 0)

/* DISPC_CONFIG register fields. */
#define DISPC_CONFIG_PALETTEGAMMATABLE          (1 <<  3)
#define DISPC_CONFIG_LOADMODE_FRDATLEFR         (1 <<  2)
#define DISPC_CONFIG_LOADMODE_PGTABUSETB        (1 <<  1)

#if 0
/* DISPC_TIMING_H register fields. */
#define DISPC_TIMING_H_HBP_SHIFT                (20)
#define DISPC_TIMING_H_HFP_SHIFT                (8)
#define DISPC_TIMING_H_HSW_SHIFT                (0)

/* DISPC_TIMING_V register fields. */
#define DISPC_TIMING_V_VBP_SHIFT                (20)
#define DISPC_TIMING_V_VFP_SHIFT                (8)
#define DISPC_TIMING_V_VSW_SHIFT                (0)
#endif

/* DISPC_POL_FREQ register fields. */
#define DISPC_POL_FREQ_IHS                      (1 << 13)
#define DISPC_POL_FREQ_RF_SHIFT                 (16)
#define DISPC_POL_FREQ_ONOFF_SHIFT              (17)
#define DISPC_POL_FREQ_IPC_SHIFT                (14)
#define DISPC_POL_FREQ_IHS_SHIFT                (13)
#define DISPC_POL_FREQ_IVS_SHIFT                (12)

/* DISPC_DIVISOR. */
// #define DISPC_DIVISOR_LCD_SHIFT                 (16)
// #define DISPC_DIVISOR_PCD_SHIFT                 (0)

/* DISPC_SIZE_LCD register fields. */
#define DISPC_SIZE_LCD_LPP                      (0x7FF << 16)
// #define DISPC_SIZE_LCD_LPP_SHIFT                (16)
// #define DISPC_SIZE_LCD_PPL                      (0x7FF)
// #define DISPC_SIZE_LCD_PPL_SHIFT                (0)

/* DISPC_GFX_SIZE register fields. */
// #define DISPC_GFX_SIZE_LPP                      (0x7FF << 16)
// #define DISPC_GFX_SIZE_LPP_SHIFT                (16)
// #define DISPC_GFX_SIZE_PPL                      (0x7FF)
// #define DISPC_GFX_SIZE_PPL_SHIFT                (0)

/* DISPC_ATTRIBUTES register fields. */
#define DISPC_ATTRIBUTES_GFXENABLE              (1 << 0)
#define DISPC_GFX_ATTRIBUTES_REPLICATION_EN     (1 << 5)
#define DISPC_VID_ATTRIBUTES_REPLICATION_EN     (1 << 10)

#if 0
/* DISPC_GFX_FIFO_THRESHOLD register fields. */
#define DISPC_GFX_FIFO_THRESHOLD_HIGH_SHIFT     (16)
#define DISPC_GFX_FIFO_THRESHOLD_LOW_SHIFT      (0)
#define DISPC_FIFO_THRESHOLD_HIGH               (225)
#define DISPC_FIFO_THRESHOLD_LOW                (194)
#endif
/* DISPC_IRQSTATUS registerr fields. */
#define DISPC_IRQSTATUS_FRAMEDONE               (1 <<  0)

/* Backlight data. */
#define BACK_LIGHT_DISABLE  0x00
#define BACK_LIGHT_ENABLE   0x01

/* Image Formats. */
#define BITMAP1     (0x0)
#define BITMAP2     (0x1)
#define BITMAP4     (0x2)
#define BITMAP8     (0x3)
#define RGB12       (0x4)
#define RGB16       (0x6)
#define RGB24       (0x8)
#define YUV422      (0xA)
#define UYVY422     (0xB)

/* Image types. */
#define SQCIF       (0x0)
#define QCIF        (0x1)
#define CIF         (0x2)
#define VGA         (0x3)
#define SVGA        (0x4)
#define XGA         (0x5)
#define SXGA        (0x6)
#define UXGA        (0x7)
#define QSIF        (0x8)
#define SIF         (0x9)

/* LCD display colours. */

/* #define RED_COLOR 		0xF800
#define GREEN_COL0R		0x07E0
#define BLUE_COLOR		0x001F
#define YELLOW_COLOR		0xFFE0
#define WHITE_COLOR		0xFFFF
#define BLACK_COLOR		0x0000
#define NO_COLOR        	0xFEFE */

#if 0
#define RED_COLOUR          (0x3F0000)
#define GREEN_COL0R         (0x003F00)
#define BLUE_COLOUR         (0x00003F)
#define YELLOW_COLOUR       (0x3FFC0)
#define WHITE_COLOUR        (0x3FFFF)
#define BLACK_COLOUR        (0x00000)
#define CYAN_COLOUR         (0x00FFF)
#define MAGENTA_COLOUR      (0x3F03F)
#define NO_COLOUR           (0x0FEFE)
#endif

// 32bpp
#if 0
#define RED_COLOUR          (0x00FF0000)
#define GREEN_COL0R         (0x0000FF00)
#define BLUE_COLOUR         (0x000000FF)
#define YELLOW_COLOUR       (0x00FFFF00)
#define WHITE_COLOUR        (0x00FFFFFF)
#define BLACK_COLOUR        (0x00000000)
#define CYAN_COLOUR         (0x0000FFFF)
#define MAGENTA_COLOUR      (0x00FF00FF)
#define NO_COLOUR           (0x000F0F0F)
#endif

#if 1
#define RED_COLOR          (0xF800)
#define GREEN_COL0R         (0x07E0)
#define BLUE_COLOR         (0x001F)
#define YELLOW_COLOR       (0xFFE0)
#define WHITE_COLOR        (0xFFFF)
#define BLACK_COLOR        (0x0000)
#define NO_COLOR           (0xFEFE)

#define RED_COLOUR          (0xF800)
#define GREEN_COL0UR         (0x07E0)
#define BLUE_COLOUR         (0x001F)
#define YELLOW_COLOUR       (0xFFE0)
#define WHITE_COLOUR        (0xFFFF)
#define BLACK_COLOUR        (0x0000)
#define NO_COLOUR           (0xFEFE)
#endif

#if 0
#define RED_COLOUR          (0x003F0000)
#define GREEN_COL0R         (0x00003F00)
#define BLUE_COLOUR         (0x0000003F)
#define YELLOW_COLOUR       (0xFFE0)
#define WHITE_COLOUR        (0xFFFF)
#define BLACK_COLOUR        (0x0000)
#define NO_COLOUR           (0xFEFE)
#endif

#ifdef ES_1_REV_A

#define GPIO_NUM_RESB       173
#define GPIO_NUM_QVGA_nVGA  175
#define GPIO_NUM_VDD        176
#define GPIO_NUM_INI	    177

#define GPIO_NUM_LR	2
#define GPIO_NUM_UD	3	

#else

#define GPIO_NUM_RESB       155
#define GPIO_NUM_QVGA_nVGA  154
#define GPIO_NUM_VDD        153
#define GPIO_NUM_INI	    152
#define GPIO_NUM_LR	2
#define GPIO_NUM_UD	3	

#endif


#define CONV_SCAN_DIRECTION     1
#define INVR_SCAN_DIRECTION     0

typedef struct
{
  U16 x_res;
  U16 y_res;

}T_IMAGE_TYPE;
  

extern T_IMAGE_TYPE image_type_val[];

/* Function Prototypes. */
S32 enable_lcd_power (void);
S32 enable_lcd_backlight (lcd_drv_struct_t *lcd_init_struct);
S32 enable_lcd_reset (void);
S32 enable_INI (void);
S32 enable_lcd_HVIF (void);
S32 disable_lcd_backlight (lcd_drv_struct_t *lcd_init_struct);
S32 disable_lcd_power (void);
S32 disble_INI (void);
S32 disable_lcd_reset (void);
void mux_setup_lcd_bk_vdd (void);
void mux_setup_lcd (void);
void issue_go_lcd (void);
void disable_dss (void);
void reset_display_controller (void);
void set_default_colour (U32 colour);
void set_transparency_colour (U32 colour);
void set_panel_size (U8 panel_type);
void set_lcd_interface_datalines (U8 datalines);
void set_buffer_base_addr (U8 pipeline, U32 base_address);
void set_window_pos (U8 pipeline, U32 x_pos, U32 y_pos);
void set_window_size (U8 pipeline, U8 image_type);
void set_csc_coff (U8 pipeline);
void set_format (U8 pipeline, U8 format);
void set_fifo_threshold (U8 pipeline);
void set_row_inc (U8 pipeline, U32 inc_val);
void set_pixel_inc (U8 pipeline, U32 inc_val);
void enable_pipeline (U8 pipeline);
void configure_dss (U8 mode);
void display_lcd_image (void);
void enable_replication_logic (U8 pipeline);
void disable_pipeline (U8 pipeline);
void  fill_colour_32 (U32 * image_addr, U32 colour);
void fill_colour (U16 * image_addr, U16 colour);
S32 configure_vert_scan_direction (U8 direction);
S32 configure_horiz_scan_direction (U8 direction);
status_t configure_lcd (lcd_drv_struct_t *lcd_init_struct);

#ifdef __cplusplus
}
#endif

#endif  /* __MS_DIAG_LCD_H__ */
