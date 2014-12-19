// SinkWorld StyleModification class.
// Copyright 2005 Neil Hodgson
// File written in C~ subset of C++ for automatic translation to Java and C#.

#include <assert.h>
#include <string.h>
#include <string>

#include "base.h"

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

long StyleModification::DigitValue(int ch) {
	if ((ch >= '0') && (ch <= '9')) {
		return ch - '0';
	} else if ((ch >= 'a') && (ch <= 'f')) {
		return ch - 'a' + 10;
	} else if ((ch >= 'A') && (ch <= 'F')) {
		return ch - 'A' + 10;
	} else {
		return 0;
	}
}

long StyleModification::ColourFromLiteral(Literal *value) {
	if (value->Length() < 7) {
		return 0;
	} else {
		return 
			(DigitValue(value->CharAt(1)) << 4) + 
			DigitValue(value->CharAt(2)) + 
			(DigitValue(value->CharAt(3)) << 12) + 
			(DigitValue(value->CharAt(4)) << 8) + 
			(DigitValue(value->CharAt(5)) << 20) + 
			(DigitValue(value->CharAt(6)) << 16);
	}
}

void StyleModification::AddFacet(Literal *name, Literal *value, LiteralSet *fontNames) {
	Literal nFont = Literal("font");
	Literal nSize = Literal("size");
	Literal nBold = Literal("bold");
	Literal nItalics = Literal("italics");
	Literal nFore = Literal("fore");
	Literal nBack = Literal("back");
	Literal nEOLFilled = Literal("eolfilled");
	Literal nBox = Literal("box");
	Literal nDecoration = Literal("decoration");
	Literal nBackDecoration = Literal("backdecoration");
	Literal litDrop = Literal("~");		// Indicates the facet should not have a setting in this object

	int specifiedFacet = 0;
	if ((name->Equals(&nFont)) && (fontNames != NULL)) {
		// TODO: attach fontNames so will always be valid
		AddFont(fontNames->FindOrAddKey(value));
		specifiedFacet = sdFont;
	} else if (name->Equals(&nSize)) {
		AddSize(value->Value());
		specifiedFacet = sdSize;
	} else if (name->Equals(&nBold)) {
		AddBold(value->Value() != 0);
		specifiedFacet = sdBold;
	} else if (name->Equals(&nItalics)) {
		AddItalics(value->Value() != 0);
		specifiedFacet = sdItalics;
	} else if (name->Equals(&nFore)) {
		RGBColor fore = RGBColor(ColourFromLiteral(value));
		AddFore(fore);
		specifiedFacet = sdFore;
	} else if (name->Equals(&nBack)) {
		RGBColor back = RGBColor(ColourFromLiteral(value));
		AddBack(back);
		specifiedFacet = sdBack;
	} else if (name->Equals(&nEOLFilled)) {
		AddEOLFilled(value->Value() != 0);
		specifiedFacet = sdEOLFilled;
	} else if (name->Equals(&nBox)) {
		RGBColor box = RGBColor(ColourFromLiteral(value));
		AddBox(box);
		specifiedFacet = sdBox;
	} else if (name->Equals(&nDecoration)) {
		Literal decorationName = Literal(value, 8, value->Length() - 8);
		Literal litDecoration = Literal("Decoration");
		IDecoration *decoration = (IDecoration *)PrototypeRegistry::FromName(&litDecoration, &decorationName);
		if (decoration != NULL) {
			RGBColor fore = RGBColor(ColourFromLiteral(value));
			decoration->SetFore(fore);
			AddDecorationFore(decoration);
		}
		specifiedFacet = sdDecorationFore;
	} else if (name->Equals(&nBackDecoration)) {
		Literal decorationName = Literal(value, 8, value->Length() - 8);
		Literal litDecoration = Literal("Decoration");
		IDecoration *decoration = (IDecoration *)PrototypeRegistry::FromName(&litDecoration, &decorationName);
		if (decoration != NULL) {
			RGBColor fore = RGBColor(ColourFromLiteral(value));
			decoration->SetFore(fore);
			AddDecorationBack(decoration);
		}
		specifiedFacet = sdDecorationBack;
	}
	if (value->Equals(&litDrop)) {
		specified &= ~specifiedFacet;
	}
}

StyleModification *StyleModification::FromOptions(Literal *mode, Literal *styleSeg, OptionSet *options, LiteralSet *fontNames) {
	StyleModification *sty = new StyleModification();
	Literal facets = Literal("font size bold italics fore back eolfilled box");
	for (int fac=0;fac<=facets.Count((SW_BYTE)' ');fac++) {
		Literal *styleFacet = facets.Segment(fac, ' ');
		Literal facetPath = Literal(styleSeg, (SW_BYTE)'.', styleFacet);
		Literal *styleValue = options->ModeValue(mode, &facetPath);
		if (styleValue != NULL) {
			sty->AddFacet(styleFacet, styleValue, fontNames);
		}
		delete styleFacet;
	}
	Literal compoundFacets = Literal("decoration backdecoration");
	Literal nDecoration = Literal("decoration");
	Literal nBackDecoration = Literal("backdecoration");
	Literal litDecoration = Literal("Decoration");
	for (int fac=0;fac<=compoundFacets.Count((SW_BYTE)' ');fac++) {
		Literal *styleFacet = compoundFacets.Segment(fac, ' ');
		Literal facetPath = Literal(styleSeg, (SW_BYTE)'.', styleFacet);
		Literal indicator = Literal("type");
		Literal styleIndicator = Literal(&facetPath, (SW_BYTE)'.', &indicator);
		Literal *styleType = options->ModeValue(mode, &styleIndicator);
		if (styleType != NULL) {
			IDecoration *decoration = (IDecoration *)PrototypeRegistry::FromName(&litDecoration, styleType);
			if (decoration != NULL) {
				decoration->FromOptions(mode, &facetPath, options);
				if (fac == 0) {
					sty->AddDecorationFore(decoration);
				} else {
					sty->AddDecorationBack(decoration);
				}
			}
		}
		delete styleFacet;
	}
	return sty;
}
