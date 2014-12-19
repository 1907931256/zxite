// SinkWorld FontDescription class.
// Copyright 2005 Neil Hodgson
// File written in C~ subset of C++ for automatic translation to Java and C#.

/// Set of attributes that define a font: name, size, bold and italics flags.

SW_PUBLIC class FontDescription {
	int hash;
	void SetHash() {
		if (font != NULL)
			hash = font->HashValue();
		else
			hash = 0;
		hash *= 1000003;
		hash ^= height;
		hash *= 1000003;
		hash ^= bold ? 'B' : '-';
		hash *= 1000003;
		hash ^= italics ? 'I' : '-';
	}
public:
	const Literal *font;
	int height;
	bool bold;
	bool italics;
	FontDescription() {
		hash = 0;
		font = NULL;
		height = 0;
		bold = false;
		italics = false;
		SetHash();
	}
	FontDescription(const Literal *font_, int height_, bool bold_, bool italics_) {
		hash = 0;
		font = font_;
		height = height_;
		bold = bold_;
		italics = italics_;
		SetHash();
	}
	FontDescription(const FontDescription &orig) {
		hash = 0;
		font = orig.font;
		height = orig.height;
		bold = orig.bold;
		italics = orig.italics;
		SetHash();
	}
	~FontDescription() {
	}
#ifdef SW_CXX
	FontDescription &operator=(const FontDescription &other) {
		hash = other.hash;
		font = other.font;
		height = other.height;
		bold = other.bold;
		italics = other.italics;
		return *this;
	}
#endif
	int Hash() const {
		return hash;
	}
	bool Equals(const FontDescription &other) {
		return
			(hash == other.hash) &&
			(font == other.font) &&
			(height == other.height) &&
			(bold == other.bold) &&
			(italics == other.italics);
	}
};
