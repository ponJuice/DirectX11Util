#pragma once
#include "Terminal.h"

//プロトタイプ宣言？
class Texture;	
class FontLoder;

//定数バッファのデータ定義　変更しないデータ
typedef struct cbNeverChanged{
	XMFLOAT4X4 Projection;
}CbNeverChanges;

//定数バッファのデータ定義　変更頻度の低いデータ
typedef struct cbChangesEveryFrame{
	XMFLOAT4X4 View;	//ビュー変換行列
	XMFLOAT3 Light;		//光源座標（ワールド座標系）
	FLOAT dummy;		//定数バッファのサイズを16の倍数にするためのダミー
}CbChangesEveryFrame;

//定数バッファのデータ定義　変更頻度の高いデータ
typedef struct cbChangesEveryObject{
	XMFLOAT4X4 World;	//ワールド変換行列
	XMFLOAT4	Seek;	//テクスチャシーク
}CbChangesEveryObject;

//頂点バッファ（頂点）に収めるデータ形式
typedef struct XYZBuffer{
	XMFLOAT3 Position;	//座標値
}XYZBUFFER;

//頂点バッファ（色）に収めるデータ形式
typedef struct ColBuffer{
	XMFLOAT3 Color;
}COLBUFFER;

class D3DUtil
{
private:
	static HWND*					window;
	static ID3D11Device*			g_pD3DDevice;		//デバイス
	static ID3D11DeviceContext*		g_pImmediateContext;	//デバイスコンテキスト
	static IDXGISwapChain*			g_pSwapChain;		//スワップチェイン
	static ID3D11Texture2D*			g_pDepthStencil;			//深度/ステンシルテクスチャを受け取る変数
	static ID3D11DepthStencilView*	g_pDepthStencilView;		//深度/ステンシルビュー

	static ID3D11VertexShader*		g_pVertexShader;		//頂点シェーダオブジェクト
	static ID3D11GeometryShader*	g_pGeometryShader;	//ジオメトリシェーダオブジェクト
	static ID3D11GeometryShader*	g_pAnimateGeometryShader;	//文字用ジオメトリシェーダ
	static ID3D11PixelShader*		g_pPixelShader;		//ピクセルシェーダオブジェクト

	static ID3DBlob*				pBlobVS;				//頂点シェーダ格納用変数
	static ID3DBlob*				pBlobGS;				//ジオメトリシェーダ格納用変数
	static ID3DBlob*				pBlobPS;				//ピクセルシェーダ―格納用変数

	//定数バッファのデータ
	static CbNeverChanges			g_cbNeverChanges;	//透視変換行列
	static CbChangesEveryFrame		g_cbChangesEveryFrame;	//ビュー変換行列　光源座標
	static CbChangesEveryObject		g_cbChangesEveryObject;	//ワールド変換行列
	static ID3D11Buffer*			g_pCBuffer[3]; //= { NULL, NULL, NULL };	//定数バッファ
	static D3D11_MAPPED_SUBRESOURCE MappedResource;

	//ラスタライザステートオブジェクト
	static ID3D11RasterizerState*	g_pRasterizerState;

	//ブレンドステートオブジェクト
	static ID3D11BlendState*		g_pBlendState;

	//深度ステンシルステートオブジェクト
	static ID3D11DepthStencilState* g_pDepthStencilState;

	//入力要素配列
	static D3D11_INPUT_ELEMENT_DESC* layout;

	//入力レイアウトオブジェクト
	static ID3D11InputLayout*		g_pInputLayout;

	static ID3D11Buffer*			g_pVerBuffer[2];// = { NULL, NULL };	//頂点バッファのインターフェース
	static ID3D11Buffer*			g_pIdxBuffer;		//インデックスバッファのインターフェース

	//サンプラステートオブジェクト
	static ID3D11SamplerState*		g_pSampler;

	//頂点バッファ
	static XYZBUFFER*				posVertex;
	//カラーバッファ
	static COLBUFFER*				colVertex;
	//点光源座標
	static XMFLOAT3					g_vLightPos;

	//ウィンドウの大きさ
	static float					windowWidth;
	static float					windowHeight;

	//ウィンドウモード
	static bool						windowMode;
		
	//デフォルトフォント
	//static FontLoder*				defFont;

	static ID3D11RenderTargetView*	g_pRenderTargetView;	//描画ターゲットビュー

	//D3D11_SHADER_RESOURCE_VIEW_DESC srDesc;

public:
	/*******Directx11初期化******/
	static bool initD3DUtil(HWND* targetWindow, float l_windowWidth, float l_windowHeight, bool fullScreen);
	/*******画像ロード系*******/
	static bool loadTextrue(LPWSTR fileName, Texture* tex);
	static bool loadFontTextrue(LPWSTR fileName, Texture* tex);
	static bool loadFontTexture(Texture* tex);
	/*******描画系*******/
	static bool drawGraph(Texture* tex, float x, float y, float scaleX, float scaleY);
	static bool drawString(Texture* tex, float x, float y, const wchar_t* str, int size);
	static bool drawString(float x, float y, float scaleX, float scaleY, TCHAR* str, UINT size);
	//bool drawString(FontLoder* font, float x, float y, float scaleX, float scaleY, std::wstring str);
	static bool drawString(float x, float y, float scaleX, float scaleY, std::wstring str);
	static bool drawGraphAnimate(Texture* tex, float x, float y, float scaleX, float scaleY, float su, float sv, float eu, float ev);
	//bool drawString(float x, float y, float scaleX, float scaleY, std::wstring str);
	/*******フォント系*******/
	static bool initFont(LPCWSTR fontName, UINT fontSize);
	/******画面更新系********/
	static bool refreshScreen();
	static void clearScreen();
	static bool changeScreenMode();
	static bool changeDisplayMode(int x, int y);
	static bool changeSwapChain();
	static bool changeFullScreen(int x, int y);
	static bool getScreenMode();
	/**************************/
	static bool resizeBuffer();
	static int getWindowY();
	/***********終了処理************/
	static void CleanupDirect3D();

private:
	static bool initDeviceAndSwapChain();
	static bool initBackBuffer();
	static bool initViewPort();
	static bool initDepthAndStencil();
	static void initDispray();
	static bool isDeviceDisappearance();
	static bool InitDirect3D();
	static bool initShaderAndConstantBuffer();
	static bool initConstantBuffer();
	static bool initVertexBuffer();
	static bool initInputAssembler();
	static bool initRasterizer();
	static bool initOutputMerger();
	static bool initSampler();
private:
	D3DUtil();
	~D3DUtil();
};

