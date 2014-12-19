// SinkWorld ChangePositionsCache class.
// Copyright 2004 Neil Hodgson
// File written in C~ subset of C++ for automatic translation to Java and C#.

/// The set of change positions for an individual line as stored in a ChangePositionsCache.
/// Only used by ChangePositionsCache.

class ChangePositionsEntry {
public:
	int line;
	SetIntegers *cf;
	ChangePositionsEntry() {
		line = -1;
		cf = NULL;
	}
	~ChangePositionsEntry() {
		delete cf;
		cf = NULL;
	}
	bool Match(int line_) {
		return line == line_;
	}
	void Set(int line_, SetIntegers *cf_) {
		delete cf;
		line = line_;
		cf = cf_;
	}
};

/// Cache of the change positions for some of the lines in a document.
/// This is a speed optimisation.

SW_PUBLIC class ChangePositionsCache {
	int size;
	ChangePositionsEntry **pces;
public:
	ChangePositionsCache();
	~ChangePositionsCache();
	void Clear();
	SetIntegers *Find(int line);
	void Set(int line, SetIntegers *cf);
};

