using System;
using System.Drawing;

namespace SinkWorld {
	
public class PaintContext {
	public Surface surface;
	public SRectangle rc;
	public PaintContext(Surface surface_, SRectangle rc_) {
		surface = surface_;
		rc = rc_;
	}
}

}
