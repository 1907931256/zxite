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

void add_TextColumn() {
    scope* TextColumn_scope = new scope(
    class_< TextColumn, boost::noncopyable >("TextColumn", init<  >())
        .def_readwrite("next", &TextColumn::next)
        .def_readwrite("type", &TextColumn::type)
        .def_readwrite("width", &TextColumn::width)
        .def_readwrite("left", &TextColumn::left)
        .def_readwrite("right", &TextColumn::right)
    );

    scope().attr("tcText") = (int)TextColumn::tcText;

    scope().attr("tcLineNumber") = (int)TextColumn::tcLineNumber;

    scope().attr("tcSeparator") = (int)TextColumn::tcSeparator;

    scope().attr("tcWhite") = (int)TextColumn::tcWhite;

    scope().attr("tcFold") = (int)TextColumn::tcFold;

    delete TextColumn_scope;

}
