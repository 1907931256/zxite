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

#if unix
#include <gtk/gtk.h>
#define typename typo
#include <pygobject.h>           
#include <pygtk/pygtk.h>    
#undef typename
void call_create_window(TentacleControl &tc, PyObject *o) {
	PyGObject *py_widget = (PyGObject *)o;
	GtkWidget *widget = GTK_WIDGET(py_widget->obj);
	tc.CreateControl((void *)widget);
}
void add_to(TentacleControl &tc, PyObject *o) {
	PyGObject *py_widget = (PyGObject *)o;
	GtkContainer *container = GTK_CONTAINER(py_widget->obj);
	gtk_container_add(container, (GtkWidget *)tc.TopWindow());
}
#else
void call_create_window(TentacleControl &tc, int parent) {
	tc.CreateControl((HWND) parent);
}
void break_to_debugger() {
	__debugbreak();
}
#endif

class GILWrapper : public Wrapper {
public:
	PyGILState_STATE h;
	int entries;

	GILWrapper() {
		h = PyGILState_UNLOCKED;
		entries = 0;
	}

	virtual ~GILWrapper() {
	}

	void Start() {
		if (entries == 0) {
			h = PyGILState_Ensure();
		}
		entries++;
	}

	void Finish() {
		entries--;
		if (entries == 0) {
			PyGILState_Release(h);
		}
	}
};

static GILWrapper gilwrap;

static void Initialise() {
	TentacleControl::SetWrapper(&gilwrap);
}

void add_TentacleControl() {
    scope* TentacleControl_scope = new scope(
    class_< TentacleControl, boost::noncopyable, TentacleControl_Wrapper >("TentacleControl", init<  >())
        //~ .def_readwrite("braceLevel", &TentacleControl::braceLevel)
        //~ .def_readwrite("fancy", &TentacleControl::fancy)
        .def_readwrite("wrap", &TentacleControl::wrap)
	.add_property("tv", 
		make_getter(&TentacleControl::tv, return_internal_reference<>()))
	.add_property("sr", 
		make_getter(&TentacleControl::sr, return_internal_reference<>()))
        //~ .def_readwrite("pstyleFeedBack", &TentacleControl::pstyleFeedBack)
        //~ .def_readwrite("ssSelection", &TentacleControl::ssSelection)
        //~ .def_readwrite("ssCaret", &TentacleControl::ssCaret)
        .def("TopWindow", pure_virtual((int (TentacleControl::*)())&TentacleControl::TopWindow))
        .def("HasFocus", pure_virtual(&TentacleControl::HasFocus))
        .def("Focus", pure_virtual(&TentacleControl::Focus))
        .def("SurfaceForMeasurement", pure_virtual(&TentacleControl::SurfaceForMeasurement), return_value_policy< manage_new_object >())
        .def("PositionFromLocation", pure_virtual(&TentacleControl::PositionFromLocation))
        .def("SetPosition", &TentacleControl::SetPosition, &TentacleControl_Wrapper::default_SetPosition)
        .def("GetViewRectangle", pure_virtual(&TentacleControl::GetViewRectangle))
        .def("CreateControl", call_create_window)
        .def("EditScrollWindow", pure_virtual(&TentacleControl::EditScrollWindow))
        .def("SetScrollPos", pure_virtual(&TentacleControl::SetScrollPos))
        .def("SetScrollBars", pure_virtual(&TentacleControl::SetScrollBars))
        .def("SetPrimary", pure_virtual(&TentacleControl::SetPrimary))
        .def("Copy", pure_virtual(&TentacleControl::Copy))
        .def("Paste", pure_virtual(&TentacleControl::Paste))
        .def("SetIdle", pure_virtual(&TentacleControl::SetIdle))
        .def("Deallocate", &TentacleControl::Deallocate)
        .def("InsertBytes", (int (TentacleControl::*)(int, char *, int))&TentacleControl::InsertBytes)
        .def("InsertUTF32", &TentacleControl::InsertUTF32)
        .def("InsertUTF8", (int (TentacleControl::*)(int, char *, int))&TentacleControl::InsertUTF8)
        .def("DeleteRange", &TentacleControl::DeleteRange)
        .def("CheckWrap", &TentacleControl::CheckWrap)
        .def("SetDocument", &TentacleControl::SetDocument)
        .def("SetSelectionStyles", &TentacleControl::SetSelectionStyles)
        .def("InvalidateRectangle", &TentacleControl::InvalidateRectangle)
        .def("InvalidateRange", &TentacleControl::InvalidateRange)
        .def("Invalidate", &TentacleControl::Invalidate)
        .def("PerformIdle", &TentacleControl::PerformIdle)
        .def("SizeChanged", &TentacleControl::SizeChanged)
        .def("ScrollTo", &TentacleControl::ScrollTo)
        .def("PaintRect", &TentacleControl::PaintRect)
        .def("SetSelection", (void (TentacleControl::*)(int, int) )&TentacleControl::SetSelection)
        .def("SetSelection", (void (TentacleControl::*)(int) )&TentacleControl::SetSelection)
        .def("MoveCaret", &TentacleControl::MoveCaret, TentacleControl_MoveCaret_overloads_2_3())
        .def("CheckLineForInvalidate", &TentacleControl::CheckLineForInvalidate)
        .def("LineDuplicate", &TentacleControl::LineDuplicate)
        .def("KeyPressed", &TentacleControl::KeyPressed)
        .def("CharPressed", &TentacleControl::CharPressed)
        .def("Undo", &TentacleControl::Undo)
        .def("Redo", &TentacleControl::Redo)
        .def("Cut", &TentacleControl::Cut)
        .def("Clear", &TentacleControl::Clear)
        .def("SelectAll", &TentacleControl::SelectAll)
        .def("FoldLine", &TentacleControl::FoldLine)
        .def("FoldAll", &TentacleControl::FoldAll)
        .def("ToggleFold", &TentacleControl::ToggleFold)
        .def("ToggleBraceLevel", &TentacleControl::ToggleBraceLevel)
        .def("ToggleFancy", &TentacleControl::ToggleFancy)
        .def("StartWrap", &TentacleControl::StartWrap)
        .def("ToggleWrap", &TentacleControl::ToggleWrap)
        .def("EnsureCaretVisible", &TentacleControl::EnsureCaretVisible)
        .def("MouseDrag", &TentacleControl::MouseDrag)
        .def("LiteralFromRange", &TentacleControl::LiteralFromRange, return_value_policy< manage_new_object >())
        .def("WordAtPosition", &TentacleControl::WordAtPosition, return_value_policy< manage_new_object >())
        .def("ShowFeedBack", &TentacleControl::ShowFeedBack)
        .def("MouseFeedBack", &TentacleControl::MouseFeedBack)
        .def("MouseDown", &TentacleControl::MouseDown)
        .def("MouseUp", &TentacleControl::MouseUp)
        .def("Tick", &TentacleControl::Tick)
        .def("Allocate", &TentacleControl::Allocate, return_value_policy< manage_new_object >())
        .staticmethod("Allocate")
#if unix
        .def("AddTo", &add_to)
#else
        .def("Debugger", &break_to_debugger)
        .staticmethod("Debugger")
#endif
        .def("Init", &Initialise)
        .staticmethod("Init")
    );

    enum_< TentacleControl::FoldCommand >("FoldCommand")
        .value("fcDiscover", TentacleControl::fcDiscover)
        .value("fcShow", TentacleControl::fcShow)
        .value("fcFold", TentacleControl::fcFold)
    ;

    delete TentacleControl_scope;

}
