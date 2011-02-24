/*********************************************************************
 *
 * Copyright (C) 2007,  Blekinge Institute of Technology
 *
 * Filename:      cibyl-fileops.c
 * Author:        Simon Kagstrom <ska@bth.se>
 * Description:   File operations
 *
 * $Id:$
 *
 ********************************************************************/
#include "sarien.h"

#undef opendir
#undef readdir
#undef closedir
#undef fopen

#include <cibar.h>
#include <stdio.h>

#include <sys/types.h>
#include <dirent.h>

static int cur_file = 0;

struct dirent *cibyl_readdir(DIR *dir)
{
  static struct dirent out;
  cibar_t *p = (cibar_t*)dir;

  if (cur_file >= p->n_files)
    return NULL;

  snprintf( out.d_name, 255, "%s", p->files[cur_file].name );

  cur_file++;

  return &out;
}

static cibar_t *cur;

DIR *cibyl_opendir(const char *name)
{
  char buf[255];
  FILE *f;

  snprintf(buf, 255, "%s%s", get_current_directory(), name);

  cur_file = 0;

  if (cur)
    {
      return (DIR*)cur;
    }

  f = fopen(buf, "r");
  if (!f)
    return NULL;
  cur = cibar_open(f);
  fclose(f);
  return (DIR*)cur;
}

int cibyl_closedir(DIR *dir)
{
  cibar_t *p = (cibar_t*)dir;

  /* Don't do anything (cleanup at exit) */

  return 0;
}

FILE *cibyl_open(const char *path, const char *mode)
{
  printf("Opening %s\n", path);

  /* Real file  open it */
  if (strcmp(path, get_config_file()) == 0)
    {
      return fopen(path, mode);
    }

  printf("... as cibar\n");

  return cibar_file_open(cur, strpbrk(path, "/")+1);
}


char **cibyl_read_cibar_directory(char *base_dir)
{
        DIR *d = opendir(base_dir);
        char **out;
        int cur = 0;
        struct dirent *de;

        if (!d)
                return NULL;

        out = malloc(128 * sizeof(char*));
        out[cur] = NULL;

        for (de = readdir(d);
             de;
             de = readdir(d))
        {
                if (strstr(de->d_name, ".cibar"))
                {
                        char *p;
                        /* Sanity check: More than 127 AGI games! */
                        p = strdup(de->d_name);
                        out[cur++] = p;
                        out[cur] = NULL;
                        if (cur > 127)
                                return NULL;
                }
        }
        closedir(d);

        return out;
}
