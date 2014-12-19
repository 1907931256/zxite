// SinkWorld DecorationButton class.
// Copyright 2004 Neil Hodgson
// File written in C~ subset of C++ for automatic translation to Java and C#.

/// Draw a bevelled button around some text in either an up or pressed state.

SW_PUBLIC class DecorationButton : public BaseDecoration {
	bool pressed;
public:
	DecorationButton() {
		delete name;
		name = new Literal("Button");
		pressed = true;
	}
	DecorationButton(bool pressed_) {
		delete name;
		name = new Literal("Button");
		pressed = pressed_;
	}
	~DecorationButton() {
	}
	SW_OVERRIDE IPrototype *CreateNew() {
		return new DecorationButton();
	}
	SW_OVERRIDE IDecoration *Clone() {
		return new DecorationButton(pressed);
	}
	SW_OVERRIDE void SetFore(RGBColor fore_) {
	}
	SW_OVERRIDE void Paint(PaintContext *pc);
};
