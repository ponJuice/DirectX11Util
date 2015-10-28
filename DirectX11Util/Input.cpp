#include "Input.h"

void Input::init(HWND* wnd, MSG* msg){
	Input::msg = msg;
	Input::hWnd = wnd;
}

Input::Input()
{
}


Input::~Input()
{
}

HWND*	Input::hWnd;
POINT	Input::pt;
MSG*	Input::msg;
