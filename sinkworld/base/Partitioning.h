// SinkWorld Partitioning class.
// Copyright 2004 Neil Hodgson
// File written in C~ subset of C++ for automatic translation to Java and C#.

/// A split vector of 32 bit integers with a method for adding a value to all elements
/// in a range.
/// Used by the Partitioning class.

class SplitVectorWithRangeAdd : public SplitVector_4 {
public:
	SplitVectorWithRangeAdd(int growSize_) {
		SetGrowSize(growSize_);
		ReAllocate(growSize_);
	}
	~SplitVectorWithRangeAdd() {
	}
	void RangeAddDelta(int start, int end, int delta) {
		// end is 1 past end, so end-start is number of elements to change
		int i = 0;
		int rangeLength = end - start;
		int range1Length = rangeLength;
		int part1Left = part1Length - start;
		if (range1Length > part1Left)
			range1Length = part1Left;
		while (i < range1Length) {
			body[start++] += delta;
			i++;
		}
		start += gapLength;
		while (i < rangeLength) {
			body[start++] += delta;
			i++;
		}
	}
};

/// Divide an interval into multiple partitions.
/// Useful for breaking a document down into sections such as lines.
/// Empty partitions are allowed which is used for empty state and final line.
/// Other uses of empty partitions are less defined or tested.

SW_PUBLIC class Partitioning : public IStyles  {
private:
	// To avoid calculating all the partition positions whenever any text is inserted
	// there may be a step somewhere in the list.
	int stepPartition;
	int stepLength;
	SplitVectorWithRangeAdd *body;

	// Move step forward
	void ApplyStep(int partitionUpTo) {
		if (stepLength != 0) {
			body->RangeAddDelta(stepPartition+1, partitionUpTo + 1, stepLength);
		}
		stepPartition = partitionUpTo;
		if (stepPartition >= body->Length()-1) {
			stepPartition = body->Length()-1;
			stepLength = 0;
		}
	}

	// Move step backward
	void BackStep(int partitionDownTo) {
		if (stepLength != 0) {
			body->RangeAddDelta(partitionDownTo+1, stepPartition+1, -stepLength);
		}
		stepPartition = partitionDownTo;
	}

public:
	Partitioning(int growSize_) {
		body = new SplitVectorWithRangeAdd(growSize_);
		stepPartition = 0;
		stepLength = 0;
		body->Insert(0, 0);	// This value stays 0 for ever
		body->Insert(1, 0);	// This is the end of the first partition and will be the start of the second
	}

	~Partitioning() {
		delete body;
		body = NULL;
	}

	int Partitions() {
		return body->Length()-1;
	}

	int Length() {
		return DocumentLength();
	}

	int DocumentLength() {
	    	return PositionFromPartition(body->Length()-1);
	}

	void InsertPartition(int partition, int pos) {
		if (stepPartition < partition) {
			ApplyStep(partition);
		}
		body->Insert(partition, pos);
		stepPartition++;
	}

	void SetPartitionStartPosition(int partition, int pos) {
		ApplyStep(partition+1);
		if ((partition < 0) || (partition > body->Length())) {
			throw new BadPositionException();
		}
		body->SetValueAt(partition, pos);
	}

	void InsertText(int partitionInsert, int delta) {
		// Point all the partitions after the insertion point further along in the buffer
		if (stepLength != 0) {
			if (partitionInsert >= stepPartition) {
				// Fill in up to the new insertion point
				ApplyStep(partitionInsert);
				stepLength += delta;
			} else if (partitionInsert >= (stepPartition - body->Length() / 10)) {
				// Close to step but before so move step back
				BackStep(partitionInsert);
				stepLength += delta;
			} else {
				ApplyStep(body->Length()-1);
				stepPartition = partitionInsert;
				stepLength = delta;
			}
		} else {
			stepPartition = partitionInsert;
			stepLength = delta;
		}
	}

	void RemovePartition(int partition) {
		if (partition > stepPartition) {
			ApplyStep(partition);
			stepPartition--;
		} else {
			stepPartition--;
		}
		body->Delete(partition);
	}

	int PositionFromPartition(int partition) {
		if ((partition < 0) || (partition >= body->Length())) {
			throw new BadPositionException();
		}
		int pos = body->ValueAt(partition);
		if (partition > stepPartition)
			pos += stepLength;
		return pos;
	}

	int PartitionFromPosition(int pos) {
		if (body->Length() <= 1)
			return 0;
		if (pos >= (PositionFromPartition(body->Length()-1)))
			return body->Length() - 1 - 1;
		int lower = 0;
		int upper = body->Length()-1;
		do {
			int middle = (upper + lower + 1) / 2; 	// Round high
			int posMiddle = body->ValueAt(middle);
			if (middle > stepPartition)
				posMiddle += stepLength;
			if (pos < posMiddle) {
				upper = middle - 1;
			} else {
				lower = middle;
			}
		} while (lower < upper);
		return lower;
	}
	// Remove any partitions after start up to end
	void RemovePartitions(int start, int end) {
		int partitionRemove = PartitionFromPosition(start);
		if (PositionFromPartition(partitionRemove) <= start) {
			// Avoid removing the partition including the start.
			partitionRemove++;
		}
		while (PositionFromPartition(partitionRemove) < end) {
			RemovePartition(partitionRemove);
		}
		// If range ends with empty partition, remove it.
		if ((PositionFromPartition(partitionRemove) == end) &&
			(partitionRemove < body->Length() - 1) &&
			(PositionFromPartition(partitionRemove+1) == end)) {
			RemovePartition(partitionRemove);
		}
	}

	// Implement IStyles interface so can listen for document modifications
	void InsertSpace(int position, int insertLength) {
		int partition = PartitionFromPosition(position);
		InsertText(partition, insertLength);
	}
	void Delete(int position) {
		DeleteRange(position, 1);
	}
	void DeleteRange(int position, int deleteLength) {
		RemovePartitions(position, position + deleteLength);
		int partition = PartitionFromPosition(position);
		InsertText(partition, -deleteLength);
	}
	void DeleteAll() {
		DeleteRange(0, Length());
	}
	// These are stubs required to fulfil IStyles interface but not actually used
	int ValueAt(int position) {
		return 0;
	}
	int FindNextChange(int position, int end) {
		return end;
	}
	void FillRange(int position, int val, int fillLength) {
	}
};
