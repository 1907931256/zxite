// SinkWorld StyleLayer class.
// Copyright 2004 Neil Hodgson
// File written in C~ subset of C++ for automatic translation to Java and C#.

/// Subclass of ChangeLayer that can find positions of significant changes
/// in a style buffer.

SW_PUBLIC class StyleLayer : public ChangeLayer {
public:
	IStyles *pstyles;

	StyleLayer(int position_, int layer_, IStyles *pstyles_, int end) {
		position = position_;
		layer = layer_;
		pstyles = pstyles_;
		FindNextChange(end);
	}
	SW_OVERRIDE void FindNextChange(int end) {
		position = pstyles->FindNextChange(position, end);
	}
};
