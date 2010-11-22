/*
 * dg_ram.c: Memory diagnostic code.
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

/* Includes. */
#include <common.h>
#include "types.h"
#include <diag_core.h>
#include <dg_memory.h>
#include <omap3530evm.h>

#define TEST_START_ADDR         0x81800000
#define TEST_END_ADDR 	        0x87FFFFFF
#define TEST_DATA1		0x55
#define TEST_DATA2		0xAA
#define OK 	0
#define ERROR	-1
#define RAND32		        (u32) 0xa5a5a5a5
#define MAKE32FROM8(X)	(u32) (X | X << 8 | X << 16 | X << 24)

typedef unsigned int UINT32;
typedef unsigned char UINT8;
typedef unsigned short UINT16;

/* Datatypes. */
typedef struct teststruct_noarg {
	char *name;
	int (*fp) (u32 *, u32 *, u32);
} TEST_STRUCT_NO_ARG;

typedef struct teststruct_onearg {
	char *name;
	int (*fp) (u32 *, u32 *, u32, int);
	int arg;
} TEST_STRUCT_ONE_ARG;

/* Data. */
int silent_test;
u32 runerrors;
u32 totalerrors;

/* Function declarations. */
int testVerifySuccess(u32 * bp1, u32 * bp2, u32 u32Count);
int testRandomValue(u32 * bp1, u32 * bp2, u32 u32Count);
int testXorComparison(u32 * bp1, u32 * bp2, u32 u32Count);
int testSubComparison(u32 * bp1, u32 * bp2, u32 u32Count);
int testMulComparison(u32 * bp1, u32 * bp2, u32 u32Count);
int testDivComparison(u32 * bp1, u32 * bp2, u32 u32Count);
int testOrComparison(u32 * bp1, u32 * bp2, u32 u32Count);
int testAndComparison(u32 * bp1, u32 * bp2, u32 u32Count);
int testSeqincComparison(UINT32 * bp1, UINT32 * bp2, UINT32 u32Count);
int testCheckerboardComparison(UINT32 * bp1, UINT32 * bp2, UINT32 u32Count);
int testSolidbitsComparison(UINT32 * bp1, UINT32 * bp2, UINT32 u32Count);
int testBlockseqComparison(UINT32 * bp1, UINT32 * bp2, UINT32 u32Count);
int testWalkbitsComparison(UINT32 * bp1, UINT32 * bp2, UINT32 u32Count, int mode);
int testBitspreadComparison(UINT32 * bp1, UINT32 * bp2, UINT32 u32Count);
int testBitflipComparison(UINT32 * bp1, UINT32 * bp2, UINT32 u32Count);
int testStuckAddress(UINT32 * bp1, UINT32 * bp2, UINT32 u32Count);
void testTaskEntry(UINT32, UINT32, UINT32);
S32 test_sdram(REG_U8 *, u32);

/* Data. */
TEST_STRUCT_NO_ARG testsNoArg[] = {
	{"Block Sequential", testBlockseqComparison},
	{NULL, 0}
};

TEST_STRUCT_ONE_ARG testsOneArg[] = {
	{"Walking Ones", testWalkbitsComparison, 0},
	{"Walking Zeroes", testWalkbitsComparison, 1},
	{NULL, 0, 0}
};

void printTestName(char *testname, UINT8 * test)
{
	printf("  Test %2lu:  %20.20s:  ", (long unsigned int)*test, testname);
	(*test)++;
}

void sdram_test(void)
{
	REG_U8 *base_addr = (REG_U8 *) TEST_START_ADDR;
	REG_U8 *end_addr = (REG_U8 *) TEST_END_ADDR;
	u32 nBytes = ((u32) end_addr - (u32) base_addr);

	printf("SDRAM Start address 0x%x, End address 0x%x\n", (unsigned int)base_addr, (unsigned int)end_addr);
	test_sdram((REG_U8 *) base_addr, nBytes);
}

void testTaskEntry(UINT32 u32TaskMemBytes, UINT32 u32TaskStartAddress, UINT32 u32MaxIters)
{
	UINT32 u32MemSplit = u32TaskMemBytes / 2;
	UINT32 *pu32Chank1 = (UINT32 *) u32TaskStartAddress;
	UINT32 *pu32Chank2 = (UINT32 *) ((UINT32) u32TaskStartAddress + u32MemSplit);
	UINT32 u32Count = u32MemSplit / sizeof(UINT32);
	UINT32 u32Iter = 0;
	UINT8 u8TestCount, i, rc, *u8Test = &u8TestCount;

	for (u32Iter = 0, *u8Test = 1; ++u32Iter, u32Iter <= u32MaxIters; *u8Test = 1) {
		/* Do basic tests (taking two pointers plus count) here. */
		for (i = 0;; i++) {
			if (testsNoArg[i].name != NULL) {
				printTestName(testsNoArg[i].name, u8Test);
				rc = testsNoArg[i].fp(pu32Chank1, pu32Chank2, u32Count);
				if (!rc)
					printf("Passed.\n");
				else
					runerrors++;
			} else
				break;
		}

		printf("Run %4lu completed " " (%lu tests showed errors).\n", (long unsigned int)u32Iter, (long unsigned int)runerrors);
		totalerrors += runerrors;
		runerrors = 0;
	}
}

/* Function definitions. */
int testVerifySuccess(UINT32 * bp1, UINT32 * bp2, UINT32 u32Count)
{
	volatile UINT32 *p1 = (volatile UINT32 *)bp1;
	volatile UINT32 *p2 = (volatile UINT32 *)bp2;
	UINT32 i;

	for (i = 0; i < u32Count; i++, p1++, p2++) {
		if (*p1 != *p2) {
			printf("\nFAILURE: 0x%08lx != 0x%08lx at offset 0x%08lx.\n", (long unsigned int)*p1, (long unsigned int)*p2, (long unsigned int)i);
			printf("Skipping to next test...\n");
			return (ERROR);
		}
	}
	return (OK);
}

int testRandomValue(UINT32 * bp1, UINT32 * bp2, UINT32 u32Count)
{
	volatile UINT32 *p1 = (volatile UINT32 *)bp1;
	volatile UINT32 *p2 = (volatile UINT32 *)bp2;
	UINT32 i;

	silent_test = 0;
	for (i = 0; i < u32Count; i++) {
		*p1++ = *p2++ = RAND32;
	}
	return (testVerifySuccess(bp1, bp2, u32Count));
}

int testXorComparison(UINT32 * bp1, UINT32 * bp2, UINT32 u32Count)
{
	volatile UINT32 *p1 = (volatile UINT32 *)bp1;
	volatile UINT32 *p2 = (volatile UINT32 *)bp2;
	UINT32 i;
	UINT32 q = RAND32;

	silent_test = 0;
	for (i = 0; i < u32Count; i++) {
		*p1++ ^= q;
		*p2++ ^= q;
	}
	return (testVerifySuccess(bp1, bp2, u32Count));
}

int testSubComparison(UINT32 * bp1, UINT32 * bp2, UINT32 u32Count)
{
	volatile UINT32 *p1 = (volatile UINT32 *)bp1;
	volatile UINT32 *p2 = (volatile UINT32 *)bp2;
	UINT32 i;
	UINT32 q = RAND32;

	silent_test = 0;
	for (i = 0; i < u32Count; i++) {
		*p1++ -= q;
		*p2++ -= q;
	}
	return (testVerifySuccess(bp1, bp2, u32Count));
}

int testMulComparison(UINT32 * bp1, UINT32 * bp2, UINT32 u32Count)
{
	volatile UINT32 *p1 = (volatile UINT32 *)bp1;
	volatile UINT32 *p2 = (volatile UINT32 *)bp2;
	UINT32 i;
	UINT32 q = RAND32;

	silent_test = 0;
	for (i = 0; i < u32Count; i++) {
		*p1++ *= q;
		*p2++ *= q;
	}
	return (testVerifySuccess(bp1, bp2, u32Count));
}

int testDivComparison(UINT32 * bp1, UINT32 * bp2, UINT32 u32Count)
{
	volatile UINT32 *p1 = (volatile UINT32 *)bp1;
	volatile UINT32 *p2 = (volatile UINT32 *)bp2;
	UINT32 i;
	UINT32 q = RAND32;

	silent_test = 0;
	for (i = 0; i < u32Count; i++) {
		if (!q)
			q++;
		*p1++ /= q;
		*p2++ /= q;
	}
	return (testVerifySuccess(bp1, bp2, u32Count));
}

int testOrComparison(UINT32 * bp1, UINT32 * bp2, UINT32 u32Count)
{
	volatile UINT32 *p1 = (volatile UINT32 *)bp1;
	volatile UINT32 *p2 = (volatile UINT32 *)bp2;
	UINT32 i;
	UINT32 q = RAND32;

	silent_test = 0;
	for (i = 0; i < u32Count; i++) {
		*p1++ |= q;
		*p2++ |= q;
	}
	return (testVerifySuccess(bp1, bp2, u32Count));
}

int testAndComparison(UINT32 * bp1, UINT32 * bp2, UINT32 u32Count)
{
	volatile UINT32 *p1 = (volatile UINT32 *)bp1;
	volatile UINT32 *p2 = (volatile UINT32 *)bp2;
	UINT32 i;
	UINT32 q = RAND32;

	silent_test = 0;
	for (i = 0; i < u32Count; i++) {
		*p1++ &= q;
		*p2++ &= q;
	}
	return (testVerifySuccess(bp1, bp2, u32Count));
}

int testSeqincComparison(UINT32 * bp1, UINT32 * bp2, UINT32 u32Count)
{
	volatile UINT32 *p1 = (volatile UINT32 *)bp1;
	volatile UINT32 *p2 = (volatile UINT32 *)bp2;
	UINT32 i;
	UINT32 q = RAND32;

	silent_test = 0;
	for (i = 0; i < u32Count; i++) {
		*p1++ = *p2++ = (i + q);
	}
	return (testVerifySuccess(bp1, bp2, u32Count));
}

int testSolidbitsComparison(UINT32 * bp1, UINT32 * bp2, UINT32 u32Count)
{
	volatile UINT32 *p1 = (volatile UINT32 *)bp1;
	volatile UINT32 *p2 = (volatile UINT32 *)bp2;
	UINT32 q, i, j;

	silent_test = 1;
	printf("    ");
	for (j = 0; j < 2; j++) {
		q = (j % 2) == 0 ? 0xFFFFFFFF : 0x00000000;
		p1 = (volatile UINT32 *)bp1;
		p2 = (volatile UINT32 *)bp2;
		for (i = 0; i < u32Count; i++) {
			*p1++ = *p2++ = (i % 2) == 0 ? q : ~q;
		}
		if (testVerifySuccess(bp1, bp2, u32Count) == ERROR) {
			return (ERROR);
		}
	}
	return (OK);
}

int testCheckerboardComparison(UINT32 * bp1, UINT32 * bp2, UINT32 u32Count)
{
	volatile UINT32 *p1 = (volatile UINT32 *)bp1;
	volatile UINT32 *p2 = (volatile UINT32 *)bp2;
	UINT32 q, i, j;

	silent_test = 1;
	printf("    ");
	for (j = 0; j < 2; j++) {
		q = (j % 2) == 0 ? 0x55555555 : 0xAAAAAAAA;
		p1 = (volatile UINT32 *)bp1;
		p2 = (volatile UINT32 *)bp2;
		for (i = 0; i < u32Count; i++) {
			*p1++ = *p2++ = (i % 2) == 0 ? q : ~q;
		}
		if (testVerifySuccess(bp1, bp2, u32Count) == ERROR) {
			return (ERROR);
		}
	}
	return (OK);
}

int testBlockseqComparison(UINT32 * bp1, UINT32 * bp2, UINT32 u32Count)
{
	volatile UINT32 *p1 = (volatile UINT32 *)bp1;
	volatile UINT32 *p2 = (volatile UINT32 *)bp2;
	UINT32 i, j;

	silent_test = 1;
	printf("    ");
	for (j = 0; j < 256; j++) {
		p1 = (volatile UINT32 *)bp1;
		p2 = (volatile UINT32 *)bp2;
		for (i = 0; i < u32Count; i++) {
			*p1++ = *p2++ = MAKE32FROM8(j);
		}
		if (testVerifySuccess(bp1, bp2, u32Count) == ERROR) {
			return (ERROR);
		}
	}
	return (OK);
}

int testWalkbitsComparison(UINT32 * bp1, UINT32 * bp2, UINT32 u32Count, int m)
{
	volatile UINT32 *p1 = (volatile UINT32 *)bp1;
	volatile UINT32 *p2 = (volatile UINT32 *)bp2;
	UINT32 i, j;

	silent_test = 1;
	printf("    ");
	for (j = 0; j < 32; j++) {
		p1 = (volatile UINT32 *)bp1;
		p2 = (volatile UINT32 *)bp2;
		for (i = 0; i < u32Count; i++) {
			/* Walk it up. */

			*p1++ = *p2++ = (i % 2 == 0) ? 0x00000001 << j : 0xFFFFFFFF ^ (0x00000001 << j);
		}
		if (testVerifySuccess(bp1, bp2, u32Count) == ERROR) {
			return (ERROR);
		}
	}
	return (OK);
}

int testBitspreadComparison(UINT32 * bp1, UINT32 * bp2, UINT32 u32Count)
{
	volatile UINT32 *p1 = (volatile UINT32 *)bp1;
	volatile UINT32 *p2 = (volatile UINT32 *)bp2;
	UINT32 i, j;

	silent_test = 1;
	printf("    ");
	for (j = 0; j < 32; j++) {
		p1 = (volatile UINT32 *)bp1;
		p2 = (volatile UINT32 *)bp2;
		for (i = 0; i < u32Count; i++) {
			/* Walk it back down. */
			*p1++ = *p2++ = (i % 2 == 0) ? (0x00000001 << (31 - j)) | (0x00000001 << (33 - j)) : 0xFFFFFFFF ^ (0x00000001 << (31 - j) | (0x00000001 << (33 - j)));
		}
		if (testVerifySuccess(bp1, bp2, u32Count) == ERROR) {
			return (ERROR);
		}
	}
	return (OK);
}

int testBitflipComparison(UINT32 * bp1, UINT32 * bp2, UINT32 u32Count)
{
	volatile UINT32 *p1 = (volatile UINT32 *)bp1;
	volatile UINT32 *p2 = (volatile UINT32 *)bp2;
	UINT32 i, k;
	UINT32 q;

	silent_test = 1;
	printf("    ");
	for (k = 0; k < 32; k++) {
		q = 0x00000001 << k;

		q = ~q;
		p1 = (volatile UINT32 *)bp1;
		p2 = (volatile UINT32 *)bp2;
		for (i = 0; i < u32Count; i++) {
			*p1++ = *p2++ = (i % 2) == 0 ? q : ~q;
		}

		if (testVerifySuccess(bp1, bp2, u32Count) == ERROR) {
			return (ERROR);
		}
	}
	return (OK);
}

int testStuckAddress(UINT32 * bp1, UINT32 * unused, UINT32 u32Count)
{
	volatile UINT32 *p1;
	/* second argument is not used; just gives it a compatible signature. */
	UINT32 i;

	u32Count <<= 1;
	p1 = (volatile UINT32 *)bp1;
	for (i = 0; i < u32Count; i++) {
		*p1 = ((i % 2) == 0) ? *p1 : ~(*p1);
		p1++;
	}
	p1 = (volatile UINT32 *)bp1;
	for (i = 0; i < u32Count; i++, p1++) {
		if (*p1 != ((i % 2) == 0 ? (UINT32) p1 : ~((UINT32) p1))) {
			printf("\nFAILURE: possible bad address line at offset 0x%08lx.\n", (long unsigned int)i);

			printf("Skipping to next test...\n");
			return (ERROR);
		}
	}
	return (OK);
}

S32 test_sdram(REG_U8 * baseAddress, u32 nBytes)
{
	u8 pattern;
	u8 antipattern;
	u32 addressMask;
	u32 nWords;
	u32 offset;
	u32 testOffset;
	status_t test_status = SUCCESS;
	status_t p_test_status = SUCCESS;

	/*
	 ** DATA BUS TEST.
	 **
	 ** Test the data bus wiring in a memory region by
	 ** performing a walking 1's test at a fixed address
	 ** within that region.  The address (and hence the
	 ** memory region) is selected by the caller.
	 */

	printf("Running data bus test...");
	/* Perform a walking 1's test at the given address. */
	for (pattern = 1; pattern != 0; pattern <<= 1) {
		/* Write the test pattern. */
		*baseAddress = pattern;

		/* Read it back (immediately is okay for this test). */
		if (*baseAddress != pattern) {
			printf("SDRAM - Data Bus Test failed for data pattern 0x%x\n", pattern);
			printf("SDRAM - Walking 1's test failed.\r\n");
			p_test_status = test_status = FAILURE;
		}
	}
	if (FAILURE != p_test_status) {
		printf("PASSED\n");
	} else
		printf("FAILED\n");

	p_test_status = SUCCESS;

	/*
	 ** ADDRESS BUS TEST.
	 **
	 ** Test the address bus wiring in a memory region by
	 ** performing a walking 1's test on the relevant bits
	 ** of the address and checking for aliasing.  The test
	 ** will find single-bit address failures such as stuck
	 ** -high, stuck-low, and shorted pins.  The base address
	 ** and size of the region are selected by the caller.
	 **
	 ** Notes:
	 **      For best results, the selected base address should
	 **      have enough LSB 0's to guarantee single address bit
	 **      changes.  For example, to test a 64 KB region, select
	 **      a base address on a 64 KB boundary.  Also, select the
	 **      region size as a power-of-two--if at all possible.
	 **
	 **      A nonzero result is the first address at which an
	 **      aliasing problem was uncovered.  By examining the
	 **      contents of memory, it may be possible to gather
	 **      additional information about the problem.
	 */

	addressMask = (nBytes - 1);
	pattern = (REG_U8) 0xAAAAAAAA;
	antipattern = (REG_U8) 0x55555555;

	printf("Running address bus test...");
	/* Write the default pattern at each of the power-of-two offsets. */
	for (offset = sizeof(REG_U8); (offset & addressMask) != 0; offset <<= 1) {

		baseAddress[offset] = pattern;
	}

	/* Check for address bits stuck high. */
	testOffset = 0;
	baseAddress[testOffset] = antipattern;

	for (offset = sizeof(REG_U8); (offset & addressMask) != 0; offset <<= 1) {
		if (baseAddress[offset] != pattern) {
			printf("SDRAM - Address Bus Test failed at 0x%x\n", offset);
			printf("SDRAM - Check for address bits stuck high failed.\r\n");
			p_test_status = test_status = FAILURE;
		}
	}

	baseAddress[testOffset] = pattern;

	/* Check for address bits stuck low or shorted. */
	for (testOffset = sizeof(REG_U8); (testOffset & addressMask) != 0; testOffset <<= 1) {
		baseAddress[testOffset] = antipattern;
		for (offset = sizeof(REG_U8); (offset & addressMask) != 0; offset <<= 1) {
			if ((baseAddress[offset] != pattern) && (offset != testOffset)) {
				printf("SDRAM - Address Bus Test failed at 0x%x\n", offset);
				printf("SDRAM - Check for address bits stuck low or shorted failed.\r\n");
				p_test_status = test_status = FAILURE;
			}
		}

		baseAddress[testOffset] = pattern;
	}
	if (p_test_status != FAILURE) {
		printf("PASSED\n");
	} else
		printf("FAILED\n");

	p_test_status = SUCCESS;

	/*
	 ** DEVICE TEST.
	 **
	 ** Test the integrity of a physical memory device by
	 ** performing an increment/decrement test over the
	 ** entire region.  In the process every storage bit
	 ** in the device is tested as a zero and a one.  The
	 ** base address and the size of the region are
	 ** selected by the caller.
	 **
	 ** Notes:
	 **      If the test succeeds the entire memory region
	 **      will be filled with zeros.
	 **      A nonzero result is the first address at which an
	 **      incorrect value was read back.  By examining the
	 **      contents of memory, it may be possible to gather
	 **      additional information about the problem.
	 */

	nWords = (nBytes / sizeof(REG_U8));

	printf("Running memory Read/Write test... Please wait... ");
	/* Fill memory with a known pattern. */
	for (pattern = 1, offset = 0; offset < nWords; pattern++, offset++) {
		baseAddress[offset] = pattern;
	}

	/* Check each location and invert it for the second pass. */
	for (pattern = 1, offset = 0; offset < nWords; pattern++, offset++) {
		if (baseAddress[offset] != pattern) {
			printf("SDRAM - Read?Write Test failed at 0x%x.\r\n", offset);
			printf("SDRAM - Check for pattern in each location failed.\r\n");
			p_test_status = test_status = FAILURE;
		}
		antipattern = ~pattern;
		baseAddress[offset] = antipattern;
	}

	/* Check each location for the inverted pattern and zero it. */
	for (pattern = 1, offset = 0; offset < nWords; pattern++, offset++) {
		antipattern = ~pattern;
		if (baseAddress[offset] != antipattern) {
			printf("SDRAM - Read?Write Test failed at 0x%x\n", offset);
			printf("SDRAM - Check for inverted pattern in each location failed.\r\n");
			p_test_status = test_status = FAILURE;
		}

		baseAddress[offset] = 0;
	}
	if (FAILURE != p_test_status) {
		printf("PASSED\n");
	} else
		printf("FAILED\n");

	printf("SDRAM Test Completed... %s\n", ((FAILURE != test_status) ? "PASS" : "FAIL"));

	return test_status;
}
