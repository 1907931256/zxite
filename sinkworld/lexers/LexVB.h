// SinkWorld LexVB class.
// Copyright 2004 Neil Hodgson
// File written in C~ subset of C++ for automatic translation to Java and C#.

/// LexVB is a lexer for the Visual Basic language.

SW_PUBLIC class LexVB : public LexBase {
public:
	enum { Default = 0 };
	enum { Comment = 1 };
	enum { Number = 2 };
	enum { Quoted = 3 };
	enum { Preprocessor = 4 };
	enum { Operator = 5 };
	enum { Date = 6 };
	enum { Identifier = 7 };
	enum { KeyWord = 8 };

	Literal rem;
	KeyWords *keyWords;

	bool vbScriptSyntax;

	// This is local lexing state -> should go into another object for reentrance
	int visibleChars;

	LexVB() {
		thisStates = 9;
		delete name;
		name = new Literal("VB");
		delete stateNames;
		stateNames = new Literal(
"Default Comment Number Quoted Preprocessor Operator Date Identifier KeyWord");
		rem = Literal("rem");
		keyWords = new KeyWords(false);
		vbScriptSyntax = false;
		visibleChars = 0;
	}

	virtual ~LexVB() {
		delete keyWords;
		keyWords = NULL;
	}

	SW_OVERRIDE IPrototype *CreateNew() {
		return new LexVB();
	}

	static bool isWordStart(int ch) {
		return
			(ch >= 'a' && ch <= 'z') ||
			(ch >= 'A' && ch <= 'Z') ||
			(ch >= '0' && ch <= '9') ||
			(ch == '_');
	}

	static bool isWordChar(int ch) {
		return
			(ch >= 'a' && ch <= 'z') ||
			(ch >= 'A' && ch <= 'Z') ||
			(ch >= '0' && ch <= '9') ||
			(ch == '_') ||
			(ch == '.');
	}

	static bool isDateChar(int ch) {
		return
			(ch >= 'a' && ch <= 'z') ||
			(ch >= 'A' && ch <= 'Z') ||
			(ch >= '0' && ch <= '9') ||
			(ch == '|') ||
			(ch == '-') ||
			(ch == '/') ||
			(ch == ':') ||
			(ch == ' ') ||
			(ch == '\t');
	}

	static bool isTypeChar(int ch) {
		return
			(ch == '%') ||
			(ch == '&') ||
			(ch == '@') ||
			(ch == '!') ||
			(ch == '#') ||
			(ch == '$');
	}

	static bool isOperator(int ch) {
		return ch == '%' || ch == '&' || ch == '*' ||
			ch == '(' || ch == ')' || ch == '-' || ch == '+' ||
			ch == '=' || ch == '|' || ch == '{' || ch == '}' ||
			ch == '[' || ch == ']' || ch == ':' || ch == ';' ||
			ch == '<' || ch == '>' || ch == ',' || ch == '/' ||
			ch == '?' || ch == '!' || ch == '.' || ch == '~';
	}

	SW_OVERRIDE void InvalidateAfter(int position) {
		visibleChars = 0;
	}

	SW_OVERRIDE void ColourisePiece(Styler *sc);
	SW_OVERRIDE void Colourise(Styler *sc);

	SW_OVERRIDE bool InStateRange(int state) {
		return (state >= baseState) && (state < baseState + States());
	}

	SW_OVERRIDE bool CanBeInterrupted(int state) {
		int stateLocal = state - baseState;
		switch (stateLocal) {
			case Quoted:
				return false;
			default:
				return true;
		}
	}

	SW_OVERRIDE void SetKeyWords(int n, Literal *keyWordsN) {
		keyWords->SetText(keyWordsN, n);
	}
};
