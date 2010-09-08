/*
 * dg_i2c.c: i2c diagnostic code.
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

/* includes */
#include <common.h>
#include <i2c.h>
#include "diag_core.h"
#include "omap3530evm.h"
#include "types.h"
#include "dg_gpio.h"
#include "i2c_utils.h"
#include "i2c_drv.h"
#include "dg_i2c.h"

extern unsigned long strtoul(char *cp);

void register_i2c_tests(void)
{
	status_t ret;
	char print_buf[512];

	DEBUG("Registering I2C diagnostics.\n");
	ret = diag_hw_interface_add(do_i2c_diag, "i2c", "Test I2C interface");
	if (ret != SUCCESS) {
		printf("Failed to add I2C interface.\n");
	}

	/* I2C write test. */
	sprintf(print_buf, "Writes to the I2C device\
                    \n\r\t\tParamaters:\
                    \n\r\t\t[I2C_number] - I2C number (1 or 2)\
                    \n\r\t\t[Device_ID ] - Device ID (8-bit value)\
                    \n\r\t\t[Offset    ] - Offset (8-bit value)\
                    \n\r\t\t[Data      ] - Data to be written (8-bit value)");

	ret = diag_hw_test_add("i2c", DG_I2C_WRITE, "write", print_buf, 4, TYPE_U8, "I2C_number", TYPE_U8, "Device_ID", TYPE_U8, "Offset", TYPE_U8, "Data");

	/* I2C read test. */
	sprintf(print_buf, "Reads from the I2C device\
                    \n\r\t\tParamaters:\
                    \n\r\t\t[I2C_number] - I2C number (1 or 2)\
                    \n\r\t\t[Device_ID ] - Device ID (8-bit value)\
                    \n\r\t\t[Offset    ] - Offset (8-bit value)\
                    \n\r\t\t[Size      ] - Number of bytes to be read (1 to 255)");

	ret = diag_hw_test_add("i2c", DG_I2C_READ, "read", print_buf, 4, TYPE_U8, "I2C_number", TYPE_U8, "Device_ID", TYPE_U8, "Offset", TYPE_U8, "Size");

	/* High speed I2C write test. */
	sprintf(print_buf, "High speed I2C write\
                    \n\r\t\tParamaters:\
                    \n\r\t\t[I2C_number] - I2C number (1 or 2)\
                    \n\r\t\t[Device_ID ] - Device ID (8-bit value)\
                    \n\r\t\t[Offset    ] - Offset (8-bit value)\
                    \n\r\t\t[Data      ] - Data to be written (8-bit value)\
                    \n\r\t\t[Size      ] - Number of bytes to be written (1 to 255)");

	ret = diag_hw_test_add("i2c", DG_I2C_HSWRITE, "hswrite", print_buf, 5, TYPE_U8, "I2C_number", TYPE_U8, "Device_ID", TYPE_U8, "Offset", TYPE_U8, "Data", TYPE_U8, "Size");

	/* High speed I2C read test. */
	sprintf(print_buf, "High speed I2C read\
                    \n\r\t\tParamaters:\
                    \n\r\t\t[I2C_number] - I2C number (1 or 2)\
                    \n\r\t\t[Device_ID ] - Device ID (8-bit value)\
                    \n\r\t\t[Offset    ] - Offset (8-bit value)\
                    \n\r\t\t[Size      ] - Number of bytes to be written (1 to 255)");

	ret = diag_hw_test_add("i2c", DG_I2C_HSREAD, "hsread", print_buf, 4, TYPE_U8, "I2C_number", TYPE_U8, "Device_ID", TYPE_U8, "Offset", TYPE_U8, "Size");
}

status_t do_i2c_diag(u8 diag_id, int argc, char *argv[])
{
	U8 i2c_no;
	U8 slave_addr;
	U8 offset;
	U8 size;
	U8 data;
	S32 ret_val = SUCCESS;

	/* Read the test parameters. */
	i2c_no = (U8) strtoul(argv[0]);
	slave_addr = (U8) strtoul(argv[1]);
	offset = (U8) strtoul(argv[2]);
	data = (U8) strtoul(argv[3]);
	size = 1;

#ifdef OMAP3530_DEBUG
	DEBUG("i2c_number :0x%x\r\n", i2c_no);
	DEBUG("slave_addr :0x%x\r\n", slave_addr);
	DEBUG("offset     :0x%x\r\n", offset);
	DEBUG("data       :0x%x\r\n", data);
#endif

	/* Sanity check. */
	if (NULL == argv[3]) {	/* Invalid data buffer. */
		fprintf(stdout, "Invalid Data.\r\n");
		return FAILURE;
	}

	if (slave_addr <= 0) {	/* Invalid slave address. */
		fprintf(stdout, "Invalid Slave Address.\r\n");
		return FAILURE;
	}

	if ((1 != i2c_no) && (2 != i2c_no)) {	/* I2C number validation. */
		fprintf(stdout, "Invalid I2C Number.\r\n");
		return FAILURE;
	}

	switch (diag_id) {
	case DG_I2C_WRITE:
		ret_val = diag_i2c_write(i2c_no, slave_addr, &data, offset, size);
		break;

	case DG_I2C_READ:
		size = (U8) strtoul(argv[3]);
		ret_val = diag_i2c_read(i2c_no, slave_addr, offset, size);
		break;

	case DG_I2C_HSWRITE:
		ret_val = diag_i2c_hswrite(i2c_no, slave_addr, &data, offset, size);
		break;

	case DG_I2C_HSREAD:
		size = (U8) strtoul(argv[3]);
		ret_val = diag_i2c_hsread(i2c_no, slave_addr, offset, size);
		break;

	default:
		fprintf(stdout, "Invalid test request received.\r\n");
		break;
	}

	return ret_val;
}

/* I2C write test. */
status_t diag_i2c_write(U8 i2c_no, U8 slave_addr, U8 * data, U8 offset, U8 size)
{
	i2c_init_structure_t init_struct;
	S32 ret_val = SUCCESS;
	U32 tempsize = size;

	/* Initilize the init structure. */
	if (1 == i2c_no) {
		init_struct.i2c_no = I2C1;
	} else {
		init_struct.i2c_no = I2C2;
	}
	init_struct.slave_addr = slave_addr;	/* Slave address.    */
	init_struct.clock_speed = I2C_100K_CLK;	/* Set for 100 kbps. */

#ifdef OMAP3530_DEBUG
	DEBUG("I2C access @100KHz (Standard Mode).\n\r");
	DEBUG("Init Structure is : ");
	DEBUG("I2C NO: %d Slave address: %d Clock_Speed: %d kbps\r\n", init_struct.i2c_no, init_struct.slave_addr, init_struct.clock_speed);
	DEBUG("Offset: %0xX Data: %0xX Size: %d\r\n", offset, *data, size);
#endif

	/* Initialize the i2c. */
	ret_val = dg_i2c_init(&init_struct);
	if (SUCCESS != ret_val) {
#ifdef OMAP3530_DEBUG
		DEBUG("%s:%d - I2C init failed.\n\r", __FILE__, __LINE__);
#endif
		return ret_val;
	}

	/* Write to the device. */
	ret_val = dg_i2c_write(&init_struct, I2C_OFFSET(offset), &tempsize, data);
	if (SUCCESS != ret_val) {
#ifdef OMAP3530_DEBUG
		DEBUG("%s:%d - I2C write failed.\n\r", __FILE__, __LINE__);
#endif
		dg_i2c_deinit(&init_struct);
		return ret_val;
	}

	/* Deinitialize the device. */
	ret_val = dg_i2c_deinit(&init_struct);
	if (SUCCESS != ret_val) {
#ifdef OMAP3530_DEBUG
		DEBUG("%s:%d - deinit failed.\n\r", __FILE__, __LINE__);
#endif
		return ret_val;
	}

	return ret_val;
}

/* I2C read test. */
status_t diag_i2c_read(U8 i2c_no, U8 slave_addr, U8 offset, U8 size)
{
	i2c_init_structure_t init_struct;
	S32 ret_val = SUCCESS;
	U32 tempsize = 1, count = 0;
	U8 data = 0;

	/* Initilize the init structure. */
	if (1 == i2c_no) {
		init_struct.i2c_no = I2C1;
	} else {
		init_struct.i2c_no = I2C2;
	}
	init_struct.slave_addr = slave_addr;	/* Slave address.    */
	init_struct.clock_speed = I2C_100K_CLK;	/* Set for 100 kbps. */

#ifdef OMAP3530_DEBUG
	DEBUG("I2C access @100KHz (Standard Mode).\n\r");
	DEBUG("Init Structure is : ");
	DEBUG("I2C NO: %d Slave address: %d Clock_Speed: %d kbps\r\n", init_struct.i2c_no, init_struct.slave_addr, init_struct.clock_speed);
#endif

	/* Initialize the i2c. */
	ret_val = dg_i2c_init(&init_struct);
	if (SUCCESS != ret_val) {
#ifdef OMAP3530_DEBUG
		DEBUG("%s:%d - I2C init failed.\n\r", __FILE__, __LINE__);
#endif
		return ret_val;
	}

	for (count = 0; count < size; count++) {
		/* Read from the device. */
		ret_val = dg_i2c_read(&init_struct, I2C_OFFSET(offset), &tempsize, &data);
		if (SUCCESS != ret_val) {
#ifdef OMAP3530_DEBUG
			DEBUG("%s:%d - I2C read failed.\n\r", __FILE__, __LINE__);
#endif
			dg_i2c_deinit(&init_struct);
			return ret_val;
		}
#ifdef OMAP3530_DEBUG
		DEBUG("Address: 0x%.2X, -> Data: 0x%.2X\r\n", offset, data);
#endif
		/* Move to the next offset. */
		offset += 1;
	}

	/* Deinitialize the device. */
	ret_val = dg_i2c_deinit(&init_struct);
	if (SUCCESS != ret_val) {
#ifdef OMAP3530_DEBUG
		DEBUG("%s:%d - deinit failed.\n\r", __FILE__, __LINE__);
#endif
		return ret_val;
	}

	fprintf(stdout, "I2C read success.\n\r");
	return ret_val;
}

/* I2C high speed write test. */
status_t diag_i2c_hswrite(U8 i2c_no, U8 slave_addr, U8 * data, U8 offset, U8 size)
{
	i2c_init_structure_t init_struct;
	S32 ret_val;
	U32 tempsize = size;

	/* Initilize the init structure. */
	if (1 == i2c_no) {
		init_struct.i2c_no = I2C1;
	} else {
		init_struct.i2c_no = I2C2;
	}
	init_struct.slave_addr = slave_addr;	/* Slave address.    */

	/* Checking whether access is to T2. */
	if ((0x48 == slave_addr) || (0x49 == slave_addr)
	    || (0x4A == slave_addr) || (0x4B == slave_addr)) {
		/* T2 maximum HS I2C speed is 2.6MHz, so the access is made at 2.6MHz */
		init_struct.clock_speed = I2C_2P6M_CLK;	/* Set for 2.6 mbps. */
	} else {
		init_struct.clock_speed = I2C_3P4M_CLK;	/* Set for 3.4 mbps. */
	}

	if (I2C_3P4M_CLK == init_struct.clock_speed) {
#ifdef OMAP3530_DEBUG
		DEBUG("I2C access @3.4MHz (HS Mode).\n\r");
#endif
	} else if (I2C_2P6M_CLK == init_struct.clock_speed) {
#ifdef OMAP3530_DEBUG
		DEBUG("I2C access @2.6MHz (HS Mode).\n\r");
#endif
	} else if (I2C_1P95M_CLK == init_struct.clock_speed) {
#ifdef OMAP3530_DEBUG
		DEBUG("I2C access @1.95MHz (HS Mode).\n\r");
#endif
	}

	/* Print init structure for debug purpose. */
	DEBUG("Init Structure is : ");
	DEBUG("I2C NO: %d Slave address: %d Clock_Speed: %d kbps\r\n", init_struct.i2c_no, init_struct.slave_addr, init_struct.clock_speed);
	DEBUG("Offset: %0xX Data: %0xX Size: %d\r\n", offset, *data, size);

	/* Initialize the i2c. */
	ret_val = dg_i2c_init(&init_struct);
	if (SUCCESS != ret_val) {
#ifdef OMAP3530_DEBUG
		DEBUG("%s:%d - I2C init failed.\n\r", __FILE__, __LINE__);
#endif
		return ret_val;
	}

	/* Write to the device. */
	ret_val = dg_i2c_write(&init_struct, I2C_OFFSET(offset), &tempsize, data);
	if (SUCCESS != ret_val) {
#ifdef OMAP3530_DEBUG
		DEBUG("%s:%d - I2C write failed.\n\r", __FILE__, __LINE__);
#endif
		dg_i2c_deinit(&init_struct);
		return ret_val;
	}

	/* Deinitialize the device. */
	ret_val = dg_i2c_deinit(&init_struct);
	if (SUCCESS != ret_val) {
#ifdef OMAP3530_DEBUG
		DEBUG("%s:%d - deinit failed.\n\r", __FILE__, __LINE__);
#endif
		return ret_val;
	}

	fprintf(stdout, "I2C hswrite success.\n\r");
	return ret_val;
}

/* I2C high speed read test. */
status_t diag_i2c_hsread(U8 i2c_no, U8 slave_addr, U8 offset, U8 size)
{
	i2c_init_structure_t init_struct;
	S32 ret_val = SUCCESS;
	U32 tempsize = 1, count = 0;
	U8 data = 0;

	/* Initilize the init structure. */
	if (1 == i2c_no) {
		init_struct.i2c_no = I2C1;
	} else {
		init_struct.i2c_no = I2C2;
	}
	init_struct.slave_addr = slave_addr;	/* Slave address.    */

	/* Checking whether access is to T2. */
	if ((0x48 == slave_addr) || (0x49 == slave_addr)
	    || (0x4A == slave_addr) || (0x4B == slave_addr)) {
		/* T2 maximum HS I2C speed is 2.6MHz, so the access is made at 2.6MHz */
		init_struct.clock_speed = I2C_2P6M_CLK;	/* Set for 2.6 mbps. */
	} else {
		init_struct.clock_speed = I2C_3P4M_CLK;	/* Set for 3.4 mbps. */
	}

	if (I2C_3P4M_CLK == init_struct.clock_speed) {
#ifdef OMAP3530_DEBUG
		DEBUG("I2C access @3.4MHz (HS Mode).\n\r");
#endif
	} else if (I2C_2P6M_CLK == init_struct.clock_speed) {
#ifdef OMAP3530_DEBUG
		DEBUG("I2C access @2.6MHz (HS Mode).\n\r");
#endif
	} else if (I2C_1P95M_CLK == init_struct.clock_speed) {
#ifdef OMAP3530_DEBUG
		DEBUG("I2C access @1.95MHz (HS Mode).\n\r");
#endif
	}

	/* Print init structure for debug purpose. */
	DEBUG("Init Structure is : ");
	DEBUG("I2C NO: %d Slave address: %d Clock_Speed: %d kbps\r\n", init_struct.i2c_no, init_struct.slave_addr, init_struct.clock_speed);

	/* Initialize the i2c. */
	ret_val = dg_i2c_init(&init_struct);
	if (SUCCESS != ret_val) {
#ifdef OMAP3530_DEBUG
		DEBUG("%s:%d - I2C init failed.\n\r", __FILE__, __LINE__);
#endif
		return ret_val;
	}

	for (count = 0; count < size; count++) {
		/* Read from the device. */
		ret_val = dg_i2c_read(&init_struct, I2C_OFFSET(offset), &tempsize, &data);
		if (SUCCESS != ret_val) {
#ifdef OMAP3530_DEBUG
			DEBUG("%s:%d - I2C read failed.\n\r", __FILE__, __LINE__);
#endif
			dg_i2c_deinit(&init_struct);
			return ret_val;
		}
#ifdef OMAP3530_DEBUG
		DEBUG("Address: 0x%.2X, -> Data: 0x%.2X\r\n", offset, data);
#endif
		/* Move to the next offset. */
		offset += 1;
	}

	/* Deinitialize the device. */
	ret_val = dg_i2c_deinit(&init_struct);
	if (SUCCESS != ret_val) {
#ifdef OMAP3530_DEBUG
		DEBUG("%s:%d - deinit failed.\n\r", __FILE__, __LINE__);
#endif
		return ret_val;
	}

	fprintf(stdout, "I2C hsread success.\n\r");
	return ret_val;
}
