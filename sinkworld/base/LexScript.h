// SinkWorld LexScript class.
// Copyright 2004 Neil Hodgson
// File written in C~ subset of C++ for automatic translation to Java and C#.

/// LexScript is a base class for lexers that support embedded scripts.

SW_PUBLIC class LexScript : public LexBase {
public:
	bool statesAtStart;
	SubLexer *slScriptsRoot;

	LexScript() {
		delete name;
		name = new Literal("Script");
		statesAtStart = true;
		slScriptsRoot = NULL;
	}

	virtual ~LexScript() {
		while (slScriptsRoot != NULL) {
			SubLexer *slToFree = slScriptsRoot;
			slScriptsRoot = slScriptsRoot->slNext;
			delete slToFree;
		}
	}

	int FindSubLexer(const int *s, int len) {
		SubLexer *sl = slScriptsRoot;
		int lexNum=0;
		while (sl != NULL) {
			if (sl->name->IsIn(s, 0, len)) {
				return lexNum;
			} else if (sl->identifiers != NULL) {
				for (int elem=0;elem<=sl->identifiers->Count((SW_BYTE)' ');elem++) {
					Literal *identifier = sl->identifiers->Segment(elem, (SW_BYTE)' ');
					if (identifier->IsIn(s, 0, len)) {
						delete identifier;
						return lexNum;
					}
					delete identifier;
				}
			}
			sl = sl->slNext;
			lexNum++;
		}
		return -1;
	}

	void SwitchToSubLexer(Styler *sc, int lexNum) {
		SubLexer *sl = slScriptsRoot;
		while ((sl != NULL) && (lexNum > 0)) {
			sl = sl->slNext;
			lexNum--;
		}
		if (sl != NULL) {
			sc->SetState(sl->lexer->GetBaseState());
		}
	}

	void ColourisePieceSubLexer(Styler *sc) {
		SubLexer *sl = slScriptsRoot;
		while (sl != NULL) {
			if (sl->lexer->InStateRange(sc->state)) {
				sl->lexer->ColourisePiece(sc);
				break;
			}
			sl = sl->slNext;
		}
	}

	SW_OVERRIDE void InvalidateAfter(int position) {
		SubLexer *sl = slScriptsRoot;
		while (sl != NULL) {
			sl->lexer->InvalidateAfter(position);
			sl = sl->slNext;
		}
	}

	SW_OVERRIDE void AllocateStates(int startAllocation) {
		int allocatedState = startAllocation;
		baseState = allocatedState;
		if (statesAtStart) {
			allocatedState += thisStates;
		}
		SubLexer *sl = slScriptsRoot;
		while (sl != NULL) {
			subStates += sl->lexer->States();
			sl->lexer->SetBaseState(allocatedState);
			allocatedState += sl->lexer->States();
			sl = sl->slNext;
		}
		if (!statesAtStart) {
			allocatedState += thisStates;
		}
	}

	SW_OVERRIDE void SetSubLexer(ILexer *lx, Literal *name, Literal *name2) {
		SubLexer *sl = new SubLexer(lx, name, name2);
		if (slScriptsRoot != NULL) {
			SubLexer *slLast = slScriptsRoot;
			while (slLast->slNext != NULL) {
				slLast = slLast->slNext;
			}
			slLast->slNext = sl;
		} else {
			slScriptsRoot = sl;
		}
		AllocateStates(baseAllocation);
	}

	SW_OVERRIDE ILexer *GetSubLexer(Literal *name) {
		for (SubLexer *sl=slScriptsRoot; sl != NULL; sl = sl->slNext) {
			if (name->Equals(sl->name)) {
				return sl->lexer;
			}
		}
		return NULL;
	}

	SW_OVERRIDE Literal *StateName(int state) {
		for (SubLexer *sl=slScriptsRoot; sl != NULL; sl = sl->slNext) {
			if (sl->lexer->InStateRange(state)) {
				return sl->lexer->StateName(state);
			}
		}
		return stateNames->Segment(state - baseState, ' ');
	}

	SW_OVERRIDE ILexer *LexerFromState(int state) {
		for (SubLexer *sl=slScriptsRoot; sl != NULL; sl = sl->slNext) {
			if (sl->lexer->InStateRange(state)) {
				return sl->lexer;
			}
		}
		return NULL;
	}

	SW_OVERRIDE void AttachHistories(SplitText *pbuffer, int priority) {
		for (SubLexer *sl=slScriptsRoot; sl != NULL; sl = sl->slNext) {
			sl->lexer->AttachHistories(pbuffer, priority);
		}
	}
};
