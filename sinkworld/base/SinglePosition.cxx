// SinkWorld SinglePosition class.
// Copyright 2004 Neil Hodgson
// File written in C~ subset of C++ for automatic translation to Java and C#.

#include <string.h>
#include <assert.h>
#include <string>

#include "base.h"

SinglePosition::SinglePosition() {
	lengthBody = 0;
	index = 0;
}

SinglePosition::~SinglePosition() {
}

int SinglePosition::Length() {
	return lengthBody;
}

int SinglePosition::ValueAt(int position) {
	return 0;
}

int SinglePosition::FindNextChange(int position, int end) {
	if (position < index) {
		return index;
	} else {
		// Return value out of range to indicate no more changes not even at end
		return end + 1;
	}
}

void SinglePosition::FillRange(int position, int ch, int fillLength) {
	// As a single position can not be a range this must fail
	assert(fillLength == 0);
	index = position;
}

void SinglePosition::InsertSpace(int position, int insertLength) {
	lengthBody += insertLength;
	if (index >= position) {
		index += insertLength;
	}
}

void SinglePosition::DeleteAll() {
	lengthBody = 0;
	index = 0;
}

void SinglePosition::DeleteRange(int position, int deleteLength) {
	lengthBody -= deleteLength;
	if (index > position) {
		if (index < position + deleteLength) {
			index = position;
		} else {
			index -= deleteLength;
		}
	}
}

int SinglePosition::Get() {
	return index;
}
