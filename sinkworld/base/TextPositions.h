// SinkWorld TextPositions class.
// Copyright 2004 Neil Hodgson
// File written in C~ subset of C++ for automatic translation to Java and C#.

/** Holds an accumulating set of integer positions that can be expanded.
 *   @note In the future, this class may change to use floating point positions
 *   and may hold a sparse array of positions that can be filled in as
 *   needed by a measurement object. It may also have to deal with
 *   bidirectional text.
 */

SW_PUBLIC class TextPositions {
	int *body;
	int size;
	int lengthBody;
public:
	TextPositions() {
		size = 80;	// Reasonable initial line length
		body = new int[size];
		body[0] = 0;
		lengthBody = 1;
	}
	TextPositions(int size_) {
		size = size_;
		body = new int[size];
		body[0] = 0;
		lengthBody = 1;
	}
	TextPositions(const TextPositions &orig) {
		size = orig.lengthBody;
		body = new int[size];
		lengthBody = orig.lengthBody;
		for (int j=0;j<lengthBody;j++)
			body[j] = orig.body[j];
	}
	~TextPositions() {
		delete []body;
		body = NULL;
	}
	//! Append \a val to the end.
	void Add(int val) {
		if (lengthBody == size) {
			int *newBody = new int[size * 2];
			for (int j=0;j<lengthBody;j++) {
				newBody[j] = body[j];
			}
			size = size * 2;
			delete []body;
			body = newBody;
		}
		body[lengthBody++] = val;
	}
	/// Append all elements from \a other to the end.
	void Append(const TextPositions *other) {
		int start = LastPosition();
		for (int j=1; j<other->Last(); j++) {
			Add(start + other->body[j]);
		}
	}
	/// Replicate the last element up to the new end position \a len
	void FillTo(int len) {
		assert(len >= lengthBody);
		int lastPos = (lengthBody == 0) ? 0 : body[lengthBody-1];
		while (lengthBody<len) {
			Add(lastPos);
		}
	}
	/// Retrieve the position at \a index
	int At(int index) const {
		return body[index];
	}
	/// Position halfway between the position at \a index and the next position
	int AtMiddle(int index) const {
		return (body[index] + body[index+1]) / 2;
	}
	/// Length of the set
	int Last() const {
		return lengthBody;
	}
	/// Last position of the set
	int LastPosition() const {
		return body[lengthBody-1];
	}
};
