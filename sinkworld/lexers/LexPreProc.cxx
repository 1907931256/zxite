// SinkWorld LexPreProc class.
// Copyright 2004 Neil Hodgson
// File written in C~ subset of C++ for automatic translation to Java and C#.

#include <string.h>
#include <assert.h>
#include <string>

#include "base.h"
#include "lexers.h"

int LexPreProc::EvaluateExpression(Styler *sc, Literal *expr) {
	return 0;
}

bool LexPreProc::EvaluateConditional(Styler *sc) {
	return 1==1;
}

void LexPreProc::ColourisePiece(Styler *sc) {
}

void LexPreProc::Colourise(Styler *sc) {
}
