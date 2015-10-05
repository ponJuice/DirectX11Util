#pragma once
#include "Texture.h"

static class FontLoder
{
public:
	static HFONT hFont;
	static HFONT oldFont;
	static HDC hdc;
	static GLYPHMETRICS gm;
	static TEXTMETRIC tm;
	static HWND hwnd;
	static BYTE* bitmapData;
	static Texture tex;
public:
	static bool getCharGraph(TCHAR* c);
	static bool changeFont(LPCWSTR fontName,int fontSize);
	static bool prevFont();
	static bool initFont(HWND hWnd, LPCWSTR fontName, UINT fontSize);
private:
	FontLoder(HWND hwnd,LPCWSTR fontName,UINT fontSize);
	FontLoder();
	~FontLoder();
};

