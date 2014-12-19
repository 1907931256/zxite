// SinkWorld SetIntegers class.
// Copyright 2004 Neil Hodgson
// File written in C~ subset of C++ for automatic translation to Java and C#.

/// Hold a set of integers that are accumulated.
/// Maximum size is fixed at creation.

SW_PUBLIC class SetIntegers {
	int *body;
	int size;
	int lengthBody;
public:
	SetIntegers(int length_)  {
		size = length_;
		body = new int[size];
		lengthBody = 0;
	}
	~SetIntegers() {
		delete []body;
		body = NULL;
	}
	void Add(int v) {
		body[lengthBody++] = v;
	}
	int Length() const {
		return lengthBody;
	}
	int At(int index) const {
		return body[index];
	}
};

