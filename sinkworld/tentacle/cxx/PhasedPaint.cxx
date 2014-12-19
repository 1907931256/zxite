// SinkWorld Phased Paint classes.
// Copyright 2004 Neil Hodgson
// File written in C~ subset of C++ for automatic translation to Java and C#.

#include <string.h>
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

int PhaseBack::FlagsUse() {
	return Style::sdBack | Style::sdEOLFilled;
}

void PhaseBack::Paint(PaintParameters *pp) {
	// Background phase
	int flags = FlagsUse();
	pp->surface->SetTransparent(false);
	int lineStart = pp->pbuffer->PositionFromLine(pp->lineDoc);
	int lineEnd = pp->pbuffer->PositionFromLine(pp->lineDoc+1);
	int xStart = pp->leftColumn - pp->left + pp->positions->At(pp->subLineStart-lineStart);
	int positionChange = lineStart;
	long lastColour = 0;
	if (lineStart < lineEnd) {
		int startSegment = lineStart;
		Style styleStart = Style(pp->ssc->fontNameDefault);
		pp->ssc->Modify(lineStart, flags, styleStart);
		for (int ip=0;ip<pp->cf->Length();ip++) {
			positionChange = pp->cf->At(ip);
			Style styleNow = Style(pp->ssc->fontNameDefault);
			pp->ssc->Modify(positionChange, flags, styleNow);
			if ((positionChange >= lineEnd) || (styleNow.DifferentTo(styleStart, flags))) {
				if (positionChange > pp->subLineEnd)	// Ensure characters only drawn once
					positionChange = pp->subLineEnd;
				pp->rcw.left = pp->positions->At(startSegment-lineStart) - xStart;
				pp->rcw.right = pp->positions->At(positionChange-lineStart) - xStart;
				pp->surface->FillRectangle(pp->rcw, styleStart.back);
				lastColour = styleStart.back.AsBGR();
				styleStart = styleNow;
				startSegment = positionChange;
			}
		}
	}
	// Paint to end of line
	pp->rcw.left = pp->positions->At(positionChange - lineStart) - xStart;
	pp->rcw.right = 10000;
	int lineLast = pp->pbuffer->LineEndPositionFromLine(pp->lineDoc) - 1;
	if (lineLast < lineStart) {
		lineLast = lineStart;
	}
	Style styleDefault = Style(pp->ssc->fontNameDefault);
	pp->ssc->ModifyToFirst(flags, styleDefault);
	Style styleEnd = Style(pp->ssc->fontNameDefault);
	pp->ssc->Modify(lineLast, flags, styleEnd);
	if (styleEnd.eolFilled) {
		pp->surface->FillRectangle(pp->rcw, styleEnd.back);
	} else {
		pp->surface->FillRectangle(pp->rcw, styleDefault.back);
	}
	if (!pp->fs->GetVisible(pp->lineDoc+1)) {
		pp->rcw.left = 0;
		pp->rcw.top = pp->rcw.bottom - 1;
		pp->rcw.right = 10000;
		pp->surface->FillRectangle(pp->rcw, styleDefault.back);
	}
}

void PhaseText::DrawTabArrow(Surface *surface, SRectangle rcw, RGBColor fore) {
	surface->SetPen(fore);
	int mid = (rcw.bottom+rcw.top)/2+1;
	// Drawn in this specific way to avoid cross-platform disagreement
	// on whether lines include their last point.
	surface->MoveTo(rcw.right-3, mid);
	surface->LineTo(rcw.left+2, mid);
	surface->MoveTo(rcw.right-7, mid-4);
	surface->LineTo(rcw.right-3, mid);
	surface->MoveTo(rcw.right-7, mid+4);
	surface->LineTo(rcw.right-3, mid);
}

int PhaseText::FlagsUse() {
	return Style::sdFore | Style::sdItalics | Style::sdBold | Style::sdSize | Style::sdFont;
}

void PhaseText::Paint(PaintParameters *pp) {
	// Mask = fore|italics|bold|size|font|modechange
	// Literal phase
	int flags = FlagsUse();
	pp->surface->SetTransparent(true);
	int lineStart = pp->pbuffer->PositionFromLine(pp->lineDoc);
	int lineEnd = pp->pbuffer->PositionFromLine(pp->lineDoc+1);
	if (lineStart == lineEnd) {
		return;
	}

	int startSegment = lineStart;
	Style styleStart = Style(pp->ssc->fontNameDefault);
	pp->ssc->Modify(lineStart, flags, styleStart);
	ByteBuffer s = ByteBuffer();
	int xStart = pp->leftColumn - pp->left + pp->positions->At(pp->subLineStart-lineStart);
	for (int ip=0;(ip<pp->cf->Length()) && (startSegment < pp->subLineEnd);ip++) {
		int positionChange = pp->cf->At(ip);
		Style styleNow = Style(pp->ssc->fontNameDefault);
		pp->ssc->Modify(positionChange, flags, styleNow);
		if ((positionChange >= lineEnd) ||
			(styleNow.DifferentTo(styleStart, flags)) ||
			SubstanceLayer::IsControlCharacter(pp->pbuffer->CharValue(startSegment)) ||
			SubstanceLayer::IsControlCharacter(pp->pbuffer->CharValue(positionChange))) {
			pp->rcw.left = pp->positions->At(startSegment-lineStart) - xStart;
			pp->rcw.right = pp->positions->At(positionChange-lineStart) - xStart;
			if (positionChange > pp->subLineEnd)	// Ensure characters only drawn once
				positionChange = pp->subLineEnd;
			if (pp->pbuffer->CharValue(startSegment) == '\t') {
				if (pp->visibleWhitespace) {
					DrawTabArrow(pp->surface, pp->rcw, styleStart.fore);
				}
			} else if (pp->pbuffer->CharValue(startSegment) == '\r') {
				//pp->surface->FillRectangle(pp->rcw, styleStart.back);
			} else if (pp->pbuffer->CharValue(startSegment) == '\n') {
				//pp->surface->FillRectangle(pp->rcw, styleStart.back);
			} else {
				int len = positionChange - startSegment;
				//s.Resize(len+1);
				//pp->pbuffer->RetrieveString(startSegment, s.Bytes(), len);
				//s.Bytes()[len] = 0;
				pp->surface->SetTextColor(styleStart.fore);
				FontDescription fd = FontDescription(styleStart.font, 
					styleStart.size+pp->magnification, styleStart.bold, styleStart.italics);
				pp->surface->SetFont(fd);
				//pp->surface->PaintText(s.Bytes(), len, &pp->rcw, pp->ascent, pp->pbuffer->GetEncoding());
				pp->surface->PaintText(pp->pbuffer, startSegment, len, &pp->rcw, pp->ascent);
			}
			styleStart = styleNow;
			startSegment = positionChange;
		}
	}
	//char level[100];
	//sprintf(level, "%4d", pstyleBraceCount->ValueAt(lineStart));
	//rcw.left = right-40;
	//rcw.right = right;
	//surface.PaintText(reinterpret_cast<unsigned char *>(level), strlen(level), 
	//	&rcw, ascent, pbuffer->GetEncoding());
}

PhaseBox::PhaseBox() {
	layerMax = 0;
	boxLayers = NULL;
}

PhaseBox::~PhaseBox() {
	for (int i=0;i<layerMax;i++) {
		delete boxLayers[i]->rsPrevLine;
		boxLayers[i]->rsPrevLine = NULL;
		delete boxLayers[i];
		boxLayers[i] = NULL;
	}
	delete []boxLayers;
	boxLayers = NULL;
}

int PhaseBox::FlagsUse() {
	return Style::sdBox;
}

OneCorner::OneCorner(Surface *surface_, int x_, int y_, int xPrevious_, int xNext_) {
	surface = surface_;
	x = x_;
	y = y_;
	xPrevious = xPrevious_;
	xNext = xNext_;
}

void OneCorner::Draw(int quadrant) {
	// Quadrant numbered clockwise from top: 1, 2, 3, 4
	int xDir = ((quadrant == 1) || (quadrant == 2)) ? 1 : -1;
	int yDir = ((quadrant == 2) || (quadrant == 3)) ? 1 : -1;
	int xSpace = (xDir == 1) ? (xNext - x) : (x - xPrevious);
	DrawCorner(surface, x, y, xDir, yDir, xSpace, PhaseBox::cornerSize);
}

void OneCorner::DrawCorner(Surface *surface, int x, int y, int xDir, int yDir, int xSpace, int corner) {
	if (xSpace > corner * 2) {
		surface->MoveTo(x,y + yDir * (corner));
		surface->LineTo(x + xDir * (corner+Surface::lineExtra),y-(Surface::lineExtra * (yDir)));
	} else {
		surface->MoveTo(x,y);
		surface->LineTo(x + xDir * (xSpace),y);
		surface->MoveTo(x,y);
		surface->LineTo(x,y + yDir * (corner+Surface::lineExtra));
	}
}

void PhaseBox::PaintCorner(Surface *surface, int x, int y, int val, int valPrevious, int xPrevious, int xNext) {
	int combined = valPrevious * 4 + val;
	OneCorner oc = OneCorner(surface, x, y, xPrevious, xNext);
	switch (combined) {
		// One corner filled
		case 1:	oc.Draw(1); break;
		case 2:	oc.Draw(2); break;
		case 4:	oc.Draw(4); break;
		case 8:	oc.Draw(3); break;

		// Three corners filled
		case 7:	oc.Draw(3); break;
		case 11:	oc.Draw(4); break;
		case 13:	oc.Draw(2); break;
		case 14:	oc.Draw(1); break;

		// Opposite corners
		case 6:
			oc.Draw(1); 
			oc.Draw(3); 
			break;
		case 9:
			oc.Draw(2); 
			oc.Draw(4); 
			break;

		// All on one side so vertical
		case 3:
		case 12:
			// Use 0 xSpace to just draw vertical
			OneCorner::DrawCorner(surface, x, y, 1, -1, 0, cornerSize);
			OneCorner::DrawCorner(surface, x, y, 1, 1, 0, cornerSize);
			break;

		// No change so not possible
		case 0:
		case 5:
		case 10:
		case 15:
			break;
	}
}

void PhaseBox::Paint(PaintParameters *pp) {
	int flags = FlagsUse();
	int xxBig = 100000;
	if (boxLayers == NULL) {
		layerMax = pp->ssc->Length();
		boxLayers = new BoxLayer*[layerMax];
		for (int i=0;i<layerMax;i++) {
			boxLayers[i] = new BoxLayer();
			boxLayers[i]->rsPrevLine = new RunStyles();
			boxLayers[i]->rsPrevLine->InsertSpace(0,xxBig);
		}
	}
	for (int ss=0; ss<pp->ssc->Length(); ss++) {
		// Depends on lines being painted in order
		RunStyles *rsLine = new RunStyles();
		int lineStart = pp->pbuffer->PositionFromLine(pp->lineDoc);
		int lineEnd = pp->pbuffer->PositionFromLine(pp->lineDoc+1);
		int xStart = pp->leftColumn - pp->left + pp->positions->At(pp->subLineStart-lineStart);
		rsLine->InsertSpace(0,xxBig);
		int positionChange = lineStart;
		RGBColor transparent = RGBColor();
		if (lineStart < lineEnd) {
			int startSegment = lineStart;
			Style styleStart = Style(pp->ssc->fontNameDefault);
			pp->ssc->ModifySet(ss, lineStart, flags, styleStart);
			for (int ip=0;ip<pp->cf->Length();ip++) {
				positionChange = pp->cf->At(ip);
				Style styleNow = Style(pp->ssc->fontNameDefault);
				pp->ssc->ModifySet(ss, positionChange, flags, styleNow);
				if ((positionChange >= lineEnd) || (styleNow.DifferentTo(styleStart, flags))) {
					if (positionChange > pp->subLineEnd)	// Ensure characters only drawn once
						positionChange = pp->subLineEnd;
					if (styleStart.box.NotEqual(transparent)) {
						boxLayers[ss]->color = styleStart.box.AsBGR();
						int xx = pp->positions->At(startSegment-lineStart) - xStart;
						int xxMax = pp->positions->At(positionChange-lineStart) - xStart;
						rsLine->FillRange(xx, 1, xxMax - xx);
						boxLayers[ss]->rsPrevLine->IncrementRange(xx, 2, xxMax - xx);
					}
					styleStart = styleNow;
					startSegment = positionChange;
				}
			}
		}
		int prevVal = 0;
		int xPrevious = 0;
		pp->surface->SetPen(RGBColor(boxLayers[ss]->color));
		int xxx = boxLayers[ss]->rsPrevLine->FindNextChange(-1 -xStart, xxBig);
		if (boxLayers[ss]->rsPrevLine->ValueAt(xxx) == 0) {
			xPrevious = xxx;
			xxx = boxLayers[ss]->rsPrevLine->FindNextChange(xxx, xxBig);
		}
		int xxxNext =  boxLayers[ss]->rsPrevLine->FindNextChange(xxx, xxBig);
		while (xxx < xxBig) {
			int startVal = boxLayers[ss]->rsPrevLine->ValueAt(xxx);
			int endVal = boxLayers[ss]->rsPrevLine->ValueAt(xxxNext);

			bool drawBorderLine = (startVal == 1) || (startVal == 2);
			if (drawBorderLine && ((xxxNext - xxx) >= cornerSize * 2)) {
				pp->surface->MoveTo(xxx + cornerSize, pp->rcw.top);
				pp->surface->LineTo(xxxNext+Surface::lineExtra - cornerSize, pp->rcw.top);
			}

			// Draw corners
			PaintCorner(pp->surface, xxx, pp->rcw.top, startVal, prevVal, xPrevious, xxxNext);
			PaintCorner(pp->surface, xxxNext, pp->rcw.top, endVal, startVal, xxx, 
				boxLayers[ss]->rsPrevLine->FindNextChange(xxxNext, xxBig));

			// Draw vertical lines if needed
			if (((prevVal & 2) == 0) && ((startVal & 2) != 0)) {
				pp->surface->MoveTo(xxx, pp->rcw.top + cornerSize);
				pp->surface->LineTo(xxx, pp->rcw.bottom - cornerSize);
			}
			if (((startVal & 2) != 0) && ((endVal & 2) == 0)) {
				pp->surface->MoveTo(xxxNext, pp->rcw.top + cornerSize);
				pp->surface->LineTo(xxxNext, pp->rcw.bottom - cornerSize);
			}

			if (drawBorderLine && (endVal == 0)) {
				xPrevious = xxxNext;
				xxx = boxLayers[ss]->rsPrevLine->FindNextChange(xxxNext, xxBig);
				xxxNext = boxLayers[ss]->rsPrevLine->FindNextChange(xxx, xxBig);
				prevVal = endVal;
			} else {
				xPrevious = xxx;
				xxx = xxxNext;
				xxxNext = boxLayers[ss]->rsPrevLine->FindNextChange(xxx, xxBig);
				prevVal = startVal;
			}
		}

		delete boxLayers[ss]->rsPrevLine;
		boxLayers[ss]->rsPrevLine = rsLine;
	}
}

int PhaseDecoration::FlagsUse() {
	return fore ? (int)Style::sdDecorationFore : (int)Style::sdDecorationBack;
}

void PhaseDecoration::Paint(PaintParameters *pp) {
	int flags = FlagsUse();
	int lineStart = pp->pbuffer->PositionFromLine(pp->lineDoc);
	int lineEnd = pp->pbuffer->PositionFromLine(pp->lineDoc+1);
	int lineLastVisible = pp->subLineEnd - 1;
	if (pp->lineDoc == (pp->pbuffer->Lines()-1))	// Final line of document
		lineLastVisible = pp->subLineEnd;
	int xStart = pp->leftColumn - pp->left + pp->positions->At(pp->subLineStart-lineStart);
	for (int l=0; l<pp->ssc->Length(); l++) {
		StyleSet *ss=pp->ssc->Get(l);
		if ((ss != NULL) && ((ss->SpecifiedFlags() & flags) != 0)) {
			IStyles *layer = pp->ssc->GetLayer(l);
			if (layer != NULL) {
				int start = lineStart;	// Allow return of lineStart to draw caret at start of line
				int style = 0;
				try {
					// This may read one beyond end of document to 
					// ensure caret is drawn at end.
					style = layer->ValueAt(start);
				} catch (BadPositionException *e) {
					delete e;
				}
				while (start <= lineLastVisible) {
					int nextChange = layer->FindNextChange(start, lineEnd);
					// Ensure decorators only drawn once
					StyleModification *sm = ss->Get(style);
					if ((sm != NULL) && ((sm->specified & flags) != 0)) {
						IDecoration *pDecoration = fore ? sm->decorationFore : sm->decorationBack;
						if (pDecoration != NULL) {
							SRectangle rcDecoration = SRectangle(pp->rcw);
							rcDecoration.left = 0 - xStart;
							if (start >= lineStart) {
								rcDecoration.left = pp->positions->At(start-lineStart) - xStart;
							}
							if (nextChange > lineLastVisible) {
								rcDecoration.right = pp->positions->At(lineLastVisible-lineStart) - xStart;
							} else {
								rcDecoration.right = pp->positions->At(nextChange-lineStart) - xStart;
							}
							PaintContext pc = PaintContext(pp->surface, rcDecoration);
							pDecoration->Paint(&pc);
						}
					}
					start = nextChange;
					try {
						// This may read one beyond end of document to 
						// ensure caret is drawn at end.
						style = layer->ValueAt(start);
					} catch (BadPositionException *e) {
						style = 0;
						delete e;
					}
				}
			}
		}
	}
}
