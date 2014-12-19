import junit.framework.*;

/**
 * Initial tests for the cbx class.
 */
public class SimpleTest extends TestCase {
	Literal test = new Literal("abcde");
	Literal longtest = new Literal("ghijklmnopqrstuvwxyz");
	Literal shortInsert = new Literal("bc");
	Literal LF = new Literal("\n");
	Literal CR = new Literal("\r");
	Literal CRLF = new Literal("\r\n");
	Literal X = new Literal("x");
	Literal utest = new Literal("u");
	Literal xytest = new Literal("xy");

	public SimpleTest(String name) {
		super(name);
	}
	protected void setUp() {
	}
	public static Test suite() {
		return new TestSuite(SimpleTest.class);
	}

	static String ToString(SplitText p) {
		byte[] v = new byte[p.Length()];
		p.RetrieveString(0, v, p.Length());
		char[] cv = new char[p.Length()];
		for (int i=0; i<p.Length(); i++)
			cv[i] = (char)v[i];
		return new String(cv);
	}

	void InsertString(SplitText p, int pos, byte []s) {
		p.InsertString(pos, s, s.length);
	}

	public void testSequence() {
		SplitVector_1 buffer = new SplitVector_1();
		buffer.Create(20,100);
		int lenTest = test.Length();
		buffer.InsertString(0, test.Data(), lenTest);
		assertEquals(buffer.Length(), lenTest);
		byte[] ret = new byte[3];
		buffer.RetrieveString(1, ret, 3);
		for (int h = 0; h < 3; h++) {
			assertEquals(ret[h], test.CharAt(h + 1));
		}
		for (int i = 0; i < lenTest; i++) {
			assertEquals(test.CharAt(i), buffer.CharAt(i));
		}
		buffer.DeleteRange(1, 2);
		assertEquals(buffer.Length(), lenTest - 2);
		buffer.InsertString(1, shortInsert.Data(), 2);
		assertEquals(buffer.Length(), lenTest);
		for (int k = 0; k < lenTest; k++) {
			assertEquals(test.CharAt(k), buffer.CharAt(k));
		}
	}

	public void testManyInsertions() {
		SplitVector_1 buffer = new SplitVector_1();
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
		assertEquals(buffer.Length(), total);
	}

	public void testBiggerSize() {
		SplitText pbuffer = new SplitText();
		pbuffer.AttachStyles(SplitText.CreateStyles(1), null, 0);
		int lenTest = test.Length();
		pbuffer.InsertString(0, test.Data(), lenTest);
		assertEquals(pbuffer.Length(), lenTest);
		for (int n = 0; n < lenTest; n++) {
			assertEquals(test.CharAt(n), pbuffer.CharAt(n));
		}
	}

	public void testBadPositionException() {
		SplitText pbuffer = new SplitText();
		pbuffer.AttachStyles(SplitText.CreateStyles(1), null, 0);
		try {
			pbuffer.InsertString(1, test.Data(), 1);
			fail("Should raise an BadPositionException");
		} catch (BadPositionException e) {
		}
	}

	public void testFillRange() {
		SplitVector_1 buffer = new SplitVector_1();
		buffer.Create(20,100);
		int total = 0;
		buffer.InsertString(0, longtest.Data(), 10);
		buffer.FillRange(0, (byte)'x', 10);
		total++;
		for (int l = 0; l < 10; l++) {
			assertEquals('x', buffer.CharAt(l));
		}
	}

	public void testIntegerVector() {
		Partitioning v = new Partitioning(10);
		assertEquals(0, v.Length());
		try {
			v.RemovePartition(0);
			fail("Should raise an BadPositionException");
		} catch (BadPositionException e) {
		}
		try {
			v.InsertPartition(1, 11);
			fail("Should raise an BadPositionException");
		} catch (BadPositionException e) {
		}
		v.InsertPartition(0, 100);
		assertEquals(1, v.Length());
		assertEquals(100, v.PositionFromPartition(0));
		v.SetPartitionStartPosition(0, 200);
		assertEquals(1, v.Length());
		assertEquals(200, v.PositionFromPartition(0));
		v.SetPartitionStartPosition(0, 300);
		assertEquals(1, v.Length());
		assertEquals(300, v.PositionFromPartition(0));
		v.RemovePartition(0);
		assertEquals(0, v.Length());
	}

	public void testGappedIntegerVector() {
		Partitioning v = new Partitioning(10);
		assertEquals(0, v.Length());
		try {
			v.RemovePartition(0);
			fail("Should raise an BadPositionException");
		} catch (BadPositionException e) {
		}
		try {
			v.InsertPartition(1, 11);
			fail("Should raise an BadPositionException");
		} catch (BadPositionException e) {
		}
		v.InsertPartition(0, 100);
		assertEquals(1, v.Length());
		assertEquals(100, v.PositionFromPartition(0));
		v.SetPartitionStartPosition(0, 200);
		assertEquals(1, v.Length());
		assertEquals(200, v.PositionFromPartition(0));
		v.SetPartitionStartPosition(0, 300);
		assertEquals(1, v.Length());
		assertEquals(300, v.PositionFromPartition(0));
		v.RemovePartition(0);
		assertEquals(0, v.Length());
	}

	public void testLineVector() {
		Partitioning v = new Partitioning(10);
		assertEquals(1, v.Partitions());
		v.InsertPartition(1, 100);
		assertEquals(2, v.Partitions());
		assertEquals(100, v.PositionFromPartition(1));
		v.SetPartitionStartPosition(1, 200);
		assertEquals(2, v.Partitions());
		assertEquals(200, v.PositionFromPartition(1));
		v.SetPartitionStartPosition(1, 300);
		assertEquals(2, v.Partitions());
		assertEquals(300, v.PositionFromPartition(1));
		v.RemovePartition(0);
		assertEquals(1, v.Partitions());
	}

	public void showLines(SplitText pbuffer) {
		System.out.print("]" + pbuffer.Lines() + ":");
		for (int i=0;i<=pbuffer.Lines();i++) {
			if (i != 0)
				System.out.print(",");
			System.out.print("" + pbuffer.PositionFromLine(i));
		}
		System.out.print("\n");
	}

	public void testLineInsertionDeletion() {

		// The complexity in the line vector is caused by treating any
		// of \r, \n, and \r\n as line ends so test a variety of these.
		SplitText pbuffer = new SplitText();
		pbuffer.AttachStyles(SplitText.CreateStyles(1), null, 0);
		assertEquals(pbuffer.Lines(), 1);
		assertEquals(pbuffer.PositionFromLine(0), 0);

		InsertString(pbuffer, 0, shortInsert.Data());
		// bc
		assertEquals(pbuffer.Lines(), 1);
		assertEquals(pbuffer.PositionFromLine(0), 0);
		assertEquals(pbuffer.PositionFromLine(1), 2);

		InsertString(pbuffer, 0, CR.Data());
		// \rbc
		assertEquals(pbuffer.Lines(), 2);
		assertEquals(pbuffer.PositionFromLine(0), 0);
		assertEquals(pbuffer.PositionFromLine(1), 1);
		assertEquals(pbuffer.PositionFromLine(2), 3);

		InsertString(pbuffer, 0, LF.Data());
		// \n\rbc
		assertEquals(pbuffer.Lines(), 3);
		assertEquals(pbuffer.PositionFromLine(0), 0);
		assertEquals(pbuffer.PositionFromLine(1), 1);
		assertEquals(pbuffer.PositionFromLine(2), 2);
		assertEquals(pbuffer.PositionFromLine(3), 4);

		// This \r is merged into the previous line end
		InsertString(pbuffer, 0, CR.Data());
		//showLines(pbuffer);
		// \r\n\rbc
		assertEquals(pbuffer.Lines(), 3);
		assertEquals(pbuffer.PositionFromLine(0), 0);
		assertEquals(pbuffer.PositionFromLine(1), 2);
		assertEquals(pbuffer.PositionFromLine(2), 3);
		assertEquals(pbuffer.PositionFromLine(3), 5);

		// As is this \n
		InsertString(pbuffer, 3, LF.Data());
		// \r\n\r\nbc
		assertEquals(pbuffer.Lines(), 3);
		assertEquals(pbuffer.PositionFromLine(0), 0);
		assertEquals(pbuffer.PositionFromLine(1), 2);
		assertEquals(pbuffer.PositionFromLine(2), 4);
		assertEquals(pbuffer.PositionFromLine(3), 6);

		// Separate a \r\n pair
		InsertString(pbuffer, 3, X.Data());
		// \r\n\rx\nbc
		assertEquals(pbuffer.Lines(), 4);
		// Check all the line starts are correct
		assertEquals(pbuffer.PositionFromLine(0), 0);
		assertEquals(pbuffer.PositionFromLine(1), 2);
		assertEquals(pbuffer.PositionFromLine(2), 3);
		assertEquals(pbuffer.PositionFromLine(3), 5);
		assertEquals(pbuffer.PositionFromLine(4), 7);

		// Destroy and recreate the lines data to ensure it is recreated correctly
		pbuffer.DiscardLines();
		assertEquals(pbuffer.Lines(), 4);
		// Check all the line starts are correct
		assertEquals(pbuffer.PositionFromLine(0), 0);
		assertEquals(pbuffer.PositionFromLine(1), 2);
		assertEquals(pbuffer.PositionFromLine(2), 3);
		assertEquals(pbuffer.PositionFromLine(3), 5);
		assertEquals(pbuffer.PositionFromLine(4), 7);

		// Put them back together
		pbuffer.DeleteRange(3, 1);
		// \r\n\r\nbc
		assertEquals(pbuffer.Lines(), 3);

		pbuffer.DeleteRange(0, 1);
		// \n\r\nbc
		assertEquals(pbuffer.Lines(), 3);

		pbuffer.DeleteRange(0, 1);
		// \r\nbc
		assertEquals(pbuffer.Lines(), 2);

		pbuffer.DeleteRange(0, 2);
		// bc
		assertEquals(pbuffer.Lines(), 1);
	}

	public void testUndo() {
		SplitText ub = new SplitText();
		ub.AttachStyles(SplitText.CreateStyles(1), null, 0);
		int lenUtest = utest.Length();
		int lenXYtest = xytest.Length();

		ub.InsertString(0, xytest.Data(), lenXYtest);
		assertEquals(ToString(ub), "xy");
		ub.InsertString(0, utest.Data(), lenUtest);
		assertEquals(ToString(ub), "uxy");
		ub.DeleteRange(1, 1);
		assertEquals(ToString(ub), "uy");
		ub.PerformUndoAction();
		assertEquals(ToString(ub), "uxy");
		ub.PerformRedoAction();
		assertEquals(ToString(ub), "uy");

		// Undo to beginning and redo again
		ub.PerformUndoAction();
		assertEquals(ToString(ub), "uxy");
		ub.PerformUndoAction();
		assertEquals(ToString(ub), "xy");
		ub.PerformUndoAction();
		assertEquals(ToString(ub), "");
		ub.PerformRedoAction();
		assertEquals(ToString(ub), "xy");
		ub.PerformRedoAction();
		assertEquals(ToString(ub), "uxy");
		ub.PerformRedoAction();
		assertEquals(ToString(ub), "uy");

		// Do it again. Undo to beginning and redo again
		ub.PerformUndoAction();
		assertEquals(ToString(ub), "uxy");
		ub.PerformUndoAction();
		assertEquals(ToString(ub), "xy");
		ub.PerformUndoAction();
		assertEquals(ToString(ub), "");
		ub.PerformRedoAction();
		assertEquals(ToString(ub), "xy");
		ub.PerformRedoAction();
		assertEquals(ToString(ub), "uxy");
		ub.PerformRedoAction();
		assertEquals(ToString(ub), "uy");

		// Add a new step in
		ub.PerformUndoAction();
		assertEquals(ToString(ub), "uxy");
		ub.DeleteRange(0, 1);
		assertEquals(ToString(ub), "xy");
		ub.PerformUndoAction();
		assertEquals(ToString(ub), "uxy");
		ub.PerformUndoAction();
		assertEquals(ToString(ub), "xy");
		ub.PerformUndoAction();
		assertEquals(ToString(ub), "");
	}

	public void testUndoBatching() {
		SplitText ub = new SplitText();
		ub.AttachStyles(SplitText.CreateStyles(1), null, 0);
		int lenUtest = utest.Length();
		// Checking for batching
		// Typing one character then another after should be one step
		ub.InsertString(0, utest.Data(), lenUtest);
		assertEquals(ub.StartUndo(), 1);
		assertEquals(ToString(ub), "u");
		ub.InsertString(1, utest.Data(), lenUtest);
		assertEquals(ub.StartUndo(), 2);
		assertEquals(ToString(ub), "uu");
		ub.PerformUndoStep();
		assertEquals(ToString(ub), "");
		ub.PerformRedoStep();
		assertEquals(ToString(ub), "uu");
		// This one should not merge as it is after a redo
		ub.InsertString(2, utest.Data(), lenUtest);
		assertEquals(ub.StartUndo(), 1);
		assertEquals(ToString(ub), "uuu");
		// This one should not merge as it is a delete and previous was insert
		ub.DeleteRange(0, 1);
		assertEquals(ub.StartUndo(), 1);
		assertEquals(ToString(ub), "uu");
		// This one should merge as it is a delete and next to previous
		ub.DeleteRange(0, 1);
		assertEquals(ub.StartUndo(), 2);
		assertEquals(ToString(ub), "u");
		ub.PerformUndoStep();
		assertEquals(ToString(ub), "uuu");
		// This one should not merge as it is after an undo
		ub.DeleteRange(2, 1);
		assertEquals(ub.StartUndo(), 1);
		assertEquals(ToString(ub), "uu");
		// This one should merge as it is a delete and next to previous
		ub.DeleteRange(1, 1);
		assertEquals(ub.StartUndo(), 2);
		assertEquals(ToString(ub), "u");
	}
}