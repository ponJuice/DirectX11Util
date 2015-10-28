#pragma once
#pragma comment(lib,"d3d11.lib")
#if defined(DEBUG) || defined(_DEBUG)
#pragma comment(lib,"d3dx11d.lib")
#else
#pragma comment(lib,"d3dx11.lib")
#endif
#pragma comment(lib,"dxerr.lib")

#pragma comment(lib, "winmm.lib")	//timeGetTime()用

#define SAFE_RELEASE(x){if (x){(x)->Release();(x)=NULL;}}	//インターレース解放ようマクロ

#include <Windows.h>
#include <D3DX11.h>
#include <DxErr.h>
#include <DirectXMath.h>
#include <string>