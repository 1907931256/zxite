// SinkWorld DecorationBox class.
// Copyright 2004 Neil Hodgson
// File written in C~ subset of C++ for automatic translation to Java and C#.

/// Draw a box around text.

SW_PUBLIC class DecorationBox : public BaseDecoration {
	RGBColor fore;
public:
	DecorationBox() {
		delete name;
		name = new Literal("Box");
	}
	~DecorationBox() {
	}
	SW_OVERRIDE IPrototype *CreateNew() {
		return new DecorationBox();
	}
	SW_OVERRIDE IDecoration *Clone() {
		IDecoration *ret = new DecorationBox();
		ret->SetFore(fore);
		return ret;
	}
	SW_OVERRIDE void SetFore(RGBColor fore_) {
		fore = fore_;
	}
	SW_OVERRIDE void Paint(PaintContext *pc);
};
