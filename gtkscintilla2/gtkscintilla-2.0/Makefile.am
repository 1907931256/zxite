lib_LTLIBRARIES = libgtkscintilla-2.0.la

nodist_libgtkscintilla_2_0_la_SOURCES = \
	gtkscintilla.c  \
	gtkscintilla.h
	
INCLUDES = \
	-I. -I..		\
	$(GTKSCINTILLA_CFLAGS)

libgtkscintilla_2_0_la_LIBADD = \
	$(GTKSCINTILLA_LIBS)    \
	../scintilla-lib/libscintilla.la

library_includedir = $(includedir)/gtkscintilla-2.0
nodist_library_include_HEADERS = gtkscintilla.h

EXTRA_DIST = \
	gtkscintilla.ctmpl	\
	gtkscintilla.htmpl

gtkscintilla.c gtkscintilla.h: \
	../data/scigen.py	\
	../scintilla/include/Scintilla.iface
	$(PYTHON) ../data/scigen.py
	
