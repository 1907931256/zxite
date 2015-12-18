# Make file for SciTE on Linux or compatible OS
# Copyright 1998-2010 by Neil Hodgson <neilh@scintilla.org>
# The License.txt file describes the conditions under which this software may be distributed.
# This makefile assumes GCC 4.x is used and changes will be needed to use other compilers.
# GNU make does not like \r\n line endings so should be saved to CVS in binary form.


# Use V=1 to switch from silent to verbose
V ?= 0

# Generic variables for most rules
export VERBOSE   := $(filter 1, $(V))
export AT        := $(if $(VERBOSE),,@)
export MAKEFLAGS += $(if $(VERBOSE),,--no-print-directory)

# Use C=0 to disable ANSI colors
C ?= 1
ifeq ($(C),1)
# Generating ESC char with printf will ensure it will be recognized by different tools
  ESC_CHAR := $(shell printf '\033')
  export ANSI_RED  := '$(ESC_CHAR)[31m'
  export ANSI_REDB := '$(ESC_CHAR)[1;31m'
  export ANSI_CYAN := '$(ESC_CHAR)[36m'
  export ANSI_WHITE:= '$(ESC_CHAR)[37m'
  export ANSI_OFF  := '$(ESC_CHAR)[0m'
else
  export ANSI_RED  :=
  export ANSI_CYAN :=
  export ANSI_OFF  :=
endif



.SUFFIXES: .cxx .o .h .a .c
ifdef CLANG
CC := clang
CCOMP := clang
else
CC := g++
CCOMP := gcc
endif
AR := ar

# For the Gnome desktop stuff to work, prefix must point to where Gnome thinks it is.
CONFIGFLAGS   := $(shell pkg-config --cflags gtk+-2.0)
CONFIGLIB     := $(shell pkg-config --libs gtk+-2.0 gthread-2.0 gmodule-2.0)
CONFIGTHREADS :=
gnomeprefix   := $(shell pkg-config --variable=prefix gtk+-2.0 2>/dev/null)
ifndef prefix
ifdef gnomeprefix
  prefix := $(gnomeprefix)
else
  prefix := /usr
endif
endif
datadir      := $(prefix)/share
pixmapdir    := $(datadir)/pixmaps
bindir       := $(prefix)/bin
SYSCONF_PATH := $(prefix)/share/scite

INSTALL := install
PROG    := ../bin/SciTE

all: $(PROG)

vpath %.h ../src ../../scintilla/include
vpath %.cxx ../src

CFLAGS := -I ../../scintilla/include -I ../src
CFLAGS += -W -Wall -Wno-char-subscripts -pedantic -DGTK -DSCI_LEXER -DPIXMAP_PATH=\"$(pixmapdir)\" -DSYSCONF_PATH=\"$(SYSCONF_PATH)\"

DEBUG := 1

ifdef DEBUG
CFLAGS += -DDEBUG -g
else
CFLAGS += -DNDEBUG -Os
endif

ifndef NO_LUA
LUA_CORE_OBJS := lapi.o lcode.o ldebug.o ldo.o ldump.o lfunc.o lgc.o llex.o \
		lmem.o lobject.o lopcodes.o lparser.o lstate.o lstring.o \
		ltable.o ltm.o lundump.o lvm.o lzio.o

LUA_LIB_OBJS :=	lauxlib.o lbaselib.o ldblib.o liolib.o lmathlib.o ltablib.o \
		lstrlib.o loadlib.o loslib.o linit.o

LUA_OBJS := LuaExtension.o $(LUA_CORE_OBJS) $(LUA_LIB_OBJS)

CFLAGS += -I../lua/include

$(LUA_CORE_OBJS): ../lua/src/*.c
	@echo $(ANSI_WHITE)"[CC]"$(ANSI_OFF) $@
	$(AT)$(CCOMP) -DLUA_USE_POSIX $(CFLAGS) -c ../lua/src/*.c

$(LUA_LIB_OBJS): ../lua/src/lib/*.c
	@echo $(ANSI_WHITE)"[CC]"$(ANSI_OFF) $@
	$(AT)$(CCOMP) -DLUA_USE_POSIX $(CFLAGS) -c ../lua/src/lib/*.c

else
CFLAGS += -DNO_LUA
endif

ifeq ($(shell uname), Darwin)
CFLAGS += -Dunix
endif

CXXFLAGS := $(CFLAGS) -std=gnu++11

.cxx.o:
	@echo $(ANSI_WHITE)"[C++]"$(ANSI_OFF) $@
	$(AT)$(CC) $(CONFIGFLAGS) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f *.o $(PROG)

deps:
	@echo $(ANSI_WHITE)"[dep]"$(ANSI_OFF) $@
	$(AT)$(CC) -MM $(CONFIGFLAGS) $(CXXFLAGS) *.cxx ../src/*.cxx | sed -e 's/\/usr.* //' | grep [a-zA-Z] >deps.mak

# make should be run in ../../scintilla/gtk to compile all the lexers.
COMPLIB := ../../scintilla/bin/scintilla.a

$(PROG): SciTEGTK.o GUIGTK.o FilePath.o SciTEBase.o SciTEBuffers.o SciTEIO.o StringList.o Exporters.o \
    PropSetFile.o MultiplexExtension.o DirectorExtension.o SciTEProps.o StyleWriter.o Utf8_16.o \
    JobQueue.o GTKMutex.o IFaceTable.o Project.o $(COMPLIB) $(LUA_OBJS)
	@echo $(ANSI_WHITE)"[LD]"$(ANSI_OFF) $@
	$(AT)$(CC) `$(CONFIGTHREADS)` -rdynamic -Wl,--as-needed -Wl,--version-script lua.vers -DGTK $^ -o $@ $(CONFIGLIB) -ldl -lstdc++

# Automatically generate header dependencies with "make deps"
-include deps.mak

# The two last install commands will fail if Gnome is not installed or is not at $(prefix).
# This is OK - just means no SciTE in the Gnome Applications menu
# Dead:	install -D SciTEGTK.properties $(SYSCONF_PATH)/SciTEGlobal.properties
install:
	$(INSTALL) -m 755 -d $(DESTDIR)$(bindir) $(DESTDIR)$(SYSCONF_PATH)

	$(INSTALL) -m 755 $(PROG) $(DESTDIR)$(bindir)

	for files in ../src/*.properties ../doc/*.html ../doc/SciTEIco.png ../doc/PrintHi.png; \
	do \
		$(INSTALL) -m 644 $$files $(DESTDIR)$(SYSCONF_PATH); \
	done

ifdef gnomeprefix
	$(INSTALL) -m 755 -D SciTE.desktop $(DESTDIR)$(datadir)/applications/SciTE.desktop
	$(INSTALL) -m 644 -D Sci48M.png $(DESTDIR)$(pixmapdir)/Sci48M.png
endif

uninstall:
	rm -f $(DESTDIR)$(bindir)/SciTE
	rm -rf $(DESTDIR)$(SYSCONF_PATH)
ifdef gnomeprefix
	rm -f $(DESTDIR)$(datadir)/applications/SciTE.desktop
	rm -f $(DESTDIR)$(pixmapdir)/Sci48M.png
endif
