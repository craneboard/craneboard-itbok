/*
 * mmc_readWrite.c: MMC driver.
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
 *   12 jan 2010 - Created from OMAP2 ITBOK source.
 */

#include "types.h"
#include <common.h>
#include "../inc/mmc.h"

static unsigned int mmc_high_density_card = 0;

u32 buffer[1024];

/*-----------------------------------------------------------------------------
| Function    : reset_mmc_lines
+------------------------------------------------------------------------------
|
| Description : soft resets the cmd line or data lines
|
| Parameters  : U32 reset_val - (cmd line or data line reset val)
|
| Returns     : S32
+-----------------------------------------------------------------------------*/
s32 reset_mmc_lines(u32 reset_val)
{
	u32 j = 1000;
	u32 temp_data = 0;

	MMCHS_FSET_R(MMCHS_SYSCTL, reset_val);

	do {			/*check reset state */
		temp_data = MMCHS_GET_R(MMCHS_SYSCTL);
		temp_data &= reset_val;
		j--;		/*to avoid the dead loop */
		if (j == 0) {
			return ERROR;
		}

		udelay(ONE_MILLISEC);

	}
	while (temp_data == reset_val);

	return SUCCESS;
}

/*-----------------------------------------------------------------------------
| Function    : send_mmc_cmd
+------------------------------------------------------------------------------
|
| Description : sends a command to the card.
|
| Parameters  : u32 cmd, u16 argh, u16 argl
|
| Returns     : s32
|
+-----------------------------------------------------------------------------*/
s32 send_mmc_cmd(u32 cmd, u16 argh, u16 argl)
{
	u32 status, retry = 5000;
	u32 arg;
	u32 count = 0xFFFFFFFF;

	/* clear the MMCSTAT register of its previous value */
	MMCHS_ASET_R(MMCHS_STAT, 0xFFFFFFFF);

	MMCHS_SET_R(MMCHS_BLK, (MMCHS_NBLK_ONE | MMCHS_BLKSZ_512));
	MMCHS_FSET_R(MMCHS_SYSCTL, (MMCHS_DTO_MAX));

	/*generate a single arg */
	arg = argh;
	arg = (arg << 16);
	arg = (arg | argl);

	retry = 500;
	while (((MMCHS_GET_R(MMCHS_PSTAT) & MMCHS_PSTAT_CMDI) != 0) && (retry > 0)) {
		udelay(ONE_MILLISEC);
		retry--;
	}
	if ((retry <= 0) && (cmd != MMCHS_GO_IDLE_STATE)) {
		printf("CMD: 0x%x, MMCHS_PSTAT not out from the CMDI state!\r\n", (cmd & MMCHS_CMD63));
	}

	MMCHS_SET_R(MMCHS_ARG, arg);	/*send the argumant value */

	MMCHS_SET_R(MMCHS_IE, 0xFFFFFEFF);	/*status enable */
	MMCHS_SET_R(MMCHS_CMD, cmd);	/*send the command data */

	/* Wait for response done */
	do {
		/* Get status */
		status = MMCHS_GET_R(MMCHS_STAT);

		/* Status is Eod of command phase - OK */
		if ((((status & MMCHS_MMCSTAT_CCRC) == MMCHS_MMCSTAT_CCRC) && ((status & MMCHS_MMCSTAT_CTO) == MMCHS_MMCSTAT_CTO))) {
			if (reset_mmc_lines(MMCHS_SYSCTL_SRC) == ERROR) {
				/*terminate the loop if SRC is not cleared */
				printf("soft reset command line failed..\r\n");
			}
			break;
		} else if ((status & MMCHS_MMCSTAT_CTO) == MMCHS_MMCSTAT_CTO) {
			if (reset_mmc_lines(MMCHS_SYSCTL_SRC) == ERROR) {
				/*terminate the loop if SRC is not cleared */
				printf("soft reset command line failed..\r\n");
			}

			break;
		} else if ((status & MMCHS_MMCSTAT_CC) == MMCHS_MMCSTAT_CC) {
			break;
		}
		count--;
	}
	while (count);

	status = MMCHS_GET_R(MMCHS_STAT);

	if ((status & MMCHS_MMCSTAT_CCRC) != 0) {
		printf("Command CRC error happened during cmd %x\r\n", cmd);
	}

	if ((status & MMCHS_MMCSTAT_DCRC) != 0) {
		printf("DATA CRC error happened during cmd %x\r\n", cmd);
	}

	if ((status & MMCHS_MMCSTAT_CERR) != 0) {
		printf("CMD: 0x%x, Status: 0x%x, MMCHS_MMCSTAT_CERR!\r\n", (cmd & MMCHS_CMD63), status);

		return MMC_CARD_ER;
	}

	if ((status & MMCHS_MMCSTAT_CTO) != 0) {
		if (cmd == SDHS_APP_CMD) {
			printf("CMD: 0x%x, Status: 0x%x, MMCHS_MMCSTAT_CTO!\r\n", (cmd & MMCHS_CMD63), status);
		} else {
			printf("CMD: 0x%x, Status: 0x%x, MMCHS_MMCSTAT_CTO!\r\n", (cmd & MMCHS_CMD63), status);
		}

		return MMC_CMD_TO_ER;
	} else if ((status & MMCHS_MMCSTAT_CC) != 0) {
		printf("CMD: 0x%x, Status: 0x%x\r\n", (cmd & MMCHS_CMD63), status);

		return SUCCESS;
	} else {
		return MMC_CMD_FAIL;
	}

}				/* end of send_mmc_cmd */

/*-----------------------------------------------------------------------------
| Function    : data_transfer_complete
+------------------------------------------------------------------------------
|
| Description : Check the data transfers complete or not
|
| Parameters  : void
|
| Returns     : u32
|
+-----------------------------------------------------------------------------*/
u32 data_transfer_complete(void)
{
	u32 i;

	i = 1000;
	/*Wait for Block ready for read */
	while (((MMCHS_GET_R(MMCHS_STAT) & MMCHS_MMCSTAT_TC) != MMCHS_MMCSTAT_TC)
	       && (i > 0)) {
		udelay(ONE_MILLISEC);
		i--;

		if ((MMCHS_GET_R(MMCHS_STAT) & (MMCHS_MMCSTAT_DEB | MMCHS_MMCSTAT_DCRC | MMCHS_MMCSTAT_DTO))) {
			if (reset_mmc_lines(MMCHS_SYSCTL_SRD) == ERROR) {
				/*terminate the loop if SRD is not cleared */
				printf("Controller soft reset data failed..\r\n");
			}

			return ERROR;	/*In Bustest Procedure no need to send error for DTO */
		}

	}
	if (i == 0) {
		printf("DataTransComplete: TC state not set -status: %x\r\n", (unsigned int)MMCHS_GET_R(MMCHS_STAT));
		return ERROR;
	}

	return SUCCESS;
}

/*-----------------------------------------------------------------------------
| Function    : mmc_write_block_data()
+------------------------------------------------------------------------------
|
| Description : writes given data pattern to one block memory of the card from
|               the given card's address
|
| Parameters  : u32 card_addr, void *buffer
|
| Returns     : u32
|
+-----------------------------------------------------------------------------*/
u32 mmc_write_block_data(u32 card_addr, void *buffer)
{
	u32 ret_value = SUCCESS;
	u32 j = 1000;
	u32 i = 1000;
	u32 *buf = (u32 *) buffer;
	u32 temp_data;

	j = 500;
	while (((MMCHS_GET_R(MMCHS_PSTAT) & MMCHS_PSTAT_DATI) != 0) && (j > 0)) {
		udelay(ONE_MILLISEC);
		j--;
	}
	if (j <= 0) {
		printf("MMCHS_PSTAT not out from the DATI busy state!\r\n");

		return MMC_BUSY_ER;
	}

	ret_value = send_mmc_cmd(MMCHS_WRITE_SINGLE_BLOCK, (u16) ((card_addr >> 16) & MMC_STUFF_BITS), (u16) (card_addr & MMC_STUFF_BITS));
	if (SUCCESS != ret_value) {
		j = 0;
		return ret_value;
	}

	for (j = 0; j < (MMC_BLOCK_SIZE / sizeof(u32)); j++)	// write data to DATA buffer
	{
		i = 1000;
		/*Wait for Block ready for read */
		while (((MMCHS_GET_R(MMCHS_STAT) & MMCHS_MMCSTAT_BWR) != MMCHS_MMCSTAT_BWR) && (i > 0)) {
			udelay(ONE_MILLISEC);
			i--;
		}
		if (i == 0) {
			printf("Card could not Set for BWR state: Read status: %x\r\n", (unsigned int)MMCHS_GET_R(MMCHS_STAT));
			return MMC_BUSY_ER;
		}

		temp_data = *buf;
		MMCHS_SET_R(MMCHS_DATA, temp_data);
	}

	ret_value = data_transfer_complete();
	if (SUCCESS != ret_value) {
		printf("WRITE BLOCK: TC state not set -status: %x\r\n", (unsigned int)MMCHS_GET_R(MMCHS_STAT));
		return ret_value;
	}

	return ret_value;
}

/*-----------------------------------------------------------------------------
| Function    : mmc_read_single_block()
+------------------------------------------------------------------------------
|
| Description : reads single block of data and puts into the buffer
|
| Parameters  : u32 card_addr, void *buffer
|
| Returns     : u32
|
+-----------------------------------------------------------------------------*/
u32 mmc_read_single_block(u32 card_addr, void *buffer)
{
	u32 ret_value = SUCCESS;
	u32 i = 1000;
	u32 j = 1000;
	u32 *buf = (u32 *) buffer;

	i = 500;
	while (((MMCHS_GET_R(MMCHS_PSTAT) & MMCHS_PSTAT_DATI) != 0) && (i > 0)) {

		udelay(ONE_MILLISEC);
		i--;
	}
	if (i <= 0) {
		printf("MMCHS_PSTAT not out from the DATI busy state!\r\n");

		return MMC_BUSY_ER;
	}

	/* CMD17 R1 (READ_SINGLE_BLOCK) Arguments are address specified by caller */
	ret_value = send_mmc_cmd(MMCHS_READ_SINGLE_BLOCK, (u16) ((card_addr >> 16) & MMC_STUFF_BITS), (u16) (card_addr & MMC_STUFF_BITS));
	if (SUCCESS != ret_value) {
		return ret_value;
	}

	/* Get all data from DATA register and write in user buffer */
	for (i = 0; i < (MMC_BLOCK_SIZE / sizeof(u32)); i++) {
		j = 1000;
		/*Wait for Block ready for read */
		while (((MMCHS_GET_R(MMCHS_STAT) & MMCHS_MMCSTAT_BRR) != MMCHS_MMCSTAT_BRR) && (j > 0)) {

			udelay(ONE_MILLISEC);
			j--;
		}
		if (j == 0) {
			printf("Card could not Set for BRR state: Read status: %x\r\n", (unsigned int)MMCHS_GET_R(MMCHS_STAT));
			return MMC_BUSY_ER;
		}

		*buf = MMCHS_GET_R(MMCHS_DATA);
		buf++;
	}

	ret_value = data_transfer_complete();
	if (SUCCESS != ret_value) {
		printf("READ BLOCK: TC state not set -status: %x\r\n", (unsigned int)MMCHS_GET_R(MMCHS_STAT));
		return ret_value;
	}

	return ret_value;
}

/******************************************************************************/
/*-----------------------------------------------------------------------------
| Function    : get_mmc_cmd_response
+------------------------------------------------------------------------------
|
| Description : gets the response for the send command.
|
| Parameters  : u32 *resp, u8 response_type
|
| Returns     : u32
|
+-----------------------------------------------------------------------------*/
u32 get_mmc_cmd_response(u32 * resp, u8 response_type)
{
	u32 ret_value = SUCCESS;

	if (MMC_RSP2 == response_type) {
		*resp = MMCHS_GET_R(MMCHS_RSP10);
		resp++;
		*resp = MMCHS_GET_R(MMCHS_RSP32);
		resp++;
		*resp = MMCHS_GET_R(MMCHS_RSP54);
		resp++;
		*resp = MMCHS_GET_R(MMCHS_RSP76);
	} else {
		*resp = MMCHS_GET_R(MMCHS_RSP10);
	}

	return ret_value;
}

/*-----------------------------------------------------------------------------
| Function    : is_card_tranfer_state
+------------------------------------------------------------------------------
|
| Description : Checks whether the card is in transferstate or not
|
| Parameters  : u16 card_rca
|
| Returns     : u32
|
+-----------------------------------------------------------------------------*/
u32 is_card_tranfer_state(u16 card_rca)
{
	u32 i;
	u32 res_data = 0;
	u32 ret_value = SUCCESS;

	for (i = 0; (i < 10) && (res_data != MMC_CARD_STATE_TRAN); i++) {
		ret_value = get_mmc_cmd_response(&res_data, MMC_RSP1);
		if (SUCCESS != ret_value) {
			printf("FAIL: BUSY state\n");
			return ret_value;
		}

		printf("Card state 0x%x, i= %d\r\n", res_data, i);

		res_data = (res_data & MMC_CARD_STATE_TRAN);
	}

	if (res_data != MMC_CARD_STATE_TRAN) {
		printf("Card is in BUSY state\n");
		return (MMC_BUSY_ER);	/*Busy in other state */
	}

	return SUCCESS;
}

/*-----------------------------------------------------------------------------
| Function    : sd_mmc_card_write
+------------------------------------------------------------------------------
|
| Description : writes the given data to card with given tranfer speed and data
|               width
|
| Parameters  : u16 card_rca, u8 card_type,u8 data_width,
|               u32 transfer_clk,u32 len,void *buf
|
| Returns     : u32
|
+-----------------------------------------------------------------------------*/
u32 sd_mmc_card_write(u16 card_rca, u8 card_type, u8 data_width, u32 transfer_clk, u32 len, void *buf)
{
	u32 ret_value = SUCCESS;
	u32 *data = (u32 *) buf;
	u32 card_addr = 0;
	u32 nu_blocks;
	u32 pattern;
	u32 start_block, i;
	u8 mmc_slot;

	mmc_slot = 1;

	ret_value = is_card_tranfer_state(card_rca);
	printf("card transfer state %d\n", ret_value);

	if (SUCCESS != ret_value) {
		printf("FAIL: card is NOT in transfer state %d\n", ret_value);
		return ret_value;
	}
	start_block = (*data);
	data++;
	nu_blocks = (*data);
	data++;
	pattern = (*data);
	printf("Start block = %u\n", start_block);
	printf("number of block = %u\n", start_block);
	printf("pattern = %x\n", pattern);

	for (i = 0; i < nu_blocks; i++) {
		ret_value = is_card_tranfer_state(card_rca);
		if (SUCCESS != ret_value) {
			return ret_value;
		}
		if (mmc_high_density_card == 1) {
			/* High Density card - Sectorwise addressing */
			card_addr = (start_block + i);
		} else {
			/* Normal card - Bytewise addressing */
			card_addr = (start_block + i) * MMC_BLOCK_SIZE;
		}

		printf("card Address = 0x%x\n", card_addr);
		ret_value = mmc_write_block_data(card_addr, &pattern);
		if (SUCCESS != ret_value) {
			printf("Write fails \n");
			return ret_value;
		}
	}
	ret_value = mmc_read_single_block(card_addr, &buffer);
	if (SUCCESS != ret_value) {
		printf("Read fails \n");
		return ret_value;
	}
	printf("Verifying the data.....");
	for (i = 0; i < 128; i++) {

		if (buffer[i] != pattern) {
			printf("Data does not match\n");
			return (ERROR);
		}
	}

	printf("done.\n");
	return ret_value;
}

/*-----------------------------------------------------------------------------
| Function    : mmc_write()
+------------------------------------------------------------------------------
| Description : This functions implements the write data,
|                and erase for the mmc.
|
| parameters: dis_addr, tag, len, buf
|
| return : s32
|-----------------------------------------------------------------------------*/

s32 mmc_write(u32 * len, u8 * buf)
{				/*Write to the mmc */

	s32 ret_val = SUCCESS;

	ret_val = sd_mmc_card_write(4, 1, 4, 48000,	// 26000,
				    *len, buf);
	return ret_val;
}

/*-----------------------------------------------------------------------------
| Function    : mmc_write_test
+------------------------------------------------------------------------------
|
| Description : write to the mmc card
|
| Parameters  : u32 sdu_handle, u8 mmc_slot_number, u16 mmc_volt,
|               u8 data_width, u32 data_trans_freq, u32 start_block_number,
|               u32 number_of_blocks,u32 data_pattern
|
| Returns     : void
|
+-----------------------------------------------------------------------------*/
int mmc_write_test(void)
{
	s32 ret_val = SUCCESS;
	u32 buf[3], size;

	printf("Writing SD/MMC...");

	size = sizeof(u8);

	size = sizeof(u32);

	size = sizeof(u32);

	/*write file */
	size = sizeof(u8);

	size = sizeof(u32);

	buf[0] = 512;		//start_block_number;
	buf[1] = 1;		//number_of_blocks;
	buf[2] = 0xa5;		//data_pattern;
	size = 512;

	ret_val = mmc_write(&size, (u8 *) buf);
	if (ret_val != SUCCESS) {
		printf("MMC Write Test failed");
		return -1;
	} else {
		printf("MMC Raw Read Write Test Completed Successfully");
		return 0;
	}

}
