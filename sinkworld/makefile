# makefile for SinkWorld
# Only works on Win32 although some targets such as mj.class may work on Linux
# Compatible with GNU make and Microsoft nmake
# Building everything requires Microsoft .NET SDK, Python 2.1, and Java.
#     .NET: http://msdn.microsoft.com/net/
#     Python: http://www.python.org/
#     Java: http://java.sun.com/j2se/1.3/
# Compile all the code with "make"
# Running all the unit tests requires CppUnit, JUnit and NUnit.
#     CppUnit: http://cppunit.sourceforge.net/
#     JUnit: http://www.junit.org/
#     NUnit: http://nunit.sourceforge.net/
# Run the unit tests with "make testall", Must have run "make" first.
# There will be some linker errors in "make testall" that can be ignored.

.SUFFIXES: .cxx .java .h .cs

# For gcc
#CC=g++
#CCOPTS=-O
#LIBS=-lkernel32

# For Microsoft
CC=cl
#CCOPTS=-GX -GR -Ox -GL -Zi -DNDEBUG
CCOPTS=-GX -GR -Ox -Zi -DNDEBUG
LIBS=

MCC=cl
CSC=csc
JAVA=java
#JAVAC=javac
JAVAC=H:\jdk\bin\javac
NUNIT=C:\NUnit\bin\nunit-console
PYTHON=python

# Main target is all of the demonstration programs - C++, MC++, C#, and Java
#all: mc.exe ms.exe mj.class
#all: mc.exe ms.exe mj.class SimpleTest.class SimpleTest.dll
all: mc.exe ms.exe mj.class SimpleTest.exe

CGENERATED=SplitVector_1.h \
	SplitVector_2.h \
	SplitVector_4.h \
	SplitVector_1.cxx \
	SplitVector_2.cxx \
	SplitVector_4.cxx
CSRCS=mc.cxx \
	SinglePosition.cxx \
	SplitVector_0.cxx \
	SplitVector_1.cxx \
	SplitVector_2.cxx \
	SplitVector_4.cxx \
	RunStyles.cxx \
	MarkerSet.cxx \
	SplitText.cxx \
	KeyWords.cxx \
	WordList.cxx \
	Dictionary.cxx \
	OptionSet.cxx \
	Literal.cxx \
	UniConversion.cxx \
	LexASP.cxx \
	LexBraceCount.cxx \
	LexCPP.cxx \
	LexHTML.cxx \
	LexPHP.cxx \
	LexPreProc.cxx \
	LexPython.cxx \
	LexSpell.cxx \
	LexURL.cxx \
	LexVB.cxx
mc.exe: $(CSRCS) \
	SWBase.h \
	SWTest.h \
	SWTest.cxx \
	BadPositionException.h \
	IStyles.h \
	SinglePosition.h \
	SplitVector_0.h \
	SplitVector_1.h \
	SplitVector_2.h \
	SplitVector_4.h \
	SimpleIntegerVector.h \
	SetIntegers.h \
	Partitioning.h \
	UndoAction.h \
	UndoStack.h \
	LineData.h \
	MarkerSet.h \
	RunStyles.h \
	SplitText.h \
	Literal.h \
	Atom.h \
	SetAtoms.h \
	Styler.h \
	KeyWords.h \
	WordList.h \
	SymbolTable.h \
	Dictionary.h \
	OptionSet.h \
	UniConversion.h \
	ILexer.h \
	History.h \
	LexBase.h \
	LexScript.h \
	TextPositions.h \
	LexASP.h \
	LexBraceCount.h \
	LexCPP.h \
	LexHTML.h \
	LexPHP.h \
	LexPreProc.h \
	LexPython.h \
	LexSpell.h \
	LexURL.h \
	LexVB.h
	$(CC) $(CCOPTS) $(CSRCS) SWTest.cxx $(LIBS) -o mc.exe
#	$(CC) $(CCOPTS) mc.cxx SWTest.cxx Kernel32.lib

XLANGCS = \
	IStyles.cs \
	SinglePosition.cs \
	SplitVector_0.cs \
	SplitVector_1.cs \
	SplitVector_2.cs \
	SplitVector_4.cs \
	SimpleIntegerVector.cs \
	SetIntegers.cs \
	Partitioning.cs \
	UndoAction.cs \
	UndoStack.cs \
	LineData.cs \
	MarkerSet.cs \
	RunStyles.cs \
	SplitText.cs \
	Literal.cs \
	Atom.cs \
	SetAtoms.cs \
	Styler.cs \
	KeyWords.cs \
	WordList.cs \
	SymbolTable.cs \
	Dictionary.cs \
	OptionSet.cs \
	UniConversion.cs \
	ILexer.cs \
	History.cs \
	LexBase.cs \
	LexScript.cs \
	TextPositions.cs \
	LexASP.cs \
	LexBraceCount.cs \
	LexCPP.cs \
	LexHTML.cs \
	LexPHP.cs \
	LexPreProc.cs \
	LexPython.cs \
	LexSpell.cs \
	LexURL.cs \
	LexVB.cs

# Do not want to ever build using automatic rules so depend on old file
BadPositionException.cs: Source.css
	touch BadPositionException.cs

CSSRCS=ms.cs SWTest.cs BadPositionException.cs $(XLANGCS)

ms.exe: $(CSSRCS)
	$(CSC) /nowarn:0162 /o /d:xDEBUG /target:exe $(CSSRCS) /r:system.dll

XLANGJAVA = \
	IStyles.java \
	SinglePosition.java \
	SplitVector_0.java \
	SplitVector_1.java \
	SplitVector_2.java \
	SplitVector_4.java \
	SimpleIntegerVector.java \
	SetIntegers.java \
	Partitioning.java \
	UndoAction.java \
	UndoStack.java \
	LineData.java \
	MarkerSet.java \
	RunStyles.java \
	SplitText.java \
	Literal.java \
	Atom.java \
	SetAtoms.java \
	Styler.java \
	KeyWords.java \
	WordList.java \
	SymbolTable.java \
	Dictionary.java \
	OptionSet.java \
	UniConversion.java \
	ILexer.java \
	History.java \
	LexBase.java \
	LexScript.java \
	TextPositions.java \
	LexASP.java \
	LexBraceCount.java \
	LexCPP.java \
	LexHTML.java \
	LexPHP.java \
	LexPreProc.java \
	LexPython.java \
	LexSpell.java \
	LexURL.java \
	LexVB.java

# Do not want to ever build using automatic rules so depend on old file
BadPositionException.java: Source.css
	touch BadPositionException.java

JSRCS=mj.java SWTest.java BadPositionException.java $(XLANGJAVA)
mj.class: $(JSRCS)
	$(JAVAC) -source 1.4 -O $(JSRCS)

SplitVector_1.h: SplitVector.h
	$(PYTHON) Template.py SplitVector.h SplitVector_1.h N=1 T=SW_BYTE
SplitVector_1.cxx: SplitVector.cxx
	$(PYTHON) Template.py SplitVector.cxx SplitVector_1.cxx N=1 T=SW_BYTE
SplitVector_2.h: SplitVector.h
	$(PYTHON) Template.py SplitVector.h SplitVector_2.h N=2 T=SW_SHORT
SplitVector_2.cxx: SplitVector.cxx
	$(PYTHON) Template.py SplitVector.cxx SplitVector_2.cxx N=2 T=SW_SHORT
SplitVector_4.h: SplitVector.h
	$(PYTHON) Template.py SplitVector.h SplitVector_4.h N=4 T=int
SplitVector_4.cxx: SplitVector.cxx
	$(PYTHON) Template.py SplitVector.cxx SplitVector_4.cxx N=4 T=int

.h.cs:
	$(PYTHON) XLang.py -c $<

# Some also depend on implementation files.

SinglePosition.cs: SinglePosition.cxx
SplitText.cs: SplitText.cxx
SplitVector_0.cs: SplitVector_0.cxx
SplitVector_1.cs: SplitVector_1.cxx
SplitVector_2.cs: SplitVector_2.cxx
SplitVector_4.cs: SplitVector_4.cxx
MarkerSet.cs: MarkerSet.cxx
KeyWords.cs: KeyWords.cxx
WordList.cs: WordList.cxx
Dictionary.cs: Dictionary.cxx
OptionSet.cs: OptionSet.cxx
UniConversion.cs: UniConversion.cxx

LexASP.cs: LexASP.cxx
LexBraceCount.cs: LexBraceCount.cxx
LexCPP.cs: LexCPP.cxx
LexHTML.cs: LexHTML.cxx
LexPHP.cs: LexPHP.cxx
LexPreProc.cs: LexPreProc.cxx
LexPython.cs: LexPython.cxx
LexSpell.cs: LexSpell.cxx
LexURL.cs: LexURL.cxx
LexVB.cs: LexVB.cxx

.h.java:
	$(PYTHON) XLang.py -j $<

# Some also depend on implementation files.

SinglePosition.java: SinglePosition.cxx
SplitText.java: SplitText.cxx
SplitVector_0.java: SplitVector_0.cxx
SplitVector_1.java: SplitVector_1.cxx
SplitVector_2.java: SplitVector_2.cxx
SplitVector_4.java: SplitVector_4.cxx
MarkerSet.java: MarkerSet.cxx
KeyWords.java: KeyWords.cxx
WordList.java: WordList.cxx
Dictionary.java: Dictionary.cxx
OptionSet.java: OptionSet.cxx
UniConversion.java: UniConversion.cxx

LexASP.java: LexASP.cxx
LexBraceCount.java: LexBraceCount.cxx
LexCPP.java: LexCPP.cxx
LexHTML.java: LexHTML.cxx
LexPHP.java: LexPHP.cxx
LexPreProc.java: LexPreProc.cxx
LexPython.java: LexPython.cxx
LexSpell.java: LexSpell.cxx
LexURL.java: LexURL.cxx
LexVB.java: LexVB.cxx

CPPUNITDIR=C:\cppunit
CPPUNITSRC=$(CPPUNITDIR)\src\cppunit\xu.lib

CSST=SimpleTest.cs BadPositionException.cs $(XLANGCS)
SimpleTest.dll: $(CSST)
	$(CSC) /nowarn:0162 /debug /target:library $(CSST) /r:system.dll /r:/NUnit/bin/nunit.framework.dll
JST=SimpleTest.java BadPositionException.java $(XLANGJAVA)
SimpleTest.class: $(JST)
	$(JAVAC) -source 1.4 -O $(JST)
COBJS=\
	SinglePosition.obj \
	SplitVector_0.obj \
	SplitVector_1.obj \
	SplitVector_2.obj \
	SplitVector_4.obj \
	RunStyles.obj \
	MarkerSet.obj \
	SplitText.obj \
	KeyWords.obj \
	WordList.obj \
	Literal.obj \
	UniConversion.obj \
	LexASP.obj \
	LexCPP.obj \
	LexBraceCount.obj \
	LexHTML.obj \
	LexPHP.obj \
	LexPreProc.obj \
	LexPython.obj \
	LexSpell.obj \
	LexURL.obj \
	LexVB.obj
SimpleTest.exe: SimpleTest.cxx \
	SWTest.h \
	SWTest.cxx \
	IStyles.h \
	SinglePosition.h \
	SplitVector_0.h \
	SplitVector_1.h \
	SplitVector_2.h \
	SplitVector_4.h \
	UndoAction.h \
	UndoStack.h \
	SplitText.h $(COBJS)
	$(CC) $(CCOPTS) -I $(CPPUNITDIR)\include SimpleTest.cxx SWTest.cxx $(COBJS) $(CPPUNITSRC) Kernel32.lib /link /FORCE:MULTIPLE

#testall: SimpleTest.dll  SimpleTest.class SimpleTest.exe
#	SimpleTest
testall: SimpleTest.dll  SimpleTest.class
	$(JAVA) junit.textui.TestRunner SimpleTest
	$(NUNIT) SimpleTest.dll

clean:
	del *.class *.obj *.exe *.dll *.pdb *.pyc $(XLANGCS) $(XLANGJAVA) $(CGENERATED)
zip:
	-del *.class *.obj *.exe *.dll *.pdb *.pyc $(XLANGCS) $(XLANGJAVA) $(CGENERATED)
	zip SinkWorld.zip makefile *.py *.h *.cxx *.cpp *.hpp *.cs *.java
