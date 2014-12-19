// SinkWorld RGBColor class.
// Copyright 2004 Neil Hodgson
// File written in C~ subset of C++ for automatic translation to Java and C#.

/// A color with red green and blue componenets each in the range 0 to 255.

SW_PUBLIC class RGBColor {
public:
	long co;
	RGBColor() {
		co = 0;
	}
	RGBColor(long co_) {
		co = co_;
	}
	RGBColor(long r, long g, long b) {
		co = b*0x10000 + g*0x100 + r;
	}
	long AsBGR() {
		return co;
	}
	long AsRGB() {
		long red = GetRed();
		long green = GetGreen();
		long blue = GetBlue();
		return red*0x10000 + green*0x100 + blue;
	}
	bool NotEqual(const RGBColor &other) {
		return co != other.co;
	}
	long GetRed() const {
		return co & 0xff;
	}
	long GetGreen() const {
		return (co & 0xff00) >> 8;
	}
	long GetBlue() const {
		return (co & 0xff0000) >> 16;
	}
	RGBColor Merge(const RGBColor &other, int proportion) {
		long r = (other.GetRed() * proportion + GetRed() * (256 - proportion)) / 256;
		long g = (other.GetGreen() * proportion + GetGreen() * (256 - proportion)) / 256;
		long b = (other.GetBlue() * proportion + GetBlue() * (256 - proportion)) / 256;
		return RGBColor(r, g, b);
	}
};

