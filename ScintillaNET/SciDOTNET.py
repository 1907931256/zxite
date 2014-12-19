# SciDOTNET.py - Updates MC++ code used to create .NET scintilla wrapper.
# Original Code by Simon Steele. (Dude! way to go!)
# updates by Garrett Serack
#

import string
import sys
import os

def sanitiseLine(line):
	if line[-1:] == '\n': line = line[:-1]
	if string.find(line, "##") != -1:
		line = line[:string.find(line, "##")]
	line = string.strip(line)
	return line

def safename(s):
	ret = s
	#if s == "end":
	#	ret = "end_"
	#elif s == "begin":
	#	ret = "begin_"
	#elif s == "SetFocus":
	#	ret = "SetFocusEx"
	return ret

def decodeFunction(featureVal):
	retType, rest = string.split(featureVal, " ", 1)
	nameIdent, params = string.split(rest, "(")
	name, value = string.split(nameIdent, "=")
	if name != "":
		name = safename(name)
	params, rest = string.split(params, ")")
	param1, param2 = string.split(params, ",")[0:2]
	return retType, name, value, param1, param2
	
def decodeEvent(featureVal):
	retType, rest = string.split(featureVal, " ", 1)
	nameIdent, params = string.split(rest, "(")
	name, value = string.split(nameIdent, "=")
	params, rest = string.split(params, ")")
	parms = ParamList()
	parms.AddParams(params)
	return retType, name, value, parms

def decodeParam(p):
	param = string.strip(p)
	type = ""
	name = ""
	value = ""
	if " " in param:
		type, nv = string.split(param, " ")
		if "=" in nv:
			name, value = string.split(nv, "=")
		else:
			name = nv
	if name != "":
		name = safename(name)
	return type, name, value

class ParamList:
	def __init__(self):
		self.order = []
		self.params = {}
	
	def AddParams(self, str):
		notempty = 1;
		main = str
		string.strip(main)
		while notempty == 1:
			if "," in main:
				cur, main = string.split(main, ",", 1)
			else:
				cur = main
				main = ""
			t, name, v = decodeParam(cur)
			if name != "":
				name = safename(name)
				self.params[name] = {
					"ParamType" : t,
					"ParamValue" : v
				}
				self.order.append(name)
				string.strip(main)
			if main == "" or main == ",":
				notempty = 0

class Face:

	def __init__(self):
		self.order = []
		self.features = {}
		self.values = {}
		self.events = {}
		
	def ReadFromFile(self, name):
		currentCategory = ""
		currentComment = []
		currentCommentFinished = 0
		file = open(name)
		for line in file.readlines():
			line = sanitiseLine(line)
			if line:
				if line[0] == "#":
					if line[1] == " ":
						if currentCommentFinished:
							currentComment = []
							currentCommentFinished = 0
						currentComment.append(line[2:])
				else:
					currentCommentFinished = 1
					featureType, featureVal = string.split(line, " ", 1)
					if featureType in ["fun", "get", "set"]:
						retType, name, value, param1, param2 = decodeFunction(featureVal)
						p1 = decodeParam(param1)
						p2 = decodeParam(param2)
						self.features[name] = {
							"FeatureType": featureType,
							"ReturnType": retType,
							"Value": value,
							"Param1Type": p1[0], "Param1Name": p1[1], "Param1Value": p1[2],
							"Param2Type": p2[0],	"Param2Name": p2[1], "Param2Value": p2[2],
							"Category": currentCategory, "Comment": currentComment
						}
						if self.values.has_key(value):
							raise "Duplicate value " + value + " " + name
						self.values[value] = 1
						self.order.append(name)
					elif featureType == "evt":
						retType, name, value, params = decodeEvent(featureVal)
						p1 = decodeParam(param1)
						p2 = decodeParam(param2)
						self.features[name] = {
							"FeatureType": featureType,
							"ReturnType": retType,
							"Value": value,
							"Params" : params,
							"Category": currentCategory, "Comment": currentComment
						}
						if self.events.has_key(value):
							raise "Duplicate event " + value + " " + name
						self.events[value] = 1
						self.order.append(name)
					elif featureType == "cat":
						currentCategory = featureVal
					elif featureType == "val":
						name, value = string.split(featureVal, "=", 1)
						self.features[name] = {
							"FeatureType": featureType,
							"Category": currentCategory,
							"Value": value }
						self.order.append(name)



def Contains(s,sub):
	return string.find(s, sub) != -1

def pType(s):
	if s == "bool":
		return "bool"
	elif s == "position":
		return "long"
	elif s == "colour":
		return "long"
	elif s == "string":
		return "const char*"
	elif s == "stringresult":
		return "char*"
	elif s == "cells":
		return "char*"
	elif s == "textrange":
		return "TextRange*"
	elif s == "findtext":
		return "TextToFind*"
	elif s == "keymod":
		return "long"
	elif s == "formatrange":
		return "long"
	elif s == "int":
		return "int"
	return ""

def fixConstant(c):
	#r = string.replace(c, "|", "or")
	#r = string.replace(r, "0x", "$")
	return c

def printDefines(f, out):
	for name in f.order:
		v = f.features[name]
		if v["Category"] != "Deprecated":
			if v["FeatureType"] in ["fun", "get", "set"]:
				featureDefineName = "SCI_" + string.upper(name)
				out.write("#define " + featureDefineName + " " + fixConstant(v["Value"]) + ";\n")
			elif v["FeatureType"] in ["evt"]:
				featureDefineName = "SCN_" + string.upper(name)
				out.write("#define " + featureDefineName + " " + fixConstant(v["Value"]) + ";\n")
			elif v["FeatureType"] in ["val"]:
				featureDefineName = string.upper(name)
				out.write("#define " + featureDefineName + " " + fixConstant(v["Value"]) + ";\n")

def genfuncPrototype(name, fun):
	retval = name
	retval = retval + "("
	bp1 = 0
	if fun["Param1Type"] != "" and fun["Param1Type"] != " ":
		retval = retval + pType(fun["Param1Type"]) + " " + fun["Param1Name"]
		bp1 = 1
	if fun["Param2Type"] != "" and fun["Param2Type"] != " ":
		if bp1 == 1:
			retval = retval + ", "
		retval = retval + pType(fun["Param2Type"]) + " " + fun["Param2Name"]
	retval = retval + ")"
	return retval

def getfuncHeader(name, fun, extra):
	retval = genfuncPrototype(name, fun)
	line = ""
	if pType(fun["ReturnType"]) == "":
		line = line + "void " + extra + retval
	else:
		line = line + pType(fun["ReturnType"]) + " " + extra + retval
	return line

def printFunctionDefs(f, out):
	for name in f.order:
		v = f.features[name]
		if v["Category"] != "Deprecated":
			if v["FeatureType"] in ["fun", "set", "get"]:
				line = ""
				if not v["Comment"] in ["", " "]:
					line = line + "		/**\n";
					
					for cline in v["Comment"]:
						line = line + "		 * " + cline + "\n";
					line = line + "		 */\n		";
				else:
					line = "		";
				line = line + getfuncHeader(name, v, "") + ";\n"
				out.write(line)

def isFunction(v):
	if pType(v["ReturnType"]) == "":
		ret = 0
	else:
		ret = 1
	return ret

def printFunctionImpl(f, out):
	for name in f.order:
		v = f.features[name]
		if v["Category"] != "Deprecated":
			if v["FeatureType"] in ["fun", "set", "get"]:
				header = getfuncHeader(name, v, "ScintillaControl::") + "\n    "
				out.write(header)
				out.write("{\n    ")
				line = "	"
				endline = ""
				if not isFunction(v):
					line = line + "SPerform("
				else:
					line = line + "return "
					if pType(v["ReturnType"]) != "long":
						if pType(v["ReturnType"]) != "bool":
							line = line + "(" + pType(v["ReturnType"]) + ")"
						else:
							endline = " != 0"
					line = line + "SPerform("
				line = line + "SCI_" + string.upper(name) + ", "
				if not (v["Param1Type"] in ["", " "]):
					add = ""
					if pType(v["Param1Type"]) != "long":
						line = line + "(long)"
					line= line + v["Param1Name"] + add + ", "
				else:
					line = line + "0, "
				if not (v["Param2Type"] in ["", " "]):
					add = ""
					if pType(v["Param2Type"]) != "long":
						line = line + "(long)"
					line = line + v["Param2Name"] + add + ")"
				else:
					line = line + "0)"
				line = line + endline + ";"
				out.write(line + "\n    }\n\n    ")

def genMainControl(input, output, definition):
	copying = 1
	for line in input.readlines():
		if copying:
			output.write(line)
		#if Contains(line, "//++Autogenerated"):
		#	copying = 0
		#	genfn(definition, output)
		if Contains(line, "//++EventTypes"):
			copying = 0
			printEventDefs(definition, output)
		if Contains(line,"//++EventPrivates"):
			copying = 0
			printEventPrivates(definition, output)
		if Contains(line, "//++EventProperties"):
			copying = 0
			printEventProperties(definition, output)
		if Contains(line,"//++FuncDef"):
			copying = 0
			printFunctionDefs(definition, output)
		if Contains(line,"//++Const"):
			copying = 0
			printDefines(definition, output)
		if Contains(line,"//++FuncImp"):
			copying = 0
			printFunctionImpl(definition, output)
		if Contains(line,"//++EventImpl"):
			copying = 0
			printEventImpl(definition, output)
		if Contains(line,"//--"):
			copying = 1
			output.write(line)

def genConsts(input, output, definition):
	copying = 1
	for line in input.readlines():
		if copying:
			output.write(line)
		if Contains(line,"//++Const"):
			copying = 0
			printDefines(definition, output)
		if Contains(line,"//--") and copying == 0:
			copying = 1
			output.write(line)

def Regenerate(filename, definition, fn):
	tempname = "PasFacer.tmp"
	out = open(tempname,"w")
	hfile = open(filename)
	#CopyWithInsertion(hfile, out, definition)
	fn(hfile, out, definition)
	out.close()
	hfile.close()
	os.unlink(filename)
	os.rename(tempname, filename)

# Program Start
f = Face()
f.ReadFromFile("..\scintilla\include\Scintilla.iface")
Regenerate("scintillaNet.cpp", f, genMainControl)
Regenerate("scintillaNet.h", f, genMainControl)
