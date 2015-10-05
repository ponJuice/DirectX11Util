#pragma once
#include "Input.h"

class KeybordInput : public Input
{
private:
	static BYTE key[256];
public:
	static BOOL getKeyState();
	static BYTE* getKey();
protected:
	KeybordInput();
	~KeybordInput();
};

