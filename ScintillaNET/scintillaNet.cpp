// This is the main DLL file.

#include "stdafx.h"
#include "ScintillaNet.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "Scintilla.h"
#include "SciLexer.h"
#include "ConfigurationUtility.h"

void Platform_Initialise(void *hInstance);
typedef  System::Byte bytebuffer __gc[];
typedef Byte (__pin *pinnedbytepointer);

//! Scintilla direct message function.
typedef long(__cdecl* scmsgfn)(void *ptr, long Msg, WPARAM wParam, LPARAM lParam);

//! Function pointer to Scintilla window message pump.
scmsgfn	Perform;

/**
* SPerform uses the function pointer (Perform)
* to run scintilla commands on the relevant scintilla control.
*/
// #define SPerform( n1 , n2 , n3 ) if( m_Pointer ) Perform( m_Pointer,n1 , n2, n3)



namespace Scintilla
{
	class charstringbuf
	{
		public:
			char* buffer;

		charstringbuf( int bufsize )
		{
			buffer = new char[bufsize];
		}
		~charstringbuf( )
		{
			delete buffer;
		}
		String* toString()
		{
			return new String( buffer );
		}
	};

	void ScintillaControl::AddIgnoredKey(System::Windows::Forms::Keys key , System::Windows::Forms::Keys modifier )
	{
		ignoredKeys->Add( __box((int)key+(int)modifier),__box((int)key+(int)modifier) );
	}

	void ScintillaControl::AddIgnoredKey(System::Windows::Forms::Shortcut shortcutkey )
	{
		ignoredKeys->Add( __box((int)shortcutkey),__box((int)shortcutkey) );
	}

	void ScintillaControl::addShortcuts( Menu* m )
	{
		for( int i = 0; i< (m->MenuItems)->Count ; i++ )
		{
			MenuItem* mi = m->MenuItems->get_Item(i);

			if( mi->Shortcut != Shortcut::None )
				AddIgnoredKey(mi->Shortcut );
			if( mi->MenuItems->Count > 0 )
				addShortcuts( mi );
		}
	}

	void ScintillaControl::AddShortcutsFromForm(System::Windows::Forms::Form* parentForm)
	{
		if( parentForm && parentForm->Menu )
			addShortcuts(parentForm->Menu );
	}

	bool ScintillaControl::PreProcessMessage(Message* m)
	{
		switch( m->Msg  )
		{
			case WM_KEYDOWN:
				{
					if( ignoredKeys->ContainsKey( __box((int)Control::ModifierKeys+(int)m->WParam) ) )
						return Parent->PreProcessMessage( m );
				}
			break;
		}
		return false;
	}

	void ScintillaControl::WndProc( Message* m )
	{
		SCNotification* scn =  reinterpret_cast<SCNotification *>((void*)m->LParam);
		if ( m->Msg == WM_NOTIFY  && (scn->nmhdr.hwndFrom== hwndScintilla )  )
		{
			//(((LPNMHDR)(void*)m->LParam)
			
			// message from scintilla.
			switch( scn->nmhdr.code)
			{
				case SCN_STYLENEEDED:
					StyleNeeded( this , scn->position );
				break;

				case SCN_CHARADDED:
					CharAdded( this , scn->ch );
				break;

				case SCN_SAVEPOINTREACHED:
					SavePointReached( this );
				break;

				case SCN_SAVEPOINTLEFT:
					SavePointLeft( this );
				break;

				case SCN_MODIFYATTEMPTRO:
					ModifyAttemptRO( this );
				break;

				case SCN_KEY:
					Key( this , scn->ch , scn->modifiers, scn->modificationType  );
				break;

				case SCN_DOUBLECLICK:
					DoubleClick( this );
				break;

				case SCN_UPDATEUI:
					UpdateUI( this );
				break;

				case SCN_MODIFIED:
					if( scn->modificationType && SC_MOD_INSERTTEXT )
						TextInserted( this ,scn->position ,scn->length , scn->linesAdded );
					if( scn->modificationType && SC_MOD_DELETETEXT )
						TextDeleted( this ,scn->position ,scn->length , scn->linesAdded  );
					if( scn->modificationType && SC_MOD_CHANGESTYLE )
						StyleChanged( this ,scn->position ,scn->length );
					if( scn->modificationType && SC_MOD_CHANGEFOLD )
						FoldChanged( this , scn->line , scn->foldLevelNow , scn->foldLevelPrev );
					if( scn->modificationType && SC_PERFORMED_USER )
						UserPerformed( this );
					if( scn->modificationType && SC_PERFORMED_UNDO )
						UndoPerformed( this );
					if( scn->modificationType && SC_PERFORMED_REDO )
						RedoPerformed( this);
					if( scn->modificationType && SC_LASTSTEPINUNDOREDO )
					    LastStepInUndoRedo( this );
					if( scn->modificationType && SC_MOD_CHANGEMARKER )
						MarkerChanged( this , scn->line );
					if( scn->modificationType && SC_MOD_BEFOREINSERT )
						BeforeInsert( this , scn->position , scn->length  );
					if( scn->modificationType && SC_MOD_BEFOREDELETE )
						BeforeDelete( this , scn->position , scn->length );

					Modified( this );
				break;

				case SCN_MACRORECORD:
					MacroRecord( this );
				break;

				case SCN_MARGINCLICK:
					MarginClick( this );
				break;

				case SCN_NEEDSHOWN:
					NeedShown( this );
				break;

				case SCN_PAINTED:
					Painted( this );
				break;

				case SCN_USERLISTSELECTION:
					UserListSelection( this , scn->listType  );
				break;

				case SCN_URIDROPPED:
					UriDropped( this );
				break;

				case SCN_DWELLSTART:
					DwellStart( this, scn->position , scn->x , scn->y );
				break;

				case SCN_DWELLEND:
					DwellEnd( this , scn->position , scn->x , scn->y );
				break;

				case SCN_ZOOM:
					Zoom( this );
				break;

				case SCN_HOTSPOTCLICK:
				break;

				case SCN_HOTSPOTDOUBLECLICK:
				break;

				case SCN_CALLTIPCLICK:
				break;


			}

		}
		else
			Control::WndProc( m );			
	}

	ScintillaControl::ScintillaControl()
	{
		try
		{
		ignoredKeys = new System::Collections::Hashtable();

		Platform_Initialise( 0 );
		
		hwndScintilla = (HWND)(void*)CreateWindowEx(0, "Scintilla","", WS_CHILD|WS_VISIBLE|WS_TABSTOP, 0,0,this->Width,this->Height,(HWND)this->Handle.ToPointer() ,0, 0,0);
			
		_scintillaWin = (ScintillaWin*)(void *)SendMessage((HWND)(void*)hwndScintilla,SCI_GETDIRECTPOINTER,0,0);

		m_Pointer = (void *) ::SendMessage(hwndScintilla, SCI_GETDIRECTPOINTER, 0, 0);
		
		if( Perform == NULL )
			Perform = (scmsgfn) ::SendMessage(hwndScintilla, SCI_GETDIRECTFUNCTION, 0, 0);

		this->add_Resize( new EventHandler(this, doResize ) );
		}
		catch( ... )
		{
		}
	}

	void ScintillaControl::doResize(Object* sender, EventArgs* e)
	{
		::SetWindowPos(hwndScintilla, 0, 0, 0, this->Width , this->Height , 0);
	}

	String* ScintillaControl::get_Line(int line)
	{
		charstringbuf buf( LineLength(line) );
		SPerform(SCI_GETLINE, (long)line, (long)buf.buffer);
		return buf.toString();
	}

	String* ScintillaControl::get_SelectedText()
	{
		charstringbuf buf(SelectionEnd - SelectionStart +1);
		SPerform(SCI_GETSELTEXT, 0, (long)buf.buffer);
		return buf.toString();
	}

	String* ScintillaControl::get_Text()
	{
		int i = Length+1;
		charstringbuf buf(i);
		SPerform(SCI_GETTEXT, i, (long)buf.buffer);
		return buf.toString();
	}

	String* ScintillaControl::get_CurrentLine()
	{
		int i = LineLength(LineFromPosition( CurrentPos ))+1;
		charstringbuf buf(i);
		SPerform( SCI_GETCURLINE , i , (long)buf.buffer );
		return buf.toString();
	}

	void ScintillaControl::AddText(int length, String* text)
    {
		pinnedbytepointer ptr = &(System::Text::Encoding::ASCII->GetBytes( text )[0]);
		SPerform(SCI_ADDTEXT, (long)length, (long)ptr);
    }

    void ScintillaControl::AddStyledText(int length, String* text)
    {
		pinnedbytepointer ptr = &(System::Text::Encoding::ASCII->GetBytes( text )[0]);
    	SPerform(SCI_ADDSTYLEDTEXT, (long)length, (long)ptr);
    }

    void ScintillaControl::InsertText(long pos, String* text)
    {
		pinnedbytepointer ptr = &(System::Text::Encoding::ASCII->GetBytes( text )[0]);
    	SPerform(SCI_INSERTTEXT, pos, (long)ptr);
    }

    void ScintillaControl::ClearAll()
    {
    	SPerform(SCI_CLEARALL, 0, 0);
    }

    void ScintillaControl::ClearDocumentStyle()
    {
    	SPerform(SCI_CLEARDOCUMENTSTYLE, 0, 0);
    }

    void ScintillaControl::Redo()
    {
    	SPerform(SCI_REDO, 0, 0);
    }

    void ScintillaControl::SelectAll()
    {
    	SPerform(SCI_SELECTALL, 0, 0);
    }

    void ScintillaControl::SetSavePoint()
    {
    	SPerform(SCI_SETSAVEPOINT, 0, 0);
    }

    int ScintillaControl::GetStyledText(TextRange* tr)
    {
    	return (int)SPerform(SCI_GETSTYLEDTEXT, 0, (long)tr);
    }

    int ScintillaControl::MarkerLineFromHandle(int handle)
    {
    	return (int)SPerform(SCI_MARKERLINEFROMHANDLE, (long)handle, 0);
    }

    void ScintillaControl::MarkerDeleteHandle(int handle)
    {
    	SPerform(SCI_MARKERDELETEHANDLE, (long)handle, 0);
    }

    int ScintillaControl::PositionFromPoint(int x, int y)
    {
    	return (int)SPerform(SCI_POSITIONFROMPOINT, (long)x, (long)y);
    }

    int ScintillaControl::PositionFromPointClose(int x, int y)
    {
    	return (int)SPerform(SCI_POSITIONFROMPOINTCLOSE, (long)x, (long)y);
    }

    void ScintillaControl::GotoLine(int line)
    {
    	SPerform(SCI_GOTOLINE, (long)line, 0);
    }

    void ScintillaControl::GotoPos(long pos)
    {
    	SPerform(SCI_GOTOPOS, pos, 0);
    }

    void ScintillaControl::ConvertEOLs(int eolMode)
    {
    	SPerform(SCI_CONVERTEOLS, (long)eolMode, 0);
    }


    void ScintillaControl::StartStyling(long pos, int mask)
    {
    	SPerform(SCI_STARTSTYLING, pos, (long)mask);
    }

    void ScintillaControl::SetStyling(int length, int style)
    {
    	SPerform(SCI_SETSTYLING, (long)length, (long)style);
    }

    void ScintillaControl::set_CodePage(int codePage)
    {
    	SPerform(SCI_SETCODEPAGE, (long)codePage, 0);
    }

    void ScintillaControl::set_UsePalette(bool usePalette)
    {
    	SPerform(SCI_SETUSEPALETTE, (long)usePalette, 0);
    }

    void ScintillaControl::MarkerDefine(int markerNumber, int markerSymbol)
    {
    	SPerform(SCI_MARKERDEFINE, (long)markerNumber, (long)markerSymbol);
    }

    void ScintillaControl::MarkerSetFore(int markerNumber, long fore)
    {
    	SPerform(SCI_MARKERSETFORE, (long)markerNumber, fore);
    }

    void ScintillaControl::MarkerSetBack(int markerNumber, long back)
    {
    	SPerform(SCI_MARKERSETBACK, (long)markerNumber, back);
    }

    int ScintillaControl::MarkerAdd(int line, int markerNumber)
    {
    	return (int)SPerform(SCI_MARKERADD, (long)line, (long)markerNumber);
    }

    void ScintillaControl::MarkerDelete(int line, int markerNumber)
    {
    	SPerform(SCI_MARKERDELETE, (long)line, (long)markerNumber);
    }

    void ScintillaControl::MarkerDeleteAll(int markerNumber)
    {
    	SPerform(SCI_MARKERDELETEALL, (long)markerNumber, 0);
    }

    int ScintillaControl::MarkerGet(int line)
    {
    	return (int)SPerform(SCI_MARKERGET, (long)line, 0);
    }

    int ScintillaControl::MarkerNext(int lineStart, int markerMask)
    {
    	return (int)SPerform(SCI_MARKERNEXT, (long)lineStart, (long)markerMask);
    }

    int ScintillaControl::MarkerPrevious(int lineStart, int markerMask)
    {
    	return (int)SPerform(SCI_MARKERPREVIOUS, (long)lineStart, (long)markerMask);
    }

    void ScintillaControl::MarkerDefinePixmap(int markerNumber, String* pixmap)
    {
		pinnedbytepointer ptr = &(System::Text::Encoding::ASCII->GetBytes( pixmap )[0]);
		SPerform(SCI_MARKERDEFINEPIXMAP, (long)markerNumber, (long)ptr);
    }

    void ScintillaControl::StyleClearAll()
    {
    	SPerform(SCI_STYLECLEARALL, 0, 0);
    }

    void ScintillaControl::StyleSetFore(int style, long fore)
    {
    	SPerform(SCI_STYLESETFORE, (long)style, fore);
    }

    void ScintillaControl::StyleSetBack(int style, long back)
    {
    	SPerform(SCI_STYLESETBACK, (long)style, back);
    }

    void ScintillaControl::StyleSetBold(int style, bool bold)
    {
    	SPerform(SCI_STYLESETBOLD, (long)style, (long)bold);
    }

    void ScintillaControl::StyleSetItalic(int style, bool italic)
    {
    	SPerform(SCI_STYLESETITALIC, (long)style, (long)italic);
    }

    void ScintillaControl::StyleSetSize(int style, int sizePoints)
    {
    	SPerform(SCI_STYLESETSIZE, (long)style, (long)sizePoints);
    }

    void ScintillaControl::StyleSetFont(int style, String* fontName)
    {
		pinnedbytepointer ptr = &(System::Text::Encoding::ASCII->GetBytes( fontName )[0]);
    	SPerform(SCI_STYLESETFONT, (long)style, (long)ptr);
    }

    void ScintillaControl::StyleSetEOLFilled(int style, bool filled)
    {
    	SPerform(SCI_STYLESETEOLFILLED, (long)style, (long)filled);
    }

    void ScintillaControl::StyleResetDefault()
    {
    	SPerform(SCI_STYLERESETDEFAULT, 0, 0);
    }

    void ScintillaControl::StyleSetUnderline(int style, bool underline)
    {
    	SPerform(SCI_STYLESETUNDERLINE, (long)style, (long)underline);
    }

    void ScintillaControl::StyleSetCase(int style, int caseForce)
    {
    	SPerform(SCI_STYLESETCASE, (long)style, (long)caseForce);
    }

    void ScintillaControl::StyleSetCharacterSet(int style, int characterSet)
    {
    	SPerform(SCI_STYLESETCHARACTERSET, (long)style, (long)characterSet);
    }

    void ScintillaControl::SetSelFore(bool useSetting, long fore)
    {
    	SPerform(SCI_SETSELFORE, (long)useSetting, fore);
    }

    void ScintillaControl::SetSelBack(bool useSetting, long back)
    {
    	SPerform(SCI_SETSELBACK, (long)useSetting, back);
    }

    void ScintillaControl::AssignCmdKey(long km, int msg)
    {
    	SPerform(SCI_ASSIGNCMDKEY, km, (long)msg);
    }

    void ScintillaControl::ClearCmdKey(long km)
    {
    	SPerform(SCI_CLEARCMDKEY, km, 0);
    }

    void ScintillaControl::ClearAllCmdKeys()
    {
    	SPerform(SCI_CLEARALLCMDKEYS, 0, 0);
    }

    void ScintillaControl::SetStylingEx(int length, String* styles)
    {
		pinnedbytepointer ptr = &(System::Text::Encoding::ASCII->GetBytes( styles )[0]);
    	SPerform(SCI_SETSTYLINGEX, (long)length, (long)ptr);
    }

    void ScintillaControl::StyleSetVisible(int style, bool visible)
    {
    	SPerform(SCI_STYLESETVISIBLE, (long)style, (long)visible);
    }
	
	void ScintillaControl::StyleSetHotspot(int style, bool hotspot)
	{
		SPerform(SCI_STYLESETHOTSPOT , (long)style , (long)hotspot );
	}
	
    void ScintillaControl::set_WordChars(String* characters)
    {
		pinnedbytepointer ptr = &(System::Text::Encoding::ASCII->GetBytes( characters )[0]);
    	SPerform(SCI_SETWORDCHARS, 0, (long)ptr);
    }

    void ScintillaControl::BeginUndoAction()
    {
    	SPerform(SCI_BEGINUNDOACTION, 0, 0);
    }

    void ScintillaControl::EndUndoAction()
    {
    	SPerform(SCI_ENDUNDOACTION, 0, 0);
    }

    void ScintillaControl::IndicSetStyle(int indic, int style)
    {
    	SPerform(SCI_INDICSETSTYLE, (long)indic, (long)style);
    }

    int ScintillaControl::IndicGetStyle(int indic)
    {
    	return (int)SPerform(SCI_INDICGETSTYLE, (long)indic, 0);
    }

    void ScintillaControl::IndicSetFore(int indic, long fore)
    {
    	SPerform(SCI_INDICSETFORE, (long)indic, fore);
    }

    long ScintillaControl::IndicGetFore(int indic)
    {
    	return SPerform(SCI_INDICGETFORE, (long)indic, 0);
    }

    void ScintillaControl::SetWhitespaceFore(bool useSetting, long fore)
    {
    	SPerform(SCI_SETWHITESPACEFORE, (long)useSetting, fore);
    }

    void ScintillaControl::SetWhitespaceBack(bool useSetting, long back)
    {
    	SPerform(SCI_SETWHITESPACEBACK, (long)useSetting, back);
    }

    void ScintillaControl::set_StyleBits(int bits)
    {
    	SPerform(SCI_SETSTYLEBITS, (long)bits, 0);
    }

    int ScintillaControl::get_StyleBits()
    {
    	return (int)SPerform(SCI_GETSTYLEBITS, 0, 0);
    }

    void ScintillaControl::set_LineState(int line, int state)
    {
    	SPerform(SCI_SETLINESTATE, (long)line, (long)state);
    }

    int ScintillaControl::get_LineState(int line)
    {
    	return (int)SPerform(SCI_GETLINESTATE, (long)line, 0);
    }

    int ScintillaControl::get_MaxLineState()
    {
    	return (int)SPerform(SCI_GETMAXLINESTATE, 0, 0);
    }

    bool ScintillaControl::get_CaretLineVisible()
    {
    	return SPerform(SCI_GETCARETLINEVISIBLE, 0, 0) != 0;
    }

    void ScintillaControl::set_CaretLineVisible(bool show)
    {
    	SPerform(SCI_SETCARETLINEVISIBLE, (long)show, 0);
    }

    void ScintillaControl::StyleSetChangeable(int style, bool changeable)
    {
    	SPerform(SCI_STYLESETCHANGEABLE, (long)style, (long)changeable);
    }

    void ScintillaControl::AutoCShow(int lenEntered, String* itemList)
    {
		pinnedbytepointer ptr = &(System::Text::Encoding::ASCII->GetBytes(itemList )[0]);
    	SPerform(SCI_AUTOCSHOW, (long)lenEntered, (long)ptr);
    }

    void ScintillaControl::AutoCCancel()
    {
    	SPerform(SCI_AUTOCCANCEL, 0, 0);
    }

    bool ScintillaControl::AutoCActive()
    {
    	return SPerform(SCI_AUTOCACTIVE, 0, 0) != 0;
    }

    long ScintillaControl::AutoCPosStart()
    {
    	return SPerform(SCI_AUTOCPOSSTART, 0, 0);
    }

    void ScintillaControl::AutoCComplete()
    {
    	SPerform(SCI_AUTOCCOMPLETE, 0, 0);
    }

    void ScintillaControl::AutoCStops(String* characterSet)
    {
		pinnedbytepointer ptr = &(System::Text::Encoding::ASCII->GetBytes( characterSet)[0]);
    	SPerform(SCI_AUTOCSTOPS, 0, (long)ptr);
    }

    void ScintillaControl::AutoCSetSeparator(int separatorCharacter)
    {
    	SPerform(SCI_AUTOCSETSEPARATOR, (long)separatorCharacter, 0);
    }

    int ScintillaControl::AutoCGetSeparator()
    {
    	return (int)SPerform(SCI_AUTOCGETSEPARATOR, 0, 0);
    }

    void ScintillaControl::AutoCSelect(String* text)
    {
		pinnedbytepointer ptr = &(System::Text::Encoding::ASCII->GetBytes( text )[0]);
    	SPerform(SCI_AUTOCSELECT, 0, (long)ptr);
    }

    void ScintillaControl::AutoCSetCancelAtStart(bool cancel)
    {
    	SPerform(SCI_AUTOCSETCANCELATSTART, (long)cancel, 0);
    }

    bool ScintillaControl::AutoCGetCancelAtStart()
    {
    	return SPerform(SCI_AUTOCGETCANCELATSTART, 0, 0) != 0;
    }

    void ScintillaControl::AutoCSetFillUps(String* characterSet)
    {
		pinnedbytepointer ptr = &(System::Text::Encoding::ASCII->GetBytes( characterSet)[0]);
    	SPerform(SCI_AUTOCSETFILLUPS, 0, (long)ptr);
    }

    void ScintillaControl::AutoCSetChooseSingle(bool chooseSingle)
    {
    	SPerform(SCI_AUTOCSETCHOOSESINGLE, (long)chooseSingle, 0);
    }

    bool ScintillaControl::AutoCGetChooseSingle()
    {
    	return SPerform(SCI_AUTOCGETCHOOSESINGLE, 0, 0) != 0;
    }

    void ScintillaControl::AutoCSetIgnoreCase(bool ignoreCase)
    {
    	SPerform(SCI_AUTOCSETIGNORECASE, (long)ignoreCase, 0);
    }

    bool ScintillaControl::AutoCGetIgnoreCase()
    {
    	return SPerform(SCI_AUTOCGETIGNORECASE, 0, 0) != 0;
    }

    void ScintillaControl::UserListShow(int listType, String* itemList)
    {
		pinnedbytepointer ptr = &(System::Text::Encoding::ASCII->GetBytes( itemList )[0]);
    	SPerform(SCI_USERLISTSHOW, (long)listType, (long)ptr);
    }

    void ScintillaControl::AutoCSetAutoHide(bool autoHide)
    {
    	SPerform(SCI_AUTOCSETAUTOHIDE, (long)autoHide, 0);
    }

    bool ScintillaControl::AutoCGetAutoHide()
    {
    	return SPerform(SCI_AUTOCGETAUTOHIDE, 0, 0) != 0;
    }

    void ScintillaControl::AutoCSetDropRestOfWord(bool dropRestOfWord)
    {
    	SPerform(SCI_AUTOCSETDROPRESTOFWORD, (long)dropRestOfWord, 0);
    }

    bool ScintillaControl::AutoCGetDropRestOfWord()
    {
    	return SPerform(SCI_AUTOCGETDROPRESTOFWORD, 0, 0) != 0;
    }

    void ScintillaControl::RegisterImage(int type, String* xpmData)
    {
		pinnedbytepointer ptr = &(System::Text::Encoding::ASCII->GetBytes( xpmData )[0]);
		SPerform(SCI_REGISTERIMAGE, (long)type, (long)ptr);
    }

    void ScintillaControl::ClearRegisteredImages()
    {
    	SPerform(SCI_CLEARREGISTEREDIMAGES, 0, 0);
    }

    void ScintillaControl::set_Indent(int indentSize)
    {
    	SPerform(SCI_SETINDENT, (long)indentSize, 0);
    }

    int ScintillaControl::get_Indent()
    {
    	return (int)SPerform(SCI_GETINDENT, 0, 0);
    }

    void ScintillaControl::set_UseTabs(bool useTabs)
    {
    	SPerform(SCI_SETUSETABS, (long)useTabs, 0);
    }

    bool ScintillaControl::get_UseTabs()
    {
    	return SPerform(SCI_GETUSETABS, 0, 0) != 0;
    }

    void ScintillaControl::SetLineIndentation(int line, int indentSize)
    {
    	SPerform(SCI_SETLINEINDENTATION, (long)line, (long)indentSize);
    }

    int ScintillaControl::GetLineIndentation(int line)
    {
    	return (int)SPerform(SCI_GETLINEINDENTATION, (long)line, 0);
    }

    long ScintillaControl::GetLineIndentPosition(int line)
    {
    	return SPerform(SCI_GETLINEINDENTPOSITION, (long)line, 0);
    }

    int ScintillaControl::GetColumn(long pos)
    {
    	return (int)SPerform(SCI_GETCOLUMN, pos, 0);
    }

    void ScintillaControl::set_HorizontalScrollBarVisible(bool show)
    {
    	SPerform(SCI_SETHSCROLLBAR, (long)show, 0);
    }

    bool ScintillaControl::get_HorizontalScrollBarVisible()
    {
    	return SPerform(SCI_GETHSCROLLBAR, 0, 0) != 0;
    }

    void ScintillaControl::set_IndentationGuidesVisible(bool show)
    {
    	SPerform(SCI_SETINDENTATIONGUIDES, (long)show, 0);
    }

    bool ScintillaControl::get_IndentationGuidesVisible()
    {
    	return SPerform(SCI_GETINDENTATIONGUIDES, 0, 0) != 0;
    }

    void ScintillaControl::set_HighlightGuide(int column)
    {
    	SPerform(SCI_SETHIGHLIGHTGUIDE, (long)column, 0);
    }

    int ScintillaControl::get_HighlightGuide()
    {
    	return (int)SPerform(SCI_GETHIGHLIGHTGUIDE, 0, 0);
    }

    int ScintillaControl::GetLineEndPosition(int line)
    {
    	return (int)SPerform(SCI_GETLINEENDPOSITION, (long)line, 0);
    }

    int ScintillaControl::get_CodePage()
    {
    	return (int)SPerform(SCI_GETCODEPAGE, 0, 0);
    }

    bool ScintillaControl::get_UsePalette()
    {
    	return SPerform(SCI_GETUSEPALETTE, 0, 0) != 0;
    }

    bool ScintillaControl::get_ReadOnly()
    {
    	return SPerform(SCI_GETREADONLY, 0, 0) != 0;
    }

    void ScintillaControl::set_CurrentPos(long pos)
    {
    	SPerform(SCI_SETCURRENTPOS, pos, 0);
    }

    void ScintillaControl::set_SelectionStart(long pos)
    {
    	SPerform(SCI_SETSELECTIONSTART, pos, 0);
    }

    long ScintillaControl::get_SelectionStart()
    {
    	return SPerform(SCI_GETSELECTIONSTART, 0, 0);
    }

    void ScintillaControl::set_SelectionEnd(long pos)
    {
    	SPerform(SCI_SETSELECTIONEND, pos, 0);
    }

    long ScintillaControl::get_SelectionEnd()
    {
    	return SPerform(SCI_GETSELECTIONEND, 0, 0);
    }

    void ScintillaControl::set_PrintMagnification(int magnification)
    {
    	SPerform(SCI_SETPRINTMAGNIFICATION, (long)magnification, 0);
    }

    int ScintillaControl::get_PrintMagnification()
    {
    	return (int)SPerform(SCI_GETPRINTMAGNIFICATION, 0, 0);
    }

    void ScintillaControl::set_PrintColourMode(int mode)
    {
    	SPerform(SCI_SETPRINTCOLOURMODE, (long)mode, 0);
    }

    int ScintillaControl::get_PrintColourMode()
    {
    	return (int)SPerform(SCI_GETPRINTCOLOURMODE, 0, 0);
    }

    long ScintillaControl::FindText(int flags, TextToFind* ft)
    {
    	return SPerform(SCI_FINDTEXT, (long)flags, (long)ft);
    }

    void ScintillaControl::FormatRange(bool draw, long fr)
    {
    	SPerform(SCI_FORMATRANGE, (long)draw, fr);
    }

    int ScintillaControl::get_FirstVisibleLine()
    {
    	return (int)SPerform(SCI_GETFIRSTVISIBLELINE, 0, 0);
    }

    String* ScintillaControl::GetLine(int line)
    {
		charstringbuf buf( LineLength(line) );
		SPerform(SCI_GETLINE, (long)line, (long)buf.buffer);
		return buf.toString();
    }

    int ScintillaControl::get_LineCount()
    {
    	return (int)SPerform(SCI_GETLINECOUNT, 0, 0);
    }

    void ScintillaControl::set_MarginLeft(int pixelWidth)
    {
    	SPerform(SCI_SETMARGINLEFT, 0, (long)pixelWidth);
    }

    int ScintillaControl::get_MarginLeft()
    {
    	return (int)SPerform(SCI_GETMARGINLEFT, 0, 0);
    }

    void ScintillaControl::set_MarginRight(int pixelWidth)
    {
    	SPerform(SCI_SETMARGINRIGHT, 0, (long)pixelWidth);
    }

    int ScintillaControl::get_MarginRight()
    {
    	return (int)SPerform(SCI_GETMARGINRIGHT, 0, 0);
    }

    bool ScintillaControl::get_IsModified()
    {
    	return SPerform(SCI_GETMODIFY, 0, 0) != 0;
    }

    void ScintillaControl::SetSel(long start, long end)
    {
    	SPerform(SCI_SETSEL, start, end);
    }

    String* ScintillaControl::GetSelText()
    {
		charstringbuf buf(SelectionEnd - SelectionStart +1);
		SPerform(SCI_GETSELTEXT, 0, (long)buf.buffer);
		return buf.toString();
    }

    int ScintillaControl::GetTextRange(TextRange* tr)
    {
    	return (int)SPerform(SCI_GETTEXTRANGE, 0, (long)tr);
    }

    void ScintillaControl::HideSelection(bool normal)
    {
    	SPerform(SCI_HIDESELECTION, (long)normal, 0);
    }

    int ScintillaControl::PointXFromPosition(long pos)
    {
    	return (int)SPerform(SCI_POINTXFROMPOSITION, 0, pos);
    }

    int ScintillaControl::PointYFromPosition(long pos)
    {
    	return (int)SPerform(SCI_POINTYFROMPOSITION, 0, pos);
    }

    int ScintillaControl::LineFromPosition(long pos)
    {
    	return (int)SPerform(SCI_LINEFROMPOSITION, pos, 0);
    }

    int ScintillaControl::PositionFromLine(int line)
    {
    	return (int)SPerform(SCI_POSITIONFROMLINE, (long)line, 0);
    }

    void ScintillaControl::LineScroll(int columns, int lines)
    {
    	SPerform(SCI_LINESCROLL, (long)columns, (long)lines);
    }

    void ScintillaControl::ScrollCaret()
    {
    	SPerform(SCI_SCROLLCARET, 0, 0);
    }

    void ScintillaControl::ReplaceSel(String* text)
    {
		pinnedbytepointer ptr = &(System::Text::Encoding::ASCII->GetBytes( text )[0]);
    	SPerform(SCI_REPLACESEL, 0, (long)ptr);
    }

    void ScintillaControl::set_ReadOnly(bool readOnly)
    {
    	SPerform(SCI_SETREADONLY, (long)readOnly, 0);
    }

    void ScintillaControl::Null()
    {
    	SPerform(SCI_NULL, 0, 0);
    }

    bool ScintillaControl::get_CanPaste()
    {
    	return SPerform(SCI_CANPASTE, 0, 0) != 0;
    }

    bool ScintillaControl::get_CanUndo()
    {
    	return SPerform(SCI_CANUNDO, 0, 0) != 0;
    }

    void ScintillaControl::EmptyUndoBuffer()
    {
    	SPerform(SCI_EMPTYUNDOBUFFER, 0, 0);
    }

    void ScintillaControl::Undo()
    {
    	SPerform(SCI_UNDO, 0, 0);
    }

    void ScintillaControl::Cut()
    {
    	SPerform(SCI_CUT, 0, 0);
    }

    void ScintillaControl::Copy()
    {
    	SPerform(SCI_COPY, 0, 0);
    }

    void ScintillaControl::Paste()
    {
    	SPerform(SCI_PASTE, 0, 0);
    }

    void ScintillaControl::Clear()
    {
    	SPerform(SCI_CLEAR, 0, 0);
    }

    void ScintillaControl::set_Text(String* text)
    {
		pinnedbytepointer ptr = &(System::Text::Encoding::ASCII->GetBytes( text )[0]);
    	SPerform(SCI_SETTEXT, 0, (long)ptr);
    }

    int ScintillaControl::get_TextLength()
    {
    	return (int)SPerform(SCI_GETTEXTLENGTH, 0, 0);
    }

    int ScintillaControl::GetDirectFunction()
    {
    	return (int)SPerform(SCI_GETDIRECTFUNCTION, 0, 0);
    }

    int ScintillaControl::GetDirectPointer()
    {
    	return (int)SPerform(SCI_GETDIRECTPOINTER, 0, 0);
    }

    void ScintillaControl::set_Overtype(bool overtype)
    {
    	SPerform(SCI_SETOVERTYPE, (long)overtype, 0);
    }

    bool ScintillaControl::get_Overtype()
    {
    	return SPerform(SCI_GETOVERTYPE, 0, 0) != 0;
    }

    void ScintillaControl::set_CaretWidth(int pixelWidth)
    {
    	SPerform(SCI_SETCARETWIDTH, (long)pixelWidth, 0);
    }

    int ScintillaControl::get_CaretWidth()
    {
    	return (int)SPerform(SCI_GETCARETWIDTH, 0, 0);
    }

    void ScintillaControl::set_TargetStart(long pos)
    {
    	SPerform(SCI_SETTARGETSTART, pos, 0);
    }

    long ScintillaControl::get_TargetStart()
    {
    	return SPerform(SCI_GETTARGETSTART, 0, 0);
    }

    void ScintillaControl::set_TargetEnd(long pos)
    {
    	SPerform(SCI_SETTARGETEND, pos, 0);
    }

    long ScintillaControl::get_TargetEnd()
    {
    	return SPerform(SCI_GETTARGETEND, 0, 0);
    }

    int ScintillaControl::ReplaceTarget(int length, String* text)
    {
		pinnedbytepointer ptr = &(System::Text::Encoding::ASCII->GetBytes( text )[0]);
    	return (int)SPerform(SCI_REPLACETARGET, (long)length, (long)ptr);
    }

    int ScintillaControl::ReplaceTargetRE(int length, String* text)
    {
		pinnedbytepointer ptr = &(System::Text::Encoding::ASCII->GetBytes( text )[0]);
    	return (int)SPerform(SCI_REPLACETARGETRE, (long)length, (long)ptr);
    }

    int ScintillaControl::SearchInTarget(int length, String* text)
    {
		pinnedbytepointer ptr = &(System::Text::Encoding::ASCII->GetBytes( text )[0]);
    	return (int)SPerform(SCI_SEARCHINTARGET, (long)length, (long)ptr);
    }

    void ScintillaControl::set_SearchFlags(int flags)
    {
    	SPerform(SCI_SETSEARCHFLAGS, (long)flags, 0);
    }

    int ScintillaControl::get_SearchFlags()
    {
    	return (int)SPerform(SCI_GETSEARCHFLAGS, 0, 0);
    }

    void ScintillaControl::CallTipShow(long pos, String* definition)
    {
		pinnedbytepointer ptr = &(System::Text::Encoding::ASCII->GetBytes( definition)[0]);
    	SPerform(SCI_CALLTIPSHOW, pos, (long)ptr);
    }

    void ScintillaControl::CallTipCancel()
    {
    	SPerform(SCI_CALLTIPCANCEL, 0, 0);
    }

    bool ScintillaControl::CallTipActive()
    {
    	return SPerform(SCI_CALLTIPACTIVE, 0, 0) != 0;
    }

    long ScintillaControl::CallTipPosStart()
    {
    	return SPerform(SCI_CALLTIPPOSSTART, 0, 0);
    }

    void ScintillaControl::CallTipSetHlt(int start, int end)
    {
    	SPerform(SCI_CALLTIPSETHLT, (long)start, (long)end);
    }

    void ScintillaControl::CallTipSetBack(long back)
    {
    	SPerform(SCI_CALLTIPSETBACK, back, 0);
    }

	void ScintillaControl::CallTipSetFore(long fore)
	{
    	SPerform(SCI_CALLTIPSETFORE , fore , 0);
	}
	void ScintillaControl::CallTipSetForeHlt(long fore)
	{
    	SPerform(SCI_CALLTIPSETFOREHLT , fore , 0);
	}

    int ScintillaControl::VisibleFromDocLine(int line)
    {
    	return (int)SPerform(SCI_VISIBLEFROMDOCLINE, (long)line, 0);
    }

    int ScintillaControl::DocLineFromVisible(int lineDisplay)
    {
    	return (int)SPerform(SCI_DOCLINEFROMVISIBLE, (long)lineDisplay, 0);
    }

    void ScintillaControl::SetFoldLevel(int line, int level)
    {
    	SPerform(SCI_SETFOLDLEVEL, (long)line, (long)level);
    }

    int ScintillaControl::GetFoldLevel(int line)
    {
    	return (int)SPerform(SCI_GETFOLDLEVEL, (long)line, 0);
    }

    int ScintillaControl::GetLastChild(int line, int level)
    {
    	return (int)SPerform(SCI_GETLASTCHILD, (long)line, (long)level);
    }

    int ScintillaControl::GetFoldParent(int line)
    {
    	return (int)SPerform(SCI_GETFOLDPARENT, (long)line, 0);
    }

    void ScintillaControl::ShowLines(int lineStart, int lineEnd)
    {
    	SPerform(SCI_SHOWLINES, (long)lineStart, (long)lineEnd);
    }

    void ScintillaControl::HideLines(int lineStart, int lineEnd)
    {
    	SPerform(SCI_HIDELINES, (long)lineStart, (long)lineEnd);
    }

    bool ScintillaControl::GetLineVisible(int line)
    {
    	return SPerform(SCI_GETLINEVISIBLE, (long)line, 0) != 0;
    }

    void ScintillaControl::SetFoldExpanded(int line, bool expanded)
    {
    	SPerform(SCI_SETFOLDEXPANDED, (long)line, (long)expanded);
    }

    bool ScintillaControl::GetFoldExpanded(int line)
    {
    	return SPerform(SCI_GETFOLDEXPANDED, (long)line, 0) != 0;
    }

    void ScintillaControl::ToggleFold(int line)
    {
    	SPerform(SCI_TOGGLEFOLD, (long)line, 0);
    }

    void ScintillaControl::EnsureVisible(int line)
    {
    	SPerform(SCI_ENSUREVISIBLE, (long)line, 0);
    }

    void ScintillaControl::SetFoldFlags(int flags)
    {
    	SPerform(SCI_SETFOLDFLAGS, (long)flags, 0);
    }

    void ScintillaControl::EnsureVisibleEnforcePolicy(int line)
    {
    	SPerform(SCI_ENSUREVISIBLEENFORCEPOLICY, (long)line, 0);
    }

    void ScintillaControl::set_TabIndents(bool tabIndents)
    {
    	SPerform(SCI_SETTABINDENTS, (long)tabIndents, 0);
    }

    bool ScintillaControl::get_TabIndents()
    {
    	return SPerform(SCI_GETTABINDENTS, 0, 0) != 0;
    }


    void ScintillaControl::set_MouseDwellTime(int periodMilliseconds)
    {
    	SPerform(SCI_SETMOUSEDWELLTIME, (long)periodMilliseconds, 0);
    }

    int ScintillaControl::get_MouseDwellTime()
    {
    	return (int)SPerform(SCI_GETMOUSEDWELLTIME, 0, 0);
    }

    int ScintillaControl::WordStartPosition(long pos, bool onlyWordCharacters)
    {
    	return (int)SPerform(SCI_WORDSTARTPOSITION, pos, (long)onlyWordCharacters);
    }

    int ScintillaControl::WordEndPosition(long pos, bool onlyWordCharacters)
    {
    	return (int)SPerform(SCI_WORDENDPOSITION, pos, (long)onlyWordCharacters);
    }

    void ScintillaControl::set_WrapMode(int mode)
    {
    	SPerform(SCI_SETWRAPMODE, (long)mode, 0);
    }

    int ScintillaControl::get_WrapMode()
    {
    	return (int)SPerform(SCI_GETWRAPMODE, 0, 0);
    }

    void ScintillaControl::set_LayoutCache(int mode)
    {
    	SPerform(SCI_SETLAYOUTCACHE, (long)mode, 0);
    }

    int ScintillaControl::get_LayoutCache()
    {
    	return (int)SPerform(SCI_GETLAYOUTCACHE, 0, 0);
    }

    void ScintillaControl::set_ScrollWidth(int pixelWidth)
    {
    	SPerform(SCI_SETSCROLLWIDTH, (long)pixelWidth, 0);
    }

    int ScintillaControl::get_ScrollWidth()
    {
    	return (int)SPerform(SCI_GETSCROLLWIDTH, 0, 0);
    }

    int ScintillaControl::TextWidth(int style, String* text)
    {
		pinnedbytepointer ptr = &(System::Text::Encoding::ASCII->GetBytes( text )[0]);
    	return (int)SPerform(SCI_TEXTWIDTH, (long)style, (long)ptr);
    }

    void ScintillaControl::set_EndAtLastLine(bool endAtLastLine)
    {
    	SPerform(SCI_SETENDATLASTLINE, (long)endAtLastLine, 0);
    }

    bool ScintillaControl::get_EndAtLastLine()
    {
    	return SPerform(SCI_GETENDATLASTLINE, 0, 0) != 0;
    }

    int ScintillaControl::TextHeight(int line)
    {
    	return (int)SPerform(SCI_TEXTHEIGHT, (long)line, 0);
    }

    void ScintillaControl::set_VerticalScrollBarVisible(bool show)
    {
    	SPerform(SCI_SETVSCROLLBAR, (long)show, 0);
    }

    bool ScintillaControl::get_VerticalScrollBarVisible()
    {
    	return SPerform(SCI_GETVSCROLLBAR, 0, 0) != 0;
    }

    void ScintillaControl::AppendText(int length, String* text)
    {
		pinnedbytepointer ptr = &(System::Text::Encoding::ASCII->GetBytes( text )[0]);
    	SPerform(SCI_APPENDTEXT, (long)length, (long)ptr);
    }

    void ScintillaControl::LineDown()
    {
    	SPerform(SCI_LINEDOWN, 0, 0);
    }

    void ScintillaControl::LineDownExtend()
    {
    	SPerform(SCI_LINEDOWNEXTEND, 0, 0);
    }

    void ScintillaControl::LineUp()
    {
    	SPerform(SCI_LINEUP, 0, 0);
    }

    void ScintillaControl::LineUpExtend()
    {
    	SPerform(SCI_LINEUPEXTEND, 0, 0);
    }

    void ScintillaControl::CharLeft()
    {
    	SPerform(SCI_CHARLEFT, 0, 0);
    }

    void ScintillaControl::CharLeftExtend()
    {
    	SPerform(SCI_CHARLEFTEXTEND, 0, 0);
    }

    void ScintillaControl::CharRight()
    {
    	SPerform(SCI_CHARRIGHT, 0, 0);
    }

    void ScintillaControl::CharRightExtend()
    {
    	SPerform(SCI_CHARRIGHTEXTEND, 0, 0);
    }

    void ScintillaControl::WordLeft()
    {
    	SPerform(SCI_WORDLEFT, 0, 0);
    }

    void ScintillaControl::WordLeftExtend()
    {
    	SPerform(SCI_WORDLEFTEXTEND, 0, 0);
    }

    void ScintillaControl::WordRight()
    {
    	SPerform(SCI_WORDRIGHT, 0, 0);
    }

    void ScintillaControl::WordRightExtend()
    {
    	SPerform(SCI_WORDRIGHTEXTEND, 0, 0);
    }

    void ScintillaControl::Home()
    {
    	SPerform(SCI_HOME, 0, 0);
    }

    void ScintillaControl::HomeExtend()
    {
    	SPerform(SCI_HOMEEXTEND, 0, 0);
    }

    void ScintillaControl::LineEnd()
    {
    	SPerform(SCI_LINEEND, 0, 0);
    }

    void ScintillaControl::LineEndExtend()
    {
    	SPerform(SCI_LINEENDEXTEND, 0, 0);
    }

    void ScintillaControl::DocumentStart()
    {
    	SPerform(SCI_DOCUMENTSTART, 0, 0);
    }

    void ScintillaControl::DocumentStartExtend()
    {
    	SPerform(SCI_DOCUMENTSTARTEXTEND, 0, 0);
    }

    void ScintillaControl::DocumentEnd()
    {
    	SPerform(SCI_DOCUMENTEND, 0, 0);
    }

    void ScintillaControl::DocumentEndExtend()
    {
    	SPerform(SCI_DOCUMENTENDEXTEND, 0, 0);
    }

    void ScintillaControl::PageUp()
    {
    	SPerform(SCI_PAGEUP, 0, 0);
    }

    void ScintillaControl::PageUpExtend()
    {
    	SPerform(SCI_PAGEUPEXTEND, 0, 0);
    }

    void ScintillaControl::PageDown()
    {
    	SPerform(SCI_PAGEDOWN, 0, 0);
    }

    void ScintillaControl::PageDownExtend()
    {
    	SPerform(SCI_PAGEDOWNEXTEND, 0, 0);
    }

    void ScintillaControl::EditToggleOvertype()
    {
    	SPerform(SCI_EDITTOGGLEOVERTYPE, 0, 0);
    }

    void ScintillaControl::Cancel()
    {
    	SPerform(SCI_CANCEL, 0, 0);
    }

    void ScintillaControl::DeleteBack()
    {
    	SPerform(SCI_DELETEBACK, 0, 0);
    }

    void ScintillaControl::Tab()
    {
    	SPerform(SCI_TAB, 0, 0);
    }

    void ScintillaControl::BackTab()
    {
    	SPerform(SCI_BACKTAB, 0, 0);
    }

    void ScintillaControl::NewLine()
    {
    	SPerform(SCI_NEWLINE, 0, 0);
    }

    void ScintillaControl::FormFeed()
    {
    	SPerform(SCI_FORMFEED, 0, 0);
    }

    void ScintillaControl::VCHome()
    {
    	SPerform(SCI_VCHOME, 0, 0);
    }

    void ScintillaControl::VCHomeExtend()
    {
    	SPerform(SCI_VCHOMEEXTEND, 0, 0);
    }

    void ScintillaControl::ZoomIn()
    {
    	SPerform(SCI_ZOOMIN, 0, 0);
    }

    void ScintillaControl::ZoomOut()
    {
    	SPerform(SCI_ZOOMOUT, 0, 0);
    }

    void ScintillaControl::DelWordLeft()
    {
    	SPerform(SCI_DELWORDLEFT, 0, 0);
    }

    void ScintillaControl::DelWordRight()
    {
    	SPerform(SCI_DELWORDRIGHT, 0, 0);
    }

    void ScintillaControl::LineCut()
    {
    	SPerform(SCI_LINECUT, 0, 0);
    }

    void ScintillaControl::LineDelete()
    {
    	SPerform(SCI_LINEDELETE, 0, 0);
    }

    void ScintillaControl::LineTranspose()
    {
    	SPerform(SCI_LINETRANSPOSE, 0, 0);
    }

    void ScintillaControl::LineDuplicate()
    {
    	SPerform(SCI_LINEDUPLICATE, 0, 0);
    }

    void ScintillaControl::LowerCase()
    {
    	SPerform(SCI_LOWERCASE, 0, 0);
    }

    void ScintillaControl::UpperCase()
    {
    	SPerform(SCI_UPPERCASE, 0, 0);
    }

    void ScintillaControl::LineScrollDown()
    {
    	SPerform(SCI_LINESCROLLDOWN, 0, 0);
    }

    void ScintillaControl::LineScrollUp()
    {
    	SPerform(SCI_LINESCROLLUP, 0, 0);
    }

    void ScintillaControl::DeleteBackNotLine()
    {
    	SPerform(SCI_DELETEBACKNOTLINE, 0, 0);
    }

    void ScintillaControl::HomeDisplay()
    {
    	SPerform(SCI_HOMEDISPLAY, 0, 0);
    }

    void ScintillaControl::HomeDisplayExtend()
    {
    	SPerform(SCI_HOMEDISPLAYEXTEND, 0, 0);
    }

    void ScintillaControl::LineEndDisplay()
    {
    	SPerform(SCI_LINEENDDISPLAY, 0, 0);
    }

    void ScintillaControl::LineEndDisplayExtend()
    {
    	SPerform(SCI_LINEENDDISPLAYEXTEND, 0, 0);
    }

    void ScintillaControl::LineCopy()
    {
    	SPerform(SCI_LINECOPY , 0, 0);
    }

	void ScintillaControl::MoveCaretInsideView()
    {
    	SPerform(SCI_MOVECARETINSIDEVIEW, 0, 0);
    }

    int ScintillaControl::LineLength(int line)
    {
    	return (int)SPerform(SCI_LINELENGTH, (long)line, 0);
    }

    void ScintillaControl::BraceHighlight(long pos1, long pos2)
    {
    	SPerform(SCI_BRACEHIGHLIGHT, pos1, pos2);
    }

    void ScintillaControl::BraceBadLight(long pos)
    {
    	SPerform(SCI_BRACEBADLIGHT, pos, 0);
    }

    long ScintillaControl::BraceMatch(long pos)
    {
    	return SPerform(SCI_BRACEMATCH, pos, 0);
    }


    int ScintillaControl::GetDocPointer()
    {
    	return (int)SPerform(SCI_GETDOCPOINTER, 0, 0);
    }

    void ScintillaControl::SetDocPointer(int pointer)
    {
    	SPerform(SCI_SETDOCPOINTER, 0, (long)pointer);
    }


    void ScintillaControl::SearchAnchor()
    {
    	SPerform(SCI_SEARCHANCHOR, 0, 0);
    }

    int ScintillaControl::SearchNext(int flags, String* text)
    {
		pinnedbytepointer ptr = &(System::Text::Encoding::ASCII->GetBytes( text )[0]);
    	return (int)SPerform(SCI_SEARCHNEXT, (long)flags, (long)ptr);
    }

    int ScintillaControl::SearchPrev(int flags, String* text)
    {
		pinnedbytepointer ptr = &(System::Text::Encoding::ASCII->GetBytes( text )[0]);
    	return (int)SPerform(SCI_SEARCHPREV, (long)flags, (long)ptr);
    }

    int ScintillaControl::CreateDocument()
    {
    	return (int)SPerform(SCI_CREATEDOCUMENT, 0, 0);
    }

    void ScintillaControl::AddRefDocument(int doc)
    {
    	SPerform(SCI_ADDREFDOCUMENT, 0, (long)doc);
    }

    void ScintillaControl::ReleaseDocument(int doc)
    {
    	SPerform(SCI_RELEASEDOCUMENT, 0, (long)doc);
    }

    void ScintillaControl::WordPartLeft()
    {
    	SPerform(SCI_WORDPARTLEFT, 0, 0);
    }

    void ScintillaControl::WordPartLeftExtend()
    {
    	SPerform(SCI_WORDPARTLEFTEXTEND, 0, 0);
    }

    void ScintillaControl::WordPartRight()
    {
    	SPerform(SCI_WORDPARTRIGHT, 0, 0);
    }

    void ScintillaControl::WordPartRightExtend()
    {
    	SPerform(SCI_WORDPARTRIGHTEXTEND, 0, 0);
    }

    void ScintillaControl::SetVisiblePolicy(int visiblePolicy, int visibleSlop)
    {
    	SPerform(SCI_SETVISIBLEPOLICY, (long)visiblePolicy, (long)visibleSlop);
    }

    void ScintillaControl::DelLineLeft()
    {
    	SPerform(SCI_DELLINELEFT, 0, 0);
    }

    void ScintillaControl::DelLineRight()
    {
    	SPerform(SCI_DELLINERIGHT, 0, 0);
    }

    void ScintillaControl::ChooseCaretX()
    {
    	SPerform(SCI_CHOOSECARETX, 0, 0);
    }

    void ScintillaControl::GrabFocus()
    {
    	SPerform(SCI_GRABFOCUS, 0, 0);
    }

    void ScintillaControl::SetXCaretPolicy(int caretPolicy, int caretSlop)
    {
    	SPerform(SCI_SETXCARETPOLICY, (long)caretPolicy, (long)caretSlop);
    }

    void ScintillaControl::SetYCaretPolicy(int caretPolicy, int caretSlop)
    {
    	SPerform(SCI_SETYCARETPOLICY, (long)caretPolicy, (long)caretSlop);
    }

    void ScintillaControl::StartRecord()
    {
    	SPerform(SCI_STARTRECORD, 0, 0);
    }

    void ScintillaControl::StopRecord()
    {
    	SPerform(SCI_STOPRECORD, 0, 0);
    }

    void ScintillaControl::Colourise(long start, long end)
    {
    	SPerform(SCI_COLOURISE, start, end);
    }

	bool ScintillaControl::GetTwoPhaseDraw()
	{
		return SPerform(SCI_GETTWOPHASEDRAW , 0, 0 ) != 0;
	}

	void ScintillaControl::SetTwoPhaseDraw(bool twoPhase)
	{
		SPerform(SCI_SETTWOPHASEDRAW, (long)twoPhase, 0);
	}

	void ScintillaControl::TargetFromSelection()
	{
		SPerform(SCI_TARGETFROMSELECTION, 0, 0);
	}

	void ScintillaControl::LinesJoin()
	{
		SPerform(SCI_LINESJOIN, 0, 0);
	}

	void ScintillaControl::LinesSplit(int pixelWidth)
	{
		SPerform(SCI_LINESSPLIT, (long)pixelWidth, 0);
	}

	void ScintillaControl::SetFoldMarginColour(bool useSetting, long back)
	{
		SPerform(SCI_SETFOLDMARGINCOLOUR, (long)useSetting, back);
	}
	void ScintillaControl::SetFoldMarginHiColour(bool useSetting, long fore)
	{
		SPerform(SCI_SETFOLDMARGINHICOLOUR, (long)useSetting, fore);
	}

	void ScintillaControl::HomeWrap()
	{
		SPerform( SCI_HOMEWRAP  , 0, 0 );
	}
	void ScintillaControl::HomeWrapExtend()
	{
		SPerform( SCI_HOMEWRAPEXTEND , 0, 0 );
	}
	void ScintillaControl::LineEndWrap()
	{
		SPerform( SCI_LINEENDWRAP  , 0, 0 );
	}
	void ScintillaControl::LineEndWrapExtend()
	{
		SPerform( SCI_LINEENDWRAPEXTEND , 0, 0 );
	}
	void ScintillaControl::VCHomeWrap()
	{
		SPerform( SCI_VCHOMEWRAP  , 0, 0 );
	}
	void ScintillaControl::VCHomeWrapExtend()
	{
		SPerform( SCI_VCHOMEWRAPEXTEND , 0, 0 );
	}


	void ScintillaControl::SetHotspotActiveFore(bool useSetting, long fore)
	{
		SPerform( SCI_SETHOTSPOTACTIVEFORE  , (long)useSetting , (long)fore );
	}

	void ScintillaControl::SetHotspotActiveBack(bool useSetting, long back)
	{
		SPerform( SCI_SETHOTSPOTACTIVEBACK  , (long)useSetting , back );
	}

	void ScintillaControl::SetHotspotActiveUnderline(bool underline)
	{
		SPerform( SCI_SETHOTSPOTACTIVEUNDERLINE  , (long)underline , 0 );
	}

	void ScintillaControl::SetHotspotSingleLine(bool underline)
	{
		SPerform( SCI_SETHOTSPOTSINGLELINE  , (long)underline , 0 );
	}

	void ScintillaControl::ParaDown()
	{
		SPerform( SCI_PARADOWN  , 0 , 0 );
	}
	void ScintillaControl::ParaDownExtend()
	{
		SPerform( SCI_PARADOWNEXTEND , 0 , 0 );
	}
	void ScintillaControl::ParaUp()
	{
		SPerform( SCI_PARAUP , 0 , 0 );
	}
	void ScintillaControl::ParaUpExtend()
	{
		SPerform( SCI_PARAUPEXTEND , 0 , 0 );
	}

	long ScintillaControl::PositionBefore(long pos)
	{
		return SPerform( SCI_POSITIONBEFORE , pos , 0 );
	}
	long ScintillaControl::PositionAfter(long pos)
	{
		return SPerform( SCI_POSITIONAFTER  , pos , 0 );
	}

	void ScintillaControl::CopyRange( long start, long end )
	{
		SPerform( SCI_COPYRANGE  , start , end );
	}

	void ScintillaControl::CopyText( long length, String* text)
	{
		pinnedbytepointer ptr = &(System::Text::Encoding::ASCII->GetBytes( text )[0]);
		SPerform(SCI_COPYTEXT, (long)length, (long)ptr);
	}

	/// ------------------- .Net Frienly Code follows here. --------------------------------

	int ScintillaControl::get_AutoCGetTypeSeparator()
	{
		return (int)SPerform(SCI_AUTOCGETTYPESEPARATOR , 0, 0);
	}

	void ScintillaControl::set_AutoCSetTypeSeparator(int separatorCharacter)
	{
		SPerform(SCI_AUTOCSETTYPESEPARATOR , (long)separatorCharacter, 0);
	}

    int ScintillaControl::get_CaretPeriod()
    {
    	return (int)SPerform(SCI_GETCARETPERIOD, 0, 0);
    }

    void ScintillaControl::set_CaretPeriod(int periodMilliseconds)
    {
    	SPerform(SCI_SETCARETPERIOD, (long)periodMilliseconds, 0);
    }

    void ScintillaControl::set_MarginTypeN(int margin, int marginType)
    {
    	SPerform(SCI_SETMARGINTYPEN, (long)margin, (long)marginType);
    }

    int ScintillaControl::get_MarginTypeN(int margin)
    {
    	return (int)SPerform(SCI_GETMARGINTYPEN, (long)margin, 0);
    }

    void ScintillaControl::set_MarginWidthN(int margin, int pixelWidth)
    {
    	SPerform(SCI_SETMARGINWIDTHN, (long)margin, (long)pixelWidth);
    }

    int ScintillaControl::get_MarginWidthN(int margin)
    {
    	return (int)SPerform(SCI_GETMARGINWIDTHN, (long)margin, 0);
    }

    void ScintillaControl::set_MarginMaskN(int margin, int mask)
    {
    	SPerform(SCI_SETMARGINMASKN, (long)margin, (long)mask);
    }

    int ScintillaControl::get_MarginMaskN(int margin)
    {
    	return (int)SPerform(SCI_GETMARGINMASKN, (long)margin, 0);
    }

    void ScintillaControl::set_MarginSensitiveN(int margin, bool sensitive)
    {
    	SPerform(SCI_SETMARGINSENSITIVEN, (long)margin, (long)sensitive);
    }

    bool ScintillaControl::get_MarginSensitiveN(int margin)
    {
    	return SPerform(SCI_GETMARGINSENSITIVEN, (long)margin, 0) != 0;
    }


    void ScintillaControl::set_TabWidth(int tabWidth)
    {
    	SPerform(SCI_SETTABWIDTH, (long)tabWidth, 0);
    }

    int ScintillaControl::get_TabWidth()
    {
    	return (int)SPerform(SCI_GETTABWIDTH, 0, 0);
    }
    long ScintillaControl::get_EndStyled()
    {
    	return SPerform(SCI_GETENDSTYLED, 0, 0);
    }

	int ScintillaControl::get_EOLMode()
    {
    	return (int)SPerform(SCI_GETEOLMODE, 0, 0);
    }

    void ScintillaControl::set_EOLMode(int eolMode)
    {
    	SPerform(SCI_SETEOLMODE, (long)eolMode, 0);
    }

    void ScintillaControl::set_UndoCollection(bool collectUndo)
    {
    	SPerform(SCI_SETUNDOCOLLECTION, (long)collectUndo, 0);
    }

    bool ScintillaControl::get_IsUndoCollected()
    {
    	return SPerform(SCI_GETUNDOCOLLECTION, 0, 0) != 0;
    }

    int ScintillaControl::get_WhitespaceVisibleState()
    {
    	return (int)SPerform(SCI_GETVIEWWS, 0, 0);
    }

    void ScintillaControl::set_WhitespaceVisibleState(int viewWS)
    {
    	SPerform(SCI_SETVIEWWS, (long)viewWS, 0);
    }

    bool ScintillaControl::get_CanRedo()
    {
    	return SPerform(SCI_CANREDO, 0, 0) != 0;
    }


    int ScintillaControl::get_Length()
    {
    	return (int)SPerform(SCI_GETLENGTH, 0, 0);
    }

    int ScintillaControl::get_CharAt(long pos)
    {
    	return (int)SPerform(SCI_GETCHARAT, pos, 0);
    }

    long ScintillaControl::get_CurrentPos()
    {
    	return SPerform(SCI_GETCURRENTPOS, 0, 0);
    }

    int ScintillaControl::get_StyleAt(long pos)
    {
    	return (int)SPerform(SCI_GETSTYLEAT, pos, 0);
    }

    long ScintillaControl::get_CaretLineBackground()
    {
    	return SPerform(SCI_GETCARETLINEBACK, 0, 0);
    }

    void ScintillaControl::set_CaretLineBackground(long back)
    {
    	SPerform(SCI_SETCARETLINEBACK, back, 0);
    }

    long ScintillaControl::get_CaretForeground()
    {
    	return SPerform(SCI_GETCARETFORE, 0, 0);
    }

	void ScintillaControl::set_CaretForeground(long fore)
    {
    	SPerform(SCI_SETCARETFORE, fore, 0);
    }

    bool ScintillaControl::get_BufferedDraw()
    {
    	return SPerform(SCI_GETBUFFEREDDRAW, 0, 0) != 0;
    }

    void ScintillaControl::set_BufferedDraw(bool buffered)
    {
    	SPerform(SCI_SETBUFFEREDDRAW, (long)buffered, 0);
    }


    void ScintillaControl::set_BackSpaceUnIndents(bool bsUnIndents)
    {
    	SPerform(SCI_SETBACKSPACEUNINDENTS, (long)bsUnIndents, 0);
    }

    bool ScintillaControl::get_BackSpaceUnIndents()
    {
    	return SPerform(SCI_GETBACKSPACEUNINDENTS, 0, 0) != 0;
    }

    long ScintillaControl::get_AnchorPosition()
    {
    	return SPerform(SCI_GETANCHOR, 0, 0);
    }

	void ScintillaControl::set_AnchorPosition(long posAnchor)
    {
    	SPerform(SCI_SETANCHOR, posAnchor, 0);
    }


    bool ScintillaControl::get_EOLCharactersVisible()
    {
    	return SPerform(SCI_GETVIEWEOL, 0, 0) != 0;
    }

    void ScintillaControl::set_EOLCharactersVisible(bool visible)
    {
    	SPerform(SCI_SETVIEWEOL, (long)visible, 0);
    }


    void ScintillaControl::set_focus(bool focus)
    {
		Control::Focus();
    	SPerform(SCI_SETFOCUS, (long)focus, 0);
    }

    bool ScintillaControl::get_focus()
    {
    	return SPerform(SCI_GETFOCUS, 0, 0) != 0;
    }


    bool ScintillaControl::get_IsSelectionRectangle()
    {
    	return SPerform(SCI_SELECTIONISRECTANGLE, 0, 0) != 0;
    }

    int ScintillaControl::get_LinesOnScreen()
    {
    	return (int)SPerform(SCI_LINESONSCREEN, 0, 0);
    }

    int ScintillaControl::get_ModEventMask()
    {
    	return (int)SPerform(SCI_GETMODEVENTMASK, 0, 0);
    }

    void ScintillaControl::set_ModEventMask(int mask)
    {
    	SPerform(SCI_SETMODEVENTMASK, (long)mask, 0);
    }

    int ScintillaControl::get_EdgeColumn()
    {
    	return (int)SPerform(SCI_GETEDGECOLUMN, 0, 0);
    }

    void ScintillaControl::set_EdgeColumn(int column)
    {
    	SPerform(SCI_SETEDGECOLUMN, (long)column, 0);
    }

    int ScintillaControl::get_EdgeMode()
    {
    	return (int)SPerform(SCI_GETEDGEMODE, 0, 0);
    }

    void ScintillaControl::set_EdgeMode(int mode)
    {
    	SPerform(SCI_SETEDGEMODE, (long)mode, 0);
    }

    long ScintillaControl::get_EdgeColour()
    {
    	return SPerform(SCI_GETEDGECOLOUR, 0, 0);
    }

    void ScintillaControl::set_EdgeColour(long edgeColour)
    {
    	SPerform(SCI_SETEDGECOLOUR, edgeColour, 0);
    }

	void ScintillaControl::set_UsePopUp(bool allowPopUp)
    {
    	SPerform(SCI_USEPOPUP, (long)allowPopUp, 0);
    }

    void ScintillaControl::set_ZoomLevel(int zoom)
    {
    	SPerform(SCI_SETZOOM, (long)zoom, 0);
    }

    int ScintillaControl::get_ZoomLevel()
    {
    	return (int)SPerform(SCI_GETZOOM, 0, 0);
    }

	void ScintillaControl::set_Status(int statusCode)
    {
    	SPerform(SCI_SETSTATUS, (long)statusCode, 0);
    }

    int ScintillaControl::get_Status()
    {
    	return (int)SPerform(SCI_GETSTATUS, 0, 0);
    }

    void ScintillaControl::set_MouseDownCaptures(bool captures)
    {
    	SPerform(SCI_SETMOUSEDOWNCAPTURES, (long)captures, 0);
    }

    bool ScintillaControl::get_MouseDownCaptures()
    {
    	return SPerform(SCI_GETMOUSEDOWNCAPTURES, 0, 0) != 0;
    }

    void ScintillaControl::set_CursorType(int cursorType)
    {
    	SPerform(SCI_SETCURSOR, (long)cursorType, 0);
    }

    int ScintillaControl::get_CursorType()
    {
    	return (int)SPerform(SCI_GETCURSOR, 0, 0);
    }

    void ScintillaControl::set_ControlCharSymbol(int symbol)
    {
    	SPerform(SCI_SETCONTROLCHARSYMBOL, (long)symbol, 0);
    }

    int ScintillaControl::get_ControlCharSymbol()
    {
    	return (int)SPerform(SCI_GETCONTROLCHARSYMBOL, 0, 0);
    }


    void ScintillaControl::set_XOffset(int newOffset)
    {
    	SPerform(SCI_SETXOFFSET, (long)newOffset, 0);
    }

    int ScintillaControl::get_XOffset()
    {
    	return (int)SPerform(SCI_GETXOFFSET, 0, 0);
    }


    void ScintillaControl::set_PrintWrapMode(int mode)
    {
    	SPerform(SCI_SETPRINTWRAPMODE, (long)mode, 0);
    }

    int ScintillaControl::get_PrintWrapMode()
    {
    	return (int)SPerform(SCI_GETPRINTWRAPMODE, 0, 0);
    }


    void ScintillaControl::set_Lexer(int lexer)
    {
    	SPerform(SCI_SETLEXER, (long)lexer, 0);
    }

    int ScintillaControl::get_Lexer()
    {
    	return (int)SPerform(SCI_GETLEXER, 0, 0);
    }


	void ScintillaControl::set_Property(String* key, String* value)
	{
		pinnedbytepointer ptr = &(System::Text::Encoding::ASCII->GetBytes( key )[0]);
		pinnedbytepointer ptr2 = &(System::Text::Encoding::ASCII->GetBytes( value )[0]);
		SPerform(SCI_SETPROPERTY, (long)ptr, (long)ptr2);
	}

	void ScintillaControl::set_KeyWords(int keywordSet, String* keywords)
	{
		pinnedbytepointer ptr;
		if( !keywords || keywords->Length == 0 )
			return;
		ptr = &(System::Text::Encoding::ASCII->GetBytes( keywords )[0]);
		SPerform(SCI_SETKEYWORDS, (long)keywordSet, (long)ptr);
	}

	String* ScintillaControl::get_LexerLanguage()
	{
		return _lexerLanguage;
	}
	
	void ScintillaControl::set_LexerLanguage( String* lexerLanguage )
	{
		if (!lexerLanguage)
	 		return;
		// Check to see if it is the same as the current one, in order to
		// cut down on unneccasary re-encoding of the string to a bytebuffer.
		if( !lexerLanguage->Equals( _lexerLanguage ) )
		{
			_lexerLanguage = lexerLanguage;
			pinnedbytepointer ptr = &(System::Text::Encoding::ASCII->GetBytes( _lexerLanguage )[0]);
    		SPerform(SCI_SETLEXERLANGUAGE, 0, (long)ptr);
		}
	}

	/**
	* Sets the configuration object that the control will use to set styles.
	*/
	void ScintillaControl::set_Configuration( Scintilla::Configuration::Scintilla* config)
	{
		_configuration = config;
	}

	/**
	* Gets the configuration object that the control will use to set styles.
	*/
	Scintilla::Configuration::Scintilla* ScintillaControl::get_Configuration( )
	{
		return _configuration;
	}

	/**
	* Changes the language for the style profile.
	*/
	void ScintillaControl::set_ConfigurationLanguage( String* languageName )
	{
		if( !languageName || languageName->Equals("") )
			return;

		Scintilla::Configuration::Language* lang = _configuration->GetLanguage( languageName );
		if( lang == NULL )
			return;

		StyleClearAll();

		Lexer = lang->lexer->key;
		for( int j =0 ; j < lang->usestyles->Length ; j++)
		{
			Scintilla::Configuration::UseStyle* usestyle = lang->usestyles[j];

			if(usestyle->HasForegroundColor)
				StyleSetFore( usestyle->key , usestyle->ForegroundColor );
			if(usestyle->HasBackgroundColor )
				StyleSetBack( usestyle->key , usestyle->BackgroundColor );
			if( usestyle->HasFontName )
				StyleSetFont( usestyle->key , usestyle->FontName );
			if( usestyle->HasFontSize )
				StyleSetSize( usestyle->key , usestyle->FontSize );
			if( usestyle->HasBold )
				StyleSetBold( usestyle->key , usestyle->IsBold );
			if( usestyle->HasItalics )
				StyleSetItalic( usestyle->key , usestyle->IsItalics );
		}

		// clear the keywords lists	
		for( int j=0; j < 9; j++ )
			KeyWords[j] = "";

		for( int j=0; j < lang->usekeywords->Length ; j++ )
		{
			Scintilla::Configuration::UseKeyword* usekeyword = lang->usekeywords[j];
			Scintilla::Configuration::KeywordClass* kc = _configuration->GetKeywordClass( usekeyword->cls );
			if(kc)
				KeyWords[usekeyword->key] = kc->val;
		}
	}

	/**
	* Gets the language for the style profile.
	*/
	String* ScintillaControl::get_ConfigurationLanguage( )
	{
		return "";
	}

};
