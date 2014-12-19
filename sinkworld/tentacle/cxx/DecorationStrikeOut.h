// SinkWorld DecorationStrikeOut class.
// Copyright 2004 Neil Hodgson
// File written in C~ subset of C++ for automatic translation to Java and C#.

/// Draws a line through the middle of some text.

SW_PUBLIC class DecorationStrikeOut : public BaseDecoration {
	RGBColor fore;
public:
	DecorationStrikeOut() {
		delete name;
		name = new Literal("StrikeOut");
	}
	~DecorationStrikeOut() {
	}
	SW_OVERRIDE IPrototype *CreateNew() {
		return new DecorationStrikeOut();
	}
	SW_OVERRIDE IDecoration *Clone() {
		IDecoration *ret = new DecorationStrikeOut();
		ret->SetFore(fore);
		return ret;
	}
	SW_OVERRIDE void SetFore(RGBColor fore_) {
		fore = fore_;
	}
	SW_OVERRIDE void Paint(PaintContext *pc);
};
