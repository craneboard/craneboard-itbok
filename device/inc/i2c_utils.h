/*
 * i2c_utils.h: I2C utilities related constants and function prototypes.
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

#ifndef __MS_DIAG_I2C_UTILS_H__
#define __MS_DIAG_I2C_UTILS_H__

#ifdef __cplusplus
extern "C" {
#endif

#define I2C1            0
#define I2C2            1
#define I2C3            2
#define I2C4			3


#define TEN             10
#define HUNDRED         100
#define THOUSAND        1000
#define TENTHOUSAND     10000

/* Power area in Triton2 I2C. */
#define T2PWR_DEVICE_ADDR   0x4B

#define I2C_100K_CLK        1 /*    Stardard mode  - 100 kbits/s   */
#define I2C_400K_CLK        2 /*    Fast mode  - 400 kbits/s       */
#define I2C_1P95M_CLK       3 /*    High Speed - 1.95 Mbits/s      */
#define I2C_2P6M_CLK        4 /*    High Speed - 2.6 Mbits/s       */    
#define I2C_3P4M_CLK        5 /*    High Speed - 3.4 Mbits/s       */

#define I2C1_REG_BASE                           (0x48070000)
#define I2C2_REG_BASE                           (0x48072000)

#define I2C3_REG_BASE							(0x48060000)

#define I2C_REV_OFFSET                          0x00
#define I2C_IE_OFFSET                           0x04
#define I2C_STAT_OFFSET                         0x08
#define I2C_SYSS_OFFSET                         0x10
#define I2C_BUF_OFFSET                          0x14
#define I2C_CNT_OFFSET                          0x18
#define I2C_DATA_OFFSET                         0x1C
#define I2C_SYSC_OFFSET                         0x20
#define I2C_CON_OFFSET                          0x24
#define I2C_OA_OFFSET                           0x28
#define I2C_SA_OFFSET                           0x2C
#define I2C_PSC_OFFSET                          0x30
#define I2C_SCLL_OFFSET                         0x34
#define I2C_SCLH_OFFSET                         0x38
#define I2C_SYSTEST_OFFSET                      0x3C
#define I2C_BUFSTAT_OFFSET                      0x40

#define I2C_OA_VAL                              0x0E
#define I2C_PSC_VAL                             0x07
#define I2C_SCLL_VAL                            0x35
#define I2C_SCLH_VAL                            0x37
#define I2C_FS_PSC_VAL                          0x04
#define I2C_FS_SCLL_VAL                         0x12
#define I2C_FS_SCLH_VAL                         0x12
#define I2C_HSPSC_VAL                           0x04
#define I2C_HSSCLL_VAL                          0x0512
#define I2C_HSSCLH_VAL                          0x0C12
#define I2C_SLAVE_MASK                          0x007f
#define I2C_DATA_CNT_VAL3                       3
#define I2C_DATA_CNT_VAL2                       2
#define I2C_DATA_CNT_VAL1                       1

/* Fields of Status register. */
#define I2C_STAT_AL                             0x0001
#define I2C_STAT_NACK                           0x0002
#define I2C_STAT_ARDY                           0x0004
#define I2C_STAT_RRDY                           0x0008
#define I2C_STAT_XRDY                           0x0010
#define I2C_STAT_GC                             0x0020
#define I2C_STAT_AAS                            0x0200
#define I2C_STAT_XUDF                           0x0400
#define I2C_STAT_ROVR                           0x0800
#define I2C_STAT_BB                             0x1000
#define I2C_STAT_RDR                            0x2000
#define I2C_STAT_XDR                            0x4000
#define I2C_STAT_SBD                            0x8000
#define I2C_MASTER_CODE                         0x4000

/* Fields of Connection register. */
#define I2C_CON_STT                             0x0001
#define I2C_CON_STP                             0x0002
#define I2C_CON_TRX                             0x0200
#define I2C_CON_MST                             0x0400
#define I2C_CON_BE                              0x4000
#define I2C_CON_EN                              0x8000
#define I2C_CON_DISABLE                         0x0000
#define I2C_CON_HS_MODE                         0x1000
#define I2C_CON_SD_MODE                         0x0000
#define I2C_SYSS_RDONE                          0x0001

/* Fields of Bufstat register */
#define I2C_BUFSTAT_TXSTAT                      0x003F
#define I2C_BUFSTAT_RXSTAT                      0x3F00
#define I2C_BUFSTAT_FIFODEPTH                   0xC000

/* Fields of buffer setup register */
#define I2C_BUF_XTRSH                           0x003F
#define I2C_BUF_TXFIFO_CLR                      0x0040
#define I2C_BUF_XDMA_EN                         0x0080
#define I2C_BUF_RTRSH                           0x3F00
#define I2C_BUF_RXFIFO_CLR                      0x4000
#define I2C_BUF_RDMA_EN                         0x8000

#define I2C_CLK_MASK                            0x00180000

#define I2C_1_BYTE_ADDRESS      1
#define I2C_2_BYTE_ADDRESS		2


/* Flag for Slave Address - bit 29. */
#define I2C_SLAVE_ADDR              0x20000000
/* Flag for Clock Setting n- bit 28. */
#define I2C_CLOCK_SPEED             0x10000000
/* Flag for noaddress data - bit 27. */
#define I2C_NOADDRESS_DATA          0x08000000
/* Flag for data transfer using buffered data (FIFO) - bit 25. */
#define I2C_DATA_USING_FIFO         0x02000000
/* Flag for using I2C 2-byte subaddress - bit 24. */
#define I2C_USING_2BYTE_SUBADDRESS  0x01000000  
/*
** Flag for data transfer - single byte subaddress, 
** one byte data transfered per i2c operation - bit 26.
*/
#define I2C_DATA                    0x04000000

/* Macro to give offset of the read or write. */
/* Offset stays in the lower 16 bits -> flag is set at bit 26. */
#define I2C_OFFSET(X)               (I2C_DATA | (X))  
#define I2C_OFFSET_USING_FIFO(X)    (I2C_DATA_USING_FIFO |(X)) 
#define I2C_USE_2BYTE_SUBADDRESS(X) (I2C_USING_2BYTE_SUBADDRESS | (X))


/* Function Prototypes. */
void mux_setup_i2c1 (void);
void mux_setup_i2c2 (void);
S32 check_i2c_status (U16 status);
S32 clear_i2c (void);
U16 get_hssclh_val (U16 clock_speed);
U16 get_hsscll_val (U16 clock_speed);
void set_i2c_clocks (U16 clock_speed);
void configure_i2c (U32 i2c_no, U16 clock_speed, U8 slave_addr);
S32 read_i2c_payload_using_fifo (U16 device, U16 size, U8* data);
S32 write_i2c_nodata (U16 device, U8 data);
void write_i2c_onlydata (U16 device, U8 * buf, U32 *len);
S32 write_i2c_with_dataptr (U16 device, U32 length, U8 *dataptr);
S32 read_i2c_with_dataptr_2byte_subaddr (U16 device, U16 subaddr,
                                         U8 address_mode, U16 size, U8* data);
S32 write_i2c_data (U16 device, U8 subaddr, U8 data);
S32 read_i2c_data (U16 device, U8 subaddr, U8 * data);
status_t deinit_i2c (void);

#ifdef __cplusplus
}
#endif

#endif  /* __MS_DIAG_I2C_UTILS_H__ */
