#include <boost/python.hpp>
#include <boost/cstdint.hpp>

#include <string>
#include <iostream>
#include <assert.h>
#include "base.h"

// Using =======================================================================
using namespace boost::python;

#include "bbase.h"

void add_History() {
    class_< History, boost::noncopyable >("History", init<  >())
        .def(init< int >())
        .def("Append", (void (History::*)(int, int, Literal*) )&History::Append)
        .def("Append", (void (History::*)(int, int) )&History::Append)
        .def("AppendIfChanged", &History::AppendIfChanged)
        .def("Append", (void (History::*)(int, Literal*) )&History::Append)
        .def("Push", (void (History::*)(int, int, Literal*) )&History::Push)
        .def("Push", (void (History::*)(int, int) )&History::Push)
        .def("Push", (void (History::*)(int, Literal*) )&History::Push)
        .def("Pop", &History::Pop)
        .def("InvalidateAfter", &History::InvalidateAfter)
        .def("Value", &History::Value)
        .def("TextValue", &History::TextValue, return_internal_reference<>())
        .def("Depth", &History::Depth)
    ;

}
