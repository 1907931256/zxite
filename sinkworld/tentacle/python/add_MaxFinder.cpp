#include <boost/python.hpp>
#include <boost/cstdint.hpp>

#include <string>
#include <iostream>
#include <assert.h>
#include "base.h"

// Using =======================================================================
using namespace boost::python;

#include "bbase.h"

void add_MaxFinder() {
    class_< MaxFinder, boost::noncopyable >("MaxFinder", init<  >())
        .def("Allocate", &MaxFinder::Allocate)
        .def("SetMinimum", &MaxFinder::SetMinimum)
        .def("GetMinimum", &MaxFinder::GetMinimum)
        .def("Add", &MaxFinder::Add)
        .def("Remove", &MaxFinder::Remove)
        .def("Replace", &MaxFinder::Replace)
        .def("Length", &MaxFinder::Length)
        .def("Maximum", &MaxFinder::Maximum)
    ;

}
