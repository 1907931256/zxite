// SinkWorld Styler class.
// Copyright 2004 Neil Hodgson
// File written in C~ subset of C++ for automatic translation to Java and C#.

/// Lexers read from a text buffer and write to a style buffer using this intermediary.

SW_PUBLIC class Styler {
private:
	int currentPosition;
	int endPosition;
	int currentWidth;
	int nextWidth;
	SplitText *document;
	IStyles *style;

	int *word;
	int wordLength;
	int wordSize;

public:
	int startSeg;
	bool atLineStart;
	bool atLineEnd;

	bool folding;
	int lineCurrent;
	int levelCurrent;
	int levelNext;

	int state;
	int stateAtEnd;
	bool changeBeyondEnd;
	int chPrev;
	int ch;
	int chNext;

	Styler(SplitText *document_, IStyles *style_, int startPos, int length, bool folding_) {
		currentPosition = startPos;
		currentWidth = 0;
		nextWidth = 0;
		endPosition = startPos + length;
		startSeg = currentPosition;
		document = document_;

		style = style_;

		folding = folding_;
		if (folding) {
			lineCurrent = document->LineFromPosition(currentPosition);
			if (lineCurrent == 0) {
				levelCurrent = 100;
				levelNext = 100;
			} else {
				int levels = style->ValueAt(lineCurrent-1);
				levelCurrent = levels & 0xFFFF;
				levelNext = levels >> 16;
			}
		}

		atLineStart = true;
		atLineEnd = true;
		if (!folding && (currentPosition > 0))
			state = style->ValueAt(currentPosition-1);
		else
			state = 0;
		stateAtEnd = 0;
		if (!folding && (endPosition > 0))
			stateAtEnd = style->ValueAt(endPosition - 1);
		changeBeyondEnd = false;
		chPrev = 0;
		ch = 0;
		chNext = 0;
		word = NULL;
		wordLength = 0;
		wordSize = 0;

		Forward();
		Forward();
	}

	~Styler() {
		delete []word;
		word = NULL;
	}

	void Complete() {
		if (folding) {
			style->FillRange(lineCurrent, levelCurrent | levelNext << 16, 1);
			lineCurrent++;
			levelCurrent = levelNext;
		} else {
			ColourTo(endPosition - 1, state);
			changeBeyondEnd = changeBeyondEnd ||
				(endPosition <= 0) ||
				(stateAtEnd != style->ValueAt(endPosition - 1));
		}
	}

	bool More() {
		return currentPosition < endPosition;
	}

	void Forward() {
		if (currentPosition < endPosition) {
			atLineStart = atLineEnd;
			if (folding) {
				if (atLineStart && (currentWidth > 0)) {	// TODO: work out how this copes with empty first line
					style->FillRange(lineCurrent, levelCurrent | levelNext << 16, 1);
					lineCurrent++;
					levelCurrent = levelNext;
				}
			}

			chPrev = ch;
			currentPosition += currentWidth;
			currentWidth = nextWidth;
			ch = chNext;
			int nextPosition = currentPosition + nextWidth;
			if (nextPosition < document->Length()) {
				// This is a little tricky as it jumps over the \n in a \r\n
				nextWidth = document->DistanceNextCaret(nextPosition);
				chNext = document->CharValue(nextPosition);
			} else {
				nextWidth = 1;	// Could be 0 but may lead to infinite loops.
				chNext = ' ';
			}
			// Trigger on CR only (Mac style) or either on LF from CR+LF (Dos/Win) or on LF alone (Unix)
			// Avoid triggering two times on Dos/Win
			// End of line
			if (currentWidth > 0)
				// Due to use of DistanceNextCaret above, no need to check for \r\n.
				atLineEnd = (ch == '\r') || (ch == '\n') || (currentPosition >= endPosition);
				//atLineEnd = (ch == '\r' && chNext != '\n') || (ch == '\n') || (currentPosition >= endPosition);
		} else {
			atLineStart = false;
			chPrev = ' ';
			ch = ' ';
			chNext = ' ';
			atLineEnd = true;
		}
	}

	void ChangeState(int state_) {
		state = state_;
	}

	void SetState(int state_) {
		ColourTo(currentPosition - 1, state);
		state = state_;
	}

	void ForwardSetState(int state_) {
		Forward();
		ColourTo(currentPosition - 1, state);
		state = state_;
	}

	void ChangeFoldLevel(int levelChange) {
		levelNext += levelChange;
	}

	bool Match(char ch0) {
		return ch == ch0;
	}

	bool Match(char ch0, char ch1) {
		return (ch == ch0) && (chNext == ch1);
	}

	bool Match(Literal *s) {
		if (currentPosition + s->Length() > document->Length())
			return false;
		int posInMatch = currentPosition;
		for (int n=0; n < s->Length(); n++) {
			if (s->CharAt(n) != document->CharValue(posInMatch))
				return false;
			posInMatch += document->LenChar(posInMatch);
		}
		return true;
	}

	bool MatchIgnoreCase(Literal *s) {
		if (currentPosition + s->Length() > document->Length())
			return false;
		int posInMatch = currentPosition;
		for (int n=0; n < s->Length(); n++) {
			if (s->CharAt(n) !=
				MakeLowerCase(document->CharValue(posInMatch)))
				return false;
			posInMatch += document->LenChar(posInMatch);
		}
		return true;
	}

	int GetRelative(int n) {
		int pos = currentPosition;
		for (int i=0; i<n && (pos < document->Length()); i++) {
			pos += document->LenChar(pos);
		}
		if (pos < document->Length())
			return document->CharValue(pos);
		else
			return (int)' ';
	}

	Literal *GetCurrent() {
		int characters = document->Characters(startSeg, currentPosition - startSeg);
		int *s = new int[characters];
		document->RetrieveUTF32(startSeg, s, characters);
		Literal *lit = new Literal(s, 0, characters);
		delete []s;
		return lit;
	}

	Literal *GetCurrentLowered() {
		int characters = document->Characters(startSeg, currentPosition - startSeg);
		int *s = new int[characters];
		document->RetrieveUTF32(startSeg, s, characters);
		for (int n=0; n < characters; n++) {
			s[n] = MakeLowerCase(s[n]);
		}
		Literal *lit = new Literal(s, 0, characters);
		delete []s;
		return lit;
	}

	// Character class helpers

	static bool IsDigit(int ch) {
		return ch >= '0' && ch <= '9';
	}

	static bool IsAlphaNumeric(int ch) {
		return
			(ch >= 'a' && ch <= 'z') ||
			(ch >= 'A' && ch <= 'Z') ||
			(ch >= '0' && ch <= '9');
	}

	static bool IsSpaceChar(int ch) {
		return (ch == ' ') || (ch == '\t') || (ch == '\r') ||
			(ch == '\n') || (ch == '\f');
	}

	static int MakeLowerCase(int ch) {
		if (ch < 'A' || ch > 'Z')
			return ch;
		else
			return (SW_BYTE)(ch - 'A' + 'a');
	}

	int Position() {
		return currentPosition;
	}

	int End() {
		return endPosition;
	}

	void SetWord() {
		int start = startSeg;
		int characters = document->Characters(start, currentPosition - start);
		if (characters > wordSize) {
			delete []word;
			word = new int[characters];
			wordSize = characters;
		}
		wordLength = characters;
		document->RetrieveUTF32(start, word, characters);
	}

	int WordInList(KeyWords *kw) {
		return kw->Find(word, 0, wordLength);
	}

	bool WordIs(Literal *lit) {
		if (wordLength != lit->Length()) {
			return false;
		}
		for (int i = 0; i < wordLength; i++) {
			if (word[i] != lit->CharAt(i)) {
				return false;
			}
		}
		return true;
	}

private:

	SW_BYTE XCharAt(int position) {
		return (SW_BYTE)(document->CharValue(position));
	}

	void ColourTo(int position, int state) {
		if (position >= startSeg) {
			style->FillRange(startSeg, state, position - startSeg + 1);
			startSeg = position + 1;
		}
	}
};
