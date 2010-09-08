/*
 * dg_flash_test.c: NAND flash diagnostic code.
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

#include <config.h>
#include <common.h>
#include <command.h>
#include <linux/types.h>

#include <linux/mtd/mtd.h>
#include <linux/mtd/nand.h>
#include <nand.h>

#define NAND_PROGMABLE_AREA 	0x780000
#define MAX_BAD_BLK_LIMIT   	40

#define FLASH_READ_BUF_ADDR		0x84000000
#define FLASH_WRITE_BUF_ADDR	0x84080000

extern nand_info_t nand_info[];
static int check_nand_bad_block(nand_info_t * nand);

int flash_test(void)
{
	struct nand_chip *nand_chip = nand_info[0].priv;
	nand_info_t *nand;
	int count;

	int ret = 0;
/*********************** ERASE - start ***************************************/
	struct erase_info instr;
	ulong start;
	ulong end = 0x8000000;
	ulong ofs = 0;
	ulong block;
	ulong off;
	size_t size;
	int i = 0;
	int test_passed = 1;
	int p_test_passed = 1;
	u_char *read_buff = (u_char *) FLASH_READ_BUF_ADDR;
	u_char *write_buff = (u_char *) FLASH_WRITE_BUF_ADDR;

	nand = &nand_info[nand_curr_device];

	/* Added to check nand bad block */
	ret = check_nand_bad_block(nand);
#ifdef CONFIG_OMAP3_EVM
	if (ret != 0) {
		printf("FAILED\n");
		return -1;
	}
#endif
	for (count = 0; count < 3; count++) {
		start = 283;
		end = 345;
		if (count == 1) {
			start = 1103;
			end = 1165;
		} else if (count == 2) {
			start = 1922;
			end = 1984;
		}

		if (!end || end < 0)
			end = start;

		printf("Running Erase Test...Please wait...");
		printf("Start Block : %d, End Block : %d\n", (int)start, (int)end);

		for (i = 0; i < 0x20000; i++) {
			write_buff[i] = (unsigned char)i;
		}

		for (block = start; block <= end; ofs++, block++) {
			instr.addr = block << nand_chip->phys_erase_shift;
			instr.len = 1 << nand_chip->phys_erase_shift;
			off = instr.addr;
			size = instr.len;
			ret = nand_erase(nand, off, size);
			
				if (ret) {
					if (!nand_block_isbad(nand, off)) {
					printf("erase failed at block %d\n", (int)block);
					p_test_passed = test_passed = 0;
				}
				continue;
			}
		}
/*********************** ERASE - end ***************************************/
		if (p_test_passed) {
			printf("PASSED\n");
		} else
			printf("FAILED\n");

		p_test_passed = 1;
		printf("Running Write test...Please wait...");
		printf("Start Block : %d, End Block : %d\n", (int)start, (int)end);
		for (block = start; block <= end; ofs++, block++) {

			instr.addr = block << nand_chip->phys_erase_shift;
			instr.len = 1 << nand_chip->phys_erase_shift;
			off = instr.addr;
			size = instr.len;
			ret = nand_write(nand, off, &size, (u_char *) write_buff);

			if (ret) {
				printf("Error writing to NAND: ret = %d\n", ret);
				p_test_passed = test_passed = 0;
				continue;
			}
		}

		if (p_test_passed) {
			printf("PASSED\n");
		} else
			printf("FAILED\n");

		p_test_passed = 1;
		printf("Running Read test...Please wait...");
		printf("Start Block : %d, End Block : %d\n", (int)start, (int)end);

		for (block = start; block <= end; ofs++, block++) {

			instr.addr = block << nand_chip->phys_erase_shift;
			instr.len = 1 << nand_chip->phys_erase_shift;
			off = instr.addr;
			size = instr.len;
			nand_read(nand, off, &size, (u_char *) read_buff);

			for (i = 0; i < 0x20000; i++) {
				if (read_buff[i] != (unsigned char)i) {
					if (!nand_block_isbad(nand, off)) {
						printf("write-read failed at offset 0x%x (0x%x)\n", (unsigned int)(block * 0x20000 + i), read_buff[i]);
						p_test_passed = test_passed = 0;
					}
					break;
				}
			}
		}

		if (p_test_passed) {
			printf("PASSED\n");
		} else {
			printf("FAILED\n");
		}
	}

/*********************** ERASE - start ***************************************/
	printf("Erase nand after completing the Nand write read test... \n");
	{
		struct erase_info instr;
		ulong start;
		ulong end = 0x8000000;
		ulong ofs = 0;
		ulong block;
		ulong off, size;
		int i = 0;
		u_char *write_buff = (u_char *) FLASH_WRITE_BUF_ADDR;

		nand = &nand_info[nand_curr_device];

		for (count = 0; count < 3; count++) {
			start = 283;
			end = 345;
			if (count == 1) {
				start = 1103;
				end = 1165;
			} else if (count == 2) {
				start = 1922;
				end = 1984;
			}

			if (!end || end < 0)
				end = start;

			printf("Running Erase Test...Please wait...");
			printf("Start Block : %d, End Block : %d\n", (int)start, (int)end);

			for (i = 0; i < 0x20000; i++) {
				write_buff[i] = (unsigned char)i;
			}
			for (block = start; block <= end; ofs++, block++) {
				instr.addr = block << nand_chip->phys_erase_shift;
				instr.len = 1 << nand_chip->phys_erase_shift;
				off = instr.addr;
				size = instr.len;
				ret = nand_erase(nand, off, size);
				if (ret) {
					if (!nand_block_isbad(nand, off)) {
						printf("erase failed at block %d\n", (int)block);
						p_test_passed = test_passed = 0;
					}
					continue;
				}
			}
		}
	}
/*********************** ERASE - end ***************************************/
	printf("Nand Test Completed... %s\n", ((test_passed) ? "PASS" : "FAIL"));
	return 0;
}

static int check_nand_bad_block(nand_info_t * nand)
{
	unsigned int off;
	unsigned int bad_blk_cnt = 0;
	for (off = 0; off < nand->size; off += nand->erasesize) {
		if (nand_block_isbad(nand, off)) {
			/*printf("  %08lx\n", (long unsigned int)off);*/
			printf("Bad block found at :  %08lx\n", (long unsigned int)off);
			bad_blk_cnt++;

			if (off <= NAND_PROGMABLE_AREA) {
				printf("ERROR: NAND bad block exists in programmable area (offset %08lx)\n", (long unsigned int)off);
				return -1;
			}
		}

		if (bad_blk_cnt >= MAX_BAD_BLK_LIMIT) {
			printf("ERROR: Bad NAND Block Count exceeds MAX_BAD_BLK_LIMIT (%d)\n", MAX_BAD_BLK_LIMIT);
			return -1;
		}
	}
	printf("Bad NAND Block Count %d\n", bad_blk_cnt);
	return 0;
}
