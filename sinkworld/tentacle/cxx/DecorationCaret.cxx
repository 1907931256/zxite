// SinkWorld DecorationCaret class.
// Copyright 2004 Neil Hodgson
// File written in C~ subset of C++ for automatic translation to Java and C#.

#include <assert.h>
#include <string.h>
#include <string>

#include "base.h"

#include "RGBColor.h"
#include "IDecoration.h"
#include "BaseDecoration.h"
#include "DecorationCaret.h"

#include "FontDescription.h"
#include "Surface.h"

void DecorationCaret::Paint(PaintContext *pc) {
	SRectangle rc = pc->rc;
	rc.left = rc.left - width / 2;
	rc.right = rc.left + width;
	pc->surface->FillRectangle(rc, fore);
}
