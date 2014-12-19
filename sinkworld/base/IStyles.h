// SinkWorld IStyles class.
// Copyright 2004 Neil Hodgson
// File written in C~ subset of C++ for automatic translation to Java and C#.

/// The interface that all style buffers implement
SW_PUBLIC class IStyles {
public:
	virtual ~IStyles() {
	}
	virtual int Length() = 0;
	virtual int ValueAt(int position) = 0;
	virtual int FindNextChange(int position, int end) = 0;
	virtual void FillRange(int position, int val, int fillLength) = 0;
	virtual void InsertSpace(int position, int insertLength) = 0;
	virtual void DeleteAll() = 0;
	virtual void DeleteRange(int position, int deleteLength) = 0;
};
