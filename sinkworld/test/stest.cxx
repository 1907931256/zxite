#include <string.h>
#include <stdio.h>
#include <assert.h>

#include "SWTest.h"

#include "SWBase.h"
#include "SWLexers.h"

void Ins(OptionSet *os, const char *k, const char *v) {
	Literal lKey(k);
	Literal lValue(v);
	os->Insert(&lKey, &lValue);
}

SW_BYTE *Find(OptionSet *os, const char *k) {
	Literal lKey(k);
	Literal *l = os->Value(&lKey);
	return l ? l->Data() : NULL;
}

void Check(OptionSet *os, const char *k) {
	SW_BYTE *answer = Find(os, k);
	assert(answer);
	printf("Key=%s Value=%s\n", k, answer);
}

static bool GetFullLine(const SW_BYTE *propsData, int &pos, int &lenData, SW_BYTE *lineBuffer, int lenBuffer) {
	//~ return complete;
}

void ReadOptionsFile(OptionSet *os, Literal *prefix, char *filename) {
	FILE *rcfile = fopen(filename, "rb");
	if (rcfile) {
		enum { sizeFile = 60000 };
		SW_BYTE propsData[sizeFile];
		int lenData = static_cast<int>(fread(propsData, 1, sizeof(propsData), rcfile));
		fclose(rcfile);
		os->SetFromText(prefix, propsData, 0, lenData);
	}
}

void PrintStats(OptionSet *os) {
	int used = os->Used();
	int allocated = os->Allocated();
	int slots = os->SlotsUsed();
	int multiple = used - slots;
	printf("Allocated=%4d   Slots=%4d   Used=%4d   Multiple=%4d  %g\n",
		allocated, slots, used, multiple, (double)multiple / used);
};

int main() {
	OptionSet *os = new OptionSet();
	
	//~ ReadOptionsFile(os, NULL, "opts.txt");
	Literal prefix("mode.base");
	ReadOptionsFile(os, &prefix, "base.mode");
	PrintStats(os);
	Literal exts(".extensions");
	WordList *keys=os->Keys(&exts);
	printf("Keys = %2d\n", keys->Length());
	for (int k=0;k<keys->Length();k++) {
		Literal *lKey = keys->GetText(k);
		Literal *lVal = os->Value(lKey);
		printf("[%2d] %s=%s\n", k, (char *)(lKey->Data()), (char *)(lVal->Data()));
	}
	delete keys;
	printf("\n");

	Literal all("");
	keys=os->Keys(&all);
	printf("All = %2d\n", keys->Length());
	for (int k=0;k<keys->Length();k++) {
		Literal *lKey = keys->GetText(k);
		Literal *lVal = os->Value(lKey);
		printf("[%2d] %s=%s\n", k, (char *)(lKey->Data()), (char *)(lVal->Data()));
	}
	delete keys;
	printf("\n");
	Literal lModeCxx("mode.base.js_client");
	Literal lKeyLexer("lexer");
	Literal *lLexer = os->ModeValue(&lModeCxx, &lKeyLexer);
	printf("%s==>%s\n", (char *)(lKeyLexer.Data()), (char *)(lLexer->Data()));
	delete lLexer;
	printf("\n");
	printf("\n");
	printf("\n");

	PrintStats(os);
	assert(os);
	Literal lName("cxx-cpp");
	Literal lMode("cxx");
	os->Insert(&lName, &lMode);
	Check(os, "cxx-cpp");
	PrintStats(os);

	Ins(os, "style", "font:andale");
	Check(os, "cxx-cpp");
	Check(os, "style");
	PrintStats(os);
	
	Literal lKey("style");
	Literal *l;
	l = os->ModeValue(&lMode, &lKey);
	printf("Mode=%s Key=%s Value=%s\n", 
		lMode.Data(), lKey.Data(), l ? (char *)(l->Data()) : "<null>");
	Ins(os, "cxx.style", "font:verida sans");
	l = os->ModeValue(&lMode, &lKey);
	printf("Mode=%s Key=%s Value=%s\n", 
		lMode.Data(), lKey.Data(), l ? (char *)(l->Data()) : "<null>");
	Ins(os, "cxx.super", "c");
	l = os->ModeValue(&lMode, &lKey);
	printf("Mode=%s Key=%s Value=%s\n", 
		lMode.Data(), lKey.Data(), l ? (char *)(l->Data()) : "<null>");
	Literal lKeyExtensions("extensions");
	l = os->ModeValue(&lMode, &lKeyExtensions);
	printf("Mode=%s Key=%s Value=%s\n", 
		lMode.Data(), lKeyExtensions.Data(), l ? (char *)(l->Data()) : "<null>");
	Ins(os, "c.extensions", "c cc cxx");
	l = os->ModeValue(&lMode, &lKeyExtensions);
	printf("Mode=%s Key=%s Value=%s\n", 
		lMode.Data(), lKeyExtensions.Data(), l ? (char *)(l->Data()) : "<null>");

	//~ ReadOptionsFile(os, "lopts.txt");
	//~ PrintStats(os);

	delete os;
}
