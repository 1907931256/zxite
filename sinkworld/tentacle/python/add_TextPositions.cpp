#include <boost/python.hpp>
#include <boost/cstdint.hpp>

#include <string>
#include <iostream>
#include <assert.h>
#include "base.h"

// Using =======================================================================
using namespace boost::python;

#include "bbase.h"

void add_TextPositions() {
    class_< TextPositions >("TextPositions", init<  >())
        .def(init< int >())
        .def(init< const TextPositions& >())
        .def("Add", &TextPositions::Add)
        .def("Append", &TextPositions::Append)
        .def("FillTo", &TextPositions::FillTo)
        .def("At", &TextPositions::At)
        .def("AtMiddle", &TextPositions::AtMiddle)
        .def("Last", &TextPositions::Last)
        .def("LastPosition", &TextPositions::LastPosition)
    ;

}
