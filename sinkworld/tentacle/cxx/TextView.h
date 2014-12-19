// SinkWorld TextView class.
// Copyright 2004 Neil Hodgson
// File written in C~ subset of C++ for automatic translation to Java and C#.

/// A single column of a TextView.

class TextColumn {
public:
	TextColumn *next;
	enum { tcText = 0 };
	enum { tcLineNumber = 1 };
	enum { tcSeparator = 2 };
	enum { tcWhite = 3 };
	enum { tcFold = 4 };
	int type;
	int width;
	int left;
	int right;
	TextColumn() {
		next = NULL;
		type = tcSeparator;
		width = 1;
		left = 0;
		right = 1;
	}
};

/// Draws the text of a buffer into rectangular portion of a window.
/// Divides the rectangle up into multiple columns, for displaying aspects
/// of the buffer including the text and line numbers as well as separator
/// columns.

SW_PUBLIC class TextView {
private:
	// Owned by container
	SplitText *pbuffer;
	// Specific to TextView
	LinePositionCache *linePositions;
	ChangePositionsCache *changePositions;
	PositionCache *pc;
	bool realised;
	TextColumn *root;
	StyleSetCollection *ssc;
	StyleSetCollection *sscMeasure;
	IStyles *foldLevels;
	void Realise(Surface *surface);
	void SetStylesMeasure();
public:
	PhysicalLayout *fs;
	int leftColumn;
	int textLeft;
	bool wrap;
	int wrapWidth;
	int magnification;
	bool visibleWhitespace;
	int lineHeight;
	int pageHeight;
	int lineWidth;
	bool accurateWidths;
	int ascent;
	int topLine;
	// Should move into style settings
	int tabWidth;
	TextView();
	~TextView();
	void Deallocate();
	void StylesChanged();
	void SetVisibleWhitespace(bool visibleWhitespace_);
	void SetText(SplitText *pbuffer_, int lineNumberWidth, int foldWidth);
	void SetStyles(StyleSetCollection *ssc_);
	void SetFolds(IStyles *foldLevels_);
	void RemoveStyleSet(StyleSet *ss);
	void AddStyleSet(StyleSet *ss);
	void SetRectangle(SRectangle *rc);
	void ModifyMagnification(int delta);
	void InvalidateChangePositions();
	int DisplayLineFromPosition(int position);
	int PageSize();
	int MaxScrollPos();
	SetIntegers *ChangePositionsFromLine(int line);
	TextPositions *MeasureText(Surface *surface, int line, const SetIntegers *cf);
	int ColumnTypeFromLocation(Point pt);
	int PositionFromSurfaceLocation(Surface *surface, Point pt);
	void WrapLine(Surface *surface, int line);
	void PaintRect(Surface *surface, SRectangle rcPaint);
	void AddColumn(int type, int width);
	static bool IsSpaceOrTab(int ch) {
		return ch == ' ' || ch == '\t';
	}
private:
	void PaintFoldColumn(Surface *surface, SRectangle rcPaint, TextColumn *tc);
	void PaintLineNumberColumn(Surface *surface, SRectangle rcPaint, TextColumn *tc);
	void PaintTextColumn(Surface *surface, SRectangle rcPaint, TextColumn *tc);
};
