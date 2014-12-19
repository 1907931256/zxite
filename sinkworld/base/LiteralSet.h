// SinkWorld LiteralSet class.
// Copyright 2005 Neil Hodgson
// File written in C~ subset of C++ for automatic translation to Java and C#.

/// Manage a set of relatively constant text strings.
/// Current use is for font names as it is much easier
/// to have a set of immortal names than to allocate
/// memory to hold copies.

SW_PUBLIC class LiteralSet : public Dictionary {
public:
	LiteralSet() {
	}
	~LiteralSet() {
		while (Used() > 0) {
			DeleteOne();
		}
	}

	Literal *FindOrAddKey(Literal *key) {
		Literal *keyFound = FindKey(key);
		if (keyFound != NULL) {
			return keyFound;
		} else {
			Add(key, NULL);
			return FindKey(key);
		}
	}
};
