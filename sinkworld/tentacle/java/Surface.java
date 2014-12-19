package SinkWorld;

import java.awt.*;

class FontCacheEntry {
	public FontDescription font;
	public Font fontHandle;
}

class FontCache {
	public int size;
	public FontCacheEntry []entries;

	FontCache() {
		size = 0x20;
		entries = new FontCacheEntry[size];
	}
	private int Hash(FontDescription font) {
		return font.Hash() & 0x1F;
	}
	Font GetFont(FontDescription font) {
		FontCacheEntry fce = entries[Hash(font)];
		if (fce == null)
			return null;
		else if (fce.font.Equals(font))
			return fce.fontHandle;
		else
			return null;
	}
	void SetFont(FontDescription font, Font fontHandle) {
		int probe = Hash(font);
		entries[probe] = new FontCacheEntry();
		entries[probe].font = font;
		entries[probe].fontHandle = fontHandle;
	}
}

public class Surface {

	public static Literal fontDefault=null;
	static {
		fontDefault = new Literal("Verdana");
	}

	static int totals=0;
	static int misses = 0;

	Graphics g;
	FontDescription font;
	Font fontHandle;
	FontCache fc = new FontCache();
	FontMetrics fm;
	RGBColor currentColor = new RGBColor(0x1000000);
	int x = 0;
	int y = 0;

	Surface(Graphics g_) {
		g = g_;
	}

	public void SetFont(FontDescription font_) {
		if (font != font_) {
			if ((font == null) || (!font.Equals(font_))) {
				// Only create a new font when different from cached
				font = font_;
				fontHandle = fc.GetFont(font);
				totals++;
				if (fontHandle == null) {
					misses++;
					//~ if (misses % 500 == 0) {
						//~ System.err.println("Font Cache total " + totals +
							//~ "  misses " + misses + "  proportion " +
							//~ ((float)(misses) / totals));
					//~ }
					// Not in cache so create
					String stringFont = "Verdana";
					try {
						int maxSize = font.font.Length() * 3;
						byte nameBytes[] = new byte[maxSize];
						int lenUTF8 = UniConversion.UTF8FromUTF32(
							nameBytes, nameBytes.length,
							font.font.Data(), font.font.Length());
						stringFont = new String(nameBytes, 0, lenUTF8, "UTF8");
					} catch (java.io.UnsupportedEncodingException e) {
						System.out.println("Failed to convert");
					} catch (java.lang.NullPointerException e) {
						System.out.println("Null to convert");
					}
					int fs=0;
					if (font.bold)
						fs |= Font.BOLD;
					if (font.italics)
						fs |= Font.ITALIC;
					fontHandle = new Font(stringFont, fs, font.height+2);
					fc.SetFont(font, fontHandle);
				}
				g.setFont(fontHandle);
				fm = null;
			}
		}
	}
	public void SetTextColor(RGBColor color) {
		SetPen(color);
	}
	public void SetPen(RGBColor color) {
		if (currentColor.NotEqual(color)) {
			g.setColor(new java.awt.Color((int)color.GetRed(),
				(int)color.GetGreen(), (int)color.GetBlue()));
			currentColor.co = color.co;
		}
	}
	public void Free() {
	}
	public void FillRectangle(SRectangle rc, RGBColor color) {
		SetPen(color);
		g.fillRect(rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top);
	}
	public void FrameRectangle(SRectangle rc, RGBColor color) {
		SetPen(color);
		g.drawRect(rc.left, rc.top, rc.right - rc.left - 1, rc.bottom - rc.top - 1);
	}
	public void Polygon(Point []pts, int npts, RGBColor fore, int alphaFill, int alphaOutline) {
		// Convert from SinkWorld Point array to parallel int arrays
		Polygon poly = new Polygon();
		for (int i=0; i<npts; i++) {
			poly.addPoint(pts[i].x, pts[i].y);
		}
		g.setColor(new java.awt.Color((int)fore.GetRed(), (int)fore.GetGreen(), (int)fore.GetBlue(), alphaFill));
		g.fillPolygon(poly);
		g.setColor(new java.awt.Color((int)fore.GetRed(), (int)fore.GetGreen(), (int)fore.GetBlue(), alphaOutline));
		g.drawPolygon(poly);
		
		// Ensure synchronized
		g.setColor(new java.awt.Color((int)fore.GetRed(),
			(int)fore.GetGreen(), (int)fore.GetBlue()));
		currentColor.co = fore.co;
	}
	public void MoveTo(int x_, int y_) {
		x = x_;
		y = y_;
	}
	public void LineTo(int x_, int y_) {
		g.drawLine(x, y, x_, y_);
		x = x_;
		y = y_;
	}
	public void SetTransparent(boolean transparent) {
	}
	public void MeasureWidths(byte []s, int len, TextPositions positions, int enc) {
		try {
			String sg = new String(s, 0, len, "UTF8");
			if (fm == null)
				fm = g.getFontMetrics(fontHandle);
			int x = 0;
			for (int i=0;i<sg.length(); i++) {
				char ch = sg.charAt(i);
				//~ if (ch > '\177') {
					//~ System.out.println("Measuring " + Character.getNumericValue(ch));
				//~ }
				x += fm.charWidth(ch);
				positions.Add(x);
				if (ch >= 0x80)
					positions.Add(x);
				if (ch >= 0x800)
					positions.Add(x);
			}
		} catch (java.io.UnsupportedEncodingException e) {
			System.out.println("Failed to convert");
		}
	}
	public void MeasureWidths(SplitText st, int position, int len, TextPositions positions) {
		byte s[] = new byte[len * 3];
		int lenUTF8 = st.RetrieveUTF8(position, s, len);
		MeasureWidths(s, lenUTF8, positions, st.GetEncoding());
	}
	public void PaintText(byte []s, int len, SRectangle rcw, int ascent, int enc) {
		try {
			String sg = new String(s, 0, len, "UTF8");
			g.drawString(sg, rcw.left, rcw.top+ascent);
		} catch (java.io.UnsupportedEncodingException e) {
			System.out.println("Failed to convert");
		}
	}
	public void PaintText(SplitText st, int position, int len, SRectangle rcw, int ascent) {
		try {
			byte s[] = new byte[len * 3];
			int lenUTF8 = st.RetrieveUTF8(position, s, len);
			String sg = new String(s, 0, lenUTF8, "UTF8");
			g.drawString(sg, rcw.left, rcw.top+ascent);
		} catch (java.io.UnsupportedEncodingException e) {
			System.out.println("Failed to convert");
		}
	}
	public int TextAscent() {
		if (fm == null)
			fm = g.getFontMetrics(fontHandle);
		return fm.getAscent();
	}
	public int TextDescent() {
		if (fm == null)
			fm = g.getFontMetrics(fontHandle);
		return fm.getDescent();
		//~ return fm.getLeading() + fm.getDescent();
	}
	void SetClip(SRectangle rc) {
		g.setClip(rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top);
	}
	public static final int lineExtra = 0;
};

