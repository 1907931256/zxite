// SinkWorld main header.
// Copyright 2001-2004 Neil Hodgson
// File should not be translated to Java and C#.

/// Include all the basic SinkWorld headers.
/// Before including this, string, string.h and assert.h should have been included.

#define SW_CXX 1

#define SW_BYTE unsigned char
#define SW_SHORT short
#define SW_OBJECT void *

#define SW_ABSTRACT
#define SW_OVERRIDE
#define SW_PUBLIC

#define SW_ARRAYCOPY(srcA, srcIndex, destA, destIndex, length) \
	memmove((destA) + (destIndex), (srcA) + (srcIndex), \
		sizeof(srcA[0]) *(length));

#include "UniConversion.h"
#include "BadPositionException.h"
#include "CharacterFragmentException.h"
#include "ConfigurationException.h"
#include "IStyles.h"
#include "SinglePosition.h"
#include "SplitVector_0.h"
#include "SplitVector_1.h"
#include "SplitVector_2.h"
#include "SplitVector_4.h"
#include "SimpleIntegerVector.h"
#include "SetIntegers.h"
#include "MaxFinder.h"
#include "Partitioning.h"
#include "UndoAction.h"
#include "UndoStack.h"
#include "LineData.h"
#include "RunStyles.h"
#include "Substance.h"
#include "Literal.h"
#include "History.h"
#include "HistorySet.h"
#include "SplitText.h"
#include "KeyWords.h"
#include "WordList.h"
#include "Styler.h"
#include "IPrototype.h"
#include "ILexer.h"
#include "SymbolTable.h"
#include "Dictionary.h"
#include "OptionSet.h"
#include "LiteralSet.h"
#include "LexBase.h"
#include "SubLexer.h"
#include "LexScript.h"
#include "LexNull.h"
#include "FoldBase.h"
#include "TextPositions.h"
#include "ByteBuffer.h"
#include "PrototypeRegistry.h"
