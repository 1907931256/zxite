// SinkWorld LexPHP class.
// Copyright 2004 Neil Hodgson
// File written in C~ subset of C++ for automatic translation to Java and C#.

/// LexPHP is a lexer for the PHP language.
/// It supports a sublexer for HTML.

SW_PUBLIC class LexPHP : public LexBase {
public:
	enum { Default = 0 };
	enum { Comment = 1 };
	enum { CommentLine = 2 };
	enum { HString = 3 };
	enum { HStringVariable = 4 };
	enum { SimpleString = 5 };
	enum { Number = 6 };
	enum { Operator = 7 };
	enum { Delimiters = 8 };
	enum { Identifier = 9 };
	enum { KeyWord = 10 };

	ILexer *lexerHTML;

	Literal phpStart;
	Literal comment;

	KeyWords *keyWords;

	LexPHP() {
		thisStates = 11;
		delete name;
		name = new Literal("PHP");
		delete stateNames;
		stateNames = new Literal(
"Default Comment CommentLine HString HStringVariable SimpleString Number Operator Delimiters Identifier KeyWord");

		keyWords = new KeyWords(false);

		lexerHTML = NULL;
		phpStart = Literal("<?php");
		comment = Literal("<!--");
	}

	~LexPHP() {
		delete keyWords;
		keyWords = NULL;
	}

	SW_OVERRIDE IPrototype *CreateNew() {
		return new LexPHP();
	}

	static bool isWordStart(int ch) {
		return
			(ch >= 'a' && ch <= 'z') ||
			(ch >= 'A' && ch <= 'Z') ||
			(ch >= '0' && ch <= '9') ||
			(ch == '_');
	}

	static bool isOperator(int ch) {
		return ch == '%' || ch == '#' || ch == '&' || ch == '*' ||
			ch == '(' || ch == ')' || ch == '-' || ch == '+' ||
			ch == '=' || ch == '|' || ch == '{' || ch == '}' ||
			ch == '[' || ch == ']' || ch == ':' || ch == ';' ||
			ch == '<' || ch == '>' || ch == ',' || ch == '/' ||
			ch == '?' || ch == '!' || ch == '.' || ch == '~';
	}

	bool isStringState(int state) {
		return
			(state == (HString + baseState)) ||
			(state == (SimpleString + baseState)) ||
			(state == (HStringVariable + baseState));
	}

	SW_OVERRIDE void InvalidateAfter(int position) {}
	SW_OVERRIDE void ColourisePiece(Styler *sc);
	SW_OVERRIDE void Colourise(Styler *sc);

	SW_OVERRIDE bool CanBeInterrupted(int state) {
		return false;
	}

	SW_OVERRIDE void SetKeyWords(int n, Literal *keyWordsN) {
		keyWords->SetText(keyWordsN, n);
	}

	SW_OVERRIDE void AllocateStates(int startAllocation) {
		subStates = 0;
		int allocatedState = startAllocation;
		if (lexerHTML != NULL) {
			subStates += lexerHTML->States();
			lexerHTML->SetBaseState(allocatedState);
			allocatedState += lexerHTML->States();
		}
		// Allocate PHP after HTML so HTML gets default 0
		baseState = allocatedState;
		allocatedState += thisStates;
	}

	SW_OVERRIDE void SetSubLexer(ILexer *lx, Literal *name, Literal *name2) {
		lexerHTML = lx;
		AllocateStates(baseAllocation);
	}
};
