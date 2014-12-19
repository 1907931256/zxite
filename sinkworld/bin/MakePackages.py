# SinkWorld project
# MakePackages.py
# Archive files into packages for uploading.

import glob
import os
import sys
import zipfile

version = 6
versionString = "%1d" % version

root = os.path.dirname(os.getcwd())
packageDirectory = os.path.join(os.path.dirname(root), "packages")

srcExts = [".ct", ".ht", ".cxx", ".cpp", ".h", ".rc", ".py", ".bat",
	".cvsignore",  ".config", ".MF", ".sln", ".vcproj",
	".jam", ".asp", ".html", ".css", ".ico", ".png", ".jpg",
	".mode", ".options", ".styled", ".properties", ""]
objExts = [".obj", ".o", ".lib", ".exe", ".ilk", ".pdb", ".exp",
	".res", ".idb", ".aps", ".sconsign",
	".CMD",
	".dll", ".pyd", ".pyc", ".class", ".jar",
	".output", ".run", ".test"]
autoExts = [".cs", ".java"]

def PackageSource():
	name = os.path.join(packageDirectory, "sinkworld%s.zip" % versionString)
	z = zipfile.ZipFile(name, "w", zipfile.ZIP_DEFLATED)
	for (p,d,f) in os.walk(root):
		reld = p[len(os.path.dirname(root))+1:]
		#~ print "relative", reld
		if "CVS" not in reld:
			for n in f:
				ext = os.path.splitext(n)[1]
				absp = os.path.join(p,n)
				relp = os.path.join(reld,n)
				if not n.startswith("SplitVector_") or ("0" in n):
					if n == "tentacle": # Do not include linux executable in sources
						pass
					elif ext in srcExts or n.startswith("x."):
						#~ if ext == "":
							#~ print absp, relp
						#~ print absp, relp
						z.write(absp, relp)
					elif ext in autoExts:
						# Include if not generated
						sf = file(absp, "rt")
						contents = sf.read()
						sf.close()
						if not contents.startswith("// Automatically translated"):
							#~ print relp
							z.write(absp, relp)
					#~ elif ext not in objExts:
						#~ print "Unknown file", os.path.join(reld,n)
	z.close()

optFiles = ["user.options", "*.mode"]

def AddOptions(z):
	for pat in optFiles:
		for name in glob.glob(os.path.join(root, "tentacle", pat)):
			z.write(name, os.path.join("tentacle", os.path.basename(name)))

def PackageEXE():
	name = os.path.join(packageDirectory, "TentacleW%s.zip" % versionString)
	z = zipfile.ZipFile(name, "w", zipfile.ZIP_DEFLATED)
	z.write(os.path.join(root, "tentacle/cxx/Release/Tentacle.exe"),
		os.path.join("tentacle", "Tentacle.exe"))
	AddOptions(z)
	z.close()

def PackageLinEXE():
	name = os.path.join(packageDirectory, "TentacleG%s.zip" % versionString)
	z = zipfile.ZipFile(name, "w", zipfile.ZIP_DEFLATED)
	z.write(os.path.join(root, "tentacle/cxx/tentacle"),
		os.path.join("tentacle", "tentacle"))
	AddOptions(z)
	z.close()

def PackageJava():
	name = os.path.join(packageDirectory, "TentacleJ%s.zip" % versionString)
	z = zipfile.ZipFile(name, "w", zipfile.ZIP_DEFLATED)
	z.write(os.path.join(root, "tentacle/java/tentacle.jar"),
		os.path.join("tentacle", "tentacle.jar"))
	AddOptions(z)
	z.close()

def PackageJython():
	name = os.path.join(packageDirectory, "PentacleJ%s.zip" % versionString)
	z = zipfile.ZipFile(name, "w", zipfile.ZIP_DEFLATED)
	for name in glob.glob(os.path.join(root, "tentacle", "java", "*.py")):
		z.write(name, os.path.join("tentacle", os.path.basename(name)))
	for name in glob.glob(os.path.join(root, "tentacle", "java", "SinkWorld", "*.class")):
		z.write(name, os.path.join("tentacle", "SinkWorld", os.path.basename(name)))
	AddOptions(z)
	z.close()

def PackageNet():
	name = os.path.join(packageDirectory, "TentacleN%s.zip" % versionString)
	z = zipfile.ZipFile(name, "w", zipfile.ZIP_DEFLATED)
	z.write(os.path.join(root, "tentacle/csharp/TentacleN.exe"),
		os.path.join("tentacle", "TentacleN.exe"))
	AddOptions(z)
	z.close()

def PackageIronPython():
	name = os.path.join(packageDirectory, "PentacleN%s.zip" % versionString)
	z = zipfile.ZipFile(name, "w", zipfile.ZIP_DEFLATED)
	for name in glob.glob(os.path.join(root, "tentacle", "csharp", "*.py")):
		z.write(name, os.path.join("tentacle", os.path.basename(name)))
	z.write(os.path.join(root, "tentacle/csharp/SinkWorld.dll"),
		os.path.join("tentacle", "SinkWorld.dll"))
	AddOptions(z)
	z.close()

def PackagePython():
	name = os.path.join(packageDirectory, "PentacleW%s.zip" % versionString)
	z = zipfile.ZipFile(name, "w", zipfile.ZIP_DEFLATED)
	for name in glob.glob(os.path.join(root, "tentacle", "python", "*.py")):
		if "GTK" not in name:
			z.write(name, os.path.join("tentacle", os.path.basename(name)))
	z.write(os.path.join(root, "tentacle/python/SinkWorld.pyd"),
		os.path.join("tentacle", "SinkWorld.pyd"))
	z.write(os.path.join(root, "tentacle/python/boost_python.dll"),
		os.path.join("tentacle", "boost_python.dll"))
	AddOptions(z)
	z.close()

def PackageGTKPython():
	name = os.path.join(packageDirectory, "PentacleG%s.zip" % versionString)
	z = zipfile.ZipFile(name, "w", zipfile.ZIP_DEFLATED)
	for name in glob.glob(os.path.join(root, "tentacle", "python", "*.py")):
		if "Win" not in name:
			z.write(name, os.path.join("tentacle", os.path.basename(name)))
	z.write(os.path.join(root, "tentacle/python/SinkWorld.so"),
		os.path.join("tentacle", "SinkWorld.so"))
	AddOptions(z)
	z.close()

try:
	os.mkdir(packageDirectory)
except OSError:
	pass
PackageSource()
PackageEXE()
PackageLinEXE()
PackageJava()
PackageJython()
PackageNet()
PackageIronPython()
PackagePython()
PackageGTKPython()
