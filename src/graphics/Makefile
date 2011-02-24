# $Id: Makefile,v 1.9 2003/09/02 01:10:55 cmatsuoka Exp $

XCFLAGS = -I../include
OBJS	=
DFILES	= Makefile
CFILES	=
DDIRS	= msdos sdl x11 svgalib allegro win32 photon dive macos amiga \
	  script dc directx cocoa
XDFILES =
LIB	=

all:
	(cd $(CONSOLE_DRIVER); $(MAKE) all)

include ../../Rules

