# SinkWorld project
# Translate.py [clean]
# First stage of the build process of SinkWorld
# Expand the templated files in base and lexers
# Translate files in (base, lexers) into java versions in java
# Translate files in (base, lexers) into C# versions in csharp
# If the argument clean is used, all generated files are removed 
# along with object files in those directories
# Copyright 2005 Neil Hodgson

import glob
import os
import sys
import RegistryGen
import Template
import XLang

sizes = ["1", "2", "4"]
type = {"1":"SW_BYTE", "2":"SW_SHORT", "4":"int"}
exts = [".h", ".cxx"]
templateExts = {".h" : ".ht", ".cxx" : ".ct"}
root = os.path.dirname(os.getcwd())

def UnlinkGenerated(path, checkSignature):
	for name in glob.glob(path):
		if checkSignature:
			f = file(name, "rt")
			contents = f.read()
			f.close()
			if contents.startswith("// Automatically translated"):
				os.unlink(name)
		else:
			os.unlink(name)

def Clean():
	os.chdir(os.path.join(root, "base"))
	for e in exts:
		for n in sizes:
			name = os.path.join(root, "base", "SplitVector_" + n + e)
			#~ print name
			try:
				os.unlink(name)
			except OSError:	# Does not matter if the file does not exist
				pass
	UnlinkGenerated(os.path.join(root, "base", "*.obj"), 0)
	UnlinkGenerated(os.path.join(root, "lexers", "*.obj"), 0)
	UnlinkGenerated(os.path.join(root, "java", "*.java"), 1)
	UnlinkGenerated(os.path.join(root, "java", "*.class"), 0)
	UnlinkGenerated(os.path.join(root, "csharp", "*.cs"), 1)
	UnlinkGenerated(os.path.join(root, "tentacle", "cxx", "*.cxx"), 1)
	UnlinkGenerated(os.path.join(root, "tentacle", "java", "*.java"), 1)
	UnlinkGenerated(os.path.join(root, "tentacle", "java", "*.class"), 0)
	UnlinkGenerated(os.path.join(root, "tentacle", "csharp", "*.cs"), 1)
	
	
def Translate():
	
	RegistryGen.GenerateAll()
	
	print "%s:" % root
	os.chdir(os.path.join(root, "base"))
	print "%s:" % os.getcwd()
	for e in exts:
		templateFile = os.path.join(root, "base", "SplitVector" + templateExts[e])
		for n in sizes:
			name = os.path.join(root, "base", "SplitVector_" + n + e)
			argv = [templateFile, name, "N=" + n, "T="+type[n], "L=C~"]
			Template.expand(argv)
			
	os.chdir(os.path.join(root, "java"))
	print "%s:" % os.getcwd()
	XLang.translateArg(os.path.join(root, "base"), ".java")
	XLang.translateArg(os.path.join(root, "lexers"), ".java")
	
	os.chdir(os.path.join(root, "csharp"))
	print "%s:" % os.getcwd()
	XLang.translateArg(os.path.join(root, "base"), ".cs")
	XLang.translateArg(os.path.join(root, "lexers"), ".cs")

	os.chdir(os.path.join(root, "tentacle", "java"))
	print "%s:" % os.getcwd()
	XLang.translateArg(os.path.join(root, "tentacle", "cxx"), ".java")

	os.chdir(os.path.join(root, "tentacle", "csharp"))
	print "%s:" % os.getcwd()
	XLang.translateArg(os.path.join(root, "tentacle", "cxx"), ".cs")

if "clean" in sys.argv:
	Clean()
else:
	Translate()
