// SinkWorld UndoAction class.
// Copyright 2004 Neil Hodgson
// File written in C~ subset of C++ for automatic translation to Java and C#.

/// One change in an UndoStack.

SW_PUBLIC class UndoAction {
public:
	int actionType;
	int position;
	int lenData;
	SW_BYTE *data;

	UndoAction() {
		actionType = 0;
		position = 0;
		lenData = 0;
		data = NULL;
	}

	~UndoAction() {
		delete []data;
		data = NULL;
	}
};
