// SinkWorld Dictionary class.
// Copyright 2005 Neil Hodgson
// File written in C~ subset of C++ for automatic translation to Java and C#.

/// One element in a Dictionary contains a key and a value.

class DictionaryElement {
public:
	int hashCode;
	Literal *key;
	SW_OBJECT value;
	DictionaryElement *next;
	DictionaryElement(int hashCode_, Literal *key_, SW_OBJECT value_) {
		hashCode = hashCode_;
		key = new Literal(key_);
		value = value_;
		next = NULL;
	}
	~DictionaryElement() {
		hashCode = 0;
		delete key;
		key = NULL;
		assert(value==NULL);
		next = NULL;
	}
	bool Match(int hashKey, Literal *key_) {
		return (hashKey == hashCode) && (key->Equals(key_));
	}
};

/// A Dictionary is indexed by Literal keys and contains object values.
/// As the values are arbitrary objects and do not derive from a known base
/// class, the Dictionary does not know how to delete its values so they must
/// be deleted by calling code. The DeleteOne method can be called in a loop
/// to delete all entries.

SW_PUBLIC class Dictionary {
private:
	DictionaryElement **elements;	///< Storage for directly accessible elements.
	int size;	///< Allocated size of elements array.
	int used;	///< Number of elements.
	int deletePosition;	///< Position of last element deleted by DeleteOne. Makes deleting all faster.

	/// Insert an existing element
	void InsertElement(DictionaryElement *element);
	/// Change size to \a sizeNew elements.
	void Resize(int sizeNew);
	/// Find the element associated with \a key
	DictionaryElement *FindElement(Literal *key);

public:
	Dictionary();
	virtual ~Dictionary();
	/// Change size to \a sizeNew elements.
	/// \warning May only be called when there are no elements.
	void Allocate(int sizeNew);

	/// Add an element with \a key and \a value.
	/// \return Previous value for this \a key.
	SW_OBJECT Add(Literal *key, SW_OBJECT value);
	/// Remove the \a key element.
	/// \return Previous value for this \a key.
	SW_OBJECT Remove(Literal *key);
	/// Remove an arbitrary element.
	/// \return Previous value of deleted element to allow caller to delete it.
	SW_OBJECT DeleteOne();
	/// Find the value associated with \a key
	SW_OBJECT Find(Literal *key);
	/// Find the stored key object equal to \a key
	Literal *FindKey(Literal *key);
	/// Return a list of all key values which end with \a suffix to allow enumeration.
	WordList *Keys(Literal *suffix);

	// Statistics interface for checking how it is working
	int Allocated() {
		return size;
	}
	int Used() {
		return used;
	}
	int SlotsUsed() {
		int n = 0;
		for (int i=0;i<size;i++) {
			if (elements[i] != NULL)
				n++;
		}
		return n;
	}
};
