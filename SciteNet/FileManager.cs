using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;
using Scintilla;

namespace SciteNet
{
	/// <summary>
	/// Summary description for FileManager.
	/// </summary>
	public class FileManager
	{
		private ArrayList tabPages = new ArrayList();
		private System.Windows.Forms.TabControl container;
		private LexerConfiguration config;

		public IFileStore fileStore;

		public FileManager(System.Windows.Forms.TabControl Container, IFileStore filestore)
		{
			container = Container;
			fileStore = filestore;

			System.IO.Stream res = GetType().Module.Assembly.GetManifestResourceStream("SciteNet.ScintillaNET.xml");
			if( res != null)
			{
			byte[] buf = new Byte[ res.Length];
			res.Read( buf , 0 , buf.Length );
			config = new LexerConfiguration( buf );
		}
		}

		public void addFile( String filename )
		{
			//either a full-path or a relative path.
			FileTabPage tabPage = new FileTabPage( makeFullPath( filename  ) , this );
			tabPage.SuspendLayout();

			container.Controls.AddRange(new System.Windows.Forms.Control[] {tabPage});

			tabPage.Location = new System.Drawing.Point(0, 0);
			tabPage.Text = "Untitled";
			tabPage.Size = container.Size;
			tabPage.TabIndex = 0;

			tabPages.Add( tabPage );

			tabPage.ResumeLayout(false);
			tabPage.fullpath = makeFullPath( filename );
			tabPage.Init();
			if( config != null )
			config.ApplyConfiguration( tabPage.scintillaControl , "javascript");
			container.SelectedTab = tabPage;

			if( SelectedFileTab != null )
				SelectedFileTab.Focus();
			else
				container.Focus();
		}
	
		public FileTabPage findFileTabPage( string filename )
		{
			string fn = makeFullPath( filename  );

			foreach( FileTabPage ftp in tabPages )
				if( ftp.fullpath == fn )
					return ftp;
			return null;
		}

		public void requestCloseFile( string filename )
		{
			 
		}
		public void requestCloseFile( )
		{
			FileTabPage ftp = SelectedFileTab;
			if( ftp == null )
				return;
			if( ftp.scintillaControl.IsModified )
			{
				if( MessageBox.Show( "The file has been modified. Do you wish to save it?","Save File", MessageBoxButtons.YesNo ) == DialogResult.Yes  )
					ftp.save();
			}

			container.Controls.Remove( ftp );
			ftp.scintillaControl.Dispose();
			ftp.scintillaControl = null;
			ftp.Dispose();
			if( SelectedFileTab != null )
				SelectedFileTab.Focus();
			else
				container.Focus();
		}
		
		public void openFileViaDialog( )
		{
			FileDialog fd = fileStore.OpenFileDialog();
			fd.InitialDirectory = "c:\\" ;
			fd.Filter = "txt files (*.js)|*.js|All files (*.*)|*.*" ;
			fd.FilterIndex = 1 ;
			fd.RestoreDirectory = true ;

			if(fd.ShowDialog() == DialogResult.OK)
			{
				addFile( fd.FileName  );
			}
		}

		public string saveFileAs( )
		{
			FileDialog fd = fileStore.SaveFileDialog();
			fd.InitialDirectory = "c:\\" ;
			fd.Filter = "txt files (*.js)|*.js|All files (*.*)|*.*" ;
			fd.FilterIndex = 1 ;
			fd.RestoreDirectory = true ;

			if(fd.ShowDialog() == DialogResult.OK)
				return fd.FileName;

			return "";
		}


		public string makeFullPath( string Filename )
		{
			if( Filename == null || Filename == "" )
				return "";
			return new System.IO.FileInfo( Filename ).FullName;
		}

		public FileTabPage SelectedFileTab
		{
			get
			{
				return (FileTabPage)container.SelectedTab;
			}
		}
	}
}