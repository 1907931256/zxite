
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

	//std::cerr <<"GM: <" <<str <<"> <" <<glob <<"\n";

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


LabelPath::LabelPath(SString d, SString r, bool file_set): label(d), path(r) {

	if (file_set) {
		const GUI::gui_string &std_path = r.c_str();
		size_t sep_last = std_path.rfind(SEP);
		//std::cerr <<"---- bug " <<sep_last <<" " <<std_path.length() <<"\n";
		filename = path.substr(sep_last + 1, std_path.length());
	}

	//std::cerr <<"---- OH " <<d.length() <<" " <<r.length() <<" " <<(long long)d.c_str() <<" " <<(long long)r.c_str() <<"\n";
}


bool LabelPath::operator<(const LabelPath &other) const
{
	const unsigned char * s1 = (const unsigned char *) label.c_str();
	const unsigned char * s2 = (const unsigned char *) other.label.c_str();

	for (;;) {
		if (*s1 == '\0') {
			if (*s2 == '\0')
				return strcmp(filename.c_str(), other.filename.c_str()) < 0;
			else

				return *s2 != SEP;
		}

		if (*s2 == '\0')
			return *s1 == SEP;

		if (*s1 != *s2)
			return *s1 < *s2;

		s1++, s2++;
	}
}

bool LabelPath::operator==(const LabelPath &other) const
{
	#if 0
	if (path == "zite.zte" || other.path == "zite.zte")
	printf(" ~~~~~ <%s[%lu] == %s[%lu] ?>   <%s[%lu] == %s[%lu] ?>   -> %x / %x => %x ~~~~~ \n",
		label.c_str(), label.size(), other.label.c_str(), other.label.size(),
		path.c_str(), path.size(), other.path.c_str(), other.path.size(),
		label == other.label, path == other.path,
		(label == other.label) && (!path || !other.path || path == other.path));
	#endif

	return (label == other.label) && (!path || !other.path || path == other.path);
}



void Project::ProcessXxclude(const char * prop_type, std::list<LabelPath> &prop_list) {

	SString value;
	int i = 0;
	while ((value = propFile.Get(SString(prop_type).append(i++).c_str()))) {

		// proj.include.n = [ label | ] files [; files [; ...] ]
		// with 'files' = folder or regex or folder/regex

		int pos = value.search("|");
		SString label;
		if (pos != -1) {
			label = value.substr(0, pos);
			label.trim();
			if (label == "")
				label = "|";
		}

		while (++pos < (int)value.length()) {
			int npos = value.search(";", pos);
			if (npos == -1)
				npos = value.length();

			FilePath path(value.substr(pos, npos - pos).trim().c_str());
			SString glob = path.NormalizePath().AsInternal();
			if (glob.length() && glob_check(glob.c_str())) {
				LabelPath xxx = LabelPath(label.trim(), glob, false);
				prop_list.push_back(xxx);
			}
			pos = npos;
		}
	}

	prop_list.sort();
	prop_list.unique();

	#if 0
	std::cerr <<"===Rule: " <<prop_type <<"\n";
	for (std::list<LabelPath>::iterator it = prop_list.begin(); it != prop_list.end(); it++)
		std::cerr <<"   <" <<it->label.c_str() <<">\t<" <<it->path.c_str() <<">\n";
	#endif
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


	basePath = propFile.Get("proj.path").c_str();

	if (!basePath.IsAbsolute())
		basePath = file.Directory() + basePath.AsInternal();

	if (!basePath.IsDirectory()) {
		msg.assign(GUI_TEXT("Project path does not point to a valid directory"));
		return false;
	}

	// Process all include rules
	ProcessXxclude("proj.include.", ruleInclude);
	ProcessXxclude("proj.exclude.", ruleExclude);

	Populate();

	// dbg
	#if 0
	int i = 0;
	std::cerr <<"===Files:\n";
	for (std::vector<LabelPath>::iterator it = files.begin(); it != files.end(); it++)
    		std::cerr <<std::setw(3) <<i++ <<"   " <<it->label.c_str() <<"   " <<it->path.c_str() <<'\n';
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


void Project::FillFiles(SString label, FilePath dir, SString glob, bool recursive) {

	FilePathSet dirSet, fileSet;

	dir.List(dirSet, fileSet);

	if (recursive)
		for (size_t i = 0; i < dirSet.Length(); i ++)
			FillFiles(label, dirSet.At(i), glob, recursive);

	if (!label)
		label = dir.AsInternal() + basePath.Length() + 1;

	for (size_t i = 0; i < fileSet.Length(); i ++) {
		const char * entry = fileSet.At(i).AsInternal()  + basePath.Length() + 1;

		if (!glob_match(entry, glob.c_str()))
			continue;


	//	std::list<SString>::iterator ex;
	//	for (ex = ruleExclude.begin(); ex != ruleExclude.end(); ex++)
	//		if (glob_match(entry.AsInternal(), ex->c_str()))
	//			break;
	//
	//	std::cerr <<"   C: " <<entry.AsInternal() <<'\n';
	//
	//	if (ex != ruleExclude.end())
	//		continue;

		files.push_back(LabelPath(label, entry, true));
	}

}


void Project::Populate()
{

	//std::cerr <<"===Populate!\n";

	for (std::list<LabelPath>::iterator it = ruleInclude.begin(); it != ruleInclude.end(); it++)  {
		//std::cerr <<"   " <<it->label.c_str() <<'\t' <<it->path.c_str() <<'\n';

		const char * glob = it->path.c_str();
		int pos = 0;
		int last_sep = 0;
		bool recursive = false;

		for (;;) {
			char c = glob[pos];
			if (c == '\0') {
				last_sep = pos;
				glob = "**";
				recursive = true;
				break;
			} else if (c == SEP) {
				last_sep = pos;
			} else if (c == '*' || c == '?' || c == '[') {
				do {
					if (c == '*' && glob[pos + 1] == '*') {
						recursive = true;
						break;
					}
				} while (glob[++pos]);
				break;
			} else if (c == '\\') {
				pos++;
			}
			pos++;
		}

		SString path_str = it->path.substr(0, last_sep);
		FilePath path = basePath + path_str.c_str();


		if (path.IsDirectory())
			// fill files
			FillFiles(it->label, path, glob, recursive);

		else if (path.Exists())

			files.push_back(LabelPath(it->label, path_str, true));
	}

	//std::cerr <<"===Sort:\n";
	// Sort according to operator < defined for LabelPath
	std::sort(files.begin(), files.end());
	std::vector<LabelPath>::iterator last = std::unique(files.begin(), files.end());
	files.resize(last - files.begin());

	//for (std::vector<LabelPath>::iterator it = files.begin(); it != files.end(); it++)
	//	it->show();
}
