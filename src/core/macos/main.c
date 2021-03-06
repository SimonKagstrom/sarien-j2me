/*  Sarien - A Sierra AGI resource interpreter engine
 *  Copyright (C) 1999-2002 Stuart George and Claudio Matsuoka
 *
 *  $Id: main.c,v 1.5 2002/11/10 13:36:26 cmatsuoka Exp $
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; see docs/COPYING for further details.
 */

#include <Memory.h>
#include <stdio.h>
#include <string.h>
#include "sarien.h"
#include "agi.h"
#include "text.h"
#include "graphics.h"
#include "sprite.h"


extern int optind;

struct sarien_options opt;
struct game_id_list game_info;
struct agi_game game;

#if !defined(_TRACE) && !defined(__GNUC__)
INLINE void _D (char *s, ...) { s = s; }
#endif


int main (int argc, char *argv[])
{
	int ec;

	MaxApplZone ();

	game.clock_enabled = FALSE;
	game.state = STATE_INIT;

	init_machine (argc, argv);

	game.color_fg = 15;
	game.color_bg = 0;

	if ((game.sbuf = calloc (_WIDTH, _HEIGHT)) == NULL) {
		ec = err_NotEnoughMemory;
		goto bail_out;
	}
#ifdef USE_HIRES
	if ((game.hires = calloc (_WIDTH * 2, _HEIGHT)) == NULL) {
		ec = err_NotEnoughMemory;
		goto bail_out_2;
	};
	opt.hires = 1;
#endif

	if (init_sprites () != err_OK) {
		ec = err_NotEnoughMemory;
		goto bail_out_3;
	}

	if (init_video () != err_OK) {
		ec = err_Unk;
		goto bail_out_4;
	}

	console_init ();
	report ("--- Starting console ---\n\n");

	_D ("Init sound");
	init_sound ();

	game.ver = -1;		/* Disable conf file warning */

	report (" \nSarien " VERSION " is ready.\n");
	if (game.state < STATE_LOADED) {
       		console_prompt ();
		do { main_cycle (); } while (game.state < STATE_RUNNING);
		if (game.ver < 0)
			game.ver = 0;	/* Enable conf file warning */
	}

	ec = run_game ();

	deinit_sound ();
	deinit_video ();

bail_out_4:
	deinit_sprites ();
bail_out_3:
#ifdef USE_HIRES
	free (game.hires);
#endif
bail_out_2:
	free (game.sbuf);
bail_out:
	if (ec == err_OK || ec == err_DoNothing) {
		deinit_machine ();
		exit (ec);
	}

	deinit_machine ();

	return ec;
}

