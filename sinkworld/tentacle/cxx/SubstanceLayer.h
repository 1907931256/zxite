// SinkWorld SubstanceLayer class.
// Copyright 2004 Neil Hodgson
// File written in C~ subset of C++ for automatic translation to Java and C#.

/// Subclass of ChangeLayer that can find positions of significant changes
/// in a text buffer.

SW_PUBLIC class SubstanceLayer : public ChangeLayer {
public:
	Substance *psubstance;
	SubstanceLayer(int position_, int layer_, Substance *psubstance_, int end) {
		position = position_;
		layer = layer_;
		psubstance = psubstance_;
		FindNextChange(end);
	}
	static bool IsControlCharacter(int ch) {
#ifdef SW_CXX
		return ch < ' ';
#endif
#ifdef SW_JAVA
		return (ch >= 0) && (ch < ' ');
#endif
#ifdef SW_CS
		return (ch >= 0) && (ch < ' ');
#endif
	}
	SW_OVERRIDE void FindNextChange(int end);
};
