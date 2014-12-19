// SinkWorld OptionSet class.
// Copyright 2005 Neil Hodgson
// File written in C~ subset of C++ for automatic translation to Java and C#.

#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <string>

#include "base.h"

void OptionSet::DeleteAll() {
	SW_OBJECT o;
	while ((o=DeleteOne()) != NULL) {
		delete (Literal *)(o);
	}
}

Literal *OptionSet::CombinedKeyValue(Literal *mode, Literal *key) {
	if ((mode == NULL) || (mode->Length() == 0)) {
		return Value(key);
	} else {
		Literal *modedKey = new Literal(mode, separator, key);
		Literal *value = Value(modedKey);
		delete modedKey;
		return value;
	}
}

OptionSet::OptionSet() {
	litSuper = new Literal("super");
	separator = (SW_BYTE)'.';
}

OptionSet::~OptionSet() {
	DeleteAll();
	delete litSuper;
	litSuper = NULL;
}

void OptionSet::Insert(Literal *key, Literal *value) {
	Literal *valueOld = (Literal *)Add(key, new Literal(value));
	delete valueOld;
}

void OptionSet::Delete(Literal *key) {
	Literal *valueExisting = (Literal *)(Remove(key));
	delete valueExisting;
}

Literal *OptionSet::Value(Literal *key) {
	return (Literal *)(Find(key));
}

Literal *OptionSet::ModeValue(Literal *mode, Literal *key) {
	Literal *val = CombinedKeyValue(mode, key);
	while ((val == NULL) && (mode != NULL)) {
		mode = CombinedKeyValue(mode, litSuper);
		if (mode != NULL) {
			val = CombinedKeyValue(mode, key);
		} else {
			val = Value(key);
		}
	}
	return val;
}

int OptionSet::IntegerModeValue(Literal *mode, Literal *key, int defaultValue) {
	Literal *val = ModeValue(mode, key);
	if (val == NULL) {
		return defaultValue;
	} else {
		return val->Value();
	}
}

Literal *OptionSet::LiteralUnquoted(int *line, int start, int end) {
	// Remove '\' quoting
	int lenNoQuotes = 0;
	for (int i=start; i<end; ) {
		if (line[i] == '\\') {
			i++;
		}
		lenNoQuotes++;
		i++;
	}
	int *valWithoutQuotes = new int[lenNoQuotes];
	int j = 0;
	for (int i=start; i<end; ) {
		if (line[i] == '\\') {
			i++;
		}
		valWithoutQuotes[j] = line[i];
		j++;
		i++;
	}
	Literal *lit = new Literal(valWithoutQuotes, 0, lenNoQuotes);
	delete []valWithoutQuotes;
	return lit;
}

int OptionSet::SetFromLineDictionary(Literal *prefix, int *line, int eq, int len) {
	int start = eq + 1;
	while (start < len) {
		eq = start;
		while ((eq < len) && (line[eq] != '=')) {
			if (line[eq] == '\\') {
				eq++;
			}
			eq++;
		}
		// If non-empty key
		if ((eq > (start+1)) && (eq < len)) {
			int comma = eq+1;
			bool isDictionaryValue = line[eq+1] == '{';
			int braceCount = 0;
			while ((comma < len) && ((braceCount > 0) || ((line[comma] != ',') && (line[comma] != '}')))) {
				if (line[comma] == '\\') {
					comma++;
				} else if (line[comma] == '{') {
					braceCount++;
				} else if (line[comma] == '}') {
					braceCount--;
				}
				comma++;
			}
			Literal *lPartKey = new Literal(line, start, eq - start);
			Literal *lPrefixedPartKey=new Literal(prefix, separator, lPartKey);
			if (isDictionaryValue) {
				SetFromLineDictionary(lPrefixedPartKey, line, eq+1, comma);
			} else {
				Literal *lPartValue = LiteralUnquoted(line, eq + 1, comma);
				Insert(lPrefixedPartKey, lPartValue);
				delete lPartValue;
			}
			delete lPrefixedPartKey;
			delete lPartKey;
			start = comma + 1;
		} else {
			start = eq;
		}
	}
	return eq;
}

bool OptionSet::SetFromLine(Literal *prefix, int *line, int pos, int len) {
	int key = pos;
	int end = key;
	while ((end < len) && (line[end] != '\n')) {
		end++;
	}
	int eq = key;
	while ((eq < len) && (line[eq] != '=') && (line[eq] != ':')) {
		if (line[eq] == '\\') {
			eq++;
		}
		eq++;
	}
	if (eq < len) {
		bool isLineDictionary = line[eq] == (SW_BYTE)':';
		Literal *lKey = new Literal(line, key, eq - key);
		Literal *lKeyPrefixed;
		if ((prefix != NULL) && (prefix->Length() > 0)) {
			lKeyPrefixed = new Literal(prefix, separator, lKey);
		} else {
			lKeyPrefixed = new Literal(lKey);
		}
		if (isLineDictionary) {
			eq = SetFromLineDictionary(lKeyPrefixed, line, eq, len);
		} else {
			Literal *lValue = LiteralUnquoted(line, eq + 1, end);
			Insert(lKeyPrefixed, lValue);
			delete lValue;
		}
		delete lKey;
		delete lKeyPrefixed;
		//~ printf("Setting Key=%s Value=%s\n", lKeyPrefixed.Data(), lValue.Data());
	}
	return eq < len;
}

int OptionSet::SetFromText(Literal *prefix, SW_BYTE *text, int pos, int len) {
	int *lineBuffer = new int[len+1];
	int indentationSegment = -1;
	int posLine = pos;
	while (posLine < len) {
		bool continuation = false;
		bool complete = false;
		int indentation = -1;
		int lenLine = 0;
		while ((lenLine < len) && (pos < len) && !complete) {
			SW_BYTE ch = text[pos];
			pos++;
			if ((indentation < 0) && (ch != ' ' && ch != '\t')) {
				indentation = lenLine;
				if (indentationSegment < 0) {
					indentationSegment = indentation;
				}
				if (indentation < indentationSegment) {
					delete []lineBuffer;
					return posLine;
				}
			}
			if ((ch == '\r') || (ch == '\n')) {
				if (!continuation) {
					if ((pos < len) && (ch == '\r') && (text[pos] == '\n')) {
						// munch the second half of a crlf
						pos++;
					}
					complete = true;
				} else if ((ch == '\n') || (pos >= len) || (text[pos] != '\n')) {
					continuation = false;
				}
			} else if ((ch == '\\') && (pos < len) && ((text[pos] == '\r') || (text[pos] == '\n'))) {
				continuation = true;
			} else {
				continuation = false;
				lineBuffer[lenLine++] = ch;
			}
		}
		if ((lenLine > 0) && (lineBuffer[indentation] != '#')) {
			if (lineBuffer[lenLine-1] == ':') {
				Literal *segment = new Literal(lineBuffer, indentation, lenLine - indentation-1);
				if (prefix != NULL) {
					Literal *nestedPrefix = new Literal(prefix, separator, segment);
					pos = SetFromText(nestedPrefix, text, pos, len);
					delete nestedPrefix;
				} else {
					pos = SetFromText(segment, text, pos, len);
				}
				delete segment;
			} else {
				SetFromLine(prefix, lineBuffer, indentation, lenLine);
			}
		}
		posLine = pos;
	}
	delete []lineBuffer;
	return posLine;
}

