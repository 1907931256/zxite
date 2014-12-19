#include <string>
#include <assert.h>
#include <vector>
#include <iostream>
#include <typeinfo>

#include "cppunit/TestCase.h"
#include "cppunit/TestCaller.h"
#include "cppunit/TestSuite.h"
#include "cppunit/TestResult.h"
#include "cppunit/TestRunner.h"
#include "cppunit/Test.h"
#include "cppunit/TextTestResult.h"
#include "cppunit/CompilerOutputter.h"
#include "cppunit/extensions/HelperMacros.h"

#include "base.h"
#include "lexers.h"
/*

Tests should be ordered from most basic classes with no dependencies to those
with simple dependencies to more complex classes. This ensures that breakage
in an underlying class is diagnosed at that level where it should be easier to
diagnose than in a more complex class that uses it.

-- Trivial, interfaces and abstract classes
BadPositionException
ILexer
IStyles
LineData
Substance:SplitVector_1
SymbolEvent

-- Private
KWNode
DictionaryElement
HistoricEvent
UndoAction
StylesWithValidity

-- No dependencies or depends only on private classes
Literal
LiteralSet
SetIntegers
MaxFinder
SimpleIntegerVector
SinglePosition
SplitVector_0
SplitVector_1
SplitVector_2
SplitVector_4
TextPositions
UniConversion
ByteBuffer

-- Have dependencies

SubstanceUTF8:SplitVector_1
SubstanceByte:SplitVector_1
SubstanceUTF16LE:SplitVector_1
SubstanceUTF16BE:SplitVector_1

SplitVectorWithRangeAdd:SplitVector_4
Partitioning:SplitVectorWithRangeAdd
RunStyles:Partitioning:SplitVector_4

KeyWords:Literal
Dictionary:Literal
OptionSet:Literal:Dictionary
History:Literal
WordList:Literal
SymbolTable:Literal

UndoStack:SimpleIntegerVector:SplitVector_1
SplitText:Substance:Partitioning:StylesWithValidity:UndoStack
Styler:SplitText:Literal:KeyWords

LexNull
LexBase
LexScript
LexBraceCount
LexCPP
LexPython
LexSpell
LexURL
LexVB
LexHTML
LexASP
LexPHP

TODO:
*/

unsigned char shortInsert[] = "bc";
unsigned char mediumInsert[] = "cronulla";

class LiteralTest : public CppUnit::TestFixture {
CPPUNIT_TEST_SUITE(LiteralTest);
CPPUNIT_TEST(testCreation);
CPPUNIT_TEST(testString);
CPPUNIT_TEST(testInteger);
CPPUNIT_TEST(testContents);
CPPUNIT_TEST(testSegment);
CPPUNIT_TEST_SUITE_END();

	void testCreation() {
		// Empty literal
		Literal *l0 = new Literal();
		CPPUNIT_ASSERT_EQUAL(0, l0->Length());
		CPPUNIT_ASSERT_EQUAL((const int *)NULL, l0->Data());
		// Literal "bc"
		Literal *l = new Literal((char *)shortInsert);
		CPPUNIT_ASSERT_EQUAL(2, l->Length());
		CPPUNIT_ASSERT_EQUAL(603887303, l->HashValue());
		// Check copy
		Literal *l2 = new Literal(l);
		CPPUNIT_ASSERT(l->Equals(l2));
		CPPUNIT_ASSERT(l2->Equals(l));
		// Check range copy of whole string
		Literal *l3 = new Literal(l, 0, l->Length());
		CPPUNIT_ASSERT(l->Equals(l3));
		// Check range copy of substring
		Literal *l4 = new Literal(l, 1, 1);
		CPPUNIT_ASSERT_EQUAL(1, l4->Length());
		unsigned char cInsert[] = "c";
		Literal *l5 = new Literal((char *)cInsert);
		CPPUNIT_ASSERT_EQUAL(1, l5->Length());
		CPPUNIT_ASSERT(l4->Equals(l5));
		CPPUNIT_ASSERT(!l->Equals(l5));
		// Check combination
		Literal *l6 = new Literal(l, (SW_BYTE) '|', l5);
		CPPUNIT_ASSERT_EQUAL(4, l6->Length());
		unsigned char cCombined[] = "bc|c";
		Literal *l7 = new Literal((char *)cCombined);
		CPPUNIT_ASSERT(l7->Equals(l6));
		// Check operations on null
		Literal *l8 = new Literal();
		CPPUNIT_ASSERT(!l8->Equals(l7));
		Literal *l9 = new Literal(l8);
		CPPUNIT_ASSERT(l9->Equals(l8));

		delete l9;
		delete l8;
		delete l7;
		delete l6;
		delete l5;
		delete l4;
		delete l3;
		delete l2;
		delete l;
	}

	void testString() {
		// Setting from string literals
		Literal *l = new Literal("");
		CPPUNIT_ASSERT_EQUAL(0, l->Length());
		CPPUNIT_ASSERT(l->Data() != (int *)NULL);
		Literal *l2 = new Literal("lakemba");
		CPPUNIT_ASSERT_EQUAL(7, l2->Length());
		CPPUNIT_ASSERT(!l->Equals(l2));
		// Stack allocation
		Literal lStack("cronulla");
		CPPUNIT_ASSERT(!lStack.Equals(l));
		// Copy constructor
		Literal *l3 = new Literal(lStack);
		CPPUNIT_ASSERT(l3->Equals(&lStack));
		Literal lStack2 = lStack;
		CPPUNIT_ASSERT(lStack.Equals(&lStack2));
		delete l;
		delete l2;
		delete l3;
	}

	void testInteger() {
		// Check integer constructor
		Literal l(123);
		CPPUNIT_ASSERT_EQUAL(123, l.Value());
		CPPUNIT_ASSERT_EQUAL(3, l.Length());
		// Check decoding when set from string
		Literal l2("4567");
		CPPUNIT_ASSERT_EQUAL(4567, l2.Value());
		CPPUNIT_ASSERT_EQUAL(4, l2.Length());
		// Check negative numbers
		Literal l3(-98);
		CPPUNIT_ASSERT_EQUAL(-98, l3.Value());
		CPPUNIT_ASSERT_EQUAL(3, l3.Length());
		// Check null
		Literal l4;
		CPPUNIT_ASSERT_EQUAL(0, l4.Value());
		CPPUNIT_ASSERT_EQUAL(0, l4.Length());
		// Check 0
		Literal l5(0);
		CPPUNIT_ASSERT_EQUAL(0, l5.Value());
		CPPUNIT_ASSERT_EQUAL(1, l5.Length());
	}

	void testContents() {
		// Check character access
		Literal l("woolloomooloo");
		CPPUNIT_ASSERT_EQUAL((int)'w', l.CharAt(0));
		CPPUNIT_ASSERT_EQUAL((int)'o', l.CharAt(1));
		CPPUNIT_ASSERT_EQUAL((int)'l', l.CharAt(10));
		CPPUNIT_ASSERT_EQUAL((int)'o', l.CharAt(12));
		// Character counting
		CPPUNIT_ASSERT_EQUAL(1, l.Count('w'));
		CPPUNIT_ASSERT_EQUAL(8, l.Count('o'));
		CPPUNIT_ASSERT_EQUAL(0, l.Count('x'));
		// Match
		Literal loo("loo");
		CPPUNIT_ASSERT(!l.Match(&loo, 0));
		CPPUNIT_ASSERT(l.Match(&loo, 4));
		// EndsWith
		Literal loox("loox");
		CPPUNIT_ASSERT(l.EndsWith(&loo));
		CPPUNIT_ASSERT(!l.EndsWith(&loox));
		// Try longer than subject
		CPPUNIT_ASSERT(!loo.EndsWith(&loox));
		// Find
		CPPUNIT_ASSERT_EQUAL(4, l.Find(&loo, 0));
		CPPUNIT_ASSERT_EQUAL(-1, l.Find(&loox, 0));
		CPPUNIT_ASSERT_EQUAL(4, l.Find(&loo, 4));
		CPPUNIT_ASSERT_EQUAL(10, l.Find(&loo, 5));
		CPPUNIT_ASSERT_EQUAL(-1, loo.Find(&loox, 0));
		// IsOn
		int shortInsertI[] = {'b', 'c'};
		CPPUNIT_ASSERT(!loo.IsIn(shortInsertI, 0, 2));
		CPPUNIT_ASSERT(loo.IsIn(l.Data(), 6, l.Length()));
	}

	void testSegment() {
		Literal lSet("a:bc::e");
		Literal *lSegment;

		lSegment = lSet.Segment(0, ':');
		Literal la("a");
		CPPUNIT_ASSERT(lSegment->Equals(&la));
		delete lSegment;

		lSegment = lSet.Segment(1, ':');
		Literal lbc("bc");
		CPPUNIT_ASSERT(lSegment->Equals(&lbc));
		delete lSegment;

		lSegment = lSet.Segment(2, ':');
		Literal lnull("");
		CPPUNIT_ASSERT(lSegment->Equals(&lnull));
		delete lSegment;

		lSegment = lSet.Segment(3, ':');
		Literal le("e");
		CPPUNIT_ASSERT(lSegment->Equals(&le));
		delete lSegment;

		lSegment = lSet.Segment(4, ':');
		CPPUNIT_ASSERT(lSegment->Equals(&lnull));
		delete lSegment;

		// Not present
		lSegment = lSet.Segment(4, '$');
		CPPUNIT_ASSERT(lSegment->Equals(&lnull));
		delete lSegment;
	}
};


class LiteralSetTest : public CppUnit::TestFixture {
CPPUNIT_TEST_SUITE(LiteralSetTest);
CPPUNIT_TEST(testCreation);
CPPUNIT_TEST_SUITE_END();

	void testCreation() {
		Literal litShortInsert("bc");
		Literal litMediumInsert("cronulla");

		LiteralSet *ls = new LiteralSet();
		CPPUNIT_ASSERT(ls);
		Literal *a = ls->FindOrAddKey(&litShortInsert);
		CPPUNIT_ASSERT(a);
		CPPUNIT_ASSERT_EQUAL(2, a->Length());
		CPPUNIT_ASSERT_EQUAL(603887303, a->HashValue());
		// Test that the same value is returned
		CPPUNIT_ASSERT_EQUAL(a, ls->FindOrAddKey(&litShortInsert));
		// Even after adding another
		Literal *b = ls->FindOrAddKey(&litMediumInsert);
		CPPUNIT_ASSERT(b);
		CPPUNIT_ASSERT_EQUAL(a, ls->FindOrAddKey(&litShortInsert));
		CPPUNIT_ASSERT_EQUAL(b, ls->FindOrAddKey(&litMediumInsert));
		delete ls;
	}
};


class SetIntegersTest : public CppUnit::TestFixture {
CPPUNIT_TEST_SUITE(SetIntegersTest);
CPPUNIT_TEST(testCreation);
CPPUNIT_TEST_SUITE_END();

	void testCreation() {
		SetIntegers *si = new SetIntegers(3);
		CPPUNIT_ASSERT(si);
		CPPUNIT_ASSERT_EQUAL(0, si->Length());
		si->Add(10);
		CPPUNIT_ASSERT_EQUAL(1, si->Length());
		CPPUNIT_ASSERT_EQUAL(10, si->At(0));
		si->Add(20);
		CPPUNIT_ASSERT_EQUAL(2, si->Length());
		CPPUNIT_ASSERT_EQUAL(10, si->At(0));
		CPPUNIT_ASSERT_EQUAL(20, si->At(1));
		delete si;
	}
};


class MaxFinderTest : public CppUnit::TestFixture {
CPPUNIT_TEST_SUITE(MaxFinderTest);
CPPUNIT_TEST(testAdditionRemoval);
CPPUNIT_TEST(testMinimum);
CPPUNIT_TEST(testProblem);
CPPUNIT_TEST_SUITE_END();

	MaxFinder *mf;
public:
	void setUp() {
		mf = new MaxFinder();
	}

	void tearDown() {
		delete mf;
	}

	void testAdditionRemoval() {
		CPPUNIT_ASSERT_EQUAL(0, mf->Length());
		CPPUNIT_ASSERT_EQUAL(0, mf->Maximum());
		mf->Add(3);
		CPPUNIT_ASSERT_EQUAL(1, mf->Length());
		CPPUNIT_ASSERT_EQUAL(3, mf->Maximum());
		mf->Add(2);
		CPPUNIT_ASSERT_EQUAL(2, mf->Length());
		CPPUNIT_ASSERT_EQUAL(3, mf->Maximum());
		mf->Remove(3);
		CPPUNIT_ASSERT_EQUAL(1, mf->Length());
		CPPUNIT_ASSERT_EQUAL(2, mf->Maximum());
		mf->Add(5);
		CPPUNIT_ASSERT_EQUAL(2, mf->Length());
		CPPUNIT_ASSERT_EQUAL(5, mf->Maximum());
		mf->Add(5);
		CPPUNIT_ASSERT_EQUAL(3, mf->Length());
		CPPUNIT_ASSERT_EQUAL(5, mf->Maximum());
		mf->Add(6);
		CPPUNIT_ASSERT_EQUAL(4, mf->Length());
		CPPUNIT_ASSERT_EQUAL(6, mf->Maximum());
		mf->Remove(5);
		mf->Remove(5);
		mf->Remove(6);
		CPPUNIT_ASSERT_EQUAL(1, mf->Length());
		CPPUNIT_ASSERT_EQUAL(2, mf->Maximum());
	}

	void testMinimum() {
		mf->Allocate(2);
		CPPUNIT_ASSERT_EQUAL(0, mf->Length());
		CPPUNIT_ASSERT_EQUAL(0, mf->Maximum());
		mf->SetMinimum(4);
		CPPUNIT_ASSERT_EQUAL(0, mf->Length());
		CPPUNIT_ASSERT_EQUAL(4, mf->Maximum());
		mf->Add(3);
		CPPUNIT_ASSERT_EQUAL(0, mf->Length());
		CPPUNIT_ASSERT_EQUAL(4, mf->Maximum());
		mf->Add(2);
		CPPUNIT_ASSERT_EQUAL(0, mf->Length());
		CPPUNIT_ASSERT_EQUAL(4, mf->Maximum());
		mf->Remove(3);
		CPPUNIT_ASSERT_EQUAL(0, mf->Length());
		CPPUNIT_ASSERT_EQUAL(4, mf->Maximum());
		mf->Add(5);
		CPPUNIT_ASSERT_EQUAL(1, mf->Length());
		CPPUNIT_ASSERT_EQUAL(5, mf->Maximum());
		mf->Add(5);
		CPPUNIT_ASSERT_EQUAL(2, mf->Length());
		CPPUNIT_ASSERT_EQUAL(5, mf->Maximum());
		mf->Add(6);
		CPPUNIT_ASSERT_EQUAL(3, mf->Length());
		CPPUNIT_ASSERT_EQUAL(6, mf->Maximum());
		mf->Remove(5);
		mf->Remove(5);
		mf->Remove(6);
		CPPUNIT_ASSERT_EQUAL(0, mf->Length());
		CPPUNIT_ASSERT_EQUAL(4, mf->Maximum());
	}

	void testProblem() {
		// This sequence caused a problem on an earlier version
		mf->Add(446);
		CPPUNIT_ASSERT_EQUAL(446, mf->Maximum());
		mf->Add(516);
		CPPUNIT_ASSERT_EQUAL(516, mf->Maximum());
		mf->Add(0);
		CPPUNIT_ASSERT_EQUAL(516, mf->Maximum());
		mf->Remove(446);
		CPPUNIT_ASSERT_EQUAL(516, mf->Maximum());
		mf->Add(447);
		CPPUNIT_ASSERT_EQUAL(516, mf->Maximum());
		mf->Remove(516);
		CPPUNIT_ASSERT_EQUAL(447, mf->Maximum());
		mf->Remove(447);
		CPPUNIT_ASSERT_EQUAL(0, mf->Maximum());
	}
};


class SIVTest : public CppUnit::TestFixture {
CPPUNIT_TEST_SUITE(SIVTest);
CPPUNIT_TEST(testCreation);
CPPUNIT_TEST_SUITE_END();

	void testCreation() {
		SimpleIntegerVector *siv = new SimpleIntegerVector();
		CPPUNIT_ASSERT(siv);
		CPPUNIT_ASSERT_EQUAL(0, siv->Length());
		// Add an element
		siv->SetValueAllowingExpansion(3, 30);
		CPPUNIT_ASSERT_EQUAL(4, siv->Length());
		CPPUNIT_ASSERT_EQUAL(30, siv->ValueAt(3));
		// Change an element
		siv->SetValueAt(1, 11);
		CPPUNIT_ASSERT_EQUAL(4, siv->Length());
		CPPUNIT_ASSERT_EQUAL(11, siv->ValueAt(1));
		// Insertion
		siv->Insert(2, 20);
		CPPUNIT_ASSERT_EQUAL(5, siv->Length());
		CPPUNIT_ASSERT_EQUAL(20, siv->ValueAt(2));
		CPPUNIT_ASSERT_EQUAL(30, siv->ValueAt(4));
		// Deletion
		siv->Delete(1);
		CPPUNIT_ASSERT_EQUAL(4, siv->Length());
		CPPUNIT_ASSERT_EQUAL(20, siv->ValueAt(1));
		CPPUNIT_ASSERT_EQUAL(30, siv->ValueAt(3));
		// Adding to values
		siv->RangeAddDelta(0, 3, 2);
		CPPUNIT_ASSERT_EQUAL(4, siv->Length());
		CPPUNIT_ASSERT_EQUAL(22, siv->ValueAt(1));
		CPPUNIT_ASSERT_EQUAL(30, siv->ValueAt(3));
		siv->RangeAddDelta(0, 4, 3);
		CPPUNIT_ASSERT_EQUAL(4, siv->Length());
		CPPUNIT_ASSERT_EQUAL(25, siv->ValueAt(1));
		CPPUNIT_ASSERT_EQUAL(33, siv->ValueAt(3));
		delete siv;
	}
};


class SPTest : public CppUnit::TestFixture {
CPPUNIT_TEST_SUITE(SPTest);
CPPUNIT_TEST(testCreation);
CPPUNIT_TEST_SUITE_END();

	SinglePosition *sp;
public:
	void setUp() {
		sp = new SinglePosition();
	}

	void tearDown() {
		delete sp;
	}

	void testCreation() {
		// Note that a single point does not have an automatic change point at the end
		CPPUNIT_ASSERT_EQUAL(0, sp->Length());
		CPPUNIT_ASSERT_EQUAL(0, sp->Get());
		CPPUNIT_ASSERT_EQUAL(0, sp->FindNextChange(-1, sp->Length()));
		CPPUNIT_ASSERT_EQUAL(1, sp->FindNextChange(0, sp->Length()));
		// After insertion
		sp->InsertSpace(0, 10);
		CPPUNIT_ASSERT_EQUAL(10, sp->Length());
		CPPUNIT_ASSERT_EQUAL(10, sp->Get());
		CPPUNIT_ASSERT_EQUAL(10, sp->FindNextChange(-1, sp->Length()));
		CPPUNIT_ASSERT_EQUAL(10, sp->FindNextChange(0, sp->Length()));
		CPPUNIT_ASSERT_EQUAL(11, sp->FindNextChange(10, sp->Length()));
		// Move
		sp->FillRange(7, 1, 0);
		CPPUNIT_ASSERT_EQUAL(10, sp->Length());
		CPPUNIT_ASSERT_EQUAL(7, sp->Get());
		CPPUNIT_ASSERT_EQUAL(7, sp->FindNextChange(-1, sp->Length()));
		CPPUNIT_ASSERT_EQUAL(7, sp->FindNextChange(0, sp->Length()));
		CPPUNIT_ASSERT_EQUAL(11, sp->FindNextChange(7, sp->Length()));
		// After deletion
		sp->DeleteRange(2, 3);
		CPPUNIT_ASSERT_EQUAL(7, sp->Length());
		CPPUNIT_ASSERT_EQUAL(4, sp->Get());
		CPPUNIT_ASSERT_EQUAL(4, sp->FindNextChange(-1, sp->Length()));
		CPPUNIT_ASSERT_EQUAL(4, sp->FindNextChange(0, sp->Length()));
		CPPUNIT_ASSERT_EQUAL(8, sp->FindNextChange(4, sp->Length()));
		for (int i=0;i<sp->Length();i++) {
			CPPUNIT_ASSERT_EQUAL(0, sp->ValueAt(i));
		}
		// Clear
		sp->DeleteAll();
		CPPUNIT_ASSERT_EQUAL(0, sp->Length());
		CPPUNIT_ASSERT_EQUAL(0, sp->Get());
		CPPUNIT_ASSERT_EQUAL(0, sp->FindNextChange(-1, sp->Length()));
		CPPUNIT_ASSERT_EQUAL(1, sp->FindNextChange(0, sp->Length()));
	}
};

class SV0Test : public CppUnit::TestFixture {
CPPUNIT_TEST_SUITE(SV0Test);
CPPUNIT_TEST(testEmpty);
CPPUNIT_TEST(testInsertion);
CPPUNIT_TEST(testFill);
CPPUNIT_TEST(testDeletion);
CPPUNIT_TEST(testDeleteAll);
CPPUNIT_TEST_SUITE_END();

	SplitVector_0 *sv;

public:

	void setUp() {
		sv = new SplitVector_0();
	}

	void tearDown() {
		delete sv;
	}

	void testEmpty() {
		CPPUNIT_ASSERT_EQUAL(0, sv->Length());
		CPPUNIT_ASSERT_EQUAL(0, sv->FindNextChange(-1, 0));
		CPPUNIT_ASSERT_EQUAL(1, sv->FindNextChange(0, 0));
	}

	void testInsertion() {
		// After insertion
		sv->InsertSpace(0, 10);
		CPPUNIT_ASSERT_EQUAL(10, sv->Length());
		CPPUNIT_ASSERT_EQUAL(10, sv->FindNextChange(-1, sv->Length()));
		CPPUNIT_ASSERT_EQUAL(10, sv->FindNextChange(0, sv->Length()));
		CPPUNIT_ASSERT_EQUAL(10, sv->FindNextChange(9, sv->Length()));
		CPPUNIT_ASSERT_EQUAL(11, sv->FindNextChange(10, sv->Length()));
		for (int i=0;i<sv->Length();i++) {
			CPPUNIT_ASSERT_EQUAL(0, sv->ValueAt(i));
		}
	}

	void testFill() {
		sv->InsertSpace(0, 10);
		CPPUNIT_ASSERT_EQUAL(10, sv->Length());
		sv->FillRange(7, 1, 1);	// Should have no effect
		CPPUNIT_ASSERT_EQUAL(10, sv->Length());
		CPPUNIT_ASSERT_EQUAL(10, sv->FindNextChange(-1, sv->Length()));
		CPPUNIT_ASSERT_EQUAL(10, sv->FindNextChange(0, sv->Length()));
		CPPUNIT_ASSERT_EQUAL(10, sv->FindNextChange(7, sv->Length()));
		CPPUNIT_ASSERT_EQUAL(10, sv->FindNextChange(8, sv->Length()));
		CPPUNIT_ASSERT_EQUAL(11, sv->FindNextChange(10, sv->Length()));
		for (int i=0;i<sv->Length();i++) {
			CPPUNIT_ASSERT_EQUAL(0, sv->ValueAt(i));
		}
	}

	void testDeletion() {
		// After deletion
		sv->InsertSpace(0, 10);
		sv->FillRange(7, 1, 1);	// Should have no effect
		sv->DeleteRange(2, 3);	// Should have no effect
		CPPUNIT_ASSERT_EQUAL(7, sv->Length());
		CPPUNIT_ASSERT_EQUAL(7, sv->FindNextChange(-1, sv->Length()));
		CPPUNIT_ASSERT_EQUAL(7, sv->FindNextChange(0, sv->Length()));
		CPPUNIT_ASSERT_EQUAL(7, sv->FindNextChange(4, sv->Length()));
		CPPUNIT_ASSERT_EQUAL(7, sv->FindNextChange(5, sv->Length()));
		CPPUNIT_ASSERT_EQUAL(8, sv->FindNextChange(7, sv->Length()));
		for (int i=0;i<sv->Length();i++) {
			CPPUNIT_ASSERT_EQUAL(0, sv->ValueAt(i));
		}
	}

	void testDeleteAll() {
		sv->InsertSpace(0, 10);
		sv->FillRange(7, 1, 1);	// Should have no effect
		sv->DeleteRange(2, 3);	// Should have no effect
		sv->DeleteAll();
		CPPUNIT_ASSERT_EQUAL(0, sv->Length());
		CPPUNIT_ASSERT_EQUAL(0, sv->FindNextChange(-1, sv->Length()));
		CPPUNIT_ASSERT_EQUAL(1, sv->FindNextChange(0, sv->Length()));
	}
};

class TestIStyles {
	// Generic tests for IStyles that store all styles received
	// that is all normal implementations except SinglePosition.
public:

	IStyles *s;

	void testEmpty() {
		CPPUNIT_ASSERT_EQUAL(0, s->Length());
		CPPUNIT_ASSERT_EQUAL(0, s->FindNextChange(-1, 0));
		CPPUNIT_ASSERT_EQUAL(1, s->FindNextChange(0, 0));
	}

	void testInsertion() {
		// After insertion
		s->InsertSpace(0, 10);
		CPPUNIT_ASSERT_EQUAL(10, s->Length());
		CPPUNIT_ASSERT_EQUAL(10, s->FindNextChange(-1, s->Length()));
		CPPUNIT_ASSERT_EQUAL(10, s->FindNextChange(0, s->Length()));
		CPPUNIT_ASSERT_EQUAL(10, s->FindNextChange(9, s->Length()));
		CPPUNIT_ASSERT_EQUAL(11, s->FindNextChange(10, s->Length()));
		for (int i=0;i<s->Length();i++) {
			CPPUNIT_ASSERT_EQUAL(0, s->ValueAt(i));
		}
	}

	void testFill() {
		s->InsertSpace(0, 10);
		CPPUNIT_ASSERT_EQUAL(10, s->Length());
		s->FillRange(7, 1, 1);
		CPPUNIT_ASSERT_EQUAL(10, s->Length());
		CPPUNIT_ASSERT_EQUAL(7, s->FindNextChange(-1, s->Length()));
		CPPUNIT_ASSERT_EQUAL(7, s->FindNextChange(0, s->Length()));
		CPPUNIT_ASSERT_EQUAL(8, s->FindNextChange(7, s->Length()));
		CPPUNIT_ASSERT_EQUAL(10, s->FindNextChange(8, s->Length()));
		CPPUNIT_ASSERT_EQUAL(10, s->FindNextChange(9, s->Length()));
		CPPUNIT_ASSERT_EQUAL(11, s->FindNextChange(10, s->Length()));
		for (int i=0;i<s->Length();i++) {
			if (i == 7) {
				CPPUNIT_ASSERT_EQUAL(1, s->ValueAt(i));
			} else {
				CPPUNIT_ASSERT_EQUAL(0, s->ValueAt(i));
			}
		}
	}

	void testDeletion() {
		// After deletion
		s->InsertSpace(0, 10);
		s->FillRange(7, 1, 1);
		s->DeleteRange(2, 3);
		CPPUNIT_ASSERT_EQUAL(7, s->Length());
		CPPUNIT_ASSERT_EQUAL(4, s->FindNextChange(-1, s->Length()));
		CPPUNIT_ASSERT_EQUAL(4, s->FindNextChange(0, s->Length()));
		CPPUNIT_ASSERT_EQUAL(5, s->FindNextChange(4, s->Length()));
		CPPUNIT_ASSERT_EQUAL(7, s->FindNextChange(5, s->Length()));
		CPPUNIT_ASSERT_EQUAL(7, s->FindNextChange(6, s->Length()));
		CPPUNIT_ASSERT_EQUAL(8, s->FindNextChange(7, s->Length()));
		for (int i=0;i<s->Length();i++) {
			if (i == 4) {
				CPPUNIT_ASSERT_EQUAL(1, s->ValueAt(i));
			} else {
				CPPUNIT_ASSERT_EQUAL(0, s->ValueAt(i));
			}
		}
	}

	void testDeleteAll() {
		s->InsertSpace(0, 10);
		s->FillRange(7, 1, 1);
		s->DeleteRange(2, 3);
		s->DeleteAll();
		CPPUNIT_ASSERT_EQUAL(0, s->Length());
		CPPUNIT_ASSERT_EQUAL(0, s->FindNextChange(-1, s->Length()));
		CPPUNIT_ASSERT_EQUAL(1, s->FindNextChange(0, s->Length()));
	}
};

class SV1Test : public CppUnit::TestFixture, TestIStyles {
CPPUNIT_TEST_SUITE(SV1Test);
CPPUNIT_TEST(testEmpty);
CPPUNIT_TEST(testInsertion);
CPPUNIT_TEST(testFill);
CPPUNIT_TEST(testDeletion);
CPPUNIT_TEST(testDeleteAll);
CPPUNIT_TEST(testSequence);
CPPUNIT_TEST(testManyInsertions);
CPPUNIT_TEST(testFillRange);
CPPUNIT_TEST(testVarious);
CPPUNIT_TEST_SUITE_END();

	SplitVector_1 *sv;

public:
	void setUp() {
		sv = new SplitVector_1();
		s = sv;
	}

	void tearDown() {
		delete sv;
	}

	void testSequence() {
		sv->Create(20, 100);
		unsigned char test[]="abcde";
		int lenTest = strlen(reinterpret_cast<char *>(test));
		sv->InsertString(0, test, lenTest);
		CPPUNIT_ASSERT_EQUAL(lenTest, sv->Length());
		unsigned char ret[3];
		sv->RetrieveString(1, ret, 3);
		for (int h = 0; h < 3; h++) {
			CPPUNIT_ASSERT(ret[h] == test[h + 1]);
		}
		for (int i = 0; i < lenTest; i++) {
			CPPUNIT_ASSERT_EQUAL(sv->CharAt(i), test[i]);
		}

		sv->DeleteRange(1, 2);
		CPPUNIT_ASSERT_EQUAL(lenTest - 2, sv->Length());
		sv->InsertString(1, shortInsert, 2);
		CPPUNIT_ASSERT_EQUAL(lenTest, sv->Length());
		for (int k = 0; k < lenTest; k++) {
			CPPUNIT_ASSERT_EQUAL(sv->CharAt(k), test[k]);
		}
	}

	void testManyInsertions() {
		sv->Create(20, 100);
		int total = 0;
		unsigned char longtest[]="ghijklmnopqrstuvwxyz";
		sv->InsertString(0, longtest, 1);
		total++;
		for (int l = 0; l < 10000; l++) {
			for (int m = 0; m < 20; m++) {
				sv->InsertString(1, longtest, m + 1);
				total += m + 1;
			}
		}
		CPPUNIT_ASSERT_EQUAL(total, sv->Length());
	}

	void testFillRange() {
		sv->Create(20, 100);
		int total = 0;
		unsigned char longtest[]="ghijklmnopqrstuvwxyz";
		sv->InsertString(0, longtest, 10);
		sv->FillRange(0, 'x', 10);
		total++;
		for (int l = 0; l < 10; l++) {
			CPPUNIT_ASSERT_EQUAL(static_cast<char>(sv->CharAt(l)), 'x');
		}
	}

	void testVarious() {
		char test[]="abcde";
		try {
			sv->InsertString(1, (SW_BYTE *)test, 1);
		} catch (BadPositionException *e) {
			delete e;
		}
		int lenTest = strlen(test);
		sv->InsertString(0, (SW_BYTE *)test, lenTest);
		CPPUNIT_ASSERT_EQUAL(lenTest, sv->Length());
		SW_BYTE ret[3];
		sv->RetrieveString(1, ret, 3);
		for (int h = 0; h < 3; h++) {
			CPPUNIT_ASSERT(ret[h] == test[h + 1]);
		}
		for (int i = 0; i < lenTest; i++) {
			CPPUNIT_ASSERT(test[i] == sv->CharAt(i));
		}
		sv->DeleteRange(1, 2);
		CPPUNIT_ASSERT_EQUAL(lenTest - 2, sv->Length());
		sv->InsertString(1, shortInsert, 2);
		CPPUNIT_ASSERT_EQUAL(lenTest, sv->Length());
		for (int k = 0; k < lenTest; k++) {
			CPPUNIT_ASSERT(test[k] == sv->CharAt(k));
		}
		char longtest[] = "ghijklmnopqrstuvwxyz";
		for (int l = 0; l < 100; l++) {
			for (int m = 0; m < 20; m++) {
				sv->InsertString(1, (SW_BYTE *)longtest, m + 1);
			}
		}
	}
};


class SV2Test : public CppUnit::TestFixture, TestIStyles {
CPPUNIT_TEST_SUITE(SV2Test);
CPPUNIT_TEST(testEmpty);
CPPUNIT_TEST(testInsertion);
CPPUNIT_TEST(testFill);
CPPUNIT_TEST(testDeletion);
CPPUNIT_TEST(testDeleteAll);
CPPUNIT_TEST_SUITE_END();

	SplitVector_2 *sv;

public:
	void setUp() {
		sv = new SplitVector_2();
		s = sv;
	}

	void tearDown() {
		delete sv;
	}
};


class SV4Test : public CppUnit::TestFixture, TestIStyles {
CPPUNIT_TEST_SUITE(SV4Test);
CPPUNIT_TEST(testEmpty);
CPPUNIT_TEST(testInsertion);
CPPUNIT_TEST(testFill);
CPPUNIT_TEST(testDeletion);
CPPUNIT_TEST(testDeleteAll);
CPPUNIT_TEST_SUITE_END();

	SplitVector_4 *sv;

public:
	void setUp() {
		sv = new SplitVector_4();
		s = sv;
	}

	void tearDown() {
		delete sv;
	}
};


class TextPosTest : public CppUnit::TestFixture {
CPPUNIT_TEST_SUITE(TextPosTest);
CPPUNIT_TEST(testCreation);
CPPUNIT_TEST(testAppend);
CPPUNIT_TEST(testFill);
CPPUNIT_TEST(testFillMany);
CPPUNIT_TEST_SUITE_END();

	TextPositions *tp;

public:
	void setUp() {
		tp = new TextPositions();
	}

	void tearDown() {
		delete tp;
	}

	void testCreation() {
		CPPUNIT_ASSERT_EQUAL(1, tp->Last());
		CPPUNIT_ASSERT_EQUAL(0, tp->At(0));
		CPPUNIT_ASSERT_EQUAL(0, tp->LastPosition());
		tp->Add(10);
		CPPUNIT_ASSERT_EQUAL(2, tp->Last());
		CPPUNIT_ASSERT_EQUAL(0, tp->At(0));
		CPPUNIT_ASSERT_EQUAL(10, tp->At(1));
		tp->Add(20);
		CPPUNIT_ASSERT_EQUAL(3, tp->Last());
		CPPUNIT_ASSERT_EQUAL(0, tp->At(0));
		CPPUNIT_ASSERT_EQUAL(10, tp->At(1));
		CPPUNIT_ASSERT_EQUAL(20, tp->At(2));
		CPPUNIT_ASSERT_EQUAL(15, tp->AtMiddle(1));
		CPPUNIT_ASSERT_EQUAL(20, tp->LastPosition());
	}

	void testAppend() {
		tp->Add(10);
		tp->Add(20);
		TextPositions *tp2 = new TextPositions(*tp);
		tp2->Add(30);
		tp->Append(tp2);
		CPPUNIT_ASSERT_EQUAL(6, tp->Last());
		CPPUNIT_ASSERT_EQUAL(0, tp->At(0));
		CPPUNIT_ASSERT_EQUAL(10, tp->At(1));
		CPPUNIT_ASSERT_EQUAL(20, tp->At(2));
		CPPUNIT_ASSERT_EQUAL(30, tp->At(3));
		CPPUNIT_ASSERT_EQUAL(40, tp->At(4));
		CPPUNIT_ASSERT_EQUAL(50, tp->At(5));
		delete tp2;
	}

	void testFill() {
		tp->Add(10);
		tp->FillTo(5);
		CPPUNIT_ASSERT_EQUAL(5, tp->Last());
		CPPUNIT_ASSERT_EQUAL(0, tp->At(0));
		CPPUNIT_ASSERT_EQUAL(10, tp->At(1));
		CPPUNIT_ASSERT_EQUAL(10, tp->At(2));
		CPPUNIT_ASSERT_EQUAL(10, tp->At(3));
		CPPUNIT_ASSERT_EQUAL(10, tp->At(4));
	}

	void testFillMany() {
		tp->Add(10);
		// More than fit in initial allocation
		tp->FillTo(85);
		CPPUNIT_ASSERT_EQUAL(85, tp->Last());
		CPPUNIT_ASSERT_EQUAL(10, tp->LastPosition());
	}
};

// Length in characters of a UTF32 string
static int xcslen(int *a) {
	int i = 0;
	while (a[i])
		i++;
	return i;
}

// Comparison function for two UTF32 strings
static int xcscmp(int *a, int *b) {
	while (*a && *b) {
		if (*a != *b) {
			return *a - *b;
		}
		a++;
		b++;
	}
	// Either *a or *b is 0
	return *a - *b;
}

class UniConvTest : public CppUnit::TestFixture {
CPPUNIT_TEST_SUITE(UniConvTest);
CPPUNIT_TEST(testByteLength);
CPPUNIT_TEST(testLength);
CPPUNIT_TEST(testValidate);
CPPUNIT_TEST(test8From16);
CPPUNIT_TEST(test16From8);
CPPUNIT_TEST(test8From32);
CPPUNIT_TEST(test32From8);
CPPUNIT_TEST(test16From32);
CPPUNIT_TEST(test32From16);
CPPUNIT_TEST(testnonBMP);
CPPUNIT_TEST(testSurrogates);
CPPUNIT_TEST_SUITE_END();

public:
	// The unicode string used here has 7 characters:
	// CJK UNIFIED IDEOGRAPH-66E8, HEBREW LETTER SHIN, HIRAGANA LETTER N,
	// LATIN SMALL LETTER T, CYRILLIC SMALL LETTER DE,
	// GREEK CAPITAL LETTER GAMMA, LATIN SMALL LETTER SHARP S
	// It requires 15 bytes in UTF-8 including 1, 2, and 3 byte characters.

	void testByteLength() {
		CPPUNIT_ASSERT_EQUAL(1, UniConversion::UTF8ByteLength(0x40));
		CPPUNIT_ASSERT_EQUAL(2, UniConversion::UTF8ByteLength(0x90));
		CPPUNIT_ASSERT_EQUAL(3, UniConversion::UTF8ByteLength(0xE0));
		CPPUNIT_ASSERT_EQUAL(4, UniConversion::UTF8ByteLength(0xF0));
	}

	void testLength() {
		wchar_t example[] = L"abc";
		CPPUNIT_ASSERT_EQUAL(3, UniConversion::UTF8LengthFromUTF16(
			(SW_SHORT *)example, wcslen(example)));
		wchar_t ex2[] = L"\u66e8\u05e9\u3093t\u0434\u0393\xdf";
		CPPUNIT_ASSERT_EQUAL(15, UniConversion::UTF8LengthFromUTF16(
			(SW_SHORT *)ex2, wcslen(ex2)));
		CPPUNIT_ASSERT_EQUAL(7, UniConversion::UTF32LengthFromUTF16(
			(SW_SHORT *)ex2, wcslen(ex2)));

		char ex3[] = "\xe6\x9b\xa8\xd7\xa9\xe3\x82\x93\x74\xd0\xb4\xce\x93\xc3\x9f";
		CPPUNIT_ASSERT_EQUAL(7, UniConversion::UTF16LengthFromUTF8(
			(SW_BYTE *)ex3, strlen(ex3)));
		CPPUNIT_ASSERT_EQUAL(7, UniConversion::UTF32LengthFromUTF8(
			(SW_BYTE *)ex3, strlen(ex3)));

		int ex4[] = { 0x66e8, 0x05e9, 0x3093, 't', 0x0434, 0x0393, 0xdf, 0};
		CPPUNIT_ASSERT_EQUAL(15, UniConversion::UTF8LengthFromUTF32(
			ex4, xcslen(ex4)));
		CPPUNIT_ASSERT_EQUAL(7, UniConversion::UTF16LengthFromUTF32(
			ex4, xcslen(ex4)));
	}

	bool validateStr(char *s) {
		return UniConversion::UTF8Validate((SW_BYTE *)s, strlen(s));
	}

	void testValidate() {
		char example[] = "abc";
		CPPUNIT_ASSERT(validateStr(example));
		char ex2[] = "\xe6\x9b\xa8\xd7\xa9\xe3\x82\x93\x74\xd0\xb4\xce\x93\xc3\x9f";
		CPPUNIT_ASSERT(validateStr(ex2));
		// Bad byte 0xff
		CPPUNIT_ASSERT(!validateStr("\xff"));
		// Bad byte 0xc0
		CPPUNIT_ASSERT(!validateStr("\xc0"));
		// Bad 2nd byte is start byte
		CPPUNIT_ASSERT(!validateStr("\xe6\xeb\x65"));
		// Short sequence
		CPPUNIT_ASSERT(!validateStr("\xf4\x8f\xbf"));
	}

	void test8From16() {
		wchar_t example[] = L"abc";
		SW_BYTE converted[20];
		int len = UniConversion::UTF8FromUTF16(converted, sizeof(converted),
			(SW_SHORT *)example, wcslen(example));
		CPPUNIT_ASSERT_EQUAL(3, len);
		converted[len] = '\0';
		CPPUNIT_ASSERT_EQUAL(0, strcmp((char *)converted, "abc"));
		wchar_t ex2[] = L"\u66e8\u05e9\u3093t\u0434\u0393\xdf";
		char conv2[] = "\xe6\x9b\xa8\xd7\xa9\xe3\x82\x93\x74\xd0\xb4\xce\x93\xc3\x9f";
		len = UniConversion::UTF8FromUTF16(converted, sizeof(converted),
			(SW_SHORT *)ex2, wcslen(ex2));
		CPPUNIT_ASSERT_EQUAL(15, len);
		converted[len] = '\0';
		CPPUNIT_ASSERT_EQUAL(0, strcmp((char *)converted, conv2));
	}

	void test16From8() {
		char example[] = "abc";
		wchar_t converted[20];
		int len = UniConversion::UTF16FromUTF8(
			(SW_SHORT *)converted, sizeof(converted)/sizeof(converted[0]),
			(SW_BYTE *)example, strlen(example));
		CPPUNIT_ASSERT_EQUAL(3, len);
		converted[len] = 0;
		CPPUNIT_ASSERT_EQUAL(0, wcscmp(converted, L"abc"));
		char ex2[] = "\xe6\x9b\xa8\xd7\xa9\xe3\x82\x93\x74\xd0\xb4\xce\x93\xc3\x9f";
		wchar_t uex2[] = L"\u66e8\u05e9\u3093t\u0434\u0393\xdf";
		len = UniConversion::UTF16FromUTF8(
			(SW_SHORT *)converted, sizeof(converted)/sizeof(converted[0]),
			(SW_BYTE *)ex2, strlen(ex2));
		CPPUNIT_ASSERT_EQUAL(7, len);
		converted[len] = 0;
		CPPUNIT_ASSERT_EQUAL(0, wcscmp(converted, uex2));
	}

	void test8From32() {
		int example[] = { 'a', 'b', 'c', 0};
		SW_BYTE converted[20];
		CPPUNIT_ASSERT_EQUAL(3, UniConversion::UTF8LengthFromUTF32(example, 3));
		int len = UniConversion::UTF8FromUTF32(converted, sizeof(converted),
			(int *)example, xcslen(example));
		CPPUNIT_ASSERT_EQUAL(3, len);
		converted[len] = '\0';
		CPPUNIT_ASSERT_EQUAL(0, strcmp((char *)converted, "abc"));
		int ex2[] = { 0x66e8, 0x05e9, 0x3093, 't', 0x0434, 0x0393, 0xdf, 0};
		char conv2[] = "\xe6\x9b\xa8\xd7\xa9\xe3\x82\x93\x74\xd0\xb4\xce\x93\xc3\x9f";
		CPPUNIT_ASSERT_EQUAL(15, UniConversion::UTF8LengthFromUTF32(ex2, xcslen(ex2)));
		len = UniConversion::UTF8FromUTF32(converted, sizeof(converted),
			(int *)ex2, xcslen(ex2));
		CPPUNIT_ASSERT_EQUAL(15, len);
		converted[len] = '\0';
		CPPUNIT_ASSERT_EQUAL(0, strcmp((char *)converted, conv2));
	}

	void test32From8() {
		char example[] = "abc";
		int xexample[] = { 'a', 'b', 'c', 0};
		int converted[20];
		int len = UniConversion::UTF32FromUTF8(
			(int *)converted, sizeof(converted)/sizeof(converted[0]),
			(SW_BYTE *)example, strlen(example));
		CPPUNIT_ASSERT_EQUAL(3, len);
		converted[len] = 0;
		CPPUNIT_ASSERT_EQUAL(0, xcscmp(converted, xexample));
		char ex2[] = "\xe6\x9b\xa8\xd7\xa9\xe3\x82\x93\x74\xd0\xb4\xce\x93\xc3\x9f";
		int uex2[] = { 0x66e8, 0x05e9, 0x3093, 't', 0x0434, 0x0393, 0xdf, 0};
		len = UniConversion::UTF32FromUTF8(
			(int *)converted, sizeof(converted)/sizeof(converted[0]),
			(SW_BYTE *)ex2, strlen(ex2));
		CPPUNIT_ASSERT_EQUAL(7, len);
		converted[len] = 0;
		CPPUNIT_ASSERT_EQUAL(0, xcscmp(converted, uex2));
	}

	void test16From32() {
		int example[] = { 'a', 'b', 'c', 0};
		SW_SHORT converted[20];
		CPPUNIT_ASSERT_EQUAL(3, UniConversion::UTF16LengthFromUTF32(example, 3));
		int len = UniConversion::UTF16FromUTF32(converted, sizeof(converted),
			(int *)example, xcslen(example));
		CPPUNIT_ASSERT_EQUAL(3, len);
		converted[len] = '\0';
		CPPUNIT_ASSERT_EQUAL(0, wcscmp((wchar_t *)converted, L"abc"));
		int ex2[] = { 0x66e8, 0x05e9, 0x3093, 't', 0x0434, 0x0393, 0xdf, 0};
		SW_SHORT *conv2 = (SW_SHORT *)(void *)L"\x66e8\x05e9\x3093t\x0434\x0393\xdf";
		CPPUNIT_ASSERT_EQUAL(7, UniConversion::UTF16LengthFromUTF32(ex2, xcslen(ex2)));
		len = UniConversion::UTF16FromUTF32(converted, sizeof(converted),
			(int *)ex2, xcslen(ex2));
		CPPUNIT_ASSERT_EQUAL(7, len);
		converted[len] = '\0';
		CPPUNIT_ASSERT_EQUAL(0, wcscmp((wchar_t *)converted, (wchar_t *)conv2));
	}

	void test32From16() {
		SW_SHORT *example = (SW_SHORT *)(void *)L"abc";
		int xexample[] = { 'a', 'b', 'c', 0};
		int converted[20];
		int len = UniConversion::UTF32FromUTF16(
			(int *)converted, sizeof(converted)/sizeof(converted[0]),
			(SW_SHORT *)example, wcslen((wchar_t *)example));
		CPPUNIT_ASSERT_EQUAL(3, len);
		converted[len] = 0;
		CPPUNIT_ASSERT_EQUAL(0, xcscmp(converted, xexample));
		SW_SHORT *ex2 = (SW_SHORT *)(void *)L"\x66e8\x05e9\x3093t\x0434\x0393\xdf";
		int uex2[] = { 0x66e8, 0x05e9, 0x3093, 't', 0x0434, 0x0393, 0xdf, 0};
		len = UniConversion::UTF32FromUTF16(
			(int *)converted, sizeof(converted)/sizeof(converted[0]),
			(SW_SHORT *)ex2, wcslen((wchar_t *)ex2));
		CPPUNIT_ASSERT_EQUAL(7, len);
		converted[len] = 0;
		CPPUNIT_ASSERT_EQUAL(0, xcscmp(converted, uex2));
	}

	void testnonBMP() {
		// Using sup. PU as a test
		int example[] = { 0x10FFFD, 0};
		char expected[] = "\xf4\x8f\xbf\xbd";
		char converted[20];
		int len = UniConversion::UTF8FromUTF32((SW_BYTE *)converted, sizeof(converted),
			example, xcslen(example));
		CPPUNIT_ASSERT_EQUAL(4, len);
		converted[len] = '\0';
		CPPUNIT_ASSERT(strcmp(converted, expected)== 0);
		int convBack[4];
		len = UniConversion::UTF32FromUTF8(
			convBack, sizeof(convBack)/sizeof(convBack[0]),
			(SW_BYTE *)converted, strlen(converted));
		CPPUNIT_ASSERT_EQUAL(1, len);
		convBack[len] = 0;
		CPPUNIT_ASSERT(xcscmp(example, convBack) == 0);

		wchar_t wcs[20];
		len = UniConversion::UTF16FromUTF8(
			(SW_SHORT *)wcs, sizeof(wcs)/sizeof(wcs[0]),
			(SW_BYTE *)expected, strlen(expected));
		CPPUNIT_ASSERT_EQUAL(2, len);
		wcs[len] = 0;
		wchar_t expw[] = L"\xDBFF\xDFFD";
		CPPUNIT_ASSERT(wcscmp(expw, wcs) == 0);
	}

	void testSurrogates() {
		// Using sup. PU as a test
		int example[] = { 0x10FFFD, 0};
		wchar_t expected[] = L"\xDBFF\xDFFD";
		SW_SHORT converted[20];
		int len = UniConversion::UTF16FromUTF32(
			(SW_SHORT *)converted, sizeof(converted)/sizeof(converted[0]),
			example, xcslen(example));
		CPPUNIT_ASSERT_EQUAL(2, len);
		converted[len] = 0;
		CPPUNIT_ASSERT(wcscmp((wchar_t *)converted, expected)== 0);
		int convBack[4];
		len = UniConversion::UTF32FromUTF16(
			convBack, sizeof(convBack)/sizeof(convBack[0]),
			(SW_SHORT *)converted, len);
		CPPUNIT_ASSERT_EQUAL(1, len);
		convBack[len] = 0;
		CPPUNIT_ASSERT(xcscmp(example, convBack) == 0);
	}
};


class ByteBufferTest : public CppUnit::TestFixture {
CPPUNIT_TEST_SUITE(ByteBufferTest);
CPPUNIT_TEST(testBB);
CPPUNIT_TEST(testBig);
CPPUNIT_TEST_SUITE_END();

	ByteBuffer *bb;

public:
	void setUp() {
		bb = new ByteBuffer();
	}

	void tearDown() {
		delete bb;
	}

	void testBB() {
		// Will start pointing at the small buffer
		CPPUNIT_ASSERT(bb->Bytes() != NULL);
		SW_BYTE *smallBuffer = bb->Bytes();
		bb->Set("Frog");
		CPPUNIT_ASSERT(strcmp((char *)bb->Bytes(), "Frog") == 0);
		CPPUNIT_ASSERT_EQUAL(smallBuffer, bb->Bytes());
		CPPUNIT_ASSERT_EQUAL(4, bb->Length());
	}

	void testBig() {
		// Will start pointing at the small buffer
		SW_BYTE *smallBuffer = bb->Bytes();
		bb->Set("A text that is longer than 32 bytes so it will move.");
		CPPUNIT_ASSERT(smallBuffer != bb->Bytes());
		CPPUNIT_ASSERT(bb->Length() > 32);
	}
};


class SubstUTF8Test : public CppUnit::TestFixture {
CPPUNIT_TEST_SUITE(SubstUTF8Test);
CPPUNIT_TEST(testCrLf);
CPPUNIT_TEST(testUTF);
CPPUNIT_TEST(testUnit);
CPPUNIT_TEST(testMove);
CPPUNIT_TEST(testUTFRetrieval);
CPPUNIT_TEST_SUITE_END();

	SubstanceUTF8 *su;

public:
	void setUp() {
		su = new SubstanceUTF8();
		su->Create(20, 100);
	}

	void tearDown() {
		delete su;
	}

	void testCrLf() {
		CPPUNIT_ASSERT(!su->IsCrLf(0));
		unsigned char crlf[]="\r\n";
		su->InsertString(0, crlf, 2);
		CPPUNIT_ASSERT(su->IsCrLf(0));
		CPPUNIT_ASSERT_EQUAL(2, su->DistanceNextCaret(0));
	}

	void testUTF() {
		// CJK UNIFIED IDEOGRAPH-66E8 (3), HEBREW LETTER SHIN (2), LATIN SMALL LETTER T (1)
		unsigned char utf[]="\xe6\x9b\xa8\xd7\xa9t";
		su->InsertString(0, utf, strlen((char *)utf));
		for (int i=0; i<su->Length();i++) {
			CPPUNIT_ASSERT(!su->IsCrLf(i));
		}
		CPPUNIT_ASSERT_EQUAL(3, su->LenChar(0));
		CPPUNIT_ASSERT_EQUAL(2, su->LenChar(3));
		CPPUNIT_ASSERT_EQUAL(1, su->LenChar(5));
		CPPUNIT_ASSERT_EQUAL(0x66e8, su->CharValue(0));
		CPPUNIT_ASSERT_EQUAL(0x5e9, su->CharValue(3));
		CPPUNIT_ASSERT_EQUAL(0x74, su->CharValue(5));
	}

	void testUnit() {
		// CJK UNIFIED IDEOGRAPH-66E8 (3), HEBREW LETTER SHIN (2), LATIN SMALL LETTER T (1)
		unsigned char utf[]="\xe6\x9b\xa8\xd7\xa9t";
		su->InsertString(0, utf, strlen((char *)utf));
		for (int i=0; i<su->Length();i++) {
			CPPUNIT_ASSERT_EQUAL((int)utf[i], su->UnitValue(i));
		}
		for (int i=0; i<su->Length();i++) {
			CPPUNIT_ASSERT_EQUAL((int)utf[i], su->UnitFromArray(utf, i));
		}
	}

	void testMove() {
		// CJK UNIFIED IDEOGRAPH-66E8 (3), HEBREW LETTER SHIN (2), LATIN SMALL LETTER T (1)
		unsigned char utf[]="\xe6\x9b\xa8\xd7\xa9\r\nt";
		su->InsertString(0, utf, strlen((char *)utf));
		CPPUNIT_ASSERT_EQUAL(0, su->MovePositionOutsideChar(0, -1, false));
		CPPUNIT_ASSERT_EQUAL(0, su->MovePositionOutsideChar(0, 1, false));
		CPPUNIT_ASSERT_EQUAL(0, su->MovePositionOutsideChar(1, -1, false));
		CPPUNIT_ASSERT_EQUAL(3, su->MovePositionOutsideChar(1, 1, false));
		CPPUNIT_ASSERT_EQUAL(0, su->MovePositionOutsideChar(2, -1, false));
		CPPUNIT_ASSERT_EQUAL(3, su->MovePositionOutsideChar(2, 1, false));
		CPPUNIT_ASSERT_EQUAL(3, su->MovePositionOutsideChar(3, -1, false));

		CPPUNIT_ASSERT_EQUAL(3, su->MovePositionOutsideChar(3, 1, false));
		CPPUNIT_ASSERT_EQUAL(3, su->MovePositionOutsideChar(4, -1, false));
		CPPUNIT_ASSERT_EQUAL(5, su->MovePositionOutsideChar(4, 1, false));
		CPPUNIT_ASSERT_EQUAL(5, su->MovePositionOutsideChar(5, -1, false));

		CPPUNIT_ASSERT_EQUAL(5, su->MovePositionOutsideChar(5, 1, false));
		CPPUNIT_ASSERT_EQUAL(6, su->MovePositionOutsideChar(6, -1, false));
		CPPUNIT_ASSERT_EQUAL(6, su->MovePositionOutsideChar(6, 1, false));
		CPPUNIT_ASSERT_EQUAL(7, su->MovePositionOutsideChar(7, -1, false));
		CPPUNIT_ASSERT_EQUAL(7, su->MovePositionOutsideChar(7, 1, false));
		CPPUNIT_ASSERT_EQUAL(8, su->MovePositionOutsideChar(8, -1, false));
		CPPUNIT_ASSERT_EQUAL(8, su->MovePositionOutsideChar(8, 1, false));

		// Try with line end detection on: should move away from position 6 between cr and lf
		CPPUNIT_ASSERT_EQUAL(5, su->MovePositionOutsideChar(5, 1, true));
		CPPUNIT_ASSERT_EQUAL(5, su->MovePositionOutsideChar(6, -1, true));
		CPPUNIT_ASSERT_EQUAL(7, su->MovePositionOutsideChar(6, 1, true));
		CPPUNIT_ASSERT_EQUAL(7, su->MovePositionOutsideChar(7, -1, true));
		CPPUNIT_ASSERT_EQUAL(7, su->MovePositionOutsideChar(7, 1, true));
		CPPUNIT_ASSERT_EQUAL(8, su->MovePositionOutsideChar(8, -1, true));
		CPPUNIT_ASSERT_EQUAL(8, su->MovePositionOutsideChar(8, 1, true));
	}

	void testUTFRetrieval() {
		unsigned char utf[]="\xe6\x9b\xa8\xd7\xa9t\xf4\x8f\xbf\xbd";
		su->InsertString(0, utf, strlen((char *)utf));
		char c[20];
		int len = su->RetrieveUTF8(0, (SW_BYTE *)c, su->Length());
		CPPUNIT_ASSERT_EQUAL(10, len);
		CPPUNIT_ASSERT(memcmp(utf, c, len) == 0);
		wchar_t w[20];
		wchar_t ucs[]=L"\u66e8\u05e9\x74\xDBFF\xDFFD";
		len = su->RetrieveUTF16(0, (SW_SHORT *)w, su->Length());
		CPPUNIT_ASSERT_EQUAL(5, len);
		CPPUNIT_ASSERT(memcmp(ucs, w, len*2) == 0);
		int x[20];
		int xcs[]= { 0x66e8, 0x05e9, 0x74,  0x10FFFD};
		len = su->RetrieveUTF32(0, x, su->Length());
		CPPUNIT_ASSERT_EQUAL(4, len);
		CPPUNIT_ASSERT(memcmp(xcs, x, len*4) == 0);
	}

};


class SubstByteTest : public CppUnit::TestFixture {
CPPUNIT_TEST_SUITE(SubstByteTest);
CPPUNIT_TEST(testCrLf);
CPPUNIT_TEST(testString);
CPPUNIT_TEST(testUnit);
CPPUNIT_TEST(testMove);
CPPUNIT_TEST(testUTFRetrieval);
CPPUNIT_TEST_SUITE_END();

	SubstanceByte *su;

public:
	void setUp() {
		su = new SubstanceByte();
		su->Create(20, 100);
	}

	void tearDown() {
		delete su;
	}

	void testCrLf() {
		CPPUNIT_ASSERT(!su->IsCrLf(0));
		unsigned char crlf[]="\r\n";
		su->InsertString(0, crlf, 2);
		CPPUNIT_ASSERT(su->IsCrLf(0));
		CPPUNIT_ASSERT_EQUAL(2, su->DistanceNextCaret(0));
	}

	void testString() {
		unsigned char test[]="August";
		su->InsertString(0, test, strlen((char *)test));
		for (int i=0; i<su->Length();i++) {
			CPPUNIT_ASSERT(!su->IsCrLf(i));
		}
		for (int i=0; i<su->Length();i++) {
			CPPUNIT_ASSERT_EQUAL(1, su->LenChar(i));
		}
	}

	void testUnit() {
		unsigned char test[]="August";
		su->InsertString(0, test, strlen((char *)test));
		for (int i=0; i<su->Length();i++) {
			CPPUNIT_ASSERT_EQUAL((int)test[i], su->UnitValue(i));
		}
		for (int i=0; i<su->Length();i++) {
			CPPUNIT_ASSERT_EQUAL((int)test[i], su->UnitFromArray(test, i));
		}
	}

	void testMove() {
		unsigned char utf[]="\xe6\x9b\xa8\xd7\xa9\r\nt";
		su->InsertString(0, utf, strlen((char *)utf));
		for (int i=0; i<=su->Length();i++) {
			CPPUNIT_ASSERT_EQUAL(i, su->MovePositionOutsideChar(i, -1, false));
			CPPUNIT_ASSERT_EQUAL(i, su->MovePositionOutsideChar(i, 1, false));
		}

		// Try with line end detection on: should move away from position 6 between cr and lf
		for (int i=0; i<=su->Length();i++) {
			if (i == 6) {
				CPPUNIT_ASSERT_EQUAL(i-1, su->MovePositionOutsideChar(i, -1, true));
				CPPUNIT_ASSERT_EQUAL(i+1, su->MovePositionOutsideChar(i, 1, true));
			} else {
				CPPUNIT_ASSERT_EQUAL(i, su->MovePositionOutsideChar(i, -1, true));
				CPPUNIT_ASSERT_EQUAL(i, su->MovePositionOutsideChar(i, 1, true));
			}
		}
	}

	void testUTFRetrieval() {
		unsigned char utf[]="\xe6\x9b\xa8\xd7\xa9t\xf4\x8f\xbf\xbd";
		su->InsertString(0, utf, strlen((char *)utf));
		char c[20];
		int len = su->RetrieveUTF8(0, (SW_BYTE *)c, su->Length());
		CPPUNIT_ASSERT_EQUAL(10, len);
		CPPUNIT_ASSERT(memcmp(utf, c, len) == 0);
		wchar_t w[20];
		wchar_t ucs[]=L"\xe6\x9b\xa8\xd7\xa9t\xf4\x8f\xbf\xbd";
		len = su->RetrieveUTF16(0, (SW_SHORT *)w, su->Length());
		CPPUNIT_ASSERT_EQUAL(10, len);
		CPPUNIT_ASSERT(memcmp(ucs, w, len*2) == 0);
		int x[20];
		int xcs[]= { 0xe6, 0x9b, 0xa8, 0xd7, 0xa9, 't', 0xf4, 0x8f, 0xbf, 0xbd};
		len = su->RetrieveUTF32(0, x, su->Length());
		CPPUNIT_ASSERT_EQUAL(10, len);
		CPPUNIT_ASSERT(memcmp(xcs, x, len*4) == 0);
	}
};


class SubstUCSLETest : public CppUnit::TestFixture {
CPPUNIT_TEST_SUITE(SubstUCSLETest);
CPPUNIT_TEST(testCrLf);
CPPUNIT_TEST(testUCS);
CPPUNIT_TEST(testUnit);
CPPUNIT_TEST(testMove);
CPPUNIT_TEST(testUTFRetrieval);
CPPUNIT_TEST_SUITE_END();

	SubstanceUTF16LE *su;

public:
	void setUp() {
		su = new SubstanceUTF16LE();
		su->Create(20, 100);
	}

	void tearDown() {
		delete su;
	}

	void testCrLf() {
		CPPUNIT_ASSERT(!su->IsCrLf(0));
		wchar_t crlf[]=L"\x0d\x0a";
		su->InsertString(0, (SW_BYTE *)crlf, 4);
		CPPUNIT_ASSERT(su->IsCrLf(0));
		CPPUNIT_ASSERT_EQUAL(4, su->DistanceNextCaret(0));
	}

	void testUCS() {
		// CJK UNIFIED IDEOGRAPH-66E8 (3), HEBREW LETTER SHIN (2), LATIN SMALL LETTER T (1)
		wchar_t ucs[]=L"\u66e8\u05e9\x74";
		su->InsertString(0, (SW_BYTE *)ucs, 2*wcslen(ucs));
		for (int i=0; i<su->Length();i++) {
			CPPUNIT_ASSERT(!su->IsCrLf(i));
		}
		CPPUNIT_ASSERT_EQUAL(2, su->LenChar(0));
		CPPUNIT_ASSERT_EQUAL(2, su->LenChar(2));
		CPPUNIT_ASSERT_EQUAL(2, su->LenChar(4));
		CPPUNIT_ASSERT_EQUAL(0x66e8, su->CharValue(0));
		CPPUNIT_ASSERT_EQUAL(0x5e9, su->CharValue(2));
		CPPUNIT_ASSERT_EQUAL(0x74, su->CharValue(4));
	}

	void testUnit() {
		// CJK UNIFIED IDEOGRAPH-66E8 (3), HEBREW LETTER SHIN (2), LATIN SMALL LETTER T (1)
		wchar_t ucs[]=L"\u66e8\u05e9\x74";
		su->InsertString(0, (SW_BYTE *)ucs, 2*wcslen(ucs));
		for (int i=0; i<su->Length()/2;i++) {
			CPPUNIT_ASSERT_EQUAL((int)ucs[i], su->UnitValue(i*2));
		}
		for (int i=0; i<su->Length()/2;i++) {
			CPPUNIT_ASSERT_EQUAL((int)ucs[i], su->UnitFromArray((SW_BYTE *)ucs, i*2));
		}
	}

	void testMove() {
		// CJK UNIFIED IDEOGRAPH-66E8 (3), HEBREW LETTER SHIN (2), LATIN SMALL LETTER T (1)
		wchar_t ucs[]=L"\u66e8\u05e9\x0d\x0a\x74";
		su->InsertString(0, (SW_BYTE *)ucs, 2*wcslen(ucs));
		CPPUNIT_ASSERT_EQUAL(0, su->MovePositionOutsideChar(0, -1, false));
		CPPUNIT_ASSERT_EQUAL(0, su->MovePositionOutsideChar(0, 1, false));
		CPPUNIT_ASSERT_EQUAL(0, su->MovePositionOutsideChar(1, -1, false));
		CPPUNIT_ASSERT_EQUAL(2, su->MovePositionOutsideChar(1, 1, false));
		CPPUNIT_ASSERT_EQUAL(2, su->MovePositionOutsideChar(2, -1, false));
		CPPUNIT_ASSERT_EQUAL(2, su->MovePositionOutsideChar(2, 1, false));
		CPPUNIT_ASSERT_EQUAL(2, su->MovePositionOutsideChar(3, -1, false));
		CPPUNIT_ASSERT_EQUAL(4, su->MovePositionOutsideChar(3, 1, false));
		CPPUNIT_ASSERT_EQUAL(4, su->MovePositionOutsideChar(4, -1, false));
		CPPUNIT_ASSERT_EQUAL(4, su->MovePositionOutsideChar(4, 1, false));
		CPPUNIT_ASSERT_EQUAL(4, su->MovePositionOutsideChar(5, -1, false));
		CPPUNIT_ASSERT_EQUAL(6, su->MovePositionOutsideChar(5, 1, false));
		CPPUNIT_ASSERT_EQUAL(6, su->MovePositionOutsideChar(6, -1, false));
		CPPUNIT_ASSERT_EQUAL(6, su->MovePositionOutsideChar(6, 1, false));
		CPPUNIT_ASSERT_EQUAL(6, su->MovePositionOutsideChar(7, -1, false));
		CPPUNIT_ASSERT_EQUAL(8, su->MovePositionOutsideChar(7, 1, false));
		CPPUNIT_ASSERT_EQUAL(8, su->MovePositionOutsideChar(8, -1, false));
		CPPUNIT_ASSERT_EQUAL(8, su->MovePositionOutsideChar(8, 1, false));
		CPPUNIT_ASSERT_EQUAL(8, su->MovePositionOutsideChar(9, -1, false));
		CPPUNIT_ASSERT_EQUAL(10, su->MovePositionOutsideChar(9, 1, false));
		CPPUNIT_ASSERT_EQUAL(10, su->MovePositionOutsideChar(10, -1, false));
		CPPUNIT_ASSERT_EQUAL(10, su->MovePositionOutsideChar(10, 1, false));

		// Try with line end detection on: should move away from position  between cr and lf
		CPPUNIT_ASSERT_EQUAL(4, su->MovePositionOutsideChar(5, -1, true));
		CPPUNIT_ASSERT_EQUAL(8, su->MovePositionOutsideChar(5, 1, true));
		CPPUNIT_ASSERT_EQUAL(4, su->MovePositionOutsideChar(6, -1, true));
		CPPUNIT_ASSERT_EQUAL(8, su->MovePositionOutsideChar(6, 1, true));
		CPPUNIT_ASSERT_EQUAL(4, su->MovePositionOutsideChar(7, -1, true));
		CPPUNIT_ASSERT_EQUAL(8, su->MovePositionOutsideChar(7, 1, true));
		CPPUNIT_ASSERT_EQUAL(8, su->MovePositionOutsideChar(8, -1, true));
		CPPUNIT_ASSERT_EQUAL(8, su->MovePositionOutsideChar(8, 1, true));
	}

	void testUTFRetrieval() {
		char ucsBytes[]="\xe8\x66\xe9\x05\x74\x00\xFF\xDB\xFD\xDF";
		su->InsertString(0, (SW_BYTE *)ucsBytes, sizeof(ucsBytes)-1);

		unsigned char utf[]="\xe6\x9b\xa8\xd7\xa9t\xf4\x8f\xbf\xbd";
		char c[20];
		int len = su->RetrieveUTF8(0, (SW_BYTE *)c, su->Length());
		CPPUNIT_ASSERT_EQUAL(10, len);
		CPPUNIT_ASSERT(memcmp(utf, c, len) == 0);
		wchar_t w[20];
		wchar_t ucs[]=L"\u66e8\u05e9\x74\xDBFF\xDFFD";
		len = su->RetrieveUTF16(0, (SW_SHORT *)w, su->Length());
		CPPUNIT_ASSERT_EQUAL(5, len);
		CPPUNIT_ASSERT(memcmp(ucs, w, len*2) == 0);
		int x[20];
		int xcs[]= { 0x66e8, 0x05e9, 0x74,  0x10FFFD};
		len = su->RetrieveUTF32(0, x, su->Length());
		CPPUNIT_ASSERT_EQUAL(4, len);
		CPPUNIT_ASSERT(memcmp(xcs, x, len*4) == 0);
	}
};


class SubstUCSBETest : public CppUnit::TestFixture {
CPPUNIT_TEST_SUITE(SubstUCSBETest);
CPPUNIT_TEST(testCrLf);
CPPUNIT_TEST(testUCS);
CPPUNIT_TEST(testUnit);
CPPUNIT_TEST(testMove);
CPPUNIT_TEST(testUTFRetrieval);
CPPUNIT_TEST_SUITE_END();

	SubstanceUTF16BE *su;

public:
	void setUp() {
		su = new SubstanceUTF16BE();
		su->Create(20, 100);
	}

	void tearDown() {
		delete su;
	}

	void testCrLf() {
		CPPUNIT_ASSERT(!su->IsCrLf(0));
		wchar_t crlf[]=L"\x0d00\x0a00";
		su->InsertString(0, (SW_BYTE *)crlf, 4);
		CPPUNIT_ASSERT(su->IsCrLf(0));
		CPPUNIT_ASSERT_EQUAL(4, su->DistanceNextCaret(0));
	}

	void testUCS() {
		// CJK UNIFIED IDEOGRAPH-66E8 (3), HEBREW LETTER SHIN (2), LATIN SMALL LETTER T (1)
		wchar_t ucs[]=L"\ue866\ue905\x7400";
		su->InsertString(0, (SW_BYTE *)ucs, 2*wcslen(ucs));
		for (int i=0; i<su->Length();i++) {
			CPPUNIT_ASSERT(!su->IsCrLf(i));
		}
		CPPUNIT_ASSERT_EQUAL(2, su->LenChar(0));
		CPPUNIT_ASSERT_EQUAL(2, su->LenChar(2));
		CPPUNIT_ASSERT_EQUAL(2, su->LenChar(4));
		CPPUNIT_ASSERT_EQUAL(0x66e8, su->CharValue(0));
		CPPUNIT_ASSERT_EQUAL(0x5e9, su->CharValue(2));
		CPPUNIT_ASSERT_EQUAL(0x74, su->CharValue(4));
	}

	void testUnit() {
		// CJK UNIFIED IDEOGRAPH-66E8 (3), HEBREW LETTER SHIN (2), LATIN SMALL LETTER T (1)
		wchar_t ucs[]=L"\ue866\ue905\x7400";
		wchar_t ucsSwapped[]=L"\u66e8\u05e9\x0074";
		su->InsertString(0, (SW_BYTE *)ucs, 2*wcslen(ucs));
		for (int i=0; i<su->Length()/2;i++) {
			CPPUNIT_ASSERT_EQUAL((int)ucsSwapped[i], su->UnitValue(i*2));
		}
		for (int i=0; i<su->Length()/2;i++) {
			CPPUNIT_ASSERT_EQUAL((int)ucsSwapped[i], su->UnitFromArray((SW_BYTE *)ucs, i*2));
		}
	}

	void testMove() {
		// CJK UNIFIED IDEOGRAPH-66E8 (3), HEBREW LETTER SHIN (2), LATIN SMALL LETTER T (1)
		wchar_t ucs[]=L"\ue866\ue905\x0d00\x0a00\x7400";
		su->InsertString(0, (SW_BYTE *)ucs, 2*wcslen(ucs));
		CPPUNIT_ASSERT_EQUAL(0, su->MovePositionOutsideChar(0, -1, false));
		CPPUNIT_ASSERT_EQUAL(0, su->MovePositionOutsideChar(0, 1, false));
		CPPUNIT_ASSERT_EQUAL(0, su->MovePositionOutsideChar(1, -1, false));
		CPPUNIT_ASSERT_EQUAL(2, su->MovePositionOutsideChar(1, 1, false));
		CPPUNIT_ASSERT_EQUAL(2, su->MovePositionOutsideChar(2, -1, false));
		CPPUNIT_ASSERT_EQUAL(2, su->MovePositionOutsideChar(2, 1, false));
		CPPUNIT_ASSERT_EQUAL(2, su->MovePositionOutsideChar(3, -1, false));
		CPPUNIT_ASSERT_EQUAL(4, su->MovePositionOutsideChar(3, 1, false));
		CPPUNIT_ASSERT_EQUAL(4, su->MovePositionOutsideChar(4, -1, false));
		CPPUNIT_ASSERT_EQUAL(4, su->MovePositionOutsideChar(4, 1, false));
		CPPUNIT_ASSERT_EQUAL(4, su->MovePositionOutsideChar(5, -1, false));
		CPPUNIT_ASSERT_EQUAL(6, su->MovePositionOutsideChar(5, 1, false));
		CPPUNIT_ASSERT_EQUAL(6, su->MovePositionOutsideChar(6, -1, false));
		CPPUNIT_ASSERT_EQUAL(6, su->MovePositionOutsideChar(6, 1, false));
		CPPUNIT_ASSERT_EQUAL(6, su->MovePositionOutsideChar(7, -1, false));
		CPPUNIT_ASSERT_EQUAL(8, su->MovePositionOutsideChar(7, 1, false));
		CPPUNIT_ASSERT_EQUAL(8, su->MovePositionOutsideChar(8, -1, false));
		CPPUNIT_ASSERT_EQUAL(8, su->MovePositionOutsideChar(8, 1, false));
		CPPUNIT_ASSERT_EQUAL(8, su->MovePositionOutsideChar(9, -1, false));
		CPPUNIT_ASSERT_EQUAL(10, su->MovePositionOutsideChar(9, 1, false));
		CPPUNIT_ASSERT_EQUAL(10, su->MovePositionOutsideChar(10, -1, false));
		CPPUNIT_ASSERT_EQUAL(10, su->MovePositionOutsideChar(10, 1, false));

		// Try with line end detection on: should move away from position  between cr and lf
		CPPUNIT_ASSERT_EQUAL(4, su->MovePositionOutsideChar(5, -1, true));
		CPPUNIT_ASSERT_EQUAL(8, su->MovePositionOutsideChar(5, 1, true));
		CPPUNIT_ASSERT_EQUAL(4, su->MovePositionOutsideChar(6, -1, true));
		CPPUNIT_ASSERT_EQUAL(8, su->MovePositionOutsideChar(6, 1, true));
		CPPUNIT_ASSERT_EQUAL(4, su->MovePositionOutsideChar(7, -1, true));
		CPPUNIT_ASSERT_EQUAL(8, su->MovePositionOutsideChar(7, 1, true));
		CPPUNIT_ASSERT_EQUAL(8, su->MovePositionOutsideChar(8, -1, true));
		CPPUNIT_ASSERT_EQUAL(8, su->MovePositionOutsideChar(8, 1, true));
	}

	void testUTFRetrieval() {
		char ucsBytes[]="\x66\xe8\x05\xe9\x00\x74\xDB\xFF\xDF\xFD";
		su->InsertString(0, (SW_BYTE *)ucsBytes, sizeof(ucsBytes)-1);

		unsigned char utf[]="\xe6\x9b\xa8\xd7\xa9t\xf4\x8f\xbf\xbd";
		char c[20];
		int len = su->RetrieveUTF8(0, (SW_BYTE *)c, su->Length());
		CPPUNIT_ASSERT_EQUAL(10, len);
		CPPUNIT_ASSERT(memcmp(utf, c, len) == 0);
		wchar_t w[20];
		wchar_t ucs[]=L"\u66e8\u05e9\x74\xDBFF\xDFFD";
		len = su->RetrieveUTF16(0, (SW_SHORT *)w, su->Length());
		CPPUNIT_ASSERT_EQUAL(5, len);
		CPPUNIT_ASSERT(memcmp(ucs, w, len*2) == 0);
		int x[20];
		int xcs[]= { 0x66e8, 0x05e9, 0x74,  0x10FFFD};
		len = su->RetrieveUTF32(0, x, su->Length());
		CPPUNIT_ASSERT_EQUAL(4, len);
		CPPUNIT_ASSERT(memcmp(xcs, x, len*4) == 0);
	}
};


class SVWRATest : public CppUnit::TestFixture {
CPPUNIT_TEST_SUITE(SVWRATest);
CPPUNIT_TEST(testAdd);
CPPUNIT_TEST_SUITE_END();

	SplitVectorWithRangeAdd *sv;

public:
	void setUp() {
		sv = new SplitVectorWithRangeAdd(4);
	}

	void tearDown() {
		delete sv;
	}

	void testAdd() {
		sv->InsertSpace(0, 4);
		for (int i=0; i<sv->Length(); i++) {
			CPPUNIT_ASSERT_EQUAL(0, sv->ValueAt(i));
		}
		sv->RangeAddDelta(1, 4, 1);
		sv->RangeAddDelta(2, 4, 1);
		sv->RangeAddDelta(3, 4, 1);
		for (int i=0; i<sv->Length(); i++) {
			CPPUNIT_ASSERT_EQUAL(i, sv->ValueAt(i));
		}
	}
};


class PartitionTest : public CppUnit::TestFixture {
CPPUNIT_TEST_SUITE(PartitionTest);
CPPUNIT_TEST(testCreation);
CPPUNIT_TEST(testBasic);
CPPUNIT_TEST(testSequence);
CPPUNIT_TEST(testEmptyLastDelete);
CPPUNIT_TEST(testEmptyWorks);
CPPUNIT_TEST_SUITE_END();

	Partitioning *p;

public:
	void setUp() {
		p = new Partitioning(4);
	}

	void tearDown() {
		delete p;
	}

	void testCreation() {
		CPPUNIT_ASSERT_EQUAL(1, p->Partitions());
		CPPUNIT_ASSERT_EQUAL(0, p->PositionFromPartition(0));
		CPPUNIT_ASSERT_EQUAL(0, p->PositionFromPartition(1));
	}

	void testBasic() {
		p->RemovePartition(0);
		p->InsertPartition(1, 11);
		p->InsertPartition(0,100);
		CPPUNIT_ASSERT_EQUAL(100, p->PositionFromPartition(0));
	}

	void testSequence() {
		p->InsertText(0, 4);
		CPPUNIT_ASSERT_EQUAL(1, p->Partitions());
		CPPUNIT_ASSERT_EQUAL(0, p->PositionFromPartition(0));
		CPPUNIT_ASSERT_EQUAL(4, p->PositionFromPartition(1));
		p->InsertPartition(1,2);
		CPPUNIT_ASSERT_EQUAL(2, p->Partitions());
		CPPUNIT_ASSERT_EQUAL(0, p->PositionFromPartition(0));
		CPPUNIT_ASSERT_EQUAL(2, p->PositionFromPartition(1));
		CPPUNIT_ASSERT_EQUAL(4, p->PositionFromPartition(2));
		p->InsertPartition(1,1);
		CPPUNIT_ASSERT_EQUAL(3, p->Partitions());
		CPPUNIT_ASSERT_EQUAL(0, p->PositionFromPartition(0));
		CPPUNIT_ASSERT_EQUAL(1, p->PositionFromPartition(1));
		CPPUNIT_ASSERT_EQUAL(2, p->PositionFromPartition(2));
		CPPUNIT_ASSERT_EQUAL(4, p->PositionFromPartition(3));
		p->InsertText(0, 1);
		CPPUNIT_ASSERT_EQUAL(3, p->Partitions());
		CPPUNIT_ASSERT_EQUAL(0, p->PositionFromPartition(0));
		CPPUNIT_ASSERT_EQUAL(2, p->PositionFromPartition(1));
		CPPUNIT_ASSERT_EQUAL(3, p->PositionFromPartition(2));
		CPPUNIT_ASSERT_EQUAL(5, p->PositionFromPartition(3));
		p->RemovePartition(2);
		CPPUNIT_ASSERT_EQUAL(2, p->Partitions());
		CPPUNIT_ASSERT_EQUAL(0, p->PositionFromPartition(0));
		CPPUNIT_ASSERT_EQUAL(2, p->PositionFromPartition(1));
		CPPUNIT_ASSERT_EQUAL(5, p->PositionFromPartition(2));
		p->InsertPartition(1, 2);
		CPPUNIT_ASSERT_EQUAL(3, p->Partitions());
		CPPUNIT_ASSERT_EQUAL(0, p->PositionFromPartition(0));
		CPPUNIT_ASSERT_EQUAL(2, p->PositionFromPartition(1));
		CPPUNIT_ASSERT_EQUAL(2, p->PositionFromPartition(2));
		CPPUNIT_ASSERT_EQUAL(5, p->PositionFromPartition(3));
	}

	void testEmptyLastDelete() {
		p->InsertText(0, 3);
		p->InsertPartition(1,3);
		CPPUNIT_ASSERT_EQUAL(2, p->Partitions());
		CPPUNIT_ASSERT_EQUAL(0, p->PositionFromPartition(0));
		CPPUNIT_ASSERT_EQUAL(3, p->PositionFromPartition(1));
		CPPUNIT_ASSERT_EQUAL(3, p->PositionFromPartition(2));
		p->DeleteAll();
		CPPUNIT_ASSERT_EQUAL(1, p->Partitions());
		CPPUNIT_ASSERT_EQUAL(0, p->PositionFromPartition(0));
		CPPUNIT_ASSERT_EQUAL(0, p->PositionFromPartition(1));
	}

	void testEmptyWorks() {
		p->InsertText(0, 3);
		p->InsertPartition(1,2);
		p->InsertPartition(2,2);
		CPPUNIT_ASSERT_EQUAL(3, p->Partitions());
		CPPUNIT_ASSERT_EQUAL(0, p->PositionFromPartition(0));
		CPPUNIT_ASSERT_EQUAL(2, p->PositionFromPartition(1));
		CPPUNIT_ASSERT_EQUAL(2, p->PositionFromPartition(2));
		CPPUNIT_ASSERT_EQUAL(3, p->PositionFromPartition(3));
		p->DeleteRange(1,1);
		CPPUNIT_ASSERT_EQUAL(2, p->Partitions());
		CPPUNIT_ASSERT_EQUAL(0, p->PositionFromPartition(0));
		CPPUNIT_ASSERT_EQUAL(1, p->PositionFromPartition(1));
		CPPUNIT_ASSERT_EQUAL(2, p->PositionFromPartition(2));
	}
};

class RunStylesTest : public CppUnit::TestFixture {
CPPUNIT_TEST_SUITE(RunStylesTest);
CPPUNIT_TEST(testRunStyles);
CPPUNIT_TEST_SUITE_END();

	void showRS(RunStyles *prs) {
		//~ if (!prs) {
		char s[100];
		int prev=-1;
		int len = prs->Runs();
		sprintf(s, "%d \n", len);
		std::cout << s;
		for (int i=0; i<=len; i++) {
			int pos = prs->starts->PositionFromPartition(i);
			int val = prs->ValueAt(pos);
			//if (val != prev) {
				sprintf(s, "  %1d %1d %3d\n", i, pos, val);
				std::cout << s;
				prev = val;
			// }
		}
		//~ }
	}
	void checkRS(RunStyles *prs, char *s, int len) {
		CPPUNIT_ASSERT_EQUAL(len, prs->Length());
		for (int i=0;i<len;i++) {
			CPPUNIT_ASSERT_EQUAL(static_cast<int>(s[i]), prs->ValueAt(i));
		}
	}
	void testRunStyles() {
		RunStyles *prs = new RunStyles();
		//~ showRS(prs);
		prs->InsertSpace(0, 5);
		checkRS(prs, "\0\0\0\0\0", 5);
		CPPUNIT_ASSERT_EQUAL(prs->Runs(), 1);
		//~ showRS(prs);
		prs->FillRange(1, '1', 3);
		checkRS(prs, "\000111\000", 5);
		CPPUNIT_ASSERT_EQUAL(prs->Runs(), 3);
		//~ showRS(prs);
		prs->FillRange(2, '2', 1);
		checkRS(prs, "\000121\000", 5);
		CPPUNIT_ASSERT_EQUAL(prs->Runs(), 5);
		//~ showRS(prs);
		prs->FillRange(3, '2', 1);
		checkRS(prs, "\000122\000", 5);
		//~ showRS(prs);
		CPPUNIT_ASSERT_EQUAL(prs->Runs(), 4);
		prs->DeleteRange(2, 1);
		checkRS(prs, "\00012\000", 4);
		CPPUNIT_ASSERT_EQUAL(prs->Runs(), 4);
		//~ showRS(prs);
		prs->DeleteRange(0, 4);
		checkRS(prs, "", 0);
		CPPUNIT_ASSERT_EQUAL(prs->Runs(), 1);
		//~ showRS(prs);
		delete prs;
	}
};


class KeyWordsTest : public CppUnit::TestFixture {
CPPUNIT_TEST_SUITE(KeyWordsTest);
CPPUNIT_TEST(testSet);
CPPUNIT_TEST(testClear);
CPPUNIT_TEST(testMultiple);
CPPUNIT_TEST_SUITE_END();

	KeyWords *kw;

public:
	void setUp() {
		kw = new KeyWords(false);
	}

	void tearDown() {
		delete kw;
	}

	void testSet() {
		Literal lClass("class");
		CPPUNIT_ASSERT_EQUAL(0, kw->Find(lClass.Data(),  0, lClass.Length()));
		CPPUNIT_ASSERT_EQUAL(0, kw->Sets());
		kw->SetText(&lClass, 1);
		CPPUNIT_ASSERT_EQUAL(1, kw->Find(lClass.Data(),  0, lClass.Length()));
		CPPUNIT_ASSERT_EQUAL(1, kw->Sets());
	}

	void testClear() {
		Literal lClass("class");
		kw->SetText(&lClass, 1);
		CPPUNIT_ASSERT_EQUAL(1, kw->Find(lClass.Data(),  0, lClass.Length()));
		CPPUNIT_ASSERT_EQUAL(1, kw->Sets());
		kw->Clear();
		CPPUNIT_ASSERT_EQUAL(0, kw->Find(lClass.Data(),  0, lClass.Length()));
		CPPUNIT_ASSERT_EQUAL(0, kw->Sets());
	}

	void testMultiple() {
		Literal lKW("class monad struct");
		Literal lKW2("char float int");
		Literal lClass("class");
		Literal lStruct("struct");
		Literal lInterface("interface");
		Literal lFloat("float");
		kw->SetText(&lKW, 2);
		kw->SetText(&lKW2, 3);
		CPPUNIT_ASSERT_EQUAL(2, kw->Find(lClass.Data(),  0, lClass.Length()));
		CPPUNIT_ASSERT_EQUAL(2, kw->Find(lStruct.Data(),  0, lStruct.Length()));
		CPPUNIT_ASSERT_EQUAL(3, kw->Find(lFloat.Data(),  0, lFloat.Length()));
		CPPUNIT_ASSERT_EQUAL(0, kw->Find(lInterface.Data(),  0, lInterface.Length()));
		CPPUNIT_ASSERT_EQUAL(3, kw->Sets());
	}
};


class DictionaryTest : public CppUnit::TestFixture {
CPPUNIT_TEST_SUITE(DictionaryTest);
CPPUNIT_TEST(testAdd);
CPPUNIT_TEST(testRemove);
CPPUNIT_TEST_SUITE_END();

	Dictionary *dict;

public:
	void setUp() {
		dict = new Dictionary();
	}

	void tearDown() {
		delete dict;
	}

	void testAdd() {
		dict->Allocate(8);
		Literal lKey("key");
		Literal lValue("value");
		SW_OBJECT ret = dict->Add(&lKey, new Literal(&lValue));
		CPPUNIT_ASSERT_EQUAL((SW_OBJECT)NULL, ret);
		CPPUNIT_ASSERT_EQUAL((SW_OBJECT)NULL, dict->Find(&lValue));
		Literal *lFind = (Literal *)dict->Find(&lKey);
		CPPUNIT_ASSERT(lFind);
		Literal *lFind2 = (Literal *)dict->Find(&lKey);
		CPPUNIT_ASSERT_EQUAL(lFind, lFind2);
		Literal *lKey1 = (Literal *)dict->FindKey(&lKey);
		CPPUNIT_ASSERT(lKey1);
		Literal *lKey2 = (Literal *)dict->FindKey(&lKey);
		CPPUNIT_ASSERT_EQUAL(lKey1, lKey2);

		SW_OBJECT o;
		while ((o=dict->DeleteOne()) != NULL) {
			delete (Literal *)(o);
		}
	}

	void testRemove() {
		Literal lKey("key");
		Literal lKey2("key2");
		Literal lValue("value");
		Literal lValue2("value");
		SW_OBJECT ret;
		ret = dict->Add(&lKey, new Literal(&lValue));
		CPPUNIT_ASSERT(ret == NULL);
		ret = dict->Add(&lKey, new Literal(&lValue2));
		CPPUNIT_ASSERT(ret);
		delete (Literal *)ret;
		ret = dict->Add(&lKey2, new Literal(&lValue));
		CPPUNIT_ASSERT(ret == NULL);
		ret = dict->Remove(&lKey);
		CPPUNIT_ASSERT(ret);
		delete (Literal *)ret;
		ret = dict->Remove(&lKey2);
		CPPUNIT_ASSERT(ret);
		delete (Literal *)ret;
	}
};


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

class OptionSetTest : public CppUnit::TestFixture {
CPPUNIT_TEST_SUITE(OptionSetTest);
CPPUNIT_TEST(testInsertion);
CPPUNIT_TEST(testKeys);
CPPUNIT_TEST(testMode);
CPPUNIT_TEST(testQuoting);
CPPUNIT_TEST(testStructuredValue);
CPPUNIT_TEST_SUITE_END();

	OptionSet *os;

public:
	void setUp() {
		os = new OptionSet();
	}

	void tearDown() {
		delete os;
	}

	void testInsertion() {
		Literal lName("level");
		Literal *ret = os->Value(&lName);
		CPPUNIT_ASSERT(ret == NULL);
		Literal lVal("14");
		os->Insert(&lName, &lVal);
		ret = os->Value(&lName);
		CPPUNIT_ASSERT(ret);
	}

	void testKeys() {
		Literal prefix("mode.base");
		ReadOptionsFile(os, &prefix, "examples/base.mode");
		Literal exts(".extensions");
		WordList *keys=os->Keys(&exts);
		for (int k=0;k<keys->Length();k++) {
			Literal *lKey = keys->GetText(k);
			CPPUNIT_ASSERT(lKey);
			Literal *lVal = os->Value(lKey);
			CPPUNIT_ASSERT(lVal);
			delete lKey;
		}
		delete keys;
	}

	void testMode() {
		Literal prefix("mode.base");
		ReadOptionsFile(os, &prefix, "examples/base.mode");
		Literal lModePython("mode.base.python");
		Literal lKeyLexer("lexer");
		Literal *lLexer = os->ModeValue(&lModePython, &lKeyLexer);
		CPPUNIT_ASSERT(lLexer);
		Literal python("Python");
		CPPUNIT_ASSERT(lLexer->Equals(&python));
		Literal lStyleItalics("mode.base.python.style.CommentLine.italics");
		Literal lExpected("1");
		CPPUNIT_ASSERT(os->Value(&lStyleItalics)->Equals(&lExpected));
	}

	void testQuoting() {
		Literal prefix("mode.base");
		ReadOptionsFile(os, &prefix, "examples/base.mode");
		Literal lModePython("mode.base.python");
		Literal python("Python");
		// Check unquoting: name is P\ython
		Literal lKeyName("name");
		Literal *lName = os->ModeValue(&lModePython, &lKeyName);
		CPPUNIT_ASSERT(lName->Equals(&python));
	}

	void testStructuredValue() {
		Literal prefix("mode.base");
		ReadOptionsFile(os, &prefix, "examples/base.mode");
		Literal lStyleDecorationType("mode.base.spell.style.Mistake.backdecoration.type");
		Literal lExpected("StrikeOut");
		CPPUNIT_ASSERT(os->Value(&lStyleDecorationType)->Equals(&lExpected));
	}
};


class HistoryTest : public CppUnit::TestFixture {
CPPUNIT_TEST_SUITE(HistoryTest);
CPPUNIT_TEST(testSimple);
CPPUNIT_TEST(testStack);
CPPUNIT_TEST_SUITE_END();

	History *h;

public:
	void setUp() {
		h = new History();
	}

	void tearDown() {
		delete h;
	}

	void testSimple() {
		CPPUNIT_ASSERT_EQUAL(0, h->Value());
		h->Append(10, 1);
		CPPUNIT_ASSERT_EQUAL(1, h->Value());
		h->InvalidateAfter(10);
		CPPUNIT_ASSERT_EQUAL(1, h->Value());
		h->InvalidateAfter(9);
		CPPUNIT_ASSERT_EQUAL(0, h->Value());
		h->Append(10, 1);
		CPPUNIT_ASSERT_EQUAL(1, h->Value());
		h->Append(20, 2);
		CPPUNIT_ASSERT_EQUAL(2, h->Value());
		h->Append(30, 3);
		CPPUNIT_ASSERT_EQUAL(3, h->Value());
		h->InvalidateAfter(20);
		CPPUNIT_ASSERT_EQUAL(2, h->Value());
		h->InvalidateAfter(10);
		CPPUNIT_ASSERT_EQUAL(1, h->Value());
		h->InvalidateAfter(9);
		CPPUNIT_ASSERT_EQUAL(0, h->Value());
	}

	void testStack() {
		Literal *te = h->TextValue();
		Literal first("first");
		Literal second("second");
		Literal third("third");
		CPPUNIT_ASSERT_EQUAL((Literal *)NULL, te);
		h->Push(10, new Literal("first"));
		te = h->TextValue();
		CPPUNIT_ASSERT(te->Equals(&first));
		h->Push(20, new Literal("second"));
		te = h->TextValue();
		CPPUNIT_ASSERT(te->Equals(&second));
		h->Push(30, new Literal("third"));
		te = h->TextValue();
		CPPUNIT_ASSERT(te->Equals(&third));
		h->Pop(40);
		te = h->TextValue();
		CPPUNIT_ASSERT(te->Equals(&second));
		h->Pop(50);
		te = h->TextValue();
		CPPUNIT_ASSERT(te->Equals(&first));
		h->Push(60, new Literal("twoA"));
		Literal twoA("twoA");
		CPPUNIT_ASSERT(h->TextValue()->Equals(&twoA));
		h->Pop(70);
		te = h->TextValue();
		CPPUNIT_ASSERT(te->Equals(&first));

		h->InvalidateAfter(75);
		CPPUNIT_ASSERT(h->TextValue()->Equals(&first));
		h->InvalidateAfter(65);
		CPPUNIT_ASSERT(h->TextValue()->Equals(&twoA));
		h->InvalidateAfter(55);
		CPPUNIT_ASSERT(h->TextValue()->Equals(&first));
		h->InvalidateAfter(45);
		CPPUNIT_ASSERT(h->TextValue()->Equals(&second));
		h->InvalidateAfter(35);
		CPPUNIT_ASSERT(h->TextValue()->Equals(&third));
		h->InvalidateAfter(25);
		CPPUNIT_ASSERT(h->TextValue()->Equals(&second));
		h->InvalidateAfter(15);
		CPPUNIT_ASSERT(h->TextValue()->Equals(&first));
		h->InvalidateAfter(5);
		CPPUNIT_ASSERT_EQUAL((Literal *)NULL, h->TextValue());
	}
};



class WordListTest : public CppUnit::TestFixture {
CPPUNIT_TEST_SUITE(WordListTest);
CPPUNIT_TEST(testInsertions);
CPPUNIT_TEST_SUITE_END();

	WordList *wl;

public:
	void setUp() {
		wl = new WordList(false);
	}

	void tearDown() {
		delete wl;
	}

	void testInsertions() {
		Literal keywords("alpha \xc2\xa8theta gamma");
		wl->Set(keywords.Data(), keywords.Length());
		Literal alpha("alpha");
		Literal theta("\xc2\xa8theta");
		Literal gamma("gamma");
		Literal delta("delta");
		Literal large("\xc2\xa0large");
		CPPUNIT_ASSERT(wl->InList(alpha.Data(), 0, alpha.Length()));
		CPPUNIT_ASSERT(wl->InList(theta.Data(), 0, theta.Length()));
		CPPUNIT_ASSERT(wl->InList(gamma.Data(), 0, gamma.Length()));
		CPPUNIT_ASSERT(!wl->InList(delta.Data(), 0, delta.Length()));
		CPPUNIT_ASSERT(!wl->InList(large.Data(), 0, large.Length()));
	}
};


class SymTabTest : public CppUnit::TestFixture {
CPPUNIT_TEST_SUITE(SymTabTest);
CPPUNIT_TEST(testAppend);
CPPUNIT_TEST(testInvalidate);
CPPUNIT_TEST(testInvalidateAll);
CPPUNIT_TEST_SUITE_END();

	SymbolTable *st;

public:
	void setUp() {
		st = new SymbolTable();
	}

	void tearDown() {
		delete st;
	}

	void testAppend() {
		Literal alpha("alpha");
		Literal beta("beta");
		Literal v1("1");
		Literal v2("2");
		Literal *ret = st->GetValue(0, &alpha);
		CPPUNIT_ASSERT(ret == NULL);
		st->Append(1, &alpha, &v1);
		st->Append(2, &beta, &v2);
		st->Append(3, &alpha, &v2);
		ret = st->GetValue(0, &alpha);
		CPPUNIT_ASSERT(ret == NULL);
		ret = st->GetValue(1, &alpha);
		CPPUNIT_ASSERT(ret == NULL);
		ret = st->GetValue(2, &alpha);
		CPPUNIT_ASSERT(ret != NULL);
		CPPUNIT_ASSERT(ret->Equals(&v1));
		ret = st->GetValue(3, &alpha);
		CPPUNIT_ASSERT(ret->Equals(&v1));
		ret = st->GetValue(4, &alpha);
		CPPUNIT_ASSERT(ret->Equals(&v2));
		ret = st->GetValue(5, &alpha);
		CPPUNIT_ASSERT(ret->Equals(&v2));
	}

	void testInvalidate() {
		Literal alpha("alpha");
		Literal beta("beta");
		Literal v1("1");
		Literal v2("2");
		st->Append(1, &alpha, &v1);
		st->Append(2, &beta, &v2);
		st->Append(3, &alpha, &v2);
		st->InvalidateAfter(2);
		Literal *ret;
		ret = st->GetValue(0, &alpha);
		CPPUNIT_ASSERT(ret == NULL);
		ret = st->GetValue(1, &alpha);
		CPPUNIT_ASSERT(ret == NULL);
		ret = st->GetValue(2, &alpha);
		CPPUNIT_ASSERT(ret != NULL);
		CPPUNIT_ASSERT(ret->Equals(&v1));
		ret = st->GetValue(3, &alpha);
		CPPUNIT_ASSERT(ret->Equals(&v1));
		ret = st->GetValue(4, &alpha);
		CPPUNIT_ASSERT(ret->Equals(&v1));
		ret = st->GetValue(5, &alpha);
		CPPUNIT_ASSERT(ret->Equals(&v1));
	}

	void testInvalidateAll() {
		Literal alpha("alpha");
		Literal beta("beta");
		Literal v1("1");
		Literal v2("2");
		st->Append(1, &alpha, &v1);
		st->Append(2, &beta, &v2);
		st->Append(3, &alpha, &v2);
		st->InvalidateAfter(-1);
		Literal *ret;
		ret = st->GetValue(0, &alpha);
		CPPUNIT_ASSERT(ret == NULL);
		ret = st->GetValue(1, &alpha);
		CPPUNIT_ASSERT(ret == NULL);
		ret = st->GetValue(2, &alpha);
		CPPUNIT_ASSERT(ret == NULL);
	}
};


class UndoStackTest : public CppUnit::TestFixture {
CPPUNIT_TEST_SUITE(UndoStackTest);
CPPUNIT_TEST(testInitial);
CPPUNIT_TEST(testSingleStep);
CPPUNIT_TEST_SUITE_END();

	// Batching aspects are tested in SplitTextTest

	UndoStack *us;
	SplitVector_1 *sv;

public:
	void setUp() {
		us = new UndoStack(20, 100);
		sv = new SplitVector_1();
		char contents[] = "abcdefgh";
		sv->InsertString(0, (SW_BYTE *)contents, strlen(contents));
	}

	void tearDown() {
		delete us;
		delete sv;
	}

	void testInitial() {
		CPPUNIT_ASSERT(us->IsSavePoint());
		CPPUNIT_ASSERT(!us->CanUndo());
		CPPUNIT_ASSERT(!us->CanRedo());
	}

	void testSingleStep() {
		us->AppendAction(UndoStack::ACTION_INSERT, 2, sv, 3);
		CPPUNIT_ASSERT(!us->IsSavePoint());
		CPPUNIT_ASSERT(us->CanUndo());
		CPPUNIT_ASSERT(!us->CanRedo());
		us->SetSavePoint();
		CPPUNIT_ASSERT(us->IsSavePoint());

		// Try undo and redo twice to ensure no bad memory
		for (int i=0;i<2;i++) {
			// Perform undo
			int steps = us->StartUndo();
			CPPUNIT_ASSERT_EQUAL(1, steps);
			UndoAction *ua = us->GetUndoStep();
			CPPUNIT_ASSERT(ua);
			CPPUNIT_ASSERT_EQUAL((int)UndoStack::ACTION_INSERT, ua->actionType);
			CPPUNIT_ASSERT_EQUAL(2, ua->position);
			CPPUNIT_ASSERT_EQUAL(3, ua->lenData);
			ua->data = new SW_BYTE[ua->lenData];
			sv->RetrieveString(ua->position, ua->data, ua->lenData);

			us->AppendInsertDataStep(ua);
			us->CompletedUndoStep();
			delete ua;
			CPPUNIT_ASSERT(!us->IsSavePoint());
			CPPUNIT_ASSERT(!us->CanUndo());
			CPPUNIT_ASSERT(us->CanRedo());

			// Perform redo
			steps = us->StartRedo();
			CPPUNIT_ASSERT_EQUAL(1, steps);
			ua = us->GetRedoStep();
			CPPUNIT_ASSERT(ua);
			CPPUNIT_ASSERT_EQUAL((int)UndoStack::ACTION_INSERT, ua->actionType);
			CPPUNIT_ASSERT_EQUAL(2, ua->position);
			CPPUNIT_ASSERT_EQUAL(3, ua->lenData);
			us->CompletedRedoStep();
			delete ua;
			CPPUNIT_ASSERT(us->IsSavePoint());
			CPPUNIT_ASSERT(us->CanUndo());
			CPPUNIT_ASSERT(!us->CanRedo());
		}
	}
};


class SplitTextTest : public CppUnit::TestFixture {
CPPUNIT_TEST_SUITE(SplitTextTest);
CPPUNIT_TEST(testBiggerSize);
CPPUNIT_TEST(testHash);
CPPUNIT_TEST(testSmallSimple);
CPPUNIT_TEST(testBadPositionException);
CPPUNIT_TEST(testWithStyles);
CPPUNIT_TEST(testUndoRedo);
CPPUNIT_TEST(testUTFAccess);
CPPUNIT_TEST_SUITE_END();

	SplitText *st;

public:
	void setUp() {
		st = new SplitText();
	}

	void tearDown() {
		delete st;
	}

	void testBiggerSize() {
		unsigned char test[]="abcde";
		int lenTest = strlen(reinterpret_cast<char *>(test));
		st->InsertBytes(0, test, lenTest);
		CPPUNIT_ASSERT_EQUAL(lenTest, st->Length());
		for (int n = 0; n < lenTest; n++) {
			CPPUNIT_ASSERT_EQUAL(st->CharValue(n), (int)test[n]);
		}
	}

	void testHash() {
		// Values found by running Python: hash(""), hash("a"), hash("ab")
		CPPUNIT_ASSERT_EQUAL(0, st->HashRange(0,0));
		unsigned char a[]="a";
		st->InsertBytes(0, a, 1);
		CPPUNIT_ASSERT_EQUAL(-468864544, st->HashRange(0,1));
		unsigned char b[]="b";
		st->InsertBytes(1, b, 1);
		CPPUNIT_ASSERT_EQUAL(-1549758589, st->HashRange(0,2));
	}

	void testSmallSimple() {
		SplitText *pbuffer = new SplitText(8, 8);
		//~ pbuffer->DiscardLines();
		//~ pbuffer->DeleteRange(0, pbuffer->Length());
		pbuffer->SetEncoding(SplitText::ENCODING_UTF8);
		unsigned char test[]="he\r\n";
		pbuffer->InsertBytes(0, test, 4);
		CPPUNIT_ASSERT_EQUAL(4, pbuffer->Length());
		CPPUNIT_ASSERT_EQUAL(2, pbuffer->Lines());
		CPPUNIT_ASSERT_EQUAL(0, pbuffer->PositionFromLine(0));
		CPPUNIT_ASSERT_EQUAL(4, pbuffer->PositionFromLine(1));
		CPPUNIT_ASSERT_EQUAL(4, pbuffer->PositionFromLine(2));
		CPPUNIT_ASSERT_EQUAL(2, pbuffer->LineEndPositionFromLine(0));
		CPPUNIT_ASSERT_EQUAL(4, pbuffer->LineEndPositionFromLine(1));

		int test32[] = {'h', 'e', '\r'};
		int ret32[] = {0,0,0,0};
		CPPUNIT_ASSERT_EQUAL(3, pbuffer->Characters(0,3));
		pbuffer->RetrieveUTF32(0, ret32, 3);
		CPPUNIT_ASSERT(memcmp(ret32, test32, sizeof(test32)) == 0);

		delete pbuffer;
	}

	void testBadPositionException() {
		// Now raises CharacterFragmentException so need another test for BadPositionException
		unsigned char test[]="abcde";
		try {
			st->InsertBytes(1, test, 1);
			// Must not reach here
			CPPUNIT_ASSERT(false);
			//fail("Should raise an CharacterFragmentException");
		} catch (BadPositionException *p) {
			delete p;
		} catch (CharacterFragmentException *p) {
			delete p;
		}
	}

	void testWithStyles() {
		Literal shortInsert("bc");
		Literal LF("\n");
		Literal CR("\r");

		IStyles *pstyles = SplitText::CreateStyles(1);

		st->AttachStyles(pstyles, NULL, 0, false);
		st->InsertUTF32(0, shortInsert.Data(), 2);
		// bc
		st->InsertUTF32(0, CR.Data(), 1);
		// \rbc
		st->InsertUTF32(0, LF.Data(), 1);
		CPPUNIT_ASSERT_EQUAL(0, st->PositionFromLine(0));
		// \n\rbc
		// This \r is merged into the previous line end
		st->InsertUTF32(0, CR.Data(), 1);

		delete pstyles;
	}

	void assertValue(SplitText *p, const char *s) {
		size_t len = p->Length();
		CPPUNIT_ASSERT_EQUAL(len, strlen(s));
		SW_BYTE *v = new SW_BYTE[len];
		p->RetrieveBytes(0, v, len);
		CPPUNIT_ASSERT_EQUAL(0, memcmp(v, s, len));
		delete []v;
	}

	void testUndoRedo() {
		IStyles *pstyles = SplitText::CreateStyles(1);

		st->AttachStyles(pstyles, NULL, 0, false);
		Literal utest("u");
		int lenUtest = utest.Length();
		Literal uutest("wmin");
		int lenUUtest = uutest.Length();
		st->InsertBytes(0, (SW_BYTE *)"xy", 2);
		assertValue(st, "xy");
		st->InsertUTF32(0, utest.Data(), lenUtest);
		assertValue(st, "uxy");
		st->DeleteRange(1, 1);
		assertValue(st, "uy");
		st->PerformUndoAction();
		assertValue(st, "uxy");
		st->PerformRedoAction();
		assertValue(st, "uy");

		// Undo to beginning and redo again
		st->PerformUndoAction();
		assertValue(st, "uxy");
		st->PerformUndoAction();
		assertValue(st, "xy");
		st->PerformUndoAction();
		assertValue(st, "");
		st->PerformRedoAction();
		assertValue(st, "xy");
		st->PerformRedoAction();
		assertValue(st, "uxy");
		st->PerformRedoAction();
		assertValue(st, "uy");

		// Do it again. Undo to beginning and redo again
		st->PerformUndoAction();
		assertValue(st, "uxy");
		st->PerformUndoAction();
		assertValue(st, "xy");
		st->PerformUndoAction();
		assertValue(st, "");
		st->PerformRedoAction();
		assertValue(st, "xy");
		st->PerformRedoAction();
		assertValue(st, "uxy");
		st->PerformRedoAction();
		assertValue(st, "uy");

		// Add a new step in
		st->PerformUndoAction();
		assertValue(st, "uxy");
		st->DeleteRange(0, 1);
		assertValue(st, "xy");
		st->PerformUndoAction();
		assertValue(st, "uxy");
		st->PerformUndoAction();
		assertValue(st, "xy");
		st->PerformUndoAction();
		assertValue(st, "");

		// Checking for batching
		// Typing one character then another after should be one step
		st->InsertUTF32(0, utest.Data(), lenUtest);
		CPPUNIT_ASSERT_EQUAL(1, st->StartUndo());
		assertValue(st, "u");
		st->InsertUTF32(1, utest.Data(), lenUtest);
		CPPUNIT_ASSERT_EQUAL(2, st->StartUndo());
		assertValue(st, "uu");
		st->PerformUndoStep();
		assertValue(st, "");
		st->PerformRedoStep();
		assertValue(st, "uu");
		// This one should not merge as it is after a redo
		st->InsertUTF32(2, utest.Data(), lenUtest);
		CPPUNIT_ASSERT_EQUAL(1, st->StartUndo());
		assertValue(st, "uuu");
		// This one should not merge as it is a delete and previous was insert
		st->DeleteRange(0, 1);
		CPPUNIT_ASSERT_EQUAL(1, st->StartUndo());
		assertValue(st, "uu");
		// This one should merge as it is a delete and next to previous
		st->DeleteRange(0, 1);
		CPPUNIT_ASSERT_EQUAL(2, st->StartUndo());
		assertValue(st, "u");
		st->PerformUndoStep();
		assertValue(st, "uuu");
		// This one should not merge as it is after an undo
		st->DeleteRange(2, 1);
		CPPUNIT_ASSERT_EQUAL(1, st->StartUndo());
		assertValue(st, "uu");
		// This one should merge as it is a delete and next to previous
		st->DeleteRange(1, 1);
		CPPUNIT_ASSERT_EQUAL(2, st->StartUndo());
		assertValue(st, "u");

		delete pstyles;
	}

	void testUTFAccess() {
		SplitText *st = new SplitText(8, 8);
		st->SetEncoding(SplitText::ENCODING_UTF8);
		unsigned char utf[]="\xe6\x9b\xa8\xd7\xa9t\xf4\x8f\xbf\xbd";
		st->InsertBytes(0, utf, strlen((char *)utf));
		char c[20];
		int len = st->RetrieveUTF8(0, (SW_BYTE *)c, st->Length());
		CPPUNIT_ASSERT_EQUAL(10, len);
		CPPUNIT_ASSERT(memcmp(utf, c, len) == 0);
		wchar_t w[20];
		wchar_t ucs[]=L"\u66e8\u05e9\x74\xDBFF\xDFFD";
		len = st->RetrieveUTF16(0, (SW_SHORT *)w, st->Length());
		CPPUNIT_ASSERT_EQUAL(5, len);
		CPPUNIT_ASSERT(memcmp(ucs, w, len*2) == 0);
		int x[20];
		int xcs[]= { 0x66e8, 0x05e9, 0x74,  0x10FFFD};
		len = st->RetrieveUTF32(0, x, st->Length());
		CPPUNIT_ASSERT_EQUAL(4, len);
		CPPUNIT_ASSERT(memcmp(xcs, x, len*4) == 0);
		delete st;
	}
};

char *kwCPP =
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
	"using virtual void while";

char *kwHTML =
	"a abbr acronym address applet area b base basefont "
	"bdo big blockquote body br button caption center "
	"cite code col colgroup dd del dfn dir div dl dt em "
	"fieldset font form frame frameset h1 h2 h3 h4 h5 h6 "
	"head hr html i iframe img input ins isindex kbd label "
	"legend li link map menu meta noframes noscript "
	"object ol optgroup option p param pre q s samp "
	"script select small span strike strong style sub sup "
	"table tbody td textarea tfoot th thead title tr tt u ul "
	"var xml xmlns";

char *kwPython =
	"and assert break class continue def del elif "
	"else except exec finally for from global if import in is lambda None "
	"not or pass print raise return try while yield";

char *kwVB =
	"and begin case call continue do each else elseif end erase "
	"error event exit false for function get gosub goto if implement in load loop lset "
	"me mid new next not nothing on or property raiseevent rem resume return rset "
	"select set stop sub then to true unload until wend while with withevents "
	"attribute alias as boolean byref byte byval const compare currency date declare dim double "
	"enum explicit friend global integer let lib long module object option optional "
	"preserve private property public redim single static string type variant";

char *kwPHP =
	"and argv as argc break case cfunction class continue declare default do "
	"die echo else elseif empty enddeclare endfor endforeach endif endswitch "
	"endwhile e_all e_parse e_error e_warning eval exit extends false for "
	"foreach function global http_cookie_vars http_get_vars http_post_vars "
	"http_post_files http_env_vars http_server_vars if include include_once "
	"list new not null old_function or parent php_os php_self php_version "
	"print require require_once return static switch stdclass this true var "
	"xor virtual while __file__ __line__ __sleep __wakeup";

char *kwSpell =
	"and are for is of the";

Literal *ReadFromFile(char *name) {
	Literal *ret = new Literal();
	FILE *file = fopen(name, "rb");
	if (file) {
		enum { sizeFile = 60000 };
		SW_BYTE data[sizeFile];
		int lenData = static_cast<int>(fread(data, 1, sizeof(data), file));
		fclose(file);
		delete ret;
		ret = new Literal((char *)data, lenData);
	}
	return ret;
}

void AppendUTF8(char *buf, Literal *value) {
	int lenBytes = UniConversion::UTF8LengthFromUTF32(value->Data(), value->Length());
	SW_BYTE *bytes = new SW_BYTE[lenBytes];
	UniConversion::UTF8FromUTF32(bytes, lenBytes, value->Data(), value->Length());
	int len = strlen(buf);
	memcpy(buf + len, bytes, lenBytes);
	buf[len + lenBytes] = 0;
	delete []bytes;
}

Literal *AsStyled(SplitText *st, ILexer *lx) {
	enum { sizeFile = 60000 };
	SW_BYTE data[sizeFile];
	int len = 0;

	for (int s=0;s<lx->States();s++) {
		Literal *lexPath = new Literal(lx->Name());
		ILexer *stateLexer = lx->LexerFromState(s);
		while (stateLexer != NULL) {
			Literal *lexPathNew = new Literal(lexPath, '.', stateLexer->Name());
			delete lexPath;
			lexPath = lexPathNew;
			stateLexer = stateLexer->LexerFromState(s);
		}
		Literal *stateName = lx->StateName(s);
		char stateBuf[200];
		sprintf(stateBuf, "%0d=", s);
		AppendUTF8(stateBuf, lexPath);
		strcat(stateBuf, ".");
		AppendUTF8(stateBuf, stateName);
		strcat(stateBuf, "\r\n");
		delete stateName;
		strcpy((char *)data + len, stateBuf);
		len += strlen(stateBuf);
	}
	int prevStyle = -1;
	for (int o=0; o<st->Length(); o++) {
		int styleNow = st->StyleAt(o);
		if (styleNow != prevStyle) {
			char styleBuf[20];
			sprintf(styleBuf, "{%0d}", styleNow);
			strcpy((char *)data + len, styleBuf);
			len += strlen(styleBuf);
			prevStyle = styleNow;
		}
		data[len++] =st->XCharAt(o);
	}
	Literal *ret = new Literal((char *)data, len);
	return ret;
}

void WriteToFile(char *name, Literal *value) {
	FILE *file = fopen(name, "wb");
	if (file) {
		int lenBytes = UniConversion::UTF8LengthFromUTF32(value->Data(), value->Length());
		SW_BYTE *bytes = new SW_BYTE[lenBytes];
		UniConversion::UTF8FromUTF32(bytes, lenBytes, value->Data(), value->Length());
		fwrite(bytes, 1, lenBytes, file);
		delete []bytes;
		fclose(file);
	}
}

class LanguageTest : public CppUnit::TestFixture {
CPPUNIT_TEST_SUITE(LanguageTest);
CPPUNIT_TEST(testNull);
CPPUNIT_TEST(testCPP);
CPPUNIT_TEST(testPython);
CPPUNIT_TEST(testVB);
CPPUNIT_TEST(testHTML);
CPPUNIT_TEST(testHTMLScript);
CPPUNIT_TEST(testPHP);
CPPUNIT_TEST(testASP);
CPPUNIT_TEST(testASPHTML);
CPPUNIT_TEST(testASPScript);
CPPUNIT_TEST(testSpell);
CPPUNIT_TEST(testURL);
CPPUNIT_TEST(testBraceCount);
CPPUNIT_TEST_SUITE_END();

	SplitText *st;
	IStyles *pstyle;
	Literal *prevStyled;
	Literal *progStyled;

public:
	void setUp() {
		st = new SplitText();
		pstyle = SplitText::CreateStyles(1);
		prevStyled = NULL;
		progStyled = NULL;
	}

	void tearDown() {
		delete st;
		delete pstyle;
		delete prevStyled;
		delete progStyled;
	}

	void testExample(char *name, ILexer *lx) {
		char nameExample[_MAX_PATH];
		strcpy(nameExample, "examples\\");
		strcat(nameExample, name);
		char namePrevious[_MAX_PATH];
		strcpy(namePrevious, nameExample);
		strcat(namePrevious, ".styled");
		char nameNew[_MAX_PATH];
		strcpy(nameNew, namePrevious);
		strcat(nameNew, ".new");

		Literal *prog = ReadFromFile(nameExample);
		st->InsertUTF32(0, prog->Data(), prog->Length());
		delete prog;
		st->LexTo(st->Length(), 0, false);
		prevStyled = ReadFromFile(namePrevious);
		progStyled = AsStyled(st, lx);
		if (!progStyled->Equals(prevStyled)) {
			WriteToFile(nameNew, progStyled);
			CPPUNIT_ASSERT(progStyled->Equals(prevStyled));
			// The whole file doesn't parse like it did before so don't try line by line
			// as that is likely to fail many times.
			return;
		}

		// Try partial lexes from the start of every line which should all be identical.
		for (int line = 0; line<st->Lines(); line++) {
			int lineStart = st->PositionFromLine(line);
			lx->InvalidateAfter(lineStart);
			st->LexTo(st->Length(), 0, false);
			delete progStyled;
			progStyled = AsStyled(st, lx);
			if (!progStyled->Equals(prevStyled)) {
				WriteToFile(nameNew, progStyled);
				CPPUNIT_ASSERT(progStyled->Equals(prevStyled));
				// Give up after one failure
				return;
			}
		}
	}

	void testNull() {
		LexNull lx;
		st->AttachStyles(pstyle, &lx, 0, false);
		testExample("x.null", &lx);
	}

	void testCPP() {
		LexCPP lx;
		st->AttachStyles(pstyle, &lx, 0, false);
		Literal keywordsCPP(kwCPP);
		lx.SetKeyWords(1, &keywordsCPP);
		testExample("x.cxx", &lx);
	}

	void testPython() {
		LexPython lx;
		st->AttachStyles(pstyle, &lx, 0, false);
		Literal keywordsPython(kwPython);
		lx.SetKeyWords(1, &keywordsPython);
		testExample("x.py", &lx);
	}

	void testVB() {
		LexVB lx;
		st->AttachStyles(pstyle, &lx, 0, false);
		Literal keywordsVB(kwVB);
		lx.SetKeyWords(1, &keywordsVB);
		testExample("x.vb", &lx);
	}

	void testHTML() {
		LexHTML lx;
		st->AttachStyles(pstyle, &lx, 0, false);
		Literal keywordsHTML(kwHTML);
		lx.SetKeyWords(1, &keywordsHTML);
		testExample("x.html", &lx);
	}

	void testHTMLScript() {
		LexHTML lx;
		Literal keywordsHTML(kwHTML);
		lx.SetKeyWords(1, &keywordsHTML);

		LexCPP lxj;
		Literal keywordsCPP(kwCPP);
		lxj.SetKeyWords(1, &keywordsCPP);
		Literal javascript("javascript");
		lx.SetSubLexer(&lxj, &javascript, NULL);

		LexVB lxvb;
		Literal keywordsVB(kwVB);
		lxvb.SetKeyWords(1, &keywordsVB);
		Literal vbs("vbs");
		lx.SetSubLexer(&lxvb, &vbs, NULL);

		st->AttachStyles(pstyle, &lx, 0, false);

		testExample("scripted.html", &lx);
	}

	void testPHP() {
		LexPHP lx;
		st->AttachStyles(pstyle, &lx, 0, false);
		Literal keywordsPHP(kwPHP);
		lx.SetKeyWords(1, &keywordsPHP);
		testExample("x.php", &lx);
	}

	void testASP() {
		LexASP la;
		// Install a null lexer in the html slot.
		LexNull lNull;
		Literal html("html");
		la.SetSubLexer(&lNull, &html, NULL);
		st->AttachStyles(pstyle, &la, 0, false);
		testExample("x.asp", &la);
	}

	void testASPHTML() {
		LexASP la;
		// Install a null lexer in the html slot.
		LexHTML lHTML;
		Literal html("html");
		la.SetSubLexer(&lHTML, &html, NULL);
		st->AttachStyles(pstyle, &la, 0, false);
		testExample("html.asp", &la);
	}

	void testASPScript() {
		Literal keywordsCPP(kwCPP);
		Literal keywordsVB(kwVB);
		Literal html("html");
		Literal javascript("javascript");
		Literal vbs("vbs");

		LexASP la;

		LexHTML lx;
		Literal keywordsHTML(kwHTML);
		lx.SetKeyWords(1, &keywordsHTML);

		LexCPP lxsj;
		lxsj.SetKeyWords(1, &keywordsCPP);
		la.SetSubLexer(&lxsj, &javascript, &javascript);

		LexVB lxsvb;
		lxsvb.SetKeyWords(1, &keywordsVB);
		la.SetSubLexer(&lxsvb, &vbs, &vbs);

		LexCPP lxj;
		lxj.SetKeyWords(1, &keywordsCPP);
		lx.SetSubLexer(&lxj, &javascript, &javascript);

		LexVB lxvb;
		lxvb.SetKeyWords(1, &keywordsVB);
		lx.SetSubLexer(&lxvb, &vbs, &vbs);

		la.SetSubLexer(&lx, &html, NULL);

		st->AttachStyles(pstyle, &la, 0, false);
		testExample("scripted.asp", &la);
	}

	void testSpell() {
		LexSpell lx;
		st->AttachStyles(pstyle, &lx, 0, false);
		Literal keywordsSpell(kwSpell);
		lx.SetKeyWords(1, &keywordsSpell);
		testExample("x.spell", &lx);
	}

	void testURL() {
		LexURL lx;
		st->AttachStyles(pstyle, &lx, 0, false);
		testExample("x.web", &lx);
	}

	void testBraceCount() {
		LexBraceCount lx;
		st->AttachStyles(pstyle, &lx, 0, false);
		testExample("x.brace", &lx);
	}
};

void PerformTests() {
	CppUnit::TestResult controller;
	CppUnit::TestResultCollector result;
	controller.addListener(&result);
	CppUnit::TestRunner runner;
	runner.addTest(LiteralTest::suite());
	runner.addTest(LiteralSetTest::suite());
	runner.addTest(SetIntegersTest::suite());
	runner.addTest(MaxFinderTest::suite());
	runner.addTest(SIVTest::suite());
	runner.addTest(SPTest::suite());
	runner.addTest(SV0Test::suite());
	runner.addTest(SV1Test::suite());
	runner.addTest(SV2Test::suite());
	runner.addTest(SV4Test::suite());
	runner.addTest(TextPosTest::suite());
	runner.addTest(UniConvTest::suite());
	runner.addTest(ByteBufferTest::suite());
	runner.addTest(SubstUTF8Test::suite());
	runner.addTest(SubstByteTest::suite());
	runner.addTest(SubstUCSLETest::suite());
	runner.addTest(SubstUCSBETest::suite());
	runner.addTest(SVWRATest::suite());
	runner.addTest(PartitionTest::suite());
	runner.addTest(RunStylesTest::suite());
	runner.addTest(KeyWordsTest::suite());
	runner.addTest(DictionaryTest::suite());
	runner.addTest(OptionSetTest::suite());
	runner.addTest(HistoryTest::suite());
	runner.addTest(WordListTest::suite());
	runner.addTest(SymTabTest::suite());
	runner.addTest(UndoStackTest::suite());
	runner.addTest(SplitTextTest::suite());
	runner.addTest(LanguageTest::suite());
	runner.run(controller);
	CppUnit::CompilerOutputter outputter(&result, std::cerr );
	outputter.write();
}

int main(int, char**) {
	//~ _CrtSetBreakAlloc(204);

	PerformTests();
#if _DEBUG
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDERR);
	_CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDERR);
	_CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
	_CrtDumpMemoryLeaks();
#endif
	return 0;
}
