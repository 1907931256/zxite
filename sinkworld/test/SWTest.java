// Not generated from C++
class SWTest {
	public static void Assert(boolean isIt) {
		if (!isIt)
			throw new RuntimeException("Assertion failed");
	}

	public static void Write(String s) {
		System.out.print(s);
	}

	public static void Write(int i) {
		System.out.print("" + i);
	}

	public static void Write(char ch) {
		System.out.print("" + ch);
	}

	public static void Write(double d) {
		String s = "" + d;
		int posDot = s.indexOf('.');
		s = s.substring(0, posDot + 4);
		System.out.print(s);
	}

	public static void WriteLine(String s) {
		System.out.println(s);
	}

	public static double Now() {
		return System.currentTimeMillis() / 1000.0;
	}
};
