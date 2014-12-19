#include <boost/python.hpp>
#include <boost/cstdint.hpp>

#include <string>
#include <iostream>
#include <assert.h>
#include "base.h"

// Using =======================================================================
using namespace boost::python;

#include "bbase.h"

void add_LiteralSet() {
    class_< LiteralSet, boost::noncopyable >("LiteralSet", init<  >())
        .def("FindOrAddKey", &LiteralSet::FindOrAddKey, return_internal_reference<>())
        //~ .def("Allocate", &Dictionary::Allocate)
        //~ .def("Add", &Dictionary::Add)
        //~ .def("Remove", &Dictionary::Remove)
        //~ .def("DeleteOne", &Dictionary::DeleteOne)
        //~ .def("Find", &Dictionary::Find)
        .def("FindKey", &Dictionary::FindKey, return_internal_reference<>())
        .def("Keys", &Dictionary::Keys, return_value_policy< manage_new_object >())
        .def("Allocated", &Dictionary::Allocated)
        .def("Used", &Dictionary::Used)
        .def("SlotsUsed", &Dictionary::SlotsUsed)
    ;

}
