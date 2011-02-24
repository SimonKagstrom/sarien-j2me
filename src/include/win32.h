/*  Sarien - A Sierra AGI resource interpreter engine
 *  Copyright (C) 1999-2001 Stuart George and Claudio Matsuoka
 *  
 *  $Id: win32.h,v 1.7 2002/09/08 15:19:16 cmatsuoka Exp $
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; see docs/COPYING for further details.
 */

#ifndef _WIN_32_H
#define _WIN_32_H 1

#define snprintf _vsnprintf

#ifdef	__cplusplus
extern "C" {
#endif

HWND     hwndMain;
void     (*flush_sound) (PWAVEHDR pWave);

BOOL CALLBACK AboutDlgProc(HWND, UINT, WPARAM, LPARAM);
void open_file (HWND);

#ifdef __cplusplus
} 
#endif

#endif  /* WIN_32_H */

