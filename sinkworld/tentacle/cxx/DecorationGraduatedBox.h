// SinkWorld DecorationGraduatedBox class.
// Copyright 2004 Neil Hodgson
// File written in C~ subset of C++ for automatic translation to Java and C#.

/// Draws a set of three nested rectangles around text starting with the fore color
/// as the innermost with lighter shades surrounding.

SW_PUBLIC class DecorationGraduatedBox : public BaseDecoration {
	RGBColor fore;
public:
	DecorationGraduatedBox() {
		delete name;
		name = new Literal("GraduatedBox");
	}
	~DecorationGraduatedBox() {
	}
	SW_OVERRIDE IPrototype *CreateNew() {
		return new DecorationGraduatedBox();
	}
	SW_OVERRIDE IDecoration *Clone() {
		IDecoration *ret = new DecorationGraduatedBox();
		ret->SetFore(fore);
		return ret;
	}
	SW_OVERRIDE void SetFore(RGBColor fore_) {
		fore = fore_;
	}
	SW_OVERRIDE void Paint(PaintContext *pc);
};
