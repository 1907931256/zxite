// SinkWorld IDecoration interface.
// Copyright 2004 Neil Hodgson
// File written in C~ subset of C++ for automatic translation to Java and C#.

#ifdef SW_CXX
class PaintContext;
#endif

/// Interface for decorators to implement.
/// A decorator draws something under or over text such as an underline or box.

SW_PUBLIC class IDecoration : public IPrototype {
public:
	virtual ~IDecoration() {
	}
	//~ virtual Literal *Name()=0;
	//~ virtual IPrototype *CreateNew()=0;
	virtual IDecoration *Clone()=0;
	virtual void Paint(PaintContext *pc)=0;
	virtual void SetFore(RGBColor fore_)=0;
	virtual void FromOptions(Literal *mode, Literal *facetPath, OptionSet *options)=0;
};

