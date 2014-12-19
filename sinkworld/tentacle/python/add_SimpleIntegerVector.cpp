#include <boost/python.hpp>
#include <boost/cstdint.hpp>

#include <string>
#include <iostream>
#include <assert.h>
#include "base.h"

// Using =======================================================================
using namespace boost::python;

#include "bbase.h"

void add_SimpleIntegerVector() {
    class_< SimpleIntegerVector, boost::noncopyable >("SimpleIntegerVector", init<  >())
        .def("SetGrowSize", &SimpleIntegerVector::SetGrowSize)
        .def("Length", &SimpleIntegerVector::Length)
        .def("SetValueAt", &SimpleIntegerVector::SetValueAt)
        .def("SetValueAllowingExpansion", &SimpleIntegerVector::SetValueAllowingExpansion)
        .def("ValueAt", &SimpleIntegerVector::ValueAt)
        .def("Insert", &SimpleIntegerVector::Insert)
        .def("Delete", &SimpleIntegerVector::Delete)
        .def("RangeAddDelta", &SimpleIntegerVector::RangeAddDelta)
    ;

}
