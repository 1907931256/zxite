// SinkWorld SinglePosition class.
// Copyright 2004 Neil Hodgson
// File written in C~ subset of C++ for automatic translation to Java and C#.

/// SinkWorld styles object for holding a position rather than a set of ranges.
/// An example use is for the caret.
/// \note Unlike other types of IStyles there is no automatic change point at the end.

SW_PUBLIC class SinglePosition : public IStyles {
	int lengthBody;
	int index;
public:
	SinglePosition();
	~SinglePosition();
	int Length();
	int ValueAt(int position);
	int FindNextChange(int position, int end);
	void FillRange(int position, int ch, int fillLength);
	void InsertSpace(int position, int insertLength);
	void DeleteAll();
	void DeleteRange(int position, int deleteLength);
	int Get();
};
