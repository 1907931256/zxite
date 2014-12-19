// SinkWorld DecorationGraduatedBox class.
// Copyright 2004 Neil Hodgson
// File written in C~ subset of C++ for automatic translation to Java and C#.

#include <assert.h>
#include <string.h>
#include <string>

#include "base.h"

#include "RGBColor.h"
#include "IDecoration.h"
#include "BaseDecoration.h"
#include "DecorationGraduatedBox.h"

#include "FontDescription.h"
#include "Surface.h"

void DecorationGraduatedBox::Paint(PaintContext *pc) {
	SRectangle rc = pc->rc;
	rc.left--;
	rc.right++;
	pc->surface->FrameRectangle(rc, fore);
	rc.left--;
	rc.top--;
	rc.right++;
	rc.bottom++;
	RGBColor in1 = fore.Merge(RGBColor(0xff,0xff,0xff), 127);
	pc->surface->FrameRectangle(rc, in1);
	rc.left--;
	rc.top--;
	rc.right++;
	rc.bottom++;
	RGBColor in2 = fore.Merge(RGBColor(0xff,0xff,0xff), 195);
	pc->surface->FrameRectangle(rc, in2);
}
