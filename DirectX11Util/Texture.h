#pragma once
#include "Terminal.h"

class Texture
{
public:
	Texture();
	~Texture();
public:
	ID3D11Resource* textrue;
	ID3D11ShaderResourceView* shaerResourceView;
	ID3D11Texture2D* dynamicsTexture;
	int width;
	int height;
	void releaseTexture();
	//virtual void drawTexture();
};

