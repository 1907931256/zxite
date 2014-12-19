

bool glob_check(const char *glob);
bool glob_match(const char *str, const char *glob);



struct ImportRule {
	SString dir;
	SString glob;

	ImportRule(SString d, SString r): dir(d), glob(r) {}

	bool operator<(const ImportRule &other) const;
	bool operator==(const ImportRule &other) const;
};


class Project {

	PropSetFile propFile;

	SString name;

	std::list<ImportRule> ruleInclude;
	std::list<SString> ruleExclude;


	void FillFiles(FilePath dir, SString glob, bool recursive, int prefix);
public:
	bool opened;
	GUI::gui_string msg;

	FilePath path;
	std::vector<FilePath> files;

	Project() : opened(false) {}
	~Project() {}

	bool open(FilePath _propFile);
	bool close();


	void Populate();


};
