/*
 * eeprom.c: EEPROM functions.
 *
 * (C) Copyright 2010
 * Mistral Solutions Private Limited, <www.mistralsolutions.com>
 *
 * Author :
 * Jitendra <jitendra@mistralsolutions.com>
 *
 * Derived from kernel source.
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
 *   10 Mar 2009 - Created from OMAP2 ITBOK source.
 */

#include <common.h>
#include <command.h>
#include <asm/io.h>
#include <linux/ctype.h>

#define EEPROM_SUCCESS	0
#define EEPROM_FAIL		-1
#define SIGNATURE_BYTE	0xA5
/* Below are the register offsets and bit definitions
 * of the Lan911x memory space
 */
#define	ENABLE_EEPROM				0xFF8FFFFF
#define	GPIO_CFG					(CONFIG_SMC911X_BASE + 0x88)
#define	E2P_CMD_EPC_BUSY_			(0x80000000)	/* Self Clearing */
#define	E2P_CMD_EPC_CMD_			(0x70000000)	/* R/W */
#define	E2P_CMD_EPC_CMD_READ_		(0x00000000)	/* R/W */
#define	E2P_CMD_EPC_CMD_EWDS_		(0x10000000)	/* R/W */
#define	E2P_CMD_EPC_CMD_EWEN_		(0x20000000)	/* R/W */
#define	E2P_CMD_EPC_CMD_WRITE_		(0x30000000)	/* R/W */
#define	E2P_CMD_EPC_CMD_WRAL_		(0x40000000)	/* R/W */
#define	E2P_CMD_EPC_CMD_ERASE_		(0x50000000)	/* R/W */
#define	E2P_CMD_EPC_CMD_ERAL_		(0x60000000)	/* R/W */
#define	E2P_CMD_EPC_CMD_RELOAD_		(0x70000000)	/* R/W */
#define	E2P_CMD_EPC_TIMEOUT_		(0x00000200)	/* RO */
#define	E2P_CMD_MAC_ADDR_LOADED_	(0x00000100)	/* RO */
#define	E2P_CMD_EPC_ADDR_			(0x000000FF)	/* R/W */

#define	E2P_CMD						(CONFIG_SMC911X_BASE + 0xB0)
#define	E2P_DATA					(CONFIG_SMC911X_BASE + 0xB4)
#define	E2P_DATA_EEPROM_DATA		0x000000FF	/* R/W */
#define	ID_REV_ID_MASK				(0xFFFF0000)
#define	ID_REV_CHIP_118				(0x01180000)
#define	ID_REV_CHIP_115				(0x01150000)
#define	ID_REV_CHIP_218				(0x118A0000)
#define	ID_REV_REV_MASK				(0x0000FFFF)

#define	MAC_CSR_CMD					(CONFIG_SMC911X_BASE + 0xA4)
#define	MAC_CSR_CMD_CSR_BUSY		0x80000000	/* Self Clearing */
#define	MAC_CSR_CMD_R_NOT_W			0x40000000	/* R/W */
#define	MAC_CSR_CMD_CSR_ADDR		0x000000FF	/* R/W */

#define	MAC_CSR_DATA				(CONFIG_SMC911X_BASE + 0xA8)
#define	ID_REV						(CONFIG_SMC911X_BASE + 0x50)

#define	ID_REV_CHIP_ID				0xFFFF0000	/* RO */
#define	ID_REV_REV_ID				0x0000FFFF	/* RO */
#define	BYTE_TEST					(volatile unsigned long *)(CONFIG_SMC911X_BASE + 0x64)

#define	mdelay(n)					udelay((n)*1000)
#define	DELAY(n)	( {   \
							int _i = n; \
							do { \
							ulong _temp; \
							 _temp = *BYTE_TEST; \
							} while (--_i); \
						} )

/* Board Revision offset and Length offset */
#define	EEPROM_START_OFFSET			0
#define	EEPROM_END_OFFSET			0x3FF
#define EEPROM_REV_LENGTH_OFFSET	7
#define EEPROM_REV_BYTE_OFFSET		8

/* Board info str len */
#define BOARD_INFO_STRLEN		20
/***********************************************************/
/* Reset the LAN9220 					                   */
/***********************************************************/
void reset_smsc_lan(void)
{
	int i = 0;
	/* GPIO 7 configuration in CONTROL_PADCONF_SYS_BOOT5 register mux mode is 4. */
	(*(volatile int *)0x48002A14) |= 0x00000018;
	(*(volatile int *)0x48002A14) &= 0xFFFFFFF8;
	(*(volatile int *)0x48002A14) |= 0x00000004;

	/* Enable Clock for GPIO 1-6 module in CM_FCLKEN_PER and CM_ICLKEN_PER registers */
	(*(volatile int *)0x48005000) |= 0x0003E800;
	(*(volatile int *)0x48005010) |= 0x0003E800;

	/* Make GPIO 7 as output pin */
	(*(volatile int *)0x48310034) &= ~(0x00000080);

	/* Now send a pulse on the GPIO pin */
	(*(volatile int *)0x4831003C) |= 0x00000080;
	for (i = 0; i < 99; i++) ;
	(*(volatile int *)0x4831003C) &= ~(0x00000080);
	mdelay(100);
	for (i = 0; i < 99; i++) ;
	(*(volatile int *)0x4831003C) |= 0x00000080;
	mdelay(100);

}

/***********************************************************/
/* Function to init the EEPROM 			                   */
/***********************************************************/
int eepromInit(void)
{
	unsigned long cfg;

	/* rese the LAN9220 */
	reset_smsc_lan();

	/* Enable EEPROM */
	cfg = __raw_readl(GPIO_CFG);
	__raw_writel((cfg & ENABLE_EEPROM), GPIO_CFG);

	return EEPROM_SUCCESS;
}

/***********************************************************/
/* Functions to read/write the LAN registers               */
/***********************************************************/
inline u32 e_reg_read(u32 addr)
{
	return *(volatile u32 *)addr;
}

/* write from LAN9220 */
void e_reg_write(u32 addr, u32 val)
{
	*(volatile u16 *)addr = (u16) val;
	*(volatile u16 *)(addr + 2) = (u16) (val >> 16);
}

/***********************************************************/
/* Function to read EEPROM registers 		               */
/***********************************************************/
int read_eeprom(u8 reg, int *val)
{
	volatile unsigned long cmd;
	volatile unsigned char data;
	int i, addr;

	/* check for status busy! */
	DELAY(10000);
	cmd = __raw_readl(E2P_CMD);
	DELAY(10000);
	cmd = __raw_readl(E2P_CMD);
	if (cmd & E2P_CMD_EPC_BUSY_) {
		printf("Error: Device busy\n");
		return EEPROM_FAIL;
	}
	DELAY(10000);

	/* Read signature byte */
	for (i = 0; i < 3; i++) {
		addr = 0;
		cmd = E2P_CMD_EPC_BUSY_ | E2P_CMD_EPC_CMD_READ_ | (addr & 0XFF);
		__raw_writel(cmd, E2P_CMD);
		while ((cmd & E2P_CMD_EPC_BUSY_) && !(cmd & E2P_CMD_EPC_TIMEOUT_)) {
			cmd = __raw_readl(E2P_CMD);
		}
		if (cmd & E2P_CMD_EPC_TIMEOUT_) {
			printf("Error! Device timed out while reading\n");
			return EEPROM_FAIL;
		}
		data = __raw_readl(E2P_DATA);
	}

	if (data != SIGNATURE_BYTE) {
		printf("Error: Signature byte not read\n");
		return EEPROM_FAIL;
	}

	/* Read Data byte */
	addr = reg;
	cmd = E2P_CMD_EPC_BUSY_ | E2P_CMD_EPC_CMD_READ_ | (addr & 0xFF);
	__raw_writel(cmd, E2P_CMD);
	while ((cmd & E2P_CMD_EPC_BUSY_) && !(cmd & E2P_CMD_EPC_TIMEOUT_)) {
		cmd = __raw_readl(E2P_CMD);
	}

	data = __raw_readl(E2P_DATA);
	DELAY(10000);
	*val = data;

	return EEPROM_SUCCESS;
}

/***********************************************************/
/* Function to write into EEPROM registers	               */
/***********************************************************/
int write_eeprom(u8 reg, u32 val)
{
	volatile unsigned long cmd;
	volatile unsigned char data;
	int i, addr;

	/* check if status busy! */
	cmd = __raw_readl(E2P_CMD);
	if (cmd & E2P_CMD_EPC_BUSY_) {
		printf("Error: Device busy\n");
		return EEPROM_FAIL;
	}
	DELAY(10000);

	/* set the signature byte */
	for (i = 0; i < 3; i++) {
		addr = 0;
		cmd = E2P_CMD_EPC_BUSY_ | E2P_CMD_EPC_CMD_READ_ | (addr & 0XFF);
		__raw_writel(cmd, E2P_CMD);
		while ((cmd & E2P_CMD_EPC_BUSY_) && !(cmd & E2P_CMD_EPC_TIMEOUT_)) {
			cmd = __raw_readl(E2P_CMD);
		}
		if (cmd & E2P_CMD_EPC_TIMEOUT_) {
			printf("Error! Device timed out while reading\n");
			return EEPROM_FAIL;
		}
		data = __raw_readl(E2P_DATA);
	}

	cmd = E2P_CMD_EPC_BUSY_ | E2P_CMD_EPC_CMD_EWEN_;
	__raw_writel(cmd, E2P_CMD);

	while ((cmd & E2P_CMD_EPC_BUSY_) && !(cmd & E2P_CMD_EPC_TIMEOUT_)) {
		cmd = __raw_readl(E2P_CMD);
	}
	if (cmd & E2P_CMD_EPC_TIMEOUT_) {
		printf("Error! Device timed out while Enabling write\n");
		return EEPROM_FAIL;
	}

	DELAY(10000);

	/* write data byte */
	data = SIGNATURE_BYTE;
	addr = 0;
	__raw_writel(data, E2P_DATA);
	cmd = E2P_CMD_EPC_CMD_WRITE_ | (addr & 0XFF);
	__raw_writel(cmd, E2P_CMD);
	__raw_writel((__raw_readl(E2P_CMD) | E2P_CMD_EPC_BUSY_), E2P_CMD);
	while ((cmd & E2P_CMD_EPC_BUSY_) && !(cmd & E2P_CMD_EPC_TIMEOUT_)) {
		cmd = __raw_readl(E2P_CMD);
	}
	if (cmd & E2P_CMD_EPC_TIMEOUT_) {
		printf("Error! Device timed out while reading\n");
		return EEPROM_FAIL;
	}
	DELAY(20000);
	data = val;
	addr = reg;
	__raw_writel(data, E2P_DATA);
	cmd = E2P_CMD_EPC_CMD_WRITE_ | (addr & 0XFF);
	__raw_writel(cmd, E2P_CMD);
	__raw_writel((__raw_readl(E2P_CMD) | E2P_CMD_EPC_BUSY_), E2P_CMD);
	while ((cmd & E2P_CMD_EPC_BUSY_) && !(cmd & E2P_CMD_EPC_TIMEOUT_)) {
		cmd = __raw_readl(E2P_CMD);
	}
	if (cmd & E2P_CMD_EPC_TIMEOUT_) {
		printf("Error! Device timed out while reading\n");
		return EEPROM_FAIL;
	}
	DELAY(20000);

	return EEPROM_SUCCESS;
}

/***********************************************************/
/* Function to convert ascii to integer values             */
/***********************************************************/
unsigned char atoint(unsigned char ch)
{
	if (isdigit(ch)) {
		return (ch - '0');
	}
	ch = toupper(ch);
	if (ch >= 'A' && ch <= 'F')
		return (10 + (ch - 'A'));
	return 0;
}

/***********************************************************/
/* Function to read or write from EEPROM device            */
/***********************************************************/
int read_write_eeprom(int flag, char *val, ulong off)
{
	eepromInit();

	if (flag == 0) {	/* read eerpom */
		/* read actual bytes from  eeprom */
		int value = 0;
		if (read_eeprom(off, &value) < 0) {
			printf("Error! Reading from EEPROM\n");
			return EEPROM_FAIL;
		} else {
			*val = value;
		}
		return EEPROM_SUCCESS;
	}

	if (flag == 1) {	/* write eeprom */
		/* write actual bytes into eeprom */
		if (write_eeprom(off, *val) < 0) {
			printf("Error! Writing to EEPROM\n");
			return EEPROM_FAIL;
		}
		return EEPROM_SUCCESS;
	}
	return EEPROM_FAIL;
}

/***********************************************************/
/* Function to find the string length			           */
/***********************************************************/
size_t str_len(const char *s)
{
	const char *sc;

	for (sc = s; *sc != '\0'; ++sc)
		/* nothing */ ;
	return sc - s;
}

/***********************************************************/
/* Function to read or write from EEPROM device            */
/***********************************************************/
int board_info_rw(cmd_tbl_t * cmdtp, int flag, int argc, char *argv[])
{
	int board_id[1024];
	int cnt = 0;
	char val = 0;
	int i = 0;
	ulong offset = EEPROM_REV_BYTE_OFFSET;
	unsigned char board_info[BOARD_INFO_STRLEN];

	if ((argc == 1) || ((argc == 2) && (strcmp(argv[1], "write")) == 0)) {
		printf("Usage:\n");
		printf("To read board information: board_info read\n");
		printf("To write board information: board_info write <string>\n");
		printf("Where <string> is <board revision ID:Board Serial Number>\n");
		printf("Example: board_info write REV-E:05:0027\n");
		return EEPROM_SUCCESS;
	}

	if (strcmp(argv[1], "read") == 0) {
		int value = 0;
		ulong length_offset = EEPROM_REV_LENGTH_OFFSET;
		/* find the lenght of bytes to be read */
		if (read_eeprom(length_offset, &value) < 0) {
			printf("Error! Reading from EEPROM\n");
			return EEPROM_FAIL;
		} else {
#ifdef EEPROM_DEBUG_PRINT
			if (isdigit(value))
				printf("Value read from eeprom is %d\n", (value - 48));
			else
				printf("Value read from eeprom is %c\n", value);
#endif
		}

		/* read the bytes */
		for (i = 0; i < value; i++) {
			if (read_write_eeprom(0, &val, offset + i) < 0) {
				printf("Error! Reading from EEPROM\n");
				return EEPROM_FAIL;
			}
			board_id[i] = val;
		}

		printf("Board Revision ID and Serial Number : ");
		for (i = 0; i < value; i++) {
			if (isdigit(board_id[i]))
				printf("%d", (board_id[i] - 48));
			else
				printf("%c", board_id[i]);
		}
		printf("\n");

	} else if (strcmp(argv[1], "write") == 0) {
		cnt = str_len(argv[2]);

		/* write the lenght at EEPROM_REV_LENGTH_OFFSET */
		ulong length_offset = EEPROM_REV_LENGTH_OFFSET;
		if (write_eeprom(length_offset, cnt) < 0) {
			printf("Error! Writing to EEPROM\n");
			return EEPROM_FAIL;
		}
		/* write the revision into eeprom */
		if (cnt >= BOARD_INFO_STRLEN) {
			printf("Error Board Info string: too large\n");
			return EEPROM_FAIL;
		}

		strncpy((char *)board_info, argv[2], cnt + 1);
#if EEPROM_DEBUG_PRINT
		printf("board_info %s\n", board_info);
#endif
		for (i = 0; i < cnt; i++) {
			val = board_info[i];
			if (read_write_eeprom(1, &val, offset + i) < 0) {
				printf("Error! Writing into EEPROM\n");
				return EEPROM_FAIL;
			}
		}
		printf("Board Revision ID and Serial Number Programmed successfully\n");
	}
	return EEPROM_SUCCESS;
}

U_BOOT_CMD(board_info, 3, 1, board_info_rw, "", "Usage: board_info read \n \
         board_info write <revision -id>:<serial number> \n \
         Example:  board_info write REV-E:05-0027 \n");
