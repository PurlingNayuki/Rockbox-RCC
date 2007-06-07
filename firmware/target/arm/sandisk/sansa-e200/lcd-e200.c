/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * $Id$
 *
 * Rockbox driver for Sansa e200 LCDs
 *
 * Based on reverse engineering done my MrH
 *
 * Copyright (c) 2006 Daniel Ankers
 *
 * All files in this archive are subject to the GNU General Public License.
 * See the file COPYING in the source tree root for full license agreement.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 ****************************************************************************/
#include "lcd.h"
#include "system.h"
#include <string.h>
#include "backlight-target.h"
#include "pp5024.h"

#define LCD_DATA_IN_GPIO GPIOB_INPUT_VAL
#define LCD_DATA_IN_PIN 6

#define LCD_DATA_OUT_GPIO GPIOB_OUTPUT_VAL
#define LCD_DATA_OUT_PIN 7

#define LCD_CLOCK_GPIO GPIOB_OUTPUT_VAL
#define LCD_CLOCK_PIN 5

#define LCD_CS_GPIO GPIOD_OUTPUT_VAL
#define LCD_CS_PIN 6

#define LCD_REG_0 (*(volatile unsigned long *)(0xc2000000))
#define LCD_REG_1 (*(volatile unsigned long *)(0xc2000004))
#define LCD_REG_2 (*(volatile unsigned long *)(0xc2000008))
#define LCD_REG_3 (*(volatile unsigned long *)(0xc200000c))
#define LCD_REG_4 (*(volatile unsigned long *)(0xc2000010))
#define LCD_REG_5 (*(volatile unsigned long *)(0xc2000014))
#define LCD_REG_6 (*(volatile unsigned long *)(0xc2000018))
#define LCD_REG_7 (*(volatile unsigned long *)(0xc200001c))
#define LCD_REG_8 (*(volatile unsigned long *)(0xc2000020))
#define LCD_REG_9 (*(volatile unsigned long *)(0xc2000024))
#define LCD_FB_BASE_REG (*(volatile unsigned long *)(0xc2000028))

/* Taken from HD66789 datasheet and seems similar enough.
   Definitely a Renesas chip though with a perfect register index
   match but at least one bit seems to be set that that datasheet
   doesn't show. It says T.B.D. on the regmap anyway. */
#define R_START_OSC             0x00
#define R_DRV_OUTPUT_CONTROL    0x01
#define R_DRV_WAVEFORM_CONTROL  0x02
#define R_ENTRY_MODE            0x03
#define R_COMPARE_REG1          0x04
#define R_COMPARE_REG2          0x05
#define R_DISP_CONTROL1         0x07
#define R_DISP_CONTROL2         0x08
#define R_DISP_CONTROL3         0x09
#define R_FRAME_CYCLE_CONTROL   0x0b
#define R_EXT_DISP_INTF_CONTROL 0x0c
#define R_POWER_CONTROL1        0x10
#define R_POWER_CONTROL2        0x11
#define R_POWER_CONTROL3        0x12
#define R_POWER_CONTROL4        0x13
#define R_RAM_ADDR_SET          0x21
#define R_RAM_READ_DATA         0x21
#define R_RAM_WRITE_DATA        0x22
#define R_RAM_WRITE_DATA_MASK1  0x23
#define R_RAM_WRITE_DATA_MASK2  0x24
#define R_GAMMA_FINE_ADJ_POS1   0x30
#define R_GAMMA_FINE_ADJ_POS2   0x31
#define R_GAMMA_FINE_ADJ_POS3   0x32
#define R_GAMMA_GRAD_ADJ_POS    0x33
#define R_GAMMA_FINE_ADJ_NEG1   0x34
#define R_GAMMA_FINE_ADJ_NEG2   0x35
#define R_GAMMA_FINE_ADJ_NEG3   0x36
#define R_GAMMA_GRAD_ADJ_NEG    0x37
#define R_GAMMA_AMP_ADJ_POS     0x38
#define R_GAMMA_AMP_ADJ_NEG     0x39
#define R_GATE_SCAN_START_POS   0x40
#define R_VERT_SCROLL_CONTROL   0x41
#define R_1ST_SCR_DRIVE_POS     0x42
#define R_2ND_SCR_DRIVE_POS     0x43
#define R_HORIZ_RAM_ADDR_POS    0x44
#define R_VERT_RAM_ADDR_POS     0x45

/* We don't know how to receive a DMA finished signal from the LCD controller
 * To avoid problems with flickering, we double-buffer the framebuffer and turn
 * off DMA while updates are taking place */
static fb_data lcd_driver_framebuffer[LCD_FBHEIGHT][LCD_FBWIDTH]
    __attribute__((aligned(16))); /* Same alignment as in lcd-16bit.c */

static inline void lcd_init_gpio(void)
{
    GPIOB_ENABLE |= (1<<7);
    GPIOB_ENABLE |= (1<<5);
    GPIOB_OUTPUT_EN |= (1<<7);
    GPIOB_OUTPUT_EN |= (1<<5);
    GPIOD_ENABLE |= (1<<6);
    GPIOD_OUTPUT_EN |= (1<<6);
}

static inline void lcd_bus_idle(void)
{
    LCD_CLOCK_GPIO |= (1 << LCD_CLOCK_PIN);
    LCD_DATA_OUT_GPIO |= (1 << LCD_DATA_OUT_PIN);
}

static inline void lcd_send_byte(unsigned char byte)
{

    int i;

    for (i = 7; i >=0 ; i--)
    {
        LCD_CLOCK_GPIO &= ~(1 << LCD_CLOCK_PIN);
        if ((byte >> i) & 1)
        {
            LCD_DATA_OUT_GPIO |= (1 << LCD_DATA_OUT_PIN);
        } else {
            LCD_DATA_OUT_GPIO &= ~(1 << LCD_DATA_OUT_PIN);
        }
        udelay(1);
        LCD_CLOCK_GPIO |= (1 << LCD_CLOCK_PIN);
        udelay(1);
        lcd_bus_idle();
        udelay(3);
    }
}

static inline void lcd_send_msg(unsigned char cmd, unsigned int data)
{
    lcd_bus_idle();
    udelay(1);
    LCD_CS_GPIO &= ~(1 << LCD_CS_PIN);
    udelay(10);
    lcd_send_byte(cmd);
    lcd_send_byte((unsigned char)(data >> 8));
    lcd_send_byte((unsigned char)(data & 0xff));
    LCD_CS_GPIO |= (1 << LCD_CS_PIN);
    udelay(1);
    lcd_bus_idle();
}

static inline void lcd_write_reg(unsigned int reg, unsigned int data)
{
    lcd_send_msg(0x70, reg);
    lcd_send_msg(0x72, data);
}

/* The LCD controller gets passed the address of the framebuffer, but can only
   use the physical, not the remapped, address.  This is a quick and dirty way
   of correcting it */
static unsigned long phys_fb_address(unsigned long address)
{
    if(address < 0x10000000)
    {
        return address + 0x10000000;
    } else {
        return address;
    }
}

inline void lcd_init_device(void)
{
/* All this is magic worked out by MrH */

/* Stop any DMA which is in progress */
    LCD_REG_6 &= ~1;
    udelay(100000);

/* Init GPIO ports */
    lcd_init_gpio();
/* Controller init */
    outl((inl(0x70000084) | (1 << 28)), 0x70000084);
    outl((inl(0x70000080) & ~(1 << 28)), 0x70000080);
    outl(((inl(0x70000010) & (0x03ffffff)) | (0x15 << 26)), 0x70000010);
    outl(((inl(0x70000014) & (0x0fffffff)) | (0x5 << 28)), 0x70000014);
    outl((inl(0x70000020) & ~(0x3 << 10)), 0x70000020);
    DEV_EN |= DEV_LCD; /* Enable controller */
    outl(0x6, 0x600060d0);
    DEV_RS |= DEV_LCD; /* Reset controller */
    outl((inl(0x70000020) & ~(1 << 14)), 0x70000020);
    lcd_bus_idle();
    DEV_RS &=~DEV_LCD; /* Clear reset */
    udelay(1000);

    LCD_REG_0 = (LCD_REG_0 & (0x00ffffff)) | (0x22 << 24);
    LCD_REG_0 = (LCD_REG_0 & (0xff00ffff)) | (0x14 << 16);
    LCD_REG_0 = (LCD_REG_0 & (0xffffc0ff)) | (0x3 << 8);
    LCD_REG_0 = (LCD_REG_0 & (0xffffffc0)) | (0xa);

    LCD_REG_1 &= 0x00ffffff;
    LCD_REG_1 &= 0xff00ffff;
    LCD_REG_1 = (LCD_REG_1 & 0xffff03ff) | (0x2 << 10);
    LCD_REG_1 = (LCD_REG_1 & 0xfffffc00) | (0xdd);

    LCD_REG_2 |= (1 << 5);
    LCD_REG_2 |= (1 << 6);
    LCD_REG_2 = (LCD_REG_2 & 0xfffffcff) | (0x2 << 8);

    LCD_REG_7 &= (0xf800ffff);
    LCD_REG_7 &= (0xfffff800);

    LCD_REG_8 = (LCD_REG_8 & (0xf800ffff)) | (0xb0 << 16);
    LCD_REG_8 = (LCD_REG_8 & (0xfffff800)) | (0xde); /* X-Y Geometry? */

    LCD_REG_5 |= 0xc;
    LCD_REG_5 = (LCD_REG_5 & ~(0x70)) | (0x3 << 4);
    LCD_REG_5 |= 2;

    LCD_REG_6 &= ~(1 << 15);
    LCD_REG_6 |= (0xe00);
    LCD_REG_6 = (LCD_REG_6 & (0xffffff1f)) | (0x4 << 5);
    LCD_REG_6 |= (1 << 4);

    LCD_REG_5 &= ~(1 << 7);
    LCD_FB_BASE_REG = phys_fb_address((unsigned long)lcd_driver_framebuffer);

    udelay(100000);

/* LCD init */

    /* TODO: Eliminate some of this outside the bootloader since this
       will already be setup and that will eliminate white-screen */

    /* Pull RESET low, then high */
    outl((inl(0x70000080) & ~(1 << 28)), 0x70000080);
    udelay(10000);
    outl((inl(0x70000080) | (1 << 28)), 0x70000080);
    udelay(10000);

    lcd_write_reg(R_POWER_CONTROL1, 0x4444);
    lcd_write_reg(R_POWER_CONTROL2, 0x0001);
    lcd_write_reg(R_POWER_CONTROL3, 0x0003);
    lcd_write_reg(R_POWER_CONTROL4, 0x1119);
    lcd_write_reg(R_POWER_CONTROL3, 0x0013);
    udelay(50000);

    lcd_write_reg(R_POWER_CONTROL1, 0x4440);
    lcd_write_reg(R_POWER_CONTROL4, 0x3119);
    udelay(150000);

    lcd_write_reg(R_DRV_OUTPUT_CONTROL, 0x101b);
    lcd_write_reg(R_DRV_WAVEFORM_CONTROL, 0x0700);
    lcd_write_reg(R_ENTRY_MODE, 0x6020);
    lcd_write_reg(R_COMPARE_REG1, 0x0000);
    lcd_write_reg(R_COMPARE_REG2, 0x0000);
    lcd_write_reg(R_DISP_CONTROL2, 0x0102);
    lcd_write_reg(R_DISP_CONTROL3, 0x0000);
    lcd_write_reg(R_FRAME_CYCLE_CONTROL, 0x4400);
    lcd_write_reg(R_EXT_DISP_INTF_CONTROL, 0x0110);

    lcd_write_reg(R_GATE_SCAN_START_POS, 0x0000);
    lcd_write_reg(R_VERT_SCROLL_CONTROL, 0x0000);
    lcd_write_reg(R_1ST_SCR_DRIVE_POS, (219 << 8));
    lcd_write_reg(R_2ND_SCR_DRIVE_POS, 0x0000);
    lcd_write_reg(R_HORIZ_RAM_ADDR_POS, (175 << 8));
    lcd_write_reg(R_VERT_RAM_ADDR_POS, (219 << 8));

    lcd_write_reg(R_GAMMA_FINE_ADJ_POS1, 0x0000);
    lcd_write_reg(R_GAMMA_FINE_ADJ_POS2, 0x0704);
    lcd_write_reg(R_GAMMA_FINE_ADJ_POS3, 0x0107);
    lcd_write_reg(R_GAMMA_GRAD_ADJ_POS, 0x0704);
    lcd_write_reg(R_GAMMA_FINE_ADJ_NEG1, 0x0107);
    lcd_write_reg(R_GAMMA_FINE_ADJ_NEG2, 0x0002);
    lcd_write_reg(R_GAMMA_FINE_ADJ_NEG3, 0x0707);
    lcd_write_reg(R_GAMMA_GRAD_ADJ_NEG, 0x0503);
    lcd_write_reg(R_GAMMA_AMP_ADJ_POS, 0x0000);
    lcd_write_reg(R_GAMMA_AMP_ADJ_NEG, 0x0000);

    lcd_write_reg(R_RAM_ADDR_SET, 175);

    lcd_write_reg(R_EXT_DISP_INTF_CONTROL, 0x0110);

    lcd_write_reg(R_POWER_CONTROL1, 0x4740);

    lcd_write_reg(R_DISP_CONTROL1, 0x0045);

    udelay(50000);

    lcd_write_reg(R_DISP_CONTROL1, 0x0065);
    lcd_write_reg(R_DISP_CONTROL1, 0x0067);

    udelay(50000);

    lcd_write_reg(R_DISP_CONTROL1, 0x0077);

    lcd_send_msg(0x70, R_RAM_WRITE_DATA);

    LCD_REG_6 |= 1; /* Start DMA */
}

void lcd_enable(bool on)
{
    if(on)
    {
        if(!(DEV_EN & DEV_LCD))
        {
            DEV_EN |= DEV_LCD; /* Enable LCD controller */
            lcd_update();      /* Resync display */
            LCD_REG_6 |= 1;    /* Restart DMA */
        }
    }
    else
    {
        if(DEV_EN & DEV_LCD)
        {
            LCD_REG_6 &= ~1;    /* Disable DMA */
            udelay(20000);      /* Wait for dma end (assuming 50Hz) */
            DEV_EN &= ~DEV_LCD; /* Disable LCD controller */
        }
    }
}

void lcd_update_rect(int x, int y, int width, int height)
{
    (void)x;
    (void)width;

    if(DEV_EN & DEV_LCD)
    {
#if 0
        /* Turn off DMA and wait for the transfer to complete */
        /* TODO: Work out the proper delay */
        LCD_REG_6 &= ~1;
        udelay(1000);
#endif
        /* Copy the Rockbox framebuffer to the second framebuffer */
        /* TODO: Move the second framebuffer into uncached SDRAM */
        memcpy(((char*)&lcd_driver_framebuffer)+(y * sizeof(fb_data) * LCD_WIDTH),
               ((char *)&lcd_framebuffer)+(y * sizeof(fb_data) * LCD_WIDTH),
               ((height * sizeof(fb_data) * LCD_WIDTH)));
        flush_icache();
#if 0
        /* Restart DMA */
        LCD_REG_6 |= 1;
#endif
    }
}

void lcd_update(void)
{
    if(DEV_EN & DEV_LCD)
    {
        /* TODO: It may be faster to swap the addresses of lcd_driver_framebuffer
         * and lcd_framebuffer */
#if 0
        /* Turn off DMA and wait for the transfer to complete */
        LCD_REG_6 &= ~1;
        udelay(1000);
#endif
        /* Copy the Rockbox framebuffer to the second framebuffer */
        memcpy(lcd_driver_framebuffer, lcd_framebuffer,
               sizeof(fb_data) * LCD_WIDTH * LCD_HEIGHT);
        flush_icache();
#if 0
        /* Restart DMA */
        LCD_REG_6 |= 1;
#endif
    }
}


/*** hardware configuration ***/

void lcd_set_contrast(int val)
{
    /* TODO: Implement lcd_set_contrast() */
    (void)val;
}

void lcd_set_invert_display(bool yesno)
{
    /* TODO: Implement lcd_set_invert_display() */
    (void)yesno;
}

/* turn the display upside down (call lcd_update() afterwards) */
void lcd_set_flip(bool yesno)
{
    /* TODO: Implement lcd_set_flip() */
    (void)yesno;
}

/* Blitting functions */

void lcd_blit(const fb_data* data, int x, int by, int width,
              int bheight, int stride)
{
    /* TODO: Implement lcd_blit() */
    (void)data;
    (void)x;
    (void)by;
    (void)width;
    (void)bheight;
    (void)stride;
}

/* Line write helper function for lcd_yuv_blit. Write two lines of yuv420. */
extern void lcd_write_yuv420_lines(fb_data *dst,
                                   unsigned char chroma_buf[LCD_HEIGHT/2*3],
                                   unsigned char const * const src[3],
                                   int width,
                                   int stride);
/* Performance function to blit a YUV bitmap directly to the LCD */
/* For the e200 - show it rotated */
/* So the LCD_WIDTH is now the height */
void lcd_yuv_blit(unsigned char * const src[3],
                  int src_x, int src_y, int stride,
                  int x, int y, int width, int height)
{
    if(DEV_EN & DEV_LCD)
    {
        /* Caches for chroma data so it only need be recaculated every other
           line */
        static unsigned char chroma_buf[LCD_HEIGHT/2*3]; /* 330 bytes */
        unsigned char const * yuv_src[3];
        off_t z;

        /* Sorry, but width and height must be >= 2 or else */
        width &= ~1;
        height >>= 1;

        fb_data *dst = (fb_data*)lcd_driver_framebuffer + 
                       x * LCD_WIDTH + (LCD_WIDTH - y) - 1;

        z = stride*src_y;
        yuv_src[0] = src[0] + z + src_x;
        yuv_src[1] = src[1] + (z >> 2) + (src_x >> 1);
        yuv_src[2] = src[2] + (yuv_src[1] - src[1]);

        do
        {
            lcd_write_yuv420_lines(dst, chroma_buf, yuv_src, width,
                                   stride);
            yuv_src[0] += stride << 1; /* Skip down two luma lines */
            yuv_src[1] += stride >> 1; /* Skip down one chroma line */
            yuv_src[2] += stride >> 1;
            dst -= 2;
        }
        while (--height > 0);
    }
}
