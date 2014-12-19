#include <boost/python.hpp>
#include <boost/cstdint.hpp>

#include <string>
#include <iostream>
#include <assert.h>
#include "base.h"

// Using =======================================================================
using namespace boost::python;

#include "bbase.h"

void add_UndoStack() {
    scope* UndoStack_scope = new scope(
    class_< UndoStack, boost::noncopyable >("UndoStack", init< int, int >())
        .def_readwrite("actionType", &UndoStack::actionType)
        .def_readwrite("position", &UndoStack::position)
        .def_readwrite("lenData", &UndoStack::lenData)
        .def_readwrite("mayCoalesce", &UndoStack::mayCoalesce)
        .def_readwrite("dataInserted", &UndoStack::dataInserted)
        .def_readwrite("cursorInserted", &UndoStack::cursorInserted)
        .def_readwrite("dataRemoved", &UndoStack::dataRemoved)
        .def_readwrite("cursorRemoved", &UndoStack::cursorRemoved)
        .def_readwrite("currentAction", &UndoStack::currentAction)
        .def_readwrite("maxAction", &UndoStack::maxAction)
        .def_readwrite("undoSequenceDepth", &UndoStack::undoSequenceDepth)
        .def_readwrite("savePoint", &UndoStack::savePoint)
        .def("Create", &UndoStack::Create)
        .def("EnsureDataInserted", &UndoStack::EnsureDataInserted)
        .def("AppendAction", &UndoStack::AppendAction)
        .def("BeginUndoAction", &UndoStack::BeginUndoAction)
        .def("EndUndoAction", &UndoStack::EndUndoAction)
        .def("DropUndoSequence", &UndoStack::DropUndoSequence)
        .def("SetSavePoint", &UndoStack::SetSavePoint)
        .def("IsSavePoint", &UndoStack::IsSavePoint)
        .def("CanUndo", &UndoStack::CanUndo)
        .def("StartUndo", &UndoStack::StartUndo)
        .def("GetUndoStep", &UndoStack::GetUndoStep, return_internal_reference<>())
        .def("AppendInsertDataStep", &UndoStack::AppendInsertDataStep)
        .def("RetrieveRemoveDataStep", &UndoStack::RetrieveRemoveDataStep)
        .def("CompletedUndoStep", &UndoStack::CompletedUndoStep)
        .def("CanRedo", &UndoStack::CanRedo)
        .def("StartRedo", &UndoStack::StartRedo)
        .def("GetRedoStep", &UndoStack::GetRedoStep, return_internal_reference<>())
        .def("CompletedRedoStep", &UndoStack::CompletedRedoStep)
    );

    scope().attr("ACTION_INSERT") = (int)UndoStack::ACTION_INSERT;

    scope().attr("ACTION_REMOVE") = (int)UndoStack::ACTION_REMOVE;

    scope().attr("ACTION_START") = (int)UndoStack::ACTION_START;

    delete UndoStack_scope;

}
