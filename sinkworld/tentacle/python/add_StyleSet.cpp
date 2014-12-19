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

void add_StyleSet() {
    class_< StyleSet, boost::noncopyable >("StyleSet", init< IStyles*, int >())
        .def_readwrite("len", &StyleSet::len)
        .def_readwrite("styles", &StyleSet::styles)
        .def_readwrite("buffer", &StyleSet::buffer)
        .def_readwrite("specified", &StyleSet::specified)
        .def_readwrite("specifiedValid", &StyleSet::specifiedValid)
        .def("Set", &StyleSet::Set)
        .def("Get", &StyleSet::Get, return_internal_reference<>())
        .def("SetSpecifiedFlags", &StyleSet::SetSpecifiedFlags)
        .def("SpecifiedFlags", &StyleSet::SpecifiedFlags)
        .def("GetBuffer", &StyleSet::GetBuffer, return_internal_reference<>())
    ;

}
