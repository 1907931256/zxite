// SinkWorld Tentacle class.
// Tentacle.cxx, a demonstration application for SinkWorld
// Copyright 2004 Neil Hodgson

#ifndef unix
#include <stdlib.h>
#include <crtdbg.h>
#endif

#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>
#include <string>
#include <vector>
#include <algorithm>

#ifdef unix

#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
typedef GtkWidget * HWND;

#define IDYES 1
#define IDCANCEL 2
#define MAX_PATH 260

#else

#define _WIN32_WINNT  0x0400
#include <windows.h>
#include <windowsx.h>
#include <richedit.h>

#ifdef _MSC_VER
#pragma warning(disable: 4244 4312)
#endif

// Take care of 32/64 bit pointers
#ifdef GetWindowLongPtr
static void *PointerFromWindow(HWND hWnd, int nIndex) {
	return reinterpret_cast<void *>(::GetWindowLongPtr(hWnd, nIndex));
}
static void SetWindowPointer(HWND hWnd, int nIndex, void *ptr) {
	::SetWindowLongPtr(hWnd, nIndex, reinterpret_cast<LONG_PTR>(ptr));
}
#else
static void *PointerFromWindow(HWND hWnd, int nIndex) {
	return reinterpret_cast<void *>(::GetWindowLong(hWnd, nIndex));
}
static void SetWindowPointer(HWND hWnd, int nIndex, void *ptr) {
	::SetWindowLong(hWnd, nIndex, reinterpret_cast<LONG>(ptr));
}
#endif

#endif

#include "resource.h"

#include "base.h"
#include "lexers.h"

#include "PhysicalLayout.h"

#include "RGBColor.h"
#include "IDecoration.h"
#include "RegisteredLexers.h"
#include "RegisteredDecorations.h"
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

#include "PhasedPaint.h"
#include "TextView.h"
#include "RangeSelection.h"

#include "TentacleControl.h"

#ifdef unix

void EntryFromLiteral(Literal *s, GtkWidget *w);

#endif

/// The single Tentacle application object.
/// Holds a Document and two TentacleControl objects for viewing it.

struct TentacleApp : public IDocumentChangeListener {
	int test;
	HWND wMain;
	enum { controls=2 };
	int activeControl;
	Document *pdoc;
	TentacleControl *tc[controls];
	enum { idModeStart=2000 };

#ifdef unix
	GtkWidget *findPane;
	GtkWidget *btnFind;
	GtkWidget *entryFind;
	GtkWidget *replacePane;
	GtkWidget *btnFRFind;
	GtkWidget *btnReplaceInSelection;
	GtkWidget *entryFRFind;
	GtkWidget *entryReplace;
	GtkWidget *toggleWord;
	GtkWidget *toggleCase;

	bool allowMenuActions;
	GtkAccelGroup *accelGroup;
	GtkItemFactory *itemFactory;
#else
	HINSTANCE hInstance;
	HWND currentDialog;
#endif
	bool titleAtSavePoint;
	char fullPath[MAX_PATH];

	enum panes { paneContentOnly, paneFind, paneReplace } panesVisible;
	bool haveFound;
	Literal *findWhat;
	Literal *replaceWhat;
	bool wholeWord;
	bool matchCase;
	const char *appName;
	enum { blockSize = 128 * 1024};

	LiteralSet fontNames;
	Literal *overrideMode;

	TentacleApp();
	virtual ~TentacleApp();

	// IDocumentChangeListener methods
	virtual void StateAltered();
	virtual void Invalidate();
	virtual void InvalidateRange(int a, int b);

	TentacleControl &ControlActive();
	void Destroy();
	void Deallocate();
	void DeallocateStyling();
	void CreateDocument(const char *name);
	void ReadOptionsFile(Literal *prefix, char *filename);
	void ReadOptionsAndModes();
	void CreateStyling(const char *name);
	void SizeChanged();
	void GetViewRectangle(SRectangle *rc);

	void SetTitle(bool changePath=false);
	void NewDocument();
	void New();
	void Open();
	void OpenFile(const char *fileName);
	void Save();
	void SaveAs();
	void SaveFile(const char *fileName);
	int SaveIfUnsure();
	void Quit();
	void ModeMenu(int id);
	void CheckMenuItem(int wIDCheckItem, bool val);

	void Command(int id);
	void EnableAMenuItem(int id, bool enable);
	void CheckMenus();

	int SearchText(Literal *text, int start, int end);
	void SearchFindNext();
	void ReplaceOnce();
	void SearchFind();
	void SearchReplace();
	void ReplaceAll(bool selectionRange);

	int DisplayMessage(char *m, bool ask=false);
	void ReadEntries();
#ifdef unix
	static void MenuSignal(TentacleApp *ta, guint action, GtkWidget *w);
	void SelectPane(panes pane);
	void FFind(GtkWidget *w);
	void FRFind(GtkWidget *w);
	void FRReplace(GtkWidget *w);
	void FRReplaceAll(GtkWidget *w);
	void FRReplaceInSelection(GtkWidget *w);
	static void FFindSignal(GtkWidget *w, TentacleApp *ta) { ta->FFind(w); }
	static void FRFindSignal(GtkWidget *w, TentacleApp *ta) { ta->FRFind(w); }
	static void FRReplaceSignal(GtkWidget *w, TentacleApp *ta) { ta->FRReplace(w); }
	static void FRReplaceAllSignal(GtkWidget *w, TentacleApp *ta) { ta->FRReplaceAll(w); }
	static void FRReplaceInSelectionSignal(GtkWidget *w, TentacleApp *ta) { ta->FRReplaceInSelection(w); }
	gint PaneKey(GtkWidget *w, GdkEventKey *event);
	static gint PaneSignalKey(GtkWidget *w, GdkEventKey *event, TentacleApp *ta) {
		return ta->PaneKey(w, event);
	}
	GtkWidget *PaneFind();
	GtkWidget *PaneFindReplace();
	void Run(int argc, char *argv[]);
#else
	void Create(HWND wMain_);
	static TentacleApp *Caller(HWND hDlg, UINT message, LPARAM lParam) {
		if (message == WM_INITDIALOG) {
			SetWindowPointer(hDlg, DWL_USER, reinterpret_cast<void *>(lParam));
		}
		return reinterpret_cast<TentacleApp *>(::PointerFromWindow(hDlg, DWL_USER));
	}
	BOOL ReplaceMessage(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static BOOL CALLBACK ReplaceDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
		return Caller(hDlg, message, lParam)->ReplaceMessage(hDlg, message, wParam, lParam);
	}
	BOOL FindMessage(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static BOOL CALLBACK FindDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
		return Caller(hDlg, message, lParam)->FindMessage(hDlg, message, wParam, lParam);
	}
	LRESULT ContextMenu(WPARAM wParam, LPARAM lParam);
	LRESULT WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
	void AddModeMenu();
#endif
};

static TentacleApp *app=0;

TentacleApp::TentacleApp() {
#ifdef unix
#else
	hInstance = 0;
	currentDialog = 0;
#endif
	wMain = 0;
	activeControl = 0;
	titleAtSavePoint = false;
	fullPath[0] = '\0';
	haveFound = false;
	findWhat = new Literal();
	replaceWhat = new Literal();
	appName = "Tentacle";
	panesVisible = paneContentOnly;
	overrideMode = NULL;

	pdoc = new Document(fontNames.FindOrAddKey(Surface::fontDefault), &fontNames);

	for (int c=0;c<controls;c++) {
		tc[c] = TentacleControl::Allocate();
		tc[c]->SetDocument(pdoc, this);
	}
}

TentacleApp::~TentacleApp() {
	Deallocate();
	for (int c=0;c<controls;c++) {
		delete tc[c];
		tc[c] = NULL;
	}
	delete findWhat;
	delete replaceWhat;
}

void TentacleApp::StateAltered() {
	for (int c=0;c<controls;c++) {
		if (tc[c]->HasFocus()) {
			activeControl = c;
		}
	}
	SetTitle();
	CheckMenus();
}

void TentacleApp::Invalidate() {
	for (int c=0;c<controls;c++) {
		tc[c]->Invalidate();
	}
}

void TentacleApp::InvalidateRange(int a, int b) {
	for (int c=0;c<controls;c++) {
		tc[c]->InvalidateRange(a,b);
	}
}

TentacleControl &TentacleApp::ControlActive() {
	return *tc[activeControl];
}

void TentacleApp::Destroy() {
}

void TentacleApp::Deallocate() {
	DeallocateStyling();
	delete pdoc;
	pdoc = NULL;
	for (int c=0;c<controls;c++) {
		tc[c]->Deallocate();
	}
	delete overrideMode;
	overrideMode = NULL;
}

void TentacleApp::DeallocateStyling() {
	pdoc->DeallocateStyling();
}

void TentacleApp::CreateDocument(const char *name) {
	Deallocate();
	pdoc = new Document(fontNames.FindOrAddKey(Surface::fontDefault), &fontNames);
}

void TentacleApp::ReadOptionsFile(Literal *prefix, char *filename) {
	FILE *rcfile = fopen(filename, "rb");
	if (rcfile) {
		enum { sizeFile = 60000 };
		SW_BYTE propsData[sizeFile];
		int lenData = static_cast<int>(fread(propsData, 1, sizeof(propsData), rcfile));
		fclose(rcfile);
		pdoc->options->SetFromText(prefix, propsData, 0, lenData);
	}
}

void TentacleApp::ReadOptionsAndModes() {
	pdoc->ClearOptions();

	// Find the directory of the executable which is where the options and mode files are
	char dirBase[MAX_PATH];
#ifdef unix
	strcpy(dirBase, getenv("HOME"));
	strcat(dirBase, "/");
#else
#ifdef MODULE_RELATIVE_FILES
	::GetModuleFileName(0, dirBase, sizeof(dirBase));
	// Remove the SciTE.exe
	char *lastSlash = strrchr(dirBase, '\\');
	if (lastSlash)
		*(lastSlash+1) = '\0';
#endif
	strcpy(dirBase, getenv("USERPROFILE"));
	strcat(dirBase, "\\");
#endif

	// Read in the user's options
	char path[MAX_PATH];
	strcpy(path, dirBase);
	strcat(path, "user.options");
	ReadOptionsFile(NULL, path);

	const char *modeExtension = ".mode";
	const size_t modeLen = strlen(modeExtension);
#ifdef unix
	DIR *dp;
	struct dirent *ent;

	if ((dp = opendir(dirBase)) == NULL) {
		fprintf(stderr, "%s: cannot open for reading: %s\n",
			dirBase, strerror(errno));
		return;
	}

	errno = 0;
	while ((ent = readdir(dp)) != NULL) {
		size_t lenName = strlen(ent->d_name);
		if (lenName > strlen(modeExtension)) {
			if (memcmp(ent->d_name + lenName - modeLen, modeExtension, modeLen) == 0) {
				Literal modeText("mode");
				Literal entryName(ent->d_name);
				Literal modeName(&entryName, 0, lenName - strlen(modeExtension));
				Literal modePrefix(&modeText, (SW_BYTE)'.', &modeName);
				strcpy(path, dirBase);
				strcat(path, ent->d_name);
				ReadOptionsFile(&modePrefix, path);
			}
		}
	}

	if (errno != 0) {
		fprintf(stderr, "%s: reading directory entries: %s\n",
			dirBase, strerror(errno));
		return;
	}

	if (closedir(dp) != 0) {
		fprintf(stderr, "%s: closedir: %s\n",
			dirBase, strerror(errno));
		return;
	}

#else
	// Find all *.mode files and read them in
	WIN32_FIND_DATA findFileData;
	strcpy(path, dirBase);
	strcat(path, "*.mode");
	HANDLE hFind = FindFirstFile(path, &findFileData);
	bool complete = false;
	if (hFind != INVALID_HANDLE_VALUE) {
		while (!complete) {
			char drive[MAX_PATH];
			char dir[MAX_PATH];
			char fname[MAX_PATH];
			char ext[MAX_PATH];
			_splitpath(findFileData.cFileName, drive, dir, fname, ext);
			Literal modeText("mode");
			Literal modeName(fname);
			Literal modePrefix(&modeText, (SW_BYTE)'.', &modeName);
			strcpy(path, dirBase);
			strcat(path, findFileData.cFileName);
			ReadOptionsFile(&modePrefix, path);
			if (!FindNextFile(hFind, &findFileData)) {
				complete = true;
			}
		}
		FindClose(hFind);
	}
#endif
}

void TentacleApp::SetTitle(bool changePath) {
	if (changePath || (titleAtSavePoint != pdoc->pbuffer->IsSavePoint())) {
		titleAtSavePoint = pdoc->pbuffer->IsSavePoint();
		char title[MAX_PATH + 100];
		strcpy(title, fullPath);
		if (titleAtSavePoint) {
			strcat(title, " - ");
		} else {
			strcat(title, " * ");
		}
		strcat(title, appName);
		if (pdoc->currentMode->Data()) {
			strcat(title, " : ");
			char *mode = (char *)pdoc->currentMode->AsUTF8();
			strcat(title, mode);
			delete []mode;
		}
#ifdef unix
		gtk_window_set_title(GTK_WINDOW(wMain), title);
#else
		::SetWindowText(wMain, title);
#endif
	}
}

void TentacleApp::NewDocument() {
	for (int c=0;c<controls;c++) {
		tc[c]->SetScrollBars();
	}
	pdoc->pbuffer->DiscardLines();
	pdoc->pbuffer->DeleteRange(0, pdoc->pbuffer->Length());
}

void TentacleApp::New() {
	NewDocument();
	CreateDocument("");
	CreateStyling("");
	fullPath[0] = '\0';
	SetTitle(true);
	Invalidate();
}

void TentacleApp::CreateStyling(const char *name) {
	Literal *literalName = new Literal(name);
	int dot = literalName->Length();
	while ((dot > 0) && literalName->CharAt(dot-1) != '.') {
		dot--;
	}
	int slen = literalName->Length() - dot;
	Literal *literalExtension = new Literal(literalName, dot, literalName->Length() - dot);
	ReadOptionsAndModes();
	delete pdoc->currentMode;
	if (overrideMode != NULL) {
		pdoc->currentMode = new Literal(overrideMode);
		Literal litEmpty("");
		pdoc->CreateStyling(&litEmpty);
	} else {
		pdoc->currentMode = new Literal("");
		pdoc->CreateStyling(literalExtension);
	}
	delete literalExtension;
	delete literalName;
	for (int c=0;c<controls;c++) {
		tc[c]->SetDocument(pdoc, this);
	}
}

void TentacleApp::OpenFile(const char *fileName) {
	strcpy(fullPath, fileName);
	FILE *fp = fopen(fullPath, "rb");
	if (fp) {
		NewDocument();
		pdoc->pbuffer->SetUndoCollection(false);
		pdoc->pbuffer->SetValidating(false);
		CreateDocument(fullPath);
		CreateStyling(fullPath);
		unsigned char data[blockSize];
		const unsigned char *d=reinterpret_cast<unsigned char *>(data);
		int lenFile = static_cast<int>(fread(data, 1, sizeof(data), fp));
		while (lenFile > 0) {
			if (pdoc->pbuffer->Length() == 0) {
				if ((d[0] == 0xFE) && (d[1] == 0xFF)) {
					pdoc->pbuffer->SetEncoding(SplitText::ENCODING_UTF16BE);
					ControlActive().InsertBytes(pdoc->pbuffer->Length(), data+2, lenFile-2);
				} else if ((d[0] == 0xFF) && (d[1] == 0xFE)) {
					pdoc->pbuffer->SetEncoding(SplitText::ENCODING_UTF16LE);
					ControlActive().InsertBytes(pdoc->pbuffer->Length(), data+2, lenFile-2);
				} else if ((d[0] == 0xEF) && (d[1] == 0xBB) && (d[2] == 0xBF)) {
					pdoc->pbuffer->SetEncoding(SplitText::ENCODING_UTF8);
					ControlActive().InsertBytes(pdoc->pbuffer->Length(), data+3, lenFile-3);
				} else {
					pdoc->pbuffer->SetEncoding(SplitText::ENCODING_UTF8);
					ControlActive().InsertBytes(pdoc->pbuffer->Length(), data, lenFile);
				}
			} else {
				ControlActive().InsertBytes(pdoc->pbuffer->Length(), data, lenFile);
			}
			lenFile = static_cast<int>(fread(data, 1, sizeof(data), fp));
		}
		fclose(fp);
		pdoc->pbuffer->DeleteUndoHistory();
		try {
			pdoc->pbuffer->SetValidating(true);
		} catch (CharacterFragmentException *) {
			DisplayMessage("Character encoding failure", false);
			throw;
		}
		pdoc->pbuffer->SetUndoCollection(true);
		SetTitle(true);
	} else {
		CreateDocument("");
		CreateStyling("");
		char msg[MAX_PATH + 100];
		strcpy(msg, "Could not open file \"");
		strcat(msg, fullPath);
		strcat(msg, "\".");
		DisplayMessage(msg);
	}
	for (int c=0;c<controls;c++) {
		tc[c]->SetSelection(0,0);
		tc[c]->Invalidate();
		tc[c]->SetScrollBars();
		tc[c]->ScrollTo(0, true);
	}
}

void TentacleApp::Save() {
	SaveFile(fullPath);
	SetTitle(true);
}

void TentacleApp::SaveFile(const char *fileName) {
	FILE *fp = fopen(fullPath, "wb");
	if (fp) {
		unsigned char data[blockSize + 1];
		int length = pdoc->pbuffer->Length();
		for (int i = 0; length > 0; i += blockSize) {
			int grabSize = length - i;
			if (grabSize > blockSize)
				grabSize = blockSize;
			pdoc->pbuffer->RetrieveBytes(i, data, i + grabSize);
			fwrite(data, grabSize, 1, fp);
			length -= grabSize;
		}
		fclose(fp);
		pdoc->pbuffer->SetSavePoint();
	} else {
		char msg[MAX_PATH + 100];
		strcpy(msg, "Could not save file \"");
		strcat(msg, fullPath);
		strcat(msg, "\".");
		DisplayMessage(msg);
	}
}

int TentacleApp::SaveIfUnsure() {
	if (!pdoc->pbuffer->IsSavePoint()) {
		char msg[MAX_PATH + 100];
		strcpy(msg, "Save changes to \"");
		strcat(msg, fullPath);
		strcat(msg, "\"?");
		int decision = DisplayMessage(msg, true);
		if (decision == IDYES) {
			Save();
		}
		return decision;
	}
	return IDYES;
}

void TentacleApp::Quit() {
#ifdef unix
	gtk_main_quit();
#else
	::PostQuitMessage(0);
#endif
}

void TentacleApp::Command(int id) {
	switch (id) {
	case IDM_FILE_NEW:
		if (SaveIfUnsure() != IDCANCEL) {
			New();
		}
		break;
	case IDM_FILE_OPEN:
		if (SaveIfUnsure() != IDCANCEL) {
			Open();
		}
		break;
	case IDM_FILE_SAVE:
		Save();
		break;
	case IDM_FILE_SAVEAS:
		SaveAs();
		break;
	case IDM_FILE_EXIT:
		if (SaveIfUnsure() != IDCANCEL) {
			Quit();
		}
		break;

	case IDM_EDIT_UNDO:
		ControlActive().Undo();
		break;
	case IDM_EDIT_REDO:
		ControlActive().Redo();
		break;
	case IDM_EDIT_CUT:
		ControlActive().Cut();
		break;
	case IDM_EDIT_COPY:
		ControlActive().Copy();
		break;
	case IDM_EDIT_PASTE:
		ControlActive().Paste();
		break;
	case IDM_EDIT_DELETE:
		ControlActive().Clear();
		break;
	case IDM_EDIT_SELECTALL:
		ControlActive().SelectAll();
		break;

	case ID_EDIT_FOLD:
		ControlActive().ToggleFold();
		break;
	case ID_EDIT_FOLDALL:
		ControlActive().FoldAll(TentacleControl::fcFold);
		break;
	case ID_SHOW_ALL:
		ControlActive().FoldAll(TentacleControl::fcShow);
		break;

	case IDM_SEARCH_FIND:
		SearchFind();
		break;
	case IDM_SEARCH_FINDNEXT:
		SearchFindNext();
		break;
	case IDM_SEARCH_REPLACE:
		SearchReplace();
		break;

	case ID_OPTIONS_BRACELEVEL:
		ControlActive().ToggleBraceLevel();
		CreateStyling(fullPath);
		CheckMenus();
		break;

	case ID_OPTIONS_FANCY:
		ControlActive().ToggleFancy();
		CreateStyling(fullPath);
		CheckMenus();
		break;

	case IDM_EDIT_WRAP:
		ControlActive().ToggleWrap();
		CheckMenus();
		break;

	case ID_OPTIONS_SWITCH_PANE:
		if (tc[0]->HasFocus()) {
			tc[1]->Focus();
		} else {
			tc[0]->Focus();
		}
		break;

	default:
		if (id >= idModeStart && id < idModeStart + 100) {
			ModeMenu(id);
		}
		break;

	};
}

void TentacleApp::EnableAMenuItem(int id, bool enable) {
#ifdef unix
#else
	if (enable)
		::EnableMenuItem(::GetMenu(wMain), id, MF_ENABLED | MF_BYCOMMAND);
	else
		::EnableMenuItem(::GetMenu(wMain), id, MF_DISABLED | MF_GRAYED | MF_BYCOMMAND);
#endif
}

void TentacleApp::CheckMenus() {
	//EnableAMenuItem(IDM_FILE_SAVE, isDirty);
	CheckMenuItem(ID_OPTIONS_BRACELEVEL, ControlActive().braceLevel);
	CheckMenuItem(ID_OPTIONS_FANCY, ControlActive().fancy);
	CheckMenuItem(IDM_EDIT_WRAP, ControlActive().wrap);

	for (int id = idModeStart; id < idModeStart + 100; id++) {
		// Mode menu
#ifdef unix
#else
		wchar_t language[200];
		if (::GetMenuStringW(::GetMenu(wMain), id, language, 200, MF_BYCOMMAND)) {
			Literal litItem(language);
			Literal litSpace("\t");
			int space = litItem.Find(&litSpace, 0);
			Literal litMode(&litItem, 0, space);
			CheckMenuItem(id, (pdoc->currentMode != NULL) && litMode.Equals(pdoc->currentMode));
		}
#endif
	}
}

int TentacleApp::SearchText(Literal *text, int start, int end) {
	int findLength = pdoc->pbuffer->LengthInText(text->Data(), text->Length());
	int last = end - findLength;
	for (int look=start; look<last; look=pdoc->pbuffer->PositionNext(look)) {
		bool found = true;
		int check = look;
		for (int j=0;j<findLength && found;j++,check = pdoc->pbuffer->PositionNext(check)) {
			if (pdoc->pbuffer->CharValue(check) != text->CharAt(j))
				found = false;
		}
		if (found) {
			return look;
		}
	}
	return -1;
}

void TentacleApp::SearchFindNext() {
	haveFound = false;
	int docLength = pdoc->pbuffer->Length();
	int startEnd = ControlActive().sr.EndSelection();
	int next = SearchText(findWhat, startEnd, docLength);
	if (next < 0) {
		next = SearchText(findWhat, 0, startEnd);
	}
	if (next >= 0) {
		int findLength = pdoc->pbuffer->LengthInText(findWhat->Data(), findWhat->Length());
		ControlActive().SetSelection(next + findLength, next);
		ControlActive().EnsureCaretVisible();
		ControlActive().Invalidate();
		haveFound = true;
	}
}

void TentacleApp::ReplaceOnce() {
	if (haveFound) {
		int start = ControlActive().sr.StartSelection();
		ControlActive().DeleteRange(start, ControlActive().sr.EndSelection() - start);
		int len = ControlActive().InsertUTF32(start, replaceWhat->Data(), replaceWhat->Length());
		ControlActive().SetSelection(start, start + len);
		ControlActive().Invalidate();
	}
	SearchFindNext();
}

void TentacleApp::SearchFind() {
#ifdef unix
	SelectPane(paneFind);
	haveFound = false;
	if (panesVisible == paneFind) {
		Literal *wordAtCaret = ControlActive().WordAtPosition(ControlActive().sr.Caret());
		EntryFromLiteral(wordAtCaret, entryFind);
		gtk_entry_select_region(GTK_ENTRY(entryFind), 0, wordAtCaret->LengthUTF8());
		delete wordAtCaret;
		gtk_widget_grab_focus(entryFind);
		gtk_widget_grab_default(GTK_WIDGET(btnFind));
	}
	ControlActive().EnsureCaretVisible();

#else
	if (!currentDialog) {
		currentDialog = ::CreateDialogParamW(hInstance,
		(LPCWSTR)MAKEINTRESOURCE(IDD_SEARCH),
		wMain,
		reinterpret_cast<DLGPROC>(FindDlg),
		reinterpret_cast<LPARAM>(this));
		::ShowWindow(currentDialog, SW_SHOW);
	}
#endif
}

void TentacleApp::SearchReplace() {
#ifdef unix
	SelectPane(paneReplace);
	haveFound = false;
	if (panesVisible == paneReplace) {
		Literal *wordAtCaret = ControlActive().WordAtPosition(ControlActive().sr.Caret());
		EntryFromLiteral(wordAtCaret, entryFRFind);
		gtk_entry_select_region(GTK_ENTRY(entryFRFind), 0, wordAtCaret->LengthUTF8());
		delete wordAtCaret;
		gtk_widget_grab_focus(entryFRFind);
		gtk_widget_grab_default(GTK_WIDGET(btnFRFind));
	}
	ControlActive().EnsureCaretVisible();
#else
	if (!currentDialog) {
		currentDialog = ::CreateDialogParamW(hInstance,
		(LPCWSTR)MAKEINTRESOURCE(IDD_REPLACE),
		wMain,
		reinterpret_cast<DLGPROC>(ReplaceDlg),
		reinterpret_cast<LPARAM>(this));
		::ShowWindow(currentDialog, SW_SHOW);
	}
#endif
}

void TentacleApp::ReplaceAll(bool selectionRange) {
	int startPosition = selectionRange ? ControlActive().sr.StartSelection() : 0;
	int endPosition = selectionRange ? ControlActive().sr.EndSelection() : pdoc->pbuffer->Length();
	int findLength = pdoc->pbuffer->LengthInText(findWhat->Data(), findWhat->Length());
	if (findLength == 0)
		return;
	int nextMatch = SearchText(findWhat, startPosition, endPosition);
	while (nextMatch >= 0) {
		ControlActive().DeleteRange(nextMatch, findLength);
		int replaceLength = ControlActive().InsertUTF32(nextMatch, replaceWhat->Data(), replaceWhat->Length());
		startPosition = nextMatch + replaceLength;
		endPosition += replaceLength - findLength;
		nextMatch = SearchText(findWhat, startPosition, endPosition);
	}
	ControlActive().Invalidate();
}

#ifdef unix

///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////// Unix ////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////

void TentacleApp::SizeChanged() {
}

void TentacleApp::Open() {
	GtkWidget *dlg = gtk_file_chooser_dialog_new("Open File",
			      GTK_WINDOW(wMain),
			      GTK_FILE_CHOOSER_ACTION_OPEN,
			      GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
			      GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
			      NULL);
	g_object_set(dlg, "show-hidden", TRUE, NULL);
	gtk_dialog_set_default_response(GTK_DIALOG(dlg), GTK_RESPONSE_ACCEPT);

	GtkFileFilter *filter;
	filter = gtk_file_filter_new();
	gtk_file_filter_set_name(filter, "C (.c;.cpp;.cxx;.h;.cs;.java)");
	gtk_file_filter_add_pattern(filter, "*.c");
	gtk_file_filter_add_pattern(filter, "*.cpp");
	gtk_file_filter_add_pattern(filter, "*.cxx");
	gtk_file_filter_add_pattern(filter, "*.h");
	gtk_file_filter_add_pattern(filter, "*.cs");
	gtk_file_filter_add_pattern(filter, "*.java");
	gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dlg), filter);
	filter = gtk_file_filter_new();
	gtk_file_filter_set_name(filter, "Python (.py;.pyw)");
	gtk_file_filter_add_pattern(filter, "*.py");
	gtk_file_filter_add_pattern(filter, "*.pyw");
	gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dlg), filter);
	filter = gtk_file_filter_new();
	gtk_file_filter_set_name(filter, "Basic (.vb;.vbs;.bas)");
	gtk_file_filter_add_pattern(filter, "*.vb");
	gtk_file_filter_add_pattern(filter, "*.vbs");
	gtk_file_filter_add_pattern(filter, "*.bas");
	gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dlg), filter);
	filter = gtk_file_filter_new();
	gtk_file_filter_set_name(filter, "Web (.html;.htm;.asp;.shtml;.css;.xml)");
	gtk_file_filter_add_pattern(filter, "*.html");
	gtk_file_filter_add_pattern(filter, "*.htm");
	gtk_file_filter_add_pattern(filter, "*.asp");
	gtk_file_filter_add_pattern(filter, "*.shtml");
	gtk_file_filter_add_pattern(filter, "*.css");
	gtk_file_filter_add_pattern(filter, "*.xml");
	gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dlg), filter);
	filter = gtk_file_filter_new();
	gtk_file_filter_set_name(filter, "Settings Files (.mode;.options)");
	gtk_file_filter_add_pattern(filter, "*.options");
	gtk_file_filter_add_pattern(filter, "*.mode");
	gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dlg), filter);
	filter = gtk_file_filter_new();
	gtk_file_filter_set_name(filter, "All Files");
	gtk_file_filter_add_pattern(filter, "*");
	gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dlg), filter);

	if (gtk_dialog_run(GTK_DIALOG(dlg)) == GTK_RESPONSE_ACCEPT) {
		GSList *names = gtk_file_chooser_get_filenames(GTK_FILE_CHOOSER(dlg));
		GSList *nameCurrent = names;
		while (nameCurrent) {
			char *filename = static_cast<char *>(nameCurrent->data);
			OpenFile(filename);
			g_free(filename);
			nameCurrent = g_slist_next(nameCurrent);
		}
		g_slist_free(names);
	}
	gtk_widget_destroy(dlg);
}

void TentacleApp::SaveAs() {
	GtkWidget *dlg = gtk_file_chooser_dialog_new("Save File",
			      GTK_WINDOW(wMain),
			      GTK_FILE_CHOOSER_ACTION_SAVE,
			      GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
			      GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT,
			      NULL);

	if (gtk_dialog_run(GTK_DIALOG(dlg)) == GTK_RESPONSE_ACCEPT) {
		char *filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dlg));
		strcpy(fullPath, filename);
		SetTitle(true);
		SaveFile(fullPath);
		Invalidate();
		g_free(filename);
	}
	gtk_widget_destroy(dlg);
}

void TentacleApp::ModeMenu(int id) {
	//~ // Mode menu
	//~ wchar_t language[200];
	//~ ::GetMenuStringW(::GetMenu(wMain), id, language, 200, MF_BYCOMMAND);
	//~ Literal litItem(language);
	//~ wchar_t *wSpace = wcschr(language, L'\t');
	//~ *wSpace = 0;
	//~ Literal exts(".name");
	//~ WordList *keys = pdoc->options->Keys(&exts);
	//~ for (int k=0;k<keys->Length();k++) {
		//~ Literal *lKey = keys->GetText(k);
		//~ Literal *lVal = pdoc->options->Value(lKey);
		//~ if (litItem.EndsWith(lVal)) {
			//~ delete overrideMode;
			//~ overrideMode = new Literal(language);
			//~ CreateStyling("X.XXXXX");
			//~ CheckMenus();
			//~ ControlActive().Invalidate();
			//~ SetTitle(true);
		//~ }
		//~ delete lKey;
	//~ }
	//~ delete keys;
}

void TentacleApp::CheckMenuItem(int wIDCheckItem, bool val) {
	GtkWidget *item = gtk_item_factory_get_widget_by_action(itemFactory, wIDCheckItem);
	allowMenuActions = false;
	if (item) {
		gtk_check_menu_item_set_state(GTK_CHECK_MENU_ITEM(item), val ? TRUE : FALSE);
	}
	allowMenuActions = true;
}

void TentacleApp::GetViewRectangle(SRectangle *rc) {
	rc->left = wMain->allocation.x;
	rc->top = wMain->allocation.y;
	rc->right = rc->left + wMain->allocation.width - 1;
	rc->bottom = rc->top + wMain->allocation.height - 1;
}

int TentacleApp::DisplayMessage(char *m, bool ask) {
	GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(wMain),
		GTK_DIALOG_DESTROY_WITH_PARENT,
		GTK_MESSAGE_ERROR,
		GTK_BUTTONS_CLOSE,
		m);
	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);
	//~ fprintf(stderr, "%s: %s\n", appName, m);
	return ask ? 0 : 1;
}

static int exit_app(GtkWidget*, GdkEventAny*, gpointer) {
	gtk_main_quit();
	return 1;
}

Literal *LiteralFromEntry(GtkWidget *w) {
	const char *value = gtk_entry_get_text(GTK_ENTRY(w));
	return new Literal(value);
}

void EntryFromLiteral(Literal *s, GtkWidget *w) {
	SW_BYTE *asUTF8=s->AsUTF8();
	gtk_entry_set_text(GTK_ENTRY(w), (char *)asUTF8);
	delete []asUTF8;
}

void TentacleApp::ReadEntries() {
	delete findWhat;
	findWhat = LiteralFromEntry(entryFRFind);
	delete replaceWhat;
	replaceWhat = LiteralFromEntry(entryReplace);
}

void TentacleApp::MenuSignal(TentacleApp *ta, guint action, GtkWidget *w) {
	if (ta->allowMenuActions)
		ta->Command(action);
}

static GtkWidget *MakeCommand(const char *text, GtkAccelGroup *accel_group,
		GtkSignalFunc func, gpointer data) {
	GtkWidget *command = gtk_button_new_with_label("");
	GTK_WIDGET_SET_FLAGS(command, GTK_CAN_DEFAULT);
	guint key = gtk_label_parse_uline(GTK_LABEL(GTK_BIN(command)->child), text);
	gtk_widget_add_accelerator(command, "clicked", accel_group,
	                          key, GDK_MOD1_MASK, (GtkAccelFlags)0);
	gtk_signal_connect(GTK_OBJECT(command), "clicked", func, data);
	return command;
}

static GtkWidget *MakeToggle(const char *text, GtkAccelGroup *accel_group, bool active) {
	GtkWidget *toggle = gtk_check_button_new_with_label("");
	guint key = gtk_label_parse_uline(GTK_LABEL(GTK_BIN(toggle)->child), text);
	gtk_widget_add_accelerator(toggle, "clicked", accel_group,
	                           key, GDK_MOD1_MASK, (GtkAccelFlags)0);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(toggle), active);
	return toggle;
}

void AddCell(GtkWidget *wTable, GtkWidget *w, guint column, guint row, bool expand=false) {
	GtkAttachOptions opts = static_cast<GtkAttachOptions>(GTK_FILL);
	if (expand)
		opts = static_cast<GtkAttachOptions>(GTK_SHRINK | GTK_FILL | GTK_EXPAND);
	gtk_table_attach(GTK_TABLE(wTable), w, column, column+1, row, row+1, opts, opts, 0, 0);
}

void ShrinkChildren(GtkWidget *w) {
	GList *all = gtk_container_get_children(GTK_CONTAINER(w));
	GList *wCurrent = all;
	while (wCurrent) {
		gtk_misc_set_padding(GTK_MISC(wCurrent), 0, 0);
		wCurrent = g_list_next(wCurrent);
	}
	g_list_free(all);
}

void TentacleApp::SelectPane(panes pane) {
	if (pane != panesVisible) {
		switch (panesVisible) {
		case paneFind:
			gtk_widget_hide_all(findPane);
			break;
		case paneReplace:
			gtk_widget_hide_all(replacePane);
			break;
		}
		switch (pane) {
		case paneFind:
			gtk_widget_show_all(findPane);
			break;
		case paneReplace:
			gtk_widget_show_all(replacePane);
			break;
		}
		panesVisible = pane;
	};
}

void TentacleApp::FFind(GtkWidget *) {
	delete findWhat;
	findWhat = LiteralFromEntry(entryFind);
	SearchFindNext();
}

void TentacleApp::FRFind(GtkWidget *) {
	ReadEntries();
	SearchFindNext();
}

void TentacleApp::FRReplace(GtkWidget *) {
	ReadEntries();
	ReplaceOnce();
}

void TentacleApp::FRReplaceAll(GtkWidget *) {
	ReadEntries();
	ReplaceAll(false);
}

void TentacleApp::FRReplaceInSelection(GtkWidget *) {
	ReadEntries();
	ReplaceAll(true);
}

gint TentacleApp::PaneKey(GtkWidget *w, GdkEventKey *event) {
//~ fprintf(stderr, "PaneKey %x\n", event->keyval);
	if (event->keyval == GDK_Escape) {
		SelectPane(paneContentOnly);
		return TRUE;
	} else if (event->keyval == GDK_Return) {
		switch (panesVisible) {
			case paneFind:
				FFind(0);
				break;
			case paneReplace:
				FRFind(0);
				break;
		}
		return TRUE;
	} else if (event->keyval == GDK_Tab) {
		switch (panesVisible) {
			case paneFind:
				return (GTK_WIDGET_HAS_FOCUS(btnFind));
			case paneReplace:
				return (GTK_WIDGET_HAS_FOCUS(btnReplaceInSelection));
		}
		return FALSE;
	} else if (event->keyval == GDK_ISO_Left_Tab) {
		switch (panesVisible) {
			case paneFind:
				return (GTK_WIDGET_HAS_FOCUS(entryFind));
			case paneReplace:
				return (GTK_WIDGET_HAS_FOCUS(entryFRFind));
		}
		return FALSE;
	} else if (event->keyval == GDK_Down) {
		return TRUE;
	}
	return FALSE;
}

GtkWidget *TentacleApp::PaneFind() {
	GtkWidget *wFind = gtk_table_new(1, 3, FALSE);
	GtkWidget *wTable = wFind;
	gtk_container_set_border_width(GTK_CONTAINER(wTable), 4);
	gtk_table_set_row_spacings(GTK_TABLE(wTable), 4);
	gtk_table_set_col_spacings(GTK_TABLE(wTable), 4);

	GtkWidget *labelFind = gtk_label_new("Find what:");
	AddCell(wTable, labelFind, 0, 0);
	gtk_misc_set_alignment(GTK_MISC(labelFind), 1.0f, 0.5f);
	entryFind = gtk_entry_new();
	AddCell(wTable, entryFind, 1, 0, true);
	gtk_signal_connect(GTK_OBJECT(entryFind), "activate",
		GtkSignalFunc(FFindSignal), this);

	btnFind = MakeCommand("F_ind", accelGroup,
		GtkSignalFunc(FFindSignal), this);
	AddCell(wTable, btnFind, 2, 0);

	GTK_WIDGET_SET_FLAGS(GTK_WIDGET(btnFind), GTK_CAN_DEFAULT);

	gtk_signal_connect(GTK_OBJECT(wFind), "key_press_event",
		GtkSignalFunc(PaneSignalKey), this);

	return wFind;
}

GtkWidget *TentacleApp::PaneFindReplace() {
	GtkWidget *wFindReplace = gtk_table_new(2, 4, FALSE);
	GtkWidget *wTable = wFindReplace;
	gtk_container_set_border_width(GTK_CONTAINER(wTable), 4);
	gtk_table_set_row_spacings(GTK_TABLE(wTable), 4);
	gtk_table_set_col_spacings(GTK_TABLE(wTable), 4);

	GtkWidget *labelFind = gtk_label_new("Find what:");
	AddCell(wTable, labelFind, 0, 0);
	gtk_misc_set_alignment(GTK_MISC(labelFind), 1.0f, 0.5f);
	entryFRFind = gtk_entry_new();
	AddCell(wTable, entryFRFind, 1, 0, true);
	gtk_signal_connect(GTK_OBJECT(entryFRFind), "activate",
		GtkSignalFunc(FRFindSignal), this);

	GtkWidget *labelReplace = gtk_label_new("Replace with:");
	gtk_misc_set_alignment(GTK_MISC(labelReplace), 1.0f, 0.5f);
	AddCell(wTable, labelReplace, 0, 1);
	entryReplace = gtk_entry_new();
	AddCell(wTable, entryReplace, 1, 1, true);
	gtk_signal_connect(GTK_OBJECT(entryReplace), "activate",
		GtkSignalFunc(FRReplaceSignal), this);

	btnFRFind = MakeCommand("F_ind", accelGroup,
		GtkSignalFunc(FRFindSignal), this);
	AddCell(wTable, btnFRFind, 2, 0);

	GtkWidget *btnReplace = MakeCommand("_Replace", accelGroup,
		GtkSignalFunc(FRReplaceSignal), this);
	AddCell(wTable, btnReplace, 2, 1);

	GtkWidget *btnReplaceAll = MakeCommand("Replace _All", accelGroup,
		GtkSignalFunc(FRReplaceAllSignal), this);
	AddCell(wTable, btnReplaceAll, 3, 0);

	btnReplaceInSelection = MakeCommand("I_n Selection", accelGroup,
		GtkSignalFunc(FRReplaceInSelectionSignal), this);
	AddCell(wTable, btnReplaceInSelection, 3, 1);

	GTK_WIDGET_SET_FLAGS(GTK_WIDGET(btnFRFind), GTK_CAN_DEFAULT);

	gtk_signal_connect(GTK_OBJECT(wFindReplace), "key_press_event",
		GtkSignalFunc(PaneSignalKey), this);

	return wFindReplace;
}

void TentacleApp::Run(int argc, char *argv[]) {
	gtk_set_locale();
	gtk_init(&argc, &argv);

	test = 0;

	char fileName[2000];
	strcpy(fileName, "/root/sinkworld/tentacle/cxx/PositionCache.h");

	for (int i=1;i<argc;i++) {
		strcpy(fileName, argv[i]);
	}

	wMain = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	GtkWidget *boxMain = gtk_vbox_new(FALSE, 0);
	gtk_container_add(GTK_CONTAINER(wMain), boxMain);
	GTK_WIDGET_UNSET_FLAGS(boxMain, GTK_CAN_FOCUS);

	gtk_window_set_policy(GTK_WINDOW(wMain), TRUE, TRUE, FALSE);
	gtk_signal_connect(GTK_OBJECT(wMain), "delete_event",
		GTK_SIGNAL_FUNC(exit_app), 0);
	GtkItemFactoryCallback menuSig = GtkItemFactoryCallback(MenuSignal);
	GtkItemFactoryEntry menuItems[] = {
		{"/_File", NULL, NULL, 0, "<Branch>"},
		{"/File/_New", "<control>N", menuSig, IDM_FILE_NEW, 0},
		{"/File/_Open...", "<control>O", menuSig, IDM_FILE_OPEN, 0},
		{"/File/_Save", "<control>S", menuSig, IDM_FILE_SAVE, 0},
		{"/File/Save _As", "", menuSig, IDM_FILE_SAVEAS, 0},
		{"/File/E_xit", "", menuSig, IDM_FILE_EXIT, 0},
		{"/_Edit", NULL, NULL, 0, "<Branch>"},
		{"/Edit/_Undo", "<control>Z", menuSig, IDM_EDIT_UNDO, 0},
		{"/Edit/_Redo", "<control>Y", menuSig, IDM_EDIT_REDO, 0},
		{"/Edit/sep1", NULL, NULL, 0, "<Separator>"},
		{"/Edit/Cu_t", "<control>X", menuSig, IDM_EDIT_CUT, 0},
		{"/Edit/_Copy", "<control>C", menuSig, IDM_EDIT_COPY, 0},
		{"/Edit/_Paste", "<control>V", menuSig, IDM_EDIT_PASTE, 0},
		{"/Edit/_Delete", "Del", menuSig, IDM_EDIT_DELETE, 0},
		{"/Edit/Select _All", "<control>A", menuSig, IDM_EDIT_SELECTALL, 0},
		{"/Edit/sep2", NULL, NULL, 0, "<Separator>"},
		{"/Edit/_Fold", "<control>L", menuSig, ID_EDIT_FOLD, 0},
		{"/Edit/F_old All", NULL, menuSig, ID_EDIT_FOLDALL, 0},
		{"/Edit/S_how All", NULL, menuSig, ID_SHOW_ALL, 0},
		{"/_Search", NULL, NULL, 0, "<Branch>"},
		{"/Search/_Find...", "<control>F", menuSig, IDM_SEARCH_FIND, 0},
		{"/Search/Find _Next", "F3", menuSig, IDM_SEARCH_FINDNEXT, 0},
		{"/Search/_Replace...", "<control>H", menuSig, IDM_SEARCH_REPLACE, 0},
		{"/_Options", NULL, NULL, 0, "<Branch>"},
		//{"/Options/_Brace Level", NULL, menuSig, ID_OPTIONS_BRACELEVEL, "<CheckItem>"},
		//{"/Options/_Fancy", NULL, menuSig, ID_OPTIONS_FANCY, "<CheckItem>"},
		{"/Options/_Wrap", NULL, menuSig, IDM_EDIT_WRAP, "<CheckItem>"},
		{"/Options/_Switch Pane", "F6", menuSig, ID_OPTIONS_SWITCH_PANE, 0},
	};

	allowMenuActions = true;
	accelGroup = gtk_accel_group_new();
	itemFactory = gtk_item_factory_new(GTK_TYPE_MENU_BAR, "<main>", accelGroup);
	char *gthis = reinterpret_cast<char *>(this);
	gtk_item_factory_create_items(itemFactory, sizeof(menuItems) / sizeof(menuItems[0]), menuItems, gthis);
	gtk_window_add_accel_group(GTK_WINDOW(wMain), accelGroup);

	//GtkWidget *boxHandle = gtk_handle_box_new();
	//gtk_container_add(GTK_CONTAINER(boxHandle),
	//                  gtk_item_factory_get_widget(itemFactory, "<main>"));
	//gtk_box_pack_start(GTK_BOX(boxMain), boxHandle, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(boxMain),
		gtk_item_factory_get_widget(itemFactory, "<main>"), FALSE, FALSE, 0);

	GtkWidget *vPane = gtk_vpaned_new();
	for (int c=0;c<controls;c++) {
		tc[c]->CreateControl(wMain);
		//~ gtk_box_pack_end(GTK_BOX(boxMain), tc[c]->TopWindow(), TRUE, TRUE, 0);
	}
	gtk_box_pack_end(GTK_BOX(boxMain), vPane, TRUE, TRUE, 0);
	gtk_paned_add1(GTK_PANED(vPane), tc[0]->TopWindow());
	gtk_paned_add2(GTK_PANED(vPane), tc[1]->TopWindow());
	// Maximize the top pane by making it unreasonably large
	gtk_paned_set_position(GTK_PANED(vPane), 2000);

	gtk_widget_show_all(wMain);

	replacePane = PaneFindReplace();
	gtk_box_pack_start(GTK_BOX(boxMain), replacePane, FALSE, FALSE, 0);
	findPane = PaneFind();
	gtk_box_pack_start(GTK_BOX(boxMain), findPane, FALSE, FALSE, 0);

	app->OpenFile(fileName);

	gtk_main();
}

int main(int argc, char *argv[]) {
	PrototypeRegistry::Init();
	RegisteredDecorations::Init();
	RegisteredLexers::Init();
	Surface::Init();
	app = new TentacleApp();
	app->Run(argc, argv);
	delete app;
	PrototypeRegistry::Finish();
	return 0;
}

#else

///////////////////////////////////////////////////////////////////////////////////
////////////////////////////////// Windows //////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////

void TentacleApp::SizeChanged() {
	SRectangle rcClient;
	GetViewRectangle(&rcClient);
	SRectangle rc0(rcClient);
	Literal sizeTopProportion("size.top.proportion");
	Literal *topProportion = pdoc->options->ModeValue(pdoc->currentMode, &sizeTopProportion);
	if (topProportion) {
		rc0.bottom = (rcClient.bottom + rcClient.top) * topProportion->Value() / 100;
	}
	tc[0]->SetPosition(&rc0);
	SRectangle rc1(rcClient);
	rc1.top = rc0.bottom+1;
	if (rc1.top >= rc1.bottom) {
		rc1.bottom = rc1.top + 1;
	}
	tc[1]->SetPosition(&rc1);
}

void TentacleApp::Open() {
	char openName[MAX_PATH] = "\0";
	OPENFILENAME ofn = {sizeof(OPENFILENAME)};
	ofn.hwndOwner = wMain;
	ofn.hInstance = hInstance;
	ofn.lpstrFile = openName;
	ofn.nMaxFile = sizeof(openName);

	Literal litFilter(".filter");
	WordList *keys = pdoc->options->Keys(&litFilter);
	std::vector<std::string> filterSet;
	for (int k=0;k<keys->Length();k++) {
		Literal *lKey = keys->GetText(k);
		Literal *lVal = pdoc->options->Value(lKey);
		SW_BYTE *sVal = lVal->AsUTF8();
		std::string mItem(reinterpret_cast<char *>(sVal));
		filterSet.push_back(mItem);
		delete []sVal;
		delete lKey;
	}
	delete keys;
	std::sort(filterSet.begin(), filterSet.end());
	std::string filter;
	for (std::vector<std::string>::iterator i = filterSet.begin(); 
		i != filterSet.end(); 
		i++) {
		filter.append(*i + "|");
	}
	std::replace(filter.begin(), filter.end(), '|', '\0');

	ofn.lpstrFilter = filter.c_str();
	ofn.lpstrCustomFilter = 0;
	ofn.nMaxCustFilter = 0;
	ofn.nFilterIndex = 0;
	ofn.lpstrTitle = "Open File";
	ofn.Flags = OFN_HIDEREADONLY;

	if (::GetOpenFileName(&ofn)) {
		OpenFile(openName);
	}
}

void TentacleApp::SaveAs() {
	char openName[MAX_PATH] = "\0";
	strcpy(openName, fullPath);
	OPENFILENAME ofn = {sizeof(ofn)};
	ofn.hwndOwner = wMain;
	ofn.hInstance = hInstance;
	ofn.lpstrFile = openName;
	ofn.nMaxFile = sizeof(openName);
	ofn.lpstrTitle = "Save File";
	ofn.Flags = OFN_HIDEREADONLY;

	if (::GetSaveFileName(&ofn)) {
		strcpy(fullPath, openName);
		SetTitle(true);
		SaveFile(fullPath);
		for (int c=0;c<controls;c++) {
			tc[c]->Invalidate();
		}
	}
}

void TentacleApp::CheckMenuItem(int wIDCheckItem, bool val) {
	if (val)
		::CheckMenuItem(::GetMenu(wMain), wIDCheckItem, MF_CHECKED | MF_BYCOMMAND);
	else
		::CheckMenuItem(::GetMenu(wMain), wIDCheckItem, MF_UNCHECKED | MF_BYCOMMAND);
}

void TentacleApp::ModeMenu(int id) {
	// Mode menu
	wchar_t language[200];
	::GetMenuStringW(::GetMenu(wMain), id, language, 200, MF_BYCOMMAND);
	Literal litItem(language);
	wchar_t *wSpace = wcschr(language, L'\t');
	*wSpace = 0;
	Literal exts(".name");
	WordList *keys = pdoc->options->Keys(&exts);
	for (int k=0;k<keys->Length();k++) {
		Literal *lKey = keys->GetText(k);
		Literal *lVal = pdoc->options->Value(lKey);
		if (litItem.EndsWith(lVal)) {
			delete overrideMode;
			overrideMode = new Literal(language);
			try {
				CreateStyling("X.XXXXX");
			} catch (ConfigurationException *e) {
				delete overrideMode;
				overrideMode = NULL;
				CreateStyling(fullPath);
				delete e;
				DisplayMessage("Mode did not work");
			}
			CheckMenus();
			ControlActive().Invalidate();
			SetTitle(true);
		}
		delete lKey;
	}
	delete keys;
}

void TentacleApp::GetViewRectangle(SRectangle *rc) {
	::GetClientRect(wMain, reinterpret_cast<RECT *>(rc));
}

int TentacleApp::DisplayMessage(char *m, bool ask) {
	return ::MessageBox(wMain, m, appName, ask ? MB_YESNOCANCEL : MB_OK);
}

Literal *LiteralFromEntry(HWND dlg, int id) {
	HWND wFind = ::GetDlgItem(dlg, id);
	if (wFind) {
		int len = static_cast<int>(::SendMessageW(wFind, WM_GETTEXTLENGTH, 0, 0));
		wchar_t *w = new wchar_t[len+1];
		::SendMessageW(wFind, WM_GETTEXT, len+1, reinterpret_cast<LPARAM>(w));
		Literal *litEntry = new Literal(w);
		delete []w;
		return litEntry;
	} else {
		return new Literal();
	}
}

void EntryFromLiteral(Literal *s, HWND dlg, int id) {
	std::wstring utf16 = s->AsWString();
	::SendMessageW(::GetDlgItem(dlg, id), WM_SETTEXT, 0, reinterpret_cast<LPARAM>(utf16.c_str()));
}

void TentacleApp::ReadEntries() {
	delete findWhat;
	findWhat = LiteralFromEntry(currentDialog, IDC_FIND_WHAT);
	delete replaceWhat;
	replaceWhat = LiteralFromEntry(currentDialog, IDREPLACEWITH);
}

BOOL TentacleApp::ReplaceMessage(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
	int cmd = static_cast<int>(wParam & 0xffff);
	switch (message) {
	case WM_INITDIALOG: {
			Literal *wordAtCaret = ControlActive().WordAtPosition(ControlActive().sr.Caret());
			EntryFromLiteral(wordAtCaret, hDlg, IDC_FIND_WHAT);
			delete wordAtCaret;
			::SetFocus(::GetDlgItem(hDlg, IDC_FIND_WHAT));
			return FALSE;
		}
	case WM_COMMAND:
		if ((wParam & 0xffff) == IDCANCEL) {
			::EndDialog(hDlg, IDCANCEL);
			currentDialog = 0;
			return FALSE;
		} else {
			ReadEntries();
			if (cmd == ID_FIND) {
				SearchFindNext();
			} else if (cmd == IDREPLACE) {
				ReplaceOnce();
			} else if (cmd == IDC_REPLACE_ALL) {
				ReplaceAll(false);
			} else if (cmd == IDC_REPLACE_IN_SELECTION) {
				ReplaceAll(true);
			}
			return FALSE;
		}
	}
	return FALSE;
}

BOOL TentacleApp::FindMessage(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_INITDIALOG: {
			Literal *wordAtCaret = ControlActive().WordAtPosition(ControlActive().sr.Caret());
			EntryFromLiteral(wordAtCaret, hDlg, IDC_FIND_WHAT);
			delete wordAtCaret;
			::SetFocus(::GetDlgItem(hDlg, IDC_FIND_WHAT));
			return FALSE;
		}
	case WM_COMMAND:
		if ((wParam & 0xffff) == IDCANCEL) {
			::EndDialog(hDlg, IDCANCEL);
			currentDialog = 0;
			return FALSE;
		} else if ((wParam & 0xffff) == IDOK) {
			delete findWhat;
			findWhat = LiteralFromEntry(hDlg, IDC_FIND_WHAT);
			::EndDialog(hDlg, IDOK);
			SearchFindNext();
			currentDialog = 0;
			return FALSE;
		} else {
			//return HandleReplaceCommand(ControlIDOfCommand(wParam));
		}
	}
	return FALSE;
}

void TentacleApp::Create(HWND wMain_) {
	test = 0;
	wMain = wMain_;
	for (int c=0;c<controls;c++) {
		tc[c]->CreateControl(wMain);
	}
}

LRESULT TentacleApp::ContextMenu(WPARAM wParam, LPARAM lParam) {
	HMENU hmenuPopup = ::CreatePopupMenu();
	::AppendMenu(hmenuPopup, MF_STRING, IDM_EDIT_UNDO, "Undo");
	::AppendMenu(hmenuPopup, MF_STRING, IDM_EDIT_REDO, "Redo");
	::AppendMenu(hmenuPopup, MF_SEPARATOR, 0, "");
	::AppendMenu(hmenuPopup, MF_STRING, IDM_EDIT_CUT, "Cut");
	::AppendMenu(hmenuPopup, MF_STRING, IDM_EDIT_COPY, "Copy");
	::AppendMenu(hmenuPopup, MF_STRING, IDM_EDIT_PASTE, "Paste");
	::AppendMenu(hmenuPopup, MF_STRING, IDM_EDIT_DELETE, "Delete");
	::AppendMenu(hmenuPopup, MF_SEPARATOR, 0, "");
	::AppendMenu(hmenuPopup, MF_STRING, IDM_EDIT_SELECTALL, "Select All");

	::TrackPopupMenu(hmenuPopup,
		0, GET_X_LPARAM(lParam) - 4, GET_Y_LPARAM(lParam), 0,
		wMain, NULL);
	return 0;
}

LRESULT TentacleApp::WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	switch (iMessage) {
	case WM_CREATE:
		Create(hWnd);
		return 0;

	case WM_DESTROY:
		Destroy();
		return 0;

	case WM_SIZE:
		if (wParam != 1) {
			SizeChanged();
		}
		return 0;

	case WM_PAINT: {
			PAINTSTRUCT ps;
			::BeginPaint(hWnd, &ps);
			::SetBkColor(ps.hdc, RGB(0x80,0x80,0x80));
			RECT rectw = {0,0,10000,10000};
			::ExtTextOut(ps.hdc, 0,0, ETO_OPAQUE, &rectw, "", 0, NULL);
			::EndPaint(hWnd, &ps);
			return 0;
		}

	case WM_COMMAND:
		Command(LOWORD(wParam));
		CheckMenus();
		return 0;

	case WM_CONTEXTMENU:
		return ContextMenu(wParam, lParam);

	case WM_MENUSELECT:
		CheckMenus();
		return 0;

	case WM_TIMER:
		return 0;

	case WM_CLOSE:
		if (SaveIfUnsure() != IDCANCEL) {
			Quit();
		}
		return 0;

	case WM_ACTIVATE:
		//Platform::DebugPrintf("Focus: w:%x l:%x %x e=%x o=%x\n", wParam, lParam, ::GetFocus(), wEditor.GetID(), wOutput.GetID());
		if (wParam != WA_INACTIVE) {
			ControlActive().Focus();
		}
		return 0;

	default:
		return ::DefWindowProc(hWnd, iMessage, wParam, lParam);
	}
}

void TentacleApp::AddModeMenu() {
	ReadOptionsAndModes();
	HMENU menu = ::CreateMenu();
	HMENU menuBar = ::GetMenu(wMain);
	int id = idModeStart;
	// Determine mode from file extension
	Literal exts(".name");
	WordList *keys = pdoc->options->Keys(&exts);
	for (int k=0;k<keys->Length();k++) {
		Literal *lKey = keys->GetText(k);
		Literal *lVal = pdoc->options->Value(lKey);
		Literal lKeyNoName(lKey, 0, lKey->Length() - 5);
		std::wstring mItem = lKeyNoName.AsWString() + L'\t' + lVal->AsWString();
		::AppendMenuW(menu, 0, id, mItem.c_str());
		id++;
		delete lKey;
	}
	delete keys;
	::AppendMenuW(menuBar, MF_POPUP, (UINT_PTR)menu, L"&Mode");
	::DrawMenuBar(wMain);
}

LRESULT CALLBACK AppWndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	return app->WndProc(hWnd, iMessage, wParam, lParam);
}

const char appClassName[] = "TentacleAppWindow";

static void RegisterWindowClass() {
	ControlRegisterWindowClass(app->hInstance);

	const char resourceName[] = "TentacleApp";

	WNDCLASS appWndclass;
	appWndclass.style = CS_HREDRAW | CS_VREDRAW;
	appWndclass.lpfnWndProc = AppWndProc;
	appWndclass.cbClsExtra = 0;
	appWndclass.cbWndExtra = 0;
	appWndclass.hInstance = app->hInstance;
	appWndclass.hIcon = 0;
	appWndclass.hCursor = ::LoadCursor(NULL, MAKEINTRESOURCE(IDC_IBEAM));
	appWndclass.hbrBackground = NULL;
	appWndclass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	appWndclass.lpszClassName = appClassName;

	if (!::RegisterClass(&appWndclass))
		::exit(FALSE);
}

int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lpszCmdLine, int nCmdShow) {
	//_CrtSetBreakAlloc(14628);
	PrototypeRegistry::Init();
	RegisteredDecorations::Init();
	RegisteredLexers::Init();
	Surface::Init();
	app = new TentacleApp();
	app->hInstance = hInstance;

	HACCEL hAccTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_ACCELERATOR1));

	RegisterWindowClass();

	::CreateWindowEx(
	             WS_EX_CLIENTEDGE,
	             appClassName,
	             "Demonstration",
	             WS_CAPTION | WS_SYSMENU | WS_THICKFRAME |
	             WS_MINIMIZEBOX | WS_MAXIMIZEBOX |
	             WS_MAXIMIZE | WS_CLIPCHILDREN,
				 1170, 20,
				 500, 700,
	             NULL,
	             NULL,
	             app->hInstance,
	             0);

	char fileName[MAX_PATH];
	//strcpy(fileName, "C:\\os\\sinkworld\\tentacle\\cxx\\BadPositionException.h");
	//strcpy(fileName, "C:\\os\\scintilla\\src\\Editor.cxx");
	//strcpy(fileName, "C:\\os\\sinkworld\\tentacle\\cxx\\ILexer.h");
	//strcpy(fileName, "C:\\os\\Platform.h");
	strcpy(fileName, "C:\\os\\sinkworld\\tentacle\\cxx\\test.asp");
	strcpy(fileName, "C:\\os\\sinkworld\\tentacle\\cxx\\Tentacle.cxx");
	//strcpy(fileName, "C:\\os\\le.txt");

	char cmdLine[2000];
	strcpy(cmdLine, lpszCmdLine);

	char *arg = cmdLine;
	char *next = strchr(arg, ' ');
	while (arg && *arg) {
		if (next) {
			*next = '\0';
		}
		if (arg[0] == '-') {
		} else {
			strcpy(fileName, arg);
		}
		arg = next ? next + 1 : NULL;
		if (arg)
			next = strchr(arg, ' ');
	}
	app->SetTitle(true);
	::ShowWindow(app->wMain, nCmdShow);

	app->OpenFile(fileName);
	app->AddModeMenu();

	bool going = true;
	MSG msg;
	msg.wParam = 0;
	while (going) {
		going = !!GetMessage(&msg, NULL, 0, 0);
		if (app->currentDialog && going) {
			if (!IsDialogMessage(app->currentDialog, &msg)) {
				if (TranslateAccelerator(msg.hwnd, hAccTable, &msg) == 0) {
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}
		} else if (going) {
			if (TranslateAccelerator(app->wMain, hAccTable, &msg) == 0) {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
	}

	delete app;
	PrototypeRegistry::Finish();
	Surface::Finalize();
	_CrtDumpMemoryLeaks();
	return static_cast<int>(msg.wParam);
}
#endif
