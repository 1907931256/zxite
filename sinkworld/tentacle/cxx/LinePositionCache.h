// SinkWorld LinePositionCache class.
// Copyright 2004 Neil Hodgson
// File written in C~ subset of C++ for automatic translation to Java and C#.

/// The set of pixel positions for an individual line as stored in a LinePositionCache.
/// Only used by LinePositionCache.

class LineCacheEntry {
public:
	int line;
	TextPositions *positions;
	LineCacheEntry() {
		line = -1;
		positions = NULL;
	}
	~LineCacheEntry() {
		delete positions;
	}
	bool Match(int line_) {
		return line == line_;
	}
	void Set(int line_, TextPositions *positions_) {
		line = line_;
		delete positions;
		positions = positions_;
	}
};

/// Cache of the pixel positions of the characters in a document line.
/// This is a speed optimisation.

SW_PUBLIC class LinePositionCache {
	int size;
	LineCacheEntry **pces;
public:
	LinePositionCache();
	~LinePositionCache();
	void Clear();
	TextPositions *Find(int line);
	void Set(int line, TextPositions *positions);
};
