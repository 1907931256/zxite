// Declarations ================================================================
namespace  {

struct IStyles_Wrapper: IStyles
{
    int Length() {
        return call_method< int >(py_self, "Length");
    }

    int ValueAt(int p0) {
        return call_method< int >(py_self, "ValueAt", p0);
    }

    int FindNextChange(int p0, int p1) {
        return call_method< int >(py_self, "FindNextChange", p0, p1);
    }

    void FillRange(int p0, int p1, int p2) {
        call_method< void >(py_self, "FillRange", p0, p1, p2);
    }

    void InsertSpace(int p0, int p1) {
        call_method< void >(py_self, "InsertSpace", p0, p1);
    }

    void DeleteAll() {
        call_method< void >(py_self, "DeleteAll");
    }

    void DeleteRange(int p0, int p1) {
        call_method< void >(py_self, "DeleteRange", p0, p1);
    }

    PyObject* py_self;
};

struct SinglePosition_Wrapper: SinglePosition
{
    SinglePosition_Wrapper(PyObject* py_self_):
        SinglePosition(), py_self(py_self_) {}

    int Length() {
        return call_method< int >(py_self, "Length");
    }

    int default_Length() {
        return SinglePosition::Length();
    }

    int ValueAt(int p0) {
        return call_method< int >(py_self, "ValueAt", p0);
    }

    int default_ValueAt(int p0) {
        return SinglePosition::ValueAt(p0);
    }

    int FindNextChange(int p0, int p1) {
        return call_method< int >(py_self, "FindNextChange", p0, p1);
    }

    int default_FindNextChange(int p0, int p1) {
        return SinglePosition::FindNextChange(p0, p1);
    }

    void FillRange(int p0, int p1, int p2) {
        call_method< void >(py_self, "FillRange", p0, p1, p2);
    }

    void default_FillRange(int p0, int p1, int p2) {
        SinglePosition::FillRange(p0, p1, p2);
    }

    void InsertSpace(int p0, int p1) {
        call_method< void >(py_self, "InsertSpace", p0, p1);
    }

    void default_InsertSpace(int p0, int p1) {
        SinglePosition::InsertSpace(p0, p1);
    }

    void DeleteAll() {
        call_method< void >(py_self, "DeleteAll");
    }

    void default_DeleteAll() {
        SinglePosition::DeleteAll();
    }

    void DeleteRange(int p0, int p1) {
        call_method< void >(py_self, "DeleteRange", p0, p1);
    }

    void default_DeleteRange(int p0, int p1) {
        SinglePosition::DeleteRange(p0, p1);
    }

    PyObject* py_self;
};

struct SplitVector_0_Wrapper: SplitVector_0
{
    SplitVector_0_Wrapper(PyObject* py_self_):
        SplitVector_0(), py_self(py_self_) {}

    int Length() {
        return call_method< int >(py_self, "Length");
    }

    int default_Length() {
        return SplitVector_0::Length();
    }

    int ValueAt(int p0) {
        return call_method< int >(py_self, "ValueAt", p0);
    }

    int default_ValueAt(int p0) {
        return SplitVector_0::ValueAt(p0);
    }

    int FindNextChange(int p0, int p1) {
        return call_method< int >(py_self, "FindNextChange", p0, p1);
    }

    int default_FindNextChange(int p0, int p1) {
        return SplitVector_0::FindNextChange(p0, p1);
    }

    void FillRange(int p0, int p1, int p2) {
        call_method< void >(py_self, "FillRange", p0, p1, p2);
    }

    void default_FillRange(int p0, int p1, int p2) {
        SplitVector_0::FillRange(p0, p1, p2);
    }

    void InsertSpace(int p0, int p1) {
        call_method< void >(py_self, "InsertSpace", p0, p1);
    }

    void default_InsertSpace(int p0, int p1) {
        SplitVector_0::InsertSpace(p0, p1);
    }

    void DeleteAll() {
        call_method< void >(py_self, "DeleteAll");
    }

    void default_DeleteAll() {
        SplitVector_0::DeleteAll();
    }

    void DeleteRange(int p0, int p1) {
        call_method< void >(py_self, "DeleteRange", p0, p1);
    }

    void default_DeleteRange(int p0, int p1) {
        SplitVector_0::DeleteRange(p0, p1);
    }

    PyObject* py_self;
};

struct SplitVector_1_Wrapper: SplitVector_1
{
    SplitVector_1_Wrapper(PyObject* py_self_):
        SplitVector_1(), py_self(py_self_) {}

    int ValueAt(int p0) {
        return call_method< int >(py_self, "ValueAt", p0);
    }

    int default_ValueAt(int p0) {
        return SplitVector_1::ValueAt(p0);
    }

    int FindNextChange(int p0, int p1) {
        return call_method< int >(py_self, "FindNextChange", p0, p1);
    }

    int default_FindNextChange(int p0, int p1) {
        return SplitVector_1::FindNextChange(p0, p1);
    }

    int Length() {
        return call_method< int >(py_self, "Length");
    }

    int default_Length() {
        return SplitVector_1::Length();
    }

    void InsertSpace(int p0, int p1) {
        call_method< void >(py_self, "InsertSpace", p0, p1);
    }

    void default_InsertSpace(int p0, int p1) {
        SplitVector_1::InsertSpace(p0, p1);
    }

    void FillRange(int p0, int p1, int p2) {
        call_method< void >(py_self, "FillRange", p0, p1, p2);
    }

    void default_FillRange(int p0, int p1, int p2) {
        SplitVector_1::FillRange(p0, p1, p2);
    }

    void DeleteRange(int p0, int p1) {
        call_method< void >(py_self, "DeleteRange", p0, p1);
    }

    void default_DeleteRange(int p0, int p1) {
        SplitVector_1::DeleteRange(p0, p1);
    }

    void DeleteAll() {
        call_method< void >(py_self, "DeleteAll");
    }

    void default_DeleteAll() {
        SplitVector_1::DeleteAll();
    }

    PyObject* py_self;
};

struct SplitVector_2_Wrapper: SplitVector_2
{
    SplitVector_2_Wrapper(PyObject* py_self_):
        SplitVector_2(), py_self(py_self_) {}

    int ValueAt(int p0) {
        return call_method< int >(py_self, "ValueAt", p0);
    }

    int default_ValueAt(int p0) {
        return SplitVector_2::ValueAt(p0);
    }

    int FindNextChange(int p0, int p1) {
        return call_method< int >(py_self, "FindNextChange", p0, p1);
    }

    int default_FindNextChange(int p0, int p1) {
        return SplitVector_2::FindNextChange(p0, p1);
    }

    int Length() {
        return call_method< int >(py_self, "Length");
    }

    int default_Length() {
        return SplitVector_2::Length();
    }

    void InsertSpace(int p0, int p1) {
        call_method< void >(py_self, "InsertSpace", p0, p1);
    }

    void default_InsertSpace(int p0, int p1) {
        SplitVector_2::InsertSpace(p0, p1);
    }

    void FillRange(int p0, int p1, int p2) {
        call_method< void >(py_self, "FillRange", p0, p1, p2);
    }

    void default_FillRange(int p0, int p1, int p2) {
        SplitVector_2::FillRange(p0, p1, p2);
    }

    void DeleteRange(int p0, int p1) {
        call_method< void >(py_self, "DeleteRange", p0, p1);
    }

    void default_DeleteRange(int p0, int p1) {
        SplitVector_2::DeleteRange(p0, p1);
    }

    void DeleteAll() {
        call_method< void >(py_self, "DeleteAll");
    }

    void default_DeleteAll() {
        SplitVector_2::DeleteAll();
    }

    PyObject* py_self;
};

struct SplitVector_4_Wrapper: SplitVector_4
{
    SplitVector_4_Wrapper(PyObject* py_self_):
        SplitVector_4(), py_self(py_self_) {}

    int ValueAt(int p0) {
        return call_method< int >(py_self, "ValueAt", p0);
    }

    int default_ValueAt(int p0) {
        return SplitVector_4::ValueAt(p0);
    }

    int FindNextChange(int p0, int p1) {
        return call_method< int >(py_self, "FindNextChange", p0, p1);
    }

    int default_FindNextChange(int p0, int p1) {
        return SplitVector_4::FindNextChange(p0, p1);
    }

    int Length() {
        return call_method< int >(py_self, "Length");
    }

    int default_Length() {
        return SplitVector_4::Length();
    }

    void InsertSpace(int p0, int p1) {
        call_method< void >(py_self, "InsertSpace", p0, p1);
    }

    void default_InsertSpace(int p0, int p1) {
        SplitVector_4::InsertSpace(p0, p1);
    }

    void FillRange(int p0, int p1, int p2) {
        call_method< void >(py_self, "FillRange", p0, p1, p2);
    }

    void default_FillRange(int p0, int p1, int p2) {
        SplitVector_4::FillRange(p0, p1, p2);
    }

    void DeleteRange(int p0, int p1) {
        call_method< void >(py_self, "DeleteRange", p0, p1);
    }

    void default_DeleteRange(int p0, int p1) {
        SplitVector_4::DeleteRange(p0, p1);
    }

    void DeleteAll() {
        call_method< void >(py_self, "DeleteAll");
    }

    void default_DeleteAll() {
        SplitVector_4::DeleteAll();
    }

    PyObject* py_self;
};

struct Partitioning_Wrapper: Partitioning
{
    Partitioning_Wrapper(PyObject* py_self_, int p0):
        Partitioning(p0), py_self(py_self_) {}

    int Length() {
        return call_method< int >(py_self, "Length");
    }

    int default_Length() {
        return Partitioning::Length();
    }

    void InsertSpace(int p0, int p1) {
        call_method< void >(py_self, "InsertSpace", p0, p1);
    }

    void default_InsertSpace(int p0, int p1) {
        Partitioning::InsertSpace(p0, p1);
    }

    void DeleteRange(int p0, int p1) {
        call_method< void >(py_self, "DeleteRange", p0, p1);
    }

    void default_DeleteRange(int p0, int p1) {
        Partitioning::DeleteRange(p0, p1);
    }

    void DeleteAll() {
        call_method< void >(py_self, "DeleteAll");
    }

    void default_DeleteAll() {
        Partitioning::DeleteAll();
    }

    int ValueAt(int p0) {
        return call_method< int >(py_self, "ValueAt", p0);
    }

    int default_ValueAt(int p0) {
        return Partitioning::ValueAt(p0);
    }

    int FindNextChange(int p0, int p1) {
        return call_method< int >(py_self, "FindNextChange", p0, p1);
    }

    int default_FindNextChange(int p0, int p1) {
        return Partitioning::FindNextChange(p0, p1);
    }

    void FillRange(int p0, int p1, int p2) {
        call_method< void >(py_self, "FillRange", p0, p1, p2);
    }

    void default_FillRange(int p0, int p1, int p2) {
        Partitioning::FillRange(p0, p1, p2);
    }

    PyObject* py_self;
};

struct LineData_Wrapper: LineData
{
    LineData_Wrapper(PyObject* py_self_):
        LineData(), py_self(py_self_) {}

    void Clear() {
        call_method< void >(py_self, "Clear");
    }

    void InsertLine(int p0) {
        call_method< void >(py_self, "InsertLine", p0);
    }

    void RemoveLine(int p0) {
        call_method< void >(py_self, "RemoveLine", p0);
    }

    PyObject* py_self;
};

struct RunStyles_Wrapper: RunStyles
{
    RunStyles_Wrapper(PyObject* py_self_):
        RunStyles(), py_self(py_self_) {}

    int Length() {
        return call_method< int >(py_self, "Length");
    }

    int default_Length() {
        return RunStyles::Length();
    }

    int ValueAt(int p0) {
        return call_method< int >(py_self, "ValueAt", p0);
    }

    int default_ValueAt(int p0) {
        return RunStyles::ValueAt(p0);
    }

    int FindNextChange(int p0, int p1) {
        return call_method< int >(py_self, "FindNextChange", p0, p1);
    }

    int default_FindNextChange(int p0, int p1) {
        return RunStyles::FindNextChange(p0, p1);
    }

    void FillRange(int p0, int p1, int p2) {
        call_method< void >(py_self, "FillRange", p0, p1, p2);
    }

    void default_FillRange(int p0, int p1, int p2) {
        RunStyles::FillRange(p0, p1, p2);
    }

    void InsertSpace(int p0, int p1) {
        call_method< void >(py_self, "InsertSpace", p0, p1);
    }

    void default_InsertSpace(int p0, int p1) {
        RunStyles::InsertSpace(p0, p1);
    }

    void DeleteAll() {
        call_method< void >(py_self, "DeleteAll");
    }

    void default_DeleteAll() {
        RunStyles::DeleteAll();
    }

    void DeleteRange(int p0, int p1) {
        call_method< void >(py_self, "DeleteRange", p0, p1);
    }

    void default_DeleteRange(int p0, int p1) {
        RunStyles::DeleteRange(p0, p1);
    }

    PyObject* py_self;
};

struct Substance_Wrapper: Substance
{
    Substance_Wrapper(PyObject* py_self_):
        Substance(), py_self(py_self_) {}

    int DistanceNextCaret(int p0) {
        return call_method< int >(py_self, "DistanceNextCaret", p0);
    }

    int LenChar(int p0) {
        return call_method< int >(py_self, "LenChar", p0);
    }

    int LenCharacters(const int* p0, int p1) {
        return call_method< int >(py_self, "LenCharacters", p0, p1);
    }

    int BytesFromCharacters(unsigned char* p0, int p1, const int* p2, int p3) {
        return call_method< int >(py_self, "BytesFromCharacters", p0, p1, p2, p3);
    }

    int RetrieveUTF8(int p0, unsigned char* p1, int p2) {
        return call_method< int >(py_self, "RetrieveUTF8", p0, p1, p2);
    }

    int RetrieveUTF16(int p0, short int* p1, int p2) {
        return call_method< int >(py_self, "RetrieveUTF16", p0, p1, p2);
    }

    int RetrieveUTF32(int p0, int* p1, int p2) {
        return call_method< int >(py_self, "RetrieveUTF32", p0, p1, p2);
    }

    int LengthInText(const int* p0, int p1) {
        return call_method< int >(py_self, "LengthInText", p0, p1);
    }

    int UnitValue(int p0) {
        return call_method< int >(py_self, "UnitValue", p0);
    }

    int UnitFromArray(unsigned char* p0, int p1) {
        return call_method< int >(py_self, "UnitFromArray", p0, p1);
    }

    int CharValue(int p0) {
        return call_method< int >(py_self, "CharValue", p0);
    }

    int MovePositionOutsideChar(int p0, int p1, bool p2) {
        return call_method< int >(py_self, "MovePositionOutsideChar", p0, p1, p2);
    }

    bool BetweenCharacters(int p0) {
        return call_method< bool >(py_self, "BetweenCharacters", p0);
    }

    bool ValidateRange(int p0, int p1) {
        return call_method< bool >(py_self, "ValidateRange", p0, p1);
    }

    bool ValidateArray(unsigned char* p0, int p1) {
        return call_method< bool >(py_self, "ValidateArray", p0, p1);
    }

    int ValueAt(int p0) {
        return call_method< int >(py_self, "ValueAt", p0);
    }

    int default_ValueAt(int p0) {
        return SplitVector_1::ValueAt(p0);
    }

    int FindNextChange(int p0, int p1) {
        return call_method< int >(py_self, "FindNextChange", p0, p1);
    }

    int default_FindNextChange(int p0, int p1) {
        return SplitVector_1::FindNextChange(p0, p1);
    }

    int Length() {
        return call_method< int >(py_self, "Length");
    }

    int default_Length() {
        return SplitVector_1::Length();
    }

    void InsertSpace(int p0, int p1) {
        call_method< void >(py_self, "InsertSpace", p0, p1);
    }

    void default_InsertSpace(int p0, int p1) {
        SplitVector_1::InsertSpace(p0, p1);
    }

    void FillRange(int p0, int p1, int p2) {
        call_method< void >(py_self, "FillRange", p0, p1, p2);
    }

    void default_FillRange(int p0, int p1, int p2) {
        SplitVector_1::FillRange(p0, p1, p2);
    }

    void DeleteRange(int p0, int p1) {
        call_method< void >(py_self, "DeleteRange", p0, p1);
    }

    void default_DeleteRange(int p0, int p1) {
        SplitVector_1::DeleteRange(p0, p1);
    }

    void DeleteAll() {
        call_method< void >(py_self, "DeleteAll");
    }

    void default_DeleteAll() {
        SplitVector_1::DeleteAll();
    }

    PyObject* py_self;
};

struct SubstanceUTF8_Wrapper: SubstanceUTF8
{
    SubstanceUTF8_Wrapper(PyObject* py_self_):
        SubstanceUTF8(), py_self(py_self_) {}

    int DistanceNextCaret(int p0) {
        return call_method< int >(py_self, "DistanceNextCaret", p0);
    }

    int default_DistanceNextCaret(int p0) {
        return SubstanceUTF8::DistanceNextCaret(p0);
    }

    int LenChar(int p0) {
        return call_method< int >(py_self, "LenChar", p0);
    }

    int default_LenChar(int p0) {
        return SubstanceUTF8::LenChar(p0);
    }

    int LenCharacters(const int* p0, int p1) {
        return call_method< int >(py_self, "LenCharacters", p0, p1);
    }

    int default_LenCharacters(const int* p0, int p1) {
        return SubstanceUTF8::LenCharacters(p0, p1);
    }

    int BytesFromCharacters(unsigned char* p0, int p1, const int* p2, int p3) {
        return call_method< int >(py_self, "BytesFromCharacters", p0, p1, p2, p3);
    }

    int default_BytesFromCharacters(unsigned char* p0, int p1, const int* p2, int p3) {
        return SubstanceUTF8::BytesFromCharacters(p0, p1, p2, p3);
    }

    int RetrieveUTF8(int p0, unsigned char* p1, int p2) {
        return call_method< int >(py_self, "RetrieveUTF8", p0, p1, p2);
    }

    int default_RetrieveUTF8(int p0, unsigned char* p1, int p2) {
        return SubstanceUTF8::RetrieveUTF8(p0, p1, p2);
    }

    int RetrieveUTF16(int p0, short int* p1, int p2) {
        return call_method< int >(py_self, "RetrieveUTF16", p0, p1, p2);
    }

    int default_RetrieveUTF16(int p0, short int* p1, int p2) {
        return SubstanceUTF8::RetrieveUTF16(p0, p1, p2);
    }

    int RetrieveUTF32(int p0, int* p1, int p2) {
        return call_method< int >(py_self, "RetrieveUTF32", p0, p1, p2);
    }

    int default_RetrieveUTF32(int p0, int* p1, int p2) {
        return SubstanceUTF8::RetrieveUTF32(p0, p1, p2);
    }

    int LengthInText(const int* p0, int p1) {
        return call_method< int >(py_self, "LengthInText", p0, p1);
    }

    int default_LengthInText(const int* p0, int p1) {
        return SubstanceUTF8::LengthInText(p0, p1);
    }

    int UnitValue(int p0) {
        return call_method< int >(py_self, "UnitValue", p0);
    }

    int default_UnitValue(int p0) {
        return SubstanceUTF8::UnitValue(p0);
    }

    int UnitFromArray(unsigned char* p0, int p1) {
        return call_method< int >(py_self, "UnitFromArray", p0, p1);
    }

    int default_UnitFromArray(unsigned char* p0, int p1) {
        return SubstanceUTF8::UnitFromArray(p0, p1);
    }

    int CharValue(int p0) {
        return call_method< int >(py_self, "CharValue", p0);
    }

    int default_CharValue(int p0) {
        return SubstanceUTF8::CharValue(p0);
    }

    int MovePositionOutsideChar(int p0, int p1, bool p2) {
        return call_method< int >(py_self, "MovePositionOutsideChar", p0, p1, p2);
    }

    int default_MovePositionOutsideChar(int p0, int p1, bool p2) {
        return SubstanceUTF8::MovePositionOutsideChar(p0, p1, p2);
    }

    bool BetweenCharacters(int p0) {
        return call_method< bool >(py_self, "BetweenCharacters", p0);
    }

    bool default_BetweenCharacters(int p0) {
        return SubstanceUTF8::BetweenCharacters(p0);
    }

    bool ValidateRange(int p0, int p1) {
        return call_method< bool >(py_self, "ValidateRange", p0, p1);
    }

    bool default_ValidateRange(int p0, int p1) {
        return SubstanceUTF8::ValidateRange(p0, p1);
    }

    bool ValidateArray(unsigned char* p0, int p1) {
        return call_method< bool >(py_self, "ValidateArray", p0, p1);
    }

    bool default_ValidateArray(unsigned char* p0, int p1) {
        return SubstanceUTF8::ValidateArray(p0, p1);
    }

    int ValueAt(int p0) {
        return call_method< int >(py_self, "ValueAt", p0);
    }

    int default_ValueAt(int p0) {
        return SplitVector_1::ValueAt(p0);
    }

    int FindNextChange(int p0, int p1) {
        return call_method< int >(py_self, "FindNextChange", p0, p1);
    }

    int default_FindNextChange(int p0, int p1) {
        return SplitVector_1::FindNextChange(p0, p1);
    }

    int Length() {
        return call_method< int >(py_self, "Length");
    }

    int default_Length() {
        return SplitVector_1::Length();
    }

    void InsertSpace(int p0, int p1) {
        call_method< void >(py_self, "InsertSpace", p0, p1);
    }

    void default_InsertSpace(int p0, int p1) {
        SplitVector_1::InsertSpace(p0, p1);
    }

    void FillRange(int p0, int p1, int p2) {
        call_method< void >(py_self, "FillRange", p0, p1, p2);
    }

    void default_FillRange(int p0, int p1, int p2) {
        SplitVector_1::FillRange(p0, p1, p2);
    }

    void DeleteRange(int p0, int p1) {
        call_method< void >(py_self, "DeleteRange", p0, p1);
    }

    void default_DeleteRange(int p0, int p1) {
        SplitVector_1::DeleteRange(p0, p1);
    }

    void DeleteAll() {
        call_method< void >(py_self, "DeleteAll");
    }

    void default_DeleteAll() {
        SplitVector_1::DeleteAll();
    }

    PyObject* py_self;
};

struct SubstanceByte_Wrapper: SubstanceByte
{
    SubstanceByte_Wrapper(PyObject* py_self_):
        SubstanceByte(), py_self(py_self_) {}

    int DistanceNextCaret(int p0) {
        return call_method< int >(py_self, "DistanceNextCaret", p0);
    }

    int default_DistanceNextCaret(int p0) {
        return SubstanceByte::DistanceNextCaret(p0);
    }

    int LenChar(int p0) {
        return call_method< int >(py_self, "LenChar", p0);
    }

    int default_LenChar(int p0) {
        return SubstanceByte::LenChar(p0);
    }

    int LenCharacters(const int* p0, int p1) {
        return call_method< int >(py_self, "LenCharacters", p0, p1);
    }

    int default_LenCharacters(const int* p0, int p1) {
        return SubstanceByte::LenCharacters(p0, p1);
    }

    int BytesFromCharacters(unsigned char* p0, int p1, const int* p2, int p3) {
        return call_method< int >(py_self, "BytesFromCharacters", p0, p1, p2, p3);
    }

    int default_BytesFromCharacters(unsigned char* p0, int p1, const int* p2, int p3) {
        return SubstanceByte::BytesFromCharacters(p0, p1, p2, p3);
    }

    int RetrieveUTF8(int p0, unsigned char* p1, int p2) {
        return call_method< int >(py_self, "RetrieveUTF8", p0, p1, p2);
    }

    int default_RetrieveUTF8(int p0, unsigned char* p1, int p2) {
        return SubstanceByte::RetrieveUTF8(p0, p1, p2);
    }

    int RetrieveUTF16(int p0, short int* p1, int p2) {
        return call_method< int >(py_self, "RetrieveUTF16", p0, p1, p2);
    }

    int default_RetrieveUTF16(int p0, short int* p1, int p2) {
        return SubstanceByte::RetrieveUTF16(p0, p1, p2);
    }

    int RetrieveUTF32(int p0, int* p1, int p2) {
        return call_method< int >(py_self, "RetrieveUTF32", p0, p1, p2);
    }

    int default_RetrieveUTF32(int p0, int* p1, int p2) {
        return SubstanceByte::RetrieveUTF32(p0, p1, p2);
    }

    int LengthInText(const int* p0, int p1) {
        return call_method< int >(py_self, "LengthInText", p0, p1);
    }

    int default_LengthInText(const int* p0, int p1) {
        return SubstanceByte::LengthInText(p0, p1);
    }

    int UnitValue(int p0) {
        return call_method< int >(py_self, "UnitValue", p0);
    }

    int default_UnitValue(int p0) {
        return SubstanceByte::UnitValue(p0);
    }

    int UnitFromArray(unsigned char* p0, int p1) {
        return call_method< int >(py_self, "UnitFromArray", p0, p1);
    }

    int default_UnitFromArray(unsigned char* p0, int p1) {
        return SubstanceByte::UnitFromArray(p0, p1);
    }

    int CharValue(int p0) {
        return call_method< int >(py_self, "CharValue", p0);
    }

    int default_CharValue(int p0) {
        return SubstanceByte::CharValue(p0);
    }

    int MovePositionOutsideChar(int p0, int p1, bool p2) {
        return call_method< int >(py_self, "MovePositionOutsideChar", p0, p1, p2);
    }

    int default_MovePositionOutsideChar(int p0, int p1, bool p2) {
        return SubstanceByte::MovePositionOutsideChar(p0, p1, p2);
    }

    bool BetweenCharacters(int p0) {
        return call_method< bool >(py_self, "BetweenCharacters", p0);
    }

    bool default_BetweenCharacters(int p0) {
        return SubstanceByte::BetweenCharacters(p0);
    }

    bool ValidateRange(int p0, int p1) {
        return call_method< bool >(py_self, "ValidateRange", p0, p1);
    }

    bool default_ValidateRange(int p0, int p1) {
        return SubstanceByte::ValidateRange(p0, p1);
    }

    bool ValidateArray(unsigned char* p0, int p1) {
        return call_method< bool >(py_self, "ValidateArray", p0, p1);
    }

    bool default_ValidateArray(unsigned char* p0, int p1) {
        return SubstanceByte::ValidateArray(p0, p1);
    }

    int ValueAt(int p0) {
        return call_method< int >(py_self, "ValueAt", p0);
    }

    int default_ValueAt(int p0) {
        return SplitVector_1::ValueAt(p0);
    }

    int FindNextChange(int p0, int p1) {
        return call_method< int >(py_self, "FindNextChange", p0, p1);
    }

    int default_FindNextChange(int p0, int p1) {
        return SplitVector_1::FindNextChange(p0, p1);
    }

    int Length() {
        return call_method< int >(py_self, "Length");
    }

    int default_Length() {
        return SplitVector_1::Length();
    }

    void InsertSpace(int p0, int p1) {
        call_method< void >(py_self, "InsertSpace", p0, p1);
    }

    void default_InsertSpace(int p0, int p1) {
        SplitVector_1::InsertSpace(p0, p1);
    }

    void FillRange(int p0, int p1, int p2) {
        call_method< void >(py_self, "FillRange", p0, p1, p2);
    }

    void default_FillRange(int p0, int p1, int p2) {
        SplitVector_1::FillRange(p0, p1, p2);
    }

    void DeleteRange(int p0, int p1) {
        call_method< void >(py_self, "DeleteRange", p0, p1);
    }

    void default_DeleteRange(int p0, int p1) {
        SplitVector_1::DeleteRange(p0, p1);
    }

    void DeleteAll() {
        call_method< void >(py_self, "DeleteAll");
    }

    void default_DeleteAll() {
        SplitVector_1::DeleteAll();
    }

    PyObject* py_self;
};

struct SubstanceUTF16_Wrapper: SubstanceUTF16
{
    SubstanceUTF16_Wrapper(PyObject* py_self_):
        SubstanceUTF16(), py_self(py_self_) {}

    int DistanceNextCaret(int p0) {
        return call_method< int >(py_self, "DistanceNextCaret", p0);
    }

    int default_DistanceNextCaret(int p0) {
        return SubstanceUTF16::DistanceNextCaret(p0);
    }

    int LenChar(int p0) {
        return call_method< int >(py_self, "LenChar", p0);
    }

    int default_LenChar(int p0) {
        return SubstanceUTF16::LenChar(p0);
    }

    int LenCharacters(const int* p0, int p1) {
        return call_method< int >(py_self, "LenCharacters", p0, p1);
    }

    int default_LenCharacters(const int* p0, int p1) {
        return SubstanceUTF16::LenCharacters(p0, p1);
    }

    int RetrieveUTF8(int p0, unsigned char* p1, int p2) {
        return call_method< int >(py_self, "RetrieveUTF8", p0, p1, p2);
    }

    int default_RetrieveUTF8(int p0, unsigned char* p1, int p2) {
        return SubstanceUTF16::RetrieveUTF8(p0, p1, p2);
    }

    int RetrieveUTF16(int p0, short int* p1, int p2) {
        return call_method< int >(py_self, "RetrieveUTF16", p0, p1, p2);
    }

    int default_RetrieveUTF16(int p0, short int* p1, int p2) {
        return SubstanceUTF16::RetrieveUTF16(p0, p1, p2);
    }

    int RetrieveUTF32(int p0, int* p1, int p2) {
        return call_method< int >(py_self, "RetrieveUTF32", p0, p1, p2);
    }

    int default_RetrieveUTF32(int p0, int* p1, int p2) {
        return SubstanceUTF16::RetrieveUTF32(p0, p1, p2);
    }

    int LengthInText(const int* p0, int p1) {
        return call_method< int >(py_self, "LengthInText", p0, p1);
    }

    int default_LengthInText(const int* p0, int p1) {
        return SubstanceUTF16::LengthInText(p0, p1);
    }

    int CharValue(int p0) {
        return call_method< int >(py_self, "CharValue", p0);
    }

    int default_CharValue(int p0) {
        return SubstanceUTF16::CharValue(p0);
    }

    int MovePositionOutsideChar(int p0, int p1, bool p2) {
        return call_method< int >(py_self, "MovePositionOutsideChar", p0, p1, p2);
    }

    int default_MovePositionOutsideChar(int p0, int p1, bool p2) {
        return SubstanceUTF16::MovePositionOutsideChar(p0, p1, p2);
    }

    bool BetweenCharacters(int p0) {
        return call_method< bool >(py_self, "BetweenCharacters", p0);
    }

    bool default_BetweenCharacters(int p0) {
        return SubstanceUTF16::BetweenCharacters(p0);
    }

    bool ValidateRange(int p0, int p1) {
        return call_method< bool >(py_self, "ValidateRange", p0, p1);
    }

    bool default_ValidateRange(int p0, int p1) {
        return SubstanceUTF16::ValidateRange(p0, p1);
    }

    bool ValidateArray(unsigned char* p0, int p1) {
        return call_method< bool >(py_self, "ValidateArray", p0, p1);
    }

    bool default_ValidateArray(unsigned char* p0, int p1) {
        return SubstanceUTF16::ValidateArray(p0, p1);
    }

    int BytesFromCharacters(unsigned char* p0, int p1, const int* p2, int p3) {
        return call_method< int >(py_self, "BytesFromCharacters", p0, p1, p2, p3);
    }

    int UnitValue(int p0) {
        return call_method< int >(py_self, "UnitValue", p0);
    }

    int UnitFromArray(unsigned char* p0, int p1) {
        return call_method< int >(py_self, "UnitFromArray", p0, p1);
    }

    int ValueAt(int p0) {
        return call_method< int >(py_self, "ValueAt", p0);
    }

    int default_ValueAt(int p0) {
        return SplitVector_1::ValueAt(p0);
    }

    int FindNextChange(int p0, int p1) {
        return call_method< int >(py_self, "FindNextChange", p0, p1);
    }

    int default_FindNextChange(int p0, int p1) {
        return SplitVector_1::FindNextChange(p0, p1);
    }

    int Length() {
        return call_method< int >(py_self, "Length");
    }

    int default_Length() {
        return SplitVector_1::Length();
    }

    void InsertSpace(int p0, int p1) {
        call_method< void >(py_self, "InsertSpace", p0, p1);
    }

    void default_InsertSpace(int p0, int p1) {
        SplitVector_1::InsertSpace(p0, p1);
    }

    void FillRange(int p0, int p1, int p2) {
        call_method< void >(py_self, "FillRange", p0, p1, p2);
    }

    void default_FillRange(int p0, int p1, int p2) {
        SplitVector_1::FillRange(p0, p1, p2);
    }

    void DeleteRange(int p0, int p1) {
        call_method< void >(py_self, "DeleteRange", p0, p1);
    }

    void default_DeleteRange(int p0, int p1) {
        SplitVector_1::DeleteRange(p0, p1);
    }

    void DeleteAll() {
        call_method< void >(py_self, "DeleteAll");
    }

    void default_DeleteAll() {
        SplitVector_1::DeleteAll();
    }

    PyObject* py_self;
};

struct SubstanceUTF16LE_Wrapper: SubstanceUTF16LE
{
    SubstanceUTF16LE_Wrapper(PyObject* py_self_):
        SubstanceUTF16LE(), py_self(py_self_) {}

    int BytesFromCharacters(unsigned char* p0, int p1, const int* p2, int p3) {
        return call_method< int >(py_self, "BytesFromCharacters", p0, p1, p2, p3);
    }

    int default_BytesFromCharacters(unsigned char* p0, int p1, const int* p2, int p3) {
        return SubstanceUTF16LE::BytesFromCharacters(p0, p1, p2, p3);
    }

    int UnitValue(int p0) {
        return call_method< int >(py_self, "UnitValue", p0);
    }

    int default_UnitValue(int p0) {
        return SubstanceUTF16LE::UnitValue(p0);
    }

    int UnitFromArray(unsigned char* p0, int p1) {
        return call_method< int >(py_self, "UnitFromArray", p0, p1);
    }

    int default_UnitFromArray(unsigned char* p0, int p1) {
        return SubstanceUTF16LE::UnitFromArray(p0, p1);
    }

    int DistanceNextCaret(int p0) {
        return call_method< int >(py_self, "DistanceNextCaret", p0);
    }

    int default_DistanceNextCaret(int p0) {
        return SubstanceUTF16::DistanceNextCaret(p0);
    }

    int LenChar(int p0) {
        return call_method< int >(py_self, "LenChar", p0);
    }

    int default_LenChar(int p0) {
        return SubstanceUTF16::LenChar(p0);
    }

    int LenCharacters(const int* p0, int p1) {
        return call_method< int >(py_self, "LenCharacters", p0, p1);
    }

    int default_LenCharacters(const int* p0, int p1) {
        return SubstanceUTF16::LenCharacters(p0, p1);
    }

    int RetrieveUTF8(int p0, unsigned char* p1, int p2) {
        return call_method< int >(py_self, "RetrieveUTF8", p0, p1, p2);
    }

    int default_RetrieveUTF8(int p0, unsigned char* p1, int p2) {
        return SubstanceUTF16::RetrieveUTF8(p0, p1, p2);
    }

    int RetrieveUTF16(int p0, short int* p1, int p2) {
        return call_method< int >(py_self, "RetrieveUTF16", p0, p1, p2);
    }

    int default_RetrieveUTF16(int p0, short int* p1, int p2) {
        return SubstanceUTF16::RetrieveUTF16(p0, p1, p2);
    }

    int RetrieveUTF32(int p0, int* p1, int p2) {
        return call_method< int >(py_self, "RetrieveUTF32", p0, p1, p2);
    }

    int default_RetrieveUTF32(int p0, int* p1, int p2) {
        return SubstanceUTF16::RetrieveUTF32(p0, p1, p2);
    }

    int LengthInText(const int* p0, int p1) {
        return call_method< int >(py_self, "LengthInText", p0, p1);
    }

    int default_LengthInText(const int* p0, int p1) {
        return SubstanceUTF16::LengthInText(p0, p1);
    }

    int CharValue(int p0) {
        return call_method< int >(py_self, "CharValue", p0);
    }

    int default_CharValue(int p0) {
        return SubstanceUTF16::CharValue(p0);
    }

    int MovePositionOutsideChar(int p0, int p1, bool p2) {
        return call_method< int >(py_self, "MovePositionOutsideChar", p0, p1, p2);
    }

    int default_MovePositionOutsideChar(int p0, int p1, bool p2) {
        return SubstanceUTF16::MovePositionOutsideChar(p0, p1, p2);
    }

    bool BetweenCharacters(int p0) {
        return call_method< bool >(py_self, "BetweenCharacters", p0);
    }

    bool default_BetweenCharacters(int p0) {
        return SubstanceUTF16::BetweenCharacters(p0);
    }

    bool ValidateRange(int p0, int p1) {
        return call_method< bool >(py_self, "ValidateRange", p0, p1);
    }

    bool default_ValidateRange(int p0, int p1) {
        return SubstanceUTF16::ValidateRange(p0, p1);
    }

    bool ValidateArray(unsigned char* p0, int p1) {
        return call_method< bool >(py_self, "ValidateArray", p0, p1);
    }

    bool default_ValidateArray(unsigned char* p0, int p1) {
        return SubstanceUTF16::ValidateArray(p0, p1);
    }

    int ValueAt(int p0) {
        return call_method< int >(py_self, "ValueAt", p0);
    }

    int default_ValueAt(int p0) {
        return SplitVector_1::ValueAt(p0);
    }

    int FindNextChange(int p0, int p1) {
        return call_method< int >(py_self, "FindNextChange", p0, p1);
    }

    int default_FindNextChange(int p0, int p1) {
        return SplitVector_1::FindNextChange(p0, p1);
    }

    int Length() {
        return call_method< int >(py_self, "Length");
    }

    int default_Length() {
        return SplitVector_1::Length();
    }

    void InsertSpace(int p0, int p1) {
        call_method< void >(py_self, "InsertSpace", p0, p1);
    }

    void default_InsertSpace(int p0, int p1) {
        SplitVector_1::InsertSpace(p0, p1);
    }

    void FillRange(int p0, int p1, int p2) {
        call_method< void >(py_self, "FillRange", p0, p1, p2);
    }

    void default_FillRange(int p0, int p1, int p2) {
        SplitVector_1::FillRange(p0, p1, p2);
    }

    void DeleteRange(int p0, int p1) {
        call_method< void >(py_self, "DeleteRange", p0, p1);
    }

    void default_DeleteRange(int p0, int p1) {
        SplitVector_1::DeleteRange(p0, p1);
    }

    void DeleteAll() {
        call_method< void >(py_self, "DeleteAll");
    }

    void default_DeleteAll() {
        SplitVector_1::DeleteAll();
    }

    PyObject* py_self;
};

struct SubstanceUTF16BE_Wrapper: SubstanceUTF16BE
{
    SubstanceUTF16BE_Wrapper(PyObject* py_self_):
        SubstanceUTF16BE(), py_self(py_self_) {}

    int BytesFromCharacters(unsigned char* p0, int p1, const int* p2, int p3) {
        return call_method< int >(py_self, "BytesFromCharacters", p0, p1, p2, p3);
    }

    int default_BytesFromCharacters(unsigned char* p0, int p1, const int* p2, int p3) {
        return SubstanceUTF16BE::BytesFromCharacters(p0, p1, p2, p3);
    }

    int UnitValue(int p0) {
        return call_method< int >(py_self, "UnitValue", p0);
    }

    int default_UnitValue(int p0) {
        return SubstanceUTF16BE::UnitValue(p0);
    }

    int UnitFromArray(unsigned char* p0, int p1) {
        return call_method< int >(py_self, "UnitFromArray", p0, p1);
    }

    int default_UnitFromArray(unsigned char* p0, int p1) {
        return SubstanceUTF16BE::UnitFromArray(p0, p1);
    }

    int DistanceNextCaret(int p0) {
        return call_method< int >(py_self, "DistanceNextCaret", p0);
    }

    int default_DistanceNextCaret(int p0) {
        return SubstanceUTF16::DistanceNextCaret(p0);
    }

    int LenChar(int p0) {
        return call_method< int >(py_self, "LenChar", p0);
    }

    int default_LenChar(int p0) {
        return SubstanceUTF16::LenChar(p0);
    }

    int LenCharacters(const int* p0, int p1) {
        return call_method< int >(py_self, "LenCharacters", p0, p1);
    }

    int default_LenCharacters(const int* p0, int p1) {
        return SubstanceUTF16::LenCharacters(p0, p1);
    }

    int RetrieveUTF8(int p0, unsigned char* p1, int p2) {
        return call_method< int >(py_self, "RetrieveUTF8", p0, p1, p2);
    }

    int default_RetrieveUTF8(int p0, unsigned char* p1, int p2) {
        return SubstanceUTF16::RetrieveUTF8(p0, p1, p2);
    }

    int RetrieveUTF16(int p0, short int* p1, int p2) {
        return call_method< int >(py_self, "RetrieveUTF16", p0, p1, p2);
    }

    int default_RetrieveUTF16(int p0, short int* p1, int p2) {
        return SubstanceUTF16::RetrieveUTF16(p0, p1, p2);
    }

    int RetrieveUTF32(int p0, int* p1, int p2) {
        return call_method< int >(py_self, "RetrieveUTF32", p0, p1, p2);
    }

    int default_RetrieveUTF32(int p0, int* p1, int p2) {
        return SubstanceUTF16::RetrieveUTF32(p0, p1, p2);
    }

    int LengthInText(const int* p0, int p1) {
        return call_method< int >(py_self, "LengthInText", p0, p1);
    }

    int default_LengthInText(const int* p0, int p1) {
        return SubstanceUTF16::LengthInText(p0, p1);
    }

    int CharValue(int p0) {
        return call_method< int >(py_self, "CharValue", p0);
    }

    int default_CharValue(int p0) {
        return SubstanceUTF16::CharValue(p0);
    }

    int MovePositionOutsideChar(int p0, int p1, bool p2) {
        return call_method< int >(py_self, "MovePositionOutsideChar", p0, p1, p2);
    }

    int default_MovePositionOutsideChar(int p0, int p1, bool p2) {
        return SubstanceUTF16::MovePositionOutsideChar(p0, p1, p2);
    }

    bool BetweenCharacters(int p0) {
        return call_method< bool >(py_self, "BetweenCharacters", p0);
    }

    bool default_BetweenCharacters(int p0) {
        return SubstanceUTF16::BetweenCharacters(p0);
    }

    bool ValidateRange(int p0, int p1) {
        return call_method< bool >(py_self, "ValidateRange", p0, p1);
    }

    bool default_ValidateRange(int p0, int p1) {
        return SubstanceUTF16::ValidateRange(p0, p1);
    }

    bool ValidateArray(unsigned char* p0, int p1) {
        return call_method< bool >(py_self, "ValidateArray", p0, p1);
    }

    bool default_ValidateArray(unsigned char* p0, int p1) {
        return SubstanceUTF16::ValidateArray(p0, p1);
    }

    int ValueAt(int p0) {
        return call_method< int >(py_self, "ValueAt", p0);
    }

    int default_ValueAt(int p0) {
        return SplitVector_1::ValueAt(p0);
    }

    int FindNextChange(int p0, int p1) {
        return call_method< int >(py_self, "FindNextChange", p0, p1);
    }

    int default_FindNextChange(int p0, int p1) {
        return SplitVector_1::FindNextChange(p0, p1);
    }

    int Length() {
        return call_method< int >(py_self, "Length");
    }

    int default_Length() {
        return SplitVector_1::Length();
    }

    void InsertSpace(int p0, int p1) {
        call_method< void >(py_self, "InsertSpace", p0, p1);
    }

    void default_InsertSpace(int p0, int p1) {
        SplitVector_1::InsertSpace(p0, p1);
    }

    void FillRange(int p0, int p1, int p2) {
        call_method< void >(py_self, "FillRange", p0, p1, p2);
    }

    void default_FillRange(int p0, int p1, int p2) {
        SplitVector_1::FillRange(p0, p1, p2);
    }

    void DeleteRange(int p0, int p1) {
        call_method< void >(py_self, "DeleteRange", p0, p1);
    }

    void default_DeleteRange(int p0, int p1) {
        SplitVector_1::DeleteRange(p0, p1);
    }

    void DeleteAll() {
        call_method< void >(py_self, "DeleteAll");
    }

    void default_DeleteAll() {
        SplitVector_1::DeleteAll();
    }

    PyObject* py_self;
};

struct IPrototype_Wrapper: IPrototype
{
    Literal* Name() {
        return call_method< Literal* >(py_self, "Name");
    }

    IPrototype* CreateNew() {
        return call_method< IPrototype* >(py_self, "CreateNew");
    }

    PyObject* py_self;
};

struct ILexer_Wrapper: ILexer
{
    Literal* Name() {
        return call_method< Literal* >(py_self, "Name");
    }

    IPrototype* CreateNew() {
        return call_method< IPrototype* >(py_self, "CreateNew");
    }

    void InvalidateAfter(int p0) {
        call_method< void >(py_self, "InvalidateAfter", p0);
    }

    void ColourisePiece(Styler* p0) {
        call_method< void >(py_self, "ColourisePiece", p0);
    }

    void Colourise(Styler* p0) {
        call_method< void >(py_self, "Colourise", p0);
    }

    int States() {
        return call_method< int >(py_self, "States");
    }

    void SetBaseState(int p0) {
        call_method< void >(py_self, "SetBaseState", p0);
    }

    int GetBaseState() {
        return call_method< int >(py_self, "GetBaseState");
    }

    bool InStateRange(int p0) {
        return call_method< bool >(py_self, "InStateRange", p0);
    }

    int StateFromName(unsigned char* p0, int p1) {
        return call_method< int >(py_self, "StateFromName", p0, p1);
    }

    bool CanBeInterrupted(int p0) {
        return call_method< bool >(py_self, "CanBeInterrupted", p0);
    }

    void SetKeyWords(int p0, Literal* p1) {
        call_method< void >(py_self, "SetKeyWords", p0, p1);
    }

    void SetSubLexer(ILexer* p0, Literal* p1, Literal* p2) {
        call_method< void >(py_self, "SetSubLexer", p0, p1, p2);
    }

    ILexer* GetSubLexer(Literal* p0) {
        return call_method< ILexer* >(py_self, "GetSubLexer", p0);
    }

    void SetInputStyles(IStyles* p0) {
        call_method< void >(py_self, "SetInputStyles", p0);
    }

    Literal* StateNames() {
        return call_method< Literal* >(py_self, "StateNames");
    }

    Literal* StateName(int p0) {
        return call_method< Literal* >(py_self, "StateName", p0);
    }

    ILexer* LexerFromState(int p0) {
        return call_method< ILexer* >(py_self, "LexerFromState", p0);
    }

    PyObject* py_self;
};

struct LexScript_Wrapper: LexScript
{
    LexScript_Wrapper(PyObject* py_self_):
        LexScript(), py_self(py_self_) {}

    Literal* Name() {
        return call_method< Literal* >(py_self, "Name");
    }

    Literal* default_Name() {
        return LexBase::Name();
    }

    IPrototype* CreateNew() {
        return call_method< IPrototype* >(py_self, "CreateNew");
    }

    IPrototype* default_CreateNew() {
        return LexScript::CreateNew();
    }

    void InvalidateAfter(int p0) {
        call_method< void >(py_self, "InvalidateAfter", p0);
    }

    void default_InvalidateAfter(int p0) {
        LexScript::InvalidateAfter(p0);
    }

    void AllocateStates(int p0) {
        call_method< void >(py_self, "AllocateStates", p0);
    }

    void default_AllocateStates(int p0) {
        LexScript::AllocateStates(p0);
    }

    void SetSubLexer(ILexer* p0, Literal* p1, Literal* p2) {
        call_method< void >(py_self, "SetSubLexer", p0, p1, p2);
    }

    void default_SetSubLexer(ILexer* p0, Literal* p1, Literal* p2) {
        LexScript::SetSubLexer(p0, p1, p2);
    }

    ILexer* GetSubLexer(Literal* p0) {
        return call_method< ILexer* >(py_self, "GetSubLexer", p0);
    }

    ILexer* default_GetSubLexer(Literal* p0) {
        return LexScript::GetSubLexer(p0);
    }

    Literal* StateName(int p0) {
        return call_method< Literal* >(py_self, "StateName", p0);
    }

    Literal* default_StateName(int p0) {
        return LexScript::StateName(p0);
    }

    ILexer* LexerFromState(int p0) {
        return call_method< ILexer* >(py_self, "LexerFromState", p0);
    }

    ILexer* default_LexerFromState(int p0) {
        return LexScript::LexerFromState(p0);
    }

    void ColourisePiece(Styler* p0) {
        call_method< void >(py_self, "ColourisePiece", p0);
    }

    void default_ColourisePiece(Styler* p0) {
        LexBase::ColourisePiece(p0);
    }

    void Colourise(Styler* p0) {
        call_method< void >(py_self, "Colourise", p0);
    }

    void default_Colourise(Styler* p0) {
        LexBase::Colourise(p0);
    }

    int States() {
        return call_method< int >(py_self, "States");
    }

    int default_States() {
        return LexBase::States();
    }

    void SetBaseState(int p0) {
        call_method< void >(py_self, "SetBaseState", p0);
    }

    void default_SetBaseState(int p0) {
        LexBase::SetBaseState(p0);
    }

    int GetBaseState() {
        return call_method< int >(py_self, "GetBaseState");
    }

    int default_GetBaseState() {
        return LexBase::GetBaseState();
    }

    bool InStateRange(int p0) {
        return call_method< bool >(py_self, "InStateRange", p0);
    }

    bool default_InStateRange(int p0) {
        return LexBase::InStateRange(p0);
    }

    int StateFromName(unsigned char* p0, int p1) {
        return call_method< int >(py_self, "StateFromName", p0, p1);
    }

    int default_StateFromName(unsigned char* p0, int p1) {
        return LexBase::StateFromName(p0, p1);
    }

    bool CanBeInterrupted(int p0) {
        return call_method< bool >(py_self, "CanBeInterrupted", p0);
    }

    bool default_CanBeInterrupted(int p0) {
        return LexBase::CanBeInterrupted(p0);
    }

    void SetKeyWords(int p0, Literal* p1) {
        call_method< void >(py_self, "SetKeyWords", p0, p1);
    }

    void default_SetKeyWords(int p0, Literal* p1) {
        LexBase::SetKeyWords(p0, p1);
    }

    void SetInputStyles(IStyles* p0) {
        call_method< void >(py_self, "SetInputStyles", p0);
    }

    void default_SetInputStyles(IStyles* p0) {
        LexBase::SetInputStyles(p0);
    }

    Literal* StateNames() {
        return call_method< Literal* >(py_self, "StateNames");
    }

    Literal* default_StateNames() {
        return LexBase::StateNames();
    }

    PyObject* py_self;
};

struct LexBase_Wrapper: LexBase, wrapper<LexBase> {

	static int deletions;
	
	virtual ~LexBase_Wrapper() {
		deletions++;
	}

	Literal* Name() {
		if (override f = this->get_override("Name"))
			return f();
		else
			return LexBase::Name();
	}

	Literal* default_Name() {
		return this->LexBase::Name();
	}

	IPrototype* CreateNew() {
		if (override f = this->get_override("CreateNew"))
			return f();
		else
			return LexBase::CreateNew();
	}

	IPrototype* default_CreateNew() {
		return this->LexBase::CreateNew();
	}

	void InvalidateAfter(int p0) {
		if (override f = this->get_override("InvalidateAfter"))
			f(p0);
		else
			LexBase::InvalidateAfter(p0);
	}

	void default_InvalidateAfter(int p0) {
		this->LexBase::InvalidateAfter(p0);
	}

	void ColourisePiece(Styler* p0) {
		if (override f = this->get_override("ColourisePiece"))
			f(p0);
		else
			LexBase::ColourisePiece(p0);
	}

	void default_ColourisePiece(Styler* p0) {
		this->LexBase::ColourisePiece(p0);
	}

	void Colourise(Styler* p0) {
		if (override f = this->get_override("Colourise"))
			f(p0);
		else
			LexBase::Colourise(p0);
	}

	void default_Colourise(Styler* p0) {
		this->LexBase::Colourise(p0);
	}

	int States() {
		if (override f = this->get_override("States"))
			return call<int>(f.ptr());
		return LexBase::States();
	}

	int default_States() {
		return this->LexBase::States();
	}

	void SetBaseState(int p0) {
		if (override f = this->get_override("SetBaseState"))
			f(p0);
		else
			LexBase::SetBaseState(p0);
	}

	void default_SetBaseState(int p0) {
		this->LexBase::SetBaseState(p0);
	}

	int GetBaseState() {
		if (override f = this->get_override("GetBaseState"))
			return call<int>(f.ptr());
		return LexBase::GetBaseState();
	}

	int default_GetBaseState() {
		return this->LexBase::GetBaseState();
	}

	bool InStateRange(int p0) {
		if (override f = this->get_override("InStateRange"))
			return call<bool>(f.ptr(), p0);
		return LexBase::InStateRange(p0);
	}

	bool default_InStateRange(int p0) {
		return this->LexBase::InStateRange(p0);
	}

	int StateFromName(unsigned char* p0, int p1) {
		if (override f = this->get_override("StateFromName"))
			return call<int>(f.ptr(), p0, p1);
			//return f(p0, p1);
		return LexBase::StateFromName(p0, p1);
	}

	int default_StateFromName(unsigned char* p0, int p1) {
		return this->LexBase::StateFromName(p0, p1);
	}

	bool CanBeInterrupted(int p0) {
		if (override f = this->get_override("CanBeInterrupted"))
			return call<bool>(f.ptr(), p0);
			//return f(p0);
		return LexBase::CanBeInterrupted(p0);
	}

	bool default_CanBeInterrupted(int p0) {
		return this->LexBase::CanBeInterrupted(p0);
	}

	void SetKeyWords(int p0, Literal* p1) {
		if (override f = this->get_override("SetKeyWords"))
			f(p0, p1);
		else
			LexBase::SetKeyWords(p0, p1);
	}

	void default_SetKeyWords(int p0, Literal* p1) {
		this->LexBase::SetKeyWords(p0, p1);
	}

	void SetSubLexer(ILexer* p0, Literal* p1, Literal* p2) {
		if (override f = this->get_override("SetSubLexer"))
			f(p0, p1, p2);
		else
			LexBase::SetSubLexer(p0, p1, p2);
	}

	void default_SetSubLexer(ILexer* p0, Literal* p1, Literal* p2) {
		LexBase::SetSubLexer(p0, p1, p2);
	}

	ILexer* GetSubLexer(Literal* p0) {
		if (override f = this->get_override("GetSubLexer"))
			return f(p0);
		return LexBase::GetSubLexer(p0);
	}

	ILexer* default_GetSubLexer(Literal* p0) {
		return this->LexBase::GetSubLexer(p0);
	}

	void SetInputStyles(IStyles* p0) {
		if (override f = this->get_override("SetInputStyles"))
			f(p0);
		else
			LexBase::SetInputStyles(p0);
	}

	void default_SetInputStyles(IStyles* p0) {
		this->LexBase::SetInputStyles(p0);
	}

	void AllocateStates(int p0) {
		if (override f = this->get_override("AllocateStates"))
			f(p0);
		else
			LexBase::AllocateStates(p0);
	}

	void default_AllocateStates(int p0) {
		this->LexBase::AllocateStates(p0);
	}

	Literal* StateNames() {
		override f = this->get_override("StateNames");
		if (f)
			return f();
		else
			return LexBase::StateNames();
	}

	Literal* default_StateNames() {
		return this->LexBase::StateNames();
	}

	Literal* StateName(int p0) {
		if (override f = this->get_override("StateName"))
			return f(p0);
		return LexBase::StateName(p0);
	}

	Literal* default_StateName(int p0) {
		return this->LexBase::StateName(p0);
	}

	ILexer* LexerFromState(int p0) {
		if (override f = this->get_override("LexerFromState"))
			return f(p0);
		return LexBase::LexerFromState(p0);
	}

	ILexer* default_LexerFromState(int p0) {
		return this->LexBase::LexerFromState(p0);
	}
};



}// namespace 
