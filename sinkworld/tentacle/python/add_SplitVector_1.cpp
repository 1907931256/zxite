#include <boost/python.hpp>
#include <boost/cstdint.hpp>

#include <string>
#include <iostream>
#include <assert.h>
#include "base.h"

// Using =======================================================================
using namespace boost::python;

#include "bbase.h"

void add_SplitVector_1() {
    class_< SplitVector_1, bases< IStyles > , boost::noncopyable, SplitVector_1_Wrapper >("SplitVector_1", init<  >())
        .def("ValueAt", (int (SplitVector_1::*)(int) )&SplitVector_1::ValueAt, (int (SplitVector_1_Wrapper::*)(int))&SplitVector_1_Wrapper::default_ValueAt)
        .def("FindNextChange", (int (SplitVector_1::*)(int, int) )&SplitVector_1::FindNextChange, (int (SplitVector_1_Wrapper::*)(int, int))&SplitVector_1_Wrapper::default_FindNextChange)
        .def("Length", (int (SplitVector_1::*)() )&SplitVector_1::Length, (int (SplitVector_1_Wrapper::*)())&SplitVector_1_Wrapper::default_Length)
        .def("InsertSpace", (void (SplitVector_1::*)(int, int) )&SplitVector_1::InsertSpace, (void (SplitVector_1_Wrapper::*)(int, int))&SplitVector_1_Wrapper::default_InsertSpace)
        .def("FillRange", (void (SplitVector_1::*)(int, int, int) )&SplitVector_1::FillRange, (void (SplitVector_1_Wrapper::*)(int, int, int))&SplitVector_1_Wrapper::default_FillRange)
        .def("DeleteRange", (void (SplitVector_1::*)(int, int) )&SplitVector_1::DeleteRange, (void (SplitVector_1_Wrapper::*)(int, int))&SplitVector_1_Wrapper::default_DeleteRange)
        .def("DeleteAll", (void (SplitVector_1::*)() )&SplitVector_1::DeleteAll, (void (SplitVector_1_Wrapper::*)())&SplitVector_1_Wrapper::default_DeleteAll)
        .def("Create", &SplitVector_1::Create)
        .def("GetGrowSize", &SplitVector_1::GetGrowSize)
        .def("SetGrowSize", &SplitVector_1::SetGrowSize)
        .def("CharAt", &SplitVector_1::CharAt)
        .def("SetValueAt", &SplitVector_1::SetValueAt)
        .def("RetrieveString", &SplitVector_1::RetrieveString)
        .def("Insert", &SplitVector_1::Insert)
        .def("InsertString", &SplitVector_1::InsertString)
        .def("AppendString", &SplitVector_1::AppendString)
        .def("AppendFromBuffer", &SplitVector_1::AppendFromBuffer)
        .def("Delete", &SplitVector_1::Delete)
    ;

}
