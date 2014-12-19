package SinkWorld;

import java.awt.*;
import java.awt.event.*;
import java.awt.datatransfer.*;
import java.io.*;
import javax.swing.*;

/**
 * TentacleJ demonstration editor copyright 2004 Neil Hodgson.
 */

class TentacleJApplication extends JFrame
	implements ActionListener, IDocumentChangeListener {

	final static int blockSize = 32768;

	LiteralSet fontNames = new LiteralSet();
	Document pdoc = null;

	boolean titleAtSavePoint = true;
	String fullPath = "";

	int panesVisible = 0;
	JPanel panelHeader;
	FindPanel panelFind;
	ReplacePanel panelReplace;
	boolean haveFound;
	Literal findWhat = new Literal("");
	Literal replaceWhat = new Literal("");
	int controls = 1;
	int activeControl = 0;
	TentacleControl tc[] = new TentacleControl[controls];

	JCheckBoxMenuItem menuItemBraceLevel;
	JCheckBoxMenuItem menuItemWrap;

	String appName = "TentacleJ";

	static Literal LiteralFromEntry(JTextField w) {
		return new Literal(w.getText());
	}

	static void EntryFromLiteral(Literal s, JTextField w) {
		byte[] asUTF8 = s.AsUTF8();
		w.setText(new String(asUTF8));
	}

	class FindPanel extends JPanel implements ActionListener {
		JTextField textFind;
		FindPanel() {
			setLayout(new GridBagLayout());
			setVisible(false);
			GridBagConstraints c = new GridBagConstraints();

			c.fill = GridBagConstraints.HORIZONTAL;
			c.ipadx = 2;
			c.ipady = 1;
			c.insets = new java.awt.Insets(2,2,2,2);

			JLabel labelFind = new JLabel("Find What:");
			c.anchor = GridBagConstraints.LINE_END;
			c.weightx = 0.0;
			c.gridx = 0;
			c.gridy = 0;
			add(labelFind, c);

			textFind = new JTextField(20);
			c.anchor = GridBagConstraints.CENTER;
			c.weightx = 0.5;
			c.gridx = 1;
			c.gridy = 0;
			add(textFind, c);

			JButton buttonFind = new JButton("Find");
			buttonFind.setMnemonic(KeyEvent.VK_I);
			buttonFind.addActionListener(this);
			c.weightx = 0.0;
			c.gridx = 2;
			c.gridy = 0;
			add(buttonFind, c);
		}
		public void actionPerformed(ActionEvent e) {
			String command = e.getActionCommand();
			if (command.equals("Find")) {
				findWhat = LiteralFromEntry(textFind);
				SearchFindNext();
			}
		}
	}

	class ReplacePanel extends JPanel implements ActionListener {
		JTextField textFind;
		JTextField textReplace;
		ReplacePanel() {
			setLayout(new GridBagLayout());
			setVisible(false);
			GridBagConstraints c = new GridBagConstraints();

			c.fill = GridBagConstraints.HORIZONTAL;
			c.ipadx = 2;
			c.ipady = 1;
			c.insets = new java.awt.Insets(2,2,2,2);

			JLabel labelFind = new JLabel("Find What:");
			c.anchor = GridBagConstraints.LINE_END;
			c.weightx = 0.0;
			c.gridx = 0;
			c.gridy = 0;
			add(labelFind, c);

			textFind = new JTextField(20);
			c.anchor = GridBagConstraints.CENTER;
			c.weightx = 0.5;
			c.gridx = 1;
			c.gridy = 0;
			add(textFind, c);

			JButton buttonFind = new JButton("Find");
			buttonFind.setMnemonic(KeyEvent.VK_I);
			buttonFind.addActionListener(this);
			c.weightx = 0.0;
			c.gridx = 2;
			c.gridy = 0;
			add(buttonFind, c);

			JButton buttonAll = new JButton("Replace All");
			buttonAll.setMnemonic(KeyEvent.VK_A);
			buttonAll.addActionListener(this);
			c.weightx = 0.0;
			c.gridx = 3;
			c.gridy = 0;
			add(buttonAll, c);

			JLabel labelReplace = new JLabel("Replace With:");
			c.anchor = GridBagConstraints.LINE_END;
			c.weightx = 0.0;
			c.gridx = 0;
			c.gridy = 1;
			add(labelReplace, c);

			textReplace = new JTextField(20);
			c.anchor = GridBagConstraints.CENTER;
			c.weightx = 0.5;
			c.gridx = 1;
			c.gridy = 1;
			add(textReplace, c);

			JButton buttonReplace = new JButton("Replace");
			buttonReplace.setMnemonic(KeyEvent.VK_R);
			buttonReplace.addActionListener(this);
			c.weightx = 0.0;
			c.gridx = 2;
			c.gridy = 1;
			add(buttonReplace, c);

			JButton buttonInSelection = new JButton("In Selection");
			buttonInSelection.setMnemonic(KeyEvent.VK_N);
			buttonInSelection.addActionListener(this);
			c.weightx = 0.0;
			c.gridx = 3;
			c.gridy = 1;
			add(buttonInSelection, c);
		}

		public void actionPerformed(ActionEvent e) {
			String command = e.getActionCommand();
			//~ System.out.println("Actor" + command);
			if (command.equals("Find")) {
				findWhat = LiteralFromEntry(textFind);
				//~ System.out.println("Actor" + textFind.getText());
				SearchFindNext();
			} else if (command.equals("Replace")) {
				findWhat = LiteralFromEntry(textFind);
				replaceWhat = LiteralFromEntry(textReplace);
				//~ System.out.println("R " + textFind.getText());
				ReplaceOnce();
			} else if (command.equals("Replace All")) {
				findWhat = LiteralFromEntry(textFind);
				replaceWhat = LiteralFromEntry(textReplace);
				//~ System.out.println("RAll " + textFind.getText());
				ReplaceAll(false);
			} else if (command.equals("In Selection")) {
				findWhat = LiteralFromEntry(textFind);
				replaceWhat = LiteralFromEntry(textReplace);
				//~ System.out.println("InS " + textFind.getText());
				ReplaceAll(true);
			}
		}
	}

	TentacleJApplication() {
		pdoc = new Document(fontNames.FindOrAddKey(Surface.fontDefault), fontNames);
		for (int c=0;c<controls;c++) {
			tc[c] = new TentacleControl();
			tc[c].SetDocument(pdoc, this);
		}
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

	boolean ReadOptionsFile(Literal prefix, String filename) {
		try {
			FileInputStream fsr = new FileInputStream(filename);
			try {
				byte[] propsData = new byte[60000];
				int lenData = fsr.read(propsData, 0, 60000);
				pdoc.options.SetFromText(prefix, propsData, 0, lenData);
				return true;
			} catch (IOException x) {
			}
		} catch (FileNotFoundException x) {
		}
		return false;
	}

	void ReadOptionsAndModes() {
		pdoc.ClearOptions();

		String homeDir = System.getProperty("user.home") + System.getProperty("file.separator");
		String userOptions = homeDir + "user.options";
		if (!ReadOptionsFile(null, userOptions)) {
			System.err.println("No options file: " + userOptions);
			System.exit(1);
		}
		File myDir = new File(homeDir);
		File[] contents = myDir.listFiles();
		for (int i = 0; i < contents.length; i++) {
			File f = contents[i];
			String name = f.getName();
			if (name.endsWith(".mode")) {
				String baseName = name.substring(0, name.length() - 5);
				Literal modeName = new Literal("mode." + baseName);
				ReadOptionsFile(modeName, homeDir + name);
			}
		}
	}

	void CreateStyling(String name) {
		if (name.equals("")) {
			return;
		}
		DeallocateStyling();

		Literal literalName = new Literal(name);
		int dot = literalName.Length();
		while ((dot > 0) && literalName.CharAt(dot-1) != '.') {
			dot--;
		}
		Literal literalExtension = new Literal(literalName, dot, literalName.Length() - dot);
		ReadOptionsAndModes();
		pdoc.currentMode = new Literal("");
		pdoc.CreateStyling(literalExtension);
		for (int c=0;c<controls;c++) {
			tc[c].SetDocument(pdoc, this);
		}
	}

	void SetTitle(boolean changePath) {
		if (changePath || (titleAtSavePoint != pdoc.pbuffer.IsSavePoint())) {
			titleAtSavePoint = pdoc.pbuffer.IsSavePoint();
			String title = fullPath;
			if (titleAtSavePoint) {
				title += " - ";
			} else {
				title += " * ";
			}
			title += appName;
			setTitle(title);
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
		if (SaveIfUnsure() != JOptionPane.CANCEL_OPTION) {
			NewDocument();
			SetTitle(true);
		}
		InvalidateAll();
	}

	void Open() {
		if (SaveIfUnsure() != JOptionPane.CANCEL_OPTION) {
			JFileChooser chooser = new JFileChooser(".");
			int returnVal = chooser.showOpenDialog(this);
			if (returnVal == JFileChooser.APPROVE_OPTION) {
				GrabFile(chooser.getSelectedFile().getAbsolutePath());
				SetTitle(true);
				InvalidateAll();
			}
		}
	}

	int SaveIfUnsure() {
		if (!pdoc.pbuffer.IsSavePoint()) {
			String msg= "Save changes to \"" + fullPath + "\"?";
			int decision = JOptionPane.showConfirmDialog(this,
				msg, appName, JOptionPane.YES_NO_CANCEL_OPTION);
			if (decision == JOptionPane.YES_OPTION) {
				Save();
			}
			return decision;
		}
		return JOptionPane.YES_OPTION;
	}

	void Save() {
		try {
			FileOutputStream fos = new FileOutputStream(fullPath);
			byte[] data = new byte[blockSize];
			int length = pdoc.pbuffer.Length();
			for (int i = 0; length > 0; i += blockSize) {
				int grabSize = length - i;
				if (grabSize > blockSize)
					grabSize = blockSize;
				pdoc.pbuffer.RetrieveBytes(i, data, i + grabSize);
				fos.write(data, 0, grabSize);
				length -= grabSize;
			}
			fos.close();
			pdoc.pbuffer.SetSavePoint();
		} catch (IOException x) {
			System.err.println("Failed to write to " + fullPath);
		}
	}

	void SaveAs() {
		JFileChooser chooser = new JFileChooser(".");
		int returnVal = chooser.showSaveDialog(this);
		if (returnVal == JFileChooser.APPROVE_OPTION) {
			fullPath = chooser.getSelectedFile().getAbsolutePath();
			if (fullPath != null) {
				Save();
				SetTitle(true);
			}
		}
	}

	void Exit() {
		if (SaveIfUnsure() != JOptionPane.CANCEL_OPTION) {
			System.exit(0);
		}
	}

	public void actionPerformed(ActionEvent e) {
		String command = e.getActionCommand();
		//~ System.out.println("Actor" + command);
		if (command.equals("New")) {
			New();
		} else if (command.equals("Open")) {
			Open();
		} else if (command.equals("Save")) {
			if ((fullPath == null) || (fullPath.length() == 0)) {
				SaveAs();
			} else {
				Save();
			}
		} else if (command.equals("Save As")) {
			SaveAs();
		} else if (command.equals("Exit")) {
			Exit();
		} else if (command.equals("Undo")) {
			ControlActive().Undo();
		} else if (command.equals("Redo")) {
			ControlActive().Redo();
		} else if (command.equals("Cut")) {
			ControlActive().Cut();
		} else if (command.equals("Copy")) {
			ControlActive().Copy();
		} else if (command.equals("Paste")) {
			ControlActive().Paste();
		} else if (command.equals("Delete")) {
			ControlActive().Delete();
		} else if (command.equals("Select All")) {
			ControlActive().SelectAll();
		} else if (command.equals("Fold")) {
			ControlActive().ToggleFold();
		} else if (command.equals("Fold All")) {
			ControlActive().FoldAll(TentacleControl.fcFold);
		} else if (command.equals("Show All")) {
			ControlActive().FoldAll(TentacleControl.fcShow);
		} else if (command.equals("Find")) {
			SearchFind();
		} else if (command.equals("Find Next")) {
			if (findWhat.equals("")) {
				SearchFind();
			} else {
				SearchFindNext();
			}
		} else if (command.equals("Replace")) {
			SearchReplace();
		} else if (command.equals("Hide")) {
			SelectPane(0);
		} else if (command.equals("Wrap")) {
			ControlActive().ToggleWrap();
			CheckMenus();
		} else if (command.equals("Switch Pane")) {
			if (tc[0].textArea.hasFocus()) {
				tc[1].GrabFocus();
			} else {
				tc[0].GrabFocus();
			}
		}
	}

	private void CheckMenus() {
		//EnableAMenuItem(IDM_FILE_SAVE, isDirty);
		//~ menuItemBraceLevel.setState(ControlActive().braceLevel);
		menuItemWrap.setState(ControlActive().wrap);
	}

	private void SetUpMenus() {
		JMenuBar menuBar = new JMenuBar();
		JMenu menuFile = new JMenu("File");
		menuFile.setMnemonic(KeyEvent.VK_F);
		menuBar.add(menuFile);
		JMenuItem menuItem;
		menuItem = new JMenuItem("New", KeyEvent.VK_N);
		menuItem.setAccelerator(KeyStroke.getKeyStroke(
        		KeyEvent.VK_N, ActionEvent.CTRL_MASK));
		menuFile.add(menuItem);
        	menuItem.addActionListener(this);
		menuItem = new JMenuItem("Open", KeyEvent.VK_O);
		menuItem.setAccelerator(KeyStroke.getKeyStroke(
        		KeyEvent.VK_O, ActionEvent.CTRL_MASK));
		menuFile.add(menuItem);
        	menuItem.addActionListener(this);
		menuItem = new JMenuItem("Save", KeyEvent.VK_S);
		menuItem.setAccelerator(KeyStroke.getKeyStroke(
        		KeyEvent.VK_S, ActionEvent.CTRL_MASK));
		menuFile.add(menuItem);
        	menuItem.addActionListener(this);
		menuItem = new JMenuItem("Save As", KeyEvent.VK_A);
		menuItem.setAccelerator(KeyStroke.getKeyStroke(
        		KeyEvent.VK_S, ActionEvent.CTRL_MASK | ActionEvent.SHIFT_MASK));
		menuFile.add(menuItem);
        	menuItem.addActionListener(this);
		menuFile.addSeparator();
		menuItem = new JMenuItem("Exit", KeyEvent.VK_X);
		menuFile.add(menuItem);
        	menuItem.addActionListener(this);

		JMenu menuEdit = new JMenu("Edit");
		menuEdit.setMnemonic(KeyEvent.VK_E);
		menuBar.add(menuEdit);
		menuItem = new JMenuItem("Undo", KeyEvent.VK_U);
		menuItem.setAccelerator(KeyStroke.getKeyStroke(
        		KeyEvent.VK_Z, ActionEvent.CTRL_MASK));
		menuEdit.add(menuItem);
        	menuItem.addActionListener(this);
		menuItem = new JMenuItem("Redo", KeyEvent.VK_R);
		menuItem.setAccelerator(KeyStroke.getKeyStroke(
        		KeyEvent.VK_Y, ActionEvent.CTRL_MASK));
		menuEdit.add(menuItem);
        	menuItem.addActionListener(this);
		menuEdit.addSeparator();
		menuItem = new JMenuItem("Cut", KeyEvent.VK_T);
		menuItem.setAccelerator(KeyStroke.getKeyStroke(
        		KeyEvent.VK_X, ActionEvent.CTRL_MASK));
		menuEdit.add(menuItem);
        	menuItem.addActionListener(this);
		menuItem = new JMenuItem("Copy", KeyEvent.VK_C);
		menuItem.setAccelerator(KeyStroke.getKeyStroke(
        		KeyEvent.VK_C, ActionEvent.CTRL_MASK));
		menuEdit.add(menuItem);
        	menuItem.addActionListener(this);
		menuItem = new JMenuItem("Paste", KeyEvent.VK_P);
		menuItem.setAccelerator(KeyStroke.getKeyStroke(
        		KeyEvent.VK_V, ActionEvent.CTRL_MASK));
		menuEdit.add(menuItem);
        	menuItem.addActionListener(this);
		menuItem = new JMenuItem("Delete", KeyEvent.VK_D);
		menuItem.setAccelerator(KeyStroke.getKeyStroke(
        		KeyEvent.VK_DELETE, 0));
		menuEdit.add(menuItem);
        	menuItem.addActionListener(this);
		menuItem = new JMenuItem("Select All", KeyEvent.VK_A);
		menuItem.setAccelerator(KeyStroke.getKeyStroke(
        		KeyEvent.VK_A, ActionEvent.CTRL_MASK));
		menuEdit.add(menuItem);
        	menuItem.addActionListener(this);
		menuEdit.addSeparator();
		menuItem = new JMenuItem("Fold", KeyEvent.VK_F);
		menuItem.setAccelerator(KeyStroke.getKeyStroke(
        		KeyEvent.VK_L, ActionEvent.CTRL_MASK));
		menuEdit.add(menuItem);
        	menuItem.addActionListener(this);
		menuItem = new JMenuItem("Fold All", KeyEvent.VK_O);
		menuEdit.add(menuItem);
        	menuItem.addActionListener(this);
		menuItem = new JMenuItem("Show All", KeyEvent.VK_H);
		menuEdit.add(menuItem);
        	menuItem.addActionListener(this);

		JMenu menuSearch = new JMenu("Search");
		menuSearch.setMnemonic(KeyEvent.VK_S);
		menuBar.add(menuSearch);
		menuItem = new JMenuItem("Find", KeyEvent.VK_F);
		menuItem.setAccelerator(KeyStroke.getKeyStroke(
        		KeyEvent.VK_F, ActionEvent.CTRL_MASK));
		menuSearch.add(menuItem);
        	menuItem.addActionListener(this);
		menuItem = new JMenuItem("Find Next", KeyEvent.VK_N);
		menuItem.setAccelerator(KeyStroke.getKeyStroke(
        		KeyEvent.VK_F3, 0));
		menuSearch.add(menuItem);
        	menuItem.addActionListener(this);
		menuItem = new JMenuItem("Replace", KeyEvent.VK_R);
		menuItem.setAccelerator(KeyStroke.getKeyStroke(
        		KeyEvent.VK_H, ActionEvent.CTRL_MASK));
		menuSearch.add(menuItem);
        	menuItem.addActionListener(this);
		menuItem = new JMenuItem("Hide", KeyEvent.VK_H);
		menuItem.setAccelerator(KeyStroke.getKeyStroke(
        		KeyEvent.VK_ESCAPE, 0));
		menuSearch.add(menuItem);
        	menuItem.addActionListener(this);

		JMenu menuOptions = new JMenu("Options");
		menuOptions.setMnemonic(KeyEvent.VK_O);
		menuBar.add(menuOptions);
		menuItemWrap = new JCheckBoxMenuItem("Wrap");
		menuItemWrap.setMnemonic(KeyEvent.VK_W);
		menuOptions.add(menuItemWrap);
        	menuItemWrap.addActionListener(this);
		menuItem = new JMenuItem("Switch Pane", KeyEvent.VK_S);
		menuItem.setAccelerator(KeyStroke.getKeyStroke(
        		KeyEvent.VK_F6, 0));
		menuOptions.add(menuItem);
        	menuItem.addActionListener(this);

		setJMenuBar(menuBar);

	}

	public void init() {
		String laf = UIManager.getSystemLookAndFeelClassName();
		try {
			//UIManager.setLookAndFeel(laf);
			UIManager.setLookAndFeel("javax.swing.plaf.metal.MetalLookAndFeel");
		} catch (UnsupportedLookAndFeelException x) {
			System.err.println("Warning: UnsupportedLookAndFeel: " + laf);
		} catch (Exception x) {
			System.err.println("Error loading " + laf + ": " + x);
		}

		SetUpMenus();

		//getContentPane().setLayout(new BorderLayout());
		getContentPane().setLayout(new GridBagLayout());
		GridBagConstraints gbc = new GridBagConstraints();

		gbc.fill = GridBagConstraints.HORIZONTAL;
		gbc.ipadx = 0;
		gbc.ipady = 0;
		gbc.insets = new java.awt.Insets(0,0,0,0);

		//c.anchor = GridBagConstraints.LINE_END;
		gbc.weightx = 1.0;
		gbc.weighty = 0.0;
		gbc.gridx = 0;
		gbc.gridy = 0;
		//add(labelFind, c);

		panelHeader = new JPanel();
		panelHeader.setLayout(new BorderLayout());
		panelFind = new FindPanel();
		panelReplace = new ReplacePanel();
		panelHeader.add(panelReplace, BorderLayout.PAGE_START);
		panelHeader.add(panelFind, BorderLayout.PAGE_END);

		//getContentPane().add(panelHeader, BorderLayout.PAGE_START);
		getContentPane().add(panelHeader, gbc);
		getContentPane().setPreferredSize(new Dimension(600,800));

		CreateDocument(fullPath);
		CreateStyling(fullPath);
		for (int c=0;c<controls;c++) {
			gbc.weighty = 1.0;
			gbc.gridy = c+1;
			gbc.fill = GridBagConstraints.BOTH;
			tc[c].setDoubleBuffered(true);
			//getContentPane().add(tc[c], BorderLayout.CENTER);
			getContentPane().add(tc[c], gbc);
			tc[c].GrabFocus();
		}
	}

	void GrabFile(String name) {
		try {
			fullPath = name;
			CreateDocument(fullPath);
			CreateStyling(fullPath);
			FileInputStream fsr = new FileInputStream(name);
			NewDocument();
			try {
				byte[] data = new byte[32768];
				int count = fsr.read(data, 0, 32768);
				while (count > 0) {
					ControlActive().AddContents(data, count);
					count = fsr.read(data, 0, 32768);
				}
				pdoc.pbuffer.DeleteUndoHistory();
			} catch (IOException x) {
			}
		} catch (FileNotFoundException x) {
		}
	}

	int SearchText(Literal text, int start, int end) {
		int findLength = pdoc.pbuffer.LengthInText(text.Data(), text.Length());
		int last = end - findLength;
		for (int look=start; look<last; look = pdoc.pbuffer.PositionNext(look)) {
			boolean found = true;
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
		int startEnd = java.lang.Math.max(
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
			int start = ControlActive().textArea.sr.StartSelection();
			ControlActive().textArea.DeleteRange(start, ControlActive().textArea.sr.EndSelection() - start);
			int len = ControlActive().textArea.InsertUTF32(start, replaceWhat.Data(), replaceWhat.Length());
			ControlActive().SetSelection(start, start + len);
			ControlActive().textArea.Invalidate();
		}
		SearchFindNext();
	}

	void SearchFind() {
		SelectPane(1);
		haveFound = false;
		Literal wordAtCaret = ControlActive().textArea.WordAtPosition(ControlActive().textArea.sr.Caret());
		EntryFromLiteral(wordAtCaret, panelFind.textFind);
		panelFind.textFind.requestFocus();
		ControlActive().EnsureCaretVisible();
	}

	void SearchReplace() {
		SelectPane(2);
		haveFound = false;
		Literal wordAtCaret = ControlActive().textArea.WordAtPosition(ControlActive().textArea.sr.Caret());
		EntryFromLiteral(wordAtCaret, panelReplace.textFind);
		panelReplace.textFind.requestFocus();
		ControlActive().EnsureCaretVisible();
	}

	void ReplaceAll(boolean selectionRange) {
		int startPosition = selectionRange ? ControlActive().textArea.sr.StartSelection() : 0;
		int endPosition = selectionRange ? ControlActive().textArea.sr.EndSelection() : pdoc.pbuffer.Length();
		int findLength = pdoc.pbuffer.LengthInText(findWhat.Data(), findWhat.Length());
		if (findLength == 0)
			return;
		int nextMatch = SearchText(findWhat, startPosition, endPosition);
		while (nextMatch >= 0) {
			ControlActive().textArea.DeleteRange(nextMatch, findLength);
			int replaceLength =
				ControlActive().textArea.InsertUTF32(nextMatch, replaceWhat.Data(), replaceWhat.Length());
			startPosition = nextMatch + replaceLength;
			endPosition += replaceLength - findLength;
			nextMatch = SearchText(findWhat, startPosition, endPosition);
		}
		InvalidateAll();
	}

	void SelectPane(int pane) {
		if (pane != panesVisible) {
			switch (panesVisible) {
			case 1:
				panelFind.setVisible(false);
				break;
			case 2:
				panelReplace.setVisible(false);
				break;
			}
			switch (pane) {
			case 1:
				panelFind.setVisible(true);
				break;
			case 2:
				panelReplace.setVisible(true);
				break;
			}
			panesVisible = pane;
			if (pane == 0) {
				ControlActive().GrabFocus();
			}
			pack();
		}
	}
}

public class TentacleJ {

	private static void GUI(String[] args) {
		PrototypeRegistry.Init();
		RegisteredDecorations.Init();
		RegisteredLexers.Init();
		String fileToEdit = "TentacleJ.java";
		//~ String fileToEdit = "uni.py";
		for (int i=0;i<args.length;i++) {
			if (args[i].startsWith("-")) {
			} else {
				fileToEdit = args[i];
			}
		}
		final TentacleJApplication theTentacle = new TentacleJApplication();
		theTentacle.addWindowListener(
			new WindowAdapter() {
				public void windowClosing(WindowEvent event) {
					theTentacle.Exit();
				}
			});
		theTentacle.init();
		theTentacle.pack();
		theTentacle.setVisible(true);
		theTentacle.GrabFile(fileToEdit);
		theTentacle.SetTitle(true);
	}

	static public void main(final String[] args) {
		javax.swing.SwingUtilities.invokeLater(new Runnable() {
			public void run() {
				GUI(args);
			}
		});
	}
};
