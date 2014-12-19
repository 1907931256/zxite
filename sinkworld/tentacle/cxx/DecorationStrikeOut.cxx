// SinkWorld DecorationStrikeOut class.
// Copyright 2004 Neil Hodgson
// File written in C~ subset of C++ for automatic translation to Java and C#.

#include <assert.h>
#include <string.h>
#include <string>

#include "base.h"

#include "RGBColor.h"
#include "IDecoration.h"
#include "BaseDecoration.h"
#include "DecorationStrikeOut.h"

#include "FontDescription.h"
#include "Surface.h"

void DecorationStrikeOut::Paint(PaintContext *pc) {
	SRectangle rc = pc->rc;
	rc.top = (pc->rc.bottom+pc->rc.top)/2;
	rc.bottom = rc.top + 2;
	pc->surface->FillRectangle(rc, fore);
}
