// SinkWorld UniConversion class.
// Copyright 2004 Neil Hodgson
// File written in C~ subset of C++ for automatic translation to Java and C#.

/// Methods for converting between Unicode formats.
/// All the conversion calls return the length of the output and do not
/// add 0 terminators. Embedded 0 characters are OK.
/// See http://www.unicode.org and http://en.wikipedia.org/wiki/UTF-8

SW_PUBLIC class UniConversion {
public:
	enum { SURROGATE_LEAD_FIRST = 0xD800 };
	enum { SURROGATE_TRAIL_FIRST = 0xDC00 };
	enum { SURROGATE_TRAIL_LAST = 0xDFFF };
	enum { UNICODE_LAST = 0x10FFFF};

	static int UTF8ByteLength(int leadByte);
	static bool UTF8Validate(const SW_BYTE *s, int sLength);
	static bool UTF32Validate(const int *s, int sLength);

	static int UTF8LengthFromUTF16Char(int ch);
	static int UTF8LengthFromUTF16(const SW_SHORT *s, int sLength);
	static int UTF8FromUTF16(SW_BYTE *dest, int destSize, const SW_SHORT *s, int sLength);
	static int UTF16LengthFromUTF8(const SW_BYTE *s, int sLength);
	static int UTF16FromUTF8(SW_SHORT *dest, int destSize, const SW_BYTE *s, int sLength);

	static int UTF8LengthFromUTF32Char(int ch);
	static int UTF8LengthFromUTF32(const int *s, int sLength);
	static int UTF8FromUTF32(SW_BYTE *dest, int destSize, const int *s, int sLength);
	static int UTF32LengthFromUTF8(const SW_BYTE *s, int sLength);
	static int UTF32FromUTF8(int *dest, int destSize, const SW_BYTE *s, int sLength);

	static int UTF16LengthFromUTF32Char(int ch);
	static int UTF16LengthFromUTF32(const int *s, int sLength);
	static int UTF16FromUTF32(SW_SHORT *dest, int destSize, const int *s, int sLength);
	static int UTF32LengthFromUTF16(const SW_SHORT *s, int sLength);
	static int UTF32FromUTF16(int *dest, int destSize, const SW_SHORT *s, int sLength);
};
