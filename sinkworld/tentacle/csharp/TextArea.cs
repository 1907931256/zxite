using System;
using System.Diagnostics;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using SinkWorld;
/// <summary> TextArea copyright 2005 Neil Hodgson.
/// </summary>
public class TextArea : Control {
	private TentacleControl panel;
	Timer backgroundTimer;
	public TextView tv;
	public RangeSelection sr;
	int selType;
	long doubleClickTime = 5000000;
	long timeLastCaret = 0;
	long timeLastClick = 0;
	Point lastClickAt = new Point(0, 0);
	Point mouseDragLast = new Point(0, 0);
	internal int nextLineWrapped = 0;
	internal int goodTopLine = 0;
	Literal endOfLine = new Literal("\r\n");

	public TextArea(TentacleControl panel_):base() {
		panel = panel_;
		Paint += new PaintEventHandler(Contents_Paint);
		MouseDown += new MouseEventHandler(Me_MouseDown);
		MouseMove += new MouseEventHandler(Me_MouseMove);
		MouseUp += new MouseEventHandler(Me_MouseUp);
		MouseWheel += new MouseEventHandler(Me_MouseWheel);
		KeyDown += new KeyEventHandler(Me_KeyDown);
		KeyPress += new KeyPressEventHandler(Me_KeyPressed);
		SizeChanged += new EventHandler(Me_Resize);
		GotFocus += new EventHandler(FocusGained);
		LostFocus += new EventHandler(FocusLost);
		
		backgroundTimer = new Timer();
		backgroundTimer.Tick += new EventHandler(Me_Background);
		backgroundTimer.Interval = 50;
		backgroundTimer.Start();
		
		EventHandler clicky = new EventHandler(this.MenuClick);
		ContextMenu = new ContextMenu();
		ContextMenu.MenuItems.Clear();
		ContextMenu.MenuItems.Add(new MenuItem("Undo", clicky));
		ContextMenu.MenuItems.Add(new MenuItem("Redo", clicky));
		ContextMenu.MenuItems.Add(new MenuItem("-", clicky));
		ContextMenu.MenuItems.Add(new MenuItem("Cut", clicky));
		ContextMenu.MenuItems.Add(new MenuItem("Copy", clicky));
		ContextMenu.MenuItems.Add(new MenuItem("Paste", clicky));
		ContextMenu.MenuItems.Add(new MenuItem("Delete", clicky));
		ContextMenu.MenuItems.Add(new MenuItem("-", clicky));
		ContextMenu.MenuItems.Add(new MenuItem("Select All", clicky));

		tv = new TextView();
		sr = new RangeSelection();

		//SetStyle(ControlStyles.AllPaintingInWmPaint | ControlStyles.UserPaint | ControlStyles.DoubleBuffer, true);
		SetStyle(ControlStyles.ResizeRedraw | ControlStyles.Opaque | ControlStyles.DoubleBuffer |
			ControlStyles.AllPaintingInWmPaint | ControlStyles.UserPaint, true);
	}

	private void Contents_Paint(object sender, PaintEventArgs e) {
		Graphics g = e.Graphics;
		Rectangle rc = Rectangle.Truncate(g.ClipBounds);
		Rectangle rcBounds = Bounds;
		rc.Intersect(rcBounds);
		SRectangle rcPaint = new SRectangle();
		rcPaint.left = rc.X;
		rcPaint.top = rc.Y;
		rcPaint.right = rc.X + rc.Width;
		rcPaint.bottom = rc.Y + rc.Height;
		Surface surface = new Surface(g);

		tv.PaintRect(surface, rcPaint);

		panel.SetScrollBars();

	}

	public int PositionFromLocation(Point pt) {
		Surface surface = new Surface(CreateGraphics());
		return tv.PositionFromSurfaceLocation(surface, pt);
	}

	public void InvalidateRange(int a, int b) {
		if (a < 0)
			a = 0;
		if (b < 0)
			b = 0;
		if (a > b) {
			int t = b;
			b = a;
			a = t;
		}
		int lineTop = tv.DisplayLineFromPosition(a);
		int lineDocBottom = panel.pdoc.pbuffer.LineFromPosition(b) + 1;
		int lineBottom = tv.fs.DisplayFromDoc(lineDocBottom);
		SRectangle rcInvalidate = new SRectangle();
		rcInvalidate.left = 0;
		rcInvalidate.right = 2000;
		rcInvalidate.top = (lineTop - panel.ScrollPos()) * tv.lineHeight;
		rcInvalidate.bottom = (lineBottom - panel.ScrollPos()) * tv.lineHeight;
		// Allow for decorators that go 2 pixels beyond line.
		rcInvalidate.top -= 2;
		rcInvalidate.bottom += 2;
		Invalidate(new Rectangle(rcInvalidate.left, rcInvalidate.top,
			rcInvalidate.Width(), rcInvalidate.Height()));
	}

	public int InsertBytes(int position, byte[] s, int insertLength) {
		int lenInText = insertLength;
		panel.pdoc.pbuffer.InsertBytes(position, s, insertLength);
		tv.fs.InvalidateWidths();
		panel.frame.StateAltered();
		if (panel.wrap) {
			panel.StartWrap();
		}
		return lenInText;
	}

	public int InsertUTF32(int position, int[] s, int insertLength) {
		int lenInText = panel.pdoc.pbuffer.InsertUTF32(position, s, insertLength);
		tv.fs.InvalidateWidths();
		panel.frame.StateAltered();
		if (panel.wrap) {
			panel.StartWrap();
		}
		return lenInText;
	}

	public bool DeleteRange(int position, int deleteLength) {
		int lineStart = panel.pdoc.pbuffer.LineFromPosition(position);
		int lineEnd = panel.pdoc.pbuffer.LineFromPosition(position + deleteLength);
		panel.pdoc.pbuffer.DeleteRange(position, deleteLength);
		tv.fs.InvalidateWidths();
		panel.frame.StateAltered();
		return lineStart != lineEnd;
	}

	public bool CheckWrap() {
		if (nextLineWrapped < panel.pdoc.pbuffer.Lines()) {
			bool redisplay = false;
			SRectangle rcClient = new SRectangle();
			panel.GetViewRectangle(rcClient);
			tv.lineWidth = rcClient.right - rcClient.left;
			if (panel.wrap) {
				int minBytesToWrap = tv.lineWidth / tv.tabWidth + 1;
				//ElapsedTime et;
				panel.pdoc.pbuffer.LexTo(panel.pdoc.pbuffer.Length(), 0, false);
				// Get a surface
				Surface surface = new Surface(CreateGraphics());
				for (int line=0;line<200 && nextLineWrapped < panel.pdoc.pbuffer.Lines();line++) {
					if (nextLineWrapped == goodTopLine)
						redisplay = true;
					tv.WrapLine(surface, nextLineWrapped);
					nextLineWrapped++;
				}
			} else {
				while (nextLineWrapped < panel.pdoc.pbuffer.Lines()) {
					tv.fs.SetHeight(nextLineWrapped, 1);
					nextLineWrapped++;
				}
			}
			if (redisplay) {
				Invalidate();
				panel.ScrollTo(tv.fs.DisplayFromDoc(goodTopLine));
			}
			if (!(nextLineWrapped < panel.pdoc.pbuffer.Lines())) {
				panel.ScrollTo(tv.fs.DisplayFromDoc(goodTopLine));
				panel.SetScrollBars();
				Invalidate();
			}
		}
		return nextLineWrapped < panel.pdoc.pbuffer.Lines();
	}

	void LineDuplicate() {
		int line = panel.pdoc.pbuffer.LineFromPosition(sr.Caret());
		int start = panel.pdoc.pbuffer.PositionFromLine(line);
		int end = panel.pdoc.pbuffer.LineEndPositionFromLine(line);
		int len = end - start;
		int[] thisLine = new int[len];
		int lenCharacters = panel.pdoc.pbuffer.RetrieveUTF32(start, thisLine, len);
		int lenInsert = InsertUTF32(end, endOfLine.Data(), endOfLine.Length());
		InsertUTF32(end + lenInsert, thisLine, lenCharacters);
		Invalidate();
	}

	private void Me_KeyPressed(Object sender, KeyPressEventArgs e) {
		if ((char) e.KeyChar == '\n') {
		} else if ((char) e.KeyChar == '\b') {
		} else {
			try {
				String s = "" + e.KeyChar;
				byte[] bytes = Encoding.UTF8.GetBytes(s);
				if ((bytes.Length > 1) || (e.KeyChar >= ' ')) {
					if (!sr.Empty()) {
						panel.Clear();
						Invalidate();
					}
					//Console.Out.WriteLine("Character typed" + (char) e.KeyChar + ":" + bytes.Length);
					int []charUTF32 = { bytes[0] };
					int len = InsertUTF32(sr.Caret(), charUTF32, 1);
					panel.CheckLineForInvalidate(sr.Caret());
				}
			}
			catch (System.IO.IOException) {
			}
		}
	}
	protected override bool IsInputKey(Keys keyData) {
		return true;
	}

	private void Me_KeyDown(Object sender, KeyEventArgs e) {
		//~ System.err.println("Character pressed" + e.getKeyCode() + " " + e.getModifiers());
		bool control = e.Control;
		bool shift = e.Shift;
		int keyCode = (int) e.KeyValue;
		bool consumed = true;
		if (keyCode == (int) Keys.Add && control) {
			tv.ModifyMagnification(1);
			Invalidate();
		} else if (keyCode == (int) Keys.Subtract && control) {
			tv.ModifyMagnification(-1);
			Invalidate();
		} else if (keyCode == (int) Keys.Left && control) {
			panel.MoveCaret(panel.pdoc.NextWordStart(sr.Caret()-1, -1), shift);
		} else if (keyCode == (int) Keys.Left) {
			int newCaret = panel.pdoc.pbuffer.MovePositionOutsideChar(sr.Caret()-1, -1, true);
			int line = panel.pdoc.pbuffer.LineFromPosition(newCaret);
			if (!tv.fs.GetVisible(line))
				newCaret = panel.pdoc.pbuffer.LineEndPositionFromLine(
					tv.fs.DocFromDisplay(tv.fs.DisplayFromDoc(line) - 1));
			panel.MoveCaret(newCaret, shift);
		} else if (keyCode == (int) Keys.Right && control) {
			panel.MoveCaret(panel.pdoc.NextWordStart(sr.Caret()+1, 1), shift);
		} else if (keyCode == (int) Keys.Right) {
			int newCaret = panel.pdoc.pbuffer.MovePositionOutsideChar(sr.Caret()+1, 1, true);
			int line = panel.pdoc.pbuffer.LineFromPosition(newCaret);
			if (!tv.fs.GetVisible(line))
				newCaret = panel.pdoc.pbuffer.PositionFromLine(
					tv.fs.DocFromDisplay(tv.fs.DisplayFromDoc(line)));
			panel.MoveCaret(newCaret, shift);
		} else if (keyCode == (int) Keys.Home) {
			int line = control?0:panel.pdoc.pbuffer.LineFromPosition(sr.Caret());
			panel.MoveCaret(panel.pdoc.pbuffer.PositionFromLine(line), shift);
		} else if (keyCode == (int) Keys.End) {
			int line = control?panel.pdoc.pbuffer.Lines():panel.pdoc.pbuffer.LineFromPosition(sr.Caret());
			panel.MoveCaret(panel.pdoc.pbuffer.LineEndPositionFromLine(line), shift);
		} else if (keyCode == (int) Keys.Up) {
			int lineDisplay = panel.pdoc.pbuffer.LineFromPosition(sr.Caret()) - 1;
			if (lineDisplay < 0) {
				lineDisplay = 0;
			}
			panel.MoveCaret(panel.pdoc.pbuffer.PositionFromLine(
				tv.fs.DocFromDisplay(lineDisplay)), shift);
		} else if (keyCode == (int) Keys.PageUp) {
			int lineDisplay = tv.DisplayLineFromPosition(sr.Caret()) - panel.PageSize();
			if (lineDisplay < 0) {
				lineDisplay = 0;
			}
			panel.MoveCaret(panel.pdoc.pbuffer.PositionFromLine(
				tv.fs.DocFromDisplay(lineDisplay)), shift);
		} else if (keyCode == (int) Keys.Down) {
			int lineDisplay = tv.DisplayLineFromPosition(sr.Caret()) + 1;
			if (lineDisplay > tv.fs.DisplayFromDoc(panel.pdoc.pbuffer.Lines())) {
				lineDisplay = tv.fs.DisplayFromDoc(panel.pdoc.pbuffer.Lines());
			}
			panel.MoveCaret(panel.pdoc.pbuffer.PositionFromLine(
				tv.fs.DocFromDisplay(lineDisplay)), shift);
		} else if (keyCode == (int) Keys.PageDown) {
			int lineDisplay = tv.DisplayLineFromPosition(sr.Caret()) + panel.PageSize();
			if (lineDisplay > tv.fs.DisplayFromDoc(panel.pdoc.pbuffer.Lines())) {
				lineDisplay = tv.fs.DisplayFromDoc(panel.pdoc.pbuffer.Lines());
			}
			panel.MoveCaret(panel.pdoc.pbuffer.PositionFromLine(
				tv.fs.DocFromDisplay(lineDisplay)), shift);
		} else if (keyCode == (int) Keys.Back) {
			if (control) {
				int startDelete = panel.pdoc.NextWordStart(sr.Caret()-1, -1);
				if (DeleteRange(startDelete, sr.Caret()-startDelete)) {
					Invalidate();
				}
				panel.CheckLineForInvalidate(sr.Caret());
			} else if (sr.Empty()) {
				if (sr.Caret() > 0) {
					if (DeleteRange(sr.Caret() - 1, 1)) {
						Invalidate();
					}
					panel.CheckLineForInvalidate(sr.Caret());
				}
			} else {
				panel.Clear();
			}
		} else if (keyCode == (int) Keys.Delete) {
			if (control) {
				int endDelete = panel.pdoc.NextWordStart(sr.Caret(), 1);
				if (DeleteRange(sr.Caret(), endDelete-sr.Caret())) {
					Invalidate();
				}
				panel.CheckLineForInvalidate(sr.Caret());
			} else if (sr.Empty()) {
				if (sr.Caret() < panel.pdoc.pbuffer.Length()) {
					int endDelete = panel.pdoc.MovePositionOutsideChar(sr.Caret() + 1, 1, true);
					if (DeleteRange(sr.Caret(), endDelete-sr.Caret())) {
						Invalidate();
					}
					panel.CheckLineForInvalidate(sr.Caret());
				}
			} else {
				panel.Clear();
			}
		} else if (keyCode == (int) Keys.Escape) {
		} else if (keyCode == (int) Keys.Enter) {
			if (!sr.Empty()) {
				panel.Clear();
			}
			InsertUTF32(sr.Caret(), endOfLine.Data(), endOfLine.Length());
			//Debug.Assert(pbuffer.PositionFromLine(pbuffer.Lines()) == pbuffer.Length());
			Invalidate();
		} else if (keyCode == 'D' && control) {
			LineDuplicate();
		} else {
			consumed = false;
		}
		e.Handled = consumed;
	}

	bool IsWordCharacter(int ch) {
		return (ch >= 128) ||
			(ch == '_') ||
			(ch >= 'a' && ch <= 'z') ||
			(ch >= 'A' && ch <= 'Z') ||
			(ch >= '0' && ch <= '9');
	}

	public Literal LiteralFromRange(int position, int length) {
		int len = panel.pdoc.pbuffer.Characters(position, length);
		int[] utf = new int[len];
		panel.pdoc.pbuffer.RetrieveUTF32(position, utf, len);
		return new Literal(utf, 0, len);
	}

	public Literal WordAtPosition(int position) {
		int start = panel.pdoc.ExtendWordSelect(position, -1);
		int end = panel.pdoc.ExtendWordSelect(position, 1);
		return LiteralFromRange(start, end - start);
	}

	void ShowFeedBack(int position, bool mouseDown, bool showDown) {
		int start = panel.pdoc.ExtendWordSelect(position, -1);
		if (panel.feedBack.ChangeState(start, mouseDown, showDown)) {
			int end = panel.pdoc.ExtendWordSelect(position, 1);
			panel.feedBack.word = WordAtPosition(position);
			/*
			panel.pstyleFeedBack.DeleteAll();
			panel.pstyleFeedBack.InsertSpace(0, panel.pdoc.pbuffer.Length());
			if (panel.feedBack.show && (panel.feedBack.word.Length != 0)) {
				//~ System.out.println("ShowFeedBack " + position + " : " + mouseDown + " : " +
				//~ showDown + " " + feedBack.word + ":" + end);
				panel.pstyleFeedBack.FillRange(start, panel.feedBack.down?2:1, end - start);
			}
			*/
			Invalidate();
		}
	}

	public virtual void mouseClicked(Object event_sender, EventArgs e) {
	}

	private void MenuClick(Object sender, EventArgs e) {
		String command = ((MenuItem)sender).Text;
		if (command.Equals("Undo")) {
			panel.Undo();
		} else if (command.Equals("Redo")) {
			panel.Redo();
		} else if (command.Equals("Cut")) {
			panel.Cut();
		} else if (command.Equals("Copy")) {
			panel.Copy();
		} else if (command.Equals("Paste")) {
			panel.Paste();
		} else if (command.Equals("Delete")) {
			panel.Delete();
		} else if (command.Equals("Select All")) {
			panel.SelectAll();
		}
	}
	
	private void Me_MouseDown(Object sender, MouseEventArgs e) {
		Focus();
		if (e.Button == MouseButtons.Right) {
			ContextMenu.Show(this, new System.Drawing.Point(e.X, e.Y));
			return;
		}
		Point pt = new Point(e.X, e.Y);
		long timeMessage = DateTime.Now.Ticks;

		//~ Console.Error.WriteLine("Mouse pressed " + e.X + "," + e.Y + " @ " + timeMessage);

		int posPoint = PositionFromLocation(pt);
		if (tv.ColumnTypeFromLocation(pt) == TextColumn.tcFold) {
			panel.FoldLine(panel.pdoc.pbuffer.LineFromPosition(posPoint), panel.fcDiscover);
			return;
		}
		bool altDown = (Control.ModifierKeys & Keys.Control) != 0;
		/*
		if (panel.pstyleFeedBack.ValueAt(posPoint) != 0) {
			ShowFeedBack(posPoint, true, altDown);
			return;
		}
		*/
		mouseDragLast = pt;
		if (((timeMessage - timeLastClick) < doubleClickTime) &&
			(lastClickAt.DistanceSquared(pt) < 20)) {
			selType = (selType + 1) % 3;
		} else {
			selType = 0;
		}
		if (selType == 0) {
			panel.MoveCaret(posPoint, false);
		} else if (selType == 1) {	// Word selection
			panel.SetSelection(panel.pdoc.ExtendWordSelect(posPoint, -1),
				panel.pdoc.ExtendWordSelect(posPoint, 1));
			Invalidate();
			panel.EnsureCaretVisible();
		} else if (selType == 2) {	// Line selection
			int line = panel.pdoc.pbuffer.LineFromPosition(posPoint);
			panel.SetSelection(panel.pdoc.pbuffer.PositionFromLine(line),
				panel.pdoc.pbuffer.LineEndPositionFromLine(line));
			Invalidate();
			panel.EnsureCaretVisible();
		}
		lastClickAt = pt;
		timeLastClick = timeMessage;
	}

	private void Me_MouseUp(Object sender, MouseEventArgs e) {
		Point pt = new Point(e.X, e.Y);
		if (panel.feedBack.down) {
			int posPoint = PositionFromLocation(pt);
			bool altDown = (Control.ModifierKeys & Keys.Control) != 0;
			ShowFeedBack(posPoint, false, altDown);
			Console.Out.WriteLine("Click " + panel.feedBack.word);
		}
	}

	private void Me_MouseWheel(Object sender, MouseEventArgs e) {
		//Console.Out.WriteLine("Wheel " + e);
		panel.ScrollTo(tv.topLine - (3 * e.Delta / 120));
	}

	private void Me_MouseMove(Object sender, MouseEventArgs e) {
		Point pt = new Point(e.X, e.Y);
		//~ System.out.println("mouseMoved " + e.getModifiers());
		if (e.Button == MouseButtons.Left) {
			if (!panel.feedBack.down) {
				if (!(pt.equals(mouseDragLast))) {
					mouseDragLast = pt;
					panel.MoveCaret(PositionFromLocation(mouseDragLast), true);
				}
			}
		} else if (e.Button == MouseButtons.None) {
			bool altDown = (Control.ModifierKeys & Keys.Control) != 0;
			if (altDown || panel.feedBack.down) {
				ShowFeedBack(PositionFromLocation(pt), false, altDown);
			}
		}
	}

	private void Me_Resize(Object sender, EventArgs e) {
		SRectangle rcClient = new SRectangle();
		panel.GetViewRectangle(rcClient);
		tv.SetRectangle(rcClient);
		if (nextLineWrapped >= panel.pdoc.pbuffer.Lines()) {
			goodTopLine = tv.fs.DocFromDisplay(tv.topLine);
		}
		nextLineWrapped = 0;
		CheckWrap();
		//Console.WriteLine("Idling");
	}

	internal void CaretOn() {
		long timeEvent = DateTime.Now.Ticks;
		timeLastCaret = timeEvent;
		sr.SetOn(true);
		InvalidateRange(sr.Caret(), sr.Caret());
	}

	private void Me_Background(Object sender, EventArgs e) {
		CheckWrap();
		long timeEvent = DateTime.Now.Ticks;
		if (timeEvent > (timeLastCaret + 5000000)) {
			timeLastCaret = timeEvent;
			sr.SetOn(!sr.IsOn());
			InvalidateRange(sr.Caret(), sr.Caret());
		}
	}

	public bool hasFocus() {
		return Focused;
	}

	public void FocusGained(object sender, EventArgs e) {
		panel.SetSelectionStyles();
		Invalidate();
		panel.frame.StateAltered();
	}

	public void FocusLost(object sender, EventArgs e) {
		panel.SetSelectionStyles();
		Invalidate();
		panel.frame.StateAltered();
	}
}
