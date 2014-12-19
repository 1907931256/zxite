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

#include "bbase.h"


static void add_prototype(Literal *role, LexBase *lb) {
	//~ __debugbreak();
	PrototypeRegistry::AddPrototype(role, lb);
}

void add_PrototypeRegistry() {
    class_< PrototypeRegistry, boost::noncopyable >("PrototypeRegistry", no_init)
        .def("Init", &PrototypeRegistry::Init)
        .def("Finish", &PrototypeRegistry::Finish)
        .def("FromName", &PrototypeRegistry::FromName, return_value_policy< manage_new_object >())
        //.def("AddPrototype", &PrototypeRegistry::AddPrototype)
        .def("AddPrototype", &add_prototype)
        .staticmethod("Init")
        .staticmethod("Finish")
        .staticmethod("FromName")
        .staticmethod("AddPrototype")
    ;

}
