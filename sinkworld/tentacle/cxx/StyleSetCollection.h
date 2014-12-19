// SinkWorld StyleSetCollection class.
// Copyright 2004 Neil Hodgson
// File written in C~ subset of C++ for automatic translation to Java and C#.

/// A collection of StyleSet objects responsible for the layered styling of a buffer.

SW_PUBLIC class StyleSetCollection {
	/// Must not be used to decrease the size of the buffer.
	void ReAllocate(int newSize) {
		if (newSize == 0) {
			newSize = 1;
		}
		StyleSet **newStyleSets = new StyleSet*[newSize];
		for (int i=0; i<newSize; i++) {
			newStyleSets[i] = NULL;
			if ((styleSets != NULL) && (i < len)) {
				newStyleSets[i] = styleSets[i];
			}
		}
		delete []styleSets;
		styleSets = newStyleSets;
		len = newSize;
	}
	int len;
	StyleSet **styleSets;
public:
	bool isView;	
	Literal *fontNameDefault;
	StyleSetCollection(int dim, Literal *fontName) {
		isView = false;
		len = 0;
		styleSets = NULL;
		fontNameDefault = fontName;
		ReAllocate(dim);
	}
	~StyleSetCollection() {
		if (!isView) {
			for (int i=0;i<len;i++) {
				delete styleSets[i];
				styleSets[i] = NULL;
			}
		}
		delete []styleSets;
		styleSets = NULL;
	}
	StyleSet *Get(int i) {
		return styleSets[i];
	}
	StyleSet *GetMainStyleSet() {
		return styleSets[0];
	}
	void ModifySet(int set, int position, int flagsModify, Style &sty) {
		// For each style set
		StyleSet *ss = styleSets[set];
		if (ss != NULL) {
			if (position < ss->buffer->Length()) {
				int styleNext = ss->buffer->ValueAt(position);
				StyleModification *s = ss->Get(styleNext);
				if (s != NULL)
					s->Modify(sty, flagsModify);
			}
		}
	}
	void Modify(int position, int flagsModify, Style &sty) {
		for (int i=0;i<len;i++) {
			// For each style set
			StyleSet *ss = styleSets[i];
			if (ss != NULL) {
				if (position < ss->buffer->Length()) {
					int styleNext = ss->buffer->ValueAt(position);
					StyleModification *s = ss->Get(styleNext);
					if (s != NULL)
						s->Modify(sty, flagsModify);
				}
			}
		}
	}
	void ModifyToFirst(int flagsModify, Style &sty) {
		for (int i=0;i<len;i++) {
			// For each style set
			StyleSet *ss = styleSets[i];
			if (ss != NULL) {
				StyleModification *s = ss->Get(0);
				if (s != NULL)
					s->Modify(sty, flagsModify);
			}
		}
	}
	void Add(StyleSet *ss) {
		for (int i=0;i<len;i++) {
			if (styleSets[i] == NULL) {
				styleSets[i] = ss;
				return;
			}
		}
		int addPosition = len;
		ReAllocate(len*2);
		styleSets[addPosition] = ss;
	}
	void Remove(StyleSet *ss) {
		for (int i=0;i<len;i++) {
			if (styleSets[i] == ss) {
				styleSets[i] = NULL;
				return;
			}
		}
	}
	int Length() {
		return len;
	}
	IStyles *GetLayer(int layer) {
		if (styleSets[layer] != NULL)
			return styleSets[layer]->buffer;
		else
			return NULL;
	}
	void SetSpecifiedFlags() {
		for (int i=0;i<len;i++) {
			// For each style set
			if (styleSets[i] != NULL) {
				styleSets[i]->SetSpecifiedFlags();
			}
		}
	}
	// Return a subset of the StyleSets that modify a set of flags
	StyleSetCollection *View(int flagsModify) {
		SetSpecifiedFlags();
		int newLen = 0;
		for (int i=0;i<len;i++) {
			StyleSet *ss = styleSets[i];
			if ((ss != NULL) && ((ss->SpecifiedFlags() & flagsModify) != 0)) {
				newLen++;
			}
		}
		StyleSetCollection *sscView = new StyleSetCollection(newLen, fontNameDefault);
		sscView->isView = true;
		for (int j=0;j<len;j++) {
			StyleSet *ss = styleSets[j];
			if ((ss != NULL) && ((ss->SpecifiedFlags() & flagsModify) != 0)) {
				sscView->Add(ss);
			}
		}
		return sscView;
	}
};
