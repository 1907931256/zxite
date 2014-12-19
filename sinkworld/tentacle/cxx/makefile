.SUFFIXES: .cxx .o .h .a .c

vpath %.h ../../base;../../lexers
vpath %.cxx ../../base:../../lexers

tentacle: \
Tentacle.o \
TentacleControl.o \
BaseDecoration.o \
ChangeFinder.o \
ChangePositionsCache.o \
DecorationBox.o \
DecorationButton.o \
DecorationCaret.o \
DecorationGraduatedBox.o \
DecorationRoundedBox.o \
DecorationStrikeOut.o \
DecorationUnderLine.o \
Dictionary.o \
Document.o \
FoldBraceCount.o \
History.o \
KeyWords.o \
LexASP.o \
LexBraceCount.o \
LexCPP.o \
LexHTML.o \
LexPHP.o \
LexPreProc.o \
LexPython.o \
LexSpell.o \
LexURL.o \
LexVB.o \
LinePositionCache.o \
Literal.o \
OptionSet.o \
MaxFinder.o \
PhasedPaint.o \
PositionCache.o \
PrototypeRegistry.o \
RegisteredDecorations.o \
RegisteredLexers.o \
RunStyles.o \
SinglePosition.o \
SplitText.o \
SplitVector_0.o \
SplitVector_1.o \
SplitVector_2.o \
SplitVector_4.o \
Substance.o \
SubstanceLayer.o \
StyleModification.o \
Surface.o \
TextView.o \
UniConversion.o \
WordList.o
	g++ -g `pkg-config --libs gtk+-2.0` $^ -o tentacle

.cxx.o:
	g++ -g -I ../../base -I ../../lexers -DNDEBUG `pkg-config --cflags gtk+-2.0` -c $<

clean:
	rm *.o tentacle
	
