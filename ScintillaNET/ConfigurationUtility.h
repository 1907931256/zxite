#include "stdafx.h"
#define null 0L
#pragma once

using namespace System;
using namespace System::Xml;
using namespace System::Xml::Serialization;
using namespace System::IO;

#include "ConfigClasses.h"
#include "ScintillaNET.h"

typedef  System::Byte bytebuffer __gc[];

namespace Scintilla
{ 
	namespace Configuration
	{
		public __gc class ConfigurationUtility
		{

		protected: 
			/**
			*	This is the assembly that the application is loading the data from.
			*/
			System::Reflection::Assembly* _assembly;

			/**
			*	Loads a file from the local application as an embedded resource, or the local filesystem.,
			*	Local filesystem has precedence.
			**/
			virtual bytebuffer LoadFile( String* filename,ConfigFile* parent );
			
			/**
			*	Opens a file from the local application as an embedded resource, or the local filesystem.,
			*	Local filesystem has precedence.
			**/
			virtual Stream* OpenFile( String* filename,ConfigFile* parent );

			Object* Deserialize( TextReader* reader , System::Type* aType  );

		public:
			ConfigurationUtility(System::Reflection::Assembly* assembly);

			/**
			*	Loads the configuration from the embedded resources and the filesystem.
			**/
			virtual Object* LoadConfiguration(System::Type* configType, String* filename );

			/**
			*	Loads the configuration from the embedded resources and the filesystem.
			*   and "ScintillaNet.xml" for a filename
			**/
			virtual Object* LoadConfiguration(System::Type* configType);

			/**
			*	Loads the configuration from the embedded resources and the filesystem.
			*   uses Scintilla::Configuration::Scintilla for a Type
			**/
			virtual Object* LoadConfiguration(String* filename );

			/**
			*	Loads the configuration from the embedded resources and the filesystem.
			*   uses Scintilla::Configuration::Scintilla for a Type
			*   and "ScintillaNet.xml" for a filename
			**/
			virtual Object* LoadConfiguration();


			/**
			*	Loads the configuration from the embedded resources and the filesystem.
			**/
			virtual Object* LoadConfiguration(System::Type* configType, String* filename, ConfigFile* parent);

			/**
			*	Loads the configuration from the embedded resources and the filesystem.
			*   and "ScintillaNet.xml" for a filename
			**/
			virtual Object* LoadConfiguration(System::Type* configType, ConfigFile* parent);

			/**
			*	Loads the configuration from the embedded resources and the filesystem.
			*   uses Scintilla::Configuration::Scintilla for a Type
			**/
			virtual Object* LoadConfiguration(String* filename ,ConfigFile* parent);

			/**
			*	Loads the configuration from the embedded resources and the filesystem.
			*   uses Scintilla::Configuration::Scintilla for a Type
			*   and "ScintillaNet.xml" for a filename
			**/
			virtual Object* LoadConfiguration(ConfigFile* parent);


			/** 
			*   Uses the configuration data to set the styling information in a ScintillaControl.
			**/
			// virtual void ApplyConfiguration(ScintillaControl* scintillaControl, String* languageName );

		};
	}

}