#include <string.h>
#include <assert.h>
#include <string>

#ifdef unix
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include <gdk-pixbuf/gdk-pixdata.h>
#else
#include <windows.h>
#include <windowsx.h>
#endif

#include "base.h"

#include "RGBColor.h"
#include "FontDescription.h"
#include "Surface.h"

int Point::DistanceSquared(const Point &other) const {
	int xdelta = other.x - x;
	int ydelta = other.y - y;
	return xdelta * xdelta + ydelta * ydelta;
}

Literal *Surface::fontDefault = NULL;

void Surface::Init() {
#ifdef unix
	fontDefault = new Literal("Sans");
#else
	fontDefault = new Literal("Verdana");
#endif
}

void Surface::Finalize() {
	delete fontDefault;
	fontDefault = NULL;
}

#ifdef unix
typedef void *HDC;

/// Private GTK+-specific implementation of graphical operations used by Surface.

class SurfaceDetails {
public:
	GdkGC *gc;
	PangoContext *pcontext;
	PangoLayout *layout;
	PangoFontDescription *pfd;
	int x;
	int y;
	FontDescription fontCurrent;
	RGBColor colorCurrent;
	ByteBuffer sText;
	int ascent;
	SurfaceDetails() : gc(0), pcontext(0), layout(0), pfd(0), x(0), y(0),
		colorCurrent(0x10000000), ascent(1) {
	}
	~SurfaceDetails() {
		Free(0);
	}
	void Free(void *) {
		if (gc)
			gdk_gc_unref(gc);
		gc = 0;
		if (pfd)
			pango_font_description_free(pfd);
		pfd = 0;
		if (layout)
			g_object_unref(layout);
		layout = 0;
		if (pcontext)
			g_object_unref(pcontext);
		pcontext = 0;
	}
};

Surface::Surface(void *hdc_, void *wid) :
	hdc(hdc_) {
	details = new SurfaceDetails;
	details->gc = gdk_gc_new(reinterpret_cast<GdkDrawable *>(hdc));
	details->pcontext = gtk_widget_create_pango_context(static_cast<GtkWidget *>(wid));
	details->layout = pango_layout_new(details->pcontext);
	// Ask for lines that do not paint the last pixel so is like Win32
	gdk_gc_set_line_attributes(details->gc, 0, GDK_LINE_SOLID, GDK_CAP_NOT_LAST, GDK_JOIN_MITER);
}
#else

/// Private Windows-specific implementation of graphical operations used by Surface.

class SurfaceDetails {
public:
	int logPixelsY;
	HFONT font;
	HFONT fontOld;
	HPEN pen;
	HPEN penOld;
	HBITMAP hbm;
	HBITMAP hbmOld;
	wchar_t *tbuf;
	int tbufSize;
	int *poses;
	int posesSize;
	FontDescription fontCurrent;
	RGBColor colorCurrent;
	ByteBuffer sWide;
	int ascent;
	SurfaceDetails() :
		logPixelsY(1), font(0), fontOld(0), pen(0), penOld(0),
		hbm(0), hbmOld(0),
		tbuf(0), tbufSize(0), poses(0), posesSize(0),
		colorCurrent(0x10000000), ascent(1) {
	}
	void Free(HDC hdc);
	void EnsureWideBufferSize(int len);
	void EnsurePositionsSize(int tlen);
};

void SurfaceDetails::Free(HDC hdc) {
	if (fontOld) {
		SelectFont(hdc, fontOld);
		fontOld = 0;
		if (font) {
			::DeleteObject(font);
			font = 0;
		}
	}
	if (penOld) {
		::SelectObject(hdc, penOld);
		penOld = 0;
		if (pen) {
			::DeleteObject(pen);
			pen = 0;
		}
	}
	if (hbmOld) {
		SelectBitmap(hdc, hbmOld);
		hbmOld = 0;
		if (hbm) {
			::DeleteObject(hbm);
			hbm = 0;
		}
	}
	if (hdc) {
		DeleteDC(hdc);
		hdc = 0;
	}
	delete []tbuf;
	tbuf = 0;
	tbufSize = 0;
	delete []poses;
	poses = 0;
	posesSize = 0;
}

void SurfaceDetails::EnsureWideBufferSize(int len) {
	if (tbufSize < len+1) {
		delete []tbuf;
		tbufSize = len+1;
		tbuf = new wchar_t[tbufSize];
	}
}

void SurfaceDetails::EnsurePositionsSize(int tlen) {
	if (posesSize < tlen+1) {
		delete []poses;
		posesSize = tlen+1;
		poses = new int[posesSize];
	}
}

Surface::Surface(void *hdc_, void *) :
	details(0), hdc(hdc_) {
	details = new SurfaceDetails;
	details->logPixelsY = ::GetDeviceCaps(reinterpret_cast<HDC>(hdc), LOGPIXELSY);
}
#endif
Surface::~Surface() {
	details->Free(reinterpret_cast<HDC>(hdc));
	delete details;
	details = 0;
}

void Surface::Free() {
	details->Free(reinterpret_cast<HDC>(hdc));
	hdc = 0;
}

#ifdef unix

void Surface::SetFont(const FontDescription &font_) {
	if (!(details->fontCurrent.Equals(font_))) {
		if (details->pfd)
			pango_font_description_free(details->pfd);
		details->pfd = pango_font_description_new();
		//fprintf(stderr, "Set font %s %d\n", font_.font->Value(), font_.height);
		char *fontFamily = (char *)font_.font->AsUTF8();
		pango_font_description_set_family(details->pfd, fontFamily);
		delete []fontFamily;
		assert(font_.height > 0);
		pango_font_description_set_size(details->pfd,
			font_.height * PANGO_SCALE);
		pango_font_description_set_weight(details->pfd,
			font_.bold ? PANGO_WEIGHT_BOLD : PANGO_WEIGHT_NORMAL);
		pango_font_description_set_style(details->pfd,
			font_.italics ? PANGO_STYLE_ITALIC : PANGO_STYLE_NORMAL);
		details->fontCurrent = font_;
		details->ascent = TextAscent();
		pango_layout_set_font_description(details->layout, details->pfd);
	}
}

void Surface::SetTextColor(RGBColor color) {
	SetPen(color);
}

void Surface::SetPen(RGBColor color) {
	if (details->colorCurrent.NotEqual(color)) {
		if (details->gc) {
			GdkColor co;
			co.pixel = color.AsRGB();
			gdk_gc_set_foreground(details->gc, &co);
			details->colorCurrent = color;
		}
	}
}

void Surface::FillRectangle(SRectangle rc, RGBColor color) {
	SetPen(color);
	gdk_draw_rectangle(reinterpret_cast<GdkDrawable *>(hdc), details->gc, 1,
		rc.left, rc.top,
		rc.right - rc.left, rc.bottom - rc.top);
}

void Surface::FrameRectangle(SRectangle rc, RGBColor color) {
	SetPen(color);
	gdk_draw_rectangle(reinterpret_cast<GdkDrawable *>(hdc), details->gc, 0,
		rc.left, rc.top,
		rc.right - rc.left - 1, rc.bottom - rc.top - 1);
}

void Surface::Polygon(Point *pts, int npts, RGBColor fore, int alphaFill, int alphaOutline) {
	int minX = pts[0].x;
	int maxX = pts[0].x;
	int minY = pts[0].y;
	int maxY = pts[0].y;
	for (int i=1; i<npts; i++) {
		if (minX > pts[i].x)
			minX = pts[i].x;
		if (maxX < pts[i].x)
			maxX = pts[i].x;
		if (minY > pts[i].y)
			minY = pts[i].y;
		if (maxY < pts[i].y)
			maxY = pts[i].y;
	}
	GdkPoint gpts[npts];
	for (int j=0; j<npts; j++) {
		gpts[j].x = pts[j].x - minX;
		gpts[j].y = pts[j].y - minY;
	}
	int width = maxX - minX + 1;
	int height = maxY - minY + 1;
	GdkPixmap *ppixmap = gdk_pixmap_new(reinterpret_cast<GdkDrawable *>(hdc), width, height, -1);
	GdkGC *pixgc = gdk_gc_new(reinterpret_cast<GdkDrawable *>(hdc));
	
	// Draw the polygon in gray scale using the two alphas
	GdkColor black;
	black.pixel = 0;
	gdk_gc_set_foreground(pixgc, &black);
	gdk_draw_rectangle(ppixmap, pixgc, 1, 0, 0, width, height);
	GdkColor fill;
	fill.pixel = alphaFill * 256 * 256 + alphaFill * 256 + alphaFill;
	gdk_gc_set_foreground(pixgc, &fill);
	gdk_draw_polygon(reinterpret_cast<GdkDrawable *>(ppixmap), pixgc, 1, gpts, npts);
	GdkColor outline;
	outline.pixel = alphaOutline * 256 * 256 + alphaOutline * 256 + alphaOutline;
	gdk_gc_set_foreground(pixgc, &outline);
	gdk_draw_polygon(reinterpret_cast<GdkDrawable *>(ppixmap), pixgc, 0, gpts, npts);
	
	// Grab the pixmap into a default (probably 24 bit) pixbuf
	// This may involve copying the bits from the server
	GdkColormap *cmap = gdk_colormap_get_system();
	GdkPixbuf *pixbuf = gdk_pixbuf_get_from_drawable(NULL, ppixmap,
		cmap, 0,0, 0,0, width,height);

	// Now make a 32 bit deep pixbuf with alpha
	GdkPixbuf *pixalpha = gdk_pixbuf_new(GDK_COLORSPACE_RGB, TRUE, 8, width, height);
	// Copy from the default pixbuf into one with alpha
	gdk_pixbuf_scale(pixbuf, pixalpha, 0,0, width,height, 0.0,0.0, 1.0,1.0, GDK_INTERP_NEAREST);
	
	// Access the bits of the pixbuf
	// Loop for every pixel, setting the alpha as drawn in the gray scale and copying in the 
	unsigned char red = fore.GetRed();
	unsigned char green = fore.GetGreen();
	unsigned char blue = fore.GetBlue();
	for (int yy=0; yy<height; yy++) {
		unsigned char *pdata = gdk_pixbuf_get_pixels(pixalpha) + yy * gdk_pixbuf_get_rowstride(pixalpha);
		for (int xx=0; xx<width; xx++) {
			pdata[4 * xx + 3] = pdata[4 * xx + 0];	// Alpha
			pdata[4 * xx + 0] = red;
			pdata[4 * xx + 1] = green;
			pdata[4 * xx + 2] = blue;
		}
	}

	// Draw with alpha
	gdk_draw_pixbuf(reinterpret_cast<GdkDrawable *>(hdc), details->gc, pixalpha,
		0,0, minX,minY, width,height, GDK_RGB_DITHER_NORMAL, 0, 0);
	
	g_object_unref(pixgc);
	g_object_unref(ppixmap);
	g_object_unref(pixbuf);
	g_object_unref(pixalpha);
}

void Surface::MoveTo(int x_, int y_) {
	details->x = x_;
	details->y = y_;
}

void Surface::LineTo(int x_, int y_) {
	if (hdc && details->gc) {
		gdk_draw_line(reinterpret_cast<GdkDrawable *>(hdc), details->gc, details->x, details->y, x_, y_);
	}
	details->x = x_;
	details->y = y_;
}

void Surface::SetTransparent(bool) {
}

// This code will not return sensible interanl positions for bidirectional text
// although the overall width is correct
void Surface::MeasureWidths(SW_BYTE *s, int len, TextPositions *positions, int enc) {
	pango_layout_set_text(details->layout, reinterpret_cast<char *>(s), len);
	PangoRectangle prLayout;
	PangoLayoutIter *iter = pango_layout_get_iter(details->layout);
	int sIndex = 0;
	while (pango_layout_iter_next_cluster(iter)) {
		pango_layout_iter_get_cluster_extents(iter, NULL, &prLayout);
		int nextIndex = pango_layout_iter_get_index(iter);
		while (sIndex<nextIndex) {
			positions->Add(PANGO_PIXELS(prLayout.x));
			sIndex++;
		}
	}
	pango_layout_iter_free(iter);
}

void Surface::MeasureWidths(SplitText *s, int position, int len, TextPositions *positions) {
	details->sText.Resize(len+1);
	int tlen = s->RetrieveUTF8(position, details->sText.Bytes(), len);
	MeasureWidths(details->sText.Bytes(), tlen, positions, s->GetEncoding());
}

void Surface::PaintText(SW_BYTE *s, int len, SRectangle *rcw, int ascentLine, int enc) {
	pango_layout_set_text(details->layout, reinterpret_cast<char *>(s), len);
	PangoLayoutLine *pll = pango_layout_get_line(details->layout, 0);
	gdk_draw_layout_line(reinterpret_cast<GdkDrawable *>(hdc), details->gc, rcw->left, 
		rcw->top + ascentLine - (ascentLine - details->ascent), pll);
}

void Surface::PaintText(SplitText *s, int position, int len, SRectangle *rcw, int ascentLine) {
	details->sText.Resize(len+1);
	int tlen = s->RetrieveUTF8(position, details->sText.Bytes(), len);
	pango_layout_set_text(details->layout, details->sText.AsCharPointer(), tlen);
	PangoLayoutLine *pll = pango_layout_get_line(details->layout, 0);
	gdk_draw_layout_line(reinterpret_cast<GdkDrawable *>(hdc), details->gc, rcw->left, 
		rcw->top + ascentLine - (ascentLine - details->ascent), pll);
}

int Surface::TextAscent() {
	PangoFontMetrics *metrics = pango_context_get_metrics(details->pcontext,
		details->pfd, pango_context_get_language(details->pcontext));
	int ascent = PANGO_PIXELS(pango_font_metrics_get_ascent(metrics));
	pango_font_metrics_unref(metrics);
	return ascent;
}

int Surface::TextDescent() {
	PangoFontMetrics *metrics = pango_context_get_metrics(details->pcontext,
		details->pfd, pango_context_get_language(details->pcontext));
	int descent = PANGO_PIXELS(pango_font_metrics_get_descent(metrics));
	pango_font_metrics_unref(metrics);
	return descent;
}


void Surface::SetClip(SRectangle *rc) {
	GdkRectangle area = {rc->left, rc->top,
	                     rc->right - rc->left, rc->bottom - rc->top};
	gdk_gc_set_clip_rectangle(details->gc, &area);
}

#else

void Surface::SetFont(const FontDescription &font_) {
	if (!(details->fontCurrent.Equals(font_))) {
		LOGFONT lf;
		memset(&lf, 0, sizeof(lf));
		lf.lfHeight = -::MulDiv(font_.height, details->logPixelsY, 72);
		lf.lfWeight = font_.bold ? FW_BOLD : FW_NORMAL;
		lf.lfItalic = font_.italics ? 1 : 0;
		lf.lfCharSet = 0;
		int lenName = UniConversion::UTF8FromUTF32(
			reinterpret_cast<SW_BYTE *>(lf.lfFaceName), sizeof(lf.lfFaceName),
			font_.font->Data(), font_.font->Length());
		HFONT hfont = CreateFontIndirect(&lf);
		if (details->fontOld) {
			SelectFont(reinterpret_cast<HDC>(hdc), hfont);
		} else {
			details->fontOld = SelectFont(reinterpret_cast<HDC>(hdc), hfont);
		}
		if (details->font) {
			::DeleteObject(details->font);
			details->font = 0;
		}
		details->font = hfont;
		details->fontCurrent = font_;
		details->ascent = TextAscent();
	}
}

void Surface::SetTextColor(RGBColor color) {
	if (details->colorCurrent.NotEqual(color)) {
		::SetTextColor(reinterpret_cast<HDC>(hdc), color.AsBGR());
		details->colorCurrent = color;
	}
}

void Surface::SetPen(RGBColor color) {
	HPEN pen_ = ::CreatePen(0,1,color.AsBGR());
	if (details->penOld) {
		SelectPen(reinterpret_cast<HDC>(hdc), pen_);
	} else {
		details->penOld = SelectPen(reinterpret_cast<HDC>(hdc), pen_);
	}
	if (details->pen) {
		::DeleteObject(details->pen);
	}
	details->pen = pen_;
}

void Surface::SetBitmap(void *hbm_) {
	HBITMAP hbmCast = reinterpret_cast<HBITMAP>(hbm_);
	if (details->hbmOld) {
		SelectBitmap(reinterpret_cast<HDC>(hdc), hbmCast);
	} else {
		details->hbmOld = SelectBitmap(reinterpret_cast<HDC>(hdc), hbmCast);
	}
	if (details->hbm) {
		::DeleteObject(details->hbm);
	}
	details->hbm = hbmCast;
}

void Surface::FillRectangle(SRectangle rc, RGBColor color) {
	::SetBkColor(reinterpret_cast<HDC>(hdc), color.AsBGR());
	RECT *rectw = reinterpret_cast<RECT *>(&rc);
	::ExtTextOut(reinterpret_cast<HDC>(hdc), rc.left, rc.top, ETO_OPAQUE, rectw, "", 0, NULL);
}

void Surface::FrameRectangle(SRectangle rc, RGBColor color) {
	HBRUSH brush = ::CreateSolidBrush(color.AsBGR());
	RECT *rectw = reinterpret_cast<RECT *>(&rc);
	::FrameRect(reinterpret_cast<HDC>(hdc), rectw, brush);
	::DeleteObject(brush);
}

void Surface::Polygon(Point *pts, int npts, RGBColor fore, int alphaFill, int alphaOutline) {
	HDC hMemDC = ::CreateCompatibleDC(reinterpret_cast<HDC>(hdc));
	int minX = pts[0].x;
	int maxX = pts[0].x;
	int minY = pts[0].y;
	int maxY = pts[0].y;
	for (int i=1; i<npts; i++) {
		if (minX > pts[i].x)
			minX = pts[i].x;
		if (maxX < pts[i].x)
			maxX = pts[i].x;
		if (minY > pts[i].y)
			minY = pts[i].y;
		if (maxY < pts[i].y)
			maxY = pts[i].y;
	}
	for (int j=0; j<npts; j++) {
		pts[j].x -= minX;
		pts[j].y -= minY;
	}
	int width = maxX - minX + 1;
	int height = maxY - minY + 1;
	BITMAPINFO bpih = {sizeof(BITMAPINFOHEADER), width, height, 1, 32, BI_RGB, 0, 0, 0, 0, 0};
	void *image = 0;
	HBITMAP hbmMem = CreateDIBSection(reinterpret_cast<HDC>(hMemDC), &bpih, 
		DIB_RGB_COLORS, &image, NULL, 0);

	HBITMAP hbmOld = SelectBitmap(hMemDC, hbmMem);

	::SetBkColor(reinterpret_cast<HDC>(hMemDC), RGB(0,0,0));
	RECT rcw = {0,0,width,height};
	::ExtTextOut(reinterpret_cast<HDC>(hMemDC), 0, 0, ETO_OPAQUE, &rcw, "", 0, NULL);

	// Draw first in gray scale
	HPEN penOutline = ::CreatePen(0,1,RGB(alphaOutline, alphaOutline, alphaOutline));
	HPEN penOld = static_cast<HPEN>(::SelectObject(reinterpret_cast<HDC>(hMemDC), penOutline));
	HBRUSH brushGray = ::CreateSolidBrush(RGB(alphaFill, alphaFill, alphaFill));
	HBRUSH brushOld = static_cast<HBRUSH>(::SelectObject(reinterpret_cast<HDC>(hMemDC), brushGray));

	::Polygon(reinterpret_cast<HDC>(hMemDC), reinterpret_cast<POINT *>(pts), npts);

	::SelectObject(reinterpret_cast<HDC>(hMemDC), brushOld);
	::DeleteObject(brushGray);
	::SelectObject(reinterpret_cast<HDC>(hMemDC), penOld);
	::DeleteObject(penOutline);

	// Convert grey scale to alpha then draw real color
	for (int y=0; y<height; y++) {
		for (int x=0; x<width; x++) {
			BYTE *pixel = reinterpret_cast<BYTE *>(image) + (y * width + x) * 4;
			pixel[3] = pixel[0];
			if (pixel[3] != 0) {
				pixel[0] = static_cast<BYTE>(fore.GetBlue() * pixel[3] / 255);
				pixel[1] = static_cast<BYTE>(fore.GetGreen() * pixel[3] / 255);
				pixel[2] = static_cast<BYTE>(fore.GetRed() * pixel[3] / 255);
			}
		}
	}

	//BLENDFUNCTION merge = { AC_SRC_OVER, 0, 192, 0};
	BLENDFUNCTION merge = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };

	::AlphaBlend(reinterpret_cast<HDC>(hdc), minX, minY, width, height, hMemDC, 0, 0, width, height, merge);
	//::BitBlt(reinterpret_cast<HDC>(hdc), minX, minY, width, height, hMemDC, 0, 0, SRCCOPY);

	SelectBitmap(hMemDC, hbmOld);
	::DeleteObject(hbmMem);
	::DeleteObject(hMemDC);
}

void Surface::MoveTo(int x_, int y_) {
	::MoveToEx(reinterpret_cast<HDC>(hdc), x_, y_, 0);
}

void Surface::LineTo(int x_, int y_) {
	::LineTo(reinterpret_cast<HDC>(hdc), x_, y_);
}

void Surface::SetTransparent(bool transparent) {
	::SetBkMode(reinterpret_cast<HDC>(hdc), transparent ? TRANSPARENT : OPAQUE);
}

void Surface::MeasureWidths(SW_BYTE *s, int len, TextPositions *positions, int enc) {
	int endEntry = positions->Last() + len;
	details->EnsureWideBufferSize(len);

	int tlen = 0;
	if (enc == SplitText::ENCODING_UTF16LE) {
		memcpy(details->tbuf,s,len);
		tlen = len / 2;
	} else if (enc == SplitText::ENCODING_UTF16BE) {
		swab(reinterpret_cast<char *>(s), reinterpret_cast<char *>(details->tbuf),len);
		tlen = len / 2;
	} else {
		tlen = UniConversion::UTF16FromUTF8(reinterpret_cast<short *>(details->tbuf), len, s, len);
	}
	details->tbuf[tlen] = L'\0';
	details->EnsurePositionsSize(tlen);
	int fit = tlen;
	const int maxWidthMeasure = 1000000;
	SIZE sz={0,0};
	int lastPosition = positions->LastPosition();
	// Using GetTextExtentPoint to only get total width is about 30% faster
	::GetTextExtentExPointW(reinterpret_cast<HDC>(hdc), details->tbuf, tlen,
		maxWidthMeasure, &fit, details->poses, &sz);
	//::GetTextExtentPointW(reinterpret_cast<HDC>(hdc), tbuf, tlen, &sz);
	for (int k=0; k<tlen; k++)
		//details->poses[k] = lastPosition;
		details->poses[k] += lastPosition;
	//details->poses[tlen-1] += sz.cx;
	// Map the widths given for UCS-2 characters back onto the UTF-8 input string
	int ui=0;
	const unsigned char *us = reinterpret_cast<const unsigned char *>(s);
	while (ui<tlen) {
		if ((enc == SplitText::ENCODING_UTF16LE) || (enc == SplitText::ENCODING_UTF16BE)) {
			positions->Add(details->poses[ui]);
			positions->Add(details->poses[ui]);
			us += 2;
		} else {
			unsigned char uch = *us++;
			positions->Add(details->poses[ui]);
			if (uch >= 0x80) {
				us++;
				positions->Add(details->poses[ui]);
				if (uch >= (0x80 + 0x40 + 0x20)) {
					us++;
					positions->Add(details->poses[ui]);
				}
			}
		}
		ui++;
	}
	positions->FillTo(endEntry);
}

void Surface::MeasureWidths(SplitText *s, int position, int len, TextPositions *positions) {
	int endEntry = positions->Last() + len;
	details->EnsureWideBufferSize(len);

	int tlen = s->RetrieveUTF16(position, reinterpret_cast<short *>(details->tbuf), len);
	details->tbuf[tlen] = L'\0';
	details->EnsurePositionsSize(tlen);
	int fit = tlen;
	const int maxWidthMeasure = 1000000;
	SIZE sz={0,0};
	int lastPosition = positions->LastPosition();
	::GetTextExtentExPointW(reinterpret_cast<HDC>(hdc), details->tbuf, tlen,
		maxWidthMeasure, &fit, details->poses, &sz);
	for (int k=0; k<tlen; k++)
		details->poses[k] += lastPosition;
	// Map the widths given for UTF-16 characters back onto the byte array document
	int posBuffer = position;
	for (int ui=0; ui<tlen; ui++) {
		int lenChar = s->LenChar(posBuffer);
		for (int i=0;i<lenChar;i++) {
			positions->Add(details->poses[ui]);
		}
		posBuffer += s->LenChar(posBuffer);
	}
	positions->FillTo(endEntry);
}

void Surface::PaintText(SW_BYTE *s, int len, SRectangle *rcw, int ascentLine, int enc) {
	RECT *rectw = reinterpret_cast<RECT *>(rcw);
	if (enc == SplitText::ENCODING_UTF16LE) {
		::ExtTextOutW(reinterpret_cast<HDC>(hdc), rcw->left, rcw->top + ascentLine - details->ascent, 0, rectw,
			reinterpret_cast<wchar_t *>(s), len/2, NULL);
	} else if (enc == SplitText::ENCODING_UTF16BE) {
		details->sWide.Resize(len+1);
		swab(reinterpret_cast<char *>(s),details->sWide.AsCharPointer(),len);
		::ExtTextOutW(reinterpret_cast<HDC>(hdc), rcw->left, rcw->top + ascentLine - details->ascent, 0, rectw,
			details->sWide.AsWCharTPointer(), len/2, NULL);
	} else {
		details->sWide.Resize(sizeof(wchar_t) * len+1);
		int tlen = UniConversion::UTF16FromUTF8(
			reinterpret_cast<short *>(details->sWide.AsWCharTPointer()), len,
			s, len);
		::ExtTextOutW(reinterpret_cast<HDC>(hdc), rcw->left, rcw->top + ascentLine - details->ascent, 0, rectw,
			details->sWide.AsWCharTPointer(), tlen, NULL);
	}
}

void Surface::PaintText(SplitText *s, int position, int len, SRectangle *rcw, int ascentLine) {
	RECT *rectw = reinterpret_cast<RECT *>(rcw);
	details->sWide.Resize(sizeof(wchar_t) * len+1);
	int tlen = s->RetrieveUTF16(position, reinterpret_cast<short *>(details->sWide.AsWCharTPointer()), len);
	::ExtTextOutW(reinterpret_cast<HDC>(hdc), rcw->left, rcw->top + ascentLine - details->ascent, 0, rectw,
		details->sWide.AsWCharTPointer(), tlen, NULL);
}

int Surface::TextAscent() {
	TEXTMETRIC tm;
	::GetTextMetrics(reinterpret_cast<HDC>(hdc), &tm);
	return tm.tmAscent;
}

int Surface::TextDescent() {
	TEXTMETRIC tm;
	::GetTextMetrics(reinterpret_cast<HDC>(hdc), &tm);
	return tm.tmDescent;
}

void Surface::SetClip(SRectangle *rc) {
	RECT rcRegion = {rc->left, rc->top, rc->right, rc->bottom};
	HRGN hrgn = ::CreateRectRgnIndirect(&rcRegion);
	SelectClipRgn(reinterpret_cast<HDC>(hdc), hrgn);
	::DeleteObject(hrgn);
	//::IntersectClipRect(reinterpret_cast<HDC>(hdc), rc->left, rc->top, rc->right, rc->bottom);
}

#endif
