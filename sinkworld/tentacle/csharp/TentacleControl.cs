using System;
using System.Diagnostics;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
/// <summary> TentacleControl demonstration editor control copyright 2004 Neil Hodgson.
/// </summary>

namespace SinkWorld {

class FeedBack {
	public int start = -1;
	public bool down = false;
	public bool show = false;
	public Literal word = new Literal("");
	public bool ChangeState(int start_, bool down_, bool show_) {
		if ((start != start_) || (down != down_) || (show != show_)) {
			start = start_;
			down = down_;
			show = show_;
			return true;
		} else {
			return false;
		}
	}
	public void Invalidate() {
		start = -1;
	}
};

public class TentacleControl : Panel {

	internal IDocumentChangeListener frame = null;

	internal FeedBack feedBack = new FeedBack();

	internal Document pdoc = null;
	internal StyleSet ssSelection;
	internal StyleSet ssCaret;

	public TextArea textArea;
	public VScrollBar scrollbarv;
	public HScrollBar scrollbarh;
	public bool braceLevel = false;
	public bool fancy = false;
	public bool wrap = false;

	LiteralSet fontNames = new LiteralSet();

	public TentacleControl() : base() {
		SizeChanged += new EventHandler(this.Me_SizeChanged);
		textArea = new TextArea(this);
		CreateDocument("");
		textArea.tv.topLine = 0;
		scrollbarv = new VScrollBar();
		scrollbarv.ValueChanged += new EventHandler(this.Scroll_ValueChanged);
		scrollbarh = new HScrollBar();
		scrollbarh.ValueChanged += new EventHandler(this.Scroll_HValueChanged);
		Controls.Add(textArea);
		Controls.Add(scrollbarv);
		Controls.Add(scrollbarh);
	}

	public void Deallocate() {
		pdoc = null;
		textArea.tv.Deallocate();
		textArea.sr.Deallocate();
		//pstyleFeedBack = null;
	}

	public void SetDocument(Document pdoc_, IDocumentChangeListener frame_) {
		pdoc = pdoc_;
		frame = frame_;
		Literal litLineNumberWidth = new Literal("lineNumberWidth");
		int lineNumberWidth = pdoc.options.IntegerModeValue(pdoc.currentMode, litLineNumberWidth, 4);
		Literal litFoldWidth = new Literal("foldWidth");
		int foldWidth = pdoc.options.IntegerModeValue(pdoc.currentMode, litFoldWidth, 1);
		textArea.tv.SetText(pdoc.pbuffer, lineNumberWidth, foldWidth);
		textArea.tv.SetStyles(pdoc.ssc);
		textArea.sr.Allocate();
		SetSelectionStyles();
		textArea.tv.SetFolds(pdoc.GetFolds());
	}

	private void Scroll_ValueChanged(Object sender, EventArgs e)	{
		int newTopLine = scrollbarv.Value;
		int diff = textArea.tv.topLine - newTopLine;
		textArea.tv.topLine = newTopLine;
		textArea.Invalidate();
	}

	private void Scroll_HValueChanged(Object sender, EventArgs e)	{
		textArea.tv.leftColumn = scrollbarh.Value;
		textArea.Invalidate();
	}

	private void Me_SizeChanged(object sender, EventArgs e) {
		textArea.Width = this.ClientSize.Width - scrollbarv.Width;
		textArea.Height = this.ClientSize.Height - scrollbarh.Height;
		scrollbarv.Top = 0;
		scrollbarv.Left = textArea.Width;
		scrollbarv.Height = textArea.Height;
		scrollbarh.Top = textArea.Height;
		scrollbarh.Left = 0;
		scrollbarh.Width = textArea.Width;
		SRectangle rcClient = new SRectangle();
		GetViewRectangle(rcClient);
		textArea.tv.SetRectangle(rcClient);
	}

	internal int ScrollPos() {
		return scrollbarv.Value;
	}

	public void SetScrollBars() {
		SRectangle rcClient = new SRectangle();
		GetViewRectangle(rcClient);
		textArea.tv.lineWidth = rcClient.right - rcClient.left;
		int nPage = PageSize();
		int pageScroll = PageSize();
		int nMax = textArea.tv.fs.DisplayFromDoc(pdoc.pbuffer.Lines());
		scrollbarv.Maximum = nMax;
		scrollbarv.LargeChange = pageScroll;
		scrollbarh.Maximum = 1200;
		scrollbarh.SmallChange = 8;
		scrollbarh.LargeChange = 120;
	}

	public void InitScrollBars() {
		SetScrollBars();
		textArea.tv.topLine = 0;
		scrollbarv.Value = textArea.tv.topLine;
	}

	internal int PageSize() {
		SRectangle rcClient = new SRectangle();
		GetViewRectangle(rcClient);
		int linesOnScreen = (rcClient.bottom - rcClient.top) / textArea.tv.lineHeight;
		if (linesOnScreen > 0) {
			return linesOnScreen;
		} else {
			return 1;
		}
	}

	public void InvalidateRange(int a, int b) {
		textArea.InvalidateRange(a, b);
	}

	internal void CheckLineForInvalidate(int position) {
		int line = pdoc.pbuffer.LineFromPosition(position);
		int positionCheck = pdoc.pbuffer.PositionFromLine(line+1);
		bool changedBeyondLine = pdoc.pbuffer.LexTo(positionCheck, 0, true);
		if (changedBeyondLine) {
			frame.InvalidateAll();
		} else {
			frame.InvalidateRange(position, position);
		}
	}

	public void AddContents(byte[] data, int length) {
		pdoc.pbuffer.InsertBytes(pdoc.pbuffer.Length(), data, length);
	}

	public void SetSelection(int newCaret, int newAnchor) {
		textArea.sr.SetSelectionStyleRanges(newAnchor, newCaret);
	}

	public void SetSelection(int newCaret) {
		SetSelection(newCaret, newCaret);
	}

	public void EnsureCaretVisible() {
		SRectangle rcClient = new SRectangle();
		GetViewRectangle(rcClient);
		int height = rcClient.bottom - rcClient.top;
		int posCaret = textArea.sr.Caret();
		int line = textArea.tv.DisplayLineFromPosition(textArea.sr.Caret());
		Point pt = new Point(0, (line-ScrollPos()) * textArea.tv.lineHeight);
		Point ptBottomCaret = new Point(pt.x, pt.y + textArea.tv.lineHeight - 1);
		int lineCaret = line;

		// Vertical positioning
		if (pt.y < rcClient.top || ptBottomCaret.y > height) {
			int newTopLine = ScrollPos();
			if (lineCaret < ScrollPos()) {
				// Caret goes too high
				newTopLine = lineCaret;
			} else if (lineCaret > ScrollPos() + PageSize() - 1) {
				// Caret goes too low
				newTopLine = lineCaret - PageSize() + 1;
			}
			ScrollTo(newTopLine);
		}
	}

	internal void ScrollTo(int newTopLine) {
		if (newTopLine > (scrollbarv.Maximum - PageSize()) + 1) {
			newTopLine = scrollbarv.Maximum - PageSize() + 1;
		}
		if (newTopLine < 0) {
			newTopLine = 0;
		}
		textArea.tv.topLine = newTopLine;
		scrollbarv.Value = textArea.tv.topLine;
		textArea.Invalidate();
	}

	internal void MoveCaret(int newCaret, bool retainCurrentAnchor) {
		if (newCaret < 0) {
			newCaret = 0;
		} else if (newCaret > pdoc.pbuffer.Length()) {
			newCaret = pdoc.pbuffer.Length();
		}
		int delta = newCaret - textArea.sr.Caret();
		newCaret = pdoc.pbuffer.MovePositionOutsideChar(newCaret, delta, true);
		int newAnchor = textArea.sr.Anchor();
		if (!retainCurrentAnchor) {
			newAnchor = newCaret;
		}
		if ((textArea.sr.Caret() != newCaret) || (textArea.sr.Anchor() != newAnchor)) {
			textArea.CaretOn();
			textArea.InvalidateRange(
				System.Math.Min(System.Math.Min(textArea.sr.StartSelection(), newCaret), newAnchor),
				System.Math.Max(System.Math.Max(textArea.sr.EndSelection(), newCaret), newAnchor));
			SetSelection(newCaret, newAnchor);
			EnsureCaretVisible();
		}
	}

	public void Paste() {
		IDataObject t = Clipboard.GetDataObject();
		try {
			Object o = t.GetData(DataFormats.StringFormat.ToString());
			String s = (String)o;
			byte[] bytes = Encoding.UTF8.GetBytes(s);
			int[] clipUTF32 = new int[bytes.Length];
			int lenUTF32 = UniConversion.UTF32FromUTF8(clipUTF32, bytes.Length, bytes, bytes.Length);
			textArea.InsertUTF32(textArea.sr.Caret(), clipUTF32, lenUTF32);
		} catch (Exception) {
		}
		textArea.Invalidate();
	}

	public void Copy() {
		int start = textArea.sr.StartSelection();
		int end = textArea.sr.EndSelection();
		byte []selection = new byte[pdoc.pbuffer.Characters(start, end-start) * 4];
		int lenUTF8 = pdoc.pbuffer.RetrieveUTF8(start, selection, end-start);
		byte []selectionUTF8 = new byte[lenUTF8];
		System.Array.Copy(selection, 0, selectionUTF8, 0, lenUTF8);

		try {
			String sg = Encoding.UTF8.GetString(selectionUTF8);
			DataObject ss = new DataObject(sg);
			Clipboard.SetDataObject(ss);
		} catch (System.IO.IOException) {
		}
	}

	public void Clear() {
		int start = textArea.sr.StartSelection();
		int end = textArea.sr.EndSelection();
		textArea.DeleteRange(start, end - start);
		textArea.Invalidate();
	}

	public void Cut() {
		Copy();
		Clear();
		textArea.Invalidate();
	}

	public void Undo() {
		pdoc.pbuffer.PerformUndoStep();
		textArea.Invalidate();
		frame.StateAltered();
	}

	public void Redo() {
		pdoc.pbuffer.PerformRedoStep();
		textArea.Invalidate();
		frame.StateAltered();
	}

	public void Delete() {
		if (textArea.sr.Empty()) {
			if (textArea.sr.Caret() < pdoc.pbuffer.Length()) {
				if (textArea.DeleteRange(textArea.sr.Caret(), 1)) {
					textArea.Invalidate();
				}
				CheckLineForInvalidate(textArea.sr.Caret());
			}
		} else {
			Clear();
			textArea.Invalidate();
		}
	}

	public void SelectAll() {
		SetSelection(0, pdoc.pbuffer.Length());
		textArea.Invalidate();
	}

	public int fcShow = 0;
	public int fcFold = 1;
	public int fcDiscover = 2;

	public void FoldLine(int currentLine, int cmd) {
		pdoc.pbuffer.LexTo(pdoc.pbuffer.Length(), 0, false);
		IStyles foldLevels = pdoc.GetFolds();
		int currentLevel = foldLevels.ValueAt(currentLine) & 0xFFFF;
		int line = currentLine;
		int level = currentLevel + 1;
		bool showing = cmd == fcShow;
		if (cmd == fcDiscover)
			showing = !textArea.tv.fs.GetExpanded(currentLine);
		textArea.tv.fs.SetExpanded(currentLine, showing);
		while (level > currentLevel) {
			line++;
			level = foldLevels.ValueAt(line) & 0xFFFF;
			if (level > currentLevel)
				textArea.tv.fs.SetVisible(line, line, showing);
		}
		textArea.Invalidate();
	}

	public void ToggleFold() {
		FoldLine(pdoc.pbuffer.LineFromPosition(textArea.sr.Caret()),
			fcDiscover);
	}

	public void FoldAll(int cmd) {
		for (int line=0;line<pdoc.pbuffer.Lines()-1;line++) {
			FoldLine(line, cmd);
		}
	}

	public void ToggleBraceLevel() {
		braceLevel = !braceLevel;
		textArea.Invalidate();
		StartWrap();
	}

	public void ToggleFancy() {
		fancy = !fancy;
		textArea.Invalidate();
		StartWrap();
	}

	public void StartWrap() {
		textArea.nextLineWrapped = 0;
		textArea.goodTopLine = textArea.tv.fs.DocFromDisplay(textArea.tv.topLine);
		SetScrollBars();
	}

	public void ToggleWrap() {
		wrap = !wrap;
		textArea.tv.wrap = wrap;
		textArea.Invalidate();
		StartWrap();
	}

	internal void GetViewRectangle(SRectangle rc) {
		Rectangle r = textArea.Bounds;
		rc.left = r.X;
		rc.top = r.Y;
		rc.right = r.X + r.Width;
		rc.bottom = r.Y + r.Height;
	}

	public void CreateDocument(String name) {
		textArea.sr.Deallocate();
	}

	public void GrabFocus() {
		textArea.Focus();
	}

	public void SetSelectionStyles() {
		textArea.tv.RemoveStyleSet(ssSelection);
		ssSelection = new StyleSet(textArea.sr.pstyleSelection, 2);
		Literal styleName = new Literal(textArea.hasFocus() ?
			"selection.style" : "selection.style.no.focus");
		StyleModification smSelection = StyleModification.FromOptions(
			pdoc.currentMode, styleName, pdoc.options, null);
		ssSelection.Set(1, smSelection);
		textArea.tv.AddStyleSet(ssSelection);

		textArea.tv.RemoveStyleSet(ssCaret);
		ssCaret = new StyleSet(textArea.sr, 2);
		StyleModification smCaret = new StyleModification();
		Literal litDecoration = new Literal("Decoration");
		Literal litCaret = new Literal("Caret");
		IDecoration caret = (IDecoration)PrototypeRegistry.FromName(litDecoration, litCaret);

		Literal caretName = new Literal(textArea.hasFocus() ?
			"caret.color" : "caret.color.no.focus");
		RGBColor caretColor = textArea.hasFocus() ? 
			new RGBColor(0xff,0,0) :  new RGBColor(0,0,0);
		Literal caretValue = pdoc.options.ModeValue(pdoc.currentMode, caretName);
		if (caretValue != null) {
			caretColor = new RGBColor(StyleModification.ColourFromLiteral(caretValue));
		}
		caret.SetFore(caretColor);
		smCaret.AddDecorationFore(caret);
		ssCaret.Set(1, smCaret);
		textArea.tv.AddStyleSet(ssCaret);

		pdoc.pbuffer.DetachStyles(textArea.sr);
		pdoc.pbuffer.AttachStyles(textArea.sr, null, 0, false);
	}
	/*
	public void SetStyles(StyleSetCollection ssc_, IStyles pstyle_, IStyles pstyleBraceCount_) {
		pstyle = pstyle_;
		pstyleBraceCount = pstyleBraceCount_;
		textArea.tv.SetStyles(ssc_);
		if (braceLevel && (pstyleBraceCount != null)) {
			StyleSet ssBC = LanguageDefinition.BraceCountStyleSet(pstyleBraceCount);
			textArea.tv.AddStyleSet(ssBC);
		}
		textArea.sr.Allocate();

		SetSelectionStyles();


		if (fancy) {
			pstyleFeedBack = new RunStyles();
			StyleSet ssFeedBack = new StyleSet(pstyleFeedBack, 3);
			StyleModification smFeedBack = new StyleModification();
			smFeedBack.AddDecorationBack(new DecorationButton(false));
			//smFeedBack.AddDecorationBack(new DecorationUnderLine(RGBColor(0x0,0x0,0xff)));
			ssFeedBack.Set(1, smFeedBack);
			StyleModification smFeedBackHit = new StyleModification();
			smFeedBackHit.AddDecorationBack(new DecorationButton(true));
			//smFeedBackHit.AddDecorationBack(new DecorationUnderLine(RGBColor(0x0,0xff,0xff)));
			ssFeedBack.Set(2, smFeedBackHit);
			textArea.tv.AddStyleSet(ssFeedBack);
		} else {
			pstyleFeedBack = new RunStyles();
		}

	}
	*/
}

}
