// SinkWorld PrototypeRegistry class.
// Copyright 2005 Neil Hodgson
// File written in C~ subset of C++ for automatic translation to Java and C#.

/// The PrototypeRegistry class holds a global set of known 
/// prototypes.

SW_PUBLIC class PrototypeRegistry {
	static Dictionary *prototypes;
public:
	static void Init();
	static void Finish();
	static IPrototype *FromName(Literal *role, Literal *name);
	static void AddPrototype(Literal *role, IPrototype *proto);
	static void AddPrototypeNamed(Literal *role, Literal *protoName, IPrototype *proto);
};
