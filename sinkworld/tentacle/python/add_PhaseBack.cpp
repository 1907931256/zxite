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

void add_PhaseBack() {
    class_< PhaseBack, bases< IPaintPhase > , boost::noncopyable, PhaseBack_Wrapper >("PhaseBack", no_init)
        .def("FlagsUse", (int (PhaseBack::*)() )&PhaseBack::FlagsUse, (int (PhaseBack_Wrapper::*)())&PhaseBack_Wrapper::default_FlagsUse)
        .def("Paint", (void (PhaseBack::*)(PaintParameters*) )&PhaseBack::Paint, (void (PhaseBack_Wrapper::*)(PaintParameters*))&PhaseBack_Wrapper::default_Paint)
    ;

}
