######################################################################
##
## Copyright (C) 2005,  Blekinge Institute of Technology
##
## Filename:      Makefile
## Author:        Simon Kagstrom <ska@bth.se>
## Description:   Generic Makefile
##
## $Id: Makefile 9386 2006-08-16 06:28:16Z ska $
##
######################################################################
SUBDIRS = c
RESOURCES=res/program.data.bin res/sarien.conf res/Time_Quest___Chad_Goulding_\(built-in\).cibar
TARGET   =sarien.jar sarien.jad
CIBYL_SYSCALL_SETS ?= resource_manager command_mgr

CIBYL_GENERATE_JAVA_WRAPPERS_OPTS=-D JSR075 -S syscalls -I include
CIBYL_CLASSPATH=$(WTK_PATH)/lib/jsr75.jar
CIBYL_MIPS2JAVA_OPTS +=-Iinclude -DJSR075

all: include/ $(SUBDIRS)

include $(CIBYL_BASE)/build/Rules-j2me.mk

tmpclasses/CibylConfig.java: CibylConfig.java
	cp $< $@

res/Time_Quest___Chad_Goulding_\(built-in\).cibar: time_quest/
	$(CIBYL_BASE)/tools/cibyl-generate-cibar $< $@

include/: FORCE
	$(CIBYL_BASE)/tools/cibyl-generate-c-header -o $@ syscalls/ command_mgr
FORCE:
