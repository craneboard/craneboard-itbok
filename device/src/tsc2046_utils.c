/*
 * tsc2046_utils.c: TSC2046 device related routines
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

/* includes */
#include <common.h>
#include <malloc.h>
#include "types.h"
#include "omap3530evm.h"
#include "mcspi_dis.h"
#include "mcspi_utils.h"
#include "mcspi_drv.h"
#include "tsc2046_utils.h"
#include "touchscreen_drv.h"

volatile T_TSC2046_DIS *tsc2046_dis;

/* touch screen globals */
static S32 m_DXdisplay = 1;
static S32 m_DXtouch = 1;
static S32 m_X0 = 0;
static S32 m_x0 = 0;
static S32 m_DYdisplay = 1;
static S32 m_DYtouch = 1;
static S32 m_Y0 = 0;
static S32 m_y0 = 0;

static U8 ts_calibration_done = 0;

S32 tsc2046_init(const void *init_str, U32 * dis_addr)
{
	S32 ret_status;
	T_MCSPI_INIT_STRUCTURE mcspi_init_str;

	/* Precalculated calibration values !! */
	S16 xA = 0x1F4;		/* touched points */
	S16 xB = 0xe30;
	S16 yA = 0x1F0;
	S16 yB = 0xeBC;
	S16 XA = 24;		/* display points */
	S16 XB = 240 - 24;
	S16 YA = 32;
	S16 YB = 320 - 32;

	/* malloc the tsc2046 DIS structure */

	tsc2046_dis = (volatile T_TSC2046_DIS *)malloc(sizeof(T_TSC2046_DIS));
	if (tsc2046_dis == 0) {
		printf("error\n");
		/* error */
		ret_status = FAILURE;
		return ret_status;
	}

	*dis_addr = (U32) tsc2046_dis;
	memcpy((T_TSC2046_DIS *) tsc2046_dis, init_str, sizeof(T_TSC2046_INIT_STRUCTURE));

	/*
	   Initialize the McSPI
	 */
	mcspi_init_str.sid = MCSPI_1;
	mcspi_init_str.irq_poll = MCSPI_POLL;
	mcspi_init_str.master = MCSPI_MASTER;

	// on 3530c silicon the maximum clk that 2046 can operate on is 2.5M
	// so configure any value lower than this
	// using divider 32 ,so spi clk will be 48/32=1.5M htz
	mcspi_init_str.clk_baudrate = 1500000;	/* 1.5Mbps */

	mcspi_init_str.wordlength = 16;
	mcspi_init_str.channel_number = 0;
	mcspi_init_str.cs_polarity = MCSPI_CS_ACTIVELOW;
	mcspi_init_str.clk_phase = MCSPI_DATA_LATCH_ODDEDGES;
	mcspi_init_str.clk_polarity = MCSPI_CLK_ACTIVEHIGH;
	mcspi_init_str.spi_mode = MCSPI_TXRX_MODE;
	ret_status = mcspi_init((void *)&mcspi_init_str, (U32 *) & (tsc2046_dis->mcspi_dis_handle));

	if (ret_status != SUCCESS) {
		printf("mcspi_init failed with ret_status= %d \n", (int)ret_status);
		return ret_status;
	}

	/* do the default calibrations only  once */
	if (ts_calibration_done == 0) {
		set_touchscreen_calibration(xB - xA, XB - XA, xA, XA, yB - yA, YB - YA, yA, YA);
		ts_calibration_done = 1;
	}
	return ret_status;

}				/* end tsc2046_init */

S32 tsc2046_read(U32 dis_addr, U32 tag, U32 * size, U8 * buffer)
{

	U16 *pBuffer = (U16 *) buffer;
	static S16 x = 0, y = 0;

	volatile T_TSC2046_DIS *p_tsc2046_dis = tsc2046_dis;	//(T_TSC2046_DIS *)dis_addr;

	/* Check if anything to be read from DIS */
	switch (tag) {

	case TSC2046_XY_POSITION_TAG:
		tsc2046_get_xy_position(p_tsc2046_dis->mcspi_dis_handle, &x, &y);
		*size = 4;	// 4 bytes read
		*pBuffer++ = x;
		*pBuffer = y;
		break;

	default:
		/* nothing from DIS */
		break;
	}

	return SUCCESS;

}				/* end tsc2046_read */

S32 tsc2046_write(U32 dis_addr, U32 tag, U32 * size, U8 * buffer)
{
	U16 *buff = (U16 *) buffer;
	switch (tag) {
	case TSC2046_SET_CALIB_TAG:
		set_touchscreen_calibration(buff[0], buff[1], buff[2], buff[3], buff[4], buff[5], buff[6], buff[7]);
		break;
	default:
		/* nothing from DIS */
		break;
	}
	return SUCCESS;

}				/* end tsc2046_write */

S32 tsc2046_deinit(U32 dis_addr)
{
	T_TSC2046_DIS *p_tsc2046_dis = (T_TSC2046_DIS *) dis_addr;

	if (dis_addr != 0) {
		/* free the allocated McSPI DIS */
		mcspi_deinit(p_tsc2046_dis->mcspi_dis_handle);

		/* free the allocated dis */
		free((void *)dis_addr);
	}

	return SUCCESS;

}				/* end tsc2046_deinit */

void tsc2046_get_spi_data(U32 spi_handle, U16 cntl_byte, U32 * read_data)
{
	U32 len = 1;
	U32 contr_byte = (U32) cntl_byte;
	// First Transmit the cntl byte.
	mcspi_write(spi_handle, MCSPI_RW_TAG, &len, (U8 *) (&contr_byte));

	// call mcspi_read ,this read returns the register data
	len = 1;
	mcspi_read(spi_handle, MCSPI_RW_TAG, &len, (U8 *) read_data);

}

void set_touchscreen_calibration(U32 DXtouch, U32 DXdisplay, U32 x0, U32 X0, U32 DYtouch, U32 DYdisplay, U32 y0, U32 Y0)
{
	m_DXtouch = DXtouch;	/* Touch value of the Distant X = m_DXtouch = xB-xA */
	m_DXdisplay = DXdisplay;	/* Display value of the Distant X= m_DXdisplay = XB-XA */
	m_x0 = x0;		/* Touch value of closer X = m_x0 = xA */
	m_X0 = X0;		/* Display value of closer X= m_X0=XA */
	m_DYtouch = DYtouch;	/* Touch value of the Distant Y = m_DYtouch = yB-yA */
	m_DYdisplay = DYdisplay;	/* Display value of the Distant Y= m_DYdisplay = YB-YA */
	m_y0 = y0;		/* Touch value of closer Y = m_y0 = yA */
	m_Y0 = Y0;		/* Display value of closer Y= m_Y0=YA */

}				/* end of set_touchscreen_calibration */

void tsc2046_get_xy_position(U32 spi_handle, S16 * x, S16 * y)
{
	U32 data = 0;
	U32 len = 1;
	U32 data1 = 0, data2 = 0, data3 = 0, data4 = 0, tempdata = 0;
	U16 px, py;
	S32 lx, ly;

	data = MCSPI_CHANNEL_ENABLE;
	mcspi_write(spi_handle, MCSPI_CHANNEL_EN_TAG, &len, (U8 *) (&data));

	/* Transmit the 0x8000 cntl byte */
	tsc2046_get_spi_data(spi_handle, 0x8000, &data1);

	/* Transmit the X cntl byte: D300 */
	tsc2046_get_spi_data(spi_handle, 0xD300, &data2);

	/* Transmit the Y cntl byte: 9300 */
	tsc2046_get_spi_data(spi_handle, 0x9300, &data3);

	/* Transmit the 0x8000 cntl byte */
	tsc2046_get_spi_data(spi_handle, 0x8000, &data4);

	// calculate y
	tempdata = data2 & 0x7F;
	tempdata = tempdata << 5;
	py = tempdata;

	tempdata = data3 & 0x0000F800;
	tempdata = tempdata >> 11;
	py |= tempdata;

	// calculate x
	tempdata = data3 & 0x7F;
	tempdata = tempdata << 5;
	px = tempdata;

	tempdata = data4 & 0x0000F800;
	tempdata = tempdata >> 11;
	px |= tempdata;

	/* disable spi channel */
	data = MCSPI_CHANNEL_DISABLE;
	mcspi_write(spi_handle, MCSPI_CHANNEL_EN_TAG, &len, (U8 *) (&data));

	lx = px;
	ly = py;

	// Apply the error correction.
	*x = ((lx - m_x0) * m_DXdisplay) / m_DXtouch + m_X0;
	*y = ((ly - m_y0) * m_DYdisplay) / m_DYtouch + m_Y0;

}
