# Makefile for Sarien/Cibyl. Based on the SarienDC Makefile
#
# $Id: Makefile.dc,v 1.3 2002/03/09 00:52:31 peekb Exp $

TOPDIR=../..

include $(TOPDIR)/Version

DEFINES = -DNDEBUG -DVERSION='"$(VERSION)"'
INCLUDES= -I$(TOPDIR)/src/include -I../include/
TARGET = program
MAKEFILE = Makefile.dc
OBJS = 	../../src/core/global.o ../../src/core/agi_v2.o ../../src/filesys/cibyl/cibyl-fileops.o \
        ../../src/core/agi.o ../../src/core/words.o ../../src/core/objects.o \
	../../src/core/picture.o ../../src/core/view.o ../../src/core/id.o ../../src/core/logic.o \
	../../src/core/op_test.o ../../src/core/op_cmd.o ../../src/core/op_dbg.o ../../src/core/patches.o \
	../../src/core/keyboard.o ../../src/core/rand.o ../../src/core/cibyl/main.o ../../src/core/menu.o \
	../../src/core/font.o ../../src/core/lzw.o ../../src/core/savegame.o ../../src/core/sound.o \
	../../src/core/silent.o ../../src/core/cycle.o ../../src/core/inv.o  ../../src/core/checks.o\
	../../src/core/text.o ../../src/core/graphics.o ../../src/core/sprite.o ../../src/core/motion.o \
	../../src/filesys/unix/fileglob.o ../../src/sound/dummy/dummy.o ../../src/filesys/cibyl/path.o \
        ../../src/graphics/cibyl/cibyl.o ../../src/core/console.o ../../src/core/cli.o \
        ../../src/graphics/cibyl/s9_input.o
        # ../../src/core/agi_v3.o ../../src/core/agi_v4.o
LDLIBS = -lmidp -ljsr075 -lcibar -ls9

include $(CIBYL_BASE)/build/Rules.mk
