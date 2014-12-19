#include <boost/python.hpp>
#include <boost/cstdint.hpp>

#include <string>
#include <iostream>
#include <assert.h>
#include "base.h"

// Using =======================================================================
using namespace boost::python;

#include "bbase.h"

void add_SinglePosition() {
    class_< SinglePosition, bases< IStyles > , boost::noncopyable, SinglePosition_Wrapper >("SinglePosition", init<  >())
        .def("Length", (int (SinglePosition::*)() )&SinglePosition::Length, (int (SinglePosition_Wrapper::*)())&SinglePosition_Wrapper::default_Length)
        .def("ValueAt", (int (SinglePosition::*)(int) )&SinglePosition::ValueAt, (int (SinglePosition_Wrapper::*)(int))&SinglePosition_Wrapper::default_ValueAt)
        .def("FindNextChange", (int (SinglePosition::*)(int, int) )&SinglePosition::FindNextChange, (int (SinglePosition_Wrapper::*)(int, int))&SinglePosition_Wrapper::default_FindNextChange)
        .def("FillRange", (void (SinglePosition::*)(int, int, int) )&SinglePosition::FillRange, (void (SinglePosition_Wrapper::*)(int, int, int))&SinglePosition_Wrapper::default_FillRange)
        .def("InsertSpace", (void (SinglePosition::*)(int, int) )&SinglePosition::InsertSpace, (void (SinglePosition_Wrapper::*)(int, int))&SinglePosition_Wrapper::default_InsertSpace)
        .def("DeleteAll", (void (SinglePosition::*)() )&SinglePosition::DeleteAll, (void (SinglePosition_Wrapper::*)())&SinglePosition_Wrapper::default_DeleteAll)
        .def("DeleteRange", (void (SinglePosition::*)(int, int) )&SinglePosition::DeleteRange, (void (SinglePosition_Wrapper::*)(int, int))&SinglePosition_Wrapper::default_DeleteRange)
        .def("Get", &SinglePosition::Get)
    ;

}
