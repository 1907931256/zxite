#include <boost/python.hpp>
#include <boost/cstdint.hpp>

#include <string>
#include <iostream>
#include <assert.h>
#include "base.h"

// Using =======================================================================
using namespace boost::python;

#include "bbase.h"

void add_KeyWords() {
    class_< KeyWords, boost::noncopyable >("KeyWords", init< bool >())
        .def("Clear", &KeyWords::Clear)
        .def("SetText", &KeyWords::SetText)
        .def("Find", &KeyWords::Find)
        .def("Sets", &KeyWords::Sets)
    ;

}
