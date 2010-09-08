/*
 * types.h: data types definitions
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

#ifndef __MS_DIAG_TYPES_H__
#define __MS_DIAG_TYPES_H__

#ifdef __cplusplus
extern "C" {
#endif


#ifndef NULL
#define NULL    ((void *)0)
#endif

typedef unsigned char U8;
typedef signed char S8;
typedef char C8;
typedef U8 UWORD8;
typedef S8 WORD8;
typedef U8 BOOLEAN;

typedef unsigned short int U16;
typedef signed short int S16;
typedef U16 UWORD16;
typedef S16 WORD16;

typedef float F32;

typedef unsigned long U32;
typedef signed long S32;
typedef U32 UWORD32;
typedef S32 WORD32;

typedef volatile unsigned char  REG_U8;
typedef volatile signed   char  REG_S8;

typedef volatile unsigned short REG_U16;
typedef volatile          short REG_S16;

typedef volatile unsigned long  REG_U32;
typedef volatile          long  REG_S32;

typedef volatile unsigned char*  PREG_U8;
typedef volatile signed   char*  PREG_S8;

typedef volatile unsigned short*  PREG_U16;
typedef volatile          short*  PREG_S16;

typedef volatile unsigned long*  PREG_U32;
typedef volatile          long*  PREG_S32;

typedef enum
{
    FAILURE,
    SUCCESS
}status_t;

typedef enum {
    FALSE = 0,
    TRUE = 1
} boolean_t;

#ifdef __cplusplus
}
#endif

#endif /* __MS_DIAG_TYPES_H__ */
