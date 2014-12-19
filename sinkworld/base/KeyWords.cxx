// SinkWorld KeyWords class.
// Copyright 2001 Neil Hodgson
// File written in C~ subset of C++ for automatic translation to Java and C#.

#include <string.h>
#include <assert.h>
#include <string>

#include "base.h"

KWNode::KWNode(int splitChar_, int setId_) {
	splitChar = splitChar_;
	setId = setId_;
	low = NULL;
	equal = NULL;
	high = NULL;
}

KWNode::~KWNode() {
	delete low;
	delete equal;
	delete high;
}

KWNode *KWNode::Insert(KWNode *p, const int s[], int start, int end, int setId) {
	int sStart = 0;
	if (start < end) {
		sStart = s[start];
	}
	if (p == NULL) {
		p = new KWNode(sStart, setId);
	}
	if (sStart < p->splitChar) {
		p->low = Insert(p->low, s, start, end, setId);
	} else if (sStart == p->splitChar) {
		if (sStart != 0) {
			p->equal = Insert(p->equal, s, start+1, end, setId);
		}
	} else {
		p->high = Insert(p->high, s, start, end, setId);
	}
	return p;
}

int KWNode::Find(KWNode *p, const int s[], int start, int slen) {
	while (p != NULL) {
		if (start == slen) {
			if (0 == p->splitChar) {
				return p->setId;
			} else {
				p = p->low;
			}
		} else if (s[start] < p->splitChar) {
			p = p->low;
		} else if (s[start] == p->splitChar) {
			start++;
			p = p->equal;
		} else {
			p = p->high;
		}
	}
	return 0;
}

KeyWords::KeyWords(bool onlyLineEnds_) {
	onlyLineEnds = onlyLineEnds_;
	maxSet = 0;
	root = NULL;
}

KeyWords::~KeyWords() {
	Clear();
}

void KeyWords::Clear() {
	delete root;
	root = NULL;
	maxSet = 0;
}

int KeyWords::Find(const int s[], int start, int slen) {
	if (slen <= 0)
		return 0;
	if (root == NULL)
		return 0;
	return KWNode::Find(root, s, start, slen);
	// Check for prefixes
	//~ j = starts['^'];
	//~ if (j >= 0) {
		//~ while (list[words[j]] == '^') {
			//~ int a = words[j] + 1;
			//~ int b = 0;
			//~ while ((b < slen) && (list[a] != 0) && (list[a] == s[b + start])) {
				//~ a++;
				//~ b++;
			//~ }
			//~ if (list[a] != 0)
				//~ return j;
			//~ j++;
		//~ }
	//~ }
	//~ return 0;
}

// Insert into tree from centre to avoid degeneration from sorted input
void KeyWords::Build(const int s[], int starts[], int ends[], int b, int e, int setId) {
	if (e > b) {
		int mid = (b+e) / 2;
		root = KWNode::Insert(root, s, starts[mid], ends[mid], setId);
		Build(s, starts, ends, b, mid, setId);
		Build(s, starts, ends, mid+1, e, setId);
	}
}

void KeyWords::Set(const int s[], int slen, int setId) {
	if (maxSet < setId) {
		maxSet = setId;
	}
	// For rapid determination of whether a character is a separator, build
	// a look up table.
	bool wordSeparator[256];
	for (int i=0;i<256; i++) {
		wordSeparator[i] = false;
	}
	wordSeparator[(int)'\r'] = true;
	wordSeparator[(int)'\n'] = true;
	if (!onlyLineEnds) {
		wordSeparator[(int)' '] = true;
		wordSeparator[(int)'\t'] = true;
	}

	bool prevIsSeparator = true;
	int startWord = 0;
	int *starts = new int[slen];
	int *ends = new int[slen];
	int len = 0;
	for (int k = 0; k < slen; k++) {
		// Signed byte -> unsigned for use as an index
		int charIndex = s[k];
		if (charIndex < 0)
			charIndex += 256;
		bool isSeparator = wordSeparator[charIndex];
		if (isSeparator != prevIsSeparator) {
			if (isSeparator) {
				starts[len] = startWord;
				ends[len] = k;
				len++;
			} else {
				startWord = k;
			}
		}
		prevIsSeparator = isSeparator;
	}
	if (!prevIsSeparator && (startWord < slen)) {
		starts[len] = startWord;
		ends[len] = slen;
		len++;
	}
	Build(s, starts, ends, 0, len, setId);
	delete []starts;
	delete []ends;
}

void KeyWords::SetText(Literal *t, int setId) {
	Set(t->Data(), t->Length(), setId);
}
