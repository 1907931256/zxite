#include <boost/python.hpp>
#include <boost/cstdint.hpp>

#include <string>
#include <iostream>
#include <assert.h>
#include "base.h"

// Using =======================================================================
using namespace boost::python;

#include "bbase.h"

void add_SubstanceByte() {
    class_< SubstanceByte, bases< Substance > , boost::noncopyable, SubstanceByte_Wrapper >("SubstanceByte", init<  >())
        .def("DistanceNextCaret", (int (SubstanceByte::*)(int) )&SubstanceByte::DistanceNextCaret, (int (SubstanceByte_Wrapper::*)(int))&SubstanceByte_Wrapper::default_DistanceNextCaret)
        .def("LenChar", (int (SubstanceByte::*)(int) )&SubstanceByte::LenChar, (int (SubstanceByte_Wrapper::*)(int))&SubstanceByte_Wrapper::default_LenChar)
        .def("LenCharacters", (int (SubstanceByte::*)(int*, int) )&SubstanceByte::LenCharacters, (int (SubstanceByte_Wrapper::*)(int*, int))&SubstanceByte_Wrapper::default_LenCharacters)
        .def("BytesFromCharacters", (int (SubstanceByte::*)(unsigned char*, int, int*, int) )&SubstanceByte::BytesFromCharacters, (int (SubstanceByte_Wrapper::*)(unsigned char*, int, int*, int))&SubstanceByte_Wrapper::default_BytesFromCharacters)
        .def("RetrieveUTF8", (int (SubstanceByte::*)(int, unsigned char*, int) )&SubstanceByte::RetrieveUTF8, (int (SubstanceByte_Wrapper::*)(int, unsigned char*, int))&SubstanceByte_Wrapper::default_RetrieveUTF8)
        .def("RetrieveUTF16", (int (SubstanceByte::*)(int, short int*, int) )&SubstanceByte::RetrieveUTF16, (int (SubstanceByte_Wrapper::*)(int, short int*, int))&SubstanceByte_Wrapper::default_RetrieveUTF16)
        .def("RetrieveUTF32", (int (SubstanceByte::*)(int, int*, int) )&SubstanceByte::RetrieveUTF32, (int (SubstanceByte_Wrapper::*)(int, int*, int))&SubstanceByte_Wrapper::default_RetrieveUTF32)
        .def("LengthInText", (int (SubstanceByte::*)(int*, int) )&SubstanceByte::LengthInText, (int (SubstanceByte_Wrapper::*)(int*, int))&SubstanceByte_Wrapper::default_LengthInText)
        .def("UnitValue", (int (SubstanceByte::*)(int) )&SubstanceByte::UnitValue, (int (SubstanceByte_Wrapper::*)(int))&SubstanceByte_Wrapper::default_UnitValue)
        .def("UnitFromArray", (int (SubstanceByte::*)(unsigned char*, int) )&SubstanceByte::UnitFromArray, (int (SubstanceByte_Wrapper::*)(unsigned char*, int))&SubstanceByte_Wrapper::default_UnitFromArray)
        .def("CharValue", (int (SubstanceByte::*)(int) )&SubstanceByte::CharValue, (int (SubstanceByte_Wrapper::*)(int))&SubstanceByte_Wrapper::default_CharValue)
        .def("MovePositionOutsideChar", (int (SubstanceByte::*)(int, int, bool) )&SubstanceByte::MovePositionOutsideChar, (int (SubstanceByte_Wrapper::*)(int, int, bool))&SubstanceByte_Wrapper::default_MovePositionOutsideChar)
        .def("BetweenCharacters", (bool (SubstanceByte::*)(int) )&SubstanceByte::BetweenCharacters, (bool (SubstanceByte_Wrapper::*)(int))&SubstanceByte_Wrapper::default_BetweenCharacters)
        .def("ValidateRange", (bool (SubstanceByte::*)(int, int) )&SubstanceByte::ValidateRange, (bool (SubstanceByte_Wrapper::*)(int, int))&SubstanceByte_Wrapper::default_ValidateRange)
        .def("ValidateArray", (bool (SubstanceByte::*)(unsigned char*, int) )&SubstanceByte::ValidateArray, (bool (SubstanceByte_Wrapper::*)(unsigned char*, int))&SubstanceByte_Wrapper::default_ValidateArray)
        .def("ValueAt", (int (SplitVector_1::*)(int) )&SplitVector_1::ValueAt, (int (SubstanceByte_Wrapper::*)(int))&SubstanceByte_Wrapper::default_ValueAt)
        .def("FindNextChange", (int (SplitVector_1::*)(int, int) )&SplitVector_1::FindNextChange, (int (SubstanceByte_Wrapper::*)(int, int))&SubstanceByte_Wrapper::default_FindNextChange)
        .def("Length", (int (SplitVector_1::*)() )&SplitVector_1::Length, (int (SubstanceByte_Wrapper::*)())&SubstanceByte_Wrapper::default_Length)
        .def("InsertSpace", (void (SplitVector_1::*)(int, int) )&SplitVector_1::InsertSpace, (void (SubstanceByte_Wrapper::*)(int, int))&SubstanceByte_Wrapper::default_InsertSpace)
        .def("FillRange", (void (SplitVector_1::*)(int, int, int) )&SplitVector_1::FillRange, (void (SubstanceByte_Wrapper::*)(int, int, int))&SubstanceByte_Wrapper::default_FillRange)
        .def("DeleteRange", (void (SplitVector_1::*)(int, int) )&SplitVector_1::DeleteRange, (void (SubstanceByte_Wrapper::*)(int, int))&SubstanceByte_Wrapper::default_DeleteRange)
        .def("DeleteAll", (void (SplitVector_1::*)() )&SplitVector_1::DeleteAll, (void (SubstanceByte_Wrapper::*)())&SubstanceByte_Wrapper::default_DeleteAll)
    ;

}
