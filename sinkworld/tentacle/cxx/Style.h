// SinkWorld Style class.
// Copyright 2004 Neil Hodgson
// File written in C~ subset of C++ for automatic translation to Java and C#.

/// A set of visual attributes that determine how text is displayed.

SW_PUBLIC class Style {
public:
	enum { sdNone = 0 };
	enum { sdFont = 0x1 };
	enum { sdSize = 0x2 };
	enum { sdBold = 0x4 };
	enum { sdItalics = 0x8 };
	enum { sdUnderlined = 0x10 };
	enum { sdFore = 0x20 };
	enum { sdBack = 0x40 };
	enum { sdSizeRelative = 0x80 };
	enum { sdDecorationFore = 0x100 };
	enum { sdDecorationBack = 0x200 };
	enum { sdEOLFilled = 0x400 };
	enum { sdBox = 0x800 };
	enum { sdAll = 0xffff };
	const Literal *font;
	int size;
	bool bold;
	bool italics;
	bool underlined;
	RGBColor fore;
	RGBColor back;
	IDecoration *decorationFore;
	IDecoration *decorationBack;
	bool eolFilled;
	RGBColor box;
	Style() {
		font = NULL;
		size = 10;
		bold = false;
		italics = false;
		underlined = false;
		fore = RGBColor();
		back = RGBColor(0xff, 0xff, 0xff);
		decorationFore = NULL;
		decorationBack = NULL;
		eolFilled = false;
		box = RGBColor();
	}
	Style(const Literal *font_) {
		font = font_;
		size = 10;
		bold = false;
		italics = false;
		underlined = false;
		fore = RGBColor();
		back = RGBColor(0xff, 0xff, 0xff);
		decorationFore = NULL;
		decorationBack = NULL;
		eolFilled = false;
		box = RGBColor();
	}
	Style(const Literal *font_, int size_, bool bold_, bool italics_, bool underlined_, RGBColor fore_, RGBColor back_) {
		font = font_;
		size = size_;
		bold = bold_;
		italics = italics_;
		underlined = underlined_;
		fore = fore_;
		back = back_;
		decorationFore = NULL;
		decorationBack = NULL;
		eolFilled = false;
		box = RGBColor();
	}
	Style(const Style &other) {
		font = other.font;
		size = other.size;
		bold = other.bold;
		italics = other.italics;
		underlined = other.underlined;
		fore = other.fore;
		back = other.back;
		decorationFore = (other.decorationFore != NULL) ? other.decorationFore->Clone() : NULL;
		decorationBack = (other.decorationBack != NULL) ? other.decorationBack->Clone() : NULL;
		eolFilled = other.eolFilled;
		box = other.box;
	}
	~Style() {
		delete decorationFore;
		delete decorationBack;
	}
#ifdef SW_CXX
	Style &operator=(const Style &other) {
		font = other.font;
		size = other.size;
		bold = other.bold;
		italics = other.italics;
		underlined = other.underlined;
		fore = other.fore;
		back = other.back;
		decorationFore = (other.decorationFore != NULL) ? other.decorationFore->Clone() : NULL;
		decorationBack = (other.decorationBack != NULL) ? other.decorationBack->Clone() : NULL;
		eolFilled = other.eolFilled;
		box = other.box;
		return *this;
	}
#endif
	bool DifferentTo(const Style &other, int check) {
		if ((check & sdSize) != 0) {
			if (size != other.size) {
				return true;
			}
		}
		if ((check & sdBold) != 0) {
			if (bold != other.bold) {
				return true;
			}
		}
		if ((check & sdItalics) != 0) {
			if (italics != other.italics) {
				return true;
			}
		}
		if ((check & sdUnderlined) != 0) {
			if (underlined != other.underlined) {
				return true;
			}
		}
		if ((check & sdFore) != 0) {
			if (fore.NotEqual(other.fore)) {
				return true;
			}
		}
		if ((check & sdBack) != 0) {
			if (back.NotEqual(other.back)) {
				return true;
			}
		}
		if ((check & sdDecorationFore) != 0) {
			if (decorationFore != other.decorationFore) {
				// Do a shallow inaccurate check for speed
				return true;
			}
		}
		if ((check & sdDecorationBack) != 0) {
			if (decorationBack != other.decorationBack) {
				// Do a shallow inaccurate check for speed
				return true;
			}
		}
		if ((check & sdEOLFilled) != 0) {
			if (eolFilled != other.eolFilled) {
				return true;
			}
		}
		if ((check & sdBox) != 0) {
			if (box.NotEqual(other.box)) {
				return true;
			}
		}
		return false;
	}
};
