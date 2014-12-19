// SinkWorld KeyWords class.
// Copyright 2004 Neil Hodgson
// File written in C~ subset of C++ for automatic translation to Java and C#.

/// KWNode is used internally by KeyWords.

class KWNode {
	int splitChar;
	int setId;
	KWNode *low;
	KWNode *equal;
	KWNode *high;
public:
	KWNode(int splitChar_, int setId_);
	~KWNode();
	static KWNode *Insert(KWNode *p, const int s[], int start, int end, int setId);
	static int Find(KWNode *p, const int s[], int start, int slen);
};

/// Holds multiple sets of keywords and returns the set number
/// given an input string or 0 if not found.
/// The data structure is a trie.

SW_PUBLIC class KeyWords {
	KWNode *root;
	bool onlyLineEnds;	///< Delimited by any white space or only line ends
	int maxSet;
	void Build(const int s[], int starts[], int ends[], int b, int e, int setId);
	void Set(const int s[], int slen, int setId);
public:
	KeyWords(bool onlyLineEnds_);
	~KeyWords();
	void Clear();
	void SetText(Literal *t, int setId);
	int Find(const int s[], int start, int slen);
	int Sets() {
		return maxSet;
	}
};

