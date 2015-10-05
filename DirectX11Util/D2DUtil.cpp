#include "D2DUtil.h"

bool D2DUtil::initD2DUtil(HWND* targetWindow, float l_windowWidth, float l_windowHeight, bool fullScreen){
	OutputDebugString(L"D2DUtilの初期化を開始します\n");
	bool result = initDX11Util(targetWindow, l_windowWidth, l_windowHeight, fullScreen);
	centor = XMMatrixTranslation(-0.5f, -0.5f, .0f);
	reverce = XMMatrixTranslation(0.5f, 0.5f,.0f);
	return result;
}

bool D2DUtil::loadTextrue(LPWSTR fileName, Texture* tex){
	/***************************************/
	/*　　　テクスチャ読み込み             */
	/***************************************/
	D3DX11_IMAGE_INFO imageInfo;	//情報を受け取る変数
	HRESULT hr;
	hr = D3DX11GetImageInfoFromFile(fileName, NULL, &imageInfo, &hr);
	if (FAILED(hr)){
		OutputDebugString(L"画像情報の取得に失敗");
		return false;
	}

	//テクスチャクラスに画像の大きさを保存
	tex->width = imageInfo.Width;
	tex->height = imageInfo.Height;

	//作成するテクスチャの設定
	D3DX11_IMAGE_LOAD_INFO imageLoadInfo;
	imageLoadInfo.Width = imageInfo.Width;	//テクスチャの幅
	imageLoadInfo.Height = imageInfo.Height;	//テクスチャの高さ
	imageLoadInfo.Depth = 0;	//テクスチャの深さ
	imageLoadInfo.FirstMipLevel = 0;	//最初に読み込むミップマップレベル
	imageLoadInfo.MipLevels = 8;		//ミップマップレベルの数
	imageLoadInfo.Usage = D3D11_USAGE_DEFAULT;	//デフォルト使用法
	imageLoadInfo.BindFlags = D3D11_BIND_SHADER_RESOURCE;	//シェーダリソース
	imageLoadInfo.CpuAccessFlags = 0;	//CPUからアクセスしない
	imageLoadInfo.MiscFlags = 0;		//オプションなし
	imageLoadInfo.Format = DXGI_FORMAT_B8G8R8A8_UNORM;	//フォーマット
	imageLoadInfo.Filter = D3DX11_FILTER_LINEAR;		//線形フィルタ
	imageLoadInfo.MipFilter = D3DX11_FILTER_LINEAR;		//線形フィルタ
	imageLoadInfo.pSrcInfo = NULL;
	//テクスチャ読み込み
	D3DX11CreateTextureFromFile(
		g_pD3DDevice,		//リソースを作成するデバイス名
		fileName,			//画像ファイル名
		&imageLoadInfo,		//作成するテクスチャの設定
		NULL,				//非同期で実行しない	
		&tex->textrue,	//テクスチャを取得する変数
		&hr					//戻り値を取得する変数
		);
	if (FAILED(hr)){
		OutputDebugString(L"テクスチャの作成に失敗\n");
		return false;
	}
	//2Dテクスチャにアクセスするシェーダリソースビューの設定
	D3D11_SHADER_RESOURCE_VIEW_DESC srDesc;
	srDesc.Format = imageLoadInfo.Format;	//フォーマット
	srDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;	//2Dテクスチャ
	srDesc.Texture2D.MostDetailedMip = 0;	//最も詳細なミップマップレベル
	srDesc.Texture2D.MipLevels = -1;		//全てのミップマップレベル

	//シェーダ―リソースビューの作成
	hr = g_pD3DDevice->CreateShaderResourceView(
		tex->textrue,			//アクセスするテクスチャリソース
		&srDesc,			//シェーダ―リソースビューの設定
		&tex->shaerResourceView		//受け取る変数
		);
	if (FAILED(hr)){
		OutputDebugString(L"シェーダ―リソースビューの作成に失敗\n");
		return false;
	}
	return true;
}

bool D2DUtil::loadFontTextrue(LPWSTR fileName, Texture* tex){
	/***************************************/
	/*　　　テクスチャ読み込み             */
	/***************************************/
	D3DX11_IMAGE_INFO imageInfo;	//情報を受け取る変数
	HRESULT hr;
	hr = D3DX11GetImageInfoFromFile(fileName, NULL, &imageInfo, &hr);
	if (FAILED(hr)){
		return false;
	}

	//テクスチャクラスに画像の大きさを保存
	tex->width = 50;
	tex->height = 100;

	//作成するテクスチャの設定
	D3DX11_IMAGE_LOAD_INFO imageLoadInfo;
	imageLoadInfo.Width = imageInfo.Width;	//テクスチャの幅
	imageLoadInfo.Height = imageInfo.Height;	//テクスチャの高さ
	imageLoadInfo.Depth = 0;	//テクスチャの深さ
	imageLoadInfo.FirstMipLevel = 0;	//最初に読み込むミップマップレベル
	imageLoadInfo.MipLevels = 8;		//ミップマップレベルの数
	imageLoadInfo.Usage = D3D11_USAGE_DEFAULT;	//デフォルト使用法
	imageLoadInfo.BindFlags = D3D11_BIND_SHADER_RESOURCE;	//シェーダリソース
	imageLoadInfo.CpuAccessFlags = 0;	//CPUからアクセスしない
	imageLoadInfo.MiscFlags = 0;		//オプションなし
	imageLoadInfo.Format = DXGI_FORMAT_B8G8R8A8_UNORM;	//フォーマット
	imageLoadInfo.Filter = D3DX11_FILTER_LINEAR;		//線形フィルタ
	imageLoadInfo.MipFilter = D3DX11_FILTER_LINEAR;		//線形フィルタ
	imageLoadInfo.pSrcInfo = NULL;
	//テクスチャ読み込み
	D3DX11CreateTextureFromFile(
		g_pD3DDevice,		//リソースを作成するデバイス名
		fileName,			//画像ファイル名
		&imageLoadInfo,		//作成するテクスチャの設定
		NULL,				//非同期で実行しない	
		&tex->textrue,	//テクスチャを取得する変数
		&hr					//戻り値を取得する変数
		);
	if (FAILED(hr)){
		return false;
	}
	//2Dテクスチャにアクセスするシェーダリソースビューの設定
	D3D11_SHADER_RESOURCE_VIEW_DESC srDesc;
	srDesc.Format = imageLoadInfo.Format;	//フォーマット
	srDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;	//2Dテクスチャ
	srDesc.Texture2D.MostDetailedMip = 0;	//最も詳細なミップマップレベル
	srDesc.Texture2D.MipLevels = -1;		//全てのミップマップレベル

	//シェーダ―リソースビューの作成
	hr = g_pD3DDevice->CreateShaderResourceView(
		tex->textrue,			//アクセスするテクスチャリソース
		&srDesc,			//シェーダ―リソースビューの設定
		&tex->shaerResourceView		//受け取る変数
		);
	if (FAILED(hr)){
		return false;
	}
	return true;
}

bool D2DUtil::loadFontTexture(Texture* tex){
	/*******2Dテクスチャ作成********/
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

	//リソースのマップ
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

	//文字座標の算出
	UINT strX = FontLoder::gm.gmptGlyphOrigin.x;
	UINT strY = (FontLoder::tm.tmAscent - FontLoder::gm.gmptGlyphOrigin.y);

	//リソースの書き込み
	UINT nLine = ((FontLoder::gm.gmBlackBoxX + 3) / 4) * 4;
	UCHAR* texel = (UCHAR*)MappedResource.pData;
	for (UINT row = 0; row < desc.Height; row++){	//行
		UINT rowStart = row * MappedResource.RowPitch;
		for (UINT col = 0; col < desc.Width; col++){//列
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

	//マップ解除
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
	//合成モードの設定
	if (blend == ADDITION){
		//OMにブレンドステートオブジェクトを設定
		FLOAT BlendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		g_pImmediateContext->OMSetBlendState(
			g_pAdditionBlendState,	//設定するオブジェクト
			BlendFactor,		//定数値(RGBA)
			0xffffffff);		//サンプル用マスク
	}
	else if (blend == MIX){
		//OMにブレンドステートオブジェクトを設定
		FLOAT BlendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		g_pImmediateContext->OMSetBlendState(
			g_pMixBlendState,	//設定するオブジェクト
			BlendFactor,		//定数値(RGBA)
			0xffffffff);		//サンプル用マスク
	}
	//PSにシェーダ―リソースビューを設定
	g_pImmediateContext->PSSetShaderResources(
		0,				//設定する最初のスロット番号
		1,				//設定するシェーダリソースビューの数
		&tex->shaerResourceView	//設定するシェーダリソースビューの配列
		);

	XMMATRIX mat = XMMatrixTranslation(x, y, 0.0f);
	XMMATRIX scale = XMMatrixScaling(tex->width*scaleX, tex->height*scaleY, 0.0f);
	XMMATRIX rotate = XMMatrixRotationZ(XMConvertToRadians(angle));

	XMStoreFloat4x4(&g_cbChangesEveryObject.World, XMMatrixTranspose(rotate*scale*mat));
	//定数バッファ（頻繁に変更する）のマップ取得
	HRESULT hr = g_pImmediateContext->Map(
		g_pCBuffer[2],				//マップするリソース
		0,							//サブリソースのインデックス番号
		D3D11_MAP_WRITE_DISCARD,	//書き込みアクセス
		0,
		&MappedResource				//データの書き込み先ポインタ
		);
	if (FAILED(hr)){
		return false;
	}
	//データ書き込み
	CopyMemory(MappedResource.pData, &g_cbChangesEveryObject, sizeof(cbChangesEveryObject));
	//マップ解除
	g_pImmediateContext->Unmap(g_pCBuffer[2], 0);
	/****************************************/
	//描画（インデックスバッファを使う）
	g_pImmediateContext->DrawIndexed(
		6,		//頂点数
		0,		//インデックスバッファの最初のインデックスから描画開始
		0		//頂点バッファ内の最初の頂点データから使用開始
		);
	//g_pImmediateContext->Draw(4, 0);
	/****************************************/
	return true;
}

bool D2DUtil::drawGraphAbsolutely(Texture* tex, float x, float y, float scaleX, float scaleY,float radAngle, DX11UTIL_BLEND blend){
	//合成モードの設定
	if (blend == ADDITION){
		//OMにブレンドステートオブジェクトを設定
		FLOAT BlendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		g_pImmediateContext->OMSetBlendState(
			g_pAdditionBlendState,	//設定するオブジェクト
			BlendFactor,		//定数値(RGBA)
			0xffffffff);		//サンプル用マスク
	}
	else if (blend == MIX){
		//OMにブレンドステートオブジェクトを設定
		FLOAT BlendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		g_pImmediateContext->OMSetBlendState(
			g_pMixBlendState,	//設定するオブジェクト
			BlendFactor,		//定数値(RGBA)
			0xffffffff);		//サンプル用マスク
	}
	//PSにシェーダ―リソースビューを設定
	g_pImmediateContext->PSSetShaderResources(
		0,				//設定する最初のスロット番号
		1,				//設定するシェーダリソースビューの数
		&tex->shaerResourceView	//設定するシェーダリソースビューの配列
		);

	XMMATRIX mat = XMMatrixTranslation(x, y, 0.0f);
	XMMATRIX scale = XMMatrixScaling(scaleX, scaleY, 0.0f);
	XMMATRIX rotate = XMMatrixRotationZ(radAngle);

	XMStoreFloat4x4(&g_cbChangesEveryObject.World, XMMatrixTranspose(rotate*scale*mat));
	//定数バッファ（頻繁に変更する）のマップ取得
	HRESULT hr = g_pImmediateContext->Map(
		g_pCBuffer[2],				//マップするリソース
		0,							//サブリソースのインデックス番号
		D3D11_MAP_WRITE_DISCARD,	//書き込みアクセス
		0,
		&MappedResource				//データの書き込み先ポインタ
		);
	if (FAILED(hr)){
		return false;
	}
	//データ書き込み
	CopyMemory(MappedResource.pData, &g_cbChangesEveryObject, sizeof(cbChangesEveryObject));
	//マップ解除
	g_pImmediateContext->Unmap(g_pCBuffer[2], 0);
	/****************************************/
	//描画（インデックスバッファを使う）
	g_pImmediateContext->DrawIndexed(
		6,		//頂点数
		0,		//インデックスバッファの最初のインデックスから描画開始
		0		//頂点バッファ内の最初の頂点データから使用開始
		);
	//g_pImmediateContext->Draw(4, 0);
	/****************************************/
	return true;
}

bool D2DUtil::drawGraphAnimate(Texture* tex, float x, float y, float scaleX, float scaleY, float su, float sv, float eu, float ev){
	//PSにシェーダ―リソースビューを設定
	g_pImmediateContext->PSSetShaderResources(
		0,				//設定する最初のスロット番号
		1,				//設定するシェーダリソースビューの数
		&tex->shaerResourceView	//設定するシェーダリソースビューの配列
		);

	XMMATRIX mat = XMMatrixTranslation(x, y, 0.0f);
	XMMATRIX scale = XMMatrixScaling(tex->width*scaleX, tex->height*scaleY, 0.0f);

	g_cbChangesEveryObject.Seek = XMFLOAT4(su, sv, eu, ev);

	XMStoreFloat4x4(&g_cbChangesEveryObject.World, XMMatrixTranspose(scale*mat));
	//定数バッファ（頻繁に変更する）のマップ取得
	HRESULT hr = g_pImmediateContext->Map(
		g_pCBuffer[2],				//マップするリソース
		0,							//サブリソースのインデックス番号
		D3D11_MAP_WRITE_DISCARD,	//書き込みアクセス
		0,
		&MappedResource				//データの書き込み先ポインタ
		);
	if (FAILED(hr)){
		return false;
	}
	//データ書き込み
	CopyMemory(MappedResource.pData, &g_cbChangesEveryObject, sizeof(cbChangesEveryObject));
	//マップ解除
	g_pImmediateContext->Unmap(g_pCBuffer[2], 0);

	//ジオメトリシェーダ
	g_pImmediateContext->GSSetShader(g_pAnimateGeometryShader, NULL, 0);
	/****************************************/
	//描画（インデックスバッファを使う）
	g_pImmediateContext->DrawIndexed(
		6,		//頂点数
		0,		//インデックスバッファの最初のインデックスから描画開始
		0		//頂点バッファ内の最初の頂点データから使用開始
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

//ポリゴンのセンター位置
XMMATRIX					D2DUtil::centor;
XMMATRIX					D2DUtil::reverce;