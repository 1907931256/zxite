// SinkWorld LinePositionCache class.
// Copyright 2004 Neil Hodgson
// File written in C~ subset of C++ for automatic translation to Java and C#.

#include <string.h>
#include <assert.h>
#include <stdio.h>
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

LinePositionCache::LinePositionCache() {
	/// \todo Allow changing the size.
	/// 256 is used as this allows all of a reasonable sized window to be cached.
	size = 256;
	pces = new LineCacheEntry*[size];
	for (int i=0;i<size;i++) {
		pces[i] = NULL;
	}
}

LinePositionCache::~LinePositionCache() {
	for (int i=0;i<size;i++) {
		delete pces[i];
		pces[i] = NULL;
	}
	delete []pces;
}

void LinePositionCache::Clear() {
	for (int i=0;i<size;i++) {
		delete pces[i];
		pces[i] = NULL;
	}
}

TextPositions *LinePositionCache::Find(int line) {
	int probe = line % size;
	if ((pces[probe] != NULL) && pces[probe]->Match(line)) {
		return pces[probe]->positions;
	} else {
		return NULL;
	}
}

void LinePositionCache::Set(int line, TextPositions *positions) {
	int probe = line % size;
	if (pces[probe] == NULL)
		pces[probe] = new LineCacheEntry();
	pces[probe]->Set(line, positions);
}
