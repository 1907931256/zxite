// ScintillaNet.h

#pragma once

#include "ScintillaNET.h"

using namespace System;
using namespace System::Collections;
using namespace System::Xml;

typedef  System::Byte bytebuffer __gc[];
typedef Byte (__pin *pinnedbytepointer);


namespace Scintilla
{
	public __gc class LexerConfiguration;
	
	public __gc class Style
	{
	public:

		String* classname;
		String* inheritstyle;
		
		String* foreground;
		String* background;
		String* size;
		String* font;
		String* bold;
		String* italics;
		int key;

		void inherit( Style* parentstyle );
		void resolveGlobals( LexerConfiguration* lexerconfig );

		__property bool get_IsBold();
		__property bool get_IsItalics();
		
		__property int get_ForegroundColor();
		__property int get_BackgroundColor();
		__property int get_FontSize();
		__property bytebuffer get_FontName();

		__property bool get_HasForegroundColor();
		__property bool get_HasBackgroundColor();
		__property bool get_HasFontSize();
		__property bool get_HasFontName();
		__property bool get_HasBold();
		__property bool get_HasItalics();

	};

	public __gc class Language 
	{
	public :
		Language();
		String* name;
		String* lexerName;
		int lexerKey;
		String* fileExtensions;
		Hashtable* useKeywords; // (key , keywordtext )
		Hashtable* useStyles;// (key , useStyle )
	};

	// [event_source(managed)]
	public __gc class LexerConfiguration 
	{
	private:
		Hashtable* globals; // (name,value)
		Hashtable* styleclasses; // ( name , styleclass)
		Hashtable* keywordclasses; // ( name , keywordtext )
		Hashtable* languages; // ( languagename , Language )

	protected:
		// Loading
		void AddGlobal(String* name, String* value );
		void AddStyleClass( String* name , String* inherits,  String* fore, String* back , String* size, String* font, String* bold , String* italics );
		void AddKeywordClass(String* name, bytebuffer text );

		Language* AddLanguage( String* name );
		void AddKeywords(Language* language,  String* key , String* keywordclass );
		void AddStyle( Language* language, String* key , String* name , String* inherits, String* fore, String* back , String* size, String* font, String* bold , String* italics  );

		// Retrieving Settings
		String* GetGlobal( String* name, String* toplevelkey );

		bytebuffer GetKeywordClass( String* kclass );
		Style* GetStyleClass( String* classname );

		Language* GetLanguage( String* languagename );

		void ProcessConfigFile( bytebuffer xmldata );

		void ProcessGlobals( XmlTextReader* reader );
		void ProcessKeywordClasses( XmlTextReader* reader );
		void ProcessStyleClasses( XmlTextReader* reader );
		void ProcessImports( XmlTextReader* reader );
		void ProcessLanugage( XmlTextReader* reader );
		void ProcessUseKeyword( XmlTextReader* reader , Language* lang );
		void ProcessUseStyle( XmlTextReader* reader , Language* lang );


	public:
		LexerConfiguration( bytebuffer xmldata);
		String* GetGlobal( String* name );

		void ApplyConfiguration( ScintillaControl* control , String* language );
	};
}
