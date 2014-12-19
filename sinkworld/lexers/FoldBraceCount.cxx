// SinkWorld LexBraceCount class.
// Copyright 2004 Neil Hodgson
// File written in C~ subset of C++ for automatic translation to Java and C#.

#include <string.h>
#include <assert.h>
#include <string>

#include "base.h"
#include "lexers.h"

void FoldBraceCount::ColourisePiece(Styler *sc) {
	while (isEndBrace(sc->ch) &&
		((stylesBase == NULL) || (stylesBase->ValueAt(sc->Position()) == matchStyle))) {
		sc->ChangeFoldLevel(-1);
		sc->Forward();
	}
	if (isStartBrace(sc->ch) &&
		((stylesBase == NULL) || (stylesBase->ValueAt(sc->Position()) == matchStyle))) {
		sc->ChangeFoldLevel(1);
	}
}

void FoldBraceCount::Colourise(Styler *sc) {
	InvalidateAfter(sc->startSeg);

	for (; sc->More(); sc->Forward()) {
		ColourisePiece(sc);
	}
	sc->Complete();
}
