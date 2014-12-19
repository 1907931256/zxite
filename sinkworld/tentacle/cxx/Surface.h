// SinkWorld graphics classes.
// Copyright 2004 Neil Hodgson

/// A graphical coordinate.

class Point {
public:
	int x;
	int y;

	Point(int x_=0, int y_=0) : x(x_), y(y_) {
	}

	bool operator==(const Point &other) const {
		return x == other.x && y == other.y;
	}

	// Other automatically defined methods (assignment, copy constructor, destructor) are fine
	int DistanceSquared(const Point &other) const;
};

/// A graphical rectangle.

struct SRectangle {
	int left;
	int top;
	int right;
	int bottom;
	SRectangle() {
		left = 0;
		top = 0;
		right = 0;
		bottom = 0;
	}
	SRectangle(const SRectangle &other) {
		left = other.left;
		top = other.top;
		right = other.right;
		bottom = other.bottom;
	}
	SRectangle(int left_, int top_, int right_, int bottom_) {
		left = left_;
		top = top_;
		right = right_;
		bottom = bottom_;
	}
};

class SurfaceDetails;

/// A place to draw graphics.

SW_PUBLIC class Surface {
	SurfaceDetails *details;
public:
	static Literal *fontDefault;
	void *hdc;
	Surface(void *hdc_, void *wid);
	~Surface();
	void SetFont(const FontDescription &font_);
	void SetTextColor(RGBColor color);
	void SetPen(RGBColor color);
#ifndef unix
	void SetBitmap(void *hbm_);
#endif
	void Free();
	void FillRectangle(SRectangle rc, RGBColor color);
	void FrameRectangle(SRectangle rc, RGBColor color);
	void Polygon(Point *pts, int npts, RGBColor fore, int alphaFill, int alphaOutline);
	void MoveTo(int x, int y);
	void LineTo(int x, int y);
	void SetTransparent(bool transparent);
	void MeasureWidths(SW_BYTE *s, int len, TextPositions *positions, int enc);
	void MeasureWidths(SplitText *s, int position, int len, TextPositions *positions);
	void PaintText(SW_BYTE *s, int len, SRectangle *rcw, int ascentLine, int enc);
	void PaintText(SplitText *s, int position, int len, SRectangle *rcw, int ascentLine);
	int TextAscent();
	int TextDescent();
	void SetClip(SRectangle *rc);
	static void Init();
	static void Finalize();
	enum {lineExtra = 1};
};

/// A surface to paint on and the rectangle within that surface that needs painting.

class PaintContext {
public:
	Surface *surface;
	SRectangle rc;
	PaintContext(Surface *surface_, SRectangle rc_) : surface(surface_), rc(rc_) {
	}
};
