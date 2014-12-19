#include <boost/python.hpp>
#include <boost/cstdint.hpp>

#include <string>
#include <iostream>
#include <assert.h>
#include "base.h"

// Using =======================================================================
using namespace boost::python;

#include "bbase.h"

int LexBase_Wrapper::deletions;

static void name_from_literal(LexBase &self, Literal *lit) {
	delete self.name;
	self.name = new Literal(lit);
}

static void statenames_from_literal(LexBase &self, Literal *lit) {
	delete self.stateNames;
	self.stateNames = new Literal(lit);
}

void add_LexBase() {
	//~ class_< LexBase_Wrapper, std::auto_ptr<LexBase_Wrapper>, bases< ILexer >, boost::noncopyable  >("LexBase", init< >())
	class_< LexBase_Wrapper, bases< ILexer >, boost::noncopyable  >("LexBase", init< >())
	.def_readwrite("baseAllocation", &LexBase::baseAllocation)
	.def_readwrite("baseState", &LexBase::baseState)
	.def_readwrite("subStates", &LexBase::subStates)
	.def_readwrite("thisStates", &LexBase::thisStates)
	.add_property("name",
	              make_getter(&LexBase::name, return_internal_reference<>()),
	              name_from_literal)
	.add_property("stateNames",
	              make_getter(&LexBase::stateNames, return_internal_reference<>()),
	              statenames_from_literal)
	.def("Name",
	     &LexBase::Name,
	     &LexBase_Wrapper::default_Name,
	     return_internal_reference<>())
	.def("CreateNew",
	     &LexBase::CreateNew,
	     &LexBase_Wrapper::default_CreateNew,
	     return_value_policy< manage_new_object >())

	.def("InvalidateAfter", &LexBase::InvalidateAfter, &LexBase_Wrapper::default_InvalidateAfter)
	.def("ColourisePiece", &LexBase::ColourisePiece, &LexBase_Wrapper::default_ColourisePiece)
	.def("Colourise", &LexBase::Colourise, &LexBase_Wrapper::default_Colourise)
	.def("States", &LexBase::States, &LexBase_Wrapper::default_States)
	.def("SetBaseState", &LexBase::SetBaseState, &LexBase_Wrapper::default_SetBaseState)
	.def("GetBaseState", &LexBase::GetBaseState, &LexBase_Wrapper::default_GetBaseState)
	.def("InStateRange", &LexBase::InStateRange)
	.def("StateFromName", &LexBase::StateFromName, &LexBase_Wrapper::default_StateFromName)
	.def("CanBeInterrupted", &LexBase::CanBeInterrupted, &LexBase_Wrapper::default_CanBeInterrupted)
	.def("SetKeyWords", &LexBase::SetKeyWords, &LexBase_Wrapper::default_SetKeyWords)
	//.def("SetSubLexer", &LexBase::SetSubLexer, &LexBase_Wrapper::default_SetSubLexer)
	//.def("GetSubLexer", &LexBase::GetSubLexer)
	.def("SetInputStyles", &LexBase::SetInputStyles, &LexBase_Wrapper::default_SetInputStyles)
	.def("AllocateStates", &LexBase::AllocateStates)
	.def("StateNames", &LexBase::StateNames, &LexBase_Wrapper::default_StateNames, return_internal_reference<>())
	.def("StateName", &LexBase::StateName, return_value_policy< manage_new_object >())
	.def("LexerFromState", &LexBase::LexerFromState, return_internal_reference<>())
	;

}
