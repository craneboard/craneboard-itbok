/*
 * audio_drv.c: Audio driver.
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

/* includes */
#include <common.h>
#include "types.h"
#include "omap3530evm.h"
#include "triton2_utils.h"
#include "triton2_drv.h"
#include "mcbsp_utils.h"
#include "mcbsp_drv.h"
#include "audio_drv.h"

/* Initialize the audio. */
S32 audio_init(audio_drv_struct_t * audio_drv)
{
	S32 ret_status = SUCCESS;

	/*
	 ** Initialize the McBSP.
	 */
	audio_drv->mcbsp_drv.mcbsp_initstr.mcbsp_no = MCBSP_2;
	audio_drv->mcbsp_drv.mcbsp_initstr.master = MCBSP_SLAVE;
	audio_drv->mcbsp_drv.mcbsp_initstr.clksr_sel = MCBSP_SAMPLINGRATE_SEL;
	audio_drv->mcbsp_drv.mcbsp_initstr.clksr_val = audio_drv->sampling_rate;
	audio_drv->mcbsp_drv.mcbsp_initstr.mode = MCBSP_I2S;
	audio_drv->mcbsp_drv.mcbsp_initstr.num_of_phases = 2;
	audio_drv->mcbsp_drv.mcbsp_initstr.num_words_phase1 = 1;
	audio_drv->mcbsp_drv.mcbsp_initstr.num_words_phase2 = 1;
	audio_drv->mcbsp_drv.mcbsp_initstr.word_size_phase1 = 16;
	audio_drv->mcbsp_drv.mcbsp_initstr.word_size_phase2 = 16;
	ret_status = mcbsp_init(&(audio_drv->mcbsp_drv));

	if (SUCCESS != ret_status) {
#ifdef OMAP3530_DEBUG
		DEBUG("%s:%d - McBSP init failed.\r\n", __FILE__, __LINE__);
#endif
		return FAILURE;
	}
	/*
	 ** Initialize the TRITON2 CODEC.
	 */
	audio_drv->triton2_drv.triton2_init_str.t2pwron_irq_callback = NULL;
	audio_drv->triton2_drv.triton2_init_str.sampling_rate = audio_drv->sampling_rate;

	ret_status = dg_triton2_init(&(audio_drv->triton2_drv));

	if (SUCCESS != ret_status) {
#ifdef OMAP3530_DEBUG
		DEBUG("%s:%d - Triton2 init failed.\r\n", __FILE__, __LINE__);
#endif
		return FAILURE;
	}
	return ret_status;
}

/* Reads data from the audio. */
S32 audio_read(audio_drv_struct_t * audio_drv, audio_tag_t tag, U32 * size, U8 * buffer)
{
	S32 ret_status = SUCCESS;
	U32 data;
	U8 *pData;

	pData = (U8 *) & data;

	switch (tag) {
	case AUDIO_SAMPLINGRATE_TAG:
		*((U32 *) buffer) = audio_drv->sampling_rate;
		*size = 1;
		break;

	case AUDIO_VOLUME_TAG:
		*((U32 *) buffer) = audio_drv->volume;
		*size = 1;
		break;

	case AUDIO_MIC_TAG:
		*buffer = audio_drv->mic;
		*size = 1;
		break;

	case AUDIO_LOUDSPEAKER_TAG:
		*buffer = audio_drv->loudspeaker;
		*size = 1;
		break;

	case AUDIO_TIME_TAG:
		*buffer = audio_drv->time;
		*size = 1;
		break;

	case AUDIO_FILE_STARTADDR_TAG:
		*((U32 *) buffer) = audio_drv->file_startaddr;
		*size = 1;
		break;

	case AUDIO_FILE_SIZE_TAG:
		*((U32 *) buffer) = audio_drv->file_size;
		*size = 1;
		break;

	case AUDIO_SINEWAVE_AMP_TAG:
		*((U32 *) buffer) = audio_drv->sinewave_amp;
		*size = 1;
		break;

	case AUDIO_SINEWAVE_SAMPLES_TAG:
		*((U32 *) buffer) = audio_drv->sinewave_num_samples;
		*size = 1;
		break;

	case AUDIO_RW_TAG:
		/* This will read from McBSP. */
		ret_status = mcbsp_read(&(audio_drv->mcbsp_drv), MCBSP_RW_TAG, size, buffer);
		if (SUCCESS != ret_status) {
#ifdef OMAP3530_DEBUG
			DEBUG("%s:%d - McBSP read failed.\r\n", __FILE__, __LINE__);
#endif
		}
		break;

	default:
#ifdef OMAP3530_DEBUG
		DEBUG("%s:%d - Invalid tag to Audio read.\r\n", __FILE__, __LINE__);
#endif
		return FAILURE;
	}

	return ret_status;
}

/* Writes data to the audio. */
S32 audio_write(audio_drv_struct_t * audio_drv, audio_tag_t tag, U32 * size, U8 * buffer)
{
	S32 ret_status = SUCCESS;
	U32 data;
	U8 *pData;

	pData = (U8 *) & data;

	switch (tag) {
	case AUDIO_VOLUME_TAG:
		if (NULL != buffer) {
			audio_drv->volume = *(U32 *) buffer;
		}

		/* Pass the new volume value to CODEC. */
		ret_status = dg_triton2_write(&(audio_drv->triton2_drv), TRITON2_VOLUME_TAG, size, buffer);
		if (SUCCESS != ret_status) {
#ifdef OMAP3530_DEBUG
			DEBUG("%s:%d - Triton2 write failed.\r\n", __FILE__, __LINE__);
#endif
		}
		break;

	case AUDIO_LOUDSPEAKER_TAG:
		if (NULL != buffer) {
			audio_drv->loudspeaker = *buffer;
		}

		/* Pass the new loudspeaker to CODEC. */
		ret_status = dg_triton2_write(&(audio_drv->triton2_drv), TRITON2_LOUDSPEAKER_TAG, size, buffer);
		if (SUCCESS != ret_status) {
#ifdef OMAP3530_DEBUG
			DEBUG("%s:%d - Triton2 write failed.\r\n", __FILE__, __LINE__);
#endif
		}
		break;

	case AUDIO_RW_TAG:
		/* This will write to McBSP. */
		ret_status = mcbsp_write(&(audio_drv->mcbsp_drv), MCBSP_RW_TAG, size, buffer);
		if (SUCCESS != ret_status) {
#ifdef OMAP3530_DEBUG
			DEBUG("%s:%d - McBSP write failed.\r\n", __FILE__, __LINE__);
#endif
		}
		break;

	case AUDIO_MIC_TAG:
		/* Not required for OMAP3530 EVM. */
		break;

	default:
#ifdef OMAP3530_DEBUG
		DEBUG("%s:%d - Invalid tag to Audio write.\r\n", __FILE__, __LINE__);
#endif
		return FAILURE;
	}

	return ret_status;
}

/* Deinitialize the audio. */
S32 audio_deinit(audio_drv_struct_t * audio_drv)
{
	S32 ret_status = SUCCESS;

	/*
	 ** Deinitialize the CODEC.
	 */
	ret_status = dg_triton2_deinit(&(audio_drv->triton2_drv));
	if (SUCCESS != ret_status) {
#ifdef OMAP3530_DEBUG
		DEBUG("%s:%d - Triton2 deinit failed.\r\n", __FILE__, __LINE__);
#endif
		return FAILURE;
	}

	/*
	 ** Deinitialize the McBSP.
	 */
	ret_status = mcbsp_deinit(&(audio_drv->mcbsp_drv));
	if (SUCCESS != ret_status) {
#ifdef OMAP3530_DEBUG
		DEBUG("%s:%d - McBSP init failed.\r\n", __FILE__, __LINE__);
#endif
		return FAILURE;
	}

	return ret_status;
}

U32 fm_loopback_test(audio_drv_struct_t * audio_drv)
{
	triton2_fm_loop_init(&(audio_drv->triton2_drv));
	triton2_line_in_init(&(audio_drv->triton2_drv));
	return 0;
}
