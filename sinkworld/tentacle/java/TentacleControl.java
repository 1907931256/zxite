package SinkWorld;

import java.awt.*;
import java.awt.event.*;
import java.awt.datatransfer.*;
import java.io.*;
import javax.swing.*;

/**
 * TentacleJ demonstration editor copyright 2004 Neil Hodgson.
 */

class FeedBack {
	public int start = -1;
	public boolean down = false;
	public boolean show = false;
	public Literal word = new Literal("");
	public boolean ChangeState(int start_, boolean down_, boolean show_) {
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

public class TentacleControl extends JPanel {

	IDocumentChangeListener frame = null;

	FeedBack feedBack = new FeedBack();

	Document pdoc = null;
	StyleSet ssSelection;
	StyleSet ssCaret;

	public TextArea textArea;
	public JScrollBar scrollbarv;
	public JScrollBar scrollbarh;
	boolean braceLevel = false;
	boolean fancy = false;
	public boolean wrap = false;

	LiteralSet fontNames = new LiteralSet();

	public TentacleControl() {
		super(true);
		textArea = new TextArea(this);
		CreateDocument("");
		textArea.tv.topLine = 0;
		setLayout(new GridBagLayout());
		scrollbarv = new JScrollBar();
		scrollbarh = new JScrollBar(Adjustable.HORIZONTAL);
		GridBagConstraints c = new GridBagConstraints();
		c.gridx = 0;
		c.gridy = 0;
		c.weightx = 1.0;
		c.weighty = 1.0;
		c.fill = GridBagConstraints.BOTH;
		add(textArea, c);
		c.weightx = 0.0;
		c.weighty = 0.0;
		c.gridx = 1;
		c.gridy = 0;
		c.fill = GridBagConstraints.VERTICAL;
		add(scrollbarv, c);
		c.gridx = 0;
		c.gridy = 1;
		c.fill = GridBagConstraints.HORIZONTAL;
		add(scrollbarh, c);
		scrollbarv.addAdjustmentListener(textArea);
		scrollbarh.addAdjustmentListener(new AdjustmentListener() {
			public void adjustmentValueChanged(AdjustmentEvent e) {
				textArea.horizontalAdjustmentValueChanged(e);
			}
		});
	}

	public void Deallocate() {
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

	public Dimension getPreferredSize() {
		return new Dimension(50, 50);
	}

	int ScrollPos() {
		return scrollbarv.getValue();
	}

	void SetScrollBars() {
		if ((textArea.tv == null) || (textArea.tv.fs == null))
			return;
		SRectangle rcClient = new SRectangle();
		GetViewRectangle(rcClient);
		textArea.tv.lineWidth = rcClient.right - rcClient.left;
		//int nPage = PageSize();
		int pageScroll = PageSize();
		int nMax = textArea.tv.fs.DisplayFromDoc(pdoc.pbuffer.Lines());
		scrollbarv.setMaximum(nMax);
		scrollbarv.setBlockIncrement(pageScroll);
		scrollbarv.setVisibleAmount(pageScroll);
		scrollbarh.setMaximum(1200);
		scrollbarh.setUnitIncrement(8);
		scrollbarh.setBlockIncrement(120);
		scrollbarh.setVisibleAmount(120);
	}

	public void InitScrollBars() {
		SetScrollBars();
		textArea.tv.topLine = 0;
		scrollbarv.setValue(textArea.tv.topLine);
	}

	int PageSize() {
		SRectangle rcClient = new SRectangle();
		GetViewRectangle(rcClient);
		int linesOnScreen = (rcClient.bottom - rcClient.top) / textArea.tv.lineHeight;
		if (linesOnScreen > 0) {
			return linesOnScreen;
		} else {
			return 1;
		}
	}

	public Dimension getMinimumSize() {
		return new Dimension(100, 100);
	}

	public void InvalidateRange(int a, int b) {
		textArea.InvalidateRange(a, b);
	}

	void CheckLineForInvalidate(int position) {
		int line = pdoc.pbuffer.LineFromPosition(position);
		System.err.println("Invalidating for " + position + " on line " + line);
		int positionCheck = pdoc.pbuffer.PositionFromLine(line+1);
		boolean changedBeyondLine = pdoc.pbuffer.LexTo(positionCheck, 0, true);
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

	void ScrollTo(int newTopLine) {
		//~ System.out.println("ScrollTo " + newTopLine + "max " + scrollbarv.getMaximum());
		if (newTopLine > (scrollbarv.getMaximum() - PageSize())) {
			newTopLine = scrollbarv.getMaximum() - PageSize();
		}
		if (newTopLine < 0) {
			newTopLine = 0;
		}
		textArea.tv.topLine = newTopLine;
		scrollbarv.setValue(textArea.tv.topLine);
		textArea.Invalidate();
	}

	void MoveCaret(int newCaret, boolean retainCurrentAnchor) {
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
				java.lang.Math.min(java.lang.Math.min(textArea.sr.StartSelection(), newCaret), newAnchor),
				java.lang.Math.max(java.lang.Math.max(textArea.sr.EndSelection(), newCaret), newAnchor));
			SetSelection(newCaret, newAnchor);
			EnsureCaretVisible();
		}
	}

	public void Paste() {
		Clipboard clip = getToolkit().getSystemClipboard();
		Transferable t = clip.getContents(this);
		try {
			Object o = t.getTransferData(DataFlavor.stringFlavor);
			String s = (String)o;
			byte[] bytes = s.getBytes("UTF8");
			int[] clipUTF32 = new int[bytes.length];
			int lenUTF32 = UniConversion.UTF32FromUTF8(clipUTF32, bytes.length, bytes, bytes.length);
			textArea.InsertUTF32(textArea.sr.Caret(), clipUTF32, lenUTF32);
		} catch (java.awt.datatransfer.UnsupportedFlavorException x) {
		} catch (java.io.UnsupportedEncodingException x) {
		} catch (java.io.IOException x) {
		}
		textArea.Invalidate();
	}

	public void Copy() {
		Clipboard clip = getToolkit().getSystemClipboard();
		int start = textArea.sr.StartSelection();
		int end = textArea.sr.EndSelection();
		byte []selection = new byte[pdoc.pbuffer.Characters(start, end-start) * 4];
		int lenUTF8 = pdoc.pbuffer.RetrieveUTF8(start, selection, end-start);
		byte []selectionUTF8 = new byte[lenUTF8];
		System.arraycopy(selection, 0, selectionUTF8, 0, lenUTF8);
		try {
			String sg = new String(selectionUTF8, "UTF8");
			StringSelection ss = new StringSelection(sg);
			clip.setContents(ss, ss);
		} catch (java.io.UnsupportedEncodingException x) {
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

	public static final int fcShow = 0;
	public static final int fcFold = 1;
	public static final int fcDiscover = 2;

	public void FoldLine(int currentLine, int cmd) {
		pdoc.pbuffer.LexTo(pdoc.pbuffer.Length(), 0, false);
		IStyles foldLevels = pdoc.GetFolds();
		int currentLevel = foldLevels.ValueAt(currentLine) & 0xFFFF;
		int line = currentLine;
		int level = currentLevel + 1;
		boolean showing = cmd == fcShow;
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

	void GetViewRectangle(SRectangle rc) {
		Rectangle r = textArea.getBounds();
		rc.left = r.x;
		rc.top = r.y;
		rc.right = r.x + r.width;
		rc.bottom = r.y + r.height;
	}

	public void CreateDocument(String name) {
		textArea.sr.Deallocate();
	}

	public void GrabFocus() {
		textArea.requestFocus();
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
			caretColor = new RGBColor(
				StyleModification.ColourFromLiteral(caretValue));
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
		textArea.tv.SetStyles(ssc_);
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
