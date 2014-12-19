// SinkWorld RangeSelection class.
// Copyright 2005 Neil Hodgson
// File written in C~ subset of C++ for automatic translation to Java and C#.

/// A range that the user has selected.
/// Similar in concept to SinglePosition.
/// \note RangeSelection not in base as it may be extended to allow rectangular 
/// and multiple selection

SW_PUBLIC class RangeSelection : public IStyles {
	int caret;
	int anchor;
	int length;
	bool on;
public:
	IStyles *pstyleSelection;

	RangeSelection() {
		caret = 0;
		anchor = 0;
		length = 0;
		on = false;
		pstyleSelection = NULL;
	}

	~RangeSelection() {
		Deallocate();
	}

	void Allocate() {
		caret = 0;
		anchor = 0;
		length = 0;
		on = false;
		delete pstyleSelection;
		pstyleSelection = new RunStyles();
	}

	void Deallocate() {
		caret = 0;
		anchor = 0;
		length = 0;
		on = false;
		delete pstyleSelection;
		pstyleSelection = NULL;
	}

	// IStyles interface required to receive updates from buffer changes
	int Length() {
		return length;
	}

	int ValueAt(int position) {
		return ((position == caret) && on) ? 1 : 0;
	}

	int FindNextChange(int position, int end) {
		if (position < caret) {
			return caret;
		} else {
			return end + 1;
		}
	}

	void FillRange(int position, int val, int fillLength) {
	}

	void InsertSpace(int position, int insertLength) {
		length += insertLength;
		if (caret >= position) {
			caret += insertLength;
		}
		if (anchor >= position) {
			anchor += insertLength;
		}
	}

	void DeleteAll() {
		caret = 0;
		anchor = 0;
		length = 0;
	}

	void DeleteRange(int position, int deleteLength) {
		length -= deleteLength;
		if (caret > position) {
			if (caret < position + deleteLength) {
				caret = position;
			} else {
				caret -= deleteLength;
			}
		}
		if (anchor > position) {
			if (anchor < position + deleteLength) {
				anchor = position;
			} else {
				anchor -= deleteLength;
			}
		}
	}

	// Selection functionality
	int Caret() {
		return caret;
	}

	int Anchor() {
		return anchor;
	}

	int StartSelection() {
		return caret < anchor ? caret : anchor;
	}

	int EndSelection() {
		return caret > anchor ? caret : anchor;
	}

	void SetSelectionStyleRanges(int anchor_, int caret_) {
		if (caret_ < 0)
			caret_ = 0;
		if (anchor_ < 0)
			anchor_ = 0;
		caret = caret_;
		anchor = anchor_;
		pstyleSelection->DeleteAll();
		pstyleSelection->InsertSpace(0, length);
		pstyleSelection->FillRange(StartSelection(), 1, 
			EndSelection() - StartSelection());
	}

	bool Empty() {
		return caret == anchor;
	}

	void SetOn(bool on_) {
		on = on_;
	}

	bool IsOn() {
		return on;
	}
};

