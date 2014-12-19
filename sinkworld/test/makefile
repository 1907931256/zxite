# makefile for SinkWorld java

.SUFFIXES: .java

JAVA=java
#JAVAC=javac
JAVAC=H:\jdk\bin\javac
PYTHON=python

all: mj.class SimpleTest.class

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

vpath %.java ../java

JSRCS=mj.java SWTest.java BadPositionException.java $(XLANGJAVA)
mj.class: $(JSRCS)
	$(JAVAC) -source 1.4 -O $(JSRCS)

JST=SimpleTest.java BadPositionException.java $(XLANGJAVA)
SimpleTest.class: $(JST)
	$(JAVAC) -classpath c:\junit\junit.jar -source 1.4 -O $(JST)

clean:
	del *.class
