// SinkWorld LexBase class.
// Copyright 2004 Neil Hodgson
// File written in C~ subset of C++ for automatic translation to Java and C#.

/// An implementation of the ILexer interface that provides standard features.
/// It can be subclassed either for simple lexers that are completely responsible 
/// for performing lexing or composite lexers that use sublexers to handle parts 
/// of the text.

SW_PUBLIC class LexBase : public ILexer {
public:
	int baseAllocation;	// The starting state number for this lexer and any sublexers.
	int baseState;	// The starting state number for this lexer, ignoring sublexers.
	int subStates;	// The number of states required by sublexers.
	int thisStates;	// The number of states required for this lexer, ignoring sublexers.
	Literal *name;
	Literal *stateNames;

	LexBase() {
		baseState = 0;
		subStates = 0;
		baseAllocation = 0;
		thisStates = 1;
		name = new Literal("Base");
		stateNames = new Literal("");
	}

	virtual ~LexBase() {
		delete stateNames;
		stateNames = NULL;
		delete name;
		name = NULL;
	}

	virtual IPrototype *CreateNew() {
		return NULL;
	}

	virtual void InvalidateAfter(int position) {
	}

	virtual void ColourisePiece(Styler *sc) {
	}

	virtual void Colourise(Styler *sc) {
		InvalidateAfter(sc->startSeg);
		for (; sc->More(); sc->Forward()) {
			ColourisePiece(sc);
		}
		sc->Complete();
	}

	virtual int States() {
		return thisStates + subStates;
	}

	virtual void SetBaseState(int state) {
		baseAllocation = state;
		AllocateStates(baseAllocation);
	}

	virtual int GetBaseState() {
		// TODO Work out why baseAllocation was being used return baseAllocation;
		return baseState;
	}

	virtual bool InStateRange(int state) {
		return (state >= baseAllocation) && (state < baseAllocation + States());
	}

	virtual int StateFromName(SW_BYTE s[], int slen) {
		if (stateNames != NULL) {
			// TODO search for state return stateNames->Find(s, slen, 0);
			return 0;
		} else {
			return -1;
		}
	}

	virtual bool CanBeInterrupted(int state) {
		return true;
	}

	virtual void SetKeyWords(int n, Literal *keyWordsN) {
	}

	virtual void SetSubLexer(ILexer *lx, Literal *name, Literal *name2) {
		throw new ConfigurationException();
	}

	virtual ILexer *GetSubLexer(Literal *name) {
                return NULL;
        }

	virtual void SetInputStyles(IStyles *stylesInput) {
	}

	virtual void AllocateStates(int startAllocation) {
		baseState = startAllocation;
	}

	virtual Literal *Name() { 
		return name; 
	}

	virtual Literal *StateNames() { 
		return stateNames; 
	}

	virtual Literal *StateName(int state) { 
		return stateNames->Segment(state - baseState, ' '); 
	}

	virtual ILexer *LexerFromState(int state) {
		return NULL;
	}

	virtual void AttachHistories(SplitText *pbuffer, int priority) {
	}

};
