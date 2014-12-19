using System;
using System.Diagnostics;

namespace SinkWorld {
	
public interface IDocumentChangeListener {
	void StateAltered();
	void InvalidateAll();
	void InvalidateRange(int a, int b);
};

public delegate void Feedback();
public delegate void FeedbackRange(int a, int b);

// IronPython 0.7.5 can't yet implement an interface so use delegates
public class IDCL : IDocumentChangeListener {
	public event Feedback StateAlteredEvent; 
	public event Feedback InvalidateAllEvent; 
	public event FeedbackRange InvalidateRangeEvent; 
	public IDCL() {
	}
	public virtual void StateAltered() {
		if (StateAlteredEvent != null) {
			StateAlteredEvent();
		}
	}
	public virtual void InvalidateAll() {
		if (InvalidateAllEvent != null) {
			InvalidateAllEvent();
		}
	}
	public virtual void InvalidateRange(int a, int b) {
		if (InvalidateRangeEvent != null) {
			InvalidateRangeEvent(a,b);
		}
	}
};

}
