// SinkWorld RunStyles class.
// Copyright 2004 Neil Hodgson
// File written in C~ subset of C++ for automatic translation to Java and C#.

/// SinkWorld styling buffer using one element for each run rather than using
/// a filled buffer.

class RunStyles : public IStyles {
public:
	Partitioning *starts;
	SplitVector_4 *styles;
	int RunFromPosition(int position);
	int SplitRun(int position);
	void RemoveRun(int run);
	void RemoveRunIfEmpty(int run);
	void RemoveRunIfSameAsPrevious(int run);
public:
	RunStyles();
	~RunStyles();
	int Length();
	int ValueAt(int position);
	int FindNextChange(int position, int end);
	void FillRange(int position, int ch, int fillLength);
	void IncrementRange(int position, int value, int fillLength);
	void InsertSpace(int position, int insertLength);
	void DeleteAll();
	void DeleteRange(int position, int deleteLength);
	int Runs() {
		return starts->Partitions();
	}
};
