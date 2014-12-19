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

void add_Style() {
    scope* Style_scope = new scope(
    class_< Style >("Style", init<  >())
        .def(init< const Literal* >())
        .def(init< const Literal*, int, bool, bool, bool, RGBColor, RGBColor >())
        .def(init< const Style& >())
        .def_readonly("font", &Style::font)
        .def_readwrite("size", &Style::size)
        .def_readwrite("bold", &Style::bold)
        .def_readwrite("italics", &Style::italics)
        .def_readwrite("underlined", &Style::underlined)
        .def_readwrite("fore", &Style::fore)
        .def_readwrite("back", &Style::back)
        .def_readwrite("decorationFore", &Style::decorationFore)
        .def_readwrite("decorationBack", &Style::decorationBack)
        .def_readwrite("eolFilled", &Style::eolFilled)
        .def("DifferentTo", &Style::DifferentTo)
    );

    scope().attr("sdNone") = (int)Style::sdNone;

    scope().attr("sdFont") = (int)Style::sdFont;

    scope().attr("sdSize") = (int)Style::sdSize;

    scope().attr("sdBold") = (int)Style::sdBold;

    scope().attr("sdItalics") = (int)Style::sdItalics;

    scope().attr("sdUnderlined") = (int)Style::sdUnderlined;

    scope().attr("sdFore") = (int)Style::sdFore;

    scope().attr("sdBack") = (int)Style::sdBack;

    scope().attr("sdSizeRelative") = (int)Style::sdSizeRelative;

    scope().attr("sdDecorationFore") = (int)Style::sdDecorationFore;

    scope().attr("sdDecorationBack") = (int)Style::sdDecorationBack;

    scope().attr("sdEOLFilled") = (int)Style::sdEOLFilled;

    scope().attr("sdAll") = (int)Style::sdAll;

    delete Style_scope;

}
