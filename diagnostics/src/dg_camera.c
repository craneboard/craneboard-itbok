/*
 * dg_camera.c: Camera diagnostic code.
 *
 * (C) Copyright 2010
 * Mistral Solutions Private Limited, <www.mistralsolutions.com>
 *
 * Author :
 *	Ganeshan <nganeshan@mistralsolutions.com>
 *
 * Derived from linux kernel source
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
 *   12 jan 2010 - Created from OMAP2 ITBOK source.
 */

#include <common.h>
#include <i2c.h>
#include <asm/io.h>
#include <asm/arch/omap3.h>
#include <asm/arch/mux.h>
#include "types.h"
#include "../../common/inc/dg_gpio.h"
#include "../inc/dg_camera.h"
#include "../inc/mt9t111.h"
#include "../inc/ispregs.h"
#include "../../device/inc/i2c_utils.h"
#include "../../drivers/inc/i2c_drv.h"

#define BIT(x)      (1 << (x))

#define CAMERA_TEST_COUNT 1
#define CAM_DEBUG 	 0

#define ispWritel(v,a)      writel(v,a)

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

#define  GPIOCTR_ID2   		0xAA
#define  GPIOPUPDCTR1_ID2   0xAB
#define  GPIOPUPDCTR2_ID2   0xAC
#define  GPIOPUPDCTR3_ID2   0xAD

extern int config_sensor(void);
extern int mt9t111_config(int);
extern int capture_display_image(void);
extern status_t omap_diag_i2c_write(U8 i2c_no, U8 slave_addr, U8 * data, U32 key, U8 size);
void camera_i2c_2byte_write(u32 addr, u16 write_buf, u32 size);

i2c_init_structure_t camera_i2c;

/* This function calculate the proper devisor value and sets the XCLKA */
/*
 * Function Name : 
 */
void isp_set_cam_xclk(unsigned int xclk)
{
	unsigned int divisor;
	unsigned int currentxclk;
	unsigned int gpioData;

	if (xclk >= CM_CAM_MCLK_HZ) {
		divisor = ISPTCTRL_CTRL_DIV_BYPASS;
		currentxclk = CM_CAM_MCLK_HZ;
	} else if (xclk >= 2) {
		divisor = CM_CAM_MCLK_HZ / xclk;
		if (divisor >= ISPTCTRL_CTRL_DIV_BYPASS)
			divisor = ISPTCTRL_CTRL_DIV_BYPASS - 1;
		currentxclk = CM_CAM_MCLK_HZ / divisor;
	} else {
		divisor = xclk;
		currentxclk = 0;
	}

/* setting the CAM_XCLKA */
#if  CAM_DEBUG
	printf("Camera clock set Divisor %d \n", divisor);
#endif
	gpioData = readl(CAM_TCTRL_CTRL);
#if CAM_DEBUG
	printf("Camera clock read  %x \n", gpioData);
#endif
	writel(gpioData | divisor, CAM_TCTRL_CTRL);
#if CAM_DEBUG
	gpioData = readl(CAM_TCTRL_CTRL);
	printf("camera clock write %x \n", gpioData);
#endif

}

/*
 * Function Name : 
 */
void isp_ick_fck_enable(void)
{
	/* Clock setting */
	writel(readl(CM_CLKSEL_CAM) | 0x4, CM_CLKSEL_CAM);
	writel(readl(CM_FCLKEN_CAM) | 0x3, CM_FCLKEN_CAM);
	writel(readl(CM_ICLKEN_CAM) | 0x1, CM_ICLKEN_CAM);
	writel(readl(PM_PWSTCTRL_CAM) | 0x30107, PM_PWSTCTRL_CAM);

}

void camera_mux_settings(void)
{
	unsigned char byte;

	/* Mux setting */
	/* GPIO 134 */
	MUX_VAL(CP(MMC2_DAT2), (IEN | PTU | DIS | M4));

	/* GPIO 54 */
	MUX_VAL(CP(GPMC_NCS3), (IEN | PTU | DIS | M4))

	    /* GPIO 136 */
	    MUX_VAL(CP(MMC2_DAT4), (IEN | PTD | DIS | M4));

	/* GPIO 134 set DIR set as output */
	writel(readl(OMAP34XX_GPIO5_BASE + GPIO_OE) & ~(GPIO6), OMAP34XX_GPIO5_BASE + GPIO_OE);

	/* GPIO 136 set DIR set as output */
	writel(readl(OMAP34XX_GPIO5_BASE + GPIO_OE) & ~(GPIO8), OMAP34XX_GPIO5_BASE + GPIO_OE);

	/* GPIO 54 set DIR set as output */
	writel(readl(OMAP34XX_GPIO2_BASE + GPIO_OE) & ~(GPIO22), OMAP34XX_GPIO2_BASE + GPIO_OE);

	/* GPIO_157 for Camera. make it output and drive low */
	writel(readl(OMAP34XX_GPIO5_BASE + GPIO_OE) & ~(GPIO29), OMAP34XX_GPIO5_BASE + GPIO_OE);
	writel(GPIO29, OMAP34XX_GPIO5_BASE + GPIO_CLEARDATAOUT);

	/* GPIO_2 of T2 set as OUTPUT : drive it Low */
	byte = 0;
	i2c_read(PWRMGT_ADDR_ID2, GPIODATADIR1_ID2, 1, &byte, 1);

	byte |= 0x04;
	i2c_write(PWRMGT_ADDR_ID2, GPIODATADIR1_ID2, 1, &byte, 1);

	byte = 0;
	i2c_read(PWRMGT_ADDR_ID2, GPIODATAOUT1_ID2, 1, &byte, 1);

	byte &= 0xFB;
	i2c_write(PWRMGT_ADDR_ID2, GPIODATAOUT1_ID2, 1, &byte, 1);

	/* GPIO_8 of T2 set as OUTPUT : drive it High */
	byte = 0;
	i2c_read(PWRMGT_ADDR_ID2, GPIODATADIR2_ID2, 1, &byte, 1);

	byte |= 0x01;
	i2c_write(PWRMGT_ADDR_ID2, GPIODATADIR2_ID2, 1, &byte, 1);

	byte = 0;
	i2c_read(PWRMGT_ADDR_ID2, GPIODATAOUT1_ID2, 1, &byte, 1);

	byte |= 0x01;
	i2c_write(PWRMGT_ADDR_ID2, GPIODATAOUT1_ID2, 1, &byte, 1);

	/* Set PULL UP Register GPIOPUPDCTR1 */
	byte = 0xAA;
	i2c_write(PWRMGT_ADDR_ID2, GPIOPUPDCTR1_ID2, 1, &byte, 1);

	udelay(200);

}

void twl4030_gpio_settings(void)
{
	unsigned char byte;

	/****************************** T2 GPIO Setting **********************/
	byte = 0x04;
	i2c_write(PWRMGT_ADDR_ID2, GPIOCTR_ID2, 1, &byte, 1);

	/* GPIO_2 of T2 set as OUTPUT : drive it Low */
	byte = 0xFD;
	i2c_read(PWRMGT_ADDR_ID2, GPIODATADIR1_ID2, 1, &byte, 1);
	byte = 0x00;
	i2c_write(PWRMGT_ADDR_ID2, GPIODATAOUT1_ID2, 1, &byte, 1);

	/* GPIO_8 of T2 set as OUTPUT : drive it High */
	byte = 0x01;
	i2c_read(PWRMGT_ADDR_ID2, GPIODATADIR2_ID2, 1, &byte, 1);
	byte = 0x00;
	i2c_read(PWRMGT_ADDR_ID2, GPIODATAOUT2_ID2, 1, &byte, 1);

	/* Set PULL UP Register GPIOPUPDCTR1 */
	byte = 0xAA;
	i2c_write(PWRMGT_ADDR_ID2, GPIOPUPDCTR1_ID2, 1, &byte, 1);
	i2c_write(PWRMGT_ADDR_ID2, GPIOPUPDCTR2_ID2, 1, &byte, 1);
	i2c_write(PWRMGT_ADDR_ID2, GPIOPUPDCTR3_ID2, 1, &byte, 1);

	/* OMAP3_GPIO_98_VIDEO_DEC_RESET set dataout to 1 */
	writel(readl(OMAP34XX_GPIO4_BASE + GPIO_SETDATAOUT) | BIT(2), OMAP34XX_GPIO4_BASE + GPIO_SETDATAOUT);
}

void select_camera(void)
{
	unsigned int gpiodata;

	/****************** OMAP GPIO Directon Settings *************************/
	/* GPIO 157 MUX Setting */
	MUX_VAL(CP(MCBSP1_CLKR), (IEN | PTU | EN | M4))

	    /*setting GPIO 157 as output and dataout low */
	    gpiodata = readl(OMAP34XX_GPIO5_BASE + GPIO_OE);
#if CAM_DEBUG
	printf("GPIO 157 OE read  %x \n", gpiodata);
#endif
	writel(gpiodata & ~BIT(29), OMAP34XX_GPIO5_BASE + GPIO_OE);
#if CAM_DEBUG
	gpioData = readl(OMAP34XX_GPIO5_BASE + GPIO_OE);
	printf("GPIO 157 OE write %x \n", gpiodata);
#endif
	writel(readl(OMAP34XX_GPIO5_BASE + GPIO_CLEARDATAOUT) | BIT(29), OMAP34XX_GPIO5_BASE + GPIO_CLEARDATAOUT);
}

void mt9t_deinit(void)
{
	unsigned int gpioData;

	/* Enable Camera Video in (GPIO136 = 0) */
#if CAM_DEBUG
	printf("\nGPIO 136\n");
#endif
	gpioData = readl(OMAP34XX_GPIO5_BASE + GPIO_DATAOUT);
#if CAM_DEBUG
	printf("GPIO 136 setdata read  %x \n", gpioData);
#endif
	writel(gpioData | BIT(8), OMAP34XX_GPIO5_BASE + GPIO_DATAOUT);
#if CAM_DEBUG
	gpioData = readl(OMAP34XX_GPIO5_BASE + GPIO_DATAOUT);
	printf("GPIO 136 setdata write %x \n", gpioData);
#endif

}

/*
 * Function Name : 
 */
void mt9t111_init(void)
{
	unsigned int gpioData;

	/* Disable TVP5146 Video in (GPIO134 = 1) */
#if CAM_DEBUG
	printf("\nGPIO 134\n");
#endif
	gpioData = readl(OMAP34XX_GPIO5_BASE + GPIO_SETDATAOUT);
#if CAM_DEBUG
	printf("GPIO 134 setdata read  %x \n", gpioData);
#endif
	writel(gpioData | BIT(6), OMAP34XX_GPIO5_BASE + GPIO_SETDATAOUT);
#if CAM_DEBUG
	gpioData = readl(OMAP34XX_GPIO5_BASE + GPIO_SETDATAOUT);
	printf("GPIO 134 setdata write %x \n", gpioData);
#endif

	/* Disable Exapansion Camera Video in (GPIO54 = 1) */
#if CAM_DEBUG
	printf("\nGPIO 54\n");
#endif
	gpioData = readl(OMAP34XX_GPIO2_BASE + GPIO_SETDATAOUT);
#if CAM_DEBUG
	printf("GPIO 54 setdata read  %x \n", gpioData);
#endif
	writel(gpioData | BIT(22), OMAP34XX_GPIO2_BASE + GPIO_SETDATAOUT);
#if CAM_DEBUG
	gpioData = readl(OMAP34XX_GPIO2_BASE + GPIO_SETDATAOUT);
	printf("GPIO 54 setdata write %x \n", gpioData);
#endif

	/* Enable Camera Video in (GPIO136 = 0) */
#if CAM_DEBUG
	printf("\nGPIO 136\n");
#endif
	gpioData = readl(OMAP34XX_GPIO5_BASE + GPIO_DATAOUT);
#if CAM_DEBUG
	printf("GPIO 136 setdata read  %x \n", gpioData);
#endif
	writel(gpioData & ~BIT(8), OMAP34XX_GPIO5_BASE + GPIO_DATAOUT);
#if CAM_DEBUG
	gpioData = readl(OMAP34XX_GPIO5_BASE + GPIO_DATAOUT);
	printf("GPIO 136 setdata write %x \n", gpioData);
#endif

	/* **************** GPIO_98 MT9T111_RESET_GPIO  ********************** */
#if CAM_DEBUG
	printf("\nGPIO 98 MT9T111_RESET_GPIO \n");
#endif
	/* set GPIO_98  OUPUT register as High */
	gpioData = readl(OMAP34XX_GPIO4_BASE + GPIO_OE);
#if CAM_DEBUG
	printf("GPIO 98 GPIO_OE read \n", gpioData);
#endif
	writel(gpioData | BIT(2), OMAP34XX_GPIO4_BASE + GPIO_OE);
#if CAM_DEBUG
	gpioData = readl(OMAP34XX_GPIO4_BASE + GPIO_OE);
	printf("GPIO 98 GPIO_OE write %x \n", gpioData);
#endif
	udelay(8000);
	udelay(8000);
	udelay(8000);
	udelay(8000);

	/* set GPIO_98 to high in CLEARDATAOUT register to set low */
	gpioData = readl(OMAP34XX_GPIO4_BASE + GPIO_DATAOUT);
#if CAM_DEBUG
	printf("GPIO 98 cleardata  read %x \n", gpioData);
#endif
	writel(gpioData & ~BIT(2), OMAP34XX_GPIO4_BASE + GPIO_DATAOUT);
	udelay(8000);
	udelay(8000);
	udelay(8000);
	udelay(8000);
	udelay(8000);
	udelay(8000);

	/* set GPIO_98 to high  in SETDATAOUT register */
	gpioData = readl(OMAP34XX_GPIO4_BASE + GPIO_SETDATAOUT);
#if CAM_DEBUG
	printf("GPIO 98 setdata read %x \n", gpioData);
#endif
	writel(gpioData | BIT(2), OMAP34XX_GPIO4_BASE + GPIO_SETDATAOUT);
#if CAM_DEBUG
	gpioData = readl(OMAP34XX_GPIO4_BASE + GPIO_SETDATAOUT);
	printf("GPIO 98 setdata write %x \n", gpioData);
#endif
	udelay(8000);
	udelay(8000);
	udelay(8000);
	udelay(8000);
	udelay(8000);
	udelay(8000);

#if CAM_DEBUG
	gpioData = readl(OMAP34XX_GPIO4_BASE + GPIO_CLEARDATAOUT);
	printf("GPIO 98 cleardata  write %x \n", gpioData);
#endif
	udelay(200);

	/* **************** GPIO_167 MT9T111_STANDBY_GPIO ********************** */
#if CAM_DEBUG
	printf("\nGPIO 167 MT9T111_STANDBY_GPIO \n");
#endif

	/* set GPIO_167  OUPUT register as low */
	gpioData = readl(OMAP34XX_GPIO6_BASE + GPIO_OE);
#if CAM_DEBUG
	printf("GPIO 167 GPIO_OE read \n", gpioData);
#endif
	writel(gpioData & ~BIT(7), OMAP34XX_GPIO6_BASE + GPIO_OE);
#if CAM_DEBUG
	gpioData = readl(OMAP34XX_GPIO6_BASE + GPIO_OE);
	printf("GPIO 167 GPIO_OE write %x \n", gpioData);
#endif
	udelay(8000);
	udelay(8000);

	/* GPIO  167 drive high */
	gpioData = readl(OMAP34XX_GPIO6_BASE + GPIO_SETDATAOUT);
#if CAM_DEBUG
	printf("GPIO 167 setdata read %x \n", gpioData);
#endif
	writel(gpioData | BIT(7), OMAP34XX_GPIO6_BASE + GPIO_SETDATAOUT);
#if CAM_DEBUG
	gpioData = readl(OMAP34XX_GPIO6_BASE + GPIO_SETDATAOUT);
	printf("GPIO 167 setdata write %x \n", gpioData);
#endif
	udelay(100);

	/* **************** GPIO_110 ********************** */

	/* GPIO 110 Mux settings */
	MUX_VAL(CP(CAM_D11), (IEN | PTU | EN | M4));
	/* set GPIO_110  OUPUT register as low */
	gpioData = readl(OMAP34XX_GPIO4_BASE + GPIO_OE);
#if CAM_DEBUG
	printf("GPIO 110 GPIO_OE read \n", gpioData);
#endif
	writel(gpioData & ~BIT(14), OMAP34XX_GPIO4_BASE + GPIO_OE);
#if CAM_DEBUG
	gpioData = readl(OMAP34XX_GPIO4_BASE + GPIO_OE);
	printf("GPIO 110 GPIO_OE write %x \n", gpioData);
#endif
	udelay(8000);
	udelay(8000);

	/* GPIO  110 drive high */
	gpioData = readl(OMAP34XX_GPIO4_BASE + GPIO_SETDATAOUT);
#if CAM_DEBUG
	printf("GPIO 110 setdata read  %x \n", gpioData);
#endif
	writel(gpioData | BIT(14), OMAP34XX_GPIO4_BASE + GPIO_SETDATAOUT);
#if CAM_DEBUG
	gpioData = readl(OMAP34XX_GPIO4_BASE + GPIO_SETDATAOUT);
	printf("GPIO 110 setdata write %x \n", gpioData);
#endif
	udelay(100);
	udelay(100);
#if CAM_DEBUG
	printf("GPIO setting done\n");
#endif
}

int mt9t111_detect(void)
{

	u32 addr = 0;
	u32 size = 0;
	u8 addr_lsb = 0x00, addr_msb = 0x00;
	u8 read_buf[4];

	printf("Reading Micron Camera ID...\n");
	addr = 0x0000;
	addr_lsb = addr & 0x00FF;
	addr_msb = (addr & 0xFF00 >> 8);

	read_buf[0] = (addr & 0xFF00) >> 8;
	read_buf[1] = (addr & 0x00FF);
	size = 2;
	memset(read_buf, '\0', sizeof(read_buf));
	read_i2c_with_dataptr_2byte_subaddr(CAMERA_I2C_ADDR, 0x00, I2C_2_BYTE_ADDRESS, 2, (u8 *) & read_buf);
	udelay(1000);

	printf(" Data Read at I2C address 0x%x is\n %x\n", CAMERA_I2C_ADDR, read_buf[0]);
	printf(" Data Read at I2C address 0x%x is\n %x\n", CAMERA_I2C_ADDR, read_buf[1]);

	if ((read_buf[0] == 0x26) && (read_buf[1] == 0x80)) {
		printf(".....Found Micron Camera\n");
	} else {
		printf("Not valid device ID\n");
		return -1;
	}
	return 0;
}

void isp_power_settings(void)
{
	ispWritel((ISP_SYSCONFIG_AUTOIDLE | (ISP_SYSCONFIG_MIDLEMODE_SMARTSTANDBY << ISP_SYSCONFIG_MIDLEMODE_SHIFT)), ISP_SYSCONFIG);

	/* MMU setting not required */
}

/*
 * Function Name : 
 */
int camera_test(void)
{
	int pass_count;
	short retval;

	/* Initialize Camera I2C structure */
	camera_i2c.i2c_no = I2C2;
	camera_i2c.clock_speed = I2C_100K_CLK; //		I2C_400K_CLK;
	camera_i2c.slave_addr = CAMERA_I2C_ADDR;
#if CAM_DEBUG
	printf("\nStarting Camera Test...\n");
#endif
	isp_ick_fck_enable();
	select_camera();
#if CAM_DEBUG
	printf("Selected Device: Camera...\n");
#endif
	camera_mux_settings();
#if CAM_DEBUG
	printf("Camera Pin muxing done...\n");
#endif
	twl4030_gpio_settings();
#if CAM_DEBUG
	printf("TWL4030 GPIO setting done...\n");
#endif

	/* Initilaize the I2C interface. */
	dg_i2c_init(&camera_i2c);
	udelay(200);
	dg_i2c_deinit(&camera_i2c);
	udelay(200);
	dg_i2c_init(&camera_i2c);
	udelay(200);

	isp_power_settings();

	pass_count = 0;

	printf("Configuring ISP Sensor..... \n");
	config_sensor();
	printf("\nConfiguring MT9T111 Camera Sensor..... \n");
	retval = mt9t111_config(1);
	if (-1 == retval) {
		return -1;
	}
	if (0 > capture_display_image()) {
		printf("\nfaild to configure the camera\n");
		return -1;
	} else {
		pass_count = 1;
	}

	mt9t_deinit();
	printf("***********************************************************\n");
	return !!!(pass_count == CAMERA_TEST_COUNT);
}

/*
 * Function Name : 
 */
void camera_i2c_2byte_write(u32 addr, u16 write_val, u32 size)
{

	u32 key = 0x0AF00000;
	u8 addr_lsb = 0x00, addr_msb = 0x00;
	u8 write_buf[4];
	u8 temp_buf[4];

	write_buf[0] = (write_val & 0x00FF);
	write_buf[1] = ((write_val & 0xFF00) >> 8);
	addr_lsb = (addr & 0x00FF);
	addr_msb = ((addr & 0xFF00) >> 8);
	key = (key | addr_msb);

	temp_buf[0] = addr_lsb;
	temp_buf[1] = write_buf[1];
	temp_buf[2] = write_buf[0];
	omap_diag_i2c_write(2, 0x3c, temp_buf, key, size - 1);

}
