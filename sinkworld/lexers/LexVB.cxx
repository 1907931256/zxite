// SinkWorld LexVB class.
// Copyright 2004 Neil Hodgson
// File written in C~ subset of C++ for automatic translation to Java and C#.

#include <string.h>
#include <assert.h>
#include <string>

#include "base.h"
#include "lexers.h"

void LexVB::ColourisePiece(Styler *sc) {
	if (sc->state == Operator + baseState) {
		sc->SetState(Default + baseState);
	} else if (sc->state == KeyWord + baseState) {
		if (!isWordChar(sc->ch)) {
			if (vbScriptSyntax || !isTypeChar(sc->ch)) {
				if (sc->ch == ']')
					sc->Forward();
				Literal *t = sc->GetCurrentLowered();
				if (keyWords->Find(t->Data(), 0, t->Length()) != 0) {
					if (rem.Equals(t)) {
						sc->ChangeState(Comment + baseState);
						if (sc->atLineEnd) {
							sc->SetState(Default + baseState);
						}
					} else {
						sc->SetState(Default + baseState);
					}
				} else {
					sc->ChangeState(Identifier + baseState);
					sc->SetState(Default + baseState);
				}
				delete t;
			}
		}
	} else if (sc->state == Number + baseState) {
		if (!isWordChar(sc->ch)) {
			sc->SetState(Default + baseState);
		}
	} else if (sc->state == Quoted + baseState) {
		// VB doubles quotes to preserve them, so just end this string
		// state now as a following quote will start again
		if (sc->ch == '\"') {
			if ((sc->chNext == 'c') || (sc->chNext == 'C')) {
				sc->Forward();
			}
			sc->ForwardSetState(Default + baseState);
		}
	} else if (sc->state == Comment + baseState) {
		if (sc->atLineEnd) {
			sc->SetState(Default + baseState);
		}
	} else if (sc->state == Preprocessor + baseState) {
		if (sc->atLineEnd) {
			sc->SetState(Default + baseState);
		}
	} else if (sc->state == Date + baseState) {
		if (sc->ch == '#' || !(isDateChar(sc->chNext) || sc->chNext=='#')) {
			sc->ForwardSetState(Default + baseState);
		}
	}

	if (sc->state == Default + baseState) {
		if (sc->ch == '\'') {
			sc->SetState(Comment + baseState);
		} else if (sc->ch == '\"') {
			sc->SetState(Quoted + baseState);
		} else if (sc->ch == '#' && visibleChars == 0) {
			// Preprocessor commands are alone on their line
			sc->SetState(Preprocessor + baseState);
		} else if (sc->ch == '#') {
			int n = 1;
			int chSeek = ' ';
			while (chSeek == ' ' || chSeek == '\t') {
				/// \todo This is of quadratic complexity so use iterator over document
				chSeek = sc->GetRelative(n);
				n++;
			}
			if (Styler::IsDigit(chSeek)) {
				sc->SetState(Date + baseState);
			} else {
				sc->SetState(Operator + baseState);
			}
		} else if (sc->ch == '&' && ((sc->chNext == 'h') || (sc->chNext == 'H'))) {
			sc->SetState(Number + baseState);
		} else if (sc->ch == '&' && ((sc->chNext == 'o') || (sc->chNext == 'O'))) {
			sc->SetState(Number + baseState);
		} else if (Styler::IsDigit(sc->ch) || (sc->ch == '.' && Styler::IsDigit(sc->chNext))) {
			sc->SetState(Number + baseState);
		} else if (isWordStart(sc->ch) || (sc->ch == '[')) {
			sc->SetState(KeyWord + baseState);
		} else if (isOperator(sc->ch) || (sc->ch == '\\')) {
			sc->SetState(Operator + baseState);
		}
	}

	if (sc->atLineEnd) {
		visibleChars = 0;
	}
	if (!Styler::IsSpaceChar(sc->ch)) {
		visibleChars++;
	}
}

void LexVB::Colourise(Styler *sc) {
	InvalidateAfter(sc->startSeg);

	for (; sc->More(); sc->Forward()) {
		ColourisePiece(sc);
	}
	sc->Complete();
}
