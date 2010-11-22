/*
 * video input isp file.
 *
 * (C) Copyright 2010
 * Mistral Solutions Private Limited, <www.mistralsolutions.com>
 *
 * Author :
 *	Ganeshan <nganeshan@mistralsolutions.com>
 *
 * Derived from NA
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
 *   14 Jul 2009 - Created 
 */

#include <common.h>
#include "types.h"
#include "../inc/ispregs.h"

#define DISPLAY_WIDTH 		640
#define DISPLAY_HEIGHT		480


#define USE_RAW
//#undef USE_RAW

#define COLOR_BAR
#undef COLOR_BAR

#define USE_YUV422
#undef USE_YUV422

#define USE_RGB565
#undef USE_RGB565



#define DEBUG_ISP		0

#define SD_CAMERA_BUFFER 0x80530000

#define SD_VIDEOIN_BUFFER1 0x80530000
/* The length is calculate as 720 * 640 * 2 */
#define SD_VIDEOIN_BUFFER2 0x80611000


#define ISP_32B_BOUNDARY_BUF    0xFFFFFFE0
#define ISP_32B_BOUNDARY_OFFSET 0x0000FFE0
#define DEFAULT_VD1_VALUE       50
#define CCDC_CFG_SET            (0x8100)


/* ISP register read write functions */
#define ispReadb(a)         (*(volatile unsigned char  *)(a))
#define ispReadw(a)         (*(volatile unsigned short *)(a))
#define ispReadl(a)         (*(volatile unsigned int   *)(a))
#define ispWriteb(v,a)      (*(volatile unsigned char  *)(a) = (v))
#define ispWritew(v,a)      (*(volatile unsigned short *)(a) = (v))
#define ispWritel(v,a)      (*(volatile unsigned int   *)(a) = (v))


static int videioin_ccdc_setup(void)
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

#define SD_LCD_BUFFER    0x80377000
static void videoin_sdram_addr_init(void)
{
	U32 u32Val;
	U32 RetVal;
	/* Already disabled lsc_config */
	u32Val = SD_VIDEOIN_BUFFER1;

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

void videoin_isp_power_set(void)
{
    ispWritel((ISP_SYSCONFIG_AUTOIDLE | (ISP_SYSCONFIG_MIDLEMODE_SMARTSTANDBY << ISP_SYSCONFIG_MIDLEMODE_SHIFT)), ISP_SYSCONFIG);

    /* MMU setting not required */
}

#define CCDC_DISABLE	0
#define CCDC_ENABLE		1

void videoin_ispccdc_enable(int enable)
{
    unsigned int u32Val = 0;
    u32Val = ispReadl(CCDC_PCR);
    if (enable == 0) {
        u32Val &= 0xfffffffe;
#if DEBUG_ISP
        printf("\nDisable CCDC");
#endif
        while ((ispReadl(CCDC_PCR)) & 0x1) {
            ispWritel(u32Val, CCDC_PCR);    // disable CCDC module
            printf(".");
        }
    } else if (enable == 1) {
        u32Val |= 0x1;
#if DEBUG_ISP
        printf("\nEnable CCDC");
#endif
        while (((ispReadl(CCDC_PCR)) & 0x1) != 0x1) {
            ispWritel(u32Val, CCDC_PCR);    // enable CCDC module
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

void videoin_isp_enable_irq(void)
{
    U32 u32Val = 0;
    u32Val = 0x0;
    ispWritel(-1, ISP_IRQ0STATUS);
    /* Read the IRQ0 Enable Reg and Update */
    u32Val |= HS_VS_IRQ0_ENABLE | VD0_IRQ0_ENABLE | IRQ0_OVF_ENABLE;

    ispWritel(u32Val, ISP_IRQ0ENABLE);
}

void videoin_ccdc_config_yuv422(void)
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


static void videoin_ispccdc_request(void)
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

int videoin_isp_config(void)
{

	videoin_ispccdc_enable(0);
#ifdef USE_YUV422
    videoin_ccdc_config_yuv422();
#endif

#ifdef COLOR_BAR
    videoin_ccdc_config_yuv422();
#endif

#ifdef USE_RAW
    videioin_ccdc_setup();
#endif

#ifdef USE_RGB565
    videioin_ccdc_setup();
#endif

    videoin_ispccdc_request();

    return 1;
}


void videoin_isp_init(void)
{

    videoin_isp_power_set();

	videoin_isp_config();

	videoin_sdram_addr_init();
}

#define CAM_TCTRL_CTRL 0x480BC050
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

