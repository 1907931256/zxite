// SinkWorld StyleModification class.
// Copyright 2005 Neil Hodgson
// File written in C~ subset of C++ for automatic translation to Java and C#.

/// A set of modifications to apply to a style.
/// Can be layered together to reflect the combination of several aspects 
/// such as selection over brace nesting over basic syntax.

SW_PUBLIC class StyleModification : public Style {
public:
	int specified;
	StyleModification() {
		specified = sdNone;
	}
	StyleModification(const Style &other) {
		specified = sdNone;
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
	}
#ifdef SW_CXX
	StyleModification &operator=(const Style &other) {
		specified = sdNone;
		font = other.font;
		size = other.size;
		bold = other.bold;
		italics = other.italics;
		underlined = other.underlined;
		fore = other.fore;
		back = other.back;
		decorationFore = other.decorationFore ? other.decorationFore->Clone() : NULL;
		decorationBack = other.decorationBack ? other.decorationBack->Clone() : NULL;
		eolFilled = other.eolFilled;
		return *this;
	}
#endif
	/// For each facet in this that is specified and the corresponding bit in \a flagsModify
	/// is set, change the \a sty to match the facet.
	void Modify(Style &sty, int flagsModify) {
		if ((specified & sdFont & flagsModify) != 0) {
			sty.font = font;
		}
		if ((specified & sdSize & flagsModify) != 0) {
			sty.size = size;
		}
		if ((specified & sdBold & flagsModify) != 0) {
			sty.bold = bold;
		}
		if ((specified & sdItalics & flagsModify) != 0) {
			sty.italics = italics;
		}
		if ((specified & sdUnderlined & flagsModify) != 0) {
			sty.underlined = underlined;
		}
		if ((specified & sdFore & flagsModify) != 0) {
			sty.fore = fore;
		}
		if ((specified & sdBack & flagsModify) != 0) {
			sty.back = back;
		}
		if ((specified & sdSizeRelative & flagsModify) != 0) {
			sty.size += size;
		}
		if ((specified & sdDecorationFore & flagsModify) != 0) {
			sty.decorationFore = (decorationFore != NULL) ? decorationFore->Clone() : NULL;
		}
		if ((specified & sdDecorationBack & flagsModify) != 0) {
			sty.decorationBack = (decorationBack != NULL) ? decorationBack->Clone() : NULL;
		}
		if ((specified & sdEOLFilled & flagsModify) != 0) {
			sty.eolFilled = eolFilled;
		}
		if ((specified & sdBox & flagsModify) != 0) {
			sty.box = box;
		}
	}
	void AddFont(Literal *font_) {
		font = font_;
		specified = specified | sdFont;
	}
	void AddSize(int size_) {
		size = size_;
		specified = specified | sdSize;
	}
	void AddBold(bool bold_) {
		bold = bold_;
		specified = specified | sdBold;
	}
	void AddItalics(bool italics_) {
		italics = italics_;
		specified = specified | sdItalics;
	}
	void AddFore(RGBColor fore_) {
		fore = fore_;
		specified = specified | sdFore;
	}
	void AddBack(RGBColor back_) {
		back = back_;
		specified = specified | sdBack;
	}
	void AddDecorationFore(IDecoration *decorationFore_) {
		delete decorationFore;
		decorationFore = decorationFore_;
		specified = specified | sdDecorationFore;
	}
	void AddDecorationBack(IDecoration *decorationBack_) {
		delete decorationBack;
		decorationBack = decorationBack_;
		specified = specified | sdDecorationBack;
	}
	void AddEOLFilled(bool eolFilled_) {
		eolFilled = eolFilled_;
		specified = specified | sdEOLFilled;
	}
	void AddBox(RGBColor box_) {
		box = box_;
		specified = specified | sdBox;
	}
	/// Convert from a hex character to a number between 0 and 15.
	static long DigitValue(int ch);
	/// Convert from a hex color value like #FF7F00 to an integer value.
	static long ColourFromLiteral(Literal *value);
	/// Add to facet \a name \a value and mark the facet as specified.
	void AddFacet(Literal *name, Literal *value, LiteralSet *fontNames);
	/// Add each facet in \a styleSeg relative to \a mode.
	static StyleModification *FromOptions(Literal *mode, Literal *styleSeg, OptionSet *options, LiteralSet *fontNames);
};
