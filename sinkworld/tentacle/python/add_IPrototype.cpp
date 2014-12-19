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

struct IPrototypeWrap : IPrototype, wrapper<IPrototype> {
    Literal *Name() {
	PyGILState_STATE h = PyGILState_Ensure();
	Literal *n = this->get_override("Name")();
	PyGILState_Release(h);
	return n;
    }

    IPrototype *CreateNew() {
	PyGILState_STATE h = PyGILState_Ensure();
	IPrototype *ip = this->get_override("CreateNew")();
	PyGILState_Release(h);
	return ip;
    }
};

void add_IPrototype() {
    class_< IPrototypeWrap, boost::noncopyable >("IPrototype")
        .def("Name", pure_virtual(&IPrototype::Name), return_internal_reference<>())
        .def("CreateNew", pure_virtual(&IPrototype::CreateNew), return_value_policy< manage_new_object >())
    ;
}
