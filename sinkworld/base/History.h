// SinkWorld History class.
// Copyright 2004 Neil Hodgson
// File written in C~ subset of C++ for automatic translation to Java and C#.

/// A single item with a history.
/// TODO: Ensure that the text value form teVal works as well as the numeric form

class HistoricEvent {
public:
	HistoricEvent *next;
	HistoricEvent *previous;
	int position;
	int value;
	Literal *teVal;
	bool freeText;
	int depth;
	HistoricEvent(int position_, int value_, Literal *teVal_, int depth_, bool freeText_) {
		next = NULL;
		previous = NULL;
		position = position_;
		value = value_;
		teVal = teVal_;
		freeText = freeText_;
		depth = depth_;
	}
	virtual ~HistoricEvent() {
		if (freeText) {
			delete teVal;
		}
		teVal = NULL;
	}
};

/// Remembers long running state.
/// Can either be a simple list of positions and values or a stack.
/// Used by lexers that need to remember modes.

SW_PUBLIC class History : public IStyles {
private:
	HistoricEvent *first;
	HistoricEvent *last;
	int startValue;
	Literal *teValStart;
	int length;

	void Append(HistoricEvent *he) {
		assert((last == NULL) || (last->position < he->position));
		he->previous = last;
		if (first == NULL) {
			// Empty list
			first = he;
		} else {
			// Non-empty list
			last->next = he;
		}
		last = he;
	}
	
	void Append(int position, int val, Literal *teVal, int depth, bool freeText) {
		Append(new HistoricEvent(position, val, teVal, depth, freeText));
	}

	void SetStartValue(int startValue_, Literal *teValStart_) {
		startValue = startValue_;
		delete teValStart;
		teValStart = NULL;
		if (teValStart_ != NULL) {
			teValStart = new Literal(teValStart_);
		}
	}

	HistoricEvent *FindBefore(int position);
	HistoricEvent *FindAfter(int position);

public:
	History() {
		first = NULL;
		last = NULL;
		startValue = 0;
		teValStart = NULL;
		length = 0;
	}

	History(int startValue_) {
		first = NULL;
		last = NULL;
		startValue = startValue_;
		teValStart = NULL;
		length = 0;
	}

	~History() {
		DeleteAll();
		delete teValStart;
		teValStart = NULL;
	}

	void DeleteAll();

	void Append(int position, int val, Literal *teVal) {
		Append(position, val, teVal, 0, true);
	}

	void Append(int position, int val) {
		Append(position, val, NULL);
	}

	void AppendIfChanged(int position, int val) {
		if (val != Value()) {
			Append(position, val, NULL);
		}
	}

	void Append(int position, Literal *teVal) {
		Append(position, startValue, teVal);
	}

	void Push(int position, int val, Literal *teVal) {
		int depth = Depth();
		Append(position, val, teVal, depth+1, true);
	}

	void Push(int position, int val) {
		Push(position, val, NULL);
	}

	void Push(int position, Literal *teVal) {
		Push(position, startValue, teVal);
	}

	void Pop(int position);
	void InvalidateAfter(int position);
	History *Split(int position);
	bool ChangedHistory(int position, History *historyPast);
	void RepairHistory(int position, History *historyPast);
	int Value();
	Literal *TextValue();
	int ValueAt(int position);
	int Depth();
	int Length();
	int FindNextChange(int position, int end);
	void FillRange(int position, int val, int fillLength);
	void InsertSpace(int position, int insertLength);
	void DeleteRange(int position, int deleteLength);
};

