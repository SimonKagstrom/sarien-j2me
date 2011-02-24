# $Id: Makefile,v 1.4 2002/11/06 07:57:31 cmatsuoka Exp $

XCFLAGS	= -I../../include $(X11CFLAGS)
OBJS	= x11.o
DFILES	= Makefile $(OBJS:.o=.c)
CFILES	=
DDIRS	=
XDFILES	= dga.c
LIB	= ../../../lib/libsarien.a

all: $(LIB)

x11.o: x11.c dga.c

include ../../../Version
include ../../../Rules

