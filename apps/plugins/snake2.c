/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * $Id$
 *
 * Copyright (C) 2003 Mat Holton
 *
 * All files in this archive are subject to the GNU General Public License.
 * See the file COPYING in the source tree root for full license agreement.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 ****************************************************************************/

/*
Snake2!

Board consists of a WIDTHxHEIGHT grid. If board element is 0 then nothing is 
there otherwise it is part of the snake or a wall.

Head and Tail are stored

*/

#include "plugin.h"
#ifdef HAVE_LCD_BITMAP

#define WIDTH  28
#define HEIGHT 16

/*Board itself - 2D int array*/
static int board[WIDTH][HEIGHT];
/*
  Buffer for sorting movement (in case user presses two movements during a 
  single frame
*/
static int ardirectionbuffer[2];
static unsigned int score, hiscore = 0;
static short applex;
static short appley;
static short dir;
static short frames;
static short apple;
static short level = 4, speed = 5,dead = 0, quit = 0, sillydir = 0, num_levels=0; 
static short level_from_file = 1;
static struct plugin_api* rb;
static int headx, heady, tailx, taily, applecountdown = 5;
static int game_type = 0;
static int num_apples_to_get=1;
static int num_apples_to_got=0;
static int game_b_level=1;

#define NORTH       1   
#define EAST        2   
#define SOUTH       4   
#define WEST        8   
#define HEAD        16  

#define EAST_NORTH  32
#define EAST_SOUTH  64
#define WEST_NORTH  128
#define WEST_SOUTH  256

#define NORTH_EAST  512
#define NORTH_WEST  1024
#define SOUTH_EAST  2048
#define SOUTH_WEST  4096

#define LEVELS_FILE         "/.rockbox/snake2.levels"

static void set_level_count(void) 
{
    int line_count=0;
    int fd = 0;
    char buffer[WIDTH+2]; /* WIDTH plus CR/LF and \0 */
    
    if ((fd = rb->open(LEVELS_FILE, O_RDONLY)) < 0) 
    {
        rb->splash(0, true, "Unable to open %s", LEVELS_FILE);
    }
    
    if(!(fd < 0))
    {
        while(1)
        {
            int len = rb->read_line(fd, buffer, sizeof(buffer));
            if(len <= 0)
                break;
            else
                line_count++;

            if(line_count==HEIGHT)
            {
                num_levels++;
                line_count=0;
            }
        }

        rb->close(fd);
    }

}

/*
** Completely clear the board of walls and/or snake
*/
void clear_board( void)
{
    int x,y;

    for (x = 0; x < WIDTH; x++) 
    {
        for (y = 0; y < HEIGHT; y++) 
        {
            board[x][y] = 0;
        }
    }
}

int load_level( int level_number )
{
    int fd = 0;
    int x,y, len;
    unsigned char buffer[WIDTH+2];

    /* open file */
    if ((fd = rb->open(LEVELS_FILE, O_RDONLY)) < 0)
    {
        return -1;
    }
       
    rb->lseek(fd, level_number*(WIDTH+2)*(HEIGHT+1), SEEK_SET);
    
    clear_board();

    for(y=0; y < HEIGHT; y++)
    {
        len = rb->read_line(fd, buffer, WIDTH+2);
        if(len <= 0)
        {
            rb->close(fd);
            return -1;
        }
        else
        {
             /*Read a line in, now add to the array:*/
             for(x=0;x < WIDTH;x++)
             {
                 switch(buffer[x])
                 {
                     case '1':
                         board[x][y] = NORTH;
                     break;

                     case '2':
                         board[x][y] = EAST;
                     break;

                     case 'H':
                         board[x][y] = HEAD;
                         break;
                 }
             }
        }
    }
    rb->close(fd);
    return 1;
}

/*
** Gets the currently chosen direction from the first place
** in the direction buffer. If there is something in the
** next part of the buffer then that is moved to the first place
*/
void get_direction( void )
{
    /*if 1st place is empty*/
    if(ardirectionbuffer[0] != -1)
    {
        /*return this direction*/
        dir = ardirectionbuffer[0];
        ardirectionbuffer[0]=-1;
        /*now see if one needs moving:*/
        if(ardirectionbuffer[1] != -1)
        {
            /*there's a move waiting to be done
              so move it into the space:*/
            ardirectionbuffer[0] = ardirectionbuffer[1];
            ardirectionbuffer[1] = -1;
        }
    }
}

/*
** Sets the direction 
*/
void set_direction(int newdir)
{
    if(ardirectionbuffer[0] != newdir)
    {
        /*if 1st place is empty*/
        if(ardirectionbuffer[0] == -1)
        {
            /*use 1st space:*/
            ardirectionbuffer[0] = newdir;
        }
        else
        {
            /*use 2nd space:*/
            if(ardirectionbuffer[0] != newdir) ardirectionbuffer[1] = newdir;
        }

        if(frames < 0) ardirectionbuffer[0] = newdir;
    }
}

void init_snake( void )
{
    ardirectionbuffer[0] = -1;
    ardirectionbuffer[1] = -1;
    dir   = EAST;
    headx = WIDTH/2;
    heady = HEIGHT/2;
    tailx = headx - 4;
    taily = heady;
    applecountdown = 0;
    /*Create a small snake to start off with*/      
    board[headx][heady]   = dir;
    board[headx-1][heady] = dir;
    board[headx-2][heady] = dir;
    board[headx-3][heady] = dir;
    board[headx-4][heady] = dir;
    num_apples_to_got=0;
    num_apples_to_get=1;
    level_from_file = 1;
    game_b_level=1;
}

/*
** Draws the apple. If it doesn't exist then
** a new one get's created.
*/
void draw_apple( void )
{
    short x,y;
    if (!apple)
    {
          do
          {
            x = (rb->rand() % (WIDTH-1))+1;
            y = (rb->rand() % (HEIGHT-1))+1;
          } while (board[x][y]);
          apple=1;
          board[x][y]=-1;
          applex = x;appley = y;
    }
    rb->lcd_fillrect((applex*4)+1,appley*4,2,4);
    rb->lcd_fillrect(applex*4,(appley*4)+1,4,2);
}

/*
    * x x *
    * x x *
    * x x *
    * x x *
*/
void draw_vertical_bit(int x, int y)
{
    rb->lcd_fillrect(x*4+1,y*4,2,4);
}

/*
    * * * *
    X X X X
    X X X X
    * * * *
*/
void draw_horizontal_bit(int x, int y)
{
    rb->lcd_fillrect(x*4,y*4+1,4,2);
}

/*
    * * * *
    * * X X
    * X X X
    * X X *
*/
void draw_n_to_e_bit(int x, int y)
{
    rb->lcd_fillrect(x*4+1,y*4+2,2,2);
    rb->lcd_fillrect(x*4+2,y*4+1,2,2);
}

/*
   * * * *
   * * X X
   * X X X
   * X X *
*/
void draw_w_to_s_bit(int x, int y)
{
    draw_n_to_e_bit(x,y);
}

/*
   * * * *
   X X * *
   X X X *
   * X X *
*/
void draw_n_to_w_bit(int x, int y)
{
    rb->lcd_fillrect(x*4,y*4+1,2,2);
    rb->lcd_fillrect(x*4+1,y*4+2,2,2);
}

/*
   * * * *
   X X * *
   X X X *
   * X X *
*/
void draw_e_to_s_bit(int x, int y)
{
    draw_n_to_w_bit(x, y);
}

/*
   * X X *
   * X X X
   * * X X
   * * * *
*/
void draw_s_to_e_bit(int x, int y)
{
    rb->lcd_fillrect(x*4+1,y*4,2,2);
    rb->lcd_fillrect(x*4+2,y*4+1,2,2);
}

/*
   * X X *
   * X X X
   * * X X
   * * * *
*/
void draw_w_to_n_bit(int x, int y)
{
    draw_s_to_e_bit(x,y);
}

/*
   * X X *
   X X X *
   X X * *
   * * * *
*/
void draw_e_to_n_bit(int x, int y)
{
    rb->lcd_fillrect(x*4+1,y*4,2,2);
    rb->lcd_fillrect(x*4,y*4+1,2,2);
}

/*
   * X X *
   X X X *
   X X * *
   * * * *
*/
void draw_s_to_w_bit(int x, int y)
{
    draw_e_to_n_bit(x, y);
}

/*
** Draws a wall/obsticals
*/
void draw_boundary ( void )
{
    int x, y;
    
    /*TODO: Load levels from file!*/

    /*top and bottom line*/
    for(x=0; x < WIDTH; x++)
    {
        board[x][0] = EAST;
        board[x][HEIGHT-1] = WEST;
    }
    
    /*left and right lines*/
    for(y=0; y < HEIGHT; y++)
    {
        board[0][y] = NORTH;
        board[WIDTH-1][y] = SOUTH;
    }

    /*corners:*/
    board[0][0]              = NORTH_EAST;
    board[WIDTH-1][0]        = EAST_SOUTH;
    board[0][HEIGHT-1]       = SOUTH_EAST;
    board[WIDTH-1][HEIGHT-1] = EAST_NORTH;
}

/*
** Redraw the entire board
*/
void redraw (void)
{
    short x,y;
    rb->lcd_clear_display();
   
    for (x = 0; x < WIDTH; x++) 
    {
        for (y = 0; y < HEIGHT; y++) 
        {
            switch (board[x][y]) 
            {
                case -1:
                    rb->lcd_fillrect((x*4)+1,y*4,2,4);
                    rb->lcd_fillrect(x*4,(y*4)+1,4,2);
                    break;
                case 0:
                    break;
                
                case NORTH:
                case SOUTH:
                    draw_vertical_bit(x,y);
                    break;
                
                case EAST:
                case WEST:
                    draw_horizontal_bit(x,y);
                    break;
                    
                default:
                    rb->lcd_fillrect(x*4,y*4,4,4);
                    break;
            }
        }
    }
    rb->lcd_update();
}

/*
** Draws the snake bit described by nCurrentBit at position x/y
** deciding whether it's a corner bit by examing the nPrevious bit
*/
void draw_snake_bit(int currentbit, int previousbit, int x, int y)
{
    rb->lcd_clearrect(x*4,y*4,4,4);
    switch(currentbit)
    {
        case(NORTH):
            switch(previousbit)
            {
                case(SOUTH):
                case(NORTH):
                    draw_vertical_bit(x,y);
                break;
                
                case(EAST):
                    draw_e_to_n_bit(x,y);
                break;

                case(WEST):
                    draw_w_to_n_bit(x,y);
                break;
            }
        break;

        case(EAST):
            switch(previousbit)
            {
                case(WEST):
                case(EAST):
                    draw_horizontal_bit(x,y);
                break;  
                
                case(NORTH):
                    draw_n_to_e_bit(x,y);
                break;

                case(SOUTH):
                    draw_s_to_e_bit(x,y);
                break;
            }
        break;

        case(SOUTH):
            switch(previousbit)
            {
                case(SOUTH):
                case(NORTH):
                    draw_vertical_bit(x,y);
                break;
            
                case(EAST):
                    draw_e_to_s_bit(x,y);
                break;

                case(WEST):
                    draw_w_to_s_bit(x,y);
                break;
            }
        break;

        case(WEST): 
            switch(previousbit)
            {
                case(EAST):
                case(WEST):             
                    draw_horizontal_bit(x,y);
                break;  

                case(SOUTH):
                    draw_s_to_w_bit(x,y);
                break;

                case(NORTH):
                    draw_n_to_w_bit(x,y);
                break;
            }
        break;
    }
}

/*
** Death 'sequence' and end game stuff.
*/
void die (void)
{
    int n=100;
    int count;
    char pscore[15],hscore[17];

    /*Flashy death sequence (flashy as in 'flashes')*/
    for(count=0;count<24;count++)
    {
        rb->sleep(HZ/n);
        rb->lcd_clear_display();
        draw_apple();
        rb->lcd_update();

        rb->sleep(HZ/n);
        redraw();
        rb->lcd_update();
    }
    
    rb->lcd_clear_display();
    draw_apple();
    rb->lcd_update();

    rb->snprintf(pscore,sizeof(pscore),"%d",score);
    rb->lcd_putsxy(LCD_WIDTH/2 - 15,12,"Dead");
    rb->lcd_putsxy(LCD_WIDTH/2 - 35,22,"Your score :");
    rb->lcd_putsxy(LCD_WIDTH/2 - 35,32, pscore);
    if (score>hiscore)
    {
        hiscore=score;
        rb->lcd_putsxy(3,42,"New High Score!");
    }
    else 
    {
        rb->snprintf(hscore,sizeof(hscore),"High Score: %d",hiscore);
        rb->lcd_putsxy(5,42,hscore);
    }
    rb->lcd_update();
    rb->sleep(3*HZ);
    dead=1;
}

/*
** Check for collision. TODO: Currently this
** sets of the death sequence. What we want is it to only return a true/false
** depending on whether a collision occured.
*/
void collision ( int x, int y )
{
    int bdeath=0;

    switch (board[x][y]) 
    {
        case 0:
        
        break; 
        case -1:
            score+=2;
            apple=0;
            applecountdown=2;
            
            if(game_type==1)
            {
                if(num_apples_to_get == num_apples_to_got)
                {
                    level_from_file++;
                    if(level_from_file >= num_levels)
                    {
                        level_from_file = 1;
                        /*and increase the number of apples to pick up
                        before level changes*/
                        num_apples_to_get+=2;
                        game_b_level++;
                    }
                    rb->splash(0, true, "Level Completed!");
                    rb->sleep(HZ);
                    rb->lcd_clear_display();
                    num_apples_to_got=0;
                    load_level(level_from_file);
                    init_snake();
                    redraw();
                }
                else
                    num_apples_to_got++;
            }
        break;
        default:
            bdeath=1;
            break;
    }

    if(bdeath==1)
    {
        die();
        sillydir = dir;
        frames = -110;
    }
}

void move( void )
{
    int taildir;
    /*this actually sets the dir variable.*/
    get_direction();
    /*draw head*/
    switch (dir) 
    {
        case (NORTH):
            board[headx][heady]=NORTH;
            heady--;
            break;
        case (EAST):
            board[headx][heady]=EAST;
            headx++; 
            break;
        case (SOUTH):
            board[headx][heady]=SOUTH;
            heady++;
            break;
        case (WEST):
            board[headx][heady]=WEST;
            headx--;
            break;
    }
    
    if(headx == WIDTH)
        headx = 0;
    else if(headx < 0)
        headx = WIDTH-1;

    if(heady == HEIGHT)
        heady = 0;
    else if(heady < 0)
        heady = HEIGHT-1;
    
    rb->lcd_fillrect(headx*4,heady*4,4,4);
    
    /*clear tail*/
    if(applecountdown <= 0) 
    {
        rb->lcd_clearrect(tailx*4,taily*4,4,4);
        
        taildir = board[tailx][taily];
        board[tailx][taily] = 0;

        switch (taildir)
        {
            case(NORTH):
                taily--;
            break;

            case(EAST):
                tailx++;
            break;

            case(SOUTH):
                taily++;
            break;

            case(WEST):
                tailx--;
            break;
        }
        
        if(tailx == WIDTH)
            tailx = 0;
        else if(tailx < 0)
            tailx = WIDTH-1;

        if(taily == HEIGHT)
            taily = 0;
        else if(taily < 0)
            taily = HEIGHT-1;
    }
    else
        applecountdown--;
}

void frame (void)
{
    int olddir, noldx, noldy, temp;
    noldx  = headx;
    noldy  = heady;
    olddir = 0;  
    switch(dir)
    {
        case(NORTH):
            if(heady == HEIGHT-1)
                temp = 0;
            else
                temp = heady + 1;
            
            olddir = board[headx][temp];
        break;

        case(EAST):
            if(headx == 0)
                temp = WIDTH-1;
            else
                temp = headx - 1;

            olddir = board[temp][heady];
        break;

        case(SOUTH):
            if(heady == 0)
                temp = HEIGHT-1;
            else
                temp = heady - 1;

            olddir = board[headx][temp];
        break;

        case(WEST):
            if(headx == WIDTH-1)
                temp = 0;
            else
                temp = headx + 1;

            olddir = board[temp][heady];
        break;
    }

    move();
    
    /*
      now redraw the bit that was
      the tail, to something snake-like:
     */
    draw_snake_bit(dir, olddir, noldx, noldy);   
    
    collision(headx, heady);
    
    rb->lcd_update();
}

void game_pause (void) 
{
    rb->lcd_clear_display();
    rb->lcd_putsxy(33,12,"Paused");
    
    rb->lcd_update();
    while (1) 
    {
        switch (rb->button_get(true)) 
        {
            case BUTTON_PLAY:
                redraw();
                rb->sleep(HZ/2);
                return;
        }
    }
}

void game (void) 
{
    redraw();
    /*main loop:*/
    while (1) 
    {
        if(frames==5) 
        {
           frame();
           if(frames>0) frames=0;
        }
        frames++;
        
        if(frames == 0)
        {
            die();
        }
        else
        {
            if(frames < 0)
            {
                if(sillydir != dir)
                {
                    /*it has, great set frames to a positive value again:*/
                    frames = 1;
                }
            }
        }
        
        if (dead) return;
                
        draw_apple();
       
        rb->sleep(HZ/speed);

        switch (rb->button_get(false)) 
        {
             case BUTTON_UP:
             case BUTTON_UP | BUTTON_REPEAT:             
                 if (dir != SOUTH) set_direction(NORTH);
                 break;

             case BUTTON_RIGHT:
             case BUTTON_RIGHT | BUTTON_REPEAT:
                 if (dir != WEST) set_direction(EAST); 
                 break;

             case BUTTON_DOWN:
             case BUTTON_DOWN | BUTTON_REPEAT:
                 if (dir != NORTH) set_direction(SOUTH);
                 break;

             case BUTTON_LEFT:
             case BUTTON_LEFT | BUTTON_REPEAT:
                 if (dir != EAST) set_direction(WEST);
                 break;

             case BUTTON_OFF:
                 dead=1;
                 return;

             case BUTTON_PLAY:
                 game_pause();
                 break;
        }      
    }
}

void game_init(void) 
{
    char plevel[30];
    char phscore[30];

    dead=0;
    apple=0;
    score=0;
    
    while (1) 
    {    
        switch (rb->button_get(true)) 
        {
            case BUTTON_RIGHT:
            case BUTTON_UP:         
                if (level<10) 
                    level+=1;
                break;
            case BUTTON_LEFT:
            case BUTTON_DOWN:           
                if (level>1)
                    level-=1;
                break;
            case BUTTON_OFF:
                quit=1;
                return;
                break;
            case BUTTON_PLAY:
                speed = level*20;
                return;
                break;
            case BUTTON_F3:
                if(game_type==0)game_type=1; else game_type=0;
                break;
            case BUTTON_F1:
                
                level_from_file++;
                if(level_from_file > num_levels)
                {
                    level_from_file = 1;
                }
                
                load_level( level_from_file );
                
                break;
        }
       
        rb->lcd_clear_display();
        redraw();
        /*TODO: CENTER ALL TEXT!!!!*/
        rb->snprintf(plevel,sizeof(plevel),"Speed - %d  ",level);
        rb->lcd_putsxy(LCD_WIDTH/2 - 30,5, plevel);
        rb->snprintf(plevel,sizeof(plevel),"F1 - Maze %d  ",level_from_file);
        rb->lcd_putsxy(18, 20, plevel);
        if(game_type==0)
            rb->lcd_putsxy(18, 30, "F3 - Game A");
        else
            rb->lcd_putsxy(18, 30, "F3 - Game B");
        
        rb->snprintf(phscore,sizeof(phscore),"Hi Score: %d",hiscore);
        rb->lcd_putsxy(LCD_WIDTH/2 - 37,50, phscore);
        rb->lcd_update();
    }
}

enum plugin_status plugin_start(struct plugin_api* api, void* parameter)
{
    TEST_PLUGIN_API(api);
    (void)(parameter);
    rb = api;
    set_level_count();

    if (num_levels == 0) {
        rb->splash(HZ*2, true, "Failed loading levels!");
        return PLUGIN_OK;
    }

    /* Lets use the default font */
    rb->lcd_setfont(FONT_SYSFIXED);
    /*load the 1st level in*/
    load_level( level_from_file );
                
    while(quit==0)
    {
      game_init(); 
      rb->lcd_clear_display();
      frames=1;
      
      if(quit==0)
      {
        load_level( level_from_file );
        init_snake();
        /*Start Game:*/
        game();
        /* Game over, reload level*/
        init_snake();
        load_level( level_from_file );
      }
    }
    
    return false;
}

#endif
