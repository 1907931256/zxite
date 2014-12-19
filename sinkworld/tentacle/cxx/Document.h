// SinkWorld Document class.
// Copyright 2005 Neil Hodgson
// File written in C~ subset of C++ for automatic translation to Java and C#.

/// A SubMode joins a lexer into a Layer and is responsible for a range of styling values
/// within the owning Layer object's styling buffer. Control is switched around between 
/// lexers by code in the lexer as lexical elements are found.

class SubMode {
public:
	Literal *mode;
	Literal *lexerName;
	ILexer *lexer;
	Literal *folderName;
	ILexer *folder;
	SubMode(Literal *mode_, OptionSet *options);
	~SubMode();
};

/// A Layer has a styling buffer that its lexer(s) fill with information derived from the 
/// text or lower layers.

class Layer {
	void ReAllocate(int newSize);
public:
	IStyles *styles;
	IStyles *stylesFolder;
	int len;
	SubMode **subModes;

	Layer(Literal *mode_, OptionSet *options, bool denseStyles);
	~Layer();
	void AddSubMode(SubMode *sm);
	void CreateSubLexers(SubMode *smBase, OptionSet *options);
};

/// Holds the text of a document, the options and mode name associated with that document,
/// the style buffers and style appearence, and a set of set of layered modes that 
/// determine how the document will be interpreted and styled.
/// It is the primary owner of subobjects so is responsible for their lifetime.
/// It does not have access to views or other user interface objects.

SW_PUBLIC class Document {
	void ReAllocate(int newSize);
	Layer **layers;
	void AddLayer(Layer *layer);
	void InitialiseSubLexers(Layer *layer, int priority);
public:
	SplitText *pbuffer;
	OptionSet *options;
	Literal *currentMode;
	StyleSetCollection *ssc;
	int len;
	LiteralSet *fontNames;

	Document(Literal *fontName, LiteralSet *fontNames_);
	~Document();
	void AllocateText();
	void DeallocateStyling();
	void ClearOptions();

	void SetLexStyles(ILexer *lex, StyleSet *ss, Literal *mode);
	void CreateStyling(Literal *literalExtension);

	static bool IsWordCharacter(int ch);
	static bool IsSpaceTab(int ch);
	int MovePositionOutsideChar(int position, int moveDirection, bool checkLineEnd);
	int ExtendWordSelect(int pos, int delta);
	int NextWordStart(int pos, int delta);

	IStyles *GetFolds();
};
