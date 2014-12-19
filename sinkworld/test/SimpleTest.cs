using System;

using NUnit.Framework;

/**
 * Initial tests for the cbx class.
 */
[TestFixture]
public class SimpleTest {
	Text test = new Text("abcde");
	Text longtest = new Text("ghijklmnopqrstuvwxyz");
	Text shortInsert = new Text("bc");
	Text LF = new Text("\n");
	Text CR = new Text("\r");
	Text CRLF = new Text("\r\n");
	Text X = new Text("x");
	Text utest = new Text("u");
	Text xytest = new Text("xy");

	public SimpleTest() { }

	public static byte[] XToByteArray(String s) {
		byte[] ret = new byte[s.Length];
		for (int i=0; i<s.Length; i++)
			ret[i] = (byte)s[i];
		return ret;
	}

	static String ToString(pbx p) {
		byte[] v = new byte[p.Length()];
		p.RetrieveString(0, v, p.Length());
		char[] cv = new char[p.Length()];
		for (int i=0; i<p.Length(); i++)
			cv[i] = (char)v[i];
		return new String(cv);
	}

	void InsertString(pbx p, int pos, byte []s) {
		p.InsertString(pos, s, s.Length);
	}
	[Test]
	public void TestSequence() {
		cbx_1 buffer = new cbx_1();
		buffer.Create(20,100);
		int lenTest = test.Length();
		buffer.InsertString(0, test.Data(), lenTest);
		Assert.AreEqual(buffer.Length(), lenTest);
		byte[] ret = new byte[3];
		buffer.RetrieveString(1, ret, 3);
		for (int h = 0; h < 3; h++) {
			Assert.AreEqual(ret[h], test.CharAt(h + 1));
		}
		for (int i = 0; i < lenTest; i++) {
			Assert.AreEqual(test.CharAt(i), buffer.CharAt(i));
		}
		buffer.DeleteRange(1, 2);
		Assert.AreEqual(buffer.Length(), lenTest - 2);
		buffer.InsertString(1, shortInsert.Data(), 2);
		Assert.AreEqual(buffer.Length(), lenTest);
		for (int k = 0; k < lenTest; k++) {
			Assert.AreEqual(test.CharAt(k), buffer.CharAt(k));
		}
	}
	[Test]
	public void TestManyInsertions() {
		cbx_1 buffer = new cbx_1();
		buffer.Create(20,100);
		int total = 0;
		buffer.InsertString(0, longtest.Data(), 1);
		total++;
		for (int l = 0; l < 10000; l++) {
			for (int m = 0; m < 20; m++) {
				buffer.InsertString(1, longtest.Data(), m + 1);
				total += m + 1;
			}
		}
		Assert.AreEqual(buffer.Length(), total);
	}

	[Test]
	public void TestBiggerSize() {
		pbx pbuffer = new pbx();
		pbuffer.AttachStyles(pbx.CreateStyles(1), null, 0);
		int lenTest = test.Length();
		pbuffer.InsertString(0, test.Data(), lenTest);
		Assert.AreEqual(pbuffer.Length(), lenTest);
		for (int n = 0; n < lenTest; n++) {
			Assert.AreEqual(test.CharAt(n), pbuffer.CharAt(n));
		}
	}

	[Test]
	public void TestBadPositionException() {
		pbx pbuffer = new pbx();
		pbuffer.AttachStyles(pbx.CreateStyles(1), null, 0);
		try {
			pbuffer.InsertString(1, test.Data(), 1);
			Assert.Fail("Should raise an BadPositionException");
		} catch (BadPositionException) {
		}
	}

	[Test]
	public void testFillRange() {
		cbx_1 buffer = new cbx_1();
		buffer.Create(20,100);
		int total = 0;
		buffer.InsertString(0, longtest.Data(), 10);
		buffer.FillRange(0, (byte)'x', 10);
		total++;
		for (int l = 0; l < 10; l++) {
			Assert.AreEqual('x', (char)buffer.CharAt(l));
		}
	}

	[Test]
	public void testIntegerVector() {
		iv v = new iv();
		Assert.AreEqual(0, v.Length());
		try {
			v.Remove(0);
			Assert.Fail("Should raise an BadPositionException");
		} catch (BadPositionException) {
		}
		try {
			v.Insert(1, 11);
			Assert.Fail("Should raise an BadPositionException");
		} catch (BadPositionException) {
		}
		v.Insert(0, 100);
		Assert.AreEqual(1, v.Length());
		Assert.AreEqual(100, v.GetValue(0));
		v.SetValue(0, 200);
		Assert.AreEqual(1, v.Length());
		Assert.AreEqual(200, v.GetValue(0));
		v.SetValue(0, 300);
		Assert.AreEqual(1, v.Length());
		Assert.AreEqual(300, v.GetValue(0));
		v.Remove(0);
		Assert.AreEqual(0, v.Length());
	}

	[Test]
	public void testGappedIntegerVector() {
		giv v = new giv();
		Assert.AreEqual(0, v.Length());
		try {
			v.Remove(0);
			Assert.Fail("Should raise an BadPositionException");
		} catch (BadPositionException) {
		}
		try {
			v.Insert(1, 11);
			Assert.Fail("Should raise an BadPositionException");
		} catch (BadPositionException) {
		}
		v.Insert(0, 100);
		Assert.AreEqual(1, v.Length());
		Assert.AreEqual(100, v.GetValue(0));
		v.SetValue(0, 200);
		Assert.AreEqual(1, v.Length());
		Assert.AreEqual(200, v.GetValue(0));
		v.SetValue(0, 300);
		Assert.AreEqual(1, v.Length());
		Assert.AreEqual(300, v.GetValue(0));
		v.Remove(0);
		Assert.AreEqual(0, v.Length());
	}

	[Test]
	public void testLineVector() {
		lv v = new lv(32);
		Assert.AreEqual(1, v.Lines());
		Assert.AreEqual(v.PositionFromLine(0), 0);
		v.InsertLine(1, 100);
		Assert.AreEqual(2, v.Lines());
		Assert.AreEqual(v.PositionFromLine(0), 0);
		Assert.AreEqual(v.PositionFromLine(1), 100);
		v.SetLineStartPosition(1, 200);
		Assert.AreEqual(2, v.Lines());
		Assert.AreEqual(200, v.PositionFromLine(1));
		v.SetLineStartPosition(1, 300);
		Assert.AreEqual(2, v.Lines());
		Assert.AreEqual(300, v.PositionFromLine(1));
		v.Remove(0);
		Assert.AreEqual(1, v.Lines());
	}

	[Test]
	public void testLineInsertionDeletion() {
		// The complexity in the line vector is caused by treating any
		// of \r, \n, and \r\n as line ends so test a variety of these.
		pbx pbuffer = new pbx();
		pbuffer.AttachStyles(pbx.CreateStyles(1), null, 0);
		//Assert.AreEqual(pbuffer.Lines(), 1);
		//Assert.AreEqual(pbuffer.PositionFromLine(0), 0);
		//Assert.AreEqual(pbuffer.LineEndPositionFromLine(0), 0);

		InsertString(pbuffer, 0, shortInsert.Data());
		// bc
		//Assert.AreEqual(pbuffer.Lines(), 1);
		//Assert.AreEqual(pbuffer.PositionFromLine(0), 0);
		//Assert.AreEqual(pbuffer.LineEndPositionFromLine(0), 2);

		InsertString(pbuffer, 0, CR.Data());
		// \rbc
		Assert.AreEqual(pbuffer.Lines(), 2);
		Assert.AreEqual(pbuffer.PositionFromLine(0), 0);
		Assert.AreEqual(pbuffer.PositionFromLine(1), 1);
		Assert.AreEqual(pbuffer.PositionFromLine(2), 3);

		InsertString(pbuffer, 0, LF.Data());
		// \n\rbc
		Assert.AreEqual(pbuffer.Lines(), 3);
		Assert.AreEqual(pbuffer.PositionFromLine(0), 0);
		Assert.AreEqual(pbuffer.PositionFromLine(1), 1);
		Assert.AreEqual(pbuffer.PositionFromLine(2), 2);
		Assert.AreEqual(pbuffer.PositionFromLine(3), 4);

		// This \r is merged into the previous line end
		InsertString(pbuffer, 0, CR.Data());
		// \r\n\rbc
		Assert.AreEqual(pbuffer.Lines(), 3);
		Assert.AreEqual(pbuffer.PositionFromLine(0), 0);
		Assert.AreEqual(pbuffer.PositionFromLine(1), 2);
		Assert.AreEqual(pbuffer.PositionFromLine(2), 3);
		Assert.AreEqual(pbuffer.PositionFromLine(3), 5);

		// As is this \n
		InsertString(pbuffer, 3, LF.Data());
		// \r\n\r\nbc
		Assert.AreEqual(pbuffer.Lines(), 3);
		Assert.AreEqual(pbuffer.PositionFromLine(0), 0);
		Assert.AreEqual(pbuffer.PositionFromLine(1), 2);
		Assert.AreEqual(pbuffer.PositionFromLine(2), 4);
		Assert.AreEqual(pbuffer.PositionFromLine(3), 6);

		// Separate a \r\n pair
		InsertString(pbuffer, 3, X.Data());
		// \r\n\rx\nbc
		Assert.AreEqual(pbuffer.Lines(), 4);
		// Check all the line starts are correct
		Assert.AreEqual(pbuffer.PositionFromLine(0), 0);
		Assert.AreEqual(pbuffer.PositionFromLine(1), 2);
		Assert.AreEqual(pbuffer.PositionFromLine(2), 3);
		Assert.AreEqual(pbuffer.PositionFromLine(3), 5);
		Assert.AreEqual(pbuffer.PositionFromLine(4), 7);

		// Destroy and recreate the lines data to ensure it is recreated correctly
		pbuffer.DiscardLines();
		Assert.AreEqual(pbuffer.Lines(), 4);
		// Check all the line starts are correct
		Assert.AreEqual(pbuffer.PositionFromLine(0), 0);
		Assert.AreEqual(pbuffer.PositionFromLine(1), 2);
		Assert.AreEqual(pbuffer.PositionFromLine(2), 3);
		Assert.AreEqual(pbuffer.PositionFromLine(3), 5);
		Assert.AreEqual(pbuffer.PositionFromLine(4), 7);

		// Put them back together
		pbuffer.DeleteRange(3, 1);
		// \r\n\r\nbc
		Assert.AreEqual(pbuffer.Lines(), 3);

		pbuffer.DeleteRange(0, 1);
		// \n\r\nbc
		Assert.AreEqual(pbuffer.Lines(), 3);

		pbuffer.DeleteRange(0, 1);
		// \r\nbc
		Assert.AreEqual(pbuffer.Lines(), 2);

		pbuffer.DeleteRange(0, 2);
		// bc
		Assert.AreEqual(pbuffer.Lines(), 1);
	}

	[Test]
	public void testUndo() {
		pbx ub = new pbx();
		ub.AttachStyles(pbx.CreateStyles(1), null, 0);
		int lenUtest = utest.Length();
		int lenXYtest = xytest.Length();

		ub.InsertString(0, xytest.Data(), lenXYtest);
		Assert.AreEqual(ToString(ub), "xy");
		ub.InsertString(0, utest.Data(), lenUtest);
		Assert.AreEqual(ToString(ub), "uxy");
		ub.DeleteRange(1, 1);
		Assert.AreEqual(ToString(ub), "uy");
		ub.PerformUndoAction();
		Assert.AreEqual(ToString(ub), "uxy");
		ub.PerformRedoAction();
		Assert.AreEqual(ToString(ub), "uy");

		// Undo to beginning and redo again
		ub.PerformUndoAction();
		Assert.AreEqual(ToString(ub), "uxy");
		ub.PerformUndoAction();
		Assert.AreEqual(ToString(ub), "xy");
		ub.PerformUndoAction();
		Assert.AreEqual(ToString(ub), "");
		ub.PerformRedoAction();
		Assert.AreEqual(ToString(ub), "xy");
		ub.PerformRedoAction();
		Assert.AreEqual(ToString(ub), "uxy");
		ub.PerformRedoAction();
		Assert.AreEqual(ToString(ub), "uy");

		// Do it again. Undo to beginning and redo again
		ub.PerformUndoAction();
		Assert.AreEqual(ToString(ub), "uxy");
		ub.PerformUndoAction();
		Assert.AreEqual(ToString(ub), "xy");
		ub.PerformUndoAction();
		Assert.AreEqual(ToString(ub), "");
		ub.PerformRedoAction();
		Assert.AreEqual(ToString(ub), "xy");
		ub.PerformRedoAction();
		Assert.AreEqual(ToString(ub), "uxy");
		ub.PerformRedoAction();
		Assert.AreEqual(ToString(ub), "uy");

		// Add a new step in
		ub.PerformUndoAction();
		Assert.AreEqual(ToString(ub), "uxy");
		ub.DeleteRange(0, 1);
		Assert.AreEqual(ToString(ub), "xy");
		ub.PerformUndoAction();
		Assert.AreEqual(ToString(ub), "uxy");
		ub.PerformUndoAction();
		Assert.AreEqual(ToString(ub), "xy");
		ub.PerformUndoAction();
		Assert.AreEqual(ToString(ub), "");
	}

	[Test]
	public void testUndoBatching() {
		pbx ub = new pbx();
		ub.AttachStyles(pbx.CreateStyles(1), null, 0);
		int lenUtest = utest.Length();
		// Checking for batching
		// Typing one character then another after should be one step
		ub.InsertString(0, utest.Data(), lenUtest);
		Assert.AreEqual(ub.StartUndo(), 1);
		Assert.AreEqual(ToString(ub), "u");
		ub.InsertString(1, utest.Data(), lenUtest);
		Assert.AreEqual(ub.StartUndo(), 2);
		Assert.AreEqual(ToString(ub), "uu");
		ub.PerformUndoStep();
		Assert.AreEqual(ToString(ub), "");
		ub.PerformRedoStep();
		Assert.AreEqual(ToString(ub), "uu");
		// This one should not merge as it is after a redo
		ub.InsertString(2, utest.Data(), lenUtest);
		Assert.AreEqual(ub.StartUndo(), 1);
		Assert.AreEqual(ToString(ub), "uuu");
		// This one should not merge as it is a delete and previous was insert
		ub.DeleteRange(0, 1);
		Assert.AreEqual(ub.StartUndo(), 1);
		Assert.AreEqual(ToString(ub), "uu");
		// This one should merge as it is a delete and next to previous
		ub.DeleteRange(0, 1);
		Assert.AreEqual(ub.StartUndo(), 2);
		Assert.AreEqual(ToString(ub), "u");
		ub.PerformUndoStep();
		Assert.AreEqual(ToString(ub), "uuu");
		// This one should not merge as it is after an undo
		ub.DeleteRange(2, 1);
		Assert.AreEqual(ub.StartUndo(), 1);
		Assert.AreEqual(ToString(ub), "uu");
		// This one should merge as it is a delete and next to previous
		ub.DeleteRange(1, 1);
		Assert.AreEqual(ub.StartUndo(), 2);
		Assert.AreEqual(ToString(ub), "u");
	}
}
