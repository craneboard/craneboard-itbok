/*
 * dg_audio.h: Audio diagnostic header file.
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


#ifndef __MS_DIAG_AUDIO_H__
#define __MS_DIAG_AUDIO_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "types.h"

#define DG_AUDIO_PLAY_FIXEDTONE             1
#define DG_AUDIO_PLAY_RECORD_AND_PLAY       2
#define DG_AUDIO_ANALOG_LOOPBACK            3

#define MAX_VOLUME_VAL          20
#define SAMPLING_RATE_44100     44100

/* Function Prototypes. */
status_t do_audio_diag (u8 diag_id, int argc, char *argv[]);
void dg_audio_play_fixedtone (U8 time, U32 volume);
void  diag_audio_record_playback (U8 time, U32 volume);
void diag_audio_fmloopback (U8 time);

#ifdef __cplusplus
}
#endif

#endif  /* __MS_DIAG_I2C_H__ */
