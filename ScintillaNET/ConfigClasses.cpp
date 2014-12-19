#include "ConfigClasses.h"
#include "ConfigurationUtility.h"

#define TO_COLORREF( c ) (((c & 0xff0000) >> 16) + ((c & 0x0000ff) << 16) + (c & 0x00ff00) )

namespace Scintilla
{ 
	namespace Configuration
	{

		/// CONFIG ITEM --------------------------------------------------------------------------------
		void ConfigItem::init(ConfigurationUtility* utility, ConfigFile* theParent)
		{
			if( theParent == NULL )
				_parent = __try_cast<ConfigFile*>(this);
			else
				_parent = theParent;
		}


		/// CONFIG FILE --------------------------------------------------------------------------------
		void ConfigFile::init(ConfigurationUtility* utility, ConfigFile* theParent)
		{
			if( filename && theParent )
				System::Console::Out->WriteLine("Starting filename={0} , theParent={1}, Type={2}", filename , theParent->filename , GetType()->FullName );
			else if ( filename )
				System::Console::Out->WriteLine("Starting filename={0} , theParent=(NULL), Type={1}", filename , GetType()->FullName );
			else if ( theParent )
				System::Console::Out->WriteLine("Starting filename=(NULL) , theParent={0}, Type={1}", theParent->filename , GetType()->FullName );
				
			includedFiles = new ConfigFile*[0];
			ConfigItem::init( utility, theParent );

			if( includes == NULL )
				includes = new include*[0];

			for( int i=0; i< includes->Length; i++)
				includes[i]->init(utility,_parent);

			for( int i = 0; i< includes->Length ; i++ )
			{
				ConfigFile* inc;
				inc = __try_cast<ConfigFile*>(utility->LoadConfiguration( this->GetType(), includes[i]->file, __try_cast<ConfigFile*>(_parent) ));
				addIncludedFile( inc ); 
			}

			if( filename && theParent )
				System::Console::Out->WriteLine("Ending filename={0} , theParent={1}", filename , theParent->filename );
			else if ( filename )
				System::Console::Out->WriteLine("Ending filename={0} , theParent=(NULL)", filename );
			else if ( theParent )
				System::Console::Out->WriteLine("Ending filename=(NULL) , theParent={0}", theParent->filename );
		}

		void ConfigFile::addIncludedFile( ConfigFile* file )
		{
			ConfigFile* new_array[] = new ConfigFile* [includedFiles->Length+1];
			includedFiles->CopyTo( new_array, 0);
			new_array[includedFiles->Length] = file;
			includedFiles = new_array;
		}

		Scintilla* ConfigFile::get_MasterScintilla()
		{	
			// am I talking to the parent?
			if( _parent == this )
			{
				//if( GetType()->Equals( __typeof( Scintilla ) ) )
				//	return __try_cast<Scintilla*>(this);
				//else
				return ChildScintilla; 
			}
			else // talk to the parent.
				return _parent->MasterScintilla;
		}

		Scintilla* ConfigFile::get_ChildScintilla()
		{	
			return NULL;
		}

		// builds a master list of scintilla objects for the parent node.
		// This allows us to rebuild the list at any time (say when a file is refreshed)
		void ConfigFile::CollectScintillaNodes(System::Collections::ArrayList* list)
		{
			if( _parent == this )
			{
				list = new System::Collections::ArrayList();
				// add my children's includes first!
				if( ChildScintilla != NULL )
					ChildScintilla->CollectScintillaNodes( list );

			}
			else if( list == NULL )
				return;

			ConfigFile* cf;
			for( int i = 0 ; i< includedFiles->Length ; i++ )
			{
				cf = includedFiles[i];
				// add the included file's scintilla to the list.
				if( cf->ChildScintilla != NULL )
				{
					list->Add( cf->ChildScintilla );


					if(cf->ChildScintilla->filename && cf->ChildScintilla->_parent)
						System::Console::Out->WriteLine("Adding filename={0} , theParent={1}, Type={2}", cf->ChildScintilla->filename , cf->ChildScintilla->_parent->filename , cf->ChildScintilla->GetType()->FullName );
					else if ( cf->ChildScintilla->filename )
						System::Console::Out->WriteLine("Adding filename={0} , theParent=(NULL), Type={1}", cf->ChildScintilla->filename , cf->ChildScintilla->GetType()->FullName );
					else if ( cf->ChildScintilla->_parent )
						System::Console::Out->WriteLine("Adding filename=(NULL) , theParent={0}, Type={1}", cf->ChildScintilla->_parent->filename , cf->ChildScintilla->GetType()->FullName );

				}

				// add the child scintilla's children to the list 
				if( cf->ChildScintilla != NULL )
					cf->ChildScintilla->CollectScintillaNodes( list );

				// add the included file's included
				if( cf->includedFiles != NULL && cf->includedFiles->Length > 0 )
					cf->CollectScintillaNodes(list);

			}

			if( _parent == this )
			{
				ChildScintilla->includedFiles = new ConfigFile* [list->Count];
				list->CopyTo( ChildScintilla->includedFiles );
			}
		}


		/// SCINTILLA CONFIG --------------------------------------------------------------------------------
		Scintilla* Scintilla::get_ChildScintilla()
		{	
			return this;
		}

		void Scintilla::init(ConfigurationUtility* utility, ConfigFile* theParent)
		{
			ConfigFile::init( utility, theParent );

			if( languages == NULL )
				languages = new Language*[0];

			if( styleclasses == NULL )
				styleclasses = new StyleClass*[0];

			if( keywordclass == NULL )
				keywordclass = new KeywordClass*[0];

			if( globals == NULL )
				globals = new Value*[0];

			for( int i=0; i< languages->Length; i++)
				languages[i]->init(utility,_parent);

			for( int i=0; i< styleclasses->Length; i++)
				styleclasses[i]->init(utility,_parent);

			for( int i=0; i< keywordclass->Length; i++)
				keywordclass[i]->init(utility,_parent);

			for( int i=0; i< globals->Length; i++)
				globals[i]->init(utility,_parent);

		}

		Value* Scintilla::GetValue(String* keyName )
		{
			Value* result = NULL;
			if( MasterScintilla == this )
			{
				// check the children first (from the end)
				for( int i = includedFiles->Length-1 ; i > -1; i-- )
				{
					Scintilla* child = __try_cast<Scintilla*>(includedFiles[i]);
					result = child->GetValue(keyName);
					if( result != NULL )
						return result;
				}
			}
			//other wise just check here.
			for( int i = globals->Length -1 ;  i > -1 ; i-- )
			{
				if( globals[i]->name->Equals( keyName ) )
					result = globals[i];
			}
			
			return result;	
		}

		StyleClass* Scintilla::GetStyleClass(String* styleName)
		{
			StyleClass* result = NULL;
			if( MasterScintilla == this )
			{
				// check the children first (from the end)
				for( int i = includedFiles->Length-1 ; i > -1; i-- )
				{
					Scintilla* child = __try_cast<Scintilla*>(includedFiles[i]);
					result = child->GetStyleClass(styleName);
					if( result != NULL )
						return result;
				}
			}
			//other wise just check here.
			for( int i = styleclasses->Length -1 ;  i > -1 ; i-- )
			{
				if( styleclasses[i]->name->Equals( styleName ) )
					result = styleclasses[i];
			}
			
			return result;	
		}
		KeywordClass* Scintilla::GetKeywordClass(String* keywordName)
		{
			KeywordClass* result = NULL;
			if( MasterScintilla == this )
			{
				// check the children first (from the end)
				for( int i = includedFiles->Length-1 ; i > -1; i-- )
				{
					Scintilla* child = __try_cast<Scintilla*>(includedFiles[i]);
					result = child->GetKeywordClass(keywordName);
					if( result != NULL )
						return result;
				}
			}
			//other wise just check here.
			for( int i = keywordclass->Length -1 ;  i > -1 ; i-- )
			{
				if( keywordclass[i]->name->Equals( keywordName ) )
					result = keywordclass[i];
			}
			
			return result;	
		}
		Language* Scintilla::GetLanguage(String* languageName)
		{
			Language* result = NULL;
			if( MasterScintilla == this )
			{
				// check the children first (from the end)
				for( int i = includedFiles->Length-1 ; i > -1; i-- )
				{
					Scintilla* child = __try_cast<Scintilla*>(includedFiles[i]);
					result = child->GetLanguage(languageName);
					if( result != NULL )
						return result;
				}
			}
			//other wise just check here.
			for( int i = languages->Length -1 ;  i > -1 ; i-- )
			{
				if( languages[i]->name->Equals( languageName ) )
					result = languages[i];
			}
			
			return result;	
		}



		/// LANGUAGE --------------------------------------------------------------------------------
		void Language::init(ConfigurationUtility* utility, ConfigFile* theParent)
		{
			ConfigItem::init( utility,theParent );

			if( usekeywords == NULL )
				usekeywords = new UseKeyword*[0];
			if( usestyles == NULL )
				usestyles = new UseStyle*[0];


			// do Nothing;
			for( int i=0; i< usekeywords->Length; i++)
				usekeywords[i]->init(utility,_parent);

			for( int i=0; i< usestyles->Length; i++)
				usestyles[i]->init(utility,_parent);

			if( lexer )
				lexer->init( utility, _parent );
		}


		/// STYLE CLASS --------------------------------------------------------------------------------

		StyleClass* StyleClass::get_ParentClass()
		{
			if( inheritstyle != NULL && !inheritstyle->Equals("") )
			{
				return _parent->MasterScintilla->GetStyleClass( inheritstyle );
			}
			// if it has no parent, the default class will be the parent.
			// caution: it is not programmatically guaranteed that there is a default.
			if( !name->Equals("default") )
				return _parent->MasterScintilla->GetStyleClass( "default" );
			return NULL;
		}

		bool StyleClass::get_IsBold()
		{
			if( bold )
				return bold->Equals("true");
			
			StyleClass* p = ParentClass;
			if( p ) 
				return p->IsBold;
			return false;

		}
		bool StyleClass::get_IsItalics()
		{
			if( italics )
				return italics->Equals( "true");
			
			StyleClass* p = ParentClass;
			if( p ) 
				return p->IsItalics;
			return false;
		}

		int StyleClass::ResolveColor( String* aColor )
		{
			if( aColor )
			{
				Value* v =  _parent->MasterScintilla->GetValue(aColor);
				while( v )
				{
					aColor = v->val;
					v = _parent->MasterScintilla->GetValue(aColor);
				}
				// first try known-color string
				System::Drawing::Color c = System::Drawing::Color::FromName( aColor );
				if( c.ToArgb() == 0 )
				{
					// didn't find, or is black.
					//hex?
					if( aColor->IndexOf( "0x" ) == 0 )
						return TO_COLORREF(Int32::Parse( aColor->Substring( 2 ) , System::Globalization::NumberStyles::HexNumber ));
					else //decimal
					try
					{
						return TO_COLORREF(Int32::Parse( aColor ));
					}
					catch( ... )
					{
					}
				}
				return TO_COLORREF( c.ToArgb() & 0x00ffffff );
			}
			return 0;
		}

		int StyleClass::ResolveNumber( String* number )
		{
			if( number )
			{
				Value* v =  _parent->MasterScintilla->GetValue(number);
				while( v )
				{
					number = v->val;
					v = _parent->MasterScintilla->GetValue(number);
				}
				//hex?
				if( number->IndexOf( "0x" ) == 0 )
					return Int32::Parse( number->Substring( 2 ) , System::Globalization::NumberStyles::HexNumber );
				else //decimal
				try
				{
					return Int32::Parse( number );
				}
				catch( ... )
				{
				}
			}
			return 0;
		}

		String*  StyleClass::ResolveString( String* aString )
		{
			if( aString )
			{
				Value* v =  _parent->MasterScintilla->GetValue(aString );
				while( v )
				{
					aString = v->val;
					v = _parent->MasterScintilla->GetValue(aString);
				}
			}
			return aString;
		}

		int StyleClass::get_ForegroundColor()
		{
			if( fore )
				return ResolveColor( fore );

			StyleClass* p = ParentClass;
			if( p ) 
				return p->ForegroundColor;
			return 0;
		}
		int StyleClass::get_BackgroundColor()
		{
			if( back )
				return ResolveColor( back );

			StyleClass* p = ParentClass;
			if( p ) 
				return p->BackgroundColor;
			return 0;
		}
		int StyleClass::get_FontSize()
		{
			if( size )
				return ResolveNumber( size );

			StyleClass* p = ParentClass;
			if( p ) 
				return p->FontSize;
			return 10;  // default 10 pt?
		}
		String* StyleClass::get_FontName()
		{
			if( font )
				return ResolveString( font );

			StyleClass* p = ParentClass;
			if( p ) 
				return p->FontName;
			return "courier";  // default 10 pt?
		}

		bool StyleClass::get_HasForegroundColor()
		{
			if( fore ) 
				return true;

			StyleClass* p = ParentClass;
			if( p ) 
				return p->HasForegroundColor;

			return false;
		}
		bool StyleClass::get_HasBackgroundColor()
		{
			if( back ) 
				return true;

			StyleClass* p = ParentClass;
			if( p ) 
				return p->HasBackgroundColor;

			return false;
		}
		bool StyleClass::get_HasFontSize()
		{
			if( size ) 
				return true;

			StyleClass* p = ParentClass;
			if( p ) 
				return p->HasFontSize;

			return false;
		}
		bool StyleClass::get_HasFontName()
		{
			if( font ) 
				return true;

			StyleClass* p = ParentClass;
			if( p ) 
				return p->HasFontName;

			return false;
		}
		bool StyleClass::get_HasBold()
		{
			if( bold ) 
				return true;

			StyleClass* p = ParentClass;
			if( p ) 
				return p->HasBold;

			return false;
		}
		bool StyleClass::get_HasItalics()
		{
			if( italics ) 
				return true;

			StyleClass* p = ParentClass;
			if( p ) 
				return p->HasItalics;

			return false;
		}

		//USE STYLE -------------------------------------------------------------------------------
		void UseStyle::init(ConfigurationUtility* utility, ConfigFile* theParent)
		{
			ConfigItem::init( utility,theParent );
			if( cls && cls->Length > 0 )
				inheritstyle = cls;
		}

	}
}