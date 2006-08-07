/***************************************************************************
*             __________               __   ___.
*   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
*   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
*   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
*   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
*                     \/            \/     \/    \/            \/
* $Id$
*
* Greyscale framework
* Core & miscellaneous functions
*
* This is a generic framework to display up to 33 shades of grey
* on low-depth bitmap LCDs (Archos b&w, Iriver 4-grey, iPod 4-grey)
* within plugins.
*
* Copyright (C) 2004-2006 Jens Arnold
*
* All files in this archive are subject to the GNU General Public License.
* See the file COPYING in the source tree root for full license agreement.
*
* This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
* KIND, either express or implied.
*
****************************************************************************/

#include "plugin.h"

#ifdef HAVE_LCD_BITMAP
#include "gray.h"

#if defined(CPU_PP) && defined(HAVE_ADJUSTABLE_CPU_FREQ)
#define NEED_BOOST
#endif

/* Global variables */
struct plugin_api *_gray_rb = NULL; /* global api struct pointer */
struct _gray_info _gray_info;       /* global info structure */

#ifndef SIMULATOR
short _gray_random_buffer;          /* buffer for random number generator */

#if CONFIG_LCD == LCD_SSD1815
/* measured and interpolated curve */
static const unsigned char lcdlinear[256] = {
      0,   3,   5,   8,  11,  13,  16,  18,
     21,  23,  26,  28,  31,  33,  36,  38,
     40,  42,  45,  47,  49,  51,  53,  55,
     57,  59,  60,  62,  64,  66,  67,  69,
     70,  72,  73,  74,  76,  77,  78,  79,
     81,  82,  83,  84,  85,  86,  87,  88,
     88,  89,  90,  91,  92,  92,  93,  94,
     95,  95,  96,  97,  97,  98,  99,  99,
    100, 101, 102, 102, 103, 104, 104, 105,
    106, 106, 107, 107, 108, 109, 109, 110,
    111, 111, 112, 113, 113, 114, 114, 115,
    116, 116, 117, 117, 118, 119, 119, 120,
    120, 121, 121, 122, 122, 123, 123, 124,
    124, 125, 125, 126, 126, 127, 127, 128,
    128, 128, 129, 129, 130, 130, 131, 131,
    132, 132, 133, 133, 133, 134, 134, 135,
    135, 136, 136, 137, 137, 138, 138, 138,
    139, 139, 140, 140, 141, 141, 142, 142,
    143, 143, 144, 144, 145, 145, 146, 146,
    147, 147, 148, 148, 148, 149, 149, 150,
    150, 151, 151, 152, 152, 153, 153, 153,
    154, 154, 155, 155, 156, 156, 157, 157,
    158, 158, 158, 159, 159, 160, 160, 161,
    161, 162, 162, 163, 163, 164, 164, 165,
    165, 166, 167, 167, 168, 168, 169, 169,
    170, 171, 171, 172, 173, 173, 174, 175,
    176, 176, 177, 178, 179, 180, 181, 181,
    182, 183, 184, 185, 186, 188, 189, 190,
    191, 192, 194, 195, 196, 198, 199, 201,
    202, 204, 205, 207, 209, 211, 213, 215,
    217, 219, 222, 224, 226, 229, 231, 234,
    236, 239, 242, 244, 247, 250, 252, 255
};
#elif CONFIG_LCD == LCD_S1D15E06
/* measured and interpolated curve */
static const unsigned char lcdlinear[256] = {
      0,   5,  11,  16,  21,  27,  32,  37,
     42,  47,  51,  56,  60,  64,  68,  72,
     75,  78,  81,  84,  87,  89,  91,  93,
     95,  96,  98,  99, 101, 102, 103, 104,
    105, 106, 107, 108, 109, 110, 111, 111,
    112, 113, 113, 114, 115, 115, 116, 117,
    117, 118, 118, 119, 119, 120, 120, 121,
    121, 122, 122, 123, 123, 124, 124, 125,
    125, 126, 126, 127, 127, 127, 128, 128,
    129, 129, 130, 130, 131, 131, 132, 132,
    133, 133, 134, 134, 135, 135, 136, 136,
    137, 137, 138, 138, 138, 139, 139, 140,
    140, 141, 141, 141, 142, 142, 143, 143,
    143, 144, 144, 145, 145, 145, 146, 146,
    146, 147, 147, 147, 148, 148, 149, 149,
    149, 150, 150, 150, 151, 151, 151, 152,
    152, 153, 153, 153, 154, 154, 155, 155,
    155, 156, 156, 157, 157, 157, 158, 158,
    159, 159, 159, 160, 160, 161, 161, 162,
    162, 162, 163, 163, 164, 164, 164, 165,
    165, 166, 166, 167, 167, 167, 168, 168,
    169, 169, 170, 170, 170, 171, 171, 172,
    172, 173, 173, 174, 174, 175, 175, 176,
    176, 177, 177, 178, 178, 179, 179, 180,
    180, 181, 182, 182, 183, 184, 184, 185,
    186, 186, 187, 188, 188, 189, 190, 191,
    191, 192, 193, 194, 195, 196, 196, 197,
    198, 199, 200, 201, 202, 203, 204, 205,
    206, 207, 208, 209, 210, 211, 213, 214,
    215, 216, 218, 219, 220, 222, 223, 225,
    227, 228, 230, 232, 233, 235, 237, 239,
    241, 243, 245, 247, 249, 251, 253, 255
};
#elif (CONFIG_LCD == LCD_IPOD2BPP) || (CONFIG_LCD == LCD_IPODMINI)
/* measured and interpolated curve for mini LCD */
/* TODO: verify this curve on the fullsize greyscale LCD */
static const unsigned char lcdlinear[256] = {
      0,   3,   6,   8,  11,  14,  17,  19,
     22,  24,  27,  29,  32,  34,  36,  38,
     40,  42,  44,  45,  47,  48,  50,  51,
     52,  54,  55,  56,  57,  58,  58,  59,
     60,  61,  62,  62,  63,  64,  64,  65,
     66,  66,  67,  67,  68,  68,  69,  69,
     70,  70,  70,  71,  71,  71,  72,  72,
     73,  73,  73,  74,  74,  74,  74,  75,
     75,  75,  76,  76,  76,  77,  77,  77,
     78,  78,  78,  79,  79,  79,  80,  80,
     80,  80,  81,  81,  81,  82,  82,  82,
     83,  83,  83,  84,  84,  84,  85,  85,
     85,  85,  86,  86,  86,  87,  87,  87,
     87,  88,  88,  88,  89,  89,  89,  89,
     90,  90,  90,  91,  91,  91,  92,  92,
     92,  93,  93,  93,  94,  94,  94,  95,
     95,  96,  96,  96,  97,  97,  98,  98,
     99,  99,  99, 100, 100, 101, 101, 102,
    102, 103, 103, 104, 104, 105, 105, 106,
    106, 107, 107, 108, 108, 109, 109, 110,
    110, 111, 111, 112, 113, 113, 114, 114,
    115, 115, 116, 117, 117, 118, 118, 119,
    120, 120, 121, 122, 122, 123, 124, 124,
    125, 126, 126, 127, 128, 128, 129, 130,
    131, 131, 132, 133, 134, 134, 135, 136,
    137, 138, 139, 140, 141, 142, 143, 144,
    145, 146, 147, 148, 149, 150, 152, 153,
    154, 156, 157, 159, 160, 162, 163, 165,
    167, 168, 170, 172, 174, 176, 178, 180,
    182, 184, 187, 189, 192, 194, 197, 200,
    203, 206, 209, 212, 215, 219, 222, 226,
    229, 233, 236, 240, 244, 248, 251, 255
};
#endif
#else /* SIMULATOR */
/* undo a (generic) PC display gamma of 2.0 to simulate target behaviour */
static const unsigned char lcdlinear[256] = {
      0,  16,  23,  28,  32,  36,  39,  42,
     45,  48,  50,  53,  55,  58,  60,  62,
     64,  66,  68,  70,  71,  73,  75,  77,
     78,  80,  81,  83,  84,  86,  87,  89,
     90,  92,  93,  94,  96,  97,  98, 100,
    101, 102, 103, 105, 106, 107, 108, 109,
    111, 112, 113, 114, 115, 116, 117, 118,
    119, 121, 122, 123, 124, 125, 126, 127,
    128, 129, 130, 131, 132, 133, 134, 135,
    135, 136, 137, 138, 139, 140, 141, 142,
    143, 144, 145, 145, 146, 147, 148, 149,
    150, 151, 151, 152, 153, 154, 155, 156,
    156, 157, 158, 159, 160, 160, 161, 162,
    163, 164, 164, 165, 166, 167, 167, 168,
    169, 170, 170, 171, 172, 173, 173, 174,
    175, 176, 176, 177, 178, 179, 179, 180,
    181, 181, 182, 183, 183, 184, 185, 186,
    186, 187, 188, 188, 189, 190, 190, 191,
    192, 192, 193, 194, 194, 195, 196, 196,
    197, 198, 198, 199, 199, 200, 201, 201,
    202, 203, 203, 204, 204, 205, 206, 206,
    207, 208, 208, 209, 209, 210, 211, 211,
    212, 212, 213, 214, 214, 215, 215, 216,
    217, 217, 218, 218, 219, 220, 220, 221,
    221, 222, 222, 223, 224, 224, 225, 225,
    226, 226, 227, 228, 228, 229, 229, 230,
    230, 231, 231, 232, 233, 233, 234, 234,
    235, 235, 236, 236, 237, 237, 238, 238,
    239, 240, 240, 241, 241, 242, 242, 243,
    243, 244, 244, 245, 245, 246, 246, 247,
    247, 248, 248, 249, 249, 250, 250, 251,
    251, 252, 252, 253, 253, 254, 254, 255
};
#endif /* SIMULATOR */

/* Prototypes */
static inline void _deferred_update(void) __attribute__ ((always_inline));
static int exp_s16p16(int x);
static int log_s16p16(int x);
static void gray_screendump_hook(int fd);
#ifdef SIMULATOR
static unsigned long _gray_get_pixel(int x, int y);
#else
static void _timer_isr(void);
#endif

/* Update LCD areas not covered by the greyscale overlay */
static inline void _deferred_update(void)
{
    int x1 = MAX(_gray_info.x, 0);
    int x2 = MIN(_gray_info.x + _gray_info.width, LCD_WIDTH);
    int y1 = MAX(_gray_info.y, 0);
    int y2 = MIN(_gray_info.y + _gray_info.height, LCD_HEIGHT);

    if (y1 > 0)  /* refresh part above overlay, full width */
        _gray_rb->lcd_update_rect(0, 0, LCD_WIDTH, y1);

    if (y2 < LCD_HEIGHT) /* refresh part below overlay, full width */
        _gray_rb->lcd_update_rect(0, y2, LCD_WIDTH, LCD_HEIGHT - y2);

    if (x1 > 0) /* refresh part to the left of overlay */
        _gray_rb->lcd_update_rect(0, y1, x1, y2 - y1);

    if (x2 < LCD_WIDTH) /* refresh part to the right of overlay */
        _gray_rb->lcd_update_rect(x2, y1, LCD_WIDTH - x2, y2 - y1);
}

#ifndef SIMULATOR
/* Timer interrupt handler: display next bitplane */
static void _timer_isr(void)
{
#if LCD_PIXELFORMAT == HORIZONTAL_PACKING
    _gray_rb->lcd_blit(_gray_info.plane_data + MULU16(_gray_info.plane_size,
                       _gray_info.cur_plane), _gray_info.bx, _gray_info.y,
                       _gray_info.bwidth, _gray_info.height, _gray_info.bwidth);
#else
    _gray_rb->lcd_blit(_gray_info.plane_data + MULU16(_gray_info.plane_size,
                       _gray_info.cur_plane), _gray_info.x, _gray_info.by,
                       _gray_info.width, _gray_info.bheight, _gray_info.width);
#endif

    if (++_gray_info.cur_plane >= _gray_info.depth)
        _gray_info.cur_plane = 0;

    if (_gray_info.flags & _GRAY_DEFERRED_UPDATE)  /* lcd_update() requested? */
    {
        _deferred_update();
        _gray_info.flags &= ~_GRAY_DEFERRED_UPDATE; /* clear request */
    }
}
#endif /* !SIMULATOR */

/* fixed point exp() */
static int exp_s16p16(int x)
{
    int t;
    int y = 0x00010000;
    
    if (x < 0) x += 0xb1721,            y >>= 16;
    t = x - 0x58b91; if (t >= 0) x = t, y <<= 8;
    t = x - 0x2c5c8; if (t >= 0) x = t, y <<= 4;
    t = x - 0x162e4; if (t >= 0) x = t, y <<= 2;
    t = x - 0x0b172; if (t >= 0) x = t, y <<= 1;
    t = x - 0x067cd; if (t >= 0) x = t, y += y >> 1;
    t = x - 0x03920; if (t >= 0) x = t, y += y >> 2;
    t = x - 0x01e27; if (t >= 0) x = t, y += y >> 3;
    t = x - 0x00f85; if (t >= 0) x = t, y += y >> 4;
    t = x - 0x007e1; if (t >= 0) x = t, y += y >> 5;
    t = x - 0x003f8; if (t >= 0) x = t, y += y >> 6;
    t = x - 0x001fe; if (t >= 0) x = t, y += y >> 7;
    y += ((y >> 8) * x) >> 8;

    return y;
}

/* fixed point log() */
static int log_s16p16(int x)
{
    int t;
    int y = 0xa65af;

    if (x < 0x00008000) x <<=16,                        y -= 0xb1721;
    if (x < 0x00800000) x <<= 8,                        y -= 0x58b91;
    if (x < 0x08000000) x <<= 4,                        y -= 0x2c5c8;
    if (x < 0x20000000) x <<= 2,                        y -= 0x162e4;
    if (x < 0x40000000) x <<= 1,                        y -= 0x0b172;
    t = x + (x >> 1); if ((t & 0x80000000) == 0) x = t, y -= 0x067cd;
    t = x + (x >> 2); if ((t & 0x80000000) == 0) x = t, y -= 0x03920;
    t = x + (x >> 3); if ((t & 0x80000000) == 0) x = t, y -= 0x01e27;
    t = x + (x >> 4); if ((t & 0x80000000) == 0) x = t, y -= 0x00f85;
    t = x + (x >> 5); if ((t & 0x80000000) == 0) x = t, y -= 0x007e1;
    t = x + (x >> 6); if ((t & 0x80000000) == 0) x = t, y -= 0x003f8;
    t = x + (x >> 7); if ((t & 0x80000000) == 0) x = t, y -= 0x001fe;
    x = 0x80000000 - x;
    y -= x >> 15;

    return y;
}

/* Initialise the framework and prepare the greyscale display buffer

 arguments:
   newrb     = pointer to plugin api
   gbuf      = pointer to the memory area to use (e.g. plugin buffer)
   gbuf_size = max usable size of the buffer
   buffered  = use chunky pixel buffering with delta buffer?
               This allows to use all drawing functions, but needs more
               memory. Unbuffered operation provides only a subset of
               drawing functions. (only gray_bitmap drawing and scrolling)
   width     = width in pixels  (1..LCD_WIDTH)
   height    = height in pixels (1..LCD_HEIGHT)
               Note that depending on the target LCD, either height or
               width are rounded up to a multiple of 8.
   depth     = number of bitplanes to use (1..32).
   gamma     = gamma value as s8p8 fixed point. gamma <= 0 means no
               correction at all, i.e. no LCD linearisation as well.

 result:
   = depth  if there was enough memory
   < depth  if there wasn't enough memory. The number of displayable
            shades is smaller than desired, but it still works
   = 0      if there wasn't even enough memory for 1 bitplane

   You can request any depth in the allowed range, not just powers of 2. The
   routine performs "graceful degradation" if the memory is not sufficient for
   the desired depth. As long as there is at least enough memory for 1 bitplane,
   it creates as many bitplanes as fit into memory, although 1 bitplane won't
   deliver an enhancement over the native display.
 
   The number of displayable shades is calculated as follows:
   shades = depth + 1

   If you need info about the memory taken by the greyscale buffer, supply a
   long* as the last parameter. This long will then contain the number of bytes
   used. The total memory needed can be calculated as follows:
 total_mem =
     shades * sizeof(long)               (bitpatterns)
   + [horizontal_packing] ?              (bitplane data)
       ((width + 7) / 8) * height * depth : width * ((height + 7) / 8) * depth
   + buffered ?                          (chunky front- & backbuffer)
       (width * height * 2) : 0
   + 0..3                                (longword alignment)

   The function tries to be as authentic as possible regarding memory usage on
   the simulator, even if it doesn't use all of the allocated memory. There's
   one situation where it will consume more memory on the sim than on the
   target: if you're allocating a low depth (< 8) without buffering. */
int gray_init(struct plugin_api* newrb, unsigned char *gbuf, long gbuf_size,
              bool buffered, int width, int height, int depth, int gamma,
              long *buf_taken)
{
    int possible_depth, bdim, i;
    long plane_size, buftaken;
    unsigned data;
#ifndef SIMULATOR
    int j;
#endif

    _gray_rb = newrb;

    if ((unsigned) width > LCD_WIDTH
        || (unsigned) height > LCD_HEIGHT
        || depth < 1)
        return 0;

#if LCD_PIXELFORMAT == HORIZONTAL_PACKING
    bdim = (width + 7) >> 3;
    width = bdim << 3;
#else
    bdim = (height + 7) >> 3;
    height = bdim << 3;
#endif

    /* the buffer has to be long aligned */
    buftaken = (-(long)gbuf) & 3;
    gbuf += buftaken;

    /* chunky front- & backbuffer */
    if (buffered)  
    {
        plane_size = MULU16(width, height);
        buftaken += 2 * plane_size;
        if (buftaken > gbuf_size)
            return 0;

        _gray_info.cur_buffer = gbuf;
        gbuf += plane_size;
        /* set backbuffer to 0xFF to guarantee the initial full update */
        _gray_rb->memset(gbuf, 0xFF, plane_size);
        _gray_info.back_buffer = gbuf;
        gbuf += plane_size;
    }

#if LCD_PIXELFORMAT == HORIZONTAL_PACKING
    plane_size = MULU16(bdim, height);
#else
    plane_size = MULU16(width, bdim);
#endif
    possible_depth = (gbuf_size - buftaken - sizeof(long))
                     / (plane_size + sizeof(long));

    if (possible_depth < 1)
        return 0;

    depth = MIN(depth, 32);
    depth = MIN(depth, possible_depth);

#ifdef SIMULATOR
    if (!buffered)
    {
        long orig_size = depth * plane_size + (depth + 1) * sizeof(long);
        
        plane_size = MULU16(width, height);
        if (plane_size > orig_size)
        {
            buftaken += plane_size;
            if (buftaken > gbuf_size)
                return 0;
        }
        else
        {
            buftaken += orig_size;
        }
        _gray_info.cur_buffer = gbuf;
    }
    else
#endif
        buftaken += depth * plane_size + (depth + 1) * sizeof(long);

    _gray_info.x = 0;
    _gray_info.y = 0;
    _gray_info.width = width;
    _gray_info.height = height;
#if LCD_PIXELFORMAT == HORIZONTAL_PACKING
    _gray_info.bx = 0;
    _gray_info.bwidth = bdim;
#else
    _gray_info.by = 0;
    _gray_info.bheight = bdim;
#endif
    _gray_info.depth = depth;
    _gray_info.flags = 0;
#ifndef SIMULATOR
    _gray_info.cur_plane = 0;
    _gray_info.plane_size = plane_size;
    _gray_info.plane_data = gbuf;
    gbuf += depth * plane_size;
    _gray_info.bitpattern = (unsigned long *)gbuf;
              
    i = depth - 1;
    j = 8;
    while (i != 0)
    {
        i >>= 1;
        j--;
    }
    _gray_info.randmask = 0xFFu >> j;

    /* Precalculate the bit patterns for all possible pixel values */
    for (i = 0; i <= depth; i++)
    {
        unsigned long pattern = 0;
        int value = 0;

        for (j = 0; j < depth; j++)
        {
            pattern <<= 1;
            value += i;

            if (value >= depth)
                value -= depth;   /* "white" bit */
            else
                pattern |= 1;     /* "black" bit */
        }
        /* now the lower <depth> bits contain the pattern */

        _gray_info.bitpattern[i] = pattern;
    }
#endif

    /* precalculate the value -> pattern index conversion table, taking 
       linearisation and gamma correction into account */
    if (gamma <= 0)
    {
        for (i = 0; i < 256; i++)
        {
            data = MULU16(depth, i) + 127;
            _gray_info.idxtable[i] = (data + (data >> 8)) >> 8;
                                      /* approx. data / 255 */
        }
    }
    else
    {
        for (i = 0; i < 256; i++)
        {
            data = exp_s16p16((gamma * log_s16p16(i * 257 + 1)) >> 8) + 128;
            data = (data - (data >> 8)) >> 8; /* approx. data /= 257 */
            data = MULU16(depth, lcdlinear[data]) + 127;
            _gray_info.idxtable[i] = (data + (data >> 8)) >> 8;
                                      /* approx. data / 255 */
        }
    }

    _gray_info.fg_index = 0;
    _gray_info.bg_index = depth;
    _gray_info.fg_brightness = 0;
    _gray_info.bg_brightness = 255;
    _gray_info.drawmode = DRMODE_SOLID;
    _gray_info.curfont = FONT_SYSFIXED;

    if (buf_taken)  /* caller requested info about space taken */
        *buf_taken = buftaken;

    return depth;
}

/* Release the greyscale display buffer and the library
   DO CALL either this function or at least gray_show_display(false)
   before you exit, otherwise nasty things may happen. */
void gray_release(void)
{
    gray_show(false);
}

/* Switch the greyscale overlay on or off
   DO NOT call lcd_update() or any other api function that directly accesses
   the lcd while the greyscale overlay is running! If you need to do
   lcd_update() to update something outside the greyscale overlay area, use
   gray_deferred_update() instead.

 Other functions to avoid are:
   lcd_blit() (obviously), lcd_update_rect(), lcd_set_contrast(),
   lcd_set_invert_display(), lcd_set_flip(), lcd_roll() */
void gray_show(bool enable)
{
    if (enable && !(_gray_info.flags & _GRAY_RUNNING))
    {
        _gray_info.flags |= _GRAY_RUNNING;
#ifdef SIMULATOR
        _gray_rb->sim_lcd_ex_init(_gray_info.depth + 1, _gray_get_pixel);
        gray_update();
#else /* !SIMULATOR */
#ifdef NEED_BOOST
        _gray_rb->cpu_boost(true);
#endif
#if CONFIG_LCD == LCD_SSD1815
        _gray_rb->timer_register(1, NULL, TIMER_FREQ / 67, 1, _timer_isr);
#elif CONFIG_LCD == LCD_S1D15E06
        _gray_rb->timer_register(1, NULL, TIMER_FREQ / 70, 1, _timer_isr);
#elif CONFIG_LCD == LCD_IPOD2BPP
        /* FIXME: verify value */
        _gray_rb->timer_register(1, NULL, TIMER_FREQ / 80, 1, _timer_isr);
#elif CONFIG_LCD == LCD_IPODMINI
        _gray_rb->timer_register(1, NULL, TIMER_FREQ / 88, 1, _timer_isr);
#elif CONFIG_LCD == LCD_IFP7XX
        (void)_timer_isr;   /* TODO: implement for iFP */
#endif /* CONFIG_LCD */
#endif /* !SIMULATOR */
        _gray_rb->screen_dump_set_hook(gray_screendump_hook);
    }
    else if (!enable && (_gray_info.flags & _GRAY_RUNNING))
    {
#ifdef SIMULATOR
        _gray_rb->sim_lcd_ex_init(0, NULL);
#else
        _gray_rb->timer_unregister();
#ifdef NEED_BOOST
        _gray_rb->cpu_boost(false);
#endif
#endif
        _gray_info.flags &= ~_GRAY_RUNNING;
        _gray_rb->screen_dump_set_hook(NULL);
        _gray_rb->lcd_update(); /* restore whatever there was before */
    }
}

#ifdef SIMULATOR
/* Callback function for gray_update_rect() to read a pixel from the graybuffer.
   Note that x and y are in LCD coordinates, not graybuffer coordinates! */
static unsigned long _gray_get_pixel(int x, int y)
{
#if LCD_PIXELFORMAT == HORIZONTAL_PACKING
    return _gray_info.cur_buffer[MULU16(y - _gray_info.y, _gray_info.width)
                                 + x - _gray_info.x]
           + (1 << LCD_DEPTH);
#else
    return _gray_info.cur_buffer[MULU16(x - _gray_info.x, _gray_info.height)
                                 + y - _gray_info.y]
           + (1 << LCD_DEPTH);
#endif
}

/* Update a rectangular area of the greyscale overlay */
void gray_update_rect(int x, int y, int width, int height)
{
    if (x + width > _gray_info.width)
        width = _gray_info.width - x;
    if (y + height > _gray_info.height)
        height = _gray_info.height - y;
        
    x += _gray_info.x;
    y += _gray_info.y;

    if (x + width > LCD_WIDTH)
        width = LCD_WIDTH - x;
    if (y + height > LCD_HEIGHT)
        height = LCD_HEIGHT - y;
        
    _gray_rb->sim_lcd_ex_update_rect(x, y, width, height);
}

#else /* !SIMULATOR */

#if LCD_PIXELFORMAT == HORIZONTAL_PACKING

/* Update a rectangular area of the greyscale overlay */
void gray_update_rect(int x, int y, int width, int height)
{
    int xmax, bwidth;
    long srcofs;
    unsigned char *dst;

    if ((width <= 0) || (height <= 0))
        return; /* nothing to do */

    /* The X coordinates have to work on whole pixel block columns */
    xmax = (x + width - 1) >> 3;
    x >>= 3;

    if (y + height > _gray_info.height)
        height = _gray_info.height - y;
    if (xmax >= _gray_info.bwidth)
        xmax = _gray_info.bwidth - 1;
    bwidth = xmax - x + 1;
        
    srcofs = MULU16(_gray_info.width, y) + (x << 3);
    dst = _gray_info.plane_data + MULU16(_gray_info.bwidth, y) + x;
    
    /* Copy specified rectangle bitmap to hardware */
    for (; height > 0; height--)
    {
        long srcofs_row = srcofs;
        unsigned char *dst_row = dst;
        unsigned char *dst_end = dst_row + bwidth;

        do
        {
            unsigned long pat_stack[8];
            unsigned long *pat_ptr;
            unsigned char *cbuf, *bbuf;
            unsigned change;

            cbuf = _gray_info.cur_buffer + srcofs_row;
            bbuf = _gray_info.back_buffer + srcofs_row;

#ifdef CPU_ARM
            asm volatile (            
                "ldr     r0, [%[cbuf]]           \n"
                "ldr     r1, [%[bbuf]]           \n"
                "eor     r1, r0, r1              \n"
                "ldr     r0, [%[cbuf], #4]       \n"
                "ldr     %[chg], [%[bbuf], #4]   \n"
                "eor     %[chg], r0, %[chg]      \n"
                "orr     %[chg], %[chg], r1      \n"
                : /* outputs */
                [chg] "=&r"(change)
                : /* inputs */
                [cbuf]"r"(cbuf),
                [bbuf]"r"(bbuf)
                : /* clobbers */
                "r0", "r1"
            );

            if (change != 0)
            {
                unsigned char *addr, *end;
                unsigned mask, trash;
                
                pat_ptr = &pat_stack[8];
                
                /* precalculate the bit patterns with random shifts
                 * for all 8 pixels and put them on an extra "stack" */
                asm volatile (
                    "mov     r3, #8                      \n"  /* loop count */
                    "mov     %[mask], #0                 \n"

                ".ur_pre_loop:                           \n"
                    "mov     %[mask], %[mask], lsl #1    \n"  /* shift mask */
                    "ldrb    r0, [%[cbuf]], #1           \n"  /* read current buffer */
                    "ldrb    r1, [%[bbuf]]               \n"  /* read back buffer */
                    "strb    r0, [%[bbuf]], #1           \n"  /* update back buffer */
                    "mov     r2, #0                      \n"  /* preset for skipped pixel */
                    "cmp     r0, r1                      \n"  /* no change? */
                    "beq     .ur_skip                    \n"  /* -> skip */

                    "ldr     r2, [%[bpat], r0, lsl #2]   \n"  /* r2 = bitpattern[byte]; */

                    "add     r0, %[rnd], %[rnd], lsl #3  \n"  /* multiply by 75 */
                    "add     %[rnd], %[rnd], %[rnd], lsl #1  \n"
                    "add     %[rnd], %[rnd], r0, lsl #3  \n"
                    "add     %[rnd], %[rnd], #74         \n"  /* add another 74 */
                    /* Since the lower bits are not very random:   get bits 8..15 (need max. 5) */
                    "and     r1, %[rmsk], %[rnd], lsr #8 \n"  /* ..and mask out unneeded bits */

                    "cmp     r1, %[dpth]                 \n"  /* random >= depth ? */
                    "subhs   r1, r1, %[dpth]             \n"  /* yes: random -= depth */

                    "mov     r0, r2, lsl r1              \n"  /** rotate pattern **/
                    "sub     r1, %[dpth], r1             \n"
                    "orr     r2, r0, r2, lsr r1          \n"

                    "orr     %[mask], %[mask], #1        \n"  /* set mask bit */
                    
                ".ur_skip:                               \n"
                    "str     r2, [%[patp], #-4]!         \n"  /* push on pattern stack */

                    "subs    r3, r3, #1                  \n"  /* loop 8 times (pixel block) */
                    "bne     .ur_pre_loop                \n"
                    : /* outputs */
                    [cbuf]"+r"(cbuf),
                    [bbuf]"+r"(bbuf),
                    [patp]"+r"(pat_ptr),
                    [rnd] "+r"(_gray_random_buffer),
                    [mask]"=&r"(mask)
                    : /* inputs */
                    [bpat]"r"(_gray_info.bitpattern),
                    [dpth]"r"(_gray_info.depth),
                    [rmsk]"r"(_gray_info.randmask)
                    : /* clobbers */
                    "r0", "r1", "r2", "r3"
                );

                addr = dst_row;
                end = addr + MULU16(_gray_info.depth, _gray_info.plane_size);

                /* set the bits for all 8 pixels in all bytes according to the
                 * precalculated patterns on the pattern stack */
                asm volatile (
                    "ldmia   %[patp], {r2 - r8, %[rx]}   \n" /* pop all 8 patterns */
        
                    "mvn     %[mask], %[mask]        \n" /* "set" mask -> "keep" mask */
                    "ands    %[mask], %[mask], #0xff \n"
                    "beq     .ur_sloop               \n" /* short loop if nothing to keep */

                ".ur_floop:                          \n" /** full loop (there are bits to keep)**/
                    "movs    %[rx], %[rx], lsr #1    \n" /* shift out pattern bit */
                    "adc     r0, r0, r0              \n" /* put bit into LSB for byte */
                    "movs    r8, r8, lsr #1          \n"
                    "adc     r0, r0, r0              \n"
                    "movs    r7, r7, lsr #1          \n"
                    "adc     r0, r0, r0              \n"
                    "movs    r6, r6, lsr #1          \n"
                    "adc     r0, r0, r0              \n"
                    "movs    r5, r5, lsr #1          \n"
                    "adc     r0, r0, r0              \n"
                    "movs    r4, r4, lsr #1          \n"
                    "adc     r0, r0, r0              \n"
                    "movs    r3, r3, lsr #1          \n"
                    "adc     r0, r0, r0              \n"
                    "movs    r2, r2, lsr #1          \n"
                    "adc     r0, r0, r0              \n"

                    "ldrb    r1, [%[addr]]           \n" /* read old value */
                    "and     r1, r1, %[mask]         \n" /* mask out replaced bits */
                    "orr     r1, r1, r0              \n" /* set new bits */
                    "strb    r1, [%[addr]], %[psiz]  \n" /* store value, advance to next bpl */

                    "cmp     %[end], %[addr]         \n" /* loop for all bitplanes */
                    "bne     .ur_floop               \n"

                    "b       .ur_end                 \n"

                ".ur_sloop:                          \n" /** short loop (nothing to keep) **/
                    "movs    %[rx], %[rx], lsr #1    \n" /* shift out pattern bit */
                    "adc     r0, r0, r0              \n" /* put bit into LSB for byte */
                    "movs    r8, r8, lsr #1          \n"
                    "adc     r0, r0, r0              \n"
                    "movs    r7, r7, lsr #1          \n"
                    "adc     r0, r0, r0              \n"
                    "movs    r6, r6, lsr #1          \n"
                    "adc     r0, r0, r0              \n"
                    "movs    r5, r5, lsr #1          \n"
                    "adc     r0, r0, r0              \n"
                    "movs    r4, r4, lsr #1          \n"
                    "adc     r0, r0, r0              \n"
                    "movs    r3, r3, lsr #1          \n"
                    "adc     r0, r0, r0              \n"
                    "movs    r2, r2, lsr #1          \n"
                    "adc     r0, r0, r0              \n"

                    "strb    r0, [%[addr]], %[psiz]  \n" /* store byte, advance to next bpl */

                    "cmp     %[end], %[addr]         \n" /* loop for all bitplanes */
                    "bne     .ur_sloop               \n"

                ".ur_end:                            \n"
                    : /* outputs */
                    [addr]"+r"(addr),
                    [mask]"+r"(mask),
                    [rx]  "=&r"(trash)
                    : /* inputs */
                    [psiz]"r"(_gray_info.plane_size),
                    [end] "r"(end),
                    [patp]"[rx]"(pat_ptr)
                    : /* clobbers */
                    "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7", "r8"
                );
             }
#else /* C version, for reference*/
#warning C version of gray_update_rect() used
            (void)pat_ptr;
            /* check whether anything changed in the 8-pixel block */
            change  = *(uint32_t *)cbuf ^ *(uint32_t *)bbuf;
            change |= *(uint32_t *)(cbuf + 4) ^ *(uint32_t *)(bbuf + 4);

            if (change != 0)
            {
                unsigned char *addr, *end;
                unsigned mask = 0;
                unsigned test = 1;
                int i;

                /* precalculate the bit patterns with random shifts
                 * for all 8 pixels and put them on an extra "stack" */
                for (i = 7; i >= 0; i--)
                {
                    unsigned pat = 0;
                    unsigned char cur = *cbuf++;
                    unsigned char back = *bbuf;
                    
                    *bbuf++ = cur;

                    mask <<= 1;
                    if (cur != back)
                    {
                        int shift;

                        pat = _gray_info.bitpattern[cur];

                        /* shift pattern pseudo-random, simple & fast PRNG */
                        _gray_random_buffer = 75 * _gray_random_buffer + 74;
                        shift = (_gray_random_buffer >> 8) & _gray_info.randmask;
                        if (shift >= _gray_info.depth)
                            shift -= _gray_info.depth;
                            
                        pat = (pat << shift) | (pat >> (_gray_info.depth - shift));
                        
                        mask |= 1;
                    }
                    pat_stack[i] = pat;
                }

                addr = dst_row;
                end = addr + MULU16(_gray_info.depth, _gray_info.plane_size);

                /* set the bits for all 8 pixels in all bytes according to the
                 * precalculated patterns on the pattern stack */
                mask = (~mask & 0xff);
                if (mask == 0)
                {
                    do
                    {
                        unsigned data = 0;

                        for (i = 7; i >= 0; i--)
                            data = (data << 1) | ((pat_stack[i] & test) ? 1 : 0);

                        *addr = data;
                        addr += _gray_info.plane_size;
                        test <<= 1;
                    }
                    while (addr < end);
                }
                else
                {
                    do
                    {
                        unsigned data = 0;

                        for (i = 7; i >= 0; i--)
                            data = (data << 1) | ((pat_stack[i] & test) ? 1 : 0);
                        
                        *addr = (*addr & mask) | data;
                        addr += _gray_info.plane_size;
                        test <<= 1;
                    }
                    while (addr < end);
                }

            }
#endif /* CONFIG_CPU */
            srcofs_row += 8;
            dst_row++;
        }
        while (dst_row < dst_end);
        
        srcofs += _gray_info.width;
        dst += _gray_info.bwidth;
    }
}
#else /* LCD_PIXELFORMAT == VERTICAL_PACKING */

/* Update a rectangular area of the greyscale overlay */
void gray_update_rect(int x, int y, int width, int height)
{
    int ymax;
    long srcofs;
    unsigned char *dst;

    if ((width <= 0) || (height <= 0))
        return; /* nothing to do */

    /* The Y coordinates have to work on whole pixel block rows */
    ymax = (y + height - 1) >> 3;
    y >>= 3;

    if (x + width > _gray_info.width)
        width = _gray_info.width - x;
    if (ymax >= _gray_info.bheight)
        ymax = _gray_info.bheight - 1;
        
    srcofs = (y << 3) + MULU16(_gray_info.height, x);
    dst = _gray_info.plane_data + MULU16(_gray_info.width, y) + x;
    
    /* Copy specified rectangle bitmap to hardware */
    for (; y <= ymax; y++)
    {
        long srcofs_row = srcofs;
        unsigned char *dst_row = dst;
        unsigned char *dst_end = dst_row + width;

        do
        {
            unsigned long pat_stack[8];
            unsigned long *pat_ptr;
            unsigned char *cbuf, *bbuf;
            unsigned change;

            cbuf = _gray_info.cur_buffer + srcofs_row;
            bbuf = _gray_info.back_buffer + srcofs_row;

#if CONFIG_CPU == SH7034
            asm volatile (
                "mov.l   @%[cbuf],r1         \n"
                "mov.l   @%[bbuf],r2         \n"
                "xor     r1,r2               \n"
                "mov.l   @(4,%[cbuf]),r1     \n"
                "mov.l   @(4,%[bbuf]),%[chg] \n"
                "xor     r1,%[chg]           \n"
                "or      r2,%[chg]           \n"
                : /* outputs */
                [chg] "=r"(change)
                : /* inputs */
                [cbuf]"r"(cbuf),
                [bbuf]"r"(bbuf)
                : /* clobbers */
                 "r1", "r2"
            );

            if (change != 0)
            {
                unsigned char *addr, *end;
                unsigned mask, trash;

                pat_ptr = &pat_stack[8];

                /* precalculate the bit patterns with random shifts
                 * for all 8 pixels and put them on an extra "stack" */
                asm volatile (
                    "mov     #8,r3       \n"  /* loop count */

                ".ur_pre_loop:           \n"
                    "mov.b   @%[cbuf]+,r0\n"  /* read current buffer */
                    "mov.b   @%[bbuf],r1 \n"  /* read back buffer */
                    "mov     #0,r2       \n"  /* preset for skipped pixel */
                    "mov.b   r0,@%[bbuf] \n"  /* update back buffer */
                    "add     #1,%[bbuf]  \n"
                    "cmp/eq  r0,r1       \n"  /* no change? */
                    "bt      .ur_skip    \n"  /* -> skip */

                    "shll2   r0          \n"  /* pixel value -> pattern offset */
                    "mov.l   @(r0,%[bpat]),r4\n"  /* r4 = bitpattern[byte]; */

                    "mov     #75,r0      \n"
                    "mulu    r0,%[rnd]   \n"  /* multiply by 75 */
                    "sts     macl,%[rnd] \n"
                    "add     #74,%[rnd]  \n"  /* add another 74 */
                    /* Since the lower bits are not very random: */
                    "swap.b  %[rnd],r1   \n"  /* get bits 8..15 (need max. 5) */
                    "and     %[rmsk],r1  \n"  /* mask out unneeded bits */

                    "cmp/hs  %[dpth],r1  \n"  /* random >= depth ? */
                    "bf      .ur_ntrim   \n"
                    "sub     %[dpth],r1  \n"  /* yes: random -= depth; */
                ".ur_ntrim:              \n"
                
                    "mov.l   .ashlsi3,r0 \n"  /** rotate pattern **/
                    "jsr     @r0         \n"  /* r4 -> r0, shift left by r5 */
                    "mov     r1,r5       \n"

                    "mov     %[dpth],r5  \n"
                    "sub     r1,r5       \n"  /* r5 = depth - r1 */
                    "mov.l   .lshrsi3,r1 \n"
                    "jsr     @r1         \n"  /* r4 -> r0, shift right by r5 */
                    "mov     r0,r2       \n"  /* store previous result in r2 */
                                         
                    "or      r0,r2       \n"  /* rotated_pattern = r2 | r0 */
                    "clrt                \n"  /* mask bit = 0 (replace) */

                ".ur_skip:               \n"  /* T == 1 if skipped */
                    "rotcr   %[mask]     \n"  /* get mask bit */
                    "mov.l   r2,@-%[patp]\n"  /* push on pattern stack */

                    "add     #-1,r3      \n"  /* loop 8 times (pixel block) */
                    "cmp/pl  r3          \n"
                    "bt      .ur_pre_loop\n"  

                    "shlr8   %[mask]     \n"  /* shift mask to low byte */
                    "shlr16  %[mask]     \n"
                    : /* outputs */
                    [cbuf]"+r"(cbuf),
                    [bbuf]"+r"(bbuf),
                    [rnd] "+r"(_gray_random_buffer),
                    [patp]"+r"(pat_ptr),
                    [mask]"=&r"(mask)
                    : /* inputs */
                    [dpth]"r"(_gray_info.depth),
                    [bpat]"r"(_gray_info.bitpattern),
                    [rmsk]"r"(_gray_info.randmask)
                    : /* clobbers */
                    "r0", "r1", "r2", "r3", "r4", "r5", "macl", "pr"
                );

                addr = dst_row;
                end = addr + MULU16(_gray_info.depth, _gray_info.plane_size);

                /* set the bits for all 8 pixels in all bytes according to the
                 * precalculated patterns on the pattern stack */
                asm volatile (
                    "mov.l   @%[patp]+,r1    \n"  /* pop all 8 patterns */
                    "mov.l   @%[patp]+,r2    \n"
                    "mov.l   @%[patp]+,r3    \n"
                    "mov.l   @%[patp]+,r6    \n"
                    "mov.l   @%[patp]+,r7    \n"
                    "mov.l   @%[patp]+,r8    \n"
                    "mov.l   @%[patp]+,r9    \n"
                    "mov.l   @%[patp],r10    \n"

                    "tst     %[mask],%[mask] \n"
                    "bt      .ur_sloop       \n"  /* short loop if nothing to keep */

                ".ur_floop:                  \n"  /** full loop (there are bits to keep)**/
                    "shlr    r1              \n"  /* rotate lsb of pattern 1 to t bit */
                    "rotcl   r0              \n"  /* rotate t bit into r0 */
                    "shlr    r2              \n"
                    "rotcl   r0              \n"
                    "shlr    r3              \n"
                    "rotcl   r0              \n"
                    "shlr    r6              \n"
                    "rotcl   r0              \n"
                    "shlr    r7              \n"
                    "rotcl   r0              \n"
                    "shlr    r8              \n"
                    "rotcl   r0              \n"
                    "shlr    r9              \n"
                    "rotcl   r0              \n"
                    "shlr    r10             \n"
                    "mov.b   @%[addr],%[rx]  \n"  /* read old value */
                    "rotcl   r0              \n"
                    "and     %[mask],%[rx]   \n"  /* mask out replaced bits */
                    "or      %[rx],r0        \n"  /* set new bits */
                    "mov.b   r0,@%[addr]     \n"  /* store value to bitplane */
                    "add     %[psiz],%[addr] \n"  /* advance to next bitplane */
                    "cmp/hi  %[addr],%[end]  \n"  /* loop through all bitplanes */
                    "bt      .ur_floop       \n"

                    "bra     .ur_end         \n"
                    "nop                     \n"

                    /* References to C library routines used in the precalc block */
                    ".align  2               \n"
                ".ashlsi3:                   \n"  /* C library routine: */
                    ".long   ___ashlsi3      \n"  /* shift r4 left by r5, res. in r0 */
                ".lshrsi3:                   \n"  /* C library routine: */
                    ".long   ___lshrsi3      \n"  /* shift r4 right by r5, res. in r0 */
                    /* both routines preserve r4, destroy r5 and take ~16 cycles */

                ".ur_sloop:                  \n"  /** short loop (nothing to keep) **/
                    "shlr    r1              \n"  /* rotate lsb of pattern 1 to t bit */
                    "rotcl   r0              \n"  /* rotate t bit into r0 */
                    "shlr    r2              \n"
                    "rotcl   r0              \n"
                    "shlr    r3              \n"
                    "rotcl   r0              \n"
                    "shlr    r6              \n"
                    "rotcl   r0              \n"
                    "shlr    r7              \n"
                    "rotcl   r0              \n"
                    "shlr    r8              \n"
                    "rotcl   r0              \n"
                    "shlr    r9              \n"
                    "rotcl   r0              \n"
                    "shlr    r10             \n"
                    "rotcl   r0              \n"
                    "mov.b   r0,@%[addr]     \n"  /* store byte to bitplane */
                    "add     %[psiz],%[addr] \n"  /* advance to next bitplane */
                    "cmp/hi  %[addr],%[end]  \n"  /* loop through all bitplanes */
                    "bt      .ur_sloop       \n"

                ".ur_end:                    \n"
                    : /* outputs */
                    [addr]"+r"(addr),
                    [mask]"+r"(mask),
                    [rx]  "=&r"(trash)
                    : /* inputs */
                    [psiz]"r"(_gray_info.plane_size),
                    [end] "r"(end),
                    [patp]"[rx]"(pat_ptr)
                    : /* clobbers */
                    "r0", "r1", "r2", "r3", "r6", "r7", "r8", "r9", "r10"
                );
            }
#elif defined(CPU_COLDFIRE)
            asm volatile (
                "move.l  (%[cbuf]),%%d0      \n"
                "move.l  (%[bbuf]),%%d1      \n"
                "eor.l   %%d0,%%d1           \n"
                "move.l  (4,%[cbuf]),%%d0    \n"
                "move.l  (4,%[bbuf]),%[chg]  \n"
                "eor.l   %%d0,%[chg]         \n"
                "or.l    %%d1,%[chg]         \n"
                : /* outputs */
                [chg] "=&d"(change)
                : /* inputs */
                [cbuf]"a"(cbuf),
                [bbuf]"a"(bbuf)
                : /* clobbers */
                "d0", "d1"
            );

            if (change != 0)
            {
                unsigned char *addr, *end;
                unsigned mask, trash;

                pat_ptr = &pat_stack[8];

                /* precalculate the bit patterns with random shifts
                 * for all 8 pixels and put them on an extra "stack" */
                asm volatile (
                    "moveq.l #8,%%d3         \n"  /* loop count */
                    "clr.l   %[mask]         \n"

                ".ur_pre_loop:               \n"
                    "clr.l   %%d0            \n"
                    "move.b  (%[cbuf])+,%%d0 \n"  /* read current buffer */
                    "clr.l   %%d1            \n"
                    "move.b  (%[bbuf]),%%d1  \n"  /* read back buffer */
                    "move.b  %%d0,(%[bbuf])+ \n"  /* update back buffer */
                    "clr.l   %%d2            \n"  /* preset for skipped pixel */
                    "cmp.l   %%d0,%%d1       \n"  /* no change? */
                    "beq.b   .ur_skip        \n"  /* -> skip */

                    "move.l  (%%d0:l:4,%[bpat]),%%d2 \n"  /* d2 = bitpattern[byte]; */

                    "mulu.w  #75,%[rnd]      \n"  /* multiply by 75 */
                    "add.l   #74,%[rnd]      \n"  /* add another 74 */
                    /* Since the lower bits are not very random: */
                    "move.l  %[rnd],%%d1     \n"
                    "lsr.l   #8,%%d1         \n"  /* get bits 8..15 (need max. 5) */
                    "and.l   %[rmsk],%%d1    \n"  /* mask out unneeded bits */

                    "cmp.l   %[dpth],%%d1    \n"  /* random >= depth ? */
                    "blo.b   .ur_ntrim       \n"
                    "sub.l   %[dpth],%%d1    \n"  /* yes: random -= depth; */
                ".ur_ntrim:                  \n"

                    "move.l  %%d2,%%d0       \n"  /** rotate pattern **/
                    "lsl.l   %%d1,%%d0       \n"
                    "sub.l   %[dpth],%%d1    \n"
                    "neg.l   %%d1            \n"  /* d1 = depth - d1 */
                    "lsr.l   %%d1,%%d2       \n"
                    "or.l    %%d0,%%d2       \n"  /* rotated_pattern = d2 | d0 */

                    "or.l    #0x0100,%[mask] \n"  /* set mask bit */

                ".ur_skip:                   \n"
                    "lsr.l   #1,%[mask]      \n"  /* shift mask */
                    "move.l  %%d2,-(%[patp]) \n"  /* push on pattern stack */

                    "subq.l  #1,%%d3         \n"  /* loop 8 times (pixel block) */
                    "bne.b   .ur_pre_loop    \n"
                    : /* outputs */
                    [cbuf]"+a"(cbuf),
                    [bbuf]"+a"(bbuf),
                    [patp]"+a"(pat_ptr),
                    [rnd] "+d"(_gray_random_buffer),
                    [mask]"=&d"(mask)
                    : /* inputs */
                    [bpat]"a"(_gray_info.bitpattern),
                    [dpth]"d"(_gray_info.depth),
                    [rmsk]"d"(_gray_info.randmask)
                    : /* clobbers */
                    "d0", "d1", "d2", "d3"
                );

                addr = dst_row;
                end = addr + MULU16(_gray_info.depth, _gray_info.plane_size);

                /* set the bits for all 8 pixels in all bytes according to the
                 * precalculated patterns on the pattern stack */
                asm volatile (
                    "movem.l (%[patp]),%%d2-%%d6/%%a0-%%a1/%[ax] \n" 
                                                  /* pop all 8 patterns */
                    "not.l   %[mask]         \n"  /* "set" mask -> "keep" mask */
                    "and.l   #0xFF,%[mask]   \n"
                    "beq.b   .ur_sstart      \n"  /* short loop if nothing to keep */

                ".ur_floop:                  \n"  /** full loop (there are bits to keep)**/
                    "clr.l   %%d0            \n"
                    "lsr.l   #1,%%d2         \n"  /* shift out pattern bit */
                    "addx.l  %%d0,%%d0       \n"  /* put bit into LSB of byte */
                    "lsr.l   #1,%%d3         \n"
                    "addx.l  %%d0,%%d0       \n"
                    "lsr.l   #1,%%d4         \n"
                    "addx.l  %%d0,%%d0       \n"
                    "lsr.l   #1,%%d5         \n"
                    "addx.l  %%d0,%%d0       \n"
                    "lsr.l   #1,%%d6         \n"
                    "addx.l  %%d0,%%d0       \n"
                    "move.l  %%a0,%%d1       \n"
                    "lsr.l   #1,%%d1         \n"
                    "addx.l  %%d0,%%d0       \n"
                    "move.l  %%d1,%%a0       \n"
                    "move.l  %%a1,%%d1       \n"
                    "lsr.l   #1,%%d1         \n"
                    "addx.l  %%d0,%%d0       \n"
                    "move.l  %%d1,%%a1       \n"
                    "move.l  %[ax],%%d1      \n"
                    "lsr.l   #1,%%d1         \n"
                    "addx.l  %%d0,%%d0       \n"
                    "move.l  %%d1,%[ax]      \n"

                    "move.b  (%[addr]),%%d1  \n"  /* read old value */
                    "and.l   %[mask],%%d1    \n"  /* mask out replaced bits */
                    "or.l    %%d0,%%d1       \n"  /* set new bits */
                    "move.b  %%d1,(%[addr])  \n"  /* store value to bitplane */

                    "add.l   %[psiz],%[addr] \n"  /* advance to next bitplane */
                    "cmp.l   %[addr],%[end]  \n"  /* loop through all bitplanes */
                    "bhi.b   .ur_floop       \n"

                    "bra.b   .ur_end         \n"
                    
                ".ur_sstart:                 \n"
                    "move.l  %%a0,%[mask]    \n"  /* mask isn't needed here, reuse reg */

                ".ur_sloop:                  \n"  /** short loop (nothing to keep) **/
                    "clr.l   %%d0            \n"
                    "lsr.l   #1,%%d2         \n"  /* shift out pattern bit */
                    "addx.l  %%d0,%%d0       \n"  /* put bit into LSB of byte */
                    "lsr.l   #1,%%d3         \n"
                    "addx.l  %%d0,%%d0       \n"
                    "lsr.l   #1,%%d4         \n"
                    "addx.l  %%d0,%%d0       \n"
                    "lsr.l   #1,%%d5         \n"
                    "addx.l  %%d0,%%d0       \n"
                    "lsr.l   #1,%%d6         \n"
                    "addx.l  %%d0,%%d0       \n"
                    "lsr.l   #1,%[mask]      \n"
                    "addx.l  %%d0,%%d0       \n"
                    "move.l  %%a1,%%d1       \n"
                    "lsr.l   #1,%%d1         \n"
                    "addx.l  %%d0,%%d0       \n"
                    "move.l  %%d1,%%a1       \n"
                    "move.l  %[ax],%%d1      \n"
                    "lsr.l   #1,%%d1         \n"
                    "addx.l  %%d0,%%d0       \n"
                    "move.l  %%d1,%[ax]      \n"

                    "move.b  %%d0,(%[addr])  \n"  /* store byte to bitplane */
                    "add.l   %[psiz],%[addr] \n"  /* advance to next bitplane */
                    "cmp.l   %[addr],%[end]  \n"  /* loop through all bitplanes */
                    "bhi.b   .ur_sloop       \n"

                ".ur_end:                    \n"
                    : /* outputs */
                    [addr]"+a"(addr),
                    [mask]"+d"(mask),
                    [ax]  "=&a"(trash)
                    : /* inputs */
                    [psiz]"a"(_gray_info.plane_size),
                    [end] "a"(end),
                    [patp]"[ax]"(pat_ptr)
                    : /* clobbers */
                    "d0", "d1", "d2", "d3", "d4", "d5", "d6", "a0", "a1"
                );
            }
#else /* C version, for reference*/
#warning C version of gray_update_rect() used
            (void)pat_ptr;
            /* check whether anything changed in the 8-pixel block */
            change  = *(uint32_t *)cbuf ^ *(uint32_t *)bbuf;
            change |= *(uint32_t *)(cbuf + 4) ^ *(uint32_t *)(bbuf + 4);
            
            if (change != 0)
            {
                unsigned char *addr, *end;
                unsigned mask = 0;
                unsigned test = 1;
                int i;

                /* precalculate the bit patterns with random shifts
                 * for all 8 pixels and put them on an extra "stack" */
                for (i = 0; i < 8; i++)
                {
                    unsigned pat = 0;
                    unsigned char cur = *cbuf++;
                    unsigned char back = *bbuf;
                    
                    *bbuf++ = cur;

                    if (cur != back)
                    {
                        int shift;

                        pat = _gray_info.bitpattern[cur];

                        /* shift pattern pseudo-random, simple & fast PRNG */
                        _gray_random_buffer = 75 * _gray_random_buffer + 74;
                        shift = (_gray_random_buffer >> 8) & _gray_info.randmask;
                        if (shift >= _gray_info.depth)
                            shift -= _gray_info.depth;
                            
                        pat = (pat << shift) | (pat >> (_gray_info.depth - shift));
                        
                        mask |= 0x100;
                    }
                    mask >>= 1;
                    pat_stack[i] = pat;
                }

                addr = dst_row;
                end = addr + MULU16(_gray_info.depth, _gray_info.plane_size);

                /* set the bits for all 8 pixels in all bytes according to the
                 * precalculated patterns on the pattern stack */
                mask = (~mask & 0xff);
                if (mask == 0)
                {
                    do
                    {
                        unsigned data = 0;

                        for (i = 7; i >= 0; i--)
                            data = (data << 1) | ((pat_stack[i] & test) ? 1 : 0);
                        
                        *addr = data;
                        addr += _gray_info.plane_size;
                        test <<= 1;
                    }
                    while (addr < end);
                }
                else
                {
                    do
                    {
                        unsigned data = 0;

                        for (i = 7; i >= 0; i--)
                            data = (data << 1) | ((pat_stack[i] & test) ? 1 : 0);
                        
                        *addr = (*addr & mask) | data;
                        addr += _gray_info.plane_size;
                        test <<= 1;
                    }
                    while (addr < end);
                }

            }
#endif /* CONFIG_CPU */
            srcofs_row += _gray_info.height;
            dst_row++;
        }
        while (dst_row < dst_end);
        
        srcofs += 8;
        dst += _gray_info.width;
    }
}
#endif /* LCD_PIXELFORMAT */

#endif /* !SIMULATOR */

/* Update the whole greyscale overlay */
void gray_update(void)
{
    gray_update_rect(0, 0, _gray_info.width, _gray_info.height);
}

/* Do an lcd_update() to show changes done by rb->lcd_xxx() functions
   (in areas of the screen not covered by the greyscale overlay). */
void gray_deferred_lcd_update(void)
{
    if (_gray_info.flags & _GRAY_RUNNING)
    {
#ifdef SIMULATOR
        _deferred_update();
#else
        _gray_info.flags |= _GRAY_DEFERRED_UPDATE;
#endif
    }
    else
        _gray_rb->lcd_update();
}

/*** Screenshot ***/

#define BMP_FIXEDCOLORS (1 << LCD_DEPTH)
#define BMP_VARCOLORS   33
#define BMP_NUMCOLORS   (BMP_FIXEDCOLORS + BMP_VARCOLORS)
#define BMP_BPP         8
#define BMP_LINESIZE    ((LCD_WIDTH + 3) & ~3)
#define BMP_HEADERSIZE  (54 + 4 * BMP_NUMCOLORS)
#define BMP_DATASIZE    (BMP_LINESIZE * LCD_HEIGHT)
#define BMP_TOTALSIZE   (BMP_HEADERSIZE + BMP_DATASIZE)

#define LE16_CONST(x) (x)&0xff, ((x)>>8)&0xff
#define LE32_CONST(x) (x)&0xff, ((x)>>8)&0xff, ((x)>>16)&0xff, ((x)>>24)&0xff

static const unsigned char bmpheader[] =
{
    0x42, 0x4d,                 /* 'BM' */
    LE32_CONST(BMP_TOTALSIZE),  /* Total file size */
    0x00, 0x00, 0x00, 0x00,     /* Reserved */
    LE32_CONST(BMP_HEADERSIZE), /* Offset to start of pixel data */

    0x28, 0x00, 0x00, 0x00,     /* Size of (2nd) header */
    LE32_CONST(LCD_WIDTH),      /* Width in pixels */
    LE32_CONST(LCD_HEIGHT),     /* Height in pixels */
    0x01, 0x00,                 /* Number of planes (always 1) */
    LE16_CONST(BMP_BPP),        /* Bits per pixel 1/4/8/16/24 */
    0x00, 0x00, 0x00, 0x00,     /* Compression mode, 0 = none */
    LE32_CONST(BMP_DATASIZE),   /* Size of bitmap data */
    0xc4, 0x0e, 0x00, 0x00,     /* Horizontal resolution (pixels/meter) */
    0xc4, 0x0e, 0x00, 0x00,     /* Vertical resolution (pixels/meter) */
    LE32_CONST(BMP_NUMCOLORS),  /* Number of used colours */
    LE32_CONST(BMP_NUMCOLORS),  /* Number of important colours */

    /* Fixed colours */
#if LCD_DEPTH == 1
    0x90, 0xee, 0x90, 0x00,     /* Colour #0 */
    0x00, 0x00, 0x00, 0x00      /* Colour #1 */
#elif LCD_DEPTH == 2
    0xe6, 0xd8, 0xad, 0x00,     /* Colour #0 */
    0x99, 0x90, 0x73, 0x00,     /* Colour #1 */
    0x4c, 0x48, 0x39, 0x00,     /* Colour #2 */
    0x00, 0x00, 0x00, 0x00      /* Colour #3 */
#endif
};

#if LCD_DEPTH == 1
#define BMP_RED   0x90
#define BMP_GREEN 0xee
#define BMP_BLUE  0x90
#elif LCD_DEPTH == 2
#define BMP_RED   0xad
#define BMP_GREEN 0xd8
#define BMP_BLUE  0xe6
#endif

/* Hook function for core screen_dump() to save the current display
   content (b&w and greyscale overlay) to an 8-bit BMP file. */
static void gray_screendump_hook(int fd)
{
    int i;
    int x, y;
    int gx, gy;
#if (LCD_DEPTH == 1) || !defined(SIMULATOR)
    int mask;
#endif
#if LCD_PIXELFORMAT == HORIZONTAL_PACKING
    unsigned data;
#else
    int by;
#if LCD_DEPTH == 2
    int shift;
#endif
#endif
    unsigned char *clut_entry;
    unsigned char *lcdptr;
    unsigned char linebuf[MAX(4*BMP_VARCOLORS,BMP_LINESIZE)];

    _gray_rb->write(fd, bmpheader, sizeof(bmpheader));  /* write header */

    /* build clut */
    _gray_rb->memset(linebuf, 0, 4*BMP_VARCOLORS);
    clut_entry = linebuf;

    for (i = _gray_info.depth; i > 0; i--)
    {
        *clut_entry++ = MULU16(BMP_BLUE,  i) / _gray_info.depth;
        *clut_entry++ = MULU16(BMP_GREEN, i) / _gray_info.depth;
        *clut_entry++ = MULU16(BMP_RED,   i) / _gray_info.depth;
        clut_entry++;
    }
    _gray_rb->write(fd, linebuf, 4*BMP_VARCOLORS);

    /* BMP image goes bottom -> top */
    for (y = LCD_HEIGHT - 1; y >= 0; y--)
    {
        _gray_rb->memset(linebuf, 0, BMP_LINESIZE);

        gy = y - _gray_info.y;
#if LCD_PIXELFORMAT == HORIZONTAL_PACKING
#if LCD_DEPTH == 2
        lcdptr = _gray_rb->lcd_framebuffer + MULU16(LCD_FBWIDTH, y);

        if ((unsigned) gy < (unsigned) _gray_info.height)
        {
            /* line contains greyscale (and maybe b&w) graphics */
#ifndef SIMULATOR
            unsigned char *grayptr = _gray_info.plane_data
                                   + MULU16(_gray_info.bwidth, gy);
#endif

            for (x = 0; x < LCD_WIDTH; x += 4)
            {
                gx = x - _gray_info.x;

                if ((unsigned)gx < (unsigned)_gray_info.width)
                {
#ifdef SIMULATOR
                    data = MULU16(gy, _gray_info.width) + gx;

                    for (i = 0; i < 4; i++)
                        linebuf[x + i] = BMP_FIXEDCOLORS + _gray_info.depth
                                       - _gray_info.cur_buffer[data + i];
#else
                    mask = 0x80 >> (gx & 7);

                    for (i = 0; i < 4; i++)
                    {
                        int j;
                        int idx = BMP_FIXEDCOLORS;
                        unsigned char *grayptr2 = grayptr + (gx >> 3);

                        for (j = _gray_info.depth; j > 0; j--)
                        {
                            if (*grayptr2 & mask)
                                idx++;
                            grayptr2 += _gray_info.plane_size;
                        }
                        linebuf[x + i] = idx;
                        mask >>= 1;
                    }
#endif
                }
                else
                {
                    data = *lcdptr;
                    linebuf[x]     = (data >> 6) & 3;
                    linebuf[x + 1] = (data >> 4) & 3;
                    linebuf[x + 2] = (data >> 2) & 3;
                    linebuf[x + 3] = data & 3;
                }
                lcdptr++;
            }
        }
        else
        {
            /* line contains only b&w graphics */
            for (x = 0; x < LCD_WIDTH; x += 4)
            {
                data = *lcdptr++;
                linebuf[x] = (data >> 6) & 3;
                linebuf[x + 1] = (data >> 4) & 3;
                linebuf[x + 2] = (data >> 2) & 3;
                linebuf[x + 3] = data & 3;
            }
        }
#endif /* LCD_DEPTH */
#else /* LCD_PIXELFORMAT == VERTICAL_PACKING */
#if LCD_DEPTH == 1
        mask = 1 << (y & 7);
        by = y >> 3;
        lcdptr = _gray_rb->lcd_framebuffer + MULU16(LCD_WIDTH, by);

        if ((unsigned) gy < (unsigned) _gray_info.height)
        {
            /* line contains greyscale (and maybe b&w) graphics */
#ifndef SIMULATOR
            unsigned char *grayptr = _gray_info.plane_data 
                                   + MULU16(_gray_info.width, gy >> 3);
#endif

            for (x = 0; x < LCD_WIDTH; x++)
            {
                gx = x - _gray_info.x;
                
                if ((unsigned)gx < (unsigned)_gray_info.width)
                {
#ifdef SIMULATOR
                    linebuf[x] = BMP_FIXEDCOLORS + _gray_info.depth
                               - _gray_info.cur_buffer[MULU16(gx, _gray_info.height) + gy];
#else
                    int idx = BMP_FIXEDCOLORS;
                    unsigned char *grayptr2 = grayptr + gx;

                    for (i = _gray_info.depth; i > 0; i--)
                    {
                        if (*grayptr2 & mask)
                            idx++;
                        grayptr2 += _gray_info.plane_size;
                    }
                    linebuf[x] = idx;
#endif
                }
                else
                {
                    linebuf[x] = (*lcdptr & mask) ? 1 : 0;
                }
                lcdptr++;
            }
        }
        else
        {
            /* line contains only b&w graphics */
            for (x = 0; x < LCD_WIDTH; x++)
                linebuf[x] = (*lcdptr++ & mask) ? 1 : 0;
        }
#elif LCD_DEPTH == 2
        shift = 2 * (y & 3);
        by = y >> 2;
        lcdptr = _gray_rb->lcd_framebuffer + MULU16(LCD_WIDTH, by);

        if ((unsigned)gy < (unsigned)_gray_info.height)
        {
            /* line contains greyscale (and maybe b&w) graphics */
#ifndef SIMULATOR
            unsigned char *grayptr = _gray_info.plane_data
                                   + MULU16(_gray_info.width, gy >> 3);
            mask = 1 << (gy & 7);
#endif

            for (x = 0; x < LCD_WIDTH; x++)
            {
                gx = x - _gray_info.x;
                
                if ((unsigned)gx < (unsigned)_gray_info.width)
                {
#ifdef SIMULATOR
                    linebuf[x] = BMP_FIXEDCOLORS + _gray_info.depth
                               - _gray_info.cur_buffer[MULU16(gx, _gray_info.height) + gy];
#else
                    int idx = BMP_FIXEDCOLORS;
                    unsigned char *grayptr2 = grayptr + gx;

                    for (i = _gray_info.depth; i > 0; i--)
                    {
                        if (*grayptr2 & mask)
                            idx++;
                        grayptr2 += _gray_info.plane_size;
                    }
                    linebuf[x] = idx;
#endif
                }
                else
                {
                    linebuf[x] = (*lcdptr >> shift) & 3;
                }
                lcdptr++;
            }
        }
        else
        {
            /* line contains only b&w graphics */
            for (x = 0; x < LCD_WIDTH; x++)
                linebuf[x] = (*lcdptr++ >> shift) & 3;
        }
#endif /* LCD_DEPTH */
#endif /* LCD_PIXELFORMAT */

        _gray_rb->write(fd, linebuf, BMP_LINESIZE);
    }
}

#endif /* HAVE_LCD_BITMAP */

