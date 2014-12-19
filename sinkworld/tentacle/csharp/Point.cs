using System;
using System.Drawing;

public class Point {
	internal int x;
	internal int y;
	internal Point(int x_, int y_) {
		x = x_;
		y = y_;
	}
	internal virtual bool equals(Point other) {
		if (other == null)
			return false;
		return (x == other.x) && (y == other.y);
	}
	internal virtual int DistanceSquared(Point other) {
		int xdelta = other.x - x;
		int ydelta = other.y - y;
		return xdelta * xdelta + ydelta * ydelta;
	}
}
