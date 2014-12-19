#include <boost/python.hpp>
#include <boost/cstdint.hpp>

#include <string>
#include <iostream>
#include <assert.h>
#include "base.h"

// Using =======================================================================
using namespace boost::python;

#include "bbase.h"

void add_RunStyles() {
    class_< RunStyles, bases< IStyles > , boost::noncopyable, RunStyles_Wrapper >("RunStyles", init<  >())
        .def_readwrite("starts", &RunStyles::starts)
        .def_readwrite("styles", &RunStyles::styles)
        .def("Length", (int (RunStyles::*)() )&RunStyles::Length, (int (RunStyles_Wrapper::*)())&RunStyles_Wrapper::default_Length)
        .def("ValueAt", (int (RunStyles::*)(int) )&RunStyles::ValueAt, (int (RunStyles_Wrapper::*)(int))&RunStyles_Wrapper::default_ValueAt)
        .def("FindNextChange", (int (RunStyles::*)(int, int) )&RunStyles::FindNextChange, (int (RunStyles_Wrapper::*)(int, int))&RunStyles_Wrapper::default_FindNextChange)
        .def("FillRange", (void (RunStyles::*)(int, int, int) )&RunStyles::FillRange, (void (RunStyles_Wrapper::*)(int, int, int))&RunStyles_Wrapper::default_FillRange)
        .def("InsertSpace", (void (RunStyles::*)(int, int) )&RunStyles::InsertSpace, (void (RunStyles_Wrapper::*)(int, int))&RunStyles_Wrapper::default_InsertSpace)
        .def("DeleteAll", (void (RunStyles::*)() )&RunStyles::DeleteAll, (void (RunStyles_Wrapper::*)())&RunStyles_Wrapper::default_DeleteAll)
        .def("DeleteRange", (void (RunStyles::*)(int, int) )&RunStyles::DeleteRange, (void (RunStyles_Wrapper::*)(int, int))&RunStyles_Wrapper::default_DeleteRange)
        .def("RunFromPosition", &RunStyles::RunFromPosition)
        .def("SplitRun", &RunStyles::SplitRun)
        .def("RemoveRun", &RunStyles::RemoveRun)
        .def("CoalesceRuns", &RunStyles::CoalesceRuns)
        .def("Runs", &RunStyles::Runs)
    ;

}
