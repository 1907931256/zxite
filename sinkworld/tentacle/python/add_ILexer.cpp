#include <boost/python.hpp>
#include <boost/cstdint.hpp>

#include <string>
#include <iostream>
#include <assert.h>
#include "base.h"

// Using =======================================================================
using namespace boost::python;

#include "bbase.h"

void add_ILexer() {
    class_< ILexer, bases< IPrototype > , boost::noncopyable, ILexer_Wrapper >("ILexer", no_init)
        .def("Name", pure_virtual(&ILexer::Name), return_internal_reference<>())
        .def("CreateNew", pure_virtual(&ILexer::CreateNew), return_value_policy< manage_new_object >())
        .def("InvalidateAfter", pure_virtual(&ILexer::InvalidateAfter))
        .def("ColourisePiece", pure_virtual(&ILexer::ColourisePiece))
        .def("Colourise", pure_virtual(&ILexer::Colourise))
        .def("States", pure_virtual(&ILexer::States))
        .def("SetBaseState", pure_virtual(&ILexer::SetBaseState))
        .def("GetBaseState", pure_virtual(&ILexer::GetBaseState))
        .def("InStateRange", pure_virtual(&ILexer::InStateRange))
        .def("StateFromName", pure_virtual(&ILexer::StateFromName))
        .def("CanBeInterrupted", pure_virtual(&ILexer::CanBeInterrupted))
        .def("SetKeyWords", pure_virtual(&ILexer::SetKeyWords))
        .def("SetSubLexer", pure_virtual(&ILexer::SetSubLexer))
        .def("GetSubLexer", pure_virtual(&ILexer::GetSubLexer), return_internal_reference<>())
        .def("SetInputStyles", pure_virtual(&ILexer::SetInputStyles))
        .def("StateNames", pure_virtual(&ILexer::StateNames), return_internal_reference<>())
        .def("StateName", pure_virtual(&ILexer::StateName), return_value_policy< manage_new_object >())
        .def("LexerFromState", pure_virtual(&ILexer::LexerFromState), return_internal_reference<>())
    ;

}
