// SinkWorld WordList class.
// Copyright 2001 Neil Hodgson
// File written in C~ subset of C++ for automatic translation to Java and C#.

/// Used for holding sets of keywords.

SW_PUBLIC class WordList {
	// Each word contains at least one character - a empty word acts as sentinel at the end.
	int *words;
	int *list;
	int nWords;
	bool onlyLineEnds;	///< Delimited by any white space or only line ends
	bool sorted;
	int starts[256];
	int Compare(int a, int b);
	void SortWordList();
	void EnsureSorted();
public:
	WordList(bool onlyLineEnds_);
	~WordList();
	void Clear();
	void Set(const int s[], int slen);
	void SetText(Literal *t);
	int Find(const int s[], int start, int slen);
	bool InList(const int s[], int start, int slen);
	int Length();
	Literal *GetText(int n);
};

