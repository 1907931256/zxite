#include <boost/python.hpp>
#include <boost/cstdint.hpp>

#include <string>
#include <iostream>
#include <assert.h>
#include "base.h"

// Using =======================================================================
using namespace boost::python;

#include "bbase.h"

void add_LexScript() {
    class_< LexScript, bases< LexBase > , boost::noncopyable, LexScript_Wrapper >("LexScript", init<  >())
        //~ .def_readwrite("statesAtStart", &LexScript::statesAtStart)
        //~ .def_readwrite("slScriptsRoot", &LexScript::slScriptsRoot)
        //~ .def("InvalidateAfter", (void (LexScript::*)(int) )&LexScript::InvalidateAfter, (void (LexScript_Wrapper::*)(int))&LexScript_Wrapper::default_InvalidateAfter)
        //~ .def("AllocateStates", (void (LexScript::*)(int) )&LexScript::AllocateStates, (void (LexScript_Wrapper::*)(int))&LexScript_Wrapper::default_AllocateStates)
        //~ .def("SetSubLexer", (void (LexScript::*)(ILexer*, Literal*, Literal*) )&LexScript::SetSubLexer, (void (LexScript_Wrapper::*)(ILexer*, Literal*, Literal*))&LexScript_Wrapper::default_SetSubLexer)
        //~ .def("GetSubLexer", (ILexer* (LexScript::*)(Literal*) )&LexScript::GetSubLexer, (ILexer* (LexScript_Wrapper::*)(Literal*))&LexScript_Wrapper::default_GetSubLexer, return_internal_reference<>())
        //~ .def("StateName", (Literal* (LexScript::*)(int) )&LexScript::StateName, (Literal* (LexScript_Wrapper::*)(int))&LexScript_Wrapper::default_StateName, return_value_policy< manage_new_object >())
        //~ .def("LexerFromState", (ILexer* (LexScript::*)(int) )&LexScript::LexerFromState, (ILexer* (LexScript_Wrapper::*)(int))&LexScript_Wrapper::default_LexerFromState, return_internal_reference<>())
        //~ .def("CreateNew", (ILexer* (LexBase::*)() )&LexBase::CreateNew, (ILexer* (LexScript_Wrapper::*)())&LexScript_Wrapper::default_CreateNew)
        //~ .def("ColourisePiece", (void (LexBase::*)(Styler*) )&LexBase::ColourisePiece, (void (LexScript_Wrapper::*)(Styler*))&LexScript_Wrapper::default_ColourisePiece)
        //~ .def("Colourise", (void (LexBase::*)(Styler*) )&LexBase::Colourise, (void (LexScript_Wrapper::*)(Styler*))&LexScript_Wrapper::default_Colourise)
        //~ .def("States", (int (LexBase::*)() )&LexBase::States, (int (LexScript_Wrapper::*)())&LexScript_Wrapper::default_States)
        //~ .def("SetBaseState", (void (LexBase::*)(int) )&LexBase::SetBaseState, (void (LexScript_Wrapper::*)(int))&LexScript_Wrapper::default_SetBaseState)
        //~ .def("GetBaseState", (int (LexBase::*)() )&LexBase::GetBaseState, (int (LexScript_Wrapper::*)())&LexScript_Wrapper::default_GetBaseState)
        //~ .def("InStateRange", (bool (LexBase::*)(int) )&LexBase::InStateRange, (bool (LexScript_Wrapper::*)(int))&LexScript_Wrapper::default_InStateRange)
        //~ .def("StateFromName", (int (LexBase::*)(unsigned char*, int) )&LexBase::StateFromName, (int (LexScript_Wrapper::*)(unsigned char*, int))&LexScript_Wrapper::default_StateFromName)
        //~ .def("CanBeInterrupted", (bool (LexBase::*)(int) )&LexBase::CanBeInterrupted, (bool (LexScript_Wrapper::*)(int))&LexScript_Wrapper::default_CanBeInterrupted)
        //~ .def("SetKeyWords", (void (LexBase::*)(int, Literal*) )&LexBase::SetKeyWords, (void (LexScript_Wrapper::*)(int, Literal*))&LexScript_Wrapper::default_SetKeyWords)
        //~ .def("SetInputStyles", (void (LexBase::*)(IStyles*) )&LexBase::SetInputStyles, (void (LexScript_Wrapper::*)(IStyles*))&LexScript_Wrapper::default_SetInputStyles)
        //~ .def("Name", (Literal* (LexBase::*)() )&LexBase::Name, (Literal* (LexScript_Wrapper::*)())&LexScript_Wrapper::default_Name)
        //~ .def("StateNames", (Literal* (LexBase::*)() )&LexBase::StateNames, (Literal* (LexScript_Wrapper::*)())&LexScript_Wrapper::default_StateNames)
        //~ .def("FindSubLexer", &LexScript::FindSubLexer)
        //~ .def("SwitchToSubLexer", &LexScript::SwitchToSubLexer)
        //~ .def("ColourisePieceSubLexer", &LexScript::ColourisePieceSubLexer)
    ;

}
