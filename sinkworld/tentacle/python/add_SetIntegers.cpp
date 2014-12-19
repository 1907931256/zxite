#include <boost/python.hpp>
#include <boost/cstdint.hpp>

#include <string>
#include <iostream>
#include <assert.h>
#include "base.h"

// Using =======================================================================
using namespace boost::python;

#include "bbase.h"

void add_SetIntegers() {
    class_< SetIntegers, boost::noncopyable >("SetIntegers", init< int >())
        .def("Add", &SetIntegers::Add)
        .def("Length", &SetIntegers::Length)
        .def("At", &SetIntegers::At)
    ;

}
