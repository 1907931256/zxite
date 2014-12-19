// SinkWorld MaxFinder class.
// Copyright 2005 Neil Hodgson
// File written in C~ subset of C++ for automatic translation to Java and C#.

/// Hold a set of integers so the largest element can be found quickly.
/// This is useful for holding the widths of lines so that the range of the
/// horizontal scroll bar can be quickly determined.
/// The data structure used is a binary heap
/// See http://www.informatics.susx.ac.uk/courses/dats/notes/html/node77.html

SW_PUBLIC class MaxFinder {
	int *body;
	int size;
	int lengthBody;
	int minimum;	///< The smallest value of interest.
public:
	MaxFinder();
	~MaxFinder();
	/// Change size to \a sizeNew elements.
	/// \warning Must not be used to decrease the size of the buffer to
	/// be smaller than current length.
	void Allocate(int sizeNew);
	/// Change minimum interesting value to \a v.
	/// \warning Must be empty when calling this to avoid having invalid entries.
	void SetMinimum(int v);
	int GetMinimum();
	void Add(int v);
	void Remove(int v);
	void Replace(int vOld, int vNew);
	int Length() const;
	int Maximum() const;
};
