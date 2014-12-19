// SinkWorld LexASP class.
// Copyright 2004 Neil Hodgson
// File written in C~ subset of C++ for automatic translation to Java and C#.

#include <string.h>
#include <assert.h>
#include <string>

#include "base.h"
#include "lexers.h"

void LexASP::ColourisePiece(Styler *sc) {
	// Possibly exit current state to HTML base state
	if (sc->Match('%', '>')) {
		if (sc->state == (ASPAt + baseState)) {
			Literal *t = sc->GetCurrentLowered();
			int lexNum = FindSubLexer(t->Data(), t->Length());
			if (lexNum >= 0) {
				defaultLexer->Append(sc->startSeg, lexNum);
			}
			sc->SetState(ASP + baseState);
			sc->Forward();
			if (lexerHTML != NULL) {
				sc->ForwardSetState(lexerHTML->GetBaseState());
			} else {
				// No HTML lexer, so go one after this lexer's states
				sc->ForwardSetState(thisStates + baseState);
			}
			delete t;
		} else {
			//SubLexer *sl = slScriptsRoot;
			//while (sl != NULL) {
			//	if (sl->lexer->InStateRange(sc->state)) {
					sc->SetState(ASP + baseState);
					sc->Forward();
					if (lexerHTML != NULL) {
						sc->ForwardSetState(lexerHTML->GetBaseState());
					} else {
						// No HTML lexer, so go one after this lexer's states
						sc->ForwardSetState(thisStates + baseState);
					}
			//	}
			//	sl = sl->slNext;
			//}
		}
	}

	// Handle some ASP script
	if ((lexerHTML != NULL) && (lexerHTML->InStateRange(sc->state))) {
		lexerHTML->ColourisePiece(sc);
	} else {
		ColourisePieceSubLexer(sc);
	}

	// Enter new sc->state
	if ((lexerHTML == NULL) || (!lexerHTML->InStateRange(sc->state)) || (lexerHTML->CanBeInterrupted(sc->state))) {
		if (sc->Match('<', '%')) {
			//if (sc->state == SCE_H_TAGUNKNOWN)
			//	sc->ChangeState(SCE_H_ASP);
			//else
				sc->SetState(ASP + baseState);
			sc->Forward();
			sc->Forward();
			if (sc->ch == '@') {
				sc->ForwardSetState(ASPAt + baseState);
			} else {
				if (sc->ch == '=') {
					sc->Forward();
				}
				SwitchToSubLexer(sc, defaultLexer->Value());
			}
		}
	}
}

void LexASP::Colourise(Styler *sc) {
	InvalidateAfter(sc->startSeg);

	for (; sc->More(); sc->Forward()) {
		ColourisePiece(sc);
	}

	sc->Complete();
}

