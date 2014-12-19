// SinkWorld PositionCache class.
// Copyright 2004 Neil Hodgson
// File written in C~ subset of C++ for automatic translation to Java and C#.

#include <string.h>
#include <stdio.h>
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

PositionCache::PositionCache() {
	// Size must be a power of 2 so that the hash can be 
	// converted into an index easily by using and.
	size = 0x400;
	mask = size - 1;
	clock = 0;
	pces = new PositionCacheEntry *[size];
	for (int i=0;i<size;i++)
		pces[i] = new PositionCacheEntry();
}

PositionCache::~PositionCache() {
	for (int i=0;i<size;i++) {
		delete pces[i];
		pces[i] = NULL;
	}
	delete []pces;
}

void PositionCache::MeasureStyledText(Surface *surface, const FontDescription &fd, SplitText *s, int position, int len, TextPositions *positions) {
	int hashFull = s->HashRange(position, len) ^ fd.Hash();
	int probe = hashFull & mask;
	// First try the primary probe
	if (pces[probe]->Match(fd, s, position, len)) {
		positions->Append(pces[probe]->positions);
	} else {
		// Then the secondary probe
		int probe2 = (hashFull * 37) & mask;
		if (pces[probe2]->Match(fd, s, position, len)) {
			positions->Append(pces[probe2]->positions);
		} else {
			// Item not in cache so dislodge the oldest entry at one of the probe locations
			if (pces[probe2]->clock < pces[probe]->clock) {
				probe = probe2;
			}
			clock++;
			surface->SetFont(fd);
			TextPositions *positionsSegment = new TextPositions(len+1);
			surface->MeasureWidths(s, position, len, positionsSegment);
			pces[probe]->Set(fd, s, position, len, positionsSegment,clock);
			positions->Append(positionsSegment);
		}
	}
}
