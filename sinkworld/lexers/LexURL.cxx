// SinkWorld LexURL class.
// Copyright 2004 Neil Hodgson
// File written in C~ subset of C++ for automatic translation to Java and C#.

#include <string.h>
#include <assert.h>
#include <string>

#include "base.h"
#include "lexers.h"

void LexURL::ColourisePiece(Styler *sc) {
	// Determine if the current state should terminate.
	switch (sc->state - baseState) {
		case URL:
			if (!isURLChar(sc->ch)) {
				Literal *t = sc->GetCurrent();
				bool isURL = false;
				for (int i=1; i<t->Length()-2; i++) {
					// Look for "://"
					if ((t->CharAt(i) == ':') && (t->CharAt(i+1) == '/') && (t->CharAt(i+2) == '/')) {
						isURL = true;
					}
				}
				if (!isURL) {
					sc->ChangeState(NotURL + baseState);
				}
				sc->SetState(Default + baseState);
				delete t;
			}
			break;
	}

	// Determine if a new state should be entered.
	if (sc->state == Default + baseState) {
		if (isURLStart(sc->ch)) {
			sc->SetState(URL + baseState);
		}
	}
}

void LexURL::Colourise(Styler *sc) {
	InvalidateAfter(sc->startSeg);

	for (; sc->More(); sc->Forward()) {
		ColourisePiece(sc);
	}
	sc->Complete();
}
