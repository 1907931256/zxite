package SinkWorld;

public interface IDocumentChangeListener {
	public void StateAltered();
	public void InvalidateAll();
	public void InvalidateRange(int a, int b);
};
