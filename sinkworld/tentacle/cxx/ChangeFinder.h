// SinkWorld ChangeFinder class.
// Copyright 2004 Neil Hodgson
// File written in C~ subset of C++ for automatic translation to Java and C#.

/// Find all positions in a text buffer and set of parallel style buffers where significant 
/// changes occur that may require changes in drawing parameters.
/// This breaks up the text into a sequence of runs that will be drawn separately.

SW_PUBLIC class ChangeFinder {
	SplitText *pbuffer;
	int position;
	int end;
	ChangeLayer *root;

	void InsertSorted(ChangeLayer *pcl);
	int FindNextChange();
	void RemoveLayers();
public:
	/// For \a pbuffer_ and \a ssc_, in the range \a position_ .. \a end, place all the change positions in \a cp.
	ChangeFinder(SplitText *pbuffer_, int position_, int end_, StyleSetCollection *ssc_, SetIntegers *cp);
	~ChangeFinder() {
	}
};
