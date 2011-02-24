/*  Sarien - A Sierra AGI resource interpreter engine
 *  Copyright (C) 1999-2003 Stuart George and Claudio Matsuoka
 *
 *  $Id: main.c,v 1.87 2003/09/02 01:10:55 cmatsuoka Exp $
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; see docs/COPYING for further details.
 */

#include <stdio.h>
#include <string.h>
#include <javax/microedition/io.h>
#include <s9.h>
#include "../../graphics/cibyl/s9_input.h"
#include "command_mgr.h"

#include "sarien.h"
#include "agi.h"
#include "text.h"
#include "graphics.h"
#include "sprite.h"

struct sarien_options opt;
struct game_id_list game_info;
struct agi_game game;
s9_t sarien_s9;

static NOPH_List_t game_list;
static char *selected_game = NULL;
static char **all_games = NULL;
extern int cibyl_default_game_selected;

#define DEFAULT_CIBAR_ROOT "/"
#define DEFAULT_CIBAR_FILENAME "Time_Quest___Chad_Goulding_(built-in).cibar"

/* Is this ugly? Yes. Horribly! */
static void select_game_callback(void *unused)
{
        int nr = NOPH_List_getSelectedIndex(game_list);

	if (nr == 0)
	  {
	    selected_game = DEFAULT_CIBAR_FILENAME;
	    return;
	  }

        printf("SG: %d:%s\n", nr, all_games[nr]);
        selected_game = all_games[nr - 1];
}

static void select_game(char *base_dir)
{
        NOPH_Display_t display = NOPH_Display_getDisplay(NOPH_MIDlet_get());
        NOPH_Displayable_t cur = NOPH_Display_getCurrent(display);
        NOPH_CommandMgr_t cm = NOPH_CommandMgr_getInstance();
        char **p;

        all_games = cibyl_read_cibar_directory(get_current_directory ());
        if (!all_games)
                return;

        game_list = NOPH_List_new("Select game", NOPH_Choice_IMPLICIT);

	/* Add the default one first */
	NOPH_List_append(game_list, DEFAULT_CIBAR_FILENAME, 0);
        for (p = all_games; *p; p++)
        {
                NOPH_List_append(game_list, *p, 0);
        }
        NOPH_Display_setCurrent(display, game_list);

        NOPH_CommandMgr_setList(cm, game_list, select_game_callback, NULL);

        while(selected_game == NULL)
        {
                NOPH_Thread_sleep(250);
        }
        free(all_games);

        NOPH_Display_setCurrent(display, cur);
        NOPH_delete(game_list);
}


extern void cibyl_select_fs_root(void *unused);
extern char *get_filesystem_root ();
extern void set_filesystem_root (char *root);
#undef fopen
#undef fread
#undef fwrite
#undef fclose

int main (int argc, char *argv[])
{
        NOPH_Display_t display = NOPH_Display_getDisplay(NOPH_MIDlet_get());
        NOPH_Displayable_t cur = NOPH_Display_getCurrent(display);
	int ec;
        FILE *conf;

        /* Create and setup the fs root conf if not present */
        if ( !(conf = fopen("recordstore://sarien-conf:1", "r")) )
        {
                cibyl_select_fs_root(NULL);
        }
        else
        {
                        char buf[80];
                        fread(buf, 1, 80, conf);
                        printf("Read conf, root at %s\n", buf);
                        set_filesystem_root(buf);
                        fclose(conf);
        }

        select_game(get_current_directory());
        if (selected_game == NULL)
          {
            NOPH_Alert_t msg;

            msg = NOPH_Alert_new("Cannot find games",
				 "Cannot find any games. The games should be in e:/sarien/. The example"
				 "'Time quest' by Chad Goulding will be started",
                                 0, NOPH_AlertType_get(NOPH_AlertType_ERROR));
            NOPH_Alert_setTimeout(msg, NOPH_Alert_FOREVER);
            NOPH_Display_setCurrent(display, msg);
            NOPH_Thread_sleep(2000);
            NOPH_RecordStore_deleteRecordStore("sarien-conf");

	    selected_game = DEFAULT_CIBAR_FILENAME;
          }
	if ( strncmp(selected_game, DEFAULT_CIBAR_FILENAME, strlen(DEFAULT_CIBAR_FILENAME)) == 0 )
	  cibyl_default_game_selected = 1;
        NOPH_Display_setCurrent(display, cur);

        s9_init(&sarien_s9, 200);
        s9_input_init(&sarien_s9);

	game.clock_enabled = FALSE;
	game.state = STATE_INIT;

	opt.scale = 1;

	init_machine (argc, argv);

	game.color_fg = 15;
	game.color_bg = 0;

	if ((game.sbuf = calloc (_WIDTH, _HEIGHT)) == NULL) {
		ec = err_NotEnoughMemory;
		goto bail_out;
	}

	if (init_sprites () != err_OK) {
		ec = err_NotEnoughMemory;
		goto bail_out_3;
	}

	if (init_video () != err_OK) {
		ec = err_Unk;
		goto bail_out_4;
	}

	report ("Enabling interpreter console\n");
	console_init ();
	report ("--- Starting console ---\n\n");
	if (!opt.gfxhacks)
		report ("Graphics driver hacks disabled (if any)\n");

	game.ver = -1;		/* Don't display the conf file warning */

	_D ("Detect game");
	if (agi_detect_game (selected_game) == err_OK)
	{
		game.state = STATE_LOADED;
		_D (_D_WARN "game loaded");
	} else {
          report ("Could not open AGI game \"%s%s\".\n\n",
                  get_current_directory (),selected_game);
	}

	_D ("Init sound");
	init_sound ();

	report (" \nSarien " VERSION " is ready.\n");
	if (game.state < STATE_LOADED) {
       		console_prompt ();
		do { main_cycle (); } while (game.state < STATE_RUNNING);
		if (game.ver < 0) game.ver = 0;	/* Enable conf file warning */
	}

	ec = run_game ();

	deinit_sound ();
	deinit_video ();

bail_out_4:
	deinit_sprites ();
bail_out_3:
	free (game.sbuf);
bail_out:
	if (ec == err_OK || ec == err_DoNothing) {
		deinit_machine ();
		exit (ec);
	}

	printf ("Error %04i: ", ec);

	switch (ec) {
	case err_BadCLISwitch:
		printf("Bad CLI switch.\n");
		break;
	case err_InvalidAGIFile:
		printf("Invalid or inexistent AGI file.\n");
		break;
	case err_BadFileOpen:
		printf("Unable to open file.\n");
		break;
	case err_NotEnoughMemory:
		printf("Not enough memory.\n");
		break;
	case err_BadResource:
		printf("Error in resource.\n");
		break;
	case err_UnknownAGIVersion:
		printf("Unknown AGI version.\n");
		break;
	case err_NoGameList:
		printf("No game ID List was found!\n");
		break;
	}
	printf("\nUse parameter -h to list the command line options\n");

	deinit_machine ();

	return ec;
}
