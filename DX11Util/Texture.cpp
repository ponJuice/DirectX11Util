#include "Texture.h"


Texture::Texture()
{
}


Texture::~Texture()
{

}

void Texture::releaseTexture(){
	SAFE_RELEASE(textrue);
	SAFE_RELEASE(shaerResourceView);
	SAFE_RELEASE(dynamicsTexture);
}
