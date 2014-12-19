#include <boost/python.hpp>
#include <boost/cstdint.hpp>

#include <string>
#include <iostream>
#include <assert.h>
#include "base.h"

// Using =======================================================================
using namespace boost::python;

#include "bbase.h"

void add_SplitVector_4() {
    class_< SplitVector_4, bases< IStyles > , boost::noncopyable, SplitVector_4_Wrapper >("SplitVector_4", init<  >())
        .def("ValueAt", (int (SplitVector_4::*)(int) )&SplitVector_4::ValueAt, (int (SplitVector_4_Wrapper::*)(int))&SplitVector_4_Wrapper::default_ValueAt)
        .def("FindNextChange", (int (SplitVector_4::*)(int, int) )&SplitVector_4::FindNextChange, (int (SplitVector_4_Wrapper::*)(int, int))&SplitVector_4_Wrapper::default_FindNextChange)
        .def("Length", (int (SplitVector_4::*)() )&SplitVector_4::Length, (int (SplitVector_4_Wrapper::*)())&SplitVector_4_Wrapper::default_Length)
        .def("InsertSpace", (void (SplitVector_4::*)(int, int) )&SplitVector_4::InsertSpace, (void (SplitVector_4_Wrapper::*)(int, int))&SplitVector_4_Wrapper::default_InsertSpace)
        .def("FillRange", (void (SplitVector_4::*)(int, int, int) )&SplitVector_4::FillRange, (void (SplitVector_4_Wrapper::*)(int, int, int))&SplitVector_4_Wrapper::default_FillRange)
        .def("DeleteRange", (void (SplitVector_4::*)(int, int) )&SplitVector_4::DeleteRange, (void (SplitVector_4_Wrapper::*)(int, int))&SplitVector_4_Wrapper::default_DeleteRange)
        .def("DeleteAll", (void (SplitVector_4::*)() )&SplitVector_4::DeleteAll, (void (SplitVector_4_Wrapper::*)())&SplitVector_4_Wrapper::default_DeleteAll)
        .def("Create", &SplitVector_4::Create)
        .def("GetGrowSize", &SplitVector_4::GetGrowSize)
        .def("SetGrowSize", &SplitVector_4::SetGrowSize)
        .def("CharAt", &SplitVector_4::CharAt)
        .def("SetValueAt", &SplitVector_4::SetValueAt)
        .def("RetrieveString", &SplitVector_4::RetrieveString)
        .def("Insert", &SplitVector_4::Insert)
        .def("InsertString", &SplitVector_4::InsertString)
        .def("AppendString", &SplitVector_4::AppendString)
        .def("AppendFromBuffer", &SplitVector_4::AppendFromBuffer)
        .def("Delete", &SplitVector_4::Delete)
    ;

}
