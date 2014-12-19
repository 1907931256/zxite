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

static void set_current_mode(Document &doc, Literal *mode) {
	Literal *current = doc.currentMode;
	doc.currentMode = new Literal(mode);
	delete current;
}

void add_Document() {
    class_< Document, boost::noncopyable >("Document", init< Literal*, LiteralSet* >())
	.add_property("pbuffer", 
		make_getter(&Document::pbuffer, return_internal_reference<>()))
	.add_property("options", 
		make_getter(&Document::options, return_internal_reference<>()))
	.add_property("currentMode", 
		make_getter(&Document::currentMode, return_internal_reference<>()),
		make_function(&set_current_mode))
	.add_property("ssc", 
		make_getter(&Document::ssc, return_internal_reference<>()))
        .def_readonly("len", &Document::len)
	.add_property("fontNames", 
		make_getter(&Document::fontNames, return_internal_reference<>()))
        .def("AllocateText", &Document::AllocateText)
        .def("DeallocateStyling", &Document::DeallocateStyling)
        .def("ClearOptions", &Document::ClearOptions)
        .def("SetLexStyles", &Document::SetLexStyles)
        .def("CreateStyling", &Document::CreateStyling)
        .def("IsWordCharacter", &Document::IsWordCharacter)
        .def("IsSpaceTab", &Document::IsSpaceTab)
        .def("MovePositionOutsideChar", &Document::MovePositionOutsideChar)
        .def("ExtendWordSelect", &Document::ExtendWordSelect)
        .def("NextWordStart", &Document::NextWordStart)
        .def("GetFolds", &Document::GetFolds, return_internal_reference<>())
        .staticmethod("IsWordCharacter")
        .staticmethod("IsSpaceTab")
    ;

}
