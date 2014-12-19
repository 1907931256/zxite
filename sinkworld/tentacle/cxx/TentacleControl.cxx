// SinkWorld TentacleControl class.
// Copyright 2004 Neil Hodgson

// TentacleControl.cxx, a demonstration application for SinkWorld

#ifndef unix
#include <stdlib.h>
#include <crtdbg.h>
#endif

#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>
#include <string>

#ifdef unix

#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
typedef GtkWidget * HWND;
int min(int a, int b) {
	return a < b ? a : b;
}
int max(int a, int b) {
	return a > b ? a : b;
}

#else

#define _WIN32_WINNT  0x0400
#include <windows.h>
#include <windowsx.h>
#include <richedit.h>

#ifdef _MSC_VER
#pragma warning(disable: 4244 4312)
#endif

// Take care of 32/64 bit pointers
#ifdef GetWindowLongPtr
static void *PointerFromWindow(HWND hWnd, int nIndex) {
	return reinterpret_cast<void *>(::GetWindowLongPtr(hWnd, nIndex));
}
static void SetWindowPointer(HWND hWnd, int nIndex, void *ptr) {
	::SetWindowLongPtr(hWnd, nIndex, reinterpret_cast<LONG_PTR>(ptr));
}
#else
static void *PointerFromWindow(HWND hWnd, int nIndex) {
	return reinterpret_cast<void *>(::GetWindowLong(hWnd, nIndex));
}
static void SetWindowPointer(HWND hWnd, int nIndex, void *ptr) {
	::SetWindowLong(hWnd, nIndex, reinterpret_cast<LONG>(ptr));
}
#endif

#endif

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
#include "RangeSelection.h"

#include "TentacleControl.h"

#ifdef unix

#define VK_ADD GDK_KP_Add
#define VK_SUBTRACT GDK_KP_Subtract
#define VK_LEFT GDK_Left
#define VK_RIGHT GDK_Right
#define VK_HOME GDK_Home
#define VK_END GDK_End
#define VK_UP GDK_Up
#define VK_PRIOR GDK_Page_Up
#define VK_DOWN GDK_Down
#define VK_NEXT GDK_Page_Down
#define VK_BACK GDK_BackSpace
#define VK_DELETE GDK_Delete
#define VK_RETURN GDK_Return
#define VK_TAB GDK_Tab
#define VK_ESCAPE GDK_Escape

void OutputDebugString(char *s) {
	fprintf(stderr, "%s", s);
}

enum {
    TARGET_STRING,
    TARGET_TEXT,
    TARGET_COMPOUND_TEXT,
    TARGET_UTF8_STRING
};

void SetAdjustment(GtkAdjustment *adj, int maxPosition, int pageSize, int pageScroll, int lineSize) {
	if (adj->upper != maxPosition ||
		adj->page_size != pageSize ||
		adj->page_increment != pageScroll ||
		adj->step_increment != lineSize) {
		adj->upper = maxPosition;
		adj->page_size = pageSize;
		adj->page_increment = pageScroll;
		adj->step_increment = lineSize;
		gtk_adjustment_changed(adj);
	}
}

#endif

const char controlClassName[] = "TentacleControlWindow";

/// Measure durations accurately.

class ElapsedTime {
	long bigBit;
	long littleBit;
public:
	ElapsedTime();
	double Duration(bool reset=false);
};

TentacleControl::TentacleControl() {
	wEdit = NULL;
	pdoc = NULL;
	frame = NULL;
	braceLevel = false;
	fancy = false;
	wrap = false;
	wheelDelta = 0;
	doubleClickTime = 500;
	timeLastCaret = 0;
	timeLastClick = 0;
	inDrag = false;
	selType = 0;
	positionFeedBack = 0;
	showFeedBack = false;
	downFeedBack = false;
	wordFeedBack = new Literal();
	idlerState = false;
	idleCount = 0;
	idlesInFlight = 0;
	nextLineWrapped = 100000000;
	goodTopLine = 0;
	pstyleFeedBack = NULL;
	ssSelection = NULL;
	ssCaret = NULL;
	endOfLine = new Literal("\r\n");
	tab = new Literal("\t");
}

TentacleControl::~TentacleControl() {
	delete wordFeedBack;
	wordFeedBack = NULL;
	delete ssSelection;
	ssSelection = NULL;
	delete ssCaret;
	ssCaret = NULL;
	delete endOfLine;
	endOfLine = NULL;
	delete tab;
	tab = NULL;
}

void TentacleControl::Deallocate() {
	pdoc = NULL;
	tv.Deallocate();
	sr.Deallocate();
	delete pstyleFeedBack;
	pstyleFeedBack = 0;
}

void TentacleControl::SetDocument(Document *pdoc_, IDocumentChangeListener *frame_) {
	pdoc = pdoc_;
	frame = frame_;
	Literal litLineNumberWidth("lineNumberWidth");
	int lineNumberWidth = pdoc->options->IntegerModeValue(pdoc->currentMode, &litLineNumberWidth, 4);
	Literal litFoldWidth("foldWidth");
	int foldWidth = pdoc->options->IntegerModeValue(pdoc->currentMode, &litFoldWidth, 1);
	Literal litVisibleWhitespace("visibleWhitespace");
	tv.SetText(pdoc->pbuffer, lineNumberWidth, foldWidth);
	bool visibleWhitespace = pdoc->options->IntegerModeValue(pdoc->currentMode, &litVisibleWhitespace, 0) != 0;
	tv.SetVisibleWhitespace(visibleWhitespace);
	tv.SetStyles(pdoc->ssc);
	sr.Allocate();
	SetSelectionStyles();
	tv.SetFolds(pdoc->GetFolds());
}

void TentacleControl::SetSelectionStyles() {
	tv.RemoveStyleSet(ssSelection);
	delete ssSelection;
	ssSelection = new StyleSet(sr.pstyleSelection, 2);
	Literal styleName(HasFocus() ? "selection.style" : "selection.style.no.focus");
	StyleModification *smSelection = StyleModification::FromOptions(
		pdoc->currentMode, &styleName, pdoc->options, NULL);
	ssSelection->Set(1, smSelection);
	tv.AddStyleSet(ssSelection);

	tv.RemoveStyleSet(ssCaret);
	delete ssCaret;
	ssCaret = new StyleSet(&sr, 2);
	StyleModification *smCaret = new StyleModification();
	Literal litDecoration = Literal("Decoration");
	Literal litCaret = Literal("Caret");
	IDecoration *caret = (IDecoration *)PrototypeRegistry::FromName(&litDecoration, &litCaret);
	//DecorationCaret *caret = new DecorationCaret(1);

	Literal caretName(HasFocus() ? "caret.color" : "caret.color.no.focus");
	RGBColor caretColor = HasFocus() ? RGBColor(0xff,0,0) :  RGBColor(0,0,0);
	Literal *caretValue = pdoc->options->ModeValue(pdoc->currentMode, &caretName);
	if (caretValue != NULL) {
		caretColor = RGBColor(StyleModification::ColourFromLiteral(caretValue));
	}
	caret->SetFore(caretColor);
	smCaret->AddDecorationFore(caret);
	ssCaret->Set(1, smCaret);
	tv.AddStyleSet(ssCaret);

	if (pdoc->pbuffer != NULL) {
		pdoc->pbuffer->DetachStyles(&sr);
		pdoc->pbuffer->AttachStyles(&sr, NULL, 0, false);
	}
}

#ifdef OLDER
void TentacleControl::SetStyles(StyleSetCollection *ssc_, IStyles *pstyle_) {
	pstyle = pstyle_;
	tv.SetStyles(ssc_);
	sr.Allocate();

	SetSelectionStyles();

	// This allows for buttons that appear and can be pushed
	if (fancy) {
		pstyleFeedBack = new RunStyles();
		StyleSet *ssFeedBack = new StyleSet(pstyleFeedBack, 3);
		StyleModification *smFeedBack = new StyleModification();
		smFeedBack->AddDecorationBack(new DecorationButton(false));
		//smFeedBack->AddDecorationBack(new DecorationUnderLine(RGBColor(0x0,0x0,0xff)));
		ssFeedBack->Set(1, smFeedBack);
		StyleModification *smFeedBackHit = new StyleModification();
		smFeedBackHit->AddDecorationBack(new DecorationButton(true));
		//smFeedBackHit->AddDecorationBack(new DecorationUnderLine(RGBColor(0x0,0xff,0xff)));
		ssFeedBack->Set(2, smFeedBackHit);
		tv.AddStyleSet(ssFeedBack);
	}
}
#endif

bool TentacleControl::CheckWrap() {
	if (nextLineWrapped < pdoc->pbuffer->Lines()) {
		bool redisplay = false;
		if (wrap) {
			ElapsedTime et;
			pdoc->pbuffer->LexTo(pdoc->pbuffer->Length(), 0, false);
			double durLex = et.Duration(true);
			char szMessage[100];
			sprintf(szMessage, "LexWrap:%9.6g \n", durLex);
			::OutputDebugString(szMessage);
			tv.InvalidateChangePositions();
			// Get a surface
			Surface *surface = SurfaceForMeasurement();
			if (surface != NULL) {
				for (int line=0;line<20000 && nextLineWrapped < pdoc->pbuffer->Lines();line++) {
					if (nextLineWrapped == goodTopLine)
						redisplay = true;
					tv.WrapLine(surface, nextLineWrapped);
					nextLineWrapped++;
				}
				delete surface;
			}
		} else {
			while (nextLineWrapped < pdoc->pbuffer->Lines()) {
				tv.fs->SetHeight(nextLineWrapped, 1);
				nextLineWrapped++;
			}
		}
		if (redisplay) {
			Invalidate();
			ScrollTo(tv.fs->DisplayFromDoc(goodTopLine), true);
		}
	}
	if (!(nextLineWrapped < pdoc->pbuffer->Lines())) {
		ScrollTo(tv.fs->DisplayFromDoc(goodTopLine), true);
		SetScrollBars();
		Invalidate();
	}
	return nextLineWrapped < pdoc->pbuffer->Lines();
}

int TentacleControl::InsertBytes(int position, SW_BYTE s[], int insertLength) {
	int lenInText = insertLength;
	pdoc->pbuffer->InsertBytes(position, s, insertLength);
	tv.fs->InvalidateWidths();
	frame->StateAltered();
	if (wrap) {
		StartWrap();
	}
	return lenInText;
}

int TentacleControl::InsertUTF32(int position, const int s[], int insertLength) {
	int lenInText = pdoc->pbuffer->InsertUTF32(position, s, insertLength);
	//tv.fs->InvalidateWidths();
	frame->StateAltered();
	if (wrap) {
		StartWrap();
	}
	EnsureCaretVisible();
	return lenInText;
}

int TentacleControl::InsertUTF8(int position, SW_BYTE s[], int insertLength) {
	int len32 = UniConversion::UTF32LengthFromUTF8(s, insertLength);
	int *asUTF32 = new int[len32];
	UniConversion::UTF32FromUTF8(asUTF32, len32, s, insertLength);
	int lenInText = InsertUTF32(position, asUTF32, len32);
	delete []asUTF32;
	return lenInText;
}

bool TentacleControl::DeleteRange(int position, int deleteLength) {
	int lineStart = pdoc->pbuffer->LineFromPosition(position);
	int lineEnd = pdoc->pbuffer->LineFromPosition(position + deleteLength);
	pdoc->pbuffer->DeleteRange(position, deleteLength);
	tv.fs->InvalidateWidths();
	frame->StateAltered();
	if (wrap) {
		StartWrap();
	}
	return lineStart != lineEnd;
}

void TentacleControl::InvalidateRectangle(SRectangle rcInvalidate) {
#ifdef unix
	gtk_widget_queue_draw_area(wEdit,
		rcInvalidate.left, rcInvalidate.top,
		rcInvalidate.right - rcInvalidate.left, rcInvalidate.bottom - rcInvalidate.top);
#else
	::InvalidateRect(wEdit, reinterpret_cast<RECT *>(&rcInvalidate), 0);
#endif
}

void TentacleControl::InvalidateRange(int a, int b) {
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
	int lineDocBottom = pdoc->pbuffer->LineFromPosition(b) + 1;
	int lineBottom = tv.fs->DisplayFromDoc(lineDocBottom);
	SRectangle rcInvalidate;
	rcInvalidate.left = 0;
	rcInvalidate.right = 2000;	// TODO: make as wide as window
	rcInvalidate.top = (lineTop - tv.topLine) * tv.lineHeight;
	rcInvalidate.bottom = (lineBottom - tv.topLine) * tv.lineHeight;
	// Allow for decorators that go 2 pixels beyond line.
	rcInvalidate.top -= 2;
	rcInvalidate.bottom += 2;
	InvalidateRectangle(rcInvalidate);
}

void TentacleControl::Invalidate() {
#ifdef unix
	gtk_widget_queue_draw(wEdit);
#else
	::InvalidateRect(wEdit, 0, 0);
#endif
}

bool TentacleControl::PerformIdle(bool checkIfInIdle) {
	if (checkIfInIdle && !idlerState) {
		return false;
	}
	return CheckWrap();
}

void TentacleControl::SizeChanged() {
	SRectangle rcClient;
	GetViewRectangle(&rcClient);
	tv.SetRectangle(&rcClient);
	nextLineWrapped = 0;
	if (!idlerState) {
		goodTopLine = tv.fs->DocFromDisplay(tv.topLine);
	}
	SetIdle(true);
	SetScrollBars();
}

void TentacleControl::ScrollTo(int line, bool updateScrollBar) {
//fprintf(stderr, "ScrollTo %d %d %d \n", tv.topLine, line, updateScrollBar);
	if (line > tv.MaxScrollPos())
		line = tv.MaxScrollPos();
	if (line < 0)
		line = 0;
	if (tv.topLine != line) {
		int change = tv.topLine - line;
		tv.topLine = line;

		if (abs(change) < 10) {
			EditScrollWindow(change * tv.lineHeight);
		} else {
			//~ SRectangle rcClient;
			//~ GetViewRectangle(&rcClient);
			//~ rcClient.bottom -= rcClient.top;
			//~ rcClient.right -= rcClient.left;
			//~ rcClient.top = 0;
			//~ rcClient.left = 0;
			//~ InvalidateRectangle(rcClient);
			Invalidate();
		}

		if (updateScrollBar) {
			SetScrollPos(line);
		}
	}
}

void TentacleControl::PaintRect(Surface &surface, SRectangle rcPaint) {
	tv.PaintRect(&surface, rcPaint);
	SetScrollBars();
}

void TentacleControl::SetSelection(int newCaret, int newAnchor) {
	sr.SetSelectionStyleRanges(newAnchor, newCaret);
	SetPrimary();
}

void TentacleControl::MoveCaret(int newCaret, bool retainCurrentAnchor, bool updateSynchronous) {
	if (newCaret < 0) {
		newCaret = 0;
	} else if (newCaret > pdoc->pbuffer->Length()) {
		newCaret = pdoc->pbuffer->Length();
	}
	int delta = newCaret - sr.Caret();
	newCaret = pdoc->MovePositionOutsideChar(newCaret, delta, true);
	int newAnchor = sr.Anchor();
	if (!retainCurrentAnchor) {
		newAnchor = newCaret;
	}
	if ((sr.Caret() != newCaret) || (sr.Anchor() != newAnchor)) {
		CaretOn();
		InvalidateRange(min(min(sr.StartSelection(), newCaret), newAnchor),
			max(max(sr.EndSelection(), newCaret), newAnchor));
		SetSelection(newCaret, newAnchor);
		if (updateSynchronous) {
#ifdef unix
			gdk_window_process_updates(wEdit->window, FALSE);
#else
			::UpdateWindow(wEdit);
#endif
		}
		EnsureCaretVisible();
	}
}

void TentacleControl::CheckLineForInvalidate(int position) {
	int line = pdoc->pbuffer->LineFromPosition(position);
	int positionCheck = pdoc->pbuffer->PositionFromLine(line+1);
	bool changedBeyondLine = pdoc->pbuffer->LexTo(positionCheck, 0, true);
	if (changedBeyondLine) {
		frame->Invalidate();
	} else {
		frame->InvalidateRange(position, position);
	}
}

void TentacleControl::LineDuplicate() {
	int line = pdoc->pbuffer->LineFromPosition(sr.Caret());
	int start = pdoc->pbuffer->PositionFromLine(line);
	int end = pdoc->pbuffer->LineEndPositionFromLine(line);
	int len = end - start;
	int *thisLine = new int[len];
	int lenCharacters = pdoc->pbuffer->RetrieveUTF32(start, thisLine, len);
	int lenInsert = InsertUTF32(end, endOfLine->Data(), endOfLine->Length());
	InsertUTF32(end + lenInsert, thisLine, lenCharacters);
	delete []thisLine;
	Invalidate();
}

bool TentacleControl::KeyPressed(int keyCode, bool control, bool shift, long timeMessage) {
	timeEvent = timeMessage;
	if (keyCode == VK_ADD && control) {
		tv.ModifyMagnification(1);
		Invalidate();
	} else if (keyCode == VK_SUBTRACT && control) {
		tv.ModifyMagnification(-1);
		Invalidate();
	} else if (keyCode == VK_LEFT && control) {
		MoveCaret(pdoc->NextWordStart(sr.Caret()-1, -1), shift);
	} else if (keyCode == VK_LEFT) {
		int newCaret = pdoc->MovePositionOutsideChar(sr.Caret()-1, -1, true);
		int line = pdoc->pbuffer->LineFromPosition(newCaret);
		if (!tv.fs->GetVisible(line))
			newCaret = pdoc->pbuffer->LineEndPositionFromLine(
				tv.fs->DocFromDisplay(tv.fs->DisplayFromDoc(line) - 1));
		MoveCaret(newCaret, shift);
	} else if (keyCode == VK_RIGHT && control) {
		MoveCaret(pdoc->NextWordStart(sr.Caret()+1, 1), shift);
	} else if (keyCode == VK_RIGHT) {
		int newCaret = pdoc->MovePositionOutsideChar(sr.Caret()+1, 1, true);
		int line = pdoc->pbuffer->LineFromPosition(newCaret);
		if (!tv.fs->GetVisible(line))
			newCaret = pdoc->pbuffer->PositionFromLine(
				tv.fs->DocFromDisplay(tv.fs->DisplayFromDoc(line)));
		MoveCaret(newCaret, shift);
	} else if (keyCode == VK_HOME) {
		int line = control ? 0 : pdoc->pbuffer->LineFromPosition(sr.Caret());
		MoveCaret(pdoc->pbuffer->PositionFromLine(line), shift);
	} else if (keyCode == VK_END) {
		int line = control ? pdoc->pbuffer->Lines() : pdoc->pbuffer->LineFromPosition(sr.Caret());
		MoveCaret(pdoc->pbuffer->LineEndPositionFromLine(line), shift);
	} else if (keyCode == VK_UP) {
		int lineDisplay = tv.DisplayLineFromPosition(sr.Caret()) - 1;
		if (lineDisplay < 0) {
			lineDisplay = 0;
		}
		MoveCaret(pdoc->pbuffer->PositionFromLine(
			tv.fs->DocFromDisplay(lineDisplay)), shift);
	} else if (keyCode == VK_PRIOR) {
		int lineDisplay = tv.DisplayLineFromPosition(sr.Caret()) - tv.PageSize();
		if (lineDisplay < 0) {
			lineDisplay = 0;
		}
		MoveCaret(pdoc->pbuffer->PositionFromLine(
			tv.fs->DocFromDisplay(lineDisplay)), shift, false);
	} else if (keyCode == VK_DOWN) {
		int lineDisplay = tv.DisplayLineFromPosition(sr.Caret()) + 1;
		if (lineDisplay > tv.fs->DisplayFromDoc(pdoc->pbuffer->Lines())) {
			lineDisplay = tv.fs->DisplayFromDoc(pdoc->pbuffer->Lines());
		}
		MoveCaret(pdoc->pbuffer->PositionFromLine(
			tv.fs->DocFromDisplay(lineDisplay)), shift);
	} else if (keyCode == VK_NEXT) {
		int lineDisplay = tv.DisplayLineFromPosition(sr.Caret()) + tv.PageSize();
		if (lineDisplay > tv.fs->DisplayFromDoc(pdoc->pbuffer->Lines())) {
			lineDisplay = tv.fs->DisplayFromDoc(pdoc->pbuffer->Lines());
		}
		MoveCaret(pdoc->pbuffer->PositionFromLine(
			tv.fs->DocFromDisplay(lineDisplay)), shift, false);
	} else if (keyCode == 'V' && control) {
		Paste();
	} else if (keyCode == 'C' && control) {
		Copy();
	} else if (keyCode == 'X' && control) {
		Cut();
	} else if (keyCode == 'Z' && control) {
		Undo();
	} else if (keyCode == 'Y' && control) {
		Redo();
	} else if (keyCode == 'A' && control) {
		SelectAll();
	} else if (keyCode == 'D' && control) {
		LineDuplicate();
	} else if (keyCode == VK_BACK) {
		if (control) {
			int startDelete = pdoc->NextWordStart(sr.Caret()-1, -1);
			if (DeleteRange(startDelete, sr.Caret()-startDelete)) {
				Invalidate();
			}
			CheckLineForInvalidate(sr.Caret());
		} else if (sr.Empty()) {
			if (sr.Caret() > 0) {
				int startDelete = pdoc->MovePositionOutsideChar(sr.Caret() - 1, -1, true);
				if (DeleteRange(startDelete, sr.Caret()-startDelete)) {
					Invalidate();
				}
				CheckLineForInvalidate(sr.Caret());
			}
		} else {
			Clear();
		}
	} else if (keyCode == VK_DELETE) {
		if (control) {
			int endDelete = pdoc->NextWordStart(sr.Caret(), 1);
			if (DeleteRange(sr.Caret(), endDelete-sr.Caret())) {
				Invalidate();
			}
			CheckLineForInvalidate(sr.Caret());
		} else if (sr.Empty()) {
			if (sr.Caret() < pdoc->pbuffer->Length()) {
				int endDelete = pdoc->MovePositionOutsideChar(sr.Caret() + 1, 1, true);
				if (DeleteRange(sr.Caret(), endDelete-sr.Caret())) {
					Invalidate();
				}
				CheckLineForInvalidate(sr.Caret());
			}
		} else {
			Clear();
		}
	} else if (keyCode == VK_RETURN) {
		if (!sr.Empty()) {
			Clear();
		}
		InsertUTF32(sr.Caret(), endOfLine->Data(), endOfLine->Length());
		//assert(pdoc->pbuffer->PositionFromLine(pdoc->pbuffer->Lines()) == pdoc->pbuffer->Length());
		Invalidate();
	} else if (keyCode == VK_TAB) {
		if (!sr.Empty()) {
			Clear();
			Invalidate();
		}
		InsertUTF32(sr.Caret(), tab->Data(), tab->Length());
		//assert(pdoc->pbuffer->PositionFromLine(pdoc->pbuffer->Lines()) == pdoc->pbuffer->Length());
		CheckLineForInvalidate(sr.Caret()+1);
	} else if (keyCode == VK_ESCAPE) {
#ifdef unix
		//SelectPane(paneContentOnly);
#endif
	} else if (!control && (keyCode >= ' ') && (keyCode <= 127)) {
		// No action
		return false;
	}
	return true;
}

void TentacleControl::CharPressed(int charPressed) {
	if ((charPressed >= ' ') && (charPressed < 127)) {
		if (!sr.Empty()) {
			Clear();
			Invalidate();
		}
		int charUTF32[] = { charPressed };
		InsertUTF32(sr.Caret(), charUTF32, 1);
		CheckLineForInvalidate(sr.Caret());
	}
}

void TentacleControl::Undo() {
	int position = pdoc->pbuffer->PerformUndoStep();
	if (position >= 0) {
		SetSelection(position);
	}
	Invalidate();
	frame->StateAltered();
}

void TentacleControl::Redo() {
	int position = pdoc->pbuffer->PerformRedoStep();
	if (position >= 0) {
		SetSelection(position);
	}
	Invalidate();
	frame->StateAltered();
}

void TentacleControl::Cut() {
	Copy();
	Clear();
	Invalidate();
}

void TentacleControl::Clear() {
	int start = sr.StartSelection();
	int end = sr.EndSelection();
	DeleteRange(start, end - start);
	SetSelection(start);
	Invalidate();
}

void TentacleControl::SelectAll() {
	SetSelection(0, pdoc->pbuffer->Length());
	Invalidate();
}

void TentacleControl::FoldLine(int currentLine, FoldCommand cmd) {
	int maxLine = pdoc->pbuffer->Lines();
	IStyles *foldLevels = pdoc->GetFolds();
	pdoc->pbuffer->LexTo(pdoc->pbuffer->Length(), 0, false);
	int currentLevel = foldLevels->ValueAt(currentLine) & 0xFFFF;
	int line = currentLine;
	int level = currentLevel + 1;
	bool showing = cmd == fcShow;
	if (cmd == fcDiscover)
		showing = !tv.fs->GetExpanded(currentLine);
	tv.fs->SetExpanded(currentLine, showing);
	while ((level > currentLevel) && (line < maxLine-1)) {
		line++;
		level = foldLevels->ValueAt(line) & 0xFFFF;
		if (level > currentLevel)
			tv.fs->SetVisible(line, line, showing);
	}
	Invalidate();
}

void TentacleControl::ToggleFold() {
	FoldLine(pdoc->pbuffer->LineFromPosition(sr.Caret()), fcDiscover);
}

void TentacleControl::FoldAll(FoldCommand cmd) {
	for (int line=0;line<pdoc->pbuffer->Lines()-1;line++) {
		FoldLine(line, cmd);
	}
}

void TentacleControl::ToggleBraceLevel() {
	braceLevel = !braceLevel;
	Invalidate();
	StartWrap();
}

void TentacleControl::ToggleFancy() {
	fancy = !fancy;
	Invalidate();
	StartWrap();
}

void TentacleControl::StartWrap() {
	nextLineWrapped = 0;
	goodTopLine = tv.fs->DocFromDisplay(tv.topLine);
	SetScrollBars();
	SetIdle(true);
}

void TentacleControl::ToggleWrap() {
	wrap = !wrap;
	tv.wrap = wrap;
	StartWrap();
}

void TentacleControl::EnsureCaretVisible() {
	int line = tv.DisplayLineFromPosition(sr.Caret());
	Point pt(0, (line-tv.topLine) * tv.lineHeight);
	Point ptBottomCaret = pt;
	ptBottomCaret.y += tv.lineHeight - 1;
	int lineCaret = line;

	// Vertical positioning
	if (pt.y < 0 || ptBottomCaret.y > tv.pageHeight) {
		int newTopLine = tv.topLine;
		if (lineCaret < tv.topLine) {
			// Caret goes too high
			newTopLine = lineCaret;
		} else if (lineCaret > tv.topLine + tv.PageSize() - 1) {
			// Caret goes too low
			newTopLine = lineCaret - tv.PageSize() + 1;
		}
		ScrollTo(newTopLine, true);
	}
}

void TentacleControl::MouseDrag(Point pt) {
	if (!downFeedBack) {
		if (!(mouseDragLast == pt)) {
			mouseDragLast = pt;
			MoveCaret(PositionFromLocation(mouseDragLast), true);
		}
	}
}

Literal *TentacleControl::LiteralFromRange(int position, int length) {
	int len = pdoc->pbuffer->Characters(position, length);
	int *utf = new int[len];
	pdoc->pbuffer->RetrieveUTF32(position, utf, len);
	Literal *lit = new Literal(utf, 0, len);
	delete []utf;
	return lit;
}

Literal *TentacleControl::WordAtPosition(int position) {
	int start = pdoc->ExtendWordSelect(position, -1);
	int end = pdoc->ExtendWordSelect(position, 1);
	return LiteralFromRange(start, end - start);
}

void TentacleControl::ShowFeedBack(int position, bool force) {
	if (force || (positionFeedBack != position)) {
		positionFeedBack = position;
		if (pstyleFeedBack) {
			pstyleFeedBack->DeleteAll();
			pstyleFeedBack->InsertSpace(0, pdoc->pbuffer->Length());
			delete wordFeedBack;
			wordFeedBack = WordAtPosition(position);
			if (showFeedBack && wordFeedBack->Length()) {
				int start = pdoc->ExtendWordSelect(position, -1);
				pstyleFeedBack->FillRange(start, downFeedBack ? 2 : 1,
					wordFeedBack->Length());
			}
			Invalidate();
		}
	}
}

void TentacleControl::MouseFeedBack(Point pt) {
	ShowFeedBack(PositionFromLocation(pt), false);
}

void TentacleControl::MouseDown(Point pt, long timeMessage) {
	timeEvent = timeMessage;
	Focus();
#ifdef unix
#else
	::SetCapture(wEdit);
#endif
	int posPoint = PositionFromLocation(pt);
	if (tv.ColumnTypeFromLocation(pt) == TextColumn::tcFold) {
		FoldLine(pdoc->pbuffer->LineFromPosition(posPoint), fcDiscover);
		return;
	}
	if (pstyleFeedBack && pstyleFeedBack->ValueAt(posPoint)) {
		downFeedBack = !downFeedBack;
		ShowFeedBack(posPoint, true);
		return;
	}
	inDrag = true;
	mouseDragLast = pt;
	if (((timeMessage - timeLastClick) < doubleClickTime) &&
		(lastClickAt.DistanceSquared(pt) < 20)) {
		selType = (selType + 1) % 3;
	} else {
		selType = 0;
	}
	if (selType == 0) {
		MoveCaret(posPoint, false);
	} else if (selType == 1) {	// Word selection
		SetSelection(pdoc->ExtendWordSelect(posPoint, 1),
			pdoc->ExtendWordSelect(posPoint, -1));
		Invalidate();
		EnsureCaretVisible();
	} else if (selType == 2) {	// Line selection
		int line = pdoc->pbuffer->LineFromPosition(posPoint);
		SetSelection(pdoc->pbuffer->PositionFromLine(line+1),
			pdoc->pbuffer->PositionFromLine(line));
		Invalidate();
		EnsureCaretVisible();
	}
	lastClickAt = pt;
	timeLastClick = timeMessage;
}

void TentacleControl::MouseUp(Point pt) {
#ifdef unix
#else
	::ReleaseCapture();
#endif
	inDrag = false;
	if (downFeedBack) {
		int posPoint = PositionFromLocation(pt);
		downFeedBack = !downFeedBack;
		ShowFeedBack(posPoint, true);
		//DisplayMessage(wordFeedBack.AsCharPointer());
	}
}

void TentacleControl::CaretOn() {
	timeLastCaret = timeEvent;
	sr.SetOn(true);
	InvalidateRange(sr.Caret(), sr.Caret());
}

void TentacleControl::Tick(long timeMessage) {
	if ((inDrag) && (timeMessage > (timeLastClick + 200))) {
		MoveCaret(PositionFromLocation(mouseDragLast), true);
	}
	if (timeMessage > (timeLastCaret + 500)) {
		timeLastCaret = timeMessage;
		sr.SetOn(!sr.IsOn());
		InvalidateRange(sr.Caret(), sr.Caret());
	}
}

Wrapper *TentacleControl::wrapper = 0;

void TentacleControl::SetWrapper(Wrapper *wrap_) {
	wrapper = wrap_;
}

#ifdef unix

///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////// Unix ////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////

ElapsedTime::ElapsedTime() {
	GTimeVal curTime;
	g_get_current_time(&curTime);
	bigBit = curTime.tv_sec;
	littleBit = curTime.tv_usec;
}

double ElapsedTime::Duration(bool reset) {
	GTimeVal curTime;
	g_get_current_time(&curTime);
	long endBigBit = curTime.tv_sec;
	long endLittleBit = curTime.tv_usec;
	double result = 1000000.0 * (endBigBit - bigBit);
	result += endLittleBit - littleBit;
	result /= 1000000.0;
	if (reset) {
		bigBit = endBigBit;
		littleBit = endLittleBit;
	}
	return result;
}

/// GTK+ specific implementation of TentaclControl.

class TentacleControlG : public TentacleControl {
private:
	GtkWidget *boxContent;
	GtkObject *adjustmentv;
	GtkWidget *scrollbarv;
	GtkObject *adjustmenth;
	GtkWidget *scrollbarh;
	GdkAtom atomClipboard;
	GdkAtom atomPrimary;
	unsigned int idlerID;

	virtual HWND TopWindow();
	virtual bool HasFocus();
	virtual void Focus();
	virtual Surface *SurfaceForMeasurement();
	virtual int PositionFromLocation(Point pt);
	virtual void SetPosition(SRectangle *rc);
	virtual void GetViewRectangle(SRectangle *rc);
	virtual void CreateControl(HWND wParent);
	virtual void SetScrollPos(int line);
	virtual void SetScrollBars();
	virtual void Copy();
	virtual void Paste();
	virtual void SetPrimary();
	virtual void SetIdle(bool on);

	gint Idle();
	static gint IdleCallback(TentacleControlG *tc) {
		return tc->Idle();
	}
	gint EditKey(GdkEventKey *event);
	static gint KeySignal(GtkWidget * /*widget*/, GdkEventKey *event, TentacleControlG *tc) {
		return tc->EditKey(event);
	}
	static gint KeyUpSignal(GtkWidget * /*widget*/, GdkEventKey *event, TentacleControlG *tc) {
		return FALSE;
	}
	gint EditMotion(GdkEventMotion *event);
	static gint MotionSignal(GtkWidget * /*widget*/, GdkEventMotion *event, TentacleControlG *tc) {
		return tc->EditMotion(event);
	}
	gint ScrollWheel(GdkEventScroll *event);
	static gint ScrollWheelSignal(GtkWidget * /*widget*/, GdkEventScroll *event, TentacleControlG *tc) {
		return tc->ScrollWheel(event);
	}
	gint EditPress(GdkEventButton *event);
	static gint PressSignal(GtkWidget * /*widget*/, GdkEventButton *event, TentacleControlG *tc) {
		return tc->EditPress(event);
	}
	gint EditRelease(GdkEventButton *event);
	static gint ReleaseSignal(GtkWidget * /*widget*/, GdkEventButton *event, TentacleControlG *tc) {
		return tc->EditRelease(event);
	}
	void EditScroll(GtkAdjustment *adj);
	static void ScrollSignal(GtkAdjustment *adj, TentacleControlG *tc) {
		tc->EditScroll(adj);
	};
	void EditScrollH(GtkAdjustment *adj);
	static void ScrollSignalH(GtkAdjustment *adj, TentacleControlG *tc) {
		tc->EditScrollH(adj);
	};
	gint EditPaint(GtkWidget *widget, GdkEventExpose *ose);
	static gint ExposeSignal(GtkWidget *widget, GdkEventExpose *ose, TentacleControlG *tc) {
		return tc->EditPaint(widget, ose);
	}
	gint FocusIn(GdkEventFocus *event);
	static gint FocusInSignal(GtkWidget * /*widget*/, GdkEventFocus *event, TentacleControlG *tc) {
		return tc->FocusIn(event);
		return FALSE;
	}
	gint FocusOut(GdkEventFocus *event);
	static gint FocusOutSignal(GtkWidget * /*widget*/, GdkEventFocus *event, TentacleControlG *tc) {
		return tc->FocusOut(event);
	}
	static gint MoveResize(GtkWidget *, GtkAllocation *, TentacleControlG *tc) {
		tc->SizeChanged();
		return FALSE;
	};
	void EditScrollWindow(int pixels);
public:
	TentacleControlG() {
	}
	virtual ~TentacleControlG() {
	}
};

HWND TentacleControlG::TopWindow() {
	return boxContent;
}

bool TentacleControlG::HasFocus() {
	if (wEdit == NULL)
		return false;
	else
		return GTK_WIDGET_HAS_FOCUS(wEdit);
}

void TentacleControlG::Focus() {
	gtk_widget_grab_focus(wEdit);
}

Surface *TentacleControlG::SurfaceForMeasurement() {
	if (wEdit->window != NULL)
		return new Surface(wEdit->window, wEdit);
	else
		return NULL;
}

int TentacleControlG::PositionFromLocation(Point pt) {
	Surface surface(wEdit->window, wEdit);
	return tv.PositionFromSurfaceLocation(&surface, pt);
}

void TentacleControlG::SetPosition(SRectangle *rc) {
	// TODO: support window movement on GTK+
	//::SetWindowPos(wEdit,
	//	0, rc->left, rc->top, rc->right - rc->left, rc->bottom - rc->top,
	//	SWP_NOZORDER|SWP_NOACTIVATE);
}

void TentacleControlG::GetViewRectangle(SRectangle *rc) {
	rc->left = wEdit->allocation.x;
	rc->top = wEdit->allocation.y;
	rc->right = rc->left + wEdit->allocation.width - 1;
	rc->bottom = rc->top + wEdit->allocation.height - 1;
}

void TentacleControlG::SetPrimary() {
	int start = sr.StartSelection();
	int end = sr.EndSelection();
	int len = end - start;
	GtkClipboard *clipBoard =
		gtk_widget_get_clipboard(GTK_WIDGET(wEdit), atomPrimary);
	if (clipBoard) {
		ByteBuffer selection(len+1);
		pdoc->pbuffer->RetrieveBytes(start, selection, len);
		selection[len] = '\0';
		gtk_clipboard_set_text(clipBoard, selection.AsCharPointer(), len);
	}
}

void TentacleControlG::SetIdle(bool on) {
	if (idlerState != on) {
		if (on) {
			idlerID = gtk_idle_add((GtkFunction)IdleCallback, this);
		} else {
			gtk_idle_remove(idlerID);
		}
		idlerState = on;
	}
}

void TentacleControlG::SetScrollPos(int line) {
	gtk_adjustment_set_value(GTK_ADJUSTMENT(adjustmentv), line);
}

void TentacleControlG::SetScrollBars() {
	SetAdjustment(GTK_ADJUSTMENT(adjustmentv),
		tv.fs->DisplayFromDoc(pdoc->pbuffer->Lines()), tv.PageSize(), tv.PageSize(), 1);
	if (wrap) {
		tv.leftColumn = 0;
		SetAdjustment(GTK_ADJUSTMENT(adjustmenth), 1, 1, 1, 1);
	} else {
		int maxWidth = tv.fs->GetMaxWidth();
		int widthExtra = maxWidth;
		if (widthExtra < 1)
			widthExtra = 1;
		SetAdjustment(GTK_ADJUSTMENT(adjustmenth), widthExtra, tv.wrapWidth, 120, 1);
	}
}

void TentacleControlG::Copy() {
	int start = sr.StartSelection();
	int end = sr.EndSelection();
	int len = end - start;
	GtkClipboard *clipBoard =
		gtk_widget_get_clipboard(GTK_WIDGET(wEdit), atomClipboard);
	if (clipBoard) {
		ByteBuffer selection(len*3+1);
		int lenUTF8 = pdoc->pbuffer->RetrieveUTF8(start, selection, len);
		selection[lenUTF8] = '\0';
		gtk_clipboard_set_text(clipBoard, selection.AsCharPointer(), len);
	}
}

void TentacleControlG::Paste() {
	GtkClipboard *clipBoard =
		gtk_widget_get_clipboard(GTK_WIDGET(wEdit), atomClipboard);
	if (clipBoard) {
		gchar *ptr = gtk_clipboard_wait_for_text(clipBoard);
		if (ptr) {
			Clear();
			int len = strlen(ptr);
			int *clipUTF32 = new int[len];
			int lenUTF32 = UniConversion::UTF32FromUTF8(clipUTF32, len, (SW_BYTE *)ptr, len);
			InsertUTF32(sr.Caret(), clipUTF32, lenUTF32);
			delete []clipUTF32;
			g_free(ptr);
		}
	}
}

gint TentacleControlG::Idle() {
	if (!PerformIdle(true)) {
		SetIdle(false);
	}
	return FALSE;
}

gint TentacleControlG::EditKey(GdkEventKey *event) {
	bool control = event->state & GDK_CONTROL_MASK;
	bool shift = event->state & GDK_SHIFT_MASK;
	bool handled = KeyPressed(event->keyval, control, shift, event->time);
	//fprintf(stderr, "EditKey %x %c %c %c\n", event->keyval, control ? 'C' : '-', shift ? 'S' : '-', handled ? 'H' : '-');
	if (handled) {
		gtk_signal_emit_stop_by_name(GTK_OBJECT(wEdit), "key_press_event");
	} else {
		CharPressed(event->keyval);
	}
	return TRUE;
}

gint TentacleControlG::EditMotion(GdkEventMotion *event) {
	if (event->window != wEdit->window)
		return FALSE;
	int x = 0;
	int y = 0;
	GdkModifierType state;
	if (event->is_hint) {
		gdk_window_get_pointer(event->window, &x, &y, &state);
	} else {
		x = static_cast<int>(event->x);
		y = static_cast<int>(event->y);
		state = static_cast<GdkModifierType>(event->state);
	}
	Point pt(x,y);
//~ fprintf(stderr, "Motion %3d %3d %x %x\n", x, y, state, state & GDK_BUTTON1_MASK);
	if (state & GDK_BUTTON1_MASK) {
		MouseDrag(pt);
	} else {
		showFeedBack = state & GDK_CONTROL_MASK;
		MouseFeedBack(pt);
	}
	return 0;
}

gint TentacleControlG::ScrollWheel(GdkEventScroll *event) {
	ScrollTo(tv.topLine +
		((event->direction == GDK_SCROLL_UP) ? -4 : 4), true);
	return FALSE;
}

gint TentacleControlG::EditPress(GdkEventButton *event) {
	if (event->type != GDK_BUTTON_PRESS)
		return FALSE;
	int x = 0;
	int y = 0;
	GdkModifierType state;
	gdk_window_get_pointer(wEdit->window, &x, &y, &state);
	MouseDown(Point(x,y), event->time);
	return FALSE;
}

gint TentacleControlG::EditRelease(GdkEventButton *event) {
	int x = 0;
	int y = 0;
	GdkModifierType state;
	gdk_window_get_pointer(wEdit->window, &x, &y, &state);
	MouseUp(Point(x,y));
	return 0;
}

void TentacleControlG::EditScroll(GtkAdjustment *adj) {
//fprintf(stderr, "EditScroll %d %d \n", tv.topLine, adj->value);
	ScrollTo(static_cast<int>(adj->value), false);
}

void TentacleControlG::EditScrollWindow(int pixels) {
//fprintf(stderr, "ScrollWindow %d %d \n", tv.topLine, pixels);
	if (wEdit->window) {
		gdk_window_scroll(wEdit->window, 0, pixels);
		gdk_window_process_updates(wEdit->window, FALSE);
	}
}

void TentacleControlG::EditScrollH(GtkAdjustment *adj) {
	tv.leftColumn = static_cast<int>(adj->value);
	Invalidate();
}

gint TentacleControlG::EditPaint(GtkWidget *widget, GdkEventExpose *ose) {
	SRectangle rcPaint;
	rcPaint.left = ose->area.x;
	rcPaint.top = ose->area.y;
	rcPaint.right = ose->area.x + ose->area.width;
	rcPaint.bottom = ose->area.y + ose->area.height;
	Surface surface(widget->window, widget);
	PaintRect(surface, rcPaint);
	return 0;
}

gint TentacleControlG::FocusIn(GdkEventFocus *event) {
	//GTK_WIDGET_SET_FLAGS(wEdit, GTK_HAS_FOCUS);
	SetSelectionStyles();
	Invalidate();
	frame->StateAltered();
	return FALSE;
}

gint TentacleControlG::FocusOut(GdkEventFocus *event) {
	//GTK_WIDGET_UNSET_FLAGS(wEdit, GTK_HAS_FOCUS);
	SetSelectionStyles();
	Invalidate();
	frame->StateAltered();
	return FALSE;
}

void TentacleControlG::CreateControl(HWND wParent) {
	atomClipboard = gdk_atom_intern("CLIPBOARD", FALSE);
	atomPrimary = gdk_atom_intern("PRIMARY", FALSE);

	boxContent =  gtk_table_new(2, 2, FALSE);
	//gtk_hbox_new(FALSE, 0);
	GTK_WIDGET_UNSET_FLAGS(boxContent, GTK_CAN_FOCUS);
	wEdit = gtk_drawing_area_new();
	GTK_WIDGET_SET_FLAGS(wEdit, GTK_CAN_FOCUS);
	GTK_WIDGET_SET_FLAGS(wEdit, GTK_SENSITIVE);
	gtk_widget_set_events(wEdit,
		GDK_EXPOSURE_MASK
		| GDK_KEY_PRESS_MASK
		| GDK_KEY_RELEASE_MASK
		| GDK_LEAVE_NOTIFY_MASK
		| GDK_BUTTON_PRESS_MASK
		| GDK_BUTTON_RELEASE_MASK
		| GDK_POINTER_MOTION_MASK
		| GDK_POINTER_MOTION_HINT_MASK);
	char *gthis = reinterpret_cast<char *>(this);
	gtk_signal_connect(GTK_OBJECT(wEdit), "size_allocate", GtkSignalFunc(MoveResize), gthis);
	gtk_signal_connect(GTK_OBJECT(wEdit), "key_press_event", GtkSignalFunc(KeySignal), gthis);
	gtk_signal_connect(GTK_OBJECT(wEdit), "key_release_event", GtkSignalFunc(KeyUpSignal), gthis);
	gtk_signal_connect(GTK_OBJECT(wEdit), "motion_notify_event", GtkSignalFunc(MotionSignal), gthis);
	gtk_signal_connect(GTK_OBJECT(wEdit), "scroll_event", GtkSignalFunc(ScrollWheelSignal), gthis);
	gtk_signal_connect(GTK_OBJECT(wEdit), "button_press_event", GtkSignalFunc(PressSignal), gthis);
	gtk_signal_connect(GTK_OBJECT(wEdit), "button_release_event", GtkSignalFunc(ReleaseSignal), gthis);
	gtk_signal_connect(GTK_OBJECT(wEdit), "expose_event", GtkSignalFunc(ExposeSignal), gthis);
	gtk_signal_connect(GTK_OBJECT(wEdit), "focus_in_event", GtkSignalFunc(FocusInSignal), gthis);
	gtk_signal_connect(GTK_OBJECT(wEdit), "focus_out_event", GtkSignalFunc(FocusOutSignal), gthis);

	GtkAttachOptions opts;
	opts = static_cast<GtkAttachOptions>(GTK_SHRINK | GTK_FILL | GTK_EXPAND);
	gtk_table_attach(GTK_TABLE(boxContent), wEdit, 0, 1, 0, 1, opts, opts, 0, 0);

	gtk_widget_set_usize(boxContent, 600, 350);

	adjustmentv = gtk_adjustment_new(0.0, 0.0, 201.0, 1.0, 20.0, 20.0);
	scrollbarv = gtk_vscrollbar_new(GTK_ADJUSTMENT(adjustmentv));
	GTK_WIDGET_UNSET_FLAGS(scrollbarv, GTK_CAN_FOCUS);
	gtk_signal_connect(GTK_OBJECT(adjustmentv), "value_changed",
	                   GTK_SIGNAL_FUNC(ScrollSignal), this);
	opts = static_cast<GtkAttachOptions>(GTK_SHRINK | GTK_FILL);
	gtk_table_attach(GTK_TABLE(boxContent), scrollbarv, 1, 2, 0, 1, opts, opts, 0, 0);

	adjustmenth = gtk_adjustment_new(0.0, 0.0, 1201.0, 8.0, 120.0, 120.0);
	scrollbarh = gtk_hscrollbar_new(GTK_ADJUSTMENT(adjustmenth));
	GTK_WIDGET_UNSET_FLAGS(scrollbarh, GTK_CAN_FOCUS);
	gtk_signal_connect(GTK_OBJECT(adjustmenth), "value_changed",
	                   GTK_SIGNAL_FUNC(ScrollSignalH), this);
	opts = static_cast<GtkAttachOptions>(GTK_SHRINK | GTK_FILL);
	gtk_table_attach(GTK_TABLE(boxContent), scrollbarh, 0, 1, 1, 2, opts, opts, 0, 0);
}

TentacleControl *TentacleControl::Allocate() {
	return new TentacleControlG();
}

#else

///////////////////////////////////////////////////////////////////////////////////
////////////////////////////////// Windows //////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////

static bool initialisedET = false;
static bool usePerformanceCounter = false;
static LARGE_INTEGER frequency;

ElapsedTime::ElapsedTime() {
	if (!initialisedET) {
		usePerformanceCounter = ::QueryPerformanceFrequency(&frequency) != 0;
		initialisedET = true;
	}
	if (usePerformanceCounter) {
		LARGE_INTEGER timeVal;
		::QueryPerformanceCounter(&timeVal);
		bigBit = timeVal.HighPart;
		littleBit = timeVal.LowPart;
	} else {
		bigBit = clock();
	}
}

double ElapsedTime::Duration(bool reset) {
	double result;
	long endBigBit;
	long endLittleBit;

	if (usePerformanceCounter) {
		LARGE_INTEGER lEnd;
		::QueryPerformanceCounter(&lEnd);
		endBigBit = lEnd.HighPart;
		endLittleBit = lEnd.LowPart;
		LARGE_INTEGER lBegin;
		lBegin.HighPart = bigBit;
		lBegin.LowPart = littleBit;
		double elapsed = static_cast<double>(lEnd.QuadPart - lBegin.QuadPart);
		result = elapsed / static_cast<double>(frequency.QuadPart);
	} else {
		endBigBit = clock();
		endLittleBit = 0;
		double elapsed = endBigBit - bigBit;
		result = elapsed / CLOCKS_PER_SEC;
	}
	if (reset) {
		bigBit = endBigBit;
		littleBit = endLittleBit;
	}
	return result;
}

/// Windows specific implementation of TentaclControl.

class TentacleControlW : public TentacleControl {
	UINT_PTR tickerID;
	void Create(HWND wEdit_);
	virtual HWND TopWindow();
	virtual bool HasFocus();
	virtual void Focus();
	virtual Surface *SurfaceForMeasurement();
	virtual int PositionFromLocation(Point pt);
	virtual void SetPosition(SRectangle *rc);
	virtual void GetViewRectangle(SRectangle *rc);
	virtual void CreateControl(HWND wParent);
	void PaintIt();
	virtual void SetScrollPos(int line);
	virtual void SetScrollBars();
	void ScrollMessage(WPARAM wParam);
	void EditScrollWindow(int pixels);
	void HScrollMessage(WPARAM wParam);
	virtual void Copy();
	virtual void Paste();
	virtual void SetPrimary();
	virtual void SetIdle(bool on);
public:
	LRESULT WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
	TentacleControlW() {
		tickerID = 0;
	}
	virtual ~TentacleControlW() {
	}
};

void TentacleControlW::Create(HWND wEdit_) {
	wEdit = wEdit_;
	doubleClickTime = ::GetDoubleClickTime();
	tickerID = ::SetTimer(wEdit, 3, 30, NULL);
}

HWND TentacleControlW::TopWindow() {
	return wEdit;
}

bool TentacleControlW::HasFocus() {
	return ::GetFocus() == wEdit;
}

void TentacleControlW::Focus() {
	::SetFocus(wEdit);
}

Surface *TentacleControlW::SurfaceForMeasurement() {
	return new Surface(::CreateCompatibleDC(NULL), 0);
}

int TentacleControlW::PositionFromLocation(Point pt) {
	Surface surface(::CreateCompatibleDC(NULL), 0);
	return tv.PositionFromSurfaceLocation(&surface, pt);
}

void TentacleControlW::SetPosition(SRectangle *rc) {
	::SetWindowPos(wEdit,
		0, rc->left, rc->top, rc->right - rc->left, rc->bottom - rc->top,
		SWP_NOZORDER|SWP_NOACTIVATE);
}

void TentacleControlW::GetViewRectangle(SRectangle *rc) {
	::GetClientRect(wEdit, reinterpret_cast<RECT *>(rc));
}

void TentacleControlW::SetPrimary() {
}

#define IDLE_MESSAGE WM_USER+1

void TentacleControlW::SetIdle(bool on) {
	if (idlerState != on) {
		if (on) {
			PostMessage(wEdit, IDLE_MESSAGE, 0, 1);
			idlesInFlight++;
		}
		idlerState = on;
	}
}

void TentacleControlW::PaintIt() {
	PAINTSTRUCT ps;
	::BeginPaint(wEdit, &ps);
	Surface surface(::CreateCompatibleDC(ps.hdc), 0);
	surface.SetBitmap(::CreateCompatibleBitmap(ps.hdc, tv.lineWidth, tv.pageHeight));
	SRectangle rcPaint(ps.rcPaint.left, ps.rcPaint.top, ps.rcPaint.right, ps.rcPaint.bottom);
	PaintRect(surface, rcPaint);
	::BitBlt(ps.hdc, 0, 0, tv.lineWidth, tv.pageHeight, reinterpret_cast<HDC>(surface.hdc), 0, 0, SRCCOPY);
	::EndPaint(wEdit, &ps);
}

void TentacleControlW::SetScrollPos(int line) {
	SCROLLINFO si;
	si.cbSize = sizeof(si);
	si.fMask = SIF_ALL;
	if (::GetScrollInfo(wEdit, SB_VERT, &si)) {
		if (si.nPos != line) {
			si.fMask = SIF_POS;
			si.nPos = line;
			::SetScrollInfo(wEdit, SB_VERT, &si, TRUE);
		}
	}
}

static int SetScrollBarProperties(HWND w, int fnBar, int nMax, int page) {
	if (nMax <= 0)
		nMax = 1;
	if (page > nMax+1)
		page = nMax + 1;
	SCROLLINFO si;
	si.cbSize = sizeof(si);
	si.fMask = SIF_ALL;
	int pos = 0;
	if (::GetScrollInfo(w, fnBar, &si)) {
		pos = si.nPos;
		if ((si.nMax != nMax) ||
			(static_cast<int>(si.nPage) != page)) {
			si.fMask = SIF_RANGE | SIF_PAGE;
			si.nPage = page;
			si.nMax = nMax;
			::SetScrollInfo(w, fnBar, &si, TRUE);
		}
	}
	return pos;
}

void TentacleControlW::SetScrollBars() {
	if (wEdit) {
		int nMax = tv.fs->DisplayFromDoc(pdoc->pbuffer->Lines())-1;
		SetScrollBarProperties(wEdit, SB_VERT, nMax, tv.PageSize());

		int maxWidth = tv.fs->GetMaxWidth();
		int maxHoriz = maxWidth; //horizontalRange;
		if (wrap) {
			tv.leftColumn = 0;
			maxHoriz = 0;
		}
		int pageHoriz = tv.lineWidth - tv.textLeft; //horizontalPage;
		int posHoriz = SetScrollBarProperties(wEdit, SB_HORZ, maxHoriz, pageHoriz);
		if (tv.leftColumn != posHoriz) {
			Invalidate();
			tv.leftColumn = posHoriz;
		}
	}
}

void TentacleControlW::ScrollMessage(WPARAM wParam) {
	SCROLLINFO si;
	si.cbSize = sizeof(si);
	si.fMask = SIF_ALL;
	if (::GetScrollInfo(wEdit, SB_VERT, &si)) {
		int topLineNew = si.nPos;
		switch (LOWORD(wParam)) {
		case SB_LINEUP: topLineNew -= 1;	break;
		case SB_LINEDOWN: topLineNew += 1; break;
		case SB_PAGEUP: topLineNew -= tv.PageSize(); break;
		case SB_PAGEDOWN: topLineNew += tv.PageSize(); break;
		case SB_TOP: topLineNew = 0; break;
		case SB_BOTTOM: topLineNew = tv.MaxScrollPos(); break;
		case SB_THUMBPOSITION: topLineNew = si.nTrackPos; break;
		case SB_THUMBTRACK: topLineNew = si.nTrackPos; break;
		}

		if (topLineNew > tv.MaxScrollPos())
			topLineNew = tv.MaxScrollPos();
		if (topLineNew < 0)
			topLineNew = 0;
		tv.topLine = topLineNew;
		si.fMask = SIF_POS;
		int change = si.nPos - topLineNew;
		si.nPos = topLineNew;
		::SetScrollInfo(wEdit, SB_VERT, &si, TRUE);
		if (abs(change) < 30) {
			EditScrollWindow(tv.lineHeight * change);
		} else {
			Invalidate();
		}
	}
}

void TentacleControlW::EditScrollWindow(int pixels) {
	::ScrollWindow(wEdit, 0, pixels, 0, 0);
	::UpdateWindow(wEdit);
}

void TentacleControlW::HScrollMessage(WPARAM wParam) {
	SCROLLINFO si;
	si.cbSize = sizeof(si);
	si.fMask = SIF_ALL;
	if (::GetScrollInfo(wEdit, SB_HORZ, &si)) {
		int leftColumnNew = si.nPos;
		switch (LOWORD(wParam)) {
		case SB_LINEUP: leftColumnNew -= 12;	break;
		case SB_LINEDOWN: leftColumnNew += 12; break;
		case SB_PAGEUP: leftColumnNew -= horizontalPage; break;
		case SB_PAGEDOWN: leftColumnNew += horizontalPage; break;
		case SB_TOP: leftColumnNew = 0; break;
		case SB_BOTTOM: leftColumnNew = horizontalRange; break;
		case SB_THUMBPOSITION: leftColumnNew = si.nTrackPos; break;
		case SB_THUMBTRACK: leftColumnNew = si.nTrackPos; break;
		}
		if (	leftColumnNew < 0)
			leftColumnNew = 0;
		si.fMask = SIF_POS;
		si.nPos = leftColumnNew;
		::SetScrollInfo(wEdit, SB_HORZ, &si, TRUE);
		Invalidate();
		tv.leftColumn = leftColumnNew;
	}
}

void TentacleControlW::Copy() {
	int start = sr.StartSelection();
	int end = sr.EndSelection();
	int lenMax = pdoc->pbuffer->Characters(start, end - start);
	SW_SHORT *uBuffer = new SW_SHORT[lenMax * 2];
	int lenUTF16 = pdoc->pbuffer->RetrieveUTF16(start, uBuffer, end - start);
	::OpenClipboard(wEdit);
	::EmptyClipboard();

	HGLOBAL hand = ::GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, (lenUTF16 + 1) * 2);
	if (hand) {
		SW_SHORT *ptr = static_cast<SW_SHORT  *>(::GlobalLock(hand));
		memcpy(ptr, uBuffer, lenUTF16 * 2);
		ptr[lenUTF16] = '\0';
		::GlobalUnlock(hand);
	}
	::SetClipboardData(CF_UNICODETEXT, hand);
	::CloseClipboard();
	delete []uBuffer;
}

void TentacleControlW::Paste() {
	Clear();
	::OpenClipboard(wEdit);
	HGLOBAL hmemSelection = ::GetClipboardData(CF_UNICODETEXT);
	if (hmemSelection) {
		SW_SHORT *ptr = static_cast<SW_SHORT *>(
			::GlobalLock(hmemSelection));
		if (ptr) {
			int bytes = ::GlobalSize(hmemSelection);
			int len = (int)(bytes / 2);
			for (int i = 0; i < bytes / 2; i++) {
				if ((len == bytes/2) && (0 == ptr[i]))
					len = i;
			}
			int *clipUTF32 = new int[len];
			int lenUTF32 = UniConversion::UTF32FromUTF16(clipUTF32, len, ptr, len);
			InsertUTF32(sr.Caret(), clipUTF32, lenUTF32);
			delete []clipUTF32;
			::GlobalUnlock(hmemSelection);
		}
	}
	::CloseClipboard();
}

class WrapperCaller {
	Wrapper *wrapper;
public:
	WrapperCaller(Wrapper *wrapper_) : wrapper(wrapper_) {
		if (wrapper)
			wrapper->Start();
	}
	~WrapperCaller() {
		if (wrapper)
			wrapper->Finish();
	}
};

LRESULT TentacleControlW::WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	WrapperCaller w(wrapper);
	switch (iMessage) {
	case WM_CREATE:
		Create(hWnd);
		return 0;

	case WM_DESTROY:
		::KillTimer(wEdit, tickerID);
		return 0;

	case WM_KILLFOCUS:
		SetSelectionStyles();
		Invalidate();
		frame->StateAltered();
		return 0;

	case WM_SETFOCUS:
		SetSelectionStyles();
		Invalidate();
		frame->StateAltered();
		return 0;

	case WM_SIZE:
		if (wParam != 1) {
			SizeChanged();
		}
		return 0;

	case WM_KEYDOWN:
		KeyPressed(static_cast<int>(wParam),
			(::GetKeyState(VK_CONTROL) & 0x80000000) != 0,
			(::GetKeyState(VK_SHIFT) & 0x80000000) != 0,
			::GetMessageTime());
		return 0;

	case WM_CHAR:
		CharPressed(static_cast<int>(wParam));
		return 0;

	case WM_LBUTTONDOWN:
		MouseDown(Point(LOWORD(lParam), HIWORD(lParam)),
			::GetMessageTime());
		return 0;

	//~ case WM_MBUTTONDOWN:
		//~ __debugbreak();
		//~ return 0;

	case WM_LBUTTONUP:
		MouseUp(Point(LOWORD(lParam), HIWORD(lParam)));
		return 0;

	case WM_MOUSEMOVE:
		if ((hWnd == ::GetCapture()) && (wParam & MK_LBUTTON)) {
			MouseDrag(Point(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)));
		} else {
			showFeedBack = (::GetKeyState(VK_LCONTROL) & 0x80000000) != 0;
			//MouseFeedBack(Point(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)));
		}
		return 0;

	case WM_MOUSEWHEEL:
		if (!(wParam & (MK_CONTROL | MK_SHIFT))) {
#ifdef GET_WHEEL_DELTA_WPARAM
			wheelDelta -= GET_WHEEL_DELTA_WPARAM(wParam);
#else
			wheelDelta -= 5;
#endif
			if (abs(wheelDelta) >= WHEEL_DELTA) {
				int linesToScroll = 3 * (wheelDelta / WHEEL_DELTA);
				if (wheelDelta >= 0)
					wheelDelta = wheelDelta % WHEEL_DELTA;
				else
					wheelDelta = - (-wheelDelta % WHEEL_DELTA);
				// Scroll
				ScrollTo(tv.topLine + linesToScroll, true);
			}
		}
		return 0;

	case WM_TIMER:
		Tick(::GetMessageTime());
		if (PerformIdle(true)) {
			if (idlesInFlight == 0) {
				idleCount = 10;
				PostMessage(hWnd, IDLE_MESSAGE, 0, 1);
				idlesInFlight++;
			}
		} else {
			SetIdle(false);
		}
		return 0;

	case WM_PAINT:
		PaintIt();
		return 0;

	case WM_VSCROLL:
		ScrollMessage(wParam);
		return 0;

	case WM_HSCROLL:
		HScrollMessage(wParam);
		return 0;

	case IDLE_MESSAGE:
		idlesInFlight--;
		idleCount--;
		if (PerformIdle(true)) {
			if (idleCount > 0) {
				PostMessage(hWnd, IDLE_MESSAGE, 0, 1);
				idlesInFlight++;
			}
		} else {
			SetIdle(false);
		}
		return 0;

	default:
		return ::DefWindowProc(hWnd, iMessage, wParam, lParam);
	}
}

LRESULT CALLBACK ControlWndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	// Find C++ object associated with window.
	TentacleControlW *tc = reinterpret_cast<TentacleControlW *>(PointerFromWindow(hWnd, 0));
	// sci will be zero if WM_CREATE not seen yet
	if (tc == 0) {
		if (iMessage == WM_CREATE) {
			// Associate CallTip object with window
			CREATESTRUCT *pCreate = reinterpret_cast<CREATESTRUCT *>(lParam);
			SetWindowPointer(hWnd, 0, pCreate->lpCreateParams);
			TentacleControlW *tc = reinterpret_cast<TentacleControlW *>(pCreate->lpCreateParams);
			return tc->WndProc(hWnd, iMessage, wParam, lParam);
		} else {
			return ::DefWindowProc(hWnd, iMessage, wParam, lParam);
		}
	} else {
		if (iMessage == WM_NCDESTROY) {
			//tc->Finalise();
			::SetWindowLong(hWnd, 0, 0);
			return ::DefWindowProc(hWnd, iMessage, wParam, lParam);
		} else {
			return tc->WndProc(hWnd, iMessage, wParam, lParam);
		}
	}
}

static HINSTANCE hinst=0;

void ControlRegisterWindowClass(HINSTANCE hInstance) {
	if (hinst)
		return;
	hinst=hInstance;
	WNDCLASS controlWndclass;
	controlWndclass.style = CS_HREDRAW | CS_VREDRAW;
	controlWndclass.lpfnWndProc = ControlWndProc;
	controlWndclass.cbClsExtra = 0;
	controlWndclass.cbWndExtra = sizeof(TentacleControl *);
	controlWndclass.hInstance = hInstance;
	controlWndclass.hIcon = 0;
	controlWndclass.hCursor = ::LoadCursor(NULL, MAKEINTRESOURCE(IDC_IBEAM));
	controlWndclass.hbrBackground = NULL;
	controlWndclass.lpszMenuName = NULL;
	controlWndclass.lpszClassName = controlClassName;

	if (!::RegisterClass(&controlWndclass))
		::exit(FALSE);
}

void TentacleControlW::CreateControl(HWND wParent) {
	::CreateWindowEx(
	              0,
	              controlClassName,
	              "Source",
	              WS_VISIBLE | WS_CHILD | WS_VSCROLL | WS_HSCROLL |
				  WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
	              0, 0,
	              150, 200,
	              wParent,
	              reinterpret_cast<HMENU>(1),
	              hinst,
	              this);
}

TentacleControl *TentacleControl::Allocate() {
	//~ __debugbreak();
	ControlRegisterWindowClass(GetModuleHandle(NULL));
	return new TentacleControlW();
}

#endif
