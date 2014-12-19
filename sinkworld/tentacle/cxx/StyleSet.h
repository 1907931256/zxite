// SinkWorld StyleSet class.
// Copyright 2004 Neil Hodgson
// File written in C~ subset of C++ for automatic translation to Java and C#.

/// An array of StyleModification objects mapping values in buffer to visual 
/// appearance of text.

SW_PUBLIC class StyleSet {
public:
	int len;
	StyleModification **styles;
	IStyles *buffer;
	int specified;
	bool specifiedValid;
	StyleSet(IStyles *buffer_, int dim) {
		buffer = buffer_;
		len = dim;
		styles = new StyleModification*[len];
		for (int i=0;i<len;i++) {
			styles[i] = NULL;
		}
		specified = Style::sdNone;
		specifiedValid = false;
	}
	~StyleSet() {
		for (int i=0;i<len;i++) {
			delete styles[i];
			styles[i] = NULL;
		}
		delete []styles;
		styles = NULL;
	}
	void Set(int i, StyleModification *s) {
		delete styles[i];
		styles[i] = s;
		specified = Style::sdNone;
		specifiedValid = false;
	}
	StyleModification *Get(int i) {
		if (i >= len)
			return NULL;
		else
			return styles[i];
	}
	void SetSpecifiedFlags() {
		if (!specifiedValid) {
			specified = Style::sdNone;
			for (int i=0;i<len;i++) {
				if (styles[i] != NULL) {
					specified = specified | styles[i]->specified;
				}
			}
		}
	}
	int SpecifiedFlags() {
		return specified;
	}
	IStyles *GetBuffer() {
		return buffer;
	}
};
