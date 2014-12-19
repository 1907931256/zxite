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

void add_StyleSetCollection() {
    class_< StyleSetCollection, boost::noncopyable >("StyleSetCollection", init< int, Literal* >())
        .def_readwrite("isView", &StyleSetCollection::isView)
        .def_readwrite("fontNameDefault", &StyleSetCollection::fontNameDefault)
        .def("Get", &StyleSetCollection::Get, return_internal_reference<>())
        .def("GetMainStyleSet", &StyleSetCollection::GetMainStyleSet, return_internal_reference<>())
        .def("Modify", &StyleSetCollection::Modify)
        .def("Add", &StyleSetCollection::Add)
        .def("Remove", &StyleSetCollection::Remove)
        .def("Length", &StyleSetCollection::Length)
        .def("GetLayer", &StyleSetCollection::GetLayer, return_internal_reference<>())
        .def("SetSpecifiedFlags", &StyleSetCollection::SetSpecifiedFlags)
        .def("View", &StyleSetCollection::View, return_value_policy< manage_new_object >())
    ;

}
