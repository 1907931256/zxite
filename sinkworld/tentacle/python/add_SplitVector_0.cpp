#include <boost/python.hpp>
#include <boost/cstdint.hpp>

#include <string>
#include <iostream>
#include <assert.h>
#include "base.h"

// Using =======================================================================
using namespace boost::python;

#include "bbase.h"

void add_SplitVector_0() {
    class_< SplitVector_0, bases< IStyles > , boost::noncopyable, SplitVector_0_Wrapper >("SplitVector_0", init<  >())
        .def("Length", (int (SplitVector_0::*)() )&SplitVector_0::Length, (int (SplitVector_0_Wrapper::*)())&SplitVector_0_Wrapper::default_Length)
        .def("ValueAt", (int (SplitVector_0::*)(int) )&SplitVector_0::ValueAt, (int (SplitVector_0_Wrapper::*)(int))&SplitVector_0_Wrapper::default_ValueAt)
        .def("FindNextChange", (int (SplitVector_0::*)(int, int) )&SplitVector_0::FindNextChange, (int (SplitVector_0_Wrapper::*)(int, int))&SplitVector_0_Wrapper::default_FindNextChange)
        .def("FillRange", (void (SplitVector_0::*)(int, int, int) )&SplitVector_0::FillRange, (void (SplitVector_0_Wrapper::*)(int, int, int))&SplitVector_0_Wrapper::default_FillRange)
        .def("InsertSpace", (void (SplitVector_0::*)(int, int) )&SplitVector_0::InsertSpace, (void (SplitVector_0_Wrapper::*)(int, int))&SplitVector_0_Wrapper::default_InsertSpace)
        .def("DeleteAll", (void (SplitVector_0::*)() )&SplitVector_0::DeleteAll, (void (SplitVector_0_Wrapper::*)())&SplitVector_0_Wrapper::default_DeleteAll)
        .def("DeleteRange", (void (SplitVector_0::*)(int, int) )&SplitVector_0::DeleteRange, (void (SplitVector_0_Wrapper::*)(int, int))&SplitVector_0_Wrapper::default_DeleteRange)
    ;

}
