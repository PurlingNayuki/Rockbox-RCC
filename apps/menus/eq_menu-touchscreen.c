/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * $Id: eq_menu.c 12179 2007-02-01 23:08:15Z amiconn $
 *
 * Copyright (C) 2006 Dan Everton, 2009 Carsten Schreiter
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 ****************************************************************************/

#include "config.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "eq_menu.h"
#include "system.h"
#include "kernel.h"
#include "lcd.h"
#include "menu.h"
#include "action.h"
#include "mp3_playback.h"
#include "settings.h"
#include "screens.h"
#include "icons.h"
#include "font.h"
#include "lang.h"
/*sprintf.h is no longer needed in new version*/
/*#include "sprintf.h"*/
#include "talk.h"
#include "misc.h"
#include "sound.h"
#include "dsp.h"
#include "tree.h"
#include "screen_access.h"
#include "keyboard.h"
#include "gui/scrollbar.h"
#include "menu_common.h"
#include "viewport.h"
#include "stdlib.h"
#include "gui/touchscreen.h"
#include "exported_menus.h"

/*
 * Utility functions
 */

/* cutoff limits min and max for each eq band              */
/* required for better setting resolution on a touchscreen */
const int cutoff_lim[2][5] = {
    {    20,    20,   500,  1000,  5000 },
    {   500,  2000,  5000, 15000, 22040 },
};

/* variables used to store buttonbar, band and slider position */

struct button_bar{
    struct button ok;
    struct button cancel;
    struct button edit_mode;
};

struct eq_screen_data{
    struct eq_slider_data{
        short x;
        short y;
        short height;
        short length;
    }slider;
    struct eq_txt_data{
        short x;
        short y[5];
        short height;
        short width;
    }band;
};


enum eq_slider_mode {
    GAIN,
    CUTOFF,
    Q,
};

static void save_eq_settings(int *eq_settings)
{
    int i;
    int *setting = &global_settings.eq_band0_cutoff;
    
    for (i = 0; i < 15; i ++)
    {
        eq_settings[i] = setting[i];
    }
    return;
}

static void restore_eq_settings(int *eq_settings)
{
    int i;
    int *setting = &global_settings.eq_band0_cutoff;
    
    for (i = 0; i < 15; i++)
    {
        setting[i] =  eq_settings[i];
    }
    for (i = 0; i < 5; i++)
    {
        dsp_set_eq_coefs(i);
    } 
    return;
}

/* left and right border the eq screen */
#define EQ_OUTER_MARGIN 5
/* to let reach the max setting shortly before slider end for better control */
#define SLIDER_TOUCH_MARGIN 5
#define BUTTON_BAR_MARGIN_BOTTOM 7
#define SLIDER_WIDTH 20

static void draw_slider_bar(struct screen * screen, int x, int y,
                        int width, int height, int value,
                        int min, int max)
{
    int inner_x, inner_y, inner_wd, inner_ht;
    int start, size;
    short edge_reached = 0;
#ifdef HAVE_LCD_COLOR
    int infill;
#endif

#define LEFT_EDGE  1
#define RIGHT_EDGE 2

    inner_x  = x + 1;
    inner_y  = y + 1;
    inner_wd = width  - 2;
    inner_ht = height - 2;

    size = SLIDER_WIDTH;
    start = inner_x + ((value - min) * inner_wd / (max - min)) - SLIDER_WIDTH / 2;
    
    if (start <= inner_x)
    {
        size = start + SLIDER_WIDTH - inner_x;
        start = inner_x;
        edge_reached = LEFT_EDGE;
    }
    
    if (start > inner_x + inner_wd - SLIDER_WIDTH)
    {
        size = inner_x + inner_wd - start;
        edge_reached = RIGHT_EDGE;
    }

    /* draw box */
#ifdef HAVE_LCD_COLOR
    /* must avoid corners if case of (flags & FOREGROUND) */
    screen->hline(inner_x, x + inner_wd, y);
    screen->hline(inner_x, x + inner_wd, y + height - 1);
    screen->vline(x, inner_y, y + inner_ht);
    screen->vline(x + width - 1, inner_y, y + inner_ht);
#else
    screen->drawrect(x, y, width, height);
#endif

    screen->set_drawmode(DRMODE_SOLID | DRMODE_INVERSEVID);

#ifdef HAVE_LCD_COLOR
    infill = (screen->depth > 1 ? INNER_FILL_MASK : INNER_FILL);

#endif
    /* clear corner pixels */
    screen->drawpixel(x, y);
    screen->drawpixel(x + width - 1, y);
    screen->drawpixel(x, y + height - 1);
    screen->drawpixel(x + width - 1, y + height - 1);

#ifdef HAVE_LCD_COLOR
    if (infill != INNER_BGFILL)
        infill = INNER_FILL;

    if (infill == INNER_FILL)
#endif
    {
        /* clear pixels in progress bar */
        screen->fillrect(inner_x, inner_y, inner_wd, inner_ht);
    }

    screen->set_drawmode(DRMODE_SOLID);

#ifdef HAVE_LCD_COLOR
    if (infill == INNER_BGFILL)
    {
        /* fill inner area with current background color */
        unsigned fg = screen->get_foreground();
        screen->set_foreground(screen->get_background());
        screen->fillrect(inner_x, inner_y, inner_wd, inner_ht);
        screen->set_foreground(fg);
    }
#endif
    /* draw slider */
    screen->fillrect(start, inner_y, size, inner_ht);

    /* clear corner pixels of slider */
    screen->set_drawmode(DRMODE_SOLID | DRMODE_INVERSEVID);
    if (edge_reached != LEFT_EDGE)
    {
        screen->drawpixel(start, inner_y);
        screen->drawpixel(start, inner_y + inner_ht - 1);
    }
    if (edge_reached != RIGHT_EDGE)
    {
        screen->drawpixel(start + size - 1, inner_y);
        screen->drawpixel(start + size - 1, inner_y + inner_ht - 1);
    }
    screen->set_drawmode(DRMODE_SOLID);
}

/* Draw's all the EQ sliders. */
static void draw_eq_sliders(struct screen * screen,
                            struct viewport *vp,
                            struct eq_screen_data *eq_data,
                            int current_band,
                            enum eq_slider_mode mode)
{
    int band, w, h, gain, q, cutoff;
    int *setting = &global_settings.eq_band0_cutoff;
    char buf[25];
    int value, min_item, max_item;
    int x1, x2, y1;

    screen->set_viewport(vp);

    for (band = 0; band < 5; band ++) {
        cutoff = *setting++;
        q = *setting++;
        gain = *setting++;
    
        switch(mode) {
        case Q:
            value = q;
            min_item = EQ_Q_MIN;
            max_item = EQ_Q_MAX;
            break;        
        case CUTOFF:
            value = cutoff;
            min_item = EQ_CUTOFF_MIN(band);
            max_item = EQ_CUTOFF_MAX(band);
            break;         
        case GAIN:
        default:
            value = gain;
            min_item = EQ_GAIN_MIN;
            max_item = EQ_GAIN_MAX;
            break;
        }
    
        /* Start two pixels in, one for border, one for margin */
        x1 = (int)eq_data->band.x + 2;
        y1 = (int)eq_data->band.y[band] + 2;
    
        /* Print out the band label */
        if (band == 0) {
            screen->putsxy(x1, y1, "LS: ");
            screen->getstringsize("LS:", &w, &h);
        } else if (band == 4) {
            screen->putsxy(x1, y1, "HS: ");
            screen->getstringsize("HS:", &w, &h);
        } else {
            snprintf(buf, sizeof(buf),  "PK%d:", band);
            screen->putsxy(x1, y1, buf);
            screen->getstringsize(buf, &w, &h);
        }
    
        screen->getstringsize("A", &w, &h);
        x1 += 5*w; /* 4 chars for label + 1 space = 5 */
    
        /* Print out gain part of status line (left justify after label) */
        if (mode == GAIN && band == current_band)
            screen->set_drawmode(DRMODE_SOLID | DRMODE_INVERSEVID);
        else
            screen->set_drawmode(DRMODE_SOLID);
    
        snprintf(buf, sizeof(buf), "%s%2d.%d%s", gain < 0 ? "-" : " ",
            abs(gain) / EQ_USER_DIVISOR, abs(gain) % EQ_USER_DIVISOR,"dB");
        screen->putsxy(x1, y1, buf);
        screen->getstringsize(buf, &w, &h);
    
        x1 += w;
    
        /* Print out Q part of status line (right justify) */
        if (mode == Q && band == current_band)
            screen->set_drawmode(DRMODE_SOLID | DRMODE_INVERSEVID);
        else
            screen->set_drawmode(DRMODE_SOLID);
    
        snprintf(buf, sizeof(buf), "%d.%d%s", q / EQ_USER_DIVISOR,
                 q % EQ_USER_DIVISOR, " Q");
        screen->getstringsize(buf, &w, &h);
        x2 = eq_data->band.x +  eq_data->band.width - w - 2;
        screen->putsxy(x2, y1, buf);
    
        /* Print out cutoff part of status line (center between gain & Q) */
        if (mode == CUTOFF && band == current_band)
            screen->set_drawmode(DRMODE_SOLID | DRMODE_INVERSEVID);
        else
            screen->set_drawmode(DRMODE_SOLID);
        
        snprintf(buf, sizeof(buf),  "%5d%s", cutoff, "Hz");
        screen->getstringsize(buf, &w, &h);
        x1 = x1 + (x2 - x1 - w)/2;
        screen->putsxy(x1, y1, buf);
    
        screen->set_drawmode(DRMODE_SOLID);
        
        /* Draw horizontal slider */
        draw_slider_bar(screen, eq_data->band.x, eq_data->band.y[band] +
                        eq_data->slider.y, eq_data->band.width,
                        eq_data->slider.height, value, min_item, max_item);
    }
    return;
}

/* draws the button bar at the top of the screen */
static void draw_button_bar(struct screen *screen,
                            struct viewport *vp,
                            enum eq_slider_mode mode,
                            struct button_bar *btn_bar)
{
    char buf [25];
    screen->set_viewport(vp);
    screen->set_drawmode(DRMODE_SOLID);

    /* Edit mode button */
    if (mode == CUTOFF) {
        /* cutoff */
        snprintf(buf, sizeof(buf), str(LANG_SYSFONT_EQUALIZER_MODE),
                         str(LANG_SYSFONT_EQUALIZER_BAND_CUTOFF), "(Hz)");
    }
    else if (mode == GAIN) {
        /* gain */
        snprintf(buf, sizeof(buf), str(LANG_SYSFONT_EQUALIZER_MODE),
                 str(LANG_SYSFONT_GAIN), "(dB)");
    }
    else {
            /* Q */
        snprintf(buf, sizeof(buf), str(LANG_SYSFONT_EQUALIZER_MODE),
                 str(LANG_EQUALIZER_BAND_Q), "");
    }
    btn_bar->edit_mode.txt = buf;
    draw_button(screen, &(btn_bar->edit_mode));
    
    /* Cancel button */              
    draw_button(screen, &(btn_bar->cancel));
    
    /* Ok button */              
    draw_button(screen, &(btn_bar->ok));  
            
    return;
}

#ifdef HAVE_REMOTE_LCD
void draw_remote_screen(struct screen * screen,
                        struct viewport *vp,
                        int current_band,
                        enum eq_slider_mode mode,
                        int setting)
{
    int x, h, y;
    char buf[10];

    screen->set_viewport(vp);
   
    screen->getstringsize("PK  ", &x, &h);
    y = (vp->height - h) / 2;
   
    if (current_band == 0) {
        screen->putsxy(2, y, "LS ");
    } else if (current_band == 4) {
        screen->putsxy(2, y, "HS ");
    } else {
        snprintf(buf, sizeof(buf),  "PK%d", current_band);
        screen->putsxy(2, y, buf);
    }
    
    switch(mode) {
        case Q:
            snprintf (buf, sizeof(buf), "-Q: %d.%d", setting / EQ_USER_DIVISOR,
                 setting % EQ_USER_DIVISOR);
            break;        
        case CUTOFF:
            snprintf (buf, sizeof(buf), "-C: %5d", setting);
            break;         
        case GAIN:
        default:
            snprintf (buf, sizeof(buf), "-G: %s%2d.%d", setting < 0 ? "-" : " ",
                abs(setting) / EQ_USER_DIVISOR, abs(setting) % EQ_USER_DIVISOR);
            break;
        }
    screen->putsxy(x, y, buf);
}
#endif

/* calculates buttonbar, band and slider postionions and sizes */
static bool init_eq_screen(struct screen * screen,
                           struct viewport *vp,
                           struct button_bar *btn_bar,
                           struct eq_screen_data  *eq_data)
{
    int w, w1, h, i;
    char buf [26];

    btn_bar->ok.txt = str(LANG_GRAPHIC_EQ_OK);
    btn_bar->cancel.txt = str(LANG_GRAPHIC_EQ_CANCEL);
    
    btn_bar->ok.vp = btn_bar->cancel.vp = btn_bar->edit_mode.vp = vp;
    btn_bar->ok.clicked = btn_bar->cancel.clicked =
                                    btn_bar->edit_mode.clicked = false;  
    btn_bar->ok.y = btn_bar->cancel.y = btn_bar->edit_mode.y= 0;

    /* Edit mode button */
    /* don't know which setting is the longest in each language, therefore */
    /* check all 3 settings*/
    snprintf(buf, sizeof(buf), str(LANG_SYSFONT_EQUALIZER_MODE),
                         str(LANG_SYSFONT_EQUALIZER_BAND_CUTOFF), "(Hz)");
    screen->getstringsize(buf, &w1, &h);
    w = w1;

    snprintf(buf, sizeof(buf), str(LANG_SYSFONT_EQUALIZER_MODE),
                                        str(LANG_SYSFONT_GAIN), "(dB)");
    screen->getstringsize(buf, &w1, &h);
    if (w1 > w) {
        w = w1;
    }
    
    snprintf(buf, sizeof(buf), str(LANG_SYSFONT_EQUALIZER_MODE),
                                        str(LANG_EQUALIZER_BAND_Q), "");
    screen->getstringsize(buf, &w1, &h);
    if (w1 > w) {
        w = w1;
    }
    btn_bar->edit_mode.width = w + 10;
    btn_bar->edit_mode.x = vp->width - btn_bar->edit_mode.width;

    /* Cancel button */              
    screen->getstringsize(btn_bar->cancel.txt, &w, &h);
    btn_bar->cancel.width = w + 10;
    btn_bar->cancel.x = btn_bar->cancel.width + 5;
    
    /* Ok button */              
    screen->getstringsize(btn_bar->ok.txt, &w, &h);
    btn_bar->ok.x = 0;
    btn_bar->ok.width = btn_bar->cancel.width;

    screen->getstringsize("A", &w, &h);
    if (w * 20 > vp->width ||
        (btn_bar->cancel.width + btn_bar->ok.width +
         btn_bar->edit_mode.width + 10 ) > vp->width)
    {
        return false; /* buttonbar or 3 settings do not fit on the screen */
    }
    
    /* determine the height of the button bar */
    if (h <= 16) {
        btn_bar->ok.height = btn_bar->cancel.height =
                                    btn_bar->edit_mode.height = 20;
    }
    else {
        btn_bar->ok.height = btn_bar->cancel.height =
                                    btn_bar->edit_mode.height = h + 4;
    }
    
    /* calculate band postion and size */ 
    eq_data->band.x = 0;
    eq_data->band.width = vp->width;
    eq_data->band.height = (vp->height - 1 - btn_bar->ok.height -
                BUTTON_BAR_MARGIN_BOTTOM) / 5;
    for (i=0; i < 5; i++)
    {
        eq_data->band.y[i] = btn_bar->ok.height + BUTTON_BAR_MARGIN_BOTTOM + i *
                                                eq_data->band.height;
    } 
    
    /* calculate slider postion within a band and size */ 
    eq_data->slider.x = SLIDER_TOUCH_MARGIN;
    eq_data->slider.length = vp->width - 2 * SLIDER_TOUCH_MARGIN;
    eq_data->slider.y = h + 3 + 1;
    eq_data->slider.height = eq_data->band.height - eq_data->slider.y;

    if (eq_data->slider.height > 30)
    {
        eq_data->slider.height = 30;
    }
    else if (eq_data->slider.height < 15)
    {
        return false; /* sliders are too small */
    }   
    return true;
}


/* Provides a graphical means of editing the EQ settings */
bool eq_menu_graphical_touchscreen(void)
{
    bool exit_request = false;
    bool result = true;
    bool has_changed = false;
    int button;
    int *setting;
    int current_band = 0;
    int step, min, max, voice_unit;
    enum eq_slider_mode mode = GAIN;
    int eq_settings[15];
    struct button_bar btn_bar;
    struct eq_screen_data eq_data;
    struct viewport vp [NB_SCREENS];

    touchscreen_set_mode(TOUCHSCREEN_POINT);
    save_eq_settings(eq_settings);

    FOR_NB_SCREENS(i) {
        screens[i].set_viewport(NULL);
        screens[i].stop_scroll();
        viewport_set_defaults(&vp[i], i);
        screens[i].set_viewport(&vp[i]);
        screens[i].clear_viewport();
        screens[i].update_viewport();
        if (screens[i].screen_type == SCREEN_MAIN) {
            vp[i].font = FONT_UI;
            vp[i].x = EQ_OUTER_MARGIN;
            vp[i].width -= 2 * EQ_OUTER_MARGIN;
            vp[i].y += EQ_OUTER_MARGIN;
            vp[i].height -= EQ_OUTER_MARGIN; 
            vp[i].font = FONT_UI;
            /* Select the font. Use the theme font if possible, use Sysfont if the */
            /* buttonbar or EQ do not fit on the screen. Additionally buttonbar    */
            /* band and slider coordinates are calculated once.                    */
            if (!init_eq_screen(&screens[i], &vp[i], &btn_bar, &eq_data))
            {
                vp[i].font = FONT_SYSFIXED;
                init_eq_screen(&screens[i], &vp[i], &btn_bar, &eq_data);
            }
        }
        else {
            vp[i].font = FONT_SYSFIXED;
        }
    }

    while (!exit_request) {

        FOR_NB_SCREENS(i) {
            screens[i].clear_viewport();
        }

        /* Set pointer to the band data currently editable */
        if (mode == GAIN) {
            /* gain */
            setting = &global_settings.eq_band0_gain;
            setting += current_band * 3;

            step = EQ_GAIN_STEP;
            min = EQ_GAIN_MIN;
            max = EQ_GAIN_MAX;
            voice_unit = UNIT_DB;                

        } else if (mode == CUTOFF) {
            /* cutoff */
            setting = &global_settings.eq_band0_cutoff;
            setting += current_band * 3;

            step = EQ_CUTOFF_STEP;
            min = EQ_CUTOFF_MIN(current_band);
            max = EQ_CUTOFF_MAX(current_band);
            voice_unit = UNIT_HERTZ;

        } else {
            /* Q */
            setting = &global_settings.eq_band0_q;
            setting += current_band * 3;

            step = EQ_Q_STEP;
            min = EQ_Q_MIN;
            max = EQ_Q_MAX;
            voice_unit = UNIT_INT;
        }

        /* Draw button bar and equalizer bands */
        draw_button_bar(&screens[SCREEN_MAIN], &vp[SCREEN_MAIN], mode, &btn_bar);
        draw_eq_sliders(&screens[SCREEN_MAIN], &vp[SCREEN_MAIN],
                                                &eq_data, current_band, mode);
#ifdef HAVE_REMOTE_LCD 
        draw_remote_screen(&screens[SCREEN_REMOTE], &vp[SCREEN_REMOTE],
                                                 current_band, mode, *setting);
#endif
        FOR_NB_SCREENS(i) {
            screens[i].set_viewport(&vp[i]);
            screens[i].update_viewport();
        }

        /* wait here until a button or the touchscreen is pressed. no need to */
        /* to redraw if there is no action                                    */
        do
        {
            button = get_action(CONTEXT_SETTINGS_EQ,TIMEOUT_BLOCK);
        }
        while (button == BUTTON_NONE);
        
#define EQ_SLIDER_TCONST 2

        static bool slider_change = false;

        if ((button == ACTION_TOUCHSCREEN &&
            touchscreen_get_mode() == TOUCHSCREEN_POINT) ||
            slider_change)
        {
            bool slider_touched = false;
            short selected_band = current_band;
            int button_touchscreen;
            short touch_x = 0, touch_y = 0;
            int target_value = 0;

            button_touchscreen = action_get_touchscreen_press(&touch_x, &touch_y);
            
            if (check_button_click(&btn_bar.ok, touch_x, touch_y) &&
                    !slider_change)
            {
                btn_bar.ok.clicked = true;                
                if (button_touchscreen & BUTTON_REL) {
                    button = ACTION_STD_CANCEL;
                    slider_change = false;
                }
            }
            else  if (check_button_click(&btn_bar.cancel, touch_x, touch_y) &&
                        !slider_change)
            {
                btn_bar.cancel.clicked = true;
                if (button_touchscreen & BUTTON_REL ) {
                    restore_eq_settings(eq_settings);
                    button = ACTION_STD_CANCEL;
                    slider_change = false;
                }
            }
            else if (check_button_click(&btn_bar.edit_mode, touch_x, touch_y) &&
                    !slider_change)
            {
                btn_bar.edit_mode.clicked = true;                
                if (button_touchscreen & BUTTON_REL) {
                    button = ACTION_STD_OK;
                    btn_bar.edit_mode.clicked = false;
                    /* change Edit Mode */
                }
            }
            else if (viewport_point_within_vp(&vp[SCREEN_MAIN],
                                     (int)touch_x, (int)touch_y))
            {
                button = BUTTON_NONE;
                touch_x -= vp[SCREEN_MAIN].x;
                touch_y -= vp[SCREEN_MAIN].y;
                button = BUTTON_NONE;
                selected_band = 0;
                if (touch_x >= eq_data.band.x &&
                    touch_y >= eq_data.band.y[0] + eq_data.slider.y)
                {
                    do {  /* find out which item and band has been touched */
                        if ( touch_y >= eq_data.band.y[selected_band] +
                                                    eq_data.slider.y  &&
                            (touch_y <= (eq_data.band.y[selected_band] +
                                eq_data.slider.y + eq_data.slider.height)))
                        {
                            slider_touched = true;
                        }
                        else
                        {
                            selected_band++;
                        }
                    } while (selected_band < 5 && !slider_touched);
                }
            }
            else
            {
                button = BUTTON_NONE;
            }

            if (slider_touched || slider_change)
            {
                if (!(slider_change))
                {                    
                    if (current_band != selected_band)
                    {
                        current_band = selected_band;
                    }
                    else if (slider_touched) {
                        slider_change = true;
                    }
                }

                if (slider_change)
                {
                    switch (mode) {
                        case GAIN:
                            target_value = max * 2 * (touch_x - eq_data.slider.x -
                                eq_data.slider.length/2) / eq_data.slider.length;
                            break;
                        case CUTOFF:
                            switch (current_band) {
                                case 1:
                                    step = step * 2;
                                    break;
                                case 2:
                                case 3:
                                case 4:
                                    step = step * 5;
                                    break;
                                case 5:
                                    step = step * 10;
                                    break;
                            }
                            target_value = min + ((max-min) * (touch_x -
                                    eq_data.slider.x) / eq_data.slider.length);
                            break;
                        case Q:
                            target_value = min + ((max-min) * (touch_x -
                                eq_data.slider.x) / eq_data.slider.length);
                    }
                                        
                    target_value = (target_value / step) * step;
                    
                    if (mode != Q && abs(target_value) % step >= step / 2)
                    {
                        if (target_value >= 0)
                        {
                            target_value += step;
                        }
                        else
                        {
                            target_value -= step;
                        }
                    }
                    
                    if (target_value < min) {
                        target_value = min;
                    }
                    else if (target_value > max) {
                        target_value = max;
                    }
    
                    /* make the slider movement a little smoother */
                    *(setting) = (*(setting) + (target_value - *(setting)) /
                                EQ_SLIDER_TCONST + (target_value - *(setting)) %
                                EQ_SLIDER_TCONST);
                    has_changed = true;
                }
            }

            if (slider_change && (button_touchscreen & BUTTON_REL)) {
                slider_change = false;
                *(setting) = target_value;
            }
        }

        switch (button) {
        case ACTION_SETTINGS_DEC:
        case ACTION_SETTINGS_DECREPEAT:
            *(setting) -= step;
            has_changed = true;
            if (*(setting) < min)
                *(setting) = min;
            break;

        case ACTION_SETTINGS_INC:
        case ACTION_SETTINGS_INCREPEAT:
            *(setting) += step;
            has_changed = true;
            if (*(setting) > max)
                *(setting) = max;
            break;

        case ACTION_STD_PREV:
        case ACTION_STD_PREVREPEAT:
            current_band--;
            if (current_band < 0)
                current_band = 4; /* wrap around */
            break;

        case ACTION_STD_NEXT:
        case ACTION_STD_NEXTREPEAT:
            current_band++;
            if (current_band > 4)
                current_band = 0; /* wrap around */
            break;

        case ACTION_STD_OK:
            mode++;
            if (mode > Q)
                mode = GAIN; /* wrap around */
            break;

        case ACTION_STD_CANCEL:
            exit_request = true;
            result = false;
            touchscreen_set_mode(global_settings.touch_mode);
            break;
        default:
            if(default_event_handler(button) == SYS_USB_CONNECTED) {
                exit_request = true;
                result = true;
            }
            break;
        }
        
        /* Update the filter if the user changed something */
        if (has_changed) {
            dsp_set_eq_coefs(current_band);
            has_changed = false;
        }
    }

    /* Reset screen settings */
    FOR_NB_SCREENS(i) {
        screens[i].setfont(FONT_UI);
        screens[i].clear_display();
        screens[i].set_viewport(NULL);
    }
    return result;
}
