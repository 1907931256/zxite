// SinkWorld ChangePositionsCache class.
// Copyright 2004 Neil Hodgson
// File written in C~ subset of C++ for automatic translation to Java and C#.

#include <assert.h>
#include <string.h>
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

ChangePositionsCache::ChangePositionsCache() {
	/// \todo Allow changing the size.
	/// 256 is used as this allows all of a reasonable sized window to be cached.
	size = 256;
	pces = new ChangePositionsEntry*[size];
	for (int i=0;i<size;i++) {
		pces[i] = NULL;
	}
}

ChangePositionsCache::~ChangePositionsCache() {
	for (int i=0;i<size;i++) {
		delete pces[i];
		pces[i] = NULL;
	}
	delete []pces;
}

void ChangePositionsCache::Clear() {
	for (int i=0;i<size;i++) {
		delete pces[i];
		pces[i] = NULL;
	}
}

SetIntegers *ChangePositionsCache::Find(int line) {
	int probe = line % size;
	if ((pces[probe] != NULL) && pces[probe]->Match(line)) {
		return pces[probe]->cf;
	} else {
		return NULL;
	}
}

void ChangePositionsCache::Set(int line, SetIntegers *cf) {
	int probe = line % size;
	if (pces[probe] == NULL)
		pces[probe] = new ChangePositionsEntry();
	pces[probe]->Set(line, cf);
}
