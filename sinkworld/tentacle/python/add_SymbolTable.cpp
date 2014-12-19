#include <boost/python.hpp>
#include <boost/cstdint.hpp>

#include <string>
#include <iostream>
#include <assert.h>
#include "base.h"

// Using =======================================================================
using namespace boost::python;

#include "bbase.h"

void add_SymbolTable() {
    class_< SymbolTable, boost::noncopyable >("SymbolTable", init<  >())
        .def("Append", (void (SymbolTable::*)(int, Literal*, Literal*) )&SymbolTable::Append)
        .def("InvalidateAfter", &SymbolTable::InvalidateAfter)
        .def("GetValue", &SymbolTable::GetValue, return_internal_reference<>())
    ;

}
