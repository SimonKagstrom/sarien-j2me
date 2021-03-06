/*  Sarien - A Sierra AGI resource interpreter engine
 *  Copyright (C) 1999-2001 Stuart George and Claudio Matsuoka
 *
 *  $Id: cocoa.m,v 1.4 2002/11/09 11:32:10 cmatsuoka Exp $
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; see docs/COPYING for further details.
 *
 *  MacOS X port by Richard Houle <richard.houle@sandtechnology.com>
 */

#import <Cocoa/Cocoa.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/time.h>

#include "sarien.h"
#include "graphics.h"
#include "cocoa.h"

unsigned int sarienPalette[32];
SarienView* sarienView;

struct gfx_driver gfx_cocoa = {
	cocoa_init,
	cocoa_deinit,
	cocoa_put_block,
	cocoa_put_pixels,
	cocoa_timer,
	cocoa_keypress,
	cocoa_get_key
};


int init_machine (int argc, char **argv)
{
	gfx = &gfx_cocoa;
	return err_OK;
}


int deinit_machine (void)
{
	return err_OK;
}


int cocoa_init(void)
{
	int			i, j;
	unsigned char* c = (unsigned char*)sarienPalette;

	for (i = 0, j = 0; i < 32; i++) {
		*c++ = 255;
		*c++ = palette[j++] << 2;
		*c++ = palette[j++] << 2;
		*c++ = palette[j++] << 2;
	}

	return err_OK;
}


int cocoa_deinit(void)
{
	return err_OK;
}


void cocoa_put_block(int x, int y, int w, int h)
{
/*	NSRect rect;
	
	rect.origin.x = x * opt.scale;
	rect.origin.y = y * opt.scale;
	rect.size.width = w * opt.scale;
	rect.size.height = h * opt.scale;
*/	
//	[sarienView lock];
	[sarienView display];
//	[sarienView unlock];
}


void cocoa_put_pixels(int x, int y, int w, UINT8 *p)
{
	unsigned int* pixels;

	[sarienView lock];
	pixels = &[sarienView screenContent][x + (y * GFX_WIDTH)];
	
	while (w--) {
		*pixels++ = sarienPalette[*p++];
	}
	[sarienView unlock];
}


int cocoa_keypress(void)
{
	return [sarienView hasPendingKey];
}


int cocoa_get_key(void)
{
	return [sarienView getKey];
}


void cocoa_timer(void)
{
	static double msec = 0.0;
	struct timeval tv;
	struct timezone tz;
	double m;
	
	gettimeofday(&tv, &tz);
	m = 1000.0 * tv.tv_sec + tv.tv_usec / 1000.0;

	while (m - msec < 42) {
		usleep(5000);
		gettimeofday(&tv, &tz);
		m = 1000.0 * tv.tv_sec + tv.tv_usec / 1000.0;
	}

	msec = m; 
}

