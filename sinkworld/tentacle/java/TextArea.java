package SinkWorld;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;

public class TextArea extends JPanel
	implements KeyListener, MouseListener, MouseMotionListener, MouseWheelListener,
	AdjustmentListener, ComponentListener, FocusListener, ActionListener {

	private TentacleControl panel;
	public SinkWorld.TextView tv;
	public SinkWorld.RangeSelection sr;
	JPopupMenu popup;
	int selType;
	long doubleClickTime = 500;
	long timeEvent = 0;
	long timeLastCaret = 0;
	long timeLastClick = 0;
	Point lastClickAt = new Point(0, 0);
	Point mouseDragLast = new Point(0, 0);
	int nextLineWrapped;
	int goodTopLine = 0;
	Literal endOfLine = new Literal("\r\n");

	TextArea(TentacleControl panel_) {
		super(true);
		panel = panel_;
		addKeyListener(this);
		addMouseListener(this);
		addMouseMotionListener(this);
		addMouseWheelListener(this);
		addComponentListener(this);
		addFocusListener(this);
		ActionListener taskPerformer = new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				CheckWrap();
				timeEvent = e.getWhen();
				if (timeEvent > (timeLastCaret + 500)) {
					timeLastCaret = timeEvent;
					sr.SetOn(!sr.IsOn());
					InvalidateRange(sr.Caret(), sr.Caret());
				}
			}
		};
		new Timer(50, taskPerformer).start();
		tv = new SinkWorld.TextView();
		sr = new SinkWorld.RangeSelection();
		popup = new JPopupMenu();
		addPopupItem("Undo");
		addPopupItem("Redo");
		popup.addSeparator();
		addPopupItem("Cut");
		addPopupItem("Copy");
		addPopupItem("Paste");
		addPopupItem("Delete");
		popup.addSeparator();
		addPopupItem("Select All");
	}

	public void paint(Graphics g) {
		Rectangle rc = g.getClipBounds();
		SinkWorld.SRectangle rcPaint = new SinkWorld.SRectangle();
		rcPaint.left = rc.x;
		rcPaint.top = rc.y;
		rcPaint.right = rc.x + rc.width;
		rcPaint.bottom = rc.y + rc.height;
		SinkWorld.Surface surface = new SinkWorld.Surface(g);

		tv.PaintRect(surface, rcPaint);

		panel.SetScrollBars();
	}

	public Dimension getMinimumSize() {
		return new Dimension(100, 100);
	}

	public void adjustmentValueChanged(AdjustmentEvent e) {
		if (e.getAdjustmentType() == AdjustmentEvent.TRACK) {
			int newTopLine = e.getValue();
			int diff = tv.topLine - newTopLine;
			tv.topLine = newTopLine;
			RepaintManager rm = RepaintManager.currentManager(this);
			Rectangle rDirty = rm.getDirtyRegion(this);
			if (rDirty.isEmpty() && (java.lang.Math.abs(diff) < (panel.PageSize()/2))) {
				Graphics g = getGraphics();
				Rectangle r = getBounds();
				int move = diff * tv.lineHeight;
				g.copyArea(0, 0, r.width, r.height, 0, move);
				if (move < 0) {
					paintImmediately(0, r.height+move, r.width, -move);
				} else {
					paintImmediately(0, 0, r.width, move);
				}
			} else {
				Invalidate();
			}
		}
	}

	public void horizontalAdjustmentValueChanged(AdjustmentEvent e) {
		if (e.getAdjustmentType() == AdjustmentEvent.TRACK) {
			tv.leftColumn = e.getValue();
			Invalidate();
		}
	}

	public int PositionFromLocation(Point pt) {
		SinkWorld.Surface surface = new SinkWorld.Surface(getGraphics());
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
		SinkWorld.SRectangle rcInvalidate = new SinkWorld.SRectangle();
		rcInvalidate.left = 0;
		rcInvalidate.right = 2000;
		rcInvalidate.top = (lineTop - panel.ScrollPos()) * tv.lineHeight;
		rcInvalidate.bottom = (lineBottom - panel.ScrollPos()) * tv.lineHeight;
		// Allow for decorators that go 2 pixels beyond line.
		rcInvalidate.top -= 2;
		rcInvalidate.bottom += 2;
		repaint(rcInvalidate.left, rcInvalidate.top,
			rcInvalidate.Width(), rcInvalidate.Height());
	}

	public void Invalidate() {
		//~ System.out.println("Big redraw");
		repaint();
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

	public boolean DeleteRange(int position, int deleteLength) {
		int lineStart = panel.pdoc.pbuffer.LineFromPosition(position);
		int lineEnd = panel.pdoc.pbuffer.LineFromPosition(position + deleteLength);
		panel.pdoc.pbuffer.DeleteRange(position, deleteLength);
		tv.fs.InvalidateWidths();
		panel.frame.StateAltered();
		return lineStart != lineEnd;
	}

	public boolean CheckWrap() {
		if ((panel.pdoc == null) || (tv == null) || (tv.fs == null))
			return true;
		if (nextLineWrapped < panel.pdoc.pbuffer.Lines()) {
			boolean redisplay = false;
			SRectangle rcClient = new SRectangle();
			panel.GetViewRectangle(rcClient);
			tv.lineWidth = rcClient.right - rcClient.left;
			if (panel.wrap) {
				int minBytesToWrap = tv.lineWidth / tv.tabWidth + 1;
				//ElapsedTime et;
				panel.pdoc.pbuffer.LexTo(panel.pdoc.pbuffer.Length(), 0, false);
				// Get a surface
				Surface surface = new Surface(getGraphics());
				for (int line=0;line<20000 && nextLineWrapped < panel.pdoc.pbuffer.Lines();line++) {
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

	public void keyTyped(KeyEvent e) {
		if (e.getKeyChar() == '\n') {
		} else if (e.getKeyChar() == '\b') {
		} else if (e.getKeyChar() == '\177') {
		} else if (! e.isAltDown()) {
			try {
				String s = "" + e.getKeyChar();
				byte[] bytes = s.getBytes("UTF8");
				if ((bytes.length > 1) || (!e.isControlDown())) {
					if (!sr.Empty()) {
						panel.Clear();
						Invalidate();
					}
					//System.out.println("Character typed" + e.getKeyChar() + ":" + bytes.length);
					int charUTF32[] = { bytes[0] };
					int len = InsertUTF32(sr.Caret(), charUTF32, 1);
					panel.CheckLineForInvalidate(sr.Caret());
				}
			} catch (java.io.UnsupportedEncodingException x) {
			}
		}
	}

	public void keyPressed(KeyEvent e) {
		timeEvent = e.getWhen();
		//~ System.err.println("Character pressed" + e.getKeyCode() + " " + e.getModifiers());
		boolean control = e.isControlDown();
		boolean shift = e.isShiftDown();
		int keyCode = e.getKeyCode();
		if (keyCode == KeyEvent.VK_ADD && control) {
			tv.ModifyMagnification(1);
			Invalidate();
			e.consume();
		} else if (keyCode == KeyEvent.VK_SUBTRACT && control) {
			tv.ModifyMagnification(-1);
			Invalidate();
			e.consume();
		} else if (keyCode == KeyEvent.VK_LEFT && control) {
			panel.MoveCaret(panel.pdoc.NextWordStart(sr.Caret()-1, -1), shift);
		} else if (keyCode == KeyEvent.VK_LEFT) {
			int newCaret = panel.pdoc.pbuffer.MovePositionOutsideChar(sr.Caret()-1, -1, true);
			int line = panel.pdoc.pbuffer.LineFromPosition(newCaret);
			if (!tv.fs.GetVisible(line))
				newCaret = panel.pdoc.pbuffer.LineEndPositionFromLine(
					tv.fs.DocFromDisplay(tv.fs.DisplayFromDoc(line) - 1));
			panel.MoveCaret(newCaret, shift);
			e.consume();
		} else if (keyCode == KeyEvent.VK_RIGHT && control) {
			panel.MoveCaret(panel.pdoc.NextWordStart(sr.Caret()+1, 1), shift);
		} else if (keyCode == KeyEvent.VK_RIGHT) {
			int newCaret = panel.pdoc.pbuffer.MovePositionOutsideChar(sr.Caret()+1, 1, true);
			int line = panel.pdoc.pbuffer.LineFromPosition(newCaret);
			if (!tv.fs.GetVisible(line))
				newCaret = panel.pdoc.pbuffer.PositionFromLine(
					tv.fs.DocFromDisplay(tv.fs.DisplayFromDoc(line)));
			panel.MoveCaret(newCaret, shift);
			e.consume();
		} else if (keyCode == KeyEvent.VK_HOME) {
			int line = control ? 0 : panel.pdoc.pbuffer.LineFromPosition(sr.Caret());
			panel.MoveCaret(panel.pdoc.pbuffer.PositionFromLine(line), shift);
			e.consume();
		} else if (keyCode == KeyEvent.VK_END) {
			int line = control ? panel.pdoc.pbuffer.Lines() : panel.pdoc.pbuffer.LineFromPosition(sr.Caret());
			panel.MoveCaret(panel.pdoc.pbuffer.LineEndPositionFromLine(line), shift);
			e.consume();
		} else if (keyCode == KeyEvent.VK_UP) {
			int lineDisplay = tv.DisplayLineFromPosition(sr.Caret()) - 1;
			if (lineDisplay < 0) {
				lineDisplay = 0;
			}
			panel.MoveCaret(panel.pdoc.pbuffer.PositionFromLine(
				tv.fs.DocFromDisplay(lineDisplay)), shift);
			e.consume();
		} else if (keyCode == KeyEvent.VK_PAGE_UP) {
			int lineDisplay = tv.DisplayLineFromPosition(sr.Caret()) - panel.PageSize();
			if (lineDisplay < 0) {
				lineDisplay = 0;
			}
			panel.MoveCaret(panel.pdoc.pbuffer.PositionFromLine(
				tv.fs.DocFromDisplay(lineDisplay)), shift);
			e.consume();
		} else if (keyCode == KeyEvent.VK_DOWN) {
			int lineDisplay = tv.DisplayLineFromPosition(sr.Caret()) + 1;
			if (lineDisplay > tv.fs.DisplayFromDoc(panel.pdoc.pbuffer.Lines())) {
				lineDisplay = tv.fs.DisplayFromDoc(panel.pdoc.pbuffer.Lines());
			}
			panel.MoveCaret(panel.pdoc.pbuffer.PositionFromLine(
				tv.fs.DocFromDisplay(lineDisplay)), shift);
			e.consume();
		} else if (keyCode == KeyEvent.VK_PAGE_DOWN) {
			int lineDisplay = tv.DisplayLineFromPosition(sr.Caret()) + panel.PageSize();
			if (lineDisplay > tv.fs.DisplayFromDoc(panel.pdoc.pbuffer.Lines())) {
				lineDisplay = tv.fs.DisplayFromDoc(panel.pdoc.pbuffer.Lines());
			}
			panel.MoveCaret(panel.pdoc.pbuffer.PositionFromLine(
				tv.fs.DocFromDisplay(lineDisplay)), shift);
			e.consume();
		} else if (keyCode == KeyEvent.VK_BACK_SPACE) {
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
			e.consume();
		} else if (keyCode == KeyEvent.VK_DELETE) {
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
			e.consume();
		} else if (keyCode == KeyEvent.VK_ESCAPE) {
			e.consume();
		} else if (keyCode == KeyEvent.VK_ENTER) {
			if (!sr.Empty()) {
				panel.Clear();
			}
			InsertUTF32(sr.Caret(), endOfLine.Data(), endOfLine.Length());
			//assert(panel.pdoc.pbuffer.PositionFromLine(panel.pdoc.pbuffer.Lines()) == panel.pdoc.pbuffer.Length());
			Invalidate();
			e.consume();
		} else if (keyCode == 'D' && control) {
			LineDuplicate();
			e.consume();
		}
	}

	public void keyReleased(KeyEvent e) { }

	boolean IsWordCharacter(int ch) {
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

	void ShowFeedBack(int position, boolean mouseDown, boolean showDown) {
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

	public void mouseClicked(MouseEvent e) {
	}

	public void actionPerformed(ActionEvent e) {
		String command = e.getActionCommand();
		//~ System.out.println("Context " + command);
		if (command.equals("Undo")) {
			panel.Undo();
		} else if (command.equals("Redo")) {
			panel.Redo();
		} else if (command.equals("Cut")) {
			panel.Cut();
		} else if (command.equals("Copy")) {
			panel.Copy();
		} else if (command.equals("Paste")) {
			panel.Paste();
		} else if (command.equals("Delete")) {
			panel.Clear();
		} else if (command.equals("Select All")) {
			panel.SelectAll();
		}
	}

	void addPopupItem(String s) {
		JMenuItem menuItem = new JMenuItem(s);
		popup.add(menuItem);
        	menuItem.addActionListener(this);
	}

	public void contextMenu(MouseEvent e) {
		popup.show(this, e.getX(), e.getY());
	}

	public void mousePressed(MouseEvent e) {
		panel.GrabFocus();
		if (e.getButton() != MouseEvent.BUTTON1) {
			//~ System.err.println("Mouse ci " + e.getX() + "," + e.getY());
			contextMenu(e);
			return;
		}
		Point pt = new Point(e.getX(), e.getY());
		long timeMessage = e.getWhen();

		//~ System.err.println("Mouse pressed " + e.getX() + "," + e.getY());

		int posPoint = PositionFromLocation(pt);
		if (tv.ColumnTypeFromLocation(pt) == TextColumn.tcFold) {
			panel.FoldLine(panel.pdoc.pbuffer.LineFromPosition(posPoint), panel.fcDiscover);
			return;
		}
		/*
		if (pstyleFeedBack.ValueAt(posPoint) != 0) {
			ShowFeedBack(posPoint, true,
				(e.getModifiers() & InputEvent.CTRL_MASK) != 0);
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

	public void mouseReleased(MouseEvent e) {
		Point pt = new Point(e.getX(), e.getY());
		if (panel.feedBack.down) {
			int posPoint = PositionFromLocation(pt);
			ShowFeedBack(posPoint, false,
				(e.getModifiers() & InputEvent.CTRL_MASK) != 0);
			System.out.println("Click " + panel.feedBack.word);
		}
	}

	public void mouseEntered(MouseEvent e) { }

	public void mouseExited(MouseEvent e) { }

	public void mouseDragged(MouseEvent e) {
		Point pt = new Point(e.getX(), e.getY());
		if (!panel.feedBack.down) {
			if (!(pt.equals(mouseDragLast))) {
				mouseDragLast = pt;
				panel.MoveCaret(PositionFromLocation(mouseDragLast), true);
			}
		}
	}

	public void mouseMoved(MouseEvent e) {
		Point pt = new Point(e.getX(), e.getY());
		//~ System.out.println("mouseMoved " + e.getModifiers());
		boolean ctrlDown = (e.getModifiers() & InputEvent.CTRL_MASK) != 0;
		if (ctrlDown || panel.feedBack.down) {
			ShowFeedBack(PositionFromLocation(pt), false, ctrlDown);
		}
	}

	public void mouseWheelMoved(MouseWheelEvent e) {
		panel.ScrollTo(tv.topLine + e.getWheelRotation() * e.getScrollAmount());
	}

	public void componentHidden(ComponentEvent e) { }

	public void componentMoved(ComponentEvent e) { }

	public void componentResized(ComponentEvent e) {
		if ((tv == null) || (tv.fs == null))
			return;
		SRectangle rcClient = new SRectangle();
		panel.GetViewRectangle(rcClient);
		tv.SetRectangle(rcClient);
		if (nextLineWrapped >= panel.pdoc.pbuffer.Lines()) {
			goodTopLine = tv.fs.DocFromDisplay(tv.topLine);
		}
		nextLineWrapped = 0;
		CheckWrap();
	}

	public void CaretOn() {
		timeLastCaret = timeEvent;
		sr.SetOn(true);
		InvalidateRange(sr.Caret(), sr.Caret());
	}

	public void componentShown(ComponentEvent e) { }

	public void focusGained(FocusEvent e) {
		panel.SetSelectionStyles();
		Invalidate();
		panel.frame.StateAltered();
	}

	public void focusLost(FocusEvent e) {
		panel.SetSelectionStyles();
		Invalidate();
		panel.frame.StateAltered();
	}
}
