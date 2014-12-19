# -*- coding: utf-8 -*-

""" Define the menu structure used by the Pentacle applications """

MENU_FILE, MENU_EDIT, MENU_SEARCH, MENU_TOOLS, MENU_OPTIONS, MENU_MODE = range(6)
MenuStructure = [
	["&File", [
		["&New", "<control>N"],
		["&Open...", "<control>O"],
		["&Save", "<control>S"],
		["Save &As...", "<control><shift>S"],
		["-", ""],
		["&Exit", ""]]],
	[ "&Edit", [
		["&Undo", "<control>Z"],
		["&Redo", "<control>Y"],
		["-", ""],
		["Cu&t", "<control>X"],
		["&Copy", "<control>C"],
		["&Paste", "<control>V"],
		["&Delete", "Del"],
		["Select &All", "<control>A"],
		["-", ""],
		["&Fold", "<control>L"],
		["F&old All", ""],
		["&Show All", ""]]],
	[ "&Search", [
		["&Find...", "<control>F"],
		["Find &Next", "F3"],
		["&Replace...", "<control>H"],
		["&Hide", "Esc"]]],
	[ "&Tools", [
		]],
	[ "&Options", [
		["&Wrap", ""]]],
	[ "&Mode", [
		]],
]
