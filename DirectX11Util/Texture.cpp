#include "Texture.h"


Texture::Texture()
{
}


Texture::~Texture()
{
	OutputDebugString(L"�e�N�X�`���̏I���������J�n���܂�\n");
	releaseTexture();
	OutputDebugString(L"����\n");
}

void Texture::releaseTexture(){
	SAFE_RELEASE(textrue);
	SAFE_RELEASE(shaerResourceView);
	SAFE_RELEASE(dynamicsTexture);
}
