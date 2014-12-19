# -*- coding: utf-8 -*-

# Pentacle application superclass

try:
	import os
	import glob
except ImportError:
	class os:
		pass
	class glob:
		pass

import sys

import SinkWorld
from SinkWorld import Literal, LiteralSet

SinkWorld.PrototypeRegistry.Init()
SinkWorld.RegisteredDecorations.Init()
SinkWorld.RegisteredLexers.Init()

def LiteralFromUnicode(u):
	return Literal(u.encode("UTF-8"))

class Pentacle:
	def __init__(self):
		self.activeControl = None
		self.appName = u"Pentacle"
		self.fullPath = u""
		self.overrideMode = None
		self.win = None
		self.titleAtSavePoint = 1
		self.fontNames = LiteralSet()
		self.fontDefault = Literal("Verdana")
		self.pdoc = SinkWorld.Document(self.fontDefault, self.fontNames)
		self.tc = [SinkWorld.TentacleControl.Allocate()]
		self.tc[0].SetDocument(self.pdoc, self)
		self.ReadOptionsAndModes()

	def StateAltered(self):
		c = 0
		for t in self.tc:
			if t.HasFocus():
				self.activeControl = t
			#~ if t.wrap:
				#~ t.StartWrap()
			c += 1
		self.SetTitle(0)
		self.CheckMenus()

	def Invalidate(self):
		for t in self.tc:
			t.Invalidate()
			#~ print "Invalidate all"

	def InvalidateRange(self, a, b):
		for t in self.tc:
			t.InvalidateRange(a,b)
			#~ print "Invalidate", a, b

	def ControlActive(self):
		return self.activeControl

	def Deallocate(self):
		self.DeallocateStyling()
		for t in self.tc:
			t.Deallocate()

	def DeallocateStyling(self):
		self.pdoc.DeallocateStyling()

	def CreateDocument(self, name):
		self.Deallocate()
		self.pdoc = SinkWorld.Document(self.fontDefault, self.fontNames)

	def ReadOptionsFile(self, prefix, filename):
		fsr = open(filename, "rb")
		propsData = fsr.read()
		opts = self.pdoc.options
		opts.SetFromText(prefix, propsData, 0, len(propsData))
		return 1

	def ReadOptionsAndModes(self):
		self.pdoc.ClearOptions()
		try:
			homeDir = os.environ["USERPROFILE"]
		except KeyError:
			homeDir = os.environ["HOME"]
		userOptions = os.path.join(homeDir, "user.options")
		if not self.ReadOptionsFile(None, userOptions):
			print >>stderr, "No options file:", userOptions
			system.exit(1)
		for name in glob.glob(os.path.join(homeDir, "*")):
			if name.endswith(".mode"):
				baseName = os.path.basename(name)
				baseName = baseName[:-5]
				modeName = Literal("mode." + baseName)
				self.ReadOptionsFile(modeName, name)

	def OptionsFromSuffix(self, suffix):
		opts = {}
		if self.pdoc and self.pdoc.options:
			literalSuffix = Literal(suffix)
			keys = self.pdoc.options.Keys(literalSuffix)
			for i in range(keys.Length()):
				k = keys.GetText(i)
				opts[unicode(k)[:-len(suffix)]] = unicode(self.pdoc.options.Value(k))
		return opts

	def ModeNames(self):
		return self.OptionsFromSuffix(".name")

	def SetOverrideMode(self, mode):
		self.overrideMode = LiteralFromUnicode(mode)
		self.CreateStyling("X.XXXXX")
		self.CheckMenus()

	def CreateStyling(self, name):
		self.DeallocateStyling()

		literalName = LiteralFromUnicode(name)

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
			t.SetDocument(self.pdoc, self)

	def WordAtCaret(self):
		word = Literal()
		self.ControlActive().WordAtPosition(\
			self.ControlActive().GetSelectionRange().Caret(), word)
		return word

	def SetTitle(self, changePath):
		self.titleAtSavePoint = self.pdoc.pbuffer.IsSavePoint()
		self.title = self.fullPath
		if self.titleAtSavePoint:
			self.title += " - "
		else:
			self.title += " * "
		self.title += self.appName
		#~ print title, type(title), hex(self.win)
		#~ self.title = self.title.encode("UTF-8")

	def Command(self, id):
		id = id.replace(u" ", "")
		method = "Cmd" + id
		cmd = None
		try:
			cmd = getattr(self, method)
		except AttributeError:
			for k,v in self.ModeNames().iteritems():
				if v.replace(u" ", u"") == id:
					self.SetOverrideMode(k)
					self.ControlActive().Invalidate()
					return
		if cmd:
			cmd()

	def NewDocument(self):
		for t in self.tc:
			t.SetScrollBars()
		self.pdoc.pbuffer.DiscardLines()
		self.pdoc.pbuffer.DeleteRange(0, self.pdoc.pbuffer.Length())

	def SaveIfUnsure(self):
		if not self.pdoc.pbuffer.IsSavePoint():
			msg = u"Save changes to \"" + self.fullPath + u"\"?"
			print msg
			decision = self.DisplayMessage(msg, True)
			if decision:
				self.CmdSave()
			return decision
		return True

	def New(self):
		if self.SaveIfUnsure():
			self.fullPath = u""
			self.overrideMode = None
			self.CreateDocument(self.fullPath)
			self.CreateStyling(self.fullPath)
			self.NewDocument()
			self.SetTitle(1)
		self.Invalidate()

	def CheckMenus(self):
		if self.ControlActive() and self.pdoc:
			self.CheckMenuItem("Wrap", self.ControlActive().wrap)
			mode = unicode(self.pdoc.currentMode)
			modes = self.ModeNames()
			for k,v in self.ModeNames().iteritems():
				self.CheckMenuItem(v.replace(u" ", u""), k == mode)

	def GrabFile(self, name):
		self.fullPath = name
		self.overrideMode = None
		self.CreateDocument(self.fullPath)
		self.CreateStyling(self.fullPath)
		self.NewDocument()
		fsr = open(name, "rb")
		data = fsr.read()
		fsr.close()
		self.ControlActive().InsertBytes(0, data, len(data))
		self.pdoc.pbuffer.DeleteUndoHistory()
		self.ControlActive().SetSelection(0, 0)
		self.SetTitle(1)
		self.Invalidate()

	def Save(self):
		fos = open(self.fullPath, "wb")
		blockSize = 1024
		length = self.pdoc.pbuffer.Length()
		i = 0
		while i < length:
			grabSize = length - i
			if grabSize > blockSize:
				grabSize = blockSize
			#~ print i, grabSize, length
			data = self.pdoc.pbuffer.RetrieveUTF8(i, grabSize)
			fos.write(data)
			i += grabSize
		fos.close()
		self.pdoc.pbuffer.SetSavePoint()
		self.SetTitle(0)

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
		self.ControlActive().FoldAll(SinkWorld.TentacleControl.FoldCommand.fcFold)

	def CmdShowAll(self):
		self.ControlActive().FoldAll(SinkWorld.TentacleControl.FoldCommand.fcShow)

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
		findLength = self.pdoc.pbuffer.LengthInText(text)
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
		startEnd = max(self.ControlActive().GetSelectionRange().Anchor(),
			self.ControlActive().GetSelectionRange().Caret())
		next = self.SearchText(self.findWhat, startEnd, docLength)
		if next < 0:
			next = self.SearchText(self.findWhat, 0, startEnd)
		if next >= 0:
			findLength = self.pdoc.pbuffer.LengthInText(self.findWhat)
			self.ControlActive().SetSelection(next + findLength, next)
			self.ControlActive().EnsureCaretVisible()
			self.ControlActive().Invalidate()
			self.haveFound = 1

	def ReplaceOnce(self):
		if self.haveFound:
			start = self.ControlActive().GetSelectionRange().StartSelection()
			self.ControlActive().DeleteRange(start, self.ControlActive().GetSelectionRange().EndSelection() - start)
			rep = str(self.replaceWhat)
			replaceLength = self.ControlActive().InsertUTF8(\
				start, rep, len(rep))
			self.ControlActive().SetSelection(start + replaceLength, start)
			self.ControlActive().Invalidate()
		self.SearchFindNext()

	def ReplaceAll(self, selectionRange):
		if selectionRange:
			startPosition = self.ControlActive().GetSelectionRange().StartSelection()
			endPosition = self.ControlActive().GetSelectionRange().EndSelection()
		else:
			startPosition = 0
			endPosition = self.pdoc.pbuffer.Length()
		findLength = self.pdoc.pbuffer.LengthInText(self.findWhat)
		if findLength == 0:
			return
		nextMatch = self.SearchText(self.findWhat, startPosition, endPosition)
		while nextMatch >= 0:
			self.ControlActive().DeleteRange(nextMatch, findLength)
			rep = str(self.replaceWhat)
			replaceLength = \
				self.ControlActive().InsertUTF8(nextMatch, rep, len(rep))
			startPosition = nextMatch + replaceLength
			endPosition += replaceLength - findLength
			nextMatch = self.SearchText(self.findWhat, startPosition, endPosition)
		self.ControlActive().Invalidate()

