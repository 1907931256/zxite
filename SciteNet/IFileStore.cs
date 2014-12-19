using System;
using System.Windows.Forms;

namespace SciteNet
{
	/// <summary>
	/// Summary description for IFileStore.
	/// </summary>
	public interface IFileStore
	{
		byte[] LoadFile( string fullpath );
		void SaveFile( string fullpath , byte[] filedata );
		
		long GetFileSize( string fullpath );
		string GetFullPath( string fullpath );

		FileDialog OpenFileDialog( );
		FileDialog SaveFileDialog( );

	}
}
