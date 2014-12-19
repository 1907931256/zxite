// SinkWorld Document class.
// Copyright 2005 Neil Hodgson
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

SubMode::SubMode(Literal *mode_, OptionSet *options) {
	mode = new Literal(mode_);
	Literal litLexer = Literal("lexer");
	lexerName = new Literal(options->ModeValue(mode, &litLexer));
	Literal litLexerRegistry = Literal("Lexer");
	lexer = (ILexer *)PrototypeRegistry::FromName(&litLexerRegistry, lexerName);
	if (lexer == NULL) {
		Literal litNull = Literal("Null");
		lexer = (ILexer *)PrototypeRegistry::FromName(&litLexerRegistry, &litNull);
	}
	Literal litFolder = Literal("folder");
	Literal *folderSet = options->ModeValue(mode, &litFolder);
	if (folderSet == NULL) {
		folderName = NULL;
		folder = NULL;
	} else {
		folderName = new Literal(folderSet);
		folder = (ILexer *)PrototypeRegistry::FromName(&litLexerRegistry, folderName);
	}
}

SubMode::~SubMode() {
	delete mode;
	mode = NULL;
	delete lexerName;
	lexerName = NULL;
	delete lexer;
	lexer = NULL;
	delete folderName;
	folderName = NULL;
	delete folder;
	folder = NULL;
}

Layer::Layer(Literal *mode_, OptionSet *options, bool denseStyles) {
	styles = NULL;
	len = 0;
	subModes = NULL;
	SubMode *sm = new SubMode(mode_, options);
	AddSubMode(sm);
	// TODO Include a hint in options to choose a good style representation
	if (denseStyles) {
		styles = SplitText::CreateStyles(1);
	} else {
		styles = new RunStyles();
	}
	if (sm->folder != NULL) {
		stylesFolder = SplitText::CreateStyles(4);
	} else {
		stylesFolder = NULL;
	}
}

Layer::~Layer() {
	delete styles;
	styles = NULL;
	delete stylesFolder;
	stylesFolder = NULL;
	for (int i=0;i<len;i++) {
		delete subModes[i];
		subModes[i] = NULL;
	}
	delete []subModes;
	subModes = NULL;
	len = 0;
}

void Layer::ReAllocate(int newSize) {
	if (newSize == 0) {
		newSize = 1;
	}
	SubMode **newSubModes = new SubMode*[newSize];
	for (int i=0; i<newSize; i++) {
		newSubModes[i] = NULL;
		if ((subModes != NULL) && (i < len)) {
			newSubModes[i] = subModes[i];
		}
	}
	delete []subModes;
	subModes = newSubModes;
	len = newSize;
}

void Layer::AddSubMode(SubMode *sm) {
	ReAllocate(len + 1);
	for (int i=0; i<len; i++) {
		if (subModes[i] == NULL) {
			subModes[i] = sm;
			return;
		}
	}
}

Document::Document(Literal *fontName, LiteralSet *fontNames_) {
	pbuffer = new SplitText(8,8);
	options = new OptionSet();
	currentMode = new Literal("");
	ssc = new StyleSetCollection(0, fontNames_->FindOrAddKey(fontName));
	len = 0;
	layers = NULL;
	fontNames = fontNames_;
}

Document::~Document() {
	delete pbuffer;
	pbuffer = NULL;
	delete options;
	options = NULL;
	DeallocateStyling();
	delete currentMode;
	currentMode = NULL;
}

void Document::AllocateText() {
	delete pbuffer;
	pbuffer = new SplitText(8,8);
}

void Document::DeallocateStyling() {
	if (pbuffer != NULL) {
		pbuffer->DetachAllStyles();
	}
	delete ssc;
	ssc = NULL;
	for (int i=0;i<len;i++) {
		delete layers[i];
		layers[i] = NULL;
	}
	delete []layers;
	layers = NULL;
	len = 0;
}

/// Must not be used to decrease the size of the buffer.
void Document::ReAllocate(int newSize) {
	if (newSize == 0) {
		newSize = 1;
	}
	Layer **newLayers = new Layer*[newSize];
	for (int i=0; i<newSize; i++) {
		newLayers[i] = NULL;
		if ((layers != NULL) && (i < len)) {
			newLayers[i] = layers[i];
		}
	}
	delete []layers;
	layers = newLayers;
	len = newSize;
}

void Document::AddLayer(Layer *layer) {
	ReAllocate(len + 1);
	for (int i=0; i<len; i++) {
		if (layers[i] == NULL) {
			layers[i] = layer;
			return;
		}
	}
}

void Document::ClearOptions() {
	delete options;
	options = new OptionSet();
	options->Allocate(64);
}

void Document::SetLexStyles(ILexer *lex, StyleSet *ss, Literal *mode) {
	Literal *names = lex->StateNames();
	for (int elem=0;elem<=names->Count(' ');elem++) {
		Literal *styleName = names->Segment(elem, ' ');
		Literal styleBranch = Literal("style");
		Literal styleSeg = Literal(&styleBranch, (SW_BYTE)'.', styleName);
		StyleModification *sty = StyleModification::FromOptions(
			mode, &styleSeg, options, fontNames);
		ss->Set(lex->GetBaseState() + elem, sty);
		delete styleName;
	}
	for (int kwSet=1;kwSet<9;kwSet++) {
		Literal number = Literal(kwSet);
		Literal keywordName = Literal("keywords");
		Literal keywordN = Literal(&keywordName, (SW_BYTE)'.', &number);
		Literal *kwValue = options->ModeValue(mode, &keywordN);
		if (kwValue != NULL) {
			lex->SetKeyWords(kwSet, kwValue);
		}
	}
}

void Layer::CreateSubLexers(SubMode *smBase, OptionSet *options) {
	Literal subModeName = Literal("submodes");
	Literal *subModes = options->ModeValue(smBase->mode, &subModeName);
	if (subModes != NULL) {
		for (int sub=0;sub<=subModes->Count((SW_BYTE)' ');sub++) {
			Literal *smx = subModes->Segment(sub, ' ');
			Literal *plug = NULL;
			Literal *submode = smx;
			// TODO Is there really a need for a new character '|' for this role?
			if (smx->Count((SW_BYTE)'|') > 0) {
				plug = smx->Segment(0, '|');
				submode = smx->Segment(1, '|');
			}
			SubMode *sm = new SubMode(submode, options);
			AddSubMode(sm);
			Literal litIdentifiers = Literal("identifiers");
			Literal *lSubModeIdentifiers = options->ModeValue(sm->mode, &litIdentifiers);
			smBase->lexer->SetSubLexer(sm->lexer, (plug != NULL) ? plug : sm->lexerName, lSubModeIdentifiers);
			CreateSubLexers(sm, options);
			if (plug != NULL) {
				delete plug;
				delete submode;
			}
			delete smx;
		}
	}
}

void Document::InitialiseSubLexers(Layer *layer, int priority) {
	AddLayer(layer);
	pbuffer->AttachStyles(layer->styles, layer->subModes[0]->lexer, priority, false);
	if (layer->stylesFolder != NULL) {
		pbuffer->AttachStyles(layer->stylesFolder, layer->subModes[0]->folder, priority, true);
	}
	layer->CreateSubLexers(layer->subModes[0], options);
	layer->subModes[0]->lexer->SetBaseState(0);
	layer->subModes[0]->lexer->AttachHistories(pbuffer, priority-1);
	StyleSet *ss = new StyleSet(layer->styles, layer->subModes[0]->lexer->States());
	SetLexStyles(layer->subModes[0]->lexer, ss, layer->subModes[0]->mode);
	for (int s=1;s<layer->len;s++) {
		if (layer->subModes[s] != NULL) {
			SetLexStyles(layer->subModes[s]->lexer, ss, layer->subModes[s]->mode);
		}
	}
	ssc->Add(ss);
}

void Document::CreateStyling(Literal *literalExtension) {
	DeallocateStyling();

	ssc = new StyleSetCollection(6, fontNames->FindOrAddKey(Surface::fontDefault));

	// Determine mode from file extension
	Literal exts = Literal(".extensions");
	WordList *keys=options->Keys(&exts);
	for (int k=0;k<keys->Length();k++) {
		Literal *lKey = keys->GetText(k);
		Literal *lVal = options->Value(lKey);
		WordList *wlKeys = new WordList(false);
		wlKeys->SetText(lVal);
		if (wlKeys->InList(literalExtension->Data(), 0, literalExtension->Length())) {
			delete currentMode;
			currentMode = new Literal(lKey, 0, lKey->Length() - exts.Length());
		}
		delete lKey;
		delete wlKeys;
	}
	delete keys;

	// Create main layer
	Layer *layerMain = new Layer(currentMode, options, true);
	InitialiseSubLexers(layerMain, 3);

	// Create additional layers
	Literal layerNames = Literal("layers");
	Literal *layerModes = options->ModeValue(currentMode, &layerNames);
	if (layerModes != NULL) {
		for (int l=0;l<=layerModes->Count((SW_BYTE)' ');l++) {
			Literal *layerMode = layerModes->Segment(l, ' ');
			Layer *layer = new Layer(layerMode, options, false);
			InitialiseSubLexers(layer, 4);
			delete layerMode;
		}
	}

	ssc->SetSpecifiedFlags();
}

int Document::MovePositionOutsideChar(int position, int moveDirection, bool checkLineEnd) {
	return pbuffer->MovePositionOutsideChar(position, moveDirection, checkLineEnd);
}

bool Document::IsWordCharacter(int ch) {
	return (ch >= 128) ||
		(ch == '_') ||
		(ch >= 'a' && ch <= 'z') ||
		(ch >= 'A' && ch <= 'Z') ||
		(ch >= '0' && ch <= '9');
}

bool Document::IsSpaceTab(int ch) {
	return (ch == ' ') || (ch == '\t');
}

int Document::ExtendWordSelect(int pos, int delta) {
	if (delta < 0) {
		while ((pos > 0) && 
			IsWordCharacter(pbuffer->CharValueBefore(pos)))
			pos = pbuffer->PositionBefore(pos);
	} else {
		while (pos < (pbuffer->Length()) && IsWordCharacter(pbuffer->CharValue(pos)))
			pos = pbuffer->PositionNext(pos);
	}
	return pos;
}

int Document::NextWordStart(int pos, int delta) {
	int ret = pos;
	if (delta < 0) {
		while (ret > 0 && IsSpaceTab(pbuffer->CharValueBefore(ret)))
			ret = pbuffer->PositionBefore(ret);
		while (ret > 0 && IsWordCharacter(pbuffer->CharValueBefore(ret)))
			ret = pbuffer->PositionBefore(ret);
	} else {
		// Over word characters.
		while (ret < (pbuffer->Length()) && IsWordCharacter(pbuffer->CharValue(ret)))
			ret = pbuffer->PositionNext(ret);
		// Ensure some progress made.
		if (ret < (pbuffer->Length()) && (ret == pos))
			ret = pbuffer->PositionNext(ret);
		// Over the following white space.
		while (ret < (pbuffer->Length()) && IsSpaceTab(pbuffer->CharValue(ret)))
			ret = pbuffer->PositionNext(ret);
	}
	return ret;
}

IStyles *Document::GetFolds() {
	if (layers == NULL) {
		return NULL;
	} else {
		return layers[0]->stylesFolder;
	}
}
