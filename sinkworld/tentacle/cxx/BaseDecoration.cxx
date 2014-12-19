// SinkWorld BaseDecoration class.
// Copyright 2004 Neil Hodgson
// File written in C~ subset of C++ for automatic translation to Java and C#.

#include <assert.h>
#include <string.h>
#include <string>

#include "base.h"

#include "RGBColor.h"
#include "IDecoration.h"
#include "BaseDecoration.h"
#include "Style.h"
#include "StyleModification.h"

void BaseDecoration::FromOptions(Literal *mode, Literal *facetPath, OptionSet *options)	{
	Literal litFore = Literal("fore");
	Literal styleFore = Literal(facetPath, (SW_BYTE)'.', &litFore);
	Literal *valFore = options->ModeValue(mode, &styleFore);
	if (valFore != NULL) {
		RGBColor foreColor = RGBColor(StyleModification::ColourFromLiteral(valFore));
		SetFore(foreColor);
	}
}
