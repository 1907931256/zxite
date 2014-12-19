#include <boost/python.hpp>
#include <boost/cstdint.hpp>

#include <string>
#include <iostream>
#include <assert.h>
#include "base.h"

// Using =======================================================================
using namespace boost::python;

#include "bbase.h"

void add_Substance() {
    class_< Substance, bases< SplitVector_1 > , boost::noncopyable, Substance_Wrapper >("Substance", init<  >())
        .def_readwrite("bytesInUnit", &Substance::bytesInUnit)
        .def("DistanceNextCaret", pure_virtual(&Substance::DistanceNextCaret))
        .def("LenChar", pure_virtual(&Substance::LenChar))
        .def("LenCharacters", pure_virtual(&Substance::LenCharacters))
        .def("BytesFromCharacters", pure_virtual(&Substance::BytesFromCharacters))
        .def("RetrieveUTF8", pure_virtual(&Substance::RetrieveUTF8))
        .def("RetrieveUTF16", pure_virtual(&Substance::RetrieveUTF16))
        .def("RetrieveUTF32", pure_virtual(&Substance::RetrieveUTF32))
        .def("LengthInText", pure_virtual(&Substance::LengthInText))
        .def("UnitValue", pure_virtual(&Substance::UnitValue))
        .def("UnitFromArray", pure_virtual(&Substance::UnitFromArray))
        .def("CharValue", pure_virtual(&Substance::CharValue))
        .def("MovePositionOutsideChar", pure_virtual(&Substance::MovePositionOutsideChar))
        .def("BetweenCharacters", pure_virtual(&Substance::BetweenCharacters))
        .def("ValidateRange", pure_virtual(&Substance::ValidateRange))
        .def("ValidateArray", pure_virtual(&Substance::ValidateArray))
        .def("ValueAt", (int (SplitVector_1::*)(int) )&SplitVector_1::ValueAt, (int (Substance_Wrapper::*)(int))&Substance_Wrapper::default_ValueAt)
        .def("FindNextChange", (int (SplitVector_1::*)(int, int) )&SplitVector_1::FindNextChange, (int (Substance_Wrapper::*)(int, int))&Substance_Wrapper::default_FindNextChange)
        .def("Length", (int (SplitVector_1::*)() )&SplitVector_1::Length, (int (Substance_Wrapper::*)())&Substance_Wrapper::default_Length)
        .def("InsertSpace", (void (SplitVector_1::*)(int, int) )&SplitVector_1::InsertSpace, (void (Substance_Wrapper::*)(int, int))&Substance_Wrapper::default_InsertSpace)
        .def("FillRange", (void (SplitVector_1::*)(int, int, int) )&SplitVector_1::FillRange, (void (Substance_Wrapper::*)(int, int, int))&Substance_Wrapper::default_FillRange)
        .def("DeleteRange", (void (SplitVector_1::*)(int, int) )&SplitVector_1::DeleteRange, (void (Substance_Wrapper::*)(int, int))&Substance_Wrapper::default_DeleteRange)
        .def("DeleteAll", (void (SplitVector_1::*)() )&SplitVector_1::DeleteAll, (void (Substance_Wrapper::*)())&Substance_Wrapper::default_DeleteAll)
        .def("UnsignedByteAt", &Substance::UnsignedByteAt)
        .def("IsCrLf", &Substance::IsCrLf)
    ;

}
