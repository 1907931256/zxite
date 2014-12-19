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

//~ class DocumentChangeListener : public IDocumentChangeListener {
//~ public:
	//~ DocumentChangeListener() {
	//~ }
	//~ virtual void StateAltered() {
	//~ }
	//~ virtual void Invalidate() {
	//~ }
	//~ virtual void InvalidateRange(int, int) {
	//~ }
//~ };

//~ struct DocumentChangeListener_Wrapper: DocumentChangeListener, wrapper<DocumentChangeListener>
//~ {
    //~ DocumentChangeListener_Wrapper(PyObject* self_) : py_self(self_) {
    //~ }

    //~ void StateAltered() {
        //~ call_method< void >(py_self, "StateAltered");
    //~ }

    //~ void Invalidate() {
        //~ call_method< void >(py_self, "Invalidate");
    //~ }

    //~ void InvalidateRange(int p0, int p1) {
        //~ call_method< void >(py_self, "InvalidateRange", p0, p1);
    //~ }

    //~ PyObject* py_self;
//~ };

struct IDocumentChangeListenerWrap : IDocumentChangeListener, wrapper<IDocumentChangeListener> {
    void StateAltered() {
	PyGILState_STATE h = PyGILState_Ensure();
        this->get_override("StateAltered")();
	PyGILState_Release(h);
    }

    void Invalidate() {
	PyGILState_STATE h = PyGILState_Ensure();
        this->get_override("Invalidate")();
	PyGILState_Release(h);
    }

    void InvalidateRange(int p0, int p1) {
	PyGILState_STATE h = PyGILState_Ensure();
        this->get_override("InvalidateRange")(p0, p1);
	PyGILState_Release(h);
    }
};

void add_IDocumentChangeListener() {
    class_< IDocumentChangeListenerWrap, boost::noncopyable >("IDocumentChangeListener")
        .def("StateAltered", pure_virtual(&IDocumentChangeListener::StateAltered))
        .def("Invalidate", pure_virtual(&IDocumentChangeListener::Invalidate))
        .def("InvalidateRange", pure_virtual(&IDocumentChangeListener::InvalidateRange))
    ;

    //~ class_< IDocumentChangeListener, boost::noncopyable, IDocumentChangeListener_Wrapper >("IDocumentChangeListener", no_init)
        //~ .def("StateAltered", pure_virtual(&IDocumentChangeListener::StateAltered))
        //~ .def("Invalidate", pure_virtual(&IDocumentChangeListener::Invalidate))
        //~ .def("InvalidateRange", pure_virtual(&IDocumentChangeListener::InvalidateRange))
    //~ ;

    //~ class_< DocumentChangeListener, bases< IDocumentChangeListener >, boost::noncopyable, DocumentChangeListener_Wrapper >(
		//~ "DocumentChangeListener")
        //~ .def("StateAltered", &DocumentChangeListener::StateAltered)
        //~ .def("Invalidate", &DocumentChangeListener::Invalidate)
        //~ .def("InvalidateRange", &DocumentChangeListener::InvalidateRange)
    //~ ;

}
