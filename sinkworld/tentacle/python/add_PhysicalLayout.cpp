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

void add_PhysicalLayout() {
    class_< PhysicalLayout, boost::noncopyable, PhysicalLayout_Wrapper >("PhysicalLayout", init<  >())
        .def_readwrite("lineFolds", &PhysicalLayout::lineFolds)
        .def_readwrite("next", &LineData::next)
        .def("Clear", (void (PhysicalLayout::*)() )&PhysicalLayout::Clear, (void (PhysicalLayout_Wrapper::*)())&PhysicalLayout_Wrapper::default_Clear)
        .def("InsertLine", (void (PhysicalLayout::*)(int) )&PhysicalLayout::InsertLine, (void (PhysicalLayout_Wrapper::*)(int))&PhysicalLayout_Wrapper::default_InsertLine)
        .def("RemoveLine", (void (PhysicalLayout::*)(int) )&PhysicalLayout::RemoveLine, (void (PhysicalLayout_Wrapper::*)(int))&PhysicalLayout_Wrapper::default_RemoveLine)
        .def("LinesInDoc", &PhysicalLayout::LinesInDoc)
        .def("LinesDisplayed", &PhysicalLayout::LinesDisplayed)
        .def("DisplayFromDoc", &PhysicalLayout::DisplayFromDoc)
        .def("DocFromDisplay", &PhysicalLayout::DocFromDisplay)
        .def("SubLineFromDisplay", &PhysicalLayout::SubLineFromDisplay)
        .def("GetVisible", &PhysicalLayout::GetVisible)
        .def("SetVisible", &PhysicalLayout::SetVisible)
        .def("GetExpanded", &PhysicalLayout::GetExpanded)
        .def("SetExpanded", &PhysicalLayout::SetExpanded)
        .def("GetHeight", &PhysicalLayout::GetHeight)
        .def("SetHeight", &PhysicalLayout::SetHeight)
        .def("GetWidth", &PhysicalLayout::GetWidth)
        .def("SetWidth", &PhysicalLayout::SetWidth)
        .def("GetMaxWidth", &PhysicalLayout::GetMaxWidth)
        .def("ShowAll", &PhysicalLayout::ShowAll)
        .def("InvalidateWidths", &PhysicalLayout::InvalidateWidths)
    ;

}
