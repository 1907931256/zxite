// SinkWorld LexASP class.
// Copyright 2004 Neil Hodgson
// File written in C~ subset of C++ for automatic translation to Java and C#.

/// LexASP is a lexer for the ASP language.
/// It supports sublexers for HTML and for embedded scripts.

SW_PUBLIC class LexASP : public LexScript {
	typedef LexScript super;
public:
	enum { ASP = 0 };
	enum { ASPAt = 1 };

	Literal htmlLexerName;

	ILexer *lexerHTML;
	History *defaultLexer;

	LexASP() {
		thisStates = 2;
		delete name;
		name = new Literal("ASP");
		delete stateNames;
		stateNames = new Literal(
			"ASP ASPAt");

		htmlLexerName = Literal("html");
		lexerHTML = NULL;
		defaultLexer = new History(0);
	}

	virtual ~LexASP() {
		delete defaultLexer;
		defaultLexer = NULL;
	}

	SW_OVERRIDE IPrototype *CreateNew() {
		return new LexASP();
	}

	SW_OVERRIDE void SetKeyWords(int n, Literal *keyWordsN) {
	}

	SW_OVERRIDE void InvalidateAfter(int position) {
		if (lexerHTML != NULL) {
			lexerHTML->InvalidateAfter(position);
		}
		super::InvalidateAfter(position);
	}
	SW_OVERRIDE void ColourisePiece(Styler *sc);
	SW_OVERRIDE void Colourise(Styler *sc);

	SW_OVERRIDE bool CanBeInterrupted(int state) {
		return false;
	}

	SW_OVERRIDE void AllocateStates(int startAllocation) {
		subStates = 0;
		int allocatedState = startAllocation;
		// Start with HTML states so the default 0 is HTML
		if (lexerHTML != NULL) {
			subStates += lexerHTML->States();
			lexerHTML->SetBaseState(allocatedState);
			allocatedState += lexerHTML->States();
		}
		super::AllocateStates(allocatedState);
	}

	SW_OVERRIDE void SetSubLexer(ILexer *lx, Literal *name, Literal *name2) {
		if (name->Equals(&htmlLexerName)) {
			lexerHTML = lx;
			AllocateStates(baseAllocation);
		} else {
			super::SetSubLexer(lx, name, name2);
		}
	}

	SW_OVERRIDE ILexer*GetSubLexer(Literal *name) {
		if (name->Equals(&htmlLexerName)) {
			return lexerHTML;
		} else {
			return super::GetSubLexer(name);
		}
	}

	SW_OVERRIDE Literal *StateName(int state) { 
		if ((lexerHTML != NULL) && (lexerHTML->InStateRange(state))) {
			return lexerHTML->StateName(state);
		} else {
			return super::StateName(state); 
		}
	}

	SW_OVERRIDE ILexer *LexerFromState(int state) {
		if ((lexerHTML != NULL) && (lexerHTML->InStateRange(state))) {
			return lexerHTML;
		} else {
			return super::LexerFromState(state);
		}
	}

	SW_OVERRIDE void AttachHistories(SplitText *pbuffer, int priority) {
		pbuffer->AttachHistory(this, defaultLexer);
		if (lexerHTML != NULL) {
			lexerHTML->AttachHistories(pbuffer, priority);
		}
		super::AttachHistories(pbuffer, priority);
	}
};
