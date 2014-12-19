// SinkWorld OptionSet class.
// Copyright 2005 Neil Hodgson
// File written in C~ subset of C++ for automatic translation to Java and C#.

/// Stores a dictionary indexed by Literal keys containing Literal values.
/// This is the in memory representation of an options file although it could
/// be populated from another source such as a registry or database.

SW_PUBLIC class OptionSet : public Dictionary {
private:
	Literal *litSuper;
	int separator;

	void DeleteAll();
	Literal *CombinedKeyValue(Literal *mode, Literal *key);
	Literal *LiteralUnquoted(int *line, int start, int end);

public:
	OptionSet();
	virtual ~OptionSet();
	/// Add an element with \a key and \a value.
	void Insert(Literal *key, Literal *value);
	/// Remove the \a key element.
	void Delete(Literal *key);
	/// Find the value associated with \a key
	Literal *Value(Literal *key);
	/// Find the value associated with \a key in mode \a mode or a superclass.
	Literal *ModeValue(Literal *mode, Literal *key);
	/// Find the integer value associated with \a key in mode \a mode 
	/// or a superclass, returning \a defaultValue if \a key is not present.
	int IntegerModeValue(Literal *mode, Literal *key, int defaultValue);
	/// Add a set of ',' separated elements from a line of text \a line.
	int SetFromLineDictionary(Literal *prefix, int *line, int eq, int len);
	/// Add an element from a line of text \a line.
	/// Main form is key=value.
	/// May also be a dictionary assignment like key:=subkey:x,subkey2:y.
	/// If \a line is a comment no element is added.
	bool SetFromLine(Literal *prefix, int *line, int pos, int len);
	/// Calls SetFromLine for each logical line in \a text.
	/// A logical line is a set of physical lines where all but the last end in a '\'.
	int SetFromText(Literal *prefix, SW_BYTE *text, int pos, int len);
};
