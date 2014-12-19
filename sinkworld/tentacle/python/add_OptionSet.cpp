#include <boost/python.hpp>
#include <boost/cstdint.hpp>

#include <string>
#include <iostream>
#include <assert.h>
#include "base.h"

// Using =======================================================================
using namespace boost::python;

#include "bbase.h"

void add_OptionSet() {
    class_< OptionSet, boost::noncopyable >("OptionSet", init<  >())
        .def("Insert", &OptionSet::Insert)
        .def("Delete", &OptionSet::Delete)
        .def("Value", &OptionSet::Value, return_internal_reference<>())
        .def("ModeValue", &OptionSet::ModeValue, return_internal_reference<>())
        .def("IntegerModeValue", &OptionSet::IntegerModeValue)
        .def("SetFromLine", &OptionSet::SetFromLine)
        .def("SetFromText", (void (OptionSet::*)(Literal *, char *, int, int))&OptionSet::SetFromText)
        .def("Allocate", &Dictionary::Allocate)
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
