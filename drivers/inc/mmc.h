/*
 * mmc.h: MMC driver driver definitions.
 *
 * (C) Copyright 2010
 * Mistral Solutions Private Limited, <www.mistralsolutions.com>
 *
 * Author :
 *	Ganeshan <nganeshan@mistralsolutions.com>
 *
 * Derived from CSST.
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
 *   12 mar 2010 - Created from OMAP2 ITBOK source.
 */

#ifndef _MMC_H
#define _MMC_H

#define MMC_C_PRIVATE 1
#ifdef MMC_C_PRIVATE


U8 mmc_slot_no;                 /*Slot number used in Register Macros */


/*Macros to get and set*/
#define MMCHS_GET_R(reg)            (*reg)
#define MMCHS_SET_R(reg,val)        (*reg = val)
#define MMCHS_FSET_R(reg,val)       ((*reg)|=val)
#define MMCHS_ASET_R(reg,val)       ((*reg)&=val)

#define MMCHS_STUFF_BITS            (0x0000)
#define MMC_STUFF_BITS              (0xFFFF)

#define MMC_OCR_ACCESS_MODE_BYTE             (0x0  << 29)
#define MMC_OCR_ACCESS_MODE_SECTOR           (0x2  << 29)

#define MMC_PWR_STABLE              (0x202)
#define PBIAS_LITE_VMMC1_3V         (0x101)
#define PBIAS_LITE_VMMC1_52MHZ      (0x404)
#define PBIAS_LITE_MMC1_ERROR       (0x808)

/*CARD status Bits 12:9*/
#define MMC_CARD_STATE_IDLE         (0x0 << 9)
#define MMC_CARD_STATE_READY        (0x1 << 9)
#define MMC_CARD_STATE_IDENT        (0x2 << 9)
#define MMC_CARD_STATE_STBY         (0x3 << 9)
#define MMC_CARD_STATE_TRAN         (0x4 << 9)
#define MMC_CARD_STATE_DATA         (0x5 << 9)
#define MMC_CARD_STATE_RCV          (0x6 << 9)
#define MMC_CARD_STATE_PRG          (0x7 << 9)
#define MMC_CARD_STATE_DIS          (0x8 << 9)
#define MMC_CARD_STATE_BTST         (0x9 << 9)
#define MMC_CARD_STATE_MASK         (0xF << 9)

#define MMC_HIGH_VOLTAGE_CARD       (0x00FF8000)
#define MMCHS_BASE			(0x4809C000) 

/*OMAP HSMMC Controller Registers*/
#define MMCHS_SYSC                  (volatile U32 *)(MMCHS_BASE + 0x010)
#define MMCHS_SYSS                  (volatile U32 *)(MMCHS_BASE + 0x014)
#define MMCHS_CSRE                  (volatile U32 *)(MMCHS_BASE + 0x024)
#define MMCHS_SYST                  (volatile U32 *)(MMCHS_BASE + 0x028)
#define MMCHS_CON                   (volatile U32 *)(MMCHS_BASE + 0x02C)
#define MMCHS_BLK                   (volatile U32 *)(MMCHS_BASE + 0x104)
#define MMCHS_ARG                   (volatile U32 *)(MMCHS_BASE + 0x108)
#define MMCHS_CMD                   (volatile U32 *)(MMCHS_BASE + 0x10C)
#define MMCHS_RSP10                 (volatile U32 *)(MMCHS_BASE + 0x110)
#define MMCHS_RSP32                 (volatile U32 *)(MMCHS_BASE + 0x114)
#define MMCHS_RSP54                 (volatile U32 *)(MMCHS_BASE + 0x118)
#define MMCHS_RSP76                 (volatile U32 *)(MMCHS_BASE + 0x11C)
#define MMCHS_DATA                  (volatile U32 *)(MMCHS_BASE + 0x120)
#define MMCHS_PSTAT                 (volatile U32 *)(MMCHS_BASE + 0x124)
#define MMCHS_HCTL                  (volatile U32 *)(MMCHS_BASE + 0x128)
#define MMCHS_SYSCTL                (volatile U32 *)(MMCHS_BASE + 0x12C)
#define MMCHS_STAT                  (volatile U32 *)(MMCHS_BASE + 0x130)
#define MMCHS_IE                    (volatile U32 *)(MMCHS_BASE + 0x134)
#define MMCHS_ISE                   (volatile U32 *)(MMCHS_BASE + 0x138)
#define MMCHS_AC12                  (volatile U32 *)(MMCHS_BASE + 0x13C)
#define MMCHS_CAPA                  (volatile U32 *)(MMCHS_BASE + 0x140)
#define MMCHS_CURCAPA               (volatile U32 *)(MMCHS_BASE + 0x148)
#define MMCHS_REV                   (volatile U32 *)(MMCHS_BASE + 0x1FC)

/* MMCHS_SYSC (sysconfig) register fields*/
#define SYSC_AUTOIDLE               ((0x1)<<0)  /*auto clock gating */
#define SYSC_SOFTRESET              ((0x1)<<1)  /*module reset */
#define SYSC_ENWKUP                 ((0x1)<<2)  /*wakeup feature */
#define SYSC_CLKACT_CLKOFF          ((0x0)<<8)  /*OCP FCLK switch off */
#define SYSC_CLKACT_OCP             ((0x1)<<8)
#define SYSC_CLKACT_FCLK            ((0x2)<<8)
#define SYSC_CLKACT_OCP_FCLK        ((0x3)<<8)

/* MMCHS_SYSS (sysstaus) register */
#define MMCHS_SYSS_RESETDONE        ((0x1)<<0)

/* MMCHS_CSRE register fields*/

/* MMCHS_CON bit fields*/
#define MMCHS_ODRN                  ((0x1)<<0) /*open drain*/   /*for broad cast cmds 1,2,3 and 40 must set */
#define MMCHS_INIT                  ((0x1)<<1)  /*init sequence */
#define MMCHS_CON_HR                ((0x1)<<2)  /* HOST generates 48bit resp */
#define MMCHS_CON_STR               ((0x1)<<3)  /* enable stream orented data transfer */
#define MMCHS_CON_DW8               ((0x1)<<5)  /* enable 8 bit data width */
#define MMCHS_CON_MIT               ((0x1)<<6)  /* command timeout disable */
#define MMCHS_CON_CDP               ((0x1)<<7)  /*card detect polarity */
#define MMCHS_CTO                   (0x0)<<6    /*command timeout enable */
#define MMCHS_CON_CTPL              ((0x1)<<11)
#define MMCHS_CON_PADEN             ((0x1)<<15)

/* MMCHS_SYST (SYETEM TEST) register*/
#define MMCHS_SYST_MCKD             ((0x1)<<0)  /*OUTPUT CLOCK */
#define MMCHS_SYST_CDIR_H2C         ((0x0)<<1)  /* cmd direction host to card) */
#define MMCHS_SYST_CDIR_C2H         ((0x1)<<1)  /* cmd direction card to host) */
#define MMCHS_SYST_DDIR_H2C         ((0X0)<<3)  /*DATA direction host to card */
#define MMCHS_SYST_DDIR_C2H         ((0X1)<<3)  /*data direction card to host */
#define MMCHS_SYST_SSB              ((0x1)<<12) /*set status bit clear statusreg */
#define MMCHS_SYST_WAKD_LOW         ((0x0)<<13)
#define MMCHS_SYST_WAKD_HIGH        ((0x1)<<13)
#define MMCHS_SYST_SDWP             ((0x1)<<14) /*card write protect */
#define MMCHS_SYST_SDCD             ((0X1)<<15) /* card detect */
#define MMCHS_HCTL_DTW              (0x1<<1)
#define MMCHS_SYSCTL_CLK_DIS        (0xFFFF003F)

/* MMCHS_BLK reg values*/
#define MMCHS_NBLK_ONE              (0x00010000)
#define MMCHS_BLKSZ_512             (0x00000200)

/* MMCHS_PSTAT */
#define MMCHS_PSTAT_CLEV            (0x1<<24)
#define MMCHS_PSTAT_DLEV            (0xF<<20)
#define MMCHS_PSTAT_DBOUNCE         (0x3<<16)
#define MMCHS_PSTAT_BRE             (0x1<<11)
#define MMCHS_PSTAT_BWE             (0x1<<10)
#define MMCHS_PSTAT_RTA             (0x1<<9)
#define MMCHS_PSTAT_WTA             (0x1<<8)
#define MMCHS_PSTAT_DLA             (0x1<<2)
#define MMCHS_PSTAT_DATI            (0x1<<1)
#define MMCHS_PSTAT_CMDI            (0x1<<0)

/*SYSCTL REG*/
#define MMCHS_DTO_MAX               ((0xE)<<16) /*data timeout */
#define MMCHS_SYSCTL_ICE            (0x1<<0)    /*internal clock enable */
#define MMCHS_SYSCTL_ICS            (0x1<<1)
#define MMCHS_SYSCTL_CEN            (0x1<<2)    /*clock enable */
#define CLK_DIV                     (0x4)   /*this value should be changed based on the fclk value */
#define MMCHS_SYSCTL_CLKD_1         (0x1<<6)
#define MMCHS_SYSCTL_CLKD_2         (0x2<<6)
#define MMCHS_SYSCTL_CLKD_4         (0x4<<6)
#define MMCHS_SYSCTL_CLKD_5         (0x5<<6)
#define MMCHS_SYSCTL_CLKD_8         (0x8<<6)
#define MMCHS_SYSCTL_CLKD_48        (0x30<<6)
#define MMCHS_SYSCTL_RESET          (0x2)   /*reset */
#define CLK_DIV0                    ((0x0)<<6)
#define MMCHS_SYSCTL_SRA            (0x1<<24)   /*Soft reset all */
#define MMCHS_SYSCTL_SRC            (0x1<<25)   /*Soft reset command line */
#define MMCHS_SYSCTL_SRD            (0x1<<26)   /*Soft reset data lines */

/*HCTL reg*/
#define MMCHS_SDVS_1_8V             (0x5<<9)    /*select 1.8v */
#define MMCHS_SDVS_3_0V             (0x6<<9)    /*select 3.0v */
#define MMCHS_SDVS_3_3V             (0x7<<9)    /*select 3.3v */
#define MMCHS_SDBP                  (0x0100)    /*enable sd bus pwr */
#define MMCHS_HCTL_IBG              (0x1<<19)
#define MMCHS_HCTL_RWC              (0x1<<18)
#define MMCHS_HCTL_CR               (0x1<<17)
/*CAPA Reg*/
#define MMCHS_1_8V                  (0x1<<26)
#define MMCHS_3_0V                  (0x1<<25)
#define MMCHS_3_3V                  (0x1<<24)

/*  MMCHS STAT field values  */
/*Error interrupt status*/
#define MMCHS_MMCSTAT_BADA          (0x20000000)    /*Bad access to data space */
#define MMCHS_MMCSTAT_CERR          (0x10000000)    /*card error */
#define MMCHS_MMCSTAT_ACE           (0x01000000)    /*Auto cmd12 er */
#define MMCHS_MMCSTAT_DEB           (0x00400000)    /*Data end bit er */
#define MMCHS_MMCSTAT_DCRC          (0x00200000)    /*Data crc er */
#define MMCHS_MMCSTAT_DTO           (0x00100000)    /*Data timeout */
#define MMCHS_MMCSTAT_CIE           (0x00080000)    /*command index er */
#define MMCHS_MMCSTAT_CEB           (0x00040000)    /*command endbit er */
#define MMCHS_MMCSTAT_CCRC          (0x00020000)    /*command crc error */
#define MMCHS_MMCSTAT_CTO           (0x00010000)    /*command timeout */
/*normal interrupt status*/
#define MMCHS_MMCSTAT_ERRI          (0x8000)    /*error interrupt */
#define MMCHS_MMCSTAT_CIRQ          (0x0100)    /*card interrupt, only for SD */
#define MMCHS_MMCSTAT_CREM          (0x0080)    /*card removed */
#define MMCHS_MMCSTAT_CINS          (0x0040)    /*card inserted */
#define MMCHS_MMCSTAT_BRR           (0x0020)    /*buffer read ready */
#define MMCHS_MMCSTAT_BWR           (0x0010)    /*buffer write ready */
#define MMCHS_MMCSTAT_DMA           (0x0008)    /*DMA interrupt, not supported */
#define MMCHS_MMCSTAT_BGE           (0x0004)    /*block gap event */
#define MMCHS_MMCSTAT_TC            (0x0002)    /*transfer complete */
#define MMCHS_MMCSTAT_CC            (0x0001)    /*command complete */

/*  MMCHS CMD field values  */
/*Response lengths*/
#define MMCHS_RSP_LEN48             (0x00020000)
#define MMCHS_RSP_LEN48B            (0x00030000)
#define MMCHS_RSP_LEN136            (0x00010000)
#define MMCHS_RSP_NONE              (0x00000000)

/*command index*/
#define MMCHS_CMD0                  (0x00000000)
#define MMCHS_CMD1                  (0x01000000)
#define MMCHS_CMD2                  (0x02000000)
#define MMCHS_CMD3                  (0x03000000)
#define MMCHS_CMD4                  (0x04000000)
#define MMCHS_CMD5                  (0x05000000)
#define MMCHS_CMD6                  (0x06000000)
#define MMCHS_CMD7                  (0x07000000)
#define MMCHS_CMD8                  (0x08000000)
#define MMCHS_CMD9                  (0x09000000)
#define MMCHS_CMD10                 (0x0A000000)
#define MMCHS_CMD11                 (0x0B000000)
#define MMCHS_CMD12                 (0x0C000000)
#define MMCHS_CMD13                 (0x0D000000)
#define MMCHS_CMD14                 (0x0E000000)
#define MMCHS_CMD15                 (0x0F000000)
#define MMCHS_CMD16                 (0x10000000)
#define MMCHS_CMD17                 (0x11000000)
#define MMCHS_CMD18                 (0x12000000)
#define MMCHS_CMD19                 (0x13000000)
#define MMCHS_CMD20                 (0x14000000)
#define MMCHS_CMD21                 (0x15000000)
#define MMCHS_CMD22                 (0x16000000)
#define MMCHS_CMD23                 (0x17000000)
#define MMCHS_CMD24                 (0x18000000)
#define MMCHS_CMD25                 (0x19000000)
#define MMCHS_CMD26                 (0x1A000000)
#define MMCHS_CMD27                 (0x1B000000)
#define MMCHS_CMD28                 (0x1C000000)
#define MMCHS_CMD29                 (0x1D000000)
#define MMCHS_CMD30                 (0x1E000000)
#define MMCHS_CMD31                 (0x1F000000)
#define MMCHS_CMD32                 (0x20000000)
#define MMCHS_CMD33                 (0x21000000)
#define MMCHS_CMD34                 (0x22000000)
#define MMCHS_CMD35                 (0x23000000)
#define MMCHS_CMD36                 (0x24000000)
#define MMCHS_CMD37                 (0x25000000)
#define MMCHS_CMD38                 (0x26000000)
#define MMCHS_CMD39                 (0x27000000)
#define MMCHS_CMD40                 (0x28000000)
#define MMCHS_CMD41                 (0x29000000)
#define MMCHS_CMD42                 (0x2A000000)
#define MMCHS_CMD43                 (0x2B000000)
#define MMCHS_CMD44                 (0x2C000000)
#define MMCHS_CMD45                 (0x2D000000)
#define MMCHS_CMD46                 (0x2E000000)
#define MMCHS_CMD47                 (0x2F000000)
#define MMCHS_CMD48                 (0x30000000)
#define MMCHS_CMD49                 (0x31000000)
#define MMCHS_CMD50                 (0x32000000)
#define MMCHS_CMD51                 (0x33000000)
#define MMCHS_CMD52                 (0x34000000)
#define MMCHS_CMD53                 (0x35000000)
#define MMCHS_CMD54                 (0x36000000)
#define MMCHS_CMD55                 (0x37000000)
#define MMCHS_CMD56                 (0x38000000)
#define MMCHS_CMD57                 (0x39000000)
#define MMCHS_CMD58                 (0x3A000000)
#define MMCHS_CMD59                 (0x3B000000)
#define MMCHS_CMD60                 (0x3C000000)
#define MMCHS_CMD61                 (0x3D000000)
#define MMCHS_CMD62                 (0x3E000000)
#define MMCHS_CMD63                 (0x3F000000)

/*command type*/
#define MMCHS_CMD_NORMAL            (0x00000000)
#define MMCHS_CMD_SUSP              (0x1<<22)
#define MMCHS_FUNC_SEL              (0x2<<22)
#define MMCHS_CMD_ABORT             (0x2<<22)
/*other fileds*/
#define MMCHS_CMD_DMA               (0x00000001)    /*DMA mode */
#define MMCHS_CMD_BCE               (0x00000002)    /*block count enable */
#define MMCHS_CMD_ACEN              (0x00000004)    /*auto cmd12 enable */
#define MMCHS_CMD_DDIR              (0x00000010)    /*data direction card to host */
#define MMCHS_CMD_READ              (0x00000010)    /*read, card to host */
#define MMCHS_CMD_WRITE             (0x00000000)    /*write, host to card */
#define MMCHS_CMD_MSBS              (0x00000020)    /*multiblockselect */
#define MMCHS_CMD_CCCE              (0x00080000)    /*cmd crc enable */
#define MMCHS_CMD_CICE              (0x00100000)    /*cmd index check enable */
#define MMCHS_CMD_DP                (0x00200000)    /*data present */

/*================== COMMANDS =================================================*/

#define MMCHS_GO_IDLE_STATE           (MMCHS_CMD0  | MMCHS_RSP_NONE)
#define MMCHS_SEND_OP_COND            (MMCHS_CMD1  | MMCHS_CMD_CICE | MMCHS_RSP_LEN48 | MMCHS_CMD_NORMAL)
#define MMCHS_ALL_SEND_CID            (MMCHS_CMD2  | MMCHS_RSP_LEN136 |MMCHS_CMD_CCCE | MMCHS_CMD_NORMAL)
#define MMCHS_SET_RELATIVE_ADDR       (MMCHS_CMD3  | MMCHS_RSP_LEN48 | MMCHS_CMD_NORMAL)
#define MMCHS_SET_DSR                 (MMCHS_CMD4  | MMCHS_RSP_NONE | MMCHS_CMD_NORMAL)
#define MMCHS_SELECT_CARD             (MMCHS_CMD7  | MMCHS_RSP_LEN48B | MMCHS_CMD_NORMAL)
#define MMCHS_DESELECT_CARD           (MMCHS_CMD7  | MMCHS_RSP_NONE | MMCHS_CMD_NORMAL)
#define MMCHS_SEND_CSD                (MMCHS_CMD9  | MMCHS_RSP_LEN136 | MMCHS_CMD_NORMAL)
#define MMCHS_SEND_CID                (MMCHS_CMD10 | MMCHS_RSP_LEN136 | MMCHS_CMD_NORMAL)
#define MMCHS_SEND_CID_SPI            (MMCHS_CMD9  | MMCHS_RSP_LEN136 | MMCHS_CMD_NORMAL)
/*ALONG WITH CMD11, ENABLE STREAM MODE IN MMCHS_CON REG*/
#define MMCHS_READ_DAT_UNTIL_STOP     (MMCHS_CMD11 | MMCHS_RSP_LEN48 | MMCHS_CMD_READ | MMCHS_CMD_DP | MMCHS_CMD_NORMAL)
#define MMCHS_STOP_TRANSMISSION       (MMCHS_CMD12 | MMCHS_RSP_LEN48B | MMCHS_CMD_ABORT)
#define MMCHS_SEND_STATUS             (MMCHS_CMD13 | MMCHS_RSP_LEN48 | MMCHS_CMD_NORMAL)
#define MMCHS_GO_INACTIVE_STATE       (MMCHS_CMD15 | MMCHS_RSP_NONE | MMCHS_CMD_NORMAL)
#define MMCHS_SET_BLOCKLEN            (MMCHS_CMD16 | MMCHS_CMD_CCCE | MMCHS_CMD_CICE | MMCHS_RSP_LEN48 | MMCHS_CMD_NORMAL)
#define MMCHS_READ_SINGLE_BLOCK       (MMCHS_CMD17 | MMCHS_RSP_LEN48 | MMCHS_CMD_READ | MMCHS_CMD_DP | MMCHS_CMD_NORMAL)
#define MMCHS_READ_MULTIPLE_BLOCK     (MMCHS_CMD18 | MMCHS_RSP_LEN48 | MMCHS_CMD_MSBS | MMCHS_CMD_BCE | MMCHS_CMD_READ | |MMCHS_CMD_DP|MMCHS_CMD_NORMAL)
/*ALONG WITH CMD 20, ENABLE STREAM MODE IN MMCHS_CON REG*/
#define MMCHS_WRITE_DAT_UNTIL_STOP    (MMCHS_CMD20 | MMCHS_RSP_LEN48B | MMCHS_CMD_DP | MMCHS_CMD_NORMAL)
#define MMCHS_WRITE_SINGLE_BLOCK      (MMCHS_CMD24 | MMCHS_RSP_LEN48 | MMCHS_CMD_DP | MMCHS_CMD_NORMAL|MMCHS_CMD_WRITE)
#define MMCHS_WRITE_MULTIPLE_BLOCK    (MMCHS_CMD25 | MMCHS_RSP_LEN48 | MMCHS_CMD_DP | MMCHS_CMD_MSBS | MMCHS_CMD_BCE | MMCHS_CMD_NORMAL)
#define MMCHS_PROGRAM_CID             (MMCHS_CMD26 | MMCHS_RSP_LEN48 | MMCHS_CMD_NORMAL)
#define MMCHS_PROGRAM_CSD             (MMCHS_CMD27 | MMCHS_RSP_LEN48 | MMCHS_CMD_NORMAL)
#define MMCHS_SET_WRITE_PROT          (MMCHS_CMD28 | MMCHS_RSP_LEN48B | MMCHS_CMD_NORMAL)
#define MMCHS_CLR_WRITE_PROT          (MMCHS_CMD29 | MMCHS_RSP_LEN48B | MMCHS_CMD_NORMAL)
#define MMCHS_SEND_WRITE_PROT         (MMCHS_CMD30 | MMCHS_RSP_LEN48 | MMCHS_CMD_READ | MMCHS_CMD_NORMAL)
#define MMCHS_ERASE_GROUP_START       (MMCHS_CMD35 |MMCHS_RSP_LEN48 | MMCHS_CMD_NORMAL)
#define MMCHS_ERASE_GROUP_END         (MMCHS_CMD36 |MMCHS_RSP_LEN48 | MMCHS_CMD_NORMAL)
#define MMCHS_ERASE                   (MMCHS_CMD38 | MMCHS_RSP_LEN48B | MMCHS_CMD_NORMAL)
#define MMCHS_FAST_IO                 (MMCHS_CMD39 | MMCHS_RSP_LEN48 | MMCHS_CMD_NORMAL)
#define MMCHS_GO_IRQ_STATE            (MMCHS_CMD40 | MMCHS_RSP_LEN48 | MMCHS_CMD_NORMAL)
#define MMCHS_CRC_ON_OFF_SPI          (MMCHS_CMD59 | MMCHS_RSP_LEN48 | MMCHS_CMD_NORMAL)
//SD ACMD Commands
#define SDHS_SET_BUS_WIDTH            (MMCHS_CMD6  | MMCHS_RSP_LEN48 | MMCHS_CMD_NORMAL)
#define SDHS_STATUS                   (MMCHS_CMD13 | MMCHS_RSP_LEN48 | MMCHS_CMD_NORMAL)
#define SDHS_SEND_NUM_WR_BLOCKS       (MMCHS_CMD22 | MMCHS_RSP_LEN48 | MMCHS_CMD_READ | MMCHS_CMD_NORMAL)
#define SDHS_SET_WR_BLK_ERASE_COUNT   (MMCHS_CMD23 | MMCHS_RSP_LEN48 | MMCHS_CMD_WRITE | MMCHS_CMD_NORMAL)
#define SDHS_APP_OP_COND              (MMCHS_CMD41 | MMCHS_RSP_LEN48 | MMCHS_CMD_NORMAL)
#define SDHS_SET_CLR_CARD_DETECT      (MMCHS_CMD42 | MMCHS_RSP_LEN48 | MMCHS_CMD_NORMAL)
#define SDHS_SEND_SCR                 (MMCHS_CMD51 | MMCHS_RSP_LEN48 | MMCHS_CMD_DP| MMCHS_CMD_NORMAL)

//SD commands
#define SDHS_ALL_SEND_CID              (MMCHS_CMD2 | MMCHS_RSP_LEN136 | MMCHS_CMD_NORMAL)
#define SDHS_SEND_RELATIVE_ADDR       (MMCHS_CMD3 | MMCHS_RSP_LEN48 | MMCHS_CMD_NORMAL)
#define SDHS_APP_CMD                  (MMCHS_CMD55| MMCHS_RSP_LEN48 | MMCHS_CMD_NORMAL)
#define SDHS_LOCK_UNLOCK              (MMCHS_CMD42| MMCHS_RSP_LEN48 | MMCHS_CMD_READ | MMCHS_CMD_NORMAL)
#define SDHS_GEN_CMD_WR               (MMCHS_CMD56| MMCHS_RSP_LEN48 | MMCHS_CMD_WRITE | MMCHS_CMD_NORMAL)
#define SDHS_GEN_CMD_RD               (MMCHS_CMD56| MMCHS_RSP_LEN48B| MMCHS_CMD_READ | MMCHS_CMD_NORMAL)
#define SDHS_ERASE_WR_BLK_START       (MMCHS_CMD32| MMCHS_RSP_LEN48 | MMCHS_CMD_NORMAL)
#define SDHS_ERASE_WR_BLK_END         (MMCHS_CMD33| MMCHS_RSP_LEN48 | MMCHS_CMD_NORMAL)
#define MMCHS_SEND_EXT_CSD            (MMCHS_CMD8 | MMCHS_RSP_LEN48 | MMCHS_CMD_READ | MMCHS_CMD_DP | MMCHS_CMD_NORMAL)
#define MMCHS_SEND_SWITCH             (MMCHS_CMD6 | MMCHS_RSP_LEN48 | MMCHS_CMD_NORMAL)
#define MMCHS_BUSTEST_R               (MMCHS_CMD14 | MMCHS_RSP_LEN48 | MMCHS_CMD_READ | MMCHS_CMD_DP | MMCHS_CMD_NORMAL)
#define MMCHS_BUSTEST_W               (MMCHS_CMD19 | MMCHS_RSP_LEN48 | MMCHS_CMD_WRITE| MMCHS_CMD_DP | MMCHS_CMD_NORMAL)

/*================== TYPES =================================================*/
typedef struct
{
    U8 mfg_id;                  /* 8 bit Manufacturer’s ID */
    U8 product_name[6];         /* 7 character Product Name */
    U16 hw_rev;                 /* 4 bit Hardware  Revision Number */
    U16 fw_rev;                 /* 4 bit Firmware Revision Number */
    U8 serial_number[4];        /* 24 bit Serial Number */
    U16 month_code;             /* 4 bit Manufacturing Date (Month) */
    U16 year_code;              /* 4 bit Manufacturing Date (Year) */
    U16 day_code;               /* 4 bit Manufacturing Date (Day) */
    U16 crc;                    /*7 bit crc */
    U32 app_id;
    U32 sd_unique_id;
    U32 mmc_v3unique_id;
} T_MMC_CID;

typedef struct
{
    U16 erase_blk_len;
    U16 erase_sector_size;
    U16 csd_structure;          // 2 bit structure type field
    U16 mmc_prot;               // 2 bit MMC protocol
    U16 taac;                   // 8 bit TAAC
    U16 nsac;                   // 8 bit NSAC
    U16 tran_speed;             // 8 bit max data transmission speed
    U16 ccc;                    // 12 bit card command classes
    U16 read_blk_len;           // 4 bit maximum Read Block Length
    U16 read_blk_partial;       // 1 bit indicates if partial read blocks allowed
    U16 write_blk_misalign;     // 1 bit flag indicates write block misalignment
    U16 read_blk_misalign;      // 1 bit flag indicates read block misalignment
    U16 dsr_imp;                // 1 bit flag indicates whether card has DSR reg
    U16 csize;                  // 12 bit device size
    U16 vdd_rcurr_min;          // 3 bit Max. Read Current @ Vdd Min
    U16 vdd_rcurr_max;          // 3 bit Max. Read Current @ Vdd Max
    U16 vdd_wcurr_min;          // 3 bit Max. Write Current @ Vdd Min
    U16 vdd_wcurr_max;          // 3 bit Max. Write Current @ Vdd Max
    U16 csize_mult;             // 3 bit device size multiplier
    U16 erase_grp_mult;         // 5 bit erase sector size
    U16 erase_grp_size;         // 5 bit erase group size
    U16 wp_grp_size;            // 5 bit write protect group size
    U16 wp_grp_enable;          // 1 bit write protect enable flag
    U16 default_ecc;            // 2 bit Manufacturer Default ECC
    U16 r2w_factor;             // 3 bit stream write factor
    U16 write_bl_len;           // 4 bit maximum write block length
    U16 write_bl_partial;       // 1 bit indicates if partial write blocks allowed
    U16 copy;                   // 1 bit copy flag
    U16 perm_write_protect;     // 1 bit to dis/en-able permanent write protection
    U16 tmp_write_protect;      // 1 bit to dis/en-able temporary write protection
    U16 crc;                    // 7 bit r/w/e redundancy check
} T_MMC_CSD;

#endif
/*================== END MMC PRIVATE DATA=======================================*/



#define MMC_CARD               1    // MMC card
#define SD_CARD                2    // SD card
#define HS_MMC_CARD            3    // MMC card

#define    MMC_SLOT_1          1
#define    MMC_SLOT_2          2

#define    MMC_VOLT_1_8        1
#define    MMC_VOLT_3_0        2

#define    MMC_DATAWIDTH_8_BITS     8
#define    MMC_DATAWIDTH_4_BITS     4
#define    MMC_DATAWIDTH_1_BITS     1

#define    MMC_TRANFER_ENABLE       1
#define    MMC_TRANFER_DISABLE      0

#define    MMC_OPENDRAIN_ENABLE     1
#define    MMC_OPENDRAIN_DISABLE    0

#define    MMC_BLOCK_SIZE           512

#define MMC_SLOT_NUMBER(sid)    (sid + 1)
#define GET_T2_MMC_NO(slotno)   (slotno-1)

#define    MMC_RSP1    1
#define    MMC_RSP2    2
#define    MMC_RSP3    3
#define    MMC_RSP4    4
#define    MMC_RSP5    5
#define    MMC_RSP6    6
#define    MMC_RSP7    7

// Macros added 
#define ONE_MILLISEC         1000
#define ERROR				0


/* MMC error codes*/
#define NO_MMC_CARD             0xF2055000
#define MMC_WRITE_ER                0xF2055001
#define MMC_PARAM_ER                        0xF2055002
#define MMC_WR_PR                           0xF2055003
#define MMC_INIT_ER                         0xF2055004
#define MMC_CARD_ER                         0xF2055005
#define MMC_CMD_TO_ER                       0xF2055006
#define MMC_CMD_FAIL                        0xF2055007
#define MMC_BUSY_ER                         0xF2055008
#define MMC_DATA_TO_ER                  0xF2055009
#define MMC_CRC_ER                          0xF205500A


#endif /*CSST_MMC_H */
