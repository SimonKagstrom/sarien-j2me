/* Sarien - A Sierra AGI resource interpreter engine
 * Copyright (C) 1999-2001 Stuart George and Claudio Matsuoka
 *
 * $Id: path.c,v 1.3 2001/09/02 11:29:26 lagernet Exp $
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; see docs/COPYING for further details.
 */

#include <javax/microedition/lcdui.h>
#include <javax/microedition/io.h>
#include <java/util.h>
#include <stdio.h>
#include <string.h>
#include "sarien.h"
#include "agi.h"
#include "command_mgr.h"

static NOPH_List_t fs_list;
static char roots[15][80];
static char root[80];
char *fs_root = NULL;
int cibyl_default_game_selected = 0;

static void select_fs_callback(void *unused)
{
  int nr = NOPH_List_getSelectedIndex(fs_list);

  fs_root = roots[nr];
}

void cibyl_select_fs_root(void *unused)
{
        NOPH_Display_t display = NOPH_Display_getDisplay(NOPH_MIDlet_get());
        NOPH_Displayable_t cur = NOPH_Display_getCurrent(display);
        NOPH_CommandMgr_t cm = NOPH_CommandMgr_getInstance();
        NOPH_Enumeration_t en = NOPH_FileSystemRegistry_listRoots();
        FILE *conf;
        int i = 0;

        fs_root = NULL;

        fs_list = NOPH_List_new("Select fs root", NOPH_Choice_IMPLICIT);

        while (NOPH_Enumeration_hasMoreElements(en))
        {
                NOPH_Object_t o = NOPH_Enumeration_nextElement(en);

                NOPH_String_toCharPtr(o, roots[i], 80);
                NOPH_List_append(fs_list, roots[i], 0);
                NOPH_delete(o);
                i++;
        }
        NOPH_delete(en);
        NOPH_Display_setCurrent(display, fs_list);
        NOPH_CommandMgr_setList(cm, fs_list, select_fs_callback, NULL);

        while(fs_root == NULL)
        {
                NOPH_Thread_sleep(250);
        }
#undef fopen
#undef fread
#undef fwrite
#undef fclose
        conf = fopen("recordstore://sarien-conf:1", "w");
        if (conf)
        {
                char buf[80];

                strncpy(buf, fs_root, 80);
                fwrite(buf, 1, 80, conf);
                fclose(conf);
        }

        NOPH_Display_setCurrent(display, cur);
        NOPH_delete(fs_list);
}


int get_app_dir (char *app_dir, unsigned int size)
{
	strncpy (app_dir, "/", size);
	return 0;
}

char* get_config_file (void)
{
	return "/sarien.conf";
}

char *get_current_directory ()
{
	if (cibyl_default_game_selected)
		return "/";
        snprintf(root, 80, "file:///%ssarien/", fs_root);
	return root;
}

char *get_filesystem_root ()
{
	return fs_root;
}

void set_filesystem_root (char *root)
{
        fs_root = strdup(root);
}
