// SinkWorld BaseDecoration class.
// Copyright 2005 Neil Hodgson
// File written in C~ subset of C++ for automatic translation to Java and C#.

/// Handle naming of decorations so they can be registered.

SW_PUBLIC class BaseDecoration : public IDecoration {
public:
	Literal *name;

	BaseDecoration() {
		name = NULL;
	}
	
	~BaseDecoration() {
		delete name;
		name = NULL;
	}
	
	Literal *Name() {
		return name;
	}
	
	virtual IPrototype *CreateNew() {
		return NULL;
	}

	virtual IDecoration *Clone() {
		return NULL;
	}

	virtual void Paint(PaintContext *pc) {
	}
	
	virtual void SetFore(RGBColor fore_) {
	}

	virtual void FromOptions(Literal *mode, Literal *facetPath, OptionSet *options);
};
