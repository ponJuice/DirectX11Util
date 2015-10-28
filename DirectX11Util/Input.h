#pragma once
#include "Terminal.h"
class Input
{
protected:
	static HWND* hWnd;
	static POINT pt;
	static MSG* msg;
public:
	static void init(HWND* wnd, MSG* msg);
protected:
	Input();
	~Input();
};

