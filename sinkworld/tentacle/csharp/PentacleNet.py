"""An IronPython version of the Tentacle editor
"""

import sys
import clr

clr.AddReference("System")
clr.AddReference("System.Drawing")
clr.AddReference("System.Windows.Forms")

clr.AddReferenceToFile("./SinkWorld.dll")

from SinkWorld import *

from Pentacle import uiBase, Pentacle

from System import *
from System.Drawing import *
from System.Text import *
from System.Windows.Forms import *

PrototypeRegistry.Init()
RegisteredDecorations.Init()
RegisteredLexers.Init()

def BytesFromString(s):
	l = len(s)
	pd = Array.CreateInstance(Byte, l)
	for i in range(l):
		pd[i] = ord(s[i])
	return pd

def StringFromLiteral(lit):
	return Encoding.UTF8.GetString(lit.AsUTF8())

def LiteralFromUnicode(u):
	return Literal(u)

def LiteralFromEntry(w):
	return Literal(w.Text)

def EntryFromLiteral(s, w):
	w.Text = StringFromLiteral(s)

buttonWidth = 80
buttonHeight = 20
pad = 3

class StripPanel(UserControl):
	""" A dialog that appears at the top of the main window. """
	def __init__(self, app):
		self.app = app
		self.SizeChanged += self.SizeChangedEvent
		self.Visible = 0

	def Set(self, description):
		self.description = description
		height = len(description)
		self.heightDesired = height * (buttonHeight + pad) + pad
		lines = 0
		self.texts = []
		self.windows = []
		for line in description:
			self.windows.append([])
			top = lines * (buttonHeight + pad) + pad
			for widget in range(len(line)):
				text = line[widget]
				if widget == 0:
					w = Label()
					w.Text = text#.replace("&", "")
					w.Top = top + 2
					w.Left = 5
					w.Width = 72
				elif widget == 1:
					w = TextBox()
					self.texts.append(w)
					w.Top = top
					w.Left = 80
					w.Size = Size(100,20)
					# This key handling allows Escape and Enter to hide or perform
					# but still cause a beep. Should also not be needed as the form
					# should be able to see this and perform default button handling.
					#~ w.KeyDown += self.KeyHit
				else:
					w = Button()
					w.Text = text#.replace("&&", "")
					w.Top = top
					w.Left = 200 + (widget - 2) * 100
					w.Size = Size(buttonWidth, buttonHeight)
					w.Click += self.Action
				self.Controls.Add(w)
				self.windows[lines].append(w)
			lines += 1

	def Start(self, text):
		EntryFromLiteral(text, self.texts[0])
		self.AcceptButton = self.windows[0][2]
		self.texts[0].Focus()

	def Lines(self):
		return len(self.description)

	def Buttons(self):
		return len(self.description[0]) - 2

	def SizeChangedEvent(self, sender, e):
		self.OnSizeChanged(e)

	def OnSizeChanged(self, e):
		for line in range(self.Lines()):
			widthText = self.ClientSize.Width - self.windows[line][1].Left - \
				self.Buttons() * (buttonWidth + pad) - pad
			if widthText < 10:
				widthText = 10
			self.windows[line][1].Width = widthText
			for b in range(self.Buttons()):
				self.windows[line][b + 2].Left = self.windows[line][1].Left + \
					widthText + pad + b * (buttonWidth + pad)

class FindPanel(StripPanel):
	def __init__(self, app):
		StripPanel.__init__(self, app)
		self.Set([[u"Find &what:", u"", u"Fin&d"]])

	def Action(self, sender, e):
		command = sender.Text.replace("&", "")
		self.app.findWhat = LiteralFromEntry(self.texts[0])
		if command == "Find":
			self.app.SearchFindNext()

	def KeyHit(self, sender, e):
		if e.KeyValue == 13:
			self.app.findWhat = LiteralFromEntry(self.texts[0])
			if sender in self.texts:
				# Pressed Enter inside a text field
				if sender == self.texts[0]:
					self.app.SearchFindNext()
					e.Handled = 1
		elif e.KeyValue == 27:
			self.app.SearchHide()
			e.Handled = 1

class ReplacePanel(StripPanel):
	def __init__(self, app):
		StripPanel.__init__(self, app)
		self.Set(
			[[u"Find &what:", u"", u"Fin&d", u"Replace &All"],
			[u"Replace wit&h:", u"", u"&Replace", u"&In Selection"]])

	def Action(self, sender, e):
		command = sender.Text.replace("&", "")
		self.app.findWhat = LiteralFromEntry(self.texts[0])
		if command != "Find":
			self.app.replaceWhat = LiteralFromEntry(self.texts[1])
		if command == "Find":
			self.app.SearchFindNext()
		elif command == "Replace":
			self.app.ReplaceOnce()
		elif command == "Replace All":
			self.app.ReplaceAll(0)
		elif command == "In Selection":
			self.app.ReplaceAll(1)

	def KeyHit(self, sender, e):
		if e.KeyValue == 13:
			print "xEnter"
			if sender in self.texts:
				self.app.findWhat = LiteralFromEntry(self.texts[0])
				# Pressed Enter inside a text field
				e.Handled = 1
				if sender == self.texts[0]:
					self.app.SearchFindNext()
				else:
					self.app.replaceWhat = LiteralFromEntry(self.texts[1])
					self.app.ReplaceOnce()
		elif e.KeyValue == 27:
			self.app.SearchHide()
			e.Handled = 1

class TentaclePApplication(Form):
	def __init__(self):
		self.blockSize = 32768

		self.fontNames = LiteralSet()
		self.pdoc = None

		self.IDCLThunk = IDCL()
		self.IDCLThunk.StateAlteredEvent += self.StateAltered
		self.IDCLThunk.InvalidateAllEvent += self.InvalidateAll
		self.IDCLThunk.InvalidateRangeEvent += self.InvalidateRange

		self.titleAtSavePoint = 1
		self.fullPath = ".\\TentacleN.cs"
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

		self.topPanelHeight = 0

		self.appName = "TentacleN"
		self.pdoc = Document(Surface.fontDefault, self.fontNames)
		self.tc = [None] * self.controls
		for c in range(self.controls):
			self.tc[c] = TentacleControl()
			self.tc[c].SetDocument(self.pdoc, self.IDCLThunk)
		self.ConstructUI()
		self.Size = Size(600, 800)
		self.Show()
		self.GrabFile(self.fullPath)
		self.SetTitle(1)

	def ControlActive(self):
		return self.tc[0]

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
		fsr = open(filename, "r")
		propsData = fsr.read()
		fsr.close()
		opts = self.pdoc.options
		#~ print opts, dir(opts)
		pd = BytesFromString(propsData)
		opts.SetFromText(prefix, pd, 0, len(pd))
		return 1

	def HomeDirectory(self):
		return Environment.GetEnvironmentVariable("USERPROFILE")

	def ReadOptionsAndModes(self):
		self.pdoc.ClearOptions()
		homeDir = self.HomeDirectory() + "\\"
		userOptions = homeDir + "user.options"
		if not self.ReadOptionsFile(Literal(""), userOptions):
			System.err.println("No options file: " + userOptions)
			System.exit(1)
		dir = IO.DirectoryInfo(homeDir)
		modeFiles = dir.GetFiles("*.mode")
		for f in modeFiles:
			name = f.Name
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
		word = self.ControlActive().WordAtPosition(\
			self.ControlActive().textArea.sr.Caret())
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
			print "Base Mode", literalExtension
			self.pdoc.currentMode = Literal("")
			self.pdoc.CreateStyling(literalExtension)
		for t in self.tc:
			t.SetDocument(self.pdoc, self.IDCLThunk)

	def SetTitle(self, changePath):
		self.titleAtSavePoint = self.pdoc.pbuffer.IsSavePoint()
		self.title = self.fullPath
		if self.titleAtSavePoint:
			self.title += " - "
		else:
			self.title += " * "
		self.title += self.appName

	def NewDocument(self):
		for t in self.tc:
			t.InitScrollBars()
		self.pdoc.pbuffer.DiscardLines()
		self.pdoc.pbuffer.DeleteRange(0, self.pdoc.pbuffer.Length())
		self.tc[0].GrabFocus()

	def SaveIfUnsure(self):
		if not self.pdoc.pbuffer.IsSavePoint():
			msg = u"Save changes to \"" + self.fullPath + u"\"?"
			decision = self.DisplayMessage(msg, 1)
			if decision:
				self.CmdSave()
			return decision
		return 1

	def New(self):
		if self.SaveIfUnsure():
			self.fullPath = ""
			self.overrideMode = None
			self.CreateDocument(self.fullPath)
			self.CreateStyling(self.fullPath)
			self.NewDocument()
			self.SetTitle(1)
		self.Invalidate()

	def GrabFile(self, name):
		self.fullPath = name
		self.overrideMode = None
		self.CreateDocument(self.fullPath)
		self.CreateStyling(self.fullPath)
		self.NewDocument()
		try:
			fsr = open(name, "rb")
			data = fsr.read()
			fsr.close()
			data = BytesFromString(data)
			self.ControlActive().AddContents(data, len(data))
		except IO.FileNotFoundException:
			# File not present is OK.
			pass
		self.pdoc.pbuffer.DeleteUndoHistory()
		self.ControlActive().SetSelection(0, 0)
		self.SetTitle(1)
		self.Invalidate()

	def Open(self):
		filters = self.OptionsFromSuffix(".filter").values()
		filters.sort()
		filterText = "|".join(filters)
		chooser = OpenFileDialog()
		chooser.InitialDirectory = "."
		chooser.Filter = filterText
		returnVal = chooser.ShowDialog()
		if returnVal == DialogResult.OK:
			self.GrabFile(chooser.FileName)
			self.SetTitle(1)
			self.InvalidateAll()
		self.FocusText()

	def OpenOptions(self):
		chooser = OpenFileDialog()
		chooser.InitialDirectory = self.HomeDirectory()
		chooser.Filter = StringFromLiteral(self.pdoc.options.Value(Literal(u"mode.base.settings.filter")))
		returnVal = chooser.ShowDialog()
		if returnVal == DialogResult.OK:
			self.GrabFile(chooser.FileName)
			self.SetTitle(1)
			self.InvalidateAll()
		self.FocusText()

	def Save(self):
		print "Saving", self.fullPath
		fos = open(self.fullPath, "wb")
		length = self.pdoc.pbuffer.Length()
		i = 0
		pd = Array.CreateInstance(Byte, self.blockSize + 1)
		while i < length:
			grabSize = length - i
			if grabSize > self.blockSize:
				grabSize = self.blockSize
			#~ print i, grabSize, length
			self.pdoc.pbuffer.RetrieveBytes(i, pd, grabSize)
			data = "".join([chr(pd[j]) for j in range(grabSize)])
			fos.write(data)
			i += grabSize
		fos.close()
		self.pdoc.pbuffer.SetSavePoint()
		self.SetTitle(0)

	def SaveAs(self):
		chooser = SaveFileDialog()
		chooser.InitialDirectory = "."
		chooser.FileName = self.fullPath
		returnVal = chooser.ShowDialog()
		if returnVal == DialogResult.OK:
			self.fullPath = chooser.FileName
			if self.fullPath:
				self.Save()
				self.SetTitle(1)
		self.FocusText()

	def MenuClick(self, sender, e):
		id = sender.Text.replace("&", "")
		id = id.replace(" ", "")
		id = id.replace("...", "")
		method = "Cmd" + id
		#~ print method
		cmd = getattr(self, method, "")
		if not cmd:
			for k,v in self.ModeNames().iteritems():
				if v.replace(u" ", u"") == id:
					self.SetOverrideMode(k)
					self.ControlActive().textArea.Invalidate()
					return
		if cmd:
			cmd()

	def CheckMenus(self):
		#~ menuItemWrap.Checked = ControlActive().wrap
		pass

	def SetUpMenus(self):
		modes = self.ModeNames()
		modeMenu = sorted([[n, ""] for n in modes.values()])
		ui = uiBase
		ui.append([ "&Mode", modeMenu ])
		self.cmds = {}
		self.keys = {}
		id = 0
		self.menuBar = MainMenu()
		for name, contents in ui:
			id += 1
			menu = MenuItem(name)
			for item in contents:
				text, key = item
				cmdText = text.replace("&", "")
				cmdText = cmdText.replace("...", "")
				cmdText = cmdText.replace(" ", "")
				id += 1
				keyVal = None
				if key:
					keyText = key.replace("Esc", "<control>E")
					keyText = keyText.replace("<control>", "Ctrl")
					keyText = keyText.replace("<shift>", "Shift")
					if keyText != "Esc":
						keyVal = Enum.Parse(Shortcut, keyText)
				if text == "-":
					menuItem = MenuItem("-")
				elif keyVal is not None:
					menuItem = MenuItem(text, self.MenuClick, keyVal)
				else:
					menuItem = MenuItem(text, self.MenuClick)
				menu.MenuItems.Add(menuItem)
				self.cmds[id] = cmdText
				self.keys[key] = cmdText
			self.menuBar.MenuItems.Add(menu)
		self.Menu = self.menuBar
		self.CheckMenus()

	def ConstructUI(self):
		self.panelHeader = Panel()
		self.panelFind = FindPanel(self)
		self.panelFind.Top = 0
		self.panelFind.Left = 0
		self.panelFind.Size = Size(400,30)
		self.panelReplace = ReplacePanel(self)
		self.panelHeader.Controls.Add(self.panelReplace)
		self.panelHeader.Controls.Add(self.panelFind)

		self.Controls.Add(self.panelHeader)
		self.panelHeader.Top = 0
		self.panelHeader.Left = 0
		self.panelHeader.Size = Size(400,30)

		self.CreateDocument(self.fullPath)
		self.CreateStyling(self.fullPath)
		for t in self.tc:
			self.Controls.Add(t)
			t.GrabFocus()
		self.SetUpMenus()
		self.SizeChanged += self.SizeChangedEvent

	def SizeChangedEvent(self, sender, e):
		self.OnSizeChanged(e)

	def OnSizeChanged(self, e):
		self.tc[0].Width = self.ClientSize.Width
		self.tc[0].Top = self.topPanelHeight
		self.tc[0].Height = self.ClientSize.Height - self.topPanelHeight
		self.panelFind.Width = self.ClientSize.Width
		self.panelFind.Height = self.topPanelHeight
		self.panelReplace.Width = self.ClientSize.Width
		self.panelReplace.Height = self.topPanelHeight
		self.panelHeader.Width = self.ClientSize.Width
		self.panelHeader.Height = self.topPanelHeight

	def StateAltered(self):
		c = 0
		for t in self.tc:
			if t.textArea.hasFocus():
				self.activeControl = t
			if t.wrap:
				t.StartWrap()
			c += 1
		self.SetTitle(0)
		self.CheckMenus()

	def Invalidate(self):
		for t in self.tc:
			t.textArea.Invalidate()
			#~ print "Invalidate all"

	def InvalidateAll(self):
		self.Invalidate()

	def InvalidateRange(self, a, b):
		for t in self.tc:
			t.InvalidateRange(a,b)
			#~ print "Invalidate", a, b

	def FocusText(self):
		self.tc[0].GrabFocus()

	# Command handlers are called by menu actions

	def CmdNew(self):
		self.New()

	def CmdOpen(self):
		self.Open()

	def CmdOpenOptions(self):
		self.OpenOptions()

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
		self.ControlActive().FoldAll(TentacleControl.FoldCommand.fcFold)

	def CmdShowAll(self):
		self.ControlActive().FoldAll(TentacleControl.FoldCommand.fcShow)

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
		startEnd = max(self.ControlActive().textArea.sr.Anchor(),
			self.ControlActive().textArea.sr.Caret())
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
		self.ControlActive().textArea.Invalidate()

	def SearchFind(self):
		self.SelectPane(1)
		self.haveFound = 0
		wordAtCaret = self.ControlActive().textArea.WordAtPosition(\
			self.ControlActive().textArea.sr.Caret())
		self.panelFind.Start(wordAtCaret)
		self.ControlActive().EnsureCaretVisible()

	def SearchReplace(self):
		self.SelectPane(2)
		self.haveFound = 0
		wordAtCaret = self.ControlActive().textArea.WordAtPosition(\
			self.ControlActive().textArea.sr.Caret())
		self.panelReplace.Start(wordAtCaret)
		self.ControlActive().EnsureCaretVisible()

	def SearchHide(self):
		self.SelectPane(0)

	def SelectPane(self, pane):
		if pane != self.panesVisible:
			if self.panesVisible == 1:
				self.panelFind.Visible = 0
			elif self.panesVisible == 2:
				self.panelReplace.Visible = 0
			if pane == 1:
				self.panelFind.Visible = 1
				self.topPanelHeight = self.panelFind.heightDesired
			elif pane == 2:
				self.panelReplace.Visible = 1
				self.topPanelHeight = self.panelReplace.heightDesired
			self.panesVisible = pane
			if pane == 0:
				self.topPanelHeight = 0
				self.ControlActive().GrabFocus()
			self.OnSizeChanged(None)

	def Exit(self):
		if self.SaveIfUnsure():
			Environment.Exit(0)

f = TentaclePApplication()
f.Visible = 0
f.ShowDialog()
