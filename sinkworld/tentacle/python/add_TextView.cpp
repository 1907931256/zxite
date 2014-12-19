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

void add_TextView() {
    class_< TextView, boost::noncopyable >("TextView", init<  >())
	.add_property("fs", 
		make_getter(&TextView::fs, return_internal_reference<>()))
        .def_readwrite("leftColumn", &TextView::leftColumn)
        .def_readwrite("textLeft", &TextView::textLeft)
        .def_readwrite("wrap", &TextView::wrap)
        .def_readwrite("wrapWidth", &TextView::wrapWidth)
        .def_readwrite("magnification", &TextView::magnification)
        .def_readwrite("lineHeight", &TextView::lineHeight)
        .def_readwrite("pageHeight", &TextView::pageHeight)
        .def_readwrite("lineWidth", &TextView::lineWidth)
        .def_readwrite("accurateWidths", &TextView::accurateWidths)
        .def_readwrite("ascent", &TextView::ascent)
        .def_readwrite("topLine", &TextView::topLine)
        .def_readwrite("tabWidth", &TextView::tabWidth)
        .def("Deallocate", &TextView::Deallocate)
        .def("StylesChanged", &TextView::StylesChanged)
        .def("SetText", &TextView::SetText)
        .def("SetStyles", &TextView::SetStyles)
        .def("SetFolds", &TextView::SetFolds)
        .def("RemoveStyleSet", &TextView::RemoveStyleSet)
        .def("AddStyleSet", &TextView::AddStyleSet)
        .def("SetRectangle", &TextView::SetRectangle)
        .def("ModifyMagnification", &TextView::ModifyMagnification)
        .def("InvalidateChangePositions", &TextView::InvalidateChangePositions)
        .def("DisplayLineFromPosition", &TextView::DisplayLineFromPosition)
        .def("PageSize", &TextView::PageSize)
        .def("MaxScrollPos", &TextView::MaxScrollPos)
        .def("ChangePositionsFromLine", &TextView::ChangePositionsFromLine, return_internal_reference<>())
        .def("MeasureText", &TextView::MeasureText, return_internal_reference<>())
        .def("PositionFromSurfaceLocation", &TextView::PositionFromSurfaceLocation)
        .def("WrapLine", &TextView::WrapLine)
        .def("PaintRect", &TextView::PaintRect)
        .def("AddColumn", &TextView::AddColumn)
        .def("IsSpaceOrTab", &TextView::IsSpaceOrTab)
        .staticmethod("IsSpaceOrTab")
    ;

}
