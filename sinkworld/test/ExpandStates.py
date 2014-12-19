# Expand states into text forms
# ExpandStates.py [-a|-s|-t] files
# -a indicates that all lexers should be shown
# -s indicates that the lexers should not be shown
# -t indicates that the top level lexer should not be shown [default]
import sys

def expandStates(name, lexers):
	readingDefs = True
	defs = {}
	for l in file(name):
		l = l.rstrip("\n")
		if readingDefs:
			if l.startswith("{"):
				readingDefs = False
			else:
				l.strip()
				n, v = l.split("=")
				n = "{" + n + "}"
				if lexers == 0:
					v = v.split(".")[-1]
				elif lexers == 1:
					v = ".".join(v.split(".")[1:])
				v = "{" + v + "}"
				defs[n] = v
				print l
		if not readingDefs:
			for d in defs.keys():
				l = l.replace(d, defs[d])
			print l
	
if __name__ == "__main__":
	lexers = 1
	for arg in sys.argv[1:]:
		if arg == "-a":
			lexers = 2
		elif arg == "-s":
			lexers = 0
		elif arg == "-t":
			lexers = 1
		else:
			expandStates(arg, lexers)
