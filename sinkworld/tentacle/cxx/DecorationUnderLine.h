// SinkWorld DecorationUnderLine class.
// Copyright 2004 Neil Hodgson
// File written in C~ subset of C++ for automatic translation to Java and C#.

/// Draws an underline under text.

SW_PUBLIC class DecorationUnderLine : public BaseDecoration {
	RGBColor fore;
public:
	DecorationUnderLine() {
		delete name;
		name = new Literal("UnderLine");
	}
	~DecorationUnderLine() {
	}
	SW_OVERRIDE IPrototype *CreateNew() {
		return new DecorationUnderLine();
	}
	SW_OVERRIDE IDecoration *Clone() {
		IDecoration *ret = new DecorationUnderLine();
		ret->SetFore(fore);
		return ret;
	}
	SW_OVERRIDE void SetFore(RGBColor fore_) {
		fore = fore_;
	}
	SW_OVERRIDE void Paint(PaintContext *pc);
};
