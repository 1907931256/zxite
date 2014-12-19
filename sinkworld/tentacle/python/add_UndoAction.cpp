#include <boost/python.hpp>
#include <boost/cstdint.hpp>

#include <string>
#include <iostream>
#include <assert.h>
#include "base.h"

// Using =======================================================================
using namespace boost::python;

#include "bbase.h"

void add_UndoAction() {
    class_< UndoAction, boost::noncopyable >("UndoAction", init<  >())
        .def_readwrite("actionType", &UndoAction::actionType)
        .def_readwrite("position", &UndoAction::position)
        .def_readwrite("lenData", &UndoAction::lenData)
        .def_readwrite("data", &UndoAction::data)
    ;

}
