// SinkWorld LexURL class.
// Copyright 2004 Neil Hodgson
// File written in C~ subset of C++ for automatic translation to Java and C#.

/// LexURL is a lexer that recognises URLs in text.

SW_PUBLIC class LexURL : public LexBase {
public:
	enum { Default = 0 };
	enum { URL = 1 };
	enum { NotURL = 2 };

	LexURL() {
		thisStates = 3;
		delete name;
		name = new Literal("URL");
		delete stateNames;
		stateNames = new Literal("Default URL NotURL");
	}

	~LexURL() {
	}

	SW_OVERRIDE IPrototype *CreateNew() {
		return new LexURL();
	}

	static bool isURLStart(int ch) {
		return
			(ch >= 'a' && ch <= 'z') ||
			(ch >= 'A' && ch <= 'Z');
	}

	static bool isURLChar(int ch) {
		return
			(ch >= 'a' && ch <= 'z') ||
			(ch >= 'A' && ch <= 'Z') ||
			(ch >= '0' && ch <= '9') ||
			ch == '_' ||
			ch == '-' ||
			ch == '=' ||
			ch == ':' ||
			ch == '/' ||
			ch == '?' ||
			ch == '.';
	}

	SW_OVERRIDE void InvalidateAfter(int position) {
	}

	SW_OVERRIDE void ColourisePiece(Styler *sc);
	SW_OVERRIDE void Colourise(Styler *sc);

	SW_OVERRIDE bool InStateRange(int state) {
		return (state >= baseState) && (state < baseState + States());
	}

	SW_OVERRIDE bool CanBeInterrupted(int state) {
		return true;
	}

	SW_OVERRIDE void SetKeyWords(int n, Literal *keyWordsN) {
	}
};
