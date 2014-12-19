// SinkWorld FoldBase class.
// Copyright 2005 Neil Hodgson
// File written in C~ subset of C++ for automatic translation to Java and C#.

/// An implementation of the ILexer interface that provides standard features.
/// It can be subclassed either for simple folders that are completely responsible 
/// for performing folding or composite folders that use subfolders to handle parts 
/// of the text.

SW_PUBLIC class FoldBase : public ILexer {
public:
	Literal *name;

	FoldBase() {
		name = new Literal("FoldBase");
	}

	virtual ~FoldBase() {
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
		return 1;
	}

	virtual void SetBaseState(int state) {
	}

	virtual int GetBaseState() {
		return 0;
	}

	virtual bool InStateRange(int state) {
		return true;
	}

	virtual int StateFromName(SW_BYTE s[], int slen) {
		return 0;
	}

	virtual bool CanBeInterrupted(int state) {
		return true;
	}

	virtual void SetKeyWords(int n, Literal *keyWordsN) {
	}

	virtual void SetSubLexer(ILexer *lx, Literal *name, Literal *name2) {
	}

	virtual ILexer *GetSubLexer(Literal *name) {
                return NULL;
        }

	virtual void SetInputStyles(IStyles *stylesInput) {
	}

	virtual void AllocateStates(int startAllocation) {
	}

	virtual Literal *Name() { 
		return name; 
	}

	virtual Literal *StateNames() { 
		return NULL; 
	}

	virtual Literal *StateName(int state) { 
		return NULL; 
	}

	virtual ILexer *LexerFromState(int state) {
		return NULL;
	}

	virtual void AttachHistories(SplitText *pbuffer, int priority) {
	}
};
