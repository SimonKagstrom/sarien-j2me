# $Id: Makefile,v 1.9 2003/09/02 13:00:57 cmatsuoka Exp $

XCFLAGS = -I../include
OBJS	=
DFILES	= Makefile
CFILES	=
DDIRS	= oss sdl dummy alsa bsd solaris sgi win32 freebsd qnx dart \
	  amiga macos macosx dc allegro
XDFILES =
LIB	=

all:
	(cd $(SOUND_DRIVER); $(MAKE) all)

include ../../Rules

