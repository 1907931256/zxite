// SinkWorld PrototypeRegistry class.
// Copyright 2005 Neil Hodgson
// File written in C~ subset of C++ for automatic translation to Java and C#.

#include <string.h>
#include <assert.h>
#include <string>

#include "base.h"

Dictionary *PrototypeRegistry::prototypes = NULL;

void PrototypeRegistry::Init() {
	prototypes = new Dictionary();
}

void PrototypeRegistry::Finish() {
	SW_OBJECT o;
	while ((o=prototypes->DeleteOne()) != NULL) {
		delete (IPrototype *)(o);
	}
	delete prototypes;
	prototypes = NULL;
}

IPrototype *PrototypeRegistry::FromName(Literal *role, Literal *name) {
	Literal *roleName = new Literal(role, (int)'.', name);
	IPrototype *proto = (IPrototype *)(prototypes->Find(roleName));
	delete roleName;
	if (proto == NULL) {
		return NULL;
	} else {
		return proto->CreateNew();
	}
}

void PrototypeRegistry::AddPrototype(Literal *role, IPrototype *proto) {
	Literal *roleName = new Literal(role, (int)'.', proto->Name());
	prototypes->Add(roleName, proto);
	delete roleName;
}

void PrototypeRegistry::AddPrototypeNamed(Literal *role, Literal *protoName, IPrototype *proto) {
	Literal *roleName = new Literal(role, (int)'.', protoName);
	prototypes->Add(roleName, proto);
	delete roleName;
}

