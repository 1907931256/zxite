#include <boost/python.hpp>
#include <boost/cstdint.hpp>

#include <string>
#include <iostream>
#include <assert.h>
#include "base.h"

// Using =======================================================================
using namespace boost::python;

#include "bbase.h"

void add_SubstanceUTF16() {
    class_< SubstanceUTF16, bases< Substance > , boost::noncopyable, SubstanceUTF16_Wrapper >("SubstanceUTF16", init<  >())
        .def("DistanceNextCaret", (int (SubstanceUTF16::*)(int) )&SubstanceUTF16::DistanceNextCaret, (int (SubstanceUTF16_Wrapper::*)(int))&SubstanceUTF16_Wrapper::default_DistanceNextCaret)
        .def("LenChar", (int (SubstanceUTF16::*)(int) )&SubstanceUTF16::LenChar, (int (SubstanceUTF16_Wrapper::*)(int))&SubstanceUTF16_Wrapper::default_LenChar)
        .def("LenCharacters", (int (SubstanceUTF16::*)(int*, int) )&SubstanceUTF16::LenCharacters, (int (SubstanceUTF16_Wrapper::*)(int*, int))&SubstanceUTF16_Wrapper::default_LenCharacters)
        .def("RetrieveUTF8", (int (SubstanceUTF16::*)(int, unsigned char*, int) )&SubstanceUTF16::RetrieveUTF8, (int (SubstanceUTF16_Wrapper::*)(int, unsigned char*, int))&SubstanceUTF16_Wrapper::default_RetrieveUTF8)
        .def("RetrieveUTF16", (int (SubstanceUTF16::*)(int, short int*, int) )&SubstanceUTF16::RetrieveUTF16, (int (SubstanceUTF16_Wrapper::*)(int, short int*, int))&SubstanceUTF16_Wrapper::default_RetrieveUTF16)
        .def("RetrieveUTF32", (int (SubstanceUTF16::*)(int, int*, int) )&SubstanceUTF16::RetrieveUTF32, (int (SubstanceUTF16_Wrapper::*)(int, int*, int))&SubstanceUTF16_Wrapper::default_RetrieveUTF32)
        .def("LengthInText", (int (SubstanceUTF16::*)(int*, int) )&SubstanceUTF16::LengthInText, (int (SubstanceUTF16_Wrapper::*)(int*, int))&SubstanceUTF16_Wrapper::default_LengthInText)
        .def("CharValue", (int (SubstanceUTF16::*)(int) )&SubstanceUTF16::CharValue, (int (SubstanceUTF16_Wrapper::*)(int))&SubstanceUTF16_Wrapper::default_CharValue)
        .def("MovePositionOutsideChar", (int (SubstanceUTF16::*)(int, int, bool) )&SubstanceUTF16::MovePositionOutsideChar, (int (SubstanceUTF16_Wrapper::*)(int, int, bool))&SubstanceUTF16_Wrapper::default_MovePositionOutsideChar)
        .def("BetweenCharacters", (bool (SubstanceUTF16::*)(int) )&SubstanceUTF16::BetweenCharacters, (bool (SubstanceUTF16_Wrapper::*)(int))&SubstanceUTF16_Wrapper::default_BetweenCharacters)
        .def("ValidateRange", (bool (SubstanceUTF16::*)(int, int) )&SubstanceUTF16::ValidateRange, (bool (SubstanceUTF16_Wrapper::*)(int, int))&SubstanceUTF16_Wrapper::default_ValidateRange)
        .def("ValidateArray", (bool (SubstanceUTF16::*)(unsigned char*, int) )&SubstanceUTF16::ValidateArray, (bool (SubstanceUTF16_Wrapper::*)(unsigned char*, int))&SubstanceUTF16_Wrapper::default_ValidateArray)
        .def("BytesFromCharacters", pure_virtual(&Substance::BytesFromCharacters))
        .def("UnitValue", pure_virtual(&Substance::UnitValue))
        .def("UnitFromArray", pure_virtual(&Substance::UnitFromArray))
        .def("ValueAt", (int (SplitVector_1::*)(int) )&SplitVector_1::ValueAt, (int (SubstanceUTF16_Wrapper::*)(int))&SubstanceUTF16_Wrapper::default_ValueAt)
        .def("FindNextChange", (int (SplitVector_1::*)(int, int) )&SplitVector_1::FindNextChange, (int (SubstanceUTF16_Wrapper::*)(int, int))&SubstanceUTF16_Wrapper::default_FindNextChange)
        .def("Length", (int (SplitVector_1::*)() )&SplitVector_1::Length, (int (SubstanceUTF16_Wrapper::*)())&SubstanceUTF16_Wrapper::default_Length)
        .def("InsertSpace", (void (SplitVector_1::*)(int, int) )&SplitVector_1::InsertSpace, (void (SubstanceUTF16_Wrapper::*)(int, int))&SubstanceUTF16_Wrapper::default_InsertSpace)
        .def("FillRange", (void (SplitVector_1::*)(int, int, int) )&SplitVector_1::FillRange, (void (SubstanceUTF16_Wrapper::*)(int, int, int))&SubstanceUTF16_Wrapper::default_FillRange)
        .def("DeleteRange", (void (SplitVector_1::*)(int, int) )&SplitVector_1::DeleteRange, (void (SubstanceUTF16_Wrapper::*)(int, int))&SubstanceUTF16_Wrapper::default_DeleteRange)
        .def("DeleteAll", (void (SplitVector_1::*)() )&SplitVector_1::DeleteAll, (void (SubstanceUTF16_Wrapper::*)())&SubstanceUTF16_Wrapper::default_DeleteAll)
    ;

}
