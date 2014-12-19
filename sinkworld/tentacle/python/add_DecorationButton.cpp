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

void add_DecorationButton() {
    class_< DecorationButton, bases< IDecoration > , boost::noncopyable, DecorationButton_Wrapper >("DecorationButton", init< bool >())
        .def("CreateNew", (IPrototype* (DecorationButton::*)() )&DecorationButton::CreateNew, (IPrototype* (DecorationButton_Wrapper::*)())&DecorationButton_Wrapper::default_CreateNew, return_value_policy< manage_new_object >())
        .def("Clone", (IDecoration* (DecorationButton::*)() )&DecorationButton::Clone, (IDecoration* (DecorationButton_Wrapper::*)())&DecorationButton_Wrapper::default_Clone, return_value_policy< manage_new_object >())
        .def("SetFore", (void (DecorationButton::*)(RGBColor) )&DecorationButton::SetFore, (void (DecorationButton_Wrapper::*)(RGBColor))&DecorationButton_Wrapper::default_SetFore)
        .def("Paint", (void (DecorationButton::*)(PaintContext*) )&DecorationButton::Paint, (void (DecorationButton_Wrapper::*)(PaintContext*))&DecorationButton_Wrapper::default_Paint)
    ;

}
