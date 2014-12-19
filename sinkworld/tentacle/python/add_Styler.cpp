#include <boost/python.hpp>
#include <boost/cstdint.hpp>

#include <string>
#include <iostream>
#include <assert.h>
#include "base.h"

// Using =======================================================================
using namespace boost::python;

#include "bbase.h"

void add_Styler() {
    class_< Styler >("Styler", init< SplitText*, IStyles*, int, int, bool >())
        .def_readwrite("startSeg", &Styler::startSeg)
        .def_readwrite("atLineStart", &Styler::atLineStart)
        .def_readwrite("atLineEnd", &Styler::atLineEnd)
        .def_readwrite("folding", &Styler::folding)
        .def_readwrite("lineCurrent", &Styler::lineCurrent)
        .def_readwrite("levelCurrent", &Styler::levelCurrent)
        .def_readwrite("levelNext", &Styler::levelNext)
        .def_readwrite("state", &Styler::state)
        .def_readwrite("stateAtEnd", &Styler::stateAtEnd)
        .def_readwrite("changeBeyondEnd", &Styler::changeBeyondEnd)
        .def_readwrite("chPrev", &Styler::chPrev)
        .def_readwrite("ch", &Styler::ch)
        .def_readwrite("chNext", &Styler::chNext)
        .def("Complete", &Styler::Complete)
        .def("More", &Styler::More)
        .def("Forward", &Styler::Forward)
        .def("ChangeState", &Styler::ChangeState)
        .def("SetState", &Styler::SetState)
        .def("ForwardSetState", &Styler::ForwardSetState)
        .def("ChangeFoldLevel", &Styler::ChangeFoldLevel)
        .def("Match", (bool (Styler::*)(char) )&Styler::Match)
        .def("Match", (bool (Styler::*)(char, char) )&Styler::Match)
        .def("Match", (bool (Styler::*)(Literal*) )&Styler::Match)
        .def("MatchIgnoreCase", &Styler::MatchIgnoreCase)
        .def("GetRelative", &Styler::GetRelative)
        .def("GetCurrent", &Styler::GetCurrent, return_value_policy< manage_new_object >())
        .def("GetCurrentLowered", &Styler::GetCurrentLowered, return_value_policy< manage_new_object >())
        .def("IsDigit", &Styler::IsDigit)
        .def("IsAlphaNumeric", &Styler::IsAlphaNumeric)
        .def("IsSpaceChar", &Styler::IsSpaceChar)
        .def("MakeLowerCase", &Styler::MakeLowerCase)
        .def("Position", &Styler::Position)
        .def("SetWord", &Styler::SetWord)
        .def("WordInList", &Styler::WordInList)
        .def("WordIs", &Styler::WordIs)
        .staticmethod("IsDigit")
        .staticmethod("IsAlphaNumeric")
        .staticmethod("MakeLowerCase")
        .staticmethod("IsSpaceChar")
    ;

}
