using System;
using System.IO;
using System.Windows.Forms;

namespace SciteNet
{
	public class FileStore : IFileStore
	{
		public FileStore()
		{
		}

		public byte[] LoadFile( string fullpath )
		{
			long n = GetFileSize( fullpath );
			if( n > 0 )
			{
				byte[] buf = new byte[n];
				FileStream infile = File.OpenRead( fullpath );
				n = infile.Read( buf , 0 , buf.Length );
				infile.Close();
				return buf;
			}
			return null;
		}

		public long GetFileSize( string fullpath )
		{
			if( File.Exists( fullpath ) )
			{
				FileInfo fileinfo = new FileInfo(fullpath);
				return fileinfo.Length;
			}
			return 0;
		}

		public string GetFullPath( string fullpath )
		{
			return null;
		}

		public void SaveFile( string fullpath , byte[] filedata )
		{
			File.Delete( fullpath );
			FileStream outfile = File.OpenWrite( fullpath );
			outfile.Write( filedata , 0 , filedata.Length );
			outfile.Close();
		}

		public FileDialog OpenFileDialog( )
		{
			return new System.Windows.Forms.OpenFileDialog();
		}
		public FileDialog SaveFileDialog( )
		{
			return new System.Windows.Forms.SaveFileDialog();
		}

	}
}
