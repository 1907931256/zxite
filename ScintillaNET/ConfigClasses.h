#include "stdafx.h"
#define null 0L
#pragma once
using namespace System;
using namespace System::Xml;
using namespace System::Xml::Serialization;
using namespace System::IO;

#define TO_COLORREF( c ) (((c & 0xff0000) >> 16) + ((c & 0x0000ff) << 16) + (c & 0x00ff00) )

namespace Scintilla
{ 
	namespace Configuration
	{
		public __gc class Scintilla;
		public __gc class ConfigurationUtility;
		public __gc class ConfigFile;

		[Serializable]
		public __gc class ConfigItem
		{
			protected:
				[NonSerialized]
				ConfigFile* _parent;

			public:
				virtual void init(ConfigurationUtility* utility, ConfigFile* theParent);
		};

		[Serializable]
		public __gc struct include : public ConfigItem
		{
			public:
				[XmlAttributeAttribute]
				String *file;
		};


		[Serializable]
		public __gc class ConfigFile : public ConfigItem
		{
			protected:
				[NonSerialized]
				ConfigFile* includedFiles[];

			public:
				[XmlArray("includes"),XmlArrayItem("include")]
				include* includes[];

				[NonSerialized]
				String* filename;

				virtual void addIncludedFile( ConfigFile* file );

				virtual void init(ConfigurationUtility* utility, ConfigFile* theParent);

				virtual void CollectScintillaNodes(System::Collections::ArrayList* list);

				__property virtual Scintilla* get_MasterScintilla();

			protected:
				__property virtual Scintilla* get_ChildScintilla();
		};


		[Serializable]
		public __gc struct Value : public ConfigItem
		{
			public:
				[XmlAttributeAttribute]
				String *name;

				[XmlText]
				String *val;
		};

		[Serializable]
		public __gc class UseKeyword : public ConfigItem
		{
			public:
				[XmlAttributeAttribute]
				int key;

				[XmlAttributeAttribute("class")]
				String* cls;
		};


		[Serializable]
		public __gc class Lexer : public ConfigItem
		{
			public:
				[XmlAttributeAttribute]
				int key;

				[XmlAttributeAttribute("name")]
				String* name;
		};

		[Serializable]
		public __gc class StyleClass : public ConfigItem
		{
			public:
				[XmlAttributeAttribute]
				int key;

				[XmlAttributeAttribute]
				String* name;

				[XmlAttributeAttribute]
				String* fore;

				[XmlAttributeAttribute]
				String* back;

				[XmlAttributeAttribute]
				String* size;
				
				[XmlAttributeAttribute]
				String* font;

				[XmlAttributeAttribute]
				String* bold;

				[XmlAttributeAttribute]
				String* italics;

				[XmlAttributeAttribute("inherit-style")]
				String* inheritstyle;

				int ResolveColor( String* color );
				int ResolveNumber( String* number );
				String* ResolveString( String* number );
			public:
				__property bool get_IsBold();
				__property bool get_IsItalics();
				
				__property int get_ForegroundColor();
				__property int get_BackgroundColor();
				__property int get_FontSize();
				__property String* get_FontName();

				__property bool get_HasForegroundColor();
				__property bool get_HasBackgroundColor();
				__property bool get_HasFontSize();
				__property bool get_HasFontName();
				__property bool get_HasBold();
				__property bool get_HasItalics();
				__property StyleClass* get_ParentClass();

		};

		[Serializable]
		public __gc class UseStyle : public StyleClass
		{
			public:
				[XmlAttributeAttribute("class")]
				String* cls;

			public:
				virtual void init(ConfigurationUtility* utility, ConfigFile* theParent);

		};

		[Serializable]
		public __gc class KeywordClass : public ConfigItem
		{
			public:
				[XmlAttributeAttribute]
				String* name;

				[XmlText]
				String* val;

		};

		[Serializable]
		public __gc class Language : public ConfigItem
		{
			public:
				[XmlAttributeAttribute]
				String* name;

				Lexer* lexer;

				[XmlElement(ElementName = "file-extensions")]
				String* fileextensions;

				[XmlArray("use-keywords"), XmlArrayItem("keyword")]
				UseKeyword* usekeywords[];

				[XmlArray("use-styles"), XmlArrayItem("style")]
				UseStyle* usestyles[];

			public:
				virtual void init(ConfigurationUtility* utility, ConfigFile* theParent);

		};
		[Serializable]
		public __gc class Scintilla : public ConfigFile 
		{
			public:
				[XmlArray("globals"),XmlArrayItem("value")]
				Value* globals[];

				[XmlArray("style-classes"),XmlArrayItem("style-class")]
				StyleClass* styleclasses[];

				[XmlArray("keyword-classes"),XmlArrayItem("keyword-class")]
				KeywordClass* keywordclass[];

				[XmlArray("languages"),XmlArrayItem("language")]
				Language* languages[];	

				Value* GetValue(String* keyName );
				StyleClass* GetStyleClass(String* styleName);
				KeywordClass* GetKeywordClass(String* keywordName);
				Language* GetLanguage(String* languageName);

			public:
				virtual void init(ConfigurationUtility* utility, ConfigFile* theParent);

			protected:
				__property virtual Scintilla* get_ChildScintilla();

		};
	}
}