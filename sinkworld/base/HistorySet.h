// SinkWorld HistorySet class.
// Copyright 2005 Neil Hodgson
// File written in C~ subset of C++ for automatic translation to Java and C#.

/// A list of histories.

SW_PUBLIC class HistorySet {
	void ReAllocate(int newSize) {
		if (newSize == 0) {
			newSize = 1;
		}
		History **newHistories = new History*[newSize];
		for (int i=0; i<newSize; i++) {
			newHistories[i] = NULL;
			if ((histories != NULL) && (i < len)) {
				newHistories[i] = histories[i];
			}
		}
		delete []histories;
		histories = newHistories;
		len = newSize;
	}

public:
	int len;
	History **histories;
	HistorySet(int dim) {
		len = dim;
		histories = new History*[len];
		for (int i=0;i<len;i++) {
			histories[i] = NULL;
		}
	}
	~HistorySet() {
		for (int i=0;i<len;i++) {
			histories[i] = NULL;
		}
		delete []histories;
		histories = NULL;
	}
	void Set(int i, History *s) {
		histories[i] = s;
	}
	void AddHistory(History *s) {
		ReAllocate(len+1);
		histories[len-1] = s;
	}
	History *Get(int i) {
		if (i >= len)
			return NULL;
		else
			return histories[i];
	}
};
