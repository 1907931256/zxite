using System;
using System.Drawing;
namespace SinkWorld {

public class Surface {
	public static Literal fontDefault = null;

	internal Graphics g;
	internal FontDescription font;
	internal Font fontHandle;
	internal int ascent = 1;
	internal SolidBrush textBrush = new SolidBrush(Color.Black);
	internal Pen linePen = new Pen(Color.Black);

	internal RGBColor currentColor = new RGBColor(0x1000000);
	internal int x = 0;
	internal int y = 0;
	internal StringFormat sf = null;

	internal Surface(Graphics g_) {
		g = g_;
		sf = new StringFormat(StringFormat.GenericTypographic);
		sf.FormatFlags = StringFormatFlags.MeasureTrailingSpaces;
		//g.TextRenderingHint = System.Drawing.Text.TextRenderingHint.AntiAlias;
		//g.SmoothingMode = System.Drawing.Drawing2D.SmoothingMode.AntiAlias;
		g.SmoothingMode = System.Drawing.Drawing2D.SmoothingMode.None;
	}

	public virtual void SetFont(FontDescription font_) {
		if (font != font_) {
			if ((font == null) || (!font.Equals(font_))) {
				font = font_;
				System.String fontName = "Verdana";
				try {
					int maxSize = font.font.Length() * 3;
					byte[] nameBytes = new byte[maxSize];
					int lenUTF8 = UniConversion.UTF8FromUTF32(
						nameBytes, nameBytes.Length,
						font.font.Data(), font.font.Length());
					//~ byte[] nameBytes = font.font.Value();
					fontName = System.Text.Encoding.UTF8.GetString(nameBytes, 0, lenUTF8);
				}
				catch (System.IO.IOException) {
					System.Console.Out.WriteLine("Failed to convert");
				}
				catch (System.NullReferenceException) {
					System.Console.Out.WriteLine("Null to convert");
				}
				//~ System.out.println("SetFont " + fontName + " " + font.height);
				int fs = 0;
				if (font.bold)
					fs |= (int) FontStyle.Bold;
				if (font.italics)
					fs |= (int) FontStyle.Italic;
				fontHandle = new Font(fontName, font.height, (FontStyle) fs);
				ascent = TextAscent();
			}
		}
	}
	public virtual void SetTextColor(RGBColor color) {
		SetPen(color);
	}
	public virtual void SetPen(RGBColor color) {
		if (currentColor.NotEqual(color)) {
			Color col = Color.FromArgb((int)color.GetRed(), (int)color.GetGreen(), (int)color.GetBlue());
			textBrush = new SolidBrush(col);
			linePen = new Pen(col, 1.0f);
			currentColor.co = color.co;
		}
	}
	public virtual void Free() {
	}
	public virtual void FillRectangle(SRectangle rc, RGBColor color) {
		SetPen(color);
		g.FillRectangle(textBrush, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top);
	}
	public virtual void FrameRectangle(SRectangle rc, RGBColor color) {
		SetPen(color);
		g.DrawRectangle(linePen, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top);
	}
	public virtual void Polygon(Point []pts, int npts, RGBColor fore, int alphaFill, int alphaOutline) {
		// Convert from SinkWorld Point array to system Point array
		System.Drawing.Point[] poly = new System.Drawing.Point[npts];
		for (int i=0; i<npts; i++) {
			poly[i] = new System.Drawing.Point(pts[i].x, pts[i].y);
		}
		Color colFill = Color.FromArgb(alphaFill, (int)fore.GetRed(), (int)fore.GetGreen(), (int)fore.GetBlue());
		g.FillPolygon(new SolidBrush(colFill), poly);
		Color colOutline = Color.FromArgb(alphaOutline, (int)fore.GetRed(), (int)fore.GetGreen(), (int)fore.GetBlue());
		g.DrawPolygon(new Pen(colOutline, 0.0f), poly);
	}
	public virtual void MoveTo(int x_, int y_) {
		x = x_;
		y = y_;
	}
	public virtual void LineTo(int x_, int y_) {
		g.SmoothingMode = System.Drawing.Drawing2D.SmoothingMode.AntiAlias;
		g.DrawLine(linePen, x, y, x_, y_);
		g.SmoothingMode = System.Drawing.Drawing2D.SmoothingMode.None;
		x = x_;
		y = y_;
	}
	public virtual void SetTransparent(bool transparent) {
	}
	public virtual void xMeasureWidths(byte[] s, int len, TextPositions positions, int enc) {
		try {
			String sg = System.Text.Encoding.UTF8.GetString(s, 0, len);
			int sLength = sg.Length;
			SizeF sz = g.MeasureString(sg, fontHandle, 2000, sf);
			float maxWidth = sz.Width;
			float x = 0;
			float []xPositions = new float[sLength];
			for (int i = 0; i < sLength; i++) {
				char ch = sg[i];
				sz = g.MeasureString(new String(ch, 1), fontHandle, 2000, sf);
				x += sz.Width;
				xPositions[i] = x;
			}
			float ratio = maxWidth / x;
			for (int i = 0; i < sLength; i++) {
				char ch = sg[i];
				int xPixel = (int)(xPositions[i] * ratio);
				positions.Add(xPixel);
				if (ch >= 0x80)
					positions.Add(xPixel);
				if (ch >= 0x800)
					positions.Add(xPixel);
			}
		}
		catch (System.IO.IOException) {
			System.Console.Out.WriteLine("Failed to convert");
		}
	}
	public virtual void MeasureWidths(byte[] s, int len, TextPositions positions, int enc) {
		// A bug in GDI+ means that the string has to be broken up into blocks
		// of 32 or less.
		int maxMeasurePerCall = 32;
		try {
			String sAll = System.Text.Encoding.UTF8.GetString(s, 0, len);
			int sAllLength = sAll.Length;
			for (int start=0; start<sAllLength; start+=maxMeasurePerCall) {
				int sLength = sAllLength - start;
				if (sLength > maxMeasurePerCall) {
					sLength = maxMeasurePerCall;
				}
				CharacterRange []characterRanges = new CharacterRange[sLength];
				for (int i = 0; i<sLength; i++) {
					// Another problem with measuring trailing spaces means
					// that this doesn't work with a range containing a single space.
					if (sAll[start + i] == ' ')
						characterRanges[i] = new CharacterRange(start, i+1);
					else
						characterRanges[i] = new CharacterRange(start+i, 1);
				}
				sf.SetMeasurableCharacterRanges(characterRanges);
				RectangleF layoutRect = new RectangleF(0.0f, 0.0f, float.MaxValue, float.MaxValue);
				Region[] regions = new Region[sLength];
				regions = g.MeasureCharacterRanges(sAll, fontHandle, layoutRect, sf);

				for (int i = 0; i < sLength; i++) {
					char ch = sAll[start+i];
					float xOfCharRight = (regions[i].GetBounds(g).Right);
					int xPixel = (int)(xOfCharRight);
					positions.Add(xPixel);
					if (ch >= 0x80)
						positions.Add(xPixel);
					if (ch >= 0x800)
						positions.Add(xPixel);
				}
			}
		}
		catch (System.IO.IOException) {
			System.Console.Out.WriteLine("Failed to convert");
		}
	}
	public virtual void MeasureWidths(SplitText st, int position, int len, TextPositions positions) {
		byte[] s = new byte[len * 3];
		int lenUTF8 = st.RetrieveUTF8(position, s, len);
		MeasureWidths(s, lenUTF8, positions, st.GetEncoding());
	}
	public virtual void PaintText(byte[] s, int len, SRectangle rcw, int ascentLine, int enc) {
		try {
			System.String sg = System.Text.Encoding.UTF8.GetString(s, 0, len);
			g.DrawString(sg, fontHandle, textBrush, rcw.left, rcw.top + ascentLine - ascent, sf);
		}
		catch (System.IO.IOException) {
			System.Console.Out.WriteLine("Failed to convert");
		}
	}
	public virtual void PaintText(SplitText st, int position, int len, SRectangle rcw, int ascentLine) {
		try {
			byte[] s = new byte[len * 3];
			int lenUTF8 = st.RetrieveUTF8(position, s, len);
			System.String sg = System.Text.Encoding.UTF8.GetString(s, 0, lenUTF8);
			g.DrawString(sg, fontHandle, textBrush, rcw.left, rcw.top + ascentLine - ascent, sf);
		}
		catch (System.IO.IOException) {
			System.Console.Out.WriteLine("Failed to convert");
		}
	}
	public virtual int TextAscent() {
		FontFamily fontFamily = fontHandle.FontFamily;
		int ascentCell = fontFamily.GetCellAscent(fontHandle.Style);
		int ascentPixel = (int)fontHandle.Size * ascentCell / fontFamily.GetEmHeight(fontHandle.Style);
		return ascentPixel;
	}
	public virtual int TextDescent() {
		FontFamily fontFamily = fontHandle.FontFamily;
		int ascent = fontFamily.GetCellAscent(fontHandle.Style);
		int ascentPixel = (int)fontHandle.Size * ascent / fontFamily.GetEmHeight(fontHandle.Style);
		return (int)fontHandle.GetHeight() - ascentPixel;
	}
	public virtual void SetClip(SRectangle rc) {
		Rectangle clipRect = new Rectangle(
			rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top);
		Region clipRegion = new Region(clipRect);
		g.Clip = clipRegion;
	}
	static Surface() {
		fontDefault = new Literal("Verdana");
	}
	public const int lineExtra = 0;
}

}
