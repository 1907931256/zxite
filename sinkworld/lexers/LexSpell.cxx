// SinkWorld LexSpell class.
// Copyright 2004 Neil Hodgson
// File written in C~ subset of C++ for automatic translation to Java and C#.

#include <string.h>
#include <assert.h>
#include <string>

#include "base.h"
#include "lexers.h"

void LexSpell::ColourisePiece(Styler *sc) {
	// Determine if the current state should terminate.
	switch (sc->state - baseState) {
		case Word:
			if (!isWordChar(sc->ch)) {
				Literal *t = sc->GetCurrentLowered();
				if (keyWords->Find(t->Data(), 0, t->Length()) == 0) {
					sc->ChangeState(Mistake + baseState);
				}
				sc->SetState(Default + baseState);
				delete t;
			}
			break;
	}

	// Determine if a new state should be entered.
	if (sc->state == Default + baseState) {
		if (isWordStart(sc->ch)) {
			sc->SetState(Word + baseState);
		}
	}
}

void LexSpell::Colourise(Styler *sc) {
	InvalidateAfter(sc->startSeg);

	for (; sc->More(); sc->Forward()) {
		ColourisePiece(sc);
	}
	sc->Complete();
}
