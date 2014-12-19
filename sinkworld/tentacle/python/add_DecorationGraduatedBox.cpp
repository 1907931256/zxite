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

void add_DecorationGraduatedBox() {
    class_< DecorationGraduatedBox, bases< IDecoration > , boost::noncopyable, DecorationGraduatedBox_Wrapper >("DecorationGraduatedBox", init<  >())
        .def("CreateNew", (IPrototype* (DecorationGraduatedBox::*)() )&DecorationGraduatedBox::CreateNew, (IPrototype* (DecorationGraduatedBox_Wrapper::*)())&DecorationGraduatedBox_Wrapper::default_CreateNew, return_value_policy< manage_new_object >())
        .def("Clone", (IDecoration* (DecorationGraduatedBox::*)() )&DecorationGraduatedBox::Clone, (IDecoration* (DecorationGraduatedBox_Wrapper::*)())&DecorationGraduatedBox_Wrapper::default_Clone, return_value_policy< manage_new_object >())
        .def("SetFore", (void (DecorationGraduatedBox::*)(RGBColor) )&DecorationGraduatedBox::SetFore, (void (DecorationGraduatedBox_Wrapper::*)(RGBColor))&DecorationGraduatedBox_Wrapper::default_SetFore)
        .def("Paint", (void (DecorationGraduatedBox::*)(PaintContext*) )&DecorationGraduatedBox::Paint, (void (DecorationGraduatedBox_Wrapper::*)(PaintContext*))&DecorationGraduatedBox_Wrapper::default_Paint)
    ;

}
