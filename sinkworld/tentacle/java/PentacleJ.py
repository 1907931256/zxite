"""A Jython version of the Pentacle editor
"""

from pawt import swing
import java.awt
from java.awt.event.KeyEvent import VK_UP, VK_DOWN
from javax.swing import KeyStroke
from java.awt.event import KeyEvent
from java.awt.event import ActionEvent
from java.awt import GridBagLayout, GridBagConstraints, Insets
from java.lang import Thread, System
from pawt.swing import JMenuBar, JMenu, JMenuItem, JCheckBoxMenuItem
from pawt.swing import JOptionPane, JFileChooser, JPanel
import java.io
import sys
import jarray

from SinkWorld import *

from Pentacle import Pentacle
sys.path.append("../python")
import PentacleMenu

def StringFromLiteral(lit):
	return str(java.lang.String(lit.AsUTF8()))

def LiteralFromUnicode(u):
	return Literal(u)

def LiteralFromEntry(s, w):
	try:
		bytes = w.getText()
		s.SetFromUTF8(bytes, len(bytes))
	except java.io.UnsupportedEncodingException, x:
		s.Set("")

def EntryFromLiteral(s, w):
	w.setText(StringFromLiteral(s))

def MnemonicValue(menu):
	return KeyEvent.VK_A + (ord(menu[menu.find("&") + 1].upper()) - ord("A"))

class StripPanel(swing.JPanel, java.awt.event.ActionListener):
	""" A dialog that appears at the top of the main window. """
	def __init__(self, app):
		JPanel.__init__(self)
		self.app = app
		self.setLayout(GridBagLayout())
		self.visible = 0

	def Set(self, description):
		lines = 0
		self.texts = []
		c = GridBagConstraints()
		c.fill = GridBagConstraints.HORIZONTAL
		c.ipadx = 2
		c.ipady = 1
		c.insets = java.awt.Insets(2,2,2,2)
		for line in description:
			lines += 1
			for widget in range(len(line)):
				text = line[widget]
				c.weightx = 0.0
				c.gridx = widget
				c.gridy = lines-1
				if widget == 0:
					w = swing.JLabel(text.replace("&", ""))
				elif widget == 1:
					w = swing.JTextField(20)
					self.texts.append(w)
					c.weightx = 0.5
					w.addActionListener(self)
				else:
					w = swing.JButton(text.replace("&", ""))
					w.setMnemonic(MnemonicValue(text))
					w.addActionListener(self)
				self.add(w, c)

	def Start(self, text):
		EntryFromLiteral(text, self.texts[0])
		self.texts[0].requestFocus()

class FindPanel(StripPanel):

	def __init__(self, app):
		StripPanel.__init__(self, app)
		self.Set([[u"Find &what:", u"", u"Fin&d"]])

	def actionPerformed(self, e):
		command = e.getActionCommand()
		LiteralFromEntry(self.app.findWhat, self.texts[0])
		self.app.SearchFindNext()

class ReplacePanel(StripPanel):

	def __init__(self, app):
		StripPanel.__init__(self, app)
		self.Set(
			[[u"Find &what:", u"", u"Fin&d", u"Replace &All"],
			[u"Replace wit&h:", u"", u"&Replace", u"&In Selection"]])

	def actionPerformed(self, e):
		LiteralFromEntry(self.app.findWhat, self.texts[0])
		#~ print command, e.getSource()
		if e.getSource() in self.texts:
			# Pressed Enter inside a text field
			if e.getSource() == self.texts[0]:
				self.app.SearchFindNext()
			else:
				LiteralFromEntry(self.app.replaceWhat, self.texts[1])
				self.app.ReplaceOnce()
		else:
			# Pressed a button
			command = e.getActionCommand()
			if command != "Find":
				LiteralFromEntry(self.app.replaceWhat, self.texts[1])
			if command == "Find":
				self.app.SearchFindNext()
			elif command == "Replace":
				self.app.ReplaceOnce()
			elif command == "Replace All":
				self.app.ReplaceAll(0)
			elif command == "In Selection":
				self.app.ReplaceAll(1)

class PentacleApplication(swing.JFrame, Pentacle, IDocumentChangeListener, \
	java.awt.event.ActionListener):

	def __init__(self):
		self.fontNames = LiteralSet()
		self.pdoc = None
		self.titleAtSavePoint = 1
		self.fullPath = ""
		self.overrideMode = None
		self.panesVisible = 0
		self.panelHeader = None
		self.panelFind = None
		self.panelReplace = None
		self.haveFound = 0
		self.findWhat = Literal("")
		self.replaceWhat = Literal("")
		self.controls = 1
		self.activeControl = 0

		self.menuItemWrap = None
		self.appName = "PentacleJ"
		swing.JFrame.__init__(self, self.appName, size=(512, 700), windowClosing=self.exit)
		self.pdoc = Document(Surface.fontDefault, self.fontNames)
		self.tc = [None] * self.controls
		for c in range(self.controls):
			self.tc[c] = TentacleControl()
			self.tc[c].SetDocument(self.pdoc, self)

	def ControlActive(self):
		return self.tc[self.activeControl]

	def Deallocate(self):
		self.DeallocateStyling()
		pdoc = None
		for t in self.tc:
			t.Deallocate()
		self.overrideMode = None

	def DeallocateStyling(self):
		self.pdoc.DeallocateStyling()

	def CreateDocument(self, name):
		self.Deallocate()
		self.pdoc = Document(self.fontNames.FindOrAddKey(Surface.fontDefault), self.fontNames)

	def ReadOptionsFile(self, prefix, filename):
		fsr = open(filename, "rb")
		try:
			propsData = fsr.read()
			self.pdoc.options.SetFromText(prefix, propsData, 0, len(propsData))
			return 1
		except IOException, x:
			pass
		return 0

	def ReadOptionsAndModes(self):
		self.pdoc.ClearOptions()
		homeDir = System.getProperty("user.home") + System.getProperty("file.separator")
		userOptions = homeDir + "user.options"
		if not self.ReadOptionsFile(None, userOptions):
			System.err.println("No options file: " + userOptions)
			System.exit(1)
		myDir = java.io.File(homeDir)
		contents = myDir.listFiles()
		for f in contents:
			name = f.getName()
			if name.endswith(".mode"):
				baseName = name[:- 5]
				modeName = Literal("mode." + baseName)
				self.ReadOptionsFile(modeName, homeDir + name)

	def OptionsFromSuffix(self, suffix):
		opts = {}
		if self.pdoc and self.pdoc.options:
			literalSuffix = Literal(suffix)
			keys = self.pdoc.options.Keys(literalSuffix)
			for i in range(keys.Length()):
				k = keys.GetText(i)
				opts[StringFromLiteral(k)[:-len(suffix)]] = StringFromLiteral(self.pdoc.options.Value(k))
		return opts

	def ModeNames(self):
		return self.OptionsFromSuffix(".name")

	def SetOverrideMode(self, mode):
		self.overrideMode = LiteralFromUnicode(mode)
		self.CreateStyling("X.XXXXX")
		self.CheckMenus()

	def WordAtCaret(self):
		word = Literal()
		self.ControlActive().WordAtPosition(\
			self.ControlActive().textArea.sr.Caret(), word)
		return word

	def CreateStyling(self, name):
		self.DeallocateStyling()

		literalName = Literal(name)
		dot = literalName.Length()
		while (dot > 0) and literalName.CharAt(dot-1) != ord('.'):
			dot -= 1
		slen = literalName.Length() - dot
		literalExtension = Literal(literalName, dot, literalName.Length() - dot)
		self.ReadOptionsAndModes()
		if self.overrideMode:
			print "Override Mode", self.overrideMode, literalExtension
			self.pdoc.currentMode = self.overrideMode
			self.pdoc.CreateStyling(Literal(""))
		else:
			#~ print "Base Mode", literalExtension
			self.pdoc.currentMode = Literal("")
			self.pdoc.CreateStyling(literalExtension)
		for t in self.tc:
			t.SetDocument(self.pdoc, self)

	def SetTitle(self, changePath):
		if changePath or self.titleAtSavePoint != self.pdoc.pbuffer.IsSavePoint():
			self.titleAtSavePoint = self.pdoc.pbuffer.IsSavePoint()
			title = self.fullPath
			if self.titleAtSavePoint:
				title += " - "
			else:
				title += " * "
			title += self.appName
			self.setTitle(title)

	def NewDocument(self):
		for t in self.tc:
			t.InitScrollBars()
		self.pdoc.pbuffer.DiscardLines()
		self.pdoc.pbuffer.DeleteRange(0, self.pdoc.pbuffer.Length())
		self.tc[0].GrabFocus()

	def SaveIfUnsure(self):
		if not self.pdoc.pbuffer.IsSavePoint():
			msg = "Save changes to \"" + self.fullPath + "\"?"
			decision = JOptionPane.showConfirmDialog(self, \
				msg, self.appName, JOptionPane.YES_NO_CANCEL_OPTION)
			if decision == JOptionPane.YES_OPTION:
				Save()
			return decision != JOptionPane.CANCEL_OPTION
		return 1

	def New(self):
		if self.SaveIfUnsure():
			self.fullPath = ""
			self.overrideMode = None
			self.CreateDocument(self.fullPath)
			self.CreateStyling(self.fullPath)
			self.NewDocument()
			self.SetTitle(1)
		self.InvalidateAll()

	def GrabFile(self, name):
		self.fullPath = name
		self.CreateDocument(self.fullPath)
		self.CreateStyling(self.fullPath)
		self.NewDocument()
		try:
			fsr = open(name)
			data = fsr.read()
			self.ControlActive().AddContents(data, len(data))
			self.pdoc.pbuffer.DeleteUndoHistory()
		except IOError:
			pass	# OK to try to open file that does not exist
		except IOException:
			pass
		self.ControlActive().SetSelection(0, 0)

	def Open(self):
		if self.SaveIfUnsure():
			chooser = JFileChooser(".")
			returnVal = chooser.showOpenDialog(self)
			if returnVal == JFileChooser.APPROVE_OPTION:
				self.GrabFile(chooser.getSelectedFile().getAbsolutePath())
				self.SetTitle(1)
				self.InvalidateAll()

	def Save(self):
		#~ print "Saving", self.fullPath
		fos = open(self.fullPath, "wb")
		blockSize = 1024
		length = self.pdoc.pbuffer.Length()
		i = 0
		pd = jarray.zeros(blockSize, "b")
		while i < length:
			grabSize = length - i
			if grabSize > blockSize:
				grabSize = blockSize
			#~ print i, grabSize, length
			self.pdoc.pbuffer.RetrieveBytes(i, pd, grabSize)
			data = "".join([chr(pd[j]) for j in range(grabSize)])
			fos.write(data)
			i += grabSize
		fos.close()
		self.pdoc.pbuffer.SetSavePoint()
		self.SetTitle(0)

	def SaveAs(self):
		chooser = JFileChooser(".")
		returnVal = chooser.showSaveDialog(self)
		if returnVal == JFileChooser.APPROVE_OPTION:
			self.fullPath = chooser.getSelectedFile().getAbsolutePath()
			if self.fullPath is not None:
				self.Save()
				self.SetTitle(1)

	def actionPerformed(self, e):
		command = e.getActionCommand()
		id = e.getActionCommand()
		id = id.replace(" ", "")
		id = id.replace("...", "")
		method = "Cmd" + id
		#~ print 'actionPerformed', method, "[%s]" % command
		cmd = getattr(self, method, "")
		if not cmd:
			modes = self.ModeNames()
			for k in modes.keys():
				v = modes[k]
				if v.replace(u" ", u"") == id:
					self.SetOverrideMode(k)
					self.ControlActive().textArea.Invalidate()
					return
		if cmd:
			cmd()

	# Command handlers are called by menu actions

	def CmdNew(self):
		self.New()

	def CmdOpen(self):
		self.Open()

	def CmdSave(self):
		if (self.fullPath == None) or (len(self.fullPath) == 0):
			self.SaveAs()
		else:
			self.Save()

	def CmdSaveAs(self):
		self.SaveAs()

	def CmdExit(self):
		self.Exit()

	def CmdUndo(self):
		self.ControlActive().Undo()

	def CmdRedo(self):
		self.ControlActive().Redo()

	def CmdCut(self):
		self.ControlActive().Cut()

	def CmdCopy(self):
		self.ControlActive().Copy()

	def CmdPaste(self):
		self.ControlActive().Paste()

	def CmdDelete(self):
		self.ControlActive().Clear()

	def CmdSelectAll(self):
		self.ControlActive().SelectAll()

	def CmdFold(self):
		self.ControlActive().ToggleFold()

	def CmdFoldAll(self):
		self.ControlActive().FoldAll(TentacleControl.fcFold)

	def CmdShowAll(self):
		self.ControlActive().FoldAll(TentacleControl.fcShow)

	def CmdFind(self):
		self.SearchFind()

	def CmdFindNext(self):
		if self.findWhat == "":
			self.SearchFind()
		else:
			self.SearchFindNext()

	def CmdReplace(self):
		self.SearchReplace()

	def CmdHide(self):
		self.SearchHide()

	def CmdWrap(self):
		self.ControlActive().ToggleWrap()
		self.CheckMenus()

	def CheckMenus(self):
		#~ self.menuItemWrap.setState(self.ControlActive().wrap)
		pass

	def SetUpMenus(self):
		modes = self.ModeNames()
		modeMenu = [[n, ""] for n in modes.values()]
		modeMenu.sort()
		ui = PentacleMenu.MenuStructure
		ui.append([ "&Mode", modeMenu ])
		self.cmds = {}
		self.keys = {}
		id = 0
		menuBar = JMenuBar()
		for name, contents in ui:
			id += 1
			menu = JMenu(name.replace("&", ""))
			menu.setMnemonic(MnemonicValue(name))
			for item in contents:
				text, key = item
				textShow = text.replace("&", "")
				cmdText = text.replace("...", "")
				cmdText = cmdText.replace(" ", "")
				id += 1
				keyVal = None
				if key:
					keyText = key.replace("<control>", "control ")
					keyText = keyText.replace("<shift>", "shift ")
					keyText = keyText.replace("Del", "DELETE")
					keyText = keyText.replace("Esc", "ESCAPE")
					keyVal = KeyStroke.getKeyStroke(keyText)
					#~ print keyText, keyVal
				if text == "-":
					menu.addSeparator()
				else:
					menuItem = JMenuItem(textShow)
					menuItem.setMnemonic(MnemonicValue(item[0]))
					if keyVal is not None:
						menuItem.setAccelerator(keyVal)
					menuItem.addActionListener(self)
					menu.add(menuItem)
				self.cmds[id] = cmdText
				self.keys[key] = cmdText
			menuBar.add(menu)

		self.setJMenuBar(menuBar)

	def ConstructUI(self):
		swing.UIManager.setLookAndFeel(swing.UIManager.getSystemLookAndFeelClassName())
		#~ swing.UIManager.setLookAndFeel("javax.swing.plaf.metal.MetalLookAndFeel")

		self.getContentPane().setLayout(GridBagLayout())
		gbc = GridBagConstraints()

		gbc.fill = GridBagConstraints.HORIZONTAL
		gbc.ipadx = 0
		gbc.ipady = 0
		gbc.insets = Insets(0,0,0,0)

		gbc.weightx = 1.0
		gbc.weighty = 0.0
		gbc.gridx = 0
		gbc.gridy = 0

		self.panelHeader = swing.JPanel()
		self.panelHeader.setLayout(java.awt.BorderLayout())
		self.panelReplace = ReplacePanel(self)
		self.panelHeader.add(self.panelReplace, java.awt.BorderLayout.PAGE_START)
		self.panelFind = FindPanel(self)
		self.panelHeader.add(self.panelFind, java.awt.BorderLayout.PAGE_END)

		self.getContentPane().add(self.panelHeader, gbc)

		self.CreateDocument(self.fullPath)
		self.CreateStyling(self.fullPath)

		self.SetUpMenus()

		for c in range(self.controls):
			gbc.weighty = 1.0
			gbc.gridy = c+1
			gbc.fill = GridBagConstraints.BOTH
			self.tc[c].setDoubleBuffered(1)
			self.getContentPane().add(self.tc[c], gbc)
			self.tc[c].GrabFocus()

	def StateAltered(self):
		c = 0
		for t in self.tc:
			if t.textArea.hasFocus():
				self.activeControl = c
			if t.wrap:
				t.StartWrap()
			c += 1
		self.SetTitle(0)
		self.CheckMenus()

	def InvalidateAll(self):
		for t in self.tc:
			t.textArea.Invalidate()

	def InvalidateRange(self, a, b):
		for t in self.tc:
			t.InvalidateRange(a,b)

	def SearchText(self, text, start, end):
		findLength = self.pdoc.pbuffer.LengthInText(text.Data(), text.Length())
		last = end - findLength
		look = start
		while look < last:
			found = 1
			check = look
			j = 0
			while j < findLength and found:
				if self.pdoc.pbuffer.UnsignedByteAt(look+j) != text.CharAt(j):
					found = 0
				check = self.pdoc.pbuffer.PositionNext(check)
				j += 1
			if found:
				return look
			look = self.pdoc.pbuffer.PositionNext(look)
		return -1

	def SearchFindNext(self):
		self.haveFound = 0
		docLength = self.pdoc.pbuffer.Length()
		startEnd = java.lang.Math.max(
			self.ControlActive().textArea.sr.Anchor(), self.ControlActive().textArea.sr.Caret())
		next = self.SearchText(self.findWhat, startEnd, docLength)
		if next < 0:
			next = self.SearchText(self.findWhat, 0, startEnd)
		if next >= 0:
			findLength = self.pdoc.pbuffer.LengthInText(self.findWhat.Data(), self.findWhat.Length())
			self.ControlActive().SetSelection(next + findLength, next)
			self.ControlActive().EnsureCaretVisible()
			self.ControlActive().textArea.Invalidate()
			self.haveFound = 1

	def ReplaceOnce(self):
		if self.haveFound:
			start = self.ControlActive().textArea.sr.StartSelection()
			self.pdoc.pbuffer.BeginUndoAction()
			self.ControlActive().textArea.DeleteRange(start, self.ControlActive().textArea.sr.EndSelection() - start)
			replaceLength = self.ControlActive().textArea.InsertUTF32(\
				start, self.replaceWhat.Data(), self.replaceWhat.Length())
			self.pdoc.pbuffer.EndUndoAction()
			self.ControlActive().SetSelection(start + replaceLength, start)
			self.ControlActive().textArea.Invalidate()
		self.SearchFindNext()

	def ReplaceAll(self, selectionRange):
		if selectionRange:
			startPosition = self.ControlActive().textArea.sr.StartSelection()
			endPosition = self.ControlActive().textArea.sr.EndSelection()
		else:
			startPosition = 0
			endPosition = self.pdoc.pbuffer.Length()
		findLength = self.pdoc.pbuffer.LengthInText(self.findWhat.Data(), self.findWhat.Length())
		if findLength == 0:
			return
		nextMatch = self.SearchText(self.findWhat, startPosition, endPosition)
		self.pdoc.pbuffer.BeginUndoAction()
		while nextMatch >= 0:
			self.ControlActive().textArea.DeleteRange(nextMatch, findLength)
			replaceLength = \
				self.ControlActive().textArea.InsertUTF32(nextMatch, \
					self.replaceWhat.Data(), self.replaceWhat.Length())
			startPosition = nextMatch + replaceLength
			endPosition += replaceLength - findLength
			nextMatch = self.SearchText(self.findWhat, startPosition, endPosition)
		self.pdoc.pbuffer.EndUndoAction()
		self.InvalidateAll()

	def SearchFind(self):
		self.SelectPane(1)
		self.haveFound = 0
		wordAtCaret = Literal()
		self.ControlActive().textArea.WordAtPosition(\
			self.ControlActive().textArea.sr.Caret(), wordAtCaret)
		self.panelFind.Start(wordAtCaret)
		self.ControlActive().EnsureCaretVisible()

	def SearchReplace(self):
		self.SelectPane(2)
		self.haveFound = 0
		wordAtCaret = Literal()
		self.ControlActive().textArea.WordAtPosition(\
			self.ControlActive().textArea.sr.Caret(), wordAtCaret)
		self.panelReplace.Start(wordAtCaret)
		self.ControlActive().EnsureCaretVisible()

	def SearchHide(self):
		self.SelectPane(0)

	def SelectPane(self, pane):
		if pane != self.panesVisible:
			if self.panesVisible == 1:
				self.panelFind.visible = 0
			elif self.panesVisible == 2:
				self.panelReplace.visible = 0
			if pane == 1:
				self.panelFind.visible = 1
			elif pane == 2:
				self.panelReplace.visible = 1
			self.panesVisible = pane
			if pane == 0:
				self.ControlActive().GrabFocus()

	def exit(self, event):
		System.exit(0)

	def Exit(self):
		if self.SaveIfUnsure():
			System.exit(0)

class RunFromPentacle(java.lang.Runnable):

	def run(self):
		RegisteredDecorations.Init()
		RegisteredLexers.Init()
		fileToEdit = "TentacleJ.java"
		for a in sys.argv[1:]:
			fileToEdit = s
		thePentacle = PentacleApplication()
		thePentacle.ConstructUI()
		thePentacle.visible = 1
		thePentacle.GrabFile(fileToEdit)
		thePentacle.SetTitle(1)

if __name__ == '__main__':
	swing.SwingUtilities.invokeLater(RunFromPentacle())
