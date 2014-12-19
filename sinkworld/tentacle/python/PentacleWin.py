#!/usr/bin/python
# -*- coding: utf-8 -*-

import copy, glob, os, Queue, subprocess, sys, traceback, time, threading

import SinkWorld
from SinkWorld import Literal, LiteralSet

#~ SinkWorld.TentacleControl.Debugger()

import msvcrt
import ctypes
from ctypes import wintypes
from ctypes import c_int, c_ulong, c_char_p, c_wchar_p, c_ushort
user32=ctypes.windll.user32
gdi32=ctypes.windll.gdi32
from MessageNumbers import msgs, sgsm

import Pentacle
import PentacleMenu

WFUNC = ctypes.WINFUNCTYPE(c_int, c_int, c_int, c_int, c_int)

WS_CAPTION = 0xc00000
WS_SYSMENU = 0x80000
WS_CHILD = 0x40000000
WS_CLIPCHILDREN = 0x2000000
WS_CLIPSIBLINGS = 0x4000000
WS_OVERLAPPEDWINDOW = 0xcf0000
WS_VISIBLE = 0x10000000
SS_RIGHT = 2
DS_MODALFRAME = 128
WS_EX_CLIENTEDGE = 512
SWP_NOREDRAW = 8
SWP_DEFERERASE = 8192
SWP_NOCOPYBITS = 256
WA_INACTIVE = 0
MF_POPUP = 16
MF_SEPARATOR = 0x800
IDYES = 6
OFN_HIDEREADONLY = 4
MB_OK = 0
MB_YESNOCANCEL = 3
MF_CHECKED = 8
MF_UNCHECKED = 0

class OPENFILENAME(ctypes.Structure):
	_fields_ = (("lStructSize", c_int),
		("hwndOwner", c_int),
		("hInstance", c_int),
		("lpstrFilter", c_wchar_p),
		("lpstrCustomFilter", c_char_p),
		("nMaxCustFilter", c_int),
		("nFilterIndex", c_int),
		("lpstrFile", c_wchar_p),
		("nMaxFile", c_int),
		("lpstrFileTitle", c_wchar_p),
		("nMaxFileTitle", c_int),
		("lpstrInitialDir", c_wchar_p),
		("lpstrTitle", c_wchar_p),
		("flags", c_int),
		("nFileOffset", c_ushort),
		("nFileExtension", c_ushort),
		("lpstrDefExt", c_char_p),
		("lCustData", c_int),
		("lpfnHook", c_char_p),
		("lpTemplateName", c_char_p),
		("pvReserved", c_char_p),
		("dwReserved", c_int),
		("flagsEx", c_int))

	def __init__(self, win, title):
		ctypes.Structure.__init__(self)
		self.lStructSize = ctypes.sizeof(OPENFILENAME)
		self.nMaxFile = 1024
		self.hwndOwner = win
		self.lpstrTitle = title
		self.Flags = OFN_HIDEREADONLY

trace = False
#~ trace = True

def WindowSize(w):
	rc = wintypes.RECT()
	user32.GetClientRect(w, ctypes.byref(rc))
	return rc.right - rc.left, rc.bottom - rc.top

VK_SHIFT = 16
VK_CONTROL = 17
VK_MENU = 18

def IsKeyDown(key):
	return (user32.GetKeyState(key) & 0x8000) != 0

def KeyTranslate(w):
	tr = { 9: u"Tab", 0xD:"Enter", 0x1B: u"Esc" }
	if w in tr:
		return tr[w]
	elif ord(u"A") <= w <= ord(u"Z"):
		return chr(w)
	elif 0x70 <= w <= 0x7b:
		return "F" + str(w-0x70+1)
	else:
		return "Unknown_" + hex(w)

def GetClassName(w):
	val = u"\0" * 1024
	user32.GetClassNameW(w, val, len(val))
	return val.replace(u"\0", u"")

def GetText(w):
	val = u"\0" * 1024
	user32.GetWindowTextW(w, val, len(val))
	return val.replace(u"\0", u"")

class WNDCLASS(ctypes.Structure):
	_fields_= (\
		('style', c_int),
		('lpfnWndProc', WFUNC),
		('cls_extra', c_int),
		('wnd_extra', c_int),
		('hInst', c_int),
		('hIcon', c_int),
		('hCursor', c_int),
		('hbrBackground', c_int),
		('menu_name', c_wchar_p),
		('lpzClassName', c_wchar_p),
	)

class DLGTEMPLATE(ctypes.Structure):
	_pack_ = 2
	_fields_ = [
		("style", c_int),
		("exStyle", c_int),
		("cDlgItems", c_ushort),
		("x", c_ushort),
		("y", c_ushort),
		("cx", c_ushort),
		("cy", c_ushort)
	]

hinst = ctypes.windll.kernel32.GetModuleHandleW(0)

def RegisterClass(name, func, background = 0):
	# register a window class for toplevel windows.
	wc = WNDCLASS()
	wc.style = 0
	wc.lpfnWndProc = func
	wc.cls_extra = 0
	wc.wnd_extra = 0
	wc.hInst = hinst
	wc.hIcon = 0
	wc.hCursor = 0
	wc.hbrBackground = background
	wc.menu_name = 0
	wc.lpzClassName = name
	user32.RegisterClassW(ctypes.byref(wc))

# For variables which may not stay alive by default
keepAround = []

class Vowels(SinkWorld.LexBase):

	def __init__(self):
		print "Vowels::__init__"
		SinkWorld.LexBase.__init__(self)
		self.name = Literal("Vowels")
		states = "C V"
		self.vchars = [ord(ch) for ch in "aeiouAEIOU"]
		self.stateNames = Literal(states)
		self.thisStates = states.count(" ") + 1
		keepAround.append(self)

	def CreateNew(self):
		print "Vowels::CreateNew"
		return Vowels()

	def Colourise(self, sc):
		print "Vowels::Colourise"
		self.InvalidateAfter(sc.startSeg)

		while sc.More():

			if sc.ch in self.vchars:
				sc.SetState(1)
			else:
				sc.SetState(0)

			sc.Forward()

		sc.Complete()
		print "Vowels::Colourise ended"

	def SetBaseState(self, s):
		print "Vowels::SetBaseState", s

	def GetBaseState(self):
		print "Vowels::GetBaseState"
		return 0

class ErrorParser(SinkWorld.LexBase):

	def __init__(self):
		print "ErrorParser::__init__"
		SinkWorld.LexBase.__init__(self)
		self.name = Literal("Errors")
		states = "Default Command Python Borland GCC Microsoft DiffName DiffAdd DiffRemove"
		self.stateNames = Literal(states)
		self.thisStates = states.count(" ") + 1
		keepAround.append(self)

	def CreateNew(self):
		print "ErrorParser::CreateNew"
		return ErrorParser()

	def Colourise(self, sc):
		#~ print "ErrorParser::Colourise"
		try:
			self.InvalidateAfter(sc.startSeg)

			line = ""
			sc.SetState(1)
			ii = 0
			while sc.More():

				if sc.ch in [10, 13]:
					if line:
						#~ print line
						#~ type = ii % 5
						(type, path, line, column) = Pentacle.ParseErrorLine(line)
						sc.ChangeState(type)
					line = ""
					ii += 1
					sc.SetState(0)
				else:
					line += chr(sc.ch)

				sc.Forward()
		except:
			# Print out here otherwise gets munged inside trasfer to C++
			traceback.print_exc()
			raise

		sc.Complete()
		#~ print "ErrorParser::Colourise ended"

	def SetBaseState(self, s):
		pass

	def GetBaseState(self):
		return 0

class Strip:
	def __init__(self, parent):
		self.parent = parent
		self.lineHeight = 23
		self.borderHeight = 5
		self.gap = 4
		self.lines = 1
		self.windows = []
		self.buttons = []
		self.columns = 0
		self.cmdBase = 1000
		self.ditch = []
		self.hfont = gdi32.GetStockObject(17)
		class ControlInfo:
			def __init__(self, yAdjustment, width, height, flags=0, extraFlags=0):
				self.yAdjustment = yAdjustment
				self.width = width
				self.height = height
				self.flags = flags
				self.extraFlags = extraFlags
		self.ctlInfo = {
			u"Static": ControlInfo(3, 70, 17, SS_RIGHT),
			u"Edit": ControlInfo(0, 240, 20, 0, WS_EX_CLIENTEDGE),
			u"Button": ControlInfo(0, 80, 20),
		}
		self.windowName = u"Strip"
		self.wfunc = WFUNC(self.WndProc)
		RegisterClass(self.windowName, self.wfunc, 15 + 1)

	def PerformButton(self, window):
		params = [GetText(window).replace(u"&", u"")]
		for w in self.windows:
			if GetClassName(w) == u"Edit":
				params.append(GetText(w))
		self.parent.StripCommand(params)

	def WndProc(self, h, m, w, l):
		if m == msgs["WM_COMMAND"]:
			id = w & 0xffff
			window = user32.GetDlgItem(self.win, id)
			if window in self.buttons:
				self.PerformButton(window)
			return 0
		else:
			return user32.DefWindowProcW(h, m, w, l)

	def NextInTabOrder(self, wFocus, reverse=0):
		lenWindows = len(self.windows)
		next = False
		for i in range(lenWindows * 2):
			pos = i
			if reverse:
				pos = lenWindows - 1 - pos
			w = self.windows[pos % lenWindows]
			if w == wFocus:
				next = True
			elif next and GetClassName(w) in [u"Edit", u"Button"]:
				return w
		return None

	def KeyDown(self, keyName):
		if keyName.startswith(u"<alt>"):
			keyChar = u"&" + keyName[5]
			for i in range(len(self.windows)):
				w = self.windows[i]
				if keyChar in GetText(w).upper():
					if GetClassName(w) == u"Static":
						user32.SetFocus(self.windows[i + 1][0])
						return True
					elif GetClassName(w) == u"Button":
						self.PerformButton(w)
						return True
		elif keyName.endswith(u"Tab") and self.windows:
			reverse = keyName.startswith(u"<shift>")
			wFocus = user32.GetFocus()
			wFocusNext = self.NextInTabOrder(wFocus, reverse)
			if wFocusNext:
				user32.SetFocus(wFocusNext)
				return True
		elif keyName == u"Enter":
			wFocus = user32.GetFocus()
			if GetClassName(wFocus) == u"Edit":
				self.PerformButton(self.NextInTabOrder(wFocus))
				return True
			elif GetClassName(wFocus) == u"Button":
				self.PerformButton(wFocus)
				return True
		return False

	def Height(self):
		if self.windows:
			return self.lineHeight * self.lines - 2 + 2 * self.borderHeight
		else:
			return 0

	def Create(self):
		self.win = user32.CreateWindowExW(\
			0, self.windowName, u"", WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN, \
			0, 0, 500, 700, self.parent.win, 1, hinst, 0)

	def AddControl(self, ctlType, text):
		c = self.ctlInfo[ctlType]
		cmd = self.cmdBase + len(self.windows)
		w = user32.CreateWindowExW(\
			c.extraFlags, ctlType, text, WS_VISIBLE | WS_CHILD | c.flags, 0, 0,
				c.width, c.height, self.win, cmd, hinst, 0)
		user32.SendMessageW(w, msgs["WM_SETFONT"], self.hfont, 0)
		self.windows.append(w)
		if ctlType == u"Button":
			self.buttons.append(w)

	def Set(self, description):
		for w in self.windows:
			user32.DestroyWindow(w)
		self.description = description
		if not self.ditch:
			for i in range(1):
				self.ditch.append(user32.CreateWindowExW(\
					0, u"Static", u"", WS_VISIBLE | WS_CHILD | 16, 0, 0,
					4000, 2, self.win, 0, hinst, 0))
		self.lines = 0
		self.windows = []
		self.buttons = []
		self.columns = 0
		for line in self.description:
			self.lines += 1
			self.AddControl(u"Static", line[0])
			self.AddControl(u"Edit", line[1])
			self.columns = max(self.columns, len(line))
			for button in line[2:]:
				self.AddControl(u"Button", button)
		if self.windows:
			user32.SetFocus(self.windows[1])

	def OnSize(self, width, height):
		user32.SetWindowPos(self.win, 0, 0,height - self.Height(), width, self.Height(), 0)
		if self.windows:
			user32.SetWindowPos(self.ditch[0], 0, 0,0, 4000, 2, 0)
			#~ user32.SetWindowPos(self.ditch[1], 0, 0, self.Height()-2, 4000, 2, 0)
			x = 0
			y = self.borderHeight - self.lineHeight
			n = 0
			fixedWidth = self.columns * self.gap + (self.columns-2) * self.ctlInfo[u"Button"].width + \
				self.ctlInfo[u"Static"].width
			editWidth = width - fixedWidth
			for w in self.windows:
				ctlType = GetClassName(w)
				c = self.ctlInfo[ctlType]
				if ctlType == u"Static":
					y += self.lineHeight
					x = 0
				width = c.width
				if ctlType == u"Edit":
					width = editWidth
				user32.SetWindowPos(w, 0, x,y+c.yAdjustment, width, c.height, SWP_NOCOPYBITS)
				x += width + self.gap

class RunThread(threading.Thread):
	def __init__(self, cmd, win, queue):
		threading.Thread.__init__(self)
		self.exit = False
		self.cmd = cmd
		self.win = win
		self.queue = queue
	def send(self, data):
		self.queue.put(data)
		user32.PostMessageW(self.win, msgs["WM_USER"], 0, 0)
	def run(self):
		start = time.time()
		p = subprocess.Popen(self.cmd, shell=True, bufsize=0,
			  stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
		outputHandle = msvcrt.get_osfhandle(p.stdout.fileno())
		while p.poll() is None and not self.exit:
			try:
				avail = c_ulong(0)
				ctypes.windll.kernel32.PeekNamedPipe(outputHandle, None, 0, None, \
					ctypes.byref(avail), None)
				#~ print "%%", avail.value
				if avail.value:
					self.send(p.stdout.read(avail.value))
					time.sleep(0.0)
				else:
					# Nothing happening so sleep on this thread so UI is responsive
					time.sleep(0.01)
			except:
				# Print out here otherwise gets munged inside trasfer to C++
				traceback.print_exc()
				raise
		data = p.stdout.read()
		#~ print "End of data", len(data), data
		self.send(data)
		if p.returncode is not None:
			end = time.time()
			self.send("> Exit code %d   %f\n" % (p.returncode, (end-start)))

class PentacleWin(Pentacle.Pentacle):
	def __init__(self):
		Pentacle.Pentacle.__init__(self)

		lexer = Literal("Lexer")
		self.vowelProto = Vowels()
		#~ xx = self.vowelProto.CreateNew()
		#~ print vowelProto.Name()
		SinkWorld.PrototypeRegistry.AddPrototype(lexer, self.vowelProto)

		self.errorProto = ErrorParser()
		SinkWorld.PrototypeRegistry.AddPrototype(lexer, self.errorProto)

		self.cmds = {}
		self.strip = Strip(self)
		self.windowName = u"PentacleWindow"
		self.wfunc = WFUNC(self.WndProc)
		RegisterClass(self.windowName, self.wfunc)
		user32.CreateWindowExW(0, self.windowName, u"PW", \
			WS_VISIBLE | WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN, \
			0, 0, 500, 700, 0, 0, hinst, 0)

		args = sys.argv[1:]
		runningCommand = len(args) > 1 and args[0] == "-x"
		self.SetMenus(runningCommand)
		self.rt = None
		if runningCommand:
			self.queue = Queue.Queue(5)
			self.rt = RunThread(args[1], self.win, self.queue)
			self.rt.start()
			self.fullPath = u">" + args[1]
			self.SetOverrideMode("mode.base.errors")
		elif args:
			self.GrabFile(args[0])
			self.FocusText()
			self.ControlActive().ScrollTo(100000, True)
			if len(args) > 1:
				lineNumber = int(args[1])
				self.GoLine(lineNumber - 1)
				# TODO does not scroll to caret.

	def OnSize(self):
		width, height = WindowSize(self.win)
		self.strip.OnSize(width, height)
		user32.SetWindowPos(self.wText, 0, 0,0, width,
			height-self.strip.Height(), 0)
		user32.InvalidateRect(self.win, 0, 0)

	def OnCreate(self, hwnd):
		#~ tc.Create(w)
		self.win = hwnd
		for t in self.tc:
			t.CreateControl(self.win)
			self.wText = user32.GetWindow(self.win, 5)
			#~ print "wText", hex(self.wText)
		self.strip.Create()

	def WndProc(self, h, m, w, l):
		ms = sgsm.get(m, "XXX")
		if trace:
			print hex(h)[2:],ms,w,l
		if ms == "WM_CLOSE":
			user32.PostQuitMessage(0)
			if self.rt:
				self.rt.exit = True
			return 0
		elif ms == "WM_CREATE":
			self.OnCreate(h)
			return 0
		elif ms == "WM_SIZE":
			# Work out size
			if w != 1:
				self.OnSize()
			return 0
		elif ms == "WM_COMMAND":
			self.Command(self.cmds[w & 0xffff])
			return 0
		elif ms == "WM_ACTIVATE":
			if w != WA_INACTIVE:
				self.FocusText()
			return 0
		elif ms == "WM_USER":
			data = self.queue.get()
			self.ControlActive().InsertBytes(self.pdoc.pbuffer.Length(), data, len(data))
			self.ControlActive().ScrollTo(self.pdoc.pbuffer.Length(), True)
			self.Invalidate()
			time.sleep(0.01)	# Give the producer some time
			return 0
		else:
			return user32.DefWindowProcW(h, m, w, l)

	def KeyDown(self, w, prefix = u""):
		keyName = prefix
		if IsKeyDown(VK_CONTROL):
			keyName += u"<control>"
		if IsKeyDown(VK_SHIFT):
			keyName += u"<shift>"
		keyName += KeyTranslate(w)
		if trace:
			print "Key:", keyName
		if keyName in self.keys:
			method = "Cmd" + self.keys[keyName]
			getattr(self, method)()
			return True
		if self.strip.KeyDown(keyName):
			return True
		#~ print "UKey:", keyName
		return False

	def Accelerator(self, msg):
		ms = sgsm.get(msg.message, "XXX")
		if ms == "WM_KEYDOWN":
			return self.KeyDown(msg.wParam)
		elif ms == "WM_SYSKEYDOWN":
			return self.KeyDown(msg.wParam, u"<alt>")
		return False

	def AppLoop(self):
		msg = wintypes.MSG()
		lpmsg = ctypes.byref(msg)
		while user32.GetMessageW(lpmsg, 0, 0, 0):
			if trace and msg.message != msgs["WM_TIMER"]:
				print 'mm', hex(msg.hWnd)[2:],sgsm.get(msg.message, "XXX")
			if not self.Accelerator(msg):
				user32.TranslateMessage(lpmsg)
				user32.DispatchMessageW(lpmsg)

	def FocusText(self):
		user32.SetFocus(self.wText)

	def SetTitle(self, changePath):
		if changePath or self.titleAtSavePoint != self.pdoc.pbuffer.IsSavePoint():
			Pentacle.Pentacle.SetTitle(self, changePath)
			if self.win:
				user32.SetWindowTextW(self.win, self.title)

	def Open(self):
		ofx = OPENFILENAME(self.win, "Open File")
		opath = u"\0" * 1024
		ofx.lpstrFile = opath
		filters = sorted(self.OptionsFromSuffix(".filter").values())
		filterText = unicode("\0".join([f.replace("|", "\0") for f in filters])+"\0\0")
		ofx.lpstrFilter = filterText
		if ctypes.windll.comdlg32.GetOpenFileNameW(ctypes.byref(ofx)):
			absPath = opath.replace(u"\0", u"")
			self.GrabFile(absPath)
			self.FocusText()

	def SaveAs(self):
		ofx = OPENFILENAME(self.win, "Save File")
		opath = u"\0" * 1024
		ofx.lpstrFile = opath
		if ctypes.windll.comdlg32.GetSaveFileNameW(ctypes.byref(ofx)):
			self.fullPath = opath.replace(u"\0", u"")
			self.Save()
			self.FocusText()

	def StripCommand(self, params):
		command = params[0]
		self.findWhat = Pentacle.LiteralFromUnicode(params[1])
		params[1]
		if len(params) > 2:
			self.replaceWhat = Pentacle.LiteralFromUnicode(params[2])
		if command == "Find":
			self.SearchFindNext()
		elif command == "Replace":
			self.ReplaceOnce()
		elif command == "Replace All":
			self.ReplaceAll(0)
		elif command == "In Selection":
			self.ReplaceAll(1)

	def SetMenus(self, runningCommand=False):
		modes = self.ModeNames()
		modeMenu = sorted([[n, ""] for n in modes.values()])
		#~ print "Mode Menu"
		#~ print modeMenu
		ui = copy.deepcopy(PentacleMenu.MenuStructure)
		if self.mru:
			mru = [["-", ""]] + [[n, ""] for n in self.mru]
			ui[PentacleMenu.MENU_FILE][1][4:4] = mru
		if runningCommand:
			ui[PentacleMenu.MENU_SEARCH][1].extend([ 
				[ "&Next Message", "F4" ], [ "&Previous Message", "" ]])
		ui[PentacleMenu.MENU_MODE][1].extend(modeMenu)
		#~ ui.append([ "&Mode", modeMenu ])
		modeFiles = self.ModeFiles()
		modeFileMenu = sorted([[n, ""] for n in modeFiles])
		#~ print "ModeFileMenu", "|".join(modeFiles)
		ui[PentacleMenu.MENU_OPTIONS][1].extend([["-", ""]] + modeFileMenu)
		self.cmds = {}
		self.keys = {}
		
		toolNames = ["Compile", "Build", "Go"]
		tools = []
		#~ kk = Literal("")
		#~ kl = self.pdoc.options.Keys(kk)
		#~ for x in range(kl.Length()):
			#~ print x, kl.GetText(x)
		for tool in toolNames:
			cmd = self.pdoc.options.ModeValue(self.pdoc.currentMode, Literal("tools." + tool))
			if cmd:
				tools.append([tool, ""])
		if tools:
			ui[PentacleMenu.MENU_TOOLS][1].extend(tools)

		id = 0
		self.menuBar = user32.CreateMenu()
		for name, contents in ui:
			id += 1
			menu = user32.CreateMenu()
			for item in contents:
				text, key = item
				cmdText = text.replace("&", "")
				cmdText = cmdText.replace("...", "")
				cmdText = cmdText.replace(" ", "")
				id += 1
				if key:
					keyText = key.replace("<control>", "Ctrl+")
					keyText = keyText.replace("<shift>", "Shift+")
					text += "\t" + keyText
				if text == "-":
					user32.AppendMenuW(menu, MF_SEPARATOR, id, unicode(text))
				else:
					user32.AppendMenuW(menu, 0, id, unicode(text))
				self.cmds[id] = cmdText
				self.keys[key] = cmdText
				#~ print id, item
			user32.AppendMenuW(self.menuBar, MF_POPUP, menu, unicode(name))
		user32.SetMenu(self.win, self.menuBar)
		self.CheckMenuItem("Wrap", True)
		user32.ShowWindow(self.win, 5)

	def CheckMenuItem(self, name, val):
		#~ print name, val
		if self.cmds:
			for k,v in self.cmds.iteritems():
				if v == name:
					#~ print name, k
					user32.CheckMenuItem(user32.GetMenu(self.win), \
						k, [MF_UNCHECKED, MF_CHECKED][val]);

	def Exit(self):
		sys.exit(0)

	def SearchHide(self):
		self.strip.Set([])
		self.OnSize()
		self.FocusText()

	def SearchFind(self):
		#~ self.SelectPane(1)
		self.haveFound = 0
		wordAtCaret = self.WordAtCaret()
		self.ControlActive().EnsureCaretVisible()
		self.strip.Set([[u"Find &what:", unicode(wordAtCaret), u"Fin&d"]])
		self.OnSize()
		self.ControlActive().EnsureCaretVisible()

	def SearchReplace(self):
		#~ self.SelectPane(2)
		self.haveFound = 0
		wordAtCaret = self.WordAtCaret()
		self.ControlActive().EnsureCaretVisible()
		self.strip.Set(\
			[[u"Find &what:", unicode(wordAtCaret), u"Fin&d", u"Replace &All"],
			[u"Replace wit&h:", u"", u"&Replace", u"&In Selection"]])
		self.OnSize()
		self.ControlActive().EnsureCaretVisible()

	def DisplayMessage(self, msg, ask):
		return IDYES == user32.MessageBoxW(self.win, \
			msg, self.appName, [MB_OK, MB_YESNOCANCEL][ask]);

fr = PentacleWin()
fr.AppLoop()
