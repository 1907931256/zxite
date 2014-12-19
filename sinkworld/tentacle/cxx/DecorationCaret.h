// SinkWorld DecorationCaret class.
// Copyright 2004 Neil Hodgson
// File written in C~ subset of C++ for automatic translation to Java and C#.

/// Draw a caret, which is a thin vertical line used to indicate where input will go.

SW_PUBLIC class DecorationCaret : public BaseDecoration {
	RGBColor fore;
	int width;
public:
	DecorationCaret() {
		delete name;
		name = new Literal("Caret");
		width = 1;
	}
	DecorationCaret(int width_) {
		delete name;
		name = new Literal("Caret");
		width = width_;
	}
	~DecorationCaret() {
	}
	SW_OVERRIDE IPrototype *CreateNew() {
		return new DecorationCaret();
	}
	SW_OVERRIDE IDecoration *Clone() {
		IDecoration *ret = new DecorationCaret(width);
		ret->SetFore(fore);
		return ret;
	}
	SW_OVERRIDE void SetFore(RGBColor fore_) {
		fore = fore_;
	}
	SW_OVERRIDE void Paint(PaintContext *pc);
};
