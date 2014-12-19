// SinkWorld Dictionary class.
// Copyright 2005 Neil Hodgson
// File written in C~ subset of C++ for automatic translation to Java and C#.

#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <string>

#include "base.h"

void Dictionary::InsertElement(DictionaryElement *element) {
	used++;
	int position = element->hashCode & (size-1);
	if (elements[position] == NULL) {
		elements[position] = element;
	} else {
		element->next = elements[position];
		elements[position] = element;
	}
}

void Dictionary::Resize(int sizeNew) {
	//~ printf("Resizing to %d\n", sizeNew);
	DictionaryElement **elementsOld = elements;
	int sizeOld = size;
	elements = NULL;
	size = 0;
	used = 0;
	Allocate(sizeNew);
	for (int i=0; i<sizeOld; i++) {
		DictionaryElement *element = elementsOld[i];
		while (element != NULL) {
			DictionaryElement *optNext = element->next;
			element->next = NULL;
			InsertElement(element);
			element = optNext;
		}
	}
	delete []elementsOld;
}

DictionaryElement *Dictionary::FindElement(Literal *key) {
	if ((key == NULL) || (key->Data() == NULL)) {
		return NULL;
	}
	int hashKey = key->HashValue();
	int position = hashKey & (size-1);
	DictionaryElement *element = elements[position];
	while (element != NULL) {
		if (element->Match(hashKey, key)) {
			return element;
		}
		element = element->next;
	}
	return NULL;
}

Dictionary::Dictionary() {
	elements = NULL;
	size = 0;
	used = 0;
	deletePosition = 0;
	Allocate(4);
}

Dictionary::~Dictionary() {
	assert(used == 0);
	delete []elements;
	elements = NULL;
	size = 0;
	deletePosition = 0;
}

void Dictionary::Allocate(int sizeNew) {
	assert(used == 0);
	size = sizeNew;
	used = 0;
	delete []elements;
	elements = new DictionaryElement *[size];
	for (int i=0; i<size; i++) {
		elements[i] = NULL;
	}
}

SW_OBJECT Dictionary::Add(Literal *key, SW_OBJECT value) {
	if ((used > size * 2 / 3)) {
		Resize(size * 2);
	}
	SW_OBJECT valueExisting = NULL;
	int hashKey = key->HashValue();
	int position = hashKey & (size-1);
	DictionaryElement *element = elements[position];
	while ((element != NULL) && (valueExisting == NULL)) {
		if (element->Match(hashKey, key)) {
			valueExisting = element->value;
			element->value = value;
		}
		element = element->next;
	}
	if (valueExisting == NULL) {
		InsertElement(new DictionaryElement(hashKey, key, value));
	}
	return valueExisting;
}

SW_OBJECT Dictionary::Remove(Literal *key) {
	int hashKey = key->HashValue();
	int position = hashKey & (size-1);
	DictionaryElement *element = elements[position];
	if (element != NULL) {
		if (element->Match(hashKey, key)) {
			elements[position] = element->next;
		} else {
			DictionaryElement *optPrev = element;
			while (element != NULL) {
				if (element->Match(hashKey, key)) {
					optPrev->next = element->next;
					break;
				}
			}
		}
	}
	SW_OBJECT valueExisting = NULL;
	if (element != NULL) {
		used--;
		valueExisting = element->value;
		element->value = NULL;
		delete element;
	}
	return valueExisting;
}

SW_OBJECT Dictionary::DeleteOne() {
	if (deletePosition > size) {
		deletePosition = 0;
	}
	int startDelete = deletePosition;
	do {
		DictionaryElement *element = elements[deletePosition];
		if (element != NULL) {
			elements[deletePosition] = element->next;
			used--;
			SW_OBJECT valueExisting = element->value;
			element->value = NULL;
			delete element;
			return valueExisting;
		}
		deletePosition = (deletePosition + 1) % size;
	} while (deletePosition != startDelete);
	return NULL;
}

SW_OBJECT Dictionary::Find(Literal *key) {
	DictionaryElement *element = FindElement(key);
	if (element != NULL)
		return element->value;
	else
		return NULL;
}

Literal *Dictionary::FindKey(Literal *key) {
	DictionaryElement *element = FindElement(key);
	if (element != NULL)
		return element->key;
	else
		return NULL;
}

WordList *Dictionary::Keys(Literal *suffix) {
	// First find out total length of all keys
	int lenKeys = 0;
	int i;
	for (i=0; i<size; i++) {
		DictionaryElement *element = elements[i];
		while (element != NULL) {
			if (element->key->EndsWith(suffix)) {
				lenKeys = lenKeys + element->key->Length() + 1;
			}
			element = element->next;
		}
	}
	int *keyText = new int[lenKeys];
	int pos = 0;
	for (i=0; i<size; i++) {
		DictionaryElement *element = elements[i];
		while (element != NULL) {
			if (element->key->EndsWith(suffix)) {
				for (int k=0;k<element->key->Length();k++) {
					keyText[pos++] = (element->key->CharAt(k));
				}
				keyText[pos++] = '\n';
			}
			element = element->next;
		}
	}
	WordList *wlKeys = new WordList(true);
	wlKeys->Set(keyText, pos);
	delete []keyText;
	return wlKeys;
}
