// SinkWorld SymbolTable class.
// Copyright 2004 Neil Hodgson
// File written in C~ subset of C++ for automatic translation to Java and C#.

/// A change in value in an incremental symbol table.

class SymbolEvent {
public:
	SymbolEvent *next;
	SymbolEvent *previous;
	int position;
	Literal *teKey;
	Literal *teVal;
	SymbolEvent(int position_, Literal *teKey_, Literal *teVal_) {
		next = NULL;
		previous = NULL;
		position = position_;
		teKey = new Literal(teKey_, 0, teKey_->Length());
		if (teVal_ != NULL) {
			teVal = new Literal(teVal_, 0, teVal_->Length());
		} else {
			teVal = NULL;
		}
	}
	virtual ~SymbolEvent() {
		delete teKey;
		delete teVal;
	}
	bool MatchesBefore(int position_, Literal *teKey_) {
		return (position_ > position) && teKey_->Equals(teKey);
	}
};

/// Stores an incremental symbol table.

SW_PUBLIC class SymbolTable {
private:
	SymbolEvent *first;
	SymbolEvent *last;
	void DeleteAll() {
		while (first != NULL) {
			SymbolEvent *next = first->next;
			delete first;
			first = next;
		}
		last = NULL;
	}
	void Append(SymbolEvent *se) {
		se->previous = last;
		if (first == NULL) {
			// Empty list
			first = se;
		} else {
			// Non-empty list
			last->next = se;
		}
		last = se;
	}
public:
	SymbolTable() {
		first = NULL;
		last = NULL;
	}

	~SymbolTable() {
		DeleteAll();
	}

	void Append(int position, Literal *teKey_, Literal *teVal_) {
		Append(new SymbolEvent(position, teKey_, teVal_));
	}

	void InvalidateAfter(int position) {
		if (first != NULL) {
			if (position < first->position) {
				DeleteAll();
			} else {
				while (position < last->position) {
					SymbolEvent *toFree = last;
					last = last->previous;
					delete toFree;
				}
				last->next = NULL;
				if (last->previous != NULL) {
					last->previous->next = last;
				}
			}
		}
	}

	Literal *GetValue(int position, Literal *teKey) {
		SymbolEvent *se = last;
		while (se != NULL) {
			if (se->MatchesBefore(position, teKey)) {
				return se->teVal;
			}
			se = se->previous;
		}
		return NULL;
	}
};
