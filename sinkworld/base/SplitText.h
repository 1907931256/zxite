// SinkWorld SplitText class.
// Copyright 2001 Neil Hodgson
// File written in C~ subset of C++ for automatic translation to Java and C#.

class ILexer;

/// StylesWithValidity attaches a lexer to a styling buffer and tracks the
/// range that is validly styled.

class StylesWithValidity {
public:
	StylesWithValidity *next;
	IStyles *style;
	ILexer *lexer;
	int validTo;
	int holeStart;
	int holeEnd;
	int priority;
	bool lineStyles;
	HistorySet *histories;
	StylesWithValidity(IStyles *style_, ILexer *lexer_, int priority_, bool lineStyles_) {
		next = NULL;
		style = style_;
		lexer = lexer_;
		validTo = 0;
		holeStart = -1;
		holeEnd = -1;
		priority = priority_;
		lineStyles = lineStyles_;
		histories = new HistorySet(0);
	}
	~StylesWithValidity() {
		delete histories;
		histories = NULL;
	}
	bool HasHole() const {
		return holeStart >= 0;
	}
	enum { holeDistance = 0 };
	bool InHole(int position) const {
		return HasHole() && (position >= holeStart) && (position <= holeEnd);
	}
};

SW_PUBLIC class SplitText {
private:
	Substance *substance;
	Partitioning *lines;
	LineData *pldRoot;
	int encoding;
	StylesWithValidity *first;
	int initialLength;
	int growSize;
	UndoStack *us;
	bool validating;

	void BasicInsertBytes(int position, SW_BYTE s[], int insertLength);
	void BasicDeleteRange(int position, int deleteLength);

protected:
	void InsertLine(int lineInsert, int position);
	void RemoveLine(int lineRemove);
	void Create(int initialLength_, int growSize_);

public:
	enum { ENCODING_BYTE = 0 };
	enum { ENCODING_UTF8 = 1 };
	enum { ENCODING_UTF16LE = 2 };
	enum { ENCODING_UTF16BE = 3 };

	SplitText(int initialLength_, int growSize_) {
		Create(initialLength_, growSize_);
	}
	SplitText() {
		Create(1024, 1024);
	}
	~SplitText();

	SW_BYTE XCharAt(int position) {
		return substance->CharAt(position);
	}
	// Want unsigned bytes when manipulating but Java only has signed bytes
	// so and with 0xff. This appears to be optimized out for C++ and C#.
	int UnsignedByteAt(int position) {
		return (int)(substance->CharAt(position)) & 0xff;
	}

	int StyleAt(int position);
	int Layers();
	int StyleAtLayer(int position, int layer);
	IStyles *GetLayer(int layer);
	Substance *GetSubstance() { return substance; }
	int Length() {
		return substance->Length();
	}

	void DetachAllStyles();
	void DetachStyles(IStyles *style_);
	void AttachStyles(IStyles *style_, ILexer *lexer_, int priority, bool lineStyles);
	void AttachHistory(ILexer *lexer_, History *history);
	static IStyles *CreateStylesSized(int initialLength, int growSize, int styleWidth);
	static IStyles *CreateStyles(int styleWidth);
	bool LexTo(int position, int priority, bool justTheHole);

	/// Retrieving positions outside the range of the buffer fails
	void RetrieveBytes(int position, SW_BYTE s[], int retrieveLength);
	int HashRange(int position, int len);
	int Characters(int position, int len);
	int RetrieveUTF8(int position, SW_BYTE s[], int retrieveLength);
	int RetrieveUTF16(int position, SW_SHORT s[], int retrieveLength);
	int RetrieveUTF32(int position, int s[], int retrieveLength);
	int LengthInText(const int s[], int sLength);
	void DiscardLines();
	void RecreateLines();
	bool CheckLines();
	void SetValidating(bool validating_);
	bool GetValidating() {
		return validating;
	}
	void InsertBytes(int position, SW_BYTE s[], int insertLength);
	int InsertUTF32(int position, const int *s, int insertLength);
	void DeleteRange(int position, int deleteLength);
	int Lines();
	int PositionFromLine(int line);
	int LineEndPositionFromLine(int line);
	int LineFromPosition(int position);

	void AddPerLineData(LineData *pld);
	void RemovePerLineData(LineData *pld);

	/// Undo and redo
	void SetUndoCollection(bool collectUndo);
	bool IsCollectingUndo();
	void BeginUndoAction();
	void EndUndoAction();
	void DeleteUndoHistory();
	bool CanUndo();
	int StartUndo();
	UndoAction *GetUndoStep();
	int PerformUndoAction();
	int PerformUndoStep();
	bool CanRedo();
	int StartRedo();
	UndoAction *GetRedoStep();
	int PerformRedoAction();
	int PerformRedoStep();
	void SetSavePoint() {
		if (us != NULL) {
			us->SetSavePoint();
		}
	}
	bool IsSavePoint() {
		return (us != NULL) ? us->IsSavePoint() : false;
	}

	int ClampPositionIntoDocument(int position);
	void SetEncoding(int encoding_);
	int GetEncoding() {
		return encoding;
	}
	// LenChar returns the size in bytes of the complete character at \a position.
	int DistanceNextCaret(int position) { return substance->DistanceNextCaret(position); }
	// LenChar returns the size in bytes of the complete character at \a position.
	int LenChar(int position) { return substance->LenChar(position); }
	// PositionNext returns the complete character position after \a position.
	int PositionNext(int position) {
		return position + LenChar(position);
	}
	// PositionBefore returns the complete character position before \a position.
	int PositionBefore(int position) {
		return MovePositionOutsideChar(position-1, -1, false);
	}
	// CharValue returns the value of the complete character after \a position.
	int CharValue(int position) { return substance->CharValue(position); }
	// CharValueBefore returns the value of the complete character before \a position.
	int CharValueBefore(int position) {
		return substance->CharValue(PositionBefore(position));
	}
	// Stride returns the size in bytes of the minimum addressable unit.
	int Stride() {
		return substance->bytesInUnit;
	}
	// Find the nearest position that is between characters in the moveDirection.
	int MovePositionOutsideChar(int position, int moveDirection, bool checkLineEnd);
};
