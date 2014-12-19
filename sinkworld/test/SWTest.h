class SWTest_ {
public:
	static void Assert(bool isIt);
	static void Write(const char *s);
	static void Write(int i);
	static void Write(char ch);
	static void Write(double d);
	static void WriteLine(const char *s);
	static double Now();
};

extern SWTest_ SWTest;
