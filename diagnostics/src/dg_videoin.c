/*
 * video input test file.
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

#include <common.h>
#include "types.h"
#include <asm/io.h>
#include "../inc/dg_videoin.h"
#include "../../common/inc/dg_gpio.h"
#include "../inc/ispregs.h"
#include "../inc/dg_lcd.h"
#include "../../drivers/inc/lcd_drv.h"


#define CAM_DEBUG	1

/*
 * Macros
 */
/* Source composite */
#define ITBOK_VIDEOIN_COMPOSITE		0
/* Source S-Video */
#define ITBOK_VIDEOIN_SVIDEO		1

#define VIDEOIN_DISPLAY_WIDTH 		480
#define VIDEOIN_DISPLAY_HEIGHT		640


#define SD_VIDEOIN_BUFFER1 0x80530000
/* The length is calculate as 720 * 640 * 2 */
#define SD_VIDEOIN_BUFFER2 0x80611000


#define USE_RAW
//#undef USE_RAW

#define COLOR_BAR
#undef COLOR_BAR

#define USE_YUV422
#undef USE_YUV422

#define USE_RGB565
#undef USE_RGB565

#define DEBUG_ISP   0


#define SD_LCD_BUFFER    0x80377000
#define SD_RGB24_BUFFER  0x80177000

static unsigned char *vga_map_data;
unsigned int videoin_lcd;

extern int i2c_write(uchar, uint, int, uchar *, int);
extern int i2c_read(uchar, uint, int, uchar *, int);
extern void tvp5146_power_off(void);
extern void videoin_i2c_config(void);
extern int tvp5146_initialize(void);
extern void videoin_isp_init(void);
extern int videoin_capture_display_image(void);
extern void videoin_isp_enable_irq(void);
extern void videoin_ispccdc_enable(int);


/* ISP register read write functions */
#define ispReadb(a)         (*(volatile unsigned char  *)(a))
#define ispReadw(a)         (*(volatile unsigned short *)(a))
#define ispReadl(a)         (*(volatile unsigned int   *)(a))
#define ispWriteb(v,a)      (*(volatile unsigned char  *)(a) = (v))
#define ispWritew(v,a)      (*(volatile unsigned short *)(a) = (v))
#define ispWritel(v,a)      (*(volatile unsigned int   *)(a) = (v))

#define RGB24_TO_RGB16(red, green, blue) ( ((red >> 3) << 11) | ((green >> 2) << 5) | (blue >> 3))

/* I2C power management companion definitions */
#define PWRMGT_ADDR_ID1     0x48
#define PWRMGT_ADDR_ID2     0x49
#define PWRMGT_ADDR_ID3     0x4A
#define PWRMGT_ADDR_ID4     0x4B

/* I2C ID2 (slave2) registers */
#define  GPIODATADIR1_ID2   0x9B
#define  GPIODATADIR2_ID2   0x9C

#define  GPIODATAIN1_ID2    0x98
#define  GPIODATAIN2_ID2    0x99

#define  GPIODATAOUT1_ID2   0x9E
#define  GPIODATAOUT2_ID2   0x9F

#define  GPIOCTRL_ID2  		0xAA
#define  GPIOPUPDCTR1_ID2   0xAB
#define  GPIOPUPDCTR2_ID2   0xAC
#define  GPIOPUPDCTR3_ID2   0xAD

#include <asm/arch/mux.h>
#define BIT(x)      (1 << (x))

void videoin_gpio_dir_set(void)
{
	unsigned char byte;

	/*
	 * OMAP GPIO Direction settings
	 */
    (*(volatile unsigned int *) (OMAP34XX_GPIO4_BASE + GPIO_OE)) = 0xffffbffb;

	/* GPIO 54 set DIR set as output */
	writel(readl(OMAP34XX_GPIO2_BASE + GPIO_OE) & ~(GPIO22), OMAP34XX_GPIO2_BASE + GPIO_OE);

	/* GPIO 134 set DIR set as output */
	writel(readl(OMAP34XX_GPIO5_BASE + GPIO_OE) & ~(GPIO6), OMAP34XX_GPIO5_BASE + GPIO_OE);

	/* GPIO 136 set DIR set as output */
	writel(readl(OMAP34XX_GPIO5_BASE + GPIO_OE) & ~(GPIO8), OMAP34XX_GPIO5_BASE + GPIO_OE);

	/* GPIO_157 for Camera. make it output and drive low */
	writel(readl(OMAP34XX_GPIO5_BASE + GPIO_OE) & ~(GPIO29), OMAP34XX_GPIO5_BASE + GPIO_OE);

	/*
	 * T2 GPIO Direction settings
	 */
	/* Select GPIO mode */
	byte = 0x04;
	i2c_write(PWRMGT_ADDR_ID2, GPIOCTRL_ID2, 1, &byte, 1);

	/* GPIO_2 of T2 set as OUTPUT */
	byte = 0;
	i2c_read(PWRMGT_ADDR_ID2, GPIODATADIR1_ID2, 1, &byte, 1);
	byte |= 0x04;
	i2c_write(PWRMGT_ADDR_ID2, GPIODATADIR1_ID2, 1, &byte, 1);

	/* GPIO_8 of T2 set as OUTPUT */
	byte = 0;
	i2c_read(PWRMGT_ADDR_ID2, GPIODATADIR2_ID2, 1, &byte, 1);
	byte |= 0x01;
	i2c_write(PWRMGT_ADDR_ID2, GPIODATADIR2_ID2, 1, &byte, 1);

}

void videoin_gpio_set(void)
{

	unsigned char byte;
	unsigned int gpioData;

	/* setting GPIO 157 : Drive HIGH */
    /* GPIO 157 - Drive High */
    gpioData = (*(volatile unsigned int *) (OMAP34XX_GPIO5_BASE + GPIO_DATAOUT));
    (*(volatile unsigned int *) (OMAP34XX_GPIO5_BASE + GPIO_DATAOUT)) = gpioData | 0x20000000;

	/* GPIO_2 of T2 : Drive LOW */
	byte = 0;
	i2c_read(PWRMGT_ADDR_ID2, GPIODATAOUT1_ID2, 1, &byte, 1);

	byte &= 0xFB;
	i2c_write(PWRMGT_ADDR_ID2, GPIODATAOUT1_ID2, 1, &byte, 1);

	/* GPIO_8 of T2 : Drive LOW */
	byte = 0;
	i2c_read(PWRMGT_ADDR_ID2, GPIODATAOUT1_ID2, 1, &byte, 1);

	byte &= 0xFE;
	i2c_write(PWRMGT_ADDR_ID2, GPIODATAOUT1_ID2, 1, &byte, 1);

	/* Set PULL UP Register GPIOPUPDCTR1 */
	byte = 0xAA;
	i2c_write(PWRMGT_ADDR_ID2, GPIOPUPDCTR1_ID2, 1, &byte, 1);
	i2c_write(PWRMGT_ADDR_ID2, GPIOPUPDCTR2_ID2, 1, &byte, 1);
	i2c_write(PWRMGT_ADDR_ID2, GPIOPUPDCTR3_ID2, 1, &byte, 1);

	udelay(200);

	/* **************** GPIO_98 MT9T111_RESET_GPIO  ********************** */
	/* set GPIO_98  OUPUT register as High */
	gpioData = readl(OMAP34XX_GPIO4_BASE + GPIO_DATAOUT);
	writel(gpioData | BIT(2), OMAP34XX_GPIO4_BASE + GPIO_SETDATAOUT);

	udelay(8000);
	udelay(8000);
	udelay(8000);
	udelay(8000);

	/* set GPIO_98 to high in CLEARDATAOUT register to set low */
	gpioData = readl(OMAP34XX_GPIO4_BASE + GPIO_DATAOUT);
	writel(gpioData & ~BIT(2), OMAP34XX_GPIO4_BASE + GPIO_DATAOUT);
	udelay(8000);
	udelay(8000);
	udelay(8000);
	udelay(8000);
	udelay(8000);
	udelay(8000);

	/* set GPIO_98 to high  in SETDATAOUT register */
	gpioData = readl(OMAP34XX_GPIO4_BASE + GPIO_SETDATAOUT);
	writel(gpioData | BIT(2), OMAP34XX_GPIO4_BASE + GPIO_SETDATAOUT);

	udelay(8000);
	udelay(8000);
	udelay(8000);
	udelay(8000);
	udelay(8000);
	udelay(8000);
}

/*
 * Function Name :
 */
void tvp5146_cam_init(void)
{
	unsigned int gpioData;

	/* Disable TVP5146 Video in (GPIO134 = 1) */
	gpioData = readl(OMAP34XX_GPIO5_BASE + GPIO_SETDATAOUT);
	writel(gpioData | BIT(6), OMAP34XX_GPIO5_BASE + GPIO_SETDATAOUT);

	/* Disable Exapansion Camera Video in (GPIO54 = 1) */
	gpioData = readl(OMAP34XX_GPIO2_BASE + GPIO_SETDATAOUT);
	writel(gpioData | BIT(22), OMAP34XX_GPIO2_BASE + GPIO_SETDATAOUT);

	/* Enable Camera Video in (GPIO136 = 0) */
	gpioData = readl(OMAP34XX_GPIO5_BASE + GPIO_DATAOUT);
	writel(gpioData & ~BIT(8), OMAP34XX_GPIO5_BASE + GPIO_DATAOUT);

	udelay(200);

	/* **************** GPIO_167 MT9T111_STANDBY_GPIO ********************** */
	/* set GPIO_167  OUPUT register as low */
	gpioData = readl(OMAP34XX_GPIO6_BASE + GPIO_OE);
	writel(gpioData & ~BIT(7), OMAP34XX_GPIO6_BASE + GPIO_OE);
	udelay(8000);
	udelay(8000);

	/* GPIO  167 drive high */
	gpioData = readl(OMAP34XX_GPIO6_BASE + GPIO_SETDATAOUT);
	writel(gpioData | BIT(7), OMAP34XX_GPIO6_BASE + GPIO_SETDATAOUT);
	udelay(100);

	/* **************** GPIO_110 ********************** */

	/* set GPIO_110  OUPUT register as low */
	gpioData = readl(OMAP34XX_GPIO4_BASE + GPIO_OE);
	writel(gpioData & ~BIT(14), OMAP34XX_GPIO4_BASE + GPIO_OE);
	udelay(8000);
	udelay(8000);

	/* GPIO  110 drive high */
	gpioData = readl(OMAP34XX_GPIO4_BASE + GPIO_SETDATAOUT);
	writel(gpioData | BIT(14), OMAP34XX_GPIO4_BASE + GPIO_SETDATAOUT);
	udelay(100);
	udelay(100);
}

/*************************************************************************/
/* Enable interface and functional clock */
static void videoin_enable_clock(void)
{
	/* Clock setting */
	writel(readl(CM_CLKSEL_CAM) | 0x4, CM_CLKSEL_CAM);
	writel(readl(CM_FCLKEN_CAM) | 0x3, CM_FCLKEN_CAM);
	writel(readl(CM_ICLKEN_CAM) | 0x1, CM_ICLKEN_CAM);
	writel(readl(PM_PWSTCTRL_CAM) | 0x30107, PM_PWSTCTRL_CAM);
}

/* Set the appropriate pin muxing */
static void videoin_pin_muxing(void)
{
    unsigned int gpioData;
    printf ("tvp5146 Mux setting GPIO\n");

    /* GPIO 157 */
    gpioData = (*(volatile unsigned int *)0x4800218C);
    (*(volatile unsigned int *)0x4800218C) = gpioData | 0x011c0000;


    /* GPIO 98 */
    gpioData = (*(volatile unsigned int *)0x48002114);
    (*(volatile unsigned int *)0x48002114) = gpioData | 0x0000011c;

    /* GPIO 54 */
    gpioData = (*(volatile unsigned int *)0x480020b4);
    (*(volatile unsigned int *)0x480020b4) = gpioData | 0x0000011c;

    /* GPIO 134 */
    gpioData = (*(volatile unsigned int *)0x48002160);
    (*(volatile unsigned int *)0x48002160) = gpioData | 0x0000011c;

    /* GPIO 136 */
    gpioData = (*(volatile unsigned int *)0x48002164);
    (*(volatile unsigned int *)0x48002164) = gpioData | 0x0000011c;

	/* GPIO 110 Mux settings */
	MUX_VAL(CP(CAM_D11), (IEN | PTU | EN | M4));
}

/* Enabling TVP5146 decoder */
static void videoin_decoder_enable(void)
{

	videoin_gpio_dir_set();
	tvp5146_cam_init();
	videoin_gpio_set();
}

/* Cleanup if any before quiting the test */
void videoin_reset(void)
{

	tvp5146_power_off();
}

/********************************************************************/
/* Test Entry Point */
void dg_videoin_test(void)
{
	int ret_val;

	videoin_i2c_config();

	/* Enable interface and functional clock */
	videoin_enable_clock();

	/* Set the appropriate pin muxing */
	videoin_pin_muxing();

	/* Enabling TVP5146 decoder */
	videoin_decoder_enable();
	udelay(8000);

	/* Initialize I2C3 */

	udelay(8000);
	udelay(8000);
	udelay(8000);
	udelay(8000);
	udelay(8000);
	udelay(8000);
	udelay(8000);
	udelay(8000);
	udelay(8000);
	/* Configuring the TVP5146 for Composite or S-Video */
	ret_val = tvp5146_initialize();

	/* Configure CCDC */
	videoin_isp_init();

	/* capture and display */
	videoin_capture_display_image();

	/* Cleanup if any before quiting the test */
//	videoin_reset();
}

/*********************** DISPLAY SECTION *******************************/

unsigned int buff_switch = 0;

void videoin_poll_interrupt(void)
{
    u32 u32IrqStatus = 0;
	U32 u32Val;

        u32IrqStatus = ispReadl(ISP_IRQ0STATUS);

        /* Clear the IRQ0 status */
        ispWritel(u32IrqStatus, ISP_IRQ0STATUS);
        do {
            if ((u32IrqStatus & CCDC_VD0) == CCDC_VD0) {
#if DEBUG_ISP
                printf("Received VD0 buffer interrupt\n");
#endif
                break;
            }
            u32IrqStatus = ispReadl(ISP_IRQ0STATUS);
            ispWritel(u32IrqStatus, ISP_IRQ0STATUS);
        } while (1);

#if DEBUG_ISP
        printf("IRQ status %x .........\n", u32IrqStatus);
#endif
        /* Added to clear post write interrupt */
        u32IrqStatus = ispReadl(ISP_IRQ0STATUS);

        /* Clear the IRQ0 status */
        ispWritel(u32IrqStatus, ISP_IRQ0STATUS);
        u32IrqStatus = 0;

	if (0 == buff_switch) {
		u32Val = SD_VIDEOIN_BUFFER2;
		ispWritel(u32Val, CCDC_SDR_ADDR);
		buff_switch = 1;
	}
	else if (1 == buff_switch) {
		u32Val = SD_VIDEOIN_BUFFER1;
		ispWritel(u32Val, CCDC_SDR_ADDR);
		buff_switch = 0;
	}
}



void videoin_set_copy_buf(void)
{
    U32 ypixel;
    unsigned long int i;

    unsigned char *image_address_buffer;
    U16 *image_address_camera = NULL;
    unsigned char value;

	if (0 == buff_switch) {
    	image_address_camera = (U16 *) SD_VIDEOIN_BUFFER1;
	}
	else if (1 == buff_switch) {
    	image_address_camera = (U16 *) SD_VIDEOIN_BUFFER2;
	}

    image_address_buffer = (unsigned char *)SD_LCD_BUFFER;

    bm_lcd_width = VIDEOIN_DISPLAY_WIDTH;
    bm_lcd_height = VIDEOIN_DISPLAY_HEIGHT;
    i = 0;
    for (ypixel = 0; ypixel < (bm_lcd_height * bm_lcd_width); ypixel++) {
        value = (unsigned char)((image_address_camera[ypixel] >> 2) & 0xff);
        image_address_buffer[i] = value;
        image_address_buffer[i + 1] = value;
        image_address_buffer[i + 2] = value;
        i += 3;
    }
}

void videoin_rgb24_to_rgb16(void)
{
    U32 x, i;

    unsigned char *image_address_buffer = NULL;
#if 1
    unsigned char *image_address_lcd = NULL;
    image_address_lcd = (unsigned char *)SD_LCD_BUFFER;
#else
    unsigned short *image_address_lcd = NULL;
    image_address_lcd = (unsigned short *)SD_LCD_BUFFER;
#endif
    image_address_buffer = (unsigned char *)SD_RGB24_BUFFER;
    i = 0;
#if DEBUG_ISP
    printf("SD_RGB24_BUFFER: 0x%x SD_LCD_BUFFER 0x%x\n", SD_RGB24_BUFFER, SD_LCD_BUFFER);
#endif

    for (x = 0; x < (VIDEOIN_DISPLAY_WIDTH * VIDEOIN_DISPLAY_HEIGHT); ++x) {
#if 1
        image_address_lcd[i] = image_address_buffer[i];
        image_address_lcd[i + 1] = image_address_buffer[i + 1];
        image_address_lcd[i + 2] = image_address_buffer[i + 2];
        i += 3;
#else
        if ((image_address_buffer[i + 2] & 0x0f) == image_address_buffer[i + 2])
            value = (0xFFFFFF) & (((((image_address_buffer[i + 2] << 16) + (image_address_buffer[i + 1] << 12)) >> 4) + (image_address_buffer[i] << 4)) >> 4);
        else
            value = (0xFFFFFF) & (image_address_buffer[i + 2] << 16) + (image_address_buffer[i + 1] << 8) + image_address_buffer[i];

        val8 = &value;
        image_address_lcd[x] = RGB24_TO_RGB16(val8[2], val8[1], val8[0]);
        i += 3;
        if (x < 10)
            printf("0x%x 0x%hx ", value, image_address_lcd[x]);

#endif
    }
}

void videoin_set_copy_buf_rgb565(void)
{
    U32 ypixel;
    unsigned short *image_address_lcd;
    unsigned short *image_address_camera;
    bm_lcd_height = VIDEOIN_DISPLAY_HEIGHT;
    bm_lcd_width = VIDEOIN_DISPLAY_WIDTH;
    image_address_lcd = (unsigned short *)SD_LCD_BUFFER;
    image_address_camera = (U16 *) SD_VIDEOIN_BUFFER1;
    for (ypixel = 0; ypixel < (bm_lcd_height * bm_lcd_width); ypixel++) {
        image_address_lcd[ypixel] = image_address_camera[ypixel];
    }
}

static void videoin_yuv_to_rgb16(unsigned char y, unsigned char u, unsigned char v, unsigned char *rgb)
{
    register int r, g, b;
    int rgb16;

    r = (1192 * (y - 16) + 1634 * (v - 128)) >> 10;
    g = (1192 * (y - 16) - 833 * (v - 128) - 400 * (u - 128)) >> 10;
    b = (1192 * (y - 16) + 2066 * (u - 128)) >> 10;

    r = r > 255 ? 255 : r < 0 ? 0 : r;
    g = g > 255 ? 255 : g < 0 ? 0 : g;
    b = b > 255 ? 255 : b < 0 ? 0 : b;

    rgb16 = (int)(((r >> 3) << 11) | ((g >> 2) << 5) | ((b >> 3) << 0));

    *rgb = (unsigned char)(rgb16 & 0xFF);
    rgb++;
    *rgb = (unsigned char)((rgb16 & 0xFF00) >> 8);

}

void videoin_yuyv422_to_rgb16(unsigned char *buf, unsigned char *rgb, int width, int height)
{
    int y;
    int blocks;

    blocks = (width * height) * 2;

    for (y = 0; y < blocks; y += 4) {
        unsigned char Y1, Y2, U, V;

        Y1 = buf[y + 0];
        U = buf[y + 1];
        Y2 = buf[y + 2];
        V = buf[y + 3];

        videoin_yuv_to_rgb16(Y1, U, V, &rgb[y]);
        videoin_yuv_to_rgb16(Y2, U, V, &rgb[y + 2]);
    }
}


void videoin_display_image(void)
{
	/* S32 ret_val;*/
    lcd_drv_struct_t lcd_init_struct;

    lcd_init_struct.mode = VGA;
    bm_lcd_height = VIDEOIN_DISPLAY_HEIGHT;
    bm_lcd_width = VIDEOIN_DISPLAY_WIDTH;
    image_address = (U16 *) SD_LCD_BUFFER;

    vga_map_data = (unsigned char *)SD_LCD_BUFFER;

    /* Initialize the LCD. */
    printf("Displaying image...... \n");

}

int videoin_capture_display_image(void)
{
	unsigned int timeout = 60;
    lcd_drv_struct_t lcd_init_struct;
    lcd_init_struct.mode = VGA;

    videoin_isp_enable_irq();
    videoin_lcd = 1;

    dg_lcd_init(&lcd_init_struct);
    videoin_ispccdc_enable(1);

	while( timeout > 1 ) {

    	videoin_poll_interrupt();
#if DEBUG_ISP
	    isp_reg_dump();
#endif

#ifdef USE_RAW
    	videoin_set_copy_buf();
//    videoin_rgb24_to_rgb16();
#endif

#ifdef USE_RGB565
	    videoin_set_copy_buf_rgb565();
#endif

#ifdef USE_YUV422
	    videoin_yuyv422_to_rgb16((unsigned char *)SD_VIDEOIN_BUFFER1, (unsigned char *)SD_LCD_BUFFER, VIDEOIN_DISPLAY_WIDTH, VIDEOIN_DISPLAY_HEIGHT);
#endif

#ifdef COLOR_BAR
		videoin_yuyv422_to_rgb16((unsigned char *)SD_VIDEOIN_BUFFER1, (unsigned char *)SD_LCD_BUFFER, VIDEOIN_DISPLAY_WIDTH, VIDEOIN_DISPLAY_HEIGHT);
#endif
		timeout--;
	}
	videoin_ispccdc_enable(0);
	videoin_lcd = 0;
	return 1;
}

