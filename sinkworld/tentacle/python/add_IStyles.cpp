#include <boost/python.hpp>
#include <boost/cstdint.hpp>

#include <string>
#include <iostream>
#include <assert.h>
#include "base.h"

// Using =======================================================================
using namespace boost::python;

#include "bbase.h"

void add_IStyles() {
    class_< IStyles, boost::noncopyable, IStyles_Wrapper >("IStyles", no_init)
        .def("Length", pure_virtual(&IStyles::Length))
        .def("ValueAt", pure_virtual(&IStyles::ValueAt))
        .def("FindNextChange", pure_virtual(&IStyles::FindNextChange))
        .def("FillRange", pure_virtual(&IStyles::FillRange))
        .def("InsertSpace", pure_virtual(&IStyles::InsertSpace))
        .def("DeleteAll", pure_virtual(&IStyles::DeleteAll))
        .def("DeleteRange", pure_virtual(&IStyles::DeleteRange))
    ;

}
