using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;
using Scintilla;
using System.IO;

namespace SciteNet
{
	/// <summary>
	/// Summary description for FileTabPage.
	/// </summary>
	public class FileTabPage : System.Windows.Forms.TabPage
	{
		public ScintillaControl scintillaControl;
		public string fullpath="";
		private FileManager fileManager;

		public FileTabPage(string Filename, FileManager filemanager )
		{
			//
			// TODO: Add constructor logic here
			//
			scintillaControl = new ScintillaControl();

			this.Controls.AddRange(new System.Windows.Forms.Control[] {this.scintillaControl } );

			scintillaControl.Location = new System.Drawing.Point(0, 0);
			scintillaControl.Size = Size;
			scintillaControl.Name = Filename;
			scintillaControl.TabIndex = 0;
			scintillaControl.TabStop = true;
			scintillaControl.Anchor = (((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
				| System.Windows.Forms.AnchorStyles.Left) 
				| System.Windows.Forms.AnchorStyles.Right);

			scintillaControl.SavePointLeft += new Scintilla.SavePointLeftHandler(this.SavePointLeft);
			scintillaControl.SavePointReached += new Scintilla.SavePointReachedHandler(this.SavePointReached);

			fileManager = filemanager;

			scintillaControl.Lexer =  ScintillaControl.LEX_CPP  ;
			for( int i= 0; i< 19 ; i++ )
			{
				scintillaControl.StyleSetBack( i , 0xFFFFFF  );
				scintillaControl.StyleSetFore( i , 0x000000);
				scintillaControl.StyleSetSize( i , 10 );
				scintillaControl.StyleSetFont( i , "Lucida Console" );
			}
			GotFocus+= new System.EventHandler(this.FocusReceived );
			
		}

		public void FocusReceived( Object source, System.EventArgs e )
		{
			if( scintillaControl != null )
				scintillaControl.focus = true;
		}

		public Form GetParentForm()
		{
			Control p = Parent;
			while( p.Parent != null )
				p = p.Parent;

			if( p.GetType().IsSubclassOf( typeof(Form) ) )
				return (System.Windows.Forms.Form)p;

			return null;
		}

		public void Init()
		{
			scintillaControl.Size = Size;
			reload();

			Form p = GetParentForm();
			scintillaControl.AddShortcutsFromForm( p );
		}

		public void reload()
		{
			byte[] buf = fileManager.fileStore.LoadFile( fullpath );
			if( buf != null )
			{
				scintillaControl.ClearAll();
				//scintillaControl.AddText( buf.Length  , buf );
				scintillaControl.SetSavePoint();
				SavePointReached( scintillaControl );
			}
		}

		public void save()
		{
			if( fullpath == "" )
				saveAs();
			else
			{
				byte[] buf = stringtobytes( scintillaControl.AllText );
				fileManager.fileStore.SaveFile( fullpath , buf );
				scintillaControl.SetSavePoint();
			}
		}
		
		public void saveAs( )
		{
			fullpath = fileManager.saveFileAs();
			if( fullpath != "" )
			{
				save();
			}
		}

	
		private int SavePointLeft(ScintillaControl sender)
		{
			if( fullpath == "" )
				Text = "Untitled*";
			else
				Text= fullpath+"*";
			return 0;
		}
		private int SavePointReached(ScintillaControl sender)
		{
			if( fullpath == "" )
				Text = "Untitled";
			else
				Text= fullpath;
			return 0;
		}

		private byte[] stringtobytes( string txt )
		{
			return System.Text.Encoding.ASCII.GetBytes(txt );
		}
	}
}
