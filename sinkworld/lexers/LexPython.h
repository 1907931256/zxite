// SinkWorld LexPython class.
// Copyright 2004 Neil Hodgson
// File written in C~ subset of C++ for automatic translation to Java and C#.

/// LexPython is a lexer for the Python language.

SW_PUBLIC class LexPython : public LexBase {
public:
	enum { Default = 0 };
	enum { CommentLine = 1 };
	enum { CommentBlock = 2 };
	enum { Number = 3 };
	enum { Quoted = 4 };
	enum { DoubleQuoted = 5 };
	enum { Triple = 6 };
	enum { TripleDouble = 7 };
	enum { StringEOL = 8 };
	enum { Operator = 9 };
	enum { ClassName = 10 };
	enum { DefName = 11 };
	enum { Identifier = 12 };
	enum { KeyWord = 13 };

	enum { kwOther = 0 };
	enum { kwClass = 1 };
	enum { kwDef = 2 };
	enum { kwImport = 3 };

	Literal tripleQuote;
	Literal tripleDouble;
	Literal tAs;
	Literal tClass;
	Literal tDef;
	Literal tImport;
	KeyWords *keyWords;

	// This is local lexing state -> should go into another object for reentrance
	int kwLast;
	bool hexadecimal;

	LexPython() {
		thisStates = 14;
		delete name;
		name = new Literal("Python");
		delete stateNames;
		stateNames = new Literal(
"Default CommentLine CommentBlock Number Quoted DoubleQuoted Triple TripleDouble StringEOL Operator ClassName DefName Identifier KeyWord");

		tripleQuote = Literal("'''");
		tripleDouble = Literal("\"\"\"");
		tAs = Literal("as");
		tClass = Literal("class");
		tDef = Literal("def");
		tImport = Literal("import");

		keyWords = new KeyWords(false);

		kwLast = kwOther;
		hexadecimal = false;
	}

	~LexPython() {
		delete keyWords;
		keyWords = NULL;
	}

	SW_OVERRIDE IPrototype *CreateNew() {
		return new LexPython();
	}

	SW_OVERRIDE void SetKeyWords(int n, Literal *keyWordsN) {
		keyWords->SetText(keyWordsN, n);
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

	static bool isOperator(int ch) {
		return ch == '%' || ch == '&' || ch == '*' ||
			ch == '(' || ch == ')' || ch == '-' || ch == '+' ||
			ch == '=' || ch == '|' || ch == '{' || ch == '}' ||
			ch == '[' || ch == ']' || ch == ':' || ch == ';' ||
			ch == '<' || ch == '>' || ch == ',' || ch == '/' ||
			ch == '?' || ch == '!' || ch == '.' || ch == '~';
	}

	static bool isStringStart(int ch, int chNext, int chNext2) {
		if (ch == '\'' || ch == '"')
			return true;
		if (ch == 'u' || ch == 'U') {
			if (chNext == '"' || chNext == '\'')
				return true;
			if ((chNext == 'r' || chNext == 'R') && (chNext2 == '"' || chNext2 == '\''))
				return true;
		}
		if ((ch == 'r' || ch == 'R') && (chNext == '"' || chNext == '\''))
			return true;

		return false;
	}

	SW_OVERRIDE void InvalidateAfter(int position) {
		kwLast = kwOther;
		hexadecimal = false;
	}
	SW_OVERRIDE void ColourisePiece(Styler *sc);
	SW_OVERRIDE void Colourise(Styler *sc);

	SW_OVERRIDE bool CanBeInterrupted(int state) {
		return true;
	}
};
