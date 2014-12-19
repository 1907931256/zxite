#include <boost/python.hpp>
#include <boost/cstdint.hpp>

#include <string>
#include <iostream>
#include <assert.h>
#include "base.h"

// Using =======================================================================
using namespace boost::python;

#include "bbase.h"

void add_SubstanceUTF8() {
    class_< SubstanceUTF8, bases< Substance > , boost::noncopyable, SubstanceUTF8_Wrapper >("SubstanceUTF8", init<  >())
        .def("DistanceNextCaret", (int (SubstanceUTF8::*)(int) )&SubstanceUTF8::DistanceNextCaret, (int (SubstanceUTF8_Wrapper::*)(int))&SubstanceUTF8_Wrapper::default_DistanceNextCaret)
        .def("LenChar", (int (SubstanceUTF8::*)(int) )&SubstanceUTF8::LenChar, (int (SubstanceUTF8_Wrapper::*)(int))&SubstanceUTF8_Wrapper::default_LenChar)
        .def("LenCharacters", (int (SubstanceUTF8::*)(int*, int) )&SubstanceUTF8::LenCharacters, (int (SubstanceUTF8_Wrapper::*)(int*, int))&SubstanceUTF8_Wrapper::default_LenCharacters)
        .def("BytesFromCharacters", (int (SubstanceUTF8::*)(unsigned char*, int, int*, int) )&SubstanceUTF8::BytesFromCharacters, (int (SubstanceUTF8_Wrapper::*)(unsigned char*, int, int*, int))&SubstanceUTF8_Wrapper::default_BytesFromCharacters)
        .def("RetrieveUTF8", (int (SubstanceUTF8::*)(int, unsigned char*, int) )&SubstanceUTF8::RetrieveUTF8, (int (SubstanceUTF8_Wrapper::*)(int, unsigned char*, int))&SubstanceUTF8_Wrapper::default_RetrieveUTF8)
        .def("RetrieveUTF16", (int (SubstanceUTF8::*)(int, short int*, int) )&SubstanceUTF8::RetrieveUTF16, (int (SubstanceUTF8_Wrapper::*)(int, short int*, int))&SubstanceUTF8_Wrapper::default_RetrieveUTF16)
        .def("RetrieveUTF32", (int (SubstanceUTF8::*)(int, int*, int) )&SubstanceUTF8::RetrieveUTF32, (int (SubstanceUTF8_Wrapper::*)(int, int*, int))&SubstanceUTF8_Wrapper::default_RetrieveUTF32)
        .def("LengthInText", (int (SubstanceUTF8::*)(int*, int) )&SubstanceUTF8::LengthInText, (int (SubstanceUTF8_Wrapper::*)(int*, int))&SubstanceUTF8_Wrapper::default_LengthInText)
        .def("UnitValue", (int (SubstanceUTF8::*)(int) )&SubstanceUTF8::UnitValue, (int (SubstanceUTF8_Wrapper::*)(int))&SubstanceUTF8_Wrapper::default_UnitValue)
        .def("UnitFromArray", (int (SubstanceUTF8::*)(unsigned char*, int) )&SubstanceUTF8::UnitFromArray, (int (SubstanceUTF8_Wrapper::*)(unsigned char*, int))&SubstanceUTF8_Wrapper::default_UnitFromArray)
        .def("CharValue", (int (SubstanceUTF8::*)(int) )&SubstanceUTF8::CharValue, (int (SubstanceUTF8_Wrapper::*)(int))&SubstanceUTF8_Wrapper::default_CharValue)
        .def("MovePositionOutsideChar", (int (SubstanceUTF8::*)(int, int, bool) )&SubstanceUTF8::MovePositionOutsideChar, (int (SubstanceUTF8_Wrapper::*)(int, int, bool))&SubstanceUTF8_Wrapper::default_MovePositionOutsideChar)
        .def("BetweenCharacters", (bool (SubstanceUTF8::*)(int) )&SubstanceUTF8::BetweenCharacters, (bool (SubstanceUTF8_Wrapper::*)(int))&SubstanceUTF8_Wrapper::default_BetweenCharacters)
        .def("ValidateRange", (bool (SubstanceUTF8::*)(int, int) )&SubstanceUTF8::ValidateRange, (bool (SubstanceUTF8_Wrapper::*)(int, int))&SubstanceUTF8_Wrapper::default_ValidateRange)
        .def("ValidateArray", (bool (SubstanceUTF8::*)(unsigned char*, int) )&SubstanceUTF8::ValidateArray, (bool (SubstanceUTF8_Wrapper::*)(unsigned char*, int))&SubstanceUTF8_Wrapper::default_ValidateArray)
        .def("ValueAt", (int (SplitVector_1::*)(int) )&SplitVector_1::ValueAt, (int (SubstanceUTF8_Wrapper::*)(int))&SubstanceUTF8_Wrapper::default_ValueAt)
        .def("FindNextChange", (int (SplitVector_1::*)(int, int) )&SplitVector_1::FindNextChange, (int (SubstanceUTF8_Wrapper::*)(int, int))&SubstanceUTF8_Wrapper::default_FindNextChange)
        .def("Length", (int (SplitVector_1::*)() )&SplitVector_1::Length, (int (SubstanceUTF8_Wrapper::*)())&SubstanceUTF8_Wrapper::default_Length)
        .def("InsertSpace", (void (SplitVector_1::*)(int, int) )&SplitVector_1::InsertSpace, (void (SubstanceUTF8_Wrapper::*)(int, int))&SubstanceUTF8_Wrapper::default_InsertSpace)
        .def("FillRange", (void (SplitVector_1::*)(int, int, int) )&SplitVector_1::FillRange, (void (SubstanceUTF8_Wrapper::*)(int, int, int))&SubstanceUTF8_Wrapper::default_FillRange)
        .def("DeleteRange", (void (SplitVector_1::*)(int, int) )&SplitVector_1::DeleteRange, (void (SubstanceUTF8_Wrapper::*)(int, int))&SubstanceUTF8_Wrapper::default_DeleteRange)
        .def("DeleteAll", (void (SplitVector_1::*)() )&SplitVector_1::DeleteAll, (void (SubstanceUTF8_Wrapper::*)())&SubstanceUTF8_Wrapper::default_DeleteAll)
    ;

}
