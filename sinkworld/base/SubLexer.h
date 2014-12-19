// SinkWorld SubLexer class.
// Copyright 2005 Neil Hodgson
// File written in C~ subset of C++ for automatic translation to Java and C#.

/// SubLexer is used by LexScript to store information about a sublexer.

SW_PUBLIC class SubLexer {
public:
	SubLexer *slNext;
	ILexer *lexer;
	Literal *name;
	Literal *identifiers;
	SubLexer(ILexer *lexer_, Literal *name_, Literal *identifiers_) {
		slNext = NULL;
		lexer = lexer_;
		name = new Literal(name_);
		identifiers = new Literal(identifiers_);
	}
	~SubLexer() {
		delete name;
		name = NULL;
		delete identifiers;
		identifiers = NULL;
	}
};
