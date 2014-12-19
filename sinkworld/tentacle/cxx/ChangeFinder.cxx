// SinkWorld ChangeFinder class.
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

void ChangeFinder::InsertSorted(ChangeLayer *pcl) {
	assert(pcl->next == NULL);
	ChangeLayer *prevP = NULL;
	ChangeLayer *p = root;
	while ((p != NULL) && (p->position < pcl->position)) {
		prevP = p;
		p = p->next;
	}
	if (prevP != NULL) {
		pcl->next = p;
		prevP->next = pcl;
	} else {
		pcl->next = root;
		root = pcl;
	}
}

void ChangeFinder::RemoveLayers() {
	ChangeLayer *pcl = root;
	while (pcl != NULL) {
		ChangeLayer *pclNext = pcl->next;
		delete pcl;
		pcl = pclNext;
	}
	root = NULL;
}

ChangeFinder::ChangeFinder(SplitText *pbuffer_, int position_, int end_, StyleSetCollection *ssc_, SetIntegers *cp) {
	pbuffer = pbuffer_;
	position = position_;
	end = end_;
	root = NULL;
	int layers = ssc_->Length();
	// Add all the style layers
	for (int layer=0; layer<layers; layer++) {
		IStyles *player = ssc_->GetLayer(layer);
		if (player != NULL) {
			InsertSorted(new StyleLayer(position, layer, player, end));
		}
	}

	// Add the substance layer
	Substance *psubstance = pbuffer->GetSubstance();
	InsertSorted(new SubstanceLayer(position, -1, psubstance, end));

	// Find all the changes
	int oB = position_;
	while (oB<end_) {
		oB = FindNextChange();
		cp->Add(oB);
	}
	RemoveLayers();
}

int ChangeFinder::FindNextChange() {
	int positionChange = root->position;
	while ((root->position < end) && (root->position <= positionChange)) {
		root->FindNextChange(end);
		if (root->next != NULL) {
			if (root->position > root->next->position) {
				ChangeLayer *pcl = root;
				root = pcl->next;
				pcl->next = NULL;
				InsertSorted(pcl);
			}
		}
	}
	return positionChange;
}
