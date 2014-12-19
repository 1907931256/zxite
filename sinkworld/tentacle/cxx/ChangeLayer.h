// SinkWorld ChangeLayer class.
// Copyright 2004 Neil Hodgson
// File written in C~ subset of C++ for automatic translation to Java and C#.

/// Objects derived from ChangeLayer are used by a ChangeFinder to iterate through 
/// a buffer finding positions of significant changes.

SW_PUBLIC SW_ABSTRACT class ChangeLayer {
public:
	ChangeLayer *next;
	int position;
	int layer;
	ChangeLayer() {
		next = NULL;
		position = 0;
		layer = 0;
	}
	virtual void FindNextChange(int end) = 0;
};
