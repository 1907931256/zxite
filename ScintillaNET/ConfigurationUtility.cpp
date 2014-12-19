#include "ConfigurationUtility.h"

using namespace System::Xml::Serialization;
using namespace System::IO;

namespace Scintilla
{ 
	namespace Configuration
	{
			ConfigurationUtility::ConfigurationUtility(System::Reflection::Assembly* assembly)
			{
				_assembly = assembly;
			}

			Object* ConfigurationUtility::Deserialize( TextReader* reader , System::Type* aType  )
			{
				System::Xml::Serialization::XmlSerializer* myDeSerializer = new System::Xml::Serialization::XmlSerializer(aType);
				Object* result = myDeSerializer->Deserialize(reader);
				reader->Close();
				return result;
			}

			Object*  ConfigurationUtility::LoadConfiguration(String* filename , ConfigFile* parent )
			{
				return LoadConfiguration( __typeof(Scintilla) , filename , parent);
			}

			Object*  ConfigurationUtility::LoadConfiguration(System::Type* configType ,ConfigFile* parent)
			{
				return LoadConfiguration( configType , "ScintillaNET.xml" , parent );
			}

			Object*  ConfigurationUtility::LoadConfiguration( ConfigFile* parent)
			{
				return LoadConfiguration( __typeof(Scintilla) , "ScintillaNET.xml" , parent );
			}

			Object*  ConfigurationUtility::LoadConfiguration(System::Type* configType, String* filename , ConfigFile* parent )
			{
				ConfigFile* result = NULL;
				if( __typeof( ConfigFile )->IsAssignableFrom( configType) )
				{
					Stream* res = OpenFile(filename, parent );
					if( res )
					{
						TextReader* reader = new StreamReader(res);
						result = dynamic_cast<ConfigFile*>(Deserialize( reader , configType ));
						result->filename = filename;
						result->init( this , parent  );
					}
				}
				return result;
			}


			Object*  ConfigurationUtility::LoadConfiguration(String* filename )
			{
				return LoadConfiguration( __typeof(Scintilla) , filename, NULL );
			}

			Object*  ConfigurationUtility::LoadConfiguration(System::Type* configType )
			{
				return LoadConfiguration( configType , "ScintillaNET.xml", NULL );
			}

			Object*  ConfigurationUtility::LoadConfiguration()
			{
				return LoadConfiguration( __typeof(Scintilla) , "ScintillaNET.xml", NULL );
			}

			Object*  ConfigurationUtility::LoadConfiguration(System::Type* configType, String* filename )
			{
				return LoadConfiguration( configType , filename, NULL );
			}

			bytebuffer ConfigurationUtility::LoadFile( String* filename, ConfigFile* parent   )
			{
				System::IO::Stream* res;
				bytebuffer buf;
				// check for local file.
				res = OpenFile( filename, parent );
				if( res != null)
				{
					buf = new Byte[res->Length];
					res->Read( buf , 0 , buf->Length );
					return buf;
				}
				return null;
			}

			Stream* ConfigurationUtility::OpenFile( String* filename, ConfigFile* parent )
			{
				System::IO::Stream* res;
				// check for local file.
				if( System::IO::File::Exists( filename ) )
				{
					res = new System::IO::FileStream( filename , System::IO::FileMode::Open );
				}
				else
				{
					// filename = String::Format( "{0}.{1}" , _assembly->GetName()->Name, filename->Replace("\\" , "." ) );
					res = _assembly->GetManifestResourceStream(String::Format( "{0}.{1}" , _assembly->GetName()->Name, filename->Replace("\\" , "." ) ));
				}
				if( !res && parent && parent->filename )
				{
					// try by prepending the path from the parent to the path.
					int p = parent->filename->LastIndexOf('\\');
					if( p > 0 )
						return OpenFile( String::Format( "{0}\\{1}" , parent->filename->Substring(0 , p) , filename ) , NULL );
				}

				return res;
			}
	}
}