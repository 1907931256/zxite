// SinkWorld History class.
// Copyright 2004 Neil Hodgson
// File written in C~ subset of C++ for automatic translation to Java and C#.

/// A single item with a history.
/// TODO: Ensure that the text value form teVal works as well as the numeric form

#include <string.h>
#include <assert.h>
#include <string>

#include "base.h"

HistoricEvent *History::FindBefore(int position) {
	HistoricEvent *he = first;
	while (he != NULL) {
		if (he->position > position) {
			return he->previous;
		}
		he = he->next;
	}
	return last;
}

HistoricEvent *History::FindAfter(int position) {
	HistoricEvent *he = first;
	while (he != NULL) {
		if (he->position > position) {
			return he;
		}
		he = he->next;
	}
	return NULL;
}

void History::DeleteAll() {
	while (first != NULL) {
		HistoricEvent *next = first->next;
		delete first;
		first = next;
	}
	last = NULL;
	length = 0;
}

void History::Pop(int position) {
	int depth = Depth() - 1;
	HistoricEvent *popValue = last;
	while (popValue != NULL) {
		if (popValue->depth == depth) {
			Append(position, popValue->value, popValue->teVal, depth, false);
			return;
		}
		popValue = popValue->previous;
	}
	// Stack now empty
	Append(position, startValue, NULL, depth, false);
}

void History::InvalidateAfter(int position) {
	if (first != NULL) {
		if (position < first->position) {
			DeleteAll();
		} else {
			while (position < last->position) {
				HistoricEvent *toFree = last;
				last = last->previous;
				delete toFree;
			}
			last->next = NULL;
			if (last->previous != NULL) {
				last->previous->next = last;
			}
		}
	}
}

History *History::Split(int position) {
	History *historyPast = new History(startValue);
	if (first != NULL) {
		if (position < first->position) {
			// Transfer all to historyPast
			historyPast->first = first;
			historyPast->last = last;
			first = NULL;
			last = NULL;
		} else if (position > last->position) {
			// Keep all here
		} else {
			HistoricEvent *heBeforeSplit = first;
			HistoricEvent *heSplit = first;
			while (position > heSplit->position) {
				heBeforeSplit = heSplit;
				heSplit = heSplit->next;
			}
			heBeforeSplit->next = NULL;
			historyPast->first = heSplit;
			historyPast->last = last;
			heSplit->previous = NULL;
			last = heBeforeSplit;
		}
	}
	historyPast->SetStartValue(Value(), TextValue());
	return historyPast;
}

bool History::ChangedHistory(int position, History *historyPast) {
	int valAtPosition = startValue;
	int valAtPositionPast = historyPast->startValue;
	Literal *teValAtPosition = teValStart;
	Literal *teValAtPositionPast = historyPast->teValStart;
	int depthAtPosition = 0;
	int depthAtPositionPast = 0;

	HistoricEvent *he = FindBefore(position);
	if (he != NULL) {
		valAtPosition = he->value;
		teValAtPosition = he->teVal;
		depthAtPosition = he->depth;
	}

	HistoricEvent *hePast = historyPast->FindBefore(position);
	if (hePast != NULL) {
		valAtPositionPast = hePast->value;
		teValAtPositionPast = hePast->teVal;
		depthAtPositionPast = hePast->depth;
	}

	if ((valAtPosition == valAtPositionPast) && (depthAtPosition == depthAtPositionPast)) {
		if (teValAtPosition == teValAtPositionPast) {
			return false;
		} else {
			if ((teValAtPosition != NULL) && (teValAtPositionPast != NULL)) {
				return !teValAtPosition->Equals(teValAtPositionPast);
			}
		}
	}
	return true;
}

void History::RepairHistory(int position, History *historyPast) {
	InvalidateAfter(position);
	HistoricEvent *hePast = historyPast->first;
	while (hePast != NULL) {
		if (hePast->position > position) {

			HistoricEvent *heLast = historyPast->last;
			// Detach from past
			HistoricEvent *heEndPast = hePast->previous;
			if (heEndPast != NULL) {
				heEndPast->next = NULL;
			}
			historyPast->last = heEndPast;
			hePast->previous = NULL;
			if (historyPast->first == hePast) {
				historyPast->first = NULL;
			}

			// Graft back onto this
			if (last == NULL) {
				first = hePast;
			} else {
				last->next = hePast;
			}
			hePast->previous = last;
			last = heLast;
			break;
		}
		hePast = hePast->next;
	}
}

int History::Value() {
	if (last != NULL) {
		return last->value;
	} else {
		return startValue;
	}
}

Literal *History::TextValue() {
	if (last != NULL) {
		return last->teVal;
	} else {
		return teValStart;
	}
}

int History::ValueAt(int position) {
	HistoricEvent *he = FindBefore(position);
	return (he != NULL) ? he->value : startValue;
}

int History::Depth() {
	if (last != NULL) {
		return last->depth;
	} else {
		return 0;
	}
}

int History::Length() {
	return length;
}

int History::FindNextChange(int position, int end) {
	HistoricEvent *he = FindAfter(position);
	if ((he != NULL) && (he->position <= end)) {
		return he->position;
	} else {
		return end + 1;
	}
}

void History::FillRange(int position, int val, int fillLength) {
}

void History::InsertSpace(int position, int insertLength) {
	HistoricEvent *he = FindAfter(position);
	while (he != NULL) {
		he->position += insertLength;
		he = he->next;
	}
	length += insertLength;
}

void History::DeleteRange(int position, int deleteLength) {
	HistoricEvent *he = FindAfter(position);
	while (he != NULL) {
		if (he->position - deleteLength <= position) {
			// TODO? Remove any events for the same position
			he->position = position;
		} else {
			he->position -= deleteLength;
		}
		he = he->next;
	}
	length -= deleteLength;
}
