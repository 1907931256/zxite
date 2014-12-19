package SinkWorld;

public class SRectangle {
	public int left;
	public int top;
	public int right;
	public int bottom;
	SRectangle() {
		left = 0;
		top = 0;
		right = 0;
		bottom = 0;
	}
	SRectangle(SRectangle other) {
		left = other.left;
		top = other.top;
		right = other.right;
		bottom = other.bottom;
	}
	int Width() {
		return right - left;
	}
	int Height() {
		return bottom - top;
	}
};
