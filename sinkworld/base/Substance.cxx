// SinkWorld Substance classes.
// Copyright 2005 Neil Hodgson
// File written in C~ subset of C++ for automatic translation to Java and C#.

#include <string.h>
#include <assert.h>
#include <string>

#include "base.h"

SubstanceUTF8::~SubstanceUTF8() {
}
int SubstanceUTF8::DistanceNextCaret(int position) {
	// Up to 4 byte characters.
	int u = UnsignedByteAt(position);
	if (u == '\r') {
		if (IsCrLf(position))
			return 2;
		return 1;
	} else {
		return UniConversion::UTF8ByteLength(u);
	}
}
int SubstanceUTF8::LenChar(int position) {
	// Up to 4 byte characters.
	int u = UnsignedByteAt(position);
	return UniConversion::UTF8ByteLength(u);
}
int SubstanceUTF8::LenCharacters(const int *s, int sLength) {
	return UniConversion::UTF8LengthFromUTF32(s, sLength);
}
int SubstanceUTF8::BytesFromCharacters(SW_BYTE *dest, int destSize, const int *s, int sLength) {
	return UniConversion::UTF8FromUTF32(dest, destSize, s, sLength);
}
int SubstanceUTF8::RetrieveUTF8(int position, SW_BYTE s[], int retrieveLength) {
	RetrieveString(position, s, retrieveLength);
	return retrieveLength;
}
int SubstanceUTF8::RetrieveUTF16(int position, SW_SHORT s[], int retrieveLength) {
	SW_BYTE *bytes = new SW_BYTE[retrieveLength];
	RetrieveString(position, bytes, retrieveLength);
	int len = UniConversion::UTF16FromUTF8(s, retrieveLength, bytes, retrieveLength);
	delete []bytes;
	return len;
}
int SubstanceUTF8::RetrieveUTF32(int position, int s[], int retrieveLength) {
	SW_BYTE *bytes = new SW_BYTE[retrieveLength];
	RetrieveString(position, bytes, retrieveLength);
	int len = UniConversion::UTF32FromUTF8(s, retrieveLength, bytes, retrieveLength);
	delete []bytes;
	return len;
}
int SubstanceUTF8::LengthInText(const int s[], int sLength) {
	return UniConversion::UTF8LengthFromUTF32(s, sLength);
}
int SubstanceUTF8::UnitValue(int position) {
	return UnsignedByteAt(position);
}
int SubstanceUTF8::UnitFromArray(SW_BYTE s[], int position) {
	return (int)(s[position]) & 0xff;
}
int SubstanceUTF8::CharValue(int position) {
	int value = 0;
	int ch = UnsignedByteAt(position);
	if (ch < 0x80) {
		value = ch;
	} else if (ch < 0x80 + 0x40 + 0x20) {
		value = (ch & 0x1F) << 6;
		ch = CharAt(position+1);
		value += ch & 0x7F;
	} else if (ch < 0x80 + 0x40 + 0x20 + 0x10) {
		value = (ch & 0xF) << 12;
		ch = CharAt(position+1);
		value += (ch & 0x3F) << 6;
		ch = CharAt(position+2);
		value += ch & 0x3F;
	} else {
		value = (int)((ch & 0x7) << 18);
		ch = CharAt(position+1);
		value += (ch & 0x3F) << 12;
		ch = CharAt(position+2);
		value += (ch & 0x3F) << 6;
		ch = CharAt(position+3);
		value += ch & 0x3F;
	}
	return value;
}
int SubstanceUTF8::MovePositionOutsideChar(int position, int moveDirection, bool checkLineEnd) {
	if ((position > 0) && (position < Length())) {
		int ch = UnsignedByteAt(position);
		while ((position > 0) && (position < Length()) && (ch >= 0x80) && (ch < (0x80 + 0x40))) {
			// ch is a trail byte
			if (moveDirection > 0)
				position++;
			else
				position--;
			ch = UnsignedByteAt(position);
		}
		if (checkLineEnd && (ch == '\n') && IsCrLf(position - 1)) {
			if (moveDirection > 0)
				position++;
			else
				position--;
		}
	}
	return position;
}
bool SubstanceUTF8::BetweenCharacters(int position) {
	if (position < 0) {
		return false;
	} else if (position > Length()) {
		return false;
	} else if (position == Length()) {
		return true;
	} else {
		int b = UnsignedByteAt(position);
		if (b < 0x80) {
			// ASCII range
			return true;
		} else if ((b == 0xc0) || (b == 0xc1) || (b >= 0xf5)) {
			// Invalid byte
			return false;
		} else if (b >= 0x80 + 0x40) {
			// Lead byte
			return true;
		} else {
			// Before trail byte
			return false;
		}
	}
}
bool SubstanceUTF8::ValidateRange(int position, int validateLength) {
	for (int i = 0; i < Length(); i++) {
		int b = UnsignedByteAt(i);
		if ((b == 0xc0) || (b == 0xc1) || (b >= 0xf5))
			return false;
		if (b < 0x80) {
			;	// Always OK
		} else {
			/// check for trail bytes
			int extensionBytes = UniConversion::UTF8ByteLength(b) - 1;
			while ((extensionBytes--) > 0) {
				i++;
				if (i >= Length())
					return false;
				if ((UnsignedByteAt(i) & 0xc0) != 0x80)
					return false;
			}
		}
	}
	return true;
}
bool SubstanceUTF8::ValidateArray(SW_BYTE s[], int sLength) {
	return true;
}

SubstanceByte::~SubstanceByte() {
}
int SubstanceByte::DistanceNextCaret(int position) {
	int u = UnsignedByteAt(position);
	if (u == '\r') {
		if (IsCrLf(position))
			return 2;
	}
	return 1;
}
int SubstanceByte::LenChar(int position) {
	return 1;
}
int SubstanceByte::LenCharacters(const int *s, int sLength) {
	return sLength;
}
int SubstanceByte::BytesFromCharacters(SW_BYTE *dest, int destSize, const int *s, int sLength) {
	int i = 0;
	for (; (i<sLength) && (i<destSize); i++) {
		dest[i] = (SW_BYTE)s[i];
	}
	return i;
}
int SubstanceByte::RetrieveUTF8(int position, SW_BYTE s[], int retrieveLength) {
	RetrieveString(position, s, retrieveLength);
	return retrieveLength;
}
int SubstanceByte::RetrieveUTF16(int position, SW_SHORT s[], int retrieveLength) {
	SW_BYTE *bytes = new SW_BYTE[retrieveLength];
	RetrieveString(position, bytes, retrieveLength);
	for (int i=0; i<retrieveLength; i++) {
		s[i] = bytes[i];
	}
	delete []bytes;
	return retrieveLength;
}
int SubstanceByte::RetrieveUTF32(int position, int s[], int retrieveLength) {
	SW_BYTE *bytes = new SW_BYTE[retrieveLength];
	RetrieveString(position, bytes, retrieveLength);
	for (int i=0; i<retrieveLength; i++) {
		s[i] = bytes[i];
	}
	delete []bytes;
	return retrieveLength;
}
int SubstanceByte::LengthInText(const int s[], int sLength) {
	return sLength;
}
int SubstanceByte::UnitValue(int position) {
	return UnsignedByteAt(position);
}
int SubstanceByte::UnitFromArray(SW_BYTE s[], int position) {
	return (int)(s[position]) & 0xff;
}
int SubstanceByte::CharValue(int position) {
	return UnsignedByteAt(position);
}
int SubstanceByte::MovePositionOutsideChar(int position, int moveDirection, bool checkLineEnd) {
	if ((position > 0) && (position < Length())) {
		if (checkLineEnd && IsCrLf(position - 1)) {
			if (moveDirection > 0)
				position++;
			else
				position--;
		}
	}
	return position;
}
bool SubstanceByte::BetweenCharacters(int position) {
	return true;
}
bool SubstanceByte::ValidateRange(int position, int validateLength) {
	return true;
}
bool SubstanceByte::ValidateArray(SW_BYTE s[], int sLength) {
	return true;
}

int SubstanceUTF16::DistanceNextCaret(int position) {
	return IsCrLf(position) ? 4 : LenChar(position);
}
int SubstanceUTF16::LenChar(int position) {
	int u = UnitValue(position);
	if ((u >= UniConversion::SURROGATE_LEAD_FIRST) &&
		(u <= UniConversion::SURROGATE_TRAIL_LAST))
		return 4;
	return 2;
}
int SubstanceUTF16::LenCharacters(const int *s, int sLength) {
	return UniConversion::UTF16LengthFromUTF32(s, sLength) * 2;
}
int SubstanceUTF16::RetrieveUTF8(int position, SW_BYTE s[], int retrieveLength) {
	SW_SHORT *w = new SW_SHORT[retrieveLength];
	int pos = position;
	int len = 0;
	while (pos < (position+retrieveLength)) {
		w[len] = (SW_SHORT)(UnitValue(pos));
		pos += 2;
		len++;
	}
	int utfLength = UniConversion::UTF8FromUTF16(s, len * 3, w, len);
	delete []w;
	return utfLength;
}
int SubstanceUTF16::RetrieveUTF16(int position, SW_SHORT s[], int retrieveLength) {
	int pos = position;
	int len = 0;
	while (pos < (position+retrieveLength)) {
		s[len] = (SW_SHORT)(UnitValue(pos));
		pos += 2;
		len++;
	}
	return len;
}
int SubstanceUTF16::RetrieveUTF32(int position, int s[], int retrieveLength) {
	int pos = position;
	int len = 0;
	while (pos < (position+retrieveLength)) {
		s[len] = CharValue(pos);
		pos += LenChar(pos);
		len++;
	}
	return len;
}
int SubstanceUTF16::LengthInText(const int s[], int sLength) {
	return UniConversion::UTF16LengthFromUTF32(s, sLength) * 2;
}
int SubstanceUTF16::CharValue(int position) {
	int u = UnitValue(position);
	if ((u >= UniConversion::SURROGATE_LEAD_FIRST) &&
		(u <= UniConversion::SURROGATE_TRAIL_LAST)) {
		u = 0x10000 + ((u & 0x3ff) << 10) + (UnitValue(position+2) & 0x3ff);
	}
	return u;
}
int SubstanceUTF16::MovePositionOutsideChar(int position, int moveDirection, bool checkLineEnd) {
	if ((position & 1) == 1) {
		if (moveDirection > 0) {
			position++;
		} else {
			position--;
		}
	}
	if (checkLineEnd && IsCrLf(position - bytesInUnit)) {
		if (moveDirection > 0)
			return position + bytesInUnit;
		else
			return position - bytesInUnit;
	}
	return position;
}
bool SubstanceUTF16::BetweenCharacters(int position) {
	if (position < 0) {
		return false;
	} else if (position > Length()) {
		return false;
	} else if (position == Length()) {
		return true;
	} else if ((position % 2) == 1) {
		// Odd byte index
		return false;
	} else {
		int u = UnitValue(position);
		if ((u >= UniConversion::SURROGATE_TRAIL_FIRST) &&
			(u <= UniConversion::SURROGATE_TRAIL_LAST)) {
			// Trail surrogate
			return false;
		} else {
			return true;
		}
	}
}
bool SubstanceUTF16::ValidateRange(int position, int validateLength) {
	if (validateLength % 2 == 1) {
		return false;
	} else {
		// Check that Lead surrogates are followed by trail surrogates
		int i = position;
		while (i < validateLength) {
			int u = UnitValue(i);
			if ((u >= UniConversion::SURROGATE_LEAD_FIRST) &&
				(u < UniConversion::SURROGATE_TRAIL_FIRST)) {
				i += bytesInUnit;
				if (i >= validateLength) {
					// Surrogate lead at end
					return false;
				}
				u = UnitValue(i);
				if ((u < UniConversion::SURROGATE_TRAIL_FIRST) ||
					(u > UniConversion::SURROGATE_TRAIL_LAST)) {
					// No trail after lead.
					return false;
				}
				i += bytesInUnit;
			} else if ((u >= UniConversion::SURROGATE_TRAIL_FIRST) &&
				(u <= UniConversion::SURROGATE_TRAIL_LAST)) {
				// Lone trail
				return false;
			} else {
				i += bytesInUnit;
			}
		}
		return true;
	}
}
bool SubstanceUTF16::ValidateArray(SW_BYTE s[], int sLength) {
	if ((sLength % 2) == 1) {
		return false;
	} else {
		// Check that Lead surrogates are followed by trail surrogates
		int i = 0;
		while (i < sLength) {
			int u = UnitFromArray(s, i);
			if ((u >= UniConversion::SURROGATE_LEAD_FIRST) &&
				(u < UniConversion::SURROGATE_TRAIL_FIRST)) {
				i += bytesInUnit;
				if (i >= sLength) {
					// Surrogate lead at end
					return false;
				}
				u = UnitFromArray(s, i);
				if ((u < UniConversion::SURROGATE_TRAIL_FIRST) ||
					(u > UniConversion::SURROGATE_TRAIL_LAST)) {
					// No trail after lead.
					return false;
				}
				i += bytesInUnit;
			} else if ((u >= UniConversion::SURROGATE_TRAIL_FIRST) &&
				(u <= UniConversion::SURROGATE_TRAIL_LAST)) {
				// Lone trail
				return false;
			} else {
				i += bytesInUnit;
			}
		}
		return true;
	}
}

SubstanceUTF16LE::~SubstanceUTF16LE() {
}
int SubstanceUTF16LE::BytesFromCharacters(SW_BYTE *dest, int destSize, const int *s, int sLength) {
	SW_SHORT *w = new SW_SHORT[destSize/2];
	UniConversion::UTF16FromUTF32(w, destSize/2, s, sLength);
	for (int i=0; i<destSize/2; i++) {
		dest[i*2] = (SW_BYTE)(w[i] & 0xff);
		dest[i*2+1] = (SW_BYTE)(w[i] >> 8);
	}
	delete []w;
	return destSize;
}
int SubstanceUTF16LE::UnitValue(int position) {
	int u1 = UnsignedByteAt(position);
	int u2 = UnsignedByteAt(position+1);
	return u1 + (u2 << 8);
}
int SubstanceUTF16LE::UnitFromArray(SW_BYTE s[], int position) {
	int u1 = (int)(s[position]) & 0xff;
	int u2 = (int)(s[position+1]) & 0xff;
	return u1 + (u2 << 8);
}

SubstanceUTF16BE::~SubstanceUTF16BE() {
}
int SubstanceUTF16BE::BytesFromCharacters(SW_BYTE *dest, int destSize, const int *s, int sLength) {
	SW_SHORT *w = new SW_SHORT[destSize/2];
	UniConversion::UTF16FromUTF32(w, destSize/2, s, sLength);
	for (int i=0; i<destSize/2; i++) {
		dest[i*2] = (SW_BYTE)(w[i] >> 8);
		dest[i*2+1] = (SW_BYTE)(w[i] & 0xff);
	}
	delete []w;
	return destSize;
}
int SubstanceUTF16BE::UnitValue(int position) {
	int u1 = UnsignedByteAt(position);
	int u2 = UnsignedByteAt(position+1);
	return (u1 << 8) + u2;
}
int SubstanceUTF16BE::UnitFromArray(SW_BYTE s[], int position) {
	int u1 = (int)(s[position]) & 0xff;
	int u2 = (int)(s[position+1]) & 0xff;
	return (u1 << 8) + u2;
}
