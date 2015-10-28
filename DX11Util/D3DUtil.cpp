#include "D3DUtil.h"
#include "Texture.h"


D3DUtil::D3DUtil()
{
}


D3DUtil::~D3DUtil()
{
	CleanupDirect3D();
}

bool D3DUtil::initD3DUtil(HWND* targetWindow,float l_windowWidth,float l_windowHeight,bool fullScreen){
	if (XMVerifyCPUSupport() != TRUE)
	{
		DebugBreak();
		return false;
	}
	window = targetWindow;
	windowMode = fullScreen;

	g_pSwapChain = NULL;
	g_pVertexShader = NULL;
	g_pGeometryShader = NULL;
	g_pAnimateGeometryShader = NULL;
	g_pPixelShader = NULL;
	pBlobVS = NULL;
	pBlobGS = NULL;
	pBlobPS = NULL;
	g_pRasterizerState = NULL;
	g_pBlendState = NULL;
	g_pDepthStencilState = NULL;
	g_pInputLayout = NULL;
	g_pIdxBuffer = NULL;
	g_pSampler = NULL;

	if (!windowMode){
		RECT clientRect;
		GetClientRect(*window, &clientRect);
		windowWidth = clientRect.right;
		windowHeight = clientRect.bottom;
	}
	else{
		windowWidth = l_windowWidth;
		windowHeight = l_windowHeight;
	}

	if (!initDeviceAndSwapChain()){
		MessageBox(*targetWindow, L"filed to initialize the device and swapchain", NULL, 0);
		DebugBreak();
	}
	else if (!initBackBuffer()){
		MessageBox(*targetWindow, L"filed to initialize the backbuffer", NULL, 0);
		DebugBreak();
	}
	else if (!initViewPort()){
		MessageBox(*targetWindow, L"filed to initialize the viewport", NULL, 0);
		DebugBreak();
	}
	else if (!initDepthAndStencil()){
		MessageBox(*targetWindow, L"filed to initialize the depth and stencil buffer", NULL, 0);
		DebugBreak();
	}
	else if (!initVertexBuffer()){
		MessageBox(*targetWindow, L"filed to initialize the vertex buffer", NULL, 0);
		DebugBreak();
	}
	else if (!initShaderAndConstantBuffer()){
		MessageBox(*targetWindow, L"filed to initialize the shader and constant buffer", NULL, 0);
		DebugBreak();
	}
	else if (!initInputAssembler()){
		MessageBox(*targetWindow, L"filed to initialize the input assembler", NULL, 0);
		DebugBreak();
	}
	else if (!initRasterizer()){
		MessageBox(*targetWindow, L"filed to initialize the rasterizer", NULL, 0);
		DebugBreak();
	}
	else if (!initOutputMerger()){
		MessageBox(*targetWindow, L"filed to initialize the input merger", NULL, 0);
		DebugBreak();
	}
	else if (!initConstantBuffer()){
		MessageBox(*targetWindow, L"filed to initialize the constant buffer", NULL, 0);
		DebugBreak();
	}
	else if (!initSampler()){
		MessageBox(*targetWindow, L"filed to initialize sampler", NULL, 0);
		DebugBreak();
	}
	return true;
}

bool D3DUtil::initFont(LPCWSTR fontName,UINT fontSize){
	return FontLoder::initFont(NULL,fontName, fontSize);
}

bool D3DUtil::initDeviceAndSwapChain(){

	/****************************************/
	/*			スワップチェインの設定		*/
	/****************************************/
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;			//バックバッファ数
	sd.BufferDesc.Width = windowWidth;	//バックバッファの幅
	sd.BufferDesc.Height = windowHeight;	//バックバッファの高さ
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	//フォーマット
	sd.BufferDesc.RefreshRate.Numerator = 60;	//リフレッシュレート　分子
	sd.BufferDesc.RefreshRate.Denominator = 1;	//リフレッシュレート　分母
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	//バックバッファの使用法
	sd.OutputWindow = *window;	//関連付けるウィンドウ
	sd.SampleDesc.Count = 1;	//マルチサンプリング数
	sd.SampleDesc.Quality = 0;	//マルチサンプリングのクオリティ
	sd.Windowed = !windowMode;			//ウィンドウモードにするかどうか
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;	//モード自動切り替え
	
	/*****************************************/
	/*			  機能レベルの設定			 */
	/*****************************************/

	D3D_FEATURE_LEVEL	g_pFeatureLevels[] = {	//機能レベル配列
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	DWORD				g_FeatureLevels = 3;	//配列の要素数
	D3D_FEATURE_LEVEL g_FeatureLevelsSupported;	//デバイス作成時に返される機能レベル

	/*******************************************/
	/*			デバック用デバイスの作成	   */
	/*******************************************/
#if defined(DEBUG) || defined(_DEBUG)
	DWORD createDeviceFlags = D3D11_CREATE_DEVICE_DEBUG;	//デバッグ時にはデバック用デバイスを設定
#else
	DWORD createDeviceFlags = 0;
#endif

	/*******************************************/
	/*	デバイスとスワップチェインの同時作成   */
	/*******************************************/

	//ハードウェアデバイスを作成
	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		createDeviceFlags,
		g_pFeatureLevels,
		g_FeatureLevels,
		D3D11_SDK_VERSION,
		&sd,
		&g_pSwapChain,
		&g_pD3DDevice,
		&g_FeatureLevelsSupported,
		&g_pImmediateContext);
	if (FAILED(hr)){
		//ハードウェアがダメならWARPデバイスを作成
		hr = D3D11CreateDeviceAndSwapChain(
			NULL,
			D3D_DRIVER_TYPE_WARP,
			NULL,
			createDeviceFlags,
			g_pFeatureLevels,
			g_FeatureLevels,
			D3D11_SDK_VERSION,
			&sd,
			&g_pSwapChain,
			&g_pD3DDevice,
			&g_FeatureLevelsSupported,
			&g_pImmediateContext);
		if (FAILED(hr)){
			//WARPデバイスがダメならリファレンスデバイスの作成
			hr = D3D11CreateDeviceAndSwapChain(
				NULL,
				D3D_DRIVER_TYPE_REFERENCE,
				NULL,
				createDeviceFlags,
				g_pFeatureLevels,
				g_FeatureLevels,
				D3D11_SDK_VERSION,
				&sd,
				&g_pSwapChain,
				&g_pD3DDevice,
				&g_FeatureLevelsSupported,
				&g_pImmediateContext);
			if ((FAILED(hr))){
				return false;
			}
		}
	}
	return true;
}

bool D3DUtil::initBackBuffer(){
	/****************************************/
	/*        描画ターゲットビューの設定    */
	/****************************************/

	//スワップチェインから最初のバックバッファを取得する
	ID3D11Texture2D* pBackBuffer;	//バッファのアクセスに使うインターフェース
	HRESULT hr = g_pSwapChain->GetBuffer(	//バッファをpBackBufferで受け取る
		0,									//バック・バッファの番号
		__uuidof(ID3D11Texture2D),			//バッファにアクセスするインターフェース
		(LPVOID*)&pBackBuffer);				//バッファを受け取る変数
	if (FAILED(hr)){
		return false;
	}

	//バックバッファの描画ターゲットビューを作る
	hr = g_pD3DDevice->CreateRenderTargetView(
		pBackBuffer,				//ビューでアクセスするリソース
		NULL,						//描画ターゲットビューの定義(NULLはデフォルト設定使用)
		&g_pRenderTargetView);		//描画ターゲットビューを受け取る変数

	SAFE_RELEASE(pBackBuffer);		//設定が終わったので、直接扱わないバックバッファの入れ物は解放する
	if (FAILED(hr)){
		return false;
	}

	//描画ターゲットビューを出力マージャーの描画ターゲットとして設定
	g_pImmediateContext->OMSetRenderTargets(
		1,						//描画ターゲット数
		&g_pRenderTargetView,	//描画ターゲットビュー配列
		NULL);					//深度/ステンシルビューの設定(NULL=使用しない)

	return true;
}

bool D3DUtil::initViewPort(){
	/*****************************************/
	/*          ビューポートの設定           */
	/*****************************************/

	D3D11_VIEWPORT g_ViewPort[1];	//ビューポートの設定
	g_ViewPort[0].TopLeftX	= 0.0f;			//ビューポート領域の左上X座標
	g_ViewPort[0].TopLeftY	= 0.0f;			//ビューポート領域の左上Y座標
	g_ViewPort[0].Width = windowWidth;		//ビューポート領域の幅
	g_ViewPort[0].Height = windowHeight;	//ビューポート領域の高さ
	g_ViewPort[0].MinDepth = 0.0f;			//ビューポート領域の深度値の最小値
	g_ViewPort[0].MaxDepth = 1.0f;			//ビューポート領域の深度値の最大値

	//ラスタライザにビューポートを設定
	g_pImmediateContext->RSSetViewports(1, g_ViewPort);

	return true;
}

bool D3DUtil::initDepthAndStencil(){
	/*************************************/
	/*  深度/ステンシルテクスチャの作成  */
	/*************************************/

	D3D11_TEXTURE2D_DESC descDepth;
	descDepth.Width = windowWidth;
	descDepth.Height = windowHeight;
	descDepth.MipLevels = 1;	//ミップマップのレベル数
	descDepth.ArraySize = 1;	//配列サイズ
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;	//フォーマット（深度のみ）
	descDepth.SampleDesc.Count = 1;	//マルチサンプリングの設定
	descDepth.SampleDesc.Quality = 0;	//マルチサンプリングの品質
	descDepth.Usage = D3D11_USAGE_DEFAULT;	//デフォルト使用法
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;	//深度/ステンシルとして使用
	descDepth.CPUAccessFlags = 0;	//CPUからはアクセスしない
	descDepth.MiscFlags = 0;		//その他の設定なし

	HRESULT hr = g_pD3DDevice->CreateTexture2D(
		&descDepth,			//作成する2Dテクスチャの設定
		NULL,				//
		&g_pDepthStencil);	//作成したテクスチャを受け取る変数

	if (FAILED(hr)){
		return false;
	}

	/************************************/
	/*   深度/ステンシルビューの作成    */
	/************************************/

	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	descDSV.Format = descDepth.Format;	//ビューのフォーマット
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Flags = 0;
	descDSV.Texture2D.MipSlice = 0;
	
	hr = g_pD3DDevice->CreateDepthStencilView(
		g_pDepthStencil,		//深度/ステンシルビューを作るテクスチャ
		&descDSV,				//深度/ステンシルビューの設定
		&g_pDepthStencilView);	//作成したビューを受け取る変数
	if (FAILED(hr)){
		return false;
	}

	//深度/ステンシルビューを出力マージャーに設定
	g_pImmediateContext->OMSetRenderTargets(
		1,						//設定する描画ターゲットビューの数
		&g_pRenderTargetView,	//設定する描画ターゲットビューの配列
		g_pDepthStencilView);	//設定する深度/ステンシルビュー

	return true;
}

void D3DUtil::initDispray(){
	//描画ターゲットのクリア
	float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f };//クリアする値(RGBA)
	g_pImmediateContext->ClearRenderTargetView(
		g_pRenderTargetView,	//クリアする描画ターゲット
		ClearColor);			//クリアする値

	//深度ステンシルのクリア
	g_pImmediateContext->ClearDepthStencilView(
		g_pDepthStencilView,	//クリアする深度ステンシルビュー
		D3D11_CLEAR_DEPTH,		//深度値だけをクリアする
		1.0f,					//深度バッファをクリアする値
		0);						//ステンシルバッファをクリアする値(この場合は無関係)

	//バックバッファの表示
	HRESULT hr = g_pSwapChain->Present(
		0,	//画面をすぐに更新する
		0);	//画面を実際に更新する
}

bool D3DUtil::isDeviceDisappearance(){
	//デバイスの消失処理
	HRESULT hr = g_pD3DDevice->GetDeviceRemovedReason();
	switch (hr){
	case S_OK:
		break;			//正常
	case DXGI_ERROR_DEVICE_HUNG:
	case DXGI_ERROR_DEVICE_RESET:
		CleanupDirect3D();
		if (!InitDirect3D()){
			return false;		//復旧失敗。アプリケーション終了
		}
		break;
	case DXGI_ERROR_DEVICE_REMOVED:
	case DXGI_ERROR_DRIVER_INTERNAL_ERROR:
	case DXGI_ERROR_INVALID_CALL:
	default:
		return false;			//どうしようもの無いのでアプリケーション終了
	};
}

bool D3DUtil::InitDirect3D(){
	return initD3DUtil(window, windowWidth, windowHeight,windowMode);
}

void D3DUtil::CleanupDirect3D(){
	//デバイスステートのクリア
	if (g_pImmediateContext){
		g_pImmediateContext->ClearState();
	}

	//フルスクリーン時用の終了処理
	if (g_pSwapChain && windowMode){
		g_pSwapChain->SetFullscreenState(FALSE, NULL);
	}

	//取得したインターフェースの解放
	SAFE_RELEASE(pBlobVS);
	SAFE_RELEASE(pBlobGS);
	SAFE_RELEASE(pBlobPS);

	SAFE_RELEASE(g_pVertexShader);
	SAFE_RELEASE(g_pGeometryShader);
	SAFE_RELEASE(g_pAnimateGeometryShader);
	SAFE_RELEASE(g_pPixelShader);

	SAFE_RELEASE(g_pCBuffer[0]);
	SAFE_RELEASE(g_pCBuffer[1]);
	SAFE_RELEASE(g_pCBuffer[2]);
	
	SAFE_RELEASE(g_pRasterizerState);
	SAFE_RELEASE(g_pBlendState);
	SAFE_RELEASE(g_pDepthStencilState);
	SAFE_RELEASE(g_pInputLayout);

	SAFE_RELEASE(g_pVerBuffer[0]);
	SAFE_RELEASE(g_pVerBuffer[1]);
	SAFE_RELEASE(g_pIdxBuffer);

	SAFE_RELEASE(g_pRenderTargetView);
	SAFE_RELEASE(g_pSampler);

	SAFE_RELEASE(g_pDepthStencilView);
	SAFE_RELEASE(g_pDepthStencil);
	SAFE_RELEASE(g_pSwapChain);
	SAFE_RELEASE(g_pImmediateContext);
	SAFE_RELEASE(g_pD3DDevice);
}

bool D3DUtil::initShaderAndConstantBuffer(){
	/*****************************************/
	/*         シェーダ―の初期化            */
	/*****************************************/

	//頂点シェーダの初期化
	HRESULT hr = D3DX11CompileFromFile(
		L"../Shader\\ShaderCode.sh",						//ファイル名
		NULL,									//マクロ定義
		NULL,									//インクルードファイル定義
		"VS",									//どのシェーダ―か（VSは頂点シェーダ用と言うこと）
		"vs_4_0",								//シェーダ―モデル
		D3D10_SHADER_ENABLE_STRICTNESS |		//コンパイルオプション
		D3D10_SHADER_PACK_MATRIX_COLUMN_MAJOR,
		0,										//エフェクトのコンパイルオプション
		NULL,									//直ぐにコンパイルしてから関数を抜ける
		&pBlobVS,								//バイトコードの格納先
		NULL,									//エラーメッセージは不要
		NULL);									//戻り値
	if (FAILED(hr)){
		return false;
	}

	//ジオメトリシェーダ―の初期化
	hr = D3DX11CompileFromFile(
		L"../Shader\\ShaderCode.sh",						//ファイル名
		NULL,									//マクロ定義
		NULL,									//インクルードファイル定義
		"GS",									//どのシェーダ―か（GSはジオメトリシェーダ用と言うこと）
		"gs_4_0",								//シェーダ―モデル
		D3D10_SHADER_ENABLE_STRICTNESS |		//コンパイルオプション
		D3D10_SHADER_PACK_MATRIX_COLUMN_MAJOR,
		0,										//エフェクトのコンパイルオプション
		NULL,									//直ぐにコンパイルしてから関数を抜ける
		&pBlobGS,								//バイトコードの格納先
		NULL,									//エラーメッセージは不要
		NULL);									//戻り値
	if (FAILED(hr)){
		return false;
	}

	//ジオメトリシェーダオブジェクトの生成
	hr = g_pD3DDevice->CreateGeometryShader(
		pBlobGS->GetBufferPointer(),	//バイトコードへのポインタ
		pBlobGS->GetBufferSize(),		//バイトコードの長さ
		NULL,
		&g_pGeometryShader);				//シェーダを受け取る変数
	if (FAILED(hr)){
		return false;
	}

	//文字用ジオメトリシェーダの初期化
	hr = D3DX11CompileFromFile(
		L"../Shader\\ShaderCode.sh",						//ファイル名
		NULL,									//マクロ定義
		NULL,									//インクルードファイル定義
		"GS_STRING",							//どのシェーダ―か（GSはジオメトリシェーダ用と言うこと）
		"gs_4_0",								//シェーダ―モデル
		D3D10_SHADER_ENABLE_STRICTNESS |		//コンパイルオプション
		D3D10_SHADER_PACK_MATRIX_COLUMN_MAJOR,
		0,										//エフェクトのコンパイルオプション
		NULL,									//直ぐにコンパイルしてから関数を抜ける
		&pBlobGS,								//バイトコードの格納先
		NULL,									//エラーメッセージは不要
		NULL);									//戻り値
	if (FAILED(hr)){
		return false;
	}

	//ジオメトリシェーダオブジェクトの生成
	hr = g_pD3DDevice->CreateGeometryShader(
		pBlobGS->GetBufferPointer(),	//バイトコードへのポインタ
		pBlobGS->GetBufferSize(),		//バイトコードの長さ
		NULL,
		&g_pAnimateGeometryShader);				//シェーダを受け取る変数
	if (FAILED(hr)){
		return false;
	}

	//ピクセルシェーダの初期化
	hr = D3DX11CompileFromFile(
		L"../Shader\\ShaderCode.sh",						//ファイル名
		NULL,									//マクロ定義
		NULL,									//インクルードファイル定義
		"PS_Sample",									//どのシェーダ―か（GSはジオメトリシェーダ用と言うこと）
		"ps_4_0",								//シェーダ―モデル
		D3D10_SHADER_ENABLE_STRICTNESS |		//コンパイルオプション
		D3D10_SHADER_PACK_MATRIX_COLUMN_MAJOR,
		0,										//エフェクトのコンパイルオプション
		NULL,									//直ぐにコンパイルしてから関数を抜ける
		&pBlobPS,								//バイトコードの格納先
		NULL,									//エラーメッセージは不要
		NULL);									//戻り値
	if (FAILED(hr)){
		return false;
	}

	//頂点シェーダオブジェクトの生成
	hr = g_pD3DDevice->CreateVertexShader(
		pBlobVS->GetBufferPointer(),	//バイトコードへのポインタ
		pBlobVS->GetBufferSize(),		//バイトコードの長さ
		NULL,
		&g_pVertexShader);				//シェーダを受け取る変数
	if (FAILED(hr)){
		return false;
	}

	//ピクセルシェーダオブジェクトの生成
	hr = g_pD3DDevice->CreatePixelShader(
		pBlobPS->GetBufferPointer(),	//バイトコードへのポインタ
		pBlobPS->GetBufferSize(),		//バイトコードの長さ
		NULL,
		&g_pPixelShader);				//シェーダを受け取る変数
	if (FAILED(hr)){
		return false;
	}

	//シェーダオブジェクトの設定
	//頂点シェーダ
	g_pImmediateContext->VSSetShader(g_pVertexShader, NULL, 0);

	//ジオメトリシェーダ
	g_pImmediateContext->GSSetShader(g_pGeometryShader, NULL, 0);

	//ピクセルシェーダ
	g_pImmediateContext->PSSetShader(g_pPixelShader, NULL, 0);

	/*****************************************/
	/*         定数バッファの作成            */
	/*****************************************/

	//定数バッファの定義
	D3D11_BUFFER_DESC cBufferDesc;
	cBufferDesc.Usage = D3D11_USAGE_DYNAMIC;				//動的使用法
	cBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;		//定数バッファ
	cBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;	//CPUから書き込む
	cBufferDesc.MiscFlags = 0;								
	
	//定数バッファ（変更しないデータ）の作成
	cBufferDesc.ByteWidth = sizeof(cbNeverChanged);	//バッファサイズ
	hr = g_pD3DDevice->CreateBuffer(
		&cBufferDesc,
		NULL,
		&g_pCBuffer[0]);
	if (FAILED(hr)){
		return false;
	}

	//定数バッファ（変更頻度の低いデータ）の作成
	cBufferDesc.ByteWidth = sizeof(cbChangesEveryFrame);	//バッファサイズ
	hr = g_pD3DDevice->CreateBuffer(
		&cBufferDesc,
		NULL,
		&g_pCBuffer[1]);
	if (FAILED(hr)){
		return false;
	}

	//定数バッファ（変更頻度の高いデータ）の作成
	cBufferDesc.ByteWidth = sizeof(cbChangesEveryObject);	//バッファサイズ
	hr = g_pD3DDevice->CreateBuffer(
		&cBufferDesc,
		NULL,
		&g_pCBuffer[2]);
	if (FAILED(hr)){
		return false;
	}

	//頂点シェーダに定数バッファを設定
	g_pImmediateContext->VSSetConstantBuffers(0, 3, g_pCBuffer);
	//ジオメトリシェーダに定数バッファを設定
	g_pImmediateContext->GSSetConstantBuffers(0, 3, g_pCBuffer);
	//ピクセルシェーダに定数バッファを設定
	g_pImmediateContext->PSSetConstantBuffers(0, 3, g_pCBuffer);
	
	return true;
}

bool D3DUtil::initConstantBuffer(){
	//定数バッファ（変更しない）
	//射影変換行列（パースペクティブ（透視法）射影）
	/*XMMATRIX mat = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(30.0f),	//視野角30°
		windowWidth / windowHeight,	//アスペクト比
		1.0f,						//前方投影面までの距離
		100.0f						//後方投影面までの距離
	);*/
	XMMATRIX mat = XMMatrixOrthographicLH(
		windowWidth,
		windowHeight,
		1.0f,
		20.f
		);
	mat = XMMatrixTranspose(mat);
	XMStoreFloat4x4(&g_cbNeverChanges.Projection, mat);

	//定数バッファ（変更しない）のマップ取得
	HRESULT hr = g_pImmediateContext->Map(
		g_pCBuffer[0],						//マップするリソース
		0,									//サブリソースのインデックス番号
		D3D11_MAP_WRITE_DISCARD,			//書き込みアクセス
		0,
		&MappedResource);					//データの書き込み先ポインタ
	if (FAILED(hr)){
		return false;
	}
	//データ書き込み
	CopyMemory(MappedResource.pData, &g_cbNeverChanges, sizeof(cbNeverChanged));
	//マップ解除
	g_pImmediateContext->Unmap(g_pCBuffer[0], 0);

	//定数バッファ(変更頻度の低い)
	//ビュー変換行列
	XMVECTORF32 eyePosition = { windowWidth/2.0f, windowHeight/2.0f, -10.0f, 1.0f };	//視点（カメラの位置）
	XMVECTORF32 focuPosition = { windowWidth / 2.0f, windowHeight / 2.0f, 0.0f, 1.0f };	//注視点
	XMVECTORF32 upDirection = { 0.0f, 1.0f, 0.0f, 1.0f };	//カメラの上方向
	mat = XMMatrixLookAtLH(eyePosition, focuPosition, upDirection);
	XMStoreFloat4x4(&g_cbChangesEveryFrame.View, XMMatrixTranspose(mat));
	//点光源座標
	g_vLightPos = XMFLOAT3(3.0f, 3.0f, -3.0f);
	XMVECTOR vec = XMVector3TransformCoord(XMLoadFloat3(&g_vLightPos), mat);
	XMStoreFloat3(&g_cbChangesEveryFrame.Light, vec);
	//定数バッファ（変更頻度の低い）のマップ取得
	hr = g_pImmediateContext->Map(
		g_pCBuffer[1],				//マップするリソース
		0,							//サブリソースのインデックス番号
		D3D11_MAP_WRITE_DISCARD,	//書き込みアクセス
		0,
		&MappedResource				//データの書き込み先ポインタ
		);
	if (FAILED(hr)){
		return false;
	}
	//データ書き込み
	CopyMemory(MappedResource.pData, &g_cbChangesEveryFrame, sizeof(cbChangesEveryFrame));
	//マップ解除
	g_pImmediateContext->Unmap(g_pCBuffer[1], 0);

	//定数バッファ（頻繁に変更する）を更新
	//ワールド変換行列
	mat = XMMatrixTranslation(0.0f, 0.0f, 0.0f);
	XMStoreFloat4x4(&g_cbChangesEveryObject.World, XMMatrixTranspose(mat));
	//定数バッファ（頻繁に変更する）のマップ取得
	hr = g_pImmediateContext->Map(
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
	return true;
}

bool D3DUtil::initVertexBuffer(){
	/*********************************************/
	/*          頂点バッファの作成               */
	/*********************************************/

	XYZBUFFER pos[] = {
		XMFLOAT3(0.0f, 1.0f, 0.0f),
		XMFLOAT3(1.0f, 1.0f, 0.0f),
		XMFLOAT3(1.0f, 0.0f, 0.0f),
		XMFLOAT3(0.0f, 0.0f, 0.0f)
	};

	posVertex = pos;

	D3D11_BUFFER_DESC xyzBufferDesc;
	xyzBufferDesc.Usage = D3D11_USAGE_DEFAULT;			//デフォルト使用法
	xyzBufferDesc.ByteWidth = sizeof(XYZBuffer) * 6;	//
	xyzBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;	//頂点バッファ
	xyzBufferDesc.CPUAccessFlags = 0;
	xyzBufferDesc.MiscFlags = 0;
	xyzBufferDesc.StructureByteStride = 0;

	//頂点バッファ（頂点）のサブリソースの定義
	D3D11_SUBRESOURCE_DATA xyzSubData;
	xyzSubData.pSysMem = pos;	//バッファデータの初期値
	xyzSubData.SysMemPitch = 0;
	xyzSubData.SysMemSlicePitch = 0;

	//頂点バッファ（頂点）の作成
	HRESULT hr = g_pD3DDevice->CreateBuffer(
		&xyzBufferDesc,
		&xyzSubData,
		&g_pVerBuffer[0]);
	if (FAILED(hr)){
		return false;
	}

	COLBUFFER col[] = {
			XMFLOAT3(1.0, 0.0f, 0.0f) ,  XMFLOAT3(1.0f, 0.0f, 0.0f) ,  XMFLOAT3(1.0f, 0.0f, 0.0f) , XMFLOAT3(1.0f, 0.0f, 0.0f)
	};

	colVertex = col;

	//頂点バッファ（色）の定義
	D3D11_BUFFER_DESC colBufferDesc;
	colBufferDesc.Usage = D3D11_USAGE_DEFAULT;			//デフォルト使用法
	colBufferDesc.ByteWidth = sizeof(ColBuffer) * 6;	//
	colBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;	//頂点バッファ
	colBufferDesc.CPUAccessFlags = 0;
	colBufferDesc.MiscFlags = 0;
	colBufferDesc.StructureByteStride = 0;

	//頂点バッファ（色）のサブリソースの定義
	D3D11_SUBRESOURCE_DATA colSubData;
	colSubData.pSysMem = col;	//バッファの初期値
	colSubData.SysMemPitch = 0;
	colSubData.SysMemSlicePitch = 0;

	//頂点バッファ（色）のサブリソースの作成
	hr = g_pD3DDevice->CreateBuffer(&colBufferDesc, &colSubData, &g_pVerBuffer[1]);
	if (FAILED(hr)){
		return false;
	}

	/*******************************************/
	/*      インデックスバッファ作成           */
	/*******************************************/

	//インデックスバッファのサブリソースの初期値（頂点ID）
	UINT idxVertexID[] = {
		0, 1, 2, 0, 2, 3
	};

	//インデックスバッファの定義
	D3D11_BUFFER_DESC idxBufferDesc;
	idxBufferDesc.Usage = D3D11_USAGE_DEFAULT;	//デフォルト使用法
	idxBufferDesc.ByteWidth = sizeof(idxVertexID);
	idxBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;	//インデックスバッファ
	idxBufferDesc.CPUAccessFlags = 0;
	idxBufferDesc.MiscFlags = 0;
	idxBufferDesc.StructureByteStride = sizeof(UINT);

	//インデックスバッファのサブリソースの定義
	D3D11_SUBRESOURCE_DATA idxSubData;
	idxSubData.pSysMem = idxVertexID;	//バッファデータの初期値
	idxSubData.SysMemPitch = 0;
	idxSubData.SysMemSlicePitch = 0;

	//インデックスバッファの作成
	hr = g_pD3DDevice->CreateBuffer(&idxBufferDesc, &idxSubData, &g_pIdxBuffer);
	if (FAILED(hr)){
		return false;
	}

	return true;
}

bool D3DUtil::initInputAssembler(){
	/*************************************/
	/*      入力アセンブラの設定         */
	/*************************************/

	D3D11_INPUT_ELEMENT_DESC l[2] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	layout = l;

	//IAに頂点バッファを設定
	UINT strides[2] = {
		sizeof(XYZBUFFER),		//頂点バッファ（頂点）に含まれる頂点データのサイズ
		sizeof(COLBUFFER)		//頂点バッファ（色）に含まれる頂点データのサイズ
	};

	UINT offsets[2] = {
		0,					//頂点バッファ（頂点）のオフセット
		0					//頂点バッファ（色）のオフセット
	};
	g_pImmediateContext->IASetVertexBuffers(
		0,				//設定する最初の入力スロット番号
		2,				//設定する頂点バッファの数
		g_pVerBuffer,	//設定する頂点バッファの配列
		strides,		//各頂点バッファの要素サイズ
		offsets			//各頂点バッファのオフセット
		);

	//IAにインデックスバッファを設定
	g_pImmediateContext->IASetIndexBuffer(
		g_pIdxBuffer,			//設定するインデックスバッファ
		DXGI_FORMAT_R32_UINT,	//インデックスバッファのフォーマット
		0						//インデックスバッファのオフセット
		);

	//入力レイアウトオブジェクトの作成
	HRESULT hr = g_pD3DDevice->CreateInputLayout(
		layout,							//定義の配列
		_countof(l),				//定義の要素数
		pBlobVS->GetBufferPointer(),	//バイト・コードへのポインタ
		pBlobVS->GetBufferSize(),		//バイト・コードのサイズ
		&g_pInputLayout					//受け取る変数のポインタ
		);
	if (FAILED(hr)){
		return false;
	}

	//入力レイアウトオブジェクトの設定
	g_pImmediateContext->IASetInputLayout(g_pInputLayout);

	//プリミティブの種類
	g_pImmediateContext->IASetPrimitiveTopology(
		D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST	//三角形リスト
		);

	return true;
}

bool D3DUtil::initRasterizer(){
	/****************************************/
	/*          ラスタライザの設定          */
	/****************************************/

	//ラスタライザステートオブジェクトの作成
	D3D11_RASTERIZER_DESC RSDesc;
	RSDesc.FillMode = D3D11_FILL_SOLID;	//普通の描画
	RSDesc.CullMode = D3D11_CULL_BACK;	//面の表示設定
	RSDesc.FrontCounterClockwise = FALSE;	//時計回りが表面
	RSDesc.DepthBias = 0;				//深度バイアス「0」
	RSDesc.DepthBiasClamp = 0;
	RSDesc.SlopeScaledDepthBias = 0;
	RSDesc.DepthClipEnable = FALSE;		//深度クリッピングなし
	RSDesc.ScissorEnable = FALSE;		//シザー矩形なし
	RSDesc.MultisampleEnable = FALSE;	//マルチサンプリングなし
	RSDesc.AntialiasedLineEnable = FALSE;	//ラインアンチエイリアシングなし
	HRESULT hr = g_pD3DDevice->CreateRasterizerState(
		&RSDesc,
		&g_pRasterizerState
		);
	if (FAILED(hr)){
		return false;
	}

	//RSにラスタライザステートオブジェクトを設定
	g_pImmediateContext->RSSetState(g_pRasterizerState);

	return true;
}

bool D3DUtil::initOutputMerger(){
	/**********************************/
	/*      出力マージャー設定        */
	/**********************************/

	//アルファブレンディングの有効化
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(D3D10_BLEND_DESC));
	blendDesc.AlphaToCoverageEnable = FALSE;	//アルファtoカバレッジの有無
	blendDesc.IndependentBlendEnable = FALSE;	//同時処理のレンダーターゲットにおいて独立したブレンディングを有効化するかどうか
	blendDesc.RenderTarget[0].BlendEnable = TRUE;	//ブレンディングの有効化
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	HRESULT hr = g_pD3DDevice->CreateBlendState(&blendDesc, &g_pBlendState);
	if (FAILED(hr)){
		return false;
	}

	//OMにブレンドステートオブジェクトを設定
	FLOAT BlendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	g_pImmediateContext->OMSetBlendState(
		g_pBlendState,		//設定するオブジェクト
		BlendFactor,		//定数値(RGBA)
		0xffffffff);		//サンプル用マスク

	//深度ステンシルオブジェクトの作成
	D3D11_DEPTH_STENCIL_DESC DepthStencil;
	DepthStencil.DepthEnable = TRUE;	//深度テストあり
	DepthStencil.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;	//書き込む
	DepthStencil.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;	//手前の物体を描画
	DepthStencil.StencilEnable = FALSE;	//ステンシルテストなし
	DepthStencil.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;	//ステンシル読み込みマスク
	DepthStencil.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;	//ステンシル書き込みマスク
		//面が表を向いている場合のステンシルテストの設定
	DepthStencil.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;	//維持
	DepthStencil.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;	//維持
	DepthStencil.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;	//維持
	DepthStencil.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;	//常に成功
		//面が裏を向いている場合のステンシルテストの設定
	DepthStencil.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;	//維持
	DepthStencil.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;	//維持
	DepthStencil.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;	//維持
	DepthStencil.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;	//常に成功
	hr = g_pD3DDevice->CreateDepthStencilState(
		&DepthStencil,
		&g_pDepthStencilState);
	if (FAILED(hr)){
		return false;
	}

	//OMに深度ステンシルステートオブジェクトを設定
	g_pImmediateContext->OMSetDepthStencilState(
		g_pDepthStencilState,
		0);//ステンシルテストで使う参照値

	return true;
}

bool D3DUtil::initSampler(){

	//サンプラステートオブジェクトの設定
	D3D11_SAMPLER_DESC descSampler;
	descSampler.Filter = D3D11_FILTER_ANISOTROPIC;
	descSampler.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	descSampler.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	descSampler.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	descSampler.MipLODBias = 0.0f;
	descSampler.MaxAnisotropy = 2;
	descSampler.ComparisonFunc = D3D11_COMPARISON_NEVER;
	descSampler.BorderColor[0] = 0.0f;
	descSampler.BorderColor[1] = 0.0f;
	descSampler.BorderColor[2] = 0.0f;
	descSampler.BorderColor[3] = 0.0f;
	descSampler.MinLOD = -FLT_MAX;
	descSampler.MaxLOD = FLT_MAX;

	//サンプラステートオブジェクトの作成
	HRESULT hr = g_pD3DDevice->CreateSamplerState(
		&descSampler,
		&g_pSampler);
	if (FAILED(hr)){
		return false;
	}

	//ピクセルシェーダにサンプラを設定
	g_pImmediateContext->PSSetSamplers(0, 1, &g_pSampler);


	return true;
}

void D3DUtil::clearScreen(){
	//描画ターゲットのクリア
	float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };//クリアする値(RGBA)
	g_pImmediateContext->ClearRenderTargetView(
		g_pRenderTargetView,	//クリアする描画ターゲット
		ClearColor);			//クリアする値

	//深度ステンシルのクリア
	g_pImmediateContext->ClearDepthStencilView(
		g_pDepthStencilView,	//クリアする深度ステンシルビュー
		D3D11_CLEAR_DEPTH,		//深度値だけをクリアする
		1.0f,					//深度バッファをクリアする値
		0);						//ステンシルバッファをクリアする値(この場合は無関係)
}

bool D3DUtil::loadTextrue(LPWSTR fileName,Texture* tex){
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

bool D3DUtil::drawGraph(Texture* tex,float x,float y,float scaleX,float scaleY){
	
	//PSにシェーダ―リソースビューを設定
	g_pImmediateContext->PSSetShaderResources(
		0,				//設定する最初のスロット番号
		1,				//設定するシェーダリソースビューの数
		&tex->shaerResourceView	//設定するシェーダリソースビューの配列
		);

	XMMATRIX mat = XMMatrixTranslation(x, y, 0.0f);
	XMMATRIX scale = XMMatrixScaling(tex->width*scaleX, tex->height*scaleY, 0.0f);
	
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

bool D3DUtil::drawGraphAnimate(Texture* tex, float x, float y, float scaleX, float scaleY, float su, float sv, float eu, float ev){
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

bool D3DUtil::refreshScreen(){
	//バックバッファの表示
	HRESULT hr = g_pSwapChain->Present(
		0,	//画面をすぐに更新する
		0);	//画面を実際に更新する

	return true;
}

bool D3DUtil::loadFontTextrue(LPWSTR fileName, Texture* tex){
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

bool D3DUtil::loadFontTexture(Texture* tex){
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

bool D3DUtil::drawString(float x, float y,float scaleX,float scaleY ,TCHAR* str,UINT size){
	for (UINT n = 0; n < size; n++){
		FontLoder::getCharGraph(&str[n]);
		loadFontTexture(&FontLoder::tex);
		drawGraph(&FontLoder::tex, x, y, scaleX, scaleY);
		x += FontLoder::tex.width*scaleX;
	}
	return true;
}

bool D3DUtil::drawString(float x, float y, float scaleX, float scaleY, std::wstring str){
	return drawString(x, y,scaleX,scaleY, (TCHAR*)str.c_str(), str.size());
}

int D3DUtil::getWindowY(){
	return windowHeight;
}

bool D3DUtil::changeScreenMode(){
	BOOL isFullScreen = false;
	g_pSwapChain->GetFullscreenState(&isFullScreen,0);
	if (isFullScreen){
		isFullScreen = false;
	}
	else{
		isFullScreen = true;
	}
	g_pSwapChain->SetFullscreenState(isFullScreen, 0);

	return true;
}

bool D3DUtil::getScreenMode(){
	BOOL isFullScreen = false;
	g_pSwapChain->GetFullscreenState(&isFullScreen, 0);
	return isFullScreen;
}

bool D3DUtil::changeSwapChain(){
	HRESULT hr;
	//描画ターゲットを解除する
	g_pImmediateContext->OMSetRenderTargets(0, NULL, NULL);
	SAFE_RELEASE(g_pRenderTargetView);
	//バッファの変更
	hr = g_pSwapChain->ResizeBuffers(1,
		windowWidth,
		windowHeight,
		DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);
	if (FAILED(hr)){
		DebugBreak();
	}
	//initDeviceAndSwapChain();
	//バッファの再設定
	if (!initBackBuffer()){
		DebugBreak();
	}
	//ビューポートの再設定
	if (!initViewPort()){
		DebugBreak();
	}
	//深度・ステンシルの再設定
	if (!initDepthAndStencil()){
		DebugBreak();
	}
	return true;
}

bool D3DUtil::changeDisplayMode(int x, int y){
	RECT clientRect;
	GetClientRect(*window, &clientRect);
	windowWidth = clientRect.right;
	windowHeight = clientRect.bottom;
	//ディスプレイモードの変更
	DXGI_MODE_DESC desc;
	desc.Width = windowWidth;
	desc.Height = windowHeight;
	desc.RefreshRate.Numerator = 60;
	desc.RefreshRate.Denominator = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	desc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	HRESULT hr;
	hr = g_pSwapChain->ResizeTarget(&desc);
	if (FAILED(hr)){
		return false;
	}
}

bool D3DUtil::changeFullScreen(int x,int y){
	changeDisplayMode(x, y);
	//changeSwapChain(x, y);
	//changeScreenMode();
	return true;
}

HWND*						D3DUtil::window;
ID3D11Device*				D3DUtil::g_pD3DDevice;		//デバイス
ID3D11DeviceContext*		D3DUtil::g_pImmediateContext;	//デバイスコンテキスト
IDXGISwapChain*				D3DUtil::g_pSwapChain;		//スワップチェイン
ID3D11Texture2D*			D3DUtil::g_pDepthStencil;			//深度/ステンシルテクスチャを受け取る変数
ID3D11DepthStencilView*		D3DUtil::g_pDepthStencilView;		//深度/ステンシルビュー

ID3D11VertexShader*			D3DUtil::g_pVertexShader;		//頂点シェーダオブジェクト
ID3D11GeometryShader*		D3DUtil::g_pGeometryShader;	//ジオメトリシェーダオブジェクト
ID3D11GeometryShader*		D3DUtil::g_pAnimateGeometryShader;	//文字用ジオメトリシェーダ
ID3D11PixelShader*			D3DUtil::g_pPixelShader;		//ピクセルシェーダオブジェクト

ID3DBlob*					D3DUtil::pBlobVS;				//頂点シェーダ格納用変数
ID3DBlob*					D3DUtil::pBlobGS;				//ジオメトリシェーダ格納用変数
ID3DBlob*					D3DUtil::pBlobPS;				//ピクセルシェーダ―格納用変数

//定数バッファのデータ
CbNeverChanges				D3DUtil::g_cbNeverChanges;	//透視変換行列
CbChangesEveryFrame			D3DUtil::g_cbChangesEveryFrame;	//ビュー変換行列　光源座標
CbChangesEveryObject		D3DUtil::g_cbChangesEveryObject;	//ワールド変換行列
ID3D11Buffer*				D3DUtil::g_pCBuffer[3]; //= { NULL, NULL, NULL };	//定数バッファ
D3D11_MAPPED_SUBRESOURCE	D3DUtil::MappedResource;

//ラスタライザステートオブジェクト
ID3D11RasterizerState*		D3DUtil::g_pRasterizerState;

//ブレンドステートオブジェクト
ID3D11BlendState*			D3DUtil::g_pBlendState;

//深度ステンシルステートオブジェクト
ID3D11DepthStencilState*	D3DUtil::g_pDepthStencilState;

//入力要素配列
D3D11_INPUT_ELEMENT_DESC*	D3DUtil::layout;

//入力レイアウトオブジェクト
ID3D11InputLayout*			D3DUtil::g_pInputLayout;

ID3D11Buffer*				D3DUtil::g_pVerBuffer[2];// = { NULL, NULL };	//頂点バッファのインターフェース
ID3D11Buffer*				D3DUtil::g_pIdxBuffer;		//インデックスバッファのインターフェース

//サンプラステートオブジェクト
ID3D11SamplerState*			D3DUtil::g_pSampler;

//頂点バッファ
XYZBUFFER*					D3DUtil::posVertex;
//カラーバッファ
COLBUFFER*					D3DUtil::colVertex;
//点光源座標
XMFLOAT3					D3DUtil::g_vLightPos;

//ウィンドウの大きさ
float						D3DUtil::windowWidth;
float						D3DUtil::windowHeight;

//ウィンドウモード
bool						D3DUtil::windowMode;

//デフォルトフォント
//FontLoder*					D3DUtil::defFont;

ID3D11RenderTargetView*		D3DUtil::g_pRenderTargetView;	//描画ターゲットビュー