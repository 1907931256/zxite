
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <algorithm>
#include <string>
#include <map>
#include <list>
#include <vector>

#include <iostream> //dbg
#include <iomanip> //dbg

#include "Scintilla.h"

#include "GUI.h"
#include "SString.h"
#include "StringList.h"
#include "FilePath.h"
#include "PropSetFile.h"
#include "StyleWriter.h"
#include "Extender.h"
#include "SciTE.h"
#include "Mutex.h"
#include "JobQueue.h"
#include "Project.h"
#include "SciTEBase.h"


#ifdef unix
static const char SEP = '/';
#endif
#ifdef WIN32
// Windows
static const char SEP = '\\';
#endif


bool glob_check(const char *glob) {
	const unsigned char *g = (const unsigned char *) glob;

	if (g == NULL)
		return false;

	while (*g) {
		if (*g == '*' && *++g == '*' && *++g == '*') {
			// Only 2 '*' max
			return false;
		} else if (*g == '\\') {
			if (*++g == '\0')
				// escape char cannot ends the glob
				return false;
		} else if (*g == '[') {
			if (*++g == '!' || *g == '^')
				g++;
			int range_beg = false;
			if (*g == '-' || *g == ']') {
				g++;
				range_beg = true;
			}

			while (*g != ']') {
				if (*g == '\0')
					// '[' must be matched by ']'
					return false;
				else if (*g == '-' && g[1] != ']') {
					if (!range_beg)
						// invalid range
						return false;
					g++;
					range_beg = false;
				} else {
					range_beg = true;
				}
				g++;
			}
			g++;
		} else if (*g == SEP && *++g == SEP) {
			// No consecutive separators
			return false;
		} else {
			g++;
		}
	}

	return true;

}




// Assume the glob is valid
bool glob_match(const char *str, const char *glob) {
	const unsigned char *s = (const unsigned char *)str;
	const unsigned char *g = (const unsigned char *)glob;
	const unsigned char *s0 = (const unsigned char *)s;

	for (;;) {
		if (*g == '*') {
			char sep = SEP;
			bool all_char = true;

			if (*++g == '*') {
				sep = '\0';
				// "**/" count for zero or more
				if (*++g == SEP && (g[1] != '\0' || (s == s0 || s[-1] == SEP))) {
					g++;
					all_char = false;
				}
			} else if ((s == s0 || s[-1] == SEP) && (*s == '\0' || *s == SEP))
					return false;

			for (;;) {
				if (all_char || s == s0 || s[-1] == SEP)
					if (glob_match((char*)s, (char*)g))
						return true;

				if (*s == '\0' || *s == sep)
					return false;

				s++;
			}

		} else if (*g == '?') {
			if (*s == SEP || *s == '\0')
				return false;

		} else  if (*g == '\\') {
			if (*++g != *s)
				return false;

		} else if (*g == '[') {
			bool inv = false;
			if (*++g == '!' || *g == '^') {
				inv = true;
				g++;
			}

			if (*g == '-' || *g == ']') {
				if (*s == *g)
					goto brace_match;
				else
					g++;
			}

			for (;;) {
				if (*g == '-') {
					if (g[1] == ']') {
						if (*s == *g)
							goto brace_match;
					} else {
						g++;
						if (  (g[-2] <= *s && *s <= *g)
						   || (*g <= *s && *s <= g[-2]))
							goto brace_match;
					}
				} else if (*g == *s) {
				brace_match:
					if (inv)
						return false;
					while (*++g != ']')
						;
					break;

				} else if (*g == ']') {
					if (!inv)
						return false;
					break;
				}
				g++;
			}
		} else {
			if (*g != *s) {
				// ignore potential trailing "/**"
				if (*s =='\0' && g[0] == SEP && g[1] == '*' && g[2] == '*' && g[3] == '\0')
					return true;
				// usual case: normal chars are !=
				return false;

			} else if (*g == '\0') {
				// end of glob
				return true;

			}
			// here: normal chars are ==
		}
		s++;
		g++;
	}

}




bool ImportRule::operator<(const ImportRule &other) const
{
	return strcmp(dir.c_str(), other.dir.c_str()) < 0;
}

bool ImportRule::operator==(const ImportRule &other) const
{
	if (0)
	printf(" ~~~~~ <%s[%lu] == %s[%lu] ?>   <%s[%lu] == %s[%lu] ?>   -> %x / %x ~~~~~ \n",
		dir.c_str(), dir.size(), other.dir.c_str(), other.dir.size(),
		glob.c_str(), glob.size(), other.glob.c_str(), other.glob.size(),
		dir == other.dir, glob == other.glob);

	return (dir == other.dir) && (!glob || !other.glob || glob == other.glob);
}

bool Project::open(FilePath file)
{
	if (opened)
		close();

	propFile.Read(file, file.Directory());

	name = propFile.Get("proj.name");
	if (!name.size()) {
		msg.assign(GUI_TEXT("Cannot find project name in ")) + file.AsInternal();
		return false;
	}


	path = propFile.Get("proj.path").c_str();
	//std::cout <<"path: " <<path.AsInternal() <<"\n";

	if (!path.IsAbsolute())
		path = file.Directory() + path.AsInternal();
	//std::cout <<"path2: " <<path.AsInternal() <<"\n";

	if (!path.IsDirectory()) {
		msg.assign(GUI_TEXT("Project path does not point to a valid directory"));
		return false;
	}

	// Process all include rules
	SString value;
	int i = 0;
	while ((value = propFile.Get(SString("proj.include.").append(i++).c_str()))) {

		int fpos = value.search(";");
		int pos = fpos + 1;

		SString dir(value.substr(0, fpos).c_str());

		// if no ';' or after first pos
		if (fpos)
			do {
				int npos = value.search(";", pos);

				FilePath full(dir.c_str());
				if (pos)
					full += value.substr(pos, npos - pos).c_str();
				else
					full += "**";
				SString glob = full.NormalizePath().AsInternal();
				if (glob_check(glob.c_str()))
					ruleInclude.push_back(ImportRule(dir, glob.c_str()));

				pos = npos + 1;
			} while (pos);
	}

	ruleInclude.sort();
	ruleInclude.unique();

	// Process all exclude rules
	i = 0;
	while ((value = propFile.Get(SString("proj.exclude.").append(i++).c_str()))) {
		int pos = 0;
		do {
			int npos = value.search(";", pos);
			FilePath path = value.substr(pos, npos - pos).c_str();

			if (path.IsFilename())
				path = FilePath("**") + path + "**";

			SString glob = path.NormalizePath().AsInternal();

			if (glob_check(glob.c_str()))
				ruleExclude.push_back(glob.c_str());

			pos = npos + 1;
		} while (pos);
	}

	ruleExclude.sort();
	ruleExclude.unique();

	// dbg
	#if 0
	std::cout <<"Rule Include:\n";
	for (std::list<ImportRule>::iterator it = ruleInclude.begin(); it != ruleInclude.end(); it++)
    		std::cout <<"   " <<it->dir.c_str() <<'\t' <<it->glob.c_str() <<'\n';

	std::cout <<"Rule Exclude:\n";
	for (std::list<SString>::iterator it = ruleExclude.begin(); it != ruleExclude.end(); it++)
    		std::cout <<"   " <<it->c_str() <<'\t' <<it->c_str() <<'\n';
	#endif

	Populate();

	// dbg
	#if 0
	i = 0;
	std::cout <<"Files:\n";
	for (std::vector<FilePath>::iterator it = files.begin(); it != files.end(); it++)
    		std::cout <<std::setw(3) <<i++ <<"   " <<it->AsInternal() <<'\n';
	#endif

	opened = true;
	return true;
}



bool Project::close()
{

	propFile.Clear();
	name.clear();
	ruleInclude.clear();
	ruleExclude.clear();
	//ruleFiles.clear();

	files.clear();

	opened = false;
	return true;

}


void Project::FillFiles(FilePath dir, SString glob, bool recursive, int prefix) {

	FilePathSet dirSet, fileSet;
	FilePath realDir = path + dir;
	realDir.List(dirSet, fileSet);

	//std::cout <<"   " << dir.AsInternal() <<' ' << dirSet.Length() <<' ' << fileSet.Length()<<'\n';

	for (size_t i = 0; i < dirSet.Length(); i ++)
		FillFiles(dirSet.At(i).AsInternal() + prefix, glob, recursive, prefix);

	for (size_t i = 0; i < fileSet.Length(); i ++) {
		FilePath entry = fileSet.At(i).AsInternal() + prefix;
		entry = entry.NormalizePath();

    		//std::cout <<"   A: " <<entry.AsInternal() <<" ?? " <<glob.c_str() <<'\n';

		if (!glob_match(entry.AsInternal(), glob.c_str()))
			continue;

    		//std::cout <<"   B: " <<entry.AsInternal() <<'\n';

		std::list<SString>::iterator ex;
		for (ex = ruleExclude.begin(); ex != ruleExclude.end(); ex++)
			if (glob_match(entry.AsInternal(), ex->c_str()))
				break;

    		//std::cout <<"   C: " <<entry.AsInternal() <<'\n';

		if (ex != ruleExclude.end())
			continue;

    		//std::cout <<"   D: " <<entry.AsInternal() <<'\n';

		files.push_back(entry.NormalizePath());
		//std::cout <<"     - " << entry.AsInternal() <<'\n';
	}

}


void Project::Populate()
{
	int prefix = strlen(path.AsInternal()) + 1;

	//std::cout <<"Populate!\n";

	for (std::list<ImportRule>::iterator it = ruleInclude.begin(); it != ruleInclude.end(); it++)  {
    		//std::cout <<"   " <<it->dir.c_str() <<'\t' <<it->glob.c_str() <<'\n';
    		FilePath realDir = path + it->dir.c_str();

		if (realDir.IsDirectory())
			FillFiles(it->dir.c_str(), it->glob, true, prefix);
	}

	std::sort(files.begin(), files.end());
	std::vector<FilePath>::iterator last = std::unique(files.begin(), files.end());
	files.resize(last - files.begin());
}
