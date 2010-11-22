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
 *   05 Aug 2010 - Added Support for videoin S-Video Test 
 */

#include <common.h>
#include "types.h"
#include "dg_videoin.h"
#include "tvp514x_regs.h"
#include "../../device/inc/i2c_utils.h"
#include "../../drivers/inc/i2c_drv.h"
#include "../../common/inc/dg_gpio.h"

#define TVP5146_I2C_DELAY		200
#define TVP5146_I2C_ADDR        (0x5C)

#define CVBS_SELECTION		0x0C
#define SVIDEO_SELECTION	0x46

/* TVP514x default register values */
static struct tvp514x_reg tvp514x_reg_list[] = {
    {TOK_WRITE, REG_INPUT_SEL, 0x00},   /* Composite selected */
    {TOK_WRITE, REG_AFE_GAIN_CTRL, 0x0F},
    {TOK_WRITE, REG_VIDEO_STD, 0x00},   /* Auto mode */
    {TOK_WRITE, REG_OPERATION_MODE, 0x00},
    {TOK_SKIP, REG_AUTOSWITCH_MASK, 0x3F},
    {TOK_WRITE, REG_COLOR_KILLER, 0x10},
    {TOK_WRITE, REG_LUMA_CONTROL1, 0x00},
    {TOK_WRITE, REG_LUMA_CONTROL2, 0x00},
    {TOK_WRITE, REG_LUMA_CONTROL3, 0x02},
    {TOK_WRITE, REG_BRIGHTNESS, 0x80},
    {TOK_WRITE, REG_CONTRAST, 0x80},
    {TOK_WRITE, REG_SATURATION, 0x80},
    {TOK_WRITE, REG_HUE, 0x00},
    {TOK_WRITE, REG_CHROMA_CONTROL1, 0x00},
    {TOK_WRITE, REG_CHROMA_CONTROL2, 0x0E},
    {TOK_SKIP, 0x0F, 0x00}, /* Reserved */
    {TOK_WRITE, REG_COMP_PR_SATURATION, 0x80},
    {TOK_WRITE, REG_COMP_Y_CONTRAST, 0x80},
    {TOK_WRITE, REG_COMP_PB_SATURATION, 0x80},
    {TOK_SKIP, 0x13, 0x00}, /* Reserved */
    {TOK_WRITE, REG_COMP_Y_BRIGHTNESS, 0x80},
    {TOK_SKIP, 0x15, 0x00}, /* Reserved */
    {TOK_SKIP, REG_AVID_START_PIXEL_LSB, 0x55}, /* NTSC timing */
    {TOK_SKIP, REG_AVID_START_PIXEL_MSB, 0x00},
    {TOK_SKIP, REG_AVID_STOP_PIXEL_LSB, 0x25},
    {TOK_SKIP, REG_AVID_STOP_PIXEL_MSB, 0x03},
    {TOK_SKIP, REG_HSYNC_START_PIXEL_LSB, 0x00},    /* NTSC timing */
    {TOK_SKIP, REG_HSYNC_START_PIXEL_MSB, 0x00},
    {TOK_SKIP, REG_HSYNC_STOP_PIXEL_LSB, 0x40},
    {TOK_SKIP, REG_HSYNC_STOP_PIXEL_MSB, 0x00},
    {TOK_SKIP, REG_VSYNC_START_LINE_LSB, 0x04}, /* NTSC timing */
    {TOK_SKIP, REG_VSYNC_START_LINE_MSB, 0x00},
    {TOK_SKIP, REG_VSYNC_STOP_LINE_LSB, 0x07},
    {TOK_SKIP, REG_VSYNC_STOP_LINE_MSB, 0x00},
    {TOK_SKIP, REG_VBLK_START_LINE_LSB, 0x01},  /* NTSC timing */
    {TOK_SKIP, REG_VBLK_START_LINE_MSB, 0x00},
    {TOK_SKIP, REG_VBLK_STOP_LINE_LSB, 0x15},
    {TOK_SKIP, REG_VBLK_STOP_LINE_MSB, 0x00},
    {TOK_SKIP, 0x26, 0x00}, /* Reserved */
    {TOK_SKIP, 0x27, 0x00}, /* Reserved */
    {TOK_SKIP, REG_FAST_SWTICH_CONTROL, 0xCC},
    {TOK_SKIP, 0x29, 0x00}, /* Reserved */
    {TOK_SKIP, REG_FAST_SWTICH_SCART_DELAY, 0x00},
    {TOK_SKIP, 0x2B, 0x00}, /* Reserved */
    {TOK_SKIP, REG_SCART_DELAY, 0x00},
    {TOK_SKIP, REG_CTI_DELAY, 0x00},
    {TOK_SKIP, REG_CTI_CONTROL, 0x00},
    {TOK_SKIP, 0x2F, 0x00}, /* Reserved */
    {TOK_SKIP, 0x30, 0x00}, /* Reserved */
    {TOK_SKIP, 0x31, 0x05}, /* Reserved */
    {TOK_WRITE, REG_SYNC_CONTROL, 0x00},    /* HS, VS active high */
    {TOK_WRITE, REG_OUTPUT_FORMATTER1, 0x00},   /* 10-bit BT.656 */
    {TOK_WRITE, REG_OUTPUT_FORMATTER2, 0x11},   /* Enable clk & data */
    {TOK_WRITE, REG_OUTPUT_FORMATTER3, 0xEE},   /* Enable AVID & FLD */
    {TOK_WRITE, REG_OUTPUT_FORMATTER4, 0xAF},   /* Enable VS & HS */
    {TOK_WRITE, REG_OUTPUT_FORMATTER5, 0xFF},
    {TOK_WRITE, REG_OUTPUT_FORMATTER6, 0xFF},
    {TOK_WRITE, REG_CLEAR_LOST_LOCK, 0x00}, /* Clear status */
    {TOK_TERM, 0, 0},
};


static const struct tvp514x_reg tvp5146_init_reg_seq[] = {
    {TOK_WRITE, REG_VBUS_ADDRESS_ACCESS1, 0x02},
    {TOK_WRITE, REG_VBUS_ADDRESS_ACCESS2, 0x00},
    {TOK_WRITE, REG_VBUS_ADDRESS_ACCESS3, 0x80},
    {TOK_WRITE, REG_VBUS_DATA_ACCESS_NO_VBUS_ADDR_INCR, 0x01},
    {TOK_WRITE, REG_VBUS_ADDRESS_ACCESS1, 0x60},
    {TOK_WRITE, REG_VBUS_ADDRESS_ACCESS2, 0x00},
    {TOK_WRITE, REG_VBUS_ADDRESS_ACCESS3, 0xB0},
    {TOK_WRITE, REG_VBUS_DATA_ACCESS_NO_VBUS_ADDR_INCR, 0x01},
    {TOK_WRITE, REG_VBUS_DATA_ACCESS_NO_VBUS_ADDR_INCR, 0x00},
    {TOK_WRITE, REG_OPERATION_MODE, 0x01},
    {TOK_WRITE, REG_OPERATION_MODE, 0x00},
};

static const struct tvp514x_init_seq tvp5146_init = {
    .no_regs = ARRAY_SIZE(tvp5146_init_reg_seq),
    .init_reg_seq = tvp5146_init_reg_seq,
};


static i2c_init_structure_t tvp5146_i2c;

/* Initialize I2C3 */
void videoin_i2c_config(void)
{

    /* Initilaize the I2C interface. */
    tvp5146_i2c.i2c_no = I2C3;
    tvp5146_i2c.clock_speed = I2C_100K_CLK;
    tvp5146_i2c.slave_addr = TVP5146_I2C_ADDR;

	/* Initilaize the I2C interface. */
    dg_i2c_init(&tvp5146_i2c);

	udelay(TVP5146_I2C_DELAY);
    dg_i2c_deinit(&tvp5146_i2c);
	udelay(TVP5146_I2C_DELAY);
    dg_i2c_init(&tvp5146_i2c);
	udelay(TVP5146_I2C_DELAY);
}



/*
 * Write a value to a register in an TVP5146/47 decoder device.
 * Returns zero if successful, or non-zero otherwise.
 */
static int tvp514x_write_reg(U8 reg, U8 val)
{
    S32 ret_val = SUCCESS;
    U32 tempsize = 2;
    
#if DEBUG_TVP
    printf ("Before: tvp514x_write_reg: reg 0x%x \t val 0x%x\n",reg,val);
#endif
    ret_val = dg_i2c_write(&tvp5146_i2c, I2C_OFFSET(reg), &tempsize, &val);
#if DEBUG_TVP
    printf ("After: tvp514x_write_reg: reg 0x%x \t val 0x%x\n",reg,val);
#endif
    if (SUCCESS != ret_val)
    {
        printf("%s:%d - I2C  write failed.\n\r", __FILE__, __LINE__);
        dg_i2c_deinit(&tvp5146_i2c);
        return ret_val;
    }
   // udelay (100);
    printf ("");
    val = 0xff;
    ret_val = dg_i2c_read(&tvp5146_i2c, I2C_OFFSET(reg), &tempsize, &val);
    if (SUCCESS != ret_val)
    {
        printf("%s:%d - I2C read failed.\n\r", __FILE__, __LINE__);
        dg_i2c_deinit(&tvp5146_i2c);
        return ret_val;
    }
#if DEBUG_TVP
    printf ("tvp514x_write_reg: 0x%x\n",val);
#endif
    return SUCCESS;
}

/*
 * Read a value from a register in an TVP5146/47 decoder device.
 * Returns value read if successful, or non-zero (-1) otherwise.
 */
static int tvp514x_read_reg(U8 reg)
{
    S32 ret_val = SUCCESS;
    U32 tempsize = 2;
    U8  val = 0x00;
    ret_val = dg_i2c_read(&tvp5146_i2c, I2C_OFFSET(reg), &tempsize, &val);
    if (SUCCESS != ret_val)
    {
        printf("%s:%d - I2C read failed.\n\r", __FILE__, __LINE__);
        dg_i2c_deinit(&tvp5146_i2c);
        return ret_val;
    }
    return val;
}

/*
 * tvp514x_write_regs : Initializes a list of TVP5146/47 registers
 *      if token is TOK_TERM, then entire write operation terminates
 *      if token is TOK_DELAY, then a delay of 'val' msec is introduced
 *      if token is TOK_SKIP, then the register write is skipped
 *      if token is TOK_WRITE, then the register write is performed
 *
 * reglist - list of registers to be written
 * Returns zero if successful, or non-zero otherwise.
 */
static int tvp5146_config(void)
{
    int err = SUCCESS ,i;

#if DEBUG_TVP
    printf ("tvp514x_configure: start\n");
#endif
    for (i = 0; tvp514x_reg_list[i].token != TOK_TERM; i++) {


#if DEBUG_TVP
        printf ("tvp514x_reg_list[%d].token , %d \n", i,tvp514x_reg_list[i].token);
#endif
        if (tvp514x_reg_list[i].token == TOK_DELAY) {
            udelay(tvp514x_reg_list[i].val);
            continue;
        }
   
        if (tvp514x_reg_list[i].token == TOK_SKIP)
            continue;

#if DEBUG_TVP
        printf ("tvp514x_reg.init_reg_seq[i].reg 0x%x \t tvp514x_reg.init_reg_seq[i].val 0x%x\n",
				 tvp514x_reg_list[i].reg, tvp514x_reg_list[i].val);
#endif
        err = tvp514x_write_reg(tvp514x_reg_list[i].reg, (U8) tvp514x_reg_list[i].val);
        if (err != SUCCESS) {
            printf("Write failed. Err[%d] line %d\n", err,__LINE__);
            return err;
        }
    }

#if DEBUG_TVP
    printf ("tvp514x_configure: exit\n");
#endif
    return err;
}

/*
 * Detect if an tvp514x is present, and if so which revision.
 * A device is considered to be detected if the chip ID (LSB and MSB)
 * registers match the expected values.
 * Any value of the rom version register is accepted.
 * Returns ENODEV error number if no device is detected, or zero
 * if a device is detected.
 */
static int tvp514x_detect()
{
    S32 ret_val = SUCCESS;
    U32 tempsize = 2;
    U8  val = 0x00;
    U8 chip_id_msb, chip_id_lsb, rom_ver;

#if DEBUG_TVP
    printf ("tvp514x_detect : start\n");
#endif
    ret_val = dg_i2c_read(&tvp5146_i2c, I2C_OFFSET(REG_CHIP_ID_MSB), &tempsize, &val);
    if (SUCCESS != ret_val)
    {
        printf("%s:%d - I2C read failed.\n\r", __FILE__, __LINE__);
        dg_i2c_deinit(&tvp5146_i2c);
        return ret_val;
    }
    chip_id_msb = val;
    udelay (1000);

    ret_val = dg_i2c_read(&tvp5146_i2c, I2C_OFFSET(REG_CHIP_ID_LSB), &tempsize, &val);
    if (SUCCESS != ret_val)
    {
        printf("%s:%d - I2C read failed.\n\r", __FILE__, __LINE__);
        dg_i2c_deinit(&tvp5146_i2c);
        return ret_val;
    }
    chip_id_lsb = val;

    rom_ver = tvp514x_read_reg(REG_ROM_VERSION);

    printf("chip id detected msb:0x%x lsb:0x%x rom version:0x%x\n",
            chip_id_msb, chip_id_lsb, rom_ver);
    if ((chip_id_msb != TVP514X_CHIP_ID_MSB)
        || ((chip_id_lsb != TVP5146_CHIP_ID_LSB)
        && (chip_id_lsb != TVP5147_CHIP_ID_LSB))) {
        /* We didn't read the values we expected, so this must not be
         * an TVP5146/47.
         */
        printf( "chip id mismatch msb:0x%x lsb:0x%x\n",
                 chip_id_msb, chip_id_lsb);
        return FAILURE;
    }
#if DEBUG_TVP
    printf ("tvp514x_detect : exit\n");
#endif
    return SUCCESS;
}

static int tvp5146_power_seq(void)
{
    int i = 0;
    int err = SUCCESS;
    for (i = 0; i < tvp5146_init.no_regs; i++) 
    {

#if DEBUG_TVP
        printf ("tvp5146_init.init_reg_seq[i].reg 0x%x \t  tvp5146_init.init_reg_seq[i].val 0x%x\n",
				 tvp5146_init.init_reg_seq[i].reg, tvp5146_init.init_reg_seq[i].val);
#endif
		err = tvp514x_write_reg(tvp5146_init.init_reg_seq[i].reg,
                                 tvp5146_init.init_reg_seq[i].val);
    udelay (80000);
        if (SUCCESS != err)
		{
        		printf("Unable to detect tvp5146 init power sequence %d \n", err);
        		return err;
 		}
#if DEBUG_TVP
         printf ("inside tvp5146_power_on loop \n ");
#endif
    } 
}

extern unsigned int videoin_select;

int tvp5146_format ()
{
    int err = SUCCESS;
    S32 ret_val = SUCCESS;
    U32 tempsize = 2, reg = 0;
    U8 val = 0xFF;
	U8 sync_lock_status, lock_mask;
	
	if (VIDEOIN_CVBS == videoin_select) {
		tvp514x_write_reg(REG_INPUT_SEL, CVBS_SELECTION);
	}
	else if (VIDEOIN_SVIDEO == videoin_select) {
    	tvp514x_write_reg(REG_INPUT_SEL, SVIDEO_SELECTION);
	}

    U32 try_count = 4;
    U8 input_sel;
    U8 std;
	ret_val = dg_i2c_read(&tvp5146_i2c, I2C_OFFSET(REG_INPUT_SEL), &tempsize, &val);
    if (SUCCESS != ret_val)
    {
        printf("%s:%d - I2C read failed.\n\r", __FILE__, __LINE__);
        dg_i2c_deinit(&tvp5146_i2c);
        return ret_val;
    }

    printf ("dg_i2c_read: REG_INPUT_SEL  0x%x\n",val);

	if (VIDEOIN_CVBS == videoin_select) {
	    input_sel = CVBS_SELECTION;
    	tvp514x_write_reg(REG_INPUT_SEL, input_sel);
	    udelay (200000);
	}
	else if (VIDEOIN_SVIDEO == videoin_select) {
	    input_sel = SVIDEO_SELECTION;
    	tvp514x_write_reg(REG_INPUT_SEL, input_sel);
	    udelay (200000);

    	tvp514x_write_reg(REG_AUTOSWITCH_MASK, 0x3f);
	    udelay (200000);

    	tvp514x_write_reg(REG_LUMA_CONTROL3, 0x00);
	    udelay (200000);

    	tvp514x_write_reg(REG_CHROMA_CONTROL2, 0x04);
	    udelay (200000);
	}

    udelay (200000);
    udelay (200000);
    udelay (200000);

	tvp514x_write_reg(REG_CLEAR_LOST_LOCK, 0x01);
    udelay (200000);
    udelay (200000);
    udelay (200000);

    if (input_sel >= 0x00 && input_sel <= 0x0C)
    {
        lock_mask = STATUS_CLR_SUBCAR_LOCK_BIT |
            		STATUS_HORZ_SYNC_LOCK_BIT |
            		STATUS_VIRT_SYNC_LOCK_BIT;

		printf ("Input Selected: CVBS\n");	
    }
   /* For S-Video, the ondition should be 44, 45, 46,
     * 54, 55, 56, 4C, 4D, 4E, 5C, 5D, 5E */
    else if (input_sel >= 0x44 && input_sel <= 0x5e)
    {
        lock_mask = STATUS_HORZ_SYNC_LOCK_BIT |
            		STATUS_VIRT_SYNC_LOCK_BIT;
		printf ("Input Selected: S-VIDEO\n");
    }
    else
    {
		printf ("Invalid input: Selected\n");
        return FAILURE;
    }

    while (try_count-- > 0)
    {
		udelay (200000);
    	val = 0;
	  	ret_val = dg_i2c_read(&tvp5146_i2c, I2C_OFFSET(REG_VIDEO_STD), &tempsize, &val);
    	if (SUCCESS != ret_val)
    	{
        	printf("%s:%d - I2C read failed.\n\r", __FILE__, __LINE__);
        	dg_i2c_deinit(&tvp5146_i2c);
        	return ret_val;
   	 	}

    	printf ("dg_i2c_read: REG_VIDEO_STD  0x%x\n",val);

    	if ((val & VIDEO_STD_MASK) == VIDEO_STD_AUTO_SWITCH_BIT) 
    	{
		
			ret_val = dg_i2c_read(&tvp5146_i2c, I2C_OFFSET(REG_VIDEO_STD_STATUS), &tempsize, &val);
    		printf ("dg_i2c_read: REG_VIDEO_STD_STATUS 0x%x\n",val);
    	}
        
		if (val == STD_INVALID)
		    continue;

        ret_val = dg_i2c_read(&tvp5146_i2c, I2C_OFFSET(REG_STATUS1), &tempsize, &sync_lock_status);

    	printf ("dg_i2c_read: sync_lock_status 0x%x lock_mask 0x%x\n",sync_lock_status, lock_mask);
        if (lock_mask == (sync_lock_status & lock_mask))
            break;  /* Input detected */
    }
    if (std == STD_NTSC_MJ)
	{	
		printf ("current std: STD_NTSC_MJ\n");
	}
    else if (std == STD_PAL_BDGHIN)
	{
		printf ("current std: STD_PAL_BDGHIN\n");
	}
    else 
    {
		printf ("Invalid Std\n");
 		return FAILURE;
    }
       
}


static int tvp5146_power_on(void)
{
    int i = 0;
    S32 retval = SUCCESS;
    U32 tempsize = 2, reg = 0;
    U8 val = 0xFF;

    /* Power Up Sequence */
#if DEBUG_TVP
    printf ("tvp5146_power_on: start\n ");
#endif
    udelay (80000);
    tvp5146_power_seq();
    udelay (80000);

   /* Detect the sensor is not already detected */
    retval = tvp514x_detect();
    if (SUCCESS != retval) 
    {
        printf("Unable to detect tvp5146 decoder\n");
        return retval;
    }
  
}

void tvp5146_power_off (void)
{
    unsigned int gpioData;
    unsigned short *gpio5_base = (unsigned short *)OMAP34XX_GPIO5_BASE;

#if DEBUG_TVP
    printf ("\ntvp5146_power_off ...: Start\n");
#endif

    /* Set TVP5146 in power save mode */
    tvp514x_write_reg(REG_OPERATION_MODE, 0x01);

    /* Disable GPIO 134 */
    gpioData = (*(volatile unsigned int *) (OMAP34XX_GPIO5_BASE + GPIO_SETDATAOUT));
    (*(volatile unsigned int *) (OMAP34XX_GPIO5_BASE + GPIO_SETDATAOUT)) = gpioData | 0x00000040;

#if DEBUG_TVP
    printf ("\ntvp5146_power_off ...: Exit\n");
#endif
    dg_i2c_deinit(&tvp5146_i2c);
    udelay(100);
}

int	tvp5146_initialize(void) 
{

	int retval = 0; 

    tvp5146_power_on();
    printf("\ntvp5146_power_on done...\n");

	udelay(8000);
	udelay(8000);
	udelay(8000);
	udelay(8000);
	udelay(8000);
	udelay(8000);
	udelay(8000);

    retval = tvp5146_config();
    if (SUCCESS != retval)
    {
        printf("Unable to configure tvp5146 decoder\n");
        return retval;
    }
	tvp5146_format();

	return 0;
}

