#include "Texture.h"


Texture::Texture()
{
}


Texture::~Texture()
{
	OutputDebugString(L"テクスチャの終了処理を開始します\n");
	releaseTexture();
	OutputDebugString(L"完了\n");
}

void Texture::releaseTexture(){
	SAFE_RELEASE(textrue);
	SAFE_RELEASE(shaerResourceView);
	SAFE_RELEASE(dynamicsTexture);
}
