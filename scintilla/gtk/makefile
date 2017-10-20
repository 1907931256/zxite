# Make file for Scintilla on Linux or compatible OS
# Copyright 1998-2010 by Neil Hodgson <neilh@scintilla.org>
# The License.txt file describes the conditions under which this software may be distributed.
# This makefile assumes GCC 4.3 is used and changes will be needed to use other compilers.
# GNU make does not like \r\n line endings so should be saved to CVS in binary form.
# Builds for GTK+ 2 and no longer supports GTK+ 1.

# Use V=1 to switch from silent to verbose
V ?= 0
NO_LUA := 1

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
RANLIB = touch

ifndef windir
ifeq ($(shell uname),Darwin)
RANLIB = ranlib
endif
endif

COMPLIB=../bin/scintilla.a

vpath %.h ../src ../include
vpath %.cxx ../src

INCLUDEDIRS=-I ../include -I ../src
CXXBASEFLAGS=-Wall -Wno-missing-braces -Wno-char-subscripts -pedantic -DGTK -DSCI_LEXER $(INCLUDEDIRS) -std=gnu++11

ifdef NOTHREADS
THREADFLAGS=-DG_THREADS_IMPL_NONE
else
THREADFLAGS=
endif

ifdef DEBUG
CXXFLAGS=-DDEBUG -g $(CXXBASEFLAGS) $(THREADFLAGS)
else
CXXFLAGS=-DNDEBUG -Os $(CXXBASEFLAGS) $(THREADFLAGS)
endif

CONFIGFLAGS:=$(shell pkg-config --cflags gtk+-2.0)
MARSHALLER=scintilla-marshal.o

.cxx.o:
	@echo $(ANSI_WHITE)"[C++]"$(ANSI_OFF) $@
	$(AT)$(CC) $(CONFIGFLAGS) $(CXXFLAGS) -c $<
.c.o:
	@echo $(ANSI_WHITE)"[C]"$(ANSI_OFF) $@
	$(AT)$(CCOMP) $(CONFIGFLAGS) $(CXXFLAGS) -w -c $<

LEXOBJS:=$(addsuffix .o,$(basename $(notdir $(wildcard ../src/Lex*.cxx))))

all: $(COMPLIB)

clean:
	rm -f *.o $(COMPLIB)

deps:
	@echo $(ANSI_WHITE)"[dep]"$(ANSI_OFF) $@
	$(AT)$(CC) -MM $(CONFIGFLAGS) $(CXXFLAGS) *.cxx ../src/*.cxx | sed -e 's/\/usr.* //' | grep [a-zA-Z] >deps.mak

$(COMPLIB): DocumentAccessor.o WindowAccessor.o KeyWords.o StyleContext.o \
	CharClassify.o Decoration.o Document.o PerLine.o CallTip.o \
	ScintillaBase.o ContractionState.o Editor.o ExternalLexer.o PropSet.o PlatGTK.o \
	KeyMap.o LineMarker.o PositionCache.o ScintillaGTK.o CellBuffer.o ViewStyle.o \
	RESearch.o RunStyles.o Selection.o Style.o Indicator.o AutoComplete.o UniConversion.o XPM.o \
	$(MARSHALLER) $(LEXOBJS)
	$(AR) rc $@ $^
	$(RANLIB) $@

# Automatically generate header dependencies with "make deps"
include deps.mak
