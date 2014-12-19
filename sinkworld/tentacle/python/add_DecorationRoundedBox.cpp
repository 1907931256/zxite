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

void add_DecorationRoundedBox() {
    class_< DecorationRoundedBox, bases< IDecoration > , boost::noncopyable, DecorationRoundedBox_Wrapper >("DecorationRoundedBox", init<  >())
        .def("CreateNew", (IPrototype* (DecorationRoundedBox::*)() )&DecorationRoundedBox::CreateNew, (IPrototype* (DecorationRoundedBox_Wrapper::*)())&DecorationRoundedBox_Wrapper::default_CreateNew, return_value_policy< manage_new_object >())
        .def("Clone", (IDecoration* (DecorationRoundedBox::*)() )&DecorationRoundedBox::Clone, (IDecoration* (DecorationRoundedBox_Wrapper::*)())&DecorationRoundedBox_Wrapper::default_Clone, return_value_policy< manage_new_object >())
        .def("SetFore", (void (DecorationRoundedBox::*)(RGBColor) )&DecorationRoundedBox::SetFore, (void (DecorationRoundedBox_Wrapper::*)(RGBColor))&DecorationRoundedBox_Wrapper::default_SetFore)
        .def("Paint", (void (DecorationRoundedBox::*)(PaintContext*) )&DecorationRoundedBox::Paint, (void (DecorationRoundedBox_Wrapper::*)(PaintContext*))&DecorationRoundedBox_Wrapper::default_Paint)
    ;

}
