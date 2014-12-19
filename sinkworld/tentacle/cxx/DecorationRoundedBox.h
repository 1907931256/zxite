// SinkWorld DecorationRoundedBox class.
// Copyright 2005 Neil Hodgson
// File written in C~ subset of C++ for automatic translation to Java and C#.

/// Draw a rounded box around text.

SW_PUBLIC class DecorationRoundedBox : public BaseDecoration {
	typedef BaseDecoration super;
	RGBColor fore;
	int alpha;
	int alphaOutline;
public:
	DecorationRoundedBox() {
		delete name;
		name = new Literal("RoundedBox");
		alpha = 0x40;
		alphaOutline = 0x40;
	}
	~DecorationRoundedBox() {
	}
	SW_OVERRIDE IPrototype *CreateNew() {
		return new DecorationRoundedBox();
	}
	SW_OVERRIDE IDecoration *Clone() {
		DecorationRoundedBox *ret = new DecorationRoundedBox();
		ret->SetFore(fore);
		ret->alpha = alpha;
		ret->alphaOutline = alphaOutline;
		return ret;
	}
	SW_OVERRIDE void SetFore(RGBColor fore_) {
		fore = fore_;
	}
	SW_OVERRIDE void Paint(PaintContext *pc);
	SW_OVERRIDE void FromOptions(Literal *mode, Literal *facetPath, OptionSet *options);
};
