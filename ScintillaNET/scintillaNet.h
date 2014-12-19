// Scintilla.h

#pragma once

using namespace System;
using namespace System::Windows::Forms;

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "Scintilla.h"
#include "SciLexer.h"

class ScintillaWin;

#define MAX_LINE_LENGTH 8192

//! Scintilla direct message function.
typedef long(__cdecl* scmsgfn)(void *ptr, long Msg, WPARAM wParam, LPARAM lParam);


extern scmsgfn	Perform;



namespace Scintilla
{

	namespace Configuration
	{
		public __gc class Scintilla;
	}

	public __gc class ScintillaControl;
	
	public __delegate void CharAddedHandler(Scintilla::ScintillaControl * pSender, System::Char CharacterAdded );
	public __delegate StyleNeededHandler( Scintilla::ScintillaControl* pSender, int position );
	public __delegate SavePointReachedHandler( Scintilla::ScintillaControl* pSender);
	public __delegate SavePointLeftHandler( Scintilla::ScintillaControl* pSender );
	public __delegate ModifyAttemptROHandler( Scintilla::ScintillaControl* pSender );
	public __delegate KeyHandler( Scintilla::ScintillaControl* pSender , int ch , int modifiers , int modificationType    );
	public __delegate DoubleClickHandler( Scintilla::ScintillaControl* pSender );
	public __delegate UpdateUIHandler( Scintilla::ScintillaControl* pSender );
	public __delegate ModifiedHandler( Scintilla::ScintillaControl* pSender );
	public __delegate MacroRecordHandler( Scintilla::ScintillaControl* pSender );
	public __delegate MarginClickHandler( Scintilla::ScintillaControl* pSender );
	public __delegate NeedShownHandler( Scintilla::ScintillaControl* pSender );
	public __delegate PaintedHandler( Scintilla::ScintillaControl* pSender );
	public __delegate UserListSelectionHandler( Scintilla::ScintillaControl* pSender , int listType );
	public __delegate UriDroppedHandler( Scintilla::ScintillaControl* pSender );
	public __delegate DwellStartHandler( Scintilla::ScintillaControl* pSender , int position, int x, int y);
	public __delegate DwellEndHandler( Scintilla::ScintillaControl* pSender , int position, int x, int y);
	public __delegate ZoomHandler( Scintilla::ScintillaControl* pSender );
	public __delegate HotSpotClickHandler( Scintilla::ScintillaControl* pSender ,int modifiers, int position);
	public __delegate HotSpotDoubleClickHandler( Scintilla::ScintillaControl* pSender,int modifiers, int position  );
	public __delegate CallTipClickHandler( Scintilla::ScintillaControl* pSender ,int position  );

	// SC_MOD_INSERTTEXT 0x01 Text has been inserted into the document. position, length, linesAdded 
	public __delegate TextInsertedHandler( Scintilla::ScintillaControl* pSender ,int position  , int length , int linesAdded);

	// SC_MOD_DELETETEXT 0x02 Text has been removed from the document. position, length, linesAdded 
	public __delegate TextDeletedHandler( Scintilla::ScintillaControl* pSender ,int position  , int length , int linesRemoved);

	// SC_MOD_CHANGESTYLE 0x04 A style change has occurred. position, length 
	public __delegate StyleChangedHandler( Scintilla::ScintillaControl* pSender ,int position  , int length );

	// SC_MOD_CHANGEFOLD 0x08 A folding change has occurred. line, foldLevelNow, foldLevelPrev 
	public __delegate FoldChangedHandler( Scintilla::ScintillaControl* pSender ,int line, int foldLevelNow, int foldLevelPrev );

	// SC_PERFORMED_USER 0x10 Information: the operation was done by the user. None 
	public __delegate UserPerformedHandler( Scintilla::ScintillaControl* pSender );

	// SC_PERFORMED_UNDO 0x20 Information: this was the result of an Undo. None 
	public __delegate UndoPerformedHandler( Scintilla::ScintillaControl* pSender );

	// SC_PERFORMED_REDO 0x40 Information: this was the result of a Redo. None 
	public __delegate RedoPerformedHandler( Scintilla::ScintillaControl* pSender );

	// SC_LASTSTEPINUNDOREDO 0x100 This is the final step in an Undo or Redo that has several sections. None 
	public __delegate LastStepInUndoRedoHandler( Scintilla::ScintillaControl* pSender );

	// SC_MOD_CHANGEMARKER 0x200 One or more markers has changed in a line. line 
	public __delegate MarkerChangedHandler( Scintilla::ScintillaControl* pSender, int line );

	// SC_MOD_BEFOREINSERT 0x400 Text is about to be inserted into the document. position, length 
	public __delegate BeforeInsertHandler( Scintilla::ScintillaControl* pSender ,int position  , int length);

	// SC_MOD_BEFOREDELETE 0x800 Text is about to be deleted from the document. position, length 
	public __delegate BeforeDeleteHandler( Scintilla::ScintillaControl* pSender,int position  , int length );


	[event_source(managed)]
	public __gc class ScintillaControl : public Control  
	{
      public:

      static int LEX_CONTAINER=SCLEX_CONTAINER;
      static int LEX_NULL=SCLEX_NULL;
      static int LEX_PYTHON=SCLEX_PYTHON;
      static int LEX_CPP=SCLEX_CPP;
      static int LEX_HTML=SCLEX_HTML;
      static int LEX_XML=SCLEX_XML;
      static int LEX_PERL=SCLEX_PERL;
      static int LEX_SQL=SCLEX_SQL;
      static int LEX_VB=SCLEX_VB;
      static int LEX_PROPERTIES=SCLEX_PROPERTIES;
      static int LEX_ERRORLIST=SCLEX_ERRORLIST;
      static int LEX_MAKEFILE=SCLEX_MAKEFILE;
      static int LEX_BATCH=SCLEX_BATCH;
      static int LEX_XCODE=SCLEX_XCODE;
      static int LEX_LATEX=SCLEX_LATEX;
      static int LEX_LUA=SCLEX_LUA;
      static int LEX_DIFF=SCLEX_DIFF;
      static int LEX_CONF=SCLEX_CONF;
      static int LEX_PASCAL=SCLEX_PASCAL;
      static int LEX_AVE=SCLEX_AVE;
      static int LEX_ADA=SCLEX_ADA;
      static int LEX_LISP=SCLEX_LISP;
      static int LEX_RUBY=SCLEX_RUBY;
      static int LEX_EIFFEL=SCLEX_EIFFEL;
      static int LEX_EIFFELKW=SCLEX_EIFFELKW;
      static int LEX_TCL=SCLEX_TCL;
      static int LEX_NNCRONTAB=SCLEX_NNCRONTAB;
      static int LEX_BULLANT=SCLEX_BULLANT;
      static int LEX_VBSCRIPT=SCLEX_VBSCRIPT;
      static int LEX_ASP=SCLEX_ASP;
      static int LEX_PHP=SCLEX_PHP;
      static int LEX_BAAN=SCLEX_BAAN;
      static int LEX_MATLAB=SCLEX_MATLAB;
      static int LEX_SCRIPTOL=SCLEX_SCRIPTOL;
      static int LEX_ASM=SCLEX_ASM;
      static int LEX_CPPNOCASE=SCLEX_CPPNOCASE;
      static int LEX_FORTRAN=SCLEX_FORTRAN;
      static int LEX_F77=SCLEX_F77;
      static int LEX_CSS=SCLEX_CSS;
      static int LEX_POV=SCLEX_POV;
      static int LEX_LOUT=SCLEX_LOUT;
      static int LEX_ESCRIPT=SCLEX_ESCRIPT;
      static int LEX_PS=SCLEX_PS;
      static int LEX_NSIS=SCLEX_NSIS;
      static int LEX_MMIXAL=SCLEX_MMIXAL;
      static int LEX_CLW=SCLEX_CLW;
      static int LEX_CLWNOCASE=SCLEX_CLWNOCASE;
	  static int LEX_LOT=SCLEX_LOT;
	  static int LEX_YAML=SCLEX_YAML;
	  static int LEX_TEX=SCLEX_TEX;
	  static int LEX_METAPOST=SCLEX_METAPOST;
	  static int LEX_POWERBASIC=SCLEX_POWERBASIC;
	  static int LEX_FORTH=SCLEX_FORTH;
      
	private:
		HWND hwndScintilla;
		ScintillaWin* _scintillaWin;
		System::Collections::Hashtable* ignoredKeys;
		Scintilla::Configuration::Scintilla* _configuration;

		//! Used for Scintilla's GetDirectPointer
		void	*m_Pointer;

	protected:
		virtual void WndProc( Message* m );
		virtual void addShortcuts( Menu* m );

	public:
		ScintillaControl();
		virtual void AddIgnoredKey(System::Windows::Forms::Keys key , System::Windows::Forms::Keys modifier );
		virtual void AddIgnoredKey(System::Windows::Forms::Shortcut shortcutkey );
		virtual void AddShortcutsFromForm(System::Windows::Forms::Form* parentForm);
		
		virtual bool PreProcessMessage(Message* m);

		__event CharAddedHandler* CharAdded;
		__event StyleNeededHandler* StyleNeeded;
		__event SavePointReachedHandler* SavePointReached;
		__event SavePointLeftHandler* SavePointLeft;
		__event ModifyAttemptROHandler* ModifyAttemptRO;
		__event KeyHandler* Key;
		__event DoubleClickHandler* DoubleClick;
		__event UpdateUIHandler* UpdateUI;
		__event ModifiedHandler* Modified;
		__event MacroRecordHandler* MacroRecord;
		__event MarginClickHandler* MarginClick;
		__event NeedShownHandler* NeedShown;
		__event PaintedHandler* Painted;
		__event UserListSelectionHandler* UserListSelection;
		__event UriDroppedHandler* UriDropped;
		__event DwellStartHandler* DwellStart;
		__event DwellEndHandler* DwellEnd;
		__event ZoomHandler* Zoom;
		__event HotSpotClickHandler* HotSpotClick;
		__event HotSpotDoubleClickHandler* HotSpotDoubleClick;
		__event CallTipClickHandler* CallTipClick;

		__event TextInsertedHandler* TextInserted;
		__event TextDeletedHandler* TextDeleted;
		__event StyleChangedHandler* StyleChanged;
		__event FoldChangedHandler* FoldChanged;
		__event UserPerformedHandler* UserPerformed;
		__event UndoPerformedHandler* UndoPerformed;
		__event RedoPerformedHandler* RedoPerformed;
		__event LastStepInUndoRedoHandler* LastStepInUndoRedo;
		__event MarkerChangedHandler* MarkerChanged;
		__event BeforeInsertHandler* BeforeInsert;
		__event BeforeDeleteHandler* BeforeDelete;

		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property String* get_Line(int line);

		/**
		 * Retrieve the selected text.
		 * Return the length of the text.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property String* get_SelectedText();

		/**
		 * Retrieve all the text in the document.
		 * Returns number of characters retrieved.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property String* get_Text();

		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property String* get_CurrentLine();



		/**
		* Sets the configuration object that the control will use to set styles.
		*/
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property void set_Configuration( Scintilla::Configuration::Scintilla* config);

		/**
		* Gets the configuration object that the control will use to set styles.
		*/
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property Scintilla::Configuration::Scintilla* get_Configuration( );

		/**
		* Changes the language for the style profile.
		*/
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property void set_ConfigurationLanguage( String* language );

		/**
		* Gets the language for the style profile.
		*/
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property String* get_ConfigurationLanguage( );

		/**
		* Watches the parent class for resize events.
		*/
		void doResize(Object* sender, EventArgs* e);

		/**
		 * Find some text starting at the search anchor and moving backwards.
		 * Does not ensure the selection is visible.
		 */
		int SearchPrev(int flags, System::String* text);
		/**
		 * Add text to the document.
		 */
		void AddText(int length, String* text);
		/**
		 * Add array of cells to document.
		 */
		void AddStyledText(int length, String* c);
		/**
		 * Insert string at a position.
		 */
		void InsertText(long pos, String* text);
		/**
		 * Delete all text in the document.
		 */
		void ClearAll();
		/**
		 * Set all style bytes to 0, remove all folding information.
		 */
		void ClearDocumentStyle();
		/**
		 * Redoes the next action on the undo history.
		 */
		void Redo();
		/**
		 * Select all the text in the document.
		 */
		void SelectAll();
		/**
		 * Remember the current position in the undo history as the position
		 * at which the document was saved.
		 */
		void SetSavePoint();
		/**
		 * Retrieve a buffer of cells.
		 * Returns the number of bytes in the buffer not including terminating nulls.
		 */
		int GetStyledText(TextRange* tr);
		/**
		 * Retrieve the line number at which a particular marker is located.
		 */
		int MarkerLineFromHandle(int handle);
		/**
		 * Delete a marker.
		 */
		void MarkerDeleteHandle(int handle);
		/**
		 * Find the position from a point within the window.
		 */
		int PositionFromPoint(int x, int y);
		/**
		 * Find the position from a point within the window but return
		 * INVALID_POSITION if not close to text.
		 */
		int PositionFromPointClose(int x, int y);
		/**
		 * Set caret to start of a line and ensure it is visible.
		 */
		void GotoLine(int line);
		/**
		 * Set caret to a position and ensure it is visible.
		 */
		void GotoPos(long pos);
		/**
		 * Convert all line endings in the document to one mode.
		 */
		void ConvertEOLs(int eolMode);
		/**
		 * Set the current styling position to pos and the styling mask to mask.
		 * The styling mask can be used to protect some bits in each styling byte from modification.
		 */
		void StartStyling(long pos, int mask);
		/**
		 * Change style from current styling position for length characters to a style
		 * and move the current styling position to after this newly styled segment.
		 */
		void SetStyling(int length, int style);
		/**
		 * Set the symbol used for a particular marker number.
		 */
		void MarkerDefine(int markerNumber, int markerSymbol);
		/**
		 * Set the foreground colour used for a particular marker number.
		 */
		void MarkerSetFore(int markerNumber, long fore);
		/**
		 * Set the background colour used for a particular marker number.
		 */
		void MarkerSetBack(int markerNumber, long back);
		/**
		 * Add a marker to a line, returning an ID which can be used to find or delete the marker.
		 */
		int MarkerAdd(int line, int markerNumber);
		/**
		 * Delete a marker from a line.
		 */
		void MarkerDelete(int line, int markerNumber);
		/**
		 * Delete all markers with a particular number from all lines.
		 */
		void MarkerDeleteAll(int markerNumber);
		/**
		 * Get a bit mask of all the markers set on a line.
		 */
		int MarkerGet(int line);
		/**
		 * Find the next line after lineStart that includes a marker in mask.
		 */
		int MarkerNext(int lineStart, int markerMask);
		/**
		 * Find the previous line before lineStart that includes a marker in mask.
		 */
		int MarkerPrevious(int lineStart, int markerMask);
		/**
		 * Define a marker from a pixmap.
		 */
		void MarkerDefinePixmap(int markerNumber, String* pixmap);
		/**
		 * Clear all the styles and make equivalent to the global default style.
		 */
		void StyleClearAll();
		/**
		 * Set the foreground colour of a style.
		 */
		void StyleSetFore(int style, long fore);
		/**
		 * Set the background colour of a style.
		 */
		void StyleSetBack(int style, long back);
		/**
		 * Set a style to be bold or not.
		 */
		void StyleSetBold(int style, bool bold);
		/**
		 * Set a style to be italic or not.
		 */
		void StyleSetItalic(int style, bool italic);
		/**
		 * Set the size of characters of a style.
		 */
		void StyleSetSize(int style, int sizePoints);
		/**
		 * Set the font of a style.
		 */
		void StyleSetFont(int style, String* fontName);
		/**
		 * Set a style to have its end of line filled or not.
		 */
		void StyleSetEOLFilled(int style, bool filled);
		/**
		 * Reset the default style to its state at startup
		 */
		void StyleResetDefault();
		/**
		 * Set a style to be underlined or not.
		 */
		void StyleSetUnderline(int style, bool underline);
		/**
		 * Set a style to be mixed case, or to force upper or lower case.
		 */
		void StyleSetCase(int style, int caseForce);
		/**
		 * Set the character set of the font in a style.
		 */
		void StyleSetCharacterSet(int style, int characterSet);
		/**
		 * Set the foreground colour of the selection and whether to use this setting.
		 */
		void SetSelFore(bool useSetting, long fore);
		/**
		 * Set the background colour of the selection and whether to use this setting.
		 */
		void SetSelBack(bool useSetting, long back);
		/**
		 * When key+modifier combination km is pressed perform msg.
		 */
		void AssignCmdKey(long km, int msg);
		/**
		 * When key+modifier combination km do nothing.
		 */
		void ClearCmdKey(long km);
		/**
		 * Drop all key mappings.
		 */
		void ClearAllCmdKeys();
		/**
		 * Set the styles for a segment of the document.
		 */
		void SetStylingEx(int length, String* styles);
		/**
		 * Set a style to be visible or not.
		 */
		void StyleSetVisible(int style, bool visible);
		/**
		 * Set a style to be a hotspot or not.
		 */
		void StyleSetHotspot(int style, bool hotspot);
		/**
		 * Start a sequence of actions that is undone and redone as a unit.
		 * May be nested.
		 */
		void BeginUndoAction();
		/**
		 * End a sequence of actions that is undone and redone as a unit.
		 */
		void EndUndoAction();
		/**
		 * Set an indicator to plain, squiggle or TT.
		 */
		void IndicSetStyle(int indic, int style);
		/**
		 * Retrieve the style of an indicator.
		 */
		int IndicGetStyle(int indic);
		/**
		 * Set the foreground colour of an indicator.
		 */
		void IndicSetFore(int indic, long fore);
		/**
		 * Retrieve the foreground colour of an indicator.
		 */
		long IndicGetFore(int indic);
		/**
		 * Set the foreground colour of all whitespace and whether to use this setting.
		 */
		void SetWhitespaceFore(bool useSetting, long fore);
		/**
		 * Set the background colour of all whitespace and whether to use this setting.
		 */
		void SetWhitespaceBack(bool useSetting, long back);
		/**
		 * Set a style to be changeable or not (read only).
		 * Experimental feature, currently buggy.
		 */
		void StyleSetChangeable(int style, bool changeable);
		/**
		 * Display a auto-completion list.
		 * The lenEntered parameter indicates how many characters before
		 * the caret should be used to provide context.
		 */
		void AutoCShow(int lenEntered, String* itemList);
		/**
		 * Remove the auto-completion list from the screen.
		 */
		void AutoCCancel();
		/**
		 * Is there an auto-completion list visible?
		 */
		bool AutoCActive();
		/**
		 * Retrieve the position of the caret when the auto-completion list was displayed.
		 */
		long AutoCPosStart();
		/**
		 * User has selected an item so remove the list and insert the selection.
		 */
		void AutoCComplete();
		/**
		 * Define a set of character that when typed cancel the auto-completion list.
		 */
		void AutoCStops(String* characterSet);
		/**
		 * Change the separator character in the string setting up an auto-completion list.
		 * Default is space but can be changed if items contain space.
		 */
		void AutoCSetSeparator(int separatorCharacter);
		/**
		 * Retrieve the auto-completion list separator character.
		 */
		int AutoCGetSeparator();
		/**
		 * Select the item in the auto-completion list that starts with a string.
		 */
		void AutoCSelect(String* text);
		/**
		 * Should the auto-completion list be cancelled if the user backspaces to a
		 * position before where the box was created.
		 */
		void AutoCSetCancelAtStart(bool cancel);
		/**
		 * Retrieve whether auto-completion cancelled by backspacing before start.
		 */
		bool AutoCGetCancelAtStart();
		/**
		 * Define a set of characters that when typed will cause the autocompletion to
		 * choose the selected item.
		 */
		void AutoCSetFillUps(String* characterSet);
		/**
		 * Should a single item auto-completion list automatically choose the item.
		 */
		void AutoCSetChooseSingle(bool chooseSingle);
		/**
		 * Retrieve whether a single item auto-completion list automatically choose the item.
		 */
		bool AutoCGetChooseSingle();
		/**
		 * Set whether case is significant when performing auto-completion searches.
		 */
		void AutoCSetIgnoreCase(bool ignoreCase);
		/**
		 * Retrieve state of ignore case flag.
		 */
		bool AutoCGetIgnoreCase();
		/**
		 * Display a list of strings and send notification when user chooses one.
		 */
		void UserListShow(int listType, String* itemList);
		/**
		 * Set whether or not autocompletion is hidden automatically when nothing matches.
		 */
		void AutoCSetAutoHide(bool autoHide);
		/**
		 * Retrieve whether or not autocompletion is hidden automatically when nothing matches.
		 */
		bool AutoCGetAutoHide();
		/**
		 * Set whether or not autocompletion deletes any word characters
		 * after the inserted text upon completion.
		 */
		void AutoCSetDropRestOfWord(bool dropRestOfWord);
		/**
		 * Retrieve whether or not autocompletion deletes any word characters
		 * after the inserted text upon completion.
		 */
		bool AutoCGetDropRestOfWord();
		/**
		 * Register an XPM image for use in autocompletion lists.
		 */
		void RegisterImage(int type, String* xpmData);
		/**
		 * Clear all the registered XPM images.
		 */
		void ClearRegisteredImages();
		/**
		 * Change the indentation of a line to a number of columns.
		 */
		void SetLineIndentation(int line, int indentSize);
		/**
		 * Retrieve the number of columns that a line is indented.
		 */
		int GetLineIndentation(int line);
		/**
		 * Retrieve the position before the first non indentation character on a line.
		 */
		long GetLineIndentPosition(int line);
		/**
		 * Retrieve the column number of a position, taking tab width into account.
		 */
		int GetColumn(long pos);
		/**
		 * Get the position after the last visible characters on a line.
		 */
		int GetLineEndPosition(int line);
		/**
		 * Find some text in the document.
		 */
		long FindText(int flags, TextToFind* ft);
		/**
		 * On Windows, will draw the document into a display context such as a printer.
		 */
		void FormatRange(bool draw, long fr);
		/**
		 * Retrieve the contents of a line.
		 * Returns the length of the line.
		 */
		String* GetLine(int line);
		/**
		 * Select a range of text.
		 */
		void SetSel(long start, long end);
		/**
		 * Retrieve the selected text.
		 * Return the length of the text.
		 */
		String* GetSelText();
		/**
		 * Retrieve a range of text.
		 * Return the length of the text.
		 */
		int GetTextRange(TextRange* tr);
		/**
		 * Draw the selection in normal style or with selection highlighted.
		 */
		void HideSelection(bool normal);
		/**
		 * Retrieve the x value of the point in the window where a position is displayed.
		 */
		int PointXFromPosition(long pos);
		/**
		 * Retrieve the y value of the point in the window where a position is displayed.
		 */
		int PointYFromPosition(long pos);
		/**
		 * Retrieve the line containing a position.
		 */
		int LineFromPosition(long pos);
		/**
		 * Retrieve the position at the start of a line.
		 */
		int PositionFromLine(int line);
		/**
		 * Scroll horizontally and vertically.
		 */
		void LineScroll(int columns, int lines);
		/**
		 * Ensure the caret is visible.
		 */
		void ScrollCaret();
		/**
		 * Replace the selected text with the argument text.
		 */
		void ReplaceSel(String* text);
		/**
		 * Null operation.
		 */
		void Null();
		/**
		 * Delete the undo history.
		 */
		void EmptyUndoBuffer();
		/**
		 * Undo one action in the undo history.
		 */
		void Undo();
		/**
		 * Cut the selection to the clipboard.
		 */
		void Cut();
		/**
		 * Copy the selection to the clipboard.
		 */
		void Copy();
		/**
		 * Paste the contents of the clipboard into the document replacing the selection.
		 */
		void Paste();
		/**
		 * Clear the selection.
		 */
		void Clear();
		/**
		 * Retrieve a pointer to a function that processes messages for this Scintilla.
		 */
		int GetDirectFunction();
		/**
		 * Retrieve a pointer value to use as the first argument when calling
		 * the function returned by GetDirectFunction.
		 */
		int GetDirectPointer();
		/**
		 * Replace the target text with the argument text.
		 * Text is counted so it can contain nulls.
		 * Returns the length of the replacement text.
		 */
		int ReplaceTarget(int length, String* text);
		/**
		 * Replace the target text with the argument text after \d processing.
		 * Text is counted so it can contain nulls.
		 * Looks for \d where d is between 1 and 9 and replaces these with the strings
		 * matched in the last search operation which were surrounded by \( and \).
		 * Returns the length of the replacement text including any change
		 * caused by processing the \d patterns.
		 */
		int ReplaceTargetRE(int length, String* text);
		/**
		 * Search for a counted string in the target and set the target to the found
		 * range. Text is counted so it can contain nulls.
		 * Returns length of range or -1 for failure in which case target is not moved.
		 */
		int SearchInTarget(int length, String* text);
		/**
		 * Show a call tip containing a definition near position pos.
		 */
		void CallTipShow(long pos, String* definition);
		/**
		 * Remove the call tip from the screen.
		 */
		void CallTipCancel();
		/**
		 * Is there an active call tip?
		 */
		bool CallTipActive();
		/**
		 * Retrieve the position where the caret was before displaying the call tip.
		 */
		long CallTipPosStart();
		/**
		 * Highlight a segment of the definition.
		 */
		void CallTipSetHlt(int start, int end);
		/**
		 * Set the background colour for the call tip.
		 */
		void CallTipSetBack(long back);
		/**
		 * Set the foreground colour for the call tip.
		 */
		void CallTipSetFore(long fore);
		/**
		 * Set the foreground colour for the highlighted part of the call tip.
		 */
		void CallTipSetForeHlt(long fore);
		/**
		 * Find the display line of a document line taking hidden lines into account.
		 */
		int VisibleFromDocLine(int line);
		/**
		 * Find the document line of a display line taking hidden lines into account.
		 */
		int DocLineFromVisible(int lineDisplay);
		/**
		 * Set the fold level of a line.
		 * This encodes an integer level along with flags indicating whether the
		 * line is a header and whether it is effectively white space.
		 */
		void SetFoldLevel(int line, int level);
		/**
		 * Retrieve the fold level of a line.
		 */
		int GetFoldLevel(int line);
		/**
		 * Find the last child line of a header line.
		 */
		int GetLastChild(int line, int level);
		/**
		 * Find the parent line of a child line.
		 */
		int GetFoldParent(int line);
		/**
		 * Make a range of lines visible.
		 */
		void ShowLines(int lineStart, int lineEnd);
		/**
		 * Make a range of lines invisible.
		 */
		void HideLines(int lineStart, int lineEnd);
		/**
		 * Is a line visible?
		 */
		bool GetLineVisible(int line);
		/**
		 * Show the children of a header line.
		 */
		void SetFoldExpanded(int line, bool expanded);
		/**
		 * Is a header line expanded?
		 */
		bool GetFoldExpanded(int line);
		/**
		 * Switch a header line between expanded and contracted.
		 */
		void ToggleFold(int line);
		/**
		 * Ensure a particular line is visible by expanding any header line hiding it.
		 */
		void EnsureVisible(int line);
		/**
		 * Set some style options for folding.
		 */
		void SetFoldFlags(int flags);
		/**
		 * Ensure a particular line is visible by expanding any header line hiding it.
		 * Use the currently set visibility policy to determine which range to display.
		 */
		void EnsureVisibleEnforcePolicy(int line);
		/**
		 * Get position of start of word.
		 */
		int WordStartPosition(long pos, bool onlyWordCharacters);
		/**
		 * Get position of end of word.
		 */
		int WordEndPosition(long pos, bool onlyWordCharacters);
		/**
		 * Measure the pixel width of some text in a particular style.
		 * Nul terminated text argument.
		 * Does not handle tab or control characters.
		 */
		int TextWidth(int style, String* text);
		/**
		 * Retrieve the height of a particular line of text in pixels.
		 */
		int TextHeight(int line);
		/**
		 * Append a string to the end of the document without changing the selection.
		 */
		void AppendText(int length, String* text);
		/**
		 * Move caret down one line.
		 */
		void LineDown();
		/**
		 * Move caret down one line extending selection to new caret position.
		 */
		void LineDownExtend();
		/**
		 * Move caret up one line.
		 */
		void LineUp();
		/**
		 * Move caret up one line extending selection to new caret position.
		 */
		void LineUpExtend();
		/**
		 * Move caret left one character.
		 */
		void CharLeft();
		/**
		 * Move caret left one character extending selection to new caret position.
		 */
		void CharLeftExtend();
		/**
		 * Move caret right one character.
		 */
		void CharRight();
		/**
		 * Move caret right one character extending selection to new caret position.
		 */
		void CharRightExtend();
		/**
		 * Move caret left one word.
		 */
		void WordLeft();
		/**
		 * Move caret left one word extending selection to new caret position.
		 */
		void WordLeftExtend();
		/**
		 * Move caret right one word.
		 */
		void WordRight();
		/**
		 * Move caret right one word extending selection to new caret position.
		 */
		void WordRightExtend();
		/**
		 * Move caret to first position on line.
		 */
		void Home();
		/**
		 * Move caret to first position on line extending selection to new caret position.
		 */
		void HomeExtend();
		/**
		 * Move caret to last position on line.
		 */
		void LineEnd();
		/**
		 * Move caret to last position on line extending selection to new caret position.
		 */
		void LineEndExtend();
		/**
		 * Move caret to first position in document.
		 */
		void DocumentStart();
		/**
		 * Move caret to first position in document extending selection to new caret position.
		 */
		void DocumentStartExtend();
		/**
		 * Move caret to last position in document.
		 */
		void DocumentEnd();
		/**
		 * Move caret to last position in document extending selection to new caret position.
		 */
		void DocumentEndExtend();
		/**
		 * Move caret one page up.
		 */
		void PageUp();
		/**
		 * Move caret one page up extending selection to new caret position.
		 */
		void PageUpExtend();
		/**
		 * Move caret one page down.
		 */
		void PageDown();
		/**
		 * Move caret one page down extending selection to new caret position.
		 */
		void PageDownExtend();
		/**
		 * Switch from insert to overtype mode or the reverse.
		 */
		void EditToggleOvertype();
		/**
		 * Cancel any modes such as call tip or auto-completion list display.
		 */
		void Cancel();
		/**
		 * Delete the selection or if no selection, the character before the caret.
		 */
		void DeleteBack();
		/**
		 * If selection is empty or all on one line replace the selection with a tab character.
		 * If more than one line selected, indent the lines.
		 */
		void Tab();
		/**
		 * Dedent the selected lines.
		 */
		void BackTab();
		/**
		 * Insert a new line, may use a CRLF, CR or LF depending on EOL mode.
		 */
		void NewLine();
		/**
		 * Insert a Form Feed character.
		 */
		void FormFeed();
		/**
		 * Move caret to before first visible character on line.
		 * If already there move to first character on line.
		 */
		void VCHome();
		/**
		 * Like VCHome but extending selection to new caret position.
		 */
		void VCHomeExtend();
		/**
		 * Magnify the displayed text by increasing the sizes by 1 point.
		 */
		void ZoomIn();
		/**
		 * Make the displayed text smaller by decreasing the sizes by 1 point.
		 */
		void ZoomOut();
		/**
		 * Delete the word to the left of the caret.
		 */
		void DelWordLeft();
		/**
		 * Delete the word to the right of the caret.
		 */
		void DelWordRight();
		/**
		 * Cut the line containing the caret.
		 */
		void LineCut();
		/**
		 * Delete the line containing the caret.
		 */
		void LineDelete();
		/**
		 * Switch the current line with the previous.
		 */
		void LineTranspose();
		/**
		 * Duplicate the current line.
		 */
		void LineDuplicate();
		/**
		 * Transform the selection to lower case.
		 */
		void LowerCase();
		/**
		 * Transform the selection to upper case.
		 */
		void UpperCase();
		/**
		 * Scroll the document down, keeping the caret visible.
		 */
		void LineScrollDown();
		/**
		 * Scroll the document up, keeping the caret visible.
		 */
		void LineScrollUp();
		/**
		 * Delete the selection or if no selection, the character before the caret.
		 * Will not delete the character before at the start of a line.
		 */
		void DeleteBackNotLine();
		/**
		 * Move caret to first position on display line.
		 */
		void HomeDisplay();
		/**
		 * Move caret to first position on display line extending selection to
		 * new caret position.
		 */
		void HomeDisplayExtend();
		/**
		 * Move caret to last position on display line.
		 */
		void LineEndDisplay();
		/**
		 * Move caret to last position on display line extending selection to new
		 * caret position.
		 */
		void LineEndDisplayExtend();
		/**
		 * Copy the line containing the caret.
		 */
		void LineCopy();
		/**
		 * Move the caret inside current view if it's not there already.
		 */
		void MoveCaretInsideView();
		/**
		 * How many characters are on a line, not including end of line characters?
		 */
		int LineLength(int line);
		/**
		 * Highlight the characters at two positions.
		 */
		void BraceHighlight(long pos1, long pos2);
		/**
		 * Highlight the character at a position indicating there is no matching brace.
		 */
		void BraceBadLight(long pos);
		/**
		 * Find the position of a matching brace or INVALID_POSITION if no match.
		 */
		long BraceMatch(long pos);
		/**
		 * Retrieve a pointer to the document object.
		 */
		int GetDocPointer();
		/**
		 * Change the document object used.
		 */
		void SetDocPointer(int pointer);
		/**
		 * Sets the current caret position to be the search anchor.
		 */
		void SearchAnchor();
		/**
		 * Find some text starting at the search anchor.
		 * Does not ensure the selection is visible.
		 */
		int SearchNext(int flags, String* text);
		/**
		 * Create a new document object.
		 * Starts with reference count of 1 and not selected into editor.
		 */
		int CreateDocument();
		/**
		 * Extend life of document.
		 */
		void AddRefDocument(int doc);
		/**
		 * Release a reference to the document, deleting document if it fades to black.
		 */
		void ReleaseDocument(int doc);
		/**
		 * Move to the previous change in capitalisation.
		 */
		void WordPartLeft();
		/**
		 * Move to the previous change in capitalisation extending selection
		 * to new caret position.
		 */
		void WordPartLeftExtend();
		/**
		 * Move to the change next in capitalisation.
		 */
		void WordPartRight();
		/**
		 * Move to the next change in capitalisation extending 
		 * selection to new caret position.
		 */
		void WordPartRightExtend();
		/**
		 * Set the way the display area is determined 
		 * when a particular line
		 * is to be moved to by Find, FindNext, GotoLine, etc.
		 */
		void SetVisiblePolicy(int visiblePolicy, int visibleSlop);
		/**
		 * Delete back from the current position to 
		 * the start of the line.
		 */
		void DelLineLeft();
		/**
		 * Delete forwards from the current position to the 
		 * end of the line.
		 */
		void DelLineRight();
		/**
		 * Set the last x chosen value to be the caret 
		 * x position
		 */
		void ChooseCaretX();
		/**
		 * Set the focus to this Scintilla widget.
		 * GTK+ Specific.
		 */
		void GrabFocus();
		/**
		 * Set the way the caret is kept visible when going sideway.
		 * The exclusion zone is given in pixels.
		 */
		void SetXCaretPolicy(int caretPolicy, int caretSlop);
		/**
		 * Set the way the line the caret is on is kept visible.
		 * The exclusion zone is given in lines.
		 */
		void SetYCaretPolicy(int caretPolicy, int caretSlop);
		/**
		 * Start notifying the container of all key presses and 
		 * commands.
		 */
		void StartRecord();
		/**
		* Stop notifying the container of all key presses and 
		* commands.
		 */
		void StopRecord();
		/**
		 * Colourise a segment of the document using the 
		 * current lexing language.
		 */
		void Colourise(long start, long end);
		/**
		 * Set up a value that may be used by a lexer for some 
		 * optional feature.
		 */

		/**
		 * Is drawing done in two phases with backgrounds drawn before faoregrounds?
		 */
		bool GetTwoPhaseDraw();

		/**
		 * In twoPhaseDraw mode, drawing is performed in two phases, first the background
		 * and then the foreground. This avoids chopping off characters that overlap the next run.
		 */
		void SetTwoPhaseDraw(bool twoPhase);

		/**
		 * Make the target range start and end be the same as the selection range start and end.
		 */
		void TargetFromSelection();

		/**
		 * Join the lines in the target.
		 */
		void LinesJoin();

		/**
		 * Split the lines in the target into lines that are less wide than pixelWidth
		 * where possible.
		 */
		void LinesSplit(int pixelWidth);

		/**
		 * Set the colours used as a chequerboard pattern in the fold margin
		 */
		void SetFoldMarginColour(bool useSetting, long back);
		void SetFoldMarginHiColour(bool useSetting, long fore);

		/**
		 * These are like their namesakes Home(Extend)?, LineEnd(Extend)?, VCHome(Extend)?
		 * except they behave differently when word-wrap is enabled:
		 * They go first to the start / end of the display line, like (Home|LineEnd)Display
		 * The difference is that, the cursor is already at the point, it goes on to the start
		 * or end of the document line, as appropriate for (Home|LineEnd|VCHome)Extend.
		 */

		void HomeWrap();
		void HomeWrapExtend();
		void LineEndWrap();
		void LineEndWrapExtend();
		void VCHomeWrap();
		void VCHomeWrapExtend();

		/**
		 * Set a fore colour for active hotspots.
		 */
		void SetHotspotActiveFore(bool useSetting, long fore);

		/**
		 * Set a back colour for active hotspots.
		 */
		void SetHotspotActiveBack(bool useSetting, long back);

		/**
		 * Enable / Disable underlining active hotspots.
		 */
		void SetHotspotActiveUnderline(bool underline);

		/**
		*  Limit hotspots to single line so hotspots on two lines don't merge.
		*/
		void SetHotspotSingleLine(bool singleLine);

		/**
		 * Move caret between paragraphs (delimited by empty lines)
		 */
		void ParaDown();
		void ParaDownExtend();
		void ParaUp();
		void ParaUpExtend();

		/**
		 * Given a valid document position, return the previous position taking code
		 * page into account. Returns 0 if passed 0.
		 */
		long PositionBefore(long pos);

		/**
		 * Given a valid document position, return the next position taking code
		 * page into account. Maximum value returned is the last position in the document.
		 */
		long PositionAfter(long pos);

		/** 
		* Copy a range of text to the clipboard. Positions are clipped into the document.
		*/
		void CopyRange(long start, long end);

		/** 
		* Copy argument text to the clipboard.
		*/
		void CopyText(long length, String* text);

		/// ------------------- .Net Frienly Code follows here. --------------------------------
		/**
		 * Retrieve the auto-completion list type-separator character.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property int get_AutoCGetTypeSeparator();

		/**
		 * Change the type-separator character in the string setting up an auto-completion list.
		 * Default is '?' but can be changed if items contain '?'.
		*/ 
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property void set_AutoCSetTypeSeparator(int separatorCharacter);


		/**
		 * Set the code page used to interpret the bytes of the document as characters.
		 * The SC_CP_UTF8 value can be used to enter Unicode mode.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property void set_CodePage(int codePage);
		/**
		 * In palette mode, Scintilla uses the environment's palette calls to display
		 * more colours. This may lead to ugly displays.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property void set_UsePalette(bool usePalette);
		/**
		 * Set the set of characters making up words for when moving or selecting by word.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property void set_WordChars(String* characters);
		/**
		 * Divide each styling byte into lexical class bits (default: 5) and indicator
		 * bits (default: 3). If a lexer requires more than 32 lexical states, then this
		 * is used to expand the possible states.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property void set_StyleBits(int bits);
		/**
		 * Retrieve number of bits in style bytes used to hold the lexical state.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property int get_StyleBits();
		/**
		 * Used to hold extra styling information for each line.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property void set_LineState(int line, int state);
		/**
		 * Retrieve the extra styling information for a line.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property int get_LineState(int line);
		/**
		 * Retrieve the last line number that has line state.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property int get_MaxLineState();
		/**
		 * Is the background of the line containing the caret in a different colour?
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property bool get_CaretLineVisible();
		/**
		 * Display the background of the line containing the caret in a different colour.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property void set_CaretLineVisible(bool show);
		/**
		 * Set the number of spaces used for one level of indentation.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property void set_Indent(int indentSize);
		/**
		 * Retrieve indentation size.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property int get_Indent();
		/**
		 * Indentation will only use space characters if useTabs is false, otherwise
		 * it will use a combination of tabs and spaces.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property void set_UseTabs(bool useTabs);
		/**
		 * Retrieve whether tabs will be used in indentation.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property bool get_UseTabs();
		/**
		 * Show or hide the horizontal scroll bar.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property void set_HorizontalScrollBarVisible(bool show);
		/**
		 * Is the horizontal scroll bar visible?
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property bool get_HorizontalScrollBarVisible();
		/**
		 * Show or hide indentation guides.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property void set_IndentationGuidesVisible(bool show);
		/**
		 * Are the indentation guides visible?
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property bool get_IndentationGuidesVisible();
		/**
		 * Set the highlighted indentation guide column.
		 * 0 = no highlighted guide.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property void set_HighlightGuide(int column);
		/**
		 * Get the highlighted indentation guide column.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property int get_HighlightGuide();
		/**
		 * Get the code page used to interpret the bytes of the document as characters.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property int get_CodePage();
		/**
		 * In palette mode?
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property bool get_UsePalette();
		/**
		 * In read-only mode?
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property bool get_ReadOnly();
		/**
		 * Sets the position of the caret.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property void set_CurrentPos(long pos);
		/**
		 * Sets the position that starts the selection - this becomes the anchor.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property void set_SelectionStart(long pos);
		/**
		 * Returns the position at the start of the selection.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property long get_SelectionStart();
		/**
		 * Sets the position that ends the selection - this becomes the currentPosition.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property void set_SelectionEnd(long pos);
		/**
		 * Returns the position at the end of the selection.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property long get_SelectionEnd();
		/**
		 * Sets the print magnification added to the point size of each style for printing.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property void set_PrintMagnification(int magnification);
		/**
		 * Returns the print magnification.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property int get_PrintMagnification();
		/**
		 * Modify colours when printing for clearer printed text.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property void set_PrintColourMode(int mode);
		/**
		 * Returns the print colour mode.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property int get_PrintColourMode();
		/**
		 * Retrieve the display line at the top of the display.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property int get_FirstVisibleLine();
		/**
		 * Returns the number of lines in the document. There is always at least one.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property int get_LineCount();
		/**
		 * Sets the size in pixels of the left margin.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property void set_MarginLeft(int pixelWidth);
		/**
		 * Returns the size in pixels of the left margin.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property int get_MarginLeft();
		/**
		 * Sets the size in pixels of the right margin.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property void set_MarginRight(int pixelWidth);
		/**
		 * Returns the size in pixels of the right margin.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property int get_MarginRight();
		/**
		 * Is the document different from when it was last saved?
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property bool get_IsModified();
		/**
		 * Set to read only or read write.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property void set_ReadOnly(bool readOnly);
		/**
		 * Will a paste succeed?
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property bool get_CanPaste();
		/**
		 * Are there any undoable actions in the undo history?
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property bool get_CanUndo();
		/**
		 * Replace the contents of the document with the argument text.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property void set_Text(String* text);
		/**
		 * Retrieve the number of characters in the document.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property int get_TextLength();
		/**
		 * Set to overtype (true) or insert mode.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property void set_Overtype(bool overtype);
		/**
		 * Returns true if overtype mode is active otherwise false is returned.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property bool get_Overtype();
		/**
		 * Set the width of the insert mode caret.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property void set_CaretWidth(int pixelWidth);
		/**
		 * Returns the width of the insert mode caret.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property int get_CaretWidth();
		/**
		 * Sets the position that starts the target which is used for updating the
		 * document without affecting the scroll position.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property void set_TargetStart(long pos);
		/**
		 * Get the position that starts the target.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property long get_TargetStart();
		/**
		 * Sets the position that ends the target which is used for updating the
		 * document without affecting the scroll position.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property void set_TargetEnd(long pos);
		/**
		 * Get the position that ends the target.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property long get_TargetEnd();
		/**
		 * Set the search flags used by SearchInTarget.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property void set_SearchFlags(int flags);
		/**
		 * Get the search flags used by SearchInTarget.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property int get_SearchFlags();
		/**
		 * Sets whether a tab pressed when caret is within indentation indents.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property void set_TabIndents(bool tabIndents);
		/**
		 * Does a tab pressed when caret is within indentation indent?
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property bool get_TabIndents();
		/**
		 * Sets the time the mouse must sit still to generate a mouse dwell event.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property void set_MouseDwellTime(int periodMilliseconds);
		/**
		 * Retrieve the time the mouse must sit still to generate a mouse dwell event.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property int get_MouseDwellTime();
		/**
		 * Sets whether text is word wrapped.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property void set_WrapMode(int mode);
		/**
		 * Retrieve whether text is word wrapped.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property int get_WrapMode();
		/**
		 * Sets the degree of caching of layout information.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property void set_LayoutCache(int mode);
		/**
		 * Retrieve the degree of caching of layout information.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property int get_LayoutCache();
		/**
		 * Sets the document width assumed for scrolling.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property void set_ScrollWidth(int pixelWidth);
		/**
		 * Retrieve the document width assumed for scrolling.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property int get_ScrollWidth();
		/**
		 * Sets the scroll range so that maximum scroll position has
		 * the last line at the bottom of the view (default).
		 * Setting this to false allows scrolling one page below the last line.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property void set_EndAtLastLine(bool endAtLastLine);
		/**
		 * Retrieve whether the maximum scroll position has the last
		 * line at the bottom of the view.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property bool get_EndAtLastLine();
		/**
		 * Show or hide the vertical scroll bar.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property void set_VerticalScrollBarVisible(bool show);
		/**
		 * Is the vertical scroll bar visible?
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property bool get_VerticalScrollBarVisible();



		/**
		 * Get the time in milliseconds that the caret is on and off.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property int get_CaretPeriod();
		/**
		 * Get the time in milliseconds that the caret is on and off. 0 = steady on.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property void set_CaretPeriod(int periodMilliseconds);
		/**
		 * Set a margin to be either numeric or symbolic.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property void set_MarginTypeN(int margin, int marginType);
		/**
		 * Retrieve the type of a margin.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property int get_MarginTypeN(int margin);
		/**
		 * Set the width of a margin to a width expressed in pixels.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property void set_MarginWidthN(int margin, int pixelWidth);
		/**
		 * Retrieve the width of a margin in pixels.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property int get_MarginWidthN(int margin);
		/**
		 * Set a mask that determines which markers are displayed in a margin.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property void set_MarginMaskN(int margin, int mask);
		/**
		 * Retrieve the marker mask of a margin.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property int get_MarginMaskN(int margin);
		/**
		 * Make a margin sensitive or insensitive to mouse clicks.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property void set_MarginSensitiveN(int margin, bool sensitive);
		/**
		 * Retrieve the mouse click sensitivity of a margin.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property bool get_MarginSensitiveN(int margin);
		/**
		 * Change the visible size of a tab to be a multiple of the width of a space character.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property void set_TabWidth(int tabWidth);
		/**
		 * Retrieve the visible size of a tab.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property int get_TabWidth();
		/**
		 * Retrieve the position of the last correctly styled character.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property long get_EndStyled();
		/**
		 * Retrieve the current end of line mode - one of CRLF, CR, or LF.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property int get_EOLMode();
		/**
		 * Set the current end of line mode.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property void set_EOLMode(int eolMode);
		/**
		 * Choose between collecting actions into the undo
		 * history and discarding them.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property void set_UndoCollection(bool collectUndo);
		/**
		 * Is undo history being collected?
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property bool get_IsUndoCollected();
		/**
		 * Are white space characters currently visible?
		 * Returns one of SCWS_* constants.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property int get_WhitespaceVisibleState();
		/**
		 * Make white space characters invisible, always visible or visible outside indentation.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property void set_WhitespaceVisibleState(int viewWS);
		/**
		 * Are there any redoable actions in the undo history?
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property bool get_CanRedo();
		/**
		 * The number of characters in the document.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property int get_Length();
		/**
		 * Returns the character byte at the position.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property int get_CharAt(long pos);
		/**
		 * Returns the position of the caret.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property long get_CurrentPos();
		/**
		 * Returns the style byte at the position.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property int get_StyleAt(long pos);
		/**
		 * Get the colour of the background of the line containing the caret.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property long get_CaretLineBackground();
		/**
		 * Set the colour of the background of the line containing the caret.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property void set_CaretLineBackground(long back);
		/**
		 * Set the foreground colour of the caret.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property void set_CaretForeground(long fore);
		/**
		 * Get the foreground colour of the caret.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property long get_CaretForeground();
		/**
		 * Is drawing done first into a buffer or direct to the screen?
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property bool get_BufferedDraw();
		/**
		 * If drawing is buffered then each line of text is drawn into a bitmap buffer
		 * before drawing it to the screen to avoid flicker.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property void set_BufferedDraw(bool buffered);
		/**
		 * Sets whether a backspace pressed when caret is within indentation unindents.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property void set_BackSpaceUnIndents(bool bsUnIndents);
		/**
		 * Does a backspace pressed when caret is within indentation unindent?
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property bool get_BackSpaceUnIndents();
		/**
		 * Returns the position of the opposite end of the selection to the caret.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property long get_AnchorPosition();
		/**
		 * Set the selection anchor to a position. The anchor is the opposite
		 * end of the selection from the caret.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property void set_AnchorPosition(long posAnchor);
		/**
		 * Are the end of line characters visible?
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property bool get_EOLCharactersVisible();
		/**
		 * Make the end of line characters visible or invisible.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property void set_EOLCharactersVisible(bool visible);
		/**
		 * Change internal focus flag.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property void set_focus(bool focus);
		/**
		 * Get internal focus flag.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property bool get_focus();
		/**
		 * Is the selection rectangular? The alternative is the more common stream selection.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property bool get_IsSelectionRectangle();
		/**
		 * Retrieves the number of lines completely visible.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property int get_LinesOnScreen();

		/**
		 * Get which document modification events are sent to the container.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property int get_ModEventMask();
		/**
		 * Set which document modification events are sent to the container.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property void set_ModEventMask(int mask);
		/**
		 * Retrieve the column number which text should be kept within.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property int get_EdgeColumn();
		/**
		 * Set the column number of the edge.
		 * If text goes past the edge then it is highlighted.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property void set_EdgeColumn(int column);
		/**
		 * Retrieve the edge highlight mode.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property int get_EdgeMode();
		/**
		 * The edge may be displayed by a line (EDGE_LINE) or by highlighting text that
		 * goes beyond it (EDGE_BACKGROUND) or not displayed at all (EDGE_NONE).
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property void set_EdgeMode(int mode);
		/**
		 * Retrieve the colour used in edge indication.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property long get_EdgeColour();
		/**
		 * Change the colour used in edge indication.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property void set_EdgeColour(long edgeColour);
		/**
		 * Set whether a pop up menu is displayed automatically when the user presses
		 * the wrong mouse button.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property void set_UsePopUp(bool allowPopUp);
		/**
		 * Set the zoom level. This number of points is added to the size of all fonts.
		 * It may be positive to magnify or negative to reduce.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property void set_ZoomLevel(int zoom);
		/**
		 * Retrieve the zoom level.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property int get_ZoomLevel();
		/**
		 * Change error status - 0 = OK.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property void set_Status(int statusCode);
		/**
		 * Get error status.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property int get_Status();
		/**
		 * Set whether the mouse is captured when its button is pressed.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property void set_MouseDownCaptures(bool captures);
		/**
		 * Get whether mouse gets captured.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property bool get_MouseDownCaptures();
		/**
		 * Sets the cursor to one of the SC_CURSOR* values.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property void set_CursorType(int cursorType);
		/**
		 * Get cursor type.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property int get_CursorType();
		/**
		 * Change the way control characters are displayed:
		 * If symbol is < 32, keep the drawn way, else, use the given character.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property void set_ControlCharSymbol(int symbol);
		/**
		 * Get the way control characters are displayed.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property int get_ControlCharSymbol();
		/**
		 * Get and Set the xOffset (ie, horizonal scroll position).
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property void set_XOffset(int newOffset);
		/**
		 * Get and Set the xOffset (ie, horizonal scroll position).
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property int get_XOffset();
		/**
		 * Set printing to line wrapped (SC_WRAP_WORD) or not line wrapped (SC_WRAP_NONE).
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property void set_PrintWrapMode(int mode);
		/**
		 * Is printing line wrapped.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property int get_PrintWrapMode();
		/**
		 * Set the lexing language of the document.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property void set_Lexer(int lexer);
		/**
		 * Retrieve the lexing language of the document.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property int get_Lexer();
		/**
		 *  Set up a value that may be used by a lexer for some 
		 *  optional feature;
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property void set_Property(String* key, String* value);

		/**
		 * Set up the key words used by the lexer.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property void set_KeyWords(int keywordSet, String* keywords);


		/**
		 * The lexing language of the document based 
		 * on string name.
		 */
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property String* get_LexerLanguage();
		[ System::ComponentModel::BrowsableAttribute( false ) ]
		__property void set_LexerLanguage( String* lexerLanguage );
		
		///------------------------------------------------------------------------------------
		/// Private Data Members 
		///------------------------------------------------------------------------------------
		

		private: 
			/// The Lexer Language that has been set. 
			String* _lexerLanguage;

		inline long SPerform( long n1, long n2, long n3 )
		{
			if( m_Pointer )
				return Perform( m_Pointer, n1, n2 , n3 );
			return 0;
		}

	};
}
