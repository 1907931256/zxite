// SinkWorld SplitVector_0 class.
// Copyright 2002 Neil Hodgson
// File written in C~ subset of C++ for automatic translation to Java and C#.

#include <string.h>
#include <assert.h>
#include <string>

#include "base.h"

SplitVector_0::SplitVector_0() {
	lengthBody = 0;
}

SplitVector_0::~SplitVector_0() {
}

int SplitVector_0::Length() {
	return lengthBody;
}

int SplitVector_0::ValueAt(int position) {
	return 0;
}

int SplitVector_0::FindNextChange(int position, int end) {
	if (position < end)
		return end;
	else
		return end + 1;
}

void SplitVector_0::FillRange(int position, int ch, int fillLength) {
}

void SplitVector_0::InsertSpace(int position, int insertLength) {
	lengthBody += insertLength;
}

void SplitVector_0::DeleteAll() {
	lengthBody = 0;
}

void SplitVector_0::DeleteRange(int position, int deleteLength) {
	lengthBody -= deleteLength;
}
