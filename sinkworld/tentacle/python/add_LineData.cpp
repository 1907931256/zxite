#include <boost/python.hpp>
#include <boost/cstdint.hpp>

#include <string>
#include <iostream>
#include <assert.h>
#include "base.h"

// Using =======================================================================
using namespace boost::python;

#include "bbase.h"

void add_LineData() {
    class_< LineData, boost::noncopyable, LineData_Wrapper >("LineData", init<  >())
        .def_readwrite("next", &LineData::next)
        .def("Clear", pure_virtual(&LineData::Clear))
        .def("InsertLine", pure_virtual(&LineData::InsertLine))
        .def("RemoveLine", pure_virtual(&LineData::RemoveLine))
    ;

}
