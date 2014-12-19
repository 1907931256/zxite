// SinkWorld DecorationRoundedBox class.
// Copyright 2005 Neil Hodgson
// File written in C~ subset of C++ for automatic translation to Java and C#.

#include <assert.h>
#include <string.h>
#include <string>

#include "base.h"

#include "RGBColor.h"
#include "IDecoration.h"
#include "BaseDecoration.h"
#include "DecorationRoundedBox.h"

#include "FontDescription.h"
#include "Surface.h"

void DecorationRoundedBox::Paint(PaintContext *pc) {
	if ((pc->rc.right - pc->rc.left) > 4) {
		// The curved corners need 4 pixels to look correct
		Point pts[]  = {
			Point(pc->rc.left+2, pc->rc.top+1),
			Point(pc->rc.right-2, pc->rc.top+1),
			Point(pc->rc.right, pc->rc.top+3),
			Point(pc->rc.right, pc->rc.bottom-3),
			Point(pc->rc.right-2, pc->rc.bottom-1),
			Point(pc->rc.left+2, pc->rc.bottom-1),
			Point(pc->rc.left, pc->rc.bottom-3),
			Point(pc->rc.left, pc->rc.top+3),
		};
		pc->surface->Polygon(pts, 8, fore, alpha, alphaOutline);
	} else {
		Point pts[]  = {
			Point(pc->rc.left, pc->rc.top),
			Point(pc->rc.right, pc->rc.top),
			Point(pc->rc.right, pc->rc.bottom),
			Point(pc->rc.left, pc->rc.bottom),
		};
		pc->surface->Polygon(pts, 4, fore, alpha, alphaOutline);
	}
}

void DecorationRoundedBox::FromOptions(Literal *mode, Literal *facetPath, OptionSet *options) {
	super::FromOptions(mode, facetPath, options);
	Literal litAlpha = Literal("alpha");
	Literal litAlphaOutline = Literal("alphaoutline");

	Literal styleAlpha = Literal(facetPath, (SW_BYTE)'.', &litAlpha);
	Literal *valAlpha = options->ModeValue(mode, &styleAlpha);
	if (valAlpha != NULL) {
		alpha = valAlpha->Value();
		alphaOutline = alpha;
	}

	Literal styleAlphaOutline = Literal(facetPath, (SW_BYTE)'.', &litAlphaOutline);
	Literal *valAlphaOutline = options->ModeValue(mode, &styleAlphaOutline);
	if (valAlphaOutline != NULL) {
		alphaOutline = valAlphaOutline->Value();
	}
}
