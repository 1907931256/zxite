package SinkWorld;

public class Point {
	int x;
	int y;
	Point(int x_, int y_) {
		x = x_;
		y = y_;
	}
	boolean equals(Point other) {
		if (other == null)
			return false;
		return (x == other.x) && (y == other.y);
	}
	int DistanceSquared(Point other) {
		int xdelta = other.x - x;
		int ydelta = other.y - y;
		return xdelta * xdelta + ydelta * ydelta;
	}
};
