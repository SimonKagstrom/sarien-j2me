# $Id: Makefile,v 1.5 2002/11/16 15:32:35 cmatsuoka Exp $

XCFLAGS =
OBJS	=
DFILES	= Makefile
CFILES	=
DDIRS	= include filesys sound graphics core
XDDIRS	= tests
XDFILES =
LIB	=

all:
	for i in $(DDIRS); do		\
		(cd $$i; $(MAKE) all);	\
	done

include ../Rules

