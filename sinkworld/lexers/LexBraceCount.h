// SinkWorld LexBraceCount class.
// Copyright 2004 Neil Hodgson
// File written in C~ subset of C++ for automatic translation to Java and C#.

/// LexBraceCount is a lexer that counts braces.
/// It can be used to provide folding or to color-code nesting depth.

SW_PUBLIC class LexBraceCount : public LexBase {
public:
	enum { Default = 0 };
	enum { Last = 18 };
	enum { Negative = 19 };

	IStyles *stylesBase;
	int matchStyle;
	bool insideBrace;

	LexBraceCount() {
		thisStates = 20;
		delete name;
		name = new Literal("BraceCount");
		delete stateNames;
		stateNames = new Literal(
"Default L1 L2 L3 L4 L5 L6 L7 L8 L9 L10 L11 L12 L13 L14 L15 L16 L17 L18 Negative");

		stylesBase = NULL;
		matchStyle = 0;
		insideBrace = false;
	}

	~LexBraceCount() {
	}

	SW_OVERRIDE IPrototype *CreateNew() {
		return new LexBraceCount();
	}

	static bool isStartBrace(int ch) {
		return (ch == '(') || (ch == '[') || (ch == '{');
	}

	static bool isEndBrace(int ch) {
		return (ch == ')') || (ch == ']') || (ch == '}');
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

	SW_OVERRIDE void SetInputStyles(IStyles *stylesInput) {
		stylesBase = stylesInput;
	}

	void SetInputMatch(int matchStyle_) {
		matchStyle = matchStyle_;
	}

	int GetInputMatch() {
		return matchStyle;
	}

	void SetInsideBrace(bool insideBrace_) {
		insideBrace = insideBrace_;
	}

	bool GetInsideBrace() {
		return insideBrace;
	}
};
