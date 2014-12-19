// SinkWorld DecorationButton class.
// Copyright 2004 Neil Hodgson
// File written in C~ subset of C++ for automatic translation to Java and C#.

#include <assert.h>
#include <string.h>
#include <string>

#include "base.h"

#include "RGBColor.h"
#include "IDecoration.h"
#include "BaseDecoration.h"
#include "DecorationButton.h"

#include "FontDescription.h"
#include "Surface.h"

void DecorationButton::Paint(PaintContext *pc) {
	SRectangle rc = pc->rc;
	rc.left--;
	rc.top--;
	rc.right++;
	rc.bottom++;
	pc->surface->FillRectangle(rc, RGBColor(0xD4, 0xD2,0xD2));
	if (pressed) {
		pc->surface->SetPen(RGBColor(0x80,0x80,0x80));
		pc->surface->MoveTo(rc.left, rc.bottom-1);
		pc->surface->LineTo(rc.left, rc.top);
		pc->surface->LineTo(rc.right, rc.top);
		pc->surface->SetPen(RGBColor(0xf0,0xf0,0xf0));
		pc->surface->MoveTo(rc.left, rc.bottom-1);
		pc->surface->LineTo(rc.right, rc.bottom-1);
		pc->surface->LineTo(rc.right, rc.top);
	} else {
		pc->surface->SetPen(RGBColor(0xf0,0xf0,0xf0));
		pc->surface->MoveTo(rc.left, rc.bottom-1);
		pc->surface->LineTo(rc.left, rc.top);
		pc->surface->LineTo(rc.right, rc.top);
		pc->surface->SetPen(RGBColor(0x80,0x80,0x80));
		pc->surface->MoveTo(rc.left, rc.bottom-1);
		pc->surface->LineTo(rc.right, rc.bottom-1);
		pc->surface->LineTo(rc.right, rc.top);
	}
}

