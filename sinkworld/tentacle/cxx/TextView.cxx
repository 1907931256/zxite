// SinkWorld TextView class.
// Copyright 2004 Neil Hodgson
// File written in C~ subset of C++ for automatic translation to Java and C#.

#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <string>

#include "base.h"

#include "PhysicalLayout.h"

#include "RGBColor.h"
#include "IDecoration.h"
#include "Style.h"
#include "StyleModification.h"
#include "StyleSet.h"
#include "StyleSetCollection.h"
#include "FontDescription.h"
#include "Surface.h"
#include "ChangeLayer.h"
#include "ChangeFinder.h"
#include "StyleLayer.h"
#include "SubstanceLayer.h"
#include "PositionCache.h"
#include "LinePositionCache.h"
#include "ChangePositionsCache.h"
#include "Document.h"

#include "PhasedPaint.h"
#include "TextView.h"

void TextView::Realise(Surface *surface) {
	if ((ssc != NULL) && !realised) {
		realised = true;
		ascent = 1;
		int descent = 1;
		StyleSet *ss = ssc->GetMainStyleSet();
		for (int i=0; i < ss->len; i++) {
			Style styleNow = Style(ssc->fontNameDefault, 10,
				false, false, false, RGBColor(), RGBColor(0xff,0xff,0xff));
			StyleModification *sm = ss->Get(i);
			int flags = Style::sdItalics | Style::sdBold | Style::sdSize | Style::sdFont;
			if (sm != NULL)
				sm->Modify(styleNow, flags);
			FontDescription fd = FontDescription(styleNow.font, 
				styleNow.size + magnification, styleNow.bold, styleNow.italics);
			surface->SetFont(fd);
			int a = surface->TextAscent();
			int d = surface->TextDescent();
			if (ascent < a)
				ascent = a;
			if (descent < d)
				descent = d;
		}
		lineHeight = ascent + descent;
	}
}

TextView::TextView() {
	pbuffer = NULL;
	linePositions = new LinePositionCache();
	changePositions = new ChangePositionsCache();
	pc = new PositionCache();
	realised = false;
	root = NULL;
	ssc = NULL;
	sscMeasure = NULL;
	foldLevels = NULL;
	fs = NULL;
	leftColumn = 0;
	textLeft = 1;
	wrap = false;
	wrapWidth = 0;
	magnification = 0;
	visibleWhitespace = false;
	lineHeight = 13;
	pageHeight = 100;
	lineWidth = 100;
	accurateWidths = true;
	ascent = 10;
	topLine = 0;
	tabWidth = 32;
}

TextView::~TextView() {
	Deallocate();
	delete linePositions;
	linePositions = NULL;
	delete changePositions;
	changePositions = NULL;
	delete pc;
	pc = NULL;
	delete sscMeasure;
	sscMeasure = NULL;
}

void TextView::Deallocate() {
	delete fs;
	fs = NULL;
	delete ssc;
	ssc = NULL;
	delete sscMeasure;
	sscMeasure = NULL;
	TextColumn *tc=root;
	while (tc != NULL) {
		TextColumn *tcNext = tc->next;
		delete tc;
		tc = tcNext;
	}
	root = NULL;
	pbuffer = NULL;
}

void TextView::StylesChanged() {
	realised = false;
}

void TextView::SetVisibleWhitespace(bool visibleWhitespace_) {
	visibleWhitespace = visibleWhitespace_;
}

void TextView::SetText(SplitText *pbuffer_, int lineNumberWidth, int foldWidth) {
	if (fs != NULL) {
		pbuffer->RemovePerLineData(fs);
		delete fs;
	}
	pbuffer = pbuffer_;
	realised = false;
	fs = new PhysicalLayout();
	pbuffer->AddPerLineData(fs);
	pbuffer->AttachStyles(fs->lineFolds, NULL, 0, false);
	leftColumn = 0;
	if (root == NULL) {
		AddColumn(TextColumn::tcLineNumber, lineNumberWidth * 8 + 3);
		AddColumn(TextColumn::tcSeparator, 1);
		if (foldWidth == 1)
			AddColumn(TextColumn::tcFold, 17);
		else if (foldWidth == 2)
			AddColumn(TextColumn::tcFold, 70);
		if (foldWidth > 0)
			AddColumn(TextColumn::tcSeparator, 1);
		AddColumn(TextColumn::tcWhite, 1);
		AddColumn(TextColumn::tcText, 2000);
	}
}

void TextView::SetStylesMeasure() {
	delete sscMeasure;
	int flags = 
		Style::sdFore | Style::sdItalics | Style::sdBold | Style::sdSize | Style::sdFont;
	sscMeasure = ssc->View(flags);
}

void TextView::SetStyles(StyleSetCollection *ssc_) {
	delete ssc;
	ssc = ssc_->View(Style::sdAll);
	ssc->SetSpecifiedFlags();
	SetStylesMeasure();
}

void TextView::SetFolds(IStyles *foldLevels_) {
	foldLevels = foldLevels_;
}

void TextView::RemoveStyleSet(StyleSet *ss) {
	if (ssc != NULL) {
		ssc->Remove(ss);
		ssc->SetSpecifiedFlags();
		SetStylesMeasure();
	}
}

void TextView::AddStyleSet(StyleSet *ss) {
	if (ssc != NULL) {
		ssc->Add(ss);
		ssc->SetSpecifiedFlags();
		SetStylesMeasure();
	}
}

void TextView::SetRectangle(SRectangle *rc) {
	lineWidth = rc->right - rc->left;
	wrapWidth = lineWidth - textLeft;
	if (wrapWidth < 20)
		wrapWidth = 20;
	pageHeight = rc->bottom - rc->top;
}

void TextView::ModifyMagnification(int delta) {
	magnification += delta;
	if (magnification < -10) {
		magnification = -10;
	}
	StylesChanged();
	InvalidateChangePositions();
}

void TextView::InvalidateChangePositions() {
	linePositions->Clear();
	changePositions->Clear();
}

int TextView::DisplayLineFromPosition(int position) {
	int line = pbuffer->LineFromPosition(position);
	return fs->DisplayFromDoc(line);
}

int TextView::PageSize() {
	int linesOnScreen = pageHeight / lineHeight;
	if (linesOnScreen > 0) {
		return linesOnScreen;
	} else {
		return 1;
	}
}

int TextView::MaxScrollPos() {
	int scrollMax = fs->DisplayFromDoc(pbuffer->Lines()) - PageSize();
	if (scrollMax < 0)
		scrollMax = 0;
	return scrollMax;
}

SetIntegers *TextView::ChangePositionsFromLine(int line) {
	SetIntegers *cp = changePositions->Find(line);
	if (cp == NULL) {
		int lineStart = pbuffer->PositionFromLine(line);
		int lineEnd = pbuffer->PositionFromLine(line+1);
		assert(lineEnd >= lineStart);

		// Find all potential change positions
		cp = new SetIntegers(lineEnd - lineStart + 1);
		ChangeFinder cf = ChangeFinder(pbuffer, lineStart, lineEnd, ssc, cp);
		changePositions->Set(line, cp);
	}
	return cp;
}

TextPositions *TextView::MeasureText(Surface *surface, int line, const SetIntegers *cf) {
	Realise(surface);
	TextPositions *positions = linePositions->Find(line);
	if (positions != NULL) {
		return positions;
	}

	positions = new TextPositions();
	int flags = 
		Style::sdFore | Style::sdItalics | Style::sdBold | Style::sdSize | Style::sdFont;
	int lineStart = pbuffer->PositionFromLine(line);
	int lineEnd = pbuffer->PositionFromLine(line+1);
	if (lineStart == lineEnd) {
		linePositions->Set(line, positions);
		return positions;
	}
	int startSegment = lineStart;
	Style styleStart = Style(sscMeasure->fontNameDefault);
	sscMeasure->Modify(lineStart, flags, styleStart);
	for (int ip=0;ip<cf->Length();ip++) {
		int positionChange = cf->At(ip);
		Style styleNow = Style(sscMeasure->fontNameDefault);
		sscMeasure->Modify(positionChange, flags, styleNow);
		if ((positionChange >= lineEnd) ||
			(styleNow.DifferentTo(styleStart, flags)) ||
			SubstanceLayer::IsControlCharacter(pbuffer->CharValue(startSegment)) ||
			SubstanceLayer::IsControlCharacter(pbuffer->CharValue(positionChange))) {
			int len = positionChange - startSegment;
			int chFirst = pbuffer->CharValue(startSegment);
			if (chFirst == '\t') {
				int lastPosition = positions->LastPosition();
				int nextTab = lastPosition / tabWidth * tabWidth + tabWidth;
				positions->Add(nextTab);
			} else if ((chFirst == '\r') || (chFirst == '\n')) {
				positions->Add(positions->LastPosition() + 4);
				positions->FillTo(positions->Last() + len);
			} else {
				FontDescription fd = FontDescription(styleStart.font, 
					styleStart.size + magnification, styleStart.bold, styleStart.italics);
				pc->MeasureStyledText(surface, fd, pbuffer, startSegment, len, positions);
			}
			styleStart = styleNow;
			startSegment = positionChange;
		}
	}
	linePositions->Set(line, positions);
	return positions;
}

int TextView::ColumnTypeFromLocation(Point pt) {
	for (TextColumn *tc=root; tc != NULL; tc = tc->next) {
		if (pt.x < tc->right) {
			return tc->type;
		}
	}
	return TextColumn::tcWhite;
}

int TextView::PositionFromSurfaceLocation(Surface *surface, Point pt) {
	Realise(surface);
	int lineDisplay = pt.y / lineHeight + topLine;
	if (lineDisplay < 0) {
		return 0;
	}
	int line = fs->DocFromDisplay(lineDisplay);
	if (line >= pbuffer->Lines()) {
		return pbuffer->Length();
	}
	int subLine = fs->SubLineFromDisplay(lineDisplay);

	int lineStart = pbuffer->PositionFromLine(line);
	int lineEnd = pbuffer->LineEndPositionFromLine(line);
	if (lineEnd < lineStart) {
		lineStart = pbuffer->PositionFromLine(line);
		lineEnd = pbuffer->LineEndPositionFromLine(line);
		assert(false);
	}
	assert(lineEnd >= lineStart);
	if (lineEnd == lineStart) {
		return lineEnd;
	}
	int lineLen = lineEnd - lineStart;
	ByteBuffer s = ByteBuffer(lineLen+1);
	pbuffer->RetrieveBytes(lineStart, s.Bytes(), lineLen);
	s.Bytes()[lineLen] = 0;

	// Find all potential change positions
	SetIntegers *cf = ChangePositionsFromLine(line);

	TextPositions *positions = MeasureText(surface, line, cf);
	int subLineStart = lineStart;
	if (subLine > 0) {
		int lineFlow = fs->lineFolds->PartitionFromPosition(lineStart) + subLine;
		subLineStart = fs->lineFolds->PositionFromPartition(lineFlow);
	}
	int sp = subLineStart - lineStart;
	int xStart = leftColumn - textLeft + positions->At(subLineStart-lineStart);
	for (; sp<positions->Last()-1; sp++) {
		if (positions->AtMiddle(sp) - xStart >= pt.x) {
			return pbuffer->MovePositionOutsideChar(lineStart + sp, 1, true);
		}
	}
	if (lineEnd > lineStart) {
		return lineEnd;
	} else {
		return subLineStart;
	}
}

void TextView::WrapLine(Surface *surface, int line) {
	Realise(surface);
	// This should really hook into pbuffer expansion so that the
	// partition positions are maintained.
	if (pbuffer->Length() != fs->lineFolds->Length()) {
		fs->lineFolds->InsertText(
			fs->lineFolds->Length(), pbuffer->Length() - fs->lineFolds->Length());
	}
	int height = 1;
	int width = fs->GetWidth(line);
	int lineStart = pbuffer->PositionFromLine(line);
	int lineEnd = pbuffer->LineEndPositionFromLine(line);
	int minBytesToWrap = wrapWidth / tabWidth + 1;
	fs->lineFolds->RemovePartitions(lineStart, lineEnd);
	if (((width <= 0) || (width > wrapWidth)) && 
		(lineEnd - lineStart >= minBytesToWrap)) {
		SetIntegers *cf = ChangePositionsFromLine(line);
		TextPositions *positions = MeasureText(surface, line, cf);
		int lineFlow = fs->lineFolds->PartitionFromPosition(lineStart);
		height = 1;
		if (fs->lineFolds->PositionFromPartition(lineFlow) != lineStart) {
			lineFlow++;
			fs->lineFolds->InsertPartition(lineFlow, lineStart);
		}
		StyleSet *ssMain = sscMeasure->GetMainStyleSet();
		IStyles *pstyles = NULL;
		if (ssMain != NULL)	// There may be no styles for plain text
			pstyles = ssMain->GetBuffer();
		int lastGoodBreak = 0;
		int lastLineStart = 0;
		int startOffset = 0;
		int p = 0;
		while (p < (lineEnd - lineStart + 1)) {
			// Would be more efficient to hop along by change positions
			if ((positions->At(p+1) - startOffset) > wrapWidth) {
				if (lastGoodBreak == lastLineStart) {
					// Try moving to start of last character
					if (p > 0) {
						lastGoodBreak = pbuffer->MovePositionOutsideChar(
							p+lineStart, -1, false) - lineStart;
					}
					if (lastGoodBreak == lastLineStart) {
						// Ensure at least one character on line.
						lastGoodBreak = pbuffer->MovePositionOutsideChar(
							lastGoodBreak + lineStart + 1, 1, false) - lineStart;
					}
				}
				lastLineStart = lastGoodBreak;
				lineFlow++;
				fs->lineFolds->InsertPartition(lineFlow, lastGoodBreak+lineStart);
				startOffset = positions->At(lastGoodBreak);
				p = lastGoodBreak + 1;
				height++;
				continue;
			}
			if (p > 0) {
				if ((pstyles != NULL) && (pstyles->ValueAt(p+lineStart) != pstyles->ValueAt(p-1+lineStart))) {
					lastGoodBreak = p;
				} else if (
					IsSpaceOrTab(pbuffer->CharValue(pbuffer->MovePositionOutsideChar(p + lineStart - 1, -1, false))) && 
					!IsSpaceOrTab(pbuffer->CharValue(p + lineStart))) {
					lastGoodBreak = p;
				}
			}
			p++;
		}
		width = positions->LastPosition();
		fs->SetWidth(line, width);
	} else {
		int lineFlow = fs->lineFolds->PartitionFromPosition(lineStart);
		if (fs->lineFolds->PositionFromPartition(lineFlow) != lineStart) {
			fs->lineFolds->InsertPartition(lineFlow+1, lineStart);
		}
	}
	fs->SetHeight(line, height);
}

void TextView::PaintRect(Surface *surface, SRectangle rcPaint) {
	if (pbuffer == NULL)
		return;
	Realise(surface);
	int pageEnd = topLine + PageSize() + 1;
	if (pageEnd > pbuffer->Lines())
		pageEnd = pbuffer->Lines();
	pbuffer->LexTo(pbuffer->PositionFromLine(fs->DocFromDisplay(pageEnd)), 0, false);
	for (TextColumn *tc=root; tc != NULL; tc = tc->next) {
		SRectangle rcStrip = SRectangle(rcPaint);
		if (rcStrip.left < tc->left)
			rcStrip.left = tc->left;
		if (rcStrip.right > tc->right)
			rcStrip.right = tc->right;
		if (rcStrip.left < rcStrip.right) {
			surface->SetClip(&rcStrip);
			if (tc->type == TextColumn::tcLineNumber) {
				PaintLineNumberColumn(surface, rcStrip, tc);
			} else if (tc->type == TextColumn::tcText) {
				PaintTextColumn(surface, rcStrip, tc);
			} else if (tc->type == TextColumn::tcSeparator) {
				surface->FillRectangle(rcStrip, RGBColor(0xb0,0xb0,0xb0));
			} else if (tc->type == TextColumn::tcWhite) {
				surface->FillRectangle(rcStrip, RGBColor(0xff,0xff,0xff));
			} else if (tc->type == TextColumn::tcFold) {
				PaintFoldColumn(surface, rcStrip, tc);
			}
		}
	}
}

void TextView::PaintFoldColumn(Surface *surface, SRectangle rcPaint, TextColumn *tc) {

	surface->FillRectangle(rcPaint, RGBColor(0xfa,0xfa,0xfa));

	int startLine = rcPaint.top / lineHeight - 1;
	if (topLine + startLine < 0)
		startLine = 0;
	int endLine = rcPaint.bottom / lineHeight + 1;

	SRectangle rcw = SRectangle();

	int y = startLine;
	int lineDisplay = topLine+startLine;
	surface->SetTextColor(RGBColor(0,0,0));
	while (lineDisplay < fs->LinesDisplayed() && y <= endLine) {
		if (fs->SubLineFromDisplay(lineDisplay) == 0) {
			rcw.top = y * lineHeight;
			rcw.bottom = rcw.top + lineHeight;
			rcw.left = 0;
			rcw.right = tc->right;
			int levelCurrent = 0;
			int levelNext = 0;
			int lineDoc = fs->DocFromDisplay(lineDisplay);
			if ((foldLevels != NULL) && (lineDoc < foldLevels->Length())) {
				int levels = foldLevels->ValueAt(lineDoc);
				levelCurrent = levels & 0xFFFF;
				int levelsNextLine = 100;
				if ((lineDoc+1) < foldLevels->Length())
					levelsNextLine = foldLevels->ValueAt(lineDoc+1);
				levelNext = levelsNextLine & 0xFFFF;
				if (tc->right > tc->left + 30) {
					// Only display level numbers if column is wide
					Literal levC = Literal(levelCurrent);
					Literal levN = Literal(levelNext);
					Literal *lineNumber = new Literal(&levC, (int)' ', &levN);
					FontDescription fd = FontDescription(sscMeasure->fontNameDefault, 
						10 + magnification, false, false);
					surface->SetFont(fd);
					TextPositions *tpln = new TextPositions(10);
					SW_BYTE lineNumberUTF8[100];
					int lenUTF8 = UniConversion::UTF8LengthFromUTF32(lineNumber->Data(), lineNumber->Length());
					UniConversion::UTF8FromUTF32(lineNumberUTF8, lenUTF8,
						lineNumber->Data(), lineNumber->Length());
					surface->MeasureWidths(lineNumberUTF8, lenUTF8, tpln, 0);
					rcw.left = rcw.right - tpln->LastPosition() - 2;
					surface->PaintText(lineNumberUTF8, lenUTF8, &rcw, ascent, 0);
					delete lineNumber;
					delete tpln;
				}
				if (levelNext > levelCurrent) {
					rcw.left = tc->left;
					int armLength = (rcw.bottom-rcw.top)/2 - 3;
					int mid = (rcw.bottom+rcw.top)/2;
					if (fs->GetVisible(lineDoc+1)) {
						surface->MoveTo(rcw.left + 2, mid-3);
						surface->LineTo(rcw.left + 2 + armLength * 2, mid-3);
						surface->LineTo(rcw.left + 2 + armLength, mid + armLength-3);
						surface->LineTo(rcw.left + 2, mid-3);
					} else {
						surface->MoveTo(rcw.left + armLength+5, mid);
						surface->LineTo(rcw.left+5, mid - armLength);
						surface->LineTo(rcw.left+5, mid + armLength);
						surface->LineTo(rcw.left + armLength+5, mid);
					}
				}
			}
		}
		lineDisplay++;
		y++;
	}

	// Blank end.
	int endY = 0;
	lineDisplay = topLine;
	while ((lineDisplay < fs->LinesDisplayed()) && (endY <= PageSize())) {
		lineDisplay++;
		endY++;
	}
	rcw.left = 0;
	rcw.right = 2000;
	rcw.top = endY * lineHeight;
	rcw.bottom = 2000;
	surface->FillRectangle(rcw, RGBColor(0xff,0xff,0xff));
}

void TextView::PaintLineNumberColumn(Surface *surface, SRectangle rcPaint, TextColumn *tc) {

	surface->FillRectangle(rcPaint, RGBColor(0xf0,0xf0,0xf0));

	int startLine = rcPaint.top / lineHeight - 1;
	if (topLine + startLine < 0)
		startLine = 0;
	int endLine = rcPaint.bottom / lineHeight + 1;

	SRectangle rcw = SRectangle();

	int y = startLine;
	int lineDisplay = topLine+startLine;
	surface->SetTextColor(RGBColor(0,0,0));
	while (lineDisplay < fs->LinesDisplayed() && y <= endLine) {
		if (fs->SubLineFromDisplay(lineDisplay) == 0) {
			rcw.top = y * lineHeight;
			rcw.bottom = rcw.top + lineHeight;
			rcw.left = 0;
			rcw.right = tc->right;
			Literal lineNumber = Literal(fs->DocFromDisplay(lineDisplay)+1);
			FontDescription fd = FontDescription(sscMeasure->fontNameDefault, 
				10 + magnification, false, false);
			surface->SetFont(fd);
			TextPositions *tpln = new TextPositions(10);
			SW_BYTE lineNumberUTF8[100];
			int lenUTF8 = UniConversion::UTF8LengthFromUTF32(lineNumber.Data(), lineNumber.Length());
			UniConversion::UTF8FromUTF32(lineNumberUTF8, lenUTF8,
				lineNumber.Data(), lineNumber.Length());
			surface->MeasureWidths(lineNumberUTF8, lenUTF8, tpln, 0);
			rcw.left = rcw.right - tpln->LastPosition() - 2;
			surface->PaintText(lineNumberUTF8, lenUTF8, &rcw, ascent, 0);
			delete tpln;
		}
		lineDisplay++;
		y++;
	}

	// Blank end.
	int endY = 0;
	lineDisplay = topLine;
	while ((lineDisplay < fs->LinesDisplayed()) && (endY <= PageSize())) {
		lineDisplay++;
		endY++;
	}
	rcw.left = 0;
	rcw.right = 2000;
	rcw.top = endY * lineHeight;
	rcw.bottom = 2000;
	surface->FillRectangle(rcw, RGBColor(0xff,0xff,0xff));
}

void TextView::PaintTextColumn(Surface *surface, SRectangle rcPaint, TextColumn *tc) {

	int startLine = rcPaint.top / lineHeight - 1;
	if (topLine + startLine < 0)
		startLine = 0;
	int endLine = rcPaint.bottom / lineHeight + 1;

	SRectangle rcw = SRectangle();

	IPaintPhase  *paints[] = {
		new PhaseBack(),
		new PhaseDecoration(false),
		new PhaseText(),
		new PhaseBox(),
		new PhaseDecoration(true),
		NULL,
	};

	PaintParameters *pp = new PaintParameters();
	pp->pbuffer = pbuffer;
	pp->fs = fs;
	pp->surface = surface;
	pp->leftColumn = leftColumn;
	pp->magnification = magnification;
	pp->visibleWhitespace = visibleWhitespace;
	pp->lineWidth = wrapWidth;
	pp->ascent = ascent;

	InvalidateChangePositions();
	for (int method=0; paints[method] != NULL; method++) {
		int y = startLine;
		int lineDisplay = topLine+startLine;
		pp->ssc = ssc->View(paints[method]->FlagsUse());
		while (lineDisplay < fs->LinesDisplayed() && y <= endLine) {
			rcw.top = y * lineHeight;
			rcw.bottom = rcw.top + lineHeight;
			rcw.left = 0;
			rcw.right = rcPaint.right;
			pp->rcw = rcw;
			pp->left = tc->left;
			pp->lineDoc = fs->DocFromDisplay(lineDisplay);
			pp->subLine = fs->SubLineFromDisplay(lineDisplay);
			int lineStart = pbuffer->PositionFromLine(pp->lineDoc);
			int lineEnd = pbuffer->PositionFromLine(pp->lineDoc+1);
			pp->subLineStart = lineStart;
			pp->subLineEnd = lineEnd;
			if (wrap) {
				int lineFlow = fs->lineFolds->PartitionFromPosition(lineStart) + pp->subLine;
				int lineFlowStart = fs->lineFolds->PositionFromPartition(lineFlow);
				int lineFlowEnd = fs->lineFolds->PositionFromPartition(lineFlow+1);
				if ((lineFlowStart >= lineStart) && (lineFlowEnd <= lineEnd)) {
					pp->subLineStart = lineFlowStart;
					pp->subLineEnd = lineFlowEnd;
				}
			}
			// Find all potential change positions
			pp->cf = ChangePositionsFromLine(pp->lineDoc);
			pp->positions = MeasureText(surface, pp->lineDoc, pp->cf);
			if (accurateWidths) {
				fs->SetWidth(pp->lineDoc, pp->positions->LastPosition());
			}
			paints[method]->Paint(pp);
			lineDisplay++;
			y++;
		}
		delete pp->ssc;
		pp->ssc = NULL;
	}

	delete pp;
	pp = NULL;

	for (int meth=0; paints[meth] != NULL; meth++) {
		delete paints[meth];
	}

	// Blank end.
	int endY = 0;
	int lineBlank = topLine;
	while ((lineBlank < fs->LinesDisplayed()) && (endY <= PageSize())) {
		lineBlank++;
		endY++;
	}
	rcw.left = 0;
	rcw.right = 2000;
	rcw.top = endY * lineHeight;
	rcw.bottom = 2000;
	surface->FillRectangle(rcw, RGBColor(0xff,0xff,0xff));
}

void TextView::AddColumn(int type, int width) {
	TextColumn *tcNew = new TextColumn();
	tcNew->type = type;
	tcNew->width = width;
	if (root == NULL) {
		root = tcNew;
	} else {
		TextColumn *prev = root;
		while (prev->next != NULL) {
			prev = prev->next;
		}
		prev->next = tcNew;
	}
	int x = 0;
	for (TextColumn *tc=root; tc != NULL; tc = tc->next) {
		tc->left = x;
		if (tc->type == TextColumn::tcText) {
			textLeft = x;
			wrapWidth = lineWidth - textLeft;
			if (wrapWidth < 20)
				wrapWidth = 20;
		}
		x += tc->width;
		tc->right = x;
	}
}

