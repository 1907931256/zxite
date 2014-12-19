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

void add_Surface() {
    //class_< Surface, boost::noncopyable >("Surface", init< void*, void* >())
    class_< Surface, boost::noncopyable >("Surface", no_init)
        .def_readwrite("fontDefault", &Surface::fontDefault)
        //.def_readwrite("hdc", &Surface::hdc)
        .def("SetFont", &Surface::SetFont)
        .def("SetTextColor", &Surface::SetTextColor)
        .def("SetPen", &Surface::SetPen)
        .def("Free", &Surface::Free)
        .def("FillRectangle", &Surface::FillRectangle)
        .def("FrameRectangle", &Surface::FrameRectangle)
	.def("Polygon", &Surface::Polygon)
        .def("MoveTo", &Surface::MoveTo)
        .def("LineTo", &Surface::LineTo)
        .def("SetTransparent", &Surface::SetTransparent)
        .def("MeasureWidths", (void (Surface::*)(unsigned char*, int, TextPositions*, int) )&Surface::MeasureWidths)
        .def("MeasureWidths", (void (Surface::*)(SplitText*, int, int, TextPositions*) )&Surface::MeasureWidths)
        .def("PaintText", (void (Surface::*)(unsigned char*, int, SRectangle*, int, int) )&Surface::PaintText)
        .def("PaintText", (void (Surface::*)(SplitText*, int, int, SRectangle*, int) )&Surface::PaintText)
        .def("TextAscent", &Surface::TextAscent)
        .def("TextDescent", &Surface::TextDescent)
        .def("SetClip", &Surface::SetClip)
        .def("Init", &Surface::Init)
        .staticmethod("Init")
        .def("Finalize", &Surface::Finalize)
        .staticmethod("Finalize")
    ;

    class_< SRectangle >("SRectangle", init< int, int, int, int >())
    ;

}
