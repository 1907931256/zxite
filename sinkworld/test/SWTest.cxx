#include <stdio.h>
#include <time.h>
#include <assert.h>
#include <Windows.h>

#include "SWTest.h"

void SWTest_::Assert(bool isIt) {
	assert(isIt);
}

void SWTest_::Write(const char *s) {
	printf("%s", s);
}

void SWTest_::Write(int i) {
	printf("%0d", i);
}

void SWTest_::Write(char ch) {
	printf("%c", ch);
}

void SWTest_::Write(double d) {
	printf("%0.3f", d);
}

void SWTest_::WriteLine(const char *s) {
	printf("%s\n", s);
}

double SWTest_::Now() {
	return GetTickCount() / 1000.0;
}

SWTest_ SWTest;
