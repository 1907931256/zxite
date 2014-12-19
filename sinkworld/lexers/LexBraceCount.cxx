// SinkWorld LexBraceCount class.
// Copyright 2004 Neil Hodgson
// File written in C~ subset of C++ for automatic translation to Java and C#.

#include <string.h>
#include <assert.h>
#include <string>

#include "base.h"
#include "lexers.h"

void LexBraceCount::ColourisePiece(Styler *sc) {

	int currentLevel = sc->state - baseState;
	if (insideBrace) {
		while (isStartBrace(sc->ch) && 
			((stylesBase == NULL) || (stylesBase->ValueAt(sc->Position()) == matchStyle))) {
			currentLevel = currentLevel + 1;
			if (currentLevel > Negative)
				currentLevel = Default;
			sc->ForwardSetState(currentLevel + baseState);
		}
		if (isEndBrace(sc->ch) && 
			((stylesBase == NULL) || (stylesBase->ValueAt(sc->Position()) == matchStyle))) {
			currentLevel = currentLevel - 1;
			if (currentLevel < Default)
				currentLevel = Negative;
			sc->SetState(currentLevel + baseState);
		}
	} else {
		while (isEndBrace(sc->ch) &&
			((stylesBase == NULL) || (stylesBase->ValueAt(sc->Position()) == matchStyle))) {
			currentLevel = currentLevel - 1;
			if (currentLevel < Default)
				currentLevel = Negative;
			sc->ForwardSetState(currentLevel + baseState);
		}
		if (isStartBrace(sc->ch) &&
			((stylesBase == NULL) || (stylesBase->ValueAt(sc->Position()) == matchStyle))) {
			currentLevel = currentLevel + 1;
			if (currentLevel > Negative)
				currentLevel = Default;
			sc->SetState(currentLevel + baseState);
		}
	}
}

void LexBraceCount::Colourise(Styler *sc) {
	InvalidateAfter(sc->startSeg);

	for (; sc->More(); sc->Forward()) {
		ColourisePiece(sc);
	}
	sc->Complete();
}
