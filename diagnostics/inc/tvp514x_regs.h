/*
 * video input TVP5146 device file.
 *
 * (C) Copyright 2010
 * Mistral Solutions Private Limited, <www.mistralsolutions.com>
 *
 * Author :
 *	Ganeshan <nganeshan@mistralsolutions.com>
 *
 * Derived from NA
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
 *   14 Jul 2009 - Created 
 */

#ifndef _TVP514X_REGS_H
#define _TVP514X_REGS_H

//typedef long long v4l2_std_id;

/*  Values for the 'type' field */
#define V4L2_INPUT_TYPE_TUNER       1
#define V4L2_INPUT_TYPE_CAMERA      2

/*
 * TVP5146/47 registers
 */
#define REG_INPUT_SEL			(0x00)
#define REG_AFE_GAIN_CTRL		(0x01)
#define REG_VIDEO_STD			(0x02)
#define REG_OPERATION_MODE		(0x03)
#define REG_AUTOSWITCH_MASK		(0x04)

#define REG_COLOR_KILLER		(0x05)
#define REG_LUMA_CONTROL1		(0x06)
#define REG_LUMA_CONTROL2		(0x07)
#define REG_LUMA_CONTROL3		(0x08)

#define REG_BRIGHTNESS			(0x09)
#define REG_CONTRAST			(0x0A)
#define REG_SATURATION			(0x0B)
#define REG_HUE				(0x0C)

#define REG_CHROMA_CONTROL1		(0x0D)
#define REG_CHROMA_CONTROL2		(0x0E)

/* 0x0F Reserved */

#define REG_COMP_PR_SATURATION		(0x10)
#define REG_COMP_Y_CONTRAST		(0x11)
#define REG_COMP_PB_SATURATION		(0x12)

/* 0x13 Reserved */

#define REG_COMP_Y_BRIGHTNESS		(0x14)

/* 0x15 Reserved */

#define REG_AVID_START_PIXEL_LSB	(0x16)
#define REG_AVID_START_PIXEL_MSB	(0x17)
#define REG_AVID_STOP_PIXEL_LSB		(0x18)
#define REG_AVID_STOP_PIXEL_MSB		(0x19)

#define REG_HSYNC_START_PIXEL_LSB	(0x1A)
#define REG_HSYNC_START_PIXEL_MSB	(0x1B)
#define REG_HSYNC_STOP_PIXEL_LSB	(0x1C)
#define REG_HSYNC_STOP_PIXEL_MSB	(0x1D)

#define REG_VSYNC_START_LINE_LSB	(0x1E)
#define REG_VSYNC_START_LINE_MSB	(0x1F)
#define REG_VSYNC_STOP_LINE_LSB		(0x20)
#define REG_VSYNC_STOP_LINE_MSB		(0x21)

#define REG_VBLK_START_LINE_LSB		(0x22)
#define REG_VBLK_START_LINE_MSB		(0x23)
#define REG_VBLK_STOP_LINE_LSB		(0x24)
#define REG_VBLK_STOP_LINE_MSB		(0x25)

/* 0x26 - 0x27 Reserved */

#define REG_FAST_SWTICH_CONTROL		(0x28)

/* 0x29 Reserved */

#define REG_FAST_SWTICH_SCART_DELAY	(0x2A)

/* 0x2B Reserved */

#define REG_SCART_DELAY			(0x2C)
#define REG_CTI_DELAY			(0x2D)
#define REG_CTI_CONTROL			(0x2E)

/* 0x2F - 0x31 Reserved */

#define REG_SYNC_CONTROL		(0x32)
#define REG_OUTPUT_FORMATTER1		(0x33)
#define REG_OUTPUT_FORMATTER2		(0x34)
#define REG_OUTPUT_FORMATTER3		(0x35)
#define REG_OUTPUT_FORMATTER4		(0x36)
#define REG_OUTPUT_FORMATTER5		(0x37)
#define REG_OUTPUT_FORMATTER6		(0x38)
#define REG_CLEAR_LOST_LOCK		(0x39)

#define REG_STATUS1			(0x3A)
#define REG_STATUS2			(0x3B)

#define REG_AGC_GAIN_STATUS_LSB		(0x3C)
#define REG_AGC_GAIN_STATUS_MSB		(0x3D)

/* 0x3E Reserved */

#define REG_VIDEO_STD_STATUS		(0x3F)
#define REG_GPIO_INPUT1			(0x40)
#define REG_GPIO_INPUT2			(0x41)

/* 0x42 - 0x45 Reserved */

#define REG_AFE_COARSE_GAIN_CH1		(0x46)
#define REG_AFE_COARSE_GAIN_CH2		(0x47)
#define REG_AFE_COARSE_GAIN_CH3		(0x48)
#define REG_AFE_COARSE_GAIN_CH4		(0x49)

#define REG_AFE_FINE_GAIN_PB_B_LSB	(0x4A)
#define REG_AFE_FINE_GAIN_PB_B_MSB	(0x4B)
#define REG_AFE_FINE_GAIN_Y_G_CHROMA_LSB	(0x4C)
#define REG_AFE_FINE_GAIN_Y_G_CHROMA_MSB	(0x4D)
#define REG_AFE_FINE_GAIN_PR_R_LSB	(0x4E)
#define REG_AFE_FINE_GAIN_PR_R_MSB	(0x4F)
#define REG_AFE_FINE_GAIN_CVBS_LUMA_LSB	(0x50)
#define REG_AFE_FINE_GAIN_CVBS_LUMA_MSB	(0x51)

/* 0x52 - 0x68 Reserved */

#define REG_FBIT_VBIT_CONTROL1		(0x69)

/* 0x6A - 0x6B Reserved */

#define REG_BACKEND_AGC_CONTROL		(0x6C)

/* 0x6D - 0x6E Reserved */

#define REG_AGC_DECREMENT_SPEED_CONTROL	(0x6F)
#define REG_ROM_VERSION			(0x70)

/* 0x71 - 0x73 Reserved */

#define REG_AGC_WHITE_PEAK_PROCESSING	(0x74)
#define REG_FBIT_VBIT_CONTROL2		(0x75)
#define REG_VCR_TRICK_MODE_CONTROL	(0x76)
#define REG_HORIZONTAL_SHAKE_INCREMENT	(0x77)
#define REG_AGC_INCREMENT_SPEED		(0x78)
#define REG_AGC_INCREMENT_DELAY		(0x79)

/* 0x7A - 0x7F Reserved */

#define REG_CHIP_ID_MSB			(0x80)
#define REG_CHIP_ID_LSB			(0x81)

/* 0x82 Reserved */

#define REG_CPLL_SPEED_CONTROL		(0x83)

/* 0x84 - 0x96 Reserved */

#define REG_STATUS_REQUEST		(0x97)

/* 0x98 - 0x99 Reserved */

#define REG_VERTICAL_LINE_COUNT_LSB	(0x9A)
#define REG_VERTICAL_LINE_COUNT_MSB	(0x9B)

/* 0x9C - 0x9D Reserved */

#define REG_AGC_DECREMENT_DELAY		(0x9E)

/* 0x9F - 0xB0 Reserved */

#define REG_VDP_TTX_FILTER_1_MASK1	(0xB1)
#define REG_VDP_TTX_FILTER_1_MASK2	(0xB2)
#define REG_VDP_TTX_FILTER_1_MASK3	(0xB3)
#define REG_VDP_TTX_FILTER_1_MASK4	(0xB4)
#define REG_VDP_TTX_FILTER_1_MASK5	(0xB5)
#define REG_VDP_TTX_FILTER_2_MASK1	(0xB6)
#define REG_VDP_TTX_FILTER_2_MASK2	(0xB7)
#define REG_VDP_TTX_FILTER_2_MASK3	(0xB8)
#define REG_VDP_TTX_FILTER_2_MASK4	(0xB9)
#define REG_VDP_TTX_FILTER_2_MASK5	(0xBA)
#define REG_VDP_TTX_FILTER_CONTROL	(0xBB)
#define REG_VDP_FIFO_WORD_COUNT		(0xBC)
#define REG_VDP_FIFO_INTERRUPT_THRLD	(0xBD)

/* 0xBE Reserved */

#define REG_VDP_FIFO_RESET		(0xBF)
#define REG_VDP_FIFO_OUTPUT_CONTROL	(0xC0)
#define REG_VDP_LINE_NUMBER_INTERRUPT	(0xC1)
#define REG_VDP_PIXEL_ALIGNMENT_LSB	(0xC2)
#define REG_VDP_PIXEL_ALIGNMENT_MSB	(0xC3)

/* 0xC4 - 0xD5 Reserved */

#define REG_VDP_LINE_START		(0xD6)
#define REG_VDP_LINE_STOP		(0xD7)
#define REG_VDP_GLOBAL_LINE_MODE	(0xD8)
#define REG_VDP_FULL_FIELD_ENABLE	(0xD9)
#define REG_VDP_FULL_FIELD_MODE		(0xDA)

/* 0xDB - 0xDF Reserved */

#define REG_VBUS_DATA_ACCESS_NO_VBUS_ADDR_INCR	(0xE0)
#define REG_VBUS_DATA_ACCESS_VBUS_ADDR_INCR	(0xE1)
#define REG_FIFO_READ_DATA			(0xE2)

/* 0xE3 - 0xE7 Reserved */

#define REG_VBUS_ADDRESS_ACCESS1	(0xE8)
#define REG_VBUS_ADDRESS_ACCESS2	(0xE9)
#define REG_VBUS_ADDRESS_ACCESS3	(0xEA)

/* 0xEB - 0xEF Reserved */

#define REG_INTERRUPT_RAW_STATUS0	(0xF0)
#define REG_INTERRUPT_RAW_STATUS1	(0xF1)
#define REG_INTERRUPT_STATUS0		(0xF2)
#define REG_INTERRUPT_STATUS1		(0xF3)
#define REG_INTERRUPT_MASK0		(0xF4)
#define REG_INTERRUPT_MASK1		(0xF5)
#define REG_INTERRUPT_CLEAR0		(0xF6)
#define REG_INTERRUPT_CLEAR1		(0xF7)

/* 0xF8 - 0xFF Reserved */

/*
 * Mask and bit definitions of TVP5146/47 registers
 */
/* The ID values we are looking for */
#define TVP514X_CHIP_ID_MSB		(0x51)
#define TVP5146_CHIP_ID_LSB		(0x46)
#define TVP5147_CHIP_ID_LSB		(0x47)

#define VIDEO_STD_MASK			(0x07)
#define VIDEO_STD_AUTO_SWITCH_BIT	(0x00)
#define VIDEO_STD_NTSC_MJ_BIT		(0x01)
#define VIDEO_STD_PAL_BDGHIN_BIT	(0x02)
#define VIDEO_STD_PAL_M_BIT		(0x03)
#define VIDEO_STD_PAL_COMBINATION_N_BIT	(0x04)
#define VIDEO_STD_NTSC_4_43_BIT		(0x05)
#define VIDEO_STD_SECAM_BIT		(0x06)
#define VIDEO_STD_PAL_60_BIT		(0x07)

/*
 * Status bit
 */
#define STATUS_TV_VCR_BIT		(1<<0)
#define STATUS_HORZ_SYNC_LOCK_BIT	(1<<1)
#define STATUS_VIRT_SYNC_LOCK_BIT	(1<<2)
#define STATUS_CLR_SUBCAR_LOCK_BIT	(1<<3)
#define STATUS_LOST_LOCK_DETECT_BIT	(1<<4)
#define STATUS_FEILD_RATE_BIT		(1<<5)
#define STATUS_LINE_ALTERNATING_BIT	(1<<6)
#define STATUS_PEAK_WHITE_DETECT_BIT	(1<<7)

/* Tokens for register write */
#define TOK_WRITE                       (0)     /* token for write operation */
#define TOK_TERM                        (1)     /* terminating token */
#define TOK_DELAY                       (2)     /* delay token for reg list */
#define TOK_SKIP                        (3)     /* token to skip a register */

#define TVP514X_XCLK_BT656      (27000000)

/* Number of pixels and number of lines per frame for different standards */
#define NTSC_NUM_ACTIVE_PIXELS      (720)
#define NTSC_NUM_ACTIVE_LINES       (480)
#define PAL_NUM_ACTIVE_PIXELS       (720)
#define PAL_NUM_ACTIVE_LINES        (576)

#define V4L2_STD_NTSC_M         ((v4l2_std_id)0x00001000)
#define V4L2_STD_NTSC_M_JP      ((v4l2_std_id)0x00002000)
#define V4L2_STD_NTSC_443       ((v4l2_std_id)0x00004000)
#define V4L2_STD_NTSC_M_KR      ((v4l2_std_id)0x00008000)


#define V4L2_STD_NTSC           (V4L2_STD_NTSC_M    |\
                 V4L2_STD_NTSC_M_JP     |\
                 V4L2_STD_NTSC_M_KR)


/* one bit for each */
#define V4L2_STD_PAL_B          ((v4l2_std_id)0x00000001)
#define V4L2_STD_PAL_B1         ((v4l2_std_id)0x00000002)
#define V4L2_STD_PAL_G          ((v4l2_std_id)0x00000004)
#define V4L2_STD_PAL_H          ((v4l2_std_id)0x00000008)
#define V4L2_STD_PAL_I          ((v4l2_std_id)0x00000010)
#define V4L2_STD_PAL_D          ((v4l2_std_id)0x00000020)
#define V4L2_STD_PAL_D1         ((v4l2_std_id)0x00000040)
#define V4L2_STD_PAL_K          ((v4l2_std_id)0x00000080)

#define V4L2_STD_PAL_M          ((v4l2_std_id)0x00000100)
#define V4L2_STD_PAL_N          ((v4l2_std_id)0x00000200)
#define V4L2_STD_PAL_Nc         ((v4l2_std_id)0x00000400)
#define V4L2_STD_PAL_60         ((v4l2_std_id)0x00000800)


/* FIXME:
   Although std_id is 64 bits, there is an issue on PPC32 architecture that
   makes switch(__u64) to break. So, there's a hack on v4l2-common.c rounding
   this value to 32 bits.
   As, currently, the max value is for V4L2_STD_ATSC_16_VSB (30 bits wide),
   it should work fine. However, if needed to add more than two standards,
   v4l2-common.c should be fixed.
 */

/* some merged standards */
#define V4L2_STD_MN (V4L2_STD_PAL_M|V4L2_STD_PAL_N|V4L2_STD_PAL_Nc|V4L2_STD_NTSC)
#define V4L2_STD_B  (V4L2_STD_PAL_B|V4L2_STD_PAL_B1|V4L2_STD_SECAM_B)
#define V4L2_STD_GH (V4L2_STD_PAL_G|V4L2_STD_PAL_H|V4L2_STD_SECAM_G|V4L2_STD_SECAM_H)
#define V4L2_STD_DK (V4L2_STD_PAL_DK|V4L2_STD_SECAM_DK)

/* some common needed stuff */
#define V4L2_STD_PAL_BG     (V4L2_STD_PAL_B     |\
                 V4L2_STD_PAL_B1    |\
                 V4L2_STD_PAL_G)
#define V4L2_STD_PAL_DK     (V4L2_STD_PAL_D     |\
                 V4L2_STD_PAL_D1    |\
                 V4L2_STD_PAL_K)
#define V4L2_STD_PAL        (V4L2_STD_PAL_BG    |\
                 V4L2_STD_PAL_DK    |\
                 V4L2_STD_PAL_H     |\
                 V4L2_STD_PAL_I)


/* Values for 'capabilities' field */
#define V4L2_CAP_VIDEO_CAPTURE      0x00000001  /* Is a video capture device */
#define V4L2_CAP_VIDEO_OUTPUT       0x00000002  /* Is a video output device */
#define V4L2_CAP_VIDEO_OVERLAY      0x00000004  /* Can do video overlay */
#define V4L2_CAP_VBI_CAPTURE        0x00000010  /* Is a raw VBI capture device */
#define V4L2_CAP_VBI_OUTPUT     0x00000020  /* Is a raw VBI output device */
#define V4L2_CAP_SLICED_VBI_CAPTURE 0x00000040  /* Is a sliced VBI capture device */
#define V4L2_CAP_SLICED_VBI_OUTPUT  0x00000080  /* Is a sliced VBI output device */
#define V4L2_CAP_RDS_CAPTURE        0x00000100  /* RDS data capture */
#define V4L2_CAP_VIDEO_OUTPUT_OVERLAY   0x00000200  /* Can do video output overlay */
#define V4L2_CAP_HW_FREQ_SEEK       0x00000400  /* Can do hardware frequency seek  */

#define V4L2_CAP_TUNER          0x00010000  /* has a tuner */
#define V4L2_CAP_AUDIO          0x00020000  /* has audio support */
#define V4L2_CAP_RADIO          0x00040000  /* is a radio device */

#define V4L2_CAP_READWRITE              0x01000000  /* read/write systemcalls */
#define V4L2_CAP_ASYNCIO                0x02000000  /* async I/O */
#define V4L2_CAP_STREAMING              0x04000000  /* streaming I/O ioctls */

/**

 * enum tvp514x_input - enum for different decoder input pin
 *      configuration.
 */
enum tvp514x_input {
    /*
     * CVBS input selection
     */
    INPUT_CVBS_VI1A = 0x0,
    INPUT_CVBS_VI1B,
    INPUT_CVBS_VI1C,
    INPUT_CVBS_VI2A = 0x04,
    INPUT_CVBS_VI2B,
    INPUT_CVBS_VI2C,
    INPUT_CVBS_VI3A = 0x08,
    INPUT_CVBS_VI3B,
    INPUT_CVBS_VI3C,
    INPUT_CVBS_VI4A = 0x0C,
    /*
     * S-Video input selection
     */
    INPUT_SVIDEO_VI2A_VI1A = 0x44,
    INPUT_SVIDEO_VI2B_VI1B,
    INPUT_SVIDEO_VI2C_VI1C,
    INPUT_SVIDEO_VI2A_VI3A = 0x54,
    INPUT_SVIDEO_VI2B_VI3B,
    INPUT_SVIDEO_VI2C_VI3C,
    INPUT_SVIDEO_VI4A_VI1A = 0x4C,
    INPUT_SVIDEO_VI4A_VI1B,
    INPUT_SVIDEO_VI4A_VI1C,
    INPUT_SVIDEO_VI4A_VI3A = 0x5C,
    INPUT_SVIDEO_VI4A_VI3B,
    INPUT_SVIDEO_VI4A_VI3C,

    /* Need to add entries for
     * RGB, YPbPr and SCART.
     */
    INPUT_INVALID
};

/**
 * enum tvp514x_output - enum for output format
 *          supported.
 *
 */
enum tvp514x_output {
    OUTPUT_10BIT_422_EMBEDDED_SYNC = 0,
    OUTPUT_20BIT_422_SEPERATE_SYNC,
    OUTPUT_10BIT_422_SEPERATE_SYNC = 3,
    OUTPUT_INVALID
};


/**
 * enum tvp514x_std - enum for supported standards
 */
enum tvp514x_std {
    STD_NTSC_MJ = 0,
    STD_PAL_BDGHIN,
    STD_INVALID
};
#if 0
struct v4l2_fract {
    U32   numerator;
    U32   denominator;
};

struct v4l2_standard {
   U32            index;
   v4l2_std_id      id;
    U8             name[24];
    struct v4l2_fract    frameperiod; /* Frames, not fields */
    U32            framelines;
    U32            reserved[4];
};

struct tvp514x_std_info {
    unsigned long width;
    unsigned long height;
    U8 video_std;
    struct v4l2_standard standard;
};
/*
 * Supported standards -
 *
 * Currently supports two standards only, need to add support for rest of the
 * modes, like SECAM, etc...
 */
static struct tvp514x_std_info tvp514x_std_list[] = {
    /* Standard: STD_NTSC_MJ */
    [STD_NTSC_MJ] = {
     .width = NTSC_NUM_ACTIVE_PIXELS,
     .height = NTSC_NUM_ACTIVE_LINES,
     .video_std = VIDEO_STD_NTSC_MJ_BIT,
     .standard = {
              .index = 0,
              .id = V4L2_STD_NTSC,
              .name = "NTSC",
              .frameperiod = {1001, 30000},
              .framelines = 525
             },
    /* Standard: STD_PAL_BDGHIN */
    },
    [STD_PAL_BDGHIN] = {
     .width = PAL_NUM_ACTIVE_PIXELS,
     .height = PAL_NUM_ACTIVE_LINES,
     .video_std = VIDEO_STD_PAL_BDGHIN_BIT,
     .standard = {
              .index = 1,
              .id = V4L2_STD_PAL,
              .name = "PAL",
              .frameperiod = {1, 25},
              .framelines = 625
             },
    },
    /* Standard: need to add for additional standard */
};


/**
 * struct tvp514x_reg - Structure for TVP5146/47 register initialization values
 * @token - Token: TOK_WRITE, TOK_TERM etc..
 * @reg - Register offset
 * @val - Register Value for TOK_WRITE or delay in ms for TOK_DELAY
 */
struct tvp514x_reg {
	U8 token;
	U8 reg;
	U32 val;
};

/**
 * struct tvp514x_init_seq - Structure for TVP5146/47/46M2/47M1 power up
 *		Sequence.
 * @ no_regs - Number of registers to write for power up sequence.
 * @ init_reg_seq - Array of registers and respective value to write.
 */
struct tvp514x_init_seq {
	unsigned int no_regs;
	const struct tvp514x_reg *init_reg_seq;
};
#endif

/* Number of pixels and number of lines per frame for different standards */
#define NTSC_NUM_ACTIVE_PIXELS      (720)
#define NTSC_NUM_ACTIVE_LINES       (480)
#define PAL_NUM_ACTIVE_PIXELS       (720)
#define PAL_NUM_ACTIVE_LINES        (576)
#if 0
/*
 *  V I D E O   I N P U T S
 */
struct v4l2_input {
    U32        index;     /*  Which input */
    U8         name[32];      /*  Label */
    U32        type;      /*  Type of input */
    U32        audioset;      /*  Associated audios (bitfield) */
    U32        tuner;             /*  Associated tuner */
    U32        std;
    U32        status;
    U32        reserved[4];
};
#endif
enum v4l2_field {
    V4L2_FIELD_ANY           = 0, /* driver can choose from none,
                     top, bottom, interlaced
                     depending on whatever it thinks
                     is approximate ... */
    V4L2_FIELD_NONE          = 1, /* this device has no fields ... */
    V4L2_FIELD_TOP           = 2, /* top field only */
    V4L2_FIELD_BOTTOM        = 3, /* bottom field only */
    V4L2_FIELD_INTERLACED    = 4, /* both fields interlaced */
    V4L2_FIELD_SEQ_TB        = 5, /* both fields sequential into one
                     buffer, top-bottom order */
    V4L2_FIELD_SEQ_BT        = 6, /* same as above + bottom-top order */
    V4L2_FIELD_ALTERNATE     = 7, /* both fields alternating into
                     separate buffers */
    V4L2_FIELD_INTERLACED_TB = 8, /* both fields interlaced, top field
                     first and the top field is
                     transmitted first */
    V4L2_FIELD_INTERLACED_BT = 9, /* both fields interlaced, top field
                     first and the bottom field is
                     transmitted first */
};

/* see also http://vektor.theorem.ca/graphics/ycbcr/ */
enum v4l2_colorspace {
    /* ITU-R 601 -- broadcast NTSC/PAL */
    V4L2_COLORSPACE_SMPTE170M     = 1,

    /* 1125-Line (US) HDTV */
    V4L2_COLORSPACE_SMPTE240M     = 2,

    /* HD and modern captures. */
    V4L2_COLORSPACE_REC709        = 3,

    /* broken BT878 extents (601, luma range 16-253 instead of 16-235) */
    V4L2_COLORSPACE_BT878         = 4,

    /* These should be useful.  Assume 601 extents. */
    V4L2_COLORSPACE_470_SYSTEM_M  = 5,
    V4L2_COLORSPACE_470_SYSTEM_BG = 6,

    /* I know there will be cameras that send this.  So, this is
     * unspecified chromaticities and full 0-255 on each of the
     * Y'CbCr components
     */
    V4L2_COLORSPACE_JPEG          = 7,

    /* For RGB colourspaces, this is probably a good start. */
    V4L2_COLORSPACE_SRGB          = 8,
};

enum v4l2_buf_type {
    V4L2_BUF_TYPE_VIDEO_CAPTURE        = 1,
    V4L2_BUF_TYPE_VIDEO_OUTPUT         = 2,
    V4L2_BUF_TYPE_VIDEO_OVERLAY        = 3,
    V4L2_BUF_TYPE_VBI_CAPTURE          = 4,
    V4L2_BUF_TYPE_VBI_OUTPUT           = 5,
    V4L2_BUF_TYPE_SLICED_VBI_CAPTURE   = 6,
    V4L2_BUF_TYPE_SLICED_VBI_OUTPUT    = 7,
#if 1
    /* Experimental */
    V4L2_BUF_TYPE_VIDEO_OUTPUT_OVERLAY = 8,
#endif
    V4L2_BUF_TYPE_PRIVATE              = 0x80,
};


#if 0
struct v4l2_pix_format {
    U32               width;
    U32           height;
    U32           pixelformat;
    enum v4l2_field     field;
    U32               bytesperline;   /* for padding, zero if unused */
    U32               sizeimage;
    enum v4l2_colorspace    colorspace;
    U32           priv;       /* private data, depends on pixelformat */
};

/*  Stream data format
 */
struct v4l2_format {
    enum v4l2_buf_type type;
    union {
        struct v4l2_pix_format      pix;     /* V4L2_BUF_TYPE_VIDEO_CAPTURE */
//        struct v4l2_window      win;     /* V4L2_BUF_TYPE_VIDEO_OVERLAY */
//        struct v4l2_vbi_format      vbi;     /* V4L2_BUF_TYPE_VBI_CAPTURE */
//        struct v4l2_sliced_vbi_format   sliced;  /* V4L2_BUF_TYPE_SLICED_VBI_CAPTURE */
        U8    raw_data[200];                   /* user-defined */
    } fmt;
};


/* Routing definition, device dependent. It specifies which inputs (if any)
   should be routed to which outputs (if any). */
struct v4l2_routing {
    U32 input;
    U32 output;
    enum tvp514x_std current_std;
	v4l2_std_id current_norm;
    U32 capabilities;   /* Device capabilities */

    int ver;
    enum tvp514x_state state;

    struct v4l2_pix_format pix;
    int num_fmts;
    const struct v4l2_fmtdesc *fmt_list;

    enum tvp514x_std current_std;
    int num_stds;
    struct tvp514x_std_info *std_list;
};

/* List of image formats supported by TVP5146/47 decoder
 * Currently we are using 8 bit mode only, but can be
 * extended to 10/20 bit mode.
 */
static const struct v4l2_fmtdesc tvp514x_fmt_list[] = {
    {
     .index = 0,
     .type = V4L2_BUF_TYPE_VIDEO_CAPTURE,
     .flags = 0,
     .description = "8-bit UYVY 4:2:2 Format",
     .pixelformat = V4L2_PIX_FMT_UYVY,
    },
};
#endif

#endif				/* ifndef _TVP514X_REGS_H */
