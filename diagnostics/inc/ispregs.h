/*
 * ispregs.h: ISP registers defines.
 *
 * (C) Copyright 2010
 * Mistral Solutions Private Limited, <www.mistralsolutions.com>
 *
 * Author :
 * Ganeshan <nganeshan@mistralsolutions.com>
 *
 * Derived from linux kernel source
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
 *   12 jan 2010 - Created from OMAP2 ITBOK source.
 */

#ifndef __OMAP3_ISPREGS_H__
#define __OMAP3_ISPREGS_H__

#define PM_PWSTCTRL_CAM		0x48306FE0

/* ISP offset */
#define ISP_REG_BASE			0x480BC000
#define ISP_REG_END				0x480BDB70
#define ISP_REG_SIZE			0x00001600

/* ISP module registers offset */
#define ISP_SYSCONFIG			0x480BC004
#define ISP_IRQ0ENABLE			0x480BC00C
#define ISP_IRQ0STATUS			0x480BC010
#define ISP_IRQ1ENABLE			0x480BC014
#define ISP_IRQ1STATUS			0x480BC018
#define ISP_CTRL				0x480BC040

/* CCDC module registers offset */
#define CCDC_PCR				0x480BC604
#define CCDC_SYN_MODE			0x480BC608
#define CCDC_HD_VD_WID			0x480BC60C
#define CCDC_PIX_LINES			0x480BC610
#define CCDC_HORZ_INFO			0x480BC614
#define CCDC_VERT_START			0x480BC618
#define CCDC_VERT_LINES			0x480BC61C
#define CCDC_CULLING			0x480BC620
#define CCDC_HSIZE_OFF			0x480BC624
#define CCDC_SDOFST				0x480BC628
#define CCDC_SDR_ADDR			0x480BC62C
#define CCDC_CLAMP				0x480BC630
#define CCDC_DCSUB				0x480BC634
#define CCDC_COLPTN				0x480BC638
#define CCDC_BLKCMP				0x480BC63C
#define CCDC_FPC				0x480BC640
#define CCDC_FPC_ADDR			0x480BC644
#define CCDC_VDINT				0x480BC648
#define CCDC_ALAW				0x480BC64C
#define CCDC_REC656IF			0x480BC650
#define CCDC_CFG				0x480BC654
#define CCDC_VP_OUT				0x480BC694
#define CCDC_FMTCFG   		    0x480BC658

/* VD0, VD1 and HV sync interrupts */
#define VD0_IRQ0_ENABLE					(1<<8)
#define VD1_IRQ0_ENABLE					(1<<9)
#define PREVIEW_DONE_IRQ				(1<<20)
#define HS_VS_IRQ0_ENABLE				(1<<31)
#define IRQ0_OVF_ENABLE					(1<<25)
#define IRQ0_LSC_DONE_IRQ       	 	(1 << 17)
#define IRQ0_LSC_PREF_COMP_IRQ			(1 << 18)
#define IRQ0_LSC_PREF_ERR_IRQ			(1 << 19)
#define ISP_PAR_SER_CLK_SEL_parallel	0x0
#define ISP_PAR_SER_CLK_SEL_MASK		0xFFFFFFFC

/* ISP control Sync detect */
#define ISP_PAR_BRIDGE_SHIFT		2
#define ISP_PAR_BRIDGE_BENDIAN		(0x3 << 2)
#define ISP_PAR_CLK_POL_SHIFT		4
#define ISP_PAR_CLK_POL_INV			(1 << 4)
#define ISP_SHIFT_SHIFT				6
#define ISP_SHIFT_MASK				(~(0x3 << 6))
#define ISP_CCDC_CLK_EN				(1 << 8)
#define ISP_SYNC_DETECT_SHIFT		14
#define ISP_SYNC_DETECT_VSRISE		0x3
#define ISP_CCDC_RAM_EN     		(1 << 16)
#define ISP_SBL_WR1_RAM_EN  		(1 << 19)
#define ISP_SBL_WR0_RAM_EN  		(1 << 20)
#define ISP_SBL_SHARED_RPORTB		(1 << 28)
#define ISP_SBL_RD_RAM_EN			(1 << 18)
#define ISP_PREV_RAM_EN				(1 << 17)
#define ISP_CTRL_SET_NOSHIFT        (~(0x3 << 6))
/* SYNC mode register bits  */
#define CCDC_SYN_MODE_VDHDOUT		0x1
#define CCDC_SYN_MODE_FLDOUT		(1 << 1)
#define CCDC_SYN_MODE_VDPOL			(1 << 2)
#define CCDC_SYN_MODE_HDPOL			(1 << 3)
#define CCDC_SYN_MODE_FLDPOL		(1 << 4)
#define CCDC_SYN_MODE_EXWEN			(1 << 5)
#define CCDC_SYN_MODE_DATAPOL  		(1 << 6)
#define CCDC_SYN_MODE_FLDMODE		(1 << 7)
#define CCDC_SYN_MODE_DATSIZ_MASK	0xFFFFF8FF
#define CCDC_SYN_MODE_DATSIZ_10BIT	(0x6 << 8)
#define CCDC_SYN_MODE_DATSIZ_8BIT	(0x7 << 8)
#define CCDC_SYN_MODE_PACK16        (~(1< 10))
#define CCDC_SYN_MODE_INPMOD_MASK	0xFFFFCFFF
#define CCDC_SYN_MODE_INPMOD_RAW	(0 << 12)
#define CCDC_SYN_MODE_LPF			(1 << 14)
#define CCDC_SYN_MODE_FLDSTAT		(1 << 15)
#define CCDC_SYN_MODE_VDHDEN		(1 << 16)
#define CCDC_SYN_MODE_WEN			(1 << 17)
#define CCDC_SYN_MODE_VP2SDR		(1 << 18)
#define CCDC_SYN_MODE_SDR2RSZ		(1 << 19)
#define CCDC_SYN_MODE_INPMOD_YCBCR16     (1 << 12)
#define CCDC_SYN_MODE_INPMOD_YCBCR8      (2 << 12)

/* CCDC horizontal mask and bits */
#define CCDC_HORZ_INFO_NPH_SHIFT	0
#define CCDC_HORZ_INFO_NPH_MASK		0xFFFF8000
#define CCDC_HORZ_INFO_SPH_MASK		0x1000FFFF
#define CCDC_HORZ_INFO_SPH_SHIFT	16

/* CCDC vertical mask and bits */
#define CCDC_VERT_START_SLV0_SHIFT	16
#define CCDC_VERT_START_SLV0_MASK	0x1000FFFF
#define CCDC_VERT_START_SLV1_SHIFT	0
#define CCDC_VERT_LINES_NLV_MASK	0xFFFF8000
#define CCDC_VERT_LINES_NLV_SHIFT	0

/* horizontal bit shift */
#define CCDC_HSIZE_OFF_SHIFT		0

/* CCDC next line offset bits */
#define CCDC_SDOFST_FINV			(1 << 14)
#define CCDC_SDOFST_FOFST_1			(~(3 << 12))
#define CCDC_SDOFST_FOFST_4			(3 << 12)
#define CCDC_SDOFST_LOFST3_SHIFT	0
#define CCDC_SDOFST_LOFST2_SHIFT	3
#define CCDC_SDOFST_LOFST1_SHIFT	6
#define CCDC_SDOFST_LOFST0_SHIFT	9

/* CCDC vd0 and vd1 interrutps bits */
#define CCDC_VDINT_1_SHIFT		0
#define CCDC_VDINT_0_SHIFT		16
#define CCDC_VDINT_0_MASK		0x7FFF
#define CCDC_VDINT_1_MASK		0x7FFF
#define CCDC_CFG_WENLOG     	(1 << 8)
#define CCDC_REC656IF_R656ON	1
#define CCDC_CFG_VDLC        	(1 << 15)


/* Optical clamp mode bit sets */
#define CCDC_CLAMP_OBGAIN_SHIFT	0
#define CCDC_CLAMP_OBST_SHIFT	10
#define CCDC_CLAMP_OBSLN_SHIFT	25
#define CCDC_CLAMP_OBSLEN_SHIFT	28
#define CCDC_CLAMP_CLAMPEN		(1 << 31)

/* Black clamp mode bit sets */
#define CCDC_BLKCMP_B_MG_SHIFT   0
#define CCDC_BLKCMP_GB_G_SHIFT   8
#define CCDC_BLKCMP_GR_CY_SHIFT  6
#define CCDC_BLKCMP_R_YE_SHIFT   24

/* FPC mode bit sets */
#define CCDC_FPC_FPNUM_SHIFT	 0
#define CCDC_FPC_FPCEN      	 (1 << 15)
#define CCDC_FPC_FPERR      	 (1 << 16)

/* LSC configuraion Registers */
#define CCDC_LSC_CONFIG			0x480BC698
#define CCDC_LSC_INITIAL 		0x480BC69C
#define CCDC_LSC_TABLE_BASE		0x480BC6A0
#define CCDC_LSC_TABLE_OFFSET	0x480BC6A4

/* LSC gain mode bit sets */
#define CCDC_LSC_GAIN_MODE_N_MASK	    0x700
#define CCDC_LSC_GAIN_MODE_N_SHIFT  	 8
#define CCDC_LSC_GAIN_MODE_M_MASK    	0x3800
#define CCDC_LSC_GAIN_MODE_M_SHIFT  	 12
#define CCDC_LSC_GAIN_FORMAT_MASK   	 0xE
#define CCDC_LSC_GAIN_FORMAT_SHIFT  	1
#define CCDC_LSC_AFTER_REFORMATTER_MASK	(1<<6)
#define CCDC_LSC_INITIAL_X_MASK			0x3F
#define CCDC_LSC_INITIAL_X_SHIFT		0
#define CCDC_LSC_INITIAL_Y_MASK			0x3F0000
#define CCDC_LSC_INITIAL_Y_SHIFT		16

/* Culling factors */
#define CCDC_CULLING_CULV_SHIFT		0
#define CCDC_CULLING_CULHODD_SHIFT	16
#define CCDC_CULLING_CULHEVN_SHIFT	24

/* camera clock definition*/
#define CM_FCLKEN_CAM			0x48004f00
#define CM_ICLKEN_CAM			0x48004f10
#define CM_AUTOIDLE_CAM			0x48004f30
#define CM_CLKSEL_CAM			0x48004f40
#define CM_CLKEN_PLL			0x48004D00
#define CM_CLKSEL2_PLL			0x48004D44
#define CTRL_PADCONF_CAM_HS		0x4800210C
#define CTRL_PADCONF_CAM_XCLKA	0x48002110
#define CTRL_PADCONF_CAM_D1		0x48002118
#define CTRL_PADCONF_CAM_D3		0x4800211C
#define CTRL_PADCONF_CAM_D5		0x48002120
#define CTRL_PADCONF_CAM_D7		0x48002124
#define CTRL_PADCONF_CAM_D9		0x48002128
#define CTRL_PADCONF_CAM_D11	0x4800212C

/* ISP module clock enable */
#define ISP_CCDC_CLK_EN     (1 << 8)
#define ISP_SCMP_CLK_EN     (1 << 9)
#define ISP_H3A_CLK_EN      (1 << 10)
#define ISP_HIST_CLK_EN     (1 << 11)
#define ISP_PREV_CLK_EN     (1 << 12)
#define ISP_RSZ_CLK_EN      (1 << 13)

/* preview registers and bit settings */
#define PREVIEW_PCR				0x480BCE04
#define PREVIEW_HORZ_INFO		0x480BCE08
#define PREVIEW_VERT_INFO		0x480BCE0C
#define PREVIEW_RSDR_ADDR		0x480BCE10
#define PREVIEW_RADR_OFFSET		0x480BCE14
#define PREVIEW_DSDR_ADDR		0x480BCE18
#define PREVIEW_DRKF_OFFSET		0x480BCE1C
#define PREVIEW_WSDR_ADDR		0x480BCE20
#define PREVIEW_WADD_OFFSET		0x480BCE24
#define PREVIEW_AVE				0x480BCE28
#define PREVIEW_HMED			0x480BCE2C
#define PREVIEW_NF				0x480BCE30
#define PREVIEW_WB_DGAIN		0x480BCE34
#define PREVIEW_WBGAIN			0x480BCE38
#define PREVIEW_WBSEL			0x480BCE3C
#define PREVIEW_CFA				0x480BCE40
#define PREVIEW_BLKADJOFF		0x480BCE44
#define PREVIEW_RGB_MAT1		0x480BCE48
#define PREVIEW_RGB_MAT2		0x480BCE4C
#define PREVIEW_RGB_MAT3		0x480BCE50
#define PREVIEW_RGB_MAT4		0x480BCE54
#define PREVIEW_RGB_MAT5		0x480BCE58
#define PREVIEW_RGB_OFF1		0x480BCE5C
#define PREVIEW_RGB_OFF2		0x480BCE60
#define PREVIEW_CSC0			0x480BCE64
#define PREVIEW_CSC1			0x480BCE68
#define PREVIEW_CSC2			0x480BCE6C
#define PREVIEW_CSC_OFFSET		0x480BCE70
#define PREVIEW_CNT_BRT			0x480BCE74
#define PREVIEW_CSUP			0x480BCE78
#define PREVIEW_SETUP_YC		0x480BCE7C
#define PREVIEW_SET_TBL_ADDR	0x480BCE80
#define PREVIEW_SET_TBL_DATA	0x480BCE84

/* Preview control bits */
#define PREVIEW_PCR_EN				1
#define PREVIEW_PCR_SOURCE			(1<<2)
#define PREVIEW_PCR_ONESHOT			(1<<3)
#define PREVIEW_PCR_YCPOS_YCrYCb 	(0<<17)
#define PREVIEW_PCR_YCPOS_YCbYCr 	(1<<17)
#define PREVIEW_PCR_YCPOS_CbYCrY 	(2<<17)
#define PREVIEW_PCR_YCPOS_CrYCbY 	(3<<17)
#define PREVIEW_PCR_RSZPORT      	(1<<19)
#define PREVIEW_PCR_SDRPORT      	(1<<20)
#define PREVIEW_PCR_SCOMP_EN     	(1<<21)
#define PREVIEW_PCR_GAMMA_BYPASS 	(1<<26)
#define PREVIEW_PCR_YCPOS_SHIFT     17

/* Preview Horizontal bits settings */
#define PREVIEW_HORZ_INFO_EPH_SHIFT  0
#define PREVIEW_HORZ_INFO_EPH_MASK   0x3fff;
#define PREVIEW_HORZ_INFO_SPH_SHIFT  16
#define PREVIEW_HORZ_INFO_SPH_MASK   0x3fff0

/* Preview Vertical bits settings */
#define PREVIEW_VERT_INFO_ELV_SHIFT  0
#define PREVIEW_VERT_INFO_ELV_MASK   0x3fff
#define PREVIEW_VERT_INFO_SLV_SHIFT  16
#define PREVIEW_VERT_INFO_SLV_MASK   0x3fff0

/* VP configuration */
#define CCDC_FMTCFG					0x480BC658
#define CCDC_FMT_HORZ				0x480BC65C
#define CCDC_FMT_VERT				0x480BC660

/* Video Port bit settings */
#define CCDC_FMTCFG_FMTEN			0x1
#define CCDC_FMTCFG_LNALT			(1 << 1)
#define CCDC_FMTCFG_LNUM_SHIFT		2
#define CCDC_FMTCFG_PLEN_ODD_SHIFT	4
#define CCDC_FMTCFG_PLEN_EVEN_SHIFT	8
#define CCDC_FMTCFG_VPIN_MASK		0xFFFF8000
#define CCDC_FMTCFG_VPIN_12_3		(0x3 << 12)
#define CCDC_FMTCFG_VPIN_11_2		(0x4 << 12)
#define CCDC_FMTCFG_VPIN_10_1		(0x5 << 12)
#define CCDC_FMTCFG_VPIN_9_0		(0x6 << 12)
#define CCDC_FMTCFG_VPEN			(1 << 15)
#define CCDC_FMTCF_VPIF_FRQ_MASK	0xFFF8FFFF
#define CCDC_FMTCF_VPIF_FRQ_BY2		(0x0 << 16)
#define CCDC_FMTCF_VPIF_FRQ_BY3		(0x1 << 16)
#define CCDC_FMTCF_VPIF_FRQ_BY4		(0x2 << 16)
#define CCDC_FMTCF_VPIF_FRQ_BY5		(0x3 << 16)
#define CCDC_FMTCF_VPIF_FRQ_BY6		(0x4 << 16)
#define CCDC_FMT_HORZ_FMTLNH_SHIFT	0
#define CCDC_FMT_HORZ_FMTSPH_SHIFT	16
#define CCDC_FMT_VERT_FMTLNV_SHIFT	0
#define CCDC_FMT_VERT_FMTSLV_SHIFT	16
#define CCDC_FMT_HORZ_FMTSPH_MASK	0x1FFF0000
#define CCDC_FMT_HORZ_FMTLNH_MASK	0x1FFF
#define CCDC_FMT_VERT_FMTSLV_MASK	0x1FFF0000
#define CCDC_FMT_VERT_FMTLNV_MASK	0x1FFF
#define CCDC_VP_OUT_HORZ_ST_SHIFT	0
#define CCDC_VP_OUT_HORZ_NUM_SHIFT	4
#define CCDC_VP_OUT_VERT_NUM_SHIFT	17

#define ISPCCDC_FMT_HORZ_FMTLNH_SHIFT       0
#define ISPCCDC_FMT_HORZ_FMTSPH_SHIFT       16

#define ISPCCDC_FMT_VERT_FMTLNV_SHIFT       0
#define ISPCCDC_FMT_VERT_FMTSLV_SHIFT       16


/* Gamma Table Address */
#define PREVIEW_REDGAMMA_TABLE_ADDR		0x0000
#define PREVIEW_GREENGAMMA_TABLE_ADDR	0x0400
#define PREVIEW_BLUEGAMMA_TABLE_ADDR	0x0800
#define PREVIEW_NF_TABLE_ADDR			0x0C00
#define PREVIEW_YENH_TABLE_ADDR			0x1000
#define PREVIEW_CFA_TABLE_ADDR			0x1400

#define	HORZ_LINE_START		0
#define	VERT_LINE_START		0
#define	DEF_VD0INT_MASK		25
#define	DEF_VD1INT_MASK		50

#define	HORZ_PREV_LINE_START		4
#define	START_PREV_VERT_LINE		0
#define OFFSET_MASK					0xFFFF

/* Define bit fields within selected registers */
#define ISP_REVISION_SHIFT          0

#define ISP_SYSCONFIG_AUTOIDLE          0
#define ISP_SYSCONFIG_SOFTRESET         (1 << 1)
#define ISP_SYSCONFIG_MIDLEMODE_SHIFT       12
#define ISP_SYSCONFIG_MIDLEMODE_FORCESTANDBY    0x0
#define ISP_SYSCONFIG_MIDLEMODE_NOSTANBY    0x1
#define ISP_SYSCONFIG_MIDLEMODE_SMARTSTANDBY    0x2

#define ISPMMU_MMUCNTL_MMU_EN           (1<<1)
#define ISPMMU_MMUCNTL_TWL_EN           (1<<2)
#define ISPMMU_MMUCNTL_EMUTLBUPDATE     (1<<3)
#define ISPMMU_AUTOIDLE             0x1
#define ISPMMU_SIDLEMODE_FORCEIDLE      0
#define ISPMMU_SIDLEMODE_NOIDLE         1
#define ISPMMU_SIDLEMODE_SMARTIDLE      2
#define ISPMMU_SIDLEMODE_SHIFT          3

#define ISPMMU_SYSCONFIG        (0x010)

#define CCDC_CFG_Y8POS           (1 << 11)
#define CCDC_CFG_BSWD            (1 << 12)

/*! \enum EN_ISP_IRQ_EVENTS
 * ISP interrupts event
 */
typedef enum{
    CCDC_VD0 = 0x100,
    CCDC_VD1 = 0x200,
    CDC_VD2 = 0x400,
    CCDC_ERR = 0x800,
    H3A_AWB_DONE = 0x2000,
    H3A_AF_DONE = 0x1000,
    HIST_DONE = 0x10000,
    PREV_DONE = 0x100000,
    LSC_DONE = 0x20000,
    LSC_PRE_COMP = 0x40000,
    LSC_PRE_ERR = 0x80000,
    RESZ_DONE = 0x1000000,
    SBL_OVF = 0x2000000,
    MMU_ERR = 0x10000000,
    OCP_ERR = 0x20000000,
    HS_VS = 0x80000000
}EN_ISP_IRQ_EVENTS;

#endif	/* __OMAP3_ISPREGS_H__ */
