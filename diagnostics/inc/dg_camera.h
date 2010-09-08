/*
 * dg_camera.h: Camera diagnostic header file.
 *
 * (C) Copyright 2010
 * Mistral Solutions Private Limited, <www.mistralsolutions.com>
 *
 * Author :
 *	Ganeshan <nganeshan@mistralsolutions.com>
 *
 * Derived from CSST
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

#ifndef DG_CAMERA_H
#define DG_CAMERA_H

#ifdef __cplusplus
extern "C" {
#endif


#define SET 1

#define CAM_TCTRL_CTRL 0x480BC050
#define CAMERA_I2C_ADDR	(0x3c)

#define CAM_XCLKA  24000000

#define DISP_CMP_TARGET_BOARD_INFO_REQ               0x1
#define DISP_CMP_TARGET_BOARD_INFO_CNF               0x2
#define DISP_CMP_MODULE_INFO_REQ                     0x3
#define DISP_CMP_MODULE_INFO_CNF                     0x4
#define DISP_CMP_ABORT_REQ                           0x5
#define DISP_CMP_ABORT_CNF                           0x6
#define DISP_CMP_UART_CHANGE_PARAM_REQ               0x7
#define DISP_CMP_UART_CHANGE_PARAM_CNF               0x8

#define DISP_CMP_MODULE_ID_ERROR                     0x9

/*related to diagnostics */
/*init function return values */
#define DIAG_INIT_PASS            1
#define DIAG_INIT_FAIL 			 -1

/*service query handler return value*/
#define DIAG_SQ_PASS  			  1
#define DIAG_SQ_ERROR 			 -1


/*diag send response return value */
#define DIAG_SEND_PASS            1
#define DIAG_SEND_FAIL            0

/*related to dispatcher*/
#define DISP_REG_SUCCESS        1
#define DISP_REG_FAILURE        0

#define DISP_SEND_FAILURE     0x0
#define DISP_SEND_SUCCESS     0x1

#define DISP_UNREG_FAILURE    0x0
#define DISP_UNREG_SUCCESS    0x1

/*related to download*/
#define DNLD_INIT_PASS			  1
#define DNLD_INIT_FAIL 			 -1

#define RESULT_OK      1
#define RESULT_NOT_OK  0


/****** Other stuff *****/
#define CAM_TEST_STREAMING 0x0
#define CAM_TEST_ONESHOT   0x1

#define LCD_HEIGHT_QVGA    320
#define LCD_WIDTH_QVGA     240
#define LCD_HEIGHT_VGA     640
#define LCD_WIDTH_VGA      480

#define LCD_HEIGHT_QSXGA   2592
#define LCD_WIDTH_QSXGA    1944
#define LCD_HEIGHT_QXGA    2048
#define LCD_WIDTH_QXGA     1536
#define LCD_HEIGHT_XGA     1024
#define LCD_WIDTH_XGA      768

#define CAM_TEST_TRUE      1
#define CAM_TEST_FALSE     0

/************ Define the Number of Boards available */
#define CAM_BOARD_TYPE1   1
#define CAM_BOARD_TYPE2   2
#define CAM_BOARD_TYPE3   3
#define CAM_BOARD_TYPE4   4
#define CAM_BOARD_TYPE5   5
#define CAM_BOARD_TYPE6   6
#define CAM_BOARD_TYPE7   7
#define CAM_BOARD_TYPE8   8

/* Data Paths taken by sensor */
#define CAMERA_DATA_PATH_PARALLEL       0
#define CAMERA_DATA_PATH_CSI1           1
#define CAMERA_DATA_PATH_CSI2           2

/* Which OMAP clock does the sensor depend on? */
#define CAMERA_CLOCK_XCLKA              1
#define CAMERA_CLOCK_XCLKB              2


/* Auto focus options */
#define CAMERA_AUTO_FOCUS_DISABLE         0
#define CAMERA_AUTO_FOCUS_MACRO           1
#define CAMERA_AUTO_FOCUS_INFINITE        2
#define CAMERA_AUTO_FOCUS_MIDLEVEL        3
#define CAMERA_AUTO_FOCUS_AUTO            4

/* Flash operation */
#define CAMERA_FLASH_DISABLE              0
#define CAMERA_FLASH_ENABLE               1

#define CAM_NO_OF_FLASHOPTIONS 2
#define CAM_NO_OF_AUTOFOCUS_OPTIONS 5

#define OMAP3430SDP_CHMN
#ifdef OMAP3430SDP_CHMN
#define CAM_NO_OF_BOARDS 1
#define CAM_NO_OF_SENSORS 2
#else
#define CAM_NO_OF_BOARDS 6
#define CAM_NO_OF_SENSORS 2
#endif /*OMAP3430SDP_CHMN*/

/* For the base loop display*/
#define SPACE_ASCII_VAL             0x20

#define LCD_MAX_REG_CNT             8

#define LCD_REG_BASE                (0x08000040)

#define MPDB_FPGA_BASE_ADDR         0x08000000
#define SW10_DIP_SWITCH_REG_ADDR    (MPDB_FPGA_BASE_ADDR + 0x0062)
#define SW14_DIP_SWITCH_REG_ADDR    (MPDB_FPGA_BASE_ADDR + 0x0064)

#define SW_ON                       0
#define SW_OFF                      1

#define SW_LCD_VGA_QVGA_SEL         0x8000
#define SW_LCD_VGA_QVGA_SEL_RSHIFT  15

#define SW_LCD_DVI_SEL              0x4000
#define SW_LCD_DVI_SEL_RSHIFT       14

#define SW_LCD_24BIT_MD             0x1000
#define SW_LCD_24BIT_MD_RSHIFT      12

#define SW_LCD_16BIT_N18BIT         0x2000
#define SW_LCD_16BIT_N18BIT_RSHIFT  13

#define SW_HDMI_24BIT_MD            0x8000
#define SW_HDMI_24BIT_MD_RSHIFT     12

#define SW_HDMI_18BIT_MD            0x4000
#define SW_HDMI_18BIT_MD_RSHIFT     12

#define SW_HDMI_SEL                 0x2000
#define SW_HDMI_SEL_RSHIFT          12

#define SW_LCD_18BIT_MD             0x4000
#define SW_LCD_18BIT_MD_RSHIFT      12

/* LCD */
#define LCD_MODE_VGA                 0x1
#define LCD_MODE_QVGA                0x2

#define DAL_EDISCO_PID          52
#define DAL_LCD_PID             19
#define DAL_CAMERA_PID          25

#define LCD_TAG_BASE                     0
#define LCD_BITS_PER_PIXEL              (LCD_TAG_BASE + 4)
#define LCD_REFRESH_RATE                (LCD_TAG_BASE + 5)
#define LCD_IMAGE_HEIGHT                (LCD_TAG_BASE + 6)
#define LCD_IMAGE_WIDTH                 (LCD_TAG_BASE + 8)
#define LCD_IMAGE_ADDRESS               (LCD_TAG_BASE + 9)
#define LCD_START_DISPLAY               (LCD_TAG_BASE + 13)
#define LCD_TFTDATA_LINES               (LCD_TAG_BASE + 17)
#define LCD_BACKGROUND_COLOR            (LCD_TAG_BASE + 21)
#define LCD_GFX_TRANSPEARENCY_COLOR_KEY (LCD_TAG_BASE + 25)
#define LCD_ROTATION                    (LCD_TAG_BASE + 29)
#define LCD_XY_POSITION                 (LCD_TAG_BASE + 33)
#define LCD_ROW_INC                     (LCD_TAG_BASE + 37)
#define LCD_REG_WRITE                   (LCD_TAG_BASE + 38)

#define LCD_REG_READ                    (LCD_TAG_BASE + 0)

#define LCD_MAX_SID                     1

#define GFX_PIPELINE            0x0
#define V1_PIPELINE             0x1
#define V2_PIPELINE             0x2


#define _PD_SETPIXEL(x,y,c) ts_lcd_buf_ptr[y * pd_lcd_width + x] = c

/*image formats*/
#define BITMAP1             0x0
#define BITMAP2             0x1
#define BITMAP4             0x2
#define BITMAP8             0x3
#define RGB12               0x4
#define RGB16               0x6
#define RGB24               0x8
#define YUV422              0xA
#define UYVY422             0xB
/***************pixel formats******************/

#define  YUV422BE      0x0      //CC_CCPDFR[3:0]
//#define  YUV422        0x1
#define  YUV420        0x2
#define  RGB444        0x4
#define  RGB565        0x5
#define  RGB888_NDE    0x6
#define  RGB888_DE     0x7
#define  RAW8_NDE      0x8
#define  RAW8_DE       0x9
#define  RAW10_NDE     0xA
#define  RAW10_DE      0xB
#define  RAW12_NDE     0xC
#define  RAW12_DE      0xD
#define  JPEG8_FSP     0xE
#define  JPEG8         0xF
#define  RGB666        0x10
#define  RGB888        0x11

/*image types*/

#define SQCIF               0x0
#define QCIF                0x1
#define CIF             0x2
#define VGA             0x3
#define SVGA                0x4
#define XGA             0x5
#define SXGA                0x6
#define UXGA                0x7
#define QSIF                0x8
#define SIF             0x9
#define QVGA                0xA
#define HDMI                0xB
#define RFBI_QVGA                       0xC
/*Specific to LCD*/
#define DVI             0xFF

#define LCD_QVGA        0x0
#define LCD_VGA         0x1
#define DVI_VGA         0x2
#define DVI_720P        0x3
/*standard resolution*/
#define LCD_HEIGHT_QVGA   320
#define LCD_WIDTH_QVGA    240
#define LCD_HEIGHT_VGA    640
#define LCD_WIDTH_VGA     480
/* lcd interface width */
#define LCD_WIDTH_18BIT             0x1
#define LCD_WIDTH_16BIT             0x2

#define YUV_TO_RGB_CONV_ENABLE  1
#define YUV_TO_RGB_CONV_DISABLE 2

#define  QCIF_PAL           (176*144)
#define  QCIF_NTPC          (176*120)
#define  QVGA_PAL_POTRAIT       (240*320)   /* QVGA in Portrait */
#define  QVGA_PAL_LANDSCAPE     (320*240)   /* QVGA in Landscape */
#define  VGA_LANDSCAPE          (640*480)

/* lcd display colors */
#define RED_COLOR       0xF800
#define GREEN_COL0R     0x07E0
#define BLUE_COLOR      0x001F
#define YELLOW_COLOR            0xFFE0
#define WHITE_COLOR     0xFFFF
#define BLACK_COLOR     0x0000
#define NO_COLOR                0xFEFE

/* Display endianess */
#define LITTLE_ENDIAN   0x0
#define BIG_ENDIAN      0x1

/*camera elated stuff*/
#define CAM_PREVIEW 0x0
#define CAM_ONESHOT 0x1

#define I2C_100K_CLK        1 /* Stardard mode  - 100 kbits/s */
#define I2C_400K_CLK        2 /* Fast mode  - 400 kbits/s */
#define I2C_1P95M_CLK       3 /* High Speed - 1.95 Mbits/s */
#define I2C_2P6M_CLK        4 /* High Speed - 2.6 Mbits/s */
#define I2C_3P4M_CLK        5 /* High Speed - 3.4 Mbits/s */

#define CAMERA_GPIO_NUM(val) ((val) & 0xFF)
#define CAMERA_GPIO_POLARITY_STR(val) (((val) & 0x100)?"High":"Low")


/* This currently validates Sdu Handle by checking whether it is NULL or not.
If a SDU function is provided in future to validate SDU Handle, this
macro needs to be modified */
#define VALIDATE_SDU_HANDLE(SduHandle) ((SduHandle != (U32)NULL) ? TRUE : FALSE)



#define GPIO_177                  177

#define GPIO_QVGA_nVGA_SEL        GPIO_177

#ifndef _MANAGED
#ifndef NULL
#define NULL    ((void *)0)
#endif
#endif

/***********cam poll/interrupt mode**************/

#define  CAM_SID1         1
#define  CAM_SID2         2
#define  CAM_SID3         3
#define  CAM_SID4         4

#define  CAM_SID          CAM_SID1

#define  CAM_POLL_EN       0x01
#define  CAM_INTERRUPT_EN  (!CAM_POLL_EN)

/* These macros in lower case (exception) since these will be changed to function
once mmu comes into picture  */
#define in_regb(offSet)         (*(PREG_U8)(offSet))
#define in_regs(offSet)         (*(PREG_U16)(offSet))
#define in_regl(offSet)         (*(PREG_U32)(offSet))
#define out_regb(offSet, value)        (*(PREG_U8)(offSet) = (U8)(value))
#define out_regs(offSet, value)       (*(PREG_U16)(offSet) = (U16)(value))
#define out_regl(offSet, value)       (*(PREG_U32)(offSet) = (U32)(value))


typedef struct
{
    U16 pid;
    U16 sid;

    U32 image_type;
    U32 buf_base_adr;
    U8 pixel_format;//CC_CCPDFR[3-0];
    U8 capture_rate;
    U8 capture_mode;
    U8 poll_interrupt;
    void (* frame_end_irq_callback)(void *data);
    void * data;
    void * sensor_private;

}T_CAM_INIT;


/* Diagnostics Specific Structures *****/
/* Structure to store user entry Vs param mapping */
typedef struct S_DGCAM_PARAMS
{
    char *option;
    U8 id;
    char *help_string;
} T_DGCAM_PARAMS;

/* Structure to store the params the test suite needs to remember */
typedef struct S_DGCAM_CAM_PARAMS
{
    U8 init_state;
    U32 handle;
    U32 lcd_handle;
    /* Stored params */
    U8 board_id;
    U8 cam_sid;
    U8 flash_control;
    U8 auto_focus;
} T_DGCAM_CAM_PARAMS;

#define     DAL_MAX_DEVICES                     60//30
#define     DAL_MAX_DEVICE_HANDLE               10

/*void * is used for init_str, since the initialization structures
for various devices are different
same for the other paramters also*/

#define     DDDT                                dal_device_driver_table
#define     DDHT                                dal_device_handle_table
#define     DAL_DIRTY_RECORD                    0xFFFFFFFF

/************* Auto focus logic **********/
/* Not implemented by camera core.. But implemented by a/f logic per sensor */
/* For use for Camera autofocus mechanism */
typedef struct S_CAM_AF_DEV
{
    U32 i2c_sid;                /* bus address */
    U8 slave_address;
    U32 i2c_clk_speed;
    U32 freq_mclk;              /* in khz */
    void *af_private;
} T_CAM_AF_DEV;

typedef struct S_CAMCORE_AUTOFOCUS
{
    char *af_name;
    T_CAM_AF_DEV af_dev;
    S32(*camaf_init) (T_CAM_AF_DEV * af_dev);
    S32(*camaf_deinit) (T_CAM_AF_DEV * af_dev);
    S32(*camaf_setfocus) (T_CAM_AF_DEV * af_dev, U8 posn);
} T_CAMCORE_AUTOFOCUS;


typedef struct S_CAMERA_SENSOR_PRIVATE_PARAMS
{
    U8 data_pattern;
    U8 flash_control;
    U8 auto_white_balance;
    U8 auto_focus;
    U8 data_path;               /* CSI2, CSI1 or CPP2 */
    U8 xclk;                    /* XCLKA or XCLKB */
    U8 mux_cfg;                 /* MUX CFG1/2 */
    U16 gpio_reset;             /* Lower U8 for GPIO number, higher U8 for Polarity for Reset assertion */
    U16 gpio_powerdn;           /* Lower U8 for GPIO number, higher U8 for polarity for flash assertion */
    U16 gpio_torch;             /* Lower U8 for GPIO number, higher U8 for polarity for torch assertion */
    U16 gpio_flash;             /* Lower U8 for GPIO number, higher U8 for polarity for flash assertion */
    U16 sensor_i2c_bus;
    U16 sensor_i2c_address;
    U16 sensor_i2c_speed;
    T_CAMCORE_AUTOFOCUS *af_device;
} T_CAMERA_SENSOR_PRIVATE_PARAMS;


static const T_DGCAM_PARAMS dgcamera_flash[] = {
    {"disable", CAMERA_FLASH_DISABLE, "Disable Flash"},
    {"enable", CAMERA_FLASH_ENABLE, "Enable Flash"},
};

static const T_DGCAM_PARAMS dgcamera_auto_focus[] = {
    {"disable", CAMERA_AUTO_FOCUS_DISABLE, "No Autofocus"},
    {"macro", CAMERA_AUTO_FOCUS_MACRO, "Close up focus"},
    {"intermediate", CAMERA_AUTO_FOCUS_MIDLEVEL, "intermediate focus"},
    {"infinite", CAMERA_AUTO_FOCUS_INFINITE, "Inifinite focus"},
    {"auto", CAMERA_AUTO_FOCUS_AUTO, "Automatic Focus control"},
};

static const T_DGCAM_PARAMS dgcamera_boards[] = {
    {"v1", CAM_BOARD_TYPE1, "3430v1.0 Mistral cam module"},
};

/************ Define the Number of Sensors available */

static const T_DGCAM_PARAMS dgcamera_sensors[] = {
    {"micron", CAM_SID1, "Micron MT9T111"},
};

#define SD_LCD_BUFFER    0x80377000
#define SD_RGB24_BUFFER  0x80177000

/* OMAP3 GPIO registers */
#define OMAP34XX_GPIO1_BASE     0x48310000
#define OMAP34XX_GPIO2_BASE     0x49050000
#define OMAP34XX_GPIO3_BASE     0x49052000
#define OMAP34XX_GPIO4_BASE     0x49054000
#define OMAP34XX_GPIO5_BASE     0x49056000
#define OMAP34XX_GPIO6_BASE     0x49058000

#define GPIO_OE             	0x34
#define GPIO_CLEARDATAOUT       0x90
#define GPIO_SETDATAOUT         0x94

#define GPIO0               (0x1 << 0)
#define GPIO1               (0x1 << 1)
#define GPIO2               (0x1 << 2)
#define GPIO3               (0x1 << 3)
#define GPIO4               (0x1 << 4)
#define GPIO5               (0x1 << 5)
#define GPIO6               (0x1 << 6)
#define GPIO7               (0x1 << 7)
#define GPIO8               (0x1 << 8)
#define GPIO9               (0x1 << 9)
#define GPIO10              (0x1 << 10)
#define GPIO11              (0x1 << 11)
#define GPIO12              (0x1 << 12)
#define GPIO13              (0x1 << 13)
#define GPIO14              (0x1 << 14)
#define GPIO15              (0x1 << 15)
#define GPIO16              (0x1 << 16)
#define GPIO17              (0x1 << 17)
#define GPIO18              (0x1 << 18)
#define GPIO19              (0x1 << 19)
#define GPIO20              (0x1 << 20)
#define GPIO21              (0x1 << 21)
#define GPIO22              (0x1 << 22)
#define GPIO23              (0x1 << 23)
#define GPIO24              (0x1 << 24)
#define GPIO25              (0x1 << 25)
#define GPIO26              (0x1 << 26)
#define GPIO27              (0x1 << 27)
#define GPIO28              (0x1 << 28)
#define GPIO29              (0x1 << 29)
#define GPIO30              (0x1 << 30)
#define GPIO31              (0x1 << 31)

#define OFFS(x)             ((x) >> 2)

/* camera clock */
#define CM_CAM_MCLK_HZ          216000000
#define ISPTCTRL_CTRL_DIV_LOW       0x0
#define ISPTCTRL_CTRL_DIV_HIGH      0x1
#define ISPTCTRL_CTRL_DIV_BYPASS    0x1F

#ifdef __cplusplus
}
#endif

#endif /* DG_CAMERA_H */

