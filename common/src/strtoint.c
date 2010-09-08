/*
 * strtoint.c: String to integer functions.
 *
 * (C) Copyright 2010
 * Mistral Solutions Private Limited, <www.mistralsolutions.com>
 *
 * Author :
 *	Ganeshan <nganeshan@mistralsolutions.com>
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

#define DEBUG 0

static unsigned int power(int radix);
int strToHex(char *arg, unsigned int *result);

int strToInt(char *argv, unsigned int *result)
{
	unsigned int hexNum = 0;
	int radix;
	int status = 0;
	unsigned int hexChar = 0;
	unsigned int index;

	while (argv[hexChar++] != '\0') ;

	hexChar -= 1;

#if DEBUG
	printf("Number of character in string %d \n", hexChar);
#endif
	for (radix = hexChar - 1, index = 0; radix >= 0; radix--, index++) {
#if DEBUG
		printf(" hexdChar %d argv[%d] = %c\n", hexChar, index, argv[index]);
#endif
		switch (argv[index]) {
		case '0':
			hexNum += 0 * power(radix);
			break;
		case '1':
			hexNum += 1 * power(radix);
			break;
		case '2':
			hexNum += 2 * power(radix);
			break;
		case '3':
			hexNum += 3 * power(radix);
			break;
		case '4':
			hexNum += 4 * power(radix);
			break;
		case '5':
			hexNum += 5 * power(radix);
			break;
		case '6':
			hexNum += 6 * power(radix);
			break;
		case '7':
			hexNum += 7 * power(radix);
			break;
		case '8':
			hexNum += 8 * power(radix);
			break;
		case '9':
			hexNum += 9 * power(radix);
			break;
		default:
			status = 1;
			break;
		}
		if (status == 1) {
#if DEBUG
			printf("Invalid Integer Number \n");
#endif
			return -1;
		}
	}

#if DEBUG
	printf("Integer Number  %u is 0x%x\n", hexNum, hexNum);
#endif

	*result = hexNum;
	return 0;

}

static unsigned int power(int radix)
{
	unsigned int result = 10;

	if (radix == 0)
		return 1;

	for (; radix > 0; radix--) {
		if (radix == 1) {
			return result;
		}
		result = result * 10;
	}
	return 0;
}
