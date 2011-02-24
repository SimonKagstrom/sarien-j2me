# Find out the path to the wireless toolkit                                                                                   
PREVERIFY_BINARY_PATH=$(shell which preverify)
PREVERIFY_REAL_PATH=$(shell readlink -f $(PREVERIFY_BINARY_PATH))
PREVERIFY_DIRNAME=$(shell dirname $(PREVERIFY_BINARY_PATH))
WTK_PATH=$(PREVERIFY_DIRNAME)/../

CFLAGS  =
DEFINES =-DNDEBUG -DVERSION='"$(VERSION)"'
INCLUDES=-Iinclude -I../src/include/
OBJS = 	../src/core/global.o ../src/core/agi_v2.o ../src/filesys/cibyl/cibyl-fileops.o \
		../src/core/agi.o ../src/core/words.o ../src/core/objects.o \
		../src/core/picture.o ../src/core/view.o ../src/core/id.o ../src/core/logic.o \
		../src/core/op_test.o ../src/core/op_cmd.o ../src/core/op_dbg.o ../src/core/patches.o \
		../src/core/keyboard.o ../src/core/rand.o ../src/core/cibyl/main.o ../src/core/menu.o \
		../src/core/font.o ../src/core/lzw.o ../src/core/savegame.o ../src/core/sound.o \
		../src/core/silent.o ../src/core/cycle.o ../src/core/inv.o  ../src/core/checks.o\
		../src/core/text.o ../src/core/graphics.o ../src/core/sprite.o ../src/core/motion.o \
		../src/filesys/unix/fileglob.o ../src/sound/dummy/dummy.o ../src/filesys/cibyl/path.o \
        ../src/graphics/cibyl/cibyl.o ../src/core/console.o ../src/core/cli.o \
        ../src/graphics/cibyl/s9_input.o
        # ../src/core/agi_v3.o ../src/core/agi_v4.o

LDLIBS = -lcrt0 -Wl,--start-group -lcrt0 -lmidp -ljsr075 -lcibar -ls9 -lstdc++ -lc -Wl,--end-group -ljava


all: sarien.jar

include/cibyl-syscalls.db: syscalls/command_mgr/include/command_mgr.h
	cibyl-generate-c-header -o include syscalls command_mgr

tmpclasses/Cibyl.j: program
	cibyl-mips2java -DJSR075 -I include -O -d tmpclasses program


%.o: %.c
	mips-cibyl-elf-gcc $(INCLUDES) $(CFLAGS) $(DEFINES) -c -Os -o $@ $<

program: include/cibyl-syscalls.db $(OBJS)
	mips-cibyl-elf-gcc -o $@ $(OBJS) $(LDLIBS)

res/Time_Quest___Chad_Goulding_\(built-in\).cibar: time_quest/
	install -d res
	cibyl-generate-cibar $< $@

sarien.jar: tmpclasses/Cibyl.j res/Time_Quest___Chad_Goulding_\(built-in\).cibar
	install -d res
	cp `cibyl-config --prefix`/mips-cibyl-elf/sys-root/usr/share/java/Main.java tmpclasses
	cp `cibyl-config --prefix`/mips-cibyl-elf/sys-root/usr/share/java/GameScreenCanvas.java tmpclasses
	cp CibylConfig.java tmpclasses/
	cd tmpclasses && javac -classpath : -source 1.4 -bootclasspath $(WTK_PATH)/lib/cldcapi11.jar:$(WTK_PATH)/lib/midpapi20.jar:$(WTK_PATH)/lib/jsr75.jar -source 1.4 *.java
	preverify -classpath $(WTK_PATH)/lib/cldcapi11.jar:$(WTK_PATH)/lib/midpapi20.jar:$(WTK_PATH)/lib/jsr75.jar: -d classes/ tmpclasses/
	cp tmpclasses/program.data.bin res/
	cp resources/sarien.conf res/
	jar cfm $@ MANIFEST.MF -C classes . -C res .

sarien.jad: sarien.jad.templ sarien.jar
	cat $< > $@
	echo -n "MIDlet-Jar-Size: " >> $@
	wc -c sarien.jar | cut -d ' ' -f -1 >> $@

clean:
	rm -rf classes tmpclasses program *.jar *.jad *~ res include ../src/*.o

FORCE:
