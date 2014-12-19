/// IPrototype interface is implemented by all classes that can be created
/// from prototype objects from a registry.
// Copyright 2005 Neil Hodgson
// File written in C~ subset of C++ for automatic translation to Java and C#.

SW_PUBLIC class IPrototype {
public:
	virtual ~IPrototype() {
	}
	virtual Literal *Name() = 0;
	virtual IPrototype *CreateNew() = 0;
};
