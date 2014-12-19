// A simple counted ASCII string class.
// For C++ there is a terminating NUL to make it easier to use.
// For Java and C# there is no terminating NUL.

#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <assert.h>
#include <wchar.h>
#include <string>

#include "base.h"

#ifdef SW_CXX

Literal::Literal(const char *s, int length) {
	textValue = NULL;
	len = 0;
	int sLength = (length == -1) ? (int)strlen(s) : length;
	int lengthUTF32 = UniConversion::UTF32LengthFromUTF8(
		reinterpret_cast<const SW_BYTE *>(s), sLength);
	int *t = new int[lengthUTF32];
	UniConversion::UTF32FromUTF8(t, lengthUTF32,
		reinterpret_cast<const SW_BYTE *>(s), sLength);
	SetForLength(t, 0, lengthUTF32);
	delete []t;
}

Literal::Literal(const wchar_t *w) {
	textValue = NULL;
	len = 0;
	int wLength = (int)(wcslen(w));
	if (WCHAR_MAX > 66000L) {
		// 4 byte wchar_t
		SetForLength(const_cast<int *>(reinterpret_cast<const int *>(w)), 
			0, wLength);
	} else {
		int length = UniConversion::UTF32LengthFromUTF16(
			reinterpret_cast<const SW_SHORT *>(w), wLength);
		int *t = new int[length];
		UniConversion::UTF32FromUTF16(t, length,
			reinterpret_cast<const SW_SHORT *>(w), wLength);
		SetForLength(t, 0, length);
		delete []t;
	}
}

std::wstring Literal::AsWString() const {
	if (WCHAR_MAX > 66000L) {
		// 4 byte wchar_t
		std::wstring rv(reinterpret_cast<wchar_t *>(textValue), len);
		return rv;
	} else {
		int length = UniConversion::UTF16LengthFromUTF32(textValue, len);
		wchar_t *w = new wchar_t[length];
		UniConversion::UTF16FromUTF32((SW_SHORT *)w, length, textValue, len);
		std::wstring rv(w, length);
		delete []w;
		return rv;
	}
}
#endif

#ifdef SW_CS
void Literal::Set(System.String s) {
	textValue = null;
	len = 0;
	if (s != null) {
		byte[] utf8 = System.Text.Encoding.UTF8.GetBytes(s);
		SetFromUTF8(utf8, utf8.Length);
	}
}
System.String Literal::ToString() {
	return System.Text.Encoding.UTF8.GetString(AsUTF8());
}
#endif
#ifdef SW_JAVA
void Literal::SetJ(String s) {
	textValue = null;
	len = 0;
	if (s != NULL) {
		try {
			byte utf8[] = s.getBytes("UTF8");
			SetFromUTF8(utf8, utf8.length);
		} catch (java.io.UnsupportedEncodingException x) {
		}
	}
}
#endif

Literal::Literal(int i) {
	textValue = NULL;
	len = 0;
#ifdef SW_CXX
	char s[20];
	sprintf(s, "%0d", i);
	SetFromUTF8(reinterpret_cast<const SW_BYTE *>(s), (int)strlen(s));
#endif
#ifdef SW_CS
	Set("" + i);
#endif
#ifdef SW_JAVA
	SetJ("" + i);
#endif
}

void Literal::Allocate(int lenAllocate) {
	delete []textValue;
	len = lenAllocate;
	textValue = new int[len];
}

void Literal::SetFromUTF8(const SW_BYTE *s, int sLength) {
	int iLength = UniConversion::UTF32LengthFromUTF8(s, sLength);
	Allocate(iLength);
	UniConversion::UTF32FromUTF8(textValue, len, s, sLength);
}

int Literal::Value() const {
	int v = 0;
	int sign = 1;
	for (int i=0; i<len; i++) {
		if (textValue[i] == '-') {
			sign = -1;
		} else if ((textValue[i] >= '0') && (textValue[i] <= '9')) {
			v = v * 10 + (textValue[i] - '0');
		}
	}
	return (sign == 1) ? v : -v;
}

SW_BYTE *Literal::AsUTF8() const {
	int lenUTF8 = UniConversion::UTF8LengthFromUTF32(textValue, len);
#ifdef SW_CXX
	SW_BYTE *ret = new SW_BYTE[lenUTF8+1];
	ret[lenUTF8] = 0;
#endif
#ifdef SW_CS
	SW_BYTE *ret = new SW_BYTE[lenUTF8];
#endif
#ifdef SW_JAVA
	SW_BYTE *ret = new SW_BYTE[lenUTF8];
#endif
	UniConversion::UTF8FromUTF32(ret, lenUTF8, textValue, len);
	return ret;
}

int Literal::LengthUTF8() const {
	return UniConversion::UTF8LengthFromUTF32(textValue, len);
}

