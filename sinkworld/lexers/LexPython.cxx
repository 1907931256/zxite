// SinkWorld LexPython class.
// Copyright 2004 Neil Hodgson
// File written in C~ subset of C++ for automatic translation to Java and C#.

#include <string.h>
#include <assert.h>
#include <string>

#include "base.h"
#include "lexers.h"

void LexPython::ColourisePiece(Styler *sc) {
	if (sc->atLineEnd) {
		if ((sc->state == Default + baseState) ||
			(sc->state == Triple + baseState) ||
			(sc->state == TripleDouble + baseState)) {
			// Perform colourisation of white space and triple quoted strings at end of each line to allow
			// tab marking to work inside white space and triple quoted strings
			sc->ForwardSetState(sc->state);
		}
		if ((sc->state == DoubleQuoted + baseState) || (sc->state == Quoted + baseState)) {
			sc->ChangeState(StringEOL + baseState);
			sc->ForwardSetState(Default + baseState);
		}
		if (!sc->More())
			return;
	}

	// Check for a state end
	if (sc->state == Operator + baseState) {
		kwLast = kwOther;
		sc->SetState(Default + baseState);
	} else if (sc->state == Number + baseState) {
		if (!isWordChar(sc->ch) &&
			!(!hexadecimal && ((sc->ch == '+' || sc->ch == '-') && (sc->chPrev == 'e' || sc->chPrev == 'E')))) {
			sc->SetState(Default + baseState);
		}
	} else if (sc->state == Identifier + baseState) {
		if ((sc->ch == '.') || (!isWordChar(sc->ch))) {
			Literal *t = sc->GetCurrent();
			int style = Identifier + baseState;
			if ((kwLast == kwImport) && (tAs.Equals(t))) {
				style = KeyWord + baseState;
			} else if (keyWords->Find(t->Data(),0,t->Length()) != 0) {
				style = KeyWord + baseState;
			} else if (kwLast == kwClass) {
				style = ClassName + baseState;
			} else if (kwLast == kwDef) {
				style = DefName + baseState;
			}
			sc->ChangeState(style);
			sc->SetState(Default + baseState);
			if (style == KeyWord + baseState) {
				if (tClass.Equals(t))
					kwLast = kwClass;
				else if (tDef.Equals(t))
					kwLast = kwDef;
				else if (tImport.Equals(t))
					kwLast = kwImport;
				else
					kwLast = kwOther;
			} else if (style == ClassName + baseState) {
				kwLast = kwOther;
			} else if (style == DefName + baseState) {
				kwLast = kwOther;
			}
			delete t;
		}
	} else if ((sc->state == CommentLine + baseState) || (sc->state == CommentBlock + baseState)) {
		if (sc->ch == '\r' || sc->ch == '\n') {
			sc->SetState(Default + baseState);
		}
	} else if ((sc->state == DoubleQuoted + baseState) || (sc->state == Quoted + baseState)) {
		if (sc->ch == '\\') {
			if (sc->chNext == '\r') {
				sc->Forward();
				if (sc->chNext == '\n') {
					sc->Forward();
				}
			} else {
				sc->Forward();
			}
		} else if ((sc->state == DoubleQuoted + baseState) && (sc->ch == '\"')) {
			sc->ForwardSetState(Default + baseState);
		} else if ((sc->state == Quoted + baseState) && (sc->ch == '\'')) {
			sc->ForwardSetState(Default + baseState);
		}
	} else if (sc->state == Triple + baseState) {
		if (sc->ch == '\\') {
			sc->Forward();
		} else if (sc->Match(&tripleQuote)) {
			sc->Forward();
			sc->Forward();
			sc->ForwardSetState(Default + baseState);
		}
	} else if (sc->state == TripleDouble + baseState) {
		if (sc->ch == '\\') {
			sc->Forward();
		} else if (sc->Match(&tripleDouble)) {
			sc->Forward();
			sc->Forward();
			sc->ForwardSetState(Default + baseState);
		}
	}

	// Check for a new state starting character
	if (sc->state == Default + baseState) {
		if (Styler::IsDigit(sc->ch) || (sc->ch == '.' && Styler::IsDigit(sc->chNext))) {
			if (sc->ch == '0' && (sc->chNext == 'x' || sc->chNext == 'X')) {
				hexadecimal = true;
			} else {
				hexadecimal = false;
			}
			sc->SetState(Number + baseState);
		} else if (isOperator(sc->ch) || sc->ch == '`') {
			sc->SetState(Operator + baseState);
		} else if (sc->ch == '#') {
			sc->SetState(sc->chNext == '#' ? CommentBlock + baseState : CommentLine + baseState);
		} else if (isStringStart(sc->ch, sc->chNext, sc->GetRelative(2))) {
			sc->SetState(DoubleQuoted + baseState);
			if (sc->ch == 'u' || sc->ch == 'U') {
				sc->Forward();
			}
			if (sc->ch == 'r' || sc->ch == 'R') {
				sc->Forward();
			}
			if (sc->Match(&tripleQuote)) {
				sc->ChangeState(Triple + baseState);
				sc->Forward();
				sc->Forward();
			} else if (sc->Match(&tripleDouble)) {
				sc->ChangeState(TripleDouble + baseState);
				sc->Forward();
				sc->Forward();
			} else if (sc->ch == '\'') {
				sc->ChangeState(Quoted + baseState);
			} else {
			}
		} else if (isWordStart(sc->ch)) {
			sc->SetState(Identifier + baseState);
		}
	}
}

void LexPython::Colourise(Styler *sc) {
	InvalidateAfter(sc->startSeg);

	// Do not leak onto next line
	if (sc->state == StringEOL + baseState) {
		sc->SetState(Default + baseState);
	}

	for (; sc->More(); sc->Forward()) {
		ColourisePiece(sc);
	}
	sc->Complete();
}
