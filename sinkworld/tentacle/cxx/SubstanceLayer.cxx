// SinkWorld SubstanceLayer class.
// Copyright 2004 Neil Hodgson
// File written in C~ subset of C++ for automatic translation to Java and C#.

#include <string.h>
#include <assert.h>
#include <string>

#include "base.h"

#include "RGBColor.h"
#include "IDecoration.h"
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

void SubstanceLayer::FindNextChange(int end) {
	if (position < end) {
		int positionCurrent = position;
		position += psubstance->LenChar(position);
		if (IsControlCharacter(psubstance->CharValue(positionCurrent))) {
			return;
		}
		while (position < end) {
			if (IsControlCharacter(psubstance->CharValue(position))) {
				return;
			}
			position += psubstance->LenChar(position);
		}
	}
}
