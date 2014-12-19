// Declarations ================================================================
namespace  {

struct IPaintPhase_Wrapper: IPaintPhase
{
    int FlagsUse() {
        return call_method< int >(py_self, "FlagsUse");
    }

    void Paint(PaintParameters* p0) {
        call_method< void >(py_self, "Paint", p0);
    }

    PyObject* py_self;
};

struct PhaseBack_Wrapper: PhaseBack
{
    int FlagsUse() {
        return call_method< int >(py_self, "FlagsUse");
    }

    int default_FlagsUse() {
        return PhaseBack::FlagsUse();
    }

    void Paint(PaintParameters* p0) {
        call_method< void >(py_self, "Paint", p0);
    }

    void default_Paint(PaintParameters* p0) {
        PhaseBack::Paint(p0);
    }

    PyObject* py_self;
};

struct PhaseText_Wrapper: PhaseText
{
    int FlagsUse() {
        return call_method< int >(py_self, "FlagsUse");
    }

    int default_FlagsUse() {
        return PhaseText::FlagsUse();
    }

    void Paint(PaintParameters* p0) {
        call_method< void >(py_self, "Paint", p0);
    }

    void default_Paint(PaintParameters* p0) {
        PhaseText::Paint(p0);
    }

    PyObject* py_self;
};

struct PhaseDecoration_Wrapper: PhaseDecoration
{
    PhaseDecoration_Wrapper(PyObject* py_self_, bool p0):
        PhaseDecoration(p0), py_self(py_self_) {}

    int FlagsUse() {
        return call_method< int >(py_self, "FlagsUse");
    }

    int default_FlagsUse() {
        return PhaseDecoration::FlagsUse();
    }

    void Paint(PaintParameters* p0) {
        call_method< void >(py_self, "Paint", p0);
    }

    void default_Paint(PaintParameters* p0) {
        PhaseDecoration::Paint(p0);
    }

    PyObject* py_self;
};

struct IDocumentChangeListener_Wrapper: IDocumentChangeListener
{
    void StateAltered() {
        call_method< void >(py_self, "StateAltered");
    }

    void Invalidate() {
        call_method< void >(py_self, "Invalidate");
    }

    void InvalidateRange(int p0, int p1) {
        call_method< void >(py_self, "InvalidateRange", p0, p1);
    }

    PyObject* py_self;
};

struct TentacleControl_Wrapper: TentacleControl
{
    TentacleControl_Wrapper(PyObject* py_self_):
        TentacleControl(), py_self(py_self_) {}

    HWND TopWindow() {
        return (HWND)(call_method< int >(py_self, "TopWindow"));
    }

    bool HasFocus() {
        return call_method< bool >(py_self, "HasFocus");
    }

    void Focus() {
        call_method< void >(py_self, "Focus");
    }

    Surface* SurfaceForMeasurement() {
        return call_method< Surface* >(py_self, "SurfaceForMeasurement");
    }

    int PositionFromLocation(Point p0) {
        return call_method< int >(py_self, "PositionFromLocation", p0);
    }

    void SetPosition(SRectangle* p0) {
        call_method< void >(py_self, "SetPosition", p0);
    }

    void default_SetPosition(SRectangle* p0) {
        TentacleControl::SetPosition(p0);
    }

    void GetViewRectangle(SRectangle* p0) {
        call_method< void >(py_self, "GetViewRectangle", p0);
    }

    void CreateControl(HWND p0) {
        call_method< void >(py_self, "CreateControl", (int)p0);
    }

    void EditScrollWindow(int p0) {
        call_method< void >(py_self, "EditScrollWindow", p0);
    }

    void SetScrollPos(int p0) {
        call_method< void >(py_self, "SetScrollPos", p0);
    }

    void SetScrollBars() {
        call_method< void >(py_self, "SetScrollBars");
    }

    void SetPrimary() {
        call_method< void >(py_self, "SetPrimary");
    }

    void Copy() {
        call_method< void >(py_self, "Copy");
    }

    void Paste() {
        call_method< void >(py_self, "Paste");
    }

    void SetIdle(bool p0) {
        call_method< void >(py_self, "SetIdle", p0);
    }

    PyObject* py_self;
};

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(TentacleControl_MoveCaret_overloads_2_3, MoveCaret, 2, 3)

struct ChangeLayer_Wrapper: ChangeLayer
{
    ChangeLayer_Wrapper(PyObject* py_self_):
        ChangeLayer(), py_self(py_self_) {}

    void FindNextChange(int p0) {
        call_method< void >(py_self, "FindNextChange", p0);
    }

    PyObject* py_self;
};

struct BaseDecoration_Wrapper: BaseDecoration
{
    BaseDecoration_Wrapper(PyObject* py_self_):
        BaseDecoration(), py_self(py_self_) {}

    IPrototype* CreateNew() {
        return call_method< IPrototype* >(py_self, "CreateNew");
    }

    IPrototype* default_CreateNew() {
        return BaseDecoration::CreateNew();
    }

    IDecoration* Clone() {
        return call_method< IDecoration* >(py_self, "Clone");
    }

    IDecoration* default_Clone() {
        return BaseDecoration::Clone();
    }

    void SetFore(RGBColor p0) {
        call_method< void >(py_self, "SetFore", p0);
    }

    void default_SetFore(RGBColor p0) {
        BaseDecoration::SetFore(p0);
    }

    void Paint(PaintContext* p0) {
        call_method< void >(py_self, "Paint", p0);
    }

    void default_Paint(PaintContext* p0) {
        BaseDecoration::Paint(p0);
    }

    void FromOptions(Literal* p0, Literal* p1, OptionSet* p2) {
        call_method< void >(py_self, "FromOptions", p0, p1, p2);
    }

    void default_FromOptions(Literal* p0, Literal* p1, OptionSet* p2) {
        BaseDecoration::FromOptions(p0, p1, p2);
    }

    PyObject* py_self;
};

struct DecorationBox_Wrapper: DecorationBox
{
    DecorationBox_Wrapper(PyObject* py_self_):
        DecorationBox(), py_self(py_self_) {}

    IPrototype* CreateNew() {
        return call_method< IPrototype* >(py_self, "CreateNew");
    }

    IPrototype* default_CreateNew() {
        return DecorationBox::CreateNew();
    }

    IDecoration* Clone() {
        return call_method< IDecoration* >(py_self, "Clone");
    }

    IDecoration* default_Clone() {
        return DecorationBox::Clone();
    }

    void SetFore(RGBColor p0) {
        call_method< void >(py_self, "SetFore", p0);
    }

    void default_SetFore(RGBColor p0) {
        DecorationBox::SetFore(p0);
    }

    void Paint(PaintContext* p0) {
        call_method< void >(py_self, "Paint", p0);
    }

    void default_Paint(PaintContext* p0) {
        DecorationBox::Paint(p0);
    }

    void FromOptions(Literal* p0, Literal* p1, OptionSet* p2) {
        call_method< void >(py_self, "FromOptions", p0, p1, p2);
    }

    void default_FromOptions(Literal* p0, Literal* p1, OptionSet* p2) {
        DecorationBox::FromOptions(p0, p1, p2);
    }

    PyObject* py_self;
};

struct DecorationButton_Wrapper: DecorationButton
{
    DecorationButton_Wrapper(PyObject* py_self_, bool p0):
        DecorationButton(p0), py_self(py_self_) {}

    IPrototype* CreateNew() {
        return call_method< IPrototype* >(py_self, "CreateNew");
    }

    IPrototype* default_CreateNew() {
        return DecorationButton::CreateNew();
    }

    IDecoration* Clone() {
        return call_method< IDecoration* >(py_self, "Clone");
    }

    IDecoration* default_Clone() {
        return DecorationButton::Clone();
    }

    void SetFore(RGBColor p0) {
        call_method< void >(py_self, "SetFore", p0);
    }

    void default_SetFore(RGBColor p0) {
        DecorationButton::SetFore(p0);
    }

    void Paint(PaintContext* p0) {
        call_method< void >(py_self, "Paint", p0);
    }

    void default_Paint(PaintContext* p0) {
        DecorationButton::Paint(p0);
    }

    void FromOptions(Literal* p0, Literal* p1, OptionSet* p2) {
        call_method< void >(py_self, "FromOptions", p0, p1, p2);
    }

    void default_FromOptions(Literal* p0, Literal* p1, OptionSet* p2) {
        DecorationButton::FromOptions(p0, p1, p2);
    }

    PyObject* py_self;
};

struct DecorationCaret_Wrapper: DecorationCaret
{
    DecorationCaret_Wrapper(PyObject* py_self_, int p0):
        DecorationCaret(p0), py_self(py_self_) {}

    IPrototype* CreateNew() {
        return call_method< IPrototype* >(py_self, "CreateNew");
    }

    IPrototype* default_CreateNew() {
        return DecorationCaret::CreateNew();
    }

    IDecoration* Clone() {
        return call_method< IDecoration* >(py_self, "Clone");
    }

    IDecoration* default_Clone() {
        return DecorationCaret::Clone();
    }

    void SetFore(RGBColor p0) {
        call_method< void >(py_self, "SetFore", p0);
    }

    void default_SetFore(RGBColor p0) {
        DecorationCaret::SetFore(p0);
    }

    void Paint(PaintContext* p0) {
        call_method< void >(py_self, "Paint", p0);
    }

    void default_Paint(PaintContext* p0) {
        DecorationCaret::Paint(p0);
    }

    void FromOptions(Literal* p0, Literal* p1, OptionSet* p2) {
        call_method< void >(py_self, "FromOptions", p0, p1, p2);
    }

    void default_FromOptions(Literal* p0, Literal* p1, OptionSet* p2) {
        DecorationCaret::FromOptions(p0, p1, p2);
    }

    PyObject* py_self;
};

struct DecorationGraduatedBox_Wrapper: DecorationGraduatedBox
{
    DecorationGraduatedBox_Wrapper(PyObject* py_self_):
        DecorationGraduatedBox(), py_self(py_self_) {}

    IPrototype* CreateNew() {
        return call_method< IPrototype* >(py_self, "CreateNew");
    }

    IPrototype* default_CreateNew() {
        return DecorationGraduatedBox::CreateNew();
    }

    IDecoration* Clone() {
        return call_method< IDecoration* >(py_self, "Clone");
    }

    IDecoration* default_Clone() {
        return DecorationGraduatedBox::Clone();
    }

    void SetFore(RGBColor p0) {
        call_method< void >(py_self, "SetFore", p0);
    }

    void default_SetFore(RGBColor p0) {
        DecorationGraduatedBox::SetFore(p0);
    }

    void Paint(PaintContext* p0) {
        call_method< void >(py_self, "Paint", p0);
    }

    void default_Paint(PaintContext* p0) {
        DecorationGraduatedBox::Paint(p0);
    }

    void FromOptions(Literal* p0, Literal* p1, OptionSet* p2) {
        call_method< void >(py_self, "FromOptions", p0, p1, p2);
    }

    void default_FromOptions(Literal* p0, Literal* p1, OptionSet* p2) {
        DecorationGraduatedBox::FromOptions(p0, p1, p2);
    }

    PyObject* py_self;
};

struct DecorationStrikeOut_Wrapper: DecorationStrikeOut
{
    DecorationStrikeOut_Wrapper(PyObject* py_self_):
        DecorationStrikeOut(), py_self(py_self_) {}

    IPrototype* CreateNew() {
        return call_method< IPrototype* >(py_self, "CreateNew");
    }

    IPrototype* default_CreateNew() {
        return DecorationStrikeOut::CreateNew();
    }

    IDecoration* Clone() {
        return call_method< IDecoration* >(py_self, "Clone");
    }

    IDecoration* default_Clone() {
        return DecorationStrikeOut::Clone();
    }

    void SetFore(RGBColor p0) {
        call_method< void >(py_self, "SetFore", p0);
    }

    void default_SetFore(RGBColor p0) {
        DecorationStrikeOut::SetFore(p0);
    }

    void Paint(PaintContext* p0) {
        call_method< void >(py_self, "Paint", p0);
    }

    void default_Paint(PaintContext* p0) {
        DecorationStrikeOut::Paint(p0);
    }

    void FromOptions(Literal* p0, Literal* p1, OptionSet* p2) {
        call_method< void >(py_self, "FromOptions", p0, p1, p2);
    }

    void default_FromOptions(Literal* p0, Literal* p1, OptionSet* p2) {
        DecorationStrikeOut::FromOptions(p0, p1, p2);
    }

    PyObject* py_self;
};

struct DecorationRoundedBox_Wrapper: DecorationRoundedBox
{
    DecorationRoundedBox_Wrapper(PyObject* py_self_):
        DecorationRoundedBox(), py_self(py_self_) {}

    IPrototype* CreateNew() {
        return call_method< IPrototype* >(py_self, "CreateNew");
    }

    IPrototype* default_CreateNew() {
        return DecorationRoundedBox::CreateNew();
    }

    IDecoration* Clone() {
        return call_method< IDecoration* >(py_self, "Clone");
    }

    IDecoration* default_Clone() {
        return DecorationRoundedBox::Clone();
    }

    void SetFore(RGBColor p0) {
        call_method< void >(py_self, "SetFore", p0);
    }

    void default_SetFore(RGBColor p0) {
        DecorationRoundedBox::SetFore(p0);
    }

    void Paint(PaintContext* p0) {
        call_method< void >(py_self, "Paint", p0);
    }

    void default_Paint(PaintContext* p0) {
        DecorationRoundedBox::Paint(p0);
    }

    void FromOptions(Literal* p0, Literal* p1, OptionSet* p2) {
        call_method< void >(py_self, "FromOptions", p0, p1, p2);
    }

    void default_FromOptions(Literal* p0, Literal* p1, OptionSet* p2) {
        DecorationRoundedBox::FromOptions(p0, p1, p2);
    }

    PyObject* py_self;
};

struct DecorationUnderLine_Wrapper: DecorationUnderLine
{
    DecorationUnderLine_Wrapper(PyObject* py_self_):
        DecorationUnderLine(), py_self(py_self_) {}

    IPrototype* CreateNew() {
        return call_method< IPrototype* >(py_self, "CreateNew");
    }

    IPrototype* default_CreateNew() {
        return DecorationUnderLine::CreateNew();
    }

    IDecoration* Clone() {
        return call_method< IDecoration* >(py_self, "Clone");
    }

    IDecoration* default_Clone() {
        return DecorationUnderLine::Clone();
    }

    void SetFore(RGBColor p0) {
        call_method< void >(py_self, "SetFore", p0);
    }

    void default_SetFore(RGBColor p0) {
        DecorationUnderLine::SetFore(p0);
    }

    void Paint(PaintContext* p0) {
        call_method< void >(py_self, "Paint", p0);
    }

    void default_Paint(PaintContext* p0) {
        DecorationUnderLine::Paint(p0);
    }

    void FromOptions(Literal* p0, Literal* p1, OptionSet* p2) {
        call_method< void >(py_self, "FromOptions", p0, p1, p2);
    }

    void default_FromOptions(Literal* p0, Literal* p1, OptionSet* p2) {
        DecorationUnderLine::FromOptions(p0, p1, p2);
    }

    PyObject* py_self;
};

struct PhysicalLayout_Wrapper: PhysicalLayout
{
    PhysicalLayout_Wrapper(PyObject* py_self_):
        PhysicalLayout(), py_self(py_self_) {}

    void Clear() {
        call_method< void >(py_self, "Clear");
    }

    void default_Clear() {
        PhysicalLayout::Clear();
    }

    void InsertLine(int p0) {
        call_method< void >(py_self, "InsertLine", p0);
    }

    void default_InsertLine(int p0) {
        PhysicalLayout::InsertLine(p0);
    }

    void RemoveLine(int p0) {
        call_method< void >(py_self, "RemoveLine", p0);
    }

    void default_RemoveLine(int p0) {
        PhysicalLayout::RemoveLine(p0);
    }

    PyObject* py_self;
};

struct IDecoration_Wrapper: IDecoration
{
    IPrototype* CreateNew() {
        return call_method< IPrototype* >(py_self, "CreateNew");
    }

    IDecoration* Clone() {
        return call_method< IDecoration* >(py_self, "Clone");
    }

    void Paint(PaintContext* p0) {
        call_method< void >(py_self, "Paint", p0);
    }

    void SetFore(RGBColor p0) {
        call_method< void >(py_self, "SetFore", p0);
    }

    PyObject* py_self;
};

struct RangeSelection_Wrapper: RangeSelection
{
    RangeSelection_Wrapper(PyObject* py_self_):
        RangeSelection(), py_self(py_self_) {}

    int Length() {
        return call_method< int >(py_self, "Length");
    }

    int default_Length() {
        return RangeSelection::Length();
    }

    int ValueAt(int p0) {
        return call_method< int >(py_self, "ValueAt", p0);
    }

    int default_ValueAt(int p0) {
        return RangeSelection::ValueAt(p0);
    }

    int FindNextChange(int p0, int p1) {
        return call_method< int >(py_self, "FindNextChange", p0, p1);
    }

    int default_FindNextChange(int p0, int p1) {
        return RangeSelection::FindNextChange(p0, p1);
    }

    void FillRange(int p0, int p1, int p2) {
        call_method< void >(py_self, "FillRange", p0, p1, p2);
    }

    void default_FillRange(int p0, int p1, int p2) {
        RangeSelection::FillRange(p0, p1, p2);
    }

    void InsertSpace(int p0, int p1) {
        call_method< void >(py_self, "InsertSpace", p0, p1);
    }

    void default_InsertSpace(int p0, int p1) {
        RangeSelection::InsertSpace(p0, p1);
    }

    void DeleteAll() {
        call_method< void >(py_self, "DeleteAll");
    }

    void default_DeleteAll() {
        RangeSelection::DeleteAll();
    }

    void DeleteRange(int p0, int p1) {
        call_method< void >(py_self, "DeleteRange", p0, p1);
    }

    void default_DeleteRange(int p0, int p1) {
        RangeSelection::DeleteRange(p0, p1);
    }

    PyObject* py_self;
};

struct StyleLayer_Wrapper: StyleLayer
{
    StyleLayer_Wrapper(PyObject* py_self_, int p0, int p1, IStyles* p2, int p3):
        StyleLayer(p0, p1, p2, p3), py_self(py_self_) {}

    void FindNextChange(int p0) {
        call_method< void >(py_self, "FindNextChange", p0);
    }

    void default_FindNextChange(int p0) {
        StyleLayer::FindNextChange(p0);
    }

    PyObject* py_self;
};

struct SubstanceLayer_Wrapper: SubstanceLayer
{
    SubstanceLayer_Wrapper(PyObject* py_self_, int p0, int p1, Substance* p2, int p3):
        SubstanceLayer(p0, p1, p2, p3), py_self(py_self_) {}

    void FindNextChange(int p0) {
        call_method< void >(py_self, "FindNextChange", p0);
    }

    void default_FindNextChange(int p0) {
        SubstanceLayer::FindNextChange(p0);
    }

    PyObject* py_self;
};


}// namespace 


