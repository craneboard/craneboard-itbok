/*
 * omap3530evm.h: OMAP3530 EVM specific definitions
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

#ifndef __MS_DIAG_OMAP3530EVM_H__
#define __MS_DIAG_OMAP3530EVM_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <common.h>
#include "types.h"

#ifdef OMAP3530_DEBUG
#define DEBUG(fmt, args...)	fprintf(stdout, fmt, ##args)
#else
#define DEBUG(fmt, args...)
#endif

#define CLEAR_NONE          0
#define SET_NONE            0

#define CLK32K_COUNTER_REGISTER     ((volatile U32 *)0x48320010)		//0x49020010)

/* Select the SDRAM speed. */
#define SDRAM_133M          1
#define SDRAM_100M          0

/*
** Select the crystal clock. Set only one of the three.
** When selecting 26M ,set both XTALCLK_13M and XTALCLK_26M macros.
*/
#define XTALCLK_13M     1   /* set to 1 for 13MHz crystal clock */  
#define XTALCLK_19M     0   /* set to 1 for 19.2MHz crystal clock */
#define XTALCLK_26M     1   /* set to 1 for 26MHz crystal clock */

/* GFX clock. */
#define GFX             0   /* set to 1 to enable GFX clocks */

#define TRITON_I2C	I2C1

#define TRITON2_ADDR    0x49

#define ONE_MICROSEC         1
#define TWENTY_MICROSEC      20
#define HUNDRED_MICROSEC     100
#define FIVEHUNDRED_MICROSEC 500
#define ONE_MILLISEC         1000
#define TEN_MILLISEC         10000
#define HUNDRED_MILLISEC     100000
#define ONE_SECOND           1000000

#define CONTROL_DEVCONF0	((volatile U32 *)0x48002274)

/* PRCM */
#define CM_FCLKEN_IVA2      0x48004000
#define CM_CLKEN_PLL_IVA2   0x48004004
#define CM_IDLEST_PLL_IVA2  0x48004024
#define CM_CLKSEL1_PLL_IVA2 0x48004040
#define CM_CLKSEL2_PLL_IVA2 0x48004044
#define CM_CLKEN_PLL_MPU    0x48004904
#define CM_IDLEST_PLL_MPU   0x48004924
#define CM_CLKSEL1_PLL_MPU  0x48004940
#define CM_CLKSEL2_PLL_MPU  0x48004944
#define CM_FCLKEN1_CORE     0x48004a00
#define CM_ICLKEN1_CORE     0x48004a10
#define CM_ICLKEN2_CORE     0x48004a14
#define CM_CLKSEL_CORE      0x48004a40
#define CM_FCLKEN_GFX       0x48004b00
#define CM_ICLKEN_GFX       0x48004b10
#define CM_CLKSEL_GFX       0x48004b40
#define CM_FCLKEN_WKUP      0x48004c00
#define CM_ICLKEN_WKUP      0x48004c10
#define CM_CLKSEL_WKUP      0x48004c40
#define CM_IDLEST_WKUP      0x48004c20
#define CM_CLKEN_PLL        0x48004d00
#define CM_IDLEST_CKGEN     0x48004d20
#define CM_CLKSEL1_PLL      0x48004d40
#define CM_CLKSEL2_PLL      0x48004d44
#define CM_CLKSEL3_PLL      0x48004d48
#define CM_FCLKEN_DSS       0x48004e00
#define CM_ICLKEN_DSS       0x48004e10
#define CM_CLKSEL_DSS       0x48004e40
#define CM_FCLKEN_CAM       0x48004f00
#define CM_ICLKEN_CAM       0x48004f10
#define CM_CLKSEL_CAM       0x48004F40
#define CM_FCLKEN_PER       0x48005000
#define CM_ICLKEN_PER       0x48005010
#define CM_AUTOIDLE_PER	    0x48005030
#define CM_CLKSEL_PER       0x48005040
#define CM_CLKSEL1_EMU      0x48005140

#define PRM_CLKSEL           0x48306d40
#define PRM_RSTCTRL          0x48307250
#define PRM_CLKSRC_CTRL      0x48307270

/* SMX-APE */
#define PM_RT_APE_BASE_ADDR_ARM         (SMX_APE_BASE + 0x10000)
#define PM_GPMC_BASE_ADDR_ARM           (SMX_APE_BASE + 0x12400)
#define PM_OCM_RAM_BASE_ADDR_ARM        (SMX_APE_BASE + 0x12800)
#define PM_OCM_ROM_BASE_ADDR_ARM        (SMX_APE_BASE + 0x12C00)

#define RT_REQ_INFO_PERMISSION_1        (PM_RT_APE_BASE_ADDR_ARM + 0x68)
#define RT_READ_PERMISSION_0    (PM_RT_APE_BASE_ADDR_ARM + 0x50)
#define RT_WRITE_PERMISSION_0   (PM_RT_APE_BASE_ADDR_ARM + 0x58)
#define RT_ADDR_MATCH_1 (PM_RT_APE_BASE_ADDR_ARM + 0x60)

#define GPMC_REQ_INFO_PERMISSION_0      (PM_GPMC_BASE_ADDR_ARM + 0x48)
#define GPMC_READ_PERMISSION_0  (PM_GPMC_BASE_ADDR_ARM + 0x50)
#define GPMC_WRITE_PERMISSION_0 (PM_GPMC_BASE_ADDR_ARM + 0x58)

#define OCM_REQ_INFO_PERMISSION_0       (PM_OCM_RAM_BASE_ADDR_ARM + 0x48)
#define OCM_READ_PERMISSION_0   (PM_OCM_RAM_BASE_ADDR_ARM + 0x50)
#define OCM_WRITE_PERMISSION_0  (PM_OCM_RAM_BASE_ADDR_ARM + 0x58)
#define OCM_ADDR_MATCH_2        (PM_OCM_RAM_BASE_ADDR_ARM + 0x80)

#define IVA2_REQ_INFO_PERMISSION_0      (PM_IVA2_BASE_ADDR_ARM + 0x48)
#define IVA2_READ_PERMISSION_0  (PM_IVA2_BASE_ADDR_ARM + 0x50)
#define IVA2_WRITE_PERMISSION_0 (PM_IVA2_BASE_ADDR_ARM + 0x58)

/* IVA2 */
#define PM_IVA2_BASE_ADDR_ARM           (SMX_APE_BASE + 0x14000)

#define in_regb(offSet)             (*(PREG_U8)(offSet))
#define in_regs(offSet)             (*(PREG_U16)(offSet))
#define in_regl(offSet)             (*(PREG_U32)(offSet))
#define out_regb(offSet, value)     (*(PREG_U8)(offSet) = (U8)(value))
#define out_regs(offSet, value)     (*(PREG_U16)(offSet) = (U16)(value))
#define out_regl(offSet, value)     (*(PREG_U32)(offSet) = (U32)(value))

#define SetBit(registre,masq)       (registre|=masq)
#define ClearBit(registre,masq)     (registre&=~masq)

#define reg_modify32(registre, clear, set) {\
                                           *(PREG_U32)(registre) |= (set);\
                                           *(PREG_U32)(registre) &= ~(clear);\
                                           }

/* Function Prototypes. */
void tolower_str (U8* str);
void get_string (U8 port, S8 *str, S16 size);
u32 get_number (U8 port, S8 *str, S16 size);
void get_line (U8 port, S8 *str, S16 size);

void dg_prcm_init (void);
void dss_clock_init (void);
void register_uarttests (void);
void register_ramtests (void);
void register_lcd_tests (void);
void register_i2c_tests (void);
void register_audio_tests (void);
void register_keypad_tests(void);
void register_ts_tests(void);
void register_tvout_tests(void);
void register_svideo_tests(void);

#ifdef __cplusplus
}
#endif

#endif  /* __MS_DIAG_OMAP3530EVM_H__ */
