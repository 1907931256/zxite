#include <boost/python.hpp>
#include <boost/cstdint.hpp>

#include <string>
#include <iostream>
#include <assert.h>
#include "base.h"
#include "lexers.h"

// Using =======================================================================
using namespace boost::python;

typedef void *HWND;
typedef void *HINSTANCE;

#include "PhysicalLayout.h"

#include "RegisteredLexers.h"
#include "RGBColor.h"
#include "IDecoration.h"
#include "BaseDecoration.h"
#include "RegisteredDecorations.h"
#include "Style.h"
#include "StyleModification.h"
#include "StyleSet.h"
#include "StyleSetCollection.h"
#include "FontDescription.h"
#include "Surface.h"
#include "ChangeLayer.h"
#include "ChangeFinder.h"
#include "StyleLayer.h"
#include "SubstanceLayer.h"
#include "PositionCache.h"
#include "LinePositionCache.h"
#include "ChangePositionsCache.h"
#include "Document.h"

#include "PhasedPaint.h"
#include "TextView.h"
#include "RangeSelection.h"

#include "DecorationBox.h"
#include "DecorationButton.h"
#include "DecorationCaret.h"
#include "DecorationGraduatedBox.h"
#include "DecorationStrikeOut.h"
#include "DecorationRoundedBox.h"
#include "DecorationUnderLine.h"

#include "TentacleControl.h"

#include "SWWrappers.h"

void add_RangeSelection() {
    class_< RangeSelection, boost::noncopyable, RangeSelection_Wrapper >("RangeSelection", init<  >())
        .def_readwrite("pstyleSelection", &RangeSelection::pstyleSelection)
        .def("Length", (int (RangeSelection::*)() )&RangeSelection::Length, (int (RangeSelection_Wrapper::*)())&RangeSelection_Wrapper::default_Length)
        .def("ValueAt", (int (RangeSelection::*)(int) )&RangeSelection::ValueAt, (int (RangeSelection_Wrapper::*)(int))&RangeSelection_Wrapper::default_ValueAt)
        .def("FindNextChange", (int (RangeSelection::*)(int, int) )&RangeSelection::FindNextChange, (int (RangeSelection_Wrapper::*)(int, int))&RangeSelection_Wrapper::default_FindNextChange)
        .def("FillRange", (void (RangeSelection::*)(int, int, int) )&RangeSelection::FillRange, (void (RangeSelection_Wrapper::*)(int, int, int))&RangeSelection_Wrapper::default_FillRange)
        .def("InsertSpace", (void (RangeSelection::*)(int, int) )&RangeSelection::InsertSpace, (void (RangeSelection_Wrapper::*)(int, int))&RangeSelection_Wrapper::default_InsertSpace)
        .def("DeleteAll", (void (RangeSelection::*)() )&RangeSelection::DeleteAll, (void (RangeSelection_Wrapper::*)())&RangeSelection_Wrapper::default_DeleteAll)
        .def("DeleteRange", (void (RangeSelection::*)(int, int) )&RangeSelection::DeleteRange, (void (RangeSelection_Wrapper::*)(int, int))&RangeSelection_Wrapper::default_DeleteRange)
        .def("Allocate", &RangeSelection::Allocate)
        .def("Deallocate", &RangeSelection::Deallocate)
        .def("Caret", &RangeSelection::Caret)
        .def("Anchor", &RangeSelection::Anchor)
        .def("StartSelection", &RangeSelection::StartSelection)
        .def("EndSelection", &RangeSelection::EndSelection)
        .def("SetSelectionStyleRanges", &RangeSelection::SetSelectionStyleRanges)
        .def("Empty", &RangeSelection::Empty)
    ;

}
