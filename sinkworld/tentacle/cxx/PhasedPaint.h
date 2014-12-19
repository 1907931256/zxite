// SinkWorld Phased Paint classes.
// Copyright 2004 Neil Hodgson
// File written in C~ subset of C++ for automatic translation to Java and C#.

/// A bundle of data passed into the different paint phases.

class PaintParameters {
public:
	SplitText *pbuffer;
	StyleSetCollection *ssc;
	PhysicalLayout *fs;
	Surface *surface;
	int leftColumn;
	int magnification;
	bool visibleWhitespace;
	int lineWidth;
	int ascent;
	int lineDoc;
	int subLine;
	int subLineStart;
	int subLineEnd;
	SRectangle rcw;
	int left;
	const SetIntegers *cf;
	const TextPositions *positions;
};

/// Drawing in the text column is performed in multiple phases starting with the background 
/// and ending at the foreground. 
/// The set of phases is currently background, background decoration, text and 
/// foreground decoration but this may be augmented for future needs.

class IPaintPhase {
public:
	virtual ~IPaintPhase() {
	}
	virtual int FlagsUse() = 0;
	virtual void Paint(PaintParameters *pp) = 0;
};

/// Draw the background fill colors.

class PhaseBack : public IPaintPhase {
public:
	int FlagsUse();
	void Paint(PaintParameters *pp);
};

/// Draw the text characters and white space symbols.

class PhaseText : public IPaintPhase {
	static void DrawTabArrow(Surface *surface, SRectangle rcw, RGBColor fore);
public:
	int FlagsUse();
	void Paint(PaintParameters *pp);
};

/// Draw multi-line boxes.

class OneCorner {	// Helper class
public:
	Surface *surface;
	int x;
	int y;
	int xPrevious;
	int xNext;
	OneCorner(Surface *surface_, int x_, int y_, int xPrevious_, int xNext_);
	void Draw(int quadrant);
	static void DrawCorner(Surface *surface, int x, int y, int xDir, int yDir, int xSpace, int corner);
};

class BoxLayer {
public:
	RunStyles *rsPrevLine;
	long color;
};

class PhaseBox : public IPaintPhase {
	int layerMax;
	BoxLayer **boxLayers;
public:
	enum { cornerSize = 2 };
	PhaseBox();
	virtual ~PhaseBox();
	int FlagsUse();
	void PaintCorner(Surface *surface, int x, int y, int val, int valPrevious, int xPrevious, int xNext);
	void Paint(PaintParameters *pp);
};

/// Draw decorations such as underlines or boxes either below the text or over the text.

class PhaseDecoration : public IPaintPhase {
	bool fore;
public:
	PhaseDecoration(bool fore_) {
		fore = fore_;
	}
	int FlagsUse();
	void Paint(PaintParameters *pp);
};
