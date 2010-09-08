/*
 * dg_audio.c: Audio diagnostic code.
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
#include <malloc.h>
#include <diag_core.h>
#include <omap3530evm.h>
#include "types.h"
#include "dg_gpio.h"
#include "audio_drv.h"
#include "dg_audio.h"
#include "dg_fixedtone.h"
#include "diag_core.h"

extern unsigned long strtoul(char *cp);
extern U32 triton2_fm_loop_init(triton_drv_struct_t * triton2_drv);
extern U32 triton2_fm_loop_deinit(triton_drv_struct_t * triton2_drv);

void register_audio_tests(void)
{
	status_t ret;
	char print_buf[512];

	ret = diag_hw_interface_add(do_audio_diag, "audio", "Test Audio Interface");
	if (ret != SUCCESS) {
		fprintf(stdout, "Failed to add Audio interface.\r\n");
	}

	/* Add play fixed tone test. */
	sprintf(print_buf, "Plays fixed tone" "\n\r\t\tParamaters:" "\n\r\t\t[Time    ] - Duration in seconds (1 - 255)" "\n\r\t\t[Volume  ] - Volume (0 - 20)\r\n" "\n\r\t\tSampling rate is fixed at 44100 Hz");

	ret = diag_hw_test_add("audio", DG_AUDIO_PLAY_FIXEDTONE, "tone", print_buf, 2, TYPE_U8, "Time in Secs", TYPE_U32, "Volume");

	/* Add records and playback test. */
	sprintf(print_buf, "Records for given duration and playback" "\n\r\t\tParamaters:" "\n\r\t\t[Time    ] - Duration in seconds (1 - 10)" "\n\r\t\t[Volume  ] - Volume (0 - 20)\r\n" "\n\r\t\tSampling rate is fixed at 44100 Hz");

	ret = diag_hw_test_add("audio", DG_AUDIO_PLAY_RECORD_AND_PLAY, "rcdplay", print_buf, 2, TYPE_U8, "Time in Secs", TYPE_U32, "Volume");

	/* Analog loopback test. */
	sprintf(print_buf, "Loops back the audio-in to audio-out" "\n\r\t\tParamaters:" "\n\r\t\t[Time    ] - Duration in seconds (1 - 255)\r\n" "\n\r\t\tSampling rate is fixed at 44100 Hz");

	ret = diag_hw_test_add("audio", DG_AUDIO_ANALOG_LOOPBACK, "fmloopback", print_buf, 1, TYPE_U8, "Time in Secs");

}

status_t do_audio_diag(u8 diag_id, int argc, char *argv[])
{
	U8 time;
	U32 volume;

	/* Read the time and volume. */
	time = (U8) strtoul(argv[0]);
	if (diag_id != DG_AUDIO_ANALOG_LOOPBACK)
		volume = (U32) strtoul(argv[1]);

	switch (diag_id) {
	case DG_AUDIO_PLAY_FIXEDTONE:
#ifdef OMAP3530_DEBUG
		DEBUG("Test parameters for Play Fixed Tone:\n\r" "Time :0x%x \n\rVolume :0x%x \n\r", time, volume);
#endif
		dg_audio_play_fixedtone(time, volume);
		break;

	case DG_AUDIO_PLAY_RECORD_AND_PLAY:
#ifdef OMAP3530_DEBUG
		DEBUG("Test parameters for Audio Record And Play:\n\r" "Time :0x%x \n\rVolume :0x%x \n\r", time, volume);
#endif
		diag_audio_record_playback(time, volume);
		break;

	case DG_AUDIO_ANALOG_LOOPBACK:
#ifdef OMAP3530_DEBUG
		DEBUG("Test parameters for Audio Loop back:\n\r" "Time :0x%x \n\r", time);
#endif
		diag_audio_fmloopback(time);
		break;

	default:
		fprintf(stdout, "Error...Invalid test request received.\n");
	}

	return SUCCESS;
}

/* Test function to test plays fixed tone. */
void dg_audio_play_fixedtone(U8 time, U32 volume)
{
	U8 *pbuffer;
	S32 ret_val = SUCCESS;
	U32 size;
	U32 buffer;
	U32 initTimeval;
	U32 finTimeval;
	U32 secondsElapsed;
	volatile U32 *sync32khzClk;
	audio_drv_struct_t audio_drv;

	sync32khzClk = CLK32K_COUNTER_REGISTER;
	initTimeval = *sync32khzClk;

	/* Validate the input parameters. */
	if (0 == time) {
#ifdef OMAP3530_DEBUG
		DEBUG("Time given is zero.\n\r");
		DEBUG("Valid time: 1 - 255\n\r");
#endif
		return;
	}

	if (volume > MAX_VOLUME_VAL) {
#ifdef OMAP3530_DEBUG
		DEBUG("Volume given is too high.\n\r");
		DEBUG("Valid volume: 0 - 20\n\r");
#endif
		return;
	}

	/* Initilise the Audio driver structure. */
	audio_drv.sampling_rate = SAMPLING_RATE_44100;
	audio_drv.codec = TRITON2_CODEC;

	/* Initialise the Audio. */
	ret_val = audio_init(&audio_drv);
	if (SUCCESS != ret_val) {
#ifdef OMAP3530_DEBUG
		DEBUG("%s:%d - Audio init failed.\r\n", __FILE__, __LINE__);
#endif
		return;
	}

	/* Enable / disable loudspeaker. */
	size = 1;
	pbuffer = (U8 *) & buffer;
	ret_val = audio_write(&audio_drv, AUDIO_LOUDSPEAKER_TAG, &size, (U8 *) pbuffer);
	if (SUCCESS != ret_val) {
#ifdef OMAP3530_DEBUG
		DEBUG("%s:%d - Audio write failed.\r\n", __FILE__, __LINE__);
#endif
		return;
	}

	/* Update the volume. */
	size = 1;
	buffer = volume;
	ret_val = audio_write(&audio_drv, AUDIO_VOLUME_TAG, &size, (U8 *) & buffer);
	if (SUCCESS != ret_val) {
#ifdef OMAP3530_DEBUG
		DEBUG("%s:%d - Audio write failed.\r\n", __FILE__, __LINE__);
#endif
		return;
	}

	/* Play the tone. */
	size = (sizeof(audio_fixedtone) / 2);

	/* Time. */
	finTimeval = *sync32khzClk;
	secondsElapsed = ((finTimeval - initTimeval) * 31 / 1000000);

	while (secondsElapsed <= time) {
		ret_val = audio_write(&audio_drv, AUDIO_RW_TAG, &size, (U8 *) audio_fixedtone);
		if (SUCCESS != ret_val) {
#ifdef OMAP3530_DEBUG
			DEBUG("%s:%d - Audio write failed.\r\n", __FILE__, __LINE__);
#endif
			return;
		}

		finTimeval = *sync32khzClk;
		secondsElapsed = ((finTimeval - initTimeval) * 31 / 1000000);
	}

	fprintf(stdout, "\r\nAudio play fixed tone test done.\r\n\n");

	/* Deinitialize the Audio. */
	ret_val = audio_deinit(&audio_drv);
	if (SUCCESS != ret_val) {
#ifdef OMAP3530_DEBUG
		DEBUG("%s:%d - Audio deinit failed.\r\n", __FILE__, __LINE__);
#endif
		return;
	}

	return;
}

/* Record form line-in and play it back on line-out. */
void diag_audio_record_playback(U8 time, U32 volume)
{
	U8 count1;
	U8 count2;
	U8 *pbuffer;
	S32 ret_val = SUCCESS;
	U32 size;
	U32 buffer;
	U32 initTimeval;
	U32 rec_memsize;
	U32 rec_memsize_temp;
	volatile U32 *sync32khzClk;
	U8 *precbuffer[5];
	audio_drv_struct_t audio_drv;

	sync32khzClk = CLK32K_COUNTER_REGISTER;
	initTimeval = *sync32khzClk;

	/* Validate the input parameters. */
	if ((time > 10) || (time < 1)) {
#ifdef OMAP3530_DEBUG
		DEBUG("Time given is zero.\n\r");
		DEBUG("Valid time: 1 - 10\n\r");
#endif
		return;
	}

	if (volume > MAX_VOLUME_VAL) {
#ifdef OMAP3530_DEBUG
		DEBUG("Volume given is too high.\n\r");
		DEBUG("Valid volume: 0 - 20\n\r");
#endif
		return;
	}

	/* Initilise the Audio driver structure. */
	audio_drv.sampling_rate = SAMPLING_RATE_44100;
	audio_drv.codec = TRITON2_CODEC;

	/* Initialise the Audio. */
	ret_val = audio_init(&audio_drv);
	if (SUCCESS != ret_val) {
#ifdef OMAP3530_DEBUG
		DEBUG("%s:%d - Audio init failed.\r\n", __FILE__, __LINE__);
#endif
		return;
	}

	/* 
	 ** Allocate the audio sample holding buffer.
	 ** Record the Data.
	 */
	rec_memsize = (4 * SAMPLING_RATE_44100);
	rec_memsize_temp = (rec_memsize / 2);

	fprintf(stdout, "\r\nRecording from audio-in...");
	for (count1 = 0; count1 < time; count1++) {
		precbuffer[count1] = (U8 *) malloc(rec_memsize);
		if (NULL == precbuffer[count1]) {
			fprintf(stdout, "Memory allocation for record buffer failed.\r\n");
			fprintf(stdout, "Enter less record time.\r\n");
			goto RET_FAILURE;
		}

		ret_val = audio_read(&audio_drv, AUDIO_RW_TAG, &rec_memsize_temp, (U8 *) precbuffer[count1]);
		if (SUCCESS != ret_val) {
#ifdef OMAP3530_DEBUG
			DEBUG("%s:%d - Audio read failed.\r\n", __FILE__, __LINE__);
#endif
			goto RET_FAILURE;
		}
	}
	fprintf(stdout, "done.\r\n");

	/* Enable / disable loudspeaker. */
	size = 1;
	pbuffer = (U8 *) & buffer;
	ret_val = audio_write(&audio_drv, AUDIO_LOUDSPEAKER_TAG, &size, (U8 *) pbuffer);
	if (SUCCESS != ret_val) {
#ifdef OMAP3530_DEBUG
		DEBUG("%s:%d - Audio write failed.\r\n", __FILE__, __LINE__);
#endif
		goto RET_FAILURE;
	}

	/* Update the volume. */
	size = 1;
	buffer = volume;
	ret_val = audio_write(&audio_drv, AUDIO_VOLUME_TAG, &size, (U8 *) & buffer);
	if (SUCCESS != ret_val) {
#ifdef OMAP3530_DEBUG
		DEBUG("%s:%d - Audio write failed.\r\n", __FILE__, __LINE__);
#endif
		goto RET_FAILURE;
	}

	/* Start playing it back. */
	fprintf(stdout, "\r\nPlaying back on audio-out...");
	for (count1 = 0; count1 < time; count1++) {
		ret_val = audio_write(&audio_drv, AUDIO_RW_TAG, &rec_memsize_temp, (U8 *) precbuffer[count1]);
		if (SUCCESS != ret_val) {
#ifdef OMAP3530_DEBUG
			DEBUG("%s:%d - Audio write failed.\r\n", __FILE__, __LINE__);
#endif
			count1 = time;
			goto RET_FAILURE;
		}
	}
	fprintf(stdout, "done.\r\n");
	fprintf(stdout, "\r\nAudio record and playback test done.\r\n\n");

	/* Freeing the record buffer. */
	if (0 != count1) {
		for (count2 = 0; count2 < time; count2++)
			free((void *)precbuffer[count2]);
	}

	/* Deinitialize the Audio. */
	ret_val = audio_deinit(&audio_drv);
	if (SUCCESS != ret_val) {
#ifdef OMAP3530_DEBUG
		DEBUG("%s:%d - Audio deinit failed.\r\n", __FILE__, __LINE__);
#endif
		return;
	}

	return;

 RET_FAILURE:
	fprintf(stdout, "\r\nAudio record and playback test failed.\r\n\n");
	/* Freeing the record buffer. */
	if (0 != count1) {
		for (count2 = 0; count2 < count1; count2++)
			free((void *)precbuffer[count2]);
	}

	/* Deinitialize the Audio. */
	ret_val = audio_deinit(&audio_drv);
	if (SUCCESS != ret_val) {
#ifdef OMAP3530_DEBUG
		DEBUG("%s:%d - Audio deinit failed.\r\n", __FILE__, __LINE__);
#endif
		return;
	}
}

/* Analog loopback of audio-in and audio-out interfaces. */
void diag_audio_fmloopback(U8 time)
{
	U8 *pbuffer;
	S32 ret_val = SUCCESS;
	U32 size;
	U32 buffer;
	U32 initTimeval;
	U32 finTimeval;
	U32 secondsElapsed;
	volatile U32 *sync32khzClk;
	audio_drv_struct_t audio_drv;

	sync32khzClk = CLK32K_COUNTER_REGISTER;
	initTimeval = *sync32khzClk;

	if (0 == time) {
#ifdef OMAP3530_DEBUG
		DEBUG("Time given is zero.\n\r");
		DEBUG("Valid time: 1 - 255\n\r");
#endif
		return;
	}

	/* Initilise the Audio driver structure. */
	audio_drv.sampling_rate = SAMPLING_RATE_44100;
	audio_drv.codec = TRITON2_CODEC;

	/* Initialise the Audio. */
	ret_val = audio_init(&audio_drv);
	if (SUCCESS != ret_val) {
#ifdef OMAP3530_DEBUG
		DEBUG("%s:%d - Audio init failed.\r\n", __FILE__, __LINE__);
#endif
		return;
	}

	/* Enable / disable loudspeaker. */
	size = 1;
	pbuffer = (U8 *) & buffer;
	ret_val = audio_write(&audio_drv, AUDIO_LOUDSPEAKER_TAG, &size, (U8 *) pbuffer);
	if (SUCCESS != ret_val) {
#ifdef OMAP3530_DEBUG
		DEBUG("%s:%d - Audio write failed.\r\n", __FILE__, __LINE__);
#endif
		return;
	}

	/* Analog loopback test. */

	triton2_fm_loop_init(&(audio_drv.triton2_drv));

	/* Time out. */
	finTimeval = *sync32khzClk;
	secondsElapsed = ((finTimeval - initTimeval) * 31 / 1000000);
	while (secondsElapsed <= time) {
		finTimeval = *sync32khzClk;
		secondsElapsed = ((finTimeval - initTimeval) * 31 / 1000000);
	}

	fprintf(stdout, "\r\nAudio loopback test done.\r\n\n");
	triton2_fm_loop_deinit(&(audio_drv.triton2_drv));

	/* Deinitialize the Audio. */
	ret_val = audio_deinit(&audio_drv);
	if (SUCCESS != ret_val) {
#ifdef OMAP3530_DEBUG
		DEBUG("%s:%d - Audio deinit failed.\r\n", __FILE__, __LINE__);
#endif
		return;
	}

	return;
}
