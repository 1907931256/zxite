#if !defined(AFX_SCINTILLACTRL_H__4A2DFF6E_C146_11D3_9CDF_482471000000__INCLUDED_)
#define AFX_SCINTILLACTRL_H__4A2DFF6E_C146_11D3_9CDF_482471000000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ScintillaCtrl.h : Declaration of the CScintillaCtrl ActiveX Control class.

/////////////////////////////////////////////////////////////////////////////
// CScintillaCtrl : See ScintillaCtrl.cpp for implementation.

class CScintillaCtrl : public COleControl
{
	DECLARE_DYNCREATE(CScintillaCtrl)

// Constructor
public:
	CScintillaCtrl();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScintillaCtrl)
	public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL
protected:
	virtual BOOL IsSubclassedControl(void);
protected:

// Implementation
protected:
	~CScintillaCtrl();

	DECLARE_OLECREATE_EX(CScintillaCtrl)    // Class factory and guid
	DECLARE_OLETYPELIB(CScintillaCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CScintillaCtrl)     // Property page IDs
	DECLARE_OLECTLTYPE(CScintillaCtrl)		// Type name and misc status

// Message maps
	//{{AFX_MSG(CScintillaCtrl)
	afx_msg LRESULT OnOcmNotify(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// Dispatch maps
	//$[DISPMSG_SECTION)
	//{{AFX_DISPATCH(CScintillaCtrl)
	afx_msg long GetINVALID_POSITION();
	afx_msg long GetSCI_START();
	afx_msg long GetSCI_OPTIONAL_START();
	afx_msg long GetSCI_LEXER_START();
	afx_msg long GetSCWS_INVISIBLE();
	afx_msg long GetSCWS_VISIBLEALWAYS();
	afx_msg long GetSCWS_VISIBLEAFTERINDENT();
	afx_msg long GetSC_EOL_CRLF();
	afx_msg long GetSC_EOL_CR();
	afx_msg long GetSC_EOL_LF();
	afx_msg long GetSC_CP_UTF8();
	afx_msg long GetMARKER_MAX();
	afx_msg long GetSC_MARK_CIRCLE();
	afx_msg long GetSC_MARK_ROUNDRECT();
	afx_msg long GetSC_MARK_ARROW();
	afx_msg long GetSC_MARK_SMALLRECT();
	afx_msg long GetSC_MARK_SHORTARROW();
	afx_msg long GetSC_MARK_EMPTY();
	afx_msg long GetSC_MARK_ARROWDOWN();
	afx_msg long GetSC_MARK_MINUS();
	afx_msg long GetSC_MARK_PLUS();
	afx_msg long GetSC_MARKNUM_FOLDER();
	afx_msg long GetSC_MARKNUM_FOLDEROPEN();
	afx_msg long GetSC_MARGIN_SYMBOL();
	afx_msg long GetSC_MARGIN_NUMBER();
	afx_msg long GetSTYLE_DEFAULT();
	afx_msg long GetSTYLE_LINENUMBER();
	afx_msg long GetSTYLE_BRACELIGHT();
	afx_msg long GetSTYLE_BRACEBAD();
	afx_msg long GetSTYLE_CONTROLCHAR();
	afx_msg long GetSTYLE_MAX();
	afx_msg long GetSC_CHARSET_ANSI();
	afx_msg long GetSC_CHARSET_DEFAULT();
	afx_msg long GetSC_CHARSET_BALTIC();
	afx_msg long GetSC_CHARSET_CHINESEBIG5();
	afx_msg long GetSC_CHARSET_EASTEUROPE();
	afx_msg long GetSC_CHARSET_GB2312();
	afx_msg long GetSC_CHARSET_GREEK();
	afx_msg long GetSC_CHARSET_HANGUL();
	afx_msg long GetSC_CHARSET_MAC();
	afx_msg long GetSC_CHARSET_OEM();
	afx_msg long GetSC_CHARSET_RUSSIAN();
	afx_msg long GetSC_CHARSET_SHIFTJIS();
	afx_msg long GetSC_CHARSET_SYMBOL();
	afx_msg long GetSC_CHARSET_TURKISH();
	afx_msg long GetSC_CHARSET_JOHAB();
	afx_msg long GetSC_CHARSET_HEBREW();
	afx_msg long GetSC_CHARSET_ARABIC();
	afx_msg long GetSC_CHARSET_VIETNAMESE();
	afx_msg long GetSC_CHARSET_THAI();
	afx_msg long GetINDIC_MAX();
	afx_msg long GetINDIC_PLAIN();
	afx_msg long GetINDIC_SQUIGGLE();
	afx_msg long GetINDIC_TT();
	afx_msg long GetINDIC_DIAGONAL();
	afx_msg long GetINDIC_STRIKE();
	afx_msg long GetINDIC0_MASK();
	afx_msg long GetINDIC1_MASK();
	afx_msg long GetINDIC2_MASK();
	afx_msg long GetINDICS_MASK();
	afx_msg long GetSC_PRINT_NORMAL();
	afx_msg long GetSC_PRINT_INVERTLIGHT();
	afx_msg long GetSC_PRINT_BLACKONWHITE();
	afx_msg long GetSCFIND_MATCHCASE();
	afx_msg long GetSCFIND_WHOLEWORD();
	afx_msg long GetSCFIND_DOWN();
	afx_msg long GetSC_FOLDLEVELBASE();
	afx_msg long GetSC_FOLDLEVELWHITEFLAG();
	afx_msg long GetSC_FOLDLEVELHEADERFLAG();
	afx_msg long GetSC_FOLDLEVELNUMBERMASK();
	afx_msg long GetEDGE_NONE();
	afx_msg long GetEDGE_LINE();
	afx_msg long GetEDGE_BACKGROUND();
	afx_msg long GetCARET_SLOP();
	afx_msg long GetCARET_CENTER();
	afx_msg long GetCARET_STRICT();
	afx_msg long GetSC_MOD_INSERTTEXT();
	afx_msg long GetSC_MOD_DELETETEXT();
	afx_msg long GetSC_MOD_CHANGESTYLE();
	afx_msg long GetSC_MOD_CHANGEFOLD();
	afx_msg long GetSC_PERFORMED_USER();
	afx_msg long GetSC_PERFORMED_UNDO();
	afx_msg long GetSC_PERFORMED_REDO();
	afx_msg long GetSC_LASTSTEPINUNDOREDO();
	afx_msg long GetSC_MOD_CHANGEMARKER();
	afx_msg long GetSC_MOD_BEFOREINSERT();
	afx_msg long GetSC_MOD_BEFOREDELETE();
	afx_msg long GetSC_MODEVENTMASKALL();
	afx_msg long GetSCN_STYLENEEDED();
	afx_msg long GetSCN_CHARADDED();
	afx_msg long GetSCN_SAVEPOINTREACHED();
	afx_msg long GetSCN_SAVEPOINTLEFT();
	afx_msg long GetSCN_MODIFYATTEMPTRO();
	afx_msg long GetSCN_KEY();
	afx_msg long GetSCN_DOUBLECLICK();
	afx_msg long GetSCN_UPDATEUI();
	afx_msg long GetSCN_CHECKBRACE();
	afx_msg long GetSCN_MODIFIED();
	afx_msg long GetSCN_MACRORECORD();
	afx_msg long GetSCN_MARGINCLICK();
	afx_msg long GetSCN_NEEDSHOWN();
	afx_msg long GetSCEN_CHANGE();
	afx_msg long GetSCEN_SETFOCUS();
	afx_msg long GetSCEN_KILLFOCUS();
	afx_msg long GetSCK_DOWN();
	afx_msg long GetSCK_UP();
	afx_msg long GetSCK_LEFT();
	afx_msg long GetSCK_RIGHT();
	afx_msg long GetSCK_HOME();
	afx_msg long GetSCK_END();
	afx_msg long GetSCK_PRIOR();
	afx_msg long GetSCK_NEXT();
	afx_msg long GetSCK_DELETE();
	afx_msg long GetSCK_INSERT();
	afx_msg long GetSCK_ESCAPE();
	afx_msg long GetSCK_BACK();
	afx_msg long GetSCK_TAB();
	afx_msg long GetSCK_RETURN();
	afx_msg long GetSCK_ADD();
	afx_msg long GetSCK_SUBTRACT();
	afx_msg long GetSCK_DIVIDE();
	afx_msg long GetSCMOD_SHIFT();
	afx_msg long GetSCMOD_CTRL();
	afx_msg long GetSCMOD_ALT();
	afx_msg long GetSCLEX_CONTAINER();
	afx_msg long GetSCLEX_NULL();
	afx_msg long GetSCLEX_PYTHON();
	afx_msg long GetSCLEX_CPP();
	afx_msg long GetSCLEX_HTML();
	afx_msg long GetSCLEX_XML();
	afx_msg long GetSCLEX_PERL();
	afx_msg long GetSCLEX_SQL();
	afx_msg long GetSCLEX_VB();
	afx_msg long GetSCLEX_PROPERTIES();
	afx_msg long GetSCLEX_ERRORLIST();
	afx_msg long GetSCLEX_MAKEFILE();
	afx_msg long GetSCLEX_BATCH();
	afx_msg long GetSCLEX_XCODE();
	afx_msg long GetSCLEX_LATEX();
	afx_msg long GetSCLEX_LUA();
	afx_msg long GetSCE_P_DEFAULT();
	afx_msg long GetSCE_P_COMMENTLINE();
	afx_msg long GetSCE_P_NUMBER();
	afx_msg long GetSCE_P_STRING();
	afx_msg long GetSCE_P_CHARACTER();
	afx_msg long GetSCE_P_WORD();
	afx_msg long GetSCE_P_TRIPLE();
	afx_msg long GetSCE_P_TRIPLEDOUBLE();
	afx_msg long GetSCE_P_CLASSNAME();
	afx_msg long GetSCE_P_DEFNAME();
	afx_msg long GetSCE_P_OPERATOR();
	afx_msg long GetSCE_P_IDENTIFIER();
	afx_msg long GetSCE_P_COMMENTBLOCK();
	afx_msg long GetSCE_P_STRINGEOL();
	afx_msg long GetSCE_C_DEFAULT();
	afx_msg long GetSCE_C_COMMENT();
	afx_msg long GetSCE_C_COMMENTLINE();
	afx_msg long GetSCE_C_COMMENTDOC();
	afx_msg long GetSCE_C_NUMBER();
	afx_msg long GetSCE_C_WORD();
	afx_msg long GetSCE_C_STRING();
	afx_msg long GetSCE_C_CHARACTER();
	afx_msg long GetSCE_C_UUID();
	afx_msg long GetSCE_C_PREPROCESSOR();
	afx_msg long GetSCE_C_OPERATOR();
	afx_msg long GetSCE_C_IDENTIFIER();
	afx_msg long GetSCE_C_STRINGEOL();
	afx_msg long GetSCE_H_DEFAULT();
	afx_msg long GetSCE_H_TAG();
	afx_msg long GetSCE_H_TAGUNKNOWN();
	afx_msg long GetSCE_H_ATTRIBUTE();
	afx_msg long GetSCE_H_ATTRIBUTEUNKNOWN();
	afx_msg long GetSCE_H_NUMBER();
	afx_msg long GetSCE_H_DOUBLESTRING();
	afx_msg long GetSCE_H_SINGLESTRING();
	afx_msg long GetSCE_H_OTHER();
	afx_msg long GetSCE_H_COMMENT();
	afx_msg long GetSCE_H_ENTITY();
	afx_msg long GetSCE_H_TAGEND();
	afx_msg long GetSCE_H_XMLSTART();
	afx_msg long GetSCE_H_XMLEND();
	afx_msg long GetSCE_H_SCRIPT();
	afx_msg long GetSCE_H_ASP();
	afx_msg long GetSCE_H_ASPAT();
	afx_msg long GetSCE_H_CDATA();
	afx_msg long GetSCE_H_QUESTION();
	afx_msg long GetSCE_HJ_START();
	afx_msg long GetSCE_HJ_DEFAULT();
	afx_msg long GetSCE_HJ_COMMENT();
	afx_msg long GetSCE_HJ_COMMENTLINE();
	afx_msg long GetSCE_HJ_COMMENTDOC();
	afx_msg long GetSCE_HJ_NUMBER();
	afx_msg long GetSCE_HJ_WORD();
	afx_msg long GetSCE_HJ_KEYWORD();
	afx_msg long GetSCE_HJ_DOUBLESTRING();
	afx_msg long GetSCE_HJ_SINGLESTRING();
	afx_msg long GetSCE_HJ_SYMBOLS();
	afx_msg long GetSCE_HJ_STRINGEOL();
	afx_msg long GetSCE_HJA_START();
	afx_msg long GetSCE_HJA_DEFAULT();
	afx_msg long GetSCE_HJA_COMMENT();
	afx_msg long GetSCE_HJA_COMMENTLINE();
	afx_msg long GetSCE_HJA_COMMENTDOC();
	afx_msg long GetSCE_HJA_NUMBER();
	afx_msg long GetSCE_HJA_WORD();
	afx_msg long GetSCE_HJA_KEYWORD();
	afx_msg long GetSCE_HJA_DOUBLESTRING();
	afx_msg long GetSCE_HJA_SINGLESTRING();
	afx_msg long GetSCE_HJA_SYMBOLS();
	afx_msg long GetSCE_HJA_STRINGEOL();
	afx_msg long GetSCE_HB_START();
	afx_msg long GetSCE_HB_DEFAULT();
	afx_msg long GetSCE_HB_COMMENTLINE();
	afx_msg long GetSCE_HB_NUMBER();
	afx_msg long GetSCE_HB_WORD();
	afx_msg long GetSCE_HB_STRING();
	afx_msg long GetSCE_HB_IDENTIFIER();
	afx_msg long GetSCE_HB_STRINGEOL();
	afx_msg long GetSCE_HBA_START();
	afx_msg long GetSCE_HBA_DEFAULT();
	afx_msg long GetSCE_HBA_COMMENTLINE();
	afx_msg long GetSCE_HBA_NUMBER();
	afx_msg long GetSCE_HBA_WORD();
	afx_msg long GetSCE_HBA_STRING();
	afx_msg long GetSCE_HBA_IDENTIFIER();
	afx_msg long GetSCE_HBA_STRINGEOL();
	afx_msg long GetSCE_HP_START();
	afx_msg long GetSCE_HP_DEFAULT();
	afx_msg long GetSCE_HP_COMMENTLINE();
	afx_msg long GetSCE_HP_NUMBER();
	afx_msg long GetSCE_HP_STRING();
	afx_msg long GetSCE_HP_CHARACTER();
	afx_msg long GetSCE_HP_WORD();
	afx_msg long GetSCE_HP_TRIPLE();
	afx_msg long GetSCE_HP_TRIPLEDOUBLE();
	afx_msg long GetSCE_HP_CLASSNAME();
	afx_msg long GetSCE_HP_DEFNAME();
	afx_msg long GetSCE_HP_OPERATOR();
	afx_msg long GetSCE_HP_IDENTIFIER();
	afx_msg long GetSCE_HPA_START();
	afx_msg long GetSCE_HPA_DEFAULT();
	afx_msg long GetSCE_HPA_COMMENTLINE();
	afx_msg long GetSCE_HPA_NUMBER();
	afx_msg long GetSCE_HPA_STRING();
	afx_msg long GetSCE_HPA_CHARACTER();
	afx_msg long GetSCE_HPA_WORD();
	afx_msg long GetSCE_HPA_TRIPLE();
	afx_msg long GetSCE_HPA_TRIPLEDOUBLE();
	afx_msg long GetSCE_HPA_CLASSNAME();
	afx_msg long GetSCE_HPA_DEFNAME();
	afx_msg long GetSCE_HPA_OPERATOR();
	afx_msg long GetSCE_HPA_IDENTIFIER();
	afx_msg long GetSCE_HPHP_DEFAULT();
	afx_msg long GetSCE_HPHP_HSTRING();
	afx_msg long GetSCE_HPHP_SIMPLESTRING();
	afx_msg long GetSCE_HPHP_WORD();
	afx_msg long GetSCE_HPHP_NUMBER();
	afx_msg long GetSCE_HPHP_VARIABLE();
	afx_msg long GetSCE_HPHP_COMMENT();
	afx_msg long GetSCE_HPHP_COMMENTLINE();
	afx_msg long GetSCE_HPHP_STRINGEOL();
	afx_msg long GetSCE_PL_DEFAULT();
	afx_msg long GetSCE_PL_HERE();
	afx_msg long GetSCE_PL_COMMENTLINE();
	afx_msg long GetSCE_PL_POD();
	afx_msg long GetSCE_PL_NUMBER();
	afx_msg long GetSCE_PL_WORD();
	afx_msg long GetSCE_PL_STRING();
	afx_msg long GetSCE_PL_CHARACTER();
	afx_msg long GetSCE_PL_PUNCTUATION();
	afx_msg long GetSCE_PL_PREPROCESSOR();
	afx_msg long GetSCE_PL_OPERATOR();
	afx_msg long GetSCE_PL_IDENTIFIER();
	afx_msg long GetSCE_PL_SCALAR();
	afx_msg long GetSCE_PL_ARRAY();
	afx_msg long GetSCE_PL_HASH();
	afx_msg long GetSCE_PL_SYMBOLTABLE();
	afx_msg long GetSCE_PL_REF();
	afx_msg long GetSCE_PL_REGEX();
	afx_msg long GetSCE_PL_REGSUBST();
	afx_msg long GetSCE_PL_LONGQUOTE();
	afx_msg long GetSCE_PL_BACKTICKS();
	afx_msg long GetSCE_PL_DATASECTION();
	afx_msg long GetSCE_L_DEFAULT();
	afx_msg long GetSCE_L_COMMAND();
	afx_msg long GetSCE_L_TAG();
	afx_msg long GetSCE_L_MATH();
	afx_msg long GetSCE_L_COMMENT();
	afx_msg long GetSCE_LUA_DEFAULT();
	afx_msg long GetSCE_LUA_COMMENT();
	afx_msg long GetSCE_LUA_COMMENTLINE();
	afx_msg long GetSCE_LUA_COMMENTDOC();
	afx_msg long GetSCE_LUA_NUMBER();
	afx_msg long GetSCE_LUA_WORD();
	afx_msg long GetSCE_LUA_STRING();
	afx_msg long GetSCE_LUA_CHARACTER();
	afx_msg long GetSCE_LUA_LITERALSTRING();
	afx_msg long GetSCE_LUA_PREPROCESSOR();
	afx_msg long GetSCE_LUA_OPERATOR();
	afx_msg long GetSCE_LUA_IDENTIFIER();
	afx_msg long GetSCE_LUA_STRINGEOL();
	afx_msg long GetEN_CHANGE();
	afx_msg long GetEN_KILLFOCUS();
	afx_msg long GetEN_SETFOCUS();
	afx_msg long GetEC_LEFTMARGIN();
	afx_msg long GetEC_RIGHTMARGIN();
	afx_msg long GetEC_USEFONTINFO();
	afx_msg long GetSEL_EMPTY();
	afx_msg long GetSEL_TEXT();
	afx_msg long GetFR_MATCHCASE();
	afx_msg long GetFR_WHOLEWORD();
	afx_msg long GetFR_DOWN();
	afx_msg long GetSHIFT_PRESSED();
	afx_msg long GetLEFT_CTRL_PRESSED();
	afx_msg long GetLEFT_ALT_PRESSED();
	afx_msg void AddText(long length,LPCTSTR text);
	afx_msg void AddStyledText(long length,LPCTSTR c);
	afx_msg void InsertText(long pos,LPCTSTR text);
	afx_msg void ClearAll();
	afx_msg void ClearDocumentStyle();
	afx_msg long GetLength();
	afx_msg long GetCharAt(long pos);
	afx_msg long GetCurrentPos();
	afx_msg long GetAnchor();
	afx_msg long GetStyleAt(long pos);
	afx_msg void Redo();
	afx_msg void SetUndoCollection(BOOL collectUndo);
	afx_msg void SelectAll();
	afx_msg void SetSavePoint();
	afx_msg long GetStyledText(long trMin,long trMax,BSTR FAR* trStr);
	afx_msg BOOL CanRedo();
	afx_msg long MarkerLineFromHandle(long handle);
	afx_msg void MarkerDeleteHandle(long handle);
	afx_msg BOOL GetUndoCollection();
	afx_msg long GetViewWS();
	afx_msg void SetViewWS(long viewWS);
	afx_msg long PositionFromPoint(long x,long y);
	afx_msg void GotoLine(long line);
	afx_msg void GotoPos(long pos);
	afx_msg void SetAnchor(long posAnchor);
	afx_msg long GetCurLine(long length,BSTR FAR* textStr);
	afx_msg long GetEndStyled();
	afx_msg void ConvertEOLs();
	afx_msg long GetEOLMode();
	afx_msg void SetEOLMode(long eolMode);
	afx_msg void StartStyling(long pos,long mask);
	afx_msg void SetStyling(long length,long style);
	afx_msg BOOL GetBufferedDraw();
	afx_msg void SetBufferedDraw(BOOL buffered);
	afx_msg void SetTabWidth(long tabWidth);
	afx_msg long GetTabWidth();
	afx_msg void SetCodePage(long codePage);
	afx_msg void SetUsePalette(BOOL usePalette);
	afx_msg void MarkerDefine(long markerNumber,long markerSymbol);
	afx_msg void MarkerSetFore(long markerNumber,long fore);
	afx_msg void MarkerSetBack(long markerNumber,long back);
	afx_msg void MarkerAdd(long line,long markerNumber);
	afx_msg void MarkerDelete(long line,long markerNumber);
	afx_msg void MarkerDeleteAll(long markerNumber);
	afx_msg long MarkerGet(long line);
	afx_msg long MarkerNext(long lineStart,long markerMask);
	afx_msg long MarkerPrevious(long lineStart,long markerMask);
	afx_msg void SetMarginTypeN(long margin,long marginType);
	afx_msg long GetMarginTypeN(long margin);
	afx_msg void SetMarginWidthN(long margin,long pixelWidth);
	afx_msg long GetMarginWidthN(long margin);
	afx_msg void SetMarginMaskN(long margin,long mask);
	afx_msg long GetMarginMaskN(long margin);
	afx_msg void SetMarginSensitiveN(long margin,BOOL sensitive);
	afx_msg BOOL GetMarginSensitiveN(long margin);
	afx_msg void StyleClearAll();
	afx_msg void StyleSetFore(long style,long fore);
	afx_msg void StyleSetBack(long style,long back);
	afx_msg void StyleSetBold(long style,BOOL bold);
	afx_msg void StyleSetItalic(long style,BOOL italic);
	afx_msg void StyleSetSize(long style,long sizePoints);
	afx_msg void StyleSetFont(long style,LPCTSTR fontName);
	afx_msg void StyleSetEOLFilled(long style,BOOL filled);
	afx_msg void StyleResetDefault();
	afx_msg void StyleSetUnderline(long style,BOOL underline);
	afx_msg void StyleSetCharacterSet(long style,long characterSet);
	afx_msg void SetSelFore(BOOL useSetting,long fore);
	afx_msg void SetSelBack(BOOL useSetting,long back);
	afx_msg void SetCaretFore(long fore);
	afx_msg void AssignCmdKey(short kmKey,short kmMod,long msg);
	afx_msg void ClearCmdKey(short kmKey,short kmMod);
	afx_msg void ClearAllCmdKeys();
	afx_msg void SetStylingEx(long length,LPCTSTR styles);
	afx_msg long GetCaretPeriod();
	afx_msg void SetCaretPeriod(long periodMilliseconds);
	afx_msg void SetWordChars(LPCTSTR characters);
	afx_msg void BeginUndoAction();
	afx_msg void EndUndoAction();
	afx_msg void IndicSetStyle(long indic,long style);
	afx_msg long IndicGetStyle(long indic);
	afx_msg void IndicSetFore(long indic,long fore);
	afx_msg long IndicGetFore(long indic);
	afx_msg void SetStyleBits(long bits);
	afx_msg long GetStyleBits();
	afx_msg void SetLineState(long line,long state);
	afx_msg long GetLineState(long line);
	afx_msg long GetMaxLineState();
	afx_msg void AutoCShow(long lenEntered,LPCTSTR itemList);
	afx_msg void AutoCCancel();
	afx_msg BOOL AutoCActive();
	afx_msg long AutoCPosStart();
	afx_msg void AutoCComplete();
	afx_msg void AutoCStops(LPCTSTR characterSet);
	afx_msg void AutoCSetSeparator(long separatorCharacter);
	afx_msg long AutoCGetSeparator();
	afx_msg void AutoCSelect(LPCTSTR text);
	afx_msg void AutoCSetCancelAtStart(BOOL cancel);
	afx_msg BOOL AutoCGetCancelAtStart();
	afx_msg void SetIndent(long indentSize);
	afx_msg long GetIndent();
	afx_msg void SetUseTabs(BOOL useTabs);
	afx_msg BOOL GetUseTabs();
	afx_msg void SetLineIndentation(long line,long indentSize);
	afx_msg long GetLineIndentation(long line);
	afx_msg long GetLineIndentPosition(long line);
	afx_msg long GetColumn(long pos);
	afx_msg void SetHScrollBar(BOOL show);
	afx_msg BOOL GetHScrollBar();
	afx_msg void SetIndentationGuides(BOOL show);
	afx_msg BOOL GetIndentationGuides();
	afx_msg void SetHighlightGuide(long column);
	afx_msg long GetHighlightGuide();
	afx_msg long GetLineEndPosition(long line);
	afx_msg long GetCodePage();
	afx_msg long GetCaretFore();
	afx_msg BOOL GetUsePalette();
	afx_msg BOOL GetReadOnly();
	afx_msg void SetCurrentPos(long pos);
	afx_msg void SetSelectionStart(long pos);
	afx_msg long GetSelectionStart();
	afx_msg void SetSelectionEnd(long pos);
	afx_msg long GetSelectionEnd();
	afx_msg void SetPrintMagnification(long magnification);
	afx_msg long GetPrintMagnification();
	afx_msg void SetPrintColourMode(long mode);
	afx_msg long GetPrintColourMode();
	afx_msg long FindText(long flags,long ftMin,long ftMax,LPCTSTR ftStr);
	afx_msg void FormatRange(BOOL draw,long fr);
	afx_msg long GetFirstVisibleLine();
	afx_msg long GetLine(long line,BSTR FAR* textStr);
	afx_msg long GetLineCount();
	afx_msg void SetMarginLeft(long width);
	afx_msg long GetMarginLeft();
	afx_msg void SetMarginRight(long width);
	afx_msg long GetMarginRight();
	afx_msg BOOL GetModify();
	afx_msg void SetSel(long start,long end);
	afx_msg long GetSelText(BSTR FAR* textStr);
	afx_msg long GetTextRange(long trMin,long trMax,BSTR FAR* trStr);
	afx_msg void HideSelection(BOOL normal);
	afx_msg long PointXFromPosition(long pos);
	afx_msg long PointYFromPosition(long pos);
	afx_msg long LineFromPosition(long pos);
	afx_msg long PositionFromLine(long line);
	afx_msg void LineScroll(long columns,long lines);
	afx_msg void ScrollCaret();
	afx_msg void ReplaceSel(LPCTSTR text);
	afx_msg void SetReadOnly(BOOL readOnly);
	afx_msg void Null();
	afx_msg BOOL CanPaste();
	afx_msg BOOL CanUndo();
	afx_msg void EmptyUndoBuffer();
	afx_msg void Undo();
	afx_msg void Cut();
	afx_msg void Copy();
	afx_msg void Paste();
	afx_msg void Clear();
	afx_msg void SetText(LPCTSTR text);
	afx_msg long GetText(long length,BSTR FAR* textStr);
	afx_msg long GetTextLength();
	afx_msg long GetDirectFunction();
	afx_msg long GetDirectPointer();
	afx_msg void CallTipShow(long pos,LPCTSTR definition);
	afx_msg void CallTipCancel();
	afx_msg BOOL CallTipActive();
	afx_msg long CallTipPosStart();
	afx_msg void CallTipSetHlt(long start,long end);
	afx_msg void CallTipSetBack(long back);
	afx_msg long VisibleFromDocLine(long line);
	afx_msg long DocLineFromVisible(long lineDisplay);
	afx_msg void SetFoldLevel(long line,long level);
	afx_msg long GetFoldLevel(long line);
	afx_msg long GetLastChild(long line,long level);
	afx_msg long GetFoldParent(long line);
	afx_msg void ShowLines(long lineStart,long lineEnd);
	afx_msg void HideLines(long lineStart,long lineEnd);
	afx_msg BOOL GetLineVisible(long line);
	afx_msg void SetFoldExpanded(long line,BOOL expanded);
	afx_msg BOOL GetFoldExpanded(long line);
	afx_msg void ToggleFold(long line);
	afx_msg void EnsureVisible(long line);
	afx_msg void SetFoldFlags(long flags);
	afx_msg void LineDown();
	afx_msg void LineDownExtend();
	afx_msg void LineUp();
	afx_msg void LineUpExtend();
	afx_msg void CharLeft();
	afx_msg void CharLeftExtend();
	afx_msg void CharRight();
	afx_msg void CharRightExtend();
	afx_msg void WordLeft();
	afx_msg void WordLeftExtend();
	afx_msg void WordRight();
	afx_msg void WordRightExtend();
	afx_msg void Home();
	afx_msg void HomeExtend();
	afx_msg void LineEnd();
	afx_msg void LineEndExtend();
	afx_msg void DocumentStart();
	afx_msg void DocumentStartExtend();
	afx_msg void DocumentEnd();
	afx_msg void DocumentEndExtend();
	afx_msg void PageUp();
	afx_msg void PageUpExtend();
	afx_msg void PageDown();
	afx_msg void PageDownExtend();
	afx_msg void EditToggleOvertype();
	afx_msg void Cancel();
	afx_msg void DeleteBack();
	afx_msg void Tab();
	afx_msg void BackTab();
	afx_msg void NewLine();
	afx_msg void FormFeed();
	afx_msg void VCHome();
	afx_msg void VCHomeExtend();
	afx_msg void ZoomIn();
	afx_msg void ZoomOut();
	afx_msg void DelWordLeft();
	afx_msg void DelWordRight();
	afx_msg void LineCut();
	afx_msg void LineDelete();
	afx_msg void LineTranspose();
	afx_msg void LowerCase();
	afx_msg void UpperCase();
	afx_msg void LineScrollDown();
	afx_msg void LineScrollUp();
	afx_msg long LineLength(long line);
	afx_msg void BraceHighlight(long pos1,long pos2);
	afx_msg void BraceBadLight(long pos);
	afx_msg long BraceMatch(long pos);
	afx_msg BOOL GetViewEOL();
	afx_msg void SetViewEOL(BOOL visible);
	afx_msg long GetDocPointer();
	afx_msg void SetDocPointer(long pointer);
	afx_msg void SetModEventMask(long mask);
	afx_msg long GetEdgeColumn();
	afx_msg void SetEdgeColumn(long column);
	afx_msg long GetEdgeMode();
	afx_msg void SetEdgeMode(long mode);
	afx_msg long GetEdgeColour();
	afx_msg void SetEdgeColour(long edgeColour);
	afx_msg void SearchAnchor();
	afx_msg long SearchNext(long flags,LPCTSTR text);
	afx_msg long SearchPrev(long flags,LPCTSTR text);
	afx_msg void SetCaretPolicy(long caretPolicy,long caretSlop);
	afx_msg long LinesOnScreen();
	afx_msg void UsePopUp(BOOL allowPopUp);
	afx_msg BOOL SelectionIsRectangle();
	afx_msg void SetZoom(long zoom);
	afx_msg long GetZoom();
	afx_msg long CreateDocument();
	afx_msg void AddRefDocument(long doc);
	afx_msg void ReleaseDocument(long doc);
	afx_msg long GetModEventMask();
	afx_msg void GrabFocus();
	afx_msg void StartRecord();
	afx_msg void StopRecord();
	afx_msg void SetLexer(long lexer);
	afx_msg long GetLexer();
	afx_msg void Colourise(long start,long end);
	afx_msg void SetProperty(LPCTSTR key,LPCTSTR value);
	afx_msg void SetKeyWords(long keywordSet,LPCTSTR keyWords);
	afx_msg BOOL EM_CanPaste();
	afx_msg BOOL EM_CanUndo();
	afx_msg long EM_CharFromPos(long ptX,long ptY);
	afx_msg void EM_EmptyUndoBuffer();
	afx_msg void EM_ExGetSel(long* crMin,long* crMax);
	afx_msg long EM_ExLineFromChar(long pos);
	afx_msg void EM_ExSetSel(long crMin,long crMax);
	afx_msg long EM_FindText(long flags,long ftMin,long ftMax,LPCTSTR ftStr);
	afx_msg long EM_FindTextEx(long flags,long ftMin,long ftMax,LPCTSTR ftStr,long* ftTxtMin,long* ftTxtMax);
	afx_msg void EM_FormatRange(BOOL draw,long fr);
	afx_msg long EM_GetFirstVisibleLine();
	afx_msg long EM_GetLine(long line,long textLen,BSTR FAR* textStr);
	afx_msg long EM_GetLineCount();
	afx_msg long EM_GetMargins();
	afx_msg BOOL EM_GetModify();
	afx_msg void EM_GetRect(long* rLeft,long* rTop,long* rRight,long* rBottom);
	afx_msg long EM_GetSel();
	afx_msg long EM_GetSelText(BSTR FAR* textStr);
	afx_msg long EM_GetTextRange(long trMin,long trMax,BSTR FAR* trStr);
	afx_msg void EM_HideSelection(BOOL normal);
	afx_msg long EM_LineFromChar(long pos);
	afx_msg long EM_LineIndex(long line);
	afx_msg long EM_LineLength(long line);
	afx_msg void EM_LineScroll(long columns,long lines);
	afx_msg void EM_PosFromChar(long* ptX,long* ptY,long pos);
	afx_msg void EM_ReplaceSel(LPCTSTR text);
	afx_msg void EM_ScrollCaret();
	afx_msg void EM_SelectionType();
	afx_msg void EM_SetMargins(long flags,long values);
	afx_msg void EM_SetReadOnly(BOOL readOnly);
	afx_msg void EM_SetSel(long start,long end);
	afx_msg void EM_Undo();
	afx_msg void WM_Null();
	afx_msg void WM_Clear();
	afx_msg void WM_Command();
	afx_msg void WM_Copy();
	afx_msg void WM_Cut();
	afx_msg long WM_GetText(long length,BSTR FAR* textStr);
	afx_msg long WM_GetTextLength();
	afx_msg void WM_Notify(long id,long stuff);
	afx_msg void WM_Paste();
	afx_msg void WM_SetText(LPCTSTR text);
	afx_msg void WM_Undo();
	//}}AFX_DISPATCH
	//$(DISPMSG_SECTION]
	DECLARE_DISPATCH_MAP()

// Event maps
	//$[DISPEVT_SECTION)
	//{{AFX_EVENT(CScintillaCtrl)
	void FireStyleNeeded(long position)
		{FireEvent(eventidStyleNeeded,EVENT_PARAM(VTS_I4),position);}
	void FireCharAdded(long ch)
		{FireEvent(eventidCharAdded,EVENT_PARAM(VTS_I4),ch);}
	void FireSavePointReached()
		{FireEvent(eventidSavePointReached,EVENT_PARAM(VTS_NONE));}
	void FireSavePointLeft()
		{FireEvent(eventidSavePointLeft,EVENT_PARAM(VTS_NONE));}
	void FireModifyAttemptRO()
		{FireEvent(eventidModifyAttemptRO,EVENT_PARAM(VTS_NONE));}
	void FireKey(long ch,long modifiers)
		{FireEvent(eventidKey,EVENT_PARAM(VTS_I4 VTS_I4),ch,modifiers);}
	void FireDoubleClick()
		{FireEvent(eventidDoubleClick,EVENT_PARAM(VTS_NONE));}
	void FireUpdateUI()
		{FireEvent(eventidUpdateUI,EVENT_PARAM(VTS_NONE));}
	void FireModified(long position,long modificationType,LPCTSTR text,long length,long linesAdded,long line,long foldLevelNow,long foldLevelPrev)
		{FireEvent(eventidModified,EVENT_PARAM(VTS_I4 VTS_I4 VTS_BSTR VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4),position,modificationType,text,length,linesAdded,line,foldLevelNow,foldLevelPrev);}
	void FireMacroRecord(long message,long wParam,long lParam)
		{FireEvent(eventidMacroRecord,EVENT_PARAM(VTS_I4 VTS_I4 VTS_I4),message,wParam,lParam);}
	void FireMarginClick(long modifiers,long position,long margin)
		{FireEvent(eventidMarginClick,EVENT_PARAM(VTS_I4 VTS_I4 VTS_I4),modifiers,position,margin);}
	void FireNeedShown(long position,long length)
		{FireEvent(eventidNeedShown,EVENT_PARAM(VTS_I4 VTS_I4),position,length);}
	void FireEN_Change()
		{FireEvent(eventidEN_Change,EVENT_PARAM(VTS_NONE));}
	//}}AFX_EVENT
	//$(DISPEVT_SECTION]
	DECLARE_EVENT_MAP()

// Dispatch and event IDs
public:
	enum {
	//$[DISPID_SECTION)
	//{{AFX_DISP_ID(CScintillaCtrl)
	dispidINVALID_POSITION = 1L,
	dispidSCI_START = 2L,
	dispidSCI_OPTIONAL_START = 3L,
	dispidSCI_LEXER_START = 4L,
	dispidSCWS_INVISIBLE = 5L,
	dispidSCWS_VISIBLEALWAYS = 6L,
	dispidSCWS_VISIBLEAFTERINDENT = 7L,
	dispidSC_EOL_CRLF = 8L,
	dispidSC_EOL_CR = 9L,
	dispidSC_EOL_LF = 10L,
	dispidSC_CP_UTF8 = 11L,
	dispidMARKER_MAX = 12L,
	dispidSC_MARK_CIRCLE = 13L,
	dispidSC_MARK_ROUNDRECT = 14L,
	dispidSC_MARK_ARROW = 15L,
	dispidSC_MARK_SMALLRECT = 16L,
	dispidSC_MARK_SHORTARROW = 17L,
	dispidSC_MARK_EMPTY = 18L,
	dispidSC_MARK_ARROWDOWN = 19L,
	dispidSC_MARK_MINUS = 20L,
	dispidSC_MARK_PLUS = 21L,
	dispidSC_MARKNUM_FOLDER = 22L,
	dispidSC_MARKNUM_FOLDEROPEN = 23L,
	dispidSC_MARGIN_SYMBOL = 24L,
	dispidSC_MARGIN_NUMBER = 25L,
	dispidSTYLE_DEFAULT = 26L,
	dispidSTYLE_LINENUMBER = 27L,
	dispidSTYLE_BRACELIGHT = 28L,
	dispidSTYLE_BRACEBAD = 29L,
	dispidSTYLE_CONTROLCHAR = 30L,
	dispidSTYLE_MAX = 31L,
	dispidSC_CHARSET_ANSI = 32L,
	dispidSC_CHARSET_DEFAULT = 33L,
	dispidSC_CHARSET_BALTIC = 34L,
	dispidSC_CHARSET_CHINESEBIG5 = 35L,
	dispidSC_CHARSET_EASTEUROPE = 36L,
	dispidSC_CHARSET_GB2312 = 37L,
	dispidSC_CHARSET_GREEK = 38L,
	dispidSC_CHARSET_HANGUL = 39L,
	dispidSC_CHARSET_MAC = 40L,
	dispidSC_CHARSET_OEM = 41L,
	dispidSC_CHARSET_RUSSIAN = 42L,
	dispidSC_CHARSET_SHIFTJIS = 43L,
	dispidSC_CHARSET_SYMBOL = 44L,
	dispidSC_CHARSET_TURKISH = 45L,
	dispidSC_CHARSET_JOHAB = 46L,
	dispidSC_CHARSET_HEBREW = 47L,
	dispidSC_CHARSET_ARABIC = 48L,
	dispidSC_CHARSET_VIETNAMESE = 49L,
	dispidSC_CHARSET_THAI = 50L,
	dispidINDIC_MAX = 51L,
	dispidINDIC_PLAIN = 52L,
	dispidINDIC_SQUIGGLE = 53L,
	dispidINDIC_TT = 54L,
	dispidINDIC_DIAGONAL = 55L,
	dispidINDIC_STRIKE = 56L,
	dispidINDIC0_MASK = 57L,
	dispidINDIC1_MASK = 58L,
	dispidINDIC2_MASK = 59L,
	dispidINDICS_MASK = 60L,
	dispidSC_PRINT_NORMAL = 61L,
	dispidSC_PRINT_INVERTLIGHT = 62L,
	dispidSC_PRINT_BLACKONWHITE = 63L,
	dispidSCFIND_MATCHCASE = 64L,
	dispidSCFIND_WHOLEWORD = 65L,
	dispidSCFIND_DOWN = 66L,
	dispidSC_FOLDLEVELBASE = 67L,
	dispidSC_FOLDLEVELWHITEFLAG = 68L,
	dispidSC_FOLDLEVELHEADERFLAG = 69L,
	dispidSC_FOLDLEVELNUMBERMASK = 70L,
	dispidEDGE_NONE = 71L,
	dispidEDGE_LINE = 72L,
	dispidEDGE_BACKGROUND = 73L,
	dispidCARET_SLOP = 74L,
	dispidCARET_CENTER = 75L,
	dispidCARET_STRICT = 76L,
	dispidSC_MOD_INSERTTEXT = 77L,
	dispidSC_MOD_DELETETEXT = 78L,
	dispidSC_MOD_CHANGESTYLE = 79L,
	dispidSC_MOD_CHANGEFOLD = 80L,
	dispidSC_PERFORMED_USER = 81L,
	dispidSC_PERFORMED_UNDO = 82L,
	dispidSC_PERFORMED_REDO = 83L,
	dispidSC_LASTSTEPINUNDOREDO = 84L,
	dispidSC_MOD_CHANGEMARKER = 85L,
	dispidSC_MOD_BEFOREINSERT = 86L,
	dispidSC_MOD_BEFOREDELETE = 87L,
	dispidSC_MODEVENTMASKALL = 88L,
	dispidSCN_STYLENEEDED = 89L,
	dispidSCN_CHARADDED = 90L,
	dispidSCN_SAVEPOINTREACHED = 91L,
	dispidSCN_SAVEPOINTLEFT = 92L,
	dispidSCN_MODIFYATTEMPTRO = 93L,
	dispidSCN_KEY = 94L,
	dispidSCN_DOUBLECLICK = 95L,
	dispidSCN_UPDATEUI = 96L,
	dispidSCN_CHECKBRACE = 97L,
	dispidSCN_MODIFIED = 98L,
	dispidSCN_MACRORECORD = 99L,
	dispidSCN_MARGINCLICK = 100L,
	dispidSCN_NEEDSHOWN = 101L,
	dispidSCEN_CHANGE = 102L,
	dispidSCEN_SETFOCUS = 103L,
	dispidSCEN_KILLFOCUS = 104L,
	dispidSCK_DOWN = 105L,
	dispidSCK_UP = 106L,
	dispidSCK_LEFT = 107L,
	dispidSCK_RIGHT = 108L,
	dispidSCK_HOME = 109L,
	dispidSCK_END = 110L,
	dispidSCK_PRIOR = 111L,
	dispidSCK_NEXT = 112L,
	dispidSCK_DELETE = 113L,
	dispidSCK_INSERT = 114L,
	dispidSCK_ESCAPE = 115L,
	dispidSCK_BACK = 116L,
	dispidSCK_TAB = 117L,
	dispidSCK_RETURN = 118L,
	dispidSCK_ADD = 119L,
	dispidSCK_SUBTRACT = 120L,
	dispidSCK_DIVIDE = 121L,
	dispidSCMOD_SHIFT = 122L,
	dispidSCMOD_CTRL = 123L,
	dispidSCMOD_ALT = 124L,
	dispidSCLEX_CONTAINER = 125L,
	dispidSCLEX_NULL = 126L,
	dispidSCLEX_PYTHON = 127L,
	dispidSCLEX_CPP = 128L,
	dispidSCLEX_HTML = 129L,
	dispidSCLEX_XML = 130L,
	dispidSCLEX_PERL = 131L,
	dispidSCLEX_SQL = 132L,
	dispidSCLEX_VB = 133L,
	dispidSCLEX_PROPERTIES = 134L,
	dispidSCLEX_ERRORLIST = 135L,
	dispidSCLEX_MAKEFILE = 136L,
	dispidSCLEX_BATCH = 137L,
	dispidSCLEX_XCODE = 138L,
	dispidSCLEX_LATEX = 139L,
	dispidSCLEX_LUA = 140L,
	dispidSCE_P_DEFAULT = 141L,
	dispidSCE_P_COMMENTLINE = 142L,
	dispidSCE_P_NUMBER = 143L,
	dispidSCE_P_STRING = 144L,
	dispidSCE_P_CHARACTER = 145L,
	dispidSCE_P_WORD = 146L,
	dispidSCE_P_TRIPLE = 147L,
	dispidSCE_P_TRIPLEDOUBLE = 148L,
	dispidSCE_P_CLASSNAME = 149L,
	dispidSCE_P_DEFNAME = 150L,
	dispidSCE_P_OPERATOR = 151L,
	dispidSCE_P_IDENTIFIER = 152L,
	dispidSCE_P_COMMENTBLOCK = 153L,
	dispidSCE_P_STRINGEOL = 154L,
	dispidSCE_C_DEFAULT = 155L,
	dispidSCE_C_COMMENT = 156L,
	dispidSCE_C_COMMENTLINE = 157L,
	dispidSCE_C_COMMENTDOC = 158L,
	dispidSCE_C_NUMBER = 159L,
	dispidSCE_C_WORD = 160L,
	dispidSCE_C_STRING = 161L,
	dispidSCE_C_CHARACTER = 162L,
	dispidSCE_C_UUID = 163L,
	dispidSCE_C_PREPROCESSOR = 164L,
	dispidSCE_C_OPERATOR = 165L,
	dispidSCE_C_IDENTIFIER = 166L,
	dispidSCE_C_STRINGEOL = 167L,
	dispidSCE_H_DEFAULT = 168L,
	dispidSCE_H_TAG = 169L,
	dispidSCE_H_TAGUNKNOWN = 170L,
	dispidSCE_H_ATTRIBUTE = 171L,
	dispidSCE_H_ATTRIBUTEUNKNOWN = 172L,
	dispidSCE_H_NUMBER = 173L,
	dispidSCE_H_DOUBLESTRING = 174L,
	dispidSCE_H_SINGLESTRING = 175L,
	dispidSCE_H_OTHER = 176L,
	dispidSCE_H_COMMENT = 177L,
	dispidSCE_H_ENTITY = 178L,
	dispidSCE_H_TAGEND = 179L,
	dispidSCE_H_XMLSTART = 180L,
	dispidSCE_H_XMLEND = 181L,
	dispidSCE_H_SCRIPT = 182L,
	dispidSCE_H_ASP = 183L,
	dispidSCE_H_ASPAT = 184L,
	dispidSCE_H_CDATA = 185L,
	dispidSCE_H_QUESTION = 186L,
	dispidSCE_HJ_START = 187L,
	dispidSCE_HJ_DEFAULT = 188L,
	dispidSCE_HJ_COMMENT = 189L,
	dispidSCE_HJ_COMMENTLINE = 190L,
	dispidSCE_HJ_COMMENTDOC = 191L,
	dispidSCE_HJ_NUMBER = 192L,
	dispidSCE_HJ_WORD = 193L,
	dispidSCE_HJ_KEYWORD = 194L,
	dispidSCE_HJ_DOUBLESTRING = 195L,
	dispidSCE_HJ_SINGLESTRING = 196L,
	dispidSCE_HJ_SYMBOLS = 197L,
	dispidSCE_HJ_STRINGEOL = 198L,
	dispidSCE_HJA_START = 199L,
	dispidSCE_HJA_DEFAULT = 200L,
	dispidSCE_HJA_COMMENT = 201L,
	dispidSCE_HJA_COMMENTLINE = 202L,
	dispidSCE_HJA_COMMENTDOC = 203L,
	dispidSCE_HJA_NUMBER = 204L,
	dispidSCE_HJA_WORD = 205L,
	dispidSCE_HJA_KEYWORD = 206L,
	dispidSCE_HJA_DOUBLESTRING = 207L,
	dispidSCE_HJA_SINGLESTRING = 208L,
	dispidSCE_HJA_SYMBOLS = 209L,
	dispidSCE_HJA_STRINGEOL = 210L,
	dispidSCE_HB_START = 211L,
	dispidSCE_HB_DEFAULT = 212L,
	dispidSCE_HB_COMMENTLINE = 213L,
	dispidSCE_HB_NUMBER = 214L,
	dispidSCE_HB_WORD = 215L,
	dispidSCE_HB_STRING = 216L,
	dispidSCE_HB_IDENTIFIER = 217L,
	dispidSCE_HB_STRINGEOL = 218L,
	dispidSCE_HBA_START = 219L,
	dispidSCE_HBA_DEFAULT = 220L,
	dispidSCE_HBA_COMMENTLINE = 221L,
	dispidSCE_HBA_NUMBER = 222L,
	dispidSCE_HBA_WORD = 223L,
	dispidSCE_HBA_STRING = 224L,
	dispidSCE_HBA_IDENTIFIER = 225L,
	dispidSCE_HBA_STRINGEOL = 226L,
	dispidSCE_HP_START = 227L,
	dispidSCE_HP_DEFAULT = 228L,
	dispidSCE_HP_COMMENTLINE = 229L,
	dispidSCE_HP_NUMBER = 230L,
	dispidSCE_HP_STRING = 231L,
	dispidSCE_HP_CHARACTER = 232L,
	dispidSCE_HP_WORD = 233L,
	dispidSCE_HP_TRIPLE = 234L,
	dispidSCE_HP_TRIPLEDOUBLE = 235L,
	dispidSCE_HP_CLASSNAME = 236L,
	dispidSCE_HP_DEFNAME = 237L,
	dispidSCE_HP_OPERATOR = 238L,
	dispidSCE_HP_IDENTIFIER = 239L,
	dispidSCE_HPA_START = 240L,
	dispidSCE_HPA_DEFAULT = 241L,
	dispidSCE_HPA_COMMENTLINE = 242L,
	dispidSCE_HPA_NUMBER = 243L,
	dispidSCE_HPA_STRING = 244L,
	dispidSCE_HPA_CHARACTER = 245L,
	dispidSCE_HPA_WORD = 246L,
	dispidSCE_HPA_TRIPLE = 247L,
	dispidSCE_HPA_TRIPLEDOUBLE = 248L,
	dispidSCE_HPA_CLASSNAME = 249L,
	dispidSCE_HPA_DEFNAME = 250L,
	dispidSCE_HPA_OPERATOR = 251L,
	dispidSCE_HPA_IDENTIFIER = 252L,
	dispidSCE_HPHP_DEFAULT = 253L,
	dispidSCE_HPHP_HSTRING = 254L,
	dispidSCE_HPHP_SIMPLESTRING = 255L,
	dispidSCE_HPHP_WORD = 256L,
	dispidSCE_HPHP_NUMBER = 257L,
	dispidSCE_HPHP_VARIABLE = 258L,
	dispidSCE_HPHP_COMMENT = 259L,
	dispidSCE_HPHP_COMMENTLINE = 260L,
	dispidSCE_HPHP_STRINGEOL = 261L,
	dispidSCE_PL_DEFAULT = 262L,
	dispidSCE_PL_HERE = 263L,
	dispidSCE_PL_COMMENTLINE = 264L,
	dispidSCE_PL_POD = 265L,
	dispidSCE_PL_NUMBER = 266L,
	dispidSCE_PL_WORD = 267L,
	dispidSCE_PL_STRING = 268L,
	dispidSCE_PL_CHARACTER = 269L,
	dispidSCE_PL_PUNCTUATION = 270L,
	dispidSCE_PL_PREPROCESSOR = 271L,
	dispidSCE_PL_OPERATOR = 272L,
	dispidSCE_PL_IDENTIFIER = 273L,
	dispidSCE_PL_SCALAR = 274L,
	dispidSCE_PL_ARRAY = 275L,
	dispidSCE_PL_HASH = 276L,
	dispidSCE_PL_SYMBOLTABLE = 277L,
	dispidSCE_PL_REF = 278L,
	dispidSCE_PL_REGEX = 279L,
	dispidSCE_PL_REGSUBST = 280L,
	dispidSCE_PL_LONGQUOTE = 281L,
	dispidSCE_PL_BACKTICKS = 282L,
	dispidSCE_PL_DATASECTION = 283L,
	dispidSCE_L_DEFAULT = 284L,
	dispidSCE_L_COMMAND = 285L,
	dispidSCE_L_TAG = 286L,
	dispidSCE_L_MATH = 287L,
	dispidSCE_L_COMMENT = 288L,
	dispidSCE_LUA_DEFAULT = 289L,
	dispidSCE_LUA_COMMENT = 290L,
	dispidSCE_LUA_COMMENTLINE = 291L,
	dispidSCE_LUA_COMMENTDOC = 292L,
	dispidSCE_LUA_NUMBER = 293L,
	dispidSCE_LUA_WORD = 294L,
	dispidSCE_LUA_STRING = 295L,
	dispidSCE_LUA_CHARACTER = 296L,
	dispidSCE_LUA_LITERALSTRING = 297L,
	dispidSCE_LUA_PREPROCESSOR = 298L,
	dispidSCE_LUA_OPERATOR = 299L,
	dispidSCE_LUA_IDENTIFIER = 300L,
	dispidSCE_LUA_STRINGEOL = 301L,
	dispidEN_CHANGE = 302L,
	dispidEN_KILLFOCUS = 303L,
	dispidEN_SETFOCUS = 304L,
	dispidEC_LEFTMARGIN = 305L,
	dispidEC_RIGHTMARGIN = 306L,
	dispidEC_USEFONTINFO = 307L,
	dispidSEL_EMPTY = 308L,
	dispidSEL_TEXT = 309L,
	dispidFR_MATCHCASE = 310L,
	dispidFR_WHOLEWORD = 311L,
	dispidFR_DOWN = 312L,
	dispidSHIFT_PRESSED = 313L,
	dispidLEFT_CTRL_PRESSED = 314L,
	dispidLEFT_ALT_PRESSED = 315L,
	dispidAddText = 316L,
	dispidAddStyledText = 317L,
	dispidInsertText = 318L,
	dispidClearAll = 319L,
	dispidClearDocumentStyle = 320L,
	dispidGetLength = 321L,
	dispidGetCharAt = 322L,
	dispidGetCurrentPos = 323L,
	dispidGetAnchor = 324L,
	dispidGetStyleAt = 325L,
	dispidRedo = 326L,
	dispidSetUndoCollection = 327L,
	dispidSelectAll = 328L,
	dispidSetSavePoint = 329L,
	dispidGetStyledText = 330L,
	dispidCanRedo = 331L,
	dispidMarkerLineFromHandle = 332L,
	dispidMarkerDeleteHandle = 333L,
	dispidGetUndoCollection = 334L,
	dispidGetViewWS = 335L,
	dispidSetViewWS = 336L,
	dispidPositionFromPoint = 337L,
	dispidGotoLine = 338L,
	dispidGotoPos = 339L,
	dispidSetAnchor = 340L,
	dispidGetCurLine = 341L,
	dispidGetEndStyled = 342L,
	dispidConvertEOLs = 343L,
	dispidGetEOLMode = 344L,
	dispidSetEOLMode = 345L,
	dispidStartStyling = 346L,
	dispidSetStyling = 347L,
	dispidGetBufferedDraw = 348L,
	dispidSetBufferedDraw = 349L,
	dispidSetTabWidth = 350L,
	dispidGetTabWidth = 351L,
	dispidSetCodePage = 352L,
	dispidSetUsePalette = 353L,
	dispidMarkerDefine = 354L,
	dispidMarkerSetFore = 355L,
	dispidMarkerSetBack = 356L,
	dispidMarkerAdd = 357L,
	dispidMarkerDelete = 358L,
	dispidMarkerDeleteAll = 359L,
	dispidMarkerGet = 360L,
	dispidMarkerNext = 361L,
	dispidMarkerPrevious = 362L,
	dispidSetMarginTypeN = 363L,
	dispidGetMarginTypeN = 364L,
	dispidSetMarginWidthN = 365L,
	dispidGetMarginWidthN = 366L,
	dispidSetMarginMaskN = 367L,
	dispidGetMarginMaskN = 368L,
	dispidSetMarginSensitiveN = 369L,
	dispidGetMarginSensitiveN = 370L,
	dispidStyleClearAll = 371L,
	dispidStyleSetFore = 372L,
	dispidStyleSetBack = 373L,
	dispidStyleSetBold = 374L,
	dispidStyleSetItalic = 375L,
	dispidStyleSetSize = 376L,
	dispidStyleSetFont = 377L,
	dispidStyleSetEOLFilled = 378L,
	dispidStyleResetDefault = 379L,
	dispidStyleSetUnderline = 380L,
	dispidStyleSetCharacterSet = 381L,
	dispidSetSelFore = 382L,
	dispidSetSelBack = 383L,
	dispidSetCaretFore = 384L,
	dispidAssignCmdKey = 385L,
	dispidClearCmdKey = 386L,
	dispidClearAllCmdKeys = 387L,
	dispidSetStylingEx = 388L,
	dispidGetCaretPeriod = 389L,
	dispidSetCaretPeriod = 390L,
	dispidSetWordChars = 391L,
	dispidBeginUndoAction = 392L,
	dispidEndUndoAction = 393L,
	dispidIndicSetStyle = 394L,
	dispidIndicGetStyle = 395L,
	dispidIndicSetFore = 396L,
	dispidIndicGetFore = 397L,
	dispidSetStyleBits = 398L,
	dispidGetStyleBits = 399L,
	dispidSetLineState = 400L,
	dispidGetLineState = 401L,
	dispidGetMaxLineState = 402L,
	dispidAutoCShow = 403L,
	dispidAutoCCancel = 404L,
	dispidAutoCActive = 405L,
	dispidAutoCPosStart = 406L,
	dispidAutoCComplete = 407L,
	dispidAutoCStops = 408L,
	dispidAutoCSetSeparator = 409L,
	dispidAutoCGetSeparator = 410L,
	dispidAutoCSelect = 411L,
	dispidAutoCSetCancelAtStart = 412L,
	dispidAutoCGetCancelAtStart = 413L,
	dispidSetIndent = 414L,
	dispidGetIndent = 415L,
	dispidSetUseTabs = 416L,
	dispidGetUseTabs = 417L,
	dispidSetLineIndentation = 418L,
	dispidGetLineIndentation = 419L,
	dispidGetLineIndentPosition = 420L,
	dispidGetColumn = 421L,
	dispidSetHScrollBar = 422L,
	dispidGetHScrollBar = 423L,
	dispidSetIndentationGuides = 424L,
	dispidGetIndentationGuides = 425L,
	dispidSetHighlightGuide = 426L,
	dispidGetHighlightGuide = 427L,
	dispidGetLineEndPosition = 428L,
	dispidGetCodePage = 429L,
	dispidGetCaretFore = 430L,
	dispidGetUsePalette = 431L,
	dispidGetReadOnly = 432L,
	dispidSetCurrentPos = 433L,
	dispidSetSelectionStart = 434L,
	dispidGetSelectionStart = 435L,
	dispidSetSelectionEnd = 436L,
	dispidGetSelectionEnd = 437L,
	dispidSetPrintMagnification = 438L,
	dispidGetPrintMagnification = 439L,
	dispidSetPrintColourMode = 440L,
	dispidGetPrintColourMode = 441L,
	dispidFindText = 442L,
	dispidFormatRange = 443L,
	dispidGetFirstVisibleLine = 444L,
	dispidGetLine = 445L,
	dispidGetLineCount = 446L,
	dispidSetMarginLeft = 447L,
	dispidGetMarginLeft = 448L,
	dispidSetMarginRight = 449L,
	dispidGetMarginRight = 450L,
	dispidGetModify = 451L,
	dispidSetSel = 452L,
	dispidGetSelText = 453L,
	dispidGetTextRange = 454L,
	dispidHideSelection = 455L,
	dispidPointXFromPosition = 456L,
	dispidPointYFromPosition = 457L,
	dispidLineFromPosition = 458L,
	dispidPositionFromLine = 459L,
	dispidLineScroll = 460L,
	dispidScrollCaret = 461L,
	dispidReplaceSel = 462L,
	dispidSetReadOnly = 463L,
	dispidNull = 464L,
	dispidCanPaste = 465L,
	dispidCanUndo = 466L,
	dispidEmptyUndoBuffer = 467L,
	dispidUndo = 468L,
	dispidCut = 469L,
	dispidCopy = 470L,
	dispidPaste = 471L,
	dispidClear = 472L,
	dispidSetText = 473L,
	dispidGetText = 474L,
	dispidGetTextLength = 475L,
	dispidGetDirectFunction = 476L,
	dispidGetDirectPointer = 477L,
	dispidCallTipShow = 478L,
	dispidCallTipCancel = 479L,
	dispidCallTipActive = 480L,
	dispidCallTipPosStart = 481L,
	dispidCallTipSetHlt = 482L,
	dispidCallTipSetBack = 483L,
	dispidVisibleFromDocLine = 484L,
	dispidDocLineFromVisible = 485L,
	dispidSetFoldLevel = 486L,
	dispidGetFoldLevel = 487L,
	dispidGetLastChild = 488L,
	dispidGetFoldParent = 489L,
	dispidShowLines = 490L,
	dispidHideLines = 491L,
	dispidGetLineVisible = 492L,
	dispidSetFoldExpanded = 493L,
	dispidGetFoldExpanded = 494L,
	dispidToggleFold = 495L,
	dispidEnsureVisible = 496L,
	dispidSetFoldFlags = 497L,
	dispidLineDown = 498L,
	dispidLineDownExtend = 499L,
	dispidLineUp = 500L,
	dispidLineUpExtend = 501L,
	dispidCharLeft = 502L,
	dispidCharLeftExtend = 503L,
	dispidCharRight = 504L,
	dispidCharRightExtend = 505L,
	dispidWordLeft = 506L,
	dispidWordLeftExtend = 507L,
	dispidWordRight = 508L,
	dispidWordRightExtend = 509L,
	dispidHome = 510L,
	dispidHomeExtend = 511L,
	dispidLineEnd = 512L,
	dispidLineEndExtend = 513L,
	dispidDocumentStart = 514L,
	dispidDocumentStartExtend = 515L,
	dispidDocumentEnd = 516L,
	dispidDocumentEndExtend = 517L,
	dispidPageUp = 518L,
	dispidPageUpExtend = 519L,
	dispidPageDown = 520L,
	dispidPageDownExtend = 521L,
	dispidEditToggleOvertype = 522L,
	dispidCancel = 523L,
	dispidDeleteBack = 524L,
	dispidTab = 525L,
	dispidBackTab = 526L,
	dispidNewLine = 527L,
	dispidFormFeed = 528L,
	dispidVCHome = 529L,
	dispidVCHomeExtend = 530L,
	dispidZoomIn = 531L,
	dispidZoomOut = 532L,
	dispidDelWordLeft = 533L,
	dispidDelWordRight = 534L,
	dispidLineCut = 535L,
	dispidLineDelete = 536L,
	dispidLineTranspose = 537L,
	dispidLowerCase = 538L,
	dispidUpperCase = 539L,
	dispidLineScrollDown = 540L,
	dispidLineScrollUp = 541L,
	dispidLineLength = 542L,
	dispidBraceHighlight = 543L,
	dispidBraceBadLight = 544L,
	dispidBraceMatch = 545L,
	dispidGetViewEOL = 546L,
	dispidSetViewEOL = 547L,
	dispidGetDocPointer = 548L,
	dispidSetDocPointer = 549L,
	dispidSetModEventMask = 550L,
	dispidGetEdgeColumn = 551L,
	dispidSetEdgeColumn = 552L,
	dispidGetEdgeMode = 553L,
	dispidSetEdgeMode = 554L,
	dispidGetEdgeColour = 555L,
	dispidSetEdgeColour = 556L,
	dispidSearchAnchor = 557L,
	dispidSearchNext = 558L,
	dispidSearchPrev = 559L,
	dispidSetCaretPolicy = 560L,
	dispidLinesOnScreen = 561L,
	dispidUsePopUp = 562L,
	dispidSelectionIsRectangle = 563L,
	dispidSetZoom = 564L,
	dispidGetZoom = 565L,
	dispidCreateDocument = 566L,
	dispidAddRefDocument = 567L,
	dispidReleaseDocument = 568L,
	dispidGetModEventMask = 569L,
	dispidGrabFocus = 570L,
	dispidStartRecord = 571L,
	dispidStopRecord = 572L,
	dispidSetLexer = 573L,
	dispidGetLexer = 574L,
	dispidColourise = 575L,
	dispidSetProperty = 576L,
	dispidSetKeyWords = 577L,
	dispidEM_CanPaste = 578L,
	dispidEM_CanUndo = 579L,
	dispidEM_CharFromPos = 580L,
	dispidEM_EmptyUndoBuffer = 581L,
	dispidEM_ExGetSel = 582L,
	dispidEM_ExLineFromChar = 583L,
	dispidEM_ExSetSel = 584L,
	dispidEM_FindText = 585L,
	dispidEM_FindTextEx = 586L,
	dispidEM_FormatRange = 587L,
	dispidEM_GetFirstVisibleLine = 588L,
	dispidEM_GetLine = 589L,
	dispidEM_GetLineCount = 590L,
	dispidEM_GetMargins = 591L,
	dispidEM_GetModify = 592L,
	dispidEM_GetRect = 593L,
	dispidEM_GetSel = 594L,
	dispidEM_GetSelText = 595L,
	dispidEM_GetTextRange = 596L,
	dispidEM_HideSelection = 597L,
	dispidEM_LineFromChar = 598L,
	dispidEM_LineIndex = 599L,
	dispidEM_LineLength = 600L,
	dispidEM_LineScroll = 601L,
	dispidEM_PosFromChar = 602L,
	dispidEM_ReplaceSel = 603L,
	dispidEM_ScrollCaret = 604L,
	dispidEM_SelectionType = 605L,
	dispidEM_SetMargins = 606L,
	dispidEM_SetReadOnly = 607L,
	dispidEM_SetSel = 608L,
	dispidEM_Undo = 609L,
	dispidWM_Null = 610L,
	dispidWM_Clear = 611L,
	dispidWM_Command = 612L,
	dispidWM_Copy = 613L,
	dispidWM_Cut = 614L,
	dispidWM_GetText = 615L,
	dispidWM_GetTextLength = 616L,
	dispidWM_Notify = 617L,
	dispidWM_Paste = 618L,
	dispidWM_SetText = 619L,
	dispidWM_Undo = 620L,
	eventidStyleNeeded = 1L,
	eventidCharAdded = 2L,
	eventidSavePointReached = 3L,
	eventidSavePointLeft = 4L,
	eventidModifyAttemptRO = 5L,
	eventidKey = 6L,
	eventidDoubleClick = 7L,
	eventidUpdateUI = 8L,
	eventidModified = 9L,
	eventidMacroRecord = 10L,
	eventidMarginClick = 11L,
	eventidNeedShown = 12L,
	eventidEN_Change = 13L,
	//}}AFX_DISP_ID
	//$(DISPID_SECTION]
	};
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCINTILLACTRL_H__4A2DFF6E_C146_11D3_9CDF_482471000000__INCLUDED)
