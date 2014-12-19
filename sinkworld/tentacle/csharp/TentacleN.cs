using System;
using System.Diagnostics;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO;
/// <summary> TentacleN demonstration editor copyright 2004 Neil Hodgson.
/// </summary>
using SinkWorld;

class TentacleNApplication:Form, IDocumentChangeListener {

	const int blockSize = 32768;

	LiteralSet fontNames = new LiteralSet();
	Document pdoc = null;

	bool titleAtSavePoint = true;
	String fullPath = "";

	int panesVisible = 0;
	Panel panelHeader;
	FindPanel panelFind;
	ReplacePanel panelReplace;
	bool haveFound;
	Literal findWhat = new Literal("");
	Literal replaceWhat = new Literal("");
	const int controls = 2;
	int activeControl = 0;
	TentacleControl []tc = new TentacleControl[controls];

	MenuItem menuItemWrap;

	static int buttonWidth = 80;
	static int buttonHeight = 20;
	static int pad = 3;
	int topPanelHeight = 0;

	String fileToEdit = "PositionCache.cs";
	String appName = "TentacleN";

	static Literal LiteralFromEntry(TextBox w) {
		return new Literal(w.Text);
	}

	static void EntryFromLiteral(Literal s, TextBox w) {
		byte[] asUTF8 = s.AsUTF8();
		w.Text = Encoding.UTF8.GetString(asUTF8);
	}

	class FindPanel:Panel {
		private TentacleNApplication app;
		internal TextBox textFind;
		Button buttonFind;
		public int heightDesired;

		internal FindPanel(TentacleNApplication app_) {
			app = app_;
			heightDesired = buttonHeight + 2 * pad;
			SizeChanged += new EventHandler(this.Me_SizeChanged);
			Visible = false;

			Label labelFind = new Label();
			labelFind.Text = "Find What:";
			labelFind.Top = pad + 2;
			labelFind.Left = 5;
			labelFind.Width = 70;
			Controls.Add(labelFind);

			textFind = new TextBox();
			textFind.Top = pad;
			textFind.Left = 80;
			textFind.Size = new Size(100,20);
			Controls.Add(textFind);

			buttonFind = new Button();
			//buttonFind.Text = "F&ind";
			buttonFind.Text = "Find";
			buttonFind.Top = pad;
			buttonFind.Left = 200;
			buttonFind.Size = new Size(buttonWidth,20);
			buttonFind.Click += new EventHandler(Find);
			Controls.Add(buttonFind);
		}
		private void Find(Object sender, EventArgs e) {
			app.findWhat = LiteralFromEntry(textFind);
			app.SearchFindNext();
		}
		private void Me_SizeChanged(object sender, EventArgs e) {
			int col3 = ClientSize.Width - buttonWidth - pad;
			int col2 = col3 - pad;
			int widthText = col2 - textFind.Left;
			if (widthText < 10)
				widthText = 10;
			textFind.Width = widthText;
			buttonFind.Left = col3;
		}
		public virtual void actionPerformed(Object event_sender, EventArgs e) {
			//String command = e.getActionCommand();
			String command = "Find";
			if (command.Equals("Find")) {
				app.findWhat = LiteralFromEntry(textFind);
				app.SearchFindNext();
			}
		}
	}

	class ReplacePanel:Panel {
		private TentacleNApplication app;
		internal TextBox textFind;
		Button buttonFind;
		Button buttonAll;
		internal TextBox textReplace;
		Button buttonReplace;
		Button buttonInSelection;
		public int heightDesired;

		internal ReplacePanel(TentacleNApplication app_) {
			app = app_;
			heightDesired = buttonHeight * 2 + 3 * pad;
			SizeChanged += new EventHandler(this.Me_SizeChanged);
			Visible = false;

			Label labelFind = new Label();
			labelFind.Text = "Find What:";
			labelFind.Top = pad + 2;
			labelFind.Left = 20;
			labelFind.Width = 60;
			Controls.Add(labelFind);

			textFind = new TextBox();
			textFind.Top = pad;
			textFind.Left = 80;
			textFind.Size = new Size(100,buttonHeight);
			Controls.Add(textFind);

			buttonFind = new Button();
			//buttonFind.Text = "F&ind";
			buttonFind.Text = "Find";
			buttonFind.Top = pad;
			buttonFind.Left = 200;
			buttonFind.Size = new Size(buttonWidth,buttonHeight);
			buttonFind.Click += new EventHandler(Find);
			Controls.Add(buttonFind);

			buttonAll = new Button();
			//buttonAll.Text = "Replace &All";
			buttonAll.Text = "Replace All";
			buttonAll.Top = pad;
			buttonAll.Left = 300;
			buttonAll.Size = new Size(buttonWidth,buttonHeight);
			buttonAll.Click += new EventHandler(ReplaceAll);
			Controls.Add(buttonAll);

			int line2 = pad + buttonHeight + pad;
			Label labelReplace = new Label();
			labelReplace.Text = "Replace With:";
			labelReplace.Top = line2 + 3;
			labelReplace.Left = 5;
			labelReplace.Width = 75;
			Controls.Add(labelReplace);

			textReplace = new TextBox();
			textReplace.Top = line2;
			textReplace.Left = 80;
			textReplace.Size = new Size(100,buttonHeight);
			Controls.Add(textReplace);

			buttonReplace = new Button();
			//buttonReplace.Text = "&Replace";
			buttonReplace.Text = "Replace";
			buttonReplace.Top = line2;
			buttonReplace.Left = 200;
			buttonReplace.Size = new Size(buttonWidth, buttonHeight);
			buttonReplace.Click += new EventHandler(Replace);
			Controls.Add(buttonReplace);

			buttonInSelection = new Button();
			//buttonInSelection.Text = "I&n Selection";
			buttonInSelection.Text = "In Selection";
			buttonInSelection.Top = buttonHeight + pad * 2;
			buttonInSelection.Left = 300;
			buttonInSelection.Size = new Size(buttonWidth, buttonHeight);
			buttonInSelection.Click += new EventHandler(ReplaceInSelection);
			Controls.Add(buttonInSelection);
		}
		private void Find(Object sender, EventArgs e) {
			app.findWhat = LiteralFromEntry(textFind);
			app.replaceWhat = LiteralFromEntry(textReplace);
			app.SearchFindNext();
		}
		private void Replace(Object sender, EventArgs e) {
			app.findWhat = LiteralFromEntry(textFind);
			app.replaceWhat = LiteralFromEntry(textReplace);
			app.ReplaceOnce();
		}
		private void ReplaceAll(Object sender, EventArgs e) {
			app.findWhat = LiteralFromEntry(textFind);
			app.replaceWhat = LiteralFromEntry(textReplace);
			app.ReplaceAll(false);
		}
		private void ReplaceInSelection(Object sender, EventArgs e) {
			app.findWhat = LiteralFromEntry(textFind);
			app.replaceWhat = LiteralFromEntry(textReplace);
			app.ReplaceAll(true);
		}

		private void Me_SizeChanged(object sender, EventArgs e) {
			int col4 = ClientSize.Width - buttonWidth - pad;
			int col3 = col4 - buttonWidth - pad;
			int col2 = col3 - pad;
			int widthText = col2 - textFind.Left;
			if (widthText < 10)
				widthText = 10;
			textFind.Width = widthText;
			buttonFind.Left = col3;
			buttonAll.Left = col4;
			textReplace.Width = widthText;
			buttonReplace.Left = col3;
			buttonInSelection.Left = col4;
		}

		public virtual void actionPerformed(Object event_sender, EventArgs e) {
			//String command = e.getActionCommand();
			String command = "Find";
			//~ System.out.println("Actor" + command);
			if (command.Equals("Find")) {
				app.findWhat = LiteralFromEntry(textFind);
				//Console.Out.WriteLine("Actor" + textFind.Text);
				app.SearchFindNext();
			} else if (command.Equals("Replace")) {
				app.findWhat = LiteralFromEntry(textFind);
				app.replaceWhat = LiteralFromEntry(textReplace);
				//Console.Out.WriteLine("R " + textFind.Text);
				app.ReplaceOnce();
			} else if (command.Equals("Replace All")) {
				app.findWhat = LiteralFromEntry(textFind);
				app.replaceWhat = LiteralFromEntry(textReplace);
				//Console.Out.WriteLine("RAll " + textFind.Text);
				app.ReplaceAll(false);
			} else if (command.Equals("In Selection")) {
				app.findWhat = LiteralFromEntry(textFind);
				app.replaceWhat = LiteralFromEntry(textReplace);
				//Console.Out.WriteLine("InS " + textFind.Text);
				app.ReplaceAll(true);
			}
		}
	}

	public TentacleNApplication(String[] args) {
		PrototypeRegistry.Init();
		RegisteredDecorations.Init();
		RegisteredLexers.Init();
		pdoc = new Document(fontNames.FindOrAddKey(Surface.fontDefault), fontNames);
		for (int i=0;i<args.Length;i++) {
			fileToEdit = args[i];
		}
		for (int c=0;c<controls;c++) {
			tc[c] = new TentacleControl();
			tc[c].SetDocument(pdoc, this);
		}
		init();
		Size = new Size(600, 800);
		Show();
		GrabFile(fileToEdit);
		SetTitle(true);
	}

	TentacleControl ControlActive() {
		return tc[activeControl];
	}

	void Deallocate() {
		DeallocateStyling();
		for (int c=0;c<controls;c++) {
			tc[c].Deallocate();
		}
	}

	void DeallocateStyling() {
		pdoc.DeallocateStyling();
	}

	void CreateDocument(String name) {
		Deallocate();
		pdoc = new Document(fontNames.FindOrAddKey(Surface.fontDefault), fontNames);
	}

	bool ReadOptionsFile(Literal prefix, String filename) {
		try {
			FileStream fsr = new FileStream(filename, FileMode.Open, FileAccess.Read);
			try {
				byte[] propsData = new byte[60000];
				int lenData = fsr.Read(propsData, 0, 60000);
				pdoc.options.SetFromText(prefix, propsData, 0, lenData);
				return true;
			} catch (IOException) {
			}
		} catch (FileNotFoundException ) {
		}
		return false;
	}

	void ReadOptionsAndModes() {
		pdoc.ClearOptions();

		String homeDir = Environment.GetEnvironmentVariable("USERPROFILE") + "\\";
		String userOptions = homeDir + "user.options";
		if (!ReadOptionsFile(null, userOptions)) {
			Console.WriteLine("No options file: " + userOptions);
			Environment.Exit(1);
		}
		DirectoryInfo dir = new DirectoryInfo(homeDir);
		FileInfo[] modeFiles = dir.GetFiles("*.mode");
		foreach (FileInfo f in modeFiles) {
			String name = f.Name;
			if (name.EndsWith(".mode")) {
				String baseName = name.Substring(0, name.Length - 5);
				Literal modeName = new Literal("mode." + baseName);
				ReadOptionsFile(modeName, homeDir + name);
			}
		}
	}

	void CreateStyling(String name) {
		if (name.Equals("")) {
			return;
		}
		DeallocateStyling();

		Literal literalName = new Literal(name);
		int dot = literalName.Length();
		while ((dot > 0) && literalName.CharAt(dot-1) != '.') {
			dot--;
		}
		int slen = literalName.Length() - dot;
		Literal literalExtension = new Literal(literalName, dot, literalName.Length() - dot);
		ReadOptionsAndModes();
		pdoc.currentMode = new Literal("");
		pdoc.CreateStyling(literalExtension);
		for (int c=0;c<controls;c++) {
			tc[c].SetDocument(pdoc, this);
		}
	}

	void SetTitle(bool changePath) {
		if (changePath || (titleAtSavePoint != pdoc.pbuffer.IsSavePoint())) {
			titleAtSavePoint = pdoc.pbuffer.IsSavePoint();
			String title = fullPath;
			if (titleAtSavePoint) {
				title += " - ";
			} else {
				title += " * ";
			}
			title += appName;
			Text = title;
		}
	}

	public void StateAltered() {
		for (int c=0;c<controls;c++) {
			if (tc[c].textArea.hasFocus()) {
				activeControl = c;
			}
			if (tc[c].wrap) {
				tc[c].StartWrap();
			}
		}
		SetTitle(false);
		CheckMenus();
	}

	public void InvalidateAll() {
		for (int c=0;c<controls;c++) {
			tc[c].textArea.Invalidate();
		}
	}

	public void InvalidateRange(int a, int b) {
		for (int c=0;c<controls;c++) {
			tc[c].InvalidateRange(a,b);
		}
	}

	void NewDocument() {
		for (int c=0;c<controls;c++) {
			tc[c].InitScrollBars();
		}
		pdoc.pbuffer.DiscardLines();
		pdoc.pbuffer.DeleteRange(0, pdoc.pbuffer.Length());
		tc[0].GrabFocus();
	}

	void New() {
		if (SaveIfUnsure() != DialogResult.Cancel) {
			NewDocument();
			SetTitle(true);
		}
		InvalidateAll();
	}

	void Open() {
		if (SaveIfUnsure() != DialogResult.Cancel) {
			OpenFileDialog chooser = new OpenFileDialog();
			chooser.InitialDirectory = "." ;
			DialogResult returnVal = chooser.ShowDialog();
			if (returnVal == DialogResult.OK) {
				GrabFile(chooser.FileName);
				SetTitle(true);
				InvalidateAll();
			}
		}
	}

	DialogResult SaveIfUnsure() {
		if (!pdoc.pbuffer.IsSavePoint()) {
			String msg = "Save changes to \"" + fullPath + "\"?";
			DialogResult decision = MessageBox.Show(msg, appName,
				MessageBoxButtons.YesNoCancel, MessageBoxIcon.Exclamation);
			if (decision == DialogResult.Yes) {
				Save();
			}
			return decision;
		}
		return DialogResult.Yes;
	}

	void Save() {
		try {
			FileStream fos = new FileStream(fullPath, FileMode.Create);
			byte[] data = new byte[blockSize];
			int length = pdoc.pbuffer.Length();
			for (int i = 0; length > 0; i += blockSize) {
				int grabSize = length - i;
				if (grabSize > blockSize)
					grabSize = blockSize;
				pdoc.pbuffer.RetrieveBytes(i, data, grabSize);
				fos.Write(data, 0, grabSize);
				length -= grabSize;
			}
			fos.Close();
			pdoc.pbuffer.SetSavePoint();
		} catch (IOException) {
			Console.Out.WriteLine("Failed to write to " + fullPath);
		}
	}

	void SaveAs() {
		SaveFileDialog chooser = new SaveFileDialog();
		chooser.InitialDirectory = "." ;
		chooser.FileName = fullPath;
		DialogResult returnVal = chooser.ShowDialog();
		if (returnVal == DialogResult.OK) {
			fullPath = chooser.FileName;
			if (fullPath != null) {
				Save();
				SetTitle(true);
			}
		}
	}

	void Exit() {
		if (SaveIfUnsure() != DialogResult.Cancel) {
			Environment.Exit(0);
		}
	}

	private void MenuClick(Object sender, EventArgs e) {
		MenuItem m = (MenuItem)sender;
		String command = m.Text.Replace("&", "");
		if (command.Equals("New")) {
			New();
		} else if (command.Equals("Open")) {
			Open();
		} else if (command.Equals("Save")) {
			if ((fullPath == null) || (fullPath.Length == 0)) {
				SaveAs();
			} else {
				Save();
			}
		} else if (command.Equals("Save As")) {
			SaveAs();
		} else if (command.Equals("Exit")) {
			Exit();
		} else if (command.Equals("Undo")) {
			ControlActive().Undo();
		} else if (command.Equals("Redo")) {
			ControlActive().Redo();
		} else if (command.Equals("Cut")) {
			ControlActive().Cut();
		} else if (command.Equals("Copy")) {
			ControlActive().Copy();
		} else if (command.Equals("Paste")) {
			ControlActive().Paste();
		} else if (command.Equals("Delete")) {
			ControlActive().Delete();
		} else if (command.Equals("Select All")) {
			ControlActive().SelectAll();
		} else if (command.Equals("Fold")) {
			ControlActive().ToggleFold();
		} else if (command.Equals("Fold All")) {
			ControlActive().FoldAll(ControlActive().fcFold);
		} else if (command.Equals("Show All")) {
			ControlActive().FoldAll(ControlActive().fcShow);
		} else if (command.Equals("Find")) {
			SearchFind();
		} else if (command.Equals("Find Next")) {
			if (findWhat.Equals("")) {
				SearchFind();
			} else {
				SearchFindNext();
			}
		} else if (command.Equals("Replace")) {
			SearchReplace();
		} else if (command.Equals("Hide")) {
			SelectPane(0);
		} else if (command.Equals("Wrap")) {
			ControlActive().ToggleWrap();
			CheckMenus();
		} else if (command.Equals("Switch Pane")) {
			if (tc[0].textArea.hasFocus()) {
				tc[1].GrabFocus();
			} else {
				tc[0].GrabFocus();
			}
		}
	}

	private void CheckMenus() {
		//EnableAMenuItem(IDM_FILE_SAVE, isDirty);
		menuItemWrap.Checked = ControlActive().wrap;
	}

	private void SetUpMenus() {
		EventHandler clicky = new EventHandler(this.MenuClick);

		MainMenu menuBar = new MainMenu();
		MenuItem menuFile = new MenuItem("&File");
		MenuItem menuItem;
		menuItem = new MenuItem("&New", clicky, Shortcut.CtrlN);
		menuFile.MenuItems.Add(menuItem);
		menuItem = new MenuItem("&Open", clicky, Shortcut.CtrlO);
		menuFile.MenuItems.Add(menuItem);
		menuItem = new MenuItem("&Save", clicky, Shortcut.CtrlS);
		menuFile.MenuItems.Add(menuItem);
		menuItem = new MenuItem("Save &As", clicky, Shortcut.CtrlShiftS);
		menuFile.MenuItems.Add(menuItem);
		menuItem = new MenuItem("-");
		menuFile.MenuItems.Add(menuItem);
		menuItem = new MenuItem("E&xit", clicky);
		menuFile.MenuItems.Add(menuItem);
		menuBar.MenuItems.Add(menuFile);

		MenuItem menuEdit = new MenuItem("&Edit");
		menuBar.MenuItems.Add(menuEdit);
		menuItem = new MenuItem("&Undo", clicky, Shortcut.CtrlZ);
		menuEdit.MenuItems.Add(menuItem);
		menuItem = new MenuItem("&Redo", clicky, Shortcut.CtrlY);
		menuEdit.MenuItems.Add(menuItem);
		menuItem = new MenuItem("-");
		menuEdit.MenuItems.Add(menuItem);
		menuItem = new MenuItem("Cu&t", clicky, Shortcut.CtrlX);
		menuEdit.MenuItems.Add(menuItem);
		menuItem = new MenuItem("&Copy", clicky, Shortcut.CtrlC);
		menuEdit.MenuItems.Add(menuItem);
		menuItem = new MenuItem("&Paste", clicky, Shortcut.CtrlV);
		menuEdit.MenuItems.Add(menuItem);
		menuItem = new MenuItem("&Delete", clicky, Shortcut.Del);
		menuEdit.MenuItems.Add(menuItem);
		menuItem = new MenuItem("Select &All", clicky, Shortcut.CtrlA);
		menuEdit.MenuItems.Add(menuItem);
		menuItem = new MenuItem("-");
		menuEdit.MenuItems.Add(menuItem);
		menuItem = new MenuItem("&Fold", clicky, Shortcut.CtrlL);
		menuEdit.MenuItems.Add(menuItem);
		menuItem = new MenuItem("F&old All", clicky);
		menuEdit.MenuItems.Add(menuItem);
		menuItem = new MenuItem("S&how All", clicky);
		menuEdit.MenuItems.Add(menuItem);

		MenuItem menuSearch = new MenuItem("&Search");
		menuBar.MenuItems.Add(menuSearch);
		menuItem = new MenuItem("&Find", clicky, Shortcut.CtrlF);
		menuSearch.MenuItems.Add(menuItem);
		menuItem = new MenuItem("Find &Next", clicky, Shortcut.F3);
		menuSearch.MenuItems.Add(menuItem);
		menuItem = new MenuItem("&Replace", clicky, Shortcut.CtrlH);
		menuSearch.MenuItems.Add(menuItem);
		menuItem = new MenuItem("&Hide", clicky);
		//menuItem = new MenuItem("&Hide", clicky, Shortcut.Esc);
		menuSearch.MenuItems.Add(menuItem);

		MenuItem menuOptions = new MenuItem("&Options");
		menuBar.MenuItems.Add(menuOptions);
		menuItemWrap = new MenuItem("&Wrap", clicky);
		menuOptions.MenuItems.Add(menuItemWrap);
		menuItem = new MenuItem("&Switch Pane", clicky, Shortcut.F6);
		menuOptions.MenuItems.Add(menuItem);

		Menu = menuBar;
	}

	private void Me_SizeChanged(object sender, EventArgs e) {
		tc[0].Width = ClientSize.Width;
		tc[0].Top = topPanelHeight;
		Literal sizeTopProportion = new Literal("size.top.proportion");
		Literal topProportion = pdoc.options.ModeValue(pdoc.currentMode, sizeTopProportion);
		if (topProportion != null) {
			tc[0].Height = (ClientSize.Height - topPanelHeight) * topProportion.Value() / 100;
		} else {
			tc[0].Height = (ClientSize.Height - topPanelHeight);
		}
		tc[1].Width = ClientSize.Width;
		tc[1].Height = (ClientSize.Height - topPanelHeight) - tc[0].Height;
		tc[1].Top = tc[0].Top + tc[0].Height;
		panelFind.Width = this.ClientSize.Width;
		panelFind.Height = topPanelHeight;
		panelReplace.Width = this.ClientSize.Width;
		panelReplace.Height = topPanelHeight;
		panelHeader.Width = this.ClientSize.Width;
		panelHeader.Height = topPanelHeight;
	}

	public virtual void init() {
		SetUpMenus();

		panelHeader = new Panel();
		panelFind = new FindPanel(this);
		panelFind.Top = 0;
		panelFind.Left = 0;
		panelFind.Size = new Size(400,30);
		panelReplace = new ReplacePanel(this);
		panelHeader.Controls.Add(panelReplace);
		panelHeader.Controls.Add(panelFind);

		Controls.Add(panelHeader);
		panelHeader.Top = 0;
		panelHeader.Left = 0;
		panelHeader.Size = new Size(400,30);

		fullPath = fileToEdit;
		CreateDocument(fullPath);
		CreateStyling(fullPath);
		for (int c=0;c<controls;c++) {
			Controls.Add(tc[c]);
			tc[c].GrabFocus();
		}
		SizeChanged += new EventHandler(this.Me_SizeChanged);
	}

	void GrabFile(String name) {
		try {
			fullPath = name;
			CreateDocument(fullPath);
			CreateStyling(fullPath);
			FileStream fsr = new FileStream(name, FileMode.Open, FileAccess.Read);
			NewDocument();
			try {
				byte[] data = new byte[32768];
				int count = fsr.Read(data, 0, 32768);
				while (count > 0) {
					ControlActive().AddContents(data, count);
					count = fsr.Read(data, 0, 32768);
				}
				pdoc.pbuffer.DeleteUndoHistory();
			} catch (IOException) {
			}
		} catch (FileNotFoundException) {
		}
	}

	int SearchText(Literal text, int start, int end) {
		int findLength = pdoc.pbuffer.LengthInText(text.Data(), text.Length());
		int last = end - findLength;
		for (int look=start; look<last; look = pdoc.pbuffer.PositionNext(look)) {
			bool found = true;
			int check = look;
			for (int j=0;j<findLength && found;j++) {
				if (pdoc.pbuffer.UnsignedByteAt(look+j) != text.CharAt(j))
					found = false;
				check = pdoc.pbuffer.PositionNext(check);
			}
			if (found) {
				return look;
			}
		}
		return -1;
	}

	void SearchFindNext() {
		haveFound = false;
		int docLength = pdoc.pbuffer.Length();
		int startEnd = System.Math.Max(
			ControlActive().textArea.sr.Anchor(), ControlActive().textArea.sr.Caret());
		int next = SearchText(findWhat, startEnd, docLength);
		if (next < 0) {
			next = SearchText(findWhat, 0, startEnd);
		}
		if (next >= 0) {
			int findLength = pdoc.pbuffer.LengthInText(findWhat.Data(), findWhat.Length());
			ControlActive().SetSelection(next, next + findLength);
			ControlActive().EnsureCaretVisible();
			ControlActive().textArea.Invalidate();
			haveFound = true;
		}
	}

	void ReplaceOnce() {
		if (haveFound) {
			try {
				int start = ControlActive().textArea.sr.StartSelection();
				ControlActive().textArea.DeleteRange(start, ControlActive().textArea.sr.EndSelection() - start);
				int len = ControlActive().textArea.InsertUTF32(start, replaceWhat.Data(), replaceWhat.Length());
				ControlActive().SetSelection(start, start + len);
				ControlActive().textArea.Invalidate();
			} catch (IOException) {
			}
		}
		SearchFindNext();
	}

	void SearchFind() {
		SelectPane(1);
		haveFound = false;
		Literal wordAtCaret = ControlActive().textArea.WordAtPosition(ControlActive().textArea.sr.Caret());
		EntryFromLiteral(wordAtCaret, panelFind.textFind);
		panelFind.textFind.Focus();
		ControlActive().EnsureCaretVisible();
	}

	void SearchReplace() {
		SelectPane(2);
		haveFound = false;
		Literal wordAtCaret = ControlActive().textArea.WordAtPosition(ControlActive().textArea.sr.Caret());
		EntryFromLiteral(wordAtCaret, panelReplace.textFind);
		panelReplace.textFind.Focus();
		ControlActive().EnsureCaretVisible();
	}

	void ReplaceAll(bool selectionRange) {
		int startPosition = selectionRange ? ControlActive().textArea.sr.StartSelection() : 0;
		int endPosition = selectionRange ? ControlActive().textArea.sr.EndSelection() : pdoc.pbuffer.Length();
		int findLength = pdoc.pbuffer.LengthInText(findWhat.Data(), findWhat.Length());
		if (findLength == 0)
			return;
		try {
			int nextMatch = SearchText(findWhat, startPosition, endPosition);
			while (nextMatch >= 0) {
				ControlActive().textArea.DeleteRange(nextMatch, findLength);
				int replaceLength =
					ControlActive().textArea.InsertUTF32(nextMatch, replaceWhat.Data(), replaceWhat.Length());
				startPosition = nextMatch + replaceLength;
				endPosition += replaceLength - findLength;
				nextMatch = SearchText(findWhat, startPosition, endPosition);
			}
		} catch (IOException) {
		}
		InvalidateAll();
	}

	void SelectPane(int pane) {
		if (pane != panesVisible) {
			switch (panesVisible) {

				case 1:
					panelFind.Visible = false;
					break;

				case 2:
					panelReplace.Visible = false;
					break;
			}
			switch (pane) {

				case 1:
					topPanelHeight = panelFind.heightDesired;
					panelFind.Visible = true;
					break;

				case 2:
					topPanelHeight = panelReplace.heightDesired;
					panelReplace.Visible = true;
					break;
			}
			panesVisible = pane;
			if (pane == 0) {
				topPanelHeight = 0;
				ControlActive().GrabFocus();
			}
			Me_SizeChanged(null, null);
		}
	}
}

public class TentacleN {
	[STAThread]
	static public void Main(String[] args) {
		Application.Run(new TentacleNApplication(args));
	}
}
