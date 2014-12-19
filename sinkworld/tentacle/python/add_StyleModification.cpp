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

void add_StyleModification() {
    class_< StyleModification, bases< Style > , boost::noncopyable >("StyleModification", init<  >())
        .def(init< const Style& >())
        .def_readwrite("specified", &StyleModification::specified)
        .def("Modify", &StyleModification::Modify)
        .def("AddFont", &StyleModification::AddFont)
        .def("AddSize", &StyleModification::AddSize)
        .def("AddBold", &StyleModification::AddBold)
        .def("AddItalics", &StyleModification::AddItalics)
        .def("AddFore", &StyleModification::AddFore)
        .def("AddBack", &StyleModification::AddBack)
        .def("AddDecorationFore", &StyleModification::AddDecorationFore)
        .def("AddDecorationBack", &StyleModification::AddDecorationBack)
        .def("AddEOLFilled", &StyleModification::AddEOLFilled)
        .def("DigitValue", &StyleModification::DigitValue)
        .def("ColourFromLiteral", &StyleModification::ColourFromLiteral)
        .def("AddFacet", &StyleModification::AddFacet)
        .def("FromOptions", &StyleModification::FromOptions, return_value_policy< manage_new_object >())
        .staticmethod("ColourFromLiteral")
        .staticmethod("FromOptions")
        .staticmethod("DigitValue")
    ;

}
