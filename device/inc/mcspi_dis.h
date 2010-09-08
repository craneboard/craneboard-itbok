/*
 * mcspi_dis.h: McSPI interface related definitions and function prototypes.
 *
 * (C) Copyright 2010
 * Mistral Solutions Private Limited, <www.mistralsolutions.com>
 *
 * Author :
 * Naveen <anaveen@mistralsolutions.com>
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

#ifndef __MS_DIAG_MCSPI_DIS_H__
#define __MS_DIAG_MCSPI_DIS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "types.h"

#define MCSPI_TAG_BASE				0

#define MCSPI_PID_TAG				(MCSPI_TAG_BASE+ 0)			/* Primary ID Key */
#define MCSPI_SID_TAG				(MCSPI_PID_TAG + 4)			/* Second ID Key*/
#define MCSPI_RX_CALLBACK_TAG 		(MCSPI_SID_TAG + 4)			/* Callback RX */
#define MCSPI_TX_CALLBACK_TAG		(MCSPI_RX_CALLBACK_TAG+4)
#define MCSPI_ERROR_CALLBACK_TAG	(MCSPI_TX_CALLBACK_TAG+4)
#define MCSPI_IRQPOLL_TAG			(MCSPI_ERROR_CALLBACK_TAG+4)
#define MCSPI_MASTER_TAG			(MCSPI_IRQPOLL_TAG+1)
#define MCSPI_BAUDRATE_TAG			(MCSPI_MASTER_TAG+1)
#define MCSPI_CSPOLARITY_TAG		(MCSPI_BAUDRATE_TAG+4)
#define MCSPI_CLKPHASE_TAG			(MCSPI_CSPOLARITY_TAG+1)
#define MCSPI_CLKPOLARITY_TAG		(MCSPI_CLKPHASE_TAG+1)
#define MCSPI_WORDLENGTH_TAG		(MCSPI_CLKPOLARITY_TAG+1)
#define MCSPI_MODE_TAG				(MCSPI_WORDLENGTH_TAG+1)
#define MCSPI_CHANNELNUM_TAG		(MCSPI_MODE_TAG+1)
#define MCSPI_RW_TAG				(MCSPI_CHANNELNUM_TAG+1)
#define MCSPI_CHANNEL_EN_TAG		(MCSPI_RW_TAG+1)


/* Defines for DIS members values */

/* Values for DIS member irq_poll */
#define MCSPI_POLL		1
#define MCSPI_INTERRUPT		0

/* Values for DIS member mcspi_master */
#define MCSPI_MASTER		1
#define MCSPI_SLAVE		0

/* Values for DIS member cs_polarity */
#define MCSPI_CS_ACTIVELOW		1
#define MCSPI_CS_ACTIVEHIGH		0

/* Values for DIS member clk_phase */
#define MCSPI_DATA_LATCH_ODDEDGES		1
#define MCSPI_DATA_LATCH_EVENEDGES		0

/* Values for DIS member clk_polarity */
#define MCSPI_CLK_ACTIVELOW		1
#define MCSPI_CLK_ACTIVEHIGH		0

/* Values for DIS member spi_mode */
#define MCSPI_TXRX_MODE		0
#define MCSPI_TX_MODE		1
#define MCSPI_RX_MODE		2

/* Values for DIS member channel_enable */
#define MCSPI_CHANNEL_ENABLE	1
#define MCSPI_CHANNEL_DISABLE	0


/* The two sid values of MCSPI*/
#define MCSPI_1					1
#define MCSPI_2					2

typedef struct
{
    U32 pid;				/* Primary ID */
    U32 sid;                            /* Second ID */

    void(*mcspi_irq_callback);		/* mcspi RX/Tx/Error Callback */

    U8  irq_poll;			/* Poll or Interrupt Request */
    U8	master;                         /* 1=Mcspi master,0=Mcspi Slave */
    U32 clk_baudrate;                   /* 11.7kbps to 48Mbps */

    U8	wordlength;			/* 4 to 32 bits */
    U8	channel_number;                 /* 0-3 */

    U8	cs_polarity;			/* chip select polarity */
    U8	clk_phase;                      /**/
    U8	clk_polarity;                   /**/
    U8	spi_mode;                       /* 0=Tx/Rx ; 1=Tx ;2=Rx */


} T_MCSPI_INIT_STRUCTURE;


#ifdef __cplusplus
}
#endif

#endif /*__MS_DIAG_MCSPI_DIS_H__*/
