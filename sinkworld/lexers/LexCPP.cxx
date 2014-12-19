// SinkWorld LexCPP class.
// Copyright 2004 Neil Hodgson
// File written in C~ subset of C++ for automatic translation to Java and C#.

#include <string.h>
#include <assert.h>
#include <string>

#include "base.h"
#include "lexers.h"

void LexCPP::ColourisePiece(Styler *sc) {
	if (sc->atLineStart && (sc->state == StringEOL + baseState)) {
		// Prevent QuotedEOL from leaking onto next line
		sc->SetState(Default + baseState);
	}

	// Handle line continuation generically.
	if (sc->ch == '\\') {
		if (sc->chNext == '\n' || sc->chNext == '\r') {
			sc->Forward();
			if (sc->ch == '\r' && sc->chNext == '\n') {
				sc->Forward();
			}
			return;
		}
	}

	// Determine if the current state should terminate.
	switch (sc->state - baseState) {
		case Operator:
			sc->SetState(Default + baseState);
			break;
		case Number:
			if (!isWordChar(sc->ch)) {
				sc->SetState(Default + baseState);
			}
			break;
		case Identifier:
			if (!isWordChar(sc->ch) || (sc->ch == '.')) {
				sc->SetWord();
				int kwSet = sc->WordInList(keyWords);
				if (kwSet != 0) {
					lastWordWasUUID = sc->WordIs(&uuid);
					sc->ChangeState(Identifier + kwSet + baseState);
				}
				sc->SetState(Default + baseState);
			}
			break;
		case Preprocessor:
			if (stylingWithinPreprocessor) {
				if (Styler::IsSpaceChar(sc->ch)) {
					sc->SetState(Default + baseState);
				}
			} else {
				if ((sc->atLineEnd) || (sc->Match('/', '*')) || (sc->Match('/', '/'))) {
					sc->SetState(Default + baseState);
				}
			}
			break;
		case Comment:
			if (sc->Match('*', '/')) {
				sc->Forward();
				sc->ForwardSetState(Default + baseState);
			}
			break;
		case CommentDoc:
			if (sc->Match('*', '/')) {
				sc->Forward();
				sc->ForwardSetState(Default + baseState);
			} else if (sc->ch == '@' || sc->ch == '\\') {
				sc->SetState(CommentDocKeyWord + baseState);
			}
			break;
		case CommentLine:
		case CommentLineDoc:
			if (sc->atLineEnd) {
				sc->SetState(Default + baseState);
				visibleChars = 0;
			}
			break;
		case CommentDocKeyWord:
			if (sc->Match('*', '/')) {
				sc->ChangeState(CommentDocKeyWordError + baseState);
				sc->Forward();
				sc->ForwardSetState(Default + baseState);
			} else if (!isDoxygenChar(sc->ch)) {
				sc->SetState(CommentDoc + baseState);
			}
			break;
		case Quoted:
			if (sc->ch == '\\') {
				if (sc->chNext == '\"' || sc->chNext == '\'' || sc->chNext == '\\') {
					sc->Forward();
				}
			} else if (sc->ch == '\"') {
				sc->ForwardSetState(Default + baseState);
			} else if (sc->atLineEnd) {
				sc->ChangeState(StringEOL + baseState);
				sc->ForwardSetState(Default + baseState);
				visibleChars = 0;
			}
			break;
		case Character:
			if (sc->atLineEnd) {
				sc->ChangeState(StringEOL + baseState);
				sc->ForwardSetState(Default + baseState);
				visibleChars = 0;
			} else if (sc->ch == '\\') {
				if (sc->chNext == '\"' || sc->chNext == '\'' || sc->chNext == '\\') {
					sc->Forward();
				}
			} else if (sc->ch == '\'') {
				sc->ForwardSetState(Default + baseState);
			}
			break;
		case RegEx:
			if (sc->ch == '\r' || sc->ch == '\n' || sc->ch == '/') {
				sc->ForwardSetState(Default + baseState);
			} else if (sc->ch == '\\') {
				// Gobble up the quoted Character
				if (sc->chNext == '\\' || sc->chNext == '/') {
					sc->Forward();
				}
			}
			break;
		case Verbatim:
			if (sc->ch == '\"') {
				if (sc->chNext == '\"') {
					sc->Forward();
				} else {
					sc->ForwardSetState(Default + baseState);
				}
			}
			break;
		case UUID:
			if (sc->ch == '\r' || sc->ch == '\n' || sc->ch == ')') {
				sc->SetState(Default + baseState);
			}
			break;
	}

	// Determine if a new state should be entered.
	if (sc->state == Default + baseState) {
		if (sc->Match('@', '\"')) {
			sc->SetState(Verbatim + baseState);
			sc->Forward();
		} else if (Styler::IsDigit(sc->ch) || (sc->ch == '.' && Styler::IsDigit(sc->chNext))) {
			if (lastWordWasUUID) {
				sc->SetState(UUID + baseState);
				lastWordWasUUID = false;
			} else {
				sc->SetState(Number + baseState);
			}
		} else if (isWordStart(sc->ch) || (sc->ch == '@')) {
			if (lastWordWasUUID) {
				sc->SetState(UUID + baseState);
				lastWordWasUUID = false;
			} else {
				sc->SetState(Identifier + baseState);
			}
		} else if (sc->Match('/', '*')) {
			if (sc->Match(&doxygenStart1) || sc->Match(&doxygenStart2)) {	// Support of Qt/Doxygen doc. style
				sc->SetState(CommentDoc + baseState);
			} else {
				sc->SetState(Comment + baseState);
			}
			sc->Forward();	// Eat the * so it isn't used for the end of the Comment
		} else if (sc->Match('/', '/')) {
			if (sc->Match(&doxygenStart3) || sc->Match(&doxygenStart4))	// Support of Qt/Doxygen doc. style
				sc->SetState(CommentLineDoc + baseState);
			else
				sc->SetState(CommentLine + baseState);
		} else if (sc->ch == '/' && isOKBeforeRE(chPrevNonWhite)) {
			sc->SetState(RegEx + baseState);
		} else if (sc->ch == '\"') {
			sc->SetState(Quoted + baseState);
		} else if (sc->ch == '\'') {
			sc->SetState(Character + baseState);
		} else if (sc->ch == '#' && visibleChars == 0) {
			// Preprocessor commands are alone on their line
			sc->SetState(Preprocessor + baseState);
			// Skip whitespace between # and Preprocessor word
			do {
				sc->Forward();
			} while ((sc->ch == ' ') && (sc->ch == '\t') && sc->More());
			if (sc->atLineEnd) {
				sc->SetState(Default + baseState);
			}
		} else if (isOperator(sc->ch)) {
			sc->SetState(Operator + baseState);
		}
	}

	if (sc->atLineEnd) {
		// Reset states to begining of colourise so no surprises
		// if different sets of lines lexed.
		chPrevNonWhite = ' ';
		visibleChars = 0;
		lastWordWasUUID = false;
	}
	if (!Styler::IsSpaceChar(sc->ch)) {
		chPrevNonWhite = sc->ch;
		visibleChars++;
	}
}

void LexCPP::Colourise(Styler *sc) {
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
