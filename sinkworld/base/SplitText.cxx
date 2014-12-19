// SinkWorld SplitText class.
// Copyright 2004 Neil Hodgson
// File written in C~ subset of C++ for automatic translation to Java and C#.

#include <string.h>
#include <assert.h>
#include <string>

#include "base.h"

void SplitText::InsertLine(int lineInsert, int position) {
	lines->InsertPartition(lineInsert, position);
	LineData *pld = pldRoot;
	while (pld != NULL) {
		pld->InsertLine(lineInsert);
		pld = pld->next;
	}
	StylesWithValidity *swv = first;
	while (swv != NULL) {
		if (swv->lineStyles) {
			swv->style->InsertSpace(lineInsert, 1);
		}
		swv = swv->next;
	}
}

void SplitText::RemoveLine(int lineRemove) {
	lines->RemovePartition(lineRemove);
	LineData *pld = pldRoot;
	while (pld != NULL) {
		pld->RemoveLine(lineRemove);
		pld = pld->next;
	}
	StylesWithValidity *swv = first;
	while (swv != NULL) {
		if (swv->lineStyles) {
			swv->style->DeleteRange(lineRemove, 1);
		}
		swv = swv->next;
	}
}

void SplitText::BasicInsertBytes(int position, SW_BYTE s[], int insertLength) {
	assert((lines == NULL) || (PositionFromLine(Lines()) == Length()));
	assert(CheckLines());
	int stride = Stride();
	substance->InsertString(position, s, insertLength);
	StylesWithValidity *swv = first;
	while (swv != NULL) {
		if (!swv->lineStyles) {
			swv->style->InsertSpace(position, insertLength);
			for (int h=0; h<swv->histories->len; h++) {
				swv->histories->Get(h)->InsertSpace(position, insertLength);
			}
		}
		if (swv->HasHole()) {
			// Need to move the hole
			if (swv->holeStart >= position)
				swv->holeStart += insertLength;
			if (swv->holeEnd >= position)
				swv->holeEnd += insertLength;
		}
		if (swv->validTo > position) {
			// TODO? possibly move hole range to fit whole lines
			// Don't create a hole near the end of valid data
			bool holePotential = (swv->validTo - position + insertLength) > StylesWithValidity::holeDistance;
			if (swv->HasHole() || holePotential) {
				if (swv->HasHole()) {
					if (swv->holeStart > position) {
						swv->holeStart = position;
					}
					if (swv->holeEnd < position + insertLength) {
						swv->holeEnd = position + insertLength;
					}
				} else {
					swv->holeStart = position;
					swv->holeEnd = position + insertLength;
				}
				if (swv->validTo <= swv->holeEnd) {
					// Hole extends to valid end so move valid bak to start of hole and drop hole
					// TODO? merge when near validTo?
					swv->validTo = swv->holeStart;
					swv->holeStart = -1;
				}
			} else {
				// Invalidation near end so just wind back
				swv->validTo = position;
			}
		}
		swv = swv->next;
	}

	if (lines != NULL) {
		int lineInsert = lines->PartitionFromPosition(position);
		lines->InsertText(lineInsert, insertLength);
		lineInsert++;
		int chPrev = ' ';
		if ((position - stride) >= 0)
			chPrev = substance->UnitValue(position - stride);
		int chAfter = ' ';
		if (position + insertLength < Length())
			chAfter = substance->UnitValue(position + insertLength);
		if (chPrev == '\r' && chAfter == '\n') {
			// Splitting up a crlf pair at position
			InsertLine(lineInsert, position);
			lineInsert++;
		}
		int ch = ' ';
		for (int i = 0; i < insertLength; i += stride) {
			if (stride == 1)
				ch = (int)(s[i]) & 0xff;
			else
				ch = substance->UnitFromArray(s, i);
			if (ch == '\r') {
				if ((i != (insertLength-stride)) || (chAfter != '\n')) {
					InsertLine(lineInsert, (position + i) + stride);
					lineInsert++;
				}
			} else if (ch == '\n') {
				if (chPrev == '\r') {
					// Patch up what was end of line
					lines->SetPartitionStartPosition(lineInsert - 1, (position + i) + stride);
				} else {
					InsertLine(lineInsert, (position + i) + stride);
					lineInsert++;
				}
			}
			chPrev = ch;
		}
	}
	assert((lines == NULL) || (PositionFromLine(Lines()) == Length()));
	assert(CheckLines());
}

void SplitText::BasicDeleteRange(int position, int deleteLength) {
	// Have to fix up line positions before doing deletion as looking at text in buffer
	// to work out which lines have been removed
	assert((lines == NULL) || (PositionFromLine(Lines()) == Length()));
	assert(CheckLines());
	if (lines != NULL) {
		int lineRemove = lines->PartitionFromPosition(position);
		// Point all the lines after the insertion point less further along in the buffer
		lines->InsertText(lineRemove, -deleteLength);
		lineRemove++;
		int chPrev = ' ';
		if (position >= 1)
			chPrev = substance->UnitValue(position - Stride());
		int chBefore = chPrev;
		int chNext = ' ';
		if (position < Length())
			chNext = substance->UnitValue(position);
		int i = 0;
		if (chPrev == '\r' && chNext == '\n') {
			// Breaking apart a \r\n pair
			// Move back one
			lines->SetPartitionStartPosition(lineRemove, position);
			lineRemove++;
			// First \n is not real deletion so skip it
			i += Stride();
			chNext = ' ';
			if (position + i < Length())
				chNext = substance->UnitValue(position + i);
		}

		int ch = chNext;
		for (;i < deleteLength; i += Stride()) {
			chNext = ' ';
			if (position + i + Stride() < Length())
				chNext = substance->UnitValue(position + i + Stride());
			if (ch == '\r') {
				if (chNext != '\n') {
					RemoveLine(lineRemove);
				}
			} else if (ch == '\n') {
				RemoveLine(lineRemove);
			}
			ch = chNext;
		}

		// May have to fix up end if last deletion causes cr to be next to lf
		// or removes one of a crlf pair
		int chAfter = ' ';
		if ((position + deleteLength) < Length())
			chAfter = substance->UnitValue(position + deleteLength);
		if (chBefore == '\r' && chAfter == '\n') {
			// Using lineRemove-1 as cr ended line before start of deletion
			RemoveLine(lineRemove - 1);
			lines->SetPartitionStartPosition(lineRemove - 1, position + Stride());
		}
	}

	substance->DeleteRange(position, deleteLength);
	StylesWithValidity *swv = first;
	while (swv != NULL) {
		if (!swv->lineStyles) {
			swv->style->DeleteRange(position, deleteLength);
			for (int h=0; h<swv->histories->len; h++) {
				swv->histories->Get(h)->DeleteRange(position, deleteLength);
			}
		}
		if (swv->HasHole()) {
			// Need to move the hole
			if (swv->holeStart >= position) {
				swv->holeStart -= deleteLength;
				if (swv->holeStart < position) {
					swv->holeStart = position;
				}
			}
			if (swv->holeEnd >= position) {
				swv->holeEnd -= deleteLength;
				if (swv->holeEnd < position) {
					swv->holeEnd = position;
				}
			}
		}
		if (swv->validTo > position) {
			// TODO? possibly move hole range to fit whole lines
			// Don't create a hole near the end of valid data
			bool holePotential = (swv->validTo - position) > StylesWithValidity::holeDistance;
			if (swv->HasHole() || holePotential) {
				if (swv->HasHole()) {
					if (swv->holeStart > position) {
						swv->holeStart = position;
					}
					if (swv->holeEnd < position) {
						swv->holeEnd = position;
					}
				} else {
					swv->holeStart = position;
					swv->holeEnd = position;
				}
				if (swv->validTo <= swv->holeEnd) {
					// Hole extends to valid end so move valid bak to start of hole and drop hole
					// TODO? merge when near validTo?
					swv->validTo = swv->holeStart;
					swv->holeStart = -1;
				}
			} else {
				// Invalidation near end so just wind back
				swv->validTo = position;
			}
		}
		swv = swv->next;
	}
	assert((lines == NULL) || (PositionFromLine(Lines()) == Length()));
	assert(CheckLines());
}

void SplitText::Create(int initialLength_, int growSize_) {
	first = NULL;
	initialLength = initialLength_;
	growSize = growSize_;
	substance = new SubstanceUTF8();
	substance->Create(initialLength, growSize);
	lines = new Partitioning(growSize_);
	us = new UndoStack(1000, 1000);
	pldRoot = NULL;
	encoding = ENCODING_BYTE;
	validating = true;
}

SplitText::~SplitText() {
	delete substance;
	substance = NULL;
	delete lines;
	lines = NULL;
	delete us;
	us = NULL;
	DetachAllStyles();
}

// TODO: is the primary style a reasonable concept or should this be explicit?
int SplitText::StyleAt(int position) {
	if (first != NULL) {
		return first->style->ValueAt(position);
	} else {
		return 0;
	}
}

int SplitText::Layers() {
	int layer=0;
	StylesWithValidity *swv = first;
	while (swv != NULL) {
		swv = swv->next;
		layer++;
	}
	return layer;
}

int SplitText::StyleAtLayer(int position, int layer) {
	StylesWithValidity *swv = first;
	while ((swv != NULL) && (layer > 0)) {
		swv = swv->next;
		layer--;
	}
	if (swv != NULL) {
		return swv->style->ValueAt(position);
	} else {
		return 0;
	}
}

IStyles *SplitText::GetLayer(int layer) {
	StylesWithValidity *swv = first;
	while ((swv != NULL) && (layer > 0)) {
		swv = swv->next;
		layer--;
	}
	if (swv != NULL) {
		return swv->style;
	} else {
		return NULL;
	}
}

void SplitText::DetachAllStyles() {
	while (first != NULL) {
		StylesWithValidity *next = first->next;
		first->next = NULL;
		delete first;
		first = next;
	}
}

void SplitText::DetachStyles(IStyles *style_) {
	StylesWithValidity *swvPrev = NULL;
	StylesWithValidity *swv = first;
	while (swv != NULL) {
		if (swv->style == style_) {
			if (swvPrev != NULL) {
				swvPrev->next = swv->next;
			} else {
				first = swv->next;
			}
			delete swv;
			return;
		}
		swvPrev = swv;
		swv = swv->next;
	}
}

void SplitText::AttachStyles(IStyles *style_, ILexer *lexer_, int priority, bool lineStyles) {
	StylesWithValidity *swvNew = new StylesWithValidity(style_, lexer_, priority, lineStyles);
	// Ensure styling buffer resized to match document
	if (lineStyles) {
		if (swvNew->style->Length() < Lines()) {
			swvNew->style->InsertSpace(0, Lines() - swvNew->style->Length());
		}
	} else {
		if (swvNew->style->Length() < Length()) {
			swvNew->style->InsertSpace(swvNew->style->Length(),
				Length() - swvNew->style->Length());
		}
	}
	StylesWithValidity *swvPrev = NULL;
	StylesWithValidity *swv = first;
	while ((swv != NULL) && (swv->priority > priority)) {
		swvPrev = swv;
		swv = swv->next;
	}
	if (swvPrev == NULL) {
		swvNew->next = first;
		first = swvNew;
	} else {
		swvNew->next = swv;
		swvPrev->next = swvNew;
	}
}

void SplitText::AttachHistory(ILexer *lexer_, History *history) {
	StylesWithValidity *swv = first;
	while (swv != NULL) {
		if (swv->lexer == lexer_) {
			swv->histories->AddHistory(history);
			history->InsertSpace(0, swv->style->Length());
			return;
		}
		swv = swv->next;
	}
}

IStyles *SplitText::CreateStylesSized(int initialLength, int growSize, int styleWidth) {
	if (styleWidth == 0) {
		return new SplitVector_0();
	} else if (styleWidth == 1) {
		SplitVector_1 *ret = new SplitVector_1();
		ret->Create(initialLength, growSize);
		return ret;
	} else if (styleWidth == 2) {
		SplitVector_2 *ret = new SplitVector_2();
		ret->Create(initialLength, growSize);
		return ret;
	} else if (styleWidth == 4) {
		SplitVector_4 *ret = new SplitVector_4();
		ret->Create(initialLength, growSize);
		return ret;
	} else {
		return NULL;
	}
}

IStyles *SplitText::CreateStyles(int styleWidth) {
	return CreateStylesSized(1024, 1024, styleWidth);
}

bool SplitText::LexTo(int position, int priority, bool justTheHole) {
	bool changeBeyondEnd = false;
	StylesWithValidity *swv = first;
	while ((swv != NULL) && (swv->priority >= priority)) {
		if (swv->lexer != NULL) {
			bool needToLex = false;
			int posLexStart;
			int posLexEnd;
			if (justTheHole) {
				needToLex = swv->HasHole();	//  && (swv->holeStart < position);
				posLexStart = swv->holeStart;
				posLexEnd = swv->holeEnd;
			} else {
				needToLex = (swv->validTo < position) || swv->HasHole();
				posLexStart = swv->validTo;
				if (swv->HasHole()) {
					posLexStart = swv->holeStart;
				}
				posLexEnd = position;
			}
			if (needToLex) {
				int validLine = LineFromPosition(posLexStart);
				posLexStart = PositionFromLine(validLine);
				int endLine = LineFromPosition(posLexEnd);
				posLexEnd = PositionFromLine(endLine + 1);
				if (justTheHole) {
					swv->holeStart = -1;
				} else {
					swv->validTo = posLexEnd;
				}
				Styler *styler = new Styler(this,
					swv->style, posLexStart, posLexEnd - posLexStart, swv->lineStyles);
				HistorySet *historyPast = new HistorySet(swv->histories->len);
				for (int h=0; h<swv->histories->len; h++) {
					historyPast->Set(h, swv->histories->Get(h)->Split(styler->startSeg));
				}
				swv->lexer->Colourise(styler);
				for (int h=0; h<swv->histories->len; h++) {
					if (swv->histories->Get(h)->ChangedHistory(styler->End(), historyPast->Get(h))) {
						styler->changeBeyondEnd = true;
						if (justTheHole) {
							swv->validTo = posLexEnd;	// Need to lex again from hole
						}
					} else {
						// Repairable change
						swv->histories->Get(h)->RepairHistory(styler->End(), historyPast->Get(h));
					}
					delete historyPast->Get(h);
					historyPast->Set(h, NULL);
				}
				delete historyPast;
				changeBeyondEnd = changeBeyondEnd || styler->changeBeyondEnd;
				delete styler;
			}
		}
		swv = swv->next;
	}
	return changeBeyondEnd;
}

/// Retrieving positions outside the range of the buffer fails
void SplitText::RetrieveBytes(int position, SW_BYTE s[], int retrieveLength) {
	substance->RetrieveString(position, s, retrieveLength);
}

int SplitText::HashRange(int position, int len) {
	// Use the Python string hash algorithm
	// http://effbot.org/zone/python-hash.htm
	int ret = 0;
	if (len > 0) {
		ret = (substance->CharAt(position) & 0xff) << 7;
		for (int i=0; i<len; i++) {
			ret *= 1000003;
			ret ^= (substance->CharAt(position+i) & 0xff);
		}
		ret ^= len;
	}
	return ret;
}

int SplitText::Characters(int position, int len) {
	int lenChars = 0;
	int end = position + len;
	while (position < end) {
		lenChars++;
		position = PositionNext(position);
	}
	if (position > end) {
		throw new BadPositionException();
	}
	return lenChars;
}

int SplitText::RetrieveUTF8(int position, SW_BYTE s[], int retrieveLength) {
	return substance->RetrieveUTF8(position, s, retrieveLength);
}

int SplitText::RetrieveUTF16(int position, SW_SHORT s[], int retrieveLength) {
	return substance->RetrieveUTF16(position, s, retrieveLength);
}

int SplitText::RetrieveUTF32(int position, int s[], int retrieveLength) {
	return substance->RetrieveUTF32(position, s, retrieveLength);
}

int SplitText::LengthInText(const int s[], int sLength) {
	return substance->LengthInText(s, sLength);
}

void SplitText::DiscardLines() {
	delete lines;
	lines = NULL;
}

void SplitText::RecreateLines() {
	delete lines;
	lines = new Partitioning(128);
	int chPrev = ' ';
	int len = Length();
	int lineInsert = 0;
	lines->InsertText(lineInsert, len);
	lineInsert++;
	for (int i = 0; i < len; i += Stride()) {
		int ch = substance->UnitValue(i);
		if (ch == '\r') {
			InsertLine(lineInsert, i + Stride());
			lineInsert++;
		} else if (ch == '\n') {
			if (chPrev == '\r') {
				// Patch up what was end of line
				lines->SetPartitionStartPosition(lineInsert - 1, i + Stride());
			} else {
				InsertLine(lineInsert, i + Stride());
				lineInsert++;
			}
		}
		chPrev = ch;
	}
	assert(CheckLines());
}

bool SplitText::CheckLines() {
	// Turn off checking while profiling
	if (lines != NULL)
		return true;
	if (lines == NULL)	// Lines turned off
		return true;
	int chPrev = ' ';
	int len = Length();
	assert(PositionFromLine(0) == 0);
	if (len > 0) {
		for (int j=0; j<Lines()-1;j++) {
			assert(PositionFromLine(j) < PositionFromLine(j+1));
			if (PositionFromLine(j) >= PositionFromLine(j+1)) {
				return false;
			}
		}
		assert(PositionFromLine(Lines()-1) <= PositionFromLine(Lines()));
	}
	int lineInsert = 1;
	for (int i = 0; i < len; i += Stride()) {
		int ch = substance->UnitValue(i);
		if (ch == '\r') {
			int chNext = (i+1<len) ? substance->UnitValue(i+Stride()) : ' ';
			if (chNext != '\n') {
				assert(PositionFromLine(lineInsert) == (i + Stride()));
				if (PositionFromLine(lineInsert) != (i + Stride())) {
					return false;
				}
			}
			lineInsert++;
		} else if (ch == '\n') {
			if (chPrev == '\r') {
				// Patch up what was end of line
				assert(PositionFromLine(lineInsert-1) == (i + Stride()));
				if (PositionFromLine(lineInsert-1) != (i + Stride())) {
					return false;
				}
			} else {
				assert(PositionFromLine(lineInsert) == (i + Stride()));
				if (PositionFromLine(lineInsert) != (i + Stride())) {
					return false;
				}
				lineInsert++;
			}
		}
		chPrev = ch;
	}
	return true;
}

void SplitText::SetValidating(bool validating_) {
	validating = validating_;
	if (validating) {
		if (!substance->ValidateRange(0, substance->Length())) {
			throw new CharacterFragmentException();
		}
	}
}

void SplitText::InsertBytes(int position, SW_BYTE s[], int insertLength) {
	if ((position < 0) || (position > Length())) {
		throw new BadPositionException();
	}
	if (validating) {
		if (!substance->BetweenCharacters(position)) {
			throw new CharacterFragmentException();
		}
		if (!substance->ValidateArray(s, insertLength)) {
			throw new CharacterFragmentException();
		}
	}
	BasicInsertBytes(position, s, insertLength);
	if (us != NULL) {
		us->AppendAction(UndoStack::ACTION_INSERT, position, NULL, insertLength);
	}
}

int SplitText::InsertUTF32(int position, const int *s, int insertLength) {
	int lenBytes = substance->LenCharacters(s, insertLength);
	SW_BYTE *bytes = new SW_BYTE[lenBytes];
	substance->BytesFromCharacters(bytes, lenBytes, s, insertLength);
	InsertBytes(position, bytes, lenBytes);
	delete []bytes;
	return lenBytes;
}

void SplitText::DeleteRange(int position, int deleteLength) {
	if ((position < 0) || ((position + deleteLength) > Length())) {
		throw new BadPositionException();
	}
	if (validating) {
		if (!substance->BetweenCharacters(position)) {
			throw new CharacterFragmentException();
		}
		if (!substance->ValidateRange(position, deleteLength)) {
			throw new CharacterFragmentException();
		}
	}
	if (us != NULL) {
		us->AppendAction(UndoStack::ACTION_REMOVE, position, substance, deleteLength);
	}
	BasicDeleteRange(position, deleteLength);
}

int SplitText::Lines() {
	if (lines == NULL)
		RecreateLines();
	return lines->Partitions();
}

int SplitText::PositionFromLine(int line) {
	if (lines == NULL)
		RecreateLines();
	return lines->PositionFromPartition(line);
}

int SplitText::LineEndPositionFromLine(int line) {
	if (line >= (Lines()-1)) {
		return Length();
	} else {
		int positionStartLine = PositionFromLine(line);
		int position = PositionFromLine(line + 1);
		if (position > positionStartLine) {
			position -= Stride();
			if ((position > positionStartLine) && (substance->IsCrLf(position - Stride()))) {
				position -= Stride();
			}
		}
		return position;
	}
}

int SplitText::LineFromPosition(int position) {
	if (lines == NULL)
		RecreateLines();
	return lines->PartitionFromPosition(position);
}

void SplitText::AddPerLineData(LineData *pld) {
	pld->next = pldRoot;
	pldRoot = pld;
	// Synchronize to current number of lines
	for (int i=0; i < Lines(); i++) {
		pld->InsertLine(i);
	}
}

void SplitText::RemovePerLineData(LineData *pld) {
	if (pldRoot != NULL) {
		if (pldRoot == pld) {
			pldRoot = pld->next;
			pld->next = NULL;
		} else {
			LineData *pldBase = pldRoot;
			while ((pldBase->next != NULL) && (pldBase->next != pld)) {
				pldBase = pldBase->next;
			}
			if (pldBase->next != NULL) {
				pldBase->next = pld->next;
				pld->next = NULL;
			}
		}
	}
}

void SplitText::SetUndoCollection(bool collectUndo) {
	if (collectUndo) {
		if (us == NULL)
			us = new UndoStack(1000, 1000);
		us->DropUndoSequence();
	} else {
		delete us;
		us = NULL;
	}
}

bool SplitText::IsCollectingUndo() {
	return us != NULL;
}

void SplitText::BeginUndoAction() {
	us->BeginUndoAction();
}

void SplitText::EndUndoAction() {
	us->EndUndoAction();
}

void SplitText::DeleteUndoHistory() {
	delete us;
	us = new UndoStack(1000, 1000);
}

bool SplitText::CanUndo() {
	return us->CanUndo();
}

int SplitText::StartUndo() {
	return us->StartUndo();
}

UndoAction *SplitText::GetUndoStep() {
	UndoAction *action = us->GetUndoStep();
	int at = action->actionType;
	at &= ~UndoStack::ACTION_START;
	action->data = new SW_BYTE[action->lenData];
	if (UndoStack::ACTION_INSERT == at) {
		substance->RetrieveString(action->position, action->data, action->lenData);
		// Add to inserts so can be redone
		us->AppendInsertDataStep(action);
	} else {
		us->RetrieveRemoveDataStep(action);
	}
	return action;
}

int SplitText::PerformUndoAction() {
	UndoAction *action = GetUndoStep();
	int position = action->position;
	if (action->actionType == UndoStack::ACTION_INSERT) {
		BasicDeleteRange(action->position, action->lenData);
	} else if (action->actionType == UndoStack::ACTION_REMOVE) {
		BasicInsertBytes(action->position, action->data, action->lenData);
	}
	us->CompletedUndoStep();
	delete action;
	return position;
}

int SplitText::PerformUndoStep() {
	int position = -1;
	int steps = StartUndo();
	for (int i=0;i<steps;i++)
		position = PerformUndoAction();
	return position;
}

bool SplitText::CanRedo() {
	return us->CanRedo();
}

int SplitText::StartRedo() {
	return us->StartRedo();
}

UndoAction *SplitText::GetRedoStep() {
	return us->GetRedoStep();
}

int SplitText::PerformRedoAction() {
	UndoAction *action = GetRedoStep();
	int position = action->position;
	if (action->actionType == UndoStack::ACTION_INSERT) {
		BasicInsertBytes(action->position, action->data, action->lenData);
	} else if (action->actionType == UndoStack::ACTION_REMOVE) {
		BasicDeleteRange(action->position, action->lenData);
	}
	us->CompletedRedoStep();
	delete action;
	return position;
}

int SplitText::PerformRedoStep() {
	int position = -1;
	int steps = StartRedo();
	for (int i=0;i<steps;i++)
		position = PerformRedoAction();
	return position;
}

int SplitText::ClampPositionIntoDocument(int position) {
	int len = Length();
	if (position > len)
		return len;
	if (position < 0)
		return 0;
	return position;
}

void SplitText::SetEncoding(int encoding_) {
	encoding = encoding_;
	delete substance;
	switch (encoding_) {
		case ENCODING_BYTE:
			substance = new SubstanceByte();
			break;
		case ENCODING_UTF8:
			substance = new SubstanceUTF8();
			break;
		case ENCODING_UTF16LE:
			substance = new SubstanceUTF16LE();
			break;
		case ENCODING_UTF16BE:
			substance = new SubstanceUTF16BE();
			break;
	}
	substance->Create(initialLength, growSize);
}

// Normalise a position so that it is not halfway through a multi byte character.
// This can occur in two situations -
// When lines are terminated with \r\n pairs which should be treated as one character.
// When displaying DBCS text such as Japanese.
// If moving, move the position in the indicated direction.
int SplitText::MovePositionOutsideChar(int position, int moveDirection, bool checkLineEnd) {
	// If out of range, just return value - should be fixed up after
	if (position < 0)
		return position;
	if (position > Length())
		return position;

	// Position 0 and Length() can not be between any two characters
	if (position == 0)
		return position;
	if (position == Length())
		return position;

	// assert position > 0 && position < Length()
	if (checkLineEnd && substance->IsCrLf(position - Stride())) {
		if (moveDirection > 0)
			return position + Stride();
		else
			return position - Stride();
	}

	// Not between CR and LF
	return substance->MovePositionOutsideChar(position, moveDirection, checkLineEnd);
}

