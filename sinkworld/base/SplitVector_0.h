// SinkWorld SplitVector_0 class.
// Copyright 2002 Neil Hodgson
// File written in C~ subset of C++ for automatic translation to Java and C#.

/// Degenerate split buffer class using no storage as every position is 0.

SW_PUBLIC class SplitVector_0 : public IStyles {
	int lengthBody;
public:
	SplitVector_0();
	~SplitVector_0();
	int Length();
	int ValueAt(int position);
	int FindNextChange(int position, int end);
	void FillRange(int position, int ch, int fillLength);
	void InsertSpace(int position, int insertLength);
	void DeleteAll();
	void DeleteRange(int position, int deleteLength);
};
