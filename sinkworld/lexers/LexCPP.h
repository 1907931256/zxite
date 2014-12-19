// SinkWorld LexCPP class.
// Copyright 2004 Neil Hodgson
// File written in C~ subset of C++ for automatic translation to Java and C#.

/// LexCPP is a lexer for the C, C++, Java, C# and JavaScript languages.

SW_PUBLIC class LexCPP : public LexBase {
public:
	enum { Default = 0 };
	enum { Comment = 1 };
	enum { CommentLine = 2 };
	enum { CommentDoc = 3 };
	enum { Number = 4 };
	enum { Quoted = 5 };
	enum { Character = 6 };
	enum { UUID = 7 };
	enum { Preprocessor = 8 };
	enum { Operator = 9 };
	enum { StringEOL = 10 };
	enum { Verbatim = 11 };
	enum { RegEx = 12 };
	enum { CommentLineDoc = 13 };
	enum { CommentDocKeyWord = 14 };
	enum { CommentDocKeyWordError = 15 };
	enum { Identifier = 16 };
	enum { KeyWord = 17 };

	Literal doxygenStart1;
	Literal doxygenStart2;
	Literal doxygenStart3;
	Literal doxygenStart4;
	Literal uuid;

	KeyWords *keyWords;
	bool stylingWithinPreprocessor;

	// This is local lexing state -> should go into another object for reentrance
	int chPrevNonWhite;
	int visibleChars;
	bool lastWordWasUUID;

	LexCPP() {
		thisStates = 18;
		delete name;
		name = new Literal("CPP");
		delete stateNames;
		stateNames = new Literal(
"Default Comment CommentLine CommentDoc Number Quoted Character UUID Preprocessor Operator StringEOL Verbatim RegEx CommentLineDoc CommentDocKeyWord CommentDocKeyWordError Identifier KeyWord");

		doxygenStart1 = Literal("/**");
		doxygenStart2 = Literal("/*!");
		doxygenStart3 = Literal("///");
		doxygenStart4 = Literal("//!");
		uuid = Literal("uuid");
		keyWords = new KeyWords(false);
		stylingWithinPreprocessor = false;

		chPrevNonWhite = ' ';
		visibleChars = 0;
		lastWordWasUUID = false;
	}

	~LexCPP() {
		delete keyWords;
		keyWords = NULL;
	}

	SW_OVERRIDE IPrototype *CreateNew() {
		return new LexCPP();
	}

	static bool isOKBeforeRE(int ch) {
		return (ch == '(') || (ch == '=') || (ch == ',');
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
		return ch == '%' || ch == '#' || ch == '&' || ch == '*' ||
			ch == '(' || ch == ')' || ch == '-' || ch == '+' ||
			ch == '=' || ch == '|' || ch == '{' || ch == '}' ||
			ch == '[' || ch == ']' || ch == ':' || ch == ';' ||
			ch == '<' || ch == '>' || ch == ',' || ch == '/' ||
			ch == '?' || ch == '!' || ch == '.' || ch == '~';
	}

	static bool isDoxygenChar(int ch) {
		return (ch >= 'a' && ch <= 'z') ||
				ch == '$' || ch == '@' ||
				ch == '\\' || ch == '&' || ch == '<' ||
				ch == '>' || ch == '#' || ch == '{' ||
				ch == '}' || ch == '[' || ch == ']';
	}

	SW_OVERRIDE void InvalidateAfter(int position) {
		chPrevNonWhite = ' ';
		visibleChars = 0;
		lastWordWasUUID = false;
	}

	SW_OVERRIDE void ColourisePiece(Styler *sc);
	SW_OVERRIDE void Colourise(Styler *sc);

	SW_OVERRIDE bool InStateRange(int state) {
		return (state >= baseState) && (state < baseState + States());
	}

	SW_OVERRIDE bool CanBeInterrupted(int state) {
		int stateLocal = state - baseState;
		switch (stateLocal) {
			case Comment:
			case CommentLine:
			case CommentDoc:
			case Quoted:
			case Character:
			case Verbatim:
			case RegEx:
			case CommentLineDoc:
			case CommentDocKeyWord:
			case CommentDocKeyWordError:
				return false;
			default:
				return true;
		}
	}

	SW_OVERRIDE void SetKeyWords(int n, Literal *keyWordsN) {
		keyWords->SetText(keyWordsN, n);
		// There should always be at least one set of keywords and the identfier state.
		thisStates = 16 + 1 + keyWords->Sets();
	}

	SW_OVERRIDE Literal *StateNames() { 
		return stateNames; 
	}
};
