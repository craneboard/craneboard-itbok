/*
 * audio_drv.h: Audio driver definitions.
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

#ifndef __MS_DIAG_AUDIO_DRV_H__
#define __MS_DIAG_AUDIO_DRV_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "types.h"
#include "mcbsp_drv.h"
#include "triton2_drv.h"

#define TRITON2_CODEC   1

typedef struct audio_drv_struct
{
    U32 sampling_rate;
    U32 codec;
    U32 volume;                        
    U8 mic;                            
    U8 loudspeaker;
    U8 time;
    U32 file_startaddr;              
    U32 file_size;    
    U32 sinewave_amp;
    U32 sinewave_num_samples; 
    mcbsp_drv_struct_t mcbsp_drv;
    triton_drv_struct_t triton2_drv;

} audio_drv_struct_t;

typedef enum
{
    AUDIO_SAMPLINGRATE_TAG,
    AUDIO_VOLUME_TAG,
    AUDIO_MIC_TAG,
    AUDIO_LOUDSPEAKER_TAG,
    AUDIO_TIME_TAG,
    AUDIO_FILE_STARTADDR_TAG,
    AUDIO_FILE_SIZE_TAG,
    AUDIO_SINEWAVE_AMP_TAG,
    AUDIO_SINEWAVE_SAMPLES_TAG,
    AUDIO_RW_TAG

} audio_tag_t;


/* Function Prototypes. */
S32 audio_init (audio_drv_struct_t *audio_drv);
S32 audio_read (audio_drv_struct_t *audio_drv, audio_tag_t tag,
                U32 *size, U8 *buffer);
S32 audio_write (audio_drv_struct_t *audio_drv, audio_tag_t tag,
                 U32 *size, U8 *buffer);
S32 audio_deinit (audio_drv_struct_t *audio_drv);

U32 fm_loopback_test ( audio_drv_struct_t *audio_drv );

#ifdef __cplusplus
}
#endif

#endif  /* __MS_DIAG_AUDIO_DRV_H__ */
