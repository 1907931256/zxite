// Boost Includes ==============================================================
#include <boost/python.hpp>
#include <boost/cstdint.hpp>

// Includes ====================================================================

#include <string>
#include <iostream>
#include <assert.h>
#include "base.h"
#include "lexers.h"

typedef void *HWND;
typedef void *HINSTANCE;

#include "PhysicalLayout.h"

#include "RegisteredLexers.h"
#include "RGBColor.h"
#include "IDecoration.h"
#include "BaseDecoration.h"
#include "RegisteredDecorations.h"
#include "Style.h"
#include "StyleModification.h"
#include "StyleSet.h"
#include "StyleSetCollection.h"
#include "FontDescription.h"
#include "Surface.h"
#include "ChangeLayer.h"
#include "ChangeFinder.h"
#include "StyleLayer.h"
#include "SubstanceLayer.h"
#include "PositionCache.h"
#include "LinePositionCache.h"
#include "ChangePositionsCache.h"
#include "Document.h"

#include "PhasedPaint.h"
#include "TextView.h"
#include "RangeSelection.h"

#include "DecorationBox.h"
#include "DecorationButton.h"
#include "DecorationCaret.h"
#include "DecorationGraduatedBox.h"
#include "DecorationStrikeOut.h"
#include "DecorationRoundedBox.h"
#include "DecorationUnderLine.h"

#include "TentacleControl.h"

// Using =======================================================================
using namespace boost::python;

#include "SWWrappers.h"

void add_UniConversion();
void add_BadPositionException();
void add_CharacterFragmentException();
void add_IStyles();
void add_SinglePosition();
void add_SplitVector_0();
void add_SplitVector_1();
void add_SplitVector_2();
void add_SplitVector_4();
void add_SimpleIntegerVector();
void add_SetIntegers();
void add_MaxFinder();
void add_Partitioning();
void add_UndoAction();
void add_UndoStack();
void add_LineData();
void add_RunStyles();
void add_Substance();
void add_SubstanceUTF8();
void add_SubstanceByte();
void add_SubstanceUTF16();
void add_SubstanceUTF16LE();
void add_SubstanceUTF16BE();
void add_SplitText();
void add_Literal();
void add_KeyWords();
void add_WordList();
void add_Styler();
void add_IPrototype();
void add_ILexer();
void add_History();
void add_SymbolTable();
void add_OptionSet();
void add_OptionSet();
void add_LiteralSet();
void add_LexBase();
void add_LexScript();
void add_TextPositions();
void add_ByteBuffer();

void add_ChangePositionsEntry();
void add_SubMode();
void add_Layer();
void add_PaintContext();
void add_LineCacheEntry();
void add_PaintParameters();
void add_IPaintPhase();
void add_PhaseBack();
void add_PhaseText();
void add_PhaseDecoration();
void add_PositionCacheEntry();
void add_Point();
void add_PaintContext();
void add_IDocumentChangeListener();
void add_TextColumn();
void add_TentacleControl();
void add_ChangeFinder();
void add_ChangeLayer();
void add_ChangePositionsCache();
void add_BaseDecoration();
void add_DecorationBox();
void add_DecorationButton();
void add_DecorationCaret();
void add_DecorationGraduatedBox();
void add_DecorationStrikeOut();
void add_DecorationRoundedBox();
void add_DecorationUnderLine();
void add_Document();
void add_PhysicalLayout();
void add_FontDescription();
void add_IDecoration();
void add_LinePositionCache();
void add_PositionCache();
void add_PrototypeRegistry();
void add_RegisteredDecorations();
void add_RegisteredLexers();
void add_RGBColor();
void add_RangeSelection();
void add_Style();
void add_StyleLayer();
void add_StyleModification();
void add_StyleSetCollection();
void add_StyleSet();
void add_SubstanceLayer();
void add_Surface();
void add_TextView();

// Module ======================================================================
BOOST_PYTHON_MODULE(SinkWorld)
{
	add_UniConversion();
	add_BadPositionException();
	add_CharacterFragmentException();
	add_IStyles();
	add_SinglePosition();
	add_SplitVector_0();
	add_SplitVector_1();
	add_SplitVector_2();
	add_SplitVector_4();
	add_SimpleIntegerVector();
	add_SetIntegers();
	add_MaxFinder();
	add_Partitioning();
	add_UndoAction();
	add_UndoStack();
	add_LineData();
	add_RunStyles();
	add_Substance();
	add_SubstanceUTF8();
	add_SubstanceByte();
	add_SubstanceUTF16();
	add_SubstanceUTF16LE();
	add_SubstanceUTF16BE();
	add_SplitText();
	add_Literal();
	add_KeyWords();
	add_WordList();
	add_Styler();
	add_IPrototype();
	add_ILexer();
	add_History();
	add_SymbolTable();
	add_OptionSet();
	add_OptionSet();
	add_LiteralSet();
	add_LexBase();
	add_LexScript();
	add_TextPositions();
	add_ByteBuffer();

	add_ChangePositionsEntry();
	add_SubMode();
	add_Layer();
	add_PaintContext();
	add_LineCacheEntry();
	add_PaintParameters();
	add_IPaintPhase();
	add_PhaseBack();
	add_PhaseText();
	add_PhaseDecoration();
	add_PositionCacheEntry();
	add_Point();
	add_PaintContext();
	add_IDocumentChangeListener();
	add_TextColumn();
	add_TentacleControl();
	add_ChangeFinder();
	add_ChangeLayer();
	add_ChangePositionsCache();
	add_IDecoration();
	add_BaseDecoration();
	add_DecorationBox();
	add_DecorationButton();
	add_DecorationCaret();
	add_DecorationGraduatedBox();
	add_DecorationStrikeOut();
	add_DecorationRoundedBox();
	add_DecorationUnderLine();
	add_Document();
	add_PhysicalLayout();
	add_FontDescription();
	add_LinePositionCache();
	add_PositionCache();
	add_PrototypeRegistry();
	add_RegisteredDecorations();
	add_RegisteredLexers();
	add_RGBColor();
	add_RangeSelection();
	add_Style();
	add_StyleLayer();
	add_StyleModification();
	add_StyleSetCollection();
	add_StyleSet();
	add_SubstanceLayer();
	add_Surface();
	add_TextView();
}

