# SinkWorld project
# XLang.py
# Translate from a subset of C++ to either Java or C#
# Copyright 2001-2005 Neil Hodgson
import sys
import os
import glob
import cStringIO
import stat
import CxxTokens

recurse = 1

# Should discover the class names by looking through the files
classTypes = ["RGBColor", "Style", "FontDescription", "ByteBuffer",
	"Point", "PaintContext", "SRectangle", "Literal", "ChangeFinder", "OneCorner"]

def translateCode(tokens, state, methods):
	extraLines = []
	level = state["level"]
	startLevel = level
	for tok in tokens.tokens:
		if tok == "o}":
			level -= 1
		if tok == "o{":
			level += 1
	state["level"] = level

	if state["inDestructor"]:
		if level == 1:
			state["inDestructor"] = 0
		return "~", tokens.outComment, extraLines

	if tokens.findTokenStrings("kSW_ABSTRACT") >= 0:
		tokens.substTokenStrings("kSW_ABSTRACT", "abstract")
	if tokens.findTokenStrings("kSW_OVERRIDE") >= 0:
		if state["language"] == "Java":
			tokens.substTokenStrings("kSW_OVERRIDE~w", "")
		else:
			tokens.substTokenStrings("kSW_OVERRIDE", "override")

	# Remove all "const"
	tokens.substTokenStrings("kconst", "", 1)

	#print tokens.tokens
	if tokens.matchTokenStrings("kprivate~o:"):
		# Remove private sign but remember
		state["privacy"] = "private"
		#tokens = CxxTokens.CxxTokens(["l//" + tokens.flatten()])
		return "~", tokens.outComment, extraLines
	elif tokens.matchTokenStrings("kpublic~o:"):
		# Remove public sign but remember
		state["privacy"] = "public"
		#tokens = CxxTokens.CxxTokens(["l//" + tokens.flatten()])
		return "~", tokens.outComment, extraLines
	elif tokens.matchTokenStrings("kprotected~o:"):
		# Remove public sign but remember
		state["privacy"] = "protected"
		#tokens = CxxTokens.CxxTokens(["l//" + tokens.flatten()])
		return "~", tokens.outComment, extraLines
	elif tokens.findTokenStrings("kdelete") >= 0:
		# Remove delete
		tokens.substTokenStrings("kdelete", "// $0")
	elif tokens.findTokenStrings("ktypedef") >= 0:
		# Remove line as typedefs not available in C# or Java
		tokens.substTokenStrings("kdelete", "// $0")
		return "~", tokens.outComment, extraLines
	elif tokens.findTokenStrings("kclass") >= 0:
		# Start class in private state
		state["privacy"] = "private"
		if tokens.findTokenStrings("o;") >= 0:
			# Forward declaration -> not needed so remove
			return "~", tokens.outComment, extraLines
		else:
			posClass = tokens.findTokenStrings("kclass")
			state["class"] = tokens.tokens[posClass + 2][1:]
			start = tokens.findTokenStrings("kclass")
			# Convert "class" to "interface" if names starts with "I"
			tokens.substTokenStrings("kpublic", "")
			isInterface = 0
			for tok in tokens.tokens[start+1:]:
				if tok[0] == "i":
					if tok[1] == "I":
						isInterface = 1
						tokens.substTokenStrings("kclass", "interface")
					break
			# Change superclass declarations to "extends" for Java
			if state["language"] == "Java":
				foundOp = 0
				supers = []
				interfaces = []
				startList = 0
				endList = 0
				n = start+1
				for tok in tokens.tokens[start+1:]:
					if tok[0] == "o":
						if tok[1] != ",":
							foundOp += 1
							if foundOp == 1:
								startList = n
					if foundOp == 1 and tok[0] == "i":
						endList = n
						if tok[1] == "I":
							interfaces.append(tok)
						else:
							supers.append(tok)
					n += 1
				classes = []
				if supers:
					classes.append("kextends")
					classes.append("w ")
					classes.append(supers[0])
				if interfaces:
					if classes:
						classes.append("w ")
					if isInterface:
						classes.append("kextends")
					else:
						classes.append("kimplements")
					classes.append("w ")
					first = 1
					for inter in interfaces:
						if not first:
							classes.append("o,")
							classes.append("w ")
						classes.append(inter)
						first = 0
				tokens.tokens[startList:endList+1] = classes
	elif tokens.findTokenStrings("k~o*~i", 1) >= 0:
		# Change pointer declaations to array declarations
		tokens.substTokenStrings("k~o*~i", "$0[] $2", 1)
	elif tokens.findTokenStrings("w~i~w~o*~i~o;~w~l//Array") >= 0:
		# Change array * declarations marked with //Array comment to [] declarations
		tokens.substTokenStrings("w~i~w~o*~i~o;~w~l//Array", "$0$1[] $4;")
	elif tokens.findTokenStrings("i~o[~o]~o=~o{", 1) >= 0:
		# Change array * initialisations into [] declarations
		tokens.substTokenStrings("i~o[~o]~o=~o{", "[]$0 = {", 1)
	elif tokens.findTokenStrings("i~o*~i~o=~knew~i~o[~*~o]", 1) >= 0:
		# Change array * allocations with [] allocations
		tokens.substTokenStrings("i~o*~i~o=~knew~i~o[~*~o]", "$0[] $2 = new $0[$7]", 1)
	elif tokens.findTokenStrings("i~o*~i~o=~knew~i~o(", 1) >= 0:
		# Change class * allocations into class allocations
		tokens.substTokenStrings("i~o*~i~o=~knew", "$0 $2 = new ", 1)
	elif tokens.findTokenStrings("k~i~o[~*~o]", 1) >= 0:
		# Change array definitions into array declarations with allocations
		pos = tokens.findTokenStrings("k~i~o[~*~o]", 1)
		typeOfTok = tokens[pos]
		if typeOfTok[1:] in CxxTokens.baseTypes:
			tokens.substTokenStrings("k~i~o[~*~o]", "$0[] $1 = new $0[$3]", 1)
	elif tokens.findTokenStrings("i~o*~i~o=", 1) >= 0:
		# Pointer assignment
		tokens.substTokenStrings("i~o*~i~o=", "$0 $2 =", 1)
	elif tokens.findTokenStrings("o*~o*~i", 1) >= 0:
		# Array of pointers
		tokens.substTokenStrings("o*~o*", "[]")
	elif tokens.findTokenStrings("o*~i", 1) >= 0:
		# Possible pointer declaration
		if (tokens.findTokenStrings("o=") < 0) or (tokens.findTokenStrings("o=~m0~o;", 1) > 0):
			tokens.substTokenStrings("o*~i", " $1", 1)
	elif tokens.findTokenStrings("kenum~o{~i~o=~m~o}", 1) >= 0:
		# Turn enums into const or final depending on language
		if state["language"] == "Java":
			tokens.substTokenStrings("kenum~o{~i~o=~m~o}", "static final int $2 = $4", 1)
		else:
			tokens.substTokenStrings("kenum~o{~i~o=~m~o}", "const int $2 = $4", 1)
	elif tokens.findTokenStrings("kenum~o{~i~o=~o-~m~o}", 1) >= 0:
		# Turn negative enums into const or final depending on language
		if state["language"] == "Java":
			tokens.substTokenStrings("kenum~o{~i~o=~o-~m~o}", "static final int $2 = $5", 1)
		else:
			tokens.substTokenStrings("kenum~o{~i~o=~o-~m~o}", "const int $2 = $5", 1)

	# Remove address of "&" before identifiers
	tokens.substTokenStrings("o&~i", "$1")

	# Remove any remaining "*" before identifiers
	tokens.substTokenStrings("o*~i", "$1")

	# Convert pointer casts
	tokens.substTokenStrings("o(~i~o*~o)", "($1)", 1)

	# Remove any remaining "*" before arrays
	tokens.substTokenStrings("o*~o[", "[")

	# Change form of array declarations to have the [] before the identifier
	tokens.substTokenStrings("k~i~o[~o]", "$0[] $1", 1)

	# Change :: to .
	tokens.substTokenStrings("o::", ".")

	# For Java. "bool" is "boolean"
	if state["language"] == "Java":
		tokens.substTokenStrings("kbool", "boolean")
		tokens.substTokenStrings("iSW_ARRAYCOPY", "System.arraycopy")
		tokens.substTokenStrings("kSW_OBJECT", "Object")
	else:	# C#
		tokens.substTokenStrings("iSW_ARRAYCOPY", "System.Array.Copy")
		tokens.substTokenStrings("iassert", "System.Diagnostics.Debug.Assert")
		tokens.substTokenStrings("kSW_OBJECT", "object")
	tokens.substTokenStrings("kSW_BYTE", "byte")
	tokens.substTokenStrings("kSW_SHORT", "short")
	classConstructor = "i" + state["class"] + "~o("
	if startLevel > 1:
		# For implementation code, change value class initialisation into allocation with new.
		for cl in classTypes:
			tokens.substTokenStrings("i" + cl + "~o(", "new " +cl + "(")

	# Remove generated "new new".
	tokens.substTokenStrings("knew~knew", "new", 1)

	if startLevel == 1 and level > 0:
		# For abstract methods, remove abstract indicator, and public and virtual keywords
		if tokens.findTokenStrings("o)~o=~m0~o;", 1) >= 0:
			# Abstract
			privacy = ["k" + state["privacy"], "w "]
			if not state["class"].startswith("I"):
				if tokens.findTokenStrings("k~i~o(", 1) >= 0:
					tokens.substTokenStrings("k~i~o(", "abstract $0 $1(", 1)
				else:
					tokens.substTokenStrings("i~i~o(", "abstract $0 $1(", 1)
			tokens.substTokenStrings("o)~o=~m0", ")", 1)
			tokens.substTokenStrings("kvirtual~w", "")
		else:
			privacy = ["k" + state["privacy"], "w "]
			if (state["language"] == "Java"):
				tokens.substTokenStrings("kvirtual~w", "")

		# Feature definition level
		if tokens.findTokenStrings("o=") >= 0:
			# Assignment on line
			if tokens.findTokenStrings("o=~d", 1) >= 0:
				# Assignment of string
				tokens.substTokenStrings("o=~d", "= $1.ToCharArray()", 1)
		else:
			# No assignment on line
			isFunction = 0
			if tokens.findTokenStrings("o(") >= 0 and \
				tokens.findTokenStrings("o)") >= 0 and \
				((tokens.findTokenStrings("o{") >= 0) or
				(tokens.findTokenStrings("o;") >= 0)) :
				isFunction = 1
			if tokens.findTokenStrings("kwhile") >= 0 or \
				tokens.findTokenStrings("kif") >= 0 or \
				tokens.findTokenStrings("kfor") >= 0 or \
				tokens.findTokenStrings("kswitch") >= 0 :
				isFunction = 0
			if tokens.findTokenStrings("o~~") >= 0:
				# Finalizers in C# do not like public
				isFunction = 0
				if tokens.findTokenStrings("o;") < 0:
					state["inDestructor"] = 1
				return "~", tokens.outComment, extraLines
			if isFunction and (tokens.findTokenStrings("o;") >= 0):
				fnamePos = tokens.findTokenStrings("i~o(", 1)
				fname = tokens.tokens[fnamePos][1:]
				methodName = state["class"] + "::" + fname
				if methods.has_key(methodName):
					tokens.substTokenStrings("o;", " {")
					extraLines = methods[methodName]
					level += 1
					state["level"] = level

		# Insert privacy level to feature
		if not state["class"].startswith("I"):	# Interface so remove public
			tokens.tokens = tokens.tokens[:1] + privacy + tokens.tokens[1:]

	# Normalise whitespace after indentation to be single spaces
	tokens.substTokenStrings("w", " ", 0, 1)

	# C++ -> C# or Java, -> becomes .
	tokens.substTokenStrings("o->", ".")

	# Change strlen calls into Length attributes
	if state["language"] == "Java":
		tokens.substTokenStrings("istrlen~o(~i~o)", "$2.length", 1)
	else:
		tokens.substTokenStrings("istrlen~o(~i~o)", "$2.Length", 1)

	# Change super. into base. for C#
	if state["language"] == "C#":
		tokens.substTokenStrings("isuper~o.", "base.", 1)

	# Make public classes public
	#~ if state["language"] == "Java":
	tokens.substTokenStrings("kSW_PUBLIC", "public")
	#~ else:
		#~ tokens.substTokenStrings("kSW_PUBLIC", "")

	# Change NULL to null
	tokens.substTokenStrings("iNULL", "null")

	# Change time_t to long
	tokens.substTokenStrings("itime_t", "long")

	# Remove ';' after '}' for end of class.
	if tokens.findTokenStrings("o}~o;") == 0:
		tokens.substTokenStrings("o}~o;", "}")

	# Change time(0) to DateTime.Now/1000
	tokens.substTokenStrings("itime~o(~m0~o)", "DateTime.Now/1000")

	return tokens.flatten(), tokens.outComment, extraLines

def translateLine(line, inComment, state, methods):
	line = line.rstrip()
	tokens = CxxTokens.CxxTokens(line, inComment)
	hasCode = 0
	extraLines = []
	for tok in tokens.tokens:
		if tok[0] in CxxTokens.codeTypes:
			hasCode = 1
	if hasCode:
		lline = line.lstrip()
		if lline.startswith("#include"):
			line = "//" + line
		elif lline.startswith("#"):
			if lline.startswith("#ifdef"):
				sym = lline[len("#ifdef") + 1:].strip()
				if state.has_key(sym) and state[sym]:
					state["copying"] = 1
				else:
					state["copying"] = 0
			elif lline.startswith("#else"):
				state["copying"] = not state["copying"]
			elif lline.startswith("#endif"):
				state["copying"] = 1
			line = "~"
		elif state["copying"]:
			line, inComment, extraLines = translateCode(tokens, state, methods)
		else:
			line = "~"
	return line, inComment, extraLines

# Methods start with a line containing "::" and starting at left and continue
# until a line that starts with "}"
def methodsIn(sourceName):
	methods = {}
	try:
		cfile = file(sourceName, "rt")
		currentMethodName = ""
		for line in cfile.readlines():
			line = line.rstrip()
			colons = line.find("::")
			if colons >= 0 and line[0] not in " \t/":
				startName = line.rfind(' ', 0, colons)
				if startName < 0:
					startName = -1
				endName = line.find('(', colons)
				if endName < 0:
					endName = len(line)
				currentMethodName = line[startName+1:endName]
				if currentMethodName[0] == "*":
					currentMethodName = currentMethodName[1:]
				methods[currentMethodName] = []
			elif currentMethodName:
				indentedLine = "\t" + line
				methods[currentMethodName] += [indentedLine]
			if currentMethodName and line and line[0] == '}':
				currentMethodName = ""
	finally:
		return methods

def replaceFile(name, contents, force=False):
	if force:
		currentContents = None
	else:
		try:
			current = file(name, "rt")
			currentContents = current.read()
			current.close()
		except IOError:	# No such file is OK
			currentContents = None
	if contents != currentContents:	# only write file if really changed
		temporaryName = name[:name.rfind(".")] + ".tmp"
		try:
			os.unlink(temporaryName)
		except OSError:	# Does not exist yet, so can create OK.
			pass
		temporary = file(temporaryName, "wt")
		temporary.write(contents)
		temporary.close()
		try:
			os.unlink(name)
		except OSError:	# Does not exist yet, so can create OK.
			pass
		os.rename(temporaryName, name)
		return True
	else:
		return False

def dependentNewer(sourceFile, dependentFile):
	try:
		sourceTime = os.stat(sourceFile).st_mtime
	except OSError: # Source does not exist, no need to update
		sourceTime = 0
	try:
		dependentTime = os.stat(dependentFile).st_mtime
	except OSError: # Dependent does not exist, so must update if source exists
		dependentTime = 0
	return dependentTime > sourceTime

def translateFile(headerName, sourceName, outName):
	lang = "C#"
	if outName.endswith(".java"):
		lang = "Java"
	lastLine = 1
	sources = os.path.basename(headerName)
	print "  Translating to %4s: %-24s" % (lang, os.path.basename(headerName)),
	if dependentNewer(headerName, outName) and dependentNewer(sourceName, outName):
		print "No change"
		return
	methods = methodsIn(sourceName)
	if methods:
		sources += " and " + os.path.basename(sourceName)
	out = cStringIO.StringIO()
	hfile = file(headerName, "rt")
	out.write("// Automatically translated by XLang.py to " \
		+ lang + " from " + sources + ".\n")
	if lang == "C#":
		out.write("namespace SinkWorld {\n")
	else:
		out.write("package SinkWorld;\n")
	
	#out.write("using System;\n")
	inComment = ""
	state = { "privacy" : "private",
		"inDestructor" : 0,
		"copying" : 1,
		"level" : 0,
		"SW_CXX": 0,
		"SW_CS": 0,
		"SW_JAVA": 0,
		"language": lang,
		"class": ""}
	if lang == "Java":
		state["SW_JAVA"] = 1
	else:
		state["SW_CS"] = 1
	for line in hfile.readlines():
		line, inComment, extraLines = translateLine(line, inComment, state, methods)
		#print line
		if line != "~":
			if line or lastLine:
				out.write(line)
				out.write("\n")
			if extraLines:
				for line in extraLines:
					line, inComment, extraLines = translateLine(line, inComment, state, methods)
					if line and line != "~":
						out.write(line)
						out.write("\n")
				out.write("\n")
			lastLine = line
	if lang == "C#":
		out.write("}\n")
	text = out.getvalue()
	out.close()
	if replaceFile(outName, text, True):
		print "Modified"
	else:
		print "No content change"

# Try to use psyco to speed up processing but doesn't matter if not available
try:
	from psyco.classes import *
	import psyco
	psyco.profile()
except ImportError:
	pass

def translateArg(arg, outExt):
	if os.path.isdir(arg):
		# For a directory argument, translate all files that contain the text marker C~
		for header in glob.glob(os.path.join(arg, "*.h")):
			hfile = file(header, "rt")
			text = hfile.read()
			hfile.close()
			if text.count("C~"):
				translateArg(header, outExt)
			else:
				print "  Not C~:", header
	else:
		dir, basename = os.path.split(arg)
		name, ext = os.path.splitext(basename)
		#~ print methodsIn(os.path.join(dir, name + ".cxx"))
		translateFile(os.path.join(dir, name + ".h"),
			os.path.join(dir, name + ".cxx"),
			name + outExt)

if __name__ == "__main__":
	if len(sys.argv) > 1:
		outExt = ".cs"
		for arg in sys.argv[1:]:
			if arg == "-c":
				outExt = ".cs"
			elif arg == "-j":
				outExt = ".java"
			else:
				translateArg(arg, outExt)
				break	# Only handle one file at a time
