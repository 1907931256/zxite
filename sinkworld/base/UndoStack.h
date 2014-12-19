// SinkWorld UndoStack class.
// Copyright 2004 Neil Hodgson
// File written in C~ subset of C++ for automatic translation to Java and C#.

/// Stores the actions performed on a text allowing them to be undone and redone.
/// This class minimizes storage use by not storing the text for insert actions as 
/// they are done as that data is available from the document. Instead, it is only when 
/// an action is undone that that data is copied from the document into the undo stack
/// in the dataInserted field. Similarly, the text removed by a delete action is stored 
/// in dataRemoved as the action is performed but this data is removed from 
/// dataRemoved when the action is undone.
/// Several insert and delete actions may be caused by a single action by the user, so 
/// there are calls to begin and end a compound step: BeginUndoAction/EndUndoAction.
/// Some actions such as typing or deleting a range of text are automatically 
/// coalesced into compound steps based on their positions. 
/// The application may nest undo steps so that a global replace may be constructed 
/// from multiple replace steps and still be undone in one step.
/// One point in the stack is marked as the save point which is where the document 
/// was last saved.

SW_PUBLIC class UndoStack {
private:
public:
	SimpleIntegerVector *actionType;
	SimpleIntegerVector *position;
	SimpleIntegerVector *lenData;
	bool mayCoalesce;

	/// Text that has been inserted in the document in steps after the current
	/// action and that may be needed if Redo is performed.
	SplitVector_1 *dataInserted;
	int cursorInserted;

	/// Text that has been removed from the document in steps before the current
	/// action and that may be needed if Undo is performed.
	SplitVector_1 *dataRemoved;
	int cursorRemoved;

	int currentAction;
	int maxAction;
	int undoSequenceDepth;
	int savePoint;

	void Create(int n, int actionType_, int position_, int lenData_) {
		position->SetValueAllowingExpansion(n, position_);
		actionType->SetValueAllowingExpansion(n, actionType_);
		lenData->SetValueAllowingExpansion(n, lenData_);
	}

	void EnsureDataInserted() {
		if (dataInserted == NULL) {
			cursorInserted = 0;
			int growSize = dataRemoved->GetGrowSize();
			dataInserted = new SplitVector_1();
			dataInserted->Create(growSize, growSize);
		}
	}

public:
	enum { ACTION_INSERT = 0 };
	enum { ACTION_REMOVE = 1 };
	enum { ACTION_START = 2 };

	UndoStack(int initialLength, int growSize_) {
		actionType = new SimpleIntegerVector();
		actionType->SetGrowSize(growSize_);
		position = new SimpleIntegerVector();
		position->SetGrowSize(growSize_);
		lenData = new SimpleIntegerVector();
		lenData->SetGrowSize(growSize_);
		mayCoalesce = false;

		dataInserted = NULL;
		cursorInserted = 0;
		dataRemoved = NULL;
		cursorRemoved = 0;
		currentAction = 0;
		maxAction = 0;
		undoSequenceDepth = 0;
		savePoint = 0;
		dataRemoved = new SplitVector_1();
		dataRemoved->Create(1000, 1000);
	}

	~UndoStack() {
		delete actionType;
		actionType = NULL;
		delete position;
		position = NULL;
		delete lenData;
		lenData = NULL;
		delete dataRemoved;
		dataRemoved = NULL;
		delete dataInserted;
		dataInserted = NULL;
	}

	void AppendAction(int at, int position_, SplitVector_1 *substance, int lengthData_) {
		// May need to discard dataRemoved stack
		int isStart = ACTION_START;
		if (mayCoalesce)
			isStart = 0;
		if ((undoSequenceDepth == 0) && (currentAction > 0)) {
			// Top level actions may not always be coalesced
			// See if current action can be coalesced into previous action
			// Will work if both are inserts or deletes and position is same
			// Small weirdness here to avoid isolated & which XLang.py would remove
			int prevAT = actionType->ValueAt(currentAction-1);
			prevAT &= ~ACTION_START;
			if (at != prevAT) {
				isStart = ACTION_START;
			} else if (currentAction == savePoint) {
				isStart = ACTION_START;
			} else if ((at == ACTION_INSERT) &&
				   (position_ != (position->ValueAt(currentAction - 1) +
					lenData->ValueAt(currentAction - 1)))) {
				// Insertions must be immediately after to coalesce
				isStart = ACTION_START;
			} else if (at == ACTION_REMOVE) {
				if ((lengthData_ == 1) || (lengthData_ == 2)){
					if ((position_ + lengthData_) == position->ValueAt(currentAction - 1)) {
						; // Backspace -> OK
					} else if (position_ == position->ValueAt(currentAction - 1)) {
						; // Delete -> OK
					} else {
						// Removals must be at same position to coalesce
						isStart = ACTION_START;
					}
				} else {
					// Removals must be of one or two characters to coalesce
					isStart = ACTION_START;
				}
			}
		}
		if (at == ACTION_REMOVE) {
			// First truncate at current position if needed
			dataRemoved->DeleteRange(cursorRemoved,
				dataRemoved->Length() - cursorRemoved);
			// Capture the characters about to be deleted
			dataRemoved->AppendFromBuffer(substance, position_, lengthData_);
			cursorRemoved += lengthData_;
		}
		Create(currentAction, at | isStart, position_, lengthData_);
		currentAction++;
		maxAction = currentAction;
		mayCoalesce = true;

		delete dataInserted;
		dataInserted = NULL;
		cursorInserted = 0;
	}

	void BeginUndoAction() {
		mayCoalesce = false;
		undoSequenceDepth++;
	}

	void EndUndoAction() {
		undoSequenceDepth--;
	}

	void DropUndoSequence() {
		undoSequenceDepth = 0;
	}

	void SetSavePoint() {
		savePoint = currentAction;
	}

	bool IsSavePoint() {
		return savePoint == currentAction;
	}

	bool CanUndo() {
		return (currentAction > 0) && (maxAction > 0);
	}

	int StartUndo() {
		// Count the steps in this action
		int act = currentAction;
		while (act > 0) {
			act--;
			int at = actionType->ValueAt(act);
			if (at >= ACTION_START)
				break;
		}
		return currentAction - act;
	}

	UndoAction *GetUndoStep() {
		EnsureDataInserted();
		UndoAction *act = new UndoAction();
		int at = actionType->ValueAt(currentAction-1);
		at &= ~ACTION_START;
		act->actionType = at;
		act->position = position->ValueAt(currentAction-1);
		act->lenData = lenData->ValueAt(currentAction-1);
		act->data = NULL;
		return act;
	}

	void AppendInsertDataStep(UndoAction *action) {
		// Add to inserts so can be redone
		dataInserted->AppendString(action->data, action->lenData);
	}

	void RetrieveRemoveDataStep(UndoAction *action) {
		// Copy into act from removal stack
		dataRemoved->RetrieveString(cursorRemoved - action->lenData, action->data, action->lenData);
	}

	void CompletedUndoStep() {
		currentAction--;
		if (currentAction >= 0) {
			// currentAction is consumed so
			int at = actionType->ValueAt(currentAction);
			at &= ~ACTION_START;
			if (ACTION_INSERT == at) {
				cursorInserted += lenData->ValueAt(currentAction);
			} else {
				cursorRemoved -= lenData->ValueAt(currentAction);
			}
		}
		mayCoalesce = false;
	}

	bool CanRedo() {
		return maxAction > currentAction;
	}

	int StartRedo() {
		// Count the steps in this action
		if (currentAction >= maxAction) {
			return 0;
		} else {
			int act = currentAction + 1;
			while (act < maxAction) {
				int at = actionType->ValueAt(act);
				if (at >= ACTION_START)
					break;
				act++;
			}
			// Either at a start action or beyond end of stack
			return act - currentAction;
		}
	}

	UndoAction *GetRedoStep() {
		UndoAction *act = new UndoAction();

		int at = actionType->ValueAt(currentAction);
		at &= ~ACTION_START;
		act->actionType = at;
		act->position = position->ValueAt(currentAction);
		act->lenData = lenData->ValueAt(currentAction);
		act->data = new SW_BYTE[act->lenData];
		if (ACTION_INSERT == at) {
			// Copy into act from removal stack
			dataInserted->RetrieveString(cursorInserted-act->lenData, act->data, act->lenData);
		} else {
			// Copy into act from removal stack
			dataRemoved->RetrieveString(cursorRemoved, act->data, act->lenData);
		}

		return act;
	}

	void CompletedRedoStep() {
		int at = actionType->ValueAt(currentAction);
		at &= ~ACTION_START;
		if (ACTION_INSERT == at) {
			cursorInserted -= lenData->ValueAt(currentAction);
			dataInserted->DeleteRange(cursorInserted, dataInserted->Length() - cursorInserted);
		} else {
			cursorRemoved += lenData->ValueAt(currentAction);
		}
		currentAction++;
		mayCoalesce = false;
	}

};
