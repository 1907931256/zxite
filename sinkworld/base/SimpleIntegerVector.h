// SinkWorld SimpleIntegerVector class.
// Copyright 2004 Neil Hodgson
// File written in C~ subset of C++ for automatic translation to Java and C#.

/// Simple expandable integer vector.

SW_PUBLIC class SimpleIntegerVector {
private:
	int *body;

protected:
	int size;
	int lengthBody;
	int growSize;

	void Expand(int newSize) {
		int *newBody = new int[newSize];
		if (body != NULL)
			SW_ARRAYCOPY(body, 0, newBody, 0, lengthBody);
		delete []body;
		body = newBody;
		size = newSize;
	}

public:
	SimpleIntegerVector() {
		body = NULL;
		size = 0;
		lengthBody = 0;
		growSize = 8;
	}

	~SimpleIntegerVector() {
		delete []body;
		body = NULL;
	}

	void SetGrowSize(int growSize_) {
		growSize = growSize_;
	}

	int Length() {
		return lengthBody;
	}

	void SetValueAt(int position, int v) {
		body[position] = v;
	}

	void SetValueAllowingExpansion(int position, int v) {
		if (position >= size) {
			if (growSize * 6 < size)
				growSize *= 2;
			Expand(position + growSize);
		}
		if (lengthBody <= position) {
			lengthBody = position + 1;
		}
		body[position] = v;
	}

	int ValueAt(int position) {
		return body[position];
	}

	void Insert(int position, int v) {
		if ((position < 0) || (position > lengthBody)) {
			throw new BadPositionException();
		}
		if ((lengthBody + 1) >= size) {
			if (growSize * 6 < size)
				growSize *= 2;
			Expand(size + growSize);
		}
		lengthBody++;
		for (int i = lengthBody-1; i > position; i--) {
			body[i] = body[i - 1];
		}
		body[position] = v;
	}

	void Delete(int position) {
		if ((position < 0) || (position >= lengthBody)) {
			throw new BadPositionException();
		}
		for (int i = position; i < lengthBody-1; i++) {
			body[i] = body[i + 1];
		}
		lengthBody--;
	}

	void RangeAddDelta(int start, int end, int delta) {
		for (int i = start; i < end; i++) {
			body[i] += delta;
		}
	}
};
