/*
 * mac_evm.c: MAC EEPROM R/W functions.
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
#include <asm/arch/mux.h>
#include <linux/ctype.h>
#include <omap3530evm.h>
#include "../../common/inc/dg_gpio.h"

#define ENABLE_EEPROM 0xFF8FFFFF
#undef DEBUG_EEPROM

#define BIT(x)	(1<<x)

#define	SIGNATURE_BYTE 0xA5
#undef DEBUG_MAC_STORAGE

/* LAN register offsets and bit definitions */
#define GPIO_CFG					(CONFIG_SMC911X_BASE + 0x88)
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

#define E2P_CMD						(CONFIG_SMC911X_BASE + 0xB0)
#define	E2P_CMD_EPC_BUSY			0x80000000	/* Self Clearing */
#define	E2P_CMD_EPC_CMD				0x70000000	/* R/W */
#define	E2P_CMD_EPC_CMD_READ		0x00000000	/* R/W */
#define	E2P_CMD_EPC_CMD_EWDS		0x10000000	/* R/W */
#define	E2P_CMD_EPC_CMD_EWEN		0x20000000	/* R/W */
#define	E2P_CMD_EPC_CMD_WRITE		0x30000000	/* R/W */
#define	E2P_CMD_EPC_CMD_WRAL		0x40000000	/* R/W */
#define	E2P_CMD_EPC_CMD_ERASE		0x50000000	/* R/W */
#define	E2P_CMD_EPC_CMD_ERAL		0x60000000	/* R/W */
#define	E2P_CMD_EPC_CMD_RELOAD		0x70000000	/* R/W */
#define	E2P_CMD_EPC_TIMEOUT			0x00000200	/* RO */
#define	E2P_CMD_MAC_ADDR_LOADED		0x00000100	/* RO */
#define	E2P_CMD_EPC_ADDR			0x000000FF	/* R/W */

#define E2P_DATA					(CONFIG_SMC911X_BASE + 0xB4)
#define	E2P_DATA_EEPROM_DATA		0x000000FF	/* R/W */
/* end of LAN register offsets and bit definitions */

#define	ID_REV_ID_MASK				(0xFFFF0000)
#define	ID_REV_CHIP_118				(0x01180000)
#define	ID_REV_CHIP_115				(0x01150000)
#define	ID_REV_CHIP_218				(0x118A0000)
#define	ID_REV_REV_MASK				(0x0000FFFF)
#define ID_REV						(CONFIG_SMC911X_BASE + 0x50)
#define	ID_REV_CHIP_ID				0xFFFF0000	/* RO */
#define	ID_REV_REV_ID				0x0000FFFF	/* RO */

#define BYTE_TEST					(volatile unsigned long *)(CONFIG_SMC911X_BASE + 0x64)
#define	DELAY(n)	( {   \
						int _i = n; \
						do { \
						ulong _temp; \
						_temp = *BYTE_TEST; \
						} while (--_i); \
					} )

int smsc_eeprom_init(void);
int eeprom_read_mac(unsigned char *mac_addr)
{
	volatile unsigned long cmd;
	volatile unsigned char data;
	int i, addr;

	smsc_eeprom_init();
	DELAY(10000);
	cmd = __raw_readl(E2P_CMD);
	DELAY(10000);
	cmd = __raw_readl(E2P_CMD);
	if (cmd & E2P_CMD_EPC_BUSY_) {
		printf("Error: Device busy\n");
		return 1;
	}
#ifdef DEBUG_EEPROM
	printf("EEPROM contents are : \n");
#endif
	printf(" ");
	DELAY(10000);

	for (i = 0; i < 3; i++) {
		addr = 0;
		cmd = E2P_CMD_EPC_BUSY_ | E2P_CMD_EPC_CMD_READ_ | (addr & 0XFF);
		__raw_writel(cmd, E2P_CMD);
		while ((cmd & E2P_CMD_EPC_BUSY_) && !(cmd & E2P_CMD_EPC_TIMEOUT_)) {
			cmd = __raw_readl(E2P_CMD);
		}
		if (cmd & E2P_CMD_EPC_TIMEOUT_) {
			printf("Error! Device timed out while reading\n");
			return 1;
		}
		data = __raw_readl(E2P_DATA);
	}
	if (data != SIGNATURE_BYTE) {
		printf("Error: Signature byte not read\n");
		return 1;
	}

	for (i = 1; i <= 6; i++) {
		addr = i;
		cmd = E2P_CMD_EPC_BUSY_ | E2P_CMD_EPC_CMD_READ_ | (addr & 0xFF);
		__raw_writel(cmd, E2P_CMD);
		while ((cmd & E2P_CMD_EPC_BUSY_) && !(cmd & E2P_CMD_EPC_TIMEOUT_)) {
			cmd = __raw_readl(E2P_CMD);
		}
		data = __raw_readl(E2P_DATA);
#ifdef DEBUG_EEPROM
		printf("Data [%d] = %02X\n", i, data);
#endif
		DELAY(10000);
		mac_addr[i - 1] = data;
	}
	return 0;
}

int eeprom_write_mac(unsigned char *mac_addr)
{
	volatile unsigned long cmd;
	volatile unsigned char data;
	int i, addr;

	smsc_eeprom_init();

	cmd = __raw_readl(E2P_CMD);
	if (cmd & E2P_CMD_EPC_BUSY_) {
		printf("Error: Device busy\n");
		return 1;
	}
#ifdef DEBUG_EEPROM
	printf("EEPROM contents are : \n");
#endif
	printf(" ");
	DELAY(10000);

	for (i = 0; i < 3; i++) {
		addr = 0;
		cmd = E2P_CMD_EPC_BUSY_ | E2P_CMD_EPC_CMD_READ_ | (addr & 0XFF);
		__raw_writel(cmd, E2P_CMD);
		while ((cmd & E2P_CMD_EPC_BUSY_) && !(cmd & E2P_CMD_EPC_TIMEOUT_)) {
			cmd = __raw_readl(E2P_CMD);
		}
		if (cmd & E2P_CMD_EPC_TIMEOUT_) {
			printf("Error! Device timed out while reading\n");
			return 1;
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
		return 1;
	}

	DELAY(10000);
	for (i = 0; i <= 6; i++) {
		addr = i;
		if (addr == 0) {
			data = SIGNATURE_BYTE;
		} else {
			data = mac_addr[i - 1];
		}
		__raw_writel(data, E2P_DATA);
		cmd = E2P_CMD_EPC_CMD_WRITE_ | (addr & 0XFF);
		__raw_writel(cmd, E2P_CMD);
		__raw_writel((__raw_readl(E2P_CMD) | E2P_CMD_EPC_BUSY_), E2P_CMD);
		while ((cmd & E2P_CMD_EPC_BUSY_) && !(cmd & E2P_CMD_EPC_TIMEOUT_)) {
			cmd = __raw_readl(E2P_CMD);
		}
		if (cmd & E2P_CMD_EPC_TIMEOUT_) {
			printf("Error! Device timed out while reading\n");
			return 1;
		}
#ifdef DEBUG_EEPROM
		printf("Addr = %d Data [%d] = %02X\n", addr, i, data);
#endif
		DELAY(20000);
	}
	return 0;

}

#define mdelay(n)       udelay((n)*1000)
void reset_smscChip(void)
{
	int i = 0;
	/* do a hard reset */
	//GPIO 7 configuration in CONTROL_PADCONF_SYS_BOOT5 register mux mode is 4.
	MUX_VAL(CP(SYS_BOOT5), (IEN | PTU | EN | M4))
	    // Enable Clock for GPIO 1-6 module in CM_FCLKEN_PER and CM_ICLKEN_PER registers
	    writel(readl(CM_FCLKEN_PER) | 0x0003E800, CM_FCLKEN_PER);
	writel(readl(CM_ICLKEN_PER) | 0x0003E800, CM_ICLKEN_PER);

	// Make GPIO 7 as output pin 
	writel(readl(OMAP34XX_GPIO1_BASE + GPIO_OE) & ~BIT(7), OMAP34XX_GPIO3_BASE + GPIO_OE);

	// Now send a pulse on the GPIO pin
	writel(readl(OMAP34XX_GPIO1_BASE + GPIO_DATAOUT) | BIT(7), OMAP34XX_GPIO3_BASE + GPIO_DATAOUT);
	for (i = 0; i < 99; i++) ;
	writel(readl(OMAP34XX_GPIO1_BASE + GPIO_DATAOUT) & ~BIT(7), OMAP34XX_GPIO3_BASE + GPIO_DATAOUT);
	mdelay(100);
	for (i = 0; i < 99; i++) ;
	writel(readl(OMAP34XX_GPIO1_BASE + GPIO_DATAOUT) | BIT(7), OMAP34XX_GPIO3_BASE + GPIO_DATAOUT);
	mdelay(100);

}

int smsc_eeprom_init(void)
{
	unsigned long cfg;
	reset_smscChip();

	cfg = __raw_readl(GPIO_CFG);
	__raw_writel((cfg & ENABLE_EEPROM), GPIO_CFG);
	return 0;
}

unsigned char atoh(unsigned char ch)
{
	if (isdigit(ch)) {
		return (ch - '0');
	}
	ch = toupper(ch);
	if (ch >= 'A' && ch <= 'F')
		return (10 + (ch - 'A'));
	return 0;
}

int eeprom_sscanf(char *str, unsigned char *mac_addr)
{
	int count = 0;
	unsigned char digit1, digit2;
	int i;

	while (isspace(*str)) {
		str++;
	}

	for (i = 0; i < 6; i++) {
		digit1 = *str++;
		digit2 = *str++;
		if (isxdigit(digit1) && isxdigit(digit2)) {
			count++;
			mac_addr[i] = (atoh(digit1) << 4) | atoh(digit2);
#ifdef DEBUG_EEPROM
			printf("Digit1 = %c Digit 2= %c\n", digit1, digit2);
			printf("MAC_ADDR[%d] = %x\n", i, mac_addr[i]);
#endif
		}
		if (*str++ != ':' && i <= 4)
			break;
	}
	return count;
}

int mac_opr(cmd_tbl_t * cmdtp, int flag, int argc, char *argv[])
{
	unsigned char mac_addr[6];
	int cnt;

	if (argc == 1) {
		printf("%s", cmdtp->help);
		return 0;
	}

	if ((strcmp(argv[1], "read") == 0) && (argc == 2)) {
		if (eeprom_read_mac(mac_addr) == 0) {
			printf("MAC-ADDR : ");
			printf("%02X:%02X:%02X:%02X:%02X:%02X\n", mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
		} else {
			printf("Error! Reading MAC-ADDR from EEPROM\n");
		}
		return 0;
	}

	if ((strcmp(argv[1], "write") == 0) && (argc == 3)) {
		cnt = eeprom_sscanf(argv[2], mac_addr);
		if (cnt < 6) {
			printf("Error! Invalid MAC-ADDR entered\n");
			return 1;
		}
		if (eeprom_write_mac(mac_addr) == 0) {
			printf("MAC-ADDR Programmed successfully\n");
		} else {
			printf("Error! Writing MAC-ADDR to EEPROM\n");
		}
		return 0;
	}
	printf("%s", cmdtp->help);
	return 1;
}

U_BOOT_CMD(mac, 3, 1, mac_opr, "", "Usage: mac [read|write] <MAC-ADDR>\n \
         Reads/Writes MAC-ADDR to/from EEPROM \n \
         Example: mac write 11:22:33:44:55:66 stores 11:22:33:44:55:66 as New MAC\n");
