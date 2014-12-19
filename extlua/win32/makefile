# Make file for SciTE on Windows
# Copyright 1998-2000 by Neil Hodgson <neilh@scintilla.org>
# The License.txt file describes the conditions under which this software may be distributed.
# This makefile assumes the mingw32 version of GCC 2.95.2 is used and changes will
# be needed to use other compilers.

.SUFFIXES: .cxx
CPP = g++
CC = gcc
DLLWRAP = dllwrap

PROG	= ../bin/SciTE.exe
PROGSTATIC = ../bin/Sc1.exe

vpath %.h ../src ../../scintilla/include ../../scintilla/win32
vpath %.cxx ../src
vpath %.c ../lua
vpath %.o ../../scintilla/win32

LDFLAGS = -lkernel32 -lgdi32 -luser32 -lcomdlg32 -lcomctl32 -limm32 -lole32 -luuid
#CXXFLAGS = -W -Wall
# Add -MMD to get dependencies
#CXXFLAGS = -g -pg -pedantic -Os -fno-exceptions -fvtable-thunks -fno-rtti
INCLUDEDIRS=-I ../../scintilla/include -I ../../scintilla/win32 -I ../src -I../lua
RCINCLUDEDIRS=--include-dir ../../scintilla/win32 --include-dir ../src 
CXXFLAGS = -W -Wall -pedantic $(INCLUDEDIRS) -DLUA_SCRIPTING -Os -fno-exceptions -fvtable-thunks -fno-rtti

.cxx.o:
	$(CPP) $(CXXFLAGS) -c $< -o $@

.c.o:
	$(CC) $(CXXFLAGS) -c $< -o $@

LEXOBJS	= LexCPP.o LexHTML.o LexLua.o LexOthers.o LexPerl.o LexPython.o LexSQL.o LexVB.o
OBJS	= SciTEBase.o SciTEWin.o WindowAccessor.o PropSet.o PlatWin.o \
	UniConversion.o SciTERes.o LuaExtension.o
LUAOBJS= lapi.o lauxlib.o lbuffer.o lbuiltin.o ldo.o lfunc.o lgc.o llex.o lmem.o lobject.o \
	lparser.o lstate.o lstring.o ltable.o ltm.o lundump.o lvm.o lzio.o liolib.o lstrlib.o

DLLS=..\bin\Scintilla.dll ..\bin\SciLexer.dll

PROPS=..\bin\SciTEGlobal.properties ..\bin\cpp.properties ..\bin\html.properties \
..\bin\latex.properties ..\bin\lua.properties ..\bin\others.properties \
..\bin\perl.properties ..\bin\python.properties ..\bin\sql.properties ..\bin\vb.properties

ALL:	$(PROG) $(PROGSTATIC) $(DLLS) $(PROPS)

clean:
	del /q *.exe *.o *.obj *.dll *.res *.map

..\bin\Scintilla.dll:	..\..\scintilla\bin\Scintilla.dll
	copy ..\..\scintilla\bin\Scintilla.dll $@

..\bin\SciLexer.dll:	..\..\scintilla\bin\SciLexer.dll
	copy ..\..\scintilla\bin\SciLexer.dll $@
	
..\bin\SciTEGlobal.properties:	..\src\SciTEGlobal.properties
	copy $^ $@
..\bin\cpp.properties:	..\src\cpp.properties
	copy $^ $@
..\bin\html.properties:	..\src\html.properties
	copy $^ $@
..\bin\latex.properties:	..\src\latex.properties
	copy $^ $@
..\bin\lua.properties:	..\src\lua.properties
	copy $^ $@
..\bin\others.properties:	..\src\others.properties
	copy $^ $@
..\bin\perl.properties:	..\src\perl.properties
	copy $^ $@
..\bin\python.properties:	..\src\python.properties
	copy $^ $@
..\bin\sql.properties:	..\src\sql.properties
	copy $^ $@
..\bin\vb.properties:	..\src\vb.properties
	copy $^ $@

$(PROG): $(OBJS) $(LUAOBJS)
	$(CPP) "-Wl,-s" -Xlinker --subsystem -Xlinker windows -o  $@ $^ $(LDFLAGS)

OBJSSTATIC = Sc1.o SciTEBase.o ScintillaWinL.o ScintillaBaseL.o Editor.o \
	WindowAccessor.o DocumentAccessor.o KeyWords.o $(LEXOBJS) PropSet.o \
	ContractionState.o Document.o CellBuffer.o CallTip.o Sc1Res.o PlatWin.o UniConversion.o \
	KeyMap.o Indicator.o LineMarker.o Style.o ViewStyle.o AutoComplete.o \
	LuaExtension.o

$(PROGSTATIC): $(OBJSSTATIC) $(LUAOBJS)
	$(CPP) "-Wl,-s" -Xlinker --subsystem -Xlinker windows -o $@ $^ $(LDFLAGS)

SciTEBase.o: SciTEBase.cxx Platform.h SciTE.h PropSet.h Accessor.h WindowAccessor.h \
 KeyWords.h Scintilla.h SciLexer.h SciTEBase.h
SciTEWin.o: SciTEWin.cxx Platform.h SciTE.h PropSet.h Accessor.h WindowAccessor.h \
 KeyWords.h Scintilla.h SciTEBase.h

#	$(CPP) -Xlinker --subsystem -Xlinker windows -o $(PROGSTATIC) $(OBJSSTATIC) $(LDFLAGS)

Sc1.o: SciTEWin.cxx SciTE.h Accessor.h WindowAccessor.h KeyWords.h PropSet.h Scintilla.h
	$(CPP) $(CXXFLAGS) -D STATIC_BUILD -c $< -o $@

SciTERes.o:	SciTERes.rc SciTE.h PlatformRes.h
	windres $(RCINCLUDEDIRS) SciTERes.rc $@

Sc1Res.o:	SciTERes.rc SciTE.h PlatformRes.h
	windres $(RCINCLUDEDIRS) SciTERes.rc --define STATIC_BUILD $@
