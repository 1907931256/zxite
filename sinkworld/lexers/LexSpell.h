// SinkWorld LexSpell class.
// Copyright 2004 Neil Hodgson
// File written in C~ subset of C++ for automatic translation to Java and C#.

/// LexSpell demonstration aspects of how a spell checking lexer could be written.
/// A good implementation of a spell checker would need a better way to handle the 
/// word list.

SW_PUBLIC class LexSpell : public LexBase {
public:
	enum { Default = 0 };
	enum { Word = 1 };
	enum { Mistake = 2 };

	KeyWords *keyWords;

	LexSpell() {
		thisStates = 3;
		delete name;
		name = new Literal("Spelling");
		delete stateNames;
		stateNames = new Literal("Default Word Mistake");

		keyWords = new KeyWords(false);
	}

	~LexSpell() {
		delete keyWords;
		keyWords = NULL;
	}

	SW_OVERRIDE IPrototype *CreateNew() {
		return new LexSpell();
	}

	static bool isWordStart(int ch) {
		return
			(ch >= 'a' && ch <= 'z') ||
			(ch >= 'A' && ch <= 'Z');
	}

	static bool isWordChar(int ch) {
		return
			(ch >= 'a' && ch <= 'z') ||
			(ch >= 'A' && ch <= 'Z');
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
		if (n == 1) {
			keyWords->SetText(keyWordsN, n);
		}
	}
};
