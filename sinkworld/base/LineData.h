// SinkWorld LineData class.
// Copyright 2004 Neil Hodgson
// File written in C~ subset of C++ for automatic translation to Java and C#.

/// Objects derived from LineData can be attached to a SplitText 
/// object and they will receive InsertLine and RemoveLine calls
/// as these operations are performed on the SplitText.
/// They can thus be used for data that has to stick to lines such as 
/// markers or folding state.
SW_PUBLIC SW_ABSTRACT class LineData {
public:
	LineData *next;
	LineData() {
		next = NULL;
	}
	virtual ~LineData() {
	}
	virtual void Clear()=0;
	virtual void InsertLine(int position)=0;
	virtual void RemoveLine(int position)=0;
};
