#include "Button.h"


Button::Button(D3DUtil* d3dUtil,FontLoder* Font,LPWSTR imageName,int x,int y,float scaleX,float scaleY,BUTTON_TYPE buttonType)
{
	tex = new Texture();
	d3dUtil->loadTextrue(imageName, tex);
	scX = scaleX;
	scY = scaleY;
	X = x;
	Y = y;
	bt = buttonType;
	d3dutil = d3dUtil;
	font = Font;
}

Button::~Button()
{
	delete tex;
}

void Button::drawButton(){
	//drawButton(X, Y);
	d3dutil->drawGraph(tex, X, Y, 1, 1);
}

void Button::drawButton(int x, int y){
	d3dutil->drawGraphAnimate(tex, x, y, scX, scY,0.0,0.0,1.0,1.0);
}

bool Button::clickCheck(int x,int y){
	if (X <= x && Y >= y && (X + tex->width) >= x && (Y - tex->height)<= y){
		d3dutil->drawString(0, -300,0.5f,0.5f, L"CLICK!!");
	}
	return true;
}