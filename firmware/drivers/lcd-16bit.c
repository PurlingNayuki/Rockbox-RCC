/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * $Id$
 *
 * Copyright (C) 2005 by Dave Chapman
 *
 * Rockbox driver for 16-bit colour LCDs
 *
 * All files in this archive are subject to the GNU General Public License.
 * See the file COPYING in the source tree root for full license agreement.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 ****************************************************************************/
#include "config.h"

#include "cpu.h"
#include "lcd.h"
#include "kernel.h"
#include "thread.h"
#include <string.h>
#include <stdlib.h>
#include "memory.h"
#include "file.h"
#include "debug.h"
#include "system.h"
#include "font.h"
#include "rbunicode.h"
#include "bidi.h"
#include "scroll_engine.h"

enum fill_opt {
    OPT_NONE = 0,
    OPT_SET,
    OPT_COPY
};

/*** globals ***/
fb_data lcd_framebuffer[LCD_FBHEIGHT][LCD_FBWIDTH] IRAM_LCDFRAMEBUFFER __attribute__ ((aligned (16)));


static fb_data* lcd_backdrop = NULL;
static long lcd_backdrop_offset IDATA_ATTR = 0;

#if !defined(TOSHIBA_GIGABEAT_F) || defined(SIMULATOR)
static unsigned fg_pattern IDATA_ATTR = LCD_DEFAULT_FG;
static unsigned bg_pattern IDATA_ATTR = LCD_DEFAULT_BG;
static unsigned lss_pattern IDATA_ATTR = LCD_DEFAULT_LS;
static unsigned lse_pattern IDATA_ATTR = LCD_DEFAULT_BG;
static unsigned lst_pattern IDATA_ATTR = LCD_DEFAULT_FG;
#else
unsigned fg_pattern IDATA_ATTR = LCD_DEFAULT_FG;
unsigned bg_pattern IDATA_ATTR = LCD_DEFAULT_BG;
unsigned lss_pattern IDATA_ATTR = LCD_DEFAULT_LS;
unsigned lse_pattern IDATA_ATTR = LCD_DEFAULT_BG;
unsigned lst_pattern IDATA_ATTR = LCD_DEFAULT_FG;
#endif

static int drawmode = DRMODE_SOLID;
static int xmargin = 0;
static int ymargin = 0;
static int curfont = FONT_SYSFIXED;

/* LCD init */
void lcd_init(void)
{
    lcd_clear_display();

    /* Call device specific init */
    lcd_init_device();
    scroll_init();
}

/*** parameter handling ***/

void lcd_set_drawmode(int mode)
{
    drawmode = mode & (DRMODE_SOLID|DRMODE_INVERSEVID);
}

int lcd_get_drawmode(void)
{
    return drawmode;
}

void lcd_set_foreground(unsigned color)
{
    fg_pattern = color;
}

unsigned lcd_get_foreground(void)
{
    return fg_pattern;
}

void lcd_set_background(unsigned color)
{
    bg_pattern = color;
}

unsigned lcd_get_background(void)
{
    return bg_pattern;
}

void lcd_set_selector_start(unsigned color)
{
    lss_pattern = color;
}

void lcd_set_selector_end(unsigned color)
{
    lse_pattern = color;
}

void lcd_set_selector_text(unsigned color)
{
    lst_pattern = color;
}

void lcd_set_drawinfo(int mode, unsigned fg_color, unsigned bg_color)
{
    lcd_set_drawmode(mode);
    fg_pattern = fg_color;
    bg_pattern = bg_color;
}

void lcd_setmargins(int x, int y)
{
    xmargin = x;
    ymargin = y;
}

int lcd_getxmargin(void)
{
    return xmargin;
}

int lcd_getymargin(void)
{
    return ymargin;
}

void lcd_setfont(int newfont)
{
    curfont = newfont;
}

int lcd_getstringsize(const unsigned char *str, int *w, int *h)
{
    return font_getstringsize(str, w, h, curfont);
}

/*** low-level drawing functions ***/

#define LCDADDR(x, y) (&lcd_framebuffer[(y)][(x)])

static void setpixel(fb_data *address) ICODE_ATTR;
static void setpixel(fb_data *address)
{
    *address = fg_pattern;
}

static void clearpixel(fb_data *address) ICODE_ATTR;
static void clearpixel(fb_data *address)
{
    *address = bg_pattern;
}

static void clearimgpixel(fb_data *address) ICODE_ATTR;
static void clearimgpixel(fb_data *address)
{
    *address = *(fb_data *)((long)address + lcd_backdrop_offset);
}

static void flippixel(fb_data *address) ICODE_ATTR;
static void flippixel(fb_data *address)
{
    *address = ~(*address);
}

static void nopixel(fb_data *address) ICODE_ATTR;
static void nopixel(fb_data *address)
{
    (void)address;
}

lcd_fastpixelfunc_type* const lcd_fastpixelfuncs_bgcolor[8] = {
    flippixel, nopixel, setpixel, setpixel,
    nopixel, clearpixel, nopixel, clearpixel
};

lcd_fastpixelfunc_type* const lcd_fastpixelfuncs_backdrop[8] = {
    flippixel, nopixel, setpixel, setpixel,
    nopixel, clearimgpixel, nopixel, clearimgpixel
};

lcd_fastpixelfunc_type* const * lcd_fastpixelfuncs = lcd_fastpixelfuncs_bgcolor;

void lcd_set_backdrop(fb_data* backdrop)
{
    lcd_backdrop = backdrop;
    if (backdrop)
    {
        lcd_backdrop_offset = (long)backdrop - (long)&lcd_framebuffer[0][0];
        lcd_fastpixelfuncs = lcd_fastpixelfuncs_backdrop;
    }
    else
    {
        lcd_backdrop_offset = 0;
        lcd_fastpixelfuncs = lcd_fastpixelfuncs_bgcolor;
    }
}

fb_data* lcd_get_backdrop(void)
{
    return lcd_backdrop;
}

/*** drawing functions ***/

/* Clear the whole display */
void lcd_clear_display(void)
{
    fb_data *dst = LCDADDR(0, 0);

    if (drawmode & DRMODE_INVERSEVID)
    {
        memset16(dst, fg_pattern, LCD_WIDTH*LCD_HEIGHT);
    }
    else
    {
        if (!lcd_backdrop)
            memset16(dst, bg_pattern, LCD_WIDTH*LCD_HEIGHT);
        else
            memcpy(dst, lcd_backdrop, sizeof(lcd_framebuffer));
    }

    lcd_scroll_info.lines = 0;
}

/* Set a single pixel */
void lcd_drawpixel(int x, int y)
{
    if (((unsigned)x < LCD_WIDTH) && ((unsigned)y < LCD_HEIGHT))
        lcd_fastpixelfuncs[drawmode](LCDADDR(x, y));
}

/* Draw a line */
void lcd_drawline(int x1, int y1, int x2, int y2)
{
    int numpixels;
    int i;
    int deltax, deltay;
    int d, dinc1, dinc2;
    int x, xinc1, xinc2;
    int y, yinc1, yinc2;
    lcd_fastpixelfunc_type *pfunc = lcd_fastpixelfuncs[drawmode];

    deltax = abs(x2 - x1);
    deltay = abs(y2 - y1);
    xinc2 = 1;
    yinc2 = 1;

    if (deltax >= deltay)
    {
        numpixels = deltax;
        d = 2 * deltay - deltax;
        dinc1 = deltay * 2;
        dinc2 = (deltay - deltax) * 2;
        xinc1 = 1;
        yinc1 = 0;
    }
    else
    {
        numpixels = deltay;
        d = 2 * deltax - deltay;
        dinc1 = deltax * 2;
        dinc2 = (deltax - deltay) * 2;
        xinc1 = 0;
        yinc1 = 1;
    }
    numpixels++; /* include endpoints */

    if (x1 > x2)
    {
        xinc1 = -xinc1;
        xinc2 = -xinc2;
    }

    if (y1 > y2)
    {
        yinc1 = -yinc1;
        yinc2 = -yinc2;
    }

    x = x1;
    y = y1;

    for (i = 0; i < numpixels; i++)
    {
        if (((unsigned)x < LCD_WIDTH) && ((unsigned)y < LCD_HEIGHT))
            pfunc(LCDADDR(x, y));

        if (d < 0)
        {
            d += dinc1;
            x += xinc1;
            y += yinc1;
        }
        else
        {
            d += dinc2;
            x += xinc2;
            y += yinc2;
        }
    }
}

/* Draw a horizontal line (optimised) */
void lcd_hline(int x1, int x2, int y)
{
    int x, width;
    unsigned bits = 0;
    enum fill_opt fillopt = OPT_NONE;
    fb_data *dst, *dst_end;
    lcd_fastpixelfunc_type *pfunc = lcd_fastpixelfuncs[drawmode];

    /* direction flip */
    if (x2 < x1)
    {
        x = x1;
        x1 = x2;
        x2 = x;
    }

    /* nothing to draw? */
    if (((unsigned)y >= LCD_HEIGHT) || (x1 >= LCD_WIDTH) || (x2 < 0))
        return;

    /* clipping */
    if (x1 < 0)
        x1 = 0;
    if (x2 >= LCD_WIDTH)
        x2 = LCD_WIDTH-1;

    if (drawmode & DRMODE_INVERSEVID)
    {
        if (drawmode & DRMODE_BG)
        {
            if (!lcd_backdrop)
            {
                fillopt = OPT_SET;
                bits = bg_pattern;
            }
            else
                fillopt = OPT_COPY;
        }
    }
    else
    {
        if (drawmode & DRMODE_FG)
        {
            fillopt = OPT_SET;
            bits = fg_pattern;
        }
    }
    dst = LCDADDR(x1, y);
    width = x2 - x1 + 1;

    switch (fillopt)
    {
      case OPT_SET:
        memset16(dst, bits, width);
        break;

      case OPT_COPY:
        memcpy(dst, (void *)((long)dst + lcd_backdrop_offset),
               width * sizeof(fb_data));
        break;

      case OPT_NONE:
        dst_end = dst + width;
        do
            pfunc(dst++);
        while (dst < dst_end);
        break;
    }
}

/* Draw a vertical line (optimised) */
void lcd_vline(int x, int y1, int y2)
{
    int y;
    fb_data *dst, *dst_end;
    lcd_fastpixelfunc_type *pfunc = lcd_fastpixelfuncs[drawmode];

    /* direction flip */
    if (y2 < y1)
    {
        y = y1;
        y1 = y2;
        y2 = y;
    }

    /* nothing to draw? */
    if (((unsigned)x >= LCD_WIDTH) || (y1 >= LCD_HEIGHT) || (y2 < 0))
        return;

    /* clipping */
    if (y1 < 0)
        y1 = 0;
    if (y2 >= LCD_HEIGHT)
        y2 = LCD_HEIGHT-1;

    dst = LCDADDR(x, y1);
    dst_end = dst + (y2 - y1) * LCD_WIDTH;

    do
    {
        pfunc(dst);
        dst += LCD_WIDTH;
    }
    while (dst <= dst_end);
}

/* Draw a rectangular box */
void lcd_drawrect(int x, int y, int width, int height)
{
    if ((width <= 0) || (height <= 0))
        return;

    int x2 = x + width - 1;
    int y2 = y + height - 1;

    lcd_vline(x, y, y2);
    lcd_vline(x2, y, y2);
    lcd_hline(x, x2, y);
    lcd_hline(x, x2, y2);
}

/* Fill a rectangular area */
void lcd_fillrect(int x, int y, int width, int height)
{
    unsigned bits = 0;
    enum fill_opt fillopt = OPT_NONE;
    fb_data *dst, *dst_end;
    lcd_fastpixelfunc_type *pfunc = lcd_fastpixelfuncs[drawmode];

    /* nothing to draw? */
    if ((width <= 0) || (height <= 0) || (x >= LCD_WIDTH) || (y >= LCD_HEIGHT)
        || (x + width <= 0) || (y + height <= 0))
        return;

    /* clipping */
    if (x < 0)
    {
        width += x;
        x = 0;
    }
    if (y < 0)
    {
        height += y;
        y = 0;
    }
    if (x + width > LCD_WIDTH)
        width = LCD_WIDTH - x;
    if (y + height > LCD_HEIGHT)
        height = LCD_HEIGHT - y;

    if (drawmode & DRMODE_INVERSEVID)
    {
        if (drawmode & DRMODE_BG)
        {
            if (!lcd_backdrop)
            {
                fillopt = OPT_SET;
                bits = bg_pattern;
            }
            else
                fillopt = OPT_COPY;
        }
    }
    else
    {
        if (drawmode & DRMODE_FG)
        {
            fillopt = OPT_SET;
            bits = fg_pattern;
        }
    }
    dst = LCDADDR(x, y);
    dst_end = dst + height * LCD_WIDTH;

    do
    {
        fb_data *dst_row, *row_end;

        switch (fillopt)
        {
          case OPT_SET:
            memset16(dst, bits, width);
            break;

          case OPT_COPY:
            memcpy(dst, (void *)((long)dst + lcd_backdrop_offset),
                   width * sizeof(fb_data));
            break;

          case OPT_NONE:
            dst_row = dst;
            row_end = dst_row + width;
            do
                pfunc(dst_row++);
            while (dst_row < row_end);
            break;
        }
        dst += LCD_WIDTH;
    }
    while (dst < dst_end);
}

/* About Rockbox' internal monochrome bitmap format:
 *
 * A bitmap contains one bit for every pixel that defines if that pixel is
 * black (1) or white (0). Bits within a byte are arranged vertically, LSB
 * at top.
 * The bytes are stored in row-major order, with byte 0 being top left,
 * byte 1 2nd from left etc. The first row of bytes defines pixel rows
 * 0..7, the second row defines pixel row 8..15 etc.
 *
 * This is the mono bitmap format used on all other targets so far; the
 * pixel packing doesn't really matter on a 8bit+ target. */

/* Draw a partial monochrome bitmap */

void lcd_mono_bitmap_part(const unsigned char *src, int src_x, int src_y,
                          int stride, int x, int y, int width, int height)
                          ICODE_ATTR;
void lcd_mono_bitmap_part(const unsigned char *src, int src_x, int src_y,
                          int stride, int x, int y, int width, int height)
{
    const unsigned char *src_end;
    bool has_backdrop;
    fb_data *dst, *dst_end, *backdrop;
    lcd_fastpixelfunc_type *fgfunc, *bgfunc;

    /* nothing to draw? */
    if ((width <= 0) || (height <= 0) || (x >= LCD_WIDTH) || (y >= LCD_HEIGHT)
        || (x + width <= 0) || (y + height <= 0))
        return;

    /* clipping */
    if (x < 0)
    {
        width += x;
        src_x -= x;
        x = 0;
    }
    if (y < 0)
    {
        height += y;
        src_y -= y;
        y = 0;
    }
    if (x + width > LCD_WIDTH)
        width = LCD_WIDTH - x;
    if (y + height > LCD_HEIGHT)
        height = LCD_HEIGHT - y;

    src += stride * (src_y >> 3) + src_x; /* move starting point */
    src_y  &= 7;
    src_end = src + width;

    dst = LCDADDR(x, y);
    has_backdrop = (lcd_backdrop != NULL);
    backdrop = lcd_backdrop + y * LCD_WIDTH + x;
    fgfunc = lcd_fastpixelfuncs[drawmode];
    bgfunc = lcd_fastpixelfuncs[drawmode ^ DRMODE_INVERSEVID];
    do 
    {
        const unsigned char *src_col = src++;
        unsigned data = *src_col >> src_y;
        fb_data *dst_col = dst++;
        int numbits = 8 - src_y;
        fb_data *backdrop_col = backdrop++;
        dst_end = dst_col + height * LCD_WIDTH;
        do 
        {
            switch (drawmode)
            {
                case DRMODE_SOLID:
                    if (data & 0x01)
                        *dst_col = fg_pattern;
                    else
                        *dst_col = has_backdrop ? *backdrop_col : bg_pattern;
                    break;
                case DRMODE_FG:
                    if (data & 0x01)
                        *dst_col = fg_pattern;
                    break;
                case (DRMODE_SOLID|DRMODE_INVERSEVID):
                    if (data & 0x01)
                        *dst_col = has_backdrop ? *backdrop_col : bg_pattern;
                    else
                        *dst_col = fg_pattern;
                    break;
                default:
                    if (data & 0x01)
                        fgfunc(dst_col);
                    else
                        bgfunc(dst_col);
                    break;
            }
            dst_col += LCD_WIDTH;
            backdrop_col += LCD_WIDTH;
            data >>= 1;
            if (--numbits == 0) 
            {
                src_col += stride;
                data = *src_col;
                numbits = 8;
            }
        }
        while (dst_col < dst_end);
    }
    while (src < src_end);
}
/* Draw a full monochrome bitmap */
void lcd_mono_bitmap(const unsigned char *src, int x, int y, int width, int height)
{
    lcd_mono_bitmap_part(src, 0, 0, width, x, y, width, height);
}

/* Draw a partial native bitmap */
void lcd_bitmap_part(const fb_data *src, int src_x, int src_y,
                     int stride, int x, int y, int width, int height)
                     ICODE_ATTR;
void lcd_bitmap_part(const fb_data *src, int src_x, int src_y,
                     int stride, int x, int y, int width, int height)
{
    fb_data *dst, *dst_end;

    /* nothing to draw? */
    if ((width <= 0) || (height <= 0) || (x >= LCD_WIDTH) || (y >= LCD_HEIGHT)
        || (x + width <= 0) || (y + height <= 0))
        return;

    /* clipping */
    if (x < 0)
    {
        width += x;
        src_x -= x;
        x = 0;
    }
    if (y < 0)
    {
        height += y;
        src_y -= y;
        y = 0;
    }
    if (x + width > LCD_WIDTH)
        width = LCD_WIDTH - x;
    if (y + height > LCD_HEIGHT)
        height = LCD_HEIGHT - y;

    src += stride * src_y + src_x; /* move starting point */
    dst = LCDADDR(x, y);
    dst_end = dst + height * LCD_WIDTH;

    do
    {
        memcpy(dst, src, width * sizeof(fb_data));
        src += stride;
        dst += LCD_WIDTH;
    }
    while (dst < dst_end);
}

/* Draw a full native bitmap */
void lcd_bitmap(const fb_data *src, int x, int y, int width, int height)
{
    lcd_bitmap_part(src, 0, 0, width, x, y, width, height);
}

#if !defined(TOSHIBA_GIGABEAT_F) && !defined(TOSHIBA_GIGABEAT_S) \
    || defined(SIMULATOR)
/* Draw a partial native bitmap */
void lcd_bitmap_transparent_part(const fb_data *src, int src_x, int src_y,
                                 int stride, int x, int y, int width,
                                 int height) ICODE_ATTR;
void lcd_bitmap_transparent_part(const fb_data *src, int src_x, int src_y,
                                 int stride, int x, int y, int width,
                                 int height)
{
    fb_data *dst, *dst_end;

    /* nothing to draw? */
    if ((width <= 0) || (height <= 0) || (x >= LCD_WIDTH) || (y >= LCD_HEIGHT)
        || (x + width <= 0) || (y + height <= 0))
        return;

    /* clipping */
    if (x < 0)
    {
        width += x;
        src_x -= x;
        x = 0;
    }
    if (y < 0)
    {
        height += y;
        src_y -= y;
        y = 0;
    }
    if (x + width > LCD_WIDTH)
        width = LCD_WIDTH - x;
    if (y + height > LCD_HEIGHT)
        height = LCD_HEIGHT - y;

    src += stride * src_y + src_x; /* move starting point */
    dst = LCDADDR(x, y);
    dst_end = dst + height * LCD_WIDTH;

    do
    {
        int i;
        for(i = 0;i < width;i++)
        {
            if (src[i] == REPLACEWITHFG_COLOR)
                dst[i] = fg_pattern;
            else if(src[i] != TRANSPARENT_COLOR)
                dst[i] = src[i];
        }
        src += stride;
        dst += LCD_WIDTH;
    }
    while (dst < dst_end);
}
#endif /* !defined(TOSHIBA_GIGABEAT_F) || defined(SIMULATOR) */

/* Draw a full native bitmap with a transparent color */
void lcd_bitmap_transparent(const fb_data *src, int x, int y,
                            int width, int height)
{
    lcd_bitmap_transparent_part(src, 0, 0, width, x, y, width, height);
}

/* put a string at a given pixel position, skipping first ofs pixel columns */
static void lcd_putsxyofs(int x, int y, int ofs, const unsigned char *str)
{
    unsigned short ch;
    unsigned short *ucs;
    struct font* pf = font_get(curfont);

    ucs = bidi_l2v(str, 1);

    while ((ch = *ucs++) != 0 && x < LCD_WIDTH)
    {
        int width;
        const unsigned char *bits;

        /* get proportional width and glyph bits */
        width = font_get_width(pf,ch);

        if (ofs > width)
        {
            ofs -= width;
            continue;
        }

        bits = font_get_bits(pf, ch);

        lcd_mono_bitmap_part(bits, ofs, 0, width, x, y, width - ofs, pf->height);

        x += width - ofs;
        ofs = 0;
    }
}

/* put a string at a given pixel position */
void lcd_putsxy(int x, int y, const unsigned char *str)
{
    lcd_putsxyofs(x, y, 0, str);
}

/*** line oriented text output ***/

/* put a string at a given char position */
void lcd_puts(int x, int y, const unsigned char *str)
{
    lcd_puts_style_offset(x, y, str, STYLE_DEFAULT, 0);
}

void lcd_puts_style(int x, int y, const unsigned char *str, int style)
{
    lcd_puts_style_offset(x, y, str, style, 0);
}

void lcd_puts_offset(int x, int y, const unsigned char *str, int offset)
{
    lcd_puts_style_offset(x, y, str, STYLE_DEFAULT, offset);
}

/* put a string at a given char position, style, and pixel position,
 * skipping first offset pixel columns */
void lcd_puts_style_offset(int x, int y, const unsigned char *str, int style,
                           int offset)
{
    int xpos,ypos,w,h,xrect;
    int lastmode = drawmode;
    int oldfgcolor = fg_pattern;
    int oldbgcolor = bg_pattern;

    /* make sure scrolling is turned off on the line we are updating */
    lcd_scroll_info.lines &= ~(1 << y);

    if(!str || !str[0])
        return;

    lcd_getstringsize(str, &w, &h);
    xpos = xmargin + x*w / utf8length(str);
    ypos = ymargin + y*h;
    drawmode = (style & STYLE_INVERT) ?
               (DRMODE_SOLID|DRMODE_INVERSEVID) : DRMODE_SOLID;
    if (style & STYLE_GRADIENT || style & STYLE_COLORBAR) {
        fg_pattern = lss_pattern;
    }
    else if (style & STYLE_COLORED) {
        if (drawmode == DRMODE_SOLID)
            fg_pattern = style & STYLE_COLOR_MASK;
        else
            bg_pattern = style & STYLE_COLOR_MASK;
    }
    drawmode ^= DRMODE_INVERSEVID;
    xrect = xpos + MAX(w - offset, 0);

    if (style & STYLE_GRADIENT) {
        int h_r = RGB_UNPACK_RED(lss_pattern) << 16;
        int h_b = RGB_UNPACK_BLUE(lss_pattern) << 16;
        int h_g = RGB_UNPACK_GREEN(lss_pattern) << 16;
        int rstep = (h_r - ((signed)RGB_UNPACK_RED(lse_pattern) << 16)) / h;
        int gstep = (h_g - ((signed)RGB_UNPACK_GREEN(lse_pattern) << 16)) / h;
        int bstep = (h_b - ((signed)RGB_UNPACK_BLUE(lse_pattern) << 16)) / h;
        int count;

        drawmode = DRMODE_FG;
        for(count = 0; count < h; count++) {
            lcd_hline(xpos, LCD_WIDTH, ypos + count);
            h_r -= rstep;
            h_g -= gstep;
            h_b -= bstep;
            fg_pattern = LCD_RGBPACK(h_r >> 16, h_g >> 16, h_b >> 16);
        }
        fg_pattern = lst_pattern;
    }
    else if (style & STYLE_COLORBAR) {
        drawmode = DRMODE_FG;
        lcd_fillrect(xpos, ypos, LCD_WIDTH - xpos, h);
        fg_pattern = lst_pattern;
    }
    else {
        lcd_fillrect(xrect, ypos, LCD_WIDTH - xrect, h);
        drawmode = (style & STYLE_INVERT) ?
        (DRMODE_SOLID|DRMODE_INVERSEVID) : DRMODE_SOLID;
    }
    lcd_putsxyofs(xpos, ypos, offset, str);
    drawmode = lastmode;
    fg_pattern = oldfgcolor;
    bg_pattern = oldbgcolor;
}

/*** scrolling ***/
void lcd_puts_scroll(int x, int y, const unsigned char *string)
{
    lcd_puts_scroll_style(x, y, string, STYLE_DEFAULT);
}

void lcd_puts_scroll_style(int x, int y, const unsigned char *string, int style)
{
     lcd_puts_scroll_style_offset(x, y, string, style, 0);
}

void lcd_puts_scroll_offset(int x, int y, const unsigned char *string, int offset)
{
     lcd_puts_scroll_style_offset(x, y, string, STYLE_DEFAULT, offset);
}

void lcd_puts_scroll_style_offset(int x, int y, const unsigned char *string,
                                         int style, int offset)
{
    struct scrollinfo* s;
    int w, h;

    if(y>=LCD_SCROLLABLE_LINES) return;

    s = &lcd_scroll_info.scroll[y];

    s->start_tick = current_tick + lcd_scroll_info.delay;
    s->invert = false;
    if (style & STYLE_INVERT) {
        s->invert = 1;
    }
    else if (style & STYLE_COLORBAR) {
        s->invert = 2;
    }
    else if (style & STYLE_GRADIENT) {
        s->invert = 3;
    }
    lcd_puts_style_offset(x,y,string,style,offset);

    lcd_getstringsize(string, &w, &h);

    if (LCD_WIDTH - x * 8 - xmargin < w) {
        /* prepare scroll line */
        char *end;

        memset(s->line, 0, sizeof s->line);
        strcpy(s->line, string);

        /* get width */
        s->width = lcd_getstringsize(s->line, &w, &h);

        /* scroll bidirectional or forward only depending on the string
           width */
        if ( lcd_scroll_info.bidir_limit ) {
            s->bidir = s->width < (LCD_WIDTH - xmargin) *
                (100 + lcd_scroll_info.bidir_limit) / 100;
        }
        else
            s->bidir = false;

        if (!s->bidir) { /* add spaces if scrolling in the round */
            strcat(s->line, "   ");
            /* get new width incl. spaces */
            s->width = lcd_getstringsize(s->line, &w, &h);
        }

        end = strchr(s->line, '\0');
        strncpy(end, string, LCD_WIDTH/2);

        s->len = utf8length(string);
        s->offset = offset;
        s->startx = xmargin + x * s->width / s->len;
        s->backward = false;
        s->line_color = (style&STYLE_COLORED)?
                            (style&STYLE_COLOR_MASK): -1;
        lcd_scroll_info.lines |= (1<<y);
    }
    else
        /* force a bit switch-off since it doesn't scroll */
        lcd_scroll_info.lines &= ~(1<<y);
}

void lcd_scroll_fn(void)
{
    struct font* pf;
    struct scrollinfo* s;
    int index;
    int xpos, ypos;
    int lastmode;
    unsigned old_fgcolor = fg_pattern;
    unsigned old_bgcolor = bg_pattern;

    for ( index = 0; index < LCD_SCROLLABLE_LINES; index++ ) {
        /* really scroll? */
        if ((lcd_scroll_info.lines & (1 << index)) == 0)
            continue;

        s = &lcd_scroll_info.scroll[index];

        /* check pause */
        if (TIME_BEFORE(current_tick, s->start_tick))
            continue;

        if (s->line_color >= 0) {
            if (s->invert) {
                fg_pattern = old_fgcolor;
                bg_pattern = s->line_color;
            }
            else {
                fg_pattern = s->line_color;
                bg_pattern = old_bgcolor;
            }
        }

        if (s->backward)
            s->offset -= lcd_scroll_info.step;
        else
            s->offset += lcd_scroll_info.step;

        pf = font_get(curfont);
        xpos = s->startx;
        ypos = ymargin + index * pf->height;

        if (s->bidir) { /* scroll bidirectional */
            if (s->offset <= 0) {
                /* at beginning of line */
                s->offset = 0;
                s->backward = false;
                s->start_tick = current_tick + lcd_scroll_info.delay * 2;
            }
            if (s->offset >= s->width - (LCD_WIDTH - xpos)) {
                /* at end of line */
                s->offset = s->width - (LCD_WIDTH - xpos);
                s->backward = true;
                s->start_tick = current_tick + lcd_scroll_info.delay * 2;
            }
        }
        else {
            /* scroll forward the whole time */
            if (s->offset >= s->width)
                s->offset %= s->width;
        }

        lastmode = drawmode;
        drawmode = s->invert == 1 ?
            (DRMODE_SOLID|DRMODE_INVERSEVID) : DRMODE_SOLID;
        if (s->invert == 2) {
            fg_pattern = lss_pattern;
            drawmode = DRMODE_FG;
            lcd_fillrect(0, ypos, LCD_WIDTH, pf->height);
            fg_pattern = lst_pattern;
        }
        else if (s->invert == 3) {
            int h_r = RGB_UNPACK_RED(lss_pattern) << 16;
            int h_b = RGB_UNPACK_BLUE(lss_pattern) << 16;
            int h_g = RGB_UNPACK_GREEN(lss_pattern) << 16;
            int rstep = (h_r - ((signed)RGB_UNPACK_RED(lse_pattern) << 16))
                        / (signed)pf->height;
            int gstep = (h_g - ((signed)RGB_UNPACK_GREEN(lse_pattern) << 16))
                        / (signed)pf->height;
            int bstep = (h_b - ((signed)RGB_UNPACK_BLUE(lse_pattern) << 16))
                        / (signed)pf->height;
            unsigned int count;

            fg_pattern = lss_pattern;
            drawmode = DRMODE_FG;
            for(count = 0; count < pf->height; count++) {
                lcd_hline(0, LCD_WIDTH , ypos + count);
                h_r -= rstep;
                h_g -= gstep;
                h_b -= bstep;
                fg_pattern = LCD_RGBPACK(h_r >> 16, h_g >> 16, h_b >> 16);
            }
            fg_pattern = lst_pattern;
        }
        lcd_putsxyofs(xpos, ypos, s->offset, s->line);
        drawmode = lastmode;
        lcd_update_rect(xpos, ypos, LCD_WIDTH - xpos, pf->height);
    }

    fg_pattern = old_fgcolor;
    bg_pattern = old_bgcolor;
}
