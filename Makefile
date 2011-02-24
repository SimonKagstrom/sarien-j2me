SUBDIRS = j2me

all: $(SUBDIRS)

# Rule for making subdirectories
.PHONY: $(SUBDIRS)

$(SUBDIRS):
	if [ -f $@/Makefile ]; then make -C $@ ; fi
