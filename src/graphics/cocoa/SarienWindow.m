/*  Sarien - A Sierra AGI resource interpreter engine
 *  Copyright (C) 1999-2001 Stuart George and Claudio Matsuoka
 *
 *  $Id: SarienWindow.m,v 1.2 2002/11/06 23:12:37 cmatsuoka Exp $
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; see docs/COPYING for further details.
 *
 *  MacOS X port by Richard Houle <richard.houle@sandtechnology.com>
 */

#import "SarienWindow.h"
#include "sarien.h"
#include "graphics.h"

@implementation SarienWindow

- (void)awakeFromNib
{
	NSRect screenRect;

	screenRect = [[self screen] frame];
    
	if (screenRect.size.width >= 1280)
		maxScale = 4;
	else if (screenRect.size.width >= 1024)
		maxScale = 3;
	else if (screenRect.size.width >= 640)
		maxScale = 2;
	else
		maxScale = 1;
    
	[self setScale: maxScale < 2 ? maxScale : 2];
}


- (void)close
{
	[super close];
	[NSApp terminate: self];
}


- (void)setScale:(int)newScale
{
	NSRect newRect;
	NSMenu* scaleMenu;
    
	if ((newScale > maxScale) || (scale == newScale))
		return;
    
	scaleMenu = [[[NSApp mainMenu] itemWithTag: 100] submenu];
	[[scaleMenu itemWithTag: newScale] setState: NSOnState];
    
	if (scale != 0)
		[[scaleMenu itemWithTag: scale] setState: NSOffState];
    
	scale = newScale;
    
	newRect             = [self frame];
	newRect.size.width  = GFX_WIDTH  * newScale;
	newRect.size.height = GFX_HEIGHT * newScale;
	[self setFrame:newRect display:TRUE animate:TRUE];
}


- (IBAction)setScale1x:(id)sender
{
	[self setScale:1];
}


- (IBAction)setScale2x:(id)sender
{
	[self setScale:2];
}


- (IBAction)setScale3x:(id)sender
{
	[self setScale:3];
}


- (IBAction)setScale4x:(id)sender
{
	[self setScale:4];
}

@end

