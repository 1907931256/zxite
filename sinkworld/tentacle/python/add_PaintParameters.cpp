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

void add_PaintParameters() {
    class_< PaintParameters, boost::noncopyable >("PaintParameters", no_init)
        .def_readwrite("pbuffer", &PaintParameters::pbuffer)
        .def_readwrite("ssc", &PaintParameters::ssc)
        .def_readwrite("fs", &PaintParameters::fs)
        .def_readwrite("surface", &PaintParameters::surface)
        .def_readwrite("leftColumn", &PaintParameters::leftColumn)
        .def_readwrite("magnification", &PaintParameters::magnification)
        .def_readwrite("lineWidth", &PaintParameters::lineWidth)
        .def_readwrite("ascent", &PaintParameters::ascent)
        .def_readwrite("lineDoc", &PaintParameters::lineDoc)
        .def_readwrite("subLine", &PaintParameters::subLine)
        .def_readwrite("subLineStart", &PaintParameters::subLineStart)
        .def_readwrite("subLineEnd", &PaintParameters::subLineEnd)
        .def_readwrite("rcw", &PaintParameters::rcw)
        .def_readwrite("left", &PaintParameters::left)
        .def_readonly("cf", &PaintParameters::cf)
        .def_readonly("positions", &PaintParameters::positions)
    ;

}
