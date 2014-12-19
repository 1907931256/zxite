#include <boost/python.hpp>
#include <boost/cstdint.hpp>

#include <string>
#include <iostream>
#include <assert.h>
#include "base.h"

// Using =======================================================================
using namespace boost::python;

#include "bbase.h"

void add_WordList() {
    class_< WordList, boost::noncopyable >("WordList", init< bool >())
        .def("Clear", &WordList::Clear)
        .def("Set", &WordList::Set)
        .def("SetText", &WordList::SetText)
        .def("Find", &WordList::Find)
        .def("InList", &WordList::InList)
        .def("Length", &WordList::Length)
        .def("GetText", &WordList::GetText, return_value_policy< manage_new_object >())
    ;

}
