// SinkWorld LexPHP class.
// Copyright 2004 Neil Hodgson
// File written in C~ subset of C++ for automatic translation to Java and C#.

#include <string.h>
#include <assert.h>
#include <string>

#include "base.h"
#include "lexers.h"

void LexPHP::ColourisePiece(Styler *sc) {
	if (sc->state >= (Default + baseState) && sc->state <= (Delimiters + baseState)) {
		if (!isStringState(sc->state) &&
			(sc->state != Comment + baseState) &&
			(sc->Match('?', '>'))) {
			sc->SetState(Delimiters + baseState);
			sc->Forward();
			sc->Forward();
			if (lexerHTML != NULL) {
				sc->SetState(lexerHTML->GetBaseState());
			}
		}
	}

	if ((lexerHTML != NULL) && (lexerHTML->InStateRange(sc->state))) {
		lexerHTML->ColourisePiece(sc);
	}

	// Handle some PHP script
	if (sc->state == KeyWord + baseState) {
		if (!isWordStart(sc->ch)) {
			sc->SetState(Default + baseState);
		}
	} else if (sc->state == CommentLine + baseState) {
		if (sc->ch == '\r' || sc->ch == '\n') {
			sc->SetState(Default + baseState);
		}
	} else if (sc->state == Comment + baseState) {
		if (sc->Match('*', '/')) {
			sc->Forward();
			sc->Forward();
			sc->SetState(Default + baseState);
		}
	} else if (sc->state == HString + baseState) {
		if (sc->ch == '\"') {
			sc->ForwardSetState(Default + baseState);
		}
	} else if (sc->state == SimpleString + baseState) {
		if (sc->ch == '\'') {
			sc->ForwardSetState(Default + baseState);
		}
	} else if (sc->state == Identifier + baseState) {
		if (!isWordStart(sc->ch)) {
			sc->SetState(Default + baseState);
		}
	} else if (sc->state == Operator + baseState) {
		sc->SetState(Default + baseState);
	}

	// Enter new sc->state
	if ((lexerHTML == NULL) || (!lexerHTML->InStateRange(sc->state)) || (lexerHTML->CanBeInterrupted(sc->state))) {
		if (sc->Match(&phpStart)) {
			sc->SetState(Delimiters + baseState);
			sc->Forward();
			sc->Forward();
			sc->Forward();
			sc->Forward();
			sc->Forward();
			sc->SetState(Default + baseState);
		}
	}
	if (sc->state == Default + baseState) {
		if (isWordStart(sc->ch)) {
			sc->SetState(KeyWord + baseState);
		} else if (sc->ch == '#') {
			sc->SetState(CommentLine + baseState);
		} else if (sc->Match(&comment)) {
			sc->SetState(CommentLine + baseState);
		} else if (sc->Match('/', '/')) {
			sc->SetState(CommentLine + baseState);
		} else if (sc->Match('/', '*')) {
			sc->SetState(Comment + baseState);
		} else if (sc->ch == '\"') {
			sc->SetState(HString + baseState);
		} else if (sc->ch == '\'') {
			sc->SetState(SimpleString + baseState);
		} else if (sc->ch == '$') {
			sc->SetState(Identifier + baseState);
		} else if (isOperator(sc->ch)) {
			sc->SetState(Operator + baseState);
		}
	}
}

void LexPHP::Colourise(Styler *sc) {

	for (; sc->More(); sc->Forward()) {
		ColourisePiece(sc);
	}
	sc->Complete();
}
