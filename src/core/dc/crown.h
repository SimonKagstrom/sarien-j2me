/*  Sarien - A Sierra AGI resource interpreter engine
 *  Copyright (C) 1999-2001 Stuart George and Claudio Matsuoka
 *  Dreamcast files Copyright (C) 2002 Brian Peek/Ganksoft Entertainment
 *  
 *  $Id: crown.h,v 1.1 2002/03/09 00:54:23 peekb Exp $
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; see docs/COPYING for further details.
 */
 

// colors
short crown_palette[16] = {
0xf008, 0xfff0, 0xfdc0, 0xfd00, 
0xf800, 0xf000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
};

//data
unsigned char crown_data[512] = {
0x55, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x55, 
0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 
0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x52, 0x15, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x52, 0x25, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x55, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x55, 0x00, 
0x05, 0x21, 0x50, 0x00, 0x00, 0x05, 0x55, 0x52, 0x25, 0x55, 0x50, 0x00, 0x00, 0x05, 0x21, 0x50, 
0x05, 0x22, 0x15, 0x00, 0x00, 0x53, 0x33, 0x52, 0x25, 0x33, 0x35, 0x00, 0x00, 0x52, 0x22, 0x50, 
0x00, 0x55, 0x22, 0x50, 0x05, 0x33, 0x34, 0x52, 0x25, 0x43, 0x33, 0x50, 0x05, 0x22, 0x55, 0x00, 
0x00, 0x05, 0x22, 0x15, 0x54, 0x33, 0x34, 0x52, 0x25, 0x43, 0x33, 0x45, 0x52, 0x21, 0x50, 0x00, 
0x00, 0x05, 0x22, 0x11, 0x54, 0x33, 0x34, 0x52, 0x25, 0x43, 0x33, 0x45, 0x22, 0x11, 0x50, 0x00, 
0x00, 0x00, 0x52, 0x21, 0x15, 0x43, 0x35, 0x22, 0x11, 0x53, 0x34, 0x52, 0x21, 0x15, 0x00, 0x00, 
0x00, 0x00, 0x52, 0x21, 0x11, 0x54, 0x45, 0x22, 0x11, 0x54, 0x45, 0x22, 0x11, 0x15, 0x00, 0x00, 
0x00, 0x00, 0x52, 0x21, 0x11, 0x15, 0x45, 0x22, 0x11, 0x54, 0x52, 0x21, 0x11, 0x15, 0x00, 0x00, 
0x00, 0x00, 0x05, 0x22, 0x11, 0x11, 0x52, 0x21, 0x11, 0x15, 0x22, 0x11, 0x11, 0x50, 0x00, 0x00, 
0x00, 0x00, 0x05, 0x22, 0x11, 0x11, 0x22, 0x11, 0x11, 0x12, 0x21, 0x11, 0x11, 0x50, 0x00, 0x00, 
0x00, 0x00, 0x05, 0x22, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x50, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x52, 0x21, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x15, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x52, 0x21, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x15, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x52, 0x21, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x15, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x05, 0x22, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x50, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x05, 0x22, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x50, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x00, 0x00, 
0x00, 0x05, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x50, 0x00, 
0x00, 0x05, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x50, 0x00, 
0x50, 0x00, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x00, 0x05, 
0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 
0x55, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x55, 
};

