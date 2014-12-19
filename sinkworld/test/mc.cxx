// SinkWorld demonstration program - C++ version
// Copyright 2001-2004 Neil Hodgson
#include <string.h>
#include <stdio.h>
//#undef NDEBUG
#include <assert.h>
#include <string>
#include <Windows.h>

#include "SWTest.h"

#include "base.h"
#include "lexers.h"

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

int testlv() {
	//assert(0);
	Partitioning *v = new Partitioning(4);
	try {
	v->RemovePartition(0);
	v->InsertPartition(1, 11);
	} catch (BadPositionException *e) {
		delete e;
	}
	v->InsertPartition(0,100);
	int x = v->PositionFromPartition(0);
	delete v;

	v = new Partitioning(4);
	assert(v->Partitions() == 1);
	assert(v->PositionFromPartition(0) == 0);
	assert(v->PositionFromPartition(1) == 0);
	//Literal shortInsert("abcd");
	v->InsertText(0, 4);
	assert(v->Partitions() == 1);
	assert(v->PositionFromPartition(0) == 0);
	assert(v->PositionFromPartition(1) == 4);
	v->InsertPartition(1,2);
	assert(v->Partitions() == 2);
	assert(v->PositionFromPartition(0) == 0);
	assert(v->PositionFromPartition(1) == 2);
	assert(v->PositionFromPartition(2) == 4);
	v->InsertPartition(1,1);
	assert(v->Partitions() == 3);
	assert(v->PositionFromPartition(0) == 0);
	assert(v->PositionFromPartition(1) == 1);
	assert(v->PositionFromPartition(2) == 2);
	assert(v->PositionFromPartition(3) == 4);
	v->InsertText(0, 1);
	assert(v->Partitions() == 3);
	assert(v->PositionFromPartition(0) == 0);
	assert(v->PositionFromPartition(1) == 2);
	assert(v->PositionFromPartition(2) == 3);
	assert(v->PositionFromPartition(3) == 5);
	v->RemovePartition(2);
	assert(v->Partitions() == 2);
	assert(v->PositionFromPartition(0) == 0);
	assert(v->PositionFromPartition(1) == 2);
	assert(v->PositionFromPartition(2) == 5);
	v->InsertPartition(1, 2);
	assert(v->Partitions() == 3);
	assert(v->PositionFromPartition(0) == 0);
	assert(v->PositionFromPartition(1) == 2);
	assert(v->PositionFromPartition(2) == 2);
	assert(v->PositionFromPartition(3) == 5);
	delete v;

	return 0;
}

int tu() {
	Partitioning *v = new Partitioning(8);
	try {
	v->Delete(0);
	v->InsertPartition(1, 11);
	} catch (BadPositionException *e) {
		delete e;
	}
	v->InsertPartition(0,100);
	int x = v->ValueAt(0);
	delete v;

	Literal shortInsert("bc");
	Literal LF("\n");
	Literal CR("\r");

	SplitText *pbuffer = new SplitText();
	pbuffer->AttachStyles(SplitText::CreateStyles(1), NULL, 0, false);
	pbuffer->InsertUTF32(0, shortInsert.Data(), 2);
	// bc
	pbuffer->InsertUTF32(0, CR.Data(), 1);
	// \rbc
	pbuffer->InsertUTF32(0, LF.Data(), 1);
	assert(pbuffer->PositionFromLine(0) == 0);
	// \n\rbc
	// This \r is merged into the previous line end
	pbuffer->InsertUTF32(0, CR.Data(), 1);
	delete pbuffer;
	return 0;
}

void assertValue(SplitText *p, const char *s) {
	size_t len = p->Length();
	SWTest.Assert(len == strlen(s));
	SW_BYTE *v = new SW_BYTE[len];
	p->RetrieveBytes(0, v, len);
	SWTest.Assert(0 == memcmp(v, s, len));
	delete []v;
}

void showUB(UndoStack *us) {
/*
	SWTest.Write("Undo steps=");
	SWTest.Write(us->maxAction);
	SWTest.Write("   InBatch=");
	SWTest.Write(us->StartUndo());
	SWTest.Write("  R=");
	SWTest.Write(us->dataRemoved ? us->dataRemoved->Length() : 0);
	SWTest.Write("  I=");
	SWTest.Write(us->dataInserted ? us->dataInserted->Length() : 0);
	SWTest.Write("\n");
	for (int i=0; i<us->maxAction; i++) {
		if (i == us->currentAction)
			SWTest.Write("   !");
		else
			SWTest.Write("    ");
		SWTest.Write(us->actionType->GetValue(i));
		SWTest.Write("    ");
		SWTest.Write(us->position->GetValue(i));
		SWTest.Write("    ");
		SWTest.Write(us->lenData->GetValue(i));
		SWTest.Write("\n");
	}
*/
}

void testUndoRedo() {
	SplitText *ub = new SplitText();
	ub->AttachStyles(SplitText::CreateStyles(1), NULL, 0, false);
	//showUB(ub->us);
	Literal utest("u");
	int lenUtest = utest.Length();
	Literal uutest("wmin");
	int lenUUtest = uutest.Length();
	ub->InsertBytes(0, (SW_BYTE *)"xy", 2);
	assertValue(ub, "xy");
	//showUB(ub->us);
	ub->InsertUTF32(0, utest.Data(), lenUtest);
	assertValue(ub, "uxy");
	//showUB(ub->us);
	ub->DeleteRange(1, 1);
	assertValue(ub, "uy");
	//showUB(ub->us);
	ub->PerformUndoAction();
	assertValue(ub, "uxy");
	//showUB(ub->us);
	ub->PerformRedoAction();
	assertValue(ub, "uy");
	//showUB(ub->us);

	// Undo to beginning and redo again
	ub->PerformUndoAction();
	assertValue(ub, "uxy");
	//showUB(ub->us);
	ub->PerformUndoAction();
	assertValue(ub, "xy");
	//showUB(ub->us);
	ub->PerformUndoAction();
	assertValue(ub, "");
	//showUB(ub->us);
	ub->PerformRedoAction();
	assertValue(ub, "xy");
	//showUB(ub->us);
	ub->PerformRedoAction();
	assertValue(ub, "uxy");
	//showUB(ub->us);
	ub->PerformRedoAction();
	assertValue(ub, "uy");
	//showUB(ub->us);

	// Do it again. Undo to beginning and redo again
	ub->PerformUndoAction();
	assertValue(ub, "uxy");
	//showUB(ub->us);
	ub->PerformUndoAction();
	assertValue(ub, "xy");
	//showUB(ub->us);
	ub->PerformUndoAction();
	assertValue(ub, "");
	//showUB(ub->us);
	ub->PerformRedoAction();
	assertValue(ub, "xy");
	//showUB(ub->us);
	ub->PerformRedoAction();
	assertValue(ub, "uxy");
	//showUB(ub->us);
	ub->PerformRedoAction();
	assertValue(ub, "uy");
	//showUB(ub->us);

	// Add a new step in
	ub->PerformUndoAction();
	assertValue(ub, "uxy");
	//showUB(ub->us);
	ub->DeleteRange(0, 1);
	assertValue(ub, "xy");
	//showUB(ub->us);
	ub->PerformUndoAction();
	assertValue(ub, "uxy");
	//showUB(ub->us);
	ub->PerformUndoAction();
	assertValue(ub, "xy");
	//showUB(ub->us);
	ub->PerformUndoAction();
	assertValue(ub, "");
	//showUB(ub->us);

	// Checking for batching
	// Typing one character then another after should be one step
	ub->InsertUTF32(0, utest.Data(), lenUtest);
	SWTest.Assert(ub->StartUndo() == 1);
	assertValue(ub, "u");
	//showUB(ub->us);
	ub->InsertUTF32(1, utest.Data(), lenUtest);
	SWTest.Assert(ub->StartUndo() == 2);
	assertValue(ub, "uu");
	//showUB(ub->us);
	ub->PerformUndoStep();
	assertValue(ub, "");
	ub->PerformRedoStep();
	assertValue(ub, "uu");
	// This one should not merge as it is after a redo
	ub->InsertUTF32(2, utest.Data(), lenUtest);
	SWTest.Assert(ub->StartUndo() == 1);
	assertValue(ub, "uuu");
	//showUB(ub->us);
	// This one should not merge as it is a delete and previous was insert
	ub->DeleteRange(0, 1);
	SWTest.Assert(ub->StartUndo() == 1);
	assertValue(ub, "uu");
	//showUB(ub->us);
	// This one should merge as it is a delete and next to previous
	ub->DeleteRange(0, 1);
	SWTest.Assert(ub->StartUndo() == 2);
	assertValue(ub, "u");
	//showUB(ub->us);
	ub->PerformUndoStep();
	assertValue(ub, "uuu");
	//showUB(ub->us);
	// This one should not merge as it is after an undo
	ub->DeleteRange(2, 1);
	SWTest.Assert(ub->StartUndo() == 1);
	assertValue(ub, "uu");
	//showUB(ub->us);
	// This one should merge as it is a delete and next to previous
	ub->DeleteRange(1, 1);
	SWTest.Assert(ub->StartUndo() == 2);
	assertValue(ub, "u");
	//showUB(ub->us);

	delete ub;
}

void ShowStyled(SplitText *buffer) {
	int prevStyle = 0;
	for (int o=0; o<buffer->Length(); o++) {
		int styleNow = buffer->StyleAt(o);
		if (styleNow != prevStyle) {
			SWTest.Write("[");
			SWTest.Write(styleNow);
			SWTest.Write("]");
			prevStyle = styleNow;
		}
		SWTest.Write((char)buffer->XCharAt(o));
	}
}

void TestLanguage(ILexer &lexer, Literal &source, Literal &keyWords) {
	SplitText *buffer = new SplitText();
	IStyles *style = SplitText::CreateStyles(1);
	buffer->AttachStyles(style, &lexer, 0, false);
	buffer->InsertUTF32(0, source.Data(), source.Length());
	lexer.SetKeyWords(0, &keyWords);
	buffer->LexTo(buffer->Length(), 0, false);
	//ShowStyled(buffer);
	delete buffer;
}

void TestHistory() {
	SWTest.WriteLine("Test History");
	// First a simple list of integers
	History historySimple;
	SWTest.Assert(historySimple.Value() == 0);
	historySimple.Append(10, 1);
	SWTest.Assert(historySimple.Value() == 1);
	historySimple.InvalidateAfter(10);
	SWTest.Assert(historySimple.Value() == 1);
	historySimple.InvalidateAfter(9);
	SWTest.Assert(historySimple.Value() == 0);
	historySimple.Append(10, 1);
	SWTest.Assert(historySimple.Value() == 1);
	historySimple.Append(20, 2);
	SWTest.Assert(historySimple.Value() == 2);
	historySimple.Append(30, 3);
	SWTest.Assert(historySimple.Value() == 3);
	historySimple.InvalidateAfter(20);
	SWTest.Assert(historySimple.Value() == 2);
	historySimple.InvalidateAfter(10);
	SWTest.Assert(historySimple.Value() == 1);
	historySimple.InvalidateAfter(9);
	SWTest.Assert(historySimple.Value() == 0);

	// A stack of strings
	History historyStack;
	Literal *te = historyStack.TextValue();
	Literal first("first");
	Literal second("second");
	Literal third("third");
	SWTest.Assert(te == NULL);
	historyStack.Push(10, new Literal("first"));
	te = historyStack.TextValue();
	SWTest.Assert(te->Equals(&first));
	historyStack.Push(20, new Literal("second"));
	te = historyStack.TextValue();
	SWTest.Assert(te->Equals(&second));
	historyStack.Push(30, new Literal("third"));
	te = historyStack.TextValue();
	SWTest.Assert(te->Equals(&third));
	historyStack.Pop(40);
	te = historyStack.TextValue();
	SWTest.Assert(te->Equals(&second));
	historyStack.Pop(50);
	te = historyStack.TextValue();
	SWTest.Assert(te->Equals(&first));
	historyStack.Push(60, new Literal("twoA"));
	Literal twoA("twoA");
	SWTest.Assert(historyStack.TextValue()->Equals(&twoA));
	historyStack.Pop(70);
	te = historyStack.TextValue();
	SWTest.Assert(te->Equals(&first));

	historyStack.InvalidateAfter(75);
	SWTest.Assert(historyStack.TextValue()->Equals(&first));
	historyStack.InvalidateAfter(65);
	SWTest.Assert(historyStack.TextValue()->Equals(&twoA));
	historyStack.InvalidateAfter(55);
	SWTest.Assert(historyStack.TextValue()->Equals(&first));
	historyStack.InvalidateAfter(45);
	SWTest.Assert(historyStack.TextValue()->Equals(&second));
	historyStack.InvalidateAfter(35);
	SWTest.Assert(historyStack.TextValue()->Equals(&third));
	historyStack.InvalidateAfter(25);
	SWTest.Assert(historyStack.TextValue()->Equals(&second));
	historyStack.InvalidateAfter(15);
	SWTest.Assert(historyStack.TextValue()->Equals(&first));
	historyStack.InvalidateAfter(5);
	SWTest.Assert(historyStack.TextValue() == NULL);

}

/// MarkerSet objects hold a set of flags for each line in a SplitText.
class MarkerSet : public LineData {
private:
public:
	SplitVector_4 *markers;
	MarkerSet() {
		markers = new SplitVector_4();
		markers->Create(10,10);
	}
	virtual ~MarkerSet() {
		delete markers;
	}
	void Clear() {
		markers->DeleteRange(0, markers->Length());
	}
	void InsertLine(int position) {
		markers->InsertSpace(position, 1);
	}
	void RemoveLine(int position) {
		markers->DeleteRange(position, 1);
	}
};

#ifdef TESTC
int xmain()
#else
int main()
#endif
{
SWTest.Write("jehrbf wjerhb");
SWTest.Write("\n");
	TestHistory();

	WordList wl(false);
	Literal keywords("alpha \250theta gamma");
	wl.Set(keywords.Data(), keywords.Length());
	Literal alpha("alpha");
	Literal theta("\250theta");
	Literal gamma("gamma");
	Literal delta("delta");
	Literal large("\240large");
	if (wl.InList(alpha.Data(), 0, alpha.Length()))
		SWTest.Write("Contains alpha");
	else
		SWTest.Write("Does not contain alpha");
	SWTest.Write("\n");
	if (wl.InList(theta.Data(), 0, theta.Length()))
		SWTest.Write("Contains theta");
	else
		SWTest.Write("Does not contain theta");
	SWTest.Write("\n");
	if (wl.InList(gamma.Data(), 0, gamma.Length()))
		SWTest.Write("Contains gamma");
	else
		SWTest.Write("Does not contain beta");
	SWTest.Write("\n");
	if (wl.InList(delta.Data(), 0, delta.Length()))
		SWTest.Write("Contains delta");
	else
		SWTest.Write("Does not contain delta");
	SWTest.Write("\n");
	if (wl.InList(large.Data(), 0, large.Length()))
		SWTest.Write("Contains large");
	else
		SWTest.Write("Does not contain large");
	SWTest.Write("\n");

	testUndoRedo();

	testlv();
	//if (1)
	//return 0;

	SWTest.WriteLine("Start test");
	SplitVector_1 buffer;
	buffer.Create(20, 100);
	SWTest.Assert(buffer.Length() == 0);
	Literal test("abcde");
	SW_BYTE *tt = test.AsUTF8();
	try {
		buffer.InsertString(1, tt, 1);
	} catch (BadPositionException *e) {
		SWTest.Write("Caught\n");
		delete e;
	}
	int lenTest = test.Length();
	buffer.InsertString(0, tt, lenTest);
	delete []tt;
	SWTest.Assert(buffer.Length() == lenTest);
	SW_BYTE ret[3];
	buffer.RetrieveString(1, ret, 3);
	for (int h = 0; h < 3; h++) {
		SWTest.Assert(ret[h] == test.CharAt(h + 1));
	}
	for (int i = 0; i < lenTest; i++) {
		SWTest.Assert(test.CharAt(i) == buffer.CharAt(i));
		SWTest.Write(i);
		SWTest.Write(":");
		SWTest.Write(buffer.CharAt(i));
		SWTest.Write(" ");
	}
	SWTest.Write("\n");
	buffer.DeleteRange(1, 2);
	SWTest.Assert(buffer.Length() == lenTest - 2);
	//for (int j = -1; j < buffer.Length() + 1; j++) {
	for (int j = 0; j < buffer.Length(); j++) {
		SWTest.Write(j);
		SWTest.Write("=");
		SWTest.Write(buffer.CharAt(j));
		SWTest.Write(" ");
	}
	SWTest.Write("\n");
	Literal shortInsert("bc");
	SW_BYTE *si = shortInsert.AsUTF8();
	buffer.InsertString(1, si, 2);
	SWTest.Assert(buffer.Length() == lenTest);
	for (int k = 0; k < lenTest; k++) {
		SWTest.Assert(test.CharAt(k) == buffer.CharAt(k));
		SWTest.Write(k);
		SWTest.Write(":");
		SWTest.Write(buffer.CharAt(k));
		SWTest.Write(" ");
	}
	delete []si;
	SWTest.Write("\n");
	double st = SWTest.Now();
	Literal longtest("ghijklmnopqrstuvwxyz");
	SW_BYTE *lt = longtest.AsUTF8();
	for (int l = 0; l < 100; l++) {
		for (int m = 0; m < 20; m++) {
			buffer.InsertString(1, lt, m + 1);
		}
	}
	delete []lt;

	SplitText *pbuffer = new SplitText();
	pbuffer->AttachStyles(SplitText::CreateStyles(1), NULL, 0, false);
	//pbuffer->DiscardLines();
	SWTest.Assert(pbuffer->Length() == 0);
	SWTest.Assert(pbuffer->Lines() == 1);
	pbuffer->InsertUTF32(0, test.Data(), lenTest);
	SWTest.Assert(pbuffer->Length() == lenTest);
	for (int n = 0; n < lenTest; n++) {
		SWTest.Assert(test.CharAt(n) == pbuffer->XCharAt(n));
		//~ SWTest.Write(n);
		//~ SWTest.Write(":");
		//~ SWTest.Write(pbuffer->CharAt(n));
		//~ SWTest.Write(" ");
	}
	SWTest.Write("\n");

	double t2 = SWTest.Now();
	SWTest.Write(t2-st);
	SWTest.Write(" seconds for Insertion test\n");

	SplitText *sbuffer = new SplitText();
	IStyles *sstyle = SplitText::CreateStyles(1);
	LexCPP lx;
	sbuffer->AttachStyles(sstyle, &lx, 0, false);
	MarkerSet *ms = new MarkerSet;
	sbuffer->AddPerLineData(ms);
	MarkerSet *ms3 = new MarkerSet;
	sbuffer->AddPerLineData(ms3);
	MarkerSet *ms4 = new MarkerSet;
	sbuffer->AddPerLineData(ms4);
	//sbuffer->DiscardLines();
	Literal prog(
		"//// A demonstration program\n"
		"int main() {\n"
		"\tprintf(\"hello world %d\\n\", 9);\n"
		"}\n");
	int lenProg = prog.Length();
	sbuffer->InsertUTF32(0, prog.Data(), lenProg);
	SWTest.Assert(sbuffer->Lines() == 5);
	SWTest.Assert(ms->markers->Length() == 5);
	// Ensure removal works
	sbuffer->RemovePerLineData(ms);
	sbuffer->RemovePerLineData(ms3);
	sbuffer->RemovePerLineData(ms4);
	Literal keywordsCPP(
		"abstract as base bool break byte case catch char "
		"checked class const continue decimal default "
		"delegate do double else enum event explicit "
		"extern false finally fixed float for foreach goto "
		"if implicit in int interface internal is lock long "
		"namespace new null object operator out override "
		"params private protected public readonly ref "
		"return sbyte sealed short sizeof stackalloc "
		"static string struct switch this throw true try "
		"typeof uint ulong unchecked unsafe ushort "
		"using virtual void while");
	lx.SetKeyWords(1, &keywordsCPP);
	sbuffer->LexTo(sbuffer->Length(), 0, false);
	//ShowStyled(sbuffer);

	Literal html(
		"<html xmlns=\"http://www.w3.org/1999/xhtml\">\n"
		"<head>\n"
		"    <meta name=\"Date.Modified\" content=\"20010515\" />\n"
		"    <title>SinkWorld - Portability</title>\n"
		"    <link rel=\"stylesheet\" type=\"text/css\" href=\"SW.css\">\n"
		"</head>\n");
	Literal keywordsHTML(
		"a abbr acronym address applet area b base basefont "
		"bdo big blockquote body br button caption center "
		"cite code col colgroup dd del dfn dir div dl dt em "
		"fieldset font form frame frameset h1 h2 h3 h4 h5 h6 "
		"head hr html i iframe img input ins isindex kbd label "
		"legend li link map menu meta noframes noscript "
		"object ol optgroup option p param pre q s samp "
		"script select small span strike strong style sub sup "
		"table tbody td textarea tfoot th thead title tr tt u ul "
		"var xml xmlns");
	TestLanguage(LexHTML(), html, keywordsHTML);
	SWTest.Write("\n");

	Literal python(
		"# Convert all punctuation characters except '_', '*', and '.' into spaces.\n"
		"def depunctuate(s):\n"
		"	\'\'\'A docstring\'\'\'\n"
		"	\"\"\"Docstring 2\"\"\"\n"
		"	d = \"\"\n"
		"	for ch in s:\n"
		"		if ch in 'abcde':\n"
		"			d = d + ch\n"
		"		else:\n"
		"			d = d + " "\n"
		"	return d\n");
	Literal keywordsPython(
		"and assert break class continue def del elif "
		"else except exec finally for from global if import in is lambda None "
		"not or pass print raise return try while yield");
	TestLanguage(LexPython(), python, keywordsPython);

	Literal keywordsVB(
		"and begin case call continue do each else elseif end erase "
		"error event exit false for function get gosub goto if implement in load loop lset "
		"me mid new next not nothing on or property raiseevent rem resume return rset "
		"select set stop sub then to true unload until wend while with withevents "
		"attribute alias as boolean byref byte byval const compare currency date declare dim double "
		"enum explicit friend global integer let lib long module object option optional "
		"preserve private property public redim single static string type variant");

	Literal vbSource(
		"' String"
		"Dim a As String = \"hello, world\"\n"
		"Dim b As String = \"hello    world\"\n"
		"Dim c As String = \"Joe said \"\"Hello\"\" to me\"\n"
		"Dim d As String = \"\\\\server\\share\\file.txt\"\n"
		"' Character\n"
		"\"\"C \"c\"C \"cc\"C\n"
		"' Date\n"
		"d = #5/31/1993# or # 01/01/0001 12:00:00AM #\n");
	TestLanguage(LexVB(), vbSource, keywordsVB);

	Literal phpSource(
		"<head> <!-- About to script -->\n"
		"<?php\n"
		"echo \"<!-- -->\n\";\n"
		"/* ?> */\n"
		"?>\n"
		"<strong>for</strong><b>if</b>\n");
	Literal keywordsPHP(
		"and argv as argc break case cfunction class continue declare default do "
		"die echo else elseif empty enddeclare endfor endforeach endif endswitch "
		"endwhile e_all e_parse e_error e_warning eval exit extends false for "
		"foreach function global http_cookie_vars http_get_vars http_post_vars "
		"http_post_files http_env_vars http_server_vars if include include_once "
		"list new not null old_function or parent php_os php_self php_version "
		"print require require_once return static switch stdclass this true var "
		"xor virtual while __file__ __line__ __sleep __wakeup");
	TestLanguage(LexPHP(), phpSource, keywordsPHP);
	SWTest.Write("\n");

	LexPHP lxphp;
	lxphp.SetKeyWords(0, &keywordsPHP);
	LexHTML lxht;
	lxphp.SetSubLexer(&lxht, new Literal("html"), NULL);
	SplitText *phbuffer = new SplitText();
	IStyles *phstyle = SplitText::CreateStyles(4);
	phbuffer->AttachStyles(phstyle, &lxphp, 0, false);
	phbuffer->InsertUTF32(0, phpSource.Data(), phpSource.Length());
	lxphp.InvalidateAfter(0);
	phbuffer->LexTo(phbuffer->Length(), 0, false);
	//ShowStyled(phbuffer);
	//SWTest.Write("\n");

	LexASP la;
	LexHTML lxx;
	LexCPP lxc;
	LexCPP lxj;
	LexVB lxvb;
	lxx.SetSubLexer(&lxj, new Literal("javascript"), new Literal("jscript"));
	lxx.SetSubLexer(&lxvb, new Literal("vbs"), NULL);
	la.SetSubLexer(&lxx, new Literal("html"), NULL);
	la.SetSubLexer(&lxc, new Literal("javascript"), new Literal("jscript"));
	la.SetSubLexer(&lxvb, new Literal("vbs"), NULL);
	lxc.SetKeyWords(0, &keywordsCPP);
	lxj.SetKeyWords(0, &keywordsCPP);
	lxvb.SetKeyWords(0, &keywordsVB);
	lxx.SetKeyWords(0, &keywordsHTML);
	Literal asp(
		"<%@language=javas%>\n"
		"<% \n"
		"#include \n"
		"function x() {\n"
		"}\n"
		"%>\n"
		"<%@language=vbscript%>\n"
		"<% \n"
		"sub x 'comment \n"
		"%>\n"
		"<head>\n"
		"<script language=\"vbscript\">\n"
		"function changeMagnification(n) {\n"
		"	alert(\":\" + document.b.getMagnification());\n"
		"	document.a.setMagnification(document.a.getMagnification() + n);\n"
		"}\n"
		"</script>\n"
		);
	SplitText *abuffer = new SplitText();
	IStyles *astyle = SplitText::CreateStyles(4);
	abuffer->AttachStyles(astyle, &la, 0, false);
	abuffer->InsertUTF32(0, asp.Data(), asp.Length());
	la.InvalidateAfter(0);
	abuffer->LexTo(abuffer->Length(), 0, false);
	//ShowStyled(abuffer);
	SWTest.Write("\n");

	for (int liney = 0; liney<abuffer->Lines(); liney++) {
		int lineStart = abuffer->PositionFromLine(liney);
		int lineEnd = abuffer->LineEndPositionFromLine(liney);
		la.InvalidateAfter(lineStart);
		abuffer->LexTo(lineEnd, 0, false);
	}
	//ShowStyled(abuffer);

	// Start buffer as big as needed to avoid measuring allocation and copying
	SplitText bigbuffer(300000,1024);
	IStyles *bigstyle = SplitText::CreateStyles(4);
	bigbuffer.AttachStyles(bigstyle, &lx, 0, false);
	MarkerSet *ms2 = new MarkerSet;
	bigbuffer.AddPerLineData(ms2);
	//bigbuffer.DiscardLines();
	// prog is 80 characters so create a realistic large 120K document
	for (int q=0; q<3000; q++) {
		// Insert in middle to require copying of lines information
		bigbuffer.InsertUTF32(lenProg*(q/2), prog.Data(), lenProg);
	}
	for (int q2=0; q2<1500; q2++) {
		// Delete in middle to require copying of lines information
		bigbuffer.DeleteRange(lenProg*(q2/2), lenProg);
	}
	double t3 = SWTest.Now();
	SWTest.Write(t3-t2);
	SWTest.Write(" seconds for Lines Insertion Deletion test\n");
	SWTest.Assert(bigbuffer.Length() == 1500 * lenProg);
	SWTest.Assert(bigbuffer.Lines() == 1500 * 4 + 1);

	// Run 50 times to get a large enough time to measure Colourise operation
	for (int p=0; p<50; p++) {
		bigbuffer.LexTo(0, 0, false);
		bigbuffer.LexTo(bigbuffer.Length(), 0, false);
	}

	double t31 = SWTest.Now();
	SWTest.Write((t31-t3) / 50.0);
	SWTest.Write(" seconds for Colourise test\n");

	int biglength = bigbuffer.Length();

	// Search for "printf"
	Literal printfs("printf");
	int lenPrintfs = printfs.Length();
	for (int r=0; r<50; r++) {
		int count = 0;
		for (int s=0; s<biglength; s++) {
			if (bigbuffer.XCharAt(s) == printfs.CharAt(0)) {
				int found = 1;
				for (int t=1; t<lenPrintfs && found==1; t++) {
					if (bigbuffer.XCharAt(s+t) != printfs.CharAt(t)) {
						found = 0;
					}
				}
				if (found==1) {
					count++;
				}
			}
		}
		SWTest.Assert(count == 1500);
	}

	double t4 = SWTest.Now();
	SWTest.Write((t4-t31) / 50.0);
	SWTest.Write(" seconds for Search test\n");

	// Search for "printf" and replace with "OutputTrace"
	// Then reverse the process so it can be done again
	Literal replace("OutputTrace");
	int lenReplace = replace.Length();
	for (int u=0; u<50; u++) {
		int count = 0;
		for (int s=0; s<biglength; s++) {
			if (bigbuffer.XCharAt(s) == printfs.CharAt(0)) {
				int found = 1;
				for (int t=1; t<lenPrintfs && found==1; t++) {
					if (bigbuffer.XCharAt(s+t) != printfs.CharAt(t)) {
						found = 0;
					}
				}
				if (found==1) {
					count++;
					bigbuffer.DeleteRange(s, lenPrintfs);
					bigbuffer.InsertUTF32(s, replace.Data(), lenReplace);
					biglength += lenReplace - lenPrintfs;
				}
			}
		}
		SWTest.Assert(count == 1500);
		count = 0;
		for (int s1=0; s1<biglength; s1++) {
			if (bigbuffer.XCharAt(s1) == replace.CharAt(0)) {
				int found = 1;
				for (int t=1; t<lenReplace && found==1; t++) {
					if (bigbuffer.XCharAt(s1+t) != replace.CharAt(t)) {
						found = 0;
					}
				}
				if (found==1) {
					count++;
					bigbuffer.DeleteRange(s1, lenReplace);
					bigbuffer.InsertUTF32(s1, printfs.Data(), lenPrintfs);
					biglength += lenPrintfs - lenReplace;
				}
			}
		}
		SWTest.Assert(count == 1500);
	}

	double en = SWTest.Now();
	SWTest.Write((en-t4) / 100.0);
	SWTest.Write(" seconds for Replace test\n");

	delete pbuffer;
	delete sbuffer;

	return 0;
}

