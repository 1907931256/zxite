

#include<iostream>


bool glob_check(const char *glob);
bool glob_match(const char *str, const char *glob, int nocase);



struct LabelPath {
	SString label;
	SString path;
	SString filename;

	LabelPath(): label(), path() { }
	LabelPath(SString d, SString r, bool file_set);

	void show() {
		std::cerr <<label.c_str() <<" " <<path.c_str() <<"\n";
	}

	bool operator<(const LabelPath &other) const;
	bool operator==(const LabelPath &other) const;
};


struct CTag {
	SString name;
	SString file;
	SString pattern;
	int line;
	SString kind;
	std::list<SString> ext;

	CTag(SString _name, SString _file, SString _pattern, int _line, SString _kind, std::list<SString> _ext) :
		name(_name), file(_file), pattern(_pattern), line(_line), kind(_kind), ext(_ext) {}
};

class Project {

	FilePath projFile;
	PropSetFile propFile;

	SString name;

	std::list<LabelPath> ruleInclude;
	std::list<LabelPath> ruleExclude;

	void ProcessXxclude(const char * prop_type, std::list<LabelPath> &prop_list);
	void FillFiles(SString label, FilePath dir, SString glob, bool recursive, int offset);
public:
	bool opened;
	GUI::gui_string msg;

	FilePath basePath;
	std::vector<LabelPath> files;

	Project() : opened(false) {}
	~Project() {}

	bool Open(FilePath _propFile);
	bool Close();

	FilePath GetSessionFile();

	void Populate();
	int BuildTags(SString &cmd, SString &dir);
	std::vector<CTag> FindTag(SString tag, int cmdID);
};
