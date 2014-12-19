# Makefile for the scintilla library to be used from within the gtkscintilla2
# tree
#
# We're using this directory to be able to keep a clean scintilla tree in our
# cvs but have a libtool-compatible library to build with

vpath %.h ../scintilla/src ../scintilla/include
vpath %.cxx ../scintilla/src ../scintilla/gtk

noinst_LTLIBRARIES = libscintilla.la

nodist_libscintilla_la_SOURCES = \
	DocumentAccessor.cxx	\
	WindowAccessor.cxx	\
	KeyWords.cxx		\
	StyleContext.cxx	\
	Document.cxx		\
	CallTip.cxx		\
	ScintillaBase.cxx	\
	ContractionState.cxx	\
	Editor.cxx		\
	PropSet.cxx		\
	PlatGTK.cxx		\
	KeyMap.cxx		\
	LineMarker.cxx		\
	ScintillaGTK.cxx	\
	CellBuffer.cxx		\
	ViewStyle.cxx		\
	RESearch.cxx		\
	Style.cxx		\
	Indicator.cxx		\
	AutoComplete.cxx	\
	UniConversion.cxx	\
	XPM.cxx

INCLUDES = \
	-I../scintilla/src	\
	-I../scintilla/include	\
	-W -Wall -DGTK -DGTK2	\
	-DSCI_LEXER		\
	$(GTKSCINTILLA_CFLAGS)

# SCINTILLA_INCLUDE = include
# SCINTILLA_LIB = bin/scintilla.a
# SCINTILLA_IFACE = $(SCINTILLA_INCLUDE)/Scintilla.iface

#all: $(SCINTILLA_LIB)

#EXTRA_DIST = \
#	bin		\
#	delbin.bat	\
#	delcvs.bat	\
#	doc		\
#	gtk		\
#	include		\
#	License.txt	\
#	README		\
#	src		\
#	tgzsrc		\
#	vcbuild		\
#	version.txt	\
#	win32		\
#	zipsrc.bat

#$(SCINTILLA_LIB):
#	cd gtk ; $(MAKE) GTK2=1

#gtkscintilla.h gtkscintilla.c: scigen.py $(SCINTILLA_IFACE)
#	python scigen.py

#clean:
#	rm -f $(SCINTILLA_LIB)
#	rm -f gtk/*.o

