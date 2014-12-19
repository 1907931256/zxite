// Not generated from C++
using System;
class SWTest {
	public static void Assert(bool isIt) {
		System.Diagnostics.Debug.Assert(isIt);
	}

	public static void Write(System.String s) {
		Console.Write(s);
	}

	public static void Write(int i) {
		Console.Write(i.ToString());
	}

	public static void Write(char ch) {
		Console.Write(Char.ToString(ch));
	}

	public static void Write(double d) {
		Console.Write(d.ToString("f3"));
	}

	public static void WriteLine(System.String s) {
		Console.WriteLine(s);
	}

	public static double Now() {
		return DateTime.Now.Ticks / 10000000.0;
	}
};
