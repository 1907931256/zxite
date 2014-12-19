// SinkWorld demonstration program - C# version
// Copyright 2001 Neil Hodgson
using System;

[assembly: CLSCompliantAttribute(true)]
class ms {

	public static void DisplayHTML(SplitText sbuffer) {
		int prevStyle = 0;
		SWTest.Write("<html>\n<head>\n");
		SWTest.Write("<link rel=\"stylesheet\" type=\"text/css\" href=\"Source.css\">\n");
		SWTest.Write("</head>\n<body>\n");
		for (int o=0; o<sbuffer.Length(); o++) {
			int styleNow = sbuffer.StyleAt(o);
			if (styleNow != prevStyle) {
				SWTest.Write("</span><span class=\"S");
				SWTest.Write(styleNow);
				SWTest.Write("\">");
				prevStyle = styleNow;
			}
			byte ch = sbuffer.CharAt(o);
			if (ch == '\n')
				SWTest.Write("<br />");
			else if (ch == '\t')
				SWTest.Write("&nbsp;&nbsp;&nbsp;&nbsp;");
			else if (ch == ' ')
				SWTest.Write("&nbsp;");
			else if (ch == '<')
				SWTest.Write("&lt;");
			else if (ch == ' ')
				SWTest.Write("&gt;");
			else if (ch != '\r')
				SWTest.Write(ch);
		}
		SWTest.Write("</span>\n</body>\n</html>\n");
	}

	public static void assertValue(SplitText p, String s) {
		int len = p.Length();
		SWTest.Assert(len == s.Length);
		byte[] v = new byte[s.Length];
		p.RetrieveString(0, v, len);
		for (int i=0; i<len; i++)
			SWTest.Assert((byte)s[i] == v[i]);
	}

	public static void showUB(UndoStack p) {
	}

	public static void testUndoRedo() {
		SplitText ub = new SplitText();
		ub.AttachStyles(SplitText.CreateStyles(1), null, 0);
		showUB(ub.us);
		Literal utest = new Literal("u");
		int lenUtest = utest.Length();
		Literal xytest = new Literal("xy");
		int lenXYtest = xytest.Length();
		ub.InsertString(0, xytest.Data(), lenXYtest);
		assertValue(ub, "xy");
		showUB(ub.us);
		ub.InsertString(0, utest.Data(), lenUtest);
		assertValue(ub, "uxy");
		showUB(ub.us);
		ub.DeleteRange(1, 1);
		assertValue(ub, "uy");
		showUB(ub.us);
		ub.PerformUndoAction();
		assertValue(ub, "uxy");
		showUB(ub.us);
		ub.PerformRedoAction();
		assertValue(ub, "uy");
		showUB(ub.us);

		// Undo to beginning and redo again
		ub.PerformUndoAction();
		assertValue(ub, "uxy");
		showUB(ub.us);
		ub.PerformUndoAction();
		assertValue(ub, "xy");
		showUB(ub.us);
		ub.PerformUndoAction();
		assertValue(ub, "");
		showUB(ub.us);
		ub.PerformRedoAction();
		assertValue(ub, "xy");
		showUB(ub.us);
		ub.PerformRedoAction();
		assertValue(ub, "uxy");
		showUB(ub.us);
		ub.PerformRedoAction();
		assertValue(ub, "uy");
		showUB(ub.us);

		// Do it again. Undo to beginning and redo again
		ub.PerformUndoAction();
		assertValue(ub, "uxy");
		showUB(ub.us);
		ub.PerformUndoAction();
		assertValue(ub, "xy");
		showUB(ub.us);
		ub.PerformUndoAction();
		assertValue(ub, "");
		showUB(ub.us);
		ub.PerformRedoAction();
		assertValue(ub, "xy");
		showUB(ub.us);
		ub.PerformRedoAction();
		assertValue(ub, "uxy");
		showUB(ub.us);
		ub.PerformRedoAction();
		assertValue(ub, "uy");
		showUB(ub.us);

		// Add a new step in
		ub.PerformUndoAction();
		assertValue(ub, "uxy");
		showUB(ub.us);
		ub.DeleteRange(0, 1);
		assertValue(ub, "xy");
		showUB(ub.us);
		ub.PerformUndoAction();
		assertValue(ub, "uxy");
		showUB(ub.us);
	}

	public static void Main(String[] args) {
		SWTest.WriteLine("Start test");

		WordList wl = new WordList(false);
		Literal keywords = new Literal("alpha \u00a8theta gamma");
		wl.Set(keywords.Data(), keywords.Length());
		Literal alpha = new Literal("alpha");
		Literal theta = new Literal("\u00a8theta");
		Literal gamma = new Literal("gamma");
		Literal delta = new Literal("delta");
		Literal large = new Literal("\u00a0large");
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

		SplitVector_1 buffer = new SplitVector_1();
		buffer.Create(20,100);
		SWTest.Assert(buffer.Length() == 0);
		Literal test = new Literal("abcde");
		try {
			buffer.InsertString(1, test.Data(), 1);
		} catch (BadPositionException) {
			SWTest.Write("Caught\n");
		}
		int lenTest = test.Length();
		buffer.InsertString(0, test.Data(), lenTest);
		SWTest.Assert(buffer.Length() == lenTest);
		byte[] ret = new byte[3];
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
		Literal shortInsert = new Literal("bc");
		buffer.InsertString(1, shortInsert.Data(), 2);
		SWTest.Assert(buffer.Length() == lenTest);
		for (int k = 0; k < lenTest; k++) {
			SWTest.Assert(test.CharAt(k) == buffer.CharAt(k));
			SWTest.Write(k);
			SWTest.Write(":");
			SWTest.Write(buffer.CharAt(k));
			SWTest.Write(" ");
		}
		SWTest.Write("\n");
		double st = SWTest.Now();
		Literal longtest = new Literal("ghijklmnopqrstuvwxyz");
		for (int l = 0; l < 10000; l++) {
			for (int m = 0; m < 20; m++) {
				buffer.InsertString(1, longtest.Data(), m + 1);
			}
		}

		SplitText pbuffer = new SplitText();
		pbuffer.AttachStyles(SplitText.CreateStyles(1), null, 0);
		//pbuffer.DiscardLines();
		SWTest.Assert(pbuffer.Length() == 0);
		//SWTest.Assert(pbuffer.Lines() == 1);
		pbuffer.InsertString(0, test.Data(), lenTest);
		SWTest.Assert(pbuffer.Length() == lenTest);
		for (int n = 0; n < lenTest; n++) {
			SWTest.Assert(test.CharAt(n) == pbuffer.CharAt(n));
			SWTest.Write(n);
			SWTest.Write(":");
			SWTest.Write(pbuffer.CharAt(n));
			SWTest.Write(" ");
		}
		SWTest.Write("\n");

		double t2 = SWTest.Now();
		SWTest.Write(t2-st);
		SWTest.Write(" seconds for Insertion test\n");

		SplitText sbuffer = new SplitText();
		IStyles sstyles = SplitText.CreateStyles(1);
		sbuffer.AttachStyles(sstyles, null, 0);
		//sbuffer.DiscardLines();
		Literal prog = new Literal("//// A demonstration program\r\nint main() {\r\n\tprintf(\"hello world %d\\n\", 9);\r\n}\r\n");
		int lenProg = prog.Length();
		sbuffer.InsertString(0, prog.Data(), lenProg);
		//SWTest.Assert(sbuffer.Lines() == 5);
		Styler sa = new Styler(sbuffer, sstyles, 0, sbuffer.Length());
		LexCPP lx = new LexCPP();
		WordList wlCPP = new WordList(false);
		Literal keywordsCPP = new Literal(
			"abstract as base bool break byte case catch char " +
			"checked class const continue decimal default " +
			"delegate do double else enum event explicit " +
			"extern false finally fixed float for foreach goto " +
			"if implicit in int interface internal is lock long " +
			"namespace new null object operator out override " +
			"params private protected public readonly ref " +
			"return sbyte sealed short sizeof stackalloc " +
			"static string struct switch this throw true try " +
			"typeof uint ulong unchecked unsafe ushort " +
			"using virtual void while");
		lx.SetKeyWords(0, keywordsCPP);
		lx.Colourise(sa);
		//DisplayHTML(sbuffer);

		// Start buffer as big as needed to avoid measuring allocation and copying
		SplitText bigbuffer = new SplitText(300000,1024);
		IStyles bigstyles = SplitText.CreateStyles(1);
		bigbuffer.AttachStyles(bigstyles, null, 0);
		MarkerSet ms2 = new MarkerSet();
		bigbuffer.AddPerLineData(ms2);
		//bigbuffer.DiscardLines();
		// prog is about 80 characters so create a realistic large 120K document
		for (int q=0; q<3000; q++) {
			bigbuffer.InsertString(lenProg*(q/2), prog.Data(), lenProg);
		}
		for (int q2=0; q2<1500; q2++) {
			bigbuffer.DeleteRange(lenProg*(q2/2), lenProg);
		}
		SWTest.Assert(bigbuffer.Length() == 1500 * lenProg);
		SWTest.Assert(bigbuffer.Lines() == 1500 * 4 + 1);
		double t3 = SWTest.Now();
		SWTest.Write(t3-t2);
		SWTest.Write(" seconds for Lines Insertion Deletion test\n");

		// Run 50 times to get a large enough time to measure Colourise operation
		for (int p=0; p<50; p++) {
			Styler sabig = new Styler(bigbuffer, bigstyles, 0, bigbuffer.Length());
			lx.Colourise(sabig);
		}

		double t31 = SWTest.Now();
		SWTest.Write((t31-t3) / 50.0);
		SWTest.Write(" seconds for Colourise test\n");

		int biglength = bigbuffer.Length();

		// Search for "printf"
		Literal printfs = new Literal("printf");
		int lenPrintfs = printfs.Length();
		for (int r=0; r<50; r++) {
			int count = 0;
			for (int s=0; s<biglength; s++) {
				if (bigbuffer.CharAt(s) == printfs.CharAt(0)) {
					int found = 1;
					for (int t=1; t<lenPrintfs && found==1; t++) {
						if (bigbuffer.CharAt(s+t) != printfs.CharAt(t)) {
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
		Literal replace = new Literal("OutputTrace");
		int lenReplace = replace.Length();
		for (int u=0; u<50; u++) {
			int count = 0;
			for (int s=0; s<biglength; s++) {
				if (bigbuffer.CharAt(s) == printfs.CharAt(0)) {
					int found = 1;
					for (int t=1; t<lenPrintfs && found==1; t++) {
						if (bigbuffer.CharAt(s+t) != printfs.CharAt(t)) {
							found = 0;
						}
					}
					if (found==1) {
						count++;
						bigbuffer.DeleteRange(s, lenPrintfs);
						bigbuffer.InsertString(s, replace.Data(), lenReplace);
						biglength += lenReplace - lenPrintfs;
					}
				}
			}
			SWTest.Assert(count == 1500);
			count = 0;
			for (int s=0; s<biglength; s++) {
				if (bigbuffer.CharAt(s) == replace.CharAt(0)) {
					int found = 1;
					for (int t=1; t<lenReplace && found==1; t++) {
						if (bigbuffer.CharAt(s+t) != replace.CharAt(t)) {
							found = 0;
						}
					}
					if (found==1) {
						count++;
						bigbuffer.DeleteRange(s, lenReplace);
						bigbuffer.InsertString(s, printfs.Data(), lenPrintfs);
						biglength += lenPrintfs - lenReplace;
					}
				}
			}
			SWTest.Assert(count == 1500);
		}

		double en = SWTest.Now();
		SWTest.Write((en-t4) / 100.0);
		SWTest.Write(" seconds for Replace test\n");
	}
};
