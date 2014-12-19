#include "stdafx.h"
#include "LexerConfiguration.h"
#define null 0L

using namespace System::Xml;
using namespace System::IO;

#define TO_COLORREF( c ) (((c & 0xff0000) >> 16) + ((c & 0x0000ff) << 16) + (c & 0x00ff00) )

namespace Scintilla
{
	String* bytebufferToString( bytebuffer b )
	{
		pinnedbytepointer ptr = &b[0];
		return new String( (const char*)ptr);
	}

	void Style::inherit( Style* parentstyle )		
	{
		if( !HasForegroundColor  )
			foreground = parentstyle->foreground;
		if( !HasBackgroundColor  )
			background = parentstyle->background;
		if( !HasFontSize )
			size = parentstyle->size;
		if( !HasFontName )
			font = parentstyle->font;
		if( !bold || bold->Equals( String::Empty ) )
			bold = parentstyle->bold;
		if( !italics|| italics->Equals( String::Empty ) )
			italics = parentstyle->italics;
		
		inheritstyle = parentstyle->inheritstyle;
		
	}

	void Style::resolveGlobals( LexerConfiguration* lexerconfig )
	{
		if( HasForegroundColor )
			foreground = lexerconfig->GetGlobal( foreground );
		if( HasBackgroundColor )
			background = lexerconfig->GetGlobal( background );
		if( HasFontSize )
			size = lexerconfig->GetGlobal( size );
		if( HasFontName )
			font = lexerconfig->GetGlobal( font );
		if( HasBold )
			bold = lexerconfig->GetGlobal( bold );
		if( HasItalics)
			italics = lexerconfig->GetGlobal( italics );
	}

	bool Style::get_IsBold()
	{
		return bold && bold->Equals("true");
	}
	bool Style::get_IsItalics()
	{
		return italics && italics->Equals("true");
	}

	int strToColor( String* s )
	{
		if( s && !s->Equals(String::Empty) )
		{
			// first try known-color string
			System::Drawing::Color c = System::Drawing::Color::FromName( s );

			if( c.ToArgb() == 0 )
			{
				// didn't find, or is black.
				//hex?
				if( s->IndexOf( "0x" ) == 0 )
					return Int32::Parse( s->Substring( 2 ) , System::Globalization::NumberStyles::HexNumber );
				else //decimal
				try
				{
					return Int32::Parse( s );
				}
				catch( ... )
				{
				}
			}
			return c.ToArgb() & 0x00ffffff;
		}
		return 0;
	}

	int Style::get_ForegroundColor()
	{
		int c = strToColor( foreground );

		return TO_COLORREF( strToColor( foreground ) );
	}
	int Style::get_BackgroundColor()
	{
		return TO_COLORREF( strToColor( background ) );
	}
	int Style::get_FontSize()
	{
		//hex?
		if( size->IndexOf( "0x" ) == 0 )
			return Int32::Parse( size->Substring( 2 ) , System::Globalization::NumberStyles::HexNumber );
		else //decimal
		try
		{
			return Int32::Parse( size );
		}
		catch( ... )
		{
		}
		return 10; //10 pt.
	}
	bytebuffer Style::get_FontName()
	{
		if( this->HasFontName )
			return System::Text::Encoding::ASCII->GetBytes(font);
		return new System::Byte[0];
	}

	bool Style::get_HasForegroundColor()
	{
		return foreground && !foreground->Equals(String::Empty);
	}
	bool Style::get_HasBackgroundColor()
	{
		return background && !background ->Equals(String::Empty);
	}
	bool Style::get_HasFontSize()
	{
		return size && !size->Equals(String::Empty);
	}
	bool Style::get_HasFontName()
	{
		return font && !font->Equals(String::Empty);
	}
	bool Style::get_HasBold()
	{
		return bold && !bold->Equals(String::Empty);
	}
	bool Style::get_HasItalics()
	{
		return italics && !italics->Equals(String::Empty);
	}

	Language::Language()
	{
		useKeywords = new Hashtable();
		useStyles = new Hashtable();
	}


	LexerConfiguration::LexerConfiguration( bytebuffer xmldata )
	{
		globals = new Hashtable();
		styleclasses = new Hashtable();
		keywordclasses = new Hashtable();
		languages = new Hashtable();
		ProcessConfigFile( xmldata );	
	}

	void LexerConfiguration::ProcessConfigFile( bytebuffer xmldata )
	{
		XmlTextReader* reader = new XmlTextReader( new MemoryStream(xmldata) );
		
		while (reader->Read())
		{
			// skip it all until we reach the Scintilla Element
			if( reader->NodeType == XmlNodeType::Element && reader->Name->Equals("Scintilla" ) && !reader->IsEmptyElement )
			{
				// process the Scintilla element 
				while (reader->Read() && !(reader->NodeType == XmlNodeType::EndElement && reader->Name->Equals("Scintilla") ) )
				{
					if(reader->NodeType == XmlNodeType::Element && !reader->IsEmptyElement )
					{
						String* name = reader->Name;
						if( reader->Name->Equals("globals") )
							ProcessGlobals( reader );
						else if( reader->Name->Equals("keyword-classes") )
							ProcessKeywordClasses( reader );
						else if( reader->Name->Equals("style-classes") )
							ProcessStyleClasses( reader );
						else if( reader->Name->Equals("language") )
							ProcessLanugage( reader );
						else if( reader->Name->Equals("imports") )
							ProcessImports( reader );
					}
				}
			}
		}
	}
  
	void LexerConfiguration::ProcessGlobals( XmlTextReader* reader )
	{
		String* value;
		String* name;
		while (reader->Read() && !( reader->NodeType == XmlNodeType::EndElement /* && reader->Name->Equals("globals") */ ) )
		{
			if (reader->NodeType == XmlNodeType::Element && reader->LocalName->Equals("value" ) )
			{
				name = reader->GetAttribute("name");
				if( reader->IsEmptyElement )
					AddGlobal( name , "" );
				else
				{
					reader->Read();
					value = reader->Value;
					AddGlobal( name , value );
					while(!(reader->NodeType == XmlNodeType::EndElement && reader->LocalName->Equals("value" ) ) )
						reader->Read(); // get rid of the closing tag.
				}
			}
		}
	}

	void LexerConfiguration::ProcessKeywordClasses( XmlTextReader* reader )
	{
		String* value;
		String* name;
		while (reader->Read() && !( reader->NodeType == XmlNodeType::EndElement  ) )
		{
			if (reader->NodeType == XmlNodeType::Element && reader->LocalName->Equals("keyword-class" ) )
			{
				name = reader->GetAttribute("name");
				if( reader->IsEmptyElement )
					AddKeywordClass( name , System::Text::Encoding::ASCII->GetBytes("") );
				else
				{
					reader->Read();
					value = reader->Value;
					value = value->Replace("\r","")->Replace("\n" , "" )->Replace("\t" , " " );
					while( value->IndexOf("  ") != -1 )
						value = value->Replace("  " , " " );

					AddKeywordClass( name , System::Text::Encoding::ASCII->GetBytes( value ) );

					while(!(reader->NodeType == XmlNodeType::EndElement && reader->LocalName->Equals("keyword-class" ) ) )
						reader->Read(); // get rid of the closing tag.
				}
			}
		}
	}
	void LexerConfiguration::ProcessStyleClasses( XmlTextReader* reader )
	{
		String* name;
		while (reader->Read() && !( reader->NodeType == XmlNodeType::EndElement  ) )
		{
			if (reader->NodeType == XmlNodeType::Element && reader->LocalName->Equals("style-class" ) )
			{
				name = reader->GetAttribute("name");
				if( !name->Equals(String::Empty) )
					AddStyleClass( name  , reader->GetAttribute("inherit-style") , reader->GetAttribute("fore"), reader->GetAttribute("back"), reader->GetAttribute("size"), reader->GetAttribute("font"), reader->GetAttribute("bold") , reader->GetAttribute("italics") );

				if( !reader->IsEmptyElement  )
				{
					while(!(reader->NodeType == XmlNodeType::EndElement && reader->LocalName->Equals("style-class" ) ) )
						reader->Read(); // get rid of the closing tag.
				}
				
			}
		}

	}
	void LexerConfiguration::ProcessImports( XmlTextReader* reader )
	{
		String* name;
		while (reader->Read() && !( reader->NodeType == XmlNodeType::EndElement /* && reader->Name->Equals("imports") */ ) )
		{
			if (reader->NodeType == XmlNodeType::Element && reader->LocalName->Equals("file" ) )
			{
				name = reader->GetAttribute("name");
				/*Import( name ) */ ;

				if(! reader->IsEmptyElement )
				{
					while(!(reader->NodeType == XmlNodeType::EndElement && reader->LocalName->Equals("file" ) ) )
						reader->Read(); // get rid of the closing tag.
				}
			}
		}
	}
	void LexerConfiguration::ProcessLanugage( XmlTextReader* reader )
	{
		String* name;
		name = reader->GetAttribute("name");
		Language* lang = AddLanguage( name );

		while (reader->Read() && !( reader->NodeType == XmlNodeType::EndElement  ) )
		{
			if(reader->NodeType == XmlNodeType::Element  )
			{
				String* name = reader->Name;
				if( reader->Name->Equals("use-keywords") )
					ProcessUseKeyword( reader , lang );
				else if( reader->Name->Equals("use-styles") )
					ProcessUseStyle( reader , lang );
				else if( reader->Name->Equals("lexer") )
				{
					name = reader->GetAttribute("name");
					
					
					if( !name->Equals(String::Empty) )
						lang->lexerName = name ;

					lang->lexerKey = Int32::Parse( reader->GetAttribute("key") );

					if( !reader->IsEmptyElement  )
						while(!(reader->NodeType == XmlNodeType::EndElement && reader->LocalName->Equals("lexer" ) ) )
							reader->Read(); // get rid of the closing tag.
				}
				else if( reader->Name->Equals("file-extensions") )
				{
					if( !reader->IsEmptyElement )
					{
						reader->Read();
						lang->fileExtensions = reader->Value;

						while(!(reader->NodeType == XmlNodeType::EndElement && reader->LocalName->Equals("file-extensions" ) ) )
							reader->Read(); // get rid of the closing tag.
					}
				}
			}
		}
	}
	void LexerConfiguration::ProcessUseKeyword( XmlTextReader* reader , Language* lang )
	{
		String* value;
		String* key;
		while (reader->Read() && !( reader->NodeType == XmlNodeType::EndElement /* && reader->Name->Equals("imports") */ ) )
		{
			if (reader->NodeType == XmlNodeType::Element && reader->LocalName->Equals("keyword" ) )
			{
				key = reader->GetAttribute("key");
				value = reader->GetAttribute("class");
				AddKeywords( lang , key , value );

				if(!reader->IsEmptyElement )
					while(!(reader->NodeType == XmlNodeType::EndElement && reader->LocalName->Equals("keyword" ) ) )
						reader->Read(); // get rid of the closing tag.
			}
		}
	}
	void LexerConfiguration::ProcessUseStyle( XmlTextReader* reader , Language* lang )
	{
		String* value;
		String* key;
		while (reader->Read() && !( reader->NodeType == XmlNodeType::EndElement /* && reader->Name->Equals("imports") */ ) )
		{
			if (reader->NodeType == XmlNodeType::Element && reader->LocalName->Equals("style" ) )
			{
				key = reader->GetAttribute("key");
 				value = reader->GetAttribute("class");
				AddStyle( lang , key , reader->GetAttribute("name") , reader->GetAttribute("class") , reader->GetAttribute("fore"), reader->GetAttribute("back"), reader->GetAttribute("size"), reader->GetAttribute("font"), reader->GetAttribute("bold") , reader->GetAttribute("italics") );
				
				if(!reader->IsEmptyElement )
					while(!(reader->NodeType == XmlNodeType::EndElement && reader->LocalName->Equals("style" ) ) )
						reader->Read(); // get rid of the closing tag.
			}
		}
	}
 
	void LexerConfiguration::ApplyConfiguration( ScintillaControl* control , String* language )
	{
		Language* lang = GetLanguage( language );

		IDictionaryEnumerator* iter = lang->useStyles->GetEnumerator();
		while( iter->MoveNext() )
		{
			Style* tmp = dynamic_cast<Style*>(iter->Value);
			int key = (tmp)->key;

			Style* s = new Style();
			s->classname = tmp->classname;
			s->inherit( tmp );

			while( s->inheritstyle && !s->inheritstyle->Equals(String::Empty) )
				s->inherit( GetStyleClass( s->inheritstyle ) );

			s->resolveGlobals( this );
			control->Lexer =  lang->lexerKey ;
			if(s->HasForegroundColor)
				control->StyleSetFore( key , s->ForegroundColor );
			if(s->HasBackgroundColor )
				control->StyleSetBack( key , s->BackgroundColor );
			if( s->HasFontName )
				control->StyleSetFont( key , bytebufferToString(  s->FontName  ) );
			if( s->HasFontSize )
				control->StyleSetSize( key , s->FontSize );
			if( s->HasBold )
				control->StyleSetBold( key , s->IsBold );
			if( s->HasItalics )
				control->StyleSetItalic( key , s->IsItalics );
		}

		iter = lang->useKeywords->GetEnumerator();
		while( iter->MoveNext() )
		{
			bytebuffer  tmp = GetKeywordClass(dynamic_cast<String*>(iter->Value));
			int key = Int32::Parse(dynamic_cast<String*>(iter->Key));
			

			// hack it back to a string!
			pinnedbytepointer ptr = &tmp[0];
			String *s = new String( (const char*)ptr);
			
			control->KeyWords[key] = s;
		}
	}

	void LexerConfiguration::AddKeywordClass(String* name, bytebuffer text )
	{
		keywordclasses->set_Item( name , text );
	}

	void LexerConfiguration::AddGlobal(String* name, String* value )
	{
		globals->set_Item( name , value );
	}
	void LexerConfiguration::AddStyleClass( String* name , String* inherits,  String* fore, String* back , String* size, String* font, String* bold , String* italics )
	{
		Style* s = new Style();
		s->classname = name;
		s->background = back;
		s->foreground = fore;
		s->italics = italics;
		s->bold = bold;
		s->size = size;
		s->font = font;
		s->inheritstyle = inherits;

		styleclasses->set_Item( name , s );
	}

	Language* LexerConfiguration::AddLanguage( String* name )
	{
		Language* result = new Language();
		result->name = name;
		languages->set_Item( name , result );
		return result;
	}
	void LexerConfiguration::AddKeywords(Language* language,  String* key , String* keywordclass )
	{
		language->useKeywords->set_Item( key , keywordclass );
	}
	void LexerConfiguration::AddStyle( Language* language, String* key , String* name , String* inherits, String* fore, String* back , String* size, String* font, String* bold , String* italics  )
	{
		Style* s = new Style();
		s->key = Int32::Parse( key );
		s->classname = name;
		s->background = back;
		s->foreground = fore;
		s->italics = italics;
		s->bold = bold;
		s->size = size;
		s->font = font;
		s->inheritstyle = inherits;

		language->useStyles->set_Item( key , s );
	}

	// Retrieving Settings
	String* LexerConfiguration::GetGlobal( String* name)
	{
		return GetGlobal( name, name );
	}
	String* LexerConfiguration::GetGlobal( String* name, String* toplevelkey)
	{
		if( globals->ContainsKey( name ) )
		{
			String* result = globals->get_Item( name )->ToString();
			if( result->Equals( toplevelkey ) || !globals->ContainsKey( result ))
				return result; // stop before going in circles.
			else
				return GetGlobal( result , toplevelkey );
		}
		return name;
	}

	bytebuffer LexerConfiguration::GetKeywordClass( String* kclass )
	{
		if( keywordclasses->ContainsKey( kclass ) )
			return (bytebuffer)keywordclasses->get_Item( kclass );
		return new System::Byte[0];
	}

	Style* LexerConfiguration::GetStyleClass( String* classname )
	{
		Style* result = new Style();
		result->classname = classname->Concat( new String( "GETSTYLECLASS" ));

		if( styleclasses->ContainsKey( classname ) )
		{
			Style* temp = dynamic_cast<Style*>(styleclasses->get_Item( classname ));
			result->inherit( temp );
			
			if( temp->inheritstyle && !temp->inheritstyle->Equals(String::Empty) )
				result->inherit( GetStyleClass( temp->inheritstyle ) );
		}
		return result;
	}

	Language* LexerConfiguration::GetLanguage( String* languagename )
	{
		if( languages->ContainsKey( languagename  ) )
			return dynamic_cast<Language*> (languages->get_Item( languagename ));
		return null;
	}

};
