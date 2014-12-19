// SinkWorld Substance classes.
// Copyright 2005 Neil Hodgson
// File written in C~ subset of C++ for automatic translation to Java and C#.

/// The text of a buffer is stored in an array of bytes in a class that extends Substance.
/// The basic unit of access may be 1, 2, or 4 bytes. A character may require 1 or more units.
/// For example, UTF-8 can require 1, 2, 3, or 4 bytes per character.

SW_PUBLIC SW_ABSTRACT class Substance : public SplitVector_1 {
public:
	int bytesInUnit;
	Substance() {}
	virtual ~Substance() {
	}
	int UnsignedByteAt(int position) {
		return (int)(CharAt(position)) & 0xff;
	}
	// Is position located between the two units of a CR+LF line end?
	bool IsCrLf(int position) {
		if (position < 0)
			return false;
		if (position >= (Length() - bytesInUnit))
			return false;
		return (CharValue(position) == '\r') && (CharValue(position + bytesInUnit) == '\n');
	}
	virtual int DistanceNextCaret(int position) = 0;
	virtual int LenChar(int position) = 0;
	virtual int LenCharacters(const int *s, int sLength) = 0;
	virtual int BytesFromCharacters(SW_BYTE *dest, int destSize, const int *s, int sLength) = 0;
	virtual int RetrieveUTF8(int position, SW_BYTE s[], int retrieveLength) = 0;
	virtual int RetrieveUTF16(int position, SW_SHORT s[], int retrieveLength) = 0;
	virtual int RetrieveUTF32(int position, int s[], int retrieveLength) = 0;
	virtual int LengthInText(const int s[], int sLength) = 0;
	// UnitValue returns the value of the minimum addressable unit at position.
	virtual int UnitValue(int position) = 0;
	virtual int UnitFromArray(SW_BYTE s[], int position) = 0;
	virtual int CharValue(int position) = 0;
	virtual int MovePositionOutsideChar(int position, int moveDirection, bool checkLineEnd) = 0;
	virtual bool BetweenCharacters(int position) = 0;
	virtual bool ValidateRange(int position, int validateLength) = 0;
	virtual bool ValidateArray(SW_BYTE s[], int sLength) = 0;
};

/// Interprets the buffer as encoded in UTF-8.

class SubstanceUTF8 : public Substance {
public:
	SubstanceUTF8() {
		bytesInUnit = 1;
	}
	virtual ~SubstanceUTF8();
	SW_OVERRIDE int DistanceNextCaret(int position);
	SW_OVERRIDE int LenChar(int position);
	SW_OVERRIDE int LenCharacters(const int *s, int sLength);
	SW_OVERRIDE int BytesFromCharacters(SW_BYTE *dest, int destSize, const int *s, int sLength);
	SW_OVERRIDE int RetrieveUTF8(int position, SW_BYTE s[], int retrieveLength);
	SW_OVERRIDE int RetrieveUTF16(int position, SW_SHORT s[], int retrieveLength);
	SW_OVERRIDE int RetrieveUTF32(int position, int s[], int retrieveLength);
	SW_OVERRIDE int LengthInText(const int s[], int sLength);
	SW_OVERRIDE int UnitValue(int position);
	SW_OVERRIDE int UnitFromArray(SW_BYTE s[], int position);
	SW_OVERRIDE int CharValue(int position);
	SW_OVERRIDE int MovePositionOutsideChar(int position, int moveDirection, bool checkLineEnd);
	SW_OVERRIDE bool BetweenCharacters(int position);
	SW_OVERRIDE bool ValidateRange(int position, int validateLength);
	SW_OVERRIDE bool ValidateArray(SW_BYTE s[], int sLength);
};

/// Interprets the buffer as encoded in 8 bit fixed length encoding such as ISO 8859-1.

class SubstanceByte : public Substance {
public:
	SubstanceByte() {
		bytesInUnit = 1;
	}
	virtual ~SubstanceByte();
	SW_OVERRIDE int DistanceNextCaret(int position);
	SW_OVERRIDE int LenChar(int position);
	SW_OVERRIDE int LenCharacters(const int *s, int sLength);
	SW_OVERRIDE int BytesFromCharacters(SW_BYTE *dest, int destSize, const int *s, int sLength);
	SW_OVERRIDE int RetrieveUTF8(int position, SW_BYTE s[], int retrieveLength);
	SW_OVERRIDE int RetrieveUTF16(int position, SW_SHORT s[], int retrieveLength);
	SW_OVERRIDE int RetrieveUTF32(int position, int s[], int retrieveLength);
	SW_OVERRIDE int LengthInText(const int s[], int sLength);
	SW_OVERRIDE int UnitValue(int position);
	SW_OVERRIDE int UnitFromArray(SW_BYTE s[], int position);
	SW_OVERRIDE int CharValue(int position);
	SW_OVERRIDE int MovePositionOutsideChar(int position, int moveDirection, bool checkLineEnd);
	SW_OVERRIDE bool BetweenCharacters(int position);
	SW_OVERRIDE bool ValidateRange(int position, int validateLength);
	SW_OVERRIDE bool ValidateArray(SW_BYTE s[], int sLength);
};

/// Abstract superclass for UTF-16 shares common code.

SW_ABSTRACT class SubstanceUTF16 : public Substance {
public:
	SubstanceUTF16() {
		bytesInUnit = 2;
	}
	SW_OVERRIDE int DistanceNextCaret(int position);
	SW_OVERRIDE int LenChar(int position);
	SW_OVERRIDE int LenCharacters(const int *s, int sLength);
	SW_OVERRIDE int RetrieveUTF8(int position, SW_BYTE s[], int retrieveLength);
	SW_OVERRIDE int RetrieveUTF16(int position, SW_SHORT s[], int retrieveLength);
	SW_OVERRIDE int RetrieveUTF32(int position, int s[], int retrieveLength);
	SW_OVERRIDE int LengthInText(const int s[], int sLength);
	SW_OVERRIDE int CharValue(int position);
	SW_OVERRIDE int MovePositionOutsideChar(int position, int moveDirection, bool checkLineEnd);
	SW_OVERRIDE bool BetweenCharacters(int position);
	SW_OVERRIDE bool ValidateRange(int position, int validateLength);
	SW_OVERRIDE bool ValidateArray(SW_BYTE s[], int sLength);
};

/// Interprets the buffer as encoded in UTF16-LE.

class SubstanceUTF16LE : public SubstanceUTF16 {
public:
	SubstanceUTF16LE() {
	}
	virtual ~SubstanceUTF16LE();
	SW_OVERRIDE int BytesFromCharacters(SW_BYTE *dest, int destSize, const int *s, int sLength);
	SW_OVERRIDE int UnitValue(int position);
	SW_OVERRIDE int UnitFromArray(SW_BYTE s[], int position);
};

/// Interprets the buffer as encoded in UTF16-LE.

class SubstanceUTF16BE : public SubstanceUTF16 {
public:
	SubstanceUTF16BE() {
	}
	virtual ~SubstanceUTF16BE();
	SW_OVERRIDE int BytesFromCharacters(SW_BYTE *dest, int destSize, const int *s, int sLength);
	SW_OVERRIDE int UnitValue(int position);
	SW_OVERRIDE int UnitFromArray(SW_BYTE s[], int position);
};
