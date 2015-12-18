
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
#include <assert.h>

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

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



// ================================================= GLOB FUNC

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
bool glob_match(const char *str, const char *glob, int nocase) {
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
					if (glob_match((char*)s, (char*)g, nocase))
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
			char gc = *g, sc = *s;
			if (nocase) {
				if ('a' <= gc && gc <= 'z')
					gc -= 'a' - 'A';
				if ('a' <= sc && sc <= 'z')
					sc -= 'a' - 'A';
			}
			if (gc != sc) {
				// ignore potential trailing "/**"
				if (sc =='\0' && gc == SEP && g[1] == '*' && g[2] == '*' && g[3] == '\0')
					return true;
				// usual case: normal chars are !=
				return false;

			} else if (gc == '\0') {
				// end of glob
				return true;

			}
			// here: normal chars are ==
		}
		s++;
		g++;
	}

}



// ================================================= TAGS FUNC

#define BUF_LEN 1024

struct buf_cache {
	int fd;
	off_t len; 	// file len
	off_t off;	// last off request by cache_get
	char *p;	// pointer to off
	char *p_beg;	// min valid data in buffer
	char *p_end;	// max valid data in buffer
	unsigned int magic0;
	char bl[BUF_LEN];
	char bh[BUF_LEN];
	unsigned int magic1;
};

struct buf_cache *cache_new(int fd, off_t len) {
	struct buf_cache *cache = (struct buf_cache *) malloc(sizeof(struct buf_cache));
	cache->p = NULL;
	cache->fd = fd;
	cache->len = len;
	cache->magic0 = 0xDEADBEEF;
	cache->magic1 = 0xDEADBEEF;
	return cache;
}


static char *cache_fetch_off(struct buf_cache *cache, off_t off)
{
	assert(cache);
	assert(0 <= off);
	assert(off < cache->len);

	if (cache->p) {
		off_t off_beg = cache->off - (cache->p - cache->p_beg);
		off_t off_end = cache->off + (cache->p_end - cache->p);

		assert(0 <= off_beg);
		assert(off_end <= cache->len);

		if (off_beg <= off && off < off_end) {
			cache->p += off - cache->off;
			cache->off = off;
			return cache->p;

		} else if (off_end <= off && off < off_end + BUF_LEN) {
			assert(cache->p_end == cache->bh + BUF_LEN);

			cache->p_beg = cache->bl;
			memcpy(cache->bl, cache->bh, BUF_LEN);

			lseek(cache->fd, off_end, SEEK_SET);
			cache->p = cache->bh + off - off_end;
			cache->p_end = cache->bh + read(cache->fd, cache->bh, BUF_LEN);
			cache->off = off;
			return cache->p;

		} else if (off_beg - BUF_LEN <= off && off < off_beg) {

			assert(cache->p_beg == cache->bl || cache->p_beg == cache->bh);

			if (cache->p_beg == cache->bl) {
				memcpy(cache->bh, cache->bl, BUF_LEN);
				cache->p_end = cache->bh + BUF_LEN;
			}

			if (off_beg < BUF_LEN) {
				lseek(cache->fd, 0, SEEK_SET);
				cache->p_beg = cache->bl + BUF_LEN - off_beg;
				int len = read(cache->fd, cache->p_beg, off_beg);
				(void) len;
				assert(len == off_beg);
			} else {
				cache->p_beg = cache->bl;

				lseek(cache->fd, off_beg - BUF_LEN, SEEK_SET);
				int len = read(cache->fd, cache->p_beg, BUF_LEN);
				(void) len;
				assert(len == BUF_LEN);
			}

			cache->p = cache->bh - (off_beg - off);
			cache->off = off;
			return cache->p;
		}
	}

	lseek(cache->fd, off, SEEK_SET);
	cache->p_beg = cache->p = cache->bh;
	cache->p_end = cache->p + read(cache->fd, cache->p, BUF_LEN);

	cache->off = off;
	return cache->p;
}

static off_t cache_off(struct buf_cache *cache, char *p)
{
	return cache->off + p - cache->p;
}

static char *cache_fetch_p(struct buf_cache *cache, char *p)
{
	assert(cache->p);
	off_t off = cache_off(cache, p);
	if (off < 0 || cache->len <= off)
		return NULL;
	return cache_fetch_off(cache, off);
}



static char *tag_prev_eol(struct buf_cache *cache, off_t off)
{
	char *p = cache_fetch_off(cache, off);

	do {
		p--;
		if (p < cache->p_beg)
			p = cache_fetch_p(cache, p);
	} while (p && *p != '\n');
	return p;
}

static char *tag_next_eol(struct buf_cache *cache, off_t off)
{
	char *p = cache_fetch_off(cache, off);
	while (p && *p != '\n') {
		p++;
		if (cache->p_end <= p)
			p = cache_fetch_p(cache, p);
	}
	return p;
}

static char **tag_find(const char *tagfile, const char *str)
{
	char **res_dyn = NULL;
	int fd = open(tagfile, O_RDONLY);
	if (fd < 0) {
		printf("ERR open %s\n", tagfile);
		return NULL;
	}

	off_t beg = 0;
	off_t end = lseek(fd, 0, SEEK_END);
	if (end == (off_t) -1) {
		printf("ERR lseek\n");
		return NULL;
	}

	off_t mid = (beg + end) / 2;
	off_t lllast_sol = -1;
	off_t llast_sol = -1;
	off_t last_sol = -1;

	struct buf_cache *cache = cache_new(fd, end);

	char *pb = tag_next_eol(cache, mid);
	if (pb == NULL) {
		pb = tag_prev_eol(cache, mid);
		if (pb == NULL)
			goto bs_exit;
	}

	for (;;) {
		pb = cache_fetch_p(cache, pb + 1);
		mid = cache_off(cache, pb);

		if (mid == llast_sol && lllast_sol == last_sol)
			goto bs_exit;
		lllast_sol = llast_sol;
		llast_sol = last_sol;
		last_sol = mid;

		const char *ps = str;
		for (;;) {
			if (pb >= cache->p_end)
				pb = cache_fetch_p(cache, pb);
			if (pb == NULL)
				goto bs_exit;

			if (*ps == '\0' && *pb == '\t')
				// found
				goto found;
			int diff = *pb++ - *ps++;
			if (diff > 0) {
				//before
				end = mid;
				pb = tag_prev_eol(cache, (beg + end) / 2);
				if (pb == NULL)
					pb = cache_fetch_off(cache, 0) - 1;
				break;
			}
			if (diff < 0) {
				//after
				beg = mid;
				pb = tag_next_eol(cache, (beg + end) / 2);
				if (pb == NULL)
					goto bs_exit;
				break;
			}
		}
	}

found:
	{
		int str_len = strlen(str);

		for (;;) {
			if (mid == 0)
				break;
			pb = tag_prev_eol(cache, mid - 1);
			if (pb++ == NULL)
				pb = cache_fetch_off(cache, 0);

			if (strncmp(str, pb, str_len) || pb[str_len] != '\t')
				break;
			mid = cache_off(cache, pb);
		}

		#define MAX_CNT 16
		int idx = 0;
		char *res[MAX_CNT + 1];

		char *npb = tag_next_eol(cache, mid);
		assert(npb);

		while (idx < MAX_CNT) {
			pb = cache_fetch_off(cache, mid);
			res[idx++] = strndup(pb, npb - pb);

			mid = cache_off(cache, npb + 1);
			if (cache->len <= mid)
				break;

			npb = tag_next_eol(cache, mid);
			if (npb == NULL)
				break;
			pb = cache_fetch_off(cache, mid);
			if (strncmp(str, pb, str_len) || pb[str_len] != '\t')
				break;
		}

		res[idx++] = NULL;

		res_dyn = (char **) malloc(sizeof(char *) * idx);
		memcpy(res_dyn, res, sizeof(char *) * idx);
	}

bs_exit:
	assert(cache->magic0 == 0xDEADBEEF);
	assert(cache->magic1 == 0xDEADBEEF);
	free(cache);
	close(fd);

	return res_dyn;
}




// ================================================= TAGS FUNC


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

	projFile = file;

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

		if (!glob_match(entry, glob.c_str(), false))
			continue;


	//	std::list<SString>::iterator ex;
	//	for (ex = ruleExclude.begin(); ex != ruleExclude.end(); ex++)
	//		if (glob_match(entry.AsInternal(), ex->c_str(), false))
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


int Project::BuildTags(SString &cmd, SString &dir)
{
	if (!opened)
		return 1;

	SString files_file = propFile.Get("proj.tags.files_file");
	if (!files_file.length()) {
		FilePath tmp = projFile.Directory() + "files.tag";
		propFile.Set("proj.tags.files_file", tmp.AsInternal());
	}

	SString symbols_file = propFile.Get("proj.tags.symbols_file");
	if (!symbols_file.length()) {
		FilePath tmp = projFile.Directory() + "symbols.tag";
		propFile.Set("proj.tags.symbols_file", tmp.AsInternal());
	}

	SString tag_cmd = propFile.Get("proj.tags.cmd");
	if (!tag_cmd.length()) {
		propFile.Set("proj.tags.cmd", "ctags --c-kinds=+p -R --fields=+n -L $(proj.tags.files_file) -f $(proj.tags.symbols_file)");
	}

	if (propFile.GetInt("proj.tags.files_auto", 1)) {
		FilePath files_fp(propFile.Get("proj.tags.files_file").c_str());
		FILE * fd = files_fp.Open(fileWrite);
		if (fd) {
			for (std::vector<LabelPath>::iterator it = files.begin(); it != files.end(); it++) {
				::fwrite(it->path.c_str(), it->path.length(), 1, fd);
				::fwrite("\n", 1, 1, fd);
			}
			::fclose(fd);
		}
	}

	std::cout <<"TAGS: " <<propFile.GetExpanded("proj.tags.cmd").c_str() <<'\n';

	cmd = propFile.GetExpanded("proj.tags.cmd");
	dir = projFile.Directory().AsInternal();
	return 0;
}

std::vector<CTag> Project::FindTag(SString tag, int cmdID)
{
	std::vector<CTag> vector;
	SString symbols_file = propFile.Get("proj.tags.symbols_file");
	std::cout <<"open: " << symbols_file.c_str() <<"\n";
	char ** found = tag_find(symbols_file.c_str(), tag.c_str());

	while (found && *found) {
		//std::cout <<tag.c_str() <<": " << *found <<"\n";

		char *s = *found;
		char *filename = NULL;
		long int line = 0;
		char *pattern = NULL;
		char kind = '\0';
		std::list<SString> ext;

		if ((s = strchr(s, '\t')) == NULL)
			goto err;
		filename = ++s;
		if ((s = strchr(s, '\t')) == NULL)
			goto err;
		s[0] = '\0';
		if (s[1] != '/' || s[2] != '^') {
			char *end;
			line = strtol(s + 1, &end, 10);
			if (line < 1)
				goto err;
			s = end;
		} else {
			s += 3;
			pattern = ++s;
			while (s[0] != '$' || s[1] != '/')
				if (!*s++)
					goto err;
			s[0] = '\0';
			s += 2;
		}
		if (*s) {
			if (s[0] != ';' || s[1] != '"')
				goto err;
			s += 2;

			do {
				if (*s++ != '\t')
					goto err;
				char *ext_val = s;
				char *ext_name = s;
				while (*s && *s != '\t')
					if (*s++ == ':')
						ext_val = s;
				if (ext_val == ext_name || !strncmp(ext_name, "kind", ext_val - 1 - ext_name)) {
					if (s - ext_val != 1)
						goto err;
					kind = *ext_val;
				} else if (!strncmp(ext_name, "line", ext_val - 1 - ext_name)) {
					line = strtol(ext_val, &ext_val, 10);
					if (line < 1 || ext_val != s)
						goto err;
				} else if (ext_val < s) {
					//ext.push_back(SString(ext_name, 0, ext_val -1 - ext_name));
					//ext.push_back(SString(ext_val, 0, s - ext_val));
					ext.push_back(SString(ext_name, 0, s - ext_name));
				}
			} while (*s);
		}

		if ((cmdID == IDM_FINDTAG_ALL)
			|| (cmdID == IDM_FINDTAG_DECL && kind != 'p' && kind != 'x')
			|| (cmdID == IDM_FINDTAG_DEF && (kind == 'p' || kind == 'x'))
		) {
			SString kind_str = "pouet";
			switch (kind) {
			case 'c': kind_str = "class"; break;
			case 'd': kind_str = "macro"; break;
			case 'e': kind_str = "enumerator"; break;
			case 'f': kind_str = "function"; break;
			case 'g': kind_str = "enumeration"; break;
			case 'l': kind_str = "local"; break;
			case 'm': kind_str = "member"; break;
			case 'n': kind_str = "namespace"; break;
			case 'p': kind_str = "prototype"; break;
			case 's': kind_str = "structure"; break;
			case 't': kind_str = "typedef"; break;
			case 'u': kind_str = "union"; break;
			case 'v': kind_str = "variable"; break;
			case 'x': kind_str = "external"; break;
			default: kind_str = "unknown";
			}

			//printf("<%s><%ld><%s>\n", filename, line, pattern);
			vector.push_back(CTag(tag, filename, pattern, line, kind_str, ext));
		}
		if (0) {
		err:
			std::cerr <<"Unable to parse <" <<*found <<">\n";

		}
		free(*found);
		found++;
	}

	return vector;

}

