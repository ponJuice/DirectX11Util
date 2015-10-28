#include "MouseInput.h"


MouseInput::MouseInput()
{
}


MouseInput::~MouseInput()
{
}
int MouseInput::getX(){
	pt = msg->pt;
	if (!ScreenToClient(*hWnd,&pt)){
		return -1;
	}
	return pt.x;
}

int MouseInput::getY(){
	pt = msg->pt;
	if (!ScreenToClient(*hWnd,&pt)){
		return -1;
	}
	return DX11Util::getWindowY()-(pt.y);
}

bool MouseInput::isClick(){
	if (msg->message == WM_LBUTTONDOWN){
		return true;
	}
	return false;
}