// SinkWorld DecorationUnderLine class.
// Copyright 2004 Neil Hodgson
// File written in C~ subset of C++ for automatic translation to Java and C#.

#include <assert.h>
#include <string.h>
#include <string>

#include "base.h"

#include "RGBColor.h"
#include "IDecoration.h"
#include "BaseDecoration.h"
#include "DecorationUnderLine.h"

#include "FontDescription.h"
#include "Surface.h"

void DecorationUnderLine::Paint(PaintContext *pc) {
	SRectangle rc = pc->rc;
	pc->surface->SetPen(fore);
	pc->surface->MoveTo(rc.left, rc.bottom-1);
	pc->surface->LineTo(rc.right, rc.bottom-1);
}
