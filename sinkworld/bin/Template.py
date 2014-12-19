# SinkWorld project
# Template.py
# Usage:
# Template.py input_file output_file [definition=expansion]*
# Expand a templated source file using a set of command line arguments.
# If the command line has T=SW_BYTE then each <T> string in the template file is expanded 
# to SW_BYTE in the output
# Copyright 2002 Neil Hodgson
import sys
import os

def expand(argv):
	infile = argv[0]
	#print infile
	text = file(infile, "rb").read()
	outfile = argv[1]
	#print outfile
	for arg in argv[2:]:
		definition, expansion = arg.split("=")
		#print definition, expansion
		text = text.replace("<" + definition + ">", expansion)
	try:
		originalText = file(outfile, "rb").read()
	except IOError:
		originalText = None
	if text != originalText:
		print "Expanding %s to %s" % \
			(os.path.basename(infile), os.path.basename(outfile))
		file(outfile, "wb").write(text)

if __name__ == "__main__":
	if len(sys.argv) > 2:
		expand(sys.argv[1:])
	else:
		print "Usage:"
		print "Template.py input_file output_file [definition=expansion]*"
