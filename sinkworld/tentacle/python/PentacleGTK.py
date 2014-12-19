#!/usr/bin/python
# -*- coding: utf-8 -*-

import os
import glob
import sys

import pygtk
pygtk.require('2.0')
import gtk, gobject

import SinkWorld
from SinkWorld import Literal, LiteralSet

import Pentacle
import PentacleMenu

trace = False

stocks = {
	'New': gtk.STOCK_NEW,
	'Open': gtk.STOCK_OPEN,
	'Save': gtk.STOCK_SAVE,
	'Save As': gtk.STOCK_SAVE_AS,
	'Exit': gtk.STOCK_QUIT,
	'Undo': gtk.STOCK_UNDO,
	'Redo': gtk.STOCK_REDO,
	'Cut': gtk.STOCK_CUT,
	'Copy': gtk.STOCK_COPY,
	'Paste': gtk.STOCK_PASTE,
	'Delete': gtk.STOCK_DELETE,
	'Find': gtk.STOCK_FIND,
	'Replace': gtk.STOCK_FIND_AND_REPLACE,
}

def LiteralFromEntry(s, w):
	return Literal(w.get_text())

def EntryFromLiteral(s, w):
	w.set_text(str(s))

class PentacleGTK(Pentacle.Pentacle):
	def __init__(self):
		Pentacle.__init__(self)
		self.win = gtk.Window()
		self.win.connect("delete_event", self.Exit)

		self.boxMain = gtk.VBox(False, 0)
		self.win.add(self.boxMain)
		self.boxMain.unset_flags(gtk.CAN_FOCUS)

		self.SetMenus()

		self.findWhat = Literal("")
		self.replaceWhat = Literal("")
		self.panesVisible = 0
		self.findPane = self.PaneFind()
		self.boxMain.pack_start(self.findPane, False, False, 0)
		self.replacePane = self.PaneFindReplace()
		self.boxMain.pack_start(self.replacePane, False, False, 0)

		for t in self.tc:
			t.CreateControl(self.win)
		self.tc[0].AddTo(self.boxMain)
		
		self.win.show_all()
		self.findPane.hide_all()
		self.replacePane.hide_all()
		self.tc[0].Focus()

	def Exit(self, b=None, d=None):
		gtk.main_quit()

	def AppLoop(self):
		gtk.main()

	def SetTitle(self, changePath):
		if changePath or self.titleAtSavePoint != self.pdoc.GetBuffer().IsSavePoint():
			Pentacle.SetTitle(self, changePath)
			if self.win:
				self.win.set_title(self.title)

	def Action(self, action=None):
		print action, dir(action), action.get_name()
		self.Command(action.get_name())

	def Open(self):
		dlg = gtk.FileChooserDialog("Open File", self.win, \
			gtk.FILE_CHOOSER_ACTION_OPEN, (gtk.STOCK_CANCEL, gtk.RESPONSE_CANCEL, \
			gtk.STOCK_OPEN, gtk.RESPONSE_ACCEPT))
	
		dlg.set_property("show-hidden", True)
		dlg.set_default_response(gtk.RESPONSE_ACCEPT)
		filters = self.OptionsFromSuffix(".filter").values()
		filters.sort()
		for f in filters:
			filterName, patterns = f.split("|")
			filterLine = gtk.FileFilter()
			filterLine.set_name(filterName)
			for pattern in patterns.split(";"):
				filterLine.add_pattern(pattern)
			dlg.add_filter(filterLine)
	
		if dlg.run() == gtk.RESPONSE_ACCEPT:
			names = dlg.get_filenames()
			absPath = names[0]
			self.GrabFile(absPath)
		dlg.destroy()
		self.FocusText()

	def SaveAs(self):
		dlg = gtk.FileChooserDialog("Save File", self.win, \
			gtk.FILE_CHOOSER_ACTION_SAVE, (gtk.STOCK_CANCEL, gtk.RESPONSE_CANCEL, \
			gtk.STOCK_SAVE, gtk.RESPONSE_ACCEPT))
	
		dlg.set_property("show-hidden", True)
		dlg.set_default_response(gtk.RESPONSE_ACCEPT)
		dlg.set_filename(self.fullPath)

		if dlg.run() == gtk.RESPONSE_ACCEPT:
			names = dlg.get_filenames()
			self.fullPath = names[0]
			print self.fullPath, type(self.fullPath)
			self.Save()
			self.SetTitle(True)
		dlg.destroy()
		self.FocusText()
		self.Invalidate()

	def FocusText(self):
		self.ControlActive().Focus()

	def CheckMenus(self):
		pass

	def SetMenus(self):

		actiongroup = gtk.ActionGroup('Pentacle')
		self.actiongroup = actiongroup
		
		actiongroup.add_toggle_actions([('Wrap', None, '_Wrap', None, 'Wrap the file', self.Action)])

		modes = self.ModeNames()
		modeMenu = [[str(n), ""] for n in modes.values()]
		modeMenu.sort()
		ui = PentacleMenu.MenuStructure
		ui.append([ "&Mode", modeMenu ])

		actionSet = []
		for name, contents in ui:
			name = name.replace("&", "_")
			cmdText = name.replace("_", "")
			cmdText = cmdText.replace("...", "")
			cmdText = cmdText.replace(" ", "")
			actionSet.append((cmdText, None, name))
			for item in contents:
				text, key = item
				text = text.replace("&", "_")
				cmdText = text.replace("_", "")
				cmdText = cmdText.replace("...", "")
				if key == "":
					key = None
				keyTrans = {"Del":"Delete", "Esc":"Escape"}
				key = keyTrans.get(key, key)
				stock = stocks.get(cmdText, None)
				if cmdText != "-":
					actionSet.append((cmdText, stock, text, key, cmdText, self.Action))
		#~ for a in actionSet: 
			#~ print a
		actiongroup.add_actions(actionSet)
		uimanager = gtk.UIManager()
		uimanager.insert_action_group(actiongroup, 0)		
		self.accelGroup = uimanager.get_accel_group()
		self.win.add_accel_group(self.accelGroup)

		self.cmds = {}
		self.keys = {}

		id = 0
		menuUI = '\t<menubar name="MenuBar">'
		for name, contents in ui:
			id += 1
			menuUI += '\t\t<menu action="%s">\n' % name.replace("&", "")
			for item in contents:
				text, key = item
				cmdText = text.replace("&", "")
				cmdText = cmdText.replace("...", "")
				id += 1
				if key:
					keyText = key.replace("<control>", "Ctrl+")
					keyText = keyText.replace("<shift>", "Shift+")
					text += "\t" + keyText
				if text == "-":
					menuUI += '\t\t\t<separator/>\n'
				else:
					menuUI += '\t\t\t<menuitem action="%s"/>\n' % cmdText
				cmdTextDict = cmdText.replace(" ", "")
				self.cmds[id] = cmdTextDict
				self.keys[key] = cmdTextDict
			menuUI += '\t\t</menu>\n'
		menuUI += '\t</menubar>\n'
		
		toolbarUI = '\t<toolbar name="Toolbar">\n'
		for name, contents in ui:
			for item in contents:
				text, key = item
				cmdText = text.replace("&", "")
				cmdText = cmdText.replace("...", "")
				if cmdText in stocks:
					toolbarUI += '\t\t<toolitem action="%s"/>\n' % cmdText
		toolbarUI += '\t</toolbar>\n'

		uiFull = '<ui>\n' + menuUI + toolbarUI + '</ui>\n'
		uimanager.add_ui_from_string(uiFull)

		menubar = uimanager.get_widget('/MenuBar')
		toolbar = uimanager.get_widget('/Toolbar')
		self.boxMain.pack_start(menubar, False)
		self.boxMain.pack_start(toolbar, False)

	def DisplayMessage(self, msg, ask):
		dlg = gtk.MessageDialog(self.win, gtk.DIALOG_DESTROY_WITH_PARENT, \
			[gtk.MESSAGE_ERROR, gtk.MESSAGE_QUESTION][ask], 
			[gtk.BUTTONS_CLOSE, gtk.BUTTONS_YES_NO][ask], msg)
		ret = dlg.run()
		dlg.destroy()
		return ret == gtk.RESPONSE_YES

	def MakeCommand(self, text, accel_group, func):
		label = gtk.Label(text)
		label.set_use_underline(True)
		command = gtk.Button()
		command.add(label)
		command.set_flags(gtk.CAN_DEFAULT)
		#~ key = gtk_label_parse_uline(GTK_LABEL(GTK_BIN(command)->child), text)
		key = label.get_mnemonic_keyval()
		#~ print key
		command.add_accelerator("clicked", accel_group, key, gtk.gdk.MOD1_MASK, 0)
		command.connect("clicked", func)
		return command

	def AddCell(self, wTable, w, column, row, expand=False):
		opts = gtk.FILL
		if expand:
			opts = gtk.SHRINK | gtk.FILL | gtk.EXPAND
		wTable.attach(w, column, column+1, row, row+1, opts, opts, 0, 0)

	def ReadEntries(self):
		self.findWhat = LiteralFromEntry(self.entryFRFind)
		LiteralFromEntry(self.replaceWhat = LiteralFromEntry(self.entryReplace)

	def FFind(self, data):
		self.findWhat = LiteralFromEntry(self.entryFind)
		self.SearchFindNext()

	def FRFind(self, data):
		self.ReadEntries()
		self.SearchFindNext()

	def FRReplace(self, data):
		self.ReadEntries()
		self.ReplaceOnce()

	def FRReplaceAll(self, data):
		self.ReadEntries()
		self.ReplaceAll(False)

	def FRReplaceInSelection(self, data):
		self.ReadEntries()
		self.ReplaceAll(True)

	def PaneSignalKey(self, w, event):
		print w, event
		if event.keyval == gtk.keysyms.Escape:
			self.SelectPane(0)
			return True
		elif event.keyval == gtk.keysyms.Return:
			if self.panesVisible == 1:
				FFind(0)
			elif self.panesVisible == 2:
				FRFind(0)
			else:
				return True
		elif event.keyval == gtk.keysyms.Tab:
			if self.panesVisible == 1:
				return self.btnFind.get_property("has_focus")
			elif self.panesVisible == 2:
				return self.btnReplaceInSelection.get_property("has_focus")
			return False
		elif event.keyval == gtk.keysyms.ISO_Left_Tab:
			if self.panesVisible == 1:
				return self.entryFind.get_property("has_focus")
			elif self.panesVisible == 2:
				return self.entryFRFind.get_property("has_focus")
			return False
		elif event.keyval == gtk.keysyms.Down:
			return True
		return False

	def PaneFind(self):
		wFind = gtk.Table(1, 3, False)
		wTable = wFind
		wTable.set_border_width(4)
		wTable.set_row_spacings(4)
		wTable.set_col_spacings(4)
	
		labelFind = gtk.Label("Find what:")
		self.AddCell(wTable, labelFind, 0, 0)
		labelFind.set_alignment(1.0, 0.5)
		self.entryFind = gtk.Entry()
		self.AddCell(wTable, self.entryFind, 1, 0, True)
		self.entryFind.connect("activate", self.FFind)
	
		self.btnFind = self.MakeCommand("F_ind", self.accelGroup, self.FFind)
		self.AddCell(wTable, self.btnFind, 2, 0)
	
		self.btnFind.set_flags(gtk.CAN_DEFAULT)
	
		wFind.connect("key_press_event", self.PaneSignalKey)
	
		return wFind

	def PaneFindReplace(self):
		wFindReplace = gtk.Table(2, 4, False)
		wTable = wFindReplace
		wTable.set_border_width(4)
		wTable.set_row_spacings(4)
		wTable.set_col_spacings(4)
	
		labelFind = gtk.Label("Find what:")
		self.AddCell(wTable, labelFind, 0, 0)
		labelFind.set_alignment(1.0, 0.5)
		self.entryFRFind = gtk.Entry()
		self.AddCell(wTable, self.entryFRFind, 1, 0, True)
		self.entryFRFind.connect("activate", self.FRFind)
	
		labelReplace = gtk.Label("Replace with:")
		labelReplace.set_alignment(1.0, 0.5)
		self.AddCell(wTable, labelReplace, 0, 1)
		self.entryReplace = gtk.Entry()
		self.AddCell(wTable, self.entryReplace, 1, 1, True)
		self.entryReplace.connect("activate", self.FRReplace)
	
		btnFRFind = self.MakeCommand("F_ind", self.accelGroup, self.FRFind)
		self.AddCell(wTable, btnFRFind, 2, 0)
	
		btnReplace = self.MakeCommand("_Replace", self.accelGroup, \
			self.FRReplace)
		self.AddCell(wTable, btnReplace, 2, 1)
	
		btnReplaceAll = self.MakeCommand("Replace _All", self.accelGroup, \
			self.FRReplaceAll)
		self.AddCell(wTable, btnReplaceAll, 3, 0)
	
		self.btnReplaceInSelection = self.MakeCommand("I_n Selection", self.accelGroup, \
			self.FRReplaceInSelection)
		self.AddCell(wTable, self.btnReplaceInSelection, 3, 1)
	
		btnFRFind.set_flags(gtk.CAN_DEFAULT)
	
		wFindReplace.connect("key_press_event", self.PaneSignalKey)
	
		return wFindReplace

	def SelectPane(self, pane):
		if pane != self.panesVisible:
			if self.panesVisible == 1:
				self.findPane.hide_all()
			elif self.panesVisible == 2:
				self.replacePane.hide_all()
			if pane == 1:
				self.findPane.show_all()
			elif pane == 2:
				self.replacePane.show_all()
			self.panesVisible = pane

	def SearchFind(self):
		self.haveFound = 0
		word = self.WordAtCaret()
		self.ControlActive().EnsureCaretVisible()
		EntryFromLiteral(word, self.entryFind)
		self.SelectPane(1)
		self.entryFind.grab_focus()

	def SearchReplace(self):
		self.haveFound = 0
		word = self.WordAtCaret()
		self.ControlActive().EnsureCaretVisible()
		EntryFromLiteral(word, self.entryFRFind);
		self.SelectPane(2)
		self.entryFRFind.grab_focus()

	def SearchHide(self):
		self.SelectPane(0)

fr = PentacleGTK()
fr.AppLoop()
