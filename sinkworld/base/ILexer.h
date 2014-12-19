/// ILexer interface is implemented by all lexers. 
// Copyright 2004 Neil Hodgson
// File written in C~ subset of C++ for automatic translation to Java and C#.

SW_PUBLIC class ILexer : public IPrototype {
public:
	virtual ~ILexer() {
	}
	virtual void InvalidateAfter(int position) = 0;
	virtual void ColourisePiece(Styler *sc) = 0;
	virtual void Colourise(Styler *sc) = 0;
	virtual int States() = 0;
	virtual void SetBaseState(int state) = 0;
	virtual int GetBaseState() = 0;
	virtual bool InStateRange(int state) = 0;
	virtual int StateFromName(SW_BYTE s[], int slen) = 0;
	virtual bool CanBeInterrupted(int state) = 0;
	virtual void SetKeyWords(int n, Literal *keyWordsN) = 0;
	virtual void SetSubLexer(ILexer *lx, Literal *name, Literal *name2) = 0;
	virtual ILexer *GetSubLexer(Literal *name) = 0;
	virtual void SetInputStyles(IStyles *stylesInput) = 0;
	virtual Literal *StateNames() = 0;
	virtual Literal *StateName(int state) = 0;
	virtual ILexer *LexerFromState(int state) = 0;
	virtual void AttachHistories(SplitText *pbuffer, int priority) = 0;
};
