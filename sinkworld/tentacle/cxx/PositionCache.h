// SinkWorld PositionCache class.
// Copyright 2004 Neil Hodgson
// File written in C~ subset of C++ for automatic translation to Java and C#.

/// Pixel positions of each character in a string when displayed with a particular font.

class PositionCacheEntry {
public:
	FontDescription fd;
	SW_BYTE *s;
	int len;
	int clock;
	TextPositions *positions;
	PositionCacheEntry() {
		s = NULL;
		len = 0;
		clock = 0;
		positions = NULL;
	}
	~PositionCacheEntry() {
		delete []s;
		s = NULL;
		delete positions;
		positions = NULL;
	}
	bool Match(const FontDescription &fd_, SplitText *st, int position, int len_) {
		if (positions == NULL)
			return false;
		bool equal = len == len_;
		for (int i=0;equal && i<len;i++) {
			equal = equal && (s[i] == st->UnsignedByteAt(position+i));
		}
		return equal && (fd.Equals(fd_));
	}
	void Set(const FontDescription &fd_, SplitText *st, int position, int len_, TextPositions *positions_, int clock_) {
		fd = fd_;
		delete []s;
		s = new SW_BYTE[len_+1];
		st->RetrieveBytes(position, s, len_);
		len = len_;
		s[len_] = 0;
		clock = clock_;
		delete positions;
		positions = positions_;
	}
};

/// A two way set associative cache of pixel positions of text strings as displayed in a font.
/// This avoids calling the graphics layer to layout text.
/// Reduces calls by about 90%.
/// The double probe is only about a 10% win but is retained in case there
/// situations with more conflicts and hence more benefit.
/// This cache can be long lived as the values completely defined by the
/// font fields and input text although it is possible that it should be
/// cleared if the graphics layer changes rendering technique or the window
/// is moved onto another display.

SW_PUBLIC class PositionCache {
	int size;
	int mask;
	int clock;
	PositionCacheEntry **pces;
public:
	PositionCache();
	~PositionCache();
	void MeasureStyledText(Surface *surface, const FontDescription &fd, SplitText *s, int position, int len, TextPositions *positions);
};
