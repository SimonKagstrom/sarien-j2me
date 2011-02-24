# $Id: Makefile,v 1.3 2002/11/06 07:57:30 cmatsuoka Exp $

XCFLAGS	= -I../../include $(X11CFLAGS)
OBJS	= ph.o
DFILES	= Makefile $(OBJS:.o=.c)
CFILES	=
DDIRS	=
LIB	= ../../../lib/libsarien.a

all: $(LIB)

x11.o: ph.c

include ../../../Version
include ../../../Rules

