#include "D2DUtil.h"

bool D2DUtil::initD2DUtil(HWND* targetWindow, float l_windowWidth, float l_windowHeight, bool fullScreen){
	OutputDebugString(L"D2DUtil�̏��������J�n���܂�\n");
	bool result = initDX11Util(targetWindow, l_windowWidth, l_windowHeight, fullScreen);
	centor = XMMatrixTranslation(-0.5f, -0.5f, .0f);
	reverce = XMMatrixTranslation(0.5f, 0.5f,.0f);
	return result;
}

bool D2DUtil::loadTextrue(LPWSTR fileName, Texture* tex){
	/***************************************/
	/*�@�@�@�e�N�X�`���ǂݍ���             */
	/***************************************/
	D3DX11_IMAGE_INFO imageInfo;	//�����󂯎��ϐ�
	HRESULT hr;
	hr = D3DX11GetImageInfoFromFile(fileName, NULL, &imageInfo, &hr);
	if (FAILED(hr)){
		OutputDebugString(L"�摜���̎擾�Ɏ��s");
		return false;
	}

	//�e�N�X�`���N���X�ɉ摜�̑傫����ۑ�
	tex->width = imageInfo.Width;
	tex->height = imageInfo.Height;

	//�쐬����e�N�X�`���̐ݒ�
	D3DX11_IMAGE_LOAD_INFO imageLoadInfo;
	imageLoadInfo.Width = imageInfo.Width;	//�e�N�X�`���̕�
	imageLoadInfo.Height = imageInfo.Height;	//�e�N�X�`���̍���
	imageLoadInfo.Depth = 0;	//�e�N�X�`���̐[��
	imageLoadInfo.FirstMipLevel = 0;	//�ŏ��ɓǂݍ��ރ~�b�v�}�b�v���x��
	imageLoadInfo.MipLevels = 8;		//�~�b�v�}�b�v���x���̐�
	imageLoadInfo.Usage = D3D11_USAGE_DEFAULT;	//�f�t�H���g�g�p�@
	imageLoadInfo.BindFlags = D3D11_BIND_SHADER_RESOURCE;	//�V�F�[�_���\�[�X
	imageLoadInfo.CpuAccessFlags = 0;	//CPU����A�N�Z�X���Ȃ�
	imageLoadInfo.MiscFlags = 0;		//�I�v�V�����Ȃ�
	imageLoadInfo.Format = DXGI_FORMAT_B8G8R8A8_UNORM;	//�t�H�[�}�b�g
	imageLoadInfo.Filter = D3DX11_FILTER_LINEAR;		//���`�t�B���^
	imageLoadInfo.MipFilter = D3DX11_FILTER_LINEAR;		//���`�t�B���^
	imageLoadInfo.pSrcInfo = NULL;
	//�e�N�X�`���ǂݍ���
	D3DX11CreateTextureFromFile(
		g_pD3DDevice,		//���\�[�X���쐬����f�o�C�X��
		fileName,			//�摜�t�@�C����
		&imageLoadInfo,		//�쐬����e�N�X�`���̐ݒ�
		NULL,				//�񓯊��Ŏ��s���Ȃ�	
		&tex->textrue,	//�e�N�X�`�����擾����ϐ�
		&hr					//�߂�l���擾����ϐ�
		);
	if (FAILED(hr)){
		OutputDebugString(L"�e�N�X�`���̍쐬�Ɏ��s\n");
		return false;
	}
	//2D�e�N�X�`���ɃA�N�Z�X����V�F�[�_���\�[�X�r���[�̐ݒ�
	D3D11_SHADER_RESOURCE_VIEW_DESC srDesc;
	srDesc.Format = imageLoadInfo.Format;	//�t�H�[�}�b�g
	srDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;	//2D�e�N�X�`��
	srDesc.Texture2D.MostDetailedMip = 0;	//�ł��ڍׂȃ~�b�v�}�b�v���x��
	srDesc.Texture2D.MipLevels = -1;		//�S�Ẵ~�b�v�}�b�v���x��

	//�V�F�[�_�\���\�[�X�r���[�̍쐬
	hr = g_pD3DDevice->CreateShaderResourceView(
		tex->textrue,			//�A�N�Z�X����e�N�X�`�����\�[�X
		&srDesc,			//�V�F�[�_�\���\�[�X�r���[�̐ݒ�
		&tex->shaerResourceView		//�󂯎��ϐ�
		);
	if (FAILED(hr)){
		OutputDebugString(L"�V�F�[�_�\���\�[�X�r���[�̍쐬�Ɏ��s\n");
		return false;
	}
	return true;
}

bool D2DUtil::loadFontTextrue(LPWSTR fileName, Texture* tex){
	/***************************************/
	/*�@�@�@�e�N�X�`���ǂݍ���             */
	/***************************************/
	D3DX11_IMAGE_INFO imageInfo;	//�����󂯎��ϐ�
	HRESULT hr;
	hr = D3DX11GetImageInfoFromFile(fileName, NULL, &imageInfo, &hr);
	if (FAILED(hr)){
		return false;
	}

	//�e�N�X�`���N���X�ɉ摜�̑傫����ۑ�
	tex->width = 50;
	tex->height = 100;

	//�쐬����e�N�X�`���̐ݒ�
	D3DX11_IMAGE_LOAD_INFO imageLoadInfo;
	imageLoadInfo.Width = imageInfo.Width;	//�e�N�X�`���̕�
	imageLoadInfo.Height = imageInfo.Height;	//�e�N�X�`���̍���
	imageLoadInfo.Depth = 0;	//�e�N�X�`���̐[��
	imageLoadInfo.FirstMipLevel = 0;	//�ŏ��ɓǂݍ��ރ~�b�v�}�b�v���x��
	imageLoadInfo.MipLevels = 8;		//�~�b�v�}�b�v���x���̐�
	imageLoadInfo.Usage = D3D11_USAGE_DEFAULT;	//�f�t�H���g�g�p�@
	imageLoadInfo.BindFlags = D3D11_BIND_SHADER_RESOURCE;	//�V�F�[�_���\�[�X
	imageLoadInfo.CpuAccessFlags = 0;	//CPU����A�N�Z�X���Ȃ�
	imageLoadInfo.MiscFlags = 0;		//�I�v�V�����Ȃ�
	imageLoadInfo.Format = DXGI_FORMAT_B8G8R8A8_UNORM;	//�t�H�[�}�b�g
	imageLoadInfo.Filter = D3DX11_FILTER_LINEAR;		//���`�t�B���^
	imageLoadInfo.MipFilter = D3DX11_FILTER_LINEAR;		//���`�t�B���^
	imageLoadInfo.pSrcInfo = NULL;
	//�e�N�X�`���ǂݍ���
	D3DX11CreateTextureFromFile(
		g_pD3DDevice,		//���\�[�X���쐬����f�o�C�X��
		fileName,			//�摜�t�@�C����
		&imageLoadInfo,		//�쐬����e�N�X�`���̐ݒ�
		NULL,				//�񓯊��Ŏ��s���Ȃ�	
		&tex->textrue,	//�e�N�X�`�����擾����ϐ�
		&hr					//�߂�l���擾����ϐ�
		);
	if (FAILED(hr)){
		return false;
	}
	//2D�e�N�X�`���ɃA�N�Z�X����V�F�[�_���\�[�X�r���[�̐ݒ�
	D3D11_SHADER_RESOURCE_VIEW_DESC srDesc;
	srDesc.Format = imageLoadInfo.Format;	//�t�H�[�}�b�g
	srDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;	//2D�e�N�X�`��
	srDesc.Texture2D.MostDetailedMip = 0;	//�ł��ڍׂȃ~�b�v�}�b�v���x��
	srDesc.Texture2D.MipLevels = -1;		//�S�Ẵ~�b�v�}�b�v���x��

	//�V�F�[�_�\���\�[�X�r���[�̍쐬
	hr = g_pD3DDevice->CreateShaderResourceView(
		tex->textrue,			//�A�N�Z�X����e�N�X�`�����\�[�X
		&srDesc,			//�V�F�[�_�\���\�[�X�r���[�̐ݒ�
		&tex->shaerResourceView		//�󂯎��ϐ�
		);
	if (FAILED(hr)){
		return false;
	}
	return true;
}

bool D2DUtil::loadFontTexture(Texture* tex){
	/*******2D�e�N�X�`���쐬********/
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Width = FontLoder::gm.gmCellIncX;
	desc.Height = FontLoder::tm.tmHeight;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	tex->width = desc.Width;
	tex->height = desc.Height;

	//ID3D11Texture2D* g_pTexture;
	SAFE_RELEASE(tex->dynamicsTexture);
	g_pD3DDevice->CreateTexture2D(&desc, NULL, &tex->dynamicsTexture);

	//���\�[�X�̃}�b�v
	D3D11_MAPPED_SUBRESOURCE MappedResource;
	HRESULT hr = g_pImmediateContext->Map(
		tex->dynamicsTexture,
		D3D11CalcSubresource(0, 0, 1),
		D3D11_MAP_WRITE_DISCARD,
		0,
		&MappedResource);
	if (FAILED(hr)){
		return false;
	}

	//�������W�̎Z�o
	UINT strX = FontLoder::gm.gmptGlyphOrigin.x;
	UINT strY = (FontLoder::tm.tmAscent - FontLoder::gm.gmptGlyphOrigin.y);

	//���\�[�X�̏�������
	UINT nLine = ((FontLoder::gm.gmBlackBoxX + 3) / 4) * 4;
	UCHAR* texel = (UCHAR*)MappedResource.pData;
	for (UINT row = 0; row < desc.Height; row++){	//�s
		UINT rowStart = row * MappedResource.RowPitch;
		for (UINT col = 0; col < desc.Width; col++){//��
			UINT colStart = col * 4;
			texel[rowStart + colStart + 0] = 255;	//R
			texel[rowStart + colStart + 1] = 255;	//G
			texel[rowStart + colStart + 2] = 255;	//B
			if ((strX < col && strX + FontLoder::gm.gmBlackBoxX > col) &&
				(strY < row && strY + FontLoder::gm.gmBlackBoxY > row)){
				texel[rowStart + colStart + 3] = (255 * FontLoder::bitmapData[((row - strY) * nLine) + (col - strX)]) / (65 - 1);
			}
			else{
				texel[rowStart + colStart + 3] = 0;
			}
			/*if (row == 0 || row == desc.Height-1){
			texel[rowStart + colStart + 3] = 255;
			}
			else if(col == 0 || col == desc.Width-1){
			texel[rowStart + colStart + 3] = 255;
			}*/
		}
	}

	//�}�b�v����
	g_pImmediateContext->Unmap(tex->dynamicsTexture, D3D11CalcSubresource(0, 0, 1));

	D3D11_SHADER_RESOURCE_VIEW_DESC srDesc;
	srDesc.Format = desc.Format;
	srDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;
	srDesc.Texture2D.MostDetailedMip = 0;
	srDesc.Texture2D.MipLevels = 1;

	SAFE_RELEASE(tex->shaerResourceView);
	g_pD3DDevice->CreateShaderResourceView(
		tex->dynamicsTexture,
		&srDesc,
		&tex->shaerResourceView
		);
	//SAFE_RELEASE(g_pTexture);

	return true;
}

bool D2DUtil::drawString(float x, float y, float scaleX, float scaleY, TCHAR* str, UINT size){
	for (UINT n = 0; n < size; n++){
		FontLoder::getCharGraph(&str[n]);
		loadFontTexture(&FontLoder::tex);
		x += FontLoder::tex.width*scaleX / 2.0f;
		drawGraph(&FontLoder::tex, x, y, scaleX, scaleY,MIX);
		x += FontLoder::tex.width*scaleX/2.0f;
	}
	return true;
}

bool D2DUtil::drawString(float x, float y, float scaleX, float scaleY, std::wstring str){
	return drawString(x, y, scaleX, scaleY, (TCHAR*)str.c_str(), str.size());
}
bool D2DUtil::drawGraph(Texture* tex, float x, float y, float scaleX, float scaleY, DX11UTIL_BLEND blend){
	return drawGraph(tex, x, y, scaleX, scaleY,blend, 0.0f);
}

bool D2DUtil::drawGraph(Texture* tex, float x, float y, float scaleX, float scaleY, DX11UTIL_BLEND blend, float angle){
	return drawGraph(tex, x, y, scaleX, scaleY,blend, angle, .0f, .0f);
}

bool D2DUtil::drawGraph(Texture* tex, float x, float y, float scaleX, float scaleY, DX11UTIL_BLEND blend, float angle, float px, float py){
	//�������[�h�̐ݒ�
	if (blend == ADDITION){
		//OM�Ƀu�����h�X�e�[�g�I�u�W�F�N�g��ݒ�
		FLOAT BlendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		g_pImmediateContext->OMSetBlendState(
			g_pAdditionBlendState,	//�ݒ肷��I�u�W�F�N�g
			BlendFactor,		//�萔�l(RGBA)
			0xffffffff);		//�T���v���p�}�X�N
	}
	else if (blend == MIX){
		//OM�Ƀu�����h�X�e�[�g�I�u�W�F�N�g��ݒ�
		FLOAT BlendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		g_pImmediateContext->OMSetBlendState(
			g_pMixBlendState,	//�ݒ肷��I�u�W�F�N�g
			BlendFactor,		//�萔�l(RGBA)
			0xffffffff);		//�T���v���p�}�X�N
	}
	//PS�ɃV�F�[�_�\���\�[�X�r���[��ݒ�
	g_pImmediateContext->PSSetShaderResources(
		0,				//�ݒ肷��ŏ��̃X���b�g�ԍ�
		1,				//�ݒ肷��V�F�[�_���\�[�X�r���[�̐�
		&tex->shaerResourceView	//�ݒ肷��V�F�[�_���\�[�X�r���[�̔z��
		);

	XMMATRIX mat = XMMatrixTranslation(x, y, 0.0f);
	XMMATRIX scale = XMMatrixScaling(tex->width*scaleX, tex->height*scaleY, 0.0f);
	XMMATRIX rotate = XMMatrixRotationZ(XMConvertToRadians(angle));

	XMStoreFloat4x4(&g_cbChangesEveryObject.World, XMMatrixTranspose(rotate*scale*mat));
	//�萔�o�b�t�@�i�p�ɂɕύX����j�̃}�b�v�擾
	HRESULT hr = g_pImmediateContext->Map(
		g_pCBuffer[2],				//�}�b�v���郊�\�[�X
		0,							//�T�u���\�[�X�̃C���f�b�N�X�ԍ�
		D3D11_MAP_WRITE_DISCARD,	//�������݃A�N�Z�X
		0,
		&MappedResource				//�f�[�^�̏������ݐ�|�C���^
		);
	if (FAILED(hr)){
		return false;
	}
	//�f�[�^��������
	CopyMemory(MappedResource.pData, &g_cbChangesEveryObject, sizeof(cbChangesEveryObject));
	//�}�b�v����
	g_pImmediateContext->Unmap(g_pCBuffer[2], 0);
	/****************************************/
	//�`��i�C���f�b�N�X�o�b�t�@���g���j
	g_pImmediateContext->DrawIndexed(
		6,		//���_��
		0,		//�C���f�b�N�X�o�b�t�@�̍ŏ��̃C���f�b�N�X����`��J�n
		0		//���_�o�b�t�@���̍ŏ��̒��_�f�[�^����g�p�J�n
		);
	//g_pImmediateContext->Draw(4, 0);
	/****************************************/
	return true;
}

bool D2DUtil::drawGraphAbsolutely(Texture* tex, float x, float y, float scaleX, float scaleY,float radAngle, DX11UTIL_BLEND blend){
	//�������[�h�̐ݒ�
	if (blend == ADDITION){
		//OM�Ƀu�����h�X�e�[�g�I�u�W�F�N�g��ݒ�
		FLOAT BlendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		g_pImmediateContext->OMSetBlendState(
			g_pAdditionBlendState,	//�ݒ肷��I�u�W�F�N�g
			BlendFactor,		//�萔�l(RGBA)
			0xffffffff);		//�T���v���p�}�X�N
	}
	else if (blend == MIX){
		//OM�Ƀu�����h�X�e�[�g�I�u�W�F�N�g��ݒ�
		FLOAT BlendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		g_pImmediateContext->OMSetBlendState(
			g_pMixBlendState,	//�ݒ肷��I�u�W�F�N�g
			BlendFactor,		//�萔�l(RGBA)
			0xffffffff);		//�T���v���p�}�X�N
	}
	//PS�ɃV�F�[�_�\���\�[�X�r���[��ݒ�
	g_pImmediateContext->PSSetShaderResources(
		0,				//�ݒ肷��ŏ��̃X���b�g�ԍ�
		1,				//�ݒ肷��V�F�[�_���\�[�X�r���[�̐�
		&tex->shaerResourceView	//�ݒ肷��V�F�[�_���\�[�X�r���[�̔z��
		);

	XMMATRIX mat = XMMatrixTranslation(x, y, 0.0f);
	XMMATRIX scale = XMMatrixScaling(scaleX, scaleY, 0.0f);
	XMMATRIX rotate = XMMatrixRotationZ(radAngle);

	XMStoreFloat4x4(&g_cbChangesEveryObject.World, XMMatrixTranspose(rotate*scale*mat));
	//�萔�o�b�t�@�i�p�ɂɕύX����j�̃}�b�v�擾
	HRESULT hr = g_pImmediateContext->Map(
		g_pCBuffer[2],				//�}�b�v���郊�\�[�X
		0,							//�T�u���\�[�X�̃C���f�b�N�X�ԍ�
		D3D11_MAP_WRITE_DISCARD,	//�������݃A�N�Z�X
		0,
		&MappedResource				//�f�[�^�̏������ݐ�|�C���^
		);
	if (FAILED(hr)){
		return false;
	}
	//�f�[�^��������
	CopyMemory(MappedResource.pData, &g_cbChangesEveryObject, sizeof(cbChangesEveryObject));
	//�}�b�v����
	g_pImmediateContext->Unmap(g_pCBuffer[2], 0);
	/****************************************/
	//�`��i�C���f�b�N�X�o�b�t�@���g���j
	g_pImmediateContext->DrawIndexed(
		6,		//���_��
		0,		//�C���f�b�N�X�o�b�t�@�̍ŏ��̃C���f�b�N�X����`��J�n
		0		//���_�o�b�t�@���̍ŏ��̒��_�f�[�^����g�p�J�n
		);
	//g_pImmediateContext->Draw(4, 0);
	/****************************************/
	return true;
}

bool D2DUtil::drawGraphAnimate(Texture* tex, float x, float y, float scaleX, float scaleY, float su, float sv, float eu, float ev){
	//PS�ɃV�F�[�_�\���\�[�X�r���[��ݒ�
	g_pImmediateContext->PSSetShaderResources(
		0,				//�ݒ肷��ŏ��̃X���b�g�ԍ�
		1,				//�ݒ肷��V�F�[�_���\�[�X�r���[�̐�
		&tex->shaerResourceView	//�ݒ肷��V�F�[�_���\�[�X�r���[�̔z��
		);

	XMMATRIX mat = XMMatrixTranslation(x, y, 0.0f);
	XMMATRIX scale = XMMatrixScaling(tex->width*scaleX, tex->height*scaleY, 0.0f);

	g_cbChangesEveryObject.Seek = XMFLOAT4(su, sv, eu, ev);

	XMStoreFloat4x4(&g_cbChangesEveryObject.World, XMMatrixTranspose(scale*mat));
	//�萔�o�b�t�@�i�p�ɂɕύX����j�̃}�b�v�擾
	HRESULT hr = g_pImmediateContext->Map(
		g_pCBuffer[2],				//�}�b�v���郊�\�[�X
		0,							//�T�u���\�[�X�̃C���f�b�N�X�ԍ�
		D3D11_MAP_WRITE_DISCARD,	//�������݃A�N�Z�X
		0,
		&MappedResource				//�f�[�^�̏������ݐ�|�C���^
		);
	if (FAILED(hr)){
		return false;
	}
	//�f�[�^��������
	CopyMemory(MappedResource.pData, &g_cbChangesEveryObject, sizeof(cbChangesEveryObject));
	//�}�b�v����
	g_pImmediateContext->Unmap(g_pCBuffer[2], 0);

	//�W�I���g���V�F�[�_
	g_pImmediateContext->GSSetShader(g_pAnimateGeometryShader, NULL, 0);
	/****************************************/
	//�`��i�C���f�b�N�X�o�b�t�@���g���j
	g_pImmediateContext->DrawIndexed(
		6,		//���_��
		0,		//�C���f�b�N�X�o�b�t�@�̍ŏ��̃C���f�b�N�X����`��J�n
		0		//���_�o�b�t�@���̍ŏ��̒��_�f�[�^����g�p�J�n
		);
	//g_pImmediateContext->Draw(4, 0);
	/****************************************/
	g_pImmediateContext->GSSetShader(g_pGeometryShader, NULL, 0);
	return true;
}

D2DUtil::D2DUtil(){

}

D2DUtil::~D2DUtil(){
	CleanupDirect3D();
}

//�|���S���̃Z���^�[�ʒu
XMMATRIX					D2DUtil::centor;
XMMATRIX					D2DUtil::reverce;