// SinkWorld ByteBuffer class.
// Copyright 2004 Neil Hodgson
// File written in C~ subset of C++ for automatic translation to Java and C#.

/// Buffer optimised for dynamically changing size.
/// Displayed string segments are mostly quite short but
/// can be very long. Using dynamic allocation for all
/// segments results in many short term allocations.
/// This class can meet most needs from a statically
/// allocated array but will expand when needed.

#ifdef _MSC_VER
#pragma warning(disable: 4996)
#endif

SW_PUBLIC class ByteBuffer {
	int size;
	SW_BYTE *s;
#ifdef SW_CXX
	enum { SmallSize = 32 };
	unsigned char smallS[SmallSize];
#endif
	void Allocate(int size_) {
		size = size_;
		s = NULL;
#ifdef SW_CXX
		if (size <= SmallSize) {
			size = SmallSize;
			s = smallS;
			return;
		}
#endif
		if (size > 0) {
			s = new SW_BYTE[size];
		}
	}
public:
	ByteBuffer(int size_) {
		Allocate(size_);
	}
	ByteBuffer() {
		Allocate(0);
	}
	~ByteBuffer() {
#ifdef SW_CXX
		if (size > SmallSize) {
			delete []s;
		}
#endif
		s = 0;
		size = 0;
	}
	void Resize(int size_) {
		if (size_ > size) {
#ifdef SW_CXX
			if (size > SmallSize) {
				delete []s;
			}
#endif
			size = size_;
			s = new SW_BYTE[size];
		}
	}
	SW_BYTE *Bytes() {
		return s;
	}
#ifdef SW_CXX
	operator unsigned char *() {
		return s;
	}
	char *AsCharPointer() {
		return reinterpret_cast<char *>(s);
	}
	wchar_t *AsWCharTPointer() {
		return reinterpret_cast<wchar_t *>(s);
	}
	int Length() {
		return static_cast<int>(strlen(reinterpret_cast<char *>(s)));
	}
	void Set(const char *s_) {
		int lengthS_ = static_cast<int>(strlen(s_) + 1);
		Resize(lengthS_);
		strcpy(reinterpret_cast<char *>(s), s_);
	}
#endif
};

