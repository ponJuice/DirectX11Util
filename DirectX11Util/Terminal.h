#pragma once
#pragma comment(lib,"d3d11.lib")
#if defined(DEBUG) || defined(_DEBUG)
#pragma comment(lib,"d3dx11d.lib")
#else
#pragma comment(lib,"d3dx11.lib")
#endif
#pragma comment(lib,"dxerr.lib")

#pragma comment(lib, "winmm.lib")	//timeGetTime()�p

#define SAFE_RELEASE(x){if (x){(x)->Release();(x)=NULL;}}	//�C���^�[���[�X����悤�}�N��

#include <Windows.h>
#include <D3DX11.h>
#include <DxErr.h>
#include <DirectXMath.h>
#include <string>