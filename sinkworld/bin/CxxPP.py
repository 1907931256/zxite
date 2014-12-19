# SinkWorld project
# CxxPP.py
# Format C++ code - demonstration of CxxTokens class
# Copyright 2001 Neil Hodgson
import sys
import os
import stat
import CxxTokens

tempname = "c:\\os\\SinkWorld\\GenC.cxx"
recurse = 1

def fixCode(tokens, level, tempindent):
	#print tokens.tokens
	# Normalise whitespace after indentation to be single spaces
	tokens.substTokenStrings("w", " ", 0, 1)
	
	# First remove as much whitespace as lexically safe
	# Remove spaces between ...
	#    identifiers and operators
	tokens.substTokenStrings("i~o", "$0$1", 1)
	tokens.substTokenStrings("o~i", "$0$1", 1)
	#    numbers and operators
	tokens.substTokenStrings("m~o", "$0$1", 1)
	tokens.substTokenStrings("o~m", "$0$1", 1)
	#    keywords and operators
	tokens.substTokenStrings("k~o", "$0$1", 1)
	tokens.substTokenStrings("o~k", "$0$1", 1)
	# Put the whitespace back where needed
	# Space after keywords
	tokens.substTokenStrings("k", "$0 ")
	# No space before ':'
	tokens.substTokenStrings("w~o:", "$1")
	# Space after commas
	tokens.substTokenStrings("o,", "$0 ")
	# Space after semicolons - at end of line will be removed by rstrip
	tokens.substTokenStrings("o;", "$0 ")
	# Space before {
	tokens.substTokenStrings("o{", " {")
	# Space after {, if at end of line wil be removed later
	tokens.substTokenStrings("o}", "} ")
	# Space before pointer *
	tokens.substTokenStrings("k~o*", "$0 *", 1)
	# Spaces around assignment and comparison operators
	for op in ["=", "==", "<", ">", ">=", "<=", "+", "-"]:
		tokens.substTokenStrings("o" + op, " " + op + " ")

	# Restore includes to not have spaces inside <>
	if tokens.findTokenStrings("o#~iinclude") >= 0:
		tokens.substTokenStrings("w~o>", ">")
		tokens.substTokenStrings("o<~w", "<")
	
	# Drop space between } and ;
	tokens.substTokenStrings("o}~o;", "};", 1)

	# Renormalise to single spacing where earlier substitutions overdid it.
	tokens.substTokenStrings("w", " ", 0, 1)
	
	for tok in tokens.tokens:
		if tok == "o}":
			level -= 1

	levelnow = level + tempindent
	tempindent = 0
	
	# Remove indentation and recreate
	if tokens.tokens[0].startswith("w"):
		tokens.tokens = tokens.tokens[1:]
	if tokens.tokens[0] in ["kprivate", "kpublic", "kprotected"]:
		levelnow -= 1
	tokens.tokens = ["w" + "\t" * levelnow] + tokens.tokens
	
	for tok in tokens.tokens:
		if tok == "o{":
			level += 1
			tempindent = 0
		elif tok == "kclass":
			tempindent = 1
		elif tok == "kdo":
			tempindent = 1
		elif tok == "kfor":
			tempindent = 1
		elif tok == "kif":
			tempindent = 1
		elif tok == "kwhile":
			tempindent = 1
	
	#print tokens.tokens

	return tokens.flatten().rstrip(), tokens.outComment, level, tempindent

def fixLine(line, inComment, level, tempindent):
	#print line
	line = line.rstrip()
	tokens = CxxTokens.CxxTokens(line, inComment)
	#print tokens.tokens
	hasCode = 0
	for tok in tokens.tokens:
		if tok[0] in CxxTokens.codeTypes:
			hasCode = 1
	if hasCode:
		line, inComment, level, tempindent = fixCode(tokens, level, tempindent)
	return line, inComment, level, tempindent
	
def fixFile(filename):
	out = open(tempname, "wt")
	cfile = open(filename, "rt")
	lastLine = 1
	print "processing", filename
	inComment = ""
	level = 0
	tempindent = 0
	for line in cfile.readlines():
		line, inComment, level, tempindent = fixLine(line, inComment, level, tempindent)
		#print line
		if line or lastLine:
			out.write(line)
			out.write("\n")
		lastLine = line
	out.close()
	cfile.close()
	#os.unlink(filename)
	#os.rename(tempname, filename)

def fixDir(dir, extensions):
	print "dir", dir
	for filename in os.listdir(dir):
		for ext in extensions:
			if not filename.count(".orig") and filename.count(ext):
				fixFile(dir + os.sep + filename)
	if recurse:
		for filename in os.listdir(dir):
			dirname =  dir + os.sep + filename
			#print ":", dirname
			if stat.S_ISDIR(os.stat(dirname)[stat.ST_MODE]):
				fixDir(dirname, extensions)

#os.chdir("\\os\\Updates\\SciTE-1.36+pl01")
if len(sys.argv) > 1:
	fixFile(sys.argv[1])
else:
	#fixDir(os.getcwd(), [".cxx", ".h"])
	fixFile("c:\\os\\SinkWorld\\CBA.cxx")
