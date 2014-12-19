// SinkWorld LexHTML class.
// Copyright 2004 Neil Hodgson
// File written in C~ subset of C++ for automatic translation to Java and C#.

#include <string.h>
#include <assert.h>
#include <string>

#include "base.h"
#include "lexers.h"

void LexHTML::ColourisePiece(Styler *sc) {
	SubLexer *sl = slScriptsRoot;
	while (sl != NULL) {
		if (sl->lexer->InStateRange(sc->state)) {
			sl->lexer->ColourisePiece(sc);
			if (sl->lexer->CanBeInterrupted(sc->state) && sc->Match('<', '/')) {
				sc->SetState(TagUnknown + baseState);
				defaultLexer->AppendIfChanged(sc->startSeg, -1);
				return;
			}
			break;
		}
		sl = sl->slNext;
	}
	if (sc->state == Script + baseState) {
		if (sc->Match('<', '/')) {
			sc->SetState(TagUnknown + baseState);
			defaultLexer->AppendIfChanged(sc->startSeg, -1);
			return;
		}
	}
	if (sc->state == Comment + baseState) {
		if (sc->Match(&commentEnd)) {
			sc->Forward();
			sc->Forward();
			sc->ForwardSetState(Default + baseState);
		}
	} else if (sc->state == Entity + baseState) {
		if (sc->ch == ';') {
			sc->ForwardSetState(Default + baseState);
		} else if (sc->ch != '#' && (sc->ch < 0x80) && !Styler::IsAlphaNumeric(sc->ch)) {
			// Should check that '#' follows '&', but it is unlikely anyway...
			sc->ChangeState(TagUnknown + baseState);
			sc->SetState(Default + baseState);
		}
	} else if (sc->state == TagUnknown + baseState) {
		if (!isHtmlWordChar(sc->ch) && !((sc->ch == '/') && (sc->chPrev == '<')) && sc->ch != '[') {
			Literal *t = sc->GetCurrentLowered();
			if ((t->Length() > 1) && (t->CharAt(1) == '/')) {
				if (keyWordsTags->Find(t->Data(), 2, t->Length()) != 0) {
					sc->ChangeState(Tag + baseState);
				}
			} else {
				if (keyWordsTags->Find(t->Data(), 1, t->Length()) != 0) {
					sc->ChangeState(Tag + baseState);
					if (scriptTag.Equals(t)) {
						defaultLexer->Append(sc->startSeg, 0);
					}
				}
			}
			if (sc->ch == '>') {
				EndTagCheckScripting(sc);
			} else if (sc->Match('/', '>')) {
				sc->SetState(TagEnd + baseState);
				sc->Forward();
				sc->ForwardSetState(Default + baseState);
				defaultLexer->AppendIfChanged(sc->startSeg, -1);
			} else {
				if (sc->state != TagUnknown + baseState) {
					sc->SetState(Other + baseState);
				}
			}
			delete t;
		}
	} else if (sc->state == Attribute + baseState) {
		if (!isHtmlWordChar(sc->ch)) {
			Literal *t = sc->GetCurrentLowered();
			if (keyWordsAttributes->Find(t->Data(), 0, t->Length()) == 0) {
				sc->ChangeState(AttributeUnknown + baseState);
			}
			CheckForLexerName(sc);
			if (sc->ch == '>') {
				EndTagCheckScripting(sc);
			} else if (sc->ch == '=') {
				sc->SetState(Other + baseState);
				sc->ForwardSetState(Value + baseState);
			} else {
				sc->SetState(Other + baseState);
			}
			delete t;
		}
	} else if (sc->state == Other + baseState) {
		if (sc->ch == '>') {
			EndTagCheckScripting(sc);
		} else if (sc->Match('?', '>')) {
			sc->SetState(XMLEnd + baseState);
		} else if (sc->Match('/', '>')) {
			sc->SetState(Tag + baseState);
			sc->Forward();
			sc->ForwardSetState(Default + baseState);
		} else if (sc->chPrev == '=') {
			sc->SetState(Value + baseState);
		}
	} else if (sc->state == DoubleString + baseState) {
		if (sc->ch == '\"') {
			CheckForLexerName(sc);
			sc->ForwardSetState(Other + baseState);
		}
	} else if (sc->state == SingleString + baseState) {
		if (sc->ch == '\'') {
			CheckForLexerName(sc);
			sc->ForwardSetState(Other + baseState);
		}
	} else if (sc->state == Number + baseState) {
		if (!Styler::IsDigit(sc->ch)) {
			sc->SetState(Other + baseState);
		}
	} else if (sc->state == XMLStart + baseState) {
		sc->ForwardSetState(Other + baseState);
	} else if (sc->state == XMLEnd + baseState) {
		sc->ForwardSetState(Default + baseState);
	} else if (sc->state == CDATA + baseState) {
		if (sc->Match(&cdataEnd)) {
			sc->Forward();
			sc->Forward();
			sc->ForwardSetState(Default + baseState);
		}
	} else if (sc->state == SGML + baseState) {
		if (sc->ch == '>') {
			sc->ForwardSetState(Default + baseState);
		} else if (sc->ch == '\"') {
			sc->SetState(SGMLDoubleString + baseState);
		} else if (sc->ch == '\'') {
			sc->SetState(SGMLSimpleString + baseState);
		}
	} else if (sc->state == SGMLCommand + baseState) {
		if (sc->ch == '>') {
			sc->ForwardSetState(Default + baseState);
		} else if (!isSgmlWordChar(sc->ch)) {
			sc->ForwardSetState(SGML + baseState);
		}
	} else if (sc->state == SGMLDoubleString + baseState) {
		if (sc->ch == '\"') {
			sc->ForwardSetState(SGML + baseState);
		}
	} else if (sc->state == SGMLSimpleString + baseState) {
		if (sc->ch == '\'') {
			sc->ForwardSetState(SGML + baseState);
		}
	}

	if (sc->state == Default + baseState) {
		if (sc->ch == '<') {
			if (sc->chNext == '?') {
				sc->SetState(XMLStart + baseState);
			} else if (sc->Match(&commentStart)) {
				sc->SetState(Comment + baseState);
			} else if (sc->Match(&cdataStart)) {
				sc->SetState(CDATA + baseState);
			} else if (sc->chNext == '!') {
				sc->SetState(SGML + baseState);
				sc->Forward();
				sc->ForwardSetState(SGMLCommand + baseState);
			} else {
				sc->SetState(TagUnknown + baseState);
			}
		} else if (sc->ch == '&') {
			sc->SetState(Entity + baseState);
		}
	} else if ((sc->state == Other + baseState) || (sc->state == Value + baseState)) {
		if (sc->ch == '\"') {
			sc->SetState(DoubleString + baseState);
		} else if (sc->ch == '\'') {
			sc->SetState(SingleString + baseState);
		} else if (Styler::IsDigit(sc->ch)) {
			sc->SetState(Number + baseState);
		} else if (sc->ch == '>') {
			EndTagCheckScripting(sc);
		} else if (sc->Match('?', '>')) {
			sc->SetState(XMLEnd + baseState);
		} else if (isHtmlWordChar(sc->ch) && (sc->state == Other + baseState)) {
			sc->SetState(Attribute + baseState);
		}
	} else if (sc->state == SGML + baseState) {
		if (sc->ch == '>') {
			sc->ForwardSetState(Default + baseState);
		}
	}
}

void LexHTML::Colourise(Styler *sc) {
	for (; sc->More(); sc->Forward()) {
		ColourisePiece(sc);
	}
	sc->Complete();
}
