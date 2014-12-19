// SinkWorld PhysicalLayout class.
// Copyright 2004,2005 Neil Hodgson
// File written in C~ subset of C++ for automatic translation to Java and C#.

/// A PhysicalLayout holds information on the physical layout of lines in a view.
/// Folding functionality is implemented by remembering if a line is visible and whether
/// it is expanded. When wrapping is active, multiple display lines may be used for a
/// document line and this height is stored in PhysicalLayout along with the width of the
/// document line in pixels.

SW_PUBLIC class PhysicalLayout : public LineData {
	// These contain 1 element for every document line.
	SplitVector_1 *visible;
	SplitVector_1 *expanded;
	SplitVector_2 *heights;
	SplitVector_4 *widths;
	/// displayLines treats the document as a list of display lines
	/// with each partition representing a document line.
	Partitioning *displayLines;
	MaxFinder *mf;	// Only ever holds values larger than 0
public:
	/// lineFolds partitions the characters of the document into display lines
	Partitioning *lineFolds;
	PhysicalLayout() {
		visible = new SplitVector_1();
		expanded = new SplitVector_1();
		heights = new SplitVector_2();
		widths = NULL;	// Only create widths when needed
		displayLines = new Partitioning(4);
		mf = NULL;
		/// Each docLine has a partition which has a size equivalent to
		/// that lines displayHeight
		lineFolds = new Partitioning(4);
	}
	~PhysicalLayout() {
		delete visible;
		visible = NULL;
		delete expanded;
		expanded = NULL;
		delete heights;
		heights = NULL;
		delete widths;
		widths = NULL;
		delete displayLines;
		displayLines = NULL;
		delete mf;
		mf = NULL;
		delete lineFolds;
		lineFolds = NULL;
	}

	int LinesInDoc() {
		return displayLines->Partitions();
	}
	int LinesDisplayed() {
		return displayLines->Length();
	}
	int DisplayFromDoc(int lineDoc) {
		if (lineDoc > displayLines->Partitions())
			lineDoc = displayLines->Partitions();
		return displayLines->PositionFromPartition(lineDoc);
	}
	int DocFromDisplay(int lineDisplay) {
		return displayLines->PartitionFromPosition(lineDisplay);
	}
	int SubLineFromDisplay(int lineDisplay) {
		int partition = displayLines->PartitionFromPosition(lineDisplay);
		return lineDisplay - DisplayFromDoc(partition);
	}

	bool GetVisible(int lineDoc) {
		if (lineDoc >= visible->Length())
			return true;
		return visible->ValueAt(lineDoc) == 1;
	}
	bool SetVisible(int lineDocStart, int lineDocEnd, bool visible_) {
		int delta = 0;
		if ((lineDocStart <= lineDocEnd) && (lineDocStart >= 0) && (lineDocEnd < LinesInDoc())) {
			for (int line=lineDocStart; line <= lineDocEnd; line++) {
				if (GetVisible(line) != visible_) {
					int difference = visible_ ? heights->ValueAt(line) : -heights->ValueAt(line);
					visible->SetValueAt(line, visible_ ? 1 : 0);
					displayLines->InsertText(line, difference);
					delta += difference;
				}
			}
		}
		//linesInDisplay += delta;
		return delta != 0;
	}

	bool GetExpanded(int lineDoc) {
		return expanded->ValueAt(lineDoc) == 1;
	}
	bool SetExpanded(int lineDoc, bool expanded_) {
		if (expanded_ != (expanded->ValueAt(lineDoc) == 1)) {
			expanded->SetValueAt(lineDoc, expanded_ ? 1 : 0);
			return true;
		} else {
			return false;
		}
	}

	int GetHeight(int lineDoc) {
		return heights->ValueAt(lineDoc);
	}
	bool SetHeight(int lineDoc, int height) {
		if (GetHeight(lineDoc) != height) {
			if (GetVisible(lineDoc)) {
				displayLines->InsertText(lineDoc, height - GetHeight(lineDoc));
			}
			heights->SetValueAt(lineDoc, height);
			return true;
		} else {
			return false;
		}
	}

	int GetWidth(int lineDoc) {
		if (widths == NULL) {
			return -1;
		} else {
			return widths->ValueAt(lineDoc);
		}
	}
	bool SetWidth(int lineDoc, int width) {
		if (widths == NULL) {
			widths = new SplitVector_4();
			mf = new MaxFinder();
			widths->InsertSpace(0, LinesInDoc());
			for (int i=0; i<LinesInDoc(); i++) {
				widths->SetValueAt(i, -1);
			}
		}
		int currentWidth = GetWidth(lineDoc);
		if (currentWidth != width) {
			mf->Replace(currentWidth, width);
			widths->SetValueAt(lineDoc, width);
			return true;
		} else {
			return false;
		}
	}

	int GetMaxWidth() {
		if (mf == NULL)
			return 0;
		else
			return mf->Maximum();
	}

	void ShowAll() {
		SetVisible(0, LinesInDoc(), true);
		for (int i=0;i<LinesInDoc();i++) {
			SetExpanded(i, true);
		}
	}

	void InvalidateWidths() {
		delete widths;
		widths = NULL;
		delete mf;
		mf = NULL;
	}

	SW_OVERRIDE void Clear() {
		delete visible;
		visible = NULL;
		delete expanded;
		expanded = NULL;
		delete heights;
		heights = NULL;
		delete displayLines;
		displayLines = NULL;

		visible = new SplitVector_1();
		expanded = new SplitVector_1();
		heights = new SplitVector_2();
		displayLines = new Partitioning(4);
	}
	SW_OVERRIDE void InsertLine(int lineDoc) {
		visible->Insert(lineDoc, (SW_BYTE)1);
		expanded->Insert(lineDoc, (SW_BYTE)1);
		heights->Insert(lineDoc, (SW_SHORT)1);
		int lineDisplay = DisplayFromDoc(lineDoc);
		displayLines->InsertPartition(lineDoc, lineDisplay);
		displayLines->InsertText(lineDoc, 1);
		if (widths != NULL) {
			widths->Insert(lineDoc, -1);
		}
	}
	SW_OVERRIDE void RemoveLine(int lineDoc) {
		if (GetVisible(lineDoc)) {
			displayLines->InsertText(lineDoc, -heights->ValueAt(lineDoc));
		}
		displayLines->RemovePartition(lineDoc);
		visible->Delete(lineDoc);
		expanded->Delete(lineDoc);
		heights->Delete(lineDoc);
		if (widths != NULL) {
			mf->Remove(GetWidth(lineDoc));
			widths->Delete(lineDoc);
		}
	}
};
