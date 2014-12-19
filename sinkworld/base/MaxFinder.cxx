// SinkWorld MaxFinder class.
// Copyright 2005 Neil Hodgson
// File written in C~ subset of C++ for automatic translation to Java and C#.

#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <string>

#include "base.h"

MaxFinder::MaxFinder()  {
	size = 0;
	body = NULL;
	lengthBody = 0;
	minimum = 0;
	Allocate(4);
}

MaxFinder::~MaxFinder() {
	delete []body;
	body = NULL;
}

void MaxFinder::Allocate(int sizeNew) {
	assert(sizeNew >= lengthBody);
	int *newBody = new int[sizeNew];
	if (body != NULL)
		SW_ARRAYCOPY(body, 0, newBody, 0, lengthBody);
	delete []body;
	body = newBody;
	size = sizeNew;
}

void MaxFinder::SetMinimum(int v) {
	assert(lengthBody == 0);
	minimum = v;
}

int MaxFinder::GetMinimum() {
	return minimum;
}

void MaxFinder::Add(int v) {
	if (v <= minimum)
		return;
	if (lengthBody >= size) {
		Allocate(size*2);
	}
	int child = lengthBody++;
	int parent = (child - 1) / 2;
	while ((child > 0) && (body[parent] < v)) {
		body[child] = body[parent];
		child = parent;
		parent = (child - 1) / 2;
	}
	body[child] = v;
}

void MaxFinder::Remove(int v) {
	if (v <= minimum)
		return;
	//~ if (lengthBody == 0) {
		//~ throw 2;
	//~ }
	int child = -1;
	for (int z=0;(z<lengthBody) && (child < 0);z++) {
		if (body[z] == v)
			child = z;
	}
	assert(child >= 0);
	assert(body[child] == v);
	body[child] = body[0] + 1;	// Ensure largest so becomes top
	int parent = (child - 1) / 2;
	while ((child > 0) && (body[parent] < body[child])) {
		int t = body[child];
		body[child] = body[parent];
		body[parent] = t;
		child = parent;
		parent = (child - 1) / 2;
	}

	int item = body[--lengthBody];
	while ((child = (2 * parent) + 1) < lengthBody) {
		if ((child + 1 < lengthBody) && (body[child] < body[child + 1])) {
			++child;
		}
		if (item < body[child]) {
			body[parent] = body[child];
			parent = child;
		} else {
			break;
		}
	}
	body[parent] = item;
}

void MaxFinder::Replace(int vOld, int vNew) {
	Remove(vOld);
	Add(vNew);
}

int MaxFinder::Length() const {
	return lengthBody;
}

int MaxFinder::Maximum() const {
	return (lengthBody <= 0) ? minimum : body[0];
}
