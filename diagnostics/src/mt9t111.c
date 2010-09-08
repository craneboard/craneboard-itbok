/*
 * mt9t111.c: MT9T111 camera routines.
 *
 * (C) Copyright 2010
 * Mistral Solutions Private Limited, <www.mistralsolutions.com>
 *
 * Author :
 *	Ganeshan <nganeshan@mistralsolutions.com>
 *
 * Derived from MT9T111.
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
 *   12 Mar 2010 - Created from OMAP2 ITBOK source.
 *   21 Jun 2010 - Fixed the camera display issue on LCD
 */

#include <common.h>
#include <i2c.h>
#include "types.h"
#include "../../common/inc/types.h"
#include "../inc/mt9t111.h"
#include "../inc/mt9t111_reg.h"
#include "../../drivers/inc/lcd_drv.h"
#include "../inc/ispregs.h"
#include "../inc/dg_camera.h"
#include "../../device/inc/i2c_utils.h"
#include "../../drivers/inc/i2c_drv.h"
#include "../inc/dg_lcd.h"

#define DISPLAY_WIDTH 		640
#define DISPLAY_HEIGHT		720

#define USE_RAW

#define COLOR_BAR
#undef COLOR_BAR

#define BIT_PATTERN_TEST
#undef BIT_PATTERN_TEST

#define USE_YUV422
#undef USE_YUV422

#define USE_RGB565
#undef USE_RGB565

#define DEBUG_MT9T 	0
#define DEBUG_ISP  	0

/* ISP register read write functions */
#define ispReadb(a)         (*(volatile unsigned char  *)(a))
#define ispReadw(a)         (*(volatile unsigned short *)(a))
#define ispReadl(a)         (*(volatile unsigned int   *)(a))
#define ispWriteb(v,a)      (*(volatile unsigned char  *)(a) = (v))
#define ispWritew(v,a)      (*(volatile unsigned short *)(a) = (v))
#define ispWritel(v,a)      (*(volatile unsigned int   *)(a) = (v))

#define RGB24_TO_RGB16(red, green, blue) ( ((red >> 3) << 11) | ((green >> 2) << 5) | (blue >> 3))

/*i2c adress for MT9T111*/
#define SENSOR_NOT_DETECTED     0
#define SENSOR_DETECTED         1

#define SD_CAMERA_BUFFER 0x80530000

#define RGB565_(r,g,b) ( (((r)>>3)<<11) | (((g)>>2)<<5) | ((b)>>3) )
#define RGB565A(c) RGB565(GetRValue(c),GetGValue(c),GetBValue(c))
#define Get565RValue(c) ( ( (c) >> 11 ) & 0x1F )
#define Get565GValue(c) ( ( (c) >> 5 )  & 0x3F )
#define Get565BValue(c) ( (c) & 0x1F )
#define RGB24TO565(c) RGB565_(Get565RValue(c),Get565GValue(c),Get565BValue(c))

#define ISP_CTRL_SET_SHIFT      (0x003BF100)

struct mt9t111_param *mt9t111_params;
struct i2c_client *i2c_client;

unsigned int mt9t111_lcd = 0;
extern i2c_init_structure_t camera_i2c;

extern void camera_i2c_2byte_write(u32 addr, u16 write_buf, u32 size);
extern void isp_set_cam_xclk(unsigned int xclk);
extern void mt9t111_init(void);
extern int mt9t111_detect(void);
extern void mt9t111_deinit(void);
extern void mt9t_deinit(void);

void cam_display_image(void);
static void mt9t111_bayer_format(void);
#ifdef COLOR_BAR
static void mt9t111_color_bar(void);
#endif
#ifdef BIT_PATTERN_TEST
static void mt9t111_bit_test_pattern(void);
#endif
#ifdef USE_RGB565
static void mt9t111_rgb565_format(void);
#endif
int mt9t111_def_reg_load(mt9t111_regs * reg_in, int cnt);

/*
 * Function Name : 
 */
static short mt9t111_enable_pll(void)
{
	U8 read_buf[4];
	unsigned short value;

	mt9t111_def_reg_load((mt9t111_regs *) & pll_regs1, sizeof(pll_regs1) / sizeof(mt9t111_regs));
#if DEBUG_MT9T
	printf("%s %s\n", __FILE__, __FUNCTION__);
#endif
	printf("Waiting for PLL lock .....\n");
	while (1) {
		read_i2c_with_dataptr_2byte_subaddr(CAMERA_I2C_ADDR, 0x0014, I2C_2_BYTE_ADDRESS, 2, read_buf);
		value = (((read_buf[0] & 0x00ff) << 8) | (read_buf[1] & 0xff));
		if ((value & 0x8000) != 0)
			break;
		udelay(2000);
	}
	mt9t111_def_reg_load((mt9t111_regs *) pll_regs2, sizeof(pll_regs2) / sizeof(mt9t111_regs));
#if DEBUG_MT9T
	printf("Received  PLL lock\n");
#endif
	return 0;
}

/*
 * Function Name : 
 */
static void mt9t111_refresh(void)
{
	int i;
	volatile unsigned short value;
	// MCU_ADDRESS [SEQ_CMD] -- refresh
	camera_i2c_2byte_write(0x098E, 0x8400, 4);
	camera_i2c_2byte_write(0x0990, 0x0006, 4);

	for (i = 0; i < 100; i++) {
		camera_i2c_2byte_write(0x098E, 0x8400, 4);
		read_i2c_with_dataptr_2byte_subaddr(CAMERA_I2C_ADDR, 0x0990, I2C_2_BYTE_ADDRESS, 2, (u8 *) & value);
		if (value == 0)
			break;
		udelay(5);
	}
}

/*
 * Function Name : 
 */
static short mt9t111_loaddefault(void)
{
	short retval;

#if DEBUG_MT9T
	printf("%s %s\n", __FILE__, __FUNCTION__);
#endif

	camera_i2c_2byte_write(0x001A, 0x0219, 4);
	udelay(100);
	camera_i2c_2byte_write(0x001A, 0x0218, 4);
	udelay(100);

	retval = mt9t111_enable_pll();
	if (-1 == retval) {
		return -1;
	}
#if DEBUG_MT9T
	printf("def_regs1  %s %d\n", __FUNCTION__, __LINE__);
#endif

	mt9t111_def_reg_load((mt9t111_regs *) & def_regs1, sizeof(def_regs1) / sizeof(mt9t111_regs));
#if DEBUG_MT9T
	printf("patch_rev6  %s %d\n", __FUNCTION__, __LINE__);
#endif

	mt9t111_def_reg_load((mt9t111_regs *) & patch_rev6, sizeof(patch_rev6) / sizeof(mt9t111_regs));
#if DEBUG_MT9T
	printf("def_regs2  %s %d\n", __FUNCTION__, __LINE__);
#endif
	mt9t111_def_reg_load((mt9t111_regs *) & def_regs2, sizeof(def_regs2) / sizeof(mt9t111_regs));

#ifdef USE_RAW
	mt9t111_bayer_format();
#endif

#ifdef COLOR_BAR
	mt9t111_color_bar();
#endif

#ifdef BIT_PATTERN_TEST
	mt9t111_bit_test_pattern();
#endif

#ifdef USE_RGB565
	mt9t111_rgb565_format();
#endif

	mt9t111_refresh();

	udelay(1);
	camera_i2c_2byte_write(0x098E, 0x492c, 4);
	udelay(1);
	camera_i2c_2byte_write(0x0990, 0x0000, 4);
	udelay(1);
	return 0;
}

#ifdef COLOR_BAR
static void mt9t111_color_bar(void)
{
	struct i2c_client *client;
#if DEBUG_MT9T
	printf("%s %s\n", __FILE__, __FUNCTION__);
#endif
	mt9t111_write_reg(client, 0x3210, 0x01B0);	// disable lens correction

	mt9t111_write_reg(client, 0x098E, 0x6003);
	mt9t111_write_reg(client, 0x0990, 0x0100);
	mt9t111_write_reg(client, 0x098E, 0x6025);
	mt9t111_write_reg(client, 0x0990, 0x0003);
}
#endif

#ifdef BIT_PATTERN_TEST
static void mt9t111_bit_test_pattern(void)
{

	struct i2c_client *client;
#if DEBUG_MT9T
	printf("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
#endif
	mt9t111_write_reg(client, 0x3210, 0x01B0);	// disable lens correction

	mt9t111_write_reg(client, 0x098E, 0x6003);
	mt9t111_write_reg(client, 0x0990, 0x01A1);
	mt9t111_write_reg(client, 0x098E, 0x6005);
	mt9t111_write_reg(client, 0x0990, 0x0002);
	mt9t111_write_reg(client, 0x098E, 0x600A);
	mt9t111_write_reg(client, 0x0990, 0xA5A5);
}
#endif

#ifdef USE_RGB565
static void mt9t111_rgb565_format(void)
{
#if DEBUG_MT9T
	printf("%s %s\n", __FILE__, __FUNCTION__);
#endif
	mt9t111_def_reg_load((mt9t111_regs *) & rgb565_pattern_regs, sizeof(rgb565_pattern_regs) / sizeof(mt9t111_regs));
}
#endif

static void mt9t111_bayer_format(void)
{
#if DEBUG_MT9T
	printf("%s %s\n", __FILE__, __FUNCTION__);
#endif
	mt9t111_def_reg_load((mt9t111_regs *) & bayer_pattern_regs, sizeof(bayer_pattern_regs) / sizeof(mt9t111_regs));
}

int mt9t111_config(int val)
{
	short retval;

	isp_set_cam_xclk(CAM_XCLKA);
	mt9t111_init();
#if DEBUG_MT9T
	printf("MT9T111 Init done...\n");
#endif
	if (val == 1) {
		isp_set_cam_xclk(CAM_XCLKA);
	}

	retval = mt9t111_detect();
	if (-1 == retval) {
		return -1;
	}
#if DEBUG_MT9T
	printf("Loading MT9T111 Default Values ...\n");
#endif
	retval = mt9t111_loaddefault();
	if (-1 == retval) {
		return -1;
	}
	if (val == 0) {
		mt9t_deinit();
	}
	retval = mt9t111_detect();
	if (-1 == retval) {
		return -1;
	}
#if DEBUG_MT9T
	printf("Loading MT9T111 Default Values ..\n.");
#endif
	retval = mt9t111_loaddefault();
	if (-1 == retval) {
		return -1;
	}
#if DEBUG_MT9T
	printf("Done\n");
#endif
	return 1;
}

void ispccdc_request(void)
{
	U32 u32Val = 0;

	u32Val = 0;
	u32Val = ispReadl(ISP_CTRL);

#ifdef COLOR_BAR
	u32Val |= (ISP_CCDC_RAM_EN | ISP_CCDC_CLK_EN | ISP_SBL_WR1_RAM_EN);
#endif

#ifdef USE_RAW
	u32Val |= (ISP_CCDC_RAM_EN | ISP_CCDC_CLK_EN | ISP_SBL_WR1_RAM_EN);
#endif

#ifdef USE_YUV422
	u32Val |= (ISP_CCDC_RAM_EN | ISP_CCDC_CLK_EN | ISP_SBL_WR1_RAM_EN);
#endif

#ifdef USE_RGB565
	u32Val |= (ISP_CCDC_RAM_EN | ISP_CCDC_CLK_EN | ISP_SBL_WR1_RAM_EN);
#endif
	ispWritel(u32Val, ISP_CTRL);

	/* In OMAP35x, VDLC bit must be set to 1 by software
	   if the CCDC is to be used */
	u32Val = 0x0;
	u32Val = ispReadl(CCDC_CFG);
	u32Val |= CCDC_CFG_VDLC;
	ispWritel(u32Val, CCDC_CFG);

}

void isp_enable_clear_irq(void)
{
	/* Clear interrupts status */
	U32 u32Val = 0;
	u32Val = 0x0;

	u32Val = ispReadl(ISP_IRQ0ENABLE);
	/* Read the IRQ0 Enable Reg and Update */
	u32Val |= ~(HS_VS_IRQ0_ENABLE | VD0_IRQ0_ENABLE | IRQ0_OVF_ENABLE);

	ispWritel(u32Val, ISP_IRQ0ENABLE);
}

void isp_enable_irq(void)
{
	U32 u32Val = 0;
	u32Val = 0x0;
	ispWritel(-1, ISP_IRQ0STATUS);
	/* Read the IRQ0 Enable Reg and Update */
	u32Val |= HS_VS_IRQ0_ENABLE | VD0_IRQ0_ENABLE | IRQ0_OVF_ENABLE;

	ispWritel(u32Val, ISP_IRQ0ENABLE);
}

void isp_realease_resources(void)
{
	U32 u32Val = 0;

	u32Val = ispReadl(ISP_CTRL);

	u32Val &= ~(ISP_CCDC_CLK_EN | ISP_CCDC_RAM_EN | ISP_SBL_WR1_RAM_EN);
	ispWritel(u32Val, ISP_CTRL);
}

static int isp_ccdc_setup(void)
{
	U32 u32reg_val = 0;
	U32 ccdcin_woffset = 0;
	U32 ccdcin_hoffset = 0;
	U32 ccdcin_w = 0;
	U32 ccdcin_h = 0;
	U32 ccdcout_w = 0;
	U32 ccdcout_h = 0;
	U32 u32Val;
	/* Configure sync mode: output */
	u32Val = ispReadl(CCDC_SYN_MODE);
	u32Val |= CCDC_SYN_MODE_VP2SDR;
	u32Val &= ~CCDC_SYN_MODE_SDR2RSZ;
	u32Val |= CCDC_SYN_MODE_WEN;
	u32Val |= CCDC_SYN_MODE_EXWEN;

	/* ISPCCDC_CFG_WENLOG */
	u32reg_val = 0;
	u32reg_val = ispReadl(CCDC_CFG);
	u32reg_val |= CCDC_CFG_WENLOG;
	ispWritel(u32reg_val, CCDC_CFG);

	ispWritel(u32Val, CCDC_SYN_MODE);

	/* Configure sync mode: Input */
	u32Val = 0;
	u32Val = ispReadl(CCDC_SYN_MODE);
	/* enable HS and VS signal */
	u32Val |= CCDC_SYN_MODE_VDHDEN;
	/* field select */
	u32Val &= ~CCDC_SYN_MODE_FLDSTAT;
	/* RAW data */
	u32Val &= CCDC_SYN_MODE_INPMOD_MASK;
	/* data size */
	u32Val &= CCDC_SYN_MODE_DATSIZ_MASK;
	u32Val |= CCDC_SYN_MODE_DATSIZ_10BIT;

	/*Progressive mode */
	u32Val &= ~CCDC_SYN_MODE_FLDMODE;
	/*Normal */
	u32Val &= ~CCDC_SYN_MODE_DATAPOL;
	/*Positive */
	u32Val &= ~CCDC_SYN_MODE_FLDPOL;
	/*Positive */
	u32Val &= ~CCDC_SYN_MODE_HDPOL;
	/*Positive */
	u32Val &= ~CCDC_SYN_MODE_VDPOL;
	/*fld, hd,vd input signals in slave mode */
	u32Val &= ~(CCDC_SYN_MODE_FLDOUT | CCDC_SYN_MODE_VDHDOUT);

	ispWritel(u32Val, CCDC_SYN_MODE);

	/* disable BT656IF */
	ispWritel((ispReadl(CCDC_REC656IF)) & (~CCDC_REC656IF_R656ON), CCDC_REC656IF);

	/* update the CCDC_SYN_MODE Reg */
	ispWritel(u32Val, CCDC_SYN_MODE);

	u32Val = 0;
	/* set color pattern */
	ispWritel(u32Val, CCDC_COLPTN);

    /****** ISPCCDC_CLAMP *****/
	u32Val = 0;
	ispWritel(u32Val, CCDC_BLKCMP);
	/* Disable FPC */
	ispWritel(u32Val, CCDC_FPC);

	/* Disable LSC */
	ispWritel(u32Val, CCDC_LSC_CONFIG);

#if DEBUG_ISP
	isp_reg_dump();
#endif
	ccdcin_woffset = 0;
	ccdcin_w = DISPLAY_WIDTH;
	ccdcout_w = DISPLAY_WIDTH;
	ccdcout_h = DISPLAY_HEIGHT;
	ccdcin_hoffset = 0;
	ccdcin_h = DISPLAY_HEIGHT;
#if DEBUG_ISP
	printf("ccdcin_w %d ccdcin_h %d ccdcout_w %d ccdcout_h %d\n  ", ccdcin_w, ccdcin_h, ccdcout_w, ccdcout_h);
#endif

	ispWritel(0, CCDC_VP_OUT);

	/* Start with 1 pixel apart */
	ispWritel(HORZ_LINE_START << CCDC_HORZ_INFO_SPH_SHIFT | ((ccdcout_w - 1) << CCDC_HORZ_INFO_NPH_SHIFT), CCDC_HORZ_INFO);

	/* Vertical start */
	ispWritel(VERT_LINE_START << CCDC_VERT_START_SLV0_SHIFT, CCDC_VERT_START);

	/* Number of vertical lines */
	ispWritel((ccdcout_h - 1) << CCDC_VERT_LINES_NLV_SHIFT, CCDC_VERT_LINES);

	/* Next address offset */
	u32Val = ccdcout_w * 2;
	if ((u32Val & ISP_32B_BOUNDARY_OFFSET) == u32Val) {
#if CCDC_DEBUG
		printf("setting next offset address........................ 0x%x\n", (u32Val & 0xFFFF));
#endif
		ispWritel((u32Val & 0xFFFF), CCDC_HSIZE_OFF);
	}

	/*0 - By default Donot inverse the field identification */
	ispWritel((ispReadl(CCDC_SDOFST) & (~CCDC_SDOFST_FINV)), CCDC_SDOFST);
	/*0 - By default one line offset */
	ispWritel(ispReadl(CCDC_SDOFST) & (~CCDC_SDOFST_FOFST_4), CCDC_SDOFST);

	ispWritel(((((ccdcout_h >> 1) - 1) & CCDC_VDINT_0_MASK) << CCDC_VDINT_0_SHIFT) | ((50 & CCDC_VDINT_1_MASK)
											  << CCDC_VDINT_1_SHIFT), CCDC_VDINT);

	return 1;
}

void sdram_addr_init(void)
{
	U32 u32Val;
	U32 RetVal;
	/* Already disabled lsc_config */
	u32Val = SD_CAMERA_BUFFER;
	if ((u32Val & ISP_32B_BOUNDARY_BUF) == u32Val) {
#if CCDC_DEBUG
		printf("Loading the address .....\n");
#endif
		ispWritel(u32Val, CCDC_SDR_ADDR);
	} else {
		printf("Address should be 32-byte alligned\n");
		RetVal = FAILURE;
	}

}

void ispccdc_config_yuv422(void)
{
	U32 u32Val = 0;
	U32 u32reg_val = 0;
	U32 ccdcin_woffset = 0;
	U32 ccdcin_hoffset = 0;
	U32 ccdcin_w = 0;
	U32 ccdcin_h = 0;
	U32 ccdcout_w = 0;
	U32 ccdcout_h = 0;

	/* Configure sync mode: output */
	u32Val = ispReadl(CCDC_SYN_MODE);
	u32Val |= CCDC_SYN_MODE_VP2SDR;
	u32Val &= ~CCDC_SYN_MODE_SDR2RSZ;
	u32Val |= CCDC_SYN_MODE_WEN;
	u32Val |= CCDC_SYN_MODE_EXWEN;
	/* ISPCCDC_CFG_WENLOG */
	u32reg_val = 0;
	u32reg_val = ispReadl(CCDC_CFG);
	u32reg_val |= CCDC_CFG_WENLOG;
	ispWritel(u32reg_val, CCDC_CFG);

	/* Enable VP port */
	u32reg_val = 0;
	u32reg_val = ispReadl(CCDC_FMTCFG);
	u32reg_val &= CCDC_FMTCFG_VPIN_MASK & CCDC_FMTCF_VPIF_FRQ_MASK;
	u32reg_val |= (CCDC_FMTCFG_VPIN_11_2 | CCDC_FMTCF_VPIF_FRQ_BY2);
	ispWritel(u32reg_val, CCDC_FMTCFG);

	u32reg_val = 0;
	u32reg_val = ispReadl(CCDC_FMTCFG);
	u32reg_val &= ~CCDC_FMTCFG_VPEN;
	u32reg_val |= 0;
	ispWritel(u32reg_val, CCDC_FMTCFG);

	ispWritel(u32Val, CCDC_SYN_MODE);

	u32Val = 0;
	/* set color pattern */
	ispWritel(u32Val, CCDC_COLPTN);

	/* Configure sync mode: Input */
	u32Val = 0;
	u32Val = ispReadl(CCDC_SYN_MODE);

	/* enable HS and VS signal */
	u32Val |= CCDC_SYN_MODE_VDHDEN;
	/* field select */
	u32Val &= ~CCDC_SYN_MODE_FLDSTAT;

	/*YUV16 data */
	u32Val &= CCDC_SYN_MODE_INPMOD_MASK;
	u32Val |= CCDC_SYN_MODE_INPMOD_YCBCR16;

	/* data size */
	u32Val &= CCDC_SYN_MODE_DATSIZ_MASK;
	u32Val |= CCDC_SYN_MODE_DATSIZ_8BIT;

	/*Progressive mode */
	u32Val &= ~CCDC_SYN_MODE_FLDMODE;

	/*Normal */
	u32Val |= CCDC_SYN_MODE_DATAPOL;

	/*Positive */
	u32Val &= ~CCDC_SYN_MODE_FLDPOL;
	/*Positive */
	u32Val &= ~CCDC_SYN_MODE_HDPOL;
	/*Positive */
	u32Val |= CCDC_SYN_MODE_VDPOL;
	/*fld, hd,vd input signals in slave mode */
	u32Val &= ~(CCDC_SYN_MODE_FLDOUT | CCDC_SYN_MODE_VDHDOUT);

	ispWritel(u32Val, CCDC_SYN_MODE);

	u32Val = 0;

	/* disable BT656IF */
	ispWritel((ispReadl(CCDC_REC656IF)) & (~CCDC_REC656IF_R656ON), CCDC_REC656IF);

     /****** ISPCCDC_CLAMP *****/
	u32Val = 0;
	ispWritel(u32Val, CCDC_DCSUB);
	ispWritel(u32Val, CCDC_BLKCMP);

	/* Disable FPC */
	ispWritel(u32Val, CCDC_FPC);

	/* Disable LSC */
	ispWritel(u32Val, CCDC_LSC_CONFIG);

#if DEBUG_ISP
	isp_reg_dump(void);
#endif

	ccdcin_woffset = 0;
	ccdcin_w = DISPLAY_WIDTH;
	ccdcout_w = DISPLAY_WIDTH;
	ccdcout_h = DISPLAY_HEIGHT;
	ccdcin_hoffset = 0;
	ccdcin_h = DISPLAY_HEIGHT;

#if DEBUG_ISP
	printf("ccdcin_w %d ccdcin_h %d ccdcout_w %d ccdcout_h %d\n  ", ccdcin_w, ccdcin_h, ccdcout_w, ccdcout_h);
#endif

	/* Disable VP */
	ispWritel(0, CCDC_VP_OUT);

	/* Start with 1 pixel apart */
	ispWritel(HORZ_LINE_START << CCDC_HORZ_INFO_SPH_SHIFT | ((ccdcout_w - 1) << CCDC_HORZ_INFO_NPH_SHIFT), CCDC_HORZ_INFO);

	/* Vertical start */
	ispWritel(VERT_LINE_START << CCDC_VERT_START_SLV0_SHIFT, CCDC_VERT_START);

	/* Number of vertical lines */
	ispWritel((ccdcout_h - 1) << CCDC_VERT_LINES_NLV_SHIFT, CCDC_VERT_LINES);

	/* Next address offset */
	u32Val = ccdcout_w * 2;
	if ((u32Val & ISP_32B_BOUNDARY_OFFSET) == u32Val) {
#if CCDC_DEBUG
		printf("setting next offset address........................\n");
#endif
		ispWritel((u32Val & 0xFFFF), CCDC_HSIZE_OFF);
	}

	/*0 - By default Donot inverse the field identification */
	ispWritel((ispReadl(CCDC_SDOFST) & (~CCDC_SDOFST_FINV)), CCDC_SDOFST);
	/*0 - By default one line offset */
	ispWritel(ispReadl(CCDC_SDOFST) & (~CCDC_SDOFST_FOFST_4), CCDC_SDOFST);

	ispWritel((((ccdcout_h - 2) & CCDC_VDINT_0_MASK) << CCDC_VDINT_0_SHIFT) | ((100 & CCDC_VDINT_1_MASK)
										   << CCDC_VDINT_1_SHIFT), CCDC_VDINT);

}

static unsigned char *vga_map_data;

void set_zero_buf(void)
{
	U16 xpixel;
	U16 ypixel;

	bm_lcd_height = DISPLAY_WIDTH;
	bm_lcd_width = DISPLAY_HEIGHT;
	image_address = (U16 *) SD_CAMERA_BUFFER;

	for (ypixel = 0; ypixel < bm_lcd_height; ypixel++) {
		for (xpixel = bm_lcd_width; xpixel > 0; xpixel--) {
			image_address[ypixel * bm_lcd_width + xpixel] = 0xFFFF;
		}
	}
}

void camera_image_dump(void)
{
	U32 ypixel;

	U32 value;
	U32 i = 0;
	unsigned char *image_address_buffer;
#if 1
	unsigned char *image_address_lcd;
	image_address_lcd = (unsigned char *)SD_LCD_BUFFER;
#else
	unsigned short *image_address_lcd;
	image_address_lcd = (unsigned short *)SD_LCD_BUFFER;
#endif

	printf(" %s Function \n", __FUNCTION__);

	image_address = (U16 *) SD_CAMERA_BUFFER;
	image_address_buffer = (unsigned char *)SD_RGB24_BUFFER;
	bm_lcd_height = DISPLAY_WIDTH;
	bm_lcd_width = DISPLAY_HEIGHT;

	for (ypixel = 0; ypixel < 10 /* (bm_lcd_height * bm_lcd_width) */ ;
	     ypixel++)
		printf("0x%hx ", (unsigned char)((image_address[ypixel] >> 2) & 0xff));
	printf("\n\n");

/* buffer */
	i = 0;
	for (ypixel = 0; ypixel < 10 /*(bm_lcd_height * bm_lcd_width) */ ;
	     ypixel++) {
		if ((image_address_buffer[i + 2] & 0x0f) == image_address_buffer[i + 2])
			value = (0xFFFFFF) & (((((image_address_buffer[i + 2] << 16) + (image_address_buffer[i + 1] << 12)) >> 4) + (image_address_buffer[i] << 4)) >> 4);
		else
			value = (0xFFFFFF) & ((image_address_buffer[i + 2] << 16) + (image_address_buffer[i + 1] << 8) + image_address_buffer[i]);

		printf("0x%x ", (unsigned int)value);
		i += 3;
	}

	printf("\n\n");

/* LCD */
#if 1
	i = 0;
	for (ypixel = 0; ypixel < 10 /* (bm_lcd_height * bm_lcd_width) */ ;
	     ypixel++) {
		if ((image_address_lcd[i + 2] & 0x0f) == image_address_lcd[i + 2])
			value = (0xFFFFFF) & (((((image_address_lcd[i + 2] << 16) + (image_address_lcd[i + 1] << 12)) >> 4) + (image_address_lcd[i] << 4)) >> 4);
		else
			value = (0xFFFFFF) & ((image_address_lcd[i + 2] << 16) + (image_address_lcd[i + 1] << 8) + image_address_lcd[i]);

		printf("0x%x ", (unsigned int)value);
		i += 3;
	}
#else
	printf("\n\n");
	for (ypixel = 0; ypixel < 10 /* (bm_lcd_height * bm_lcd_width) */ ;
	     ypixel++)
		printf("0x%hx ", (unsigned short)image_address_lcd[ypixel]);
#endif
}

void set_copy_buf(void)
{
	U32 ypixel;
	unsigned long int i;

	unsigned char *image_address_lcd;
	unsigned char *image_address_buffer;
	unsigned short *image_address_camera;
	unsigned char value;

	image_address_lcd = (unsigned char *)SD_LCD_BUFFER;
	image_address_camera = (U16 *) SD_CAMERA_BUFFER;
	image_address_buffer = (unsigned char *)SD_RGB24_BUFFER;

	bm_lcd_width = DISPLAY_WIDTH;
	bm_lcd_height = DISPLAY_HEIGHT;
	i = 0;
	for (ypixel = 0; ypixel < (bm_lcd_height * bm_lcd_width); ypixel++) {
		value = (unsigned char)((image_address_camera[ypixel] >> 2) & 0xff);
		image_address_buffer[i] = value;
		image_address_buffer[i + 1] = value;
		image_address_buffer[i + 2] = value;
		i += 3;
	}
}

void rgb24_to_rgb16(void)
{
	U32 x, i;

	unsigned char *image_address_buffer = NULL;
#if 1
	unsigned char *image_address_lcd = NULL;
	image_address_lcd = (unsigned char *)SD_LCD_BUFFER;
#else
	unsigned short *image_address_lcd = NULL;
	image_address_lcd = (unsigned short *)SD_LCD_BUFFER;
#endif
	image_address_buffer = (unsigned char *)SD_RGB24_BUFFER;
	i = 0;
#if DEBUG_ISP
	printf("SD_RGB24_BUFFER: 0x%x SD_LCD_BUFFER 0x%x\n", SD_RGB24_BUFFER, SD_LCD_BUFFER);
#endif

	for (x = 0; x < (DISPLAY_WIDTH * DISPLAY_HEIGHT); ++x) {
#if 1
		image_address_lcd[i] = image_address_buffer[i];
		image_address_lcd[i + 1] = image_address_buffer[i + 1];
		image_address_lcd[i + 2] = image_address_buffer[i + 2];
		i += 3;
#else
		if ((image_address_buffer[i + 2] & 0x0f) == image_address_buffer[i + 2])
			value = (0xFFFFFF) & (((((image_address_buffer[i + 2] << 16) + (image_address_buffer[i + 1] << 12)) >> 4) + (image_address_buffer[i] << 4)) >> 4);
		else
			value = (0xFFFFFF) & (image_address_buffer[i + 2] << 16) + (image_address_buffer[i + 1] << 8) + image_address_buffer[i];

		val8 = &value;
		image_address_lcd[x] = RGB24_TO_RGB16(val8[2], val8[1], val8[0]);
		i += 3;
		if (x < 10)
			printf("0x%x 0x%hx ", value, image_address_lcd[x]);

#endif
	}
}

void set_copy_buf_rgb565(void)
{
	U32 ypixel;
	unsigned short *image_address_lcd;
	unsigned short *image_address_camera;
	bm_lcd_height = DISPLAY_HEIGHT;
	bm_lcd_width = DISPLAY_WIDTH;
	image_address_lcd = (unsigned short *)SD_LCD_BUFFER;
	image_address_camera = (U16 *) SD_CAMERA_BUFFER;
	for (ypixel = 0; ypixel < (bm_lcd_height * bm_lcd_width); ypixel++) {
		image_address_lcd[ypixel] = image_address_camera[ypixel];
	}
}

static void yuv_to_rgb16(unsigned char y, unsigned char u, unsigned char v, unsigned char *rgb)
{
	register int r, g, b;
	int rgb16;

	r = (1192 * (y - 16) + 1634 * (v - 128)) >> 10;
	g = (1192 * (y - 16) - 833 * (v - 128) - 400 * (u - 128)) >> 10;
	b = (1192 * (y - 16) + 2066 * (u - 128)) >> 10;

	r = r > 255 ? 255 : r < 0 ? 0 : r;
	g = g > 255 ? 255 : g < 0 ? 0 : g;
	b = b > 255 ? 255 : b < 0 ? 0 : b;

	rgb16 = (int)(((r >> 3) << 11) | ((g >> 2) << 5) | ((b >> 3) << 0));

	*rgb = (unsigned char)(rgb16 & 0xFF);
	rgb++;
	*rgb = (unsigned char)((rgb16 & 0xFF00) >> 8);

}

void yuyv422_to_rgb16(unsigned char *buf, unsigned char *rgb, int width, int height)
{
	int y;
	int blocks;

	blocks = (width * height) * 2;

	for (y = 0; y < blocks; y += 4) {
		unsigned char Y1, Y2, U, V;

		Y1 = buf[y + 0];
		U = buf[y + 1];
		Y2 = buf[y + 2];
		V = buf[y + 3];

		yuv_to_rgb16(Y1, U, V, &rgb[y]);
		yuv_to_rgb16(Y2, U, V, &rgb[y + 2]);
	}
}

void cam_display_image(void)
{
	S32 ret_val;
	lcd_drv_struct_t lcd_init_struct;

	lcd_init_struct.mode = VGA;
	bm_lcd_height = DISPLAY_HEIGHT;
	bm_lcd_width = DISPLAY_WIDTH;
	image_address = (U16 *) SD_LCD_BUFFER;

	vga_map_data = (unsigned char *)SD_LCD_BUFFER;

	/* Initialize the LCD. */
	ret_val = dg_lcd_init(&lcd_init_struct);

	if (SUCCESS != ret_val) {
		printf("%s:%d - LCD initialization failed.\n\r", __FILE__, __LINE__);

		return;
	}
	printf("Displaying image...... \n");

}

#if 0
void isp_reg_dump(void)
{
	printf("###CCDC PCR=0x%x\n", ispReadl(CCDC_PCR));
	printf("###ISP_CTRL in ccdc =0x%x\n", ispReadl(ISP_CTRL));
	printf("###ISP_IRQ0ENABLE in ccdc =0x%x\n", ispReadl(ISP_IRQ0ENABLE));
	printf("###ISP_IRQ0STATUS in ccdc =0x%x\n", ispReadl(ISP_IRQ0STATUS));
	printf("###CCDC SYN_MODE=0x%x\n", ispReadl(CCDC_SYN_MODE));
	printf("###CCDC HORZ_INFO=0x%x\n", ispReadl(CCDC_HORZ_INFO));
	printf("###CCDC VERT_START=0x%x\n", ispReadl(CCDC_VERT_START));
	printf("###CCDC VERT_LINES=0x%x\n", ispReadl(CCDC_VERT_LINES));
	printf("###CCDC CULLING=0x%x\n", ispReadl(CCDC_CULLING));
	printf("###CCDC HSIZE_OFF=0x%x\n", ispReadl(CCDC_HSIZE_OFF));
	printf("###CCDC SDOFST=0x%x\n", ispReadl(CCDC_SDOFST));
	printf("###CCDC SDR_ADDR=0x%x\n", ispReadl(CCDC_SDR_ADDR));
	printf("###CCDC CLAMP=0x%x\n", ispReadl(CCDC_CLAMP));
	printf("###CCDC COLPTN=0x%x\n", ispReadl(CCDC_COLPTN));
	printf("###CCDC CFG=0x%x\n", ispReadl(CCDC_CFG));
	printf("###CCDC VP_OUT=0x%x\n", ispReadl(CCDC_VP_OUT));
	printf("###CCDC_SDR_ADDR= 0x%x\n", ispReadl(CCDC_SDR_ADDR));
	printf("###CCDC FMTCFG=0x%x\n", ispReadl(CCDC_FMTCFG));
	printf("###CCDC FMT_HORZ=0x%x\n", ispReadl(CCDC_FMT_HORZ));
	printf("###CCDC FMT_VERT=0x%x\n", ispReadl(CCDC_FMT_VERT));
	printf("###CCDC LSC_CONFIG=0x%x\n", ispReadl(CCDC_LSC_CONFIG));
	printf("###CCDC LSC_INIT=0x%x\n", ispReadl(CCDC_LSC_INITIAL));
	printf("###CCDC LSC_TABLE BASE=0x%x\n", ispReadl(CCDC_LSC_TABLE_BASE));
	printf("###CCDC LSC TABLE OFFSET=0x%x\n", ispReadl(CCDC_LSC_TABLE_OFFSET));

	printf("###CCDC_VDINT 0x%x\n", ispReadl(CCDC_VDINT));
	printf("###ISP_TCTRL_CTRL=0x%x\n", ispReadl(CAM_TCTRL_CTRL));
	printf("###ISP_SYSCONFIG=0x%x\n", ispReadl(ISP_SYSCONFIG));
}
#endif

void poll_interrupt(void)
{
	u32 u32IrqStatus = 0;
	U32 i;

	for (i = 0; i < 1; i++) {

		/* Read ISP IRQ0 status register */
		u32IrqStatus = ispReadl(ISP_IRQ0STATUS);
#if DEBUG_ISP
		printf("IRQ status %x .........\n", u32IrqStatus);
#endif
		/* Clear the IRQ0 status */
		ispWritel(u32IrqStatus, ISP_IRQ0STATUS);

		printf("Waiting for HS VS interrupt .........\n");
		do {
			if ((u32IrqStatus & HS_VS) == HS_VS) {
				printf("Received HSVS interrupt\n");
				break;
			}
			u32IrqStatus = ispReadl(ISP_IRQ0STATUS);
			ispWritel(u32IrqStatus, ISP_IRQ0STATUS);
		} while (1);

		printf("Waiting for VD0 interrupt .........\n");
		u32IrqStatus = ispReadl(ISP_IRQ0STATUS);

		/* Clear the IRQ0 status */
		ispWritel(u32IrqStatus, ISP_IRQ0STATUS);

		do {
			if ((u32IrqStatus & CCDC_VD0) == CCDC_VD0) {
				printf("Received VD0 buffer interrupt\n");
				break;
			}
			u32IrqStatus = ispReadl(ISP_IRQ0STATUS);
			ispWritel(u32IrqStatus, ISP_IRQ0STATUS);
		} while (1);
#if DEBUG_ISP
		printf("IRQ status %x .........\n", u32IrqStatus);
#endif
		/* Added to clear post write interrupt */
		u32IrqStatus = ispReadl(ISP_IRQ0STATUS);

		/* Clear the IRQ0 status */
		ispWritel(u32IrqStatus, ISP_IRQ0STATUS);
		u32IrqStatus = 0;

	}
}

void ispccdc_enable(int enable)
{
	unsigned int u32Val = 0;
	u32Val = ispReadl(CCDC_PCR);
	if (enable == 0) {
		u32Val &= 0xfffffffe;
#if DEBUG_ISP
		printf("\nDisable CCDC");
#endif
		while ((ispReadl(CCDC_PCR)) & 0x1) {
			ispWritel(u32Val, CCDC_PCR);	// disable CCDC module
			printf(".");
		}
	} else if (enable == 1) {
		u32Val |= 0x1;
#if DEBUG_ISP
		printf("\nEnable CCDC");
#endif
		while (((ispReadl(CCDC_PCR)) & 0x1) != 0x1) {
			ispWritel(u32Val, CCDC_PCR);	// enable CCDC module
			printf(".");
		}
	} else {
		printf("Invalid option\n");
		return;
	}
#if DEBUG_ISP
	printf("\nCCDC_PCR  0x%x\n", ispReadl(CCDC_PCR));
#endif
	ispReadl(CCDC_PCR);
	printf("\n");
	udelay(1000);
}

void is_ccdc_busy(void)
{
	printf("\nccdc busy");
	while ((ispReadl(CCDC_PCR) & 0x2)) {
		printf(".");
		udelay(100);
	}
	printf("\n");
}

int config_sensor(void)
{

	ispccdc_enable(0);
#ifdef USE_YUV422
	ispccdc_config_yuv422();
#endif

#ifdef COLOR_BAR
	ispccdc_config_yuv422();
#endif

#ifdef USE_RAW
	isp_ccdc_setup();
#endif

#ifdef USE_RGB565
	isp_ccdc_setup();
#endif

	ispccdc_request();
	sdram_addr_init();

	return 1;
}

int capture_display_image(void)
{
	ispccdc_enable(1);

	isp_enable_irq();
	poll_interrupt();

	ispccdc_enable(0);
#if DEBUG_ISP
	isp_reg_dump();
#endif

#ifdef USE_RAW
	set_copy_buf();
	rgb24_to_rgb16();
#endif

#ifdef USE_RGB565
	set_copy_buf_rgb565();
#endif

#ifdef USE_YUV422
	yuyv422_to_rgb16((unsigned char *)SD_CAMERA_BUFFER, (unsigned char *)SD_LCD_BUFFER, DISPLAY_WIDTH, DISPLAY_HEIGHT);
#endif

#ifdef COLOR_BAR
	yuyv422_to_rgb16((unsigned char *)SD_CAMERA_BUFFER, (unsigned char *)SD_LCD_BUFFER, DISPLAY_WIDTH, DISPLAY_HEIGHT);
#endif
	mt9t111_lcd = 1;
	cam_display_image();

	mt9t111_lcd = 0;
	return 1;
}

int mt9t111_def_reg_load(mt9t111_regs * reg_in, int cnt)
{
	int i;
	int try_count;
	mt9t111_regs *reg = reg_in;

	for (i = 0; i < cnt; i++) {
		try_count = 0;

		if (reg->delay_time == 0) {
			camera_i2c_2byte_write(reg->addr, reg->data, 4);
			udelay(3000);
		} else if (reg->addr != 0 || reg->data != 0) {
			camera_i2c_2byte_write(reg->addr, reg->data, 4);
			udelay(reg->delay_time);
		} else {
			udelay(reg->delay_time);
		}

		reg++;
	}

	return 0;
}
