// SinkWorld WordList class.
// Copyright 2001 Neil Hodgson
// File written in C~ subset of C++ for automatic translation to Java and C#.

#include <string.h>
#include <assert.h>
#include <string>

#include "base.h"

WordList::WordList(bool onlyLineEnds_ = false) {
	words = NULL;
	list = NULL;
	nWords = 0;
	onlyLineEnds = onlyLineEnds_;
	sorted = false;
}

WordList::~WordList() {
	Clear();
}

void WordList::Clear() {
	delete []list;
	list = NULL;
	delete []words;
	words = NULL;
	nWords = 0;
	sorted = false;
}

void WordList::Set(const int s[], int slen) {
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

	// Count the words first so can allocate the exact number needed.
	nWords = 0;
	bool prevSeparator = true;
	for (int j = 0; j<slen; j++) {
		// Signed byte -> unsigned for use as an index
		int charIndex = s[j];
		if (charIndex < 0)
			charIndex += 256;
		bool isSeparator = wordSeparator[charIndex];
		if (prevSeparator && !isSeparator) {
			nWords++;
		}
		prevSeparator = isSeparator;
	}

	list = new int[slen + 1];
	list[slen] = 0;
	sorted = false;

	// Copy the list and point 'words' at each word in the list.
	words = new int[nWords + 1];
	if (words != NULL) {
		bool prevIsSeparator = true;
		int w = 0;
		for (int k = 0; k < slen; k++) {
			// Signed byte -> unsigned for use as an index
			int charIndex = s[k];
			if (charIndex < 0)
				charIndex += 256;
			bool isSeparator = wordSeparator[charIndex];
			if (isSeparator) {
				list[k] = 0;
			} else {
				list[k] = s[k];
				if (prevIsSeparator) {
					words[w] = k;
					w++;
				}
			}
			prevIsSeparator = isSeparator;
		}
		// Terminate with an empty word.
		words[w] = slen;
	}
}

int WordList::Compare(int a, int b) {
	// Can't work out the correct incantation to use modern casts here
	while ((list[a] != 0) && (list[a] == list[b])) {
		a++;
		b++;
	}
	return list[a] - list[b];
}

void WordList::SortWordList() {
	// Do a shell sort as it is quick to code
	for (int g=nWords / 2; g > 0; g /= 2) {
		for (int i=g; i< nWords; i++) {
			for (int j = i-g; j >= 0 && Compare(words[j], words[j+g]) > 0; j -= g) {
				int t = words[j];
				words[j] = words[j+g];
				words[j+g] = t;
			}
		}
	}
}

void WordList::EnsureSorted() {
	if (!sorted) {
		sorted = true;
		SortWordList();
		for (int k = 0; k < 256; k++)
			starts[k] = -1;
		for (int l = nWords - 1; l >= 0; l--) {
			int indexChar = list[words[l]];
			if (indexChar < 0)
				indexChar += 256;
			starts[indexChar] = l;
		}
	}
}

int WordList::Find(const int s[], int start, int slen) {
	if (words == NULL)
		return -1;
	EnsureSorted();
	int firstChar = s[0 + start];
	int firstCharIndex = firstChar;
	if (firstCharIndex < 0)
		firstCharIndex += 256;
	int j = starts[firstCharIndex];
	if (j >= 0) {
		while (list[words[j]] == firstChar) {
			int a = words[j];
			int b = 0;
			while ((b < slen) && (list[a] != 0) && (list[a] == s[b + start])) {
				a++;
				b++;
			}
			if ((b == slen) && (list[a] == 0))
				return j;
			j++;
		}
	}
	// Check for prefixes
	j = starts[(int)'^'];
	if (j >= 0) {
		while (list[words[j]] == '^') {
			int a = words[j] + 1;
			int b = 0;
			while ((b < slen) && (list[a] != 0) && (list[a] == s[b + start])) {
				a++;
				b++;
			}
			if (list[a] != 0)
				return j;
			j++;
		}
	}
	return -1;
}

void WordList::SetText(Literal *t) {
	Set(t->Data(), t->Length());
}

bool WordList::InList(const int s[], int start, int slen) {
	if (slen <= 0)
		return false;
	else
		return Find(s, start, slen) >= 0;
}

int WordList::Length() {
	EnsureSorted();
	return nWords;
}

Literal *WordList::GetText(int n) {
	EnsureSorted();
	int start = words[n];
	int end = start;
	while (list[end] != 0) {
		end++;
	}
	return new Literal(list, start, end-start);
}
