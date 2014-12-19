// SinkWorld Literal class.
// Copyright 2004 Neil Hodgson
// File written in C~ subset of C++ for automatic translation to Java and C#.

/// A simple counted ASCII string class.
/// There is no terminating 0 character.
SW_PUBLIC class Literal {
private:
	int *textValue;
	int len;
	void SetFromUTF8(const SW_BYTE *s, int sLength);
	void Allocate(int lenAllocate);
	void SetForLength(const int *s, int start, int length) {
		len = 0;
		delete []textValue;
		textValue = NULL;
		if (s != NULL) {
			Allocate(length);
			for (int i=0; i<len; i++) {
				textValue[i] = s[start+i];
			}
		}
	}
#ifdef SW_CS
	void Set(System.String s);
#endif
#ifdef SW_JAVA
	void SetJ(String s);
#endif
public:
#ifdef SW_CS
	SW_OVERRIDE System.String ToString();
#endif
	Literal() {
		textValue = NULL;
		len = 0;
	}
	Literal(const Literal *other, int start, int length) {
		textValue = NULL;
		len = 0;
		SetForLength(other->textValue, start, length);
	}
	Literal(const Literal *other) {
		textValue = NULL;
		len = 0;
		if (other != NULL) {
			SetForLength(other->textValue, 0, other->len);
		}
	}
	Literal(const Literal *prefix, int separator, const Literal *suffix) {
		textValue = NULL;
		len = 0;
		Allocate(prefix->len + 1 + suffix->len);
		int i = 0;
		for (; i < prefix->len; i++) {
			textValue[i] = prefix->textValue[i];
		}
		textValue[i++] = separator;
		for (int j = 0; j < suffix->len; j++,i++) {
			textValue[i] = suffix->textValue[j];
		}
	}
	Literal(const int *s, int start, int length) {
		textValue = NULL;
		len = 0;
		SetForLength(s, start, length);
	}
	// In implementation file to avoid requiring stdio.h
	Literal(int i);

#ifdef SW_CXX
	Literal(const char *s, int length=-1);
	Literal(const wchar_t *w);
	Literal(const Literal &other) {
		textValue = NULL;
		len = 0;
		SetForLength(other.textValue, 0, other.len);
	}
	Literal &operator =(const Literal &other) {
		if (&other != this) {
			SetForLength(other.textValue, 0, other.len);
		}
		return *this;
	}
	std::wstring AsWString() const;
	~Literal() {
		delete []textValue;
	}
#endif
#ifdef SW_CS
	Literal(System.String s) {
		Set(s);
	}
#endif
#ifdef SW_JAVA
	Literal(String s) {
		SetJ(s);
	}
#endif

	int Value() const;

	int CharAt(int position) const {
		return textValue[position];
	}

	const int *Data() const {
		return textValue;
	}

	int Length() const {
		return len;
	}

	/// Return a UTF-8 encoding.
	/// Is NUL terminated for C++ but not NUL terminated for C# and Java.
	/// Caller responsible for deleting with delete[]
	SW_BYTE *AsUTF8() const;

	int LengthUTF8() const;

	int Count(int ch) const {
		int c = 0;
		for (int i=0; i < len; i++) {
			if (textValue[i] == ch) {
				c++;
			}
		}
		return c;
	}

	bool Equals(const Literal *other) const {
		if (len != other->len)
			return false;
		for (int i=0; i< len; i++) {
			if (textValue[i] != other->textValue[i])
				return false;
		}
		return true;
	}

	bool Match(const Literal *other, int pos) const {
		bool found = false;
		if (pos+other->len <= len) {
			found = true;
			for (int i=0; i < other->len; i++) {
				if (textValue[pos+i] != other->textValue[i]) {
					found = false;
					break;
				}
			}
		}
		return found;
	}

	bool EndsWith(const Literal *other) const {
		if (other->len > len)
			return false;
		return Match(other, len-other->len);
	}

	int Find(const Literal *other, int posStart) const {
		for (int i=posStart; i <= (len - other->len); i++) {
			bool found = Match(other, i);
			if (found) {
				return i;
			}
		}
		return -1;
	}

	bool IsIn(const int *other, int posStart, int lenOther) const {
		if (len > 0) {
			for (int i=posStart; i <= (lenOther-len); i++) {
				bool found = true;
				for (int j=0; j < len; j++) {
					if (textValue[j] != other[j+i]) {
						found = false;
						break;
					}
				}
				if (found) {
					return true;
				}
			}
		}
		return false;
	}

	Literal *Segment(int n, int separator) const {
		int start = 0;
		while ((start < Length()) && (n > 0)) {
			if (CharAt(start) == separator) {
				n--;
			}
			start++;
		}
		int end = start;
		while ((end < Length()) && (n == 0)) {
			if (CharAt(end) == separator) {
				n--;
			} else {
				end++;
			}
		}
		return new Literal(this,start,end-start);
	}

	static int HashString(const int *s, int len) {
		// Use the Python string hash algorithm
		int ret = s[0] << 7;
		for (int i=0; i<len; i++) {
			ret *= 1000003;
			ret ^= s[i];
		}
		ret ^= len;
		return ret;
	}

	int HashValue() const {
		return HashString(Data(), Length());
	}
};
