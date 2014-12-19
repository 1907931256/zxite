// SinkWorld TentacleControl class.
// Copyright 2004 Neil Hodgson

// Tentacle is a demonstration application for SinkWorld

/// An interface implemented by objects that want to be notified of changes to a Document.

class IDocumentChangeListener {
public:
	virtual void StateAltered() = 0;
	virtual void Invalidate() = 0;
	virtual void InvalidateRange(int a, int b) = 0;
};

class Wrapper {
public:
	virtual ~Wrapper() {}
	virtual void Start() = 0;
	virtual void Finish() = 0;
};

/// A TentacleControl is a widget or control that shows a view of a Document.

class TentacleControl {
protected:
	HWND wEdit;
	enum { horizontalRange = 2000 };
	enum { horizontalPage = 200 };
	long wheelDelta;
	long doubleClickTime;
	long timeEvent;
	long timeLastCaret;
	long timeLastClick;
	Point lastClickAt;
	Point mouseDragLast;
	bool inDrag;
	int selType;
	int positionFeedBack;
	bool showFeedBack;
	bool downFeedBack;
	Literal *wordFeedBack;
	bool idlerState;
	int idleCount;
	int idlesInFlight;
	int nextLineWrapped;
	int goodTopLine;
	Document *pdoc;
	IDocumentChangeListener *frame;
	Literal *endOfLine;
	Literal *tab;
public:
	bool braceLevel;
	bool fancy;
	bool wrap;

	TextView tv;
	RangeSelection sr;

	IStyles *pstyleFeedBack;
	StyleSet *ssSelection;
	StyleSet *ssCaret;
	static Wrapper *wrapper;

	TentacleControl();
	virtual ~TentacleControl();

public:
	virtual HWND TopWindow() = 0;
	virtual bool HasFocus() = 0;
	virtual void Focus() = 0;
	virtual Surface *SurfaceForMeasurement() = 0;
	virtual int PositionFromLocation(Point pt) = 0;
	virtual void SetPosition(SRectangle *rc) {}
	virtual void GetViewRectangle(SRectangle *rc) = 0;
	virtual void CreateControl(HWND wParent) = 0;

	void Deallocate();

	int InsertBytes(int position, SW_BYTE s[], int insertLength);
	int InsertUTF32(int position, const int s[], int insertLength);
	int InsertUTF8(int position, SW_BYTE s[], int insertLength);
	bool DeleteRange(int position, int deleteLength);

	bool CheckWrap();
	void SetDocument(Document *pdoc_, IDocumentChangeListener *frame_);
	void SetSelectionStyles();
	void InvalidateRectangle(SRectangle rcInvalidate);
	void InvalidateRange(int a, int b);
	void Invalidate();
	bool PerformIdle(bool checkIfInIdle);
	void SizeChanged();
	void ScrollTo(int line, bool updateScrollBar);
	virtual void EditScrollWindow(int pixels) = 0;
	virtual void SetScrollPos(int line) = 0;
	virtual void SetScrollBars() = 0;
	void PaintRect(Surface &surface, SRectangle rcPaint);
	void SetSelection(int newCaret, int newAnchor);
	void SetSelection(int newCaret) {
		SetSelection(newCaret, newCaret);
	}
	void MoveCaret(int newCaret, bool retainCurrentAnchor, bool updateSynchronous=true);
	void CheckLineForInvalidate(int position);
	void LineDuplicate();
	bool KeyPressed(int keyCode, bool control, bool shift, long timeMessage);
	void CharPressed(int charPressed);
	virtual void SetPrimary() = 0;
	void Undo();
	void Redo();
	void Cut();
	virtual void Copy() = 0;
	virtual void Paste() = 0;
	void Clear();
	void SelectAll();
	enum FoldCommand { fcShow, fcFold, fcDiscover };
	void FoldLine(int currentLine, FoldCommand cmd);
	void FoldAll(FoldCommand cmd);
	void ToggleFold();
	void ToggleBraceLevel();
	void ToggleFancy();
	void StartWrap();
	virtual void SetIdle(bool on) = 0;
	void ToggleWrap();
	void EnsureCaretVisible();
	void MouseDrag(Point pt);
	Literal *LiteralFromRange(int position, int length);
	Literal *WordAtPosition(int position);
	void ShowFeedBack(int position, bool force);
	void MouseFeedBack(Point pt);
	void MouseDown(Point pt, long timeMessage);
	void MouseUp(Point pt);
	void CaretOn();
	void Tick(long timeMessage);
public:
	static TentacleControl *Allocate();
	static void SetWrapper(Wrapper *wrap_);
};

#ifdef unix
#else
void ControlRegisterWindowClass(HINSTANCE hInstance);
#endif
