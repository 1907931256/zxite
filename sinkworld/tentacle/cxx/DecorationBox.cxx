// SinkWorld DecorationBox class.
// Copyright 2004 Neil Hodgson
// File written in C~ subset of C++ for automatic translation to Java and C#.

#include <assert.h>
#include <string.h>
#include <string>

#include "base.h"

#include "RGBColor.h"
#include "IDecoration.h"
#include "BaseDecoration.h"
#include "DecorationBox.h"

#include "FontDescription.h"
#include "Surface.h"

void DecorationBox::Paint(PaintContext *pc) {
	pc->surface->FrameRectangle(pc->rc, fore);
}
