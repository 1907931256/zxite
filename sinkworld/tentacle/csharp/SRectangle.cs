using System;
using System.Drawing;

public class SRectangle {
	public int left;
	public int top;
	public int right;
	public int bottom;
	internal SRectangle() {
		left = 0;
		top = 0;
		right = 0;
		bottom = 0;
	}
	internal SRectangle(SRectangle other) {
		left = other.left;
		top = other.top;
		right = other.right;
		bottom = other.bottom;
	}
	internal virtual int Width() {
		return right - left;
	}
	internal virtual int Height() {
		return bottom - top;
	}
}
