# $Id: Makefile,v 1.3 2002/09/03 00:47:05 cmatsuoka Exp $

XCFLAGS	= -I ../../include
OBJS	= winres.o
DFILES	= Makefile winmain.c sarien.ico sarien.info winres.rc resource.h
CFILES	=
DDIRS	=
LIB	= ../../../lib/libsarien.a

all: $(LIB)

include ../../../Rules
include ../../../Version

winres.o: winres.rc
	$(WINDRES) --define VERSION=\"$(VERSION)\" -i $*.rc -o $@ -Irc -Ocoff

