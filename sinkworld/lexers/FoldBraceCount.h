// SinkWorld FoldBraceCount class.
// Copyright 2005 Neil Hodgson
// File written in C~ subset of C++ for automatic translation to Java and C#.

/// FoldBraceCount is a folder that counts braces.

SW_PUBLIC class FoldBraceCount : public FoldBase {
public:
	IStyles *stylesBase;
	int matchStyle;

	FoldBraceCount() {
		delete name;
		name = new Literal("FoldBraceCount");
		stylesBase = NULL;
		matchStyle = 0;
	}

	~FoldBraceCount() {
	}

	SW_OVERRIDE IPrototype *CreateNew() {
		return new FoldBraceCount();
	}

	SW_OVERRIDE void InvalidateAfter(int position) {
	}

	SW_OVERRIDE void ColourisePiece(Styler *sc);
	SW_OVERRIDE void Colourise(Styler *sc);

	SW_OVERRIDE bool InStateRange(int state) {
		return true;
	}

	SW_OVERRIDE bool CanBeInterrupted(int state) {
		return true;
	}

	SW_OVERRIDE void SetKeyWords(int n, Literal *keyWordsN) {
	}

	static bool isStartBrace(int ch) {
		return (ch == '(') || (ch == '[') || (ch == '{');
	}

	static bool isEndBrace(int ch) {
		return (ch == ')') || (ch == ']') || (ch == '}');
	}

	SW_OVERRIDE void SetInputStyles(IStyles *stylesInput) {
		stylesBase = stylesInput;
	}

	void SetInputMatch(int matchStyle_) {
		matchStyle = matchStyle_;
	}
};
