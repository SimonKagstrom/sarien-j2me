/*********************************************************************
 *
 * Copyright (C) 2007,  Blekinge Institute of Technology
 *
 * Filename:      s9_input.h
 * Author:        Simon Kagstrom <ska@bth.se>
 * Description:
 *
 * $Id:$
 *
 ********************************************************************/
#ifndef __S9_INPUT_H__
#define __S9_INPUT_H__

#include <s9.h>
#include <javax/microedition/lcdui.h>

void s9_input_handle_keypress(int code);
void s9_input_draw(NOPH_Graphics_t graphics, int x, int y);
void s9_input_reset(void);
void s9_input_init(s9_t *s9);
void s9_input_erase_one(void);
char *s9_input_get_translation(void);
void s9_input_set_translation(char *what);
void s9_input_set_input_mode(int inmode);
int s9_input_switch_input_mode(void);
void s9_input_input_mode_changed(void);

#endif /* !__S9_INPUT_H__ */
