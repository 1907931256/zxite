#include <boost/python.hpp>
#include <boost/cstdint.hpp>

#include <string>
#include <iostream>
#include <assert.h>
#include "base.h"

// Using =======================================================================
using namespace boost::python;

#include "bbase.h"

void add_Partitioning() {
    class_< Partitioning, bases< IStyles > , boost::noncopyable, Partitioning_Wrapper >("Partitioning", init< int >())
        .def("Length", (int (Partitioning::*)() )&Partitioning::Length, (int (Partitioning_Wrapper::*)())&Partitioning_Wrapper::default_Length)
        .def("InsertSpace", (void (Partitioning::*)(int, int) )&Partitioning::InsertSpace, (void (Partitioning_Wrapper::*)(int, int))&Partitioning_Wrapper::default_InsertSpace)
        .def("DeleteRange", (void (Partitioning::*)(int, int) )&Partitioning::DeleteRange, (void (Partitioning_Wrapper::*)(int, int))&Partitioning_Wrapper::default_DeleteRange)
        .def("DeleteAll", (void (Partitioning::*)() )&Partitioning::DeleteAll, (void (Partitioning_Wrapper::*)())&Partitioning_Wrapper::default_DeleteAll)
        .def("ValueAt", (int (Partitioning::*)(int) )&Partitioning::ValueAt, (int (Partitioning_Wrapper::*)(int))&Partitioning_Wrapper::default_ValueAt)
        .def("FindNextChange", (int (Partitioning::*)(int, int) )&Partitioning::FindNextChange, (int (Partitioning_Wrapper::*)(int, int))&Partitioning_Wrapper::default_FindNextChange)
        .def("FillRange", (void (Partitioning::*)(int, int, int) )&Partitioning::FillRange, (void (Partitioning_Wrapper::*)(int, int, int))&Partitioning_Wrapper::default_FillRange)
        .def("Partitions", &Partitioning::Partitions)
        .def("DocumentLength", &Partitioning::DocumentLength)
        .def("InsertPartition", &Partitioning::InsertPartition)
        .def("SetPartitionStartPosition", &Partitioning::SetPartitionStartPosition)
        .def("InsertText", &Partitioning::InsertText)
        .def("RemovePartition", &Partitioning::RemovePartition)
        .def("PositionFromPartition", &Partitioning::PositionFromPartition)
        .def("PartitionFromPosition", &Partitioning::PartitionFromPosition)
        .def("RemovePartitions", &Partitioning::RemovePartitions)
        .def("Delete", &Partitioning::Delete)
    ;

}
