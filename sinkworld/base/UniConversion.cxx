/// Methods for converting between Unicode formats.
/// Copyright 2004 Neil Hodgson

#include <string.h>
#include <assert.h>
#include <string>

#include "base.h"

int UniConversion::UTF8ByteLength(int leadByte) {
	if (leadByte < 0x80) {
		return 1;
	} else if (leadByte < 0x80 + 0x40 + 0x20) {
		return 2;
	} else if (leadByte < 0x80 + 0x40 + 0x20 + 0x10) {
		return 3;
	} else {
		return 4;
	}
}

bool UniConversion::UTF8Validate(const SW_BYTE *s, int sLength) {
	for (int i = 0; i < sLength; i++) {
		if ((s[i] == 0xc0) || (s[i] == 0xc1) || (s[i] >= 0xf5))
			return false;
		if (s[i] < 0x80) {
			;	// Always OK
		} else {
			int extensionBytes = UTF8ByteLength(s[i]) - 1;
			while ((extensionBytes--) > 0) {
				i++;
				if (i >= sLength)
					return false;
				if ((s[i] & 0xc0) != 0x80)
					return false;
			}
		}
	}
	return true;
}

bool UniConversion::UTF32Validate(const int *s, int sLength) {
	for (int i=0;i<sLength;i++) {
		if ((s[i] < 0) || (s[i] > UniConversion::UNICODE_LAST)) {
			return false;
		}
	}
	return true;
}

int UniConversion::UTF8LengthFromUTF16Char(int ch) {
	if (ch < 0x80) {
		return 1;
	} else if (ch < 0x800) {
		return 2;
	} else if ((ch >= UniConversion::SURROGATE_LEAD_FIRST) &&
		(ch <= UniConversion::SURROGATE_TRAIL_LAST)) {
		// Half a surrogate pair
		return 2;
	} else {
		return 3;
	}
}

int UniConversion::UTF8LengthFromUTF16(const SW_SHORT *s, int sLength) {
	int len = 0;
	for (int i = 0; i < sLength; i++) {
		len += UTF8LengthFromUTF16Char(s[i]);
	}
	return len;
}

int UniConversion::UTF8FromUTF16(SW_BYTE *dest, int destSize, const SW_SHORT *s, int sLength) {
	int destEnd = 0;
	for (int i = 0; ((i < sLength) && (destEnd < destSize)); i++) {
		int ch = 0xFFFF & s[i];
		if (ch < 0x80) {
			dest[destEnd++] = (SW_BYTE)(ch);
		} else if (ch < 0x800) {
			dest[destEnd++] = (SW_BYTE)(0xC0 | (ch >> 6));
			dest[destEnd++] = (SW_BYTE)(0x80 | (ch & 0x3f));
		} else if ((ch >= UniConversion::SURROGATE_LEAD_FIRST) &&
			(ch <= UniConversion::SURROGATE_TRAIL_LAST)) {
			// Half a surrogate pair
			i++;
			ch = 0x10000 + ((ch & 0x3ff) << 10) + (s[i] & 0x3ff);
			dest[destEnd++] = (SW_BYTE)(0xF0 | (ch >> 18));
			dest[destEnd++] = (SW_BYTE)(0x80 | (ch >> 12) & 0x3f);
			dest[destEnd++] = (SW_BYTE)(0x80 | ((ch >> 6) & 0x3f));
			dest[destEnd++] = (SW_BYTE)(0x80 | (ch & 0x3f));
		} else {
			dest[destEnd++] = (SW_BYTE)(0xE0 | (ch >> 12));
			dest[destEnd++] = (SW_BYTE)(0x80 | ((ch >> 6) & 0x3f));
			dest[destEnd++] = (SW_BYTE)(0x80 | (ch & 0x3f));
		}
	}
	return destEnd;
}

int UniConversion::UTF16LengthFromUTF8(const SW_BYTE *s, int sLength) {
	int len = 0;
	for (int i = 0; i < sLength;) {
		int bytes = UTF8ByteLength(s[i]);
		i += bytes;
		len++;
		if (bytes == 4)
			len++;
	}
	return len;
}

int UniConversion::UTF16FromUTF8(SW_SHORT *dest, int destSize, const SW_BYTE *s, int sLength) {
	int destEnd=0;
	int i=0;
	while ((i<sLength) && (destEnd<destSize)) {
		int b = s[i++];
		if (b < 0x80) {
			dest[destEnd] = (SW_SHORT)(b);
		} else if (b < 0x80 + 0x40 + 0x20) {
			dest[destEnd] = (SW_SHORT)((b & 0x1F) << 6);
			b = s[i++];
			dest[destEnd] = (SW_SHORT)(dest[destEnd] + (b & 0x7F));
		} else if (b < 0x80 + 0x40 + 0x20 + 0x10) {
			dest[destEnd] = (SW_SHORT)((b & 0xF) << 12);
			b = s[i++];
			dest[destEnd] = (SW_SHORT)(dest[destEnd] + ((b & 0x7F) << 6));
			b = s[i++];
			dest[destEnd] = (SW_SHORT)(dest[destEnd] + (b & 0x7F));
		} else {
			// Collect into UTF32 then break into surrogate pair
			int ch = (int)((b & 0x7) << 18);
			b = s[i++];
			ch += (int)((b & 0x3F) << 12);
			b = s[i++];
			ch += (b & 0x3F) << 6;
			b = s[i++];
			ch += b & 0x3F;
			ch -= 0x10000;

			dest[destEnd] = (SW_SHORT)(((ch - 0x10000) >> 10) + UniConversion::SURROGATE_LEAD_FIRST);
			destEnd++;
			dest[destEnd] = (SW_SHORT)((ch & 0x3ff) + UniConversion::SURROGATE_TRAIL_FIRST);
		}
		destEnd++;
	}
	return destEnd;
}

int UniConversion::UTF8LengthFromUTF32Char(int ch) {
	if (ch < 0x80) {
		return 1;
	} else if (ch < 0x800) {
		return 2;
	} else if (ch < 0x10000) {
		return 3;
	} else {
		return 4;
	}
}

int UniConversion::UTF8LengthFromUTF32(const int *s, int sLength) {
	int len = 0;
	for (int i = 0; i < sLength; i++) {
		len += UTF8LengthFromUTF32Char(s[i]);
	}
	return len;
}

int UniConversion::UTF8FromUTF32(SW_BYTE *dest, int destSize, const int *s, int sLength) {
	int destEnd = 0;
	for (int i = 0; ((i < sLength) && (destEnd < destSize)); i++) {
		int ch = s[i];
		if (ch < 0x80) {
			dest[destEnd++] = (SW_BYTE)(ch);
		} else if (ch < 0x800) {
			dest[destEnd++] = (SW_BYTE)(0xC0 | (ch >> 6));
			dest[destEnd++] = (SW_BYTE)(0x80 | (ch & 0x3f));
		} else if (ch < 0x10000) {
			dest[destEnd++] = (SW_BYTE)(0xE0 | (ch >> 12));
			dest[destEnd++] = (SW_BYTE)(0x80 | ((ch >> 6) & 0x3f));
			dest[destEnd++] = (SW_BYTE)(0x80 | (ch & 0x3f));
		} else {
			dest[destEnd++] = (SW_BYTE)(0xF0 | (ch >> 18));
			dest[destEnd++] = (SW_BYTE)(0x80 | (ch >> 12) & 0x3f);
			dest[destEnd++] = (SW_BYTE)(0x80 | ((ch >> 6) & 0x3f));
			dest[destEnd++] = (SW_BYTE)(0x80 | (ch & 0x3f));
		}
	}
	return destEnd;
}

int UniConversion::UTF32LengthFromUTF8(const SW_BYTE *s, int sLength) {
	int len = 0;
	for (int i = 0; i < sLength;) {
		int bytes = UTF8ByteLength(s[i]);
		i += bytes;
		len++;
	}
	return len;
}

int UniConversion::UTF32FromUTF8(int *dest, int destSize, const SW_BYTE *s, int sLength) {
	int destEnd = 0;
	int i = 0;
	while ((i<sLength) && (destEnd<destSize)) {
		SW_BYTE ch = s[i++];
		int value = 0;
		if (ch < 0x80) {
			value = ch;
		} else if (ch < 0x80 + 0x40 + 0x20) {
			value = (ch & 0x1F) << 6;
			ch = s[i++];
			value += ch & 0x7F;
		} else if (ch < 0x80 + 0x40 + 0x20 + 0x10) {
			value = (ch & 0xF) << 12;
			ch = s[i++];
			value += (ch & 0x3F) << 6;
			ch = s[i++];
			value += ch & 0x3F;
		} else {
			value = (int)((ch & 0x7) << 18);
			ch = s[i++];
			value += (ch & 0x3F) << 12;
			ch = s[i++];
			value += (ch & 0x3F) << 6;
			ch = s[i++];
			value += ch & 0x3F;
		}
		dest[destEnd] = value;
		destEnd++;
	}
	return destEnd;
}

int UniConversion::UTF16LengthFromUTF32Char(int ch) {
	if (ch >= 0x10000) {
		return 2;
	} else {
		return 1;
	}
}

int UniConversion::UTF16LengthFromUTF32(const int *s, int sLength) {
	int len = 0;
	for (int i = 0; i < sLength; i++) {
		len += UTF16LengthFromUTF32Char(s[i]);
	}
	return len;
}

int UniConversion::UTF16FromUTF32(SW_SHORT *dest, int destSize, const int *s, int sLength) {
	int destEnd = 0;
	int i = 0;
	while ((i<destSize) && (destEnd<sLength)) {
		int ch = s[i++];
		if (ch >= 0x10000) {
			/// Turn into a surrogate pair
			ch -= 0x10000;
			dest[destEnd] = (SW_SHORT)((ch >> 10) + UniConversion::SURROGATE_LEAD_FIRST);
			destEnd++;
			dest[destEnd] = (SW_SHORT)((ch & 0x3ff) + UniConversion::SURROGATE_TRAIL_FIRST);
			destEnd++;
		} else {
			dest[destEnd] = (SW_SHORT)(ch);
			destEnd++;
		}
	}
	return destEnd;
}

int UniConversion::UTF32LengthFromUTF16(const SW_SHORT *s, int sLength) {
	int len = 0;
	int i = 0;
	while (i<sLength) {
		int ch = s[i++];
		if ((ch >= UniConversion::SURROGATE_LEAD_FIRST) &&
			(ch <= UniConversion::SURROGATE_TRAIL_LAST)) {
			i++;
		}
		len++;
	}
	return len;
}

int UniConversion::UTF32FromUTF16(int *dest, int destSize, const SW_SHORT *s, int sLength) {
	int destEnd = 0;
	int i = 0;
	while ((i<sLength) && (destEnd<destSize)) {
		int value = 0xFFFF & s[i++];
		if ((value >= UniConversion::SURROGATE_LEAD_FIRST) &&
			(value <= UniConversion::SURROGATE_TRAIL_LAST)) {
			value = 0x10000 + ((value & 0x3ff) << 10) + (s[i] & 0x3ff);
			i++;
		}
		dest[destEnd] = value;
		destEnd++;
	}
	return destEnd;
}

