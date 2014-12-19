// SinkWorld LexPreProc class.
// Copyright 2004 Neil Hodgson
// File written in C~ subset of C++ for automatic translation to Java and C#.

// Place holder for the real implementation
SW_PUBLIC class LexPreProc : public LexScript {
	typedef LexScript super;
public:

	LexPreProc() {
		delete name;
		name = new Literal("PreProc");
	}

	virtual ~LexPreProc() {
	}

	SW_OVERRIDE IPrototype *CreateNew() {
		return new LexPreProc();
	}

	static bool isWord(int ch) {
		return 1==1;
	}

	SW_OVERRIDE void InvalidateAfter(int position) {
	}
	int EvaluateExpression(Styler *sc, Literal *expr);
	bool EvaluateConditional(Styler *sc);
	SW_OVERRIDE void ColourisePiece(Styler *sc);
	SW_OVERRIDE void Colourise(Styler *sc);

	SW_OVERRIDE bool CanBeInterrupted(int state) {
		return false;
	}

	SW_OVERRIDE void AllocateStates(int startAllocation) {
		subStates = 0;
	}
};
