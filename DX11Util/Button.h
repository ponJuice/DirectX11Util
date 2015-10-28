#pragma once
#include "Terminal.h"
/**************�{�^���^�C�v*****************
BUTTON_TYPE_NONE : �N���b�N���Ă����̕ω����Ȃ�
BUTTON_TYPE_CLICK: �N���b�N����ƕ\�����ς��
BUTTON_TYPE_HOVER: �|�C���^�[���z�o�[����ƕ\�����ς��

*******************************************/
enum BUTTON_TYPE{BUTTON_TYPE_NONE,BUTTON_TYPE_CLICK,BUTTON_TYPE_HOVER};

class D3DUtil;
class FontLoder;

class Button
{
	Texture* tex;
	int bt;
	int X;
	int Y;
	float scX;
	float scY;
	D3DUtil* d3dutil;
	FontLoder* font;

public:
	void drawButton();
	void drawButton(int x,int y);
	bool clickCheck(int x,int y);
	Button(D3DUtil* d3dUtil,FontLoder* Font,LPWSTR imageName, int x, int y, float scaleX, float scaleY,BUTTON_TYPE bottonType);
	~Button();
};

