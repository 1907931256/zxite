// ScintillaCtrl.cpp : Implementation of the CScintillaCtrl ActiveX Control class.

#include "stdafx.h"
#include "..\scintilla\include\Scintilla.h"
#include "..\scintilla\include\SciLexer.h"
#include "ScintillaApp.h"
#include "ScintillaCtrl.h"
#include "ScintillaPpg.h"
#include <memory>
#include <ctype.h>
#include <time.h>

#define SCFIND_DOWN 0
#define SCN_CHECKBRACE 0
#define SCE_HPHP_STRINGEOL 0

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CScintillaCtrl, COleControl)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CScintillaCtrl, COleControl)
	//{{AFX_MSG_MAP(CScintillaCtrl)
	ON_MESSAGE(OCM_NOTIFY, OnOcmNotify)
	//}}AFX_MSG_MAP

	ON_OLEVERB(AFX_IDS_VERB_EDIT, OnEdit)
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Dispatch map

BEGIN_DISPATCH_MAP(CScintillaCtrl, COleControl)
	//$[DISPMAP_SECTION)
	//{{AFX_DISPATCH_MAP(CScintillaCtrl)
	DISP_PROPERTY_EX(CScintillaCtrl, "INVALID_POSITION", GetINVALID_POSITION, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCI_START", GetSCI_START, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCI_OPTIONAL_START", GetSCI_OPTIONAL_START, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCI_LEXER_START", GetSCI_LEXER_START, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCWS_INVISIBLE", GetSCWS_INVISIBLE, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCWS_VISIBLEALWAYS", GetSCWS_VISIBLEALWAYS, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCWS_VISIBLEAFTERINDENT", GetSCWS_VISIBLEAFTERINDENT, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SC_EOL_CRLF", GetSC_EOL_CRLF, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SC_EOL_CR", GetSC_EOL_CR, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SC_EOL_LF", GetSC_EOL_LF, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SC_CP_UTF8", GetSC_CP_UTF8, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "MARKER_MAX", GetMARKER_MAX, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SC_MARK_CIRCLE", GetSC_MARK_CIRCLE, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SC_MARK_ROUNDRECT", GetSC_MARK_ROUNDRECT, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SC_MARK_ARROW", GetSC_MARK_ARROW, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SC_MARK_SMALLRECT", GetSC_MARK_SMALLRECT, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SC_MARK_SHORTARROW", GetSC_MARK_SHORTARROW, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SC_MARK_EMPTY", GetSC_MARK_EMPTY, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SC_MARK_ARROWDOWN", GetSC_MARK_ARROWDOWN, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SC_MARK_MINUS", GetSC_MARK_MINUS, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SC_MARK_PLUS", GetSC_MARK_PLUS, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SC_MARKNUM_FOLDER", GetSC_MARKNUM_FOLDER, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SC_MARKNUM_FOLDEROPEN", GetSC_MARKNUM_FOLDEROPEN, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SC_MARGIN_SYMBOL", GetSC_MARGIN_SYMBOL, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SC_MARGIN_NUMBER", GetSC_MARGIN_NUMBER, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "STYLE_DEFAULT", GetSTYLE_DEFAULT, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "STYLE_LINENUMBER", GetSTYLE_LINENUMBER, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "STYLE_BRACELIGHT", GetSTYLE_BRACELIGHT, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "STYLE_BRACEBAD", GetSTYLE_BRACEBAD, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "STYLE_CONTROLCHAR", GetSTYLE_CONTROLCHAR, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "STYLE_MAX", GetSTYLE_MAX, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SC_CHARSET_ANSI", GetSC_CHARSET_ANSI, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SC_CHARSET_DEFAULT", GetSC_CHARSET_DEFAULT, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SC_CHARSET_BALTIC", GetSC_CHARSET_BALTIC, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SC_CHARSET_CHINESEBIG5", GetSC_CHARSET_CHINESEBIG5, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SC_CHARSET_EASTEUROPE", GetSC_CHARSET_EASTEUROPE, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SC_CHARSET_GB2312", GetSC_CHARSET_GB2312, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SC_CHARSET_GREEK", GetSC_CHARSET_GREEK, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SC_CHARSET_HANGUL", GetSC_CHARSET_HANGUL, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SC_CHARSET_MAC", GetSC_CHARSET_MAC, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SC_CHARSET_OEM", GetSC_CHARSET_OEM, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SC_CHARSET_RUSSIAN", GetSC_CHARSET_RUSSIAN, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SC_CHARSET_SHIFTJIS", GetSC_CHARSET_SHIFTJIS, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SC_CHARSET_SYMBOL", GetSC_CHARSET_SYMBOL, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SC_CHARSET_TURKISH", GetSC_CHARSET_TURKISH, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SC_CHARSET_JOHAB", GetSC_CHARSET_JOHAB, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SC_CHARSET_HEBREW", GetSC_CHARSET_HEBREW, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SC_CHARSET_ARABIC", GetSC_CHARSET_ARABIC, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SC_CHARSET_VIETNAMESE", GetSC_CHARSET_VIETNAMESE, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SC_CHARSET_THAI", GetSC_CHARSET_THAI, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "INDIC_MAX", GetINDIC_MAX, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "INDIC_PLAIN", GetINDIC_PLAIN, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "INDIC_SQUIGGLE", GetINDIC_SQUIGGLE, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "INDIC_TT", GetINDIC_TT, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "INDIC_DIAGONAL", GetINDIC_DIAGONAL, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "INDIC_STRIKE", GetINDIC_STRIKE, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "INDIC0_MASK", GetINDIC0_MASK, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "INDIC1_MASK", GetINDIC1_MASK, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "INDIC2_MASK", GetINDIC2_MASK, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "INDICS_MASK", GetINDICS_MASK, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SC_PRINT_NORMAL", GetSC_PRINT_NORMAL, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SC_PRINT_INVERTLIGHT", GetSC_PRINT_INVERTLIGHT, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SC_PRINT_BLACKONWHITE", GetSC_PRINT_BLACKONWHITE, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCFIND_MATCHCASE", GetSCFIND_MATCHCASE, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCFIND_WHOLEWORD", GetSCFIND_WHOLEWORD, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCFIND_DOWN", GetSCFIND_DOWN, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SC_FOLDLEVELBASE", GetSC_FOLDLEVELBASE, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SC_FOLDLEVELWHITEFLAG", GetSC_FOLDLEVELWHITEFLAG, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SC_FOLDLEVELHEADERFLAG", GetSC_FOLDLEVELHEADERFLAG, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SC_FOLDLEVELNUMBERMASK", GetSC_FOLDLEVELNUMBERMASK, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "EDGE_NONE", GetEDGE_NONE, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "EDGE_LINE", GetEDGE_LINE, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "EDGE_BACKGROUND", GetEDGE_BACKGROUND, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "CARET_SLOP", GetCARET_SLOP, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "CARET_CENTER", GetCARET_CENTER, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "CARET_STRICT", GetCARET_STRICT, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SC_MOD_INSERTTEXT", GetSC_MOD_INSERTTEXT, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SC_MOD_DELETETEXT", GetSC_MOD_DELETETEXT, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SC_MOD_CHANGESTYLE", GetSC_MOD_CHANGESTYLE, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SC_MOD_CHANGEFOLD", GetSC_MOD_CHANGEFOLD, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SC_PERFORMED_USER", GetSC_PERFORMED_USER, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SC_PERFORMED_UNDO", GetSC_PERFORMED_UNDO, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SC_PERFORMED_REDO", GetSC_PERFORMED_REDO, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SC_LASTSTEPINUNDOREDO", GetSC_LASTSTEPINUNDOREDO, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SC_MOD_CHANGEMARKER", GetSC_MOD_CHANGEMARKER, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SC_MOD_BEFOREINSERT", GetSC_MOD_BEFOREINSERT, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SC_MOD_BEFOREDELETE", GetSC_MOD_BEFOREDELETE, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SC_MODEVENTMASKALL", GetSC_MODEVENTMASKALL, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCN_STYLENEEDED", GetSCN_STYLENEEDED, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCN_CHARADDED", GetSCN_CHARADDED, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCN_SAVEPOINTREACHED", GetSCN_SAVEPOINTREACHED, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCN_SAVEPOINTLEFT", GetSCN_SAVEPOINTLEFT, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCN_MODIFYATTEMPTRO", GetSCN_MODIFYATTEMPTRO, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCN_KEY", GetSCN_KEY, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCN_DOUBLECLICK", GetSCN_DOUBLECLICK, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCN_UPDATEUI", GetSCN_UPDATEUI, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCN_CHECKBRACE", GetSCN_CHECKBRACE, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCN_MODIFIED", GetSCN_MODIFIED, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCN_MACRORECORD", GetSCN_MACRORECORD, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCN_MARGINCLICK", GetSCN_MARGINCLICK, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCN_NEEDSHOWN", GetSCN_NEEDSHOWN, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCEN_CHANGE", GetSCEN_CHANGE, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCEN_SETFOCUS", GetSCEN_SETFOCUS, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCEN_KILLFOCUS", GetSCEN_KILLFOCUS, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCK_DOWN", GetSCK_DOWN, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCK_UP", GetSCK_UP, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCK_LEFT", GetSCK_LEFT, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCK_RIGHT", GetSCK_RIGHT, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCK_HOME", GetSCK_HOME, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCK_END", GetSCK_END, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCK_PRIOR", GetSCK_PRIOR, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCK_NEXT", GetSCK_NEXT, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCK_DELETE", GetSCK_DELETE, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCK_INSERT", GetSCK_INSERT, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCK_ESCAPE", GetSCK_ESCAPE, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCK_BACK", GetSCK_BACK, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCK_TAB", GetSCK_TAB, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCK_RETURN", GetSCK_RETURN, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCK_ADD", GetSCK_ADD, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCK_SUBTRACT", GetSCK_SUBTRACT, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCK_DIVIDE", GetSCK_DIVIDE, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCMOD_SHIFT", GetSCMOD_SHIFT, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCMOD_CTRL", GetSCMOD_CTRL, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCMOD_ALT", GetSCMOD_ALT, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCLEX_CONTAINER", GetSCLEX_CONTAINER, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCLEX_NULL", GetSCLEX_NULL, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCLEX_PYTHON", GetSCLEX_PYTHON, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCLEX_CPP", GetSCLEX_CPP, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCLEX_HTML", GetSCLEX_HTML, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCLEX_XML", GetSCLEX_XML, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCLEX_PERL", GetSCLEX_PERL, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCLEX_SQL", GetSCLEX_SQL, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCLEX_VB", GetSCLEX_VB, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCLEX_PROPERTIES", GetSCLEX_PROPERTIES, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCLEX_ERRORLIST", GetSCLEX_ERRORLIST, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCLEX_MAKEFILE", GetSCLEX_MAKEFILE, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCLEX_BATCH", GetSCLEX_BATCH, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCLEX_XCODE", GetSCLEX_XCODE, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCLEX_LATEX", GetSCLEX_LATEX, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCLEX_LUA", GetSCLEX_LUA, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_P_DEFAULT", GetSCE_P_DEFAULT, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_P_COMMENTLINE", GetSCE_P_COMMENTLINE, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_P_NUMBER", GetSCE_P_NUMBER, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_P_STRING", GetSCE_P_STRING, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_P_CHARACTER", GetSCE_P_CHARACTER, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_P_WORD", GetSCE_P_WORD, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_P_TRIPLE", GetSCE_P_TRIPLE, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_P_TRIPLEDOUBLE", GetSCE_P_TRIPLEDOUBLE, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_P_CLASSNAME", GetSCE_P_CLASSNAME, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_P_DEFNAME", GetSCE_P_DEFNAME, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_P_OPERATOR", GetSCE_P_OPERATOR, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_P_IDENTIFIER", GetSCE_P_IDENTIFIER, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_P_COMMENTBLOCK", GetSCE_P_COMMENTBLOCK, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_P_STRINGEOL", GetSCE_P_STRINGEOL, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_C_DEFAULT", GetSCE_C_DEFAULT, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_C_COMMENT", GetSCE_C_COMMENT, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_C_COMMENTLINE", GetSCE_C_COMMENTLINE, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_C_COMMENTDOC", GetSCE_C_COMMENTDOC, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_C_NUMBER", GetSCE_C_NUMBER, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_C_WORD", GetSCE_C_WORD, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_C_STRING", GetSCE_C_STRING, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_C_CHARACTER", GetSCE_C_CHARACTER, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_C_UUID", GetSCE_C_UUID, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_C_PREPROCESSOR", GetSCE_C_PREPROCESSOR, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_C_OPERATOR", GetSCE_C_OPERATOR, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_C_IDENTIFIER", GetSCE_C_IDENTIFIER, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_C_STRINGEOL", GetSCE_C_STRINGEOL, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_H_DEFAULT", GetSCE_H_DEFAULT, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_H_TAG", GetSCE_H_TAG, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_H_TAGUNKNOWN", GetSCE_H_TAGUNKNOWN, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_H_ATTRIBUTE", GetSCE_H_ATTRIBUTE, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_H_ATTRIBUTEUNKNOWN", GetSCE_H_ATTRIBUTEUNKNOWN, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_H_NUMBER", GetSCE_H_NUMBER, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_H_DOUBLESTRING", GetSCE_H_DOUBLESTRING, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_H_SINGLESTRING", GetSCE_H_SINGLESTRING, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_H_OTHER", GetSCE_H_OTHER, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_H_COMMENT", GetSCE_H_COMMENT, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_H_ENTITY", GetSCE_H_ENTITY, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_H_TAGEND", GetSCE_H_TAGEND, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_H_XMLSTART", GetSCE_H_XMLSTART, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_H_XMLEND", GetSCE_H_XMLEND, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_H_SCRIPT", GetSCE_H_SCRIPT, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_H_ASP", GetSCE_H_ASP, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_H_ASPAT", GetSCE_H_ASPAT, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_H_CDATA", GetSCE_H_CDATA, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_H_QUESTION", GetSCE_H_QUESTION, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_HJ_START", GetSCE_HJ_START, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_HJ_DEFAULT", GetSCE_HJ_DEFAULT, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_HJ_COMMENT", GetSCE_HJ_COMMENT, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_HJ_COMMENTLINE", GetSCE_HJ_COMMENTLINE, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_HJ_COMMENTDOC", GetSCE_HJ_COMMENTDOC, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_HJ_NUMBER", GetSCE_HJ_NUMBER, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_HJ_WORD", GetSCE_HJ_WORD, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_HJ_KEYWORD", GetSCE_HJ_KEYWORD, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_HJ_DOUBLESTRING", GetSCE_HJ_DOUBLESTRING, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_HJ_SINGLESTRING", GetSCE_HJ_SINGLESTRING, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_HJ_SYMBOLS", GetSCE_HJ_SYMBOLS, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_HJ_STRINGEOL", GetSCE_HJ_STRINGEOL, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_HJA_START", GetSCE_HJA_START, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_HJA_DEFAULT", GetSCE_HJA_DEFAULT, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_HJA_COMMENT", GetSCE_HJA_COMMENT, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_HJA_COMMENTLINE", GetSCE_HJA_COMMENTLINE, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_HJA_COMMENTDOC", GetSCE_HJA_COMMENTDOC, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_HJA_NUMBER", GetSCE_HJA_NUMBER, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_HJA_WORD", GetSCE_HJA_WORD, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_HJA_KEYWORD", GetSCE_HJA_KEYWORD, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_HJA_DOUBLESTRING", GetSCE_HJA_DOUBLESTRING, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_HJA_SINGLESTRING", GetSCE_HJA_SINGLESTRING, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_HJA_SYMBOLS", GetSCE_HJA_SYMBOLS, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_HJA_STRINGEOL", GetSCE_HJA_STRINGEOL, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_HB_START", GetSCE_HB_START, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_HB_DEFAULT", GetSCE_HB_DEFAULT, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_HB_COMMENTLINE", GetSCE_HB_COMMENTLINE, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_HB_NUMBER", GetSCE_HB_NUMBER, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_HB_WORD", GetSCE_HB_WORD, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_HB_STRING", GetSCE_HB_STRING, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_HB_IDENTIFIER", GetSCE_HB_IDENTIFIER, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_HB_STRINGEOL", GetSCE_HB_STRINGEOL, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_HBA_START", GetSCE_HBA_START, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_HBA_DEFAULT", GetSCE_HBA_DEFAULT, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_HBA_COMMENTLINE", GetSCE_HBA_COMMENTLINE, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_HBA_NUMBER", GetSCE_HBA_NUMBER, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_HBA_WORD", GetSCE_HBA_WORD, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_HBA_STRING", GetSCE_HBA_STRING, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_HBA_IDENTIFIER", GetSCE_HBA_IDENTIFIER, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_HBA_STRINGEOL", GetSCE_HBA_STRINGEOL, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_HP_START", GetSCE_HP_START, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_HP_DEFAULT", GetSCE_HP_DEFAULT, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_HP_COMMENTLINE", GetSCE_HP_COMMENTLINE, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_HP_NUMBER", GetSCE_HP_NUMBER, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_HP_STRING", GetSCE_HP_STRING, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_HP_CHARACTER", GetSCE_HP_CHARACTER, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_HP_WORD", GetSCE_HP_WORD, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_HP_TRIPLE", GetSCE_HP_TRIPLE, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_HP_TRIPLEDOUBLE", GetSCE_HP_TRIPLEDOUBLE, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_HP_CLASSNAME", GetSCE_HP_CLASSNAME, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_HP_DEFNAME", GetSCE_HP_DEFNAME, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_HP_OPERATOR", GetSCE_HP_OPERATOR, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_HP_IDENTIFIER", GetSCE_HP_IDENTIFIER, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_HPA_START", GetSCE_HPA_START, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_HPA_DEFAULT", GetSCE_HPA_DEFAULT, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_HPA_COMMENTLINE", GetSCE_HPA_COMMENTLINE, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_HPA_NUMBER", GetSCE_HPA_NUMBER, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_HPA_STRING", GetSCE_HPA_STRING, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_HPA_CHARACTER", GetSCE_HPA_CHARACTER, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_HPA_WORD", GetSCE_HPA_WORD, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_HPA_TRIPLE", GetSCE_HPA_TRIPLE, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_HPA_TRIPLEDOUBLE", GetSCE_HPA_TRIPLEDOUBLE, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_HPA_CLASSNAME", GetSCE_HPA_CLASSNAME, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_HPA_DEFNAME", GetSCE_HPA_DEFNAME, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_HPA_OPERATOR", GetSCE_HPA_OPERATOR, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_HPA_IDENTIFIER", GetSCE_HPA_IDENTIFIER, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_HPHP_DEFAULT", GetSCE_HPHP_DEFAULT, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_HPHP_HSTRING", GetSCE_HPHP_HSTRING, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_HPHP_SIMPLESTRING", GetSCE_HPHP_SIMPLESTRING, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_HPHP_WORD", GetSCE_HPHP_WORD, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_HPHP_NUMBER", GetSCE_HPHP_NUMBER, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_HPHP_VARIABLE", GetSCE_HPHP_VARIABLE, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_HPHP_COMMENT", GetSCE_HPHP_COMMENT, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_HPHP_COMMENTLINE", GetSCE_HPHP_COMMENTLINE, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_HPHP_STRINGEOL", GetSCE_HPHP_STRINGEOL, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_PL_DEFAULT", GetSCE_PL_DEFAULT, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_PL_HERE", GetSCE_PL_HERE, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_PL_COMMENTLINE", GetSCE_PL_COMMENTLINE, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_PL_POD", GetSCE_PL_POD, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_PL_NUMBER", GetSCE_PL_NUMBER, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_PL_WORD", GetSCE_PL_WORD, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_PL_STRING", GetSCE_PL_STRING, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_PL_CHARACTER", GetSCE_PL_CHARACTER, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_PL_PUNCTUATION", GetSCE_PL_PUNCTUATION, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_PL_PREPROCESSOR", GetSCE_PL_PREPROCESSOR, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_PL_OPERATOR", GetSCE_PL_OPERATOR, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_PL_IDENTIFIER", GetSCE_PL_IDENTIFIER, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_PL_SCALAR", GetSCE_PL_SCALAR, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_PL_ARRAY", GetSCE_PL_ARRAY, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_PL_HASH", GetSCE_PL_HASH, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_PL_SYMBOLTABLE", GetSCE_PL_SYMBOLTABLE, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_PL_REF", GetSCE_PL_REF, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_PL_REGEX", GetSCE_PL_REGEX, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_PL_REGSUBST", GetSCE_PL_REGSUBST, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_PL_LONGQUOTE", GetSCE_PL_LONGQUOTE, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_PL_BACKTICKS", GetSCE_PL_BACKTICKS, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_PL_DATASECTION", GetSCE_PL_DATASECTION, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_L_DEFAULT", GetSCE_L_DEFAULT, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_L_COMMAND", GetSCE_L_COMMAND, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_L_TAG", GetSCE_L_TAG, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_L_MATH", GetSCE_L_MATH, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_L_COMMENT", GetSCE_L_COMMENT, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_LUA_DEFAULT", GetSCE_LUA_DEFAULT, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_LUA_COMMENT", GetSCE_LUA_COMMENT, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_LUA_COMMENTLINE", GetSCE_LUA_COMMENTLINE, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_LUA_COMMENTDOC", GetSCE_LUA_COMMENTDOC, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_LUA_NUMBER", GetSCE_LUA_NUMBER, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_LUA_WORD", GetSCE_LUA_WORD, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_LUA_STRING", GetSCE_LUA_STRING, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_LUA_CHARACTER", GetSCE_LUA_CHARACTER, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_LUA_LITERALSTRING", GetSCE_LUA_LITERALSTRING, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_LUA_PREPROCESSOR", GetSCE_LUA_PREPROCESSOR, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_LUA_OPERATOR", GetSCE_LUA_OPERATOR, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_LUA_IDENTIFIER", GetSCE_LUA_IDENTIFIER, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SCE_LUA_STRINGEOL", GetSCE_LUA_STRINGEOL, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "EN_CHANGE", GetEN_CHANGE, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "EN_KILLFOCUS", GetEN_KILLFOCUS, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "EN_SETFOCUS", GetEN_SETFOCUS, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "EC_LEFTMARGIN", GetEC_LEFTMARGIN, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "EC_RIGHTMARGIN", GetEC_RIGHTMARGIN, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "EC_USEFONTINFO", GetEC_USEFONTINFO, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SEL_EMPTY", GetSEL_EMPTY, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SEL_TEXT", GetSEL_TEXT, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "FR_MATCHCASE", GetFR_MATCHCASE, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "FR_WHOLEWORD", GetFR_WHOLEWORD, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "FR_DOWN", GetFR_DOWN, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "SHIFT_PRESSED", GetSHIFT_PRESSED, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "LEFT_CTRL_PRESSED", GetLEFT_CTRL_PRESSED, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CScintillaCtrl, "LEFT_ALT_PRESSED", GetLEFT_ALT_PRESSED, SetNotSupported, VT_I4)
	DISP_FUNCTION(CScintillaCtrl, "AddText", AddText, VT_EMPTY, VTS_I4 VTS_BSTR)
	DISP_FUNCTION(CScintillaCtrl, "AddStyledText", AddStyledText, VT_EMPTY, VTS_I4 VTS_BSTR)
	DISP_FUNCTION(CScintillaCtrl, "InsertText", InsertText, VT_EMPTY, VTS_I4 VTS_BSTR)
	DISP_FUNCTION(CScintillaCtrl, "ClearAll", ClearAll, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "ClearDocumentStyle", ClearDocumentStyle, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "GetLength", GetLength, VT_I4, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "GetCharAt", GetCharAt, VT_I4, VTS_I4)
	DISP_FUNCTION(CScintillaCtrl, "GetCurrentPos", GetCurrentPos, VT_I4, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "GetAnchor", GetAnchor, VT_I4, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "GetStyleAt", GetStyleAt, VT_I4, VTS_I4)
	DISP_FUNCTION(CScintillaCtrl, "Redo", Redo, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "SetUndoCollection", SetUndoCollection, VT_EMPTY, VTS_BOOL)
	DISP_FUNCTION(CScintillaCtrl, "SelectAll", SelectAll, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "SetSavePoint", SetSavePoint, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "GetStyledText", GetStyledText, VT_I4, VTS_I4 VTS_I4 VTS_PBSTR)
	DISP_FUNCTION(CScintillaCtrl, "CanRedo", CanRedo, VT_BOOL, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "MarkerLineFromHandle", MarkerLineFromHandle, VT_I4, VTS_I4)
	DISP_FUNCTION(CScintillaCtrl, "MarkerDeleteHandle", MarkerDeleteHandle, VT_EMPTY, VTS_I4)
	DISP_FUNCTION(CScintillaCtrl, "GetUndoCollection", GetUndoCollection, VT_BOOL, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "GetViewWS", GetViewWS, VT_I4, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "SetViewWS", SetViewWS, VT_EMPTY, VTS_I4)
	DISP_FUNCTION(CScintillaCtrl, "PositionFromPoint", PositionFromPoint, VT_I4, VTS_I4 VTS_I4)
	DISP_FUNCTION(CScintillaCtrl, "GotoLine", GotoLine, VT_EMPTY, VTS_I4)
	DISP_FUNCTION(CScintillaCtrl, "GotoPos", GotoPos, VT_EMPTY, VTS_I4)
	DISP_FUNCTION(CScintillaCtrl, "SetAnchor", SetAnchor, VT_EMPTY, VTS_I4)
	DISP_FUNCTION(CScintillaCtrl, "GetCurLine", GetCurLine, VT_I4, VTS_I4 VTS_PBSTR)
	DISP_FUNCTION(CScintillaCtrl, "GetEndStyled", GetEndStyled, VT_I4, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "ConvertEOLs", ConvertEOLs, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "GetEOLMode", GetEOLMode, VT_I4, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "SetEOLMode", SetEOLMode, VT_EMPTY, VTS_I4)
	DISP_FUNCTION(CScintillaCtrl, "StartStyling", StartStyling, VT_EMPTY, VTS_I4 VTS_I4)
	DISP_FUNCTION(CScintillaCtrl, "SetStyling", SetStyling, VT_EMPTY, VTS_I4 VTS_I4)
	DISP_FUNCTION(CScintillaCtrl, "GetBufferedDraw", GetBufferedDraw, VT_BOOL, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "SetBufferedDraw", SetBufferedDraw, VT_EMPTY, VTS_BOOL)
	DISP_FUNCTION(CScintillaCtrl, "SetTabWidth", SetTabWidth, VT_EMPTY, VTS_I4)
	DISP_FUNCTION(CScintillaCtrl, "GetTabWidth", GetTabWidth, VT_I4, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "SetCodePage", SetCodePage, VT_EMPTY, VTS_I4)
	DISP_FUNCTION(CScintillaCtrl, "SetUsePalette", SetUsePalette, VT_EMPTY, VTS_BOOL)
	DISP_FUNCTION(CScintillaCtrl, "MarkerDefine", MarkerDefine, VT_EMPTY, VTS_I4 VTS_I4)
	DISP_FUNCTION(CScintillaCtrl, "MarkerSetFore", MarkerSetFore, VT_EMPTY, VTS_I4 VTS_I4)
	DISP_FUNCTION(CScintillaCtrl, "MarkerSetBack", MarkerSetBack, VT_EMPTY, VTS_I4 VTS_I4)
	DISP_FUNCTION(CScintillaCtrl, "MarkerAdd", MarkerAdd, VT_EMPTY, VTS_I4 VTS_I4)
	DISP_FUNCTION(CScintillaCtrl, "MarkerDelete", MarkerDelete, VT_EMPTY, VTS_I4 VTS_I4)
	DISP_FUNCTION(CScintillaCtrl, "MarkerDeleteAll", MarkerDeleteAll, VT_EMPTY, VTS_I4)
	DISP_FUNCTION(CScintillaCtrl, "MarkerGet", MarkerGet, VT_I4, VTS_I4)
	DISP_FUNCTION(CScintillaCtrl, "MarkerNext", MarkerNext, VT_I4, VTS_I4 VTS_I4)
	DISP_FUNCTION(CScintillaCtrl, "MarkerPrevious", MarkerPrevious, VT_I4, VTS_I4 VTS_I4)
	DISP_FUNCTION(CScintillaCtrl, "SetMarginTypeN", SetMarginTypeN, VT_EMPTY, VTS_I4 VTS_I4)
	DISP_FUNCTION(CScintillaCtrl, "GetMarginTypeN", GetMarginTypeN, VT_I4, VTS_I4)
	DISP_FUNCTION(CScintillaCtrl, "SetMarginWidthN", SetMarginWidthN, VT_EMPTY, VTS_I4 VTS_I4)
	DISP_FUNCTION(CScintillaCtrl, "GetMarginWidthN", GetMarginWidthN, VT_I4, VTS_I4)
	DISP_FUNCTION(CScintillaCtrl, "SetMarginMaskN", SetMarginMaskN, VT_EMPTY, VTS_I4 VTS_I4)
	DISP_FUNCTION(CScintillaCtrl, "GetMarginMaskN", GetMarginMaskN, VT_I4, VTS_I4)
	DISP_FUNCTION(CScintillaCtrl, "SetMarginSensitiveN", SetMarginSensitiveN, VT_EMPTY, VTS_I4 VTS_BOOL)
	DISP_FUNCTION(CScintillaCtrl, "GetMarginSensitiveN", GetMarginSensitiveN, VT_BOOL, VTS_I4)
	DISP_FUNCTION(CScintillaCtrl, "StyleClearAll", StyleClearAll, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "StyleSetFore", StyleSetFore, VT_EMPTY, VTS_I4 VTS_I4)
	DISP_FUNCTION(CScintillaCtrl, "StyleSetBack", StyleSetBack, VT_EMPTY, VTS_I4 VTS_I4)
	DISP_FUNCTION(CScintillaCtrl, "StyleSetBold", StyleSetBold, VT_EMPTY, VTS_I4 VTS_BOOL)
	DISP_FUNCTION(CScintillaCtrl, "StyleSetItalic", StyleSetItalic, VT_EMPTY, VTS_I4 VTS_BOOL)
	DISP_FUNCTION(CScintillaCtrl, "StyleSetSize", StyleSetSize, VT_EMPTY, VTS_I4 VTS_I4)
	DISP_FUNCTION(CScintillaCtrl, "StyleSetFont", StyleSetFont, VT_EMPTY, VTS_I4 VTS_BSTR)
	DISP_FUNCTION(CScintillaCtrl, "StyleSetEOLFilled", StyleSetEOLFilled, VT_EMPTY, VTS_I4 VTS_BOOL)
	DISP_FUNCTION(CScintillaCtrl, "StyleResetDefault", StyleResetDefault, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "StyleSetUnderline", StyleSetUnderline, VT_EMPTY, VTS_I4 VTS_BOOL)
	DISP_FUNCTION(CScintillaCtrl, "StyleSetCharacterSet", StyleSetCharacterSet, VT_EMPTY, VTS_I4 VTS_I4)
	DISP_FUNCTION(CScintillaCtrl, "SetSelFore", SetSelFore, VT_EMPTY, VTS_BOOL VTS_I4)
	DISP_FUNCTION(CScintillaCtrl, "SetSelBack", SetSelBack, VT_EMPTY, VTS_BOOL VTS_I4)
	DISP_FUNCTION(CScintillaCtrl, "SetCaretFore", SetCaretFore, VT_EMPTY, VTS_I4)
	DISP_FUNCTION(CScintillaCtrl, "AssignCmdKey", AssignCmdKey, VT_EMPTY, VTS_I2 VTS_I2 VTS_I4)
	DISP_FUNCTION(CScintillaCtrl, "ClearCmdKey", ClearCmdKey, VT_EMPTY, VTS_I2 VTS_I2)
	DISP_FUNCTION(CScintillaCtrl, "ClearAllCmdKeys", ClearAllCmdKeys, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "SetStylingEx", SetStylingEx, VT_EMPTY, VTS_I4 VTS_BSTR)
	DISP_FUNCTION(CScintillaCtrl, "GetCaretPeriod", GetCaretPeriod, VT_I4, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "SetCaretPeriod", SetCaretPeriod, VT_EMPTY, VTS_I4)
	DISP_FUNCTION(CScintillaCtrl, "SetWordChars", SetWordChars, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CScintillaCtrl, "BeginUndoAction", BeginUndoAction, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "EndUndoAction", EndUndoAction, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "IndicSetStyle", IndicSetStyle, VT_EMPTY, VTS_I4 VTS_I4)
	DISP_FUNCTION(CScintillaCtrl, "IndicGetStyle", IndicGetStyle, VT_I4, VTS_I4)
	DISP_FUNCTION(CScintillaCtrl, "IndicSetFore", IndicSetFore, VT_EMPTY, VTS_I4 VTS_I4)
	DISP_FUNCTION(CScintillaCtrl, "IndicGetFore", IndicGetFore, VT_I4, VTS_I4)
	DISP_FUNCTION(CScintillaCtrl, "SetStyleBits", SetStyleBits, VT_EMPTY, VTS_I4)
	DISP_FUNCTION(CScintillaCtrl, "GetStyleBits", GetStyleBits, VT_I4, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "SetLineState", SetLineState, VT_EMPTY, VTS_I4 VTS_I4)
	DISP_FUNCTION(CScintillaCtrl, "GetLineState", GetLineState, VT_I4, VTS_I4)
	DISP_FUNCTION(CScintillaCtrl, "GetMaxLineState", GetMaxLineState, VT_I4, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "AutoCShow", AutoCShow, VT_EMPTY, VTS_I4 VTS_BSTR)
	DISP_FUNCTION(CScintillaCtrl, "AutoCCancel", AutoCCancel, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "AutoCActive", AutoCActive, VT_BOOL, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "AutoCPosStart", AutoCPosStart, VT_I4, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "AutoCComplete", AutoCComplete, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "AutoCStops", AutoCStops, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CScintillaCtrl, "AutoCSetSeparator", AutoCSetSeparator, VT_EMPTY, VTS_I4)
	DISP_FUNCTION(CScintillaCtrl, "AutoCGetSeparator", AutoCGetSeparator, VT_I4, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "AutoCSelect", AutoCSelect, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CScintillaCtrl, "AutoCSetCancelAtStart", AutoCSetCancelAtStart, VT_EMPTY, VTS_BOOL)
	DISP_FUNCTION(CScintillaCtrl, "AutoCGetCancelAtStart", AutoCGetCancelAtStart, VT_BOOL, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "SetIndent", SetIndent, VT_EMPTY, VTS_I4)
	DISP_FUNCTION(CScintillaCtrl, "GetIndent", GetIndent, VT_I4, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "SetUseTabs", SetUseTabs, VT_EMPTY, VTS_BOOL)
	DISP_FUNCTION(CScintillaCtrl, "GetUseTabs", GetUseTabs, VT_BOOL, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "SetLineIndentation", SetLineIndentation, VT_EMPTY, VTS_I4 VTS_I4)
	DISP_FUNCTION(CScintillaCtrl, "GetLineIndentation", GetLineIndentation, VT_I4, VTS_I4)
	DISP_FUNCTION(CScintillaCtrl, "GetLineIndentPosition", GetLineIndentPosition, VT_I4, VTS_I4)
	DISP_FUNCTION(CScintillaCtrl, "GetColumn", GetColumn, VT_I4, VTS_I4)
	DISP_FUNCTION(CScintillaCtrl, "SetHScrollBar", SetHScrollBar, VT_EMPTY, VTS_BOOL)
	DISP_FUNCTION(CScintillaCtrl, "GetHScrollBar", GetHScrollBar, VT_BOOL, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "SetIndentationGuides", SetIndentationGuides, VT_EMPTY, VTS_BOOL)
	DISP_FUNCTION(CScintillaCtrl, "GetIndentationGuides", GetIndentationGuides, VT_BOOL, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "SetHighlightGuide", SetHighlightGuide, VT_EMPTY, VTS_I4)
	DISP_FUNCTION(CScintillaCtrl, "GetHighlightGuide", GetHighlightGuide, VT_I4, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "GetLineEndPosition", GetLineEndPosition, VT_I4, VTS_I4)
	DISP_FUNCTION(CScintillaCtrl, "GetCodePage", GetCodePage, VT_I4, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "GetCaretFore", GetCaretFore, VT_I4, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "GetUsePalette", GetUsePalette, VT_BOOL, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "GetReadOnly", GetReadOnly, VT_BOOL, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "SetCurrentPos", SetCurrentPos, VT_EMPTY, VTS_I4)
	DISP_FUNCTION(CScintillaCtrl, "SetSelectionStart", SetSelectionStart, VT_EMPTY, VTS_I4)
	DISP_FUNCTION(CScintillaCtrl, "GetSelectionStart", GetSelectionStart, VT_I4, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "SetSelectionEnd", SetSelectionEnd, VT_EMPTY, VTS_I4)
	DISP_FUNCTION(CScintillaCtrl, "GetSelectionEnd", GetSelectionEnd, VT_I4, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "SetPrintMagnification", SetPrintMagnification, VT_EMPTY, VTS_I4)
	DISP_FUNCTION(CScintillaCtrl, "GetPrintMagnification", GetPrintMagnification, VT_I4, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "SetPrintColourMode", SetPrintColourMode, VT_EMPTY, VTS_I4)
	DISP_FUNCTION(CScintillaCtrl, "GetPrintColourMode", GetPrintColourMode, VT_I4, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "FindText", FindText, VT_I4, VTS_I4 VTS_I4 VTS_I4 VTS_BSTR)
	DISP_FUNCTION(CScintillaCtrl, "FormatRange", FormatRange, VT_EMPTY, VTS_BOOL VTS_I4)
	DISP_FUNCTION(CScintillaCtrl, "GetFirstVisibleLine", GetFirstVisibleLine, VT_I4, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "GetLine", GetLine, VT_I4, VTS_I4 VTS_PBSTR)
	DISP_FUNCTION(CScintillaCtrl, "GetLineCount", GetLineCount, VT_I4, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "SetMarginLeft", SetMarginLeft, VT_EMPTY, VTS_I4)
	DISP_FUNCTION(CScintillaCtrl, "GetMarginLeft", GetMarginLeft, VT_I4, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "SetMarginRight", SetMarginRight, VT_EMPTY, VTS_I4)
	DISP_FUNCTION(CScintillaCtrl, "GetMarginRight", GetMarginRight, VT_I4, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "GetModify", GetModify, VT_BOOL, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "SetSel", SetSel, VT_EMPTY, VTS_I4 VTS_I4)
	DISP_FUNCTION(CScintillaCtrl, "GetSelText", GetSelText, VT_I4, VTS_PBSTR)
	DISP_FUNCTION(CScintillaCtrl, "GetTextRange", GetTextRange, VT_I4, VTS_I4 VTS_I4 VTS_PBSTR)
	DISP_FUNCTION(CScintillaCtrl, "HideSelection", HideSelection, VT_EMPTY, VTS_BOOL)
	DISP_FUNCTION(CScintillaCtrl, "PointXFromPosition", PointXFromPosition, VT_I4, VTS_I4)
	DISP_FUNCTION(CScintillaCtrl, "PointYFromPosition", PointYFromPosition, VT_I4, VTS_I4)
	DISP_FUNCTION(CScintillaCtrl, "LineFromPosition", LineFromPosition, VT_I4, VTS_I4)
	DISP_FUNCTION(CScintillaCtrl, "PositionFromLine", PositionFromLine, VT_I4, VTS_I4)
	DISP_FUNCTION(CScintillaCtrl, "LineScroll", LineScroll, VT_EMPTY, VTS_I4 VTS_I4)
	DISP_FUNCTION(CScintillaCtrl, "ScrollCaret", ScrollCaret, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "ReplaceSel", ReplaceSel, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CScintillaCtrl, "SetReadOnly", SetReadOnly, VT_EMPTY, VTS_BOOL)
	DISP_FUNCTION(CScintillaCtrl, "Null", Null, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "CanPaste", CanPaste, VT_BOOL, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "CanUndo", CanUndo, VT_BOOL, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "EmptyUndoBuffer", EmptyUndoBuffer, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "Undo", Undo, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "Cut", Cut, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "Copy", Copy, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "Paste", Paste, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "Clear", Clear, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "SetText", SetText, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CScintillaCtrl, "GetText", GetText, VT_I4, VTS_I4 VTS_PBSTR)
	DISP_FUNCTION(CScintillaCtrl, "GetTextLength", GetTextLength, VT_I4, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "GetDirectFunction", GetDirectFunction, VT_I4, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "GetDirectPointer", GetDirectPointer, VT_I4, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "CallTipShow", CallTipShow, VT_EMPTY, VTS_I4 VTS_BSTR)
	DISP_FUNCTION(CScintillaCtrl, "CallTipCancel", CallTipCancel, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "CallTipActive", CallTipActive, VT_BOOL, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "CallTipPosStart", CallTipPosStart, VT_I4, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "CallTipSetHlt", CallTipSetHlt, VT_EMPTY, VTS_I4 VTS_I4)
	DISP_FUNCTION(CScintillaCtrl, "CallTipSetBack", CallTipSetBack, VT_EMPTY, VTS_I4)
	DISP_FUNCTION(CScintillaCtrl, "VisibleFromDocLine", VisibleFromDocLine, VT_I4, VTS_I4)
	DISP_FUNCTION(CScintillaCtrl, "DocLineFromVisible", DocLineFromVisible, VT_I4, VTS_I4)
	DISP_FUNCTION(CScintillaCtrl, "SetFoldLevel", SetFoldLevel, VT_EMPTY, VTS_I4 VTS_I4)
	DISP_FUNCTION(CScintillaCtrl, "GetFoldLevel", GetFoldLevel, VT_I4, VTS_I4)
	DISP_FUNCTION(CScintillaCtrl, "GetLastChild", GetLastChild, VT_I4, VTS_I4 VTS_I4)
	DISP_FUNCTION(CScintillaCtrl, "GetFoldParent", GetFoldParent, VT_I4, VTS_I4)
	DISP_FUNCTION(CScintillaCtrl, "ShowLines", ShowLines, VT_EMPTY, VTS_I4 VTS_I4)
	DISP_FUNCTION(CScintillaCtrl, "HideLines", HideLines, VT_EMPTY, VTS_I4 VTS_I4)
	DISP_FUNCTION(CScintillaCtrl, "GetLineVisible", GetLineVisible, VT_BOOL, VTS_I4)
	DISP_FUNCTION(CScintillaCtrl, "SetFoldExpanded", SetFoldExpanded, VT_EMPTY, VTS_I4 VTS_BOOL)
	DISP_FUNCTION(CScintillaCtrl, "GetFoldExpanded", GetFoldExpanded, VT_BOOL, VTS_I4)
	DISP_FUNCTION(CScintillaCtrl, "ToggleFold", ToggleFold, VT_EMPTY, VTS_I4)
	DISP_FUNCTION(CScintillaCtrl, "EnsureVisible", EnsureVisible, VT_EMPTY, VTS_I4)
	DISP_FUNCTION(CScintillaCtrl, "SetFoldFlags", SetFoldFlags, VT_EMPTY, VTS_I4)
	DISP_FUNCTION(CScintillaCtrl, "LineDown", LineDown, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "LineDownExtend", LineDownExtend, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "LineUp", LineUp, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "LineUpExtend", LineUpExtend, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "CharLeft", CharLeft, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "CharLeftExtend", CharLeftExtend, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "CharRight", CharRight, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "CharRightExtend", CharRightExtend, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "WordLeft", WordLeft, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "WordLeftExtend", WordLeftExtend, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "WordRight", WordRight, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "WordRightExtend", WordRightExtend, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "Home", Home, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "HomeExtend", HomeExtend, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "LineEnd", LineEnd, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "LineEndExtend", LineEndExtend, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "DocumentStart", DocumentStart, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "DocumentStartExtend", DocumentStartExtend, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "DocumentEnd", DocumentEnd, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "DocumentEndExtend", DocumentEndExtend, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "PageUp", PageUp, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "PageUpExtend", PageUpExtend, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "PageDown", PageDown, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "PageDownExtend", PageDownExtend, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "EditToggleOvertype", EditToggleOvertype, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "Cancel", Cancel, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "DeleteBack", DeleteBack, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "Tab", Tab, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "BackTab", BackTab, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "NewLine", NewLine, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "FormFeed", FormFeed, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "VCHome", VCHome, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "VCHomeExtend", VCHomeExtend, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "ZoomIn", ZoomIn, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "ZoomOut", ZoomOut, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "DelWordLeft", DelWordLeft, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "DelWordRight", DelWordRight, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "LineCut", LineCut, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "LineDelete", LineDelete, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "LineTranspose", LineTranspose, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "LowerCase", LowerCase, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "UpperCase", UpperCase, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "LineScrollDown", LineScrollDown, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "LineScrollUp", LineScrollUp, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "LineLength", LineLength, VT_I4, VTS_I4)
	DISP_FUNCTION(CScintillaCtrl, "BraceHighlight", BraceHighlight, VT_EMPTY, VTS_I4 VTS_I4)
	DISP_FUNCTION(CScintillaCtrl, "BraceBadLight", BraceBadLight, VT_EMPTY, VTS_I4)
	DISP_FUNCTION(CScintillaCtrl, "BraceMatch", BraceMatch, VT_I4, VTS_I4)
	DISP_FUNCTION(CScintillaCtrl, "GetViewEOL", GetViewEOL, VT_BOOL, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "SetViewEOL", SetViewEOL, VT_EMPTY, VTS_BOOL)
	DISP_FUNCTION(CScintillaCtrl, "GetDocPointer", GetDocPointer, VT_I4, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "SetDocPointer", SetDocPointer, VT_EMPTY, VTS_I4)
	DISP_FUNCTION(CScintillaCtrl, "SetModEventMask", SetModEventMask, VT_EMPTY, VTS_I4)
	DISP_FUNCTION(CScintillaCtrl, "GetEdgeColumn", GetEdgeColumn, VT_I4, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "SetEdgeColumn", SetEdgeColumn, VT_EMPTY, VTS_I4)
	DISP_FUNCTION(CScintillaCtrl, "GetEdgeMode", GetEdgeMode, VT_I4, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "SetEdgeMode", SetEdgeMode, VT_EMPTY, VTS_I4)
	DISP_FUNCTION(CScintillaCtrl, "GetEdgeColour", GetEdgeColour, VT_I4, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "SetEdgeColour", SetEdgeColour, VT_EMPTY, VTS_I4)
	DISP_FUNCTION(CScintillaCtrl, "SearchAnchor", SearchAnchor, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "SearchNext", SearchNext, VT_I4, VTS_I4 VTS_BSTR)
	DISP_FUNCTION(CScintillaCtrl, "SearchPrev", SearchPrev, VT_I4, VTS_I4 VTS_BSTR)
	DISP_FUNCTION(CScintillaCtrl, "SetCaretPolicy", SetCaretPolicy, VT_EMPTY, VTS_I4 VTS_I4)
	DISP_FUNCTION(CScintillaCtrl, "LinesOnScreen", LinesOnScreen, VT_I4, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "UsePopUp", UsePopUp, VT_EMPTY, VTS_BOOL)
	DISP_FUNCTION(CScintillaCtrl, "SelectionIsRectangle", SelectionIsRectangle, VT_BOOL, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "SetZoom", SetZoom, VT_EMPTY, VTS_I4)
	DISP_FUNCTION(CScintillaCtrl, "GetZoom", GetZoom, VT_I4, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "CreateDocument", CreateDocument, VT_I4, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "AddRefDocument", AddRefDocument, VT_EMPTY, VTS_I4)
	DISP_FUNCTION(CScintillaCtrl, "ReleaseDocument", ReleaseDocument, VT_EMPTY, VTS_I4)
	DISP_FUNCTION(CScintillaCtrl, "GetModEventMask", GetModEventMask, VT_I4, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "GrabFocus", GrabFocus, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "StartRecord", StartRecord, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "StopRecord", StopRecord, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "SetLexer", SetLexer, VT_EMPTY, VTS_I4)
	DISP_FUNCTION(CScintillaCtrl, "GetLexer", GetLexer, VT_I4, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "Colourise", Colourise, VT_EMPTY, VTS_I4 VTS_I4)
	DISP_FUNCTION(CScintillaCtrl, "SetProperty", SetProperty, VT_EMPTY, VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CScintillaCtrl, "SetKeyWords", SetKeyWords, VT_EMPTY, VTS_I4 VTS_BSTR)
	DISP_FUNCTION(CScintillaCtrl, "EM_CanPaste", EM_CanPaste, VT_BOOL, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "EM_CanUndo", EM_CanUndo, VT_BOOL, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "EM_CharFromPos", EM_CharFromPos, VT_I4, VTS_I4 VTS_I4)
	DISP_FUNCTION(CScintillaCtrl, "EM_EmptyUndoBuffer", EM_EmptyUndoBuffer, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "EM_ExGetSel", EM_ExGetSel, VT_EMPTY, VTS_PI4 VTS_PI4)
	DISP_FUNCTION(CScintillaCtrl, "EM_ExLineFromChar", EM_ExLineFromChar, VT_I4, VTS_I4)
	DISP_FUNCTION(CScintillaCtrl, "EM_ExSetSel", EM_ExSetSel, VT_EMPTY, VTS_I4 VTS_I4)
	DISP_FUNCTION(CScintillaCtrl, "EM_FindText", EM_FindText, VT_I4, VTS_I4 VTS_I4 VTS_I4 VTS_BSTR)
	DISP_FUNCTION(CScintillaCtrl, "EM_FindTextEx", EM_FindTextEx, VT_I4, VTS_I4 VTS_I4 VTS_I4 VTS_BSTR VTS_PI4 VTS_PI4)
	DISP_FUNCTION(CScintillaCtrl, "EM_FormatRange", EM_FormatRange, VT_EMPTY, VTS_BOOL VTS_I4)
	DISP_FUNCTION(CScintillaCtrl, "EM_GetFirstVisibleLine", EM_GetFirstVisibleLine, VT_I4, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "EM_GetLine", EM_GetLine, VT_I4, VTS_I4 VTS_I4 VTS_PBSTR)
	DISP_FUNCTION(CScintillaCtrl, "EM_GetLineCount", EM_GetLineCount, VT_I4, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "EM_GetMargins", EM_GetMargins, VT_I4, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "EM_GetModify", EM_GetModify, VT_BOOL, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "EM_GetRect", EM_GetRect, VT_EMPTY, VTS_PI4 VTS_PI4 VTS_PI4 VTS_PI4)
	DISP_FUNCTION(CScintillaCtrl, "EM_GetSel", EM_GetSel, VT_I4, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "EM_GetSelText", EM_GetSelText, VT_I4, VTS_PBSTR)
	DISP_FUNCTION(CScintillaCtrl, "EM_GetTextRange", EM_GetTextRange, VT_I4, VTS_I4 VTS_I4 VTS_PBSTR)
	DISP_FUNCTION(CScintillaCtrl, "EM_HideSelection", EM_HideSelection, VT_EMPTY, VTS_BOOL)
	DISP_FUNCTION(CScintillaCtrl, "EM_LineFromChar", EM_LineFromChar, VT_I4, VTS_I4)
	DISP_FUNCTION(CScintillaCtrl, "EM_LineIndex", EM_LineIndex, VT_I4, VTS_I4)
	DISP_FUNCTION(CScintillaCtrl, "EM_LineLength", EM_LineLength, VT_I4, VTS_I4)
	DISP_FUNCTION(CScintillaCtrl, "EM_LineScroll", EM_LineScroll, VT_EMPTY, VTS_I4 VTS_I4)
	DISP_FUNCTION(CScintillaCtrl, "EM_PosFromChar", EM_PosFromChar, VT_EMPTY, VTS_PI4 VTS_PI4 VTS_I4)
	DISP_FUNCTION(CScintillaCtrl, "EM_ReplaceSel", EM_ReplaceSel, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CScintillaCtrl, "EM_ScrollCaret", EM_ScrollCaret, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "EM_SelectionType", EM_SelectionType, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "EM_SetMargins", EM_SetMargins, VT_EMPTY, VTS_I4 VTS_I4)
	DISP_FUNCTION(CScintillaCtrl, "EM_SetReadOnly", EM_SetReadOnly, VT_EMPTY, VTS_BOOL)
	DISP_FUNCTION(CScintillaCtrl, "EM_SetSel", EM_SetSel, VT_EMPTY, VTS_I4 VTS_I4)
	DISP_FUNCTION(CScintillaCtrl, "EM_Undo", EM_Undo, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "WM_Null", WM_Null, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "WM_Clear", WM_Clear, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "WM_Command", WM_Command, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "WM_Copy", WM_Copy, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "WM_Cut", WM_Cut, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "WM_GetText", WM_GetText, VT_I4, VTS_I4 VTS_PBSTR)
	DISP_FUNCTION(CScintillaCtrl, "WM_GetTextLength", WM_GetTextLength, VT_I4, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "WM_Notify", WM_Notify, VT_EMPTY, VTS_I4 VTS_I4)
	DISP_FUNCTION(CScintillaCtrl, "WM_Paste", WM_Paste, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CScintillaCtrl, "WM_SetText", WM_SetText, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CScintillaCtrl, "WM_Undo", WM_Undo, VT_EMPTY, VTS_NONE)
	//}}AFX_DISPATCH_MAP
	//$(DISPMAP_SECTION]
END_DISPATCH_MAP()

/////////////////////////////////////////////////////////////////////////////
// Event map

BEGIN_EVENT_MAP(CScintillaCtrl, COleControl)
	//$[EVTMAP_SECTION)
	//{{AFX_EVENT_MAP(CScintillaCtrl)
	EVENT_CUSTOM("StyleNeeded", FireStyleNeeded, VTS_I4)
	EVENT_CUSTOM("CharAdded", FireCharAdded, VTS_I4)
	EVENT_CUSTOM("SavePointReached", FireSavePointReached, VTS_NONE)
	EVENT_CUSTOM("SavePointLeft", FireSavePointLeft, VTS_NONE)
	EVENT_CUSTOM("ModifyAttemptRO", FireModifyAttemptRO, VTS_NONE)
	EVENT_CUSTOM("Key", FireKey, VTS_I4 VTS_I4)
	EVENT_CUSTOM("DoubleClick", FireDoubleClick, VTS_NONE)
	EVENT_CUSTOM("UpdateUI", FireUpdateUI, VTS_NONE)
	EVENT_CUSTOM("Modified", FireModified, VTS_I4 VTS_I4 VTS_BSTR VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	EVENT_CUSTOM("MacroRecord", FireMacroRecord, VTS_I4 VTS_I4 VTS_I4)
	EVENT_CUSTOM("MarginClick", FireMarginClick, VTS_I4 VTS_I4 VTS_I4)
	EVENT_CUSTOM("NeedShown", FireNeedShown, VTS_I4 VTS_I4)
	EVENT_CUSTOM("EN_Change", FireEN_Change, VTS_NONE)
	//}}AFX_EVENT_MAP
	//$(EVTMAP_SECTION]
END_EVENT_MAP()


/////////////////////////////////////////////////////////////////////////////
// Property pages

BEGIN_PROPPAGEIDS(CScintillaCtrl, 1)
	PROPPAGEID(CScintillaPropPage::guid)
END_PROPPAGEIDS(CScintillaCtrl)


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CScintillaCtrl, "SCINTILLA.ScintillaCtrl.1",
	0x4a2dff62, 0xc146, 0x11d3, 0x9c, 0xdf, 0x48, 0x24, 0x71, 0, 0, 0)


/////////////////////////////////////////////////////////////////////////////
// Type library ID and version

IMPLEMENT_OLETYPELIB(CScintillaCtrl, _tlid, _wVerMajor, _wVerMinor)


/////////////////////////////////////////////////////////////////////////////
// Interface IDs

const IID BASED_CODE IID_DScintilla =
		{ 0x4a2dff60, 0xc146, 0x11d3, { 0x9c, 0xdf, 0x48, 0x24, 0x71, 0, 0, 0 } };
const IID BASED_CODE IID_DScintillaEvents =
		{ 0x4a2dff61, 0xc146, 0x11d3, { 0x9c, 0xdf, 0x48, 0x24, 0x71, 0, 0, 0 } };


/////////////////////////////////////////////////////////////////////////////
// Control type information

static const DWORD BASED_CODE _dwScintillaOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CScintillaCtrl, IDS_SCINTILLA, _dwScintillaOleMisc)


/////////////////////////////////////////////////////////////////////////////
// CScintillaCtrl::CScintillaCtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CScintillaCtrl

BOOL CScintillaCtrl::CScintillaCtrlFactory::UpdateRegistry(BOOL bRegister)
{
	// TODO: Verify that your control follows apartment-model threading rules.
	// Refer to MFC TechNote 64 for more information.
	// If your control does not conform to the apartment-model rules, then
	// you must modify the code below, changing the 6th parameter from
	// afxRegInsertable | afxRegApartmentThreading to afxRegInsertable.

	if (bRegister)
		return AfxOleRegisterControlClass(
			AfxGetInstanceHandle(),
			m_clsid,
			m_lpszProgID,
			IDS_SCINTILLA,
			IDB_SCINTILLA,
			afxRegInsertable | afxRegApartmentThreading,
			_dwScintillaOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}


/////////////////////////////////////////////////////////////////////////////
// CScintillaCtrl::CScintillaCtrl - Constructor

CScintillaCtrl::CScintillaCtrl()
{
	InitializeIIDs(&IID_DScintilla, &IID_DScintillaEvents);
}


/////////////////////////////////////////////////////////////////////////////
// CScintillaCtrl::~CScintillaCtrl - Destructor

CScintillaCtrl::~CScintillaCtrl()
{
}

LRESULT CScintillaCtrl::OnOcmNotify(WPARAM wParam, LPARAM lParam)
{
	SCNotification* scn = reinterpret_cast<SCNotification *>(lParam);
	switch(scn->nmhdr.code)
	{
		//$[EVTCPP_SECTION)
		case SCN_STYLENEEDED:
			FireStyleNeeded(scn->position);
			break;

		case SCN_CHARADDED:
			FireCharAdded(scn->ch);
			break;

		case SCN_SAVEPOINTREACHED:
			FireSavePointReached();
			break;

		case SCN_SAVEPOINTLEFT:
			FireSavePointLeft();
			break;

		case SCN_MODIFYATTEMPTRO:
			FireModifyAttemptRO();
			break;

		case SCN_KEY:
			FireKey(scn->ch,scn->modifiers);
			break;

		case SCN_DOUBLECLICK:
			FireDoubleClick();
			break;

		case SCN_UPDATEUI:
			FireUpdateUI();
			break;

		case SCN_MODIFIED:
			FireModified(scn->position,scn->modificationType,scn->text,scn->length,scn->linesAdded,scn->line,scn->foldLevelNow,scn->foldLevelPrev);
			break;

		case SCN_MACRORECORD:
			FireMacroRecord(scn->message,scn->wParam,scn->lParam);
			break;

		case SCN_MARGINCLICK:
			FireMarginClick(scn->modifiers,scn->position,scn->margin);
			break;

		case SCN_NEEDSHOWN:
			FireNeedShown(scn->position,scn->length);
			break;

		case EN_CHANGE:
			FireEN_Change();
			break;

		//$(EVTCPP_SECTION]
	}
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CScintillaCtrl::OnDraw - Drawing function

void CScintillaCtrl::OnDraw(
			CDC* pDC, const CRect& rcBounds, const CRect& rcInvalid)
{
	if (m_hWnd == NULL)
		CreateWindowForSubclassedControl();

	if (m_hWnd != NULL)
	{
		CRect rcClient;
		GetClientRect(&rcClient);

		if (rcClient.Size() != rcBounds.Size())
		{
			pDC->SetMapMode(MM_ANISOTROPIC);
			pDC->SetWindowExt(rcClient.right, rcClient.bottom);
			pDC->SetViewportExt(rcBounds.Size());
		}
		pDC->SetWindowOrg(0, 0);
		pDC->SetViewportOrg(rcBounds.left, rcBounds.top);

		CPaintDC* ppdc = dynamic_cast<CPaintDC*>(pDC);
		if(ppdc)
			::CallWindowProc(
				*GetSuperWndProcAddr(),
				m_hWnd,WM_PAINT,(WPARAM)(&ppdc->m_ps),(LPARAM)(0));
		else{
			PAINTSTRUCT ps;
			ps.hdc = pDC->m_hDC;
			ps.rcPaint = rcClient;
			ps.fErase = TRUE;
			::CallWindowProc(
				*GetSuperWndProcAddr(),
				m_hWnd,WM_PAINT,(WPARAM)(&ps),(LPARAM)(0));
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// CScintillaCtrl::DoPropExchange - Persistence support

void CScintillaCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

}


/////////////////////////////////////////////////////////////////////////////
// CScintillaCtrl::OnResetState - Reset control to default state

void CScintillaCtrl::OnResetState()
{
	COleControl::OnResetState();  // Resets defaults found in DoPropExchange
}


/////////////////////////////////////////////////////////////////////////////
// CScintillaCtrl message handlers

BOOL CScintillaCtrl::PreCreateWindow(CREATESTRUCT& cs) 
{
    cs.lpszClass = _T("Scintilla");
	cs.style |= WS_CHILD | WS_VSCROLL | WS_HSCROLL;
	return COleControl::PreCreateWindow(cs);
}

BOOL CScintillaCtrl::IsSubclassedControl(void)
{
    return TRUE;
}

//$[METHCPP_SECTION)
long CScintillaCtrl::GetINVALID_POSITION()
{
	return INVALID_POSITION;
}

long CScintillaCtrl::GetSCI_START()
{
	return SCI_START;
}

long CScintillaCtrl::GetSCI_OPTIONAL_START()
{
	return SCI_OPTIONAL_START;
}

long CScintillaCtrl::GetSCI_LEXER_START()
{
	return SCI_LEXER_START;
}

long CScintillaCtrl::GetSCWS_INVISIBLE()
{
	return SCWS_INVISIBLE;
}

long CScintillaCtrl::GetSCWS_VISIBLEALWAYS()
{
	return SCWS_VISIBLEALWAYS;
}

long CScintillaCtrl::GetSCWS_VISIBLEAFTERINDENT()
{
	return SCWS_VISIBLEAFTERINDENT;
}

long CScintillaCtrl::GetSC_EOL_CRLF()
{
	return SC_EOL_CRLF;
}

long CScintillaCtrl::GetSC_EOL_CR()
{
	return SC_EOL_CR;
}

long CScintillaCtrl::GetSC_EOL_LF()
{
	return SC_EOL_LF;
}

long CScintillaCtrl::GetSC_CP_UTF8()
{
	return SC_CP_UTF8;
}

long CScintillaCtrl::GetMARKER_MAX()
{
	return MARKER_MAX;
}

long CScintillaCtrl::GetSC_MARK_CIRCLE()
{
	return SC_MARK_CIRCLE;
}

long CScintillaCtrl::GetSC_MARK_ROUNDRECT()
{
	return SC_MARK_ROUNDRECT;
}

long CScintillaCtrl::GetSC_MARK_ARROW()
{
	return SC_MARK_ARROW;
}

long CScintillaCtrl::GetSC_MARK_SMALLRECT()
{
	return SC_MARK_SMALLRECT;
}

long CScintillaCtrl::GetSC_MARK_SHORTARROW()
{
	return SC_MARK_SHORTARROW;
}

long CScintillaCtrl::GetSC_MARK_EMPTY()
{
	return SC_MARK_EMPTY;
}

long CScintillaCtrl::GetSC_MARK_ARROWDOWN()
{
	return SC_MARK_ARROWDOWN;
}

long CScintillaCtrl::GetSC_MARK_MINUS()
{
	return SC_MARK_MINUS;
}

long CScintillaCtrl::GetSC_MARK_PLUS()
{
	return SC_MARK_PLUS;
}

long CScintillaCtrl::GetSC_MARKNUM_FOLDER()
{
	return SC_MARKNUM_FOLDER;
}

long CScintillaCtrl::GetSC_MARKNUM_FOLDEROPEN()
{
	return SC_MARKNUM_FOLDEROPEN;
}

long CScintillaCtrl::GetSC_MARGIN_SYMBOL()
{
	return SC_MARGIN_SYMBOL;
}

long CScintillaCtrl::GetSC_MARGIN_NUMBER()
{
	return SC_MARGIN_NUMBER;
}

long CScintillaCtrl::GetSTYLE_DEFAULT()
{
	return STYLE_DEFAULT;
}

long CScintillaCtrl::GetSTYLE_LINENUMBER()
{
	return STYLE_LINENUMBER;
}

long CScintillaCtrl::GetSTYLE_BRACELIGHT()
{
	return STYLE_BRACELIGHT;
}

long CScintillaCtrl::GetSTYLE_BRACEBAD()
{
	return STYLE_BRACEBAD;
}

long CScintillaCtrl::GetSTYLE_CONTROLCHAR()
{
	return STYLE_CONTROLCHAR;
}

long CScintillaCtrl::GetSTYLE_MAX()
{
	return STYLE_MAX;
}

long CScintillaCtrl::GetSC_CHARSET_ANSI()
{
	return SC_CHARSET_ANSI;
}

long CScintillaCtrl::GetSC_CHARSET_DEFAULT()
{
	return SC_CHARSET_DEFAULT;
}

long CScintillaCtrl::GetSC_CHARSET_BALTIC()
{
	return SC_CHARSET_BALTIC;
}

long CScintillaCtrl::GetSC_CHARSET_CHINESEBIG5()
{
	return SC_CHARSET_CHINESEBIG5;
}

long CScintillaCtrl::GetSC_CHARSET_EASTEUROPE()
{
	return SC_CHARSET_EASTEUROPE;
}

long CScintillaCtrl::GetSC_CHARSET_GB2312()
{
	return SC_CHARSET_GB2312;
}

long CScintillaCtrl::GetSC_CHARSET_GREEK()
{
	return SC_CHARSET_GREEK;
}

long CScintillaCtrl::GetSC_CHARSET_HANGUL()
{
	return SC_CHARSET_HANGUL;
}

long CScintillaCtrl::GetSC_CHARSET_MAC()
{
	return SC_CHARSET_MAC;
}

long CScintillaCtrl::GetSC_CHARSET_OEM()
{
	return SC_CHARSET_OEM;
}

long CScintillaCtrl::GetSC_CHARSET_RUSSIAN()
{
	return SC_CHARSET_RUSSIAN;
}

long CScintillaCtrl::GetSC_CHARSET_SHIFTJIS()
{
	return SC_CHARSET_SHIFTJIS;
}

long CScintillaCtrl::GetSC_CHARSET_SYMBOL()
{
	return SC_CHARSET_SYMBOL;
}

long CScintillaCtrl::GetSC_CHARSET_TURKISH()
{
	return SC_CHARSET_TURKISH;
}

long CScintillaCtrl::GetSC_CHARSET_JOHAB()
{
	return SC_CHARSET_JOHAB;
}

long CScintillaCtrl::GetSC_CHARSET_HEBREW()
{
	return SC_CHARSET_HEBREW;
}

long CScintillaCtrl::GetSC_CHARSET_ARABIC()
{
	return SC_CHARSET_ARABIC;
}

long CScintillaCtrl::GetSC_CHARSET_VIETNAMESE()
{
	return SC_CHARSET_VIETNAMESE;
}

long CScintillaCtrl::GetSC_CHARSET_THAI()
{
	return SC_CHARSET_THAI;
}

long CScintillaCtrl::GetINDIC_MAX()
{
	return INDIC_MAX;
}

long CScintillaCtrl::GetINDIC_PLAIN()
{
	return INDIC_PLAIN;
}

long CScintillaCtrl::GetINDIC_SQUIGGLE()
{
	return INDIC_SQUIGGLE;
}

long CScintillaCtrl::GetINDIC_TT()
{
	return INDIC_TT;
}

long CScintillaCtrl::GetINDIC_DIAGONAL()
{
	return INDIC_DIAGONAL;
}

long CScintillaCtrl::GetINDIC_STRIKE()
{
	return INDIC_STRIKE;
}

long CScintillaCtrl::GetINDIC0_MASK()
{
	return INDIC0_MASK;
}

long CScintillaCtrl::GetINDIC1_MASK()
{
	return INDIC1_MASK;
}

long CScintillaCtrl::GetINDIC2_MASK()
{
	return INDIC2_MASK;
}

long CScintillaCtrl::GetINDICS_MASK()
{
	return INDICS_MASK;
}

long CScintillaCtrl::GetSC_PRINT_NORMAL()
{
	return SC_PRINT_NORMAL;
}

long CScintillaCtrl::GetSC_PRINT_INVERTLIGHT()
{
	return SC_PRINT_INVERTLIGHT;
}

long CScintillaCtrl::GetSC_PRINT_BLACKONWHITE()
{
	return SC_PRINT_BLACKONWHITE;
}

long CScintillaCtrl::GetSCFIND_MATCHCASE()
{
	return SCFIND_MATCHCASE;
}

long CScintillaCtrl::GetSCFIND_WHOLEWORD()
{
	return SCFIND_WHOLEWORD;
}

long CScintillaCtrl::GetSCFIND_DOWN()
{
	return SCFIND_DOWN;
}

long CScintillaCtrl::GetSC_FOLDLEVELBASE()
{
	return SC_FOLDLEVELBASE;
}

long CScintillaCtrl::GetSC_FOLDLEVELWHITEFLAG()
{
	return SC_FOLDLEVELWHITEFLAG;
}

long CScintillaCtrl::GetSC_FOLDLEVELHEADERFLAG()
{
	return SC_FOLDLEVELHEADERFLAG;
}

long CScintillaCtrl::GetSC_FOLDLEVELNUMBERMASK()
{
	return SC_FOLDLEVELNUMBERMASK;
}

long CScintillaCtrl::GetEDGE_NONE()
{
	return EDGE_NONE;
}

long CScintillaCtrl::GetEDGE_LINE()
{
	return EDGE_LINE;
}

long CScintillaCtrl::GetEDGE_BACKGROUND()
{
	return EDGE_BACKGROUND;
}

long CScintillaCtrl::GetCARET_SLOP()
{
	return CARET_SLOP;
}

long CScintillaCtrl::GetCARET_CENTER()
{
	return CARET_CENTER;
}

long CScintillaCtrl::GetCARET_STRICT()
{
	return CARET_STRICT;
}

long CScintillaCtrl::GetSC_MOD_INSERTTEXT()
{
	return SC_MOD_INSERTTEXT;
}

long CScintillaCtrl::GetSC_MOD_DELETETEXT()
{
	return SC_MOD_DELETETEXT;
}

long CScintillaCtrl::GetSC_MOD_CHANGESTYLE()
{
	return SC_MOD_CHANGESTYLE;
}

long CScintillaCtrl::GetSC_MOD_CHANGEFOLD()
{
	return SC_MOD_CHANGEFOLD;
}

long CScintillaCtrl::GetSC_PERFORMED_USER()
{
	return SC_PERFORMED_USER;
}

long CScintillaCtrl::GetSC_PERFORMED_UNDO()
{
	return SC_PERFORMED_UNDO;
}

long CScintillaCtrl::GetSC_PERFORMED_REDO()
{
	return SC_PERFORMED_REDO;
}

long CScintillaCtrl::GetSC_LASTSTEPINUNDOREDO()
{
	return SC_LASTSTEPINUNDOREDO;
}

long CScintillaCtrl::GetSC_MOD_CHANGEMARKER()
{
	return SC_MOD_CHANGEMARKER;
}

long CScintillaCtrl::GetSC_MOD_BEFOREINSERT()
{
	return SC_MOD_BEFOREINSERT;
}

long CScintillaCtrl::GetSC_MOD_BEFOREDELETE()
{
	return SC_MOD_BEFOREDELETE;
}

long CScintillaCtrl::GetSC_MODEVENTMASKALL()
{
	return SC_MODEVENTMASKALL;
}

long CScintillaCtrl::GetSCN_STYLENEEDED()
{
	return SCN_STYLENEEDED;
}

long CScintillaCtrl::GetSCN_CHARADDED()
{
	return SCN_CHARADDED;
}

long CScintillaCtrl::GetSCN_SAVEPOINTREACHED()
{
	return SCN_SAVEPOINTREACHED;
}

long CScintillaCtrl::GetSCN_SAVEPOINTLEFT()
{
	return SCN_SAVEPOINTLEFT;
}

long CScintillaCtrl::GetSCN_MODIFYATTEMPTRO()
{
	return SCN_MODIFYATTEMPTRO;
}

long CScintillaCtrl::GetSCN_KEY()
{
	return SCN_KEY;
}

long CScintillaCtrl::GetSCN_DOUBLECLICK()
{
	return SCN_DOUBLECLICK;
}

long CScintillaCtrl::GetSCN_UPDATEUI()
{
	return SCN_UPDATEUI;
}

long CScintillaCtrl::GetSCN_CHECKBRACE()
{
	return SCN_CHECKBRACE;
}

long CScintillaCtrl::GetSCN_MODIFIED()
{
	return SCN_MODIFIED;
}

long CScintillaCtrl::GetSCN_MACRORECORD()
{
	return SCN_MACRORECORD;
}

long CScintillaCtrl::GetSCN_MARGINCLICK()
{
	return SCN_MARGINCLICK;
}

long CScintillaCtrl::GetSCN_NEEDSHOWN()
{
	return SCN_NEEDSHOWN;
}

long CScintillaCtrl::GetSCEN_CHANGE()
{
	return SCEN_CHANGE;
}

long CScintillaCtrl::GetSCEN_SETFOCUS()
{
	return SCEN_SETFOCUS;
}

long CScintillaCtrl::GetSCEN_KILLFOCUS()
{
	return SCEN_KILLFOCUS;
}

long CScintillaCtrl::GetSCK_DOWN()
{
	return SCK_DOWN;
}

long CScintillaCtrl::GetSCK_UP()
{
	return SCK_UP;
}

long CScintillaCtrl::GetSCK_LEFT()
{
	return SCK_LEFT;
}

long CScintillaCtrl::GetSCK_RIGHT()
{
	return SCK_RIGHT;
}

long CScintillaCtrl::GetSCK_HOME()
{
	return SCK_HOME;
}

long CScintillaCtrl::GetSCK_END()
{
	return SCK_END;
}

long CScintillaCtrl::GetSCK_PRIOR()
{
	return SCK_PRIOR;
}

long CScintillaCtrl::GetSCK_NEXT()
{
	return SCK_NEXT;
}

long CScintillaCtrl::GetSCK_DELETE()
{
	return SCK_DELETE;
}

long CScintillaCtrl::GetSCK_INSERT()
{
	return SCK_INSERT;
}

long CScintillaCtrl::GetSCK_ESCAPE()
{
	return SCK_ESCAPE;
}

long CScintillaCtrl::GetSCK_BACK()
{
	return SCK_BACK;
}

long CScintillaCtrl::GetSCK_TAB()
{
	return SCK_TAB;
}

long CScintillaCtrl::GetSCK_RETURN()
{
	return SCK_RETURN;
}

long CScintillaCtrl::GetSCK_ADD()
{
	return SCK_ADD;
}

long CScintillaCtrl::GetSCK_SUBTRACT()
{
	return SCK_SUBTRACT;
}

long CScintillaCtrl::GetSCK_DIVIDE()
{
	return SCK_DIVIDE;
}

long CScintillaCtrl::GetSCMOD_SHIFT()
{
	return SCMOD_SHIFT;
}

long CScintillaCtrl::GetSCMOD_CTRL()
{
	return SCMOD_CTRL;
}

long CScintillaCtrl::GetSCMOD_ALT()
{
	return SCMOD_ALT;
}

long CScintillaCtrl::GetSCLEX_CONTAINER()
{
	return SCLEX_CONTAINER;
}

long CScintillaCtrl::GetSCLEX_NULL()
{
	return SCLEX_NULL;
}

long CScintillaCtrl::GetSCLEX_PYTHON()
{
	return SCLEX_PYTHON;
}

long CScintillaCtrl::GetSCLEX_CPP()
{
	return SCLEX_CPP;
}

long CScintillaCtrl::GetSCLEX_HTML()
{
	return SCLEX_HTML;
}

long CScintillaCtrl::GetSCLEX_XML()
{
	return SCLEX_XML;
}

long CScintillaCtrl::GetSCLEX_PERL()
{
	return SCLEX_PERL;
}

long CScintillaCtrl::GetSCLEX_SQL()
{
	return SCLEX_SQL;
}

long CScintillaCtrl::GetSCLEX_VB()
{
	return SCLEX_VB;
}

long CScintillaCtrl::GetSCLEX_PROPERTIES()
{
	return SCLEX_PROPERTIES;
}

long CScintillaCtrl::GetSCLEX_ERRORLIST()
{
	return SCLEX_ERRORLIST;
}

long CScintillaCtrl::GetSCLEX_MAKEFILE()
{
	return SCLEX_MAKEFILE;
}

long CScintillaCtrl::GetSCLEX_BATCH()
{
	return SCLEX_BATCH;
}

long CScintillaCtrl::GetSCLEX_XCODE()
{
	return SCLEX_XCODE;
}

long CScintillaCtrl::GetSCLEX_LATEX()
{
	return SCLEX_LATEX;
}

long CScintillaCtrl::GetSCLEX_LUA()
{
	return SCLEX_LUA;
}

long CScintillaCtrl::GetSCE_P_DEFAULT()
{
	return SCE_P_DEFAULT;
}

long CScintillaCtrl::GetSCE_P_COMMENTLINE()
{
	return SCE_P_COMMENTLINE;
}

long CScintillaCtrl::GetSCE_P_NUMBER()
{
	return SCE_P_NUMBER;
}

long CScintillaCtrl::GetSCE_P_STRING()
{
	return SCE_P_STRING;
}

long CScintillaCtrl::GetSCE_P_CHARACTER()
{
	return SCE_P_CHARACTER;
}

long CScintillaCtrl::GetSCE_P_WORD()
{
	return SCE_P_WORD;
}

long CScintillaCtrl::GetSCE_P_TRIPLE()
{
	return SCE_P_TRIPLE;
}

long CScintillaCtrl::GetSCE_P_TRIPLEDOUBLE()
{
	return SCE_P_TRIPLEDOUBLE;
}

long CScintillaCtrl::GetSCE_P_CLASSNAME()
{
	return SCE_P_CLASSNAME;
}

long CScintillaCtrl::GetSCE_P_DEFNAME()
{
	return SCE_P_DEFNAME;
}

long CScintillaCtrl::GetSCE_P_OPERATOR()
{
	return SCE_P_OPERATOR;
}

long CScintillaCtrl::GetSCE_P_IDENTIFIER()
{
	return SCE_P_IDENTIFIER;
}

long CScintillaCtrl::GetSCE_P_COMMENTBLOCK()
{
	return SCE_P_COMMENTBLOCK;
}

long CScintillaCtrl::GetSCE_P_STRINGEOL()
{
	return SCE_P_STRINGEOL;
}

long CScintillaCtrl::GetSCE_C_DEFAULT()
{
	return SCE_C_DEFAULT;
}

long CScintillaCtrl::GetSCE_C_COMMENT()
{
	return SCE_C_COMMENT;
}

long CScintillaCtrl::GetSCE_C_COMMENTLINE()
{
	return SCE_C_COMMENTLINE;
}

long CScintillaCtrl::GetSCE_C_COMMENTDOC()
{
	return SCE_C_COMMENTDOC;
}

long CScintillaCtrl::GetSCE_C_NUMBER()
{
	return SCE_C_NUMBER;
}

long CScintillaCtrl::GetSCE_C_WORD()
{
	return SCE_C_WORD;
}

long CScintillaCtrl::GetSCE_C_STRING()
{
	return SCE_C_STRING;
}

long CScintillaCtrl::GetSCE_C_CHARACTER()
{
	return SCE_C_CHARACTER;
}

long CScintillaCtrl::GetSCE_C_UUID()
{
	return SCE_C_UUID;
}

long CScintillaCtrl::GetSCE_C_PREPROCESSOR()
{
	return SCE_C_PREPROCESSOR;
}

long CScintillaCtrl::GetSCE_C_OPERATOR()
{
	return SCE_C_OPERATOR;
}

long CScintillaCtrl::GetSCE_C_IDENTIFIER()
{
	return SCE_C_IDENTIFIER;
}

long CScintillaCtrl::GetSCE_C_STRINGEOL()
{
	return SCE_C_STRINGEOL;
}

long CScintillaCtrl::GetSCE_H_DEFAULT()
{
	return SCE_H_DEFAULT;
}

long CScintillaCtrl::GetSCE_H_TAG()
{
	return SCE_H_TAG;
}

long CScintillaCtrl::GetSCE_H_TAGUNKNOWN()
{
	return SCE_H_TAGUNKNOWN;
}

long CScintillaCtrl::GetSCE_H_ATTRIBUTE()
{
	return SCE_H_ATTRIBUTE;
}

long CScintillaCtrl::GetSCE_H_ATTRIBUTEUNKNOWN()
{
	return SCE_H_ATTRIBUTEUNKNOWN;
}

long CScintillaCtrl::GetSCE_H_NUMBER()
{
	return SCE_H_NUMBER;
}

long CScintillaCtrl::GetSCE_H_DOUBLESTRING()
{
	return SCE_H_DOUBLESTRING;
}

long CScintillaCtrl::GetSCE_H_SINGLESTRING()
{
	return SCE_H_SINGLESTRING;
}

long CScintillaCtrl::GetSCE_H_OTHER()
{
	return SCE_H_OTHER;
}

long CScintillaCtrl::GetSCE_H_COMMENT()
{
	return SCE_H_COMMENT;
}

long CScintillaCtrl::GetSCE_H_ENTITY()
{
	return SCE_H_ENTITY;
}

long CScintillaCtrl::GetSCE_H_TAGEND()
{
	return SCE_H_TAGEND;
}

long CScintillaCtrl::GetSCE_H_XMLSTART()
{
	return SCE_H_XMLSTART;
}

long CScintillaCtrl::GetSCE_H_XMLEND()
{
	return SCE_H_XMLEND;
}

long CScintillaCtrl::GetSCE_H_SCRIPT()
{
	return SCE_H_SCRIPT;
}

long CScintillaCtrl::GetSCE_H_ASP()
{
	return SCE_H_ASP;
}

long CScintillaCtrl::GetSCE_H_ASPAT()
{
	return SCE_H_ASPAT;
}

long CScintillaCtrl::GetSCE_H_CDATA()
{
	return SCE_H_CDATA;
}

long CScintillaCtrl::GetSCE_H_QUESTION()
{
	return SCE_H_QUESTION;
}

long CScintillaCtrl::GetSCE_HJ_START()
{
	return SCE_HJ_START;
}

long CScintillaCtrl::GetSCE_HJ_DEFAULT()
{
	return SCE_HJ_DEFAULT;
}

long CScintillaCtrl::GetSCE_HJ_COMMENT()
{
	return SCE_HJ_COMMENT;
}

long CScintillaCtrl::GetSCE_HJ_COMMENTLINE()
{
	return SCE_HJ_COMMENTLINE;
}

long CScintillaCtrl::GetSCE_HJ_COMMENTDOC()
{
	return SCE_HJ_COMMENTDOC;
}

long CScintillaCtrl::GetSCE_HJ_NUMBER()
{
	return SCE_HJ_NUMBER;
}

long CScintillaCtrl::GetSCE_HJ_WORD()
{
	return SCE_HJ_WORD;
}

long CScintillaCtrl::GetSCE_HJ_KEYWORD()
{
	return SCE_HJ_KEYWORD;
}

long CScintillaCtrl::GetSCE_HJ_DOUBLESTRING()
{
	return SCE_HJ_DOUBLESTRING;
}

long CScintillaCtrl::GetSCE_HJ_SINGLESTRING()
{
	return SCE_HJ_SINGLESTRING;
}

long CScintillaCtrl::GetSCE_HJ_SYMBOLS()
{
	return SCE_HJ_SYMBOLS;
}

long CScintillaCtrl::GetSCE_HJ_STRINGEOL()
{
	return SCE_HJ_STRINGEOL;
}

long CScintillaCtrl::GetSCE_HJA_START()
{
	return SCE_HJA_START;
}

long CScintillaCtrl::GetSCE_HJA_DEFAULT()
{
	return SCE_HJA_DEFAULT;
}

long CScintillaCtrl::GetSCE_HJA_COMMENT()
{
	return SCE_HJA_COMMENT;
}

long CScintillaCtrl::GetSCE_HJA_COMMENTLINE()
{
	return SCE_HJA_COMMENTLINE;
}

long CScintillaCtrl::GetSCE_HJA_COMMENTDOC()
{
	return SCE_HJA_COMMENTDOC;
}

long CScintillaCtrl::GetSCE_HJA_NUMBER()
{
	return SCE_HJA_NUMBER;
}

long CScintillaCtrl::GetSCE_HJA_WORD()
{
	return SCE_HJA_WORD;
}

long CScintillaCtrl::GetSCE_HJA_KEYWORD()
{
	return SCE_HJA_KEYWORD;
}

long CScintillaCtrl::GetSCE_HJA_DOUBLESTRING()
{
	return SCE_HJA_DOUBLESTRING;
}

long CScintillaCtrl::GetSCE_HJA_SINGLESTRING()
{
	return SCE_HJA_SINGLESTRING;
}

long CScintillaCtrl::GetSCE_HJA_SYMBOLS()
{
	return SCE_HJA_SYMBOLS;
}

long CScintillaCtrl::GetSCE_HJA_STRINGEOL()
{
	return SCE_HJA_STRINGEOL;
}

long CScintillaCtrl::GetSCE_HB_START()
{
	return SCE_HB_START;
}

long CScintillaCtrl::GetSCE_HB_DEFAULT()
{
	return SCE_HB_DEFAULT;
}

long CScintillaCtrl::GetSCE_HB_COMMENTLINE()
{
	return SCE_HB_COMMENTLINE;
}

long CScintillaCtrl::GetSCE_HB_NUMBER()
{
	return SCE_HB_NUMBER;
}

long CScintillaCtrl::GetSCE_HB_WORD()
{
	return SCE_HB_WORD;
}

long CScintillaCtrl::GetSCE_HB_STRING()
{
	return SCE_HB_STRING;
}

long CScintillaCtrl::GetSCE_HB_IDENTIFIER()
{
	return SCE_HB_IDENTIFIER;
}

long CScintillaCtrl::GetSCE_HB_STRINGEOL()
{
	return SCE_HB_STRINGEOL;
}

long CScintillaCtrl::GetSCE_HBA_START()
{
	return SCE_HBA_START;
}

long CScintillaCtrl::GetSCE_HBA_DEFAULT()
{
	return SCE_HBA_DEFAULT;
}

long CScintillaCtrl::GetSCE_HBA_COMMENTLINE()
{
	return SCE_HBA_COMMENTLINE;
}

long CScintillaCtrl::GetSCE_HBA_NUMBER()
{
	return SCE_HBA_NUMBER;
}

long CScintillaCtrl::GetSCE_HBA_WORD()
{
	return SCE_HBA_WORD;
}

long CScintillaCtrl::GetSCE_HBA_STRING()
{
	return SCE_HBA_STRING;
}

long CScintillaCtrl::GetSCE_HBA_IDENTIFIER()
{
	return SCE_HBA_IDENTIFIER;
}

long CScintillaCtrl::GetSCE_HBA_STRINGEOL()
{
	return SCE_HBA_STRINGEOL;
}

long CScintillaCtrl::GetSCE_HP_START()
{
	return SCE_HP_START;
}

long CScintillaCtrl::GetSCE_HP_DEFAULT()
{
	return SCE_HP_DEFAULT;
}

long CScintillaCtrl::GetSCE_HP_COMMENTLINE()
{
	return SCE_HP_COMMENTLINE;
}

long CScintillaCtrl::GetSCE_HP_NUMBER()
{
	return SCE_HP_NUMBER;
}

long CScintillaCtrl::GetSCE_HP_STRING()
{
	return SCE_HP_STRING;
}

long CScintillaCtrl::GetSCE_HP_CHARACTER()
{
	return SCE_HP_CHARACTER;
}

long CScintillaCtrl::GetSCE_HP_WORD()
{
	return SCE_HP_WORD;
}

long CScintillaCtrl::GetSCE_HP_TRIPLE()
{
	return SCE_HP_TRIPLE;
}

long CScintillaCtrl::GetSCE_HP_TRIPLEDOUBLE()
{
	return SCE_HP_TRIPLEDOUBLE;
}

long CScintillaCtrl::GetSCE_HP_CLASSNAME()
{
	return SCE_HP_CLASSNAME;
}

long CScintillaCtrl::GetSCE_HP_DEFNAME()
{
	return SCE_HP_DEFNAME;
}

long CScintillaCtrl::GetSCE_HP_OPERATOR()
{
	return SCE_HP_OPERATOR;
}

long CScintillaCtrl::GetSCE_HP_IDENTIFIER()
{
	return SCE_HP_IDENTIFIER;
}

long CScintillaCtrl::GetSCE_HPA_START()
{
	return SCE_HPA_START;
}

long CScintillaCtrl::GetSCE_HPA_DEFAULT()
{
	return SCE_HPA_DEFAULT;
}

long CScintillaCtrl::GetSCE_HPA_COMMENTLINE()
{
	return SCE_HPA_COMMENTLINE;
}

long CScintillaCtrl::GetSCE_HPA_NUMBER()
{
	return SCE_HPA_NUMBER;
}

long CScintillaCtrl::GetSCE_HPA_STRING()
{
	return SCE_HPA_STRING;
}

long CScintillaCtrl::GetSCE_HPA_CHARACTER()
{
	return SCE_HPA_CHARACTER;
}

long CScintillaCtrl::GetSCE_HPA_WORD()
{
	return SCE_HPA_WORD;
}

long CScintillaCtrl::GetSCE_HPA_TRIPLE()
{
	return SCE_HPA_TRIPLE;
}

long CScintillaCtrl::GetSCE_HPA_TRIPLEDOUBLE()
{
	return SCE_HPA_TRIPLEDOUBLE;
}

long CScintillaCtrl::GetSCE_HPA_CLASSNAME()
{
	return SCE_HPA_CLASSNAME;
}

long CScintillaCtrl::GetSCE_HPA_DEFNAME()
{
	return SCE_HPA_DEFNAME;
}

long CScintillaCtrl::GetSCE_HPA_OPERATOR()
{
	return SCE_HPA_OPERATOR;
}

long CScintillaCtrl::GetSCE_HPA_IDENTIFIER()
{
	return SCE_HPA_IDENTIFIER;
}

long CScintillaCtrl::GetSCE_HPHP_DEFAULT()
{
	return SCE_HPHP_DEFAULT;
}

long CScintillaCtrl::GetSCE_HPHP_HSTRING()
{
	return SCE_HPHP_HSTRING;
}

long CScintillaCtrl::GetSCE_HPHP_SIMPLESTRING()
{
	return SCE_HPHP_SIMPLESTRING;
}

long CScintillaCtrl::GetSCE_HPHP_WORD()
{
	return SCE_HPHP_WORD;
}

long CScintillaCtrl::GetSCE_HPHP_NUMBER()
{
	return SCE_HPHP_NUMBER;
}

long CScintillaCtrl::GetSCE_HPHP_VARIABLE()
{
	return SCE_HPHP_VARIABLE;
}

long CScintillaCtrl::GetSCE_HPHP_COMMENT()
{
	return SCE_HPHP_COMMENT;
}

long CScintillaCtrl::GetSCE_HPHP_COMMENTLINE()
{
	return SCE_HPHP_COMMENTLINE;
}

long CScintillaCtrl::GetSCE_HPHP_STRINGEOL()
{
	return SCE_HPHP_STRINGEOL;
}

long CScintillaCtrl::GetSCE_PL_DEFAULT()
{
	return SCE_PL_DEFAULT;
}

long CScintillaCtrl::GetSCE_PL_HERE()
{
	return 0;
}

long CScintillaCtrl::GetSCE_PL_COMMENTLINE()
{
	return SCE_PL_COMMENTLINE;
}

long CScintillaCtrl::GetSCE_PL_POD()
{
	return SCE_PL_POD;
}

long CScintillaCtrl::GetSCE_PL_NUMBER()
{
	return SCE_PL_NUMBER;
}

long CScintillaCtrl::GetSCE_PL_WORD()
{
	return SCE_PL_WORD;
}

long CScintillaCtrl::GetSCE_PL_STRING()
{
	return SCE_PL_STRING;
}

long CScintillaCtrl::GetSCE_PL_CHARACTER()
{
	return SCE_PL_CHARACTER;
}

long CScintillaCtrl::GetSCE_PL_PUNCTUATION()
{
	return SCE_PL_PUNCTUATION;
}

long CScintillaCtrl::GetSCE_PL_PREPROCESSOR()
{
	return SCE_PL_PREPROCESSOR;
}

long CScintillaCtrl::GetSCE_PL_OPERATOR()
{
	return SCE_PL_OPERATOR;
}

long CScintillaCtrl::GetSCE_PL_IDENTIFIER()
{
	return SCE_PL_IDENTIFIER;
}

long CScintillaCtrl::GetSCE_PL_SCALAR()
{
	return SCE_PL_SCALAR;
}

long CScintillaCtrl::GetSCE_PL_ARRAY()
{
	return SCE_PL_ARRAY;
}

long CScintillaCtrl::GetSCE_PL_HASH()
{
	return SCE_PL_HASH;
}

long CScintillaCtrl::GetSCE_PL_SYMBOLTABLE()
{
	return SCE_PL_SYMBOLTABLE;
}

long CScintillaCtrl::GetSCE_PL_REF()
{
	return 0;
}

long CScintillaCtrl::GetSCE_PL_REGEX()
{
	return SCE_PL_REGEX;
}

long CScintillaCtrl::GetSCE_PL_REGSUBST()
{
	return SCE_PL_REGSUBST;
}

long CScintillaCtrl::GetSCE_PL_LONGQUOTE()
{
	return SCE_PL_LONGQUOTE;
}

long CScintillaCtrl::GetSCE_PL_BACKTICKS()
{
	return SCE_PL_BACKTICKS;
}

long CScintillaCtrl::GetSCE_PL_DATASECTION()
{
	return SCE_PL_DATASECTION;
}

long CScintillaCtrl::GetSCE_L_DEFAULT()
{
	return SCE_L_DEFAULT;
}

long CScintillaCtrl::GetSCE_L_COMMAND()
{
	return SCE_L_COMMAND;
}

long CScintillaCtrl::GetSCE_L_TAG()
{
	return SCE_L_TAG;
}

long CScintillaCtrl::GetSCE_L_MATH()
{
	return SCE_L_MATH;
}

long CScintillaCtrl::GetSCE_L_COMMENT()
{
	return SCE_L_COMMENT;
}

long CScintillaCtrl::GetSCE_LUA_DEFAULT()
{
	return SCE_LUA_DEFAULT;
}

long CScintillaCtrl::GetSCE_LUA_COMMENT()
{
	return SCE_LUA_COMMENT;
}

long CScintillaCtrl::GetSCE_LUA_COMMENTLINE()
{
	return SCE_LUA_COMMENTLINE;
}

long CScintillaCtrl::GetSCE_LUA_COMMENTDOC()
{
	return SCE_LUA_COMMENTDOC;
}

long CScintillaCtrl::GetSCE_LUA_NUMBER()
{
	return SCE_LUA_NUMBER;
}

long CScintillaCtrl::GetSCE_LUA_WORD()
{
	return SCE_LUA_WORD;
}

long CScintillaCtrl::GetSCE_LUA_STRING()
{
	return SCE_LUA_STRING;
}

long CScintillaCtrl::GetSCE_LUA_CHARACTER()
{
	return SCE_LUA_CHARACTER;
}

long CScintillaCtrl::GetSCE_LUA_LITERALSTRING()
{
	return SCE_LUA_LITERALSTRING;
}

long CScintillaCtrl::GetSCE_LUA_PREPROCESSOR()
{
	return SCE_LUA_PREPROCESSOR;
}

long CScintillaCtrl::GetSCE_LUA_OPERATOR()
{
	return SCE_LUA_OPERATOR;
}

long CScintillaCtrl::GetSCE_LUA_IDENTIFIER()
{
	return SCE_LUA_IDENTIFIER;
}

long CScintillaCtrl::GetSCE_LUA_STRINGEOL()
{
	return SCE_LUA_STRINGEOL;
}

long CScintillaCtrl::GetEN_CHANGE()
{
	return EN_CHANGE;
}

long CScintillaCtrl::GetEN_KILLFOCUS()
{
	return EN_KILLFOCUS;
}

long CScintillaCtrl::GetEN_SETFOCUS()
{
	return EN_SETFOCUS;
}

long CScintillaCtrl::GetEC_LEFTMARGIN()
{
	return EC_LEFTMARGIN;
}

long CScintillaCtrl::GetEC_RIGHTMARGIN()
{
	return EC_RIGHTMARGIN;
}

long CScintillaCtrl::GetEC_USEFONTINFO()
{
	return EC_USEFONTINFO;
}

long CScintillaCtrl::GetSEL_EMPTY()
{
	return SEL_EMPTY;
}

long CScintillaCtrl::GetSEL_TEXT()
{
	return SEL_TEXT;
}

long CScintillaCtrl::GetFR_MATCHCASE()
{
	return FR_MATCHCASE;
}

long CScintillaCtrl::GetFR_WHOLEWORD()
{
	return FR_WHOLEWORD;
}

long CScintillaCtrl::GetFR_DOWN()
{
	return FR_DOWN;
}

long CScintillaCtrl::GetSHIFT_PRESSED()
{
	return SHIFT_PRESSED;
}

long CScintillaCtrl::GetLEFT_CTRL_PRESSED()
{
	return LEFT_CTRL_PRESSED;
}

long CScintillaCtrl::GetLEFT_ALT_PRESSED()
{
	return LEFT_ALT_PRESSED;
}

void CScintillaCtrl::AddText(long length,LPCTSTR text)
{
	SendMessage(SCI_ADDTEXT,(WPARAM)length,(LPARAM)text);
}

void CScintillaCtrl::AddStyledText(long length,LPCTSTR c)
{
	SendMessage(SCI_ADDSTYLEDTEXT,(WPARAM)length,(LPARAM)c);
}

void CScintillaCtrl::InsertText(long pos,LPCTSTR text)
{
	SendMessage(SCI_INSERTTEXT,(WPARAM)pos,(LPARAM)text);
}

void CScintillaCtrl::ClearAll()
{
	SendMessage(SCI_CLEARALL,(WPARAM)0,(LPARAM)0);
}

void CScintillaCtrl::ClearDocumentStyle()
{
	SendMessage(SCI_CLEARDOCUMENTSTYLE,(WPARAM)0,(LPARAM)0);
}

long CScintillaCtrl::GetLength()
{
	return SendMessage(SCI_GETLENGTH,(WPARAM)0,(LPARAM)0);
}

long CScintillaCtrl::GetCharAt(long pos)
{
	return SendMessage(SCI_GETCHARAT,(WPARAM)pos,(LPARAM)0);
}

long CScintillaCtrl::GetCurrentPos()
{
	return SendMessage(SCI_GETCURRENTPOS,(WPARAM)0,(LPARAM)0);
}

long CScintillaCtrl::GetAnchor()
{
	return SendMessage(SCI_GETANCHOR,(WPARAM)0,(LPARAM)0);
}

long CScintillaCtrl::GetStyleAt(long pos)
{
	return SendMessage(SCI_GETSTYLEAT,(WPARAM)pos,(LPARAM)0);
}

void CScintillaCtrl::Redo()
{
	SendMessage(SCI_REDO,(WPARAM)0,(LPARAM)0);
}

void CScintillaCtrl::SetUndoCollection(BOOL collectUndo)
{
	SendMessage(SCI_SETUNDOCOLLECTION,(WPARAM)collectUndo,(LPARAM)0);
}

void CScintillaCtrl::SelectAll()
{
	SendMessage(SCI_SELECTALL,(WPARAM)0,(LPARAM)0);
}

void CScintillaCtrl::SetSavePoint()
{
	SendMessage(SCI_SETSAVEPOINT,(WPARAM)0,(LPARAM)0);
}

long CScintillaCtrl::GetStyledText(long trMin,long trMax,BSTR FAR* trStr)
{
	long result = 0;
	TEXTRANGE tr;
	tr.chrg.cpMin = trMin;
	tr.chrg.cpMax = trMax;
	if(trMin == 0 && trMax == -1)
		tr.lpstrText = new char[(WM_GetTextLength() + 1) * 2];		
	else
		tr.lpstrText = new char[(trMax - trMin + 1) * 2];
	try
	{
		result = SendMessage(SCI_GETSTYLEDTEXT,(WPARAM)0,(LPARAM)&tr);
		*trStr = SysAllocStringByteLen(tr.lpstrText,result);
	}
	catch(...)
	{
	}
	delete[] tr.lpstrText;
	return result;
}

BOOL CScintillaCtrl::CanRedo()
{
	return SendMessage(SCI_CANREDO,(WPARAM)0,(LPARAM)0);
}

long CScintillaCtrl::MarkerLineFromHandle(long handle)
{
	return SendMessage(SCI_MARKERLINEFROMHANDLE,(WPARAM)handle,(LPARAM)0);
}

void CScintillaCtrl::MarkerDeleteHandle(long handle)
{
	SendMessage(SCI_MARKERDELETEHANDLE,(WPARAM)handle,(LPARAM)0);
}

BOOL CScintillaCtrl::GetUndoCollection()
{
	return SendMessage(SCI_GETUNDOCOLLECTION,(WPARAM)0,(LPARAM)0);
}

long CScintillaCtrl::GetViewWS()
{
	return SendMessage(SCI_GETVIEWWS,(WPARAM)0,(LPARAM)0);
}

void CScintillaCtrl::SetViewWS(long viewWS)
{
	SendMessage(SCI_SETVIEWWS,(WPARAM)viewWS,(LPARAM)0);
}

long CScintillaCtrl::PositionFromPoint(long x,long y)
{
	return SendMessage(SCI_POSITIONFROMPOINT,(WPARAM)x,(LPARAM)y);
}

void CScintillaCtrl::GotoLine(long line)
{
	SendMessage(SCI_GOTOLINE,(WPARAM)line,(LPARAM)0);
}

void CScintillaCtrl::GotoPos(long pos)
{
	SendMessage(SCI_GOTOPOS,(WPARAM)pos,(LPARAM)0);
}

void CScintillaCtrl::SetAnchor(long posAnchor)
{
	SendMessage(SCI_SETANCHOR,(WPARAM)posAnchor,(LPARAM)0);
}

long CScintillaCtrl::GetCurLine(long length,BSTR FAR* textStr)
{
	long result = 0;
	char* text = new char[length + 1];
	try
	{
		result = SendMessage(SCI_GETCURLINE,(WPARAM)length,(LPARAM)text);
		*textStr = SysAllocStringByteLen(text,length);
	}
	catch(...)
	{
	}
	delete[] text;
	return result;
}

long CScintillaCtrl::GetEndStyled()
{
	return SendMessage(SCI_GETENDSTYLED,(WPARAM)0,(LPARAM)0);
}

void CScintillaCtrl::ConvertEOLs()
{
	SendMessage(SCI_CONVERTEOLS,(WPARAM)0,(LPARAM)0);
}

long CScintillaCtrl::GetEOLMode()
{
	return SendMessage(SCI_GETEOLMODE,(WPARAM)0,(LPARAM)0);
}

void CScintillaCtrl::SetEOLMode(long eolMode)
{
	SendMessage(SCI_SETEOLMODE,(WPARAM)eolMode,(LPARAM)0);
}

void CScintillaCtrl::StartStyling(long pos,long mask)
{
	SendMessage(SCI_STARTSTYLING,(WPARAM)pos,(LPARAM)mask);
}

void CScintillaCtrl::SetStyling(long length,long style)
{
	SendMessage(SCI_SETSTYLING,(WPARAM)length,(LPARAM)style);
}

BOOL CScintillaCtrl::GetBufferedDraw()
{
	return SendMessage(SCI_GETBUFFEREDDRAW,(WPARAM)0,(LPARAM)0);
}

void CScintillaCtrl::SetBufferedDraw(BOOL buffered)
{
	SendMessage(SCI_SETBUFFEREDDRAW,(WPARAM)buffered,(LPARAM)0);
}

void CScintillaCtrl::SetTabWidth(long tabWidth)
{
	SendMessage(SCI_SETTABWIDTH,(WPARAM)tabWidth,(LPARAM)0);
}

long CScintillaCtrl::GetTabWidth()
{
	return SendMessage(SCI_GETTABWIDTH,(WPARAM)0,(LPARAM)0);
}

void CScintillaCtrl::SetCodePage(long codePage)
{
	SendMessage(SCI_SETCODEPAGE,(WPARAM)codePage,(LPARAM)0);
}

void CScintillaCtrl::SetUsePalette(BOOL usePalette)
{
	SendMessage(SCI_SETUSEPALETTE,(WPARAM)usePalette,(LPARAM)0);
}

void CScintillaCtrl::MarkerDefine(long markerNumber,long markerSymbol)
{
	SendMessage(SCI_MARKERDEFINE,(WPARAM)markerNumber,(LPARAM)markerSymbol);
}

void CScintillaCtrl::MarkerSetFore(long markerNumber,long fore)
{
	SendMessage(SCI_MARKERSETFORE,(WPARAM)markerNumber,(LPARAM)fore);
}

void CScintillaCtrl::MarkerSetBack(long markerNumber,long back)
{
	SendMessage(SCI_MARKERSETBACK,(WPARAM)markerNumber,(LPARAM)back);
}

void CScintillaCtrl::MarkerAdd(long line,long markerNumber)
{
	SendMessage(SCI_MARKERADD,(WPARAM)line,(LPARAM)markerNumber);
}

void CScintillaCtrl::MarkerDelete(long line,long markerNumber)
{
	SendMessage(SCI_MARKERDELETE,(WPARAM)line,(LPARAM)markerNumber);
}

void CScintillaCtrl::MarkerDeleteAll(long markerNumber)
{
	SendMessage(SCI_MARKERDELETEALL,(WPARAM)markerNumber,(LPARAM)0);
}

long CScintillaCtrl::MarkerGet(long line)
{
	return SendMessage(SCI_MARKERGET,(WPARAM)line,(LPARAM)0);
}

long CScintillaCtrl::MarkerNext(long lineStart,long markerMask)
{
	return SendMessage(SCI_MARKERNEXT,(WPARAM)lineStart,(LPARAM)markerMask);
}

long CScintillaCtrl::MarkerPrevious(long lineStart,long markerMask)
{
	return SendMessage(SCI_MARKERPREVIOUS,(WPARAM)lineStart,(LPARAM)markerMask);
}

void CScintillaCtrl::SetMarginTypeN(long margin,long marginType)
{
	SendMessage(SCI_SETMARGINTYPEN,(WPARAM)margin,(LPARAM)marginType);
}

long CScintillaCtrl::GetMarginTypeN(long margin)
{
	return SendMessage(SCI_GETMARGINTYPEN,(WPARAM)margin,(LPARAM)0);
}

void CScintillaCtrl::SetMarginWidthN(long margin,long pixelWidth)
{
	SendMessage(SCI_SETMARGINWIDTHN,(WPARAM)margin,(LPARAM)pixelWidth);
}

long CScintillaCtrl::GetMarginWidthN(long margin)
{
	return SendMessage(SCI_GETMARGINWIDTHN,(WPARAM)margin,(LPARAM)0);
}

void CScintillaCtrl::SetMarginMaskN(long margin,long mask)
{
	SendMessage(SCI_SETMARGINMASKN,(WPARAM)margin,(LPARAM)mask);
}

long CScintillaCtrl::GetMarginMaskN(long margin)
{
	return SendMessage(SCI_GETMARGINMASKN,(WPARAM)margin,(LPARAM)0);
}

void CScintillaCtrl::SetMarginSensitiveN(long margin,BOOL sensitive)
{
	SendMessage(SCI_SETMARGINSENSITIVEN,(WPARAM)margin,(LPARAM)sensitive);
}

BOOL CScintillaCtrl::GetMarginSensitiveN(long margin)
{
	return SendMessage(SCI_GETMARGINSENSITIVEN,(WPARAM)margin,(LPARAM)0);
}

void CScintillaCtrl::StyleClearAll()
{
	SendMessage(SCI_STYLECLEARALL,(WPARAM)0,(LPARAM)0);
}

void CScintillaCtrl::StyleSetFore(long style,long fore)
{
	SendMessage(SCI_STYLESETFORE,(WPARAM)style,(LPARAM)fore);
}

void CScintillaCtrl::StyleSetBack(long style,long back)
{
	SendMessage(SCI_STYLESETBACK,(WPARAM)style,(LPARAM)back);
}

void CScintillaCtrl::StyleSetBold(long style,BOOL bold)
{
	SendMessage(SCI_STYLESETBOLD,(WPARAM)style,(LPARAM)bold);
}

void CScintillaCtrl::StyleSetItalic(long style,BOOL italic)
{
	SendMessage(SCI_STYLESETITALIC,(WPARAM)style,(LPARAM)italic);
}

void CScintillaCtrl::StyleSetSize(long style,long sizePoints)
{
	SendMessage(SCI_STYLESETSIZE,(WPARAM)style,(LPARAM)sizePoints);
}

void CScintillaCtrl::StyleSetFont(long style,LPCTSTR fontName)
{
	SendMessage(SCI_STYLESETFONT,(WPARAM)style,(LPARAM)fontName);
}

void CScintillaCtrl::StyleSetEOLFilled(long style,BOOL filled)
{
	SendMessage(SCI_STYLESETEOLFILLED,(WPARAM)style,(LPARAM)filled);
}

void CScintillaCtrl::StyleResetDefault()
{
	SendMessage(SCI_STYLERESETDEFAULT,(WPARAM)0,(LPARAM)0);
}

void CScintillaCtrl::StyleSetUnderline(long style,BOOL underline)
{
	SendMessage(SCI_STYLESETUNDERLINE,(WPARAM)style,(LPARAM)underline);
}

void CScintillaCtrl::StyleSetCharacterSet(long style,long characterSet)
{
	SendMessage(SCI_STYLESETCHARACTERSET,(WPARAM)style,(LPARAM)characterSet);
}

void CScintillaCtrl::SetSelFore(BOOL useSetting,long fore)
{
	SendMessage(SCI_SETSELFORE,(WPARAM)useSetting,(LPARAM)fore);
}

void CScintillaCtrl::SetSelBack(BOOL useSetting,long back)
{
	SendMessage(SCI_SETSELBACK,(WPARAM)useSetting,(LPARAM)back);
}

void CScintillaCtrl::SetCaretFore(long fore)
{
	SendMessage(SCI_SETCARETFORE,(WPARAM)fore,(LPARAM)0);
}

void CScintillaCtrl::AssignCmdKey(short kmKey,short kmMod,long msg)
{
	long km = ((long)kmMod << 16) | kmKey; // Key in the low word, modifiers in the high
	SendMessage(SCI_ASSIGNCMDKEY,(WPARAM)km,(LPARAM)msg);
}

void CScintillaCtrl::ClearCmdKey(short kmKey,short kmMod)
{
	long km = ((long)kmMod << 16) | kmKey; // Key in the low word, modifiers in the high
	SendMessage(SCI_CLEARCMDKEY,(WPARAM)km,(LPARAM)0);
}

void CScintillaCtrl::ClearAllCmdKeys()
{
	SendMessage(SCI_CLEARALLCMDKEYS,(WPARAM)0,(LPARAM)0);
}

void CScintillaCtrl::SetStylingEx(long length,LPCTSTR styles)
{
	SendMessage(SCI_SETSTYLINGEX,(WPARAM)length,(LPARAM)styles);
}

long CScintillaCtrl::GetCaretPeriod()
{
	return SendMessage(SCI_GETCARETPERIOD,(WPARAM)0,(LPARAM)0);
}

void CScintillaCtrl::SetCaretPeriod(long periodMilliseconds)
{
	SendMessage(SCI_SETCARETPERIOD,(WPARAM)periodMilliseconds,(LPARAM)0);
}

void CScintillaCtrl::SetWordChars(LPCTSTR characters)
{
	SendMessage(SCI_SETWORDCHARS,(WPARAM)0,(LPARAM)characters);
}

void CScintillaCtrl::BeginUndoAction()
{
	SendMessage(SCI_BEGINUNDOACTION,(WPARAM)0,(LPARAM)0);
}

void CScintillaCtrl::EndUndoAction()
{
	SendMessage(SCI_ENDUNDOACTION,(WPARAM)0,(LPARAM)0);
}

void CScintillaCtrl::IndicSetStyle(long indic,long style)
{
	SendMessage(SCI_INDICSETSTYLE,(WPARAM)indic,(LPARAM)style);
}

long CScintillaCtrl::IndicGetStyle(long indic)
{
	return SendMessage(SCI_INDICGETSTYLE,(WPARAM)indic,(LPARAM)0);
}

void CScintillaCtrl::IndicSetFore(long indic,long fore)
{
	SendMessage(SCI_INDICSETFORE,(WPARAM)indic,(LPARAM)fore);
}

long CScintillaCtrl::IndicGetFore(long indic)
{
	return SendMessage(SCI_INDICGETFORE,(WPARAM)indic,(LPARAM)0);
}

void CScintillaCtrl::SetStyleBits(long bits)
{
	SendMessage(SCI_SETSTYLEBITS,(WPARAM)bits,(LPARAM)0);
}

long CScintillaCtrl::GetStyleBits()
{
	return SendMessage(SCI_GETSTYLEBITS,(WPARAM)0,(LPARAM)0);
}

void CScintillaCtrl::SetLineState(long line,long state)
{
	SendMessage(SCI_SETLINESTATE,(WPARAM)line,(LPARAM)state);
}

long CScintillaCtrl::GetLineState(long line)
{
	return SendMessage(SCI_GETLINESTATE,(WPARAM)line,(LPARAM)0);
}

long CScintillaCtrl::GetMaxLineState()
{
	return SendMessage(SCI_GETMAXLINESTATE,(WPARAM)0,(LPARAM)0);
}

void CScintillaCtrl::AutoCShow(long lenEntered,LPCTSTR itemList)
{
	SendMessage(SCI_AUTOCSHOW,(WPARAM)lenEntered,(LPARAM)itemList);
}

void CScintillaCtrl::AutoCCancel()
{
	SendMessage(SCI_AUTOCCANCEL,(WPARAM)0,(LPARAM)0);
}

BOOL CScintillaCtrl::AutoCActive()
{
	return SendMessage(SCI_AUTOCACTIVE,(WPARAM)0,(LPARAM)0);
}

long CScintillaCtrl::AutoCPosStart()
{
	return SendMessage(SCI_AUTOCPOSSTART,(WPARAM)0,(LPARAM)0);
}

void CScintillaCtrl::AutoCComplete()
{
	SendMessage(SCI_AUTOCCOMPLETE,(WPARAM)0,(LPARAM)0);
}

void CScintillaCtrl::AutoCStops(LPCTSTR characterSet)
{
	SendMessage(SCI_AUTOCSTOPS,(WPARAM)0,(LPARAM)characterSet);
}

void CScintillaCtrl::AutoCSetSeparator(long separatorCharacter)
{
	SendMessage(SCI_AUTOCSETSEPARATOR,(WPARAM)separatorCharacter,(LPARAM)0);
}

long CScintillaCtrl::AutoCGetSeparator()
{
	return SendMessage(SCI_AUTOCGETSEPARATOR,(WPARAM)0,(LPARAM)0);
}

void CScintillaCtrl::AutoCSelect(LPCTSTR text)
{
	SendMessage(SCI_AUTOCSELECT,(WPARAM)0,(LPARAM)text);
}

void CScintillaCtrl::AutoCSetCancelAtStart(BOOL cancel)
{
	SendMessage(SCI_AUTOCSETCANCELATSTART,(WPARAM)cancel,(LPARAM)0);
}

BOOL CScintillaCtrl::AutoCGetCancelAtStart()
{
	return SendMessage(SCI_AUTOCGETCANCELATSTART,(WPARAM)0,(LPARAM)0);
}

void CScintillaCtrl::SetIndent(long indentSize)
{
	SendMessage(SCI_SETINDENT,(WPARAM)indentSize,(LPARAM)0);
}

long CScintillaCtrl::GetIndent()
{
	return SendMessage(SCI_GETINDENT,(WPARAM)0,(LPARAM)0);
}

void CScintillaCtrl::SetUseTabs(BOOL useTabs)
{
	SendMessage(SCI_SETUSETABS,(WPARAM)useTabs,(LPARAM)0);
}

BOOL CScintillaCtrl::GetUseTabs()
{
	return SendMessage(SCI_GETUSETABS,(WPARAM)0,(LPARAM)0);
}

void CScintillaCtrl::SetLineIndentation(long line,long indentSize)
{
	SendMessage(SCI_SETLINEINDENTATION,(WPARAM)line,(LPARAM)indentSize);
}

long CScintillaCtrl::GetLineIndentation(long line)
{
	return SendMessage(SCI_GETLINEINDENTATION,(WPARAM)line,(LPARAM)0);
}

long CScintillaCtrl::GetLineIndentPosition(long line)
{
	return SendMessage(SCI_GETLINEINDENTPOSITION,(WPARAM)line,(LPARAM)0);
}

long CScintillaCtrl::GetColumn(long pos)
{
	return SendMessage(SCI_GETCOLUMN,(WPARAM)pos,(LPARAM)0);
}

void CScintillaCtrl::SetHScrollBar(BOOL show)
{
	SendMessage(SCI_SETHSCROLLBAR,(WPARAM)show,(LPARAM)0);
}

BOOL CScintillaCtrl::GetHScrollBar()
{
	return SendMessage(SCI_GETHSCROLLBAR,(WPARAM)0,(LPARAM)0);
}

void CScintillaCtrl::SetIndentationGuides(BOOL show)
{
	SendMessage(SCI_SETINDENTATIONGUIDES,(WPARAM)show,(LPARAM)0);
}

BOOL CScintillaCtrl::GetIndentationGuides()
{
	return SendMessage(SCI_GETINDENTATIONGUIDES,(WPARAM)0,(LPARAM)0);
}

void CScintillaCtrl::SetHighlightGuide(long column)
{
	SendMessage(SCI_SETHIGHLIGHTGUIDE,(WPARAM)column,(LPARAM)0);
}

long CScintillaCtrl::GetHighlightGuide()
{
	return SendMessage(SCI_GETHIGHLIGHTGUIDE,(WPARAM)0,(LPARAM)0);
}

long CScintillaCtrl::GetLineEndPosition(long line)
{
	return SendMessage(SCI_GETLINEENDPOSITION,(WPARAM)line,(LPARAM)0);
}

long CScintillaCtrl::GetCodePage()
{
	return SendMessage(SCI_GETCODEPAGE,(WPARAM)0,(LPARAM)0);
}

long CScintillaCtrl::GetCaretFore()
{
	return SendMessage(SCI_GETCARETFORE,(WPARAM)0,(LPARAM)0);
}

BOOL CScintillaCtrl::GetUsePalette()
{
	return SendMessage(SCI_GETUSEPALETTE,(WPARAM)0,(LPARAM)0);
}

BOOL CScintillaCtrl::GetReadOnly()
{
	return SendMessage(SCI_GETREADONLY,(WPARAM)0,(LPARAM)0);
}

void CScintillaCtrl::SetCurrentPos(long pos)
{
	SendMessage(SCI_SETCURRENTPOS,(WPARAM)pos,(LPARAM)0);
}

void CScintillaCtrl::SetSelectionStart(long pos)
{
	SendMessage(SCI_SETSELECTIONSTART,(WPARAM)pos,(LPARAM)0);
}

long CScintillaCtrl::GetSelectionStart()
{
	return SendMessage(SCI_GETSELECTIONSTART,(WPARAM)0,(LPARAM)0);
}

void CScintillaCtrl::SetSelectionEnd(long pos)
{
	SendMessage(SCI_SETSELECTIONEND,(WPARAM)pos,(LPARAM)0);
}

long CScintillaCtrl::GetSelectionEnd()
{
	return SendMessage(SCI_GETSELECTIONEND,(WPARAM)0,(LPARAM)0);
}

void CScintillaCtrl::SetPrintMagnification(long magnification)
{
	SendMessage(SCI_SETPRINTMAGNIFICATION,(WPARAM)magnification,(LPARAM)0);
}

long CScintillaCtrl::GetPrintMagnification()
{
	return SendMessage(SCI_GETPRINTMAGNIFICATION,(WPARAM)0,(LPARAM)0);
}

void CScintillaCtrl::SetPrintColourMode(long mode)
{
	SendMessage(SCI_SETPRINTCOLOURMODE,(WPARAM)mode,(LPARAM)0);
}

long CScintillaCtrl::GetPrintColourMode()
{
	return SendMessage(SCI_GETPRINTCOLOURMODE,(WPARAM)0,(LPARAM)0);
}

long CScintillaCtrl::FindText(long flags,long ftMin,long ftMax,LPCTSTR ftStr)
{
	FINDTEXT ft;
	ft.chrg.cpMin = ftMin;
	ft.chrg.cpMax = ftMax; 
	ft.lpstrText = const_cast<char*>(ftStr);
	return SendMessage(EM_FINDTEXT,(WPARAM)flags,(LPARAM)&ft);
}

void CScintillaCtrl::FormatRange(BOOL draw,long fr)
{
	SendMessage(SCI_FORMATRANGE,(WPARAM)draw,(LPARAM)fr);
}

long CScintillaCtrl::GetFirstVisibleLine()
{
	return SendMessage(SCI_GETFIRSTVISIBLELINE,(WPARAM)0,(LPARAM)0);
}

long CScintillaCtrl::GetLine(long line,BSTR FAR* textStr)
{
	long result = 0;
	char* text = new char[LineLength(line) + 1];
	try
	{
		result = SendMessage(SCI_GETLINE,(WPARAM)line,(LPARAM)text);
		*textStr = SysAllocStringByteLen(text,result);
	}
	catch(...)
	{
	}
	delete[] text;
	return result;
}

long CScintillaCtrl::GetLineCount()
{
	return SendMessage(SCI_GETLINECOUNT,(WPARAM)0,(LPARAM)0);
}

void CScintillaCtrl::SetMarginLeft(long width)
{
	SendMessage(SCI_SETMARGINLEFT,(WPARAM)0,(LPARAM)width);
}

long CScintillaCtrl::GetMarginLeft()
{
	return SendMessage(SCI_GETMARGINLEFT,(WPARAM)0,(LPARAM)0);
}

void CScintillaCtrl::SetMarginRight(long width)
{
	SendMessage(SCI_SETMARGINRIGHT,(WPARAM)0,(LPARAM)width);
}

long CScintillaCtrl::GetMarginRight()
{
	return SendMessage(SCI_GETMARGINRIGHT,(WPARAM)0,(LPARAM)0);
}

BOOL CScintillaCtrl::GetModify()
{
	return SendMessage(SCI_GETMODIFY,(WPARAM)0,(LPARAM)0);
}

void CScintillaCtrl::SetSel(long start,long end)
{
	SendMessage(SCI_SETSEL,(WPARAM)start,(LPARAM)end);
}

long CScintillaCtrl::GetSelText(BSTR FAR* textStr)
{
	long min = GetSelectionStart();
	long max = GetSelectionEnd();
	long result = 0;
	char* text = new char[max - min + 1];
	try
	{
		result = SendMessage(SCI_GETSELTEXT,(WPARAM)0,(LPARAM)text);
		*textStr = SysAllocStringByteLen(text,result);
	}
	catch(...)
	{
	}
	delete[] text;
	return result;
}

long CScintillaCtrl::GetTextRange(long trMin,long trMax,BSTR FAR* trStr)
{
	long result = 0;
	TEXTRANGE tr;
	tr.chrg.cpMin = trMin;
	tr.chrg.cpMax = trMax;
	if(trMin == 0 && trMax == -1)
		tr.lpstrText = new char[GetTextLength() + 1];		
	else
		tr.lpstrText = new char[trMax - trMin + 1];
	try
	{
		result = SendMessage(SCI_GETTEXTRANGE,(WPARAM)0,(LPARAM)&tr);
		*trStr = SysAllocStringByteLen(tr.lpstrText,result);
	}
	catch(...)
	{
	}
	delete[] tr.lpstrText;
	return result;
}

void CScintillaCtrl::HideSelection(BOOL normal)
{
	SendMessage(SCI_HIDESELECTION,(WPARAM)normal,(LPARAM)0);
}

long CScintillaCtrl::PointXFromPosition(long pos)
{
	return SendMessage(SCI_POINTXFROMPOSITION,(WPARAM)0,(LPARAM)pos);
}

long CScintillaCtrl::PointYFromPosition(long pos)
{
	return SendMessage(SCI_POINTYFROMPOSITION,(WPARAM)0,(LPARAM)pos);
}

long CScintillaCtrl::LineFromPosition(long pos)
{
	return SendMessage(SCI_LINEFROMPOSITION,(WPARAM)pos,(LPARAM)0);
}

long CScintillaCtrl::PositionFromLine(long line)
{
	return SendMessage(SCI_POSITIONFROMLINE,(WPARAM)line,(LPARAM)0);
}

void CScintillaCtrl::LineScroll(long columns,long lines)
{
	SendMessage(SCI_LINESCROLL,(WPARAM)columns,(LPARAM)lines);
}

void CScintillaCtrl::ScrollCaret()
{
	SendMessage(SCI_SCROLLCARET,(WPARAM)0,(LPARAM)0);
}

void CScintillaCtrl::ReplaceSel(LPCTSTR text)
{
	SendMessage(SCI_REPLACESEL,(WPARAM)0,(LPARAM)text);
}

void CScintillaCtrl::SetReadOnly(BOOL readOnly)
{
	SendMessage(SCI_SETREADONLY,(WPARAM)readOnly,(LPARAM)0);
}

void CScintillaCtrl::Null()
{
	SendMessage(SCI_NULL,(WPARAM)0,(LPARAM)0);
}

BOOL CScintillaCtrl::CanPaste()
{
	return SendMessage(SCI_CANPASTE,(WPARAM)0,(LPARAM)0);
}

BOOL CScintillaCtrl::CanUndo()
{
	return SendMessage(SCI_CANUNDO,(WPARAM)0,(LPARAM)0);
}

void CScintillaCtrl::EmptyUndoBuffer()
{
	SendMessage(SCI_EMPTYUNDOBUFFER,(WPARAM)0,(LPARAM)0);
}

void CScintillaCtrl::Undo()
{
	SendMessage(SCI_UNDO,(WPARAM)0,(LPARAM)0);
}

void CScintillaCtrl::Cut()
{
	SendMessage(SCI_CUT,(WPARAM)0,(LPARAM)0);
}

void CScintillaCtrl::Copy()
{
	SendMessage(SCI_COPY,(WPARAM)0,(LPARAM)0);
}

void CScintillaCtrl::Paste()
{
	SendMessage(SCI_PASTE,(WPARAM)0,(LPARAM)0);
}

void CScintillaCtrl::Clear()
{
	SendMessage(SCI_CLEAR,(WPARAM)0,(LPARAM)0);
}

void CScintillaCtrl::SetText(LPCTSTR text)
{
	SendMessage(SCI_SETTEXT,(WPARAM)0,(LPARAM)text);
}

long CScintillaCtrl::GetText(long length,BSTR FAR* textStr)
{
	long result = 0;
	char* text = new char[length + 1];
	try
	{
		result = SendMessage(SCI_GETTEXT,(WPARAM)length,(LPARAM)text);
		*textStr = SysAllocStringByteLen(text,result);
	}
	catch(...)
	{
	}
	delete[] text;
	return result;
}

long CScintillaCtrl::GetTextLength()
{
	return SendMessage(SCI_GETTEXTLENGTH,(WPARAM)0,(LPARAM)0);
}

long CScintillaCtrl::GetDirectFunction()
{
	return SendMessage(SCI_GETDIRECTFUNCTION,(WPARAM)0,(LPARAM)0);
}

long CScintillaCtrl::GetDirectPointer()
{
	return SendMessage(SCI_GETDIRECTPOINTER,(WPARAM)0,(LPARAM)0);
}

void CScintillaCtrl::CallTipShow(long pos,LPCTSTR definition)
{
	SendMessage(SCI_CALLTIPSHOW,(WPARAM)pos,(LPARAM)definition);
}

void CScintillaCtrl::CallTipCancel()
{
	SendMessage(SCI_CALLTIPCANCEL,(WPARAM)0,(LPARAM)0);
}

BOOL CScintillaCtrl::CallTipActive()
{
	return SendMessage(SCI_CALLTIPACTIVE,(WPARAM)0,(LPARAM)0);
}

long CScintillaCtrl::CallTipPosStart()
{
	return SendMessage(SCI_CALLTIPPOSSTART,(WPARAM)0,(LPARAM)0);
}

void CScintillaCtrl::CallTipSetHlt(long start,long end)
{
	SendMessage(SCI_CALLTIPSETHLT,(WPARAM)start,(LPARAM)end);
}

void CScintillaCtrl::CallTipSetBack(long back)
{
	SendMessage(SCI_CALLTIPSETBACK,(WPARAM)back,(LPARAM)0);
}

long CScintillaCtrl::VisibleFromDocLine(long line)
{
	return SendMessage(SCI_VISIBLEFROMDOCLINE,(WPARAM)line,(LPARAM)0);
}

long CScintillaCtrl::DocLineFromVisible(long lineDisplay)
{
	return SendMessage(SCI_DOCLINEFROMVISIBLE,(WPARAM)lineDisplay,(LPARAM)0);
}

void CScintillaCtrl::SetFoldLevel(long line,long level)
{
	SendMessage(SCI_SETFOLDLEVEL,(WPARAM)line,(LPARAM)level);
}

long CScintillaCtrl::GetFoldLevel(long line)
{
	return SendMessage(SCI_GETFOLDLEVEL,(WPARAM)line,(LPARAM)0);
}

long CScintillaCtrl::GetLastChild(long line,long level)
{
	return SendMessage(SCI_GETLASTCHILD,(WPARAM)line,(LPARAM)level);
}

long CScintillaCtrl::GetFoldParent(long line)
{
	return SendMessage(SCI_GETFOLDPARENT,(WPARAM)line,(LPARAM)0);
}

void CScintillaCtrl::ShowLines(long lineStart,long lineEnd)
{
	SendMessage(SCI_SHOWLINES,(WPARAM)lineStart,(LPARAM)lineEnd);
}

void CScintillaCtrl::HideLines(long lineStart,long lineEnd)
{
	SendMessage(SCI_HIDELINES,(WPARAM)lineStart,(LPARAM)lineEnd);
}

BOOL CScintillaCtrl::GetLineVisible(long line)
{
	return SendMessage(SCI_GETLINEVISIBLE,(WPARAM)line,(LPARAM)0);
}

void CScintillaCtrl::SetFoldExpanded(long line,BOOL expanded)
{
	SendMessage(SCI_SETFOLDEXPANDED,(WPARAM)line,(LPARAM)expanded);
}

BOOL CScintillaCtrl::GetFoldExpanded(long line)
{
	return SendMessage(SCI_GETFOLDEXPANDED,(WPARAM)line,(LPARAM)0);
}

void CScintillaCtrl::ToggleFold(long line)
{
	SendMessage(SCI_TOGGLEFOLD,(WPARAM)line,(LPARAM)0);
}

void CScintillaCtrl::EnsureVisible(long line)
{
	SendMessage(SCI_ENSUREVISIBLE,(WPARAM)line,(LPARAM)0);
}

void CScintillaCtrl::SetFoldFlags(long flags)
{
	SendMessage(SCI_SETFOLDFLAGS,(WPARAM)flags,(LPARAM)0);
}

void CScintillaCtrl::LineDown()
{
	SendMessage(SCI_LINEDOWN,(WPARAM)0,(LPARAM)0);
}

void CScintillaCtrl::LineDownExtend()
{
	SendMessage(SCI_LINEDOWNEXTEND,(WPARAM)0,(LPARAM)0);
}

void CScintillaCtrl::LineUp()
{
	SendMessage(SCI_LINEUP,(WPARAM)0,(LPARAM)0);
}

void CScintillaCtrl::LineUpExtend()
{
	SendMessage(SCI_LINEUPEXTEND,(WPARAM)0,(LPARAM)0);
}

void CScintillaCtrl::CharLeft()
{
	SendMessage(SCI_CHARLEFT,(WPARAM)0,(LPARAM)0);
}

void CScintillaCtrl::CharLeftExtend()
{
	SendMessage(SCI_CHARLEFTEXTEND,(WPARAM)0,(LPARAM)0);
}

void CScintillaCtrl::CharRight()
{
	SendMessage(SCI_CHARRIGHT,(WPARAM)0,(LPARAM)0);
}

void CScintillaCtrl::CharRightExtend()
{
	SendMessage(SCI_CHARRIGHTEXTEND,(WPARAM)0,(LPARAM)0);
}

void CScintillaCtrl::WordLeft()
{
	SendMessage(SCI_WORDLEFT,(WPARAM)0,(LPARAM)0);
}

void CScintillaCtrl::WordLeftExtend()
{
	SendMessage(SCI_WORDLEFTEXTEND,(WPARAM)0,(LPARAM)0);
}

void CScintillaCtrl::WordRight()
{
	SendMessage(SCI_WORDRIGHT,(WPARAM)0,(LPARAM)0);
}

void CScintillaCtrl::WordRightExtend()
{
	SendMessage(SCI_WORDRIGHTEXTEND,(WPARAM)0,(LPARAM)0);
}

void CScintillaCtrl::Home()
{
	SendMessage(SCI_HOME,(WPARAM)0,(LPARAM)0);
}

void CScintillaCtrl::HomeExtend()
{
	SendMessage(SCI_HOMEEXTEND,(WPARAM)0,(LPARAM)0);
}

void CScintillaCtrl::LineEnd()
{
	SendMessage(SCI_LINEEND,(WPARAM)0,(LPARAM)0);
}

void CScintillaCtrl::LineEndExtend()
{
	SendMessage(SCI_LINEENDEXTEND,(WPARAM)0,(LPARAM)0);
}

void CScintillaCtrl::DocumentStart()
{
	SendMessage(SCI_DOCUMENTSTART,(WPARAM)0,(LPARAM)0);
}

void CScintillaCtrl::DocumentStartExtend()
{
	SendMessage(SCI_DOCUMENTSTARTEXTEND,(WPARAM)0,(LPARAM)0);
}

void CScintillaCtrl::DocumentEnd()
{
	SendMessage(SCI_DOCUMENTEND,(WPARAM)0,(LPARAM)0);
}

void CScintillaCtrl::DocumentEndExtend()
{
	SendMessage(SCI_DOCUMENTENDEXTEND,(WPARAM)0,(LPARAM)0);
}

void CScintillaCtrl::PageUp()
{
	SendMessage(SCI_PAGEUP,(WPARAM)0,(LPARAM)0);
}

void CScintillaCtrl::PageUpExtend()
{
	SendMessage(SCI_PAGEUPEXTEND,(WPARAM)0,(LPARAM)0);
}

void CScintillaCtrl::PageDown()
{
	SendMessage(SCI_PAGEDOWN,(WPARAM)0,(LPARAM)0);
}

void CScintillaCtrl::PageDownExtend()
{
	SendMessage(SCI_PAGEDOWNEXTEND,(WPARAM)0,(LPARAM)0);
}

void CScintillaCtrl::EditToggleOvertype()
{
	SendMessage(SCI_EDITTOGGLEOVERTYPE,(WPARAM)0,(LPARAM)0);
}

void CScintillaCtrl::Cancel()
{
	SendMessage(SCI_CANCEL,(WPARAM)0,(LPARAM)0);
}

void CScintillaCtrl::DeleteBack()
{
	SendMessage(SCI_DELETEBACK,(WPARAM)0,(LPARAM)0);
}

void CScintillaCtrl::Tab()
{
	SendMessage(SCI_TAB,(WPARAM)0,(LPARAM)0);
}

void CScintillaCtrl::BackTab()
{
	SendMessage(SCI_BACKTAB,(WPARAM)0,(LPARAM)0);
}

void CScintillaCtrl::NewLine()
{
	SendMessage(SCI_NEWLINE,(WPARAM)0,(LPARAM)0);
}

void CScintillaCtrl::FormFeed()
{
	SendMessage(SCI_FORMFEED,(WPARAM)0,(LPARAM)0);
}

void CScintillaCtrl::VCHome()
{
	SendMessage(SCI_VCHOME,(WPARAM)0,(LPARAM)0);
}

void CScintillaCtrl::VCHomeExtend()
{
	SendMessage(SCI_VCHOMEEXTEND,(WPARAM)0,(LPARAM)0);
}

void CScintillaCtrl::ZoomIn()
{
	SendMessage(SCI_ZOOMIN,(WPARAM)0,(LPARAM)0);
}

void CScintillaCtrl::ZoomOut()
{
	SendMessage(SCI_ZOOMOUT,(WPARAM)0,(LPARAM)0);
}

void CScintillaCtrl::DelWordLeft()
{
	SendMessage(SCI_DELWORDLEFT,(WPARAM)0,(LPARAM)0);
}

void CScintillaCtrl::DelWordRight()
{
	SendMessage(SCI_DELWORDRIGHT,(WPARAM)0,(LPARAM)0);
}

void CScintillaCtrl::LineCut()
{
	SendMessage(SCI_LINECUT,(WPARAM)0,(LPARAM)0);
}

void CScintillaCtrl::LineDelete()
{
	SendMessage(SCI_LINEDELETE,(WPARAM)0,(LPARAM)0);
}

void CScintillaCtrl::LineTranspose()
{
	SendMessage(SCI_LINETRANSPOSE,(WPARAM)0,(LPARAM)0);
}

void CScintillaCtrl::LowerCase()
{
	SendMessage(SCI_LOWERCASE,(WPARAM)0,(LPARAM)0);
}

void CScintillaCtrl::UpperCase()
{
	SendMessage(SCI_UPPERCASE,(WPARAM)0,(LPARAM)0);
}

void CScintillaCtrl::LineScrollDown()
{
	SendMessage(SCI_LINESCROLLDOWN,(WPARAM)0,(LPARAM)0);
}

void CScintillaCtrl::LineScrollUp()
{
	SendMessage(SCI_LINESCROLLUP,(WPARAM)0,(LPARAM)0);
}

long CScintillaCtrl::LineLength(long line)
{
	return SendMessage(SCI_LINELENGTH,(WPARAM)line,(LPARAM)0);
}

void CScintillaCtrl::BraceHighlight(long pos1,long pos2)
{
	SendMessage(SCI_BRACEHIGHLIGHT,(WPARAM)pos1,(LPARAM)pos2);
}

void CScintillaCtrl::BraceBadLight(long pos)
{
	SendMessage(SCI_BRACEBADLIGHT,(WPARAM)pos,(LPARAM)0);
}

long CScintillaCtrl::BraceMatch(long pos)
{
	return SendMessage(SCI_BRACEMATCH,(WPARAM)pos,(LPARAM)0);
}

BOOL CScintillaCtrl::GetViewEOL()
{
	return SendMessage(SCI_GETVIEWEOL,(WPARAM)0,(LPARAM)0);
}

void CScintillaCtrl::SetViewEOL(BOOL visible)
{
	SendMessage(SCI_SETVIEWEOL,(WPARAM)visible,(LPARAM)0);
}

long CScintillaCtrl::GetDocPointer()
{
	return SendMessage(SCI_GETDOCPOINTER,(WPARAM)0,(LPARAM)0);
}

void CScintillaCtrl::SetDocPointer(long pointer)
{
	SendMessage(SCI_SETDOCPOINTER,(WPARAM)pointer,(LPARAM)0);
}

void CScintillaCtrl::SetModEventMask(long mask)
{
	SendMessage(SCI_SETMODEVENTMASK,(WPARAM)mask,(LPARAM)0);
}

long CScintillaCtrl::GetEdgeColumn()
{
	return SendMessage(SCI_GETEDGECOLUMN,(WPARAM)0,(LPARAM)0);
}

void CScintillaCtrl::SetEdgeColumn(long column)
{
	SendMessage(SCI_SETEDGECOLUMN,(WPARAM)column,(LPARAM)0);
}

long CScintillaCtrl::GetEdgeMode()
{
	return SendMessage(SCI_GETEDGEMODE,(WPARAM)0,(LPARAM)0);
}

void CScintillaCtrl::SetEdgeMode(long mode)
{
	SendMessage(SCI_SETEDGEMODE,(WPARAM)mode,(LPARAM)0);
}

long CScintillaCtrl::GetEdgeColour()
{
	return SendMessage(SCI_GETEDGECOLOUR,(WPARAM)0,(LPARAM)0);
}

void CScintillaCtrl::SetEdgeColour(long edgeColour)
{
	SendMessage(SCI_SETEDGECOLOUR,(WPARAM)edgeColour,(LPARAM)0);
}

void CScintillaCtrl::SearchAnchor()
{
	SendMessage(SCI_SEARCHANCHOR,(WPARAM)0,(LPARAM)0);
}

long CScintillaCtrl::SearchNext(long flags,LPCTSTR text)
{
	return SendMessage(SCI_SEARCHNEXT,(WPARAM)flags,(LPARAM)text);
}

long CScintillaCtrl::SearchPrev(long flags,LPCTSTR text)
{
	return SendMessage(SCI_SEARCHPREV,(WPARAM)flags,(LPARAM)text);
}

void CScintillaCtrl::SetCaretPolicy(long caretPolicy,long caretSlop)
{
	SendMessage(SCI_SETCARETPOLICY,(WPARAM)caretPolicy,(LPARAM)caretSlop);
}

long CScintillaCtrl::LinesOnScreen()
{
	return SendMessage(SCI_LINESONSCREEN,(WPARAM)0,(LPARAM)0);
}

void CScintillaCtrl::UsePopUp(BOOL allowPopUp)
{
	SendMessage(SCI_USEPOPUP,(WPARAM)allowPopUp,(LPARAM)0);
}

BOOL CScintillaCtrl::SelectionIsRectangle()
{
	return SendMessage(SCI_SELECTIONISRECTANGLE,(WPARAM)0,(LPARAM)0);
}

void CScintillaCtrl::SetZoom(long zoom)
{
	SendMessage(SCI_SETZOOM,(WPARAM)zoom,(LPARAM)0);
}

long CScintillaCtrl::GetZoom()
{
	return SendMessage(SCI_GETZOOM,(WPARAM)0,(LPARAM)0);
}

long CScintillaCtrl::CreateDocument()
{
	return SendMessage(SCI_CREATEDOCUMENT,(WPARAM)0,(LPARAM)0);
}

void CScintillaCtrl::AddRefDocument(long doc)
{
	SendMessage(SCI_ADDREFDOCUMENT,(WPARAM)0,(LPARAM)doc);
}

void CScintillaCtrl::ReleaseDocument(long doc)
{
	SendMessage(SCI_RELEASEDOCUMENT,(WPARAM)0,(LPARAM)doc);
}

long CScintillaCtrl::GetModEventMask()
{
	return SendMessage(SCI_GETMODEVENTMASK,(WPARAM)0,(LPARAM)0);
}

void CScintillaCtrl::GrabFocus()
{
	SendMessage(SCI_GRABFOCUS,(WPARAM)0,(LPARAM)0);
}

void CScintillaCtrl::StartRecord()
{
	SendMessage(SCI_STARTRECORD,(WPARAM)0,(LPARAM)0);
}

void CScintillaCtrl::StopRecord()
{
	SendMessage(SCI_STOPRECORD,(WPARAM)0,(LPARAM)0);
}

void CScintillaCtrl::SetLexer(long lexer)
{
	SendMessage(SCI_SETLEXER,(WPARAM)lexer,(LPARAM)0);
}

long CScintillaCtrl::GetLexer()
{
	return SendMessage(SCI_GETLEXER,(WPARAM)0,(LPARAM)0);
}

void CScintillaCtrl::Colourise(long start,long end)
{
	SendMessage(SCI_COLOURISE,(WPARAM)start,(LPARAM)end);
}

void CScintillaCtrl::SetProperty(LPCTSTR key,LPCTSTR value)
{
	SendMessage(SCI_SETPROPERTY,(WPARAM)key,(LPARAM)value);
}

void CScintillaCtrl::SetKeyWords(long keywordSet,LPCTSTR keyWords)
{
	SendMessage(SCI_SETKEYWORDS,(WPARAM)keywordSet,(LPARAM)keyWords);
}

BOOL CScintillaCtrl::EM_CanPaste()
{
	return SendMessage(EM_CANPASTE,(WPARAM)0,(LPARAM)0);
}

BOOL CScintillaCtrl::EM_CanUndo()
{
	return SendMessage(EM_CANUNDO,(WPARAM)0,(LPARAM)0);
}

long CScintillaCtrl::EM_CharFromPos(long ptX,long ptY)
{
	POINT pt;
	pt.x = ptX;
	pt.y = ptY;
	return SendMessage(EM_CHARFROMPOS,(WPARAM)0,(LPARAM)&pt);
}

void CScintillaCtrl::EM_EmptyUndoBuffer()
{
	SendMessage(EM_EMPTYUNDOBUFFER,(WPARAM)0,(LPARAM)0);
}

void CScintillaCtrl::EM_ExGetSel(long* crMin,long* crMax)
{
	CHARRANGE cr;
	SendMessage(EM_EXGETSEL,(WPARAM)0,(LPARAM)&cr);
	*crMin = cr.cpMin;
	*crMax = cr.cpMax;
}

long CScintillaCtrl::EM_ExLineFromChar(long pos)
{
	return SendMessage(EM_EXLINEFROMCHAR,(WPARAM)0,(LPARAM)pos);
}

void CScintillaCtrl::EM_ExSetSel(long crMin,long crMax)
{
	CHARRANGE cr;
	cr.cpMin = crMin;
	cr.cpMax = crMax;
	SendMessage(EM_EXSETSEL,(WPARAM)0,(LPARAM)&cr);
}

long CScintillaCtrl::EM_FindText(long flags,long ftMin,long ftMax,LPCTSTR ftStr)
{
	FINDTEXT ft;
	ft.chrg.cpMin = ftMin;
	ft.chrg.cpMax = ftMax; 
	ft.lpstrText = const_cast<char*>(ftStr);
	return SendMessage(EM_FINDTEXT,(WPARAM)flags,(LPARAM)&ft);
}

long CScintillaCtrl::EM_FindTextEx(long flags,long ftMin,long ftMax,LPCTSTR ftStr,long* ftTxtMin,long* ftTxtMax)
{
	FINDTEXTEX ft;
	ft.chrg.cpMin = ftMin;
	ft.chrg.cpMax = ftMax; 
	ft.lpstrText = const_cast<char*>(ftStr);
	long result = SendMessage(EM_FINDTEXTEX,(WPARAM)flags,(LPARAM)&ft);
	*ftTxtMin = ft.chrgText.cpMin;
	*ftTxtMax = ft.chrgText.cpMax; 
	return result;
}

void CScintillaCtrl::EM_FormatRange(BOOL draw,long fr)
{
	SendMessage(EM_FORMATRANGE,(WPARAM)draw,(LPARAM)fr);
}

long CScintillaCtrl::EM_GetFirstVisibleLine()
{
	return SendMessage(EM_GETFIRSTVISIBLELINE,(WPARAM)0,(LPARAM)0);
}

long CScintillaCtrl::EM_GetLine(long line,long textLen,BSTR FAR* textStr)
{
	long result = 0;
	unsigned short len = textLen;
	char* text = new char[textLen + 3];
	memcpy(text,&len,sizeof(short));
	try
	{
		result = SendMessage(EM_GETLINE,(WPARAM)line,(LPARAM)text);
		*textStr = SysAllocStringByteLen(text,result);
	}
	catch(...)
	{
	}
	delete[] text;
	return result;
}

long CScintillaCtrl::EM_GetLineCount()
{
	return SendMessage(EM_GETLINECOUNT,(WPARAM)0,(LPARAM)0);
}

long CScintillaCtrl::EM_GetMargins()
{
	return SendMessage(EM_GETMARGINS,(WPARAM)0,(LPARAM)0);
}

BOOL CScintillaCtrl::EM_GetModify()
{
	return SendMessage(EM_GETMODIFY,(WPARAM)0,(LPARAM)0);
}

void CScintillaCtrl::EM_GetRect(long* rLeft,long* rTop,long* rRight,long* rBottom)
{
	RECT r;
	SendMessage(EM_GETRECT,(WPARAM)0,(LPARAM)&r);
	*rLeft = r.left;
	*rTop = r.top;
	*rRight = r.right;
	*rBottom = r.bottom;
}

long CScintillaCtrl::EM_GetSel()
{
	return SendMessage(EM_GETSEL,(WPARAM)0,(LPARAM)0);
}

long CScintillaCtrl::EM_GetSelText(BSTR FAR* textStr)
{
	long min, max;
	long result = 0;
	EM_ExGetSel(&min,&max);
	char* text = new char[max - min + 1];
	try
	{
		result = SendMessage(EM_GETSELTEXT,(WPARAM)0,(LPARAM)text);
		*textStr = SysAllocStringByteLen(text,result);
	}
	catch(...)
	{
	}
	delete[] text;
	return result;
}

long CScintillaCtrl::EM_GetTextRange(long trMin,long trMax,BSTR FAR* trStr)
{
	long result = 0;
	TEXTRANGE tr;
	tr.chrg.cpMin = trMin;
	tr.chrg.cpMax = trMax;
	if(trMin == 0 && trMax == -1)
		tr.lpstrText = new char[WM_GetTextLength() + 1];		
	else
		tr.lpstrText = new char[trMax - trMin + 1];
	try
	{
		result = SendMessage(EM_GETTEXTRANGE,(WPARAM)0,(LPARAM)&tr);
		*trStr = SysAllocStringByteLen(tr.lpstrText,result);
	}
	catch(...)
	{
	}
	delete[] tr.lpstrText;
	return result;
}

void CScintillaCtrl::EM_HideSelection(BOOL normal)
{
	SendMessage(EM_HIDESELECTION,(WPARAM)normal,(LPARAM)0);
}

long CScintillaCtrl::EM_LineFromChar(long pos)
{
	return SendMessage(EM_LINEFROMCHAR,(WPARAM)pos,(LPARAM)0);
}

long CScintillaCtrl::EM_LineIndex(long line)
{
	return SendMessage(EM_LINEINDEX,(WPARAM)line,(LPARAM)0);
}

long CScintillaCtrl::EM_LineLength(long line)
{
	return SendMessage(EM_LINELENGTH,(WPARAM)line,(LPARAM)0);
}

void CScintillaCtrl::EM_LineScroll(long columns,long lines)
{
	SendMessage(EM_LINESCROLL,(WPARAM)columns,(LPARAM)lines);
}

void CScintillaCtrl::EM_PosFromChar(long* ptX,long* ptY,long pos)
{
	POINT pt;
	SendMessage(EM_POSFROMCHAR,(WPARAM)&pt,(LPARAM)pos);
	*ptX = pt.x;
	*ptY = pt.y;
}

void CScintillaCtrl::EM_ReplaceSel(LPCTSTR text)
{
	SendMessage(EM_REPLACESEL,(WPARAM)0,(LPARAM)text);
}

void CScintillaCtrl::EM_ScrollCaret()
{
	SendMessage(EM_SCROLLCARET,(WPARAM)0,(LPARAM)0);
}

void CScintillaCtrl::EM_SelectionType()
{
	SendMessage(EM_SELECTIONTYPE,(WPARAM)0,(LPARAM)0);
}

void CScintillaCtrl::EM_SetMargins(long flags,long values)
{
	SendMessage(EM_SETMARGINS,(WPARAM)flags,(LPARAM)values);
}

void CScintillaCtrl::EM_SetReadOnly(BOOL readOnly)
{
	SendMessage(EM_SETREADONLY,(WPARAM)readOnly,(LPARAM)0);
}

void CScintillaCtrl::EM_SetSel(long start,long end)
{
	SendMessage(EM_SETSEL,(WPARAM)start,(LPARAM)end);
}

void CScintillaCtrl::EM_Undo()
{
	SendMessage(EM_UNDO,(WPARAM)0,(LPARAM)0);
}

void CScintillaCtrl::WM_Null()
{
	SendMessage(WM_NULL,(WPARAM)0,(LPARAM)0);
}

void CScintillaCtrl::WM_Clear()
{
	SendMessage(WM_CLEAR,(WPARAM)0,(LPARAM)0);
}

void CScintillaCtrl::WM_Command()
{
	SendMessage(WM_COMMAND,(WPARAM)0,(LPARAM)0);
}

void CScintillaCtrl::WM_Copy()
{
	SendMessage(WM_COPY,(WPARAM)0,(LPARAM)0);
}

void CScintillaCtrl::WM_Cut()
{
	SendMessage(WM_CUT,(WPARAM)0,(LPARAM)0);
}

long CScintillaCtrl::WM_GetText(long length,BSTR FAR* textStr)
{
	long result = 0;
	char* text = new char[length + 1];
	try
	{
		result = SendMessage(WM_GETTEXT,(WPARAM)length,(LPARAM)text);
		*textStr = SysAllocStringByteLen(text,result);
	}
	catch(...)
	{
	}
	delete[] text;
	return result;
}

long CScintillaCtrl::WM_GetTextLength()
{
	return SendMessage(WM_GETTEXTLENGTH,(WPARAM)0,(LPARAM)0);
}

void CScintillaCtrl::WM_Notify(long id,long stuff)
{
	SendMessage(WM_NOTIFY,(WPARAM)id,(LPARAM)stuff);
}

void CScintillaCtrl::WM_Paste()
{
	SendMessage(WM_PASTE,(WPARAM)0,(LPARAM)0);
}

void CScintillaCtrl::WM_SetText(LPCTSTR text)
{
	SendMessage(WM_SETTEXT,(WPARAM)0,(LPARAM)text);
}

void CScintillaCtrl::WM_Undo()
{
	SendMessage(WM_UNDO,(WPARAM)0,(LPARAM)0);
}

//$(METHCPP_SECTION]
