/*  Sarien - A Sierra AGI resource interpreter engine
 *  Copyright (C) 1999-2001 Stuart George and Claudio Matsuoka
 *  
 *  $Id: sdl.c,v 1.32 2004/02/03 22:35:52 matt_hargett Exp $
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; see docs/COPYING for further details.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <java/lang.h>
#include <javax/microedition/lcdui/game.h>
#include <javax/microedition/io.h>
#include <s9.h>
#include <command_mgr.h>

#include "sarien.h"
#include "keyboard.h"
#include "graphics.h"
#include "s9_input.h"
#include "agi.h"


extern struct gfx_driver *gfx;
extern struct sarien_options opt;

static void cibyl_clear_fs_root(void *);

static NOPH_GameCanvas_t canvas;
static NOPH_Graphics_t graphics;

static  int cibyl_init_vidmode(void);
static  int cibyl_deinit_vidmode(void);
static void cibyl_put_block(int, int, int, int);
static void cibyl_put_block_x1_5(int, int, int, int);
static void cibyl_put_block_x2(int, int, int, int);
static void cibyl_put_pixels(int, int, int, pixel_t *);
static void cibyl_put_pixels_x1_5(int, int, int, pixel_t *);
static void cibyl_put_pixels_x2(int, int, int, pixel_t *);
static void cibyl_timer(void);
static  int cibyl_get_key(void);
static  int cibyl_keypress(void);

static void key_callback(void*);
static void bind_key_callback(void *k);
static void about_callback(void *unused);
static void toggle_display_size_callback(void *k);

static void keyReleased(int keyCode);
static void keyPressed(int keyCode);

static struct gfx_driver gfx_cibyl = {
  cibyl_init_vidmode,
  cibyl_deinit_vidmode,
  cibyl_put_block,
  cibyl_put_pixels,
  cibyl_timer,
  cibyl_keypress,
  cibyl_get_key
};



int init_machine (int argc, char **argv)
{
  gfx = &gfx_cibyl;
  return err_OK;
}


int deinit_machine ()
{
  return err_OK;
}


static int *screen;
static int rgb_palette[32];
static int screen_width, screen_height;
static int x_corr, y_corr;
static int input_mode_show_counter = 0;
static int input_mode = 0;
static char* input_mode_names[3] = { "s9", "num", "Fn" };
extern s9_t sarien_s9;
static NOPH_Font_t font;

typedef enum
{
  NORMAL,
  STRETCHED_VERTICALLY,
  DOUBLE_SIZE
} display_zoom;

static display_zoom cur_zoom;

void cibyl_set_screen_size (display_zoom zoom)
{
  switch (zoom)
  {
  case NORMAL:
    x_corr = (screen_width - GFX_WIDTH) / 2;
    y_corr = (screen_height - (GFX_HEIGHT + 20)) / 2;  // +20 => input row
    gfx_cibyl.put_block = cibyl_put_block;
    gfx_cibyl.put_pixels = cibyl_put_pixels;
    break;

  case STRETCHED_VERTICALLY:
    x_corr = (screen_width - (GFX_WIDTH + GFX_WIDTH / 2)) / 2;
    y_corr = (screen_height - (GFX_HEIGHT * 2 + 20)) / 2;
    gfx_cibyl.put_block = cibyl_put_block_x1_5;
    gfx_cibyl.put_pixels = cibyl_put_pixels_x1_5;
    break;

  case DOUBLE_SIZE:
    x_corr = (screen_width - GFX_WIDTH * 2) / 2;
    y_corr = (screen_height - (GFX_HEIGHT * 2 + 20)) / 2;
    gfx_cibyl.put_block = cibyl_put_block_x2;
    gfx_cibyl.put_pixels = cibyl_put_pixels_x2;
    break;

  default:
    /* Should never happen */
    break;
  }

  if (x_corr < 0)
    x_corr = 0;
  if (y_corr < 0)
    y_corr = 0;

  NOPH_Graphics_setColor_int(graphics, 0xff666666);
  NOPH_Graphics_fillRect(graphics, 0, 0, screen_width, screen_height);
  flush_screen();
  put_screen();
  cur_zoom = zoom;
}

static int cibyl_init_vidmode ()
{
  NOPH_CommandMgr_t cmd_mgr = NOPH_CommandMgr_getInstance();
  int i;

  screen = calloc(GFX_WIDTH*GFX_HEIGHT * 4, sizeof(int));
  NOPH_Canvas_registerKeyPressedCallback(keyPressed);
  NOPH_Canvas_registerKeyReleasedCallback(keyReleased);

  canvas = NOPH_GameCanvas_get();
  graphics = NOPH_GameCanvas_getGraphics(canvas);

  screen_width = NOPH_Canvas_getWidth(canvas);
  screen_height = NOPH_Canvas_getHeight(canvas);

  if (screen_width > 176 && screen_height > 210)
    cibyl_set_screen_size(STRETCHED_VERTICALLY);
  else if (screen_width >= GFX_WIDTH * 2)
    cibyl_set_screen_size(DOUBLE_SIZE);
  else
    cibyl_set_screen_size(NORMAL);

  NOPH_Graphics_setColor_int(graphics, 0xff666666);
  NOPH_Graphics_fillRect(graphics, 0, 0, screen_width, screen_height);
  NOPH_Graphics_setColor_int(graphics, 0);
  NOPH_GameCanvas_flushGraphics_rect(canvas, 0, 0, screen_width, screen_height);
  font = NOPH_Graphics_getFont(graphics);

  for ( i = 0; i < 16; i++ )
    {
      int r = palette[ i * 3 ];
      int g = palette[ i * 3 + 1];
      int b = palette[ i * 3 + 2];

      rgb_palette[i+16] = (0xff << 24) | (r << 16) | (g << 8) | b;
      rgb_palette[i] = (0xff << 24) | ((r*4) << 16) | ((g*4) << 8) | (b*4);
    }

  NOPH_CommandMgr_addCommand(cmd_mgr, NOPH_Command_ITEM, "Esc", key_callback, (void*)KEY_ESCAPE);
  NOPH_CommandMgr_addCommand(cmd_mgr, NOPH_Command_ITEM, "About", about_callback, NULL);
  NOPH_CommandMgr_addCommand(cmd_mgr, NOPH_Command_ITEM, "Sound on/off", bind_key_callback, (void*)3);
  NOPH_CommandMgr_addCommand(cmd_mgr, NOPH_Command_ITEM, "Clear FS root", cibyl_clear_fs_root, NULL);
  NOPH_CommandMgr_addCommand(cmd_mgr, NOPH_Command_ITEM, "Toggle display size", toggle_display_size_callback, NULL);
  NOPH_CommandMgr_addCommand(cmd_mgr, NOPH_Command_ITEM, "Switch s9/num/Fn", bind_key_callback, (void*)2);
  NOPH_CommandMgr_addCommand(cmd_mgr, NOPH_Command_ITEM, "Unbind 1", bind_key_callback, (void*)0);
  NOPH_CommandMgr_addCommand(cmd_mgr, NOPH_Command_ITEM, "Bind Alt-X -> 1", bind_key_callback, (void*)(scancode_table['X'-'A'] << 8));
  NOPH_CommandMgr_addCommand(cmd_mgr, NOPH_Command_ITEM, "Bind f1 -> 1", bind_key_callback, (void*)0x3b00);
  NOPH_CommandMgr_addCommand(cmd_mgr, NOPH_Command_ITEM, "Bind f2 -> 1", bind_key_callback, (void*)0x3c00);
  NOPH_CommandMgr_addCommand(cmd_mgr, NOPH_Command_ITEM, "Bind f3 -> 1", bind_key_callback, (void*)0x3d00);
  NOPH_CommandMgr_addCommand(cmd_mgr, NOPH_Command_ITEM, "Bind f4 -> 1", bind_key_callback, (void*)0x3e00);
  NOPH_CommandMgr_addCommand(cmd_mgr, NOPH_Command_ITEM, "Bind f5 -> 1", bind_key_callback, (void*)0x3f00);
  NOPH_CommandMgr_addCommand(cmd_mgr, NOPH_Command_ITEM, "Bind f6 -> 1", bind_key_callback, (void*)0x4000);
  NOPH_CommandMgr_addCommand(cmd_mgr, NOPH_Command_ITEM, "Bind f7 -> 1", bind_key_callback, (void*)0x4100);
  NOPH_CommandMgr_addCommand(cmd_mgr, NOPH_Command_ITEM, "Bind f8 -> 1", bind_key_callback, (void*)0x4200);
  NOPH_CommandMgr_addCommand(cmd_mgr, NOPH_Command_ITEM, "Bind f9 -> 1", bind_key_callback, (void*)0x4300);
  NOPH_CommandMgr_addCommand(cmd_mgr, NOPH_Command_ITEM, "Bind f10 -> 1", bind_key_callback, (void*)0x4400);
  NOPH_CommandMgr_addCommand(cmd_mgr, NOPH_Command_ITEM, "Bind 31 -> 1", bind_key_callback, (void*)1);

  s9_input_init(&sarien_s9);

  return 0;
}


static int cibyl_deinit_vidmode ()
{
  return 0;
}

/* blit a block onto the screen */
static void cibyl_put_block_x2 (int x1, int y1, int x2, int y2)
{
  unsigned int h, w;

  if (x1 >= GFX_WIDTH)  x1 = GFX_WIDTH - 1;
  if (y1 >= GFX_HEIGHT) y1 = GFX_HEIGHT - 1;
  if (x2 >= GFX_WIDTH)  x2 = GFX_WIDTH - 1;
  if (y2 >= GFX_HEIGHT) y2 = GFX_HEIGHT - 1;

  x1 *= 2;
  y1 *= 2;
  x2 *= 2;
  y2 *= 2;

  w = x2 - x1 + 2;
  h = y2 - y1 + 2;

  NOPH_Graphics_drawRGB(graphics, (int)screen, 0, screen_width, x_corr, y_corr, x1 + w, y1 + h, 0);
  NOPH_GameCanvas_flushGraphics_rect(canvas, x1 + x_corr, y1 + y_corr, w, h);
}


static void cibyl_put_block_x1_5 (int x1, int y1, int x2, int y2)
{
  unsigned int h, w;

  if (x1 >= GFX_WIDTH)  x1 = GFX_WIDTH - 1;
  if (y1 >= GFX_HEIGHT) y1 = GFX_HEIGHT - 1;
  if (x2 >= GFX_WIDTH)  x2 = GFX_WIDTH - 1;
  if (y2 >= GFX_HEIGHT) y2 = GFX_HEIGHT - 1;

  x1 = x1 + x1 / 2;
  y1 *= 2;
  x2 = x2 + x2 / 2;
  y2 *= 2;

  w = x2 - x1;
  h = y2 - y1;

  NOPH_Graphics_drawRGB(graphics, (int)screen, 0, screen_width, x_corr, y_corr, x1 + w, y1 + h, 0);
  NOPH_GameCanvas_flushGraphics_rect(canvas, x1 + x_corr, y1 + y_corr, w, h);
}

static void cibyl_put_block (int x1, int y1, int x2, int y2)
{
  unsigned int h, w;

  if (x1 >= GFX_WIDTH)  x1 = GFX_WIDTH  - 1;
  if (y1 >= GFX_HEIGHT) y1 = GFX_HEIGHT - 1;
  if (x2 >= GFX_WIDTH)  x2 = GFX_WIDTH  - 1;
  if (y2 >= GFX_HEIGHT) y2 = GFX_HEIGHT - 1;

  w = x2 - x1 + 1;
  h = y2 - y1 + 1;

  NOPH_Graphics_drawRGB(graphics, (int)screen, 0, GFX_WIDTH, x_corr, y_corr, x1 + w, y1 + h, 0);
  NOPH_GameCanvas_flushGraphics_rect(canvas, x1 + x_corr, y1 + y_corr, w, h);
}

static void cibyl_put_pixels_x2(int x, int y, int w, pixel_t *p_in)
{
  const int dst_pitch = screen_width;
  pixel_t *p = p_in;

  while ( w-- )
    {
      int cur = rgb_palette[*p];
      int dst_off = (y * 2) * dst_pitch + (x * 2);

      screen[dst_off] = cur;
      screen[dst_off + 1] = cur;
      screen[dst_off + dst_pitch] = cur;
      screen[dst_off + dst_pitch + 1] = cur;
      p++;
      x++;
      if (x * 2 >= screen_width)
        break;
    }
}

static void cibyl_put_pixels_x1_5(int x, int y, int w, pixel_t *p_in)
{
  const int dst_pitch = screen_width;
  pixel_t *p = p_in;

  int dst_off = (y * 2) * dst_pitch + x + x/2;

  while (w--)
    {
      int cur = rgb_palette[*p];

      screen[dst_off] = cur;
      screen[dst_off + dst_pitch] = cur;
      if (x & 1)
        {
          screen[dst_off + 1] = cur;
          screen[dst_off + dst_pitch + 1] = cur;
          dst_off++;
        }
      dst_off++;
      p++;
      x++;
      if (x + x / 2 >= screen_width)
        break;
    }
}

static void cibyl_put_pixels(int x, int y, int w, pixel_t *p)
{
  while (w--)
    {
      int cur = rgb_palette[*p];

      screen[x + (y * GFX_WIDTH)] = cur;
      p++;
      x++;
    }
}

static void cibyl_timer(void)
{
  static int64_t msec;
  static int cycle_time = 42;  // 42
  int64_t m, dt;
  int tx, ty, tw = 0;

  if (input_mode_show_counter > 0)
    {
      input_mode_show_counter--;
      tw = NOPH_Font_stringWidth(font, input_mode_names[input_mode]);
      tx = 160 + x_corr;
      ty = 160 + y_corr;

      NOPH_Graphics_setColor_int(graphics, 0xffffffff);
      NOPH_Graphics_fillRect(graphics, tx - 40, ty, 40, 20);
      NOPH_Graphics_setColor_int(graphics, 0xffdd2222);

      input_mode_show_counter--;
      if (input_mode_show_counter > 0)
        {
        NOPH_Graphics_drawString(graphics, input_mode_names[input_mode], tx - tw, ty, NOPH_Graphics_TOP | NOPH_Graphics_LEFT);
        }

      NOPH_GameCanvas_flushGraphics_rect(canvas, tx - 40, ty, 40, 20);
    }

  play_sound();

  m = NOPH_System_currentTimeMillis();
  dt = m - msec;
  while (dt < cycle_time)
    {
      NOPH_Thread_sleep(cycle_time - dt);
      m = NOPH_System_currentTimeMillis();
      dt = m - msec;
    }

  msec = m;
}

static int key_pressed = 0;
static int keycode = 0;

typedef enum
{
  NONE,
  INPUTTING,
  DONE
} input_t;

static input_t s9_input = NONE;
static int s9_ptr;
static int bound_to_1 = 0;
static int fkeys_table[] = {0x4400, 0x3B00, 0x3C00, 0x3D00, 0x3E00, 0x3F00, 0x4000, 0x4100, 0x4200, 0x4300}; // F10 first ( 0 key ) !!!

static int keycode_table_walk_special[] =
{
  0,
  KEY_UP,         /* -1 */
  KEY_DOWN,       /* -2 */
  KEY_LEFT,       /* -3 */
  KEY_RIGHT,      /* -4 */
  KEY_ENTER,      /* -5 */
  0,
  0,
  KEY_BACKSPACE,  /* -8 */
};

static void keyPressed(int code)
{
  printf("Key pressed:  %d\n", code);
  keycode = code;
  key_pressed = 1;
}

static void keyReleased(int keyCode)
{
  printf("Key released: %d\n", keyCode);
}

static void key_callback(void *k)
{
  int key = (int)k;

  printf("key callback: %x\n", key);
  keycode = key;
  key_pressed = 1;
}

static void toggle_display_size_callback(void *k)
{
  cur_zoom++;
  if (cur_zoom > DOUBLE_SIZE)
    cur_zoom = NORMAL;

  cibyl_set_screen_size(cur_zoom);
}

static void bind_key_callback(void *k)
{
  int key = (int)k;
  int b;

  bound_to_1 = key;

  if (key == 2)
    {
      bound_to_1 = 0;
      input_mode = s9_input_switch_input_mode();
      input_mode_show_counter = 60;               // counted in cibyl_timer ticks
    }

  if (key == 3)
    {
      b = getflag(F_sound_on);
      setflag (F_sound_on, !b);
      write_status();
    }
}

static void cibyl_clear_fs_root(void *k)
{
        NOPH_RecordStore_deleteRecordStore("sarien-conf");
}

static void about_callback(void *unused)
{
  NOPH_Display_t display = NOPH_Display_getDisplay(NOPH_MIDlet_get());
//  NOPH_Displayable_t cur = NOPH_Display_getCurrent(display);
  NOPH_Alert_t alert = NOPH_Alert_new("About",
                                      "Sarien is written by Stuart George, Claudio Matsuoka, Felipe Rosinha"
                                      "and others.\n\n"
                                      "J2ME support for Sarien was done by Simon K�gstr�m, tweaked by "
                                      "Zsolt Gurmai, see http://spel.bth.se/index.php/Sarien_for_Cibyl",
                                      0,
                                      NOPH_AlertType_INFO
                                      );

  NOPH_Alert_setTimeout(alert, NOPH_Alert_FOREVER);
  NOPH_Display_setCurrent(display, alert);
//  NOPH_delete(alert);
//  NOPH_Display_setCurrent(display, cur);
}


static int cibyl_keypress ()
{
  return key_pressed != 0;
}


static int cibyl_get_key (void)
{
  static char *cur_input_word = NULL;
  int code = keycode;
  int tx, ty = 0;

  keycode = 0;
  key_pressed = 0;

  tx = 0 + x_corr;
  ty = screen_height - 40;

  if (code < 0)
    {
      if (code == -5) /* Enter */
        s9_input = DONE;
      else if (code == -8) /* Backspace */
        code = '<';
      else if (code >= -8) /* Handle through the table */
        return keycode_table_walk_special[-code];
    }
  if (code == 10)
	  s9_input = DONE;


  if (input_mode == 0 || input_mode == 1)
    {
      if (code == '1')
        {
          if (bound_to_1 != 0) /* Special cases */
            {
            if (bound_to_1 == 1)
              {
                s9_input_reset();
                s9_input_set_translation("31");
                s9_input = DONE;
                cur_input_word = NULL;
              }
            else
              return bound_to_1;
            }
        }

      /* If we have s9 input, return this first */
      if (s9_input == DONE)
        {
          int cur;

          key_pressed = 1;
          if (cur_input_word == NULL)
            {
              cur_input_word = strdup(s9_input_get_translation());
              s9_ptr = 0;
            }
          else if (s9_ptr >= strlen(cur_input_word))
            {
              free(cur_input_word);
              cur_input_word = NULL;
              s9_input = NONE;
              s9_input_reset();
              s9_ptr = 0;
              put_screen();

              return KEY_ENTER;
            }

          cur = cur_input_word[s9_ptr++];

          if (cur == '\0')
            {
              free(cur_input_word);
              cur_input_word = NULL;

              NOPH_Graphics_setColor_int(graphics, 0xffffffff);
              NOPH_Graphics_fillRect(graphics, tx, ty, 160, 20);
              /* Finished! */
              s9_input = NONE;
              s9_ptr = 0;
              s9_input_reset();

              return KEY_ENTER;
            }

          return cur;
        }

      if ( (code >= '0' && code <= '9') || code == '*' || code == '#' || code == '<' )
        {
          s9_input_handle_keypress(code);
          NOPH_Graphics_setColor_int(graphics, 0xffffffff);
          NOPH_Graphics_fillRect(graphics, tx, ty, 160, 20);
          NOPH_Graphics_setColor_int(graphics, 0);
          s9_input_draw(graphics, tx, ty);
          NOPH_GameCanvas_flushGraphics_rect(canvas, tx, ty, 160, 20);

          return 0;
        }
    }

  if (input_mode == 2)
    {
      if (code >= '0' && code <= '9')
        {
        code = fkeys_table[ ( code - 48 ) ];
        }

      if (code == -5) return KEY_ENTER;
    }

  if (code == 10)
    return KEY_ENTER;

  /* Fall-through for f-keys etc */
  return code; /* s9 input */
}
