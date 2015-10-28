#pragma once
#include "DX11Util.h"

class D2DUtil : public DX11Util
{
private:
	//ポリゴンのセンター位置
	static XMMATRIX					centor;
	static XMMATRIX					reverce;

public:
	/*******D2DUtil初期化******/
	static bool initD2DUtil(HWND* targetWindow, float l_windowWidth, float l_windowHeight, bool fullScreen);
	/*******画像ロード系*******/
	static bool loadTextrue(LPWSTR fileName, Texture* tex);
	static bool loadFontTextrue(LPWSTR fileName, Texture* tex);
	static bool loadFontTexture(Texture* tex);
	/*******描画系*******/
	static bool drawGraph(Texture* tex, float x, float y, float scaleX, float scaleY,DX11UTIL_BLEND blend);
	static bool drawGraph(Texture* tex, float x, float y, float scaleX, float scaleY,DX11UTIL_BLEND blend, float angle);
	static bool drawGraph(Texture* tex, float x, float y, float scaleX, float scaleY,DX11UTIL_BLEND blend, float angle, float px, float py);
	static bool drawGraphAbsolutely(Texture* tex, float x, float y, float scaleX, float scaleY, float angle, DX11UTIL_BLEND blend);
	static bool drawString(float x, float y, float scaleX, float scaleY, TCHAR* str, UINT size);
	static bool drawString(float x, float y, float scaleX, float scaleY, std::wstring str);
	static bool drawGraphAnimate(Texture* tex, float x, float y, float scaleX, float scaleY, float su, float sv, float eu, float ev);
	/*******フォント系*******/
	static bool changeFont(LPCWSTR fontName,UINT fontSize);
protected:
	D2DUtil();
	~D2DUtil();
};

