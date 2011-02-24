# $Id: Makefile,v 1.8 2004/01/08 00:23:59 cmatsuoka Exp $

XCFLAGS	= -I../include
XLDFLAGS=
OBJS	= format.o arith.o flag.o picture.o random.o inv.o
DFILES	= README Makefile $(OBJS:.o=.c) orat.c orat.h test.c test.h
CFILES	=
DDIRS	= data
XLIBS	= -L../../lib -lagi -lsarien

all: test

test: test.o orat.o $(OBJS)
	$(LD) $(LDFLAGS) $(XLDFLAGS) $+ $(XLIBS) $(X11LIBS) $(LIBS)

include ../../Version
include ../../Rules

include depend

