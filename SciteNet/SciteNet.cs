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
	/// Summary description for Form1.
	/// </summary>
	public class frmSciteNet: System.Windows.Forms.Form
	{
		private System.Windows.Forms.MainMenu mainMenu1;
		private System.Windows.Forms.MenuItem menuItem1;
		private System.Windows.Forms.MenuItem menuItem2;
		private System.Windows.Forms.MenuItem menuItem3;
		private System.Windows.Forms.MenuItem menuItem4;
		private System.Windows.Forms.MenuItem menuItem5;
		private System.Windows.Forms.MenuItem menuItem6;
		private System.Windows.Forms.MenuItem menuItem7;
		private System.Windows.Forms.MenuItem menuItem8;
		private System.Windows.Forms.MenuItem menuItem9;
		private System.Windows.Forms.MenuItem menuItem10;
		private System.Windows.Forms.MenuItem menuItem11;
		private System.Windows.Forms.MenuItem menuItem12;
		private System.Windows.Forms.MenuItem menuItem13;
		private System.Windows.Forms.MenuItem menuItem14;
		private System.Windows.Forms.MenuItem menuItem15;
		private System.Windows.Forms.TabControl tabControl1;
		private FileManager filemanager;
		private System.Windows.Forms.MenuItem menuItem16;
		
		private System.Windows.Forms.MenuItem menuItem17;


		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;

		public frmSciteNet()
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();

			//
			// TODO: Add any constructor code after InitializeComponent call
			//
			filemanager =  new FileManager( tabControl1, new FileStore() );
		}

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		protected override void Dispose( bool disposing )
		{
			if( disposing )
			{
				if (components != null) 
				{
					components.Dispose();
				}
			}
			base.Dispose( disposing );
		}

		#region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			this.mainMenu1 = new System.Windows.Forms.MainMenu();
			this.menuItem1 = new System.Windows.Forms.MenuItem();
			this.menuItem5 = new System.Windows.Forms.MenuItem();
			this.menuItem6 = new System.Windows.Forms.MenuItem();
			this.menuItem17 = new System.Windows.Forms.MenuItem();
			this.menuItem16 = new System.Windows.Forms.MenuItem();
			this.menuItem7 = new System.Windows.Forms.MenuItem();
			this.menuItem4 = new System.Windows.Forms.MenuItem();
			this.menuItem8 = new System.Windows.Forms.MenuItem();
			this.menuItem2 = new System.Windows.Forms.MenuItem();
			this.menuItem9 = new System.Windows.Forms.MenuItem();
			this.menuItem10 = new System.Windows.Forms.MenuItem();
			this.menuItem15 = new System.Windows.Forms.MenuItem();
			this.menuItem11 = new System.Windows.Forms.MenuItem();
			this.menuItem12 = new System.Windows.Forms.MenuItem();
			this.menuItem13 = new System.Windows.Forms.MenuItem();
			this.menuItem3 = new System.Windows.Forms.MenuItem();
			this.menuItem14 = new System.Windows.Forms.MenuItem();
			this.tabControl1 = new System.Windows.Forms.TabControl();
			this.SuspendLayout();
			// 
			// mainMenu1
			// 
			this.mainMenu1.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
																					  this.menuItem1,
																					  this.menuItem2,
																					  this.menuItem3});
			// 
			// menuItem1
			// 
			this.menuItem1.Index = 0;
			this.menuItem1.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
																					  this.menuItem5,
																					  this.menuItem6,
																					  this.menuItem17,
																					  this.menuItem16,
																					  this.menuItem7,
																					  this.menuItem4,
																					  this.menuItem8});
			this.menuItem1.Text = "&File";
			// 
			// menuItem5
			// 
			this.menuItem5.Index = 0;
			this.menuItem5.Shortcut = System.Windows.Forms.Shortcut.CtrlN;
			this.menuItem5.Text = "New";
			this.menuItem5.Click += new System.EventHandler(this.menuItem5_Click);
			// 
			// menuItem6
			// 
			this.menuItem6.Index = 1;
			this.menuItem6.Shortcut = System.Windows.Forms.Shortcut.CtrlO;
			this.menuItem6.Text = "Open";
			this.menuItem6.Click += new System.EventHandler(this.menuItem6_Click);
			// 
			// menuItem17
			// 
			this.menuItem17.Index = 2;
			this.menuItem17.Shortcut = System.Windows.Forms.Shortcut.CtrlR;
			this.menuItem17.Text = "Reload";
			this.menuItem17.Click += new System.EventHandler(this.menuItem17_Click);
			// 
			// menuItem16
			// 
			this.menuItem16.Index = 3;
			this.menuItem16.Shortcut = System.Windows.Forms.Shortcut.CtrlS;
			this.menuItem16.Text = "Save";
			this.menuItem16.Click += new System.EventHandler(this.menuItem16_Click);
			// 
			// menuItem7
			// 
			this.menuItem7.Index = 4;
			this.menuItem7.Shortcut = System.Windows.Forms.Shortcut.CtrlF4;
			this.menuItem7.Text = "Close";
			this.menuItem7.Click += new System.EventHandler(this.menuItem7_Click);
			// 
			// menuItem4
			// 
			this.menuItem4.Index = 5;
			this.menuItem4.Text = "-";
			// 
			// menuItem8
			// 
			this.menuItem8.Index = 6;
			this.menuItem8.Text = "Exit";
			this.menuItem8.Click += new System.EventHandler(this.menuItem8_Click);
			// 
			// menuItem2
			// 
			this.menuItem2.Index = 1;
			this.menuItem2.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
																					  this.menuItem9,
																					  this.menuItem10,
																					  this.menuItem15,
																					  this.menuItem11,
																					  this.menuItem12,
																					  this.menuItem13});
			this.menuItem2.Text = "&Edit";
			// 
			// menuItem9
			// 
			this.menuItem9.Index = 0;
			this.menuItem9.Text = "Undo";
			// 
			// menuItem10
			// 
			this.menuItem10.Index = 1;
			this.menuItem10.Text = "Redo";
			// 
			// menuItem15
			// 
			this.menuItem15.Index = 2;
			this.menuItem15.Text = "-";
			// 
			// menuItem11
			// 
			this.menuItem11.Index = 3;
			this.menuItem11.Text = "Cut";
			// 
			// menuItem12
			// 
			this.menuItem12.Index = 4;
			this.menuItem12.Text = "Copy";
			// 
			// menuItem13
			// 
			this.menuItem13.Index = 5;
			this.menuItem13.Text = "Paste";
			// 
			// menuItem3
			// 
			this.menuItem3.Index = 2;
			this.menuItem3.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
																					  this.menuItem14});
			this.menuItem3.Text = "Help";
			// 
			// menuItem14
			// 
			this.menuItem14.Index = 0;
			this.menuItem14.Text = "About SciteNET";
			// 
			// tabControl1
			// 
			this.tabControl1.Anchor = (((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
				| System.Windows.Forms.AnchorStyles.Left) 
				| System.Windows.Forms.AnchorStyles.Right);
			this.tabControl1.Name = "tabControl1";
			this.tabControl1.SelectedIndex = 0;
			this.tabControl1.Size = new System.Drawing.Size(544, 424);
			this.tabControl1.TabIndex = 0;
			// 
			// frmSciteNet
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(544, 422);
			this.Controls.AddRange(new System.Windows.Forms.Control[] {
																		  this.tabControl1});
			this.Menu = this.mainMenu1;
			this.Name = "frmSciteNet";
			this.Text = "SciteNet";
			this.ResumeLayout(false);

		}
		#endregion

		[STAThread]
		static void Main() 
		{
			frmSciteNet app = new frmSciteNet();
			Application.Run(app);
		}

		private void menuItem8_Click(object sender, System.EventArgs e)
		{
			// exit.
			Close();
		}

		private void menuItem16_Click(object sender, System.EventArgs e)
		{
			((FileTabPage)tabControl1.SelectedTab).save();
		}

		private void menuItem17_Click(object sender, System.EventArgs e)
		{
			((FileTabPage)tabControl1.SelectedTab).reload();
		}

		private void menuItem6_Click(object sender, System.EventArgs e)
		{
			filemanager.openFileViaDialog();
		}

		private void menuItem5_Click(object sender, System.EventArgs e)
		{
			// new
			filemanager.addFile("");
		}

		private void menuItem7_Click(object sender, System.EventArgs e)
		{
			// close
			filemanager.requestCloseFile(  );
		}

		public void testSerialization()
		{
			Scintilla.Configuration.Scintilla x1;
			x1 = new Scintilla.Configuration.Scintilla();
			x1.globals = new Scintilla.Configuration.Value[2];
			x1.globals[0] = new Scintilla.Configuration.Value();
			x1.globals[1] = new Scintilla.Configuration.Value();
			x1.globals[0].name = "myname";
			x1.globals[0].val = "myval";
			x1.globals[1].name = "yourname";
			x1.globals[1].val = "yourval";


			System.Xml.Serialization.XmlSerializer myDeSerializer = new System.Xml.Serialization.XmlSerializer(typeof(Scintilla.Configuration.Scintilla));
			// To write to a file, create a StreamWriter object.
			System.IO.MemoryStream ms = new System.IO.MemoryStream(40000);

			System.IO.StreamWriter myWriter = new System.IO.StreamWriter( ms );
			myDeSerializer.Serialize(myWriter, x1);

			ms.Flush();
			string sq = System.Text.ASCIIEncoding.ASCII.GetString(ms.GetBuffer() , 0, (int)ms.Length );
			MessageBox.Show( sq  );


		}
	}
}
