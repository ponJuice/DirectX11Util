#pragma once
#include "Input.h"
#include "DX11Util.h"
class MouseInput : public Input
{
public:
	static int getX();
	static int getY();
	static bool isClick();
private:
	MouseInput();
	~MouseInput();
};

