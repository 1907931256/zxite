// SinkWorld RunStyles class.
// Copyright 2004 Neil Hodgson
// File written in C~ subset of C++ for automatic translation to Java and C#.

#include <string.h>
#include <assert.h>
#include <string>

#include "base.h"

// Find the first run at a position
int RunStyles::RunFromPosition(int position) {
	int run = starts->PartitionFromPosition(position);
	// Go to first element with this position
	while ((run > 0) && (position == starts->PositionFromPartition(run-1))) {
		run--;
	}
	return run;
}

// If there is no run boundary at position, insert one continuing style.
int RunStyles::SplitRun(int position) {
	int run = RunFromPosition(position);
	int posRun = starts->PositionFromPartition(run);
	if (posRun < position) {
		int runStyle = ValueAt(position);
		run++;
		starts->InsertPartition(run, position);
		styles->InsertSpace(run, 1);
		styles->FillRange(run, runStyle, 1);
	}
	return run;
}

void RunStyles::RemoveRun(int run) {
	starts->RemovePartition(run);
	styles->DeleteRange(run, 1);
}

void RunStyles::RemoveRunIfEmpty(int run) {
	if ((run < starts->Partitions()) && (starts->Partitions() > 1)) {
		if (starts->PositionFromPartition(run) == starts->PositionFromPartition(run+1)) {
			RemoveRun(run);
		}
	}
}

void RunStyles::RemoveRunIfSameAsPrevious(int run) {
	if ((run > 0) && (run < starts->Partitions())) {
		if (styles->ValueAt(run-1) == styles->ValueAt(run)) {
			RemoveRun(run);
		}
	}
}

RunStyles::RunStyles() {
	starts = new Partitioning(8);
	styles = new SplitVector_4();
	styles->Create(8,8);
	styles->InsertSpace(0, 2);
}

RunStyles::~RunStyles() {
	delete starts;
	starts = NULL;
	delete styles;
	styles = NULL;
}

int RunStyles::Length() {
	return starts->DocumentLength();
}

int RunStyles::ValueAt(int position) {
	return styles->ValueAt(starts->PartitionFromPosition(position));
}

int RunStyles::FindNextChange(int position, int end) {
	int run = starts->PartitionFromPosition(position);
	if (run < starts->Partitions()) {
		int runChange = starts->PositionFromPartition(run);
		if (runChange > position)
			return runChange;
		int nextChange = starts->PositionFromPartition(run + 1);
		if (nextChange > position) {
			return nextChange;
		} else if (position < end) {
			return end;
		} else {
			return end + 1;
		}
	} else {
		return end + 1;
	}
}

void RunStyles::FillRange(int position, int value, int fillLength) {
	int end = position + fillLength;
	int runEnd = RunFromPosition(end);
	if (styles->ValueAt(runEnd) == value) {
		// End already has value so trim range.
		end = starts->PositionFromPartition(runEnd);
		if (position >= end) {
			// Whole range is already same as value so no action
			return;
		}
		fillLength = end - position;
	} else {
		runEnd = SplitRun(end);
	}
	int runStart = RunFromPosition(position);
	if (styles->ValueAt(runStart) == value) {
		// Start is in expected value so trim range.
		runStart++;
		position = starts->PositionFromPartition(runStart);
		fillLength = end - position;
	} else {
		if (starts->PositionFromPartition(runStart) < position) {
			runStart = SplitRun(position);
			runEnd++;
		}
	}
	if (runStart < runEnd) {
		styles->SetValueAt(runStart, value);
		// Remove each old run over the range
		for (int run=runStart+1; run<runEnd; run++) {
			RemoveRun(runStart+1);
		}
		runEnd = RunFromPosition(end);
		RemoveRunIfSameAsPrevious(runEnd);
		RemoveRunIfSameAsPrevious(runStart);
	}
}

// May need some changes to be similar to changes made July/07 to FillRange
void RunStyles::IncrementRange(int position, int value, int fillLength) {
	int end = position + fillLength;
	SplitRun(position);
	SplitRun(end);
	int runStart = RunFromPosition(position);
	int runEnd = RunFromPosition(end);
	for (int run=runStart; run<runEnd; run++) {
		styles->SetValueAt(run, styles->ValueAt(run) + value);
	}
	runEnd = RunFromPosition(end);
	RemoveRunIfSameAsPrevious(runEnd);
	RemoveRunIfSameAsPrevious(runStart);
}

void RunStyles::InsertSpace(int position, int insertLength) {
	int runStart = RunFromPosition(position);
	if (starts->PositionFromPartition(runStart) == position) {
		int runStyle = ValueAt(position);
		// Inserting at start of run so make previous longer
		if (runStart == 0) {
			// Inserting at start of document so ensure 0
			if (runStyle) {
				styles->SetValueAt(0, 0);
				starts->InsertPartition(1, 0);
				styles->InsertSpace(1, 1);
				styles->FillRange(1, runStyle, 1);
				starts->InsertText(0, insertLength);
			} else {
				starts->InsertText(runStart, insertLength);
			}
		} else {
			if (runStyle) {
				starts->InsertText(runStart-1, insertLength);
			} else {
				// Insert at end of run so do not extend style
				starts->InsertText(runStart, insertLength);
			}
		}
	} else {
		starts->InsertText(runStart, insertLength);
	}
}

void RunStyles::DeleteAll() {
	delete starts;
	starts = NULL;
	delete styles;
	styles = NULL;
	starts = new Partitioning(8);
	styles = new SplitVector_4();
	styles->Create(8,8);
	styles->InsertSpace(0, 2);
}

void RunStyles::DeleteRange(int position, int deleteLength) {
	int end = position + deleteLength;
	int runStart = RunFromPosition(position);
	int runEnd = RunFromPosition(end);
	if (runStart == runEnd) {
		// Deleting from inside one run
		starts->InsertText(runStart, -deleteLength);
	} else {
		runStart = SplitRun(position);
		runEnd = SplitRun(end);
		starts->InsertText(runStart, -deleteLength);
		// Remove each old run over the range
		for (int run=runStart; run<runEnd; run++) {
			RemoveRun(runStart);
		}
		RemoveRunIfEmpty(runStart);
		RemoveRunIfSameAsPrevious(runStart);
	}
}
