// SinkWorld LexNull class.
// Copyright 2005 Neil Hodgson
// File written in C~ subset of C++ for automatic translation to Java and C#.

/// LexNull is a lexer that performs no lexing, producing state SCE_NULL_DEFAULT 
/// for all input.

SW_PUBLIC class LexNull : public LexBase {
public:
	enum { Default = 0 };

	LexNull() {
		thisStates = 1;
		delete name;
		name = new Literal("Null");
		delete stateNames;
		stateNames = new Literal(
"Default");
	}

	~LexNull() {
	}

	SW_OVERRIDE IPrototype *CreateNew() {
		return new LexNull();
	}

	SW_OVERRIDE void InvalidateAfter(int position) {
	}

	SW_OVERRIDE void ColourisePiece(Styler *sc) {
		// Determine if the current state should terminate.
		switch (sc->state - baseState) {
			case Default:
				sc->SetState(Default + baseState);
				break;
		}
	}
	
	SW_OVERRIDE void Colourise(Styler *sc) {
		InvalidateAfter(sc->startSeg);
		for (; sc->More(); sc->Forward()) {
			ColourisePiece(sc);
		}
		sc->Complete();
	}

	SW_OVERRIDE bool InStateRange(int state) {
		return (state >= baseState) && (state < baseState + States());
	}

	SW_OVERRIDE bool CanBeInterrupted(int state) {
		return true;
	}

	SW_OVERRIDE void SetKeyWords(int n, Literal *keyWordsN) {
	}

	SW_OVERRIDE Literal *StateNames() { 
		return stateNames; 
	}
};
