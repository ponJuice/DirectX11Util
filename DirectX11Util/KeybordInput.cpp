#include "KeybordInput.h"

BYTE* KeybordInput::getKey(){
	return key;
}

BOOL KeybordInput::getKeyState(){
	return GetKeyboardState(key);
}

KeybordInput::KeybordInput()
{
}


KeybordInput::~KeybordInput()
{
}

BYTE KeybordInput::key[256];