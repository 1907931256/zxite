// SinkWorld LexHTML class.
// Copyright 2004 Neil Hodgson
// File written in C~ subset of C++ for automatic translation to Java and C#.

/// LexHTML is a lexer for the HTML and XML languages.
/// It supports sublexers for embedded script elements.

SW_PUBLIC class LexHTML : public LexScript {
	typedef LexScript super;
public:
	enum { Default = 0 };
	enum { Comment = 1 };
	enum { Tag = 2 };
	enum { TagUnknown = 3 };
	enum { Attribute = 4 };
	enum { AttributeUnknown = 5 };
	enum { Number = 6 };
	enum { DoubleString = 7 };
	enum { SingleString = 8 };
	enum { Other = 9 };
	enum { Entity = 10 };
	enum { TagEnd = 11 };
	enum { XMLStart = 12 };
	enum { XMLEnd = 13 };
	enum { Script = 14 };
	enum { CDATA = 15 };
	enum { Value = 16 };
	enum { SGML = 17 };
	enum { SGMLCommand = 18 };
	enum { SGMLDoubleString = 19 };
	enum { SGMLSimpleString = 20 };

	Literal commentStart;
	Literal commentEnd;
	Literal scriptTag;
	Literal srcScript;
	Literal cdataStart;
	Literal cdataEnd;

	KeyWords *keyWordsTags;
	KeyWords *keyWordsAttributes;

	History *defaultLexer;

	LexHTML() {
		thisStates = 21;
		delete name;
		name = new Literal("HTML");
		delete stateNames;
		stateNames = new Literal(
"Default Comment Tag TagUnknown Attribute AttributeUnknown Number DoubleString SingleString Other Entity TagEnd XMLStart XMLEnd Script CDATA Value SGML SGMLCommand SGMLDoubleString SGMLSimpleString");

		commentStart = Literal("<!--");
		commentEnd = Literal("-->");
		scriptTag = Literal("<script");
		srcScript = Literal("src");
		cdataStart = Literal("<![CDATA[");
		cdataEnd = Literal("]]>");
		keyWordsTags = new KeyWords(false);
		keyWordsAttributes = new KeyWords(false);
		defaultLexer = new History(-1);
	}

	virtual ~LexHTML() {
		delete keyWordsTags;
		keyWordsTags = NULL;
		delete keyWordsAttributes;
		keyWordsAttributes = NULL;
		delete defaultLexer;
		defaultLexer = NULL;
	}

	SW_OVERRIDE IPrototype *CreateNew() {
		return new LexHTML();
	}

	SW_OVERRIDE void SetKeyWords(int n, Literal *keyWordsN) {
		if (n == 1) {
			keyWordsTags->SetText(keyWordsN, 1);
		} else if (n == 2) {
			keyWordsAttributes->SetText(keyWordsN, 1);
		}
	}

	SW_OVERRIDE void InvalidateAfter(int position) {
		defaultLexer->InvalidateAfter(position);
		super::InvalidateAfter(position);
	}
	SW_OVERRIDE void ColourisePiece(Styler *sc);
	SW_OVERRIDE void Colourise(Styler *sc);

	SW_OVERRIDE bool CanBeInterrupted(int state) {
		int stateLocal = state - baseState;
		return (stateLocal == Default) ||
			(stateLocal == TagUnknown) ||
			(stateLocal == XMLStart);
	}

	SW_OVERRIDE void AllocateStates(int startAllocation) {
		subStates = 0;
		super::AllocateStates(startAllocation);
	}

	static bool isHtmlWordChar(int ch) {
		return Styler::IsAlphaNumeric(ch) || ch == '.' || ch == '-' || ch == '_' || ch == ':' || ch == '!' || ch == '#';
	}

	static bool isSgmlWordChar(int ch) {
		return Styler::IsAlphaNumeric(ch) || ch == '.' || ch == '_' || ch == ':' || ch == '!' || ch == '#' || ch == '[';
	}

	void CheckForLexerName(Styler *sc) {
		if (defaultLexer->Value() >= 0) {
			Literal *t = sc->GetCurrentLowered();
			int lexNum = FindSubLexer(t->Data(), t->Length());
			if (lexNum >= 0) {
				defaultLexer->Append(sc->startSeg, lexNum);
			}
			delete t;
		}
	}

	void EndTagCheckScripting(Styler *sc) {
		sc->SetState(Tag + baseState);
		int scriptState = defaultLexer->Value();
		if (scriptState >= 0) {
			int lexNum = scriptState;
			SubLexer *slScript = slScriptsRoot;
			while ((slScript != NULL) && (lexNum > 0)) {
				slScript = slScript->slNext;
				lexNum--;
			}
			if (slScript == NULL) {
				// Unknown script
				sc->ForwardSetState(Script + baseState);
			} else {
				sc->ForwardSetState(slScript->lexer->GetBaseState());
			}
		} else {
			sc->ForwardSetState(Default + baseState);
		}
	}
};
