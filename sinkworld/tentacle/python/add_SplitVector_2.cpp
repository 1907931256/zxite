#include <boost/python.hpp>
#include <boost/cstdint.hpp>

#include <string>
#include <iostream>
#include <assert.h>
#include "base.h"

// Using =======================================================================
using namespace boost::python;

#include "bbase.h"

void add_SplitVector_2() {
    class_< SplitVector_2, bases< IStyles > , boost::noncopyable, SplitVector_2_Wrapper >("SplitVector_2", init<  >())
        .def("ValueAt", (int (SplitVector_2::*)(int) )&SplitVector_2::ValueAt, (int (SplitVector_2_Wrapper::*)(int))&SplitVector_2_Wrapper::default_ValueAt)
        .def("FindNextChange", (int (SplitVector_2::*)(int, int) )&SplitVector_2::FindNextChange, (int (SplitVector_2_Wrapper::*)(int, int))&SplitVector_2_Wrapper::default_FindNextChange)
        .def("Length", (int (SplitVector_2::*)() )&SplitVector_2::Length, (int (SplitVector_2_Wrapper::*)())&SplitVector_2_Wrapper::default_Length)
        .def("InsertSpace", (void (SplitVector_2::*)(int, int) )&SplitVector_2::InsertSpace, (void (SplitVector_2_Wrapper::*)(int, int))&SplitVector_2_Wrapper::default_InsertSpace)
        .def("FillRange", (void (SplitVector_2::*)(int, int, int) )&SplitVector_2::FillRange, (void (SplitVector_2_Wrapper::*)(int, int, int))&SplitVector_2_Wrapper::default_FillRange)
        .def("DeleteRange", (void (SplitVector_2::*)(int, int) )&SplitVector_2::DeleteRange, (void (SplitVector_2_Wrapper::*)(int, int))&SplitVector_2_Wrapper::default_DeleteRange)
        .def("DeleteAll", (void (SplitVector_2::*)() )&SplitVector_2::DeleteAll, (void (SplitVector_2_Wrapper::*)())&SplitVector_2_Wrapper::default_DeleteAll)
        .def("Create", &SplitVector_2::Create)
        .def("GetGrowSize", &SplitVector_2::GetGrowSize)
        .def("SetGrowSize", &SplitVector_2::SetGrowSize)
        .def("CharAt", &SplitVector_2::CharAt)
        .def("SetValueAt", &SplitVector_2::SetValueAt)
        .def("RetrieveString", &SplitVector_2::RetrieveString)
        .def("Insert", &SplitVector_2::Insert)
        .def("InsertString", &SplitVector_2::InsertString)
        .def("AppendString", &SplitVector_2::AppendString)
        .def("AppendFromBuffer", &SplitVector_2::AppendFromBuffer)
        .def("Delete", &SplitVector_2::Delete)
    ;

}
